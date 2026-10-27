#include "xport.h"
#include "psx.h"
#include "game_functions.h"

uint32 G3Function800169C8(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x800169c8, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 value = r_u16(command + 4);
    uint32 level = r_u32(game + 0x48);
    w_u16(level + 0x4c, value);
    level = r_u32(game + 0x48);
    G3Function8002C50C(level);
    return 2;
}

uint32 G3Function80016A04(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80016a04, "SLUS_008.06");
    uint32 target = r_u32(command + 4);
    uint32 flags = r_u32(target);
    w_u32(target, flags & 0xfffffffd);
    return 2;
}

uint32 G3Function80016A24(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80016a24, "SLUS_008.06");
    uint32 target = r_u32(command + 4);
    uint32 flags = r_u32(target);
    w_u32(target, flags | 2);
    return 2;
}

uint32 G3Function80016A68(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80016a68, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 first = r_u32(command + 4);
    uint32 second = r_u32(command + 8);
    uint32 camera = r_u32(game + 0x4c);
    G3Function80014460(camera, first, second);
    return 3;
}

uint32 G3Function80016AA0(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80016aa0, "SLUS_008.06");
    uint32 destination = r_u8(command + 4);
    uint32 first = r_u8(command + 5);
    uint32 second = r_u8(command + 6);
    uint32 left = r_u32(0x800a0760 + (first << 2));
    uint32 right = r_u32(0x800a0760 + (second << 2));
    w_u32(0x800a0760 + (destination << 2), left & right);
    return 2;
}

uint32 G3Function80016AE4(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80016ae4, "SLUS_008.06");
    uint32 destination = r_u8(command + 4);
    uint32 first = r_u8(command + 5);
    uint32 second = r_u8(command + 6);
    uint32 left = r_u32(0x800a0760 + (first << 2));
    uint32 right = r_u32(0x800a0760 + (second << 2));
    w_u32(0x800a0760 + (destination << 2), left | right);
    return 2;
}

uint32 G3Function80016B28(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80016b28, "SLUS_008.06");
    uint32 destination = r_u8(command + 4);
    uint32 first = r_u8(command + 5);
    uint32 second = r_u8(command + 6);
    uint32 left = r_u32(0x800a0760 + (first << 2));
    uint32 right = r_u32(0x800a0760 + (second << 2));
    w_u32(0x800a0760 + (destination << 2), left ^ right);
    return 2;
}

uint32 G3Function80016BDC(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80016bdc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 index = r_u8(command + 4);
    uint32 value = r_u32(0x800a0760 + (index << 2));
    uint32 sequence = r_u32(command + 8);
    if (value == 0)
    {
        if (sequence == 0)
            return 3;
        G3Function80018DE0(object, sequence, frame);
        return 0;
    }
    return sequence != 0 ? 3 : 0;
}

uint32 G3Function80016C4C(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80016c4c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 first = r_u8(command + 4);
    uint32 second = r_u8(command + 5);
    uint32 left = r_u32(0x800a0760 + (first << 2));
    uint32 right = r_u32(0x800a0760 + (second << 2));
    uint32 sequence = r_u32(command + 8);
    if (left == right)
    {
        if (sequence == 0)
            return 3;
        G3Function80018DE0(object, sequence, frame);
        return 0;
    }
    return sequence != 0 ? 3 : 0;
}

uint32 G3Function80016D44(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80016d44, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 target = r_u32(object + 0xec);
    if (target != 0)
    {
        uint32 game = r_u32(0x80097048);
        G3DispatchStateEvent(target, object, game, 0x3f2, command + 4, frame);
    }
    return 3;
}

uint32 G3Function80016D80(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80016d80, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 target = r_u32(object + 0xec);
    if (target != 0)
    {
        uint32 game = r_u32(0x80097048);
        G3DispatchStateEvent(target, object, game, 0x3f3, command + 4, frame);
    }
    return 2;
}

uint32 G3Function80016DE4(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80016de4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 target = r_u32(object + 0xec);
    if (target != 0)
    {
        uint32 game = r_u32(0x80097048);
        G3DispatchStateEvent(target, object, game, 0x7e3, command + 4, frame);
    }
    return 3;
}

uint32 G3Function80016E20(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80016e20, "SLUS_008.06");
    uint32 destination = r_u8(command + 4);
    uint32 first = r_u8(command + 5);
    uint32 second = r_u8(command + 6);
    uint32 left = r_u32(0x800a0760 + (first << 2));
    uint32 right = r_u32(0x800a0760 + (second << 2));
    w_u32(0x800a0760 + (destination << 2), left + right);
    return 2;
}

uint32 G3Function80016E64(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80016e64, "SLUS_008.06");
    uint32 destination = r_u8(command + 4);
    uint32 first = r_u8(command + 5);
    uint32 second = r_u8(command + 6);
    uint32 left = r_u32(0x800a0760 + (first << 2));
    uint32 right = r_u32(0x800a0760 + (second << 2));
    w_u32(0x800a0760 + (destination << 2), left - right);
    return 2;
}

uint32 G3Function80016EA8(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80016ea8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 sequence = r_u32(command + 4);
    G3Function80018DE0(object, sequence, frame);
    return 0;
}

uint32 G3Function80016EDC(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80016edc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 sequence = r_u32(command + 4);
    G3Function80018DE0(object, sequence, frame);
    return 2;
}

uint32 G3Function80016F50(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80016f50, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x28;
    uint32 result = 0;
    uint32 requested = r_u32(command + 8);
    uint32 flags = r_u32(game + 0x6834);
    uint32 destination = game + 0xc0;
    if ((flags & 0x10) == 0)
    {
        G3FillMemory80086094(destination, 0, 0x28);
        return 0;
    }
    for (uint32 bit = 0; bit < 12; ++bit)
    {
        if ((requested & (1 << bit)) != 0)
        {
            uint32 output_bit = bit == 10 ? 11 : bit == 11 ? 10 : bit;
            uint32 current = r_u32(destination);
            w_u32(destination, current | (1 << output_bit));
        }
    }
    if ((requested & 0x1000) != 0)
    {
        game = r_u32(0x80097048);
        uint32 player = r_u32(0x8009efc4);
        for (uint32 coordinate = 0; coordinate < 3; ++coordinate)
        {
            uint32 target = r_u16(game + 0x69e4 + coordinate * 2);
            uint32 position = r_u16(player + 0x58 + coordinate * 2);
            w_u16(frame + 0x10 + coordinate * 2, target - position);
        }
        sint32 distance = (sint32)G3Function8002FB10(frame + 0x10);
        if (distance >= 31)
        {
            if (distance >= 128)
                distance = 127;
            game = r_u32(0x80097048);
            player = r_u32(0x8009efc4);
            uint32 target_y = (uint32)r_s16(game + 0x69e6);
            uint32 player_y = (uint32)r_s16(player + 0x5a);
            uint32 target_x = (uint32)r_s16(game + 0x69e4);
            uint32 player_x = (uint32)r_s16(player + 0x58);
            uint32 bearing = G3Function8002F0C4(target_y - player_y, target_x - player_x, frame);
            game = r_u32(0x80097048);
            uint32 camera_angle = G3Function80014F00(game);
            uint32 angle = (uint32)(sint16)(bearing - camera_angle);
            uint32 sine = G3Function8005B470(angle);
            uint32 product = sine * (uint32)distance;
            game = r_u32(0x80097048);
            w_u32(game + 0xcc, (uint32)((sint32)product >> 12));
            uint32 cosine = G3Function8005B590(angle);
            product = cosine * (uint32)distance;
            game = r_u32(0x80097048);
            w_u32(game + 0xd0, 0 - (uint32)((sint32)product >> 12));
            result = 1;
        }
    }
    uint32 buttons = G3Function8004DC44();
    uint32 current = r_u32(destination);
    w_u32(destination, current | buttons);
    return result;
}

uint32 G3Function8001722C(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8001722c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 kind = r_u32(command + 4);
    w_u32(frame + 0x10, 0);
    G3Function80015304(object, kind, 0x80016f50, command, frame);
    return 3;
}

uint32 G3Function8001725C(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8001725c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function8006F55C(object, command, frame);
    return 3;
}

uint32 G3Function8004DC44(void)
{
    FUNCTION_MARKER(0x8004dc44, "SLUS_008.06");
    uint32 flags = r_u32(0x80096b28);
    if ((flags & 1) == 0)
        return 0;
    uint32 game = r_u32(0x80097048);
    return r_u32(game + 0x98) & 0x300;
}

uint32 G3Function80017384(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80017384, "SLUS_008.06");
    uint32 index = r_u16(command + 4);
    uint32 first = r_u32(command + 8);
    uint32 second = r_u32(command + 0x0c);
    uint32 setting = r_u16(command + 6);
    if (index < 16)
    {
        uint32 slot = 0x800a0b60 + index * 12;
        w_u16(slot, setting);
        if (first != 0)
            w_u32(slot + 4, first + 4);
        if (second != 0)
            w_u32(slot + 8, second + 4);
    }
    return 4;
}

uint32 G3Function800173E0(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x800173e0, "SLUS_008.06");
    uint32 index = (uint32)r_s16(command + 4);
    uint32 slot = 0x800a0b60 + index * 12;
    w_u32(slot + 4, 0);
    w_u32(slot + 8, 0);
    return 2;
}

uint32 G3Function80017410(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80017410, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 mode = r_u32(command + 4);
    if (mode == 0xffff)
    {
        uint32 game = r_u32(0x80097048);
        w_u8(game + 0x67dc, 1);
        game = r_u32(0x80097048);
        w_u8(game + 0x67dd, 1);
        G3Function8003F068(frame);
        G3Function8003F004(frame);
    }
    else if (mode == 0xfffe)
    {
        uint32 game = r_u32(0x80097048);
        w_u8(game + 0x67dc, 0);
        game = r_u32(0x80097048);
        w_u8(game + 0x67dd, 0);
        G3Function8003F040(frame);
        G3Function8003EF8C(frame);
    }
    else if ((sint32)mode >= 128)
    {
        uint32 sound = r_u16(command + 4);
        G3Function8003F128(sound, 0);
    }
    return 2;
}

uint32 G3Function800174B4(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x800174b4, "SLUS_008.06");
    uint32 volume = r_u32(command + 4);
    G3Function8003EDE8(volume);
    return 2;
}

uint32 G3Function800174D8(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800174d8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 value = r_u32(command);
    w_u32(frame + 0x14, value);
    value = r_u32(command + 4);
    w_u32(frame + 0x18, value);
    value = r_u32(command + 8);
    w_u32(frame + 0x1c, value);
    G3Function8003EAB0(frame + 0x10, frame);
    return 3;
}

uint32 G3Function80017518(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80017518, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    G3Function800274D8(frame + 0x10, frame + 0x20, command + 4);
    uint32 game = r_u32(0x80097048);
    G3Function80027644(frame + 0x10, frame + 0x20, game, frame);
    game = r_u32(0x80097048);
    uint32 value = r_u16(command + 0x10);
    w_u16(game + 0x67f0, value);
    value = r_u16(command + 0x12);
    w_u16(game + 0x67f2, value);
    value = r_u16(command + 0x14);
    w_u16(game + 0x67f4, value);
    return 6;
}

uint32 G3Function80017598(void)
{
    FUNCTION_MARKER(0x80017598, "SLUS_008.06");
    return 2;
}

uint32 G3Function800175A0(void)
{
    FUNCTION_MARKER(0x800175a0, "SLUS_008.06");
    return 2;
}

uint32 G3Function800175A8(void)
{
    FUNCTION_MARKER(0x800175a8, "SLUS_008.06");
    return 2;
}

uint32 G3Function800789C0(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800789c0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x10, flags | 0x100);
    w_u16(state + 0xe2, 0);
    w_u16(state + 0xe0, 0);
    flags = r_u32(object + 0x14);
    if ((flags & 0x10) != 0)
    {
        w_u16(object + 0x5e, 0);
        uint32 linked = r_u32(state + 0x17c);
        if (linked != 0 && r_u16(object + 0x6e) != 0x23)
        {
            G3Function800772F0(linked, game, frame);
            w_u32(state + 0x17c, 0);
            w_u16(object + 0x6e, 0x23);
        }
        else
        {
            uint32 commands = r_u32(state);
            w_u16(object + 0x6e, 0);
            w_u32(object + 0x154, 1);
            w_u32(object + 0xa4, commands);
        }
    }
    else
    {
        w_u16(frame + 0x1e, 10);
        w_u32(frame + 0x20, frame + 0x18);
        w_u32(frame + 0x28, 0x8006f2bc);
        w_u32(frame + 0x2c, 0);
        w_u32(frame + 0x30, 0);
        w_u32(frame + 0x34, object);
        w_u32(frame + 0x38, 0);
        uint32 model = r_u32(object + 0x30);
        uint32 coordinate = r_u16(model + 0x274);
        w_u16(frame + 0x18, coordinate);
        model = r_u32(object + 0x30);
        coordinate = r_u16(model + 0x278);
        w_u16(frame + 0x1a, coordinate);
        model = r_u32(object + 0x30);
        coordinate = r_u16(model + 0x27c);
        w_u16(frame + 0x1c, coordinate);
        uint32 source = frame + 0x18;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 low_lane = source & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
        uint32 packed = (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        uint32 last = (uint32)r_s16(frame + 0x1c);
        uint32 destination = frame + 0x40;
        high_address = destination + 3;
        high_lane = high_address & 3;
        low_lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, packed >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, packed << (low_lane * 8), (15 << low_lane) & 15);
        w_u16(frame + 0x44, last);
        w_u32(frame + 0x24, frame + 0x40);
        G3Function8005FE44(frame + 0x20, game, frame);
    }
    uint32 linked = r_u32(state + 0x17c);
    uint32 animation = r_u16(object + 0x6e);
    if (linked != 0 && animation == 0x22 && r_s16(object + 0x5e) >= 6)
    {
        uint32 heading = r_u16(object + 0x64);
        uint32 linked_heading = r_u16(linked + 0x140);
        uint32 turn = (linked_heading - (heading - 0x400)) & 0xfff;
        if (turn >= 0x800)
            turn |= 0xf000;
        w_u32(frame + 0x10, 0x20);
        G3Function8007E1C8(state, (uint32)(sint16)turn, 0, 0x80, frame);
        G3Function8007DEAC(object, state + 0x22a, state + 0x1b6, game, frame);
        animation = r_u16(object + 0x6e);
    }
    if (animation == 0x23)
    {
        w_u32(frame + 0x10, 0x20);
        G3Function8007E1C8(state, 0, 0, 0x80, frame);
        G3Function8007DEAC(object, state + 0x22a, state + 0x1b6, game, frame);
    }
    linked = r_u32(state + 0x17c);
    if (linked == 0)
        return 0x22;
    animation = r_u16(object + 0x6e);
    if (animation != 0x22)
        return 0x22;
    uint32 result = r_u32(object + 0x128) & 0x8000;
    if (result == 0)
        return G3Function800358C0(0, 0x50, frame);
    return result;
}

uint32 G3Function8007A19C(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007a19c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 flags = r_u32(object + 0x14);
    if ((flags & 0x10) != 0)
    {
        w_u32(object + 0x154, 4);
        w_u16(object + 0x6e, 9);
        G3Function8007CF04(object, game, state, controls, frame);
        G3Function8007CD24(object, game, state, controls, frame);
        G3Function8007D0D0(object, game, state, controls, frame);
        G3Function8007D244(object, game, state, controls, frame);
        uint32 heading = r_u16(state + 0xc8) + 0x800;
        w_u16(state + 0xc8, heading);
        w_u16(state + 0xc6, heading);
        w_u16(state + 0xc2, heading);
        uint32 camera_heading = G3Function80014F00(game);
        heading = r_u16(state + 0xc2);
        uint32 count = r_u16(state + 0x20);
        uint32 relative = heading - camera_heading;
        uint32 index = count - 1;
        w_u16(state + 0xe0, index);
        uint32 table = r_u32(state + 0x34);
        uint32 offset = (uint32)((sint32)(index << 16) >> 15);
        w_u16(state + 0xbc, relative);
        uint32 limit = r_u16(table + offset);
        w_u16(state + 0x1d8, 0);
        w_u16(state + 0xe2, limit - 1);
    }
    uint32 animation_frame = r_u16(object + 0x5e);
    if ((animation_frame & 1) != 0)
    {
        w_u32(frame + 0x10, 10);
        G3Function80072D00(object, game, state, controls, frame);
    }
    uint32 previous_result = G3Function8007DB40(object, game, state, controls, frame);
    G3Function8007DB80(object, game, state, controls, frame, previous_result);
    return G3Function8007CD24(object, game, state, controls, frame);
}

uint32 G3Function8007A738(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007a738, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 flags = r_u16(state + 0xce);
    if ((flags & 2) == 0 && (r_u32(state + 0x204) & 8) == 0)
    {
        w_u32(object + 0x154, 2);
        w_u16(object + 0x6e, 8);
        w_u16(object + 0x5e, 0);
    }
    G3Function8007CF04(object, game, state, controls, frame);
    G3Function8007CD24(object, game, state, controls, frame);
    return G3Function8007D0D0(object, game, state, controls, frame);
}

uint32 G3Function8007A9A8(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007a9a8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x68;
    uint32 flags = r_u32(object + 0x14);
    if ((flags & 0x10) != 0)
    {
        w_u32(object + 0x154, 1);
        flags = r_u32(object + 0x10);
        uint32 model = r_u32(object + 0x30);
        w_u16(object + 0x6e, 0);
        w_u16(object + 0x5e, 0);
        w_u32(object + 0x10, flags & 0xfffffeff);
        uint32 coordinate = r_u16(model + 0x34);
        w_u16(object + 0x58, coordinate);
        coordinate = r_u16(model + 0x38);
        w_u16(object + 0x5a, coordinate);
        coordinate = r_u16(model + 0x3c);
        w_u16(object + 0x5c, coordinate - 0x72);
        uint32 linked = r_u32(state + 0x1e8);
        if (linked != 0)
        {
            flags = r_u32(linked + 0x10);
            w_u32(linked + 0x10, flags & 0xff7fffff);
            flags = r_u32(state + 0xb8);
            w_u32(state + 0xb8, flags & 0xfffffffe);
        }
    }
    else
    {
        uint32 linked = r_u32(state + 0xec);
        if (linked != 0)
        {
            uint32 first = r_u32(linked + 0x30);
            if (first != 0 && r_u32(linked + 0x34) != 0)
            {
                G3Function800202A4(first, frame + 0x40, 0x15, frame);
                uint32 second = r_u32(linked + 0x34);
                G3Function800202A4(second, frame + 0x48, 0x15, frame);
                for (uint32 axis = 0; axis < 3; ++axis)
                {
                    second = r_u32(linked + 0x34);
                    uint32 position = r_u16(object + 0x58 + axis * 2);
                    uint32 origin = r_u16(second + 0x14 + axis * 4);
                    w_u16(frame + 0x30 + axis * 2, position - origin);
                }
                uint32 angle = (uint32)r_s16(object + 0x64) - 0x400;
                uint32 sine = G3Function8005B470(angle);
                uint32 product = sine * 240;
                uint32 coordinate = r_u16(frame + 0x30);
                w_u16(frame + 0x30, coordinate + (uint32)((sint32)product >> 12));
                angle = (uint32)r_s16(object + 0x64) - 0x400;
                uint32 cosine = G3Function8005B590(angle);
                product = cosine * 240;
                coordinate = r_u16(frame + 0x32);
                w_u16(frame + 0x32, coordinate + (uint32)((sint32)product >> 12));
                second = r_u32(linked + 0x34);
                G3Function800313E0(frame + 0x10, second, frame);
                ApplyMatrixSV((MATRIX *)psx_addr(frame + 0x10, 20), (SVECTOR *)psx_addr(frame + 0x30, 8), (SVECTOR *)psx_addr(frame + 0x38, 6));
                first = r_u32(linked + 0x30);
                ApplyMatrixSV((MATRIX *)psx_addr(first, 20), (SVECTOR *)psx_addr(frame + 0x38, 8), (SVECTOR *)psx_addr(frame + 0x30, 6));
                first = r_u32(linked + 0x30);
                coordinate = r_u16(frame + 0x30);
                uint32 origin = r_u16(first + 0x14);
                w_u16(object + 0x58, coordinate + origin);
                first = r_u32(linked + 0x30);
                coordinate = r_u16(frame + 0x32);
                origin = r_u16(first + 0x18);
                angle = (uint32)r_s16(object + 0x64);
                w_u16(object + 0x5a, coordinate + origin);
                first = r_u32(linked + 0x30);
                coordinate = r_u16(frame + 0x34);
                origin = r_u16(first + 0x1c);
                angle -= 0x400;
                w_u16(object + 0x5c, coordinate + origin);
                sine = G3Function8005B470(angle);
                product = sine * 240;
                angle = (uint32)r_s16(object + 0x64);
                coordinate = r_u16(object + 0x58);
                angle -= 0x400;
                w_u16(object + 0x58, coordinate - (uint32)((sint32)product >> 12));
                cosine = G3Function8005B590(angle);
                product = cosine * 240;
                coordinate = r_u16(object + 0x5a);
                w_u16(object + 0x5a, coordinate - (uint32)((sint32)product >> 12));
                uint32 old_heading = r_u16(frame + 0x44);
                uint32 new_heading = r_u16(frame + 0x4c);
                uint32 heading = r_u16(object + 0x64);
                w_u16(object + 0x64, heading + old_heading - new_heading);
            }
        }
    }
    uint32 result = r_u32(object + 0x14) & 0xffffffbf;
    w_u32(object + 0x14, result);
    return result;
}

uint32 G3Function8007AE78(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007ae78, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 result = r_u32(object + 0x14) & 0x10;
    if (result != 0)
    {
        w_u32(object + 0x154, 1);
        w_u16(object + 0x6e, 0);
        uint32 flags = r_u32(state + 0xb8);
        w_u16(state + 0xe2, 0);
        w_u16(state + 0xe0, 0);
        w_u32(state + 0x1fc, 0);
        if ((flags & 8) == 0)
            w_u16(state + 0x120, 0x3c);
        flags = r_u32(state + 0xb8);
        w_u32(state + 0xb8, flags & 0xfffffff7);
        result = G3Function8007CE9C(object, game, state, controls, frame);
    }
    return G3Function8007DB80(object, game, state, controls, frame, result);
}

uint32 G3Function8007C800(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007c800, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    uint32 counter = r_u16(object + 0x9e);
    uint32 state = object + 0x104;
    ++counter;
    w_u16(object + 0x9e, counter);
    sint32 maximum = r_s16(state + 0x1a);
    if (maximum < (sint32)(sint16)counter)
    {
        uint32 first = r_u16(state + 0x18);
        w_u16(object + 0x9e, first);
    }
    G3Function80029F0C(object, 0xffffffff, frame);
    uint32 mode = r_u32(state + 0x24);
    if (mode == 2)
    {
        uint32 owner = r_u32(object + 0x1c);
        uint32 models = r_u32(owner + 0x0c);
        uint32 model = r_u32(models);
        uint32 channels = r_u32(model + 0x30);
        uint32 ticks = r_u32(state + 4);
        uint32 channel = r_u32(channels);
        uint32 point = ticks != 0 ? G3Function8003FC34(channel, state, frame) : G3Function8003FBB0(channel, state, frame);
        if (point != 0)
        {
            for (uint32 axis = 0; axis < 3; ++axis)
            {
                uint32 coordinate = r_u16(point + axis * 2);
                uint32 offset = r_u16(state + 0x1c + axis * 2);
                w_u16(frame + 0x10 + axis * 2, coordinate + offset);
            }
            if (G3Function80038D80(object + 0x58, frame + 0x10, 10000, frame) != 0)
                w_u32(state + 0x24, 1);
        }
        ticks = r_u32(state + 4);
        w_u32(state + 4, ticks + 1);
    }
    else if (mode == 1)
    {
        uint32 owner = r_u32(object + 0x1c);
        uint32 models = r_u32(owner + 0x0c);
        uint32 model = r_u32(models);
        uint32 channels = r_u32(model + 0x30);
        uint32 channel = r_u32(channels);
        uint32 point = G3Function8003FC34(channel, state, frame);
        if (point != 0)
        {
            for (uint32 axis = 0; axis < 3; ++axis)
            {
                uint32 coordinate = r_u16(point + axis * 2);
                uint32 offset = r_u16(state + 0x1c + axis * 2);
                w_u16(object + 0x58 + axis * 2, coordinate + offset);
            }
        }
    }
    else if (mode == 3)
    {
        uint32 random = G3SdkRandom80027FA8();
        uint32 coordinate = r_u16(object + 0x58);
        uint32 remainder = (uint32)((sint32)random % 64);
        w_u16(object + 0x58, coordinate + remainder);
        random = G3SdkRandom80027FA8();
        remainder = (uint32)((sint32)random % 64);
        coordinate = r_u16(object + 0x5a);
        uint32 height = r_u16(object + 0x5c);
        w_u16(object + 0x5a, coordinate + remainder);
        w_u16(object + 0x5c, height + 0x80);
    }
    else if (mode == 4)
    {
        uint32 flags = r_u32(object + 0x10);
        if ((flags & 0x800) == 0)
        {
            uint32 player = r_u32(game + 0x50);
            uint32 motion = r_u32(player + 0x154);
            if (motion == 0x100000)
            {
                uint32 heading = (uint32)r_s16(player + 0x64);
                uint32 angle = r_u32(state + 0x0c);
                if (((heading - angle) & 0xfff) < 0x80)
                    w_u32(object + 0x10, flags | 0x800);
            }
            uint32 model = r_u32(player + 0x30);
            uint32 angle = r_u32(state + 0x0c);
            uint32 anchor = model + 0x180;
            uint32 sine = G3Function8005B470(angle);
            uint32 radius = r_u32(state + 0x10);
            uint32 product = radius * sine;
            w_u16(frame + 0x38, (uint32)((sint32)product >> 12));
            angle = r_u32(state + 0x0c);
            uint32 cosine = G3Function8005B590(angle);
            radius = r_u32(state + 0x10);
            product = radius * cosine;
            w_u16(frame + 0x3c, 0);
            w_u16(frame + 0x3a, (uint32)((sint32)product >> 12));
            uint32 kind = r_u32(player + 0x150);
            if (kind == 1)
            {
                G3Function8002F718(frame + 0x18);
                angle = (uint32)r_s16(player + 0x60);
                G3Function8005B94C(angle, frame + 0x18, frame);
                angle = (uint32)r_s16(player + 0x62);
                G3Function8005BA5C(angle, frame + 0x18, frame);
                uint32 coefficient = (uint32)r_s16(frame + 0x18);
                uint32 x = (uint32)r_s16(frame + 0x38);
                uint32 product_00 = coefficient * x;
                uint32 y = (uint32)r_s16(frame + 0x3a);
                coefficient = (uint32)r_s16(frame + 0x1a);
                uint32 product_01 = coefficient * y;
                uint32 z = (uint32)r_s16(frame + 0x3c);
                coefficient = (uint32)r_s16(frame + 0x1c);
                uint32 product_02 = coefficient * z;
                coefficient = (uint32)r_s16(frame + 0x1e);
                uint32 product_10 = coefficient * x;
                coefficient = (uint32)r_s16(frame + 0x20);
                uint32 product_11 = coefficient * y;
                coefficient = (uint32)r_s16(frame + 0x22);
                uint32 product_12 = coefficient * z;
                coefficient = (uint32)r_s16(frame + 0x24);
                uint32 product_20 = coefficient * x;
                coefficient = (uint32)r_s16(frame + 0x26);
                uint32 product_21 = coefficient * y;
                coefficient = (uint32)r_s16(frame + 0x28);
                uint32 product_22 = coefficient * z;
                uint32 row = product_00 + product_01 + product_02;
                uint32 rotated = (uint32)((sint32)row >> 12);
                w_u16(frame + 0x40, rotated);
                w_u16(frame + 0x38, rotated);
                row = product_10 + product_11 + product_12;
                rotated = (uint32)((sint32)row >> 12);
                w_u16(frame + 0x42, rotated);
                w_u16(frame + 0x3a, rotated);
                row = product_20 + product_21 + product_22;
                rotated = (uint32)((sint32)row >> 12);
                w_u16(frame + 0x44, rotated);
                w_u16(frame + 0x3c, rotated);
            }
            for (uint32 axis = 0; axis < 3; ++axis)
            {
                uint32 origin = r_u16(anchor + 0x14 + axis * 4);
                uint32 offset = r_u16(frame + 0x38 + axis * 2);
                w_u16(object + 0x58 + axis * 2, origin + offset);
            }
            angle = (r_u32(state + 0x0c) + 0x80) & 0xfff;
            w_u32(state + 0x0c, angle);
            angle = r_u16(player + 0x60);
            w_u16(object + 0x60, angle);
            angle = r_u16(player + 0x62);
            w_u16(object + 0x62, angle);
            uint32 result = r_u16(state + 0x0c) + 0x800;
            w_u16(object + 0x64, result);
            return result;
        }
    }
    uint32 x = (uint32)r_s16(object + 0x58);
    uint32 y = (uint32)r_s16(object + 0x5a);
    uint32 previous_y = (uint32)r_s16(object + 0x72);
    uint32 previous_x = (uint32)r_s16(object + 0x70);
    uint32 result = G3Function8002F240(y - previous_y, x - previous_x, frame) + 0x400;
    w_u16(object + 0x64, result);
    return result;
}

uint32 G3Function80084ED8(uint32 object, uint32 resource, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80084ed8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 data = r_u32(object + 0x20);
    uint32 angle = (uint32)r_s16(data + 0x0c);
    uint32 cosine = G3Function8005B590(angle);
    uint32 offset = (uint32)((sint32)(cosine << 9) >> 12);
    uint32 x = r_u16(object + 0x58);
    data = r_u32(object + 0x20);
    w_u16(object + 0x58, x + offset);
    angle = (uint32)r_s16(data + 0x0c);
    uint32 sine = G3Function8005B470(angle);
    uint32 shifted = 0 - (sine << 9);
    offset = (uint32)((sint32)shifted >> 12);
    uint32 y = r_u16(object + 0x5a);
    uint32 z = r_u16(object + 0x5c);
    w_u16(object + 0x5c, z + 0x100);
    w_u16(object + 0x5a, y + offset);
    uint32 result = G3Function8002A454(object, resource, frame);
    data = r_u32(object + 0x20);
    angle = (uint32)r_s16(data + 0x0c);
    cosine = G3Function8005B590(angle);
    offset = (uint32)((sint32)(cosine << 9) >> 12);
    x = r_u16(object + 0x58);
    data = r_u32(object + 0x20);
    w_u16(object + 0x58, x - offset);
    angle = (uint32)r_s16(data + 0x0c);
    sine = G3Function8005B470(angle);
    shifted = 0 - (sine << 9);
    offset = (uint32)((sint32)shifted >> 12);
    y = r_u16(object + 0x5a);
    z = r_u16(object + 0x5c);
    w_u16(object + 0x5a, y - offset);
    w_u16(object + 0x5c, z - 0x100);
    return result;
}

uint32 G3Function80017680(uint32 effect, uint32 callback, uint32 storage, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80017680, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 position = r_u32(guest_stack + 0x18);
    uint32 face = r_u32(guest_stack + 0x10);
    uint32 value = r_u16(position);
    uint32 first = r_u16(face);
    uint32 second = r_u16(face + 2);
    uint32 third = r_u16(face + 4);
    w_u16(effect + 0x1c, value);
    value = r_u16(position + 2);
    w_u16(effect + 0x1e, value);
    value = r_u16(position + 4);
    w_u16(effect + 0x20, value);
    uint32 vertices = r_u32(guest_stack + 0x14);
    for (uint32 corner = 0; corner < 3; ++corner)
    {
        uint32 index = corner == 0 ? first : corner == 1 ? second : third;
        uint32 vertex = vertices + index * 8;
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            value = r_u16(vertex + axis * 2);
            w_u16(effect + 0x24 + corner * 8 + axis * 2, value);
        }
    }
    uint32 face_flags = r_u8(face + 7);
    uint32 rotation = r_u32(guest_stack + 0x1c);
    uint32 velocity = r_u32(guest_stack + 0x20);
    uint32 lifetime = r_u16(guest_stack + 0x28);
    uint32 color;
    if ((face_flags & 2) != 0)
    {
        uint32 flags = r_u32(effect + 0x0c);
        w_u32(effect + 0x0c, flags | 1);
        uint32 texture = r_u32(face + 8);
        w_u32(effect + 8, texture);
        color = (r_u32(texture + 0x0c) & 0x03ffffff) | 0x24000000;
    }
    else
    {
        uint32 flags = r_u32(effect + 0x0c);
        w_u32(effect + 0x0c, flags & 0xfffffffe);
        color = (r_u32(face + 8) & 0x03ffffff) | 0x20000000;
    }
    w_u32(effect + 0x18, color);
    w_u32(effect + 0x10, callback);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        value = r_u16(rotation + axis * 2);
        w_u16(effect + 0x4c + axis * 2, value);
    }
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        value = r_u16(velocity + axis * 2);
        w_u16(effect + 0x52 + axis * 2, value);
    }
    uint32 game = r_u32(0x80097048);
    uint32 transform = r_u32(game + 0x4c);
    G3Function80041CE4(storage + 0x0c, transform, effect + 0x1c, 0, frame);
    uint32 result = r_u16(effect + 0x20);
    value = r_u16(effect + 0x50);
    w_u16(effect + 0x22, lifetime);
    w_u16(effect + 0x48, 0);
    w_u32(effect + 0x14, storage);
    w_u16(effect + 0x44, result);
    w_u16(effect + 0x46, value);
    return result;
}

uint32 G3Function8001785C(uint32 effect, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8001785c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    sint32 position = r_s16(effect + 0x20);
    sint32 limit = r_s16(effect + 0x50);
    sint32 previous = r_s16(effect + 0x44);
    if (previous >= position + limit)
    {
        sint32 phase = r_s8(effect + 0x48);
        uint32 raw_phase = r_u8(effect + 0x48);
        if (phase == 0 || phase == 1 || phase == 2)
        {
            uint32 value = r_u16(effect + 0x46);
            w_u16(effect + 0x48, (uint32)(sint8)(raw_phase + 1));
            value -= phase == 2 ? 10 : 15;
            w_u16(effect + 0x50, value);
            w_u16(effect + 0x46, value);
        }
    }
    uint32 first = r_u16(effect + 0x24);
    uint32 second = r_u16(effect + 0x28);
    w_u16(effect + 0x24, first - 1);
    first = r_u16(effect + 0x2c);
    w_u16(effect + 0x28, second + 3);
    second = r_u16(effect + 0x30);
    w_u16(effect + 0x2c, first - 1);
    first = r_u16(effect + 0x34);
    w_u16(effect + 0x30, second - 1);
    second = r_u16(effect + 0x38);
    w_u16(effect + 0x34, first + 3);
    w_u16(effect + 0x38, second - 1);
    return G3Function80041DE0(effect, pool, frame);
}

uint32 G3Function80038B74(uint32 object, uint32 x, uint32 y, uint32 z, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80038b74, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    uint32 output = r_u32(guest_stack + 0x10);
    w_u16(frame + 0x38, x);
    w_u16(frame + 0x3a, y);
    w_u16(frame + 0x3c, z);
    uint32 matrix = r_u32(object + 0x30);
    if (matrix != 0)
    {
        for (uint32 group = 0; group < 2; ++group)
        {
            uint32 words[4];
            for (uint32 index = 0; index < 4; ++index)
                words[index] = r_u32(matrix + group * 16 + index * 4);
            for (uint32 index = 0; index < 4; ++index)
                w_u32(frame + 0x10 + group * 16 + index * 4, words[index]);
        }
    }
    else
        G3Function8005BD00(object + 0x60, frame + 0x10);
    x = (uint32)r_s16(frame + 0x38);
    uint32 coefficient = (uint32)r_s16(frame + 0x10);
    uint32 product_00 = x * coefficient;
    y = (uint32)r_s16(frame + 0x3a);
    coefficient = (uint32)r_s16(frame + 0x12);
    uint32 product_01 = y * coefficient;
    z = (uint32)r_s16(frame + 0x3c);
    coefficient = (uint32)r_s16(frame + 0x14);
    uint32 product_02 = z * coefficient;
    coefficient = (uint32)r_s16(frame + 0x16);
    uint32 product_10 = x * coefficient;
    coefficient = (uint32)r_s16(frame + 0x18);
    uint32 product_11 = y * coefficient;
    coefficient = (uint32)r_s16(frame + 0x1a);
    uint32 product_12 = z * coefficient;
    coefficient = (uint32)r_s16(frame + 0x1c);
    uint32 product_20 = x * coefficient;
    coefficient = (uint32)r_s16(frame + 0x1e);
    uint32 product_21 = y * coefficient;
    w_u32(frame + 0x2c, 0);
    w_u32(frame + 0x28, 0);
    w_u32(frame + 0x24, 0);
    coefficient = (uint32)r_s16(frame + 0x20);
    uint32 product_22 = z * coefficient;
    uint32 row_x = (uint32)((sint32)product_00 >> 12) + (uint32)((sint32)product_01 >> 12) + (uint32)((sint32)product_02 >> 12);
    w_u16(frame + 0x30, row_x);
    uint32 row_y = (uint32)((sint32)product_10 >> 12) + (uint32)((sint32)product_11 >> 12) + (uint32)((sint32)product_12 >> 12);
    w_u16(frame + 0x32, row_y);
    uint32 row_z = (uint32)((sint32)product_20 >> 12) + (uint32)((sint32)product_21 >> 12) + (uint32)((sint32)product_22 >> 12);
    w_u16(frame + 0x34, row_z);
    uint32 position = r_u16(object + 0x58);
    w_u16(output, row_x + position);
    row_y = r_u16(frame + 0x32);
    position = r_u16(object + 0x5a);
    w_u16(output + 2, row_y + position);
    row_z = r_u16(frame + 0x34);
    position = r_u16(object + 0x5c);
    uint32 result = row_z + position;
    w_u16(output + 4, result);
    return result;
}

uint32 G3Function80017958(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80017958, "SLUS_008.06");
    uint32 frame = guest_stack - 0x78;
    uint32 resource = r_u32(command + 0x0c);
    uint32 models = r_u32(resource + 0x0c);
    uint32 face = 0;
    w_u32(frame + 0x48, 0);
    uint32 model = r_u32(models);
    uint32 quantity = r_u8(command + 4);
    sint32 kind = r_s16(model + 4);
    w_u16(frame + 0x40, quantity);
    if (kind == 1)
        face = r_u32(model + 0x14);
    if (r_u32(object + 0x60) != 0)
        w_u32(frame + 0x48, 1);
    else
    {
        uint32 pose = r_u32(object + 0x20);
        if (pose != 0 && r_u32(pose + 8) != 0)
            w_u32(frame + 0x48, 1);
    }
    uint32 radius = r_u8(command + 5);
    if (radius == 0)
        radius = 60;
    uint32 lifetime = r_u8(command + 9);
    if (lifetime == 0)
        lifetime = 25;
    quantity = r_u16(frame + 0x40);
    uint32 index = 0;
    if (quantity == 0)
        return 4;
    do
    {
        uint32 random = G3SdkRandom80027FA8();
        uint32 angle = (uint32)(sint16)((sint32)random % 4096);
        uint32 sample = G3SdkRandom80027FA8();
        uint32 sine = G3Function8005B470(angle);
        uint32 remainder = (uint32)((sint32)sample % (sint32)radius);
        uint32 product = remainder * sine;
        w_u16(frame + 0x28, (uint32)((sint32)product >> 12));
        sample = G3SdkRandom80027FA8();
        uint32 cosine = G3Function8005B590(angle);
        remainder = (uint32)((sint32)sample % (sint32)radius);
        product = remainder * cosine;
        w_u16(frame + 0x2a, (uint32)((sint32)product >> 12));
        sample = G3SdkRandom80027FA8();
        remainder = (uint32)((sint32)sample % (sint32)radius);
        uint32 rotated = r_u32(frame + 0x48);
        uint32 height = remainder + 25;
        w_u16(frame + 0x2c, height);
        if (rotated != 0)
        {
            uint32 x = (uint32)r_s16(frame + 0x28);
            uint32 y = (uint32)r_s16(frame + 0x2a);
            w_u32(frame + 0x10, frame + 0x38);
            G3Function80038B74(object, x, y, height, frame);
            uint32 output_x = r_u16(frame + 0x38);
            uint32 position_x = r_u16(object + 0x58);
            uint32 output_y = r_u16(frame + 0x3a);
            uint32 delta_x = output_x - position_x;
            w_u16(frame + 0x38, delta_x);
            uint32 position_y = r_u16(object + 0x5a);
            uint32 delta_y = output_y - position_y;
            uint32 output_z = r_u16(frame + 0x3c);
            w_u16(frame + 0x3a, delta_y);
            uint32 position_z = r_u16(object + 0x5c);
            w_u16(frame + 0x28, delta_x);
            w_u16(frame + 0x2a, delta_y);
            uint32 delta_z = output_z - position_z;
            w_u16(frame + 0x3c, delta_z);
            w_u16(frame + 0x2c, delta_z);
        }
        w_u16(frame + 0x32, 0xffff);
        w_u16(frame + 0x30, 0);
        w_u16(frame + 0x34, 0xfffb);
        kind = r_s16(model + 4);
        if (kind >= 2)
        {
            random = G3SdkRandom80027FA8();
            sint32 count = r_s16(model + 4);
            remainder = count == 0 ? random : (uint32)((sint64)(sint32)random % (sint64)count);
            uint32 selected = (uint32)(sint16)remainder;
            uint32 faces = r_u32(model + 0x14);
            face = faces + selected * 12;
        }
        w_u32(frame + 0x10, frame + 0x30);
        w_u32(frame + 0x18, 0x80017680);
        uint32 pool = r_u32(0x800a27fc);
        w_u32(frame + 0x1c, 0x8001785c);
        w_u32(frame + 0x20, lifetime);
        w_u32(frame + 0x14, pool);
        uint32 effect = G3Function8004257C(model, face, object + 0x58, frame + 0x28, frame);
        if (effect != 0)
        {
            if ((r_u8(command + 8) & 8) != 0)
                G3Function80045920(effect, frame);
            uint32 table = r_u32(model + 0x1c);
            if (table != 0)
            {
                uint32 entry = table + 4;
                sint32 count = r_s32(table);
                if (count > 0)
                {
                    uint32 material = r_u32(effect + 8);
                    for (sint32 current = 0; current < count; ++current, entry += 12)
                    {
                        uint32 candidate = r_u32(entry);
                        if (candidate == material)
                        {
                            uint32 flags = r_u32(effect + 0x0c);
                            uint32 texture = r_u32(effect + 8);
                            w_u32(effect + 0x0c, flags | 4);
                            uint32 first = r_u32(texture);
                            uint32 second = r_u32(texture + 4);
                            uint32 third = r_u32(texture + 8);
                            uint32 fourth = r_u32(texture + 0x0c);
                            w_u32(effect + 0x60, first);
                            w_u32(effect + 0x64, second);
                            w_u32(effect + 0x68, third);
                            w_u32(effect + 0x6c, fourth);
                            w_u32(effect + 0x70, entry);
                            random = G3SdkRandom80027FA8();
                            w_u16(effect + 0x74, random);
                            break;
                        }
                    }
                }
            }
            uint32 flags = r_u8(command + 8);
            if ((flags & 1) != 0)
                w_u16(effect + 0x48, 0xffff);
            else if ((flags & 2) != 0)
            {
                uint32 value = r_u8(command + 5);
                w_u16(effect + 0x44, value);
            }
        }
        ++index;
        quantity = r_u16(frame + 0x40);
    } while ((sint32)(sint16)index < (sint32)quantity);
    return 4;
}

uint32 G3Function80017D00(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80017d00, "SLUS_008.06");
    uint32 mode = r_u32(command + 4);
    uint32 object = r_u32(command + 8);
    if (mode == 0 || mode == 1)
    {
        uint32 game = r_u32(0x80097048);
        uint32 target = object != 0 ? r_u32(object + 0x24) : 0;
        uint32 camera = r_u32(game + 0x4c);
        if (mode == 0)
            G3Function8004D860(camera, target);
        else
            G3Function8004D870(camera, target);
    }
    return 3;
}

uint32 G3Function80017DC0(void)
{
    FUNCTION_MARKER(0x80017dc0, "SLUS_008.06");
    return 2;
}

uint32 G3Function80017DC8(void)
{
    FUNCTION_MARKER(0x80017dc8, "SLUS_008.06");
    return 2;
}

uint32 G3Function80017DD0(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80017dd0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 key = r_u32(command + 4);
    uint32 divisor = r_u32(command + 8);
    G3Function800358C0(key, divisor, frame);
    return 3;
}

uint32 G3Function80017DF8(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80017df8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 key = r_u32(command + 4);
    uint32 enabled = r_u32(command + 8);
    G3Function800359EC(key, enabled, frame);
    return 3;
}

uint32 G3Function80017E20(void)
{
    FUNCTION_MARKER(0x80017e20, "SLUS_008.06");
    G3Function800356E4();
    return 1;
}

uint32 G3Function80017E40(void)
{
    FUNCTION_MARKER(0x80017e40, "SLUS_008.06");
    G3Function80035A80();
    return 1;
}

uint32 G3Function80017E60(void)
{
    FUNCTION_MARKER(0x80017e60, "SLUS_008.06");
    G3Function80035AA4();
    return 1;
}

uint32 G3Function80017E80(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80017e80, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 target = r_u32(command + 4);
    if (target == 0)
        return 2;
    uint32 flags = r_u32(target + 0x18);
    if ((flags & 0x18) != 8)
        return 2;
    uint32 sequence = r_u32(command + 8);
    if (sequence == 0)
        return 2;
    G3Function80018DE0(object, sequence, frame);
    return 0;
}

uint32 G3Function80017FA4(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80017fa4, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 words[2];
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 source = command + 4 + index * 4;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 low_lane = source & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
        words[index] = (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 destination = game + 0x69e4 + index * 4;
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        uint32 low_lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15 << low_lane) & 15);
    }
    return 3;
}

uint32 G3Function80017FD4(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80017fd4, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    uint32 value = r_u32(command + 4);
    w_u32(level + 0x50, value);
    return 2;
}

uint32 G3Function8001801C(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x8001801c, "SLUS_008.06");
    uint32 target = r_u32(command + 4);
    uint32 value = r_u32(command + 8);
    w_u32(target + 4, value);
    return 3;
}

uint32 G3Function80018034(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80018034, "SLUS_008.06");
    for (uint32 component = 0; component < 3; ++component)
    {
        uint32 game = r_u32(0x80097048);
        uint32 value = r_u8(command + 4 + component);
        w_u8(game + 0xec + component, value);
    }
    return 2;
}

uint32 G3Function80018078(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80018078, "SLUS_008.06");
    for (uint32 component = 0; component < 3; ++component)
    {
        uint32 game = r_u32(0x80097048);
        uint32 level = r_u32(game + 0x48);
        uint32 value = r_u8(command + 4 + component);
        w_u8(level + 0x40 + component, value);
    }
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    uint32 red = r_u8(level + 0x40);
    uint32 green = r_u8(level + 0x41);
    uint32 blue = r_u8(level + 0x42);
    SetFarColor((sint32)red, (sint32)green, (sint32)blue);
    return 2;
}

uint32 G3Function80018108(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80018108, "SLUS_008.06");
    for (uint32 component = 0; component < 3; ++component)
    {
        uint32 game = r_u32(0x80097048);
        uint32 level = r_u32(game + 0x48);
        uint32 value = r_u8(command + 4 + component);
        w_u8(level + 0x44 + component, value);
    }
    uint32 game = r_u32(0x80097048);
    for (uint32 component = 0; component < 3; ++component)
    {
        uint32 level = r_u32(game + 0x48);
        uint32 value = r_u8(level + 0x44 + component);
        w_u8(0x800a3d98 + component, value);
    }
    return 2;
}

uint32 G3Function800181AC(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x800181ac, "SLUS_008.06");
    for (uint32 component = 0; component < 3; ++component)
    {
        uint32 game = r_u32(0x80097048);
        uint32 level = r_u32(game + 0x48);
        uint32 value = r_u8(command + 4 + component);
        w_u8(level + 0x48 + component, value);
    }
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    uint32 red = r_u8(level + 0x48);
    uint32 green = r_u8(level + 0x49);
    uint32 blue = r_u8(level + 0x4a);
    SetFarColor((sint32)red, (sint32)green, (sint32)blue);
    return 2;
}

uint32 G3Function8001823C(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x8001823c, "SLUS_008.06");
    for (uint32 component = 0; component < 3; ++component)
    {
        uint32 game = r_u32(0x80097048);
        uint32 value = r_u8(command + 4 + component);
        w_u8(game + 0x69c4 + component, value);
    }
    return 2;
}

uint32 G3Function8001829C(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8001829c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    return G3Function80015BA8(object, command, frame);
}

uint32 G3Function800182BC(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800182bc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 index = r_u8(command + 4);
    uint32 value = r_u32(0x800a0760 + (index << 2));
    if (value == 0)
        return 3;
    uint32 sequence = r_u32(command + 8);
    if (sequence == 0)
        return 3;
    G3Function80018DE0(object, sequence, frame);
    return 0;
}

uint32 G3Function80018314(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80018314, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 index = r_u8(command + 4);
    uint32 value = r_u32(0x800a0760 + (index << 2));
    if (value != 0)
        return 3;
    uint32 sequence = r_u32(command + 8);
    if (sequence == 0)
        return 3;
    G3Function80018DE0(object, sequence, frame);
    return 0;
}

uint32 G3Function8001836C(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x8001836c, "SLUS_008.06");
    for (uint32 slot_index = 0; slot_index < 8; ++slot_index)
    {
        uint32 slot = 0x800a0860 + slot_index * 16;
        if (r_u32(slot + 8) != 0)
            continue;
        uint32 value = r_u16(command + 0xa);
        w_u16(slot + 6, 0);
        w_u16(slot + 4, value);
        uint32 index = (uint32)r_s16(command + 0xa);
        uint32 initial = (uint32)r_s16(command + 8);
        w_u32(0x800a0760 + (index << 2), initial);
        value = r_u16(command + 4);
        w_u16(slot, value);
        value = r_u16(command + 6);
        w_u16(slot + 2, value);
        value = r_u32(command + 0xc);
        w_u32(slot + 8, value);
        return 4;
    }
    return 4;
}

uint32 G3Function800183E8(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x800183e8, "SLUS_008.06");
    for (uint32 slot_index = 0; slot_index < 8; ++slot_index)
    {
        uint32 slot = 0x800a0860 + slot_index * 16;
        if (r_u32(slot + 8) != 0)
            continue;
        uint32 value = r_u16(command + 0xa);
        w_u16(slot + 6, 1);
        w_u16(slot + 4, value);
        uint32 index = (uint32)r_s16(command + 0xa);
        uint32 initial = (uint32)r_s16(command + 8);
        w_u32(0x800a0760 + (index << 2), initial);
        value = r_u16(command + 4);
        w_u16(slot, value);
        value = r_u16(command + 6);
        w_u16(slot + 2, value);
        value = r_u32(command + 0xc);
        w_u32(slot + 8, value);
        value = r_u32(command + 0x10);
        w_u32(slot + 0xc, value);
        return 5;
    }
    return 5;
}

uint32 G3Function80018474(void)
{
    FUNCTION_MARKER(0x80018474, "SLUS_008.06");
    return 1;
}

uint32 G3Function8001847C(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x8001847c, "SLUS_008.06");
    uint32 target = r_u32(command + 4);
    if (target != 0)
    {
        uint32 flags = r_u32(target + 0x18);
        w_u32(target + 0x18, flags | 0x2000);
        target = r_u32(command + 4);
        uint32 child = r_u32(target + 0x20);
        if (child != 0)
        {
            flags = r_u32(child + 0x10);
            w_u32(child + 0x10, flags | 0x400);
        }
    }
    return 2;
}

uint32 G3Function800184CC(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x800184cc, "SLUS_008.06");
    uint32 target = r_u32(command + 4);
    if (target != 0)
    {
        uint32 flags = r_u32(target + 0x18);
        uint32 child = r_u32(target + 0x20);
        w_u32(target + 0x18, flags & 0xffffdfff);
        if (child != 0)
        {
            flags = r_u32(child + 0x10);
            w_u32(child + 0x10, flags & 0xfffffbff);
        }
    }
    return 2;
}

uint32 G3Function80018608(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80018608, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 player = r_u32(0x8009efc4);
    uint32 value = (uint32)r_s16(command + 4);
    uint32 animation = (uint32)r_s16(command + 6);
    G3Function8007B108(player, object, value, animation, frame);
    return 2;
}

uint32 G3Function8001863C(uint32 object, uint32 command)
{
    FUNCTION_MARKER(0x8001863c, "SLUS_008.06");
    if (r_s16(command + 6) != 0)
        object = r_u32(0x8009efc4);
    uint32 enabled = (uint32)r_s16(command + 4);
    uint32 flags = r_u32(object + 0x10);
    if (enabled != 0)
        flags &= 0xfffff7ff;
    else
        flags |= 0x800;
    w_u32(object + 0x10, flags);
    return 2;
}

uint32 G3Function80018688(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80018688, "SLUS_008.06");
    uint32 enabled = r_u32(command + 4);
    uint32 game = r_u32(0x80097048);
    uint32 flags = r_u32(game + 0x6834);
    if (enabled != 0)
        flags |= 0x80000;
    else
        flags &= 0xfff7ffff;
    w_u32(game + 0x6834, flags);
    return 2;
}

uint32 G3Function800186DC(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800186dc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 duration = r_u16(command + 8);
    uint32 first = r_u16(command + 0xa);
    uint32 second = r_u16(command + 0xc);
    uint32 resource = r_u32(command + 4);
    uint32 options = r_u16(command + 0xe);
    if (resource != 0)
    {
        uint32 created = G3Function8002A454(object, resource, frame);
        uint32 property = (uint32)r_s16(object + 0xce);
        w_u32(frame + 0x10, (uint32)(sint32)(sint16)first);
        w_u32(frame + 0x14, (uint32)(sint32)(sint16)second);
        w_u32(frame + 0x18, (uint32)(sint32)(sint16)options);
        G3Function8006B90C(object, created, property, (uint32)(sint32)(sint16)duration, frame);
    }
    return 4;
}

uint32 G3Function8001877C(uint32 object, uint32 command)
{
    FUNCTION_MARKER(0x8001877c, "SLUS_008.06");
    uint32 enabled = r_u32(command + 4);
    uint32 flags = r_u32(object + 0x18);
    if (enabled != 0)
        flags |= 0x10;
    else
        flags &= 0xffffffef;
    w_u32(object + 0x18, flags);
    return 2;
}

uint32 G3Function800187B0(void)
{
    FUNCTION_MARKER(0x800187b0, "SLUS_008.06");
    uint32 player = r_u32(0x8009efc4);
    uint32 flags = r_u32(player + 0x134);
    if ((flags & 0x40) != 0)
        w_u32(player + 0x134, flags | 0x80);
    return 1;
}

uint32 G3Function800187DC(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800187dc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 placement = r_u32(command + 4);
    uint32 duration = r_u16(command + 8);
    uint32 first = r_u16(command + 0xa);
    uint32 second = r_u16(command + 0xc);
    uint32 options = r_u16(command + 0xe);
    uint32 target = G3Function80029938(placement, frame);
    if (target != 0)
    {
        uint32 property = (uint32)r_s16(object + 0xce);
        w_u32(frame + 0x10, (uint32)(sint32)(sint16)first);
        w_u32(frame + 0x14, (uint32)(sint32)(sint16)second);
        w_u32(frame + 0x18, (uint32)(sint32)(sint16)options);
        G3Function8006B90C(object, target, property, (uint32)(sint32)(sint16)duration, frame);
    }
    return 4;
}

uint32 G3Function8001893C(void)
{
    FUNCTION_MARKER(0x8001893c, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 player = r_u32(game + 0x50);
    uint32 state = r_u32(player + 0x150);
    if (state - 2 < 2)
    {
        uint32 flags = r_u32(player + 0x134);
        w_u32(player + 0x134, flags | 0x100);
    }
    return 1;
}

uint32 G3Function800189D4(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800189d4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    sint32 scaled = r_s16(command + 0xa) * 4096;
    uint32 kind = (uint32)r_s16(command + 4);
    uint32 mode = (uint32)r_s16(command + 8);
    uint32 index = (uint32)r_s16(command + 6);
    uint32 scale = (uint32)r_s16(command + 0xc);
    w_u32(frame + 0x10, scale);
    uint32 enabled = r_s16(command + 0xe) != 0;
    w_u32(frame + 0x14, enabled);
    uint32 strength = (uint32)(sint32)(sint16)(scaled / 1000);
    G3Function8008194C(kind, mode, index, strength, frame);
    return 4;
}

uint32 G3Function80018A40(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80018a40, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    sint32 kind = r_s16(command + 4);
    if (kind < 0)
        G3Function800810FC();
    else
    {
        uint32 index = (uint32)r_s16(command + 6);
        G3Function80081B7C((uint32)kind, index, frame);
    }
    return 2;
}

uint32 G3Function80018A84(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80018a84, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    sint32 scaled = r_s16(command + 0xa) * 4096;
    uint32 kind = (uint32)r_s16(command + 4);
    uint32 index = (uint32)r_s16(command + 6);
    uint32 strength = (uint32)(sint32)(sint16)(scaled / 1000);
    G3Function80081BD4(kind, index, strength, frame);
    return 3;
}

uint32 G3Function80018AD4(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80018ad4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 game = r_u32(0x80097048);
    uint32 player = r_u32(game + 0x50);
    uint32 data = r_u32(player + 0x24);
    uint32 linked = r_u32(data + 0x174);
    if (linked != 0)
        G3Function80081E08(linked, game, frame);
    return 1;
}

uint32 G3Function80018B1C(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80018b1c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 placement = r_u32(command + 4);
    if (placement != 0)
    {
        uint32 flags = r_u32(placement + 0x18);
        w_u32(placement + 0x18, flags & 0xffffff7f);
        sint32 duration = r_s32(command + 8);
        if (duration >= 256)
            duration = 255;
        uint32 object = G3Function80029938(placement, frame);
        if (object != 0)
            G3Function80039F1C(object, (uint32)duration, 1);
    }
    return 3;
}

uint32 G3Function80081B7C(uint32 kind, uint32 index, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80081b7c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if ((kind & 0xffff) >= 2)
        return 0;
    uint32 slot = G3Function80081754((uint32)(sint32)(sint16)kind, (uint32)(sint32)(sint16)index, 0, frame);
    if (slot == 0)
        return 0;
    w_u16(slot + 6, 0);
    w_u32(0x800a2c3c, slot);
    return 1;
}

uint32 G3Function80081BD4(uint32 kind, uint32 index, uint32 strength, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80081bd4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if ((kind & 0xffff) >= 2)
        return 0;
    uint32 slot = G3Function80081754((uint32)(sint32)(sint16)kind, (uint32)(sint32)(sint16)index, 0, frame);
    if (slot == 0)
        return 0;
    sint32 signed_strength = (sint16)strength;
    uint32 value = signed_strength < 0 ? 0 : signed_strength >= 0x1000 ? 0x1000 : strength;
    w_u16(slot + 8, value);
    w_u32(0x800a2c3c, slot);
    return 1;
}

uint32 G3Function80039F1C(uint32 object, uint32 duration, uint32 direction)
{
    FUNCTION_MARKER(0x80039f1c, "SLUS_008.06");
    uint32 flags = r_u32(object + 0x18);
    w_u32(object + 0x18, flags | 0x1000);
    w_u8(object + 0x90, direction == 0 ? 0xff : 0);
    sint32 divisor = (sint32)duration;
    sint32 quotient = divisor == 0 ? -1 : 256 / divisor;
    w_u8(object + 0x91, direction);
    w_u8(object + 0x93, duration);
    w_u8(object + 0x92, (uint32)quotient);
    return (uint32)quotient;
}

uint32 G3Function80018B90(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80018b90, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 placement = r_u32(command + 4);
    if (placement != 0)
    {
        sint32 duration = r_s32(command + 8);
        if (duration >= 256)
            duration = 255;
        uint32 object = r_u32(placement + 0x20);
        if (object != 0)
            G3Function80039F1C(object, (uint32)duration, 0);
        else
        {
            uint32 flags = r_u32(placement + 0x18);
            w_u32(placement + 0x18, flags | 0x80);
        }
    }
    return 3;
}

uint32 G3Function80018C00(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80018c00, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 count = r_u32(command + 4);
    uint32 resource = G3Function8003AA48(0x8009d3ac);
    if (resource != 0)
    {
        while (count != 0)
        {
            G3Function8002A454(object, resource, frame);
            --count;
        }
    }
    return 2;
}

uint32 G3Function80018C7C(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80018c7c, "SLUS_008.06");
    uint32 first = r_u32(command + 4);
    uint32 second = r_u32(command + 8);
    G3Function80036144(first, second);
    return 3;
}

uint32 G3Function80018CA4(void)
{
    FUNCTION_MARKER(0x80018ca4, "SLUS_008.06");
    G3Function80036158();
    return 1;
}

uint32 G3Function80018CC4(uint32 context, uint32 enabled, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80018cc4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 game = r_u32(0x80097048);
    uint32 count = r_u32(game + 0x6818) + 1;
    w_u32(game + 0x6818, count);
    if (count == 100)
        w_u32(game + 0x6818, 99);
    game = r_u32(0x80097048);
    uint32 resource = r_u32(0x80098814);
    uint32 player = r_u32(game + 0x50);
    G3Function8002A454(player, resource, frame);
    return 1;
}

uint32 G3Function80036144(uint32 first, uint32 second)
{
    FUNCTION_MARKER(0x80036144, "SLUS_008.06");
    w_u16(0x800ad0c0, first);
    w_u16(0x800ad0c2, second);
    return 0x800aa7d0;
}

uint32 G3Function80036158(void)
{
    FUNCTION_MARKER(0x80036158, "SLUS_008.06");
    w_u16(0x800ad0c0, 0xffff);
    return 0xffffffff;
}

uint32 G3Function8006B90C(uint32 source, uint32 target, uint32 property, uint32 duration, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006b90c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 first = r_u32(guest_stack + 0x10);
    uint32 second = r_u32(guest_stack + 0x14);
    w_u32(frame + 0x10, property);
    w_u16(frame + 0x14, duration);
    uint32 resource = G3Function8003AA48(0x800a00c0);
    uint32 options = r_u32(guest_stack + 0x18);
    if ((options & 1) != 0)
    {
        uint32 previous = source;
        source = target;
        target = previous;
    }
    if (resource == 0 || source == 0 || target == 0)
        return 0;
    if ((sint16)duration <= 0)
        w_u16(frame + 0x14, 1);
    if ((sint16)first <= 0)
        first = 1;
    if ((sint16)second <= 0)
        second = 1;
    uint32 created = G3Function8002A454(source, resource, frame);
    if (created == 0)
        return 0;
    uint32 quotient = 0x1000 / (sint32)(sint16)first;
    uint32 data = created + 0x104;
    w_u32(data, source);
    w_u32(data + 4, target);
    uint32 value = r_u16(frame + 0x10);
    w_u16(data + 0xa, 0x1000);
    w_u16(data + 8, value);
    value = r_u16(frame + 0x14);
    w_u16(data + 0xc, value);
    w_u16(data + 0xe, first);
    w_u16(data + 0x10, second);
    w_u16(data + 0x14, 0);
    w_u16(data + 0x16, 0);
    w_u16(data + 0x12, quotient);
    value = G3Function80037094(source);
    w_u32(data + 0x18, value);
    value = G3Function80037094(target);
    w_u32(data + 0x1c, value);
    G3Function800370E8(source, 0x1f000, frame);
    G3Function800370E8(target, 0x1f000, frame);
    if ((r_u32(source + 0x10) & 0x400) != 0)
    {
        value = r_u16(data + 0x14);
        w_u16(data + 0x14, value | 2);
    }
    if ((r_u32(target + 0x10) & 0x400) != 0)
    {
        value = r_u16(data + 0x16);
        w_u16(data + 0x16, value | 2);
    }
    if ((r_u32(source + 0x10) & 0x800) != 0)
    {
        value = r_u16(data + 0x14);
        w_u16(data + 0x14, value | 1);
    }
    if ((r_u32(target + 0x10) & 0x800) != 0)
    {
        value = r_u16(data + 0x16);
        w_u16(data + 0x16, value | 1);
    }
    options = r_u32(guest_stack + 0x18);
    value = (uint32)r_s16(data + 0x14);
    if ((options & 2) != 0)
        value |= 4;
    w_u16(data + 0x14, value);
    options = r_u32(guest_stack + 0x18);
    value = r_u16(data + 0x16);
    w_u16(data + 0x16, value | (options & 4));
    uint32 source_flags = r_u16(data + 0x14);
    uint32 target_flags = r_u16(data + 0x16);
    source_flags |= options & 8;
    target_flags |= options & 8;
    w_u16(data + 0x14, source_flags);
    w_u16(data + 0x16, target_flags);
    source_flags |= options & 0x10;
    target_flags |= options & 0x10;
    w_u16(data + 0x14, source_flags);
    w_u16(data + 0x16, target_flags);
    uint32 flags = r_u32(created + 0x18);
    uint32 state = r_u32(created + 0x10);
    w_u32(created + 0x18, flags | 8);
    w_u32(created + 0x10, state | 0x400);
    flags = r_u32(source + 0x10);
    w_u32(source + 0x10, flags | 0x400);
    flags = r_u32(target + 0x10);
    w_u32(target + 0x10, flags | 0x400);
    flags = r_u32(created + 0x10);
    w_u32(created + 0x10, flags | 0x800);
    flags = r_u32(source + 0x10);
    w_u32(source + 0x10, flags | 0x800);
    flags = r_u32(target + 0x10);
    w_u32(target + 0x10, flags | 0x800);
    G3Function8006BD2C(created);
    return created;
}

uint32 G3Function8006BD2C(uint32 effect)
{
    FUNCTION_MARKER(0x8006bd2c, "SLUS_008.06");
    uint32 flags = r_u16(effect + 0x118);
    uint32 target = r_u32(effect + 0x108);
    uint32 player = r_u32(0x8009efc4);
    uint32 source = r_u32(effect + 0x104);
    uint32 pose = target == player ? target : source;
    uint32 peer = target == player ? source : target;
    for (uint32 pass = (flags & 8) != 0 ? 1 : 0; pass < 2; ++pass)
    {
        uint32 destination = pass == 0 ? peer : effect;
        uint32 position = pose + 0x58;
        uint32 high_address = position + 3;
        uint32 high_lane = high_address & 3;
        uint32 low_lane = position & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(position & 0xfffffffc, (15 << low_lane) & 15);
        uint32 word = (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        uint32 height = (uint32)r_s16(pose + 0x5c);
        position = destination + 0x58;
        high_address = position + 3;
        high_lane = high_address & 3;
        low_lane = position & 3;
        w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        w_masked_u32(position & 0xfffffffc, word << (low_lane * 8), (15 << low_lane) & 15);
        w_u16(destination + 0x5c, height);
        uint32 words[2];
        for (uint32 index = 0; index < 2; ++index)
        {
            position = pose + 0x60 + index * 4;
            high_address = position + 3;
            high_lane = high_address & 3;
            low_lane = position & 3;
            high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            low = r_masked_u32(position & 0xfffffffc, (15 << low_lane) & 15);
            words[index] = (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 index = 0; index < 2; ++index)
        {
            position = destination + 0x60 + index * 4;
            high_address = position + 3;
            high_lane = high_address & 3;
            low_lane = position & 3;
            w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            w_masked_u32(position & 0xfffffffc, words[index] << (low_lane * 8), (15 << low_lane) & 15);
        }
    }
    return player;
}
