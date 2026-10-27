#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void G3TankAlign(uint32 object, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000006ec, "G3L_27");
    uint32 frame = guest_stack - 0x50;
    uint32 game = r_u32(0x80097048);
    {
        uint32 source = module_base + 0x1840;
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
    if (difference < 65)
    {
        game = r_u32(0x80097048);
        player = r_u32(game + 0x50);
        normal = player + 0xb4;
    }
    if (r_s16(state + 0x26) == 0)
    {
        bool changed = r_s16(state + 0x16) != r_s16(normal);
        if (!changed) changed = r_s16(state + 0x18) != r_s16(normal + 2);
        if (!changed) changed = r_s16(state + 0x1a) != r_s16(normal + 4);
        if (changed)
        {
            w_u16(state + 0x26, 1);
            {
                uint32 source = state + 0x16;
                uint32 destination = state + 0xe;
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
                uint32 destination = state + 0x16;
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
            uint32 angle = G3Function8002F540(state + 0x16, state + 0xe, frame);
            uint32 y = (uint32)r_s16(state + 0x18);
            uint32 old_z = (uint32)r_s16(state + 0x12);
            uint32 yz = y * old_z;
            uint32 z_axis = (uint32)r_s16(state + 0x1a);
            uint32 old_y = (uint32)r_s16(state + 0x10);
            uint32 zy = z_axis * old_y;
            uint32 x = (uint32)r_s16(state + 0x16);
            old_z = (uint32)r_s16(state + 0x12);
            uint32 xz = x * old_z;
            z_axis = (uint32)r_s16(state + 0x1a);
            uint32 old_x = (uint32)r_s16(state + 0xe);
            uint32 zx = z_axis * old_x;
            x = (uint32)r_s16(state + 0x16);
            old_y = (uint32)r_s16(state + 0x10);
            uint32 xy = x * old_y;
            y = (uint32)r_s16(state + 0x18);
            old_x = (uint32)r_s16(state + 0xe);
            uint32 yx = y * old_x;
            w_u16(state + 0x28, angle);
            w_u16(state + 0x1e, 0u - (uint32)((sint32)(yz - zy) >> 12));
            w_u16(state + 0x20, (uint32)((sint32)(xz - zx) >> 12));
            w_u16(state + 0x22, 0u - (uint32)((sint32)(xy - yx) >> 12));
        }
        if (r_s16(state + 0x26) == 0)
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
    uint32 progress = r_u16(state + 0x26);
    uint32 step = r_u16(settings + 0x42);
    progress += step;
    w_u16(state + 0x26, progress);
    sint32 limit = r_s16(state + 0x28);
    uint32 limit_bits = r_u16(state + 0x28);
    if ((sint16)progress >= limit) w_u16(state + 0x26, limit_bits);
    w_u16(frame + 0x14, r_u16(state + 0xe));
    w_u16(frame + 0x1a, r_u16(state + 0x10));
    w_u16(frame + 0x20, r_u16(state + 0x12));
    uint32 angle = (uint32)r_s16(state + 0x26);
    G3Function8002F630(state + 0x1e, frame + 0x10, angle, frame);
    uint32 y = (uint32)r_s16(frame + 0x1a);
    uint32 z_axis = (uint32)r_s16(frame + 0x20);
    angle = G3Function8002F0C4(y, z_axis, frame);
    w_u16(object + 0x60, 0u - angle);
    uint32 x = (uint32)r_s16(frame + 0x14);
    uint32 root = G3Function8002F368((uint32)((sint32)(0x1000000u - x * x) >> 12), frame);
    x = (uint32)r_s16(frame + 0x14);
    angle = G3Function8002F0C4(x, root, frame);
    w_u16(object + 0x62, angle);
    sint32 current = r_s16(state + 0x26);
    limit = r_s16(state + 0x28);
    if (current == limit) w_u16(state + 0x26, 0);
}

void G3TankMove(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000a18, "G3L_27");
    uint32 frame = guest_stack - 0x60;
    uint32 player = r_u32(0x8009efc4);
    uint32 player_state = r_u32(player + 0x24);
    uint32 input = G3TankSteer(game, object, state, settings, frame);
    w_u16(player_state + 0xbc, input);
    G3TankSpeed((uint32)(sint32)(sint16)input, state, settings);
    if (r_u8(state + 0x2a) != 0)
    {
        G3FillMemory80086094(frame + 0x10, 0, 8);
        uint32 table = r_u32(settings + 0x3c);
        sint32 speed = r_s16(state + 6);
        sint32 threshold = r_s16(table);
        uint32 index = 0;
        while (speed >= threshold)
        {
            ++index;
            uint32 offset = (uint32)((sint32)(index << 16) >> 15);
            threshold = r_s16(table + offset);
        }
        w_u16(state + 8, index);
        table = r_u32(settings + 0x38);
        uint32 offset = (uint32)((sint32)(index << 16) >> 15);
        uint32 stride = r_u16(table + offset);
        w_u16(frame + 0x12, 0u - stride);
        if (r_u8(state + 0x2d) != 0) w_u16(frame + 0x12, stride);
        G3Function8002F718(frame + 0x20);
        G3Function8005BB6C((uint32)r_s16(state + 2), frame + 0x20, frame);
        G3Function8005BA5C((uint32)r_s16(object + 0x62), frame + 0x20, frame);
        G3Function8005B94C((uint32)r_s16(object + 0x60), frame + 0x20, frame);
        ApplyMatrixSV((MATRIX *)psx_addr(frame + 0x20, 20), (SVECTOR *)psx_addr(frame + 0x10, 8), (SVECTOR *)psx_addr(frame + 0x18, 6));
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 coordinate = r_u16(object + 0x58 + axis * 2);
            uint32 change = r_u16(frame + 0x18 + axis * 2);
            w_u16(object + 0x58 + axis * 2, coordinate + change);
        }
        for (uint32 axis = 0; axis != 3; ++axis)
            w_u16(player_state + 0x26e + axis * 2, r_u16(frame + 0x18 + axis * 2));
    }
    G3TankAlign(object, state, settings, module_base, frame);
    G3TankAnimate(object, game, state, settings, module_base, frame);
}

void G3TankFooting(uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000bf4, "G3L_27");
    uint32 frame = guest_stack - 0x68;
    uint32 player = r_u32(0x8009efc4);
    sint32 velocity = r_s16(state + 0xa);
    uint32 player_state = r_u32(player + 0x24);
    w_u8(state + 0x2a, 1);
    if (velocity != 0)
    {
        uint32 heading = (uint32)r_s16(state + 2) - 0x400;
        uint32 sine = G3Function8005B470(heading);
        player = r_u32(0x8009efc4);
        uint32 coordinate = r_u16(player + 0x58);
        w_u16(frame + 0x40, coordinate + (uint32)((sint32)(sine * 96) >> 12));
        heading = (uint32)r_s16(state + 2) - 0x400;
        uint32 cosine = G3Function8005B590(heading);
        player = r_u32(0x8009efc4);
        coordinate = r_u16(player + 0x5a);
        w_u16(frame + 0x42, coordinate + (uint32)((sint32)(cosine * 96) >> 12));
        w_u16(frame + 0x44, r_u16(player + 0x5c));
        {
            uint32 source = frame + 0x40;
            uint32 destination = frame + 0x48;
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
        w_u32(frame + 0x34, 31);
        w_u32(frame + 0x24, frame + 0x40);
        w_u32(frame + 0x28, frame + 0x48);
        uint32 bottom = r_u16(frame + 0x44);
        uint32 top = r_u16(frame + 0x4c);
        w_u32(frame + 0x38, player);
        w_u16(frame + 0x4c, top + 128);
        w_u16(frame + 0x44, bottom - 256);
        uint32 flags = r_u32(player + 0x10);
        uint32 global_game = r_u32(0x80097048);
        w_u32(player + 0x10, flags | 0x40);
        G3Function80061CA4(frame + 0x10, global_game, frame);
        player = r_u32(0x8009efc4);
        w_u32(player + 0x10, r_u32(player + 0x10) & 0xffffffbf);
        if (r_u32(frame + 0x10) == 3 && r_s16(frame + 0x20) < 3800)
        {
            uint32 face = r_u32(frame + 0x14);
            uint32 level = r_u32(game + 0x48);
            uint32 material = r_u16(face + 6);
            uint32 table = r_u32(level + 0xf0);
            if ((r_u32(table + ((material & 0x1e0) >> 1)) & 8) == 0) w_u8(state + 0x2a, 0);
        }
    }
    uint32 face = r_u32(player_state + 0x140);
    bool blocked = false;
    if (face != 0)
    {
        if (r_s16(player_state + 0x14c) >= 3801) blocked = true;
        else
        {
            uint32 material = r_u16(face + 6);
            uint32 level = r_u32(game + 0x48);
            uint32 table = r_u32(level + 0xf0);
            blocked = (r_u32(table + ((material & 0x1e0) >> 1)) & 8) != 0;
        }
    }
    w_u8(state + 0x2b, blocked ? 1 : 0);
}

void G3TankFollowPlatform(uint32 object, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000de4, "G3L_27");
    uint32 frame = guest_stack - 0x20;
    if (r_u8(state + 0x2b) != 0) return;
    uint32 player = r_u32(0x8009efc4);
    sint32 z = r_s16(object + 0x5c);
    sint32 floor = r_s16(player + 0xe0);
    sint32 difference = z - floor;
    if (difference < 0) difference = -difference;
    if (difference >= 5) return;
    player = r_u32(0x8009efc4);
    uint32 scale = r_u8(settings + 0x46);
    G3Function80036278(player + 0xb4, frame + 0x10, scale, frame);
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        uint32 coordinate = r_u16(object + 0x58 + axis * 2);
        uint32 change = r_u16(frame + 0x10 + axis * 2);
        w_u16(object + 0x58 + axis * 2, coordinate + change);
    }
}

void G3TankFire(uint32 object, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000ebc, "G3L_27");
    uint32 frame = guest_stack - 0x38;
    uint32 matrix = r_u32(object + 0x30);
    if (matrix != 0)
    {
        ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(settings + 4, 8), (SVECTOR *)psx_addr(frame + 0x18, 6));
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 transformed = r_u16(frame + 0x18 + axis * 2);
            uint32 origin = r_u16(object + 0x58 + axis * 2);
            w_u16(frame + 0x10 + axis * 2, transformed + origin);
        }
    }
    uint32 resource = G3Function8003AA48(settings + 0xc);
    if (resource != 0)
    {
        uint32 shot = G3Function8002A454(object, resource, frame);
        if (shot != 0)
        {
            uint32 heading = r_u16(object + 0x64);
            uint32 turn = r_u16(settings + 0x2e);
            {
                uint32 source = frame + 0x10;
                uint32 destination = shot + 0x58;
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
            {
                uint32 source = frame + 0x10;
                uint32 destination = shot + 0x70;
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
            uint32 zero_sine = G3Function8005B470(0);
            heading = (uint32)(sint32)(sint16)(heading + turn);
            uint32 sine = G3Function8005B470(heading);
            uint32 speed = (uint32)r_s16(settings + 0x2c);
            uint32 lateral = (uint32)((sint32)(speed * zero_sine) >> 12);
            w_u32(shot + 0x104, (uint32)((sint32)(lateral * sine) >> 12));
            zero_sine = G3Function8005B470(0);
            uint32 cosine = G3Function8005B590(heading);
            speed = (uint32)r_s16(settings + 0x2c);
            lateral = (uint32)((sint32)(speed * zero_sine) >> 12);
            w_u32(shot + 0x108, (uint32)((sint32)(lateral * cosine) >> 12));
            cosine = G3Function8005B590(0);
            speed = (uint32)r_s16(settings + 0x2c);
            uint32 vertical = (uint32)((sint32)(speed * cosine) >> 12);
            w_u32(shot + 0x118, 0);
            w_u32(shot + 0x124, 0);
            w_u32(shot + 0x128, 500);
            w_u32(shot + 0x10c, vertical);
            resource = G3Function8003AA48(settings + 0x1c);
            w_u32(shot + 0x12c, resource);
            w_u32(shot + 0x130, (uint32)r_s16(settings + 0x24));
        }
    }
    resource = G3Function8003AA48(settings + 0x14);
    if (resource != 0)
    {
        uint32 models = r_u32(resource + 0xc);
        uint32 pool = r_u32(0x800a27fc);
        uint32 model = r_u32(models);
        uint32 effect = G3Function800444E4(frame + 0x10, model, pool, 0xffffffff, frame);
        if (effect != 0) G3Function80044878(effect, settings + 0x26, 0);
    }
}

void G3TankRide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001174, "G3L_27");
    uint32 frame = guest_stack - 0x30;
    uint32 player = r_u32(game + 0x50);
    uint32 flags = r_u32(player + 0x10);
    uint32 controls = (flags & 0x100) == 0 ? game + 0x70 : 0x800a0d28;
    uint32 player_state = r_u32(player + 0x24);
    G3TankFooting(game, state, frame);
    G3TankMove(object, game, state, settings, module_base, frame);
    G3TankGround(object, state, settings);
    G3TankFollowPlatform(object, state, settings, frame);
    bool cooling = r_s16(state + 0xc) > 0;
    if (!cooling)
    {
        uint32 animations = r_u32(settings);
        uint32 animation = r_u16(object + 0x6e);
        cooling = animation == r_u8(animations + 1);
    }
    if (cooling) w_u16(state + 0xc, r_u16(state + 0xc) - 1);
    else if ((r_u32(controls + 4) & 0x20) != 0)
    {
        w_u16(state + 0xc, r_u16(settings + 0xa));
        G3TankIdle(object, state, settings);
    }
    uint32 animations = r_u32(settings);
    uint32 animation = r_u16(object + 0x6e);
    if (animation == r_u8(animations + 1))
    {
        sint32 current = r_s16(object + 0x5e);
        sint32 firing_frame = r_s16(settings + 0x44);
        if (current >= firing_frame && r_u8(state + 0x2c) == 0)
        {
            G3TankFire(object, settings, frame);
            w_u8(state + 0x2c, 1);
        }
    }
    for (uint32 axis = 0; axis != 3; ++axis)
        w_u16(player + 0x58 + axis * 2, r_u16(object + 0x58 + axis * 2));
    if (r_u16(game + 0x68b4) - 11u < 5) return;
    bool exit = (r_u32(controls + 4) & 0x40) != 0;
    if (!exit) exit = r_u8(state + 0x2f) != 0;
    if (!exit) return;
    w_u16(state, 2);
    w_u8(state + 0x2e, 15);
    w_u16(state + 6, 0);
    animations = r_u32(settings);
    animation = r_u8(animations + 2);
    uint32 current_player = r_u32(0x8009efc4);
    w_u16(object + 0x5e, 0);
    w_u16(object + 0x6e, animation);
    w_u16(current_player + 0x60, 0);
    w_u16(current_player + 0x62, 0);
    current_player = r_u32(game + 0x50);
    w_u32(current_player + 0x10, r_u32(current_player + 0x10) & 0xfffff7ff);
    uint32 heading = (uint32)r_s16(player + 0x64) - 0x400;
    uint32 sine = G3Function8005B470(heading);
    uint32 component = (uint32)((sint32)sine >> 5);
    uint32 half = (uint32)((sint32)(component + (sine >> 31)) >> 1);
    heading = (uint32)r_s16(player + 0x64) - 0x400;
    w_u32(player + 0x104, component + half);
    uint32 cosine = G3Function8005B590(heading);
    component = (uint32)((sint32)cosine >> 5);
    half = (uint32)((sint32)(component + (cosine >> 31)) >> 1);
    w_u32(player + 0x108, component + half);
    if (r_u8(state + 0x2f) != 0)
    {
        uint32 x = r_u32(player + 0x104);
        uint32 y = r_u32(player + 0x108);
        half = (uint32)((sint32)(x + (x >> 31)) >> 1);
        x += half;
        half = (uint32)((sint32)(y + (y >> 31)) >> 1);
        y += half;
        w_u32(player + 0x104, x);
        w_u32(player + 0x108, y);
    }
    w_u32(player + 0x10c, 150);
    w_u32(player_state + 0x1fc, 0);
    uint32 vertical = r_u32(player + 0x10c);
    uint32 z = r_u16(player + 0x5c);
    w_u16(player + 0x5c, z + vertical * 2);
    uint32 player_flags = r_u16(player_state + 0xce);
    w_u16(player_state + 0xce, player_flags & 0xfffe);
    w_u8(state + 0x2f, 0);
    G3Function80081B7C(0, 0x5d50, frame);
}

void G3TankUpdate(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001478, "G3L_27");
    uint32 frame = guest_stack - 0x20;
    w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
    sint32 mode = r_s16(state);
    if (mode != 1)
    {
        if (mode == 0 || mode == 3) w_u8(state + 0x2f, 0);
        return;
    }
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
    uint32 active_player = r_u32(game + 0x50);
    w_u32(active_player + 0x10, r_u32(active_player + 0x10) | 0x800);
    G3TankRide(object, game, state, settings, module_base, frame);
    player = r_u32(0x8009efc4);
    uint32 heading = r_u16(object + 0x64);
    uint32 player_state = r_u32(player + 0x24);
    w_u16(player_state + 0xc8, heading);
    w_u16(player_state + 0xc6, heading);
    w_u16(player + 0x64, heading);
}

void G3TankCollide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000015ac, "G3L_27");
    uint32 frame = guest_stack - 0x28;
    uint32 contact = r_u32(object + 0xb0);
    uint32 player = r_u32(0x8009efc4);
    sint32 mode = r_s16(state);
    uint32 other = r_u32(contact + 0x14);
    uint32 player_state = r_u32(player + 0x24);
    uint32 delta = contact + 0x28;
    bool skip_player_check = mode == 0 && other != player;
    if (mode == 0 && other == player && r_u32(other + 0x154) != 0x4000000)
    {
        sint32 kind = r_s16(contact + 6);
        if (kind == 1)
        {
            uint32 descriptor = r_u32(contact + 0xc);
            if (r_u8(descriptor + 4) == 0)
            {
                w_u16(state, kind);
                w_u16(state + 2, r_u16(object + 0x64));
                uint32 animations = r_u32(settings);
                uint32 animation = r_u8(animations + 2);
                w_u16(object + 0x5e, 0);
                w_u16(object + 0x6e, animation);
                G3Function8006F50C(object, game, contact, settings, frame);
                w_u8(player_state + 0x200, 0);
                w_u8(state + 0x2f, 0);
                return;
            }
        }
    }
    if (!skip_player_check && other == r_u32(0x8009efc4)) return;
    if (other == 0) return;
    if (other == r_u32(player_state + 0x17c)) return;
    uint32 descriptor = r_u32(other + 0x1c);
    if (descriptor != 0)
    {
        uint32 name = r_u32(descriptor + 0x24);
        uint32 expected = r_u32(module_base + 0x1848);
        uint32 actual = r_u32(name);
        if (actual == expected)
        {
            actual = r_u32(name + 4);
            expected = r_u32(module_base + 0x184c);
            if (actual == expected) return;
        }
    }
    sint32 kind = r_s16(contact + 6);
    descriptor = r_u32(contact + 0xc);
    bool translate = false;
    if (kind == 1) translate = (r_u32(descriptor) & 0x2000) != 0;
    if (!translate && kind == 5) translate = (r_u16(descriptor) & 0x2000) != 0;
    if (!translate && kind == 2) translate = (r_u16(descriptor + 6) & 15) != 8;
    if (!translate && kind == 3) translate = true;
    if (translate)
    {
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 coordinate = r_u16(object + 0x58 + axis * 2);
            uint32 change = r_u16(delta + axis * 2);
            w_u16(object + 0x58 + axis * 2, coordinate + change);
        }
        G3Function8005F3A8(object, delta);
        if (r_s16(state) != 0)
        {
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
    }
    uint32 target = r_u32(other + 0xec);
    if (target != 0)
    {
        G3DispatchStateEvent(target, other, game, 0x7e7, 0, frame);
        target = r_u32(other + 0xec);
        if (target != 0)
        {
            player = r_u32(0x8009efc4);
            G3DispatchStateEvent(target, other, game, 0x7da, player, frame);
        }
    }
}

uint32 G3TeeterEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_31");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3TeeterInitialize(object, module_base);
        return 1;
    case 2:
        G3TeeterUpdate(object, game, frame);
        return 1;
    case 3:
        G3TeeterCollide(object, game, frame);
        return 1;
    default:
        return 0;
    }
}

void G3TeeterInitialize(uint32 object, uint32 module_base)
{
    FUNCTION_MARKER(0x00000088, "G3L_31");
    if (r_u32(object + 0x28) == 0)
        w_u32(object + 0x28, module_base + 0x410);
    w_u32(object + 0x144, 0);
    w_u32(object + 0x148, 0);
}

void G3TeeterUpdate(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000ac, "G3L_31");
    uint32 frame = guest_stack - 0x38;
    uint32 angle = (uint32)r_s16(object + 0x64);
    uint32 player = r_u32(game + 0x50);
    uint32 data = r_u32(object + 0x28);
    uint32 player_state = r_u32(player + 0x24);
    uint32 cosine = G3Function8005B590(0 - angle);
    angle = (uint32)r_s16(object + 0x64);
    uint32 sine = G3Function8005B470(0 - angle);
    uint32 y = (uint32)r_s16(object + 0x62) * sine;
    uint32 x = (uint32)r_s16(object + 0x60) * cosine;
    uint32 sum = x + y;
    if ((sint32)sum < 0)
        sum += 0xfff;
    sint32 tilt = (sint32)sum >> 12;
    uint32 restoring = (0 - (uint32)tilt) << 5;
    uint32 velocity = r_u32(object + 0x128);
    sint32 mode = r_s16(data);
    uint32 response = restoring - (uint32)((sint32)velocity >> 5);
    response = (uint32)((sint32)response >> (mode == 0 ? 2 : 1));
    if (r_u32(object + 0x130) != 0)
        G3Function8003C614(object, game, frame);
    uint32 force = r_u32(object + 0x12c);
    uint32 old_velocity = r_u32(object + 0x128);
    velocity = old_velocity + response + force;
    w_u32(object + 0x128, velocity);
    uint32 limit = (uint32)r_s16(data + 4);
    uint32 upper = limit << 12;
    if ((sint32)upper < (sint32)velocity)
        w_u32(object + 0x128, upper);
    else
    {
        uint32 lower = (0 - limit) << 12;
        if ((sint32)velocity < (sint32)lower)
            w_u32(object + 0x128, lower);
    }
    uint32 position = r_u32(object + 0x138);
    velocity = r_u32(object + 0x128);
    position += velocity;
    w_u32(object + 0x138, position);
    limit = (uint32)r_s16(data + 2);
    upper = limit << 12;
    if ((sint32)upper < (sint32)position)
        w_u32(object + 0x138, upper);
    else
    {
        uint32 lower = (0 - limit) << 12;
        if ((sint32)position < (sint32)lower)
            w_u32(object + 0x138, lower);
    }
    uint32 component = (uint32)((sint32)(r_u32(object + 0x138) << 4) >> 16);
    x = component * cosine;
    if ((sint32)x < 0)
        x += 0xfff;
    component = (uint32)((sint32)(r_u32(object + 0x138) << 4) >> 16);
    y = component * sine;
    w_u16(object + 0x60, (uint32)((sint32)x >> 12));
    if ((sint32)y < 0)
        y += 0xfff;
    uint32 cooldown = r_u32(object + 0x148);
    w_u16(object + 0x62, (uint32)((sint32)y >> 12));
    if (cooldown == 0)
    {
        uint32 idle = r_u32(object + 0x144);
        if (idle != 0)
        {
            --idle;
            w_u32(object + 0x144, idle);
            if (idle == 0)
            {
                uint32 flags = r_u32(object + 0x14);
                w_u32(object + 0x144, 0);
                w_u32(object + 0x148, 10);
                w_u32(object + 0x14, flags | 0x1000);
            }
        }
        if (old_velocity != r_u32(object + 0x128))
            w_u32(object + 0x144, 5);
    }
    else
        w_u32(object + 0x148, cooldown - 1);
    sint32 threshold = r_s16(data + 6);
    if (threshold < tilt)
        w_u16(player_state + 0x128, r_u16(player_state + 0x128) + 0x20);
    else if (tilt < -threshold)
        w_u16(player_state + 0x128, r_u16(player_state + 0x128) - 0x20);
    w_u32(object + 0x130, 0);
    w_u32(object + 0x12c, 0);
}

void G3TeeterCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000318, "G3L_31");
    uint32 frame = guest_stack - 0x60;
    uint32 contact = r_u32(object + 0xb0);
    if (r_s16(contact + 6) != 4)
        return;
    uint32 other = r_u32(contact + 0x14);
    uint32 player = r_u32(game + 0x50);
    if (other != player)
        return;
    uint32 matrix = r_u32(object + 0x30);
    if (matrix == 0)
        return;
    G3Function800313E0(frame + 0x20, matrix, frame);
    uint32 first = r_u32(frame + 0x20);
    uint32 second = r_u32(frame + 0x24);
    xport_gte_write_control(0, first);
    xport_gte_write_control(1, second);
    uint32 third = r_u32(frame + 0x28);
    uint32 fourth = r_u32(frame + 0x2c);
    uint32 fifth = r_u32(frame + 0x30);
    xport_gte_write_control(2, third);
    xport_gte_write_control(3, fourth);
    xport_gte_write_control(4, fifth);
    uint32 tx = r_u32(frame + 0x34);
    uint32 ty = r_u32(frame + 0x38);
    xport_gte_write_control(5, tx);
    uint32 tz = r_u32(frame + 0x3c);
    xport_gte_write_control(6, ty);
    xport_gte_write_control(7, tz);
    // SDK output contract remains pending the documented RotTrans correction
    RotTrans((SVECTOR *)psx_addr(contact + 0x18, 8), (VECTOR *)psx_addr(frame + 0x10, 12), (sint32 *)psx_addr(frame + 0x40, 4));
    uint32 force = r_u32(frame + 0x10);
    w_u32(object + 0x12c, force << 2);
    w_u32(object + 0x134, r_u32(frame + 0x10));
    w_u32(object + 0x130, r_u32(contact + 0x14));
    G3Function8003C864(object, game, frame);
}

uint32 G3TformrEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_81");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3TformrInitialize(object, module_base);
        return 1;
    case 2:
        G3TformrUpdate(object, module_base, frame);
        return 1;
    case 3:
        G3TformrCollide(object, game, frame);
        return 1;
    case 8:
        G3TformrHit(object, frame);
        return 1;
    default:
        return 0;
    }
}

void G3TformrInitialize(uint32 object, uint32 module_base)
{
    FUNCTION_MARKER(0x000000a0, "G3L_81");
    uint32 state = object + 0x104;
    uint32 custom = r_u32(object + 0x28);
    w_u16(state + 0x22, 0);
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
        uint32 destination = object + 0x120;
        high_address = destination + 3;
        high_lane = high_address & 3;
        lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
        w_u16(destination + 4, last);
    }
    uint32 projectile_resource = G3Function8003AA48(module_base + 0xdb4);
    w_u32(state + 0x18, projectile_resource);
    for (uint32 box = 0; box != 2; ++box)
    {
        uint32 begin = box == 0 ? 0xc : 0;
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            for (uint32 side = 0; side != 2; ++side)
            {
                uint32 value;
                if (custom != 0)
                    value = r_u16(custom + begin + axis * 2 + side * 6);
                else
                {
                    value = r_u16(object + 0x58 + axis * 2);
                    value += side == 0 ? 0xfffff63c : 0x9c4;
                }
                w_u16(state + begin + axis * 2 + side * 6, value);
            }
        }
    }
    uint32 lower_z = r_u16(state + 4);
    w_u16(state + 4, lower_z - 0x9c4);
    w_u32(object + 0x150, 1);
    G3TformrSetAnimation(object, 0, 0);
}

void G3TformrUpdate(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000002dc, "G3L_81");
    uint32 frame = guest_stack - 0x18;
    if (r_u32(object + 0x150) == 1)
        G3TformrMove(object, module_base, frame);
    G3TformrClamp(object);
}

uint32 G3TformrUsesOuterBounds(uint32 object)
{
    FUNCTION_MARKER(0x00000808, "G3L_81");
    uint32 animation = r_u16(object + 0x6e);
    return animation == 0 || animation == 7 ? 1 : 0;
}

void G3TformrTurnToward(uint32 object, uint32 target, uint32 step)
{
    FUNCTION_MARKER(0x00000830, "G3L_81");
    sint32 yaw = r_s16(object + 0x64);
    sint32 target_half = (sint16)target;
    bool wrapped_up = false;
    if (target_half < yaw && yaw - target_half > 0x800)
    {
        target += 0x1000;
        wrapped_up = true;
    }
    if (!wrapped_up)
    {
        yaw = r_s16(object + 0x64);
        target_half = (sint16)target;
        if (yaw < target_half && target_half - yaw > 0x800)
            target -= 0x1000;
    }
    sint32 signed_step = (sint16)step;
    target_half = (sint16)target;
    yaw = r_s16(object + 0x64);
    uint32 yaw_half = r_u16(object + 0x64);
    if ((sint32)((uint32)yaw + (uint32)signed_step) < target_half)
        w_u16(object + 0x64, yaw_half + step);
    else if (target_half < (sint32)((uint32)yaw - (uint32)signed_step))
        w_u16(object + 0x64, yaw_half - step);
    else
        w_u16(object + 0x64, target);
    sint32 normalized = r_s16(object + 0x64) + 0x1000;
    w_u16(object + 0x64, (uint32)(normalized % 4096));
}

uint32 G3TformrHeadingTo(uint32 object, uint32 point, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000900, "G3L_81");
    uint32 frame = guest_stack - 0x18;
    uint32 x = (uint32)r_s16(object + 0x58);
    uint32 y = (uint32)r_s16(object + 0x5a);
    uint32 target_y = (uint32)r_s16(point + 2);
    uint32 target_x = (uint32)r_s16(point);
    uint32 heading = G3Function8002F240(y - target_y, x - target_x, frame);
    uint32 shifted = (heading << 16) + 0x0c000000;
    return (uint32)((sint32)shifted >> 16);
}

void G3TformrClamp(uint32 object)
{
    FUNCTION_MARKER(0x00000940, "G3L_81");
    uint32 bounds = object + 0x104;
    if (G3TformrUsesOuterBounds(object) != 0)
        bounds = object + 0x110;
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        sint32 coordinate = r_s16(object + 0x58 + axis * 2);
        sint32 lower = r_s16(bounds + axis * 2);
        uint32 lower_half = r_u16(bounds + axis * 2);
        if (coordinate < lower)
            w_u16(object + 0x58 + axis * 2, lower_half);
        coordinate = r_s16(object + 0x58 + axis * 2);
        sint32 upper = r_s16(bounds + axis * 2 + 6);
        uint32 upper_half = r_u16(bounds + axis * 2 + 6);
        if (upper < coordinate)
            w_u16(object + 0x58 + axis * 2, upper_half);
    }
}

void G3TformrCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000a38, "G3L_81");
    uint32 frame = guest_stack - 0x18;
    uint32 contact = r_u32(object + 0xb0);
    uint32 player = r_u32(0x8009efc4);
    uint32 other = r_u32(contact + 0x14);
    if (other != player)
        return;
    uint32 animation = r_u16(object + 0x6e);
    if (animation < 8)
        G3TformrSetAnimation(object, 8, 0);
    else if (animation == 10)
    {
        if (r_u32(contact + 4) != 0x10001)
            return;
        uint32 entry = r_u32(contact + 0xc);
        if (r_u8(entry + 4) == 0)
            G3Function80074044(object, game, frame);
    }
}

void G3TformrHit(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000acc, "G3L_81");
    uint32 frame = guest_stack - 0x18;
    if (r_u16(object + 0x6e) == 10)
        G3TformrSetAnimation(object, 11, 0);
    else
        G3Function80038798(object, 0, 0, frame);
}

uint32 G3TformrContains(uint32 object, uint32 point, uint32 outer)
{
    FUNCTION_MARKER(0x00000b10, "G3L_81");
    uint32 bounds = object + (outer == 0 ? 0x104 : 0x110);
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        sint32 coordinate = r_s16(point + axis * 2);
        sint32 lower = r_s16(bounds + axis * 2);
        if (coordinate <= lower)
            return 0;
        sint32 upper = r_s16(bounds + axis * 2 + 6);
        if (coordinate >= upper)
            return 0;
    }
    return 1;
}

uint32 G3TformrNearXY(uint32 object, uint32 point, uint32 radius)
{
    FUNCTION_MARKER(0x00000ba0, "G3L_81");
    uint32 target_x = (uint32)r_s16(point);
    sint32 x = r_s16(object + 0x58);
    if ((sint32)(target_x - radius) >= x || x >= (sint32)(target_x + radius))
        return 0;
    uint32 target_y = (uint32)r_s16(point + 2);
    sint32 y = r_s16(object + 0x5a);
    return (sint32)(target_y - radius) < y && y < (sint32)(target_y + radius) ? 1 : 0;
}

uint32 G3TformrSetAnimation(uint32 object, uint32 animation, uint32 frame)
{
    FUNCTION_MARKER(0x00000bec, "G3L_81");
    w_u16(object + 0x6e, animation);
    w_u16(object + 0x5e, frame);
    return 1;
}

uint32 G3PlayerUnavailable8007863C(void)
{
    uint32 game = r_u32(0x80097048);
    uint32 player = r_u32(game + 0x50);
    uint32 mode = r_u32(player + 0x150);
    uint32 data = r_u32(player + 0x24);
    if (mode - 4 < 2)
        return 1;
    return r_s16(data + 0x120) != 0 ? 1 : 0;
}

uint32 G3TformrAnimate(uint32 object, uint32 turn_hint, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000031c, "G3L_81");
    uint32 frame = guest_stack - 0x28;
    sint32 current_frame = r_s16(object + 0x5e);
    uint32 resource = r_u32(object + 0x1c);
    uint32 initial_animation = r_u16(object + 0x6e);
    uint32 animations = r_u32(resource + 0x10);
    uint32 animation_data = r_u32(animations + initial_animation * 4);
    uint32 player = r_u32(0x8009efc4);
    sint32 frame_count = r_s16(animation_data + 2);
    bool finished = current_frame == frame_count - 1;
    uint32 outer = G3TformrContains(object, player + 0x58, 1);
    player = r_u32(0x8009efc4);
    uint32 inner = G3TformrContains(object, player + 0x58, 0);
    uint32 animation = r_u16(object + 0x6e);
    uint32 changed = 0;
    if (animation < 14)
    {
        uint32 target = r_u32(module_base + 0xdc4 + animation * 4);
        switch (target - module_base)
        {
        case 0x3c4:
            if (finished)
                changed = G3TformrSetAnimation(object, inner == 0 ? 7 : 8, 0);
            break;
        case 0x3e4:
            if (inner != 0)
                changed = G3TformrSetAnimation(object, 8, 0);
            else if (finished && outer == 0)
                changed = G3TformrSetAnimation(object, 0, 0);
            break;
        case 0x40c:
            if (finished)
                changed = G3TformrSetAnimation(object, 9, 0);
            break;
        case 0x41c:
            if (finished)
                changed = G3TformrSetAnimation(object, 10, 0);
            break;
        case 0x42c:
            if (finished && inner == 0)
            {
                uint32 intro = r_u32(object + 0x20);
                if (G3TformrNearXY(object, intro + 0x10, 0x100) != 0)
                    changed = G3TformrSetAnimation(object, 13, 0);
            }
            break;
        case 0x45c:
            if (finished)
                changed = G3TformrSetAnimation(object, outer == 0 ? 0 : 7, 0);
            break;
        case 0x47c:
            if (finished)
                changed = G3TformrSetAnimation(object, 12, 0);
            break;
        case 0x498:
            if (finished)
            {
                G3Function80038798(object, 0, 0, frame);
                return 0;
            }
            break;
        case 0x4b4:
            break;
        default:
            {
#ifndef NDEBUG
            fprintf(stderr, "G3 unresolved tformr animation target %08X\n", (unsigned int)target);
#endif
            }
            abort();
        }
    }
    if (changed == 0)
        G3Function80029F0C(object, 0xffffffff, frame);
    return 1;
}

void G3TformrReturnHome(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000004e8, "G3L_81");
    uint32 frame = guest_stack - 0x20;
    uint32 player = r_u32(0x8009efc4);
    uint32 intro = r_u32(object + 0x20);
    uint32 heading = G3TformrHeadingTo(object, player + 0x58, frame);
    G3TformrTurnToward(object, (uint32)(sint32)(sint16)heading, 0x80);
    if (G3TformrNearXY(object, intro + 0x10, 0x100) != 0)
        return;
    uint32 object_x = (uint32)r_s16(object + 0x58);
    uint32 object_y = (uint32)r_s16(object + 0x5a);
    uint32 home_y = (uint32)r_s16(intro + 0x12);
    uint32 home_x = (uint32)r_s16(intro + 0x10);
    heading = G3Function8002F240(object_y - home_y, object_x - home_x, frame);
    sint32 first_angle = (sint16)(heading + 0xc00);
    sint32 normalized = (first_angle + 0xc00) % 4096;
    uint32 angle = (uint32)(sint32)(sint16)normalized;
    uint32 sine = G3Function8005B470(angle);
    sint32 x_product = (sint32)(sine * 60);
    uint32 x = r_u16(object + 0x58);
    w_u16(object + 0x58, x + (uint32)(x_product / 4096));
    uint32 cosine = G3Function8005B590(angle);
    sint32 y_product = (sint32)(cosine * 60);
    uint32 y = r_u16(object + 0x5a);
    w_u16(object + 0x5a, y + (uint32)(y_product / 4096));
}

void G3TformrMove(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000604, "G3L_81");
    uint32 frame = guest_stack - 0x40;
    uint32 animation = r_u16(object + 0x6e);
    uint32 original_yaw = r_u16(object + 0x64);
    uint32 turn = animation < 8 ? 0x60 : 0x80;
    uint32 speed = animation < 8 ? 0x18 : 0x3c;
    uint32 player = r_u32(0x8009efc4);
    G3TformrContains(object, player + 0x58, 1);
    player = r_u32(0x8009efc4);
    uint32 inner = G3TformrContains(object, player + 0x58, 0);
    animation = r_u16(object + 0x6e);
    if (animation == 7)
    {
        sint32 animation_frame = r_s16(object + 0x5e);
        if (animation_frame == 8)
            G3TformrShoot(object, 1, frame);
        else if (animation_frame == 0)
            G3TformrShoot(object, 0, frame);
    }
    bool returned_home = false;
    if (inner == 0)
    {
        animation = r_u16(object + 0x6e);
        if (animation == 8 || animation == 12 || animation == 10 || animation == 9 || animation == 13)
        {
            G3TformrReturnHome(object, frame);
            returned_home = true;
        }
    }
    if (!returned_home)
    {
        animation = r_u16(object + 0x6e);
        if (animation == 10)
        {
            if (G3PlayerUnavailable8007863C() != 0)
            {
                G3TformrReturnHome(object, frame);
                returned_home = true;
            }
            else
                animation = r_u16(object + 0x6e);
        }
        if (!returned_home && animation != 11)
        {
            uint32 destination;
            if (G3PlayerUnavailable8007863C() != 0)
            {
                uint32 intro = r_u32(object + 0x20);
                destination = intro + 0x10;
            }
            else
            {
                player = r_u32(0x8009efc4);
                destination = player + 0x58;
            }
            w_u16(frame + 0x18, speed);
            w_u16(frame + 0x1a, speed);
            w_u16(frame + 0x1c, speed);
            w_u32(frame + 0x10, 0x100);
            G3Function8003639C(object, turn, frame + 0x18, destination, 0x100, frame);
        }
    }
    uint32 current_yaw = r_u16(object + 0x64);
    sint32 difference = (sint16)(current_yaw - original_yaw);
    sint32 turn_hint = 0;
    if ((sint32)(turn >> 1) < difference || difference < -0x800)
        turn_hint = -1;
    else if (difference < -(sint32)turn / 2 || difference > 0x800)
        turn_hint = 1;
    G3TformrAnimate(object, (uint32)turn_hint, module_base, frame);
}

void G3TformrShoot(uint32 object, uint32 side, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000bfc, "G3L_81");
    uint32 frame = guest_stack - 0x28;
    sint32 yaw = r_s16(object + 0x64);
    sint32 direction = (yaw + 0xc00) % 4096;
    sint32 position_angle = (yaw + (side == 0 ? 0xd10 : 0xaf0)) % 4096;
    uint32 state = object + 0x104;
    uint32 resource = r_u32(state + 0x18);
    if (resource == 0)
        return;
    uint32 projectile = G3Function8002A454(object, resource, frame);
    if (projectile == 0)
        return;
    uint32 angle = (uint32)(sint32)(sint16)position_angle;
    w_u32(projectile + 0x20, 0);
    w_u16(projectile + 0x68, 0x1000);
    w_u16(projectile + 0x6a, 0x1000);
    w_u16(projectile + 0x6c, 0x1000);
    uint32 sine = G3Function8005B470(angle);
    sint32 product = (sint32)(sine * 300);
    uint32 x = r_u16(projectile + 0x58);
    w_u16(projectile + 0x58, x + (uint32)(product / 4096));
    uint32 cosine = G3Function8005B590(angle);
    product = (sint32)(cosine * 300);
    angle = (uint32)(sint32)(sint16)direction;
    uint32 y = r_u16(projectile + 0x5a);
    uint32 z = r_u16(projectile + 0x5c);
    w_u16(projectile + 0x5a, y + (uint32)(product / 4096));
    w_u16(projectile + 0x5c, z + 200);
    sine = G3Function8005B470(angle);
    product = (sint32)(sine * 60);
    w_u32(projectile + 0x104, (uint32)(product / 4096));
    cosine = G3Function8005B590(angle);
    product = (sint32)(cosine * 60);
    w_u32(projectile + 0x108, (uint32)(product / 4096));
    w_u32(projectile + 0x10c, 0xffffffff);
    sine = G3Function8005B470(angle);
    w_u32(projectile + 0x110, (uint32)((sint32)sine / 1024));
    cosine = G3Function8005B590(angle);
    w_u32(projectile + 0x114, (uint32)((sint32)cosine / 1024));
    w_u32(projectile + 0x118, 0);
}

uint32 G3TrackerEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_28");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3TrackerInitialize(object, game, frame);
        return 1;
    case 2:
        G3TrackerUpdate(object, game, frame);
        return 1;
    case 3:
        G3TrackerCollide(object, game, frame);
        return 1;
    case 4:
        return 1;
    case 2017:
        G3TrackerConfigure(object + 0x104, payload);
        return 1;
    default:
        return 0;
    }
}

void G3TrackerConfigure(uint32 state, uint32 data)
{
    FUNCTION_MARKER(0x000000a8, "G3L_28");
    w_u16(state, r_u16(data));
    w_u16(state + 2, r_u16(data + 2));
    uint32 maximum = r_u16(data + 4);
    sint32 denominator = r_s16(state + 2);
    uint32 numerator = (uint32)((sint32)(maximum << 16) >> 4);
    uint32 gain = denominator == 0 ? ((sint32)numerator < 0 ? 1u : 0xffffffffu) : (uint32)((sint64)(sint32)numerator / denominator);
    w_u16(state + 4, maximum);
    w_u16(state + 6, gain);
    w_u16(state + 8, r_u16(data + 6));
    w_u16(state + 0xc, r_u16(data + 8));
    uint32 acceleration = r_u16(data + 0xa);
    w_u16(state + 0xa, 0);
    w_u16(state + 0xe, acceleration);
    w_u16(state + 0x12, r_u16(data + 0xe));
    w_u16(state + 0x10, r_u16(data + 0xc));
}

void G3TrackerInitialize(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000011c, "G3L_28");
    uint32 frame = guest_stack - 0x20;
    uint32 data = r_u32(object + 0x24);
    uint32 state = object + 0x104;
    w_u16(object + 0x104, 30);
    w_u16(state + 2, 0x155);
    sint32 denominator = r_s16(state + 2);
    uint32 gain = denominator == 0 ? 0xffffffff : (uint32)((sint64)0x28000 / denominator);
    w_u16(state + 0x10, 0);
    w_u16(state + 0x12, 0);
    w_u16(state + 0xe, 0);
    w_u16(state + 0xa, 0);
    w_u16(state + 0xc, 0);
    w_u16(state + 8, 1);
    w_u16(state + 4, 40);
    w_u16(state + 6, gain);
    if (data != 0)
        G3TrackerConfigure(state, data);
    uint32 owner = r_u32(object + 0x2c);
    if (owner != 0)
    {
        uint32 target = r_u32(owner + 0xec);
        if (target != 0)
            G3DispatchStateEvent(target, owner, game, 2006, object, frame);
    }
    w_u16(state + 0x14, 0);
}

void G3TrackerTrail(uint32 state, uint32 data, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000001e0, "G3L_28");
    uint32 frame = guest_stack - 0x18;
    if (data == 0)
        return;
    uint32 counter = r_u16(state + 0x14) + 1;
    w_u16(state + 0x14, counter);
    sint32 interval = r_s32(data + 0x10);
    if (((sint32)(counter << 16) >> 16) < interval)
        return;
    w_u16(state + 0x14, 0);
    uint32 resource = G3Function8003AA48(data + 0x14);
    if (resource != 0)
        G3Function8002A454(object, resource, frame);
}

void G3TrackerUpdate(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000248, "G3L_28");
    uint32 player = r_u32(0x8009efc4);
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    uint32 data = r_u32(object + 0x24);
    uint32 player_position = player + 0x58;
    sint32 lifetime = r_s16(state + 8);
    uint32 lifetime_bits = r_u16(state + 8);
    if (lifetime > 0)
    {
        uint32 remaining = lifetime_bits - 1;
        w_u16(state + 8, remaining);
        if ((remaining << 16) == 0)
        {
            uint32 owner = r_u32(object + 0x2c);
            if (owner != 0)
            {
                uint32 target = r_u32(owner + 0xec);
                if (target != 0)
                    G3DispatchStateEvent(target, owner, game, 2007, object, frame);
            }
            uint32 resource = r_u32(object + 0x1c);
            uint32 flags = r_u32(resource);
            w_u32(resource, flags | 8);
            G3Function80038798(object, 0, 0, frame);
            w_u32(resource, flags);
            return;
        }
    }
    uint32 angle = ((uint32)r_s16(object + 0x64) + 0xc00) & 0xfff;
    uint32 sine = G3Function8005B470(angle);
    uint32 speed = (uint32)r_s16(state);
    uint32 product = speed * sine;
    uint32 x = r_u16(object + 0x58);
    w_u16(object + 0x58, x + (uint32)((sint32)product >> 12));
    uint32 cosine = G3Function8005B590(angle);
    speed = (uint32)r_s16(state);
    product = speed * cosine;
    uint32 y = r_u16(object + 0x5a);
    w_u16(object + 0x5a, y + (uint32)((sint32)product >> 12));
    uint32 px = r_u16(player_position);
    x = r_u16(object + 0x58);
    uint32 dx = px - x;
    w_u16(frame + 0x10, dx);
    uint32 signed_dx = (uint32)((sint32)(dx << 16) >> 16);
    uint32 py = r_u16(player_position + 2);
    y = r_u16(object + 0x5a);
    uint32 dy = py - y;
    w_u16(frame + 0x12, dy);
    uint32 signed_dy = (uint32)((sint32)(dy << 16) >> 16);
    uint32 pz = r_u16(player_position + 4);
    uint32 z = r_u16(object + 0x5c);
    w_u16(frame + 0x14, pz - z);
    uint32 heading = G3Function8002F240(signed_dy, signed_dx, frame);
    uint32 yaw = r_u16(object + 0x64);
    uint32 target_heading = (uint32)((sint32)(heading << 16) >> 16);
    uint32 current_heading = (uint32)((sint32)((yaw + 0xc00) << 16) >> 16);
    uint32 difference = G3Function80038B48(current_heading, target_heading);
    sint32 error = (sint32)(difference << 16) >> 16;
    sint32 limit = r_s16(state + 2);
    uint32 turn;
    if (error < -limit)
        turn = 0x1000 - (uint32)r_s16(state + 4);
    else if (error < 0)
    {
        uint32 gain = (uint32)r_s16(state + 6);
        product = (0 - (uint32)error) * gain;
        turn = 0x1000 - (uint32)((sint32)product >> 12);
    }
    else if (limit < error)
        turn = (uint32)r_s16(state + 4);
    else
    {
        uint32 gain = (uint32)r_s16(state + 6);
        product = (uint32)error * gain;
        turn = (uint32)((sint32)product >> 12);
    }
    yaw = r_u16(object + 0x64);
    w_u16(object + 0x64, (yaw + turn) & 0xfff);
    sint32 player_height = r_s16(player_position + 4);
    sint32 upper = r_s16(state + 0x12);
    sint32 height = r_s16(object + 0x5c);
    uint32 target_speed;
    if (height < player_height + upper)
        target_speed = (uint32)r_s16(state + 0xc);
    else
    {
        sint32 lower = r_s16(state + 0x10);
        if (height < player_height + lower)
            target_speed = 0;
        else
        {
            uint32 maximum = r_u16(state + 0xc);
            target_speed = (uint32)((sint32)((0 - maximum) << 16) >> 16);
        }
    }
    uint32 acceleration = (uint32)r_s16(state + 0xe);
    G3Function80038A4C(state + 0xa, target_speed, acceleration);
    z = r_u16(object + 0x5c);
    uint32 vertical_speed = r_u16(state + 0xa);
    w_u16(object + 0x5c, z + vertical_speed);
    G3TrackerTrail(state, data, object, frame);
}

void G3TrackerCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000004e8, "G3L_28");
    uint32 frame = guest_stack - 0x30;
    uint32 contact = r_u32(object + 0xb0);
    uint32 player = r_u32(game + 0x50);
    G3Function8003C548(contact, frame + 0x10, frame + 0x11);
    uint32 other = r_u32(contact + 0x14);
    if (other == player)
    {
        uint32 first = r_u8(frame + 0x10);
        bool damaged = false;
        if (first - 6 < 4)
        {
            uint32 second = r_u8(frame + 0x11);
            if (second == 9 || second - 3 < 3)
            {
                G3Function8007408C(object, game, frame);
                damaged = true;
            }
        }
        if (!damaged)
        {
            uint32 second = r_u8(frame + 0x11);
            if (second - 6 >= 4)
                return;
            first = r_u8(frame + 0x10);
            if (first != 9 && first - 3 >= 3)
                return;
            if ((r_u32(player + 0x154) & 0xa0) != 0)
                return;
        }
        uint32 owner = r_u32(object + 0x2c);
        if (owner != 0)
        {
            uint32 target = r_u32(owner + 0xec);
            if (target != 0)
                G3DispatchStateEvent(target, owner, game, 2007, object, frame);
        }
    }
    G3Function80038798(object, 0, 0, frame);
}

uint32 G3TumbleEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_18");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3TumbleInitialize(object, module_base, frame);
        return 1;
    case 2:
        G3TumbleUpdate(object, game, frame);
        return 1;
    case 3:
        G3TumbleCollide(object, game, frame);
        return 1;
    case 8:
    case 2020:
    {
        uint32 first = (uint32)r_s16(object + 0x130);
        uint32 second = (uint32)r_s16(object + 0x132);
        G3Function80038798(object, first, second, frame);
        return 1;
    }
    default:
        return 0;
    }
}

void G3PathCenter80040AD4(uint32 channel, uint32 output)
{
    sint32 count = r_s16(channel + 4);
    if (count <= 0)
        return;
    uint32 rows = r_u32(channel);
    uint32 cursor = rows + 6;
    uint32 sum_x = 0;
    uint32 sum_y = 0;
    uint32 sum_z = 0;
    for (sint32 index = 0; index < count; ++index)
    {
        uint32 x = (uint32)r_s16(cursor - 4);
        uint32 y = (uint32)r_s16(cursor - 2);
        uint32 z = (uint32)r_s16(cursor);
        cursor += 32;
        sum_x += x;
        sum_y += y;
        sum_z += z;
    }
    sint32 divisor = r_s16(channel + 4);
    uint32 quotient = divisor == 0 ? ((sint32)sum_x < 0 ? 1 : 0xffffffff) : (uint32)((sint32)sum_x / divisor);
    w_u16(output, quotient);
    divisor = r_s16(channel + 4);
    quotient = divisor == 0 ? ((sint32)sum_y < 0 ? 1 : 0xffffffff) : (uint32)((sint32)sum_y / divisor);
    w_u16(output + 2, quotient);
    divisor = r_s16(channel + 4);
    quotient = divisor == 0 ? ((sint32)sum_z < 0 ? 1 : 0xffffffff) : (uint32)((sint32)sum_z / divisor);
    w_u16(output + 4, quotient);
}

void G3TumbleConfigure(uint32 object, uint32 properties, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000a8, "G3L_18");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    w_u32(frame + 0x34, properties);
    if (properties == 0)
        return;
    while (G3Function80085DFC(frame + 0x34, frame + 0x10, frame + 0x14, frame + 0x18) != 0)
    {
        uint32 kind = r_u16(frame + 0x10);
        uint32 index = (uint32)(sint32)(sint16)(kind - 1);
        if (index >= 40)
            continue;
        uint32 target = r_u32(module_base + 0x890 + (index << 2));
        uint32 data;
        switch (target - module_base)
        {
        case 0xd4:
            continue;
        case 0x128:
        {
            data = r_u32(frame + 0x14);
            uint32 value = (uint32)r_s16(data);
            w_u32(state + 8, value * value);
            value = (uint32)r_s16(data + 2);
            w_u32(state + 0xc, value * value);
            break;
        }
        case 0x15c:
            data = r_u32(frame + 0x14);
            w_u16(state + 0x24, r_u16(data));
            w_u16(state + 0x28, r_u16(data + 2));
            break;
        case 0x17c:
            data = r_u32(frame + 0x14);
            w_u16(state + 0x26, r_u16(data));
            w_u16(state + 0x2a, r_u16(data + 2));
            break;
        case 0x19c:
            data = r_u32(frame + 0x14);
            w_u16(state + 0x2c, r_u16(data));
            w_u16(state + 0x2e, r_u16(data + 2));
            break;
        case 0x1bc:
            data = r_u32(frame + 0x14);
            w_u32(state + 4, object + 0x58);
            w_u32(state, data);
            break;
        default:
            {
#ifndef NDEBUG
            fprintf(stderr, "G3 unresolved tumble property target %08X\n", (unsigned int)target);
#endif
            }
            abort();
        }
    }
}

void G3TumbleInitialize(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000001e8, "G3L_18");
    uint32 frame = guest_stack - 0x20;
    uint32 channel = G3Function800833EC(object);
    uint32 state = object + 0x104;
    if (channel == 0)
    {
        G3Function80038798(object, 0, 0, frame);
        return;
    }
    w_u32(object + 0x10, r_u32(object + 0x10) | 0x100000);
    w_u16(state + 0x14, 0);
    w_u32(state + 0x14, r_u16(state + 0x14));
    uint32 flags = r_u32(object + 0x10);
    uint32 center = object + 0x122;
    w_u32(object + 0x150, 0);
    w_u16(object + 0x6e, 0);
    w_u16(object + 0x5e, 0);
    w_u32(object + 0x10, flags | 0x400);
    G3PathCenter80040AD4(channel, center);
    w_u32(state, 0);
    w_u32(state + 0xc, 0xa62b10);
    w_u16(state + 0x24, 45);
    w_u16(state + 0x28, 35);
    w_u16(state + 0x26, 80);
    w_u16(state + 0x2a, 320);
    w_u32(state + 4, center);
    w_u32(state + 8, 0x19c990);
    w_u16(state + 0x2c, 1);
    w_u16(state + 0x2e, 0);
    uint32 resource = r_u32(object + 0x1c);
    G3TumbleConfigure(object, r_u32(resource + 0x1c), module_base, frame);
    G3TumbleConfigure(object, r_u32(object + 0x28), module_base, frame);
    uint32 value = (uint32)r_s16(state + 0x28);
    uint32 angle = (uint32)r_s16(state + 0x2a);
    w_u32(state + 0x10, G3Function80036908(value, angle));
}

void G3TumbleUpdate(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000300, "G3L_18");
    uint32 frame = guest_stack - 0x60;
    uint32 player = r_u32(game + 0x50);
    uint32 state = object + 0x104;
    w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
    G3Function80029F0C(object, 0xffffffff, frame);
    uint32 mode = r_u32(object + 0x150);
    uint32 player_position = player + 0x58;
    if (mode == 0)
    {
        uint32 reference_x = r_u32(state + 4);
        uint32 px = r_u16(player_position + 0);
        uint32 rx = r_u16(reference_x + 0);
        uint32 dx = px - rx;
        uint32 sx = (uint32)(sint32)(sint16)dx;
        uint32 xx = sx * sx;
        w_u16(frame + 0x18, dx);
        uint32 reference_y = r_u32(state + 4);
        uint32 py = r_u16(player_position + 2);
        uint32 ry = r_u16(reference_y + 2);
        uint32 dy = py - ry;
        uint32 sy = (uint32)(sint32)(sint16)dy;
        uint32 yy = sy * sy;
        w_u16(frame + 0x1a, dy);
        uint32 reference_z = r_u32(state + 4);
        uint32 pz = r_u16(player_position + 4);
        uint32 rz = r_u16(reference_z + 4);
        uint32 dz = pz - rz;
        uint32 sz = (uint32)(sint32)(sint16)dz;
        uint32 zz = sz * sz;
        w_u16(frame + 0x1c, dz);
        sint32 threshold = r_s32(state + 8);
        uint32 distance = xx + yy + zz;
        if (threshold >= (sint32)distance)
        {
            w_u32(object + 0x150, 1);
            w_u16(object + 0x6e, 1);
            w_u16(object + 0x5e, 0);
            return;
        }
        uint32 channel = G3Function800833EC(object);
        {
            uint32 source = object + 0x118;
            uint32 destination = frame + 0x20;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            uint32 output_high = destination + 3;
            uint32 output_lane = destination & 3;
            uint32 output_high_lane = output_high & 3;
            w_masked_u32(output_high & 0xfffffffc, value >> ((3 - output_high_lane) * 8), (1 << (output_high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, value << (output_lane * 8), (15 << output_lane) & 15);
        }
        uint32 point = G3Function8003FC34(channel, frame + 0x20, frame);
        if (point == 0)
            return;
        {
            uint32 source = point;
            uint32 destination = frame + 0x28;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            uint32 last = (uint32)r_s16(source + 4);
            uint32 output_high = destination + 3;
            uint32 output_lane = destination & 3;
            uint32 output_high_lane = output_high & 3;
            w_masked_u32(output_high & 0xfffffffc, value >> ((3 - output_high_lane) * 8), (1 << (output_high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, value << (output_lane * 8), (15 << output_lane) & 15);
            w_u16(destination + 4, last);
        }
        uint32 target_x = r_u16(frame + 0x28);
        uint32 x = r_u16(object + 0x58);
        uint32 heading_dx = target_x - x;
        uint32 target_y = r_u16(frame + 0x2a);
        w_u16(frame + 0x18, heading_dx);
        uint32 y = r_u16(object + 0x5a);
        uint32 heading_dy = target_y - y;
        w_u16(frame + 0x1a, heading_dy);
        uint32 heading;
        if (((heading_dx | heading_dy) << 16) != 0)
            heading = G3Function8002F240((uint32)(sint32)(sint16)heading_dy, (uint32)(sint32)(sint16)heading_dx, frame) + 0x400;
        else
            heading = r_u16(object + 0x64);
        uint32 turn = (uint32)r_s16(state + 0x26);
        if (G3Function80038AA8(object + 0x64, (uint32)(sint32)(sint16)heading, turn) == 0)
            return;
        if (r_s16(state + 0x14) == 0)
            w_u32(object + 0x10, r_u32(object + 0x10) | 0x400);
        else if (r_s16(frame + 0x20) == 0)
            w_u32(object + 0x10, r_u32(object + 0x10) & 0xfffffbff);
        {
            uint32 source = frame + 0x20;
            uint32 destination = state + 0x14;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            uint32 output_high = destination + 3;
            uint32 output_lane = destination & 3;
            uint32 output_high_lane = output_high & 3;
            w_masked_u32(output_high & 0xfffffffc, value >> ((3 - output_high_lane) * 8), (1 << (output_high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, value << (output_lane * 8), (15 << output_lane) & 15);
        }
        {
            uint32 source = frame + 0x28;
            uint32 destination = object + 0x58;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            uint32 last = (uint32)r_s16(source + 4);
            uint32 output_high = destination + 3;
            uint32 output_lane = destination & 3;
            uint32 output_high_lane = output_high & 3;
            w_masked_u32(output_high & 0xfffffffc, value >> ((3 - output_high_lane) * 8), (1 << (output_high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, value << (output_lane * 8), (15 << output_lane) & 15);
            w_u16(destination + 4, last);
        }
        return;
    }
    if (mode == 1)
    {
        if ((r_u32(object + 0x14) & 0x10) != 0)
        {
            w_u32(object + 0x150, 2);
            w_u16(object + 0x6e, 2);
            w_u16(object + 0x5e, 0);
        }
    }
    if (mode == 1 || mode == 2)
    {
        uint32 reference_x = r_u32(state + 4);
        uint32 px = r_u16(player_position + 0);
        uint32 rx = r_u16(reference_x + 0);
        uint32 dx = px - rx;
        uint32 sx = (uint32)(sint32)(sint16)dx;
        uint32 xx = sx * sx;
        w_u16(frame + 0x18, dx);
        uint32 reference_y = r_u32(state + 4);
        uint32 py = r_u16(player_position + 2);
        uint32 ry = r_u16(reference_y + 2);
        uint32 dy = py - ry;
        uint32 sy = (uint32)(sint32)(sint16)dy;
        uint32 yy = sy * sy;
        w_u16(frame + 0x1a, dy);
        uint32 reference_z = r_u32(state + 4);
        uint32 pz = r_u16(player_position + 4);
        uint32 rz = r_u16(reference_z + 4);
        uint32 dz = pz - rz;
        uint32 sz = (uint32)(sint32)(sint16)dz;
        uint32 zz = sz * sz;
        w_u16(frame + 0x1c, dz);
        sint32 threshold = r_s32(state + 0xc);
        uint32 distance = xx + yy + zz;
        if ((sint32)distance >= threshold)
        {
            uint32 channel = G3Function800833EC(object);
            w_u32(object + 0x150, 3);
            w_u16(object + 0x6e, 0);
            w_u16(object + 0x5e, 0);
            G3Function80040A40(channel, object + 0x58, state + 0x14, frame + 0x30, frame);
            {
                uint32 source = frame + 0x30;
                uint32 destination = state + 0x18;
                uint32 high_address = source + 3;
                uint32 high_lane = high_address & 3;
                uint32 lane = source & 3;
                uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                uint32 low = r_masked_u32(source & 0xfffffffc, (15 << lane) & 15);
                uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
                uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
                uint32 last = (uint32)r_s16(source + 4);
                uint32 output_high = destination + 3;
                uint32 output_lane = destination & 3;
                uint32 output_high_lane = output_high & 3;
                w_masked_u32(output_high & 0xfffffffc, value >> ((3 - output_high_lane) * 8), (1 << (output_high_lane + 1)) - 1);
                w_masked_u32(destination & 0xfffffffc, value << (output_lane * 8), (15 << output_lane) & 15);
                w_u16(destination + 4, last);
            }
            return;
        }
        w_u16(frame + 0x38, r_u16(state + 0x24));
        uint32 turn = (uint32)r_s16(state + 0x26);
        w_u32(frame + 0x10, 0);
        G3Function8003639C(object, turn, frame + 0x38, player_position, 0, frame);
        uint32 bounds = r_u32(state);
        if (bounds == 0)
            return;
        sint32 x = r_s16(object + 0x58);
        sint32 minimum_x = r_s16(bounds);
        uint32 raw_x = r_u16(bounds);
        if (x < minimum_x)
            w_u16(object + 0x58, raw_x);
        else
        {
            sint32 maximum_x = r_s16(bounds + 6);
            raw_x = r_u16(bounds + 6);
            if (maximum_x < x)
                w_u16(object + 0x58, raw_x);
        }
        sint32 y = r_s16(object + 0x5a);
        sint32 minimum_y = r_s16(bounds + 2);
        uint32 raw_y = r_u16(bounds + 2);
        if (y < minimum_y)
            w_u16(object + 0x5a, raw_y);
        else
        {
            sint32 maximum_y = r_s16(bounds + 8);
            raw_y = r_u16(bounds + 8);
            if (maximum_y < y)
                w_u16(object + 0x5a, raw_y);
        }
        return;
    }
    if (mode == 3)
    {
        uint32 reference_x = r_u32(state + 4);
        uint32 px = r_u16(player_position + 0);
        uint32 rx = r_u16(reference_x + 0);
        uint32 dx = px - rx;
        uint32 sx = (uint32)(sint32)(sint16)dx;
        uint32 xx = sx * sx;
        w_u16(frame + 0x18, dx);
        uint32 reference_y = r_u32(state + 4);
        uint32 py = r_u16(player_position + 2);
        uint32 ry = r_u16(reference_y + 2);
        uint32 dy = py - ry;
        uint32 sy = (uint32)(sint32)(sint16)dy;
        uint32 yy = sy * sy;
        w_u16(frame + 0x1a, dy);
        uint32 reference_z = r_u32(state + 4);
        uint32 pz = r_u16(player_position + 4);
        uint32 rz = r_u16(reference_z + 4);
        uint32 dz = pz - rz;
        uint32 sz = (uint32)(sint32)(sint16)dz;
        uint32 zz = sz * sz;
        w_u16(frame + 0x1c, dz);
        sint32 threshold = r_s32(state + 8);
        uint32 distance = xx + yy + zz;
        if (threshold >= (sint32)distance)
        {
            w_u32(object + 0x150, 1);
            w_u16(object + 0x6e, 1);
            w_u16(object + 0x5e, 0);
            return;
        }
        w_u16(frame + 0x40, r_u16(state + 0x28));
        uint32 speed = (uint32)r_s16(state + 0x28);
        uint32 turn = (uint32)r_s16(state + 0x2a);
        uint32 limit = r_u32(state + 0x10);
        w_u32(frame + 0x10, limit);
        if (G3Function80036988(object, object + 0x11c, speed, turn, limit, frame) != 0)
            w_u32(object + 0x150, 0);
    }
}

void G3TumbleCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000007bc, "G3L_18");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    uint32 contact = r_u32(object + 0xb0);
    uint32 player = r_u32(game + 0x50);
    G3Function8003C548(contact, frame + 0x10, frame + 0x11);
    uint32 other = r_u32(contact + 0x14);
    if (other != player)
        return;
    uint32 type = r_u8(frame + 0x11);
    if (type == 9 || type - 3 < 3)
    {
        G3Function80074044(object, game, frame);
        return;
    }
    if (((type - 6) & 0xff) >= 4)
        return;
    if ((r_u32(other + 0x154) & 0xa0) != 0)
        return;
    uint32 first = (uint32)r_s16(state + 0x2c);
    uint32 second = (uint32)r_s16(state + 0x2e);
    G3Function80038798(object, first, second, frame);
}

uint32 G3UtakerEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_61");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3UtakerInitialize(object, frame);
        return 1;
    case 2:
        G3UtakerUpdate(object, game, module_base, frame);
        return 1;
    case 3:
        G3UtakerCollide(object, game, module_base, frame);
        return 1;
    default:
        return 0;
    }
}

void G3UtakerConfigure(uint32 object, uint32 commands, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000007c, "G3L_61");
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
            for (uint32 axis = 0; axis != 3; ++axis)
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
            w_u16(state + (kind == 0x31 ? 0x14 : 0x26), (uint32)(scaled / 3600));
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
            uint32 destination = state + 0x18;
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
            w_u32(state + 0x10, resource);
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
            case 0x5b: offset = 0x20; break;
            case 0x5c: offset = 0x1e; break;
            case 0x9d: offset = 0x22; break;
            case 0x5e: offset = 0x2a; break;
            case 0x69: offset = 0x2c; break;
            case 0x9f: offset = 0x2e; break;
            case 0x9e: offset = 0x30; break;
            }
            w_u16(state + offset, value);
            break;
        }
        }
    }
}
