#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void G3BrkblokUpdate(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000234, "G3L_47");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    if (r_s16(state + 2) != 0)
    {
        uint32 velocity = r_u16(state + 0xe) - 10;
        w_u16(state + 0xe, velocity);
        uint32 height = r_u16(object + 0x5c) + velocity;
        w_u16(object + 0x5c, height);
        sint32 floor = r_s16(state + 4);
        uint32 floor_bits = r_u16(state + 4);
        if ((sint16)height < floor)
        {
            if (r_s16(state + 0xa) == 0)
            {
                w_u16(object + 0x5c, floor_bits - 1);
                sint32 current_velocity = r_s16(state + 0xe);
                uint32 bounces = r_u16(state + 0xa);
                w_u16(state + 0xa, bounces + 1);
                w_u16(state + 0xe, (uint32)((-current_velocity) / 3));
            }
            else
            {
                w_u16(object + 0x5c, floor_bits);
                w_u16(state + 0xe, 0);
                w_u16(state + 2, 0);
            }
        }
    }
    else
    {
        sint32 timer = r_s16(state + 6);
        uint32 timer_bits = r_u16(state + 6);
        if (timer != 0)
        {
            --timer_bits;
            w_u16(state + 6, timer_bits);
            if ((uint16)timer_bits == 0)
            {
                uint32 descriptor = r_u32(object + 0x1c);
                if ((r_u32(descriptor + 0x30) & 8) != 0)
                {
                    uint32 placement = r_u32(object + 0x20);
                    if (placement != 0 && r_u32(placement + 4) != 0) G3BrkblokDropAbove(object, frame);
                }
                G3Function8002A9AC(object, frame);
            }
        }
    }
    if ((r_u32(object + 0x10) & 0x80) != 0 && (r_u16(state + 0x14) & 1) != 0)
    {
        uint32 timer = r_u16(state + 0x1a);
        sint32 period = r_s8(state + 0x18);
        ++timer;
        w_u16(state + 0x1a, timer);
        if ((sint16)timer >= period)
        {
            uint32 table = r_u32(state + 0x20);
            w_u16(state + 0x1a, 0);
            uint32 texture;
            if (table != 0)
            {
                uint32 index = r_u16(state + 0x1c);
                table = r_u32(state + 0x20);
                ++index;
                w_u16(state + 0x1c, index);
                texture = r_u32(table + (uint32)(sint32)(sint16)index * 4);
                if ((sint32)texture < 0)
                {
                    w_u16(state + 0x1c, 0);
                    texture = r_u32(table);
                }
            }
            else
            {
                uint32 current = G3Function8003B0D0(object, frame);
                sint32 next = (sint16)current;
                sint32 maximum = r_s8(state + 0x17);
                ++next;
                if (maximum < next) next = r_s8(state + 0x16);
                texture = (uint32)next;
            }
            G3Function8003B030(object, (uint32)(sint32)(sint16)texture);
        }
    }
    uint32 descriptor = r_u32(object + 0x1c);
    if ((r_u32(descriptor) & 0x2000) != 0) G3Function8003C614(object, game, frame);
}

uint32 G3BrkblokDropAbove(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000474, "G3L_47");
    uint32 frame = guest_stack - 0x38;
    uint32 placement = r_u32(object + 0x20);
    uint32 list = r_u32(placement + 4);
    sint32 distance = r_s16(object + 0x104);
    uint32 distance_bits = r_u16(object + 0x104);
    if (distance == 0) return 0;
    sint32 count = r_s32(list);
    list += 4;
    for (sint32 index = 0; index < count; ++index)
    {
        uint32 current = r_u32(list);
        placement = r_u32(object + 0x20);
        if (current == placement) return 0;
        uint32 other = r_u32(current + 0x20);
        if (other == 0) other = G3Function8002A930(current, frame);
        if (other != 0)
        {
            uint32 target = r_u32(other + 0xec);
            uint32 original_target = r_u32(object + 0xec);
            if (target == original_target)
            {
                uint32 x = r_u16(object + 0x58);
                uint32 px = r_u16(current + 0x10);
                uint32 py = r_u16(current + 0x12);
                sint32 dx = (sint16)(x - px);
                if (dx < 0) dx = -dx;
                uint32 y = r_u16(object + 0x5a);
                sint32 z = r_s16(object + 0x5c);
                sint32 dy = (sint16)(y - py);
                sint32 other_z = r_s16(other + 0x5c);
                if (dy < 0) dy = -dy;
                if (z < other_z && (sint16)dx < 256 && (sint16)dy < 256)
                {
                    uint32 state = other + 0x104;
                    w_u16(state + 2, 1);
                    w_u16(state + 0xa, 0);
                    w_u16(state + 0xe, 0);
                    w_u16(state + 4, r_u16(other + 0x5c) - distance_bits);
                }
            }
        }
        list += 4;
    }
    return 0;
}

void G3BrkblokRestorePosition(uint32 placement)
{
    FUNCTION_MARKER(0x000013c8, "G3L_47");
    uint32 destination = r_u32(placement + 0x30);
    if (destination == 0) return;
    uint32 source = placement + 0x10;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 lane = source & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
    uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
    uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
    uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
    uint32 last = (uint32)r_s16(placement + 0x14);
    high_address = destination + 3;
    high_lane = high_address & 3;
    lane = destination & 3;
    w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
    w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
    w_u16(destination + 4, last);
}

void G3BrkblokRestorePlacements(uint32 game, uint32 module_base)
{
    FUNCTION_MARKER(0x000013f8, "G3L_47");
    uint32 level = r_u32(game + 0x48);
    uint32 count = r_u32(level + 0x7c);
    uint32 data = r_u32(level);
    uint32 record = r_u32(data + 0x14);
    uint32 end = record + count * 0x34;
    uint32 destination = record + 0x10;
    while (record < end)
    {
        uint32 descriptor = r_u32(record);
        if (descriptor != 0)
        {
            uint32 name = r_u32(descriptor + 0x20);
            uint32 expected = r_u32(module_base + 0x156c);
            uint32 first = r_u32(name);
            uint32 source = r_u32(destination + 0x20);
            if (first == expected)
            {
                uint32 second = r_u32(name + 4);
                uint32 expected_second = r_u32(module_base + 0x1570);
                if (second == expected_second && source != 0)
                {
                    uint32 high_address = source + 3;
                    uint32 high_lane = high_address & 3;
                    uint32 lane = source & 3;
                    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
                    uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
                    uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
                    uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
                    uint32 last = (uint32)r_s16(source + 4);
                    high_address = destination + 3;
                    high_lane = high_address & 3;
                    lane = destination & 3;
                    w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
                    w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
                    w_u16(destination + 4, last);
                }
            }
        }
        record += 0x34;
        destination += 0x34;
    }
}

void G3BrkblokTriangle(uint32 effect, uint32 callback, uint32 storage, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000005dc, "G3L_47");
    uint32 frame = guest_stack - 8;
    uint32 polygon = r_u32(guest_stack + 0x10);
    uint32 vertices = r_u32(guest_stack + 0x14);
    uint32 first_index = r_u16(polygon);
    uint32 second_index = r_u16(polygon + 2);
    uint32 third_index = r_u16(polygon + 4);
    uint32 points[3] = { vertices + first_index * 8, vertices + second_index * 8, vertices + third_index * 8 };
    sint32 x = r_s16(points[0]);
    sint32 second_x = r_s16(points[1]);
    sint32 third_x = r_s16(points[2]);
    sint32 center_x = (x + second_x + third_x) / 3;
    uint32 lifetime = r_u16(guest_stack + 0x28);
    w_u16(frame, (uint32)center_x);
    for (uint32 axis = 1; axis != 3; ++axis)
    {
        sint32 first = r_s16(points[0] + axis * 2);
        sint32 second = r_s16(points[1] + axis * 2);
        sint32 third = r_s16(points[2] + axis * 2);
        w_u16(frame + axis * 2, (uint32)((first + second + third) / 3));
    }
    uint32 origin = r_u16(storage + 0x20);
    w_u16(effect + 0x1c, origin + (uint32)center_x);
    for (uint32 axis = 1; axis != 3; ++axis)
    {
        origin = r_u16(storage + 0x20 + axis * 4);
        uint32 center = r_u16(frame + axis * 2);
        w_u16(effect + 0x1c + axis * 2, origin + center);
    }
    for (uint32 vertex = 0; vertex != 3; ++vertex)
    {
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 coordinate = r_u16(points[vertex] + axis * 2);
            uint32 center = r_u16(frame + axis * 2);
            w_u16(effect + 0x24 + vertex * 8 + axis * 2, coordinate - center);
        }
    }
    uint32 textured = r_u8(polygon + 7) & 2;
    uint32 color;
    if (textured != 0)
    {
        w_u32(effect + 0xc, r_u32(effect + 0xc) | 1);
        uint32 texture = r_u32(polygon + 8);
        w_u32(effect + 8, texture);
        color = (r_u32(texture + 0xc) & 0x03ffffff) | 0x24000000;
    }
    else
    {
        w_u32(effect + 0xc, r_u32(effect + 0xc) & 0xfffffffe);
        color = (r_u32(polygon + 8) & 0x03ffffff) | 0x20000000;
    }
    w_u32(effect + 0x18, color);
    w_u32(effect + 0x10, callback);
    w_u32(effect + 0x14, storage);
    sint32 center = r_s16(frame);
    w_u16(effect + 0x4c, (uint32)((-center) >> 1));
    center = (sint16)r_u16(frame + 2);
    w_u16(effect + 0x4e, (uint32)((-center) >> 1));
    uint32 center_z = r_u16(frame + 4);
    w_u16(effect + 0x52, 0);
    w_u16(effect + 0x54, 0);
    w_u16(effect + 0x56, 0);
    w_u16(effect + 0x22, lifetime);
    w_u16(effect + 0x50, 0 - (uint32)((sint32)(sint16)center_z >> 1));
}

uint32 G3BrkblokFragmentTick(uint32 effect, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000085c, "G3L_47");
    uint32 frame = guest_stack - 0x18;
    G3Function8004197C(effect, pool, frame);
    uint32 storage = r_u32(effect + 0x14);
    uint32 angle = (uint32)r_s16(effect + 0x44);
    G3Function8005B94C(angle, storage + 0xc, frame);
    storage = r_u32(effect + 0x14);
    angle = (uint32)r_s16(effect + 0x46);
    G3Function8005BA5C(angle, storage + 0xc, frame);
    storage = r_u32(effect + 0x14);
    angle = (uint32)r_s16(effect + 0x48);
    G3Function8005BB6C(angle, storage + 0xc, frame);
    if (r_s16(effect + 0x22) == 29)
    {
        uint32 x = r_u16(effect + 0x4c);
        uint32 z = r_u16(effect + 0x50);
        w_u16(effect + 0x4c, 0 - x);
        uint32 y = r_u16(effect + 0x4e);
        w_u16(effect + 0x50, 0 - z);
        w_u16(effect + 0x4e, 0 - y);
        return 0 - y;
    }
    return 29;
}

void G3BrkblokShatter(uint32 object, uint32 pool, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000008e4, "G3L_47");
    uint32 frame = guest_stack - 0x68;
    w_u32(frame + 0x68, object);
    w_u32(frame + 0x6c, pool);
    uint32 descriptor = r_u32(object + 0x1c);
    sint32 model_index = r_s16(object + 0xa2);
    uint32 models = r_u32(descriptor + 0xc);
    uint32 model = r_u32(models + (uint32)model_index * 4);
    uint32 matrix = r_u32(object + 0x34);
    sint32 segments = r_s16(model + 6);
    uint32 segment = r_u32(model + 0x18);
    uint32 end = segment + (uint32)segments * 24;
    w_u32(frame + 0x38, end);
    w_u32(frame + 0x30, r_u32(model + 8));
    uint32 particle_index = 0;
    w_u32(frame + 0x34, r_u32(model + 0x14));
    while (segment < end)
    {
        sint32 last_face = r_s16(model + 4) - 1;
        sint32 segment_last = r_s16(segment + 6);
        uint32 last_face_bits = r_u16(model + 4) - 1;
        if (last_face < segment_last) w_u16(segment + 6, last_face_bits);
        last_face = r_s16(model + 4) - 1;
        sint32 first_face = r_s16(segment + 4);
        if (first_face < last_face)
        {
            uint32 faces = r_u32(frame + 0x34);
            segment_last = r_s16(segment + 6);
            uint32 polygon = faces + (uint32)first_face * 12;
            uint32 polygon_end = faces + (uint32)segment_last * 12;
            while (polygon < polygon_end)
            {
                bool randomize = (particle_index & 3) == 0;
                ++particle_index;
                if (randomize)
                {
                    w_u16(module_base + 0x1578, (G3SdkRandom80027FA8() & 255) - 128);
                    w_u16(module_base + 0x157a, (G3SdkRandom80027FA8() & 255) - 128);
                    w_u16(module_base + 0x157c, (G3SdkRandom80027FA8() & 255) - 128);
                }
                uint32 actual_pool = r_u32(frame + 0x6c);
                uint32 storage = G3Function800415CC(actual_pool, frame);
                if (storage != 0)
                {
                    actual_pool = r_u32(frame + 0x6c);
                    uint32 effect = G3Function8004161C(actual_pool, frame);
                    if (effect != 0)
                    {
                        for (uint32 group = 0; group != 2; ++group)
                        {
                            uint32 words[4];
                            for (uint32 word = 0; word != 4; ++word) words[word] = r_u32(matrix + group * 16 + word * 4);
                            for (uint32 word = 0; word != 4; ++word) w_u32(storage + 0xc + group * 16 + word * 4, words[word]);
                        }
                        uint32 vertices = r_u32(frame + 0x30);
                        actual_pool = r_u32(frame + 0x6c);
                        uint32 owner = r_u32(frame + 0x68);
                        w_u32(frame + 0x10, polygon);
                        w_u32(frame + 0x18, 0);
                        w_u32(frame + 0x1c, 0);
                        w_u32(frame + 0x20, 0);
                        w_u32(frame + 0x28, 30);
                        w_u32(frame + 0x14, vertices);
                        w_u32(frame + 0x24, actual_pool);
                        G3BrkblokTriangle(effect, module_base + 0x85c, storage, frame);
                        uint32 angular = r_u16(module_base + 0x1578);
                        w_u16(effect + 0x44, angular);
                        angular = r_u16(module_base + 0x157a);
                        actual_pool = r_u32(frame + 0x6c);
                        w_u16(effect + 0x46, angular);
                        angular = r_u16(module_base + 0x157c);
                        w_u16(effect + 0x48, angular);
                        G3Function80045B74(actual_pool + 0x11c10, effect);
                    }
                }
                polygon += 12;
            }
        }
        segment += 24;
        end = r_u32(frame + 0x38);
        matrix += 32;
    }
}

void G3BrkblokParticles(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000b6c, "G3L_47");
    uint32 frame = guest_stack - 0x50;
    uint32 resource = G3Function8003AA48(module_base + 0x14f4);
    if (resource == 0) return;
    w_u16(frame + 0x28, r_u16(object + 0x58));
    w_u16(frame + 0x2a, r_u16(object + 0x5a));
    w_u16(frame + 0x2c, r_u16(object + 0x5c) + 320);
    uint32 models = r_u32(resource + 0xc);
    uint32 model = r_u32(models);
    for (uint32 index = 0; index != 20; ++index)
    {
        sint32 random = (sint32)G3SdkRandom80027FA8();
        uint32 sample = 0x80098838 + (uint32)(random % 244) * 8;
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            sint32 coordinate = (sint16)r_u16(sample + axis * 2);
            if (axis == 2)
            {
                uint32 pool = r_u32(0x800a27fc);
                w_u16(frame + 0x34, (uint32)(coordinate >> 7));
                w_u32(frame + 0x14, pool);
            }
            else w_u16(frame + 0x30 + axis * 2, (uint32)(coordinate >> 7));
        }
        w_u32(frame + 0x10, 0);
        w_u32(frame + 0x18, 0x80044bc4);
        w_u32(frame + 0x1c, 0x80041de0);
        w_u32(frame + 0x20, 15);
        uint32 data = r_u32(model + 0x14);
        G3Function8004257C(model, data, frame + 0x28, frame + 0x30, frame);
    }
}

void G3BrkblokConfigure(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000010a0, "G3L_47");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    w_u32(frame + 0x24, commands);
    if (commands == 0) return;
    uint32 first = r_u32(commands);
    uint32 expected = r_u32(module_base + 0x150c);
    for (;;)
    {
        if (first == expected)
        {
            uint32 second = r_u32(commands + 4);
            uint32 expected_second = r_u32(module_base + 0x1510);
            if (second == expected_second) return;
        }
        commands = r_u32(frame + 0x24);
        expected = r_u32(module_base + 0x1518);
        first = r_u32(commands);
        uint32 payload = commands + 8;
        sint32 operation = -1;
        bool first_matched = false;
        for (uint32 kind = 0; kind != 7; ++kind)
        {
            if (kind != 0)
            {
                if (kind == 2 || kind == 6 || !first_matched) commands = r_u32(frame + 0x24);
                expected = r_u32(module_base + 0x1518 + kind * 12);
                first = r_u32(commands);
            }
            first_matched = first == expected;
            if (first_matched)
            {
                uint32 second = r_u32(commands + 4);
                uint32 expected_second = r_u32(module_base + 0x151c + kind * 12);
                if (second == expected_second)
                {
                    operation = (sint32)kind;
                    break;
                }
            }
        }
        uint32 words = 2;
        switch (operation)
        {
        case 0: words = 3; break;
        case 1:
            w_u32(object + 0x10, r_u32(object + 0x10) | 0x80);
            w_u16(state + 0x14, r_u16(state + 0x14) & 0xfffe);
            words = 3;
            break;
        case 2:
            w_u32(object + 0x10, r_u32(object + 0x10) | 0x80);
            w_u16(state + 0x14, r_u16(state + 0x14) | 1);
            break;
        case 3:
            w_u32(object + 0x10, r_u32(object + 0x10) | 0x80);
            w_u16(state + 0x14, r_u16(state + 0x14) & 0xfffe);
            break;
        case 4:
            w_u32(object + 0x10, r_u32(object + 0x10) | 0x80);
            w_u16(state + 0x14, r_u16(state + 0x14) ^ 1);
            break;
        case 5:
        {
            w_u32(object + 0x10, r_u32(object + 0x10) | 0x80);
            w_u32(state + 0x20, 0);
            w_u8(state + 0x16, r_u8(payload));
            uint32 flags = r_u16(state + 0x14);
            uint32 maximum = r_u8(payload + 2);
            w_u16(state + 0x14, flags | 1);
            w_u8(state + 0x17, maximum);
            words = 3;
            break;
        }
        case 6:
        {
            uint32 table = r_u32(payload);
            w_u32(state + 0x20, table);
            if (table != 0)
            {
                w_u32(object + 0x10, r_u32(object + 0x10) | 0x80);
                uint32 flags = r_u16(state + 0x14);
                w_u16(state + 0x1c, 0);
                w_u16(state + 0x14, flags | 1);
            }
            words = 3;
            break;
        }
        default: break;
        }
        if (operation >= 0)
        {
            uint32 cursor = r_u32(frame + 0x24);
            w_u32(frame + 0x24, cursor + words * 4);
        }
        else if (G3Function80085B84(frame + 0x24, frame) == 0) return;
        expected = r_u32(module_base + 0x150c);
        commands = r_u32(frame + 0x24);
        first = r_u32(commands);
    }
}

void G3BrkblokCommand(uint32 object, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000013a0, "G3L_47");
    uint32 frame = guest_stack - 0x18;
    if (payload != 0) G3BrkblokConfigure(object, payload, module_base, frame);
}

void G3BrkblokCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001064, "G3L_47");
    uint32 frame = guest_stack - 0x18;
    uint32 descriptor = r_u32(object + 0x1c);
    if ((r_u32(descriptor) & 0x2000) != 0) G3Function8003C864(object, game, frame);
}

void G3BrkblokDamage(uint32 object, uint32 game, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000cb8, "G3L_47");
    uint32 frame = guest_stack - 0x50;
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 player = r_u32(game + 0x50);
    uint32 contact = r_u32(object + 0xb0);
    uint32 flags = r_u32(descriptor);
    uint32 state = object + 0x104;
    if ((flags & 0x20000000) != 0 && (r_u16(payload + 0x12) & 1) == 0) return;
    uint32 cursor = 0;
    uint32 placement = r_u32(object + 0x20);
    if (placement != 0)
    {
        uint32 list = r_u32(placement + 4);
        if (list != 0) cursor = list;
    }
    if (cursor != 0)
    {
        sint32 count = r_s32(cursor);
        cursor += 4;
        for (sint32 index = 0; index < count; ++index)
        {
            uint32 entry = r_u32(cursor);
            uint32 other = r_u32(entry + 0x20);
            if (other != 0 && r_s16(other + 0x106) != 0) return;
            cursor += 4;
        }
    }
    if (r_s16(state + 0x12) == 0) return;
    bool high = false;
    bool low = false;
    sint32 kind = r_s16(contact + 4);
    sint32 height = 0;
    bool classify = false;
    if (kind == 5)
    {
        height = r_s16(contact + 0x1c);
        classify = true;
    }
    else if (kind == 2)
    {
        uint32 index = (uint32)r_s16(object + 0xa2) << 2;
        descriptor = r_u32(object + 0x1c);
        uint32 vertex = r_u32(contact + 8);
        uint32 models = r_u32(descriptor + 0xc);
        sint32 vertex_index = r_s16(vertex + 8);
        uint32 model = r_u32(models + index);
        uint32 vertices = r_u32(model + 0x10);
        height = r_s16(vertices + (uint32)vertex_index * 6 + 4);
        classify = true;
    }
    if (classify)
    {
        high = height >= 500;
        low = height < 4001;
    }
    if (high && r_u32(player + 0x154) != 0x20) return;
    if (low)
    {
        uint32 player_state = r_u32(player + 0x154);
        if (player_state == 0x20 || player_state == 0x80) return;
    }
    w_u16(state + 6, 10);
    G3Function800374A4(object, frame);
    descriptor = r_u32(object + 0x1c);
    if ((r_u32(descriptor + 0x30) & 8) != 0) w_u16(state + 6, 1);
    else
    {
        if ((r_u32(descriptor + 0x3c) & 0x3400) != 0)
        {
            uint32 resource = G3Function8003AA48(module_base + 0x1500);
            if (resource != 0)
            {
                for (uint32 index = 0; index != 8; ++index) G3Function8002A454(object, resource, frame);
            }
        }
        else
        {
            uint32 pool = r_u32(0x800a27fc);
            G3BrkblokParticles(object, module_base, frame);
            pool = r_u32(0x800a27fc);
            G3BrkblokShatter(object, pool, module_base, frame);
        }
        if (cursor != 0) G3BrkblokDropAbove(object, frame);
    }
    G3Function800499E0(4, 5);
    sint32 count = r_s16(state + 0x10);
    if (count > 0)
    {
        uint32 offsets = module_base + 0x1580;
        uint32 velocities = module_base + 0x15a0;
        for (sint32 index = 0; index < count; ++index)
        {
            uint32 x = (uint32)r_s16(velocities);
            uint32 y = (uint32)r_s16(velocities + 2);
            uint32 z = (uint32)r_s16(velocities + 4);
            w_u32(frame + 0x10, 0);
            w_u32(frame + 0x14, 0);
            w_u32(frame + 0x18, 0xfffffffb);
            w_u32(frame + 0x1c, 100);
            w_u32(frame + 0x20, 100);
            w_u32(frame + 0x24, 100);
            uint32 child = G3Function800460E8(object, x, y, z, frame);
            if (child != 0)
            {
                for (uint32 axis = 0; axis != 3; ++axis)
                {
                    uint32 coordinate = r_u16(child + 0x58 + axis * 2);
                    uint32 offset = r_u16(offsets + axis * 2);
                    w_u16(child + 0x58 + axis * 2, coordinate + offset);
                }
            }
            offsets += 8;
            velocities += 8;
        }
    }
    w_u32(object + 0x10, r_u32(object + 0x10) | 0x800);
    G3Function800370E8(object, 0x1000, frame);
    G3Function800370E8(object, 0x2000, frame);
    G3Function800370E8(object, 0x4000, frame);
    G3Function800370E8(object, 0x8000, frame);
}

uint32 G3BombTimerEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_63");
    uint32 frame = guest_stack - 0x18;
    uint32 data = 0;
    uint32 state = 0;
    if (object != 0)
    {
        state = object + 0x104;
        data = r_u32(object + 0x28);
    }
    if (data == 0)
        return 1;
    switch (message)
    {
    case 1:
        G3BombTimerInitialize(object, game, state, data);
        return 1;
    case 2:
        G3BombTimerUpdate(object, game, state, data, module_base, frame);
        return 1;
    case 9:
        G3BombTimerConfigure(object, payload, module_base, frame);
        return 1;
    case 11:
    {
        uint32 level = r_u32(game + 0x48);
        G3Function80045F40(r_u32(level));
        return 1;
    }
    default:
        return 0;
    }
}

void G3BombTimerInitialize(uint32 object, uint32 game, uint32 state, uint32 data)
{
    FUNCTION_MARKER(0x000000c8, "G3L_63");
    uint32 seconds = (uint32)r_s16(data);
    w_u16(state, 1);
    w_u16(state + 4, 0);
    w_u32(state + 8, seconds * 30);
    w_u32(object + 0x10, r_u32(object + 0x10) | 0xc00);
    uint32 player = r_u32(game + 0x50);
    w_u32(player + 0x128, r_u32(player + 0x128) | 0x4000);
    player = r_u32(game + 0x50);
    w_u32(player + 0x10, r_u32(player + 0x10) | 0x100);
    w_u32(object + 0x154, 0);
}

void G3BombTimerRemainingText(uint32 destination, uint32 format, uint32 value, uint32 guest_stack)
{
    const char *pattern = (const char *)psx_addr(format, 12);
    char rendered[20];
    int count = snprintf(rendered, sizeof(rendered), pattern, (int)(sint32)value);
    if (count < 0 || (size_t)count >= sizeof(rendered))
        abort();
    memcpy(psx_addr(destination, (uint32)count + 1), rendered, (size_t)count + 1);
    uint32 field = destination + 8;
    if (rendered[8] == ' ')
        ++field;
    G3Function80020CF4(destination, 0, 0xffffffff, field, guest_stack, 0);
}

void G3BombTimerUpdate(uint32 object, uint32 game, uint32 state, uint32 data, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000130, "G3L_63");
    uint32 frame = guest_stack - 0x80;
    bool running = true;
    G3Function80047EE4(5, (uint32)r_s16(state));
    for (uint32 index = 1; index < 5; ++index)
        G3Function80046B1C(index);
    sint32 mode = r_s16(state + 4);
    if (mode == 0 || mode == 3)
    {
        if (r_s16(state) != 0)
        {
            uint32 level = r_u32(game + 0x48);
            sint32 collected = r_s32(game + 0x6820);
            sint32 goal = r_s32(level + 0x30);
            if (collected < goal)
            {
                if (r_u32(0x800aa7d4) != 0)
                    w_u16(state + 0xc, 1);
                uint32 player = r_u32(game + 0x50);
                w_u32(player + 0x10, r_u32(player + 0x10) | 0x100);
            }
            else
                G3Function80046B00(5);
            uint32 delay = r_u16(state);
            running = false;
            w_u16(state, delay - 1);
        }
        if (r_s16(state + 0xc) != 0 && r_u32(0x800aa7d4) != 0)
            running = false;
        if ((r_u32(game + 0x6834) & 0x10) != 0)
            running = false;
        if (r_s16(game + 0x6786) == 0 && running)
        {
            uint32 intro = r_u32(object + 0x20);
            if (r_u32(intro + 0x28) == 0 && G3Function8004DACC() == 0)
            {
                if ((r_u32(game + 0x69b8) & 0x10000) == 0 && r_u32(game + 0x67c4) == 0)
                    w_u32(state + 8, r_u32(state + 8) - 1);
            }
        }
        uint32 level = r_u32(game + 0x48);
        sint32 collected = r_s32(game + 0x6820);
        sint32 goal = r_s32(level + 0x30);
        if (collected >= goal && r_u32(object + 0x154) == 0)
        {
            w_u16(state, 1);
            if (r_u32(data + 4) == 0x3f2)
            {
                uint32 sequence = r_u32(data + 8);
                uint32 player = r_u32(game + 0x50);
                G3Function80018DE0(player, sequence + 4, frame);
            }
            w_u32(object + 0x154, 1);
            uint32 player = r_u32(game + 0x50);
            w_u32(player + 0x128, r_u32(player + 0x128) & 0xffbfffff);
        }
        if (r_s16(state + 4) == 3)
        {
            w_u32(state + 8, 60);
            w_u16(state + 4, 1);
            uint32 player = r_u32(game + 0x50);
            w_u32(player + 0x10, r_u32(player + 0x10) | 0x100);
        }
        uint32 ticks = r_u32(state + 8);
        if ((sint32)ticks < 0)
        {
            mode = r_s16(state + 4);
            if (mode != 3 && mode != 1)
            {
                w_u32(game + 0x6820, 0);
                w_u32(state + 8, 60);
                w_u16(state + 4, 2);
                uint32 player = r_u32(game + 0x50);
                w_u32(player + 0x10, r_u32(player + 0x10) | 0x100);
            }
        }
        if (r_s16(state + 4) == 0 && G3Function8004DACC() == 0)
        {
            ticks = r_u32(state + 8);
            sint32 signed_ticks = (sint32)ticks;
            sint32 minutes = signed_ticks / 1800;
            sint32 seconds = (signed_ticks % 1800) / 30;
            if (signed_ticks >= 301 || signed_ticks % 15 >= 5)
                G3Function8003A150((uint32)minutes, (uint32)seconds, ticks, frame);
            uint32 current_game = r_u32(0x80097048);
            w_u32(current_game + 0x6834, r_u32(current_game + 0x6834) | 0x1000000);
            if (r_s16(state) == 0)
            {
                level = r_u32(game + 0x48);
                collected = r_s32(game + 0x6820);
                goal = r_s32(level + 0x30);
                if (collected < goal)
                {
                    G3Function80021A74(320);
                    G3Function80022E90(40, 210);
                    level = r_u32(game + 0x48);
                    uint32 progress = r_u32(game + 0x6820);
                    uint32 target = r_u32(level + 0x30);
                    G3BombTimerRemainingText(frame + 0x10, module_base + 0x70c, target - progress, frame);
                }
                else
                    G3Function80046B00(5);
            }
        }
        if (G3Function8004DACC() != 0)
            G3Function80046B00(5);
    }
    else
    {
        uint32 player = r_u32(game + 0x50);
        w_u32(player + 0x10, r_u32(player + 0x10) | 0x100);
        w_u32(game + 0x69b8, r_u32(game + 0x69b8) | 0x10000);
        bool expired;
        uint32 token;
        if (r_s16(state + 4) == 2)
        {
            G3Function80046B00(5);
            uint32 next = r_u32(state + 8) - 1;
            w_u32(state + 8, next);
            expired = (sint32)next < 0;
            if (!expired)
            {
                G3Function80021A74(450);
                token = 0x46;
            }
        }
        else
        {
            G3Function80046B00(5);
            uint32 next = r_u32(state + 8) - 1;
            w_u32(state + 8, next);
            expired = (sint32)next < 0;
            if (!expired)
            {
                G3Function80021A74(320);
                token = 0x47;
            }
        }
        if (expired)
        {
            uint32 root = r_u32(0x8009d668);
            uint32 name = r_u32(0x8009d66c);
            G3Function80027644(root, name, game, frame);
        }
        else
        {
            uint32 text = G3Function80065D38(token);
            G3Function800234A0(text, 120, 0, 0, frame, 0);
        }
    }
    G3Function80021A74(320);
}

void G3BombTimerConfigure(uint32 object, uint32 properties, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000620, "G3L_63");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0);
    uint32 result = G3Function80085CAC(frame + 0x10, properties, module_base + 0x718, 0, 11);
    uint32 value = r_u32(frame + 0x10);
    if (value != 0)
    {
        result = r_u32(state + 8) + value;
        w_u32(state + 8, result);
    }
    G3Function80085CAC(frame + 0x10, properties, module_base + 0x724, 0, result);
    value = r_u32(frame + 0x10);
    result = value << 4;
    if (value != 0)
    {
        result -= value;
        uint32 ticks = r_u32(state + 8);
        result <<= 1;
        w_u32(state + 8, ticks + result);
    }
    G3Function80085CAC(frame + 0x14, properties, module_base + 0x730, 0, result);
    if (r_u32(frame + 0x14) != 0)
    {
        uint32 game = r_u32(0x80097048);
        w_u16(state + 4, 3);
        w_u16(game + 0x69dc, r_u16(game + 0x69de));
    }
}

uint32 G3BurroEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_05");
    uint32 frame = guest_stack - 0x18;
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
    case 1: G3BurroInitialize(object, game, state, frame); return 1;
    case 2:
        if (r_s16(game + 0x67ee) == 1 && r_s16(state) == 2)
        {
            uint32 player = r_u32(game + 0x50);
            {
                uint32 source = player + 0x58;
                uint32 destination = object + 0x58;
                uint32 first_address = source;
                uint32 first_high_address = first_address + 3;
                uint32 first_high_lane = first_high_address & 3;
                uint32 first_lane = first_address & 3;
                uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
                uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
                uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
                uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
                uint32 z = (uint32)r_s16(source + 4);
                first_address = destination;
                first_high_address = first_address + 3;
                first_high_lane = first_high_address & 3;
                first_lane = first_address & 3;
                w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
                w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
                w_u16(destination + 4, z);
            }
        }
        {
            sint32 mode = r_s16(state);
            if (mode == 0 || mode == 3) G3BurroUpdate(object, game, state, settings, module_base, frame);
        }
        return 1;
    case 0x7ea: G3BurroUpdate(object, game, state, settings, module_base, frame); return 1;
    case 3: G3BurroCollide(object, game, state, settings, frame); return 1;
    case 0x7e2:
    {
        uint32 position = r_u16(object + 0x58);
        uint32 offset = r_u16(payload);
        w_u16(object + 0x58, position + offset);
        position = r_u16(object + 0x5a);
        offset = r_u16(payload + 2);
        w_u16(object + 0x5a, position + offset);
        position = r_u16(object + 0x5c);
        offset = r_u16(payload + 4);
        w_u16(object + 0x5c, position + offset);
        return 1;
    }
    case 0x7eb:
        w_u16(state, 3);
        G3BurroDismount(object, game, frame);
        return 1;
    case 0x7f1:
    case 0x7f2:
    {
        if (r_s16(state + 2) != 0) return 0;
        uint32 player = r_u32(0x8009efc4);
        sint32 z = r_s16(object + 0x5c);
        sint32 floor = r_s16(player + 0xe0);
        sint32 difference = z - floor;
        if (difference < 0) difference = -difference;
        if (difference >= 5) return 0;
        return r_s16(state + 0xa) == 0 ? 1 : 0;
    }
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

void G3BurroInitialize(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000240, "G3L_05");
    uint32 frame = guest_stack - 0x20;
    w_u32(object + 0x10, r_u32(object + 0x10) | 0x100000);
    G3Function80082988(object, 0, game, frame);
    w_u16(state, 0);
    w_u16(state + 0xe, 0);
    w_u16(state + 0x18, 0);
    w_u16(state + 0x1a, 0);
    w_u16(state + 0x1c, 0);
    w_u16(state + 6, 0);
    w_u8(state + 0x2f, 0);
    w_u8(state + 0x30, 0);
    w_u8(state + 0x31, 0);
    w_u8(state + 0x33, 0);
    w_u8(state + 0x35, 0);
    uint32 flags = r_u32(object + 0x10);
    w_u16(object + 0x6e, 0x11);
    w_u16(object + 0x5e, 0);
    w_u32(object + 0x10, flags | 0x400);
}

void G3BurroDust(uint32 position, uint32 angle, uint32 scale, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000002d0, "G3L_05");
    uint32 frame = guest_stack - 0x40;
    {
        uint32 source = module_base + 0x1df8;
        uint32 destination = frame + 0x10;
        uint32 first_address = source;
        uint32 first_high_address = first_address + 3;
        uint32 first_high_lane = first_high_address & 3;
        uint32 first_lane = first_address & 3;
        uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
        uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
        uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
        uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
        uint32 z = (uint32)r_s16(source + 4);
        first_address = destination;
        first_high_address = first_address + 3;
        first_high_lane = first_high_address & 3;
        first_lane = first_address & 3;
        w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
        w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
        w_u16(destination + 4, z);
    }
    uint32 resource = r_u32(0x80098724);
    if (resource == 0) return;
    angle = (uint32)(sint16)angle;
    uint32 product = G3Function8005B470(angle) * scale;
    uint32 x = (uint32)((sint32)product >> 12);
    product = G3Function8005B590(angle) * scale;
    uint32 y = (uint32)((sint32)product >> 12);
    uint32 random = G3SdkRandom80027FA8();
    w_u16(frame + 0x18, r_u16(position) + (x << 3) + (random & 0x1f) - 7);
    random = G3SdkRandom80027FA8();
    w_u16(frame + 0x1a, r_u16(position + 2) + (y << 3) + (random & 0x1f) - 7);
    random = G3SdkRandom80027FA8();
    uint32 z = r_u16(position + 4);
    w_u16(frame + 0x1c, z + (random & 0xf) + 5);
    uint32 table = r_u32(resource + 0xc);
    uint32 pool = r_u32(0x800a27fc);
    uint32 mesh = r_u32(table);
    uint32 effect = G3Function800444E4(frame + 0x18, mesh, pool, 0x14, frame);
    if (effect != 0)
    {
        uint32 result = G3Function800449F0(effect, x, y, 8, effect);
        G3Function80044878(effect, frame + 0x10, &result);
    }
}

uint32 G3BurroSteer(uint32 input, uint32 object, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000041c, "G3L_05");
    uint32 frame = guest_stack - 0x30;
    uint32 direction = 0;
    uint32 adjusted = 0;
    uint32 vertical = r_u32(input + 0xa8);
    if (vertical != 0 || r_u32(input + 0xa4) != 0)
    {
        uint32 horizontal = r_u32(input + 0xa4);
        direction = G3Function8002F240(0 - vertical, horizontal, frame) + 0x400;
        if ((sint16)direction == 0) direction = 0x1000;
        uint32 y = r_u32(input + 0xa8);
        uint32 square_y = y * y;
        uint32 x = r_u32(input + 0xa4);
        if ((sint32)(square_y + x * x) < 0x2400)
        {
            uint32 player = r_u32(0x8009efc4);
            w_u32(player + 0x128, r_u32(player + 0x128) | 0x20);
        }
    }
    sint32 signed_direction = (sint16)direction;
    if (signed_direction != 0) adjusted = direction + G3Function80014F00(input);
    if (r_s16(state + 2) == 5) adjusted = 0;
    if ((sint16)adjusted != 0)
    {
        uint32 step = (uint32)r_s16(settings + 4);
        G3Function80038AA8(state + 4, (uint32)(sint16)adjusted, step);
        uint32 target = (uint32)r_s16(state + 4);
        step = (uint32)r_s16(settings + 6);
        G3Function80038AA8(object + 0x64, target, step);
    }
    w_u16(state + 8, adjusted);
    return (uint32)signed_direction;
}

void G3BurroSpeed(uint32 direction, uint32 state, uint32 settings)
{
    FUNCTION_MARKER(0x0000056c, "G3L_05");
    if ((sint16)direction != 0 && r_u8(state + 0x2d) != 0)
    {
        uint32 count = (uint32)r_s16(settings + 0x10);
        uint32 table = r_u32(settings + 0x1c);
        sint32 limit = r_s16(table + count * 2 - 2) - 1;
        sint32 speed = r_s16(state + 0xa);
        uint32 bits = r_u16(state + 0xa);
        if (speed < limit) w_u16(state + 0xa, bits + 1);
    }
    else
    {
        sint32 speed = r_s16(state + 0xa);
        sint32 drag = r_s16(settings + 0x20);
        uint32 drag_bits = r_u16(settings + 0x20);
        uint32 speed_bits = r_u16(state + 0xa);
        w_u16(state + 0xa, drag < speed ? speed_bits - drag_bits : 0);
    }
}

void G3BurroAnimate(uint32 object, uint32 game, uint32 state, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000005ec, "G3L_05");
    uint32 frame = guest_stack - 0x28;
    sint32 phase = r_s16(state + 2);
    uint32 player = r_u32(game + 0x50);
    if (phase == 0)
    {
        if (r_s16(state + 0xa) != 0)
        {
            if (r_u16(object + 0x6e) == 2)
            {
                uint32 descriptor = r_u32(object + 0x1c);
                w_u16(object + 0x6e, 3);
                uint32 table = r_u32(descriptor + 0x10);
                uint32 moving = r_u32(table + 0xc);
                sint32 current_frame = r_s16(object + 0x5e);
                sint32 moving_length = r_s16(moving + 2);
                sint32 product = current_frame * moving_length;
                uint32 idle = r_u32(table + 8);
                sint32 idle_length = r_s16(idle + 2);
                sint32 quotient = idle_length != 0 ? product / idle_length : product < 0 ? 1 : -1;
                w_u16(object + 0x5e, (uint32)quotient);
                uint32 current_player = r_u32(0x8009efc4);
                w_u16(current_player + 0x6e, 0x31);
                w_u16(current_player + 0x5e, r_u16(object + 0x5e));
            }
            G3Function8002A1C8(player, 0xffffffff, frame);
            w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
            G3Function80029F0C(object, 0xffffffff, frame);
            bool restart = (r_u32(object + 0x14) & 0x10) != 0;
            if (!restart)
            {
                uint32 animation = r_u16(object + 0x6e);
                restart = animation - (uint32)10 < 6 || animation == 0x12;
            }
            if (restart)
            {
                w_u16(object + 0x6e, 0);
                w_u16(object + 0x5e, 0);
                w_u16(player + 0x6e, 0x31);
                w_u16(player + 0x5e, 0);
                w_u8(state + 0x30, 0);
            }
            uint32 player_frame = r_u16(player + 0x5e) & 7;
            uint32 random = G3SdkRandom80027FA8();
            if (player_frame == (random & 7))
            {
                uint32 angle = (uint32)(sint16)(r_u16(object + 0x64) + 0x400);
                G3BurroDust(object + 0x58, angle, 4, module_base, frame);
            }
            if (r_u8(state + 0x35) == 0)
            {
                w_u32(frame + 0x10, 0xffffffff);
                w_u32(frame + 0x14, 1);
                G3Function8008194C(0, 1, 0x3ca2, 0x2b8, frame);
                w_u8(state + 0x35, 1);
            }
        }
        else
        {
            if (r_u16(object + 0x6e) == 3)
            {
                uint32 descriptor = r_u32(object + 0x1c);
                w_u16(object + 0x6e, 2);
                uint32 table = r_u32(descriptor + 0x10);
                uint32 idle = r_u32(table + 8);
                sint32 current_frame = r_s16(object + 0x5e);
                sint32 idle_length = r_s16(idle + 2);
                sint32 product = current_frame * idle_length;
                uint32 moving = r_u32(table + 0xc);
                sint32 moving_length = r_s16(moving + 2);
                sint32 quotient = moving_length != 0 ? product / moving_length : product < 0 ? 1 : -1;
                w_u16(object + 0x5e, (uint32)quotient);
                uint32 current_player = r_u32(0x8009efc4);
                w_u16(current_player + 0x6e, 0x50);
                w_u16(current_player + 0x5e, r_u16(object + 0x5e));
            }
            G3Function8002A1C8(player, 0xffffffff, frame);
            w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
            G3Function80029F0C(object, 0xffffffff, frame);
            if ((r_u32(object + 0x14) & 0x10) != 0 || r_u16(object + 0x6e) == 0)
            {
                uint32 random = G3SdkRandom80027FA8() & 0xfff;
                uint32 animation;
                if (random < 0x199) animation = 0xb;
                else if (random < 0x332) animation = 0xc;
                else if (random < 0x4cb) animation = 0xd;
                else if (random < 0x664) animation = 0xe;
                else if (random < 0x7fd) animation = 0xf;
                else animation = 0xa;
                w_u16(object + 0x6e, animation);
                w_u16(object + 0x5e, 0);
                random = G3SdkRandom80027FA8() & 0xfff;
                animation = random < 0x333 ? 0x4e : random < 0xfff ? 0x4f : 0x45;
                w_u16(player + 0x6e, animation);
                w_u16(player + 0x5e, 0);
                w_u8(state + 0x30, 0);
            }
            if (r_u8(state + 0x35) != 0)
            {
                G3Function80081B7C(0, 0x3ca2, frame);
                w_u8(state + 0x35, 0);
            }
        }
    }
    else if (phase > 0 && phase < 3)
    {
        if (r_u8(state + 0x35) != 0)
        {
            G3Function80081B7C(0, 0x3ca2, frame);
            w_u8(state + 0x35, 0);
        }
        G3Function8002A1C8(player, 0xffffffff, frame);
        w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
        G3Function80029F0C(object, 0xffffffff, frame);
        if ((r_u32(object + 0x14) & 0x10) != 0)
        {
            w_u16(object + 0x6e, r_s16(state + 0xa) != 0 ? 5 : 8);
            w_u16(object + 0x5e, 0);
            w_u16(player + 0x6e, 0x47);
            w_u16(player + 0x5e, 0);
            w_u8(state + 0x30, 0);
        }
    }
}

void G3BurroBank(uint32 object, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000098c, "G3L_05");
    uint32 frame = guest_stack - 0x68;
    uint32 direction = (uint32)r_s16(state + 8);
    uint32 target = 0;
    if (direction != 0)
    {
        target = 0 - G3Function80038B48((uint32)r_s16(object + 0x64), direction);
        sint32 speed = r_s16(state + 0xa);
        sint32 factor = r_s16(settings + 0x2a);
        sint32 product = speed * factor;
        sint32 divisor = r_s16(settings + 0x2c);
        sint32 quotient = divisor != 0 ? product / divisor : product < 0 ? 1 : -1;
        sint32 limit = (sint16)quotient;
        if ((sint32)((uint32)quotient << 16) < (sint32)(target << 16)) target = (uint32)quotient;
        if ((sint16)target < -limit) target = 0 - (uint32)quotient;
    }
    G3Function80038AA8(state + 6, (uint32)(sint16)target, 0x20);
    G3Function8005BD00(object + 0x60, frame + 0x10);
    w_u16(frame + 0x50, 0);
    uint32 bank = r_u16(state + 6);
    w_u16(frame + 0x54, 0);
    w_u16(frame + 0x52, 0 - bank);
    G3Function8005BD00(frame + 0x50, frame + 0x30);
    MulMatrix0((MATRIX *)psx_addr(frame + 0x10, 20), (MATRIX *)psx_addr(frame + 0x30, 20), (MATRIX *)psx_addr(frame + 0x30, 20));
    G3Function800202A4(frame + 0x30, object + 0x60, 0x15, frame);
}

void G3BurroSlope(uint32 object, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000ab0, "G3L_05");
    uint32 frame = guest_stack - 0x50;
    uint32 game = r_u32(0x80097048);
    {
        uint32 source = module_base + 0x1e00;
        uint32 destination = frame + 0x30;
        uint32 first_address = source;
        uint32 first_high_address = first_address + 3;
        uint32 first_high_lane = first_high_address & 3;
        uint32 first_lane = first_address & 3;
        uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
        uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
        uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
        uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
        uint32 second_address = source + 4;
        uint32 second_high_address = second_address + 3;
        uint32 second_high_lane = second_high_address & 3;
        uint32 second_lane = second_address & 3;
        uint32 second_high = r_masked_u32(second_high_address & 0xfffffffc, (1 << (second_high_lane + 1)) - 1);
        uint32 second_low = r_masked_u32(second_address & 0xfffffffc, (15 << second_lane) & 15);
        uint32 second_keep = second_lane == 0 ? 0 : 0xffffffff << ((4 - second_lane) * 8);
        uint32 second_value = ((second_high << ((3 - second_high_lane) * 8)) & second_keep) | (second_low >> (second_lane * 8));
        first_address = destination;
        first_high_address = first_address + 3;
        first_high_lane = first_high_address & 3;
        first_lane = first_address & 3;
        w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
        w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
        second_address = destination + 4;
        second_high_address = second_address + 3;
        second_high_lane = second_high_address & 3;
        second_lane = second_address & 3;
        w_masked_u32(second_high_address & 0xfffffffc, second_value >> ((3 - second_high_lane) * 8), (1 << (second_high_lane + 1)) - 1);
        w_masked_u32(second_address & 0xfffffffc, second_value << (second_lane * 8), (15 << second_lane) & 15);
    }
    uint32 player = r_u32(game + 0x50);
    sint32 z = r_s16(object + 0x5c);
    sint32 floor = r_s16(player + 0xe0);
    sint32 difference = z - floor;
    if (difference < 0) difference = -difference;
    uint32 normal = frame + 0x30;
    if (difference <= 0x40)
    {
        game = r_u32(0x80097048);
        player = r_u32(game + 0x50);
        normal = player + 0xb4;
    }
    if (r_s16(normal + 4) < 0x200) normal = frame + 0x30;
    if (r_s16(state + 0x28) == 0)
    {
        sint32 old_x = r_s16(state + 0x18);
        sint32 new_x = r_s16(normal);
        bool different = old_x != new_x;
        if (!different)
        {
            sint32 old_y = r_s16(state + 0x1a);
            sint32 new_y = r_s16(normal + 2);
            different = old_y != new_y;
        }
        if (!different)
        {
            sint32 old_z = r_s16(state + 0x1c);
            sint32 new_z = r_s16(normal + 4);
            different = old_z != new_z;
        }
        if (different)
        {
            w_u16(state + 0x28, 1);
            {
                uint32 source = state + 0x18;
                uint32 destination = state + 0x10;
                uint32 first_address = source;
                uint32 first_high_address = first_address + 3;
                uint32 first_high_lane = first_high_address & 3;
                uint32 first_lane = first_address & 3;
                uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
                uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
                uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
                uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
                uint32 second_address = source + 4;
                uint32 second_high_address = second_address + 3;
                uint32 second_high_lane = second_high_address & 3;
                uint32 second_lane = second_address & 3;
                uint32 second_high = r_masked_u32(second_high_address & 0xfffffffc, (1 << (second_high_lane + 1)) - 1);
                uint32 second_low = r_masked_u32(second_address & 0xfffffffc, (15 << second_lane) & 15);
                uint32 second_keep = second_lane == 0 ? 0 : 0xffffffff << ((4 - second_lane) * 8);
                uint32 second_value = ((second_high << ((3 - second_high_lane) * 8)) & second_keep) | (second_low >> (second_lane * 8));
                first_address = destination;
                first_high_address = first_address + 3;
                first_high_lane = first_high_address & 3;
                first_lane = first_address & 3;
                w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
                w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
                second_address = destination + 4;
                second_high_address = second_address + 3;
                second_high_lane = second_high_address & 3;
                second_lane = second_address & 3;
                w_masked_u32(second_high_address & 0xfffffffc, second_value >> ((3 - second_high_lane) * 8), (1 << (second_high_lane + 1)) - 1);
                w_masked_u32(second_address & 0xfffffffc, second_value << (second_lane * 8), (15 << second_lane) & 15);
            }
            {
                uint32 source = normal;
                uint32 destination = state + 0x18;
                uint32 first_address = source;
                uint32 first_high_address = first_address + 3;
                uint32 first_high_lane = first_high_address & 3;
                uint32 first_lane = first_address & 3;
                uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
                uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
                uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
                uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
                uint32 second_address = source + 4;
                uint32 second_high_address = second_address + 3;
                uint32 second_high_lane = second_high_address & 3;
                uint32 second_lane = second_address & 3;
                uint32 second_high = r_masked_u32(second_high_address & 0xfffffffc, (1 << (second_high_lane + 1)) - 1);
                uint32 second_low = r_masked_u32(second_address & 0xfffffffc, (15 << second_lane) & 15);
                uint32 second_keep = second_lane == 0 ? 0 : 0xffffffff << ((4 - second_lane) * 8);
                uint32 second_value = ((second_high << ((3 - second_high_lane) * 8)) & second_keep) | (second_low >> (second_lane * 8));
                first_address = destination;
                first_high_address = first_address + 3;
                first_high_lane = first_high_address & 3;
                first_lane = first_address & 3;
                w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
                w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
                second_address = destination + 4;
                second_high_address = second_address + 3;
                second_high_lane = second_high_address & 3;
                second_lane = second_address & 3;
                w_masked_u32(second_high_address & 0xfffffffc, second_value >> ((3 - second_high_lane) * 8), (1 << (second_high_lane + 1)) - 1);
                w_masked_u32(second_address & 0xfffffffc, second_value << (second_lane * 8), (15 << second_lane) & 15);
            }
            uint32 angle = G3Function8002F540(state + 0x18, state + 0x10, frame);
            sint32 first = r_s16(state + 0x1a);
            sint32 second = r_s16(state + 0x14);
            uint32 yz = (uint32)(first * second);
            first = r_s16(state + 0x1c);
            second = r_s16(state + 0x12);
            uint32 zy = (uint32)(first * second);
            first = r_s16(state + 0x18);
            second = r_s16(state + 0x14);
            uint32 xz = (uint32)(first * second);
            first = r_s16(state + 0x1c);
            second = r_s16(state + 0x10);
            uint32 zx = (uint32)(first * second);
            first = r_s16(state + 0x18);
            second = r_s16(state + 0x12);
            uint32 xy = (uint32)(first * second);
            first = r_s16(state + 0x1a);
            second = r_s16(state + 0x10);
            uint32 yx = (uint32)(first * second);
            w_u16(state + 0x2a, angle);
            w_u16(state + 0x20, 0 - (uint32)((sint32)(yz - zy) >> 12));
            w_u16(state + 0x22, (uint32)((sint32)(xz - zx) >> 12));
            w_u16(state + 0x24, 0 - (uint32)((sint32)(xy - yx) >> 12));
        }
        if (r_s16(state + 0x28) == 0)
        {
            uint32 y = (uint32)r_s16(normal + 2);
            uint32 nz = (uint32)r_s16(normal + 4);
            uint32 pitch = G3Function8002F0C4(y, nz, frame);
            w_u16(object + 0x60, 0 - pitch);
            uint32 x = (uint32)r_s16(normal);
            uint32 root = G3Function8002F368((uint32)((sint32)(0x1000000 - x * x) >> 12), frame);
            x = (uint32)r_s16(normal);
            w_u16(object + 0x62, G3Function8002F0C4(x, root, frame));
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
    G3Function8002F630(state + 0x20, frame + 0x10, (uint32)r_s16(state + 0x28), frame);
    uint32 y = (uint32)r_s16(frame + 0x1a);
    uint32 nz = (uint32)r_s16(frame + 0x20);
    w_u16(object + 0x60, 0 - G3Function8002F0C4(y, nz, frame));
    uint32 x = (uint32)r_s16(frame + 0x14);
    uint32 root = G3Function8002F368((uint32)((sint32)(0x1000000 - x * x) >> 12), frame);
    x = (uint32)r_s16(frame + 0x14);
    w_u16(object + 0x62, G3Function8002F0C4(x, root, frame));
    sint32 current = r_s16(state + 0x28);
    limit = r_s16(state + 0x2a);
    if (current == limit) w_u16(state + 0x28, 0);
}

void G3BurroMove(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000df4, "G3L_05");
    uint32 frame = guest_stack - 0x58;
    uint32 player = r_u32(0x8009efc4);
    uint32 model = r_u32(player + 0x24);
    uint32 direction = G3BurroSteer(game, object, state, settings, frame);
    w_u16(model + 0xbc, direction);
    G3BurroSpeed((uint32)(sint16)direction, state, settings);
    if (r_u8(state + 0x2d) == 0) return;
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
    w_u16(state + 0xc, index);
    uint32 velocities = r_u32(settings + 0x14);
    uint32 offset = (uint32)((sint32)(index << 16) >> 15);
    uint32 velocity = r_u16(velocities + offset);
    w_u16(frame + 0x12, 0 - velocity);
    G3Function8002F718(frame + 0x20);
    G3Function8005BB6C((uint32)r_s16(state + 4), frame + 0x20, frame);
    G3Function8005BA5C((uint32)r_s16(object + 0x62), frame + 0x20, frame);
    G3Function8005B94C((uint32)r_s16(object + 0x60), frame + 0x20, frame);
    ApplyMatrixSV((MATRIX *)psx_addr(frame + 0x20, 20), (SVECTOR *)psx_addr(frame + 0x10, 8), (SVECTOR *)psx_addr(frame + 0x18, 6));
    uint32 position = r_u16(object + 0x58);
    uint32 delta = r_u16(frame + 0x18);
    w_u16(object + 0x58, position + delta);
    position = r_u16(object + 0x5a);
    delta = r_u16(frame + 0x1a);
    w_u16(object + 0x5a, position + delta);
    position = r_u16(object + 0x5c);
    delta = r_u16(frame + 0x1c);
    w_u16(object + 0x5c, position + delta);
    w_u16(model + 0x26e, r_u16(frame + 0x18));
    w_u16(model + 0x270, r_u16(frame + 0x1a));
    w_u16(model + 0x272, r_u16(frame + 0x1c));
}

void G3BurroFlags(uint32 game, uint32 state)
{
    FUNCTION_MARKER(0x00000f84, "G3L_05");
    uint32 player = r_u32(0x8009efc4);
    uint32 model = r_u32(player + 0x24);
    w_u8(state + 0x2d, 1);
    uint32 surface = r_u32(model + 0x140);
    uint32 slippery = 0;
    if (surface != 0)
    {
        if (r_s16(model + 0x14c) >= 0xb51) slippery = 1;
        else
        {
            uint32 type = r_u16(surface + 6);
            uint32 level = r_u32(game + 0x48);
            uint32 table = r_u32(level + 0xf0);
            if ((r_u32(table + ((type & 0x1e0) >> 1)) & 8) != 0) slippery = 1;
        }
    }
    w_u8(state + 0x2e, slippery);
}

void G3BurroSlide(uint32 object, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000ffc, "G3L_05");
    uint32 frame = guest_stack - 0x28;
    uint32 timer = r_u8(state + 0x33);
    if (timer != 0) w_u8(state + 0x33, timer - 1);
    if (r_u8(state + 0x2e) != 0 || r_u8(state + 0x33) != 0) w_u8(state + 0x31, 0);
    else
    {
        uint32 player = r_u32(0x8009efc4);
        sint32 z = r_s16(object + 0x5c);
        sint32 floor = r_s16(player + 0xe0);
        sint32 difference = z - floor;
        if (difference < 0) difference = -difference;
        if (difference >= 5) w_u8(state + 0x31, 0);
        else
        {
            player = r_u32(0x8009efc4);
            uint32 scale = r_u8(settings + 0x28) << 3;
            G3Function80036278(player + 0xb4, frame + 0x10, scale, frame);
            sint32 length = (sint32)G3Function8002FA50(frame + 0x10, frame);
            if (length >= 101)
            {
                sint32 x = r_s16(frame + 0x10);
                sint32 y = r_s16(frame + 0x12);
                sint32 nz = r_s16(frame + 0x14);
                sint32 scaled_x = x * 100 / length;
                sint32 scaled_y = y * 100 / length;
                sint32 scaled_z = nz * 100 / length;
                w_u16(frame + 0x10, (uint32)scaled_x);
                w_u16(frame + 0x12, (uint32)scaled_y);
                w_u16(frame + 0x14, (uint32)scaled_z);
            }
            uint32 position = r_u16(object + 0x58);
            uint32 delta = r_u16(frame + 0x10);
            w_u16(object + 0x58, position + delta);
            position = r_u16(object + 0x5a);
            delta = r_u16(frame + 0x12);
            w_u16(object + 0x5a, position + delta);
            position = r_u16(object + 0x5c);
            delta = r_u16(frame + 0x14);
            w_u16(object + 0x5c, position + delta);
            w_u8(state + 0x31, 1);
            w_u16(state + 0xa, 0);
        }
    }
    if (r_u8(state + 0x31) == 0 && r_u8(state + 0x32) != 0) w_u8(state + 0x33, 10);
    w_u8(state + 0x32, r_u8(state + 0x31));
}

void G3BurroClearKnockback(uint32 state)
{
    FUNCTION_MARKER(0x000011c0, "G3L_05");
    if (r_u8(state + 0x2f) != 0) w_u8(state + 0x2f, 0);
}

void G3BurroJump(uint32 object, uint32 game, uint32 state, uint32 settings)
{
    FUNCTION_MARKER(0x000011dc, "G3L_05");
    uint32 player = r_u32(0x8009efc4);
    uint32 input = (r_u32(player + 0x10) & 0x100) != 0 ? 0x800a0d28 : game + 0x70;
    if ((r_u32(input + 4) & 0x80) == 0 || r_u8(state + 0x2e) == 0 || r_s16(state + 2) != 0) return;
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
    index = (uint32)r_s16(state + 0xc);
    table = r_u32(settings + 0x18);
    speed = r_s16(state + 0xa);
    uint32 velocity = r_u16(table + index * 2);
    w_u16(state + 2, 1);
    w_u16(state + 0xe, velocity);
    w_u16(object + 0x6e, speed != 0 ? 4 : 7);
    player = r_u32(0x8009efc4);
    w_u16(object + 0x5e, 0);
    w_u16(player + 0x6e, 0x44);
    w_u16(player + 0x5e, 0);
}

void G3BurroAttack(uint32 object, uint32 game, uint32 state)
{
    FUNCTION_MARKER(0x000012f0, "G3L_05");
    uint32 player = r_u32(0x8009efc4);
    uint32 input = (r_u32(player + 0x10) & 0x100) != 0 ? 0x800a0d28 : game + 0x70;
    if (r_u8(state + 0x30) != 0 || (r_u32(input + 4) & 0x20) == 0 || r_u16(object + 0x6e) - (uint32)4 < 6) return;
    if (r_s16(state + 0xa) != 0) w_u16(object + 0x6e, 3);
    else
    {
        w_u16(object + 0x6e, 2);
        player = r_u32(0x8009efc4);
        w_u16(player + 0x6e, 0x50);
        w_u16(player + 0x5e, 0);
    }
    w_u16(object + 0x5e, 0);
    w_u8(state + 0x30, 1);
    w_u8(state + 0x34, 0);
    player = r_u32(0x8009efc4);
    w_u32(player + 0x138, r_u32(player + 0x138) + 1);
}

void G3BurroVertical(uint32 object, uint32 state, uint32 settings)
{
    FUNCTION_MARKER(0x000013c0, "G3L_05");
    sint32 phase = r_s16(state + 2);
    w_u8(state + 0x2c, 0);
    if (phase < 0 || phase > 5 || phase == 3) return;
    if (phase == 1) w_u16(state + 2, 2);
    else if (phase == 2)
    {
        uint32 player = r_u32(0x8009efc4);
        sint32 player_z = r_s16(player + 0x5c);
        sint32 floor = r_s16(player + 0xe0);
        uint32 floor_bits = r_u16(player + 0xe0);
        if (player_z - 64 < floor && floor < player_z + 256)
        {
            w_u16(object + 0x5c, floor_bits);
            sint32 speed = r_s16(state + 0xa);
            w_u16(state + 0xe, 0);
            w_u16(state + 2, 0);
            w_u16(object + 0x6e, speed != 0 ? 6 : 9);
            player = r_u32(0x8009efc4);
            w_u16(object + 0x5e, 0);
            w_u16(player + 0x6e, 0x48);
            w_u16(player + 0x5e, 0);
        }
    }
    else
    {
        if ((phase == 4 || phase == 5) && (r_u32(object + 0x14) & 0x10) != 0) w_u16(state + 2, 0);
        uint32 player = r_u32(0x8009efc4);
        sint32 player_z = r_s16(player + 0x5c);
        sint32 floor = r_s16(player + 0xe0);
        if (player_z - 64 < floor && floor < player_z + 256)
        {
            sint32 z = r_s16(object + 0x5c);
            sint32 difference = z - floor;
            if (difference < 0) difference = -difference;
            if (difference >= 5)
            {
                player = r_u32(0x8009efc4);
                w_u16(object + 0x5c, r_u16(player + 0xe0));
            }
            w_u16(state + 0xe, 0);
            return;
        }
    }
    uint32 z = r_u16(object + 0x5c);
    uint32 velocity = r_u16(state + 0xe);
    w_u16(object + 0x5c, z + velocity);
    velocity = r_u16(state + 0xe);
    uint32 gravity = r_u16(settings + 0xc);
    w_u16(state + 0xe, velocity + gravity);
}

void G3BurroDismount(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001538, "G3L_05");
    uint32 frame = guest_stack - 0x20;
    uint32 player = r_u32(game + 0x50);
    uint32 angle = (uint32)r_s16(player + 0x64);
    uint32 model = r_u32(player + 0x24);
    uint32 sine = G3Function8005B470(angle - 0x400);
    angle = (uint32)r_s16(player + 0x64);
    w_u32(player + 0x104, (uint32)((sint32)sine >> 5));
    uint32 cosine = G3Function8005B590(angle - 0x400);
    w_u32(player + 0x108, (uint32)((sint32)cosine >> 5));
    w_u32(player + 0x10c, 0x96);
    uint32 flags = r_u16(model + 0xce);
    w_u32(model + 0x1fc, 0);
    w_u16(model + 0xce, flags & 0xfffe);
    w_u16(object + 0x6e, 1);
    w_u16(object + 0x5e, 0);
}

void G3BurroRide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000015bc, "G3L_05");
    uint32 frame = guest_stack - 0x30;
    uint32 player = r_u32(game + 0x50);
    uint32 input = (r_u32(player + 0x10) & 0x100) != 0 ? 0x800a0d28 : game + 0x70;
    G3BurroFlags(game, state);
    G3BurroVertical(object, state, settings);
    G3BurroSlide(object, state, settings, frame);
    if (r_u8(state + 0x31) == 0) G3BurroMove(object, game, state, settings, frame);
    G3BurroSlope(object, state, settings, module_base, frame);
    G3BurroAnimate(object, game, state, module_base, frame);
    if (r_u8(state + 0x31) == 0)
    {
        G3BurroJump(object, game, state, settings);
        G3BurroAttack(object, game, state);
    }
    G3BurroClearKnockback(state);
    if (r_u8(settings + 0x26) != 0)
    {
        sint32 z = r_s16(object + 0x5c);
        sint32 floor = r_s16(player + 0xe0);
        sint32 difference = z - floor;
        if (difference < 0) difference = -difference;
        if (difference < 5 && r_u8(state + 0x2e) != 0 && r_s16(state + 2) == 0 && r_u16(game + 0x68b4) - (uint32)0xb >= 5)
        {
            uint32 buttons = r_u32(input + 4);
            uint32 model = r_u32(player + 0x24);
            if ((buttons & 0x40) != 0 && r_u32(model + 0x13c) == 3)
            {
                uint32 current_player = r_u32(0x8009efc4);
                if (r_s16(current_player + 0xb8) >= 0xfaf && r_u8(state + 0x31) == 0)
                {
                    w_u16(state, 3);
                    w_u8(state + 0x35, 0);
                }
            }
        }
    }
    w_u16(player + 0x58, r_u16(object + 0x58));
    w_u16(player + 0x5a, r_u16(object + 0x5a));
    w_u16(player + 0x5c, r_u16(object + 0x5c));
    if (r_s16(state) == 3)
    {
        G3BurroDismount(object, game, frame);
        G3Function80081B7C(0, 0x3ca2, frame);
    }
}

void G3BurroUpdate(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001824, "G3L_05");
    uint32 frame = guest_stack - 0x20;
    uint32 player = r_u32(0x8009efc4);
    if (r_u32(player + 0x150) - 4 >= 2)
    {
        uint32 terrain = r_u32(game + 0x48);
        sint32 z = r_s16(player + 0x5c);
        sint32 threshold = r_s32(terrain + 0x6c);
        if (z < threshold)
        {
            G3Function80074AB0(player, game, frame);
            return;
        }
    }
    w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
    switch (r_s16(state))
    {
    case 0:
    case 4:
        if (r_u8(settings + 8) != 0)
        {
            G3Function80029F0C(object, 0xffffffff, frame);
            if ((r_u32(object + 0x14) & 0x10) != 0)
            {
                w_u16(object + 0x6e, 0x11);
                w_u16(object + 0x5e, 0);
            }
        }
        break;
    case 2:
        player = r_u32(0x8009efc4);
        w_u32(object + 0xf8, r_u32(player + 0xf8));
        {
            uint32 source = player + 0xfc;
            uint32 destination = object + 0xfc;
            uint32 first_address = source;
            uint32 first_high_address = first_address + 3;
            uint32 first_high_lane = first_high_address & 3;
            uint32 first_lane = first_address & 3;
            uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
            uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
            uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
            uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
            uint32 second_address = source + 4;
            uint32 second_high_address = second_address + 3;
            uint32 second_high_lane = second_high_address & 3;
            uint32 second_lane = second_address & 3;
            uint32 second_high = r_masked_u32(second_high_address & 0xfffffffc, (1 << (second_high_lane + 1)) - 1);
            uint32 second_low = r_masked_u32(second_address & 0xfffffffc, (15 << second_lane) & 15);
            uint32 second_keep = second_lane == 0 ? 0 : 0xffffffff << ((4 - second_lane) * 8);
            uint32 second_value = ((second_high << ((3 - second_high_lane) * 8)) & second_keep) | (second_low >> (second_lane * 8));
            first_address = destination;
            first_high_address = first_address + 3;
            first_high_lane = first_high_address & 3;
            first_lane = first_address & 3;
            w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
            w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
            second_address = destination + 4;
            second_high_address = second_address + 3;
            second_high_lane = second_high_address & 3;
            second_lane = second_address & 3;
            w_masked_u32(second_high_address & 0xfffffffc, second_value >> ((3 - second_high_lane) * 8), (1 << (second_high_lane + 1)) - 1);
            w_masked_u32(second_address & 0xfffffffc, second_value << (second_lane * 8), (15 << second_lane) & 15);
        }
        G3BurroRide(object, game, state, settings, module_base, frame);
        G3Function800358C0(0x7a, 0x258, frame);
        break;
    case 3:
    {
        uint32 descriptor = r_u32(object + 0x1c);
        if (r_u32(descriptor + 0x10) == 0) w_u16(state, 0);
        else
        {
            G3Function80029F0C(object, 0xffffffff, frame);
            if ((r_u32(object + 0x14) & 0x10) != 0)
            {
                player = r_u32(0x8009efc4);
                w_u16(object + 0x6e, 0x10);
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
    }
    if (r_u8(settings + 9) != 0) G3BurroBank(object, state, settings, frame);
}

void G3BurroCollide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001a00, "G3L_05");
    uint32 frame = guest_stack - 0x60;
    uint32 contact = r_u32(object + 0xb0);
    uint32 player = r_u32(0x8009efc4);
    sint32 mode = r_s16(state);
    uint32 other = r_u32(contact + 0x14);
    uint32 model = r_u32(player + 0x24);
    uint32 delta = contact + 0x28;
    if (mode == 0)
    {
        if (other != player || r_s16(contact + 6) != 1) return;
        uint32 descriptor = r_u32(contact + 0xc);
        if (r_u8(descriptor + 4) != 0 || r_u32(model + 0x1fc) != 0) return;
        G3Function8006F50C(object, game, state, settings, frame);
        if (r_u32(model + 0x1fc) != object) return;
        w_u16(state, 2);
        uint32 heading = r_u16(object + 0x64);
        w_u16(object + 0x6e, 0x12);
        w_u16(object + 0x5e, 0);
        w_u16(state + 0xa, 0);
        w_u16(state + 2, 0);
        w_u16(state + 4, heading);
        w_u8(model + 0x200, r_u8(settings + 0xb));
        player = r_u32(0x8009efc4);
        w_u16(player + 0x6e, 0x45);
        w_u16(player + 0x5e, 0);
        return;
    }
    if (mode != 2 || other == player) return;
    if (other != 0)
    {
        G3Function8003C548(contact, frame + 0x30, frame + 0x31);
        uint32 first = r_u8(frame + 0x30);
        if (first - 6 < 4)
        {
            uint32 second = r_u8(frame + 0x31);
            if ((second == 9 || second - 3 < 3) && r_u8(state + 0x30) != 0)
            {
                player = r_u32(0x8009efc4);
                w_u32(frame + 0x18, 1);
                w_u16(frame + 0x28, 0);
                w_u16(frame + 0x2a, 0);
                w_u32(frame + 0x2c, contact);
                w_u32(contact + 0x10, player);
                uint32 target = r_u32(other + 0xec);
                if (target != 0) G3DispatchStateEvent(target, other, game, 8, frame + 0x18, frame);
                if (r_u8(state + 0x34) == 0)
                {
                    G3Function8003F128(0x810, 0);
                    player = r_u32(0x8009efc4);
                    uint32 part = r_u32(object + 0x30) + 0x1c0;
                    uint32 pool = r_u32(0x800a27fc);
                    uint32 z = r_u32(part + 0x1c);
                    w_u32(frame + 0x10, z);
                    uint32 x = r_u32(part + 0x14);
                    uint32 y = r_u32(part + 0x18);
                    G3Function800440D0(player, pool, x, y, frame);
                    w_u8(state + 0x34, 1);
                    w_u32(frame + 0x10, 6);
                    w_u32(frame + 0x14, 0);
                    G3Function8008194C(0, 1, 0xffffffff, 0x333, frame);
                }
            }
        }
        uint32 second = r_u8(frame + 0x31);
        if (second - 6 < 4)
        {
            first = r_u8(frame + 0x30);
            if ((first == 9 || first - 3 < 3) && r_u8(state + 0x2f) == 0 && r_s16(state + 2) != 5 && r_s16(model + 0x120) == 0)
            {
                w_u16(model + 0x120, 0x3c);
                G3Function80073D44(game, frame);
                w_u16(state + 0xa, 0);
                w_u8(state + 0x2f, 1);
                G3Function800358C0(8, 5, frame);
                w_u32(frame + 0x10, 0x12);
                w_u32(frame + 0x14, 0);
                G3Function8008194C(0, 1, 0xffffffff, 0x1000, frame);
                G3Function8003F128(0x813, 0);
            }
        }
    }
    if (r_s16(contact + 4) == 1)
    {
        uint32 descriptor = r_u32(contact + 8);
        if (r_u8(descriptor + 5) == 8) return;
    }
    sint32 kind = r_s16(contact + 6);
    uint32 descriptor = r_u32(contact + 0xc);
    bool separate = false;
    if (kind == 1) separate = (r_u32(descriptor) & 0x2000) != 0;
    else if (kind == 5) separate = (r_u16(descriptor) & 0x2000) != 0;
    else if (kind == 2) separate = (r_u16(descriptor + 6) & 0xf) != 8;
    else if (kind == 3) separate = true;
    if (!separate) return;
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
    player = r_u32(0x8009efc4);
    offset = r_u16(delta);
    position = r_u16(player + 0x58);
    w_u16(player + 0x58, position + offset);
    position = r_u16(player + 0x5a);
    offset = r_u16(delta + 2);
    w_u16(player + 0x5a, position + offset);
    position = r_u16(player + 0x5c);
    offset = r_u16(delta + 4);
    w_u16(player + 0x5c, position + offset);
    G3Function8005F3A8(player, delta);
}
