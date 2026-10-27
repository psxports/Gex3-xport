#include "game_functions.h"
#include "psx.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

uint32 G3Function800199B0(uint32 packet, uint32 order_table, uint32 face_flags, uint32 material, uint32 blend, uint32 uv0, uint32 uv1, uint32 uv2)
{
    FUNCTION_MARKER(0x800199b0, "SLUS_008.06");
    uint32 depth, first, second, third, shift, limit, value, other;
    uint32 normal0, normal1, normal2, table, previous;
    uint32 tag, packet_bytes, color0, color1, color2, blend_offset;
    if ((xport_gte_read_control(31) & 0x60000) != 0)
        return packet;
    xport_gte_execute(0x1400006);
    if ((sint32)xport_gte_read_data(24) >= 0)
        return packet;
    if ((face_flags & 4) != 0)
    {
        xport_gte_execute(0x158002d);
        depth = xport_gte_read_data(7);
    }
    else
    {
        first = xport_gte_read_data(17);
        second = xport_gte_read_data(18);
        third = xport_gte_read_data(19);
        if ((sint32)first >= (sint32)second)
            second = first;
        shift = r_u32(0x1f8000bc);
        depth = (uint32)((sint32)third >> (shift & 31));
        if ((sint32)second >= (sint32)third)
            depth = (uint32)((sint32)second >> (shift & 31));
    }
    if ((face_flags & 2) != 0)
    {
        value = (uint32)r_s8(material + 0xb);
        depth += value << 1;
    }
    limit = r_u32(0x1f8000b8);
    depth <<= 2;
    if ((sint32)depth < 0 || (sint32)depth >= (sint32)limit)
        return packet;
    value = r_u32(0x1f8000a0);
    if (value != 0)
    {
        w_u32(packet + 4, 0x80808080);
        w_u32(packet + 8, xport_gte_read_data(12));
        w_u32(packet + 0x10, xport_gte_read_data(13));
        w_u32(packet + 0x18, xport_gte_read_data(14));
        value = r_u32(0x1f800094);
        other = r_u32(0x1f800054);
        w_u32(packet + 0xc, value | other);
        value = r_u32(0x1f800098);
        other = r_u32(0x1f800058);
        w_u32(packet + 0x14, value | other);
        value = r_u32(0x1f80009c);
        w_u32(packet + 0x1c, value);
        table = order_table + depth;
        previous = r_u32(table);
        w_u32(packet, previous);
        w_u8(packet + 3, 7);
        w_u32(table, packet & 0xffffff);
        value = r_u8(packet + 7);
        w_u8(packet + 7, value | blend);
        return packet + 0x20;
    }
    if ((face_flags & 1) != 0)
        goto gouraud;
    if ((face_flags & 2) != 0)
        goto textured_flat;
    w_u32(packet + 8, xport_gte_read_data(12));
    w_u32(packet + 0xc, xport_gte_read_data(13));
    w_u32(packet + 0x10, xport_gte_read_data(14));
    if ((face_flags & 8) != 0)
    {
        w_u32(packet + 4, material);
        value = r_u8(packet + 7);
        w_u8(packet + 7, value | blend);
        table = order_table + depth;
        previous = r_u32(table);
        w_u32(packet, previous);
        w_u8(packet + 3, 4);
        w_u32(table, packet & 0xffffff);
        return packet + 0x14;
    }
    normal0 = r_u32(0x1f800070);
    xport_gte_write_data(0, r_u32(normal0));
    xport_gte_write_data(1, r_u32(normal0 + 4));
    xport_gte_write_data(6, material);
    xport_gte_execute(0xe80413);
    tag = 4;
    packet_bytes = 0x14;
    goto link_flat_color;
textured_flat:
    w_u32(packet + 8, xport_gte_read_data(12));
    w_u32(packet + 0x10, xport_gte_read_data(13));
    w_u32(packet + 0x18, xport_gte_read_data(14));
    if ((face_flags & 8) != 0)
    {
        value = r_u32(material + 0xc);
        xport_gte_write_data(22, value);
    }
    else
    {
        normal0 = r_u32(0x1f800070);
        xport_gte_write_data(0, r_u32(normal0));
        xport_gte_write_data(1, r_u32(normal0 + 4));
        value = r_u32(material + 0xc);
        xport_gte_write_data(6, value);
        xport_gte_execute(0xe80413);
    }
    value = r_u32(0x1f8000a4);
    if (value != 0)
    {
        if ((face_flags & 0x20) != 0)
        {
            first = r_u32(uv0);
            second = r_u32(uv1);
            third = r_u32(uv2);
            w_u32(packet + 0xc, first);
            w_u32(packet + 0x14, second);
            w_u32(packet + 0x1c, third);
            table = order_table + depth;
            previous = r_u32(table);
            w_u32(packet, previous);
            w_u8(packet + 3, 7);
            w_u32(table, packet & 0xffffff);
            w_u32(packet + 4, xport_gte_read_data(22));
            value = r_u8(packet + 7);
            w_u8(packet + 7, value | blend);
            packet += 0x20;
            w_u32(packet + 8, xport_gte_read_data(12));
            w_u32(packet + 0x10, xport_gte_read_data(13));
            w_u32(packet + 0x18, xport_gte_read_data(14));
        }
        first = r_u32(uv0 + 0x30);
        value = r_u32(0x1f800054);
        first |= value;
        second = r_u32(uv1 + 0x30);
        value = r_u32(0x1f800058);
        second |= value;
        third = r_u32(uv2 + 0x30);
    }
    else
    {
        first = r_u32(uv0);
        second = r_u32(uv1);
        third = r_u32(uv2);
    }
    w_u32(packet + 0xc, first);
    w_u32(packet + 0x14, second);
    w_u32(packet + 0x1c, third);
    tag = 7;
    packet_bytes = 0x20;
link_flat_color:
    table = order_table + depth;
    previous = r_u32(table);
    w_u32(packet, previous);
    w_u8(packet + 3, tag);
    w_u32(table, packet & 0xffffff);
    w_u32(packet + 4, xport_gte_read_data(22));
    value = r_u8(packet + 7);
    w_u8(packet + 7, value | blend);
    return packet + packet_bytes;
gouraud:
    if ((face_flags & 2) != 0)
        goto textured_gouraud;
    if (blend != 0)
    {
        w_u32(packet + 4, 0xe1000268);
        w_u32(packet + 0xc, xport_gte_read_data(12));
        w_u32(packet + 0x14, xport_gte_read_data(13));
        w_u32(packet + 0x1c, xport_gte_read_data(14));
        tag = 7;
        packet_bytes = 0x20;
        color0 = 8;
        color1 = 0x10;
        color2 = 0x18;
        blend_offset = 0xb;
    }
    else
    {
        w_u32(packet + 8, xport_gte_read_data(12));
        w_u32(packet + 0x10, xport_gte_read_data(13));
        w_u32(packet + 0x18, xport_gte_read_data(14));
        tag = 6;
        packet_bytes = 0x1c;
        color0 = 4;
        color1 = 0xc;
        color2 = 0x14;
        blend_offset = 7;
    }
    normal0 = r_u32(0x1f800070);
    normal1 = r_u32(0x1f800074);
    normal2 = r_u32(0x1f800078);
    xport_gte_write_data(0, r_u32(normal0));
    xport_gte_write_data(1, r_u32(normal0 + 4));
    xport_gte_write_data(2, r_u32(normal1));
    xport_gte_write_data(3, r_u32(normal1 + 4));
    xport_gte_write_data(4, r_u32(normal2));
    xport_gte_write_data(5, r_u32(normal2 + 4));
    xport_gte_write_data(6, material);
    xport_gte_execute(0xf80416);
    table = order_table + depth;
    previous = r_u32(table);
    w_u32(packet, previous);
    w_u8(packet + 3, tag);
    w_u32(table, packet & 0xffffff);
    w_u32(packet + color0, xport_gte_read_data(20));
    w_u32(packet + color1, xport_gte_read_data(21));
    w_u32(packet + color2, xport_gte_read_data(22));
    value = r_u8(packet + blend_offset);
    w_u8(packet + blend_offset, value | blend);
    return packet + packet_bytes;
textured_gouraud:
    w_u32(packet + 8, xport_gte_read_data(12));
    w_u32(packet + 0x14, xport_gte_read_data(13));
    w_u32(packet + 0x20, xport_gte_read_data(14));
    normal0 = r_u32(0x1f800070);
    normal1 = r_u32(0x1f800074);
    normal2 = r_u32(0x1f800078);
    xport_gte_write_data(0, r_u32(normal0));
    xport_gte_write_data(1, r_u32(normal0 + 4));
    xport_gte_write_data(2, r_u32(normal1));
    xport_gte_write_data(3, r_u32(normal1 + 4));
    xport_gte_write_data(4, r_u32(normal2));
    xport_gte_write_data(5, r_u32(normal2 + 4));
    value = r_u32(material + 0xc);
    xport_gte_write_data(6, value);
    xport_gte_execute(0xf80416);
    value = r_u32(0x1f8000a4);
    if (value != 0)
    {
        if ((face_flags & 0x20) != 0)
        {
            first = r_u32(uv0);
            second = r_u32(uv1);
            third = r_u32(uv2);
            w_u32(packet + 0xc, first);
            w_u32(packet + 0x18, second);
            w_u32(packet + 0x24, third);
            w_u32(packet + 4, xport_gte_read_data(20));
            w_u32(packet + 0x10, xport_gte_read_data(21));
            w_u32(packet + 0x1c, xport_gte_read_data(22));
            value = r_u8(packet + 7);
            w_u8(packet + 7, value | blend);
            table = order_table + depth;
            previous = r_u32(table);
            w_u32(packet, previous);
            w_u8(packet + 3, 9);
            w_u32(table, packet & 0xffffff);
            packet += 0x28;
            w_u32(packet + 8, xport_gte_read_data(12));
            w_u32(packet + 0x14, xport_gte_read_data(13));
            w_u32(packet + 0x20, xport_gte_read_data(14));
        }
        first = r_u32(uv0 + 0x30);
        value = r_u32(0x1f800054);
        first = (first & 0xffff) | value;
        second = r_u32(uv1 + 0x30);
        value = r_u32(0x1f800058);
        second = (second & 0xffff) | value;
        third = r_u32(uv2 + 0x30);
    }
    else
    {
        first = r_u32(uv0);
        second = r_u32(uv1);
        third = r_u32(uv2);
    }
    w_u32(packet + 0xc, first);
    w_u32(packet + 0x18, second);
    w_u32(packet + 0x24, third);
    w_u32(packet + 4, xport_gte_read_data(20));
    w_u32(packet + 0x10, xport_gte_read_data(21));
    w_u32(packet + 0x1c, xport_gte_read_data(22));
    value = r_u8(packet + 7);
    w_u8(packet + 7, value | blend);
    table = order_table + depth;
    previous = r_u32(table);
    w_u32(packet, previous);
    w_u8(packet + 3, 9);
    w_u32(table, packet & 0xffffff);
    return packet + 0x28;
}

/* Native clipping helpers preserve the original ordered scalar accesses */
static uint32 G3ClipDivide(uint32 numerator, uint32 denominator)
{
    if (denominator == 0)
        xport_mips_break(7);
    if (denominator == 0xffffffff && numerator == 0x80000000)
        xport_mips_break(6);
    return (uint32)((sint64)(sint32)numerator / (sint32)denominator);
}

static void G3ClipPosition(uint32 first, uint32 second, uint32 destination)
{
    uint32 z = (uint32)r_s16(first + 4);
    uint32 other_z = (uint32)r_s16(second + 4);
    uint32 start = (uint32)r_s16(first);
    uint32 end = (uint32)r_s16(second);
    uint32 denominator = z - other_z;
    uint32 product = (end - start) * z;
    w_u16(destination, start + G3ClipDivide(product, denominator));
    start = (uint32)r_s16(first + 2);
    end = (uint32)r_s16(second + 2);
    product = (end - start) * z;
    w_u16(destination + 2, start + G3ClipDivide(product, denominator));
}

static void G3ClipUV(uint32 first, uint32 second, uint32 first_uv, uint32 second_uv, uint32 destination)
{
    uint32 z = (uint32)r_s16(first + 4);
    uint32 other_z = (uint32)r_s16(second + 4);
    uint32 denominator = z - other_z;
    uint32 start = r_u8(first_uv);
    uint32 end = r_u8(second_uv);
    uint32 product = (end - start) * z;
    w_u8(destination, start + G3ClipDivide(product, denominator));
    start = r_u8(first_uv + 1);
    end = r_u8(second_uv + 1);
    product = (end - start) * z;
    w_u8(destination + 1, start + G3ClipDivide(product, denominator));
}

static void G3ClipProject(uint32 first, uint32 second, uint32 second_z, uint32 third)
{
    xport_gte_write_data(0, r_u32(first));
    xport_gte_write_data(1, r_u32(first + 4));
    xport_gte_write_data(2, r_u32(second));
    xport_gte_write_data(3, r_u32(second_z));
    xport_gte_write_data(4, r_u32(third));
    xport_gte_write_data(5, r_u32(third + 4));
    xport_gte_execute(0x280030);
}

static void G3ClipNormals(uint32 face, uint32 vertices, uint32 flags, uint32 first, uint32 second, uint32 third, uint32 all_flat)
{
    if ((flags & 1) != 0)
    {
        uint32 index0 = r_u16(face);
        uint32 index1 = r_u16(face + 2);
        uint32 index2 = r_u16(face + 4);
        uint32 normal0 = r_u16(vertices + (index0 << 3) + 6);
        uint32 normal1 = r_u16(vertices + (index1 << 3) + 6);
        uint32 normal2 = r_u16(vertices + (index2 << 3) + 6);
        normal0 = 0x80098838 + (normal0 << 3);
        normal1 = 0x80098838 + (normal1 << 3);
        normal2 = 0x80098838 + (normal2 << 3);
        w_u32(0x1f800088, normal0);
        w_u32(0x1f80008c, normal1);
        w_u32(0x1f800090, normal2);
        w_u32(0x1f800070, first == 0 ? normal0 : first == 1 ? normal1 : normal2);
        w_u32(0x1f800074, second == 0 ? normal0 : second == 1 ? normal1 : normal2);
        w_u32(0x1f800078, third == 0 ? normal0 : third == 1 ? normal1 : normal2);
    }
    else
    {
        uint32 normal = 0x80098838 + (r_u8(face + 6) << 3);
        w_u32(0x1f800070, normal);
        if (all_flat != 0)
        {
            w_u32(0x1f800074, normal);
            w_u32(0x1f800078, normal);
        }
    }
}

uint32 G3Function80019FC8(uint32 model, uint32 instance, uint32 vertices, uint32 matrix, uint32 pool, uint32 order_table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80019fc8, "SLUS_008.06");
    uint32 face = r_u32(model + 0x14);
    uint32 count = (uint32)r_s16(model + 4);
    uint32 face_end = face + count * 12;
    uint32 packet_limit = r_u32(pool + 8);
    uint32 packet = r_u32(pool + 4);
    w_u32(0x1f800024, packet_limit);
    w_u32(0x1f800020, face_end);
    uint32 game = r_u32(0x80097048);
    uint32 value = r_u32(game + 0x2c);
    w_u32(0x1f8000bc, value);
    value = r_u32(game + 0x28);
    w_u32(0x1f8000b8, value << 2);
    value = r_u32(instance + 0x18);
    w_u32(0x1f8000a0, (value & 0x10) >> 4);
    uint32 background = r_u32(game + 4);
    w_u32(0x1f800000, background == 0 ? 0x8001a148 : 0x8001a328);
    w_u32(0x1f800004, 0x8001a42c);
    w_u32(0x1f800008, 0x8001aa60);
    w_u32(0x1f80000c, 0x8001b6f8);
    w_u32(0x1f800010, 0x8001b0ac);
    w_u32(0x1f800014, 0x8001bd2c);
    w_u32(0x1f800018, 0x8001c378);
    w_u32(0x1f80001c, 0x8001a330);
    w_u16(0x1f80002c, 0);
    w_u16(0x1f800034, 0);
    w_u16(0x1f80003c, 0);
    uint32 original_vertices = r_u32(model + 8);
    uint32 first = r_u32(matrix);
    uint32 second = r_u32(matrix + 4);
    uint32 third = r_u32(matrix + 8);
    uint32 fourth = r_u32(matrix + 0xc);
    xport_gte_write_control(0, first);
    xport_gte_write_control(1, second);
    xport_gte_write_control(2, third);
    xport_gte_write_control(3, fourth);
    first = r_u32(matrix + 0x10);
    second = r_u32(matrix + 0x14);
    third = r_u32(matrix + 0x18);
    fourth = r_u32(matrix + 0x1c);
    xport_gte_write_control(4, first);
    xport_gte_write_control(5, second);
    xport_gte_write_control(6, third);
    xport_gte_write_control(7, fourth);
    while (face < r_u32(0x1f800020))
    {
        if (packet >= r_u32(0x1f800024))
            break;
        uint32 flags = r_u8(face + 7);
        uint32 material = r_u32(face + 8);
        uint32 index0 = r_u16(face);
        uint32 index1 = r_u16(face + 2);
        uint32 index2 = r_u16(face + 4);
        uint32 vertex0 = vertices + (index0 << 3);
        uint32 vertex1 = vertices + (index1 << 3);
        uint32 vertex2 = vertices + (index2 << 3);
        sint32 z0 = r_s16(vertex0 + 4);
        sint32 z1 = r_s16(vertex1 + 4);
        sint32 z2 = r_s16(vertex2 + 4);
        uint32 mask = (z0 > 0 ? 1 : 0) | (z1 > 0 ? 2 : 0) | (z2 > 0 ? 4 : 0);
        value = r_u32(0x1f8000a0);
        value |= flags & 0x10;
        w_u32(0x1f8000a4, value);
        if (value != 0)
        {
            value &= r_u32(0x1f800060);
            w_u32(0x1f8000a4, value);
            if (value != 0)
            {
                w_u32(0x1f8000c4, mask);
                uint32 mapping = r_u32(0x1f80005c);
                uint32 u_offset = r_u32(0x1f800044);
                uint32 u_scale = r_u32(0x1f80004c);
                uint32 v_scale = r_u32(0x1f800050);
                uint32 v_offset = r_u32(0x1f800048);
                for (uint32 vertex_index = 0; vertex_index < 3; ++vertex_index)
                {
                    uint32 offset = (vertex_index == 0 ? vertex0 : vertex_index == 1 ? vertex1 : vertex2) - vertices;
                    uint32 address = mapping + offset + (offset >> 1);
                    uint32 u = r_u32(address);
                    uint32 v = r_u32(address + 4);
                    u *= u_scale;
                    u = ((uint32)((sint32)u >> 13) + u_offset) & 0xff;
                    v *= v_scale;
                    v = ((uint32)((sint32)v >> 13) + v_offset) & 0xff;
                    value = u | (v << 8);
                    w_u32(0x1f8000ac + vertex_index * 4, value);
                    w_u32(0x1f800094 + vertex_index * 4, value);
                }
                mask = r_u32(0x1f8000c4);
            }
        }
        uint32 target = r_u32(0x1f800000 + (mask << 2));
        uint32 clip_case, blend = 0;
        switch (target)
        {
        case 0x8001a148:
            face += 12;
            continue;
        case 0x8001a328:
            blend = 2;
            clip_case = 0;
            break;
        case 0x8001a330:
            clip_case = 0;
            break;
        case 0x8001a42c: clip_case = 1; break;
        case 0x8001aa60: clip_case = 2; break;
        case 0x8001b6f8: clip_case = 3; break;
        case 0x8001b0ac: clip_case = 4; break;
        case 0x8001bd2c: clip_case = 5; break;
        case 0x8001c378: clip_case = 6; break;
        default:
            /* A corrupted internal code target has no native entry */
            abort();
        }
        if (clip_case == 0)
        {
            G3ClipProject(vertex0, vertex1, vertex1 + 4, vertex2);
            if ((flags & 2) != 0)
            {
                first = r_u32(material);
                second = r_u32(material + 4);
                third = r_u32(material + 8);
                w_u32(0x1f80007c, first);
                w_u32(0x1f800080, second);
                w_u32(0x1f800084, third);
            }
            G3ClipNormals(face, original_vertices, flags, 0, 1, 2, 0);
            packet = G3Function800199B0(packet, order_table, flags, material, blend, 0x1f80007c, 0x1f800080, 0x1f800084);
            face += 12;
            continue;
        }
        uint32 edge0_first, edge0_second, edge0_position, edge0_uv, edge0_overlay;
        uint32 edge1_first, edge1_second, edge1_position, edge1_uv, edge1_overlay;
        uint32 uv0, uv1, uv2, normal0, normal1, normal2;
        switch (clip_case)
        {
        case 1:
            edge0_first = vertex0; edge0_second = vertex1; edge0_position = 0x1f800030;
            edge1_first = vertex0; edge1_second = vertex2; edge1_position = 0x1f800038;
            edge0_uv = 0x1f800068; edge1_uv = 0x1f80006c;
            edge0_overlay = 0x1f800098; edge1_overlay = 0x1f80009c;
            uv0 = 0x1f80007c; uv1 = edge0_uv; uv2 = edge1_uv;
            normal0 = 0; normal1 = 1; normal2 = 2;
            break;
        case 2:
            edge0_first = vertex1; edge0_second = vertex0; edge0_position = 0x1f800028;
            edge1_first = vertex1; edge1_second = vertex2; edge1_position = 0x1f800038;
            edge0_uv = 0x1f800064; edge1_uv = 0x1f80006c;
            edge0_overlay = 0x1f800094; edge1_overlay = 0x1f80009c;
            uv0 = 0x1f800080; uv1 = edge1_uv; uv2 = edge0_uv;
            normal0 = 1; normal1 = 2; normal2 = 0;
            break;
        case 3:
            edge0_first = vertex1; edge0_second = vertex2; edge0_position = 0x1f800030;
            edge1_first = vertex0; edge1_second = vertex2; edge1_position = 0x1f800028;
            edge0_uv = 0x1f800068; edge1_uv = 0x1f800064;
            edge0_overlay = 0x1f800098; edge1_overlay = 0x1f800094;
            uv0 = 0x1f80007c; uv1 = 0x1f800080; uv2 = edge0_uv;
            normal0 = 0; normal1 = 1; normal2 = 1;
            break;
        case 4:
            edge0_first = vertex2; edge0_second = vertex0; edge0_position = 0x1f800028;
            edge1_first = vertex2; edge1_second = vertex1; edge1_position = 0x1f800030;
            edge0_uv = 0x1f800064; edge1_uv = 0x1f800068;
            edge0_overlay = 0x1f800094; edge1_overlay = 0x1f800098;
            uv0 = 0x1f800084; uv1 = edge0_uv; uv2 = edge1_uv;
            normal0 = 2; normal1 = 0; normal2 = 1;
            break;
        case 5:
            edge0_first = vertex0; edge0_second = vertex1; edge0_position = 0x1f800028;
            edge1_first = vertex2; edge1_second = vertex1; edge1_position = 0x1f800038;
            edge0_uv = 0x1f800064; edge1_uv = 0x1f80006c;
            edge0_overlay = 0x1f800094; edge1_overlay = 0x1f80009c;
            uv0 = 0x1f800084; uv1 = 0x1f80007c; uv2 = edge0_uv;
            normal0 = 2; normal1 = 0; normal2 = 0;
            break;
        default:
            edge0_first = vertex2; edge0_second = vertex0; edge0_position = 0x1f800038;
            edge1_first = vertex1; edge1_second = vertex0; edge1_position = 0x1f800030;
            edge0_uv = 0x1f80006c; edge1_uv = 0x1f800068;
            edge0_overlay = 0x1f80009c; edge1_overlay = 0x1f800098;
            uv0 = 0x1f800080; uv1 = 0x1f800084; uv2 = edge0_uv;
            normal0 = 1; normal1 = 2; normal2 = 2;
            break;
        }
        G3ClipPosition(edge0_first, edge0_second, edge0_position);
        G3ClipPosition(edge1_first, edge1_second, edge1_position);
        if (clip_case == 1)
            G3ClipProject(vertex0, 0x1f800030, 0x1f800034, 0x1f800038);
        else if (clip_case == 2)
            G3ClipProject(vertex1, 0x1f800038, 0x1f80003c, 0x1f800028);
        else if (clip_case == 3)
            G3ClipProject(vertex0, vertex1, vertex1 + 4, 0x1f800030);
        else if (clip_case == 4)
            G3ClipProject(vertex2, 0x1f800028, 0x1f80002c, 0x1f800030);
        else if (clip_case == 5)
            G3ClipProject(vertex2, vertex0, vertex0 + 4, 0x1f800028);
        else
            G3ClipProject(vertex1, vertex2, vertex2 + 4, 0x1f800038);
        if ((flags & 2) != 0)
        {
            first = r_u32(material);
            second = r_u32(material + 4);
            third = r_u32(material + 8);
            w_u32(0x1f80007c, first);
            w_u32(0x1f800080, second);
            if (clip_case == 1)
                w_u16(0x1f80006a, second >> 16);
            w_u32(0x1f800084, third);
            if (clip_case == 2)
            {
                w_u16(0x1f800082, first >> 16);
                w_u16(0x1f80006e, second >> 16);
            }
            else if (clip_case == 4)
            {
                w_u16(0x1f800086, first >> 16);
                w_u16(0x1f800066, second >> 16);
            }
            else if (clip_case == 5)
            {
                w_u16(0x1f800086, first >> 16);
                w_u16(0x1f80007e, second >> 16);
            }
            else if (clip_case == 6)
            {
                w_u16(0x1f800082, first >> 16);
                w_u16(0x1f800086, second >> 16);
            }
            uint32 first_uv0, second_uv0, first_uv1, second_uv1;
            switch (clip_case)
            {
            case 1: first_uv0 = 0x7c; second_uv0 = 0x80; first_uv1 = 0x7c; second_uv1 = 0x84; break;
            case 2: first_uv0 = 0x80; second_uv0 = 0x7c; first_uv1 = 0x80; second_uv1 = 0x84; break;
            case 3: first_uv0 = 0x80; second_uv0 = 0x84; first_uv1 = 0x7c; second_uv1 = 0x84; break;
            case 4: first_uv0 = 0x84; second_uv0 = 0x7c; first_uv1 = 0x84; second_uv1 = 0x80; break;
            case 5: first_uv0 = 0x7c; second_uv0 = 0x80; first_uv1 = 0x84; second_uv1 = 0x80; break;
            default: first_uv0 = 0x84; second_uv0 = 0x7c; first_uv1 = 0x80; second_uv1 = 0x7c; break;
            }
            G3ClipUV(edge0_first, edge0_second, 0x1f800000 + first_uv0, 0x1f800000 + second_uv0, edge0_uv);
            G3ClipUV(edge1_first, edge1_second, 0x1f800000 + first_uv1, 0x1f800000 + second_uv1, edge1_uv);
            if (r_u32(0x1f8000a4) != 0)
            {
                G3ClipUV(edge0_first, edge0_second, 0x1f800030 + first_uv0, 0x1f800030 + second_uv0, edge0_overlay);
                G3ClipUV(edge1_first, edge1_second, 0x1f800030 + first_uv1, 0x1f800030 + second_uv1, edge1_overlay);
            }
        }
        G3ClipNormals(face, original_vertices, flags, normal0, normal1, normal2, 1);
        packet = G3Function800199B0(packet, order_table, flags, material, 0, uv0, uv1, uv2);
        uint32 one_front = clip_case == 1 || clip_case == 2 || clip_case == 4;
        if (one_front != 0 && background == 0)
        {
            face += 12;
            continue;
        }
        blend = one_front != 0 ? 2 : 0;
        switch (clip_case)
        {
        case 1:
            G3ClipProject(0x1f800030, vertex1, vertex1 + 4, 0x1f800038);
            first = r_u16(0x1f80007e);
            third = r_u32(0x1f800074);
            uv0 = 0x1f800068; uv1 = 0x1f800080; uv2 = 0x1f80006c;
            w_u16(uv0 + 2, first);
            w_u32(0x1f800070, third);
            break;
        case 2:
            G3ClipProject(0x1f800038, vertex2, vertex2 + 4, 0x1f800028);
            first = r_u16(0x1f800082);
            second = r_u16(0x1f80006e);
            third = r_u32(0x1f800074);
            uv0 = 0x1f80006c; uv1 = 0x1f800084; uv2 = 0x1f800064;
            w_u16(uv0 + 2, first);
            w_u16(uv1 + 2, second);
            w_u32(0x1f800070, third);
            break;
        case 3:
            G3ClipProject(vertex0, 0x1f800030, 0x1f800034, 0x1f800028);
            first = r_u16(0x1f800082);
            third = r_u32(0x1f800070);
            uv0 = 0x1f80007c; uv1 = 0x1f800068; uv2 = 0x1f800064;
            w_u16(uv1 + 2, first);
            w_u32(0x1f800078, third);
            break;
        case 4:
            G3ClipProject(0x1f800028, vertex0, vertex0 + 4, 0x1f800030);
            first = r_u16(0x1f800086);
            second = r_u16(0x1f800066);
            third = r_u32(0x1f800074);
            uv0 = 0x1f800064; uv1 = 0x1f80007c; uv2 = 0x1f800068;
            w_u16(uv0 + 2, first);
            w_u16(uv1 + 2, second);
            w_u32(0x1f800070, third);
            break;
        case 5:
            G3ClipProject(vertex2, 0x1f800028, 0x1f80002c, 0x1f800038);
            first = r_u16(0x1f800086);
            second = r_u16(0x1f80007e);
            third = r_u32(0x1f800070);
            uv0 = 0x1f800084; uv1 = 0x1f800064; uv2 = 0x1f80006c;
            w_u16(uv0 + 2, first);
            w_u16(uv1 + 2, second);
            w_u32(0x1f800078, third);
            break;
        default:
            G3ClipProject(vertex1, 0x1f800038, 0x1f800034, 0x1f800030);
            first = r_u16(0x1f800082);
            second = r_u16(0x1f800086);
            third = r_u32(0x1f800070);
            uv0 = 0x1f800080; uv1 = 0x1f80006c; uv2 = 0x1f800068;
            w_u16(uv0 + 2, first);
            w_u16(uv1 + 2, second);
            w_u32(0x1f800078, third);
            break;
        }
        packet = G3Function800199B0(packet, order_table, flags, material, blend, uv0, uv1, uv2);
        if (one_front == 0 && background == 0)
        {
            face += 12;
            continue;
        }
        switch (clip_case)
        {
        case 1:
            G3ClipProject(0x1f800038, vertex1, vertex1 + 4, vertex2);
            first = r_u16(0x1f80007e);
            third = r_u32(0x1f800078);
            uv0 = 0x1f80006c; uv1 = 0x1f800080; uv2 = 0x1f800084;
            w_u16(uv0 + 2, first);
            w_u32(0x1f800070, third);
            break;
        case 2:
            G3ClipProject(0x1f800028, vertex2, vertex2 + 4, vertex0);
            first = r_u16(0x1f80006e);
            second = r_u16(0x1f800086);
            third = r_u32(0x1f800078);
            uv0 = 0x1f800064; uv1 = 0x1f800084; uv2 = 0x1f80007c;
            w_u16(uv0 + 2, first);
            w_u16(uv1 + 2, second);
            w_u32(0x1f800070, third);
            break;
        case 3:
            G3ClipProject(0x1f800028, 0x1f800030, 0x1f800034, vertex2);
            first = r_u16(0x1f80007e);
            second = r_u16(0x1f800082);
            third = r_u32(0x1f800090);
            uv0 = 0x1f800064; uv1 = 0x1f800068; uv2 = 0x1f800084;
            w_u16(uv0 + 2, first);
            w_u16(uv1 + 2, second);
            w_u32(0x1f800078, third);
            break;
        case 4:
            G3ClipProject(0x1f800030, vertex0, vertex0 + 4, vertex1);
            first = r_u16(0x1f800066);
            second = r_u16(0x1f80007e);
            third = r_u32(0x1f800078);
            uv0 = 0x1f800068; uv1 = 0x1f80007c; uv2 = 0x1f800080;
            w_u16(uv0 + 2, first);
            w_u16(uv1 + 2, second);
            w_u32(0x1f800070, third);
            break;
        case 5:
            G3ClipProject(0x1f800038, 0x1f800028, 0x1f80002c, vertex1);
            first = r_u16(0x1f800086);
            second = r_u16(0x1f800066);
            third = r_u32(0x1f80008c);
            uv0 = 0x1f80006c; uv1 = 0x1f800064; uv2 = 0x1f800080;
            w_u16(uv0 + 2, first);
            w_u16(uv1 + 2, second);
            w_u32(0x1f800078, third);
            break;
        default:
            G3ClipProject(0x1f800030, 0x1f800038, 0x1f80003c, vertex0);
            first = r_u16(0x1f800082);
            second = r_u16(0x1f80006e);
            third = r_u32(0x1f800088);
            uv0 = 0x1f800068; uv1 = 0x1f80006c; uv2 = 0x1f80007c;
            w_u16(uv0 + 2, first);
            w_u16(uv1 + 2, second);
            w_u32(0x1f800078, third);
            break;
        }
        packet = G3Function800199B0(packet, order_table, flags, material, 2, uv0, uv1, uv2);
        face += 12;
    }
    return packet;
}

uint32 G3Function80085DFC(uint32 cursor_pointer, uint32 kind_pointer, uint32 data_pointer, uint32 size_pointer)
{
    FUNCTION_MARKER(0x80085dfc, "SLUS_008.06");
    uint32 header = r_u32(cursor_pointer);
    uint32 sentinel = r_u32(header);
    uint32 payload = header + 4;
    if (sentinel == 0xffffffff)
    {
        w_u32(cursor_pointer, payload);
        return 0;
    }
    uint32 size = r_u16(header);
    if (size_pointer != 0)
        w_u16(size_pointer, size);
    uint32 units = (uint32)((sint32)((size + 3) << 16) >> 18);
    uint32 kind = r_u16(header + 2);
    if (kind_pointer != 0)
        w_u16(kind_pointer, kind);
    if ((kind << 16) == 0)
        units = 0;
    if (data_pointer != 0)
        w_u32(data_pointer, payload);
    uint32 bytes = (uint32)((sint32)(units << 16) >> 14);
    w_u32(cursor_pointer, payload + bytes);
    return 1;
}

uint32 G3Function80085D40(uint32 cursor_pointer, uint32 kind, uint32 data_pointer, uint32 size_pointer, uint32 stop_kind, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80085d40, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    sint32 stop = (sint16)(stop_kind & 0xffff);
    sint32 wanted = (sint16)(kind & 0xffff);
    for (;;)
    {
        uint32 found = G3Function80085DFC(cursor_pointer, frame + 0x10, frame + 0x14, frame + 0x18);
        if (found == 0)
            return 0;
        sint32 current = r_s16(frame + 0x10);
        if (current == stop)
            return 0;
        if (current != wanted)
            continue;
        if (data_pointer != 0)
        {
            uint32 payload = r_u32(frame + 0x14);
            w_u32(data_pointer, payload);
        }
        if (size_pointer != 0)
        {
            uint32 size = r_u16(frame + 0x18);
            w_u16(size_pointer, size);
        }
        return 1;
    }
}

/* Native SDK adapter retains the original GP1 command-class return */
uint32 G3SetDisplayMask80086314(uint32 enabled)
{
    SetDispMask((sint32)enabled);
    return 3;
}

/* Map the complete guest packet at the native SDK boundary */
void G3DrawPrimitive80086784(uint32 primitive)
{
    uint32 words = r_u8(primitive + 3);
    DrawPrim(psx_addr(primitive, (words + 1) * 4));
}

/* Preserve the guest environment identity across the native pointer ABI */
uint32 G3PutDrawEnv80086850(uint32 environment)
{
    PutDrawEnv((DRAWENV *)psx_addr(environment, 92));
    return environment;
}

/* Keep the original display object at the native SDK boundary */
uint32 G3PutDispEnv80086910(uint32 environment)
{
    PutDispEnv((DISPENV *)psx_addr(environment, 20));
    return environment;
}

/* The shared renderer follows the guest links from this first tag */
void G3Function800867E0(uint32 table)
{
    DrawOTag((uint32 *)psx_addr(table, 4));
}

/* Adapt the proven game callback to the native interrupt signature */
void G3DrawCompleteCallback(void)
{
    G3OnDrawComplete8002DDA8();
}

/* Keep the original sentinel through the existing native SDK adapter */
uint32 G3Function800866D8(uint32 table, uint32 count)
{
    if (count > PSX_DRAM_SIZE / 4)
        abort();
    uint32 span = count == 0 ? 4 : count * 4;
    ClearOTagRNativePSX((uint32 *)psx_addr(table, span), (sint32)count, 0x8009a3ec, 0x8009a3d8);
    return table;
}

/* Narrow the target at the existing SDK scalar boundary */
uint32 G3SetRootCounter80089538(uint32 counter, uint32 target, uint32 mode)
{
    return (uint32)SetRCnt(counter, (uint16)target, (sint32)mode);
}

uint32 G3ReadRootCounter800895D4(uint32 counter)
{
    return (uint32)GetRCnt(counter) & 0xffff;
}

uint32 G3StartRootCounter8008960C(uint32 counter)
{
    return (uint32)StartRCnt(counter);
}

uint32 G3StopRootCounter8008963C(uint32 counter)
{
    return (uint32)psx_root_counter_stop(counter);
}

/* Preserve the existing SDK service result at the scalar boundary */
uint32 G3EnterCritical8008A05C(void)
{
    return (uint32)EnterCriticalSectionPSX();
}

/* The original callers do not consume a critical-exit result */
void G3ExitCritical8008A03C(void)
{
    ExitCriticalSection();
}

/* Adapt the proven VBlank handler to the native callback signature */
void G3VBlankCallback(void)
{
    G3OnVSync8002DD34();
    G3OnAudioField();
}

/* Let the public API reject names longer than its twenty-byte limit */
static void G3CopyCardFilename(uint32 source, char *destination)
{
    for (uint32 index = 0; index < 21; ++index)
    {
        uint32 value = r_u8(source + index);
        destination[index] = (char)value;
        if (value == 0)
            return;
    }
    destination[21] = 0;
}

uint32 G3Function8008DCC0(uint32 channel, uint32 name, uint32 buffer, uint32 offset, uint32 bytes)
{
    char filename[22];
    G3CopyCardFilename(name, filename);
    return (uint32)MemCardReadFile((sint32)channel, filename, (uint32 *)psx_addr(buffer, bytes), (sint32)offset, (sint32)bytes);
}

uint32 G3Function8008DEE0(uint32 channel, uint32 name, uint32 buffer, uint32 offset, uint32 bytes)
{
    char filename[22];
    G3CopyCardFilename(name, filename);
    return (uint32)MemCardWriteFile((sint32)channel, filename, (uint32 *)psx_addr(buffer, bytes), (sint32)offset, (sint32)bytes);
}

uint32 G3Function8008E490(uint32 channel, uint32 name, uint32 blocks)
{
    char filename[22];
    G3CopyCardFilename(name, filename);
    return (uint32)MemCardCreateFile((sint32)channel, filename, (sint32)(blocks & 0xffff));
}

uint32 G3Function8008E698(uint32 channel, uint32 name)
{
    char filename[22];
    G3CopyCardFilename(name, filename);
    return (uint32)MemCardDeleteFile((sint32)channel, filename);
}

uint32 G3Function8008C3A4(uint32 mode)
{
    SpuReverbAttr attribute = {0};
    attribute.mask = SPU_REV_MODE;
    attribute.mode = (sint32)mode;
    return (uint32)SpuSetReverbModeParam(&attribute);
}

void G3SetReverbDepth8008C098(uint32 left, uint32 right)
{
    SpuReverbAttr attribute = {0};
    attribute.mask = SPU_REV_DEPTHL | SPU_REV_DEPTHR;
    attribute.depth.left = (sint16)(left & 0xffff);
    attribute.depth.right = (sint16)(right & 0xffff);
    SpuSetReverbDepth(&attribute);
}

uint32 G3Function8008BCA8(uint32 matrix, uint32 translation)
{
    VECTOR vector = {0};
    vector.vx = r_s32(translation);
    vector.vy = r_s32(translation + 4);
    vector.vz = r_s32(translation + 8);
    TransMatrix((MATRIX *)psx_addr(matrix, 32), &vector);
    return matrix;
}

uint32 G3Function8008C6D4(uint32 voice, uint32 attack, uint32 decay, uint32 sustain, uint32 release, uint32 level, uint32 attack_mode, uint32 sustain_mode, uint32 release_mode)
{
    SpuVoiceAttr attribute = {0};
    attribute.voice = voice < 32 ? (1u << voice) : 0;
    attribute.mask = SPU_VOICE_ADSR_AR | SPU_VOICE_ADSR_DR | SPU_VOICE_ADSR_SR | SPU_VOICE_ADSR_RR | SPU_VOICE_ADSR_SL | SPU_VOICE_ADSR_AMODE | SPU_VOICE_ADSR_SMODE | SPU_VOICE_ADSR_RMODE;
    attribute.ar = (uint16)(attack & 0x7f);
    attribute.dr = (uint16)(decay & 0xf);
    attribute.sr = (uint16)(sustain & 0x7f);
    attribute.rr = (uint16)(release & 0x1f);
    attribute.sl = (uint16)(level & 0xf);
    attribute.a_mode = (sint32)attack_mode;
    attribute.s_mode = (sint32)sustain_mode;
    attribute.r_mode = (sint32)release_mode;
    SpuSetVoiceAttr(&attribute);
    /* The original delay loop exits with a zero comparison result */
    return 0;
}

void G3Function8008C1E4(uint32 left, uint32 right)
{
    SpuCommonAttr attribute = {0};
    attribute.mask = SPU_COMMON_MVOLL | SPU_COMMON_MVOLR;
    attribute.mvol.left = (sint16)(left & 0x7fff);
    attribute.mvol.right = (sint16)(right & 0x7fff);
    SpuSetCommonAttr(&attribute);
}

/* Validate the native string span inside the existing xport memory */
static const char *G3MapGuestCStringForCrt(uint32 text)
{
    const char *begin = (const char *)psx_addr(text, 1);
    uintptr_t address = (uintptr_t)begin;
    uintptr_t dram = (uintptr_t)DRAM;
    uintptr_t scratchpad = (uintptr_t)SCRATCHPAD;
    size_t capacity;
    if (address >= dram && address - dram < sizeof(DRAM))
        capacity = sizeof(DRAM) - (size_t)(address - dram);
    else if (address >= scratchpad && address - scratchpad < sizeof(SCRATCHPAD))
        capacity = sizeof(SCRATCHPAD) - (size_t)(address - scratchpad);
    else
        abort();
    begin = (const char *)psx_addr(text, capacity);
    const char *end = (const char *)memchr(begin, 0, capacity);
    if (end == 0)
        abort();
    return (const char *)psx_addr(text, (size_t)(end - begin) + 1);
}

uint32 G3CopyString8008935C(uint32 destination, uint32 source)
{
    if (destination == 0 || source == 0)
        return 0;
    const char *input = G3MapGuestCStringForCrt(source);
    size_t bytes = strlen(input) + 1;
    strcpy((char *)psx_addr(destination, bytes), input);
    return destination;
}

uint32 G3Function8008EAC8(uint32 destination, uint32 source, sint32 count)
{
    if (destination == 0 || source == 0)
        return 0;
    if (count <= 0)
        return destination;
    strncpy((char *)psx_addr(destination, (uint32)count), (const char *)psx_addr(source, (uint32)count), (uint32)count);
    return destination;
}

uint32 G3StringLength8008929C(uint32 text)
{
    if (text == 0)
        return 0;
    return (uint32)strlen(G3MapGuestCStringForCrt(text));
}

uint32 G3CompareString80085EC4(uint32 first, uint32 second)
{
    if (first == 0 || second == 0)
    {
        if (first == second)
            return 0;
        return first == 0 ? 0xffffffff : 1;
    }
    const char *left = G3MapGuestCStringForCrt(first);
    const char *right = G3MapGuestCStringForCrt(second);
    return (uint32)strcmp(left, right);
}

uint32 G3SdkConcat8008BFE8(uint32 destination, uint32 source)
{
    if (destination == 0 || source == 0)
        return 0;
    const char *prefix = G3MapGuestCStringForCrt(destination);
    size_t prefix_bytes = strlen(prefix);
    const char *suffix = G3MapGuestCStringForCrt(source);
    size_t suffix_bytes = strlen(suffix);
    if (destination + (uint32)prefix_bytes == source + (uint32)suffix_bytes)
        return 0;
    char *output = (char *)psx_addr(destination, prefix_bytes + suffix_bytes + 1);
    strcat(output, suffix);
    return destination;
}

uint32 G3SdkSprintf80088A1C(uint32 destination, uint32 format, uint32 value)
{
    return sprintfPSX(destination, format, value);
}

uint32 G3SdkSprintfNoValues80088A1C(uint32 destination, uint32 format)
{
    const char *pattern = G3MapGuestCStringForCrt(format);
    if (strchr(pattern, '%') != 0)
        abort();
    size_t bytes = strlen(pattern);
    char *output = (char *)psx_addr(destination, bytes + 1);
    return (uint32)snprintf(output, bytes + 1, pattern);
}

uint32 G3SdkSprintfTwoIntegers80088A1C(uint32 destination, uint32 format, uint32 first, uint32 second, uint32 *last_field_address)
{
    const char *pattern = G3MapGuestCStringForCrt(format);
    const char *conversion = pattern;
    const char *last_conversion = 0;
    uint32 conversions = 0;
    while ((conversion = strchr(conversion, '%')) != 0)
    {
        if (conversion[1] != 'd' || ++conversions > 2)
            abort();
        last_conversion = conversion;
        conversion += 2;
    }
    if (conversions != 2)
        abort();
    char first_digits[12], second_digits[12];
    int first_count = snprintf(first_digits, sizeof(first_digits), "%d", (int)(sint32)first);
    int second_count = snprintf(second_digits, sizeof(second_digits), "%d", (int)(sint32)second);
    if (first_count < 0 || second_count < 0)
        abort();
    size_t bytes = strlen(pattern) - 4 + (size_t)first_count + (size_t)second_count;
    char *output = (char *)psx_addr(destination, bytes + 1);
    uint32 result = (uint32)snprintf(output, bytes + 1, pattern, (int)(sint32)first, (int)(sint32)second);
    if (last_field_address != 0)
        *last_field_address = destination + (uint32)(last_conversion - pattern - 2 + first_count);
    return result;
}

uint32 G3SdkSprintfStringInteger80088A1C(uint32 destination, uint32 format, uint32 string, uint32 value)
{
    const char *pattern = G3MapGuestCStringForCrt(format);
    const char *text = G3MapGuestCStringForCrt(string);
    const char *string_conversion = strchr(pattern, '%');
    if (string_conversion == 0 || string_conversion[1] != 's')
        abort();
    const char *integer_conversion = strchr(string_conversion + 2, '%');
    if (integer_conversion == 0)
        abort();
    bool long_decimal = integer_conversion[1] == 'l' && integer_conversion[2] == 'd';
    size_t integer_marker_bytes = long_decimal ? 3 : 2;
    if ((!long_decimal && integer_conversion[1] != 'd') || strchr(integer_conversion + integer_marker_bytes, '%') != 0)
        abort();
    char digits[12];
    int digit_count = long_decimal ? snprintf(digits, sizeof(digits), "%ld", (long)(sint32)value) : snprintf(digits, sizeof(digits), "%d", (int)(sint32)value);
    if (digit_count < 0)
        abort();
    size_t bytes = strlen(pattern) - 2 - integer_marker_bytes + strlen(text) + (size_t)digit_count;
    char *output = (char *)psx_addr(destination, bytes + 1);
    if (long_decimal)
        return (uint32)snprintf(output, bytes + 1, pattern, text, (long)(sint32)value);
    return (uint32)snprintf(output, bytes + 1, pattern, text, (int)(sint32)value);
}

uint32 G3SdkSprintfFourStrings80088A1C(uint32 destination, uint32 format, uint32 first, uint32 second, uint32 third, uint32 fourth)
{
    const char *pattern = G3MapGuestCStringForCrt(format);
    const char *conversion = pattern;
    uint32 conversions = 0;
    while ((conversion = strchr(conversion, '%')) != 0)
    {
        if (conversion[1] != 's' || ++conversions > 4)
            abort();
        conversion += 2;
    }
    if (conversions != 4)
        abort();
    const char *first_text = G3MapGuestCStringForCrt(first);
    const char *second_text = G3MapGuestCStringForCrt(second);
    const char *third_text = G3MapGuestCStringForCrt(third);
    const char *fourth_text = G3MapGuestCStringForCrt(fourth);
    size_t bytes = strlen(pattern) - 8 + strlen(first_text) + strlen(second_text) + strlen(third_text) + strlen(fourth_text);
    char *output = (char *)psx_addr(destination, bytes + 1);
    return (uint32)snprintf(output, bytes + 1, pattern, first_text, second_text, third_text, fourth_text);
}

uint32 G3Function8003A150(uint32 minutes, uint32 seconds, uint32 ticks, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003a150, "SLUS_008.06");
    uint32 previous_ticks = r_u32(0x8009db58);
    uint32 frame = guest_stack - 0x78;
    if (ticks != previous_ticks)
    {
        uint32 game = r_u32(0x80097048);
        uint32 flags = r_u32(game + 0x6834);
        w_u32(0x8009db58, ticks);
        w_u32(game + 0x6834, flags | 0x1000000);
    }
    if ((sint32)seconds >= 60)
    {
        uint32 whole_minutes = (uint32)((sint32)seconds / 60);
        minutes += whole_minutes;
        seconds -= whole_minutes * 60;
    }
    uint32 game = r_u32(0x80097048);
    uint32 row = r_u32(game + 0x69ec);
    uint32 position = 0xe6 - row * 40;
    w_u32(game + 0x69ec, row + 1);
    G3Function80021A74(400);
    uint32 y = (uint32)(sint32)(sint16)(position & 0xffff);
    G3Function80022E90(0xfffffffe, y);
    uint32 last_field_address;
    G3SdkSprintfTwoIntegers80088A1C(frame + 0x10, 0x8009db5c, minutes, seconds + 100, &last_field_address);
    sint32 phase = (sint32)ticks % 30;
    w_u8(frame + 0x11, phase < 15 ? 0x2e : 0x20);
    G3TextMeasureOperands operands;
    /* Preserve the proved terminator, exhausted count and final field */
    G3Function80020CF4(frame + 0x10, 0, 0xffffffff, last_field_address, frame, &operands);
    if (phase < 15)
    {
        uint32 dot_y = (uint32)(sint32)(sint16)((position - 15) & 0xffff);
        G3Function80022E90(40, dot_y);
        G3Function80020CF4(0x8009db64, dot_y, operands.second_glyph, operands.third_glyph, frame, 0);
    }
    return G3Function80021A74(320);
}

uint32 G3Function8006D4D0(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006d4d0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    uint32 flags = r_u32(object + 0x10);
    uint32 level = r_u16(game + 0x68b4);
    w_u32(object + 0x10, flags | 0x800);
    G3TextMeasureOperands operands;
    G3Function8006D020(game, frame, &operands);
    if (level - 0xb >= 5 && level - 0x14 >= 3)
    {
        G3Function80022E30(0xd8, 0x8f);
        uint32 current_game = r_u32(0x80097048);
        uint32 count = r_u32(current_game + 0x6820);
        G3Function80020CBC(0x800a01fc, count, operands.second_glyph, operands.third_glyph, frame, &operands);
        G3Function80022E30(0xf6, 0x8f);
        G3Function80020CBC(0x800a0200, 0x8f, operands.second_glyph, operands.third_glyph, frame, &operands);
        w_u32(frame + 0x10, 0xffffffff);
        w_u32(frame + 0x28, 0x1000);
        w_u32(frame + 0x2c, 0x1000);
        w_u32(frame + 0x20, 0x79);
        w_u32(frame + 0x1c, 0x100);
        w_u16(frame + 0x24, 1);
        uint32 resource = r_u32(0x80098764);
        w_u32(frame + 0x14, 0);
        w_u16(frame + 0x26, 0);
        uint32 animation = r_u16(0x800a2a72);
        w_u16(0x800a2a72, animation + 1);
        w_u32(frame + 0x18, (uint32)(sint32)(sint16)animation);
        G3Function800214EC(frame + 0x10, resource, 0, 0);
        w_u16(frame + 0x24, 0);
        w_u32(frame + 0x14, 1);
        G3Function800214EC(frame + 0x10, resource, &operands.second_glyph, &operands.third_glyph);
        G3Function80022E30(0x139, 0xbe);
        current_game = r_u32(0x80097048);
        count = r_u32(current_game + 0x6824);
        G3Function80020CBC(0x800a01fc, count, operands.second_glyph, operands.third_glyph, frame, &operands);
        G3Function80022E30(0xbf, 0xbe);
        current_game = r_u32(0x80097048);
        count = r_u32(current_game + 0x6828);
        G3Function80020CBC(0x800a01fc, count, operands.second_glyph, operands.third_glyph, frame, 0);
    }
    G3Function8006D3F8();
    G3Function8006D188(game, frame);
    G3Function8006D4C8();
    uint32 text = G3Function80065D38(0xb2);
    return G3Function80023448(text, 0xd4, 0, 0, frame, 0);
}

uint32 G3ArchiveSector80064D8C(uint32 filename)
{
    FUNCTION_MARKER(0x80064d8c, "SLUS_008.06");
    uint32 index = G3FindArchiveFile800654AC(filename);
    uint32 count = r_u32(0x800ad1a8);
    if (index == count)
        return 0;
    uint32 entry = 0x800ad1a8 + index * 24;
    uint32 offset = r_u32(entry + 0x14);
    uint32 base = r_u32(0x800ad1ac);
    return base + (uint32)((sint32)offset >> 11);
}

uint32 G3ScaleMatrixL8008BCD8(uint32 matrix, uint32 scale)
{
    VECTOR factors = {0};
    factors.vx = r_s32(scale);
    factors.vy = r_s32(scale + 4);
    factors.vz = r_s32(scale + 8);
    ScaleMatrixL((MATRIX *)psx_addr(matrix, 20), &factors);
    return matrix;
}

void G3AnimateVisibleStreams8005B0F0(uint32 scene, uint32 camera)
{
    FUNCTION_MARKER(0x8005b0f0, "SLUS_008.06");
    uint32 model = r_u32(scene);
    uint32 stream = r_u32(model + 0x54);
    uint32 stream_count = r_u32(model + 0x50);
    uint32 view_x = r_u32(camera + 0x60);
    if (stream_count == 0)
        return;
    uint32 view_y = r_u32(camera + 0x64);
    uint32 view_z = r_u32(camera + 0x68);
    uint32 light_x = r_u32(camera + 0x6c);
    uint32 light_y = r_u32(camera + 0x70);
    uint32 view_width = r_u32(camera + 0x88);
    uint32 controls[5];
    for (uint32 index = 0; index < 5; ++index)
        controls[index] = r_u32(camera + 0x20 + index * 4);
    for (uint32 index = 0; index < 5; ++index)
        xport_gte_write_control(index, controls[index]);
    for (uint32 index = 0; index < 5; ++index)
        controls[index] = r_u32(camera + 0x40 + index * 4);
    for (uint32 index = 0; index < 5; ++index)
        xport_gte_write_control(index + 8, controls[index]);
    uint32 end = stream + stream_count * 36;
    uint32 colors = r_u32(model + 0x34);
    do
    {
        uint32 flags = r_u32(stream);
        uint32 packed_radius = r_u32(stream + 0x0c);
        if ((flags & 2) == 0)
        {
            uint32 frame = r_u32(stream + 4);
            xport_gte_write_data(0, r_u32(stream + 8));
            xport_gte_write_data(1, packed_radius & 0xffff);
            uint32 negative_radius = 0 - (uint32)((sint32)packed_radius >> 16);
            ++frame;
            xport_gte_execute(0x486012);
            uint32 period = r_u32(stream + 0x14);
            uint32 x = xport_gte_read_data(9) - view_x;
            bool visible = (sint32)(x - negative_radius) > 0;
            if (visible)
            {
                uint32 upper = x - (view_width - negative_radius);
                uint32 y = xport_gte_read_data(10);
                visible = (sint32)upper < 0;
                if (visible)
                {
                    y -= view_y;
                    uint32 z = xport_gte_read_data(11);
                    visible = (sint32)(y - negative_radius) > 0;
                    if (visible)
                        visible = (sint32)(z - view_z - negative_radius) > 0;
                }
            }
            if (visible)
            {
                xport_gte_execute(0x4a6012);
                if ((sint32)(frame - period) >= 0)
                    frame -= period;
                w_u32(stream + 4, frame);
                uint32 x = xport_gte_read_data(9) - light_x;
                uint32 y = xport_gte_read_data(10);
                visible = (sint32)(x - negative_radius) > 0;
                if (visible)
                    visible = (sint32)(y - light_y - negative_radius) > 0;
            }
            if (!visible)
                flags &= 0xfffffffe;
            else
            {
                uint32 color_stream = flags & 4;
                flags |= 1;
                uint32 entry_count = r_u32(stream + 0x1c);
                uint32 entry = r_u32(stream + 0x20);
                if (entry_count != 0)
                {
                    uint32 entry_end = entry + entry_count * 12;
                    period = r_u32(stream + 0x14);
                    uint32 frames = r_u32(stream + 0x18);
                    do
                    {
                        uint32 phase = r_u32(entry + 8);
                        uint32 base = r_u32(entry + 4);
                        uint32 destination = r_u32(entry);
                        uint32 sample_index = (phase >> 16) + frame;
                        if ((sint32)sample_index >= (sint32)period)
                            sample_index -= period;
                        uint32 sample = frames + sample_index * 6;
                        sint32 sample_z = r_s16(sample + 4);
                        sint32 sample_y = r_s16(sample + 2);
                        uint32 z = (phase & 0xffff) + (uint32)sample_z;
                        sint32 sample_x = r_s16(sample);
                        if (color_stream != 0)
                        {
                            if ((sint32)z < 0)
                                z = 0;
                            if ((sint32)z > 255)
                                z = 255;
                            uint32 y = (base >> 16) + (uint32)sample_y;
                            uint32 color_index = r_u16(destination + 6);
                            if ((sint32)y < 0)
                                y = 0;
                            if ((sint32)y > 255)
                                y = 255;
                            uint32 x = (base & 0xffff) + (uint32)sample_x;
                            uint32 output = colors + color_index * 4;
                            uint32 command = r_u8(output + 3);
                            if ((sint32)x < 0)
                                x = 0;
                            if ((sint32)x > 255)
                                x = 255;
                            w_u32(output, (z << 16) | (y << 8) | x | (command << 24));
                        }
                        else
                        {
                            uint32 y = (base >> 16) + (uint32)sample_y;
                            r_u16(destination + 6);
                            uint32 x = (base & 0xffff) + (uint32)sample_x;
                            w_u32(destination, x | (y << 16));
                            w_u16(destination + 4, z);
                        }
                        entry += 12;
                    } while ((sint32)entry < (sint32)entry_end);
                }
            }
        }
        w_u32(stream, flags);
        stream += 36;
    } while ((sint32)stream < (sint32)end);
}

void G3Function80026CAC(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80026cac, "SLUS_008.06");
    uint32 scene = r_u32(game + 0x48);
    uint32 camera = r_u32(game + 0x4c);
    w_u32(game + 0x6598, 1);
    G3AnimateVisibleStreams8005B0F0(scene, camera);
    w_u32(game + 0x6598, 1);
}

uint32 G3Function800194C4(uint32 source, uint32 vertices, uint32 model, uint32 scene, uint32 matrices, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800194c4, "SLUS_008.06");
    sint32 count = r_s16(model + 6);
    if (count <= 0)
        return 0;
    uint32 lighting_enabled = r_u32(0x8009d918);
    matrices = r_u32(guest_stack + 0x10);
    uint32 segment = r_u32(model + 0x18);
    uint32 lighting = 0;
    if (lighting_enabled != 0)
        lighting = r_u32(0x800a2188);
    uint32 first = r_u32(scene);
    uint32 second = r_u32(scene + 4);
    w_u32(0x1f800000, first);
    w_u32(0x1f800004, second);
    first = r_u32(scene + 8);
    second = r_u32(scene + 0x0c);
    uint32 third = r_u32(scene + 0x10);
    w_u32(0x1f800008, first);
    w_u32(0x1f80000c, second);
    w_u32(0x1f800010, third);
    do
    {
        uint32 range = r_u32(segment + 8);
        sint32 last_index = (sint32)range >> 16;
        if (last_index != -1)
        {
            uint32 end = source + ((uint32)last_index << 3);
            uint32 input = source + ((range & 0xffff) << 3);
            first = r_u32(0x1f800000);
            second = r_u32(0x1f800004);
            xport_gte_write_control(0, first);
            xport_gte_write_control(1, second);
            first = r_u32(0x1f800008);
            second = r_u32(0x1f80000c);
            third = r_u32(0x1f800010);
            xport_gte_write_control(2, first);
            xport_gte_write_control(3, second);
            xport_gte_write_control(4, third);
            for (uint32 column = 0; column < 3; ++column)
            {
                uint32 x = r_u16(matrices + column * 2);
                uint32 y = r_u16(matrices + 6 + column * 2);
                uint32 z = r_u16(matrices + 12 + column * 2);
                xport_gte_write_data(9, x);
                xport_gte_write_data(10, y);
                xport_gte_write_data(11, z);
                xport_gte_execute(0x49e012);
                x = xport_gte_read_data(9);
                y = xport_gte_read_data(10);
                z = xport_gte_read_data(11);
                w_u16(0x1f800014 + column * 2, x);
                w_u16(0x1f80001a + column * 2, y);
                w_u16(0x1f800020 + column * 2, z);
            }
            first = r_u32(scene + 0x14);
            second = r_u32(scene + 0x18);
            third = r_u32(scene + 0x1c);
            xport_gte_write_control(5, first);
            xport_gte_write_control(6, second);
            xport_gte_write_control(7, third);
            uint32 translation_y = r_u16(matrices + 0x18);
            uint32 translation_x = r_u16(matrices + 0x14);
            xport_gte_write_data(0, translation_x | (translation_y << 16));
            xport_gte_write_data(1, r_u32(matrices + 0x1c));
            xport_gte_execute(0x480012);
            first = xport_gte_read_data(9);
            second = xport_gte_read_data(10);
            third = xport_gte_read_data(11);
            xport_gte_write_control(5, first);
            xport_gte_write_control(6, second);
            xport_gte_write_control(7, third);
            first = r_u32(0x1f800014);
            second = r_u32(0x1f800018);
            xport_gte_write_control(0, first);
            xport_gte_write_control(1, second);
            first = r_u32(0x1f80001c);
            second = r_u32(0x1f800020);
            third = r_u32(0x1f800024);
            xport_gte_write_control(2, first);
            xport_gte_write_control(3, second);
            xport_gte_write_control(4, third);
            uint32 prefetched[6];
            for (uint32 word = 0; word < 6; ++word)
                prefetched[word] = r_u32(input + word * 4);
            bool bulk = end >= input + 16;
            while (bulk)
            {
                for (uint32 word = 0; word < 6; ++word)
                    xport_gte_write_data(word, prefetched[word]);
                input += 24;
                xport_gte_execute(0x280030);
                for (uint32 word = 0; word < 6; ++word)
                    prefetched[word] = r_u32(input + word * 4);
                w_u32(vertices, xport_gte_read_data(12));
                w_u32(vertices + 4, xport_gte_read_data(17));
                w_u32(vertices + 8, xport_gte_read_data(13));
                uint32 flags = xport_gte_read_control(31);
                w_u32(vertices + 12, xport_gte_read_data(18));
                w_u32(vertices + 16, xport_gte_read_data(14));
                w_u32(vertices + 20, xport_gte_read_data(19));
                if (((flags >> 16) & 3) != 0)
                {
                    for (uint32 vertex = 0; vertex < 3; ++vertex)
                    {
                        xport_gte_execute(0x180001);
                        uint32 next_xy = 0;
                        uint32 next_z = 0;
                        if (vertex < 2)
                        {
                            next_xy = xport_gte_read_data(2 + vertex * 2);
                            next_z = xport_gte_read_data(3 + vertex * 2);
                        }
                        uint32 depth = xport_gte_read_data(19);
                        uint32 flags = xport_gte_read_control(31);
                        if (vertex < 2)
                        {
                            xport_gte_write_data(0, next_xy);
                            xport_gte_write_data(1, next_z);
                        }
                        w_u32(vertices + vertex * 8 + 4, depth | (((flags >> 16) & 3) << 30));
                    }
                }
                bulk = end >= input + 16;
                if (lighting_enabled != 0)
                {
                    uint32 indices[3];
                    indices[0] = (uint32)r_s16(input - 18);
                    indices[1] = (uint32)r_s16(input - 10);
                    indices[2] = (uint32)r_s16(input - 2);
                    for (uint32 vertex = 0; vertex < 3; ++vertex)
                    {
                        uint32 normal = 0x80098838 + (indices[vertex] << 3);
                        xport_gte_write_data(0, r_u32(normal));
                        xport_gte_write_data(1, r_u32(normal + 4));
                        xport_gte_execute(0x486012);
                        w_u32(lighting + vertex * 12, xport_gte_read_data(9));
                        w_u32(lighting + vertex * 12 + 4, xport_gte_read_data(10));
                        w_u32(lighting + vertex * 12 + 8, xport_gte_read_data(11));
                    }
                    lighting += 36;
                }
                vertices += 24;
            }
            if ((sint32)input <= (sint32)end)
            {
                xport_gte_write_data(0, prefetched[0]);
                xport_gte_write_data(1, prefetched[1]);
                input += 8;
                xport_gte_execute(0x180001);
                w_u32(vertices, xport_gte_read_data(14));
                uint32 depth = xport_gte_read_data(19);
                uint32 flags = xport_gte_read_control(31);
                w_u32(vertices + 4, depth | (((flags >> 16) & 3) << 30));
                vertices += 8;
                if (lighting_enabled != 0)
                {
                    uint32 normal = 0x80098838 + ((uint32)r_s16(input - 2) << 3);
                    xport_gte_write_data(0, r_u32(normal));
                    xport_gte_write_data(1, r_u32(normal + 4));
                    lighting += 12;
                    xport_gte_execute(0x486012);
                    w_u32(lighting - 12, xport_gte_read_data(9));
                    w_u32(lighting - 8, xport_gte_read_data(10));
                    w_u32(lighting - 4, xport_gte_read_data(11));
                }
                if ((sint32)input <= (sint32)end)
                {
                    xport_gte_write_data(0, prefetched[2]);
                    xport_gte_write_data(1, prefetched[3]);
                    xport_gte_execute(0x180001);
                    w_u32(vertices, xport_gte_read_data(14));
                    depth = xport_gte_read_data(19);
                    flags = xport_gte_read_control(31);
                    w_u32(vertices + 4, depth | ((flags & 3) << 30));
                    vertices += 8;
                    if (lighting_enabled != 0)
                    {
                        uint32 normal = 0x80098838 + ((uint32)r_s16(input + 6) << 3);
                        xport_gte_write_data(0, r_u32(normal));
                        xport_gte_write_data(1, r_u32(normal + 4));
                        lighting += 12;
                        xport_gte_execute(0x486012);
                        w_u32(lighting - 12, xport_gte_read_data(9));
                        w_u32(lighting - 8, xport_gte_read_data(10));
                        w_u32(lighting - 4, xport_gte_read_data(11));
                    }
                }
            }
        }
        matrices += 32;
        --count;
        segment += 24;
    } while (count != 0);
    return 0;
}

uint32 G3Function80063C6C(uint32 model, uint32 vertices, uint32 packets, uint32 order_table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80063c6c, "SLUS_008.06");
    w_u32(0x1f800064, guest_stack);
    uint32 game = r_u32(0x80097048);
    uint32 table_bytes = r_u32(game + 0x28);
    uint32 limit = r_u32(packets + 8);
    uint32 winding = r_u32(game);
    uint32 palette_mode = r_u32(game + 0x14);
    uint32 depth_shift = r_u32(game + 0x2c);
    table_bytes <<= 2;
    limit -= 0x30;
    w_u32(0x1f800000, winding);
    w_u32(0x1f80002c, palette_mode);
    w_u32(0x1f800040, depth_shift);
    w_u32(0x1f80003c, table_bytes);
    w_u32(0x1f800068, limit);
    uint32 cursor = r_u32(packets + 4);
    sint32 count = r_s16(model + 4);
    uint32 source = r_u32(model + 8);
    uint32 palette = r_u32(model + 0x0c);
    if (count == 0)
        return cursor;
    uint32 triangle = r_u32(model + 0x14);
    uint32 end = triangle + (uint32)count * 12;
    do
    {
        do
        {
            uint32 packed_first = r_u32(triangle);
            uint32 packed_second = r_u32(triangle + 4);
            uint32 projected[3];
            projected[0] = vertices + ((packed_first & 0xffff) << 3);
            projected[1] = vertices + (packed_first >> 13);
            projected[2] = vertices + ((packed_second & 0xffff) << 3);
            xport_gte_write_data(12, r_u32(projected[0]));
            xport_gte_write_data(13, r_u32(projected[1]));
            xport_gte_write_data(14, r_u32(projected[2]));
            uint32 third_uv = 0x1000200;
            xport_gte_execute(0x1400006);
            uint32 remaining = cursor - r_u32(0x1f800068);
            winding = r_u32(0x1f800000);
            if ((sint32)remaining >= 0)
                return cursor;
            uint32 area = xport_gte_read_data(24);
            if (winding != 0)
                area = 0 - area;
            if ((sint32)area >= 0)
                break;
            uint32 screen_first = xport_gte_read_data(12);
            uint32 screen_second = xport_gte_read_data(13);
            uint32 screen_third = xport_gte_read_data(14);
            if ((screen_first & screen_second & screen_third & 0x80008000) != 0)
                break;
            screen_first = 0x1000200 - screen_first;
            screen_second = 0x1000200 - screen_second;
            screen_third = 0x1000200 - screen_third;
            uint32 flags = packed_second >> 24;
            if ((screen_first & screen_second & screen_third & 0x80008000) != 0)
                break;
            uint32 depth_first = r_u32(projected[0] + 4);
            uint32 depth_second = r_u32(projected[1] + 4);
            if ((depth_first & 0xf0000000) != 0)
                break;
            xport_gte_write_data(17, depth_first & 0xffff);
            uint32 depth_third = r_u32(projected[2] + 4);
            if ((depth_second & 0xf0000000) != 0)
                break;
            xport_gte_write_data(18, depth_second & 0xffff);
            if ((depth_third & 0xf0000000) != 0)
                break;
            xport_gte_write_data(19, depth_third & 0xffff);
            uint32 bucket;
            uint32 texture;
            if ((flags & 4) != 0)
            {
                xport_gte_execute(0x158002d);
                texture = r_u32(triangle + 8);
                bucket = xport_gte_read_data(7) << 2;
            }
            else
            {
                depth_first = xport_gte_read_data(17);
                depth_second = xport_gte_read_data(18);
                depth_third = xport_gte_read_data(19);
                if ((sint32)depth_first >= (sint32)depth_second)
                    depth_second = depth_first;
                depth_shift = r_u32(0x1f800040);
                if ((sint32)depth_second >= (sint32)depth_third)
                    depth_third = depth_second;
                texture = r_u32(triangle + 8);
                bucket = (depth_third >> (depth_shift & 31)) << 2;
            }
            if ((sint32)texture <= 0)
            {
                third_uv = r_u32(texture + 8);
                bucket += (uint32)((sint32)third_uv >> 24) << 3;
            }
            table_bytes = r_u32(0x1f80003c);
            if ((sint32)bucket < 0 || (sint32)bucket >= (sint32)table_bytes)
                break;
            bool textured = (flags & 2) != 0;
            bool smooth = (flags & 1) != 0;
            if (textured && smooth && r_u32(0x1f80002c) != 0)
            {
                uint32 indices_first = r_u32(triangle);
                uint32 indices_second = r_u32(triangle + 4);
                xport_gte_write_data(6, r_u32(palette + ((indices_first & 0xffff) << 2)));
                uint32 command = r_u32(texture + 0x0c);
                xport_gte_execute(0x780010);
                uint32 color_first = xport_gte_read_data(22);
                xport_gte_write_data(6, r_u32(palette + ((indices_first >> 16) << 2)));
                command &= 0xff000000;
                color_first |= command;
                xport_gte_execute(0x780010);
                uint32 color_second = xport_gte_read_data(22);
                xport_gte_write_data(6, r_u32(palette + ((indices_second & 0xffff) << 2)));
                color_second |= command;
                xport_gte_execute(0x780010);
                uint32 color_third = xport_gte_read_data(22) | command;
                xport_gte_write_data(20, color_first);
                xport_gte_write_data(21, color_second);
                xport_gte_write_data(22, color_third);
            }
            else if (textured && (flags & 8) != 0)
            {
                if (smooth)
                {
                    xport_gte_write_data(20, r_u32(texture + 0x0c));
                    xport_gte_write_data(21, r_u32(texture + 0x0c));
                }
                xport_gte_write_data(22, r_u32(texture + 0x0c));
            }
            else
            {
                if (smooth)
                {
                    uint32 normal_first = (uint32)r_s16(source + ((packed_first & 0xffff) << 3) + 6);
                    uint32 normal_second = (uint32)r_s16(source + ((packed_first >> 16) << 3) + 6);
                    uint32 normal_third = (uint32)r_s16(source + ((packed_second & 0xffff) << 3) + 6);
                    normal_first = 0x80098838 + (normal_first << 3);
                    normal_second = 0x80098838 + (normal_second << 3);
                    normal_third = 0x80098838 + (normal_third << 3);
                    xport_gte_write_data(0, r_u32(normal_first));
                    xport_gte_write_data(1, r_u32(normal_first + 4));
                    xport_gte_write_data(2, r_u32(normal_second));
                    xport_gte_write_data(3, r_u32(normal_second + 4));
                    xport_gte_write_data(4, r_u32(normal_third));
                    xport_gte_write_data(5, r_u32(normal_third + 4));
                }
                else
                {
                    uint32 normal = 0x80098838 + ((packed_second >> 13) & 0x7f8);
                    xport_gte_write_data(0, r_u32(normal));
                    xport_gte_write_data(1, r_u32(normal + 4));
                }
                xport_gte_write_data(6, textured ? r_u32(texture + 0x0c) : texture);
                xport_gte_execute(smooth ? 0xf80416 : 0xe80413);
            }
            uint32 link = order_table + bucket;
            if (!textured)
            {
                uint32 header = r_u32(link);
                w_u32(cursor, header | (smooth ? 0x06000000 : 0x04000000));
                w_u32(link, cursor & 0xffffff);
                w_u32(cursor + 8, xport_gte_read_data(12));
                w_u32(cursor + (smooth ? 16 : 12), xport_gte_read_data(13));
                w_u32(cursor + (smooth ? 24 : 16), xport_gte_read_data(14));
                if (smooth)
                {
                    w_u32(cursor + 4, xport_gte_read_data(20));
                    w_u32(cursor + 12, xport_gte_read_data(21));
                    w_u32(cursor + 20, xport_gte_read_data(22));
                    cursor += 28;
                }
                else
                {
                    w_u32(cursor + 4, xport_gte_read_data(22));
                    cursor += 20;
                }
                break;
            }
            uint32 environment_enabled = r_u32(0x1f800060);
            bool environment = (flags & 0x10) != 0 && environment_enabled != 0;
            if (!environment || (flags & 0x20) != 0)
            {
                uint32 uv_first = r_u32(texture);
                uint32 uv_second = r_u32(texture + 4);
                if (smooth && !environment)
                    w_u32(cursor + 36, third_uv);
                w_u32(cursor + 12, uv_first);
                w_u32(cursor + (smooth ? 24 : 20), uv_second);
                if (!smooth || environment)
                    w_u32(cursor + (smooth ? 36 : 28), third_uv);
                uint32 header = r_u32(link);
                w_u32(cursor, header | (smooth ? 0x09000000 : 0x07000000));
                w_u32(link, cursor & 0xffffff);
                uint32 color = 0;
                if (environment && !smooth)
                    color = xport_gte_read_data(22);
                w_u32(cursor + 8, xport_gte_read_data(12));
                w_u32(cursor + (smooth ? 20 : 16), xport_gte_read_data(13));
                w_u32(cursor + (smooth ? 32 : 24), xport_gte_read_data(14));
                if (smooth)
                {
                    if (environment)
                    {
                        color = xport_gte_read_data(20);
                        w_u32(cursor + 16, xport_gte_read_data(21));
                        w_u32(cursor + 28, xport_gte_read_data(22));
                        w_u32(cursor + 4, color);
                    }
                    else
                    {
                        w_u32(cursor + 4, xport_gte_read_data(20));
                        w_u32(cursor + 16, xport_gte_read_data(21));
                        w_u32(cursor + 28, xport_gte_read_data(22));
                    }
                    cursor += 40;
                }
                else
                {
                    if (!environment)
                        color = xport_gte_read_data(22);
                    w_u32(cursor + 4, color);
                    cursor += 32;
                }
                if (!environment)
                    break;
                xport_gte_write_data(smooth ? 20 : 22, color & (smooth ? 0x34ffffff : 0x24ffffff));
            }
            w_u32(cursor + 8, xport_gte_read_data(12));
            w_u32(cursor + (smooth ? 20 : 16), xport_gte_read_data(13));
            w_u32(cursor + (smooth ? 32 : 24), xport_gte_read_data(14));
            if (smooth)
            {
                w_u32(cursor + 4, xport_gte_read_data(20));
                w_u32(cursor + 16, xport_gte_read_data(21));
                w_u32(cursor + 28, xport_gte_read_data(22));
            }
            else
                w_u32(cursor + 4, xport_gte_read_data(22));
            uint32 header = r_u32(link);
            w_u32(cursor, header | (smooth ? 0x09000000 : 0x07000000));
            w_u32(link, cursor & 0xffffff);
            uint32 environment_source = r_u32(0x1f80005c);
            uint32 offset_u = r_u32(0x1f800044);
            uint32 scale_u = r_u32(0x1f80004c);
            uint32 scale_v = r_u32(0x1f800050);
            uint32 offset_v = r_u32(0x1f800048);
            uint32 texture_word = r_u32(0x1f800054);
            for (uint32 vertex = 0; vertex < 3; ++vertex)
            {
                uint32 relative = projected[vertex] - vertices;
                uint32 coordinate = relative + (relative >> 1) + environment_source;
                uint32 u = r_u32(coordinate);
                uint32 v = r_u32(coordinate + 4);
                u = ((uint32)((sint32)(u * scale_u) >> 13) + offset_u) & 0xff;
                v = ((uint32)((sint32)(v * scale_v) >> 13) + offset_v) & 0xff;
                uint32 uv = u | (v << 8);
                if (vertex < 2)
                    uv |= texture_word;
                uint32 uv_offset = vertex == 0 ? 12 : (vertex == 1 ? (smooth ? 24 : 20) : (smooth ? 36 : 28));
                w_u32(cursor + uv_offset, uv);
                if (vertex == 0)
                {
                    texture_word = r_u32(0x1f800058);
                    if ((flags & 0xc0) != 0)
                    {
                        texture_word |= (((flags & 0xc0) >> 6) - 1) << 21;
                        w_u8(cursor + 7, smooth ? 0x36 : 0x26);
                    }
                }
            }
            cursor += smooth ? 40 : 32;
        } while (false);
        triangle += 12;
    } while (triangle < end);
    return cursor;
}

sint32 G3NativeSaveGame(uint32 state, uint32 guest_stack)
{
    uint32 frame = guest_stack - 0x100;
    uint32 record = frame + 0x10;
    uint32 payload = record + 4;
    G3Function8006B58C(state, payload, frame);
    uint32 checksum = G3CardChecksum80067C48(payload, 0xe0);
    w_u16(record, 1);
    w_u16(record + 2, checksum);
    return G3NativeWriteSlot(state, record);
}

sint32 G3NativeLoadGame(uint32 state, uint32 guest_stack)
{
    uint32 frame = guest_stack - 0x100;
    uint32 record = frame + 0x10;
    sint32 status = G3NativeReadSlot(state, record);
    if (status != 1)
        return status;
    uint32 payload = record + 4;
    uint32 checksum = G3CardChecksum80067C48(payload, 0xe0);
    if ((checksum & 0xffff) != r_u16(record + 2) || r_u32(payload + 0xdc) != 0xabacabcd)
        return -2;
    return G3Function8006B3A0(state, payload, 0, frame) != 0 ? 1 : -2;
}

uint32 G3NativeSaveResultPage(uint32 context, uint32 callback, uint32 incoming_third_glyph, uint32 guest_stack)
{
    uint32 frame = guest_stack - 0x80;
    uint32 state = G3Function8006A600(context);
    sint32 result = r_s32(state + 0x40);
    const char *message;
    if (result == 1)
        message = "Game saved";
    else if (result == 0)
        message = "No saved game";
    else if (result == -2)
        message = "Saved game is invalid";
    else
        message = r_u32(state + 0xc) == 2 ? "Load failed" : "Save failed";
    size_t bytes = strlen(message) + 1;
    memcpy(psx_addr(frame + 0x10, bytes), message, bytes);
    G3Function8006AAD4(context, frame + 0x10, incoming_third_glyph, frame);
    uint32 text = G3Function80065D38(result == 1 ? 0x2c : 8);
    uint32 value = G3Function80069A4C(context, 0, text, callback, frame);
    return G3Function80069BC8(context, 0, 0, 1, value, frame);
}

void G3ResetMenuText(uint32 guest_stack)
{
    w_u32(guest_stack, 0x800a0054);
    w_u32(guest_stack, 0x800a0054);
    G3Function80023030(0x800a0054, 1, guest_stack + 4, 0, guest_stack - 0x18, 0);
}

uint32 G3Function80012DC0(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80012dc0, "SLUS_008.06");
    uint32 player = r_u32(0x8009efc4);
    uint32 frame = guest_stack - 0x38;
    sint32 mode = r_s16(game + 0x6780);
    uint32 data = r_u32(player + 0x24);
    if (mode != 0)
        return player;
    G3Function80035AD4(frame);
    uint32 flags = r_u32(game + 0xe8);
    player = r_u32(0x8009efc4);
    w_u32(game + 0xe8, flags | 0x10);
    flags = r_u32(player + 0x10);
    w_u32(player + 0x10, flags | 0x100);
    G3Function80012A28(frame);
    w_u32(frame + 0x10, frame + 0x24);
    G3Function800283E8(game, frame + 0x18, frame + 0x1c, frame + 0x20, frame);
    if (r_u32(0x800a04f0) != 0 && (r_u32(game + 0x9c) & 0x4000) != 0)
    {
        uint32 state = r_u32(game + 0x67e8);
        if (G3Function800689AC(state) == 0)
        {
            uint32 result = r_u32(game + 0x6834) | 0x200000;
            w_u32(game + 0x6834, result);
            return result;
        }
    }
    player = r_u32(0x8009efc4);
    uint32 activity = r_u32(player + 0x154);
    bool menu = activity == 1;
    if (!menu && activity == 0x80000000)
    {
        uint32 linked = r_u32(data + 0x1fc);
        if (linked != 0)
        {
            uint32 callback = r_u32(linked + 0xec);
            if (callback != 0)
                menu = G3DispatchStateEvent(callback, linked, game, 0x7f2, 0, frame) != 0;
        }
    }
    if (menu && r_s32(0x800a058c) < 16)
    {
        uint32 context = r_u32(game + 0x67e4);
        G3Function80069B48(context, 1, frame);
    }
    else
    {
        G3Function80012D38(game, frame);
        G3Function80021A74(0x21c);
        uint32 text = G3Function80065D38(0x2e);
        G3Function800234A0(text, 0x50, 0, 0, frame, 0);
    }
    uint32 value = r_u32(game + 0x6818);
    w_u32(0x800a04f0, 1);
    G3Function80047EE4(0, value);
    value = (uint32)r_s16(game + 0x681c);
    G3Function80047EE4(7, value);
    value = r_u32(game + 0x6820);
    G3Function80047EE4(1, value);
    value = r_u32(game + 0x6828);
    G3Function80047EE4(2, value);
    value = r_u32(game + 0x6824);
    G3Function80047EE4(4, value);
    value = r_u32(game + 0x682c);
    G3Function80047EE4(3, value);
    return G3Function80012DA0(frame);
}
