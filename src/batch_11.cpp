#include "psx.h"
#include "xport.h"
#include "psx_spu.h"
#include "game_functions.h"

void G3Function80028F78(void)
{
    FUNCTION_MARKER(0x80028f78, "SLUS_008.06");
    w_u32(0x800a0c8c, 0);
}

void G3Function80081010(void)
{
    FUNCTION_MARKER(0x80081010, "SLUS_008.06");
    w_u32(0x800a033c, 0);
}

void G3Function8004D8B0(uint32 camera, uint32 value)
{
    FUNCTION_MARKER(0x8004d8b0, "SLUS_008.06");
    w_u32(camera + 0x88, value);
}

uint32 G3Function8004DF4C(uint32 enabled, uint32 incoming_v0)
{
    FUNCTION_MARKER(0x8004df4c, "SLUS_008.06");
    w_u32(0x80096b6c, enabled);
    return incoming_v0;
}

uint32 G3Function8002ABB8(uint32 records, uint32 count, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002abb8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 end = records + count * 52;
    while (records < end)
    {
        uint32 descriptor = r_u32(records);
        if (descriptor != 0)
        {
            uint32 target = r_u32(descriptor + 0x34);
            if (target != 0)
            {
                uint32 game = r_u32(0x80097048);
                G3DispatchStateEvent(target, 0, game, 5, records, frame);
            }
        }
        records += 52;
    }
    return 0;
}

uint32 G3Function8002AC44(uint32 game)
{
    FUNCTION_MARKER(0x8002ac44, "SLUS_008.06");
    uint32 level = r_u32(game + 0x48);
    uint32 root = r_u32(level);
    sint32 count = r_s32(level + 0x7c);
    uint32 record = r_u32(root + 0x14);
    if (count <= 0)
        return level;
    for (sint32 index = 0; index < count; ++index, record += 0x34)
    {
        uint32 flags = r_u32(record + 0x18);
        if ((flags & 0x10) != 0)
        {
            uint32 descriptor = r_u32(record);
            if (descriptor != 0 && (r_u32(descriptor) & 0x100000) == 0)
                w_u32(record + 0x18, (flags & 0xffffffef) | 0x4000);
        }
    }
    return 0;
}

uint32 G3Function8002ACC8(uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002acc8, "SLUS_008.06");
    uint32 result = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x20;
    uint32 pool = r_u32(result + 0x5c);
    uint32 object = r_u32(pool + 4);
    while (object != 0)
    {
        uint32 game = r_u32(0x80097048);
        result = r_u32(game + 0x50);
        uint32 next = r_u32(object + 8);
        if (object != result)
            result = G3Function8002937C(pool, object, frame);
        object = next;
    }
    return result;
}

void G3Function80039B18(uint32 level, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80039b18, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 entry = r_u32(level + 0xec);
    while (entry != 0)
    {
        uint32 target = r_u32(entry + 0x0c);
        if (target == 0)
            break;
        uint32 game = r_u32(0x80097048);
        G3DispatchStateEvent(target, 0, game, 11, 0, frame);
        entry = r_u32(entry + 0x14);
    }
}

uint32 G3Function8002937C(uint32 pool, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002937c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function800292F0(pool, object, frame);
    G3Function80045BA0(object);
    uint32 placement = r_u32(object + 0x20);
    w_u32(object + 0x38, 0xffffffff);
    if (placement != 0)
    {
        uint32 flags = r_u32(placement + 0x18);
        w_u32(placement + 0x18, flags & 0xfffffff7);
        placement = r_u32(object + 0x20);
        w_u32(placement + 0x20, 0);
    }
    uint32 target = r_u32(object + 0xec);
    if (target != 0)
    {
        uint32 game = r_u32(0x80097048);
        G3DispatchStateEvent(target, object, game, 4, 0, frame);
    }
    if (object != 0 && (r_u32(object + 0x10) & 2) != 0)
    {
        uint32 parent = r_u32(object + 0x2c);
        if (parent != 0)
        {
            uint32 identity = r_u32(object + 0x3c);
            uint32 parent_identity = r_u32(parent + 0x38);
            if (identity == parent_identity)
            {
                target = r_u32(parent + 0xec);
                if (target != 0)
                {
                    uint32 game = r_u32(0x80097048);
                    G3DispatchStateEvent(target, parent, game, 16, object, frame);
                }
            }
        }
    }
    uint32 effect = r_u32(object + 0xe4);
    if (effect != 0)
        w_u16(effect + 0x22, 0);
    effect = r_u32(object + 0xe8);
    if (effect != 0)
        w_u16(effect + 0x22, 0);
    uint32 descriptor = r_u32(object + 0x1c);
    if ((r_u32(descriptor + 0x30) & 0x1000) != 0)
    {
        uint32 flags = r_u32(object + 0x14);
        if ((flags & 0x2000) != 0)
        {
            uint32 handle = r_u32(object + 0x44);
            w_u32(object + 0x14, flags & 0xffffdfff);
            G3Function8003F18C(handle);
            w_u32(object + 0x44, 0);
        }
        flags = r_u32(object + 0x14);
        if ((flags & 0x10000) != 0)
        {
            uint32 handle = r_u32(object + 0x50);
            G3Function8003F18C(handle);
            flags = r_u32(object + 0x14);
            w_u32(object + 0x14, flags & 0xfffcffff);
        }
    }
    uint32 player = r_u32(0x8009efc4);
    uint32 private_state = r_u32(player + 0x24);
    if (r_u32(private_state + 0x1e8) == object)
        w_u32(private_state + 0x1e8, 0);
    if (r_u32(private_state + 0xec) == object)
        w_u32(private_state + 0xec, 0);
    if (r_u32(private_state + 0x1fc) == object)
        w_u32(private_state + 0x1fc, 0);
    descriptor = r_u32(object + 0x1c);
    uint32 result = r_u32(descriptor + 0x10);
    if (result != 0)
        result = G3Function800305B0(object, result, frame);
    return result;
}

uint32 G3Function800305B0(uint32 object, uint32 incoming_v0, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800305b0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 head = r_u32(object + 0x98);
    uint32 result = G3Function800309E8(head, incoming_v0, frame);
    head = r_u32(object + 0x94);
    return G3Function800309A4(head, result, frame);
}

uint32 G3Function80018E00(uint32 writes, uint32 incoming_v0)
{
    FUNCTION_MARKER(0x80018e00, "SLUS_008.06");
    if (writes == 0)
        return incoming_v0;
    uint32 result = r_u32(writes);
    while (result != 0)
    {
        uint32 destination = r_u32(writes);
        uint32 value = r_u32(writes + 4);
        writes += 8;
        w_u32(destination, value);
        result = r_u32(writes);
    }
    return result;
}

uint32 G3Function80076418(uint32 pool, uint32 entry)
{
    FUNCTION_MARKER(0x80076418, "SLUS_008.06");
    uint32 base = r_u32(pool + 0x0c);
    uint32 stride = r_u16(pool);
    sint32 difference = (sint32)(entry - base);
    uint32 index = stride == 0 ? (difference < 0 ? 1 : 0xffffffff) : (uint32)(difference / (sint32)stride);
    uint32 count = r_u16(pool + 2);
    uint32 indices = r_u32(pool + 8);
    --count;
    w_u16(pool + 2, count);
    uint32 destination = indices + ((count & 0xffff) << 1);
    w_u16(destination, index);
    return destination;
}

uint32 G3Function800309E8(uint32 head, uint32 incoming_v0, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800309e8, "SLUS_008.06");
    uint32 result = incoming_v0;
    while (head != 0)
    {
        uint32 next = r_u32(head);
        result = G3Function80076418(0x800a1550, head);
        head = next;
    }
    return result;
}

uint32 G3Function800309A4(uint32 head, uint32 incoming_v0, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800309a4, "SLUS_008.06");
    uint32 result = incoming_v0;
    while (head != 0)
    {
        uint32 next = r_u32(head);
        result = G3Function80076418(0x800a1560, head);
        head = next;
    }
    return result;
}

uint32 G3Function800150A8(uint32 guest_stack)
{
    FUNCTION_MARKER(0x800150a8, "SLUS_008.06");
    for (uint32 entry = 0x800a08e0; entry < 0x800a0b60; entry += 0x28)
        w_u32(entry, 0);
    for (uint32 entry = 0x800a0b60; entry < 0x800a0c20; entry += 12)
    {
        w_u32(entry + 4, 0);
        w_u32(entry + 8, 0);
    }
    for (uint32 entry = 0x800a0860; entry < 0x800a08e0; entry += 16)
    {
        w_u16(entry + 4, 0);
        w_u32(entry + 8, 0);
        w_u16(entry, 0);
        w_u16(entry + 2, 0);
    }
    for (uint32 i = 0; i < 56; ++i)
        w_u32(0x800a083c - i * 4, 0);
    uint32 player = r_u32(0x8009efc4);
    uint32 words[2];
    for (uint32 i = 0; i < 2; ++i)
    {
        uint32 source = player + 0x58 + i * 4;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
        words[i] = low_lane == 0 ? low : (high << ((4 - low_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 i = 0; i < 2; ++i)
    {
        w_masked_u32(0x8009d344 + i * 4, words[i], 15);
        w_masked_u32(0x8009d344 + i * 4, words[i], 15);
    }
    return player;
}

uint32 G3Function800151AC(uint32 guest_stack)
{
    FUNCTION_MARKER(0x800151ac, "SLUS_008.06");
    for (uint32 i = 0; i < 8; ++i)
        w_u32(0x800a0840 + i * 4, 0);
    return 0;
}

uint32 G3Function800359EC(uint32 key, uint32 enabled, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800359ec, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 admitted = G3Function800352AC((uint32)((sint32)(enabled << 16) >> 16));
    if (admitted == 0)
        return 0x800b0000;
    uint32 count = r_u32(0x800aa7d4);
    if (count < 2 && r_u8(0x800aa7d2) == 0)
    {
        w_u8(0x800aa7d2, 1);
        G3Function8003592C(key, 0xffffffff, frame);
        w_u32(0x800aa7d8, 0xffffffff);
        w_u8(0x800aa7d3, enabled);
        return 0xffffffff;
    }
    return G3Function80035594(key, enabled, frame);
}

uint32 G3Function800352AC(uint32 mode)
{
    FUNCTION_MARKER(0x800352ac, "SLUS_008.06");
    sint32 signed_mode = (sint32)(mode << 16) >> 16;
    uint32 game = r_u32(0x80097048);
    if ((r_u32(game + 0xe8) & 0x80000) == 0)
        return 0;
    uint32 offset = signed_mode == 3 ? 0x67dd : 0x67de;
    if (r_u8(game + offset) == 0)
        return 0;
    return r_u8(0x800aa7d0) == 0 ? 1 : 0;
}

uint32 G3Function80035594(uint32 key, uint32 mode, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80035594, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 node = r_u32(0x800ad010);
    while (node != 0)
    {
        if ((uint32)r_s16(node + 8) == key)
            return 0x800b0000;
        node = r_u32(node + 4);
    }
    node = G3Function80045C0C(0x800ad004, frame);
    if (node == 0)
        return 0;
    w_u16(node + 8, key);
    w_u16(node + 10, mode);
    G3Function80045B74(0x800ad00c, node);
    uint32 count = r_u32(0x800acff0) + 1;
    w_u32(0x800acff0, count);
    return count;
}

uint32 G3Function8003592C(uint32 key, uint32 index, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003592c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    w_u16(0x800ad0ca, key);
    uint32 selected = G3Function80035784(key, index, frame);
    if (selected == 0xffffffff)
    {
        w_u8(0x800aa7d2, 0);
        return 0xffffffff;
    }
    uint32 slot = 0x8009822c + (key << 2);
    uint32 row = r_u32(slot);
    uint32 sector = (uint32)r_s16(row + (selected << 1) + 2) & 0xffffbfff;
    w_u32(0x800aa7e0, sector);
    row = r_u32(slot);
    w_u32(0x800aa7d8, key);
    w_u32(0x800aa7d4, 1);
    w_u32(0x800ad000, row + (selected << 1) + 2);
    uint32 request = G3Function800653CC(0x8009da00, sector, 0x800aa7ec, 5, frame);
    w_u32(0x800aa7e8, request);
    w_u32(0x800acff4, 5);
    w_u16(0x800ad0cc, selected);
    return 5;
}

uint32 G3Function80035784(uint32 key, uint32 index, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80035784, "SLUS_008.06");
    uint32 slot = 0x8009822c + (key << 2);
    uint32 row = r_u32(slot);
    sint32 count = r_s16(row);
    if (count == 0)
        return 0xffffffff;
    if (index == 0xffffffff)
    {
        sint32 random = (sint32)G3Function80027FDC();
        index = (uint32)((sint64)random % (sint64)count);
    }
    row = r_u32(slot);
    uint32 marked = r_u16(row + (index << 1) + 2) & 0x4000;
    if (marked == 0)
        return index;
    uint32 remaining = (uint32)count - 1;
    while (remaining != 0xffffffff)
    {
        sint32 next = (sint32)(index + 1);
        index = (uint32)((sint64)next % (sint64)count);
        row = r_u32(slot);
        marked = r_u16(row + (index << 1) + 2) & 0x4000;
        --remaining;
        if (marked == 0)
            return index;
    }
    remaining = (uint32)count - 1;
    while (remaining != 0xffffffff)
    {
        row = r_u32(slot);
        uint32 address = row + (remaining << 1) + 2;
        uint32 value = r_u16(address);
        --remaining;
        w_u16(address, value & 0xffffbfff);
    }
    return index;
}

uint32 G3Function800653CC(uint32 filename, uint32 sector, uint32 destination, uint32 mode, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800653cc, "SLUS_008.06");
    uint32 index = G3FindArchiveFile800654AC(filename);
    uint32 record = 0x800ad0d4 + index * 24;
    sint32 offset = r_s32(record + 0xe8);
    uint32 start = (uint32)(offset >> 11) + sector;
    return G3QueueRead800652C8(start, destination, destination, mode, 0, 0, 0);
}

uint32 G3Function80035648(uint32 key_output, uint32 mode_output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80035648, "SLUS_008.06");
    uint32 node = r_u32(0x800ad010);
    if (node == 0)
        return 0;
    uint32 next = r_u32(node + 4);
    while (next != 0)
    {
        node = r_u32(node + 4);
        next = r_u32(node + 4);
    }
    if (key_output != 0)
        w_u32(key_output, (uint32)r_s16(node + 8));
    if (mode_output != 0)
        w_u32(mode_output, (uint32)r_s16(node + 10));
    G3Function80045BA0(node);
    uint32 count = r_u32(0x800acff0);
    w_u32(0x800acff0, count - 1);
    return 1;
}

uint32 G3Function800358C0(uint32 key, uint32 divisor, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800358c0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    if (G3Function8003525C() == 0)
        return 0x800b0000;
    uint32 result = r_u32(0x800aa7d4);
    if (result != 0)
        return result;
    result = G3Function80027FDC();
    uint32 remainder = divisor == 0 ? result : (uint32)((sint64)(sint32)result % (sint64)(sint32)divisor);
    if (remainder == 0)
        result = G3Function800359EC(key, 0, frame);
    return result;
}

uint32 G3Function8003525C(void)
{
    FUNCTION_MARKER(0x8003525c, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    if ((r_u32(game + 0xe8) & 0x80000) == 0)
        return 0;
    if (r_u8(game + 0x67de) == 0)
        return 0;
    return r_u8(0x800aa7d0) == 0 ? 1 : 0;
}

uint32 G3Function80035AD4(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80035ad4, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x28;
    if ((r_u32(game + 0x69b8) & 0x20000) != 0)
    {
        G3Function80020CBC(0x8009da1c, 0, 0, 0, frame, 0);
        uint32 key = (uint32)r_s16(0x800ad0ca);
        uint32 voice = (uint32)r_s16(0x800ad0cc);
        G3Function80020CBC(0x8009da28, key, voice, 0, frame, 0);
    }
    uint32 state = r_u32(0x800aa7d4);
    switch (state)
    {
    case 0:
        if (r_u8(0x800aa7d1) != 0)
        {
            uint32 paused = r_u8(0x800aa7d0);
            w_u8(0x800aa7d1, 0);
            w_u8(0x800aa7d0, paused + 1);
        }
        else
        {
            game = r_u32(0x80097048);
            if (r_s16(game + 0x6786) != 6 && G3Function80035648(frame + 0x10, frame + 0x14, frame) != 0)
            {
                uint32 key = r_u32(frame + 0x10);
                uint32 mode = r_u32(frame + 0x14);
                G3Function800359EC(key, mode, frame);
            }
        }
        break;
    case 1:
    {
        G3CdPump80064F44();
        uint32 request = r_u32(0x800aa7e8);
        if (G3Function80065650(request) == 0)
        {
            w_u32(0x800aa7d4, 2);
            SpuSetTransferStartAddr(0x1010);
            SpuWrite(psx_addr(0x800aafec, 0x2000), 0x2000);
        }
        break;
    }
    case 2:
        if (SpuIsTransferCompleted(0) != 0)
        {
            if (r_u8(0x800aa7d2) != 0)
                G3Function800353B8(frame);
            else
                w_u32(0x800aa7d4, 0);
        }
        break;
    case 3:
    {
        game = r_u32(0x80097048);
        uint32 now = r_u32(game + 0x154);
        uint32 started = r_u32(0x800aa7e4);
        uint32 elapsed = now - started;
        uint32 elapsed_half = (elapsed >> 1) + 1;
        uint32 length = r_u8(0x800aa7f0) + 1;
        if ((sint32)elapsed_half >= (sint32)length)
        {
            uint32 voices = r_u32(0x800ad074);
            G3Function8004999C(0, voices);
            w_u32(0x800aa7d4, 4);
            break;
        }
        uint32 transfer_time = r_u32(0x800acffc);
        if (transfer_time < elapsed && SpuIsTransferCompleted(0) != 0)
        {
            uint32 request = r_u32(0x800aa7e8);
            if (G3Function80065650(request) == 0)
            {
                uint32 bank = r_u32(0x800acff8);
                SpuSetTransferStartAddr((bank << 12) + 0x1010);
                bank = r_u32(0x800acff8);
                SpuWrite(psx_addr(0x800aafec + (bank << 12), 0x1000), 0x1000);
                bank = r_u32(0x800acff8);
                transfer_time = r_u32(0x800acffc);
                w_u32(0x800acff8, bank ^ 1);
                w_u32(0x800acffc, transfer_time + 39);
            }
        }
        game = r_u32(0x80097048);
        started = r_u32(0x800aa7e4);
        now = r_u32(game + 0x154);
        uint32 read_time = r_u32(0x800ad0bc);
        elapsed = now - started;
        if (read_time < elapsed && SpuIsTransferCompleted(0) != 0)
        {
            uint32 request = r_u32(0x800aa7e8);
            if (G3Function80065650(request) == 0 && r_s32(0x800ad0b8) > 0)
            {
                uint32 first_sector = r_u32(0x800aa7e0);
                uint32 offset = r_u32(0x800acff4);
                request = G3Function800653CC(0x8009da00, first_sector + offset, 0x800aafec, 4, frame);
                read_time = r_u32(0x800ad0bc);
                w_u32(0x800aa7e8, request);
                offset = r_u32(0x800acff4);
                w_u32(0x800ad0bc, read_time + 78);
                uint32 remaining = r_u32(0x800ad0b8);
                w_u32(0x800acff4, offset + 4);
                w_u32(0x800ad0b8, remaining - 1);
            }
        }
        break;
    }
    case 4:
        if (SpuIsTransferCompleted(0) != 0)
        {
            uint32 request = r_u32(0x800aa7e8);
            if (G3Function80065650(request) == 0)
            {
                w_u32(0x800aa7d4, 0);
                w_u8(0x800aa7d2, 0);
            }
        }
        break;
    default:
        break;
    }
    return G3CdPump80064F44();
}

uint32 G3Function800353B8(uint32 guest_stack)
{
    FUNCTION_MARKER(0x800353b8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 mode = r_u8(0x800aa7d3);
    uint32 game = r_u32(0x80097048);
    uint32 volume = r_u16(game + (mode == 3 ? 0x67d0 : 0x67d8));
    sint32 signed_volume = (sint32)(volume << 16) >> 16;
    if (signed_volume >= 128)
        volume = 127;
    else if (signed_volume < 0)
        volume = 0;
    uint32 gain = volume * volume;
    uint32 voices = r_u32(0x800ad074);
    if (voices == 0)
        return 0x10000;
    w_u32(0x800ad07c, 0x1ff93);
    w_u16(0x800ad08c, 0x400);
    w_u32(0x800ad094, 0x1010);
    w_u32(0x800ad09c, 1);
    w_u32(0x800ad0a0, 1);
    w_u32(0x800ad078, voices);
    w_u16(0x800ad080, gain);
    w_u16(0x800ad082, gain);
    w_u32(0x800ad0a4, 3);
    w_u16(0x800ad0a8, 0);
    w_u16(0x800ad0aa, 0);
    w_u16(0x800ad0ac, 0);
    w_u16(0x800ad0ae, 0);
    w_u16(0x800ad0b0, 15);
    G3Function80049968(0x800ad078);
    game = r_u32(0x80097048);
    uint32 entry = r_u32(0x800ad000);
    w_u32(0x800aa7d4, 3);
    uint32 now = r_u32(game + 0x154);
    w_u32(0x800acfec, 450);
    w_u32(0x800aa7e4, now);
    uint32 flags = r_u16(entry);
    w_u16(entry, flags | 0x4000);
    voices = r_u32(0x800ad074);
    G3Function8004999C(1, voices);
    uint32 length = r_u8(0x800aa7f0) + 40;
    sint64 product = (sint64)(sint32)length * (sint64)(sint32)0xd20d20d3;
    uint32 high = (uint32)((uint64)product >> 32);
    uint32 combined = high + length;
    uint32 remaining = (uint32)((sint32)combined >> 5) - 1;
    w_u32(0x800ad0b8, remaining);
    if ((sint32)remaining > 0)
    {
        uint32 first_sector = r_u32(0x800aa7e0);
        uint32 offset = r_u32(0x800acff4);
        uint32 request = G3Function800653CC(0x8009da00, first_sector + offset, 0x800aafec, 4, frame);
        w_u32(0x800aa7e8, request);
        offset = r_u32(0x800acff4);
        remaining = r_u32(0x800ad0b8);
        w_u32(0x800acff4, offset + 4);
        w_u32(0x800ad0b8, remaining - 1);
    }
    w_u32(0x800acffc, 45);
    w_u32(0x800ad0bc, 84);
    w_u32(0x800acff8, 0);
    return 84;
}

uint32 G3Function8003556C(void)
{
    FUNCTION_MARKER(0x8003556c, "SLUS_008.06");
    uint32 voices = r_u32(0x800ad074);
    return G3Function8004999C(0, voices);
}

uint32 G3LoadLevel80027348(uint32 root, uint32 level_name, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80027348, "SLUS_008.06");
    uint32 frame = guest_stack - 0xa0;
    for (uint32 i = 0; i < 3; ++i)
    {
        uint32 source = 0x8009d75c + i * 8;
        uint32 destination = frame + 0x68 + i * 8;
        r_masked_u32(source, 15);
        uint32 word = r_masked_u32(source, 15);
        uint32 fourth = (uint32)r_s8(source + 4);
        uint32 fifth = (uint32)r_s8(source + 5);
        uint32 high = destination + 3;
        uint32 lane = high & 3;
        w_masked_u32(high & 0xfffffffc, word >> ((3 - lane) * 8), (1 << (lane + 1)) - 1);
        lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, word << (lane * 8), (15 << lane) & 15);
        w_u8(destination + 4, fourth);
        w_u8(destination + 5, fifth);
        w_u8(destination + 6, (uint32)r_s8(source + 6));
    }
    r_masked_u32(0x8009d774, 15);
    uint32 prefix = r_masked_u32(0x8009d774, 15);
    uint32 destination = frame + 0x80;
    uint32 high = destination + 3;
    uint32 lane = high & 3;
    w_masked_u32(high & 0xfffffffc, prefix >> ((3 - lane) * 8), (1 << (lane + 1)) - 1);
    lane = destination & 3;
    w_masked_u32(destination & 0xfffffffc, prefix << (lane * 8), (15 << lane) & 15);
    w_u32(frame + 0x10, level_name);
    w_u32(frame + 0x14, level_name);
    G3SdkSprintfFourStrings80088A1C(game + 0x6690, 0x8009d778, frame + 0x80, root, level_name, level_name);
    w_u32(frame + 0x10, level_name);
    w_u32(frame + 0x14, level_name);
    G3SdkSprintfFourStrings80088A1C(frame + 0x18, 0x8009d778, frame + 0x80, root, level_name, level_name);
    sprintfPSX(game + 0x65f0, frame + 0x70, frame + 0x18);
    sprintfPSX(game + 0x6640, frame + 0x78, frame + 0x18);
    sprintfPSX(game + 0x6730, frame + 0x68, frame + 0x18);
    G3CopyString8008935C(game + 0x66e0, frame + 0x18);
    uint32 result = G3Function80038F6C(level_name, frame);
    return (uint32)((sint32)(result << 16) >> 16);
}

void G3Function80081C58(void)
{
    FUNCTION_MARKER(0x80081c58, "SLUS_008.06");
    PadSetAct(0, (uint8 *)psx_addr(0x800a2a8c, 2), 2);
}

uint32 G3Function80048870(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80048870, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 audio = r_u32(0x80098ff0);
    if (audio == 0)
        return 0;
    if ((r_u16(audio + 0x12) & 2) == 0)
    {
        SpuGetAllKeysStatus((uint8 *)psx_addr(audio + 0x4e8, 24));
        audio = r_u32(0x80098ff0);
        uint32 bit = 1;
        for (uint32 voice = 0; voice < 24; ++voice, bit <<= 1)
        {
            uint32 status = r_u8(audio + voice + 0x4e8);
            uint32 mask = r_u32(audio + 0x530);
            if (status == 3)
                w_u32(audio + 0x530, mask | bit);
            else if ((mask & bit) != 0 && (status == 0 || status == 2))
                w_u32(audio + 0x530, mask & ~bit);
        }
        audio = r_u32(0x80098ff0);
        if ((r_u16(audio + 0x12) & 4) == 0 && r_u8(audio + 4) != 0)
        {
            uint32 index = 0;
            do
            {
                audio = r_u32(0x80098ff0);
                uint32 bank = r_u32(audio + 0x20 + (index & 0xffff) * 4);
                if ((r_u16(bank + 0xf2) & 1) != 0 && (r_u16(bank + 0xf4) & 0x20) == 0)
                {
                    uint32 tick = r_u16(bank + 0x10);
                    uint32 clock = r_u32(bank);
                    uint32 fraction = r_u32(bank + 4);
                    w_u32(bank, clock + tick);
                    uint32 increment = r_u32(bank + 0xc);
                    uint32 threshold = r_u32(bank + 8);
                    uint32 next = fraction + increment;
                    w_u32(bank + 4, next);
                    if (next >= threshold)
                    {
                        clock = r_u32(bank);
                        w_u32(bank + 4, next - threshold);
                        w_u32(bank, clock + 1);
                    }
                    while (r_u16(bank + 0xda) < 15)
                    {
                        if ((r_u16(bank + 0xf4) & 0x10) != 0)
                            break;
                        uint32 result = G3Function80049BF8(frame + 0x10, bank);
                        uint32 head = r_u8(bank + 0xd8);
                        w_u32(bank + 0xe4, result);
                        uint32 slot = bank + head * 12;
                        uint32 first = r_u32(frame + 0x10);
                        uint32 second = r_u32(frame + 0x14);
                        uint32 third = r_u32(frame + 0x18);
                        w_u32(slot + 0x18, first);
                        w_u32(slot + 0x1c, second);
                        w_u32(slot + 0x20, third);
                        uint32 count = r_u16(bank + 0xda);
                        head = r_u8(bank + 0xd8) + 1;
                        w_u8(bank + 0xd8, head);
                        w_u16(bank + 0xda, count + 1);
                        if ((head & 0xff) == 16)
                            w_u8(bank + 0xd8, 0);
                    }
                    while (r_u16(bank + 0xda) != 0)
                    {
                        uint32 tail = r_u8(bank + 0xd9);
                        uint32 offset = tail * 12;
                        uint32 delta = r_u32(bank + offset + 0x18);
                        clock = r_u32(bank + 0xe0);
                        uint32 now = r_u32(bank);
                        uint32 due = delta + clock;
                        if (now < due)
                            break;
                        uint32 count = r_u16(bank + 0xda);
                        tail = r_u8(bank + 0xd9) + 1;
                        w_u32(bank + 0xe0, due);
                        w_u16(bank + 0xda, count - 1);
                        w_u8(bank + 0xd9, tail);
                        if ((tail & 0xff) == 16)
                            w_u8(bank + 0xd9, 0);
                        G3Function80049D50(bank + offset + 0x18, bank, frame);
                    }
                }
                audio = r_u32(0x80098ff0);
                ++index;
                uint32 banks = r_u8(audio + 4);
                if ((index & 0xffff) >= banks)
                    break;
            } while (true);
        }
        audio = r_u32(0x80098ff0);
        uint32 count = r_u16(audio + 0x242);
        while (count != 0)
        {
            audio = r_u32(0x80098ff0);
            uint32 tail = r_u8(audio + 0x241);
            G3Function8004C108(audio + 0x40 + tail * 16);
            audio = r_u32(0x80098ff0);
            count = r_u16(audio + 0x242);
            tail = r_u8(audio + 0x241) + 1;
            w_u8(audio + 0x241, tail);
            w_u16(audio + 0x242, count - 1);
            if ((tail & 0xff) == 32)
            {
                audio = r_u32(0x80098ff0);
                w_u8(audio + 0x241, 0);
            }
            audio = r_u32(0x80098ff0);
            count = r_u16(audio + 0x242);
        }
        audio = r_u32(0x80098ff0);
        uint32 key_on = r_u32(audio + 0x534);
        uint32 key_off = r_u32(audio + 0x530);
        key_off &= ~key_on;
        w_u32(audio + 0x530, key_off);
        if (key_off != 0)
            SpuSetKey(0, key_off);
        audio = r_u32(0x80098ff0);
        uint32 reverb = r_u32(audio + 0x538);
        SpuSetReverbVoice(1, reverb);
        audio = r_u32(0x80098ff0);
        reverb = r_u32(audio + 0x538);
        SpuSetReverbVoice(0, ~reverb);
        audio = r_u32(0x80098ff0);
        key_on = r_u32(audio + 0x534);
        if (key_on != 0)
        {
            SpuSetKey(1, key_on);
            audio = r_u32(0x80098ff0);
            w_u32(audio + 0x534, 0);
        }
    }
    audio = r_u32(0x80098ff0);
    sint32 step = r_s16(audio + 8);
    uint32 unsigned_step = r_u16(audio + 8);
    if (step != 0)
    {
        uint32 value = r_u16(audio + 6) + unsigned_step;
        sint32 signed_value = (sint32)(value << 16) >> 16;
        sint32 target = r_s16(audio + 0xa);
        bool reached = step < 0 ? signed_value < target : target < signed_value;
        if (reached)
        {
            audio = r_u32(0x80098ff0);
            value = r_u16(audio + 0xa);
            uint32 callback = r_u32(audio + 0xc);
            w_u16(audio + 8, 0);
            if (callback != 0)
                G3DispatchAudioFade(callback, frame);
        }
        G3Function800486AC((uint32)((sint32)(value << 16) >> 16));
    }
    audio = r_u32(0x80098ff0);
    step = r_s16(audio + 0x14);
    unsigned_step = r_u16(audio + 0x14);
    if (step != 0 && (r_u32(audio) & 1) == 0)
    {
        uint32 value = r_u8(audio + 0x11) + unsigned_step;
        sint32 signed_value = (sint32)(value << 16) >> 16;
        sint32 target = r_s16(audio + 0x16);
        bool reached = step < 0 ? signed_value <= target : target <= signed_value;
        if (reached)
        {
            audio = r_u32(0x80098ff0);
            value = r_u16(audio + 0x16);
            uint32 callback = r_u32(audio + 0x18);
            w_u16(audio + 0x14, 0);
            if (callback != 0)
                G3DispatchAudioFade(callback, frame);
        }
        G3Function80048718(value & 0xffff);
    }
    audio = r_u32(0x80098ff0);
    uint32 clock = r_u32(audio);
    w_u32(audio, clock + 1);
    return 0;
}

uint32 G3Function8003F088(uint32 handle)
{
    FUNCTION_MARKER(0x8003f088, "SLUS_008.06");
    return G3Function8004BC4C(handle) & 0xffff;
}

uint32 G3Function8003F0A8(uint32 handle)
{
    FUNCTION_MARKER(0x8003f0a8, "SLUS_008.06");
    return G3Function8004BCD4(handle) & 0xffff;
}

uint32 G3Function8003F0C8(uint32 sound)
{
    FUNCTION_MARKER(0x8003f0c8, "SLUS_008.06");
    return G3Function8004BD98(sound & 0xffff, 0) & 0xffff;
}

uint32 G3Function8003F0E8(uint32 sound, uint32 *third_glyph)
{
    FUNCTION_MARKER(0x8003f0e8, "SLUS_008.06");
    return G3Function8004BE24(sound & 0xffff, third_glyph) & 0xffff;
}

uint32 G3Function8003F22C(uint32 handle, uint32 volume, uint32 pan, uint32 pitch)
{
    FUNCTION_MARKER(0x8003f22c, "SLUS_008.06");
    if (handle == 0)
        return 0;
    uint32 game = r_u32(0x80097048);
    if (r_u8(game + 0x67df) == 0)
        return 0;
    if (r_u8(game + 0x67dc) == 0)
        return 0;
    uint32 signed_pitch = (uint32)((sint32)(pitch << 16) >> 16);
    return G3Function8004BF74(handle, volume & 0xffff, pan & 0xffff, signed_pitch);
}

uint32 G3Function80035A80(void)
{
    FUNCTION_MARKER(0x80035a80, "SLUS_008.06");
    if (r_u8(0x800aa7d0) != 0)
        return 0x800b0000;
    w_u8(0x800aa7d1, 1);
    return 1;
}

uint32 G3Function80035AA4(void)
{
    FUNCTION_MARKER(0x80035aa4, "SLUS_008.06");
    uint32 count = r_u8(0x800aa7d0);
    if (count != 0)
    {
        --count;
        w_u8(0x800aa7d0, count);
    }
    w_u8(0x800aa7d1, 0);
    return count;
}

uint32 G3Function80035F38(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80035f38, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 result = r_u32(game + 0x5c);
    sint32 blocked = r_s16(game + 0x6786);
    uint32 instance = r_u32(result + 4);
    if (blocked != 0)
        return result;
    result = (uint32)r_s16(game + 0x6782);
    if (result != 0)
        return result;
    result = r_u32(game + 0x67c4);
    if (result != 0)
        return result;
    result = G3Function8003525C();
    if (result == 0)
        return result;
    if (r_u16(game + 0x68b4) < 28)
    {
        bool fallback = true;
        sint32 override_key = r_s16(0x800ad0c4);
        if (override_key >= 0)
        {
            sint32 count = r_s16(0x800ad0c6);
            uint32 unsigned_count = r_u16(0x800ad0c6);
            bool request = true;
            if (count > 0)
            {
                uint32 next = unsigned_count - 1;
                w_u16(0x800ad0c6, next);
                if ((next & 0xffff) == 0)
                {
                    w_u16(0x800ad0c4, 0xffff);
                    request = false;
                }
            }
            if (request)
            {
                uint32 key = (uint32)r_s16(0x800ad0c4);
                uint32 divisor = (uint32)r_s16(0x800ad0c8);
                G3Function800358C0(key, divisor, frame);
            }
            fallback = r_s16(0x800ad0c6) < 1;
        }
        if (fallback)
        {
            sint32 key = r_s16(0x800ad0c0);
            if (key >= 0)
            {
                uint32 divisor = (uint32)r_s16(0x800ad0c2);
                G3Function800358C0((uint32)key, divisor, frame);
            }
            else
            {
                uint32 level = r_u8(game + 0x68b4);
                uint32 row = 0x80099708 + level * 28;
                key = r_s16(row + 0x14);
                if (key != -1)
                {
                    uint32 divisor = (uint32)r_s16(row + 0x16);
                    G3Function800358C0((uint32)key, divisor, frame);
                }
            }
        }
        uint32 level = r_u16(game + 0x68b4);
        if (level - 11 < 5)
            G3Function800358C0(0x123, 1000, frame);
    }
    result = 0x80090000;
    while (instance != 0)
    {
        result = r_u32(instance + 0x10) & 0x200;
        if (result != 0)
        {
            result = r_u32(0x8009efc4);
            if (instance != result)
            {
                result = r_u32(instance + 0x20);
                bool eligible = result == 0;
                if (!eligible)
                {
                    result = r_u32(result + 0x18) & 2;
                    eligible = result == 0;
                }
                if (eligible)
                {
                    result = r_u32(instance + 0x1c);
                    sint32 kind = r_s16(result + 6);
                    result = (uint32)kind << 1;
                    if (kind > 0)
                    {
                        result = 0x80097844 + (uint32)kind * 12;
                        sint32 key = r_s32(result + 4);
                        if (key > 0)
                        {
                            uint32 divisor = r_u32(result + 8);
                            result = G3Function800358C0((uint32)key, divisor, frame);
                        }
                    }
                }
            }
        }
        instance = r_u32(instance + 8);
    }
    return result;
}

uint32 G3Function8003F8A4(uint32 path, uint32 state, uint32 guest_stack, G3TextMeasureOperands *operands)
{
    FUNCTION_MARKER(0x8003f8a4, "SLUS_008.06");
    if (path == 0 || state == 0)
        return 0xffff;
    uint32 frame = guest_stack - 0x20;
    uint32 entry = r_u32(path);
    uint32 high_address = state + 3;
    uint32 high_lane = high_address & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
    uint32 low_lane = state & 3;
    uint32 low = r_masked_u32(state & 0xfffffffc, (15 << low_lane) & 15);
    uint32 value = low_lane == 0 ? low : (high << ((4 - low_lane) * 8)) | (low >> (low_lane * 8));
    uint32 destination = frame + 0x10;
    high_address = destination + 3;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    low_lane = destination & 3;
    w_masked_u32(destination & 0xfffffffc, value << (low_lane * 8), (15 << low_lane) & 15);
    uint32 count = (uint32)r_s16(state);
    uint32 index = 0;
    uint32 total = 0;
    if (count != 0)
    {
        do
        {
            ++index;
            uint32 span = r_u16(entry);
            total += span;
            entry += 0x20;
        } while (index < count);
    }
    w_u16(destination, index);
    if (operands != 0)
    {
        operands->first_glyph = destination;
        operands->second_glyph = 0;
        operands->third_glyph = value;
    }
    G3Function8003F4A0(path, destination, 0, frame, operands != 0 ? &operands->third_glyph : 0);
    value = r_u32(destination);
    return (total + (value >> 17)) & 0xffff;
}

uint32 G3Function800361FC(uint32 instance, uint32 distance)
{
    FUNCTION_MARKER(0x800361fc, "SLUS_008.06");
    uint32 angle = (uint32)r_s16(instance + 0x64) + 0xc00;
    uint32 cosine = G3Function8005B470(angle);
    uint32 product = cosine * distance;
    uint32 x = r_u16(instance + 0x58);
    uint32 delta = (uint32)((sint32)product >> 12);
    w_u16(instance + 0x58, x + delta);
    uint32 sine = G3Function8005B590(angle);
    product = sine * distance;
    uint32 y = r_u16(instance + 0x5a);
    delta = (uint32)((sint32)product >> 12);
    uint32 result = y + delta;
    w_u16(instance + 0x5a, result);
    return result;
}

uint32 G3Function80036278(uint32 direction, uint32 output, uint32 scale, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80036278, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    uint32 y = (uint32)r_s16(direction + 2);
    uint32 x = (uint32)r_s16(direction);
    uint32 angle = G3Function8002F0C4(y, x, frame) - 0x400;
    angle = (uint32)((sint32)(angle << 16) >> 16);
    uint32 cosine = G3Function8005B470(angle);
    w_u16(frame + 0x30, cosine);
    uint32 sine = G3Function8005B590(angle);
    w_u16(frame + 0x32, sine);
    w_u16(frame + 0x34, 0);
    uint32 component = r_u16(direction);
    w_u16(frame + 0x14, component);
    component = r_u16(direction + 2);
    w_u16(frame + 0x1a, component);
    component = r_u16(direction + 4);
    w_u16(frame + 0x20, component);
    G3Function8002F630(frame + 0x30, frame + 0x10, 0xfffffc00, frame);
    uint32 result = 0;
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        component = (uint32)r_s16(frame + 0x14 + axis * 6);
        uint32 product = scale * component;
        uint32 z = (uint32)r_s16(direction + 4);
        uint32 factor = 0x1000 - z;
        uint32 first = (uint32)((sint32)product >> 12);
        product = first * factor;
        result = (uint32)((sint32)product >> 12);
        w_u16(output + axis * 2, result);
    }
    return result;
}

void G3Function8002F630(uint32 axis, uint32 matrix, uint32 angle, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002f630, "SLUS_008.06");
    uint32 frame = guest_stack - 0x70;
    angle = (uint32)((sint32)(angle << 16) >> 16);
    if (angle == 0)
        return;
    uint32 y = (uint32)r_s16(axis + 2);
    uint32 y_square = y * y;
    uint32 z = (uint32)r_s16(axis + 4);
    uint32 squares = y_square + z * z + 0x800;
    uint32 length = G3Function8002F368((uint32)((sint32)squares >> 12), frame);
    y = (uint32)r_s16(axis + 2);
    z = (uint32)r_s16(axis + 4);
    uint32 first = G3Function8002F240(y, z, frame);
    w_u16(frame + 0x10, 0 - first);
    uint32 x = (uint32)r_s16(axis);
    uint32 second = G3Function8002F240(x, length, frame);
    w_u16(frame + 0x12, second);
    w_u16(frame + 0x14, 0);
    G3Function8005BD00(frame + 0x10, frame + 0x18);
    TransposeMatrix((MATRIX *)psx_addr(frame + 0x18, 20), (MATRIX *)psx_addr(frame + 0x38, 20));
    MulMatrix2((MATRIX *)psx_addr(frame + 0x38, 20), (MATRIX *)psx_addr(matrix, 20));
    G3Function8005BB6C(angle, matrix, frame);
    MulMatrix2((MATRIX *)psx_addr(frame + 0x18, 20), (MATRIX *)psx_addr(matrix, 20));
}

uint32 G3Function80038AA8(uint32 angle_address, uint32 target, uint32 step)
{
    FUNCTION_MARKER(0x80038aa8, "SLUS_008.06");
    uint32 current = (uint32)r_s16(angle_address);
    uint32 requested = (uint32)((sint32)(target << 16) >> 16);
    uint32 difference = G3Function80038B48(current, requested);
    sint32 signed_difference = (sint32)(difference << 16) >> 16;
    sint32 signed_step = (sint32)(step << 16) >> 16;
    uint32 result = 0;
    if (signed_difference < -signed_step)
        current -= step;
    else if (signed_step < signed_difference)
        current += step;
    else
    {
        current = target;
        result = 1;
    }
    w_u16(angle_address, current & 0xfff);
    return result;
}

uint32 G3Function80036738(uint32 instance, uint32 target, uint32 distance, uint32 turn, uint32 aligned_output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80036738, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 target_x = (uint32)r_s16(target);
    uint32 instance_x = (uint32)r_s16(instance + 0x58);
    uint32 target_y = (uint32)r_s16(target + 2);
    uint32 instance_y = (uint32)r_s16(instance + 0x5a);
    uint32 dx = target_x - instance_x;
    uint32 dy = target_y - instance_y;
    if ((dx | dy) == 0)
    {
        if (aligned_output != 0)
            w_u32(aligned_output, 1);
        return 1;
    }
    uint32 previous = r_u16(instance + 0x64) - 0x400;
    uint32 angle = G3Function8002F240(dy, dx, frame);
    previous = (uint32)((sint32)(previous << 16) >> 16);
    uint32 signed_angle = (uint32)((sint32)(angle << 16) >> 16);
    uint32 difference = G3Function80038B48(previous, signed_angle);
    sint32 signed_difference = (sint32)(difference << 16) >> 16;
    sint32 signed_turn = (sint32)(turn << 16) >> 16;
    if (signed_difference < -signed_turn)
    {
        uint32 current = r_u16(instance + 0x64);
        w_u16(instance + 0x64, current - turn);
    }
    else if (signed_turn < signed_difference)
    {
        uint32 current = r_u16(instance + 0x64);
        w_u16(instance + 0x64, current + turn);
    }
    else
    {
        w_u16(instance + 0x64, angle + 0x400);
        if (aligned_output != 0)
            w_u32(aligned_output, 1);
        uint32 squares = dx * dx + dy * dy;
        distance = (uint32)((sint32)(distance << 16) >> 16);
        uint32 radius_square = distance * distance;
        if ((sint32)radius_square >= (sint32)squares)
        {
            uint32 x = r_u16(target);
            w_u16(instance + 0x58, x);
            uint32 y = r_u16(target + 2);
            w_u16(instance + 0x5a, y);
            return 1;
        }
        uint32 cosine = G3Function8005B470(signed_angle);
        uint32 product = distance * cosine;
        uint32 x = r_u16(instance + 0x58);
        uint32 delta = (uint32)((sint32)product >> 12);
        w_u16(instance + 0x58, x + delta);
        uint32 sine = G3Function8005B590(signed_angle);
        product = distance * sine;
        uint32 y = r_u16(instance + 0x5a);
        delta = (uint32)((sint32)product >> 12);
        w_u16(instance + 0x5a, y + delta);
        return 0;
    }
    if (aligned_output != 0)
        w_u32(aligned_output, 0);
    return 0;
}

uint32 G3Function8003639C(uint32 instance, uint32 turn, uint32 speed, uint32 target, uint32 flags, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003639c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x58;
    uint32 result = 2;
    if (speed == 0)
        return result;
    uint32 instance_x = (uint32)r_s16(instance + 0x58);
    uint32 target_x = (uint32)r_s16(target);
    uint32 instance_y = (uint32)r_s16(instance + 0x5a);
    uint32 target_y = (uint32)r_s16(target + 2);
    uint32 original_speed = r_u16(speed);
    uint32 dx = instance_x - target_x;
    uint32 dy = instance_y - target_y;
    uint32 instance_z = (uint32)r_s16(instance + 0x5c);
    uint32 target_z = (uint32)r_s16(target + 4);
    uint32 dz = instance_z - target_z;
    uint32 angle = G3Function8002F240(dy, dx, frame);
    uint32 desired = angle + 0xc00;
    uint32 signed_turn = (uint32)((sint32)(turn << 16) >> 16);
    G3Function80038AA8(instance + 0x64, (angle + 0x2c00) & 0xfff, signed_turn);
    uint32 desired_low = desired & 0xffff;
    uint32 normalized = ((uint32)((sint32)(desired << 16) >> 16) + 0x2000) & 0xfff;
    uint32 current = (uint32)r_s16(instance + 0x64);
    sint32 difference = (sint32)(normalized - ((current + 0x2000) & 0xfff));
    if (difference < 0)
        difference = -difference;
    sint32 half_turn = (sint32)(turn + (turn >> 31)) >> 1;
    if (half_turn >= difference)
        result = 0x42;
    bool reached = false;
    bool check_progress = false;
    uint32 previous_distance = 0;
    if ((flags & 0x40) != 0)
    {
        if ((result & 0x40) != 0)
        {
            uint32 radius;
            if ((flags & 0x80) != 0)
            {
                sint32 current_speed = r_s16(speed);
                sint32 maximum = r_s16(speed + 4);
                uint32 unsigned_speed = r_u16(speed);
                if (current_speed < maximum)
                {
                    w_u16(speed, unsigned_speed << 1);
                    maximum = r_s16(speed + 4);
                    sint32 doubled = (sint32)(unsigned_speed << 17) >> 16;
                    uint32 unsigned_maximum = r_u16(speed + 4);
                    if (maximum < doubled)
                        w_u16(speed, unsigned_maximum);
                }
                radius = (uint32)r_s16(speed);
            }
            else
                radius = (uint32)((sint32)(original_speed << 16) >> 16);
            uint32 squares = dx * dx + dy * dy + dz * dz;
            reached = (sint32)(radius * radius) >= (sint32)squares;
        }
        else
        {
            check_progress = true;
            previous_distance = dx * dx + dy * dy + dz * dz;
        }
    }
    if (reached)
    {
        w_u16(instance + 0x64, desired_low);
        uint32 x = r_u16(target);
        w_u16(instance + 0x58, x);
        uint32 y = r_u16(target + 2);
        result |= 8;
        w_u16(instance + 0x5a, y);
    }
    else
        G3Function800361FC(instance, (uint32)((sint32)(original_speed << 16) >> 16));
    if (check_progress)
    {
        instance_x = (uint32)r_s16(instance + 0x58);
        target_x = (uint32)r_s16(target);
        dx = instance_x - target_x;
        instance_y = (uint32)r_s16(instance + 0x5a);
        target_y = (uint32)r_s16(target + 2);
        dy = instance_y - target_y;
        uint32 next_distance = dx * dx + dy * dy + dz * dz;
        if (next_distance >= previous_distance)
        {
            uint32 backwards = 0 - (uint32)((sint32)(original_speed << 16) >> 16);
            backwards = (uint32)((sint32)(backwards + (backwards >> 31)) >> 1);
            G3Function800361FC(instance, backwards);
            G3Function80038AA8(instance + 0x64, (desired_low + 0x2000) & 0xfff, signed_turn);
            result |= 0x10;
            if ((flags & 0x80) != 0)
            {
                uint32 value = r_u16(speed);
                sint32 signed_value = (sint32)(value << 16) >> 16;
                sint32 half = (signed_value + (sint32)(value >> 15)) >> 1;
                sint32 minimum = r_s16(speed + 2);
                w_u16(speed, (uint32)half);
                uint32 unsigned_minimum = r_u16(speed + 2);
                if (half < minimum)
                    w_u16(speed, unsigned_minimum);
            }
        }
    }
    return result;
}

uint32 G3Function80036988(uint32 instance, uint32 target, uint32 distance, uint32 turn, uint32 threshold, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80036988, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 target_x = r_u16(target);
    uint32 instance_x = r_u16(instance + 0x58);
    uint32 dx = target_x - instance_x;
    uint32 target_y = r_u16(target + 2);
    uint32 instance_y = r_u16(instance + 0x5a);
    uint32 dy = target_y - instance_y;
    uint32 target_z = r_u16(target + 4);
    uint32 instance_z = r_u16(instance + 0x5c);
    uint32 dz = target_z - instance_z;
    if (((dx | dy) << 16) == 0)
        return 1;
    dx = (uint32)((sint32)(dx << 16) >> 16);
    dy = (uint32)((sint32)(dy << 16) >> 16);
    dz = (uint32)((sint32)(dz << 16) >> 16);
    uint32 squares = dx * dx + dy * dy + dz * dz;
    uint32 previous = r_u16(instance + 0x64) - 0x400;
    uint32 angle = G3Function8002F240(dy, dx, frame);
    previous = (uint32)((sint32)(previous << 16) >> 16);
    uint32 signed_angle = (uint32)((sint32)(angle << 16) >> 16);
    uint32 difference = G3Function80038B48(previous, signed_angle);
    sint32 signed_difference = (sint32)(difference << 16) >> 16;
    sint32 signed_turn = (sint32)(turn << 16) >> 16;
    bool aligned = false;
    if (signed_difference < -signed_turn)
    {
        uint32 current = r_u16(instance + 0x64);
        w_u16(instance + 0x64, current - turn);
    }
    else if (signed_turn < signed_difference)
    {
        uint32 current = r_u16(instance + 0x64);
        w_u16(instance + 0x64, current + turn);
    }
    else
    {
        uint32 signed_distance = (uint32)((sint32)(distance << 16) >> 16);
        uint32 radius_square = signed_distance * signed_distance;
        w_u16(instance + 0x64, angle + 0x400);
        aligned = true;
        if ((sint32)radius_square >= (sint32)squares)
        {
            uint32 high_address = target + 3;
            uint32 high_lane = high_address & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low_lane = target & 3;
            uint32 low = r_masked_u32(target & 0xfffffffc, (15 << low_lane) & 15);
            uint32 word = low_lane == 0 ? low : (high << ((4 - low_lane) * 8)) | (low >> (low_lane * 8));
            uint32 z = (uint32)r_s16(target + 4);
            uint32 destination = instance + 0x58;
            high_address = destination + 3;
            high_lane = high_address & 3;
            w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            low_lane = destination & 3;
            w_masked_u32(destination & 0xfffffffc, word << (low_lane * 8), (15 << low_lane) & 15);
            w_u16(instance + 0x5c, z);
            return 1;
        }
    }
    if (!aligned && (sint32)threshold >= (sint32)squares)
        return 0;
    uint32 current_angle = (uint32)r_s16(instance + 0x64);
    uint32 sine = G3Function8005B590(current_angle);
    distance = (uint32)((sint32)(distance << 16) >> 16);
    uint32 product = distance * sine;
    current_angle = (uint32)r_s16(instance + 0x64);
    uint32 x = r_u16(instance + 0x58);
    uint32 delta = (uint32)((sint32)product >> 12);
    w_u16(instance + 0x58, x + delta);
    uint32 cosine = G3Function8005B470(current_angle);
    product = distance * cosine;
    uint32 y = r_u16(instance + 0x5a);
    delta = (uint32)((sint32)product >> 12);
    w_u16(instance + 0x5a, y - delta);
    return 0;
}

uint32 G3Function80036B80(uint32 source, uint32 target, uint32 speed, uint32 gravity, uint32 minimum, uint32 maximum, uint32 vertical_output, uint32 speed_output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80036b80, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 target_x = (uint32)r_s16(target);
    uint32 source_x = (uint32)r_s16(source);
    uint32 dx = target_x - source_x;
    uint32 x_square = dx * dx;
    uint32 target_y = (uint32)r_s16(target + 2);
    uint32 source_y = (uint32)r_s16(source + 2);
    uint32 dy = target_y - source_y;
    uint32 squares = x_square + dy * dy;
    uint32 length = G3Function8002F448(squares, 0, frame);
    sint32 signed_maximum = (sint32)(maximum << 16) >> 16;
    if (signed_maximum < (sint32)length)
        length = (uint32)signed_maximum;
    else
    {
        sint32 signed_minimum = (sint32)(minimum << 16) >> 16;
        if ((sint32)length < signed_minimum)
            length = (uint32)signed_minimum;
    }
    sint32 signed_speed = (sint32)(speed << 16) >> 16;
    if ((sint32)length <= 0 || signed_speed == 0)
        return 0;
    if (signed_speed < 0)
    {
        uint32 product = length * (0 - (uint32)signed_speed);
        speed = product >> 12;
    }
    if (speed_output != 0)
        w_u16(speed_output, speed);
    if (vertical_output != 0)
    {
        uint32 target_z = (uint32)r_s16(target + 4);
        uint32 source_z = (uint32)r_s16(source + 4);
        uint32 velocity = (uint32)((sint32)(speed << 16) >> 16);
        uint32 product = (target_z - source_z) * velocity;
        product *= velocity;
        uint32 signed_gravity = (uint32)((sint32)(gravity << 16) >> 16);
        uint32 gravity_term = squares * signed_gravity;
        uint32 numerator = (product << 1) - gravity_term;
        uint32 denominator = (velocity * length) << 1;
        uint32 quotient;
        if (denominator == 0)
            quotient = (sint32)numerator < 0 ? 1 : 0xffffffff;
        else
            quotient = (uint32)((sint64)(sint32)numerator / (sint64)(sint32)denominator);
        w_u16(vertical_output, quotient);
    }
    return 1;
}

uint32 G3Function80036D1C(uint32 object, uint32 upper, uint32 lower, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80036d1c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x58;
    uint32 flags = r_u32(object + 0x10);
    if ((flags & 0x08000000) != 0)
        return 0x10000000;
    if ((flags & 0x10000000) == 0)
        w_u32(object + 0x10, flags | 0x08000000);
    for (uint32 i = 0; i < 2; ++i)
    {
        uint32 source = object + 0x58;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
        uint32 word = low_lane == 0 ? low : (high << ((4 - low_lane) * 8)) | (low >> (low_lane * 8));
        uint32 z = (uint32)r_s16(object + 0x5c);
        uint32 destination = frame + 0x40 + i * 8;
        high_address = destination + 3;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, word << (low_lane * 8), (15 << low_lane) & 15);
        w_u16(destination + 4, z);
    }
    w_u32(frame + 0x34, 0x17);
    w_u32(frame + 0x24, frame + 0x40);
    w_u32(frame + 0x28, frame + 0x48);
    uint32 first_z = r_u16(frame + 0x44);
    uint32 second_z = r_u16(frame + 0x4c);
    w_u32(frame + 0x38, object);
    w_u16(frame + 0x44, first_z - lower);
    w_u16(frame + 0x4c, second_z + upper);
    flags = r_u32(object + 0x10);
    uint32 game = r_u32(0x80097048);
    w_u32(object + 0x10, flags | 0x40);
    G3Function80061CA4(frame + 0x10, game, frame);
    flags = r_u32(object + 0x10) & 0xffffffbf;
    w_u32(object + 0x10, flags);
    uint32 kind = r_u32(frame + 0x10);
    uint32 result = 1;
    if (kind == 1)
    {
        result = 0x1000;
        w_u16(object + 0xb4, 0);
        w_u16(object + 0xb6, 0);
        w_u16(object + 0xb8, result);
    }
    else if (kind != 0)
    {
        if (kind == 3)
        {
            uint32 face = r_u32(frame + 0x14);
            uint32 face_flags = r_u16(face + 6);
            if ((face_flags & 0x801) == 0x800)
                w_u32(object + 0x10, flags | 0x200000);
            face = r_u32(frame + 0x14);
            w_u32(object + 0xa8, face);
        }
        else
            w_u32(object + 0x10, flags & 0xffdfffff);
        uint32 component = r_u16(frame + 0x1c);
        w_u16(object + 0xb4, component);
        component = r_u16(frame + 0x1e);
        w_u16(object + 0xb6, component);
        result = r_u16(frame + 0x20);
        w_u16(object + 0xb8, result);
    }
    uint32 source = frame + 0x40;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
    uint32 low_lane = source & 3;
    uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
    uint32 word = low_lane == 0 ? low : (high << ((4 - low_lane) * 8)) | (low >> (low_lane * 8));
    uint32 z = (uint32)r_s16(frame + 0x44);
    uint32 destination = object + 0xdc;
    high_address = destination + 3;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    low_lane = destination & 3;
    w_masked_u32(destination & 0xfffffffc, word << (low_lane * 8), (15 << low_lane) & 15);
    w_u16(object + 0xe0, z);
    return result;
}

uint32 G3Function8003465C(uint32 matrix, uint32 direction, uint32 position, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003465c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    G3Function8002FF58(matrix, direction, frame);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 value = r_u16(position + axis * 2);
        w_u16(frame + 0x10 + axis * 2, 0 - value);
    }
    uint32 first = r_u32(matrix);
    uint32 second = r_u32(matrix + 4);
    xport_gte_write_control(0, first);
    xport_gte_write_control(1, second);
    first = r_u32(matrix + 8);
    second = r_u32(matrix + 0xc);
    uint32 third = r_u32(matrix + 0x10);
    xport_gte_write_control(2, first);
    xport_gte_write_control(3, second);
    xport_gte_write_control(4, third);
    xport_gte_write_data(0, r_u32(frame + 0x10));
    xport_gte_write_data(1, r_u32(frame + 0x14));
    xport_gte_execute(0x486012);
    for (uint32 axis = 0; axis < 3; ++axis)
        w_u32(frame + 0x18 + axis * 4, xport_gte_read_data(25 + axis));
    uint32 result = 0;
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        result = r_u32(frame + 0x18 + axis * 4);
        w_u32(matrix + 0x14 + axis * 4, result);
    }
    return result;
}

uint32 G3Function80034730(uint32 vertices, uint32 output, uint32 model, uint32 transform, uint32 matrices)
{
    FUNCTION_MARKER(0x80034730, "SLUS_008.06");
    sint32 count = r_s16(model + 6);
    uint32 records = r_u32(model + 0x18);
    if (count <= 0)
        return (uint32)count;
    uint32 record = records + 0xa;
    uint32 offset = 0;
    uint32 index = 0;
    do
    {
        uint32 current_records = r_u32(model + 0x18);
        sint32 end_index = r_s16(current_records + offset + 0xa);
        if (end_index != -1)
        {
            uint32 start_index = (uint32)r_s16(record - 2);
            uint32 last_index = (uint32)r_s16(record);
            uint32 vertex = vertices + (start_index << 3);
            uint32 last = vertices + (last_index << 3);
            uint32 first = r_u32(transform);
            uint32 second = r_u32(transform + 4);
            xport_gte_write_control(0, first);
            xport_gte_write_control(1, second);
            first = r_u32(transform + 8);
            second = r_u32(transform + 0xc);
            uint32 third = r_u32(transform + 0x10);
            xport_gte_write_control(2, first);
            xport_gte_write_control(3, second);
            xport_gte_write_control(4, third);
            for (uint32 column = 0; column < 3; ++column)
            {
                first = r_u16(matrices + column * 2);
                second = r_u16(matrices + column * 2 + 6);
                third = r_u16(matrices + column * 2 + 0xc);
                xport_gte_write_data(9, first);
                xport_gte_write_data(10, second);
                xport_gte_write_data(11, third);
                xport_gte_execute(0x49e012);
                first = xport_gte_read_data(9);
                second = xport_gte_read_data(10);
                third = xport_gte_read_data(11);
                w_u16(0x1f800000 + column * 2, first);
                w_u16(0x1f800006 + column * 2, second);
                w_u16(0x1f80000c + column * 2, third);
            }
            first = r_u32(transform + 0x14);
            second = r_u32(transform + 0x18);
            xport_gte_write_control(5, first);
            third = r_u32(transform + 0x1c);
            xport_gte_write_control(6, second);
            xport_gte_write_control(7, third);
            uint32 y = r_u16(matrices + 0x18);
            uint32 x = r_u16(matrices + 0x14);
            xport_gte_write_data(0, x | (y << 16));
            xport_gte_write_data(1, r_u32(matrices + 0x1c));
            xport_gte_execute(0x480012);
            for (uint32 axis = 0; axis < 3; ++axis)
                w_u32(0x1f800014 + axis * 4, xport_gte_read_data(25 + axis));
            first = r_u32(0x1f800000);
            second = r_u32(0x1f800004);
            xport_gte_write_control(0, first);
            xport_gte_write_control(1, second);
            first = r_u32(0x1f800008);
            second = r_u32(0x1f80000c);
            third = r_u32(0x1f800010);
            xport_gte_write_control(2, first);
            xport_gte_write_control(3, second);
            xport_gte_write_control(4, third);
            first = r_u32(0x1f800014);
            second = r_u32(0x1f800018);
            xport_gte_write_control(5, first);
            third = r_u32(0x1f80001c);
            xport_gte_write_control(6, second);
            xport_gte_write_control(7, third);
            while (last >= vertex)
            {
                xport_gte_write_data(0, r_u32(vertex));
                xport_gte_write_data(1, r_u32(vertex + 4));
                xport_gte_execute(0x480012);
                for (uint32 axis = 0; axis < 3; ++axis)
                    w_u32(0x1f800020 + axis * 4, xport_gte_read_data(25 + axis));
                uint32 component = r_u16(0x1f800020);
                w_u16(output, component);
                component = r_u16(0x1f800024);
                vertex += 8;
                w_u16(output + 2, component);
                component = r_u16(0x1f800028);
                w_u16(output + 4, component);
                output += 8;
            }
        }
        record += 0x18;
        matrices += 0x20;
        count = r_s16(model + 6);
        ++index;
        offset += 0x18;
    } while ((sint32)index < count);
    return 0;
}

void G3Function8002FF58(uint32 matrix, uint32 direction, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002ff58, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    sint32 x = r_s16(direction);
    sint32 absolute_x = x < 0 ? -x : x;
    if (absolute_x < 5)
    {
        sint32 y = r_s16(direction + 2);
        sint32 absolute_y = y < 0 ? -y : y;
        if (absolute_y < 5)
        {
            if (r_s16(direction + 4) >= 0)
                G3Function8002F718(matrix);
            else
            {
                w_u16(matrix, 0x1000);
                w_u16(matrix + 2, 0);
                w_u16(matrix + 4, 0);
                w_u16(matrix + 6, 0);
                w_u16(matrix + 8, 0xf000);
                w_u16(matrix + 0xa, 0);
                w_u16(matrix + 0xc, 0);
                w_u16(matrix + 0xe, 0);
                w_u16(matrix + 0x10, 0xf000);
            }
            return;
        }
    }
    uint32 negated_y = 0 - (uint32)r_s16(direction + 2);
    w_u16(frame + 0x18, negated_y);
    sint32 signed_y = (sint32)(negated_y << 16) >> 16;
    sint32 magnitude_y = signed_y < 0 ? -signed_y : signed_y;
    x = r_s16(direction);
    w_u16(frame + 0x1c, 0);
    absolute_x = x < 0 ? -x : x;
    w_u16(frame + 0x1a, (uint32)x);
    sint32 magnitudes[3];
    magnitudes[0] = (sint32)((uint32)magnitude_y << 16) >> 16;
    magnitudes[1] = (sint32)((uint32)absolute_x << 16) >> 16;
    magnitudes[2] = 0;
    for (uint32 i = 0; i < 2; ++i)
    {
        for (uint32 j = i + 1; j < 3; ++j)
        {
            if (magnitudes[i] < magnitudes[j])
            {
                sint32 temporary = magnitudes[i];
                magnitudes[i] = magnitudes[j];
                magnitudes[j] = temporary;
            }
        }
    }
    sint32 weighted = magnitudes[0] * 30 + magnitudes[1] * 12 + magnitudes[2] * 9;
    sint32 divisor = (weighted < 0 ? weighted + 7 : weighted) >> 3;
    if (divisor != 0)
    {
        uint32 normalized[3];
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 value = (uint32)r_s16(frame + 0x18 + axis * 2) << 14;
            normalized[axis] = (uint32)((sint64)(sint32)value / (sint64)divisor);
        }
        for (uint32 axis = 0; axis < 3; ++axis)
            w_u16(frame + 0x18 + axis * 2, normalized[axis]);
    }
    uint32 y = (uint32)r_s16(direction + 2);
    uint32 basis_z = (uint32)r_s16(frame + 0x1c);
    uint32 product = y * basis_z;
    uint32 basis_y = (uint32)r_s16(frame + 0x1a);
    uint32 z = (uint32)r_s16(direction + 4);
    uint32 first = (uint32)((sint32)(product - z * basis_y) >> 12);
    w_u16(frame + 0x20, first);
    uint32 source_x = (uint32)r_s16(direction);
    product = source_x * basis_z;
    uint32 basis_x = (uint32)r_s16(frame + 0x18);
    z = (uint32)r_s16(direction + 4);
    uint32 second = 0 - (uint32)((sint32)(product - z * basis_x) >> 12);
    w_u16(frame + 0x22, second);
    source_x = (uint32)r_s16(direction);
    product = source_x * basis_y;
    y = (uint32)r_s16(direction + 2);
    uint32 third = (uint32)((sint32)(product - y * basis_x) >> 12);
    sint32 first_half = (sint32)(first << 16) >> 16;
    sint32 second_half = (sint32)(second << 16) >> 16;
    sint32 third_half = (sint32)(third << 16) >> 16;
    uint32 absolute_first = (uint32)(first_half < 0 ? -first_half : first_half);
    uint32 absolute_second = (uint32)(second_half < 0 ? -second_half : second_half);
    uint32 absolute_third = (uint32)(third_half < 0 ? -third_half : third_half);
    w_u16(frame + 0x24, third);
    magnitudes[0] = (sint32)(absolute_first << 16) >> 16;
    magnitudes[1] = (sint32)(absolute_second << 16) >> 16;
    magnitudes[2] = (sint32)(absolute_third << 16) >> 16;
    for (uint32 i = 0; i < 2; ++i)
    {
        for (uint32 j = i + 1; j < 3; ++j)
        {
            if (magnitudes[i] < magnitudes[j])
            {
                sint32 temporary = magnitudes[i];
                magnitudes[i] = magnitudes[j];
                magnitudes[j] = temporary;
            }
        }
    }
    weighted = magnitudes[0] * 30 + magnitudes[1] * 12 + magnitudes[2] * 9;
    divisor = (weighted < 0 ? weighted + 7 : weighted) >> 3;
    if (divisor != 0)
    {
        uint32 normalized[3];
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 value = (uint32)r_s16(frame + 0x20 + axis * 2) << 14;
            normalized[axis] = (uint32)((sint64)(sint32)value / (sint64)divisor);
        }
        for (uint32 axis = 0; axis < 3; ++axis)
            w_u16(frame + 0x20 + axis * 2, normalized[axis]);
    }
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 component = r_u16(frame + 0x18 + axis * 2);
        w_u16(matrix + axis * 2, component);
    }
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 component = r_u16(frame + 0x20 + axis * 2);
        w_u16(matrix + 6 + axis * 2, component);
    }
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 component = r_u16(direction + axis * 2);
        w_u16(matrix + 0xc + axis * 2, component);
    }
    G3Function800202A4(matrix, frame + 0x10, 0x15, frame);
    G3Function8005BD00(frame + 0x10, matrix);
}

uint32 G3Function8003499C(uint32 vertices, uint32 model, uint32 transform, uint32 matrices)
{
    FUNCTION_MARKER(0x8003499c, "SLUS_008.06");
    uint32 output = r_u32(0x800a2188);
    sint32 count = r_s16(model + 6);
    uint32 records = r_u32(model + 0x18);
    if (count <= 0)
        return (uint32)count;
    uint32 record = records + 0xa;
    uint32 offset = 0;
    uint32 index = 0;
    do
    {
        uint32 current_records = r_u32(model + 0x18);
        sint32 end_index = r_s16(current_records + offset + 0xa);
        if (end_index != -1)
        {
            uint32 start_index = (uint32)r_s16(record - 2);
            uint32 last_index = (uint32)r_s16(record);
            uint32 vertex = vertices + (start_index << 3);
            uint32 last = vertices + (last_index << 3);
            uint32 first = r_u32(transform);
            uint32 second = r_u32(transform + 4);
            xport_gte_write_control(0, first);
            xport_gte_write_control(1, second);
            first = r_u32(transform + 8);
            second = r_u32(transform + 0xc);
            uint32 third = r_u32(transform + 0x10);
            xport_gte_write_control(2, first);
            xport_gte_write_control(3, second);
            xport_gte_write_control(4, third);
            for (uint32 column = 0; column < 3; ++column)
            {
                first = r_u16(matrices + column * 2);
                second = r_u16(matrices + column * 2 + 6);
                third = r_u16(matrices + column * 2 + 0xc);
                xport_gte_write_data(9, first);
                xport_gte_write_data(10, second);
                xport_gte_write_data(11, third);
                xport_gte_execute(0x49e012);
                first = xport_gte_read_data(9);
                second = xport_gte_read_data(10);
                third = xport_gte_read_data(11);
                w_u16(0x1f800000 + column * 2, first);
                w_u16(0x1f800006 + column * 2, second);
                w_u16(0x1f80000c + column * 2, third);
            }
            first = r_u32(transform + 0x14);
            second = r_u32(transform + 0x18);
            xport_gte_write_control(5, first);
            third = r_u32(transform + 0x1c);
            xport_gte_write_control(6, second);
            xport_gte_write_control(7, third);
            uint32 y = r_u16(matrices + 0x18);
            uint32 x = r_u16(matrices + 0x14);
            xport_gte_write_data(0, x | (y << 16));
            xport_gte_write_data(1, r_u32(matrices + 0x1c));
            xport_gte_execute(0x480012);
            for (uint32 axis = 0; axis < 3; ++axis)
                w_u32(0x1f800014 + axis * 4, xport_gte_read_data(25 + axis));
            first = r_u32(0x1f800000);
            second = r_u32(0x1f800004);
            xport_gte_write_control(0, first);
            xport_gte_write_control(1, second);
            first = r_u32(0x1f800008);
            second = r_u32(0x1f80000c);
            third = r_u32(0x1f800010);
            xport_gte_write_control(2, first);
            xport_gte_write_control(3, second);
            xport_gte_write_control(4, third);
            while (last >= vertex)
            {
                uint32 normal = r_u16(vertex + 6);
                uint32 normal_address = 0x80098838 + normal * 8;
                xport_gte_write_data(0, r_u32(normal_address));
                xport_gte_write_data(1, r_u32(normal_address + 4));
                xport_gte_execute(0x486012);
                for (uint32 axis = 0; axis < 3; ++axis)
                    w_u32(output + axis * 4, xport_gte_read_data(25 + axis));
                vertex += 8;
                output += 12;
            }
        }
        record += 0x18;
        matrices += 0x20;
        count = r_s16(model + 6);
        ++index;
        offset += 0x18;
    } while ((sint32)index < count);
    return 0;
}

uint32 G3Function80034FA4(uint32 sequence, uint32 time, uint32 *second_glyph, uint32 *third_glyph)
{
    FUNCTION_MARKER(0x80034fa4, "SLUS_008.06");
    if (sequence == 0 || time == 0xffffffff)
        return 0xffffffff;
    uint32 count = r_u32(sequence);
    if (third_glyph != 0)
        *third_glyph = 0;
    if ((sint32)count <= 0)
        return count;
    uint32 entry = sequence + 4;
    uint32 index = 0;
    do
    {
        uint32 duration = r_u32(entry + 8);
        uint32 quotient = duration == 0 ? 0xffffffff : time / duration;
        uint32 frames = r_u32(entry + 4);
        uint32 selected = frames == 0 ? quotient : quotient % frames;
        uint32 destination = r_u32(entry);
        uint32 source = destination + (selected << 4) + 0x10;
        for (uint32 word = 0; word < 4; ++word)
        {
            uint32 value = r_u32(source + word * 4);
            w_u32(destination + word * 4, value);
        }
        entry += 12;
        ++index;
        count = r_u32(sequence);
        if (second_glyph != 0)
            *second_glyph = entry + 4;
        if (third_glyph != 0)
            *third_glyph = index;
    } while ((sint32)index < (sint32)count);
    return 0;
}
