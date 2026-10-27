#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32 G3MonsterCheckRetreat5994(uint32 state, uint32 snapshot, uint32 model, uint32 health, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00005994, "G3L_65");
    uint32 frame = guest_stack - 0x48;
    uint32 resource = r_u32(model);
    uint32 base = r_u16(resource + 8);
    uint32 settings = r_u32(model + 0xc);
    uint32 difference = base - health;
    if (settings == 0) return 0;
    uint32 narrowed = (uint32)(sint16)difference;
    w_u32(frame + 0x10, r_u8(state + 0xb));
    uint32 result = G3MonsterSelectAlternate5718(state, snapshot, settings + 0x10, narrowed, frame);
    if ((sint32)result < 0) return 0;
    settings = r_u32(model + 0xc);
    {
        uint32 source = settings + 0x28;
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
        uint32 source = settings + 0x38;
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

uint32 G3MonsterCheckSpecial5AB0(uint32 state, uint32 snapshot, uint32 model, uint32 health, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00005ab0, "G3L_65");
    uint32 frame = guest_stack - 0x20;
    uint32 resource = r_u32(model);
    uint32 base = r_u16(resource + 8);
    uint32 settings = r_u32(model + 0x10);
    uint32 difference = base - health;
    if (settings == 0) return 0;
    w_u32(frame + 0x10, r_u8(state + 0x10));
    uint32 result = G3MonsterSelectAlternate5718(state, snapshot, settings + 0x10, (uint32)(sint16)difference, frame);
    return ~result >> 31;
}

uint32 G3MonsterCheckAlternate5B0C(uint32 state, uint32 model, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00005b0c, "G3L_65");
    uint32 frame = guest_stack - 0x20;
    uint32 resource = r_u32(model);
    uint32 base = r_u16(resource + 8);
    uint32 health = r_u16(state + 0x42);
    uint32 snapshot = r_u32(state + 0x1c);
    uint32 difference = base - health;
    if (model == 0) return 0;
    uint32 alternate = r_u32(model + 0x18);
    if (alternate == 0) return 0;
    if ((r_u32(state + 0x18) & 0x100000) != 0) return 1;
    uint32 table = r_u32(alternate + 4);
    if (table == 0) return 0;
    uint32 counter = r_u8(state + 0xb);
    w_u32(frame + 0x10, counter);
    sint32 result = (sint32)G3MonsterSelectAlternate5718(state, snapshot, table, (uint32)(sint16)difference, frame);
    return result < 0 ? 0 : 1;
}

uint32 G3MonsterConstantFromFlags5BA4(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00005ba4, "G3L_65");
    r_u32(state + 0x18);
    return G3MonsterConstant7304() & 0xff;
}

uint32 G3MonsterShouldWake5BD0(uint32 settings, uint32 configuration, uint32 state, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00005bd0, "G3L_65");
    uint32 frame = guest_stack - 0x30;
    uint32 percent = (uint32)r_s16(guest_stack + 0x10);
    uint32 range = r_u32(configuration);
    sint32 product = (sint32)(range * percent);
    sint32 threshold = product / 100;
    if ((r_u32(state + 0x18) & 6) != 0) return 1;
    if (G3MonsterCheckWake5D20(state, configuration, model, module_base, frame) == 0) return 0;
    uint32 flags = r_u32(state + 0x18);
    if ((flags & 0x40) != 0) return 1;
    if ((flags & 0x1000000) != 0)
    {
        uint32 resource = r_u32(model);
        if ((r_u32(resource + 0x18) & 2) == 0) return 1;
    }
    if (r_s8(state) == 0 && r_s8(state + 1) == 0xf) return 0;
    if (G3MonsterCheckWake5D20(state, configuration, model, module_base, frame) == 0) return 0;
    if ((sint32)r_u16(settings + 2) >= threshold) return 0;
    return r_u8(state + 0xc) < 0x3d ? 1 : 0;
}

uint32 G3MonsterCheckWake5D20(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00005d20, "G3L_65");
    uint32 frame = guest_stack - 0x18;
    uint32 snapshot = r_u32(state + 0x1c);
    sint32 limit = r_s32(configuration + 0xc);
    uint32 distance = r_u16(snapshot + 0x1c);
    if (limit < (sint32)distance) return 0;
    uint32 flags = r_u32(state + 0x18);
    if ((flags & 0x1000) == 0) return 1;
    uint32 resource = r_u32(model);
    if (r_u32(resource) == 1) return 1;
    if ((flags & 0x40000) != 0) return 1;
    uint32 box = G3MonsterFindProperty0C60(configuration, module_base + 0x7be8, module_base);
    sint32 coordinate = r_s16(snapshot);
    sint32 maximum = r_s16(box + 6);
    if (maximum < coordinate) return 0;
    if (coordinate < r_s16(box)) return 0;
    coordinate = r_s16(snapshot + 2);
    maximum = r_s16(box + 8);
    if (maximum < coordinate) return 0;
    if (coordinate < r_s16(box + 2)) return 0;
    maximum = r_s16(box + 0xa);
    coordinate = r_s16(snapshot + 4);
    if (maximum + 0x280 < coordinate) return 0;
    if (coordinate < r_s16(box + 4) - 0x280) return 0;
    return 1;
}

uint32 G3MonsterMinimum5E34(uint32 values)
{
    FUNCTION_MARKER(0x00005e34, "G3L_65");
    uint32 first = r_u32(values + 8);
    uint32 second = r_u32(values + 0xc);
    return (sint32)second < (sint32)first ? second : first;
}

void G3MonsterExtendPath5E58(uint32 output, uint32 state, uint32 model, uint32 angle, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00005e58, "G3L_65");
    uint32 frame = guest_stack - 0x40;
    uint32 snapshot = r_u32(state + 0x1c);
    uint32 x = r_u16(snapshot);
    uint32 origin_x = r_u16(snapshot + 8);
    w_u16(frame + 0x10, x - origin_x);
    snapshot = r_u32(state + 0x1c);
    uint32 y = r_u16(snapshot + 2);
    uint32 origin_y = r_u16(snapshot + 0xa);
    w_u16(frame + 0x12, y - origin_y);
    snapshot = r_u32(state + 0x1c);
    uint32 z = r_u16(snapshot + 4);
    uint32 origin_z = r_u16(snapshot + 0xc);
    w_u16(frame + 0x14, z - origin_z);
    G3Function8002F9A8(frame + 0x18, 1000, frame + 0x10, frame);
    if ((sint16)angle != 0) G3Function8002F8E4(frame + 0x18, (uint32)(sint16)angle);
    x = r_u16(frame + 0x10);
    uint32 extension_x = r_u16(frame + 0x18);
    uint32 extension_y = r_u16(frame + 0x1a);
    z = r_u16(frame + 0x14);
    uint32 extension_z = r_u16(frame + 0x1c);
    x += extension_x;
    y = r_u16(frame + 0x12);
    z += extension_z;
    w_u16(frame + 0x20, x);
    w_u16(frame + 0x24, z);
    w_u16(frame + 0x22, y + extension_y);
    snapshot = r_u32(state + 0x1c);
    origin_x = r_u16(snapshot + 8);
    w_u16(output, origin_x + x);
    snapshot = r_u32(state + 0x1c);
    y = r_u16(frame + 0x22);
    origin_y = r_u16(snapshot + 0xa);
    w_u16(output + 2, origin_y + y);
    snapshot = r_u32(state + 0x1c);
    z = r_u16(frame + 0x24);
    origin_z = r_u16(snapshot + 0xc);
    w_u16(output + 4, origin_z + z);
}

void G3MonsterFriction5F78(uint32 velocity, uint32 time, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00005f78, "G3L_65");
    uint32 frame = guest_stack - 0x40;
    sint32 x = r_s8(velocity);
    w_u32(frame + 0x10, (uint32)x);
    sint32 y = r_s8(velocity + 1);
    w_u32(frame + 0x18, 0);
    w_u32(frame + 0x14, (uint32)y);
    sint32 length = (sint32)G3Function8002FBF8(frame + 0x10, frame);
    if (length == 0)
    {
        w_u8(velocity, 0);
        w_u8(velocity + 1, 0);
        return;
    }
    uint32 elapsed = (uint32)(sint16)time;
    uint32 product_x = r_u32(frame + 0x10) * elapsed;
    sint32 shifted_x = (sint32)product_x >> 3;
    sint32 quotient_x = shifted_x / length;
    uint32 product_y = r_u32(frame + 0x14) * elapsed;
    sint32 shifted_y = (sint32)product_y >> 3;
    sint32 quotient_y = shifted_y / length;
    uint32 product_z = r_u32(frame + 0x18) * elapsed;
    sint32 shifted_z = (sint32)product_z >> 3;
    sint32 quotient_z = shifted_z / length;
    w_u32(frame + 0x10, product_x);
    w_u32(frame + 0x10, (uint32)shifted_x);
    w_u32(frame + 0x14, product_y);
    w_u32(frame + 0x14, (uint32)shifted_y);
    w_u32(frame + 0x18, product_z);
    w_u32(frame + 0x18, (uint32)shifted_z);
    w_u32(frame + 0x10, (uint32)quotient_x);
    w_u32(frame + 0x14, (uint32)quotient_y);
    w_u32(frame + 0x18, (uint32)quotient_z);
    if (G3Function8002FBF8(frame + 0x10, frame) == 0)
    {
        x = r_s8(velocity);
        w_u32(frame + 0x10, x < 0 ? (uint32)-1 : 1);
        y = r_s8(velocity + 1);
        w_u32(frame + 0x14, y < 0 ? (uint32)-1 : 1);
    }
    uint32 x_bits = (uint32)r_s8(velocity);
    uint32 adjustment = r_u32(frame + 0x10);
    x = (sint32)(x_bits - adjustment);
    w_u32(frame + 0x20, (uint32)x);
    uint32 y_bits = (uint32)r_s8(velocity + 1);
    adjustment = r_u32(frame + 0x14);
    y = (sint32)(y_bits - adjustment);
    w_u32(frame + 0x24, (uint32)y);
    if (x < -127) x = -127;
    if (x > 127) x = 127;
    w_u32(frame + 0x20, (uint32)x);
    if (y < -127) y = -127;
    if (y > 127) y = 127;
    sint32 z = r_s32(frame + 0x28);
    w_u32(frame + 0x24, (uint32)y);
    if (z < -127) z = -127;
    if (z > 127) z = 127;
    uint32 byte = r_u8(frame + 0x20);
    w_u32(frame + 0x28, (uint32)z);
    w_u8(velocity, byte);
    w_u8(velocity + 1, r_u8(frame + 0x24));
}

void G3MonsterVelocityDisplacement613C(uint32 output, uint32 velocity, uint32 time, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000613c, "G3L_65");
    sint32 elapsed = (sint16)time;
    sint32 x = r_s8(velocity);
    uint32 frame = guest_stack - 0x10;
    w_u32(frame, (uint32)x);
    sint32 product_x = x * elapsed;
    sint32 y = r_s8(velocity + 1);
    sint32 product_y = y * elapsed;
    w_u32(frame + 8, 0);
    w_u32(frame + 8, 0);
    w_u32(frame + 8, 0);
    w_u32(frame + 4, (uint32)y);
    x = product_x >> 6;
    w_u32(frame, (uint32)product_x);
    w_u32(frame, (uint32)x);
    w_u32(frame + 4, (uint32)product_y);
    w_u32(frame + 4, (uint32)(product_y >> 6));
    if (x < -32767) x = -32767;
    if (x > 32767) x = 32767;
    w_u16(output, (uint32)x);
    y = r_s32(frame + 4);
    if (y < -32767) y = -32767;
    if (y > 32767) y = 32767;
    w_u16(output + 2, (uint32)y);
    sint32 z = r_s32(frame + 8);
    if (z < -32767) z = -32767;
    if (z > 32767) z = 32767;
    w_u16(output + 4, (uint32)z);
}

void G3MonsterYawDisplacement6218(uint32 output, uint32 state, uint32 time, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006218, "G3L_65");
    uint32 frame = guest_stack - 0x28;
    G3MonsterSelectAngles6AD4(frame + 0x10, frame + 0x12, state);
    uint32 animation = r_u32(state + 0x14);
    sint32 divisor = r_s16(animation + 8);
    sint32 ratio = divisor == 0 ? 0 : 0x1000 / divisor;
    sint32 limit = (sint16)((sint16)ratio * (sint16)time / 40);
    sint32 target = r_s16(frame + 0x10);
    sint32 candidate = -limit;
    if (candidate < target) candidate = target;
    if (candidate < limit) limit = candidate;
    w_u16(output, 0);
    w_u16(output + 2, 0);
    w_u16(output + 4, (uint32)limit);
}

uint32 G3MonsterPositiveHalf62FC(uint32 value)
{
    FUNCTION_MARKER(0x000062fc, "G3L_65");
    return (sint32)(value << 16) > 0 ? 1 : 0;
}

uint32 G3MonsterCheckDisplacement6308(uint32 distance, uint32 table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006308, "G3L_65");
    uint32 frame = guest_stack - 0x40;
    w_u8(frame + 0x22, 6);
    G3MonsterLookupMotion2784(frame + 0x20, frame + 0x10, table, frame);
    uint32 length = G3Function8002FB10(frame + 0x10);
    sint32 threshold = (sint32)(0 - length - 100);
    return (sint16)distance < threshold ? 1 : 0;
}

uint32 G3MonsterSelectCommand635C(uint32 angle, uint32 distance, uint32 threshold, uint32 model, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000635c, "G3L_65");
    uint32 frame = guest_stack - 0x28;
    uint32 difference = distance - threshold;
    uint32 previous = r_u32(guest_stack + 0x10);
    uint32 command = G3MonsterSelectTurn6DD4(5, (uint32)(sint16)angle, model, frame) & 0xff;
    if (command == 5)
    {
        uint32 narrowed = (uint32)(sint16)difference;
        if (G3MonsterPositiveHalf62FC(narrowed) == 0)
        {
            if (G3MonsterCheckDisplacement6308(narrowed, model, frame) != 0) command = 6;
            else command = (threshold & 0xffff) == 0 ? 5 : 0;
        }
    }
    if (previous != 5 && previous != 0x18 && command == 5)
    {
        if (G3MonsterLookup23EC(0x18, 0, 0, model) != 0) command = 0x18;
    }
    else if (previous != 0 && previous != 0x19 && command == 0)
    {
        if (G3MonsterLookup23EC(0x19, 0, 0, model) != 0) command = 0x19;
    }
    return (uint32)(sint8)command;
}

void G3MonsterUpdatePath6488(uint32 state, uint32 model, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006488, "G3L_65");
    uint32 frame = guest_stack - 0x20;
    uint32 settings = r_u32(model + 0x10);
    uint32 kind = r_u32(settings + 8);
    if (kind == 1) G3MonsterExtendPath5E58(state + 0x24, state, model, 0, frame);
    else if (kind == 2)
    {
        sint32 random = (sint32)G3SdkRandom80027FA8();
        sint32 angle = (random % 2 * 2 - 1) * 0x2aa;
        G3MonsterExtendPath5E58(state + 0x24, state, model, (uint32)(sint16)angle, frame);
    }
    else
    {
        uint32 snapshot = r_u32(state + 0x1c);
        G3MonsterTurnToward6F44(snapshot + 0xe, state + 0x24, settings, 4000, frame);
    }
}

uint32 G3MonsterConstant6550()
{
    FUNCTION_MARKER(0x00006550, "G3L_65");
    return 0xb;
}

uint32 G3MonsterSelectValue6558(uint32 object)
{
    FUNCTION_MARKER(0x00006558, "G3L_65");
    uint32 resource = r_u32(object + 0xc);
    if (resource == 0) return 6;
    return r_u32(resource + 8) == 1 ? 7 : 6;
}

uint32 G3MonsterSelectDistance6594(uint32 kind, uint32 previous, uint32 mode, uint32 model, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006594, "G3L_65");
    uint32 settings = r_u32(model + 0xc);
    bool close = kind == 6 || (kind == 8 && previous == 6);
    if (!close && kind != 7 && (kind != 8 || previous != 7))
    {
        if (kind != 5 || mode != 2) return 0;
        close = G3MonsterSelectValue6558(model) == 6;
    }
    if (close) return settings == 0 ? 200 : r_u16(settings);
    return settings == 0 ? 1200 : r_u16(settings + 4);
}

void G3MonsterMeasureDistances6638(uint32 output, uint32 origin, uint32 first, uint32 target, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006638, "G3L_65");
    uint32 frame = guest_stack - 0x40;
    uint32 peer = r_u32(guest_stack + 0x10);
    uint32 last = r_u32(guest_stack + 0x14);
    if (first != 0 && origin != 0)
    {
        sint32 x = r_s16(first);
        sint32 other_x = r_s16(origin);
        w_u32(frame + 0x10, (uint32)(x - other_x));
        sint32 y = r_s16(first + 2);
        sint32 other_y = r_s16(origin + 2);
        w_u32(frame + 0x18, 0);
        w_u32(frame + 0x14, (uint32)(y - other_y));
        sint32 distance = (sint32)G3Function8002FBF8(frame + 0x10, frame);
        if (distance < 0) distance = 0;
        if (distance > 0x10000) distance = 0x10000;
        w_u16(output + 0, (uint32)distance);
    }
    if (target != 0 && origin != 0)
    {
        sint32 x = r_s16(target);
        sint32 other_x = r_s16(origin);
        w_u32(frame + 0x10, (uint32)(x - other_x));
        sint32 y = r_s16(target + 2);
        sint32 other_y = r_s16(origin + 2);
        w_u32(frame + 0x18, 0);
        w_u32(frame + 0x14, (uint32)(y - other_y));
        sint32 distance = (sint32)G3Function8002FBF8(frame + 0x10, frame);
        if (distance < 0) distance = 0;
        if (distance > 0x10000) distance = 0x10000;
        w_u16(output + 4, (uint32)distance);
    }
    if (first != 0 && peer != 0)
    {
        sint32 x = r_s16(first);
        sint32 other_x = r_s16(peer);
        w_u32(frame + 0x10, (uint32)(x - other_x));
        sint32 y = r_s16(first + 2);
        sint32 other_y = r_s16(peer + 2);
        w_u32(frame + 0x18, 0);
        w_u32(frame + 0x14, (uint32)(y - other_y));
        sint32 distance = (sint32)G3Function8002FBF8(frame + 0x10, frame);
        if (distance < 0) distance = 0;
        if (distance > 0x10000) distance = 0x10000;
        w_u16(output + 8, (uint32)distance);
    }
    if (origin != 0 && peer != 0)
    {
        sint32 x = r_s16(origin);
        sint32 other_x = r_s16(peer);
        w_u32(frame + 0x10, (uint32)(x - other_x));
        sint32 y = r_s16(origin + 2);
        sint32 other_y = r_s16(peer + 2);
        w_u32(frame + 0x18, 0);
        w_u32(frame + 0x14, (uint32)(y - other_y));
        sint32 distance = (sint32)G3Function8002FBF8(frame + 0x10, frame);
        if (distance < 0) distance = 0;
        if (distance > 0x10000) distance = 0x10000;
        w_u16(output + 2, (uint32)distance);
        uint32 peer_z = r_u16(peer + 4);
        uint32 origin_z = r_u16(origin + 4);
        w_u16(output + 0xa, peer_z - origin_z);
    }
    if (origin == 0) return;
    if (origin != 0 && last != 0)
    {
        sint32 x = r_s16(origin);
        sint32 other_x = r_s16(last);
        w_u32(frame + 0x10, (uint32)(x - other_x));
        sint32 y = r_s16(origin + 2);
        sint32 other_y = r_s16(last + 2);
        w_u32(frame + 0x18, 0);
        w_u32(frame + 0x14, (uint32)(y - other_y));
        sint32 distance = (sint32)G3Function8002FBF8(frame + 0x10, frame);
        if (distance < 0) distance = 0;
        if (distance > 0x10000) distance = 0x10000;
        w_u16(output + 6, (uint32)distance);
    }
}

void G3MonsterMeasureAngles6880(uint32 output, uint32 heading, uint32 yaw, uint32 origin, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006880, "G3L_65");
    uint32 frame = guest_stack - 0x40;
    uint32 first = r_u32(guest_stack + 0x10);
    uint32 second = r_u32(guest_stack + 0x14);
    uint32 third = r_u32(guest_stack + 0x18);
    uint32 fourth = r_u32(guest_stack + 0x1c);
    uint32 adjusted_yaw = yaw - 0x400;
    if (first != 0 && origin != 0)
    {
        uint32 x = r_u16(first);
        uint32 origin_x = r_u16(origin);
        uint32 delta_x = x - origin_x;
        w_u16(frame + 0x10, delta_x);
        uint32 y = r_u16(first + 2);
        uint32 origin_y = r_u16(origin + 2);
        uint32 delta_y = y - origin_y;
        w_u16(frame + 0x12, delta_y);
        uint32 z = r_u16(first + 4);
        uint32 origin_z = r_u16(origin + 4);
        w_u16(frame + 0x14, z - origin_z);
        uint32 angle = G3Function8002F0C4((uint32)(sint16)delta_y, (uint32)(sint16)delta_x, frame);
        uint32 difference = G3Function80038B48((uint32)(sint16)adjusted_yaw, (uint32)(sint16)angle);
        w_u16(output + 0, difference);
        difference = G3Function80038B48((uint32)(sint16)heading, (uint32)(sint16)(angle + 0x800));
        w_u16(output + 0xa, difference);
    }
    if (second != 0 && origin != 0)
    {
        uint32 x = r_u16(second);
        uint32 origin_x = r_u16(origin);
        uint32 delta_x = x - origin_x;
        w_u16(frame + 0x10, delta_x);
        uint32 y = r_u16(second + 2);
        uint32 origin_y = r_u16(origin + 2);
        uint32 delta_y = y - origin_y;
        w_u16(frame + 0x12, delta_y);
        uint32 z = r_u16(second + 4);
        uint32 origin_z = r_u16(origin + 4);
        w_u16(frame + 0x14, z - origin_z);
        uint32 angle = G3Function8002F0C4((uint32)(sint16)delta_y, (uint32)(sint16)delta_x, frame);
        uint32 difference = G3Function80038B48((uint32)(sint16)adjusted_yaw, (uint32)(sint16)angle);
        w_u16(output + 2, difference);
    }
    if (third != 0 && origin == 0) return;
    if (third != 0 && origin != 0)
    {
        uint32 x = r_u16(third);
        uint32 origin_x = r_u16(origin);
        uint32 delta_x = x - origin_x;
        w_u16(frame + 0x10, delta_x);
        uint32 y = r_u16(third + 2);
        uint32 origin_y = r_u16(origin + 2);
        uint32 delta_y = y - origin_y;
        w_u16(frame + 0x12, delta_y);
        uint32 z = r_u16(third + 4);
        uint32 origin_z = r_u16(origin + 4);
        w_u16(frame + 0x14, z - origin_z);
        uint32 angle = G3Function8002F0C4((uint32)(sint16)delta_y, (uint32)(sint16)delta_x, frame);
        uint32 difference = G3Function80038B48((uint32)(sint16)adjusted_yaw, (uint32)(sint16)angle);
        w_u16(output + 4, difference);
    }
    if (fourth != 0 && origin != 0)
    {
        uint32 x = r_u16(fourth);
        uint32 origin_x = r_u16(origin);
        uint32 delta_x = x - origin_x;
        w_u16(frame + 0x10, delta_x);
        uint32 y = r_u16(fourth + 2);
        uint32 origin_y = r_u16(origin + 2);
        uint32 delta_y = y - origin_y;
        w_u16(frame + 0x12, delta_y);
        uint32 z = r_u16(fourth + 4);
        uint32 origin_z = r_u16(origin + 4);
        w_u16(frame + 0x14, z - origin_z);
        uint32 angle = G3Function8002F0C4((uint32)(sint16)delta_y, (uint32)(sint16)delta_x, frame);
        uint32 difference = G3Function80038B48((uint32)(sint16)adjusted_yaw, (uint32)(sint16)angle);
        w_u16(output + 6, difference);
    }
}

void G3MonsterSelectAngles6AD4(uint32 first, uint32 second, uint32 state)
{
    FUNCTION_MARKER(0x00006ad4, "G3L_65");
    uint32 snapshot = r_u32(state + 0x1c);
    uint32 mode = r_u8(state + 1);
    bool directed = mode - 1 < 2;
    if (!directed && (sint8)mode == 0xb)
    {
        sint32 command = r_s8(state + 2);
        directed = command != 0x12 && command != 0xe;
    }
    if (directed)
    {
        w_u16(first, r_u16(snapshot + 0x24));
        w_u16(second, r_u16(snapshot + 0x18));
        return;
    }
    mode = r_u8(state + 1);
    if (mode - 4 < 2)
    {
        if (r_s8(state + 0x48) != 0)
        {
            w_u16(first, r_u16(snapshot + 0x26));
            w_u16(second, r_u16(snapshot + 0x1a));
            return;
        }
        if ((sint8)mode != 5)
        {
            if ((sint8)mode != 4) return;
            w_u16(first, r_u16(snapshot + 0x22));
            w_u16(second, r_u16(snapshot + 0x14));
            return;
        }
    }
    else if ((sint8)mode == 0xc)
    {
        w_u16(first, r_u16(snapshot + 0x22));
        w_u16(second, r_u16(snapshot + 0x14));
        return;
    }
    w_u16(first, r_u16(snapshot + 0x20));
    w_u16(second, r_u16(snapshot + 0x16));
}

void G3MonsterUpdateTarget6BD8(uint32 object, uint32 state, uint32 snapshot, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006bd8, "G3L_65");
    uint32 frame = guest_stack - 0x40;
    w_u16(frame + 0x20, r_u16(object + 0x60));
    w_u16(frame + 0x22, r_u16(object + 0x62));
    w_u16(frame + 0x24, r_u16(object + 0x64));
    uint32 current = r_u32(state + 0x1c);
    uint32 axis = state + 0x24;
    w_u32(frame + 0x10, snapshot);
    w_u32(frame + 0x14, output);
    G3MonsterMeasureDistances6638(current + 0x14, current + 8, current + 0xe, axis, frame);
    current = r_u32(state + 0x1c);
    uint32 yaw = (uint32)r_s16(frame + 0x24);
    uint32 heading = (uint32)r_s16(current + 6);
    uint32 angles = current + 0x20;
    w_u32(frame + 0x10, snapshot);
    uint32 next = r_u32(state + 0x1c);
    w_u32(frame + 0x18, axis);
    w_u32(frame + 0x1c, output);
    w_u32(frame + 0x14, next + 0xe);
    G3MonsterMeasureAngles6880(angles, heading, yaw, current + 8, frame);
}

uint32 G3MonsterMotionDistance6C84(uint32 kind, uint32 table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006c84, "G3L_65");
    uint32 frame = guest_stack - 0x50;
    w_u8(frame + 0x22, kind);
    G3MonsterLookupMotion2784(frame + 0x20, frame + 0x10, table, frame);
    uint32 length = G3Function8002FB10(frame + 0x10);
    uint32 second = 0;
    if (kind == 5)
    {
        w_u8(frame + 0x22, kind);
        G3MonsterLookupMotion2784(frame + 0x20, frame + 0x10, table, frame);
        second = G3Function8002FB10(frame + 0x10);
    }
    return (uint32)(sint16)(length + second);
}

uint32 G3MonsterCanTurn6D20(uint32 kind, uint32 angle, uint32 table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006d20, "G3L_65");
    uint32 frame = guest_stack - 0x30;
    uint32 animation = G3MonsterLookup23EC(kind, 0, 0, table);
    if (animation == 0) return 0;
    G3MonsterAnimationMotion271C(animation, frame + 0x10, frame);
    sint32 target = (sint16)angle;
    sint32 step = r_s16(frame + 0x1c);
    sint32 magnitude = target < 0 ? -target : target;
    sint32 step_magnitude = step < 0 ? -step : step;
    if (step_magnitude >= magnitude) return 0;
    return (target < 0) == (step < 0) ? 1 : 0;
}

uint32 G3MonsterSelectTurn6DD4(uint32 kind, uint32 angle, uint32 table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006dd4, "G3L_65");
    uint32 frame = guest_stack - 0x28;
    uint32 first;
    uint32 second;
    if (kind == 5)
    {
        first = 7;
        second = 8;
    }
    else if (kind == 1)
    {
        first = 9;
        second = 10;
    }
    else if (kind == 0xf)
    {
        first = 0x10;
        second = 0x11;
    }
    else return kind & 0xff;
    uint32 narrowed_angle = (uint32)(sint16)angle;
    if (G3MonsterCanTurn6D20(first, narrowed_angle, table, frame) != 0) return first & 0xff;
    if (G3MonsterCanTurn6D20(second, narrowed_angle, table, frame) != 0) return second & 0xff;
    return kind & 0xff;
}

void G3MonsterSelectHeading6E98(uint32 output, uint32 origin, uint32 target, uint32 minimum, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006e98, "G3L_65");
    uint32 frame = guest_stack - 0x30;
    uint32 x = r_u16(target);
    uint32 origin_x = r_u16(origin);
    w_u16(frame + 0x10, x - origin_x);
    uint32 y = r_u16(target + 2);
    uint32 origin_y = r_u16(origin + 2);
    w_u16(frame + 0x12, y - origin_y);
    uint32 origin_z = r_u16(origin + 4);
    uint32 z = r_u16(target + 4);
    w_u16(frame + 0x14, z - origin_z);
    G3Function8002F9A8(frame + 0x18, (uint32)(sint16)minimum, frame + 0x10, frame);
    x = r_u16(target);
    uint32 delta = r_u16(frame + 0x18);
    w_u16(output, x + delta);
    y = r_u16(target + 2);
    delta = r_u16(frame + 0x1a);
    w_u16(output + 2, y + delta);
    z = r_u16(target + 4);
    delta = r_u16(frame + 0x1c);
    w_u16(output + 4, z + delta);
}

uint32 G3MonsterTurnToward6F44(uint32 target, uint32 origin, uint32 settings, uint32 minimum, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006f44, "G3L_65");
    uint32 frame = guest_stack - 0x28;
    uint32 distance = (sint32)minimum < 301 ? 0 : minimum - 300;
    sint32 random = (sint32)G3SdkRandom80027FA8();
    sint32 angle = random % 4096;
    w_u16(frame + 0x10, 0);
    w_u16(frame + 0x12, 0);
    w_u16(frame + 0x14, 0);
    w_u16(frame + 0x10, distance);
    G3Function8002F8E4(frame + 0x10, (uint32)(sint16)angle);
    uint32 position = r_u16(target);
    uint32 delta = r_u16(frame + 0x10);
    w_u16(origin, position + delta);
    position = r_u16(target + 2);
    delta = r_u16(frame + 0x12);
    w_u16(origin + 2, position + delta);
    position = r_u16(target + 4);
    delta = r_u16(frame + 0x14);
    w_u16(origin + 4, position + delta);
    return 0;
}

uint32 G3MonsterStepDisplacement700C(uint32 time, uint32 rotation, uint32 position, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000700c, "G3L_65");
    uint32 frame = guest_stack - 0x38;
    uint32 animation = r_u32(state + 0x14);
    uint32 scale = (uint32)r_s16(state + 0x46);
    uint32 object = r_u32(guest_stack + 0x10);
    uint32 result = G3MonsterAdvanceBounded244C(time, state + 0x20, animation, scale);
    animation = r_u32(state + 0x14);
    G3MonsterSampleMotion2524(animation, 40, frame + 0x10, frame);
    G3MonsterScaleVector26A0(frame + 0x10, object + 0x68);
    sint32 x = r_s16(frame + 0x10);
    sint32 factor = r_s16(state + 0x44);
    x = x * factor / 100;
    sint32 y = r_s16(frame + 0x12);
    w_u16(frame + 0x10, (uint32)x);
    factor = r_s16(state + 0x44);
    y = y * factor / 100;
    sint32 z = r_s16(frame + 0x14);
    w_u16(frame + 0x12, (uint32)y);
    factor = r_s16(state + 0x44);
    w_u16(frame + 0x14, (uint32)(z * factor / 100));
    uint32 angle = (uint32)r_s16(object + 0x64);
    G3Function8002F8E4(frame + 0x10, angle);
    w_u16(rotation, r_u16(frame + 0x18));
    w_u16(rotation + 2, r_u16(frame + 0x1a));
    w_u16(rotation + 4, r_u16(frame + 0x1c));
    w_u16(position, r_u16(frame + 0x10));
    w_u16(position + 2, r_u16(frame + 0x12));
    w_u16(position + 4, r_u16(frame + 0x14));
    return result;
}

uint32 G3MonsterStep7178(uint32 time, uint32 object, uint32 state, uint32 mode, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007178, "G3L_65");
    uint32 frame = guest_stack - 0x50;
    if (r_u32(state + 0x14) == 0) return 1;
    w_u32(frame + 0x10, object);
    uint32 result = G3MonsterStepDisplacement700C(time, frame + 0x28, frame + 0x18, state, frame);
    if (r_s16(state + 0x40) == 0) G3MonsterFriction5F78(state + 0x22, 40, frame);
    G3MonsterVelocityDisplacement613C(frame + 0x20, state + 0x22, 40, frame);
    uint32 snapshot = r_u32(state + 0x1c);
    r_s16(snapshot + 0x2e);
    G3MonsterYawDisplacement6218(frame + 0x30, state, 40, frame);
    if (r_s16(state + 0x40) != 0)
    {
        w_u16(frame + 0x18, 0);
        w_u16(frame + 0x1a, 0);
        w_u16(frame + 0x1c, 0);
    }
    if (mode != 1)
    {
        uint32 position = r_u16(object + 0x58);
        uint32 delta = r_u16(frame + 0x18);
        w_u16(object + 0x58, position + delta);
        position = r_u16(object + 0x5a);
        delta = r_u16(frame + 0x1a);
        w_u16(object + 0x5a, position + delta);
        position = r_u16(object + 0x58);
        delta = r_u16(frame + 0x20);
        w_u16(object + 0x58, position + delta);
        position = r_u16(object + 0x5a);
        delta = r_u16(frame + 0x22);
        w_u16(object + 0x5a, position + delta);
        position = r_u16(object + 0x5c);
        delta = r_u16(frame + 0x24);
        w_u16(object + 0x5c, position + delta);
    }
    uint32 heading = r_u16(object + 0x64);
    uint32 rotation = r_u16(frame + 0x2c);
    heading += rotation;
    w_u16(object + 0x64, heading);
    rotation = r_u16(frame + 0x34);
    w_u16(object + 0x64, heading + rotation);
    return result;
}

void G3MonsterNoop72D0()
{
    FUNCTION_MARKER(0x000072d0, "G3L_65");
}

uint32 G3MonsterCopyVector72D8(uint32 destination, uint32 source)
{
    FUNCTION_MARKER(0x000072d8, "G3L_65");
    w_u16(destination, r_u16(source));
    w_u16(destination + 2, r_u16(source + 2));
    w_u16(destination + 4, r_u16(source + 4));
    return 2;
}

uint32 G3MonsterConstant7304()
{
    FUNCTION_MARKER(0x00007304, "G3L_65");
    return 1;
}

uint32 G3MonsterMatchBounds730C(uint32 bounds, uint32 values)
{
    FUNCTION_MARKER(0x0000730c, "G3L_65");
    uint32 distance = r_u16(values);
    sint32 minimum = r_s16(bounds);
    if ((sint32)distance < minimum) return 0;
    if (r_s16(bounds + 2) < (sint32)distance) return 0;
    minimum = r_s16(bounds + 4);
    sint32 angle = r_s16(values + 4);
    if (angle < minimum * 11) return 0;
    if (r_s16(bounds + 6) * 11 < angle) return 0;
    sint32 offset = r_s16(values + 2);
    minimum = r_s16(bounds + 8);
    if (offset < minimum) return 0;
    return r_s16(bounds + 0xa) < offset ? 0 : 1;
}

uint32 G3MonsterMatchFlags73B8(uint32 resource, uint32 state)
{
    FUNCTION_MARKER(0x000073b8, "G3L_65");
    if ((r_u16(resource + 8) & 1) != 0 && (r_u32(state + 0x18) & 0x400) != 0) return 1;
    if ((r_u16(resource + 8) & 2) == 0) return 0;
    return (r_u32(state + 0x18) & 4) != 0 ? 1 : 0;
}

uint32 G3MonsterMatchGroup7414(uint32 resource, uint32 values, uint32 state, uint32 kind, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007414, "G3L_65");
    uint32 previous = r_u32(guest_stack + 0x10);
    if (resource == 0) return 0;
    if ((kind & 1) != 0 && (r_u16(resource + 2) & 1) == 0) return 0;
    if ((kind & 8) != 0 && (r_u16(resource + 2) & 8) == 0) return 0;
    if (previous == 6 && (r_u16(resource + 2) & 2) != 0) return 0;
    if (previous == 7 && (r_u16(resource + 2) & 2) == 0) return 0;
    if ((r_u16(resource + 2) & 4) != 0 && (r_u32(state + 0x18) & 0x80000) == 0) return 0;
    sint32 type = r_s16(resource);
    if (type == 0) return G3MonsterMatchBounds730C(resource + 8, values);
    if (type == 2) return G3MonsterMatchFlags73B8(resource, state);
    return 0;
}

uint32 G3MonsterFindGroup751C(uint32 output, uint32 values, uint32 model, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000751c, "G3L_65");
    uint32 frame = guest_stack - 0x40;
    uint32 table = r_u32(model + 8);
    uint32 kind = r_u32(guest_stack + 0x10);
    uint32 previous = r_u32(guest_stack + 0x14);
    if (table == 0) return 0;
    uint32 index = 0;
    uint32 count = 0;
    uint32 group = r_u32(table);
    if (group == 0) return 0;
    group = r_u32(table);
    for (;;)
    {
        w_u32(frame + 0x10, previous);
        uint32 resource = r_u32(group);
        uint32 result = G3MonsterMatchGroup7414(resource, values, state, kind, frame);
        table += 4;
        if (result != 0)
        {
            if (output != 0) w_u8(output, index);
            return 1;
        }
        ++count;
        ++index;
        if ((sint16)count >= 10) return 0;
        group = r_u32(table);
        if (group == 0) return 0;
    }
}

uint32 G3MonsterCountGroup7618(uint32 index, uint32 model)
{
    FUNCTION_MARKER(0x00007618, "G3L_65");
    uint32 table = r_u32(model + 8);
    if (table == 0) return 0;
    uint32 group = r_u32(table + index * 4);
    if (group == 0) return 0;
    uint32 entry = group + 8;
    uint32 value = r_u32(entry);
    uint32 count = 0;
    while (value != 0)
    {
        entry += 4;
        value = r_u32(entry);
        ++count;
    }
    return count;
}

uint32 G3MonsterFindTrigger7670(uint32 kind, uint32 animation)
{
    FUNCTION_MARKER(0x00007670, "G3L_65");
    if (animation == 0) return 0;
    uint32 trigger = r_u32(animation + 0x14);
    for (uint32 index = 0; trigger != 0 && index < 20; ++index)
    {
        if ((uint32)r_s16(trigger) == kind) return trigger;
        trigger = r_u32(trigger + 4);
    }
    return 0;
}

void G3MonsterSpawnTrigger76C4(uint32 trigger, uint32 object, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000076c4, "G3L_65");
    uint32 frame = guest_stack - 0x38;
    sint32 count = r_s16(trigger + 0xe);
    if (count <= 0) count = 1;
    uint32 resource = G3Function8003AA48(trigger + 0x10);
    if (resource == 0) return;
    for (sint32 index = 0; index < count; ++index)
    {
        uint32 child = G3Function8002A454(object, resource, frame);
        if (child != 0)
        {
            uint32 heading = r_u16(object + 0x64);
            uint32 x = r_u16(trigger + 8);
            heading -= 0x400;
            w_u16(frame + 0x10, x);
            w_u16(frame + 0x12, r_u16(trigger + 0xa));
            w_u16(frame + 0x14, r_u16(trigger + 0xc));
            G3Function8002F8E4(frame + 0x10, (uint32)(sint16)heading);
            uint32 position = r_u16(child + 0x58);
            uint32 delta = r_u16(frame + 0x10);
            w_u16(child + 0x58, position + delta);
            position = r_u16(child + 0x5a);
            delta = r_u16(frame + 0x12);
            w_u16(child + 0x5a, position + delta);
            position = r_u16(child + 0x5c);
            delta = r_u16(frame + 0x14);
            w_u16(child + 0x5c, position + delta);
            x = r_u16(child + 0x58);
            uint32 y = r_u16(child + 0x5a);
            uint32 z = r_u16(child + 0x5c);
            w_u16(child + 0x88, x);
            w_u16(child + 0x8a, y);
            w_u16(child + 0x8c, z);
        }
    }
    uint32 counter = r_u8(state + 8);
    if (counter != 0xff) w_u8(state + 8, counter + 1);
}

void G3MonsterPlayerFlag7808(uint32 trigger)
{
    FUNCTION_MARKER(0x00007808, "G3L_65");
    sint32 enabled = r_s16(trigger + 8);
    uint32 player = r_u32(0x8009efc4);
    uint32 flags = r_u32(player + 0x10);
    w_u32(player + 0x150, enabled != 0 ? 7 : 0);
    w_u32(player + 0x10, enabled != 0 ? flags | 0x800 : flags & 0xfffff7ff);
}

void G3MonsterPlayerDamage7858(uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007858, "G3L_65");
    uint32 frame = guest_stack - 0x30;
    uint32 player = r_u32(0x8009efc4);
    uint32 model = r_u32(player + 0x24);
    w_u16(model + 0x120, 0);
    w_u32(player + 0x154, r_u32(player + 0x154) & 0x39bfffff);
    G3MonsterInitializePacket31F8(frame + 0x10);
    player = r_u32(0x8009efc4);
    w_u16(frame + 0x20, 3);
    uint32 target = r_u32(player + 0xec);
    if (target != 0) G3DispatchStateEvent(target, player, r_u32(0x80097048), 8, frame + 0x10, frame);
}

void G3MonsterApplyTriggerFlags78CC(uint32 trigger, uint32 state)
{
    FUNCTION_MARKER(0x000078cc, "G3L_65");
    uint32 flags = r_u16(trigger + 8);
    uint32 mask;
    if ((flags & 2) != 0) mask = 0x800000;
    else if ((flags & 1) != 0) mask = 0x400000;
    else if ((flags & 4) != 0) mask = 0x100000;
    else return;
    w_u32(state + 0x18, r_u32(state + 0x18) | mask);
}

void G3MonsterTriggerProperty7920(uint32 trigger, uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007920, "G3L_65");
    uint32 frame = guest_stack - 0x18;
    G3MonsterRunProperty0EB4(object, trigger + 8, module_base, frame);
}

uint32 G3MonsterCrossedFrame7948(uint32 previous, uint32 current, uint32 trigger_time, uint32 animation)
{
    FUNCTION_MARKER(0x00007948, "G3L_65");
    sint32 before = (sint16)previous;
    sint32 after = (sint16)current;
    sint32 start = r_s16(animation + 2) * 40;
    sint32 end = r_s16(animation + 4) * 40;
    sint32 time = (sint16)trigger_time;
    if (before < after && time <= after)
    {
        if (before < time) return 1;
        if ((sint16)start == time && before == (sint16)start) return 1;
        if (time < (sint16)start) return 1;
    }
    if (after >= before || time < after) return 0;
    if (time == (sint16)end) return before >= time ? 1 : 0;
    return time < before ? 1 : 0;
}

void G3MonsterTriggerFrame7A3C(uint32 previous, uint32 current, uint32 trigger, uint32 animation, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007a3c, "G3L_65");
    uint32 frame = guest_stack - 0x20;
    sint32 trigger_frame = r_s16(trigger + 2);
    uint32 object = r_u32(guest_stack + 0x10);
    uint32 time = (uint32)(sint16)(trigger_frame * 40);
    if (G3MonsterCrossedFrame7948((uint32)(sint16)previous, (uint32)(sint16)current, time, animation) == 0) return;
    sint32 kind = r_s16(trigger);
    switch (kind)
    {
    case 1: G3MonsterSpawnTrigger76C4(trigger, object, object + 0x104, frame); break;
    case 3: G3MonsterPlayerFlag7808(trigger); break;
    case 4: G3MonsterPlayerDamage7858(frame); break;
    case 5: G3MonsterApplyTriggerFlags78CC(trigger, object + 0x104); break;
    case 6: G3MonsterTriggerProperty7920(trigger, object, module_base, frame); break;
    }
}

void G3MonsterTriggerFrames7B2C(uint32 previous, uint32 current, uint32 animation, uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007b2c, "G3L_65");
    uint32 frame = guest_stack - 0x38;
    uint32 trigger = 0;
    uint32 count = 0;
    if (animation != 0) trigger = r_u32(animation + 0x14);
    while (trigger != 0 && count < 20)
    {
        w_u32(frame + 0x10, object);
        G3MonsterTriggerFrame7A3C((uint32)(sint16)previous, (uint32)(sint16)current, trigger, animation, module_base, frame);
        trigger = r_u32(trigger + 4);
        ++count;
    }
}

void G3MonsterNoop7BC8()
{
    FUNCTION_MARKER(0x00007bc8, "G3L_65");
}

uint32 G3MorphFaceEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_04");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3MorphFaceInitialize(object, game, frame);
        return 1;
    case 2:
        G3MorphFaceUpdate(object, game, frame);
        return 1;
    case 3:
        G3MorphFaceCollide(object, game, frame);
        return 1;
    default:
        return 0;
    }
}

void G3MorphFaceConfigure(uint32 object, uint32 properties, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000007c, "G3L_04");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    w_u32(frame + 0x34, properties);
    if (properties == 0)
        return;
    w_u16(state + 0x14, 0);
    while (G3Function80085DFC(frame + 0x34, frame + 0x10, frame + 0x14, frame + 0x18) != 0)
    {
        sint32 kind = r_s16(frame + 0x10);
        if (kind == 1)
        {
            if (r_s16(frame + 0x18) >= 4)
            {
                uint32 data = r_u32(frame + 0x14);
                uint32 value = (uint32)r_s16(data);
                w_u32(state, value * value);
                value = (uint32)r_s16(data + 2);
                w_u32(state + 4, value * value);
            }
        }
        else if (kind == 0x22)
            w_u16(state + 0x14, 1);
        else if (kind == 0x23)
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0xe, r_u16(data));
        }
        else if (kind == 0x24)
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0x10, r_u16(data));
        }
    }
}

void G3MorphFaceInitialize(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000188, "G3L_04");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    w_u32(state, 0x19c990);
    w_u32(state + 4, 0xa62b10);
    w_u16(state + 0xe, 60);
    w_u16(state + 0x10, 0);
    w_u16(state + 0x14, 0);
    uint32 resource = r_u32(object + 0x1c);
    G3MorphFaceConfigure(object, r_u32(resource + 0x1c), frame);
    G3MorphFaceConfigure(object, r_u32(object + 0x28), frame);
    uint32 intro = r_u32(object + 0x20);
    uint32 descriptor = r_u32(intro);
    uint32 models = r_u32(descriptor + 0xc);
    uint32 model = r_u32(models);
    sint32 count = r_s16(model);
    sint32 index = 0;
    while (index < count)
    {
        descriptor = r_u32(intro);
        models = r_u32(descriptor + 0xc);
        model = r_u32(models);
        uint32 vertices = r_u32(model + 8);
        w_u16(vertices + ((uint32)index << 3) + 6, 0x5d);
        intro = r_u32(object + 0x20);
        descriptor = r_u32(intro);
        models = r_u32(descriptor + 0xc);
        model = r_u32(models);
        count = r_s16(model);
        ++index;
    }
    w_u32(object + 0x10, r_u32(object + 0x10) | 0x100000);
    w_u32(state + 8, 0);
    w_u16(state + 0xc, 0);
    resource = r_u32(object + 0x1c);
    uint32 animations = r_u32(resource + 0x10);
    uint32 animation = r_u32(animations);
    w_u16(state + 0x16, r_u16(animation + 2) - 1);
    uint32 player = r_u32(game + 0x50);
    uint32 x = r_u16(object + 0x58);
    uint32 px = r_u16(player + 0x58);
    uint32 dx = x - px;
    uint32 sx = (uint32)(sint32)(sint16)dx;
    uint32 xx = sx * sx;
    w_u16(frame + 0x10, dx);
    player = r_u32(game + 0x50);
    uint32 y = r_u16(object + 0x5a);
    uint32 py = r_u16(player + 0x5a);
    uint32 dy = y - py;
    uint32 sy = (uint32)(sint32)(sint16)dy;
    uint32 yy = sy * sy;
    w_u16(frame + 0x12, dy);
    player = r_u32(game + 0x50);
    uint32 z = r_u16(object + 0x5c);
    uint32 pz = r_u16(player + 0x5c);
    uint32 dz = z - pz;
    uint32 sz = (uint32)(sint32)(sint16)dz;
    uint32 zz = sz * sz;
    w_u16(frame + 0x14, dz);
    sint32 threshold = r_s32(state);
    uint32 distance = xx + yy + zz;
    if (threshold >= (sint32)distance)
    {
        uint32 offset = (uint32)r_s16(state + 0x10);
        uint32 tick = r_u32(game + 0x14c);
        uint32 period = (uint32)r_s16(state + 0xe);
        uint32 numerator = tick + offset;
        uint32 phase = period == 0 ? numerator : numerator % period;
        if (r_s16(state + 0x16) >= (sint32)phase)
        {
            w_u32(object + 0x150, 2);
            w_u16(object + 0x5e, phase);
        }
        else
        {
            w_u32(object + 0x150, 1);
            if (r_s16(state + 0x14) != 0)
                w_u32(object + 0x10, r_u32(object + 0x10) | 0x800);
            w_u32(object + 0x144, 0xffffffff);
            uint32 flags = r_u32(object + 0x14);
            w_u32(object + 0x14c, 0);
            w_u32(object + 0x148, 0);
            w_u32(object + 0x14, flags | 0x20000000);
        }
        w_u16(state + 0x12, r_u16(state + 0xe) - phase);
    }
    else
    {
        w_u32(object + 0x150, 0);
        if (r_s16(state + 0x14) != 0)
            w_u32(object + 0x10, r_u32(object + 0x10) | 0x800);
        w_u32(object + 0x144, 0xffffffff);
        uint32 flags = r_u32(object + 0x14);
        w_u32(object + 0x14c, 0);
        w_u32(object + 0x148, 0);
        w_u32(object + 0x14, flags | 0x20000000);
    }
}

void G3MorphFaceUpdate(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000440, "G3L_04");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    uint32 mode = r_u32(object + 0x150);
    if (mode == 0)
    {
        uint32 player = r_u32(game + 0x50);
        uint32 x = r_u16(object + 0x58);
        uint32 px = r_u16(player + 0x58);
        uint32 dx = x - px;
        uint32 sx = (uint32)(sint32)(sint16)dx;
        uint32 xx = sx * sx;
        w_u16(frame + 0x18, dx);
        player = r_u32(game + 0x50);
        uint32 y = r_u16(object + 0x5a);
        uint32 py = r_u16(player + 0x5a);
        uint32 dy = y - py;
        uint32 sy = (uint32)(sint32)(sint16)dy;
        uint32 yy = sy * sy;
        w_u16(frame + 0x1a, dy);
        player = r_u32(game + 0x50);
        uint32 z = r_u16(object + 0x5c);
        uint32 pz = r_u16(player + 0x5c);
        uint32 dz = z - pz;
        uint32 sz = (uint32)(sint32)(sint16)dz;
        uint32 zz = sz * sz;
        w_u16(frame + 0x1c, dz);
        sint32 threshold = r_s32(state);
        uint32 distance = xx + yy + zz;
        if (threshold >= (sint32)distance)
        {
            uint32 offset = (uint32)r_s16(state + 0x10);
            uint32 tick = r_u32(game + 0x14c);
            uint32 period = (uint32)r_s16(state + 0xe);
            w_u32(object + 0x150, 1);
            uint32 numerator = tick + offset;
            sint32 hidden = r_s16(state + 0x14);
            uint32 phase = period == 0 ? numerator : numerator % period;
            if (hidden != 0)
                w_u32(object + 0x10, r_u32(object + 0x10) | 0x800);
            w_u32(object + 0x144, 0xffffffff);
            uint32 flags = r_u32(object + 0x14);
            w_u32(object + 0x14c, 0);
            w_u32(object + 0x148, 0);
            w_u32(object + 0x14, flags | 0x20000000);
            w_u16(state + 0x12, r_u16(state + 0xe) - phase);
        }
    }
    else if (mode == 1)
    {
        uint32 player = r_u32(game + 0x50);
        uint32 x = r_u16(object + 0x58);
        uint32 px = r_u16(player + 0x58);
        uint32 dx = x - px;
        uint32 sx = (uint32)(sint32)(sint16)dx;
        uint32 xx = sx * sx;
        w_u16(frame + 0x18, dx);
        player = r_u32(game + 0x50);
        uint32 y = r_u16(object + 0x5a);
        uint32 py = r_u16(player + 0x5a);
        uint32 dy = y - py;
        uint32 sy = (uint32)(sint32)(sint16)dy;
        uint32 yy = sy * sy;
        w_u16(frame + 0x1a, dy);
        player = r_u32(game + 0x50);
        uint32 z = r_u16(object + 0x5c);
        uint32 pz = r_u16(player + 0x5c);
        uint32 dz = z - pz;
        uint32 sz = (uint32)(sint32)(sint16)dz;
        uint32 zz = sz * sz;
        w_u16(frame + 0x1c, dz);
        sint32 threshold = r_s32(state + 4);
        uint32 distance = xx + yy + zz;
        if (threshold < (sint32)distance)
            w_u32(object + 0x150, 0);
        else
        {
            uint32 next = r_u16(state + 0x12) - 1;
            w_u16(state + 0x12, next);
            if ((sint16)next <= 0)
            {
                w_u32(object + 0x150, 2);
                w_u16(object + 0x5e, 0);
                uint32 period = r_u16(state + 0xe);
                sint32 hidden = r_s16(state + 0x14);
                w_u16(state + 0x12, period);
                if (hidden != 0)
                    w_u32(object + 0x10, r_u32(object + 0x10) & 0xfffff7ff);
                w_u32(object + 0x14, r_u32(object + 0x14) & 0xdfffffff);
            }
        }
    }
    else if (mode == 2)
    {
        sint32 current = r_s16(object + 0x5e);
        sint32 last = r_s16(state + 0x16);
        if (current == last)
        {
            w_u16(object + 0x5e, 0);
            w_u32(object + 0x150, 1);
            sint32 hidden = r_s16(state + 0x14);
            w_u16(state + 0xc, 0);
            if (hidden != 0)
                w_u32(object + 0x10, r_u32(object + 0x10) | 0x800);
            w_u32(object + 0x144, 0xffffffff);
            uint32 flags = r_u32(object + 0x14);
            w_u32(object + 0x14c, 0);
            w_u32(object + 0x148, 0);
            w_u32(object + 0x14, flags | 0x20000000);
        }
        else
        {
            G3Function80029F0C(object, 0xffffffff, frame);
            uint32 next = r_u16(state + 0x12) - 1;
            w_u16(state + 0x12, next);
            if ((sint16)next <= 0)
            {
                w_u16(object + 0x5e, 0);
                w_u16(state + 0x12, r_u16(state + 0xe));
            }
        }
    }
    uint32 handle = r_u32(state + 8);
    if (handle != 0)
    {
        if (G3Function8003F0A8(handle) == 0)
            w_u32(state + 8, 0);
        else
        {
            w_u32(frame + 0x10, 7500);
            handle = r_u32(state + 8);
            G3Function8003E7E4(object, handle, 0, 100, frame);
        }
    }
}

void G3MorphFaceCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000768, "G3L_04");
    uint32 frame = guest_stack - 0x18;
    uint32 contact = r_u32(object + 0xb0);
    if (r_s16(contact + 6) != 1)
        return;
    uint32 other = r_u32(contact + 0x14);
    uint32 player = r_u32(game + 0x50);
    if (other == player)
        G3Function80074044(object, game, frame);
}

void G3MstrdbgExtraAnimationDiagnostic(uint32 name, uint32 module_base, uint32 guest_stack)
{
    uint32 text = module_base + 0x8560;
    uint32 bytes = 0;
    while (r_u8(name + bytes) != 0)
        ++bytes;
    {
#ifndef NDEBUG
    fprintf(stdout, (const char *)psx_addr(text, 33), (const char *)psx_addr(name, bytes + 1));
#endif
    }
    w_u32(guest_stack, text);
    w_u32(guest_stack + 4, name);
    w_u32(guest_stack, text);
    G3Function80023030(text, 0, guest_stack + 4, 0, guest_stack - 0x18, 0);
}

void G3MstrdbgLiteralDiagnostic(uint32 text, uint32 guest_stack)
{
    w_u32(guest_stack, text);
    w_u32(guest_stack, text);
    G3Function80023030(text, 0, guest_stack + 4, 0, guest_stack - 0x18, 0);
}

void G3MstrdbgStringDiagnostic(uint32 text, uint32 value, uint32 guest_stack)
{
    w_u32(guest_stack, text);
    w_u32(guest_stack + 4, value);
    w_u32(guest_stack, text);
    G3Function80023030(text, 0, guest_stack + 4, 0, guest_stack - 0x18, 0);
}

void G3MstrdbgNameDiagnostic(uint32 text, uint32 name, uint32 model, uint32 guest_stack)
{
    w_u32(guest_stack, text);
    w_u32(guest_stack + 4, name);
    w_u32(guest_stack + 8, model);
    w_u32(guest_stack, text);
    G3Function80023030(text, 0, guest_stack + 4, 0, guest_stack - 0x18, 0);
}

void G3MstrdbgPursuitLiteralDiagnostic(uint32 text, uint32 guest_stack)
{
    w_u32(guest_stack, text);
    w_u32(guest_stack, text);
    G3Function80023030(text, 0, guest_stack + 4, 0, guest_stack - 0x18, 0);
}

void G3MstrdbgBrainLiteralDiagnostic(uint32 text, uint32 guest_stack)
{
    w_u32(guest_stack, text);
    w_u32(guest_stack, text);
    G3Function80023030(text, 0, guest_stack + 4, 0, guest_stack - 0x18, 0);
}

void G3MstrdbgBrainStringDiagnostic(uint32 text, uint32 name, uint32 guest_stack)
{
    w_u32(guest_stack, text);
    w_u32(guest_stack + 4, name);
    w_u32(guest_stack, text);
    G3Function80023030(text, 0, guest_stack + 4, 0, guest_stack - 0x18, 0);
}

void G3MstrdbgBrainTwoWordDiagnostic(uint32 text, uint32 first, uint32 second, uint32 guest_stack)
{
    w_u32(guest_stack, text);
    w_u32(guest_stack + 4, first);
    w_u32(guest_stack + 8, second);
    w_u32(guest_stack, text);
    G3Function80023030(text, 0, guest_stack + 4, 0, guest_stack - 0x18, 0);
}

uint32 G3MstrdbgEntry0000(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_39");
    uint32 frame = guest_stack - 0x18;
    return G3MstrdbgEvent7C1C(object, game, message, payload, module_base, frame);
}

void G3MstrdbgInitialize0020(uint32 object, uint32 diagnostic_argument, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000020, "G3L_39");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    uint32 flags = r_u32(state + 0x18);
    uint32 model = r_u32(object + 0x24);
    w_u32(state + 0x18, flags & 0xfffffffe);
    if (G3MstrdbgValidate072C(object, diagnostic_argument, module_base, frame) == 0) return;
    G3FillMemory80086094(state, 0, 0x54);
    if (r_u32(model) == 0)
    {
        w_u32(model + 0x268, 100);
        w_u32(model + 0x264, 1);
        w_u32(model + 0x26c, 1);
        uint32 first = r_u32(module_base + 0x81fc);
        uint32 second = r_u32(module_base + 0x8200);
        w_u32(model + 0x270, first);
        w_u32(model + 0x274, second);
        w_u32(model + 0x290, 0x12b9b0a1);
        w_u32(model, model + 0x264);
    }
    w_u32(model + 0x254, 2500);
    w_u32(model + 0x258, 1000);
    w_u32(model + 0x25c, 2000);
    w_u32(model + 0x260, 4000);
    if (r_u32(object + 0x28) == 0)
    {
        uint32 configuration = model + 0x254;
        if (model != 0)
        {
            uint32 resource = r_u32(model);
            if (resource != 0)
            {
                uint32 configured = r_u32(resource + 0x1c);
                if (configured != 0) configuration = configured;
            }
        }
        w_u32(object + 0x28, configuration);
    }
    uint32 initialized = r_u32(model + 0x28);
    uint32 configuration = r_u32(object + 0x28);
    if (initialized == 0) G3MstrdbgSetupChain1854(r_u32(object + 0x1c), model, module_base, frame);
    uint32 minimum = G3MstrdbgMinimum672C(configuration);
    w_u8(state + 0x3f, minimum != 0 ? 1 : 0);
    w_u8(state + 1, minimum != 0 ? 1 : 0);
    w_u8(state + 4, 0x12);
    w_u8(state + 2, 0x1c);
    w_u16(state + 0x44, 100);
    w_u16(state + 0x46, 100);
    flags = r_u32(state + 0x18);
    w_u8(state, 0);
    w_u8(state + 0xd, 0);
    w_u8(state + 0x3e, 0xff);
    w_u32(state + 0x18, flags | 0x100);
    uint32 resource = r_u32(model);
    w_u16(state + 0x42, r_u16(resource + 8));
    uint32 render_flags = r_u32(object + 0x14);
    flags = r_u32(object + 0x10);
    w_u32(object + 0x14, render_flags & 0xf7ffffff);
    w_u32(object + 0x10, flags | 0x400);
    {
        uint32 source = object + 0x58;
        uint32 destination = state + 0x2a;
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
    {
        uint32 source = object + 0x58;
        uint32 destination = state + 0x30;
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
    G3MstrdbgPackCoordinates13D4(object + 0x58, state + 0x12);
    uint32 player = r_u32(0x8009efc4);
    w_u32(state + 0x50, r_u32(player + 0x138) - 1);
    if ((r_u32(object + 0x10) & 2) != 0)
    {
        w_u8(state + 1, 15);
        w_u8(state + 2, 0x1b);
    }
    uint32 property = G3MstrdbgFindExtra0AEC(configuration, module_base + 0x8208, module_base, frame);
    flags = r_u32(state + 0x18);
    w_u32(state + 0x18, property != 0 ? flags | 0x1000 : flags & 0xffffefff);
    property = G3MstrdbgFindExtra0AEC(configuration, module_base + 0x8214, module_base, frame);
    flags = r_u32(state + 0x18);
    w_u32(state + 0x18, property != 0 ? flags | 0x20 : flags & 0xffffffdf);
    property = G3MstrdbgFindExtra0AEC(configuration, module_base + 0x8220, module_base, frame);
    flags = r_u32(state + 0x18);
    w_u32(state + 0x18, property != 0 ? flags & 0xffffdfdf : flags | 0x2000);
    property = G3MstrdbgFindExtra0AEC(configuration, module_base + 0x822c, module_base, frame);
    render_flags = r_u32(object + 0x14);
    w_u32(object + 0x14, property != 0 ? render_flags | 0x20 : render_flags & 0xffffffdf);
    flags = r_u32(state + 0x18);
    w_u32(state + 0x18, property != 0 ? flags | 0x4000 : flags & 0xffffbfff);
    player = r_u32(0x8009efc4);
    G3MstrdbgUpdateSight17AC(object + 0x58, player + 0x58, state, frame);
    resource = r_u32(model);
    if ((r_u32(resource + 0x18) & 2) != 0 && r_s8(state + 1) == 0) G3MstrdbgEnterSecondary16B8(object, frame);
    else G3MstrdbgEnterPrimary1668(object, frame);
    resource = r_u32(model);
    if ((r_u32(resource + 0x18) & 0x40) != 0) G3Function80047EE4(10, (uint32)r_s16(state + 0x42));
}

void G3MstrdbgNoop03D8(void)
{
    FUNCTION_MARKER(0x000003d8, "G3L_39");
}

void G3MstrdbgUpdate03E0(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000003e0, "G3L_39");
    uint32 frame = guest_stack - 0x78;
    uint32 state = object + 0x104;
    uint32 iterations = 0;
    uint32 model = r_u32(object + 0x24);
    uint32 configuration = r_u32(object + 0x28);
    G3MstrdbgUpdateFloor3F44(object, model, module_base, frame);
    uint32 player = r_u32(0x8009efc4);
    {
        uint32 source = object + 0x58;
        uint32 destination = state + 0x2a;
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
    G3MstrdbgBuildContext0DE8(frame + 0x10, object, player + 0x58);
    G3MstrdbgUpdateHeading106C(state, frame + 0x40, frame);
    G3MstrdbgUpdateTarget7524(object, state, r_u32(state + 0x1c), frame + 0x40, frame);
    G3MstrdbgUpdateMovementWatch1488(state, object + 0x58, frame);
    w_u16(frame + 0x48, 40);
    while ((sint16)iterations < 15)
    {
        if ((r_u32(state + 0x18) & 0x8000000) != 0) break;
        uint32 resource = r_u32(model);
        uint32 kind = r_u32(resource);
        uint32 previous_time = r_u16(state + 0x20);
        uint32 stepped = G3MstrdbgStep7AC4(frame + 0x48, object, state, kind, frame);
        uint32 current_time = (uint32)r_s16(state + 0x20);
        uint32 animation = r_u32(state + 0x14);
        G3MstrdbgTriggerFrames4968((uint32)(sint16)previous_time, current_time, animation, object, module_base, frame);
        if (stepped != 0 || G3MstrdbgNeedsDecision0E6C(state, model, configuration, module_base, frame) != 0)
        {
            uint32 next_model = G3MstrdbgDecide4A04(object, configuration, model, module_base, frame);
            animation = r_u32(state + 0x14);
            uint32 sequence = r_u8(animation);
            model = next_model;
            w_u16(object + 0x6e, sequence);
        }
        ++iterations;
        if (stepped == 0 && r_s16(frame + 0x48) <= 0) break;
    }
    uint32 time = r_u16(state + 0x20);
    uint32 descriptor = r_u32(object + 0x1c);
    w_u16(object + 0x5e, (uint32)((sint16)time / 40) & 0xff);
    if ((r_u32(descriptor + 0x30) & 0x2000) != 0) G3Function8003E314(object, frame);
    uint32 game = r_u32(0x80097048);
    uint32 world = r_u32(game + 0x5c);
    uint32 base = r_u32(world + 8);
    uint32 snapshot = r_u32(state + 0x1c);
    uint32 phase = (object - 4 - base) * 0x2fa0be83;
    G3MstrdbgStaggerSight17F8(object + 0x58, snapshot, state, (uint32)((sint32)phase >> 3), frame);
    G3MstrdbgUpdateCounters08C4(state, state, object, frame);
    G3MstrdbgPostPhysics403C(object, model, module_base, frame);
    if (r_s16(state + 0x34) < -30000 && r_s16(object + 0x5c) > 32000)
    {
        r_s8(state + 0x3e);
        G3MstrdbgNoop7E48();
        w_u8(state + 0x3e, 0xff);
        w_u32(object + 0x10, r_u32(object + 0x10) & 0xfffffbff);
        w_u32(state + 0x18, r_u32(state + 0x18) | 0x8000000);
    }
    if ((sint16)iterations == 15) G3MstrdbgBrainLiteralDiagnostic(module_base + 0x8238, frame);
    game = r_u32(0x80097048);
    if ((r_u32(game + 0x69b8) & 1) != 0) G3MstrdbgPrintState4414(r_u32(object + 0x1c), state, model, module_base, frame);
    uint32 flags = r_u32(state + 0x18);
    w_u32(state + 0x1c, 0);
    w_u32(state + 0x18, flags & 0xfffffeff);
    {
        uint32 source = object + 0x58;
        uint32 destination = state + 0x30;
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
    if ((r_u32(state + 0x18) & 0x8000000) != 0) G3Function80038798(object, 0, 0, frame);
}

uint32 G3MstrdbgValidate072C(uint32 object, uint32 diagnostic_argument, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000072c, "G3L_39");
    uint32 frame = guest_stack - 0x18;
    uint32 state = object + 0x104;
    uint32 flags = r_u32(state + 0x18);
    uint32 settings = r_u32(object + 0x24);
    if ((flags & 1) != 0)
        return 1;
    uint32 diagnostic = 0;
    if (settings == 0)
        diagnostic = 0x8274;
    else if (r_u32(settings + 0x294) != 0x12b9b0a1)
        diagnostic = 0x828c;
    else
    {
        uint32 basic = r_u32(settings);
        if (basic != 0 && r_u32(basic + 0x2c) != 0x12b9b0a1)
            diagnostic = 0x82b0;
        else
        {
            uint32 modes = r_u32(settings + 0xc);
            if (modes != 0 && r_u32(modes + 0x40) != 0x12b9b0a1)
                diagnostic = 0x82dc;
            else
            {
                uint32 maneuver = r_u32(settings + 0x10);
                if (maneuver != 0 && r_u32(maneuver + 0x28) != 0x12b9b0a1)
                    diagnostic = 0x830c;
                else
                {
                    uint32 extra = r_u32(settings + 0x18);
                    if (extra != 0 && r_u32(extra + 0x10) != 0x12b9b0a1)
                        diagnostic = 0x833c;
                }
            }
        }
    }
    if (diagnostic != 0)
    {
        uint32 descriptor = r_u32(object + 0x1c);
        uint32 name = r_u32(descriptor + 0x24);
        G3Function80020CBC(module_base + diagnostic, name, state, diagnostic_argument, frame, 0);
        return 0;
    }
    uint32 basic = r_u32(settings);
    if (basic != 0 && r_s32(basic) >= 2)
    {
        G3Function80020CBC(module_base + 0x836c, settings, state, diagnostic_argument, frame, 0);
        return 0;
    }
    flags = r_u32(state + 0x18);
    w_u32(state + 0x18, flags | 1);
    return 1;
}

void G3MstrdbgNoop08BC(void)
{
    FUNCTION_MARKER(0x000008bc, "G3L_39");
}

void G3MstrdbgUpdateCounters08C4(uint32 counters, uint32 state, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000008c4, "G3L_39");
    uint32 frame = guest_stack - 0x20;
    uint32 count = r_u8(state + 0x4e);
    w_u8(state + 0x4e, count != 0 ? count - 1 : 0);
    if (r_u8(state + 0x36) == 1)
        G3MstrdbgEnterPrimary1668(object, frame);
    count = r_u8(state + 0x36);
    w_u8(state + 0x36, count != 0 ? count - 1 : 0);
    if ((r_u32(state + 0x18) & 0x02000000) != 0)
    {
        uint32 active = r_u8(counters + 0xc);
        if (active < 0xff)
            w_u8(counters + 0xc, active + 1);
    }
    else
        w_u8(counters + 0xc, 0);
    if (r_u8(counters + 0xc) == 1)
    {
        uint32 context = r_u32(state + 0x1c);
        uint32 distance = r_u16(context + 0x1c) >> 6;
        if (distance >= 0x100)
            distance = 0xff;
        context = r_u32(state + 0x1c);
        w_u8(state + 0x4c, distance);
        uint32 angle = r_u16(context + 0x2c);
        w_u8(state + 0x4d, (angle & 0xfff) >> 4);
    }
    if (r_u8(counters + 0xc) >= 2)
    {
        sint32 threshold = (sint32)r_u8(state + 0x4c) - 2;
        if (threshold < 0)
            threshold = 0;
        uint32 context = r_u32(state + 0x1c);
        uint32 distance = r_u16(context + 0x1c) >> 6;
        if ((sint32)distance < (sint16)threshold)
        {
            uint32 flags = r_u32(state + 0x18);
            w_u32(state + 0x18, flags & 0xfdffffff);
            w_u8(counters + 0xc, 0);
        }
        context = r_u32(state + 0x1c);
        uint32 previous = r_u8(state + 0x4d);
        uint32 angle = (r_u16(context + 0x2c) - (previous << 4)) & 0xfff;
        if (angle >= 0x800)
            angle |= 0xf000;
        sint32 difference = (sint16)angle;
        if (difference < 0)
            difference = -difference;
        if (difference >= 0x201)
        {
            uint32 flags = r_u32(state + 0x18);
            w_u32(state + 0x18, flags & 0xfdffffff);
            w_u8(counters + 0xc, 0);
        }
    }
    count = r_u8(counters + 0xa) + 1;
    w_u8(counters + 0xa, count);
    if ((count & 0xff) >= 0x1e)
    {
        uint32 first = r_u8(counters + 0xb);
        if (first != 0xff)
            w_u8(counters + 0xb, first + 1);
        uint32 second = r_u8(counters + 0x10);
        if (second != 0xff)
            w_u8(counters + 0x10, second + 1);
        w_u8(counters + 0xa, 0);
    }
    count = r_u8(state + 0x37);
    if (count != 0)
        w_u8(state + 0x37, count - 1);
}

uint32 G3MstrdbgFindExtra0AEC(uint32 settings, uint32 tag, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000aec, "G3L_39");
    uint32 frame = guest_stack - 0x20;
    if (settings == 0)
        return 0;
    uint32 first = r_u32(settings + 0x10);
    uint32 cursor;
    if (first == 0x12b9b0a1)
    {
        cursor = settings + 0x14;
        first = r_u32(cursor);
        uint32 marker = r_u32(module_base + 0x8394);
        if (first != marker)
            return 0;
        uint32 second = r_u32(cursor + 4);
        marker = r_u32(module_base + 0x8398);
        if (second != marker)
            return 0;
        cursor = settings + 0x1c;
    }
    else
    {
        cursor = settings + 0x10;
        uint32 marker = r_u32(module_base + 0x8394);
        if (first != marker)
            return 0;
        uint32 second = r_u32(cursor + 4);
        marker = r_u32(module_base + 0x8398);
        if (second != marker)
            return 0;
        cursor = settings + 0x18;
    }
    while (true)
    {
        first = r_u32(cursor);
        uint32 marker = r_u32(module_base + 0x83a0);
        if (first == marker)
        {
            uint32 second = r_u32(cursor + 4);
            marker = r_u32(module_base + 0x83a4);
            if (second == marker)
                return 0;
        }
        uint32 requested = r_u32(tag);
        if (first == requested)
        {
            uint32 second = r_u32(cursor + 4);
            requested = r_u32(tag + 4);
            if (second == requested)
                return cursor + 8;
        }
        marker = r_u32(module_base + 0x83ac);
        if (first == marker)
        {
            uint32 second = r_u32(cursor + 4);
            marker = r_u32(module_base + 0x83b0);
            if (second == marker)
            {
                cursor += 0xc;
                continue;
            }
        }
        const uint32 tags[5] = {0x83b8, 0x8214, 0x822c, 0x8220, 0x8208};
        const uint32 sizes[5] = {0xc, 8, 8, 8, 0x14};
        bool skipped = false;
        for (uint32 index = 0; index < 5; ++index)
        {
            first = r_u32(cursor);
            marker = r_u32(module_base + tags[index]);
            if (first == marker)
            {
                uint32 second = r_u32(cursor + 4);
                marker = r_u32(module_base + tags[index] + 4);
                if (second == marker)
                {
                    cursor += sizes[index];
                    skipped = true;
                    break;
                }
            }
        }
        if (skipped)
            continue;
        first = r_u32(cursor);
        marker = r_u32(module_base + 0x83c4);
        if (first == marker)
        {
            cursor += 0xc;
            continue;
        }
        marker = r_u32(module_base + 0x83a0);
        if (first == marker)
        {
            uint32 second = r_u32(cursor + 4);
            marker = r_u32(module_base + 0x83a4);
            if (second == marker)
                return 0;
        }
        G3Function80020CBC(module_base + 0x83cc, tag, cursor, settings, frame, 0);
        G3SdkPrint80085E84(module_base + 0x83cc);
        return 0;
    }
}
