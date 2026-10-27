#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void G3SantaCollide(uint32 object, uint32 module_base)
{
    FUNCTION_MARKER(0x00000df0, "G3L_51");
    uint32 contact = r_u32(object + 0xb0);
    uint32 other = r_u32(contact + 0x14);
    uint32 resource = r_u32(other + 0x1c);
    uint32 state = object + 0x104;
    uint32 name = r_u32(resource + 0x20);
    uint32 expected = r_u32(module_base + 0xebc);
    uint32 actual = r_u32(name);
    if (actual != expected)
        return;
    actual = r_u32(name + 4);
    expected = r_u32(module_base + 0xec0);
    if (actual != expected)
        return;
    if (r_u32(other + 0x150) != 1)
        return;
    if (r_u16(object + 0x6e) == 2)
        return;
    uint32 health = r_u16(state + 0x3a);
    uint32 persisted = r_u32(state + 8);
    w_u16(state + 0x3a, health - 1);
    if (persisted != 0)
        w_u16(persisted, health - 1);
    uint32 flags = r_u32(object + 0x10);
    w_u16(object + 0x6e, 5);
    w_u16(object + 0x5e, 0);
    w_u32(object + 0x10, flags | 0x400);
}

void G3SantaUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000004dc, "G3L_51");
    uint32 frame = guest_stack - 0x38;
    uint32 animation = r_u16(object + 0x6e);
    uint32 state = object + 0x104;
    if (animation >= 10)
        return;
    uint32 target = r_u32(module_base + 0xe94 + animation * 4) - module_base;
    uint32 flags;
    switch (target)
    {
    case 0x534:
    {
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 player = r_u32(0x8009efc4);
        uint32 own = r_u16(object + 0x58);
        uint32 other = r_u16(player + 0x58);
        w_u16(frame + 0x10, other - own);
        other = r_u16(player + 0x5a);
        own = r_u16(object + 0x5a);
        w_u16(frame + 0x12, other - own);
        other = r_u16(player + 0x5c);
        own = r_u16(object + 0x5c);
        w_u16(frame + 0x14, other - own);
        uint32 yaw = (uint32)r_s16(state + 0x20);
        uint32 step = (uint32)r_s16(state + 0x1e);
        G3Function80038AA8(object + 0x64, yaw, step);
        uint32 x = (uint32)r_s16(frame + 0x10);
        uint32 square_x = x * x;
        uint32 y = (uint32)r_s16(frame + 0x12);
        uint32 square_y = y * y;
        uint32 z = (uint32)r_s16(frame + 0x14);
        uint32 square_z = z * z;
        sint32 radius = r_s32(state);
        if ((sint32)(square_x + square_y + square_z) >= radius)
            return;
        if (r_u32(state + 0x10) == 0)
            return;
        w_u16(object + 0x6e, 1);
        w_u16(object + 0x5e, 0);
        return;
    }
    case 0x62c:
    {
        flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags & ~0x10u);
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 player = r_u32(0x8009efc4);
        uint32 own = r_u16(object + 0x58);
        uint32 other = r_u16(player + 0x58);
        uint32 dx = other - own;
        w_u16(frame + 0x10, dx);
        other = r_u16(player + 0x5a);
        own = r_u16(object + 0x5a);
        uint32 dy = other - own;
        w_u16(frame + 0x12, dy);
        other = r_u16(player + 0x5c);
        own = r_u16(object + 0x5c);
        w_u16(frame + 0x14, other - own);
        uint32 heading = G3Function8002F240((uint32)(sint32)(sint16)dy, (uint32)(sint32)(sint16)dx, frame);
        uint32 step = (uint32)r_s16(state + 0x1e);
        G3Function80038AA8(object + 0x64, (uint32)(sint32)(sint16)(heading + 0x400), step);
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
            w_u16(object + 0x6e, 3);
            w_u16(object + 0x5e, 0);
            w_u16(state + 0x2e, 0);
            return;
        }
        sint32 animation_frame = r_s16(object + 0x5e);
        sint32 start_frame = r_s16(state + 0x36);
        if (animation_frame < start_frame)
            return;
        uint32 start_index = (uint32)r_s16(state + 0x16);
        uint32 path = r_u32(state + 0x10);
        uint32 end_index = (uint32)r_s16(state + 0x18);
        uint32 start = path + start_index * 6;
        sint32 end_frame = r_s16(state + 0x38);
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
    case 0x820:
    {
        flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags & ~0x10u);
        G3Function80029F0C(object, 0xffffffff, frame);
        if (r_s16(state + 0x2e) == 0)
        {
            uint32 player = r_u32(0x8009efc4);
            uint32 own = r_u16(object + 0x58);
            uint32 other = r_u16(player + 0x58);
            uint32 dx = other - own;
            w_u16(frame + 0x10, dx);
            other = r_u16(player + 0x5a);
            own = r_u16(object + 0x5a);
            uint32 dy = other - own;
            w_u16(frame + 0x12, dy);
            other = r_u16(player + 0x5c);
            own = r_u16(object + 0x5c);
            w_u16(frame + 0x14, other - own);
            uint32 heading = G3Function8002F240((uint32)(sint32)(sint16)dy, (uint32)(sint32)(sint16)dx, frame);
            uint32 step = (uint32)r_s16(state + 0x1e);
            G3Function80038AA8(object + 0x64, (uint32)(sint32)(sint16)(heading + 0x400), step);
            sint32 animation_frame = r_s16(object + 0x5e);
            sint32 fire_frame = r_s16(state + 0x2a);
            if (animation_frame >= fire_frame)
            {
                uint32 resource = r_u32(state + 0xc);
                w_u16(state + 0x2e, 1);
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
                        player = r_u32(0x8009efc4);
                        w_u16(projectile + 0x5c, value + translation);
                        uint32 player_x = (uint32)r_s16(player + 0x58);
                        uint32 projectile_x = (uint32)r_s16(projectile + 0x58);
                        uint32 distance_x = player_x - projectile_x;
                        uint32 square_x = distance_x * distance_x;
                        uint32 player_y = (uint32)r_s16(player + 0x5a);
                        uint32 projectile_y = (uint32)r_s16(projectile + 0x5a);
                        uint32 distance_y = player_y - projectile_y;
                        uint32 range = G3Function8002F448(square_x + distance_y * distance_y, 0, frame);
                        if (range == 0)
                            w_u16(frame + 0x1a, 0);
                        else
                        {
                            player = r_u32(0x8009efc4);
                            uint32 height = (uint32)r_s16(state + 0x3e);
                            uint32 player_z = (uint32)r_s16(player + 0x5c);
                            uint32 projectile_z = (uint32)r_s16(projectile + 0x5c);
                            uint32 speed = (uint32)r_s16(state + 0x2c);
                            sint32 numerator = (sint32)(speed * (player_z + height - projectile_z));
                            sint64 quotient = (sint64)numerator / (sint64)(sint32)range;
                            w_u16(frame + 0x1a, (uint32)quotient);
                        }
                        value = r_u16(state + 0x2c);
                        w_u16(frame + 0x18, value);
                        value = r_u16(state + 0x30);
                        w_u16(frame + 0x1c, value);
                        value = r_u16(state + 0x3c);
                        w_u16(frame + 0x1e, value);
                        uint32 callback = r_u32(projectile + 0xec);
                        if (callback != 0)
                            G3DispatchStateEvent(callback, projectile, game, 0x7e1, frame + 0x18, frame);
                    }
                }
            }
        }
        if ((r_u32(object + 0x14) & 0x10) == 0)
            return;
        w_u16(object + 0x6e, 4);
        w_u16(object + 0x5e, 0);
        w_u16(state + 0x32, 0);
        return;
    }
    case 0xb2c:
    {
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 timer = r_u16(state + 0x32);
        uint32 player = r_u32(0x8009efc4);
        w_u16(state + 0x32, timer + 1);
        uint32 other = r_u16(player + 0x58);
        uint32 own = r_u16(object + 0x58);
        uint32 dx = other - own;
        uint32 x = (uint32)(sint32)(sint16)dx;
        uint32 square_x = x * x;
        w_u16(frame + 0x10, dx);
        other = r_u16(player + 0x5a);
        own = r_u16(object + 0x5a);
        uint32 dy = other - own;
        uint32 y = (uint32)(sint32)(sint16)dy;
        uint32 square_y = y * y;
        w_u16(frame + 0x12, dy);
        other = r_u16(player + 0x5c);
        own = r_u16(object + 0x5c);
        uint32 dz = other - own;
        uint32 z = (uint32)(sint32)(sint16)dz;
        uint32 square_z = z * z;
        w_u16(frame + 0x14, dz);
        uint32 distance = square_x + square_y + square_z;
        uint32 heading = G3Function8002F240(y, x, frame);
        uint32 step = (uint32)r_s16(state + 0x1e);
        G3Function80038AA8(object + 0x64, (uint32)(sint32)(sint16)(heading + 0x400), step);
        sint32 radius = r_s32(state + 4);
        if ((sint32)distance >= radius)
        {
            w_u16(object + 0x6e, 9);
            w_u16(object + 0x5e, 0);
            return;
        }
        sint32 elapsed = r_s16(state + 0x32);
        sint32 delay = r_s16(state + 0x34);
        if (elapsed < delay)
            return;
        w_u16(object + 0x6e, 2);
        goto ChooseNext;
    }
    case 0x5f4:
    case 0xc18:
    case 0xc80:
    case 0xcb8:
    case 0xd0c:
    case 0xd98:
        flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags & ~0x10u);
        G3Function80029F0C(object, 0xffffffff, frame);
        if ((r_u32(object + 0x14) & 0x10) == 0)
            return;
        if (target == 0x5f4 || target == 0xd0c)
        {
            w_u16(object + 0x6e, 2);
            goto ChooseNext;
        }
        if (target == 0xc18)
        {
            if (r_s16(state + 0x3a) > 0)
            {
                w_u16(object + 0x6e, 6);
                flags = r_u32(object + 0x10);
                w_u16(object + 0x5e, 0);
                w_u32(object + 0x10, flags & ~0x400u);
                return;
            }
            w_u16(object + 0x6e, 7);
        }
        else if (target == 0xc80)
            w_u16(object + 0x6e, 8);
        else if (target == 0xcb8)
        {
            flags = r_u32(object + 0x10);
            w_u32(object + 0x10, flags & ~0x400u);
            G3Function80038798(object, 0, 0, frame);
            return;
        }
        else
            w_u16(object + 0x6e, 0);
        w_u16(object + 0x5e, 0);
        return;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "Unresolved Santa update target %08x\n", (unsigned int)(target + module_base));
#endif
        }
        abort();
    }
ChooseNext:
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

uint32 G3ScannonEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_13");
    uint32 frame = guest_stack - 0x18;
    if (object == 0)
        return 1;
    uint32 resource = r_u32(object + 0x1c);
    uint32 state = object + 0x104;
    uint32 controls = r_u32(resource + 0x1c);
    if (controls == 0)
        return 1;
    switch (message)
    {
    case 1:
        G3ScannonInitialize(object, state);
        return 1;
    case 2:
        G3ScannonUpdate(object, game, state, controls, module_base, frame);
        return 1;
    case 3:
        G3ScannonCollide(object, game);
        return 1;
    default:
        return 0;
    }
}

void G3ScannonInitialize(uint32 object, uint32 state)
{
    FUNCTION_MARKER(0x000000a4, "G3L_13");
    w_u32(object + 0x154, 0);
    w_u16(object + 0x5e, 0);
    w_u16(state + 0x30, 0x904);
    w_u16(state + 0x36, 0x46);
    w_u16(state + 0x32, 0x157c);
    w_u16(state + 0x26, 0);
    w_u32(state + 0x2c, 0);
    w_u16(state + 0x34, 0);
    w_u16(state + 0x38, 1);
}

uint32 G3ScannonMoveToward(uint32 output, uint32 first, uint32 second, uint32 step)
{
    FUNCTION_MARKER(0x000000dc, "G3L_13");
    step = (uint32)(sint32)(sint16)step;
    uint32 total = 0;
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        uint32 a = r_u16(first + axis * 2);
        uint32 b = r_u16(second + axis * 2);
        total += G3Function80038A4C(output + axis * 2, (uint32)(sint32)(sint16)(a + b), step);
    }
    return total == 3 ? 1 : 0;
}

uint32 G3ScannonTurnToward(uint32 output, uint32 first, uint32 second, uint32 step)
{
    FUNCTION_MARKER(0x0000019c, "G3L_13");
    step = (uint32)(sint32)(sint16)step;
    uint32 total = 0;
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        uint32 a = r_u16(first + axis * 2);
        uint32 b = r_u16(second + axis * 2);
        total += G3Function80038AA8(output + axis * 2, (uint32)(sint32)(sint16)(a + b), step);
    }
    return total == 3 ? 1 : 0;
}

void G3ScannonUpdateRotation(uint32 object, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000025c, "G3L_13");
    uint32 frame = guest_stack - 0x40;
    w_u32(frame + 0x20, 0x1000);
    w_u32(frame + 0x18, 0x1000);
    w_u32(frame + 0x1c, 0);
    w_u32(frame + 0x14, 0);
    uint32 pitch = (uint32)r_s16(state + 0x1c);
    w_u32(frame + 0x10, 0x1000);
    G3Function8005B94C(0 - pitch, frame + 0x10, frame);
    uint32 owner = r_u32(object + 0x20);
    uint32 yaw = (uint32)r_s16(state + 0x20);
    uint32 base_yaw = (uint32)r_s16(owner + 0xc);
    G3Function8005BB6C(yaw + base_yaw, frame + 0x10, frame);
    G3Function800202A4(frame + 0x10, object + 0x60, 0x15, frame);
}

void G3ScannonReadControls(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000002d8, "G3L_13");
    uint32 frame = guest_stack - 0x18;
    sint32 pitch_delta = r_s32(game + 0x80);
    sint32 yaw_delta = r_s32(game + 0x7c);
    uint32 original_pitch = r_u16(state + 0x1c);
    uint32 original_yaw = r_u16(state + 0x20);
    w_u16(state + 0x1c, original_pitch - (uint32)(pitch_delta / 8));
    uint32 yaw = r_u16(state + 0x20);
    w_u16(state + 0x20, yaw - (uint32)(yaw_delta / 6));
    sint32 lower = r_s16(controls + 0x16);
    sint32 current = r_s16(state + 0x1c);
    uint32 selected = r_u16(controls + 0x16);
    uint32 current_half = r_u16(state + 0x1c);
    if (lower < current)
        selected = current_half;
    sint32 upper = r_s16(controls + 0x18);
    uint32 result = (uint32)upper;
    if ((sint16)selected < upper)
        result = selected;
    w_u16(state + 0x1c, result);
    sint32 yaw_limit = r_s16(controls + 0x1a);
    sint32 current_yaw = r_s16(state + 0x20);
    sint32 clamped = -yaw_limit;
    if (clamped < current_yaw)
        clamped = current_yaw;
    if (clamped < yaw_limit)
        yaw_limit = clamped;
    sint32 final_pitch = r_s16(state + 0x1c);
    w_u16(state + 0x20, (uint32)yaw_limit);
    uint32 moved = (sint16)original_pitch != final_pitch || (sint16)original_yaw != (sint16)yaw_limit ? 1 : 0;
    G3Function80039F5C(object, state + 0x2c, moved, frame);
}

void G3ScannonAlignPlayer(uint32 object, uint32 controls, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000003d8, "G3L_13");
    uint32 frame = guest_stack - 0x30;
    uint32 matrix = r_u32(object + 0x30);
    if (matrix == 0)
        return;
    ApplyMatrixSV((MATRIX *)psx_addr(matrix + 0x80, 20), (SVECTOR *)psx_addr(controls + 0x10, 8), (SVECTOR *)psx_addr(frame + 0x10, 6));
    matrix = r_u32(object + 0x30);
    uint32 x = r_u16(frame + 0x10);
    uint32 translation = r_u16(matrix + 0x94);
    x += translation;
    w_u16(frame + 0x10, x);
    matrix = r_u32(object + 0x30);
    uint32 y = r_u16(frame + 0x12);
    translation = r_u16(matrix + 0x98);
    y += translation;
    w_u16(frame + 0x12, y);
    matrix = r_u32(object + 0x30);
    uint32 z = r_u16(frame + 0x14);
    translation = r_u16(matrix + 0x9c);
    uint32 player = r_u32(0x8009efc4);
    z += translation;
    w_u16(frame + 0x14, z);
    uint32 player_x = r_u16(player + 0x58);
    w_u16(frame + 0x18, x - player_x);
    uint32 player_y = r_u16(player + 0x5a);
    w_u16(frame + 0x1a, y - player_y);
    uint32 player_z = r_u16(player + 0x5c);
    w_u16(frame + 0x1c, z - player_z);
    uint32 callback = r_u32(player + 0xec);
    if (callback != 0)
        G3DispatchStateEvent(callback, player, game, 0x7e2, frame + 0x18, frame);
    uint32 yaw = r_u16(object + 0x64);
    player = r_u32(0x8009efc4);
    w_u16(player + 0x64, yaw + 0x800);
}

void G3ScannonCollide(uint32 object, uint32 game)
{
    FUNCTION_MARKER(0x00000ddc, "G3L_13");
    uint32 collision = r_u32(object + 0xb0);
    uint32 other = r_u32(collision + 0x14);
    uint32 player = r_u32(game + 0x50);
    if (other == player && r_u32(object + 0x154) == 0)
        w_u32(object + 0x154, 1);
}

void G3ScannonShoot(uint32 object, uint32 state, uint32 controls, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000004d0, "G3L_13");
    uint32 frame = guest_stack - 0x40;
    uint32 matrix = r_u32(object + 0x30);
    if (matrix != 0)
    {
        ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(controls + 0x1c, 8), (SVECTOR *)psx_addr(frame + 0x18, 6));
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 local = r_u16(frame + 0x18 + axis * 2);
            uint32 origin = r_u16(object + 0x58 + axis * 2);
            w_u16(frame + 0x10 + axis * 2, local + origin);
        }
    }
    uint32 resource = G3Function8003AA48(controls + 0x24);
    if (resource != 0)
    {
        uint32 projectile = G3Function8002A454(object, resource, frame);
        if (projectile != 0)
        {
            uint32 x = r_u16(frame + 0x10);
            w_u16(projectile + 0x58, x);
            uint32 y = r_u16(frame + 0x12);
            w_u16(projectile + 0x5a, y);
            uint32 z = r_u16(frame + 0x14);
            y = r_u16(projectile + 0x5a);
            w_u16(projectile + 0x5c, z);
            x = r_u16(projectile + 0x58);
            z = r_u16(projectile + 0x5c);
            w_u16(projectile + 0x8a, y);
            w_u16(projectile + 0x88, x);
            w_u16(projectile + 0x8c, z);
            uint32 name = r_u32(resource + 0x20);
            uint32 expected = r_u32(module_base + 0xe14);
            uint32 word = r_u32(name);
            bool cannonball = false;
            if (word == expected)
            {
                uint32 second = r_u32(name + 4);
                uint32 expected_second = r_u32(module_base + 0xe18);
                cannonball = second == expected_second;
            }
            else
                name = r_u32(resource + 0x20);
            if (cannonball)
            {
                uint32 pitch = r_u16(state + 0x1c);
                uint32 intro = r_u32(object + 0x20);
                uint32 yaw = r_u16(state + 0x20);
                pitch = (uint32)(sint32)(sint16)(0 - pitch);
                uint32 base_yaw = r_u16(intro + 0xc);
                yaw += base_yaw + 0x400;
                uint32 sine_pitch = G3Function8005B470(pitch);
                yaw = (uint32)(sint32)(sint16)yaw;
                uint32 sine_yaw = G3Function8005B470(yaw);
                uint32 speed = (uint32)r_s16(controls + 0x3e);
                uint32 horizontal = (uint32)((sint32)(speed * sine_pitch) >> 12);
                w_u32(projectile + 0x104, (uint32)((sint32)(horizontal * sine_yaw) >> 12));
                sine_pitch = G3Function8005B470(pitch);
                uint32 cosine_yaw = G3Function8005B590(yaw);
                speed = (uint32)r_s16(controls + 0x3e);
                horizontal = (uint32)((sint32)(speed * sine_pitch) >> 12);
                w_u32(projectile + 0x108, (uint32)((sint32)(horizontal * cosine_yaw) >> 12));
                uint32 cosine_pitch = G3Function8005B590(pitch);
                speed = (uint32)r_s16(controls + 0x3e);
                w_u32(projectile + 0x10c, (uint32)((sint32)(speed * cosine_pitch) >> 12));
                uint32 acceleration = (uint32)r_s16(controls + 0x40);
                w_u32(projectile + 0x118, acceleration);
                uint32 value = (uint32)r_s16(controls + 0x42);
                w_u32(projectile + 0x128, 500);
                w_u32(projectile + 0x124, value);
                uint32 effect_resource = G3Function8003AA48(controls + 0x34);
                w_u32(projectile + 0x12c, effect_resource);
                value = (uint32)r_s16(controls + 0x3c);
                w_u32(projectile + 0x130, value);
            }
            else
            {
                expected = r_u32(module_base + 0xe20);
                word = r_u32(name);
                if (word == expected)
                {
                    uint32 second = r_u32(name + 4);
                    uint32 expected_second = r_u32(module_base + 0xe24);
                    if (second == expected_second)
                    {
                        uint32 yaw = r_u16(object + 0x64);
                        w_u16(projectile + 0x64, yaw + 0x800);
                    }
                }
            }
        }
    }
    uint32 effect_resource = G3Function8003AA48(controls + 0x2c);
    if (effect_resource != 0)
    {
        uint32 data = r_u32(effect_resource + 0xc);
        uint32 pool = r_u32(0x800a27fc);
        uint32 effect_data = r_u32(data);
        uint32 effect = G3Function800444E4(frame + 0x10, effect_data, pool, 0xffffffff, frame);
        if (effect != 0)
            G3Function80044878(effect, controls + 0x44, 0);
    }
}

void G3ScannonUpdate(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000774, "G3L_13");
    uint32 frame = guest_stack - 0xa8;
    uint32 camera = r_u32(game + 0x4c);
    G3FillMemory80086094(frame + 0x18, 0, 8);
    uint32 mode = r_u32(object + 0x154);
    if (mode == 1)
    {
        uint32 player = r_u32(0x8009efc4);
        uint32 camera_enabled = r_u32(controls + 0x50);
        uint32 player_data = r_u32(player + 0x24);
        uint32 next;
        if (camera_enabled != 0)
        {
            G3Function80013CDC(camera, 8, frame);
            next = 2;
        }
        else
            next = 3;
        w_u32(object + 0x154, next);
        uint32 flags = r_u32(object + 0x10);
        player = r_u32(0x8009efc4);
        w_u32(object + 0x10, flags | 0x400);
        {
        uint32 value;
        {
            uint32 source = camera;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        }
        uint32 last = (uint32)r_s16(camera + 4);
        {
            uint32 destination = state + 6;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
        }
        w_u16(state + 6 + 4, last);
    }
        {
        uint32 first;
        {
            uint32 source = camera + 0x18;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            first = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        }
        uint32 second;
        {
            uint32 source = camera + 0x18 + 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            second = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        }
        {
            uint32 destination = state + 0xc;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, first >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, first << (lane * 8), (15u << lane) & 15);
        }
        {
            uint32 destination = state + 0xc + 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, second >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, second << (lane * 8), (15u << lane) & 15);
        }
    }
        {
        uint32 first;
        {
            uint32 source = object + 0x60;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            first = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        }
        uint32 second;
        {
            uint32 source = object + 0x60 + 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            second = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        }
        {
            uint32 destination = state + 0x14;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, first >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, first << (lane * 8), (15u << lane) & 15);
        }
        {
            uint32 destination = state + 0x14 + 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, second >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, second << (lane * 8), (15u << lane) & 15);
        }
    }
        w_u16(state + 0x1c, 0);
        w_u16(state + 0x20, 0);
        w_u16(state + 0x24, 0);
        flags = r_u32(player + 0x10);
        w_u32(player + 0x10, flags | 0x100);
        uint32 yaw = r_u16(object + 0x64) - 0x800;
        w_u16(player_data + 0xc6, yaw);
        w_u16(player + 0x64, yaw);
        uint32 matrix = r_u32(object + 0x30);
        if (matrix != 0)
        {
            G3ScannonAlignPlayer(object, controls, game, frame);
            matrix = r_u32(object + 0x30);
            ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(controls, 8), (SVECTOR *)psx_addr(frame + 0x20, 6));
            for (uint32 axis = 0; axis != 3; ++axis)
            {
                uint32 local = r_u16(frame + 0x20 + axis * 2);
                uint32 origin = r_u16(object + 0x58 + axis * 2);
                w_u16(state + axis * 2, local + origin);
            }
        }
    }
    else if (mode == 2)
    {
        uint32 player = r_u32(0x8009efc4);
        uint32 flags = r_u32(player + 0x10);
        w_u32(player + 0x10, flags | 0x100);
        {
        uint32 value;
        {
            uint32 source = state;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        }
        uint32 last = (uint32)r_s16(state + 4);
        {
            uint32 destination = camera;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
        }
        w_u16(camera + 4, last);
    }
        {
        uint32 first;
        {
            uint32 source = object + 0x60;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            first = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        }
        uint32 second;
        {
            uint32 source = object + 0x60 + 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            second = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        }
        {
            uint32 destination = camera + 8;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, first >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, first << (lane * 8), (15u << lane) & 15);
        }
        {
            uint32 destination = camera + 8 + 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, second >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, second << (lane * 8), (15u << lane) & 15);
        }
    }
        w_u32(object + 0x154, 3);
    }
    else if (mode == 3)
    {
        uint32 player = r_u32(0x8009efc4);
        uint32 flags = r_u32(player + 0x10);
        uint32 buttons = (flags & 0x100) == 0 ? game + 0x70 : 0x800a0d28;
        player = r_u32(0x8009efc4);
        flags = r_u32(player + 0x10);
        w_u32(player + 0x10, flags | 0x100);
        G3ScannonAlignPlayer(object, controls, game, frame);
        G3ScannonReadControls(object, game, state, controls, frame);
        G3ScannonUpdateRotation(object, state, frame);
        if ((r_u32(buttons + 4) & 0x80) != 0)
        {
            uint32 camera_enabled = r_u32(controls + 0x50);
            w_u32(object + 0x154, camera_enabled == 0 ? 5 : 4);
            {
        uint32 first;
        {
            uint32 source = state + 0x14;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            first = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        }
        uint32 second;
        {
            uint32 source = state + 0x14 + 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            second = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        }
        {
            uint32 destination = object + 0x60;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, first >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, first << (lane * 8), (15u << lane) & 15);
        }
        {
            uint32 destination = object + 0x60 + 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, second >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, second << (lane * 8), (15u << lane) & 15);
        }
    }
            {
        uint32 first;
        {
            uint32 source = module_base + 0xe2c;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            first = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        }
        uint32 second;
        {
            uint32 source = module_base + 0xe2c + 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            second = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        }
        {
            uint32 destination = frame + 0x30;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, first >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, first << (lane * 8), (15u << lane) & 15);
        }
        {
            uint32 destination = frame + 0x30 + 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, second >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, second << (lane * 8), (15u << lane) & 15);
        }
    }
            G3Function8002F718(frame + 0x38);
            uint32 yaw = (uint32)r_s16(object + 0x64);
            G3Function8005BB6C(yaw, frame + 0x38, frame);
            ApplyMatrixSV((MATRIX *)psx_addr(frame + 0x38, 20), (SVECTOR *)psx_addr(frame + 0x30, 8), (SVECTOR *)psx_addr(frame + 0x20, 6));
            uint32 x = r_u16(frame + 0x20);
            uint32 object_x = r_u16(object + 0x58);
            uint32 y = r_u16(frame + 0x22);
            x += object_x;
            w_u16(frame + 0x20, x);
            uint32 object_y = r_u16(object + 0x5a);
            uint32 z = r_u16(frame + 0x24);
            y += object_y;
            w_u16(frame + 0x22, y);
            uint32 object_z = r_u16(object + 0x5c);
            player = r_u32(0x8009efc4);
            z += object_z;
            w_u16(frame + 0x24, z);
            uint32 player_x = r_u16(player + 0x58);
            w_u16(frame + 0x28, x - player_x);
            uint32 player_y = r_u16(player + 0x5a);
            w_u16(frame + 0x2a, y - player_y);
            uint32 player_z = r_u16(player + 0x5c);
            w_u16(frame + 0x2c, z - player_z);
            uint32 callback = r_u32(player + 0xec);
            uint32 result = callback;
            if (callback != 0)
                result = G3DispatchStateEvent(callback, player, game, 0x7e2, frame + 0x28, frame);
            G3Function8003A114(state + 0x2c, result);
        }
        if (r_s16(object + 0x5e) != 0)
            G3Function8002A1C8(object, 0xffffffff, frame);
        if (r_s16(object + 0x5e) == 10)
            G3ScannonShoot(object, state, controls, module_base, frame);
        sint32 timer = r_s16(state + 0x26);
        uint32 timer_half = r_u16(state + 0x26);
        if (timer <= 0)
        {
            if ((r_u32(buttons) & 0x20) != 0)
            {
                uint32 interval = r_u16(controls + 0x4a);
                w_u16(state + 0x26, interval);
                w_u16(object + 0x5e, 1);
            }
        }
        else
        {
            --timer_half;
            w_u16(state + 0x26, timer_half);
            sint32 interval = r_s16(controls + 0x4a);
            if ((sint16)timer_half == interval - 10)
            {
                w_u32(frame + 0x10, 4);
                w_u32(frame + 0x14, 0);
                G3Function8008194C(0, 2, 0xffffffff, 0x1000, frame);
            }
        }
        if (r_u32(object + 0x30) != 0)
        {
            G3FillMemory80086094(frame + 0x80, 0, 8);
            uint32 current_yaw = (uint32)r_s16(state + 0x24);
            uint32 target_yaw = (uint32)r_s16(state + 0x20);
            sint32 difference = (sint16)G3Function80038B48(current_yaw, target_yaw);
            if (difference < 0)
                difference = -difference;
            sint32 speed = r_s16(controls + 0x4c);
            uint32 speed_half = r_u16(controls + 0x4c);
            if (speed < difference)
            {
                sint32 current = r_s16(state + 0x24);
                sint32 target = r_s16(state + 0x20);
                uint32 target_half = r_u16(state + 0x20);
                if (target < current)
                    w_u16(state + 0x24, target_half + speed_half);
                current = r_s16(state + 0x24);
                target = r_s16(state + 0x20);
                target_half = r_u16(state + 0x20);
                if (current < target)
                {
                    speed_half = r_u16(controls + 0x4c);
                    w_u16(state + 0x24, target_half - speed_half);
                }
            }
            uint32 yaw = r_u16(state + 0x24);
            uint32 intro = r_u32(object + 0x20);
            uint32 offset = r_u16(controls + 0xa);
            uint32 base_yaw = r_u16(intro + 0xc);
            w_u16(frame + 0x84, base_yaw + yaw + offset);
            G3Function8005BD00(frame + 0x80, frame + 0x60);
            ApplyMatrixSV((MATRIX *)psx_addr(frame + 0x60, 20), (SVECTOR *)psx_addr(controls, 8), (SVECTOR *)psx_addr(frame + 0x58, 6));
            if (r_u32(controls + 0x50) == 0)
                return;
            for (uint32 axis = 0; axis != 3; ++axis)
            {
                uint32 local = r_u16(frame + 0x58 + axis * 2);
                uint32 origin = r_u16(object + 0x58 + axis * 2);
                w_u16(camera + axis * 2, local + origin);
            }
        }
        if (r_u32(controls + 0x50) != 0)
        {
            uint32 pitch = r_u16(controls + 6);
            w_u16(camera + 0xa, 0);
            w_u16(camera + 8, pitch);
            uint32 yaw = r_u16(state + 0x24);
            uint32 intro = r_u32(object + 0x20);
            uint32 offset = r_u16(controls + 0xa);
            uint32 base_yaw = r_u16(intro + 0xc);
            w_u16(camera + 0xc, base_yaw + yaw + offset);
        }
    }
    else if (mode == 4)
    {
        {
        uint32 value;
        {
            uint32 source = state + 6;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        }
        uint32 last = (uint32)r_s16(state + 6 + 4);
        {
            uint32 destination = camera;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
        }
        w_u16(camera + 4, last);
    }
        {
        uint32 first;
        {
            uint32 source = state + 0xc;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            first = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        }
        uint32 second;
        {
            uint32 source = state + 0xc + 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            second = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        }
        {
            uint32 destination = camera + 8;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, first >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, first << (lane * 8), (15u << lane) & 15);
        }
        {
            uint32 destination = camera + 8 + 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, second >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, second << (lane * 8), (15u << lane) & 15);
        }
    }
        w_u32(object + 0x154, 5);
        G3Function80013C54(camera, frame);
    }
    else if (mode == 5)
    {
        uint32 flags = r_u32(object + 0x10);
        uint32 player = r_u32(0x8009efc4);
        w_u16(object + 0x5e, 0);
        w_u32(object + 0x10, flags & 0xfffffbff);
        flags = r_u32(player + 0x10);
        w_u32(player + 0x10, flags & 0xfffffeff);
        w_u32(object + 0x154, 0);
    }
}

uint32 G3SchestEvent(uint32 object, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_09");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3SchestInitialize(object, frame);
        return 1;
    case 2:
        G3SchestUpdate(object, frame);
        return 1;
    case 3:
        G3SchestCollide(object, frame);
        return 1;
    case 5:
        w_u16(r_u32(payload + 0x1c), 2);
        return 1;
    default:
        return 0;
    }
}

void G3SchestConfigure(uint32 object, uint32 commands, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000098, "G3L_09");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    w_u32(guest_stack + 4, commands);
    if (commands == 0)
        return;
    while (G3Function80085DFC(frame + 0x34, frame + 0x10, frame + 0x14, frame + 0x18) != 0)
    {
        sint32 kind = r_s16(frame + 0x10);
        if (kind == 0x5b || kind == 0xb || kind == 0x76)
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 offset = kind == 0x5b ? 0 : kind == 0xb ? 4 : 8;
            w_u16(state + offset, r_u16(data));
        }
        else if (kind == 0x75 || kind == 9)
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 angle = (uint32)r_s16(data) << 12;
            sint64 product = (sint64)(sint32)angle * (sint32)0x91a2b3c5;
            uint32 high = (uint32)((uint64)product >> 32);
            uint32 sum = high + angle;
            uint32 quotient = (uint32)((sint32)sum >> 11) - (uint32)((sint32)angle >> 31);
            w_u16(state + (kind == 0x75 ? 2 : 6), quotient);
        }
    }
}

void G3SchestInitialize(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000001e8, "G3L_09");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    w_u16(object + 0x104, 0);
    w_u16(state + 4, 40);
    w_u16(state + 2, 51);
    w_u16(state + 6, 51);
    w_u16(state + 8, 0xfff9);
    uint32 resource = r_u32(object + 0x1c);
    uint32 commands = r_u32(resource + 0x1c);
    uint32 saved = 0;
    G3SchestConfigure(object, commands, frame);
    commands = r_u32(object + 0x28);
    if (commands != 0)
    {
        if (r_s16(commands + 2) == 0)
        {
            saved = r_u16(commands);
            commands += 4;
        }
        G3SchestConfigure(object, commands, frame);
    }
    sint32 mode = (sint32)(saved << 16) >> 16;
    if (mode == 0 || mode == 2)
    {
        w_u32(object + 0x150, 0);
        w_u16(object + 0x6e, 0);
        w_u16(object + 0x5e, 0);
    }
    else if (mode == 1)
    {
        resource = r_u32(object + 0x1c);
        w_u32(object + 0x150, 2);
        w_u16(object + 0x6e, 0);
        uint32 animations = r_u32(resource + 0x10);
        uint32 animation = r_u32(animations);
        w_u16(object + 0x5e, r_u16(animation + 2) - 1);
    }
    uint32 angle = (uint32)r_s16(state + 2);
    uint32 sine = G3Function8005B470(angle);
    uint32 speed = (uint32)r_s16(state + 4);
    uint32 product = speed * sine;
    angle = (uint32)r_s16(state + 2);
    w_u16(state + 0xa, (uint32)((sint32)product >> 12));
    uint32 cosine = G3Function8005B590(angle);
    speed = (uint32)r_s16(state + 4);
    product = speed * cosine;
    w_u16(state + 0xc, (uint32)((sint32)product >> 12));
}

void G3SchestUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000338, "G3L_09");
    uint32 frame = guest_stack - 0x40;
    uint32 opening = r_u32(object + 0x150);
    uint32 state = object + 0x104;
    if (opening != 1)
        return;
    G3Function80029F0C(object, 0xffffffff, frame);
    if (r_s16(state + 0xe) == 0)
    {
        sint32 animation_frame = r_s16(object + 0x5e);
        sint32 trigger_frame = r_s16(object + 0x104);
        if (animation_frame >= trigger_frame)
        {
            uint32 base_heading = r_u16(object + 0x64);
            uint32 spread = r_u16(state + 6);
            base_heading += 0x400;
            for (uint32 side = 0; side < 2; ++side)
            {
                if (side != 0)
                    spread = r_u16(state + 6);
                uint32 raw_angle = side == 0 ? base_heading - spread : base_heading + spread;
                uint32 angle = (uint32)((sint32)(raw_angle << 16) >> 16);
                uint32 sine = G3Function8005B470(angle);
                uint32 radial = (uint32)r_s16(state + 0xa);
                uint32 x_product = radial * sine;
                uint32 x_shifted = (uint32)((sint32)x_product >> 12);
                uint32 cosine = G3Function8005B590(angle);
                radial = (uint32)r_s16(state + 0xa);
                uint32 y_product = radial * cosine;
                uint32 z = (uint32)r_s16(state + 0xc);
                w_u32(frame + 0x10, 0);
                w_u32(frame + 0x14, 0);
                uint32 fall = (uint32)r_s16(state + 8);
                w_u32(frame + 0x1c, 0);
                w_u32(frame + 0x20, 0);
                w_u32(frame + 0x24, 0);
                w_u32(frame + 0x18, fall);
                uint32 x = (uint32)((sint32)(x_shifted << 16) >> 16);
                uint32 y = (uint32)((sint32)(y_product << 4) >> 16);
                G3Function800460E8(object, x, y, z, frame);
            }
            w_u16(state + 0xe, opening);
            uint32 save = r_u32(object + 0x28);
            if (save != 0)
                w_u16(save, opening);
        }
    }
    uint32 resource = r_u32(object + 0x1c);
    uint32 index = r_u16(object + 0x6e);
    uint32 animations = r_u32(resource + 0x10);
    uint32 animation = r_u32(animations + (index << 2));
    sint32 frames = r_s16(animation + 2);
    sint32 current = r_s16(object + 0x5e);
    if (current == frames - 1)
    {
        w_u32(object + 0x150, 2);
        w_u32(object + 0x10, r_u32(object + 0x10) & 0xfffffbff);
    }
}

void G3SchestCollide(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000518, "G3L_09");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    uint32 contact = r_u32(object + 0xb0);
    G3Function8003C548(contact, frame + 0x10, frame + 0x11);
    uint32 other = r_u32(contact + 0x14);
    uint32 player = r_u32(0x8009efc4);
    if (other != player)
        return;
    uint32 type = r_u8(frame + 0x11);
    if (type - 6 >= 4)
        return;
    if ((r_u32(other + 0x154) & 0xa0) != 0)
        return;
    if (r_u32(object + 0x150) != 0)
        return;
    w_u32(object + 0x150, 1);
    w_u16(state + 0xe, 0);
    w_u32(object + 0x10, r_u32(object + 0x10) | 0x400);
}

uint32 G3ScrankEvent(uint32 object, uint32 game, uint32 message, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_48");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3ScrankInitialize(object, frame);
        return 1;
    case 2:
        G3ScrankUpdate(object, game, frame);
        return 1;
    case 3:
        G3ScrankCollide(object, game, frame);
        return 1;
    default:
        return 0;
    }
}

void G3ScrankConfigure(uint32 object, uint32 commands, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000007c, "G3L_48");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    w_u32(guest_stack + 4, commands);
    if (commands == 0)
        return;
    while (G3Function80085DFC(frame + 0x34, frame + 0x10, frame + 0x14, frame + 0x18) != 0)
    {
        sint32 kind = r_s16(frame + 0x10);
        if (kind == 0x31)
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 4, r_u16(data));
        }
        else if (kind == 0xa1)
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0xa, r_u16(data));
        }
    }
}

void G3ScrankInitialize(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000114, "G3L_48");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    w_u32(object + 0x104, 0);
    w_u16(state + 4, 0x100);
    w_u16(state + 0xa, 9000);
    uint32 resource = r_u32(object + 0x1c);
    uint32 commands = r_u32(resource + 0x1c);
    G3ScrankConfigure(object, commands, frame);
    commands = r_u32(object + 0x28);
    G3ScrankConfigure(object, commands, frame);
    uint32 intro = r_u32(object + 0x20);
    uint32 entries = r_u32(intro + 4);
    if (entries != 0)
    {
        uint32 cursor = entries + 4;
        sint32 count = r_s32(entries);
        for (sint32 index = 0; index < count; ++index)
        {
            uint32 entry = r_u32(cursor);
            intro = r_u32(object + 0x20);
            if (entry != intro)
            {
                w_u32(state, entry);
                break;
            }
            cursor += 4;
        }
    }
    w_u16(state + 0xe, 0);
    w_u16(state + 0xc, 0);
    w_u16(state + 8, 0);
    w_u16(state + 6, 0);
}

void G3ScrankUpdate(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000001dc, "G3L_48");
    uint32 frame = guest_stack - 0x28;
    uint32 linked_intro = r_u32(object + 0x104);
    uint32 state = object + 0x104;
    if (linked_intro == 0)
        w_u16(state + 8, 0);
    else
    {
        uint32 linked = r_u32(linked_intro + 0x20);
        if (linked == 0)
            w_u16(state + 8, 0);
        else
        {
            uint32 target = r_u32(linked + 0xec);
            if (target != 0 && G3DispatchStateEvent(target, linked, game, 2028, frame + 0x10, frame) != 0)
            {
                sint32 value = r_s16(frame + 0x10);
                sint32 gain = r_s16(state + 4);
                uint32 product = (uint32)(value * gain);
                w_u16(state + 8, (uint32)((sint32)product >> 8));
            }
        }
    }
    uint32 rotation = r_u16(object + 0x64);
    uint32 velocity = r_u16(state + 6);
    w_u16(object + 0x64, rotation + velocity);
    uint32 target = (uint32)r_s16(state + 8);
    uint32 step = (uint32)r_s16(state + 0xa);
    G3Function80038A4C(state + 6, target, step);
    uint32 touched = r_u16(state + 0xc);
    w_u16(state + 0xc, 0);
    w_u16(state + 0xe, touched);
}

void G3ScrankCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000298, "G3L_48");
    uint32 frame = guest_stack - 0x30;
    uint32 contact = r_u32(object + 0xb0);
    uint32 state = object + 0x104;
    G3Function8003C548(contact, frame + 0x10, frame + 0x11);
    uint32 other = r_u32(contact + 0x14);
    uint32 player = r_u32(0x8009efc4);
    if (other != player)
        return;
    uint32 second_type = r_u8(frame + 0x11);
    if (second_type - 6 >= 4)
        return;
    uint32 first_type = r_u8(frame + 0x10);
    if (first_type != 9 && first_type - 3 >= 3)
        return;
    if (r_s16(state + 0xe) == 0)
    {
        uint32 linked_intro = r_u32(object + 0x104);
        if (linked_intro != 0)
        {
            uint32 linked = r_u32(linked_intro + 0x20);
            if (linked != 0)
            {
                uint32 target = r_u32(linked + 0xec);
                if (target != 0)
                    G3DispatchStateEvent(target, linked, game, 9, 0, frame);
            }
        }
    }
    w_u16(state + 0xc, 1);
}

uint32 G3ShooterEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_49");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1: G3ShooterInitialize(object, module_base, frame); return 1;
    case 2: G3ShooterUpdate(object, game, module_base, frame); return 1;
    case 9: G3ShooterActivate(object, game, payload, frame); return 1;
    case 0x7e1: return 1;
    case 1000:
    {
        uint32 source = object + 0x128;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 lane = source & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
        uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
        uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        uint32 last = (uint32)r_s16(source + 4);
        high_address = payload + 3;
        high_lane = high_address & 3;
        lane = payload & 3;
        w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(payload & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
        w_u16(payload + 4, last);
        return 1;
    }
    default: return 0;
    }
}

void G3ShooterConfigure(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000b4, "G3L_49");
    uint32 frame = guest_stack - 0x38;
    uint32 state = object + 0x104;
    w_u32(frame + 0x3c, commands);
    if (commands == 0) return;
    while (G3Function80085DFC(frame + 0x3c, frame + 0x10, frame + 0x14, frame + 0x18) != 0)
    {
        uint32 kind = (uint32)(sint32)(sint16)(r_u16(frame + 0x10) - 0x37);
        if (kind >= 77) continue;
        uint32 target = r_u32(module_base + 0x107c + kind * 4) - module_base;
        switch (target)
        {
        case 0xe8: break;
        case 0x13c:
        {
            sint32 size = r_s16(frame + 0x18);
            uint32 data = r_u32(frame + 0x14);
            w_u32(state + 0x10, data);
            w_u16(state + 0x14, (uint32)(size / 12));
            break;
        }
        case 0x174:
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 radius = (uint32)r_s16(data);
            w_u32(state + 8, radius * radius);
            break;
        }
        case 0x1bc:
        {
            uint32 data = r_u32(frame + 0x14);
            sint32 scaled = (sint32)((uint32)r_s16(data) << 12);
            w_u16(state + 0x1a, (uint32)(scaled / 100) + 4096);
            break;
        }
        case 0x218: w_u16(state + 0x3e, 1); break;
        case 0x220: w_u16(state + 0x40, 1); break;
        case 0x228: w_u16(state + 0x42, 1); break;
        case 0x230: w_u16(state + 0x44, 1); break;
        case 0x24c:
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 game = r_u32(0x80097048);
            uint32 resource = G3FindLevelResource800397B0(data, game);
            sint32 size = r_s16(frame + 0x18);
            w_u32(state + 4, resource);
            if (size >= 9)
            {
                data = r_u32(frame + 0x14);
                game = r_u32(0x80097048);
                resource = G3FindLevelResource800397B0(data + 8, game);
                w_u32(state, resource);
            }
            break;
        }
        case 0x194:
        case 0x1a8:
        case 0x1f0:
        case 0x204:
        case 0x238:
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 value = r_u16(data);
            uint32 offset = target == 0x194 ? 0x16 : target == 0x1a8 ? 0x18 : target == 0x1f0 ? 0x36 : target == 0x204 ? 0x1e : 0x22;
            w_u16(state + offset, value);
            break;
        }
        default:
            {
#ifndef NDEBUG
            fprintf(stderr, "Unresolved Shooter property target %08x\n", (unsigned int)(target + module_base));
#endif
            }
            abort();
        }
    }
}

void G3ShooterInitialize(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000002a4, "G3L_49");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    w_u32(state + 4, 0); w_u32(state, 0);
    w_u16(state + 0x16, 18000); w_u16(state + 0x18, 60);
    w_u16(state + 0x1a, 0x1400); w_u16(state + 0x36, 50);
    w_u32(state + 8, 0); w_u32(state + 0xc, 0);
    w_u16(state + 0x14, 0); w_u16(state + 0x1e, 3);
    w_u16(state + 0x20, 0); w_u16(state + 0x22, 0);
    w_u16(state + 0x44, 0); w_u16(state + 0x42, 0);
    w_u16(state + 0x40, 0); w_u16(state + 0x3e, 0);
    uint32 resource = r_u32(object + 0x1c);
    uint32 commands = r_u32(resource + 0x1c);
    G3ShooterConfigure(object, commands, module_base, frame);
    commands = r_u32(object + 0x28);
    G3ShooterConfigure(object, commands, module_base, frame);
    uint32 interval = r_u16(state + 0x1e);
    w_u16(state + 0x1c, interval);
    uint32 x = r_u16(object + 0x58);
    w_u32(object + 0x150, 1);
    w_u16(state + 0x24, x);
    uint32 y = r_u16(object + 0x5a);
    w_u16(state + 0x28, 0);
    w_u16(state + 0x26, y);
    uint32 placement = r_u32(object + 0x20);
    uint32 list = r_u32(placement + 4);
    if (list != 0)
    {
        uint32 cursor = list + 4;
        sint32 count = r_s32(list);
        for (sint32 index = 0; index < count; ++index, cursor += 4)
        {
            uint32 linked = r_u32(cursor);
            placement = r_u32(object + 0x20);
            if (linked != placement)
            {
                w_u32(state + 0xc, linked);
                for (uint32 axis = 0; axis != 3; ++axis)
                {
                    linked = r_u32(cursor);
                    uint32 parent = r_u32(linked + 0x20);
                    uint32 own = r_u16(object + 0x58 + axis * 2);
                    uint32 other = r_u16(parent + 0x58 + axis * 2);
                    w_u16(state + 0x38 + axis * 2, own - other);
                }
                break;
            }
        }
    }
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x10, flags | 0x800);
}

void G3ShooterTrack(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000434, "G3L_49");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    uint32 player = r_u32(game + 0x50);
    uint32 own = r_u16(state + 0x24);
    uint32 other = r_u16(player + 0xdc);
    uint32 dx = other - own;
    w_u16(frame + 0x10, dx);
    other = r_u16(player + 0xde); own = r_u16(state + 0x26);
    uint32 dy = other - own;
    w_u16(frame + 0x12, dy);
    other = r_u16(player + 0xe0); own = r_u16(state + 0x28);
    uint32 dz = other - own;
    w_u16(frame + 0x14, dz);
    if (((dx | dy | dz) & 0xffff) == 0)
    {
        w_u16(state + 0x2e, 0); w_u16(state + 0x2c, 0); w_u16(state + 0x2a, 0);
        return;
    }
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        uint32 delta = axis == 0 ? (uint32)(sint32)(sint16)dx : (uint32)r_s16(frame + 0x10 + axis * 2);
        uint32 scale = (uint32)r_s16(state + 0x1a);
        uint32 product = delta * scale;
        uint32 base = r_u16(state + 0x24 + axis * 2);
        w_u16(state + 0x30 + axis * 2, base + (uint32)((sint32)product >> 12));
    }
    uint32 squares = 0;
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        uint32 delta = (uint32)r_s16(frame + 0x10 + axis * 2);
        squares += delta * delta;
    }
    uint32 length = G3Function8002F448(squares, 0, frame);
    uint32 last_step = 0;
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        sint32 delta = r_s16(frame + 0x10 + axis * 2);
        uint32 speed = (uint32)r_s16(state + 0x18);
        uint32 magnitude = delta < 0 ? 0u - (uint32)delta : (uint32)delta;
        sint32 numerator = (sint32)(magnitude * speed);
        uint32 step = length == 0 ? (numerator < 0 ? 1u : 0xffffffffu) : (uint32)((sint64)numerator / (sint32)length);
        if (axis == 2) last_step = step;
        else w_u16(state + 0x2a + axis * 2, step);
    }
    sint32 step_x = r_s16(state + 0x2a);
    w_u16(state + 0x2e, last_step);
    if (step_x == 0) { uint32 value = r_u16(state + 0x30); w_u16(state + 0x24, value); }
    if (r_s16(state + 0x2c) == 0) { uint32 value = r_u16(state + 0x32); w_u16(state + 0x26, value); }
    if (r_s16(state + 0x2e) == 0) { uint32 value = r_u16(state + 0x34); w_u16(state + 0x28, value); }
}

void G3ShooterCenter(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000644, "G3L_49");
    uint32 frame = guest_stack - 0x18;
    uint32 state = object + 0x104;
    uint32 player = r_u32(game + 0x50);
    sint32 own = r_s16(object + 0x58);
    sint32 other = r_s16(player + 0xdc);
    w_u16(state + 0x24, (uint32)((own + other) >> 1));
    other = r_s16(player + 0xde); own = r_s16(object + 0x5a);
    w_u16(state + 0x26, (uint32)((own + other) >> 1));
    uint32 z = r_u16(player + 0xe0);
    w_u16(state + 0x28, z);
    G3ShooterTrack(object, game, frame);
}

uint32 G3ShooterInside(uint32 object)
{
    FUNCTION_MARKER(0x000006a0, "G3L_49");
    uint32 state = object + 0x104;
    sint32 count = r_s16(state + 0x14);
    if (count <= 0) return 0;
    uint32 box = r_u32(state + 0x10);
    sint32 x = r_s16(state + 0x24);
    for (sint32 index = 0; index < count; ++index, box += 12)
    {
        if (x < r_s16(box) || r_s16(box + 6) < x) continue;
        sint32 y = r_s16(state + 0x26);
        if (y < r_s16(box + 2) || r_s16(box + 8) < y) continue;
        sint32 z = r_s16(state + 0x28);
        if (z < r_s16(box + 4) || r_s16(box + 10) < z) continue;
        return 1;
    }
    return 0;
}

void G3ShooterEffects(uint32 state, uint32 position, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000008f8, "G3L_49");
    uint32 frame = guest_stack - 0x20;
    uint32 resource = r_u32(state + 4);
    if (resource != 0)
    {
        uint32 table = r_u32(resource + 0xc);
        uint32 pool = r_u32(0x800a27fc);
        uint32 entry = r_u32(table);
        G3Function800444E4(position, entry, pool, 90, frame);
    }
    resource = r_u32(state);
    if (resource != 0)
    {
        uint32 table = r_u32(resource + 0xc);
        uint32 pool = r_u32(0x800a27fc);
        uint32 entry = r_u32(table);
        uint32 effect = G3Function800444E4(position, entry, pool, 0xffffffff, frame);
        if (effect != 0)
        {
            uint32 flags = r_u32(effect + 0xc);
            uint32 child = r_u32(effect + 4);
            w_u32(effect + 0xc, flags | 0x8000);
            if (child != 0) { flags = r_u32(child + 0xc); w_u32(child + 0xc, flags | 0x8000); }
        }
    }
}

void G3ShooterActivate(uint32 object, uint32 game, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000f90, "G3L_49");
    uint32 frame = guest_stack - 0x18;
    sint32 command = r_s16(payload);
    uint32 state = object + 0x104;
    if (command == 0) { w_u32(object + 0x150, 1); return; }
    if (command != -1 && command != 1) return;
    uint32 mode = r_u32(object + 0x150);
    if (command == -1 && mode == 0) { w_u32(object + 0x150, 1); return; }
    if (mode != 0)
    {
        w_u32(object + 0x150, 0);
        if (r_s16(payload + 2) == 0)
        {
            G3ShooterTrack(object, game, frame);
            w_u16(state + 0x20, 0);
            return;
        }
    }
    else if (r_s16(payload + 2) < 2) return;
    G3ShooterCenter(object, game, frame);
    w_u16(state + 0x20, 0);
}

void G3ShooterImpact(uint32 object, uint32 hit, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000077c, "G3L_49");
    uint32 frame = guest_stack - 0x68;
    uint32 target = r_u32(hit + 8);
    if (target == 0) return;
    uint32 callback = r_u32(target + 0xec);
    if (callback == 0) return;
    uint32 contact_game = r_u32(0x80097048);
    w_u32(frame + 0x10, 0);
    uint32 kind = r_u16(hit);
    w_u16(frame + 0x16, 4);
    w_u16(frame + 0x14, kind);
    uint32 data = r_u32(hit + 4);
    w_u32(frame + 0x1c, 0);
    w_u32(frame + 0x18, data);
    target = r_u32(hit + 8);
    w_u32(frame + 0x24, object);
    w_u32(frame + 0x20, target);
    uint32 values[2];
    for (uint32 word = 0; word != 2; ++word)
    {
        uint32 source = hit + 0x1c + word * 4;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 lane = source & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
        uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
        values[word] = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
    }
    for (uint32 word = 0; word != 2; ++word)
    {
        uint32 destination = frame + 0x28 + word * 4;
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        uint32 lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, values[word] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, values[word] << (lane * 8), (15u << lane) & 15);
    }
    w_u16(frame + 0x34, 0); w_u16(frame + 0x32, 0); w_u16(frame + 0x30, 0);
    w_u16(frame + 0x3c, 0); w_u16(frame + 0x3a, 0); w_u16(frame + 0x38, 0);
    target = r_u32(hit + 8);
    w_u32(target + 0xb0, frame + 0x10);
    target = r_u32(hit + 8);
    G3DispatchStateEvent(callback, target, contact_game, 3, frame + 0x10, frame);
    sint32 contact_kind = r_s16(frame + 0x14);
    uint32 collision_type = 0;
    if (contact_kind == 5 || contact_kind == 1 || contact_kind == 2)
    {
        data = r_u32(frame + 0x18);
        collision_type = r_u8(data + (contact_kind == 5 ? 3 : contact_kind == 1 ? 5 : 11));
    }
    if (collision_type == 9 || collision_type - 3u < 3)
    {
        w_u32(frame + 0x40, 1);
        w_u16(frame + 0x50, 0); w_u16(frame + 0x52, 0);
        w_u32(frame + 0x54, frame + 0x10);
        target = r_u32(hit + 8);
        callback = r_u32(target + 0xec);
        if (callback != 0)
            G3DispatchStateEvent(callback, target, game, 8, frame + 0x40, frame);
    }
}

void G3ShooterUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000009a4, "G3L_49");
    uint32 frame = guest_stack - 0x88;
    uint32 state = object + 0x104;
    uint32 linked = r_u32(state + 0xc);
    if (linked != 0)
    {
        if ((r_u32(linked + 0x18) & 0x10) != 0)
        {
            G3Function80038798(object, 0, 0, frame);
            return;
        }
        uint32 parent = r_u32(linked + 0x20);
        if (parent != 0)
        {
            for (uint32 axis = 0; axis != 3; ++axis)
            {
                uint32 position = r_u16(parent + 0x58 + axis * 2);
                uint32 offset = r_u16(state + 0x38 + axis * 2);
                w_u16(object + 0x58 + axis * 2, position + offset);
            }
        }
    }
    if (r_u32(object + 0x150) != 0) return;
    if (r_u32(state + 8) != 0)
    {
        uint32 player = r_u32(0x8009efc4);
        uint32 own = r_u16(object + 0x58);
        uint32 other = r_u16(player + 0x58);
        uint32 dx = other - own;
        uint32 x = (uint32)(sint32)(sint16)dx;
        uint32 square_x = x * x;
        w_u16(frame + 0x10, dx);
        other = r_u16(player + 0x5a); own = r_u16(object + 0x5a);
        uint32 dy = other - own;
        uint32 y = (uint32)(sint32)(sint16)dy;
        uint32 square_y = y * y;
        w_u16(frame + 0x14, 0);
        w_u16(frame + 0x12, dy);
        sint32 radius = r_s32(state + 8);
        if ((sint32)(square_x + square_y) >= radius)
        { w_u32(object + 0x150, 1); return; }
    }
    if (r_s16(state + 0x22) > 0)
    {
        uint32 old_timer = r_u16(state + 0x20);
        w_u16(state + 0x20, old_timer + 1);
        sint32 duration = r_s16(state + 0x22);
        if ((sint16)old_timer >= duration)
        { w_u32(object + 0x150, 1); return; }
    }
    uint32 aligned = 0xffffffff;
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        uint32 target = (uint32)r_s16(state + 0x30 + axis * 2);
        uint32 step = (uint32)r_s16(state + 0x2a + axis * 2);
        aligned &= G3Function80038A4C(state + 0x24 + axis * 2, target, step);
    }
    if (aligned != 0) G3ShooterTrack(object, game, frame);
    uint32 timer = r_u16(state + 0x1c) - 1;
    w_u16(state + 0x1c, timer);
    if ((sint16)timer > 0) return;
    uint32 interval = r_u16(state + 0x1e);
    w_u16(state + 0x1c, interval);
    if (G3ShooterInside(object) != 0) return;
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        uint32 position = r_u16(object + 0x58 + axis * 2);
        w_u16(frame + 0x18 + axis * 2, position);
    }
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        uint32 target = r_u16(state + 0x24 + axis * 2);
        uint32 position = r_u16(object + 0x58 + axis * 2);
        w_u16(frame + 0x20 + axis * 2, target - position);
    }
    uint32 spread = r_u16(state + 0x36);
    sint32 divisor = (sint32)(spread << 17) >> 16;
    for (uint32 axis = 0; axis != 2; ++axis)
    {
        uint32 random = G3SdkRandom80027FA8();
        uint32 remainder = divisor == 0 ? random : (uint32)((sint64)(sint32)random % divisor);
        spread = r_u16(state + 0x36);
        uint32 delta = r_u16(frame + 0x20 + axis * 2);
        w_u16(frame + 0x20 + axis * 2, delta + remainder - spread);
    }
    uint32 random = G3SdkRandom80027FA8();
    uint32 remainder = divisor == 0 ? random : (uint32)((sint64)(sint32)random % divisor);
    uint32 x = (uint32)r_s16(frame + 0x20);
    uint32 square_x = x * x;
    uint32 y = (uint32)r_s16(frame + 0x22);
    uint32 square_y = y * y;
    uint32 z_half = r_u16(frame + 0x24);
    spread = r_u16(state + 0x36);
    uint32 z = (uint32)(sint32)(sint16)(z_half + remainder - spread);
    w_u16(frame + 0x24, z_half + remainder - spread);
    uint32 length = G3Function8002F448(square_x + square_y + z * z, 0, frame);
    uint32 components[3];
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        uint32 delta = (uint32)r_s16(frame + 0x20 + axis * 2);
        if (axis != 0) w_u16(frame + 0x1e + axis * 2, components[axis - 1]);
        uint32 distance = (uint32)r_s16(state + 0x16);
        sint32 numerator = (sint32)(delta * distance);
        components[axis] = length == 0 ? (numerator < 0 ? 1u : 0xffffffffu) : (uint32)((sint64)numerator / (sint32)length);
    }
    w_u16(frame + 0x24, components[2]);
    uint32 position = r_u16(object + 0x58);
    w_u16(frame + 0x20, components[0] + position);
    position = r_u16(object + 0x5a);
    w_u16(frame + 0x22, components[1] + position);
    position = r_u16(object + 0x5c);
    w_u32(frame + 0x40, frame + 0x18);
    w_u32(frame + 0x3c, frame + 0x20);
    w_u32(frame + 0x28, 0);
    w_u32(frame + 0x50, object);
    w_u32(frame + 0x4c, 0x11);
    w_u16(frame + 0x24, components[2] + position);
    if (r_s16(state + 0x3e) != 0) w_u32(frame + 0x4c, 0x13);
    if (r_s16(state + 0x42) != 0)
    {
        uint32 flags = r_u32(frame + 0x4c);
        w_u32(frame + 0x4c, flags | 4);
    }
    linked = r_u32(state + 0xc);
    uint32 parent = linked == 0 ? 0 : r_u32(linked + 0x20);
    if (parent != 0)
    {
        uint32 flags = r_u32(parent + 0x14);
        uint32 saved_x = r_u32(parent + 0x144);
        uint32 saved_y = r_u32(parent + 0x148);
        uint32 saved_z = r_u32(parent + 0x14c);
        w_u32(parent + 0x14, flags | 0x20000000);
        G3Function80061CA4(frame + 0x28, game, frame);
        if ((flags & 0x20000000) == 0)
        { uint32 fresh = r_u32(parent + 0x14); w_u32(parent + 0x14, fresh & 0xdfffffff); }
        w_u32(parent + 0x144, saved_x); w_u32(parent + 0x148, saved_y); w_u32(parent + 0x14c, saved_z);
    }
    else G3Function80061CA4(frame + 0x28, game, frame);
    uint32 player = r_u32(0x8009efc4);
    G3Function8005FC70(frame + 0x58, frame + 0x18, frame + 0x20, player + 0x58);
    uint32 squares = 0;
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        player = r_u32(game + 0x50);
        uint32 projected = r_u16(frame + 0x58 + axis * 2);
        uint32 coordinate = r_u16(player + 0x58 + axis * 2);
        uint32 delta = projected - coordinate;
        uint32 signed_delta = (uint32)(sint32)(sint16)delta;
        squares += signed_delta * signed_delta;
        w_u16(frame + 0x58 + axis * 2, delta);
    }
    if ((sint32)squares < 10001) G3Function80074044(object, game, frame);
    uint32 material = r_u32(frame + 0x30);
    uint32 special = 0;
    if (material != 0)
    { uint32 data = r_u32(material + 0x1c); special = r_u32(data) & 1; }
    sint32 damage = r_s16(state + (special != 0 ? 0x44 : 0x40));
    uint32 kind = r_u32(frame + 0x28);
    if (kind >= 6) return;
    uint32 target = r_u32(module_base + 0x11b4 + kind * 4) - module_base;
    switch (target)
    {
    case 0xf30: G3ShooterEffects(state, frame + 0x20, frame); return;
    case 0xf44:
        G3ShooterEffects(state, frame + 0x20, frame);
        if (damage != 0) G3ShooterImpact(object, frame + 0x28, game, frame);
        return;
    case 0xf64: return;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "Unresolved Shooter hit target %08x\n", (unsigned int)(target + module_base));
#endif
        }
        abort();
    }
}
