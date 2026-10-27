#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32 G3CactusEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_59");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3CactusInitialize(object);
        return 1;
    case 2:
        G3CactusUpdate(object, game, module_base, frame);
        return 1;
    case 3:
        G3CactusCollide(object, game, frame);
        return 1;
    default:
        return 0;
    }
}

void G3CactusInitialize(uint32 object)
{
    FUNCTION_MARKER(0x0000007c, "G3L_59");
    uint32 data = r_u32(object + 0x28);
    uint32 x = r_u16(object + 0x58);
    uint32 state = object + 0x104;
    w_u16(state + 0x14, x);
    w_u16(state + 0x16, r_u16(object + 0x5a));
    w_u16(state + 0x18, r_u16(object + 0x5c));
    if (data != 0)
    {
        w_u16(state, r_u16(data + 0x0));
        uint32 radius = (uint32)r_s16(data + 0x2);
        w_u32(state + 4, radius * radius);
        radius = (uint32)r_s16(data + 0x4);
        w_u32(state + 8, radius * radius);
        if (r_s16(data) != 0)
        {
            uint32 origin_x = (uint32)r_s16(state + 0x14);
            uint32 offset_x = (uint32)r_s16(data + 0xa);
            uint32 half_width = (uint32)r_s16(data + 0x6);
            uint32 minimum_x = origin_x + offset_x - half_width;
            w_u32(state + 0x1c, minimum_x);
            half_width = (uint32)r_s16(data + 0x6);
            w_u32(state + 0x24, minimum_x + (half_width << 1));
            uint32 origin_y = (uint32)r_s16(state + 0x16);
            uint32 offset_y = (uint32)r_s16(data + 0xc);
            uint32 half_height = (uint32)r_s16(data + 0x8);
            uint32 minimum_y = origin_y + offset_y - half_height;
            w_u32(state + 0x20, minimum_y);
            half_height = (uint32)r_s16(data + 0x8);
            w_u32(state + 0x28, minimum_y + (half_height << 1));
        }
    }
    else
    {
        uint32 resource = r_u32(object + 0x1c);
        data = r_u32(resource + 0x1c);
        w_u16(state, r_u16(data + 0x14));
        uint32 radius = (uint32)r_s16(data + 0x10);
        w_u32(state + 4, radius * radius);
        radius = (uint32)r_s16(data + 0x12);
        w_u32(state + 8, radius * radius);
        if (r_s16(state) != 0)
        {
            uint32 origin_x = (uint32)r_s16(state + 0x14);
            uint32 offset_x = (uint32)r_s16(data + 0x1a);
            uint32 half_width = (uint32)r_s16(data + 0x16);
            uint32 minimum_x = origin_x + offset_x - half_width;
            w_u32(state + 0x1c, minimum_x);
            half_width = (uint32)r_s16(data + 0x16);
            w_u32(state + 0x24, minimum_x + (half_width << 1));
            uint32 origin_y = (uint32)r_s16(state + 0x16);
            uint32 offset_y = (uint32)r_s16(data + 0x1c);
            uint32 half_height = (uint32)r_s16(data + 0x18);
            uint32 minimum_y = origin_y + offset_y - half_height;
            w_u32(state + 0x20, minimum_y);
            half_height = (uint32)r_s16(data + 0x18);
            w_u32(state + 0x28, minimum_y + (half_height << 1));
        }
    }
    uint32 minimum_x = r_u32(state + 0x1c);
    uint32 maximum_x = r_u32(state + 0x24);
    uint32 maximum_y = r_u32(state + 0x28);
    uint32 middle_x = minimum_x + maximum_x;
    uint32 minimum_y = r_u32(state + 0x20);
    w_u32(state + 0x2c, (uint32)((sint32)middle_x >> 1));
    w_u32(state + 0x30, (uint32)((sint32)(minimum_y + maximum_y) >> 1));
    uint32 yaw = (uint32)r_s16(object + 0x64);
    w_u32(object + 0x150, 0);
    w_u16(object + 0x6e, 0);
    w_u16(object + 0x5e, 0);
    w_u32(state + 0x10, yaw);
}

uint32 G3CactusMoveToward(uint32 object, uint32 target, uint32 speed, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000228, "G3L_59");
    uint32 frame = guest_stack - 0x28;
    uint32 target_x = r_u16(target);
    uint32 x = r_u16(object + 0x58);
    uint32 dx = target_x - x;
    uint32 squared_x = (uint32)(sint32)(sint16)dx * (uint32)(sint32)(sint16)dx;
    w_u16(frame + 0x18, dx);
    uint32 target_y = r_u16(target + 2);
    uint32 y = r_u16(object + 0x5a);
    uint32 dy = target_y - y;
    uint32 squared_y = (uint32)(sint32)(sint16)dy * (uint32)(sint32)(sint16)dy;
    uint32 squared_speed = speed * speed;
    w_u16(frame + 0x1c, 0);
    uint32 distance = squared_x + squared_y;
    bool outside = (sint32)squared_speed < (sint32)distance;
    w_u16(frame + 0x1a, dy);
    if (outside)
    {
        G3Function8002F9A8(frame + 0x10, (uint32)(sint32)(sint16)speed, frame + 0x18, frame);
        x = r_u16(object + 0x58);
        dx = r_u16(frame + 0x10);
        w_u16(object + 0x58, x + dx);
        y = r_u16(object + 0x5a);
        dy = r_u16(frame + 0x12);
        uint32 z = r_u16(object + 0x5c);
        w_u16(object + 0x5a, y + dy);
        uint32 dz = r_u16(frame + 0x14);
        w_u16(object + 0x5c, z + dz);
        return 0;
    }
    w_u16(object + 0x58, r_u16(target));
    w_u16(object + 0x5a, r_u16(target + 2));
    return 1;
}

void G3CactusUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000031c, "G3L_59");
    uint32 frame = guest_stack - 0x38;
    uint32 player = r_u32(game + 0x50);
    uint32 state = object + 0x104;
    uint32 flags = r_u32(object + 0x14);
    uint32 resource = r_u32(object + 0x1c);
    flags &= 0xffffffef;
    uint32 data = r_u32(resource + 0x1c);
    uint32 player_position = player + 0x58;
    w_u32(object + 0x14, flags);
    G3Function80029F0C(object, 0xffffffff, frame);
    uint32 mode = r_u32(object + 0x150);
    if (mode >= 10)
        return;
    uint32 target = r_u32(module_base + 0xa78 + mode * 4);
    switch (target - module_base)
    {
    case 0x394:
    {
        uint32 first_x = r_u16(state + 0x14);
        uint32 second_x = r_u16(player_position + 0);
        uint32 delta_x = first_x - second_x;
        uint32 squared_x = (uint32)(sint32)(sint16)delta_x * (uint32)(sint32)(sint16)delta_x;
        w_u16(frame + 0x10, delta_x);
        uint32 first_y = r_u16(state + 0x16);
        uint32 second_y = r_u16(player_position + 2);
        uint32 delta_y = first_y - second_y;
        uint32 squared_y = (uint32)(sint32)(sint16)delta_y * (uint32)(sint32)(sint16)delta_y;
        w_u16(frame + 0x12, delta_y);
        uint32 first_z = r_u16(state + 0x18);
        uint32 second_z = r_u16(player_position + 4);
        uint32 delta_z = first_z - second_z;
        uint32 squared_z = (uint32)(sint32)(sint16)delta_z * (uint32)(sint32)(sint16)delta_z;
        w_u16(frame + 0x14, delta_z);
        uint32 distance = squared_x + squared_y + squared_z;
        sint32 radius = r_s32(state + 4);
        if (radius < (sint32)distance)
            return;
        w_u32(object + 0x150, 1);
        w_u16(object + 0x6e, 1);
        w_u16(object + 0x5e, 0);
        uint32 y = (uint32)r_s16(frame + 0x12);
        uint32 x = (uint32)r_s16(frame + 0x10);
        uint32 heading = G3Function8002F240(y, x, frame);
        uint32 desired = (uint32)(sint32)(sint16)(heading - 0x400);
        uint32 yaw = (uint32)r_s16(object + 0x64);
        uint32 difference = G3Function80038B48(yaw, desired);
        w_u32(state + 0xc, (uint32)((sint32)(sint16)difference / 25));
        return;
    }
    case 0x474:
    {
        uint32 yaw = r_u16(object + 0x64);
        uint32 turn = r_u16(state + 0xc);
        uint32 finished = r_u32(object + 0x14) & 0x10;
        w_u16(object + 0x64, yaw + turn);
        if (finished != 0)
        {
            w_u32(object + 0x150, 2);
            w_u16(object + 0x6e, 2);
            w_u16(object + 0x5e, 0);
        }
        return;
    }
    case 0x49c:
        if ((r_u32(object + 0x14) & 0x10) != 0)
        {
            w_u32(object + 0x150, r_s16(state) != 0 ? 6 : 3);
            w_u16(object + 0x6e, 3);
            w_u16(object + 0x5e, 0);
            w_u16(state + 2, r_u16(data));
        }
        return;
    case 0x4e8:
    {
        uint32 timer = r_u16(state + 2) - 1;
        w_u16(state + 2, timer);
        if ((sint16)timer <= 0)
        {
            w_u32(object + 0x150, 4);
            w_u16(object + 0x6e, 4);
            w_u16(object + 0x5e, 0);
            w_u16(object + 0x64, r_u16(state + 0x10));
        }
        return;
    }
    case 0x50c:
    {
        uint32 first_x = r_u16(player_position + 0);
        uint32 second_x = r_u16(state + 0x14);
        uint32 delta_x = first_x - second_x;
        uint32 squared_x = (uint32)(sint32)(sint16)delta_x * (uint32)(sint32)(sint16)delta_x;
        w_u16(frame + 0x10, delta_x);
        uint32 first_y = r_u16(player_position + 2);
        uint32 second_y = r_u16(state + 0x16);
        uint32 delta_y = first_y - second_y;
        uint32 squared_y = (uint32)(sint32)(sint16)delta_y * (uint32)(sint32)(sint16)delta_y;
        w_u16(frame + 0x12, delta_y);
        uint32 first_z = r_u16(player_position + 4);
        uint32 second_z = r_u16(state + 0x18);
        uint32 delta_z = first_z - second_z;
        uint32 squared_z = (uint32)(sint32)(sint16)delta_z * (uint32)(sint32)(sint16)delta_z;
        w_u16(frame + 0x14, delta_z);
        uint32 distance = squared_x + squared_y + squared_z;
        sint32 radius = r_s32(state + 8);
        if (radius < (sint32)distance)
        {
            w_u32(object + 0x150, 5);
            w_u16(object + 0x6e, 5);
            w_u16(object + 0x5e, 0);
        }
        return;
    }
    case 0x598:
        if ((r_u32(object + 0x14) & 0x10) != 0)
        {
            w_u32(object + 0x150, 0);
            w_u16(object + 0x6e, 0);
            w_u16(object + 0x5e, 0);
        }
        return;
    case 0x5b8:
    {
        G3CactusMoveToward(object, player_position, (uint32)r_s16(data + 4), frame);
        sint32 x = r_s16(object + 0x58);
        sint32 minimum_x = r_s32(state + 0x1c);
        if (x < minimum_x)
            w_u16(object + 0x58, r_u16(state + 0x1c));
        else
        {
            sint32 maximum_x = r_s32(state + 0x24);
            if (maximum_x < x)
                w_u16(object + 0x58, r_u16(state + 0x24));
        }
        sint32 y = r_s16(object + 0x5a);
        sint32 minimum_y = r_s32(state + 0x20);
        if (y < minimum_y)
            w_u16(object + 0x5a, r_u16(state + 0x20));
        else
        {
            sint32 maximum_y = r_s32(state + 0x28);
            if (maximum_y < y)
                w_u16(object + 0x5a, r_u16(state + 0x28));
        }
        uint32 timer = r_u16(state + 2) - 1;
        w_u16(state + 2, timer);
        if ((sint16)timer <= 0)
        {
            w_u32(object + 0x150, 7);
            w_u16(object + 0x6e, 4);
            w_u16(object + 0x5e, 0);
            w_u16(object + 0x64, r_u16(state + 0x10));
        }
        return;
    }
    case 0x688:
    {
        G3CactusMoveToward(object, player_position, (uint32)r_s16(data + 6), frame);
        sint32 x = r_s16(object + 0x58);
        sint32 minimum_x = r_s32(state + 0x1c);
        if (x < minimum_x)
            w_u16(object + 0x58, r_u16(state + 0x1c));
        else
        {
            sint32 maximum_x = r_s32(state + 0x24);
            if (maximum_x < x)
                w_u16(object + 0x58, r_u16(state + 0x24));
        }
        sint32 y = r_s16(object + 0x5a);
        sint32 minimum_y = r_s32(state + 0x20);
        if (y < minimum_y)
            w_u16(object + 0x5a, r_u16(state + 0x20));
        else
        {
            sint32 maximum_y = r_s32(state + 0x28);
            if (maximum_y < y)
                w_u16(object + 0x5a, r_u16(state + 0x28));
        }
        uint32 first_x = r_u16(player_position + 0);
        uint32 second_x = r_u16(state + 0x14);
        uint32 delta_x = first_x - second_x;
        uint32 squared_x = (uint32)(sint32)(sint16)delta_x * (uint32)(sint32)(sint16)delta_x;
        w_u16(frame + 0x10, delta_x);
        uint32 first_y = r_u16(player_position + 2);
        uint32 second_y = r_u16(state + 0x16);
        uint32 delta_y = first_y - second_y;
        uint32 squared_y = (uint32)(sint32)(sint16)delta_y * (uint32)(sint32)(sint16)delta_y;
        w_u16(frame + 0x12, delta_y);
        uint32 first_z = r_u16(player_position + 4);
        uint32 second_z = r_u16(state + 0x18);
        uint32 delta_z = first_z - second_z;
        uint32 squared_z = (uint32)(sint32)(sint16)delta_z * (uint32)(sint32)(sint16)delta_z;
        w_u16(frame + 0x14, delta_z);
        uint32 distance = squared_x + squared_y + squared_z;
        sint32 radius = r_s32(state + 8);
        if (radius < (sint32)distance)
        {
            w_u32(object + 0x150, 8);
            w_u16(object + 0x6e, 4);
            w_u16(object + 0x5e, 0);
        }
        return;
    }
    case 0x7ac:
    {
        uint32 first_x = r_u16(player_position + 0);
        uint32 second_x = r_u16(state + 0x14);
        uint32 delta_x = first_x - second_x;
        uint32 squared_x = (uint32)(sint32)(sint16)delta_x * (uint32)(sint32)(sint16)delta_x;
        w_u16(frame + 0x10, delta_x);
        uint32 first_y = r_u16(player_position + 2);
        uint32 second_y = r_u16(state + 0x16);
        uint32 delta_y = first_y - second_y;
        uint32 squared_y = (uint32)(sint32)(sint16)delta_y * (uint32)(sint32)(sint16)delta_y;
        w_u16(frame + 0x12, delta_y);
        uint32 first_z = r_u16(player_position + 4);
        uint32 second_z = r_u16(state + 0x18);
        uint32 delta_z = first_z - second_z;
        uint32 squared_z = (uint32)(sint32)(sint16)delta_z * (uint32)(sint32)(sint16)delta_z;
        w_u16(frame + 0x14, delta_z);
        uint32 distance = squared_x + squared_y + squared_z;
        sint32 radius = r_s32(state + 4);
        if ((sint32)distance < radius)
        {
            w_u32(object + 0x150, 7);
            w_u16(object + 0x6e, 4);
            w_u16(object + 0x5e, 0);
        }
        else if (G3CactusMoveToward(object, state + 0x14, (uint32)r_s16(data + 8), frame) != 0)
        {
            w_u32(object + 0x150, 5);
            w_u16(object + 0x6e, 5);
            w_u16(object + 0x5e, 0);
        }
        return;
    }
    case 0x850:
    {
        sint32 player_x = r_s16(player_position);
        sint32 middle_x = r_s32(state + 0x2c);
        uint32 x = middle_x < player_x ? r_u16(state + 0x1c) : r_u16(state + 0x24);
        w_u16(frame + 0x18, x);
        sint32 player_y = r_s16(player_position + 2);
        sint32 middle_y = r_s32(state + 0x30);
        uint32 y = middle_y < player_y ? r_u16(state + 0x20) : r_u16(state + 0x28);
        w_u16(frame + 0x1a, y);
        w_u16(frame + 0x1c, r_u16(state + 0x18));
        G3CactusMoveToward(object, frame + 0x18, (uint32)r_s16(data + 0xa), frame);
        uint32 timer = r_u16(state + 2) - 1;
        w_u16(state + 2, timer);
        if ((sint16)timer <= 0)
        {
            w_u32(object + 0x150, 7);
            w_u16(object + 0x6e, 4);
            w_u16(object + 0x5e, 0);
        }
        return;
    }
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved cactus update target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}

void G3CactusCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000910, "G3L_59");
    uint32 frame = guest_stack - 0x30;
    uint32 contact = r_u32(object + 0xb0);
    uint32 resource = r_u32(object + 0x1c);
    uint32 player = r_u32(game + 0x50);
    uint32 data = r_u32(resource + 0x1c);
    G3Function8003C548(contact, frame + 0x10, frame + 0x11);
    if (r_u32(contact + 0x14) != player)
        return;
    uint32 first = r_u8(frame + 0x10);
    if (first - 6 < 4)
    {
        uint32 second = r_u8(frame + 0x11);
        if (second == 9 || second - 3 < 3)
        {
            G3Function80074044(object, game, frame);
            return;
        }
    }
    uint32 second = r_u8(frame + 0x11);
    if (second - 6 < 4 && r_s8(frame + 0x10) == 0 && (r_u32(player + 0x154) & 0x20) != 0)
    {
        uint32 selector = (uint32)r_s16(data + 0xc);
        uint32 random_range = (uint32)r_s16(data + 0xe);
        G3Function80038798(object, selector, random_range, frame);
        return;
    }
    if (r_u32(object + 0x150) != 7 || r_u32(player + 0x154) != 0x40)
        return;
    sint32 player_z = r_s16(player + 0x5c);
    sint32 z = r_s16(object + 0x5c);
    if (player_z - z >= 121)
    {
        w_u32(object + 0x150, 9);
        w_u16(object + 0x6e, 4);
        w_u16(object + 0x5e, 0);
        w_u16(object + 0x106, r_u16(data + 2));
    }
}

uint32 G3CamelEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_25");
    uint32 frame = guest_stack - 0x28;
    uint32 settings = 0;
    uint32 state = 0;
    if (object != 0)
    {
        uint32 descriptor = r_u32(object + 0x1c);
        state = object + 0x104;
        settings = r_u32(descriptor + 0x1c);
    }
    if (settings == 0) return 1;
    switch (message)
    {
    case 1: G3CamelInitialize(object, game, state, frame); return 1;
    case 2:
        if (r_s16(state) == 3) G3CamelUpdate(object, game, state, settings, module_base, frame);
        return 1;
    case 3: G3CamelCollide(object, game, state, settings, frame); return 1;
    case 8:
        w_u32(frame + 0x10, payload);
        G3CamelDamage(object, game, state, frame);
        return 1;
    case 0x3f2:
    {
        uint32 player = r_u32(game + 0x50);
        w_u16(object + 0x5c, r_u16(player + 0x100) - 40);
        w_u8(state + 0x2c, 1);
        return 1;
    }
    case 0x7ea:
        G3CamelUpdate(object, game, state, settings, module_base, frame);
        w_u8(state + 0x2c, 0);
        return 1;
    case 0x7eb: w_u8(state + 0x34, 1); return 1;
    case 0x7ed:
        {
            uint32 source = object + 0x58;
            uint32 destination = payload;
            uint32 high_address0 = source + 3;
            uint32 high_lane0 = high_address0 & 3;
            uint32 lane0 = (source + 0) & 3;
            uint32 high0 = r_masked_u32(high_address0 & 0xfffffffc, (1u << (high_lane0 + 1)) - 1);
            uint32 low0 = r_masked_u32((source + 0) & 0xfffffffc, (15u << lane0) & 15);
            uint32 keep0 = lane0 == 0 ? 0 : 0xffffffff << ((4 - lane0) * 8);
            uint32 value0 = ((high0 << ((3 - high_lane0) * 8)) & keep0) | (low0 >> (lane0 * 8));
            uint32 last = (uint32)r_s16(source + 4);
            high_address0 = destination + 3;
            high_lane0 = high_address0 & 3;
            lane0 = (destination + 0) & 3;
            w_masked_u32(high_address0 & 0xfffffffc, value0 >> ((3 - high_lane0) * 8), (1u << (high_lane0 + 1)) - 1);
            w_masked_u32((destination + 0) & 0xfffffffc, value0 << (lane0 * 8), (15u << lane0) & 15);
            w_u16(destination + 4, last);
        }
        w_u16(payload + 4, r_u16(payload + 4) + 550);
        return 1;
    case 0x7f1:
    case 0x7f2:
        if (r_s16(state + 2) != 0) return 0;
        return r_s16(state + 0xa) == 0 ? 1 : 0;
    case 0x7f3:
    {
        uint32 player = r_u32(0x8009efc4);
        sint32 z = r_s16(object + 0x5c);
        sint32 floor = r_s16(player + 0xe0);
        sint32 difference = z - floor;
        if (difference < 0) difference = -difference;
        return difference < 5 ? 1 : 0;
    }
    default: return 0;
    }
}

void G3CamelInitialize(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000208, "G3L_25");
    uint32 frame = guest_stack - 0x18;
    w_u32(object + 0x10, r_u32(object + 0x10) | 0x100000);
    G3Function80082988(object, 0, game, frame);
    w_u16(state, 0);
    w_u16(state + 0xe, 0);
    w_u16(state + 0x18, 0);
    w_u16(state + 0x1a, 0);
    w_u16(state + 0x1c, 0);
    w_u16(state + 6, 0);
    w_u8(state + 0x2f, 0);
    w_u8(state + 0x34, 0);
}

void G3CamelDust(uint32 position, uint32 heading, uint32 speed, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000268, "G3L_25");
    uint32 frame = guest_stack - 0x40;
    uint32 source = module_base + 0x18ec;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 lane = source & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
    uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
    uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
    uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
    uint32 last = (uint32)r_s16(source + 4);
    uint32 destination = frame + 0x10;
    high_address = destination + 3;
    high_lane = high_address & 3;
    lane = destination & 3;
    w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
    w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
    w_u16(frame + 0x14, last);
    uint32 resource = r_u32(0x80098724);
    if (resource == 0) return;
    heading = (uint32)(sint32)(sint16)heading;
    uint32 sine = G3Function8005B470(heading);
    uint32 x = (uint32)((sint32)(sine * speed) >> 12);
    uint32 cosine = G3Function8005B590(heading);
    uint32 y = (uint32)((sint32)(cosine * speed) >> 12);
    uint32 random = G3SdkRandom80027FA8();
    uint32 origin = r_u16(position);
    w_u16(frame + 0x18, origin + x * 8 + (random & 31) - 7);
    random = G3SdkRandom80027FA8();
    origin = r_u16(position + 2);
    w_u16(frame + 0x1a, origin + y * 8 + (random & 31) - 7);
    random = G3SdkRandom80027FA8();
    origin = r_u16(position + 4);
    w_u16(frame + 0x1c, origin + (random & 15) + 5);
    uint32 models = r_u32(resource + 0xc);
    uint32 pool = r_u32(0x800a27fc);
    uint32 model = r_u32(models);
    uint32 effect = G3Function800444E4(frame + 0x18, model, pool, 20, frame);
    if (effect != 0)
    {
        G3Function800449F0(effect, x, y, 8, effect);
        G3Function80044878(effect, frame + 0x10, 0);
    }
}

uint32 G3CamelSteer(uint32 game, uint32 object, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000003b4, "G3L_25");
    uint32 frame = guest_stack - 0x30;
    uint32 target = 0;
    uint32 corrected = 0;
    uint32 y = r_u32(game + 0xa8);
    bool input = y != 0;
    if (!input) input = r_u32(game + 0xa4) != 0;
    if (input)
    {
        uint32 x = r_u32(game + 0xa4);
        target = G3Function8002F240(0u - y, x, frame) + 0x400;
        if ((target & 0xffff) == 0) target = 0x1000;
        y = r_u32(game + 0xa8);
        uint32 yy = y * y;
        x = r_u32(game + 0xa4);
        if ((sint32)(yy + x * x) < 0x2400)
        {
            uint32 player = r_u32(0x8009efc4);
            w_u32(player + 0x128, r_u32(player + 0x128) | 0x20);
        }
    }
    uint32 input_heading = (uint32)(sint32)(sint16)target;
    if (input_heading != 0) corrected = target + G3Function80014F00(game);
    if (r_s16(state + 2) == 5) corrected = 0;
    uint32 heading = (uint32)(sint32)(sint16)corrected;
    if (heading != 0)
    {
        uint32 step = (uint32)r_s16(settings + 4);
        G3Function80038AA8(state + 4, heading, step);
        heading = (uint32)r_s16(state + 4);
        step = (uint32)r_s16(settings + 6);
        G3Function80038AA8(object + 0x64, heading, step);
    }
    w_u16(state + 8, corrected);
    return input_heading;
}

void G3CamelSpeed(uint32 input, uint32 state, uint32 settings)
{
    FUNCTION_MARKER(0x00000504, "G3L_25");
    if (r_s16(state + 2) == 5) return;
    bool accelerate = (input & 0xffff) != 0;
    if (accelerate) accelerate = r_u8(state + 0x2d) != 0;
    if (accelerate)
    {
        sint32 count = r_s16(settings + 0x10);
        uint32 table = r_u32(settings + 0x1c);
        sint32 maximum = r_s16(table + (uint32)count * 2 - 2);
        sint32 speed = r_s16(state + 0xa);
        --maximum;
        uint32 bits = r_u16(state + 0xa);
        if (speed < maximum) w_u16(state + 0xa, bits + 1);
    }
    else
    {
        sint32 speed = r_s16(state + 0xa);
        sint32 step = r_s16(settings + 0x20);
        uint32 step_bits = r_u16(settings + 0x20);
        uint32 speed_bits = r_u16(state + 0xa);
        w_u16(state + 0xa, step < speed ? speed_bits - step_bits : 0);
    }
}

void G3CamelAnimate(uint32 object, uint32 game, uint32 state, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000590, "G3L_25");
    uint32 frame = guest_stack - 0x28;
    sint32 speed = r_s16(state + 0xa);
    uint32 player = r_u32(game + 0x50);
    if (speed != 0)
    {
        G3Function8002A1C8(object, 0xffffffff, frame);
        if (r_u16(object + 0x6e) == 0)
        {
            w_u16(object + 0x6e, 5);
            w_u16(object + 0x5e, 0);
            w_u16(player + 0x6e, 0x32);
        }
        else if ((r_u32(object + 0x14) & 0x10) != 0)
        {
            w_u16(object + 0x6e, 1);
            w_u16(player + 0x6e, 0x31);
        }
        uint32 mode = r_u16(state + 2);
        if (mode - 1u < 2)
        {
            w_u16(object + 0x6e, 4);
            w_u16(player + 0x6e, 0x33);
        }
        else
        {
            uint32 animation = r_u16(object + 0x6e);
            if (animation == 4 || animation == 2)
            {
                w_u16(object + 0x6e, 1);
                w_u16(player + 0x6e, 0x31);
            }
        }
        uint32 phase = r_u16(player + 0x5e) & 7;
        uint32 random = G3SdkRandom80027FA8();
        if (phase == (random & 7) && r_s16(state + 2) == 0)
        {
            uint32 heading = (uint32)(sint32)(sint16)(r_u16(object + 0x64) + 0x400);
            G3CamelDust(object + 0x58, heading, 4, module_base, frame);
        }
    }
    else
    {
        G3Function8002A1C8(object, 0xffffffff, frame);
        uint32 mode = r_u16(state + 2);
        if (mode - 1u < 2)
        {
            uint32 animation = r_u16(object + 0x6e);
            if (animation != 2 && animation != 4)
            {
                w_u16(object + 0x6e, 2);
                w_u16(object + 0x5e, 0);
                w_u16(player + 0x6e, 0x33);
            }
        }
        else
        {
            uint32 animation = r_u16(object + 0x6e);
            if (animation != 0 && animation != 3)
            {
                w_u16(object + 0x6e, 0);
                w_u16(object + 0x5e, 0);
                w_u16(player + 0x6e, 0x2a);
            }
        }
    }
    w_u16(player + 0x5e, r_u16(object + 0x5e));
}

void G3CamelBank(uint32 object, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000728, "G3L_25");
    uint32 frame = guest_stack - 0x68;
    uint32 desired = (uint32)r_s16(state + 8);
    uint32 target = 0;
    if (desired != 0)
    {
        uint32 heading = (uint32)r_s16(object + 0x64);
        uint32 turn = G3Function80038B48(heading, desired);
        sint32 speed = r_s16(state + 0xa);
        sint32 scale = r_s16(settings + 0x2a);
        sint32 numerator = speed * scale;
        sint32 denominator = r_s16(settings + 0x2c);
        sint32 quotient = denominator == 0 ? (numerator < 0 ? 1 : -1) : numerator / denominator;
        target = 0u - turn;
        if ((sint16)quotient < (sint16)target) target = (uint32)quotient;
        if ((sint16)target < -(sint32)(sint16)quotient) target = 0u - (uint32)quotient;
    }
    G3Function80038AA8(state + 6, (uint32)(sint32)(sint16)target, 32);
    G3Function8005BD00(object + 0x60, frame + 0x10);
    w_u16(frame + 0x50, 0);
    uint32 bank = r_u16(state + 6);
    w_u16(frame + 0x54, 0);
    w_u16(frame + 0x52, 0u - bank);
    G3Function8005BD00(frame + 0x50, frame + 0x30);
    MulMatrix0((MATRIX *)psx_addr(frame + 0x10, 20), (MATRIX *)psx_addr(frame + 0x30, 20), (MATRIX *)psx_addr(frame + 0x30, 20));
    G3Function800202A4(frame + 0x30, object + 0x60, 0x15, frame);
}

void G3CamelAlign(uint32 object, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000084c, "G3L_25");
    uint32 frame = guest_stack - 0x50;
    uint32 game = r_u32(0x80097048);
    {
        uint32 source = module_base + 0x18f4;
        uint32 destination = frame + 0x30;
        uint32 high_address0 = source + 3;
        uint32 high_lane0 = high_address0 & 3;
        uint32 lane0 = (source + 0) & 3;
        uint32 high0 = r_masked_u32(high_address0 & 0xfffffffc, (1u << (high_lane0 + 1)) - 1);
        uint32 low0 = r_masked_u32((source + 0) & 0xfffffffc, (15u << lane0) & 15);
        uint32 keep0 = lane0 == 0 ? 0 : 0xffffffff << ((4 - lane0) * 8);
        uint32 value0 = ((high0 << ((3 - high_lane0) * 8)) & keep0) | (low0 >> (lane0 * 8));
        uint32 high_address1 = source + 7;
        uint32 high_lane1 = high_address1 & 3;
        uint32 lane1 = (source + 4) & 3;
        uint32 high1 = r_masked_u32(high_address1 & 0xfffffffc, (1u << (high_lane1 + 1)) - 1);
        uint32 low1 = r_masked_u32((source + 4) & 0xfffffffc, (15u << lane1) & 15);
        uint32 keep1 = lane1 == 0 ? 0 : 0xffffffff << ((4 - lane1) * 8);
        uint32 value1 = ((high1 << ((3 - high_lane1) * 8)) & keep1) | (low1 >> (lane1 * 8));
        high_address0 = destination + 3;
        high_lane0 = high_address0 & 3;
        lane0 = (destination + 0) & 3;
        w_masked_u32(high_address0 & 0xfffffffc, value0 >> ((3 - high_lane0) * 8), (1u << (high_lane0 + 1)) - 1);
        w_masked_u32((destination + 0) & 0xfffffffc, value0 << (lane0 * 8), (15u << lane0) & 15);
        high_address1 = destination + 7;
        high_lane1 = high_address1 & 3;
        lane1 = (destination + 4) & 3;
        w_masked_u32(high_address1 & 0xfffffffc, value1 >> ((3 - high_lane1) * 8), (1u << (high_lane1 + 1)) - 1);
        w_masked_u32((destination + 4) & 0xfffffffc, value1 << (lane1 * 8), (15u << lane1) & 15);
    }
    uint32 player = r_u32(game + 0x50);
    sint32 z = r_s16(object + 0x5c);
    sint32 floor = r_s16(player + 0xe0);
    sint32 difference = z - floor;
    if (difference < 0) difference = -difference;
    uint32 normal = frame + 0x30;
    if (difference < 65 && r_u8(state + 0x2c) == 0)
    {
        game = r_u32(0x80097048);
        player = r_u32(game + 0x50);
        normal = player + 0xb4;
    }
    if (r_s16(normal + 4) < 0xb50) normal = frame + 0x30;
    if (r_s16(state + 0x28) == 0)
    {
        bool changed = r_s16(state + 0x18) != r_s16(normal);
        if (!changed) changed = r_s16(state + 0x1a) != r_s16(normal + 2);
        if (!changed) changed = r_s16(state + 0x1c) != r_s16(normal + 4);
        if (changed)
        {
            w_u16(state + 0x28, 1);
            {
                uint32 source = state + 0x18;
                uint32 destination = state + 0x10;
                uint32 high_address0 = source + 3;
                uint32 high_lane0 = high_address0 & 3;
                uint32 lane0 = (source + 0) & 3;
                uint32 high0 = r_masked_u32(high_address0 & 0xfffffffc, (1u << (high_lane0 + 1)) - 1);
                uint32 low0 = r_masked_u32((source + 0) & 0xfffffffc, (15u << lane0) & 15);
                uint32 keep0 = lane0 == 0 ? 0 : 0xffffffff << ((4 - lane0) * 8);
                uint32 value0 = ((high0 << ((3 - high_lane0) * 8)) & keep0) | (low0 >> (lane0 * 8));
                uint32 high_address1 = source + 7;
                uint32 high_lane1 = high_address1 & 3;
                uint32 lane1 = (source + 4) & 3;
                uint32 high1 = r_masked_u32(high_address1 & 0xfffffffc, (1u << (high_lane1 + 1)) - 1);
                uint32 low1 = r_masked_u32((source + 4) & 0xfffffffc, (15u << lane1) & 15);
                uint32 keep1 = lane1 == 0 ? 0 : 0xffffffff << ((4 - lane1) * 8);
                uint32 value1 = ((high1 << ((3 - high_lane1) * 8)) & keep1) | (low1 >> (lane1 * 8));
                high_address0 = destination + 3;
                high_lane0 = high_address0 & 3;
                lane0 = (destination + 0) & 3;
                w_masked_u32(high_address0 & 0xfffffffc, value0 >> ((3 - high_lane0) * 8), (1u << (high_lane0 + 1)) - 1);
                w_masked_u32((destination + 0) & 0xfffffffc, value0 << (lane0 * 8), (15u << lane0) & 15);
                high_address1 = destination + 7;
                high_lane1 = high_address1 & 3;
                lane1 = (destination + 4) & 3;
                w_masked_u32(high_address1 & 0xfffffffc, value1 >> ((3 - high_lane1) * 8), (1u << (high_lane1 + 1)) - 1);
                w_masked_u32((destination + 4) & 0xfffffffc, value1 << (lane1 * 8), (15u << lane1) & 15);
            }
            {
                uint32 source = normal;
                uint32 destination = state + 0x18;
                uint32 high_address0 = source + 3;
                uint32 high_lane0 = high_address0 & 3;
                uint32 lane0 = (source + 0) & 3;
                uint32 high0 = r_masked_u32(high_address0 & 0xfffffffc, (1u << (high_lane0 + 1)) - 1);
                uint32 low0 = r_masked_u32((source + 0) & 0xfffffffc, (15u << lane0) & 15);
                uint32 keep0 = lane0 == 0 ? 0 : 0xffffffff << ((4 - lane0) * 8);
                uint32 value0 = ((high0 << ((3 - high_lane0) * 8)) & keep0) | (low0 >> (lane0 * 8));
                uint32 high_address1 = source + 7;
                uint32 high_lane1 = high_address1 & 3;
                uint32 lane1 = (source + 4) & 3;
                uint32 high1 = r_masked_u32(high_address1 & 0xfffffffc, (1u << (high_lane1 + 1)) - 1);
                uint32 low1 = r_masked_u32((source + 4) & 0xfffffffc, (15u << lane1) & 15);
                uint32 keep1 = lane1 == 0 ? 0 : 0xffffffff << ((4 - lane1) * 8);
                uint32 value1 = ((high1 << ((3 - high_lane1) * 8)) & keep1) | (low1 >> (lane1 * 8));
                high_address0 = destination + 3;
                high_lane0 = high_address0 & 3;
                lane0 = (destination + 0) & 3;
                w_masked_u32(high_address0 & 0xfffffffc, value0 >> ((3 - high_lane0) * 8), (1u << (high_lane0 + 1)) - 1);
                w_masked_u32((destination + 0) & 0xfffffffc, value0 << (lane0 * 8), (15u << lane0) & 15);
                high_address1 = destination + 7;
                high_lane1 = high_address1 & 3;
                lane1 = (destination + 4) & 3;
                w_masked_u32(high_address1 & 0xfffffffc, value1 >> ((3 - high_lane1) * 8), (1u << (high_lane1 + 1)) - 1);
                w_masked_u32((destination + 4) & 0xfffffffc, value1 << (lane1 * 8), (15u << lane1) & 15);
            }
            uint32 angle = G3Function8002F540(state + 0x18, state + 0x10, frame);
            uint32 y = (uint32)r_s16(state + 0x1a);
            uint32 old_z = (uint32)r_s16(state + 0x14);
            uint32 yz = y * old_z;
            uint32 z_axis = (uint32)r_s16(state + 0x1c);
            uint32 old_y = (uint32)r_s16(state + 0x12);
            uint32 zy = z_axis * old_y;
            uint32 x = (uint32)r_s16(state + 0x18);
            old_z = (uint32)r_s16(state + 0x14);
            uint32 xz = x * old_z;
            z_axis = (uint32)r_s16(state + 0x1c);
            uint32 old_x = (uint32)r_s16(state + 0x10);
            uint32 zx = z_axis * old_x;
            x = (uint32)r_s16(state + 0x18);
            old_y = (uint32)r_s16(state + 0x12);
            uint32 xy = x * old_y;
            y = (uint32)r_s16(state + 0x1a);
            old_x = (uint32)r_s16(state + 0x10);
            uint32 yx = y * old_x;
            w_u16(state + 0x2a, angle);
            w_u16(state + 0x20, 0u - (uint32)((sint32)(yz - zy) >> 12));
            w_u16(state + 0x22, (uint32)((sint32)(xz - zx) >> 12));
            w_u16(state + 0x24, 0u - (uint32)((sint32)(xy - yx) >> 12));
        }
        if (r_s16(state + 0x28) == 0)
        {
            uint32 y = (uint32)r_s16(normal + 2);
            uint32 z_axis = (uint32)r_s16(normal + 4);
            uint32 angle = G3Function8002F0C4(y, z_axis, frame);
            w_u16(object + 0x60, 0u - angle);
            uint32 x = (uint32)r_s16(normal);
            uint32 root = G3Function8002F368((uint32)((sint32)(0x1000000u - x * x) >> 12), frame);
            x = (uint32)r_s16(normal);
            angle = G3Function8002F0C4(x, root, frame);
            w_u16(object + 0x62, angle);
            return;
        }
    }
    uint32 progress = r_u16(state + 0x28);
    uint32 step = r_u16(settings + 0x22);
    progress += step;
    w_u16(state + 0x28, progress);
    sint32 limit = r_s16(state + 0x2a);
    uint32 limit_bits = r_u16(state + 0x2a);
    if ((sint16)progress >= limit) w_u16(state + 0x28, limit_bits);
    w_u16(frame + 0x14, r_u16(state + 0x10));
    w_u16(frame + 0x1a, r_u16(state + 0x12));
    w_u16(frame + 0x20, r_u16(state + 0x14));
    uint32 angle = (uint32)r_s16(state + 0x28);
    G3Function8002F630(state + 0x20, frame + 0x10, angle, frame);
    uint32 y = (uint32)r_s16(frame + 0x1a);
    uint32 z_axis = (uint32)r_s16(frame + 0x20);
    angle = G3Function8002F0C4(y, z_axis, frame);
    w_u16(object + 0x60, 0u - angle);
    uint32 x = (uint32)r_s16(frame + 0x14);
    uint32 root = G3Function8002F368((uint32)((sint32)(0x1000000u - x * x) >> 12), frame);
    x = (uint32)r_s16(frame + 0x14);
    angle = G3Function8002F0C4(x, root, frame);
    w_u16(object + 0x62, angle);
    sint32 current = r_s16(state + 0x28);
    limit = r_s16(state + 0x2a);
    if (current == limit) w_u16(state + 0x28, 0);
}

void G3CamelMove(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000b84, "G3L_25");
    uint32 frame = guest_stack - 0x58;
    uint32 input = G3CamelSteer(game, object, state, settings, frame);
    G3CamelSpeed((uint32)(sint32)(sint16)input, state, settings);
    if (r_u8(state + 0x2d) != 0)
    {
        G3FillMemory80086094(frame + 0x10, 0, 8);
        uint32 table = r_u32(settings + 0x1c);
        sint32 speed = r_s16(state + 0xa);
        sint32 threshold = r_s16(table);
        uint32 index = 0;
        while (speed >= threshold)
        {
            ++index;
            uint32 offset = (uint32)((sint32)(index << 16) >> 15);
            threshold = r_s16(table + offset);
        }
        uint32 offset = (uint32)((sint32)(index << 16) >> 15);
        w_u16(state + 0xc, index);
        table = r_u32(settings + 0x14);
        uint32 stride = r_u16(table + offset);
        w_u16(frame + 0x12, 0u - stride);
        G3Function8002F718(frame + 0x20);
        G3Function8005BB6C((uint32)r_s16(state + 4), frame + 0x20, frame);
        G3Function8005BA5C((uint32)r_s16(object + 0x62), frame + 0x20, frame);
        G3Function8005B94C((uint32)r_s16(object + 0x60), frame + 0x20, frame);
        ApplyMatrixSV((MATRIX *)psx_addr(frame + 0x20, 20), (SVECTOR *)psx_addr(frame + 0x10, 8), (SVECTOR *)psx_addr(frame + 0x18, 6));
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 position = r_u16(object + 0x58 + axis * 2);
            uint32 delta = r_u16(frame + 0x18 + axis * 2);
            w_u16(object + 0x58 + axis * 2, position + delta);
        }
    }
    G3CamelAlign(object, state, settings, module_base, frame);
    G3CamelAnimate(object, game, state, module_base, frame);
}

void G3CamelFooting(uint32 state)
{
    FUNCTION_MARKER(0x00000d10, "G3L_25");
    w_u8(state + 0x2d, 1);
    w_u8(state + 0x2e, 1);
}

void G3CamelFollowPlatform(uint32 object, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000d20, "G3L_25");
    uint32 frame = guest_stack - 0x20;
    if (r_u8(state + 0x2e) != 0) return;
    uint32 player = r_u32(0x8009efc4);
    sint32 z = r_s16(object + 0x5c);
    sint32 floor = r_s16(player + 0xe0);
    sint32 difference = z - floor;
    if (difference < 0) difference = -difference;
    if (difference >= 5 || r_u8(state + 0x2c) != 0) return;
    player = r_u32(0x8009efc4);
    uint32 scale = r_u8(settings + 0x28);
    G3Function80036278(player + 0xb4, frame + 0x10, scale, frame);
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        uint32 position = r_u16(object + 0x58 + axis * 2);
        uint32 delta = r_u16(frame + 0x10 + axis * 2);
        w_u16(object + 0x58 + axis * 2, position + delta);
    }
}

void G3CamelReact(uint32 object, uint32 game, uint32 state)
{
    FUNCTION_MARKER(0x00000dec, "G3L_25");
    if (r_u8(state + 0x2f) == 0) return;
    uint32 player = r_u32(game + 0x50);
    w_u16(player + 0x6e, 0x29);
    w_u16(object + 0x6e, 3);
    w_u16(object + 0x5e, 0);
    w_u16(state + 2, 5);
    w_u16(state + 0xa, 0);
    w_u8(state + 0x2f, 0);
}

void G3CamelRequestJump(uint32 object, uint32 game, uint32 state, uint32 settings)
{
    FUNCTION_MARKER(0x00000e2c, "G3L_25");
    uint32 player = r_u32(0x8009efc4);
    uint32 controls = (r_u32(player + 0x10) & 0x100) == 0 ? game + 0x70 : 0x800a0d28;
    if ((r_u32(controls + 4) & 0x80) == 0) return;
    if (r_u8(state + 0x2e) == 0 || r_s16(state + 2) != 0) return;
    uint32 index = r_u16(state + 0xc);
    uint32 table = r_u32(settings + 0x1c);
    uint32 offset = (uint32)((sint32)(index << 16) >> 17) * 2;
    sint32 speed = r_s16(state + 0xa);
    sint32 threshold = r_s16(table + offset);
    if (speed < threshold)
    {
        index = r_u8(settings + 0x27);
        w_u16(state + 0xc, index);
        table = r_u32(settings + 0x1c);
        w_u16(state + 0xa, r_u16(table + index * 2) - 1);
    }
    w_u16(state + 2, 1);
    w_u16(object + 0x5e, 0);
}

void G3CamelGround(uint32 object, uint32 state, uint32 settings)
{
    FUNCTION_MARKER(0x00000ef8, "G3L_25");
    sint32 mode = r_s16(state + 2);
    switch (mode)
    {
    case 0:
    {
        uint32 player = r_u32(0x8009efc4);
        sint32 player_z = r_s16(player + 0x5c);
        sint32 floor = r_s16(player + 0xe0);
        bool in_range = player_z - 64 < floor && floor < player_z + 256;
        if (!in_range && r_u8(state + 0x2c) == 0)
        {
            uint32 z = r_u16(object + 0x5c);
            uint32 velocity = r_u16(state + 0xe);
            w_u16(object + 0x5c, z + velocity);
            velocity = r_u16(state + 0xe);
            uint32 gravity = r_u16(settings + 0xc);
            w_u16(state + 0xe, velocity + gravity);
            return;
        }
        player = r_u32(0x8009efc4);
        sint32 z = r_s16(object + 0x5c);
        floor = r_s16(player + 0xe0);
        sint32 difference = z - floor;
        if (difference < 0) difference = -difference;
        if (difference >= 5 && r_u8(state + 0x2c) == 0)
        {
            player = r_u32(0x8009efc4);
            w_u16(object + 0x5c, r_u16(player + 0xe0));
        }
        w_u16(state + 0xe, 0);
        return;
    }
    case 1: w_u16(state + 2, 2); return;
    case 2:
        if ((r_u32(object + 0x14) & 0x10) != 0)
        {
            sint32 speed = r_s16(state + 0xa);
            w_u16(state + 2, 0);
            if (speed != 0)
            {
                w_u16(object + 0x6e, 1);
                uint32 player = r_u32(0x8009efc4);
                w_u16(player + 0x6e, 0x31);
            }
        }
        return;
    case 4:
    case 5:
        if ((r_u32(object + 0x14) & 0x10) != 0)
        {
            uint32 player = r_u32(0x8009efc4);
            w_u16(object + 0x6e, 0);
            w_u16(object + 0x5e, 0);
            w_u16(player + 0x6e, 0x2a);
            w_u16(state + 2, 0);
        }
        return;
    default: return;
    }
}

void G3CamelReleaseAttachment(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000107c, "G3L_25");
    uint32 player = r_u32(0x8009efc4);
    uint32 frame = guest_stack - 0x18;
    uint32 controls = (r_u32(player + 0x10) & 0x100) == 0 ? game + 0x70 : 0x800a0d28;
    uint32 buttons = r_u32(controls + 4);
    player = r_u32(game + 0x50);
    uint32 player_state = r_u32(player + 0x24);
    if ((buttons & 0x40) == 0) return;
    uint32 attachment = r_u32(player_state + 0x17c);
    if (attachment != 0)
    {
        G3Function800772F0(attachment, game, frame);
        w_u32(player_state + 0x17c, 0);
    }
}

void G3CamelRide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000010f4, "G3L_25");
    uint32 frame = guest_stack - 0x60;
    uint32 player = r_u32(game + 0x50);
    uint32 controls = (r_u32(player + 0x10) & 0x100) == 0 ? game + 0x70 : 0x800a0d28;
    uint32 player_state = r_u32(player + 0x24);
    G3CamelFooting(state);
    G3CamelMove(object, game, state, settings, module_base, frame);
    G3CamelGround(object, state, settings);
    G3CamelRequestJump(object, game, state, settings);
    G3CamelReleaseAttachment(game, frame);
    G3CamelReact(object, game, state);
    G3CamelFollowPlatform(object, state, settings, frame);
    if (r_u8(settings + 0x26) != 0)
    {
        sint32 z = r_s16(object + 0x5c);
        sint32 floor = r_s16(player + 0xe0);
        sint32 difference = z - floor;
        if (difference < 0) difference = -difference;
        if (difference < 5 && r_u8(state + 0x2c) == 0 && (r_u32(controls + 4) & 0x40) != 0)
            w_u16(state, 3);
    }
    if (r_u8(state + 0x34) != 0)
    {
        w_u16(state, 3);
        w_u16(frame + 0x10, 0);
        w_u16(frame + 0x12, 400);
        w_u16(frame + 0x14, 0);
        G3Function8002F718(frame + 0x20);
        G3Function8005BB6C((uint32)r_s16(object + 0x64), frame + 0x20, frame);
        ApplyMatrixSV((MATRIX *)psx_addr(frame + 0x20, 20), (SVECTOR *)psx_addr(frame + 0x10, 8), (SVECTOR *)psx_addr(frame + 0x18, 6));
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 position = r_u16(object + 0x58 + axis * 2);
            uint32 delta = r_u16(frame + 0x18 + axis * 2);
            w_u16(object + 0x58 + axis * 2, position + delta);
        }
        w_u8(state + 0x34, 0);
    }
    for (uint32 axis = 0; axis != 3; ++axis)
        w_u16(player + 0x58 + axis * 2, r_u16(object + 0x58 + axis * 2));
    if (r_s16(state) == 3)
    {
        uint32 heading = (uint32)r_s16(player + 0x64) - 0x400;
        uint32 sine = G3Function8005B470(heading);
        heading = (uint32)r_s16(player + 0x64) - 0x400;
        w_u32(player + 0x104, (uint32)((sint32)sine >> 5));
        uint32 cosine = G3Function8005B590(heading);
        w_u32(player + 0x108, (uint32)((sint32)cosine >> 5));
        w_u32(player + 0x10c, 150);
        w_u32(player_state + 0x1fc, 0);
        w_u16(object + 0x6e, 0);
        w_u16(object + 0x5e, 0);
        w_u16(player_state + 0xce, r_u16(player_state + 0xce) & 0xfffe);
        w_u16(state + 0xa, 0);
    }
}

void G3CamelUpdate(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001384, "G3L_25");
    uint32 frame = guest_stack - 0x28;
    w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
    sint32 mode = r_s16(state);
    switch (mode)
    {
    case 0:
    case 4:
        if (r_u8(settings + 8) != 0) G3Function8002A1C8(object, 0xffffffff, frame);
        break;
    case 1:
    {
        uint32 descriptor = r_u32(object + 0x1c);
        if (r_u32(descriptor + 0x10) == 0) break;
        G3Function8002A1C8(object, 0xffffffff, frame);
        if ((r_u32(object + 0x14) & 0x10) == 0) break;
        w_u16(object + 0x6e, 0x31);
        w_u16(object + 0x5e, 0);
        uint32 player = r_u32(game + 0x50);
        w_u16(player + 0x5e, 0);
        player = r_u32(0x8009efc4);
        w_u16(state, 2);
        w_u16(state + 2, 0);
        for (uint32 axis = 0; axis != 3; ++axis)
            w_u16(state + 0x18 + axis * 2, r_u16(player + 0xb4 + axis * 2));
        break;
    }
    case 2:
    {
        G3CamelRide(object, game, state, settings, module_base, frame);
        uint32 player = r_u32(0x8009efc4);
        w_u32(object + 0xf8, r_u32(player + 0xf8));
    {
        uint32 source = player + 0xfc;
        uint32 destination = object + 0xfc;
        uint32 high_address0 = source + 3;
        uint32 high_lane0 = high_address0 & 3;
        uint32 lane0 = (source + 0) & 3;
        uint32 high0 = r_masked_u32(high_address0 & 0xfffffffc, (1u << (high_lane0 + 1)) - 1);
        uint32 low0 = r_masked_u32((source + 0) & 0xfffffffc, (15u << lane0) & 15);
        uint32 keep0 = lane0 == 0 ? 0 : 0xffffffff << ((4 - lane0) * 8);
        uint32 value0 = ((high0 << ((3 - high_lane0) * 8)) & keep0) | (low0 >> (lane0 * 8));
        uint32 high_address1 = source + 7;
        uint32 high_lane1 = high_address1 & 3;
        uint32 lane1 = (source + 4) & 3;
        uint32 high1 = r_masked_u32(high_address1 & 0xfffffffc, (1u << (high_lane1 + 1)) - 1);
        uint32 low1 = r_masked_u32((source + 4) & 0xfffffffc, (15u << lane1) & 15);
        uint32 keep1 = lane1 == 0 ? 0 : 0xffffffff << ((4 - lane1) * 8);
        uint32 value1 = ((high1 << ((3 - high_lane1) * 8)) & keep1) | (low1 >> (lane1 * 8));
        high_address0 = destination + 3;
        high_lane0 = high_address0 & 3;
        lane0 = (destination + 0) & 3;
        w_masked_u32(high_address0 & 0xfffffffc, value0 >> ((3 - high_lane0) * 8), (1u << (high_lane0 + 1)) - 1);
        w_masked_u32((destination + 0) & 0xfffffffc, value0 << (lane0 * 8), (15u << lane0) & 15);
        high_address1 = destination + 7;
        high_lane1 = high_address1 & 3;
        lane1 = (destination + 4) & 3;
        w_masked_u32(high_address1 & 0xfffffffc, value1 >> ((3 - high_lane1) * 8), (1u << (high_lane1 + 1)) - 1);
        w_masked_u32((destination + 4) & 0xfffffffc, value1 << (lane1 * 8), (15u << lane1) & 15);
    }
        break;
    }
    case 3:
    {
        uint32 descriptor = r_u32(object + 0x1c);
        if (r_u32(descriptor + 0x10) == 0) w_u16(state, 0);
        else
        {
            G3Function8002A1C8(object, 0xffffffff, frame);
            if ((r_u32(object + 0x14) & 0x10) != 0)
            {
                uint32 player = r_u32(0x8009efc4);
                w_u16(object + 0x6e, 0);
                w_u16(object + 0x5e, 0);
                w_u16(state, 0);
                uint32 flags = r_u32(player + 0x10);
                w_u16(player + 0x60, 0);
                w_u16(player + 0x62, 0);
                w_u32(player + 0x10, flags & 0xfffff7ff);
            }
        }
        break;
    }
    default: break;
    }
    if (r_u8(settings + 9) != 0) G3CamelBank(object, state, settings, frame);
}

void G3CamelCollide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000157c, "G3L_25");
    uint32 frame = guest_stack - 0x58;
    uint32 contact = r_u32(object + 0xb0);
    uint32 player = r_u32(0x8009efc4);
    sint32 mode = r_s16(state);
    uint32 other = r_u32(contact + 0x14);
    uint32 player_state = r_u32(player + 0x24);
    uint32 delta = contact + 0x28;
    bool skip_player_check = mode == 0 && other != player;
    if (mode == 0 && other == player)
    {
        sint32 kind = r_s16(contact + 6);
        if (kind == 1)
        {
            uint32 descriptor = r_u32(contact + 0xc);
            if (r_u8(descriptor + 4) == 0 && r_u32(player_state + 0x1fc) == 0)
            {
                if (r_u8(settings + 0xa) != 0) w_u16(state, kind);
                else w_u16(state, 2);
                w_u16(state + 4, r_u16(object + 0x64));
                G3Function8006F50C(object, game, state, settings, frame);
                w_u16(object + 0x6e, 0);
                w_u16(object + 0x5e, 0);
                w_u8(player_state + 0x200, r_u8(settings + 0xb));
                player = r_u32(0x8009efc4);
                w_u16(player + 0x6e, 0x2a);
                w_u16(player + 0x5e, 0);
                return;
            }
        }
    }
    if (!skip_player_check && other == r_u32(0x8009efc4)) return;
    if (other != 0)
    {
        if (other == r_u32(player_state + 0x17c)) return;
        uint32 target = r_u32(other + 0xec);
        if (target != 0) G3DispatchStateEvent(target, other, game, 0x7e7, 0, frame);
        G3Function8003C548(contact, frame + 0x28, frame + 0x29);
        uint32 first = r_u8(frame + 0x28);
        if (first - 6u < 4)
        {
            uint32 second = r_u8(frame + 0x29);
            if ((second == 9 || second - 3u < 3) && r_u8(state + 0x30) != 0)
            {
                w_u32(frame + 0x10, 1);
                w_u16(frame + 0x20, 0);
                w_u16(frame + 0x22, 0);
                w_u32(frame + 0x24, contact);
                target = r_u32(other + 0xec);
                if (target != 0) G3DispatchStateEvent(target, other, game, 8, frame + 0x10, frame);
            }
        }
        uint32 second = r_u8(frame + 0x29);
        if (second - 6u < 4)
        {
            first = r_u8(frame + 0x28);
            if (first == 9 || first - 3u < 3)
            {
                if (r_u8(state + 0x2f) == 0 && r_s16(state + 2) != 5 && r_s16(player_state + 0x120) == 0)
                {
                    player = r_u32(0x8009efc4);
                    uint32 current_player_state = r_u32(player + 0x24);
                    w_u8(state + 0x2f, 1);
                    w_u16(current_player_state + 0x120, 60);
                    G3Function80073D44(game, frame);
                }
            }
        }
    }
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        uint32 coordinate = r_u16(object + 0x58 + axis * 2);
        uint32 change = r_u16(delta + axis * 2);
        w_u16(object + 0x58 + axis * 2, coordinate + change);
    }
    G3Function8005F3A8(object, delta);
    player = r_u32(0x8009efc4);
    uint32 change = r_u16(delta);
    uint32 coordinate = r_u16(player + 0x58);
    w_u16(player + 0x58, coordinate + change);
    for (uint32 axis = 1; axis != 3; ++axis)
    {
        coordinate = r_u16(player + 0x58 + axis * 2);
        change = r_u16(delta + axis * 2);
        w_u16(player + 0x58 + axis * 2, coordinate + change);
    }
    G3Function8005F3A8(player, delta);
}

void G3CamelDamage(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001880, "G3L_25");
    uint32 frame = guest_stack - 0x18;
    uint32 payload = r_u32(frame + 0x28);
    uint32 contact = r_u32(payload + 0x14);
    uint32 player = r_u32(0x8009efc4);
    uint32 other = r_u32(contact + 0x14);
    uint32 player_state = r_u32(player + 0x24);
    if (other == object || r_s16(state + 2) == 5 || r_s16(player_state + 0x120) != 0) return;
    w_u8(state + 0x2f, 1);
    w_u16(player_state + 0x120, 60);
    G3Function80073D44(game, frame);
}

uint32 G3CanbalbEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_79");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3CanbalbInitialize(object);
        return 1;
    case 2:
        G3CanbalbUpdate(object, game, module_base, frame);
        return 1;
    case 3:
        G3CanbalbCollide(object, game, module_base, frame);
        return 1;
    default:
        return 0;
    }
}

void G3CanbalbInitialize(uint32 object)
{
    FUNCTION_MARKER(0x00000088, "G3L_79");
    w_u32(object + 0x134, 0);
    w_u32(object + 0x138, 0);
    w_u32(object + 0x13c, 5);
    uint32 xy = r_u32(object + 0x58);
    uint32 z = (uint32)r_s16(object + 0x5c);
    w_u32(object + 0x70, xy);
    w_u16(object + 0x74, z);
}

void G3CanbalbTrail(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000b8, "G3L_79");
    uint32 frame = guest_stack - 0x18;
    uint32 previous = r_u32(object + 0x134);
    w_u32(object + 0x134, previous + 1);
    uint32 interval = r_u32(object + 0x130);
    uint32 resource = r_u32(object + 0x12c);
    if (previous != interval)
        return;
    if (resource != 0)
    {
        uint32 models = r_u32(resource + 0xc);
        uint32 pool = r_u32(0x800a27fc);
        uint32 model = r_u32(models);
        G3Function800444E4(object + 0x70, model, pool, 0xffffffff, frame);
    }
    w_u32(object + 0x134, 0);
}

void G3CanbalbUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000120, "G3L_79");
    uint32 frame = guest_stack - 0x20;
    uint32 age = r_u16(object + 0x9e);
    uint32 dx = r_u16(object + 0x104);
    uint32 y = r_u16(object + 0x5a);
    uint32 dy = r_u16(object + 0x108);
    w_u16(object + 0x9e, age + 1);
    uint32 x = r_u16(object + 0x58);
    w_u16(object + 0x5a, y + dy);
    uint32 acceleration = r_u32(object + 0x118);
    w_u16(object + 0x58, x + dx);
    uint32 velocity = r_u32(object + 0x10c);
    uint32 limit = r_u32(object + 0x124);
    velocity += acceleration;
    w_u32(object + 0x10c, velocity);
    if ((sint32)velocity < (sint32)limit)
        w_u32(object + 0x10c, limit);
    uint32 z = r_u16(object + 0x5c);
    velocity = r_u16(object + 0x10c);
    uint32 grace = r_u32(object + 0x13c);
    w_u16(object + 0x5c, z + velocity);
    if (grace != 0)
        w_u32(object + 0x13c, grace - 1);
    uint32 lifetime = r_u32(object + 0x128);
    if (lifetime != 0)
    {
        w_u32(object + 0x128, lifetime - 1);
        G3CanbalbTrail(object, frame);
        G3CanbalbScan(object, game, module_base, frame);
    }
    else
        G3Function80038798(object, 0, 0, frame);
}

void G3CanbalbExplode(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000001fc, "G3L_79");
    uint32 frame = guest_stack - 0x18;
    uint32 resource = G3Function8003AA48(module_base + 0x408);
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
    w_u32(object + 0x138, 1);
    w_u32(object + 0x128, 0);
}

uint32 G3CanbalbCollide(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000027c, "G3L_79");
    uint32 frame = guest_stack - 0x28;
    uint32 contact = r_u32(object + 0xb0);
    uint32 exploded = r_u32(object + 0x138);
    uint32 other = r_u32(contact + 0x14);
    if (exploded != 0)
        return exploded;
    if (other != 0)
    {
        uint32 owner = r_u32(object + 0x2c);
        if (owner == other)
        {
            uint32 grace = r_u32(object + 0x13c);
            if (grace != 0) return grace;
        }
        G3Function8003C548(contact, frame + 0x10, frame + 0x11);
        uint32 type = r_u8(frame + 0x11);
        if (type == 9 || type - 3 < 3)
        {
            other = r_u32(contact + 0x14);
            uint32 target = r_u32(other + 0xec);
            if (target != 0)
                G3DispatchStateEvent(target, other, game, 2020, 0, frame);
        }
    }
    G3CanbalbExplode(object, module_base, frame);
    return 1;
}

void G3CanbalbScan(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000340, "G3L_79");
    uint32 frame = guest_stack - 0x58;
    w_u32(frame + 0x18, frame + 0x10);
    w_u32(frame + 0x20, module_base + 0x27c);
    w_u32(frame + 0x30, frame + 0x40);
    w_u8(frame + 0x45, 8);
    w_u16(frame + 0x16, 100);
    w_u32(frame + 0x24, 0);
    w_u32(frame + 0x28, 0);
    w_u32(frame + 0x2c, object);
    w_u32(frame + 0x40, 0);
    w_u8(frame + 0x44, 0);
    uint32 xy = r_u32(object + 0x58);
    uint32 z = (uint32)r_s16(object + 0x5c);
    w_u32(frame + 0x48, xy);
    w_u16(frame + 0x4c, z);
    w_u16(frame + 0x4e, 100);
    xy = r_u32(object + 0x58);
    z = (uint32)r_s16(object + 0x5c);
    w_u32(frame + 0x10, xy);
    w_u16(frame + 0x14, z);
    xy = r_u32(object + 0x70);
    z = (uint32)r_s16(object + 0x74);
    w_u32(frame + 0x38, xy);
    w_u16(frame + 0x3c, z);
    w_u32(frame + 0x1c, frame + 0x38);
    uint32 mask = G3Function80063594(object);
    G3Function8005FE88(frame + 0x18, game, mask | 0xa2a, frame);
}

uint32 G3CannonBallEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_69");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3CannonBallInitialize(object, game, frame);
        return 1;
    case 2:
        G3CannonBallUpdate(object, frame);
        return 1;
    case 3:
        G3CannonBallCollide(object, game, frame);
        return 1;
    case 4:
    case 7:
        G3CannonBallStop(object);
        return 1;
    case 2008:
        G3CannonBallSetSpin(object, payload);
        return 1;
    case 2032:
        G3CannonBallSetAxis(object, payload);
        return 1;
    case 2009:
        G3CannonBallSetVelocity(object, payload);
        return 1;
    case 2016:
        G3CannonBallSetAcceleration(object, payload);
        return 1;
    case 2022:
        G3CannonBallSetPolarVelocity(object, payload, frame);
        return 1;
    case 2025:
        G3CannonBallSetCylindricalVelocity(object, payload, frame);
        return 1;
    default:
        return 0;
    }
}

void G3CannonBallStop(uint32 object)
{
    FUNCTION_MARKER(0x00000168, "G3L_69");
    uint32 resource = r_u32(object + 0x1c);
    if ((r_u32(resource + 0x30) & 0x1000) != 0)
    {
        uint32 flags = r_u32(object + 0x14);
        if ((flags & 0x2000) != 0)
        {
            uint32 handle = r_u32(object + 0x44);
            w_u32(object + 0x14, flags & 0xffffdfff);
            G3Function8003F18C(handle);
            w_u32(object + 0x44, 0);
        }
    }
}

void G3CannonBallInitialize(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000001cc, "G3L_69");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    w_u16(state + 4, r_u16(object + 0x60));
    w_u16(state + 6, r_u16(object + 0x62));
    w_u16(state + 8, r_u16(object + 0x64));
    uint32 angle = (uint32)(r_s16(object + 0x64) - 0x400);
    uint32 sine = G3Function8005B470(angle);
    w_u16(frame + 0x10, sine >> 6);
    angle = (uint32)(r_s16(object + 0x64) - 0x400);
    uint32 cosine = G3Function8005B590(angle);
    w_u16(frame + 0x12, cosine >> 6);
    w_u16(frame + 0x14, 0);
    uint32 source = frame + 0x10;
    uint32 destination = object + 0x114;
    uint32 first_address = source;
    uint32 first_high_address = first_address + 3;
    uint32 first_high_lane = first_high_address & 3;
    uint32 first_lane = first_address & 3;
    uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
    uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
    uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
    uint32 first = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
    uint32 second_address = source + 4;
    uint32 second_high_address = second_address + 3;
    uint32 second_high_lane = second_high_address & 3;
    uint32 second_lane = second_address & 3;
    uint32 second_high = r_masked_u32(second_high_address & 0xfffffffc, (1 << (second_high_lane + 1)) - 1);
    uint32 second_low = r_masked_u32(second_address & 0xfffffffc, (15 << second_lane) & 15);
    uint32 second_keep = second_lane == 0 ? 0 : 0xffffffff << ((4 - second_lane) * 8);
    uint32 second = ((second_high << ((3 - second_high_lane) * 8)) & second_keep) | (second_low >> (second_lane * 8));
    uint32 first_output = destination;
    uint32 first_output_high = first_output + 3;
    uint32 first_output_lane = first_output & 3;
    uint32 first_output_high_lane = first_output_high & 3;
    w_masked_u32(first_output_high & 0xfffffffc, first >> ((3 - first_output_high_lane) * 8), (1 << (first_output_high_lane + 1)) - 1);
    w_masked_u32(first_output & 0xfffffffc, first << (first_output_lane * 8), (15 << first_output_lane) & 15);
    uint32 second_output = destination + 4;
    uint32 second_output_high = second_output + 3;
    uint32 second_output_lane = second_output & 3;
    uint32 second_output_high_lane = second_output_high & 3;
    w_masked_u32(second_output_high & 0xfffffffc, second >> ((3 - second_output_high_lane) * 8), (1 << (second_output_high_lane + 1)) - 1);
    w_masked_u32(second_output & 0xfffffffc, second << (second_output_lane * 8), (15 << second_output_lane) & 15);
    uint32 owner = r_u32(object + 0x2c);
    if (owner != 0)
    {
        uint32 target = r_u32(owner + 0xec);
        if (target != 0)
            G3DispatchStateEvent(target, owner, game, 2006, object, frame);
    }
    uint32 resource = r_u32(object + 0x1c);
    uint32 name = r_u32(resource + 0x1c);
    uint32 effect_resource = 0;
    if (name != 0 && r_u32(name) != 0)
        effect_resource = G3Function8003AA48(name);
    w_u32(state, effect_resource);
    w_u16(state + 0xc, 0);
    w_u16(state + 0xe, 2);
}

void G3CannonBallUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000002e4, "G3L_69");
    uint32 frame = guest_stack - 0x48;
    uint32 state = object + 0x104;
    w_u16(object + 0x58, r_u16(object + 0x58) + r_u16(state + 0x10));
    w_u16(object + 0x5a, r_u16(object + 0x5a) + r_u16(state + 0x12));
    w_u16(object + 0x5c, r_u16(object + 0x5c) + r_u16(state + 0x14));
    uint32 x = r_u16(state + 0x10);
    uint32 ax = r_u16(state + 0x18);
    uint32 ay = r_u16(state + 0x1a);
    uint32 az = r_u16(state + 0x1c);
    w_u16(state + 0x10, x + ax);
    uint32 y = r_u16(state + 0x12);
    uint32 z = r_u16(state + 0x14);
    w_u16(state + 0x12, y + ay);
    sint32 spin = r_s16(state + 0xc);
    uint32 raw_spin = r_u16(state + 0xc);
    w_u16(state + 0x14, z + az);
    if (spin != 0)
    {
        uint32 axis = (uint32)r_s16(state + 0xe);
        uint32 rotation = state + (axis << 1) + 4;
        w_u16(rotation, r_u16(rotation) + raw_spin);
        G3Function8002F718(frame + 0x10);
        G3Function8005B94C((uint32)r_s16(state + 4), frame + 0x10, frame);
        G3Function8005BA5C((uint32)r_s16(state + 6), frame + 0x10, frame);
        G3Function8005BB6C((uint32)r_s16(state + 8), frame + 0x10, frame);
        G3Function800202A4(frame + 0x10, frame + 0x30, 0x15, frame);
        w_u16(object + 0x60, r_u16(frame + 0x30));
        w_u16(object + 0x62, r_u16(frame + 0x32));
        w_u16(object + 0x64, r_u16(frame + 0x34));
    }
}

void G3CannonBallCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000404, "G3L_69");
    uint32 frame = guest_stack - 0x30;
    uint32 flags = 0;
    uint32 contact = r_u32(object + 0xb0);
    uint32 state = object + 0x104;
    G3Function8003C548(contact, frame + 0x10, frame + 0x11);
    sint32 kind = r_s16(contact + 6);
    if (kind == 1 || kind == 5)
    {
        uint32 primitive = r_u32(contact + 0xc);
        flags = r_u16(primitive);
        kind = r_s16(contact + 6);
    }
    if (kind != 3)
    {
        uint32 type = r_u8(frame + 0x11);
        if (type - 6 < 4 || (flags & 0x2000) == 0)
            return;
    }
    uint32 other = r_u32(contact + 0x14);
    uint32 player = r_u32(game + 0x50);
    if (other == player)
        G3Function80074044(object, game, frame);
    uint32 owner = r_u32(object + 0x2c);
    if (owner != 0 && r_u32(contact + 0x14) == owner)
        return;
    uint32 resource = r_u32(state);
    if (resource != 0)
    {
        uint32 models = r_u32(resource + 0xc);
        uint32 descriptor = r_u32(models);
        uint32 metadata = r_u32(descriptor + 0x1c);
        uint32 first = r_u32(metadata + 8);
        uint32 second = r_u32(metadata + 0xc);
        uint32 product = first * second;
        uint32 pool = r_u32(0x800a27fc);
        uint32 count = (uint32)(sint32)(sint16)product;
        G3Function800444E4(object + 0x58, descriptor, pool, count, frame);
    }
    G3Function80038798(object, 0, 0, frame);
}

void G3CannonBallSetSpin(uint32 object, uint32 value)
{
    FUNCTION_MARKER(0x00000554, "G3L_69");
    w_u16(object + 0x110, value);
}

void G3CannonBallSetAxis(uint32 object, uint32 value)
{
    FUNCTION_MARKER(0x0000055c, "G3L_69");
    if ((sint32)value < 0)
        value = 0;
    if ((sint32)value >= 3)
        value = 2;
    w_u16(object + 0x112, value);
}

void G3CannonBallSetVelocity(uint32 object, uint32 vector)
{
    FUNCTION_MARKER(0x00000580, "G3L_69");
    uint32 source = vector;
    uint32 destination = object + 0x114;
    uint32 first_address = source;
    uint32 first_high_address = first_address + 3;
    uint32 first_high_lane = first_high_address & 3;
    uint32 first_lane = first_address & 3;
    uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
    uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
    uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
    uint32 first = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
    uint32 second_address = source + 4;
    uint32 second_high_address = second_address + 3;
    uint32 second_high_lane = second_high_address & 3;
    uint32 second_lane = second_address & 3;
    uint32 second_high = r_masked_u32(second_high_address & 0xfffffffc, (1 << (second_high_lane + 1)) - 1);
    uint32 second_low = r_masked_u32(second_address & 0xfffffffc, (15 << second_lane) & 15);
    uint32 second_keep = second_lane == 0 ? 0 : 0xffffffff << ((4 - second_lane) * 8);
    uint32 second = ((second_high << ((3 - second_high_lane) * 8)) & second_keep) | (second_low >> (second_lane * 8));
    uint32 first_output = destination;
    uint32 first_output_high = first_output + 3;
    uint32 first_output_lane = first_output & 3;
    uint32 first_output_high_lane = first_output_high & 3;
    w_masked_u32(first_output_high & 0xfffffffc, first >> ((3 - first_output_high_lane) * 8), (1 << (first_output_high_lane + 1)) - 1);
    w_masked_u32(first_output & 0xfffffffc, first << (first_output_lane * 8), (15 << first_output_lane) & 15);
    uint32 second_output = destination + 4;
    uint32 second_output_high = second_output + 3;
    uint32 second_output_lane = second_output & 3;
    uint32 second_output_high_lane = second_output_high & 3;
    w_masked_u32(second_output_high & 0xfffffffc, second >> ((3 - second_output_high_lane) * 8), (1 << (second_output_high_lane + 1)) - 1);
    w_masked_u32(second_output & 0xfffffffc, second << (second_output_lane * 8), (15 << second_output_lane) & 15);
}

void G3CannonBallSetAcceleration(uint32 object, uint32 vector)
{
    FUNCTION_MARKER(0x000005a8, "G3L_69");
    uint32 source = vector;
    uint32 destination = object + 0x11c;
    uint32 first_address = source;
    uint32 first_high_address = first_address + 3;
    uint32 first_high_lane = first_high_address & 3;
    uint32 first_lane = first_address & 3;
    uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
    uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
    uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
    uint32 first = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
    uint32 second_address = source + 4;
    uint32 second_high_address = second_address + 3;
    uint32 second_high_lane = second_high_address & 3;
    uint32 second_lane = second_address & 3;
    uint32 second_high = r_masked_u32(second_high_address & 0xfffffffc, (1 << (second_high_lane + 1)) - 1);
    uint32 second_low = r_masked_u32(second_address & 0xfffffffc, (15 << second_lane) & 15);
    uint32 second_keep = second_lane == 0 ? 0 : 0xffffffff << ((4 - second_lane) * 8);
    uint32 second = ((second_high << ((3 - second_high_lane) * 8)) & second_keep) | (second_low >> (second_lane * 8));
    uint32 first_output = destination;
    uint32 first_output_high = first_output + 3;
    uint32 first_output_lane = first_output & 3;
    uint32 first_output_high_lane = first_output_high & 3;
    w_masked_u32(first_output_high & 0xfffffffc, first >> ((3 - first_output_high_lane) * 8), (1 << (first_output_high_lane + 1)) - 1);
    w_masked_u32(first_output & 0xfffffffc, first << (first_output_lane * 8), (15 << first_output_lane) & 15);
    uint32 second_output = destination + 4;
    uint32 second_output_high = second_output + 3;
    uint32 second_output_lane = second_output & 3;
    uint32 second_output_high_lane = second_output_high & 3;
    w_masked_u32(second_output_high & 0xfffffffc, second >> ((3 - second_output_high_lane) * 8), (1 << (second_output_high_lane + 1)) - 1);
    w_masked_u32(second_output & 0xfffffffc, second << (second_output_lane * 8), (15 << second_output_lane) & 15);
}

void G3CannonBallSetPolarVelocity(uint32 object, uint32 parameters, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000005d0, "G3L_69");
    uint32 frame = guest_stack - 0x28;
    uint32 yaw = r_u16(object + 0x64);
    uint32 state = object + 0x104;
    uint32 pitch = (uint32)r_s16(parameters);
    uint32 angle = yaw - 0x400;
    uint32 sine = G3Function8005B470(pitch);
    uint32 magnitude = (uint32)r_s16(parameters + 2);
    uint32 product = magnitude * sine;
    angle = (uint32)(sint32)(sint16)angle;
    uint32 radial = (uint32)((sint32)product >> 12);
    sine = G3Function8005B470(angle);
    product = radial * sine;
    w_u16(state + 0x10, (uint32)((sint32)product >> 12));
    uint32 cosine = G3Function8005B590(angle);
    product = radial * cosine;
    w_u16(state + 0x12, (uint32)((sint32)product >> 12));
    pitch = (uint32)r_s16(parameters);
    cosine = G3Function8005B590(pitch);
    magnitude = (uint32)r_s16(parameters + 2);
    product = magnitude * cosine;
    w_u16(state + 0x14, (uint32)((sint32)product >> 12));
}

void G3CannonBallSetCylindricalVelocity(uint32 object, uint32 parameters, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000690, "G3L_69");
    uint32 frame = guest_stack - 0x28;
    uint32 yaw = r_u16(object + 0x64);
    uint32 state = object + 0x104;
    uint32 magnitude = (uint32)r_s16(parameters);
    uint32 angle = (uint32)(sint32)(sint16)(yaw - 0x400);
    uint32 sine = G3Function8005B470(angle);
    uint32 product = magnitude * sine;
    w_u16(state + 0x10, (uint32)((sint32)product >> 12));
    uint32 cosine = G3Function8005B590(angle);
    product = magnitude * cosine;
    w_u16(state + 0x12, (uint32)((sint32)product >> 12));
    w_u16(state + 0x14, r_u16(parameters + 2));
}

uint32 G3ChickenEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_30");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3ChickenInitialize(object);
        return 1;
    case 2:
        G3ChickenUpdate(object, game, frame);
        return 1;
    case 8:
        G3ChickenHit(object, module_base, frame);
        return 1;
    default:
        return 0;
    }
}

void G3ChickenSetState(uint32 object, uint32 mode)
{
    FUNCTION_MARKER(0x00000088, "G3L_30");
    uint32 state = object + 0x104;
    switch (mode)
    {
    case 0:
        w_u32(object + 0x150, 0);
        w_u16(object + 0x6e, 0);
        w_u16(object + 0x5e, 0);
        w_u32(state + 0x34, 1);
        break;
    case 1:
    case 2:
        w_u32(object + 0x150, mode);
        w_u16(object + 0x6e, 1);
        w_u16(object + 0x5e, 0);
        break;
    }
    uint32 resource = r_u32(object + 0x1c);
    uint32 animation = r_u16(object + 0x6e);
    sint32 count = r_s16(resource + 0xa);
    if ((sint32)animation >= count)
        w_u16(object + 0x6e, 0);
}

void G3ChickenInitialize(uint32 object)
{
    FUNCTION_MARKER(0x00000110, "G3L_30");
    uint32 custom = r_u32(object + 0x28);
    uint32 state = object + 0x104;
    w_u32(state + 0x34, 0);
    uint32 intro = r_u32(object + 0x20);
    uint32 entries = r_u32(intro + 4);
    uint32 index = entries == 0 ? 0xffffffff : G3AttachmentIndex80037028(object);
    w_u16(state, index);
    w_u16(state + 2, (sint16)index != -1 ? 1 : 0);
    w_u32(state + 4, 0x1e);
    w_u32(state + 0x14, 0x78);
    w_u32(state + 8, 0x3c);
    w_u32(state + 0xc, 0x0009c400);
    w_u32(state + 0x10, 0);
    w_u16(state + 0x18, 0);
    w_u16(state + 0x1a, 0);
    w_u16(state + 0x1c, 0x3c);
    w_u16(state + 0x1e, 0xffe2);
    const uint32 source_offsets[4] = { 0, 6, 2, 8 };
    for (uint32 bound = 0; bound != 4; ++bound)
    {
        bool use_default = custom == 0;
        uint32 value = 0;
        if (!use_default)
        {
            sint32 configured = r_s16(custom + source_offsets[bound]);
            value = r_u16(custom + source_offsets[bound]);
            use_default = configured == 0;
        }
        if (use_default)
        {
            intro = r_u32(object + 0x20);
            value = r_u16(intro + 0x10 + (bound / 2) * 2);
            value += (bound & 1) == 0 ? 0xfffffd00 : 0x300;
        }
        w_u16(state + 0x20 + bound * 2, value);
    }
    uint32 count = G3RemainingPlacementCount80045FE8(object);
    w_u16(state + 0x28, count);
    w_u16(state + 0x2a, 1);
    if (custom != 0)
    {
        count = r_u16(state + 0x28);
        w_u16(custom + 0xc, count);
    }
    G3ChickenSetState(object, 0);
}

void G3ChickenClamp(uint32 object)
{
    FUNCTION_MARKER(0x000002b8, "G3L_30");
    uint32 state = object + 0x104;
    for (uint32 axis = 0; axis != 2; ++axis)
    {
        sint32 coordinate = r_s16(object + 0x58 + axis * 2);
        sint32 lower = r_s16(state + 0x20 + axis * 4);
        uint32 lower_half = r_u16(state + 0x20 + axis * 4);
        if (coordinate < lower)
            w_u16(object + 0x58 + axis * 2, lower_half);
        coordinate = r_s16(object + 0x58 + axis * 2);
        sint32 upper = r_s16(state + 0x22 + axis * 4);
        uint32 upper_half = r_u16(state + 0x22 + axis * 4);
        if (upper < coordinate)
            w_u16(object + 0x58 + axis * 2, upper_half);
    }
}

uint32 G3ChickenCanMove(uint32 object, uint32 x, uint32 y)
{
    FUNCTION_MARKER(0x00000344, "G3L_30");
    uint32 state = object + 0x104;
    uint32 current_x = r_u16(object + 0x58);
    uint32 current_y = r_u16(object + 0x5a);
    sint32 next_x = (sint16)(current_x + x);
    uint32 next_y = current_y + y;
    sint32 lower_x = r_s16(state + 0x20);
    if (next_x < lower_x)
        return 0;
    sint32 upper_x = r_s16(state + 0x22);
    if (upper_x < next_x)
        return 0;
    sint32 lower_y = r_s16(state + 0x24);
    if ((sint16)next_y < lower_y)
        return 0;
    sint32 upper_y = r_s16(state + 0x26);
    return upper_y < (sint16)next_y ? 0 : 1;
}
