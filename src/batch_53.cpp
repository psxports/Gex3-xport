#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32 G3MstrdbgCountEntries81A4(uint32 index, uint32 settings)
{
    FUNCTION_MARKER(0x000081a4, "G3L_39");
    uint32 table = r_u32(settings + 8);
    if (table == 0)
        return 0;
    uint32 entry = r_u32(table + index * 4);
    if (entry == 0)
        return 0;
    uint32 cursor = entry + 8;
    uint32 value = r_u32(cursor);
    uint32 count = 0;
    while (value != 0)
    {
        cursor += 4;
        value = r_u32(cursor);
        ++count;
    }
    return count;
}

uint32 G3PirateEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_41");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1: G3PirateInitialize(object, frame); return 1;
    case 2: G3PirateUpdate(object, game, module_base, frame); return 1;
    case 3: G3PirateCollide(object, game, module_base, frame); return 1;
    case 9: G3PirateReact(object); return 1;
    default: return 0;
    }
}

void G3PirateConfigure(uint32 object, uint32 commands, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000a0, "G3L_41");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    w_u32(frame + 0x2c, commands);
    if (commands == 0) return;
    while (G3Function80085DFC(frame + 0x2c, frame + 0x10, frame + 0x14, frame + 0x18) != 0)
    {
        sint32 kind = r_s16(frame + 0x10);
        switch (kind)
        {
        case 1:
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 value = (uint32)r_s16(data);
            w_u32(state + 8, value * value);
            value = (uint32)r_s16(data + 2);
            w_u32(state + 0xc, value * value);
            value = (uint32)r_s16(data + 4);
            w_u32(state, value * value);
            value = (uint32)r_s16(data + 6);
            w_u32(state + 4, value * value);
            break;
        }
        case 0x28: w_u32(state + 0x10, r_u32(frame + 0x14)); break;
        case 0x29:
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0x20, r_u16(data));
            break;
        }
        case 0x2a:
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0x22, r_u16(data));
            break;
        }
        case 0x2b:
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0x24, r_u16(data));
            break;
        }
        case 0x2c:
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0x26, r_u16(data));
            break;
        }
        case 0x2d:
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0x28, r_u16(data));
            break;
        }
        case 0x2e:
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0x2a, r_u16(data));
            break;
        }
        case 0x2f:
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0x2c, r_u16(data));
            break;
        }
        case 0x30:
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0x2e, r_u16(data));
            break;
        }
        case 0x31:
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0x34, r_u16(data));
            break;
        }
        case 0x32:
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0x36, r_u16(data));
            break;
        }
        case 0x33:
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0x38, r_u16(data));
            break;
        }
        case 0x99:
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0x30, r_u16(data));
            break;
        }
        default: break;
        }
    }
}

void G3PirateInitialize(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000002e8, "G3L_41");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    w_u32(state + 8, 0x15f900);
    w_u32(state + 0xc, 0x225510);
    w_u32(state, 0x6f3c90);
    w_u32(state + 4, 0xd0e490);
    w_u16(state + 0x22, 180);
    w_u16(state + 0x24, 75);
    w_u16(state + 0x26, 8);
    w_u16(state + 0x2c, 15);
    w_u16(state + 0x2e, 1800);
    w_u16(state + 0x28, 90);
    w_u16(state + 0x30, 90);
    w_u16(state + 0x34, 45);
    w_u16(state + 0x36, 10);
    w_u32(state + 0x10, 0);
    w_u16(state + 0x20, 30);
    w_u16(state + 0x2a, 30);
    w_u16(state + 0x38, 2);
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 commands = r_u32(descriptor + 0x1c);
    G3PirateConfigure(object, commands, frame);
    commands = r_u32(object + 0x28);
    G3PirateConfigure(object, commands, frame);
    w_u32(object + 0x150, 2);
    w_u16(object + 0x6e, 0);
    w_u16(object + 0x5e, 0);
    w_u16(state + 0x1e, 0);
    w_u16(state + 0x1c, 0);
    w_u32(object + 0x10, r_u32(object + 0x10) | 0x100400);
    w_u16(state + 0x18, 0);
    w_u32(state + 0x18, r_u16(state + 0x18));
    uint32 path = G3Function800833EC(object);
    w_u32(state + 0x14, path);
}

uint32 G3PirateNearbyBall(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000404, "G3L_41");
    uint32 frame = guest_stack - 8;
    uint32 list = r_u32(game + 0x5c);
    uint32 other = r_u32(list + 0xc8);
    if (other == 0) return 0;
    uint32 expected_first = r_u32(module_base + 0x1878);
    do
    {
        uint32 descriptor = r_u32(other + 0x1c);
        uint32 name = r_u32(descriptor + 0x20);
        uint32 first = r_u32(name);
        if (first == expected_first)
        {
            uint32 second = r_u32(name + 4);
            uint32 expected_second = r_u32(module_base + 0x187c);
            if (second == expected_second)
            {
                uint32 x = r_u16(other + 0x58);
                uint32 origin = r_u16(object + 0x58);
                x -= origin;
                uint32 sx = (uint32)(sint32)(sint16)x;
                uint32 xx = sx * sx;
                w_u16(frame, x);
                uint32 y = r_u16(other + 0x5a);
                origin = r_u16(object + 0x5a);
                y -= origin;
                uint32 sy = (uint32)(sint32)(sint16)y;
                uint32 yy = sy * sy;
                w_u16(frame + 2, y);
                uint32 z = r_u16(other + 0x5c);
                origin = r_u16(object + 0x5c);
                z -= origin;
                uint32 sz = (uint32)(sint32)(sint16)z;
                uint32 zz = sz * sz;
                sint32 distance = (sint32)(xx + yy + zz);
                w_u16(frame + 4, z);
                if (distance <= 0x225510) return 1;
            }
        }
        other = r_u32(other + 4);
    } while (other != 0);
    return 0;
}

void G3PirateReact(uint32 object)
{
    FUNCTION_MARKER(0x000017b4, "G3L_41");
    uint32 mode = r_u32(object + 0x150);
    uint32 state = object + 0x104;
    if (mode < 3)
    {
        w_u32(object + 0x150, 6);
        w_u16(object + 0x6e, 4);
        w_u16(object + 0x5e, 0);
        w_u16(state + 0x32, 0);
        return;
    }
    if (mode != 6) return;
    if (r_s16(state + 0x32) != 0)
    {
        w_u16(state + 0x1c, r_u16(state + 0x28));
        return;
    }
    if (r_u16(object + 0x6e) != 11) return;
    sint32 current = r_s16(object + 0x5e);
    w_u16(object + 0x6e, 4);
    if (current == 0)
    {
        uint32 time = r_u16(state + 0x28);
        w_u16(state + 0x32, 1);
        w_u16(state + 0x1c, time);
    }
    else w_u16(state + 0x32, 0);
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 animation = r_u16(object + 0x6e);
    uint32 animations = r_u32(descriptor + 0x10);
    uint32 data = r_u32(animations + animation * 4);
    uint32 frame = r_u16(object + 0x5e);
    uint32 count = r_u16(data + 2);
    w_u16(object + 0x5e, count - (frame + 1));
}

void G3PirateUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000504, "G3L_41");
    uint32 frame = guest_stack - 0x50;
    uint32 mode = r_u32(object + 0x150);
    uint32 player = r_u32(game + 0x50);
    uint32 state = object + 0x104;
    uint32 next_animation = 0;
    switch (mode)
    {
    case 0:
    {
        w_u16(state + 0x1c, r_u16(state + 0x1c) + 1);
        w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 player_x = r_u16(player + 0x58);
        uint32 object_x = r_u16(object + 0x58);
        uint32 dx = player_x - object_x;
        uint32 signed_x = (uint32)(sint32)(sint16)dx;
        uint32 square_x = signed_x * signed_x;
        w_u16(frame + 0x18, dx);
        uint32 player_y = r_u16(player + 0x5a);
        uint32 object_y = r_u16(object + 0x5a);
        uint32 dy = player_y - object_y;
        uint32 signed_y = (uint32)(sint32)(sint16)dy;
        uint32 square_y = signed_y * signed_y;
        w_u16(frame + 0x1a, dy);
        uint32 player_z = r_u16(player + 0x5c);
        uint32 object_z = r_u16(object + 0x5c);
        uint32 dz = player_z - object_z;
        uint32 signed_z = (uint32)(sint32)(sint16)dz;
        uint32 square_z = signed_z * signed_z;
        w_u16(frame + 0x1c, dz);
        sint32 distance = (sint32)(square_x + square_y + square_z);
        if (distance <= r_s32(state))
        {
            w_u32(object + 0x150, 3);
            uint32 time = r_u16(state + 0x26);
            w_u16(state + 0x32, 1);
            w_u16(state + 0x1e, 0);
            w_u16(state + 0x1c, time);
            return;
        }
        if (G3PirateNearbyBall(object, game, module_base, frame) != 0)
        {
            w_u32(object + 0x150, 6);
            next_animation = 4;
            goto restart_animation;
        }
        {
            uint32 source = state + 0x18;
            uint32 destination = frame + 0x20;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            high_address = destination + 3;
            high_lane = high_address & 3;
            lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
        }
        uint32 path = r_u32(state + 0x14);
        G3Function80041230(path, frame + 0x20, frame);
        if (r_s16(state + 0x18) == 0)
            w_u32(object + 0x10, r_u32(object + 0x10) | 0x400);
        else if (r_s16(frame + 0x20) == 0)
            w_u32(object + 0x10, r_u32(object + 0x10) & 0xfffffbff);
        path = r_u32(state + 0x14);
        G3Function80041008(path, frame + 0x20, frame + 0x28, frame);
        {
            uint32 source = frame + 0x20;
            uint32 destination = state + 0x18;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            high_address = destination + 3;
            high_lane = high_address & 3;
            lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
        }
        uint32 x = r_u16(frame + 0x28);
        uint32 origin = r_u16(object + 0x58);
        x -= origin;
        uint32 y = r_u16(frame + 0x2a);
        w_u16(frame + 0x18, x);
        origin = r_u16(object + 0x5a);
        y -= origin;
        w_u16(frame + 0x1a, y);
        if (((x | y) & 0xffff) != 0)
        {
            uint32 angle = G3Function8002F240((uint32)(sint32)(sint16)y, (uint32)(sint32)(sint16)x, frame);
            w_u16(object + 0x64, angle + 0x400);
        }
        {
            uint32 source = frame + 0x28;
            uint32 destination = object + 0x58;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            uint32 z = (uint32)r_s16(source + 4);
            high_address = destination + 3;
            high_lane = high_address & 3;
            lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
            w_u16(destination + 4, z);
        }
        if ((r_u32(object + 0x14) & 0x10) == 0) return;
        uint32 animation = r_u16(object + 0x6e);
        if (animation == 10) goto idle_mode;
        sint32 time = r_s16(state + 0x1c);
        sint32 maximum = r_s16(state + 0x22);
        if (maximum < time)
        {
            w_u16(object + 0x6e, 10);
            w_u16(object + 0x5e, 0);
        }
        else if (animation == 5)
        {
            w_u16(object + 0x6e, 6);
            w_u16(object + 0x5e, 0);
        }
        return;
    }
    case 1:
    {
        w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 player_x = r_u16(player + 0x58);
        uint32 object_x = r_u16(object + 0x58);
        uint32 dx = player_x - object_x;
        uint32 signed_x = (uint32)(sint32)(sint16)dx;
        uint32 square_x = signed_x * signed_x;
        w_u16(frame + 0x18, dx);
        uint32 player_y = r_u16(player + 0x5a);
        uint32 object_y = r_u16(object + 0x5a);
        uint32 dy = player_y - object_y;
        uint32 signed_y = (uint32)(sint32)(sint16)dy;
        uint32 square_y = signed_y * signed_y;
        w_u16(frame + 0x1a, dy);
        uint32 player_z = r_u16(player + 0x5c);
        uint32 object_z = r_u16(object + 0x5c);
        uint32 dz = player_z - object_z;
        uint32 signed_z = (uint32)(sint32)(sint16)dz;
        uint32 square_z = signed_z * signed_z;
        w_u16(frame + 0x1c, dz);
        sint32 distance = (sint32)(square_x + square_y + square_z);
        if (distance <= r_s32(state)) goto alert_mode;
        if (G3PirateNearbyBall(object, game, module_base, frame) != 0)
        {
            w_u32(object + 0x150, 6);
            next_animation = 4;
            goto restart_animation;
        }
        if ((r_u32(object + 0x14) & 0x10) == 0) return;
        if (r_u32(state + 0x14) == 0) goto reset_idle;
        if ((G3SdkRandom80027FA8() & 1) == 0) goto reset_idle;
        w_u32(object + 0x150, 0);
        w_u16(object + 0x6e, 5);
        goto clear_all_timers;
    }
    case 2:
    {
        w_u16(state + 0x1c, r_u16(state + 0x1c) + 1);
        w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 player_x = r_u16(player + 0x58);
        uint32 object_x = r_u16(object + 0x58);
        uint32 dx = player_x - object_x;
        uint32 signed_x = (uint32)(sint32)(sint16)dx;
        uint32 square_x = signed_x * signed_x;
        w_u16(frame + 0x18, dx);
        uint32 player_y = r_u16(player + 0x5a);
        uint32 object_y = r_u16(object + 0x5a);
        uint32 dy = player_y - object_y;
        uint32 signed_y = (uint32)(sint32)(sint16)dy;
        uint32 square_y = signed_y * signed_y;
        w_u16(frame + 0x1a, dy);
        uint32 player_z = r_u16(player + 0x5c);
        uint32 object_z = r_u16(object + 0x5c);
        uint32 dz = player_z - object_z;
        uint32 signed_z = (uint32)(sint32)(sint16)dz;
        uint32 square_z = signed_z * signed_z;
        w_u16(frame + 0x1c, dz);
        sint32 distance = (sint32)(square_x + square_y + square_z);
        if (distance <= r_s32(state)) goto alert_mode;
        if (G3PirateNearbyBall(object, game, module_base, frame) != 0)
        {
            w_u32(object + 0x150, 6);
            next_animation = 4;
            goto restart_animation;
        }
        if ((r_u32(object + 0x14) & 0x10) == 0) return;
        if (r_u16(object + 0x6e) == 10)
        {
            w_u16(object + 0x6e, 0);
            w_u16(object + 0x5e, 0);
            w_u16(state + 0x1c, 0);
            return;
        }
        sint32 time = r_s16(state + 0x1c);
        sint32 maximum = r_s16(state + 0x20);
        if (time <= maximum) return;
        if (r_u32(state + 0x14) != 0 && (G3SdkRandom80027FA8() & 1) != 0)
        {
            w_u32(object + 0x150, 0);
            w_u16(object + 0x6e, 5);
            w_u16(object + 0x5e, 0);
            w_u16(state + 0x1c, 0);
        }
        else
        {
            w_u32(object + 0x150, 1);
            w_u16(object + 0x6e, 1);
            w_u16(object + 0x5e, 0);
        }
        return;
    }
    case 3:
    {
        uint32 turn_time = r_u16(state + 0x1e);
        uint32 time = r_u16(state + 0x1c);
        w_u16(state + 0x1e, turn_time + 1);
        w_u16(state + 0x1c, time + 1);
        uint32 player_x = r_u16(player + 0x58);
        uint32 object_x = r_u16(object + 0x58);
        uint32 dx = player_x - object_x;
        uint32 signed_x = (uint32)(sint32)(sint16)dx;
        uint32 square_x = signed_x * signed_x;
        w_u16(frame + 0x18, dx);
        uint32 player_y = r_u16(player + 0x5a);
        uint32 object_y = r_u16(object + 0x5a);
        uint32 dy = player_y - object_y;
        uint32 signed_y = (uint32)(sint32)(sint16)dy;
        uint32 square_y = signed_y * signed_y;
        w_u16(frame + 0x1a, dy);
        uint32 player_z = r_u16(player + 0x5c);
        uint32 object_z = r_u16(object + 0x5c);
        uint32 dz = player_z - object_z;
        uint32 signed_z = (uint32)(sint32)(sint16)dz;
        uint32 square_z = signed_z * signed_z;
        w_u16(frame + 0x1c, dz);
        sint32 distance = (sint32)(square_x + square_y + square_z);
        uint32 angle = G3Function8002F240((uint32)(sint32)(sint16)dy, (uint32)(sint32)(sint16)dx, frame);
        uint32 target = (uint32)(sint32)(sint16)angle;
        uint32 heading = (uint32)(sint32)(sint16)(r_u16(object + 0x64) - 0x400);
        uint32 first_turn = G3Function80038B48(heading, target);
        bool negative = (sint16)first_turn < 0;
        heading = (uint32)(sint32)(sint16)(r_u16(object + 0x64) - 0x400);
        uint32 turn = G3Function80038B48(heading, target);
        if (negative) turn = 0u - turn;
        G3Function80029F0C(object, 0xffffffff, frame);
        if (r_s32(state + 8) >= distance)
        {
            bool defend = (r_u32(player + 0x154) & 0x2010) != 0;
            if (defend)
            {
                sint32 elapsed = r_s16(state + 0x1c);
                sint32 delay = r_s16(state + 0x30);
                defend = delay < elapsed;
            }
            if (defend)
            {
                w_u32(object + 0x150, 5);
                w_u16(object + 0x6e, 2);
                w_u16(object + 0x5e, 0);
                w_u16(state + 0x32, 0);
            }
            else if ((r_u32(player + 0x154) & 0x88) != 0)
            {
                w_u32(object + 0x150, 7);
                w_u16(object + 0x6e, 14);
                w_u16(object + 0x5e, 0);
            }
            else if ((sint16)turn < r_s16(state + 0x38))
            {
                sint32 elapsed = r_s16(state + 0x1c);
                sint32 delay = r_s16(state + 0x26);
                if (delay < elapsed)
                {
                    w_u32(object + 0x150, 4);
                    w_u16(object + 0x6e, 3);
                    w_u16(object + 0x5e, 0);
                    w_u16(state + 0x32, 0);
                }
            }
        }
        else if (distance >= r_s32(state + 4))
        {
            bool route = r_u32(state + 0x14) != 0;
            if (route)
            {
                sint32 x = r_s16(object + 0x58);
                sint32 home_x = r_s16(state + 0x3a);
                if (x == home_x)
                {
                    sint32 y = r_s16(object + 0x5a);
                    sint32 home_y = r_s16(state + 0x3c);
                    route = y != home_y;
                }
            }
            if (route)
            {
                uint32 path = r_u32(state + 0x14);
                G3Function80040A40(path, object + 0x58, state + 0x18, state + 0x3a, frame);
                w_u32(object + 0x150, 9);
                w_u16(object + 0x6e, 6);
            }
            else
            {
                w_u32(object + 0x150, 2);
                w_u16(object + 0x6e, 0);
            }
            w_u16(object + 0x5e, 0);
            w_u16(state + 0x1e, 0);
            w_u16(state + 0x1c, 0);
        }
        else if (r_s32(state + 0xc) < distance)
        {
            sint32 elapsed = r_s16(state + 0x1e);
            sint32 delay = r_s16(state + 0x2c);
            if (delay < elapsed)
            {
                uint32 bounds = r_u32(state + 0x10);
                bool allowed = bounds == 0;
                if (bounds != 0)
                {
                    sint32 direction_x = r_s16(frame + 0x18);
                    allowed = direction_x == 0;
                    if (direction_x < 0)
                    {
                        sint32 x = r_s16(object + 0x58);
                        sint32 minimum = r_s16(bounds);
                        allowed = minimum < x;
                    }
                    else if (direction_x > 0)
                    {
                        sint32 x = r_s16(object + 0x58);
                        sint32 maximum = r_s16(bounds + 6);
                        allowed = x < maximum;
                    }
                    if (allowed)
                    {
                        sint32 direction_y = r_s16(frame + 0x1a);
                        allowed = direction_y == 0;
                        if (direction_y < 0)
                        {
                            bounds = r_u32(state + 0x10);
                            sint32 y = r_s16(object + 0x5a);
                            sint32 minimum = r_s16(bounds + 2);
                            allowed = minimum < y;
                        }
                        else if (direction_y > 0)
                        {
                            bounds = r_u32(state + 0x10);
                            sint32 y = r_s16(object + 0x5a);
                            sint32 maximum = r_s16(bounds + 8);
                            allowed = y < maximum;
                        }
                    }
                }
                if (allowed)
                {
                    w_u32(object + 0x150, 8);
                    w_u16(object + 0x6e, 8);
                    w_u16(object + 0x5e, 0);
                    w_u16(state + 0x1e, 0);
                    w_u16(state + 0x1c, 0);
                }
            }
        }
        if (r_u32(object + 0x150) == 3)
        {
            sint32 threshold = r_s16(state + 0x36);
            bool changed = false;
            if ((sint16)turn < threshold)
            {
                if (r_s16(state + 0x32) != 0)
                {
                    sint32 elapsed = r_s16(state + 0x1e);
                    sint32 delay = r_s16(state + 0x2c);
                    if (delay < elapsed)
                    {
                        w_u16(object + 0x6e, 0);
                        w_u16(object + 0x5e, 0);
                        w_u16(state + 0x32, 0);
                        changed = true;
                    }
                }
            }
            else if (r_s16(state + 0x32) == 0)
            {
                sint32 elapsed = r_s16(state + 0x1e);
                sint32 delay = r_s16(state + 0x2c);
                if (delay < elapsed)
                {
                    w_u16(object + 0x6e, 15);
                    w_u16(object + 0x5e, 0);
                    w_u16(state + 0x32, 1);
                    changed = true;
                }
            }
            if (changed) w_u16(state + 0x1e, 0);
        }
        uint32 step = (uint32)r_s16(state + 0x34);
        target = (uint32)(sint32)(sint16)(angle + 0x400);
        G3Function80038AA8(object + 0x64, target, step);
        return;
    }
    case 4:
    {
        if (r_s16(state + 0x32) != 0)
        {
            uint32 time = r_u16(state + 0x1c);
            sint32 delay = r_s16(state + 0x24);
            ++time;
            w_u16(state + 0x1c, time);
            if ((sint16)time <= delay) return;
            w_u16(object + 0x6e, 12);
            goto reset_animation;
        }
        w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
        G3Function80029F0C(object, 0xffffffff, frame);
        if ((r_u32(object + 0x14) & 0x10) == 0) return;
        uint32 animation = r_u16(object + 0x6e);
        if (animation == 3) w_u16(state + 0x32, 1);
        else if (animation == 12)
        {
            w_u32(object + 0x150, 3);
            w_u16(object + 0x6e, 0);
            w_u16(object + 0x5e, 0);
            w_u16(state + 0x32, 0);
        }
        else return;
        w_u16(state + 0x1c, 0);
        return;
    }
    case 5:
    {
        uint32 player_x = r_u16(player + 0x58);
        uint32 object_x = r_u16(object + 0x58);
        uint32 dx = player_x - object_x;
        w_u16(frame + 0x18, dx);
        uint32 player_y = r_u16(player + 0x5a);
        uint32 object_y = r_u16(object + 0x5a);
        uint32 dy = player_y - object_y;
        w_u16(frame + 0x1a, dy);
        uint32 player_z = r_u16(player + 0x5c);
        uint32 object_z = r_u16(object + 0x5c);
        uint32 dz = player_z - object_z;
        w_u16(frame + 0x1c, dz);
        uint32 angle = G3Function8002F240((uint32)(sint32)(sint16)dy, (uint32)(sint32)(sint16)dx, frame);
        w_u16(object + 0x64, angle + 0x400);
        if ((r_u32(player + 0x154) & 0x2010) != 0 && r_u16(object + 0x6e) == 13)
        {
            sint32 current = r_s16(object + 0x5e);
            w_u16(object + 0x6e, 2);
            if (current == 0)
            {
                w_u16(state + 0x32, 1);
                w_u16(state + 0x1c, 0);
            }
            else w_u16(state + 0x32, 0);
            goto reverse_animation;
        }
        if (r_s16(state + 0x32) != 0)
        {
            uint32 fresh_player_x = r_u16(player + 0x58);
            uint32 fresh_object_x = r_u16(object + 0x58);
            uint32 fresh_dx = fresh_player_x - fresh_object_x;
            w_u16(frame + 0x18, fresh_dx);
            uint32 fresh_player_y = r_u16(player + 0x5a);
            uint32 fresh_object_y = r_u16(object + 0x5a);
            uint32 fresh_dy = fresh_player_y - fresh_object_y;
            w_u16(frame + 0x1a, fresh_dy);
            uint32 fresh_player_z = r_u16(player + 0x5c);
            uint32 fresh_object_z = r_u16(object + 0x5c);
            uint32 fresh_dz = fresh_player_z - fresh_object_z;
            w_u16(frame + 0x1c, fresh_dz);
            uint32 time = r_u16(state + 0x1c);
            sint32 delay = r_s16(state + 0x2a);
            ++time;
            w_u16(state + 0x1c, time);
            if ((sint16)time > delay)
            {
                next_animation = 13;
                goto restart_animation;
            }
            uint32 x = (uint32)r_s16(frame + 0x18);
            uint32 xx = x * x;
            uint32 y = (uint32)r_s16(frame + 0x1a);
            uint32 yy = y * y;
            uint32 z = (uint32)r_s16(frame + 0x1c);
            uint32 zz = z * z;
            sint32 limit = r_s32(state);
            sint32 distance = (sint32)(xx + yy + zz);
            if (distance <= limit) return;
            w_u16(object + 0x6e, 13);
            goto reset_animation;
        }
        w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
        G3Function80029F0C(object, 0xffffffff, frame);
        if ((r_u32(object + 0x14) & 0x10) == 0) return;
        uint32 animation = r_u16(object + 0x6e);
        if (animation == 2)
        {
            w_u16(state + 0x32, 1);
            w_u16(state + 0x1c, 0);
        }
        else if (animation == 13)
        {
            w_u32(object + 0x150, 3);
            w_u16(object + 0x6e, 0);
            w_u16(object + 0x5e, 0);
            w_u16(state + 0x32, 0);
            w_u16(state + 0x1e, 0);
            w_u16(state + 0x1c, 0);
        }
        return;
    }
    case 6:
    {
        uint32 player_x = r_u16(player + 0x58);
        uint32 object_x = r_u16(object + 0x58);
        uint32 dx = player_x - object_x;
        w_u16(frame + 0x18, dx);
        uint32 player_y = r_u16(player + 0x5a);
        uint32 object_y = r_u16(object + 0x5a);
        uint32 dy = player_y - object_y;
        w_u16(frame + 0x1a, dy);
        uint32 player_z = r_u16(player + 0x5c);
        uint32 object_z = r_u16(object + 0x5c);
        uint32 dz = player_z - object_z;
        w_u16(frame + 0x1c, dz);
        uint32 angle = G3Function8002F240((uint32)(sint32)(sint16)dy, (uint32)(sint32)(sint16)dx, frame);
        uint32 target = (uint32)(sint32)(sint16)(angle + 0x400);
        uint32 step = r_u16(state + 0x34);
        step = (uint32)(sint32)(sint16)(step * 2);
        G3Function80038AA8(object + 0x64, target, step);
        if (G3PirateNearbyBall(object, game, module_base, frame) != 0)
        {
            if (r_s16(state + 0x32) != 0)
            {
                w_u16(state + 0x1c, 0);
                return;
            }
            if (r_u16(object + 0x6e) == 11)
            {
                sint32 current = r_s16(object + 0x5e);
                w_u16(object + 0x6e, 4);
                if (current == 0)
                {
                    w_u16(state + 0x32, 1);
                    w_u16(state + 0x1c, 0);
                }
                else w_u16(state + 0x32, 0);
                goto reverse_animation;
            }
        w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
        G3Function80029F0C(object, 0xffffffff, frame);
            if ((r_u32(object + 0x14) & 0x10) == 0) return;
            w_u16(state + 0x32, 1);
            w_u16(state + 0x1c, 0);
            return;
        }
        if (r_s16(state + 0x32) != 0)
        {
            uint32 time = r_u16(state + 0x1c);
            sint32 delay = r_s16(state + 0x28);
            ++time;
            w_u16(state + 0x1c, time);
            if ((sint16)time <= delay) return;
            next_animation = 11;
            goto restart_animation;
        }
        w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
        G3Function80029F0C(object, 0xffffffff, frame);
        if ((r_u32(object + 0x14) & 0x10) == 0) return;
        uint32 animation = r_u16(object + 0x6e);
        if (animation == 4)
        {
            w_u16(state + 0x32, 1);
            w_u16(state + 0x1c, 0);
        }
        else if (animation == 11) goto idle_mode;
        return;
    }
    case 7:
    {
        uint32 player_x = r_u16(player + 0x58);
        uint32 object_x = r_u16(object + 0x58);
        uint32 dx = player_x - object_x;
        w_u16(frame + 0x18, dx);
        uint32 player_y = r_u16(player + 0x5a);
        uint32 object_y = r_u16(object + 0x5a);
        uint32 dy = player_y - object_y;
        w_u16(frame + 0x1a, dy);
        uint32 player_z = r_u16(player + 0x5c);
        uint32 object_z = r_u16(object + 0x5c);
        uint32 dz = player_z - object_z;
        w_u16(frame + 0x1c, dz);
        uint32 angle = G3Function8002F240((uint32)(sint32)(sint16)dy, (uint32)(sint32)(sint16)dx, frame);
        w_u16(object + 0x64, angle + 0x400);
        w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
        G3Function80029F0C(object, 0xffffffff, frame);
        if ((r_u32(object + 0x14) & 0x10) != 0) goto alert_mode;
        return;
    }
    case 8:
    {
        uint32 turn_time = r_u16(state + 0x1e);
        uint32 time = r_u16(state + 0x1c);
        w_u16(state + 0x1e, turn_time + 1);
        w_u16(state + 0x1c, time + 1);
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 player_x = r_u16(player + 0x58);
        uint32 object_x = r_u16(object + 0x58);
        uint32 dx = player_x - object_x;
        uint32 signed_x = (uint32)(sint32)(sint16)dx;
        uint32 square_x = signed_x * signed_x;
        w_u16(frame + 0x18, dx);
        uint32 player_y = r_u16(player + 0x5a);
        uint32 object_y = r_u16(object + 0x5a);
        uint32 dy = player_y - object_y;
        uint32 signed_y = (uint32)(sint32)(sint16)dy;
        uint32 square_y = signed_y * signed_y;
        w_u16(frame + 0x1a, dy);
        uint32 player_z = r_u16(player + 0x5c);
        uint32 object_z = r_u16(object + 0x5c);
        uint32 dz = player_z - object_z;
        uint32 signed_z = (uint32)(sint32)(sint16)dz;
        uint32 square_z = signed_z * signed_z;
        w_u16(frame + 0x1c, dz);
        sint32 distance = (sint32)(square_x + square_y + square_z);
        if (distance <= r_s32(state + 0xc))
        {
            sint32 time = r_s16(state + 0x1e);
            sint32 delay = r_s16(state + 0x2c);
            if (delay < time) goto alert_mode;
        }
        sint32 time_now = r_s16(state + 0x1c);
        sint32 timeout = r_s16(state + 0x2e);
        if (timeout < time_now) goto alert_mode;
        w_u32(frame + 0x10, 0);
        G3Function80036738(object, player + 0x58, 60, 95, 0, frame);
        uint32 bounds = r_u32(state + 0x10);
        if (bounds == 0) return;
        sint32 x = r_s16(object + 0x58);
        sint32 minimum = r_s16(bounds);
        uint32 value = r_u16(bounds);
        bool clamped = x < minimum;
        if (!clamped)
        {
            sint32 maximum = r_s16(bounds + 6);
            value = r_u16(bounds + 6);
            clamped = maximum < x;
        }
        bool changed = false;
        if (clamped)
        {
            w_u16(object + 0x58, value);
            changed = true;
        }
        bounds = r_u32(state + 0x10);
        sint32 y = r_s16(object + 0x5a);
        minimum = r_s16(bounds + 2);
        value = r_u16(bounds + 2);
        clamped = y < minimum;
        if (!clamped)
        {
            sint32 maximum = r_s16(bounds + 8);
            value = r_u16(bounds + 8);
            clamped = maximum < y;
        }
        if (clamped)
        {
            w_u16(object + 0x5a, value);
            changed = true;
        }
        if (changed) goto alert_mode;
        return;
    }
    case 9:
    {
        uint32 turn_time = r_u16(state + 0x1e);
        uint32 time = r_u16(state + 0x1c);
        w_u16(state + 0x1e, turn_time + 1);
        w_u16(state + 0x1c, time + 1);
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 player_x = r_u16(player + 0x58);
        uint32 object_x = r_u16(object + 0x58);
        uint32 dx = player_x - object_x;
        uint32 signed_x = (uint32)(sint32)(sint16)dx;
        uint32 square_x = signed_x * signed_x;
        w_u16(frame + 0x18, dx);
        uint32 player_y = r_u16(player + 0x5a);
        uint32 object_y = r_u16(object + 0x5a);
        uint32 dy = player_y - object_y;
        uint32 signed_y = (uint32)(sint32)(sint16)dy;
        uint32 square_y = signed_y * signed_y;
        w_u16(frame + 0x1a, dy);
        uint32 player_z = r_u16(player + 0x5c);
        uint32 object_z = r_u16(object + 0x5c);
        uint32 dz = player_z - object_z;
        uint32 signed_z = (uint32)(sint32)(sint16)dz;
        uint32 square_z = signed_z * signed_z;
        w_u16(frame + 0x1c, dz);
        sint32 distance = (sint32)(square_x + square_y + square_z);
        if (distance <= r_s32(state))
        {
            if (r_s32(state + 0xc) < distance)
            {
                sint32 time = r_s16(state + 0x1e);
                sint32 delay = r_s16(state + 0x2c);
                if (delay < time)
                {
                    w_u32(object + 0x150, 8);
                    w_u16(object + 0x6e, 8);
                    goto clear_all_timers;
                }
            }
        }
        w_u32(frame + 0x10, 0);
        if (G3Function80036738(object, state + 0x3a, 30, 95, 0, frame) == 0) return;
        goto reset_idle;
    }
    default: return;
    }

restart_animation:
    w_u16(object + 0x6e, next_animation);
reset_animation:
    w_u16(object + 0x5e, 0);
    w_u16(state + 0x32, 0);
    return;
idle_mode:
    w_u32(object + 0x150, 2);
    w_u16(object + 0x6e, 0);
    w_u16(object + 0x5e, 0);
    return;
reset_idle:
    w_u32(object + 0x150, 2);
    w_u16(object + 0x6e, 0);
clear_all_timers:
    w_u16(object + 0x5e, 0);
    w_u16(state + 0x1e, 0);
    w_u16(state + 0x1c, 0);
    return;
alert_mode:
    w_u32(object + 0x150, 3);
    w_u16(object + 0x6e, 0);
    w_u16(object + 0x5e, 0);
    {
        uint32 time = r_u16(state + 0x26);
        w_u16(state + 0x32, 0);
        w_u16(state + 0x1e, 0);
        w_u16(state + 0x1c, time);
    }
    return;
reverse_animation:
    {
        uint32 descriptor = r_u32(object + 0x1c);
        uint32 animation = r_u16(object + 0x6e);
        uint32 animations = r_u32(descriptor + 0x10);
        uint32 data = r_u32(animations + animation * 4);
        uint32 current = r_u16(object + 0x5e);
        uint32 count = r_u16(data + 2);
        w_u16(object + 0x5e, count - (current + 1));
    }
}

void G3PirateCollide(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001540, "G3L_41");
    uint32 frame = guest_stack - 0x20;
    uint32 contact = r_u32(object + 0xb0);
    if (r_s16(contact + 6) != 1) return;
    uint32 other = r_u32(contact + 0x14);
    bool damage = false;
    if (other != 0)
    {
        uint32 descriptor = r_u32(other + 0x1c);
        uint32 name = r_u32(descriptor + 0x20);
        if (name != 0)
        {
            uint32 actual = r_u32(name);
            uint32 expected = r_u32(module_base + 0x1878);
            if (actual == expected)
            {
                actual = r_u32(name + 4);
                expected = r_u32(module_base + 0x187c);
                damage = actual == expected;
            }
        }
    }
    if (!damage)
    {
        sint32 kind = r_s16(contact + 6);
        if (kind != 1) return;
        other = r_u32(contact + 0x14);
        uint32 player = r_u32(game + 0x50);
        if (other == player)
        {
            uint32 descriptor = r_u32(contact + 0xc);
            if (r_u8(descriptor + 5) >= 6)
            {
                player = r_u32(0x8009efc4);
                uint32 activity = r_u32(player + 0x154);
                if (activity != 0x80 && activity != 0x20 && r_s16(contact + 4) == kind)
                {
                    uint32 mode = r_u32(object + 0x150);
                    damage = mode != 5 && mode != 7;
                }
            }
        }
    }
    if (damage)
    {
        G3Function80038798(object, 1, 0, frame);
        return;
    }
    if (r_s16(contact + 6) != 1) return;
    other = r_u32(contact + 0x14);
    uint32 player = r_u32(game + 0x50);
    bool repel = false;
    uint32 player_x = 0;
    uint32 delta_y = 0;
    if (other == player)
    {
        uint32 descriptor = r_u32(contact + 0xc);
        if (r_u8(descriptor + 5) >= 6)
        {
            player = r_u32(0x8009efc4);
            if ((r_u32(player + 0x154) & 0xa0) == 0 && r_u32(object + 0x150) == 5)
            {
                uint32 player_y = r_u16(player + 0x5a);
                uint32 object_y = r_u16(object + 0x5a);
                player_x = r_u16(player + 0x58);
                delta_y = player_y - object_y;
                repel = true;
            }
        }
    }
    if (!repel)
    {
        if (r_s16(contact + 6) != 1) return;
        other = r_u32(contact + 0x14);
        player = r_u32(game + 0x50);
        if (other != player) return;
        if (r_s16(contact + 4) != 5) return;
        if (r_u32(object + 0x150) != 4) return;
        G3Function80074044(object, game, frame);
        player = r_u32(0x8009efc4);
        uint32 object_y = r_u16(object + 0x5a);
        uint32 player_y = r_u16(player + 0x5a);
        player_x = r_u16(player + 0x58);
        delta_y = player_y - object_y;
    }
    uint32 object_x = r_u16(object + 0x58);
    uint32 delta_x = player_x - object_x;
    uint32 angle = G3Function8002F240((uint32)(sint32)(sint16)delta_y, (uint32)(sint32)(sint16)delta_x, frame);
    angle = (uint32)(sint32)(sint16)angle;
    uint32 sine = G3Function8005B470(angle);
    player = r_u32(0x8009efc4);
    w_u32(player + 0x104, (uint32)((sint32)(sine << 7) >> 12));
    uint32 cosine = G3Function8005B590(angle);
    player = r_u32(0x8009efc4);
    w_u32(player + 0x108, (uint32)((sint32)(cosine << 7) >> 12));
    G3Function8006F468(game, frame);
}

uint32 G3PlankEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_26");
    uint32 frame = guest_stack - 0x20;
    switch (message)
    {
    case 1:
        G3PlankInitialize(object, module_base, frame);
        return 1;
    case 2:
        G3PlankUpdate(object, frame);
        G3Function8003C614(object, game, frame);
        return 1;
    case 3:
        G3Function8003C864(object, game, frame);
        return 1;
    case 4:
        G3Function8003A114(object + 0x104, 0);
        return 1;
    case 9:
        G3PlankPush(object);
        return 1;
    case 2028:
        w_u16(payload, r_u16(object + 0x11a));
        return 1;
    default:
        return 0;
    }
}

void G3PlankConfigure(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000110, "G3L_26");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    w_u32(guest_stack + 4, commands);
    if (commands == 0)
        return;
    while (G3Function80085DFC(frame + 0x34, frame + 0x10, frame + 0x14, frame + 0x18) != 0)
    {
        uint32 kind = r_u16(frame + 0x10);
        uint32 index = (uint32)((sint32)((kind - 0x11) << 16) >> 16);
        if (index >= 47)
            continue;
        uint32 target = r_u32(module_base + 0x53c + index * 4);
        uint32 offset;
        switch (target - module_base)
        {
        case 0x134:
            continue;
        case 0x188:
            offset = 0x12;
            break;
        case 0x19c:
            offset = 0x14;
            break;
        case 0x1b0:
            offset = 0x10;
            break;
        case 0x1c4:
            offset = 0x1c;
            break;
        case 0x1d8:
            offset = 0x1a;
            break;
        case 0x1ec:
            offset = 0x18;
            break;
        default:
            {
#ifndef NDEBUG
            fprintf(stderr, "G3 unresolved plank command target %08X\n", (unsigned int)target);
#endif
            }
            abort();
        }
        uint32 data = r_u32(frame + 0x14);
        w_u16(state + offset, r_u16(data));
    }
}

void G3PlankInitialize(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000214, "G3L_26");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    w_u16(state + 0x1c, 1800);
    w_u16(state + 0x12, 40);
    w_u16(state + 0x14, 40);
    w_u16(state + 0x1a, 60);
    w_u16(state + 0x10, 0);
    w_u16(state + 0x18, 2);
    w_u16(state + 0x16, 0);
    uint32 resource = r_u32(object + 0x1c);
    uint32 commands = r_u32(resource + 0x1c);
    G3PlankConfigure(object, commands, module_base, frame);
    commands = r_u32(object + 0x28);
    G3PlankConfigure(object, commands, module_base, frame);
    uint32 axis = (uint32)r_s16(state + 0x10);
    uint32 extent = r_u16(state + 0x1c);
    uint32 origin = r_u16(object + 0x58 + (axis << 1));
    uint32 direction = (extent >> 15) ^ 1;
    w_u16(state + 0x20, direction);
    w_u16(state + 0x1e, origin);
    if (direction != 0)
        w_u16(state + 0x14, 0 - r_u16(state + 0x14));
    else
        w_u16(state + 0x12, 0 - r_u16(state + 0x12));
    extent = r_u16(state + 0x1c);
    origin = r_u16(state + 0x1e);
    w_u16(state + 0x16, 0);
    w_u16(state + 0x1c, extent + origin);
    w_u32(object + 0x10, r_u32(object + 0x10) | 0x40000000);
    w_u16(state + 4, 0x907);
    w_u16(state + 0xa, 120);
    w_u32(state, 0);
    w_u16(state + 8, 0);
    w_u16(state + 6, 32000);
    w_u16(state + 0xc, 0);
}

void G3PlankUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000320, "G3L_26");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    uint32 moving = r_s16(state + 0x16) != 0 ? 1 : 0;
    G3Function80039F5C(object, state, moving, frame);
    sint32 direction = r_s16(state + 0x20);
    uint32 axis = (uint32)r_s16(state + 0x10);
    uint32 position = r_u16(object + 0x58 + (axis << 1));
    uint32 velocity = r_u16(state + 0x16);
    sint32 origin = r_s16(state + 0x1e);
    position += velocity;
    sint32 signed_position = (sint32)(position << 16) >> 16;
    uint32 origin_bits = r_u16(state + 0x1e);
    if ((direction != 0 && origin >= signed_position) || (direction == 0 && signed_position >= origin))
    {
        position = origin_bits;
        w_u16(state + 0x16, 0);
    }
    else if (direction != 0)
    {
        sint32 end = r_s16(state + 0x1c);
        uint32 end_bits = r_u16(state + 0x1c);
        if (end < signed_position)
        {
            uint32 acceleration = r_u16(state + 0x18);
            position = end_bits;
            w_u16(state + 0x16, acceleration);
        }
        velocity = r_u16(state + 0x16);
        uint32 acceleration = r_u16(state + 0x18);
        velocity -= acceleration;
        w_u16(state + 0x16, velocity);
        sint32 limit = r_s16(state + 0x14);
        uint32 limit_bits = r_u16(state + 0x14);
        if (((sint32)(velocity << 16) >> 16) < limit)
            w_u16(state + 0x16, limit_bits);
    }
    else
    {
        sint32 end = r_s16(state + 0x1c);
        uint32 end_bits = r_u16(state + 0x1c);
        if (signed_position < end)
        {
            uint32 acceleration = r_u16(state + 0x18);
            position = end_bits;
            w_u16(state + 0x16, 0 - acceleration);
        }
        velocity = r_u16(state + 0x16);
        uint32 acceleration = r_u16(state + 0x18);
        velocity += acceleration;
        w_u16(state + 0x16, velocity);
        sint32 limit = r_s16(state + 0x14);
        uint32 limit_bits = r_u16(state + 0x14);
        if (limit < ((sint32)(velocity << 16) >> 16))
            w_u16(state + 0x16, limit_bits);
    }
    axis = (uint32)r_s16(state + 0x10);
    w_u16(object + 0x58 + (axis << 1), position);
}

void G3PlankPush(uint32 object)
{
    FUNCTION_MARKER(0x000004b4, "G3L_26");
    uint32 state = object + 0x104;
    sint32 direction = r_s16(state + 0x20);
    uint32 velocity = r_u16(state + 0x16);
    uint32 impulse = r_u16(state + 0x1a);
    velocity = direction != 0 ? velocity + impulse : velocity - impulse;
    w_u16(state + 0x16, velocity);
    sint32 limit = r_s16(state + 0x12);
    uint32 limit_bits = r_u16(state + 0x12);
    sint32 signed_velocity = (sint32)(velocity << 16) >> 16;
    if ((direction != 0 && limit < signed_velocity) || (direction == 0 && signed_velocity < limit))
        w_u16(state + 0x16, limit_bits);
}

uint32 G3PongballEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_43");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3PongballInitialize(object, game, frame);
        return 1;
    case 2:
        G3PongballUpdate(object, frame);
        return 1;
    case 3:
        G3PongballCollide(object, game, frame);
        return 1;
    case 2017:
        G3PongballLaunch(object, payload, frame);
        return 1;
    default:
        return 0;
    }
}

uint32 G3FindLevelResource800397B0(uint32 name, uint32 game)
{
    uint32 level = r_u32(game + 0x48);
    uint32 table = r_u32(level + 0x3c);
    if (r_u32(table) == table)
        return 0;
    uint32 cursor = table;
    do
    {
        uint32 resource = r_u32(cursor);
        if (resource != 0)
        {
            uint32 candidate = r_u32(resource + 0x24);
            uint32 first = r_u32(name);
            uint32 other = r_u32(candidate);
            if (other == first)
            {
                other = r_u32(candidate + 4);
                first = r_u32(name + 4);
                if (other == first)
                    return resource;
            }
        }
        cursor += 4;
    } while (r_u32(cursor) != table);
    return 0;
}

void G3PongballConfigure(uint32 object, uint32 commands, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000a0, "G3L_43");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    w_u32(guest_stack + 4, commands);
    if (commands == 0)
        return;
    while (G3Function80085DFC(frame + 0x34, frame + 0x10, frame + 0x14, frame + 0x18) != 0)
    {
        sint32 kind = r_s16(frame + 0x10);
        if (kind == 0x15)
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0xc, r_u16(data));
        }
        else if (kind == 0x52)
        {
            uint32 name = r_u32(frame + 0x14);
            uint32 game = r_u32(0x80097048);
            uint32 resource = G3FindLevelResource800397B0(name, game);
            w_u32(state, resource);
        }
    }
}

void G3PongballInitialize(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000013c, "G3L_43");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    w_u16(state + 0xc, 0);
    uint32 resource = r_u32(object + 0x1c);
    uint32 commands = r_u32(resource + 0x1c);
    G3PongballConfigure(object, commands, frame);
    w_u16(state + 6, 0);
    w_u16(state + 4, 0);
    w_u16(state + 8, 25);
    uint32 owner = r_u32(object + 0x2c);
    if (owner != 0)
    {
        uint32 target = r_u32(owner + 0xec);
        if (target != 0)
            G3DispatchStateEvent(target, owner, game, 2006, object, frame);
    }
    w_u32(object + 0x150, 0);
}

void G3PongballLaunch(uint32 object, uint32 data, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000001c8, "G3L_43");
    uint32 frame = guest_stack - 0x20;
    uint32 angle = (uint32)((sint32)((r_u16(object + 0x64) - 0x400) << 16) >> 16);
    uint32 state = object + 0x104;
    uint32 sine = G3Function8005B470(angle);
    uint32 speed = (uint32)r_s16(data);
    uint32 product = speed * sine;
    w_u16(state + 4, (uint32)((sint32)product >> 12));
    uint32 cosine = G3Function8005B590(angle);
    speed = (uint32)r_s16(data);
    product = speed * cosine;
    sint32 return_speed = r_s16(state + 0xc);
    w_u16(state + 6, (uint32)((sint32)product >> 12));
    w_u16(state + 8, r_u16(data + 2));
    if (return_speed == 0)
        w_u16(state + 0xc, r_u16(data));
    w_u16(state + 0xe, r_u16(data + 4));
    w_u16(state + 0x10, r_u16(data + 6));
    (void)frame;
}

void G3PongballDestroy(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000284, "G3L_43");
    uint32 frame = guest_stack - 0x18;
    uint32 resource = r_u32(object + 0x104);
    if (resource != 0)
    {
        uint32 models = r_u32(resource + 0xc);
        uint32 model = r_u32(models);
        uint32 descriptor = r_u32(model + 0x1c);
        uint32 first = r_u32(descriptor + 8);
        uint32 second = r_u32(descriptor + 0xc);
        uint32 product = first * second;
        uint32 pool = r_u32(0x800a27fc);
        uint32 count = (uint32)((sint32)(product << 16) >> 16);
        G3Function800444E4(object + 0x58, model, pool, count, frame);
    }
    G3Function80038798(object, 0, 0, frame);
}

void G3PongballUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000304, "G3L_43");
    uint32 frame = guest_stack - 0x18;
    sint32 mode = r_s32(object + 0x150);
    uint32 state = object + 0x104;
    if (mode == 0 || mode == 1)
    {
        uint32 x = r_u16(object + 0x58);
        uint32 dx = r_u16(state + 4);
        w_u16(object + 0x58, x + dx);
        uint32 y = r_u16(object + 0x5a);
        uint32 dy = r_u16(state + 6);
        w_u16(object + 0x5a, y + dy);
        uint32 z = r_u16(object + 0x5c);
        uint32 dz = r_u16(state + 8);
        w_u16(object + 0x5c, z + dz);
        uint32 yaw = r_u16(object + 0x64);
        uint32 turn = r_u16(state + 0xe);
        w_u16(object + 0x64, yaw + turn);
    }
    else if (mode == 2)
        G3PongballDestroy(object, frame);
}

void G3PongballCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000003a4, "G3L_43");
    uint32 frame = guest_stack - 0x30;
    uint32 contact = r_u32(object + 0xb0);
    uint32 state = object + 0x104;
    G3Function8003C548(contact, frame + 0x10, frame + 0x11);
    uint32 other = r_u32(contact + 0x14);
    uint32 player = r_u32(0x8009efc4);
    if (other == player)
    {
        uint32 first = r_u8(frame + 0x10);
        if (first - 6 < 4)
        {
            uint32 second = r_u8(frame + 0x11);
            if ((second == 9 || second - 3 < 3) && r_u32(object + 0x150) == 0)
            {
                G3Function8007408C(object, game, frame);
                w_u32(object + 0x150, 2);
                return;
            }
        }
        uint32 second = r_u8(frame + 0x11);
        if (second - 6 >= 4)
            return;
        first = r_u8(frame + 0x10);
        if (first != 9 && first - 3 >= 3)
            return;
        if (r_u32(object + 0x150) != 0)
            return;
        uint32 owner = r_u32(object + 0x2c);
        w_u32(object + 0x150, 1);
        if (owner == 0)
            return;
        uint32 owner_y = (uint32)r_s16(owner + 0x5a);
        uint32 y = (uint32)r_s16(object + 0x5a);
        uint32 owner_x = (uint32)r_s16(owner + 0x58);
        uint32 dy = owner_y - y;
        uint32 x = (uint32)r_s16(object + 0x58);
        uint32 dx = owner_x - x;
        uint32 heading = G3Function8002F240(dy, dx, frame);
        uint32 angle = (uint32)((sint32)(heading << 16) >> 16);
        uint32 sine = G3Function8005B470(angle);
        uint32 speed = (uint32)r_s16(state + 0xc);
        uint32 product = speed * sine;
        w_u16(state + 4, (uint32)((sint32)product >> 12));
        uint32 cosine = G3Function8005B590(angle);
        uint32 xx = dx * dx;
        uint32 yy = dy * dy;
        speed = (uint32)r_s16(state + 0xc);
        product = speed * cosine;
        w_u16(state + 6, (uint32)((sint32)product >> 12));
        uint32 distance = G3Function8002F448(xx + yy, 0, frame);
        if (distance == 0)
        {
            w_u16(state + 8, 0);
            return;
        }
        owner = r_u32(object + 0x2c);
        uint32 height_offset = (uint32)r_s16(state + 0x10);
        uint32 owner_z = (uint32)r_s16(owner + 0x5c);
        uint32 height = owner_z + height_offset;
        uint32 z = (uint32)r_s16(object + 0x5c);
        speed = (uint32)r_s16(state + 0xc);
        product = speed * (height - z);
        uint32 vertical = (uint32)((sint64)(sint32)product / (sint32)distance);
        w_u16(state + 8, vertical);
        return;
    }
    uint32 owner = r_u32(object + 0x2c);
    if (other == owner)
    {
        if (r_u32(object + 0x150) == 1)
            w_u32(object + 0x150, 2);
        return;
    }
    uint32 second = r_u8(frame + 0x11);
    bool finish = false;
    if (second - 6 < 4)
    {
        uint32 first = r_u8(frame + 0x10);
        finish = first == 9 || first - 3 < 3;
    }
    if (!finish)
    {
        uint32 x = r_u16(object + 0x58);
        uint32 dx = r_u16(contact + 0x28);
        w_u16(object + 0x58, x + dx);
        uint32 y = r_u16(object + 0x5a);
        uint32 dy = r_u16(contact + 0x2a);
        w_u16(object + 0x5a, y + dy);
        uint32 z = r_u16(object + 0x5c);
        uint32 dz = r_u16(contact + 0x2c);
        w_u16(object + 0x5c, z + dz);
        G3Function8005F3A8(object, contact + 0x28);
    }
    w_u32(object + 0x150, 2);
}

uint32 G3ProxsigEvent(uint32 object, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_71");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3ProxsigInitialize(object, module_base, frame);
        return 1;
    case 2:
        G3ProxsigUpdate(object, frame);
        return 1;
    case 9:
        G3ProxsigConfigure(object, payload, module_base, frame);
        return 1;
    case 15:
        G3ProxsigDefinitions(payload, module_base, frame);
        return 1;
    default:
        return 0;
    }
}

uint32 G3ProxsigDistance(uint32 vector, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000009c, "G3L_71");
    uint32 frame = guest_stack - 0x20;
    uint32 first = r_u32(vector);
    uint32 second = r_u32(vector + 4);
    w_u32(frame + 0x10, first);
    w_u32(frame + 0x14, second);
    if (r_u32(state + 0xc) != 0)
        w_u16(frame + 0x14, 0);
    sint32 x = r_s16(frame + 0x10);
    uint32 xx = (uint32)(x * x);
    sint32 y = r_s16(frame + 0x12);
    uint32 yy = (uint32)(y * y);
    sint32 z = r_s16(frame + 0x14);
    uint32 zz = (uint32)(z * z);
    return G3Function8002F448(xx + yy + zz, 0, frame);
}

void G3ProxsigDefinitions(uint32 resource, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000128, "G3L_71");
    uint32 frame = guest_stack - 0x18;
    uint32 identity = r_u32(resource + 0x24);
    uint32 name = r_u32(identity);
    uint32 expected = r_u32(module_base + 0x454);
    if (name != expected)
        return;
    uint32 commands = r_u32(resource + 0x1c);
    if (commands == 0)
        return;
    uint32 result = G3Function80085AB0(commands, module_base + 0x45c, frame);
    if (result == 0)
        return;
    uint32 definitions = r_u32(result);
    if (definitions != 0)
        w_u32(module_base + 0x4a0, definitions);
}

void G3ProxsigInitialize(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000198, "G3L_71");
    uint32 frame = guest_stack - 0x28;
    uint32 data = r_u32(object + 0x28);
    uint32 state = object + 0x104;
    if (data == 0)
    {
        uint32 defaults = module_base + 0x498;
        w_u32(object + 0x28, defaults);
        w_u32(defaults, 1);
        w_u16(defaults + 4, 0);
        w_u16(defaults + 6, 0x500);
    }
    uint32 commands = r_u32(object + 0x24);
    if (commands != 0)
        G3ProxsigConfigure(object, commands, module_base, frame);
    w_u32(object + 0x10, r_u32(object + 0x10) | 0x800);
    uint32 player = r_u32(0x8009efc4);
    uint32 x = r_u16(object + 0x58);
    uint32 px = r_u16(player + 0x58);
    w_u16(frame + 0x10, x - px);
    uint32 y = r_u16(object + 0x5a);
    uint32 py = r_u16(player + 0x5a);
    w_u16(frame + 0x12, y - py);
    uint32 z = r_u16(object + 0x5c);
    uint32 pz = r_u16(player + 0x5c);
    w_u16(frame + 0x14, z - pz);
    uint32 distance = G3ProxsigDistance(frame + 0x10, state, frame);
    w_u32(state + 8, distance);
}

void G3ProxsigUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000264, "G3L_71");
    uint32 frame = guest_stack - 0x38;
    uint32 distance = 0;
    uint32 enabled = r_u32(object + 0x104);
    uint32 data = r_u32(object + 0x28);
    uint32 state = object + 0x104;
    if (enabled != 0 && data != 0)
    {
        uint32 player = r_u32(0x8009efc4);
        uint32 x = r_u16(object + 0x58);
        uint32 px = r_u16(player + 0x58);
        w_u16(frame + 0x10, x - px);
        uint32 y = r_u16(object + 0x5a);
        uint32 py = r_u16(player + 0x5a);
        w_u16(frame + 0x12, y - py);
        uint32 z = r_u16(object + 0x5c);
        uint32 pz = r_u16(player + 0x5c);
        w_u16(frame + 0x14, z - pz);
        uint32 row = data + 4;
        distance = G3ProxsigDistance(frame + 0x10, state, frame);
        uint32 count = r_u32(data);
        uint32 end = data + (count << 3) + 4;
        uint32 script_slot = data + 8;
        while (row < end)
        {
            sint32 lower = r_s16(row);
            if ((sint32)distance >= lower)
            {
                sint32 upper = r_s16(script_slot - 2);
                if ((sint32)distance < upper)
                {
                    bool trigger = true;
                    if (r_u32(state + 4) != 0)
                    {
                        sint32 previous = r_s32(state + 8);
                        if (previous >= lower && previous < upper)
                            trigger = false;
                    }
                    if (trigger)
                    {
                        uint32 script = r_u32(script_slot);
                        player = r_u32(0x8009efc4);
                        G3Function80018DE0(player, script + 4, frame);
                    }
                }
            }
            row += 8;
            script_slot += 8;
        }
    }
    w_u32(state + 8, distance);
}

void G3ProxsigConfigure(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000003a4, "G3L_71");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    if (commands == 0)
        return;
    if (G3Function80085AB0(commands, module_base + 0x468, frame) != 0)
        w_u32(object + 0x104, 1);
    else if (G3Function80085AB0(commands, module_base + 0x474, frame) != 0)
        w_u32(object + 0x104, 0);
    if (G3Function80085AB0(commands, module_base + 0x480, frame) != 0)
        w_u32(state + 4, 1);
    if (G3Function80085AB0(commands, module_base + 0x48c, frame) != 0)
        w_u32(state + 0xc, 1);
}

uint32 G3RahstafEvent(uint32 object, uint32 game, uint32 message, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_19");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3RahstafInitialize(object);
        return 1;
    case 2:
        G3RahstafUpdate(object, frame);
        return 1;
    case 3:
        G3RahstafCollide(object, game, frame);
        return 1;
    default:
        return 0;
    }
}

void G3RahstafInitialize(uint32 object)
{
    FUNCTION_MARKER(0x00000088, "G3L_19");
    uint32 parent = r_u32(object + 0x2c);
    w_u32(object + 0x150, 0);
    w_u32(object + 0x104, parent != 0 ? 1 : 0);
}

void G3RahstafUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000009c, "G3L_19");
    uint32 frame = guest_stack - 0x18;
    if (r_u32(object + 0x150) != 0)
        return;
    if (r_u32(object + 0x104) != 0)
    {
        if (r_u16(object + 0x6e) == 0)
        {
            uint32 resource = r_u32(object + 0x1c);
            uint32 sequences = r_u32(resource + 0x10);
            uint32 sequence = r_u32(sequences);
            sint32 count = r_s16(sequence + 2);
            sint32 current = r_s16(object + 0x5e);
            if (current == count - 1)
            {
                w_u16(object + 0x6e, 1);
                w_u16(object + 0x5e, 0);
                return;
            }
        }
        G3Function80029F0C(object, 0xffffffff, frame);
    }
    else
    {
        sint32 angle = r_s16(object + 0x64);
        w_u16(object + 0x6e, 0);
        w_u16(object + 0x5e, 0);
        w_u16(object + 0x64, (uint32)((angle - 64) % 4096));
    }
}

void G3RahstafCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000158, "G3L_19");
    uint32 frame = guest_stack - 0x18;
    uint32 mode = r_u32(object + 0x150);
    uint32 contact = r_u32(object + 0xb0);
    if (mode != 0)
        return;
    if (r_u32(object + 0x104) != 0)
        return;
    uint32 other = r_u32(contact + 0x14);
    uint32 player = r_u32(game + 0x50);
    if (other == player)
    {
        w_u32(object + 0x150, 1);
        G3Function80038798(object, 0, 0, frame);
    }
}

uint32 G3SantaEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_51");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3SantaInitialize(object, frame);
        return 1;
    case 2:
        G3SantaUpdate(object, game, module_base, frame);
        return 1;
    case 3:
        G3SantaCollide(object, module_base);
        return 1;
    default:
        return 0;
    }
}

void G3SantaConfigure(uint32 object, uint32 commands, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000007c, "G3L_51");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    w_u32(frame + 0x2c, commands);
    if (commands == 0)
        return;
    while (G3Function80085DFC(frame + 0x2c, frame + 0x10, frame + 0x14, frame + 0x18) != 0)
    {
        sint32 kind = r_s16(frame + 0x10);
        switch (kind)
        {
        case 1:
        {
            uint32 data = r_u32(frame + 0x14);
            for (uint32 axis = 0; axis != 2; ++axis)
            {
                uint32 radius = (uint32)r_s16(data + axis * 2);
                w_u32(state + axis * 4, radius * radius);
            }
            break;
        }
        case 0x31:
        case 0x55:
        {
            uint32 data = r_u32(frame + 0x14);
            sint32 scaled = (sint32)((uint32)r_s16(data) << 12);
            w_u16(state + (kind == 0x31 ? 0x1e : 0x30), (uint32)(scaled / 3600));
            break;
        }
        case 0x5a:
        {
            uint32 source = r_u32(frame + 0x14);
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            uint32 last = (uint32)r_s16(source + 4);
            uint32 destination = state + 0x22;
            high_address = destination + 3;
            high_lane = high_address & 3;
            lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
            w_u16(destination + 4, last);
            break;
        }
        case 0x52:
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 game = r_u32(0x80097048);
            uint32 resource = G3FindLevelResource800397B0(data, game);
            w_u32(state + 0xc, resource);
            break;
        }
        case 0x60:
        {
            sint32 size = r_s16(frame + 0x18);
            uint32 data = r_u32(frame + 0x14);
            w_u32(state + 0x10, data);
            w_u16(state + 0x14, (uint32)(size / 6));
            break;
        }
        case 0x96:
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 value = r_u16(data);
            w_u16(state + 0x36, value);
            value = r_u16(data + 2);
            w_u16(state + 0x38, value);
            break;
        }
        case 0x5b:
        case 0x5c:
        case 0x9d:
        case 0x5e:
        case 0x69:
        case 0x9f:
        case 0x9e:
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 value = r_u16(data);
            uint32 offset = 0;
            switch (kind)
            {
            case 0x5b: offset = 0x2a; break;
            case 0x5c: offset = 0x28; break;
            case 0x9d: offset = 0x2c; break;
            case 0x5e: offset = 0x34; break;
            case 0x69: offset = 0x3a; break;
            case 0x9f: offset = 0x3c; break;
            case 0x9e: offset = 0x3e; break;
            }
            w_u16(state + offset, value);
            break;
        }
        }
    }
}

void G3SantaInitialize(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000364, "G3L_51");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    w_u32(state, 0x005f5e10);
    w_u16(state + 0x1e, 0x5b);
    w_u16(state + 0x26, 0x4b0);
    w_u16(state + 0x2a, 10);
    w_u16(state + 0x2c, 0x50);
    w_u16(state + 0x34, 0x1e);
    w_u16(state + 0x38, 1);
    w_u16(state + 0x3e, 100);
    w_u16(state + 0x3c, 0x96);
    w_u32(state + 4, 0x00895440);
    w_u16(state + 0x22, 0);
    w_u16(state + 0x24, 0);
    w_u16(state + 0x28, 0);
    w_u16(state + 0x30, 0);
    w_u16(state + 0x2e, 0);
    w_u32(state + 0x10, 0);
    w_u16(state + 0x36, 0);
    w_u16(state + 0x3a, 3);
    w_u32(state + 8, 0);
    uint32 resource = r_u32(object + 0x1c);
    uint32 commands = r_u32(resource + 0x1c);
    G3SantaConfigure(object, commands, frame);
    commands = r_u32(object + 0x28);
    if (commands != 0)
    {
        if (r_s16(commands + 2) == 0)
        {
            sint32 health = r_s16(commands);
            uint32 health_half = r_u16(commands);
            if (health != 0)
                w_u16(state + 0x3a, health_half);
            else
            {
                health_half = r_u16(state + 0x3a);
                w_u16(commands, health_half);
            }
            w_u32(state + 8, commands);
            commands += 4;
        }
        G3SantaConfigure(object, commands, frame);
    }
    w_u16(object + 0x6e, 0);
    w_u16(object + 0x5e, 0);
    uint32 source = r_u32(state + 0x10);
    if (source != 0)
    {
        w_u16(state + 0x16, 0);
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 lane = source & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
        uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
        uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        uint32 last = (uint32)r_s16(source + 4);
        uint32 destination = object + 0x58;
        high_address = destination + 3;
        high_lane = high_address & 3;
        lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
        w_u16(destination + 4, last);
    }
    sint32 end_frame = r_s16(state + 0x38);
    sint32 start_frame = r_s16(state + 0x36);
    sint32 duration = end_frame - start_frame;
    uint32 step = duration == 0 ? 0xffffffff : (uint32)(4096 / duration);
    w_u16(state + 0x1a, step);
    uint32 yaw = r_u16(object + 0x64);
    w_u16(state + 0x20, yaw);
}
