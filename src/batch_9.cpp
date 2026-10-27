#include "psx.h"
#include "xport.h"
#include "game_functions.h"

uint32 G3Function8006218C(uint32 end, uint32 start, uint32 collision, uint32 normal, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006218c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    uint32 count = r_u32(collision + 0x10);
    uint32 box = r_u32(collision + 0x14);
    uint32 limit = box + count * 28;
    uint32 hit = 0;
    while (box < limit)
    {
        if ((r_u16(box) & 0x2000) != 0)
        {
            uint32 type = r_u8(box + 3);
            if (type == 9 || type - 3 < 3 || type < 3)
            {
                w_u32(frame + 0x10, end);
                w_u32(frame + 0x14, start);
                w_u32(frame + 0x18, box);
                if (G3Function8005F9F4(frame + 0x20, frame + 0x30, frame + 0x28, frame + 0x38, frame) != 0)
                {
                    w_u16(end, r_u16(frame + 0x20));
                    w_u16(end + 2, r_u16(frame + 0x22));
                    w_u16(end + 4, r_u16(frame + 0x24));
                    w_u16(normal, r_u16(frame + 0x30));
                    w_u16(normal + 2, r_u16(frame + 0x32));
                    uint32 last_normal = r_u16(frame + 0x34);
                    hit = box;
                    w_u16(normal + 4, last_normal);
                }
            }
        }
        box += 0x1c;
    }
    return hit;
}

uint32 G3Function80061EE0(uint32 end, uint32 start, uint32 collision, uint32 model, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80061ee0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    w_u32(frame + 0x54, model);
    w_u32(frame + 0x18, 0);
    uint32 count = r_u32(collision);
    uint32 face = r_u32(collision + 4);
    uint32 limit = face + count * 12;
    w_u32(frame + 0x1c, limit);
    if (face >= limit)
        return r_u32(frame + 0x18);
    do
    {
        model = r_u32(frame + 0x54);
        sint32 normal_index = r_s16(face + 8);
        uint32 normals = r_u32(model + 0x10);
        G3Function8005F304(normal_index, normals, 0x1f800060, frame);
        model = r_u32(frame + 0x54);
        sint32 first_index = r_s16(face);
        uint32 vertices = r_u32(model + 8);
        uint32 first = vertices + ((uint32)first_index << 3);
        uint32 endpoint_x = r_u16(end);
        uint32 vertex_x = r_u16(first);
        sint32 second_index = r_s16(face + 2);
        w_u16(0x1f800040, endpoint_x - vertex_x);
        for (uint32 axis = 1; axis < 3; ++axis)
        {
            uint32 offset = axis << 1;
            uint32 endpoint = r_u16(end + offset);
            uint32 vertex = r_u16(first + offset);
            w_u16(0x1f800040 + offset, endpoint - vertex);
        }
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 offset = axis << 1;
            uint32 endpoint = r_u16(start + offset);
            uint32 vertex = r_u16(first + offset);
            w_u16(0x1f800046 + offset, endpoint - vertex);
        }
        w_u16(0x1f80004c, r_u16(first));
        w_u16(0x1f80004e, r_u16(first + 2));
        uint32 last_vertex = r_u16(first + 4);
        uint32 second = vertices + ((uint32)second_index << 3);
        w_u16(0x1f800050, last_vertex);
        SetRotMatrix((MATRIX *)psx_addr(0x1f800040, 20));
        xport_gte_write_data(0, r_u32(0x1f800060));
        xport_gte_write_data(1, r_u32(0x1f800064));
        xport_gte_execute(0x486012);
        w_u32(0x1f800068, xport_gte_read_data(25));
        w_u32(0x1f80006c, xport_gte_read_data(26));
        w_u32(0x1f800070, xport_gte_read_data(27));
        sint32 end_distance = r_s32(0x1f800068);
        if (end_distance < 0)
        {
            sint32 start_distance = r_s32(0x1f80006c);
            if (end_distance < start_distance && start_distance >= 0)
            {
                uint32 game = r_u32(0x80097048);
                uint32 plane = r_u32(0x1f800070);
                w_u32(frame + 0x10, plane);
                uint32 target = r_u32(game + 0x65e8);
                G3DispatchLinePlane(target, 0x1f800074, start, end, 0x1f800060, frame);
                model = r_u32(frame + 0x54);
                sint32 third_index = r_s16(face + 4);
                vertices = r_u32(model + 8);
                w_u32(frame + 0x10, 0x1f800060);
                uint32 third = vertices + ((uint32)third_index << 3);
                if (G3Function8005F164(first, second, third, 0x1f800074, frame) != 0)
                {
                    uint32 material = r_u16(face + 6);
                    if ((material & 0xf) != 8)
                    {
                        w_u32(end, r_u32(0x1f800074));
                        w_u32(end + 4, r_u32(0x1f800078));
                        uint32 x = r_u16(0x1f800060);
                        uint32 output = r_u32(frame + 0x58);
                        w_u16(output, x);
                        w_u16(output + 2, r_u16(0x1f800062));
                        uint32 z = r_u16(0x1f800064);
                        w_u32(frame + 0x18, face);
                        w_u16(output + 4, z);
                    }
                }
            }
        }
        limit = r_u32(frame + 0x1c);
        face += 12;
    } while (face < limit);
    return r_u32(frame + 0x18);
}

uint32 G3Function8005F9F4(uint32 point, uint32 normal, uint32 alternate_point, uint32 alternate_normal, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005f9f4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    uint32 end = r_u32(guest_stack + 0x10);
    uint32 start = r_u32(guest_stack + 0x14);
    uint32 box = r_u32(guest_stack + 0x18);
    w_u32(0x800a25c0, normal);
    sint32 end_x = r_s16(end);
    sint32 start_x = r_s16(start);
    sint32 dx = end_x - start_x;
    w_u32(frame + 0x28, (uint32)dx);
    sint32 end_y = r_s16(end + 2);
    sint32 start_y = r_s16(start + 2);
    w_u32(0x800a25b8, 0x1001);
    w_u32(0x800a25bc, 0x1001);
    w_u32(0x800a25b0, point);
    w_u32(0x800a25b4, alternate_point);
    w_u32(0x800a25c4, alternate_normal);
    w_u32(frame + 0x2c, (uint32)(end_y - start_y));
    sint32 end_z = r_s16(end + 4);
    sint32 start_z = r_s16(start + 4);
    w_u16(frame + 0x20, 0xf000);
    w_u16(frame + 0x22, 0);
    w_u16(frame + 0x24, 0);
    w_u32(frame + 0x30, (uint32)(end_z - start_z));
    uint32 coordinate = r_u16(start);
    uint32 plane = r_u16(box + 4);
    w_u32(frame + 0x10, frame + 0x28);
    w_u32(frame + 0x14, box);
    w_u32(frame + 0x18, 0x8005f7d4);
    w_u32(frame + 0x1c, frame + 0x20);
    G3Function8005F8E8((sint16)(0 - coordinate), -dx, (sint16)(0 - plane), start, frame);
    for (uint32 phase = 1; phase < 6; ++phase)
    {
        uint32 axis = phase >> 1;
        bool negative = (phase & 1) == 0;
        uint32 value = negative ? 0xf000 : 0x1000;
        w_u16(frame + 0x20, axis == 0 ? value : 0);
        w_u16(frame + 0x22, axis == 1 ? value : 0);
        w_u16(frame + 0x24, axis == 2 ? value : 0);
        uint32 callback = axis == 0 ? 0x8005f7d4 : axis == 1 ? 0x8005f830 : 0x8005f88c;
        if (negative)
        {
            coordinate = r_u16(start + (axis << 1));
            uint32 delta = r_u16(frame + 0x28 + (axis << 2));
            plane = r_u16(box + 4 + (axis << 1));
            w_u32(frame + 0x10, frame + 0x28);
            w_u32(frame + 0x14, box);
            w_u32(frame + 0x18, callback);
            w_u32(frame + 0x1c, frame + 0x20);
            G3Function8005F8E8((sint16)(0 - coordinate), (sint16)(0 - delta), (sint16)(0 - plane), start, frame);
        }
        else
        {
            sint32 signed_coordinate = r_s16(start + (axis << 1));
            sint32 signed_plane = r_s16(box + 0xa + (axis << 1));
            w_u32(frame + 0x10, frame + 0x28);
            w_u32(frame + 0x14, box);
            w_u32(frame + 0x18, callback);
            w_u32(frame + 0x1c, frame + 0x20);
            sint32 delta = r_s32(frame + 0x28 + (axis << 2));
            G3Function8005F8E8(signed_coordinate, delta, signed_plane, start, frame);
        }
    }
    if (r_u32(0x800a25bc) != 0x1001)
        return 2;
    return r_u32(0x800a25b8) != 0x1001;
}

uint32 G3Function8005F7D4(uint32 point, uint32 box)
{
    FUNCTION_MARKER(0x8005f7d4, "SLUS_008.06");
    sint32 first = r_s16(point + 2);
    sint32 low = r_s16(box + 6);
    if (first < low)
        return 0;
    sint32 high = r_s16(box + 12);
    if (high < first)
        return 0;
    sint32 second = r_s16(point + 4);
    low = r_s16(box + 8);
    if (second < low)
        return 0;
    high = r_s16(box + 14);
    return second <= high;
}

uint32 G3Function8005F830(uint32 point, uint32 box)
{
    FUNCTION_MARKER(0x8005f830, "SLUS_008.06");
    sint32 first = r_s16(point + 0);
    sint32 low = r_s16(box + 4);
    if (first < low)
        return 0;
    sint32 high = r_s16(box + 10);
    if (high < first)
        return 0;
    sint32 second = r_s16(point + 4);
    low = r_s16(box + 8);
    if (second < low)
        return 0;
    high = r_s16(box + 14);
    return second <= high;
}

uint32 G3Function8005F88C(uint32 point, uint32 box)
{
    FUNCTION_MARKER(0x8005f88c, "SLUS_008.06");
    sint32 first = r_s16(point + 0);
    sint32 low = r_s16(box + 4);
    if (first < low)
        return 0;
    sint32 high = r_s16(box + 10);
    if (high < first)
        return 0;
    sint32 second = r_s16(point + 2);
    low = r_s16(box + 6);
    if (second < low)
        return 0;
    high = r_s16(box + 12);
    return second <= high;
}

uint32 G3Function8005F694(uint32 point, uint32 normal, uint32 time, uint32 first_point, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005f694, "SLUS_008.06");
    uint32 first_time = r_u32(guest_stack + 0x14);
    sint32 candidate = r_s32(time);
    uint32 first_normal = r_u32(guest_stack + 0x10);
    uint32 second_point = r_u32(guest_stack + 0x18);
    uint32 second_normal = r_u32(guest_stack + 0x1c);
    sint32 earliest = r_s32(first_time);
    uint32 second_time = r_u32(guest_stack + 0x20);
    if (candidate < earliest)
    {
        for (uint32 axis = 0; axis < 3; ++axis)
            w_u16(second_point + (axis << 1), r_u16(first_point + (axis << 1)));
        for (uint32 axis = 0; axis < 3; ++axis)
            w_u16(second_normal + (axis << 1), r_u16(first_normal + (axis << 1)));
        w_u32(second_time, r_u32(first_time));
        for (uint32 axis = 0; axis < 3; ++axis)
            w_u16(first_point + (axis << 1), r_u16(point + (axis << 1)));
        for (uint32 axis = 0; axis < 3; ++axis)
            w_u16(first_normal + (axis << 1), r_u16(normal + (axis << 1)));
        uint32 result = r_u32(time);
        w_u32(first_time, result);
        return result;
    }
    sint32 next = r_s32(second_time);
    if (candidate < next)
    {
        for (uint32 axis = 0; axis < 3; ++axis)
            w_u16(second_point + (axis << 1), r_u16(point + (axis << 1)));
        for (uint32 axis = 0; axis < 3; ++axis)
            w_u16(second_normal + (axis << 1), r_u16(normal + (axis << 1)));
        uint32 result = r_u32(time);
        w_u32(second_time, result);
        return result;
    }
    return 0;
}

void G3Function8005F8E8(sint32 start_coordinate, sint32 delta, sint32 plane, uint32 start, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005f8e8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 differences = r_u32(guest_stack + 0x10);
    uint32 callback = r_u32(guest_stack + 0x18);
    if (delta == 0)
        return;
    sint32 first = (sint16)start_coordinate;
    sint32 boundary = (sint16)plane;
    uint32 numerator_word = 0 - ((uint32)(first - boundary) << 12);
    sint32 numerator = (sint32)numerator_word;
    uint32 fraction = (uint32)(numerator / delta);
    w_u32(frame + 0x30, fraction);
    if (fraction >= 0x1001)
        return;
    for (uint32 axis = 0; axis < 2; ++axis)
    {
        uint32 difference = r_u32(differences + (axis << 2));
        uint32 product = difference * fraction;
        uint32 coordinate = r_u16(start + (axis << 1));
        sint32 displacement = (sint32)product >> 12;
        w_u16(frame + 0x28 + (axis << 1), coordinate + (uint32)displacement);
    }
    uint32 difference = r_u32(differences + 8);
    uint32 product = difference * fraction;
    uint32 box = r_u32(guest_stack + 0x14);
    uint32 coordinate = r_u16(start + 4);
    sint32 displacement = (sint32)product >> 12;
    w_u16(frame + 0x2c, coordinate + (uint32)displacement);
    if (G3DispatchBoxPlaneCheck(callback, frame + 0x28, box, frame) == 0)
        return;
    uint32 normal = r_u32(guest_stack + 0x1c);
    uint32 first_normal = r_u32(0x800a25c0);
    uint32 second_point = r_u32(0x800a25b4);
    uint32 second_normal = r_u32(0x800a25c4);
    w_u32(frame + 0x14, 0x800a25b8);
    w_u32(frame + 0x1c, second_normal);
    uint32 first_point = r_u32(0x800a25b0);
    w_u32(frame + 0x20, 0x800a25bc);
    w_u32(frame + 0x18, second_point);
    w_u32(frame + 0x10, first_normal);
    G3Function8005F694(frame + 0x28, normal, frame + 0x30, first_point, frame);
}

void G3Function8003BA9C(uint32 object, uint32 commands, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003ba9c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    w_u32(frame + 0x2c, commands);
    if (commands == 0)
        return;
    uint32 word = r_u32(commands);
    uint32 terminator = r_u32(0x8009dd74);
    while (true)
    {
        if (word == terminator)
        {
            uint32 tail = r_u32(commands + 4);
            uint32 end_tail = r_u32(0x8009dd78);
            if (tail == end_tail)
                return;
        }
        sint32 selector = -1;
        uint32 stride = 2;
        bool recognized = false;
        bool reload_cursor = true;
        uint32 data = 0;
        for (uint32 command = 0; command < 31; ++command)
        {
            if (reload_cursor)
                commands = r_u32(frame + 0x2c);
            uint32 name = 0x8009dd80 + command * 12;
            uint32 expected = r_u32(name);
            uint32 actual = r_u32(commands);
            if (command == 0)
                data = commands + 8;
            bool first_matches = actual == expected;
            if (first_matches)
            {
                uint32 actual_tail = r_u32(commands + 4);
                uint32 expected_tail = r_u32(name + 4);
                recognized = actual_tail == expected_tail;
            }
            if (!recognized)
            {
                uint32 next = command + 1;
                reload_cursor = !first_matches || (next <= 10 ? (next & 1) == 0 : (next & 1) != 0);
                continue;
            }
            switch (command)
            {
            case 0:
                w_u8(state + 0x11, r_u8(commands + 8));
                stride = 3;
                break;
            case 1:
            {
                sint32 value = r_s16(data);
                if (value >= 800)
                    value = 800;
                else if (value < 13)
                    value = 13;
                sint32 scale = (value * 4096) / 100;
                stride = 3;
                w_u16(object + 0x6c, (uint32)scale);
                w_u16(object + 0x6a, (uint32)scale);
                w_u16(object + 0x68, (uint32)scale);
                break;
            }
            case 2:
                w_u16(state + 0x12, r_u16(state + 0x12) | 8);
                break;
            case 3:
                w_u32(object + 0x10, r_u32(object + 0x10) | 0x80);
                w_u16(state + 0x12, r_u16(state + 0x12) | 0x402);
                break;
            case 4:
                w_u16(state + 0x12, r_u16(state + 0x12) | 0x40);
                break;
            case 5:
                w_u32(object + 0x18, r_u32(object + 0x18) | 2);
                break;
            case 6:
                G3Function800370E8(object, 0x1f000, frame);
                break;
            case 7:
                G3Function80037150(object, 0x1f000, frame);
                break;
            case 8:
            {
                sint32 collision_selector = r_s8(state + 0x11);
                G3Function80038798(object, (uint32)collision_selector, 0, frame);
                return;
            }
            case 9:
                for (uint32 axis = 0; axis < 3; ++axis)
                {
                    uint32 offset = axis << 1;
                    uint32 position = r_u16(object + 0x58 + offset);
                    uint32 delta = r_u16(data + offset);
                    w_u16(object + 0x58 + offset, position + delta);
                }
                break;
            case 10:
            case 11:
            {
                stride = 3;
                uint32 flags = r_u16(state + 0x12);
                uint32 value = r_u32(data);
                flags = command == 10 ? flags & 0xfffb : flags | 4;
                w_u32(state + 0x14, value);
                w_u16(state + 0x12, flags);
                break;
            }
            case 12:
                w_u16(state + 0x12, r_u16(state + 0x12) | 0x20);
                break;
            case 13:
                w_u16(state + 0x12, r_u16(state + 0x12) | 0x10);
                break;
            case 14:
                selector = r_u16(data);
                stride = 3;
                break;
            case 15:
            {
                uint32 flags = r_u32(object + 0x10);
                selector = r_u16(data);
                w_u32(object + 0x10, flags | 0x80);
                w_u16(state + 0x12, r_u16(state + 0x12) & 0xfffd);
                stride = 3;
                break;
            }
            case 16:
                w_u32(object + 0x10, r_u32(object + 0x10) | 0x80);
                w_u16(state + 0x12, r_u16(state + 0x12) | 2);
                break;
            case 17:
                w_u32(object + 0x10, r_u32(object + 0x10) | 0x80);
                w_u16(state + 0x12, r_u16(state + 0x12) & 0xfffd);
                break;
            case 18:
                w_u32(object + 0x10, r_u32(object + 0x10) | 0x80);
                w_u16(state + 0x12, r_u16(state + 0x12) ^ 2);
                break;
            case 19:
            {
                w_u32(object + 0x10, r_u32(object + 0x10) | 0x80);
                w_u32(state + 0x20, 0);
                uint32 first = r_u8(data);
                stride = 3;
                w_u8(state + 0x18, first);
                uint32 second = r_u8(data + 2);
                uint32 flags = r_u16(state + 0x12);
                uint32 stored = r_u8(state + 0x18);
                selector = (sint8)stored;
                w_u16(state + 0x12, flags | 2);
                w_u8(state + 0x19, second);
                break;
            }
            case 20:
            {
                uint32 table = r_u32(data);
                w_u32(state + 0x20, table);
                if (table != 0)
                {
                    w_u32(object + 0x10, r_u32(object + 0x10) | 0x80);
                    table = r_u32(state + 0x20);
                    uint32 flags = r_u16(state + 0x12);
                    w_u16(state + 0x1e, 0);
                    selector = r_u16(table);
                    w_u16(state + 0x12, flags | 2);
                }
                stride = 3;
                break;
            }
            case 21:
            {
                uint32 animation = r_u8(data);
                w_u16(object + 0x5e, 0);
                w_u16(object + 0x6e, animation);
                uint32 flags = r_u16(state + 0x12);
                stride = 3;
                w_u32(state + 0x34, 0);
                w_u16(state + 0x12, flags | 1);
                break;
            }
            case 22:
            {
                w_u16(object + 0x5e, r_u8(data));
                uint32 flags = r_u16(state + 0x12);
                stride = 3;
                w_u32(state + 0x34, 0);
                w_u16(state + 0x12, flags & 0xfffe);
                break;
            }
            case 23:
                w_u16(state + 0x12, r_u16(state + 0x12) & 0xfffe);
                break;
            case 24:
                w_u16(state + 0x12, r_u16(state + 0x12) | 1);
                break;
            case 25:
                w_u16(state + 0x12, r_u16(state + 0x12) & 1);
                break;
            case 26:
                w_u32(state + 0x2c, r_u32(data));
                stride = 3;
                break;
            case 27:
                w_u16(object + 0xa2, r_u16(data));
                stride = 3;
                break;
            case 28:
            {
                uint32 table = r_u32(data);
                w_u32(state + 0x34, table);
                if (table != 0)
                    w_u16(state + 0x30, 0);
                stride = 3;
                break;
            }
            case 29:
                w_u16(state + 0x12, r_u16(state + 0x12) | 0x200);
                break;
            case 30:
            {
                uint32 flags = r_u16(state + 0x12);
                stride = 20;
                w_u32(state + 0x28, data);
                w_u16(state + 0x12, flags | 0x80);
                break;
            }
            }
            break;
        }
        sint32 selected = (sint16)selector;
        if (selected >= 0)
            G3Function8003B030(object, (uint32)selected);
        if (recognized)
        {
            uint32 cursor = r_u32(frame + 0x2c);
            w_u32(frame + 0x2c, cursor + (stride << 2));
        }
        else if (G3Function80085B84(frame + 0x2c, frame) == 0)
            return;
        commands = r_u32(frame + 0x2c);
        terminator = r_u32(0x8009dd74);
        word = r_u32(commands);
    }
}

void G3Function8003C4F8(uint32 object, uint32 game, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003c4f8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (payload != 0)
        G3Function8003BA9C(object, payload, frame);
}

uint32 G3Function8003B030(uint32 object, uint32 selector)
{
    FUNCTION_MARKER(0x8003b030, "SLUS_008.06");
    uint32 descriptor = r_u32(object + 0x1c);
    sint32 model_index = r_s16(object + 0xa2);
    uint32 models = r_u32(descriptor + 0xc);
    uint32 model = r_u32(models + ((uint32)model_index << 2));
    uint32 table = r_u32(model + 0x1c);
    if (table == 0)
        return 0;
    uint32 count = r_u32(table + 8);
    sint32 maximum = (sint32)(count - 1);
    sint32 selected = (sint16)selector;
    bool use_last = selected > 0 ? selected >= maximum : maximum <= 0;
    if (use_last)
    {
        uint32 last = r_u16(table + 8);
        selected = (sint16)(last - 1);
    }
    else if (selected < 0)
        selected = 0;
    uint32 element_size = r_u32(table + 0xc);
    uint32 offset = (uint32)selected * element_size;
    w_u16(object + 0x9e, offset);
    return (uint32)selected;
}

uint32 G3Function800370E8(uint32 object, uint32 mask, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800370e8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    w_u32(object + 0x10, r_u32(object + 0x10) | mask);
    uint32 bucket = G3Function8002910C(object, frame);
    G3Function80045BA0(object);
    uint32 offset = (bucket << 3) + 0xc;
    uint32 game = r_u32(0x80097048);
    uint32 pool = r_u32(game + 0x5c);
    return G3Function80045B74(pool + offset, object);
}

uint32 G3Function80037150(uint32 object, uint32 mask, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80037150, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    w_u32(object + 0x10, r_u32(object + 0x10) & ~mask);
    uint32 bucket = G3Function8002910C(object, frame);
    G3Function80045BA0(object);
    uint32 offset = (bucket << 3) + 0xc;
    uint32 game = r_u32(0x80097048);
    uint32 pool = r_u32(game + 0x5c);
    return G3Function80045B74(pool + offset, object);
}

uint32 G3Function80085B84(uint32 cursor, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80085b84, "SLUS_008.06");
    uint32 first_length = r_u32(0x8009a200);
    uint32 command = r_u32(cursor);
    uint32 index = 0;
    if (first_length != 0)
    {
        uint32 three_first = r_u32(0x800a04b8);
        uint32 two = r_u32(0x800a04c0);
        uint32 three_second = r_u32(0x800a04c8);
        uint32 three_third = r_u32(0x800a04d0);
        while (true)
        {
            uint32 offset = (uint32)(sint32)(sint16)index * 12;
            uint32 entry = 0x8009a1f8 + offset;
            uint32 word = r_u32(command);
            uint32 expected = r_u32(entry);
            if (word == expected)
            {
                uint32 tail = r_u32(command + 4);
                uint32 expected_tail = r_u32(entry + 4);
                if (tail == expected_tail)
                {
                    uint32 length = r_u32(entry + 8);
                    command += length << 2;
                    break;
                }
            }
            word = r_u32(command);
            if (word == three_first)
            {
                command += 12;
                break;
            }
            if (word == two)
            {
                command += 8;
                break;
            }
            if (word == three_second || word == three_third)
            {
                command += 12;
                break;
            }
            uint32 next = index + 1;
            offset = (uint32)(sint32)(sint16)next * 12;
            uint32 length = r_u32(0x8009a200 + offset);
            index = next;
            if (length == 0)
                break;
        }
    }
    uint32 offset = (uint32)(sint32)(sint16)index * 12;
    w_u32(cursor, command);
    return r_u32(0x8009a200 + offset) != 0;
}

uint32 G3Function8003D390(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003d390, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 initial_flags = r_u32(object + 0x14);
    uint32 game = r_u32(0x80097048);
    uint32 pending = initial_flags & 0x200;
    uint32 result = r_u32(game + 0x6834) & 0x10000;
    if (result != 0)
        return result;
    result = r_u32(object + 0x18) & 0x800;
    if (result != 0)
        return result;
    bool delay = r_s16(object + 0x4e) != 0;
    if (!delay)
    {
        uint32 descriptor = r_u32(object + 0x1c);
        delay = r_u32(descriptor + 0x28) == 0;
    }
    if (delay)
    {
        uint32 timer = r_u16(object + 0x4e);
        w_u16(object + 0x4e, timer - 1);
        result = r_u32(object + 0x14) & 0xfffff1ff;
        w_u32(object + 0x14, result);
        return result;
    }
    uint32 index = 0;
    do
    {
        uint32 descriptor = r_u32(object + 0x1c);
        uint32 effects = r_u32(descriptor + 0x28);
        result = effects + (index << 2);
        uint32 effect = r_u32(result);
        if (effect == 0)
            break;
        uint32 type = r_u16(effect) & 0xf;
        uint32 target = r_u32(0x8009def4 + (type << 2));
        ++index;
        G3DispatchObjectEffect(target, object, effect, frame);
        result = index < 4;
    } while (result != 0);
    if (pending == 0)
        return result;
    result = r_u32(object + 0x14) & 0x200;
    if (result == 0)
        return result;
    G3Function8003F128(0x810, 0);
    result = r_u32(object + 0x14) & 0xfffffdff;
    w_u32(object + 0x14, result);
    return result;
}

uint32 G3Function8003B48C(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003b48c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    if (r_s32(object + 0x14) < 0)
    {
        uint32 camera = r_u32(game + 0x4c);
        if (G3Function8004DABC(camera, frame) == 6)
        {
            camera = r_u32(game + 0x4c);
            if (G3Function8004DB2C(camera, frame) != 0 && G3Function8003B29C(object, game, frame) != 0)
            {
                uint32 player = r_u32(game + 0x50);
                uint32 contact = r_u32(player + 0xf4);
                uint32 private_state = r_u32(player + 0x24);
                if (contact == 0 && r_s16(private_state + 0x120) == 0)
                {
                    w_u16(private_state + 0x120, 35);
                    uint32 result = G3Function8001932C(0x40000000, object, frame);
                    player = r_u32(game + 0x50);
                    w_u32(player + 0xf4, result);
                    camera = r_u32(game + 0x4c);
                    G3Function80052060(camera, 30, frame);
                }
            }
        }
    }
    if ((r_u16(state + 0x12) & 0x80) != 0)
        G3Function8003CE80(object, game, frame);
    else
    {
        G3Function8003C8C0(object, state, game, frame);
        G3Function8003C614(object, game, frame);
    }
    bool check_signal = r_u32(state + 0x14) != 0;
    if (!check_signal)
        check_signal = (r_u32(object + 0x10) & 8) != 0;
    if (check_signal)
    {
        uint32 flags = r_u16(state + 0x12);
        uint32 selected_address;
        bool eligible;
        if ((flags & 0x20) != 0)
        {
            selected_address = state + 8;
            eligible = (r_u16(selected_address + 2) & 0xfffe) == 0;
        }
        else if ((flags & 0x10) != 0)
        {
            selected_address = state + 4;
            eligible = (r_u16(selected_address + 2) & 0xfffe) == 0;
        }
        else
        {
            uint32 remainder = r_u16(state + 2);
            selected_address = state;
            eligible = remainder == 0;
        }
        if (eligible)
        {
            flags = r_u16(state + 0x12);
            uint32 table = r_u32(state + 0x14);
            if ((flags & 4) != 0)
            {
                sint32 entry = r_s32(table);
                if (entry >= 0)
                {
                    sint32 selected = r_s16(selected_address);
                    while (true)
                    {
                        if (entry == selected)
                        {
                            uint32 sequence = r_u32(table + 4);
                            G3Function80018DE0(object, sequence + 4, frame);
                            break;
                        }
                        table += 8;
                        entry = r_s32(table);
                        if (entry < 0)
                            break;
                    }
                }
            }
            else
            {
                bool matched = table == 0;
                if (table != 0)
                {
                    sint32 entry = r_s32(table);
                    if (entry >= 0)
                    {
                        sint32 selected = r_s16(selected_address);
                        while (true)
                        {
                            if (entry == selected)
                            {
                                matched = true;
                                break;
                            }
                            table += 4;
                            entry = r_s32(table);
                            if (entry < 0)
                                break;
                        }
                    }
                }
                if (matched)
                    G3Function800193F8(0x200000, object, frame);
            }
        }
    }
    if ((r_u32(object + 0x10) & 0x80) != 0 && (r_u16(state + 0x12) & 2) != 0)
    {
        uint32 counter = r_u16(state + 0x1c);
        sint32 threshold = r_s8(state + 0x1a);
        ++counter;
        w_u16(state + 0x1c, counter);
        if ((sint16)counter >= threshold)
        {
            uint32 table = r_u32(state + 0x20);
            w_u16(state + 0x1c, 0);
            sint32 selected;
            bool remove = false;
            if (table != 0)
            {
                uint32 index = r_u16(state + 0x1e);
                table = r_u32(state + 0x20);
                ++index;
                w_u16(state + 0x1e, index);
                uint32 offset = (uint32)(sint32)(sint16)index << 2;
                selected = r_s32(table + offset);
                if (selected < 0)
                {
                    uint32 flags = r_u16(state + 0x12);
                    w_u16(state + 0x1e, 0);
                    selected = r_s32(table);
                    remove = (flags & 0x400) != 0;
                }
            }
            else
            {
                uint32 current = G3Function8003B0D0(object, frame);
                selected = (sint16)current;
                sint32 last = r_s8(state + 0x19);
                ++selected;
                if (last < selected)
                {
                    if ((r_u16(state + 0x12) & 0x400) != 0)
                        remove = true;
                    else
                        selected = r_s8(state + 0x18);
                }
            }
            if (remove)
                G3Function8002A9AC(object, frame);
            G3Function8003B030(object, (uint32)(sint32)(sint16)selected);
        }
    }
    uint32 result = r_u16(state + 0x12) & 1;
    if (result == 0)
        return result;
    uint32 descriptor = r_u32(object + 0x1c);
    result = r_u32(descriptor + 0x10);
    if (result == 0)
        return result;
    G3Function80029F0C(object, 0xffffffff, frame);
    result = r_u32(object + 0x14) & 0x10;
    if (result == 0)
        return result;
    uint32 sequence = r_u32(state + 0x2c);
    if (sequence != 0)
    {
        G3Function80018DE0(object, sequence + 4, frame);
        w_u32(state + 0x2c, 0);
    }
    w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
    result = r_u32(state + 0x34);
    if (result == 0)
        return result;
    uint32 index = r_u16(state + 0x30);
    uint32 table = r_u32(state + 0x34);
    ++index;
    w_u16(state + 0x30, index);
    uint32 offset = (uint32)(sint32)(sint16)index << 3;
    sint32 animation = r_s16(table + offset);
    if (animation < 0)
    {
        w_u16(state + 0x30, 0);
        animation = r_s16(table);
    }
    w_u16(object + 0x6e, (uint32)animation);
    return (uint32)animation;
}

uint32 G3Function8003B0D0(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003b0d0, "SLUS_008.06");
    uint32 descriptor = r_u32(object + 0x1c);
    sint32 model_index = r_s16(object + 0xa2);
    uint32 models = r_u32(descriptor + 0xc);
    uint32 model = r_u32(models + ((uint32)model_index << 2));
    uint32 table = r_u32(model + 0x1c);
    uint32 offset = r_u16(object + 0x9e);
    sint32 result = (sint16)offset;
    if (table != 0)
    {
        sint32 size = r_s32(table + 0xc);
        if (size == 0)
            result = result < 0 ? 1 : -1;
        else
            result /= size;
    }
    return (uint32)(sint32)(sint16)result;
}

uint32 G3Function8003B29C(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003b29c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 camera = r_u32(game + 0x4c);
    uint32 camera_matrix = r_u32(camera + 0x78);
    SetRotMatrix((MATRIX *)psx_addr(camera_matrix, 20));
    for (uint32 column = 0; column < 3; ++column)
    {
        uint32 matrix = r_u32(object + 0x30) + (column << 1);
        uint32 x = r_u16(matrix);
        uint32 y = r_u16(matrix + 6);
        uint32 z = r_u16(matrix + 12);
        xport_gte_write_data(9, x);
        xport_gte_write_data(10, y);
        xport_gte_write_data(11, z);
        xport_gte_execute(0x49e012);
        x = xport_gte_read_data(9);
        y = xport_gte_read_data(10);
        z = xport_gte_read_data(11);
        w_u16(frame + (column << 1), x);
        w_u16(frame + 6 + (column << 1), y);
        w_u16(frame + 12 + (column << 1), z);
    }
    camera = r_u32(game + 0x4c);
    camera_matrix = r_u32(camera + 0x78);
    SetTransMatrix((MATRIX *)psx_addr(camera_matrix, 32));
    uint32 translation = r_u32(object + 0x30) + 0x14;
    uint32 y = r_u16(translation + 4);
    uint32 x = r_u16(translation);
    xport_gte_write_data(0, x | (y << 16));
    xport_gte_write_data(1, r_u32(translation + 8));
    xport_gte_execute(0x480012);
    w_u32(frame + 0x14, xport_gte_read_data(25));
    w_u32(frame + 0x18, xport_gte_read_data(26));
    w_u32(frame + 0x1c, xport_gte_read_data(27));
    uint32 depth = r_u32(frame + 0x1c);
    uint32 horizontal = r_u32(frame + 0x14);
    uint32 vertical = r_u32(frame + 0x18);
    if (depth - 321 >= 0x81f)
        return 0;
    sint32 projected_x = (sint32)(horizontal * 320) / (sint32)depth;
    sint32 projected_y = (sint32)(vertical * 320) / (sint32)depth;
    return projected_x > -50 && projected_x < 50 && projected_y > -50 && projected_y < 50;
}

uint32 G3Function8003C614(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003c614, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 flags = r_u32(object + 0x10);
    flags = (sint32)flags < 0 ? flags | 0x4000000 : flags & 0xfbffffff;
    w_u32(object + 0x10, flags);
    uint32 descriptor = r_u32(object + 0x1c);
    if ((r_u32(descriptor) & 0x2000) != 0)
    {
        uint32 player = r_u32(0x8009efc4);
        uint32 callback = r_u32(player + 0xec);
        if (callback != 0)
            G3DispatchStateEvent(callback, player, game, 1002, object, frame);
    }
    w_u32(object + 0x10, r_u32(object + 0x10) & 0x7f7fffff);
    uint32 result = r_u32(object + 0x14) & 0xfefffffc;
    w_u32(object + 0x14, result);
    return result;
}

uint32 G3Function8004DABC(uint32 camera, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004dabc, "SLUS_008.06");
    return r_u32(0x80096a88);
}

uint32 G3Function8004DB2C(uint32 camera, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004db2c, "SLUS_008.06");
    if (r_u32(0x80096a88) == 6)
        return G3Function800520CC();
    return 0;
}

uint32 G3Function80052060(uint32 camera, uint32 delay, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80052060, "SLUS_008.06");
    uint32 result = r_s32(0x80096a98) < 10;
    if (result == 0)
        w_u32(0x80096b48, delay);
    return result;
}

uint32 G3Function800520CC(void)
{
    FUNCTION_MARKER(0x800520cc, "SLUS_008.06");
    if (r_u32(0x800a23b0) == 0)
        return 0;
    if (r_u32(0x800a23ac) == 0)
        return 0;
    return r_u32(0x80096b48) == 0;
}

uint32 G3Function8003CE80(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003ce80, "SLUS_008.06");
    uint32 frame = guest_stack - 0x98;
    uint32 motion = r_u32(object + 0x12c);
    G3FillMemory80086094(frame + 0x10, 0, 8);
    if (r_s32(motion + 8) > 0 && r_u32(motion + 0x1c) == 0 && r_u32(motion + 0x20) == 0 && r_u32(motion + 0x10) == 0)
    {
        sint32 random = (sint32)G3SdkRandom80027FA8();
        sint32 angle = (sint16)(random % 4096);
        uint32 sine = G3Function8005B470((uint32)angle);
        uint32 magnitude = r_u32(motion + 8);
        sint32 horizontal = (sint32)(sine * magnitude) / 4096;
        uint32 cosine = G3Function8005B590((uint32)angle);
        magnitude = r_u32(motion + 8);
        sint32 vertical = (sint32)(cosine * magnitude) / 4096;
        uint32 doubled = magnitude << 1;
        sint32 velocity = r_s32(motion + 0x14);
        if ((sint32)(0 - doubled) < velocity && velocity < (sint32)doubled)
        {
            w_u32(motion + 0x1c, (uint32)(sint32)(sint16)horizontal << 5);
            magnitude = r_u32(motion + 8);
            w_u32(motion + 0x20, (uint32)(sint32)(sint16)vertical << 5);
            uint32 impulse = (uint32)((sint32)magnitude / 128) + 1;
            w_u32(motion + 0x10, 0 - (impulse << 12));
        }
    }
    w_u16(frame + 0x18, r_u16(object + 0x60));
    w_u16(frame + 0x1a, r_u16(object + 0x62));
    w_u16(frame + 0x1c, r_u16(object + 0x64));
    G3Function8003C8C0(object, object + 0x104, game, frame);
    uint32 changed_z = r_u16(object + 0x64);
    uint32 original_x = r_u16(frame + 0x18);
    w_u16(frame + 0x14, changed_z);
    w_u16(object + 0x60, original_x);
    w_u16(object + 0x62, r_u16(frame + 0x1a));
    w_u16(object + 0x64, r_u16(frame + 0x1c));
    if ((r_u32(object + 0x10) & 0x100000) != 0)
        w_u16(object + 0x5c, r_u16(motion + 0xc));
    sint32 rotation_x = r_s16(motion + 0x40);
    sint32 velocity_x = r_s32(motion + 0x24);
    sint32 rotation_y = r_s16(motion + 0x42);
    uint32 damping_x = (((0 - (uint32)rotation_x) << 5) - (uint32)(velocity_x >> 6)) << 1;
    sint32 velocity_y = r_s32(motion + 0x28);
    uint32 damping_y = (((0 - (uint32)rotation_y) << 5) - (uint32)(velocity_y >> 6)) << 1;
    G3Function8003C614(object, game, frame);
    uint32 force_x = r_u32(motion + 0x1c);
    uint32 speed_x = r_u32(motion + 0x24);
    uint32 speed_limit = r_u32(motion + 4);
    uint32 force_y = r_u32(motion + 0x20);
    w_u32(motion + 0x24, speed_x + damping_x + force_x);
    uint32 speed_y = r_u32(motion + 0x28);
    sint32 next_x = r_s32(motion + 0x24);
    w_u32(motion + 0x28, speed_y + damping_y + force_y);
    uint32 bound = speed_limit << 12;
    if ((sint32)bound < next_x)
        w_u32(motion + 0x24, bound);
    else
    {
        bound = (0 - speed_limit) << 12;
        if (next_x < (sint32)bound)
            w_u32(motion + 0x24, bound);
    }
    speed_limit = r_u32(motion + 4);
    sint32 next_y = r_s32(motion + 0x28);
    bound = speed_limit << 12;
    if ((sint32)bound < next_y)
        w_u32(motion + 0x28, bound);
    else
    {
        bound = (0 - speed_limit) << 12;
        if (next_y < (sint32)bound)
            w_u32(motion + 0x28, bound);
    }
    uint32 position_x = r_u32(motion + 0x2c);
    speed_x = r_u32(motion + 0x24);
    uint32 position_y = r_u32(motion + 0x30);
    speed_y = r_u32(motion + 0x28);
    position_x += speed_x;
    uint32 position_limit = r_u32(motion);
    position_y += speed_y;
    w_u32(motion + 0x2c, position_x);
    w_u32(motion + 0x30, position_y);
    bound = position_limit << 12;
    if ((sint32)bound < (sint32)position_x)
        w_u32(motion + 0x2c, bound);
    else
    {
        bound = (0 - position_limit) << 12;
        if ((sint32)position_x < (sint32)bound)
            w_u32(motion + 0x2c, bound);
    }
    position_limit = r_u32(motion);
    position_y = r_u32(motion + 0x30);
    bound = position_limit << 12;
    if ((sint32)bound < (sint32)position_y)
        w_u32(motion + 0x30, bound);
    else
    {
        bound = (0 - position_limit) << 12;
        if ((sint32)position_y < (sint32)bound)
            w_u32(motion + 0x30, bound);
    }
    position_x = r_u32(motion + 0x2c);
    position_y = r_u32(motion + 0x30);
    w_u16(motion + 0x44, 0);
    w_u16(motion + 0x40, (uint32)((sint32)position_x >> 12));
    w_u16(motion + 0x42, (uint32)((sint32)position_y >> 12));
    G3Function8005BD00(motion + 0x40, frame + 0x20);
    G3Function8005BD00(frame + 0x10, frame + 0x40);
    MulMatrix0((MATRIX *)psx_addr(frame + 0x40, 20), (MATRIX *)psx_addr(frame + 0x20, 20), (MATRIX *)psx_addr(frame + 0x60, 20));
    G3Function800202A4(frame + 0x60, object + 0x60, 0x15, frame);
    if (G3Function80083424(object, frame) == 0)
    {
        uint32 introduction = r_u32(object + 0x20);
        w_u16(object + 0x64, r_u16(introduction + 0xc));
    }
    uint32 displacement = r_u32(motion + 0x18);
    uint32 velocity = r_u32(motion + 0x14);
    uint32 acceleration = r_u32(motion + 0x10);
    uint32 damping = (uint32)((sint32)(0 - displacement) >> 4) - (uint32)((sint32)velocity >> 6);
    acceleration += damping;
    displacement = r_u32(motion + 0x18);
    velocity += acceleration;
    w_u32(motion + 0x14, velocity);
    displacement += velocity;
    w_u32(motion + 0x18, displacement);
    uint32 z = r_u16(object + 0x5c);
    w_u16(object + 0x5c, z + (uint32)((sint32)displacement >> 12));
    uint32 result = r_u32(motion + 0x38);
    w_u32(motion + 0x38, 0);
    w_u32(motion + 0x1c, 0);
    w_u32(motion + 0x20, 0);
    w_u32(motion + 0x10, 0);
    w_u32(motion + 0x3c, result);
    return result;
}

uint32 G3Function8003C8C0(uint32 object, uint32 state, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003c8c0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 result = r_u32(object + 0x10) & 0x100000;
    if (result != 0)
        return result;
    uint32 links = G3Function8008345C(object, frame + 0x10, frame + 0x14);
    if (links == 0)
        return links;
    if (state == 0)
    {
        uint32 linked_source = r_u32(frame + 0x10);
        uint32 model_source = r_u32(frame + 0x14);
        state = G3Function8008354C(object, linked_source, model_source);
        if (state == 0)
            return state;
    }
    uint32 flags = r_u32(object + 0x10);
    if ((flags & 0x2000000) == 0)
        return 0x1000000;
    sint32 direction = (flags & 0x1000000) != 0 ? -1 : 1;
    uint32 descriptor = r_u32(object + 0x1c);
    if ((r_u32(descriptor) & 0x20000) != 0)
    {
        w_u32(object + 0x10, flags | 0x400);
        uint32 shifted = G3Function8003FAE4(state, links, frame) << 16;
        if (shifted != 0)
        {
            uint32 introduction = r_u32(object + 0x20);
            if (introduction != 0)
            {
                uint32 introduction_flags = r_u32(introduction + 0x18);
                introduction_flags = direction > 0 ? introduction_flags | 0x800 : introduction_flags & 0xfffff7ff;
                w_u32(introduction + 0x18, introduction_flags);
            }
        }
    }
    result = G3Function80082DE4(object, state, (uint32)direction, frame);
    if ((sint32)result <= 0)
        return result;
    if ((r_u32(object + 0x14) & 0x10000) != 0)
    {
        uint32 handle = r_u32(object + 0x50);
        G3Function8003F18C(handle);
        w_u32(object + 0x14, r_u32(object + 0x14) & 0xfffcffff);
    }
    descriptor = r_u32(object + 0x1c);
    if ((r_u32(descriptor) & 0x20000) != 0)
        w_u32(object + 0x10, (r_u32(object + 0x10) & 0xfdfffbff) | 0x100000);
    if ((r_u32(object + 0x14) & 0x10000000) != 0)
    {
        uint32 commands = r_u32(object + 0x28);
        if (commands == 0)
            commands = r_u32(object + 0x24);
        uint32 parameter = G3Function80085AB0(commands, 0x8009dd80, frame);
        uint32 selector = 0;
        if (parameter != 0)
            selector = r_u16(parameter);
        w_u32(object + 0x10, r_u32(object + 0x10) | 0x100010);
        return G3Function80038798(object, (uint32)(sint32)(sint16)selector, 0, frame);
    }
    descriptor = r_u32(object + 0x1c);
    uint32 resource_flags = r_u32(descriptor);
    if ((resource_flags & 0x8000) != 0)
    {
        uint32 object_flags = r_u32(object + 0x10);
        descriptor = r_u32(object + 0x1c);
        w_u32(object + 0x10, object_flags & 0xfdffffff);
        if ((r_u32(descriptor) & 0x40000) != 0)
        {
            uint32 current_game = r_u32(0x80097048);
            G3Function80082988(object, state, current_game, frame);
        }
    }
    else if ((resource_flags & 0x80000) != 0)
        w_u32(object + 0x10, r_u32(object + 0x10) & 0xfdffffff);
    result = r_u32(links);
    if (result == 0)
    {
        result = r_u32(links + 4);
        if (result == 0)
            return result;
    }
    result = r_u8(result + 7) & 4;
    if (result != 0)
        return result;
    descriptor = r_u32(object + 0x1c);
    result = r_u32(descriptor) & 0x40000;
    if (result != 0)
        return result;
    result = r_u32(object + 0x10) ^ 0x1000000;
    w_u32(object + 0x10, result);
    return result;
}

uint32 G3Function80083424(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80083424, "SLUS_008.06");
    uint32 links = G3Function8008345C(object, 0, 0);
    if (links == 0)
        return 0;
    return r_u32(links + 4);
}

uint32 G3Function80029F0C(uint32 object, uint32 selector, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80029f0c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 animations = r_u32(descriptor + 0x10);
    if (animations == 0)
        w_u32(object + 0x14, r_u32(object + 0x14) | 0x10);
    else
    {
        sint32 step = 1;
        uint32 magnitude = (sint32)selector < 0 ? 0 - selector : selector;
        if ((sint32)magnitude >= 1001)
        {
            selector = (sint32)selector <= 0 ? selector + 1000 : selector - 1000;
            step = -1;
        }
        descriptor = r_u32(object + 0x1c);
        uint32 index = r_u16(object + 0x6e);
        animations = r_u32(descriptor + 0x10);
        uint32 animation = r_u32(animations + (index << 2));
        uint32 count = r_u16(animation + 2);
        uint32 last = step > 0 ? count - 1 : 0;
        uint32 wrap_boundary = step > 0 ? count : 0xffffffff;
        uint32 wrap_target = step > 0 ? 0 : count - 1;
        if ((sint32)selector > 0)
        {
            for (uint32 axis = 0; axis < 3; ++axis)
            {
                uint32 offset = axis << 1;
                sint32 current = r_s16(object + 0x58 + offset);
                sint32 previous = r_s16(object + 0x70 + offset);
                w_u32(frame + 0x20 + (axis << 2), (uint32)(current - previous));
            }
            uint32 dx = r_u32(frame + 0x20);
            uint32 dy = r_u32(frame + 0x24);
            uint32 dz = r_u32(frame + 0x28);
            w_u32(frame + 0x10, dx);
            w_u32(frame + 0x14, dy);
            w_u32(frame + 0x18, dz);
            uint32 distance = G3Function8002FBF8(frame + 0x10, frame);
            uint32 accumulator = r_u16(object + 0xa0) + distance;
            w_u16(object + 0xa0, accumulator);
            sint32 endpoint = (sint16)last;
            sint32 boundary = (sint16)wrap_boundary;
            while ((sint32)selector < (sint16)accumulator)
            {
                descriptor = r_u32(object + 0x1c);
                if ((r_u32(descriptor + 0x30) & 0x2000) != 0)
                    G3Function8003E314(object, frame);
                uint32 key = r_u16(object + 0x5e) + (uint32)step;
                w_u16(object + 0x5e, key);
                if ((sint16)key == endpoint)
                    w_u32(object + 0x14, r_u32(object + 0x14) | 0x10);
                else if ((sint16)key == boundary)
                    w_u16(object + 0x5e, wrap_target);
                accumulator = r_u16(object + 0xa0) - selector;
                w_u16(object + 0xa0, accumulator);
            }
        }
        else
        {
            uint32 accumulator = r_u16(object + 0xa0) - 1;
            w_u16(object + 0xa0, accumulator);
            if ((sint32)(accumulator << 16) > 0)
                w_u16(object + 0xa0, 0);
            sint32 remaining = r_s16(object + 0xa0);
            if ((sint32)selector >= remaining)
            {
                descriptor = r_u32(object + 0x1c);
                if ((r_u32(descriptor + 0x30) & 0x2000) != 0)
                    G3Function8003E314(object, frame);
                uint32 key = r_u16(object + 0x5e) + (uint32)step;
                w_u16(object + 0x5e, key);
                if ((key << 16) == (last << 16))
                    w_u32(object + 0x14, r_u32(object + 0x14) | 0x10);
                else if ((sint16)key == (sint16)wrap_boundary)
                    w_u16(object + 0x5e, wrap_target);
                w_u16(object + 0xa0, 0);
            }
        }
    }
    return r_u32(object + 0x14) & 0x10;
}

uint32 G3Function8002FBF8(uint32 vector, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002fbf8, "SLUS_008.06");
    uint32 x = r_u32(vector);
    uint32 z = r_u32(vector + 8);
    if ((sint32)x < 0)
        x = 0 - x;
    uint32 y = r_u32(vector + 4);
    if ((sint32)z < 0)
        z = 0 - z;
    if ((sint32)y < 0)
        y = 0 - y;
    if ((sint32)x > (sint32)y)
    {
        uint32 temporary = x;
        x = y;
        y = temporary;
    }
    if ((sint32)y > (sint32)z)
    {
        uint32 temporary = y;
        y = z;
        z = temporary;
    }
    if ((sint32)x > (sint32)y)
    {
        uint32 temporary = x;
        x = y;
        y = temporary;
    }
    uint32 weighted = z * 30 + y * 12 + x * 9;
    return (uint32)((sint32)weighted >> 5);
}

uint32 G3Function8003FA8C(uint32 path, uint32 state)
{
    FUNCTION_MARKER(0x8003fa8c, "SLUS_008.06");
    uint32 flags = r_u16(state + 2);
    if ((flags & 0xfffe) == 0)
    {
        if (r_s16(state) == 0)
            return 0xffffffff;
    }
    flags = r_u16(state + 2);
    sint32 count = r_s16(path + 4);
    if ((sint32)(flags >> 1) != count - 1)
        return 0;
    return r_s16(state) == 0;
}

uint32 G3Function8003FAE4(uint32 state, uint32 links, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003fae4, "SLUS_008.06");
    uint32 result = 0;
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 path = r_u32(links + (axis << 2));
        if (path != 0 && (sint16)result == 0)
            result = G3Function8003FA8C(path, state + (axis << 2));
    }
    return (uint32)(sint32)(sint16)result;
}

uint32 G3Function80085AB0(uint32 commands, uint32 name, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80085ab0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    w_u32(guest_stack, commands);
    w_u32(0x800a04a8, 0);
    if (commands == 0)
        return 0;
    uint32 word = r_u32(commands);
    uint32 terminator = r_u32(0x800a04ac);
    while (true)
    {
        if (word == terminator)
        {
            uint32 tail = r_u32(commands + 4);
            uint32 end_tail = r_u32(0x800a04b0);
            if (tail == end_tail)
                return 0;
        }
        commands = r_u32(guest_stack);
        uint32 expected = r_u32(name);
        uint32 actual = r_u32(commands);
        if (actual == expected)
        {
            uint32 actual_tail = r_u32(commands + 4);
            uint32 expected_tail = r_u32(name + 4);
            if (actual_tail == expected_tail)
                return commands + 8;
        }
        if (G3Function80085B84(guest_stack, frame) == 0)
        {
            w_u32(0x800a04a8, 1);
            return 0;
        }
        commands = r_u32(guest_stack);
        terminator = r_u32(0x800a04ac);
        word = r_u32(commands);
    }
}

uint32 G3Function8003E314(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003e314, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 game = r_u32(0x80097048);
    uint32 result = r_u32(game + 0x6834) & 0x10000;
    if (result != 0)
        return result;
    result = r_u32(object + 0x18) & 0x800;
    if (result != 0)
        return result;
    result = r_u32(object + 0x1c);
    uint32 event_tables = r_u32(result + 0x2c);
    if (event_tables == 0)
        return result;
    uint32 animation = r_u16(object + 0x6e);
    result = event_tables + (animation << 2);
    uint32 events = r_u32(result);
    if (events == 0)
        return result;
    uint32 player = r_u32(game + 0x50);
    bool is_player = object == player;
    uint32 key_address = events + 4;
    result = r_u8(events + 1);
    if (result == 0)
        return result;
    uint32 entry = events + 5;
    uint32 index = 0;
    do
    {
        uint32 event_key = r_u8(key_address);
        sint32 current_key = r_s16(object + 0x5e);
        if ((sint32)event_key == current_key)
        {
            uint32 sound;
            bool emit = true;
            if (is_player)
            {
                uint32 original_sound = r_u16(entry + 1);
                if (original_sound == 0x812)
                {
                    uint32 contact = r_u32(object + 0xf8);
                    if (contact == 0)
                        contact = r_u32(object + 0xa8);
                    if (contact == 0)
                        emit = false;
                    else
                    {
                        uint32 material = (r_u16(contact + 6) & 0x1e0) >> 5;
                        uint32 current_game = r_u32(0x80097048);
                        uint32 level = r_u32(current_game + 0x48);
                        uint32 materials = r_u32(level + 0xf0);
                        sint32 replacement = r_s16(materials + (material << 4) + 4);
                        sound = replacement == 0 ? 0x812 : (uint32)replacement & 0xffff;
                    }
                }
                else
                    sound = r_u16(entry + 1);
            }
            else
                sound = r_u16(entry + 1);
            if (emit)
            {
                sint32 pitch = r_s16(entry + 3);
                uint32 volume = r_u8(entry);
                sint32 mode = r_s16(events + 2);
                w_u32(frame + 0x10, (uint32)mode);
                G3Function8003E518(object, sound, (uint32)pitch, volume, frame);
            }
        }
        entry += 6;
        uint32 count = r_u8(events + 1);
        ++index;
        result = index < count;
        key_address += 6;
    } while (result != 0);
    return result;
}

uint32 G3Function80082DE4(uint32 object, uint32 state, uint32 direction, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80082de4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 links = G3Function8008345C(object, frame + 0x18, frame + 0x1c);
    if (links == 0)
        return 0xffffffff;
    if (state == 0)
    {
        uint32 linked_source = r_u32(frame + 0x18);
        uint32 model_source = r_u32(frame + 0x1c);
        state = G3Function8008354C(object, linked_source, model_source);
        if (state == 0)
            return 0xffffffff;
    }
    w_u32(frame + 0x10, r_u32(frame + 0x1c));
    uint32 translation = G3Function80082B34(object, links, state, direction, frame);
    w_u32(frame + 0x10, r_u32(frame + 0x1c));
    uint32 rotation = G3Function80082C10(object, links, state, direction, frame);
    w_u32(frame + 0x10, r_u32(frame + 0x1c));
    uint32 scale = G3Function80082D30(object, links, state, direction, frame);
    uint32 result = 0xffffffff;
    if (translation != 0xffffffff || rotation != 0xffffffff || scale != 0xffffffff)
        result = translation != 0 && rotation != 0 && scale != 0;
    if (result == 1)
        G3Function800193F8(0x4000, object, frame);
    return result;
}

uint32 G3Function80082B34(uint32 object, uint32 links, uint32 state, uint32 direction, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80082b34, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 path = r_u32(links);
    uint32 mode = r_u32(guest_stack + 0x10);
    if (path == 0)
        return 0xffffffff;
    uint32 point = 0;
    uint32 result;
    if ((sint32)direction > 0)
    {
        result = G3Function8003FC34(path, state, frame);
        point = result;
    }
    else if ((sint32)direction < 0)
    {
        result = G3Function8003FC94(path, state, frame);
        point = result;
    }
    else
    {
        result = G3Function80041008(path, state, frame + 0x10, frame);
        if (result != 0)
            point = frame + 0x10;
    }
    if (point == 0)
        return 1;
    if (mode != 0)
        G3Function80083038(object, point, result, frame);
    else
    {
        w_u16(object + 0x58, r_u16(point));
        w_u16(object + 0x5a, r_u16(point + 2));
        w_u16(object + 0x5c, r_u16(point + 4));
    }
    return 0;
}

uint32 G3Function80082C10(uint32 object, uint32 links, uint32 state, uint32 direction, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80082c10, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 flags = r_u32(object + 0x18);
    uint32 mode = r_u32(guest_stack + 0x10);
    uint32 path = r_u32(links + 4);
    if ((flags & 2) != 0)
    {
        sint32 x = r_s16(object + 0x58);
        sint32 y = r_s16(object + 0x5a);
        sint32 previous_y = r_s16(object + 0x72);
        sint32 previous_x = r_s16(object + 0x70);
        uint32 angle = G3Function8002F240((uint32)(y - previous_y), (uint32)(x - previous_x), frame);
        w_u16(object + 0x64, angle + 0x400);
        return 0xffffffff;
    }
    if (path == 0)
        return 0xffffffff;
    uint32 result = 0;
    if ((sint32)direction > 0)
        result = G3Function80041230(path, state + 4, frame) == 0;
    else if ((sint32)direction < 0)
        result = G3Function8004136C(path, state + 4, frame) == 0;
    if (result != 0)
        return result;
    if (G3Function80041008(path, state + 4, frame + 0x10, frame) == 0)
        return 1;
    w_u16(object + 0x60, r_u16(frame + 0x10));
    w_u16(object + 0x62, r_u16(frame + 0x12));
    uint32 angle = r_u16(frame + 0x14);
    w_u16(object + 0x64, angle);
    if (mode != 0)
        G3Function800828D4(object, angle, frame);
    return 0;
}

uint32 G3Function80082D30(uint32 object, uint32 links, uint32 state, uint32 direction, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80082d30, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 path = r_u32(links + 8);
    if (path == 0)
        return 0xffffffff;
    uint32 point = 0;
    if ((sint32)direction > 0)
        point = G3Function8003FC34(path, state + 8, frame);
    else if ((sint32)direction < 0)
        point = G3Function8003FC94(path, state + 8, frame);
    else if (G3Function80041008(path, state + 8, frame + 0x10, frame) != 0)
        point = frame + 0x10;
    if (point == 0)
        return 1;
    w_u16(object + 0x68, r_u16(point));
    w_u16(object + 0x6a, r_u16(point + 4));
    w_u16(object + 0x6c, r_u16(point + 2));
    return 0;
}

uint32 G3Function8003FC34(uint32 path, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003fc34, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    if (G3Function80041230(path, state, frame) == 0)
        return 0;
    if (G3Function80041008(path, state, 0x800a2118, frame) == 0)
        return 0;
    return 0x800a2118;
}

uint32 G3Function8003FC94(uint32 path, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003fc94, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    if (G3Function8004136C(path, state, frame) == 0)
        return 0;
    if (G3Function80041008(path, state, 0x800a2120, frame) == 0)
        return 0;
    return 0x800a2120;
}

uint32 G3Function80041230(uint32 path, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80041230, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    if (path == 0 || state == 0)
        return 0;
    uint32 format = r_u8(path + 6);
    sint32 index = r_s16(state);
    sint32 count = r_s16(path + 4);
    if (index >= count)
        return 0;
    uint32 offset = format == 1 ? (uint32)index * 10 : (uint32)index << 5;
    uint32 points = r_u32(path);
    sint32 duration = r_s16(points + offset);
    G3Function8003F4A0(path, state, 0, frame, 0);
    uint32 flags = r_u16(state + 2);
    uint32 advanced = (flags & 1) | ((flags & 0xfffe) + 2);
    w_u16(state + 2, advanced);
    if ((sint32)((advanced & 0xffff) >> 1) < duration)
        return 1;
    uint32 next = r_u16(state);
    flags = r_u16(state + 2);
    w_u16(state, next + 1);
    w_u16(state + 2, flags & 1);
    count = r_s16(path + 4);
    index = r_s16(state);
    uint32 unsigned_count = r_u16(path + 4);
    sint32 last = count - 1;
    if (index < last)
        return 1;
    uint32 result = index <= last;
    uint32 path_flags = r_u32(path + 4);
    w_u16(state, (path_flags & 0x06000000) != 0 ? 0 : unsigned_count - 1);
    return result;
}

uint32 G3Function8004136C(uint32 path, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004136c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    if (path == 0 || state == 0)
        return 0;
    G3Function8003F4A0(path, state, 0, frame, 0);
    uint32 format = r_u8(path + 6);
    sint32 index = r_s16(state);
    sint32 count = r_s16(path + 4);
    if (index >= count)
        return 0;
    uint32 flags = r_u16(state + 2);
    uint32 ticks = flags & 0xfffe;
    if (ticks != 0)
    {
        w_u16(state + 2, (flags & 1) | (ticks - 2));
        return 1;
    }
    index = r_s16(state);
    uint32 previous = r_u16(state) - 1;
    if (index <= 0)
    {
        uint32 path_flags = r_u32(path + 4);
        if ((path_flags & 0x06000000) == 0)
        {
            flags = r_u16(state + 2);
            w_u16(state, 0);
            w_u16(state + 2, flags & 1);
            return 0;
        }
        previous = r_u16(path + 4) - 2;
    }
    w_u16(state, previous);
    uint32 offset;
    uint32 points;
    if (format == 1)
    {
        sint32 previous_index = (sint32)(previous << 16) >> 16;
        offset = (uint32)previous_index * 10;
        points = r_u32(path);
    }
    else
    {
        points = r_u32(path);
        offset = (uint32)((sint32)(previous << 16) >> 11);
    }
    sint32 duration = r_s16(points + offset);
    flags = r_u16(state + 2);
    w_u16(state + 2, (flags & 1) | (((uint32)duration - 1) << 1));
    return 1;
}

uint32 G3Function8003F4A0(uint32 path, uint32 state, uint32 mode, uint32 guest_stack, uint32 *converted_product)
{
    FUNCTION_MARKER(0x8003f4a0, "SLUS_008.06");
    uint32 result = r_u16(state + 2) & 1;
    if ((result != 0 && mode != 0) || (result == 0 && mode == 0))
        return result;
    if ((r_u16(state + 2) & 0xfffe) != 0)
    {
        uint32 format = r_u8(path + 6);
        sint32 index = r_s16(state);
        uint32 points = r_u32(path);
        uint32 offset = format == 1 ? (uint32)index * 10 : (uint32)index << 5;
        uint32 duration = (uint32)r_s16(points + offset);
        if (mode == 0 || duration != 0)
        {
            uint32 flags = r_u16(state + 2);
            uint32 converted;
            if (mode != 0)
                converted = ((flags >> 1) << 15) / duration;
            else
            {
                uint32 product = (flags >> 1) * duration;
                if (converted_product != 0)
                    *converted_product = product;
                converted = product >> 15;
            }
            w_u16(state + 2, (flags & 1) | (converted << 1));
        }
    }
    uint32 flags = r_u16(state + 2);
    result = mode != 0 ? flags | 1 : flags & 0xfffe;
    w_u16(state + 2, result);
    return result;
}

uint32 G3Function80038318(uint32 object, uint32 selector, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80038318, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 resource_flags = r_u32(descriptor + 0x3c);
    uint32 effects = 0;
    if ((resource_flags & 0x100) != 0)
        return G3Function80082854(object, frame);
    if ((resource_flags & 0x3400) != 0)
    {
        uint32 result = G3Function80037EAC(object, frame);
        G3Function800374FC(object, result, frame);
        G3Function800376D8(object, frame);
        resource_flags = r_u32(descriptor + 0x3c);
        uint32 strength;
        uint32 radius;
        uint32 rate;
        if ((resource_flags & 0x1000) != 0)
        {
            strength = (resource_flags & 0x2000) != 0 ? 0x733 : 0x4cc;
            radius = (resource_flags & 0x2000) != 0 ? 0x258 : 0x190;
            rate = 0x64;
        }
        else
        {
            strength = (resource_flags & 0x2000) != 0 ? 0x999 : 0xc00;
            radius = (resource_flags & 0x2000) != 0 ? 0x320 : 0x3e8;
            rate = 0xc8;
        }
        uint32 player = r_u32(0x8009efc4);
        w_u32(frame + 0x10, 8);
        w_u32(frame + 0x18, object + 0x58);
        w_u32(frame + 0x20, radius);
        w_u32(frame + 0x14, 0);
        w_u32(frame + 0x24, rate);
        w_u32(frame + 0x28, 0);
        w_u32(frame + 0x1c, player + 0x58);
        return G3Function80081A90(0, 1, 0xffffffff, strength, frame);
    }
    uint32 flags = r_u32(descriptor + 0x30);
    if ((flags & 0x10) != 0)
        effects = 0x49;
    if ((flags & 0x800) != 0)
        effects |= 0xc1;
    if ((flags & 0x40) != 0)
        effects |= 0x14;
    if ((flags & 0x20) != 0)
        effects |= 3;
    if ((flags & 0x80) != 0)
        effects |= 0x21;
    if ((flags & 0x02000000) != 0)
    {
        uint32 explosion = G3Function8003AA48(0x8009dac8);
        w_u16(object + 0x5c, r_u16(object + 0x5c) + 0x1f4);
        if (explosion != 0)
        {
            uint32 models = r_u32(explosion + 0x0c);
            uint32 model = r_u32(models);
            uint32 geometry = r_u32(model + 0x1c);
            uint32 width = r_u32(geometry + 8);
            uint32 height = r_u32(geometry + 0x0c);
            uint32 pool = r_u32(0x800a27fc);
            uint32 count = (uint32)((sint32)((width * height) << 16) >> 16);
            G3Function800444E4(object + 0x58, model, pool, count, frame);
        }
        effects &= 0xfffffffe;
        w_u16(object + 0x5c, r_u16(object + 0x5c) - 0x1f4);
    }
    resource_flags = r_u32(descriptor + 0x3c);
    if ((resource_flags & 0x20) != 0)
    {
        effects |= 0x200;
        uint32 player = r_u32(0x8009efc4);
        w_u32(frame + 0x10, 0x14);
        w_u32(frame + 0x18, object + 0x58);
        w_u32(frame + 0x14, 0);
        w_u32(frame + 0x20, 0x4b0);
        w_u32(frame + 0x24, 0);
        w_u32(frame + 0x28, 0);
        w_u32(frame + 0x1c, player + 0x58);
        G3Function80081A90(0, 0, 0xffffffff, 0x1000, frame);
    }
    else
    {
        uint32 name = r_u32(descriptor + 0x20);
        uint32 expected = r_u32(0x8009dab8);
        uint32 word = r_u32(name);
        bool matched = false;
        if (word == expected)
        {
            word = r_u32(name + 4);
            expected = r_u32(0x8009dabc);
            matched = word == expected;
        }
        if (!matched)
        {
            name = r_u32(descriptor + 0x24);
            expected = r_u32(0x8009dad4);
            word = r_u32(name);
            if (word == expected)
            {
                word = r_u32(name + 4);
                expected = r_u32(0x8009dad8);
                matched = word == expected;
            }
        }
        if (!matched)
        {
            uint32 player = r_u32(0x8009efc4);
            w_u32(frame + 0x10, 8);
            w_u32(frame + 0x18, object + 0x58);
            w_u32(frame + 0x20, 0x3e8);
            w_u32(frame + 0x14, 0);
            w_u32(frame + 0x24, 0x64);
            w_u32(frame + 0x28, 0);
            w_u32(frame + 0x1c, player + 0x58);
            G3Function80081A90(0, 1, 0xffffffff, 0xc00, frame);
        }
    }
    if ((r_u32(descriptor + 0x3c) & 0x80) != 0 && (r_u32(descriptor) & 0x40) == 0 && r_u32(descriptor + 0x10) != 0)
    {
        G3Function80037D10(object, frame);
        effects |= 0x800;
    }
    bool check_flag = true;
    if (effects == 0)
    {
        uint32 object_flags = r_u32(object + 0x14);
        if ((object_flags & 1) != 0)
            effects = 3;
        if ((object_flags & 2) != 0)
        {
            uint32 model_flags = r_u32(descriptor + 0x30);
            effects |= 0x10;
            effects |= (model_flags & 0x400) != 0 ? 0x48 : 4;
        }
        if ((r_u32(object + 0x14) & 0x01000000) == 0)
            check_flag = false;
        else
            effects |= 0x49;
    }
    if (check_flag && (r_u32(object + 0x14) & 0x01000000) != 0)
        effects |= 0x100;
    flags = r_u32(descriptor + 0x30);
    if ((flags & 0x100) != 0)
    {
        effects &= 0x11;
        if (effects == 0)
            effects = 1;
        effects |= 0x40;
    }
    else if ((flags & 0x200) != 0)
        effects &= 0xffffffee;
    uint32 result = G3Function8003778C(object, 0x80037f5c, effects, selector, frame);
    if (effects != 0)
    {
        uint32 game = r_u32(0x80097048);
        result = G3Function80037F5C(object, game, frame);
    }
    return result;
}
