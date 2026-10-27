#include "xport.h"
#include "psx.h"
#include "game_functions.h"

void G3Function80065D14(uint32 object, uint32 game, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80065d14, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (payload == 0)
        G3Function8002A9AC(object, frame);
}

uint32 G3Function800689AC(uint32 state)
{
    FUNCTION_MARKER(0x800689ac, "SLUS_008.06");
    return r_u32(state + 0x30);
}

uint32 G3Function80068C54(uint32 context, uint32 selected, uint32 incoming_third_glyph, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80068c54, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 text = G3Function80065D38(6);
    G3Function8006AAD4(context, text, incoming_third_glyph, frame);
    text = G3Function80065D38(7);
    G3Function80069A4C(context, 0, text, 0x80068c1c, frame);
    text = G3Function80065D38(8);
    uint32 result = G3Function80069A4C(context, 1, text, 0x800699ac, frame);
    if ((sint32)selected < 0)
        selected = 1;
    return G3Function80069BC8(context, selected, 0, 2, result, frame);
}

uint32 G3Function80068CF0(uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80068cf0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (event != 7)
        return 7;
    uint32 result = G3Function8006A654(3, 0);
    return G3Function800698C4(context, 0x80068c54, 0xffffffff, 0, result, frame);
}









uint32 G3Function80068FD8(uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80068fd8, "SLUS_008.06");
    if (event != 7)
        return 7;
    return G3Function8006A654(6, 0);
}

uint32 G3Function8004806C(uint32 forward)
{
    FUNCTION_MARKER(0x8004806c, "SLUS_008.06");
    uint32 step = forward != 0 ? 1 : 0xffffffff;
    uint32 previous = (uint32)r_s16(0x800a223e);
    uint32 game = r_u32(0x80097048);
    uint32 selected = previous + step;
    uint32 requirement = 0x8009e864 + selected * 4;
    for (;;)
    {
        if ((sint32)selected < 11 || selected - 23 < 4)
        {
            sint32 available = r_s32(game + 0x682c);
            sint32 required = r_s32(requirement);
            if (available >= required)
                break;
        }
        else if (selected >= 28)
            break;
        requirement += step * 4;
        selected += step;
    }
    if (selected >= 28)
        return 0;
    w_u16(0x800a223e, selected);
    G3Function80047FC8(selected);
    return selected != previous ? 1 : 0;
}

uint32 G3Function8006B258(uint32 context, uint32 restore)
{
    FUNCTION_MARKER(0x8006b258, "SLUS_008.06");
    uint32 game = G3Function8006A5C0(context);
    uint32 level = 0xffffffff;
    if (restore == 0)
        level = r_u16(game + 0x68b4);
    return G3Function80047FC8(level);
}

uint32 G3Function8006B290(uint32 context, uint32 event)
{
    FUNCTION_MARKER(0x8006b290, "SLUS_008.06");
    return G3Function8004806C(event == 5 ? 1 : 0);
}

uint32 G3Function80069764(uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    if (event != 7)
        return 7;
    uint32 frame = guest_stack - 0x20;
    uint32 state = G3Function8006A600(context);
    G3Function8006A654(3, 0);
    w_u32(state + 0xc, 1);
    w_u32(state + 0x38, 0);
    sint32 result = G3NativeSaveGame(state, frame);
    w_u32(state + 0x40, (uint32)result);
    w_u32(state + 0x30, 1);
    return G3Function800698C4(context, 0x8006954c, 0, 0x80068e98, (uint32)result, frame);
}

uint32 G3Function800699AC(uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800699ac, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (event != 7)
        return 7;
    uint32 result = G3Function8006A654(2, 0);
    return G3Function80069918(context, result, frame);
}

uint32 G3Function800699E4(uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800699e4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (event != 7)
        return 7;
    uint32 result = G3Function8006A654(3, 0);
    return G3Function80069918(context, result, frame);
}

uint32 G3Function80069D40(uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80069d40, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    if (event == 7)
    {
        G3Function8006A654(2, 0);
        uint32 result = G3Function8006B258(context, 1);
        G3Function80069918(context, result, frame);
    }
    if (event == 6)
        G3Function8006B258(context, 1);
    if (event - 4 >= 2)
        return 0;
    uint32 result = G3Function8006B290(context, event);
    if (result != 0)
        return G3Function8006A654(4, 0);
    return result;
}

uint32 G3Function80069DD4(uint32 context, uint32 selected, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80069dd4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    G3Function80069A4C(context, 0, 0, 0x80069d40, frame);
    uint32 text = G3Function80065D38(31);
    uint32 result = G3Function8006AAD4(context, text, 0x80069d40, frame);
    return G3Function80069BC8(context, selected, 0, 1, result, frame);
}

uint32 G3Function80069E3C(uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80069e3c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (event != 7)
        return 7;
    G3Function8006B258(context, 0);
    uint32 result = G3Function8006A654(3, 0);
    return G3Function800698C4(context, 0x80069dd4, 0, 0, result, frame);
}

uint32 G3Function80069E8C(uint32 context, uint32 selected, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80069e8c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 text = G3Function80065D38(16);
    G3Function80069A4C(context, 0, text, 0x8006b0cc, frame);
    text = G3Function80065D38(33);
    uint32 result = G3Function80069A4C(context, 1, text, 0x800699ac, frame);
    return G3Function80069BC8(context, selected, 0, 2, result, frame);
}

uint32 G3Function80069F0C(uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80069f0c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (event != 7)
        return 7;
    uint32 result = G3Function8006A654(3, 0);
    return G3Function800698C4(context, 0x80069e8c, 1, 0, result, frame);
}

uint32 G3Function80069F54(uint32 context, uint32 selected, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80069f54, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 text = G3Function80065D38(28);
    G3Function80069A4C(context, 0, text, 0x8006b0cc, frame);
    text = G3Function80065D38(33);
    uint32 result = G3Function80069A4C(context, 1, text, 0x800699ac, frame);
    return G3Function80069BC8(context, selected, 0, 2, result, frame);
}

uint32 G3Function80069FD4(uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80069fd4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (event != 7)
        return 7;
    uint32 result = G3Function8006A654(3, 0);
    return G3Function800698C4(context, 0x80069f54, 1, 0, result, frame);
}

uint32 G3Function8006ADC8(uint32 context)
{
    FUNCTION_MARKER(0x8006adc8, "SLUS_008.06");
    return G3Function8004DF3C();
}

uint32 G3Function8006AE20(uint32 context)
{
    FUNCTION_MARKER(0x8006ae20, "SLUS_008.06");
    return G3Function80080FD4(0);
}

uint32 G3Function8006AE40(uint32 context)
{
    FUNCTION_MARKER(0x8006ae40, "SLUS_008.06");
    return G3Function8008102C();
}

uint32 G3Function8006AED8(uint32 context, uint32 index)
{
    FUNCTION_MARKER(0x8006aed8, "SLUS_008.06");
    uint32 base = r_u32(0x800a0084);
    uint32 selected = index - base;
    uint32 game = G3Function8006A5C0(context);
    uint32 volume = 0;
    if (selected == 0)
        volume = r_u32(game + 0x67d4);
    else if (selected == 1)
        volume = r_u32(game + 0x67d0);
    else if (selected == 2)
        volume = r_u32(game + 0x67d8);
    uint32 scale = r_u32(0x800a0080);
    uint32 product = scale * volume;
    sint64 wide = (sint64)(sint32)product * (sint64)(sint32)0x81020409;
    uint32 high = (uint32)((uint64)wide >> 32);
    uint32 shifted = (uint32)((sint32)(high + product) >> 6);
    uint32 sign = (uint32)((sint32)product >> 31);
    return shifted - sign;
}

uint32 G3Function8006A01C(uint32 context, uint32 index, uint32 title, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006a01c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 volume = G3Function8006AED8(context, index);
    G3SdkSprintfStringInteger80088A1C(frame + 0x10, 0x800a0058, title, volume);
    return G3Function80069A4C(context, index, frame + 0x10, 0x8006af68, frame);
}

uint32 G3Function8006A088(uint32 context, uint32 selected, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006a088, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 previous = G3Function80069984(context, 0x800);
    uint32 text = G3Function80065D38(46);
    G3Function80069A4C(context, 0, text, 0, frame);
    text = G3Function80065D38(55);
    G3Function8006A01C(context, 1, text, frame);
    text = G3Function80065D38(39);
    G3Function8006A01C(context, 2, text, frame);
    text = G3Function80065D38(57);
    G3Function8006A01C(context, 3, text, frame);
    uint32 enabled = G3Function8006ADC8(context);
    text = G3Function80065D38(enabled != 0 ? 17 : 18);
    G3Function80069A4C(context, 4, text, 0x8006ade8, frame);
    uint32 next = 5;
    if (G3Function8006AE20(context) != 0)
    {
        enabled = G3Function8006AE40(context);
        text = G3Function80065D38(enabled != 0 ? 19 : 20);
        G3Function80069A4C(context, next, text, 0x8006ae60, frame);
        ++next;
    }
    text = G3Function80065D38(21);
    uint32 result = G3Function80069A4C(context, next, text, 0x800699e4, frame);
    ++next;
    G3Function80069BC8(context, selected, 1, next, result, frame);
    return G3Function80069984(context, previous);
}

uint32 G3Function8006A1FC(uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006a1fc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (event != 7)
        return 7;
    uint32 result = G3Function8006A654(3, 0);
    return G3Function800698C4(context, 0x8006a088, 1, 0, result, frame);
}

uint32 G3Function8006A244(uint32 context, uint32 selected, uint32 guest_stack)
{
    uint32 frame = guest_stack - 0x20;
    uint32 text = G3Function80065D38(62);
    G3Function80069A4C(context, 0, text, 0x8006b170, frame);
    text = G3Function80065D38(8);
    uint32 result = G3Function80069A4C(context, 1, text, 0x800699ac, frame);
    return G3Function80069BC8(context, selected, 0, 2, result, frame);
}

void G3Function8006A580(uint32 resource, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006a580, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 argument = r_u32(0x800a0064);
    G3Function80069894(resource, 0x8006a3f8, 0, argument, frame);
    G3Function80069984(resource, 0xaf0);
}

uint32 G3Function8006AD74(uint32 context)
{
    FUNCTION_MARKER(0x8006ad74, "SLUS_008.06");
    uint32 game = G3Function8006A5C0(context);
    uint32 level = r_u16(game + 0x68b4);
    return level - 23 < 4 ? 1 : 0;
}

uint32 G3Function8006AD9C(uint32 context)
{
    FUNCTION_MARKER(0x8006ad9c, "SLUS_008.06");
    uint32 state = G3Function8006A600(context);
    uint32 result = G3Function80068BCC(state);
    return (~result) >> 31;
}

uint32 G3Function8006A3F8(uint32 context, uint32 selected, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006a3f8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 text = G3Function80065D38(51);
    G3Function80069A4C(context, 0, text, 0x8006b08c, frame);
    text = G3Function80065D38(45);
    G3Function80069A4C(context, 1, text, 0x8006a1fc, frame);
    uint32 next = 2;
    if (G3Function8006AD9C(context) != 0)
    {
        text = G3Function80065D38(49);
        G3Function80069A4C(context, 2, text, 0x80069764, frame);
        next = 3;
    }
    text = G3Function80065D38(30);
    G3Function80069A4C(context, next, text, 0x80069e3c, frame);
    ++next;
    uint32 target;
    if (G3Function8006AD74(context) != 0)
    {
        text = G3Function80065D38(16);
        target = 0x80069f0c;
    }
    else
    {
        text = G3Function80065D38(28);
        target = 0x80069fd4;
    }
    uint32 result = G3Function80069A4C(context, next, text, target, frame);
    ++next;
    return G3Function80069BC8(context, selected, 0, next, result, frame);
}

uint32 G3Function8006ADE8(uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006ade8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (event != 7)
        return 7;
    uint32 enabled = G3Function8006ADC8(context);
    G3Function8004DF00(enabled == 0 ? 1 : 0, frame);
    return G3Function8006A654(4, 0);
}

uint32 G3Function8006AE60(uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006ae60, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    if (event != 7)
        return 7;
    if (G3Function8006AE40(context) != 0)
    {
        uint32 result = G3Function8006A654(4, 0);
        G3Function80081010();
        return result;
    }
    G3Function8006A654(5, 0);
    G3Function8008101C();
    w_u32(frame + 0x10, 20);
    w_u32(frame + 0x14, 0);
    return G3Function8008194C(0, 2, 0xffffffff, 0x1000, frame);
}

uint32 G3Function8006AF68(uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006af68, "SLUS_008.06");
    uint32 base = r_u32(0x800a0084);
    uint32 channel = selected - base;
    G3Function8006A5C0(context);
    uint32 volume = G3Function8006AED8(context, selected);
    if (event - 4 >= 2)
        return 4;
    if (event == 4)
    {
        if ((sint32)volume <= 0)
            return 5;
        --volume;
    }
    if (event == 5)
    {
        sint32 maximum = r_s32(0x800a0080);
        if ((sint32)volume >= maximum)
            return 0;
        ++volume;
    }
    uint32 scale = r_u32(0x800a0080);
    uint32 numerator = (volume << 7) - volume + scale - 1;
    uint32 quotient;
    if (scale == 0)
        quotient = (sint32)numerator < 0 ? 1 : 0xffffffff;
    else if (numerator == 0x80000000 && scale == 0xffffffff)
        quotient = 0x80000000;
    else
        quotient = (uint32)((sint32)numerator / (sint32)scale);
    if (channel == 1)
        G3Function8003EDE8(quotient);
    else if ((sint32)channel < 2)
    {
        if (channel != 0)
            return 1;
        G3Function8003EE30(quotient);
    }
    else
    {
        if (channel != 2)
            return 2;
        G3Function8003EE78(quotient);
    }
    return G3Function8006A654(4, 0);
}

uint32 G3Function8006B08C(uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006b08c, "SLUS_008.06");
    if (event != 7)
        return 7;
    uint32 game = G3Function8006A5C0(context);
    uint32 flags = r_u32(game + 0x6834);
    w_u32(game + 0x6834, flags | 0x200000);
    return G3Function8006A654(3, 0);
}

uint32 G3Function8006B0CC(uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006b0cc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    if (event != 7)
        return 7;
    uint32 game = G3Function8006A5C0(context);
    uint32 level = r_u16(game + 0x68b4);
    uint32 root;
    uint32 name;
    if (level - 23 < 4)
    {
        root = r_u32(0x8009d684);
        name = r_u32(0x8009d688);
    }
    else
    {
        root = r_u32(0x8009d668);
        name = r_u32(0x8009d66c);
    }
    G3Function80027644(root, name, game, frame);
    return G3Function8006B08C(context, selected, event, frame);
}

uint32 G3Function8006B170(uint32 context, uint32 selected, uint32 event, uint32 guest_stack)
{
    uint32 frame = guest_stack - 0x20;
    if (event != 7 || selected != 0)
        return 7;
    uint32 game = G3Function8006A5C0(context);
    G3Function8006A654(9, 0);
    w_u32(0x800aa7bc, 0xffffffff);
    uint32 level = r_u16(game + 0x68b4);
    w_u16(game + 0x69dc, level);
    uint32 root = r_u32(0x8009d684);
    uint32 name = r_u32(0x8009d688);
    return G3Function80027644(root, name, game, frame);
}

uint32 G3Function8006954C(uint32 context, uint32 incoming_third_glyph, uint32 guest_stack)
{
    return G3NativeSaveResultPage(context, 0x800699e4, incoming_third_glyph, guest_stack);
}

uint32 G3Function8006F2BC(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006f2bc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    uint32 packet = r_u32(object + 0xb0);
    sint32 type = r_s16(packet + 6);
    uint32 linked = r_u32(packet + 0x14);
    if (type != 1)
        return 1;
    uint32 owner = r_u32(linked + 0x1c);
    uint32 name = r_u32(owner + 0x20);
    uint32 expected = r_u32(0x800a0210);
    uint32 actual = r_u32(name);
    if (actual != expected)
        return 1;
    actual = r_u32(name + 4);
    expected = r_u32(0x800a0214);
    if (actual != expected)
        return 1;
    uint32 player = r_u32(0x8009efc4);
    w_u32(linked + 0xb0, frame + 0x10);
    w_u32(frame + 0x24, player);
    uint32 target = r_u32(linked + 0xec);
    G3DispatchStateEvent(target, linked, game, 3, 0, frame);
    return 1;
}

uint32 G3Function8006F50C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006f50c, "SLUS_008.06");
    uint32 player = r_u32(0x8009efc4);
    uint32 result = r_u32(player + 0x150);
    uint32 state = r_u32(player + 0x24);
    if (result != 0)
        return result;
    uint32 flags = r_u32(player + 0x154);
    if ((flags & 0x76400000) != 0)
        return 0x80000000;
    w_u32(player + 0x154, 0x80000000);
    w_u16(player + 0x6e, 49);
    w_u16(player + 0x5e, 0);
    w_u16(state + 0xc4, 0);
    w_u32(state + 0x1fc, a0);
    return 49;
}

uint32 G3Function8006F55C(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006f55c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 game = r_u32(0x80097048);
    uint32 player = r_u32(game + 0x50);
    uint32 state = r_u32(player + 0x24);
    uint32 owner = r_u32(state + 0x1fc);
    if (owner == 0)
    {
        G3Function8006F468(game, frame);
        game = r_u32(0x80097048);
        w_u16(state + 0xd0, 0);
        w_u16(state + 0xd2, 0);
        w_u16(state + 0xd4, 0);
        w_u16(state + 0xd8, 0);
        w_u16(state + 0xda, 0);
        w_u16(state + 0xdc, 0);
        uint32 value = (uint32)r_s16(command + 4);
        player = r_u32(game + 0x50);
        w_u32(player + 0x104, value);
        player = r_u32(game + 0x50);
        value = (uint32)r_s16(command + 6);
        w_u32(player + 0x108, value);
        player = r_u32(game + 0x50);
        value = (uint32)r_s16(command + 8);
        w_u32(player + 0x10c, value);
    }
    else
    {
        uint32 target = r_u32(player + 0xec);
        if (target != 0)
            G3DispatchStateEvent(target, player, game, 0x7e2, command + 4, frame);
    }
    return 1;
}

uint32 G3Function8006BE58(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006be58, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    uint32 flags = r_u32(object + 0x10);
    uint32 second = r_u32(state + 4);
    w_u32(object + 0x10, flags & 0xfffffbff);
    uint32 saved = r_u16(state + 0x14);
    uint32 first = r_u32(object + 0x104);
    if ((saved & 2) == 0)
    {
        flags = r_u32(first + 0x10);
        w_u32(first + 0x10, flags & 0xfffffbff);
    }
    if ((r_u16(state + 0x16) & 2) == 0)
    {
        flags = r_u32(second + 0x10);
        w_u32(second + 0x10, flags & 0xfffffbff);
    }
    if ((r_u16(state + 0x14) & 1) == 0)
    {
        flags = r_u32(first + 0x10);
        w_u32(first + 0x10, flags & 0xfffff7ff);
    }
    if ((r_u16(state + 0x16) & 1) == 0)
    {
        flags = r_u32(second + 0x10);
        w_u32(second + 0x10, flags & 0xfffff7ff);
    }
    uint32 mask = r_u32(state + 0x18);
    G3Function80037150(first, mask, frame);
    mask = r_u32(state + 0x1c);
    G3Function80037150(second, mask, frame);
    if ((r_u16(state + 0x14) & 4) != 0 && (r_u32(first + 0x10) & 0x400) == 0)
        G3Function8002A9AC(first, frame);
    if ((r_u16(state + 0x16) & 4) != 0 && (r_u32(second + 0x10) & 0x400) == 0)
        G3Function8002A9AC(second, frame);
    return G3Function8002A9AC(object, frame);
}

uint32 G3Function8006BBF4(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006bbf4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    G3Function8006BD2C(object);
    uint32 first = r_u32(object + 0x104);
    uint32 flags = r_u32(first + 0x10);
    w_u32(first + 0x10, flags | 0x800);
    uint32 second = r_u32(state + 4);
    flags = r_u32(second + 0x10);
    w_u32(second + 0x10, flags | 0x800);
    if ((r_u16(state + 0x16) & 0x10) != 0)
    {
        second = r_u32(state + 4);
        sint32 angle = r_s16(second + 0x64);
        sint32 shifted = angle + 0x400;
        sint32 adjusted = shifted >= 0 ? shifted : angle + 0x13ff;
        uint32 multiple = (uint32)(adjusted >> 12) << 12;
        w_u16(second + 0x64, (uint32)shifted - multiple);
    }
    sint32 timer = r_s16(state + 0xc);
    uint32 unsigned_timer = r_u16(state + 0xc);
    if (timer > 0)
    {
        w_u16(state + 0xc, unsigned_timer - 1);
        w_u16(state + 0xa, 0x1000);
        return 0x1000;
    }
    timer = r_s16(state + 0xe);
    unsigned_timer = r_u16(state + 0xe);
    uint32 result = unsigned_timer - 1;
    if (timer > 0)
    {
        uint32 scale = r_u16(state + 0x12);
        uint32 product = scale * result;
        w_u16(state + 0xe, result);
        w_u16(state + 0xa, product);
        return result;
    }
    timer = r_s16(state + 0x10);
    unsigned_timer = r_u16(state + 0x10);
    result = unsigned_timer - 1;
    if (timer > 0)
    {
        w_u16(state + 0x10, result);
        w_u16(state + 0xa, 0);
        return result;
    }
    if ((r_u16(state + 0x16) & 0x10) != 0)
    {
        second = r_u32(state + 4);
        w_u16(second + 0x6e, 1);
        w_u16(second + 0x5e, 0);
    }
    return G3Function8006BE58(object, frame);
}

uint32 G3Function800715BC(uint32 output)
{
    FUNCTION_MARKER(0x800715bc, "SLUS_008.06");
uint32 state = r_u32(0x800a0284);
    if (r_u32(state + 0x10) == 0)
    {
        w_u16(output, 0);
        w_u16(output + 2, 0);
        w_u16(output + 4, 0x1000);
        return 1;
    }
    uint32 previous = r_u32(state + 0x14);
    uint32 current = r_u32(state + 0x1c);
    if (previous == current)
    {
        sint32 previous_z = r_s16(state + 0x18);
        sint32 current_z = r_s16(state + 0x20);
        if (previous_z == current_z)
            return 0;
    }
    uint32 value = r_u16(state + 0x1c);
    w_u16(output, value);
    value = r_u16(state + 0x1e);
    w_u16(output + 2, value);
    value = r_u16(state + 0x20);
    w_u16(output + 4, value);
    return 1;
}

uint32 G3Function80071640(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80071640, "SLUS_008.06");
uint32 frame = guest_stack - 0x48;
    uint32 vector = r_u32(object + 0x30);
    uint32 state = r_u32(object + 0x24);
    uint32 first = r_u16(vector);
    w_u16(frame + 0x10, first);
    sint32 x = (sint16)first;
    vector = r_u32(object + 0x30);
    sint32 absolute_x = x < 0 ? -x : x;
    uint32 second = r_u16(vector + 6);
    sint32 smallest = absolute_x;
    w_u16(frame + 0x14, 0);
    sint32 y = (sint16)second;
    sint32 absolute_y = y < 0 ? -y : y;
    sint32 narrowed_x = (sint16)absolute_x;
    w_u16(frame + 0x12, second);
    sint32 narrowed_y = (sint16)absolute_y;
    sint32 middle = absolute_y;
    sint32 largest = 0;
    if (narrowed_x < narrowed_y)
    {
        if (narrowed_x > 0)
        {
            smallest = 0;
            largest = absolute_y;
            middle = absolute_x;
        }
        else if (narrowed_y > 0)
        {
            largest = absolute_y;
            middle = 0;
        }
    }
    else
    {
        smallest = 0;
        if (narrowed_y > 0)
            largest = absolute_x;
        else
        {
            smallest = absolute_y;
            if (narrowed_x > 0)
            {
                largest = absolute_x;
                middle = 0;
            }
            else
                middle = absolute_x;
        }
    }
    sint32 weighted = (sint16)largest * 30 + (sint16)middle * 12 + (sint16)smallest * 9;
    sint32 divisor = (weighted >= 0 ? weighted : weighted + 7) >> 3;
    if (divisor != 0)
    {
        sint32 normalized_x = r_s16(frame + 0x10) * 16384 / divisor;
        sint32 normalized_y = r_s16(frame + 0x12) * 16384 / divisor;
        sint32 normalized_z = r_s16(frame + 0x14) * 16384 / divisor;
        w_u16(frame + 0x10, (uint32)normalized_x);
        w_u16(frame + 0x12, (uint32)normalized_y);
        w_u16(frame + 0x14, (uint32)normalized_z);
    }
    x = r_s16(frame + 0x10);
    y = r_s16(frame + 0x12);
    w_u16(frame + 0x24, 0);
    w_u16(frame + 0x2a, 0);
    w_u16(frame + 0x2c, 0);
    w_u16(frame + 0x2e, 0);
    w_u16(frame + 0x30, 0x1000);
    w_u16(frame + 0x20, (uint32)x);
    w_u16(frame + 0x28, (uint32)x);
    w_u16(frame + 0x22, 0 - (uint32)y);
    w_u16(frame + 0x26, (uint32)y);
    G3Function800202A4(frame + 0x20, frame + 0x18, 0x15, frame);
    uint32 result = (uint32)r_s16(frame + 0x1c);
    w_u16(state + 0xc6, result);
    w_u16(state + 0xc8, result);
    return result;
}

uint32 G3Function80074044(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80074044, "SLUS_008.06");
return G3Function80073E44(object, game, 0, 0, guest_stack - 0x18);
}

uint32 G3Function80074068(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80074068, "SLUS_008.06");
return G3Function80073E44(object, game, 1, 1, guest_stack - 0x18);
}

uint32 G3Function800740BC(uint32 object, uint32 game, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800740bc, "SLUS_008.06");
uint32 frame = guest_stack - 0x20;
    uint32 player = r_u32(0x8009efc4);
    uint32 result = (uint32)r_s16(game + 0x681c);
    uint32 flags = r_u32(player + 0x154);
    uint32 state = r_u32(player + 0x24);
    if ((flags & 0xc6400000) != 0)
        return result;
    if (r_s16(state + 0x120) != 0)
        return result;
    if (r_u32(state + 0x1b8) != 0)
        return result;
    sint32 height = r_s16(player + 0x5c);
    sint32 target_height = r_s16(player + 0xe0);
    if (height - 0x500 >= target_height)
        return result;
    result = G3Function8007408C(player, game, frame);
    if (result == 0)
        return result;
    w_u16(player + 0x6e, 0);
    w_u32(player + 0x154, 0x40000000);
    w_u16(player + 0x6c, 0);
    w_u16(state + 0x1d4, payload);
    w_u16(state + 0xc4, 0);
    uint32 value = r_u16(player + 0xe0);
    w_u16(player + 0x5c, value);
    uint32 source = player + 0x58;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 low_lane = source & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
    uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
    uint32 packed = high << ((3 - high_lane) * 8);
    uint32 keep = low_lane == 0 ? 0 : 0xffffffff << ((4 - low_lane) * 8);
    packed = (packed & keep) | (low >> (low_lane * 8));
    value = (uint32)r_s16(player + 0x5c);
    uint32 destination = state + 0x1f4;
    high_address = destination + 3;
    high_lane = high_address & 3;
    low_lane = destination & 3;
    w_masked_u32(high_address & 0xfffffffc, packed >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    w_masked_u32(destination & 0xfffffffc, packed << (low_lane * 8), (15 << low_lane) & 15);
    w_u16(state + 0x1f8, value);
    return result;
}

uint32 G3Function8007429C(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007429c, "SLUS_008.06");
uint32 frame = guest_stack - 0x20;
    uint32 player = r_u32(0x8009efc4);
    uint32 result = (uint32)r_s16(game + 0x681c);
    uint32 flags = r_u32(player + 0x154);
    uint32 state = r_u32(player + 0x24);
    if (flags == 0x02000000)
        return result;
    result = G3Function8007408C(player, game, frame);
    if (result == 0)
        return result;
    G3Function8006F468(game, frame);
    player = r_u32(0x8009efc4);
    w_u16(state + 0xd0, 0);
    w_u16(state + 0xd2, 0);
    w_u16(state + 0xd4, 0);
    w_u16(state + 0xd8, 0);
    w_u16(state + 0xda, 0);
    w_u16(state + 0xdc, 0);
    uint32 height = r_u16(player + 0x5c);
    w_u32(player + 0x10c, 0x3c);
    uint32 delta = r_u16(player + 0x10c);
    w_u32(player + 0x154, 0x200000);
    w_u16(player + 0x6e, 0x2e);
    w_u16(player + 0x5c, height + delta);
    uint32 index = r_u16(state + 0x20);
    uint32 table = r_u32(state + 0x34);
    index -= 1;
    w_u16(state + 0xe0, index);
    uint32 offset = (uint32)((sint32)(index << 16) >> 15);
    uint32 value = r_u16(table + offset);
    w_u16(state + 0xe2, value - 1);
    return result;
}

uint32 G3Function80074380(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80074380, "SLUS_008.06");
uint32 frame = guest_stack - 0x18;
    uint32 linked = r_u32(a2 + 0x14);
    if (linked != 0 && r_s16(linked + 6) == 1)
    {
        uint32 descriptor = r_u32(linked + 0xc);
        if (r_u8(descriptor + 4) == 9)
            return 9;
    }
    uint32 result = r_u32(a0 + 0x134) & 0x20;
    if (result != 0)
        return result;
    uint32 command = (uint32)r_s16(a2 + 0x10);
    if (command >= 10)
        return 0x80010000;
    switch (command)
    {
    case 0:
        return G3Function80074044(a0, a1, frame);
    case 3:
        return G3Function800740BC(a0, a1, 0x14, frame);
    case 4:
        return G3Function80074194(a0, a1, 0x14, frame);
    case 7:
        return G3Function8007429C(a0, a1, frame);
    case 8:
        return G3Function8007408C(a0, a1, frame);
    case 9:
        return G3Function80074068(a0, a1, frame);
    default:
        return 0x8007445c;
    }
}

uint32 G3Function80074598(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80074598, "SLUS_008.06");
uint32 frame = guest_stack - 0x18;
    w_u32(object + 0x150, 5);
    uint32 mode = r_u32(game + 0x6818);
    w_u32(0x800a02b8, mode == 1 ? 0xd4 : 0x5a);
    if ((r_u32(game + 0x6834) & 0x20000) != 0)
        w_u32(0x800a02b8, 0x1e);
    uint32 count = r_u32(game + 0x69f0);
    w_u32(game + 0x69f0, count + 1);
    return G3Function800359EC(0x121, 3, frame);
}

uint32 G3Function80074490(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80074490, "SLUS_008.06");
uint32 frame = guest_stack - 0x28;
    uint32 copied[2];
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 source = 0x800a02bc + word * 4;
        r_masked_u32(source, 15);
        uint32 low = r_masked_u32(source, 15);
        copied[word] = low;
    }
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 destination = frame + 0x10 + word * 4;
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        uint32 low_lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, copied[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, copied[word] << (low_lane * 8), (15 << low_lane) & 15);
    }
    if ((r_u32(game + 0x6834) & 0x20000) == 0 && r_u8(game + 0x67df) != 0)
    {
        uint32 current_game = r_u32(0x80097048);
        if (r_u16(current_game + 0x67dc) != 0)
            G3Function8003EF1C();
    }
    uint32 camera = r_u32(game + 0x4c);
    G3Function8005177C(camera, camera + 0x18, 0x2bc, frame + 0x10, frame);
    uint32 flags = r_u32(game + 0x6834);
    w_u32(game + 0x6834, flags | 0x600);
    if (r_u16(object + 0x6e) != 0x13)
    {
        flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags | 0x02000000);
    }
    flags = r_u32(game + 0x6834);
    w_u32(game + 0x6790, 1);
    uint32 result = flags | 0x1000;
    w_u32(game + 0x6834, result);
    w_u8(0x800a3d98, 0);
    w_u8(0x800a3d99, 0);
    w_u8(0x800a3d9a, 0);
    return result;
}

uint32 G3Function80074994(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80074994, "SLUS_008.06");
uint32 frame = guest_stack - 0x20;
    uint32 flags = r_u32(object + 0x128);
    uint32 state = r_u32(object + 0x24);
    w_u32(object + 0x128, flags & 0xfffffffb);
    G3Function8006F620(object, game, frame);
    flags = r_u32(object + 0x128);
    if ((flags & 0x4000) == 0)
    {
        if ((flags & 0x80) == 0)
        {
            uint32 count = r_u32(game + 0x6818);
            w_u32(game + 0x6818, count - 1);
        }
        uint32 count = r_u32(game + 0x6818);
        G3Function80047EE4(0, count);
        uint32 value = r_u16(game + 0x681e);
        w_u16(game + 0x681c, value);
    }
    uint32 current_game = r_u32(0x80097048);
    w_u16(game + 0xf0, 0x1e);
    w_u16(game + 0xf2, 0x1e);
    w_u16(game + 0xf4, 0);
    w_u16(game + 0x69b0, 0xa);
    uint32 value = r_u32(state + 0x1ec);
    uint32 level = r_u32(current_game + 0x48);
    w_u32(level + 0x6c, value);
    if ((r_u32(object + 0x128) & 0x4000) != 0)
    {
        flags = r_u32(game + 0x6834);
        w_u32(game + 0x6834, flags | 1);
        return G3Function800245E8(1, frame);
    }
    if (r_s32(game + 0x6818) <= 0)
    {
        flags = r_u32(game + 0x6834);
        w_u32(game + 0x6834, flags | 1);
        return G3Function800245E8(0, frame);
    }
    flags = r_u32(game + 0x6834);
    uint32 result = flags | 0x40000;
    w_u32(game + 0x6834, result);
    return result;
}

uint32 G3Function80074888(uint32 object, uint32 game, uint32 data, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80074888, "SLUS_008.06");
uint32 frame = guest_stack - 0x20;
    uint32 flags = r_u32(game + 0xe8);
    w_u32(game + 0xe8, flags | 0x08000000);
    flags = r_u32(object + 0x14);
    uint32 owner = r_u32(object + 0x1c);
    w_u32(object + 0x14, flags | 0x00800000);
    uint32 animation = r_u16(object + 0x6e);
    uint32 table = r_u32(owner + 0x10);
    uint32 entry = r_u32(table + (animation << 2));
    uint32 count = r_u16(entry + 2);
    w_u16(object + 0x5e, count - 1);
    G3Function8006BFB8(object, data);
    uint32 result = r_u32(0x800a02b8) - 1;
    w_u32(0x800a02b8, result);
    if ((sint32)result >= 0)
        return result;
    uint32 current_game = r_u32(0x80097048);
    uint32 level = r_u32(current_game + 0x48);
    uint32 color = r_u8(level + 0x44);
    w_u8(0x800a3d98, color);
    level = r_u32(current_game + 0x48);
    color = r_u8(level + 0x45);
    w_u8(0x800a3d99, color);
    level = r_u32(current_game + 0x48);
    color = r_u8(level + 0x46);
    w_u8(0x800a3d9a, color);
    G3Function80074994(object, game, frame);
    flags = r_u32(object + 0x14);
    w_u32(object + 0x14, flags & 0xff7fffff);
    flags = r_u32(game + 0x6834);
    result = flags & 0xfffdffff;
    w_u32(game + 0x6834, result);
    return result;
}

uint32 G3Function80074600(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80074600, "SLUS_008.06");
uint32 frame = guest_stack - 0x20;
    if ((r_u16(state + 0xce) & 1) != 0)
        w_u32(object + 0x10c, 0);
    G3Function8008004C(object, game, state, controls);
    G3Function8007F214(object);
    bool bypass = false;
    bool transition = false;
    if (r_u16(object + 0x6e) == 0x13)
    {
        if (r_s16(game + 0xf0) < -1)
            bypass = true;
        else
            transition = true;
    }
    else if ((r_u32(object + 0x14) & 0x10) != 0 || (r_u32(object + 0x128) & 4) != 0)
        transition = true;
    else if (r_s16(object + 0x5e) >= 0x14)
    {
        if ((r_u32(game + 0x6834) & 0x200) == 0 && r_u32(object + 0xf8) == 0)
            G3Function80074490(object, game, frame);
        if (r_s16(object + 0x5e) >= 0x14)
        {
            w_u8(0x800a3d98, 0);
            w_u8(0x800a3d99, 0);
            w_u8(0x800a3d9a, 0);
        }
    }
    if (transition)
    {
        uint32 flags = r_u32(object + 0x128);
        w_u32(object + 0x128, flags | 0x04800000);
        G3Function80074490(object, game, frame);
        uint32 owner = r_u32(object + 0x1c);
        uint32 animation = r_u16(object + 0x6e);
        uint32 table = r_u32(owner + 0x10);
        uint32 entry = r_u32(table + (animation << 2));
        uint32 count = r_u16(entry + 2);
        w_u16(object + 0x5e, count - 1);
        G3Function80074598(object, game, frame);
    }
    if (!bypass && r_u16(object + 0x6e) != 0x13)
    {
        uint32 flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags | 0x02000000);
    }
    uint32 flags = r_u32(object + 0x10);
    uint32 animation = r_u16(object + 0x6e);
    w_u32(object + 0x10, flags & 0xfffff7ff);
    if (animation == 0x13)
    {
        uint32 camera = r_u32(game + 0x4c);
        if (G3Function8004DABC(camera, frame) != 2)
        {
            camera = r_u32(game + 0x4c);
            uint32 target = (uint32)r_s16(object + 0x58);
            G3Function800787E0(camera, target, 0x40);
            camera = r_u32(game + 0x4c);
            target = (uint32)r_s16(object + 0x5a);
            G3Function800787E0(camera + 2, target, 0x40);
            camera = r_u32(game + 0x4c);
            target = (uint32)r_s16(object + 0x5c);
            G3Function800787E0(camera + 4, target, 0x40);
            camera = r_u32(game + 0x4c);
            G3Function800144CC(camera + 0x10, object + 0x58, camera, frame);
            camera = r_u32(game + 0x4c);
            target = (uint32)r_s16(camera + 0x10);
            G3Function80038AA8(camera + 8, target, 0x40);
            camera = r_u32(game + 0x4c);
            target = (uint32)r_s16(camera + 0x14);
            G3Function80038AA8(camera + 0xc, target, 0x40);
        }
    }
    uint32 result = (uint32)r_s16(state + 0x252);
    if ((sint32)result >= 0)
        return result;
    G3Function80021A74(0x140);
    uint32 text = G3Function80065D38(0x49);
    G3TextMeasureOperands operands;
    G3Function800234A0(text, 0x60, 0, 0, frame, &operands);
    text = G3Function80065D38(0x4a);
    return G3Function800234A0(text, 0x90, operands.second_glyph, operands.third_glyph, frame, 0);
}

uint32 G3Function800762F4(uint32 object)
{
    FUNCTION_MARKER(0x800762f4, "SLUS_008.06");
uint32 state = r_u32(object + 0x24);
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x150, 0);
    uint32 value = r_u32(state);
    uint32 result = flags & 0xfffff7ff;
    w_u32(object + 0x10, result);
    w_u32(object + 0xa4, value);
    uint32 linked = r_u32(state + 0x1fc);
    if (linked != 0)
    {
        flags = r_u32(linked + 0x10);
        result = flags & 0xfffff7ff;
        w_u32(linked + 0x10, result);
    }
    return result;
}

uint32 G3Function80078790(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80078790, "SLUS_008.06");
uint32 frame = guest_stack - 0x18;
    G3Function80078740(object, game, state, frame);
    w_u32(object + 0x154, 0x40000);
    sint32 value = r_s32(object + 0x10c);
    w_u16(object + 0x6e, 0x20);
    w_u32(object + 0x10c, (uint32)(value >> 1));
    uint32 flags = r_u32(object + 0x128);
    uint32 result = flags & 0xfffffff7;
    w_u32(object + 0x128, result);
    return result;
}

uint32 G3Function80078DE8(uint32 object, uint32 game, uint32 state)
{
    FUNCTION_MARKER(0x80078de8, "SLUS_008.06");
if ((r_u32(object + 0x14) & 0x10) != 0)
    {
        w_u32(object + 0x154, 0x1000);
        w_u16(object + 0x6e, 0x1d);
        w_u16(object + 0x5e, 0);
        return 0x1d;
    }
    uint32 destination = r_u32(state + 0xfc);
    uint32 value = r_u32(state + 0xf0);
    w_u32(destination, value);
    destination = r_u32(state + 0x108);
    value = r_u32(state + 0xf0);
    w_u32(destination, value);
    destination = r_u32(state + 0x114);
    value = r_u32(state + 0xf0);
    w_u32(destination, value);
    return value;
}

uint32 G3Function8007A708(uint32 object, uint32 game, uint32 state)
{
    FUNCTION_MARKER(0x8007a708, "SLUS_008.06");
if ((r_u32(object + 0x14) & 0x10) != 0)
    {
        w_u32(object + 0x154, 1);
        w_u16(object + 0x6e, 0);
        w_u16(object + 0x5e, 0);
        w_u16(state + 0xe0, 0);
        w_u16(state + 0xe2, 0);
    }
    return 1;
}

uint32 G3Function80072A4C(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80072a4c, "SLUS_008.06");
uint32 frame = guest_stack - 0x28;
    uint32 angle = G3Function8007F2D0(game + 0x98, object, frame);
    uint32 combined = 0;
    if ((angle << 16) != 0)
        combined = angle + G3Function80014F00(game);
    uint32 result = combined << 16;
    if (result != 0)
    {
        r_u32(game + 0x4c);
        uint32 mode = G3Function8004D9C0();
        uint32 next = combined + 0x1000;
        if (mode == 1)
        {
            r_u32(game + 0x4c);
            if (G3Function8004D9E0() != 0)
                next = angle + 0x1000;
        }
        w_u16(state + 0xc8, next);
        result = r_u16(state + 0xc8);
        w_u16(state + 0xc6, result);
    }
    w_u16(state + 0xbc, angle);
    w_u16(state + 0xc2, combined);
    return result;
}

uint32 G3Function8007ACF0(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007acf0, "SLUS_008.06");
uint32 frame = guest_stack - 0x28;
    uint32 result = r_u32(object + 0x14) & 0x10;
    if (result == 0)
    {
        w_u16(state + 0xe2, 0);
        w_u16(state + 0xe0, 0);
        return result;
    }
    if (r_u32(object + 0x150) == 1)
    {
        w_u32(object + 0x154, 2);
        w_u16(object + 0x6e, 0xc);
        w_u16(object + 0x5e, 0);
        return 0xc;
    }
    w_u32(object + 0x154, 2);
    w_u32(object + 0x150, 0);
    w_u16(object + 0x6e, 8);
    w_u16(state + 0xe2, 0);
    w_u16(state + 0xe0, 0);
    w_u16(state + 0xbc, 0);
    G3Function8007CF04(object, game, state, controls, frame);
    G3Function8007CD24(object, game, state, controls, frame);
    G3Function8007D0D0(object, game, state, controls, frame);
    G3Function8007D244(object, game, state, controls, frame);
    result = G3Function8007DB40(object, game, state, controls, frame);
    return G3Function8007DB80(object, game, state, controls, frame, result);
}

uint32 G3Function80072B0C(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80072b0c, "SLUS_008.06");
uint32 frame = guest_stack - 0x28;
    G3Function80071834(object, game, frame);
    if (r_u32(object + 0x150) == 1)
    {
        if (r_u32(object + 0x154) == 0x01000000)
            G3Function8007ACF0(object, game, state, controls, frame);
        if ((r_u32(object + 0x128) & 0x20000) == 0 && r_u32(object + 0x154) != 0x01000000)
        {
            G3Function80072A4C(object, game, state, frame);
            G3Function8007FE80(object, game, state);
            G3Function8007FA24(object, game, state, frame);
            bool alternate = r_u32(object + 0x154) == 0x10;
            if (!alternate)
            {
                G3Function8007D0D0(object, game, state, controls, frame);
                alternate = r_u32(object + 0x154) == 0x10;
            }
            if (alternate)
                G3Function80078C04(object, game, state, controls, frame);
            else
            {
                uint32 animation = 0xe;
                if (r_s16(state + 0xbc) != 0 && (r_u16(state + 0xce) & 0x400) == 0)
                    animation = 0xc;
                w_u16(object + 0x6e, animation);
            }
        }
        uint32 flags = r_u16(state + 0xce);
        w_u16(state + 0xce, flags & 0xfbff);
        uint32 steering = r_u32(0x800a0284);
        uint32 value = r_u32(steering + 0xc);
        w_u32(steering + 0x10, value);
        uint32 copied[2];
        for (uint32 word = 0; word < 2; ++word)
        {
            uint32 source = steering + 0x14 + word * 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
            copied[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 word = 0; word < 2; ++word)
        {
            uint32 destination = steering + 0x1c + word * 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, copied[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, copied[word] << (low_lane * 8), (15 << low_lane) & 15);
        }
        G3Function800727A4(object, game, 0x80099e7c, frame);
        G3Function8007246C(object, game, state, controls, frame);
    }
    uint32 steering = r_u32(0x800a0284);
    bool clear = steering != 0 && r_s16(steering + 0x18) < 0x200;
    uint32 flags = r_u32(object + 0x14);
    uint32 result = clear ? flags & 0xffffffbf : flags | 0x40;
    w_u32(object + 0x14, result);
    return result;
}

uint32 G3Function800760F4(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800760f4, "SLUS_008.06");
uint32 frame = guest_stack - 0x20;
    uint32 motion = r_u32(object + 0x154);
    if (motion == 1)
    {
        G3Function8007D190(object, game, state, controls, frame);
        uint32 flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags & 0xfffffeff);
    }
    else if (motion == 0x100000)
    {
        G3Function800789C0(object, game, state, frame);
        uint32 flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags & 0xfffffeff);
    }
    uint32 camera = r_u32(game + 0x4c);
    if (G3Function8004DB2C(camera, frame) != 0)
    {
        uint32 flags = r_u32(object + 0x10);
        uint32 result = flags | 0x800;
        w_u32(object + 0x10, result);
        uint32 linked = r_u32(state + 0x1fc);
        if (linked != 0)
        {
            flags = r_u32(linked + 0x10);
            result = flags | 0x800;
            w_u32(linked + 0x10, result);
        }
        return result;
    }
    r_u32(game + 0x4c);
    if (G3Function8004DAE4() == 0)
    {
        uint32 result = G3Function800762F4(object);
        w_u32(0x8009d34c, 0);
        return result;
    }
    r_u32(object + 0x10);
    return G3Function8007E270(object, game, state, frame);
}

uint32 G3Function8007AF30(uint32 object, uint32 game, uint32 state)
{
    FUNCTION_MARKER(0x8007af30, "SLUS_008.06");
sint32 counter = r_s16(state + 0xc4);
    sint32 limit = r_s16(state + 0x1d4);
    if (counter >= limit)
    {
        uint32 value = r_u16(0x8009da4c + ((uint32)(counter - limit) << 1));
        w_u16(object + 0x6c, value);
    }
    uint32 next = r_u16(state + 0xc4);
    limit = r_s16(state + 0x1d4);
    next += 1;
    w_u16(state + 0xc4, next);
    uint32 result = 1;
    if ((sint16)next == limit + 0x17)
    {
        w_u32(object + 0x154, 1);
        w_u16(object + 0x6c, 0x1000);
        w_u16(state + 0xe0, 0);
        w_u16(state + 0xe2, 0);
        result = 0x1000;
    }
    uint32 source = state + 0x1f4;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 low_lane = source & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
    uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
    uint32 packed = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    uint32 height = (uint32)r_s16(state + 0x1f8);
    uint32 destination = object + 0x58;
    high_address = destination + 3;
    high_lane = high_address & 3;
    low_lane = destination & 3;
    w_masked_u32(high_address & 0xfffffffc, packed >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    w_masked_u32(destination & 0xfffffffc, packed << (low_lane * 8), (15 << low_lane) & 15);
    w_u16(object + 0x5c, height);
    return result;
}

uint32 G3Function8007AFBC(uint32 object, uint32 game, uint32 state)
{
    FUNCTION_MARKER(0x8007afbc, "SLUS_008.06");
uint32 angle = r_u16(object + 0x64) + 0x4b0;
    w_u16(object + 0x64, angle);
    uint32 counter = r_u16(state + 0xc4);
    w_u16(state + 0xc6, angle);
    sint32 limit = r_s16(state + 0x1d4);
    counter += 1;
    w_u16(state + 0xc4, counter);
    if ((sint16)counter != limit + 0x17)
        return 1;
    w_u32(object + 0x154, 1);
    w_u16(object + 0x6c, 0x1000);
    w_u16(state + 0xe0, 0);
    w_u16(state + 0xe2, 0);
    return 0x1000;
}

uint32 G3Function8007B010(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007b010, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 linked = r_u32(state + 0x1fc);
    if (linked == 0)
    {
        w_u16(object + 0x6e, 0xa);
        w_u32(object + 0x154, 0x04000000);
        w_u16(object + 0x5e, 0);
        uint32 flags = r_u32(state + 0xb8);
        uint32 result = flags | 8;
        w_u32(state + 0xb8, result);
        return result;
    }
    uint32 matrices = r_u32(linked + 0x30);
    if (matrices == 0)
        return linked;
    uint32 index = r_u8(state + 0x200);
    uint32 matrix = matrices + (index << 5);
    uint32 value = r_u16(matrix + 0x14);
    w_u16(object + 0x58, value);
    value = r_u16(matrix + 0x18);
    w_u16(object + 0x5a, value);
    value = r_u16(matrix + 0x1c);
    w_u16(object + 0x5c, value);
    linked = r_u32(state + 0x1fc);
    index = r_u8(state + 0x200);
    matrices = r_u32(linked + 0x30);
    G3Function800202A4(matrices + (index << 5), object + 0x60, 0x15, frame);
    value = r_u16(object + 0x64);
    linked = r_u32(state + 0x1fc);
    w_u16(state + 0xc6, value);
    w_u16(state + 0xc8, value);
    uint32 target = r_u32(linked + 0xec);
    if (target == 0)
        return target;
    return G3DispatchStateEvent(target, linked, game, 0x7ea, 0, frame);
}

uint32 G3Function8007BA50(uint32 object, uint32 game, uint32 state, uint32 controls)
{
    FUNCTION_MARKER(0x8007ba50, "SLUS_008.06");
if ((r_u32(controls) & 0x80) != 0)
    {
        sint32 limit = r_s16(state + 0xea);
        sint32 current = r_s16(state + 0xe2);
        uint32 result = r_u16(state + 0xe2) + 1;
        if (current < limit - 1)
            w_u16(state + 0xe2, result);
        return result;
    }
    sint32 current = r_s16(state + 0xe2);
    sint32 decrement = r_s16(state + 0x8e);
    uint32 raw_current = r_u16(state + 0xe2);
    uint32 raw_decrement = r_u16(state + 0x8e);
    uint32 result = raw_current - raw_decrement;
    w_u16(state + 0xe2, decrement < current ? result : 0);
    return result;
}

uint32 G3Function8007D284(uint32 object, uint32 game, uint32 state, uint32 controls)
{
    FUNCTION_MARKER(0x8007d284, "SLUS_008.06");
if ((r_u32(controls + 4) & 0x20) == 0)
        return 0x2000;
    w_u32(object + 0x154, 0x2000);
    w_u16(object + 0x6e, 0x1e);
    w_u16(object + 0x5e, 0);
    uint32 destination = r_u32(state + 0xfc);
    uint32 value = r_u32(state + 0xf0);
    w_u32(destination, value);
    destination = r_u32(state + 0x108);
    value = r_u32(state + 0xf0);
    w_u32(destination, value);
    destination = r_u32(state + 0x114);
    value = r_u32(state + 0xf0);
    w_u32(destination, value);
    uint32 flags = r_u32(object + 0x128);
    uint32 result = flags & 0xfffffff7;
    w_u32(object + 0x128, result);
    return result;
}

uint32 G3Function80079594(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80079594, "SLUS_008.06");
uint32 frame = guest_stack - 0x28;
    if ((r_u32(object + 0x14) & 0x10) != 0)
    {
        uint32 owner = r_u32(object + 0x1c);
        uint32 animation = r_u16(object + 0x6e);
        uint32 table = r_u32(owner + 0x10);
        uint32 entry = r_u32(table + (animation << 2));
        uint32 count = r_u16(entry + 2);
        w_u16(object + 0x5e, count - 1);
    }
    if ((r_u16(state + 0xce) & 4) != 0 || ((r_u32(controls + 4) & 0x800) != 0 && (r_u32(object + 0x134) & 4) != 0))
    {
        uint32 height = r_u16(object + 0x5c);
        w_u32(object + 0x154, 0x40);
        w_u16(object + 0x6e, 0x13);
        w_u16(object + 0x5e, 0);
        w_u16(state + 0x18c, height);
    }
    uint32 destination = r_u32(state + 0xfc);
    uint32 value = r_u32(state + 0xf0);
    w_u32(destination, value);
    destination = r_u32(state + 0x108);
    value = r_u32(state + 0xf0);
    w_u32(destination, value);
    destination = r_u32(state + 0x114);
    value = r_u32(state + 0xf0);
    w_u32(destination, value);
    G3Function8007D9EC(object, game, state, controls, frame);
    G3Function8007D818(object, game, state, controls, frame);
    uint32 result = G3Function8007DB40(object, game, state, controls, frame);
    return G3Function8007DB80(object, game, state, controls, frame, result);
}

uint32 G3Function8007A5E8(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007a5e8, "SLUS_008.06");
uint32 frame = guest_stack - 0x28;
    w_u16(state + 0xe2, 0);
    w_u16(state + 0xe0, 0);
    if ((r_u32(object + 0x14) & 0x10) != 0)
    {
        w_u16(object + 0x6e, 0x1d);
        w_u16(object + 0x5e, 0);
    }
    bool fallback = false;
    if ((r_u32(controls) & 0x800) == 0)
    {
        uint32 current_game = r_u32(0x80097048);
        fallback = (r_u32(current_game + 0x6834) & 0x10) == 0;
    }
    if (!fallback)
        fallback = (r_u32(object + 0x128) & 0x20000) != 0;
    if (fallback)
    {
        w_u32(object + 0x154, 0x4000);
        w_u16(object + 0x6e, 0x1c);
        w_u16(object + 0x5e, 0);
    }
    G3Function8007D284(object, game, state, controls);
    G3Function8007CE9C(object, game, state, controls, frame);
    G3Function8007CD24(object, game, state, controls, frame);
    uint32 motion = r_u32(object + 0x154);
    if (motion != 0x1000)
        return 0x1000;
    uint32 result = r_u32(state + 0xf8);
    uint32 key = (uint32)r_s16(state + 0x18a);
    uint32 animation = r_u32(result + 0xc);
    G3Function8006F34C(animation, key - 0x3a, 0x46);
    return result;
}

uint32 G3Function8007AC3C(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007ac3c, "SLUS_008.06");
uint32 frame = guest_stack - 0x28;
    if ((r_u16(state + 0xce) & 0x10) != 0)
    {
        w_u32(object + 0x154, 1);
        w_u16(object + 0x5e, 0);
        w_u16(object + 0x6e, 0);
    }
    G3Function8007CD24(object, game, state, controls, frame);
    G3Function8007CE9C(object, game, state, controls, frame);
    uint32 result = G3Function8007D190(object, game, state, controls, frame);
    return G3Function8007DB80(object, game, state, controls, frame, result);
}

uint32 G3Function8007E7C0(uint32 object, uint32 linked, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007e7c0, "SLUS_008.06");
uint32 frame = guest_stack - 0x30;
    G3Function8003EDC4(0x7d, 1);
    uint32 descriptor = r_u32(linked + 0x20);
    uint32 paths = r_u32(descriptor + 0x24);
    uint32 path = r_u32(paths);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 index = (uint32)r_s16(path + 4);
        uint32 base = r_u32(path);
        uint32 value = r_u16(base + (index << 5) - 0x1e + axis * 2);
        w_u16(frame + 0x10 + axis * 2, value);
    }
    uint32 source = linked + 0x58;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 low_lane = source & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
    uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
    uint32 packed = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    uint32 height = (uint32)r_s16(linked + 0x5c);
    uint32 destination = frame + 0x18;
    high_address = destination + 3;
    high_lane = high_address & 3;
    low_lane = destination & 3;
    w_masked_u32(high_address & 0xfffffffc, packed >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    w_masked_u32(destination & 0xfffffffc, packed << (low_lane * 8), (15 << low_lane) & 15);
    w_u16(frame + 0x1c, height);
    w_u32(linked + 0x150, 1);
    w_u32(linked + 0x154, 0);
    uint32 object_x = (uint32)r_s16(object + 0x58);
    uint32 object_z = (uint32)r_s16(object + 0x5c);
    uint32 linked_z = (uint32)r_s16(linked + 0x5c);
    uint32 linked_x = (uint32)r_s16(linked + 0x58);
    uint32 angle = G3Function8002F240(object_z - linked_z, object_x - linked_x, frame);
    w_u32(linked + 0x130, (uint32)((sint32)(angle << 16) >> 16));
    w_u32(linked + 0x134, 0x4b0);
    uint32 saved_y = (uint32)r_s16(frame + 0x1a);
    w_u32(linked + 0x138, saved_y);
    sint32 initial_y = r_s16(frame + 0x1a);
    sint32 object_y = r_s16(object + 0x5a);
    w_u32(linked + 0x13c, object_y < initial_y ? 0x80 : 0xffffff80);
    uint32 magnitude = r_u32(linked + 0x130);
    if ((sint32)magnitude < 0)
        magnitude = 0 - magnitude;
    w_u32(linked + 0x140, (sint32)magnitude < 0x401 ? 0x88 : 0xffffff78);
    w_u16(object + 0x6e, 0x31);
    uint32 flags = r_u32(object + 0x14);
    w_u16(object + 0x5e, 0);
    w_u32(object + 0x14, flags & 0xffffffef);
    return 1;
}

uint32 G3Function8007EB0C(uint32 object, uint32 linked, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007eb0c, "SLUS_008.06");
uint32 frame = guest_stack - 0x40;
    uint32 descriptor = r_u32(linked + 0x20);
    uint32 value = r_u16(object + 0x58);
    uint32 paths = r_u32(descriptor + 0x24);
    w_u16(frame + 0x10, value);
    value = r_u16(object + 0x5a);
    w_u16(frame + 0x12, value);
    value = r_u16(object + 0x5c);
    w_u16(frame + 0x14, value);
    uint32 channel = r_u32(paths);
    uint32 point = G3Function8003FBB0(channel, paths + 0x10, frame);
    if (point == 0)
        return 0;
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        value = r_u16(point + axis * 2);
        w_u16(frame + 0x18 + axis * 2, value);
    }
    G3Function80038D80(object + 0x58, frame + 0x18, 0x10000, frame);
    if (r_u32(paths + 4) != 0)
    {
        channel = r_u32(paths);
        uint32 key = G3Function8003F8A4(channel, paths + 0x10, frame, 0);
        channel = r_u32(paths + 4);
        G3Function8003F93C(channel, paths + 0x14, key & 0xffff, frame);
        channel = r_u32(paths + 4);
        if (G3Function80041008(channel, paths + 0x14, frame + 0x20, frame) != 0)
        {
            uint32 copied[2];
            for (uint32 word = 0; word < 2; ++word)
            {
                uint32 source = frame + 0x20 + word * 4;
                uint32 high_address = source + 3;
                uint32 high_lane = high_address & 3;
                uint32 low_lane = source & 3;
                uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
                copied[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
            }
            for (uint32 word = 0; word < 2; ++word)
            {
                uint32 destination = object + 0x60 + word * 4;
                uint32 high_address = destination + 3;
                uint32 high_lane = high_address & 3;
                uint32 low_lane = destination & 3;
                w_masked_u32(high_address & 0xfffffffc, copied[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                w_masked_u32(destination & 0xfffffffc, copied[word] << (low_lane * 8), (15 << low_lane) & 15);
            }
        }
    }
    w_u16(object + 0x6e, 0x41);
    uint32 flags = r_u32(object + 0x14);
    uint32 game = r_u32(0x80097048);
    w_u16(object + 0x5e, 0);
    w_u32(object + 0x14, flags & 0xffffffef);
    uint32 camera = r_u32(game + 0x4c);
    G3Function80014220(camera, 6);
    return 1;
}

uint32 G3Function8007E6B0(uint32 object, uint32 linked, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007e6b0, "SLUS_008.06");
uint32 frame = guest_stack - 0x18;
    if ((r_u32(object + 0x128) & 1) != 0 || linked == 0)
        return 0;
    uint32 descriptor = r_u32(linked + 0x20);
    if (r_u32(descriptor + 0x24) == 0)
        return 0;
    uint32 owner = r_u32(linked + 0x1c);
    uint32 name = r_u32(owner + 0x20);
    uint32 expected = r_u32(0x800a0310);
    uint32 actual = r_u32(name);
    bool matches = actual == expected;
    if (matches)
    {
        actual = r_u32(name + 4);
        expected = r_u32(0x800a0314);
        matches = actual == expected;
    }
    if (matches)
        return G3Function8007EB0C(object, linked, frame);
    return G3Function8007E7C0(object, linked, frame);
}

uint32 G3Function8007E92C(uint32 object, uint32 linked, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007e92c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 y = (uint32)r_s16(object + 0x5a);
    uint32 target_y = r_u32(linked + 0x138);
    uint32 distance = y - target_y;
    if ((sint32)distance < 0)
        distance = target_y - y;
    if ((sint32)distance < 0x40 && r_u32(linked + 0x130) == 0x400)
    {
        G3Function8003EDC4(0x7d, 0);
        w_u32(linked + 0x150, 0xffffffff);
        return 0;
    }
    if (r_u32(linked + 0x154) == 0)
    {
        uint32 angle = r_u32(linked + 0x130);
        uint32 step = r_u32(linked + 0x140);
        w_u32(linked + 0x130, (angle + step) & 0xfff);
    }
    y = r_u16(object + 0x5a);
    uint32 vertical_step = r_u16(linked + 0x13c);
    w_u16(object + 0x5a, y + vertical_step);
    uint32 angle = r_u32(linked + 0x130);
    uint32 sine = G3Function8005B470(angle);
    uint32 product = sine * 2000;
    uint32 origin_x = r_u16(linked + 0x58);
    w_u16(object + 0x58, origin_x + (uint32)((sint32)product >> 12));
    angle = r_u32(linked + 0x130);
    uint32 cosine = G3Function8005B590(angle);
    uint32 amplitude = r_u32(linked + 0x134);
    product = amplitude * cosine;
    sint32 current_y = r_s16(object + 0x5a);
    uint32 origin_z = r_u16(linked + 0x5c);
    w_u16(object + 0x5c, origin_z + (uint32)((sint32)product >> 12) - 0xc8);
    sint32 limit_y = r_s32(linked + 0x138);
    bool clamp = false;
    if (limit_y < current_y)
        clamp = r_s32(linked + 0x13c) > 0;
    else if (current_y < limit_y)
        clamp = r_s32(linked + 0x13c) < 0;
    if (clamp)
    {
        uint32 height = r_u16(linked + 0x138);
        w_u16(object + 0x5a, height);
    }
    angle = r_u32(linked + 0x130);
    uint32 step = r_u32(linked + 0x140);
    if ((sint32)angle < 0)
        angle = 0 - angle;
    distance = angle - 0x400;
    if ((sint32)distance < 0)
        distance = 0 - distance;
    if ((sint32)step < 0)
        step = 0 - step;
    if ((sint32)distance < (sint32)step)
    {
        origin_x = r_u16(linked + 0x58);
        w_u32(linked + 0x130, 0x400);
        w_u16(object + 0x58, origin_x);
        w_u32(linked + 0x154, 1);
    }
    return 1;
}

uint32 G3Function8007EC48(uint32 object, uint32 linked, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007ec48, "SLUS_008.06");
    uint32 frame = guest_stack - 0x70;
    uint32 descriptor = r_u32(linked + 0x20);
    uint32 animation = r_u16(object + 0x6e);
    uint32 paths = r_u32(descriptor + 0x24);
    uint32 result = 0;
    if (animation == 0x41)
    {
        uint32 flags = r_u32(object + 0x10);
        uint32 mode = r_u32(object + 0x14);
        w_u32(object + 0x10, flags | 0x100);
        result = 1;
        if ((mode & 0x10) != 0)
        {
            w_u32(object + 0x14, mode & 0xffffffef);
            w_u16(object + 0x6e, 0x42);
            w_u16(object + 0x5e, 0);
        }
    }
    else
    {
        uint32 channel = r_u32(paths);
        if (channel != 0)
        {
            G3Function80041008(channel, paths + 0x10, frame + 0x18, frame);
            uint32 direction = r_u32(object + 0x128);
            channel = r_u32(paths);
            uint32 moved;
            if ((direction & 2) != 0)
                moved = G3Function8004136C(channel, paths + 0x10, frame);
            else
                moved = G3Function80041230(channel, paths + 0x10, frame);
            if (moved != 0)
            {
                channel = r_u32(paths);
                if (G3Function80041008(channel, paths + 0x10, frame + 0x10, frame) != 0)
                {
                    uint32 next = r_u16(frame + 0x10);
                    uint32 previous = r_u16(frame + 0x18);
                    uint32 dx = next - previous;
                    sint32 sx = (sint16)dx;
                    next = r_u16(frame + 0x12);
                    previous = r_u16(frame + 0x1a);
                    uint32 dy = next - previous;
                    sint32 sy = (sint16)dy;
                    next = r_u16(frame + 0x14);
                    previous = r_u16(frame + 0x1c);
                    uint32 dz = next - previous;
                    sint32 sz = (sint16)dz;
                    result = 1;
                    w_u16(frame + 0x20, dx);
                    w_u16(frame + 0x22, dy);
                    uint32 sum = (uint32)(sx * sx) + (uint32)(sy * sy) + (uint32)(sz * sz);
                    w_u16(frame + 0x24, dz);
                    if (sum != 0)
                    {
                        G3Function80038D80(object + 0x58, frame + 0x10, sum << 2, frame);
                        uint32 current = r_u16(object + 0x58);
                        previous = r_u16(object + 0x70);
                        dx = current - previous;
                        sx = (sint16)dx;
                        w_u16(frame + 0x20, dx);
                        current = r_u16(object + 0x5a);
                        previous = r_u16(object + 0x72);
                        dy = current - previous;
                        sy = (sint16)dy;
                        w_u16(frame + 0x22, dy);
                        previous = r_u16(object + 0x74);
                        current = r_u16(object + 0x5c);
                        dz = current - previous;
                        w_u16(frame + 0x24, dz);
                        sum = (uint32)(sx * sx) + (uint32)(sy * sy);
                        uint32 length;
                        if ((sint32)sum > 0x08000000)
                        {
                            uint32 root = G3Function8002F368(sum, frame);
                            if ((sint32)root < 0)
                                root += 0x3f;
                            length = (uint32)((sint32)root >> 6);
                        }
                        else if ((sint32)sum > 0x80000)
                        {
                            uint32 root = G3Function8002F448(sum << 4, 4, frame) + 8;
                            length = (uint32)((sint32)root >> 4);
                        }
                        else
                        {
                            uint32 root = G3Function8002F368(sum << 12, frame);
                            length = (uint32)((sint32)root >> 12);
                        }
                        uint32 y = (uint32)r_s16(frame + 0x22);
                        uint32 x = (uint32)r_s16(frame + 0x20);
                        uint32 angle = G3Function8002F240(y, x, frame);
                        uint32 yaw = (uint32)((sint32)(angle << 16) >> 16) + 0x400;
                        uint32 z = (uint32)r_s16(frame + 0x24);
                        angle = G3Function8002F240(0 - z, length, frame);
                        uint32 pitch = (uint32)((sint32)(angle << 16) >> 16);
                        G3Function8002F718(frame + 0x30);
                        G3Function8005BA5C(pitch, frame + 0x30, frame);
                        G3Function8005BB6C(yaw, frame + 0x30, frame);
                        G3Function800202A4(frame + 0x30, frame + 0x28, 0x15, frame);
                        for (uint32 axis = 0; axis < 3; ++axis)
                        {
                            uint32 value = r_u16(frame + 0x28 + axis * 2);
                            w_u16(object + 0x60 + axis * 2, value);
                        }
                    }
                }
            }
            uint32 game = r_u32(0x80097048);
            if ((r_u32(game + 0x74) & 0x80) != 0 || moved == 0)
            {
                uint32 current = r_u16(object + 0x58);
                uint32 previous = r_u16(object + 0x70);
                uint32 dx = current - previous;
                result = 0;
                w_u16(frame + 0x28, dx);
                current = r_u16(object + 0x5a);
                previous = r_u16(object + 0x72);
                w_u16(frame + 0x2a, current - previous);
                current = r_u16(object + 0x5c);
                previous = r_u16(object + 0x74);
                w_u16(frame + 0x2c, current - previous);
                w_u32(object + 0x104, (uint32)((sint32)(dx << 16) >> 16));
                uint32 dy = (uint32)r_s16(frame + 0x2a);
                w_u16(object + 0x6e, 0x43);
                uint32 flags = r_u32(object + 0x14);
                w_u16(object + 0x5e, 0);
                w_u32(object + 0x108, dy);
                uint32 height = r_u16(object + 0x5c);
                w_u32(object + 0x14, flags & 0xffffffef);
                w_u16(object + 0x5c, height - 0xc8);
            }
        }
    }
    if (result == 0)
    {
        uint32 game = r_u32(0x80097048);
        uint32 camera = r_u32(game + 0x4c);
        G3Function80014254(camera, 2);
        game = r_u32(0x80097048);
        camera = r_u32(game + 0x4c);
        G3Function80014254(camera, 4);
    }
    return result;
}
