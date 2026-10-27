#include "psx.h"
#include "xport.h"
#include "game_functions.h"

uint32 G3Function800607C8(uint32 position, uint32 records, uint32 count, uint32 radius, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800607c8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    for (uint32 pass = 0; pass < 3; ++pass)
        G3Function80060684(position, records, count, radius, frame);
    return 0;
}

uint32 G3Function8005DD9C(uint32 first, uint32 second, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005dd9c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    if (r_u32(first + 0x30) == 0)
        return 0;
    if (r_u32(first + 0x34) == 0)
        return 0;
    if (r_u32(second + 0x30) == 0)
        return 0;
    if (r_u32(second + 0x34) == 0)
        return 0;
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 first_extent;
        uint32 second_extent;
        uint32 first_center;
        uint32 second_center;
        if (axis == 0)
        {
            second_extent = (uint32)r_s16(second + 0xc8);
            second_center = r_u32(second + 0xbc);
            first_extent = (uint32)r_s16(first + 0xc8);
            first_center = r_u32(first + 0xbc);
        }
        else
        {
            first_extent = (uint32)r_s16(first + 0xc8 + axis * 2);
            second_extent = (uint32)r_s16(second + 0xc8 + axis * 2);
            second_center = r_u32(second + 0xbc + axis * 4);
            first_center = r_u32(first + 0xbc + axis * 4);
        }
        uint32 upper = second_center + second_extent;
        uint32 first_upper = first_center + first_extent;
        if ((sint32)first_upper < (sint32)upper)
            upper = first_upper;
        uint32 lower = second_center - second_extent;
        uint32 first_lower = first_center - first_extent;
        if ((sint32)lower < (sint32)first_lower)
            lower = first_lower;
        if ((sint32)upper < (sint32)lower)
            return 1;
    }
    G3Function8005CBCC(first, second, 1, frame);
    return G3Function8005CBCC(second, first, 0, frame);
}

void G3Function8005DF0C(uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005df0c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 game = r_u32(0x80097048);
    uint32 player = r_u32(0x8009efc4);
    uint32 primary = r_u32(game + 0x50);
    sint32 mode = r_s16(game + 0x67ee);
    uint32 player_state = r_u32(player + 0x24);
    if (mode != 1)
    {
        for (uint32 index = 0x40; index < 0x80; ++index)
        {
            uint32 object = r_u32(pool + index * 8 + 0x10);
            while (object != 0)
            {
                G3Function8005DD9C(object, primary, frame);
                object = r_u32(object + 4);
            }
        }
        if (r_u32(player_state + 0x1fc) != 0)
        {
            for (uint32 index = 0x40; index < 0x80; ++index)
            {
                uint32 object = r_u32(pool + index * 8 + 0x10);
                while (object != 0)
                {
                    uint32 linked = r_u32(player_state + 0x1fc);
                    if (object != linked)
                        G3Function8005DD9C(object, linked, frame);
                    object = r_u32(object + 4);
                }
            }
        }
    }
    for (uint32 group = 0; group < 16; ++group)
    {
        uint32 slot = r_u32(0x8009ee14 + group * 4);
        uint32 first = r_u32(pool + (slot << 3) + 0x10);
        while (first != 0)
        {
            uint32 second = r_u32(first + 4);
            while (second != 0)
            {
                uint32 flags = r_u32(first + 0x18);
                bool active = (flags & 0x40) != 0;
                if (!active)
                    active = (r_u32(second + 0x18) & 0x40) != 0;
                if (active)
                    G3Function8005DD9C(first, second, frame);
                second = r_u32(second + 4);
            }
            for (uint32 other = group + 1; other < 16; ++other)
            {
                slot = r_u32(0x8009ee14 + other * 4);
                second = r_u32(pool + (slot << 3) + 0x10);
                while (second != 0)
                {
                    uint32 flags = r_u32(first + 0x18);
                    bool active = (flags & 0x40) != 0;
                    if (!active)
                        active = (r_u32(second + 0x18) & 0x40) != 0;
                    if (active)
                        G3Function8005DD9C(first, second, frame);
                    second = r_u32(second + 4);
                }
            }
            first = r_u32(first + 4);
        }
    }
    for (uint32 phase = 0; phase < 3; ++phase)
    {
        uint32 first_table = phase == 0 ? 0x8009ee94 : 0x8009eed4;
        uint32 second_table = phase == 0 ? 0x8009ee54 : (phase == 1 ? 0x8009eef4 : 0x8009ef54);
        uint32 groups = phase == 0 ? 16 : 8;
        for (uint32 group = 0; group < groups; ++group)
        {
            uint32 slot = r_u32(first_table + group * 4);
            uint32 first = r_u32(pool + (slot << 3) + 0x10);
            while (first != 0)
            {
                for (uint32 other = 0; other < 16; ++other)
                {
                    slot = r_u32(second_table + other * 4);
                    uint32 second = r_u32(pool + (slot << 3) + 0x10);
                    while (second != 0)
                    {
                        uint32 flags = r_u32(first + 0x18);
                        bool active = (flags & 0x40) != 0;
                        if (!active)
                            active = (r_u32(second + 0x18) & 0x40) != 0;
                        if (active)
                            G3Function8005DD9C(first, second, frame);
                        second = r_u32(second + 4);
                    }
                }
                first = r_u32(first + 4);
            }
        }
    }
}

uint32 G3Function8006F11C(uint32 request, uint32 normal, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006f11c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    if (r_u32(request) == 0)
        return 0;
    uint32 object = r_u32(request + 8);
    if (object == 0)
        return 0;
    uint32 target = r_u32(object + 0xec);
    if (target == 0)
        return 4;
    uint32 type = r_u16(request);
    w_u16(frame + 0x16, 4);
    w_u16(frame + 0x14, type);
    uint32 value = r_u32(request + 4);
    uint32 player = r_u32(0x8009efc4);
    w_u32(frame + 0x18, value);
    uint32 resource = r_u32(player + 0x1c);
    uint32 models = r_u32(resource + 0xc);
    uint32 model = r_u32(models);
    uint32 segment = r_u32(model + 0x18);
    uint32 collision = r_u32(segment + 0x2c);
    if (collision != 0)
    {
        value = r_u32(collision + 0xc);
        w_u32(frame + 0x1c, value);
    }
    object = r_u32(request + 8);
    w_u32(frame + 0x24, player);
    w_u32(frame + 0x20, object);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        value = r_u16(normal + axis * 2);
        w_u16(frame + 0x38 + axis * 2, 0u - value);
    }
    uint32 position = r_u32(request + 0x14);
    value = r_u16(position);
    w_u16(frame + 0x30, value);
    w_u16(frame + 0x28, value);
    position = r_u32(request + 0x14);
    value = r_u16(position + 2);
    w_u16(frame + 0x32, value);
    w_u16(frame + 0x2a, value);
    position = r_u32(request + 0x14);
    uint32 game = r_u32(0x80097048);
    value = r_u16(position + 4);
    w_u16(frame + 0x34, value);
    w_u16(frame + 0x2c, value);
    object = r_u32(request + 8);
    w_u32(object + 0xb0, frame + 0x10);
    object = r_u32(request + 8);
    return G3DispatchStateEvent(target, object, game, 3, 0, frame);
}

uint32 G3Function8006EFE0(uint32 object, uint32 game, uint32 data, uint32 argument3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006efe0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    uint32 flags = r_u16(data + 0xce);
    w_u16(data + 0xce, flags & 0xfdff);
    uint32 mode = r_u32(object + 0x154);
    uint32 result = flags & 1;
    if ((mode & 0x80001) == 0 || result == 0)
        return result;
    uint32 angle = (uint32)r_s16(data + 0xc8);
    uint32 cosine = G3Function8005B470(angle - 0x400);
    uint32 offset = cosine << 6;
    uint32 coordinate = r_u16(object + 0x58);
    w_u16(frame + 0x40, coordinate + (uint32)((sint32)offset >> 12));
    angle = (uint32)r_s16(data + 0xc8);
    uint32 sine = G3Function8005B590(angle - 0x400);
    offset = sine << 6;
    coordinate = r_u16(object + 0x5a);
    w_u16(frame + 0x42, coordinate + (uint32)((sint32)offset >> 12));
    coordinate = r_u16(object + 0x5c);
    w_u16(frame + 0x44, coordinate);
    uint32 source = frame + 0x40;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
    uint32 low_lane = source & 3;
    uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
    uint32 packed = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    uint32 height = (uint32)r_s16(frame + 0x44);
    uint32 destination = frame + 0x48;
    high_address = destination + 3;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, packed >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
    low_lane = destination & 3;
    w_masked_u32(destination & 0xfffffffc, packed << (low_lane * 8), (15u << low_lane) & 15);
    w_u16(frame + 0x4c, height);
    w_u32(frame + 0x34, 0x1f);
    w_u32(frame + 0x24, frame + 0x40);
    w_u32(frame + 0x28, frame + 0x48);
    uint32 bottom = r_u16(frame + 0x44);
    uint32 top = r_u16(frame + 0x4c);
    w_u32(frame + 0x38, object);
    w_u16(frame + 0x44, bottom - 0x80);
    w_u16(frame + 0x4c, top + 0x80);
    flags = r_u32(object + 0x10);
    game = r_u32(0x80097048);
    w_u32(object + 0x10, flags | 0x40);
    G3Function80061CA4(frame + 0x10, game, frame);
    flags = r_u32(object + 0x10);
    w_u32(object + 0x10, flags & 0xffffffbf);
    result = r_u32(frame + 0x10);
    if (result != 0)
    {
        flags = r_u16(data + 0xce);
        result = flags | 0x210;
        w_u16(data + 0xce, result);
    }
    return result;
}

uint32 G3Function80069A4C(uint32 context, uint32 index, uint32 text, uint32 value, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80069a4c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 current = r_u32(context + 4);
    uint32 result = (current << 4) + 8;
    uint32 descriptor = context + result;
    if (context == 0)
        return result;
    result = (sint32)index < 8 ? 1 : 0;
    if ((sint32)index < 0 || result == 0)
        return result;
    if (text != 0)
    {
        uint32 selected = r_u32(descriptor + 4);
        uint32 scale = r_u32(context + 0xb4);
        w_u32(frame + 0x10, scale);
        uint32 position = r_u32(context + 0xb0);
        uint32 advance = G3Function8006AB04(context, position, text, index == selected ? 1 : 0, frame);
        position = r_u32(context + 0xb0);
        w_u32(context + 0xb0, position + advance);
    }
    w_u32(context + (index << 2) + 0x88, value);
    uint32 count = r_u32(context + 0xa8);
    result = index + 1;
    if ((sint32)index >= (sint32)count)
        w_u32(context + 0xa8, result);
    return result;
}

uint32 G3Function8006AC48(uint32 context, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006ac48, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 game = G3Function8006A5C0(context);
    uint32 level = r_u16(game + 0x68b4);
    uint32 offset = level == 0x1b ? 2 : 100;
    w_u32(frame + 0x14, 240);
    w_u32(frame + 0x1c, 240);
    offset <<= 2;
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x18, 512);
    w_u32(frame + 0x20, 0);
    w_u32(frame + 0x24, 0);
    w_u32(frame + 0x28, 0);
    w_u32(frame + 0x2c, 0);
    uint32 pool = r_u32(game + 0x68);
    w_u32(frame + 0x30, pool);
    uint32 order = r_u32(game + 0x38);
    order += offset;
    w_u32(frame + 0x34, order);
    return G3Function8001E25C(0, 0, 512, 0, 0, 240, 512, 240, 0, 0, 0, 0, pool, order);
}

uint32 G3Function8006DD7C(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006dd7c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 result = 1;
    uint32 contact = r_u32(object + 0xb0);
    uint32 state = r_u32(object + 0x24);
    sint32 type = r_s16(contact + 4);
    uint32 primitive = r_u32(contact + 8);
    bool plain = false;
    if (type == 1)
        plain = r_u8(primitive + 4) == 0;
    if (r_s16(game + 0x67ee) == 1 || r_s16(game + 0x6786) == 6 || (r_u32(object + 0x154) & 0x32000000u) != 0)
        result = 0;
    if (r_s32(object + 0x154) < 0 && plain)
        result = 0;
    if (type == 1 && (!plain || r_u32(object + 0x154) == 0x40000000u))
    {
        uint32 matrix = r_u32(object + 0x30);
        if (r_s16(matrix + 0x32) == type)
            result = 0;
    }
    if (r_u32(object + 0x150) == 1 && plain)
    {
        sint32 shape = r_s16(contact + 6);
        uint32 face = r_u32(contact + 0xc);
        uint32 delta = contact + 0x28;
        bool move = shape == 1 && (r_u32(face) & 0x2000) != 0;
        if (!move && shape == 5)
            move = (r_u16(face) & 0x2000) != 0;
        if (!move)
        {
            result = 0;
            if (shape == 2)
                move = (r_u16(face + 6) & 0xf) != 8;
        }
        if (move)
        {
            uint32 position = r_u16(object + 0x58);
            uint32 offset = r_u16(delta);
            w_u16(object + 0x58, position + offset);
            position = r_u16(object + 0x5a);
            offset = r_u16(delta + 2);
            w_u16(object + 0x5a, position + offset);
            position = r_u16(object + 0x5c);
            offset = r_u16(delta + 4);
            w_u16(object + 0x5c, position + offset);
            G3Function8005F3A8(object, delta);
            result = 0;
        }
    }
    if (r_s32(object + 0x154) >= 0 || !plain)
        return result;
    uint32 linked = r_u32(state + 0x1fc);
    if (linked == 0)
        return result;
    uint32 other = r_u32(contact + 0x14);
    if (other == linked)
        return result;
    if (other != 0 && other == r_u32(state + 0x17c))
        return result;
    w_u32(linked + 0xb0, contact);
    linked = r_u32(state + 0x1fc);
    uint32 target = r_u32(linked + 0xec);
    if (target != 0)
        G3DispatchStateEvent(target, linked, game, 0x12, contact, frame);
    return result;
}

uint32 G3Function80073B9C(uint32 game, uint32 magnitude, uint32 face, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80073b9c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 player = r_u32(0x8009efc4);
    uint32 flags = r_u32(player + 0x154);
    uint32 state = r_u32(player + 0x24);
    if (flags == 0x02000000u)
        return player;
    G3Function8006F468(game, frame);
    w_u16(state + 0xd0, 0);
    w_u16(state + 0xd2, 0);
    w_u16(state + 0xd4, 0);
    w_u16(state + 0xd8, 0);
    w_u16(state + 0xda, 0);
    w_u16(state + 0xdc, 0);
    if (face != 0)
    {
        uint32 level = r_u32(game + 0x48);
        uint32 terrain = r_u32(level);
        uint32 index = (uint32)r_s16(face + 8);
        uint32 normals = r_u32(terrain + 0x40);
        G3Function8005F304(index, normals, frame + 0x10, frame);
    }
    else
    {
        w_u16(frame + 0x10, 0);
        w_u16(frame + 0x12, 0);
        w_u16(frame + 0x14, 0x1000);
    }
    uint32 product = (uint32)r_s16(frame + 0x10) * magnitude;
    player = r_u32(0x8009efc4);
    w_u32(player + 0x104, (uint32)((sint32)product >> 12));
    product = (uint32)r_s16(frame + 0x12) * magnitude;
    w_u32(player + 0x108, (uint32)((sint32)product >> 12));
    product = (uint32)r_s16(frame + 0x14) * magnitude;
    uint32 position = r_u16(player + 0x5c);
    w_u32(player + 0x154, 0x00200000u);
    w_u32(player + 0x10c, (uint32)((sint32)product >> 12));
    uint32 displacement = r_u16(player + 0x10c);
    w_u16(player + 0x6e, 0x2e);
    w_u16(player + 0x5c, position + displacement);
    uint32 index = r_u16(state + 0x20);
    uint32 table = r_u32(state + 0x34);
    index -= 1;
    w_u16(state + 0xe0, index);
    uint32 offset = (uint32)((sint32)(index << 16) >> 15);
    uint32 result = r_u16(table + offset) - 1u;
    w_u16(state + 0xe2, result);
    return result;
}

uint32 G3Function8007408C(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007408c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    return G3Function80073E44(object, game, 1, 1, frame);
}

uint32 G3Function80073CE8(uint32 game, uint32 magnitude, uint32 face, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80073ce8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 player = r_u32(0x8009efc4);
    uint32 result = G3Function8007408C(player, game, frame);
    if (result != 0)
        result = G3Function80073B9C(game, magnitude, face, frame);
    return result;
}

void G3Function8006E1B0(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006e1b0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 contact = r_u32(object + 0xb0);
    uint32 state = r_u32(object + 0x24);
    uint32 matrix = r_u32(object + 0x30);
    sint32 type = r_s16(contact + 4);
    sint32 shape = r_s16(contact + 6);
    uint32 primitive = r_u32(contact + 8);
    w_u32(frame + 0x10, primitive);
    uint32 face = r_u32(contact + 0xc);
    uint32 other = r_u32(contact + 0x14);
    if (shape == 0)
        return;
    if (other != 0)
    {
        bool notify = type == 4;
        if (!notify)
        {
            uint32 descriptor = r_u32(other + 0x1c);
            notify = r_s32(descriptor + 0x30) < 0;
        }
        if (notify)
        {
            uint32 flags = r_u32(other + 0x10) | 0x80000000u;
            w_u32(other + 0x10, flags);
            if ((flags & 0x04000000u) == 0)
            {
                uint32 target = r_u32(other + 0xec);
                if (target != 0)
                    G3DispatchStateEvent(target, other, game, 2010, object, frame);
            }
        }
    }
    if (type == 4 && shape == 3)
    {
        uint32 current_face = r_u32(contact + 0xc);
        uint32 flags = r_u16(current_face + 6);
        uint32 level = r_u32(game + 0x48);
        uint32 materials = r_u32(level + 0xf0);
        uint32 sequence = r_u32(materials + ((flags & 0x1e0) >> 1) + 0xc);
        if (sequence != 0)
            G3Function80018DE0(object, sequence + 4, frame);
    }
    if (G3Function8006DD7C(object, game, frame) == 0)
        return;
    if (shape == 1)
    {
        bool move = type == 4;
        if (!move)
        {
            primitive = r_u32(frame + 0x10);
            if (r_u8(primitive + 4) != 9)
                move = (r_u32(face) & 0x6000) == 0x2000;
        }
        if (move)
        {
            uint32 position = r_u16(object + 0x58);
            uint32 delta = r_u16(contact + 0x28);
            w_u16(object + 0x58, position + delta);
            position = r_u16(object + 0x5a);
            delta = r_u16(contact + 0x2a);
            w_u16(object + 0x5a, position + delta);
            position = r_u16(object + 0x5c);
            delta = r_u16(contact + 0x2c);
            w_u16(object + 0x5c, position + delta);
            G3Function8005F3A8(object, contact + 0x28);
            if (type == shape)
            {
                uint32 touched = r_u32(contact + 0x14);
                uint32 platform = r_u32(state + 0x1e8);
                uint32 flags = r_u32(state + 0xb8);
                w_u32(state + 0xb8, flags | (touched == platform ? 0x10 : 4));
            }
        }
        if (type == 4)
        {
            w_u16(state + 0xce, r_u16(state + 0xce) | 1);
            return;
        }
        G3Function8006E6F4(object, contact, frame);
        return;
    }
    uint32 work = r_u32(0x800a0284);
    G3Function800388C8(contact, work + 0x14, game, frame);
    if (type != 4)
    {
        if (type != 1)
            return;
        primitive = r_u32(frame + 0x10);
        if (r_u8(primitive + 4) != 0)
        {
            if (shape == 3)
                return;
            bool process = shape == type && (r_u32(face) & 0x2000) != 0;
            if (!process && shape == 5)
                process = (r_u16(face) & 0x2000) != 0;
            if (!process && shape == 2)
                process = (r_u16(face + 6) & 0xf) != 8;
            if (process)
                G3Function8006E6F4(object, contact, frame);
            return;
        }
    }
    bool move = (uint32)(shape - 2) < 2;
    if (move)
    {
        work = r_u32(0x800a0284);
        w_u32(work + 0xc, face);
    }
    else if (shape == 5)
        move = (r_u16(face) & 0x2000) != 0;
    if (move)
    {
        if (type == 4)
        {
            bool mark = true;
            if (shape != 5 && (r_u16(face + 6) & 0xf) == 2)
            {
                w_u16(state + 0xce, r_u16(state + 0xce) | 0x41);
                mark = (r_u16(face + 6) & 0xf) != 2;
            }
            if (mark)
                w_u16(state + 0xce, r_u16(state + 0xce) | 1);
        }
        else if (type == 1)
        {
            uint32 mode = r_u32(object + 0x154);
            if ((mode & 0x4046a) != 0)
            {
                work = r_u32(0x800a0284);
                uint32 axis = (uint32)r_s16(matrix + 2);
                uint32 normal = (uint32)r_s16(work + 0x14);
                uint32 dot = axis * normal;
                axis = (uint32)r_s16(matrix + 8);
                normal = (uint32)r_s16(work + 0x16);
                dot += axis * normal;
                axis = (uint32)r_s16(matrix + 0xe);
                normal = (uint32)r_s16(work + 0x18);
                dot += axis * normal;
                if ((sint32)dot > 0x00f00000 && ((shape == 5 && (mode & 0x402) != 0) || (uint32)(shape - 2) < 2))
                    w_u16(state + 0xce, r_u16(state + 0xce) | 2);
            }
            w_u16(state + 0xce, r_u16(state + 0xce) | 4);
            if (shape == 2 || shape == 5)
            {
                uint32 touched = r_u32(contact + 0x14);
                uint32 platform = r_u32(state + 0x1e8);
                uint32 flags = r_u32(state + 0xb8);
                w_u32(state + 0xb8, flags | (touched == platform ? 0x10 : 4));
            }
            else if (shape == 3)
                w_u32(state + 0xb8, r_u32(state + 0xb8) | 4);
        }
        uint32 position = r_u16(object + 0x58);
        uint32 delta = r_u16(contact + 0x28);
        w_u16(object + 0x58, position + delta);
        position = r_u16(object + 0x5a);
        delta = r_u16(contact + 0x2a);
        w_u16(object + 0x5a, position + delta);
        position = r_u16(object + 0x5c);
        delta = r_u16(contact + 0x2c);
        w_u16(object + 0x5c, position + delta);
        G3Function8005F3A8(object, contact + 0x28);
    }
    if (r_u32(object + 0x150) != 0 || shape == 5)
        return;
    uint32 flags = r_u16(face + 6);
    if ((flags & 1) != 0)
        return;
    uint32 current_game = r_u32(0x80097048);
    uint32 level = r_u32(current_game + 0x48);
    uint32 materials = r_u32(level + 0xf0);
    if ((r_u32(materials + ((flags & 0x1e0) >> 1)) & 1) != 0)
    {
        uint32 magnitude = (uint32)r_s16(state + 0x10);
        G3Function80073CE8(game, magnitude, face, frame);
    }
}

uint32 G3Function8006F468(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006f468, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 player = r_u32(0x8009efc4);
    uint32 result = r_u32(player + 0x150);
    uint32 state = r_u32(player + 0x24);
    if (result == 1)
    {
        uint32 flags = r_u32(player + 0x128);
        if ((flags & 0x20000) == 0 && r_u32(player + 0x154) != 0x02000000u)
            G3Function800726FC(player, game, state, frame);
        result = r_u32(player + 0x150);
    }
    if (result == 0)
    {
        uint32 flags = r_u32(player + 0x154);
        result = 0x20000;
        if ((flags & 0x76400000u) == 0)
        {
            w_u32(player + 0x154, result);
            result = 0x13;
            w_u16(player + 0x6e, result);
            w_u16(player + 0x5e, 0);
            w_u16(state + 0xc4, 0);
        }
    }
    return result;
}

uint32 G3Function8005ACE4(uint32 output, uint32 first, uint32 second, uint32 magnitude, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005ace4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    sint32 length = (sint16)(magnitude & 0xffff);
    length /= 5;
    uint32 right = r_u16(second);
    uint32 left = r_u16(first);
    w_u16(frame + 0x10, right - left);
    right = r_u16(second + 2);
    left = r_u16(first + 2);
    w_u16(frame + 0x14, 0);
    w_u16(frame + 0x12, right - left);
    G3Function8002F9A8(frame + 0x18, (uint32)(sint32)(sint16)length, frame + 0x10, frame);
    w_u32(output + 4, (uint32)r_s16(frame + 0x18));
    w_u32(output + 8, (uint32)r_s16(frame + 0x1a));
    uint32 x = r_u32(output + 4);
    uint32 z = (uint32)r_s16(frame + 0x1c);
    w_u32(output + 4, x << 16);
    uint32 y = r_u32(output + 8);
    w_u32(output + 0xc, z);
    w_u32(output + 0xc, z << 16);
    uint32 result = y << 16;
    w_u32(output + 8, result);
    return result;
}

uint32 G3Function80044320(uint32 object, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80044320, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 matrix = r_u32(object + 0x30) + 0x4c0;
    uint32 z = r_u32(matrix + 0x1c);
    w_u32(frame + 0x10, z);
    uint32 x = r_u32(matrix + 0x14);
    uint32 y = r_u32(matrix + 0x18);
    return G3Function800440D0(object, pool, x, y, frame);
}

uint32 G3Function8006E6F4(uint32 object, uint32 contact, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006e6f4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x58;
    uint32 player = r_u32(0x8009efc4);
    uint32 primitive = r_u32(contact + 8);
    uint32 other = r_u32(contact + 0x14);
    uint32 kind = r_u8(primitive + 4);
    uint32 state = r_u32(player + 0x24);
    if (kind != 9)
        return 9;
    if (other != 0)
    {
        G3Function8003C548(contact, frame + 0x30, frame + 0x31);
        uint32 first_kind = r_u8(frame + 0x30);
        if (first_kind - 6u < 4)
        {
            uint32 second_kind = r_u8(frame + 0x31);
            if (second_kind == kind || second_kind - 3u < 3)
            {
                G3Function8005ACE4(frame + 0x18, object + 0x58, other + 0x58, 250, frame);
                w_u32(frame + 0x18, 1);
                w_u16(frame + 0x28, 0);
                w_u16(frame + 0x2a, 0);
                w_u32(frame + 0x2c, contact);
                if ((r_u32(object + 0x134) & 2) != 0)
                    w_u16(frame + 0x2a, 1);
                if ((r_u32(object + 0x128) & 8) == 0)
                {
                    uint32 linked = r_u32(state + 0x1fc);
                    if (linked == 0 || r_u32(contact + 0x14) != linked)
                    {
                        uint32 descriptor = r_u32(other + 0x1c);
                        if ((r_u32(descriptor + 0x30) & 0x1000) != 0)
                        {
                            uint32 current_other = r_u32(contact + 0x14);
                            uint32 flags = r_u32(current_other + 0x14);
                            descriptor = r_u32(current_other + 0x1c);
                            w_u32(current_other + 0x14, flags | 0x200);
                            if (r_u32(descriptor + 0x10) != 0)
                                w_u32(object + 0x128, r_u32(object + 0x128) | 0x10000);
                        }
                        else
                            G3Function8003F128(0x810, 0);
                        w_u32(frame + 0x10, 6);
                        w_u32(frame + 0x14, 0);
                        G3Function8008194C(0, 1, 0xffffffffu, 0x266, frame);
                    }
                }
                uint32 mode = r_u32(object + 0x154);
                switch (mode)
                {
                case 0x10:
                case 0x2000:
                    w_u16(frame + 0x28, 0);
                    break;
                case 0x20:
                case 0x80:
                    w_u16(frame + 0x28, 1);
                    break;
                case 0x200:
                    w_u16(frame + 0x28, 1);
                    w_u32(other + 0x14, r_u32(other + 0x14) | 2);
                    break;
                case 0x40000:
                    w_u16(frame + 0x28, 2);
                    w_u32(other + 0x14, r_u32(other + 0x14) | 0x01000000u);
                    break;
                }
                sint32 response = r_s16(frame + 0x28);
                if (response == 1)
                    w_u32(other + 0x14, r_u32(other + 0x14) | 2);
                else if (response == 0)
                    w_u32(other + 0x14, r_u32(other + 0x14) | 1);
                else if (response == 2)
                    w_u32(other + 0x14, r_u32(other + 0x14) | 0x01000000u);
                uint32 target = r_u32(other + 0xec);
                if (target != 0)
                {
                    uint32 game = r_u32(0x80097048);
                    G3DispatchStateEvent(target, other, game, 8, frame + 0x18, frame);
                }
            }
        }
    }
    uint32 result = r_u32(object + 0x128) & 8;
    if (result != 0)
        return result;
    uint32 linked = r_u32(state + 0x1fc);
    if (linked != 0)
    {
        result = r_u32(contact + 0x14);
        if (result == linked)
            return result;
    }
    sint32 shape = r_s16(contact + 6);
    if (shape == 1 || shape == 5)
    {
        uint32 face = r_u32(contact + 0xc);
        if ((r_u16(face) & 0x2000) != 0)
        {
            uint32 pool = r_u32(0x800a27fc);
            G3Function80044320(object, pool, frame);
        }
    }
    result = r_u32(object + 0x128) | 8;
    w_u32(object + 0x128, result);
    return result;
}

void G3Function8004D8A0(uint32, uint32 mode)
{
    FUNCTION_MARKER(0x8004d8a0, "SLUS_008.06");
    w_u16(0x80096b00, mode);
}

uint32 G3Function80073E44(uint32 object, uint32 game, uint32 mode, uint32 force, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80073e44, "SLUS_008.06");
    uint32 player = r_u32(0x8009efc4);
    uint32 frame = guest_stack - 0x80;
    uint32 flags = r_u32(player + 0x134);
    uint32 state = r_u32(player + 0x24);
    uint32 enabled = 1;
    do
    {
        if ((flags & 0x20) != 0)
            break;
        if (r_u32(state + 0x1fc) != 0)
        {
            w_u32(frame + 0x2c, frame + 0x30);
            w_u16(frame + 0x34, 0);
            w_u16(frame + 0x36, 0);
            uint32 linked = r_u32(state + 0x1fc);
            uint32 target = r_u32(linked + 0xec);
            if (target != 0)
                G3DispatchStateEvent(target, linked, game, 8, frame + 0x18, frame);
            break;
        }
        if (r_u32(player + 0x150) == 6)
        {
            if (r_u32(state + 0x174) != 0)
                enabled = 0;
            else
            {
                uint32 camera = r_u32(game + 0x4c);
                G3Function8004D8A0(camera, 0);
                camera = r_u32(game + 0x4c);
                G3Function80051FE4(camera, frame);
            }
        }
        player = r_u32(0x8009efc4);
        uint32 activity = r_u32(player + 0x150);
        if (activity != 0 && activity != 2 && activity != 3 && activity != 1 && activity != 6)
            break;
        if (force == 0)
        {
            player = r_u32(0x8009efc4);
            if ((r_u32(player + 0x154) & 0x52642030u) != 0)
                break;
        }
        if (r_s16(state + 0x1d6) != 0)
            break;
        player = r_u32(0x8009efc4);
        if ((r_u32(player + 0x154) & 0x44000000u) != 0)
            break;
        if (r_s16(state + 0x120) != 0)
            break;
        if (r_u32(state + 0x1b8) != 0)
            break;
        w_u32(frame + 0x10, enabled);
        G3Function80073854(object, player, state, mode, frame);
        G3Function80073D44(game, frame);
        w_u16(state + 0x120, 60);
        G3Function800358C0(8, 5, frame);
        w_u32(frame + 0x10, 0x12);
        w_u32(frame + 0x14, 0);
        G3Function8008194C(0, 1, 0xffffffffu, 0x1000, frame);
    } while (false);
    return (uint32)r_s16(game + 0x681c);
}

uint32 G3Function8006EB20(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006eb20, "SLUS_008.06");
    uint32 frame = guest_stack - 0x98;
    uint32 material = 0;
    uint32 player = r_u32(0x8009efc4);
    uint32 state = r_u32(player + 0x24);
    if (r_s16(game + 0x67ee) == 1)
        return 1;
    uint32 result = r_u32(player + 0x154) & 0x32000000u;
    if (result != 0)
        return result;
    if ((r_u32(player + 0x18) & 1) == 0)
    {
        uint32 flags = r_u32(player + 0x10);
        uint32 face = r_u32(player + 0xf8);
        flags &= 0xbfffffffu;
        w_u32(player + 0x10, flags);
        bool update = false;
        if (face != 0)
        {
            uint32 attributes = r_u16(face + 6);
            uint32 level = r_u32(game + 0x48);
            uint32 materials = r_u32(level + 0xf0);
            material = materials + ((attributes & 0x1e0) >> 1);
            if ((r_u32(material) & 0x10) != 0)
                w_u32(player + 0x10, flags | 0x40000000u);
            update = (r_u32(material) & 0x80) == 0;
        }
        else
        {
            face = r_u32(state + 0x214);
            if (face != 0)
            {
                uint32 attributes = r_u16(face + 6);
                uint32 level = r_u32(game + 0x48);
                uint32 materials = r_u32(level + 0xf0);
                material = materials + ((attributes & 0x1e0) >> 1);
                update = (r_u32(material) & 0x90) == 0;
            }
        }
        if (update)
            G3Function800734C8(player, frame);
    }
    uint32 activity = r_u32(player + 0x150);
    if (activity - 2u < 2)
        return 1;
    if ((r_u32(player + 0x128) & 0x1000) != 0)
        return 1;
    if (activity == 1)
    {
        uint32 flags = r_u32(player + 0x10);
        uint32 input = (flags & 0x100) != 0 ? 0x800a0d28 : game + 0x70;
        G3Function800727A4(player, game, 0x80099e7c, frame);
        G3Function8007246C(player, game, state, input, frame);
    }
    else
    {
        if ((r_u32(player + 0x154) & 0x7000) != 0)
        {
            sint32 height = r_s16(player + 0x5c);
            sint32 floor_height = r_s16(player + 0xe0);
            if (height + 0x20 < floor_height && height + 0x100 >= floor_height)
            {
                w_u16(frame + 0x70, r_u16(player + 0x58));
                w_u16(frame + 0x72, r_u16(player + 0x5a));
                w_u16(frame + 0x74, r_u16(player + 0x5c));
                uint32 source = frame + 0x70;
                uint32 high_address = source + 3;
                uint32 high_lane = high_address & 3;
                uint32 high = r_masked_u32(high_address & 0xfffffffcu, (1u << (high_lane + 1)) - 1);
                uint32 low_lane = source & 3;
                uint32 low = r_masked_u32(source & 0xfffffffcu, (15u << low_lane) & 15);
                uint32 packed = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
                uint32 z = (uint32)r_s16(frame + 0x74);
                uint32 destination = frame + 0x78;
                high_address = destination + 3;
                high_lane = high_address & 3;
                w_masked_u32(high_address & 0xfffffffcu, packed >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
                low_lane = destination & 3;
                w_masked_u32(destination & 0xfffffffcu, packed << (low_lane * 8), (15u << low_lane) & 15);
                w_u16(frame + 0x7c, z);
                w_u32(frame + 0x64, 0x1f);
                w_u32(frame + 0x54, frame + 0x70);
                w_u32(frame + 0x58, frame + 0x78);
                uint32 bottom = r_u16(frame + 0x74);
                uint32 top = r_u16(frame + 0x7c);
                w_u32(frame + 0x68, player);
                w_u16(frame + 0x74, bottom - 0x100);
                w_u16(frame + 0x7c, top + 0x80);
                uint32 flags = r_u32(player + 0x10);
                uint32 current_game = r_u32(0x80097048);
                w_u32(player + 0x10, flags | 0x40);
                G3Function80061CA4(frame + 0x40, current_game, frame);
                w_u32(player + 0x10, r_u32(player + 0x10) & 0xffffffbfu);
                w_u16(player + 0xe0, r_u16(frame + 0x74));
            }
        }
        if (r_s32(player + 0x10c) <= 0)
        {
            sint32 height = r_s16(player + 0x5c);
            sint32 floor_height = r_s16(player + 0xe0);
            if (height - 0x40 < floor_height && floor_height < height + 0x80)
            {
                uint32 request = state + 0x13c;
                w_u16(frame + 0x14, 4);
                uint32 value = r_u16(request);
                w_u32(frame + 0x18, 0);
                w_u16(frame + 0x16, value);
                value = r_u32(request + 4);
                w_u32(frame + 0x20, player);
                w_u32(frame + 0x1c, value);
                w_u32(frame + 0x24, r_u32(request + 8));
                value = r_u16(player + 0xdc);
                w_u16(frame + 0x30, value);
                w_u16(frame + 0x28, value);
                value = r_u16(player + 0xde);
                w_u16(frame + 0x32, value);
                w_u16(frame + 0x2a, value);
                value = r_u16(player + 0xe0);
                w_u16(frame + 0x38, 0);
                w_u16(frame + 0x3a, 0);
                w_u16(frame + 0x32, value);
                w_u16(frame + 0x2a, value);
                uint32 target_height = r_u16(player + 0xe0);
                uint32 current_height = r_u16(player + 0x5c);
                w_u16(frame + 0x3c, target_height - current_height);
                w_u32(player + 0xb0, frame + 0x10);
                G3Function8006E1B0(player, game, frame);
                w_u32(request + 0x14, player + 0xdc);
                G3Function8006F11C(request, frame + 0x38, frame);
            }
        }
        bool supported = false;
        if (r_u32(player + 0xf8) != 0)
        {
            if ((r_u32(player + 0x18) & 1) == 0)
            {
                uint32 sequence = r_u32(material + 0xc);
                if (sequence != 0)
                    G3Function80018DE0(player, sequence + 4, frame);
            }
            if (r_u32(player + 0xf8) != 0)
            {
                bool adjust = false;
                uint32 previous = 0;
                if ((r_u32(player + 0x18) & 1) == 0)
                {
                    sint32 previous_signed = r_s16(state + 0x258);
                    previous = r_u16(state + 0x258);
                    if (previous_signed != 5 && r_s32(player + 0x10c) <= 0)
                        adjust = (r_u32(material) & 0x40) != 0;
                }
                if (adjust)
                {
                    uint32 height = r_u16(player + 0x100);
                    uint32 difference = (uint32)(sint32)(sint16)(height - previous);
                    G3Function8006EA4C(player, game, state, difference, frame);
                    supported = true;
                }
                else
                    supported = r_u32(player + 0xf8) != 0;
            }
        }
        if (!supported && (r_u16(state + 0xce) & 1) != 0)
            w_u16(state + 0x258, 5);
    }
    uint32 saved_face = r_u32(player + 0xf8);
    uint32 saved_words[2];
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 source = player + 0xfc + word * 4;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffcu, (1u << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffcu, (15u << low_lane) & 15);
        saved_words[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 destination = frame + 0x40 + word * 4;
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffcu, saved_words[word] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        uint32 low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffcu, saved_words[word] << (low_lane * 8), (15u << low_lane) & 15);
    }
    G3Function8006EFE0(player, game, state, 0, frame);
    w_u32(player + 0xf8, saved_face);
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 source = frame + 0x40 + word * 4;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffcu, (1u << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffcu, (15u << low_lane) & 15);
        saved_words[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 destination = player + 0xfc + word * 4;
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffcu, saved_words[word] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        uint32 low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffcu, saved_words[word] << (low_lane * 8), (15u << low_lane) & 15);
    }
    uint32 flags = r_u16(state + 0xce);
    result = flags & 0xff7f;
    if ((flags & 1) != 0)
    {
        uint32 work = r_u32(0x800a0284);
        w_u16(state + 0xce, flags | 0x80);
        result = r_u32(work + 0xc);
        w_u32(player + 0xa8, result);
    }
    else
    {
        w_u16(state + 0xce, result);
        w_u32(player + 0xa8, 0);
    }
    return result;
}

uint32 G3Function8006EA4C(uint32 object, uint32, uint32 state, uint32 height, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006ea4c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    if ((r_u16(state + 0xce) & 1) != 0)
    {
        uint32 position = (uint32)r_s16(object + 0x5c);
        uint32 velocity = r_u32(object + 0x10c);
        if ((sint32)(position + velocity) >= (sint32)(sint16)height)
            return 0;
    }
    uint32 position = r_u16(object + 0x5c);
    uint32 difference = height - position;
    w_u16(frame + 0x14, difference);
    uint32 velocity = r_u32(object + 0x10c);
    sint32 delta = (sint32)(sint16)difference;
    if (delta < (sint32)velocity)
    {
        uint32 result = 0u - (uint32)delta;
        if (delta >= 0)
            w_u32(object + 0x10c, result);
        return result;
    }
    uint32 work = r_u32(0x800a0284);
    w_u16(work + 0x14, 0);
    w_u16(work + 0x16, 0);
    w_u16(work + 0x18, 0x1000);
    w_u16(state + 0xce, r_u16(state + 0xce) | 0x11);
    w_u16(frame + 0x12, 0);
    w_u16(frame + 0x10, 0);
    position = r_u16(object + 0x5c);
    difference = r_u16(frame + 0x14);
    w_u16(object + 0x5c, position + difference);
    return G3Function8005F3A8(object, frame + 0x10);
}

uint32 G3Function800734C8(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800734c8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 result = r_u32(object + 0x18) & 1;
    if (result != 0)
        return result;
    uint32 parameters = 0;
    uint32 face = r_u32(object + 0xf8);
    if (face != 0)
    {
        uint32 game = r_u32(0x80097048);
        uint32 flags = r_u16(face + 6);
        uint32 level = r_u32(game + 0x48);
        uint32 materials = r_u32(level + 0xf0);
        uint32 material = materials + ((flags & 0x1e0) >> 1);
        if ((r_u32(material) & 0x10) != 0)
            parameters = r_u32(material + 8);
    }
    if (parameters == 0)
        parameters = r_u32(0x80098784);
    if (parameters != 0)
        return G3Function80072FB0(object, parameters, frame);
    return 0x800a0000u;
}

uint32 G3Function800727A4(uint32 object, uint32 game, uint32 animations, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800727a4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 state = r_u32(object + 0x24);
    uint32 source = 0x800a0294;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffcu, (1u << (high_lane + 1)) - 1);
    uint32 low_lane = source & 3;
    uint32 low = r_masked_u32(source & 0xfffffffcu, (15u << low_lane) & 15);
    uint32 packed = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    uint32 third = (uint32)r_s16(source + 4);
    uint32 destination = frame + 0x10;
    high_address = destination + 3;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffcu, packed >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
    low_lane = destination & 3;
    w_masked_u32(destination & 0xfffffffcu, packed << (low_lane * 8), (15u << low_lane) & 15);
    w_u16(frame + 0x14, third);
    for (uint32 attempt = 0; attempt < 2; ++attempt)
    {
        w_u16(state + 0xce, r_u16(state + 0xce) & 0xff80);
        G3Function80070B54(game, animations, frame);
        uint32 selector = frame + 0x10;
        do
        {
            uint32 index = (uint32)r_s16(selector);
            uint32 face = r_u32(0x80099e7c + index * 0x34 + 0xc);
            bool valid = false;
            if (face != 0)
            {
                uint32 attributes = r_u16(face + 6);
                bool inspect = true;
                if ((attributes & 1) == 0)
                {
                    uint32 current_game = r_u32(0x80097048);
                    uint32 level = r_u32(current_game + 0x48);
                    uint32 materials = r_u32(level + 0xf0);
                    inspect = (r_u32(materials + ((attributes & 0x1e0) >> 1)) & 1) == 0;
                    if (inspect)
                        index = (uint32)r_s16(selector);
                }
                if (inspect)
                {
                    face = r_u32(0x80099e7c + index * 0x34 + 0xc);
                    uint32 shape = r_u16(face + 6) & 0xf;
                    valid = shape == 4 || shape != 2;
                }
            }
            if (!valid)
                w_u16(state + 0xce, r_u16(state + 0xce) | 0x420);
            selector += 2;
        } while ((sint32)selector < (sint32)(frame + 0x16));
        if ((r_u16(state + 0xce) & 0x20) == 0)
            break;
        uint32 x = r_u16(object + 0x70);
        uint32 y = r_u16(object + 0x72);
        uint32 z = r_u16(object + 0x74);
        w_u16(object + 0x58, x);
        w_u16(object + 0x5a, y);
        w_u16(object + 0x5c, z);
        uint32 rotation = r_u16(state + 0x1c0);
        uint32 work = r_u32(0x800a0284);
        w_u16(object + 0x60, rotation);
        w_u16(object + 0x62, r_u16(state + 0x1c2));
        w_u16(object + 0x64, r_u16(state + 0x1c4));
        w_u32(work + 0xc, r_u32(state + 0x1dc));
        if (r_u32(state + 0x1dc) != 0)
        {
            w_u16(work + 0x14, r_u16(state + 0x1e0));
            w_u16(work + 0x16, r_u16(state + 0x1e2));
            w_u16(work + 0x18, r_u16(state + 0x1e4));
        }
        uint32 end = r_u16(state + 0x20);
        uint32 key = r_u16(state + 0xe4);
        w_u16(state + 0xe0, end - 1);
        w_u16(state + 0xe2, key - 1);
        if (r_u16(object + 0x6e) == 0xc)
        {
            w_u16(object + 0x5e, 0);
            w_u16(object + 0x6e, 0xe);
        }
    }
    uint32 work = r_u32(0x800a0284);
    uint32 result = r_u32(work + 0xc);
    w_u32(state + 0x1dc, result);
    if (result != 0)
    {
        w_u16(state + 0x1e0, r_u16(work + 0x14));
        w_u16(state + 0x1e2, r_u16(work + 0x16));
        result = r_u16(work + 0x18);
        w_u16(state + 0x1e4, result);
    }
    return result;
}

uint32 G3Function8007246C(uint32 object, uint32 game, uint32 state, uint32 input, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007246c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 flags = r_u32(object + 0x128);
    uint32 result = flags & 0x10000000u;
    if (result != 0)
        return result;
    if ((r_u32(input + 4) & 0x80) == 0 && (flags & 0x100) == 0)
    {
        uint32 work = r_u32(0x800a0284);
        result = r_u32(work + 0xc);
        if (result != 0)
            return result;
    }
    uint32 work = r_u32(0x800a0284);
    uint32 face = r_u32(work + 0xc);
    uint32 found = G3Function8007225C(frame + 0x10);
    flags = r_u32(object + 0x128);
    if ((flags & 0x100) != 0)
        w_u32(object + 0x128, flags & 0xfffffeffu);
    w_u32(state + 0x1dc, 0);
    G3Function800726FC(object, game, state, frame);
    if (found == 0)
    {
        w_u32(object + 0x154, 8);
        return 8;
    }
    bool boost = false;
    if (face != 0)
    {
        uint32 attributes = r_u16(face + 6);
        uint32 level = r_u32(game + 0x48);
        uint32 materials = r_u32(level + 0xf0);
        boost = (r_u32(materials + ((attributes & 0x1e0) >> 1)) & 0x20) != 0;
    }
    if (boost)
        w_u32(object + 0x10c, 100);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 value = r_u16(frame + 0x10 + axis * 2);
        sint32 signed_value = (sint32)(sint16)value;
        uint32 delta = boost ? (uint32)(signed_value >> 3) : (uint32)((signed_value >> 4) + (signed_value >> 6));
        w_u16(frame + 0x18 + axis * 2, delta);
    }
    if (boost)
    {
        uint32 end = r_u16(state + 0x20);
        uint32 key = r_u16(state + 0xe4);
        w_u16(state + 0xe0, end - 1);
        w_u16(state + 0xe2, key - 1);
    }
    uint32 position = r_u16(object + 0x58);
    uint32 delta = r_u16(frame + 0x18);
    w_u16(object + 0x58, position + delta);
    position = r_u16(object + 0x5a);
    delta = r_u16(frame + 0x1a);
    uint32 y = position + delta;
    uint32 z = r_u16(object + 0x5c);
    w_u16(object + 0x5a, y);
    delta = r_u16(frame + 0x1c);
    z += delta;
    w_u16(object + 0x5c, z);
    if ((sint16)r_u16(frame + 0x14) < -0xfc2)
        w_u16(object + 0x5c, z - 0x40);
    G3Function8006F468(game, frame);
    uint32 x_velocity = (uint32)((sint32)(sint16)r_u16(frame + 0x18) >> 4);
    w_u16(state + 0xd0, x_velocity);
    uint32 y_delta = r_u16(frame + 0x1a);
    uint32 x_acceleration = 0u - (uint32)((sint32)(sint16)x_velocity >> 3);
    uint32 y_velocity = (uint32)((sint32)(sint16)y_delta >> 4);
    w_u16(state + 0xd2, y_velocity);
    uint32 z_delta = r_u16(frame + 0x1c);
    w_u16(state + 0xd8, x_acceleration);
    y_velocity = r_u16(state + 0xd2);
    w_u16(state + 0xdc, 0);
    uint32 z_velocity = (uint32)((sint32)(sint16)z_delta >> 4);
    result = 0u - (uint32)((sint32)(sint16)y_velocity >> 3);
    w_u16(state + 0xd4, z_velocity);
    w_u16(state + 0xda, result);
    return result;
}

uint32 G3Function8007225C(uint32 output)
{
    FUNCTION_MARKER(0x8007225c, "SLUS_008.06");
    uint32 found = 0;
    w_u16(output, 0);
    w_u16(output + 2, 0);
    w_u16(output + 4, 0);
    for (uint32 row = 0; row < 10; ++row)
    {
        uint32 record = 0x80099e7c + row * 0x34;
        if (r_u32(record + 0xc) != 0)
        {
            for (uint32 axis = 0; axis < 3; ++axis)
            {
                uint32 sum = r_u16(output + axis * 2);
                uint32 value = r_u16(record + 0x14 + axis * 2);
                w_u16(output + axis * 2, sum + value);
            }
            found = 1;
        }
    }
    if (found == 0)
        return 0;
    sint32 x = r_s16(output);
    sint32 y = r_s16(output + 2);
    sint32 z = r_s16(output + 4);
    sint32 first = (sint16)(x < 0 ? -x : x);
    sint32 second = (sint16)(y < 0 ? -y : y);
    sint32 third = (sint16)(z < 0 ? -z : z);
    if (first > second)
    {
        sint32 temporary = first;
        first = second;
        second = temporary;
    }
    if (second > third)
    {
        sint32 temporary = second;
        second = third;
        third = temporary;
    }
    if (first > second)
    {
        sint32 temporary = first;
        first = second;
        second = temporary;
    }
    sint32 divisor = (third * 30 + second * 12 + first * 9) / 8;
    if (divisor != 0)
    {
        uint32 x_bits = (uint32)r_s16(output) << 14;
        uint32 x_result = (uint32)((sint32)x_bits / divisor);
        uint32 y_bits = (uint32)r_s16(output + 2) << 14;
        uint32 y_result = (uint32)((sint32)y_bits / divisor);
        uint32 z_bits = (uint32)r_s16(output + 4) << 14;
        uint32 z_result = (uint32)((sint32)z_bits / divisor);
        w_u16(output, x_result);
        w_u16(output + 2, y_result);
        w_u16(output + 4, z_result);
    }
    return found;
}

uint32 G3Function80072FB0(uint32 object, uint32 parameters, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80072fb0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 state = r_u32(object + 0x24);
    uint32 words[2];
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 source = 0x800a02a4 + word * 4;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffcu, (1u << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffcu, (15u << low_lane) & 15);
        words[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 destination = frame + 0x20 + word * 4;
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffcu, words[word] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        uint32 low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffcu, words[word] << (low_lane * 8), (15u << low_lane) & 15);
    }
    uint32 result = r_u32(object + 0x18) & 1;
    if (result != 0)
        return result;
    uint32 default_parameters = r_u32(0x80098784);
    bool standard = parameters == default_parameters;
    if (r_u32(object + 0xf8) != 0)
    {
        uint32 count = r_u32(state + 0x220) + 1;
        w_u32(state + 0x220, count);
        uint32 mode = r_u32(object + 0x154);
        if (mode != 0x1000)
        {
            if (mode == 1)
            {
                if ((sint32)count % 5 == 0)
                    G3Function80045804(object + 0xfc, parameters, frame);
                if (standard)
                {
                    uint32 game = r_u32(0x80097048);
                    if (r_s16(game + 0x6786) != 6 && (G3SdkRandom80027FA8() & 0x1f) == 0 && G3Function8003F0C8(0x831) == 0)
                    {
                        uint32 random = G3SdkRandom80027FA8();
                        w_u32(frame + 0x10, 1500);
                        uint32 pitch = (uint32)(sint32)(sint16)((random & 0x1f) - 15u);
                        G3Function8003E518(object, 0x831, pitch, 20, frame);
                    }
                }
            }
            else
                G3Function80045804(object + 0xfc, parameters, frame);
            mode = r_u32(object + 0x154);
        }
        if (mode != 1 && mode != 0x1000 && standard && (G3SdkRandom80027FA8() & 3) == 3)
        {
            sint32 random = (sint32)G3SdkRandom80027FA8();
            w_u16(frame + 0x18, (uint32)(random % 20 - 10));
            random = (sint32)G3SdkRandom80027FA8();
            w_u16(frame + 0x1a, (uint32)(random % 20 - 10));
            random = (sint32)G3SdkRandom80027FA8();
            w_u16(frame + 0x1c, (uint32)(random % 10 + 20));
            uint32 angle = (uint32)r_s16(object + 0x100);
            w_u32(frame + 0x10, parameters);
            w_u32(frame + 0x14, 2);
            G3Function80045288(object + 0xfc, frame + 0x18, frame + 0x20, angle, frame);
        }
    }
    bool departing = false;
    bool entering = false;
    bool captured_zero = false;
    if (r_u32(state + 0x214) != 0)
    {
        if (r_u32(object + 0xf8) == 0)
            departing = true;
        else
            entering = r_u32(state + 0x214) == 0;
    }
    else
    {
        result = r_u32(object + 0xf8);
        if (result == 0)
            captured_zero = true;
        else
            entering = r_u32(state + 0x214) == 0;
    }
    if (departing)
    {
        if (standard)
        {
            for (uint32 particle = 0; particle < 10; ++particle)
            {
                sint32 random = (sint32)G3SdkRandom80027FA8();
                w_u16(frame + 0x18, (uint32)(random % 30 - 15));
                random = (sint32)G3SdkRandom80027FA8();
                w_u16(frame + 0x1a, (uint32)(random % 30 - 15));
                random = (sint32)G3SdkRandom80027FA8();
                w_u16(frame + 0x1c, (uint32)(random % 10 + 25));
                uint32 angle = (uint32)r_s16(state + 0x21c);
                w_u32(frame + 0x10, parameters);
                w_u32(frame + 0x14, 2);
                G3Function80045288(state + 0x218, frame + 0x18, frame + 0x20, angle, frame);
            }
            if (G3Function8003F0C8(0x831) == 0)
            {
                uint32 random = G3SdkRandom80027FA8();
                w_u32(frame + 0x10, 2000);
                G3Function8003E518(object, 0x831, (random & 0x1f) + 15, 60, frame);
            }
        }
        G3Function8003EDC4(0x7f, 0);
    }
    else if (entering)
    {
        if (standard)
        {
            for (uint32 particle = 0; particle < 16; ++particle)
            {
                sint32 random = (sint32)G3SdkRandom80027FA8();
                w_u16(frame + 0x18, (uint32)(random % 30 - 15));
                random = (sint32)G3SdkRandom80027FA8();
                w_u16(frame + 0x1a, (uint32)(random % 30 - 15));
                random = (sint32)G3SdkRandom80027FA8();
                w_u16(frame + 0x1c, (uint32)(random % 15 + 25));
                uint32 angle = (uint32)r_s16(object + 0x100);
                w_u32(frame + 0x10, parameters);
                w_u32(frame + 0x14, 2);
                G3Function80045288(object + 0xfc, frame + 0x18, frame + 0x20, angle, frame);
            }
            if (G3Function8003F0C8(0x831) == 0)
            {
                w_u32(frame + 0x10, 2000);
                G3Function8003E518(object, 0x831, 0, 80, frame);
            }
        }
        G3Function8003EDC4(0x7f, 1);
    }
    if (!captured_zero)
        result = r_u32(object + 0xf8);
    w_u32(state + 0x214, result);
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 source = object + 0xfc + word * 4;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffcu, (1u << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffcu, (15u << low_lane) & 15);
        words[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 destination = state + 0x218 + word * 4;
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffcu, words[word] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        uint32 low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffcu, words[word] << (low_lane * 8), (15u << low_lane) & 15);
    }
    return result;
}

uint32 G3Function80045288(uint32 position, uint32 velocity, uint32 data, uint32 angle, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80045288, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 parameters = r_u32(guest_stack + 0x10);
    uint32 selector = r_u32(guest_stack + 0x14);
    if (parameters == 0)
        return 0;
    sint32 count = r_s16(parameters + 8);
    sint32 index = (sint32)(sint16)selector;
    if (count - 1 < index)
        return 0;
    uint32 table = r_u32(parameters + 0xc);
    uint32 descriptor = r_u32(table + ((uint32)index << 2));
    uint32 pool = r_u32(0x800a27fc);
    uint32 payload = r_u32(descriptor + 0x14);
    w_u32(frame + 0x14, pool);
    w_u32(frame + 0x18, 0x80044bc4);
    w_u32(frame + 0x1c, 0x8004512c);
    w_u32(frame + 0x10, data);
    w_u32(frame + 0x20, (uint32)(sint32)(sint16)angle);
    uint32 effect = G3Function8004257C(descriptor, payload + 0xc, position, velocity, frame);
    if (effect == 0)
        return 0;
    w_u32(effect + 0x44, parameters);
    if (r_u32(descriptor + 0x1c) != 0)
    {
        uint32 flags = r_u32(effect + 0xc);
        uint32 source = r_u32(effect + 8);
        w_u32(effect + 0xc, flags | 4);
        uint32 first = r_u32(source);
        uint32 second = r_u32(source + 4);
        uint32 third = r_u32(source + 8);
        uint32 fourth = r_u32(source + 0xc);
        w_u32(effect + 0x60, first);
        w_u32(effect + 0x64, second);
        w_u32(effect + 0x68, third);
        w_u32(effect + 0x6c, fourth);
        uint32 script = r_u32(descriptor + 0x1c);
        w_u16(effect + 0x74, 0);
        w_u32(effect + 0x70, script + 4);
    }
    return effect;
}

void G3Function80045804(uint32 position, uint32 parameters, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80045804, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    if (parameters == 0)
        return;
    sint32 count = r_s16(parameters + 8);
    uint32 table = r_u32(parameters + 0xc);
    uint32 descriptor = r_u32(table + (count >= 2 ? 4 : 0));
    uint32 pool = r_u32(0x800a27fc);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, pool);
    w_u32(frame + 0x18, 0x80044bc4);
    w_u32(frame + 0x1c, 0x800456ec);
    w_u32(frame + 0x20, 0xffffffffu);
    uint32 payload = r_u32(descriptor + 0x14);
    uint32 effect = G3Function8004257C(descriptor, payload, position, 0, frame);
    if (effect == 0)
        return;
    w_u32(effect + 0x44, parameters);
    if (r_u32(descriptor + 0x1c) != 0)
    {
        uint32 flags = r_u32(effect + 0xc);
        uint32 source = r_u32(effect + 8);
        w_u32(effect + 0xc, flags | 4);
        uint32 first = r_u32(source);
        uint32 second = r_u32(source + 4);
        uint32 third = r_u32(source + 8);
        uint32 fourth = r_u32(source + 0xc);
        w_u32(effect + 0x60, first);
        w_u32(effect + 0x64, second);
        w_u32(effect + 0x68, third);
        w_u32(effect + 0x6c, fourth);
        uint32 script = r_u32(descriptor + 0x1c);
        w_u16(effect + 0x74, 0);
        w_u32(effect + 0x70, script + 4);
    }
    else
        w_u16(effect + 0x22, 30);
}

uint32 G3Function8004257C(uint32 descriptor, uint32 data, uint32 position, uint32 velocity, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004257c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 extra_data = r_u32(guest_stack + 0x10);
    uint32 pool = r_u32(guest_stack + 0x14);
    uint32 initializer = r_u32(guest_stack + 0x18);
    uint32 callback = r_u32(guest_stack + 0x1c);
    uint32 angle = r_u32(guest_stack + 0x20);
    w_u32(frame + 0x20, 0);
    w_u32(frame + 0x10, extra_data);
    w_u32(frame + 0x14, pool);
    w_u32(frame + 0x18, initializer);
    w_u32(frame + 0x1c, callback);
    w_u32(frame + 0x24, (uint32)(sint32)(sint16)angle);
    return G3Function8004242C(descriptor, data, position, velocity, frame);
}

uint32 G3Function8004242C(uint32 owner, uint32 data, uint32 position, uint32 rotation, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004242c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x58;
    uint32 storage = r_u32(guest_stack + 0x20);
    uint32 pool = r_u32(guest_stack + 0x14);
    uint32 initializer = r_u32(guest_stack + 0x18);
    uint32 callback = r_u32(guest_stack + 0x1c);
    uint32 lifetime = r_u32(guest_stack + 0x24);
    w_u32(guest_stack + 0xc, rotation);
    uint32 resource = r_u32(owner + 8);
    if (storage == 0)
    {
        storage = G3Function800415CC(pool, frame);
        if (storage == 0)
            return 0;
    }
    uint32 effect = G3Function8004161C(pool, frame);
    if (effect == 0)
        return 0;
    uint32 current_rotation = r_u32(guest_stack + 0xc);
    w_u32(frame + 0x1c, current_rotation);
    uint32 velocity = r_u32(guest_stack + 0x10);
    w_u32(frame + 0x10, data);
    w_u32(frame + 0x14, resource);
    w_u32(frame + 0x18, position);
    w_u32(frame + 0x24, pool);
    w_u32(frame + 0x28, (uint32)(sint32)(sint16)lifetime);
    w_u32(frame + 0x20, velocity);
    if (initializer != 0)
        G3DispatchEffectInitializer(initializer, effect, callback, storage, 0, frame);
    else
        G3Function80042230(effect, callback, storage, 0, frame);
    G3Function80045B74(pool + 0x11c10, effect);
    return effect;
}

uint32 G3Function80042230(uint32 effect, uint32 callback, uint32 storage, uint32, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80042230, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 position = r_u32(guest_stack + 0x18);
    uint32 data = r_u32(guest_stack + 0x10);
    uint32 x = r_u16(position);
    uint32 first_index = r_u16(data);
    uint32 second_index = r_u16(data + 2);
    uint32 third_index = r_u16(data + 4);
    w_u16(effect + 0x1c, x);
    w_u16(effect + 0x1e, r_u16(position + 2));
    w_u16(effect + 0x20, r_u16(position + 4));
    uint32 table = r_u32(guest_stack + 0x14);
    uint32 first = table + (first_index << 3);
    w_u16(effect + 0x24, r_u16(first));
    w_u16(effect + 0x26, r_u16(first + 2));
    w_u16(effect + 0x28, r_u16(first + 4));
    uint32 second = table + (second_index << 3);
    w_u16(effect + 0x2c, r_u16(second));
    w_u16(effect + 0x2e, r_u16(second + 2));
    w_u16(effect + 0x30, r_u16(second + 4));
    uint32 third = table + (third_index << 3);
    w_u16(effect + 0x34, r_u16(third));
    w_u16(effect + 0x36, r_u16(third + 2));
    w_u16(effect + 0x38, r_u16(third + 4));
    uint32 attributes = r_u8(data + 7);
    uint32 rotation = r_u32(guest_stack + 0x1c);
    uint32 velocity = r_u32(guest_stack + 0x20);
    uint32 lifetime = r_u16(guest_stack + 0x28);
    uint32 color;
    if ((attributes & 2) != 0)
    {
        w_u32(effect + 0xc, r_u32(effect + 0xc) | 1);
        uint32 texture = r_u32(data + 8);
        w_u32(effect + 8, texture);
        color = (r_u32(texture + 0xc) & 0x03ffffffu) | 0x24000000u;
    }
    else
    {
        w_u32(effect + 0xc, r_u32(effect + 0xc) & 0xfffffffeu);
        color = (r_u32(data + 8) & 0x03ffffffu) | 0x20000000u;
    }
    w_u32(effect + 0x18, color);
    w_u32(effect + 0x10, callback != 0 ? callback : 0x80041de0);
    w_u32(effect + 0x14, storage);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 value = rotation != 0 ? r_u16(rotation + axis * 2) : 0;
        w_u16(effect + 0x4c + axis * 2, value);
    }
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 value = velocity != 0 ? r_u16(velocity + axis * 2) : 0;
        w_u16(effect + 0x52 + axis * 2, value);
    }
    uint32 game = r_u32(0x80097048);
    uint32 transform = r_u32(game + 0x4c);
    uint32 result = G3Function80041CE4(storage + 0xc, transform, effect + 0x1c, 0, frame);
    w_u16(effect + 0x22, lifetime);
    return result;
}

uint32 G3Function80044BC4(uint32 effect, uint32 callback, uint32 storage, uint32, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80044bc4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 vertices = r_u32(guest_stack + 0x14);
    uint32 data = r_u32(guest_stack + 0x10);
    uint32 position = r_u32(guest_stack + 0x18);
    uint32 rotation = r_u32(guest_stack + 0x1c);
    uint32 velocity = r_u32(guest_stack + 0x20);
    uint32 lifetime = r_u16(guest_stack + 0x28);
    w_u32(guest_stack + 8, storage);
    w_u32(guest_stack + 4, callback);
    w_u16(frame + 0x10, lifetime);
    G3Function8002F718(storage + 0xc);
    uint32 coordinate = r_u16(position);
    uint32 flags = r_u32(effect + 0xc);
    w_u16(effect + 0x1c, coordinate);
    w_u16(effect + 0x1e, r_u16(position + 2));
    w_u16(effect + 0x20, r_u16(position + 4));
    for (uint32 vertex = 0; vertex < 3; ++vertex)
    {
        uint32 source = vertices + (vertex == 2 ? 0x18 : vertex * 8);
        for (uint32 axis = 0; axis < 3; ++axis)
            w_u16(effect + 0x24 + vertex * 8 + axis * 2, r_u16(source + axis * 2));
    }
    w_u16(effect + 0x3c, r_u16(vertices + 0x10));
    w_u16(effect + 0x3e, r_u16(vertices + 0x12));
    coordinate = r_u16(vertices + 0x14);
    w_u32(effect + 0xc, flags | 8);
    w_u16(effect + 0x40, coordinate);
    uint32 color;
    if ((r_u8(data + 7) & 2) != 0)
    {
        w_u32(effect + 0xc, flags | 9);
        uint32 texture = r_u32(data + 8);
        w_u32(effect + 8, texture);
        color = (r_u32(texture + 0xc) & 0x03ffffffu) | 0x2c000000u;
    }
    else
    {
        w_u32(effect + 0xc, (flags | 8) & 0xfffffffeu);
        color = (r_u32(data + 8) & 0x03ffffffu) | 0x28000000u;
    }
    w_u32(effect + 0x18, color);
    callback = r_u32(guest_stack + 4);
    w_u32(effect + 0x10, callback != 0 ? callback : 0x8004197c);
    storage = r_u32(guest_stack + 8);
    w_u32(effect + 0x14, storage);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 value = rotation != 0 ? r_u16(rotation + axis * 2) : 0;
        w_u16(effect + 0x4c + axis * 2, value);
    }
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 value = velocity != 0 ? r_u16(velocity + axis * 2) : 0;
        w_u16(effect + 0x52 + axis * 2, value);
    }
    flags = r_u32(effect + 0xc);
    lifetime = r_u16(frame + 0x10);
    uint32 result = flags | 0x4000;
    w_u16(effect + 0x22, lifetime);
    w_u32(effect + 0xc, result);
    return result;
}

uint32 G3Function8004512C(uint32 effect, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004512c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 game = r_u32(0x80097048);
    uint32 storage = r_u32(effect + 0x14);
    uint32 transform = r_u32(game + 0x4c);
    G3Function80041CE4(storage + 0xc, transform, effect + 0x1c, 0, frame);
    sint32 before_height = r_s16(effect + 0x20);
    sint32 before_limit = r_s16(effect + 0x22);
    uint32 before = before_height < before_limit;
    G3Function80044B60(effect, frame);
    sint32 height = r_s16(effect + 0x20);
    sint32 limit = r_s16(effect + 0x22);
    uint32 result = height < limit;
    uint32 reset_height = r_u16(effect + 0x22);
    if (before == result)
        return result;
    uint32 parameters = r_u32(effect + 0x44);
    w_u16(effect + 0x20, reset_height);
    if (parameters != 0)
    {
        uint32 descriptors = r_u32(parameters + 0xc);
        uint32 descriptor = r_u32(descriptors);
        uint32 child_pool = r_u32(0x800a27fc);
        w_u32(frame + 0x10, 0);
        w_u32(frame + 0x20, 0);
        w_u32(frame + 0x14, child_pool);
        w_u32(frame + 0x18, 0x80044bc4);
        w_u32(frame + 0x1c, 0x8004508c);
        uint32 data = r_u32(descriptor + 0x14);
        uint32 child = G3Function8004257C(descriptor, data, effect + 0x1c, 0, frame);
        if (child != 0)
        {
            uint32 random = G3SdkRandom80027FA8() & 0xfff;
            w_u16(child + 0x28, random >> 1);
            uint32 flags = r_u32(child + 0xc);
            w_u32(child + 0xc, flags | 0x10000);
            w_u16(child + 0x38, random >> 2);
            w_u16(child + 0x48, (random >> 3) + 1);
            w_u16(child + 0x4a, random * 3);
        }
    }
    return G3Function80041578(effect, pool, frame);
}

uint32 G3Function80044E1C(uint32 effect)
{
    FUNCTION_MARKER(0x80044e1c, "SLUS_008.06");
    uint32 flags = r_u32(effect + 0xc);
    uint32 offset;
    if ((flags & 0x10000) != 0)
        offset = 0x28;
    else if ((flags & 0x2000) != 0)
        offset = 0x26;
    else
        return flags & 0x2000;
    uint32 result = 0;
    for (uint32 coordinate = 0; coordinate < 2; ++coordinate)
    {
        uint32 address = effect + offset + coordinate * 0x10;
        uint32 value = r_u16(address);
        uint32 step = r_u16(effect + 0x48);
        uint32 sum = value + step;
        w_u16(address, sum);
        sint32 limit = r_s16(effect + 0x4a);
        sint32 advanced = (sint16)sum;
        result = r_u16(effect + 0x4a);
        if (limit < advanced)
            w_u16(address, result);
    }
    return result;
}

uint32 G3Function8004508C(uint32 effect, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004508c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 flags = r_u32(effect + 0xc);
    w_u32(effect + 0xc, flags | 0x10000);
    G3Function80044E1C(effect);
    sint32 height = r_s16(effect + 0x28);
    sint32 limit = r_s16(effect + 0x4a);
    if (height == limit)
        return G3Function80041578(effect, pool, frame);
    uint32 result = (uint32)height << 7;
    if (limit != 0)
    {
        sint32 quotient = (height * 128) / limit;
        uint32 intensity = 127 - (uint32)quotient;
        uint32 components = intensity | (intensity << 16) | (intensity << 8);
        uint32 color = r_u32(effect + 0x18);
        result = (color & 0xfe000000) | components;
        w_u32(effect + 0x18, result);
    }
    return result;
}

uint32 G3Function80044F14(uint32 effect, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80044f14, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 flags = r_u32(effect + 0xc);
    w_u32(effect + 0xc, flags | 0x2000);
    G3Function80044E1C(effect);
    G3Function8004512C(effect, pool, frame);
    uint32 result = r_u32(effect + 0xc) & 4;
    if (result != 0)
        result = G3Function80041880(effect, pool, frame);
    return result;
}

uint32 G3Function80039DBC(sint32 value, sint32 factor)
{
    FUNCTION_MARKER(0x80039dbc, "SLUS_008.06");
    uint32 product = (uint32)value * (uint32)factor;
    sint32 change = (sint32)product >> 12;
    if (change == 0 && factor != 0)
        return 0;
    if (value >= 0 ? value < change : change < value)
        return 0;
    return (uint32)value - (uint32)change;
}

uint32 G3Function80044F7C(uint32 effect, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80044f7c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 remaining = r_u16(effect + 0x48) - 1;
    w_u16(effect + 0x48, remaining);
    if ((sint16)remaining <= 0)
        return G3Function80041578(effect, pool, frame);
    uint32 storage = r_u32(effect + 0x14);
    uint32 flags = r_u32(effect + 0xc);
    uint32 game = r_u32(0x80097048);
    w_u32(effect + 0xc, flags | 0x2000);
    uint32 transform = r_u32(game + 0x4c);
    G3Function80041CE4(storage + 0xc, transform, effect + 0x1c, 0, frame);
    G3Function80044B60(effect, frame);
    sint32 value = r_s16(effect + 0x4c);
    sint32 factor = r_s16(effect + 0x58);
    uint32 result = G3Function80039DBC(value, factor);
    value = r_s16(effect + 0x4e);
    factor = r_s16(effect + 0x58);
    w_u16(effect + 0x4c, result);
    result = G3Function80039DBC(value, factor);
    value = r_s16(effect + 0x50);
    factor = r_s16(effect + 0x58);
    w_u16(effect + 0x4e, result);
    result = G3Function80039DBC(value, factor);
    w_u16(effect + 0x50, result);
    return result;
}
