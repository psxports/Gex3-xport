#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void G3MonsterDefaultPointers2238(uint32 table)
{
    FUNCTION_MARKER(0x00002238, "G3L_65");
    if (r_u32(table + 0x34) == 0) w_u32(table + 0x34, table + 0x164);
    if (r_u32(table + 0x30) == 0) w_u32(table + 0x30, table + 0x14c);
    if (r_u32(table + 0x44) == 0) w_u32(table + 0x44, table + 0x17c);
    if (r_u32(table + 0x88) == 0) w_u32(table + 0x88, r_u32(table + 0x84));
}

void G3MonsterValidateGroup229C(uint32 group, uint32 descriptor)
{
    FUNCTION_MARKER(0x0000229c, "G3L_65");
    if (group == 0) return;
    uint32 index = 0;
    uint32 table = group + 8;
    uint32 entry = r_u32(table);
    while (entry != 0)
    {
        uint32 offset = (uint32)((sint32)(index << 16) >> 14);
        G3MonsterResolveAnimation19C0(r_u32(table + offset), descriptor);
        ++index;
        offset = (uint32)((sint32)(index << 16) >> 14);
        entry = r_u32(table + offset);
    }
}

void G3MonsterValidateGroups231C(uint32 descriptor, uint32 model)
{
    FUNCTION_MARKER(0x0000231c, "G3L_65");
    uint32 cursor = r_u32(model + 8);
    if (cursor == 0) return;
    uint32 entry = r_u32(cursor);
    while (entry != 0)
    {
        entry = r_u32(cursor);
        cursor += 4;
        G3MonsterValidateGroup229C(entry, descriptor);
        entry = r_u32(cursor);
    }
}

void G3MonsterInitializeResource2380(uint32 descriptor, uint32 model, uint32 module_base)
{
    FUNCTION_MARKER(0x00002380, "G3L_65");
    G3MonsterClearPointers1AEC(model);
    G3MonsterBindAnimations1B24(model, module_base);
    G3MonsterClearCommands2060(model);
    G3MonsterBuildDefaults20B0(model);
    G3MonsterDefaultPointers2238(model);
    G3MonsterValidateAnimations1A30(descriptor, model);
    G3MonsterValidateGroups231C(descriptor, model);
}

uint32 G3MonsterLookup23EC(uint32 kind, uint32 first_index, uint32 second_index, uint32 table)
{
    FUNCTION_MARKER(0x000023ec, "G3L_65");
    if (kind != 0x15) return r_u32(table + kind * 4 + 0x2c);
    uint32 groups = r_u32(table + 8);
    if (groups == 0) return 0;
    uint32 group = r_u32(groups + first_index * 4);
    uint32 elements = group + 8;
    if (group == 0 || elements == 0) return 0;
    return r_u32(elements + second_index * 4);
}

uint32 G3MonsterAdvanceBounded244C(uint32 step, uint32 position, uint32 settings, uint32 scale)
{
    FUNCTION_MARKER(0x0000244c, "G3L_65");
    sint32 rate = r_s16(settings + 6);
    sint32 factor = rate * (sint16)scale / 100;
    uint32 amount = (uint32)r_s16(step) * (uint32)factor;
    sint32 delta = (sint32)amount / 100;
    uint32 value = r_u16(position) + (uint32)delta;
    w_u16(position, value);
    uint32 limit;
    bool clamp;
    if (factor >= 0)
    {
        sint32 maximum = r_s16(settings + 4);
        limit = (uint32)((maximum + 1) * 40 - 1);
        clamp = (sint32)(limit << 16) < (sint32)(value << 16);
    }
    else
    {
        sint32 minimum = r_s16(settings + 2);
        limit = (uint32)(minimum * 40);
        clamp = (sint32)(value << 16) < (sint32)(limit << 16);
    }
    if (clamp) w_u16(position, limit);
    w_u16(step, 0);
    return clamp ? 1 : 0;
}

void G3MonsterSampleMotion2524(uint32 animation, uint32 time, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002524, "G3L_65");
    uint32 speed_bits = r_u16(animation + 6);
    sint32 elapsed = (sint16)time;
    w_u16(output, 0);
    w_u16(output + 2, 0);
    w_u16(output + 4, 0);
    w_u16(output + 8, 0);
    w_u16(output + 0xa, 0);
    w_u16(output + 0xc, 0);
    sint32 turn = (sint16)((uint32)r_s16(animation + 0x10) * 11);
    sint32 numerator = turn * elapsed;
    uint32 end = r_u16(animation + 4);
    uint32 begin = r_u16(animation + 2);
    sint32 span = (sint16)(end - begin + 1);
    sint32 divisor = (sint16)(span * 40);
    sint32 quotient;
    if (divisor == 0) quotient = numerator < 0 ? 1 : -1;
    else if (numerator == (sint32)0x80000000 && divisor == -1) quotient = numerator;
    else quotient = numerator / divisor;
    sint32 speed = (sint16)speed_bits;
    w_u16(output + 0xc, (uint32)((sint16)quotient * speed / 100));
    numerator = r_s16(animation + 0xc) * elapsed;
    if (divisor == 0) quotient = numerator < 0 ? 1 : -1;
    else if (numerator == (sint32)0x80000000 && divisor == -1) quotient = numerator;
    else quotient = numerator / divisor;
    w_u16(output, (uint32)((sint16)quotient * speed / 100));
    numerator = r_s16(animation + 0xe) * elapsed;
    if (divisor == 0) quotient = numerator < 0 ? 1 : -1;
    else if (numerator == (sint32)0x80000000 && divisor == -1) quotient = numerator;
    else quotient = numerator / divisor;
    w_u16(output + 2, (uint32)((sint16)quotient * speed / 100));
    G3MonsterQuarterTurn19A8(output);
}

void G3MonsterScaleVector26A0(uint32 vector, uint32 scale)
{
    FUNCTION_MARKER(0x000026a0, "G3L_65");
    sint32 x = r_s16(vector);
    sint32 factor = r_s16(scale);
    sint32 product = x * factor;
    if (product < 0) product += 0xfff;
    sint32 y = r_s16(vector + 2);
    w_u16(vector, (uint32)(product >> 12));
    factor = r_s16(scale + 2);
    product = y * factor;
    if (product < 0) product += 0xfff;
    sint32 z = r_s16(vector + 4);
    w_u16(vector + 2, (uint32)(product >> 12));
    factor = r_s16(scale + 4);
    product = z * factor;
    if (product < 0) product += 0xfff;
    w_u16(vector + 4, (uint32)(product >> 12));
}

void G3MonsterAnimationMotion271C(uint32 animation, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000271c, "G3L_65");
    w_u16(output, 0);
    w_u16(output + 2, 0);
    w_u16(output + 4, 0);
    w_u16(output + 8, 0);
    w_u16(output + 0xa, 0);
    w_u16(output + 0xc, 0);
    uint32 turn = (uint32)r_s16(animation + 0x10) * 11;
    w_u16(output + 0xc, turn);
    w_u16(output, r_u16(animation + 0xc));
    w_u16(output + 2, r_u16(animation + 0xe));
    G3MonsterQuarterTurn19A8(output);
}

void G3MonsterLookupMotion2784(uint32 state, uint32 output, uint32 table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002784, "G3L_65");
    uint32 frame = guest_stack - 0x18;
    uint32 kind = (uint32)r_s8(state + 2);
    uint32 first = (uint32)r_s8(state + 5);
    uint32 second = (uint32)r_s8(state + 6);
    uint32 animation = G3MonsterLookup23EC(kind, first, second, table);
    if (animation == 0)
    {
        w_u16(output, 0);
        w_u16(output + 2, 0);
        w_u16(output + 4, 0);
        w_u16(output + 8, 0);
        w_u16(output + 0xa, 0);
        w_u16(output + 0xc, 0);
    }
    else G3MonsterAnimationMotion271C(animation, output, frame);
}

uint32 G3MonsterSelectAnimation27EC(uint32 phase, uint32 output, uint32 state, uint32 model)
{
    FUNCTION_MARKER(0x000027ec, "G3L_65");
    uint32 kind = (uint32)r_s8(state + 2);
    uint32 first_index = (uint32)r_s8(state + 5);
    uint32 second_index = (uint32)r_s8(state + 6);
    uint32 animation = G3MonsterLookup23EC(kind, first_index, second_index, model);
    if (animation != 0)
    {
        w_u32(output, animation);
        sint32 direction = r_s16(animation + 6);
        uint32 value;
        if (direction >= 0) value = (uint32)(r_s16(animation + 2) * 40);
        else value = (uint32)((r_s16(animation + 4) + 1) * 40 - 1);
        w_u16(phase, value);
    }
    return animation != 0 ? 1 : 0;
}

void G3MonsterProjectSeparation2888(uint32 output, uint32 vector, uint32 normal, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002888, "G3L_65");
    uint32 frame = guest_stack - 0x40;
    sint32 first = r_s16(normal);
    sint32 second = r_s16(vector);
    uint32 dot = (uint32)(first * second);
    first = r_s16(normal + 2);
    second = r_s16(vector + 2);
    dot += (uint32)(first * second);
    first = r_s16(normal + 4);
    second = r_s16(vector + 4);
    dot += (uint32)(first * second);
    uint32 length = G3Function8002FA50(normal, frame);
    if ((sint32)dot < 0)
    {
        uint32 magnitude = 0;
        if (length != 0)
        {
            magnitude = dot == 0x80000000 && length == 0xffffffff ? 0x80000000 : (uint32)((sint32)dot / (sint32)length);
            if ((sint32)magnitude < 0) magnitude = 0 - magnitude;
        }
        uint32 x = (uint32)r_s16(normal);
        uint32 product_x = x * magnitude;
        uint32 quotient_x = length == 0 ? ((sint32)product_x < 0 ? 1 : 0xffffffff) : product_x == 0x80000000 && length == 0xffffffff ? 0x80000000 : (uint32)((sint32)product_x / (sint32)length);
        w_u32(frame + 0x10, x);
        uint32 y = (uint32)r_s16(normal + 2);
        uint32 product_y = y * magnitude;
        uint32 quotient_y = length == 0 ? ((sint32)product_y < 0 ? 1 : 0xffffffff) : product_y == 0x80000000 && length == 0xffffffff ? 0x80000000 : (uint32)((sint32)product_y / (sint32)length);
        w_u32(frame + 0x14, y);
        uint32 z = (uint32)r_s16(normal + 4);
        uint32 product_z = z * magnitude;
        uint32 quotient_z = length == 0 ? ((sint32)product_z < 0 ? 1 : 0xffffffff) : product_z == 0x80000000 && length == 0xffffffff ? 0x80000000 : (uint32)((sint32)product_z / (sint32)length);
        w_u32(frame + 0x10, product_x);
        w_u32(frame + 0x14, product_y);
        w_u32(frame + 0x18, z);
        w_u32(frame + 0x18, product_z);
        w_u32(frame + 0x10, quotient_x);
        w_u32(frame + 0x14, quotient_y);
        w_u32(frame + 0x18, quotient_z);
        sint32 projected_x = (sint32)quotient_x;
        if (projected_x < -32767) projected_x = -32767;
        if (projected_x > 32767) projected_x = 32767;
        w_u16(frame + 0x20, (uint32)projected_x);
        sint32 projected_y = (sint32)quotient_y;
        if (projected_y < -32767) projected_y = -32767;
        if (projected_y > 32767) projected_y = 32767;
        w_u16(frame + 0x22, (uint32)projected_y);
        sint32 projected_z = (sint32)quotient_z;
        if (projected_z < -32767) projected_z = -32767;
        if (projected_z > 32767) projected_z = 32767;
        w_u16(frame + 0x24, (uint32)projected_z);
        w_u16(output, r_u16(vector) + (uint32)projected_x);
        uint32 position = r_u16(vector + 2);
        uint32 offset = r_u16(frame + 0x22);
        w_u16(output + 2, position + offset);
        position = r_u16(vector + 4);
        offset = r_u16(frame + 0x24);
        w_u16(output + 4, position + offset);
    }
    else
    {
        w_u16(output, r_u16(vector));
        w_u16(output + 2, r_u16(vector + 2));
        w_u16(output + 4, r_u16(vector + 4));
    }
}

uint32 G3MonsterFacingContact2A84(uint32 rotation, uint32 vector, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002a84, "G3L_65");
    uint32 frame = guest_stack - 0x18;
    sint32 z = r_s16(vector + 4);
    sint32 x = r_s16(vector);
    if (z < 0) z = -z;
    if (x < 0) x = -x;
    if (x < z)
    {
        sint32 y = r_s16(vector + 2);
        if (y < 0) y = -y;
        if (y < z) return 0;
    }
    uint32 heading = r_u16(rotation + 4);
    uint32 y = (uint32)r_s16(vector + 2);
    uint32 nx = (uint32)r_s16(vector);
    uint32 angle = G3Function8002F0C4(y, nx, frame);
    uint32 difference = (heading - 0x400 - angle - 0x800) & 0xfff;
    if (difference >= 0x800) difference |= 0xf000;
    sint32 magnitude = (sint16)difference;
    if (magnitude < 0) magnitude = -magnitude;
    return magnitude < 0x371 ? 1 : 0;
}

void G3MonsterDeflectVelocity2B34(uint32 velocity, uint32 delta, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002b34, "G3L_65");
    uint32 frame = guest_stack - 0x28;
    uint32 bits = r_u8(velocity);
    w_u16(frame + 0x10, (uint32)(sint8)bits);
    bits = r_u8(velocity + 1);
    w_u16(frame + 0x14, 0);
    w_u16(frame + 0x12, (uint32)(sint8)bits);
    G3MonsterProjectSeparation2888(frame + 0x18, frame + 0x10, delta, frame);
    sint32 x = r_s16(frame + 0x18);
    uint32 x_bits = r_u16(frame + 0x18);
    if (x < -127) x_bits = (uint32)-127;
    else if (x >= 127) x_bits = 127;
    sint32 y = r_s16(frame + 0x1a);
    uint32 y_bits = r_u16(frame + 0x1a);
    w_u16(frame + 0x18, x_bits);
    if (y < -127) y_bits = (uint32)-127;
    else if (y >= 127) y_bits = 127;
    bits = r_u8(frame + 0x18);
    w_u16(frame + 0x1a, y_bits);
    w_u8(velocity, bits);
    w_u8(velocity + 1, r_u8(frame + 0x1a));
}

uint32 G3MonsterClassifyContact2C0C(uint32 output, uint32 contact, uint32 side)
{
    FUNCTION_MARKER(0x00002c0c, "G3L_65");
    uint32 object;
    uint32 kind;
    uint32 descriptor;
    if (side == 0)
    {
        object = r_u32(contact + 0x10);
        kind = r_u16(contact + 4);
        descriptor = r_u32(contact + 8);
    }
    else
    {
        object = r_u32(contact + 0x14);
        kind = r_u16(contact + 6);
        descriptor = r_u32(contact + 0xc);
    }
    w_u32(output + 4, object);
    if ((sint16)kind == 1)
    {
        w_u8(output, r_u8(descriptor + 4));
        w_u8(output + 1, r_u8(descriptor + 5));
    }
    else if ((sint16)kind == 5)
    {
        w_u8(output, r_u8(descriptor + 2));
        w_u8(output + 1, r_u8(descriptor + 3));
    }
    else
    {
        w_u32(output + 4, 0);
        return 0;
    }
    return r_u32(output + 4) != 0 ? 1 : 0;
}

void G3MonsterBuildDamagePacket2CA0(uint32 packet, uint32 object, uint32 target_position, uint32 contact, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002ca0, "G3L_65");
    uint32 frame = guest_stack - 0x30;
    uint32 trigger = r_u32(guest_stack + 0x10);
    sint32 damage = r_s16(trigger + 8);
    w_u16(packet + 0x10, 0);
    w_u32(packet, (uint32)damage);
    sint32 kind = r_s16(trigger + 0x10);
    if (kind == 1) w_u16(packet + 0x10, 7);
    else if (kind == 2) w_u16(packet + 0x10, 3);
    uint32 coordinate = r_u16(target_position);
    uint32 origin = r_u16(object + 0x58);
    w_u16(frame + 0x10, coordinate - origin);
    coordinate = r_u16(target_position + 2);
    origin = r_u16(object + 0x5a);
    w_u16(frame + 0x14, 0);
    w_u16(frame + 0x12, coordinate - origin);
    uint32 magnitude_bits = r_u16(trigger + 0xa);
    sint32 length = (sint16)magnitude_bits / 5;
    G3Function8002F9A8(frame + 0x18, (uint32)(sint16)length, frame + 0x10, frame);
    w_u32(packet + 4, (uint32)r_s16(frame + 0x18));
    w_u32(packet + 8, (uint32)r_s16(frame + 0x1a));
    uint32 x = r_u32(packet + 4);
    uint32 z = (uint32)r_s16(frame + 0x1c);
    w_u32(packet + 4, x << 16);
    uint32 y = r_u32(packet + 8);
    w_u32(packet + 0x14, contact);
    w_u32(packet + 0xc, z);
    w_u32(packet + 0xc, z << 16);
    w_u32(packet + 8, y << 16);
}

void G3MonsterAttackContact2DA8(uint32 own_class, uint32 other_class, uint32 contact, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002da8, "G3L_65");
    uint32 frame = guest_stack - 0x50;
    uint32 other = r_u32(contact + 0x14);
    uint32 player = r_u32(0x8009efc4);
    if (other != player) return;
    uint32 state = object + 0x104;
    uint32 guard = ~(r_u32(state + 0x18) & 0x8000);
    if (guard == 0) return;
    if (r_u8(own_class + 1) - (uint32)6 >= 4) return;
    uint32 kind = r_u8(other_class + 1);
    if (kind != 9 && kind - 3 >= 3) return;
    uint32 trigger = G3MonsterFindTrigger7670(0, r_u32(state + 0x14));
    if (trigger == 0) return;
    uint32 own_kind = r_u8(own_class);
    sint32 required_kind = r_s16(trigger + 0xc);
    if (own_kind != (uint32)required_kind) return;
    player = r_u32(0x8009efc4);
    w_u32(frame + 0x10, trigger);
    G3MonsterBuildDamagePacket2CA0(frame + 0x18, object, player + 0x58, contact, frame);
    if (r_s16(trigger + 8) != 0)
    {
        player = r_u32(0x8009efc4);
        uint32 target = r_u32(player + 0xec);
        if (target != 0) G3DispatchStateEvent(target, player, r_u32(0x80097048), 8, frame + 0x18, frame);
    }
    w_u32(state + 0x18, r_u32(state + 0x18) | 0x8000);
    w_u32(object + 0x14, r_u32(object + 0x14) & 0xf7ffffff);
    if ((r_u16(trigger + 0xe) & 4) != 0)
    {
        G3MonsterMoveReduced1780(object, frame);
        w_u8(state + 0x36, 15);
    }
}

void G3MonsterSeparateContact2F1C(uint32 contact, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002f1c, "G3L_65");
    uint32 frame = guest_stack - 0x58;
    w_u16(frame + 0x28, r_u16(object + 0x58));
    w_u16(frame + 0x2a, r_u16(object + 0x5a));
    w_u16(frame + 0x2c, r_u16(object + 0x5c));
    w_u16(frame + 0x10, r_u16(contact + 0x28));
    uint32 offset = r_u16(contact + 0x2a);
    w_u16(frame + 0x14, 0);
    w_u16(frame + 0x12, offset);
    uint32 other = r_u32(contact + 0x14);
    uint32 state = object + 0x104;
    bool direct = true;
    if (other != 0)
    {
        uint32 descriptor = r_u32(other + 0x1c);
        bool query = (r_u32(descriptor) & 1) != 0;
        if (!query) query = other == r_u32(0x8009efc4);
        if (query) direct = G3Function8008345C(other, 0, 0) != 0;
    }
    uint32 result_z;
    if (direct)
    {
        w_u16(frame + 0x18, r_u16(contact + 0x28));
        w_u16(frame + 0x1a, r_u16(contact + 0x2a));
        w_u16(frame + 0x1c, r_u16(contact + 0x2c));
        if (G3MonsterFacingContact2A84(object + 0x60, frame + 0x18, frame) != 0) w_u32(state + 0x18, r_u32(state + 0x18) | 0x10);
        if ((r_u32(state + 0x20) & 0xffff0000) != 0) G3MonsterDeflectVelocity2B34(state + 0x22, frame + 0x10, frame);
        uint32 position = r_u16(object + 0x58);
        offset = r_u16(frame + 0x10);
        w_u16(object + 0x58, position + offset);
        position = r_u16(object + 0x5a);
        offset = r_u16(frame + 0x12);
        w_u16(object + 0x5a, position + offset);
        position = r_u16(object + 0x5c);
        offset = r_u16(frame + 0x14);
        result_z = position + offset;
    }
    else
    {
        uint32 position = r_u16(object + 0x58);
        uint32 previous = r_u16(state + 0x2a);
        w_u16(frame + 0x38, position - previous);
        position = r_u16(object + 0x5a);
        previous = r_u16(state + 0x2c);
        w_u16(frame + 0x3a, position - previous);
        position = r_u16(object + 0x5c);
        previous = r_u16(state + 0x2e);
        w_u16(frame + 0x3c, position - previous);
        G3MonsterProjectSeparation2888(frame + 0x40, frame + 0x38, frame + 0x10, frame);
        previous = r_u16(state + 0x2a);
        offset = r_u16(frame + 0x40);
        w_u16(object + 0x58, previous + offset);
        previous = r_u16(state + 0x2c);
        offset = r_u16(frame + 0x42);
        w_u16(object + 0x5a, previous + offset);
        previous = r_u16(state + 0x2e);
        offset = r_u16(frame + 0x44);
        result_z = previous + offset;
    }
    w_u16(object + 0x5c, result_z);
    uint32 coordinate = r_u16(object + 0x58);
    uint32 previous = r_u16(frame + 0x28);
    r_u32(0x80097048);
    uint32 x = coordinate - previous;
    w_u16(frame + 0x20, x);
    coordinate = r_u16(object + 0x5a);
    previous = r_u16(frame + 0x2a);
    uint32 previous_z = r_u16(frame + 0x2c);
    uint32 y = coordinate - previous;
    w_u16(frame + 0x22, y);
    coordinate = r_u16(object + 0x5c);
    w_u16(frame + 0x30, x);
    w_u16(frame + 0x32, y);
    uint32 z = coordinate - previous_z;
    w_u16(frame + 0x24, z);
    w_u16(frame + 0x34, z);
    G3Function8005F3A8(object, frame + 0x30);
}

void G3MonsterCollide313C(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000313c, "G3L_65");
    uint32 frame = guest_stack - 0x30;
    uint32 contact = r_u32(object + 0xb0);
    G3MonsterClassifyContact2C0C(frame + 0x10, contact, 0);
    G3MonsterClassifyContact2C0C(frame + 0x18, contact, 1);
    if (r_u8(frame + 0x11) - (uint32)6 < 4 && (r_u32(object + 0x14) & 0x8000000) != 0) G3MonsterAttackContact2DA8(frame + 0x10, frame + 0x18, contact, object, frame);
    uint32 kind = r_u8(frame + 0x11);
    if (kind == 9 || kind - 3 < 3) G3MonsterSeparateContact2F1C(contact, object, frame);
}

void G3MonsterInitializePacket31F8(uint32 packet)
{
    FUNCTION_MARKER(0x000031f8, "G3L_65");
    G3FillMemory80086094(packet, 0, 0x18);
    w_u32(packet, 1);
}

uint32 G3MonsterCheckFlags322C(uint32 state, uint32 model)
{
    FUNCTION_MARKER(0x0000322c, "G3L_65");
    uint32 flags = r_u32(state + 0x18);
    uint32 resource = r_u32(model);
    uint32 resource_flags = r_u32(resource + 0x18);
    uint32 result = (flags & 0x4000000) != 0 ? 1 : 0;
    if ((resource_flags & 8) != 0 && (flags & 0x800000) == 0) result = 1;
    if ((r_u32(state + 0x18) & 0x400000) != 0) result = 1;
    return result;
}

uint32 G3MonsterDamage3284(uint32 object, uint32 packet, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003284, "G3L_65");
    uint32 frame = guest_stack - 0x38;
    uint32 model = r_u32(object + 0x24);
    uint32 state = object + 0x104;
    if (packet == 0) return 0;
    uint32 contact = r_u32(packet + 0x14);
    if (contact != 0)
    {
        uint32 attacker = r_u32(contact + 0x10);
        uint32 player = r_u32(0x8009efc4);
        if (attacker == player)
        {
            uint32 serial = r_u32(attacker + 0x138);
            uint32 previous = r_u32(state + 0x50);
            if (serial == previous) return 0;
            w_u32(state + 0x50, serial);
        }
    }
    if (G3MonsterCheckFlags322C(state, model) != 0) return 0;
    if (r_s8(state + 1) == 0xa) return 0;
    uint32 resource = r_u32(model);
    if ((r_u32(resource + 0x18) & 0x20) == 0)
    {
        if ((r_u32(state + 0x18) & 0x200000) != 0) return 0;
        if (r_u8(state + 0x4e) != 0) return 0;
    }
    resource = r_u32(model);
    uint32 flags = r_u32(resource + 0x18);
    uint32 damage = r_u16(packet);
    if ((flags & 0x10) != 0 && r_s16(packet + 0x10) != 1) damage = 0;
    resource = r_u32(model);
    uint32 divisor = r_u32(resource + 4);
    if ((sint32)divisor <= 0)
    {
        w_u32(frame + 0x10, 0);
        w_u32(frame + 0x14, 0);
        w_u32(frame + 0x18, 0);
    }
    else
    {
        uint32 numerator = r_u32(packet + 4);
        uint32 quotient = (uint32)((sint32)numerator / (sint32)divisor);
        w_u32(frame + 0x10, quotient);
        resource = r_u32(model);
        numerator = r_u32(packet + 8);
        divisor = r_u32(resource + 4);
        quotient = divisor == 0 ? ((sint32)numerator < 0 ? 1 : 0xffffffff) : numerator == 0x80000000 && divisor == 0xffffffff ? 0x80000000 : (uint32)((sint32)numerator / (sint32)divisor);
        w_u32(frame + 0x14, quotient);
        resource = r_u32(model);
        numerator = r_u32(packet + 0xc);
        divisor = r_u32(resource + 4);
        quotient = divisor == 0 ? ((sint32)numerator < 0 ? 1 : 0xffffffff) : numerator == 0x80000000 && divisor == 0xffffffff ? 0x80000000 : (uint32)((sint32)numerator / (sint32)divisor);
        w_u32(frame + 0x18, quotient);
    }
    uint32 x = r_u32(frame + 0x10);
    uint32 z = r_u32(frame + 0x18);
    w_u32(frame + 0x10, (uint32)((sint32)x >> 10));
    uint32 y = r_u32(frame + 0x14);
    w_u32(frame + 0x18, (uint32)((sint32)z >> 10));
    w_u32(frame + 0x14, (uint32)((sint32)y >> 10));
    uint32 length = G3Function8002FAB0(frame + 0x10, frame);
    if ((sint32)length >= 0x80)
    {
        x = r_u32(frame + 0x10) * 127;
        uint32 scaled_x = (uint32)((sint32)x / (sint32)length);
        y = r_u32(frame + 0x14) * 127;
        uint32 scaled_y = (uint32)((sint32)y / (sint32)length);
        z = r_u32(frame + 0x18) * 127;
        uint32 scaled_z = (uint32)((sint32)z / (sint32)length);
        w_u32(frame + 0x10, x);
        w_u32(frame + 0x14, y);
        w_u32(frame + 0x18, z);
        w_u32(frame + 0x10, scaled_x);
        w_u32(frame + 0x14, scaled_y);
        w_u32(frame + 0x18, scaled_z);
    }
    sint32 component = r_s32(frame + 0x10);
    if (component < -127) component = -127;
    if (component >= 128) component = 127;
    sint32 next_component = r_s32(frame + 0x14);
    w_u32(frame + 0x10, (uint32)component);
    if (next_component < -127) next_component = -127;
    if (next_component >= 128) next_component = 127;
    component = r_s32(frame + 0x18);
    w_u32(frame + 0x14, (uint32)next_component);
    if (component < -127) component = -127;
    if (component >= 128) component = 127;
    x = r_u8(frame + 0x10);
    w_u32(frame + 0x18, (uint32)component);
    w_u8(state + 0x22, x);
    w_u8(state + 0x23, r_u8(frame + 0x14));
    w_u16(state + 0x40, r_u16(frame + 0x18));
    sint32 health = r_s16(state + 0x42);
    uint32 health_bits = r_u16(state + 0x42);
    if (health > 0) w_u16(state + 0x42, health_bits - damage);
    if (r_s16(state + 0x42) < 0) w_u16(state + 0x42, 0);
    resource = r_u32(model);
    if ((r_u32(resource + 0x18) & 0x40) != 0) G3Function80047EE4(10, (uint32)r_s16(state + 0x42));
    if (r_s16(state + 0x42) == 0) w_u32(state + 0x18, r_u32(state + 0x18) | 2);
    else
    {
        resource = r_u32(model);
        sint32 threshold = r_s32(resource + 0x14);
        if ((sint16)damage < threshold) return 1;
        w_u32(state + 0x18, r_u32(state + 0x18) | 4);
    }
    return 1;
}

uint32 G3MonsterRequestRemoval35FC(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000035fc, "G3L_65");
    uint32 frame = guest_stack - 0x18;
    uint32 descriptor = r_u32(object + 0x1c);
    if ((r_u32(descriptor + 0x3c) & 1) != 0) G3Function80082648(object, 0, frame);
    w_u32(object + 0x11c, r_u32(object + 0x11c) | 0x400);
    return 1;
}

void G3MonsterTraceFloor3650(uint32 output, uint32 object, uint32 top, uint32 bottom, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003650, "G3L_65");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x40;
    uint32 level = r_u32(game + 0x48);
    uint32 x = r_u16(object + 0x58);
    uint32 world = r_u32(level);
    w_u16(frame + 0x20, x);
    w_u16(frame + 0x22, r_u16(object + 0x5a));
    uint32 z = r_u16(object + 0x5c);
    w_u16(frame + 0x24, z);
    w_u16(frame + 0x28, r_u16(object + 0x58));
    w_u16(frame + 0x2a, r_u16(object + 0x5a));
    uint32 raised_z = r_u16(object + 0x5c);
    w_u16(frame + 0x24, z - bottom);
    w_u16(frame + 0x2c, raised_z);
    w_u16(frame + 0x2c, raised_z + top);
    uint32 flags = r_u32(object + 0x14);
    uint32 mask = (flags & 0x20) != 0 ? 0xfffffff7 : 0x1c00be0;
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, mask);
    w_u32(frame + 0x18, 0);
    uint32 face = G3Function8006238C(world, frame + 0x20, frame + 0x28, 1, frame);
    w_u32(output + 4, face);
    if (face != 0)
    {
        w_u32(output, 1);
        face = r_u32(output + 4);
        uint32 normals = r_u32(world + 0x40);
        uint32 index = (uint32)r_s16(face + 8);
        G3Function8005F304(index, normals, output + 8, frame);
        {
            uint32 source = frame + 0x20;
            uint32 destination = output + 0x10;
            uint32 first_address = source + 0;
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
            first_address = destination + 0;
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
    }
}

void G3MonsterBoxFloor3750(uint32 output, uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003750, "G3L_65");
    uint32 configuration = r_u32(object + 0x28);
    uint32 property = G3MonsterFindProperty0C60(configuration, module_base + 0x7be8, module_base);
    w_u32(output, 2);
    w_u32(output + 4, 0);
    w_u16(output + 8, 0);
    w_u16(output + 0xa, 0);
    w_u16(output + 0xc, 0x1000);
    w_u16(output + 0x10, r_u16(object + 0x58));
    w_u16(output + 0x12, r_u16(object + 0x5a));
    w_u16(output + 0x14, r_u16(object + 0x5c));
    w_u16(output + 0x14, r_u16(property + 4));
}

void G3MonsterQueryFloor37D8(uint32 output, uint32 object, uint32 top, uint32 bottom, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000037d8, "G3L_65");
    uint32 frame = guest_stack - 0x18;
    if ((r_u32(object + 0x11c) & 0x1000) == 0) G3MonsterTraceFloor3650(output, object, top, bottom, frame);
    else
    {
        r_u32(guest_stack + 0x10);
        G3MonsterBoxFloor3750(output, object, module_base, frame);
    }
}

void G3MonsterQueryTerrain3820(uint32 output, uint32 object, uint32 position, uint32 bottom, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003820, "G3L_65");
    uint32 frame = guest_stack - 0x40;
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    uint32 world = r_u32(level);
    w_u16(frame + 0x20, r_u16(object + 0x58));
    w_u16(frame + 0x22, r_u16(object + 0x5a));
    uint32 z = r_u16(object + 0x5c);
    w_u16(frame + 0x24, z);
    w_u16(frame + 0x28, r_u16(position));
    uint32 y = r_u16(position + 2);
    w_u16(frame + 0x2a, y);
    uint32 raised_z = r_u16(position + 4);
    w_u16(frame + 0x24, z - bottom);
    w_u16(frame + 0x2c, raised_z);
    w_u16(frame + 0x2c, raised_z + 10);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0x1c00bea);
    w_u32(frame + 0x18, 0);
    uint32 face = G3Function8006238C(world, frame + 0x20, frame + 0x28, 0, frame);
    w_u32(output + 4, face);
    if (face != 0)
    {
        w_u32(output, 1);
        face = r_u32(output + 4);
        uint32 normals = r_u32(world + 0x40);
        G3Function8005F304((uint32)r_s16(face + 8), normals, output + 8, frame);
        {
            uint32 source = frame + 0x20;
            uint32 destination = output + 0x10;
            uint32 first_address = source + 0;
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
            first_address = destination + 0;
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
    }
}

void G3MonsterCaptureFloor3908(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003908, "G3L_65");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x38;
    uint32 level = r_u32(game + 0x48);
    uint32 x = r_u16(object + 0x58);
    uint32 world = r_u32(level);
    w_u16(frame + 0x20, x);
    w_u16(frame + 0x22, r_u16(object + 0x5a));
    uint32 z = r_u16(object + 0x5c);
    w_u16(frame + 0x24, z);
    w_u16(frame + 0x28, r_u16(object + 0x58));
    w_u16(frame + 0x2a, r_u16(object + 0x5a));
    uint32 top = r_u16(object + 0x5c);
    w_u16(frame + 0x24, z - 0x500);
    w_u16(frame + 0x2c, top);
    w_u16(frame + 0x2c, top + 0x100);
    w_u32(frame + 0x14, 0xff3fff3f);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x18, 0);
    uint32 face = G3Function8006238C(world, frame + 0x20, frame + 0x28, 0, frame);
    if (face != 0)
    {
        w_u32(object + 0xf8, face);
        {
            uint32 source = frame + 0x20;
            uint32 destination = object + 0xfc;
            uint32 first_address = source + 0;
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
            first_address = destination + 0;
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
    }
}

void G3MonsterResolveFloor39C8(uint32 object, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000039c8, "G3L_65");
    uint32 frame = guest_stack - 0x48;
    uint32 state = object + 0x104;
    uint32 floor = frame + 0x18;
    w_u32(floor, 0);
    w_u32(frame + 0x10, model);
    G3MonsterQueryFloor37D8(floor, object, 0x100, 0x500, module_base, frame);
    bool grounded = false;
    if (r_u32(floor) != 0)
    {
        sint32 z = r_s16(object + 0x5c);
        sint32 floor_z = r_s16(floor + 0x14);
        uint32 floor_bits = r_u16(floor + 0x14);
        if (z - floor_z < 31 && r_s16(state + 0x40) < 10)
        {
            w_u16(object + 0x5c, floor_bits);
            grounded = true;
        }
    }
    if (grounded) w_u16(state + 0x40, 0);
    else
    {
        uint32 velocity = r_u16(state + 0x40) - 9;
        w_u16(state + 0x40, velocity);
        if ((sint16)velocity < -100) w_u16(state + 0x40, (uint32)-100);
    }
    uint32 game = r_u32(0x80097048);
    sint32 velocity = r_s16(state + 0x40);
    sint32 step = r_s16(game + 0x69bc);
    sint32 movement = velocity * step / 40;
    w_u16(object + 0x5c, r_u16(object + 0x5c) + (uint32)movement);
    if (r_u32(floor) == 0) w_u32(object + 0xa8, 0);
    else w_u32(object + 0xa8, r_u32(floor + 4));
    uint32 descriptor = r_u32(object + 0x1c);
    if ((r_u32(descriptor) & 0x200) == 0 || r_u32(floor) == 0) return;
    uint32 resource = r_u32(model);
    if ((r_u32(resource + 0x18) & 1) != 0)
    {
        w_u32(floor, 0);
        G3MonsterQueryTerrain3820(floor, object, floor + 0x10, 0x500, frame);
    }
    uint32 kind = r_u32(floor);
    if (kind == 0) return;
    if (kind == 1)
    {
        uint32 face = r_u32(floor + 4);
        if ((r_u16(face + 6) & 0x801) == 0x800) w_u32(object + 0x10, r_u32(object + 0x10) | 0x200000);
    }
    w_u16(object + 0xb4, r_u16(floor + 8));
    w_u16(object + 0xb6, r_u16(floor + 0xa));
    w_u16(object + 0xb8, r_u16(floor + 0xc));
    w_u16(object + 0xdc, r_u16(floor + 0x10));
    w_u16(object + 0xde, r_u16(floor + 0x12));
    w_u16(object + 0xe0, r_u16(floor + 0x14));
}

uint32 G3MonsterClampBox3C08(uint32 position, uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003c08, "G3L_65");
    uint32 frame = guest_stack - 0x40;
    uint32 configuration = r_u32(object + 0x28);
    uint32 radius = r_u16(position + 6);
    uint32 property = G3MonsterFindProperty0C60(configuration, module_base + 0x7be8, module_base);
    {
        uint32 source = property;
        uint32 destination = frame + 0x10;
        uint32 first_address = source + 0;
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
        uint32 third_address = source + 8;
        uint32 third_high_address = third_address + 3;
        uint32 third_high_lane = third_high_address & 3;
        uint32 third_lane = third_address & 3;
        uint32 third_high = r_masked_u32(third_high_address & 0xfffffffc, (1 << (third_high_lane + 1)) - 1);
        uint32 third_low = r_masked_u32(third_address & 0xfffffffc, (15 << third_lane) & 15);
        uint32 third_keep = third_lane == 0 ? 0 : 0xffffffff << ((4 - third_lane) * 8);
        uint32 third_value = ((third_high << ((3 - third_high_lane) * 8)) & third_keep) | (third_low >> (third_lane * 8));
        first_address = destination + 0;
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
        third_address = destination + 8;
        third_high_address = third_address + 3;
        third_high_lane = third_high_address & 3;
        third_lane = third_address & 3;
        w_masked_u32(third_high_address & 0xfffffffc, third_value >> ((3 - third_high_lane) * 8), (1 << (third_high_lane + 1)) - 1);
        w_masked_u32(third_address & 0xfffffffc, third_value << (third_lane * 8), (15 << third_lane) & 15);
    }
    uint32 minimum_x = r_u16(frame + 0x10) + radius;
    w_u16(frame + 0x10, minimum_x);
    uint32 minimum_y = r_u16(frame + 0x12);
    uint32 maximum_x = r_u16(frame + 0x16);
    minimum_y += radius;
    w_u16(frame + 0x12, minimum_y);
    uint32 maximum_y = r_u16(frame + 0x18);
    maximum_x -= radius;
    w_u16(frame + 0x16, maximum_x);
    w_u16(frame + 0x18, maximum_y - radius);
    sint32 signed_x = r_s16(position);
    uint32 x = r_u16(position);
    uint32 offset_x = 0;
    uint32 offset_y = 0;
    if (signed_x < (sint16)minimum_x) offset_x = minimum_x - x;
    else if ((sint16)maximum_x < signed_x) offset_x = maximum_x - x;
    minimum_y = r_u16(frame + 0x12);
    sint32 signed_y = r_s16(position + 2);
    uint32 y = r_u16(position + 2);
    if (signed_y < (sint16)minimum_y) offset_y = minimum_y - y;
    else
    {
        sint32 signed_maximum = r_s16(frame + 0x18);
        maximum_y = r_u16(frame + 0x18);
        if (signed_maximum < signed_y) offset_y = maximum_y - y;
    }
    x = r_u16(position);
    y = r_u16(position + 2);
    w_u16(position, x + offset_x);
    w_u16(position + 2, y + offset_y);
    return (sint16)offset_x != 0 || (sint16)offset_y != 0 ? 1 : 0;
}

uint32 G3MonsterTraceHorizontal3D8C(uint32 position, uint32 previous, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003d8c, "G3L_65");
    uint32 frame = guest_stack - 0x38;
    uint32 game = r_u32(0x80097048);
    w_u32(frame + 0x10, position);
    w_u32(frame + 0x14, previous);
    w_u32(frame + 0x18, 0);
    w_u32(frame + 0x1c, 0);
    w_u32(frame + 0x20, 0);
    w_u32(frame + 0x24, 0);
    w_u32(frame + 0x28, 0);
    uint32 level = r_u32(game + 0x48);
    uint32 world = r_u32(level);
    return G3Function80063664(frame + 0x10, world, 0x1c003e0, 1, frame);
}

void G3MonsterResolveHorizontal3DE4(uint32 object, uint32 radius, uint32 height, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003de4, "G3L_65");
    uint32 frame = guest_stack - 0x38;
    uint32 mode = r_u32(guest_stack + 0x10);
    {
        uint32 source = object + 0x134;
        uint32 destination = frame + 0x10;
        uint32 first_address = source + 0;
        uint32 first_high_address = first_address + 3;
        uint32 first_high_lane = first_high_address & 3;
        uint32 first_lane = first_address & 3;
        uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
        uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
        uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
        uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
        uint32 z = (uint32)r_s16(source + 4);
        first_address = destination + 0;
        first_high_address = first_address + 3;
        first_high_lane = first_high_address & 3;
        first_lane = first_address & 3;
        w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
        w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
        w_u16(destination + 4, z);
    }
    w_u16(frame + 0x1e, radius);
    {
        uint32 source = object + 0x58;
        uint32 destination = frame + 0x18;
        uint32 first_address = source + 0;
        uint32 first_high_address = first_address + 3;
        uint32 first_high_lane = first_high_address & 3;
        uint32 first_lane = first_address & 3;
        uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
        uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
        uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
        uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
        uint32 z = (uint32)r_s16(source + 4);
        first_address = destination + 0;
        first_high_address = first_address + 3;
        first_high_lane = first_high_address & 3;
        first_lane = first_address & 3;
        w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
        w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
        w_u16(destination + 4, z);
    }
    uint32 z = r_u16(frame + 0x1c);
    uint32 previous_z = r_u16(frame + 0x14);
    uint32 raised_z = z + height;
    w_u16(frame + 0x1c, raised_z);
    w_u16(frame + 0x14, previous_z + height);
    uint32 result = 0;
    if (mode == 0) result = G3MonsterTraceHorizontal3D8C(frame + 0x18, frame + 0x10, frame);
    else if (mode == 1) result = G3MonsterClampBox3C08(frame + 0x18, object, module_base, frame);
    if (result != 0)
    {
        uint32 x = r_u16(frame + 0x18);
        uint32 old_x = r_u16(object + 0x58);
        uint32 y = r_u16(frame + 0x1a);
        w_u16(frame + 0x20, x - old_x);
        z = r_u16(frame + 0x1c);
        uint32 old_y = r_u16(object + 0x5a);
        w_u16(frame + 0x24, z - raised_z);
        w_u16(frame + 0x22, y - old_y);
        if (G3MonsterFacingContact2A84(object + 0x60, frame + 0x20, frame) != 0) w_u32(object + 0x11c, r_u32(object + 0x11c) | 0x10);
        w_u16(object + 0x58, r_u16(frame + 0x18));
        w_u16(object + 0x5a, r_u16(frame + 0x1a));
    }
}

void G3MonsterResolveSides3F14(uint32 object, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003f14, "G3L_65");
    uint32 frame = guest_stack - 0x28;
    if ((r_u32(object + 0x11c) & 0x20) != 0)
    {
        w_u32(frame + 0x10, 0);
        G3MonsterResolveHorizontal3DE4(object, 100, (uint32)-200, model, module_base, frame);
    }
    if ((r_u32(object + 0x11c) & 0x1000) != 0)
    {
        w_u32(frame + 0x10, 1);
        G3MonsterResolveHorizontal3DE4(object, 250, 400, model, module_base, frame);
    }
}

void G3MonsterPrePhysics3F9C(uint32 object, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003f9c, "G3L_65");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    uint32 position = r_u16(object + 0x58);
    uint32 origin = r_u16(state + 0x2a);
    uint32 x = position - origin;
    w_u16(frame + 0x10, x);
    position = r_u16(object + 0x5a);
    origin = r_u16(state + 0x2c);
    uint32 y = position - origin;
    w_u16(frame + 0x12, y);
    position = r_u16(object + 0x5c);
    origin = r_u16(state + 0x2e);
    uint32 z = position - origin;
    w_u16(frame + 0x14, z);
    bool changed = (sint16)x != 0 || (sint16)y != 0 || (sint16)z != 0;
    if (!changed) changed = r_s16(state + 0x40) != 0;
    if (!changed) changed = (r_u32(state + 0x18) & 0x100) != 0;
    if (changed)
    {
        uint32 resource = r_u32(model);
        if (r_u32(resource) != 1) G3MonsterResolveFloor39C8(object, model, module_base, frame);
        if ((r_u32(state + 0x18) & 0x100) != 0) G3MonsterCaptureFloor3908(object, frame);
    }
    w_u32(object + 0x10, r_u32(object + 0x10) | 0x8000000);
}

void G3MonsterPostPhysics4094(uint32 object, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004094, "G3L_65");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    uint32 position = r_u16(object + 0x58);
    uint32 origin = r_u16(state + 0x30);
    uint32 x = position - origin;
    w_u16(frame + 0x10, x);
    position = r_u16(object + 0x5a);
    origin = r_u16(state + 0x32);
    uint32 y = position - origin;
    w_u16(frame + 0x12, y);
    position = r_u16(object + 0x5c);
    origin = r_u16(state + 0x34);
    uint32 z = position - origin;
    w_u16(frame + 0x14, z);
    bool changed = (sint16)x != 0 || (sint16)y != 0 || (sint16)z != 0;
    if (!changed) changed = r_s16(state + 0x40) != 0;
    if (!changed) changed = (r_u32(state + 0x18) & 0x100) != 0;
    if (changed)
    {
        uint32 resource = r_u32(model);
        if (r_u32(resource) != 1) G3MonsterResolveSides3F14(object, model, module_base, frame);
    }
}

uint32 G3MonsterTransition414C(uint32 object, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000414c, "G3L_65");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    uint32 flags = r_u32(state + 0x18);
    uint32 count = 0;
    while ((flags & 0x8000000) == 0)
    {
        G3MonsterPrepareState4230(state, configuration, model, module_base, frame);
        uint32 selected = G3MonsterSelectAnimation27EC(state + 0x20, state + 0x14, state, model);
        model = G3MonsterApplyTransition4678(state, configuration, model, object, module_base, frame);
        ++count;
        if ((selected & 0xff) != 0 || count >= 15) break;
        flags = r_u32(state + 0x18);
    }
    return model;
}

void G3MonsterPrepareState4230(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004230, "G3L_65");
    uint32 frame = guest_stack - 0x28;
    uint32 count = 0;
    for (;;)
    {
        uint32 previous = r_u8(state + 1);
        G3MonsterPrepareMode42E0(state, configuration, model, module_base, frame);
        G3MonsterDispatchMode43B4(state, configuration, model, module_base, frame);
        sint32 current = r_s8(state + 1);
        if ((sint8)previous == current) break;
        G3MonsterClearFlag4660(state);
        if (++count >= 15) break;
    }
    G3MonsterDispatchCommand4520(state, configuration, model, module_base, frame);
}

void G3MonsterPrepareMode42E0(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000042e0, "G3L_65");
    uint32 frame = guest_stack - 0x28;
    uint32 snapshot = r_u32(state + 0x1c);
    sint32 mode = r_s8(state);
    uint32 previous = r_u8(state);
    if (mode == 0)
    {
        w_u32(frame + 0x10, 100);
        if (G3MonsterShouldWake5BD0(snapshot + 0x14, configuration, state, model, module_base, frame) != 0) w_u8(state, 1);
    }
    else if (mode == 1 && r_s8(state + 1) == 0x10) w_u8(state, 0);
    mode = r_s8(state);
    if (mode != (sint8)previous)
    {
        w_u32(state + 0x18, r_u32(state + 0x18) & 0xfeffffff);
        mode = r_s8(state);
        if (mode == 1)
        {
            w_u8(state + 1, 0xd);
            w_u8(state + 2, 0x1b);
        }
        else if (mode == 0)
        {
            w_u8(state + 1, 0xe);
            w_u8(state + 2, 0x1b);
        }
    }
}

void G3MonsterDispatchMode43B4(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000043b4, "G3L_65");
    uint32 frame = guest_stack - 0x18;
    sint32 mode = r_s8(state);
    if (mode == 1) G3MonsterActiveMode43FC(state, configuration, model, module_base, frame);
    else if (mode == 0) G3MonsterIdleMode447C(state);
}

void G3MonsterActiveMode43FC(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000043fc, "G3L_65");
    uint32 frame = guest_stack - 0x20;
    uint32 previous = r_u8(state + 1);
    G3MonsterSelectState4A9C(state, configuration, model, module_base, frame);
    uint32 kind = r_u8(state + 1);
    if (kind - 6 < 2 && kind != previous && (sint8)previous != 9 && (sint8)previous != 10 && (sint8)previous != 8)
    {
        w_u8(state + 0xb, 0);
        w_u8(state + 8, 0);
        w_u8(state + 9, 0);
    }
}

void G3MonsterIdleMode447C(uint32 state)
{
    FUNCTION_MARKER(0x0000447c, "G3L_65");
    sint32 mode = r_s8(state + 1);
    uint32 next = r_u8(state + 1);
    bool check_completion = true;
    if (mode == 1)
    {
        if ((r_u32(state + 0x18) & 0x1000000) != 0) next = 2;
    }
    else if (mode == 2 || mode == 0xe || mode == 0xf)
    {
        if (r_s8(state + 2) == 0x1c) next = r_u8(state + 0x3f);
        else check_completion = false;
    }
    if (check_completion && r_s8(state + 2) == 0x1c)
    {
        w_u8(state + 1, next);
        w_u8(state + 2, 0x1b);
        w_u8(state + 0xf, 0);
        return;
    }
    if ((sint8)next != r_s8(state + 1)) w_u8(state + 0xf, 1);
}

void G3MonsterDispatchCommand4520(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004520, "G3L_65");
    uint32 frame = guest_stack - 0x20;
    uint32 snapshot = r_u32(state + 0x1c);
    sint32 kind = r_s8(state + 1);
    uint32 values = snapshot + 0x14;
    uint32 angles = snapshot + 0x20;
    switch (kind)
    {
    case 0: G3MonsterSetCommand509C(state); break;
    case 1: G3MonsterSwitchCommand528C(state, state, model, state + 0x18, frame); break;
    case 2: G3MonsterSetCommand54B4(state); break;
    case 4:
    case 5: G3MonsterResetCommand4FEC(state, state); break;
    case 6:
    case 7:
    case 0xc: G3MonsterSwitchCommand5008(state, state, model, frame); break;
    case 8:
        w_u32(frame + 0x10, state);
        G3MonsterSwitchCommand54CC(state, model, frame);
        break;
    case 9: G3MonsterSwitchCommand514C(state, state); break;
    case 0xa: G3MonsterSwitchCommand50F0(state, state); break;
    case 0xb: G3MonsterSetCommand54C0(state); break;
    case 0xd: G3MonsterSwitchCommand524C(state); break;
    case 0xe: G3MonsterSwitchCommand50A8(state); break;
    case 0xf: G3MonsterSwitchCommand50CC(state); break;
    case 0x11: G3MonsterSwitchCommand51E4(state, state); break;
    }
}

void G3MonsterClearFlag4660(uint32 state)
{
    FUNCTION_MARKER(0x00004660, "G3L_65");
    w_u32(state + 0x18, r_u32(state + 0x18) & 0xfeffffff);
}

uint32 G3MonsterApplyTransition4678(uint32 state, uint32 configuration, uint32 model, uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004678, "G3L_65");
    uint32 frame = guest_stack - 0x28;
    if (r_s8(state + 1) == 0x11 && r_u8(state + 2) - (uint32)0x16 < 2)
    {
        G3MonsterRunSequence0E7C(object, model, frame);
        uint32 resource = r_u32(model + 0x18);
        model = r_u32(resource);
        w_u32(object + 0x24, model);
        w_u8(state + 0xb, 0);
        w_u8(state + 0x10, 0);
        w_u8(state + 8, 0);
        w_u8(state + 9, 0);
    }
    uint32 flags = r_u32(state + 0x18);
    w_u16(state + 0x44, 100);
    w_u16(state + 0x46, 100);
    w_u8(state + 0x37, 0);
    w_u32(state + 0x18, flags & 0xff3f7fff);
    w_u32(object + 0x14, r_u32(object + 0x14) & 0xf7ffffff);
    sint32 mode = r_s8(state + 2);
    if (mode == 0xc)
    {
        G3MonsterNoop0A30();
        mode = r_s8(state + 2);
    }
    if (mode == 0x14)
    {
        r_s8(state + 0x3e);
        G3MonsterNoop72D0();
        w_u8(state + 0x3e, 0xff);
        G3MonsterRunProperty0EB4(object, module_base + 0x7bd0, module_base, frame);
        w_u32(object + 0x10, r_u32(object + 0x10) & 0xfffffbff);
        w_u32(state + 0x18, r_u32(state + 0x18) | 0x8000000);
    }
    uint32 resource = r_u32(model);
    sint32 kind;
    if ((r_u32(resource + 0x18) & 2) != 0)
    {
        mode = r_s8(state + 2);
        if (mode == 2)
        {
            G3MonsterMoveReduced1780(object, frame);
            kind = r_s8(state + 1);
        }
        else
        {
            kind = r_s8(state + 1);
            if (kind == 0xd && mode == 3)
            {
                G3MonsterMoveDefault1730(object, frame);
                kind = r_s8(state + 1);
            }
        }
    }
    else kind = r_s8(state + 1);
    if (kind == 0x11) w_u32(state + 0x18, r_u32(state + 0x18) & 0xffefffff);
    kind = r_s8(state + 1);
    if (kind == 6) w_u32(state + 0x18, r_u32(state + 0x18) & 0xfffff7ff);
    else if (kind == 7) w_u32(state + 0x18, r_u32(state + 0x18) | 0x800);
    if (r_s8(state + 2) == 0x20)
    {
        kind = r_s8(state + 1);
        if (kind == 1)
        {
            uint32 minimum = G3MonsterMinimum5E34(configuration);
            uint32 snapshot = r_u32(state + 0x1c);
            resource = r_u32(model);
            uint32 result = G3MonsterTurnToward6F44(snapshot + 0xe, state + 0x24, resource + 0xc, minimum, frame);
            w_u8(state + 0xd, result);
        }
        else if (kind == 2)
        {
            uint32 minimum = G3MonsterMinimum5E34(configuration);
            uint32 snapshot = r_u32(state + 0x1c);
            G3MonsterSelectHeading6E98(state + 0x24, object + 0x58, snapshot + 0xe, minimum, frame);
        }
        else if (kind == 0xb) G3MonsterUpdatePath6488(state, model, frame);
    }
    flags = r_u32(state + 0x18);
    if ((flags & 0x200000) != 0)
    {
        uint32 count = r_u8(state + 9);
        w_u32(state + 0x18, flags & 0xfffffffb);
        if (count != 0xff) w_u8(state + 9, count + 1);
        G3MonsterRunProperty0EB4(object, module_base + 0x7c30, module_base, frame);
        if (r_s8(state + 2) == 0x1c) w_u8(state + 0x4e, 8);
    }
    if (r_s8(state + 1) == 0xa && r_s8(state + 2) == 0xc) G3MonsterRunProperty0EB4(object, module_base + 0x7c30, module_base, frame);
    if (r_u8(state + 1) >= 3) w_u32(state + 0x18, r_u32(state + 0x18) & 0xffffffbf);
    if (r_s8(state + 1) == 1 && r_s8(state + 2) == 0xd) w_u8(state + 7, r_u8(state + 7) + 1);
    if (G3MonsterFindTrigger7670(0, r_u32(state + 0x14)) != 0)
    {
        w_u32(object + 0x14, r_u32(object + 0x14) | 0x8000000);
        uint32 count = r_u8(state + 8);
        if (count != 0xff) w_u8(state + 8, count + 1);
    }
    G3MonsterFindTrigger7670(1, r_u32(state + 0x14));
    w_u8(state + 0x37, 0);
    if (r_s8(state + 1) == 0xb)
    {
        mode = r_s8(state + 2);
        if (mode != 0xe)
        {
            if (mode == 0x20) G3MonsterMoveReduced1780(object, frame);
            else if (mode == 0x12) G3MonsterMoveDefault1730(object, frame);
        }
        if (r_s8(state + 1) == 0xb) w_u8(state + 0x10, 0);
    }
    return model;
}

void G3MonsterSelectState4A9C(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004a9c, "G3L_65");
    uint32 frame = guest_stack - 0x48;
    uint32 snapshot = r_u32(state + 0x1c);
    uint32 distances = snapshot + 0x14;
    w_u16(frame + 0x18, r_u16(distances + 2));
    w_u16(frame + 0x1a, r_u16(distances + 0xa));
    uint32 current_snapshot = r_u32(state + 0x1c);
    w_u16(frame + 0x1c, r_u16(current_snapshot + 0x20));
    sint32 selected = r_s8(state + 1);
    uint32 flags;
    sint32 selection;
    if (selected != 0x11 && G3MonsterCheckAlternate5B0C(state, model, frame) != 0)
    {
        selected = 0x11;
        goto CommitSelection;
    }
    flags = r_u32(state + 0x18);
    if ((flags & 0x200002) == 2)
    {
        w_u8(state + 2, 0x1c);
        r_s8(state + 0x3e);
        G3MonsterNoop72D0();
        selected = 10;
        w_u8(state + 0x3e, 0xff);
        goto CommitSelection;
    }
    if ((flags & 4) != 0)
    {
        uint32 resource = r_u32(model);
        if ((r_u32(resource + 0x18) & 0x100) == 0 && (flags & 0x200000) == 0)
        {
            selected = 9;
            w_u8(state + 2, 0x1c);
            goto CommitSelection;
        }
    }
    selection = (sint8)(r_u8(state + 1) - 4);
    switch (selection)
    {
    case 0:
        w_u32(frame + 0x10, 100);
        if (G3MonsterShouldWake5BD0(distances, configuration, state, model, module_base, frame) != 0) goto WakeSelection;
        if (r_s8(state + 2) != 0x1c) goto MarkChange;
        selected = 0xc;
        goto StoreTransition;
    case 1:
        if (G3MonsterCheckWake5D20(state, configuration, model, module_base, frame) == 0) { selected = 4; break; }
        if (r_u8(state + 0xc) >= 0x3d) { selected = 4; break; }
        if (r_s8(state + 2) == 0x1c) goto DefaultSelection;
        {
            uint32 current = r_u32(state + 0x1c);
            uint32 health = (uint32)r_s16(state + 0x42);
            if (G3MonsterCheckSpecial5AB0(state, current, model, health, frame) != 0)
            {
                selected = (sint32)G3MonsterConstant6550();
                break;
            }
            uint32 candidate = G3MonsterSelectValue6558(model);
            w_u32(frame + 0x10, 0);
            w_u32(frame + 0x14, candidate);
            if (G3MonsterFindGroup751C(0, frame + 0x18, model, state, frame) != 0) selected = (sint32)candidate;
        }
        break;
    case 2:
    case 3:
        if (G3MonsterCheckWake5D20(state, configuration, model, module_base, frame) == 0)
        {
            uint32 resource = r_u32(model);
            selected = r_u32(resource) == 1 ? 0xc : 4;
            break;
        }
        if ((r_u32(state + 0x18) & 8) != 0) { selected = 5; break; }
        {
            uint32 current = r_u32(state + 0x1c);
            uint32 health = (uint32)r_s16(state + 0x42);
            uint32 change = selection == 2 ? G3MonsterCheckRetreat5994(state, current, model, health, frame) : G3MonsterCheckAdvance5870(state, current, model, health, frame);
            if (change != 0) { selected = selection == 2 ? 7 : 6; break; }
            current = r_u32(state + 0x1c);
            health = (uint32)r_s16(state + 0x42);
            if (G3MonsterCheckSpecial5AB0(state, current, model, health, frame) != 0)
            {
                selected = (sint32)G3MonsterConstant6550();
                break;
            }
            w_u32(frame + 0x10, 0);
            uint32 previous = (uint32)r_s8(state + 1);
            w_u32(frame + 0x14, previous);
            if (G3MonsterFindGroup751C(state + 5, frame + 0x18, model, state, frame) != 0) selected = 8;
        }
        break;
    case 4:
        if (r_s8(state + 2) == 0x1c)
        {
            selected = r_s8(state + 4);
            goto StoreTransition;
        }
        w_u32(frame + 0x10, 0);
        w_u32(frame + 0x14, (uint32)r_s8(state + 1));
        if (G3MonsterFindGroup751C(frame + 0x20, frame + 0x18, model, state, frame) != 0)
        {
            uint32 index = (uint32)r_s8(frame + 0x20);
            uint32 table = r_u32(model + 8);
            uint32 previous = (uint32)r_s8(state + 5);
            uint32 group = r_u32(table + index * 4);
            uint32 old_group = r_u32(table + previous * 4);
            uint32 resource = r_u32(group);
            uint32 old_resource = r_u32(old_group);
            sint32 priority = r_s16(resource + 4);
            sint32 old_priority = r_s16(old_resource + 4);
            if (old_priority < priority) w_u8(state + 0xf, 1);
        }
        break;
    case 5:
        {
            sint32 previous = r_s8(state + 4);
            if (previous == 6) selected = 6;
            else if (previous == 7) selected = 7;
            else goto DefaultSelection;
        }
        break;
    case 6:
        selected = 10;
        break;
    case 7:
        if (r_s8(state + 2) == 0x1c) goto DefaultSelection;
        goto MarkChange;
    case 8:
        w_u32(frame + 0x10, 100);
        if (G3MonsterShouldWake5BD0(distances, configuration, state, model, module_base, frame) != 0) goto WakeSelection;
        if ((r_u32(state + 0x18) & 8) != 0) { selected = 4; break; }
        {
            uint32 distance = r_u16(distances);
            sint32 minimum = (sint32)G3MonsterMinimum5E34(configuration);
            if (minimum > 400)
            {
                minimum = (sint32)G3MonsterMinimum5E34(configuration);
                if ((sint32)distance < minimum) selected = 0x10;
            }
            else if (distance < 400) selected = 0x10;
        }
        break;
    case 9:
        if (r_s8(state + 2) == 0x1c) goto WakeSelection;
        goto MarkChange;
    case 0xd:
        goto DefaultSelection;
    }
    goto CommitSelection;
WakeSelection:
    {
        uint32 resource = r_u32(model);
        if (r_u32(resource) != 1 && G3MonsterConstantFromFlags5BA4(state, frame) == 0)
        {
            selected = 5;
            goto CommitSelection;
        }
    }
DefaultSelection:
    selected = (sint32)G3MonsterSelectValue6558(model);
CommitSelection:
    if (r_s8(state + 2) != 0x1c) goto MarkChange;
StoreTransition:
    {
        uint32 previous = r_u8(state + 1);
        w_u8(state + 1, (uint32)selected);
        w_u8(state + 2, 0x1b);
        w_u8(state + 0xf, 0);
        w_u8(state + 4, previous);
    }
    return;
MarkChange:
    if (selected != r_s8(state + 1)) w_u8(state + 0xf, 1);
}

void G3MonsterResetCommand4FEC(uint32 command, uint32 state)
{
    FUNCTION_MARKER(0x00004fec, "G3L_65");
    w_u32(state + 0x18, r_u32(state + 0x18) & 0xfffffff7);
    w_u8(command + 2, 0x1c);
}

void G3MonsterSwitchCommand5008(uint32 command, uint32 state, uint32 model, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00005008, "G3L_65");
    uint32 frame = guest_stack - 0x30;
    uint32 kind = (uint32)r_s8(command + 1);
    uint32 previous = (uint32)r_s8(command + 4);
    uint32 mode = (uint32)r_s8(state + 0x48);
    uint32 distance = G3MonsterSelectDistance6594(kind, previous, mode, model, frame);
    G3MonsterSelectAngles6AD4(frame + 0x18, frame + 0x1a, state);
    uint32 angle = (uint32)r_s16(frame + 0x18);
    uint32 target_distance = r_u16(frame + 0x1a);
    uint32 old_command = (uint32)r_s8(command + 2);
    w_u32(frame + 0x10, old_command);
    uint32 next = G3MonsterSelectCommand635C(angle, target_distance, distance & 0xffff, model, frame);
    uint32 changed = r_u8(command + 0xf);
    w_u8(command + 2, next);
    if (changed != 0) w_u8(command + 2, 0x1c);
}

void G3MonsterSetCommand509C(uint32 command)
{
    FUNCTION_MARKER(0x0000509c, "G3L_65");
    w_u8(command + 2, 2);
}

void G3MonsterSwitchCommand50A8(uint32 command)
{
    FUNCTION_MARKER(0x000050a8, "G3L_65");
    w_u8(command + 2, r_s8(command + 2) == 0x1b ? 4 : 0x1c);
}

void G3MonsterSwitchCommand50CC(uint32 command)
{
    FUNCTION_MARKER(0x000050cc, "G3L_65");
    w_u8(command + 2, r_s8(command + 2) == 0x1b ? 0x13 : 0x1c);
}

void G3MonsterSwitchCommand50F0(uint32 command, uint32 state)
{
    FUNCTION_MARKER(0x000050f0, "G3L_65");
    sint32 mode = r_s8(command + 2);
    if (mode == 0xc) w_u8(command + 2, 0x14);
    else if (mode == 0x1b)
    {
        w_u32(state + 0x18, r_u32(state + 0x18) | 0x200000);
        w_u8(command + 2, 0xc);
    }
}

void G3MonsterSwitchCommand514C(uint32 command, uint32 state)
{
    FUNCTION_MARKER(0x0000514c, "G3L_65");
    sint32 mode = r_s8(command + 2);
    if (mode == 0x1b)
    {
        w_u32(state + 0x18, r_u32(state + 0x18) | 0x200000);
        w_u8(command + 2, 0xb);
    }
    else if (mode == 0xb) w_u8(command + 2, (r_u32(state + 0x18) & 4) != 0 ? 0x1b : 0x1d);
    else if (mode == 0x1d)
    {
        w_u32(state + 0x18, r_u32(state + 0x18) & 0xffdfffff);
        w_u8(command + 2, 0x1c);
    }
}

void G3MonsterSwitchCommand51E4(uint32 command, uint32 state)
{
    FUNCTION_MARKER(0x000051e4, "G3L_65");
    sint32 mode = r_s8(command + 2);
    if (mode == 0x16 || mode == 0x17) w_u8(command + 2, 0x1c);
    else if (mode == 0x1b) w_u8(command + 2, (r_u32(state + 0x18) & 0x800) != 0 ? 0x17 : 0x16);
}

void G3MonsterSwitchCommand524C(uint32 command)
{
    FUNCTION_MARKER(0x0000524c, "G3L_65");
    sint32 mode = r_s8(command + 2);
    if (mode == 0x1b) w_u8(command + 2, 3);
    else if (mode == 3) w_u8(command + 2, 0x1c);
}

void G3MonsterSwitchCommand528C(uint32 command, uint32 state, uint32 model, uint32 flags, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000528c, "G3L_65");
    uint32 frame = guest_stack - 0x30;
    uint32 snapshot = r_u32(state + 0x1c);
    uint32 distances = snapshot + 0x14;
    uint32 angles = snapshot + 0x20;
    uint32 length = G3MonsterMotionDistance6C84(1, model, frame);
    sint32 threshold = (sint16)(length + 100) - 400;
    if (threshold < 400) threshold = 400;
    sint32 selection = (sint8)(r_u8(command + 2) - 1);
    switch (selection)
    {
    case 0:
    case 8:
    case 9:
        if ((sint32)r_u16(distances + 4) < (sint16)threshold)
        {
            w_u8(command + 2, 0x1f);
            break;
        }
        w_u8(command + 2, G3MonsterSelectTurn6DD4(1, (uint32)r_s16(angles + 4), model, frame));
        break;
    case 0x1f:
        w_u8(command + 2, G3MonsterSelectTurn6DD4(1, (uint32)r_s16(angles + 4), model, frame));
        break;
    case 0xc:
        {
            sint32 count = r_s8(command + 7);
            sint32 random = (sint32)G3SdkRandom80027FA8();
            if (count > random % 3 + 1) w_u8(command + 2, 0x20);
        }
        break;
    case 0x1a:
        w_u8(command + 2, 0x20);
        break;
    case 0x1e:
        {
            uint32 result = r_u8(command + 0xd);
            if (result == 0) w_u8(command + 2, 0x20);
            else if (result == 1)
            {
                w_u8(command + 7, 0);
                w_u8(command + 2, 0xd);
            }
        }
        break;
    }
    if (r_u8(command + 0xf) != 0) w_u8(command + 2, 0x1c);
    else if ((r_u32(state + 0x18) & 8) != 0 && r_s8(command + 2) != 0)
    {
        w_u8(command + 2, 0x20);
        w_u32(flags, r_u32(flags) & 0xfffffff7);
    }
}

void G3MonsterSetCommand54B4(uint32 command)
{
    FUNCTION_MARKER(0x000054b4, "G3L_65");
    w_u8(command + 2, 0x1c);
}

void G3MonsterSetCommand54C0(uint32 command)
{
    FUNCTION_MARKER(0x000054c0, "G3L_65");
    w_u8(command + 2, 0x1c);
}

void G3MonsterSwitchCommand54CC(uint32 command, uint32 model, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000054cc, "G3L_65");
    uint32 frame = guest_stack - 0x28;
    sint32 kind = r_s8(command + 2);
    uint32 state = r_u32(guest_stack + 0x10);
    if (kind == 0x1b)
    {
        w_u8(command + 2, 0x15);
        uint32 index = (uint32)r_s8(command + 5);
        w_u8(command + 6, 0);
        uint32 table = r_u32(model + 8);
        uint32 group = r_u32(table + index * 4);
        uint32 resource = r_u32(group);
        if (r_s16(resource) == 2 && (r_u16(resource + 8) & 2) != 0) w_u32(state + 0x18, r_u32(state + 0x18) | 0x200000);
        return;
    }
    if (kind == 0x15)
    {
        if (r_u8(command + 0xf) != 0)
        {
            uint32 animation = r_u32(state + 0x14);
            if ((r_u16(animation + 0x12) & 1) != 0)
            {
                w_u8(command + 2, 0x1d);
                return;
            }
        }
        uint32 next = (uint32)(r_s8(command + 6) + 1);
        uint32 index = (uint32)r_s8(command + 5);
        uint32 count = G3MonsterCountGroup7618(index, model);
        uint32 animation = r_u32(state + 0x14);
        uint32 trigger = G3MonsterFindTrigger7670(2, animation);
        if (trigger != 0)
        {
            sint32 condition = r_s16(trigger + 8);
            bool repeat = condition == 0;
            if (!repeat && condition == 1) repeat = (r_u32(state + 0x18) & 0x8000) != 0;
            if (repeat) next = (uint32)r_s16(trigger + 0xa);
        }
        w_u8(command + 6, next);
        if ((sint8)next >= (sint32)count) w_u8(command + 2, 0x1d);
        return;
    }
    if (kind != 0x1d) return;
    uint32 index = (uint32)r_s8(command + 5);
    uint32 table = r_u32(model + 8);
    uint32 group = r_u32(table + index * 4);
    uint32 resource = r_u32(group);
    if (r_s16(resource) == 2 && (r_u16(resource + 8) & 1) != 0) w_u32(state + 0x18, r_u32(state + 0x18) & 0xfffffbff);
    uint32 flags = r_u32(state + 0x18);
    w_u32(state + 0x18, flags & 0xffdfffff);
    w_u8(command + 2, 0x1c);
}

uint32 G3MonsterAngleBand56D4(uint32 angle)
{
    FUNCTION_MARKER(0x000056d4, "G3L_65");
    sint32 magnitude = (sint16)angle;
    if (magnitude < 0) magnitude = -magnitude;
    magnitude = (sint16)magnitude;
    if (magnitude < 0x200) return 0xfffffff6;
    return magnitude < 0x600 ? 0 : 10;
}

uint32 G3MonsterSelectAlternate5718(uint32 state, uint32 snapshot, uint32 table, uint32 difference, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00005718, "G3L_65");
    sint32 base = r_s16(table);
    uint32 angle = (uint32)r_s16(snapshot + 0x2a);
    uint32 counter = r_u8(guest_stack + 0x10);
    sint32 band = (sint16)G3MonsterAngleBand56D4(angle);
    sint32 band_factor = r_s16(table + 4);
    uint32 score = (uint32)base + (uint32)(band * band_factor);
    sint32 counter_factor = r_s16(table + 2);
    score += counter * (uint32)counter_factor;
    sint32 health_factor = r_s16(table + 6);
    score += (uint32)((sint16)difference * health_factor);
    sint32 random = (sint32)G3SdkRandom80027FA8();
    sint32 random_factor = r_s16(table + 8);
    uint32 random_score = (uint32)((random % 20 - 10) * random_factor);
    uint32 distance = r_u16(snapshot + 0x16);
    sint32 distance_factor = r_s16(table + 0xa);
    sint32 distance_score = (sint32)(distance * (uint32)distance_factor);
    sint32 offset = r_s16(snapshot + 0x1e);
    sint32 offset_factor = r_s16(table + 0xc);
    if (offset < 0) offset = -offset;
    sint32 offset_score = offset * offset_factor;
    uint32 first_count = r_u8(state + 9);
    sint32 first_factor = r_s16(table + 0x10);
    uint32 first_score = first_count * (uint32)first_factor;
    uint32 second_count = r_u8(state + 8);
    sint32 second_factor = r_s16(table + 0x12);
    score += random_score;
    score += (uint32)(distance_score >> 8);
    score += (uint32)(offset_score >> 8);
    score += first_score;
    return score + second_count * (uint32)second_factor;
}

uint32 G3MonsterCheckAdvance5870(uint32 state, uint32 snapshot, uint32 model, uint32 health, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00005870, "G3L_65");
    uint32 frame = guest_stack - 0x48;
    uint32 resource = r_u32(model);
    uint32 base = r_u16(resource + 8);
    uint32 settings = r_u32(model + 0xc);
    uint32 difference = base - health;
    if (settings == 0) return 1;
    uint32 narrowed = (uint32)(sint16)difference;
    w_u32(frame + 0x10, r_u8(state + 0xb));
    uint32 result = G3MonsterSelectAlternate5718(state, snapshot, settings + 0x28, narrowed, frame);
    if ((sint32)result < 0) return 0;
    settings = r_u32(model + 0xc);
    {
        uint32 source = settings + 0x10;
        uint32 destination = frame + 0x18;
        uint32 first_address = source + 0;
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
        uint32 third_address = source + 8;
        uint32 third_high_address = third_address + 3;
        uint32 third_high_lane = third_high_address & 3;
        uint32 third_lane = third_address & 3;
        uint32 third_high = r_masked_u32(third_high_address & 0xfffffffc, (1 << (third_high_lane + 1)) - 1);
        uint32 third_low = r_masked_u32(third_address & 0xfffffffc, (15 << third_lane) & 15);
        uint32 third_keep = third_lane == 0 ? 0 : 0xffffffff << ((4 - third_lane) * 8);
        uint32 third_value = ((third_high << ((3 - third_high_lane) * 8)) & third_keep) | (third_low >> (third_lane * 8));
        uint32 fourth_address = source + 12;
        uint32 fourth_high_address = fourth_address + 3;
        uint32 fourth_high_lane = fourth_high_address & 3;
        uint32 fourth_lane = fourth_address & 3;
        uint32 fourth_high = r_masked_u32(fourth_high_address & 0xfffffffc, (1 << (fourth_high_lane + 1)) - 1);
        uint32 fourth_low = r_masked_u32(fourth_address & 0xfffffffc, (15 << fourth_lane) & 15);
        uint32 fourth_keep = fourth_lane == 0 ? 0 : 0xffffffff << ((4 - fourth_lane) * 8);
        uint32 fourth_value = ((fourth_high << ((3 - fourth_high_lane) * 8)) & fourth_keep) | (fourth_low >> (fourth_lane * 8));
        first_address = destination + 0;
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
        third_address = destination + 8;
        third_high_address = third_address + 3;
        third_high_lane = third_high_address & 3;
        third_lane = third_address & 3;
        w_masked_u32(third_high_address & 0xfffffffc, third_value >> ((3 - third_high_lane) * 8), (1 << (third_high_lane + 1)) - 1);
        w_masked_u32(third_address & 0xfffffffc, third_value << (third_lane * 8), (15 << third_lane) & 15);
        fourth_address = destination + 12;
        fourth_high_address = fourth_address + 3;
        fourth_high_lane = fourth_high_address & 3;
        fourth_lane = fourth_address & 3;
        w_masked_u32(fourth_high_address & 0xfffffffc, fourth_value >> ((3 - fourth_high_lane) * 8), (1 << (fourth_high_lane + 1)) - 1);
        w_masked_u32(fourth_address & 0xfffffffc, fourth_value << (fourth_lane * 8), (15 << fourth_lane) & 15);
    }
    {
        uint32 source = settings + 0x20;
        uint32 destination = frame + 0x28;
        uint32 first_address = source + 0;
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
        first_address = destination + 0;
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
    w_u16(frame + 0x1a, 0);
    w_u16(frame + 0x20, 0);
    w_u16(frame + 0x28, 0);
    w_u16(frame + 0x2a, 0);
    w_u32(frame + 0x10, r_u8(state + 0xb));
    result = G3MonsterSelectAlternate5718(state, snapshot, frame + 0x18, narrowed, frame);
    return result >> 31;
}
