#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>

uint32 G3Function80082854(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80082854, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 resource = G3Function8003AA48(0x800a0458);
    uint32 result = 0;
    if (object != 0 && resource != 0)
    {
        result = G3Function8002A454(object, resource, frame);
        if (result != 0)
        {
            uint32 mask = G3Function80037094(object);
            G3Function800370E8(object, mask, frame);
            G3Function80039BF4(object, 0, frame);
        }
    }
    return result;
}

uint32 G3Function80037EAC(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80037eac, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 resource = G3Function8003AA48(0x8009dab8);
    uint32 descriptor = r_u32(object + 0x1c);
    sint32 model_index = r_s16(object + 0xa2);
    uint32 models = r_u32(descriptor + 0x0c);
    uint32 model = r_u32(models + ((uint32)model_index << 2));
    sint32 height = r_s16(model + 0x26);
    sint32 candidate = height / 512 + 4;
    uint32 result = candidate < 9;
    uint32 count = result != 0 ? (uint32)candidate : 8;
    while (count != 0)
    {
        result = G3Function8002A454(object, resource, frame);
        --count;
    }
    return result;
}

uint32 G3Function8003778C(uint32 object, uint32 callback, uint32 flags, uint32 selector, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003778c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    G3Function800370E8(object, 0x1000, frame);
    G3Function800370E8(object, 0x2000, frame);
    G3Function800370E8(object, 0x4000, frame);
    G3Function800370E8(object, 0x8000, frame);
    G3Function80039BF4(object, 0x8003774c, frame);
    w_u16(object + 0x104, selector);
    w_u16(object + 0x108, 0);
    w_u16(object + 0x10a, 0);
    w_u16(object + 0x10c, flags);
    w_u16(object + 0x10e, 1);
    return 1;
}

uint32 G3Function80037094(uint32 object)
{
    FUNCTION_MARKER(0x80037094, "SLUS_008.06");
    uint32 flags = r_u32(object + 0x10);
    return (~flags) & 0x1f000;
}

uint32 G3Function80039BF4(uint32 object, uint32 callback, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80039bf4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 previous = r_u32(object + 0xec);
    if (previous != 0)
    {
        uint32 game = r_u32(0x80097048);
        G3DispatchStateEvent(previous, object, game, 7, 0, frame);
    }
    w_u32(object + 0xec, callback);
    return 0;
}

uint32 G3Function80037F5C(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80037f5c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    w_u32(guest_stack + 4, game);
    uint32 effects = (uint32)r_s16(object + 0x10c);
    sint32 tick = r_s16(object + 0x10a);
    bool extended = (effects & 0xa00) != 0;
    bool player_effect = false;
    if ((effects & 0x100) != 0)
    {
        uint32 current_game = r_u32(guest_stack + 4);
        uint32 player = r_u32(current_game + 0x50);
        if (r_u32(player + 0x154) == 0x40000)
            player_effect = (r_u32(player + 0x128) & 8) != 0;
    }
    sint32 delay = r_s16(object + 0x10e);
    uint32 delay_value = r_u16(object + 0x10e);
    if (delay != 0)
    {
        uint32 result = delay_value - 1;
        w_u16(object + 0x10e, result);
        return result;
    }
    sint32 limit = player_effect ? 15 : extended ? 20 : 10;
    if (tick >= limit)
        return G3Function800376D8(object, frame);
    if (tick == 0)
        G3Function800374FC(object, effects & 0x40, frame);
    if ((effects & 0x40) != 0 && tick == 0)
        w_u32(object + 0x10, r_u32(object + 0x10) | 0x800);
    if ((effects & 8) != 0 && tick == 0)
        G3Function80037C6C(object, frame);
    if ((effects & 0x80) != 0 && tick == 0)
        G3Function80037C08(object, frame);
    if ((effects & 2) != 0)
    {
        uint32 angle = r_u16(object + 0x108) + 0x100;
        w_u16(object + 0x108, angle);
        G3Function800378B8(object, (uint32)((sint32)(angle << 16) >> 16));
        G3Function80037824(object, 0x4b, 0x6e);
    }
    if ((effects & 4) != 0)
        G3Function80037824(object, 0x69, 0x32);
    if ((effects & 0x20) != 0)
        G3Function80037824(object, 0x4b, 0x4b);
    if ((effects & 1) != 0)
    {
        uint32 target = object;
        if (player_effect)
        {
            uint32 current_game = r_u32(guest_stack + 4);
            target = r_u32(current_game + 0x50);
        }
        uint32 first = G3Function80037CB8(0, frame);
        uint32 second = G3Function80037CB8(1, frame);
        uint32 third = ((uint32)tick & 1) != 0 ? first : second;
        uint32 radius = 0x6e - (uint32)tick * 3;
        w_u32(frame + 0x10, radius);
        G3Function80037964(target, first, 0x14, 0x1e, frame);
        w_u32(frame + 0x10, radius);
        G3Function80037964(target, second, 0x14, 0x1e, frame);
        w_u32(frame + 0x10, radius);
        G3Function80037964(target, third, 0x14, 0x1e, frame);
    }
    if ((effects & 0x10) != 0 && tick % 3 == 0)
    {
        uint32 target = object;
        if (player_effect)
        {
            uint32 current_game = r_u32(guest_stack + 4);
            target = r_u32(current_game + 0x50);
        }
        uint32 resource = G3Function80037CB8((uint32)tick & 1, frame);
        w_u32(frame + 0x10, 0x14);
        G3Function80037A20(target, resource, 0x14, 0x4b - (uint32)tick * 3, 0x4b, frame);
    }
    if ((effects & 0x200) != 0)
    {
        sint32 current_tick = r_s16(object + 0x10a);
        if (current_tick < 10 && current_tick % 3 == 2)
            w_u32(object + 0x18, r_u32(object + 0x18) & 0xffffffdf);
        else
        {
            w_u32(object + 0x18, r_u32(object + 0x18) | 0x20);
            w_u32(object + 0x14, (r_u32(object + 0x14) & 0xfffffeff) | 0x00800000);
        }
    }
    if ((effects & 0x800) != 0)
        G3Function80037DBC(object, frame);
    uint32 result = r_u16(object + 0x10a) + 1;
    w_u16(object + 0x10a, result);
    return result;
}

uint32 G3Function800378B8(uint32 object, uint32 angle)
{
    FUNCTION_MARKER(0x800378b8, "SLUS_008.06");
    uint32 result = (r_u16(object + 0x64) + angle) & 0xfff;
    w_u16(object + 0x64, result);
    return result;
}

uint32 G3Function80037824(uint32 object, uint32 strength, uint32 radius)
{
    FUNCTION_MARKER(0x80037824, "SLUS_008.06");
    sint32 x = r_s16(object + 0x68);
    uint32 factor = strength * 41;
    sint32 scaled_x = (sint32)((uint32)x * factor) >> 12;
    sint32 next_x = scaled_x < 16 ? 16 : scaled_x;
    sint32 y = r_s16(object + 0x6a);
    uint32 product_y = (uint32)y * factor;
    w_u16(object + 0x68, (uint32)next_x);
    sint32 scaled_y = (sint32)product_y >> 12;
    sint32 next_y = scaled_y < 16 ? 16 : scaled_y;
    sint32 z = r_s16(object + 0x6c);
    sint32 scaled_z = (sint32)((uint32)z * (radius * 41)) >> 12;
    uint32 result = scaled_z < 16;
    w_u16(object + 0x6a, (uint32)next_y);
    w_u16(object + 0x6c, (uint32)(result != 0 ? 16 : scaled_z));
    return result;
}

uint32 G3Function80037CB8(uint32 variant, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80037cb8, "SLUS_008.06");
    uint32 resource = G3Function8003AA48(variant != 0 ? 0x8009daa0 : 0x8009daac);
    if (resource == 0)
        return 0;
    uint32 models = r_u32(resource + 0x0c);
    return r_u32(models);
}

uint32 G3Function80037C6C(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80037c6c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    w_u32(frame + 0x18, 0x80041de0);
    uint32 pool = r_u32(0x800a27fc);
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x1c, 8);
    w_u32(frame + 0x10, pool);
    return G3Function800425CC(object, 0, 0, 0, frame);
}

uint32 G3Function80037C08(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80037c08, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 pool = r_u32(0x800a27fc);
    uint32 words[2];
    for (uint32 i = 0; i < 2; ++i)
    {
        uint32 address = 0x8009da98 + i * 4;
        r_masked_u32(address, 15);
        words[i] = r_masked_u32(address, 15);
    }
    for (uint32 i = 0; i < 2; ++i)
    {
        uint32 address = frame + 0x20 + i * 4;
        uint32 high_address = address + 3;
        uint32 high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, words[i] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        uint32 low_lane = address & 3;
        w_masked_u32(address & 0xfffffffc, words[i] << (low_lane * 8), (15 << low_lane) & 15);
    }
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x18, 0);
    w_u32(frame + 0x10, pool);
    return G3Function8004297C(object, 0, frame + 0x20, 0, frame);
}

uint32 G3Function80037D10(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80037d10, "SLUS_008.06");
    uint32 descriptor = r_u32(object + 0x1c);
    sint32 model_index = r_s16(object + 0xa2);
    uint32 models = r_u32(descriptor + 0x0c);
    uint32 model = r_u32(models + ((uint32)model_index << 2));
    sint32 count = r_s16(model + 6);
    sint32 adjusted = count + 7;
    if (adjusted < 0)
        adjusted = count + 14;
    uint32 step = (uint32)(adjusted >> 3);
    uint32 record = object + 0x110;
    w_u32(object + 0xa4, record);
    G3FillMemory80086094(record, 0, 0x40);
    count = r_s16(model + 6);
    if (count <= 0)
        return (uint32)count;
    uint32 index = 0;
    uint32 result;
    do
    {
        w_u16(record + 6, index + 1);
        count = r_s16(model + 6);
        index += step;
        result = (sint32)index < count;
        record += 8;
    } while (result != 0);
    return result;
}

uint32 G3Function80037DBC(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80037dbc, "SLUS_008.06");
    uint32 record = object + 0x110;
    for (uint32 i = 0; i < 8; ++i)
    {
        if (r_s16(record + 6) > 0)
        {
            for (uint32 axis = 0; axis < 3; ++axis)
            {
                sint32 random = (sint32)G3SdkRandom80027FA8();
                uint32 coordinate = r_u16(record + axis * 2);
                w_u16(record + axis * 2, coordinate - 0x100 + (uint32)(random % 512));
            }
        }
        record += 8;
    }
    return 0;
}

uint32 G3Function8004297C(uint32 object, uint32 mode, uint32 vector, uint32 option, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004297c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 pool = r_u32(guest_stack + 0x10);
    uint32 flags = r_u32(guest_stack + 0x14);
    uint32 callback = r_u32(guest_stack + 0x18);
    w_u32(frame + 0x1c, 0);
    w_u32(frame + 0x10, pool);
    w_u32(frame + 0x14, flags);
    w_u32(frame + 0x18, callback);
    return G3Function800425CC(object, mode, vector, option, frame);
}

uint32 G3Function80037964(uint32 object, uint32 resource, uint32 count, uint32 radius, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80037964, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 height = r_u32(guest_stack + 0x10);
    uint32 random = G3SdkRandom80027FA8();
    uint32 divisor = radius << 1;
    uint32 remainder = divisor == 0 ? random : (uint32)((sint64)(sint32)random % (sint32)divisor);
    uint32 x = radius - remainder;
    random = G3SdkRandom80027FA8();
    remainder = divisor == 0 ? random : (uint32)((sint64)(sint32)random % (sint32)divisor);
    uint32 y = radius - remainder;
    random = G3SdkRandom80027FA8();
    remainder = height == 0 ? random : (uint32)((sint64)(sint32)random % (sint32)height);
    w_u32(frame + 0x10, y);
    uint32 sum = height + remainder;
    uint32 incoming_v0 = sum >> 31;
    uint32 z = (uint32)((sint32)(sum + incoming_v0) >> 1);
    w_u32(frame + 0x14, z);
    return G3Function800378D0(object, resource, count, x, incoming_v0, frame);
}

uint32 G3Function80037A20(uint32 object, uint32 resource, uint32 count, uint32 radius, uint32 incoming_v0, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80037a20, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 half = (uint32)((sint32)(radius + (radius >> 31)) >> 1);
    uint32 diagonal = (uint32)((sint32)(radius * 3547) >> 12);
    uint32 height = r_u32(guest_stack + 0x10);
    uint32 offsets[12][2] = {
        {0, radius}, {0, 0 - radius}, {radius, 0}, {0 - radius, 0},
        {half, diagonal}, {half, 0 - diagonal}, {0 - half, diagonal}, {0 - half, 0 - diagonal},
        {diagonal, half}, {diagonal, 0 - half}, {0 - diagonal, half}, {0 - diagonal, 0 - half}
    };
    uint32 result = incoming_v0;
    for (uint32 i = 0; i < 12; ++i)
    {
        w_u32(frame + 0x10, offsets[i][1]);
        w_u32(frame + 0x14, height);
        result = G3Function800378D0(object, resource, count, offsets[i][0], result, frame);
    }
    return result;
}

uint32 G3Function800378D0(uint32 object, uint32 resource, uint32 count, uint32 x, uint32 incoming_v0, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800378d0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    if (resource == 0)
        return incoming_v0;
    uint32 pool = r_u32(0x800a27fc);
    w_u16(frame + 0x10, r_u16(object + 0x58));
    w_u16(frame + 0x12, r_u16(object + 0x5a));
    w_u16(frame + 0x14, r_u16(object + 0x5c));
    uint32 effect = G3Function800444E4(frame + 0x10, resource, pool, (uint32)((sint32)(count << 16) >> 16), frame);
    if (effect == 0)
        return effect;
    uint32 y = r_u32(guest_stack + 0x10);
    uint32 z = r_u32(guest_stack + 0x14);
    uint32 result = G3Function800449F0(effect, x, y, z, effect);
    return G3Function80044A40(effect, 0, 0, 0xfffffffe, result);
}

uint32 G3Function800449F0(uint32 effect, uint32 x, uint32 y, uint32 z, uint32 incoming_v0)
{
    FUNCTION_MARKER(0x800449f0, "SLUS_008.06");
    if (effect == 0)
        return incoming_v0;
    uint32 flags = r_u32(effect + 0x0c);
    w_u16(effect + 0x4c, x);
    w_u16(effect + 0x4e, y);
    w_u16(effect + 0x50, z);
    uint32 result = flags & 0xfffffffd;
    w_u32(effect + 0x0c, result);
    uint32 linked = r_u32(effect + 4);
    if (linked != 0)
    {
        flags = r_u32(linked + 0x0c);
        w_u16(linked + 0x4c, x);
        w_u16(linked + 0x4e, y);
        w_u16(linked + 0x50, z);
        result = flags & 0xfffffffd;
        w_u32(linked + 0x0c, result);
    }
    return result;
}

uint32 G3Function80044A40(uint32 effect, uint32 x, uint32 y, uint32 z, uint32 incoming_v0)
{
    FUNCTION_MARKER(0x80044a40, "SLUS_008.06");
    if (effect == 0)
        return incoming_v0;
    uint32 flags = r_u32(effect + 0x0c);
    w_u16(effect + 0x52, x);
    w_u16(effect + 0x54, y);
    w_u16(effect + 0x56, z);
    uint32 result = flags & 0xfffffffd;
    w_u32(effect + 0x0c, result);
    uint32 linked = r_u32(effect + 4);
    if (linked != 0)
    {
        flags = r_u32(linked + 0x0c);
        w_u16(linked + 0x52, x);
        w_u16(linked + 0x54, y);
        w_u16(linked + 0x56, z);
        result = flags & 0xfffffffd;
        w_u32(linked + 0x0c, result);
    }
    return result;
}

uint32 G3Function800444E4(uint32 position, uint32 resource, uint32 pool, uint32 count, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800444e4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 effect = G3Function800415CC(pool, frame);
    if (effect == 0)
        return 0;
    w_u32(frame + 0x10, effect);
    return G3Function80044688(position, resource, pool, (uint32)((sint32)(count << 16) >> 16), frame);
}

uint32 G3Function80044688(uint32 position, uint32 resource, uint32 pool, uint32 count, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80044688, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 owner = r_u32(guest_stack + 0x10);
    w_u16(frame + 0x10, count);
    if (resource == 0)
        return 0;
    uint32 face = r_u32(resource + 0x14);
    sint32 lifetime = (sint32)(count << 16) >> 16;
    uint32 effect = 0;
    for (uint32 i = 0; i < 2; ++i)
    {
        effect = G3Function8004161C(pool, frame);
        if (effect == 0)
            break;
        uint32 first_index = r_u16(face);
        uint32 vertices = r_u32(resource + 8);
        uint32 second_index = r_u16(face + 2);
        uint32 third_index = r_u16(face + 4);
        uint32 high_address = position + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low_lane = position & 3;
        uint32 low = r_masked_u32(position & 0xfffffffc, (15 << low_lane) & 15);
        uint32 packed = low_lane == 0 ? low : (high << ((4 - low_lane) * 8)) | (low >> (low_lane * 8));
        uint32 z = (uint32)r_s16(position + 4);
        uint32 destination = effect + 0x1c;
        high_address = destination + 3;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, packed >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, packed << (low_lane * 8), (15 << low_lane) & 15);
        w_u16(effect + 0x20, z);
        uint32 vertex = vertices + (first_index << 3);
        w_u16(effect + 0x24, r_u16(vertex));
        w_u16(effect + 0x26, r_u16(vertex + 2));
        w_u16(effect + 0x28, r_u16(vertex + 4));
        vertex = vertices + (second_index << 3);
        w_u16(effect + 0x2c, r_u16(vertex));
        w_u16(effect + 0x2e, r_u16(vertex + 2));
        w_u16(effect + 0x30, r_u16(vertex + 4));
        vertex = vertices + (third_index << 3);
        w_u16(effect + 0x34, r_u16(vertex));
        w_u16(effect + 0x36, r_u16(vertex + 2));
        uint32 last = r_u16(vertex + 4);
        uint32 current_lifetime = r_u16(frame + 0x10);
        w_u16(effect + 0x22, current_lifetime);
        w_u16(effect + 0x38, last);
        G3Function80041664(effect, face, resource, pool, frame);
        uint32 flags = r_u32(effect + 0x0c);
        if ((flags & 4) != 0)
        {
            if (lifetime == 0)
                w_u16(effect + 0x22, 0xffff);
        }
        else if (r_s16(effect + 0x22) == -1)
            w_u16(effect + 0x22, 0x1e);
        face += 0x0c;
        flags = r_u32(effect + 0x0c);
        w_u32(effect + 0x10, 0x80041de0);
        w_u32(effect + 0x14, owner);
        w_u32(effect + 0x0c, flags | 2);
        G3Function80045B74(pool + 0x11c10, effect);
    }
    uint32 game = r_u32(0x80097048);
    uint32 transform = r_u32(game + 0x4c);
    G3Function80041CE4(owner + 0x0c, transform, position, 0, frame);
    return effect;
}

uint32 G3Function80041664(uint32 effect, uint32 face, uint32 resource, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80041664, "SLUS_008.06");
    uint32 result;
    if ((r_u8(face + 7) & 2) != 0)
    {
        w_u32(effect + 0x0c, r_u32(effect + 0x0c) | 1);
        uint32 texture = r_u32(face + 8);
        w_u32(effect + 8, texture);
        uint32 table = r_u32(resource + 0x1c);
        if (table != 0)
        {
            sint32 count = r_s32(table);
            uint32 record = table + 4;
            for (sint32 i = 0; i < count; ++i)
            {
                if (r_u32(record) == texture)
                {
                    uint32 flags = r_u32(effect + 0x0c);
                    uint32 source = r_u32(effect + 8);
                    w_u32(effect + 0x0c, flags | 4);
                    uint32 words[4];
                    for (uint32 j = 0; j < 4; ++j)
                        words[j] = r_u32(source + j * 4);
                    for (uint32 j = 0; j < 4; ++j)
                        w_u32(effect + 0x60 + j * 4, words[j]);
                    w_u32(effect + 0x70, record);
                    w_u16(effect + 0x74, 0);
                    break;
                }
                record += 0x0c;
            }
        }
        uint32 source = r_u32(effect + 8);
        result = (r_u32(source + 0x0c) & 0x03ffffff) | 0x24000000;
    }
    else
    {
        w_u32(effect + 0x0c, r_u32(effect + 0x0c) & 0xfffffffa);
        result = (r_u32(face + 8) & 0x03ffffff) | 0x20000000;
    }
    w_u32(effect + 0x18, result);
    return result;
}

uint32 G3Function80041CE4(uint32 output, uint32 transform, uint32 position, uint32 mode, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80041ce4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    sint32 target_x = r_s16(transform);
    sint32 source_x = r_s16(position);
    uint32 x = (uint32)(target_x - source_x);
    w_u32(frame + 0x10, x);
    sint32 target_y = r_s16(transform + 2);
    sint32 source_y = r_s16(position + 2);
    w_u32(frame + 0x18, 0);
    uint32 y = (uint32)(target_y - source_y);
    w_u32(frame + 0x14, y);
    uint32 yaw = (G3Function8002F0C4(y, x, frame) + 0x400) & 0xfff;
    sint32 target_z = r_s16(transform + 4);
    sint32 source_z = r_s16(position + 4);
    uint32 z = (uint32)(target_z - source_z);
    uint32 length = G3Function8002FBF8(frame + 0x10, frame);
    uint32 pitch = (G3Function8002F0C4(length, z, frame) + 0xc00) & 0xfff;
    G3Function8002F718(output);
    if ((mode << 16) != 0)
        G3Function8005BA5C(mode & 0xfff, output, frame);
    G3Function8005B94C(pitch, output, frame);
    G3Function8005BB6C(yaw, output, frame);
    w_u32(output + 0x14, (uint32)r_s16(position));
    w_u32(output + 0x18, (uint32)r_s16(position + 2));
    uint32 result = (uint32)r_s16(position + 4);
    w_u32(output + 0x1c, result);
    return result;
}

uint32 G3Function8002F0C4(uint32 y, uint32 x, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002f0c4, "SLUS_008.06");
    if (x == 0)
        x = 1;
    if (y == 0)
        return (sint32)x < 1 ? 0x800 : 0;
    uint32 absolute_x = (sint32)x < 0 ? 0 - x : x;
    uint32 absolute_y = (sint32)y < 0 ? 0 - y : y;
    uint32 numerator;
    uint32 denominator;
    uint32 base;
    bool subtract;
    if ((sint32)x > 0)
    {
        if ((sint32)y > 0)
        {
            subtract = (sint32)absolute_x < (sint32)absolute_y;
            numerator = subtract ? absolute_x : absolute_y;
            denominator = subtract ? absolute_y : absolute_x;
            base = subtract ? 0x400 : 0;
        }
        else
        {
            subtract = (sint32)absolute_y < (sint32)absolute_x;
            numerator = subtract ? absolute_y : absolute_x;
            denominator = subtract ? absolute_x : absolute_y;
            base = subtract ? 0x1000 : 0xc00;
        }
    }
    else if ((sint32)y > 0)
    {
        subtract = !((sint32)absolute_x < (sint32)absolute_y);
        numerator = subtract ? absolute_y : absolute_x;
        denominator = subtract ? absolute_x : absolute_y;
        base = subtract ? 0x800 : 0x400;
    }
    else
    {
        subtract = !((sint32)absolute_y < (sint32)absolute_x);
        numerator = subtract ? absolute_x : absolute_y;
        denominator = subtract ? absolute_y : absolute_x;
        base = subtract ? 0xc00 : 0x800;
    }
    numerator <<= 9;
    uint32 quotient = (uint32)((sint64)(sint32)numerator / (sint32)denominator);
    uint32 result = subtract ? base - quotient : base + quotient;
    return (uint32)((sint32)(result << 16) >> 16);
}

uint32 G3Function8005BA5C(uint32 angle, uint32 matrix, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005ba5c, "SLUS_008.06");
    G3AnglePair pair = G3Function8005B6B0(angle);
    uint32 first[3];
    first[2] = (uint32)r_s16(matrix + 4);
    uint32 packed = r_u32(matrix);
    first[0] = (uint32)((sint32)(packed << 16) >> 16);
    first[1] = (uint32)((sint32)packed >> 16);
    xport_gte_write_data(8, pair.cosine);
    xport_gte_write_data(11, first[2]);
    xport_gte_write_data(9, first[0]);
    xport_gte_write_data(10, first[1]);
    packed = r_u32(matrix + 0x0c);
    xport_gte_execute(0x198003d);
    uint32 second[3];
    second[2] = (uint32)r_s16(matrix + 0x10);
    second[0] = (uint32)((sint32)(packed << 16) >> 16);
    second[1] = (uint32)((sint32)packed >> 16);
    uint32 cosine_first[3];
    for (uint32 i = 0; i < 3; ++i)
        cosine_first[i] = xport_gte_read_data(9 + i);
    xport_gte_write_data(8, pair.sine);
    for (uint32 i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, second[i]);
    xport_gte_execute(0x198003d);
    uint32 sum[3];
    for (uint32 i = 0; i < 3; ++i)
        sum[i] = xport_gte_read_data(9 + i);
    for (uint32 i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, first[i]);
    xport_gte_execute(0x198003d);
    for (uint32 i = 0; i < 3; ++i)
        sum[i] += cosine_first[i];
    uint32 sine_first[3];
    for (uint32 i = 0; i < 3; ++i)
        sine_first[i] = xport_gte_read_data(9 + i);
    xport_gte_write_data(8, pair.cosine);
    for (uint32 i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, second[i]);
    xport_gte_execute(0x198003d);
    w_u32(matrix, (sum[0] & 0xffff) | (sum[1] << 16));
    w_u16(matrix + 4, sum[2]);
    uint32 difference[3];
    for (uint32 i = 0; i < 3; ++i)
        difference[i] = xport_gte_read_data(9 + i) - sine_first[i];
    w_u32(matrix + 0x0c, (difference[0] & 0xffff) | (difference[1] << 16));
    w_u16(matrix + 0x10, difference[2]);
    return pair.cosine;
}

uint32 G3Function8005B94C(uint32 angle, uint32 matrix, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005b94c, "SLUS_008.06");
    G3AnglePair pair = G3Function8005B6B0(angle);
    uint32 first[3];
    first[0] = (uint32)r_s16(matrix + 6);
    uint32 packed = r_u32(matrix + 8);
    first[1] = (uint32)((sint32)(packed << 16) >> 16);
    first[2] = (uint32)((sint32)packed >> 16);
    xport_gte_write_data(8, pair.cosine);
    for (uint32 i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, first[i]);
    packed = r_u32(matrix + 0x0c);
    xport_gte_execute(0x198003d);
    uint32 second[3];
    second[2] = (uint32)r_s16(matrix + 0x10);
    second[0] = (uint32)((sint32)(packed << 16) >> 16);
    second[1] = (uint32)((sint32)packed >> 16);
    uint32 cosine_first[3];
    for (uint32 i = 0; i < 3; ++i)
        cosine_first[i] = xport_gte_read_data(9 + i);
    xport_gte_write_data(8, pair.sine);
    for (uint32 i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, second[i]);
    xport_gte_execute(0x198003d);
    uint32 difference[3];
    for (uint32 i = 0; i < 3; ++i)
        difference[i] = xport_gte_read_data(9 + i);
    for (uint32 i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, first[i]);
    xport_gte_execute(0x198003d);
    for (uint32 i = 0; i < 3; ++i)
        difference[i] = cosine_first[i] - difference[i];
    uint32 sine_first[3];
    for (uint32 i = 0; i < 3; ++i)
        sine_first[i] = xport_gte_read_data(9 + i);
    xport_gte_write_data(8, pair.cosine);
    for (uint32 i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, second[i]);
    xport_gte_execute(0x198003d);
    w_u16(matrix + 6, difference[0]);
    w_u32(matrix + 8, (difference[1] & 0xffff) | (difference[2] << 16));
    uint32 sum[3];
    for (uint32 i = 0; i < 3; ++i)
        sum[i] = sine_first[i] + xport_gte_read_data(9 + i);
    w_u32(matrix + 0x0c, (sum[0] & 0xffff) | (sum[1] << 16));
    w_u16(matrix + 0x10, sum[2]);
    return pair.cosine;
}

uint32 G3Function8005BB6C(uint32 angle, uint32 matrix, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005bb6c, "SLUS_008.06");
    G3AnglePair pair = G3Function8005B6B0(angle);
    uint32 first_word = r_u32(matrix);
    uint32 second_word = r_u32(matrix + 4);
    uint32 first[3];
    first[0] = (uint32)((sint32)(first_word << 16) >> 16);
    first[1] = (uint32)((sint32)first_word >> 16);
    first[2] = (uint32)((sint32)(second_word << 16) >> 16);
    uint32 second[3];
    second[0] = (uint32)((sint32)second_word >> 16);
    xport_gte_write_data(8, pair.cosine);
    for (uint32 i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, first[i]);
    uint32 third_word = r_u32(matrix + 8);
    xport_gte_execute(0x198003d);
    second[1] = (uint32)((sint32)(third_word << 16) >> 16);
    second[2] = (uint32)((sint32)third_word >> 16);
    uint32 cosine_first[3];
    for (uint32 i = 0; i < 3; ++i)
        cosine_first[i] = xport_gte_read_data(9 + i);
    xport_gte_write_data(8, pair.sine);
    for (uint32 i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, second[i]);
    xport_gte_execute(0x198003d);
    uint32 difference[3];
    for (uint32 i = 0; i < 3; ++i)
        difference[i] = xport_gte_read_data(9 + i);
    for (uint32 i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, first[i]);
    xport_gte_execute(0x198003d);
    for (uint32 i = 0; i < 3; ++i)
        difference[i] = cosine_first[i] - difference[i];
    uint32 sine_first[3];
    for (uint32 i = 0; i < 3; ++i)
        sine_first[i] = xport_gte_read_data(9 + i);
    xport_gte_write_data(8, pair.cosine);
    for (uint32 i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, second[i]);
    xport_gte_execute(0x198003d);
    w_u32(matrix, (difference[0] & 0xffff) | (difference[1] << 16));
    uint32 sum[3];
    for (uint32 i = 0; i < 3; ++i)
        sum[i] = sine_first[i] + xport_gte_read_data(9 + i);
    w_u32(matrix + 4, (difference[2] & 0xffff) | (sum[0] << 16));
    w_u32(matrix + 8, (sum[1] & 0xffff) | (sum[2] << 16));
    return pair.cosine;
}

uint32 G3Function800425CC(uint32 object, uint32 mode, uint32 vector, uint32 option, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800425cc, "SLUS_008.06");
    uint32 frame = guest_stack - 0xc0;
    w_u32(guest_stack, object);
    uint32 flags = r_u32(guest_stack + 0x1c);
    object = r_u32(guest_stack);
    uint32 pool = r_u32(guest_stack + 0x10);
    w_u32(guest_stack + 4, mode);
    w_u32(guest_stack + 8, vector);
    w_u32(guest_stack + 0x0c, option);
    w_u16(frame + 0x88, 0);
    uint32 descriptor = r_u32(object + 0x1c);
    sint32 model_index = r_s16(object + 0xa2);
    uint32 models = r_u32(descriptor + 0x0c);
    uint32 model = r_u32(models + ((uint32)model_index << 2));
    uint32 vertices = r_u32(model + 8);
    w_u32(frame + 0x84, vertices);
    uint32 face = r_u32(model + 0x14);
    if ((flags & 8) != 0)
    {
        for (uint32 i = 0; i < 20; ++i)
            w_u32(frame + 0x30 + i * 4, 0);
    }
    sint32 face_count = r_s16(model + 4);
    sint32 divisor = r_s16(model + 6);
    uint32 quotient = divisor == 0 ? (face_count < 0 ? 1 : uint32(0xffffffff)) : (uint32)(face_count / divisor);
    uint32 counter = 0;
    uint32 current_object = r_u32(guest_stack);
    uint32 matrix = r_u32(current_object + 0x34);
    w_u32(frame + 0x80, quotient + 1);
    uint32 owner = G3Function800415CC(pool, frame);
    if (owner == 0)
        return owner;
    for (uint32 group = 0; group < 2; ++group)
    {
        uint32 words[4];
        for (uint32 i = 0; i < 4; ++i)
            words[i] = r_u32(matrix + group * 16 + i * 4);
        for (uint32 i = 0; i < 4; ++i)
            w_u32(owner + 0x0c + group * 16 + i * 4, words[i]);
    }
    face_count = r_s16(model + 4);
    uint32 end = face + (uint32)face_count * 12;
    uint32 result = face < end;
    if (result == 0)
        return result;
    w_u32(frame + 0x90, flags & 8);
    do
    {
        sint32 threshold = r_s32(frame + 0x80);
        if ((sint32)counter >= threshold)
        {
            owner = G3Function800415CC(pool, frame);
            counter = 0;
            if (owner != 0)
            {
                for (uint32 group = 0; group < 2; ++group)
                {
                    uint32 words[4];
                    for (uint32 i = 0; i < 4; ++i)
                        words[i] = r_u32(matrix + group * 16 + i * 4);
                    for (uint32 i = 0; i < 4; ++i)
                        w_u32(owner + 0x0c + group * 16 + i * 4, words[i]);
                }
            }
            else
                face = end;
            matrix += 0x20;
        }
        if (owner != 0)
        {
            uint32 effect = G3Function8004161C(pool, frame);
            if (effect == 0)
                face = end;
            else
            {
                uint32 initializer = r_u32(guest_stack + 0x14);
                uint32 callback = r_u32(guest_stack + 0x18);
                current_object = r_u32(guest_stack);
                vertices = r_u32(frame + 0x84);
                mode = r_u32(guest_stack + 4);
                vector = r_u32(guest_stack + 8);
                option = r_u32(guest_stack + 0x0c);
                if (initializer != 0)
                {
                    w_u32(frame + 0x18, mode);
                    initializer = r_u32(guest_stack + 0x14);
                    w_u32(frame + 0x10, face);
                    w_u32(frame + 0x24, pool);
                    w_u32(frame + 0x14, vertices);
                    w_u32(frame + 0x1c, vector);
                    w_u32(frame + 0x20, option);
                    w_u32(frame + 0x28, 0x1e);
                    G3DispatchFaceInitializer(initializer, effect, callback, owner, current_object, frame);
                }
                else
                {
                    w_u32(frame + 0x10, model);
                    w_u32(frame + 0x14, face);
                    w_u32(frame + 0x28, pool);
                    w_u32(frame + 0x18, vertices);
                    w_u32(frame + 0x1c, mode);
                    w_u32(frame + 0x20, vector);
                    w_u32(frame + 0x24, option);
                    w_u32(frame + 0x2c, 0x1e);
                    G3Function80041A14(effect, callback, owner, current_object, frame);
                }
                G3Function80045B74(pool + 0x11c10, effect);
                if (r_u32(frame + 0x90) != 0)
                {
                    G3Function80045920(effect, frame);
                    sint32 index = r_s16(frame + 0x88);
                    uint32 slot = frame + 0x30 + ((uint32)index << 2);
                    bool have_matrix = true;
                    if (r_u32(slot) == 0)
                    {
                        uint32 allocation = G3Function800415CC(pool, frame);
                        w_u32(slot, allocation);
                        have_matrix = allocation != 0;
                    }
                    if (have_matrix)
                    {
                        owner = r_u32(slot);
                        for (uint32 group = 0; group < 2; ++group)
                        {
                            uint32 words[4];
                            for (uint32 i = 0; i < 4; ++i)
                                words[i] = r_u32(matrix + group * 16 + i * 4);
                            for (uint32 i = 0; i < 4; ++i)
                                w_u32(owner + 0x0c + group * 16 + i * 4, words[i]);
                        }
                        w_u16(frame + 0x88, (uint32)((index + 1) % 20));
                    }
                }
            }
        }
        face += 0x0c;
        result = face < end;
        ++counter;
    } while (result != 0);
    return result;
}

uint32 G3Function80045920(uint32 effect, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80045920, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 random = G3SdkRandom80027FA8();
    w_u8(effect + 0x49, (random & 0x3f) - 0x10);
    random = G3SdkRandom80027FA8();
    uint32 value = (random & 0x3f) - 0x10;
    w_u16(effect + 0x4a, (uint32)((sint32)(value << 24) >> 24));
    random = G3SdkRandom80027FA8();
    uint32 owner = r_u32(effect + 0x14);
    w_u8(effect + 0x4b, (random & 0x3f) - 0x10);
    w_u32(effect + 0x0c, r_u32(effect + 0x0c) | 0x80);
    G3Function8002F718(owner + 0x0c);
    random = G3SdkRandom80027FA8();
    owner = r_u32(effect + 0x14);
    G3Function8005B94C(random & 0xfff, owner + 0x0c, frame);
    random = G3SdkRandom80027FA8();
    owner = r_u32(effect + 0x14);
    G3Function8005BA5C(random & 0xfff, owner + 0x0c, frame);
    random = G3SdkRandom80027FA8();
    owner = r_u32(effect + 0x14);
    return G3Function8005BB6C(random & 0xfff, owner + 0x0c, frame);
}

uint32 G3Function80081A90(uint32 kind, uint32 mode, uint32 index, uint32 strength, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80081a90, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 first = r_u32(guest_stack + 0x10);
    uint32 second = r_u32(guest_stack + 0x14);
    uint32 position = r_u32(guest_stack + 0x18);
    uint32 rate = r_u32(guest_stack + 0x24);
    uint32 borrowed = r_u32(guest_stack + 0x28);
    sint32 signed_radius = r_s16(guest_stack + 0x20);
    uint32 radius = r_u32(guest_stack + 0x20);
    if (signed_radius <= 0)
        return 1;
    w_u32(frame + 0x10, (uint32)((sint32)(first << 16) >> 16));
    kind = (uint32)((sint32)(kind << 16) >> 16);
    mode = (uint32)((sint32)(mode << 16) >> 16);
    index = (uint32)((sint32)(index << 16) >> 16);
    strength = (uint32)((sint32)(strength << 16) >> 16);
    w_u32(frame + 0x14, (uint32)((sint32)(second << 16) >> 16));
    if (G3Function8008194C(kind, mode, index, strength, frame) == 0)
        return 0;
    uint32 effect = r_u32(0x800a2c3c);
    uint32 target = r_u32(guest_stack + 0x1c);
    uint32 flags = r_u16(effect + 6);
    w_u32(effect + 0x20, target);
    w_u16(effect + 0x2e, radius);
    w_u16(effect + 0x30, rate);
    w_u16(effect + 6, flags | 0x10);
    if (borrowed != 0)
        w_u32(effect + 0x24, position);
    else
    {
        uint32 current_effect = r_u32(0x800a2c3c);
        uint32 high_address = position + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low_lane = position & 3;
        uint32 low = r_masked_u32(position & 0xfffffffc, (15 << low_lane) & 15);
        uint32 packed = low_lane == 0 ? low : (high << ((4 - low_lane) * 8)) | (low >> (low_lane * 8));
        uint32 z = (uint32)r_s16(position + 4);
        uint32 destination = effect + 0x28;
        high_address = destination + 3;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, packed >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, packed << (low_lane * 8), (15 << low_lane) & 15);
        w_u16(effect + 0x2c, z);
        w_u32(current_effect + 0x24, current_effect + 0x28);
    }
    return 1;
}

uint32 G3Function80041A14(uint32 effect, uint32 callback, uint32 owner, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80041a14, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 face = r_u32(guest_stack + 0x14);
    uint32 coordinate = r_u16(owner + 0x20);
    uint32 first_index = r_u16(face);
    uint32 second_index = r_u16(face + 2);
    uint32 third_index = r_u16(face + 4);
    w_u16(effect + 0x1c, coordinate);
    w_u16(effect + 0x1e, r_u16(owner + 0x24));
    w_u16(effect + 0x20, r_u16(owner + 0x28));
    uint32 vertices = r_u32(guest_stack + 0x18);
    uint32 vertex = vertices + (first_index << 3);
    w_u16(effect + 0x24, r_u16(vertex));
    w_u16(effect + 0x26, r_u16(vertex + 2));
    w_u16(effect + 0x28, r_u16(vertex + 4));
    vertex = vertices + (second_index << 3);
    w_u16(effect + 0x2c, r_u16(vertex));
    w_u16(effect + 0x2e, r_u16(vertex + 2));
    w_u16(effect + 0x30, r_u16(vertex + 4));
    vertex = vertices + (third_index << 3);
    w_u16(effect + 0x34, r_u16(vertex));
    w_u16(effect + 0x36, r_u16(vertex + 2));
    coordinate = r_u16(vertex + 4);
    uint32 model = r_u32(guest_stack + 0x10);
    uint32 origin = r_u32(guest_stack + 0x1c);
    uint32 velocity = r_u32(guest_stack + 0x20);
    uint32 acceleration = r_u32(guest_stack + 0x24);
    uint32 pool = r_u32(guest_stack + 0x28);
    uint32 lifetime = r_u16(guest_stack + 0x2c);
    w_u16(effect + 0x38, coordinate);
    if ((r_u32(object + 0x10) & 0x80) != 0)
    {
        w_u32(frame + 0x10, object);
        G3Function80041764(effect, face, model, pool, frame);
    }
    else
        G3Function80041664(effect, face, model, pool, frame);
    w_u32(effect + 0x10, callback != 0 ? callback : 0x8004197c);
    w_u32(effect + 0x14, owner);
    uint32 z;
    if (origin != 0)
    {
        if (velocity != 0)
        {
            sint32 x = r_s16(effect + 0x24);
            sint32 base_x = r_s16(origin);
            uint32 add_x = r_u16(velocity);
            sint32 y = r_s16(effect + 0x26);
            w_u16(effect + 0x4c, add_x + (uint32)((x - base_x) >> 2));
            sint32 base_y = r_s16(origin + 2);
            uint32 add_y = r_u16(velocity + 2);
            sint32 last = r_s16(effect + 0x28);
            w_u16(effect + 0x4e, add_y + (uint32)((y - base_y) >> 2));
            sint32 base_z = r_s16(origin + 4);
            uint32 add_z = r_u16(velocity + 4);
            z = add_z + (uint32)((last - base_z) >> 2);
        }
        else
        {
            for (uint32 i = 0; i < 2; ++i)
            {
                sint32 value = r_s16(effect + 0x24 + i * 2);
                sint32 base = r_s16(origin + i * 2);
                w_u16(effect + 0x4c + i * 2, (uint32)((value - base) >> 2));
            }
            sint32 value = r_s16(effect + 0x28);
            sint32 base = r_s16(origin + 4);
            z = (uint32)((value - base) >> 2);
        }
    }
    else if (velocity != 0)
    {
        uint32 x = r_u16(effect + 0x24);
        uint32 add_x = r_u16(velocity);
        uint32 y = r_u16(effect + 0x26);
        w_u16(effect + 0x4c, add_x + (uint32)((sint32)(x << 16) >> 18));
        uint32 add_y = r_u16(velocity + 2);
        uint32 last = r_u16(effect + 0x28);
        w_u16(effect + 0x4e, add_y + (uint32)((sint32)(y << 16) >> 18));
        uint32 add_z = r_u16(velocity + 4);
        z = add_z + (uint32)((sint32)(last << 16) >> 18);
    }
    else
    {
        uint32 x = r_u16(effect + 0x24);
        uint32 y = r_u16(effect + 0x26);
        w_u16(effect + 0x4c, (uint32)((sint32)(x << 16) >> 18));
        uint32 last = r_u16(effect + 0x28);
        w_u16(effect + 0x4e, (uint32)((sint32)(y << 16) >> 18));
        z = (uint32)((sint32)(last << 16) >> 18) + 0x32;
    }
    w_u16(effect + 0x50, z);
    uint32 result;
    if (acceleration != 0)
    {
        w_u16(effect + 0x52, r_u16(acceleration));
        w_u16(effect + 0x54, r_u16(acceleration + 2));
        result = r_u16(acceleration + 4);
        w_u16(effect + 0x56, result);
    }
    else
    {
        result = 0xfffffffd;
        w_u16(effect + 0x52, 0);
        w_u16(effect + 0x54, 0);
        w_u16(effect + 0x56, result);
    }
    w_u16(effect + 0x22, lifetime);
    return result;
}

uint32 G3Function80041764(uint32 effect, uint32 face, uint32 model, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80041764, "SLUS_008.06");
    uint32 result;
    if ((r_u8(face + 7) & 2) != 0)
    {
        w_u32(effect + 0x0c, (r_u32(effect + 0x0c) | 1) & 0xfffffffb);
        uint32 table = r_u32(model + 0x1c);
        if (table == 0)
            w_u32(effect + 8, r_u32(face + 8));
        else
        {
            uint32 texture = r_u32(face + 8);
            w_u32(effect + 8, texture);
            sint32 count = r_s32(table);
            uint32 record = table + 4;
            for (sint32 i = 0; i < count; ++i)
            {
                if (r_u32(record) == texture)
                {
                    uint32 object = r_u32(guest_stack + 0x10);
                    uint32 divisor = r_u32(record + 8);
                    uint32 tick = (uint32)r_s16(object + 0x9e);
                    uint32 quotient = divisor == 0 ? uint32(0xffffffff) : tick / divisor;
                    uint32 frames = r_u32(record + 4);
                    uint32 index = frames == 0 ? quotient : quotient % frames;
                    uint32 base = r_u32(face + 8);
                    w_u32(effect + 8, base + (index << 4) + 0x10);
                    break;
                }
                record += 0x0c;
            }
        }
        uint32 texture = r_u32(effect + 8);
        result = (r_u32(texture + 0x0c) & 0x03ffffff) | 0x24000000;
    }
    else
    {
        w_u32(effect + 0x0c, r_u32(effect + 0x0c) & 0xfffffffa);
        result = (r_u32(face + 8) & 0x03ffffff) | 0x20000000;
    }
    w_u32(effect + 0x18, result);
    return result;
}

uint32 G3Function8008194C(uint32 kind, uint32 mode, uint32 index, uint32 strength, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8008194c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 flags = r_u32(guest_stack + 0x14);
    uint32 scale = r_u16(guest_stack + 0x10);
    if ((kind & 0xffff) >= 2)
        return 0;
    sint32 signed_mode = (sint32)(mode << 16) >> 16;
    if (signed_mode < 0 || signed_mode >= 16)
        return 0;
    uint32 effect = G3Function80081754((uint32)((sint32)(kind << 16) >> 16), (uint32)((sint32)(index << 16) >> 16), 1, frame);
    if (effect == 0)
        return 0;
    if ((flags & 1) != 0)
        w_u16(effect + 6, r_u16(effect + 6) | 2);
    sint32 signed_strength = (sint32)(strength << 16) >> 16;
    uint32 amount = signed_strength < 0 ? 0 : signed_strength >= 0x1000 ? 0x1000 : strength;
    w_u16(effect + 8, amount);
    if ((sint32)(scale << 16) > 0)
        w_u16(effect + 0x0a, scale);
    uint32 table = 0x800a03a4 + (uint32)signed_mode * 8;
    uint32 data = r_u32(table + 4);
    w_u32(effect + 0x18, data);
    w_u32(effect + 0x14, data);
    sint32 count = r_s16(table);
    uint32 base = r_u32(table + 4);
    data = r_u32(effect + 0x18);
    w_u32(effect + 0x1c, base + (uint32)count * 6);
    sint32 current_scale = r_s16(effect + 0x0a);
    sint32 duration = r_s16(data + 2);
    uint32 product = (uint32)duration * (uint32)current_scale;
    w_u32(0x800a2c3c, effect);
    w_u16(effect + 0x0c, (uint32)((sint32)product >> 12));
    return 1;
}

uint32 G3Function80081754(uint32 kind, uint32 index, uint32 allocate, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80081754, "SLUS_008.06");
    sint32 signed_index = (sint32)(index << 16) >> 16;
    if (signed_index < 0 && allocate == 0)
        return 0;
    sint32 signed_kind = (sint32)(kind << 16) >> 16;
    uint32 base = 0x800a2a9c + (uint32)signed_kind * 0xd0;
    if (signed_index >= 0)
    {
        for (uint32 i = 0; i < 4; ++i)
        {
            uint32 slot = base + i * 0x34;
            if ((r_u16(slot + 6) & 1) != 0 && r_s16(slot + 4) == signed_index)
            {
                if (allocate != 0)
                {
                    w_u16(slot + 6, 1);
                    w_u16(slot + 8, 0x1000);
                    w_u16(slot + 2, 0);
                    w_u16(slot, 0);
                    w_u16(slot + 0x0a, 0x1e);
                }
                return slot;
            }
        }
        if (allocate == 0)
            return 0;
    }
    for (uint32 i = 0; i < 4; ++i)
    {
        uint32 slot = base + i * 0x34;
        if ((r_u16(slot + 6) & 1) == 0)
        {
            w_u16(slot + 6, 1);
            if (signed_index < 0)
            {
                w_u16(slot + 4, 0xffff);
                w_u16(slot + 8, 0x1000);
            }
            else
            {
                w_u16(slot + 8, 0x1000);
                w_u16(slot + 4, index);
            }
            w_u16(slot + 2, 0);
            w_u16(slot, 0);
            w_u16(slot + 0x0a, 0x1e);
            return slot;
        }
    }
    return 0;
}

uint32 G3Function8004197C(uint32 effect, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004197c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    sint32 lifetime = r_s16(effect + 0x22);
    uint32 unsigned_lifetime = r_u16(effect + 0x22);
    if (lifetime > 0)
    {
        w_u16(effect + 0x22, unsigned_lifetime - 1);
        lifetime = r_s16(effect + 0x22);
    }
    if (lifetime == 0)
        return G3Function80041578(effect, pool, frame);
    if ((r_u32(effect + 0x0c) & 2) == 0)
        G3Function80044B60(effect, frame);
    uint32 result = r_u32(effect + 0x0c) & 4;
    if (result != 0)
        result = G3Function80041880(effect, pool, frame);
    return result;
}

uint32 G3Function80041880(uint32 effect, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80041880, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 game = r_u32(0x80097048);
    sint32 mode = r_s16(game + 0x6786);
    if (mode == 6)
        return 6;
    uint32 animation = r_u32(effect + 0x70);
    if (animation == 0)
        return 6;
    uint32 tick = (uint32)r_s16(effect + 0x74);
    uint32 duration = r_u32(animation + 8);
    uint32 quotient = duration == 0 ? uint32(0xffffffff) : tick / duration;
    uint32 count = r_u32(animation + 4);
    uint32 index = count == 0 ? quotient : quotient % count;
    uint32 texture = r_u32(effect + 8);
    uint32 total = duration * count;
    if (index == 0 && tick >= total && r_s16(effect + 0x22) == -1)
        return G3Function80041578(effect, pool, frame);
    uint32 source = texture + 0x10 + (index << 4);
    for (uint32 i = 0; i < 4; ++i)
        w_u32(effect + 0x60 + i * 4, r_u32(source + i * 4));
    uint32 result = r_u16(effect + 0x74) + 1;
    w_u16(effect + 0x74, result);
    return result;
}

uint32 G3Function80044B60(uint32 effect, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80044b60, "SLUS_008.06");
    uint32 velocity_x = r_u16(effect + 0x4c);
    uint32 acceleration_x = r_u16(effect + 0x52);
    uint32 velocity_y = r_u16(effect + 0x4e);
    uint32 acceleration_y = r_u16(effect + 0x54);
    w_u16(effect + 0x4c, velocity_x + acceleration_x);
    uint32 velocity_z = r_u16(effect + 0x50);
    uint32 acceleration_z = r_u16(effect + 0x56);
    w_u16(effect + 0x4e, velocity_y + acceleration_y);
    uint32 x = r_u16(effect + 0x1c);
    velocity_x = r_u16(effect + 0x4c);
    w_u16(effect + 0x50, velocity_z + acceleration_z);
    uint32 y = r_u16(effect + 0x1e);
    velocity_y = r_u16(effect + 0x4e);
    w_u16(effect + 0x1c, x + velocity_x);
    uint32 z = r_u16(effect + 0x20);
    velocity_z = r_u16(effect + 0x50);
    uint32 result = y + velocity_y;
    w_u16(effect + 0x1e, result);
    w_u16(effect + 0x20, z + velocity_z);
    return result;
}

uint32 G3Function80041DE0(uint32 effect, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80041de0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    sint32 lifetime = r_s16(effect + 0x22);
    uint32 unsigned_lifetime = r_u16(effect + 0x22);
    if (lifetime > 0)
    {
        w_u16(effect + 0x22, unsigned_lifetime - 1);
        lifetime = r_s16(effect + 0x22);
    }
    if (lifetime == 0)
        return G3Function80041578(effect, pool, frame);
    uint32 flags = r_u32(effect + 0x0c);
    if ((flags & 0x20) != 0)
    {
        uint32 object = r_u32(effect + 0x4c);
        uint32 index = r_u32(effect + 0x50);
        uint32 matrices = r_u32(object + 0x30);
        uint32 matrix = matrices + (index << 5);
        w_u16(effect + 0x1c, r_u16(matrix + 0x14));
        w_u16(effect + 0x1e, r_u16(matrix + 0x18));
        w_u16(effect + 0x20, r_u16(matrix + 0x1c));
    }
    else if ((flags & 0x200) != 0)
    {
        bool exhausted = true;
        uint32 links = r_u32(effect + 0x4c);
        uint32 path = r_u32(links);
        if (path != 0)
        {
            uint32 point = G3Function8003FC34(path, effect + 0x50, frame);
            if (point != 0)
            {
                if ((r_u32(effect + 0x0c) & 0x400) != 0)
                {
                    links = r_u32(effect + 0x4c);
                    path = r_u32(links);
                    uint32 x = r_u16(point);
                    uint32 base = r_u32(path);
                    uint32 origin = r_u16(effect + 0x44);
                    uint32 reference = r_u16(base + 2);
                    w_u16(effect + 0x1c, origin + x - reference);
                    uint32 y = r_u16(point + 2);
                    reference = r_u16(base + 4);
                    origin = r_u16(effect + 0x46);
                    w_u16(effect + 0x1e, origin + y - reference);
                    uint32 z = r_u16(point + 4);
                    reference = r_u16(base + 6);
                    origin = r_u16(effect + 0x48);
                    w_u16(effect + 0x20, origin + z - reference);
                }
                else
                {
                    uint32 high_address = point + 3;
                    uint32 high_lane = high_address & 3;
                    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                    uint32 low_lane = point & 3;
                    uint32 low = r_masked_u32(point & 0xfffffffc, (15 << low_lane) & 15);
                    uint32 packed = low_lane == 0 ? low : (high << ((4 - low_lane) * 8)) | (low >> (low_lane * 8));
                    uint32 z = (uint32)r_s16(point + 4);
                    uint32 destination = effect + 0x1c;
                    high_address = destination + 3;
                    high_lane = high_address & 3;
                    w_masked_u32(high_address & 0xfffffffc, packed >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                    low_lane = destination & 3;
                    w_masked_u32(destination & 0xfffffffc, packed << (low_lane * 8), (15 << low_lane) & 15);
                    w_u16(effect + 0x20, z);
                }
                exhausted = false;
            }
        }
        links = r_u32(effect + 0x4c);
        path = r_u32(links + 8);
        if (path != 0)
        {
            uint32 point = G3Function8003FC34(path, effect + 0x54, frame);
            if (point != 0)
            {
                w_u32(effect + 0x0c, r_u32(effect + 0x0c) | 0x2000);
                w_u16(effect + 0x26, r_u16(point));
                w_u16(effect + 0x36, r_u16(point + 2));
                exhausted = false;
            }
        }
        if (exhausted)
            return G3Function80041578(effect, pool, frame);
    }
    else if ((flags & 2) == 0)
    {
        G3Function80044B60(effect, frame);
        if ((r_u32(effect + 0x0c) & 0x100) != 0)
        {
            sint32 z = r_s16(effect + 0x20);
            sint32 floor = r_s16(effect + 0x44);
            uint32 floor_value = r_u16(effect + 0x44);
            if (floor >= z)
            {
                uint32 current_flags = r_u32(effect + 0x0c);
                w_u16(effect + 0x20, floor_value);
                w_u32(effect + 0x0c, current_flags | 2);
            }
        }
    }
    if ((r_u32(effect + 0x0c) & 4) != 0)
        G3Function80041880(effect, pool, frame);
    uint32 owner = r_u32(effect + 0x14);
    uint32 owner_flags = r_u32(owner + 8);
    uint32 result = owner_flags | 2;
    if ((owner_flags & 2) != 0)
        return result;
    w_u32(owner + 8, result);
    flags = r_u32(effect + 0x0c);
    if ((flags & 0x60) == 0x60)
    {
        uint32 object = r_u32(effect + 0x4c);
        uint32 previous_x = r_u16(object + 0x70);
        uint32 current_x = r_u16(object + 0x58);
        if (((previous_x - current_x) << 16) != 0)
        {
            uint32 game = r_u32(0x80097048);
            uint32 camera = r_u32(game + 0x4c);
            uint32 camera_x = r_u16(camera);
            uint32 x = current_x - camera_x;
            w_u16(frame + 0x10, x);
            camera = r_u32(game + 0x4c);
            uint32 current_y = r_u16(object + 0x5a);
            uint32 camera_y = r_u16(camera + 2);
            uint32 y = current_y - camera_y;
            w_u16(frame + 0x12, y);
            camera = r_u32(game + 0x4c);
            previous_x = r_u16(object + 0x70);
            camera_x = r_u16(camera);
            w_u16(frame + 0x18, previous_x - camera_x);
            camera = r_u32(game + 0x4c);
            uint32 previous_y = r_u16(object + 0x72);
            camera_y = r_u16(camera + 2);
            w_u16(frame + 0x1a, previous_y - camera_y);
            uint32 first_angle = G3Function8002F0C4((uint32)((sint32)(y << 16) >> 16), (uint32)((sint32)(x << 16) >> 16), frame);
            sint32 old_y = r_s16(frame + 0x1a);
            sint32 old_x = r_s16(frame + 0x18);
            uint32 second_angle = G3Function8002F0C4((uint32)old_y, (uint32)old_x, frame);
            current_x = r_u16(object + 0x58);
            previous_x = r_u16(object + 0x70);
            x = current_x - previous_x;
            sint32 signed_x = (sint32)(x << 16) >> 16;
            uint32 square_x = (uint32)signed_x * (uint32)signed_x;
            w_u16(frame + 0x20, x);
            current_y = r_u16(object + 0x5a);
            previous_y = r_u16(object + 0x72);
            y = current_y - previous_y;
            sint32 signed_y = (sint32)(y << 16) >> 16;
            uint32 square_y = (uint32)signed_y * (uint32)signed_y;
            uint32 difference = (uint32)((sint32)(second_angle << 16) >> 16) - (uint32)((sint32)(first_angle << 16) >> 16);
            w_u16(frame + 0x22, y);
            uint32 length = G3Function8002F368((square_x + square_y) << 12, frame);
            uint32 product = length * difference;
            owner = r_u32(effect + 0x14);
            game = r_u32(0x80097048);
            camera = r_u32(game + 0x4c);
            uint32 rotation = (uint32)((sint32)product >> 12);
            rotation = 0 - rotation;
            rotation = (uint32)((sint32)(rotation << 15) >> 16);
            return G3Function80041CE4(owner + 0x0c, camera, effect + 0x1c, rotation, frame);
        }
    }
    else if ((flags & 0x80) != 0)
    {
        uint32 x = (uint32)r_s8(effect + 0x49) << 2;
        w_u16(frame + 0x10, x);
        uint32 y = (uint32)r_s8(effect + 0x4a) << 2;
        w_u16(frame + 0x12, y);
        uint32 z = (uint32)r_s8(effect + 0x4b) << 2;
        w_u16(frame + 0x14, z);
        owner = r_u32(effect + 0x14);
        G3Function8005B94C(x, owner + 0x0c, frame);
        owner = r_u32(effect + 0x14);
        y = (uint32)r_s16(frame + 0x12);
        G3Function8005BA5C(y, owner + 0x0c, frame);
        owner = r_u32(effect + 0x14);
        z = (uint32)r_s16(frame + 0x14);
        return G3Function8005BB6C(z, owner + 0x0c, frame);
    }
    uint32 game = r_u32(0x80097048);
    owner = r_u32(effect + 0x14);
    uint32 camera = r_u32(game + 0x4c);
    return G3Function80041CE4(owner + 0x0c, camera, effect + 0x1c, 0, frame);
}

uint32 G3Function800392A0(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800392a0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 resource = G3Function8003AA48(0x8009db20);
    resource = r_u32(resource + 0x0c);
    uint32 x = r_u16(object + 0x58);
    resource = r_u32(resource);
    w_u16(frame + 0x28, x);
    w_u16(frame + 0x2a, r_u16(object + 0x5a));
    uint32 z = r_u16(object + 0x5c);
    w_u16(frame + 0x1c, 70);
    w_u16(frame + 0x24, 0xfffffff8);
    uint32 game = r_u32(0x80097048);
    w_u16(frame + 0x18, 0);
    w_u16(frame + 0x1a, 0);
    w_u16(frame + 0x20, 0);
    w_u16(frame + 0x22, 0);
    w_u16(frame + 0x2c, z + 0x100);
    uint32 result = 0;
    bool recognized = false;
    for (uint32 kind = 0; kind < 3; ++kind)
    {
        if (kind != 0)
            game = r_u32(0x80097048);
        result = r_u32(game + 0x48);
        uint32 candidate = r_u32(result + 0xd8 + kind * 4);
        if (candidate == 0)
            continue;
        result = r_u32(object + 0x1c);
        uint32 name = r_u32(result + 0x24);
        result = r_u32(candidate);
        uint32 name_word = r_u32(name);
        if (name_word != result)
            continue;
        name_word = r_u32(name + 4);
        result = r_u32(candidate + 4);
        if (name_word == result)
        {
            recognized = true;
            break;
        }
    }
    if (!recognized)
        return result;
    uint32 pool = r_u32(0x800a27fc);
    uint32 effect = G3Function800444E4(frame + 0x28, resource, pool, 120, frame);
    uint32 linked = 0;
    if (effect != 0)
    {
        linked = r_u32(effect + 4);
        uint32 height = r_u16(frame + 0x2c);
        w_u32(effect + 0x10, 0x80039158);
        w_u16(effect + 0x44, height);
        w_u16(effect + 0x4c, r_u16(frame + 0x18));
        w_u16(effect + 0x4e, r_u16(frame + 0x1a));
        w_u16(effect + 0x50, r_u16(frame + 0x1c));
        w_u16(effect + 0x52, r_u16(frame + 0x20));
        w_u16(effect + 0x54, r_u16(frame + 0x22));
        uint32 flags = r_u32(effect + 0x0c);
        uint32 acceleration = r_u16(frame + 0x24);
        w_u32(effect + 0x0c, (flags & 0xfffffffb) | 0x800);
        w_u16(effect + 0x56, acceleration);
        if (linked != 0)
        {
            height = r_u16(frame + 0x2c);
            w_u32(linked + 0x10, 0x80039158);
            w_u16(linked + 0x44, height);
            w_u16(linked + 0x4c, r_u16(frame + 0x18));
            w_u16(linked + 0x4e, r_u16(frame + 0x1a));
            w_u16(linked + 0x50, r_u16(frame + 0x1c));
            w_u16(linked + 0x52, r_u16(frame + 0x20));
            flags = r_u32(linked + 0x0c);
            acceleration = r_u16(frame + 0x22);
            w_u16(linked + 0x54, acceleration);
            acceleration = r_u16(frame + 0x24);
            w_u32(linked + 0x0c, (flags & 0xfffffffb) | 0x800);
            w_u16(linked + 0x56, acceleration);
        }
    }
    game = r_u32(0x80097048);
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 level = r_u32(game + 0x48);
    uint32 name = r_u32(descriptor + 0x24);
    uint32 candidate = r_u32(level + 0xd8);
    uint32 name_word = r_u32(name);
    result = r_u32(candidate);
    bool matched = false;
    uint32 category = 0;
    if (name_word == result)
    {
        name_word = r_u32(name + 4);
        result = r_u32(candidate + 4);
        matched = name_word == result;
    }
    if (!matched)
    {
        game = r_u32(0x80097048);
        result = r_u32(game + 0x48);
        candidate = r_u32(result + 0xdc);
        if (candidate != 0)
        {
            descriptor = r_u32(object + 0x1c);
            name = r_u32(descriptor + 0x24);
            result = r_u32(candidate);
            name_word = r_u32(name);
            if (name_word == result)
            {
                name_word = r_u32(name + 4);
                result = r_u32(candidate + 4);
                matched = name_word == result;
            }
            if (!matched)
                result = r_u32(game + 0x48);
        }
        category = 1;
        if (!matched)
        {
            candidate = r_u32(result + 0xe0);
            if (candidate != 0)
            {
                descriptor = r_u32(object + 0x1c);
                name = r_u32(descriptor + 0x24);
                result = r_u32(candidate);
                name_word = r_u32(name);
                if (name_word == result)
                {
                    name_word = r_u32(name + 4);
                    result = r_u32(candidate + 4);
                    matched = name_word == result;
                }
            }
            category = 2;
        }
    }
    if (matched)
    {
        uint32 counter_offset = 0x68e4 + category * 4;
        uint32 count = r_u32(game + counter_offset) + 1;
        w_u32(game + counter_offset, count);
        if (effect != 0)
            w_u16(effect + 0x74, r_u16(game + counter_offset));
        if (linked != 0)
        {
            uint32 current_game = r_u32(0x80097048);
            w_u16(linked + 0x74, r_u16(current_game + counter_offset));
        }
        game = r_u32(0x80097048);
        level = r_u32(game + 0x48);
        count = r_u32(game + counter_offset);
        result = r_u32(level + 0xcc + category * 4);
        if (count == result)
        {
            uint32 sequence = r_u32(level + 0xb4 + category * 4);
            uint32 player = r_u32(game + 0x50);
            result = G3Function80018DE0(player, sequence + 4, frame);
        }
    }
    if (effect != 0)
    {
        game = r_u32(0x80097048);
        uint32 count = (uint32)r_s16(effect + 0x74);
        w_u32(frame + 0x10, 1500);
        uint32 pitch = (count * 25) << 17;
        pitch = (uint32)((sint32)pitch >> 16);
        uint32 player = r_u32(game + 0x50);
        G3Function8003E518(player, 0x802, pitch, 100, frame);
        pool = r_u32(0x800a27fc);
        result = G3Function80041880(effect, pool, frame);
    }
    if (linked != 0)
    {
        pool = r_u32(0x800a27fc);
        result = G3Function80041880(linked, pool, frame);
    }
    return result;
}

uint32 G3Function80039158(uint32 effect, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80039158, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    sint32 lifetime = r_s16(effect + 0x22);
    uint32 unsigned_lifetime = r_u16(effect + 0x22);
    if (lifetime > 0)
    {
        w_u16(effect + 0x22, unsigned_lifetime - 1);
        lifetime = r_s16(effect + 0x22);
    }
    if (lifetime == 0)
    {
        uint32 effect_pool = r_u32(0x800a27fc);
        uint32 game = r_u32(0x80097048);
        uint32 x = (uint32)r_s16(effect + 0x1c);
        uint32 z = (uint32)r_s16(effect + 0x20);
        uint32 y = (uint32)r_s16(effect + 0x1e);
        w_u32(frame + 0x10, z + 0x40);
        uint32 player = r_u32(game + 0x50);
        G3Function800440D0(player, effect_pool, x, y, frame);
        return G3Function80041578(effect, pool, frame);
    }
    uint32 velocity_x = r_u16(effect + 0x4c);
    uint32 acceleration_x = r_u16(effect + 0x52);
    uint32 velocity_y = r_u16(effect + 0x4e);
    uint32 acceleration_y = r_u16(effect + 0x54);
    w_u16(effect + 0x4c, velocity_x + acceleration_x);
    uint32 velocity_z = r_u16(effect + 0x50);
    uint32 acceleration_z = r_u16(effect + 0x56);
    w_u16(effect + 0x4e, velocity_y + acceleration_y);
    uint32 x = r_u16(effect + 0x1c);
    velocity_x = r_u16(effect + 0x4c);
    uint32 next_velocity_z = velocity_z + acceleration_z;
    w_u16(effect + 0x1c, x + velocity_x);
    uint32 y = r_u16(effect + 0x1e);
    velocity_y = r_u16(effect + 0x4e);
    w_u16(effect + 0x50, next_velocity_z);
    uint32 z = r_u16(effect + 0x20);
    velocity_z = r_u16(effect + 0x50);
    uint32 next_z = z + velocity_z;
    w_u16(effect + 0x20, next_z);
    w_u16(effect + 0x1e, y + velocity_y);
    sint32 floor = r_s16(effect + 0x44);
    uint32 result = ((sint32)(next_z << 16) >> 16) < floor ? 1 : 0;
    uint32 floor_value = r_u16(effect + 0x44);
    if (result != 0)
    {
        uint32 bounce = r_u16(effect + 0x46);
        w_u16(effect + 0x20, floor_value);
        ++bounce;
        w_u16(effect + 0x46, bounce);
        sint32 signed_bounce = (sint32)(bounce << 16) >> 16;
        result = signed_bounce == 1 ? 46 : 32;
        if (signed_bounce == 1 || signed_bounce == 2)
            w_u16(effect + 0x50, result);
    }
    return result;
}

uint32 G3Function80018DE0(uint32 object, uint32 sequence, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80018de0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    return G3Function80018D1C(object, sequence, 0, frame);
}

uint32 G3Function80024254(uint32 game, uint32 incoming_v0, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80024254, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function8003A114(0x8009d6b4, incoming_v0);
    w_u16(0x8009d6b8, 2100);
    w_u16(0x8009d6bc, 4000);
    w_u16(0x8009d6be, 70);
    w_u16(0x8009d6ba, 32000);
    w_u32(0x8009d6b4, 0);
    w_u16(0x8009d6c0, 1);
    G3Function800810FC();
    G3Function80081154();
    G3Function8003EF64(frame);
    w_u16(0x800ad0c0, 0xffffffff);
    w_u16(0x800ad0c4, 0xffffffff);
    uint32 level = r_u32(game + 0x48);
    uint32 player = r_u32(0x8009efc4);
    G3Function80075218(player, level + 0x28, game, frame);
    uint32 camera = r_u32(game + 0x4c);
    G3Function80013D38(camera, frame);
    camera = r_u32(game + 0x4c);
    level = r_u32(game + 0x48);
    sint32 distance = r_s32(camera + 0x88);
    uint32 shift = r_u32(level + 0x104);
    uint32 scaled_distance = (uint32)(distance >> 2) << (shift & 31);
    G3Function8004D8B0(camera, scaled_distance);
    G3Function8002ACC8(frame);
    G3Function8002AC44(game);
    level = r_u32(game + 0x48);
    w_u32(game + 0x68e4, 0);
    w_u32(game + 0x68e8, 0);
    w_u32(game + 0x68ec, 0);
    uint32 root = r_u32(level);
    sint32 count = r_s32(level + 0x7c);
    uint32 record = r_u32(root + 0x14);
    for (sint32 index = 0; index < count; ++index, record += 0x34)
    {
        if (r_u32(record) == 0)
            continue;
        uint32 original_flags = r_u32(record + 0x18);
        w_u32(record + 0x18, original_flags & 0xfffffe7f);
        for (uint32 kind = 0; kind < 3; ++kind)
        {
            uint32 descriptor = r_u32(record);
            level = r_u32(game + 0x48);
            uint32 name = r_u32(descriptor + 0x24);
            uint32 candidate = r_u32(level + 0xd8 + kind * 4);
            uint32 name_word = r_u32(name);
            uint32 candidate_word = r_u32(candidate);
            if (name_word != candidate_word)
                continue;
            name_word = r_u32(name + 4);
            candidate_word = r_u32(candidate + 4);
            if (name_word != candidate_word)
                continue;
            uint32 flags = kind == 0 ? original_flags : r_u32(record + 0x18);
            if ((flags & 0x10) == 0)
                continue;
            uint32 counter_address = game + 0x68e4 + kind * 4;
            uint32 total = r_u32(counter_address);
            w_u32(counter_address, total + 1);
            break;
        }
    }
    level = r_u32(game + 0x48);
    G3Function80039B18(level, frame);
    level = r_u32(game + 0x48);
    root = r_u32(level);
    uint32 unsigned_count = r_u32(level + 0x7c);
    record = r_u32(root + 0x14);
    G3Function8002ABB8(record, unsigned_count, frame);
    uint32 flags = r_u32(game + 0x6834);
    player = r_u32(0x8009efc4);
    w_u32(game + 0x6834, flags & 0xfffff9ff);
    w_u32(player + 0x14, r_u32(player + 0x14) & 0xfdffffff);
    w_u32(game + 0x67c4, 80);
    w_u32(game + 0x67c8, 80);
    flags = r_u32(game + 0x6834);
    level = r_u32(game + 0x48);
    w_u32(game + 0x6790, 0);
    w_u32(game + 0x6834, flags & 0xffffefff);
    uint32 sequences = r_u32(level + 0xa8);
    G3Function80018E00(sequences, flags & 0xffffefff);
    G3Function800150A8(frame);
    level = r_u32(game + 0x48);
    uint32 sequence = r_u32(level + 0x74);
    if (sequence != 0)
    {
        player = r_u32(0x8009efc4);
        G3Function80018DE0(player, sequence + 4, frame);
    }
    uint32 mode = r_u16(game + 0x68b4);
    uint32 saved = r_u16(game + 0x681e);
    w_u16(game + 0x681c, saved);
    if (mode - 20 >= 3)
    {
        G3Function80047EE4(1, r_u32(game + 0x6820));
        G3Function80047EE4(2, r_u32(game + 0x6828));
        G3Function80047EE4(4, r_u32(game + 0x6824));
        G3Function80047EE4(3, r_u32(game + 0x682c));
    }
    level = r_u32(game + 0x48);
    shift = r_u32(level + 0x104) & 31;
    G3Function8001946C(0x555 >> shift, 0x400 >> shift);
    player = r_u32(0x8009efc4);
    uint32 result = r_u32(player + 0x128) & 0x2000;
    if (result == 0)
        result = G3Function8006FF1C(player, game, game + 0x68c0, frame);
    return G3Function8004DF4C(1, result);
}

uint32 G3Function80023AC8(uint32 drm, uint32 vrm, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80023ac8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    G3SetRandomState80027F9C(0);
    G3Function8003A114(0x8009d6b4, 0);
    w_u16(0x8009d6b8, 2100);
    w_u16(0x8009d6bc, 4000);
    w_u16(0x8009d6be, 70);
    w_u16(0x8009d6ba, 32000);
    w_u32(0x8009d6b4, 0);
    w_u16(0x8009d6c0, 1);
    G3Function800810FC();
    G3Function80081154();
    G3MemPackPush8004829C();
    G3Function80065674(vrm, frame);
    G3MemPackPop80048320();
    G3Function80046AD0();
    uint32 level = G3LoadObject8002C760(drm, frame);
    G3Function8002C704(level, game + 0x6730, frame);
    while (r_u32(game + 0x68f0) == 0)
        G3WaitArchiveRead8002C830(0xffffffff, frame);
    w_u32(game + 0x48, level);
    uint32 shift = r_u32(level + 0x104) & 31;
    G3Function8001946C(0x555 >> shift, 0x400 >> shift);
    if (r_s16(game + 0x6780) == 2)
    {
        uint32 destination = r_u32(level + 0x90);
        uint32 words[2];
        for (uint32 i = 0; i < 2; ++i)
        {
            uint32 source = 0x8009d6c4 + i * 4;
            r_masked_u32(source, 15);
            words[i] = r_masked_u32(source, 15);
        }
        uint32 terminator = (uint32)r_s8(0x8009d6cc);
        for (uint32 i = 0; i < 2; ++i)
        {
            uint32 target = destination + i * 4;
            uint32 high = target + 3;
            uint32 lane = high & 3;
            w_masked_u32(high & 0xfffffffc, words[i] >> ((3 - lane) * 8), (1 << (lane + 1)) - 1);
            lane = target & 3;
            w_masked_u32(target & 0xfffffffc, words[i] << (lane * 8), (15 << lane) & 15);
        }
        w_u8(destination + 8, terminator);
    }
    uint32 current_level = r_u32(game + 0x48);
    w_u32(current_level + 0x30, 50);
    current_level = r_u32(game + 0x48);
    w_u32(current_level + 0x34, 10);
    current_level = r_u32(game + 0x48);
    w_u32(current_level + 0x38, 3);
    G3Function80030414();
    G3Function80023850(game, level);
    uint32 camera = r_u32(0x8009d328);
    uint32 state = r_u32(0x800a0c2c);
    uint32 player = r_u32(0x8009efc4);
    w_u32(game + 0x148, 0x800a3cac);
    uint32 records = r_u32(0x800a0c28);
    w_u32(game + 0x4c, camera);
    w_u32(game + 0x5c, state);
    uint32 auxiliary = r_u32(0x800a0c7c);
    uint32 pool = r_u32(0x800a0c20);
    uint32 arena = r_u32(0x800a0c78);
    w_u32(game + 0x60, records);
    w_u32(game + 0x68d4, 10000);
    w_u32(game + 0x68d8, 4000);
    w_u32(game + 0x50, player);
    w_u32(game + 0x6590, 0);
    w_u32(game + 0x6834, 0);
    w_u32(game + 0x13c, 0);
    w_u32(game + 0x140, 0);
    w_u32(game + 0x154, 0);
    w_u32(game + 0x150, 0);
    w_u32(game + 0x14c, 0);
    w_u32(game + 0x6788, 0);
    w_u32(game + 0x6794, 0);
    w_u32(game + 0x68d0, 0);
    w_u32(game + 0x68bc, 0);
    w_u8(game + 0xee, 0);
    w_u8(game + 0xed, 0);
    w_u8(game + 0xec, 0);
    w_u16(game + 0x69b0, 10);
    w_u16(game + 0x69dc, 0);
    w_u32(game + 0x64, pool);
    w_u32(game + 0x138, arena);
    w_u32(game + 0x68, auxiliary);
    w_u16(game + 0x69de, 0);
    w_u16(game + 0x69bc, 40);
    w_u32(game + 0x69c0, 0x800);
    w_u8(game + 0x69c4, 0x60);
    w_u8(game + 0x69c5, 0x60);
    w_u8(game + 0x69c6, 0xa0);
    w_u32(game + 0x65ec, 0x8005fc70);
    uint32 mode = r_u16(game + 0x68b4);
    w_u32(game + 0x65e8, 0x8005f518);
    w_u32(game + 0x69f4, 0);
    w_u32(game + 0x69f0, 0);
    w_u32(game + 0x69d8, 0);
    w_u32(game + 0x68e4, 0);
    w_u32(game + 0x68e8, 0);
    w_u32(game + 0x68ec, 0);
    if (mode != 27 && mode != 20)
    {
        w_u32(game + 0x67c4, 100);
        w_u32(game + 0x67c8, 100);
    }
    for (uint32 i = 0; i < 8; ++i)
        w_u32(game + 0x67b4 - i * 4, 0);
    if ((r_u32(game + 0xe8) & 0x2000) != 0)
        w_u16(game + 0x68b4, 2);
    uint32 storage = r_u32(0x800a2a68);
    G3Function800414B0(storage);
    G3Function800752D8();
    state = r_u32(0x800a0c2c);
    records = r_u32(0x800a0c28);
    G3Function80029010(state, records);
    current_level = r_u32(game + 0x48);
    if (r_u32(current_level + 8) != 2)
    {
        G3Function80020CBC(0x8009d6d0, 0, 0, 0, frame, 0);
        {
#ifndef NDEBUG
        printf("%s", (const char *)psx_addr(0x8009d6d0, 50));
#endif
        }
    }
    G3Function80039A3C(level);
    G3Function8003ACC8(level, frame);
    G3Function80039AA4(level, frame);
    current_level = r_u32(game + 0x48);
    uint32 root = r_u32(current_level);
    uint32 count = r_u32(current_level + 0x7c);
    uint32 list = r_u32(root + 0x14);
    G3Function8002AB10(list, count, frame);
    uint32 descriptor = r_u32(0x80098744);
    uint32 object_descriptor = r_u32(0x8009879c);
    w_u32(0x8009d6a4, descriptor);
    w_u32(game + 0x68b8, object_descriptor);
    G3Function800356E4();
    G3Function80035E80(game);
    G3Function80035AC8();
    player = r_u32(0x8009efc4);
    descriptor = r_u32(player + 0x1c);
    uint32 private_state = r_u32(descriptor + 0x1c);
    w_u32(player + 0x24, private_state);
    G3Function800751B4(frame);
    player = r_u32(0x8009efc4);
    uint32 player_flags = r_u32(player + 0x10);
    uint32 private_flags = r_u32(player + 0x128);
    w_u32(player + 0x10, player_flags | 0x800);
    if ((private_flags & 0x2000) == 0)
    {
        uint32 first = r_u32(0x8009d704);
        uint32 second = r_u32(0x8009d708);
        uint32 terminator = (uint32)r_s8(0x8009d70c);
        w_u32(game + 0x68c0, first);
        w_u32(game + 0x68c4, second);
        w_u8(game + 0x68c8, terminator);
    }
    current_level = r_u32(game + 0x48);
    if ((r_u32(current_level + 0x70) & 2) != 0)
    {
        player = r_u32(0x8009efc4);
        w_u32(player + 0x150, 7);
    }
    else
    {
        state = r_u32(0x800a0c2c);
        player = r_u32(0x8009efc4);
        G3Function80029294(state, player);
    }
    camera = r_u32(0x8009d32c);
    G3Function8004D8B8(camera, level + 0x28, frame);
    camera = r_u32(game + 0x4c);
    sint32 distance = r_s32(camera + 0x88);
    shift = r_u32(level + 0x104) & 31;
    G3Function8004D8B0(camera, (uint32)(distance >> 2) << shift);
    uint32 current_game = r_u32(0x80097048);
    uint32 level_data = r_u32(level + 0x54);
    w_u32(current_game + 0x10, level_data);
    G3Function8002C50C(level);
    uint32 red = r_u8(level + 0x40);
    uint32 green = r_u8(level + 0x41);
    uint32 blue = r_u8(level + 0x42);
    SetFarColor((sint32)red, (sint32)green, (sint32)blue);
    w_u8(0x800a3d98, r_u8(level + 0x44));
    w_u8(0x800a3d99, r_u8(level + 0x45));
    blue = r_u8(level + 0x46);
    w_u8(0x800a0c3b, 5);
    w_u8(0x800a0c3f, 40);
    w_u8(0x800a3d9a, blue);
    w_u8(0x800a0c3c, r_u8(level + 0x44));
    w_u8(0x800a0c3d, r_u8(level + 0x45));
    blue = r_u8(level + 0x46);
    w_u16(0x800a0c76, 240);
    w_u16(0x800a0c5e, 240);
    w_u16(0x800a0c62, 240);
    w_u16(0x800a0c70, 512);
    w_u16(0x800a0c74, 512);
    w_u16(0x800a0c5c, 512);
    w_u16(0x800a0c6c, 0);
    w_u16(0x800a0c6e, 0);
    w_u16(0x800a0c72, 0);
    w_u16(0x800a0c58, 0);
    w_u16(0x800a0c5a, 0);
    w_u16(0x800a0c60, 0);
    w_u8(0x800a0c67, 4);
    w_u8(0x800a0c6b, 32);
    w_u8(0x800a0c53, 4);
    w_u8(0x800a0c57, 32);
    w_u8(0x800a0c68, 255);
    w_u8(0x800a0c69, 255);
    w_u8(0x800a0c6a, 255);
    w_u8(0x800a0c3e, blue);
    w_u8(0x800a0c54, 255);
    arena = r_u32(0x800a0c78);
    w_u8(0x800a0c55, 255);
    w_u8(0x800a0c56, 255);
    G3Function8002BCA8(arena, frame);
    G3Function8002B428(level, frame);
    level_data = r_u32(level + 0x60);
    w_u32(game + 0x6790, 0);
    w_u32(game + 0x678c, 0);
    w_u32(0x8009db6c, level_data + 8);
    G3Function800150A8(frame);
    G3Function800151AC(frame);
    current_game = r_u32(0x80097048);
    w_u16(game + 0xf4, 0);
    w_u16(game + 0xf0, 30);
    w_u16(game + 0xf2, 30);
    mode = r_u16(current_game + 0x68b4);
    if (mode - 11 < 5)
        w_u32(0x800a085c, r_u32(game + 0x69f8));
    mode = r_u16(game + 0x68b4);
    if (mode - 11 < 5 || mode - 20 < 3 || mode >= 23)
        w_u32(game + 0x68f0, 0xffffffff);
    mode = r_u16(game + 0x68b4);
    if (mode == 27)
    {
        G3Function800657C0();
        G3Function80081010();
    }
    sint32 start = r_s32(game + 0x68f0);
    uint32 sequence = 0;
    if (start > 0)
        sequence = r_u32(level + (((uint32)start - 1) << 2) + 0xc0);
    if (sequence == 0)
        sequence = r_u32(level + 0x74);
    if (sequence != 0)
    {
        player = r_u32(game + 0x50);
        G3Function80018DE0(player, sequence + 4, frame);
    }
    G3Function80028F78();
    uint32 resource_name = r_u32(level + 0x94);
    uint32 resource = G3Function8003AA48(resource_name);
    uint32 cold = G3Function8003AA48(0x8009d710);
    if (resource != 0 && cold != 0)
        w_u32(cold + 0x0c, r_u32(resource + 0x0c));
    current_level = r_u32(game + 0x48);
    if ((r_u32(current_level + 0x70) & 4) != 0)
    {
        for (uint32 i = 1; i < 12; ++i)
            G3Function80046B1C(i);
        if (r_u16(game + 0x68b4) == 255)
            G3Function80046B1C(0);
    }
    if (G3Function80023A68(game) != 0)
        G3Function80023AAC(game, frame);
    if (r_u16(game + 0x68b4) != 27)
        G3Function8004DF4C(1, 27);
    G3SetRandomState80027F9C(0);
    uint32 pending = r_u32(0x8009d838);
    if (pending != 0 || r_u8(game + 0x68f8) != 0)
        srandPSX(0);
    uint32 flags = r_u32(game + 0x69b8);
    current_game = r_u32(0x80097048);
    w_u32(game + 0x69b8, flags & 0xfffeffff);
    mode = r_u16(current_game + 0x68b4);
    uint32 result = mode - 11 < 5 ? 1 : 0;
    if (result != 0)
        result = G3Function800359EC(0x122, 1, frame);
    return result;
}
