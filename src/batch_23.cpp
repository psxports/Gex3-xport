#include "xport.h"
#include "psx.h"
#include "game_functions.h"

void G3Function80075418(uint32 game, uint32 order_table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80075418, "SLUS_008.06");
    uint32 pool = r_u32(0x800a02e4);
    uint32 frame = guest_stack - 0x38;
    uint32 previous_link = pool + 4;
    uint32 effect = r_u32(pool + 4);
    uint32 camera = r_u32(game + 0x4c);
    uint32 context = r_u32(game + 0x68);
    uint32 view = r_u32(camera + 0x78);
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
    first = r_u32(view + 0x14);
    second = r_u32(view + 0x18);
    xport_gte_write_control(5, first);
    third = r_u32(view + 0x1c);
    xport_gte_write_control(6, second);
    xport_gte_write_control(7, third);
    while (effect != 0)
    {
        sint32 lifetime = r_s16(effect + 0x12);
        uint32 remaining = r_u16(effect + 0x12) - 1;
        if (lifetime == 0)
        {
            uint32 retired = effect;
            effect = r_u32(effect);
            pool = r_u32(0x800a02e4);
            w_u32(previous_link, effect);
            uint32 free_head = r_u32(pool);
            w_u32(retired, free_head);
            w_u32(pool, retired);
            continue;
        }
        w_u16(effect + 0x12, remaining);
        uint32 limit = r_u32(context + 8);
        uint32 cursor = r_u32(context + 4);
        if (cursor < limit - 0x40)
        {
            sint32 line_flag = r_s16(effect + 0xa);
            bool line = line_flag != 0 && (sint16)remaining >= 2;
            if (line)
            {
                xport_gte_write_data(0, r_u32(effect + 4));
                xport_gte_write_data(1, r_u32(effect + 8));
                xport_gte_write_data(2, r_u32(effect + 0xc));
                xport_gte_write_data(3, r_u32(effect + 0x10));
                xport_gte_write_data(4, r_u32(effect + 0xc));
                xport_gte_write_data(5, r_u32(effect + 0x10));
                xport_gte_execute(0x280030);
            }
            else
            {
                xport_gte_write_data(0, r_u32(effect + 4));
                xport_gte_write_data(1, r_u32(effect + 8));
                xport_gte_execute(0x180001);
            }
            w_u32(frame + 0x10, xport_gte_read_data(19));
            sint32 depth = r_s32(frame + 0x10);
            sint32 maximum = r_s32(game + 0x28);
            depth >>= 2;
            w_u32(frame + 0x10, (uint32)depth);
            if (depth < maximum && depth >= 0)
            {
                uint32 packet = r_u32(context + 4);
                uint32 color = r_u32(effect + 0x14);
                w_u32(packet + 4, color);
                if (line)
                {
                    setLineF2((LINE_F2 *)psx_addr(packet, 16));
                    w_u32(packet + 8, xport_gte_read_data(12));
                    w_u32(packet + 0xc, xport_gte_read_data(13));
                    w_u32(packet + 0xc, xport_gte_read_data(14));
                }
                else
                {
                    SetTile((TILE *)psx_addr(packet, 16));
                    w_u32(packet + 8, xport_gte_read_data(14));
                    depth = r_s32(frame + 0x10);
                    maximum = r_s32(game + 0x28);
                    if (depth < maximum && depth >= 0)
                    {
                        sint32 remainder = depth % 2;
                        w_u16(packet + 0xc, (uint32)(remainder + 2));
                        w_u16(packet + 0xe, (uint32)(remainder + 1));
                    }
                    else
                    {
                        w_u16(packet + 0xc, 2);
                        w_u16(packet + 0xe, 1);
                    }
                }
                uint32 link_depth = r_u32(frame + 0x10);
                uint32 entry = order_table + (link_depth << 2);
                uint32 prior = r_u32(entry);
                w_u32(packet, (prior & 0xffffff) | 0x03000000);
                w_u32(entry, packet & 0xffffff);
                uint32 count = r_u32(context);
                w_u32(context + 4, packet + 0x10);
                w_u32(context, count + 1);
            }
        }
        previous_link = effect;
        effect = r_u32(effect);
    }
}

uint32 G3Function80076CF8(uint32 angle)
{
    FUNCTION_MARKER(0x80076cf8, "SLUS_008.06");
    uint32 controlled = r_u32(0x8009efc4);
    uint32 state = r_u32(controlled + 0x24);
    uint32 result = (uint32)r_s16(state + 0xbc);
    if (result != 0)
        return result;
    uint32 previous = r_u16(state + 0xcc);
    uint32 base = r_u16(state + 0xca);
    result = angle - previous;
    uint32 adjusted = base + result;
    w_u16(state + 0xc6, adjusted);
    w_u16(controlled + 0x64, adjusted);
    return result;
}

uint32 G3Function80076D3C(uint32 a0, uint32 game, uint32 payload, uint32 a3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80076d3c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x68;
    uint32 result = r_u32(payload + 0x10) & 0x800000;
    if (result == 0)
        return result;
    uint32 controlled = r_u32(game + 0x50);
    uint32 state = r_u32(controlled + 0x24);
    uint32 source = controlled + 0x58;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
    uint32 low_lane = source & 3;
    uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
    uint32 xy = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    uint32 z = (uint32)r_s16(controlled + 0x5c);
    uint32 destination = frame + 0x38;
    high_address = destination + 3;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, xy >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    low_lane = destination & 3;
    w_masked_u32(destination & 0xfffffffc, xy << (low_lane * 8), (15 << low_lane) & 15);
    w_u16(frame + 0x3c, z);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 current = r_u16(controlled + 0x58 + axis * 2);
        uint32 previous = r_u16(controlled + 0x70 + axis * 2);
        w_u16(frame + 0x48 + axis * 2, current - previous);
    }
    uint32 definition = r_u32(payload + 0x1c);
    uint32 matrix;
    uint32 translation;
    uint32 translation_stride;
    if (r_u32(definition + 0x10) == 0)
    {
        G3Function8005BD00(payload + 0x60, frame + 0x10);
        ApplyMatrixSV((MATRIX *)psx_addr(frame + 0x10, 20), (SVECTOR *)psx_addr(state + 0x128, 8), (SVECTOR *)psx_addr(controlled + 0x58, 6));
        uint32 angle = (uint32)r_s16(payload + 0x64);
        G3Function80076CF8(angle);
        translation = payload + 0x58;
        translation_stride = 2;
    }
    else
    {
        uint32 index = (uint32)r_s16(state + 0x25c);
        matrix = r_u32(payload + 0x30);
        matrix += index << 5;
        ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(state + 0x128, 8), (SVECTOR *)psx_addr(controlled + 0x58, 6));
        G3Function800202A4(matrix, frame + 0x30, 0x15, frame);
        uint32 angle = (uint32)r_s16(frame + 0x34);
        G3Function80076CF8(angle);
        translation = matrix + 0x14;
        translation_stride = 4;
    }
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 offset = r_u16(translation + axis * translation_stride);
        uint32 difference = r_u16(frame + 0x48 + axis * 2);
        uint32 position = r_u16(controlled + 0x58 + axis * 2);
        w_u16(controlled + 0x58 + axis * 2, position + offset + difference);
    }
    result = r_u32(state + 0x1fc);
    if (result == 0)
        return result;
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 position = r_u16(controlled + 0x58 + axis * 2);
        uint32 original = r_u16(frame + 0x38 + axis * 2);
        w_u16(frame + 0x40 + axis * 2, position - original);
    }
    uint32 linked = r_u32(state + 0x1fc);
    result = r_u32(linked + 0xec);
    if (result != 0)
        return G3DispatchStateEvent(result, linked, game, 0x7e2, frame + 0x40, frame);
    return result;
}

uint32 G3Function80027FDC(void)
{
    FUNCTION_MARKER(0x80027fdc, "SLUS_008.06");
    uint32 state = r_u32(0x8009d828);
    state = state * 0x41c64e6d + 0x3039;
    w_u32(0x8009d828, state);
    return (uint32)((sint32)state >> 16) & 0x7fff;
}

uint32 G3Function8004BEE8(uint32 sound)
{
    FUNCTION_MARKER(0x8004bee8, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 count = 0;
    sound &= 0xffff;
    for (uint32 index = 0; index < 24; ++index)
    {
        uint32 voice = audio + 0x248 + index * 28;
        if (r_u8(voice + 8) != 0xd0)
            continue;
        if (r_u16(voice + 4) != sound)
            continue;
        uint32 state = r_u8(audio + 0x4e8 + index);
        if (state != 0 && state != 2)
            ++count;
    }
    return count & 0xffff;
}

uint32 G3Function8003F108(uint32 sound)
{
    FUNCTION_MARKER(0x8003f108, "SLUS_008.06");
    return G3Function8004BEE8(sound & 0xffff) & 0xffff;
}

uint32 G3Function8003DC08(uint32 object, uint32 request, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003dc08, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 flags = r_u32(object + 0x14);
    request += 2;
    if ((flags & 0x2000) != 0)
    {
        uint32 handle = r_u32(object + 0x44);
        if (G3Function8003F0A8(handle) == 0)
        {
            flags = r_u32(object + 0x14);
            w_u32(object + 0x14, flags & 0xffffdfff);
        }
        else
        {
            handle = r_u32(object + 0x44);
            if (G3Function8003F088(handle) != 0)
            {
                flags = r_u32(object + 0x14);
                if ((flags & 0x8000) == 0)
                    w_u32(object + 0x14, flags | 0x8000);
                else
                {
                    w_u32(object + 0x14, flags & 0xffff7fff);
                    uint32 radius = (uint32)r_s16(request + 2);
                    uint32 pitch = (uint32)r_s16(object + 0x4a);
                    uint32 volume = (uint32)r_s16(object + 0x48);
                    w_u32(frame + 0x10, radius);
                    handle = r_u32(object + 0x44);
                    if (G3Function8003E7E4(object, handle, pitch, volume, frame) == 0)
                    {
                        flags = r_u32(object + 0x14);
                        handle = r_u32(object + 0x44);
                        w_u32(object + 0x14, flags & 0xffffdfff);
                        G3Function8003F18C(handle);
                        w_u32(object + 0x44, 0);
                    }
                }
            }
        }
    }
    sint32 timer = r_s16(object + 0x4c);
    uint32 unsigned_timer = r_u16(object + 0x4c);
    uint32 result = unsigned_timer - 1;
    if (timer != 0)
    {
        w_u16(object + 0x4c, result);
        return result;
    }
    uint32 handle = r_u32(object + 0x44);
    result = G3Function8003F0A8(handle);
    if (result != 0)
        return result;
    uint32 threshold = r_u16(request + 0xc);
    if (threshold >= 0x2711)
    {
        uint32 sound = r_u16(request);
        result = G3Function8003F108(sound);
        threshold = r_u16(request + 0xc) - 0x2710;
        result = (sint32)result < (sint32)threshold ? 1 : 0;
        if (result == 0)
            return result;
    }
    flags = r_u32(object + 0x14);
    uint32 updated_flags = flags | 0x4000;
    if ((flags & 0x4000) != 0)
    {
        uint32 volume = r_u8(request + 4);
        w_u16(object + 0x48, volume);
        if (r_u8(request + 5) != 0)
        {
            result = G3Function80027FDC();
            uint32 divisor = r_u8(request + 5) << 1;
            uint32 remainder = divisor == 0 ? result : (uint32)((sint32)result % (sint32)divisor);
            uint32 amplitude = r_u8(request + 5);
            volume = r_u16(object + 0x48);
            w_u16(object + 0x48, volume + amplitude - remainder);
        }
        uint32 pitch = r_u16(request + 6);
        w_u16(object + 0x4a, pitch);
        if (r_u16(request + 8) != 0)
        {
            result = G3Function80027FDC();
            uint32 amplitude = r_u16(request + 8);
            uint32 divisor = amplitude << 1;
            uint32 remainder = divisor == 0 ? result : (uint32)((sint32)result % (sint32)divisor);
            pitch = r_u16(object + 0x4a);
            w_u16(object + 0x4a, pitch + amplitude - remainder);
        }
        uint32 sound = r_u16(request);
        pitch = (uint32)r_s16(object + 0x4a);
        volume = (uint32)r_s16(object + 0x48);
        uint32 radius = (uint32)r_s16(request + 2);
        w_u32(frame + 0x10, radius);
        result = G3Function8003E518(object, sound, pitch, volume, frame);
        w_u32(object + 0x44, result);
        if (result == 0)
            return result;
        updated_flags = r_u32(object + 0x14) | 0x2000;
    }
    w_u32(object + 0x14, updated_flags);
    result = r_u16(request + 0xc);
    uint32 delay = r_u16(request + 0xa);
    if (result != 0)
    {
        result = G3Function80027FDC();
        uint32 amplitude = r_u16(request + 0xc);
        uint32 divisor = amplitude << 1;
        uint32 remainder = divisor == 0 ? result : (uint32)((sint32)result % (sint32)divisor);
        delay += amplitude - remainder;
    }
    w_u16(object + 0x4c, delay);
    return result;
}

uint32 G3Function8003E0AC(uint32 object, uint32 request, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003e0ac, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 flags = r_u32(object + 0x14);
    request += 2;
    if ((flags & 0x2000) != 0)
    {
        uint32 handle = r_u32(object + 0x44);
        if (G3Function8003F0A8(handle) == 0)
        {
            flags = r_u32(object + 0x14);
            w_u32(object + 0x14, flags & 0xffffdfff);
        }
        else
        {
            handle = r_u32(object + 0x44);
            if (G3Function8003F088(handle) != 0)
            {
                flags = r_u32(object + 0x14);
                if ((flags & 0x8000) == 0)
                    w_u32(object + 0x14, flags | 0x8000);
                else
                {
                    w_u32(object + 0x14, flags & 0xffff7fff);
                    uint32 radius = (uint32)r_s16(request + 0x10);
                    uint32 pitch = (uint32)r_s16(object + 0x4a);
                    uint32 volume = (uint32)r_s16(object + 0x48);
                    w_u32(frame + 0x10, radius);
                    handle = r_u32(object + 0x44);
                    if (G3Function8003E7E4(object, handle, pitch, volume, frame) == 0)
                    {
                        flags = r_u32(object + 0x14);
                        handle = r_u32(object + 0x44);
                        w_u32(object + 0x14, flags & 0xffffdfff);
                        G3Function8003F18C(handle);
                        w_u32(object + 0x44, 0);
                    }
                }
            }
        }
    }
    sint32 timer = r_s16(object + 0x4c);
    uint32 unsigned_timer = r_u16(object + 0x4c);
    uint32 result = unsigned_timer - 1;
    if (timer != 0)
    {
        w_u16(object + 0x4c, result);
        return result;
    }
    uint32 handle = r_u32(object + 0x44);
    result = G3Function8003F0A8(handle);
    if (result != 0)
        return result;
    flags = r_u32(object + 0x14);
    uint32 updated_flags = flags | 0x4000;
    if ((flags & 0x4000) != 0)
    {
        uint32 volume = r_u8(request + 0x12);
        w_u16(object + 0x48, volume);
        if (r_u8(request + 0x13) != 0)
        {
            result = G3Function80027FDC();
            uint32 divisor = r_u8(request + 0x13) << 1;
            uint32 remainder = divisor == 0 ? result : (uint32)((sint32)result % (sint32)divisor);
            uint32 amplitude = r_u8(request + 0x13);
            volume = r_u16(object + 0x48);
            w_u16(object + 0x48, volume + amplitude - remainder);
        }
        uint32 pitch = r_u16(request + 0x14);
        w_u16(object + 0x4a, pitch);
        if (r_u16(request + 0x16) != 0)
        {
            result = G3Function80027FDC();
            uint32 amplitude = r_u16(request + 0x16);
            uint32 divisor = amplitude << 1;
            uint32 remainder = divisor == 0 ? result : (uint32)((sint32)result % (sint32)divisor);
            pitch = r_u16(object + 0x4a);
            w_u16(object + 0x4a, pitch + amplitude - remainder);
        }
        result = G3Function80027FDC();
        pitch = (uint32)r_s16(object + 0x4a);
        volume = (uint32)r_s16(object + 0x48);
        uint32 sound = r_u16(request + (result % 7) * 2);
        uint32 radius = (uint32)r_s16(request + 0x10);
        w_u32(frame + 0x10, radius);
        result = G3Function8003E518(object, sound, pitch, volume, frame);
        w_u32(object + 0x44, result);
        if (result == 0)
            return result;
        updated_flags = r_u32(object + 0x14) | 0x2000;
    }
    w_u32(object + 0x14, updated_flags);
    result = r_u16(request + 0x1a);
    uint32 delay = r_u16(request + 0x18);
    if (result != 0)
    {
        result = G3Function80027FDC();
        uint32 amplitude = r_u16(request + 0x1a);
        uint32 divisor = amplitude << 1;
        uint32 remainder = divisor == 0 ? result : (uint32)((sint32)result % (sint32)divisor);
        delay += amplitude - remainder;
    }
    w_u16(object + 0x4c, delay);
    return result;
}

uint32 G3Function80081194(uint32 origin, uint32 position, uint32 outer_radius, uint32 inner_radius, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80081194, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 destination = r_u16(position);
    uint32 source = r_u16(origin);
    uint32 difference = destination - source;
    sint32 narrowed = (sint16)difference;
    uint32 squares = (uint32)(narrowed * narrowed);
    uint32 amplitude = r_u32(guest_stack + 0x10);
    w_u16(frame + 0x10, difference);
    destination = r_u16(position + 2);
    source = r_u16(origin + 2);
    difference = destination - source;
    narrowed = (sint16)difference;
    squares += (uint32)(narrowed * narrowed);
    w_u16(frame + 0x12, difference);
    destination = r_u16(position + 4);
    source = r_u16(origin + 4);
    difference = destination - source;
    narrowed = (sint16)difference;
    squares += (uint32)(narrowed * narrowed);
    sint32 outer = (sint16)outer_radius;
    w_u16(frame + 0x14, difference);
    sint32 distance = (sint32)G3Function8002F448(squares, 0, frame);
    if (outer < distance)
        return 0;
    sint32 inner = (sint16)inner_radius;
    if (inner >= distance)
        return (uint32)(sint32)(sint16)amplitude;
    uint32 numerator = (uint32)outer - (uint32)distance;
    numerator *= (uint32)(sint32)(sint16)amplitude;
    sint32 quotient = (sint32)numerator / (outer - inner);
    return (uint32)(sint32)(sint16)quotient;
}

uint32 G3Function8001F3BC(uint32 packet, uint32 order_table, uint32 level, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8001f3bc, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x90;
    w_u32(guest_stack + 4, order_table);
    uint32 context = r_u32(game + 0x68);
    uint32 base = 0x1f800200;
    w_u32(frame + 0x64, context);
    w_u32(context + 4, packet);
    if ((sint32)level <= 0)
    {
        uint32 texture = r_u16(packet + 0xe);
        w_u16(base + 0xd4, texture);
        w_u16(base + 0x1ac, texture);
        texture = r_u16(packet + 0x1a);
        w_u16(base + 0xd6, texture);
        w_u16(base + 0x1ae, texture);
        if ((sint32)level < 0)
        {
            w_u32(frame + 0x58, r_u32(packet + 0xc));
            w_u32(frame + 0x5c, r_u32(packet + 0x18));
            w_u32(frame + 0x60, r_u32(packet + 0x24));
        }
        for (uint32 word = 0; word < 6; ++word)
            w_u32(base + word * 4, xport_gte_read_data(word));
        if ((sint32)level < 0)
        {
            for (uint32 group = 0; group < 2; ++group)
            {
                uint32 offset = group * 16;
                uint32 first = r_u32(packet + offset);
                uint32 second = r_u32(packet + offset + 4);
                uint32 third = r_u32(packet + offset + 8);
                uint32 fourth = r_u32(packet + offset + 0xc);
                w_u32(frame + 0x30 + offset, first);
                w_u32(frame + 0x34 + offset, second);
                w_u32(frame + 0x38 + offset, third);
                w_u32(frame + 0x3c + offset, fourth);
            }
            uint32 first = r_u32(packet + 0x20);
            uint32 second = r_u32(packet + 0x24);
            w_u32(frame + 0x50, first);
            w_u32(frame + 0x54, second);
            w_u32(frame + 0x10, frame + 0x5c);
            w_u32(frame + 0x14, frame + 0x60);
            uint32 color = r_u32(packet + 4);
            context = r_u32(frame + 0x64);
            uint32 order = r_u32(guest_stack + 4);
            w_u32(frame + 0x18, color);
            color = r_u32(packet + 0x10);
            w_u32(frame + 0x1c, color);
            color = r_u32(packet + 0x1c);
            w_u32(frame + 0x24, context);
            w_u32(frame + 0x28, order);
            w_u32(frame + 0x2c, level);
            w_u32(frame + 0x20, color);
            uint32 result = G3Function8001EB54(base, base + 8, base + 0x10, frame + 0x58, frame);
            if (result == 1)
            {
                context = r_u32(frame + 0x64);
                return r_u32(context + 4);
            }
            for (uint32 group = 0; group < 2; ++group)
            {
                uint32 offset = group * 16;
                first = r_u32(frame + 0x30 + offset);
                second = r_u32(frame + 0x34 + offset);
                uint32 third = r_u32(frame + 0x38 + offset);
                uint32 fourth = r_u32(frame + 0x3c + offset);
                w_u32(packet + offset, first);
                w_u32(packet + offset + 4, second);
                w_u32(packet + offset + 8, third);
                w_u32(packet + offset + 0xc, fourth);
            }
            first = r_u32(frame + 0x50);
            second = r_u32(frame + 0x54);
            w_u32(packet + 0x20, first);
            w_u32(packet + 0x24, second);
            level = 0;
        }
    }
    else if (level == 1)
    {
        base = 0x1f8002d8;
        for (uint32 word = 0; word < 6; ++word)
            w_u32(base + word * 4, xport_gte_read_data(word));
    }
    uint32 color = r_u32(packet + 4);
    uint32 first = r_u32(base + 4);
    uint32 second = r_u32(base + 0xc);
    uint32 third = r_u32(base + 0x10);
    uint32 fourth = r_u32(base + 0x14);
    w_u32(base + 0x60, color);
    color = r_u32(packet + 0x10);
    ++level;
    w_u32(base + 0x64, color);
    uint32 vertex_word = r_u32(base + 8);
    color = r_u32(packet + 0x1c);
    w_u32(base + 0x1c, first);
    w_u32(base + 0x20, vertex_word);
    vertex_word = r_u32(base);
    sint32 half = (sint16)r_u16(base + 0x20);
    sint32 carried_y = (sint16)r_u16(base + 0x22);
    w_u32(base + 0x24, second);
    w_u32(base + 0x28, third);
    w_u32(base + 0x2c, fourth);
    w_u32(base + 0x68, color);
    w_u32(base + 0x18, vertex_word);
    sint32 component = r_s16(base + 0x18);
    sint32 sum = component + half;
    half = (sint16)r_u16(base + 0x22);
    w_u16(base + 0x30, (uint32)(sum / 2));
    component = r_s16(base + 0x1a);
    sum = component + half;
    half = (sint16)r_u16(base + 0x24);
    w_u16(base + 0x32, (uint32)(sum / 2));
    component = r_s16(base + 0x1c);
    sum = component + half;
    half = (sint16)r_u16(base + 0x20);
    w_u16(base + 0x34, (uint32)(sum / 2));
    component = r_s16(base + 0x28);
    sum = half + component;
    component = r_s16(base + 0x2a);
    w_u16(base + 0x38, (uint32)(sum / 2));
    sum = carried_y + component;
    half = (sint16)r_u16(base + 0x24);
    w_u16(base + 0x3a, (uint32)(sum / 2));
    component = r_s16(base + 0x2c);
    sum = half + component;
    w_u16(base + 0x3c, (uint32)(sum / 2));
    component = r_s16(base + 0x28);
    half = r_s16(base + 0x18);
    carried_y = r_s16(base + 0x1a);
    sum = component + half;
    half = r_s16(base + 0x2a);
    w_u16(base + 0x40, (uint32)(sum / 2));
    sum = half + carried_y;
    component = r_s16(base + 0x2c);
    half = r_s16(base + 0x1c);
    w_u16(base + 0x42, (uint32)(sum / 2));
    w_u16(base + 0x44, (uint32)((component + half) / 2));
    w_u32(base + 0x48, r_u32(packet + 0xc));
    w_u32(base + 0x4c, r_u32(packet + 0x18));
    first = r_u8(base + 0x48);
    second = r_u8(base + 0x4c);
    third = r_u32(packet + 0x24);
    first += second;
    second = r_u8(base + 0x49);
    w_u32(base + 0x50, third);
    third = r_u8(base + 0x4d);
    w_u8(base + 0x54, first >> 1);
    first = r_u8(base + 0x4c);
    second += third;
    third = r_u8(base + 0x50);
    w_u8(base + 0x55, second >> 1);
    second = r_u8(base + 0x4d);
    first += third;
    third = r_u8(base + 0x51);
    w_u8(base + 0x58, first >> 1);
    first = r_u8(base + 0x50);
    second += third;
    third = r_u8(base + 0x48);
    w_u8(base + 0x59, second >> 1);
    second = r_u8(base + 0x51);
    first += third;
    third = r_u8(base + 0x49);
    w_u8(base + 0x5c, first >> 1);
    first = r_u8(base + 0x60);
    second += third;
    third = r_u8(base + 0x64);
    w_u8(base + 0x5d, second >> 1);
    w_u8(base + 0x6c, (first + third) >> 1);
    first = r_u8(base + 0x61);
    second = r_u8(base + 0x65);
    third = r_u8(base + 0x66);
    first += second;
    second = r_u8(base + 0x62);
    w_u8(base + 0x6d, first >> 1);
    first = r_u8(base + 0x64);
    second += third;
    third = r_u8(base + 0x68);
    w_u8(base + 0x6e, second >> 1);
    second = r_u8(base + 0x65);
    first += third;
    third = r_u8(base + 0x69);
    w_u8(base + 0x70, first >> 1);
    first = r_u8(base + 0x66);
    second += third;
    third = r_u8(base + 0x6a);
    w_u8(base + 0x71, second >> 1);
    second = r_u8(base + 0x68);
    first += third;
    third = r_u8(base + 0x60);
    w_u8(base + 0x72, first >> 1);
    first = r_u8(base + 0x61);
    second += third;
    third = r_u8(base + 0x69);
    w_u8(base + 0x74, second >> 1);
    second = r_u8(base + 0x62);
    third += first;
    first = r_u8(base + 0x6a);
    w_u8(base + 0x75, third >> 1);
    first += second;
    second = r_u8(base + 0x63);
    w_u8(base + 0x76, first >> 1);
    w_u8(base + 0x77, second);
    w_u8(base + 0x73, second);
    w_u8(base + 0x6f, second);
    w_u32(frame + 0x10, base + 0x54);
    w_u32(frame + 0x14, base + 0x5c);
    color = r_u32(base + 0x60);
    context = r_u32(frame + 0x64);
    uint32 order = r_u32(guest_stack + 4);
    w_u32(frame + 0x18, color);
    color = r_u32(base + 0x6c);
    w_u32(frame + 0x1c, color);
    color = r_u32(base + 0x74);
    w_u32(frame + 0x24, context);
    w_u32(frame + 0x28, order);
    w_u32(frame + 0x2c, level);
    w_u32(frame + 0x20, color);
    uint32 result = G3Function8001EB54(base + 0x18, base + 0x30, base + 0x40, base + 0x48, frame);
    w_u32(frame + 0x10, base + 0x58);
    w_u32(frame + 0x14, base + 0x50);
    color = r_u32(base + 0x74);
    context = r_u32(frame + 0x64);
    order = r_u32(guest_stack + 4);
    w_u32(frame + 0x18, color);
    color = r_u32(base + 0x70);
    w_u32(frame + 0x1c, color);
    color = r_u32(base + 0x68);
    w_u32(frame + 0x24, context);
    w_u32(frame + 0x28, order);
    w_u32(frame + 0x2c, level);
    w_u32(frame + 0x20, color);
    result |= G3Function8001EB54(base + 0x40, base + 0x38, base + 0x28, base + 0x5c, frame);
    w_u32(frame + 0x10, base + 0x58);
    w_u32(frame + 0x14, base + 0x5c);
    color = r_u32(base + 0x6c);
    context = r_u32(frame + 0x64);
    order = r_u32(guest_stack + 4);
    w_u32(frame + 0x18, color);
    color = r_u32(base + 0x70);
    w_u32(frame + 0x1c, color);
    color = r_u32(base + 0x74);
    w_u32(frame + 0x24, context);
    w_u32(frame + 0x28, order);
    w_u32(frame + 0x2c, level);
    w_u32(frame + 0x20, color);
    result |= G3Function8001EB54(base + 0x30, base + 0x38, base + 0x40, base + 0x54, frame);
    w_u32(frame + 0x10, base + 0x4c);
    w_u32(frame + 0x14, base + 0x58);
    color = r_u32(base + 0x6c);
    context = r_u32(frame + 0x64);
    order = r_u32(guest_stack + 4);
    w_u32(frame + 0x18, color);
    color = r_u32(base + 0x64);
    w_u32(frame + 0x1c, color);
    color = r_u32(base + 0x70);
    w_u32(frame + 0x24, context);
    w_u32(frame + 0x28, order);
    w_u32(frame + 0x2c, level);
    w_u32(frame + 0x20, color);
    result |= G3Function8001EB54(base + 0x30, base | 0x20, base + 0x38, base + 0x54, frame);
    if ((result & 5) == 4)
        return 0;
    context = r_u32(frame + 0x64);
    return r_u32(context + 4);
}

uint32 G3Function8001EB54(uint32 vertex0, uint32 vertex1, uint32 vertex2, uint32 uv0, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8001eb54, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 bias = r_u32(0x1f8000cc);
    uint32 context = r_u32(guest_stack + 0x24);
    w_u32(guest_stack, vertex0);
    w_u32(guest_stack + 0xc, uv0);
    bias <<= 4;
    for (uint32 index = 0; index < 3; ++index)
    {
        uint32 vertex = index == 0 ? vertex0 : (index == 1 ? vertex1 : vertex2);
        xport_gte_write_data(0, r_u32(vertex));
        xport_gte_write_data(1, r_u32(vertex + 4));
        xport_gte_execute(0x480012);
        uint32 output = 0x1f800278 + index * 12;
        w_u32(output, xport_gte_read_data(25));
        w_u32(output + 4, xport_gte_read_data(26));
        w_u32(output + 8, xport_gte_read_data(27));
    }
    if (r_s32(0x1f800280) < 320 && r_s32(0x1f80028c) < 320 && r_s32(0x1f800298) < 320)
        return 2;
    sint32 maximum_depth = 320;
    uint32 pointer = r_u32(guest_stack + 0xc);
    uint32 packet = r_u32(context + 4);
    uint32 value = r_u32(pointer);
    w_u32(0x1f8002b8, value);
    pointer = r_u32(guest_stack + 0x10);
    value = r_u32(pointer);
    w_u32(0x1f8002bc, value);
    pointer = r_u32(guest_stack + 0x14);
    uint32 last_uv = r_u32(pointer);
    value = r_u32(guest_stack + 0x18);
    w_u32(0x1f8002a8, value);
    value = r_u32(guest_stack + 0x1c);
    w_u32(0x1f8002ac, value);
    value = r_u32(guest_stack + 0x20);
    w_u32(0x1f8002c0, last_uv);
    w_u32(0x1f8002b0, value);
    uint32 count = 0;
    uint32 common_outcode = 0xffff;
    uint32 color_output = packet + 4;
    uint32 coordinate_output = packet + 0xa;
    for (uint32 index = 0; index < 3; ++index)
    {
        uint32 next = index + 1;
        if (next >= 3)
            next = 0;
        uint32 current_transform = 0x1f800200 + index * 12;
        uint32 next_transform = 0x1f800200 + next * 12;
        bool visible = r_s32(current_transform + 0x80) >= 320;
        bool next_visible = r_s32(next_transform + 0x80) >= 320;
        if (visible)
        {
            uint32 vertex = index == 0 ? r_u32(guest_stack) : (index == 1 ? vertex1 : vertex2);
            xport_gte_write_data(0, r_u32(vertex));
            xport_gte_write_data(1, r_u32(vertex + 4));
            xport_gte_execute(0x180001);
            value = r_u32(0x1f8002b8 + index * 4);
            w_u32(coordinate_output + 2, value);
            value = r_u32(0x1f8002a8 + index * 4);
            w_u32(color_output, value);
            w_u32(color_output + 4, xport_gte_read_data(14));
            w_u32(0x1f8002c4 + count * 4, xport_gte_read_data(14));
            w_u32(frame + 0x10, xport_gte_read_data(19));
            uint32 depth = r_u32(frame + 0x10) + bias;
            bool replace_depth = maximum_depth < (sint32)depth;
            w_u32(frame + 0x10, depth);
            if (replace_depth)
                maximum_depth = (sint32)depth;
            sint32 x = r_s16(coordinate_output - 2);
            sint32 y = r_s16(coordinate_output);
            uint32 outcode = (uint32)x >> 31;
            if (y < 0)
                outcode |= 2;
            if (x >= 513)
                outcode |= 4;
            if (y >= 241)
                outcode |= 8;
            common_outcode &= outcode;
            ++count;
            coordinate_output += 12;
            color_output += 12;
        }
        if (visible != next_visible)
        {
            uint32 current_z = r_u32(current_transform + 0x80);
            uint32 next_z = r_u32(next_transform + 0x80);
            uint32 numerator = (current_z - 320) << 12;
            uint32 denominator = current_z - next_z;
            uint32 quotient;
            if (denominator == 0)
                quotient = (sint32)numerator < 0 ? 1 : 0xffffffff;
            else if (numerator == 0x80000000 && denominator == 0xffffffff)
                quotient = 0x80000000;
            else
                quotient = (uint32)((sint32)numerator / (sint32)denominator);
            uint32 next_x = r_u32(next_transform + 0x78);
            uint32 current_x = r_u32(current_transform + 0x78);
            uint32 weight = (sint32)quotient < 0 ? 0 - quotient : quotient;
            uint32 product = (next_x - current_x) * weight;
            uint32 complement = 0x1000 - weight;
            if ((sint32)product < 0)
                product += 0xfff;
            uint32 projected = current_x + (uint32)((sint32)product >> 12) + 0x100;
            if ((sint32)projected >= 1024)
                projected = 1023;
            if ((sint32)projected < -1023)
                projected = (uint32)-1023;
            w_u16(coordinate_output - 2, projected);
            uint32 next_y = r_u32(next_transform + 0x7c);
            uint32 current_y = r_u32(current_transform + 0x7c);
            product = (next_y - current_y) * weight;
            if ((sint32)product < 0)
                product += 0xfff;
            projected = current_y + (uint32)((sint32)product >> 12) + 0x78;
            if ((sint32)projected >= 1024)
                projected = 1023;
            if ((sint32)projected < -1023)
                projected = (uint32)-1023;
            w_u16(coordinate_output, projected);
            for (uint32 attribute = 0; attribute < 2; ++attribute)
            {
                uint32 channels = attribute == 0 ? 2 : 3;
                uint32 samples[3];
                uint32 attribute_base = attribute == 0 ? 0x1f8002b8 : 0x1f8002a8;
                xport_gte_write_data(8, complement);
                for (uint32 channel = 0; channel < channels; ++channel)
                    samples[channel] = r_u8(attribute_base + index * 4 + channel);
                for (uint32 channel = 0; channel < channels; ++channel)
                    xport_gte_write_data(9 + channel, samples[channel]);
                xport_gte_execute(0x198003d);
                xport_gte_write_data(8, weight);
                for (uint32 channel = 0; channel < channels; ++channel)
                    samples[channel] = r_u8(attribute_base + next * 4 + channel);
                for (uint32 channel = 0; channel < channels; ++channel)
                    xport_gte_write_data(9 + channel, samples[channel]);
                xport_gte_execute(0x1a8003e);
                for (uint32 channel = 0; channel < channels; ++channel)
                    samples[channel] = xport_gte_read_data(9 + channel);
                uint32 output = color_output + (attribute == 0 ? 8 : 0);
                for (uint32 channel = 0; channel < channels; ++channel)
                    w_u8(output + channel, samples[channel]);
            }
            value = r_u32(coordinate_output - 2);
            w_u32(0x1f8002c4 + count * 4, value);
            sint32 x = r_s16(coordinate_output - 2);
            sint32 y = r_s16(coordinate_output);
            uint32 outcode = (uint32)x >> 31;
            if (y < 0)
                outcode |= 2;
            if (x >= 513)
                outcode |= 4;
            if (y >= 241)
                outcode |= 8;
            common_outcode &= outcode;
            ++count;
            coordinate_output += 12;
            color_output += 12;
        }
    }
    if (common_outcode != 0)
        return 2;
    uint32 game = r_u32(0x80097048);
    uint32 shift = r_u32(game + 0x2c);
    sint32 depth_limit = r_s32(game + 0x28);
    maximum_depth >>= shift & 31;
    if (maximum_depth >= depth_limit)
        return 2;
    sint32 level = r_s32(guest_stack + 0x2c);
    if (level <= 0)
    {
        bool subdivide = false;
        for (uint32 index = 0; index < count; ++index)
        {
            uint32 next = index + 1;
            if (next >= count)
                next = 0;
            sint32 current = r_s16(0x1f8002c4 + index * 4);
            sint32 adjacent = r_s16(0x1f8002c4 + next * 4);
            sint32 difference = current - adjacent;
            if (difference < 0)
                difference = -difference;
            if (difference >= 1024)
            {
                subdivide = true;
                break;
            }
            current = r_s16(0x1f8002c6 + index * 4);
            adjacent = r_s16(0x1f8002c6 + next * 4);
            difference = current - adjacent;
            if (difference < 0)
                difference = -difference;
            if (difference >= 512)
            {
                subdivide = true;
                break;
            }
        }
        if (!subdivide && count == 4)
        {
            sint32 current = (sint16)r_u16(0x1f8002d0);
            sint32 adjacent = (sint16)r_u16(0x1f8002c8);
            sint32 difference = current - adjacent;
            if (difference < 0)
                difference = -difference;
            if (difference >= 1024)
                subdivide = true;
            else
            {
                current = (sint16)r_u16(0x1f8002d2);
                adjacent = (sint16)r_u16(0x1f8002ca);
                difference = current - adjacent;
                if (difference < 0)
                    difference = -difference;
                subdivide = difference >= 512;
            }
        }
        if (subdivide)
        {
            if (r_u32(guest_stack + 0x2c) == 0xffffffff)
                return 4;
            uint32 original_vertex = r_u32(guest_stack);
            xport_gte_write_data(0, r_u32(original_vertex));
            xport_gte_write_data(1, r_u32(original_vertex + 4));
            xport_gte_write_data(2, r_u32(vertex1));
            xport_gte_write_data(3, r_u32(vertex1 + 4));
            xport_gte_write_data(4, r_u32(vertex2));
            xport_gte_write_data(5, r_u32(vertex2 + 4));
            w_u8(0x1f8002b3, r_u8(0x1f8002ab));
            pointer = r_u32(guest_stack + 0xc);
            w_u32(packet + 0xc, r_u32(pointer));
            pointer = r_u32(guest_stack + 0x10);
            w_u32(packet + 0x18, r_u32(pointer));
            pointer = r_u32(guest_stack + 0x14);
            w_u32(packet + 0x24, r_u32(pointer));
            uint32 order_table = r_u32(guest_stack + 0x28);
            G3Function8001F3BC(packet, order_table, 1, frame);
            return 1;
        }
    }
    uint32 packet_size;
    uint32 tag;
    if (count == 3)
    {
        uint32 color = r_u32(guest_stack + 0x18);
        uint32 command = (uint32)((sint32)color >> 24);
        w_u8(packet + 7, command);
        if ((command & 0x34) != 0x34 || (r_u8(packet + 7) & 0xcb) >= 4)
        {
            w_u8(packet + 3, 9);
            w_u8(packet + 7, 0x34);
        }
        packet_size = 0x28;
        tag = 0x09000000;
    }
    else if (count == 4)
    {
        uint32 color = r_u32(guest_stack + 0x18);
        uint32 third_color = r_u32(packet + 0x1c);
        uint32 third_xy = r_u32(packet + 0x20);
        uint32 third_uv = r_u32(packet + 0x24);
        uint32 fourth_color = r_u32(packet + 0x28);
        uint32 fourth_xy = r_u32(packet + 0x2c);
        uint32 fourth_uv = r_u32(packet + 0x30);
        uint32 command = (uint32)((sint32)color >> 24) | 8;
        w_u8(packet + 7, command);
        w_u32(packet + 0x1c, fourth_color);
        w_u32(packet + 0x20, fourth_xy);
        w_u32(packet + 0x24, fourth_uv);
        w_u32(packet + 0x28, third_color);
        w_u32(packet + 0x2c, third_xy);
        w_u32(packet + 0x30, third_uv);
        if ((command & 0x3c) != 0x3c || (r_u8(packet + 7) & 0xc3) >= 4)
        {
            w_u8(packet + 3, 12);
            w_u8(packet + 7, 0x3c);
        }
        packet_size = 0x34;
        tag = 0x0c000000;
    }
    else
        return 2;
    w_u16(packet + 0xe, r_u16(0x1f8002d4));
    w_u16(packet + 0x1a, r_u16(0x1f8002d6));
    uint32 order_table = r_u32(guest_stack + 0x28);
    uint32 entry = order_table + ((uint32)maximum_depth << 2);
    uint32 previous = r_u32(entry);
    w_u32(packet, (previous & 0xffffff) | tag);
    w_u32(entry, packet & 0xffffff);
    uint32 cursor = r_u32(context + 4);
    uint32 total = r_u32(context);
    w_u32(context + 4, cursor + packet_size);
    w_u32(context, total + 1);
    return 1;
}

uint32 G3Function80016ECC(void)
{
    FUNCTION_MARKER(0x80016ecc, "SLUS_008.06");
    return 1;
}

uint32 G3Function80065948(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80065948, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (G3Function80012A14() == 0)
    {
        G3Function80021A74(0x1cc);
        uint32 text = G3Function80065D38(0x42);
        G3Function800234A0(text, 0xe1, 0, 0, frame, 0);
    }
    uint32 count = r_u32(0x8009efb0) + 1;
    w_u32(0x8009efb0, count);
    if (count == 0x294)
    {
        w_u32(0x8009efb4, 2);
        return G3Function800275E0(frame);
    }
    return 2;
}

uint32 G3Function80065E14(uint32 object, uint32 table, uint32 resource, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80065e14, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 index = r_u32(guest_stack + 0x10);
    uint32 created = G3Function8002A454(object, resource, frame);
    w_u32(table + (index << 2), created);
    if (created == 0)
        return 0;
    uint32 flags = r_u32(created + 0x10);
    uint32 token = r_u32(guest_stack + 0x14);
    flags |= 0x480;
    w_u16(created + 0x9e, token);
    w_u32(created + 0x10, flags);
    return flags;
}

uint32 G3Function80065E70(uint32 object, uint32 table, uint32 values, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80065e70, "SLUS_008.06");
    uint32 frame = guest_stack - 0x68;
    uint32 index = 0;
    uint32 token = 0;
    w_u32(guest_stack + 4, table);
    w_u32(guest_stack + 8, values);
    uint32 data = r_u32(object + 0x20);
    uint32 direction = r_u16(data + 0xc);
    w_u16(frame + 0x38, direction);
    uint32 resource = G3Function8003AA48(0x8009ff00);
    w_u32(frame + 0x30, resource);
    if (resource == 0)
        return 0;
    uint32 model = r_u32(resource + 0xc);
    model = r_u32(model);
    G3Function80039834(model);
    uint32 source = object + 0x58;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
    uint32 low_lane = source & 3;
    uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
    uint32 xy = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    uint32 z = (uint32)r_s16(object + 0x5c);
    uint32 destination = frame + 0x20;
    high_address = destination + 3;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, xy >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    low_lane = destination & 3;
    w_masked_u32(destination & 0xfffffffc, xy << (low_lane * 8), (15 << low_lane) & 15);
    w_u16(frame + 0x24, z);
    uint32 angle = (uint32)r_s16(frame + 0x38);
    z = r_u16(object + 0x5c);
    w_u16(object + 0x5c, z + 0x96);
    uint32 first_wave = G3Function8005B470(angle);
    uint32 second_wave = G3Function8005B590(angle);
    uint32 first_offset = first_wave * 233;
    uint32 second_offset = 0 - second_wave * 350;
    uint32 x = r_u16(object + 0x58);
    x += (uint32)((sint32)first_offset >> 12);
    x -= (uint32)((sint32)second_offset >> 12);
    w_u16(object + 0x58, x);
    first_wave = G3Function8005B590(angle);
    second_wave = G3Function8005B470(angle);
    first_offset = first_wave * 233;
    second_offset = 0 - second_wave * 350;
    uint32 y = r_u16(object + 0x5a);
    y += (uint32)((sint32)first_offset >> 12);
    y += (uint32)((sint32)second_offset >> 12);
    w_u16(object + 0x5a, y);
    table = r_u32(guest_stack + 4);
    uint32 selector = r_u8(table + 0x3e);
    if (selector < 0xb)
    {
        first_wave = G3Function8005B470(angle);
        w_u16(frame + 0x18, (first_wave * 55) >> 10);
        second_wave = G3Function8005B590(angle);
        w_u16(frame + 0x1a, (second_wave * 55) >> 10);
        x = r_u16(object + 0x58);
        uint32 step = r_u16(frame + 0x18);
        w_u16(object + 0x58, x - step);
        y = r_u16(object + 0x5a);
        step = r_u16(frame + 0x1a);
        w_u16(object + 0x5a, y - step);
        first_wave = G3Function8005B470(angle);
        w_u16(frame + 0x18, (first_wave * 73) >> 11);
        second_wave = G3Function8005B590(angle);
        w_u16(frame + 0x1a, (second_wave * 73) >> 11);
        for (uint32 item = 0; item < 3; ++item)
        {
            table = r_u32(guest_stack + 4);
            uint32 game = r_u32(0x80097048);
            selector = r_u8(table + 0x3e);
            uint32 flags = r_u8(game + selector + 0x67f8);
            token = (flags & ((uint32)1 << (index & 31))) != 0 ? 0xb : 0xa;
            table = r_u32(guest_stack + 4);
            (void)r_u32(guest_stack + 8);
            resource = r_u32(frame + 0x30);
            w_u32(frame + 0x10, index);
            w_u32(frame + 0x14, token);
            G3Function80065E14(object, table, resource, frame);
            x = r_u16(object + 0x58);
            step = r_u16(frame + 0x18);
            w_u16(object + 0x58, x + step);
            y = r_u16(object + 0x5a);
            step = r_u16(frame + 0x1a);
            w_u16(object + 0x5a, y + step);
            ++index;
        }
        table = r_u32(guest_stack + 4);
        uint32 game = r_u32(0x80097048);
        selector = r_u8(table + 0x3e);
        uint32 flags = r_u32(game + 0x6838);
        token = (flags & ((uint32)1 << (selector & 31))) != 0 ? 0xb : 0xa;
        table = r_u32(guest_stack + 4);
        (void)r_u32(guest_stack + 8);
        resource = r_u32(frame + 0x30);
        w_u32(frame + 0x10, index);
        w_u32(frame + 0x14, token);
        G3Function80065E14(object, table, resource, frame);
        ++index;
    }
    else if (((selector - 0x14) & 0xff) < 3)
    {
        table = r_u32(guest_stack + 4);
        uint32 game = r_u32(0x80097048);
        selector = r_u8(table + 0x3e);
        uint32 flags = r_u32(game + 0x6838);
        bool present = (flags & ((uint32)1 << (selector & 31))) != 0;
        if (selector == 0x14 || selector == 0x15)
        {
            token = selector == 0x14 ? (present ? 0x16 : 0x15) : (present ? 0x18 : 0x17);
            table = r_u32(guest_stack + 4);
            (void)r_u32(guest_stack + 8);
            resource = r_u32(frame + 0x30);
            w_u32(frame + 0x10, 0);
            w_u32(frame + 0x14, token);
            G3Function80065E14(object, table, resource, frame);
            index = 1;
        }
    }
    else if (((selector - 0x10) & 0xff) < 4)
    {
        uint32 name = 0;
        uint32 present_token = 0;
        uint32 absent_token = 0;
        switch (selector)
        {
        case 0x13: name = 0x8009ff0c; present_token = 0x14; absent_token = 0xe; break;
        case 0x11: name = 0x8009ff18; present_token = 0xf; absent_token = 0x10; break;
        case 0x10: name = 0x8009ff24; present_token = 0x11; absent_token = 0x12; break;
        case 0x12: name = 0x8009ff30; present_token = 0x13; absent_token = 0xd; break;
        }
        uint32 game = r_u32(0x80097048);
        G3Function8002786C(game, name, frame + 0x28, 0);
        token = r_u32(frame + 0x28) != 0 ? present_token : absent_token;
        table = r_u32(guest_stack + 4);
        (void)r_u32(guest_stack + 8);
        resource = r_u32(frame + 0x30);
        w_u32(frame + 0x10, index);
        w_u32(frame + 0x14, token);
        G3Function80065E14(object, table, resource, frame);
        ++index;
    }
    else if (((selector - 0xb) & 0xff) < 5)
    {
        uint32 code = G3Function8003AA48(0x8009ff3c);
        table = r_u32(guest_stack + 4);
        uint32 game = r_u32(0x80097048);
        w_u32(frame + 0x2c, 1);
        selector = r_u8(table + 0x3e);
        values = r_u32(guest_stack + 8);
        uint32 group = selector - 0xb;
        uint32 variant = (uint32)((sint32)((uint32)r_u16(values) << 16) >> 24);
        uint32 record = 0x80099d08 + (((group << 1) + group + variant) << 3);
        uint32 name = r_u32(record + 4);
        G3Function8002786C(game, name, frame + 0x2c, 0);
        z = r_u16(object + 0x5c);
        uint32 present = r_u32(frame + 0x2c);
        w_u16(object + 0x5c, z + 0x28);
        if (present != 0 && code != 0)
        {
            first_wave = G3Function8005B470(angle);
            w_u16(frame + 0x18, (uint32)((sint32)(first_wave * 225) >> 12));
            second_wave = G3Function8005B590(angle);
            w_u16(frame + 0x1a, (uint32)((sint32)(second_wave * 225) >> 12));
            x = r_u16(object + 0x58);
            uint32 step = r_u16(frame + 0x18);
            w_u16(object + 0x58, x - step);
            y = r_u16(object + 0x5a);
            step = r_u16(frame + 0x1a);
            w_u16(object + 0x5a, y - step);
            first_wave = G3Function8005B470(angle);
            w_u16(frame + 0x18, (first_wave * 45) >> 11);
            second_wave = G3Function8005B590(angle);
            uint32 cursor = r_u32(guest_stack + 4);
            w_u16(frame + 0x1a, (second_wave * 45) >> 11);
            for (uint32 item = 0; item < 6; ++item)
            {
                table = r_u32(guest_stack + 4);
                uint32 code_index = r_u32(record);
                (void)r_u32(guest_stack + 8);
                uint32 records = r_u32(code + 0x1c);
                uint32 offset = code_index * 12;
                uint32 characters = r_u32(records + 0x14);
                resource = r_u32(frame + 0x30);
                token = r_u8(characters + offset + item) + 0x19;
                w_u32(frame + 0x10, index);
                w_u32(frame + 0x14, token);
                G3Function80065E14(object, table, resource, frame);
                x = r_u16(object + 0x58);
                step = r_u16(frame + 0x18);
                w_u16(object + 0x58, x + step);
                y = r_u16(object + 0x5a);
                step = r_u16(frame + 0x1a);
                w_u16(object + 0x5a, y + step);
                uint32 created = r_u32(cursor);
                ++index;
                w_u16(created + 0x68, 0x800);
                created = r_u32(cursor);
                w_u16(created + 0x6a, 0x800);
                created = r_u32(cursor);
                w_u16(created + 0x6c, 0x800);
                cursor += 4;
            }
        }
        z = r_u16(object + 0x5c);
        w_u16(object + 0x5c, z - 0x28);
    }
    angle = (uint32)r_s16(frame + 0x38);
    first_wave = G3Function8005B470(angle);
    second_wave = G3Function8005B590(angle);
    first_offset = 0 - first_wave * 417;
    second_offset = 0 - second_wave * 350;
    x = r_u16(frame + 0x20);
    x += (uint32)((sint32)first_offset >> 12);
    x -= (uint32)((sint32)second_offset >> 12);
    w_u16(object + 0x58, x);
    first_wave = G3Function8005B590(angle);
    second_wave = G3Function8005B470(angle);
    first_offset = 0 - first_wave * 417;
    second_offset = 0 - second_wave * 350;
    y = r_u16(frame + 0x22);
    y += (uint32)((sint32)first_offset >> 12);
    y += (uint32)((sint32)second_offset >> 12);
    w_u16(object + 0x5a, y);
    table = r_u32(guest_stack + 4);
    selector = r_u8(table + 0x3e);
    token = selector - 0xb < 5 ? 0xc : 0xb;
    table = r_u32(guest_stack + 4);
    (void)r_u32(guest_stack + 8);
    resource = r_u32(frame + 0x30);
    w_u32(frame + 0x10, index);
    w_u32(frame + 0x14, token);
    G3Function80065E14(object, table, resource, frame);
    z = r_u16(object + 0x5c);
    w_u16(object + 0x5c, z + 0x28);
    values = r_u32(guest_stack + 8);
    ++index;
    uint32 digits = r_u8(values) < 10 ? 1 : 2;
    uint32 horizontal = 0xffffff34 - (1 - digits) * 35;
    table = r_u32(guest_stack + 4);
    uint32 cursor = table + (index << 2);
    for (uint32 digit = 0; digit < digits; ++digit)
    {
        first_wave = G3Function8005B470(angle);
        second_wave = G3Function8005B590(angle);
        first_offset = horizontal * first_wave;
        second_offset = 0 - second_wave * 350;
        x = r_u16(frame + 0x20);
        x += (uint32)((sint32)first_offset >> 12);
        x -= (uint32)((sint32)second_offset >> 12);
        w_u16(object + 0x58, x);
        first_wave = G3Function8005B590(angle);
        second_wave = G3Function8005B470(angle);
        first_offset = horizontal * first_wave;
        second_offset = 0 - second_wave * 350;
        y = r_u16(frame + 0x22);
        y += (uint32)((sint32)first_offset >> 12);
        y += (uint32)((sint32)second_offset >> 12);
        w_u16(object + 0x5a, y);
        values = r_u32(guest_stack + 8);
        uint32 value = r_u8(values);
        token = digit == 0 ? value % 10 : value / 10;
        table = r_u32(guest_stack + 4);
        (void)r_u32(guest_stack + 8);
        resource = r_u32(frame + 0x30);
        w_u32(frame + 0x10, index);
        w_u32(frame + 0x14, token);
        G3Function80065E14(object, table, resource, frame);
        ++index;
        uint32 created = r_u32(cursor);
        horizontal -= 0x46;
        w_u16(created + 0x68, 0x800);
        created = r_u32(cursor);
        w_u16(created + 0x6a, 0x800);
        created = r_u32(cursor);
        w_u16(created + 0x6c, 0x800);
        cursor += 4;
    }
    source = frame + 0x20;
    high_address = source + 3;
    high_lane = high_address & 3;
    high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
    low_lane = source & 3;
    low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
    xy = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    z = (uint32)r_s16(frame + 0x24);
    destination = object + 0x58;
    high_address = destination + 3;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, xy >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    low_lane = destination & 3;
    w_masked_u32(destination & 0xfffffffc, xy << (low_lane * 8), (15 << low_lane) & 15);
    w_u16(object + 0x5c, z);
    return 0;
}

uint32 G3Function80066814(uint32 object, uint32 extension, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80066814, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 resource = G3Function8003AA48(0x8009ff48);
    uint32 created = G3Function8002A454(object, resource, frame);
    w_u32(extension + 0x38, created);
    if (created == 0)
        return 0;
    uint32 flags = r_u32(created + 0x10);
    w_u32(created + 0x10, flags | 0x100000);
    created = r_u32(extension + 0x38);
    flags = r_u32(created + 0x10) | 0x400;
    w_u32(created + 0x10, flags);
    return flags;
}

uint32 G3Function8006688C(uint32 object, uint32 extension, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006688c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 linked = r_u32(extension + 0x34);
    if (linked != 0)
    {
        uint32 flags = r_u32(linked + 0x10);
        w_u32(linked + 0x10, flags & 0xfffffbff);
        linked = r_u32(extension + 0x38);
        G3Function8002A9AC(linked, frame);
    }
    linked = r_u32(extension + 0x38);
    if (linked != 0)
    {
        uint32 flags = r_u32(linked + 0x10);
        w_u32(linked + 0x10, flags & 0xfffffbff);
        linked = r_u32(extension + 0x38);
        G3Function8002A9AC(linked, frame);
    }
    uint32 cursor = extension;
    for (uint32 index = 0; index < 13; ++index)
    {
        linked = r_u32(cursor);
        if (linked != 0)
        {
            uint32 flags = r_u32(linked + 0x10);
            w_u32(linked + 0x10, flags & 0xfffffbff);
            linked = r_u32(cursor);
            G3Function8002A9AC(linked, frame);
        }
        cursor += 4;
    }
    return 0;
}

uint32 G3Function80066978(uint32 object, uint32 game, uint32 extension, uint32 descriptor, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80066978, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    uint32 suffix = (uint32)r_s16(descriptor + 2);
    G3SdkSprintfStringInteger80088A1C(frame + 0x10, 0x8009ff54, descriptor + 4, suffix);
    uint32 selector = G3Function80038F6C(frame + 0x10, frame);
    w_u8(extension + 0x3e, selector);
    uint32 flags = r_u32(object + 0x10);
    uint32 data = r_u32(object + 0x1c);
    w_u32(object + 0x10, flags | 0x80);
    uint32 model = r_u32(data + 0xc);
    model = r_u32(model);
    G3Function80039834(model);
    selector = r_u8(extension + 0x3e);
    sint32 available = r_s32(game + (selector - 0xb < 5 ? 0x6828 : 0x682c));
    uint32 required = r_u8(descriptor);
    uint32 current;
    if (available < (sint32)required)
    {
        w_u32(object + 0x154, 2);
        current = r_u8(extension + 0x3c);
        w_u8(extension + 0x3f, 0);
        w_u16(object + 0x9e, current);
        w_u8(extension + 0x3c, 0);
        current = 3;
    }
    else
    {
        w_u32(object + 0x154, 0);
        selector = r_u8(extension + 0x3e);
        w_u8(extension + 0x3f, 1);
        uint32 entry = 0x80099708 + selector * 28;
        current = r_u16(entry + 0xc);
        w_u16(object + 0x9e, current - 1);
        current = r_u8(object + 0x9e);
        w_u8(extension + 0x3c, current);
        current = r_u8(object + 0x9e);
    }
    w_u8(extension + 0x3d, current);
    data = r_u32(object + 0x20);
    flags = r_u32(data + 0x18);
    if ((flags & 0x80) != 0)
    {
        flags = r_u32(object + 0x10) | 0x800;
        w_u32(object + 0x10, flags);
        return flags;
    }
    return G3Function80065E70(object, extension, descriptor, frame);
}

uint32 G3Function80066AE8(uint32 object, uint32 game, uint32 extension, uint32 descriptor, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80066ae8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x10, flags & 0xfffff7ff);
    uint32 enabled = r_u8(extension + 0x3f);
    if (enabled != 0)
    {
        if (r_u32(extension) == 0)
            G3Function80065E70(object, extension, descriptor, frame);
        if (r_u32(extension + 0x38) == 0)
            G3Function80066814(object, extension, frame);
    }
    else
    {
        uint32 selector = r_u8(extension + 0x3e);
        sint32 available = r_s32(game + (selector - 0xb < 5 ? 0x6828 : 0x682c));
        uint32 required = r_u8(descriptor);
        if (available >= (sint32)required)
        {
            if (r_u8(extension + 0x41) == 0)
                w_u8(extension + 0x41, 0x69);
            sint32 random = (sint32)G3SdkRandom80027FA8();
            uint32 delay = r_u8(extension + 0x41);
            sint32 divisor = (sint32)((delay >> 3) + 1);
            uint32 bound = 3;
            if (random % divisor == 0)
            {
                selector = r_u8(extension + 0x3e);
                uint32 entry = 0x80099708 + selector * 28;
                bound = r_u8(entry + 0xc) - 1;
                w_u8(extension + 0x3c, bound);
            }
            else
                w_u8(extension + 0x3c, 0);
            w_u8(extension + 0x3d, bound);
            delay = r_u8(extension + 0x41) - 1;
            w_u8(extension + 0x41, delay);
            if ((delay & 0xff) == 0)
            {
                w_u8(extension + 0x3f, 1);
                w_u32(object + 0x154, 0);
                selector = r_u8(extension + 0x3e);
                uint32 entry = 0x80099708 + selector * 28;
                uint32 last = r_u16(entry + 0xc) - 1;
                w_u16(object + 0x9e, last);
            }
        }
    }
    uint32 position = r_u16(object + 0x9e) + 1;
    w_u16(object + 0x9e, position);
    uint32 minimum = r_u8(extension + 0x3c);
    if ((sint32)(sint16)position < (sint32)minimum)
    {
        minimum = r_u8(extension + 0x3c);
        w_u16(object + 0x9e, minimum);
    }
    sint32 current = r_s16(object + 0x9e);
    uint32 maximum = r_u8(extension + 0x3d);
    if ((sint32)maximum < current)
    {
        minimum = r_u8(extension + 0x3c);
        w_u16(object + 0x9e, minimum);
        return minimum;
    }
    return 0;
}

uint32 G3Function80066CC8(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80066cc8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 source = object + 0x58;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
    uint32 low_lane = source & 3;
    uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
    uint32 xy = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    uint32 z = (uint32)r_s16(object + 0x5c);
    uint32 destination = frame + 0x10;
    high_address = destination + 3;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, xy >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    low_lane = destination & 3;
    w_masked_u32(destination & 0xfffffffc, xy << (low_lane * 8), (15 << low_lane) & 15);
    w_u16(frame + 0x14, z);
    G3Function800361FC(object, 1000);
    uint32 game = r_u32(0x80097048);
    source = object + 0x58;
    high_address = source + 3;
    high_lane = high_address & 3;
    high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
    low_lane = source & 3;
    low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
    xy = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    z = (uint32)r_s16(object + 0x5c);
    destination = game + 0x67f0;
    high_address = destination + 3;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, xy >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    low_lane = destination & 3;
    w_masked_u32(destination & 0xfffffffc, xy << (low_lane * 8), (15 << low_lane) & 15);
    w_u16(game + 0x67f4, z);
    z = r_u16(game + 0x67f4);
    w_u16(game + 0x67f4, z - 0x20);
    uint32 result = r_u16(object + 0x64) & 0xfff;
    w_u16(game + 0x67f6, result);
    source = frame + 0x10;
    high_address = source + 3;
    high_lane = high_address & 3;
    high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
    low_lane = source & 3;
    low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
    xy = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    z = (uint32)r_s16(frame + 0x14);
    destination = object + 0x58;
    high_address = destination + 3;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, xy >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    low_lane = destination & 3;
    w_masked_u32(destination & 0xfffffffc, xy << (low_lane * 8), (15 << low_lane) & 15);
    w_u16(object + 0x5c, z);
    return result;
}

uint32 G3Function80066D68(uint32 object, uint32 game, uint32 extension, uint32 descriptor, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80066d68, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    if (r_u32(object + 0x154) == 0)
    {
        uint32 player = r_u32(game + 0x50);
        uint32 button = r_u32(extension + 0x38);
        uint32 state = r_u32(player + 0x24);
        if (button != 0)
        {
            uint32 pool = r_u32(0x800a27fc);
            uint32 x = (uint32)r_s16(button + 0x58);
            uint32 z = (uint32)r_s16(button + 0x5c);
            uint32 y = (uint32)r_s16(button + 0x5a);
            w_u32(frame + 0x10, z + 0x40);
            player = r_u32(game + 0x50);
            G3Function800440D0(player, pool, x, y, frame);
        }
        w_u32(frame + 0x10, 1500);
        G3Function8003E518(object, 0x802, 0xffffff9c, 127, frame);
        w_u32(object + 0x154, 1);
        player = r_u32(game + 0x50);
        uint32 flags = r_u32(player + 0x128);
        w_u32(player + 0x128, flags | 0x2000000);
        w_u32(state + 0x1bc, object);
        G3Function80066CC8(object, frame);
        uint32 variant = r_u16(descriptor);
        variant = (uint32)((sint32)(variant << 16) >> 24);
        w_u32(game + 0x69f8, variant);
    }
    uint32 current_game = r_u32(0x80097048);
    uint32 result = r_u32(current_game + 0xe8) & 0x40000;
    if (result != 0)
        return result;
    if (r_u8(extension + 0x3f) != 0)
        return 1;
    w_u8(extension + 0x3f, 1);
    w_u8(extension + 0x3c, 0);
    w_u8(extension + 0x3d, 3);
    w_u32(object + 0x154, 0);
    return 3;
}

uint32 G3Function80067010(uint32 state, uint32 value)
{
    FUNCTION_MARKER(0x80067010, "SLUS_008.06");
    w_u32(state + 8, 1);
    w_u32(state + 4, value);
    return 1;
}

uint32 G3Function80067020(uint32 state, uint32 mode)
{
    FUNCTION_MARKER(0x80067020, "SLUS_008.06");
    uint32 value = r_u32(state + 8);
    if (value == 1)
        return 1;
    if (mode != 1)
        value = r_u32(0x80099d80 + (mode << 3) + 4);
    G3Function80067010(state, value);
    return 0;
}

uint32 G3Function800670C4(uint32 state, uint32 index)
{
    FUNCTION_MARKER(0x800670c4, "SLUS_008.06");
    uint32 header = r_u32(state + 0x20);
    uint32 header_bytes = G3HeaderBytes80067A3C(header);
    uint32 data = r_u32(state + 0x24);
    uint32 data_bytes = G3CardDataBytes80067D28(data);
    return header_bytes + index * data_bytes;
}

uint32 G3Function80067110(uint32 state, uint32 index)
{
    FUNCTION_MARKER(0x80067110, "SLUS_008.06");
    uint32 offset = G3Function800670C4(state, index);
    uint32 base = r_u32(state + 0x28);
    return base + offset;
}

uint32 G3Function8006713C(uint32 state)
{
    FUNCTION_MARKER(0x8006713c, "SLUS_008.06");
    return r_u32(state + 0x28);
}









uint32 G3Function80067A34(uint32 header)
{
    FUNCTION_MARKER(0x80067a34, "SLUS_008.06");
    return header + 4;
}











uint32 G3Function800675F4(uint32 state, uint32 base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800675f4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 io = r_u32(state + 0x1c);
    w_u32(state + 4, 6);
    w_u32(state + 8, 1);
    w_u32(state + 0x28, base);
    G3Function8006802C(io, frame);
    uint32 flags = r_u32(state) | 2;
    w_u32(state, flags);
    return flags;
}

uint32 G3Function80067640(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80067640, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 io = r_u32(state + 0x1c);
    G3Function80068068(io, frame);
    uint32 flags = r_u32(state) & 0xfffffffd;
    w_u32(state, flags);
    return flags;
}


