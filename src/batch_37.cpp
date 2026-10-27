#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void G3BirdWander(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000007c0, "G3L_38");
    uint32 frame = guest_stack - 0x38;
    uint32 state = object + 0x104;
    sint32 maximum_x = r_s16(state + 0x3e);
    sint32 minimum_x = r_s16(state + 0x38);
    sint32 minimum_y = r_s16(state + 0x3a);
    sint32 half_x = (sint16)((maximum_x - minimum_x) / 2);
    sint32 maximum_y = r_s16(state + 0x40);
    sint32 half_y = (sint16)((maximum_y - minimum_y) / 2);
    if (half_x == 0 || half_y == 0) return;
    if (r_s16(state + 0xc) != 0)
    {
        w_u16(state + 0xc, 0);
        sint32 random = (sint32)G3SdkRandom80027FA8();
        sint32 remainder = random == (sint32)0x80000000 && half_x == -1 ? 0 : random % half_x;
        uint32 minimum = r_u16(state + 0x38);
        w_u16(state + 4, minimum + (uint32)(half_x / 2) + (uint32)remainder);
        random = (sint32)G3SdkRandom80027FA8();
        remainder = random == (sint32)0x80000000 && half_y == -1 ? 0 : random % half_y;
        minimum = r_u16(state + 0x3a);
        w_u16(state + 6, minimum + (uint32)(half_y / 2) + (uint32)remainder);
        w_u16(state + 8, r_u16(object + 0x5c));
    }
    uint32 speed = (uint32)r_s16(state + 0x24);
    w_u32(frame + 0x10, 0x100);
    G3BirdMoveTo(object, speed, 0xc0, state + 4, 0x100, frame);
    w_u16(state + 0xc, G3BirdNear(object, state + 4, 0x100));
}

void G3BirdIdle(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000008fc, "G3L_38");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    if (G3BirdHomeRange(object) != 0)
    {
        w_u16(state + 0x12, 1);
        w_u16(state + 0x14, 2);
    }
    else if (r_s16(state + 0x1a) == 2) G3BirdWander(object, frame);
}

void G3BirdRetreat(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000095c, "G3L_38");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    sint32 heading = r_s16(object + 0x64);
    sint32 sum = heading + 0xc00;
    sint32 adjusted = sum >= 0 ? sum : heading + 0x1bff;
    uint32 angle = (uint32)sum - ((uint32)(adjusted >> 12) << 12);
    uint32 direction = r_s16(state + 0x1a) == 2 ? 1 : 0xffffffff;
    if (G3BirdAttackRange(object, frame) != 0 && r_s16(state + 0xe) == 0)
    {
        uint32 player = r_u32(0x8009efc4);
        w_u16(state + 0x12, 3);
        {
            uint32 source = player + 0x58;
            uint32 destination = state + 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            uint32 z = (uint32)r_s16(source + 4);
            high_address = destination + 3;
            high_lane = high_address & 3;
            lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15 << lane) & 15);
            w_u16(destination + 4, z);
        }
        w_u16(state + 8, r_u16(state + 0x3c));
    }
    sint32 next = r_s16(state + 0x12);
    if (next == 0)
    {
        uint32 variant = (uint32)r_s16(state + 0x1a);
        uint32 phase = (uint32)r_s16(state + 0x18);
        uint32 row = G3BirdTable(variant, 1, phase, module_base);
        uint32 descriptor = r_u32(object + 0x1c);
        uint32 index = (uint32)r_s16(row);
        uint32 table = r_u32(descriptor + 0x10);
        uint32 animation = r_u32(table + index * 4);
        sint32 length = r_s16(animation + 2);
        sint32 current_frame = r_s16(object + 0x5e);
        sint32 remaining = length - current_frame;
        if (remaining > 0)
        {
            uint32 attachment = r_u32(object + 0x20);
            sint32 destination = r_s16(attachment + 0x10);
            sint32 current = r_s16(object + 0x58);
            sint32 delta = (destination - current) / remaining;
            uint32 bits = r_u16(object + 0x58);
            attachment = r_u32(object + 0x20);
            w_u16(object + 0x58, bits + (uint32)delta);
            destination = r_s16(attachment + 0x12);
            current = r_s16(object + 0x5a);
            delta = (destination - current) / remaining;
            bits = r_u16(object + 0x5a);
            w_u16(object + 0x5a, bits + (uint32)delta);
            destination = r_s16(attachment + 0x14);
            current = r_s16(object + 0x5c);
            delta = (destination - current) / remaining;
            bits = r_u16(object + 0x5c);
            w_u16(object + 0x5c, bits + (uint32)delta);
            destination = r_s16(attachment + 0xc);
            current = r_s16(object + 0x64);
            delta = (destination - current) / remaining;
            bits = r_u16(object + 0x64);
            w_u16(object + 0x64, bits + (uint32)delta);
        }
    }
    else if (next == 2 || next == 3)
    {
        uint32 product = G3Function8005B470(angle) * 30;
        product *= direction;
        sint32 delta = (sint32)product / 4096;
        w_u16(object + 0x58, r_u16(object + 0x58) + (uint32)delta);
        product = G3Function8005B590(angle) * 30;
        product *= direction;
        delta = (sint32)product / 4096;
        uint32 player = 0;
        if (next == 3) player = r_u32(0x8009efc4);
        uint32 y = r_u16(object + 0x5a);
        uint32 z = r_u16(object + 0x5c);
        w_u16(object + 0x5a, y + (uint32)delta);
        w_u16(object + 0x5c, z + 15);
        if (next == 3) G3BirdTurnTo(object, player + 0x58, frame);
    }
    else w_u16(state + 0x12, 2);
}

void G3BirdPursue(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000c58, "G3L_38");
    uint32 frame = guest_stack - 0x40;
    uint32 state = object + 0x104;
    uint32 path = G3Function800833EC(object);
    sint32 previous_arrived = r_s16(state + 0xc);
    sint32 old_speed = r_s16(state + 0xa);
    sint32 desired_speed = r_s16(state + 0x24);
    w_u16(state + 0xc, 0);
    uint32 old_bits = r_u16(state + 0xa);
    uint32 desired_bits = r_u16(state + 0x24);
    uint32 next_speed = desired_speed < old_speed ? old_bits - 2 : desired_bits;
    sint32 bounded = r_s16(state + 0x1c);
    w_u16(state + 0xa, next_speed);
    if (bounded != 0 && G3BirdHomeRange(object) == 0)
    {
        uint32 attachment = r_u32(object + 0x20);
        if (G3BirdNear(object, attachment + 0x10, 0x100) != 0)
        {
            w_u16(state + 0x12, 1);
            w_u16(state + 0x14, 0);
        }
        else
        {
            uint32 speed = (uint32)r_s16(state + 0xa);
            attachment = r_u32(object + 0x20);
            w_u32(frame + 0x10, 0);
            G3BirdMoveTo(object, speed, 0x100, attachment + 0x10, 0, frame);
        }
        return;
    }
    if (G3BirdAttackRange(object, frame) != 0 && r_s16(state + 0xe) == 0)
    {
        if (r_s16(state + 0x1a) == 5) w_u16(state + 0x12, 3);
        else
        {
            w_u16(state + 0x12, 1);
            w_u16(state + 0x14, 3);
        }
        uint32 player = r_u32(0x8009efc4);
        {
            uint32 source = player + 0x58;
            uint32 destination = state + 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            uint32 z = (uint32)r_s16(source + 4);
            high_address = destination + 3;
            high_lane = high_address & 3;
            lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15 << lane) & 15);
            w_u16(destination + 4, z);
        }
        w_u16(state + 8, r_u16(state + 0x3c));
    }
    if (path != 0)
    {
        {
            uint32 source = state;
            uint32 destination = frame + 0x18;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            high_address = destination + 3;
            high_lane = high_address & 3;
            lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15 << lane) & 15);
        }
        if (previous_arrived != 0) G3Function80041230(path, frame + 0x18, frame);
        G3Function80041008(path, frame + 0x18, frame + 0x20, frame);
        {
            uint32 source = frame + 0x18;
            uint32 destination = state;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            high_address = destination + 3;
            high_lane = high_address & 3;
            lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15 << lane) & 15);
        }
        uint32 speed = (uint32)r_s16(state + 0xa);
        w_u32(frame + 0x10, 0);
        G3BirdMoveTo(object, speed, 0x80, frame + 0x20, 0, frame);
        w_u16(state + 0xc, G3BirdNear(object, frame + 0x20, 0x100));
    }
}

void G3BirdAttack(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000e34, "G3L_38");
    uint32 frame = guest_stack - 0x38;
    uint32 state = object + 0x104;
    uint32 speed = r_u16(state + 0x28);
    sint32 arrived = r_s16(state + 0xc);
    w_u16(state + 0xa, speed);
    if (arrived != 0)
    {
        w_u16(state + 0x12, 1);
        w_u16(state + 0x14, 2);
        uint32 position = r_u16(object + 0x58);
        uint32 delta = r_u16(state + 4);
        w_u16(object + 0x58, position + delta);
        position = r_u16(object + 0x5a);
        delta = r_u16(state + 6);
        w_u16(object + 0x5a, position + delta);
        position = r_u16(object + 0x5c);
        delta = r_u16(state + 8);
        w_u16(object + 0x5c, position + delta);
        return;
    }
    uint32 old_x = r_u16(object + 0x58);
    uint32 old_y = r_u16(object + 0x5a);
    w_u32(frame + 0x10, 0x100);
    G3BirdMoveTo(object, (uint32)(sint16)speed, 0xc0, state + 4, 0x100, frame);
    uint32 new_x = r_u16(object + 0x58);
    uint32 new_y = r_u16(object + 0x5a);
    uint32 delta_x = new_x - old_x;
    uint32 delta_y = new_y - old_y;
    if (r_s16(state + 0x1a) == 2)
    {
        uint32 player = r_u32(0x8009efc4);
        w_u16(state + 4, r_u16(player + 0x58));
        w_u16(state + 6, r_u16(player + 0x5a));
    }
    sint32 remaining = r_s16(state + 0x16);
    if (remaining > 0)
    {
        sint32 floor = r_s16(state + 0x3c);
        sint32 z = r_s16(object + 0x5c);
        sint32 delta = (floor - z) / remaining;
        w_u16(object + 0x5c, r_u16(object + 0x5c) + (uint32)delta);
        w_u16(state + 0x16, r_u16(state + 0x16) - 1);
    }
    uint32 player = r_u32(0x8009efc4);
    uint32 contains = G3BirdContainsXY(object, player + 0x58);
    sint32 variant = r_s16(state + 0x1a);
    w_u16(state + 0xc, contains == 0 ? 1 : 0);
    if (variant != 2)
    {
        uint32 near = G3BirdNear(object, state + 4, 0x100);
        uint32 bits = r_u16(state + 0xc);
        w_u16(state + 0xc, bits | near);
    }
    if (r_s16(state + 0xc) != 0)
    {
        w_u16(state + 4, delta_x);
        w_u16(state + 6, delta_y);
        w_u16(state + 8, 30);
    }
}

void G3BirdHurt(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001000, "G3L_38");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    if (r_s16(state + 0x1a) == 2)
    {
        w_u16(object + 0x5c, r_u16(state + 0x3c) + 40);
        if (r_s16(state + 0x10) == 0) G3Function80038798(object, (uint32)r_s16(state + 0x2c), 0, frame);
    }
    else
    {
        sint32 heading = r_s16(object + 0x64);
        sint32 sum = heading + 0xc00;
        sint32 adjusted = sum >= 0 ? sum : heading + 0x1bff;
        uint32 angle = (uint32)sum - ((uint32)(adjusted >> 12) << 12);
        sint32 product = (sint32)(G3Function8005B470(angle) * 30);
        w_u16(object + 0x58, r_u16(object + 0x58) - (uint32)(product / 4096));
        product = (sint32)(G3Function8005B590(angle) * 30);
        uint32 y = r_u16(object + 0x5a);
        uint32 z = r_u16(object + 0x5c);
        w_u16(object + 0x5a, y - (uint32)(product / 4096));
        w_u16(object + 0x5c, z + 20);
    }
}

void G3BirdUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000010fc, "G3L_38");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    sint32 cooldown = r_s16(state + 0xe);
    uint32 bits = r_u16(state + 0xe);
    if (cooldown != 0) w_u16(state + 0xe, bits - 1);
    G3BirdAnimate(object, module_base, frame);
    uint32 effect = r_u32(state + 0x44);
    if (effect != 0) w_u16(effect + 0xe, 2);
    switch (r_u32(object + 0x150))
    {
    case 0: G3BirdIdle(object, frame); break;
    case 1: G3BirdRetreat(object, module_base, frame); break;
    case 2: G3BirdPursue(object, frame); break;
    case 3: G3BirdAttack(object, frame); break;
    case 4: G3BirdHurt(object, frame); break;
    }
    G3BirdClampBounds(object);
}

uint32 G3BirdMoveTo(uint32 object, uint32 speed, uint32 turn, uint32 target, uint32 flags, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000011f8, "G3L_38");
    uint32 frame = guest_stack - 0x38;
    w_u16(frame + 0x18, speed);
    w_u16(frame + 0x1a, speed);
    w_u16(frame + 0x1c, speed);
    w_u32(frame + 0x10, flags);
    uint32 result = G3Function8003639C(object, turn, frame + 0x18, target, flags, frame);
    if ((result & 1) != 0)
    {
        w_u32(object + 0x10, r_u32(object + 0x10) & 0xf7ffffff);
        {
            uint32 source = target;
            uint32 destination = object + 0x58;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            uint32 z = (uint32)r_s16(source + 4);
            high_address = destination + 3;
            high_lane = high_address & 3;
            lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15 << lane) & 15);
            w_u16(destination + 4, z);
        }
    }
    if (target != 0 && (flags & 0x100) == 0) G3BirdApproachHeight80036E9C((uint32)(sint16)speed, (uint32)r_s16(target + 4), object);
    return result;
}

uint32 G3BirdTurnTo(uint32 object, uint32 point, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000012c0, "G3L_38");
    uint32 frame = guest_stack - 0x20;
    sint32 x = r_s16(object + 0x58);
    sint32 y = r_s16(object + 0x5a);
    sint32 point_y = r_s16(point + 2);
    sint32 point_x = r_s16(point);
    uint32 angle = G3Function8002F240((uint32)(y - point_y), (uint32)(x - point_x), frame);
    uint32 target = ((uint32)(sint16)angle + 0x2c00) & 0xfff;
    G3Function80038AA8(object + 0x64, target, 0x80);
    uint32 current = ((uint32)r_s16(object + 0x64) + 0x2000) & 0xfff;
    sint32 difference = (sint32)(target - current);
    if (difference < 0) difference = -difference;
    return difference < 0x41 ? 1 : 0;
}

uint32 G3BirdHomeRange(uint32 object)
{
    FUNCTION_MARKER(0x00001344, "G3L_38");
    uint32 player = r_u32(0x8009efc4);
    uint32 attachment = r_u32(object + 0x20);
    sint32 player_x = r_s16(player + 0x58);
    sint32 home_x = r_s16(attachment + 0x10);
    uint32 x = (uint32)(player_x - home_x);
    uint32 square_x = x * x;
    sint32 player_y = r_s16(player + 0x5a);
    sint32 home_y = r_s16(attachment + 0x12);
    uint32 y = (uint32)(player_y - home_y);
    uint32 square_y = y * y;
    uint32 radius = (uint32)r_s16(object + 0x126);
    return (sint32)(square_x + square_y) < (sint32)(radius * radius) ? 1 : 0;
}

uint32 G3BirdAttackRange(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000139c, "G3L_38");
    uint32 frame = guest_stack - 0x20;
    uint32 player = r_u32(0x8009efc4);
    sint32 object_x = r_s16(object + 0x58);
    sint32 player_x = r_s16(player + 0x58);
    uint32 x = (uint32)(player_x - object_x);
    uint32 square_x = x * x;
    sint32 player_y = r_s16(player + 0x5a);
    sint32 object_y = r_s16(object + 0x5a);
    uint32 y = (uint32)(player_y - object_y);
    uint32 distance = square_x + y * y;
    if (G3BirdContainsXY(object, player + 0x58) == 0) return 0;
    uint32 radius = (uint32)r_s16(object + 0x12a);
    (void)frame;
    return (sint32)distance < (sint32)(radius * radius) ? 1 : 0;
}

uint32 G3BirdNear(uint32 object, uint32 point, uint32 radius)
{
    FUNCTION_MARKER(0x00001434, "G3L_38");
    uint32 x = (uint32)r_s16(point);
    sint32 object_x = r_s16(object + 0x58);
    if ((sint32)(x - radius) >= object_x) return 0;
    if (object_x >= (sint32)(x + radius)) return 0;
    uint32 y = (uint32)r_s16(point + 2);
    sint32 object_y = r_s16(object + 0x5a);
    if ((sint32)(y - radius) >= object_y) return 0;
    if (object_y >= (sint32)(y + radius)) return 0;
    uint32 z = (uint32)r_s16(point + 4);
    sint32 object_z = r_s16(object + 0x5c);
    if ((sint32)(z - radius) >= object_z) return 0;
    return object_z < (sint32)(z + radius) ? 1 : 0;
}

uint32 G3BirdContainsXY(uint32 object, uint32 point)
{
    FUNCTION_MARKER(0x000014a4, "G3L_38");
    uint32 state = object + 0x104;
    sint32 x = r_s16(point);
    sint32 minimum = r_s16(state + 0x38);
    if (minimum >= x) return 0;
    sint32 maximum = r_s16(state + 0x3e);
    if (x >= maximum) return 0;
    sint32 y = r_s16(point + 2);
    minimum = r_s16(state + 0x3a);
    if (minimum >= y) return 0;
    maximum = r_s16(state + 0x40);
    return y < maximum ? 1 : 0;
}

void G3BirdClampBounds(uint32 object)
{
    FUNCTION_MARKER(0x00001500, "G3L_38");
    uint32 state = object + 0x104;
    sint32 position = r_s16(object + 0x58);
    sint32 boundary = r_s16(state + 0x38);
    uint32 bits = r_u16(state + 0x38);
    if (position < boundary) w_u16(object + 0x58, bits);
    position = r_s16(object + 0x58);
    boundary = r_s16(state + 0x3e);
    bits = r_u16(state + 0x3e);
    if (boundary < position) w_u16(object + 0x58, bits);
    position = r_s16(object + 0x5a);
    boundary = r_s16(state + 0x3a);
    bits = r_u16(state + 0x3a);
    if (position < boundary) w_u16(object + 0x5a, bits);
    position = r_s16(object + 0x5a);
    boundary = r_s16(state + 0x40);
    bits = r_u16(state + 0x40);
    if (boundary < position) w_u16(object + 0x5a, bits);
    position = r_s16(object + 0x5c);
    boundary = r_s16(state + 0x3c);
    bits = r_u16(state + 0x3c);
    if (position < boundary) w_u16(object + 0x5c, bits);
    position = r_s16(object + 0x5c);
    boundary = r_s16(state + 0x42);
    bits = r_u16(state + 0x42);
    if (boundary < position) w_u16(object + 0x5c, bits);
}

void G3BirdCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000015cc, "G3L_38");
    uint32 frame = guest_stack - 0x18;
    uint32 contact = r_u32(object + 0xb0);
    uint32 other = r_u32(contact + 0x14);
    uint32 player = r_u32(0x8009efc4);
    if (other != player) return;
    uint32 state = object + 0x104;
    if (r_u32(object + 0x150) != 3) return;
    if (r_s16(state + 0x18) != 0) return;
    G3Function80074044(object, game, frame);
    if (r_s16(state + 0x1a) == 2)
    {
        w_u16(state + 0xc, 1);
        w_u16(state + 4, 0);
        w_u16(state + 6, 0);
        w_u16(state + 8, 30);
    }
}

void G3BirdFragments(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001654, "G3L_38");
    uint32 frame = guest_stack - 0x40;
    uint32 state = object + 0x104;
    if (r_u32(state + 0x34) == 0) return;
    sint32 remaining = r_s16(state + 0x2e);
    while (remaining > 0)
    {
        sint32 random = (sint32)G3SdkRandom80027FA8();
        uint32 range_bits = r_u16(state + 0x30);
        sint32 range = (sint16)range_bits;
        sint32 remainder = range == 0 ? random : random == (sint32)0x80000000 && range == -1 ? 0 : random % range;
        uint32 raw_x = (uint32)remainder - (uint32)(range / 2);
        random = (sint32)G3SdkRandom80027FA8();
        range_bits = r_u16(state + 0x30);
        range = (sint16)range_bits;
        remainder = range == 0 ? random : random == (sint32)0x80000000 && range == -1 ? 0 : random % range;
        uint32 raw_y = (uint32)remainder - (uint32)(range / 2);
        random = (sint32)G3SdkRandom80027FA8();
        range_bits = r_u16(state + 0x30);
        range = (sint16)range_bits;
        sint32 half = range / 2;
        remainder = half == 0 ? random : random == (sint32)0x80000000 && half == -1 ? 0 : random % half;
        uint32 raw_z = (uint32)remainder + (uint32)(range / 4);
        random = (sint32)G3SdkRandom80027FA8();
        range = r_s16(state + 0x30);
        sint32 z = (sint16)raw_z;
        remainder = z == 0 ? random : random == (sint32)0x80000000 && z == -1 ? 0 : random % z;
        uint32 scalar = (uint32)remainder * (uint32)(range / 16) + (uint32)(range / 4);
        sint32 x = (sint16)raw_x;
        sint32 y = (sint16)raw_y;
        w_u16(frame + 0x10, r_u16(object + 0x58) + (uint32)x * 2);
        w_u16(frame + 0x12, r_u16(object + 0x5a) + (uint32)y * 2);
        w_u16(frame + 0x14, r_u16(object + 0x5c) + (uint32)z * 2 + 300);
        uint32 resource = r_u32(state + 0x34);
        uint32 table = r_u32(resource + 0xc);
        uint32 pool = r_u32(0x800a27fc);
        uint32 mesh = r_u32(table);
        uint32 effect = G3Function800444E4(frame + 0x10, mesh, pool, (uint32)(sint16)scalar, frame);
        if (effect != 0)
        {
            uint32 result = G3Function800449F0(effect, (uint32)x, (uint32)y, (uint32)z, effect);
            result = G3Function80044A40(effect, 0, 0, 0xffffffff, result);
            G3Function80044878(effect, object + 0x68, &result);
            G3Function80045920(effect, frame);
        }
        --remaining;
    }
}

void G3BirdDamage(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000185c, "G3L_38");
    uint32 frame = guest_stack - 0x28;
    uint32 mode = r_u32(object + 0x150);
    uint32 commands = r_u32(object + 0x28);
    uint32 state = object + 0x104;
    if (mode == 4) return;
    uint32 health = r_u16(state + 0x1e) - 1;
    w_u16(state + 0x1e, health);
    bool destroy = (sint16)health <= 0;
    if (!destroy)
    {
        uint32 player = r_u32(0x8009efc4);
        destroy = (r_u32(player + 0x134) & 2) != 0;
    }
    if (destroy)
    {
        uint32 selector = (uint32)r_s16(state + 0x2c);
        w_u16(state + 0x1e, 0);
        G3Function80038798(object, selector, 0, frame);
        uint32 effect = r_u32(state + 0x44);
        if (effect != 0)
        {
            w_u16(effect + 0xe, 0);
            w_u32(state + 0x44, 0);
        }
    }
    else
    {
        w_u16(state + 0x12, 4);
        sint32 variant = r_s16(state + 0x1a);
        uint32 previous = variant == 2 ? 0xffffffff : 1;
        uint32 next = (uint32)r_s16(state + 0x12);
        w_u16(state + 0x14, previous);
        G3BirdSetMode(object, next, 0, module_base, frame);
        if (commands != 0) w_u16(commands + 4, r_u16(commands + 4) - 1);
    }
    G3BirdFragments(object, frame);
}

void G3BirdPersistentEffect(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001960, "G3L_38");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    if (r_s16(state + 0x1a) != 5) return;
    if (r_u32(state + 0x44) != 0) return;
    uint32 effect = G3Function800757B0(object, 0, 2, 0x10, frame);
    w_u32(state + 0x44, effect);
    if (effect != 0)
    {
        {
            uint32 source = module_base + 0x1a1c;
            uint32 destination = frame + 0x10;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            high_address = destination + 3;
            high_lane = high_address & 3;
            lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15 << lane) & 15);
        }
        {
            uint32 source = module_base + 0x1a1c;
            uint32 destination = frame + 0x18;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            high_address = destination + 3;
            high_lane = high_address & 3;
            lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15 << lane) & 15);
        }
        effect = r_u32(state + 0x44);
        uint32 result = G3Function8007597C(effect, 2, frame + 0x20, frame, 0);
        effect = r_u32(state + 0x44);
        G3Function80075924(effect, frame + 0x10, frame + 0x18, result);
    }
}

uint32 G3BoingerEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_52");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3BoingerInitialize(object, frame);
        return 3;
    case 2:
        G3BoingerUpdate(object, game, frame);
        return 3;
    case 3:
        G3BoingerCollide(object, game, frame);
        return 3;
    case 9:
        G3BoingerConfigure(object, payload);
        return 3;
    default:
        return 0;
    }
}

uint32 G3PlayerBounceOverride80078728(uint32 player)
{
    return (r_u32(player + 0x154) & 0x40088) != 0 ? 1 : 0;
}

uint32 G3BoingerTriangle(uint32 value)
{
    FUNCTION_MARKER(0x000000a0, "G3L_52");
    uint32 product = value * (value + 1);
    return (uint32)((sint32)product >> 1);
}

void G3BoingerInitialize(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000b4, "G3L_52");
    uint32 frame = guest_stack - 0x30;
    uint32 resource = r_u32(object + 0x1c);
    uint32 data = r_u32(resource + 0x1c);
    uint32 player = r_u32(0x8009efc4);
    uint32 override_data = r_u32(object + 0x28);
    uint32 player_state = r_u32(player + 0x24);
    uint32 state = object + 0x104;
    if (override_data != 0)
        data = override_data;
    uint32 destination = state;
    uint32 source = data;
    for (uint32 index = 0; index < 2; ++index)
    {
        if (data != 0)
        {
            uint32 duration = r_u16(source + 4);
            uint32 divisor = G3BoingerTriangle(duration >> 1);
            uint32 height = r_u32(source + 8);
            uint32 numerator = height << 8;
            uint32 acceleration = divisor == 0 ? 0xffffffff : numerator / divisor;
            w_u32(destination + 8, acceleration);
            duration = r_u16(source + 4);
            uint32 speed = (acceleration * duration) >> 1;
            w_u32(destination + 0xc, speed);
            w_u16(destination + 6, r_u16(source + 6));
            duration = r_u16(source + 4);
            uint32 animation = r_u16(destination + 6);
            w_u16(destination + 4, duration);
            sint32 maximum = r_s16(player_state + 0x20);
            uint32 maximum_bits = r_u16(player_state + 0x20);
            if ((sint32)animation >= maximum)
                w_u16(destination + 6, maximum_bits - 1);
        }
        else
        {
            uint32 divisor = G3BoingerTriangle(225);
            uint32 acceleration = divisor == 0 ? 0xffffffff : 0x190000 / divisor;
            w_u16(destination + 6, 7);
            w_u16(destination + 4, 450);
            w_u32(destination + 8, acceleration);
            w_u32(destination + 0xc, acceleration * 225);
        }
        destination += 12;
        source += 8;
    }
    if (data != 0)
        w_u32(state + 0x24, r_u32(data));
    w_u32(state + 0x20, r_u32(state + 8) >> 8);
    uint32 intro = r_u32(object + 0x20);
    w_u16(object + 0x6e, 1);
    uint32 flags = r_u32(intro + 0x18);
    w_u8(state + 3, (flags & 0x800) != 0 ? 0 : 1);
    (void)frame;
}

void G3BoingerUpdate(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000258, "G3L_52");
    uint32 frame = guest_stack - 0x28;
    uint32 resource = r_u32(object + 0x1c);
    uint32 animation = r_u16(object + 0x6e);
    uint32 player = r_u32(0x8009efc4);
    sint32 animation_count = r_s16(resource + 0xa);
    uint32 player_state = r_u32(player + 0x24);
    uint32 state = object + 0x104;
    if ((sint32)animation < animation_count)
    {
        G3Function80029F0C(object, 0xffffffff, frame);
        if ((r_u32(object + 0x14) & 0x10) != 0)
            G3Function800361DC(object, 1, 0, frame);
    }
    uint32 mode = r_u32(object + 0x150);
    if (mode == 1)
    {
        player = r_u32(0x8009efc4);
        w_u32(object + 0x150, 0);
        uint32 flags = r_u32(player + 0x154);
        w_u8(state + 2, (flags & 0xa0) != 0 ? 1 : 0);
        player = r_u32(0x8009efc4);
        bool accepted = G3PlayerBounceOverride80078728(player) != 0;
        if (!accepted)
        {
            uint32 selection = r_u8(state + 2);
            player = r_u32(0x8009efc4);
            uint32 speed = r_u32(state + selection * 12 + 0xc);
            uint32 target = r_u32(player + 0xec);
            uint32 result = 0;
            if (target != 0)
                result = G3DispatchStateEvent(target, player, game, 1004, speed >> 8, frame);
            accepted = result == 3;
        }
        if (!accepted)
            return;
        uint32 selection = r_u8(state + 2);
        player = r_u32(0x8009efc4);
        uint32 target = r_u32(player + 0xec);
        uint32 selected_animation = r_u16(state + selection * 12 + 6);
        if (target != 0)
            G3DispatchStateEvent(target, player, game, 1005, selected_animation, frame);
        selection = r_u8(state + 2);
        uint32 acceleration = r_u32(state + selection * 12 + 8);
        player = r_u32(0x8009efc4);
        uint32 speed = acceleration >> 8;
        w_u32(state + 0x20, speed);
        target = r_u32(player + 0xec);
        if (target != 0)
            G3DispatchStateEvent(target, player, game, 1006, speed, frame);
        w_u32(object + 0x150, 2);
        G3Function800361DC(object, 0, 0, frame);
        selection = r_u8(state + 2);
        w_u16(state, r_u16(state + selection * 12 + 4));
        w_u32(object + 0x10, r_u32(object + 0x10) | 0x400);
        selection = r_u8(state + 2);
        uint32 script = r_u32(state + 0x24);
        uint32 initial_speed = r_u32(state + selection * 12 + 0xc);
        w_u16(state + 0x28, 5);
        w_u32(state + 0x1c, initial_speed);
        if (script != 0)
        {
            player = r_u32(0x8009efc4);
            G3Function80018DE0(player, script + 4, frame);
        }
        return;
    }
    if (mode != 2)
        return;
    sint32 grace = r_s16(state + 0x28);
    uint32 grace_bits = r_u16(state + 0x28);
    if (grace != 0)
    {
        w_u16(state + 0x28, grace_bits - 1);
        uint32 flags = r_u32(player_state + 0x204);
        if ((flags & 1) != 0)
            w_u32(player_state + 0x204, flags & 0xfffffffe);
    }
    uint32 counter = r_u16(state);
    grace = r_s16(state + 0x28);
    w_u16(state, counter - 1);
    if (grace == 0 && (r_u16(player_state + 0xce) & 0x80) != 0)
        w_u16(state, 0);
    if (r_s16(state) == 0)
    {
        uint32 flags = r_u32(object + 0x10);
        w_u32(object + 0x150, 0);
        w_u32(object + 0x10, flags & 0xfffffbff);
        return;
    }
    uint32 selection = r_u8(state + 2);
    uint32 accumulated = r_u32(state + 0x1c);
    uint32 acceleration = r_u32(state + selection * 12 + 8);
    player = r_u32(0x8009efc4);
    accumulated -= acceleration;
    w_u32(state + 0x1c, accumulated);
    uint32 actual_speed = r_u32(player + 0x10c);
    uint32 speed = r_u32(state + 0x20);
    uint32 adjusted = accumulated;
    if ((sint32)adjusted < 0)
        adjusted += 0xff;
    uint32 expected = (uint32)((sint32)adjusted >> 8);
    if (actual_speed + speed != expected)
    {
        uint32 correction = accumulated - (actual_speed << 8);
        if ((sint32)correction < 0)
            correction += 0xff;
        w_u32(state + 0x20, (uint32)((sint32)correction >> 8));
    }
    player = r_u32(0x8009efc4);
    speed = r_u32(state + 0x20);
    uint32 target = r_u32(player + 0xec);
    G3DispatchStateEvent(target, object, game, 1006, speed, frame);
    selection = r_u8(state + 2);
    player = r_u32(0x8009efc4);
    uint32 selected_animation = r_u16(state + selection * 12 + 6);
    target = r_u32(player + 0xec);
    G3DispatchStateEvent(target, object, game, 1005, selected_animation, frame);
}

void G3BoingerCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000005ec, "G3L_52");
    uint32 frame = guest_stack - 0x20;
    uint32 enabled = r_u8(object + 0x107);
    uint32 contact = r_u32(object + 0xb0);
    if (enabled == 0 || r_u32(object + 0x150) != 0)
        return;
    uint32 other = r_u32(contact + 0x14);
    uint32 player = r_u32(game + 0x50);
    if (other != player)
        return;
    G3Function8003C548(contact, frame + 0x10, frame + 0x11);
    if (r_s8(frame + 0x10) <= 0)
        w_u32(object + 0x150, 1);
}

void G3BoingerConfigure(uint32 object, uint32 payload)
{
    FUNCTION_MARKER(0x0000065c, "G3L_52");
    uint32 state = object + 0x104;
    if (payload != 0)
        w_u8(state + 3, r_u8(payload));
    else
        w_u8(state + 3, r_u8(state + 3) == 0 ? 1 : 0);
    uint32 enabled = r_u8(state + 3);
    uint32 intro = r_u32(object + 0x20);
    uint32 flags = r_u32(intro + 0x18);
    w_u32(intro + 0x18, enabled != 0 ? flags & 0xfffff7ff : flags | 0x800);
}

uint32 G3BomberEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_58");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3BomberInitialize(object, module_base, frame);
        return 1;
    case 2:
        G3BomberUpdate(object, game, module_base, frame);
        return 1;
    case 3:
        G3BomberCollide(object, module_base, frame);
        return 1;
    default:
        return 0;
    }
}

void G3BomberConfigure(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000007c, "G3L_58");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    w_u32(frame + 0x34, commands);
    if (commands == 0)
        return;
    while (G3Function80085DFC(frame + 0x34, frame + 0x10, frame + 0x14, frame + 0x18) != 0)
    {
        uint32 kind = (uint32)(sint32)(sint16)(r_u16(frame + 0x10) - 1);
        if (kind >= 0xa8)
            continue;
        uint32 target = r_u32(module_base + 0xef4 + kind * 4) - module_base;
        switch (target)
        {
        case 0xac:
            break;
        case 0x100:
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 value = r_u16(data);
            w_u16(state + 0x34, value);
            value = r_u16(data + 2);
            w_u16(state + 0x36, value);
            for (uint32 axis = 0; axis != 2; ++axis)
            {
                uint32 radius = (uint32)r_s16(data + 4 + axis * 2);
                w_u32(state + axis * 4, radius * radius);
            }
            break;
        }
        case 0x14c:
        case 0x2d4:
        case 0x31c:
        case 0x350:
        {
            uint32 data = r_u32(frame + 0x14);
            sint32 scaled = (sint32)((uint32)r_s16(data) << 12);
            uint32 offset = target == 0x14c ? 0x1e : target == 0x2d4 ? 0x2e : target == 0x31c ? 0x4a : 0x50;
            w_u16(state + offset, (uint32)(scaled / 3600));
            break;
        }
        case 0x180:
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
        case 0x1e4:
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 game = r_u32(0x80097048);
            uint32 resource = G3FindLevelResource800397B0(data, game);
            w_u32(state + 0xc, resource);
            break;
        }
        case 0x214:
        {
            sint32 size = r_s16(frame + 0x18);
            uint32 data = r_u32(frame + 0x14);
            w_u32(state + 0x10, data);
            w_u16(state + 0x14, (uint32)(size / 6));
            break;
        }
        case 0x270:
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 value = r_u16(data);
            w_u16(state + 0x44, value);
            value = r_u16(data + 2);
            w_u16(state + 0x46, value);
            break;
        }
        case 0x1a8:
        case 0x1bc:
        case 0x1d0:
        case 0x200:
        case 0x248:
        case 0x25c:
        case 0x290:
        case 0x2a4:
        case 0x2b8:
        case 0x308:
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 value = r_u16(data);
            uint32 offset = 0;
            switch (target)
            {
            case 0x1a8: offset = 0x28; break;
            case 0x1bc: offset = 0x2a; break;
            case 0x1d0: offset = 0x32; break;
            case 0x200: offset = 0x3a; break;
            case 0x248: offset = 0x3e; break;
            case 0x25c: offset = 0x42; break;
            case 0x290: offset = 0x48; break;
            case 0x2a4: offset = 0x2c; break;
            case 0x2b8: offset = 0x2c; value = 0u - value; break;
            case 0x308: offset = 0x30; break;
            }
            w_u16(state + offset, value);
            break;
        }
        default:
            {
#ifndef NDEBUG
            fprintf(stderr, "Unresolved Bomber property target %08x\n", (unsigned int)(target + module_base));
#endif
            }
            abort();
        }
    }
}

void G3BomberInitialize(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000039c, "G3L_58");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    w_u32(state, 0x005f5e10);
    w_u16(state + 0x1e, 0x5b);
    w_u16(state + 0x26, 0x4b0);
    w_u16(state + 0x2a, 10);
    w_u16(state + 0x32, (uint32)-7);
    w_u16(state + 0x36, 32000);
    w_u16(state + 0x3e, 30);
    w_u16(state + 0x42, 1);
    w_u16(state + 0x46, 1);
    w_u16(state + 0x48, 3);
    w_u16(state + 0x2c, 0x50);
    w_u32(state + 4, 0x00895440);
    w_u16(state + 0x22, 0);
    w_u16(state + 0x24, 0);
    w_u16(state + 0x28, 0);
    w_u16(state + 0x34, 0);
    w_u16(state + 0x38, 0);
    w_u16(state + 0x3a, 0);
    w_u32(state + 0x10, 0);
    w_u16(state + 0x44, 0);
    w_u32(state + 8, 0);
    w_u16(state + 0x2e, 0);
    w_u16(state + 0x30, 2);
    w_u16(state + 0x50, 0);
    w_u16(state + 0x4a, 0);
    uint32 resource = r_u32(object + 0x1c);
    uint32 commands = r_u32(resource + 0x1c);
    G3BomberConfigure(object, commands, module_base, frame);
    commands = r_u32(object + 0x28);
    if (commands != 0)
    {
        if (r_s16(commands + 2) == 0)
        {
            sint32 health = r_s16(commands);
            uint32 health_half = r_u16(commands);
            if (health != 0)
                w_u16(state + 0x48, health_half);
            else
            {
                health_half = r_u16(state + 0x48);
                w_u16(commands, health_half);
            }
            w_u32(state + 8, commands);
            commands += 4;
        }
        G3BomberConfigure(object, commands, module_base, frame);
    }
    w_u32(object + 0x150, 4);
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
    sint32 end_frame = r_s16(state + 0x46);
    sint32 start_frame = r_s16(state + 0x44);
    sint32 duration = end_frame - start_frame;
    uint32 step = duration == 0 ? 0xffffffff : (uint32)(4096 / duration);
    w_u16(state + 0x1a, step);
    uint32 yaw = r_u16(object + 0x64);
    w_u16(state + 0x20, yaw);
}

void G3BomberPredictPoint(uint32 output, uint32 factor)
{
    FUNCTION_MARKER(0x00000538, "G3L_58");
    uint32 player = r_u32(0x8009efc4);
    uint32 state = r_u32(player + 0x24);
    uint32 index = (uint32)r_s16(state + 0xe0);
    uint32 table = r_u32(state + 0x2c);
    uint32 sample = (uint32)r_s16(table + index * 2);
    uint32 product = sample * (uint32)(sint32)(sint16)factor;
    uint32 yaw = r_u16(player + 0x64);
    uint32 heading = (uint32)(sint32)(sint16)(yaw - 0x400);
    uint32 distance = (uint32)((sint32)product >> 2);
    uint32 cosine = G3Function8005B470(heading);
    product = distance * cosine;
    player = r_u32(0x8009efc4);
    uint32 x = r_u16(player + 0xdc);
    w_u16(output, x + (uint32)((sint32)product >> 12));
    uint32 sine = G3Function8005B590(heading);
    product = distance * sine;
    player = r_u32(0x8009efc4);
    uint32 y = r_u16(player + 0xde);
    w_u16(output + 2, y + (uint32)((sint32)product >> 12));
    uint32 z = r_u16(player + 0xe0);
    w_u16(output + 4, z);
}

void G3BomberCollide(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000e0c, "G3L_58");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    uint32 contact = r_u32(object + 0xb0);
    uint32 other = r_u32(contact + 0x14);
    G3Function8003C548(contact, frame + 0x10, frame + 0x11);
    uint32 resource = r_u32(other + 0x1c);
    uint32 name = r_u32(resource + 0x20);
    uint32 expected = r_u32(module_base + 0x11b4);
    uint32 actual = r_u32(name);
    if (actual != expected)
        return;
    actual = r_u32(name + 4);
    expected = r_u32(module_base + 0x11b8);
    if (actual != expected)
        return;
    if (r_u32(other + 0x150) != 2 || r_u32(object + 0x150) == 0)
        return;
    uint32 health = r_u16(state + 0x48);
    uint32 persisted = r_u32(state + 8);
    w_u16(state + 0x48, health - 1);
    if (persisted != 0)
        w_u16(persisted, health - 1);
    if (r_s16(state + 0x48) <= 0)
        w_u32(object + 0x150, 6);
    else
    {
        w_u32(object + 0x150, 3);
        w_u16(object + 0x6e, 4);
        w_u16(object + 0x5e, 0);
    }
}

void G3BomberUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000610, "G3L_58");
    uint32 frame = guest_stack - 0x58;
    uint32 mode = r_u32(object + 0x150);
    uint32 state = object + 0x104;
    if (mode >= 8)
        return;
    uint32 target = r_u32(module_base + 0x1194 + mode * 4) - module_base;
    uint32 flags;
    switch (target)
    {
    case 0x668:
    {
        flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags & ~0x10u);
        G3Function80029F0C(object, 0xffffffff, frame);
        if ((r_u32(object + 0x14) & 0x10) != 0)
        {
            uint32 index = r_u16(state + 0x18);
            w_u16(state + 0x16, index);
            uint32 source = r_u32(state + 0x10);
            source += (uint32)(sint32)(sint16)index * 6;
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
            goto EnterWait;
        }
        sint32 animation_frame = r_s16(object + 0x5e);
        sint32 start_frame = r_s16(state + 0x44);
        if (animation_frame < start_frame)
            return;
        uint32 start_index = (uint32)r_s16(state + 0x16);
        uint32 path = r_u32(state + 0x10);
        uint32 end_index = (uint32)r_s16(state + 0x18);
        uint32 start = path + start_index * 6;
        sint32 end_frame = r_s16(state + 0x46);
        uint32 end = path + end_index * 6;
        if (animation_frame >= end_frame)
        {
            uint32 source = end;
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
            return;
        }
        uint32 phase_half = r_u16(state + 0x1c);
        uint32 phase_step = r_u16(state + 0x1a);
        uint32 phase = phase_half + phase_step;
        w_u16(state + 0x1c, phase);
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 end_value = (uint32)r_s16(end + axis * 2);
            uint32 start_value = (uint32)r_s16(start + axis * 2);
            uint32 factor = axis == 0 ? (uint32)(sint32)(sint16)phase : (uint32)r_s16(state + 0x1c);
            uint32 product = (end_value - start_value) * factor;
            uint32 base = r_u16(start + axis * 2);
            w_u16(object + 0x58 + axis * 2, base + (uint32)((sint32)product >> 12));
        }
        return;
    }
    case 0x7e8:
    {
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 timer = r_u16(state + 0x3c);
        uint32 player = r_u32(0x8009efc4);
        w_u16(state + 0x3c, timer + 1);
        uint32 other = r_u16(player + 0x58);
        uint32 own = r_u16(object + 0x58);
        uint32 dx = other - own;
        uint32 x = (uint32)(sint32)(sint16)dx;
        uint32 square_x = x * x;
        w_u16(frame + 0x20, dx);
        other = r_u16(player + 0x5a);
        own = r_u16(object + 0x5a);
        uint32 dy = other - own;
        uint32 y = (uint32)(sint32)(sint16)dy;
        uint32 square_y = y * y;
        w_u16(frame + 0x22, dy);
        other = r_u16(player + 0x5c);
        own = r_u16(object + 0x5c);
        uint32 dz = other - own;
        uint32 z = (uint32)(sint32)(sint16)dz;
        uint32 square_z = z * z;
        w_u16(frame + 0x24, dz);
        uint32 distance = square_x + square_y + square_z;
        uint32 heading = G3Function8002F240(y, x, frame);
        sint32 radius = r_s32(state + 4);
        if ((sint32)distance >= radius)
        {
            w_u32(object + 0x150, 4);
            return;
        }
        uint32 step = (uint32)r_s16(state + 0x1e);
        if (G3Function80038AA8(object + 0x64, (uint32)(sint32)(sint16)(heading + 0x400), step) == 0)
            return;
        sint32 elapsed = r_s16(state + 0x3c);
        sint32 delay = r_s16(state + 0x3e);
        if (elapsed < delay)
            return;
        w_u32(object + 0x150, 2);
        w_u16(object + 0x6e, 3);
        w_u16(object + 0x5e, 0);
        w_u16(state + 0x38, 0);
        return;
    }
    case 0x8ec:
    {
        flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags & ~0x10u);
        G3Function80029F0C(object, 0xffffffff, frame);
        if (r_s16(state + 0x38) == 0)
        {
            uint32 player = r_u32(0x8009efc4);
            uint32 own = r_u16(object + 0x58);
            uint32 other = r_u16(player + 0x58);
            uint32 dx = other - own;
            w_u16(frame + 0x20, dx);
            other = r_u16(player + 0x5a);
            own = r_u16(object + 0x5a);
            uint32 dy = other - own;
            w_u16(frame + 0x22, dy);
            other = r_u16(player + 0x5c);
            own = r_u16(object + 0x5c);
            w_u16(frame + 0x24, other - own);
            uint32 heading = G3Function8002F240((uint32)(sint32)(sint16)dy, (uint32)(sint32)(sint16)dx, frame);
            uint32 step = (uint32)r_s16(state + 0x1e);
            G3Function80038AA8(object + 0x64, (uint32)(sint32)(sint16)(heading + 0x400), step);
            sint32 animation_frame = r_s16(object + 0x5e);
            sint32 fire_frame = r_s16(state + 0x2a);
            if (animation_frame >= fire_frame)
            {
                uint32 resource = r_u32(state + 0xc);
                w_u16(state + 0x38, 1);
                if (resource != 0)
                {
                    uint32 projectile = G3Function8002A454(object, resource, frame);
                    if (projectile != 0)
                    {
                        uint32 bone = (uint32)r_s16(state + 0x28);
                        uint32 matrix = r_u32(object + 0x30);
                        matrix += bone << 5;
                        for (uint32 axis = 0; axis != 3; ++axis)
                        {
                            uint32 x = (uint32)r_s16(state + 0x22);
                            uint32 a = (uint32)r_s16(matrix + axis * 6);
                            uint32 product_x = x * a;
                            uint32 y = (uint32)r_s16(state + 0x24);
                            uint32 b = (uint32)r_s16(matrix + axis * 6 + 2);
                            uint32 product_y = y * b;
                            uint32 z = (uint32)r_s16(state + 0x26);
                            uint32 c = (uint32)r_s16(matrix + axis * 6 + 4);
                            uint32 product_z = z * c;
                            uint32 value = (uint32)((sint32)product_x >> 12) + (uint32)((sint32)product_y >> 12) + (uint32)((sint32)product_z >> 12);
                            w_u16(projectile + 0x58 + axis * 2, value);
                        }
                        uint32 value = r_u16(projectile + 0x58);
                        uint32 translation = r_u16(matrix + 0x14);
                        w_u16(projectile + 0x58, value + translation);
                        value = r_u16(projectile + 0x5a);
                        translation = r_u16(matrix + 0x18);
                        w_u16(projectile + 0x5a, value + translation);
                        value = r_u16(projectile + 0x5c);
                        translation = r_u16(matrix + 0x1c);
                        w_u16(projectile + 0x5c, value + translation);
                        value = r_u16(state + 0x32);
                        w_u16(frame + 0x30, value);
                        uint32 factor = (uint32)r_s16(state + 0x3a);
                        G3BomberPredictPoint(frame + 0x28, factor);
                        uint32 target_y = (uint32)r_s16(frame + 0x2a);
                        uint32 projectile_y = (uint32)r_s16(projectile + 0x5a);
                        uint32 target_x = (uint32)r_s16(frame + 0x28);
                        uint32 projectile_x = (uint32)r_s16(projectile + 0x58);
                        uint32 projectile_heading = G3Function8002F240(target_y - projectile_y, target_x - projectile_x, frame);
                        w_u16(projectile + 0x64, projectile_heading + 0x400);
                        uint32 speed = (uint32)r_s16(state + 0x2c);
                        uint32 gravity = (uint32)r_s16(state + 0x32);
                        uint32 minimum = (uint32)r_s16(state + 0x34);
                        w_u32(frame + 0x10, minimum);
                        uint32 maximum = (uint32)r_s16(state + 0x36);
                        w_u32(frame + 0x18, frame + 0x32);
                        w_u32(frame + 0x1c, frame + 0x34);
                        w_u32(frame + 0x14, maximum);
                        if (G3Function80036B80(projectile + 0x58, frame + 0x28, speed, gravity, minimum, maximum, frame + 0x32, frame + 0x34, frame) == 0)
                        {
                            w_u16(frame + 0x34, 0);
                            w_u16(frame + 0x32, 500);
                        }
                        value = r_u16(state + 0x2e);
                        w_u16(frame + 0x36, value);
                        value = r_u16(state + 0x30);
                        w_u16(frame + 0x38, value);
                        value = r_u16(state + 0x4a);
                        w_u16(frame + 0x3a, value);
                        value = r_u16(state + 0x50);
                        w_u16(frame + 0x3c, value);
                        uint32 callback = r_u32(projectile + 0xec);
                        if (callback != 0)
                            G3DispatchStateEvent(callback, projectile, game, 0x7e1, frame + 0x30, frame);
                    }
                }
            }
        }
        if ((r_u32(object + 0x14) & 0x10) == 0)
            return;
        uint32 remaining = r_u16(state + 0x40) - 1;
        w_u16(state + 0x40, remaining);
        if ((sint16)remaining <= 0 && r_u32(state + 0x10) != 0)
            goto ChooseNext;
        w_u32(object + 0x150, 1);
        w_u16(object + 0x6e, 0);
        w_u16(object + 0x5e, 0);
        w_u16(state + 0x3c, 0);
        return;
    }
    case 0xccc:
    {
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 player = r_u32(0x8009efc4);
        uint32 own = r_u16(object + 0x58);
        uint32 other = r_u16(player + 0x58);
        w_u16(frame + 0x20, other - own);
        other = r_u16(player + 0x5a);
        own = r_u16(object + 0x5a);
        w_u16(frame + 0x22, other - own);
        other = r_u16(player + 0x5c);
        own = r_u16(object + 0x5c);
        w_u16(frame + 0x24, other - own);
        uint32 yaw = (uint32)r_s16(state + 0x20);
        uint32 step = (uint32)r_s16(state + 0x1e);
        G3Function80038AA8(object + 0x64, yaw, step);
        uint32 x = (uint32)r_s16(frame + 0x20);
        uint32 square_x = x * x;
        uint32 y = (uint32)r_s16(frame + 0x22);
        uint32 square_y = y * y;
        uint32 z = (uint32)r_s16(frame + 0x24);
        uint32 square_z = z * z;
        sint32 radius = r_s32(state);
        if ((sint32)(square_x + square_y + square_z) >= radius)
            return;
        w_u32(object + 0x150, 5);
        w_u16(object + 0x6e, 1);
        w_u16(object + 0x5e, 0);
        return;
    }
    case 0xc2c:
    case 0xd88:
        flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags & ~0x10u);
        G3Function80029F0C(object, 0xffffffff, frame);
        if ((r_u32(object + 0x14) & 0x10) == 0)
            return;
        if (target == 0xc2c && r_u32(state + 0x10) != 0)
            goto ChooseNext;
        goto EnterWait;
    case 0xddc:
        G3Function80038798(object, 0, 0, frame);
        return;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "Unresolved Bomber update target %08x\n", (unsigned int)(target + module_base));
#endif
        }
        abort();
    }
EnterWait:
    w_u32(object + 0x150, 1);
    w_u16(object + 0x6e, 0);
    w_u16(object + 0x5e, 0);
    {
        uint32 shots = r_u16(state + 0x42);
        w_u16(state + 0x3c, 32000);
        w_u16(state + 0x40, shots);
    }
    return;
ChooseNext:
    w_u32(object + 0x150, 0);
    w_u16(object + 0x6e, 2);
    w_u16(object + 0x5e, 0);
    uint32 random = G3SdkRandom80027FA8();
    sint32 divisor = r_s16(state + 0x14) - 1;
    uint32 remainder = divisor == 0 ? random : (uint32)((sint64)(sint32)random % divisor);
    uint32 current = r_u16(state + 0x16);
    uint32 candidate = current + remainder + 1;
    sint32 count = r_s16(state + 0x14);
    bool before_end = (sint16)candidate < count;
    w_u16(state + 0x18, candidate);
    uint32 count_half = r_u16(state + 0x14);
    if (!before_end)
        w_u16(state + 0x18, candidate - count_half);
    w_u16(state + 0x1c, 0);
}

uint32 G3BouncerEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_08");
    uint32 frame = guest_stack - 0x18;
    uint32 index = message - 1;
    if (index >= 9)
        return 0;
    uint32 target = r_u32(module_base + 0xb74 + index * 4);
    switch (target - module_base)
    {
    case 0x3c:
        G3BouncerInitialize(object);
        return 1;
    case 0x4c:
        G3BouncerUpdate(object, frame);
        return 1;
    case 0x5c:
        G3BouncerCollide(object, game, frame);
        return 1;
    case 0x6c:
        G3BouncerDamage(object, payload, frame);
        return 1;
    case 0x7c:
    {
        uint32 flags = r_u32(object + 0x10);
        uint32 value = r_u16(payload + 8);
        w_u16(object + 0x9e, value);
        w_u32(object + 0x10, flags | 0x80);
        return 1;
    }
    case 0x94:
        return 0;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved bouncer event target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}

void G3BouncerInitialize(uint32 object)
{
    FUNCTION_MARKER(0x000000ac, "G3L_08");
    uint32 data = r_u32(object + 0x28);
    uint32 state = object + 0x104;
    if (data != 0)
    {
        uint32 high_address = data + 3;
        uint32 high_lane = high_address & 3;
        uint32 lane = data & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(data & 0xfffffffc, (15u << lane) & 15);
        uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
        uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        uint32 destination = object + 0x10c;
        high_address = destination + 3;
        high_lane = high_address & 3;
        lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
    }
    else
    {
        w_u16(state + 8, 0);
        w_u16(state + 0xa, 0);
    }
    uint32 values[2];
    for (uint32 index = 0; index != 2; ++index)
    {
        uint32 source = object + 0x60 + index * 4;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 lane = source & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
        uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
        values[index] = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
    }
    for (uint32 index = 0; index != 2; ++index)
    {
        uint32 destination = state + 0x24 + index * 4;
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        uint32 lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, values[index] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, values[index] << (lane * 8), (15u << lane) & 15);
    }
    uint32 home_x = (uint32)r_s16(object + 0x88);
    w_u32(state + 0xc, home_x);
    uint32 home_y = (uint32)r_s16(object + 0x8a);
    uint32 yaw = r_u16(state + 0x28);
    w_u16(state + 6, 6);
    w_u16(state + 0x28, yaw & 0xfff);
    w_u32(state + 0x1c, 1);
    w_u32(state + 0x10, home_y);
}

void G3BouncerSpring(uint32 mode, uint32 position, uint32 target, uint32 velocity, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000134, "G3L_08");
    sint32 limit = r_s32(guest_stack + 0x14);
    uint32 acceleration = r_u32(guest_stack + 0x10);
    if (limit == 0)
    {
        w_u32(position, target);
        return;
    }
    if (limit < 0)
        limit = -4096 / limit;
    if (mode != 0 && mode != 1)
        return;
    uint32 current = r_u32(position);
    uint32 speed = r_u32(velocity);
    uint32 force = (uint32)((sint32)(target - current) >> 2) - speed;
    w_u32(acceleration, force);
    speed = r_u32(velocity) + force;
    w_u32(velocity, speed);
    if (mode == 0)
    {
        current = r_u32(position);
        w_u32(position, current + speed);
    }
    else
    {
        if (limit < (sint32)speed)
            w_u32(velocity, (uint32)limit);
        else if ((sint32)speed < (sint32)(0 - (uint32)limit))
            w_u32(velocity, 0 - (uint32)limit);
        current = r_u32(position);
        speed = r_u32(velocity);
        w_u32(position, current + speed);
    }
}

void G3BouncerDamage(uint32 object, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000ae0, "G3L_08");
    uint32 frame = guest_stack - 0x18;
    uint32 state = object + 0x104;
    sint32 mode = r_s16(state + 0xa);
    bool check_damage = mode != 3;
    if (!check_damage)
    {
        uint32 phase = r_u32(state + 0x1c);
        check_damage = phase == 2 || phase == 3 || phase == 4;
    }
    if (check_damage && G3Function8003CBC0(object, 0, payload, frame) != 0)
        G3Function80038798(object, 1, 0, frame);
}

uint32 G3BouncerCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000b54, "G3L_08");
    uint32 frame = guest_stack - 0x18;
    return G3Function8003C6E0(object, game, frame);
}

void G3BouncerUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000210, "G3L_08");
    uint32 frame = guest_stack - 0x40;
    uint32 player = r_u32(0x8009efc4);
    uint32 object_x = (uint32)r_s16(object + 0x58);
    uint32 delta_x = (uint32)r_s16(player + 0x58) - object_x;
    uint32 player_y = (uint32)r_s16(player + 0x5a);
    uint32 delta_y = player_y - (uint32)r_s16(object + 0x5a);
    uint32 distance = delta_x * delta_x + delta_y * delta_y;
    uint32 state = object + 0x104;
    uint32 heading = G3Function8002F240(delta_y, delta_x, frame);
    uint32 home_yaw = r_u16(state + 0x28);
    sint32 mode = r_s16(state + 0xa);
    uint32 relative = (((heading + 0x400) & 0xfff) - home_yaw) & 0xfff;
    bool tracking = false;
    if (mode == 0)
    {
        G3Function80029F0C(object, 0xffffffff, frame);
        tracking = true;
    }
    else if (mode == 1)
    {
        if (r_u32(state + 0x20) != 1)
            G3Function80029F0C(object, 0xffffffff, frame);
        tracking = true;
    }
    else if (mode == 2)
    {
        if (r_s16(object + 0x5e) == 0x2c)
            w_u16(object + 0x5e, 0);
        else
            G3Function80029F0C(object, 0xffffffff, frame);
        tracking = true;
    }
    else if (mode == 3)
    {
        uint32 phase = r_u32(state + 0x1c);
        if (phase == 1)
        {
            G3Function80029F0C(object, 0xffffffff, frame);
            if ((sint32)distance <= 0x017d783f && r_s16(object + 0x5e) == 0x12 && relative - 0x401 < 0x7ff)
            {
                w_u16(state + 6, 0);
                w_u16(state + 0x18, 0);
                w_u32(state + 0x1c, 4);
            }
            else
                tracking = true;
        }
        else if (phase == 4)
        {
            uint32 yaw = r_u16(object + 0x64);
            w_u16(object + 0x64, (yaw + 0x200) & 0xfff);
            uint32 timer = r_u16(state + 6) + 1;
            w_u16(state + 6, timer);
            if ((sint16)timer == 0x10)
            {
                if (relative - 0x401 < 0x7ff && (sint32)distance <= 0x017d783f)
                {
                    uint32 target_x;
                    uint32 target_y;
                    if ((sint32)distance <= 0x000f4240)
                    {
                        player = r_u32(0x8009efc4);
                        target_x = r_u16(player + 0x58);
                        target_y = r_u16(player + 0x5a);
                    }
                    else
                    {
                        uint32 angle = (uint32)(sint32)(sint16)heading;
                        uint32 sine = G3Function8005B470(angle);
                        uint32 offset_x = (uint32)((sint32)(sine * 1000) >> 12);
                        target_x = r_u16(object + 0x58) + offset_x;
                        uint32 cosine = G3Function8005B590(angle);
                        uint32 offset_y = (uint32)((sint32)(cosine * 1000) >> 12);
                        target_y = r_u16(object + 0x5a) + offset_y;
                    }
                    uint32 travel_x = ((uint32)(sint32)(sint16)target_x - (uint32)r_s16(object + 0x58)) << 12;
                    w_u16(object + 0x5e, 0x1c);
                    uint32 old_z = r_u16(object + 0x5c);
                    w_u16(object + 0x5c, old_z + 0x190);
                    w_u32(state + 0xc, (uint32)((sint32)travel_x / 10));
                    uint32 travel_y = ((uint32)(sint32)(sint16)target_y - (uint32)r_s16(object + 0x5a)) << 12;
                    w_u32(state + 0x10, (uint32)((sint32)travel_y / 10));
                    uint32 travel_z = ((uint32)(sint32)(sint16)old_z - (uint32)r_s16(object + 0x5c)) << 12;
                    w_u32(state + 0x14, (uint32)((sint32)travel_z / 10));
                    uint32 x = r_u16(object + 0x58);
                    w_u16(state, x);
                    uint32 y = r_u16(object + 0x5a);
                    w_u16(state + 2, y);
                    uint32 z = r_u16(object + 0x5c);
                    w_u16(state + 6, 0);
                    w_u32(state + 0x1c, 2);
                    w_u16(state + 4, z);
                }
                else
                    w_u32(state + 0x1c, 1);
            }
        }
        else if (phase == 2 || phase == 3)
        {
            uint32 timer = r_u16(state + 6);
            uint32 velocity = r_u32(state + 0xc);
            timer += phase == 2 ? 1 : 0xffffffff;
            uint32 product = (uint32)(sint32)(sint16)timer * velocity;
            w_u16(state + 6, timer);
            uint32 origin = r_u16(state);
            w_u16(object + 0x58, origin + (uint32)((sint32)product >> 12));
            timer = (uint32)r_s16(state + 6);
            velocity = r_u32(state + 0x10);
            product = timer * velocity;
            origin = r_u16(state + 2);
            w_u16(object + 0x5a, origin + (uint32)((sint32)product >> 12));
            timer = (uint32)r_s16(state + 6);
            velocity = r_u32(state + 0x14);
            product = timer * velocity;
            uint32 yaw = r_u16(object + 0x64);
            origin = r_u16(state + 4);
            w_u16(object + 0x64, (yaw + 0x200) & 0xfff);
            w_u16(object + 0x5c, origin + (uint32)((sint32)product >> 12));
            sint32 current_timer = r_s16(state + 6);
            if (phase == 2)
            {
                if (current_timer == 10)
                    w_u32(state + 0x1c, 3);
            }
            else if (current_timer == 0)
            {
                uint32 x = (uint32)r_s16(object + 0x58);
                w_u32(state + 0xc, x);
                uint32 y = (uint32)r_s16(object + 0x5a);
                w_u32(state + 0x10, y);
                w_u16(object + 0x5e, 0x12);
                uint32 z = r_u16(object + 0x5c);
                yaw = r_u16(object + 0x64);
                w_u16(object + 0x5c, z - 0x190);
                w_u16(object + 0x64, yaw & 0xfff);
                w_u32(state + 0x1c, 1);
            }
        }
    }
    if (tracking)
    {
        G3Function80038AA8(object + 0x64, relative, 0x100);
        uint32 angle = (uint32)r_s16(state + 0x28) + 0x400;
        uint32 sine = G3Function8005B470(angle);
        angle = (uint32)r_s16(state + 0x28) + 0x400;
        uint32 cosine = G3Function8005B590(angle);
        player = r_u32(0x8009efc4);
        uint32 home = (uint32)r_s16(object + 0x88);
        uint32 coordinate = (uint32)r_s16(player + 0x58);
        uint32 dot = sine * (home - coordinate);
        home = (uint32)r_s16(object + 0x8a);
        coordinate = (uint32)r_s16(player + 0x5a);
        dot += cosine * (home - coordinate);
        dot = (uint32)((sint32)dot >> 12);
        uint32 projected_x = (uint32)((sint32)(sine * dot) >> 12);
        uint32 projected_y = (uint32)((sint32)(cosine * dot) >> 12);
        angle = (uint32)r_s16(state + 0x28);
        uint32 player_x = r_u16(player + 0x58);
        player_y = r_u16(player + 0x5a);
        uint32 targets[2];
        targets[0] = player_x + projected_x;
        targets[1] = player_y + projected_y;
        sint32 projected_signed[2];
        projected_signed[0] = (sint16)targets[0];
        projected_signed[1] = (sint16)targets[1];
        for (uint32 axis = 0; axis != 2; ++axis)
        {
            if (axis != 0)
                angle = (uint32)r_s16(state + 0x28);
            uint32 trigonometric = axis == 0 ? G3Function8005B470(angle) : G3Function8005B590(angle);
            uint32 extent = (uint32)r_s16(state + 8);
            uint32 span = extent * trigonometric;
            home = (uint32)r_s16(object + 0x88 + axis * 2);
            span = (uint32)((sint32)span >> 12);
            if ((sint32)span < 0)
                span = 0 - span;
            if (projected_signed[axis] < (sint32)(home - span))
            {
                angle = (uint32)r_s16(state + 0x28);
                trigonometric = axis == 0 ? G3Function8005B470(angle) : G3Function8005B590(angle);
                extent = (uint32)r_s16(state + 8);
                span = extent * trigonometric;
                home = r_u16(object + 0x88 + axis * 2);
                span = (uint32)((sint32)span >> 12);
                if ((sint32)span < 0)
                    span = 0 - span;
                targets[axis] = home - span;
            }
            else
            {
                angle = (uint32)r_s16(state + 0x28);
                trigonometric = axis == 0 ? G3Function8005B470(angle) : G3Function8005B590(angle);
                extent = (uint32)r_s16(state + 8);
                span = extent * trigonometric;
                home = (uint32)r_s16(object + 0x88 + axis * 2);
                span = (uint32)((sint32)span >> 12);
                if ((sint32)span < 0)
                    span = 0 - span;
                if ((sint32)(home + span) < projected_signed[axis])
                {
                    angle = (uint32)r_s16(state + 0x28);
                    trigonometric = axis == 0 ? G3Function8005B470(angle) : G3Function8005B590(angle);
                    extent = (uint32)r_s16(state + 8);
                    span = extent * trigonometric;
                    home = r_u16(object + 0x88 + axis * 2);
                    span = (uint32)((sint32)span >> 12);
                    if ((sint32)span < 0)
                        span = 0 - span;
                    targets[axis] = home + span;
                }
            }
        }
        w_u32(frame + 0x10, state + 0x30);
        w_u32(frame + 0x14, 0xffffffc0);
        G3BouncerSpring(1, state + 0xc, (uint32)(sint32)(sint16)targets[0], state + 0x2c, frame);
        w_u32(frame + 0x10, state + 0x38);
        w_u32(frame + 0x14, 0xffffffc0);
        G3BouncerSpring(1, state + 0x10, (uint32)(sint32)(sint16)targets[1], state + 0x34, frame);
        uint32 x = r_u16(state + 0xc);
        w_u16(object + 0x58, x);
        uint32 y = r_u16(state + 0x10);
        w_u16(object + 0x5a, y);
    }
    player = r_u32(0x8009efc4);
    uint32 player_mode = r_u32(player + 0x154);
    bool bounce_active = false;
    if ((player_mode == 8 || player_mode == 0x80) && r_u32(player + 0x150) == 0 && r_s16(state + 0xa) > 0)
    {
        if (r_u32(state + 0x20) == 1)
            bounce_active = true;
        else
        {
            uint32 timer = r_u16(state + 6) - 1;
            w_u16(state + 6, timer);
            if ((timer & 0xffff) == 0)
            {
                w_u32(state + 0x20, 1);
                w_u32(object + 0x10c, 0x80);
                w_u16(state + 6, 6);
            }
        }
    }
    if (!bounce_active)
        bounce_active = r_u32(state + 0x20) == 1;
    if (bounce_active)
    {
        uint32 velocity = r_u32(object + 0x10c) - 0xc;
        if ((sint32)velocity < -0x60)
            velocity = 0xffffffa0;
        w_u32(object + 0x10c, velocity);
        uint32 z = (uint32)r_s16(object + 0x5c);
        velocity = r_u32(object + 0x10c);
        uint32 home_z = (uint32)r_s16(object + 0x8c);
        bool below_home = (sint32)(z + velocity) < (sint32)home_z;
        z = r_u16(object + 0x5c);
        home_z = r_u16(object + 0x8c);
        if (below_home)
        {
            w_u16(object + 0x5c, home_z);
            w_u32(state + 0x20, 0);
        }
        else
        {
            velocity = r_u16(object + 0x10c);
            w_u16(object + 0x5c, z + velocity);
        }
    }
}

uint32 G3BrkblokEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_47");
    uint32 frame = guest_stack - 0x18;
    uint32 index = message - 1;
    if (index >= 12) return 0;
    uint32 target = r_u32(module_base + 0x14ac + index * 4) - module_base;
    switch (target)
    {
    case 0x3c: G3BrkblokRestorePosition(payload); return 1;
    case 0x4c: G3BrkblokRestorePlacements(game, module_base); return 1;
    case 0x5c: G3BrkblokInitialize(object, module_base, frame); return 1;
    case 0x6c: G3BrkblokUpdate(object, game, frame); return 1;
    case 0x7c: G3BrkblokCollide(object, game, frame); return 1;
    case 0x8c: G3BrkblokPersist(object); return 1;
    case 0x9c: G3BrkblokDamage(object, game, payload, module_base, frame); return 1;
    case 0xac: G3BrkblokCommand(object, payload, module_base, frame); return 1;
    case 0xbc: return 0;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "Unresolved Brkblok event target %08X\n", target + module_base);
#endif
        }
        abort();
    }
}

void G3BrkblokPersist(uint32 object)
{
    FUNCTION_MARKER(0x000000d4, "G3L_47");
    uint32 state = object + 0x104;
    if (r_s16(state + 2) != 0) w_u16(object + 0x5c, r_u16(state + 4));
    uint32 placement = r_u32(object + 0x20);
    uint32 source = object + 0x58;
    uint32 destination = placement + 0x10;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 lane = source & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
    uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
    uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
    uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
    uint32 last = (uint32)r_s16(object + 0x5c);
    high_address = destination + 3;
    high_lane = high_address & 3;
    lane = destination & 3;
    w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
    w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
    w_u16(destination + 4, last);
}

void G3BrkblokInitialize(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000118, "G3L_47");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    w_u16(state + 0x12, 1);
    w_u16(state + 0x10, 1);
    uint32 commands = r_u32(object + 0x28);
    if (commands == 0) commands = r_u32(object + 0x24);
    if (commands != 0)
    {
        if (G3Function80085AB0(commands, module_base + 0x14dc, frame) != 0) w_u16(state + 0x12, 0);
        else
        {
            uint32 count = G3Function80085AB0(commands, module_base + 0x14e8, frame);
            if (count != 0) w_u16(state + 0x10, r_u16(count));
        }
    }
    uint32 placement = r_u32(object + 0x20);
    if (placement == 0) return;
    uint32 list = r_u32(placement + 4);
    if (list == 0) return;
    sint32 count = r_s32(list);
    list += 4;
    uint32 previous = 0;
    for (sint32 index = 0; index < count; ++index)
    {
        uint32 current = r_u32(list);
        placement = r_u32(object + 0x20);
        if (current == placement)
        {
            if (previous != 0)
            {
                uint32 height = r_u16(previous + 0x14);
                uint32 z = r_u16(object + 0x5c);
                w_u16(state, height - z);
            }
            return;
        }
        previous = current;
        list += 4;
    }
}
