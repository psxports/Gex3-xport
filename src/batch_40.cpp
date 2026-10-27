#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32 G3ChickenNearXY(uint32 object, uint32 point, uint32 radius)
{
    FUNCTION_MARKER(0x00000558, "G3L_30");
    uint32 target_x = (uint32)r_s16(point);
    sint32 x = r_s16(object + 0x58);
    if ((sint32)(target_x - radius) >= x || x >= (sint32)(target_x + radius))
        return 0;
    uint32 target_y = (uint32)r_s16(point + 2);
    sint32 y = r_s16(object + 0x5a);
    return (sint32)(target_y - radius) < y && y < (sint32)(target_y + radius) ? 1 : 0;
}

void G3ChickenWander(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000005a4, "G3L_30");
    uint32 frame = guest_stack - 0x40;
    uint32 state = object + 0x104;
    sint32 max_x = r_s16(state + 0x22);
    sint32 min_x = r_s16(state + 0x20);
    sint32 min_y = r_s16(state + 0x24);
    sint32 width = (sint16)((max_x - min_x) / 2);
    sint32 max_y = r_s16(state + 0x26);
    sint32 height = (sint16)((max_y - min_y) / 2);
    if (width == 0 || height == 0)
        return;
    if (r_u32(state + 0x34) != 0)
    {
        w_u32(state + 0x34, 0);
        sint32 random_x = (sint32)G3SdkRandom80027FA8();
        uint32 lower_x = r_u16(state + 0x20);
        w_u16(state + 0x2c, lower_x + (uint32)(width / 2) + (uint32)((sint64)random_x % width));
        sint32 random_y = (sint32)G3SdkRandom80027FA8();
        uint32 lower_y = r_u16(state + 0x24);
        w_u16(state + 0x2e, lower_y + (uint32)(height / 2) + (uint32)((sint64)random_y % height));
        uint32 z = r_u16(object + 0x5c);
        w_u16(state + 0x30, z);
    }
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        sint32 speed = r_s32(state + 8);
        w_u16(frame + 0x18 + axis * 2, (uint32)(speed / 4));
    }
    w_u32(frame + 0x10, 0x100);
    G3Function8003639C(object, 0x80, frame + 0x18, object + 0x130, 0x100, frame);
    uint32 arrived = G3ChickenNearXY(object, object + 0x130, 0x100);
    w_u32(state + 0x34, arrived);
}

uint32 G3ChickenIdle(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000730, "G3L_30");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    G3Function80029F0C(object, 0xffffffff, frame);
    if (r_s16(state + 2) != 0)
        return 0;
    if (r_u32(state + 0x10) != 0)
        return 1;
    G3ChickenWander(object, frame);
    return 0;
}

uint32 G3ChickenHitState(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000007f8, "G3L_30");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    G3Function80029F0C(object, 0xffffffff, frame);
    sint32 ascent = r_s16(state + 0x18);
    uint32 ascent_half = r_u16(state + 0x18);
    uint32 z;
    uint32 velocity;
    if (ascent > 0)
    {
        w_u16(state + 0x18, ascent_half - 1);
        z = r_u16(object + 0x5c);
        velocity = r_u16(state + 0x1c);
    }
    else
    {
        sint32 descent = r_s16(state + 0x1a);
        uint32 descent_half = r_u16(state + 0x1a);
        if (descent <= 0)
            return 1;
        w_u16(state + 0x1a, descent_half - 1);
        z = r_u16(object + 0x5c);
        velocity = r_u16(state + 0x1e);
    }
    uint32 yaw = r_u16(object + 0x64);
    w_u16(object + 0x5c, z + velocity);
    w_u16(object + 0x64, yaw + 0x180);
    return 2;
}

void G3ChickenFlee(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000003b0, "G3L_30");
    uint32 frame = guest_stack - 0x80;
    uint32 intro = r_u32(object + 0x20);
    {
        uint32 source = intro + 0x10;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 lane = source & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
        uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
        uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        uint32 last = (uint32)r_s16(source + 4);
        uint32 destination = frame + 0x18;
        high_address = destination + 3;
        high_lane = high_address & 3;
        lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
        w_u16(destination + 4, last);
    }
    uint32 state = object + 0x104;
    if (G3ChickenCanMove(object, 0, 0) != 0)
    {
        uint32 player = r_u32(0x8009efc4);
        uint32 x = r_u16(object + 0x58);
        uint32 player_x = r_u16(player + 0x58);
        w_u16(frame + 0x40, x - player_x);
        uint32 y = r_u16(object + 0x5a);
        uint32 player_y = r_u16(player + 0x5a);
        w_u16(frame + 0x44, 0);
        w_u16(frame + 0x42, y - player_y);
        G3Function8002F9A8(frame + 0x50, 0x1e0, frame + 0x40, frame);
        uint32 signed_x = (uint32)r_s16(frame + 0x50);
        uint32 signed_y = (uint32)r_s16(frame + 0x52);
        uint32 raw_x = r_u16(frame + 0x50);
        uint32 raw_y = r_u16(frame + 0x52);
        if (G3ChickenCanMove(object, signed_x, signed_y) == 0)
        {
            uint32 negative_y = 0 - raw_y;
            if (G3ChickenCanMove(object, (uint32)(sint32)(sint16)negative_y, signed_x) != 0)
            {
                w_u16(frame + 0x50, negative_y);
                w_u16(frame + 0x52, raw_x);
            }
            else
            {
                uint32 negative_x = 0 - raw_x;
                if (G3ChickenCanMove(object, signed_y, (uint32)(sint32)(sint16)negative_x) != 0)
                {
                    w_u16(frame + 0x50, raw_y);
                    w_u16(frame + 0x52, negative_x);
                }
                else
                {
                    w_u16(frame + 0x50, negative_x);
                    w_u16(frame + 0x52, negative_y);
                }
            }
        }
        x = r_u16(object + 0x58);
        uint32 step_x = r_u16(frame + 0x50);
        w_u16(frame + 0x18, x + step_x);
        y = r_u16(object + 0x5a);
        uint32 step_y = r_u16(frame + 0x52);
        w_u16(frame + 0x1a, y + step_y);
        uint32 z = r_u16(object + 0x5c);
        w_u16(frame + 0x1c, z);
    }
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        uint32 speed = r_u16(state + 8);
        w_u16(frame + 0x58 + axis * 2, speed);
    }
    w_u32(frame + 0x10, 0x100);
    G3Function8003639C(object, 0x80, frame + 0x58, frame + 0x18, 0x100, frame);
}

uint32 G3ChickenFleeState(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000790, "G3L_30");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    G3Function80029F0C(object, 0xffffffff, frame);
    if (r_u32(state + 0x10) == 0)
        return 0;
    G3ChickenFlee(object, frame);
    return 1;
}

void G3ChickenUpdate(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000088c, "G3L_30");
    uint32 frame = guest_stack - 0x28;
    uint32 player = r_u32(game + 0x50);
    uint32 object_x = (uint32)r_s16(object + 0x58);
    uint32 player_x = (uint32)r_s16(player + 0x58);
    uint32 delta_x = player_x - object_x;
    uint32 player_y = (uint32)r_s16(player + 0x5a);
    uint32 object_y = (uint32)r_s16(object + 0x5a);
    uint32 delta_y = player_y - object_y;
    uint32 object_z = (uint32)r_s16(object + 0x5c);
    uint32 player_z = (uint32)r_s16(player + 0x5c);
    uint32 delta_z = player_z - object_z;
    uint32 state = object + 0x104;
    uint32 mode = r_u32(object + 0x150);
    sint32 attached = r_s16(state + 2);
    uint32 distance = delta_x * delta_x + delta_y * delta_y + delta_z * delta_z;
    if (attached != 0)
    {
        sint32 index = r_s16(state);
        sint32 other_index = (index + 1) % 2;
        uint32 other = G3AttachedObject80036FE4(object, other_index);
        bool detach = other == 0;
        if (!detach)
        {
            uint32 flags = r_u32(other + 0x14);
            detach = (flags & 1) != 0 || (flags & 2) != 0;
        }
        if (detach)
            w_u16(state + 2, 0);
    }
    uint32 radius = r_u32(state + 0xc);
    w_u32(state + 0x10, (sint32)distance < (sint32)radius ? 1 : 0);
    uint32 next;
    if (mode == 1)
        next = G3ChickenFleeState(object, frame);
    else if (mode == 2)
        next = G3ChickenHitState(object, frame);
    else
        next = G3ChickenIdle(object, frame);
    uint32 current_mode = r_u32(object + 0x150);
    if (next != current_mode)
        G3ChickenSetState(object, next);
    G3ChickenClamp(object);
}

void G3ChickenSpawnFeathers(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000a0c, "G3L_30");
    uint32 frame = guest_stack - 0x48;
    uint32 resource = G3Function8003AA48(module_base + 0xd84);
    {
        uint32 source = object + 0x58;
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
        w_u16(destination + 4, last);
    }
    {
        uint32 source = module_base + 0xd90;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 lane = source & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
        uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
        uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        uint32 last = (uint32)r_s16(source + 4);
        uint32 destination = frame + 0x18;
        high_address = destination + 3;
        high_lane = high_address & 3;
        lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
        w_u16(destination + 4, last);
    }
    if (resource == 0)
        return;
    uint32 z = r_u16(frame + 0x14);
    w_u16(frame + 0x14, z + 0xb4);
    for (uint32 count = 0; count != 8; ++count)
    {
        sint32 random_x = (sint32)G3SdkRandom80027FA8();
        uint32 x = (uint32)(random_x % 48) - 24;
        sint32 random_y = (sint32)G3SdkRandom80027FA8();
        uint32 y = (uint32)(random_y % 48) - 24;
        sint32 random_z = (sint32)G3SdkRandom80027FA8();
        uint32 speed_z = (uint32)(random_z % 20) + 10;
        sint32 random_lifetime = (sint32)G3SdkRandom80027FA8();
        uint32 lifetime = (uint32)(sint32)(sint16)(random_lifetime % 8 + 8);
        uint32 data = r_u32(resource + 0xc);
        uint32 pool = r_u32(0x800a27fc);
        uint32 effect_data = r_u32(data);
        uint32 effect = G3Function800444E4(frame + 0x10, effect_data, pool, lifetime, frame);
        if (effect != 0)
        {
            G3Function800449F0(effect, (uint32)(sint32)(sint16)x, (uint32)(sint32)(sint16)y, (uint32)(sint32)(sint16)speed_z, effect);
            G3Function80044878(effect, frame + 0x18, 0);
            G3Function80045920(effect, frame);
        }
    }
}

void G3ChickenHit(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000be8, "G3L_30");
    uint32 frame = guest_stack - 0x48;
    uint32 state = object + 0x104;
    sint32 attached = r_s16(state + 2);
    uint32 custom = r_u32(object + 0x28);
    if (attached != 0)
        return;
    if (r_u32(object + 0x150) != 2)
        G3ChickenSetState(object, 2);
    sint32 ascent = r_s16(state + 0x18);
    uint32 ascent_half = r_u16(state + 0x18);
    if (ascent != 0)
        return;
    uint32 descent = r_u16(state + 0x1a);
    w_u16(state + 0x18, ascent_half + 5);
    w_u16(state + 0x1a, descent + 10);
    sint32 remaining = r_s16(state + 0x28);
    uint32 remaining_half = r_u16(state + 0x28);
    if (remaining > 0)
    {
        w_u16(state + 0x28, remaining_half - 1);
        if (custom != 0)
        {
            uint32 count = r_u16(custom + 0xc);
            w_u16(custom + 0xc, count - 1);
        }
        sint32 particles = r_s16(state + 0x2a);
        if (particles > 0)
        {
            uint32 count = 0;
            do
            {
                sint32 random_x = (sint32)G3SdkRandom80027FA8();
                uint32 x = (uint32)(random_x % 8) + 4;
                sint32 random_y = (sint32)G3SdkRandom80027FA8();
                uint32 y = (uint32)(random_y % 8) + 4;
                if ((G3SdkRandom80027FA8() & 1) == 0)
                    x = 0 - x;
                if ((G3SdkRandom80027FA8() & 1) == 0)
                    y = 0 - y;
                w_u32(frame + 0x10, 0);
                w_u32(frame + 0x14, 0);
                w_u32(frame + 0x18, 0xfffffffb);
                w_u32(frame + 0x1c, 100);
                w_u32(frame + 0x20, 100);
                w_u32(frame + 0x24, 100);
                G3Function800460E8(object, x, y, 0x40, frame);
                particles = r_s16(state + 0x2a);
                ++count;
            } while ((sint32)count < particles);
        }
    }
    G3ChickenSpawnFeathers(object, module_base, frame);
}

uint32 G3ChunkEvent(uint32 object, uint32 message, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_67");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3ChunkInitialize(object, module_base, frame);
        return 1;
    case 2:
        G3ChunkUpdate(object, frame);
        return 1;
    default:
        return 0;
    }
}

void G3ChunkInitialize(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000005c, "G3L_67");
    uint32 frame = guest_stack - 0x50;
    uint32 vector = frame + 0x28;
    uint32 state = object + 0x104;
    G3FillMemory80086094(vector, 0, 8);
    sint32 random = (sint32)G3SdkRandom80027FA8();
    w_u16(object + 0x60, (uint32)(random % 4095));
    random = (sint32)G3SdkRandom80027FA8();
    w_u16(object + 0x62, (uint32)(random % 4095));
    random = (sint32)G3SdkRandom80027FA8();
    w_u16(object + 0x64, (uint32)(random % 4095));
    uint32 lifetime = (G3SdkRandom80027FA8() & 15) + 5;
    w_u16(state + 0x20, lifetime);
    w_u16(state + 0x3a, 10);
    w_u16(state + 0x2a, 0x403);
    w_u16(state + 0x24, 100);
    w_u16(state + 0x38, 0);
    w_u16(state + 0x34, 0);
    w_u16(state + 0x26, 9);
    uint32 commands = r_u32(object + 0x24);
    if (commands != 0)
        G3ChunkConfigure(object, commands, module_base, frame);
    w_u16(frame + 0x2a, r_u16(state + 0x24));
    random = (sint32)G3SdkRandom80027FA8();
    uint32 spread_bits = r_u16(state + 0x2a);
    sint32 spread = (sint16)spread_bits;
    uint32 remainder;
    if (spread == 0)
        remainder = (uint32)random;
    else if (random == (-2147483647 - 1) && spread == -1)
        remainder = 0;
    else
        remainder = (uint32)(random % spread);
    w_u16(frame + 0x30, remainder - (uint32)(spread / 2));
    random = (sint32)G3SdkRandom80027FA8();
    w_u16(frame + 0x34, 0);
    w_u16(frame + 0x32, (uint32)(random % 4095));
    if ((r_u32(state + 0x2c) & 1) != 0)
    {
        G3Function8002F75C(vector, (uint32)r_s16(frame + 0x30));
        G3Function8002F820(vector, (uint32)r_s16(frame + 0x32));
        uint32 player = r_u32(0x8009efc4);
        G3Function800144CC(frame + 0x20, object + 0x58, player + 0x58, frame);
        G3Function8002F75C(vector, (uint32)r_s16(frame + 0x20));
        G3Function8002F8E4(vector, (uint32)r_s16(frame + 0x24));
    }
    else
    {
        uint32 angle = (uint32)(sint32)(sint16)(r_u16(frame + 0x30) + 0x400);
        G3Function8002F75C(vector, angle);
        G3Function8002F8E4(vector, (uint32)r_s16(frame + 0x32));
    }
    uint32 x = r_u16(frame + 0x28);
    uint32 y = r_u16(frame + 0x2a);
    uint32 z = r_u16(frame + 0x2c);
    w_u16(frame + 0x18, 0);
    w_u16(frame + 0x1a, 0);
    w_u16(frame + 0x10, x);
    w_u16(frame + 0x12, y);
    w_u16(frame + 0x14, z);
    sint32 acceleration = r_s16(state + 0x26);
    if (acceleration < 0)
        acceleration = -acceleration;
    w_u16(frame + 0x1c, 0 - (uint32)acceleration);
    for (uint32 group = 0; group != 2; ++group)
    {
        uint32 source = frame + 0x10 + group * 8;
        uint32 destination = state + 8 + group * 8;
        uint32 values[2];
        for (uint32 index = 0; index != 2; ++index)
        {
            uint32 input = source + index * 4;
            uint32 high_address = input + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = input & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(input & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            values[index] = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        }
        for (uint32 index = 0; index != 2; ++index)
        {
            uint32 output = destination + index * 4;
            uint32 high_address = output + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = output & 3;
            w_masked_u32(high_address & 0xfffffffc, values[index] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(output & 0xfffffffc, values[index] << (lane * 8), (15u << lane) & 15);
        }
    }
    if ((r_u32(state + 0x2c) & 4) != 0)
    {
        sint32 scale_x = r_s16(object + 0x68);
        if (scale_x < 0)
            scale_x += 7;
        sint32 scale_y = r_s16(object + 0x6a);
        w_u16(object + 0x68, (uint32)(scale_x >> 3));
        if (scale_y < 0)
            scale_y += 7;
        sint32 scale_z = r_s16(object + 0x6c);
        w_u16(object + 0x6a, (uint32)(scale_y >> 3));
        if (scale_z < 0)
            scale_z += 7;
        w_u16(object + 0x6c, (uint32)(scale_z >> 3));
        w_u16(state + 0x22, 0x80);
    }
}

void G3ChunkUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000034c, "G3L_67");
    uint32 frame = guest_stack - 0x18;
    uint32 state = object + 0x104;
    sint32 lifetime = r_s16(state + 0x20);
    uint32 next_lifetime = r_u16(state + 0x20) - 1;
    if (lifetime <= 0)
    {
        G3Function80038798(object, 0, 0, frame);
        return;
    }
    uint32 flags = r_u32(state + 0x2c);
    w_u16(state + 0x20, next_lifetime);
    if ((flags & 4) != 0)
    {
        sint32 scale = r_s16(object + 0x68);
        sint32 increment = r_s16(state + 0x22);
        uint32 increment_bits = r_u16(state + 0x22);
        bool overflow = scale + increment > 32767;
        uint32 scale_bits = r_u16(object + 0x68);
        if (!overflow)
        {
            w_u16(object + 0x68, scale_bits + increment_bits);
            scale_bits = r_u16(object + 0x6a);
            increment_bits = r_u16(state + 0x22);
            w_u16(object + 0x6a, scale_bits + increment_bits);
            scale_bits = r_u16(object + 0x6c);
            increment_bits = r_u16(state + 0x22);
            w_u16(object + 0x6c, scale_bits + increment_bits);
        }
    }
    uint32 x = r_u16(object + 0x58);
    uint32 dx = r_u16(state + 8);
    w_u16(object + 0x58, x + dx);
    uint32 y = r_u16(object + 0x5a);
    uint32 dy = r_u16(state + 0xa);
    w_u16(object + 0x5a, y + dy);
    uint32 z = r_u16(object + 0x5c);
    uint32 dz = r_u16(state + 0xc);
    w_u16(object + 0x5c, z + dz);
    dx = r_u16(state + 8);
    uint32 ax = r_u16(state + 0x10);
    uint32 ay = r_u16(state + 0x12);
    uint32 az = r_u16(state + 0x14);
    w_u16(state + 8, dx + ax);
    dy = r_u16(state + 0xa);
    dz = r_u16(state + 0xc);
    w_u16(state + 0xa, dy + ay);
    w_u16(state + 0xc, dz + az);
    x = r_u16(object + 0x60);
    dx = r_u16(state + 0x18);
    w_u16(object + 0x60, x + dx);
    y = r_u16(object + 0x62);
    dy = r_u16(state + 0x1a);
    w_u16(object + 0x62, y + dy);
    z = r_u16(object + 0x64);
    dz = r_u16(state + 0x1c);
    w_u16(object + 0x64, z + dz);
    if ((r_u32(state + 0x2c) & 2) != 0)
    {
        sint32 height = r_s16(object + 0x5c);
        sint32 floor = r_s16(state + 0x28);
        uint32 floor_bits = r_u16(state + 0x28);
        if (height < floor)
        {
            w_u16(object + 0x5c, floor_bits);
            w_u16(state + 0x20, 0);
        }
    }
    uint32 resource = r_u32(state + 0x30);
    if (resource == 0)
        return;
    if (r_s16(state + 0x36) == 0)
    {
        uint32 pool = r_u32(0x800a27fc);
        uint32 owner = r_u32(resource + 0xc);
        uint32 effect_lifetime = (uint32)r_s16(state + 0x3a);
        uint32 model = r_u32(owner);
        uint32 effect = G3Function800444E4(object + 0x58, model, pool, effect_lifetime, frame);
        if (effect != 0)
        {
            uint32 velocity = (uint32)r_s16(state + 0x38);
            if (velocity != 0)
                G3Function80044A40(effect, 0, 0, velocity, effect);
        }
    }
    sint32 interval = r_s16(state + 0x34);
    if (interval != 0)
    {
        sint32 counter = r_s16(state + 0x36);
        w_u16(state + 0x36, (uint32)((counter + 1) % interval));
    }
}

void G3ChunkNoOperation(void)
{
    FUNCTION_MARKER(0x0000055c, "G3L_67");
    // Original body contains only JR RA and its NOP delay slot
}

void G3ChunkConfigure(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000564, "G3L_67");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    w_u32(guest_stack + 4, commands);
    if (commands == 0)
        return;
    uint32 cursor = commands;
    uint32 actual = r_u32(cursor);
    uint32 expected = r_u32(module_base + 0xa68);
    for (;;)
    {
        if (actual == expected)
        {
            actual = r_u32(cursor + 4);
            expected = r_u32(module_base + 0xa6c);
            if (actual == expected)
                return;
        }
        static const uint32 names[14] = {0xa74, 0xa80, 0xa8c, 0xa98, 0xaa4, 0xab0, 0xabc, 0xac8, 0xad4, 0xae0, 0xaec, 0xaf8, 0xb04, 0xb10};
        uint32 words = 2;
        bool recognized = false;
        cursor = r_u32(frame + 0x2c);
        uint32 data = cursor + 8;
        for (uint32 kind = 0; kind != 14; ++kind)
        {
            if (kind != 0)
                cursor = r_u32(frame + 0x2c);
            expected = r_u32(module_base + names[kind]);
            actual = r_u32(cursor);
            if (actual != expected)
                continue;
            actual = r_u32(cursor + 4);
            expected = r_u32(module_base + names[kind] + 4);
            if (actual != expected)
                continue;
            recognized = true;
            switch (kind)
            {
            case 0:
            {
                sint32 scale = r_s16(data);
                if (scale < 13)
                    scale = 13;
                else if (scale >= 800)
                    scale = 800;
                uint32 value = (uint32)((scale * 4096) / 100);
                ++words;
                w_u16(object + 0x6c, value);
                w_u16(object + 0x6a, value);
                w_u16(object + 0x68, value);
                break;
            }
            case 1:
                ++words;
                w_u16(state + 0x24, r_u16(data));
                break;
            case 2:
                ++words;
                w_u16(state + 0x26, r_u16(data));
                break;
            case 3:
            {
                uint32 resource = G3Function8003AA48(data);
                w_u32(state + 0x30, resource);
                words += 2;
                break;
            }
            case 4:
                ++words;
                w_u16(state + 0x20, r_u16(data));
                break;
            case 5:
            {
                ++words;
                uint32 flags = r_u32(state + 0x2c);
                uint32 floor = r_u16(data);
                flags |= 2;
                w_u16(state + 0x28, floor);
                w_u32(state + 0x2c, flags);
                break;
            }
            case 6:
                w_u32(state + 0x2c, r_u32(state + 0x2c) | 4);
                break;
            case 7:
                w_u32(state + 0x2c, r_u32(state + 0x2c) | 1);
                break;
            case 8:
            {
                sint32 spread = r_s32(data);
                if (spread <= 0)
                    spread = 1;
                if (spread >= 361)
                    spread = 360;
                ++words;
                w_u16(state + 0x2a, (uint32)((spread * 4096) / 360));
                break;
            }
            case 9:
            {
                uint32 velocity = r_u32(data);
                ++words;
                if ((sint32)velocity < 0)
                    velocity = 0 - velocity;
                w_u16(state + 0x38, 0 - velocity);
                break;
            }
            case 10:
                ++words;
                w_u16(state + 0x34, r_u16(data));
                break;
            case 11:
                ++words;
                w_u16(state + 0x3a, r_u16(data));
                break;
            case 12:
                ++words;
                w_u16(state + 0x1a, r_u16(data));
                break;
            case 13:
                ++words;
                w_u16(state + 0x1c, r_u16(data));
                break;
            }
            break;
        }
        if (recognized)
        {
            cursor = r_u32(frame + 0x2c);
            cursor += (uint32)((sint32)(words << 16) >> 14);
            w_u32(frame + 0x2c, cursor);
        }
        else if (G3Function80085B84(frame + 0x2c, frame) == 0)
            return;
        cursor = r_u32(frame + 0x2c);
        expected = r_u32(module_base + 0xa68);
        actual = r_u32(cursor);
    }
}

uint32 G3ChunkyEvent(uint32 object, uint32 message, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_45");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3ChunkyInitialize(object, module_base, frame);
        return 1;
    case 2:
        G3ChunkyUpdate(object, frame);
        return 1;
    case 3:
        G3ChunkyCollide(object, frame);
        return 1;
    default:
        return 0;
    }
}

void G3ChunkyInitialize(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000088, "G3L_45");
    uint32 frame = guest_stack - 0x38;
    uint32 state = object + 0x128;
    uint32 parent_model = 0;
    uint32 parent_index = 0;
    uint32 lighting_index = 0;
    uint32 resource = r_u32(object + 0x1c);
    uint32 index = (uint32)r_s16(object + 0xa2);
    uint32 models = r_u32(resource + 0xc);
    uint32 own_model = r_u32(models + index * 4);
    uint32 parent = r_u32(object + 0x2c);
    uint32 flags = r_u32(object + 0x10);
    uint32 parent_resource = r_u32(parent + 0x1c);
    bool same = resource == parent_resource;
    w_u32(object + 0x10, flags | 0x800);
    if (same)
    {
        parent = r_u32(object + 0x2c);
        parent_index = r_u32(parent + 0x138);
        parent_resource = r_u32(parent + 0x130);
        parent_model = r_u32(parent + 0x134);
        lighting_index = r_u32(parent + 0x140);
    }
    else
    {
        parent = r_u32(object + 0x2c);
        parent_resource = r_u32(parent + 0x1c);
        if (parent_resource != 0)
        {
            parent_index = (uint32)r_s16(parent + 0xa2);
            lighting_index = (uint32)r_s16(parent + 0x8e);
            models = r_u32(parent_resource + 0xc);
            parent_model = r_u32(models + parent_index * 4);
        }
    }
    if (parent_model == 0 || r_s16(parent_model + 4) == 0)
        return;
    flags = r_u32(object + 0x18);
    w_u32(object + 0xf0, module_base + 0x86c);
    w_u32(object + 0x18, flags | 0x400);
    uint32 random = G3SdkRandom80027FA8();
    resource = r_u32(object + 0x1c);
    sint32 count = r_s16(resource + 8);
    uint32 remainder = count == 0 ? random : (uint32)((sint64)(sint32)random % count);
    w_u16(object + 0xa2, remainder);
    w_u32(state, 0x80);
    w_u32(state + 4, same ? 0 : 1);
    w_u32(state + 0x10, parent_index);
    w_u32(state + 8, parent_resource);
    w_u32(state + 0xc, parent_model);
    random = G3SdkRandom80027FA8();
    count = r_s16(parent_model + 4);
    remainder = count == 0 ? random : (uint32)((sint64)(sint32)random % count);
    w_u32(state + 0x18, lighting_index);
    w_u32(state + 0x14, remainder);
    sint32 scale;
    if (same)
    {
        parent = r_u32(object + 0x2c);
        uint32 half = r_u16(parent + 0x68);
        scale = (sint16)half / 2;
    }
    else
    {
        parent = r_u32(object + 0x2c);
        uint32 model_radius = (uint32)r_s16(parent_model + 0x26);
        uint32 parent_scale = (uint32)r_s16(parent + 0x68);
        sint32 product = (sint32)(parent_scale * model_radius);
        sint32 quarter = (product / 4096) / 4;
        uint32 numerator = (uint32)quarter << 12;
        parent_resource = r_u32(parent + 0x1c);
        sint32 divisor = r_s16(own_model + 0x26);
        uint32 resource_flags = r_u32(parent_resource + 0x3c);
        uint32 quotient = divisor == 0 ? ((sint32)numerator < 0 ? 1u : 0xffffffffu) : (uint32)((sint64)(sint32)numerator / divisor);
        scale = (sint32)quotient;
        if ((resource_flags & 0x2000) != 0)
        {
            if ((resource_flags & 0x1000) != 0)
                scale = (sint32)((sint64)scale / 2);
            else
                scale = (sint32)(quotient * 3) / 4;
        }
        else if ((resource_flags & 0x1000) != 0)
            scale /= 4;
        if (scale < 0x100)
            scale = 0x100;
    }
    if (scale > 20000)
        scale = 20000;
    w_u16(object + 0x68, (uint32)scale);
    w_u16(object + 0x6a, (uint32)scale);
    w_u16(object + 0x6c, (uint32)scale);
    w_u32(object + 0x110, 0x80);
    random = G3SdkRandom80027FA8();
    w_u32(object + 0x114, (uint32)((sint64)(sint32)random % 256));
    w_u32(object + 0x118, (uint32)-10);
    if (same)
        G3ChunkyScatterSame(object, frame);
    else
        G3ChunkyScatter(object, frame);
}

void G3ChunkyScatter(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000344, "G3L_45");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x128;
    sint32 random = (sint32)G3SdkRandom80027FA8();
    uint32 speed = (uint32)(random % 32 + 12);
    random = (sint32)G3SdkRandom80027FA8();
    uint32 heading = (uint32)(random % 4096);
    uint32 angle = (uint32)(sint32)(sint16)heading;
    uint32 cosine = G3Function8005B470(angle);
    w_u16(frame + 0x10, cosine);
    uint32 sine = G3Function8005B590(angle);
    w_u16(frame + 0x12, sine);
    random = (sint32)G3SdkRandom80027FA8();
    w_u16(frame + 0x14, (uint32)(random % 4096));
    w_u16(object + 0x60, heading);
    uint32 yaw = r_u16(frame + 0x14);
    w_u16(object + 0x64, yaw);
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        uint32 model = r_u32(state + 0xc);
        uint32 direction = (uint32)r_s16(frame + 0x10 + axis * 2);
        uint32 radius = (uint32)r_s16(model + 0x26);
        uint32 product = direction * radius;
        uint32 position = r_u16(object + 0x58 + axis * 2);
        w_u16(object + 0x58 + axis * 2, position + (uint32)((sint32)product >> (axis == 2 ? 15 : 14)));
    }
    uint32 direction = (uint32)r_s16(frame + 0x10);
    uint32 product = direction * (uint32)(sint32)(sint16)speed;
    w_u32(object + 0x104, (uint32)((sint32)product >> 12));
    direction = (uint32)r_s16(frame + 0x12);
    product = direction * (uint32)(sint32)(sint16)speed;
    w_u32(object + 0x108, (uint32)((sint32)product >> 12));
    random = (sint32)G3SdkRandom80027FA8();
    w_u32(object + 0x10c, (uint32)(random % 64 + 64));
}

void G3ChunkyScatterSame(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000004f4, "G3L_45");
    uint32 frame = guest_stack - 0x30;
    uint32 resource = r_u32(object + 0x1c);
    uint32 index = (uint32)r_s16(object + 0xa2);
    uint32 models = r_u32(resource + 0xc);
    uint32 model = r_u32(models + index * 4);
    sint32 random = (sint32)G3SdkRandom80027FA8();
    uint32 speed = (uint32)(random % 12 + 4);
    random = (sint32)G3SdkRandom80027FA8();
    uint32 heading = (uint32)(random % 4096);
    uint32 angle = (uint32)(sint32)(sint16)heading;
    uint32 cosine = G3Function8005B470(angle);
    w_u16(frame + 0x10, cosine);
    uint32 sine = G3Function8005B590(angle);
    w_u16(frame + 0x12, sine);
    w_u16(object + 0x60, heading);
    uint32 old_pitch = r_u16(frame + 0x14);
    w_u16(object + 0x62, old_pitch);
    for (uint32 axis = 0; axis != 2; ++axis)
    {
        uint32 direction = (uint32)r_s16(frame + 0x10 + axis * 2);
        uint32 radius = (uint32)r_s16(model + 0x26);
        uint32 product = direction * radius;
        uint32 position = r_u16(object + 0x58 + axis * 2);
        w_u16(object + 0x58 + axis * 2, position + (uint32)((sint32)product >> 14));
    }
    random = (sint32)G3SdkRandom80027FA8();
    uint32 position = r_u16(object + 0x5c);
    w_u16(object + 0x5c, position + 16 + (uint32)(random % 64));
    uint32 direction = (uint32)r_s16(frame + 0x10);
    uint32 product = direction * (uint32)(sint32)(sint16)speed;
    w_u32(object + 0x104, (uint32)((sint32)product >> 12));
    direction = (uint32)r_s16(frame + 0x12);
    product = direction * (uint32)(sint32)(sint16)speed;
    w_u32(object + 0x108, (uint32)((sint32)product >> 12));
    random = (sint32)G3SdkRandom80027FA8();
    w_u32(object + 0x10c, (uint32)(random % 32 + 52));
}

void G3ChunkyUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000006a4, "G3L_45");
    uint32 frame = guest_stack - 0x18;
    uint32 lifetime = r_u32(object + 0x128) - 1;
    w_u32(object + 0x128, lifetime);
    if ((sint32)lifetime <= 0)
    {
        G3Function80038798(object, 0, 0, frame);
        return;
    }
    uint32 angle_x = (uint32)r_s16(object + 0x60);
    uint32 speed_x = r_u32(object + 0x110);
    uint32 vx = r_u16(object + 0x104);
    uint32 vy = r_u16(object + 0x108);
    uint32 turn_x = angle_x + speed_x;
    uint32 x = r_u16(object + 0x58);
    uint32 y = r_u16(object + 0x5a);
    w_u16(object + 0x58, x + vx);
    uint32 z = r_u16(object + 0x5c);
    uint32 vz = r_u16(object + 0x10c);
    w_u16(object + 0x5a, y + vy);
    uint32 velocity_z = r_u32(object + 0x10c);
    uint32 gravity = r_u32(object + 0x118);
    w_u16(object + 0x5c, z + vz);
    w_u32(object + 0x10c, velocity_z + gravity);
    uint32 angle_y = (uint32)r_s16(object + 0x62);
    uint32 speed_y = r_u32(object + 0x114);
    w_u16(object + 0x60, (uint32)((sint32)turn_x % 4096));
    w_u16(object + 0x62, (uint32)((sint32)(angle_y + speed_y) % 4096));
}

void G3ChunkyDestroy(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000778, "G3L_45");
    uint32 frame = guest_stack - 0x18;
    uint32 flags = r_u32(object + 0x18);
    w_u32(object + 0x18, flags & ~0x400u);
    G3Function80038798(object, 0, 0, frame);
}

void G3ChunkySplit(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000007ac, "G3L_45");
    uint32 frame = guest_stack - 0x20;
    sint32 random = (sint32)G3SdkRandom80027FA8();
    for (sint32 remaining = random % 2; remaining != -1; --remaining)
    {
        uint32 resource = r_u32(object + 0x1c);
        G3Function8002A454(object, resource, frame);
    }
}

void G3ChunkyCollide(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000818, "G3L_45");
    uint32 frame = guest_stack - 0x18;
    if (r_u32(object + 0x12c) != 0 && r_s16(object + 0x68) > 0x800)
        G3ChunkySplit(object, frame);
    G3ChunkyDestroy(object, frame);
}

uint32 G3ChunkyRender(uint32 camera, uint32 object, uint32 packets, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000086c, "G3L_45");
    uint32 frame = guest_stack - 0x98;
    uint32 state = object + 0x128;
    w_u32(frame + 0x98, camera);
    w_u32(frame + 0x68, state);
    uint32 resource = r_u32(object + 0x1c);
    uint32 index = (uint32)r_s16(object + 0xa2);
    uint32 bone = r_u32(object + 0x30);
    uint32 models = r_u32(resource + 0xc);
    uint32 model = r_u32(models + index * 4);
    if (bone == 0)
        return 0;
    uint32 faces = (uint32)r_s16(model + 4);
    uint32 end = r_u32(packets + 8);
    uint32 output = r_u32(packets + 4);
    if (output + faces * 48 >= end)
        return 0;
    sint32 segments = r_s16(model + 6);
    if (segments <= 0)
        return 0;
    uint32 segment_offset = 0;
    uint32 segment_index = 0;
    do
    {
        uint32 segment_table = r_u32(model + 0x18);
        if (r_s16(segment_table + segment_offset + 6) != -1)
        {
            uint32 game = r_u32(0x80097048);
            uint32 level = r_u32(game + 0x48);
            uint32 current_state = r_u32(frame + 0x68);
            uint32 lights = r_u32(level + 4);
            uint32 light_index = r_u32(current_state + 0x18);
            uint32 light = r_u32(lights + 8) + (light_index << 6);
            MulMatrix0((MATRIX *)psx_addr(light, 20), (MATRIX *)psx_addr(bone, 20), (MATRIX *)psx_addr(frame + 0x38, 20));
            sint32 divisor = r_s16(object + 0x68);
            uint32 scale = divisor == 0 ? 0xffffffff : (uint32)(0x1000000 / divisor);
            w_u32(frame + 0x60, scale);
            w_u32(frame + 0x5c, scale);
            w_u32(frame + 0x58, scale);
            G3Function8005ADF0(frame + 0x38, frame + 0x58);
            SetLightMatrix((MATRIX *)psx_addr(frame + 0x38, 20));
            SetColorMatrix((MATRIX *)psx_addr(light + 0x20, 20));
            uint32 view = r_u32(frame + 0x98);
            uint32 first = r_u32(view);
            uint32 second = r_u32(view + 4);
            xport_gte_write_control(0, first);
            xport_gte_write_control(1, second);
            first = r_u32(view + 8);
            second = r_u32(view + 0xc);
            uint32 third = r_u32(view + 0x10);
            xport_gte_write_control(2, first);
            xport_gte_write_control(3, second);
            xport_gte_write_control(4, third);
            for (uint32 column = 0; column != 3; ++column)
            {
                first = r_u16(bone + column * 2);
                second = r_u16(bone + column * 2 + 6);
                third = r_u16(bone + column * 2 + 0xc);
                xport_gte_write_data(9, first);
                xport_gte_write_data(10, second);
                xport_gte_write_data(11, third);
                xport_gte_execute(0x49e012);
                first = xport_gte_read_data(9);
                second = xport_gte_read_data(10);
                third = xport_gte_read_data(11);
                w_u16(frame + 0x18 + column * 2, first);
                w_u16(frame + 0x1e + column * 2, second);
                w_u16(frame + 0x24 + column * 2, third);
            }
            first = r_u32(view + 0x14);
            second = r_u32(view + 0x18);
            xport_gte_write_control(5, first);
            third = r_u32(view + 0x1c);
            xport_gte_write_control(6, second);
            xport_gte_write_control(7, third);
            second = r_u16(bone + 0x18);
            first = r_u16(bone + 0x14);
            xport_gte_write_data(0, first | (second << 16));
            xport_gte_write_data(1, r_u32(bone + 0x1c));
            xport_gte_execute(0x480012);
            w_u32(frame + 0x2c, xport_gte_read_data(25));
            w_u32(frame + 0x30, xport_gte_read_data(26));
            w_u32(frame + 0x34, xport_gte_read_data(27));
            first = r_u32(frame + 0x18);
            second = r_u32(frame + 0x1c);
            xport_gte_write_control(0, first);
            xport_gte_write_control(1, second);
            first = r_u32(frame + 0x20);
            second = r_u32(frame + 0x24);
            third = r_u32(frame + 0x28);
            xport_gte_write_control(2, first);
            xport_gte_write_control(3, second);
            xport_gte_write_control(4, third);
            first = r_u32(frame + 0x2c);
            second = r_u32(frame + 0x30);
            xport_gte_write_control(5, first);
            third = r_u32(frame + 0x34);
            xport_gte_write_control(6, second);
            xport_gte_write_control(7, third);
            segment_table = r_u32(model + 0x18);
            uint32 ordering_table = r_u32(frame + 0xa8);
            w_u32(frame + 0x10, ordering_table);
            output = G3ChunkyRenderFaces(object, segment_table + segment_offset, packets, model, frame);
            w_u32(packets + 4, output);
        }
        segment_offset += 0x18;
        segments = r_s16(model + 6);
        ++segment_index;
        bone += 0x20;
    } while ((sint32)segment_index < segments);
    return 0;
}

uint32 G3ChunkyRenderFaces(uint32 object, uint32 segment, uint32 packets, uint32 model, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000b90, "G3L_45");
    uint32 frame = guest_stack - 0x40;
    uint32 state = object + 0x128;
    uint32 output = r_u32(packets + 4);
    uint32 first_index = (uint32)r_s16(segment + 4);
    uint32 material_index = r_u32(state + 0x14);
    uint32 faces = r_u32(model + 0x14);
    uint32 ordering_table = r_u32(frame + 0x50);
    uint32 vertices = r_u32(model + 8);
    uint32 last_index = (uint32)r_s16(segment + 6);
    uint32 face = faces + first_index * 12;
    uint32 last = faces + last_index * 12;
    if (last < face)
        return output;
    w_u32(frame + 0x18, frame + 8);
    uint32 normal_address = face + 6;
    uint32 material_offset = material_index * 12;
    do
    {
        uint32 limit = r_u32(packets + 8) - 0x30;
        if (limit < output)
            return output;
        uint32 first = r_u16(face);
        uint32 second = r_u16(normal_address - 4);
        uint32 third = r_u16(normal_address - 2);
        uint32 first_vertex = vertices + (first << 3);
        uint32 second_vertex = vertices + (second << 3);
        uint32 third_vertex = vertices + (third << 3);
        xport_gte_write_data(0, r_u32(first_vertex));
        xport_gte_write_data(1, r_u32(first_vertex + 4));
        xport_gte_write_data(2, r_u32(second_vertex));
        xport_gte_write_data(3, r_u32(second_vertex + 4));
        xport_gte_write_data(4, r_u32(third_vertex));
        xport_gte_write_data(5, r_u32(third_vertex + 4));
        xport_gte_execute(0x280030);
        uint32 material_model = r_u32(state + 0xc);
        uint32 materials = r_u32(material_model + 0x14);
        ++material_index;
        uint32 material = materials + material_offset;
        sint32 material_count = r_s16(material_model + 4);
        uint32 texture = r_u32(material + 8);
        material_offset += 12;
        if ((sint32)material_index >= material_count)
        {
            material_offset = 0;
            material_index = 0;
        }
        w_u32(frame, xport_gte_read_flag());
        uint32 flags = r_u32(frame);
        if ((flags & 0x60000) == 0)
        {
            xport_gte_execute(0x1400006);
            w_u32(frame + 4, xport_gte_read_data(24));
            uint32 game = r_u32(0x80097048);
            uint32 winding = r_u32(game);
            sint32 area = r_s32(frame + 4);
            if ((winding == 0 && area < 0) || (winding != 0 && area > 0))
            {
                uint32 depth_pointer = r_u32(frame + 0x18);
                w_u32(depth_pointer, xport_gte_read_data(17));
                w_u32(frame + 0xc, xport_gte_read_data(18));
                w_u32(frame + 0x10, xport_gte_read_data(19));
                w_u32(frame + 0x14, xport_gte_read_data(8));
                sint32 depth_first = r_s32(frame + 8);
                sint32 depth_second = r_s32(frame + 0xc);
                game = r_u32(0x80097048);
                uint32 shift = r_u32(game + 0x2c);
                sint32 depth_third = r_s32(frame + 0x10);
                sint32 maximum = depth_second < depth_first ? depth_first : depth_second;
                if (depth_third > maximum)
                    maximum = depth_third;
                sint32 bucket = maximum >> (shift & 31);
                game = r_u32(0x80097048);
                sint32 buckets = r_s32(game + 0x28);
                if (bucket < buckets && bucket >= 0)
                {
                    bool textured = (r_u8(material + 7) & 2) != 0;
                    w_u32(output + 8, xport_gte_read_data(12));
                    w_u32(output + (textured ? 0x10 : 0xc), xport_gte_read_data(13));
                    w_u32(output + (textured ? 0x18 : 0x10), xport_gte_read_data(14));
                    uint32 normal_index = r_u8(normal_address);
                    uint32 normal = 0x80098838 + (normal_index << 3);
                    xport_gte_write_data(0, r_u32(normal));
                    xport_gte_write_data(1, r_u32(normal + 4));
                    xport_gte_write_data(6, r_u32(textured ? texture + 0xc : material + 8));
                    uint32 factor = r_u32(frame + 0x14);
                    xport_gte_write_data(8, factor);
                    xport_gte_execute(0xe80413);
                    w_u32(output + 4, xport_gte_read_data(22));
                    uint32 ordering_entry = ordering_table + ((uint32)bucket << 2);
                    if (textured)
                    {
                        uint32 uv = r_u32(texture);
                        w_u32(output + 0xc, uv);
                        uv = r_u32(texture + 4);
                        w_u32(output + 0x14, uv);
                        uv = r_u32(texture + 8);
                        w_u8(output + 3, 7);
                        w_u8(output + 7, 0x24);
                        w_u32(output + 0x1c, uv);
                        uint32 link = r_u32(ordering_entry);
                        w_u32(output, (link & 0xffffff) | 0x07000000);
                        w_u32(ordering_entry, output & 0xffffff);
                        uint32 count = r_u32(packets);
                        output += 0x20;
                        w_u32(packets, count + 1);
                    }
                    else
                    {
                        w_u8(output + 3, 4);
                        w_u8(output + 7, 0x20);
                        uint32 link = r_u32(ordering_entry);
                        w_u32(output, (link & 0xffffff) | 0x04000000);
                        w_u32(ordering_entry, output & 0xffffff);
                        uint32 count = r_u32(packets);
                        output += 0x14;
                        w_u32(packets, count + 1);
                    }
                }
            }
        }
        normal_address += 12;
        face += 12;
    } while (face <= last);
    return output;
}

uint32 G3CoinEvent(uint32 module_base, uint32 object, uint32 game, uint32 message, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_72");
    uint32 frame = guest_stack - 0x20;
    switch (message)
    {
    case 1:
        G3CoinInitialize(object, game, frame);
        return 1;
    case 2:
        G3CoinUpdate(object, game, frame);
        return 1;
    case 3:
        G3CoinCollide(module_base, object, game, frame);
        G3CoinCollect(module_base, object, game, frame);
        return 1;
    case 8:
        G3CoinCollect(module_base, object, game, frame);
        return 1;
    case 2020:
        if (G3CoinCollect(module_base, object, game, frame) != 0)
            G3Function800193F8(0x01000000, object, frame);
        return 1;
    default:
        return 0;
    }
}

void G3CoinInitialize(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000e8, "G3L_72");
    uint32 frame = guest_stack - 0x20;
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x10, flags | 0x80);
    flags = r_u32(object + 0x128);
    uint32 state = object + 0x128;
    w_u32(object + 0x128, flags & 0xfffffffe);
    w_u16(state + 0x14, 0x8300);
    uint32 first_resource = r_u32(0x80098774);
    uint32 resource = r_u32(object + 0x1c);
    uint32 kind;
    if (first_resource == resource)
        kind = 0;
    else if (r_u32(0x8009876c) == resource)
        kind = 1;
    else
        kind = 3;
    w_u32(state + 0xc, kind);
    if (r_u32(object + 0x2c) != 0)
    {
        flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags | 0x00100000);
    }
    flags = r_u32(object + 0x10);
    uint32 intro = r_u32(object + 0x20);
    w_u32(object + 0x40, 0);
    w_u32(object + 0x10, flags | 0x40);
    if (intro == 0 || r_u16(game + 0x68b4) >= 28)
        return;
    bool collected = false;
    if (r_u32(state + 0xc) == 0)
    {
        uint32 bit = G3Function80039DFC(intro);
        uint32 level = r_u16(game + 0x68b4);
        sint32 mask = r_s16(game + 0x683c + (level << 1));
        collected = (((uint32)(mask >> (bit & 31))) & 1) != 0;
    }
    if (!collected && r_u32(state + 0xc) == 1)
    {
        intro = r_u32(object + 0x20);
        uint32 bit = G3Function80039DFC(intro);
        uint32 level = r_u16(game + 0x68b4);
        sint32 mask = r_s16(game + 0x6874 + (level << 1));
        collected = (((uint32)(mask >> (bit & 31))) & 1) != 0;
    }
    if (collected)
    {
        flags = r_u32(object + 0x10);
        w_u32(object + 0x10, ((flags | 0x10) & 0xfffffbff) | 0x20);
    }
}

uint32 G3CoinCollect(uint32 module_base, uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000264, "G3L_72");
    uint32 frame = guest_stack - 0x28;
    uint32 flags = r_u32(object + 0x128);
    uint32 camera = r_u32(game + 0x4c);
    (void)camera;
    uint32 state = object + 0x128;
    if ((flags & 1) != 0)
        return 0;
    if (G3Function8004DACC() != 0)
        return 0;
    uint32 intro = r_u32(object + 0x20);
    if (intro != 0 && (r_u32(intro + 0x18) & 0x80) != 0)
        return 0;
    G3Function800193F8(0x80000000, object, frame);
    flags = r_u32(object + 0x128);
    w_u32(object + 0x128, flags | 1);
    uint32 kind = r_u32(state + 0xc);
    if (kind == 0)
    {
        uint32 count = r_u32(game + 0x6824);
        uint32 level = r_u16(game + 0x68b4);
        w_u32(game + 0x6824, count + 1);
        if (level < 28)
        {
            intro = r_u32(object + 0x20);
            if (intro != 0)
            {
                uint32 bit = G3Function80039DFC(intro);
                level = r_u16(game + 0x68b4);
                uint32 address = game + 0x683c + (level << 1);
                uint32 mask = r_u16(address);
                w_u16(address, mask | (1u << (bit & 31)));
            }
        }
        if (r_s32(game + 0x6824) >= 25)
        {
            sint32 lives = r_s16(game + 0x681e);
            uint32 current = r_u16(game + 0x681e);
            if (lives < 8)
            {
                uint32 next = current + 1;
                w_u16(game + 0x681c, next);
                uint32 value = (uint32)r_s16(game + 0x681c);
                count = r_u32(game + 0x6824);
                w_u16(game + 0x681e, next);
                w_u32(game + 0x6824, count - 25);
                G3Function80047EE4(7, value);
            }
        }
        G3Function80047EE4(4, r_u32(game + 0x6824));
    }
    else if (kind == 1)
    {
        uint32 count = r_u32(game + 0x6828);
        uint32 level = r_u16(game + 0x68b4);
        w_u32(game + 0x6828, count + 1);
        if (level < 28)
        {
            intro = r_u32(object + 0x20);
            if (intro != 0)
            {
                uint32 bit = G3Function80039DFC(intro);
                level = r_u16(game + 0x68b4);
                uint32 address = game + 0x6874 + (level << 1);
                uint32 mask = r_u16(address);
                w_u16(address, mask | (1u << (bit & 31)));
            }
        }
        G3Function80047EE4(2, r_u32(game + 0x6828));
    }
    uint32 target = G3Function80039B78(module_base + 0x570);
    if (target != 0)
    {
        uint32 current_game = r_u32(0x80097048);
        w_u32(object + 0xec, target);
        G3DispatchStateEvent(target, object, current_game, 1, 0, frame);
    }
    return 1;
}

void G3CoinCollide(uint32 module_base, uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000045c, "G3L_72");
    uint32 frame = guest_stack - 0x18;
    uint32 contact = r_u32(object + 0xb0);
    uint32 other = r_u32(contact + 0x14);
    uint32 player = r_u32(0x8009efc4);
    if (other == player && (r_u32(other + 0x128) & 0x20000) == 0)
        G3CoinCollect(module_base, object, game, frame);
}

void G3CoinUpdate(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000004ac, "G3L_72");
    uint32 frame = guest_stack - 0x28;
    uint32 rotation = r_u16(object + 0x64);
    w_u16(object + 0x64, rotation + 0x80);
    uint32 sound = r_u32(object + 0x40);
    uint32 state = object + 0x128;
    if (sound != 0)
    {
        w_u32(frame + 0x10, 1500);
        sound = r_u32(object + 0x40);
        G3Function8003E7E4(object, sound, 0, 0x7f, frame);
    }
    uint32 timer = r_u16(object + 0x9e);
    uint32 flags = r_u32(object + 0x128);
    w_u16(object + 0x9e, timer + 1);
    if (flags == 0)
        return;
    uint32 remaining = r_u32(state + 0x10);
    if (remaining == 0)
        return;
    G3BlinkCollectedObject800389E0(object, 450, (sint32)remaining);
    remaining = r_u32(state + 0x10) - 1;
    w_u32(state + 0x10, remaining);
    if (remaining == 0)
    {
        G3Function800193F8(0x00040000, object, frame);
        G3Function8002A9AC(object, frame);
    }
}

void G3BlinkCollectedObject800389E0(uint32 object, sint32 duration, sint32 counter)
{
    uint32 adjusted = (uint32)duration;
    if (duration < 0)
        adjusted += 3;
    sint32 quarter = (sint32)adjusted >> 2;
    uint32 mask = counter < quarter ? 1 : 2;
    uint32 half_word = (uint32)duration + ((uint32)duration >> 31);
    sint32 half = (sint32)half_word >> 1;
    if (counter >= half)
        return;
    uint32 flags = r_u32(object + 0x10);
    if (((uint32)counter & mask) != 0)
        w_u32(object + 0x10, flags | 0x800);
    else
        w_u32(object + 0x10, flags & 0xfffff7ff);
}

uint32 G3CoverEvent(uint32 module_base, uint32 object, uint32 message, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_54");
    uint32 frame = guest_stack - 0x18;
    if (message == 1)
    {
        G3CoverInitialize(module_base, object);
        return 1;
    }
    if (message == 2)
    {
        G3CoverUpdate(object, frame);
        return 1;
    }
    return message == 4 ? 1 : 0;
}

void G3CoverInitialize(uint32 module_base, uint32 object)
{
    FUNCTION_MARKER(0x00000078, "G3L_54");
    uint32 state = object + 0x104;
    uint32 parent = r_u32(object + 0x2c);
    uint32 resource = r_u32(object + 0x1c);
    sint32 model_index = r_s16(parent + 0xa2);
    uint32 parent_resource = r_u32(parent + 0x1c);
    uint32 data = r_u32(resource + 0x1c);
    uint32 models = r_u32(parent_resource + 0xc);
    uint32 model = r_u32(models + ((uint32)model_index << 2));
    sint32 count = r_s16(model + 6);
    w_u32(object + 0x104, count >= 2 ? 1 : 0);
    w_u32(state + 4, (uint32)count - 1);
    sint32 divisor = r_s32(data);
    uint32 quotient = divisor == 0 ? (count < 0 ? 1 : 0xffffffff) : (uint32)(count / divisor);
    w_u32(state + 0xc, 0);
    w_u32(state + 8, quotient + 1);
    parent = r_u32(object + 0x2c);
    w_u32(object + 0xf0, module_base + 0x31c);
    uint32 flags = r_u32(parent + 0x10);
    w_u32(parent + 0x10, flags | 0x400);
    flags = r_u32(object + 0x10);
    w_u32(object + 0x10, flags | 0x400);
    flags = r_u32(object + 0x14);
    w_u32(object + 0x14, flags | 0x02000000);
}

void G3CoverUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000124, "G3L_54");
    uint32 frame = guest_stack - 0x18;
    uint32 state = object + 0x104;
    uint32 timer = r_u32(state + 0xc) + 1;
    w_u32(state + 0xc, timer);
    uint32 parent = r_u32(object + 0x2c);
    uint32 matrices = r_u32(parent + 0x30);
    if (matrices != 0)
    {
        uint32 index = r_u32(object + 0x104);
        uint32 component = r_u16(matrices + (index << 5) + 0x14);
        parent = r_u32(object + 0x2c);
        w_u16(object + 0x58, component);
        index = r_u32(object + 0x104);
        matrices = r_u32(parent + 0x30);
        component = r_u16(matrices + (index << 5) + 0x18);
        parent = r_u32(object + 0x2c);
        w_u16(object + 0x5a, component);
        index = r_u32(object + 0x104);
        matrices = r_u32(parent + 0x30);
        w_u16(object + 0x5c, r_u16(matrices + (index << 5) + 0x1c));
    }
    timer = r_u32(state + 0xc);
    if ((sint32)timer < 32)
    {
        uint32 scale = timer << 7;
        w_u16(object + 0x68, scale);
        w_u16(object + 0x6a, scale);
        w_u16(object + 0x6c, scale);
    }
    bool shrink_cover = false;
    if (r_s32(state + 0xc) >= 71)
    {
        parent = r_u32(object + 0x2c);
        sint32 parent_scale = r_s16(parent + 0x6c);
        uint32 scale = r_u16(parent + 0x6c);
        if (parent_scale < 201)
            shrink_cover = true;
        else
            w_u16(parent + 0x6c, scale - 200);
    }
    if (!shrink_cover)
    {
        parent = r_u32(object + 0x2c);
        if (r_s16(parent + 0x6c) >= 201)
            return;
    }
    sint32 cover_scale = r_s16(object + 0x6c);
    uint32 scale = r_u16(object + 0x6c);
    if (cover_scale >= 129)
        w_u16(object + 0x6c, scale - 128);
    parent = r_u32(object + 0x2c);
    if (r_s16(parent + 0x6c) >= 201 || r_s16(object + 0x6c) >= 129)
        return;
    uint32 flags = r_u32(object + 0x10);
    parent = r_u32(object + 0x2c);
    w_u32(object + 0x10, flags & 0xfffffbff);
    flags = r_u32(parent + 0x10);
    w_u32(parent + 0x10, flags & 0xfffffbff);
    flags = r_u32(object + 0x10);
    parent = r_u32(object + 0x2c);
    w_u32(object + 0x10, flags | 0x10);
    flags = r_u32(parent + 0x10);
    w_u32(parent + 0x10, flags | 0x10);
    flags = r_u32(object + 0x10);
    parent = r_u32(object + 0x2c);
    w_u32(object + 0x10, flags | 0x20);
    flags = r_u32(parent + 0x10);
    w_u32(parent + 0x10, flags | 0x20);
    parent = r_u32(object + 0x2c);
    G3Function800193F8(0x40000, parent, frame);
    parent = r_u32(object + 0x2c);
    uint32 intro = r_u32(parent + 0x20);
    if (intro != 0 && (r_u32(intro + 0x18) & 4) != 0)
        G3Function800392A0(parent, frame);
}

uint32 G3CoverRender(uint32 scene, uint32 object, uint32 vertices, uint32 packets, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000031c, "G3L_54");
    uint32 frame = guest_stack - 0x50;
    uint32 parent = r_u32(object + 0x2c);
    uint32 matrices = r_u32(parent + 0x30);
    uint32 order_table = r_u32(guest_stack + 0x10);
    uint32 state = object + 0x104;
    if (matrices == 0)
        return 0;
    if (r_u32(object + 0x30) == 0)
        return 0;
    uint32 resource = r_u32(object + 0x1c);
    sint32 model_index = r_s16(object + 0xa2);
    sint32 original_time = r_s16(object + 0x9e);
    uint32 models = r_u32(resource + 0xc);
    uint32 lookup = models + ((uint32)model_index << 2);
    uint32 component = r_u16(object + 0x58);
    uint32 model = r_u32(lookup);
    w_u16(frame + 0x20, component);
    w_u16(frame + 0x22, r_u16(object + 0x5a));
    w_u16(frame + 0x24, r_u16(object + 0x5c));
    uint32 index = r_u32(object + 0x104);
    uint32 last = r_u32(state + 4);
    while ((sint32)index <= (sint32)last)
    {
        uint32 time = r_u16(object + 0x9e) + 1;
        w_u16(object + 0x9e, time);
        uint32 sequence = r_u32(model + 0x1c);
        G3Function80034FA4(sequence, (uint32)(sint32)(sint16)time, 0, 0);
        parent = r_u32(object + 0x2c);
        uint32 offset = index << 5;
        matrices = r_u32(parent + 0x30);
        parent = r_u32(object + 0x2c);
        component = r_u16(matrices + offset + 0x14);
        w_u16(object + 0x58, component);
        matrices = r_u32(parent + 0x30);
        component = r_u16(matrices + offset + 0x18);
        w_u16(object + 0x5a, component);
        matrices = r_u32(parent + 0x30);
        component = r_u16(matrices + offset + 0x1c);
        uint32 output = r_u32(object + 0x30);
        sint32 x = r_s16(object + 0x58);
        w_u16(object + 0x5c, component);
        w_u32(output + 0x14, (uint32)x);
        output = r_u32(object + 0x30);
        w_u32(output + 0x18, (uint32)r_s16(object + 0x5a));
        output = r_u32(object + 0x30);
        w_u32(output + 0x1c, (uint32)r_s16(object + 0x5c));
        w_u32(frame + 0x10, order_table);
        w_u32(frame + 0x14, 0);
        w_u32(frame + 0x18, 0);
        G3Function80033BA8(object, scene, vertices, packets, order_table, 0, 0, frame);
        uint32 step = r_u32(state + 8);
        last = r_u32(state + 4);
        index += step;
    }
    w_u16(object + 0x9e, (uint32)original_time);
    uint32 sequence = r_u32(model + 0x1c);
    G3Function80034FA4(sequence, (uint32)original_time, 0, 0);
    w_u16(object + 0x58, r_u16(frame + 0x20));
    w_u16(object + 0x5a, r_u16(frame + 0x22));
    w_u16(object + 0x5c, r_u16(frame + 0x24));
    return 0;
}

uint32 G3CowboyEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_10");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3CowboyInitialize(object, module_base, frame);
        return 1;
    case 2:
        G3CowboyUpdate(object, game, module_base, frame);
        return 1;
    case 3:
        G3CowboyCollide(object, game, frame);
        return 1;
    case 5:
    {
        uint32 data = r_u32(payload + 0x1c);
        if (data != 0)
            w_u16(data + 0x16, 0);
        return 1;
    }
    case 8:
    case 0x7e4:
    {
        uint32 mode = r_u32(object + 0x150);
        if (mode == 0 || mode == 7 || mode == 6)
            return 1;
        uint32 state = object + 0x104;
        uint32 health = r_u16(state + 0x30) - 1;
        uint32 resource = r_u32(object + 0x1c);
        uint32 config = r_u32(resource + 0x1c);
        if ((sint16)health > 0)
        {
            w_u32(object + 0x150, 7);
            w_u16(object + 0x6e, 0);
            w_u16(object + 0x5e, 0);
            uint32 persisted = r_u32(state + 0x34);
            w_u16(state + 0x30, health);
            if (persisted != 0)
                w_u16(persisted, health);
            uint32 cooldown = r_u16(config + 0x1e);
            w_u16(state + 0x28, cooldown);
        }
        else
        {
            uint32 selector = (uint32)r_s16(config + 0x26);
            uint32 range = (uint32)r_s16(config + 0x28);
            G3Function80038798(object, selector, range, frame);
        }
        return 1;
    }
    default:
        return 0;
    }
}

void G3CowboyInitialize(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000014c, "G3L_10");
    uint32 frame = guest_stack - 0x20;
    uint32 resource = r_u32(object + 0x1c);
    uint32 custom = r_u32(object + 0x28);
    uint32 x = r_u16(object + 0x58);
    uint32 config = r_u32(resource + 0x1c);
    uint32 state = object + 0x104;
    w_u16(state + 0xc, x);
    uint32 y = r_u16(object + 0x5a);
    w_u16(state + 0xe, y);
    uint32 z = r_u16(object + 0x5c);
    w_u16(state + 0x10, z);
    uint32 parameters = custom == 0 ? config + 0x2a : custom;
    uint32 radius = (uint32)r_s16(parameters);
    w_u32(state, radius * radius);
    radius = (uint32)r_s16(parameters + 2);
    w_u32(state + 4, radius * radius);
    uint32 origins[3] = { x, y, z };
    for (uint32 index = 0; index != 9; ++index)
    {
        uint32 offset = r_u16(parameters + 4 + index * 2);
        w_u16(state + 0x12 + index * 2, origins[index % 3] + offset);
    }
    if (custom != 0)
    {
        sint32 health = r_s16(custom + 0x16);
        uint32 health_half = r_u16(custom + 0x16);
        if (health != 0)
            w_u16(state + 0x30, health_half);
        else
        {
            health_half = r_u16(config + 0x20);
            w_u16(state + 0x30, health_half);
        }
        w_u32(state + 0x34, custom + 0x16);
    }
    else
    {
        uint32 health = r_u16(config + 0x20);
        w_u32(state + 0x34, 0);
        w_u16(state + 0x30, health);
    }
    if (r_s16(config + 0x40) == 0)
    {
        sint32 turn = (sint32)((uint32)r_s16(config) << 12);
        sint32 threshold = (sint32)((uint32)r_s16(config + 2) << 12);
        sint32 projectile_angle = (sint32)((uint32)r_s16(config + 0x1a) << 12);
        sint32 rotation = (sint32)((uint32)r_s16(config + 0x22) << 12);
        w_u16(config + 0x40, 1);
        w_u16(config, (uint32)(turn / 21600));
        sint32 scale = (sint32)((uint32)r_s16(config + 0x24) << 12);
        w_u16(config + 2, (uint32)(threshold / 21600));
        w_u16(config + 0x1a, (uint32)(projectile_angle / 360));
        w_u16(config + 0x22, (uint32)(rotation / 21600));
        w_u16(config + 0x24, (uint32)(scale / 1000));
    }
    uint32 flags = r_u32(object + 0x10);
    uint32 yaw = (uint32)r_s16(object + 0x64);
    w_u32(object + 0x150, 0);
    w_u32(object + 0x10, flags | 0x800);
    w_u32(state + 8, yaw);
    uint32 random = G3SdkRandom80027FA8();
    w_u16(state + 0x26, 0);
    w_u16(state + 0x28, 0);
    w_u16(state + 0x24, (uint32)((sint64)(sint32)random % 3));
    uint32 projectile_resource = G3Function8003AA48(module_base + 0xf1c);
    w_u32(state + 0x2c, projectile_resource);
    if (projectile_resource == 0)
        G3Function80038798(object, 0, 0, frame);
}

void G3CowboyShoot(uint32 object, uint32 game, uint32 offset, uint32 bone, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000004a0, "G3L_10");
    uint32 frame = guest_stack - 0x38;
    uint32 resource = r_u32(object + 0x1c);
    uint32 projectile_resource = r_u32(object + 0x130);
    uint32 config = r_u32(resource + 0x1c);
    uint32 projectile = G3Function8002A454(object, projectile_resource, frame);
    if (projectile == 0)
        return;
    uint32 heading = (uint32)r_s16(object + 0x64) - 0x400;
    uint32 heading_sine = G3Function8005B590(heading);
    uint32 heading_cosine = G3Function8005B470(heading);
    uint32 matrix = r_u32(object + 0x30);
    uint32 x = (uint32)r_s16(offset);
    matrix += (uint32)(sint32)(sint16)bone << 5;
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        if (axis != 0)
            x = (uint32)r_s16(offset);
        uint32 a = (uint32)r_s16(matrix + axis * 6);
        uint32 product_x = x * a;
        uint32 y = (uint32)r_s16(offset + 2);
        uint32 b = (uint32)r_s16(matrix + axis * 6 + 2);
        uint32 product_y = y * b;
        uint32 z = (uint32)r_s16(offset + 4);
        uint32 c = (uint32)r_s16(matrix + axis * 6 + 4);
        uint32 product_z = z * c;
        uint32 position = (uint32)((sint32)product_x >> 12) + (uint32)((sint32)product_y >> 12) + (uint32)((sint32)product_z >> 12);
        w_u16(projectile + 0x58 + axis * 2, position);
    }
    for (uint32 axis = 0; axis != 2; ++axis)
    {
        uint32 position = r_u16(projectile + 0x58 + axis * 2);
        uint32 translation = r_u16(matrix + 0x14 + axis * 4);
        w_u16(projectile + 0x58 + axis * 2, position + translation);
    }
    uint32 position = r_u16(projectile + 0x5c);
    uint32 translation = r_u16(matrix + 0x1c);
    w_u16(projectile + 0x62, 0);
    w_u16(projectile + 0x60, 0);
    w_u16(projectile + 0x5c, position + translation);
    uint32 yaw = r_u16(object + 0x64);
    w_u16(projectile + 0x64, yaw);
    uint32 speed = (uint32)r_s16(config + 0x18);
    uint32 product = speed * heading_cosine;
    w_u16(frame + 0x10, (uint32)((sint32)product >> 12));
    speed = (uint32)r_s16(config + 0x18);
    product = speed * heading_sine;
    w_u16(frame + 0x12, (uint32)((sint32)product >> 12));
    uint32 pitch = (uint32)r_s16(config + 0x1a);
    uint32 pitch_cosine = G3Function8005B470(pitch);
    pitch = (uint32)r_s16(config + 0x1a);
    uint32 pitch_sine = G3Function8005B590(pitch);
    uint32 vx = (uint32)r_s16(frame + 0x10);
    uint32 product_x = vx * pitch_cosine;
    uint32 vy = (uint32)r_s16(frame + 0x12);
    uint32 product_y = vy * pitch_cosine;
    w_u16(frame + 0x10, (uint32)((sint32)product_x >> 12));
    w_u16(frame + 0x12, (uint32)((sint32)product_y >> 12));
    speed = (uint32)r_s16(config + 0x18);
    product = speed * pitch_sine;
    w_u16(frame + 0x14, 0u - (uint32)((sint32)product >> 12));
    uint32 callback = r_u32(projectile + 0xec);
    if (callback != 0)
        G3DispatchStateEvent(callback, projectile, game, 0x7d9, frame + 0x10, frame);
}

void G3CowboyCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000da4, "G3L_10");
    uint32 frame = guest_stack - 0x38;
    uint32 state = object + 0x104;
    uint32 contact = r_u32(object + 0xb0);
    uint32 resource = r_u32(object + 0x1c);
    uint32 player = r_u32(game + 0x50);
    uint32 config = r_u32(resource + 0x1c);
    G3Function8003C548(contact, frame + 0x10, frame + 0x11);
    if (r_u32(contact + 0x14) != player)
        return;
    uint32 kind = r_u8(frame + 0x11);
    if (kind == 9 || kind - 3 < 3)
    {
        uint32 mode = r_u32(object + 0x150);
        if (mode != 0 && mode != 6 && r_s16(state + 0x26) != 0)
        {
            G3Function80074044(object, game, frame);
            return;
        }
    }
    kind = r_u8(frame + 0x11);
    if (kind - 6 >= 4)
        return;
    uint32 mode = r_u32(object + 0x150);
    if (mode == 0 || mode == 7 || mode == 6)
        return;
    if ((r_u32(player + 0x154) & 0xa0) != 0)
        return;
    uint32 health = r_u16(state + 0x30) - 1;
    if ((sint16)health > 0)
    {
        w_u32(object + 0x150, 7);
        w_u16(object + 0x6e, 0);
        w_u16(object + 0x5e, 0);
        uint32 persisted_health = r_u32(state + 0x34);
        w_u16(state + 0x30, health);
        if (persisted_health != 0)
            w_u16(persisted_health, health);
        uint32 cooldown = r_u16(config + 0x1e);
        w_u16(state + 0x28, cooldown);
    }
    else
    {
        uint32 selector = (uint32)r_s16(config + 0x26);
        uint32 random_range = (uint32)r_s16(config + 0x28);
        G3Function80038798(object, selector, random_range, frame);
    }
}

void G3CowboyUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000744, "G3L_10");
    uint32 frame = guest_stack - 0x30;
    uint32 player = r_u32(game + 0x50);
    uint32 mode = r_u32(object + 0x150);
    uint32 player_position = player + 0x58;
    uint32 resource = r_u32(object + 0x1c);
    uint32 config = r_u32(resource + 0x1c);
    uint32 state = object + 0x104;
    if (mode >= 8)
        return;
    uint32 target = r_u32(module_base + 0xf2c + mode * 4);
    switch (target - module_base)
    {
    case 0x7a8:
    {
        {
            uint32 source = state + 0xc;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 position = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            uint32 z = (uint32)r_s16(source + 4);
            uint32 destination = object + 0x58;
            high_address = destination + 3;
            high_lane = high_address & 3;
            lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, position >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, position << (lane * 8), (15u << lane) & 15);
            w_u16(object + 0x5c, z);
        }
        uint32 hidden_z = r_u16(object + 0x5c);
        w_u16(object + 0x5c, hidden_z - 1000);
        sint32 timer = r_s16(state + 0x28);
        uint32 timer_half = r_u16(state + 0x28);
        if (timer > 0)
        {
            --timer_half;
            w_u16(state + 0x28, timer_half);
            if ((timer_half & 0xffff) != 0)
                return;
        }
        uint32 distance = 0;
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 origin = r_u16(state + 0xc + axis * 2);
            uint32 coordinate = r_u16(player_position + axis * 2);
            uint32 delta = origin - coordinate;
            uint32 signed_delta = (uint32)(sint32)(sint16)delta;
            distance += signed_delta * signed_delta;
            w_u16(frame + 0x10 + axis * 2, delta);
        }
        uint32 radius = r_u32(state);
        if ((sint32)radius < (sint32)distance)
            return;
        w_u32(object + 0x150, 1);
        uint32 flags = r_u32(object + 0x10);
        w_u16(object + 0x6e, 0);
        w_u16(object + 0x5e, 0);
        w_u32(object + 0x10, flags & 0xfffff7ff);
        uint32 selected = (uint32)r_s16(state + 0x24);
        uint32 source = state + selected * 6 + 0x12;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 lane = source & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
        uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
        uint32 position = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        uint32 z = (uint32)r_s16(source + 4);
        uint32 destination = object + 0x58;
        high_address = destination + 3;
        high_lane = high_address & 3;
        lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, position >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, position << (lane * 8), (15u << lane) & 15);
        w_u16(object + 0x5c, z);
        w_u16(object + 0x6c, 0);
        w_u16(object + 0x6a, 0);
        w_u16(object + 0x68, 0);
        return;
    }
    case 0x8d0:
    {
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 scale = r_u16(object + 0x68);
        uint32 speed = r_u16(config + 0x24);
        scale += speed;
        w_u16(object + 0x68, scale);
        if ((sint16)scale >= 0x1000)
        {
            w_u16(object + 0x6c, 0x1000);
            w_u16(object + 0x6a, 0x1000);
            w_u16(object + 0x68, 0x1000);
            w_u16(state + 0x26, 0);
            w_u32(object + 0x150, 2);
            w_u16(object + 0x6e, 0);
            w_u16(object + 0x5e, 0);
            return;
        }
        scale = r_u16(object + 0x6a);
        speed = r_u16(config + 0x24);
        w_u16(object + 0x6a, scale + speed);
        scale = r_u16(object + 0x6c);
        speed = r_u16(config + 0x24);
        w_u16(object + 0x6c, scale + speed);
        uint32 yaw = r_u16(object + 0x64);
        uint32 rotation = r_u16(config + 0x22);
        w_u16(object + 0x64, yaw + rotation);
        return;
    }
    case 0x960:
    {
        uint32 distance = 0;
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 coordinate = r_u16(player_position + axis * 2);
            uint32 origin = r_u16(state + 0xc + axis * 2);
            uint32 delta = coordinate - origin;
            uint32 signed_delta = (uint32)(sint32)(sint16)delta;
            distance += signed_delta * signed_delta;
            w_u16(frame + 0x10 + axis * 2, delta);
        }
        uint32 radius = r_u32(state + 4);
        if ((sint32)distance >= (sint32)radius)
        {
            w_u32(object + 0x150, 5);
            w_u16(object + 0x6e, 4);
            w_u16(object + 0x5e, 0);
            return;
        }
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 player_x = r_u16(player_position);
        uint32 object_x = r_u16(object + 0x58);
        uint32 delta_x = player_x - object_x;
        w_u16(frame + 0x10, delta_x);
        uint32 player_y = r_u16(player_position + 2);
        uint32 object_y = r_u16(object + 0x5a);
        uint32 delta_y = player_y - object_y;
        w_u16(frame + 0x12, delta_y);
        uint32 player_z = r_u16(player_position + 4);
        uint32 object_z = r_u16(object + 0x5c);
        w_u16(frame + 0x14, player_z - object_z);
        uint32 angle = G3Function8002F240((uint32)(sint32)(sint16)delta_y, (uint32)(sint32)(sint16)delta_x, frame);
        uint32 yaw = (uint32)r_s16(object + 0x64);
        sint32 difference = (sint16)G3Function80038B48(yaw, (uint32)(sint32)(sint16)(angle + 0x400));
        if (difference < 0)
        {
            sint32 threshold = r_s16(config + 2);
            if (difference < -threshold)
            {
                yaw = r_u16(object + 0x64);
                uint32 speed = r_u16(config);
                w_u16(object + 0x64, yaw - speed);
                return;
            }
        }
        else
        {
            sint32 threshold = r_s16(config + 2);
            if (threshold < difference)
            {
                yaw = r_u16(object + 0x64);
                uint32 speed = r_u16(config);
                w_u16(object + 0x64, yaw + speed);
                return;
            }
        }
        sint32 timer = r_s16(state + 0x28);
        uint32 timer_half = r_u16(state + 0x28);
        bool attack = timer <= 0;
        if (!attack)
        {
            --timer_half;
            w_u16(state + 0x28, timer_half);
            attack = (timer_half & 0xffff) == 0;
        }
        if (attack)
        {
            if (r_s16(state + 0x26) == 0)
            {
                w_u32(object + 0x150, 3);
                w_u16(object + 0x6e, 1);
                w_u16(object + 0x5e, 0);
            }
            else
            {
                w_u32(object + 0x150, 4);
                w_u16(object + 0x6e, 2);
                w_u16(object + 0x5e, 0);
                w_u16(state + 0x2a, 0);
            }
        }
        yaw = r_u16(object + 0x64);
        w_u16(object + 0x64, yaw + (uint32)difference);
        return;
    }
    case 0xb64:
    {
        uint32 flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags & ~0x10u);
        G3Function80029F0C(object, 0xffffffff, frame);
        if ((r_u32(object + 0x14) & 0x10) != 0)
        {
            w_u32(object + 0x150, 4);
            w_u16(object + 0x6e, 2);
            w_u16(object + 0x5e, 0);
            w_u16(state + 0x2a, 0);
            w_u16(state + 0x26, 1);
        }
        return;
    }
    case 0xbb4:
    {
        uint32 flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags & ~0x10u);
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 fired = r_u16(state + 0x2a);
        bool first_shot = false;
        if ((fired & 1) == 0)
        {
            sint32 animation_frame = r_s16(object + 0x5e);
            sint32 fire_frame = r_s16(config + 4);
            if (animation_frame == fire_frame)
            {
                uint32 bone = (uint32)r_s16(config + 6);
                G3CowboyShoot(object, game, config + 8, bone, frame);
                fired = r_u16(state + 0x2a);
                w_u16(state + 0x2a, fired | 1);
                first_shot = true;
            }
        }
        if (!first_shot)
        {
            fired = r_u16(state + 0x2a);
            if ((fired & 2) == 0)
            {
                sint32 animation_frame = r_s16(object + 0x5e);
                sint32 fire_frame = r_s16(config + 0xe);
                if (animation_frame == fire_frame)
                {
                    uint32 bone = (uint32)r_s16(config + 0x10);
                    G3CowboyShoot(object, game, config + 0x12, bone, frame);
                    fired = r_u16(state + 0x2a);
                    w_u16(state + 0x2a, fired | 2);
                }
            }
        }
        if ((r_u32(object + 0x14) & 0x10) != 0)
        {
            uint32 timer = r_u16(config + 0x1c);
            w_u16(state + 0x28, timer);
            w_u32(object + 0x150, 2);
            w_u16(object + 0x6e, 3);
            w_u16(object + 0x5e, 0);
        }
        return;
    }
    case 0xc94:
    {
        uint32 flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags & ~0x10u);
        G3Function80029F0C(object, 0xffffffff, frame);
        if ((r_u32(object + 0x14) & 0x10) != 0)
        {
            w_u32(object + 0x150, 6);
            w_u16(object + 0x6e, 0);
            w_u16(object + 0x5e, 0);
            w_u16(state + 0x26, 0);
        }
        return;
    }
    case 0xcd8:
    {
        uint32 scale = r_u16(object + 0x68);
        uint32 speed = r_u16(config + 0x24);
        scale -= speed;
        w_u16(object + 0x68, scale);
        if ((sint16)scale <= 0)
        {
            uint32 flags = r_u32(object + 0x10);
            w_u32(object + 0x150, 0);
            w_u32(object + 0x10, flags | 0x800);
            uint32 random = G3SdkRandom80027FA8();
            uint32 selected = r_u16(state + 0x24);
            selected += 1 + (random & 1);
            w_u16(state + 0x24, selected);
            if ((sint16)selected >= 3)
                w_u16(state + 0x24, selected - 3);
            return;
        }
        scale = r_u16(object + 0x6a);
        speed = r_u16(config + 0x24);
        w_u16(object + 0x6a, scale - speed);
        scale = r_u16(object + 0x6c);
        speed = r_u16(config + 0x24);
        w_u16(object + 0x6c, scale - speed);
        uint32 yaw = r_u16(object + 0x64);
        uint32 rotation = r_u16(config + 0x22);
        w_u16(object + 0x64, yaw - rotation);
        return;
    }
    case 0xd7c:
        w_u32(object + 0x150, 6);
        return;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved cowboy state target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}
