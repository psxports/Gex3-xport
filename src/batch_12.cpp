#include "psx.h"
#include "xport.h"
#include "game_functions.h"

uint32 G3Function80035044(uint32 sequence, uint32 time, uint32 packets, uint32 order_table)
{
    FUNCTION_MARKER(0x80035044, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 packet = r_u32(packets + 4);
    uint32 count = r_u32(sequence);
    uint32 order_index = r_u32(game + 0x28);
    uint32 end = r_u32(packets + 8);
    uint32 order = order_table + (order_index << 2) - 4;
    uint32 result = count * 96;
    if (packet < end - result && sequence != 0 && (sint32)count > 0)
    {
        uint32 entry = sequence;
        uint32 index = 0;
        do
        {
            uint32 row = r_u32(entry + 4);
            uint32 duration = r_u32(row + 0x1c);
            uint32 quotient = duration == 0 ? 0xffffffff : time / duration;
            uint32 frames = r_u32(row + 0x18);
            uint32 selected = frames == 0 ? quotient : quotient % frames;
            uint32 frame = row + (selected << 3) + 0x20;
            for (uint32 part = 0; part < 2; ++part)
            {
                uint32 cached;
                uint32 value;
                if (part == 0)
                {
                    cached = r_u32(row + 0x10);
                    value = r_u32(frame);
                }
                else
                {
                    value = r_u32(frame + 4);
                    cached = r_u32(row + 0x14);
                }
                if (value != cached)
                {
                    w_u32(packet + 4, 0x01000000);
                    w_u32(packet + 8, 0x80000000);
                    value = r_u32(frame + part * 4);
                    w_u32(packet + 0xc, value);
                    value = r_u32(row + part * 8);
                    w_u32(packet + 0x10, value);
                    value = r_u32(row + part * 8 + 4);
                    w_u32(packet + 0x14, value);
                    value = r_u32(order);
                    w_u32(packet, (value & 0xffffff) | 0x05000000);
                    w_u32(order, packet & 0xffffff);
                    value = r_u32(frame + part * 4);
                    w_u32(row + 0x10 + part * 4, value);
                    value = r_u32(packets);
                    packet += 0x18;
                    w_u32(packets, value + 1);
                }
            }
            count = r_u32(sequence);
            ++index;
            result = (sint32)index < (sint32)count ? 1 : 0;
            entry += 4;
        } while (result != 0);
    }
    w_u32(packets + 4, packet);
    return result;
}

uint32 G3Function8003420C(uint32 game, uint32 order_table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003420c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x68;
    uint32 result = r_u32(game + 0x5c);
    uint32 camera = r_u32(game + 0x4c);
    uint32 vertices = r_u32(game + 0x64);
    uint32 packets = r_u32(game + 0x68);
    uint32 object = r_u32(result + 4);
    uint32 scene = r_u32(camera + 0x78);
    w_u32(game + 0x65d0, 0);
    while (object != 0)
    {
        result = r_u32(game + 0x6834) & 0x400;
        if (result != 0)
        {
            result = r_u32(object + 0x14) & 0x02000000;
            if (result == 0)
            {
                object = r_u32(object + 8);
                continue;
            }
        }
        uint32 descriptor = r_u32(object + 0x1c);
        uint32 model_index = (uint32)r_s16(object + 0xa2);
        uint32 models = r_u32(descriptor + 0xc);
        uint32 model = r_u32(models + (model_index << 2));
        uint32 lower = 0 - (uint32)r_s16(model + 0x26);
        uint32 first = r_u32(camera + 0x20);
        uint32 second = r_u32(camera + 0x24);
        xport_gte_write_control(0, first);
        xport_gte_write_control(1, second);
        first = r_u32(camera + 0x28);
        second = r_u32(camera + 0x2c);
        uint32 third = r_u32(camera + 0x30);
        xport_gte_write_control(2, first);
        xport_gte_write_control(3, second);
        xport_gte_write_control(4, third);
        xport_gte_write_data(0, r_u32(object + 0x58));
        xport_gte_write_data(1, r_u32(object + 0x5c));
        xport_gte_execute(0x486012);
        for (uint32 axis = 0; axis < 3; ++axis)
            w_u32(frame + 0x20 + axis * 4, xport_gte_read_data(25 + axis));
        uint32 depth = r_u32(frame + 0x20);
        uint32 offset = r_u32(camera + 0x60);
        depth -= offset;
        w_u32(frame + 0x20, depth);
        bool visible = false;
        if ((sint32)lower < (sint32)depth)
        {
            uint32 far_limit = r_u32(camera + 0x88) - lower;
            if ((sint32)depth < (sint32)far_limit)
            {
                uint32 component = r_u32(frame + 0x24);
                offset = r_u32(camera + 0x64);
                if ((sint32)lower < (sint32)(component - offset))
                {
                    component = r_u32(frame + 0x28);
                    offset = r_u32(camera + 0x68);
                    if ((sint32)lower < (sint32)(component - offset))
                    {
                        first = r_u32(camera + 0x40);
                        second = r_u32(camera + 0x44);
                        xport_gte_write_control(0, first);
                        xport_gte_write_control(1, second);
                        first = r_u32(camera + 0x48);
                        second = r_u32(camera + 0x4c);
                        third = r_u32(camera + 0x50);
                        xport_gte_write_control(2, first);
                        xport_gte_write_control(3, second);
                        xport_gte_write_control(4, third);
                        xport_gte_write_data(0, r_u32(object + 0x58));
                        xport_gte_write_data(1, r_u32(object + 0x5c));
                        xport_gte_execute(0x486012);
                        for (uint32 axis = 0; axis < 3; ++axis)
                            w_u32(frame + 0x30 + axis * 4, xport_gte_read_data(25 + axis));
                        component = r_u32(frame + 0x30);
                        offset = r_u32(camera + 0x6c);
                        if ((sint32)lower < (sint32)(component - offset))
                        {
                            component = r_u32(frame + 0x34);
                            offset = r_u32(camera + 0x70);
                            visible = (sint32)lower < (sint32)(component - offset);
                        }
                    }
                }
            }
        }
        uint32 flags = r_u32(object + 0x10);
        w_u32(object + 0x10, visible ? flags | 0x200 : flags & 0xfffffdff);
        if (!visible)
        {
            result = r_u32(object + 0x10) & 0x20000000;
            if (result == 0)
            {
                object = r_u32(object + 8);
                continue;
            }
        }
        bool generic = (r_u32(object + 0x10) & 0x800) == 0;
        if (generic)
            generic = r_u32(object + 0x30) != 0;
        bool callback = false;
        if (generic)
        {
            descriptor = r_u32(object + 0x1c);
            uint32 signed_depth = (uint32)r_s16(frame + 0x20);
            model = G3Function800338E4(object, descriptor, signed_depth);
            uint32 sequence = r_u32(model + 0x1c);
            uint32 time = (uint32)r_s16(object + 0x9e);
            G3Function80034FA4(sequence, time, 0, 0);
            G3Function8002B168(object, frame);
            bool skip_draw = (r_u32(object + 0x10) & 0x20000000) != 0;
            if (skip_draw)
                skip_draw = (r_u32(object + 0x14) & 0x40000000) != 0;
            if (!skip_draw)
            {
                bool alternate = r_u32(object + 0xf8) != 0;
                if (alternate)
                    alternate = (r_u32(object + 0x14) & 0x00800000) == 0;
                w_u32(frame + 0x10, order_table);
                w_u32(frame + 0x14, 0);
                signed_depth = (uint32)r_s16(frame + 0x20);
                w_u32(frame + 0x18, signed_depth);
                if (alternate)
                    G3Function80034BD0(object, scene, vertices, packets, order_table, 0, signed_depth, frame);
                else
                    G3Function80033BA8(object, scene, vertices, packets, order_table, 0, signed_depth, frame);
            }
            uint32 current_game = r_u32(0x80097048);
            uint32 overlay = r_u32(current_game + 0x6788);
            if (overlay != 0)
            {
                bool draw_overlay = (r_u32(object + 0x10) & 0x20000000) != 0;
                if (!draw_overlay)
                {
                    descriptor = r_u32(object + 0x1c);
                    draw_overlay = (r_u32(descriptor + 0x30) & 0x00800000) != 0;
                }
                if (draw_overlay)
                {
                    w_u32(frame + 0x10, order_table);
                    overlay = r_u32(current_game + 0x6788);
                    w_u32(frame + 0x18, 10000);
                    w_u32(frame + 0x14, overlay);
                    G3Function80033BA8(object, scene, vertices, packets, order_table, overlay, 10000, frame);
                }
            }
            if ((r_u32(object + 0x14) & 0x40) != 0 && (r_u32(object + 0x10) & 0x40000000) == 0)
                G3Function8002B4B8(scene, object, packets, order_table, frame);
            callback = r_u32(object + 0xf0) != 0;
        }
        else
        {
            result = r_u32(object + 0x18) & 0x400;
            if (result == 0)
            {
                object = r_u32(object + 8);
                continue;
            }
            result = r_u32(object + 0xf0);
            if (result == 0)
            {
                object = r_u32(object + 8);
                continue;
            }
            callback = true;
        }
        if (callback)
        {
            w_u32(frame + 0x10, order_table);
            uint32 target = r_u32(object + 0xf0);
            G3DispatchObjectRender(target, scene, object, vertices, packets, order_table, frame);
        }
        result = r_u32(game + 0x65d0) + 1;
        w_u32(game + 0x65d0, result);
        object = r_u32(object + 8);
    }
    return result;
}

uint32 G3Function800338E4(uint32 object, uint32 descriptor, uint32 depth)
{
    FUNCTION_MARKER(0x800338e4, "SLUS_008.06");
    uint32 thresholds = r_u32(descriptor + 0x40);
    uint32 index = (uint32)r_s16(object + 0xa2);
    if (thresholds != 0)
    {
        uint32 count = r_u16(thresholds);
        thresholds += 2;
        if (count != 0)
        {
            sint32 signed_depth = (sint32)(depth << 16) >> 16;
            do
            {
                sint32 threshold = r_s16(thresholds);
                thresholds += 2;
                if (threshold >= signed_depth)
                    break;
                uint32 stride = (uint32)r_s16(descriptor + 8);
                --count;
                index += stride;
            } while ((count & 0xffff) != 0);
        }
    }
    uint32 models = r_u32(descriptor + 0xc);
    return r_u32(models + (index << 2));
}

uint32 G3Function80034BD0(uint32 object, uint32 scene, uint32 vertices, uint32 packets, uint32 order_table, uint32 overlay, uint32 depth, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80034bd0, "SLUS_008.06");
    uint32 frame = guest_stack - 0xa8;
    w_u32(guest_stack + 8, vertices);
    uint32 descriptor = r_u32(object + 0x1c);
    depth = (uint32)((sint32)(depth << 16) >> 16);
    uint32 model = G3Function800338E4(object, descriptor, depth);
    sint32 layer = r_s16(object + 0xce);
    vertices = r_u32(guest_stack + 8);
    uint32 flags = r_u32(object + 0x18);
    uint32 matrices = r_u32(object + 0x30);
    uint32 model_vertices = r_u32(model + 8);
    w_u32(0x8009d918, layer > 0 ? 1 : 0);
    uint32 position;
    if ((flags & 1) != 0)
    {
        position = r_u32(object + 0xf8);
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 value = r_u16(position + 8 + axis * 2);
            w_u16(frame + 0x78 + axis * 2, value);
        }
    }
    else
    {
        uint32 game = r_u32(0x80097048);
        uint32 level = r_u32(game + 0x48);
        uint32 attachment = r_u32(object + 0xf8);
        uint32 terrain = r_u32(level);
        uint32 vertex_index = r_u16(attachment);
        uint32 normal_index = (uint32)r_s16(attachment + 8);
        uint32 terrain_vertices = r_u32(terrain + 0x30);
        uint32 normals = r_u32(terrain + 0x40);
        position = terrain_vertices + (vertex_index << 3);
        G3Function8005F304(normal_index, normals, frame + 0x78, frame);
    }
    G3Function8003465C(frame + 0x18, frame + 0x78, position, frame);
    G3Function800313E0(frame + 0x38, frame + 0x18, frame);
    uint32 first = r_u32(scene);
    uint32 second = r_u32(scene + 4);
    xport_gte_write_control(0, first);
    xport_gte_write_control(1, second);
    first = r_u32(scene + 8);
    second = r_u32(scene + 0xc);
    uint32 third = r_u32(scene + 0x10);
    xport_gte_write_control(2, first);
    xport_gte_write_control(3, second);
    xport_gte_write_control(4, third);
    for (uint32 column = 0; column < 3; ++column)
    {
        first = r_u16(frame + 0x38 + column * 2);
        second = r_u16(frame + 0x3e + column * 2);
        third = r_u16(frame + 0x44 + column * 2);
        xport_gte_write_data(9, first);
        xport_gte_write_data(10, second);
        xport_gte_write_data(11, third);
        xport_gte_execute(0x49e012);
        first = xport_gte_read_data(9);
        second = xport_gte_read_data(10);
        third = xport_gte_read_data(11);
        w_u16(frame + 0x58 + column * 2, first);
        w_u16(frame + 0x5e + column * 2, second);
        w_u16(frame + 0x64 + column * 2, third);
    }
    first = r_u32(scene + 0x14);
    second = r_u32(scene + 0x18);
    xport_gte_write_control(5, first);
    third = r_u32(scene + 0x1c);
    xport_gte_write_control(6, second);
    xport_gte_write_control(7, third);
    uint32 y = r_u16(frame + 0x50);
    uint32 x = r_u16(frame + 0x4c);
    xport_gte_write_data(0, x | (y << 16));
    xport_gte_write_data(1, r_u32(frame + 0x54));
    xport_gte_execute(0x480012);
    for (uint32 axis = 0; axis < 3; ++axis)
        w_u32(frame + 0x6c + axis * 4, xport_gte_read_data(25 + axis));
    w_u32(frame + 0x10, matrices);
    w_u32(frame + 0x14, overlay);
    G3Function80034730(model_vertices, vertices, model, frame + 0x18, matrices);
    if (r_u32(0x8009d918) != 0)
    {
        w_u32(frame + 0x10, overlay);
        G3Function8003499C(model_vertices, model, scene, matrices);
    }
    G3Function80033A54(0);
    uint32 faces = (uint32)r_s16(model + 4);
    uint32 limit = r_u32(packets + 8);
    uint32 cursor = r_u32(packets + 4);
    if (cursor + faces * 48 >= limit)
        return 0;
    flags = r_u32(object + 0x10);
    uint32 game = r_u32(0x80097048);
    uint32 lighting = r_u32(0x8009d918);
    w_u32(game + 4, ((flags >> 30) ^ 1) & 1);
    if (lighting != 0)
    {
        uint32 level = r_u32(game + 0x48);
        layer = r_s16(object + 0xce);
        uint32 available = r_u32(level + 0xf8);
        if ((sint32)available < layer)
            G3Function80033950();
        else if (layer > 0)
        {
            uint32 table = r_u32(level + 0xf4);
            uint32 selected = table + (uint32)layer * 20;
            uint32 words[4];
            for (uint32 word = 0; word < 4; ++word)
                words[word] = r_u32(selected + word * 4);
            for (uint32 word = 0; word < 4; ++word)
                w_u32(table + word * 4, words[word]);
            uint32 last = r_u32(selected + 0x10);
            w_u32(table + 0x10, last);
        }
        G3Function80033A54(1);
        game = r_u32(0x80097048);
    }
    vertices = r_u32(guest_stack + 8);
    w_u32(frame + 0x10, packets);
    w_u32(frame + 0x14, order_table);
    uint32 target = r_u32(game + 0x65dc);
    uint32 result = G3DispatchPrimitiveRender(target, model, object, vertices, frame + 0x58, packets, order_table, frame);
    w_u32(packets + 4, result);
    return result;
}

uint32 G3Function80033950(void)
{
    FUNCTION_MARKER(0x80033950, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    uint32 entry = r_u32(level + 0xf4);
    w_u8(entry + 4, 0xff);
    w_u8(entry, 0);
    w_u8(entry + 1, 0);
    w_u8(entry + 5, 0xef);
    game = r_u32(0x80097048);
    uint32 mode = r_u32(game + 0x54);
    uint32 page = mode != 0 ? 0x112 : 0x102;
    w_u16(entry + 0x10, 0x1000);
    w_u16(entry + 6, page);
    w_u16(entry + 0x12, 0x800);
    return 0x800;
}

void G3Function80033A54(uint32 enabled)
{
    FUNCTION_MARKER(0x80033a54, "SLUS_008.06");
    if (enabled == 0)
    {
        w_u32(0x1f800060, 0);
        return;
    }
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    uint32 entry = r_u32(level + 0xf4);
    uint32 right = r_u8(entry + 4);
    uint32 left = r_u8(entry);
    uint32 scale = (uint32)r_s16(entry + 0x12);
    uint32 half_width = (uint32)((sint32)(right - left + 1) >> 1);
    uint32 horizontal_product = scale * half_width;
    uint32 bottom = r_u8(entry + 5);
    uint32 top = r_u8(entry + 1);
    uint32 half_height = (uint32)((sint32)(bottom - top + 1) >> 1);
    uint32 vertical_product = scale * half_height;
    w_u32(0x1f800048, top + half_height);
    w_u32(0x1f80004c, (uint32)((sint32)horizontal_product >> 11));
    w_u32(0x1f800050, (uint32)((sint32)vertical_product >> 11));
    uint32 value = (uint32)r_s16(entry + 2) << 16;
    w_u32(0x1f800054, value);
    uint32 page = (uint32)r_s16(entry + 6);
    uint32 normals = r_u32(0x800a2188);
    w_u32(0x1f800044, left + half_width);
    w_u32(0x1f80005c, normals);
    w_u32(0x1f800060, 0xff);
    w_u32(0x1f800058, page << 16);
}

uint32 G3Function80033BA8(uint32 object, uint32 scene, uint32 vertices, uint32 packets, uint32 order_table, uint32 overlay, uint32 depth, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80033ba8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x58;
    uint32 descriptor = r_u32(object + 0x1c);
    depth = (uint32)((sint32)(depth << 16) >> 16);
    uint32 model = G3Function800338E4(object, descriptor, depth);
    uint32 current_descriptor = r_u32(object + 0x1c);
    uint32 descriptor_flags = r_u32(current_descriptor + 0x3c);
    uint32 matrices = r_u32(object + 0x30);
    if ((descriptor_flags & 0x10) != 0)
        G3Function80033B18(descriptor, model);
    uint32 game = r_u32(0x80097048);
    w_u8(game + 0x67c3, 0);
    G3Function80033A54(0);
    if ((r_u32(descriptor + 0x30) & 0x00400000) != 0)
    {
        game = r_u32(0x80097048);
        w_u32(game + 0x14, 1);
    }
    uint32 animated = r_u32(descriptor + 0x10);
    uint32 vertex_count = r_u16(model);
    uint32 source = r_u32(model + 8);
    animated = animated != 0 ? 1 : 0;
    if (vertex_count < 0x72)
        vertices = 0x1f80006c;
    bool lighting = r_s16(object + 0xce) > 0;
    if (!lighting)
        lighting = (r_u32(object + 0x18) & 0x20) != 0;
    game = r_u32(0x80097048);
    uint32 mode = r_u32(game + 0xe8);
    w_u32(0x8009d918, lighting ? 1 : 0);
    if ((mode & 0x800) != 0 || overlay != 0)
    {
        w_u32(frame + 0x10, matrices);
        w_u32(frame + 0x14, overlay);
        w_u32(frame + 0x18, animated);
        G3Function800333F4(source, vertices, model, scene, matrices, overlay, animated, frame);
    }
    else
    {
        w_u32(frame + 0x10, matrices);
        G3Function800194C4(source, vertices, model, scene, matrices, frame);
    }
    do
    {
        if (overlay != 0)
        {
            game = r_u32(0x80097048);
            uint32 displacement = r_u32(game + 0x68d0);
            w_u32(game, 1);
            if (displacement == 0)
            {
                uint32 target = r_u32(game + 0x65d8);
                uint32 cursor = G3DispatchPolygonRender(target, model, vertices, packets, order_table, frame);
                w_u32(packets + 4, cursor);
            }
            else
            {
                uint32 camera = r_u32(game + 0x4c);
                uint32 first = r_u32(camera + 0x20);
                uint32 second = r_u32(camera + 0x24);
                xport_gte_write_control(0, first);
                xport_gte_write_control(1, second);
                first = r_u32(camera + 0x28);
                second = r_u32(camera + 0x2c);
                uint32 third = r_u32(camera + 0x30);
                xport_gte_write_control(2, first);
                xport_gte_write_control(3, second);
                xport_gte_write_control(4, third);
                xport_gte_write_data(0, r_u32(object + 0x58));
                xport_gte_write_data(1, r_u32(object + 0x5c));
                xport_gte_execute(0x486012);
                for (uint32 axis = 0; axis < 3; ++axis)
                    w_u32(frame + 0x20 + axis * 4, xport_gte_read_data(25 + axis));
                game = r_u32(0x80097048);
                camera = r_u32(game + 0x4c);
                uint32 distance = r_u32(frame + 0x20);
                uint32 near_offset = r_u32(camera + 0x60);
                distance -= near_offset;
                w_u32(frame + 0x20, distance);
                displacement = r_u32(game + 0x68d0);
                camera = r_u32(game + 0x4c);
                uint32 radius = (uint32)r_s16(model + 0x26);
                uint32 far_limit = r_u32(camera + 0x88);
                if ((sint32)(distance + displacement) < (sint32)(far_limit - radius))
                {
                    uint32 quarter = (uint32)((sint32)displacement >> 2);
                    uint32 adjusted_order = order_table + (quarter << 2);
                    uint32 previous_order = r_u32(game + 0x28);
                    uint32 target = r_u32(game + 0x65d8);
                    w_u32(game + 0x28, previous_order - quarter);
                    uint32 cursor = G3DispatchPolygonRender(target, model, vertices, packets, adjusted_order, frame);
                    game = r_u32(0x80097048);
                    w_u32(packets + 4, cursor);
                    w_u32(game + 0x28, previous_order);
                }
            }
            game = r_u32(0x80097048);
            w_u32(game, 0);
            break;
        }
        if ((r_u32(object + 0x14) & 0x80) != 0)
        {
            game = r_u32(0x80097048);
            uint32 red = r_u8(game + 0x67bc);
            w_u32(frame + 0x10, red);
            uint32 green = r_u8(game + 0x67bd);
            w_u32(frame + 0x14, green);
            uint32 blue = r_u8(game + 0x67be);
            w_u32(frame + 0x18, blue);
            uint32 cursor = G3Function8001D544(model, vertices, packets, order_table, red, green, blue, frame);
            w_u32(packets + 4, cursor);
        }
        current_descriptor = r_u32(object + 0x1c);
        bool fade = (r_u32(current_descriptor + 0x3c) & 2) != 0;
        if (!fade)
            fade = (r_u32(object + 0x18) & 0x1000) != 0;
        uint32 fade_flags = 0;
        if (fade)
        {
            fade_flags = r_u32(object + 0x18);
            fade = (fade_flags & 0x20) == 0;
        }
        if (fade)
        {
            game = r_u32(0x80097048);
            uint32 camera = r_u32(game + 0x4c);
            uint32 alpha;
            if ((fade_flags & 0x1000) != 0)
            {
                if (r_u8(object + 0x93) == 0)
                {
                    alpha = 255;
                    if (r_u8(object + 0x91) != 0)
                        w_u32(object + 0x18, fade_flags & 0xffffefff);
                    else
                    {
                        uint32 owner = r_u32(object + 0x20);
                        alpha = 0;
                        if (owner != 0)
                        {
                            uint32 flags = r_u32(owner + 0x18);
                            w_u32(owner + 0x18, flags | 0x80);
                        }
                        uint32 flags = r_u32(object + 0x10);
                        w_u32(object + 0x10, (flags & 0xfffffbff) | 0x30);
                    }
                }
                else
                {
                    uint32 direction = r_u8(object + 0x91);
                    alpha = r_u8(object + 0x90);
                    uint32 delta = r_u8(object + 0x92);
                    w_u8(object + 0x90, direction == 0 ? alpha - delta : alpha + delta);
                    uint32 remaining = r_u8(object + 0x93);
                    w_u8(object + 0x93, remaining - 1);
                }
            }
            else
            {
                uint32 object_x = (uint32)r_s16(object + 0x58);
                uint32 camera_x = (uint32)r_s16(camera);
                uint32 dx = object_x - camera_x;
                uint32 x_square = dx * dx;
                uint32 object_y = (uint32)r_s16(object + 0x5a);
                uint32 camera_y = (uint32)r_s16(camera + 2);
                uint32 dy = object_y - camera_y;
                uint32 y_square = dy * dy;
                uint32 camera_z = (uint32)r_s16(camera + 4);
                uint32 object_z = (uint32)r_s16(object + 0x5c);
                uint32 dz = object_z - camera_z;
                uint32 root = G3Function8002F368(x_square + y_square + dz * dz, frame);
                current_descriptor = r_u32(object + 0x1c);
                sint32 limit = r_s16(current_descriptor + 0x16);
                sint32 distance = (sint32)root >> 6;
                sint32 quarter = (limit < 0 ? limit + 3 : limit) >> 2;
                sint32 start = limit - quarter;
                alpha = 255;
                if (limit < distance)
                    alpha = 0xffffffff;
                else if (distance >= start)
                {
                    uint32 delta = (uint32)(distance - start);
                    uint32 numerator = (delta << 8) - delta;
                    uint32 quotient;
                    if (quarter == 0)
                        quotient = (sint32)numerator < 0 ? 1 : 0xffffffff;
                    else
                        quotient = (uint32)((sint64)(sint32)numerator / (sint64)quarter);
                    alpha = 255 - quotient;
                }
            }
            if (alpha != 255)
            {
                if ((sint32)alpha >= 0)
                {
                    w_u32(frame + 0x10, alpha);
                    w_u32(frame + 0x14, alpha);
                    w_u32(frame + 0x18, alpha);
                    uint32 cursor = G3Function8001D770(model, vertices, packets, order_table, alpha, alpha, alpha, frame);
                    w_u32(packets + 4, cursor);
                }
                break;
            }
        }
        else
        {
            if ((r_u32(object + 0x14) & 0x100) != 0)
            {
                game = r_u32(0x80097048);
                uint32 red = r_u8(game + 0x67c0);
                w_u32(frame + 0x10, red);
                uint32 green = r_u8(game + 0x67c1);
                w_u32(frame + 0x14, green);
                uint32 blue = r_u8(game + 0x67c2);
                w_u32(frame + 0x18, blue);
                uint32 cursor = G3Function8001D770(model, vertices, packets, order_table, red, green, blue, frame);
                w_u32(packets + 4, cursor);
                break;
            }
            sint32 layer = r_s16(object + 0xce);
            bool setup_lighting = layer > 0;
            if (!setup_lighting)
                setup_lighting = (r_u32(object + 0x18) & 0x20) != 0;
            if (setup_lighting)
            {
                if ((r_u32(object + 0x18) & 0x20) != 0)
                    G3Function800339B8(frame);
                else
                {
                    game = r_u32(0x80097048);
                    uint32 level = r_u32(game + 0x48);
                    uint32 available = r_u32(level + 0xf8);
                    if ((sint32)available < layer)
                        G3Function80033950();
                    else
                    {
                        uint32 table = r_u32(level + 0xf4);
                        uint32 selected = table + (uint32)layer * 20;
                        uint32 words[4];
                        for (uint32 word = 0; word < 4; ++word)
                            words[word] = r_u32(selected + word * 4);
                        for (uint32 word = 0; word < 4; ++word)
                            w_u32(table + word * 4, words[word]);
                        uint32 last = r_u32(selected + 0x10);
                        w_u32(table + 0x10, last);
                    }
                }
                G3Function80033A54(1);
                if ((r_u32(object + 0x18) & 0x30) != 0)
                {
                    uint32 cursor = G3Function8001D180(model, vertices, packets, order_table, frame);
                    w_u32(packets + 4, cursor);
                    break;
                }
            }
        }
        game = r_u32(0x80097048);
        uint32 target = r_u32(game + 0x65d8);
        uint32 cursor = G3DispatchPolygonRender(target, model, vertices, packets, order_table, frame);
        w_u32(packets + 4, cursor);
    } while (false);
    game = r_u32(0x80097048);
    w_u32(game + 0x14, 0);
    return game;
}

uint32 G3Function80033B18(uint32 descriptor, uint32 model)
{
    FUNCTION_MARKER(0x80033b18, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 face = r_u32(model + 0x14);
    uint32 mode = r_u32(game + 0x54);
    uint32 count = (uint32)r_s16(model + 4);
    uint32 page_bit = mode != 0 ? 0x10 : 0;
    uint32 end = face + count * 12;
    while (face < end)
    {
        if ((r_u8(face + 7) & 2) != 0)
        {
            uint32 texture = r_u32(face + 8);
            uint32 page = r_u16(texture + 6);
            if ((page & 15) < 5)
                w_u16(texture + 6, (page & 0xffffffef) | page_bit);
        }
        face += 12;
    }
    return 0;
}

uint32 G3Function800339B8(uint32 guest_stack)
{
    FUNCTION_MARKER(0x800339b8, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    uint32 entry = r_u32(level + 0xf4);
    w_u8(entry, 0);
    w_u8(entry + 1, 0);
    w_u8(entry + 4, 0x7f);
    w_u8(entry + 5, 0x7f);
    uint32 random = G3SdkRandom80027FA8();
    w_u16(entry + 6, 0x98);
    w_u16(entry + 0x10, 0x1000);
    w_u16(entry + 0x12, 0x800);
    uint32 remainder = (uint32)((sint32)random % 24);
    uint32 result = ((remainder + 0x110) << 6) | 0x20;
    w_u16(entry + 2, result);
    return result;
}

void G3Function8002B168(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002b168, "SLUS_008.06");
    uint32 frame = guest_stack - 0x90;
    if (r_u32(object + 0x30) == 0)
        return;
    uint32 descriptor = r_u32(object + 0x1c);
    if ((r_u32(descriptor + 0x30) & 0x00400000) != 0)
        return;
    if ((r_u32(descriptor) & 0x40) != 0)
        return;
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    uint32 lights = r_u32(level + 4);
    uint32 scale = r_u32(game + 0x69c0);
    uint32 index = (uint32)r_s16(object + 0x8e);
    uint32 table = r_u32(lights + 8);
    w_u32(frame + 0x78, scale);
    w_u32(frame + 0x74, scale);
    w_u32(frame + 0x70, scale);
    bool custom = r_u32(object + 0xd0) != 0;
    uint32 row = table + (index << 6);
    if (custom)
    {
        for (uint32 component = 0; component < 6; ++component)
        {
            uint32 value = r_u16(row + component * 2);
            w_u16(frame + 0x30 + component * 2, value);
        }
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 component = (uint32)r_s16(object + 0xd4 + axis * 2);
            uint32 factor = (uint32)r_s16(object + 0xda);
            uint32 product = component * factor;
            w_u16(frame + 0x3c + axis * 2, (uint32)((sint32)product >> 12));
        }
        for (uint32 column = 0; column < 2; ++column)
        {
            for (uint32 axis = 0; axis < 3; ++axis)
            {
                uint32 value = r_u16(row + 0x20 + column * 2 + axis * 6);
                w_u16(frame + 0x50 + column * 2 + axis * 6, value);
            }
        }
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 attachment = r_u32(object + 0xd0);
            uint32 value = r_u8(attachment + 8 + axis);
            w_u16(frame + 0x54 + axis * 6, value << 4);
        }
        uint32 matrices = r_u32(object + 0x30);
        MulMatrix0((MATRIX *)psx_addr(frame + 0x30, 20), (MATRIX *)psx_addr(matrices, 20), (MATRIX *)psx_addr(frame + 0x10, 20));
        SetLightMatrix((MATRIX *)psx_addr(frame + 0x10, 20));
        if ((r_u32(object + 0x10) & 0x200000) != 0)
            G3Function8005ADF0(frame + 0x50, frame + 0x70);
        SetColorMatrix((MATRIX *)psx_addr(frame + 0x50, 20));
    }
    else
    {
        uint32 matrices = r_u32(object + 0x30);
        MulMatrix0((MATRIX *)psx_addr(row, 20), (MATRIX *)psx_addr(matrices, 20), (MATRIX *)psx_addr(frame + 0x10, 20));
        SetLightMatrix((MATRIX *)psx_addr(frame + 0x10, 20));
        if ((r_u32(object + 0x10) & 0x200000) != 0)
        {
            for (uint32 column = 0; column < 3; ++column)
            {
                for (uint32 axis = 0; axis < 3; ++axis)
                {
                    uint32 value = r_u16(row + 0x20 + column * 2 + axis * 6);
                    w_u16(frame + 0x50 + column * 2 + axis * 6, value);
                }
            }
            G3Function8005ADF0(frame + 0x50, frame + 0x70);
            SetColorMatrix((MATRIX *)psx_addr(frame + 0x50, 20));
        }
        else
            SetColorMatrix((MATRIX *)psx_addr(row + 0x20, 20));
    }
}

uint32 G3Function8005ADF0(uint32 matrix, uint32 scale)
{
    FUNCTION_MARKER(0x8005adf0, "SLUS_008.06");
    uint32 first_word = r_u32(matrix);
    uint32 second_word = r_u32(matrix + 4);
    uint32 third_word = r_u32(matrix + 8);
    uint32 fourth_word = r_u32(matrix + 0xc);
    xport_gte_write_data(8, r_u32(scale));
    xport_gte_write_data(9, (uint32)((sint32)(first_word << 16) >> 16));
    xport_gte_write_data(10, (uint32)((sint32)second_word >> 16));
    xport_gte_write_data(11, (uint32)((sint32)(fourth_word << 16) >> 16));
    xport_gte_execute(0x198003d);
    uint32 first_x = xport_gte_read_data(9);
    uint32 first_y = xport_gte_read_data(10);
    uint32 first_z = xport_gte_read_data(11);
    xport_gte_write_data(8, r_u32(scale + 4));
    xport_gte_write_data(9, (uint32)((sint32)first_word >> 16));
    xport_gte_write_data(10, (uint32)((sint32)(third_word << 16) >> 16));
    xport_gte_write_data(11, (uint32)((sint32)fourth_word >> 16));
    w_u16(matrix, first_x);
    xport_gte_execute(0x198003d);
    uint32 last = (uint32)r_s16(matrix + 0x10);
    w_u16(matrix + 6, first_y);
    w_u16(matrix + 0xc, first_z);
    uint32 second_x = xport_gte_read_data(9);
    uint32 second_y = xport_gte_read_data(10);
    uint32 second_z = xport_gte_read_data(11);
    xport_gte_write_data(8, r_u32(scale + 8));
    xport_gte_write_data(9, (uint32)((sint32)(second_word << 16) >> 16));
    xport_gte_write_data(10, (uint32)((sint32)third_word >> 16));
    xport_gte_write_data(11, last);
    w_u16(matrix + 2, second_x);
    xport_gte_execute(0x198003d);
    w_u16(matrix + 8, second_y);
    w_u16(matrix + 0xe, second_z);
    uint32 third_x = xport_gte_read_data(9);
    uint32 third_y = xport_gte_read_data(10);
    uint32 third_z = xport_gte_read_data(11);
    w_u16(matrix + 4, third_x);
    w_u16(matrix + 0xa, third_y);
    w_u16(matrix + 0x10, third_z);
    return matrix;
}

uint32 G3Function8001D544(uint32 model, uint32 vertices, uint32 packets, uint32 order_table, uint32 red, uint32 green, uint32 blue, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8001D544, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 face = r_u32(model + 0x14);
    uint32 packet = r_u32(packets + 4);
    uint32 end = face + (uint32)r_s16(model + 4) * 12;
    uint32 color = (red & 0xff) | ((green & 0xff) << 8) | ((blue & 0xff) << 16) | 0x4e000000;
    while (face < end)
    {
        if (r_u32(packets + 8) - 0x30 < packet)
            break;
        uint32 first = vertices + ((uint32)r_u16(face) << 3);
        uint32 second = vertices + ((uint32)r_u16(face + 2) << 3);
        uint32 third_index = r_u16(face + 4);
        uint32 second_depth = r_u32(second + 4);
        uint32 third = vertices + (third_index << 3);
        uint32 sum = r_u32(first + 4);
        uint32 third_depth = r_u32(third + 4);
        sum += second_depth;
        sum += third_depth;
        uint32 game = r_u32(0x80097048);
        uint32 flags = r_u8(face + 7);
        uint32 shift = r_u32(game + 0x2c);
        uint32 depth = (uint32)((sint32)sum / 3 >> (shift & 31));
        if ((flags & 2) != 0)
        {
            uint32 texture = r_u32(face + 8);
            depth += (uint32)r_s8(texture + 0xb) * 2;
        }
        if ((sint32)depth < r_s32(game + 0x28) && (sint32)depth >= 0)
        {
            uint32 first_xy = r_u32(first);
            uint32 second_xy = r_u32(second);
            uint32 third_xy = r_u32(third);
            xport_gte_write_data(12, first_xy);
            xport_gte_write_data(14, third_xy);
            xport_gte_write_data(13, second_xy);
            xport_gte_execute(0x1400006);
            w_u32(frame, xport_gte_read_data(24));
            uint32 winding = r_u32(r_u32(0x80097048));
            sint32 area = r_s32(frame);
            if ((winding == 0 && area < 0) || (winding != 0 && area > 0))
            {
                w_u32(packet + 0x1c, 0x55555555);
                w_u32(packet + 4, 0xe1000228);
                w_u32(packet + 8, color);
                w_u32(packet + 0xc, xport_gte_read_data(12));
                w_u32(packet + 0x10, xport_gte_read_data(13));
                w_u32(packet + 0x14, xport_gte_read_data(14));
                uint32 order = order_table + (depth << 2);
                uint32 repeated_xy = r_u32(packet + 0xc);
                w_u32(packet + 0x18, repeated_xy);
                uint32 previous = r_u32(order);
                w_u32(packet, (previous & 0xffffff) | 0x07000000);
                w_u32(order, packet & 0xffffff);
                uint32 count = r_u32(packets);
                packet += 0x20;
                w_u32(packets, count + 1);
            }
        }
        face += 12;
    }
    return packet;
}

uint32 G3Function8001D180(uint32 model, uint32 vertices, uint32 packets, uint32 order_table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8001D180, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 scene = r_u32(r_u32(0x80097048) + 0x48);
    uint32 packet = r_u32(packets + 4);
    uint32 texture = r_u32(scene + 0xf4);
    uint32 face = r_u32(model + 0x14);
    sint32 right = r_u8(texture + 4);
    sint32 left = r_u8(texture);
    sint32 top = r_u8(texture + 1);
    sint32 half_width = (right - left + 1) / 2;
    sint32 bottom = r_u8(texture + 5);
    uint32 center_u = (uint32)(left + half_width);
    sint32 half_height = (bottom - top + 1) / 2;
    uint32 end = face + (uint32)r_s16(model + 4) * 12;
    uint32 center_v = (uint32)(top + half_height);
    while (face < end)
    {
        uint32 first = vertices + ((uint32)r_u16(face) << 3);
        uint32 second = vertices + ((uint32)r_u16(face + 2) << 3);
        uint32 third_index = r_u16(face + 4);
        uint32 first_xy = r_u32(first);
        uint32 second_xy = r_u32(second);
        uint32 third = vertices + (third_index << 3);
        uint32 third_xy = r_u32(third);
        xport_gte_write_data(12, first_xy);
        xport_gte_write_data(14, third_xy);
        xport_gte_write_data(13, second_xy);
        xport_gte_execute(0x1400006);
        w_u32(frame, xport_gte_read_data(24));
        uint32 winding = r_u32(r_u32(0x80097048));
        sint32 area = r_s32(frame);
        if ((winding == 0 && area < 0) || (winding != 0 && area > 0))
        {
            uint32 first_depth = r_u32(first + 4);
            uint32 second_depth = r_u32(second + 4);
            uint32 third_depth = r_u32(third + 4);
            uint32 sum = first_depth + second_depth + third_depth;
            uint32 game = r_u32(0x80097048);
            uint32 shift = r_u32(game + 0x2c);
            sint32 limit = r_s32(game + 0x28);
            sint32 depth = (sint32)sum / 3 >> (shift & 31);
            if (depth < limit && depth >= 0)
            {
                if (r_u32(packets + 8) - 0x30 < packet)
                    return packet;
                w_u32(packet + 8, xport_gte_read_data(12));
                w_u32(packet + 0x10, xport_gte_read_data(13));
                w_u32(packet + 0x18, xport_gte_read_data(14));
                w_u32(packet + 4, 0x80808080);
                w_u8(packet + 3, 7);
                w_u8(packet + 7, 0x24);
                for (uint32 vertex = 0; vertex < 3; ++vertex)
                {
                    uint32 index = r_u16(face + vertex * 2);
                    uint32 normal = r_u32(0x800a2188) + index * 12;
                    uint32 product = r_u32(normal) * (uint32)half_width;
                    w_u8(packet + 0xc + vertex * 8, center_u + (uint32)((sint32)product >> 13));
                    index = r_u16(face + vertex * 2);
                    normal = r_u32(0x800a2188) + index * 12;
                    product = r_u32(normal + 4) * (uint32)half_height;
                    w_u8(packet + 0xd + vertex * 8, center_v + (uint32)((sint32)product >> 13));
                }
                w_u16(packet + 0xe, r_u16(texture + 2));
                w_u16(packet + 0x16, r_u16(texture + 6));
                uint32 order = order_table + ((uint32)depth << 2);
                uint32 previous = r_u32(order);
                w_u32(packet, (previous & 0xffffff) | 0x07000000);
                w_u32(order, packet & 0xffffff);
                uint32 count = r_u32(packets);
                packet += 0x20;
                w_u32(packets, count + 1);
            }
        }
        face += 12;
    }
    return packet;
}

uint32 G3Function800331A8(uint32 destination, uint32 matrix, uint32 plane, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800331A8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    for (uint32 row = 0; row < 3; ++row)
    {
        for (uint32 column = 0; column < 3; ++column)
        {
            sint32 first = r_s16(plane + row * 2);
            sint32 second = row == column ? first : r_s16(plane + column * 2);
            uint32 doubled = (uint32)((first * second) >> 12) << 1;
            w_u16(frame + 0x10 + (row * 3 + column) * 2, (row == column ? 0x1000u : 0u) - doubled);
        }
    }
    MulMatrix0((MATRIX *)psx_addr(frame + 0x10, 20), (MATRIX *)psx_addr(matrix, 20), (MATRIX *)psx_addr(destination, 20));
    uint32 normal_x = (uint32)r_s16(plane);
    uint32 first_product = r_u32(matrix + 0x14) * normal_x;
    uint32 normal_y = (uint32)r_s16(plane + 2);
    uint32 second_product = r_u32(matrix + 0x18) * normal_y;
    uint32 normal_z = (uint32)r_s16(plane + 4);
    uint32 third_product = r_u32(matrix + 0x1c) * normal_z;
    uint32 sum = first_product + second_product + third_product;
    uint32 distance = (uint32)r_s16(plane + 6) << 1;
    distance -= (uint32)((sint32)sum >> 12);
    uint32 product = distance * normal_x;
    uint32 delta_x = (uint32)((sint32)product >> 11);
    w_u32(frame + 0x30, delta_x);
    product = distance * (uint32)r_s16(plane + 2);
    w_u32(frame + 0x34, (uint32)((sint32)product >> 11));
    product = distance * (uint32)r_s16(plane + 4);
    w_u32(frame + 0x38, (uint32)((sint32)product >> 11));
    w_u32(destination + 0x14, r_u32(matrix + 0x14) + delta_x);
    uint32 translation = r_u32(matrix + 0x18);
    uint32 delta = r_u32(frame + 0x34);
    w_u32(destination + 0x18, translation + delta);
    translation = r_u32(matrix + 0x1c);
    delta = r_u32(frame + 0x38);
    uint32 result = translation + delta;
    w_u32(destination + 0x1c, result);
    return result;
}

uint32 G3Function800333F4(uint32 source, uint32 vertices, uint32 model, uint32 scene, uint32 matrices, uint32 overlay, uint32 first_segment, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800333F4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    uint32 normal_output = r_u32(0x800a2188);
    uint32 records = r_u32(model + 0x18);
    uint32 segment = first_segment;
    uint32 record_offset = segment * 24;
    if ((sint32)segment < r_s16(model + 6))
    {
        uint32 second_column = 0x1f800002;
        w_u32(frame + 0x18, frame + 0x10);
        uint32 record = records + record_offset + 0xa;
        w_u32(frame + 0x1c, frame + 0x14);
        do
        {
            uint32 current_record = r_u32(model + 0x18) + record_offset;
            if (r_s16(current_record + 0xa) != -1)
            {
                uint32 start_index = (uint32)r_s16(record - 2);
                uint32 end_index = (uint32)r_s16(record);
                matrices = r_u32(guest_stack + 0x10);
                uint32 vertex = source + (start_index << 3);
                uint32 end = source + (end_index << 3);
                uint32 matrix = matrices + (segment << 5);
                bool reflected = overlay != 0;
                if (reflected)
                {
                    w_u32(frame + 0x20, scene);
                    w_u32(frame + 0x24, second_column);
                    w_u32(frame + 0x28, model);
                    w_u32(frame + 0x2c, overlay);
                    w_u32(frame + 0x30, source);
                    G3Function800331A8(0x1f800020, matrix, overlay, frame);
                    scene = r_u32(frame + 0x20);
                    matrix = 0x1f800020;
                }
                uint32 control0 = r_u32(scene);
                uint32 control1 = r_u32(scene + 4);
                xport_gte_write_control(0, control0);
                xport_gte_write_control(1, control1);
                uint32 control2 = r_u32(scene + 8);
                uint32 control3 = r_u32(scene + 0xc);
                uint32 control4 = r_u32(scene + 0x10);
                xport_gte_write_control(2, control2);
                xport_gte_write_control(3, control3);
                xport_gte_write_control(4, control4);
                for (uint32 column = 0; column < 3; ++column)
                {
                    uint32 input = matrix + column * 2;
                    uint32 x = r_u16(input);
                    uint32 y = r_u16(input + 6);
                    uint32 z = r_u16(input + 0xc);
                    xport_gte_write_data(9, x);
                    xport_gte_write_data(10, y);
                    xport_gte_write_data(11, z);
                    xport_gte_execute(0x49e012);
                    if (column == 1 && reflected)
                        second_column = r_u32(frame + 0x24);
                    x = xport_gte_read_data(9);
                    y = xport_gte_read_data(10);
                    z = xport_gte_read_data(11);
                    uint32 output = column == 1 ? second_column : 0x1f800000 + column * 2;
                    w_u16(output, x);
                    w_u16(output + 6, y);
                    w_u16(output + 0xc, z);
                }
                uint32 translation_x = r_u32(scene + 0x14);
                uint32 translation_y = r_u32(scene + 0x18);
                xport_gte_write_control(5, translation_x);
                uint32 translation_z = r_u32(scene + 0x1c);
                xport_gte_write_control(6, translation_y);
                xport_gte_write_control(7, translation_z);
                uint32 packed_y = r_u16(matrix + 0x18) << 16;
                uint32 packed_x = r_u16(matrix + 0x14);
                xport_gte_write_data(0, packed_x | packed_y);
                xport_gte_write_data(1, r_u32(matrix + 0x1c));
                xport_gte_execute(0x480012);
                w_u32(0x1f800014, xport_gte_read_data(25));
                w_u32(0x1f800018, xport_gte_read_data(26));
                w_u32(0x1f80001c, xport_gte_read_data(27));
                if (reflected)
                {
                    model = r_u32(frame + 0x28);
                    overlay = r_u32(frame + 0x2c);
                    source = r_u32(frame + 0x30);
                }
                control0 = r_u32(0x1f800000);
                control1 = r_u32(0x1f800004);
                xport_gte_write_control(0, control0);
                xport_gte_write_control(1, control1);
                control2 = r_u32(0x1f800008);
                control3 = r_u32(0x1f80000c);
                control4 = r_u32(0x1f800010);
                xport_gte_write_control(2, control2);
                xport_gte_write_control(3, control3);
                xport_gte_write_control(4, control4);
                translation_x = r_u32(0x1f800014);
                translation_y = r_u32(0x1f800018);
                xport_gte_write_control(5, translation_x);
                translation_z = r_u32(0x1f80001c);
                xport_gte_write_control(6, translation_y);
                xport_gte_write_control(7, translation_z);
                while (vertex <= end)
                {
                    xport_gte_write_data(0, r_u32(vertex));
                    xport_gte_write_data(1, r_u32(vertex + 4));
                    xport_gte_execute(0x180001);
                    w_u32(vertices, xport_gte_read_data(14));
                    uint32 flag_output = r_u32(frame + 0x18);
                    w_u32(flag_output, xport_gte_read_control(31));
                    uint32 depth_output = r_u32(frame + 0x1c);
                    w_u32(depth_output, xport_gte_read_data(19));
                    uint32 flags = r_u32(frame + 0x10);
                    if (flags != 0)
                    {
                        uint32 depth = r_u32(frame + 0x14);
                        flags = (uint32)((sint32)flags >> 16);
                        w_u32(frame + 0x10, flags);
                        w_u32(frame + 0x14, depth | (flags << 30));
                    }
                    uint32 depth = r_u32(frame + 0x14);
                    uint32 lighting = r_u32(0x8009d918);
                    w_u32(vertices + 4, depth);
                    if (lighting != 0)
                    {
                        uint32 normal = 0x80098838 + ((uint32)r_u16(vertex + 6) << 3);
                        xport_gte_write_data(0, r_u32(normal));
                        xport_gte_write_data(1, r_u32(normal + 4));
                        xport_gte_execute(0x486012);
                        w_u32(normal_output, xport_gte_read_data(25));
                        w_u32(normal_output + 4, xport_gte_read_data(26));
                        w_u32(normal_output + 8, xport_gte_read_data(27));
                        normal_output += 12;
                    }
                    vertex += 8;
                    vertices += 8;
                }
            }
            record += 24;
            sint32 count = r_s16(model + 6);
            ++segment;
            record_offset += 24;
            if ((sint32)segment >= count)
                break;
        } while (true);
    }
    uint32 result = r_u32(0x80097048) + 0x18;
    w_u32(result, xport_gte_read_data(8));
    return result;
}

uint32 G3Function8002B4B8(uint32 scene, uint32 object, uint32 packets, uint32 order_table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002B4B8, "SLUS_008.06");
    uint32 player = r_u32(0x8009efc4);
    uint32 frame = guest_stack - 0x88;
    uint32 shadow;
    if (object == player)
        shadow = r_u32(0x800986fc);
    else if ((r_u32(r_u32(object + 0x1c)) & 0x200) != 0)
        shadow = r_u32(0x8009870c);
    else
        shadow = r_u32(0x80098704);
    if (shadow == 0)
        return 0x800a0000;
    if (r_u32(object + 0xdc) == 0 && r_s16(object + 0xe0) == 0)
        return 0;
    sint32 height = r_s16(object + 0x5c);
    sint32 ground = r_s16(object + 0xe0);
    uint32 shadow_models = r_u32(shadow + 0xc);
    uint32 model = r_u32(shadow_models);
    if (height - 0x500 >= ground)
    {
        if ((r_u32(r_u32(object + 0x1c)) & 0x400) == 0)
            return 0;
        if (height - r_s16(object + 0x14c) >= ground)
            return 0;
    }
    uint32 normal_y = (uint32)r_s16(object + 0xb6);
    uint32 normal_z = (uint32)r_s16(object + 0xb8);
    uint32 angle = G3Function8002F0C4(normal_y, normal_z, frame);
    w_u16(frame + 0x18, 0u - angle);
    uint32 normal_x = (uint32)r_s16(object + 0xb4);
    uint32 squared = normal_x * normal_x;
    uint32 root = G3Function8002F368((uint32)((sint32)(0x01000000u - squared) >> 12), frame);
    normal_x = (uint32)r_s16(object + 0xb4);
    angle = G3Function8002F0C4(normal_x, root, frame);
    w_u16(frame + 0x1a, angle);
    w_u16(frame + 0x1c, r_u16(object + 0x64));
    G3Function8005BD00(frame + 0x18, frame + 0x20);
    w_u32(frame + 0x34, (uint32)r_s16(object + 0xdc));
    w_u32(frame + 0x38, (uint32)r_s16(object + 0xde));
    w_u32(frame + 0x3c, (uint32)r_s16(object + 0xe0));
    uint32 control0 = r_u32(scene);
    uint32 control1 = r_u32(scene + 4);
    xport_gte_write_control(0, control0);
    xport_gte_write_control(1, control1);
    uint32 control2 = r_u32(scene + 8);
    uint32 control3 = r_u32(scene + 0xc);
    uint32 control4 = r_u32(scene + 0x10);
    xport_gte_write_control(2, control2);
    xport_gte_write_control(3, control3);
    xport_gte_write_control(4, control4);
    for (uint32 column = 0; column < 3; ++column)
    {
        uint32 input = frame + 0x20 + column * 2;
        uint32 x = r_u16(input);
        uint32 y = r_u16(input + 6);
        uint32 z = r_u16(input + 0xc);
        xport_gte_write_data(9, x);
        xport_gte_write_data(10, y);
        xport_gte_write_data(11, z);
        xport_gte_execute(0x49e012);
        x = xport_gte_read_data(9);
        y = xport_gte_read_data(10);
        z = xport_gte_read_data(11);
        uint32 output = frame + 0x40 + column * 2;
        w_u16(output, x);
        w_u16(output + 6, y);
        w_u16(output + 0xc, z);
    }
    uint32 translation_x = r_u32(scene + 0x14);
    uint32 translation_y = r_u32(scene + 0x18);
    xport_gte_write_control(5, translation_x);
    uint32 translation_z = r_u32(scene + 0x1c);
    xport_gte_write_control(6, translation_y);
    xport_gte_write_control(7, translation_z);
    uint32 packed_y = r_u16(frame + 0x38) << 16;
    uint32 packed_x = r_u16(frame + 0x34);
    xport_gte_write_data(0, packed_x | packed_y);
    xport_gte_write_data(1, r_u32(frame + 0x3c));
    xport_gte_execute(0x480012);
    w_u32(frame + 0x54, xport_gte_read_data(25));
    w_u32(frame + 0x58, xport_gte_read_data(26));
    w_u32(frame + 0x5c, xport_gte_read_data(27));
    uint32 scale;
    if (object == r_u32(0x8009efc4))
    {
        uint32 descriptor = r_u32(object + 0x1c);
        uint32 index = (uint32)r_s16(object + 0xa2);
        uint32 models = r_u32(descriptor + 0xc);
        uint32 selected = r_u32(models + (index << 2));
        sint32 radius = r_s16(selected + 0x26);
        uint32 base = (uint32)((radius * 0x1000) / 480);
        sint32 object_height = r_s16(object + 0x5c);
        sint32 ground_height = r_s16(object + 0xe0);
        sint32 attenuation = ((object_height - ground_height) * 0x1000) / 1280;
        uint32 product = base * (0x1000u - (uint32)attenuation);
        scale = (uint32)((sint32)product >> 12);
    }
    else
    {
        uint32 descriptor = r_u32(object + 0x1c);
        if ((r_u32(descriptor) & 0x400) != 0)
        {
            sint32 object_height = r_s16(object + 0x5c);
            sint32 ground_height = r_s16(object + 0xe0);
            sint32 divisor = r_s16(object + 0x14c);
            sint32 numerator = (object_height - ground_height) * 0x1000;
            uint32 quotient = divisor == 0 ? (numerator < 0 ? 1u : 0xffffffffu) : (uint32)(numerator / divisor);
            uint32 tripled = quotient + (quotient << 1);
            uint32 attenuated = 0x1000u - (uint32)((sint32)tripled >> 2);
            uint32 weight = (uint32)r_s16(object + 0x14e);
            uint32 product = attenuated * weight;
            scale = (uint32)((sint32)product >> 7);
        }
        else
        {
            uint32 index = (uint32)r_s16(object + 0xa2);
            uint32 models = r_u32(descriptor + 0xc);
            uint32 selected = r_u32(models + (index << 2));
            uint32 radius = (uint32)r_s16(selected + 0x26) << 12;
            uint32 base = radius / 240;
            sint32 object_height = r_s16(object + 0x5c);
            sint32 ground_height = r_s16(object + 0xe0);
            sint32 attenuation = ((object_height - ground_height) * 0x1000) / 1280;
            uint32 product = base * (0x1000u - (uint32)attenuation);
            scale = product >> 12;
        }
    }
    w_u32(frame + 0x68, scale);
    w_u32(frame + 0x64, scale);
    w_u32(frame + 0x60, scale);
    if ((r_u32(r_u32(object + 0x1c) + 0x3c) & 0x800) != 0)
    {
        uint32 x = r_u32(frame + 0x60);
        uint32 z = r_u32(frame + 0x68);
        w_u32(frame + 0x60, (uint32)((sint32)x >> 1));
        uint32 y = r_u32(frame + 0x64);
        w_u32(frame + 0x68, (uint32)((sint32)z >> 1));
        w_u32(frame + 0x64, (uint32)((sint32)y >> 1));
    }
    G3Function8005ADF0(frame + 0x40, frame + 0x60);
    w_u32(0x1f80007c, 0x1000);
    w_u32(0x1f800074, 0x1000);
    w_u32(0x1f800078, 0);
    w_u32(0x1f800070, 0);
    w_u32(0x1f800088, 0);
    w_u32(0x1f800084, 0);
    w_u32(0x1f800080, 0);
    w_u32(frame + 0x10, frame + 0x40);
    uint32 source = r_u32(model + 8);
    w_u32(0x1f80006c, 0x1000);
    G3Function800194C4(source, 0x1f80008c, model, 0x1f80006c, frame + 0x40, frame);
    uint32 game = r_u32(0x80097048);
    uint32 target = r_u32(game + 0x65d8);
    uint32 result = G3DispatchPolygonRender(target, model, 0x1f80008c, packets, order_table, frame);
    w_u32(packets + 4, result);
    return result;
}

uint32 G3Function800330E4(uint32 object, uint32 matrices, uint32 model)
{
    FUNCTION_MARKER(0x800330E4, "SLUS_008.06");
    uint32 output = matrices + 0x20;
    uint32 previous = r_u32(object + 0x30);
    w_u32(object + 0x30, output);
    w_u32(object + 0x34, previous);
    for (uint32 group = 0; group < 2; ++group)
    {
        uint32 offset = group * 0x10;
        uint32 first = r_u32(previous - 0x20 + offset);
        uint32 second = r_u32(previous - 0x1c + offset);
        uint32 third = r_u32(previous - 0x18 + offset);
        uint32 fourth = r_u32(previous - 0x14 + offset);
        w_u32(matrices + offset, first);
        w_u32(matrices + offset + 4, second);
        w_u32(matrices + offset + 8, third);
        w_u32(matrices + offset + 0xc, fourth);
    }
    sint32 count = r_s16(model + 6);
    if (count <= 0)
        return (uint32)count;
    uint32 index = 0;
    do
    {
        for (uint32 group = 0; group < 2; ++group)
        {
            uint32 offset = group * 0x10;
            uint32 first = r_u32(previous + offset);
            uint32 second = r_u32(previous + offset + 4);
            uint32 third = r_u32(previous + offset + 8);
            uint32 fourth = r_u32(previous + offset + 0xc);
            w_u32(output + offset, first);
            w_u32(output + offset + 4, second);
            w_u32(output + offset + 8, third);
            w_u32(output + offset + 0xc, fourth);
        }
        output += 0x20;
        count = r_s16(model + 6);
        ++index;
        previous += 0x20;
    } while ((sint32)index < count);
    return 0;
}

uint32 G3Function80031EAC(uint32 object)
{
    FUNCTION_MARKER(0x80031EAC, "SLUS_008.06");
    uint32 flags = r_u32(object + 0x18);
    uint32 changed = (flags << 1) & 0x80;
    bool different = false;
    for (uint32 offset = 0; offset < 24; offset += 4)
    {
        uint32 current = r_u32(object + 0x58 + offset);
        uint32 previous = r_u32(object + 0x70 + offset);
        if (current != previous)
        {
            different = true;
            break;
        }
    }
    if (different || r_u32(object + 0x30) == 0 || (flags & 0x100) != 0)
        changed |= 0x40;
    uint32 result = (flags & 0xfffffe3f) | changed;
    w_u32(object + 0x18, result);
    return result;
}

uint32 G3Function800315E8(uint32 object, uint32 matrices, uint32 model, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800315E8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 scaled = 0;
    uint32 segment = r_u32(model + 0x18);
    uint32 previous = r_u32(object + 0x30);
    sint32 rotation_z = r_s16(object + 0x64);
    w_u32(object + 0x30, matrices);
    w_u32(object + 0x34, previous);
    if (rotation_z != 0 || r_u32(object + 0x60) != 0)
        G3Function8005BD00(object + 0x60, 0x1f800090);
    else
    {
        w_u32(0x1f8000a0, 0x1000);
        w_u32(0x1f800098, 0x1000);
        w_u32(0x1f800090, 0x1000);
        w_u32(0x1f80009c, 0);
        w_u32(0x1f800094, 0);
    }
    if (r_u32(object + 0x68) != 0x10001000 || r_s16(object + 0x6c) != 0x1000)
    {
        scaled = 1;
        w_u32(0x1f800028, (uint32)r_s16(object + 0x68));
        w_u32(0x1f80002c, (uint32)r_s16(object + 0x6a));
        w_u32(0x1f800030, (uint32)r_s16(object + 0x6c));
        G3Function8005ADF0(0x1f800090, 0x1f800028);
    }
    w_u32(0x1f8000a4, (uint32)r_s16(object + 0x58));
    w_u32(0x1f8000a8, (uint32)r_s16(object + 0x5a));
    w_u32(0x1f8000ac, (uint32)r_s16(object + 0x5c));
    sint32 count = r_s16(model + 6);
    if (count <= 0)
        return (uint32)count;
    uint32 index = 0;
    do
    {
        if (r_s16(segment + 6) != -1 || r_u32(segment + 0x14) != 0)
        {
            if ((r_u32(segment) & 3) != 0)
            {
                w_u32(frame + 0x10, scaled);
                G3Function80031A14(segment, matrices, 0x1f800090, object, scaled, frame);
            }
            else
            {
                for (uint32 group = 0; group < 2; ++group)
                {
                    uint32 offset = group * 0x10;
                    uint32 first = r_u32(0x1f800090 + offset);
                    uint32 second = r_u32(0x1f800094 + offset);
                    uint32 third = r_u32(0x1f800098 + offset);
                    uint32 fourth = r_u32(0x1f80009c + offset);
                    w_u32(matrices + offset, first);
                    w_u32(matrices + offset + 4, second);
                    w_u32(matrices + offset + 8, third);
                    w_u32(matrices + offset + 0xc, fourth);
                }
            }
            w_u16(matrices + 0x12, scaled);
        }
        matrices += 0x20;
        count = r_s16(model + 6);
        ++index;
        segment += 0x18;
    } while ((sint32)index < count);
    return 0;
}

uint32 G3Function800317F0(uint32 object, uint32 matrices, uint32 model, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800317F0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    bool prepared = false;
    w_u16(frame + 0x18, 0);
    w_u32(frame + 0x54, model);
    uint32 segment = r_u32(model + 0x18);
    uint32 previous = r_u32(object + 0x30);
    w_u32(object + 0x30, matrices);
    w_u32(object + 0x34, previous);
    uint32 current_model = r_u32(frame + 0x54);
    sint32 count = r_s16(current_model + 6);
    if (count <= 0)
        return (uint32)count;
    uint32 index = 0;
    do
    {
        if (r_s16(segment + 6) != -1 || r_u32(segment + 0x14) != 0)
        {
            if ((r_u32(segment) & 3) != 0)
            {
                if (!prepared)
                {
                    if (r_s16(object + 0x64) != 0 || r_u32(object + 0x60) != 0)
                        G3Function8005BD00(object + 0x60, 0x1f800098);
                    else
                    {
                        w_u32(0x1f8000a8, 0x1000);
                        w_u32(0x1f8000a0, 0x1000);
                        w_u32(0x1f800098, 0x1000);
                        w_u32(0x1f8000a4, 0);
                        w_u32(0x1f80009c, 0);
                    }
                    if (r_u32(object + 0x68) != 0x10001000 || r_s16(object + 0x6c) != 0x1000)
                    {
                        uint32 scale_x = (uint32)r_s16(object + 0x68);
                        w_u16(frame + 0x18, 1);
                        w_u32(0x1f800028, scale_x);
                        w_u32(0x1f80002c, (uint32)r_s16(object + 0x6a));
                        w_u32(0x1f800030, (uint32)r_s16(object + 0x6c));
                        G3Function8005ADF0(0x1f800098, 0x1f800028);
                    }
                    w_u32(0x1f8000ac, (uint32)r_s16(object + 0x58));
                    w_u32(0x1f8000b0, (uint32)r_s16(object + 0x5a));
                    uint32 position_z = (uint32)r_s16(object + 0x5c);
                    prepared = true;
                    w_u32(0x1f8000b4, position_z);
                }
                uint32 scaled = r_u16(frame + 0x18);
                w_u32(frame + 0x10, scaled);
                G3Function80031A14(segment, matrices, 0x1f800098, object, scaled, frame);
                w_u16(matrices + 0x12, r_u16(frame + 0x18));
            }
            else
            {
                for (uint32 group = 0; group < 2; ++group)
                {
                    uint32 offset = group * 0x10;
                    uint32 first = r_u32(previous + offset);
                    uint32 second = r_u32(previous + offset + 4);
                    uint32 third = r_u32(previous + offset + 8);
                    uint32 fourth = r_u32(previous + offset + 0xc);
                    w_u32(matrices + offset, first);
                    w_u32(matrices + offset + 4, second);
                    w_u32(matrices + offset + 8, third);
                    w_u32(matrices + offset + 0xc, fourth);
                }
            }
        }
        matrices += 0x20;
        previous += 0x20;
        current_model = r_u32(frame + 0x54);
        count = r_s16(current_model + 6);
        ++index;
        segment += 0x18;
    } while ((sint32)index < count);
    return 0;
}

void G3Function80031A14(uint32 segment, uint32 matrix, uint32 parent_matrix, uint32 object, uint32 scaled, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80031A14, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    if (r_s16(segment + 0x10) != 0 || r_u32(segment + 0xc) != 0)
    {
        uint32 control0 = r_u32(parent_matrix);
        uint32 control1 = r_u32(parent_matrix + 4);
        xport_gte_write_control(0, control0);
        xport_gte_write_control(1, control1);
        uint32 control2 = r_u32(parent_matrix + 8);
        uint32 control3 = r_u32(parent_matrix + 0xc);
        uint32 control4 = r_u32(parent_matrix + 0x10);
        xport_gte_write_control(2, control2);
        xport_gte_write_control(3, control3);
        xport_gte_write_control(4, control4);
        xport_gte_write_data(0, r_u32(segment + 0xc));
        xport_gte_write_data(1, r_u32(segment + 0x10));
        xport_gte_execute(0x486012);
        uint32 x = xport_gte_read_data(9);
        uint32 y = xport_gte_read_data(10);
        uint32 z = xport_gte_read_data(11);
        w_u16(0x1f800008, x);
        w_u16(0x1f80000a, y);
        w_u16(0x1f80000c, z);
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 value = r_u16(0x1f800008 + axis * 2);
            uint32 position = r_u16(object + 0x58 + axis * 2);
            w_u16(0x1f800008 + axis * 2, value + position);
        }
    }
    else
    {
        w_u16(0x1f800008, r_u16(object + 0x58));
        w_u16(0x1f80000a, r_u16(object + 0x5a));
        w_u16(0x1f80000c, r_u16(object + 0x5c));
    }
    uint32 position_x = (uint32)r_s16(0x1f800008);
    uint32 position_y = (uint32)r_s16(0x1f80000a);
    w_u32(0x1f8000e4, position_x);
    uint32 position_z = (uint32)r_s16(0x1f80000c);
    w_u32(0x1f8000e8, position_y);
    w_u32(0x1f8000ec, position_z);
    uint32 flags = r_u32(segment);
    if ((flags & 1) != 0)
    {
        w_u16(0x1f800058, r_u16(parent_matrix + 4));
        uint32 parent_y = r_u16(parent_matrix + 0xa);
        w_u16(0x1f80005a, parent_y);
        uint32 parent_z = r_u16(parent_matrix + 0x10);
        w_u16(0x1f80005c, parent_z);
        uint32 angle = G3Function8002F0C4((uint32)((sint32)(parent_y << 16) >> 16), (uint32)((sint32)(parent_z << 16) >> 16), frame);
        uint32 parent_x = (uint32)r_s16(0x1f800058);
        uint32 square = parent_x * parent_x;
        w_u16(0x1f800010, 0u - angle);
        uint32 root = G3Function8002F368((uint32)((sint32)(0x01000000u - square) >> 12), frame);
        angle = G3Function8002F0C4((uint32)r_s16(0x1f800058), root, frame);
        w_u16(0x1f800012, angle);
        w_u16(0x1f800014, 0);
        G3Function8005BD00(0x1f800010, 0x1f800070);
        TransposeMatrix((MATRIX *)psx_addr(0x1f800070, 18), (MATRIX *)psx_addr(0x1f800038, 18));
        uint32 target_x = r_u16(0x1f800000);
        uint32 origin_x = r_u16(0x1f800008);
        uint32 origin_y = r_u16(0x1f80000a);
        uint32 origin_z = r_u16(0x1f80000c);
        w_u16(0x1f800060, target_x - origin_x);
        uint32 target_y = r_u16(0x1f800002);
        uint32 target_z = r_u16(0x1f800004);
        w_u16(0x1f800062, target_y - origin_y);
        w_u16(0x1f800064, target_z - origin_z);
        uint32 control0 = r_u32(0x1f800038);
        uint32 control1 = r_u32(0x1f80003c);
        xport_gte_write_control(0, control0);
        xport_gte_write_control(1, control1);
        uint32 control2 = r_u32(0x1f800040);
        uint32 control3 = r_u32(0x1f800044);
        uint32 control4 = r_u32(0x1f800048);
        xport_gte_write_control(2, control2);
        xport_gte_write_control(3, control3);
        xport_gte_write_control(4, control4);
        xport_gte_write_data(0, r_u32(0x1f800060));
        xport_gte_write_data(1, r_u32(0x1f800064));
        xport_gte_execute(0x486012);
        uint32 x = xport_gte_read_data(9);
        uint32 y = xport_gte_read_data(10);
        uint32 z = xport_gte_read_data(11);
        w_u16(0x1f800068, x);
        w_u16(0x1f80006a, y);
        w_u16(0x1f80006c, z);
        uint32 turn_y = (uint32)r_s16(0x1f80006a);
        uint32 turn_x = (uint32)r_s16(0x1f800068);
        angle = G3Function8002F0C4(turn_y, turn_x, frame);
        w_u16(0x1f800014, angle + 0x400);
        w_u32(0x1f8000e0, 0x1000);
        w_u32(0x1f8000d8, 0x1000);
        w_u32(0x1f8000d0, 0x1000);
        w_u32(0x1f8000dc, 0);
        w_u32(0x1f8000d4, 0);
        if (scaled != 0)
            G3Function8005ADF0(0x1f8000d0, 0x1f800028);
        angle = (uint32)r_s16(0x1f800014);
        if (angle != 0)
            G3Function8005BB6C(angle, 0x1f8000d0, frame);
        angle = (uint32)r_s16(0x1f800012);
        if (angle != 0)
            G3Function8005BA5C(angle, 0x1f8000d0, frame);
        angle = (uint32)r_s16(0x1f800010);
        if (angle != 0)
            G3Function8005B94C(angle, 0x1f8000d0, frame);
    }
    else if ((flags & 2) != 0)
    {
        uint32 origin_x = (uint32)r_s16(0x1f800008);
        uint32 target_x = (uint32)r_s16(0x1f800000);
        uint32 target_y = (uint32)r_s16(0x1f800002);
        w_u32(0x1f800018, target_x - origin_x);
        uint32 origin_y = (uint32)r_s16(0x1f80000a);
        uint32 target_z = (uint32)r_s16(0x1f800004);
        uint32 origin_z = (uint32)r_s16(0x1f80000c);
        w_u32(0x1f80001c, target_y - origin_y);
        w_u32(0x1f800020, target_z - origin_z);
        uint32 length = G3Function8002FD3C(0x1f800018);
        uint32 delta_y = r_u32(0x1f80001c);
        uint32 delta_x = r_u32(0x1f800018);
        uint32 angle = G3Function8002F0C4(delta_y, delta_x, frame);
        uint32 delta_z = r_u32(0x1f800020);
        w_u16(0x1f800014, angle + 0x400);
        angle = G3Function8002F0C4(length, delta_z, frame);
        w_u16(0x1f800010, angle + 0xc00);
        w_u32(0x1f8000e0, 0x1000);
        w_u32(0x1f8000d8, 0x1000);
        w_u32(0x1f8000d0, 0x1000);
        w_u32(0x1f8000dc, 0);
        w_u32(0x1f8000d4, 0);
        if (scaled != 0)
            G3Function8005ADF0(0x1f8000d0, 0x1f800028);
        angle = (uint32)r_s16(object + 0x62);
        if (angle != 0)
            G3Function8005BA5C(angle, 0x1f8000d0, frame);
        angle = (uint32)r_s16(0x1f800010);
        if (angle != 0)
            G3Function8005B94C(angle, 0x1f8000d0, frame);
        angle = (uint32)r_s16(0x1f800014);
        if (angle != 0)
            G3Function8005BB6C(angle, 0x1f8000d0, frame);
    }
    for (uint32 group = 0; group < 2; ++group)
    {
        uint32 offset = group * 0x10;
        uint32 first = r_u32(0x1f8000d0 + offset);
        uint32 second = r_u32(0x1f8000d4 + offset);
        uint32 third = r_u32(0x1f8000d8 + offset);
        uint32 fourth = r_u32(0x1f8000dc + offset);
        w_u32(matrix + offset, first);
        w_u32(matrix + offset + 4, second);
        w_u32(matrix + offset + 8, third);
        w_u32(matrix + offset + 0xc, fourth);
    }
}

uint32 G3Function8002FD3C(uint32 vector)
{
    FUNCTION_MARKER(0x8002FD3C, "SLUS_008.06");
    uint32 first = r_u32(vector);
    uint32 second = r_u32(vector + 4);
    if ((sint32)first < 0)
        first = 0u - first;
    if ((sint32)second < 0)
        second = 0u - second;
    if ((sint32)second < (sint32)first)
    {
        uint32 temporary = first;
        first = second;
        second = temporary;
    }
    uint32 weighted = second * 30 + first * 12;
    return (uint32)((sint32)weighted >> 5);
}

uint32 G3Function80031F5C(uint32 object)
{
    FUNCTION_MARKER(0x80031F5C, "SLUS_008.06");
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 index = (uint32)r_s16(object + 0xa2);
    uint32 models = r_u32(descriptor + 0xc);
    uint32 model = r_u32(models + (index << 2));
    if (r_s16(model + 6) == 1)
    {
        uint32 records = r_u32(model + 0x18);
        if (r_u32(records + 0x1c) == 1 && r_u32(records + 0x24) == 0 && r_u32(records + 0x14) == 0)
        {
            w_u32(object + 0xc8, r_u32(model + 0x28));
            uint32 result = r_u16(model + 0x2c);
            w_u16(object + 0xcc, result);
            return result;
        }
    }
    uint32 x = (uint32)r_s16(model + 0x28);
    uint32 y = (uint32)r_s16(model + 0x2a);
    uint32 z = (uint32)r_s16(model + 0x2c);
    if ((x | y | z) == 0 || (r_u32(object + 0x60) == 0 && r_s16(object + 0x64) == 0))
    {
        w_u16(object + 0xc8, x);
        w_u16(object + 0xca, y);
        w_u16(object + 0xcc, z);
        return 0;
    }
    uint32 matrix = r_u32(object + 0x30);
    uint32 control0 = r_u32(matrix);
    uint32 control1 = r_u32(matrix + 4);
    xport_gte_write_control(0, control0);
    xport_gte_write_control(1, control1);
    uint32 control2 = r_u32(matrix + 8);
    uint32 control3 = r_u32(matrix + 0xc);
    uint32 control4 = r_u32(matrix + 0x10);
    xport_gte_write_control(2, control2);
    xport_gte_write_control(3, control3);
    xport_gte_write_control(4, control4);
    w_u16(0x1f800010, x);
    w_u16(0x1f800012, y);
    w_u16(0x1f800014, z);
    xport_gte_write_data(0, r_u32(0x1f800010));
    xport_gte_write_data(1, r_u32(0x1f800014));
    xport_gte_execute(0x486012);
    uint32 extents[3] = {0, 0, 0};
    w_u32(0x1f800018, xport_gte_read_data(25));
    w_u32(0x1f80001c, xport_gte_read_data(26));
    w_u32(0x1f800020, xport_gte_read_data(27));
    for (uint32 corner = 0; corner < 3; ++corner)
    {
        if (corner == 1)
        {
            w_u16(0x1f800012, 0u - y);
            w_u16(0x1f800014, z);
        }
        else
            w_u16(0x1f800014, 0u - z);
        if (corner != 0)
        {
            w_u32(0x1f800018, xport_gte_read_data(25));
            w_u32(0x1f80001c, xport_gte_read_data(26));
            w_u32(0x1f800020, xport_gte_read_data(27));
        }
        xport_gte_write_data(0, r_u32(0x1f800010));
        xport_gte_write_data(1, r_u32(0x1f800014));
        xport_gte_execute(0x486012);
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 magnitude = r_u32(0x1f800018 + axis * 4);
            if ((sint32)magnitude < 0)
                magnitude = 0u - magnitude;
            if ((sint32)extents[axis] < (sint32)magnitude)
                extents[axis] = magnitude;
        }
    }
    w_u32(0x1f800018, xport_gte_read_data(25));
    w_u32(0x1f80001c, xport_gte_read_data(26));
    w_u32(0x1f800020, xport_gte_read_data(27));
    uint32 result = 0;
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 magnitude = r_u32(0x1f800018 + axis * 4);
        if ((sint32)magnitude < 0)
            magnitude = 0u - magnitude;
        result = (sint32)extents[axis] < (sint32)magnitude ? 1u : 0u;
        if (result != 0)
            extents[axis] = magnitude;
    }
    w_u16(object + 0xc8, extents[0]);
    w_u16(object + 0xca, extents[1]);
    w_u16(object + 0xcc, extents[2]);
    return result;
}

void G3Function800322CC(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800322CC, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 matrices = r_u32(game + 0x6558);
    uint32 list = r_u32(game + 0x5c);
    uint32 camera = r_u32(game + 0x4c);
    uint32 object = r_u32(list + 4);
    w_u16(0x1f800000, r_u16(camera));
    camera = r_u32(game + 0x4c);
    w_u16(0x1f800002, r_u16(camera + 2));
    camera = r_u32(game + 0x4c);
    w_u16(0x1f800004, r_u16(camera + 4));
    while (object != 0)
    {
        G3Function80031EAC(object);
        uint32 descriptor = r_u32(object + 0x1c);
        uint32 index = (uint32)r_s16(object + 0xa2);
        uint32 model_table = r_u32(descriptor + 0xc);
        uint32 model = r_u32(model_table + (index << 2));
        uint32 count = (uint32)r_s16(model + 6);
        uint32 limit = r_u32(game + 0x655c);
        uint32 required_end = matrices + (count << 5) + 0x20;
        if (limit < required_end)
            w_u32(object + 0x30, 0);
        else
        {
            bool billboard = (r_u32(descriptor) & 0x40) != 0 && count == 1;
            bool animated = false;
            bool recompute = true;
            uint32 rotation = matrices;
            if (billboard)
            {
                uint32 target_x = (uint32)r_s16(0x1f800000);
                uint32 origin_x = (uint32)r_s16(object + 0x58);
                uint32 target_y = (uint32)r_s16(0x1f800002);
                w_u32(0x1f800018, target_x - origin_x);
                uint32 origin_y = (uint32)r_s16(object + 0x5a);
                uint32 target_z = (uint32)r_s16(0x1f800004);
                w_u32(0x1f80001c, target_y - origin_y);
                uint32 origin_z = (uint32)r_s16(object + 0x5c);
                w_u32(0x1f800020, target_z - origin_z);
                uint32 length = G3Function8002FD3C(0x1f800018);
                uint32 dy = r_u32(0x1f80001c);
                uint32 dx = r_u32(0x1f800018);
                uint32 angle = G3Function8002F0C4(dy, dx, frame);
                uint32 dz = r_u32(0x1f800020);
                w_u16(0x1f800010, angle + 0x400);
                angle = G3Function8002F0C4(length, dz, frame);
                w_u16(0x1f800014, angle + 0xc00);
                w_u16(0x1f800012, 0);
                G3Function8005BD00(0x1f800010, 0x1f8000f0);
                w_u32(0x1f800104, (uint32)r_s16(object + 0x58));
                w_u32(0x1f800108, (uint32)r_s16(object + 0x5a));
                uint32 z = (uint32)r_s16(object + 0x5c);
                w_u16(0x1f800102, 0);
                w_u32(0x1f80010c, z);
                uint32 previous = r_u32(object + 0x30);
                w_u32(object + 0x30, matrices);
                w_u32(object + 0x34, previous);
                rotation = 0x1f8000f0;
            }
            else
            {
                animated = r_u32(descriptor + 0x10) != 0;
                recompute = (r_u32(object + 0x18) & 0x40) != 0;
                if (animated)
                {
                    if (recompute)
                        G3Function800328A0(object, descriptor, matrices, model, frame);
                    else
                        G3Function800330E4(object, matrices, model);
                }
                else if (recompute)
                    G3Function800315E8(object, matrices, model, frame);
                else
                    G3Function800317F0(object, matrices, model, frame);
            }
            if (recompute)
            {
                if (r_u32(model + 0x20) != 0 || r_s16(model + 0x24) != 0)
                {
                    uint32 control0 = r_u32(rotation);
                    uint32 control1 = r_u32(rotation + 4);
                    xport_gte_write_control(0, control0);
                    xport_gte_write_control(1, control1);
                    uint32 control2 = r_u32(rotation + 8);
                    uint32 control3 = r_u32(rotation + 0xc);
                    uint32 control4 = r_u32(rotation + 0x10);
                    xport_gte_write_control(2, control2);
                    xport_gte_write_control(3, control3);
                    xport_gte_write_control(4, control4);
                    xport_gte_write_data(0, r_u32(model + 0x20));
                    xport_gte_write_data(1, r_u32(model + 0x24));
                    xport_gte_execute(0x486012);
                    uint32 temporary = billboard ? 0x1f800018 : object + 0xbc;
                    w_u32(temporary, xport_gte_read_data(25));
                    w_u32(temporary + 4, xport_gte_read_data(26));
                    w_u32(temporary + 8, xport_gte_read_data(27));
                    for (uint32 axis = 0; axis < 3; ++axis)
                    {
                        uint32 offset = r_u32(temporary + axis * 4);
                        uint32 translation = r_u32(rotation + 0x14 + axis * 4);
                        w_u32(object + 0xbc + axis * 4, offset + translation);
                    }
                }
                else
                {
                    w_u32(object + 0xbc, r_u32(rotation + 0x14));
                    w_u32(object + 0xc0, r_u32(rotation + 0x18));
                    w_u32(object + 0xc4, r_u32(rotation + 0x1c));
                }
            }
            if (billboard)
            {
                for (uint32 group = 0; group < 2; ++group)
                {
                    uint32 offset = group * 0x10;
                    uint32 first = r_u32(0x1f8000f0 + offset);
                    uint32 second = r_u32(0x1f8000f4 + offset);
                    uint32 third = r_u32(0x1f8000f8 + offset);
                    uint32 fourth = r_u32(0x1f8000fc + offset);
                    w_u32(matrices + offset, first);
                    w_u32(matrices + offset + 4, second);
                    w_u32(matrices + offset + 8, third);
                    w_u32(matrices + offset + 0xc, fourth);
                }
                matrices += 0x20;
            }
            else
            {
                uint32 bytes = (uint32)r_s16(model + 6) << 5;
                if (animated)
                    bytes += 0x20;
                matrices += bytes;
            }
        }
        uint32 changed = r_u32(object + 0x18) & 0xc0;
        if (changed == 0x40)
        {
            if (object != r_u32(0x8009efc4))
            {
                descriptor = r_u32(object + 0x1c);
                index = (uint32)r_s16(object + 0xa2);
                model_table = r_u32(descriptor + 0xc);
                uint32 selected = r_u32(model_table + (index << 2));
                sint32 z = r_s16(selected + 0x2c);
                sint32 y = r_s16(selected + 0x2a);
                uint32 maximum = r_u16(selected + 0x2c);
                uint32 y_bits = r_u16(selected + 0x2a);
                if (z < y)
                    maximum = y_bits;
                sint32 signed_maximum = (sint32)(maximum << 16) >> 16;
                sint32 x = r_s16(selected + 0x28);
                uint32 x_bits = r_u16(selected + 0x28);
                if (signed_maximum < x)
                    maximum = x_bits;
                signed_maximum = (sint32)(maximum << 16) >> 16;
                uint32 expanded = (uint32)signed_maximum + (uint32)(signed_maximum >> 1);
                w_u16(object + 0xcc, expanded);
                w_u16(object + 0xca, expanded);
                w_u16(object + 0xc8, expanded);
            }
        }
        else if (changed == 0x80)
            G3Function80031F5C(object);
        object = r_u32(object + 8);
    }
    w_u32(game + 0x6558, matrices);
}
