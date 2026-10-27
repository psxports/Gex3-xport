#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void G3FunPlatformUpdate(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000003a0, "G3L_22");
    uint32 frame = guest_stack - 0xf0;
    uint32 state = object + 0x104;
    G3FillMemory80086094(frame + 0x18, 0, 8);
    uint32 position_path = G3Function800833EC(object);
    uint32 rotation_path = G3Function80083424(object, frame);
    bool absolute_position = false;
    if (object != 0)
    {
        uint32 intro = r_u32(object + 0x20);
        if (intro != 0)
            absolute_position = r_u32(intro + 0x24) != 0;
    }
    if (r_s16(state + 4) > 0 && r_u32(state + 0x18) == 0 && r_u32(state + 0x1c) == 0 && r_u32(state + 0xc) == 0)
    {
        sint32 random = (sint32)G3SdkRandom80027FA8();
        uint32 angle = (uint32)(sint32)(sint16)(random % 4096);
        uint32 sine = G3Function8005B470(angle);
        uint32 amplitude = (uint32)r_s16(state + 4);
        uint32 product = sine * amplitude;
        if ((sint32)product < 0)
            product += 0xfff;
        uint32 x = (uint32)((sint32)product >> 12);
        uint32 cosine = G3Function8005B590(angle);
        amplitude = (uint32)r_s16(state + 4);
        product = cosine * amplitude;
        if ((sint32)product < 0)
            product += 0xfff;
        uint32 y = (uint32)((sint32)product >> 12);
        uint32 bound = amplitude << 1;
        sint32 speed = r_s32(state + 0x10);
        if ((sint32)(0 - bound) < speed && speed < (sint32)bound)
        {
            w_u32(state + 0x18, (uint32)((sint32)(x << 16) >> 11));
            amplitude = (uint32)r_s16(state + 4);
            w_u32(state + 0x1c, (uint32)((sint32)(y << 16) >> 11));
            if ((sint32)amplitude < 0)
                amplitude += 127;
            uint32 impulse = (uint32)((sint32)amplitude >> 7) + 1;
            w_u32(state + 0xc, 0 - (impulse << 12));
        }
    }
    if (position_path != 0 || rotation_path != 0)
    {
        uint32 flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags | 0x400);
    }
    if (position_path != 0)
    {
        if (absolute_position)
        {
            w_u32(frame + 0x10, 0);
            G3FunPlatformMovePath(object, position_path, state + 0x40, object + 0x58, frame);
        }
        else
        {
            w_u32(frame + 0x10, 0);
            G3FunPlatformMovePath(object, position_path, state + 0x40, frame + 0xc8, frame);
            uint32 intro = r_u32(object + 0x20);
            uint32 x = r_u16(frame + 0xc8);
            uint32 base_x = r_u16(intro + 0x10);
            intro = r_u32(object + 0x20);
            w_u16(object + 0x58, base_x + x);
            uint32 base_y = r_u16(intro + 0x12);
            uint32 y = r_u16(frame + 0xca);
            w_u16(object + 0x5a, base_y + y);
            uint32 base_z = r_u16(intro + 0x14);
            uint32 z = r_u16(frame + 0xcc);
            w_u16(object + 0x5c, base_z + z);
        }
    }
    else
        w_u16(object + 0x5c, r_u16(state + 0xa));
    if (rotation_path != 0)
    {
        w_u32(frame + 0x10, 1);
        G3FunPlatformMovePath(object, rotation_path, state + 0x44, frame + 0x20, frame);
        w_u16(frame + 0x1c, r_u16(frame + 0x24));
    }
    uint32 angle_x = (uint32)r_s16(state + 0x38);
    uint32 old_vx = r_u32(state + 0x20);
    uint32 angle_y = (uint32)r_s16(state + 0x3a);
    uint32 acceleration_x = (((0 - angle_x) << 5) - (uint32)((sint32)old_vx >> 6)) << 1;
    uint32 old_vy = r_u32(state + 0x24);
    uint32 acceleration_y = (((0 - angle_y) << 5) - (uint32)((sint32)old_vy >> 6)) << 1;
    if (r_u32(state + 0x30) != 0)
        G3Function8003C614(object, game, frame);
    uint32 force_x = r_u32(state + 0x18);
    old_vx = r_u32(state + 0x20);
    uint32 limit = (uint32)r_s16(state + 2);
    acceleration_x += force_x;
    uint32 force_y = r_u32(state + 0x1c);
    w_u32(state + 0x20, old_vx + acceleration_x);
    old_vy = r_u32(state + 0x24);
    uint32 vx = r_u32(state + 0x20);
    acceleration_y += force_y;
    w_u32(state + 0x24, old_vy + acceleration_y);
    uint32 positive = limit << 12;
    uint32 negative = (0 - limit) << 12;
    if ((sint32)positive < (sint32)vx)
        w_u32(state + 0x20, positive);
    else if ((sint32)vx < (sint32)negative)
        w_u32(state + 0x20, negative);
    limit = (uint32)r_s16(state + 2);
    uint32 vy = r_u32(state + 0x24);
    positive = limit << 12;
    negative = (0 - limit) << 12;
    if ((sint32)positive < (sint32)vy)
        w_u32(state + 0x24, positive);
    else if ((sint32)vy < (sint32)negative)
        w_u32(state + 0x24, negative);
    uint32 old_x = r_u32(state + 0x28);
    vx = r_u32(state + 0x20);
    uint32 old_y = r_u32(state + 0x2c);
    vy = r_u32(state + 0x24);
    uint32 new_x = old_x + vx;
    limit = (uint32)r_s16(state);
    uint32 new_y = old_y + vy;
    w_u32(state + 0x28, new_x);
    w_u32(state + 0x2c, new_y);
    positive = limit << 12;
    negative = (0 - limit) << 12;
    if ((sint32)positive < (sint32)new_x)
        w_u32(state + 0x28, positive);
    else if ((sint32)new_x < (sint32)negative)
        w_u32(state + 0x28, negative);
    limit = (uint32)r_s16(state);
    new_y = r_u32(state + 0x2c);
    positive = limit << 12;
    negative = (0 - limit) << 12;
    if ((sint32)positive < (sint32)new_y)
        w_u32(state + 0x2c, positive);
    else if ((sint32)new_y < (sint32)negative)
        w_u32(state + 0x2c, negative);
    new_x = r_u32(state + 0x28);
    new_y = r_u32(state + 0x2c);
    w_u16(state + 0x3c, 0);
    w_u16(state + 0x38, (uint32)((sint32)new_x >> 12));
    w_u16(state + 0x3a, (uint32)((sint32)new_y >> 12));
    G3Function8005BD00(state + 0x38, frame + 0x28);
    G3Function8005BD00(frame + 0x18, frame + 0x48);
    uint32 intro = r_u32(object + 0x20);
    G3Function8005BD00(intro + 8, frame + 0x88);
    MulMatrix0((MATRIX *)psx_addr(frame + 0x48, 20), (MATRIX *)psx_addr(frame + 0x28, 20), (MATRIX *)psx_addr(frame + 0x68, 20));
    MulMatrix0((MATRIX *)psx_addr(frame + 0x88, 20), (MATRIX *)psx_addr(frame + 0x68, 20), (MATRIX *)psx_addr(frame + 0xa8, 20));
    G3Function800202A4(frame + 0xa8, object + 0x60, 0x15, frame);
    uint32 offset = r_u32(state + 0x14);
    uint32 velocity = r_u32(state + 0x10);
    uint32 acceleration = r_u32(state + 0xc);
    uint32 damping = (uint32)((sint32)(0 - offset) >> 4) - (uint32)((sint32)velocity >> 6);
    acceleration += damping;
    offset = r_u32(state + 0x14);
    velocity += acceleration;
    w_u32(state + 0x10, velocity);
    offset += velocity;
    w_u32(state + 0x14, offset);
    uint32 z = r_u16(object + 0x5c);
    w_u16(object + 0x5c, z + (uint32)((sint32)offset >> 12));
    uint32 contact = r_u32(state + 0x30);
    w_u32(state + 0x30, 0);
    w_u32(state + 0x18, 0);
    w_u32(state + 0x1c, 0);
    w_u32(state + 0xc, 0);
    w_u32(state + 0x34, contact);
}

void G3FunPlatformCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000850, "G3L_22");
    uint32 frame = guest_stack - 0x60;
    uint32 contact = r_u32(object + 0xb0);
    uint32 state = object + 0x104;
    if (r_s16(contact + 6) != 4)
        return;
    uint32 other = r_u32(contact + 0x14);
    uint32 player = r_u32(game + 0x50);
    if (other != player)
        return;
    uint32 matrix = r_u32(object + 0x30);
    if (matrix == 0)
        return;
    G3Function800313E0(frame + 0x20, matrix, frame);
    uint32 first = r_u32(frame + 0x20);
    uint32 second = r_u32(frame + 0x24);
    xport_gte_write_control(0, first);
    xport_gte_write_control(1, second);
    uint32 third = r_u32(frame + 0x28);
    uint32 fourth = r_u32(frame + 0x2c);
    uint32 fifth = r_u32(frame + 0x30);
    xport_gte_write_control(2, third);
    xport_gte_write_control(3, fourth);
    xport_gte_write_control(4, fifth);
    uint32 tx = r_u32(frame + 0x34);
    uint32 ty = r_u32(frame + 0x38);
    xport_gte_write_control(5, tx);
    uint32 tz = r_u32(frame + 0x3c);
    xport_gte_write_control(6, ty);
    xport_gte_write_control(7, tz);
    // Uses corrected MAC output without a pad store
    RotTrans((SVECTOR *)psx_addr(contact + 0x18, 8), (VECTOR *)psx_addr(frame + 0x10, 12), (sint32 *)psx_addr(frame + 0x40, 4));
    uint32 y = r_u32(frame + 0x14);
    w_u32(state + 0x18, (0 - y) << 6);
    uint32 x = r_u32(frame + 0x10);
    uint32 previous = r_u32(state + 0x34);
    w_u32(state + 0x1c, x << 6);
    w_u32(state + 0xc, previous != 0 ? 0xfffff000 : 0xffffc000);
    other = r_u32(contact + 0x14);
    w_u32(state + 0x30, other);
    G3Function8003C864(object, game, frame);
}

uint32 G3EffectGeneratorEvent(uint32 object, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_24");
    uint32 frame = guest_stack - 0x18;
    uint32 index = message - 1;
    if (index >= 9)
        return 0;
    uint32 target = r_u32(module_base + 0x9d0 + index * 4);
    switch (target - module_base)
    {
    case 0x3c:
        G3EffectGeneratorInitialize(object, module_base, frame);
        return 1;
    case 0x4c:
        G3EffectGeneratorUpdate(object, module_base, frame);
        return 1;
    case 0x5c:
        G3EffectGeneratorShutdown(object, frame);
        return 1;
    case 0x6c:
        G3EffectGeneratorTrigger(object, payload);
        return 1;
    case 0x7c:
        return 0;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved effect generator event target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}

void G3EffectGeneratorShutdown(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000094, "G3L_24");
    uint32 frame = guest_stack - 0x18;
    uint32 state = object + 0x128;
    uint32 handle = r_u32(state + 0xc);
    if (handle != 0)
    {
        G3Function8003F18C(handle);
        w_u32(state + 0xc, 0);
    }
}

void G3EffectGeneratorInitialize(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000d0, "G3L_24");
    uint32 frame = guest_stack - 0x20;
    uint32 data = r_u32(object + 0x28);
    uint32 state = object + 0x128;
    if (data == 0)
        w_u32(object + 0x28, module_base + 0x9f4);
    data = r_u32(object + 0x28);
    uint32 resource = G3Function8003AA48(data + 0xc);
    w_u32(object + 0x128, resource);
    w_u32(state + 0xc, 0);
}

uint32 G3EffectGeneratorOscillate(uint32 effect, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000128, "G3L_24");
    uint32 frame = guest_stack - 0x20;
    uint32 angle = r_u16(effect + 0x46);
    uint32 step = r_u16(effect + 0x44);
    angle = (angle + step) & 0xfff;
    w_u16(effect + 0x46, angle);
    uint32 sine = G3Function8005B470(angle);
    uint32 amplitude = (uint32)r_s16(effect + 0x48);
    uint32 product = sine * amplitude;
    uint32 z = r_u16(effect + 0x20);
    w_u16(effect + 0x20, z + (uint32)((sint32)product >> 12));
    return G3Function80041DE0(effect, pool, frame);
}

void G3EffectGeneratorEmit(uint32 object, uint32 resource, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000198, "G3L_24");
    uint32 frame = guest_stack - 0x90;
    uint32 object_resource = r_u32(object + 0x1c);
    uint32 data = r_u32(object + 0x28);
    uint32 model_index = r_u16(object_resource + 8) - 1;
    uint32 models = r_u32(object_resource + 0xc);
    uint32 model_slot = models + (uint32)((sint32)(model_index << 16) >> 14);
    uint32 owner = r_u32(resource + 0xc);
    uint32 model = r_u32(model_slot);
    uint32 pool = r_u32(owner);
    uint32 weight_a = G3SdkRandom80027FA8() & 0xfff;
    uint32 random_b = G3SdkRandom80027FA8();
    uint32 geometry = r_u32(model + 0x18);
    uint32 faces = r_u32(geometry + 0x14);
    uint32 weight_b = random_b & 0xfff;
    if (faces == 0)
        return;
    uint32 random_face = G3SdkRandom80027FA8();
    sint32 face_count = r_s32(faces);
    sint32 dividend = (sint32)random_face;
    uint32 face_index;
    if (face_count == 0)
        face_index = random_face;
    else if (dividend == (-2147483647 - 1) && face_count == -1)
        face_index = 0;
    else
        face_index = (uint32)(dividend % face_count);
    uint32 vertices = r_u32(model + 8);
    uint32 face_rows = r_u32(faces + 4);
    uint32 face = face_rows + face_index * 12;
    uint32 index_a = (uint32)r_s16(face);
    uint32 index_b = (uint32)r_s16(face + 2);
    uint32 index_c = (uint32)r_s16(face + 4);
    uint32 vertex_a = vertices + (index_a << 3);
    uint32 vertex_b = vertices + (index_b << 3);
    uint32 matrix = r_u32(object + 0x30);
    uint32 vertex_c = vertices + (index_c << 3);
    if (matrix == 0)
        return;
    if (weight_a + weight_b > 4096)
    {
        weight_a = 4096 - weight_a;
        weight_b = 4096 - weight_b;
    }
    uint32 first = r_u16(vertex_b);
    uint32 second = r_u16(vertex_a);
    uint32 edge_ab_x = first - second;
    w_u16(frame + 0x30, edge_ab_x);
    first = r_u16(vertex_b + 2);
    second = r_u16(vertex_a + 2);
    uint32 weighted_ab_x = (uint32)(sint32)(sint16)edge_ab_x * weight_a;
    uint32 edge_ab_y = first - second;
    w_u16(frame + 0x32, edge_ab_y);
    first = r_u16(vertex_b + 4);
    second = r_u16(vertex_a + 4);
    uint32 edge_ab_z = first - second;
    w_u16(frame + 0x34, edge_ab_z);
    first = r_u16(vertex_c);
    second = r_u16(vertex_a);
    uint32 edge_ac_x = first - second;
    uint32 weighted_ac_x = (uint32)(sint32)(sint16)edge_ac_x * weight_b;
    uint32 weighted_ab_y = (uint32)(sint32)(sint16)edge_ab_y * weight_a;
    w_u16(frame + 0x38, edge_ac_x);
    first = r_u16(vertex_c + 2);
    second = r_u16(vertex_a + 2);
    uint32 edge_ac_y = first - second;
    uint32 weighted_ac_y = (uint32)(sint32)(sint16)edge_ac_y * weight_b;
    uint32 weighted_ab_z = (uint32)(sint32)(sint16)edge_ab_z * weight_a;
    w_u16(frame + 0x3a, edge_ac_y);
    first = r_u16(vertex_a + 4);
    second = r_u16(vertex_c + 4);
    uint32 edge_ac_z = second - first;
    w_u16(frame + 0x3c, edge_ac_z);
    uint32 base_x = r_u16(vertex_a);
    uint32 weighted_ac_z = (uint32)(sint32)(sint16)edge_ac_z * weight_b;
    w_u16(frame + 0x40, base_x + (uint32)((sint32)weighted_ab_x >> 12) + (uint32)((sint32)weighted_ac_x >> 12));
    uint32 base_y = r_u16(vertex_a + 2);
    w_u16(frame + 0x42, base_y + (uint32)((sint32)weighted_ab_y >> 12) + (uint32)((sint32)weighted_ac_y >> 12));
    uint32 base_z = r_u16(vertex_a + 4);
    w_u16(frame + 0x44, base_z + (uint32)((sint32)weighted_ab_z >> 12) + (uint32)((sint32)weighted_ac_z >> 12));
    matrix = r_u32(object + 0x30);
    uint32 rotation0 = r_u32(matrix);
    uint32 rotation1 = r_u32(matrix + 4);
    xport_gte_write_control(0, rotation0);
    xport_gte_write_control(1, rotation1);
    uint32 rotation2 = r_u32(matrix + 8);
    uint32 rotation3 = r_u32(matrix + 0xc);
    uint32 rotation4 = r_u32(matrix + 0x10);
    xport_gte_write_control(2, rotation2);
    xport_gte_write_control(3, rotation3);
    xport_gte_write_control(4, rotation4);
    uint32 translation_x = r_u32(matrix + 0x14);
    uint32 translation_y = r_u32(matrix + 0x18);
    xport_gte_write_control(5, translation_x);
    uint32 translation_z = r_u32(matrix + 0x1c);
    xport_gte_write_control(6, translation_y);
    xport_gte_write_control(7, translation_z);
    // Uses corrected MAC output without a pad store
    RotTrans((SVECTOR *)psx_addr(frame + 0x40, 8), (VECTOR *)psx_addr(frame + 0x50, 12), (sint32 *)psx_addr(frame + 0x60, 4));
    uint32 ab_y = (uint32)r_s16(frame + 0x32);
    uint32 ac_z = (uint32)r_s16(frame + 0x3c);
    uint32 cross_x_first = ab_y * ac_z;
    uint32 ab_z = (uint32)r_s16(frame + 0x34);
    uint32 ac_y = (uint32)r_s16(frame + 0x3a);
    uint32 cross_x_second = ab_z * ac_y;
    uint32 ab_x = (uint32)r_s16(frame + 0x30);
    uint32 cross_y_first = ab_x * ac_z;
    uint32 ac_x = (uint32)r_s16(frame + 0x38);
    uint32 cross_y_second = ab_z * ac_x;
    uint32 cross_z_first = ab_x * ac_y;
    uint32 world_y = r_u16(frame + 0x54);
    uint32 world_z = r_u16(frame + 0x58);
    w_u16(frame + 0x42, world_y);
    w_u16(frame + 0x44, world_z);
    uint32 normal_x = (uint32)((sint32)(cross_x_first - cross_x_second) >> 12);
    uint32 normal_y = 0 - (uint32)((sint32)(cross_y_first - cross_y_second) >> 12);
    uint32 cross_z_second = ab_y * ac_x;
    uint32 world_x = r_u16(frame + 0x50);
    w_u16(frame + 0x28, normal_x);
    w_u16(frame + 0x2a, normal_y);
    w_u16(frame + 0x40, world_x);
    sint32 abs_x = (sint16)normal_x;
    if (abs_x < 0)
        abs_x = -abs_x;
    sint32 abs_y = (sint16)normal_y;
    if (abs_y < 0)
        abs_y = -abs_y;
    uint32 normal_z = (uint32)((sint32)(cross_z_first - cross_z_second) >> 12);
    sint32 abs_z = (sint16)normal_z;
    if (abs_z < 0)
        abs_z = -abs_z;
    w_u16(frame + 0x2c, normal_z);
    if (abs_x > abs_y)
    {
        sint32 swap = abs_x;
        abs_x = abs_y;
        abs_y = swap;
    }
    if (abs_y > abs_z)
    {
        sint32 swap = abs_y;
        abs_y = abs_z;
        abs_z = swap;
    }
    if (abs_x > abs_y)
    {
        sint32 swap = abs_x;
        abs_x = abs_y;
        abs_y = swap;
    }
    sint32 magnitude = (30 * abs_z + 12 * abs_y + 9 * abs_x) >> 5;
    for (uint32 component = 0; component != 3; ++component)
    {
        sint32 normal = r_s16(frame + 0x28 + component * 2);
        sint32 speed = r_s16(data + 2);
        sint32 product = normal * speed;
        uint32 scaled = magnitude == 0 ? (product < 0 ? 1 : 0xffffffff) : (uint32)(product / magnitude);
        w_u16(frame + 0x28 + component * 2, scaled);
    }
    matrix = r_u32(object + 0x30);
    ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(frame + 0x28, 8), (SVECTOR *)psx_addr(frame + 0x48, 6));
    uint32 effect;
    if (r_u8(data) == 0)
    {
        w_u32(frame + 0x18, 0x80044bc4);
        uint32 effect_storage = r_u32(0x800a27fc);
        w_u32(frame + 0x10, 0);
        w_u32(frame + 0x1c, 0x80041de0);
        w_u32(frame + 0x14, effect_storage);
        w_u32(frame + 0x20, (uint32)r_s16(data + 8));
        uint32 effect_data = r_u32(pool + 0x14);
        effect = G3Function8004257C(pool, effect_data, frame + 0x40, frame + 0x48, frame);
    }
    else
    {
        w_u32(frame + 0x18, 0x80044bc4);
        uint32 effect_storage = r_u32(0x800a27fc);
        w_u32(frame + 0x10, 0);
        w_u32(frame + 0x1c, module_base + 0x128);
        w_u32(frame + 0x14, effect_storage);
        w_u32(frame + 0x20, (uint32)r_s16(data + 8));
        uint32 effect_data = r_u32(pool + 0x14);
        effect = G3Function8004257C(pool, effect_data, frame + 0x40, frame + 0x48, frame);
        if (effect != 0)
        {
            w_u16(effect + 0x44, r_u16(data + 4));
            w_u16(effect + 0x48, r_u16(data + 0xa));
        }
    }
    uint32 colors = r_u32(pool + 0x1c);
    if (colors != 0 && effect != 0)
    {
        uint32 flags = r_u32(effect + 0xc);
        uint32 color = r_u32(effect + 8);
        w_u32(effect + 0xc, flags | 4);
        uint32 red = r_u32(color);
        uint32 green = r_u32(color + 4);
        uint32 blue = r_u32(color + 8);
        uint32 alpha = r_u32(color + 0xc);
        w_u32(effect + 0x60, red);
        w_u32(effect + 0x64, green);
        w_u32(effect + 0x68, blue);
        w_u32(effect + 0x6c, alpha);
        colors = r_u32(pool + 0x1c);
        w_u16(effect + 0x74, 0);
        w_u32(effect + 0x70, colors + 4);
    }
}

void G3EffectGeneratorUpdate(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000750, "G3L_24");
    uint32 frame = guest_stack - 0x38;
    uint32 data = r_u32(object + 0x28);
    uint32 mode = r_u8(data);
    uint32 state = object + 0x128;
    if (mode >= 2 && r_s16(state + 0x1c) == 0)
        return;
    w_u16(state + 0x1c, 0);
    sint32 rate = r_s16(data + 6);
    sint32 increment = (rate * 4096) / 30;
    if (rate < 30)
    {
        sint32 random = (sint32)G3SdkRandom80027FA8();
        if (random % 4096 < increment)
        {
            uint32 resource = r_u32(object + 0x128);
            if (resource != 0)
                G3EffectGeneratorEmit(object, resource, module_base, frame);
        }
    }
    else
    {
        uint32 phase = r_u32(state + 4);
        uint32 old_phase = r_u32(state + 4);
        uint32 next_phase = phase + (uint32)increment;
        w_u32(state + 8, next_phase);
        sint32 count = (sint32)next_phase >> 12;
        count -= (sint32)old_phase >> 12;
        for (sint32 emitted = 0; emitted < count; ++emitted)
        {
            uint32 resource = r_u32(state);
            if (resource != 0)
                G3EffectGeneratorEmit(object, resource, module_base, frame);
        }
        w_u32(state + 4, r_u32(state + 8));
    }
    if ((r_u8(data + 1) & 0x80) == 0)
        return;
    uint32 handle = r_u32(state + 0xc);
    if (handle != 0)
    {
        if (G3Function8003F0A8(handle) == 0)
        {
            uint32 sound = r_u16(state + 0x14);
            uint32 pitch = (uint32)r_s16(state + 0x18);
            uint32 volume = (uint32)r_s16(state + 0x16);
            w_u32(frame + 0x10, (uint32)r_s16(state + 0x1a));
            handle = G3Function8003E518(object, sound, pitch, volume, frame);
            w_u32(state + 0xc, handle);
        }
        else
        {
            handle = r_u32(state + 0xc);
            if (G3Function8003F088(handle) != 0)
            {
                uint32 parameter = (uint32)r_s16(state + 0x1a);
                uint32 pitch = (uint32)r_s16(state + 0x18);
                uint32 volume = (uint32)r_s16(state + 0x16);
                w_u32(frame + 0x10, parameter);
                handle = r_u32(state + 0xc);
                if (G3Function8003E7E4(object, handle, pitch, volume, frame) == 0)
                {
                    handle = r_u32(state + 0xc);
                    G3Function8003F18C(handle);
                    w_u32(state + 0xc, 0);
                }
            }
        }
    }
    else
    {
        sint32 timer = r_s16(state + 0x12);
        uint32 next_timer = r_u16(state + 0x12) - 1;
        if (timer != 0)
            w_u16(state + 0x12, next_timer);
        else
        {
            uint32 intro = r_u32(object + 0x20);
            if ((r_u32(intro + 0x18) & 0x80) != 0)
                return;
            uint32 sound = r_u16(state + 0x14);
            uint32 pitch = (uint32)r_s16(state + 0x18);
            uint32 volume = (uint32)r_s16(state + 0x16);
            w_u32(frame + 0x10, (uint32)r_s16(state + 0x1a));
            handle = G3Function8003E518(object, sound, pitch, volume, frame);
            w_u32(state + 0xc, handle);
            if (handle == 0)
                w_u16(state + 0x12, r_u16(state + 0x10));
        }
    }
}

void G3EffectGeneratorTrigger(uint32 object, uint32 payload)
{
    FUNCTION_MARKER(0x000009b8, "G3L_24");
    uint32 state = object + 0x128;
    if (payload == 0)
        w_u16(state + 0x1c, 1);
}

uint32 G3GatgunEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_02");
    uint32 frame = guest_stack - 0x18;
    if (object == 0)
        return 1;
    uint32 resource = r_u32(object + 0x1c);
    uint32 state = object + 0x128;
    uint32 controls = r_u32(resource + 0x1c);
    if (controls == 0)
        return 1;
    switch (message)
    {
    case 1:
        G3GatgunInitialize(object, state);
        return 1;
    case 2:
        G3GatgunUpdate(object, game, state, controls, module_base, frame);
        return 1;
    case 3:
        G3GatgunCollide(object, game);
        return 1;
    default:
        return 0;
    }
}

void G3GatgunInitialize(uint32 object, uint32 state)
{
    FUNCTION_MARKER(0x000000a4, "G3L_02");
    w_u32(object + 0x154, 0);
    w_u16(state + 0x26, 0);
    uint32 flags = r_u32(object + 0x14);
    w_u32(object + 0x14, flags | 8);
}

uint32 G3GatgunMoveToward(uint32 output, uint32 first, uint32 second, uint32 step)
{
    FUNCTION_MARKER(0x000000c0, "G3L_02");
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

uint32 G3GatgunTurnToward(uint32 output, uint32 first, uint32 second, uint32 step)
{
    FUNCTION_MARKER(0x00000180, "G3L_02");
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

void G3GatgunUpdateRotation(uint32 object, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000240, "G3L_02");
    uint32 frame = guest_stack - 0x40;
    w_u32(frame + 0x20, 0x1000);
    w_u32(frame + 0x18, 0x1000);
    w_u32(frame + 0x1c, 0);
    w_u32(frame + 0x14, 0);
    uint32 pitch = (uint32)r_s16(state + 0x1c);
    w_u32(frame + 0x10, 0x1000);
    G3Function8005B94C(pitch, frame + 0x10, frame);
    uint32 owner = r_u32(object + 0x20);
    uint32 yaw = (uint32)r_s16(state + 0x20);
    uint32 base_yaw = (uint32)r_s16(owner + 0xc);
    G3Function8005BB6C(yaw + base_yaw, frame + 0x10, frame);
    G3Function800202A4(frame + 0x10, object + 0x60, 0x15, frame);
}

void G3GatgunReadControls(uint32 game, uint32 state, uint32 controls)
{
    FUNCTION_MARKER(0x000002b8, "G3L_02");
    sint32 pitch_delta = r_s32(game + 0x80);
    sint32 yaw_delta = r_s32(game + 0x7c);
    uint32 pitch = r_u16(state + 0x1c);
    w_u16(state + 0x1c, pitch - (uint32)(pitch_delta / 8));
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
    w_u16(state + 0x20, (uint32)yaw_limit);
}

void G3GatgunShoot(uint32 object, uint32 game, uint32 controls, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000370, "G3L_02");
    uint32 frame = guest_stack - 0x80;
    w_u16(frame + 0x58, 0);
    uint32 range = r_u16(controls + 0x22);
    w_u16(frame + 0x5c, 0);
    w_u16(frame + 0x5a, 0 - range);
    {
        uint32 source = frame + 0x58;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 lane = source & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
        uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
        uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        uint32 z = (uint32)r_s16(frame + 0x5c);
        uint32 destination = frame + 0x50;
        high_address = destination + 3;
        high_lane = high_address & 3;
        lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
        w_u16(frame + 0x54, z);
    }
    uint32 matrix = r_u32(object + 0x30);
    if (matrix != 0)
    {
        ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(controls + 0x1c, 8), (SVECTOR *)psx_addr(frame + 0x60, 6));
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 local = r_u16(frame + 0x60 + axis * 2);
            uint32 origin = r_u16(object + 0x58 + axis * 2);
            w_u16(frame + 0x48 + axis * 2, local + origin);
        }
        matrix = r_u32(object + 0x30);
        ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(frame + 0x50, 8), (SVECTOR *)psx_addr(frame + 0x60, 6));
        uint32 x = r_u16(frame + 0x60);
        uint32 origin_x = r_u16(frame + 0x48);
        uint32 origin_y = r_u16(frame + 0x4a);
        uint32 origin_z = r_u16(frame + 0x4c);
        w_u16(frame + 0x40, x + origin_x);
        uint32 y = r_u16(frame + 0x62);
        uint32 z = r_u16(frame + 0x64);
        w_u16(frame + 0x42, y + origin_y);
        w_u16(frame + 0x44, z + origin_z);
    }
    w_u32(frame + 0x34, 0x89);
    w_u32(frame + 0x24, frame + 0x40);
    w_u32(frame + 0x38, object);
    w_u32(frame + 0x28, frame + 0x48);
    G3Function80061CA4(frame + 0x10, game, frame);
    uint32 hit = r_u32(frame + 0x10);
    if (hit - 1 < 2 || hit == 5)
    {
        uint32 target_object = r_u32(frame + 0x18);
        if (target_object != 0)
        {
            uint32 callback = r_u32(target_object + 0xec);
            if (callback != 0)
                G3DispatchStateEvent(callback, target_object, game, 0x7e4, 0, frame);
        }
    }
    if (r_u32(frame + 0x10) != 0)
    {
        uint32 resource = G3Function8003AA48(module_base + 0xbe0);
        uint32 impact_resource = r_u32(0x8009880c);
        if (resource == 0)
            resource = r_u32(0x80098804);
        if (impact_resource != 0 && resource != 0)
        {
            uint32 data = r_u32(impact_resource + 0xc);
            uint32 pool = r_u32(0x800a27fc);
            uint32 effect = r_u32(data);
            G3Function800444E4(frame + 0x40, effect, pool, 0x1e, frame);
            data = r_u32(resource + 0xc);
            pool = r_u32(0x800a27fc);
            effect = r_u32(data);
            uint32 created = G3Function800444E4(frame + 0x40, effect, pool, 0xffffffff, frame);
            if (created != 0)
            {
                uint32 flags = r_u32(created + 0xc);
                uint32 linked = r_u32(created + 4);
                w_u32(created + 0xc, flags | 0x8000);
                if (linked != 0)
                {
                    flags = r_u32(linked + 0xc);
                    w_u32(linked + 0xc, flags | 0x8000);
                }
            }
        }
    }
}

void G3GatgunCollide(uint32 object, uint32 game)
{
    FUNCTION_MARKER(0x00000ba8, "G3L_02");
    uint32 collision = r_u32(object + 0xb0);
    uint32 other = r_u32(collision + 0x14);
    uint32 player = r_u32(game + 0x50);
    if (other == player && r_u32(object + 0x154) == 0)
        w_u32(object + 0x154, 1);
}

void G3GatgunUpdate(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000058c, "G3L_02");
    uint32 frame = guest_stack - 0x80;
    uint32 camera = r_u32(game + 0x4c);
    G3FillMemory80086094(frame + 0x18, 0, 8);
    uint32 player = r_u32(0x8009efc4);
    uint32 mode = r_u32(object + 0x154);
    uint32 player_data = r_u32(player + 0x24);
    if (mode == 1)
    {
        w_u16(player_data + 0x120, 3);
        G3Function80013CDC(camera, 8, frame);
        w_u32(object + 0x154, 2);
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
        uint32 yaw = r_u16(object + 0x64);
        w_u16(player_data + 0xc6, yaw);
        w_u16(player + 0x64, yaw);
        uint32 matrix = r_u32(object + 0x30);
        if (matrix != 0)
        {
            ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(controls + 0x10, 8), (SVECTOR *)psx_addr(frame + 0x20, 6));
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
            if (callback != 0)
                G3DispatchStateEvent(callback, player, game, 0x7e2, frame + 0x28, frame);
            matrix = r_u32(object + 0x30);
            ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(controls, 8), (SVECTOR *)psx_addr(frame + 0x20, 6));
            x = r_u16(frame + 0x20);
            object_x = r_u16(object + 0x58);
            w_u16(state, x + object_x);
            y = r_u16(frame + 0x22);
            object_y = r_u16(object + 0x5a);
            w_u16(state + 2, y + object_y);
            z = r_u16(frame + 0x24);
            object_z = r_u16(object + 0x5c);
            player = r_u32(0x8009efc4);
            w_u16(state + 4, z + object_z);
            w_u32(player + 0x10c, 0);
        }
    }
    else if (mode == 2)
    {
        w_u16(player_data + 0x120, 3);
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
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 rotation = r_u16(object + 0x60 + axis * 2);
            uint32 offset = r_u16(controls + 6 + axis * 2);
            w_u16(camera + 8 + axis * 2, rotation + offset);
        }
        w_u32(object + 0x154, 3);
        w_u16(state + 0x28, 0);
    }
    else if (mode == 3)
    {
        uint32 flags = r_u32(player + 0x10);
        uint32 buttons = (flags & 0x100) == 0 ? game + 0x70 : 0x800a0d28;
        player = r_u32(0x8009efc4);
        w_u16(player_data + 0x120, 3);
        flags = r_u32(player + 0x10);
        w_u32(player + 0x10, flags | 0x100);
        G3GatgunReadControls(game, state, controls);
        G3GatgunUpdateRotation(object, state, frame);
        if ((r_u32(buttons + 4) & 0x80) != 0)
        {
            w_u32(object + 0x154, 4);
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
        }
        if ((r_u32(buttons) & 0x20) != 0)
        {
            sint32 timer = r_s16(state + 0x26);
            uint32 timer_half = r_u16(state + 0x26);
            if (timer > 0)
                w_u16(state + 0x26, timer_half - 1);
            else
            {
                uint32 interval = r_u16(controls + 0x24);
                w_u16(state + 0x26, interval);
                G3GatgunShoot(object, game, controls, module_base, frame);
                G3Function8002A1C8(object, 0xffffffff, frame);
                if ((r_u32(object + 0x14) & 0x10) != 0)
                    w_u16(object + 0x6e, 1);
            }
            if (r_s16(state + 0x28) == 0)
            {
                w_u32(frame + 0x10, 0xffffffff);
                w_u32(frame + 0x14, 1);
                G3Function8008194C(0, 1, 0x26a1, 0x999, frame);
                w_u16(state + 0x28, 1);
            }
        }
        else
        {
            w_u16(object + 0x5e, 0);
            w_u16(object + 0x6e, 0);
            if (r_s16(state + 0x28) != 0)
            {
                G3Function80081B7C(0, 0x26a1, frame);
                w_u16(state + 0x28, 0);
            }
        }
        G3FillMemory80086094(frame + 0x28, 0, 8);
        uint32 x = r_u16(controls);
        w_u16(frame + 0x50, x);
        uint32 y = r_u16(controls + 2);
        w_u16(frame + 0x52, 0 - y);
        uint32 z = r_u16(controls + 4);
        w_u16(frame + 0x54, z);
        uint32 camera_yaw = (uint32)r_s16(state + 0x24);
        uint32 target_yaw = (uint32)r_s16(state + 0x20);
        sint32 difference = (sint16)G3Function80038B48(camera_yaw, target_yaw);
        if (difference < 0)
            difference = -difference;
        sint32 speed = r_s16(controls + 0x26);
        uint32 speed_half = r_u16(controls + 0x26);
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
                speed_half = r_u16(controls + 0x26);
                w_u16(state + 0x24, target_half - speed_half);
            }
        }
        uint32 yaw = r_u16(state + 0x24);
        uint32 owner = r_u32(object + 0x20);
        uint32 offset = r_u16(controls + 0xa);
        uint32 base_yaw = r_u16(owner + 0xc);
        w_u16(frame + 0x2c, base_yaw + yaw + offset);
        G3Function8005BD00(frame + 0x28, frame + 0x30);
        ApplyMatrixSV((MATRIX *)psx_addr(frame + 0x30, 20), (SVECTOR *)psx_addr(frame + 0x50, 8), (SVECTOR *)psx_addr(frame + 0x20, 6));
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 local = r_u16(frame + 0x20 + axis * 2);
            uint32 origin = r_u16(object + 0x58 + axis * 2);
            w_u16(camera + axis * 2, local + origin);
        }
        uint32 pitch = r_u16(controls + 6);
        w_u16(camera + 0xa, 0);
        w_u16(camera + 8, pitch);
        yaw = r_u16(state + 0x24);
        owner = r_u32(object + 0x20);
        offset = r_u16(controls + 0xa);
        base_yaw = r_u16(owner + 0xc);
        w_u16(camera + 0xc, base_yaw + yaw + offset);
    }
    else if (mode == 4)
    {
        w_u16(object + 0x6e, 0);
        w_u16(object + 0x5e, 0);
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 value = r_u16(state + 0xc + axis * 2);
            w_u16(camera + 8 + axis * 2, value);
        }
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 value = r_u16(state + 6 + axis * 2);
            w_u16(camera + axis * 2, value);
        }
        w_u32(object + 0x154, 0);
        G3Function80013C54(camera, frame);
        uint32 flags = r_u32(object + 0x10);
        player = r_u32(0x8009efc4);
        w_u32(object + 0x10, flags & 0xfffffbff);
        flags = r_u32(player + 0x10);
        w_u32(player + 0x10, flags & 0xfffffeff);
        G3Function80081B7C(0, 0x26a1, frame);
    }
}

uint32 G3GatorEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_73");
    uint32 frame = guest_stack - 0x20;
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
    case 1: G3GatorInitialize(object, game, state, frame); return 1;
    case 0x7ea: G3GatorUpdate(object, game, state, settings, module_base, frame); return 1;
    case 3: G3GatorCollide(object, game, state, settings, module_base, frame); return 1;
    case 0x11:
        w_u32(frame + 0x10, payload);
        G3GatorReverseCollision(object, game, state, payload, frame);
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
        if (difference < 5) return 1;
        return r_u8(state + 0x2c) != 0 ? 1 : 0;
    }
    default: return 0;
    }
}

void G3GatorInitialize(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000194, "G3L_73");
    uint32 frame = guest_stack - 0x30;
    w_u32(object + 0x10, r_u32(object + 0x10) | 0x120000);
    G3Function80082988(object, 0, game, frame);
    w_u16(state, 0);
    w_u16(state + 0xe, 0);
    w_u16(state + 0x18, 0);
    w_u16(state + 0x1a, 0);
    w_u16(state + 0x1c, 0);
    uint32 component = r_u16(object + 0x58);
    w_u16(frame + 0x18, component);
    w_u16(frame + 0x10, component);
    component = r_u16(object + 0x5a);
    w_u16(frame + 0x1a, component);
    w_u16(frame + 0x12, component);
    w_u16(frame + 0x14, r_u16(object + 0x5c) - 0x100);
    component = r_u16(object + 0x5c);
    uint32 current_game = r_u32(0x80097048);
    w_u16(frame + 0x1c, component + 0x100);
    uint32 terrain = r_u32(current_game + 0x48);
    terrain = r_u32(terrain);
    G3Function800622EC(terrain, frame + 0x10, frame + 0x18, object, frame);
    w_u16(state + 6, 0);
    w_u8(state + 0x2f, 0);
    w_u8(state + 0x30, 0);
}

uint32 G3GatorSteer(uint32 input, uint32 object, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000025c, "G3L_73");
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

void G3GatorSpeed(uint32 direction, uint32 state, uint32 settings)
{
    FUNCTION_MARKER(0x000003ac, "G3L_73");
    if (r_s16(state + 2) != 0) return;
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

void G3GatorAnimate(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000438, "G3L_73");
    uint32 frame = guest_stack - 0x38;
    sint32 speed = r_s16(state + 0xa);
    uint32 player = r_u32(game + 0x50);
    if (speed != 0)
    {
        uint32 descriptor = r_u32(object + 0x1c);
        if (r_u32(descriptor + 0x10) != 0)
        {
            G3Function80029F0C(object, 0xffffffff, frame);
            if ((r_u32(object + 0x14) & 0x10) != 0)
            {
                uint32 animations = r_u32(settings);
                w_u16(object + 0x6e, r_u8(animations + 2));
                sint32 phase = r_s16(state + 2);
                w_u16(player + 0x6e, phase == 0 || phase == 4 || phase == 5 ? 0x31 : 0x33);
                w_u8(state + 0x30, 0);
            }
        }
        w_u16(player + 0x5e, r_u16(object + 0x5e));
        if (r_u16(state + 2) - (uint32)4 >= 2 && r_u8(state + 0x30) == 0)
        {
            uint32 animation_offset = 7;
            if (r_u32(object + 0xf8) != 0)
            {
                sint32 floor = r_s16(object + 0x100);
                sint32 z = r_s16(object + 0x5c);
                if (floor + 0x20 >= z) animation_offset = 2;
            }
            uint32 animations = r_u32(settings);
            w_u16(object + 0x6e, r_u8(animations + animation_offset));
        }
        sint32 phase = r_s16(state + 2);
        if (phase != 0 && phase != 4 && phase != 5)
        {
            uint32 animations = r_u32(settings);
            w_u16(object + 0x6e, r_u8(animations + 3));
            w_u16(player + 0x6e, 0x33);
        }
        {
            uint32 source = module_base + 0x1c50;
            uint32 destination = frame + 0x18;
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
        if (r_u8(state + 0x2c) != 0)
        {
            if (r_u32(game + 0x14c) % 5 == 0) G3GatorSplash800458F8(object + 0x58, frame);
            sint32 random = (sint32)G3SdkRandom80027FA8();
            w_u16(frame + 0x10, (uint32)(random % 20 - 10));
            random = (sint32)G3SdkRandom80027FA8();
            w_u16(frame + 0x12, (uint32)(random % 20 - 10));
            random = (sint32)G3SdkRandom80027FA8();
            w_u16(frame + 0x14, (uint32)(random % 10 + 20));
            uint32 z = (uint32)r_s16(object + 0x5c);
            G3Function800453A0(object + 0x58, frame + 0x10, frame + 0x18, z, frame);
        }
    }
    else if (r_u8(settings + 0x25) != 0)
    {
        uint32 animations = r_u32(settings);
        uint32 animation = r_u8(animations);
        w_u16(object + 0x5e, 0);
        w_u16(object + 0x6e, animation);
        w_u16(player + 0x5e, 0);
    }
    else if (r_u8(settings + 8) != 0)
    {
        uint32 animations = r_u32(settings);
        uint32 current = r_u16(object + 0x6e);
        uint32 idle = r_u8(animations + 7);
        if (current != idle)
        {
            uint32 descriptor = r_u32(object + 0x1c);
            if (r_u32(descriptor + 0x10) != 0)
            {
                G3Function80029F0C(object, 0xffffffff, frame);
                if ((r_u32(object + 0x14) & 0x10) != 0)
                {
                    animations = r_u32(settings);
                    w_u16(object + 0x6e, r_u8(animations));
                    w_u16(player + 0x6e, 0x29);
                    w_u8(state + 0x30, 0);
                }
            }
            w_u16(player + 0x5e, r_u16(object + 0x5e));
        }
    }
}

void G3GatorBank(uint32 object, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000794, "G3L_73");
    uint32 frame = guest_stack - 0x68;
    uint32 direction = (uint32)r_s16(state + 8);
    uint32 target = 0;
    if (direction != 0)
    {
        target = 0 - G3Function80038B48((uint32)r_s16(object + 0x64), direction);
        uint32 animations = r_u32(settings);
        uint32 current = r_u16(object + 0x6e);
        uint32 idle = r_u8(animations + 7);
        if (current == idle) target = 0;
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

void G3GatorSlope(uint32 object, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000008cc, "G3L_73");
    uint32 frame = guest_stack - 0x50;
    uint32 game = r_u32(0x80097048);
    {
        uint32 source = module_base + 0x1c58;
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
        if (r_s16(normal + 4) < 0xd99) normal = frame + 0x30;
    }
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

void G3GatorMove(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000bf4, "G3L_73");
    uint32 frame = guest_stack - 0x58;
    uint32 direction = G3GatorSteer(game, object, state, settings, frame);
    G3GatorSpeed((uint32)(sint16)direction, state, settings);
    if (r_u8(state + 0x2d) != 0)
    {
        G3FillMemory80086094(frame + 0x10, 0, 8);
        uint32 table = r_u32(settings + 0x1c);
        sint32 speed = r_s16(state + 0xa);
        sint32 threshold = r_s16(table);
        uint32 index = 0;
        if (speed >= threshold)
        {
            do
            {
                uint32 next = index + 1;
                uint32 offset = (uint32)((sint32)(next << 16) >> 15);
                threshold = r_s16(table + offset);
                index = next;
            } while (speed >= threshold);
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
    }
    else if (r_s16(state + 2) == 5)
    {
        uint32 position = r_u16(object + 0x58);
        uint32 delta = r_u16(state + 0x32);
        w_u16(object + 0x58, position + delta);
        position = r_u16(object + 0x5a);
        delta = r_u16(state + 0x34);
        w_u16(object + 0x5a, position + delta);
    }
    G3GatorSlope(object, state, settings, module_base, frame);
    G3GatorAnimate(object, game, state, settings, module_base, frame);
}

void G3GatorPath(uint32 object, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000dbc, "G3L_73");
    uint32 frame = guest_stack - 0x28;
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x10, 0);
    uint32 paths = G3Function8008345C(object, frame + 0x10, frame + 0x14);
    uint32 linked = r_u32(frame + 0x10);
    uint32 model = r_u32(frame + 0x14);
    uint32 path_state = G3Function8008354C(object, linked, model);
    uint32 fraction = r_u32(frame + 0x10);
    if ((sint32)G3Function80082F30(object, paths, path_state, 0x10, fraction, frame) > 0) w_u16(state, 3);
}

void G3GatorFlags(uint32 object, uint32 game, uint32 state)
{
    FUNCTION_MARKER(0x00000e38, "G3L_73");
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
    uint32 grounded = 0;
    if (r_u32(object + 0xf8) != 0)
    {
        sint32 floor = r_s16(object + 0x100);
        sint32 z = r_s16(object + 0x5c);
        if (floor + 0x20 >= z) grounded = 1;
    }
    w_u8(state + 0x2e, grounded);
    if (r_s16(state + 2) == 5) w_u8(state + 0x2d, 0);
}

void G3GatorKnockback(uint32 object, uint32 game, uint32 state, uint32 settings)
{
    FUNCTION_MARKER(0x00000efc, "G3L_73");
    if (r_u8(state + 0x2f) != 0)
    {
        uint32 player = r_u32(game + 0x50);
        w_u16(player + 0x6e, 0x2a);
        uint32 animations = r_u32(settings);
        uint32 animation = r_u8(animations + 9);
        w_u16(object + 0x5e, 0);
        w_u16(object + 0x6e, animation);
        w_u16(state + 2, 5);
        w_u8(state + 0x2f, 0);
    }
}

void G3GatorAttack(uint32 object, uint32 game, uint32 state, uint32 settings)
{
    FUNCTION_MARKER(0x00000f40, "G3L_73");
    uint32 player = r_u32(0x8009efc4);
    uint32 input = (r_u32(player + 0x10) & 0x100) == 0 ? game + 0x70 : 0x800a0d28;
    if (r_s16(state + 2) == 0 && (r_u32(input + 4) & 0x20) != 0)
    {
        player = r_u32(game + 0x50);
        w_u16(player + 0x6e, 0x32);
        uint32 animations = r_u32(settings);
        uint32 animation = r_u8(animations + 8);
        w_u16(object + 0x5e, 0);
        w_u16(object + 0x6e, animation);
        w_u16(state + 2, 4);
    }
}

void G3GatorVertical(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000fc0, "G3L_73");
    uint32 frame = guest_stack - 0x20;
    uint32 z = r_u16(object + 0x5c);
    uint32 velocity = r_u16(state + 0xe);
    w_u16(object + 0x5c, z + velocity);
    uint32 gravity = r_u16(settings + 0xc);
    velocity = r_u16(state + 0xe);
    uint32 phase = (uint32)r_s16(state + 2);
    w_u8(state + 0x2c, 0);
    w_u16(state + 0xe, velocity + gravity);
    switch (phase)
    {
    case 0:
    case 4:
    case 5:
    {
        if (phase != 0 && (r_u32(object + 0x14) & 0x10) != 0) w_u16(state + 2, 0);
        uint32 player = r_u32(0x8009efc4);
        sint32 player_z = r_s16(player + 0x5c);
        sint32 player_floor = r_s16(player + 0xe0);
        if (player_z - 0x40 < player_floor && player_floor < player_z + 0x100)
        {
            sint32 object_z = r_s16(object + 0x5c);
            sint32 difference = object_z - player_floor;
            if (difference < 0) difference = -difference;
            if (difference >= 5)
            {
                player = r_u32(0x8009efc4);
                uint32 floor_bits = r_u16(player + 0xe0);
                w_u16(object + 0x5c, floor_bits);
                w_u16(player + 0x5c, floor_bits);
            }
            w_u16(state + 0xe, 0);
        }
        else
        {
            if (r_u32(object + 0xf8) == 0) goto water_tail;
            sint32 object_z = r_s16(object + 0x5c);
            sint32 floor = r_s16(object + 0x100);
            uint32 floor_bits = r_u16(object + 0x100);
            if (floor < object_z)
            {
                player = r_u32(0x8009efc4);
                w_u16(object + 0x5c, floor_bits);
                w_u16(player + 0x5c, floor_bits);
                w_u16(state + 0xe, 0);
            }
            w_u8(state + 0x2c, 1);
        }
        if (r_u32(object + 0xf8) != 0)
        {
            sint32 object_z = r_s16(object + 0x5c);
            sint32 floor = r_s16(object + 0x100);
            if (object_z < floor)
            {
                w_u16(state + 0xe, 0);
                uint32 floor_bits = r_u16(object + 0x100);
                player = r_u32(0x8009efc4);
                w_u16(object + 0x5c, floor_bits);
                w_u16(player + 0x5c, floor_bits);
                w_u8(state + 0x2c, 1);
            }
        }
        break;
    }
    case 1:
        w_u16(state + 2, 2);
        goto gravity_tail;
    case 2:
    {
        uint32 player = r_u32(0x8009efc4);
        sint32 player_z = r_s16(player + 0x5c);
        sint32 player_floor = r_s16(player + 0xe0);
        if (player_z - 0x40 < player_floor && player_floor < player_z + 0x100)
        {
            w_u16(state + 0xe, 0);
            w_u16(state + 2, 3);
        }
    }
    case 3:
    {
        sint32 object_z = r_s16(object + 0x5c);
        sint32 floor = r_s16(object + 0x100);
        uint32 floor_bits = r_u16(object + 0x100);
        if (floor < object_z)
        {
            uint32 player = r_u32(0x8009efc4);
            w_u16(object + 0x5c, floor_bits + 8);
            w_u16(player + 0x5c, floor_bits + 8);
            w_u16(state + 0xe, 0);
            w_u16(state + 2, 0);
        }
        goto gravity_tail;
    }
    default: break;
    }
    goto water_tail;
gravity_tail:
    z = r_u16(object + 0x5c);
    velocity = r_u16(state + 0xe);
    w_u16(object + 0x5c, z + velocity);
    velocity = r_u16(state + 0xe);
    gravity = r_u16(settings + 0xc);
    w_u16(state + 0xe, velocity + gravity);
water_tail:
    if (r_u8(state + 0x2c) != 0)
    {
        uint32 clock = r_u32(game + 0x14c);
        uint32 wave = G3Function8005B590(clock * 0x33);
        z = r_u16(object + 0x5c);
        w_u16(object + 0x5c, z + (uint32)((sint32)wave >> 9));
        clock = r_u32(game + 0x14c);
        wave = G3Function8005B590(clock * 0x4f);
        z = r_u16(object + 0x5c);
        uint32 player = r_u32(0x8009efc4);
        z += (uint32)((sint32)wave >> 8);
        w_u16(object + 0x5c, z);
        w_u16(player + 0x5c, z);
    }
    (void)frame;
}

void G3GatorRide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001280, "G3L_73");
    uint32 frame = guest_stack - 0x30;
    uint32 player = r_u32(game + 0x50);
    uint32 paths = G3Function8008345C(object, 0, 0);
    uint32 input = (r_u32(player + 0x10) & 0x100) == 0 ? game + 0x70 : 0x800a0d28;
    uint32 model = r_u32(player + 0x24);
    if (paths != 0)
    {
        uint32 descriptor = r_u32(object + 0x1c);
        if (r_u32(descriptor + 0x10) != 0)
        {
            G3Function80029F0C(object, 0xffffffff, frame);
            if ((r_u32(object + 0x14) & 0x10) != 0)
            {
                uint32 animations = r_u32(settings);
                w_u16(object + 0x6e, r_u8(animations + 2));
                w_u16(player + 0x6e, 0x31);
                w_u16(player + 0x5e, 0);
            }
        }
        G3GatorPath(object, state, frame);
    }
    else
    {
        G3GatorFlags(object, game, state);
        G3GatorMove(object, game, state, settings, module_base, frame);
    }
    G3GatorVertical(object, game, state, settings, frame);
    G3GatorAttack(object, game, state, settings);
    G3GatorKnockback(object, game, state, settings);
    if (r_u8(settings + 0x26) != 0)
    {
        sint32 z = r_s16(object + 0x5c);
        sint32 floor = r_s16(player + 0xe0);
        sint32 difference = z - floor;
        if (difference < 0) difference = -difference;
        if (difference < 5 && (r_u32(input + 4) & 0x20) != 0) w_u16(state, 3);
    }
    w_u16(player + 0x58, r_u16(object + 0x58));
    w_u16(player + 0x5a, r_u16(object + 0x5a));
    w_u16(player + 0x5c, r_u16(object + 0x5c));
    if (r_s16(state) == 3)
    {
        uint32 angle = (uint32)(r_s16(player + 0x64) - 0x400);
        uint32 velocity = G3Function8005B470(angle);
        angle = (uint32)(r_s16(player + 0x64) - 0x400);
        w_u32(player + 0x104, (uint32)((sint32)velocity >> 5));
        velocity = G3Function8005B590(angle);
        w_u32(player + 0x108, (uint32)((sint32)velocity >> 5));
        w_u32(player + 0x10c, 0x96);
        w_u32(model + 0x1fc, 0);
        uint32 animations = r_u32(settings);
        uint32 animation = r_u8(animations + 6);
        w_u16(object + 0x5e, 0);
        w_u16(object + 0x6e, animation);
    }
}

void G3GatorUpdate(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000014c0, "G3L_73");
    uint32 frame = guest_stack - 0x38;
    w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
    switch ((uint32)r_s16(state))
    {
    case 1:
    {
        uint32 descriptor = r_u32(object + 0x1c);
        if (r_u32(descriptor + 0x10) != 0)
        {
            G3Function80029F0C(object, 0xffffffff, frame);
            if ((r_u32(object + 0x14) & 0x10) != 0)
            {
                uint32 animations = r_u32(settings);
                uint32 animation = r_u8(animations + 2);
                w_u16(object + 0x5e, 0);
                w_u16(object + 0x6e, animation);
                uint32 player = r_u32(game + 0x50);
                w_u16(player + 0x5e, 0);
                player = r_u32(0x8009efc4);
                w_u16(state, 2);
                w_u16(state + 2, 0);
                w_u16(state + 0x18, r_u16(player + 0xb4));
                w_u16(state + 0x1a, r_u16(player + 0xb6));
                w_u16(state + 0x1c, r_u16(player + 0xb8));
            }
        }
        break;
    }
    case 2:
    {
        G3GatorRide(object, game, state, settings, module_base, frame);
        uint32 component = r_u16(object + 0x58);
        w_u32(object + 0xf8, 0);
        w_u16(frame + 0x18, component);
        w_u16(frame + 0x10, component);
        component = r_u16(object + 0x5a);
        w_u16(frame + 0x1a, component);
        w_u16(frame + 0x12, component);
        w_u16(frame + 0x14, r_u16(object + 0x5c) - 0x100);
        w_u16(frame + 0x1c, r_u16(object + 0x5c) + 0x100);
        uint32 terrain = r_u32(game + 0x48);
        terrain = r_u32(terrain);
        G3Function800622EC(terrain, frame + 0x10, frame + 0x18, object, frame);
        break;
    }
    case 3:
    {
        uint32 descriptor = r_u32(object + 0x1c);
        if (r_u32(descriptor + 0x10) == 0) w_u16(state, 0);
        else
        {
            G3Function80029F0C(object, 0xffffffff, frame);
            if ((r_u32(object + 0x14) & 0x10) != 0)
            {
                uint32 animations = r_u32(settings);
                uint32 player = r_u32(0x8009efc4);
                uint32 animation = r_u8(animations);
                w_u16(object + 0x5e, 0);
                w_u16(object + 0x6e, animation);
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
    if (r_u8(settings + 9) != 0) G3GatorBank(object, state, settings, frame);
}
