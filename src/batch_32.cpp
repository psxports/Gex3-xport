#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <string.h>
#include <stdio.h>

uint32 G3Function8007E758(uint32 object, uint32 linked, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007e758, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
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
        return G3Function8007EC48(object, linked, frame);
    return G3Function8007E92C(object, linked, frame);
}

uint32 G3Function8007EFF8(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007eff8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 linked = r_u32(state + 0x124);
    if (G3Function8007E758(object, linked, frame) != 0)
    {
        uint32 flags = r_u32(object + 0x10);
        uint32 mode = r_u32(object + 0x128);
        uint32 result = flags | 0x100;
        w_u32(object + 0x128, mode | 1);
        w_u32(object + 0x10, result);
        result |= 0x400000;
        w_u32(object + 0x10, result);
        return result;
    }
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x10, flags & 0xfffffeff);
    uint32 mode = r_u32(object + 0x128);
    flags = r_u32(object + 0x10);
    w_u32(object + 0x128, mode & 0xfffffffe);
    w_u32(object + 0x10, flags & 0xffbfffff);
    linked = r_u32(state + 0x124);
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
    {
        uint32 target = r_u32(linked + 0xec);
        if (target != 0)
        {
            G3DispatchStateEvent(target, linked, game, 0x3e8, frame + 0x10, frame);
            target = r_u32(linked + 0xec);
            if (target != 0)
                G3DispatchStateEvent(target, linked, game, 0x3e9, frame + 0x10, frame);
        }
        uint32 payload = r_u32(frame + 0x10);
        if (payload != 0)
            G3Function80018DE0(linked, payload + 4, frame);
    }
    else
    {
        linked = r_u32(state + 0x124);
        uint32 descriptor = r_u32(linked + 0x28);
        if (descriptor != 0)
        {
            uint32 kind = r_u32(descriptor);
            if (kind != 0 && kind < 0xb)
            {
                uint32 sequence = r_u32(descriptor + 8);
                G3Function80018DE0(linked, sequence + 4, frame);
            }
        }
    }
    w_u32(object + 0x154, 1);
    w_u16(object + 0x6e, 0x13);
    w_u16(object + 0x5e, 0);
    w_u16(object + 0x60, 0);
    w_u16(object + 0x62, 0);
    w_u16(object + 0x64, 0);
    return 0x13;
}

uint32 G3Function8007F1A4(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007f1a4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 state = r_u32(object + 0x24);
    uint32 linked = r_u32(state + 0x124);
    if (G3Function8007E6B0(object, linked, frame) == 0)
        return 0x10000;
    w_u32(object + 0x154, 0x10000);
    uint32 flags = r_u32(object + 0x10);
    uint32 mode = r_u32(object + 0x128);
    w_u32(object + 0x10c, 0);
    w_u32(object + 0x108, 0);
    w_u32(object + 0x104, 0);
    w_u32(object + 0x118, 0);
    w_u32(object + 0x114, 0);
    w_u32(object + 0x110, 0);
    uint32 result = flags | 0x100;
    w_u32(object + 0x10, result);
    w_u32(object + 0x128, mode | 1);
    return result;
}

uint32 G3Function8007B618(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007b618, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 buttons = r_u32(controls);
    uint32 step = r_u16(state + 0x7c);
    if ((buttons & 0x800) != 0)
        step <<= 1;
    if ((buttons & 4) != 0)
    {
        uint32 angle = r_u16(state + 0xc8);
        w_u16(state + 0xc8, angle + step);
    }
    else if ((buttons & 8) != 0)
    {
        uint32 angle = r_u16(state + 0xc8);
        w_u16(state + 0xc8, angle - step);
    }
    bool changed = false;
    if ((r_u32(controls) & 1) != 0)
    {
        uint32 pointer = r_u32(state);
        sint32 angle = r_s16(pointer);
        uint32 raw = r_u16(pointer);
        if (angle < 0x400)
        {
            w_u16(pointer, raw + step);
            changed = true;
        }
    }
    if (!changed && (r_u32(controls) & 2) != 0)
    {
        uint32 pointer = r_u32(state);
        sint32 angle = r_s16(pointer);
        uint32 raw = r_u16(pointer);
        if (angle >= -0x3ff)
        {
            w_u16(pointer, raw - step);
            changed = true;
        }
    }
    if (!changed && (r_u32(controls) & 3) == 0)
    {
        uint32 pointer = r_u32(state);
        uint32 neutral_step = (uint32)r_s16(state + 0x7c);
        G3Function80038AA8(pointer, 0, neutral_step);
        pointer = r_u32(state);
        uint32 raw = r_u16(pointer);
        w_u16(pointer, raw & 0xfff);
        pointer = r_u32(state);
        sint32 angle = r_s16(pointer);
        raw = r_u16(pointer);
        if (angle >= 0x800)
            w_u16(pointer, raw - 0x1000);
    }
    uint32 result = r_u16(state + 0xc8);
    uint32 pointer = r_u32(state);
    w_u16(state + 0xc6, result);
    w_u16(pointer + 2, 0);
    return result;
}

uint32 G3Function8007B75C(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007b75c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    memset(psx_addr(frame + 0x10, 8), 0, 8);
    memset(psx_addr(frame + 0x18, 8), 0, 8);
    uint32 thresholds = r_u32(state + 0x84);
    sint32 counter = r_s16(state + 0xe2);
    sint32 threshold = r_s16(thresholds);
    uint32 index = 0;
    if (counter >= threshold)
    {
        do
        {
            index += 1;
            uint32 offset = (uint32)((sint32)(index << 16) >> 15);
            threshold = r_s16(thresholds + offset);
        } while (counter >= threshold);
    }
    uint32 speeds = r_u32(state + 0x80);
    uint32 offset = (uint32)((sint32)(index << 16) >> 15);
    uint32 speed = 0 - r_u16(speeds + offset);
    uint32 current_game = r_u32(0x80097048);
    w_u16(frame + 0x12, speed);
    if (r_u16(current_game + 0x68b4) == 0x13)
        w_u16(frame + 0x12, speed << 1);
    w_u16(state + 0xe0, index);
    G3Function8002F718(frame + 0x20);
    uint32 pointer = r_u32(state);
    uint32 angle = (uint32)r_s16(pointer);
    G3Function8005B94C(angle, frame + 0x20, frame);
    angle = (uint32)r_s16(state + 0xc8);
    G3Function8005BB6C(angle, frame + 0x20, frame);
    ApplyMatrixSV((MATRIX *)psx_addr(frame + 0x20, 20), (SVECTOR *)psx_addr(frame + 0x10, 8), (SVECTOR *)psx_addr(frame + 0x18, 6));
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 value = r_u16(frame + 0x18 + axis * 2);
        uint32 velocity = r_u16(state + 0x9c + axis * 2);
        w_u16(frame + 0x18 + axis * 2, value + velocity);
    }
    sint32 value = r_s16(state + 0x9c);
    sint32 factor = r_s16(state + 0xa4);
    uint32 first = G3Function80039DBC(value, factor);
    value = r_s16(state + 0x9e);
    factor = r_s16(state + 0xa4);
    w_u16(state + 0x9c, first);
    uint32 second = G3Function80039DBC(value, factor);
    value = r_s16(state + 0xa0);
    factor = r_s16(state + 0xa4);
    w_u16(state + 0x9e, second);
    uint32 third = G3Function80039DBC(value, factor);
    w_u16(state + 0xa0, third);
    uint32 result = 0;
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 position = r_u16(object + 0x58 + axis * 2);
        uint32 delta = r_u16(frame + 0x18 + axis * 2);
        result = position + delta;
        w_u16(object + 0x58 + axis * 2, result);
    }
    return result;
}

uint32 G3Function8007B918(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007b918, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    memset(psx_addr(frame + 0x10, 8), 0, 8);
    memset(psx_addr(frame + 0x18, 8), 0, 8);
    uint32 thresholds = r_u32(state + 0x94);
    sint32 counter = r_s16(state + 0xe2);
    sint32 threshold = r_s16(thresholds);
    uint32 index = 0;
    if (counter >= threshold)
    {
        do
        {
            index += 1;
            uint32 offset = (uint32)((sint32)(index << 16) >> 15);
            threshold = r_s16(thresholds + offset);
        } while (counter >= threshold);
    }
    uint32 speeds = r_u32(state + 0x90);
    uint32 offset = (uint32)((sint32)(index << 16) >> 15);
    uint32 speed = 0 - r_u16(speeds + offset);
    uint32 current_game = r_u32(0x80097048);
    w_u16(frame + 0x12, speed);
    if (r_u16(current_game + 0x68b4) == 0x13)
        w_u16(frame + 0x12, speed << 1);
    w_u16(state + 0xe0, index);
    G3Function8002F718(frame + 0x20);
    uint32 angle = (uint32)r_s16(state + 0xc8);
    G3Function8005BB6C(angle, frame + 0x20, frame);
    ApplyMatrixSV((MATRIX *)psx_addr(frame + 0x20, 20), (SVECTOR *)psx_addr(frame + 0x10, 8), (SVECTOR *)psx_addr(frame + 0x18, 6));
    uint32 result = 0;
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 position = r_u16(object + 0x58 + axis * 2);
        uint32 delta = r_u16(frame + 0x18 + axis * 2);
        result = position + delta;
        w_u16(object + 0x58 + axis * 2, result);
    }
    return result;
}

uint32 G3Function80080544(uint32 object, uint32 game, uint32 state, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80080544, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 linked = r_u32(state + 0x1fc);
    if (linked != 0)
    {
        uint32 target = r_u32(linked + 0xec);
        if (target == 0)
            return target;
        return G3DispatchStateEvent(target, linked, game, 0x3f2, command, frame);
    }
    uint32 value = r_u16(command + 2);
    w_u16(state + 0x25a, value);
    value = r_u16(command + 6);
    w_u16(state + 0x256, value);
    uint32 current = r_u16(state + 0x258);
    uint32 increment = r_u16(command);
    uint32 next = current + increment;
    w_u16(state + 0x258, next);
    sint32 limit = r_s16(command + 2);
    uint32 raw_limit = r_u16(command + 2);
    if (limit < (sint16)next)
        w_u16(state + 0x258, raw_limit);
    sint32 clamped = r_s16(state + 0x258);
    sint32 threshold = r_s16(command + 4);
    if (threshold >= clamped)
        return 0;
    uint32 result = r_u32(game + 0xe8) & 0x40000;
    if (result == 0)
        return result;
    return G3Function80073630(object, game, frame);
}

uint32 G3Function8007C4AC(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007c4ac, "SLUS_008.06");
    uint32 frame = guest_stack - 0x58;
    r_masked_u32(0x800a030c, 15);
    uint32 low = r_masked_u32(0x800a030c, 15);
    uint32 destination = frame + 0x30;
    uint32 high_address = destination + 3;
    uint32 high_lane = high_address & 3;
    uint32 low_lane = destination & 3;
    w_masked_u32(high_address & 0xfffffffc, low >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    w_masked_u32(destination & 0xfffffffc, low << (low_lane * 8), (15 << low_lane) & 15);
    memset(psx_addr(frame + 0x38, 4), 0, 4);
    sint32 count = r_s16(state + 0x252) + 1;
    sint32 denominator = r_s16(state + 0xb0);
    sint32 numerator = count * 120;
    uint32 quotient = denominator == 0 ? (numerator < 0 ? 1 : 0xffffffff) : (uint32)(numerator / denominator);
    uint32 top = (uint32)((sint32)((0xb4 - quotient) << 16) >> 16);
    uint32 game = r_u32(0x80097048);
    w_u32(frame + 0x14, 0x1e0);
    w_u32(frame + 0x18, 0xb4);
    w_u32(frame + 0x1c, 0x1e8);
    w_u32(frame + 0x20, 0xb4);
    w_u32(frame + 0x10, top);
    uint32 pool = r_u32(game + 0x68);
    w_u32(frame + 0x24, pool);
    uint32 order = r_u32(game + 0x38) + 8;
    w_u32(frame + 0x28, order);
    G3Function8001E140(frame + 0x30, 0x1e0, top, 0x1e8, top, 0x1e0, 0xb4, 0x1e8, 0xb4, pool, order);
    game = r_u32(0x80097048);
    w_u32(frame + 0x10, 0x3c);
    w_u32(frame + 0x14, 0x1e0);
    w_u32(frame + 0x18, 0xb4);
    w_u32(frame + 0x1c, 0x1e8);
    w_u32(frame + 0x20, 0xb4);
    pool = r_u32(game + 0x68);
    w_u32(frame + 0x24, pool);
    order = r_u32(game + 0x38) + 8;
    w_u32(frame + 0x28, order);
    return G3Function8001E140(frame + 0x38, 0x1e0, 0x3c, 0x1e8, 0x3c, 0x1e0, 0xb4, 0x1e8, 0xb4, pool, order);
}

uint32 G3Function800708FC(uint32 object, uint32 level, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800708fc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    uint32 player = r_u32(0x8009efc4);
    w_u32(frame + 0x34, 0x5f);
    w_u32(frame + 0x38, player);
    for (uint32 index = 0; index < 4; ++index)
    {
        uint32 record = object + 0x138 + index * 0x34;
        w_u32(frame + 0x24, record + 0x24);
        uint32 second = object + 0x68 + index * 0x34 + 0x24;
        uint32 game = r_u32(0x80097048);
        w_u32(frame + 0x28, second);
        G3Function80061CA4(frame + 0x10, game, frame);
        uint32 kind = r_u32(frame + 0x10);
        if (kind == 3)
        {
            uint32 hit = r_u32(frame + 0x14);
            w_u32(record + 0xc, hit);
            hit = r_u32(frame + 0x14);
            uint32 normals = r_u32(level + 0x40);
            uint32 normal = (uint32)r_s16(hit + 8);
            G3Function8005F304(normal, normals, record + 0x14, frame);
        }
        else if (kind == 2 || kind == 5)
        {
            uint32 hit = kind == 2 ? r_u32(frame + 0x14) : 0x800a0288;
            w_u32(record + 0xc, hit);
            uint32 copied[2];
            for (uint32 word = 0; word < 2; ++word)
            {
                uint32 source = frame + 0x1c + word * 4;
                uint32 high_address = source + 3;
                uint32 high_lane = high_address & 3;
                uint32 low_lane = source & 3;
                uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
                copied[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
            }
            for (uint32 word = 0; word < 2; ++word)
            {
                uint32 destination = record + 0x14 + word * 4;
                uint32 high_address = destination + 3;
                uint32 high_lane = high_address & 3;
                uint32 low_lane = destination & 3;
                w_masked_u32(high_address & 0xfffffffc, copied[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                w_masked_u32(destination & 0xfffffffc, copied[word] << (low_lane * 8), (15 << low_lane) & 15);
            }
        }
        else if (kind == 0)
        {
            w_u32(record + 0xc, 0);
            w_u16(record + 0x14, 0);
            w_u16(record + 0x16, 0);
            w_u16(record + 0x18, 0);
        }
    }
    uint32 cursor = object + 0x208;
    uint32 offset = 0x138;
    do
    {
        uint32 record = cursor - 0xd0;
        if (r_u32(record + 0xc) == 0)
        {
            uint32 game = r_u32(0x80097048);
            uint32 query_record = object + offset;
            w_u32(frame + 0x24, cursor + 0x24);
            w_u32(frame + 0x28, query_record + 0x2c);
            G3Function80061CA4(frame + 0x10, game, frame);
            uint32 kind = r_u32(frame + 0x10);
            if (kind == 3)
            {
                uint32 hit = r_u32(frame + 0x14);
                w_u32(record + 0xc, hit);
                hit = r_u32(frame + 0x14);
                uint32 normals = r_u32(level + 0x40);
                uint32 normal = (uint32)r_s16(hit + 8);
                G3Function8005F304(normal, normals, query_record + 0x14, frame);
            }
            else if (kind == 2 || kind == 5)
            {
                uint32 hit = kind == 2 ? r_u32(frame + 0x14) : 0x800a0288;
                w_u32(record + 0xc, hit);
                uint32 copied[2];
                for (uint32 word = 0; word < 2; ++word)
                {
                    uint32 source = frame + 0x1c + word * 4;
                    uint32 high_address = source + 3;
                    uint32 high_lane = high_address & 3;
                    uint32 low_lane = source & 3;
                    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                    uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
                    copied[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
                }
                for (uint32 word = 0; word < 2; ++word)
                {
                    uint32 destination = record + 0x14 + word * 4;
                    uint32 high_address = destination + 3;
                    uint32 high_lane = high_address & 3;
                    uint32 low_lane = destination & 3;
                    w_masked_u32(high_address & 0xfffffffc, copied[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                    w_masked_u32(destination & 0xfffffffc, copied[word] << (low_lane * 8), (15 << low_lane) & 15);
                }
            }
            else if (kind == 0)
            {
                w_u32(record + 0xc, 0);
                w_u16(record + 0x14, 0);
                w_u16(record + 0x16, 0);
                w_u16(record + 0x18, 0);
            }
            for (uint32 axis = 0; axis < 3; ++axis)
            {
                uint32 position = r_u16(cursor + 0x24 + axis * 2);
                w_u16(record + 0x24 + axis * 2, position);
            }
        }
        cursor += 0x34;
        offset += 0x34;
    } while ((sint32)cursor < (sint32)(object + 0x2d8));
    return 0;
}

uint32 G3Function80070524(uint32 object, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80070524, "SLUS_008.06");
    uint32 steering = r_u32(0x800a0284);
    uint32 frame = guest_stack - 0x18;
    uint32 result = r_u32(steering + 0xc);
    if (result != 0)
        return result;
    result = r_u32(state + 0x144);
    if (result == 0)
        return result;
    result = r_u32(state + 0x178);
    if (result == 0)
        return result;
    uint32 first = r_u16(state + 0x190);
    uint32 second = r_u16(state + 0x15c);
    uint32 dx = first - second;
    w_u16(frame, dx);
    sint32 x = (sint16)dx;
    sint32 absolute_x = x < 0 ? -x : x;
    first = r_u16(state + 0x192);
    second = r_u16(state + 0x15e);
    uint32 dy = first - second;
    w_u16(frame + 2, dy);
    sint32 y = (sint16)dy;
    sint32 absolute_y = y < 0 ? -y : y;
    first = r_u16(state + 0x194);
    second = r_u16(state + 0x160);
    uint32 dz = first - second;
    sint32 z = (sint16)dz;
    sint32 absolute_z = z < 0 ? -z : z;
    sint32 ax = (sint16)absolute_x;
    sint32 ay = (sint16)absolute_y;
    sint32 az = (sint16)absolute_z;
    w_u16(frame + 4, dz);
    sint32 smallest = ax;
    sint32 middle = ay;
    sint32 largest = az;
    if (ax < ay)
    {
        if (az < ax)
        {
            smallest = az;
            middle = ax;
            largest = ay;
        }
        else if (az < ay)
        {
            middle = az;
            largest = ay;
        }
    }
    else if (az < ay)
    {
        smallest = az;
        middle = ay;
        largest = ax;
    }
    else if (az < ax)
    {
        smallest = ay;
        middle = az;
        largest = ax;
    }
    else
    {
        smallest = ay;
        middle = ax;
    }
    sint32 weighted = largest * 30 + middle * 12 + smallest * 9;
    sint32 divisor = (weighted >= 0 ? weighted : weighted + 7) >> 3;
    if (divisor != 0)
    {
        sint32 normalized_x = r_s16(frame) * 16384 / divisor;
        uint32 raw_y = r_u16(frame + 2);
        sint32 normalized_y = ((sint32)(raw_y << 16) >> 2) / divisor;
        uint32 raw_z = r_u16(frame + 4);
        sint32 normalized_z = ((sint32)(raw_z << 16) >> 2) / divisor;
        w_u16(frame, (uint32)normalized_x);
        w_u16(frame + 2, (uint32)normalized_y);
        w_u16(frame + 4, (uint32)normalized_z);
    }
    uint32 direction_z = r_u16(frame + 4);
    uint32 normal_x = r_u16(state + 0x14c);
    w_u16(frame + 8, normal_x);
    uint32 normal_y = r_u16(state + 0x14e);
    w_u16(frame + 0xa, normal_y);
    uint32 normal_z = r_u16(state + 0x150);
    uint32 direction_y = r_u16(frame + 2);
    sint32 direction_x = r_s16(frame);
    sint32 nx = (sint16)normal_x;
    sint32 ny = (sint16)normal_y;
    sint32 nz = (sint16)normal_z;
    sint32 vy = (sint16)direction_y;
    sint32 vz = (sint16)direction_z;
    uint32 cross_z = (uint32)(nx * vy) - (uint32)(ny * direction_x);
    cross_z = (uint32)((sint32)cross_z >> 12);
    uint32 cross_x = (uint32)(ny * vz) - (uint32)(nz * vy);
    cross_x = (uint32)((sint32)cross_x >> 12);
    uint32 cross_y = (uint32)(nx * vz) - (uint32)(nz * direction_x);
    cross_y = 0 - (uint32)((sint32)cross_y >> 12);
    w_u16(frame + 0xc, normal_z);
    w_u16(frame + 0x14, cross_z);
    w_u16(frame + 0x10, cross_x);
    w_u16(frame + 0x12, cross_y);
    sint32 left_x = r_s16(state + 0x15c);
    sint32 right_x = r_s16(state + 0x190);
    uint32 raw_x = r_u16(state + 0x190);
    uint32 midpoint_x = raw_x + (uint32)((left_x - right_x) >> 1);
    w_u16(frame, midpoint_x);
    sint32 left_y = r_s16(state + 0x15e);
    sint32 right_y = r_s16(state + 0x192);
    uint32 raw_y = r_u16(state + 0x192);
    w_u16(frame + 2, raw_y + (uint32)((left_y - right_y) >> 1));
    sint32 left_z = r_s16(state + 0x160);
    sint32 right_z = r_s16(state + 0x194);
    uint32 raw_z = r_u16(state + 0x194);
    w_u16(frame + 4, raw_z + (uint32)((left_z - right_z) >> 1));
    sint32 component = (sint16)cross_x;
    sint64 product = (sint64)component * 0x2e8ba2e9;
    sint32 high = (sint32)(product >> 32);
    sint32 offset = (high >> 3) - (component >> 31);
    uint32 player = r_u32(0x8009efc4);
    w_u16(player + 0x58, midpoint_x + (uint32)offset);
    uint32 raw_component = r_u16(frame + 0x12);
    component = (sint16)raw_component;
    product = (sint64)component * 0x2e8ba2e9;
    high = (sint32)(product >> 32);
    offset = (high >> 3) - (component >> 31);
    uint32 midpoint_y = r_u16(frame + 2);
    w_u16(player + 0x5a, midpoint_y + (uint32)offset);
    raw_component = r_u16(frame + 0x14);
    component = (sint16)raw_component;
    product = (sint64)component * 0x2e8ba2e9;
    high = (sint32)(product >> 32);
    offset = (high >> 3) - (component >> 31);
    uint32 midpoint_z = r_u16(frame + 4);
    steering = r_u32(0x800a0284);
    w_u16(player + 0x5c, midpoint_z + (uint32)offset);
    result = r_u32(state + 0x144);
    w_u32(steering + 0xc, result);
    uint32 copied[2];
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 source = state + 0x14c + word * 4;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 low_lane = source & 3;
        uint32 high_word = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low_word = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
        copied[word] = low_lane == 0 ? low_word : (high_word << ((3 - high_lane) * 8)) | (low_word >> (low_lane * 8));
    }
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 destination = steering + 0x14 + word * 4;
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        uint32 low_lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, copied[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, copied[word] << (low_lane * 8), (15 << low_lane) & 15);
    }
    return result;
}

void G3Function8007DE64(uint32 object, uint32 game, uint32 output, uint32 argument_3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007de64, "SLUS_008.06");
    uint32 state = r_u32(object + 0x24);
    if (output == 0)
        return;
    if (r_u32(state + 0x230) != 0)
    {
        uint32 value = r_u16(state + 0x22a);
        w_u16(output + 2, 0);
        w_u16(output, value);
        value = r_u16(state + 0x1b6);
        w_u16(output + 4, value);
    }
    else
    {
        w_u16(output, 0);
        w_u16(output + 2, 0);
        w_u16(output + 4, 0);
    }
}

uint32 G3Function80045F40(uint32 owner)
{
    FUNCTION_MARKER(0x80045f40, "SLUS_008.06");
    uint32 record = r_u32(owner + 0x14);
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    sint32 count = r_s32(level + 0x7c);
    uint32 target = G3Function80039B78(0x8009e044);
    if (count <= 0)
        return target;
    uint32 flags_address = record + 0x18;
    sint32 index = 0;
    do
    {
        uint32 descriptor = r_u32(record);
        if (descriptor != 0 && r_u32(descriptor + 0x34) == target)
        {
            uint32 flags = r_u32(flags_address);
            flags &= 0xffffffef;
            flags &= 0xffffff7f;
            w_u32(flags_address, flags);
        }
        flags_address += 0x34;
        ++index;
        record += 0x34;
    } while (index < count);
    return 0;
}

uint32 G3Function800283E8(uint32 controller, uint32 negative_x, uint32 positive_x, uint32 negative_y, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800283e8, "SLUS_008.06");
    uint32 positive_y = r_u32(guest_stack + 0x10);
    w_u32(positive_y, 0);
    w_u32(negative_y, 0);
    w_u32(positive_x, 0);
    w_u32(negative_x, 0);
    uint32 result = r_u32(controller + 0x70) & 0xc00;
    if (result != 0)
        return result;
    result = r_s32(controller + 0x80) < -64;
    w_u32(negative_x, result);
    result = r_s32(controller + 0x80) >= 65;
    w_u32(positive_x, result);
    result = r_s32(controller + 0x7c) < -64;
    w_u32(negative_y, result);
    uint32 last = r_s32(controller + 0x7c) >= 65;
    w_u32(positive_y, last);
    uint32 sum = r_u32(negative_x);
    uint32 right = r_u32(positive_x);
    uint32 up = r_u32(negative_y);
    sum += right;
    sum += up;
    sum += last;
    result = (sint32)sum < 2;
    if (result == 0)
    {
        w_u32(positive_y, 0);
        w_u32(negative_y, 0);
        w_u32(positive_x, 0);
        w_u32(negative_x, 0);
    }
    return result;
}

uint32 G3Function8007BAB8(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007bab8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 flags = r_u16(state + 0xce);
    w_u16(state + 0xce, flags | 1);
    G3Function8007F3C0(object, game, state, frame);
    uint32 controls = 0x800a0d28;
    if ((r_u32(object + 0x10) & 0x100) == 0)
        controls = game + 0x70;
    flags = r_u32(object + 0x14);
    w_u32(object + 0x14, flags | 0x40);
    G3Function8007FE80(object, game, state);
    G3Function8007B918(object, game, state, frame);
    uint32 heading = r_u16(state + 0xc6);
    uint32 contact = r_u32(object + 0xf8);
    w_u16(object + 0x64, heading);
    bool reset = false;
    if (contact != 0 && (r_u32(object + 0x18) & 1) == 0)
    {
        sint32 position = r_s16(object + 0x100);
        sint32 offset = r_s16(state + 0x8a);
        sint32 limit = r_s16(object + 0xe0);
        reset = position - offset < limit;
    }
    if (!reset)
        reset = (r_u32(object + 0x134) & 0x100) != 0;
    if (reset)
    {
        w_u32(object + 0x150, 0);
        w_u32(object + 0x154, 1);
        w_u16(object + 0x6e, 0);
        w_u16(object + 0x5e, 0);
        w_u32(object + 0x10c, 0);
        uint32 source = object + 0xdc;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 low_lane = source & 3;
        uint32 high_word = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low_word = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
        uint32 copied = low_lane == 0 ? low_word : (high_word << ((3 - high_lane) * 8)) | (low_word >> (low_lane * 8));
        uint32 z = (uint32)(sint32)r_s16(object + 0xe0);
        uint32 destination = object + 0x58;
        high_address = destination + 3;
        high_lane = high_address & 3;
        low_lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, copied >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, copied << (low_lane * 8), (15 << low_lane) & 15);
        w_u16(object + 0x5c, z);
        flags = r_u32(object + 0x134);
        w_u32(object + 0x134, flags & 0xfffffeff);
    }
    bool engage = false;
    if ((r_u32(controls + 4) & 0x80) != 0)
    {
        sint32 position = r_s16(object + 0x100);
        sint32 offset = r_s16(state + 0x8a);
        sint32 current = r_s16(object + 0x5c);
        sint32 distance = current - (position - offset);
        if (distance < 0)
            distance = (position - offset) - current;
        engage = distance < 20;
    }
    if (engage)
    {
        uint32 raw_index = r_u16(state + 0x20);
        uint32 thresholds = r_u32(state + 0x94);
        sint32 index = (sint32)(raw_index << 16) >> 17;
        w_u16(state + 0xe0, (uint32)index);
        uint32 threshold = r_u16(thresholds + (uint32)(index * 2));
        w_u16(state + 0xe2, threshold - 1);
        index = r_s16(state + 0xe0);
        uint32 speeds = r_u32(state + 0x98);
        uint32 speed = (uint32)(sint32)r_s16(speeds + (uint32)(index * 2));
        w_u32(object + 0x150, 0);
        w_u32(object + 0x10c, speed);
        G3Function80078740(object, game, state, frame);
    }
    else if ((r_u32(controls + 4) & 0x800) != 0)
    {
        contact = r_u32(object + 0xf8);
        if (contact != 0)
        {
            uint32 surface = r_u16(contact + 6);
            uint32 level = r_u32(game + 0x48);
            uint32 materials = r_u32(level + 0xf0);
            uint32 material = r_u32(materials + ((surface & 0x1e0) >> 1));
            if ((material & 0x400) == 0)
            {
                w_u32(object + 0x150, 3);
                w_u32(object + 0x154, 1);
                w_u16(object + 0x6e, 0x35);
                w_u16(object + 0x5e, 0);
                uint32 speed = (uint32)(sint32)r_s16(state + 0x7e);
                uint32 position = r_u16(object + 0x100);
                w_u32(object + 0x10c, speed);
                w_u16(state + 0x228, position);
                uint32 camera = r_u32(game + 0x4c);
                if (G3Function8004DABC(camera, frame) != 2)
                {
                    camera = r_u32(game + 0x4c);
                    G3Function80013CDC(camera, 0, frame);
                    camera = r_u32(game + 0x4c);
                    G3Function80014220(camera, 0x100);
                }
                uint32 position_key = (uint32)(sint32)r_s16(state + 0x228);
                G3Function8005820C(1, position_key);
                uint32 duration = r_u16(state + 0xb0);
                w_u16(state + 0x9c, 0);
                w_u16(state + 0x9e, 0);
                w_u16(state + 0xa0, 0);
                w_u16(state + 0x252, duration);
            }
        }
    }
    if (r_u32(object + 0x150) != 2)
        return 2;
    if (r_u32(object + 0x154) == 2)
    {
        w_u16(object + 0x6e, 0x3a);
        G3Function8007CF04(object, game, state, controls, frame);
    }
    if (r_u32(object + 0x154) == 1)
    {
        w_u16(object + 0x6e, 0x39);
        if (r_s16(state + 0xbc) != 0 && r_s16(game + 0x6786) == 0)
        {
            w_u32(object + 0x154, 2);
            w_u16(object + 0x6e, 0x3a);
            w_u16(object + 0x5e, 0);
        }
    }
    sint32 previous = r_s16(state + 0x26c);
    uint32 acceleration = (uint32)(sint32)r_s16(state + 0x8c);
    uint32 velocity = r_u32(object + 0x10c);
    sint32 captured_offset = r_s16(state + 0x8a);
    velocity += acceleration;
    w_u32(object + 0x10c, velocity);
    uint32 raw_height = r_u16(object + 0x5c);
    uint32 low_velocity = r_u16(object + 0x10c);
    sint32 position = r_s16(object + 0x100);
    uint32 new_height = raw_height + low_velocity;
    bool previously_below = position - captured_offset < previous;
    w_u16(object + 0x5c, new_height);
    position = r_s16(object + 0x100);
    sint32 offset = r_s16(state + 0x8a);
    sint32 signed_height = (sint32)(new_height << 16) >> 16;
    bool overshoot = position - offset < signed_height;
    bool clamp = overshoot && !previously_below;
    if (!clamp)
    {
        sint32 speed = r_s32(object + 0x10c);
        clamp = speed >= 0 && overshoot;
    }
    if (clamp)
    {
        uint32 raw_position = r_u16(object + 0x100);
        uint32 raw_offset = r_u16(state + 0x8a);
        w_u32(object + 0x10c, 0);
        w_u16(object + 0x5c, raw_position - raw_offset);
    }
    raw_height = r_u16(object + 0x5c);
    w_u16(state + 0x26c, raw_height);
    position = r_s16(object + 0x100);
    sint32 height = r_s16(object + 0x5c);
    uint32 linked = r_u32(state + 0xf8);
    sint32 key = position - height;
    uint32 animation = r_u32(linked + 0xc);
    offset = r_s16(state + 0x8a);
    sint32 additional = r_s16(state + 0x18a);
    key -= offset;
    key += additional;
    G3Function8006F34C(animation, (uint32)key, 0x8c);
    return (uint32)offset;
}

uint32 G3Function8007BE98(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007be98, "SLUS_008.06");
    uint32 current_game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x38;
    uint32 camera = r_u32(current_game + 0x4c);
    if (G3Function8004DACC() == 0 && (r_u32(game + 0x69b8) & 0x10000) == 0)
    {
        current_game = r_u32(0x80097048);
        uint32 level = r_u16(current_game + 0x68b4);
        if (level != 0x13 || (r_u32(current_game + 0x14c) & 1) != 0)
        {
            uint32 remaining = r_u16(state + 0x252);
            w_u16(state + 0x252, remaining - 1);
        }
        if (r_s16(state + 0x252) <= 0)
        {
            w_u16(state + 0x252, 0);
            if ((r_u32(game + 0xe8) & 0x40000) != 0)
            {
                uint32 player = r_u32(game + 0x50);
                return G3Function80073630(player, game, frame);
            }
        }
    }
    if (r_u16(object + 0x6e) != 0x35)
    {
        if ((r_u32(game + 0x69b8) & 0x10000) == 0)
            G3Function8007C4AC(state, frame);
        G3Function8007B618(object, game, state, controls, frame);
        controls = 0x800a0d28;
        if ((r_u32(object + 0x10) & 0x100) == 0)
            controls = game + 0x70;
        uint32 flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags | 0x40);
        G3Function8007BA50(object, game, state, controls);
        G3Function8007B75C(object, game, state, frame);
        uint32 heading = r_u16(state + 0xc6);
        w_u16(object + 0x64, heading);
    }
    bool transition = false;
    if (r_u32(object + 0xf8) != 0)
    {
        if ((r_u32(object + 0x18) & 1) == 0)
        {
            uint32 position = r_u16(object + 0x100);
            w_u16(state + 0x228, position);
        }
        if (r_u32(object + 0xf8) != 0 && (r_u32(object + 0x18) & 1) == 0)
        {
            sint32 offset = r_s16(state + 0x8a);
            sint32 saved = r_s16(state + 0x228);
            sint32 height = r_s16(object + 0x5c);
            if (saved - offset * 4 < height)
                transition = r_u16(object + 0x6e) != 0x35;
        }
    }
    if (!transition)
    {
        sint32 height = r_s16(object + 0x5c);
        sint32 saved = r_s16(state + 0x228);
        if (saved < height)
            transition = r_u16(object + 0x6e) == 0x35;
        if (!transition)
            transition = (r_u32(object + 0x134) & 0x100) != 0;
    }
    if (transition)
    {
        w_u32(object + 0x154, 1);
        w_u32(object + 0x150, 2);
        w_u16(object + 0x6e, 0x39);
        w_u16(object + 0x5e, 0);
        uint32 saved = r_u16(state + 0x228);
        uint32 offset = r_u16(state + 0x8a);
        w_u16(object + 0x5c, saved - offset);
        uint32 orientation = r_u32(state);
        w_u16(orientation, 0);
        uint32 height = r_u16(object + 0x5c);
        w_u16(state + 0x26c, height);
        camera = r_u32(game + 0x4c);
        if (G3Function8004DABC(camera, frame) != 2)
        {
            camera = r_u32(game + 0x4c);
            G3Function80014254(camera, 0x100);
            camera = r_u32(game + 0x4c);
            G3Function80013CDC(camera, 0, frame);
        }
        G3Function8005820C(0, 0);
    }
    if (r_u32(object + 0x150) == 3)
    {
        uint32 motion = r_u32(object + 0x154);
        if (motion == 0x10)
        {
            G3Function80078C04(object, game, state, controls, frame);
            motion = r_u32(object + 0x154);
        }
        if (motion == 2)
        {
            if ((r_u32(object + 0x14) & 0x10) != 0)
            {
                uint32 buttons = r_u32(controls);
                w_u16(object + 0x6e, (buttons & 0xf) != 0 ? 0x3c : 0x3b);
            }
            if ((r_u32(controls + 4) & 0x20) != 0)
                G3Function8007D0D0(object, game, state, controls, frame);
            else if ((r_u32(controls) & 0x80) == 0)
                w_u32(object + 0x154, 1);
        }
        bool direct_delay = false;
        if (r_u32(object + 0x154) == 1)
        {
            if ((r_u32(object + 0x14) & 0x10) != 0)
            {
                if (r_u16(object + 0x6e) == 0x35)
                {
                    uint32 first = r_u32(object + 0x30);
                    uint32 rest = r_u32(object + 0x30);
                    uint32 coordinate = r_u16(first + 0x34);
                    w_u16(object + 0x58, coordinate);
                    coordinate = r_u16(rest + 0x38);
                    w_u16(object + 0x5a, coordinate);
                    coordinate = r_u16(rest + 0x3c);
                    w_u16(object + 0x5c, coordinate);
                }
                w_u16(object + 0x6e, 0x38);
            }
            if (r_u16(object + 0x6e) == 0x35)
                direct_delay = true;
            else
            {
                if ((r_u32(controls + 4) & 0x20) != 0)
                    G3Function8007D0D0(object, game, state, controls, frame);
                else if ((r_u32(controls) & 0x80) != 0)
                {
                    w_u32(object + 0x154, 2);
                    w_u16(object + 0x6e, 0x3b);
                    w_u16(object + 0x5e, 0);
                }
            }
        }
        bool advance = true;
        if ((direct_delay || r_u16(object + 0x6e) == 0x35) && r_s16(object + 0x5e) < 15)
            advance = false;
        if (advance)
        {
            uint32 velocity = r_u32(object + 0x10c);
            if ((sint32)velocity < 0)
            {
                uint32 acceleration = (uint32)(sint32)r_s16(state + 0x8c);
                w_u32(object + 0x10c, velocity + acceleration);
            }
            else
            {
                if (r_u16(object + 0x6e) == 0x35)
                {
                    for (uint32 count = 0; count < 16; ++count)
                    {
                        uint32 copied[2];
                        for (uint32 word = 0; word < 2; ++word)
                        {
                            uint32 source = 0x800a0304 + word * 4;
                            uint32 high_address = source + 3;
                            uint32 high_lane = high_address & 3;
                            uint32 low_lane = source & 3;
                            uint32 high_word = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                            uint32 low_word = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
                            copied[word] = low_lane == 0 ? low_word : (high_word << ((3 - high_lane) * 8)) | (low_word >> (low_lane * 8));
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
                        uint32 random = G3SdkRandom80027FA8();
                        uint32 position = r_u16(object + 0x58);
                        w_u16(frame + 0x18, position + (random & 0x7f) - 0x3f);
                        random = G3SdkRandom80027FA8();
                        position = r_u16(object + 0x5a);
                        w_u16(frame + 0x1a, position + (random & 0x7f) - 0x3f);
                        random = G3SdkRandom80027FA8();
                        position = r_u16(object + 0x5c);
                        w_u16(frame + 0x1c, position + (random & 0x7f) - 0x3f);
                        uint32 angle = (uint32)(sint32)r_s16(state + 0x228);
                        G3Function800453D0(frame + 0x18, 0, frame + 0x10, angle, frame);
                    }
                }
                w_u32(object + 0x10c, 0);
            }
            uint32 height = r_u16(object + 0x5c);
            uint32 low_velocity = r_u16(object + 0x10c);
            w_u16(object + 0x5c, height + low_velocity);
        }
    }
    uint32 linked = r_u32(state + 0xf8);
    uint32 key = (uint32)(sint32)r_s16(state + 0x18a);
    uint32 animation = r_u32(linked + 0xc);
    G3Function8006F34C(animation, key - 0x40, 0x8c);
    linked = r_u32(object + 0x30);
    if (linked == 0)
        return 0;
    uint32 random = G3SdkRandom80027FA8();
    if ((random & 7) != 0)
        return 0x800a0000;
    uint32 copied[2];
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 source = 0x800a0304 + word * 4;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 low_lane = source & 3;
        uint32 high_word = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low_word = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
        copied[word] = low_lane == 0 ? low_word : (high_word << ((3 - high_lane) * 8)) | (low_word >> (low_lane * 8));
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
    random = G3SdkRandom80027FA8();
    linked = r_u32(object + 0x30);
    uint32 position = r_u16(linked + 0x1d4);
    w_u16(frame + 0x18, position + (random & 0x1f) - 0xf);
    random = G3SdkRandom80027FA8();
    linked = r_u32(object + 0x30);
    position = r_u16(linked + 0x1d8);
    w_u16(frame + 0x1a, position + (random & 0x1f) - 0xf);
    linked = r_u32(object + 0x30);
    position = r_u16(linked + 0x1dc);
    w_u16(frame + 0x1c, position + 0x20);
    uint32 angle = (uint32)(sint32)r_s16(state + 0x228);
    return G3Function800453D0(frame + 0x18, 0, frame + 0x10, angle, frame);
}

uint32 G3Function80064BE0(uint32 filename, uint32 destination, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80064be0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    while (r_u32(0x800aa7d4) != 0)
        G3Function80035AD4(frame);
    uint32 index = G3FindArchiveFile800654AC(filename);
    uint32 offset = ((index << 1) + index) << 3;
    uint32 entry = 0x800ad1b0 + offset;
    uint32 file_bytes = r_u32(entry + 4);
    uint32 packed_bytes = r_u32(entry + 8);
    uint32 output_sectors = (uint32)((sint32)(file_bytes + 0x7ff) >> 11);
    uint32 read_sectors;
    if (packed_bytes != 0)
    {
        ++output_sectors;
        read_sectors = (uint32)((sint32)(packed_bytes + 0x7ff) >> 11);
    }
    else
        read_sectors = output_sectors;
    uint32 input = destination + ((output_sectors - read_sectors) << 11);
    uint32 path_bytes = 0;
    while (r_u8(filename + path_bytes) != 0)
        ++path_bytes;
    ++path_bytes;
    {
#ifndef NDEBUG
    printf("Loading:%s\n", (const char *)psx_addr(filename, path_bytes));
#endif
    }
    uint32 metadata = r_u32(0x800ad0d4 + offset + 0xec);
    uint32 byte_offset = r_u32(entry + 0xc);
    uint32 sector_offset = (uint32)((sint32)byte_offset >> 11);
    w_u32(frame + 0x10, 1);
    w_u32(frame + 0x18, packed_bytes);
    w_u32(frame + 0x14, metadata);
    uint32 request = G3QueueRead800652C8(sector_offset, input, destination, read_sectors, 1, metadata, packed_bytes);
    while (G3Function80065650(request) != 0)
        G3CdPump80064F44();
    return destination;
}

uint32 G3Function8001E924(uint32 packets, uint32 order_table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8001e924, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    uint32 game = r_u32(0x80097048);
    uint32 color = r_u32(game + 0x69c4);
    uint32 packet = r_u32(packets + 4);
    uint32 result = color & 0xffffff;
    if (result == 0)
        return 0;
    if ((r_u8(game + 0x69c4) & 1) != 0)
    {
        w_u32(frame + 0x10, 0);
        w_u32(frame + 0x14, 0xf0);
        w_u32(frame + 0x18, 0x200);
        w_u32(frame + 0x1c, 0xf0);
        uint32 red = r_u8(game + 0x69c4);
        w_u32(frame + 0x20, red);
        uint32 green = r_u8(game + 0x69c5);
        w_u32(frame + 0x24, green);
        uint32 blue = r_u8(game + 0x69c6);
        w_u32(frame + 0x2c, 0);
        w_u32(frame + 0x28, blue);
        uint32 pool = r_u32(game + 0x68);
        w_u32(frame + 0x34, order_table);
        w_u32(frame + 0x30, pool);
        return G3Function8001E25C(0, 0, 0x200, 0, 0, 0xf0, 0x200, 0xf0, red, green, blue, 0, pool, order_table);
    }
    uint32 end = r_u32(packets + 8);
    if (packet >= end - 0x2d00)
        return 0;
    sint32 row = 0xec;
    uint32 texture = 0xee00;
    uint32 data = packet + 4;
    uint32 page = r_u32(game + 0x58) << 4;
    uint32 green = r_u8(game + 0x69c5);
    uint32 first_mode = page | 0xe1000100;
    uint32 second_mode = page | 0xe1000104;
    uint32 red = r_u8(game + 0x69c4);
    uint32 blue = r_u8(game + 0x69c6);
    uint32 command = red | (green << 8) | (blue << 16) | 0x64000000;
    uint32 index = r_u32(game + 0x14c) & 0x1f;
    do
    {
        uint32 sample_address = 0x80096b78 + index;
        index = (index + 1) & 0x1f;
        uint32 y = (uint32)row << 16;
        uint32 sample = (uint32)(sint32)r_s8(sample_address);
        row -= 2;
        w_u32(data, command);
        w_u32(data + 0xc, 0x20100);
        w_u32(data + 8, texture);
        w_u32(data + 4, (sample & 0xffff) + y);
        data += 0x14;
        uint32 second_xy = ((sample + 0x100) & 0xffff) + y;
        uint32 link = r_u32(order_table);
        w_u32(packet, (link & 0xffffff) | 0x04000000);
        uint32 next_link = packet & 0xffffff;
        packet += 0x14;
        w_u32(order_table, next_link);
        w_u32(data, first_mode);
        link = r_u32(order_table);
        data += 8;
        w_u32(packet, (link & 0xffffff) | 0x01000000);
        next_link = packet & 0xffffff;
        packet += 8;
        w_u32(order_table, next_link);
        w_u32(data, command);
        w_u32(data + 4, second_xy);
        w_u32(data + 0xc, 0x20100);
        w_u32(data + 8, texture);
        data += 0x14;
        link = r_u32(order_table);
        texture -= 0x200;
        w_u32(packet, (link & 0xffffff) | 0x04000000);
        next_link = packet & 0xffffff;
        packet += 0x14;
        w_u32(order_table, next_link);
        w_u32(data, second_mode);
        link = r_u32(order_table);
        data += 8;
        w_u32(packet, (link & 0xffffff) | 0x01000000);
        next_link = packet & 0xffffff;
        packet += 8;
        w_u32(order_table, next_link);
    } while (row >= 0);
    uint32 count = r_u32(packets);
    w_u32(packets + 4, packet);
    result = count + 0x3c0;
    w_u32(packets, result);
    return result;
}

void G3Function8002E43C(uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002e43c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    if (r_u32(0x80098ff0) == 0)
        return;
    G3EnterCritical8008A05C();
    SpuSetKey(0, 0xfffffe);
    for (uint32 index = 0; index < 4; ++index)
    {
        uint32 sound = r_u32(0x80098ff0);
        uint32 bank = r_u32(sound + 0x20 + index * 4);
        w_u16(bank + 0xf2, 0);
        w_u8(bank + 0xd9, 0);
        w_u8(bank + 0xd8, 0);
        w_u16(bank + 0xda, 0);
    }
    uint32 offset = 0x248;
    for (uint32 index = 0; index < 0x18; ++index)
    {
        uint32 sound = r_u32(0x80098ff0);
        uint32 voice = sound + offset;
        if ((r_u16(voice + 0x12) & 1) == 0)
        {
            w_u8(voice + 8, 0xff);
            w_u32(voice + 4, 0);
        }
        offset += 0x1c;
    }
    G3ExitCritical8008A03C();
    uint32 mode = G3Function80049040();
    SpuClearReverbWorkArea((sint32)(mode & 0xffff));
    uint32 game = r_u32(0x80097048);
    uint32 volume = r_u32(game + 0x67cc);
    uint32 last_bit = 0;
    while (volume != 0)
    {
        uint32 bit = volume & 0x80;
        uint32 product = volume * volume;
        if (bit != last_bit)
        {
            uint32 delay = product;
            bool repeat;
            do
            {
                ++delay;
                repeat = delay < 0x14;
                delay += product;
            } while (repeat);
            last_bit = volume & 0x80;
            w_u32(frame + 0x10, 3);
            w_u16(frame + 0x18, 0);
            w_u16(frame + 0x1a, 0);
            w_u16(frame + 0x14, volume);
            w_u16(frame + 0x16, volume);
            SpuSetCommonAttr((SpuCommonAttr *)psx_addr(frame + 0x10, sizeof(SpuCommonAttr)));
        }
        --volume;
    }
    w_u16(frame + 0x14, 0);
    w_u16(frame + 0x16, 0);
    SpuSetCommonAttr((SpuCommonAttr *)psx_addr(frame + 0x10, sizeof(SpuCommonAttr)));
}

uint32 G3Function80080FD4(uint32 index)
{
    FUNCTION_MARKER(0x80080fd4, "SLUS_008.06");
    uint32 offset = (uint32)((sint32)(index << 16) >> 16);
    uint32 port = r_u8(0x800a0340 + offset);
    return PadInfoMode((sint32)port, 2, 0) != 0;
}

uint32 G3Function8008504C(uint32 object, uint32 game, uint32 parameters, uint32 argument_3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8008504c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 button_resource = G3Function8003AA48(0x800a0464);
    uint32 red_resource = G3Function8003AA48(0x800a0470);
    uint32 screen_resource = G3Function8003AA48(0x800a047c);
    sint32 prerequisite = r_s16(parameters + 4);
    if (prerequisite != 0 && r_u32(0x800a0760 + ((uint32)prerequisite << 2)) == 0)
        w_u16(parameters + 6, 0);
    if (button_resource != 0)
    {
        uint32 button = G3Function8002A454(object, button_resource, frame);
        w_u32(object + 0x128, button);
        uint32 flags = r_u32(button + 0x10);
        w_u32(button + 0x10, flags | 0x400);
    }
    if (red_resource != 0)
    {
        uint32 definition = r_u32(object + 0x20);
        if ((r_u32(definition + 0x18) & 0x100) == 0 && (r_s16(parameters + 4) == 0 || r_s16(parameters + 6) != 0))
        {
            uint32 red = G3Function80084ED8(object, red_resource, frame);
            w_u32(object + 0x12c, red);
            uint32 flags = r_u32(red + 0x10);
            w_u32(red + 0x10, flags | 0x400);
            uint32 level = r_u16(game + 0x68b4);
            uint32 shift = (uint32)(sint32)r_s16(parameters);
            uint32 collected = r_u8(game + level + 0x67f8);
            if (((collected >> (shift & 31)) & 1) != 0)
            {
                red = r_u32(object + 0x12c);
                w_u16(red + 0xa2, 1);
            }
        }
    }
    if (screen_resource != 0)
    {
        uint32 definition = r_u32(object + 0x20);
        if ((r_u32(definition + 0x18) & 0x100) == 0)
        {
            uint32 model = r_u32(object + 0x1c);
            uint32 model_index = (uint32)(sint32)r_s16(object + 0xa2);
            uint32 models = r_u32(model + 0xc);
            uint32 source = r_u32(models + (model_index << 2));
            sint32 count = r_s16(source + 4);
            uint32 cursor = r_u32(source + 0x14);
            uint32 end = cursor + (uint32)(count * 12);
            uint32 screen = G3Function8002A454(object, screen_resource, frame);
            uint32 flags = r_u32(screen + 0x10);
            w_u32(screen + 0x10, flags | 0x80);
            uint32 height = r_u16(screen + 0x5c);
            model = r_u32(screen + 0x1c);
            w_u16(screen + 0x5c, height + 0x2a0);
            models = r_u32(model + 0xc);
            uint32 first_model = r_u32(models);
            uint32 texture = r_u32(source + 0x1c);
            w_u32(first_model + 0x1c, texture);
            w_u32(object + 0x130, screen);
            flags = r_u32(screen + 0x10);
            w_u32(screen + 0x10, flags | 0x400);
            if (cursor < end)
            {
                do
                {
                    if ((r_u8(cursor + 7) & 8) != 0)
                    {
                        model = r_u32(screen + 0x1c);
                        model_index = (uint32)(sint32)r_s16(screen + 0xa2);
                        models = r_u32(model + 0xc);
                        uint32 destination_model = r_u32(models + (model_index << 2));
                        uint32 destination = r_u32(destination_model + 0x14);
                        uint32 index = 0;
                        do
                        {
                            uint32 offset = (uint32)((sint32)(index << 16) >> 16) * 12;
                            ++index;
                            uint32 value = r_u32(cursor + offset + 8);
                            bool repeat = ((sint32)(index << 16) >> 16) < 8;
                            w_u32(destination + offset + 8, value);
                            if (!repeat)
                                break;
                        } while (true);
                        break;
                    }
                    cursor += 12;
                } while (cursor < end);
            }
            uint32 level = r_u16(game + 0x68b4);
            uint32 shift = (uint32)(sint32)r_s16(parameters);
            uint32 collected = r_u8(game + level + 0x67f8);
            if (((collected >> (shift & 31)) & 1) != 0)
                w_u16(screen + 0x9e, 1);
            else
            {
                w_u16(screen + 0x9e, 0);
                if (screen != 0)
                {
                    w_u16(screen + 0x6c, 0x200);
                    w_u16(screen + 0x68, 0x200);
                }
            }
        }
    }
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x154, 0);
    uint32 result = flags | 0x80;
    w_u32(object + 0x10, result);
    return result;
}

uint32 G3Function80085340(uint32 object, uint32 game, uint32 parameters, uint32 argument_3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80085340, "SLUS_008.06");
    uint32 frame = guest_stack - 0x58;
    sint32 prerequisite = r_s16(parameters + 4);
    uint32 mode = 0;
    bool reload_mode = true;
    if (prerequisite != 0 && r_u32(0x800a0760 + ((uint32)prerequisite << 2)) != 0)
    {
        mode = r_u32(object + 0x154);
        reload_mode = false;
        if (mode != 3)
        {
            uint32 red = r_u32(object + 0x12c);
            if (red == 0 && mode != 1)
            {
                uint32 resource = G3Function8003AA48(0x800a0470);
                if (resource != 0)
                {
                    red = G3Function80084ED8(object, resource, frame);
                    w_u32(object + 0x12c, red);
                    w_u16(red + 0x68, 1);
                    red = r_u32(object + 0x12c);
                    w_u16(red + 0x6a, 1);
                    red = r_u32(object + 0x12c);
                    w_u16(red + 0x6c, 1);
                    w_u32(object + 0x154, 3);
                    w_u16(parameters + 6, 1);
                    w_u32(object + 0x138, 0x10);
                    uint32 level = r_u16(game + 0x68b4);
                    uint32 shift = (uint32)(sint32)r_s16(parameters);
                    uint32 collected = r_u8(game + level + 0x67f8);
                    if (((collected >> (shift & 31)) & 1) != 0)
                    {
                        red = r_u32(object + 0x12c);
                        w_u16(red + 0xa2, 1);
                    }
                    uint32 current_game = r_u32(0x80097048);
                    w_u32(frame + 0x10, 0x5dc);
                    uint32 player = r_u32(current_game + 0x50);
                    G3Function8003E518(player, 0x817, (uint32)-400, 100, frame);
                }
                reload_mode = true;
            }
        }
    }
    if (reload_mode)
        mode = r_u32(object + 0x154);
    if (mode == 3)
    {
        uint32 red = r_u32(object + 0x12c);
        for (uint32 group = 0; group < 3; ++group)
        {
            uint32 count = group == 2 ? 2 : 3;
            uint32 copied[3];
            for (uint32 word = 0; word < count; ++word)
            {
                uint32 source = 0x800a0488 + group * 12 + word * 4;
                uint32 high_address = source + 3;
                uint32 high_lane = high_address & 3;
                uint32 low_lane = source & 3;
                uint32 high_word = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                uint32 low_word = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
                copied[word] = low_lane == 0 ? low_word : (high_word << ((3 - high_lane) * 8)) | (low_word >> (low_lane * 8));
            }
            for (uint32 word = 0; word < count; ++word)
            {
                uint32 destination = frame + 0x18 + group * 12 + word * 4;
                uint32 high_address = destination + 3;
                uint32 high_lane = high_address & 3;
                uint32 low_lane = destination & 3;
                w_masked_u32(high_address & 0xfffffffc, copied[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                w_masked_u32(destination & 0xfffffffc, copied[word] << (low_lane * 8), (15 << low_lane) & 15);
            }
        }
        if (red != 0)
        {
            uint32 remaining = r_u32(object + 0x138) - 1;
            w_u32(object + 0x138, remaining);
            if ((sint32)remaining <= 0)
                w_u32(object + 0x154, 0);
            else if ((sint32)remaining < 16)
            {
                uint32 value = r_u16(frame + 0x18 + ((16 - remaining) << 1));
                w_u16(red + 0x68, value);
                remaining = r_u32(object + 0x138);
                value = r_u16(frame + 0x18 + ((16 - remaining) << 1));
                w_u16(red + 0x6a, value);
                remaining = r_u32(object + 0x138);
                value = r_u16(frame + 0x18 + ((16 - remaining) << 1));
                w_u16(red + 0x6c, value);
            }
        }
    }
    if (r_u32(object + 0x154) == 1)
    {
        uint32 screen = r_u32(object + 0x130);
        if (screen != 0)
        {
            uint32 tick = r_u32(screen + 0x104);
            w_u32(screen + 0x104, tick + 1);
            uint32 flags = r_u32(object + 0x10);
            w_u32(object + 0x10, flags | 0x80);
            w_u16(screen + 0x9e, 1);
            uint32 phase = r_u32(object + 0x134);
            if (phase == 0)
            {
                sint32 scale = r_s16(screen + 0x68);
                uint32 raw_scale = r_u16(screen + 0x68);
                if (scale + 0x400 < 0x1000)
                    w_u16(screen + 0x68, raw_scale + 0x400);
                else
                {
                    w_u16(screen + 0x68, 0x1000);
                    w_u32(object + 0x134, 1);
                }
            }
            else if (phase == 1)
            {
                sint32 scale = r_s16(screen + 0x6c);
                uint32 raw_scale = r_u16(screen + 0x6c);
                if (scale + 0x400 < 0x1600)
                    w_u16(screen + 0x6c, raw_scale + 0x400);
                else
                {
                    w_u16(screen + 0x6c, 0x1600);
                    w_u32(object + 0x134, 2);
                }
            }
            else if (phase == 2)
            {
                sint32 scale = r_s16(screen + 0x6c);
                uint32 raw_scale = r_u16(screen + 0x6c);
                if (scale - 0x400 >= 0x1001)
                    w_u16(screen + 0x6c, raw_scale - 0x400);
                else
                {
                    w_u16(screen + 0x6c, 0x1000);
                    w_u32(object + 0x134, phase);
                }
            }
        }
    }
    if (r_u32(object + 0x128) == 0)
    {
        uint32 resource = G3Function8003AA48(0x800a0464);
        if (resource != 0)
        {
            uint32 button = G3Function8002A454(object, resource, frame);
            w_u32(object + 0x128, button);
        }
    }
    if (r_u32(object + 0x12c) == 0 && r_u32(object + 0x154) != 1 && r_s16(parameters + 4) == 0)
    {
        uint32 resource = G3Function8003AA48(0x800a0470);
        if (resource != 0)
        {
            uint32 red = G3Function80084ED8(object, resource, frame);
            w_u32(object + 0x12c, red);
            uint32 level = r_u16(game + 0x68b4);
            uint32 shift = (uint32)(sint32)r_s16(parameters);
            uint32 collected = r_u8(game + level + 0x67f8);
            if (((collected >> (shift & 31)) & 1) != 0)
                w_u16(red + 0xa2, 1);
        }
    }
    if (r_u32(object + 0x130) != 0 || r_u32(object + 0x154) == 1)
        return 1;
    uint32 screen_resource = G3Function8003AA48(0x800a047c);
    if (screen_resource == 0)
        return 0;
    uint32 model = r_u32(object + 0x1c);
    uint32 model_index = (uint32)(sint32)r_s16(object + 0xa2);
    uint32 models = r_u32(model + 0xc);
    uint32 source = r_u32(models + (model_index << 2));
    sint32 count = r_s16(source + 4);
    uint32 cursor = r_u32(source + 0x14);
    uint32 end = cursor + (uint32)(count * 12);
    uint32 screen = G3Function8002A454(object, screen_resource, frame);
    uint32 flags = r_u32(screen + 0x10);
    w_u32(screen + 0x10, flags | 0x80);
    uint32 height = r_u16(screen + 0x5c);
    model = r_u32(screen + 0x1c);
    w_u16(screen + 0x5c, height + 0x2a0);
    models = r_u32(model + 0xc);
    uint32 first_model = r_u32(models);
    uint32 texture = r_u32(source + 0x1c);
    w_u32(first_model + 0x1c, texture);
    w_u32(object + 0x130, screen);
    flags = r_u32(screen + 0x10);
    w_u32(screen + 0x10, flags | 0x400);
    if (cursor < end)
    {
        do
        {
            if ((r_u8(cursor + 7) & 8) != 0)
            {
                model = r_u32(screen + 0x1c);
                model_index = (uint32)(sint32)r_s16(screen + 0xa2);
                models = r_u32(model + 0xc);
                uint32 destination_model = r_u32(models + (model_index << 2));
                uint32 destination = r_u32(destination_model + 0x14);
                uint32 index = 0;
                do
                {
                    uint32 offset = (uint32)((sint32)(index << 16) >> 16) * 12;
                    ++index;
                    uint32 value = r_u32(cursor + offset + 8);
                    bool repeat = ((sint32)(index << 16) >> 16) < 8;
                    w_u32(destination + offset + 8, value);
                    if (!repeat)
                        break;
                } while (true);
                break;
            }
            cursor += 12;
        } while (cursor < end);
    }
    uint32 level = r_u16(game + 0x68b4);
    uint32 shift = (uint32)(sint32)r_s16(parameters);
    uint32 collected = r_u8(game + level + 0x67f8);
    if (((collected >> (shift & 31)) & 1) != 0)
        w_u16(screen + 0x9e, 1);
    else
    {
        w_u16(screen + 0x9e, 0);
        if (screen != 0)
        {
            w_u16(screen + 0x6c, 0x200);
            w_u16(screen + 0x68, 0x200);
            return 0x200;
        }
    }
    return 1;
}

uint32 G3Function8001FE14(uint32 angles, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8001fe14, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    uint32 value = r_u16(angles);
    w_u16(frame + 0x18, value);
    value = r_u16(angles + 2);
    w_u16(frame + 0x1a, value);
    value = r_u16(angles + 4);
    w_u16(frame + 0x1c, value);
    value = r_u16(angles + 6);
    w_u16(frame + 0x1e, value);
    uint32 copied[2];
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 source = frame + 0x18 + word * 4;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 low_lane = source & 3;
        uint32 high_word = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low_word = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
        copied[word] = low_lane == 0 ? low_word : (high_word << ((3 - high_lane) * 8)) | (low_word >> (low_lane * 8));
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
    uint32 flags = (uint32)(sint32)r_s16(frame + 0x16);
    uint32 swap = flags & 1;
    uint32 alternate = (flags >> 1) & 1;
    uint32 negate = (flags >> 2) & 1;
    uint32 first = r_u8(0x8009d618 + ((flags >> 3) & 3));
    uint32 second = r_u8(0x8009d620 + first + negate);
    uint32 third = r_u8(0x8009d620 + first - (negate - 1));
    w_u32(frame + 0x24, alternate);
    w_u32(frame + 0x20, third);
    if (swap == 1)
    {
        uint32 z = r_u16(frame + 0x14);
        uint32 x = r_u16(frame + 0x10);
        w_u16(frame + 0x10, z);
        w_u16(frame + 0x14, x);
    }
    if (negate == 1)
    {
        uint32 x = r_u16(frame + 0x10);
        uint32 z = r_u16(frame + 0x14);
        w_u16(frame + 0x10, 0 - x);
        uint32 y = r_u16(frame + 0x12);
        w_u16(frame + 0x14, 0 - z);
        w_u16(frame + 0x12, 0 - y);
    }
    uint32 x = (uint32)(sint32)r_s16(frame + 0x10);
    uint32 y = (uint32)(sint32)r_s16(frame + 0x12);
    uint32 z = (uint32)(sint32)r_s16(frame + 0x14);
    uint32 sine_x = G3Function8005B470(x);
    uint32 sine_y = G3Function8005B470(y);
    uint32 sine_z = G3Function8005B470(z);
    uint32 cosine_x = G3Function8005B590(x);
    uint32 cosine_y = G3Function8005B590(y);
    uint32 cosine_z = G3Function8005B590(z);
    uint32 sine_x_sine_z = (uint32)((sint32)(sine_x * sine_z) >> 12);
    uint32 sine_x_cosine_z = (uint32)((sint32)(sine_x * cosine_z) >> 12);
    uint32 cosine_x_sine_z = (uint32)((sint32)(cosine_x * sine_z) >> 12);
    uint32 cosine_x_cosine_z = (uint32)((sint32)(cosine_x * cosine_z) >> 12);
    alternate = r_u32(frame + 0x24);
    uint32 result;
    if (alternate == 1)
    {
        uint32 first_second = (uint32)((sint32)(cosine_y * cosine_x) >> 12);
        uint32 first_third = (uint32)((sint32)(cosine_y * sine_x) >> 12);
        uint32 second_first = (uint32)((sint32)(cosine_y * cosine_z) >> 12);
        uint32 second_second = (uint32)((sint32)((0 - sine_y) * cosine_x_cosine_z) >> 12) + sine_x_sine_z;
        uint32 second_third = (uint32)((sint32)((0 - sine_y) * sine_x_cosine_z) >> 12) - cosine_x_sine_z;
        w_u16(output + first * 8, sine_y);
        w_u16(output + second * 2 + first * 6, first_second);
        third = r_u32(frame + 0x20);
        w_u16(output + third * 2 + first * 6, first_third);
        w_u16(output + first * 2 + second * 6, second_first);
        w_u16(output + second * 8, second_second);
        w_u16(output + third * 2 + second * 6, second_third);
        uint32 third_first = (uint32)((sint32)((0 - cosine_y) * sine_z) >> 12);
        uint32 third_second = (uint32)((sint32)(sine_y * cosine_x_sine_z) >> 12) + sine_x_cosine_z;
        w_u16(output + first * 2 + third * 6, third_first);
        w_u16(output + second * 2 + third * 6, third_second);
        result = (uint32)((sint32)(sine_y * sine_x_sine_z) >> 12) - cosine_x_cosine_z;
        w_u16(output + third * 8, result);
    }
    else
    {
        uint32 first_first = (uint32)((sint32)(sine_y * sine_z) >> 12);
        uint32 first_second = (uint32)((sint32)(cosine_y * cosine_x_sine_z) >> 12) - sine_x_cosine_z;
        uint32 first_third = (uint32)((sint32)(cosine_y * sine_x_sine_z) >> 12) + cosine_x_cosine_z;
        uint32 second_first = (uint32)((sint32)(sine_y * cosine_z) >> 12);
        w_u16(output + first * 8, first_first);
        w_u16(output + second * 2 + first * 6, first_second);
        third = r_u32(frame + 0x20);
        w_u16(output + third * 2 + first * 6, first_third);
        w_u16(output + first * 2 + second * 6, second_first);
        uint32 second_second = (uint32)((sint32)(cosine_y * cosine_x_cosine_z) >> 12) + sine_x_sine_z;
        uint32 second_third = (uint32)((sint32)(cosine_y * sine_x_cosine_z) >> 12) - cosine_x_sine_z;
        w_u16(output + second * 8, second_second);
        w_u16(output + third * 2 + second * 6, second_third);
        w_u16(output + first * 2 + third * 6, 0 - cosine_y);
        uint32 third_second = (uint32)((sint32)(sine_y * cosine_x) >> 12);
        w_u16(output + second * 2 + third * 6, third_second);
        result = (uint32)((sint32)(sine_y * sine_x) >> 12);
        w_u16(output + third * 8, result);
    }
    return result;
}

uint32 G3Function80023448(uint32 text, uint32 position, uint32 incoming_second_glyph, uint32 incoming_third_glyph, uint32 guest_stack, G3TextMeasureOperands *result_operands)
{
    FUNCTION_MARKER(0x80023448, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    G3TextMeasureOperands operands;
    operands.first_glyph = position;
    operands.second_glyph = incoming_second_glyph;
    operands.third_glyph = incoming_third_glyph;
    uint32 width = G3Function80022FD0(text, &operands);
    uint32 centered = 0x100 - (uint32)((sint32)width >> 1);
    uint32 x = (uint32)((sint32)(centered << 16) >> 16);
    uint32 y = (uint32)((sint32)(position << 16) >> 16);
    G3Function80022E30(x, y);
    G3Function80020CBC(text, y, operands.second_glyph, operands.third_glyph, frame, &operands);
    if (result_operands != 0)
        *result_operands = operands;
    return text == 0 ? 0x800a0000 : 0;
}

uint32 G3Function80028F84(uint32 game, uint32 incoming_second_glyph, uint32 incoming_third_glyph, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80028f84, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    if (r_u8(game + 0x68f8) != 0)
    {
        uint32 text = G3Function80065D38(0x44);
        G3Function80023448(text, 0xd8, incoming_second_glyph, incoming_third_glyph, frame, 0);
    }
    uint32 state = r_u32(game + 0x14c);
    uint32 input = game + 0x98;
    if (state == 0)
        w_u32(0x800a0c8c, 0);
    G3Function80028AC4(input, frame);
    uint32 output = game + 0x70;
    G3Function80028004(output, input, 0);
    return G3Function80028004(output, input, 1);
}

uint32 G3Function80027B20(uint32 game, uint32 incoming_second_glyph, uint32 incoming_third_glyph, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80027b20, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function80028F84(game, incoming_second_glyph, incoming_third_glyph, frame);
    return G3Function80026F28(game, frame);
}

void G3Function8003F5B8(uint32 source_path, uint32 source_state, uint32 target_path, uint32 target_state, uint32 incoming_fraction, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003f5b8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    if (source_path == 0 || source_state == 0 || target_path == 0 || target_state == 0)
        return;
    uint32 source_flags = r_u16(source_state + 2);
    if ((source_flags & 1) == 0)
    {
        uint32 key = G3Function8003F8A4(source_path, source_state, frame, 0);
        G3Function8003F93C(target_path, target_state, key & 0xffff, frame);
        return;
    }
    uint32 source_index = (uint32)r_s16(source_state);
    uint32 source_prefix = G3Function8003F400(source_path, source_index);
    source_prefix = (uint32)((sint32)(source_prefix << 16) >> 16);
    G3Function8003F93C(target_path, target_state, source_prefix & 0xffff, frame);
    uint32 target_index = (uint32)r_s16(target_state);
    uint32 target_prefix = G3Function8003F400(target_path, target_index);
    target_prefix = (uint32)((sint32)(target_prefix << 16) >> 16);
    uint32 format = r_u8(source_path + 6);
    source_index = (uint32)r_s16(source_state);
    uint32 entries = r_u32(source_path);
    uint32 source_duration = (uint32)r_s16(entries + source_index * (format == 1 ? 10 : 32));
    format = r_u8(target_path + 6);
    target_index = (uint32)r_s16(target_state);
    entries = r_u32(target_path);
    uint32 target_duration = (uint32)r_s16(entries + target_index * (format == 1 ? 10 : 32));
    if (target_duration == 0)
        target_duration = 1;
    uint32 fraction = incoming_fraction;
    if (source_prefix == target_prefix && target_duration == source_duration)
    {
        source_flags = r_u16(source_state + 2);
        uint32 target_flags = r_u16(target_state + 2);
        w_u16(target_state + 2, (source_flags & 0xfffe) | (target_flags & 1));
    }
    else
    {
        source_flags = r_u16(source_state + 2);
        uint32 product = source_duration * (source_flags >> 1);
        uint32 target_flags = r_u16(target_state + 2);
        uint32 numerator = product + ((target_flags >> 1) << 15);
        fraction = numerator / target_duration;
        w_u16(target_state + 2, (target_flags & 1) | (fraction << 1));
    }
    if (fraction > 0x7fff)
    {
        source_flags = r_u16(source_state + 2);
        uint32 product = (source_flags >> 1) * source_duration;
        uint32 distance = product >> 15;
        uint32 key = (distance + source_prefix) & 0xffff;
        G3Function8003F93C(target_path, target_state, key, frame);
        format = r_u8(target_path + 6);
        target_index = (uint32)r_s16(target_state);
        entries = r_u32(target_path);
        target_duration = (uint32)r_s16(entries + target_index * (format == 1 ? 10 : 32));
        if (target_duration == 0 || source_duration == 0)
            return;
        uint32 consumed = (distance << 15) / source_duration;
        source_flags = r_u16(source_state + 2);
        uint32 remaining = (source_flags >> 1) - consumed;
        product = remaining * source_duration;
        uint32 converted = product / target_duration;
        uint32 target_flags = r_u16(target_state + 2);
        uint32 numerator = ((target_flags >> 1) << 15) + converted;
        uint32 result = numerator / target_duration;
        w_u16(target_state + 2, (result << 1) | 1);
    }
    else
    {
        uint32 target_flags = r_u16(target_state + 2);
        w_u16(target_state + 2, target_flags | 1);
    }
}

uint32 G3Function80082F30(uint32 object, uint32 paths, uint32 state, uint32 distance, uint32 incoming_fraction, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80082f30, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    if (state == 0)
        return 0;
    uint32 position_path = r_u32(paths);
    uint32 position = G3Function80040B70(position_path, distance, state, frame);
    uint32 failed = 0;
    if (position == 0)
        failed = 1;
    else
    {
        uint32 component = r_u16(position);
        w_u16(object + 0x58, component);
        component = r_u16(position + 2);
        w_u16(object + 0x5a, component);
        component = r_u16(position + 4);
        w_u16(object + 0x5c, component);
        uint32 rotation_path = r_u32(paths + 4);
        if (rotation_path != 0)
        {
            uint32 rotation_state = state + 4;
            position_path = r_u32(paths);
            G3Function8003F5B8(position_path, state, rotation_path, rotation_state, incoming_fraction, frame);
            rotation_path = r_u32(paths + 4);
            if (G3Function80041008(rotation_path, rotation_state, frame + 0x10, frame) == 0)
                failed = 1;
            else
            {
                component = r_u16(frame + 0x10);
                w_u16(object + 0x60, component);
                component = r_u16(frame + 0x12);
                w_u16(object + 0x62, component);
                component = r_u16(frame + 0x14);
                w_u16(object + 0x64, component);
            }
        }
    }
    if (failed == 1)
        G3Function800193F8(0x4000, object, frame);
    return failed;
}

uint32 G3Function800500C0(uint32 direction, uint32 reference, uint32 owner, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800500c0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x80;
    uint32 owner_camera = r_u32(owner + 0x4c);
    uint32 game = r_u32(0x80097048);
    uint32 matrix = r_u32(0x80096a5c);
    uint32 view = r_u32(game + 0x50);
    uint32 camera = r_u32(game + 0x4c);
    uint32 mode = r_u32(0x80096a7c);
    uint32 camera_angle = r_u16(camera + 0x1c);
    if (mode == 0)
    {
        uint32 angle = G3Function8002F540(direction, reference, frame);
        angle = (uint32)((sint32)(angle << 16) >> 16);
        G3Function80050A78(angle, frame);
        uint32 first = (uint32)r_s16(direction + 2);
        uint32 second = (uint32)r_s16(reference + 4);
        uint32 product = first * second;
        first = (uint32)r_s16(direction + 4);
        second = (uint32)r_s16(reference + 2);
        uint32 difference = product - first * second;
        w_u16(frame + 0x10, 0 - (uint32)((sint32)difference >> 12));
        first = (uint32)r_s16(direction);
        second = (uint32)r_s16(reference + 4);
        product = first * second;
        first = (uint32)r_s16(direction + 4);
        second = (uint32)r_s16(reference);
        difference = product - first * second;
        w_u16(frame + 0x12, (uint32)((sint32)difference >> 12));
        first = (uint32)r_s16(direction);
        second = (uint32)r_s16(reference + 2);
        product = first * second;
        first = (uint32)r_s16(direction + 2);
        second = (uint32)r_s16(reference);
        difference = product - first * second;
        w_u16(frame + 0x14, 0 - (uint32)((sint32)difference >> 12));
        G3Function8002F630(frame + 0x10, matrix, angle, frame);
        uint32 column_x = (uint32)r_s16(matrix);
        w_u32(frame + 0x28, column_x);
        uint32 column_y = (uint32)r_s16(matrix + 6);
        w_u32(frame + 0x2c, column_y);
        uint32 column_z = (uint32)r_s16(matrix + 0xc);
        w_u32(frame + 0x30, column_z);
        uint32 direction_x = (uint32)r_s16(direction);
        uint32 dot = column_x * direction_x;
        first = (uint32)r_s16(direction + 2);
        dot += column_y * first;
        first = (uint32)r_s16(direction + 4);
        dot += column_z * first;
        dot = (uint32)((sint32)dot >> 12);
        product = dot * direction_x;
        uint32 x = column_x - (uint32)((sint32)product >> 12);
        w_u32(frame + 0x38, x);
        first = (uint32)r_s16(direction + 2);
        product = dot * first;
        uint32 y = column_y - (uint32)((sint32)product >> 12);
        w_u32(frame + 0x3c, y);
        first = (uint32)r_s16(direction + 4);
        product = dot * first;
        uint32 z = column_z - (uint32)((sint32)product >> 12);
        w_u32(frame + 0x40, z);
        for (uint32 pass = 0; pass < 2; ++pass)
        {
            uint32 local = frame + 0x38;
            if (pass == 1)
            {
                first = (uint32)r_s16(direction + 2);
                uint32 projected_z = r_u32(frame + 0x40);
                product = first * projected_z;
                uint32 projected_y = r_u32(frame + 0x3c);
                first = (uint32)r_s16(direction + 4);
                difference = product - first * projected_y;
                x = (uint32)((sint32)(difference << 4) >> 16);
                w_u32(frame + 0x48, x);
                first = (uint32)r_s16(direction);
                product = first * projected_z;
                uint32 projected_x = r_u32(frame + 0x38);
                first = (uint32)r_s16(direction + 4);
                difference = product - first * projected_x;
                y = 0 - (uint32)((sint32)difference >> 12);
                y = (uint32)((sint32)(y << 16) >> 16);
                w_u32(frame + 0x4c, y);
                first = (uint32)r_s16(direction);
                product = first * projected_y;
                first = (uint32)r_s16(direction + 2);
                difference = product - first * projected_x;
                z = (uint32)((sint32)(difference << 4) >> 16);
                w_u32(frame + 0x50, z);
                local = frame + 0x48;
            }
            uint32 absolute_x = (sint32)x < 0 ? 0 - x : x;
            uint32 absolute_y = (sint32)y < 0 ? 0 - y : y;
            uint32 absolute_z = (sint32)z < 0 ? 0 - z : z;
            sint32 minimum = (sint16)absolute_x;
            sint32 middle = (sint16)absolute_y;
            sint32 maximum = (sint16)absolute_z;
            if (minimum > middle)
            {
                sint32 temporary = minimum;
                minimum = middle;
                middle = temporary;
            }
            if (middle > maximum)
            {
                sint32 temporary = middle;
                middle = maximum;
                maximum = temporary;
            }
            if (minimum > middle)
            {
                sint32 temporary = minimum;
                minimum = middle;
                middle = temporary;
            }
            sint32 length = (30 * maximum + 12 * middle + 9 * minimum) / 8;
            if (length != 0)
            {
                uint32 numerator = r_u32(local) << 14;
                uint32 normalized_x = (uint32)((sint64)(sint32)numerator / length);
                numerator = r_u32(local + 4) << 14;
                uint32 normalized_y = (uint32)((sint64)(sint32)numerator / length);
                numerator = r_u32(local + 8) << 14;
                uint32 normalized_z = (uint32)((sint64)(sint32)numerator / length);
                w_u32(local, (uint32)(sint32)(sint16)normalized_x);
                w_u32(local + 4, (uint32)(sint32)(sint16)normalized_y);
                w_u32(local + 8, (uint32)(sint32)(sint16)normalized_z);
            }
        }
        uint32 component = r_u16(frame + 0x38);
        w_u16(matrix, component);
        component = r_u16(frame + 0x48);
        w_u16(matrix + 2, component);
        component = r_u16(direction);
        w_u16(matrix + 4, component);
        component = r_u16(frame + 0x3c);
        w_u16(matrix + 6, component);
        component = r_u16(frame + 0x4c);
        w_u16(matrix + 8, component);
        component = r_u16(direction + 2);
        w_u16(matrix + 0xa, component);
        component = r_u16(frame + 0x40);
        w_u16(matrix + 0xc, component);
        component = r_u16(frame + 0x50);
        w_u16(matrix + 0xe, component);
        component = r_u16(direction + 4);
        w_u16(matrix + 0x10, component);
        G3Function800202A4(matrix, frame + 0x20, 0x15, frame);
        G3Function8001FA7C(frame + 0x20, 0x80096a72, frame);
        G3Function8005BD00(frame + 0x20, matrix);
        w_u32(0x80096a80, 1);
        return 1;
    }
    uint32 direction_y = (uint32)r_s16(direction + 2);
    uint32 direction_x = (uint32)r_s16(direction);
    uint32 angle = G3Function8002F240(direction_y, direction_x, frame) + 0x400;
    uint32 signed_camera_angle = (uint32)((sint32)(camera_angle << 16) >> 16);
    uint32 signed_angle = (uint32)((sint32)(angle << 16) >> 16);
    uint32 difference = G3Function80038B48(signed_camera_angle, signed_angle);
    sint32 turn = (sint16)difference;
    if (turn != 0 && (r_u32(0x800a24d0) & 0x800) == 0)
    {
        G3Function800524E4();
        uint32 flags = r_u32(0x800a2574) & 0xffffff3f;
        w_u32(0x800a2574, flags);
        w_u32(0x800a2574, flags | (turn > 0 ? 0x40 : 0x80));
        w_u32(0x800a256c, 0x20);
        w_u32(0x800a2568, signed_angle - 0x400);
    }
    sint32 horizontal = r_s16(direction);
    if (horizontal < 0)
        horizontal = -horizontal;
    bool vertical = horizontal < 5;
    if (vertical)
    {
        horizontal = r_s16(direction + 2);
        if (horizontal < 0)
            horizontal = -horizontal;
        vertical = horizontal < 5;
    }
    if (vertical)
    {
        if (r_s16(direction + 4) >= 0)
        {
            G3Function8002F718(matrix);
            uint32 heading = r_u16(owner_camera + 0x1c);
            w_u16(view + 0x64, heading - 0x400);
        }
        else
        {
            w_u16(matrix, 0x1000);
            w_u16(matrix + 2, 0);
            w_u16(matrix + 4, 0);
            w_u16(matrix + 6, 0);
            w_u16(matrix + 8, 0xfffff000);
            w_u16(matrix + 0xa, 0);
            w_u16(matrix + 0xc, 0);
            w_u16(matrix + 0xe, 0);
            w_u16(matrix + 0x10, 0xfffff000);
        }
        G3Function800202A4(matrix, frame + 0x20, 0x15, frame);
        G3Function8001FA7C(frame + 0x20, 0x80096a72, frame);
        G3Function8005BD00(frame + 0x20, matrix);
        uint32 result = r_u32(0x80096a5c);
        w_u32(0x80096a80, 0);
        w_u32(0x80096a60, result);
        return result;
    }
    direction_y = 0 - (uint32)r_s16(direction + 2);
    w_u16(frame + 0x10, direction_y);
    direction_x = (uint32)r_s16(direction);
    w_u16(frame + 0x14, 0);
    w_u16(frame + 0x12, direction_x);
    uint32 direction_z = (uint32)r_s16(direction + 4);
    uint32 product = 0 - direction_z * direction_x;
    w_u16(frame + 0x18, (uint32)((sint32)product >> 12));
    direction_z = (uint32)r_s16(direction + 4);
    uint32 signed_y = (uint32)((sint32)(direction_y << 16) >> 16);
    product = 0 - direction_z * signed_y;
    w_u16(frame + 0x1a, 0 - (uint32)((sint32)product >> 12));
    uint32 first = (uint32)r_s16(direction);
    product = first * direction_x;
    first = (uint32)r_s16(direction + 2);
    difference = product - first * signed_y;
    w_u16(frame + 0x1c, (uint32)((sint32)difference >> 12));
    w_u16(matrix, direction_y);
    uint32 component = r_u16(frame + 0x18);
    w_u16(matrix + 2, component);
    component = r_u16(direction);
    w_u16(matrix + 4, component);
    component = r_u16(frame + 0x12);
    w_u16(matrix + 6, component);
    component = r_u16(frame + 0x1a);
    w_u16(matrix + 8, component);
    component = r_u16(direction + 2);
    w_u16(matrix + 0xa, component);
    component = r_u16(frame + 0x14);
    w_u16(matrix + 0xc, component);
    component = r_u16(frame + 0x1c);
    w_u16(matrix + 0xe, component);
    component = r_u16(direction + 4);
    w_u16(matrix + 0x10, component);
    G3Function800202A4(matrix, frame + 0x20, 0x15, frame);
    G3Function8001FA7C(frame + 0x20, 0x80096a72, frame);
    G3Function8005BD00(frame + 0x20, matrix);
    uint32 current_matrix = r_u32(0x80096a5c);
    w_u32(0x80096a80, 1);
    w_u32(0x80096a60, current_matrix);
    return 1;
}
