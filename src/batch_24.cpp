#include "xport.h"
#include "psx.h"
#include "game_functions.h"









uint32 G3Function80067A28(uint32 header)
{
    FUNCTION_MARKER(0x80067a28, "SLUS_008.06");
    return r_u8(header + 0x1f);
}





uint32 G3Function80067148(uint32 state)
{
    FUNCTION_MARKER(0x80067148, "SLUS_008.06");
    uint32 bytes = G3Function80067070(state);
    uint32 buffer = r_u32(state + 0x28);
    G3FillMemory80086094(buffer, 0, bytes);
    uint32 header = r_u32(state + 0x20);
    buffer = r_u32(state + 0x28);
    return G3WriteCardHeader80067A50(header, buffer);
}

uint32 G3Function800674F4(uint32 state)
{
    FUNCTION_MARKER(0x800674f4, "SLUS_008.06");
    uint32 best_sequence = 0;
    uint32 index = 0;
    uint32 selected = 0;
    sint32 count = r_s32(state + 0xc);
    if (count <= 0)
        return selected;
    do
    {
        uint32 buffer = G3Function80067110(state, index);
        uint32 data = r_u32(state + 0x24);
        uint32 sequence = G3ValidateCardData80067D34(data, buffer);
        if ((best_sequence & 0xffff) < (sequence & 0xffff))
        {
            selected = index;
            best_sequence = sequence;
        }
        count = r_s32(state + 0xc);
        ++index;
    } while ((sint32)index < count);
    return selected;
}



uint32 G3Function80067F4C(uint32 value)
{
    FUNCTION_MARKER(0x80067f4c, "SLUS_008.06");
    return value + 1;
}

uint32 G3Function80067F54(uint32 value)
{
    FUNCTION_MARKER(0x80067f54, "SLUS_008.06");
    return value + 1;
}

uint32 G3Function80067F5C(uint32 value)
{
    FUNCTION_MARKER(0x80067f5c, "SLUS_008.06");
    return value + 2;
}

uint32 G3Function80067FE8(uint32 io, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80067fe8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    sint32 *command = (sint32 *)psx_addr(frame + 0x10, 4);
    sint32 *result = (sint32 *)psx_addr(frame + 0x14, 4);
    uint32 status = (uint32)MemCardSync(0, command, result);
    G3Function80067F5C(status);
    w_u32(io + 4, 1);
    w_u32(io + 8, 1);
    return 1;
}

uint32 G3Function8006802C(uint32 io, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006802c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    MemCardStart();
    G3Function80067FE8(io, frame);
    uint32 flags = r_u32(io) | 4;
    w_u32(io, flags);
    return flags;
}

uint32 G3Function80068068(uint32 io, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80068068, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function80067FE8(io, frame);
    MemCardStop();
    uint32 flags = r_u32(io) & 0xfffffffb;
    w_u32(io, flags);
    return flags;
}











uint32 G3Function8006869C(uint32 io, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006869c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x288;
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0xf);
    uint32 channel = r_u32(io + 0x18);
    uint32 count_address = r_u32(io + 0x34);
    uint32 status = (uint32)MemCardGetDirentry((sint32)channel, (char *)psx_addr(0x8009ff68, 2), (DIRENTRY *)psx_addr(frame + 0x18, 0x258), (sint32 *)psx_addr(count_address, 4), 0, 0xf);
    uint32 result = G3Function80067F4C(status);
    if (result != 1)
        return result;
    count_address = r_u32(io + 0x34);
    if (r_s32(count_address) <= 0)
        return result;
    uint32 index = 0;
    uint32 source = frame + 0x18;
    uint32 offset = 0;
    do
    {
        uint32 destination = r_u32(io + 0x30) + offset;
        ++index;
        G3Function8008EAC8(destination, source, 0x14);
        destination = r_u32(io + 0x30);
        uint32 bytes = r_u32(source + 0x18);
        w_u32(destination + offset + 0x14, bytes);
        count_address = r_u32(io + 0x34);
        source += 0x28;
        sint32 count = r_s32(count_address);
        offset += 0x18;
        if ((sint32)index >= count)
            break;
    } while (true);
    return result;
}

uint32 G3Function80068780(uint32 io, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80068780, "SLUS_008.06");
    uint32 frame = guest_stack - 0x280;
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0xf);
    uint32 channel = r_u32(io + 0x18);
    uint32 status = (uint32)MemCardGetDirentry((sint32)channel, (char *)psx_addr(0x8009ff68, 2), (DIRENTRY *)psx_addr(frame + 0x18, 0x258), (sint32 *)psx_addr(frame + 0x270, 4), 0, 0xf);
    uint32 result = G3Function80067F4C(status);
    if (result != 1)
        return result;
    uint32 destination = r_u32(io + 0x38);
    w_u32(destination, 0);
    if (r_s32(frame + 0x270) <= 0)
        return result;
    uint32 index = 0;
    uint32 source = frame + 0x18;
    do
    {
        uint32 bytes = r_u32(source + 0x18);
        destination = r_u32(io + 0x38);
        uint32 rounded = bytes + 0x1fff;
        if ((sint32)rounded < 0)
            rounded = bytes + 0x3ffe;
        uint32 total = r_u32(destination);
        uint32 blocks = (uint32)((sint32)rounded >> 13);
        w_u32(destination, total + blocks);
        sint32 count = r_s32(frame + 0x270);
        ++index;
        source += 0x28;
        if ((sint32)index >= count)
            break;
    } while (true);
    return result;
}



void G3Function8006A624(void)
{
    FUNCTION_MARKER(0x8006a624, "SLUS_008.06");
}

uint32 G3Function800689B8(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800689b8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function8006A624();
    uint32 core = r_u32(state);
    uint32 base = r_u32(state + 4);
    w_u32(state + 0x30, 1);
    uint32 result = G3Function800675F4(core, base, frame);
    w_u32(state + 8, 0);
    G3Function8006A624();
    return result;
}

uint32 G3Function80068A10(uint32 state, uint32 guest_stack)
{
    w_u32(state + 0x30, 0);
    return 0;
}

uint32 G3Function80068BA4(uint32 state)
{
    FUNCTION_MARKER(0x80068ba4, "SLUS_008.06");
    return state + 0x10;
}

uint32 G3Function80068BAC(uint32 state)
{
    FUNCTION_MARKER(0x80068bac, "SLUS_008.06");
    return r_u32(state + 0x48);
}

uint32 G3Function80068BB8(uint32 state)
{
    FUNCTION_MARKER(0x80068bb8, "SLUS_008.06");
    return r_u32(state + 0x3c);
}

uint32 G3Function80068BCC(uint32 state)
{
    FUNCTION_MARKER(0x80068bcc, "SLUS_008.06");
    return r_u32(state + 0x38);
}

uint32 G3Function8006A600(uint32 context)
{
    FUNCTION_MARKER(0x8006a600, "SLUS_008.06");
    uint32 game = G3Function8006A5C0(context);
    return r_u32(game + 0x67e8);
}

uint32 G3Function8006A62C(uint32 *third_glyph)
{
    FUNCTION_MARKER(0x8006a62c, "SLUS_008.06");
    uint32 result;
    do
    {
        result = G3Function8003F0E8(0x827, third_glyph);
        if (result != 0)
            psx_native_poll_events();
    } while (result != 0);
    return result;
}

uint32 G3Function8006A654(uint32 event, uint32 *third_glyph)
{
    FUNCTION_MARKER(0x8006a654, "SLUS_008.06");
    uint32 entry = 0x80099e2c + (event << 3);
    if (event - 1 >= 9)
        return 0;
    uint32 selected = r_u32(entry);
    if (selected != event)
        return 1;
    uint32 sound = r_u16(entry + 4);
    G3TextMeasureOperands operands;
    if (third_glyph != 0)
        operands.third_glyph = *third_glyph;
    G3Function8003F128(sound, third_glyph != 0 ? &operands : 0);
    if (third_glyph != 0)
        *third_glyph = operands.third_glyph;
    if (selected == 9)
        return G3Function8006A62C(third_glyph);
    return 9;
}

uint32 G3Function8006B2B4(uint32 context, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006b2b4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 game = G3Function8006A5C0(context);
    G3Function8006A62C(0);
    return G3Function80027644(0x800a00a4, 0x800a00ac, game, frame);
}

void G3Function80068BE0(uint32 state, uint32 value)
{
    FUNCTION_MARKER(0x80068be0, "SLUS_008.06");
    w_u32(state + 0x34, value);
}

uint32 G3Function80068BE8(uint32 context, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80068be8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 state = G3Function8006A600(context);
    G3Function80068A10(state, frame);
    return G3Function8006B2B4(context, frame);
}

uint32 G3Function80068C1C(uint32 context, uint32 argument, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80068c1c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (event != 7)
        return 7;
    G3Function8006A654(3, 0);
    return G3Function80068BE8(context, frame);
}



uint32 G3Function800698C4(uint32 context, uint32 callback, uint32 data, uint32 argument, uint32 incoming_v0, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800698c4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 index = r_u32(context + 4);
    if (context == 0 || callback == 0 || (sint32)index < 0)
        return incoming_v0;
    uint32 next = index + 1;
    uint32 offset = index << 4;
    if ((sint32)next >= 8)
        return offset;
    uint32 entry = context + offset;
    uint32 previous_argument = r_u32(entry + 0x10);
    w_u32(context + 4, next);
    w_u32(frame + 0x10, argument);
    uint32 result = entry;
    G3Function80069848(context, callback, data, previous_argument, frame, &result);
    return result;
}



uint32 G3Function80069000(uint32 context, uint32 selection, uint32 incoming_third_glyph, uint32 guest_stack)
{
    return G3NativeSaveSlotsPage(context, selection, incoming_third_glyph, guest_stack);
}

uint32 G3Function800694B4(uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    if (event != 7 || selected > 1)
        return 7;
    uint32 frame = guest_stack - 0x20;
    uint32 state = G3Function8006A600(context);
    G3Function8006A654(3, 0);
    w_u32(state + 0xc, selected + 1);
    w_u32(state + 0x30, 1);
    w_u32(state + 0x40, 0);
    return G3Function800698C4(context, 0x80069000, 0, 0x80068e98, 0, frame);
}

uint32 G3Function8006ACD4(uint32 context)
{
    FUNCTION_MARKER(0x8006acd4, "SLUS_008.06");
    uint32 game = G3Function8006A5C0(context);
    if ((r_u32(game + 0x98) & 0xc00) != 0)
        return 1;
    uint32 pressed = r_u32(game + 0x9c);
    if ((pressed & 1) != 0)
        return 2;
    if ((pressed & 2) != 0)
        return 3;
    if ((pressed & 4) != 0)
        return 4;
    if ((pressed & 8) != 0)
        return 5;
    if ((pressed & 0x10) != 0)
        return 6;
    if ((pressed & 0x80) != 0)
        return 7;
    return 1;
}

uint32 G3Function8006983C(uint32 context)
{
    FUNCTION_MARKER(0x8006983c, "SLUS_008.06");
    return r_u32(context + 4) == 0 ? 1 : 0;
}

uint32 G3Function80069B00(uint32 context, uint32 selected, uint32 event, uint32 incoming_v0, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80069b00, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (context == 0)
        return incoming_v0;
    if ((sint32)selected < 0)
        return 1;
    uint32 offset = selected << 2;
    if ((sint32)selected >= 8)
        return offset;
    uint32 callback = r_u32(context + offset + 0x88);
    if (callback == 0)
        return 0;
    return G3DispatchMenuRowCallback(callback, context, selected, event, frame);
}

uint32 G3Function80069918(uint32 context, uint32 incoming_v0, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80069918, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 index = r_u32(context + 4);
    if (context == 0)
        return incoming_v0;
    if ((sint32)index <= 0)
        return 1;
    uint32 offset = index << 4;
    if ((sint32)index >= 8)
        return offset;
    uint32 callback = r_u32(context + offset + 0x14);
    if (callback != 0)
        G3DispatchMenuExitCallback(callback, context, frame);
    uint32 result = r_u32(context + 4) - 1;
    w_u32(context + 4, result);
    return result;
}

uint32 G3Function80069BC8(uint32 context, uint32 selected, uint32 minimum, uint32 maximum, uint32 incoming_v0, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80069bc8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 index = r_u32(context + 4);
    if (context == 0)
        return incoming_v0;
    if ((sint32)index < 0)
        return 1;
    uint32 offset = index << 4;
    if ((sint32)index >= 8)
        return offset;
    uint32 entry = context + offset + 8;
    uint32 action = G3Function8006ACD4(context);
    w_u32(entry + 4, selected);
    if ((sint32)selected >= (sint32)minimum && (sint32)maximum >= (sint32)selected)
        G3Function80069B00(context, selected, action, 3, frame);
    if (action == 2)
    {
        if (selected == minimum)
            w_u32(entry + 4, maximum - 1);
        else if ((sint32)minimum < (sint32)selected && (sint32)maximum >= (sint32)selected)
        {
            uint32 current = r_u32(entry + 4);
            w_u32(entry + 4, current - 1);
        }
    }
    else if (action == 3)
    {
        if (selected == maximum - 1)
            w_u32(entry + 4, minimum);
        else if ((sint32)selected >= (sint32)minimum && (sint32)(selected + 1) < (sint32)maximum)
        {
            uint32 current = r_u32(entry + 4);
            w_u32(entry + 4, current + 1);
        }
    }
    else
    {
        if (action < 4)
            return 2;
        if (action != 6)
            return 6;
        uint32 root = G3Function8006983C(context);
        if (root != 0)
            return root;
        uint32 result = G3Function8006A654(2, 0);
        return G3Function80069918(context, result, frame);
    }
    uint32 current = r_u32(entry + 4);
    if (selected != current)
        return G3Function8006A654(1, 0);
    return current;
}

uint32 G3Function80068E98(uint32 context, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80068e98, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 state = G3Function8006A600(context);
    return G3Function80068A10(state, frame);
}

void G3Function8006A6BC(void)
{
    FUNCTION_MARKER(0x8006a6bc, "SLUS_008.06");
}

uint32 G3Function8006A9E4(uint32 game, uint32 text, uint32 incoming_third_glyph, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006a9e4, "SLUS_008.06");
    G3TextMeasureOperands operands;
    operands.third_glyph = incoming_third_glyph;
    uint32 frame = guest_stack - 0x30;
    uint32 maximum = 0;
    uint32 line = 0;
    for (;;)
    {
        uint32 terminator = r_u8(text);
        uint32 end = text;
        while (terminator != 0 && terminator != 0xa)
        {
            ++end;
            terminator = r_u8(end);
        }
        w_u8(end, 0);
        uint32 step = r_u32(0x800a0074);
        uint32 advance = line * step;
        uint32 origin = r_u32(0x800a0070);
        uint32 width = G3Function8006A6C4(game, origin + advance, text, operands.third_glyph, frame, &operands);
        if ((sint32)maximum < (sint32)width)
            maximum = width;
        text = end + 1;
        if (terminator == 0)
            break;
        w_u8(end, terminator);
        ++line;
    }
    uint32 step = r_u32(0x800a0074);
    (void)r_u32(0x800a0070);
    G3Function8006A6BC();
    return step;
}

uint32 G3Function8006AAD4(uint32 context, uint32 text, uint32 incoming_third_glyph, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006aad4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 game = G3Function8006A5C0(context);
    return G3Function8006A9E4(game, text, incoming_third_glyph, frame);
}

uint32 G3Function80069804(uint32 context)
{
    FUNCTION_MARKER(0x80069804, "SLUS_008.06");
    sint32 index = r_s32(context + 4);
    if (context == 0 || index < 0 || index >= 8)
        return 0;
    return r_u32(context + ((uint32)index << 4) + 0x10);
}

uint32 G3Function8006A308(uint32 context, uint32 argument, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006a308, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (event != 7)
        return 7;
    uint32 result = G3Function8006A654(3, 0);
    return G3Function800698C4(context, 0x8006a244, 0, 0, result, frame);
}

uint32 G3Function8006A350(uint32 context, uint32 selected, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006a350, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 text = G3Function80065D38(0x28);
    G3Function80069A4C(context, 0, text, 0x800694b4, frame);
    text = G3Function80065D38(0x30);
    G3Function80069A4C(context, 1, text, 0x800694b4, frame);
    text = G3Function80065D38(0xb8);
    uint32 result = G3Function80069A4C(context, 2, text, 0x8006a308, frame);
    return G3Function80069BC8(context, selected, 0, 3, result, frame);
}

uint32 G3Function8006A5E0(uint32 state)
{
    FUNCTION_MARKER(0x8006a5e0, "SLUS_008.06");
    return G3Function80068BAC(state);
}

uint32 G3Function8006A72C(uint32 text)
{
    FUNCTION_MARKER(0x8006a72c, "SLUS_008.06");
    uint32 value = r_u8(text);
    uint32 source = text;
    uint32 destination = text;
    while (value != 0)
    {
        value = r_u8(source);
        if (value != 0x3a)
        {
            w_u8(destination, value);
            ++destination;
        }
        ++source;
        value = r_u8(source);
    }
    w_u8(destination, 0);
    return value;
}

uint32 G3Function8006A774(uint32 text, uint32 parentheses, uint32 *text_end, uint32 *source_end)
{
    FUNCTION_MARKER(0x8006a774, "SLUS_008.06");
    bool inside = false;
    uint32 destination = text;
    uint32 value = r_u8(destination);
    uint32 source = destination;
    while (value != 0)
    {
        value = r_u8(source);
        if (value == 0x28)
        {
            inside = true;
            w_u8(parentheses, value);
            ++parentheses;
            if (text < destination && r_u8(destination - 1) == 0x20)
                --destination;
        }
        else if (value == 0x29)
        {
            inside = false;
            w_u8(parentheses, value);
            ++parentheses;
        }
        else if (inside)
        {
            w_u8(parentheses, value);
            ++parentheses;
        }
        else
        {
            w_u8(destination, value);
            ++destination;
        }
        ++source;
        value = r_u8(source);
    }
    w_u8(destination, 0);
    w_u8(parentheses, 0);
    if (text_end != 0)
        *text_end = destination;
    if (source_end != 0)
        *source_end = source;
    return value;
}

uint32 G3Function8008102C(void)
{
    FUNCTION_MARKER(0x8008102c, "SLUS_008.06");
    return r_u32(0x800a033c);
}

uint32 G3Function8004DF3C(void)
{
    FUNCTION_MARKER(0x8004df3c, "SLUS_008.06");
    return r_u32(0x800a0628) != 0 ? 1 : 0;
}

uint32 G3Function8006B58C(uint32 state, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006b58c, "SLUS_008.06");
    uint32 game = G3Function8006A5E0(state);
    uint32 elapsed = G3Function80068BB8(state);
    w_u32(output, elapsed);
    uint32 selected = G3Function80068BCC(state);
    w_u32(output + 4, selected);
    w_u32(output + 8, r_u16(game + 0x68b4));
    w_u32(output + 0xc, r_u32(game + 0x69e0));
    w_u32(output + 0x10, r_u32(game + 0x6818));
    w_u16(output + 0x14, r_u16(game + 0x681c));
    w_u16(output + 0x16, r_u16(game + 0x681e));
    w_u32(output + 0x18, r_u32(game + 0x6824));
    w_u32(output + 0x1c, r_u32(game + 0x6828));
    w_u32(output + 0x20, r_u32(game + 0x682c));
    w_u32(output + 0x24, r_u32(game + 0x6830));
    w_u32(output + 0x28, r_u32(game + 0x6838));
    w_u32(output + 0x30, r_u32(game + 0x67d0));
    w_u32(output + 0x2c, r_u32(game + 0x67d4));
    w_u32(output + 0x34, r_u32(game + 0x67d8));
    uint32 value = G3Function8008102C();
    w_u32(output + 0x38, value);
    value = G3Function8004DF3C();
    w_u32(output + 0x3c, value);
    uint32 first = r_u32(game + 0x69c8);
    uint32 second = r_u32(game + 0x69cc);
    uint32 third = r_u32(game + 0x69d0);
    uint32 fourth = r_u32(game + 0x69d4);
    w_u32(output + 0x40, first);
    w_u32(output + 0x44, second);
    w_u32(output + 0x48, third);
    w_u32(output + 0x4c, fourth);
    for (uint32 region = 0; region < 2; ++region)
    {
        uint32 source = game + (region == 0 ? 0x683c : 0x6874);
        uint32 destination = output + (region == 0 ? 0x50 : 0x88);
        for (uint32 offset = 0; offset < 0x30; offset += 0x10)
        {
            first = r_u32(source + offset);
            second = r_u32(source + offset + 4);
            third = r_u32(source + offset + 8);
            fourth = r_u32(source + offset + 0xc);
            w_u32(destination + offset, first);
            w_u32(destination + offset + 4, second);
            w_u32(destination + offset + 8, third);
            w_u32(destination + offset + 0xc, fourth);
        }
        first = r_u32(source + 0x30);
        second = r_u32(source + 0x34);
        w_u32(destination + 0x30, first);
        w_u32(destination + 0x34, second);
    }
    first = r_u32(game + 0x67f8);
    second = r_u32(game + 0x67fc);
    third = r_u32(game + 0x6800);
    fourth = r_u32(game + 0x6804);
    w_u32(output + 0xc0, first);
    w_u32(output + 0xc4, second);
    w_u32(output + 0xc8, third);
    w_u32(output + 0xcc, fourth);
    first = r_u32(game + 0x6808);
    second = r_u32(game + 0x680c);
    third = r_u32(game + 0x6810);
    w_u32(output + 0xd0, first);
    w_u32(output + 0xd4, second);
    w_u32(output + 0xd8, third);
    w_u32(output + 0xdc, 0xabacabcd);
    return 0xabacabcd;
}

uint32 G3Function8006B77C(uint32 state, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006b77c, "SLUS_008.06");
    uint32 name = G3Function80068BA4(state);
    if (payload == 0)
    {
        w_u8(name, 0);
        G3Function80068BE0(state, 0xffffffff);
        return name;
    }
    uint32 magic = r_u32(payload + 0xdc);
    if (magic != 0xabacabcd)
    {
        w_u8(name, 0);
        G3Function80068BE0(state, 0xffffffff);
        return 0x800a0000;
    }
    uint32 level = r_u32(payload + 8);
    uint32 descriptor = 0x80099708 + (((level << 3) - level) << 2);
    uint32 token = (uint32)r_s16(descriptor + 6);
    if (token == 0)
        token = 0x1d;
    uint32 title = G3Function80065D38(token);
    uint32 value = r_u32(payload + 0x20);
    uint32 result = G3SdkSprintfStringInteger80088A1C(name, 0x800a00b8, title, value);
    uint32 selected = r_u32(payload);
    G3Function80068BE0(state, selected);
    return result;
}

uint32 G3Function8008101C(void)
{
    FUNCTION_MARKER(0x8008101c, "SLUS_008.06");
    w_u32(0x800a033c, 1);
    return 1;
}

uint32 G3Function8004DF00(uint32 enabled, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004df00, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 game = r_u32(0x80097048);
    w_u32(0x800a0628, enabled);
    uint32 object = r_u32(game + 0x4c);
    return G3Function80053F98(object, 0, 0x1a, enabled, frame);
}

uint32 G3Function8006B2FC(uint32 game, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006b2fc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    w_u32(game + 0x67d4, r_u32(payload + 0x2c));
    G3Function8003EE30(r_u32(payload + 0x2c));
    w_u32(game + 0x67d0, r_u32(payload + 0x30));
    G3Function8003EDE8(r_u32(payload + 0x30));
    w_u32(game + 0x67d8, r_u32(payload + 0x34));
    G3Function8003EE78(r_u32(payload + 0x34));
    if (r_u32(payload + 0x38) != 0)
        G3Function8008101C();
    else
        G3Function80081010();
    uint32 enabled = r_u32(payload + 0x3c);
    return G3Function8004DF00(enabled, frame);
}

void G3Function80068BC4(uint32 state, uint32 value)
{
    FUNCTION_MARKER(0x80068bc4, "SLUS_008.06");
    w_u32(state + 0x3c, value);
}

void G3Function80068BD8(uint32 state, uint32 value)
{
    FUNCTION_MARKER(0x80068bd8, "SLUS_008.06");
    w_u32(state + 0x38, value);
}

uint32 G3Function8006B3A0(uint32 state, uint32 payload, uint32 mode, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006b3a0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 game = G3Function8006A5E0(state);
    if (r_u32(payload + 0xdc) != 0xabacabcd)
        return 0;
    G3Function8006B2FC(game, payload, frame);
    if (mode != 0)
        return 1;
    G3Function80068BC4(state, r_u32(payload));
    G3Function80068BD8(state, r_u32(payload + 4));
    w_u32(game + 0x69e0, r_u32(payload + 0xc));
    uint32 quantity = r_u32(payload + 0x10);
    w_u32(game + 0x6818, quantity);
    if ((sint32)quantity < 5)
        w_u32(game + 0x6818, 5);
    w_u16(game + 0x681c, r_u16(payload + 0x14));
    w_u16(game + 0x681e, r_u16(payload + 0x16));
    w_u32(game + 0x6824, r_u32(payload + 0x18));
    w_u32(game + 0x6828, r_u32(payload + 0x1c));
    w_u32(game + 0x682c, r_u32(payload + 0x20));
    w_u32(game + 0x6830, r_u32(payload + 0x24));
    w_u32(game + 0x6838, r_u32(payload + 0x28));
    uint32 first = r_u32(payload + 0x40);
    uint32 second = r_u32(payload + 0x44);
    uint32 third = r_u32(payload + 0x48);
    uint32 fourth = r_u32(payload + 0x4c);
    w_u32(game + 0x69c8, first);
    w_u32(game + 0x69cc, second);
    w_u32(game + 0x69d0, third);
    w_u32(game + 0x69d4, fourth);
    for (uint32 region = 0; region < 2; ++region)
    {
        uint32 source = payload + (region == 0 ? 0x50 : 0x88);
        uint32 destination = game + (region == 0 ? 0x683c : 0x6874);
        for (uint32 offset = 0; offset < 0x30; offset += 0x10)
        {
            first = r_u32(source + offset);
            second = r_u32(source + offset + 4);
            third = r_u32(source + offset + 8);
            fourth = r_u32(source + offset + 0xc);
            w_u32(destination + offset, first);
            w_u32(destination + offset + 4, second);
            w_u32(destination + offset + 8, third);
            w_u32(destination + offset + 0xc, fourth);
        }
        first = r_u32(source + 0x30);
        second = r_u32(source + 0x34);
        w_u32(destination + 0x30, first);
        w_u32(destination + 0x34, second);
    }
    first = r_u32(payload + 0xc0);
    second = r_u32(payload + 0xc4);
    third = r_u32(payload + 0xc8);
    fourth = r_u32(payload + 0xcc);
    w_u32(game + 0x67f8, first);
    w_u32(game + 0x67fc, second);
    w_u32(game + 0x6800, third);
    w_u32(game + 0x6804, fourth);
    first = r_u32(payload + 0xd0);
    second = r_u32(payload + 0xd4);
    third = r_u32(payload + 0xd8);
    w_u32(game + 0x6808, first);
    w_u32(game + 0x680c, second);
    w_u32(game + 0x6810, third);
    return 1;
}

uint32 G3Function8006AB04(uint32 context, uint32 position, uint32 text, uint32 selected, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006ab04, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    uint32 requested_scale = r_u32(guest_stack + 0x10);
    uint32 game = G3Function8006A5C0(context);
    G3Function80069804(context);
    uint32 scale = 0x800;
    if ((sint32)requested_scale >= 0x800)
        scale = (sint32)requested_scale < 0x1001 ? requested_scale : 0x1000;
    uint32 shrink = ((scale - 0x800) << 4) - (scale - 0x800);
    shrink <<= 4;
    if ((sint32)shrink < 0)
        shrink += 0x7ff;
    uint32 size = 0x26c - (uint32)((sint32)shrink >> 11);
    uint32 spacing = r_u32(0x800a0078);
    uint32 row_step = ((spacing << 1) + spacing) << 2;
    uint32 argument = G3Function80069804(context);
    uint32 product = position * row_step;
    sint64 wide_product = (sint64)(sint32)product * 0x2aaaaaab;
    uint32 scaled = ((scale << 2) + scale) << 2;
    uint32 origin = r_u32(0x800a007c);
    uint32 high = (uint32)((uint64)wide_product >> 32);
    uint32 quotient = (uint32)((sint32)high >> 1) - (uint32)((sint32)product >> 31);
    uint32 y = origin + quotient + argument;
    if ((sint32)scaled < 0)
        scaled += 0x7ff;
    y += (uint32)((sint32)scaled >> 11);
    y -= 0x14;
    G3CopyString8008935C(frame + 0x18, text);
    G3Function8006A72C(frame + 0x18);
    w_u32(frame + 0x10, size);
    return G3Function8006A824(game, y, frame + 0x18, selected, size, frame);
}

uint32 G3Function8006C110(uint32 scene, uint32 parent, uint32 vertices, uint32 packets, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006c110, "SLUS_008.06");
    uint32 frame = guest_stack - 0x1c0;
    uint32 context = r_u32(0x8009efc4);
    w_u32(guest_stack, scene);
    w_u32(guest_stack + 8, vertices);
    w_u32(guest_stack + 0xc, packets);
    uint32 attachments = r_u32(context + 0x24);
    uint32 order_table = r_u32(guest_stack + 0x10);
    uint32 current = attachments;
    for (uint32 slot = 0; slot < 3; ++slot, current += 0x10)
    {
        uint32 first_name = r_u32(current + 0x48);
        uint32 first_empty = r_u32(0x800a00d8);
        if (first_name == first_empty)
        {
            uint32 second_name = r_u32(current + 0x4c);
            uint32 second_empty = r_u32(0x800a00dc);
            if (second_name == second_empty)
                continue;
        }
        uint32 resource = G3Function8003AA48(attachments + (slot << 4) + 0x48);
        if (resource == 0)
            continue;
        uint32 models = r_u32(resource + 0xc);
        uint32 object = r_u32(current + 0x50);
        uint32 model = r_u32(models);
        if (object != 0)
        {
            if (r_u32(object + 0x30) == 0)
                continue;
            context = r_u32(0x8009efc4);
            uint32 generation = r_u16(context + 0x6e);
            if (r_u32(object + 0x104) != generation)
                w_u16(object + 0x6e, 0);
            G3Function80029F0C(object, 0xffffffff, frame);
            for (uint32 coordinate = 0; coordinate < 3; ++coordinate)
            {
                uint32 index = r_u32(current + 0x44);
                uint32 matrices = r_u32(parent + 0x30);
                uint32 value = r_u16(matrices + (index << 5) + 0x14 + (coordinate << 2));
                w_u16(object + 0x58 + (coordinate << 1), value);
            }
            uint32 index = r_u32(current + 0x44);
            uint32 matrices = r_u32(parent + 0x30);
            G3Function800202A4(matrices + (index << 5), object + 0x60, 0x15, frame);
            matrices = r_u32(object + 0x30);
            uint32 saved = r_u32(object + 0x34);
            G3Function800328A0(object, resource, matrices - 0x20, model, frame);
            w_u32(object + 0x34, saved);
            G3Function8002B168(object, frame);
            uint32 saved_scene = r_u32(guest_stack);
            uint32 saved_vertices = r_u32(guest_stack + 8);
            uint32 saved_packets = r_u32(guest_stack + 0xc);
            w_u32(frame + 0x10, order_table);
            w_u32(frame + 0x14, 0);
            w_u32(frame + 0x18, 0x2710);
            G3Function80033BA8(object, saved_scene, saved_vertices, saved_packets, order_table, 0, 0x2710, frame);
        }
        else
        {
            uint32 index = r_u32(current + 0x44);
            G3FillMemory80086094(frame + 0x20, 0, 0x158);
            w_u32(frame + 0x3c, resource);
            w_u32(frame + 0x50, frame + 0x178);
            uint32 offset = index << 5;
            object = frame + 0x20;
            for (uint32 coordinate = 0; coordinate < 3; ++coordinate)
            {
                uint32 matrices = r_u32(parent + 0x30);
                uint32 value = r_u16(matrices + offset + 0x14 + (coordinate << 2));
                w_u16(object + 0x58 + (coordinate << 1), value);
            }
            uint32 source = r_u32(parent + 0x30) + offset;
            for (uint32 block = 0; block < 0x20; block += 0x10)
            {
                uint32 first = r_u32(source + block);
                uint32 second = r_u32(source + block + 4);
                uint32 third = r_u32(source + block + 8);
                uint32 fourth = r_u32(source + block + 0xc);
                w_u32(frame + 0x178 + block, first);
                w_u32(frame + 0x17c + block, second);
                w_u32(frame + 0x180 + block, third);
                w_u32(frame + 0x184 + block, fourth);
            }
            uint32 flags = r_u32(object + 0x10) & 0xffdfffff;
            w_u32(object + 0x10, flags);
            if ((r_u32(parent + 0x10) & 0x200000) != 0)
                w_u32(object + 0x10, flags | 0x200000);
            flags = r_u32(object + 0x10) & 0xffdfffff;
            w_u32(object + 0x10, flags);
            uint32 value = r_u16(parent + 0x8e);
            w_u32(object + 0xd0, 0);
            w_u16(object + 0x8e, value);
            G3Function8002B168(object, frame);
            uint32 saved_scene = r_u32(guest_stack);
            uint32 saved_vertices = r_u32(guest_stack + 8);
            uint32 saved_packets = r_u32(guest_stack + 0xc);
            w_u16(object + 0xa2, 0);
            w_u32(frame + 0x10, order_table);
            w_u32(frame + 0x14, 0);
            w_u32(frame + 0x18, 0x2710);
            G3Function80033BA8(object, saved_scene, saved_vertices, saved_packets, order_table, 0, 0x2710, frame);
            uint32 game = r_u32(0x80097048);
            if (r_u32(game + 0x6788) == 0)
                continue;
            if ((r_u32(parent + 0x10) & 0x20000000) == 0)
            {
                uint32 owner = r_u32(parent + 0x1c);
                if ((r_u32(owner + 0x30) & 0x800000) == 0)
                    continue;
            }
            saved_scene = r_u32(guest_stack);
            saved_vertices = r_u32(guest_stack + 8);
            w_u32(frame + 0x10, order_table);
            uint32 overlay = r_u32(game + 0x6788);
            saved_packets = r_u32(guest_stack + 0xc);
            w_u32(frame + 0x18, 0x2710);
            w_u32(frame + 0x14, overlay);
            G3Function80033BA8(object, saved_scene, saved_vertices, saved_packets, order_table, overlay, 0x2710, frame);
        }
    }
    return 0;
}

void G3Function8006D4C8(void)
{
    FUNCTION_MARKER(0x8006d4c8, "SLUS_008.06");
}

uint32 G3Function8006DFB8(uint32 object, uint32 game, uint32 result_address, uint32 argument3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006dfb8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 contact = r_u32(object + 0xb0);
    uint32 state = r_u32(object + 0x24);
    uint32 primitive = r_u32(contact + 8);
    uint32 face = r_u32(contact + 0xc);
    sint32 type = r_s16(contact + 4);
    sint32 shape = r_s16(contact + 6);
    uint32 other = r_u32(contact + 0x14);
    w_u32(result_address, 1);
    if (other != 0)
    {
        uint32 owner = r_u32(other + 0x1c);
        if (r_s32(owner + 0x30) < 0)
        {
            uint32 flags = r_u32(other + 0x10) | 0x80000000;
            w_u32(other + 0x10, flags);
            if ((flags & 0x4000000) == 0)
            {
                uint32 callback = r_u32(other + 0xec);
                if (callback != 0)
                    G3DispatchStateEvent(callback, other, game, 0x7da, object, frame);
            }
        }
    }
    if (type == 4 && shape == 3)
    {
        uint32 current_face = r_u32(contact + 0xc);
        uint32 flags = r_u16(current_face + 6);
        uint32 level = r_u32(game + 0x48);
        uint32 segments = r_u32(level + 0xf0);
        uint32 sequence = r_u32(segments + ((flags & 0x1e0) >> 1) + 0xc);
        if (sequence != 0)
            G3Function80018DE0(object, sequence + 4, frame);
    }
    uint32 result = G3Function8006DD7C(object, game, frame);
    if (result == 0)
        return result;
    uint32 scratch = r_u32(0x800a0284);
    G3Function800388C8(contact, scratch + 0x14, game, frame);
    result = r_u8(primitive + 4);
    if (result != 0)
        return result;
    scratch = r_u32(0x800a0284);
    w_u32(scratch + 0xc, face);
    uint32 flags = r_u16(state + 0xce);
    uint32 state_flags = r_u32(state + 0xb8);
    flags |= 4;
    state_flags |= 4;
    w_u16(state + 0xce, flags);
    w_u32(state + 0xb8, state_flags);
    result = r_u32(object + 0x150);
    if (result == 0)
    {
        uint32 face_flags = r_u16(face + 6);
        result = face_flags & 1;
        if (result == 0)
        {
            uint32 current_game = r_u32(0x80097048);
            uint32 level = r_u32(current_game + 0x48);
            uint32 segments = r_u32(level + 0xf0);
            result = r_u32(segments + ((face_flags & 0x1e0) >> 1)) & 1;
            if (result != 0)
            {
                uint32 magnitude = (uint32)r_s16(state + 0x10);
                result = G3Function80073CE8(game, magnitude, face, frame);
            }
        }
    }
    w_u32(result_address, 0);
    return result;
}

uint32 G3Function80071490(uint32 object)
{
    FUNCTION_MARKER(0x80071490, "SLUS_008.06");
    if ((r_u32(object + 0x128) & 0x10000000) != 0)
        return 1;
    uint32 scratch = r_u32(0x800a0284);
    uint32 face = r_u32(scratch + 0xc);
    if (face == 0)
        return 0;
    return (r_u16(face + 6) & 0xf) == 4 ? 1 : 0;
}

void G3Function800714E0(uint32 object, uint32 output, uint32 argument2, uint32 argument3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800714e0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    if (output == 0)
        return;
    w_u16(output + 6, 0x50);
    if (G3Function80071490(object) != 0)
    {
        uint32 scratch = r_u32(0x800a0284);
        w_u16(frame + 0x10, r_u16(scratch + 0x14));
        w_u16(frame + 0x12, r_u16(scratch + 0x16));
        w_u16(frame + 0x14, r_u16(scratch + 0x18));
        uint32 length = (uint32)r_s16(output + 6);
        G3Function8002F9A8(frame + 0x18, length, frame + 0x10, frame);
        uint32 offset = r_u16(frame + 0x18);
        uint32 position = r_u16(object + 0x58);
        w_u16(output, offset + position);
        offset = r_u16(frame + 0x1a);
        position = r_u16(object + 0x5a);
        w_u16(output + 2, offset + position);
        offset = r_u16(frame + 0x1c);
        position = r_u16(object + 0x5c);
        w_u16(output + 4, offset + position);
    }
    else
    {
        uint32 source = object + 0x58;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 low_lane = source & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (0xf << low_lane) & 0xf);
        uint32 word = high << ((3 - high_lane) * 8);
        if (low_lane == 0)
            word = low;
        else
            word = (word & (0xffffffff << ((4 - low_lane) * 8))) | (low >> (low_lane * 8));
        uint32 z = (uint32)r_s16(object + 0x5c);
        high_address = output + 3;
        high_lane = high_address & 3;
        low_lane = output & 3;
        w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        w_masked_u32(output & 0xfffffffc, word << (low_lane * 8), (0xf << low_lane) & 0xf);
        w_u16(output + 4, z);
        z = r_u16(output + 4);
        w_u16(output + 4, z + 0x80);
    }
}

uint32 G3Function8007205C(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007205c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 scratch = r_u32(0x800a0284);
    uint32 face = r_u32(scratch + 0xc);
    if (face == 0)
        return 0;
    uint32 type = r_u16(face + 6) & 0xf;
    if (type != 4 && (r_u32(object + 0x128) & 0x10000000) == 0)
        return 0;
    uint32 normal = scratch + 0x14;
    if (r_s16(normal) == 0 && r_s16(normal + 2) == 0)
        w_u16(normal, 0x7d);
    w_u32(state + 0x1dc, 0);
    uint32 y = r_u16(normal + 2);
    uint32 x = (uint32)r_s16(normal);
    w_u16(frame + 0x12, y);
    uint32 z = r_u16(normal + 4);
    w_u16(frame + 0x10, 0);
    w_u16(frame + 0x14, z);
    uint32 length = G3Function8002FB10(frame + 0x10);
    y = (uint32)r_s16(frame + 0x12);
    z = (uint32)r_s16(frame + 0x14);
    uint32 pitch = G3Function8002F0C4(y, z, frame);
    w_u16(object + 0x60, 0 - pitch);
    uint32 yaw = G3Function8002F0C4(x, length, frame);
    w_u16(object + 0x62, yaw);
    if ((r_u16(state + 0xce) & 1) == 0)
    {
        uint32 position = r_u16(object + 0x5c);
        w_u16(object + 0x5c, position + 0x100);
        for (uint32 coordinate = 0; coordinate < 3; ++coordinate)
        {
            uint32 component = r_u16(normal + (coordinate << 1));
            position = r_u16(object + 0x58 + (coordinate << 1));
            uint32 offset = (uint32)((sint32)(component << 16) >> 21);
            w_u16(object + 0x58 + (coordinate << 1), position - offset);
        }
    }
    uint32 flags = r_u32(object + 0x154) & 6;
    w_u32(object + 0x150, 1);
    if (flags != 0 && r_s16(normal + 4) >= 0xaf1)
    {
        uint32 owner = r_u32(object + 0x1c);
        w_u32(object + 0x154, 0x1000000);
        w_u16(object + 0x6e, 0x26);
        uint32 model = r_u32(owner + 0x10);
        uint32 animation = r_u32(model + 0x98);
        uint32 count = r_u16(animation + 2);
        w_u16(object + 0x5e, count - 1);
    }
    else
    {
        w_u16(object + 0x6e, 0xc);
        w_u32(object + 0x154, 2);
        w_u16(object + 0x5e, 0);
    }
    w_u32(object + 0x10c, 0);
    uint32 audio = r_u32(game + 0x4c);
    G3Function8004D9D0(audio, 0, frame);
    G3Function8007193C(object, game, frame);
    return G3Function80071834(object, game, frame);
}

uint32 G3Function80072D00(uint32 object, uint32 argument1, uint32 state, uint32 argument3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80072d00, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    r_masked_u32(0x800a029c, 15);
    uint32 size = r_masked_u32(0x800a029c, 15);
    uint32 z_size = (uint32)r_s16(0x800a02a0);
    uint32 destination = frame + 0x10;
    uint32 high_address = destination + 3;
    uint32 high_lane = high_address & 3;
    uint32 low_lane = destination & 3;
    w_masked_u32(high_address & 0xfffffffc, size >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    w_masked_u32(destination & 0xfffffffc, size << (low_lane * 8), (0xf << low_lane) & 0xf);
    w_u16(frame + 0x14, z_size);
    uint32 flags = r_u32(object + 0x10);
    uint32 distance = r_u32(guest_stack + 0x10);
    uint32 result = flags & 0x40000000;
    if (result != 0)
        return result;
    uint32 resource = r_u32(0x80098724);
    if (resource == 0)
        return 0x800a0000;
    uint32 angle = ((uint32)r_s16(state + 0xc8) - 0x400) & 0xfff;
    uint32 sine = G3Function8005B470(angle);
    uint32 product = sine * distance;
    angle = ((uint32)r_s16(state + 0xc8) - 0x400) & 0xfff;
    uint32 x = (uint32)((sint32)product >> 12);
    uint32 cosine = G3Function8005B590(angle);
    product = cosine * distance;
    uint32 y = (uint32)((sint32)product >> 12);
    uint32 random = G3SdkRandom80027FA8();
    uint32 position = r_u16(object + 0x58);
    w_u16(frame + 0x18, position + (x << 3) + (random & 0xf) - 7);
    random = G3SdkRandom80027FA8();
    position = r_u16(object + 0x5a);
    w_u16(frame + 0x1a, position + (y << 3) + (random & 0xf) - 7);
    random = G3SdkRandom80027FA8();
    position = r_u16(object + 0x5c);
    w_u16(frame + 0x1c, position + (random & 7) + 5);
    uint32 models = r_u32(resource + 0xc);
    uint32 pool = r_u32(0x800a27fc);
    uint32 model = r_u32(models);
    uint32 effect = G3Function800444E4(frame + 0x18, model, pool, 0x14, frame);
    if (effect == 0)
        return 0;
    result = G3Function800449F0(effect, x, y, 4, effect);
    G3Function80044878(effect, frame + 0x10, &result);
    return result;
}

uint32 G3Function80078684(uint32 object)
{
    FUNCTION_MARKER(0x80078684, "SLUS_008.06");
    uint32 player = r_u32(0x8009efc4);
    uint32 state = r_u32(player + 0x24);
    if (object != player)
        return 0;
    uint32 linked = r_u32(state + 0x174);
    if (linked == 0)
        return 0;
    uint32 owner = r_u32(linked + 0x1c);
    uint32 name = r_u32(owner + 0x20);
    uint32 expected = r_u32(0x800a02f8);
    uint32 actual = r_u32(name);
    if (actual != expected)
        return 0;
    actual = r_u32(name + 4);
    expected = r_u32(0x800a02fc);
    return actual == expected ? 1 : 0;
}

void G3Function80073560(uint32 object, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80073560, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    if ((r_u32(object + 0x10) & 0x40000000) != 0)
        return;
    uint32 first;
    uint32 second;
    if (G3Function80078684(object) != 0)
    {
        uint32 resource = r_u32(0x8009871c);
        uint32 models = r_u32(resource + 0xc);
        first = r_u32(models + 0x14);
        second = r_u32(models + 8);
    }
    else
    {
        uint32 first_resource = r_u32(0x80098724);
        uint32 second_resource = r_u32(0x8009872c);
        if (first_resource == 0 || second_resource == 0)
            return;
        uint32 first_models = r_u32(first_resource + 0xc);
        uint32 second_models = r_u32(second_resource + 0xc);
        first = r_u32(first_models);
        second = r_u32(second_models);
    }
    G3Function80044360(object, first, second, pool, frame);
}

uint32 G3Function80073D44(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80073d44, "SLUS_008.06");
    uint32 player = r_u32(0x8009efc4);
    uint32 frame = guest_stack - 0x20;
    uint32 state = r_u32(player + 0x24);
    uint32 linked = r_u32(state + 0x17c);
    if (linked != 0)
    {
        uint32 result = G3Function800740B0(linked);
        w_u32(state + 0x17c, 0);
        return result;
    }
    if (r_s16(state + 0x122) != 0)
        G3Function800737E4(player, game, frame);
    if ((r_u32(game + 0xe8) & 0x40000) != 0 || r_s16(game + 0x681c) != 1)
    {
        if ((r_u32(game + 0x6900) & 0x400) == 0)
        {
            uint32 count = r_u16(game + 0x681c);
            w_u16(game + 0x681c, count - 1);
        }
    }
    if (r_s16(game + 0x681c) == 0)
    {
        player = r_u32(0x8009efc4);
        if ((r_u32(player + 0x128) & 0x80) != 0)
            w_u16(game + 0x681c, 1);
        else
            G3Function80073630(player, game, frame);
    }
    return G3Function8007446C(game);
}

uint32 G3Function80073854(uint32 object, uint32 player, uint32 state, uint32 mode, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80073854, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    sint32 object_y = r_s16(object + 0x5a);
    sint32 player_y = r_s16(player + 0x5a);
    uint32 apply_velocity = r_u32(guest_stack + 0x10);
    uint32 delta_y = (uint32)object_y - (uint32)player_y;
    sint32 object_x = r_s16(object + 0x58);
    sint32 player_x = r_s16(player + 0x58);
    uint32 delta_x = (uint32)object_x - (uint32)player_x;
    uint32 angle = G3Function8002F0C4(delta_y, delta_x, frame);
    if (r_u32(player + 0x150) - 1 < 3)
    {
        w_u16(state + 0x120, 0x3c);
        return 0x3c;
    }
    if (object == player)
        w_u16(player + 0x6e, 0x10);
    else
    {
        uint32 difference = (angle - 0x400 - r_u16(state + 0xc6)) & 0xfff;
        uint32 threshold = mode != 0 ? 0x401 : 0x200;
        uint32 animation;
        bool directed = true;
        if (difference < threshold || difference > 0x1000 - threshold)
            animation = 0x2f;
        else if (difference > threshold + 0x400 && difference < threshold + 0x800)
            animation = 0x30;
        else
        {
            animation = (G3SdkRandom80027FA8() & 1) != 0 ? 0x10 : 0x2c;
            directed = false;
        }
        w_u16(player + 0x6e, animation);
        if (directed && apply_velocity != 0)
        {
            G3FillMemory80086094(frame + 0x10, 0, 8);
            sint32 x = (sint32)(delta_x << 16) >> 16;
            if (x < 0)
                x = -x;
            sint32 y = (sint32)(delta_y << 16) >> 16;
            if (y < 0)
                y = -y;
            sint32 z = r_s16(frame + 0x14);
            w_u16(frame + 0x10, delta_x);
            w_u16(frame + 0x12, delta_y);
            if (z < 0)
                z = -z;
            sint32 smallest = x;
            sint32 middle = y;
            sint32 largest = z;
            sint32 signed_x = (sint32)((uint32)x << 16) >> 16;
            sint32 signed_y = (sint32)((uint32)y << 16) >> 16;
            sint32 signed_z = (sint32)((uint32)z << 16) >> 16;
            if (signed_x < signed_y)
            {
                if (signed_z < signed_x)
                {
                    smallest = z;
                    middle = x;
                    largest = y;
                }
                else if (signed_z < signed_y)
                {
                    middle = z;
                    largest = y;
                }
            }
            else if (signed_z < signed_y)
            {
                smallest = z;
                largest = x;
            }
            else if (signed_z < signed_x)
            {
                smallest = y;
                middle = z;
                largest = x;
            }
            else
            {
                smallest = y;
                middle = x;
            }
            smallest = (sint32)((uint32)smallest << 16) >> 16;
            middle = (sint32)((uint32)middle << 16) >> 16;
            largest = (sint32)((uint32)largest << 16) >> 16;
            sint32 weighted = 30 * largest + 12 * middle + 9 * smallest;
            sint32 length = (weighted < 0 ? weighted + 7 : weighted) >> 3;
            if (length != 0)
            {
                sint32 numerator = (sint32)((uint32)(sint32)r_s16(frame + 0x10) << 14);
                sint32 normalized_x = numerator / length;
                numerator = (sint32)((uint32)(sint32)r_s16(frame + 0x12) << 14);
                sint32 normalized_y = numerator / length;
                numerator = (sint32)((uint32)(sint32)r_s16(frame + 0x14) << 14);
                sint32 normalized_z = numerator / length;
                w_u16(frame + 0x10, normalized_x);
                w_u16(frame + 0x12, normalized_y);
                w_u16(frame + 0x14, normalized_z);
            }
            uint32 component = r_u16(frame + 0x10);
            uint32 velocity = 0 - (uint32)((sint32)(component << 16) >> 22);
            w_u32(player + 0x104, velocity);
            component = r_u16(frame + 0x12);
            w_u32(player + 0x10c, 0);
            velocity = 0 - (uint32)((sint32)(component << 16) >> 22);
            w_u32(player + 0x108, velocity);
        }
    }
    w_u32(player + 0x154, 0x4000000);
    uint32 result = r_u32(player + 0x134);
    w_u16(player + 0x5e, 0);
    result &= 0xfffffff7;
    w_u32(player + 0x134, result);
    w_u16(state + 0x1d8, 0);
    w_u16(player + 0xa2, 0);
    return result;
}

uint32 G3Function800764F4(uint32 object)
{
    FUNCTION_MARKER(0x800764f4, "SLUS_008.06");
    uint32 owner = r_u32(object + 0x1c);
    uint32 index = (uint32)(sint32)r_s16(object + 0xa2);
    uint32 models = r_u32(owner + 0xc);
    uint32 model = r_u32(models + (index << 2));
    uint32 data = r_u32(model + 0x1c) + 4;
    uint32 selector = (uint32)(sint32)r_s16(object + 0x104);
    uint32 triple = 3 * selector;
    uint32 stride = r_u32(data + 8);
    uint32 first = (triple + 1) * stride;
    w_u16(object + 0x108, first);
    stride = r_u32(data + 8);
    uint32 last = (triple + 4) * stride - 1;
    w_u16(object + 0x10a, last);
    w_u16(object + 0x9e, first);
    w_u16(object + 0x10c, 0xffff);
    return 0xffffffff;
}

uint32 G3Function8007657C(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007657c, "SLUS_008.06");
    uint32 flags = r_u32(object + 0x10);
    uint32 source = r_u32(object + 0x28);
    w_u16(object + 0x6e, 0);
    w_u16(object + 0x5e, 0);
    w_u32(object + 0x10, flags | 0x80);
    if (source != 0)
    {
        uint32 value = r_u16(source);
        w_u16(object + 0x104, value);
        value = r_u16(source + 2);
        w_u16(object + 0x106, value);
    }
    else
        w_u16(object + 0x104, 0);
    w_u32(object + 0x114, 0);
    uint32 settings = r_u32(object + 0x20);
    if ((r_u32(settings + 0x18) & 0x800) == 0)
        return G3Function800764F4(object);
    w_u32(object + 0x114, 2);
    uint32 owner = r_u32(object + 0x1c);
    uint32 animation = r_u16(object + 0x6e);
    uint32 animations = r_u32(owner + 0x10);
    uint32 data = r_u32(animations + (animation << 2));
    uint32 count = r_u16(data + 2);
    w_u16(object + 0x9e, 0);
    uint32 result = count - 1;
    w_u16(object + 0x5e, result);
    return result;
}

uint32 G3Function8007662C(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007662c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    if (r_u32(object + 0x114) - 1 >= 2)
    {
        uint32 next = r_u16(object + 0x9e) + 1;
        w_u16(object + 0x9e, next);
        sint32 last = r_s16(object + 0x10a);
        if (last < ((sint32)(next << 16) >> 16))
        {
            uint32 first = r_u16(object + 0x108);
            w_u16(object + 0x9e, first);
        }
    }
    if (r_u32(object + 0x114) == 1)
    {
        sint32 delay = r_s16(object + 0x10e);
        uint32 next_delay = r_u16(object + 0x10e) - 1;
        bool advance = true;
        if (delay > 0)
        {
            w_u16(object + 0x10e, next_delay);
            if ((next_delay << 16) != 0)
                advance = false;
            else
            {
                uint32 first = r_u16(object + 0x108);
                w_u16(object + 0x9e, first);
            }
        }
        if (advance)
        {
            uint32 selector = (uint32)(sint32)r_s16(object + 0x10c);
            G3Function80029F0C(object, selector, frame);
            if (r_s16(object + 0x10c) == -1001 && r_s16(object + 0x5e) == 0x1a)
            {
                uint32 flags = r_u32(object + 0x14);
                w_u32(object + 0x14, flags | 0x10);
            }
            if ((r_u32(object + 0x14) & 0x10) != 0)
            {
                if ((r_u16(object + 0x106) & 1) != 0)
                {
                    if (r_s16(object + 0x10c) == -1001)
                    {
                        w_u32(object + 0x114, 0);
                        w_u16(object + 0x5e, 0);
                        w_u16(object + 0x10c, 0xffff);
                        return 0xffffffff;
                    }
                    w_u16(object + 0x10e, 0x1e);
                    w_u16(object + 0x10c, 0xfc17);
                    G3Function80029F0C(object, 0xfffffc17, frame);
                    uint32 flags = r_u32(object + 0x14);
                    w_u32(object + 0x14, flags & 0xffffffef);
                }
                else
                {
                    w_u32(object + 0x114, 2);
                    G3Function80029F0C(object, 0xfffffc17, frame);
                }
                uint32 random = G3SdkRandom80027FA8();
                uint32 pitch = (uint32)((sint32)(((random & 0x1f) - 0xf) << 16) >> 16);
                w_u32(frame + 0x10, 2500);
                G3Function8003E518(object, 0x80c, pitch, 0x6e, frame);
                uint32 selection = (uint32)(sint32)r_s16(object + 0x104);
                if (selection != 5)
                {
                    uint32 x = r_u16(object + 0x58);
                    uint32 z = r_u16(object + 0x5c);
                    w_u16(object + 0x58, x - 0x8c);
                    w_u16(object + 0x5c, z + 0x8c);
                    uint32 linked = G3Function8002A6F4(object, 0xe, frame);
                    x = r_u16(object + 0x58);
                    z = r_u16(object + 0x5c);
                    w_u16(object + 0x58, x + 0x8c);
                    w_u16(object + 0x5c, z - 0x8c);
                    if (linked != 0)
                    {
                        uint32 target = r_u32(linked + 0xec);
                        if (target != 0)
                            G3DispatchStateEvent(target, linked, game, 0x7d3, selection, frame);
                        random = G3SdkRandom80027FA8();
                        w_u32(frame + 0x10, 2500);
                        pitch = (uint32)((sint32)(((random & 0x1f) - 0xf) << 16) >> 16);
                        G3Function8003E518(object, 0x81c, pitch, 0x50, frame);
                    }
                }
            }
        }
    }
    uint32 phase = r_u32(object + 0x118);
    if (phase == 1 || phase == 2)
    {
        uint32 strength = (uint32)(sint32)r_s16(object + 0x110);
        w_u32(frame + 0x10, 0);
        w_u32(frame + 0x14, 0xf0);
        w_u32(frame + 0x18, 0x200);
        w_u32(frame + 0x1c, 0xf0);
        w_u32(frame + 0x2c, 1);
        strength *= phase == 1 ? 85 : 5;
        strength = (uint32)((sint32)(strength << 16) >> 16);
        w_u32(frame + 0x20, strength);
        w_u32(frame + 0x24, strength);
        w_u32(frame + 0x28, strength);
        uint32 pool = r_u32(game + 0x68);
        w_u32(frame + 0x30, pool);
        uint32 order = r_u32(game + 0x38);
        w_u32(frame + 0x34, order);
        G3Function8001E25C(0, 0, 0x200, 0, 0, 0xf0, 0x200, 0xf0, strength, strength, strength, 1, pool, order);
        if (phase == 1)
        {
            sint32 count = r_s16(object + 0x110);
            uint32 next = r_u16(object + 0x110) + 1;
            if (count < 3)
            {
                w_u16(object + 0x110, next);
                return next;
            }
            w_u32(object + 0x118, 2);
            w_u16(object + 0x110, 0x33);
            return 0x33;
        }
        if (r_s16(object + 0x110) >= 11)
        {
            uint32 text = G3Function80065D38(0x48);
            G3Function80023448(text, 0x78, 0x200, 0, frame, 0);
        }
        sint32 count = r_s16(object + 0x110);
        uint32 next = r_u16(object + 0x110) - 1;
        if (count > 0)
            w_u16(object + 0x110, next);
        else
            w_u32(object + 0x118, 0);
        return next;
    }
    return 2;
}
