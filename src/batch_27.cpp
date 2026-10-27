#include "xport.h"
#include "psx.h"
#include "game_functions.h"

uint32 G3Function800812A0(uint32 guest_stack)
{
    FUNCTION_MARKER(0x800812a0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x58;
    for (uint32 controller = 0; controller < 2; ++controller)
    {
        uint32 port = r_u8(0x800a0340 + controller);
        w_u16(frame + 0x18, port);
        uint32 supported = (uint32)PadInfoMode((sint32)port, 2, 0);
        uint32 force = 0;
        uint32 active = 0x800a2a94 + controller * 4;
        if (supported != 0)
        {
            w_u32(active, 1);
            uint32 state = (uint32)PadGetState((sint32)port);
            uint32 motor = 0;
            uint32 total = 0;
            w_u16(frame + 0x20, state);
            for (uint32 slot = 0; slot < 4; ++slot)
            {
                uint32 effect = 0x800a2a9c + controller * 0xd0 + slot * 0x34;
                if ((r_u16(effect + 6) & 5) != 1)
                    continue;
                uint32 timer = r_u16(effect + 0xc) - 1;
                for (;;)
                {
                    w_u16(effect + 0xc, timer);
                    if ((sint32)(timer << 16) >= 0)
                        break;
                    uint32 command = r_u32(effect + 0x18);
                    uint32 opcode = r_u8(command);
                    switch (opcode)
                    {
                    case 0:
                        w_u16(effect, 1);
                        break;
                    case 1:
                        w_u16(effect, 0);
                        break;
                    case 2:
                    {
                        uint32 flags = r_u16(effect + 6);
                        w_u16(effect + 6, flags & 0xfff7);
                        uint32 amplitude = r_u8(command + 1) << 6;
                        w_u16(effect + 2, amplitude);
                        break;
                    }
                    case 3:
                    {
                        sint32 duration = r_s16(command + 4);
                        sint32 speed = r_s16(effect + 0xa);
                        sint32 product = duration * speed;
                        uint32 target = r_u8(command + 1) << 6;
                        uint32 scaled = (uint32)(product >> 12);
                        sint32 steps = (sint32)(scaled << 16) >> 16;
                        if (steps > 0)
                        {
                            uint32 current = r_u16(effect + 2);
                            uint32 difference = target - current;
                            sint32 delta = (sint32)(difference << 16) >> 16;
                            uint32 increment = (uint32)(delta / steps);
                            w_u16(effect + 0xe, target);
                            w_u16(effect + 0x12, scaled);
                            w_u16(effect + 0x10, increment);
                        }
                        else
                            w_u16(effect + 2, target);
                        uint32 flags = r_u16(effect + 6);
                        w_u16(effect + 6, flags | 8);
                        break;
                    }
                    case 4:
                        force = 1;
                        break;
                    default:
                        break;
                    }
                    uint32 command_end = r_u32(effect + 0x1c);
                    command += 6;
                    if (command >= command_end)
                    {
                        if ((r_u16(effect + 6) & 2) == 0)
                        {
                            w_u16(effect + 6, 0);
                            break;
                        }
                        command = r_u32(effect + 0x14);
                    }
                    sint32 duration = r_s16(command + 2);
                    sint32 speed = r_s16(effect + 0xa);
                    sint32 product = duration * speed;
                    w_u32(effect + 0x18, command);
                    timer = (uint32)(product >> 12);
                    w_u16(effect + 0xc, timer);
                    --timer;
                }
                if ((r_u16(effect + 6) & 8) != 0)
                {
                    uint32 remaining = r_u16(effect + 0x12) - 1;
                    w_u16(effect + 0x12, remaining);
                    if ((sint32)(remaining << 16) <= 0)
                    {
                        uint32 flags = r_u16(effect + 6);
                        uint32 target = r_u16(effect + 0xe);
                        w_u16(effect + 6, flags & 0xfff7);
                        w_u16(effect + 2, target);
                    }
                    else
                    {
                        uint32 current = r_u16(effect + 2);
                        uint32 increment = r_u16(effect + 0x10);
                        w_u16(effect + 2, current + increment);
                    }
                }
                uint32 amplitude;
                if ((r_u16(effect + 6) & 0x10) != 0)
                {
                    uint32 value = (uint32)r_s16(effect + 8);
                    uint32 outer = (uint32)r_s16(effect + 0x2e);
                    uint32 inner = (uint32)r_s16(effect + 0x30);
                    w_u32(frame + 0x10, value);
                    uint32 origin = r_u32(effect + 0x24);
                    uint32 position = r_u32(effect + 0x20);
                    w_u32(frame + 0x28, motor);
                    amplitude = G3Function80081194(origin, position, outer, inner, frame);
                    motor = r_u32(frame + 0x28);
                }
                else
                    amplitude = r_u16(effect + 8);
                sint32 enabled = r_s16(effect);
                if (enabled != 0 && (sint32)(amplitude << 16) > 0)
                    motor = 1;
                sint32 strength = r_s16(effect + 2);
                sint32 scale = (sint32)(amplitude << 16) >> 16;
                total += (uint32)((strength * scale) >> 12);
                if ((total & 0xffff) >= 0x3fc1)
                    total = 0x3fc0;
            }
            if (force != 0)
                total = 0x3fc0;
            uint32 output = 0x800a2a8c + controller * 4;
            if (r_u32(0x800a033c) != 0)
            {
                w_u8(output, motor);
                w_u8(output + 1, total >> 6);
            }
            else
            {
                w_u8(output + 1, 0);
                w_u8(output, 0);
            }
            sint32 state_code = (sint32)(r_u16(frame + 0x20) << 16) >> 16;
            if (state_code == 1)
                w_u8(output + 2, 0);
            if (r_u8(output + 2) == 0)
            {
                uint32 saved_port = r_u16(frame + 0x18);
                PadSetAct((sint32)saved_port, (uint8 *)psx_addr(output, 2), 2);
                if (state_code == 2)
                    w_u8(output + 2, 1);
                if (state_code == 6 && PadSetActAlign((sint32)saved_port, (uint8 *)psx_addr(0x800a0344, 6)) != 0)
                    w_u8(output + 2, 1);
            }
        }
        else if (r_u32(active) != 0)
        {
            for (uint32 slot = 0; slot < 4; ++slot)
                w_u16(0x800a2a9c + controller * 0xd0 + slot * 0x34 + 6, 0);
            w_u32(active, 0);
        }
    }
    uint32 mode = (uint32)PadInfoMode(0, 3, 0);
    uint32 previous = r_u32(0x800a0424);
    if (mode != previous)
    {
        w_u32(0x800a0424, mode);
        G3Function80081154();
    }
    return 1;
}

uint32 G3Function8008018C(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8008018c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    G3Function8007F3C0(object, game, state, frame);
    uint32 object_flags = r_u32(object + 0x10);
    uint32 controls = (object_flags & 0x100) != 0 ? 0x800a0d28 : game + 0x70;
    uint32 motion = r_u32(object + 0x154);
    uint32 flags = r_u32(object + 0x14);
    uint32 selector = 0;
    motion >>= 1;
    w_u32(object + 0x14, flags | 0x40);
    while (motion != 0)
    {
        motion >>= 1;
        ++selector;
    }
    if (selector < 0x20)
    {
        uint32 target = r_u32(0x8009a178 + selector * 4);
        if (target != 0)
            G3DispatchPlayerUpdate(target, object, game, state, controls, frame);
    }
    if ((r_u16(state + 0xce) & 1) != 0 && (r_u32(object + 0x154) & 0x260088) == 0)
        w_u32(object + 0x10c, 0);
    motion = r_u32(object + 0x154);
    if ((motion & 0xf2410000) == 0)
    {
        if (motion != 0x8000)
            G3Function8008004C(object, game, state, controls);
        if ((r_u16(state + 0xce) & 1) != 0)
            G3Function8007FE80(object, game, state);
        G3Function8007FA24(object, game, state, frame);
        uint32 angle = r_u16(state + 0xc6);
        w_u16(object + 0x64, angle);
    }
    if ((r_u32(object + 0x154) & 0xf2400000) == 0)
    {
        G3Function8007F214(object);
        G3Function8007205C(object, game, state, frame);
    }
    if (r_u32(object + 0x150) == 1)
    {
        uint32 contact = r_u32(0x800a0284);
        uint32 previous = r_u32(contact + 0xc);
        w_u32(contact + 0x10, previous);
    }
    else
    {
        uint32 contact = r_u32(0x800a0284);
        w_u32(contact + 0x10, 0);
        w_u32(contact + 0xc, 0);
    }
    uint32 contact = r_u32(0x800a0284);
    uint32 normal[2];
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 source = contact + 0x14 + index * 4;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
        normal[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 destination = contact + 0x1c + index * 4;
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, normal[index] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        uint32 low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, normal[index] << (low_lane * 8), (15 << low_lane) & 15);
    }
    flags = r_u16(state + 0xce);
    w_u16(state + 0xce, (flags & 1) != 0 ? flags | 0x100 : flags & 0xfeff);
    if ((r_u32(object + 0x154) & 0x01028613) != 0)
    {
        uint32 orientation = r_u32(state);
        w_u16(orientation + 2, 0);
    }
    uint32 orientation = r_u32(state);
    uint32 current_game = r_u32(0x80097048);
    w_u16(orientation + 8, 0);
    orientation = r_u32(state);
    w_u16(orientation, 0);
    if ((r_u32(current_game + 0x6834) & 0x10) == 0 && (r_u32(object + 0x128) & 0x20000000) != 0)
    {
        G3Function8006C610(current_game);
        current_game = r_u32(0x80097048);
        uint32 value = r_u32(current_game + 0x6818);
        G3Function80047EE4(0, value);
        current_game = r_u32(0x80097048);
        G3Function8007446C(current_game);
        flags = r_u32(object + 0x128);
        w_u32(object + 0x128, flags & 0xdfffffff);
    }
    if ((r_u32(game + 0x6900) & 0x2000) != 0 && (r_u32(controls + 4) & 0x8000) != 0 && G3Function8003525C() != 0 && r_u8(0x800aa7d2) == 0)
    {
        uint32 key = r_u32(0x800a031c);
        uint32 text = r_u32(0x8009822c + key * 4);
        uint32 length = (uint32)r_s16(text);
        uint32 line = r_u32(0x800a0320);
        if (line == length)
        {
            uint32 next = key + 1;
            w_u32(0x800a0320, 0);
            w_u32(0x800a031c, next);
            if (next == 0x126)
                w_u32(0x800a031c, 0);
        }
        key = r_u32(0x800a031c);
        line = r_u32(0x800a0320);
        w_u8(0x800aa7d2, 1);
        G3Function8003592C(key, line, frame);
        line = r_u32(0x800a0320);
        w_u32(0x800aa7d8, 0xffffffff);
        w_u8(0x800aa7d3, 0);
        w_u32(0x800a0320, line + 1);
    }
    G3Function8007E270(object, game, state, frame);
    uint32 result = r_u32(object + 0x134) & 0x20;
    if (result != 0)
        return G3Function8007B1A8(object, state, frame);
    return result;
}

uint32 G3Function8007B1A8(uint32 object, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007b1a8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 result = r_u32(object + 0x134) & 0x20;
    if (result == 0)
        return result;
    uint32 animation = r_u16(object + 0x6e);
    sint32 desired = r_s16(state + 0x23a);
    uint32 raw_desired = r_u16(state + 0x23a);
    if (animation != (uint32)desired)
    {
        uint32 resource = r_u32(object + 0x1c);
        w_u16(object + 0x6e, raw_desired);
        desired = r_s16(state + 0x23a);
        uint32 animations = r_u32(resource + 0x10);
        uint32 descriptor = r_u32(animations + ((uint32)desired << 2));
        sint32 length = r_s16(descriptor + 2);
        if (length != 0)
        {
            sint32 position = r_s32(state + 0x23c);
            uint32 remainder = (uint32)((sint64)position % length);
            w_u16(object + 0x5e, remainder);
        }
        else
        {
            uint32 position = r_u16(state + 0x23c);
            w_u16(object + 0x5e, position);
        }
    }
    uint32 next = r_u32(state + 0x23c) + 1;
    sint64 product = (sint64)(sint32)next * 0x55555556;
    uint32 sign = (uint32)((sint32)next >> 31);
    w_u32(state + 0x23c, next);
    uint32 high = (uint32)(product >> 32);
    result = high - sign;
    if (result * 3 == next - 1)
        return G3Function8007B280(object, state, frame);
    return result;
}

uint32 G3Function8007B280(uint32 object, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007b280, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    uint32 animation_state = state + 0x234;
    uint32 resource = r_u32(object + 0x1c);
    sint32 animation = r_s16(animation_state + 6);
    uint32 animations = r_u32(resource + 0x10);
    bool special = ((uint32)animation ^ 0x32) == 0;
    uint32 descriptor = r_u32(animations + ((uint32)animation << 2));
    uint32 current = r_u16(object + 0x5e);
    uint32 length = r_u16(descriptor + 2);
    uint32 remaining = length - (current + 1);
    sint32 signed_remaining = (sint32)(remaining << 16) >> 16;
    if (!special || signed_remaining >= 0x21)
    {
        uint32 extent = special && signed_remaining >= 0x11 ? 0x10 : remaining;
        uint32 player = r_u32(0x8009efc4);
        extent = (uint32)((sint32)(extent << 16) >> 16);
        uint32 effect = G3Function800757B0(player, 1, extent, 0xc, frame);
        if (effect == 0)
            return 0;
        uint32 radius;
        if (special)
        {
            sint32 elapsed = r_s32(animation_state + 8);
            if (elapsed < 0x78)
                radius = 0xc8 - r_u16(animation_state + 8);
            else
                radius = 0x50;
        }
        else
            radius = (r_u32(animation_state + 8) << 1) + 0x50;
        w_u16(frame + 0x10, radius);
        sint32 random = (sint32)G3SdkRandom80027FA8();
        w_u16(frame + 0x12, (uint32)(random % 200));
        random = (sint32)G3SdkRandom80027FA8();
        w_u16(frame + 0x14, (uint32)(random % 4096));
        random = (sint32)G3SdkRandom80027FA8();
        w_u16(frame + 0x16, (uint32)(random % 128) + 0x40);
        w_u16(frame + 0x18, 0);
        w_u16(frame + 0x1a, 0);
        w_u16(frame + 0x1c, 0x96);
        return G3Function8007597C(effect, 0, frame + 0x10, frame, 0x96);
    }
    uint32 linked = r_u32(animation_state);
    if (linked == 0)
        return 0;
    if (!special && r_s32(animation_state + 8) < 0x19)
        remaining = 0x19 - r_u16(animation_state + 8);
    uint32 player = r_u32(0x8009efc4);
    uint32 extent = (uint32)((sint32)(remaining << 16) >> 16);
    uint32 effect = G3Function800757B0(player, 1, extent, 0xc, frame);
    if (effect == 0)
        return 0;
    player = r_u32(0x8009efc4);
    uint32 value = r_u16(player + 0x58);
    w_u16(frame + 0x10, value);
    value = r_u16(player + 0x5a);
    w_u16(frame + 0x12, value);
    value = r_u16(player + 0x5c);
    w_u16(frame + 0x14, value + 0x96);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        linked = r_u32(animation_state);
        uint32 position = r_u16(linked + 0x58 + axis * 2);
        uint32 offset = r_u16(linked + 0x128 + axis * 2);
        w_u16(frame + 0x18 + axis * 2, position + offset);
    }
    uint32 source = special ? frame + 0x10 : frame + 0x18;
    uint32 destination = special ? frame + 0x18 : frame + 0x10;
    uint32 x = r_u16(source);
    w_u16(frame + 0x20, x);
    uint32 y = r_u16(source + 2);
    w_u16(frame + 0x22, y);
    uint32 z = r_u16(source + 4) + 0x96;
    w_u16(frame + 0x24, z);
    value = r_u16(destination);
    w_u16(frame + 0x28, value - x);
    value = r_u16(destination + 2);
    w_u16(frame + 0x2a, value - y);
    value = r_u16(destination + 4);
    w_u16(frame + 0x2c, value - z);
    if (!special)
    {
        value = r_u16(animation_state + 8);
        w_u16(frame + 0x26, value - 2);
    }
    else
        w_u16(frame + 0x26, 0);
    w_u16(frame + 0x2e, 0x78);
    sint32 random = (sint32)G3SdkRandom80027FA8();
    w_u16(frame + 0x30, (uint32)(random % 4096));
    random = (sint32)G3SdkRandom80027FA8();
    w_u16(frame + 0x32, (uint32)(random % 128) + 0x40);
    return G3Function8007597C(effect, 1, frame + 0x20, frame, (uint32)(random % 128) + 0x40);
}

uint32 G3Function800757B0(uint32 object, uint32 mode, uint32 extent, uint32 kind, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800757b0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    if (object == 0)
        return 0;
    uint32 pool = r_u32(0x800a02e4);
    uint32 effect = r_u32(pool + 0xc10);
    if (effect == 0)
        return 0;
    uint32 next = r_u32(effect);
    uint32 previous = r_u32(pool + 0xc14);
    w_u32(pool + 0xc10, next);
    w_u32(effect, previous);
    w_u32(pool + 0xc14, effect);
    w_u32(effect + 4, object);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 coordinate = r_u16(object + 0x58 + axis * 2);
        w_u16(effect + 8 + axis * 2, coordinate);
    }
    uint32 coordinate = r_u16(object + 0x58);
    w_u16(effect + 0x10, coordinate);
    coordinate = r_u16(object + 0x5a);
    w_u16(effect + 0x12, coordinate);
    coordinate = r_u16(object + 0x5c);
    w_u16(effect + 0xe, extent);
    w_u8(effect + 0x1f, kind);
    w_u16(effect + 0x16, mode);
    w_u16(effect + 0x14, coordinate);
    sint32 random = (sint32)G3SdkRandom80027FA8();
    w_u8(frame + 0x1c, 0xc8);
    w_u8(frame + 0x1d, 0xc8);
    w_u8(frame + 0x21, 0xc8);
    w_u8(frame + 0x22, 0xc8);
    w_u8(frame + 0x24, 0xc8);
    w_u8(frame + 0x26, 0xc8);
    w_u8(frame + 0x10, 0xf0);
    w_u8(frame + 0x11, 0x28);
    w_u8(frame + 0x15, 0xf0);
    w_u8(frame + 0x1a, 0xf0);
    sint32 selected = random % 6;
    selected = (sint32)((uint32)selected << 16) >> 16;
    sint32 alternate = (selected + 2) % 6;
    w_u8(frame + 0x12, 0x28);
    w_u8(frame + 0x14, 0x28);
    w_u8(frame + 0x16, 0x28);
    w_u8(frame + 0x18, 0x28);
    w_u8(frame + 0x19, 0x28);
    w_u8(frame + 0x1e, 0x28);
    w_u8(frame + 0x20, 0x28);
    w_u8(frame + 0x25, 0x28);
    uint32 first_offset = (uint32)selected << 2;
    uint32 first_color = frame + 0x10 + first_offset;
    uint32 second_color = frame + 0x10 + ((uint32)alternate << 2);
    G3Function80075924(effect, first_color, second_color, first_offset);
    return effect;
}

uint32 G3Function80075924(uint32 effect, uint32 first_color, uint32 second_color, uint32 incoming_v0)
{
    FUNCTION_MARKER(0x80075924, "SLUS_008.06");
    if (effect == 0)
        return incoming_v0;
    uint32 result = 0;
    for (uint32 channel = 0; channel < 3; ++channel)
    {
        result = r_u8(first_color + channel);
        w_u8(effect + 0x18 + channel, result);
    }
    for (uint32 channel = 0; channel < 3; ++channel)
    {
        result = r_u8(second_color + channel);
        w_u8(effect + 0x1c + channel, result);
    }
    return result;
}

uint32 G3Function8007597C(uint32 effect, uint32 mode, uint32 parameters, uint32 guest_stack, uint32 incoming_v0)
{
    FUNCTION_MARKER(0x8007597c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (effect == 0)
        return incoming_v0;
    uint32 result = (sint32)mode < 3 ? 1 : 0;
    if (parameters == 0)
        return result;
    w_u8(effect + 0x1b, result != 0 ? mode : 0);
    uint32 kind = r_u8(effect + 0x1b);
    if (kind == 1)
        G3Function80075D28(effect, parameters, frame);
    else if (kind == 0)
        G3Function80075AC0(effect, parameters, frame);
    else if (kind == 2)
        G3Function800760C0();
    return G3Function80075A28(effect, frame);
}

void G3Function800760C0(void)
{
    FUNCTION_MARKER(0x800760c0, "SLUS_008.06");
}

uint32 G3Function80075AC0(uint32 effect, uint32 parameters, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80075ac0, "SLUS_008.06");
    sint32 radius = r_s16(parameters);
    uint32 raw_radius = r_u16(parameters);
    w_u16(effect + 0x20, radius > 0x3c ? raw_radius : 0x3c);
    sint32 phase = r_s16(parameters + 2);
    uint32 raw_phase = r_u16(parameters + 2);
    w_u16(effect + 0x22, phase < 0xe1 ? raw_phase : 0);
    sint32 angle = r_s16(parameters + 4);
    w_u16(effect + 0x24, (uint32)(angle % 4096));
    uint32 result = r_u16(parameters + 6);
    w_u16(effect + 0x26, result);
    uint32 copied[2];
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 source = parameters + 8 + index * 4;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
        copied[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 destination = effect + 0x28 + index * 4;
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, copied[index] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        uint32 low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, copied[index] << (low_lane * 8), (15 << low_lane) & 15);
    }
    return result;
}

uint32 G3Function80075D28(uint32 effect, uint32 parameters, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80075d28, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    sint32 duration = r_s16(parameters + 0xe);
    uint32 elapsed_raw = r_u16(parameters + 6);
    uint32 raw_duration = r_u16(parameters + 0xe);
    w_u16(effect + 0x2e, duration > 20 ? raw_duration : 20);
    sint32 angle = r_s16(parameters + 0x10);
    w_u16(effect + 0x30, (uint32)(angle % 4096));
    uint32 value = r_u16(parameters + 0x12);
    sint32 speed = r_s16(effect + 0xe);
    w_u16(effect + 0x32, value);
    value = r_u16(parameters);
    w_u16(effect + 0x20, value);
    value = r_u16(parameters + 2);
    w_u16(effect + 0x22, value);
    value = r_u16(parameters + 4);
    w_u16(effect + 0x24, value);
    value = r_u16(parameters + 8);
    w_u16(effect + 0x28, value);
    value = r_u16(parameters + 0xa);
    w_u16(effect + 0x2a, value);
    value = r_u16(parameters + 0xc);
    w_u16(effect + 0x2c, value);
    if (speed != 0)
    {
        sint32 length = (sint32)G3Function8002FA50(parameters + 8, frame);
        if (length != 0)
        {
            uint32 x = (uint32)(sint32)r_s16(parameters + 8) << 12;
            uint32 normalized_x = (uint32)((sint64)(sint32)x / length);
            w_u16(effect + 0x28, normalized_x);
            uint32 y = (uint32)(sint32)r_s16(parameters + 0xa) << 12;
            uint32 normalized_y = (uint32)((sint64)(sint32)y / length);
            speed = r_s16(effect + 0xe);
            uint32 scale = speed == 0 ? (length < 0 ? 1 : uint32(0xffffffff)) : (uint32)((sint64)length / speed);
            w_u16(effect + 0x2a, normalized_y);
            uint32 z = (uint32)(sint32)r_s16(parameters + 0xc) << 12;
            uint32 normalized_z = (uint32)((sint64)(sint32)z / length);
            w_u16(effect + 0x26, scale);
            w_u16(effect + 0x2c, normalized_z);
        }
    }
    uint32 result = elapsed_raw << 16;
    sint32 elapsed = (sint32)result >> 16;
    if (elapsed <= 0)
        return result;
    sint32 scale = r_s16(effect + 0x26);
    uint32 scaled_x = (uint32)(elapsed * scale);
    sint32 direction_x = r_s16(effect + 0x28);
    uint32 product_x = scaled_x * (uint32)direction_x;
    if ((sint32)product_x < 0)
        product_x += 0xfff;
    scale = r_s16(effect + 0x26);
    uint32 scaled_y = (uint32)(elapsed * scale);
    sint32 direction_y = r_s16(effect + 0x2a);
    uint32 product_y = scaled_y * (uint32)direction_y;
    uint32 old_x = r_u16(effect + 0x20);
    uint32 result_x = old_x + (uint32)((sint32)product_x >> 12);
    w_u16(effect + 0x20, result_x);
    if ((sint32)product_y < 0)
        product_y += 0xfff;
    scale = r_s16(effect + 0x26);
    uint32 scaled_z = (uint32)(elapsed * scale);
    sint32 direction_z = r_s16(effect + 0x2c);
    uint32 product_z = scaled_z * (uint32)direction_z;
    uint32 old_y = r_u16(effect + 0x22);
    uint32 result_y = old_y + (uint32)((sint32)product_y >> 12);
    w_u16(effect + 0x22, result_y);
    if ((sint32)product_z < 0)
        product_z += 0xfff;
    uint32 old_z = r_u16(effect + 0x24);
    result = old_z + (uint32)((sint32)product_z >> 12);
    w_u16(effect + 0x24, result);
    return result;
}

uint32 G3Function80012DA0(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80012da0, "SLUS_008.06");
    return G3Function80022E00(guest_stack - 0x18);
}

uint32 G3Function80012D38(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80012d38, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0xf0);
    w_u32(frame + 0x18, 0x200);
    w_u32(frame + 0x1c, 0xf0);
    w_u32(frame + 0x20, 0);
    w_u32(frame + 0x24, 0);
    w_u32(frame + 0x28, 0);
    w_u32(frame + 0x2c, 0);
    uint32 pool = r_u32(game + 0x68);
    w_u32(frame + 0x30, pool);
    uint32 order = r_u32(game + 0x38) + 0x190;
    w_u32(frame + 0x34, order);
    return G3Function8001E25C(0, 0, 0x200, 0, 0, 0xf0, 0x200, 0xf0, 0, 0, 0, 0, pool, order);
}

uint32 G3Function80012954(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80012954, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 game = r_u32(0x80097048);
    uint32 object = G3Function8006C914(game, frame);
    game = r_u32(0x80097048);
    uint32 level = r_u16(game + 0x68b4);
    uint32 result = level - 20;
    if (level - 11 >= 5)
    {
        result = result < 3 ? 1 : 0;
        if (result == 0)
            return result;
    }
    if (object == 0)
        return result;
    result = r_u32(object + 0xec);
    if (result != 0)
        result = G3DispatchStateEvent(result, object, game, 0x7ee, 0, frame);
    return result;
}

uint32 G3Function800129CC(uint32 guest_stack)
{
    FUNCTION_MARKER(0x800129cc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 game = r_u32(0x80097048);
    G3Function80027118(game, frame);
    game = r_u32(0x80097048);
    uint32 flags = r_u32(game + 0xe8);
    w_u16(game + 0x67ee, 0);
    w_u32(game + 0xe8, flags ^ 0x40000);
    return game;
}

uint32 G3Function80012830(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80012830, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 count = 0;
    uint32 entry = 0x80096958;
    do
    {
        uint32 index = r_u32(0x8009cc00);
        uint32 offset = (index << 1) + index;
        uint32 records = r_u32(0x8009ceac);
        uint32 text = r_u32(records + (offset << 3) + 0xc);
        uint32 comparison = G3CompareString80085EC4(text, entry);
        ++count;
        if (comparison == 0)
        {
            uint32 selected = r_u32(entry + 0x10);
            w_u32(0x800aa7b8, selected);
            break;
        }
        entry += 0x14;
    } while (count < 3);
    w_u32(0x800aa7c0, 1);
    G3Function80027584(0x17, frame);
    uint32 game = r_u32(0x80097048);
    w_u16(game + 0x6786, 0);
    return game;
}

uint32 G3Function80052514(uint32 origin, uint32 radius, uint32 angle, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80052514, "SLUS_008.06");
    uint32 frame = guest_stack - 0x98;
    sint32 signed_angle = (sint32)(angle << 16) >> 16;
    if (signed_angle != 0)
    {
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 target = r_u16(output + axis * 2);
            uint32 start = r_u16(origin + axis * 2);
            w_u16(frame + 0x50 + axis * 2, target - start);
        }
        G3Function8002F8E4(frame + 0x50, (uint32)signed_angle);
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 rotated = r_u16(frame + 0x50 + axis * 2);
            uint32 start = r_u16(origin + axis * 2);
            w_u16(frame + 0x68 + axis * 2, rotated + start);
        }
    }
    else
    {
        uint32 high_address = output + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low_lane = output & 3;
        uint32 low = r_masked_u32(output & 0xfffffffc, (15 << low_lane) & 15);
        uint32 copied = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        sint32 tail = r_s16(output + 4);
        uint32 destination = frame + 0x68;
        high_address = destination + 3;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, copied >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, copied << (low_lane * 8), (15 << low_lane) & 15);
        w_u16(frame + 0x6c, (uint32)tail);
    }
    uint32 first_hit = 0;
    uint32 second_hit = 0;
    uint32 disabled = r_u32(0x800a2534);
    if (disabled == 0)
    {
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 target = r_u16(frame + 0x68 + axis * 2);
            uint32 start = r_u16(origin + axis * 2);
            w_u16(frame + 0x50 + axis * 2, target - start);
        }
        uint32 signed_radius = (uint32)((sint32)(radius << 16) >> 16);
        G3Function8002F9A8(frame + 0x58, signed_radius, frame + 0x50, frame);
        G3Function8002F9A8(frame + 0x60, 250, frame + 0x50, frame);
        G3Function80052454(frame + 0x20, 0xfffffc00, frame + 0x58, origin);
        G3Function80052454(frame + 0x28, 0x400, frame + 0x58, origin);
        G3Function80052454(frame + 0x30, 0xfffffc00, frame + 0x60, frame + 0x68);
        G3Function80052454(frame + 0x38, 0x400, frame + 0x60, frame + 0x68);
        uint32 game = r_u32(0x80097048);
        for (uint32 side = 0; side < 2; ++side)
        {
            uint32 copied[2];
            for (uint32 word = 0; word < 2; ++word)
            {
                uint32 source = frame + 0x30 + side * 8 + word * 4;
                uint32 high_address = source + 3;
                uint32 high_lane = high_address & 3;
                uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                uint32 low_lane = source & 3;
                uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
                copied[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
            }
            for (uint32 word = 0; word < 2; ++word)
            {
                uint32 destination = frame + 0x40 + side * 8 + word * 4;
                uint32 high_address = destination + 3;
                uint32 high_lane = high_address & 3;
                w_masked_u32(high_address & 0xfffffffc, copied[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                uint32 low_lane = destination & 3;
                w_masked_u32(destination & 0xfffffffc, copied[word] << (low_lane * 8), (15 << low_lane) & 15);
            }
        }
        uint32 world_owner = r_u32(game + 0x48);
        w_u32(frame + 0x10, 0);
        w_u32(frame + 0x14, 0x1d529e8);
        w_u32(frame + 0x18, 0);
        uint32 world = r_u32(world_owner);
        first_hit = G3Function8006238C(world, frame + 0x30, frame + 0x20, 1, frame);
        game = r_u32(0x80097048);
        world_owner = r_u32(game + 0x48);
        w_u32(frame + 0x10, 0);
        w_u32(frame + 0x14, 0x1d529e8);
        w_u32(frame + 0x18, 0);
        world = r_u32(world_owner);
        second_hit = G3Function8006238C(world, frame + 0x38, frame + 0x28, 1, frame);
        uint32 hits[2] = {first_hit, second_hit};
        for (uint32 side = 0; side < 2; ++side)
        {
            if (hits[side] != 0)
            {
                uint32 difference[3];
                uint32 squared = 0;
                for (uint32 axis = 0; axis < 3; ++axis)
                {
                    uint32 current = r_u16(frame + 0x30 + side * 8 + axis * 2);
                    uint32 previous = r_u16(frame + 0x40 + side * 8 + axis * 2);
                    difference[axis] = current - previous;
                    sint32 delta = (sint32)(difference[axis] << 16) >> 16;
                    squared += (uint32)(delta * delta);
                }
                for (uint32 axis = 0; axis < 3; ++axis)
                    w_u16(frame + 0x70 + axis * 2, difference[axis]);
                if ((sint32)squared <= 0xf423)
                    hits[side] = 0;
            }
        }
        first_hit = hits[0];
        second_hit = hits[1];
    }
    if (first_hit == 0 && second_hit == 0)
    {
        uint32 x = r_u16(frame + 0x68);
        uint32 y = r_u16(frame + 0x6a);
        uint32 z = r_u16(frame + 0x6c);
        w_u16(0x800a23ec, x);
        w_u16(0x800a23ee, y);
        w_u16(0x800a23f0, z);
        return 0;
    }
    if (first_hit != 0)
        return second_hit != 0 ? 3 : 1;
    return 2;
}

uint32 G3Function800528F4(uint32 origin, uint32 start_angle, uint32 target_angle, uint32 start_radius, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800528f4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    uint32 difference = (target_angle - start_angle) & 0xfff;
    uint32 target_radius = r_u32(guest_stack + 0x10);
    uint32 height = r_u16(guest_stack + 0x14);
    w_u16(frame + 0x28, height);
    sint32 signed_difference = difference < 0x800 ? (sint32)difference : (sint32)difference - 0x1000;
    if (signed_difference == 0)
        return 1;
    sint32 angle_step = signed_difference < 0 ? -128 : 128;
    sint32 distance = signed_difference < 0 ? -signed_difference : signed_difference;
    sint32 steps = distance / 128 + 1;
    sint32 radius_delta = ((sint32)(target_radius << 16) >> 16) - ((sint32)(start_radius << 16) >> 16);
    w_u16(frame + 0x30, (uint32)(radius_delta / steps));
    uint32 count = 0;
    uint32 current_angle = start_angle;
    uint32 current_radius = start_radius;
    sint32 initial_angle = (sint32)(start_angle << 16) >> 16;
    do
    {
        sint32 angle = (sint32)(current_angle << 16) >> 16;
        uint32 current_height = r_u16(frame + 0x28);
        w_u16(frame + 0x20, current_radius);
        w_u16(frame + 0x22, 0);
        w_u16(frame + 0x24, current_height);
        G3Function8002F8E4(frame + 0x20, (uint32)angle);
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 component = r_u16(frame + 0x20 + axis * 2);
            uint32 base = r_u16(origin + axis * 2);
            w_u16(frame + 0x18 + axis * 2, component + base);
        }
        if (angle != initial_angle)
        {
            uint32 game = r_u32(0x80097048);
            uint32 owner = r_u32(game + 0x48);
            uint32 world = r_u32(owner);
            if (G3Function80062358(world, frame + 0x18, frame + 0x10, 0, frame) != 0)
                return 0;
        }
        else
        {
            uint32 copied[2];
            for (uint32 word = 0; word < 2; ++word)
            {
                uint32 source = frame + 0x18 + word * 4;
                uint32 high_address = source + 3;
                uint32 high_lane = high_address & 3;
                uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                uint32 low_lane = source & 3;
                uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
                copied[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
            }
            for (uint32 word = 0; word < 2; ++word)
            {
                uint32 destination = frame + 0x10 + word * 4;
                uint32 high_address = destination + 3;
                uint32 high_lane = high_address & 3;
                w_masked_u32(high_address & 0xfffffffc, copied[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                uint32 low_lane = destination & 3;
                w_masked_u32(destination & 0xfffffffc, copied[word] << (low_lane * 8), (15 << low_lane) & 15);
            }
        }
        ++count;
        current_angle += (uint32)angle_step;
        {
            uint32 copied[2];
            for (uint32 word = 0; word < 2; ++word)
            {
                uint32 source = frame + 0x18 + word * 4;
                uint32 high_address = source + 3;
                uint32 high_lane = high_address & 3;
                uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                uint32 low_lane = source & 3;
                uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
                copied[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
            }
            for (uint32 word = 0; word < 2; ++word)
            {
                uint32 destination = frame + 0x10 + word * 4;
                uint32 high_address = destination + 3;
                uint32 high_lane = high_address & 3;
                w_masked_u32(high_address & 0xfffffffc, copied[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                uint32 low_lane = destination & 3;
                w_masked_u32(destination & 0xfffffffc, copied[word] << (low_lane * 8), (15 << low_lane) & 15);
            }
        }
        uint32 radius_step = r_u16(frame + 0x30);
        current_radius += radius_step;
    } while (((sint32)(count << 16) >> 16) <= steps);
    return 1;
}

uint32 G3Function80052D44(uint32 state, uint32 origin, uint32 target, uint32 pivot, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80052d44, "SLUS_008.06");
    uint32 flags = r_u32(0x800a24d0);
    uint32 frame = guest_stack - 0x38;
    w_u32(0x800a24d0, flags | 0x1000);
    for (uint32 axis = 0; axis < 2; ++axis)
    {
        uint32 position = r_u16(0x800a23ec + axis * 2);
        uint32 base = r_u16(origin + axis * 2);
        w_u16(state + 0x18 + axis * 2, position - base);
    }
    uint32 position_z = r_u16(0x800a23f0);
    uint32 origin_z = r_u16(origin + 4);
    uint32 x = r_u16(state + 0x18);
    w_u16(state + 0x1c, position_z - origin_z);
    w_u16(frame + 0x18, x);
    uint32 y = r_u16(state + 0x1a);
    w_u16(frame + 0x1a, y);
    uint32 z = r_u16(state + 0x1c);
    w_u16(frame + 0x1c, z);
    uint32 signed_x = (uint32)((sint32)(x << 16) >> 16);
    uint32 signed_y = (uint32)((sint32)(y << 16) >> 16);
    uint32 angle = G3Function8002F240(signed_y, signed_x, frame) & 0xfff;
    w_u32(state + 0x14, angle);
    if (angle >= 0x800)
        w_u32(state + 0x14, angle - 0x1000);
    uint32 start = r_u16(0x800a24ec);
    sint32 finish = r_s16(state + 0x14);
    sint32 radius = r_s16(0x800a24f8);
    uint32 start_angle = (uint32)((sint32)((start - 0x400) << 16) >> 16);
    w_u32(frame + 0x10, (uint32)radius);
    uint32 target_z = r_u16(target + 4);
    origin_z = r_u16(origin + 4);
    uint32 height = (uint32)((sint32)((target_z - origin_z) << 16) >> 16);
    w_u32(frame + 0x14, height);
    uint32 result = G3Function800528F4(pivot, start_angle, (uint32)finish, (uint32)radius, frame);
    if (result == 0)
    {
        flags = r_u32(0x800a24d0);
        result = flags | 0x400;
        w_u32(0x800a24d0, result);
    }
    return result;
}

uint32 G3Function8005309C(uint32 position, uint32 pivot, uint32 state, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005309c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    bool left = false;
    bool right = false;
    bool left_tap = false;
    bool right_tap = false;
    uint32 flags = r_u32(state + 0x1c);
    if ((flags & 0xc0) != 0)
    {
        uint32 camera_flags = r_u32(0x800a24d0);
        if ((camera_flags & 2) != 0)
            w_u32(state + 0x1c, flags & 0xffffff3f);
    }
    uint32 camera_flags = r_u32(0x800a24d0);
    bool reset = (camera_flags & 0x18) != 0;
    if (!reset)
    {
        uint32 override_input = r_u32(0x80096b54);
        if (override_input == 0)
        {
            flags = r_u32(state + 0x1c);
            reset = (flags & 0x30) != 0 && (flags & 2) != 0;
        }
    }
    if (reset)
    {
        flags = r_u32(state + 0x1c);
        w_u32(state + 8, 0);
        w_u32(state + 0xc, 0);
        w_u32(state + 0x1c, (flags & 0xffffffc3) | 1);
    }
    if (r_u32(0x80096b4c) == 0 && r_u32(0x80096b50) == 0)
    {
        camera_flags = r_u32(0x800a24d0);
        w_u32(0x800a24d0, camera_flags & 0xffffffe7);
        flags = r_u32(state + 0x1c);
        w_u32(state + 0x1c, flags & 0xfffffffe);
    }
    uint32 timer = r_u32(state + 8);
    if (timer != 0)
        w_u32(state + 8, timer - 1);
    timer = r_u32(state + 0xc);
    if (timer != 0)
        w_u32(state + 0xc, timer - 1);
    if (r_u32(0x80096b4c) != 0)
    {
        uint32 held = r_u32(state);
        w_u32(state, held + 1);
        flags = r_u32(state + 0x1c);
        w_u32(state + 0x1c, flags & 0xffffffc3);
    }
    else
    {
        uint32 held = r_u32(state);
        if (held != 0 && (sint32)held < 5)
        {
            uint32 options = r_u32(0x8009ea44);
            if ((options & 0x10000) != 0 || r_u32(state + 8) != 0)
                left_tap = true;
            else
            {
                uint32 delay = r_u32(0x8009eae0);
                w_u32(state + 0xc, 0);
                w_u32(state + 8, delay);
            }
        }
        w_u32(state, 0);
    }
    if (r_u32(0x80096b50) != 0)
    {
        uint32 held = r_u32(state + 4);
        w_u32(state + 4, held + 1);
        flags = r_u32(state + 0x1c);
        w_u32(state + 0x1c, flags & 0xffffffc3);
    }
    else
    {
        uint32 held = r_u32(state + 4);
        if (held != 0 && (sint32)held < 5)
        {
            uint32 options = r_u32(0x8009ea44);
            if ((options & 0x10000) != 0 || r_u32(state + 0xc) != 0)
                right_tap = true;
            else
            {
                uint32 delay = r_u32(0x8009eae0);
                w_u32(state + 8, 0);
                w_u32(state + 0xc, delay);
            }
        }
        w_u32(state + 4, 0);
    }
    uint32 options = r_u32(0x8009ea44);
    camera_flags = r_u32(0x800a24d0);
    bool allowed = (options & 0x20000) == 0 && (camera_flags & 0x440) == 0;
    flags = r_u32(state + 0x1c);
    if ((flags & 1) != 0)
        allowed = false;
    uint32 result = flags & 0x54;
    if (!allowed)
        return result;
    if ((flags & 0x54) != 0)
        left = true;
    else if ((flags & 0xa8) != 0)
        right = true;
    else if (r_u32(0x80096b4c) == 0)
        right = r_u32(0x80096b50) != 0;
    else if (r_u32(0x80096b50) == 0)
        left = true;
    else
    {
        uint32 left_count = r_u32(state);
        uint32 right_count = r_u32(state + 4);
        if ((sint32)right_count < (sint32)left_count)
            left = true;
        else
            right = true;
    }
    if (left_tap || right_tap)
    {
        flags = r_u32(state + 0x1c);
        uint32 cleared = flags & 0xffffff03;
        w_u32(state + 0x1c, cleared);
        uint32 selected;
        if (right_tap)
        {
            w_u32(state + 0x1c, cleared | 8);
            selected = r_u32(0x800a24ec) - 1;
        }
        else
        {
            w_u32(state + 0x1c, cleared | 4);
            selected = r_u32(0x800a24ec) + 0x400;
        }
        w_u32(state + 0x10, selected & 0xfffffc00);
        selected = r_u32(state + 0x10);
        uint32 speed = r_u32(0x8009eadc);
        selected = (selected - 0x400) & 0xfff;
        w_u32(state + 0x14, speed);
        w_u32(state + 0x10, selected);
    }
    uint32 rotation = 0;
    if (right || left)
    {
        uint32 difference[3];
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 current = r_u16(position + axis * 2);
            uint32 center = r_u16(pivot + axis * 2);
            difference[axis] = current - center;
            w_u16(frame + 0x10 + axis * 2, difference[axis]);
        }
        uint32 x = (uint32)((sint32)(difference[0] << 16) >> 16);
        uint32 y = (uint32)((sint32)(difference[1] << 16) >> 16);
        uint32 angle = G3Function8002F240(y, x, frame);
        flags = r_u32(state + 0x1c);
        if ((flags & 0xfc) != 0)
        {
            sint32 desired = r_s16(state + 0x10);
            uint32 current = (uint32)((sint32)(angle << 16) >> 16);
            uint32 delta_bits = G3Function80038B48(current, (uint32)desired);
            sint32 delta = (sint32)(delta_bits << 16) >> 16;
            uint32 speed = r_u32(state + 0x14);
            sint32 magnitude = delta < 0 ? -delta : delta;
            if ((sint32)speed >= magnitude)
            {
                flags = r_u32(state + 0x1c);
                w_u32(state + 0x18, 0);
                w_u32(state + 0x1c, (flags & 0xffffff03) | 1);
            }
            speed = r_u32(state + 0x14);
            uint32 lower = 0 - speed;
            if ((sint32)lower < delta)
                lower = (uint32)delta;
            rotation = (sint32)lower < (sint32)speed ? lower : speed;
        }
        else
        {
            uint32 speed = r_u32(state + 0x18);
            uint32 acceleration = r_u32(0x8009ead4);
            w_u32(state + 0x18, left ? speed + acceleration : speed - acceleration);
            uint32 limit = r_u32(0x8009ead8);
            speed = r_u32(state + 0x18);
            uint32 lower = 0 - limit;
            if ((sint32)lower < (sint32)speed)
                lower = speed;
            uint32 clamped = (sint32)lower < (sint32)limit ? lower : limit;
            w_u32(state + 0x18, clamped);
            rotation = r_u16(state + 0x18);
        }
    }
    else
    {
        uint32 speed = r_u32(state + 0x18);
        if (speed != 0)
        {
            uint32 reduced = (uint32)((sint32)speed >> 1);
            w_u32(state + 0x18, reduced);
            uint32 magnitude = (sint32)reduced < 0 ? 0 - reduced : reduced;
            if ((sint32)magnitude < 4)
                w_u32(state + 0x18, 0);
            rotation = r_u16(state + 0x18);
        }
    }
    result = rotation << 16;
    sint32 signed_rotation = (sint32)result >> 16;
    if (signed_rotation == 0)
        return result;
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 current = r_u16(position + axis * 2);
        uint32 center = r_u16(pivot + axis * 2);
        w_u16(frame + 0x10 + axis * 2, current - center);
    }
    G3Function8002F8E4(frame + 0x10, (uint32)signed_rotation);
    uint32 x = r_u16(frame + 0x10);
    uint32 center_x = r_u16(pivot);
    uint32 rotated_x = x + center_x;
    uint32 y = r_u16(frame + 0x12);
    w_u16(frame + 0x18, rotated_x);
    uint32 center_y = r_u16(pivot + 2);
    w_u16(frame + 0x1a, y + center_y);
    uint32 z = r_u16(frame + 0x14);
    uint32 center_z = r_u16(pivot + 4);
    w_u16(frame + 0x1c, z + center_z);
    uint32 old_x = r_u16(position);
    w_u16(output, rotated_x - old_x);
    y = r_u16(frame + 0x1a);
    uint32 old_y = r_u16(position + 2);
    w_u16(output + 2, y - old_y);
    z = r_u16(frame + 0x1c);
    uint32 old_z = r_u16(position + 4);
    result = z - old_z;
    w_u16(output + 4, result);
    return result;
}

uint32 G3Function800536AC(uint32 position, uint32 state, uint32 bounds, uint32 delta, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800536ac, "SLUS_008.06");
    uint32 frame = guest_stack - 0x58;
    uint32 game = r_u32(0x80097048);
    w_u16(delta, 0);
    w_u16(delta + 2, 0);
    w_u16(delta + 4, 0);
    w_u16(frame + 0x28, 0);
    w_u16(frame + 0x2a, 0);
    w_u16(frame + 0x2c, 0);
    uint32 controls = game + 0x70;
    G3Function8005309C(position, state, bounds, delta, frame);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 current = r_u16(position + axis * 2);
        uint32 change = r_u16(delta + axis * 2);
        w_u16(frame + 0x18 + axis * 2, current + change);
    }
    if (r_u32(0x80096b54) != 0 && G3Function80052E74(state, frame + 0x18, 0x800a2558, frame) != 0)
    {
        G3Function80053E00(position, state, bounds + 0x20, frame + 0x28, frame);
        uint32 x = r_u16(position);
        uint32 change_x = r_u16(frame + 0x28);
        uint32 change_y = r_u16(frame + 0x2a);
        w_u16(frame + 0x18, x + change_x);
        uint32 y = r_u16(position + 2);
        w_u16(frame + 0x1a, y + change_y);
        uint32 z = r_u16(position + 4);
        uint32 change_z = r_u16(frame + 0x2c);
        w_u16(frame + 0x1c, z + change_z);
        w_u16(delta, change_x);
        change_y = r_u16(frame + 0x2a);
        w_u16(delta + 2, change_y);
        change_z = r_u16(frame + 0x2c);
        w_u16(delta + 4, change_z);
    }
    uint32 options = r_u32(0x8009ea44);
    uint32 result = options & 0x100;
    if ((options & 1) != 0)
        return result;
    if (result != 0)
    {
        result = r_u32(controls) & 2;
        if (result != 0)
            return result;
    }
    uint32 flags = r_u32(0x800a24d0);
    bool active = (flags & 0x40) != 0;
    if (!active)
        active = r_u32(0x800a24e8) != 0;
    if (!active)
    {
        w_u32(0x800a24d0, flags & 0xffffca1f);
        uint32 query = G3Function80052B10(0, state, frame + 0x18, frame);
        sint32 side = (sint32)(query << 16) >> 16;
        if (side == 0)
            return G3Function800524E4();
        if (side == 3)
        {
            flags = r_u32(0x800a24d0);
            w_u32(0x800a24d0, flags | 0x800);
        }
        if (r_s16(delta) == 0 && r_s16(delta + 2) == 0 && r_s16(delta + 4) == 0)
            return G3Function80052CE8(0x800a2504, (uint32)side);
        uint32 x = r_u16(delta);
        uint32 y = r_u16(delta + 2);
        w_u16(delta, (uint32)((sint32)(x << 16) >> 17));
        uint32 z = r_u16(delta + 4);
        w_u16(delta + 2, (uint32)((sint32)(y << 16) >> 17));
        w_u16(delta + 4, (uint32)((sint32)(z << 16) >> 17));
        result = r_u32(bounds + 0x1c) & 0xffffffc3;
        w_u32(bounds + 0x1c, result);
        return result;
    }
    if ((options & 0x400) != 0)
    {
        result = r_u32(0x800a24c0);
        if (result == 0)
            return result;
    }
    uint32 query = G3Function80052B10(0, state, frame + 0x18, frame);
    sint32 side = (sint32)(query << 16) >> 16;
    if (side == 3)
    {
        flags = r_u32(0x800a24d0);
        w_u32(0x800a24d0, flags | 0x800);
    }
    if (side == 0)
    {
        w_u32(0x800a24e8, 0);
        return G3Function800524E4();
    }
    uint32 count = r_u32(0x800a2504);
    flags = r_u32(0x800a24d0);
    w_u32(0x800a2504, count + 1);
    if ((flags & 0x40) != 0)
    {
        G3Function80052BAC(0x800a2504, frame);
        sint32 search_angle = r_s16(0x800a2508);
        query = G3Function80052B10((uint32)search_angle, state, frame + 0x18, frame);
        if ((query << 16) == 0)
        {
            uint32 object = r_u32(0x80096b34);
            uint32 handler = r_u32(object + 0xec);
            if (handler != 0)
            {
                uint32 current_game = r_u32(0x80097048);
                G3DispatchStateEvent(handler, object, current_game, 0x7e8, frame + 0x30, frame);
            }
            G3Function80052D44(0x800a2504, state, position, frame + 0x30, frame);
            flags = r_u32(0x800a24d0);
            w_u32(0x800a24d0, flags & 0xffffffbf);
        }
    }
    uint32 difference[3];
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 current = r_u16(position + axis * 2);
        uint32 center = r_u16(state + axis * 2);
        difference[axis] = current - center;
        w_u16(frame + 0x10 + axis * 2, difference[axis]);
    }
    uint32 x_bits = (uint32)((sint32)(difference[0] << 16) >> 16);
    uint32 y_bits = (uint32)((sint32)(difference[1] << 16) >> 16);
    uint32 current_angle = G3Function8002F240(y_bits, x_bits, frame);
    uint32 desired = r_u16(0x800a2518);
    uint32 difference_angle = (desired - current_angle) & 0xfff;
    sint32 turn = difference_angle < 0x800 ? (sint32)difference_angle : (sint32)difference_angle - 0x1000;
    sint32 magnitude = turn < 0 ? -turn : turn;
    sint32 old_speed = r_s16(0x800a24e8);
    uint32 step_bits = G3Function80064680((uint32)magnitude, (uint32)old_speed, 2, frame);
    sint32 step = (sint32)(step_bits << 16) >> 16;
    uint32 speed = r_u32(0x800a24e8) + (uint32)step;
    w_u32(0x800a24e8, speed);
    if ((sint32)speed < -20)
        speed = (uint32)-20;
    if ((sint32)speed >= 21)
        speed = 20;
    w_u32(0x800a24e8, speed);
    if (step == 0)
    {
        uint32 absolute = (sint32)speed < 0 ? 0 - speed : speed;
        result = (sint32)absolute < 10 ? 1 : 0;
        if (result != 0)
            return result;
    }
    flags = r_u32(0x800a24d0);
    result = flags & 0x2000;
    if ((flags & 0xc00) == 0xc00 || result != 0)
        return result;
    uint32 rotation = r_u16(0x800a24e8);
    if (turn > 0)
    {
        w_u32(0x800a24d0, flags | 0x10);
        rotation = 0 - rotation;
    }
    else
        w_u32(0x800a24d0, flags | 8);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 point = r_u16(frame + 0x18 + axis * 2);
        uint32 center = r_u16(state + axis * 2);
        w_u16(frame + 0x10 + axis * 2, point - center);
    }
    uint32 signed_rotation = (uint32)((sint32)(rotation << 16) >> 16);
    G3Function8002F8E4(frame + 0x10, signed_rotation);
    uint32 x = r_u16(frame + 0x10);
    uint32 center_x = r_u16(state);
    uint32 y = r_u16(frame + 0x12);
    uint32 rotated_x = x + center_x;
    w_u16(frame + 0x18, rotated_x);
    uint32 center_y = r_u16(state + 2);
    uint32 z = r_u16(frame + 0x14);
    uint32 rotated_y = y + center_y;
    w_u16(frame + 0x1a, rotated_y);
    uint32 center_z = r_u16(state + 4);
    uint32 rotated_z = z + center_z;
    w_u16(frame + 0x1c, rotated_z);
    uint32 old_x = r_u16(position);
    uint32 change_x = rotated_x - old_x;
    w_u16(frame + 0x20, change_x);
    uint32 old_y = r_u16(position + 2);
    uint32 change_y = rotated_y - old_y;
    w_u16(frame + 0x22, change_y);
    uint32 old_z = r_u16(position + 4);
    uint32 change_z = rotated_z - old_z;
    w_u16(frame + 0x24, change_z);
    uint32 previous = r_u16(delta);
    w_u16(delta, previous + change_x);
    previous = r_u16(delta + 2);
    change_y = r_u16(frame + 0x22);
    w_u16(delta + 2, previous + change_y);
    previous = r_u16(delta + 4);
    change_z = r_u16(frame + 0x24);
    result = previous + change_z;
    w_u16(delta + 4, result);
    return result;
}

uint32 G3Function80053E00(uint32 position, uint32 pivot, uint32 state, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80053e00, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    w_u16(output, 0);
    w_u16(output + 2, 0);
    w_u16(output + 4, 0);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 target = r_u16(pivot + axis * 2);
        uint32 current = r_u16(position + axis * 2);
        w_u16(frame + 0x10 + axis * 2, target - current);
    }
    uint32 length = G3Function8002FA50(frame + 0x10, frame);
    if ((sint32)length < 801)
    {
        uint32 result = r_u32(0x800a2574) | 1;
        w_u32(0x800a2574, result);
        return result;
    }
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 target = r_u16(state + axis * 2);
        uint32 current = r_u16(position + axis * 2);
        w_u16(frame + 0x10 + axis * 2, target - current);
    }
    length = G3Function8002FA50(frame + 0x10, frame);
    if ((sint32)length < 0)
        length = 0;
    uint32 result = (sint32)length < 61 ? 1 : 0;
    if (result == 0)
        length = 60;
    if (length == 0)
        return result;
    G3Function8002F9A8(frame + 0x10, length, frame + 0x10, frame);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        result = r_u16(frame + 0x10 + axis * 2);
        w_u16(output + axis * 2, result);
    }
    return result;
}

uint32 G3Function80052E74(uint32 pivot, uint32 point, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80052e74, "SLUS_008.06");
    uint32 frame = guest_stack - 0x80;
    uint32 options = r_u32(0x8009ea44);
    if ((options & 0x100000) != 0)
        return 0;
    if (r_u32(settings + 0x18) == 0)
        return 0;
    uint32 object = r_u32(0x80096b34);
    uint32 handler = r_u32(object + 0xec);
    if (handler == 0)
        return 0;
    uint32 game = r_u32(0x80097048);
    if (G3DispatchStateEvent(handler, object, game, 0x7e8, frame + 0x28, frame) == 0)
        return 0;
    uint32 source = frame + 0x28;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
    uint32 low_lane = source & 3;
    uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
    uint32 copied = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    sint32 tail = r_s16(frame + 0x2c);
    uint32 destination = frame + 0x20;
    high_address = destination + 3;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, copied >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    low_lane = destination & 3;
    w_masked_u32(destination & 0xfffffffc, copied << (low_lane * 8), (15 << low_lane) & 15);
    w_u16(frame + 0x24, (uint32)tail);
    uint32 radius = r_u16(frame + 0x2e);
    G3Function80056C10(frame + 0x58, frame + 0x60, pivot, point, frame);
    uint32 x = r_u16(frame + 0x20);
    uint32 point_x = r_u16(point);
    w_u16(frame + 0x50, x - point_x);
    uint32 y = r_u16(frame + 0x22);
    uint32 point_y = r_u16(point + 2);
    w_u16(frame + 0x54, 0);
    w_u16(frame + 0x52, y - point_y);
    uint32 signed_radius = (uint32)((sint32)(radius << 16) >> 16);
    G3Function8002F9A8(frame + 0x30, signed_radius, frame + 0x50, frame);
    uint32 length = r_u16(frame + 0x60) + 50;
    length = (uint32)((sint32)(length << 16) >> 16);
    G3Function8002F9A8(frame + 0x38, length, frame + 0x50, frame);
    sint32 direction = r_s32(settings + 0x18);
    uint32 angle = direction > 0 ? 0xfffffc00 : 0x400;
    G3Function8002F8E4(frame + 0x30, angle);
    G3Function8002F8E4(frame + 0x38, angle);
    x = r_u16(frame + 0x30);
    uint32 base_x = r_u16(frame + 0x20);
    uint32 base_y = r_u16(frame + 0x22);
    uint32 base_z = r_u16(frame + 0x24);
    w_u16(frame + 0x48, x + base_x);
    y = r_u16(frame + 0x32);
    uint32 z = r_u16(frame + 0x34);
    w_u16(frame + 0x4a, y + base_y);
    x = r_u16(frame + 0x38);
    base_x = r_u16(frame + 0x58);
    w_u16(frame + 0x4c, z + base_z);
    y = r_u16(frame + 0x3a);
    base_y = r_u16(frame + 0x5a);
    w_u16(frame + 0x40, x + base_x);
    z = r_u16(frame + 0x3c);
    w_u16(frame + 0x42, y + base_y);
    base_z = r_u16(frame + 0x5c);
    game = r_u32(0x80097048);
    w_u16(frame + 0x44, z + base_z);
    uint32 owner = r_u32(game + 0x48);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0x1d529e8);
    w_u32(frame + 0x18, 0);
    uint32 world = r_u32(owner);
    uint32 result = G3Function8006238C(world, frame + 0x40, frame + 0x48, 1, frame);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 coordinate = r_u16(frame + 0x40 + axis * 2);
        w_u16(settings + 0x20 + axis * 2, coordinate);
    }
    return result != 0 ? 1 : 0;
}

uint32 G3Function80056C10(uint32 projected, uint32 length, uint32 origin, uint32 target, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80056c10, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    w_u16(frame + 0x10, 0);
    w_u16(frame + 0x12, 0);
    w_u16(frame + 0x14, 0);
    uint32 root = G3Function8002F368(0x16400000, frame);
    uint32 magnitude = (uint32)((sint32)root >> 12);
    uint32 angle = G3Function8002F240(256, 320, frame);
    uint32 doubled_angle = (uint32)((sint32)(angle << 17) >> 16);
    uint32 sine = G3Function8005B590(doubled_angle);
    uint32 normalized = magnitude;
    if (sine != 0)
    {
        uint32 numerator = magnitude << 12;
        normalized = (uint32)((sint64)(sint32)numerator / (sint32)sine);
    }
    w_u16(length, normalized);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 start = r_u16(origin + axis * 2);
        uint32 finish = r_u16(target + axis * 2);
        w_u16(frame + 0x10 + axis * 2, start - finish);
    }
    sint32 signed_length = r_s16(length);
    G3Function8002F9A8(frame + 0x10, (uint32)signed_length, frame + 0x10, frame);
    uint32 result = 0;
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 component = r_u16(frame + 0x10 + axis * 2);
        uint32 finish = r_u16(target + axis * 2);
        result = component + finish;
        w_u16(projected + axis * 2, result);
    }
    return result;
}

uint32 G3Function80053C78(uint32 position, uint32 object, uint32 state, uint32 delta, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80053c78, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    w_u16(delta, 0);
    w_u16(delta + 2, 0);
    w_u16(delta + 4, 0);
    uint32 result = r_u32(0x8009ea44) & 0x4000;
    bool enabled = false;
    if (result == 0)
    {
        uint32 count = r_u32(0x800a2504);
        sint32 threshold = r_s32(0x8009ead0);
        if ((sint32)count >= threshold)
            enabled = true;
        else
        {
            uint32 flags = r_u32(0x800a24d0);
            if ((flags & 0x400) != 0)
            {
                threshold = r_s32(0x8009eacc);
                if ((sint32)count >= threshold && (flags & 1) == 0)
                    enabled = true;
            }
        }
        result = r_u32(0x800a24d0) & 0x1000;
        if (result == 0)
            enabled = false;
    }
    if (!enabled)
        return result;
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 base = r_u16(state + axis * 2);
        uint32 offset = r_u16(0x800a251c + axis * 2);
        w_u16(frame + 0x18 + axis * 2, base + offset);
    }
    uint32 query = G3Function80052B10(0, state, frame + 0x18, frame);
    if (query != 0)
    {
        uint32 owner = r_u32(0x80096b34);
        w_u32(frame + 0x14, 0);
        uint32 source = owner + 0x60;
        w_u32(frame + 0x10, source);
        G3Function80054034(frame + 0x18, frame + 0x20, frame + 0x18, state, source, 0, frame);
    }
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 target = r_u16(frame + 0x18 + axis * 2);
        uint32 current = r_u16(position + axis * 2);
        result = target - current;
        w_u16(delta + axis * 2, result);
    }
    return result;
}

uint32 G3Function80054034(uint32 first, uint32 second, uint32 camera, uint32 context, uint32 source, uint32 mode, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80054034, "SLUS_008.06");
    uint32 frame = guest_stack - 0x70;
    uint32 copied[2];
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 address = source + word * 4;
        uint32 high_address = address + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low_lane = address & 3;
        uint32 low = r_masked_u32(address & 0xfffffffc, (15 << low_lane) & 15);
        copied[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 address = second + word * 4;
        uint32 high_address = address + 3;
        uint32 high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, copied[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        uint32 low_lane = address & 3;
        w_masked_u32(address & 0xfffffffc, copied[word] << (low_lane * 8), (15 << low_lane) & 15);
    }
    if (mode != 0)
    {
        uint32 differences[3];
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 current = r_u16(camera + axis * 2);
            uint32 base = r_u16(context + axis * 2);
            differences[axis] = current - base;
            w_u16(frame + 0x20 + axis * 2, differences[axis]);
        }
        uint32 x = (uint32)((sint32)(differences[0] << 16) >> 16);
        uint32 y = (uint32)((sint32)(differences[1] << 16) >> 16);
        uint32 heading = G3Function8002F240(y, x, frame) + 0x400;
        uint32 orientation = r_u16(source + 4);
        orientation = (orientation + 0x800) & 0xfff;
        uint32 turn = G3Function80038B48(heading & 0xfff, orientation);
        uint32 signed_turn = (uint32)((sint32)(turn << 16) >> 16);
        G3Function8002F8E4(frame + 0x20, signed_turn);
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 base = r_u16(context + axis * 2);
            uint32 component = r_u16(frame + 0x20 + axis * 2);
            w_u16(frame + 0x18 + axis * 2, base + component);
        }
        w_u16(second + 4, orientation);
    }
    else
    {
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 component = r_u16(camera + axis * 2);
            w_u16(frame + 0x18 + axis * 2, component);
        }
        w_u16(second, 0);
        w_u16(second + 2, 0);
        w_u16(second + 4, 0);
    }
    uint32 x = r_u16(context);
    uint32 game = r_u32(0x80097048);
    w_u16(frame + 0x38, x);
    uint32 y = r_u16(context + 2);
    w_u16(frame + 0x3a, y);
    uint32 z = r_u16(context + 4);
    w_u16(frame + 0x3c, z);
    uint32 owner = r_u32(game + 0x48);
    uint32 world = r_u32(owner);
    uint32 hit = G3Function80062358(world, frame + 0x18, frame + 0x38, 0, frame);
    uint32 high_address = context + 3;
    uint32 high_lane = high_address & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
    uint32 low_lane = context & 3;
    uint32 low = r_masked_u32(context & 0xfffffffc, (15 << low_lane) & 15);
    uint32 word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    sint32 tail = r_s16(context + 4);
    uint32 destination = frame + 0x48;
    high_address = destination + 3;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    low_lane = destination & 3;
    w_masked_u32(destination & 0xfffffffc, word << (low_lane * 8), (15 << low_lane) & 15);
    w_u16(frame + 0x4c, (uint32)tail);
    G3Function80056EA0(frame + 0x48, context, frame + 0x18, frame + 0x28, frame);
    if (r_s16(frame + 0x28) != 0 || r_s16(frame + 0x2a) != 0 || r_s16(frame + 0x2c) != 0)
    {
        G3Function80056C10(frame + 0x40, frame + 0x50, context, frame + 0x18, frame);
        w_u32(frame + 0x10, frame + 0x30);
        G3Function80056D30(frame + 0x18, context, frame + 0x40, frame + 0x28, frame + 0x30, frame);
        hit = 1;
        uint32 x = r_u16(frame + 0x18);
        uint32 dx = r_u16(frame + 0x30);
        uint32 dy = r_u16(frame + 0x32);
        uint32 dz = r_u16(frame + 0x34);
        w_u16(frame + 0x18, x + dx);
        uint32 y = r_u16(frame + 0x1a);
        uint32 z = r_u16(frame + 0x1c);
        w_u16(frame + 0x1a, y + dy);
        w_u16(frame + 0x1c, z + dz);
    }
    G3Function80014F38(frame + 0x18, frame + 0x30, frame);
    if (r_s16(frame + 0x30) != 0 || r_s16(frame + 0x32) != 0 || r_s16(frame + 0x34) != 0)
    {
        uint32 x = r_u16(frame + 0x18);
        uint32 dx = r_u16(frame + 0x30);
        uint32 dy = r_u16(frame + 0x32);
        uint32 dz = r_u16(frame + 0x34);
        w_u16(frame + 0x18, x + dx);
        uint32 y = r_u16(frame + 0x1a);
        uint32 z = r_u16(frame + 0x1c);
        w_u16(frame + 0x1a, y + dy);
        w_u16(frame + 0x1c, z + dz);
    }
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 component = r_u16(frame + 0x18 + axis * 2);
        w_u16(first + axis * 2, component);
    }
    if (hit != 0)
    {
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 component = r_u16(frame + 0x18 + axis * 2);
            uint32 base = r_u16(context + axis * 2);
            w_u16(frame + 0x20 + axis * 2, component - base);
        }
        uint32 length = G3Function8002FA50(frame + 0x20, frame);
        if ((sint32)length < 500)
            return 0;
    }
    return 1;
}

uint32 G3Function80056EA0(uint32 output, uint32 start, uint32 end, uint32 delta, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80056ea0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    w_u16(delta, 0);
    w_u16(delta + 2, 0);
    w_u16(delta + 4, 0);
    G3Function80056C10(frame + 0x20, frame + 0x40, start, end, frame);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 first = r_u16(output + axis * 2);
        uint32 second = r_u16(end + axis * 2);
        w_u16(frame + 0x38 + axis * 2, first - second);
    }
    uint32 length = G3Function8002FA50(frame + 0x38, frame);
    if ((sint32)length >= 513)
        G3Function8002F9A8(frame + 0x38, 512, frame + 0x38, frame);
    uint32 x = r_u16(frame + 0x38);
    uint32 base_x = r_u16(frame + 0x20);
    uint32 y = r_u16(frame + 0x3a);
    uint32 base_y = r_u16(frame + 0x22);
    w_u16(frame + 0x30, x + base_x);
    uint32 z = r_u16(frame + 0x3c);
    uint32 base_z = r_u16(frame + 0x24);
    w_u16(frame + 0x32, y + base_y);
    uint32 flags = r_u32(0x80096b28);
    w_u16(frame + 0x34, z + base_z);
    uint32 mask = (flags & 0x400) != 0 ? 0x1d52be8 : 0x1d529e8;
    uint32 radius = r_u16(frame + 0x40);
    uint32 game = r_u32(0x80097048);
    w_u16(0x800a24d4, 0);
    w_u16(0x800a24d6, 0);
    w_u32(frame + 0x10, 0);
    uint32 owner = r_u32(game + 0x48);
    uint32 world = r_u32(owner);
    w_u32(frame + 0x18, mask);
    w_u32(frame + 0x1c, 0);
    w_u32(frame + 0x14, world);
    G3Function80061078(frame + 0x28, frame + 0x20, frame + 0x30, radius, frame);
    uint32 result = 0;
    if (r_s16(frame + 0x28) != 0 || r_s16(frame + 0x2a) != 0 || r_s16(frame + 0x2c) != 0)
    {
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            result = r_u16(frame + 0x28 + axis * 2);
            w_u16(delta + axis * 2, result);
        }
    }
    return result;
}

uint32 G3Function80056D30(uint32 point, uint32 origin, uint32 projected, uint32 normal, uint32 delta, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80056d30, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 base = r_u16(origin + axis * 2);
        uint32 current = r_u16(point + axis * 2);
        w_u16(frame + 0x10 + axis * 2, base - current);
    }
    uint32 length = G3Function8002FA50(frame + 0x10, frame);
    uint32 sum[3];
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 offset = r_u16(normal + axis * 2);
        uint32 base = r_u16(projected + axis * 2);
        sum[axis] = offset + base;
        w_u16(frame + 0x28 + axis * 2, sum[axis]);
    }
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 base = r_u16(origin + axis * 2);
        w_u16(frame + 0x18 + axis * 2, sum[axis] - base);
    }
    uint32 signed_length = (uint32)((sint32)(length << 16) >> 16);
    G3Function8002F9A8(frame + 0x18, signed_length, frame + 0x18, frame);
    uint32 x = r_u16(frame + 0x18);
    uint32 base_x = r_u16(origin);
    uint32 corrected_x = x + base_x;
    uint32 y = r_u16(frame + 0x1a);
    w_u16(frame + 0x20, corrected_x);
    uint32 base_y = r_u16(origin + 2);
    w_u16(frame + 0x22, y + base_y);
    uint32 z = r_u16(frame + 0x1c);
    uint32 base_z = r_u16(origin + 4);
    w_u16(frame + 0x24, z + base_z);
    uint32 old_x = r_u16(point);
    w_u16(delta, corrected_x - old_x);
    y = r_u16(frame + 0x22);
    uint32 old_y = r_u16(point + 2);
    w_u16(delta + 2, y - old_y);
    z = r_u16(frame + 0x24);
    uint32 old_z = r_u16(point + 4);
    uint32 result = z - old_z;
    w_u16(delta + 4, result);
    return result;
}

uint32 G3Function80057040(uint32 object, uint32 state, uint32 position, uint32 delta, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80057040, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    w_u16(delta, 0);
    w_u16(delta + 2, 0);
    w_u16(delta + 4, 0);
    uint32 result = r_u32(0x8009ea44) & 2;
    if (result != 0)
        return result;
    G3Function80056EA0(object, state, position, delta, frame);
    if (r_s16(delta) == 0 && r_s16(delta + 2) == 0 && r_s16(delta + 4) == 0)
        return 0;
    uint32 difference[3];
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 current = r_u16(object + axis * 2);
        uint32 base = r_u16(position + axis * 2);
        difference[axis] = current - base;
        w_u16(frame + 0x18 + axis * 2, difference[axis]);
    }
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 component = r_u16(delta + axis * 2);
        w_u16(frame + 0x10 + axis * 2, component);
    }
    sint32 x = (sint32)(difference[0] << 16) >> 16;
    sint32 y = (sint32)(difference[1] << 16) >> 16;
    uint32 toward = G3Function8002F240((uint32)-y, (uint32)-x, frame);
    sint32 delta_y = r_s16(frame + 0x12);
    sint32 delta_x = r_s16(frame + 0x10);
    uint32 away = G3Function8002F240((uint32)delta_y, (uint32)delta_x, frame);
    uint32 signed_toward = (uint32)((sint32)(toward << 16) >> 16);
    uint32 signed_away = (uint32)((sint32)(away << 16) >> 16);
    uint32 angle = G3Function80038B48(signed_toward, signed_away) & 0xfff;
    w_u16(0x800a24d4, angle);
    if (angle >= 0x800)
        w_u16(0x800a24d4, angle - 0x1000);
    delta_y = r_s16(delta + 2);
    delta_x = r_s16(delta);
    away = G3Function8002F240((uint32)-delta_y, (uint32)-delta_x, frame);
    signed_away = (uint32)((sint32)(away << 16) >> 16);
    angle = G3Function80038B48(signed_toward, signed_away) & 0xfff;
    w_u16(0x800a24d6, angle);
    if (angle >= 0x800)
        w_u16(0x800a24d6, angle - 0x1000);
    uint32 flags = r_u32(0x800a24d0);
    uint32 extra = r_u32(0x8009ef9c);
    w_u32(0x800a24d0, flags | 2);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 component = r_u16(delta + axis * 2);
        w_u16(0x800a24d8 + axis * 2, component);
    }
    result = flags | 6;
    if (extra != 0)
        w_u32(0x800a24d0, result);
    return result;
}

uint32 G3Function8005FC70(uint32 output, uint32 start, uint32 end, uint32 normal)
{
    FUNCTION_MARKER(0x8005fc70, "SLUS_008.06");
    uint32 x = r_u16(end);
    uint32 base_x = r_u16(start);
    uint32 dx = x - base_x;
    w_u16(0x1f800020, dx);
    uint32 y = r_u16(end + 2);
    uint32 base_y = r_u16(start + 2);
    uint32 dy = y - base_y;
    w_u16(0x1f800022, dy);
    uint32 z = r_u16(end + 4);
    uint32 base_z = r_u16(start + 4);
    w_u16(0x1f800000, dx);
    w_u16(0x1f800002, dy);
    uint32 dz = z - base_z;
    w_u16(0x1f800024, dz);
    w_u16(0x1f800004, dz);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 component = r_u16(start + axis * 2);
        w_u16(0x1f800006 + axis * 2, component);
    }
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 component = r_u16(normal + axis * 2);
        w_u16(0x1f80000c + axis * 2, component);
    }
    G3Function8005ADA0(0x1f800000, 0x1f800020, 0x1f800030);
    uint32 first = r_u32(0x1f800034);
    uint32 second = r_u32(0x1f800038);
    uint32 denominator = r_u32(0x1f800030);
    uint32 factor = (first - second) << 12;
    if (denominator != 0)
    {
        uint32 numerator = 0 - factor;
        factor = (uint32)((sint64)(sint32)numerator / (sint32)denominator);
    }
    if ((sint32)factor <= 0)
    {
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 component = r_u16(start + axis * 2);
            w_u16(output + axis * 2, component);
        }
    }
    else if ((sint32)factor >= 4096)
    {
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 component = r_u16(end + axis * 2);
            w_u16(output + axis * 2, component);
        }
    }
    else
    {
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            sint32 component;
            if (axis == 0)
                component = r_s16(0x1f800020);
            else
            {
                uint32 raw = r_u16(0x1f800020 + axis * 2);
                component = (sint32)(raw << 16) >> 16;
            }
            uint32 product = (uint32)component * factor;
            uint32 base = r_u16(start + axis * 2);
            uint32 value = base + (uint32)((sint32)product >> 12);
            w_u16(output + axis * 2, value);
        }
    }
    return factor;
}
