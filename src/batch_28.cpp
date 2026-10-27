#include "xport.h"
#include "psx.h"
#include "game_functions.h"

// Native helpers for the audited object-pair collision body
static void G3CollisionMatrix8005CBCC(uint32 matrix, bool translation)
{
    uint32 a = r_u32(matrix);
    uint32 b = r_u32(matrix + 4);
    xport_gte_write_control(0, a);
    xport_gte_write_control(1, b);
    a = r_u32(matrix + 8);
    b = r_u32(matrix + 0xc);
    uint32 c = r_u32(matrix + 0x10);
    xport_gte_write_control(2, a);
    xport_gte_write_control(3, b);
    xport_gte_write_control(4, c);
    if (translation)
    {
        a = r_u32(matrix + 0x14);
        b = r_u32(matrix + 0x18);
        xport_gte_write_control(5, a);
        c = r_u32(matrix + 0x1c);
        xport_gte_write_control(6, b);
        xport_gte_write_control(7, c);
    }
}

static void G3CollisionVector8005CBCC(uint32 vector, uint32 opcode)
{
    uint32 value = r_u32(vector);
    xport_gte_write_data(0, value);
    value = r_u32(vector + 4);
    xport_gte_write_data(1, value);
    xport_gte_execute(opcode);
}

static bool G3CollisionKinds8005CBCC(uint32 first, uint32 second, uint32 first_kind, uint32 second_kind, uint32 nine, bool box)
{
    if ((r_u32(first + 0x14) & 0x08000000) != 0)
    {
        uint32 kind = r_u8(first_kind);
        if (kind - 6 < 4)
        {
            kind = r_u8(second_kind);
            if (kind == nine || kind - 3 < 3)
                return true;
        }
    }
    if ((r_u32(second + 0x14) & 0x08000000) != 0)
    {
        uint32 kind = r_u8(second_kind);
        if (kind - 6 < 4)
        {
            kind = r_u8(first_kind);
            if (kind == nine || kind - 3 < 3)
                return true;
        }
    }
    uint32 left = box ? second_kind : first_kind;
    uint32 right = box ? first_kind : second_kind;
    uint32 kind = r_u8(left);
    if (kind == nine || kind - 3 < 3)
    {
        kind = r_u8(right);
        if (kind == nine || kind - 3 < 3)
            return true;
    }
    kind = r_u8(left);
    if (kind == nine || kind - 3 < 3)
    {
        if (r_u8(right) < 3)
            return true;
    }
    if (r_u8(left) < 3)
    {
        kind = r_u8(right);
        if (kind == nine || kind - 3 < 3)
            return true;
    }
    return false;
}

uint32 G3Function80016ED4(void)
{
    FUNCTION_MARKER(0x80016ed4, "SLUS_008.06");
    return 0;
}

void G3Function800209F8(uint32 value)
{
    FUNCTION_MARKER(0x800209f8, "SLUS_008.06");
    w_u32(0x8009d628, value);
}

uint32 G3Function8005FE44(uint32 context, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005fe44, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 object = r_u32(context + 0x14);
    uint32 mask = G3Function80063594(object);
    return G3Function8005FE88(context, game, mask, frame);
}

uint32 G3Function8005FE88(uint32 context, uint32 game, uint32 mask, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005fe88, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 result = 0;
    uint32 lists = r_u32(game + 0x5c);
    for (uint32 bucket = 64; bucket < 128; ++bucket)
    {
        uint32 object = r_u32(lists + (bucket << 3) + 0x10);
        while (object != 0)
        {
            uint32 vector = r_u32(context);
            sint32 object_x = r_s16(object + 0x58);
            sint32 query_x = r_s16(vector);
            w_u32(0x1f800000, (uint32)(object_x - query_x));
            vector = r_u32(context);
            sint32 object_y = r_s16(object + 0x5a);
            sint32 query_y = r_s16(vector + 2);
            w_u32(0x1f800004, (uint32)(object_y - query_y));
            uint32 x = r_u32(0x1f800000);
            x = (uint32)((sint32)x >> 1);
            uint32 xx = x * x;
            vector = r_u32(context);
            sint32 object_z = r_s16(object + 0x5c);
            sint32 query_z = r_s16(vector + 4);
            w_u32(0x1f800000, x);
            uint32 y = r_u32(0x1f800004);
            y = (uint32)((sint32)y >> 1);
            uint32 yy = y * y;
            uint32 z = (uint32)(object_z - query_z);
            w_u32(0x1f800004, y);
            w_u32(0x1f800008, z);
            z = (uint32)((sint32)z >> 1);
            w_u32(0x1f800008, z);
            uint32 zz = z * z;
            uint32 model = r_u32(object + 0x1c);
            sint32 index = r_s16(object + 0xa2);
            uint32 table = r_u32(model + 0xc);
            uint32 geometry = r_u32(table + ((uint32)index << 2));
            vector = r_u32(context);
            uint32 object_radius = r_u16(geometry + 0x26);
            uint32 query_radius = r_u16(vector + 6);
            uint32 radius = (uint32)((sint32)(object_radius << 16) >> 17) + (query_radius >> 1);
            uint32 radius_squared = radius * radius;
            uint32 distance_squared = xx + yy + zz;
            if ((sint32)distance_squared < (sint32)radius_squared)
            {
                if (G3Function8005C1A8(context, object, frame) != 0)
                    result = 1;
            }
            object = r_u32(object + 4);
        }
    }
    uint32 owner = r_u32(game + 0x48);
    uint32 world = r_u32(owner);
    if (G3Function80063664(context, world, mask, 0, frame) != 0)
        result = 1;
    return result;
}

static void G3CollisionContact8005C1A8(uint32 context, uint32 object, uint32 primitive, uint32 kind, uint32 initial_handler, uint32 guest_stack)
{
    uint32 handler = initial_handler;
    if (handler == 0)
        return;
    if (r_u32(context + 0x14) == 0)
        return;
    uint32 game = r_u32(0x80097048);
    w_u16(0x1f800138, 1);
    w_u16(0x1f80013a, kind);
    uint32 payload = r_u32(context + 0x18);
    uint32 y = r_u16(0x1f800122);
    uint32 z = r_u16(0x1f800124);
    w_u32(0x1f800140, primitive);
    w_u32(0x1f80013c, payload);
    uint32 recipient = r_u32(context + 0x14);
    uint32 x = r_u16(0x1f800120);
    w_u32(0x1f800148, object);
    w_u16(0x1f80015e, y);
    w_u16(0x1f800160, z);
    w_u16(0x1f80015c, x);
    w_u32(0x1f800144, recipient);
    recipient = r_u32(context + 0x14);
    w_u32(recipient + 0xb0, 0x1f800134);
    recipient = r_u32(context + 0x14);
    handler = r_u32(context + 8);
    G3DispatchStateEvent(handler, recipient, game, 3, 0x1f800134, guest_stack);
}

uint32 G3Function8005C1A8(uint32 context, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005c1a8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    uint32 scratch = 0x1f8000b8;
    w_u32(frame + 0x28, 0);
    uint32 model = r_u32(object + 0x1c);
    sint32 index = r_s16(object + 0xa2);
    uint32 flags = r_u32(object + 0x14);
    uint32 point = r_u32(context);
    uint32 table = r_u32(model + 0xc);
    w_u32(frame + 0x18, point);
    uint32 geometry = r_u32(table + ((uint32)index << 2));
    if ((flags & 0x20000000) != 0)
    {
        uint32 mask = r_u32(object + 0x144);
        w_u32(frame + 0x28, mask);
    }
    w_u32(frame + 0x2c, 0);
    if ((flags & 0x20000000) != 0)
    {
        uint32 mask = r_u32(object + 0x148);
        w_u32(frame + 0x2c, mask);
    }
    w_u32(frame + 0x30, 0);
    if ((flags & 0x20000000) != 0)
    {
        uint32 mask = r_u32(object + 0x14c);
        w_u32(frame + 0x30, mask);
    }
    if (r_u32(object + 0x30) == 0)
        return 0;
    uint32 value = r_u32(geometry + 0x10);
    w_u32(scratch + 0xb8, value);
    value = r_u32(geometry + 8);
    w_u32(scratch + 0xbc, value);
    uint32 matrix = r_u32(object + 0x30);
    sint32 count = r_s16(geometry + 6);
    uint32 record = r_u32(geometry + 0x18);
    uint32 end = record + (((uint32)count * 3) << 3);
    w_u32(frame + 0x20, end);
    w_u32(frame + 0x1c, record);
    uint32 hit = 0;
    while (record < end)
    {
        uint32 primitives = r_u32(record + 0x14);
        if (primitives != 0)
        {
            G3Function800313E0(scratch, matrix, frame);
            uint32 a = r_u32(scratch);
            uint32 b = r_u32(scratch + 4);
            xport_gte_write_control(0, a);
            xport_gte_write_control(1, b);
            a = r_u32(scratch + 8);
            b = r_u32(scratch + 0xc);
            uint32 c = r_u32(scratch + 0x10);
            xport_gte_write_control(2, a);
            xport_gte_write_control(3, b);
            xport_gte_write_control(4, c);
            a = r_u32(scratch + 0x14);
            b = r_u32(scratch + 0x18);
            xport_gte_write_control(5, a);
            c = r_u32(scratch + 0x1c);
            xport_gte_write_control(6, b);
            xport_gte_write_control(7, c);
            uint32 original_point = r_u32(frame + 0x18);
            value = r_u32(original_point);
            xport_gte_write_data(0, value);
            value = r_u32(original_point + 4);
            xport_gte_write_data(1, value);
            xport_gte_execute(0x480012);
            for (uint32 axis = 0; axis < 3; ++axis)
                w_u32(scratch + 0x28 + axis * 4, xport_gte_read_data(25 + axis));
            w_u32(scratch + 0x70, xport_gte_read_control(31));
            uint32 previous = r_u32(context + 4);
            value = r_u32(previous);
            xport_gte_write_data(0, value);
            value = r_u32(previous + 4);
            xport_gte_write_data(1, value);
            xport_gte_execute(0x480012);
            for (uint32 axis = 0; axis < 3; ++axis)
                w_u32(scratch + 0x34 + axis * 4, xport_gte_read_data(25 + axis));
            w_u32(scratch + 0x70, xport_gte_read_control(31));
            uint32 halves[6];
            for (uint32 axis = 0; axis < 3; ++axis)
                halves[axis] = r_u16(scratch + 0x34 + axis * 4);
            for (uint32 axis = 0; axis < 3; ++axis)
                halves[axis + 3] = r_u16(scratch + 0x28 + axis * 4);
            for (uint32 axis = 0; axis < 3; ++axis)
                w_u16(scratch + 0x58 + axis * 2, halves[axis]);
            for (uint32 axis = 0; axis < 3; ++axis)
                w_u16(scratch + 0x74 + axis * 2, halves[axis + 3]);
            value = r_u16(original_point + 6);
            w_u16(scratch + 0x7a, value);
            uint32 spheres = r_u32(primitives + 8);
            if (spheres != 0 && (r_u32(frame + 0x28) & 1) == 0)
            {
                uint32 sphere = r_u32(primitives + 0xc);
                uint32 sphere_end = sphere + (spheres << 4);
                w_u32(frame + 0x24, sphere_end);
                while (sphere < sphere_end)
                {
                    uint32 differences[3];
                    uint32 squares = 0;
                    for (uint32 axis = 0; axis < 3; ++axis)
                    {
                        sint32 center = r_s16(scratch + 0x74 + axis * 2);
                        sint32 coordinate = r_s16(sphere + 8 + axis * 2);
                        differences[axis] = (uint32)(center - coordinate);
                        if (axis < 2)
                            w_u32(scratch + 0x4c + axis * 4, differences[axis]);
                        squares += differences[axis] * differences[axis];
                    }
                    uint32 radius = r_u16(scratch + 0x7a);
                    w_u32(scratch + 0x54, differences[2]);
                    uint32 other_radius = r_u16(sphere + 0xe);
                    radius += other_radius;
                    if ((sint32)squares < (sint32)(radius * radius))
                    {
                        uint32 sorted[3];
                        for (uint32 axis = 0; axis < 3; ++axis)
                            sorted[axis] = (sint32)differences[axis] < 0 ? 0 - differences[axis] : differences[axis];
                        if (sorted[0] > sorted[1]) { uint32 swap = sorted[0]; sorted[0] = sorted[1]; sorted[1] = swap; }
                        if (sorted[1] > sorted[2]) { uint32 swap = sorted[1]; sorted[1] = sorted[2]; sorted[2] = swap; }
                        if (sorted[0] > sorted[1]) { uint32 swap = sorted[0]; sorted[0] = sorted[1]; sorted[1] = swap; }
                        uint32 weighted = sorted[2] * 30 + sorted[1] * 12 + sorted[0] * 9;
                        if (weighted != 0)
                        {
                            uint32 average = (uint32)((sint32)weighted >> 5);
                            uint32 correction[3];
                            for (uint32 axis = 0; axis < 3; ++axis)
                            {
                                uint32 size = r_u16(sphere + 0xe);
                                uint32 query_radius = r_u16(scratch + 0x7a);
                                uint32 component = r_u32(scratch + 0x4c + axis * 4);
                                uint32 product = component * (size + query_radius - average);
                                w_u32(scratch + 0x4c + axis * 4, product);
                                uint32 numerator = product << 5;
                                correction[axis] = (uint32)((sint64)(sint32)numerator / (sint32)weighted);
                            }
                            w_u16(scratch + 0x60, correction[0]);
                            w_u16(scratch + 0x62, correction[1]);
                            w_u16(scratch + 0x64, correction[2]);
                            ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(scratch + 0x60, 8), (SVECTOR *)psx_addr(scratch + 0x68, 6));
                            for (uint32 axis = 0; axis < 3; ++axis)
                            {
                                value = r_u16(sphere + 8 + axis * 2);
                                w_u16(0x800a25a4 + axis * 2, value);
                            }
                            uint32 callback = r_u32(context + 8);
                            w_u32(0x800a25ac, matrix);
                            hit = 1;
                            G3CollisionContact8005C1A8(context, object, sphere, 1, callback, frame);
                        }
                    }
                    sphere_end = r_u32(frame + 0x24);
                    sphere += 0x10;
                }
            }
            uint32 triangles = r_u32(primitives);
            if (triangles != 0 && (r_u32(frame + 0x2c) & 1) == 0)
            {
                a = r_u16(scratch + 0x74);
                b = r_u16(scratch + 0x76);
                c = r_u16(scratch + 0x78);
                w_u32(scratch + 0xac, 0);
                w_u16(scratch + 0x20, a);
                w_u16(scratch + 0x22, b);
                w_u16(scratch + 0x24, c);
                triangles = r_u32(primitives);
                uint32 triangle = r_u32(primitives + 4);
                uint32 triangle_end = triangle + triangles * 12;
                while (triangle < triangle_end)
                {
                    w_u32(scratch + 0xac, 0);
                    w_u32(scratch + 0xb4, triangle);
                    if ((sint32)G3Function8006340C(scratch + 0x74, scratch + 0x58, scratch + 0xac, scratch + 0x94, frame) > 0)
                    {
                        a = r_u16(scratch + 0x74);
                        b = r_u16(scratch + 0x20);
                        c = r_u16(scratch + 0x22);
                        uint32 d = r_u16(scratch + 0x24);
                        w_u16(scratch + 0x60, a - b);
                        a = r_u16(scratch + 0x76);
                        b = r_u16(scratch + 0x78);
                        w_u16(scratch + 0x62, a - c);
                        w_u16(scratch + 0x64, b - d);
                        ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(scratch + 0x60, 8), (SVECTOR *)psx_addr(scratch + 0x68, 6));
                        uint32 copied[2];
                        for (uint32 word = 0; word < 2; ++word)
                        {
                            uint32 source = scratch + 0x94 + word * 4;
                            uint32 high_address = source + 3;
                            uint32 high_lane = high_address & 3;
                            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                            uint32 low_lane = source & 3;
                            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
                            copied[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
                        }
                        for (uint32 word = 0; word < 2; ++word)
                        {
                            uint32 destination = scratch + 0x9c + word * 4;
                            uint32 high_address = destination + 3;
                            uint32 high_lane = high_address & 3;
                            w_masked_u32(high_address & 0xfffffffc, copied[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                            uint32 low_lane = destination & 3;
                            w_masked_u32(destination & 0xfffffffc, copied[word] << (low_lane * 8), (15 << low_lane) & 15);
                        }
                        a = r_u16(scratch + 0x74);
                        b = r_u16(scratch + 0x76);
                        c = r_u16(scratch + 0x78);
                        w_u16(0x800a25a4, a);
                        w_u16(0x800a25a6, b);
                        w_u16(0x800a25a8, c);
                        uint32 callback = r_u32(context + 8);
                        w_u32(0x800a25ac, matrix);
                        hit = 1;
                        G3CollisionContact8005C1A8(context, object, triangle, 2, callback, frame);
                        a = r_u16(scratch + 0x74);
                        b = r_u16(scratch + 0x76);
                        c = r_u16(scratch + 0x78);
                        w_u16(scratch + 0x20, a);
                        w_u16(scratch + 0x22, b);
                        w_u16(scratch + 0x24, c);
                    }
                    triangle += 12;
                }
            }
            uint32 boxes = r_u32(primitives + 0x10);
            if (boxes != 0 && (r_u32(frame + 0x30) & 1) == 0)
            {
                uint32 box = r_u32(primitives + 0x14);
                uint32 box_end = box + boxes * 28;
                while (box < box_end)
                {
                    sint32 component = r_s16(box + 0x16);
                    sint32 factor = r_s16(object + 0x68);
                    uint32 product = (uint32)(component * factor);
                    component = r_s16(box + 0x18);
                    w_u16(box + 0xa, (uint32)((sint32)product >> 12));
                    factor = r_s16(object + 0x6a);
                    product = (uint32)(component * factor);
                    component = r_s16(box + 0x1a);
                    w_u16(box + 0xc, (uint32)((sint32)product >> 12));
                    factor = r_s16(object + 0x6c);
                    product = (uint32)(component * factor);
                    component = r_s16(box + 0x10);
                    w_u16(box + 0xe, (uint32)((sint32)product >> 12));
                    factor = r_s16(object + 0x68);
                    product = (uint32)(component * factor);
                    component = r_s16(box + 0x12);
                    w_u16(box + 4, (uint32)((sint32)product >> 12));
                    factor = r_s16(object + 0x6a);
                    product = (uint32)(component * factor);
                    component = r_s16(box + 0x14);
                    w_u16(box + 6, (uint32)((sint32)product >> 12));
                    factor = r_s16(object + 0x6c);
                    product = (uint32)(component * factor);
                    w_u16(box + 8, (uint32)((sint32)product >> 12));
                    a = r_u16(scratch + 0x74);
                    b = r_u16(scratch + 0x76);
                    c = r_u16(scratch + 0x78);
                    w_u32(0x8009ee10, object);
                    w_u16(scratch + 0x20, a);
                    w_u16(scratch + 0x22, b);
                    w_u16(scratch + 0x24, c);
                    if (G3Function8005E3C8(box, scratch + 0x74, scratch + 0x58, scratch + 0x94, frame) != 0)
                    {
                        a = r_u16(scratch + 0x74);
                        b = r_u16(scratch + 0x20);
                        c = r_u16(scratch + 0x22);
                        uint32 d = r_u16(scratch + 0x24);
                        w_u16(scratch + 0x60, a - b);
                        a = r_u16(scratch + 0x76);
                        b = r_u16(scratch + 0x78);
                        w_u16(scratch + 0x62, a - c);
                        w_u16(scratch + 0x64, b - d);
                        ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(scratch + 0x60, 8), (SVECTOR *)psx_addr(scratch + 0x68, 6));
                        uint32 callback = r_u32(context + 8);
                        w_u32(0x800a25ac, matrix);
                        hit = 1;
                        G3CollisionContact8005C1A8(context, object, box, 5, callback, frame);
                    }
                    box += 28;
                }
            }
            a = (uint32)(sint32)r_s16(scratch + 0x74);
            b = (uint32)(sint32)r_s16(scratch + 0x76);
            c = r_u32(scratch + 0x2c);
            uint32 old_x = r_u32(scratch + 0x28);
            w_u32(scratch + 0x2c, b - c);
            b = (uint32)(sint32)r_s16(scratch + 0x78);
            w_u32(scratch + 0x28, a - old_x);
            c = r_u32(scratch + 0x30);
            a = r_u32(scratch + 0x28);
            uint32 dz = b - c;
            w_u32(scratch + 0x30, dz);
            if (a != 0 || r_u32(scratch + 0x2c) != 0 || dz != 0)
            {
                a = r_u16(scratch + 0x28);
                b = r_u16(scratch + 0x2c);
                c = r_u16(scratch + 0x30);
                w_u16(frame + 0x10, a);
                w_u16(frame + 0x12, b);
                w_u16(frame + 0x14, c);
                a = r_u32(matrix);
                b = r_u32(matrix + 4);
                xport_gte_write_control(0, a);
                xport_gte_write_control(1, b);
                a = r_u32(matrix + 8);
                b = r_u32(matrix + 0xc);
                c = r_u32(matrix + 0x10);
                xport_gte_write_control(2, a);
                xport_gte_write_control(3, b);
                xport_gte_write_control(4, c);
                value = r_u32(frame + 0x10);
                xport_gte_write_data(0, value);
                value = r_u32(frame + 0x14);
                xport_gte_write_data(1, value);
                xport_gte_execute(0x486012);
                for (uint32 axis = 0; axis < 3; ++axis)
                    w_u32(scratch + 0x40 + axis * 4, xport_gte_read_data(25 + axis));
                uint32 current_point = r_u32(frame + 0x18);
                a = r_u16(scratch + 0x40);
                b = r_u16(current_point);
                w_u16(current_point, b + a);
                b = r_u16(current_point + 2);
                a = r_u16(scratch + 0x44);
                w_u16(current_point + 2, b + a);
                b = r_u16(current_point + 4);
                a = r_u16(scratch + 0x48);
                w_u16(current_point + 4, b + a);
            }
        }
        matrix += 0x20;
        uint32 mask_a = r_u32(frame + 0x28);
        uint32 mask_b = r_u32(frame + 0x2c);
        uint32 mask_c = r_u32(frame + 0x30);
        record = r_u32(frame + 0x1c);
        w_u32(frame + 0x28, (uint32)((sint32)mask_a >> 1));
        end = r_u32(frame + 0x20);
        record += 0x18;
        w_u32(frame + 0x2c, (uint32)((sint32)mask_b >> 1));
        w_u32(frame + 0x30, (uint32)((sint32)mask_c >> 1));
        w_u32(frame + 0x1c, record);
    }
    return hit;
}

uint32 G3Function8005CBCC(uint32 first, uint32 second, uint32 phase, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005cbcc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x98;
    uint32 scratch = 0x1f8000c8;
    uint32 packet = 0x1f800184;
    w_u32(frame + 0xa0, phase);
    w_u32(frame + 0x38, 0);
    uint32 model = r_u32(first + 0x1c);
    sint32 first_index = r_s16(first + 0xa2);
    sint32 second_index = r_s16(second + 0xa2);
    uint32 table = r_u32(model + 0xc);
    uint32 first_slot = table + ((uint32)first_index << 2);
    model = r_u32(second + 0x1c);
    table = r_u32(model + 0xc);
    uint32 geometry = r_u32(first_slot);
    uint32 flags = r_u32(first + 0x14);
    uint32 second_geometry = r_u32(table + ((uint32)second_index << 2));
    w_u32(frame + 0x34, second_geometry);
    if ((flags & 0x20000000) != 0)
    {
        uint32 value = r_u32(first + 0x144);
        w_u32(frame + 0x38, value);
    }
    flags = r_u32(second + 0x14);
    w_u32(frame + 0x3c, 0);
    if ((flags & 0x20000000) != 0)
    {
        uint32 value = r_u32(second + 0x144);
        w_u32(frame + 0x3c, value);
    }
    w_u32(frame + 0x40, 0);
    if ((flags & 0x20000000) != 0)
    {
        uint32 value = r_u32(second + 0x148);
        w_u32(frame + 0x40, value);
    }
    w_u32(frame + 0x44, 0);
    if ((flags & 0x20000000) != 0)
    {
        uint32 value = r_u32(second + 0x14c);
        w_u32(frame + 0x44, value);
    }
    uint32 matrix = r_u32(first + 0x30);
    w_u32(frame + 0x14, matrix);
    uint32 previous_matrix = r_u32(first + 0x34);
    w_u32(frame + 0x18, previous_matrix);
    sint32 count = r_s16(geometry + 6);
    uint32 record = r_u32(geometry + 0x18);
    uint32 record_end = record + (((uint32)count * 3) << 3);
    w_u32(frame + 0x20, record);
    w_u32(frame + 0x24, record_end);
    if (record >= record_end)
        return 0;
    w_u32(frame + 0x54, 0x1f800128);
    uint32 nine = 9;
    do
    {
        record = r_u32(frame + 0x20);
        uint32 first_primitives = r_u32(record + 0x14);
        if (first_primitives != 0)
        {
            uint32 sphere_count = r_u32(first_primitives + 8);
            if (sphere_count != 0 && (r_u32(frame + 0x38) & 1) == 0)
            {
                uint32 sphere = r_u32(first_primitives + 0xc);
                uint32 sphere_end = sphere + (sphere_count << 4);
                w_u32(frame + 0x30, sphere_end);
                while (sphere < sphere_end)
                {
                    uint32 kind = r_u8(sphere + 5);
                    if (kind - 6 >= 3 || (r_u32(first + 0x14) & 0x08000000) != 0)
                    {
                        matrix = r_u32(frame + 0x14);
                        G3CollisionMatrix8005CBCC(matrix, true);
                        G3CollisionVector8005CBCC(sphere + 8, 0x480012);
                        uint32 output = r_u32(frame + 0x54);
                        for (uint32 axis = 0; axis < 3; ++axis)
                            w_u32(output + axis * 4, xport_gte_read_data(25 + axis));
                        w_u32(frame + 0x10, xport_gte_read_control(31));
                        uint32 a = r_u16(scratch + 0x60);
                        uint32 b = r_u16(scratch + 0x64);
                        uint32 c = r_u16(scratch + 0x68);
                        w_u16(scratch + 0x9c, a);
                        w_u16(scratch + 0x9e, b);
                        w_u16(scratch + 0xa0, c);
                        previous_matrix = r_u32(frame + 0x18);
                        G3CollisionMatrix8005CBCC(previous_matrix, true);
                        G3CollisionVector8005CBCC(sphere + 8, 0x480012);
                        for (uint32 axis = 0; axis < 3; ++axis)
                            w_u32(output + axis * 4, xport_gte_read_data(25 + axis));
                        w_u32(frame + 0x10, xport_gte_read_control(31));
                        uint32 mask_sphere = r_u32(frame + 0x3c);
                        uint32 mask_triangle = r_u32(frame + 0x40);
                        uint32 mask_box = r_u32(frame + 0x44);
                        w_u32(frame + 0x48, mask_sphere);
                        w_u32(frame + 0x4c, mask_triangle);
                        a = r_u16(scratch + 0x60);
                        b = r_u16(scratch + 0x64);
                        c = r_u16(scratch + 0x68);
                        w_u32(frame + 0x50, mask_box);
                        w_u16(scratch + 0xac, a);
                        w_u16(scratch + 0xae, b);
                        w_u16(scratch + 0xb0, c);
                        previous_matrix = r_u32(second + 0x34);
                        uint32 second_matrix = r_u32(second + 0x30);
                        second_geometry = r_u32(frame + 0x34);
                        w_u32(frame + 0x1c, previous_matrix);
                        count = r_s16(second_geometry + 6);
                        uint32 second_record = r_u32(second_geometry + 0x18);
                        uint32 second_end = second_record + (((uint32)count * 3) << 3);
                        w_u32(frame + 0x2c, second_end);
                        w_u32(frame + 0x28, second_record);
                        while (second_record < second_end)
                        {
                            second_record = r_u32(frame + 0x28);
                            uint32 primitives = r_u32(second_record + 0x14);
                            if (primitives != 0)
                            {
                                uint32 other_count = r_u32(primitives + 8);
                                if (other_count != 0 && r_u32(frame + 0xa0) != 0 && (r_u32(frame + 0x48) & 1) == 0)
                                {
                                    uint32 other = r_u32(primitives + 0xc);
                                    uint32 other_end = other + (other_count << 4);
                                    w_u32(frame + 0x68, other_end);
                                    while (other < other_end)
                                    {
                                        if (G3CollisionKinds8005CBCC(first, second, sphere + 5, other + 5, nine, false))
                                        {
                                            G3CollisionMatrix8005CBCC(second_matrix, true);
                                            G3CollisionVector8005CBCC(other + 8, 0x480012);
                                            output = r_u32(frame + 0x54);
                                            for (uint32 axis = 0; axis < 3; ++axis)
                                                w_u32(output + axis * 4, xport_gte_read_data(25 + axis));
                                            w_u32(frame + 0x10, xport_gte_read_control(31));
                                            uint32 delta[3];
                                            a = r_u16(scratch + 0x60);
                                            sint32 center = r_s16(scratch + 0x9c);
                                            w_u16(scratch + 0xa4, a);
                                            sint32 coordinate = r_s16(scratch + 0xa4);
                                            delta[0] = (uint32)(center - coordinate);
                                            w_u32(scratch + 0x48, delta[0]);
                                            uint32 xx = delta[0] * delta[0];
                                            w_u32(frame + 0x6c, xx);
                                            a = r_u16(scratch + 0x64);
                                            center = r_s16(scratch + 0x9e);
                                            w_u16(scratch + 0xa6, a);
                                            coordinate = r_s16(scratch + 0xa6);
                                            delta[1] = (uint32)(center - coordinate);
                                            w_u32(scratch + 0x4c, delta[1]);
                                            uint32 yy = delta[1] * delta[1];
                                            a = r_u16(scratch + 0x68);
                                            center = r_s16(scratch + 0xa0);
                                            w_u16(scratch + 0xa8, a);
                                            coordinate = r_s16(scratch + 0xa8);
                                            delta[2] = (uint32)(center - coordinate);
                                            uint32 zz = delta[2] * delta[2];
                                            w_u32(scratch + 0x50, delta[2]);
                                            uint32 radius_a = r_u16(sphere + 0xe);
                                            uint32 radius_b = r_u16(other + 0xe);
                                            uint32 radius = radius_a + radius_b;
                                            xx = r_u32(frame + 0x6c);
                                            if ((sint32)(xx + yy + zz) < (sint32)(radius * radius))
                                            {
                                                uint32 sorted[3];
                                                for (uint32 axis = 0; axis < 3; ++axis)
                                                    sorted[axis] = (sint32)delta[axis] < 0 ? 0 - delta[axis] : delta[axis];
                                                if (sorted[0] > sorted[1]) { uint32 swap = sorted[0]; sorted[0] = sorted[1]; sorted[1] = swap; }
                                                if (sorted[1] > sorted[2]) { uint32 swap = sorted[1]; sorted[1] = sorted[2]; sorted[2] = swap; }
                                                if (sorted[0] > sorted[1]) { uint32 swap = sorted[0]; sorted[0] = sorted[1]; sorted[1] = swap; }
                                                uint32 weighted = sorted[2] * 30 + sorted[1] * 12 + sorted[0] * 9;
                                                if (weighted != 0)
                                                {
                                                    radius_a = r_u16(sphere + 0xe);
                                                    radius_b = r_u16(other + 0xe);
                                                    if ((sint32)((radius_a + radius_b) << 5) >= (sint32)weighted)
                                                    {
                                                        uint32 selected = radius_a < radius_b ? sphere : other;
                                                        a = r_u16(selected + 8);
                                                        uint32 selected_matrix = second_matrix;
                                                        if (radius_a < radius_b)
                                                            selected_matrix = r_u32(frame + 0x14);
                                                        w_u16(0x800a25a4, a);
                                                        b = r_u16(selected + 0xa);
                                                        w_u16(0x800a25a6, b);
                                                        c = r_u16(selected + 0xc);
                                                        w_u32(0x800a25ac, selected_matrix);
                                                        w_u16(0x800a25a8, c);
                                                        uint32 average = (uint32)((sint32)weighted >> 5);
                                                        uint32 correction[3];
                                                        for (uint32 axis = 0; axis < 3; ++axis)
                                                        {
                                                            radius_a = r_u16(sphere + 0xe);
                                                            radius_b = r_u16(other + 0xe);
                                                            uint32 component = r_u32(scratch + 0x48 + axis * 4);
                                                            uint32 product = component * (radius_a + radius_b - average);
                                                            w_u32(scratch + 0x48 + axis * 4, product);
                                                            correction[axis] = (uint32)((sint64)(sint32)(product << 5) / (sint32)weighted);
                                                        }
                                                        w_u16(scratch + 0xc0, 1);
                                                        w_u16(scratch + 0xc2, 1);
                                                        w_u16(scratch + 0xe4, correction[0]);
                                                        w_u16(scratch + 0xe6, correction[1]);
                                                        w_u16(scratch + 0xe8, correction[2]);
                                                        uint32 handler = r_u32(first + 0xec);
                                                        if (handler != 0 && r_u8(sphere + 5) >= 3)
                                                        {
                                                            uint32 game = r_u32(0x80097048);
                                                            w_u32(scratch + 0xc4, sphere);
                                                            w_u32(scratch + 0xc8, other);
                                                            w_u32(scratch + 0xcc, first);
                                                            w_u32(scratch + 0xd0, second);
                                                            w_u32(first + 0xb0, packet);
                                                            w_u32(frame + 0x5c, second_matrix);
                                                            w_u32(frame + 0x60, nine);
                                                            G3DispatchStateEvent(handler, first, game, 3, packet, frame);
                                                            nine = r_u32(frame + 0x60);
                                                            second_matrix = r_u32(frame + 0x5c);
                                                        }
                                                        handler = r_u32(second + 0xec);
                                                        if (handler != 0 && r_u8(other + 5) >= 3)
                                                        {
                                                            uint32 game = r_u32(0x80097048);
                                                            a = r_u16(scratch + 0xe4);
                                                            b = r_u16(scratch + 0xe8);
                                                            w_u32(scratch + 0xc4, other);
                                                            w_u32(scratch + 0xc8, sphere);
                                                            w_u32(scratch + 0xcc, second);
                                                            w_u32(scratch + 0xd0, first);
                                                            w_u16(scratch + 0xe4, 0 - a);
                                                            a = r_u16(scratch + 0xe6);
                                                            w_u16(scratch + 0xe8, 0 - b);
                                                            w_u16(scratch + 0xe6, 0 - a);
                                                            w_u32(second + 0xb0, packet);
                                                            w_u32(frame + 0x5c, second_matrix);
                                                            w_u32(frame + 0x60, nine);
                                                            G3DispatchStateEvent(handler, second, game, 3, packet, frame);
                                                            nine = r_u32(frame + 0x60);
                                                            second_matrix = r_u32(frame + 0x5c);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        other_end = r_u32(frame + 0x68);
                                        other += 0x10;
                                    }
                                }
                                bool local_space = false;
                                if (r_u32(primitives) != 0 && (r_u32(frame + 0x4c) & 1) == 0)
                                    local_space = true;
                                else if (r_u32(primitives + 0x10) != 0 && (r_u32(frame + 0x50) & 1) == 0)
                                    local_space = true;
                                if (local_space)
                                {
                                    w_u32(scratch + 0xec, 0);
                                    second_geometry = r_u32(frame + 0x34);
                                    a = r_u32(second_geometry + 0x10);
                                    w_u32(scratch + 0xf8, a);
                                    a = r_u32(second_geometry + 8);
                                    w_u32(scratch + 0xfc, a);
                                    w_u32(frame + 0x5c, second_matrix);
                                    w_u32(frame + 0x60, nine);
                                    G3Function800313E0(scratch, second_matrix, frame);
                                    G3CollisionMatrix8005CBCC(scratch, true);
                                    G3CollisionVector8005CBCC(scratch + 0x9c, 0x480012);
                                    for (uint32 axis = 0; axis < 3; ++axis)
                                        w_u32(scratch + 0x6c + axis * 4, xport_gte_read_data(25 + axis));
                                    w_u32(frame + 0x10, xport_gte_read_control(31));
                                    a = r_u16(scratch + 0x6c);
                                    b = r_u16(scratch + 0x70);
                                    c = r_u16(scratch + 0x74);
                                    w_u16(scratch + 0x8c, a);
                                    w_u16(scratch + 0x8e, b);
                                    w_u16(scratch + 0x90, c);
                                    a = r_u16(sphere + 0xe);
                                    previous_matrix = r_u32(frame + 0x1c);
                                    w_u16(scratch + 0x92, a);
                                    G3Function800313E0(scratch + 0x20, previous_matrix, frame);
                                    G3CollisionMatrix8005CBCC(scratch + 0x20, true);
                                    G3CollisionVector8005CBCC(scratch + 0xac, 0x480012);
                                    for (uint32 axis = 0; axis < 3; ++axis)
                                        w_u32(scratch + 0x78 + axis * 4, xport_gte_read_data(25 + axis));
                                    w_u32(frame + 0x10, xport_gte_read_control(31));
                                    a = r_u16(scratch + 0x78);
                                    b = r_u16(scratch + 0x7c);
                                    c = r_u16(scratch + 0x80);
                                    w_u16(scratch + 0x94, a);
                                    w_u16(scratch + 0x96, b);
                                    w_u16(scratch + 0x98, c);
                                    nine = r_u32(frame + 0x60);
                                    second_matrix = r_u32(frame + 0x5c);
                                }
                                uint32 triangles = r_u32(primitives);
                                if (triangles != 0 && (r_u32(frame + 0x4c) & 1) == 0)
                                {
                                    a = r_u16(scratch + 0x8c);
                                    b = r_u16(scratch + 0x8e);
                                    c = r_u16(scratch + 0x90);
                                    w_u16(scratch + 0x40, a);
                                    w_u16(scratch + 0x42, b);
                                    w_u16(scratch + 0x44, c);
                                    triangles = r_u32(primitives);
                                    uint32 triangle = r_u32(primitives + 4);
                                    uint32 triangle_end = triangle + ((triangles * 3) << 2);
                                    while (triangle < triangle_end)
                                    {
                                        w_u32(scratch + 0xec, 0);
                                        w_u32(scratch + 0xf4, triangle);
                                        w_u32(frame + 0x5c, second_matrix);
                                        w_u32(frame + 0x60, nine);
                                        uint32 result = G3Function8006340C(scratch + 0x8c, scratch + 0x94, scratch + 0xec, scratch + 0xdc, frame);
                                        second_matrix = r_u32(frame + 0x5c);
                                        nine = r_u32(frame + 0x60);
                                        if ((sint32)result > 0)
                                        {
                                            a = r_u16(scratch + 0x8c);
                                            b = r_u16(scratch + 0x40);
                                            c = r_u16(scratch + 0x42);
                                            uint32 d = r_u16(scratch + 0x44);
                                            a -= b;
                                            b = r_u16(scratch + 0x8e);
                                            w_u16(scratch + 0x84, a);
                                            a = r_u16(scratch + 0x90);
                                            w_u16(scratch + 0x86, b - c);
                                            w_u16(scratch + 0x88, a - d);
                                            G3CollisionMatrix8005CBCC(second_matrix, false);
                                            G3CollisionVector8005CBCC(scratch + 0x84, 0x486012);
                                            for (uint32 axis = 0; axis < 3; ++axis)
                                                w_u32(scratch + 0x54 + axis * 4, xport_gte_read_data(25 + axis));
                                            uint32 copied[2];
                                            for (uint32 word = 0; word < 2; ++word)
                                            {
                                                uint32 source = scratch + 0xdc + word * 4;
                                                a = r_masked_u32(source, 15);
                                                b = r_masked_u32(source, 15);
                                                copied[word] = b;
                                            }
                                            for (uint32 word = 0; word < 2; ++word)
                                            {
                                                uint32 destination = scratch + 0xd4 + word * 4;
                                                w_masked_u32(destination, copied[word], 15);
                                                w_masked_u32(destination, copied[word], 15);
                                            }
                                            a = r_u16(scratch + 0x54);
                                            b = r_u16(scratch + 0x5c);
                                            w_u16(scratch + 0xe4, 0 - a);
                                            a = r_u16(scratch + 0x58);
                                            w_u16(scratch + 0xe8, 0 - b);
                                            w_u16(scratch + 0xe6, 0 - a);
                                            uint32 handler = r_u32(second + 0xec);
                                            if (handler != 0)
                                            {
                                                uint32 game = r_u32(0x80097048);
                                                w_u16(scratch + 0xc0, 2);
                                                w_u16(scratch + 0xc2, 1);
                                                w_u32(scratch + 0xc4, triangle);
                                                w_u32(scratch + 0xc8, sphere);
                                                w_u32(scratch + 0xcc, second);
                                                w_u32(scratch + 0xd0, first);
                                                w_u32(second + 0xb0, packet);
                                                w_u32(frame + 0x5c, second_matrix);
                                                w_u32(frame + 0x60, nine);
                                                G3DispatchStateEvent(handler, second, game, 3, packet, frame);
                                                nine = r_u32(frame + 0x60);
                                                second_matrix = r_u32(frame + 0x5c);
                                            }
                                            handler = r_u32(first + 0xec);
                                            if (handler != 0 && r_u8(sphere + 5) >= 3)
                                            {
                                                uint32 game = r_u32(0x80097048);
                                                a = r_u16(scratch + 0xe4);
                                                b = r_u16(scratch + 0xe8);
                                                w_u16(scratch + 0xc0, 1);
                                                w_u16(scratch + 0xc2, 2);
                                                w_u32(scratch + 0xc4, sphere);
                                                w_u32(scratch + 0xc8, triangle);
                                                w_u32(scratch + 0xcc, first);
                                                w_u32(scratch + 0xd0, second);
                                                w_u16(scratch + 0xe4, 0 - a);
                                                a = r_u16(scratch + 0xe6);
                                                w_u16(scratch + 0xe8, 0 - b);
                                                w_u16(scratch + 0xe6, 0 - a);
                                                w_u32(first + 0xb0, packet);
                                                w_u32(frame + 0x5c, second_matrix);
                                                w_u32(frame + 0x60, nine);
                                                G3DispatchStateEvent(handler, first, game, 3, packet, frame);
                                                nine = r_u32(frame + 0x60);
                                                second_matrix = r_u32(frame + 0x5c);
                                            }
                                            a = r_u16(scratch + 0x8c);
                                            b = r_u16(scratch + 0x8e);
                                            c = r_u16(scratch + 0x90);
                                            w_u16(scratch + 0x40, a);
                                            w_u16(scratch + 0x42, b);
                                            w_u16(scratch + 0x44, c);
                                        }
                                        triangle += 0xc;
                                    }
                                }
                                uint32 boxes = r_u32(primitives + 0x10);
                                if (boxes != 0 && (r_u32(frame + 0x50) & 1) == 0)
                                {
                                    uint32 box = r_u32(primitives + 0x14);
                                    uint32 box_end = box + (((boxes << 3) - boxes) << 2);
                                    while (box < box_end)
                                    {
                                        if (G3CollisionKinds8005CBCC(first, second, sphere + 5, box + 3, nine, true))
                                        {
                                            sint32 component = r_s16(box + 0x16);
                                            sint32 scale = r_s16(second + 0x68);
                                            uint32 product = (uint32)component * (uint32)scale;
                                            component = r_s16(box + 0x18);
                                            w_u16(box + 0xa, (uint32)((sint32)product >> 12));
                                            scale = r_s16(second + 0x6a);
                                            product = (uint32)component * (uint32)scale;
                                            component = r_s16(box + 0x1a);
                                            w_u16(box + 0xc, (uint32)((sint32)product >> 12));
                                            scale = r_s16(second + 0x6c);
                                            product = (uint32)component * (uint32)scale;
                                            component = r_s16(box + 0x10);
                                            w_u16(box + 0xe, (uint32)((sint32)product >> 12));
                                            scale = r_s16(second + 0x68);
                                            product = (uint32)component * (uint32)scale;
                                            component = r_s16(box + 0x12);
                                            w_u16(box + 4, (uint32)((sint32)product >> 12));
                                            scale = r_s16(second + 0x6a);
                                            product = (uint32)component * (uint32)scale;
                                            component = r_s16(box + 0x14);
                                            w_u16(box + 6, (uint32)((sint32)product >> 12));
                                            scale = r_s16(second + 0x6c);
                                            product = (uint32)component * (uint32)scale;
                                            w_u16(box + 8, (uint32)((sint32)product >> 12));
                                            a = r_u16(scratch + 0x8c);
                                            b = r_u16(scratch + 0x8e);
                                            c = r_u16(scratch + 0x90);
                                            w_u32(0x8009ee10, second);
                                            w_u16(scratch + 0x40, a);
                                            w_u16(scratch + 0x42, b);
                                            w_u16(scratch + 0x44, c);
                                            w_u32(frame + 0x5c, second_matrix);
                                            w_u32(frame + 0x60, nine);
                                            uint32 result = G3Function8005E3C8(box, scratch + 0x8c, scratch + 0x94, scratch + 0xd4, frame);
                                            second_matrix = r_u32(frame + 0x5c);
                                            nine = r_u32(frame + 0x60);
                                            if (result != 0)
                                            {
                                                a = r_u16(scratch + 0x8c);
                                                b = r_u16(scratch + 0x40);
                                                c = r_u16(scratch + 0x42);
                                                uint32 d = r_u16(scratch + 0x44);
                                                a -= b;
                                                b = r_u16(scratch + 0x8e);
                                                w_u16(scratch + 0x84, a);
                                                a = r_u16(scratch + 0x90);
                                                w_u16(scratch + 0x86, b - c);
                                                w_u16(scratch + 0x88, a - d);
                                                G3CollisionMatrix8005CBCC(second_matrix, false);
                                                G3CollisionVector8005CBCC(scratch + 0x84, 0x486012);
                                                for (uint32 axis = 0; axis < 3; ++axis)
                                                    w_u32(scratch + 0x54 + axis * 4, xport_gte_read_data(25 + axis));
                                                a = r_u16(scratch + 0x54);
                                                b = r_u16(scratch + 0x5c);
                                                w_u16(scratch + 0xe4, 0 - a);
                                                a = r_u16(scratch + 0x58);
                                                w_u16(scratch + 0xe8, 0 - b);
                                                w_u16(scratch + 0xe6, 0 - a);
                                                uint32 handler = r_u32(second + 0xec);
                                                w_u32(0x800a25ac, second_matrix);
                                                if (handler != 0 && r_u8(box + 3) >= 3)
                                                {
                                                    uint32 game = r_u32(0x80097048);
                                                    w_u16(scratch + 0xc0, 5);
                                                    w_u16(scratch + 0xc2, 1);
                                                    w_u32(scratch + 0xc4, box);
                                                    w_u32(scratch + 0xc8, sphere);
                                                    w_u32(scratch + 0xcc, second);
                                                    w_u32(scratch + 0xd0, first);
                                                    w_u32(second + 0xb0, packet);
                                                    w_u32(frame + 0x5c, second_matrix);
                                                    w_u32(frame + 0x60, nine);
                                                    G3DispatchStateEvent(handler, second, game, 3, packet, frame);
                                                    nine = r_u32(frame + 0x60);
                                                    second_matrix = r_u32(frame + 0x5c);
                                                }
                                                handler = r_u32(first + 0xec);
                                                if (handler != 0 && r_u8(sphere + 5) >= 3)
                                                {
                                                    uint32 game = r_u32(0x80097048);
                                                    a = r_u16(scratch + 0xe4);
                                                    w_u16(scratch + 0xc2, 5);
                                                    b = r_u16(scratch + 0xe6);
                                                    w_u16(scratch + 0xc0, 1);
                                                    w_u32(scratch + 0xc4, sphere);
                                                    w_u32(scratch + 0xc8, box);
                                                    w_u32(scratch + 0xcc, first);
                                                    w_u32(scratch + 0xd0, second);
                                                    w_u16(scratch + 0xe4, 0 - a);
                                                    a = r_u16(scratch + 0xe8);
                                                    w_u16(scratch + 0xe6, 0 - b);
                                                    w_u16(scratch + 0xe8, 0 - a);
                                                    w_u32(first + 0xb0, packet);
                                                    w_u32(frame + 0x5c, second_matrix);
                                                    w_u32(frame + 0x60, nine);
                                                    G3DispatchStateEvent(handler, first, game, 3, packet, frame);
                                                    nine = r_u32(frame + 0x60);
                                                    second_matrix = r_u32(frame + 0x5c);
                                                }
                                            }
                                        }
                                        box += 0x1c;
                                    }
                                }
                            }
                            second_matrix += 0x20;
                            mask_sphere = r_u32(frame + 0x48);
                            mask_triangle = r_u32(frame + 0x4c);
                            mask_box = r_u32(frame + 0x50);
                            second_record = r_u32(frame + 0x28);
                            w_u32(frame + 0x48, (uint32)((sint32)mask_sphere >> 1));
                            previous_matrix = r_u32(frame + 0x1c);
                            w_u32(frame + 0x4c, (uint32)((sint32)mask_triangle >> 1));
                            second_end = r_u32(frame + 0x2c);
                            second_record += 0x18;
                            w_u32(frame + 0x50, (uint32)((sint32)mask_box >> 1));
                            w_u32(frame + 0x28, second_record);
                            previous_matrix += 0x20;
                            w_u32(frame + 0x1c, previous_matrix);
                        }
                    }
                    sphere_end = r_u32(frame + 0x30);
                    sphere += 0x10;
                }
            }
        }
        uint32 mask = r_u32(frame + 0x38);
        record = r_u32(frame + 0x20);
        matrix = r_u32(frame + 0x14);
        previous_matrix = r_u32(frame + 0x18);
        record += 0x18;
        matrix += 0x20;
        w_u32(frame + 0x38, (uint32)((sint32)mask >> 1));
        record_end = r_u32(frame + 0x24);
        previous_matrix += 0x20;
        w_u32(frame + 0x20, record);
        w_u32(frame + 0x14, matrix);
        w_u32(frame + 0x18, previous_matrix);
    } while (record < record_end);
    return 0;
}

uint32 G3Function80025644(uint32 effect, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80025644, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    sint32 lifetime = r_s16(effect + 0x22);
    uint32 lifetime_bits = r_u16(effect + 0x22);
    if (lifetime > 0)
    {
        w_u16(effect + 0x22, lifetime_bits - 1);
        lifetime = r_s16(effect + 0x22);
    }
    if (lifetime == 0)
        return G3Function80041578(effect, pool, frame);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        sint32 position = r_s16(effect + 0x1c + axis * 2);
        sint32 center = r_s16(effect + 0x44 + axis * 2);
        w_u32(frame + 0x10 + axis * 4, (uint32)(position - center));
    }
    uint32 length = G3Function8002FBF8(frame + 0x10, frame);
    if (length != 0)
    {
        uint32 x = r_u32(frame + 0x10);
        uint32 numerator = (0 - x) << 9;
        uint32 correction_x = (uint32)((sint64)(sint32)numerator / (sint32)length);
        uint32 y = r_u32(frame + 0x14);
        numerator = (0 - y) << 9;
        uint32 correction_y = (uint32)((sint64)(sint32)numerator / (sint32)length);
        w_u16(frame + 0x24, 0);
        w_u16(frame + 0x20, correction_x);
        w_u16(frame + 0x22, correction_y);
        uint32 velocity = r_u16(effect + 0x4c);
        w_u16(effect + 0x4c, velocity + correction_x);
        velocity = r_u16(effect + 0x4e);
        uint32 correction = r_u16(frame + 0x22);
        w_u16(effect + 0x4e, velocity + correction);
        velocity = r_u16(effect + 0x50);
        correction = r_u16(frame + 0x24);
        w_u16(effect + 0x50, velocity + correction);
    }
    uint32 velocity_x = r_u16(effect + 0x4c);
    uint32 position = r_u16(effect + 0x1c);
    uint32 velocity_y = r_u16(effect + 0x4e);
    uint32 velocity_z = r_u16(effect + 0x50);
    uint32 step_x = (uint32)((sint32)(velocity_x << 16) >> 24);
    uint32 step_y = (uint32)((sint32)(velocity_y << 16) >> 24);
    uint32 step_z = (uint32)((sint32)(velocity_z << 16) >> 24);
    w_u16(effect + 0x1c, position + step_x);
    position = r_u16(effect + 0x1e);
    sint32 updated_x = r_s16(effect + 0x1c);
    w_u16(effect + 0x1e, position + step_y);
    position = r_u16(effect + 0x20);
    sint32 center_x = r_s16(effect + 0x44);
    uint32 delta_x = (uint32)(updated_x - center_x);
    w_u16(effect + 0x20, position + step_z);
    w_u32(frame + 0x10, delta_x);
    for (uint32 axis = 1; axis < 3; ++axis)
    {
        sint32 coordinate = r_s16(effect + 0x1c + axis * 2);
        sint32 center = r_s16(effect + 0x44 + axis * 2);
        w_u32(frame + 0x10 + axis * 4, (uint32)(coordinate - center));
    }
    length = G3Function8002FBF8(frame + 0x10, frame);
    if (length != 0)
    {
        uint32 correction[3];
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 component = r_u32(frame + 0x10 + axis * 4);
            uint32 numerator = (component - (component << 4)) << 1;
            correction[axis] = (uint32)((sint64)(sint32)numerator / (sint32)length);
        }
        w_u16(frame + 0x20, correction[0]);
        w_u16(frame + 0x22, correction[1]);
        w_u16(frame + 0x24, correction[2]);
        position = r_u16(effect + 0x1c);
        w_u16(effect + 0x1c, position + correction[0]);
        position = r_u16(effect + 0x1e);
        uint32 half = r_u16(frame + 0x22);
        w_u16(effect + 0x1e, position + half);
        position = r_u16(effect + 0x20);
        half = r_u16(frame + 0x24);
        w_u16(effect + 0x20, position + half);
    }
    if ((r_u32(effect + 0xc) & 4) != 0)
        G3Function80041880(effect, pool, frame);
    uint32 game = r_u32(0x80097048);
    uint32 output = r_u32(effect + 0x14);
    uint32 transform = r_u32(game + 0x4c);
    return G3Function80041CE4(output + 0xc, transform, effect + 0x1c, 0, frame);
}

uint32 G3Function80014220(uint32 object, uint32 mask)
{
    FUNCTION_MARKER(0x80014220, "SLUS_008.06");
    if ((mask & 0x80) != 0)
        mask = (mask & 0xffffff7f) | 0x200;
    uint32 previous = r_u32(0x80096a94);
    uint32 result = previous | mask;
    w_u32(0x80096a94, result);
    return result;
}

uint32 G3Function80014254(uint32 object, uint32 mask)
{
    FUNCTION_MARKER(0x80014254, "SLUS_008.06");
    if ((mask & 0x80) != 0)
        mask = (mask & 0xffffff7f) | 0x200;
    uint32 previous = r_u32(0x80096a94);
    uint32 result = previous & ~mask;
    w_u32(0x80096a94, result);
    return result;
}

uint32 G3Function800142F4(uint32 object, uint32 flags, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800142f4, "SLUS_008.06");
    uint32 source = r_u32(0x80096a84);
    uint32 frame = guest_stack - 0x28;
    if (source == 0)
        return 0;
    if ((flags & 1) != 0 && (r_u32(0x80096a94) & 1) == 0)
    {
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 first = r_u16(source + axis * 2);
            uint32 second = r_u16(source + 0x10 + axis * 2);
            w_u16(frame + 0x10 + axis * 2, first - second);
        }
        uint32 distance = G3Function8002FA50(frame + 0x10, frame);
        uint32 upper = r_u32(0x80096998);
        if ((sint32)upper < (sint32)distance)
            distance = upper;
        else
        {
            uint32 lower = r_u32(0x80096994);
            if ((sint32)distance < (sint32)lower)
                distance = lower;
        }
        uint32 game = r_u32(0x80097048);
        w_u16(0x80096b68, distance);
        G3Function80053F98(object, game, 0x11, distance, frame);
    }
    if ((flags & 2) != 0 && (r_u32(0x80096a94) & 2) == 0)
    {
        uint32 angle = (uint32)r_s16(source + 8);
        uint32 game = r_u32(0x80097048);
        angle &= 0xfff;
        w_u16(0x80096b6a, angle);
        G3Function80053F98(object, game, 0x12, angle, frame);
    }
    if ((flags & 4) != 0)
    {
        uint32 value = (uint32)r_s16(source + 0xc);
        return G3Function80014298(object, value, frame);
    }
    return 0;
}

void G3Function80014450(uint32 source)
{
    FUNCTION_MARKER(0x80014450, "SLUS_008.06");
    w_u32(0x80096a84, source);
}

uint32 G3Function80014460(uint32 object, uint32 first, uint32 second)
{
    FUNCTION_MARKER(0x80014460, "SLUS_008.06");
    uint32 previous_first = r_u32(0x80096af0);
    if (previous_first == 0)
    {
        w_u32(0x80096af0, first);
        w_u32(0x80096af4, second);
        return 0;
    }
    if ((sint32)previous_first < (sint32)first)
        previous_first = first;
    uint32 previous_second = r_u32(0x80096af4);
    w_u32(0x80096af0, previous_first);
    uint32 result = (sint32)previous_second < (sint32)second ? 1 : 0;
    if (result != 0)
        previous_second = second;
    w_u32(0x80096af4, previous_second);
    return result;
}

uint32 G3Function800153FC(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800153fc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 destination = r_u32(command + 4);
    uint32 game = r_u32(0x80097048);
    uint32 base = r_u32(game + 0x14c);
    w_u32(frame + 0x10, command + 8);
    G3Function80015304(object, destination - base, 0, 0, frame);
    return 1;
}

uint32 G3Function8001543C(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x8001543c, "SLUS_008.06");
    uint32 reference = r_u32(command + 4);
    uint32 object = r_u32(reference + 0x20);
    if (object != 0)
    {
        uint32 flags = r_u32(object + 0x10);
        if ((flags & 0x00100000) == 0)
        {
            flags = r_u32(object + 0x14);
            if ((flags & 0x10000) == 0)
                w_u32(object + 0x14, flags | 0x1000);
        }
        object = r_u32(reference + 0x20);
        flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags | 0x02000000);
    }
    return 2;
}

uint32 G3Function800154A4(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x800154a4, "SLUS_008.06");
    uint32 reference = r_u32(command + 4);
    uint32 object = r_u32(reference + 0x20);
    if (object != 0)
    {
        uint32 flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags & 0xfdffffff);
    }
    return 2;
}

uint32 G3Function800154D4(void)
{
    FUNCTION_MARKER(0x800154d4, "SLUS_008.06");
    return 2;
}

uint32 G3Function800154DC(void)
{
    FUNCTION_MARKER(0x800154dc, "SLUS_008.06");
    return 2;
}

uint32 G3Function800154E4(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x800154e4, "SLUS_008.06");
    uint32 reference = r_u32(command + 4);
    uint32 object = r_u32(reference + 0x20);
    if (object != 0)
    {
        uint32 value = r_u8(command + 8);
        w_u16(object + 0x5e, value);
    }
    return 2;
}

uint32 G3Function80015510(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80015510, "SLUS_008.06");
    uint32 reference = r_u32(command + 4);
    uint32 object = r_u32(reference + 0x20);
    if (object != 0)
    {
        uint32 value = r_u16(command + 8);
        w_u16(object + 0xa2, value);
    }
    return 3;
}

uint32 G3Function800155D4(void)
{
    FUNCTION_MARKER(0x800155d4, "SLUS_008.06");
    return 2;
}

uint32 G3Function80015614(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80015614, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x18;
    uint32 flags = r_u32(command + 4);
    uint32 camera = r_u32(game + 0x4c);
    G3Function800142F4(camera, flags, frame);
    return 2;
}

uint32 G3Function80015644(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80015644, "SLUS_008.06");
    uint32 source = r_u32(command + 4);
    G3Function80014450(source);
    return 2;
}

uint32 G3Function80015668(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80015668, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 player = r_u32(game + 0x50);
    uint32 index = r_u32(command + 4);
    uint32 state = r_u32(player + 0x24);
    if ((sint32)index < 0)
        return 2;
    sint32 count = r_s16(state + 0xaa);
    if ((sint32)index >= count)
        return 2;
    uint32 table = r_u32(state + 0xac);
    uint32 source = r_u32(table + (index << 2));
    for (uint32 half = 0; half < 10; ++half)
    {
        uint32 value = r_u16(source + 0xc + half * 2);
        w_u16(state + 4 + half * 2, value);
    }
    for (uint32 half = 0; half < 7; ++half)
    {
        uint32 value = r_u16(source + 0x20 + half * 2);
        w_u16(state + 0x1c + half * 2, value);
    }
    for (uint32 word = 0; word < 3; ++word)
    {
        uint32 value = r_u32(source + word * 4);
        w_u32(state + 0x2c + word * 4, value);
    }
    uint32 value = r_u16(source + 0x2e);
    w_u16(state + 0xa6, value);
    value = r_u16(source + 0x30);
    w_u16(state + 0xa8, value);
    sint32 last = r_s16(state + 0x20);
    uint32 sequence = r_u32(state + 0x34);
    value = r_u16(sequence + ((uint32)last << 1) - 2);
    w_u16(state + 0xe4, value);
    return 2;
}

uint32 G3Function8001580C(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x8001580c, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 mask = r_u32(command + 4);
    uint32 camera = r_u32(game + 0x4c);
    G3Function80014220(camera, mask);
    return 2;
}

uint32 G3Function8001583C(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x8001583c, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 mask = r_u32(command + 4);
    uint32 camera = r_u32(game + 0x4c);
    G3Function80014254(camera, mask);
    return 2;
}

uint32 G3Function8001589C(void)
{
    FUNCTION_MARKER(0x8001589c, "SLUS_008.06");
    return 2;
}

uint32 G3Function800158A4(void)
{
    FUNCTION_MARKER(0x800158a4, "SLUS_008.06");
    return 2;
}

uint32 G3Function80015BA8(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80015ba8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 coordinate = r_u16(object + 0x58 + axis * 2);
        uint32 center = r_u16(command + 4 + axis * 2);
        w_u16(frame + 0x18 + axis * 2, coordinate - center);
    }
    uint32 copied[2];
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 source = frame + 0x18 + word * 4;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
        copied[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 destination = frame + 0x10 + word * 4;
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, copied[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        uint32 low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, copied[word] << (low_lane * 8), (15 << low_lane) & 15);
    }
    uint32 angle = (uint32)r_s16(command + 0xa);
    G3Function8002F8E4(frame + 0x10, angle);
    uint32 x = r_u16(frame + 0x18);
    uint32 y = r_u16(frame + 0x1a);
    uint32 z = r_u16(frame + 0x1c);
    w_u16(frame + 0x20, 0);
    w_u16(frame + 0x22, 0);
    w_u16(frame + 0x24, 0);
    x = 0 - x;
    y = 0 - y;
    z = 0 - z;
    w_u16(frame + 0x20, x);
    w_u16(frame + 0x22, y);
    w_u16(frame + 0x24, z);
    uint32 value = r_u16(command + 0xc);
    x += value;
    w_u16(frame + 0x20, x);
    value = r_u16(command + 0xe);
    uint32 rotated_y = r_u16(frame + 0x12);
    y += value;
    w_u16(frame + 0x22, y);
    uint32 destination_z = r_u16(command + 0x10);
    value = r_u16(frame + 0x10);
    y += rotated_y;
    w_u16(frame + 0x22, y);
    x += value;
    value = r_u16(frame + 0x14);
    z += destination_z;
    w_u16(frame + 0x20, x);
    w_u16(frame + 0x24, z);
    z += value;
    w_u16(frame + 0x24, z);
    angle = r_u16(command + 0xa);
    w_u16(frame + 0x26, angle);
    G3Function80015B50(object, frame + 0x20, frame);
    return 5;
}

uint32 G3Function80015CFC(void)
{
    FUNCTION_MARKER(0x80015cfc, "SLUS_008.06");
    return 2;
}

uint32 G3Function80015D04(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80015d04, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 value = r_u32(command + 4);
    uint32 level = r_u32(game + 0x48);
    uint32 shift = r_u32(game + 0x2c);
    uint32 limit = r_u32(level + 0x100);
    value = (uint32)((sint32)value >> (shift & 31));
    if ((sint32)limit < (sint32)value)
        w_u32(game + 0x28, limit);
    else
        w_u32(game + 0x28, value);
    return 2;
}

uint32 G3Function80015D40(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80015d40, "SLUS_008.06");
    uint32 shift = r_u32(command + 4);
    uint32 game = r_u32(0x80097048);
    uint32 depth = r_u32(game + 0x28);
    uint32 previous_shift = r_u32(game + 0x2c);
    G3Function8001946C(0x555 >> (shift & 31), 0x400 >> (shift & 31));
    game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    depth <<= previous_shift & 31;
    w_u32(game + 0x2c, shift);
    w_u32(level + 0x104, shift);
    level = r_u32(game + 0x48);
    depth = (uint32)((sint32)depth >> (shift & 31));
    w_u32(game + 0x28, depth);
    uint32 limit = r_u32(level + 0x100);
    if ((sint32)limit < (sint32)depth)
        w_u32(game + 0x28, limit);
    game = r_u32(0x80097048);
    level = r_u32(game + 0x48);
    G3Function8002C50C(level);
    return 2;
}

uint32 G3Function80015E30(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80015e30, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 value = r_u32(command);
    w_u32(frame + 0x14, value);
    value = r_u32(command + 4);
    w_u32(frame + 0x18, value);
    G3Function8003EAB0(frame + 0x10, frame);
    return 2;
}

uint32 G3Function80015E64(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80015e64, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 value = r_u32(command);
    w_u32(frame + 0x14, value);
    value = r_u32(command + 4);
    w_u32(frame + 0x18, value);
    G3Function8003EAB0(frame + 0x10, frame);
    return 2;
}

uint32 G3Function80015E98(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80015e98, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 value = r_u32(command);
    w_u32(frame + 0x14, value);
    value = r_u32(command + 4);
    w_u32(frame + 0x18, value);
    G3Function8003EAB0(frame + 0x10, frame);
    return 2;
}

uint32 G3Function80015ECC(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80015ecc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 value = r_u32(command);
    w_u32(frame + 0x14, value);
    value = r_u32(command + 4);
    w_u32(frame + 0x18, value);
    value = r_u32(command + 8);
    w_u32(frame + 0x1c, value);
    G3Function8003EAB0(frame + 0x10, frame);
    return 3;
}

uint32 G3Function80015F0C(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80015f0c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 value = r_u32(command);
    w_u32(frame + 0x14, value);
    value = r_u32(command + 4);
    w_u32(frame + 0x18, value);
    value = r_u32(command + 8);
    w_u32(frame + 0x1c, value);
    G3Function8003EAB0(frame + 0x10, frame);
    return 3;
}

uint32 G3Function80016174(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80016174, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x18;
    uint32 level = r_u32(game + 0x48);
    uint32 count = r_u32(level + 0x7c);
    uint32 record = r_u32(level + 0x80);
    uint32 end = record + count * 0x34;
    while (record < end)
    {
        uint32 flags = r_u32(record + 0x18);
        w_u32(record + 0x18, flags | 0x100);
        record += 0x34;
    }
    uint32 player = r_u32(0x8009efc4);
    uint32 mode = r_u32(player + 0x150);
    if (mode != 7)
    {
        if (mode == 6)
        {
            game = r_u32(0x80097048);
            uint32 camera = r_u32(game + 0x4c);
            G3Function80051FE4(camera, frame);
        }
        player = r_u32(0x8009efc4);
        mode = r_u32(player + 0x150);
        w_u32(0x8009d34c, mode);
    }
    player = r_u32(0x8009efc4);
    w_u32(player + 0x150, 7);
    return 1;
}

uint32 G3Function80016248(void)
{
    FUNCTION_MARKER(0x80016248, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    uint32 count = r_u32(level + 0x7c);
    uint32 record = r_u32(level + 0x80);
    uint32 end = record + count * 0x34;
    while (record < end)
    {
        uint32 flags = r_u32(record + 0x18);
        uint32 object = r_u32(record + 0x20);
        w_u32(record + 0x18, flags & 0xfffffeff);
        if (object != 0)
        {
            flags = r_u32(object + 0x10) & 0xff7fffff;
            w_u32(object + 0x10, flags);
            if ((flags & 0x00100000) == 0)
            {
                flags = r_u32(object + 0x14);
                if ((flags & 0x10000) == 0)
                    w_u32(object + 0x14, flags | 0x1000);
            }
        }
        record += 0x34;
    }
    uint32 player = r_u32(0x8009efc4);
    uint32 mode = r_u32(0x8009d34c);
    w_u32(player + 0x150, mode);
    return 1;
}

uint32 G3Function80016310(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80016310, "SLUS_008.06");
    uint32 placement = r_u32(command + 4);
    uint32 flags = r_u32(placement + 0x18);
    w_u32(placement + 0x18, flags & 0xffffffef);
    return 2;
}

uint32 G3Function80016330(uint32 source_object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80016330, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    uint32 placement = r_u32(command + 4);
    if (placement == 0)
        return 2;
    uint32 flags = r_u32(placement + 0x18);
    if ((flags & 0x18) != 0)
        return 2;
    w_u32(placement + 0x18, flags & 0xffffff7f);
    uint32 object = G3Function80029938(placement, frame);
    if (object == 0 || source_object == 0)
        return 2;
    uint32 source = source_object + 0x58;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
    uint32 low_lane = source & 3;
    uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
    uint32 packed = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    uint32 last = (uint32)r_s16(source_object + 0x5c);
    uint32 destination = object + 0x58;
    high_address = destination + 3;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, packed >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    low_lane = destination & 3;
    w_masked_u32(destination & 0xfffffffc, packed << (low_lane * 8), (15 << low_lane) & 15);
    w_u16(object + 0x5c, last);
    uint32 copied[2];
    for (uint32 word = 0; word < 2; ++word)
    {
        source = source_object + 0x60 + word * 4;
        high_address = source + 3;
        high_lane = high_address & 3;
        high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        low_lane = source & 3;
        low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
        copied[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 word = 0; word < 2; ++word)
    {
        destination = object + 0x60 + word * 4;
        high_address = destination + 3;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, copied[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, copied[word] << (low_lane * 8), (15 << low_lane) & 15);
    }
    uint32 reference = r_u32(source_object + 0x20);
    if (reference != 0)
    {
        uint32 a = r_u16(placement + 0x10);
        uint32 b = r_u16(reference + 0x10);
        w_u16(frame + 0x10, a - b);
        reference = r_u32(source_object + 0x20);
        a = r_u16(placement + 0x12);
        b = r_u16(reference + 0x12);
        w_u16(frame + 0x12, a - b);
        reference = r_u32(source_object + 0x20);
        a = r_u16(placement + 0x14);
        b = r_u16(reference + 0x14);
        w_u16(frame + 0x14, a - b);
    }
    else
    {
        source = placement + 0x10;
        high_address = source + 3;
        high_lane = high_address & 3;
        high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        low_lane = source & 3;
        low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
        packed = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        last = (uint32)r_s16(placement + 0x14);
        destination = frame + 0x10;
        high_address = destination + 3;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, packed >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, packed << (low_lane * 8), (15 << low_lane) & 15);
        w_u16(frame + 0x14, last);
    }
    G3Function8005BD00(object + 0x60, frame + 0x20);
    sint32 x = r_s16(frame + 0x10);
    sint32 coefficient = r_s16(frame + 0x20);
    sint32 products[9];
    products[0] = x * coefficient;
    sint32 y = r_s16(frame + 0x12);
    coefficient = r_s16(frame + 0x22);
    products[1] = y * coefficient;
    sint32 z = r_s16(frame + 0x14);
    coefficient = r_s16(frame + 0x24);
    products[2] = z * coefficient;
    for (uint32 row = 1; row < 3; ++row)
    {
        coefficient = r_s16(frame + 0x20 + row * 6);
        products[row * 3] = x * coefficient;
        coefficient = r_s16(frame + 0x22 + row * 6);
        products[row * 3 + 1] = y * coefficient;
        coefficient = r_s16(frame + 0x24 + row * 6);
        products[row * 3 + 2] = z * coefficient;
    }
    uint32 delta[3];
    for (uint32 row = 0; row < 3; ++row)
        delta[row] = (uint32)(products[row * 3] >> 12) + (uint32)(products[row * 3 + 1] >> 12) + (uint32)(products[row * 3 + 2] >> 12);
    w_u16(frame + 0x18, delta[0]);
    w_u16(frame + 0x1a, delta[1]);
    w_u16(frame + 0x1c, delta[2]);
    uint32 value = r_u16(object + 0x58);
    w_u16(object + 0x58, value + delta[0]);
    value = r_u16(object + 0x5a);
    uint32 half = r_u16(frame + 0x1a);
    w_u16(object + 0x5a, value + half);
    value = r_u16(object + 0x5c);
    half = r_u16(frame + 0x1c);
    w_u16(object + 0x5c, value + half);
    return 2;
}

uint32 G3Function80016574(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80016574, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 placement = r_u32(command + 4);
    if (placement != 0 && (r_u32(placement + 0x18) & 0x18) == 0)
        G3Function80029938(placement, frame);
    return 2;
}

// Native pose copy shared by the two audited spawn paths
static void G3CopySpawnPose800165B8(uint32 source_object, uint32 object)
{
    uint32 source = source_object + 0x58;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
    uint32 low_lane = source & 3;
    uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
    uint32 packed = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    uint32 z = (uint32)r_s16(source_object + 0x5c);
    uint32 destination = object + 0x58;
    high_address = destination + 3;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, packed >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    low_lane = destination & 3;
    w_masked_u32(destination & 0xfffffffc, packed << (low_lane * 8), (15 << low_lane) & 15);
    w_u16(object + 0x5c, z);
    uint32 copied[2];
    for (uint32 word = 0; word < 2; ++word)
    {
        source = source_object + 0x60 + word * 4;
        high_address = source + 3;
        high_lane = high_address & 3;
        high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        low_lane = source & 3;
        low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
        copied[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 word = 0; word < 2; ++word)
    {
        destination = object + 0x60 + word * 4;
        high_address = destination + 3;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, copied[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, copied[word] << (low_lane * 8), (15 << low_lane) & 15);
    }
}

uint32 G3Function800165B8(uint32 source_object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800165b8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    uint32 parent = source_object;
    if (r_s32(source_object + 0x38) < 0)
        parent = 0;
    sint32 count = r_s16(command + 8);
    uint32 step = count == 0 ? uint32(0xffffffff) : (uint32)(0x1000 / count);
    sint32 signed_radius = r_s16(command + 0xa);
    uint32 angle = (uint32)r_s16(parent + 0x64);
    uint32 radius = signed_radius < 0 ? 0 - (uint32)signed_radius : (uint32)signed_radius;
    bool scatter = signed_radius < 0;
    w_u32(frame + 0x20, step);
    if (count <= 0)
        return 3;
    uint32 index = 0;
    do
    {
        uint32 resource = r_u32(command + 4);
        if (scatter)
        {
            uint32 sample = G3SdkRandom80027FA8();
            uint32 direction = (uint32)((sint32)sample % 0x1000);
            uint32 value = G3Function8005B470(direction);
            w_u16(frame + 0x10, value >> 6);
            value = G3Function8005B590(direction);
            w_u16(frame + 0x12, value >> 6);
            sample = G3SdkRandom80027FA8();
            w_u16(frame + 0x14, (sample & 0x3f) << 1);
            w_u16(frame + 0x18, 0);
            w_u16(frame + 0x1a, 0);
            w_u16(frame + 0x1c, 0xfffffff7);
        }
        uint32 object = G3Function8002A454(parent, resource, frame);
        if (object != 0)
        {
            if (parent == 0)
                G3CopySpawnPose800165B8(source_object, object);
            if (scatter)
            {
                uint32 handler = r_u32(object + 0xec);
                if (handler != 0)
                {
                    uint32 game = r_u32(0x80097048);
                    G3DispatchStateEvent(handler, object, game, 0x7d9, frame + 0x10, frame);
                }
            }
            else
            {
                uint32 cosine = G3Function8005B470(angle);
                uint32 product = radius * cosine;
                uint32 coordinate = r_u16(object + 0x58);
                w_u16(object + 0x58, coordinate + (uint32)((sint32)product >> 12));
                uint32 sine = G3Function8005B590(angle);
                product = radius * sine;
                coordinate = r_u16(object + 0x5a);
                w_u16(object + 0x5a, coordinate + (uint32)((sint32)product >> 12));
            }
        }
        ++index;
        sint32 signed_index = (sint32)(index << 16) >> 16;
        step = r_u32(frame + 0x20);
        count = r_s16(command + 8);
        angle += step;
        if (signed_index >= count)
            break;
    } while (true);
    return 3;
}

uint32 G3Function80016820(void)
{
    FUNCTION_MARKER(0x80016820, "SLUS_008.06");
    return 5;
}

uint32 G3Function80016828(void)
{
    FUNCTION_MARKER(0x80016828, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    uint32 count = r_u32(level + 0x20);
    uint32 record = r_u32(level + 0x24);
    uint32 end = record + ((count * 3) << 3);
    while (record < end)
    {
        uint32 flags = r_u16(record);
        w_u16(record, flags | 1);
        record += 0x18;
    }
    return 1;
}

uint32 G3Function80016888(void)
{
    FUNCTION_MARKER(0x80016888, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    uint32 count = r_u32(level + 0x20);
    uint32 record = r_u32(level + 0x24);
    uint32 end = record + ((count * 3) << 3);
    while (record < end)
    {
        uint32 flags = r_u16(record);
        w_u16(record, flags & 0xfffffffe);
        record += 0x18;
    }
    return 1;
}

uint32 G3Function8001698C(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x8001698c, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 value = r_u16(command + 4);
    uint32 level = r_u32(game + 0x48);
    w_u16(level + 0x4e, value);
    level = r_u32(game + 0x48);
    G3Function8002C50C(level);
    return 2;
}
