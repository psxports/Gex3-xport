#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <string.h>

/* Three original game slots share one atomic native save file */
enum { G3_SLOT_BYTES = 0xe4, G3_SLOT_COUNT = 3, G3_SAVE_BYTES = 8 + G3_SLOT_BYTES * G3_SLOT_COUNT };

sint32 G3NativeAutosaveGame(uint32 game, uint32 guest_stack)
{
    uint32 state = r_u32(game + 0x67e8);
    if (r_u8(game + 0x68f8) != 0 || r_u32(state + 0x38) == 0xffffffff)
        return 0;
    sint32 result = G3NativeSaveGame(state, guest_stack);
    w_u32(state + 0x40, (uint32)result);
    return result;
}

static sint32 G3ReadSaveSlots(uint8 *slots)
{
    uint8 stored[G3_SAVE_BYTES];
    sint32 bytes = xport_load(stored, sizeof(stored));
    memset(slots, 0, G3_SAVE_BYTES);
    memcpy(slots, "G3S3\1\0\0\0", 8);
    if (bytes <= 0)
        return bytes;
    if (bytes == G3_SLOT_BYTES)
    {
        memcpy(slots + 8, stored, G3_SLOT_BYTES);
        return 1;
    }
    if (bytes != G3_SAVE_BYTES || memcmp(stored, "G3S3\1\0\0\0", 8) != 0)
        return -2;
    memcpy(slots, stored, G3_SAVE_BYTES);
    return 1;
}

static sint32 G3ValidateNativeSlot(uint32 record)
{
    if (r_u16(record) == 0)
        return 0;
    if (r_u32(record + 4 + 0xdc) != 0xabacabcd)
        return -2;
    uint32 checksum = G3CardChecksum80067C48(record + 4, 0xe0);
    return (checksum & 0xffff) == r_u16(record + 2) ? 1 : -2;
}

sint32 G3NativeWriteSlot(uint32 state, uint32 record)
{
    uint32 selected = r_u32(state + 0x38);
    if (selected >= G3_SLOT_COUNT)
        return -2;
    uint8 slots[G3_SAVE_BYTES];
    sint32 result = G3ReadSaveSlots(slots);
    if (result < 0)
        return result;
    memcpy(slots + 8 + selected * G3_SLOT_BYTES, psx_addr(record, G3_SLOT_BYTES), G3_SLOT_BYTES);
    return xport_save(slots, sizeof(slots));
}

sint32 G3NativeReadSlot(uint32 state, uint32 record)
{
    uint32 selected = r_u32(state + 0x38);
    if (selected >= G3_SLOT_COUNT)
        return -2;
    uint8 slots[G3_SAVE_BYTES];
    sint32 result = G3ReadSaveSlots(slots);
    if (result <= 0)
        return result;
    memcpy(psx_addr(record, G3_SLOT_BYTES), slots + 8 + selected * G3_SLOT_BYTES, G3_SLOT_BYTES);
    return G3ValidateNativeSlot(record);
}

uint32 G3NativeSaveSlotsPage(uint32 context, uint32 selected, uint32 third_glyph, uint32 guest_stack)
{
    uint32 frame = guest_stack - 0x120;
    uint32 state = G3Function8006A600(context);
    uint32 mode = r_u32(state + 0xc);
    uint8 slots[G3_SAVE_BYTES];
    sint32 status = G3ReadSaveSlots(slots);
    if (status < 0)
    {
        w_u32(state + 0x40, (uint32)status);
        return G3NativeSaveResultPage(context, 0x800699ac, third_glyph, frame);
    }
    uint32 title = G3Function80065D38(mode == 1 ? 0x28 : 0x30);
    G3Function8006AAD4(context, title, third_glyph, frame);
    uint32 result = 0;
    for (uint32 slot = 0; slot < G3_SLOT_COUNT; ++slot)
    {
        uint32 record = frame + 0x20;
        memcpy(psx_addr(record, G3_SLOT_BYTES), slots + 8 + slot * G3_SLOT_BYTES, G3_SLOT_BYTES);
        sint32 valid = G3ValidateNativeSlot(record);
        uint32 text;
        if (valid == 1)
        {
            G3Function8006B77C(state, record + 4, frame);
            text = G3Function80068BA4(state);
        }
        else
            text = G3Function80065D38(valid == 0 ? 11 : 12);
        uint32 callback = mode == 1 ? (valid == 0 ? 0x80068ec0 : 0x80068e2c) : valid == 1 ? 0x80068f1c : 0;
        result = G3Function80069A4C(context, slot, text, callback, frame);
    }
    uint32 back = G3Function80065D38(8);
    result = G3Function80069A4C(context, G3_SLOT_COUNT, back, 0x800699ac, frame);
    if ((sint32)selected < 0 || selected > G3_SLOT_COUNT)
        selected = 0;
    return G3Function80069BC8(context, selected, 0, G3_SLOT_COUNT + 1, result, frame);
}

static uint32 G3NativeChooseSaveSlot(uint32 context, uint32 selected, uint32 event, uint32 guest_stack, sint32 load)
{
    if (event != 7 || selected >= G3_SLOT_COUNT)
        return 7;
    uint32 frame = guest_stack - 0x20;
    uint32 state = G3Function8006A600(context);
    G3Function8006A654(3, 0);
    uint32 previous_slot = r_u32(state + 0x38);
    uint32 previous_elapsed = r_u32(state + 0x3c);
    w_u32(state + 0x38, selected);
    if (!load)
        w_u32(state + 0x3c, 0);
    sint32 result = load ? G3NativeLoadGame(state, frame) : G3NativeSaveGame(state, frame);
    w_u32(state + 0x38, selected);
    w_u32(state + 0x40, (uint32)result);
    if (result == 1)
        return G3Function80068BE8(context, frame);
    w_u32(state + 0x38, previous_slot);
    w_u32(state + 0x3c, previous_elapsed);
    return G3Function800698C4(context, 0x8006954c, 0, 0x80068e98, (uint32)result, frame);
}

uint32 G3Function80068EC0(uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    return G3NativeChooseSaveSlot(context, selected, event, guest_stack, 0);
}

uint32 G3Function80068F1C(uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    return G3NativeChooseSaveSlot(context, selected, event, guest_stack, 1);
}


uint32 G3Function80068D90(uint32 context, uint32 selected, uint32 incoming_third_glyph, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80068d90, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 text = G3Function80065D38(9);
    G3Function8006AAD4(context, text, incoming_third_glyph, frame);
    text = G3Function80065D38(10);
    G3Function80069A4C(context, 0, text, 0x80068d38, frame);
    text = G3Function80065D38(8);
    uint32 result = G3Function80069A4C(context, 1, text, 0x800699ac, frame);
    if ((sint32)selected < 0)
        selected = 1;
    return G3Function80069BC8(context, selected, 0, 2, result, frame);
}


uint32 G3Function80068E2C(uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80068e2c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    if (event != 7)
        return 7;
    uint32 state = G3Function8006A600(context);
    uint32 result = G3Function8006A654(3, 0);
    w_u32(state + 0x44, selected);
    return G3Function800698C4(context, 0x80068d90, 0xffffffff, 0, result, frame);
}

uint32 G3Function80068D38(uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    if (event != 7)
        return 7;
    uint32 frame = guest_stack - 0x18;
    uint32 state = G3Function8006A600(context);
    uint32 slot = r_u32(state + 0x44);
    G3Function80069918(context, 0, frame);
    return G3NativeChooseSaveSlot(context, slot, event, frame, 0);
}
