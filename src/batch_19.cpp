#include "xport.h"
#include "psx.h"
#include "game_functions.h"

uint32 G3Function8004E790(void)
{
    FUNCTION_MARKER(0x8004E790, "SLUS_008.06");
    return G3FillMemory80086094(0x800a2290, 0, 20);
}

uint32 G3Function8004E7BC(uint32 object, uint32 mode)
{
    FUNCTION_MARKER(0x8004E7BC, "SLUS_008.06");
    G3FillMemory80086094(0x800a2290, 0, 20);
    uint32 player = r_u32(0x8009efc4);
    w_u32(0x800a2290, mode);
    uint32 words[2];
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 source = object + 0x10 + index * 4;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
        words[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 index = 0; index < 2; ++index)
    {
        w_masked_u32(0x800a2294 + index * 4, words[index], 15);
        w_masked_u32(0x800a2294 + index * 4, words[index], 15);
    }
    uint32 result = 0;
    for (uint32 index = 0; index < 3; ++index)
    {
        uint32 coordinate = r_u16(object + index * 2);
        uint32 previous = r_u16(player + 0x58 + index * 2);
        result = coordinate - previous;
        w_u16(0x800a229c + index * 2, result);
    }
    return result;
}

uint32 G3Function8004E860(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004E860, "SLUS_008.06");
    uint32 positive = r_u32(0x80096b4c);
    uint32 frame = guest_stack - 0x38;
    uint32 delta = 0;
    if (positive != 0)
        delta = 32;
    else if (r_u32(0x80096b50) != 0)
        delta = 0xffffffe0;
    if (delta != 0)
    {
        uint32 angle = r_u16(0x800a2298);
        w_u16(0x800a2298, angle + delta);
        G3Function8002F8E4(0x800a229c, delta);
    }
    uint32 player = r_u32(0x8009efc4);
    uint32 offset = r_u16(0x800a229c);
    uint32 coordinate = r_u16(player + 0x58);
    w_u16(frame + 0x10, coordinate + offset);
    coordinate = r_u16(player + 0x5a);
    offset = r_u16(0x800a229e);
    w_u16(frame + 0x12, coordinate + offset);
    coordinate = r_u16(player + 0x5c);
    offset = r_u16(0x800a22a0);
    player = r_u32(0x8009efc4);
    w_u16(frame + 0x14, coordinate + offset);
    uint32 words[2];
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 source = 0x800a2294 + index * 4;
        words[index] = r_masked_u32(source, 15);
        words[index] = r_masked_u32(source, 15);
    }
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 destination = frame + 0x1c + index * 4;
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        uint32 low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15u << low_lane) & 15);
    }
    for (uint32 index = 0; index < 3; ++index)
    {
        coordinate = r_u16(frame + 0x10 + index * 2);
        uint32 previous = r_u16(player + 0x58 + index * 2);
        w_u16(frame + 0x28 + index * 2, coordinate - previous);
    }
    uint32 length = G3Function8002FA50(frame + 0x28, frame);
    w_u32(frame + 0x24, length);
    return G3Function8005A248(object, frame + 0x10);
}

uint32 G3Function8004E994(uint32 object, uint32 event, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004E994, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (event == 1)
    {
        G3Function8004E860(object, frame);
        return 1;
    }
    if (event == 0)
    {
        G3Function8004E7BC(object, payload);
        return 1;
    }
    if (event == 6)
    {
        G3Function8004E790();
        return 1;
    }
    return event == 9;
}

uint32 G3Function800551D0(uint32 object, uint32 event, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800551D0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    if (event == 0)
    {
        w_u32(0x800a2454, payload);
        uint32 coordinate = r_u16(object);
        uint32 previous = r_u16(0x80096a54);
        w_u16(frame + 0x40, coordinate - previous);
        coordinate = r_u16(object + 2);
        previous = r_u16(0x80096a56);
        uint32 owner = r_u32(0x80096b34);
        w_u16(frame + 0x42, coordinate - previous);
        coordinate = r_u16(object + 4);
        previous = r_u16(0x80096a58);
        w_u16(frame + 0x44, coordinate - previous);
        w_u32(frame + 0x10, frame + 0x40);
        G3Function80054DB8(object, object + 0x10, 0x80096b38, owner + 0x60, frame);
        return 1;
    }
    if (event == 1)
    {
        uint32 completed = G3Function80055170(frame + 0x28, frame + 0x34, 40, payload, frame) & 0xff;
        uint32 distance = (uint32)r_s16(0x80096b68);
        w_u32(frame + 0x3c, distance);
        G3Function8005A248(object, frame + 0x28);
        uint32 game = r_u32(0x80097048);
        if ((r_u32(game + 0x69b8) & 0x800) != 0)
        {
            for (uint32 point = 0; point < 2; ++point)
            {
                uint32 state = r_u32(0x8009ebd8);
                uint32 source = state + (point == 0 ? 0x20 : 0x12);
                uint32 high_address = source + 3;
                uint32 high_lane = high_address & 3;
                uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
                uint32 low_lane = source & 3;
                uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
                uint32 word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
                uint32 z = (uint32)r_s16(source + 4);
                high_address = frame + 0x4b;
                high_lane = high_address & 3;
                w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
                uint32 destination = frame + 0x48;
                low_lane = destination & 3;
                w_masked_u32(destination & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
                w_u16(frame + 0x4c, z);
                for (uint32 axis = 0; axis < 3; ++axis)
                {
                    uint32 y = (uint32)r_s16(frame + 0x4a);
                    uint32 x = (uint32)r_s16(frame + 0x48);
                    w_u32(frame + 0x10, y);
                    z = (uint32)r_s16(frame + 0x4c);
                    w_u32(frame + 0x14, z);
                    uint32 endpoint_x = x;
                    if (axis == 0)
                        endpoint_x = (uint32)(sint16)(r_u16(frame + 0x48) + 0x100);
                    w_u32(frame + 0x18, endpoint_x);
                    uint32 endpoint_y = axis == 1 ? (uint32)(sint16)(r_u16(frame + 0x4a) + 0x100) : (uint32)r_s16(frame + 0x4a);
                    w_u32(frame + 0x1c, endpoint_y);
                    uint32 endpoint_z = axis == 2 ? (uint32)(sint16)(r_u16(frame + 0x4c) + 0x100) : (uint32)r_s16(frame + 0x4c);
                    w_u32(frame + 0x20, endpoint_z);
                    G3Function800126E4(endpoint_z);
                }
            }
        }
        if (completed != 0)
        {
            uint32 mode = r_u32(0x800a2454);
            G3Function80013C94(object, mode, frame);
        }
        return 1;
    }
    if (event == 2)
    {
        uint32 value = r_u32(0x800a2458);
        if (value != 0)
        {
            w_u32(0x800a2458, 0);
            value = G3Function80016F24();
        }
        G3Function80055150(value);
        return 1;
    }
    if (event == 6)
    {
        G3Function80055194();
        return 1;
    }
    return 0;
}

uint32 G3Function80055194(void)
{
    FUNCTION_MARKER(0x80055194, "SLUS_008.06");
    G3FillMemory80086094(0x800a2454, 0, 8);
    uint32 state = r_u32(0x8009ebd8);
    return G3FillMemory80086094(state, 0, 96);
}

uint32 G3Function80055150(uint32 incoming_v0)
{
    FUNCTION_MARKER(0x80055150, "SLUS_008.06");
    return G3Function8006479C(incoming_v0);
}

uint32 G3Function80055170(uint32 position, uint32 rotation, uint32 blend, uint32 argument3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80055170, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    return G3Function80054660(position, rotation, (uint32)(sint16)blend, argument3, frame) & 0xff;
}

uint32 G3Function800126E4(uint32 incoming_v0)
{
    FUNCTION_MARKER(0x800126E4, "SLUS_008.06");
    return incoming_v0;
}

uint32 G3Function8006479C(uint32 incoming_v0)
{
    FUNCTION_MARKER(0x8006479C, "SLUS_008.06");
    return incoming_v0;
}

uint32 G3Function8005153C(uint32 object, uint32 game, uint32 event, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005153C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    switch (event)
    {
    case 0:
        G3Function80050F90(object, game, payload, payload, frame);
        return 1;
    case 1:
        G3Function800512C0(object, game, payload, payload, frame);
        return 1;
    case 4:
        G3Function80050FE8(object, game, payload, payload, frame);
        return 1;
    case 6:
        G3Function80050F64();
        return 1;
    case 7:
        return G3Function80056924();
    case 8:
    case 22:
        return 1;
    case 11:
        G3Function800512F4(object, game, payload, payload, frame);
        return 1;
    case 12:
    case 13:
    {
        uint32 value = r_u16(0x80096b68);
        w_u16(0x800a237c, value);
        return 1;
    }
    case 17:
    {
        if ((r_u32(0x80096a94) & 1) != 0)
            return 1;
        uint32 mode = r_u32(0x80096a8c);
        w_u16(0x800a237c, payload);
        if (mode == 0)
            w_u16(0x800a2374, payload);
        return 1;
    }
    case 18:
    {
        if ((r_u32(0x80096a94) & 2) != 0)
            return 1;
        uint32 mode = r_u32(0x80096a8c);
        w_u16(0x800a2378, payload);
        if (mode == 0)
            w_u16(0x800a2376, payload);
        return 1;
    }
    case 19:
        w_u16(0x800a237a, payload & 0xfff);
        return 1;
    case 24:
        return payload == 19;
    default:
        return 0;
    }
}

uint32 G3Function80050F64(void)
{
    FUNCTION_MARKER(0x80050F64, "SLUS_008.06");
    return G3FillMemory80086094(0x800a2374, 0, 22);
}

uint32 G3Function800547F0(uint32 polar, uint32 output)
{
    FUNCTION_MARKER(0x800547F0, "SLUS_008.06");
    uint32 angle = r_u32(polar);
    uint32 wave = G3Function8005B470(angle - 0x400);
    uint32 radius = (uint32)r_s16(polar + 4);
    uint32 product = radius * wave;
    w_u16(output, (uint32)((sint32)product >> 12));
    angle = r_u32(polar);
    wave = G3Function8005B590(angle - 0x400);
    radius = (uint32)r_s16(polar + 4);
    product = radius * wave;
    uint32 state = r_u32(0x8009ebd8);
    w_u16(output + 2, (uint32)((sint32)product >> 12));
    uint32 height = r_u16(polar + 6);
    w_u16(output + 4, height);
    uint32 coordinate = r_u16(output);
    uint32 origin = r_u16(state + 0x20);
    w_u16(output, coordinate + origin);
    coordinate = r_u16(output + 2);
    origin = r_u16(state + 0x22);
    uint32 result = coordinate + origin;
    w_u16(output + 2, result);
    return result;
}

uint32 G3Function8005489C(uint32 direction, uint32 current, uint32 target)
{
    FUNCTION_MARKER(0x8005489C, "SLUS_008.06");
    uint32 magnitude = r_u32(direction);
    if ((sint32)magnitude < 0)
        magnitude = 0u - magnitude;
    w_u32(direction, magnitude);
    uint32 target_angle = r_u32(target);
    uint32 current_angle = r_u32(current);
    if ((sint32)current_angle < (sint32)target_angle)
    {
        uint32 result = 0u - magnitude;
        if ((sint32)(target_angle - current_angle) >= 0x801)
        {
            w_u32(direction, result);
            result = r_u32(current) + 0x1000;
            w_u32(current, result);
        }
        return result;
    }
    if ((sint32)(current_angle - target_angle) < 0x800)
    {
        uint32 result = 0u - magnitude;
        w_u32(direction, result);
        return result;
    }
    uint32 result = target_angle + 0x1000;
    w_u32(target, result);
    return result;
}

uint32 G3Function80054918(uint32 angle, uint32 radius, uint32 height)
{
    FUNCTION_MARKER(0x80054918, "SLUS_008.06");
    uint32 state = r_u32(0x8009ebd8);
    uint32 target = r_u32(state + 0x58);
    if (angle == target)
    {
        uint32 value = r_u16(state + 0x5c);
        w_u16(radius, value);
        state = r_u32(0x8009ebd8);
        value = r_u16(state + 0x5e);
        w_u16(height, value);
        return value;
    }
    uint32 current = r_u32(state + 0x48);
    uint32 span = target - current;
    if (angle == current)
    {
        uint32 value = r_u16(state + 0x4c);
        w_u16(radius, value);
    }
    else
    {
        uint32 target_radius = (uint32)r_s16(state + 0x5c);
        uint32 current_radius = (uint32)r_s16(state + 0x4c);
        uint32 target_height = (uint32)r_s16(state + 0x5e);
        uint32 current_height = (uint32)r_s16(state + 0x4e);
        uint32 radius_origin = r_u16(state + 0x4c);
        uint32 radius_delta = target_radius - current_radius;
        uint32 height_delta = target_height - current_height;
        if (span != 0)
        {
            uint32 radius_slope = (uint32)((sint32)(radius_delta << 12) / (sint32)span);
            uint32 height_slope = (uint32)((sint32)(height_delta << 12) / (sint32)span);
            uint32 product = radius_slope * (angle - current);
            uint32 value = radius_origin + (uint32)((sint32)product >> 12);
            w_u16(radius, value);
            state = r_u32(0x8009ebd8);
            uint32 reloaded_angle = r_u32(state + 0x48);
            product = height_slope * (angle - reloaded_angle);
            uint32 height_origin = r_u16(state + 0x4e);
            value = height_origin + (uint32)((sint32)product >> 12);
            w_u16(height, value);
            return value;
        }
        uint32 value = r_u16(state + 0x48);
        w_u16(radius, value);
    }
    state = r_u32(0x8009ebd8);
    uint32 value = r_u16(state + 0x4e);
    w_u16(height, value);
    return value;
}

uint32 G3Function80054A14(uint32 angle, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80054A14, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    uint32 state = r_u32(0x8009ebd8);
    w_u32(frame + 0x18, 102);
    uint32 total = 0;
    if (r_s32(state + 0x44) < 0)
        w_u32(frame + 0x18, 0xffffff9a);
    uint32 first_word = r_u32(state + 0x58);
    uint32 second_word = r_u32(state + 0x5c);
    w_u32(frame + 0x10, first_word);
    w_u32(frame + 0x14, second_word);
    G3Function800547F0(frame + 0x10, frame + 0x20);
    state = r_u32(0x8009ebd8);
    uint32 target = (uint32)(sint16)angle;
    uint32 current = r_u32(state + 0x58);
    for (;;)
    {
        uint32 stride = r_u32(frame + 0x18);
        if (!((sint32)stride > 0 && (sint32)target < (sint32)current) && !((sint32)stride < 0 && (sint32)current < (sint32)target))
            break;
        stride = r_u32(frame + 0x18);
        uint32 magnitude = (sint32)stride < 0 ? 0u - stride : stride;
        uint32 difference = current - target;
        if ((sint32)difference < 0)
            difference = target - current;
        uint32 sample;
        if ((sint32)difference < (sint32)magnitude)
        {
            sample = target;
            w_u32(frame + 0x10, sample);
        }
        else
        {
            sample = r_u32(frame + 0x10);
            stride = r_u32(frame + 0x18);
            sample -= stride;
            w_u32(frame + 0x10, sample);
        }
        G3Function80054918(sample, frame + 0x14, frame + 0x16);
        G3Function800547F0(frame + 0x10, frame + 0x28);
        uint32 deltas[3];
        uint32 squared = 0;
        for (uint32 index = 0; index < 3; ++index)
        {
            uint32 previous = r_u16(frame + 0x20 + index * 2);
            uint32 coordinate = r_u16(frame + 0x28 + index * 2);
            deltas[index] = previous - coordinate;
            sint32 delta = (sint16)deltas[index];
            squared += (uint32)(delta * delta);
        }
        w_u16(frame + 0x30, deltas[0]);
        w_u16(frame + 0x32, deltas[1]);
        w_u16(frame + 0x34, deltas[2]);
        uint32 length;
        if ((sint32)squared > 0x08000000)
        {
            uint32 root = G3Function8002F368(squared, frame);
            if ((sint32)root < 0)
                root += 63;
            length = (uint32)((sint32)root >> 6);
        }
        else if ((sint32)squared > 0x00080000)
        {
            uint32 root = G3Function8002F448(squared << 4, 4, frame);
            length = (uint32)((sint32)(root + 8) >> 4);
        }
        else
        {
            uint32 root = G3Function8002F368(squared << 12, frame);
            length = (uint32)((sint32)root >> 12);
        }
        uint32 source = frame + 0x28;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
        uint32 word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        uint32 z = (uint32)r_s16(frame + 0x2c);
        high_address = frame + 0x23;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        uint32 destination = frame + 0x20;
        low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
        w_u16(frame + 0x24, z);
        stride = r_u32(frame + 0x18);
        total += length;
        current -= stride;
    }
    return total;
}

uint32 G3Function80054DB8(uint32 object, uint32 rotation, uint32 position, uint32 owner_rotation, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80054DB8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 state = r_u32(0x8009ebd8);
    uint32 delta = r_u32(guest_stack + 0x10);
    uint32 identifier = r_u32(state);
    G3FillMemory80086094(state, 0, 96);
    state = r_u32(0x8009ebd8);
    w_u32(state, identifier);
    uint32 high_address = object + 3;
    uint32 high_lane = high_address & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
    uint32 low_lane = object & 3;
    uint32 low = r_masked_u32(object & 0xfffffffc, (15u << low_lane) & 15);
    uint32 word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    uint32 z = (uint32)r_s16(object + 4);
    high_address = state + 7;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
    uint32 destination = state + 4;
    low_lane = destination & 3;
    w_masked_u32(destination & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
    w_u16(state + 8, z);
    uint32 words[2];
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 source = rotation + index * 4;
        high_address = source + 3;
        high_lane = high_address & 3;
        high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        low_lane = source & 3;
        low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
        words[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 index = 0; index < 2; ++index)
    {
        destination = state + 0x0a + index * 4;
        high_address = destination + 3;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15u << low_lane) & 15);
    }
    G3Function8005437C(object, rotation, position, owner_rotation, frame);
    state = r_u32(0x8009ebd8);
    uint32 angle = r_u16(state + 0x0e) & 0xfff;
    w_u16(state + 0x0e, angle);
    if (angle >= 0x800)
        w_u16(state + 0x0e, angle - 0x1000);
    state = r_u32(0x8009ebd8);
    angle = r_u16(state + 0x1c) & 0xfff;
    w_u16(state + 0x1c, angle);
    if (angle >= 0x800)
        w_u16(state + 0x1c, angle - 0x1000);
    state = r_u32(0x8009ebd8);
    words[0] = r_u32(state + 0x48);
    words[1] = r_u32(state + 0x4c);
    w_u32(state + 0x50, words[0]);
    w_u32(state + 0x54, words[1]);
    high_address = position + 3;
    high_lane = high_address & 3;
    high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
    low_lane = position & 3;
    low = r_masked_u32(position & 0xfffffffc, (15u << low_lane) & 15);
    word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    z = (uint32)r_s16(position + 4);
    high_address = state + 0x23;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
    destination = state + 0x20;
    low_lane = destination & 3;
    w_masked_u32(destination & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
    w_u16(state + 0x24, z);
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 source = owner_rotation + index * 4;
        high_address = source + 3;
        high_lane = high_address & 3;
        high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        low_lane = source & 3;
        low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
        words[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 index = 0; index < 2; ++index)
    {
        destination = state + 0x26 + index * 4;
        high_address = destination + 3;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15u << low_lane) & 15);
    }
    angle = (uint32)r_s16(state + 0x50);
    uint32 result = G3Function80054A14(angle, frame);
    state = r_u32(0x8009ebd8);
    w_u32(state + 0x3c, result);
    sint32 x = r_s16(delta);
    if (x == 0)
    {
        if (r_s16(delta + 2) == 0 && r_s16(delta + 4) == 0)
            return 0;
        x = r_s16(delta);
    }
    uint32 squared_x = (uint32)(x * x);
    sint32 y = r_s16(delta + 2);
    uint32 squared_y = (uint32)(y * y);
    sint32 signed_z = r_s16(delta + 4);
    uint32 squared = squared_x + squared_y + (uint32)(signed_z * signed_z);
    if ((sint32)squared > 0x08000000)
    {
        result = G3Function8002F368(squared, frame);
        if ((sint32)result < 0)
            result += 63;
        result = (uint32)((sint32)result >> 6);
    }
    else if ((sint32)squared > 0x00080000)
    {
        result = G3Function8002F448(squared << 4, 4, frame);
        result = (uint32)((sint32)(result + 8) >> 4);
    }
    else
    {
        result = G3Function8002F368(squared << 12, frame);
        result = (uint32)((sint32)result >> 12);
    }
    state = r_u32(0x8009ebd8);
    result = 0u - result;
    w_u32(state + 0x40, result);
    return result;
}

uint32 G3Function80054C38(uint32 distance, uint32 velocity, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80054c38, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 result = G3Function80064680((uint32)(sint32)(sint16)(distance & 0xffff), (uint32)(sint32)(sint16)(velocity & 0xffff), 32, frame);
    return (uint32)(sint32)(sint16)(result & 0xffff);
}

uint32 G3Function80054C6C(uint32 velocity, uint32 polar)
{
    FUNCTION_MARKER(0x80054c6c, "SLUS_008.06");
    sint32 radius = r_s16(polar + 4);
    uint32 step = 0;
    if (radius != 0)
    {
        uint32 numerator = ((velocity << 3) - velocity) << 12;
        sint32 denominator = radius * 44;
        step = (uint32)((sint32)numerator / denominator);
    }
    uint32 state = r_u32(0x8009ebd8);
    sint32 direction = r_s32(state + 0x44);
    if ((sint32)step < 0)
        step = uint32(0) - step;
    if (direction <= 0)
        step = uint32(0) - step;
    if (step != 0)
    {
        state = r_u32(0x8009ebd8);
        uint32 current = r_u32(polar);
        uint32 target = r_u32(state + 0x58);
        uint32 magnitude = (sint32)step < 0 ? uint32(0) - step : step;
        uint32 difference = current - target;
        uint32 distance = (sint32)difference < 0 ? target - current : difference;
        if ((sint32)magnitude >= (sint32)distance)
        {
            state = r_u32(0x8009ebd8);
            current = r_u32(polar);
            target = r_u32(state + 0x58);
            w_u32(state + 0x40, 0);
            step = target - current;
        }
        state = r_u32(0x8009ebd8);
        current = r_u32(polar);
        target = r_u32(state + 0x58);
        uint32 next = current + step;
        if (((sint32)target >= (sint32)current && (sint32)next >= (sint32)target) || ((sint32)next <= (sint32)target && (sint32)current >= (sint32)target))
        {
            step = target - current;
            w_u32(state + 0x40, 0);
        }
    }
    uint32 angle = r_u32(polar) + step;
    w_u32(polar, angle);
    return G3Function80054918(angle, polar + 4, polar + 6);
}

uint32 G3Function80054660(uint32 position, uint32 rotation, uint32 blend, uint32 argument3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80054660, "SLUS_008.06");
    uint32 state = r_u32(0x8009ebd8);
    uint32 frame = guest_stack - 0x28;
    uint32 completed = 0;
    if (r_u32(state + 0x3c) == 0)
    {
        uint32 first_game = r_u32(0x80097048);
        uint32 second_game = r_u32(0x80097048);
        uint32 target = r_u32(first_game + 0x4c);
        uint32 high_address = target + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low_lane = target & 3;
        uint32 low = r_masked_u32(target & 0xfffffffc, (15u << low_lane) & 15);
        uint32 word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        sint32 z = r_s16(target + 4);
        high_address = position + 3;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        low_lane = position & 3;
        w_masked_u32(position & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
        w_u16(position + 4, (uint32)z);
        target = r_u32(second_game + 0x4c);
        uint32 words[2];
        for (uint32 index = 0; index < 2; ++index)
        {
            uint32 source = target + 0x18 + index * 4;
            high_address = source + 3;
            high_lane = high_address & 3;
            high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            low_lane = source & 3;
            low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
            words[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 index = 0; index < 2; ++index)
        {
            uint32 destination = rotation + index * 4;
            high_address = destination + 3;
            high_lane = high_address & 3;
            w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            low_lane = destination & 3;
            w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15u << low_lane) & 15);
        }
        return 1;
    }
    uint32 angle = (uint32)r_s16(state + 0x50);
    uint32 distance = G3Function80054A14(angle, frame);
    if ((sint32)distance < 128)
    {
        state = r_u32(0x8009ebd8);
        uint32 velocity = r_u32(state + 0x40);
        if ((sint32)velocity < 0)
            velocity = uint32(0) - velocity;
        if ((sint32)velocity < 65)
        {
            uint32 target_angle = r_u32(state + 0x58);
            uint32 target_radius_height = r_u32(state + 0x5c);
            w_u32(state + 0x50, target_angle);
            w_u32(state + 0x54, target_radius_height);
            completed = 1;
        }
    }
    if (completed == 0)
    {
        state = r_u32(0x8009ebd8);
        uint32 velocity = r_u32(state + 0x40);
        uint32 change = G3Function80054C38(distance, velocity, frame);
        state = r_u32(0x8009ebd8);
        velocity = r_u32(state + 0x40) + change;
        w_u32(state + 0x40, velocity);
        if ((sint32)velocity < -700)
            velocity = (uint32)-700;
        if ((sint32)velocity > 700)
            velocity = 700;
        w_u32(state + 0x40, velocity);
        G3Function80054C6C(velocity, state + 0x50);
    }
    state = r_u32(0x8009ebd8);
    G3Function800547F0(state + 0x50, position);
    state = r_u32(0x8009ebd8);
    G3Function800144CC(rotation, state + 0x20, position, frame);
    return completed;
}

uint32 G3Function8005437C(uint32 object, uint32 rotation, uint32 position, uint32 owner_rotation, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005437c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    uint32 state = r_u32(0x8009ebd8);
    uint32 angle = r_u16(rotation);
    w_u16(state + 0x18, angle);
    w_u16(state + 0x1a, 0);
    uint32 owner_angle = r_u16(owner_rotation + 4);
    angle = r_u16(state + 0x18) & 0xfff;
    w_u16(state + 0x1c, owner_angle + 0x800);
    w_u16(state + 0x18, angle);
    if (angle >= 0x800)
        w_u16(state + 0x18, angle - 0x1000);
    state = r_u32(0x8009ebd8);
    sint32 signed_angle = r_s16(state + 0x18);
    angle = r_u16(state + 0x18);
    if (signed_angle < -950)
        angle = (uint32)-950;
    else if (signed_angle >= -100)
        angle = (uint32)-100;
    w_u16(state + 0x18, angle);
    uint32 first = r_u16(object);
    uint32 second = r_u16(position);
    uint32 difference = first - second;
    sint32 x = (sint16)(difference & 0xffff);
    uint32 square_x = (uint32)(x * x);
    w_u16(frame + 0x20, difference);
    first = r_u16(object + 2);
    second = r_u16(position + 2);
    difference = first - second;
    sint32 y = (sint16)(difference & 0xffff);
    uint32 square_y = (uint32)(y * y);
    w_u16(frame + 0x24, 0);
    uint32 squared = square_x + square_y;
    w_u16(frame + 0x22, difference);
    uint32 distance;
    if ((sint32)squared > 0x08000000)
    {
        distance = G3Function8002F368(squared, frame);
        if ((sint32)distance < 0)
            distance += 63;
        distance = (uint32)((sint32)distance >> 6);
    }
    else if ((sint32)squared > 0x00080000)
    {
        distance = G3Function8002F448(squared << 4, 4, frame);
        distance = (uint32)((sint32)(distance + 8) >> 4);
    }
    else
    {
        distance = G3Function8002F368(squared << 12, frame);
        distance = (uint32)((sint32)distance >> 12);
    }
    state = r_u32(0x8009ebd8);
    w_u16(state + 0x4c, distance);
    uint32 height = r_u16(object + 4);
    w_u16(state + 0x4e, height);
    uint32 current_angle = (uint32)r_s16(rotation + 4);
    w_u32(state + 0x48, current_angle);
    w_u32(frame + 0x10, owner_rotation);
    w_u32(frame + 0x14, 1);
    G3Function80054034(state + 0x12, state + 0x18, object, position, owner_rotation, 1, frame);
    state = r_u32(0x8009ebd8);
    second = r_u16(position);
    first = r_u16(state + 0x12);
    difference = first - second;
    x = (sint16)(difference & 0xffff);
    square_x = (uint32)(x * x);
    w_u16(frame + 0x20, difference);
    first = r_u16(state + 0x14);
    second = r_u16(position + 2);
    difference = first - second;
    y = (sint16)(difference & 0xffff);
    square_y = (uint32)(y * y);
    w_u16(frame + 0x24, 0);
    squared = square_x + square_y;
    w_u16(frame + 0x22, difference);
    if ((sint32)squared > 0x08000000)
    {
        distance = G3Function8002F368(squared, frame);
        if ((sint32)distance < 0)
            distance += 63;
        distance = (uint32)((sint32)distance >> 6);
    }
    else if ((sint32)squared > 0x00080000)
    {
        distance = G3Function8002F448(squared << 4, 4, frame);
        distance = (uint32)((sint32)(distance + 8) >> 4);
    }
    else
    {
        distance = G3Function8002F368(squared << 12, frame);
        distance = (uint32)((sint32)distance >> 12);
    }
    state = r_u32(0x8009ebd8);
    height = r_u16(state + 0x16);
    uint32 target_angle = (uint32)r_s16(state + 0x1c);
    current_angle = r_u32(state + 0x48);
    w_u16(state + 0x5c, distance);
    current_angle &= 0xfff;
    w_u16(state + 0x5e, height);
    w_u32(state + 0x58, target_angle);
    w_u32(state + 0x48, current_angle);
    if (current_angle >= 0x800)
        w_u32(state + 0x48, current_angle - 0x1000);
    state = r_u32(0x8009ebd8);
    target_angle = r_u32(state + 0x58) & 0xfff;
    w_u32(state + 0x58, target_angle);
    if (target_angle >= 0x800)
        w_u32(state + 0x58, target_angle - 0x1000);
    state = r_u32(0x8009ebd8);
    w_u32(state + 0x44, 1);
    G3Function8005489C(state + 0x44, state + 0x48, state + 0x58);
    state = r_u32(0x8009ebd8);
    angle = r_u16(state + 0x1c) & 0xfff;
    w_u16(state + 0x1c, angle);
    return angle;
}

uint32 G3Function80050F90(uint32 object, uint32 game, uint32 argument2, uint32 argument3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80050f90, "SLUS_008.06");
    G3FillMemory80086094(0x800a2374, 0, 22);
    uint32 distance = r_u16(0x80096b68);
    w_u16(0x800a2376, (uint32)-1024);
    w_u16(0x800a2378, (uint32)-1024);
    w_u16(0x800a2374, distance);
    w_u16(0x800a237c, distance);
    uint32 angle = r_u16(object + 0x1c);
    w_u16(0x800a237a, angle);
    return angle;
}

uint32 G3Function80050FE8(uint32 object, uint32 game, uint32 argument2, uint32 argument3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80050fe8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    sint32 tilt = r_s16(0x800a2376);
    uint32 high = r_masked_u32(0x80096b38, 15);
    uint32 low = r_masked_u32(0x80096b38, 15);
    sint32 height = r_s16(0x80096b3c);
    uint32 destination = frame + 0x18;
    uint32 high_address = destination + 3;
    uint32 high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, low >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
    uint32 low_lane = destination & 3;
    w_masked_u32(destination & 0xfffffffc, low << (low_lane * 8), (15u << low_lane) & 15);
    w_u16(frame + 0x1c, (uint32)height);
    uint32 output = argument2;
    if (tilt == -1024)
    {
        uint32 source = frame + 0x18;
        high_address = source + 3;
        high_lane = high_address & 3;
        high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        low_lane = source & 3;
        low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
        uint32 word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        height = r_s16(frame + 0x1c);
        high_address = output + 3;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        low_lane = output & 3;
        w_masked_u32(output & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
        w_u16(output + 4, (uint32)height);
    }
    else
    {
        uint32 distance = r_u16(0x800a2374);
        w_u16(frame + 0x10, 0);
        w_u16(frame + 0x12, 0);
        w_u16(frame + 0x14, 0);
        w_u16(frame + 0x12, uint32(0) - distance);
        G3Function8002F75C(frame + 0x10, (uint32)tilt);
        uint32 angle = (uint32)r_s16(0x800a237a);
        G3Function8002F8E4(frame + 0x10, angle);
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 offset = r_u16(frame + 0x10 + axis * 2);
            uint32 origin = r_u16(frame + 0x18 + axis * 2);
            w_u16(output + axis * 2, offset + origin);
        }
    }
    if ((r_s16(0x800a237e) != 0 || r_s16(0x800a2380) != 0 || r_s16(0x800a2382) != 0) && (r_s16(0x800a2384) != 0 || r_s16(0x800a2386) != 0 || r_s16(0x800a2388) != 0))
    {
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            sint32 lower = r_s16(0x800a237e + axis * 2);
            sint32 current = r_s16(output + axis * 2);
            uint32 selected = r_u16(0x800a237e + axis * 2);
            uint32 current_bits = r_u16(output + axis * 2);
            if (lower < current)
                selected = current_bits;
            sint32 upper = r_s16(0x800a2384 + axis * 2);
            uint32 result = (uint32)upper;
            if ((sint16)(selected & 0xffff) < upper)
                result = selected;
            w_u16(output + axis * 2, result);
        }
    }
    uint32 target_bits = r_u16(0x800a237c);
    sint32 target = r_s16(0x800a237c);
    sint32 current = r_s16(0x800a2374);
    uint32 current_bits = r_u16(0x800a2374);
    if (target != current)
    {
        sint32 difference = (sint16)((target_bits - current_bits) & 0xffff);
        sint32 magnitude = difference < 0 ? -difference : difference;
        if (magnitude < 41)
            w_u16(0x800a2374, target_bits);
        else if (difference > 0)
        {
            uint32 distance = r_u16(0x800a2374);
            w_u16(0x800a2374, distance + 40);
        }
        else
            w_u16(0x800a2374, current_bits - 40);
    }
    target_bits = r_u16(0x800a2378);
    target = r_s16(0x800a2378);
    current = r_s16(0x800a2376);
    current_bits = r_u16(0x800a2376);
    if (target != current)
    {
        sint32 difference = (sint16)((target_bits - current_bits) & 0xffff);
        sint32 magnitude = difference < 0 ? -difference : difference;
        if (magnitude < 41)
            w_u16(0x800a2376, target_bits);
        else
            w_u16(0x800a2376, difference > 0 ? current_bits + 40 : current_bits - 40);
    }
    uint32 angle = r_u16(0x800a2376);
    w_u16(output + 0xe, 0);
    w_u16(output + 0xc, angle);
    angle = r_u16(0x800a237a);
    w_u16(output + 0x10, angle);
    return angle;
}

uint32 G3Function800512C0(uint32 object, uint32 game, uint32 argument2, uint32 argument3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800512c0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    G3Function80050FE8(object, game, frame + 0x10, argument3, frame);
    return G3Function8005A248(object, frame + 0x10);
}

uint32 G3Function800512F4(uint32 object, uint32 game, uint32 argument2, uint32 argument3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800512f4, "SLUS_008.06");
    uint32 angle = r_u16(0x800a237a);
    uint32 change = r_u16(argument2 + 6);
    uint32 result = (angle + change) & 0xfff;
    w_u16(0x800a237a, result);
    return result;
}

uint32 G3Function800788CC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800788cc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 object = a0;
    uint32 output = a1;
    sint32 mode = r_s32(object + 0x154);
    uint32 data = r_u32(object + 0x24);
    uint32 handled = 0;
    if (mode < 0)
    {
        uint32 linked = r_u32(data + 0x1fc);
        if (linked != 0)
        {
            uint32 target = r_u32(linked + 0xec);
            if (target != 0)
            {
                uint32 game = r_u32(0x80097048);
                handled = G3DispatchStateEvent(target, linked, game, 0x7ed, output, frame) != 0;
            }
        }
    }
    if (handled != 0)
        return 0x8000;
    mode = r_s32(object + 0x154);
    if (mode == 0x8000)
    {
        uint32 matrix = r_u32(object + 0x30);
        uint32 value = r_u16(matrix + 0x34);
        w_u16(output, value);
        matrix = r_u32(object + 0x30);
        value = r_u16(matrix + 0x38);
        w_u16(output + 2, value);
        matrix = r_u32(object + 0x30);
        value = r_u16(matrix + 0x3c);
        w_u16(output + 4, value - 0x72);
    }
    else
    {
        uint32 source = object + 0x58;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
        uint32 word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        sint32 z = r_s16(object + 0x5c);
        high_address = output + 3;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        low_lane = output & 3;
        w_masked_u32(output & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
        w_u16(output + 4, (uint32)z);
    }
    uint32 result = r_u16(output + 4) + 0x100;
    w_u16(output + 4, result);
    return result;
}

void G3Function800657C0(void)
{
    FUNCTION_MARKER(0x800657c0, "SLUS_008.06");
    w_u32(0x8009efa4, 0);
    w_u32(0x8009efa8, 0);
    w_u32(0x8009efac, 0);
    w_u32(0x8009efb0, 0);
}

uint32 G3Function80046B1C(uint32 index)
{
    FUNCTION_MARKER(0x80046b1c, "SLUS_008.06");
    uint32 record = 0x800a218c + (index << 4);
    w_u16(record + 4, 5);
    return 5;
}

uint32 G3Function80018D1C(uint32 object, uint32 sequence, uint32 mode, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80018d1c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 step = 1;
    do
    {
        uint32 game = r_u32(0x80097048);
        uint32 flags = r_u32(game + 0x6834);
        if ((flags & 0x40) == 0 || (r_u32(sequence) & 0x80000000) == 0)
        {
            uint32 opcode = r_u32(sequence);
            uint32 target = r_u32(0x8009d3b8 + ((opcode & 0x7fffffff) << 2));
            step = G3DispatchScriptCommand(target, object, sequence, frame);
        }
        sequence += step << 2;
    }
    while (step != 0);
    return 0;
}

uint32 G3Function8004F044(uint32 camera, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004f044, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 when_done = 0;
    uint32 at_start = 0;
    uint32 allow_skip = 1;
    uint32 allow_fast = 1;
    uint32 extra = 0;
    uint32 loop_value = 0;
    uint32 loop_enabled = 0;
    G3FillMemory80086094(0x800a22a4, 0, 16);
    uint32 count = r_u32(command + 8);
    uint32 record = command + 12;
    uint32 end = command + (count << 2);
    if (record < end)
    {
        uint32 is_at_key = r_u32(0x8009e9c4);
        uint32 look_key = r_u32(0x8009e9d0);
        uint32 mode_key = r_u32(0x8009e9f0);
        uint32 done_key = r_u32(0x8009e9d8);
        uint32 start_key = r_u32(0x8009e9e4);
        uint32 first_mode = r_u32(0x8009e9f8);
        uint32 no_fast_key = r_u32(0x8009ea14);
        do
        {
            uint32 word = r_u32(record);
            uint32 second;
            uint32 expected;
            uint32 value;
            if (word == is_at_key)
            {
                second = r_u32(record + 4);
                expected = r_u32(0x8009e9c8);
                if (second == expected)
                {
                    value = r_u32(record + 12);
                    if (value != 0)
                        value = r_u32(value + 0x24);
                    w_u32(0x800a22ac, value);
                    goto next_record;
                }
                word = r_u32(record);
            }
            if (word == look_key)
            {
                value = r_u32(record + 12);
                if (value != 0)
                    value = r_u32(value + 0x24);
                w_u32(0x800a22b0, value);
                goto next_record;
            }
            if (word == done_key)
            {
                second = r_u32(record + 4);
                expected = r_u32(0x8009e9dc);
                if (second == expected)
                {
                    when_done = r_u32(record + 12);
                    goto next_record;
                }
            }
            word = r_u32(record);
            if (word == start_key)
            {
                second = r_u32(record + 4);
                expected = r_u32(0x8009e9e8);
                if (second == expected)
                {
                    at_start = r_u32(record + 12);
                    goto next_record;
                }
            }
            word = r_u32(record);
            if (word == mode_key)
            {
                value = r_u32(record + 12);
                if (value == first_mode)
                    w_u32(0x80096a8c, 0);
                else
                {
                    expected = r_u32(0x8009ea00);
                    if (value == expected)
                        w_u32(0x80096a8c, 8);
                }
                goto next_record;
            }
            expected = r_u32(0x8009ea08);
            if (word == expected)
            {
                second = r_u32(record + 4);
                expected = r_u32(0x8009ea0c);
                if (second == expected)
                {
                    allow_skip = 0;
                    goto next_record;
                }
            }
            word = r_u32(record);
            if (word == no_fast_key)
            {
                second = r_u32(record + 4);
                expected = r_u32(0x8009ea18);
                if (second == expected)
                {
                    allow_fast = 0;
                    goto next_record;
                }
            }
            word = r_u32(record);
            expected = r_u32(0x8009ea20);
            if (word == expected)
            {
                extra = 1;
                goto next_record;
            }
            expected = r_u32(0x8009ea28);
            if (word == expected)
            {
                second = r_u32(record + 4);
                expected = r_u32(0x8009ea2c);
                if (second == expected)
                {
                    loop_value = r_u16(record + 12);
                    loop_enabled = 1;
                }
            }
        next_record:
            uint32 step = r_u32(record + 8);
            record += step << 2;
        }
        while (record < end);
    }
    uint32 result = r_u32(0x800a22ac);
    if (result == 0)
        return result;
    G3Function80016F00();
    result = G3Function80013C94(camera, 5, frame);
    if (result == 0)
        return G3Function80016F24();
    w_u32(0x800a22a4, when_done);
    w_u32(0x800a22a8, at_start);
    w_u32(0x800a22b4, allow_skip);
    w_u32(0x800a22ec, allow_fast);
    w_u32(0x800a22bc, extra);
    w_u16(0x800a22e8, loop_enabled);
    w_u16(0x800a22ea, loop_value);
    uint32 game = r_u32(0x80097048);
    uint32 flags = r_u32(game + 0x6834);
    if (allow_skip != 0)
    {
        result = flags | 0x80;
        w_u32(game + 0x6834, result);
    }
    else
    {
        w_u32(game + 0x6834, flags & 0xffffff7f);
        result = game;
    }
    return result;
}

void G3Function800429B4(uint32 owner, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800429b4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 node = r_u32(owner + 0x2c04);
    while (node != 0)
    {
        uint32 flags = r_u32(node + 8);
        w_u32(node + 8, (flags & 4) != 0 ? flags | 1 : flags & 0xfffffffe);
        flags = r_u32(node + 8);
        w_u32(node + 8, flags & 0xfffffffd);
        node = r_u32(node + 4);
    }
    uint32 effect = r_u32(owner + 0x11c14);
    while (effect != 0)
    {
        uint32 associated = r_u32(effect + 0x14);
        uint32 flags = r_u32(associated + 8);
        uint32 next = r_u32(effect + 4);
        w_u32(associated + 8, flags | 1);
        uint32 target = r_u32(effect + 0x10);
        if (target != 0)
            G3DispatchPoolCallback(target, effect, owner, frame);
        effect = next;
    }
    node = r_u32(owner + 0x2c04);
    while (node != 0)
    {
        uint32 flags = r_u32(node + 8);
        uint32 next = r_u32(node + 4);
        if ((flags & 1) == 0)
        {
            G3Function80045BA0(node);
            G3Function80045B74(owner + 0x2c08, node);
        }
        node = next;
    }
}

uint32 G3Function80075A28(uint32 effect, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80075a28, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 y = r_u16(effect + 0xa);
    uint32 x = r_u16(effect + 8);
    uint32 z = r_u16(effect + 0xc);
    w_u16(effect + 0x12, y);
    uint32 kind = r_u8(effect + 0x1b);
    w_u16(effect + 0x10, x);
    w_u16(effect + 0x14, z);
    if (kind == 1)
        return G3Function80075F28(effect, frame);
    if (kind == 0)
        return G3Function80075B54(effect, frame);
    if (kind == 2)
        return G3Function800760C8(effect, frame);
    return 2;
}

void G3Function80075314(uint32 effect, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80075314, "SLUS_008.06");
    uint32 owner = r_u32(0x800a02e4);
    uint32 node = r_u32(owner);
    if (node == 0)
        return;
    uint32 next = r_u32(node);
    uint32 active = r_u32(owner + 4);
    w_u32(owner, next);
    w_u32(node, active);
    w_u32(owner + 4, node);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 value = r_u16(effect + 0x10 + axis * 2);
        w_u16(node + 4 + axis * 2, value);
    }
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 value = r_u16(effect + 8 + axis * 2);
        w_u16(node + 0xc + axis * 2, value);
    }
    uint32 value = r_u8(effect + 0x1f);
    w_u16(node + 0x12, value);
    for (uint32 channel = 0; channel < 3; ++channel)
    {
        value = r_u8(effect + 0x18 + channel);
        w_u8(node + 0x14 + channel, value);
    }
    value = r_u16(effect + 0x16);
    w_u16(node + 0xa, value);
    if (value == 0)
    {
        uint32 random = G3SdkRandom80027FA8();
        uint32 adjusted = (sint32)random < 0 ? random + 7 : random;
        uint32 quotient = (uint32)((sint32)adjusted >> 3);
        value = r_u16(node + 0x12);
        uint32 remainder = random - (quotient << 3);
        w_u16(node + 0x12, value + remainder);
    }
}

void G3Function800756C0(uint32 guest_stack)
{
    FUNCTION_MARKER(0x800756c0, "SLUS_008.06");
    uint32 owner = r_u32(0x800a02e4);
    uint32 frame = guest_stack - 0x18;
    uint32 node = r_u32(owner + 0xc14);
    uint32 previous_link = owner + 0xc14;
    while (node != 0)
    {
        sint32 lifetime = r_s16(node + 0xe);
        if (lifetime != 0)
        {
            G3Function80075A28(node, frame);
            G3Function80075314(node, frame);
            uint32 origins[3];
            uint32 changes[3];
            for (uint32 channel = 0; channel < 3; ++channel)
            {
                uint32 target = r_u8(node + 0x1c + channel);
                origins[channel] = r_u8(node + 0x18 + channel);
                lifetime = r_s16(node + 0xe);
                sint32 difference = (sint32)(target - origins[channel]);
                changes[channel] = lifetime != 0 ? (uint32)(difference / lifetime) : (difference < 0 ? 1 : 0xffffffff);
            }
            uint32 remaining = r_u16(node + 0xe);
            previous_link = node;
            w_u16(node + 0xe, remaining - 1);
            for (uint32 channel = 0; channel < 3; ++channel)
                w_u8(node + 0x18 + channel, origins[channel] + changes[channel]);
            node = r_u32(node);
        }
        else
        {
            uint32 expired = node;
            node = r_u32(expired);
            owner = r_u32(0x800a02e4);
            w_u32(previous_link, node);
            uint32 free_head = r_u32(owner + 0xc10);
            w_u32(expired, free_head);
            w_u32(owner + 0xc10, expired);
        }
    }
}

void G3Function8005EF40(uint32 pool, uint32 level, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005ef40, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 player = r_u32(0x8009efc4);
    uint32 state = r_u32(player + 0x150);
    if (state != 7)
    {
        uint32 game = r_u32(0x80097048);
        sint32 mode = r_s16(game + 0x67ee);
        if (mode != 1)
            G3Function8005EB00(player, level, frame);
    }
    uint32 slot = pool + 8;
    uint32 index = 1;
    do
    {
        uint32 node = r_u32(slot + 0x10);
        while (node != 0)
        {
            uint32 flags = r_u32(node + 0x18);
            if ((flags & 0x40) != 0)
                G3Function8005EB00(node, level, frame);
            node = r_u32(node + 4);
        }
        index += 2;
        slot += 0x10;
    }
    while (index < 128);
}

uint32 G3Function80020A44(uint32 object)
{
    FUNCTION_MARKER(0x80020a44, "SLUS_008.06");
    uint32 resource = object != 0 ? G3Function8003AA48(object) : r_u32(0x8009878c);
    if (resource == 0)
        return 0;
    uint32 pointer = r_u32(resource + 0xc);
    pointer = r_u32(pointer);
    pointer = r_u32(pointer + 0x14);
    uint32 primitive = r_u32(pointer + 8);
    uint32 value = r_u16(primitive + 6);
    w_u16(0x800a3c6c, value);
    value = r_u16(primitive + 2);
    w_u16(0x800a3c6e, value);
    for (uint32 axis = 0; axis < 2; ++axis)
    {
        uint32 first = r_u8(primitive + axis);
        uint32 second = r_u8(primitive + 4 + axis);
        uint32 third = r_u8(primitive + 8 + axis);
        if (first < second)
            value = first < third ? r_u8(primitive + axis) : r_u8(primitive + 8 + axis);
        else
            value = second < third ? r_u8(primitive + 4 + axis) : r_u8(primitive + 8 + axis);
        w_u16(0x800a3c70 + axis * 2, value);
    }
    uint32 first = r_u8(primitive);
    uint32 second = r_u8(primitive + 4);
    uint32 third = r_u8(primitive + 8);
    uint32 maximum;
    if (second < first)
        maximum = third < first ? first : r_u8(primitive + 8);
    else
        maximum = third < second ? r_u8(primitive + 4) : r_u8(primitive + 8);
    first = r_u8(primitive);
    second = r_u8(primitive + 4);
    third = r_u8(primitive + 8);
    uint32 minimum;
    if (first < second)
        minimum = first < third ? first : r_u8(primitive + 8);
    else
        minimum = second < third ? r_u8(primitive + 4) : r_u8(primitive + 8);
    uint32 result = maximum - minimum;
    w_u16(0x800a3c74, result);
    return result;
}

uint32 G3Function8004DC7C(uint32 command)
{
    FUNCTION_MARKER(0x8004dc7c, "SLUS_008.06");
    uint32 count = r_u32(command + 8);
    uint32 record = command + 12;
    uint32 end = command + (count << 2);
    if (record >= end)
        return 0x800a0000;
    uint32 key = r_u32(0x8009e984);
    do
    {
        uint32 word = r_u32(record);
        if (word == key)
        {
            uint32 enabled = r_u32(record + 12);
            uint32 flags = r_u32(0x80096b28);
            w_u32(0x80096b28, enabled != 0 ? flags | 1 : flags & 0xfffffffe);
        }
        uint32 step = r_u32(record + 8);
        record += step << 2;
    }
    while (record < end);
    return 0;
}

uint32 G3Function8004FF60(uint32 camera, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004ff60, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    bool has_angle = false;
    uint32 angle = 0;
    uint32 count = r_u32(command + 8);
    uint32 record = command + 12;
    uint32 end = command + (count << 2);
    uint32 path = 0;
    if (record < end)
    {
        uint32 angle_key = r_u32(0x8009ea34);
        uint32 path_key = r_u32(0x8009ea3c);
        do
        {
            uint32 word = r_u32(record);
            if (word == angle_key)
            {
                angle = r_u16(record + 12);
                has_angle = true;
            }
            else if (word == path_key)
            {
                uint32 source = r_u32(record + 12);
                path = source == 0 ? 0 : r_u32(source + 0x24);
            }
            uint32 step = r_u32(record + 8);
            record += step << 2;
        }
        while (record < end);
    }
    w_u32(0x80096af8, path);
    uint32 result = G3Function80013C94(camera, 18, frame);
    if (has_angle)
        w_u16(0x800a2304, angle);
    return result;
}

uint32 G3Function80057B70(uint32 camera, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80057b70, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 count = r_u32(command + 8);
    uint32 record = command + 12;
    uint32 end = command + (count << 2);
    if (record < end)
    {
        uint32 mode_key = r_u32(0x8009ece0);
        uint32 fast_key = r_u32(0x8009ece8);
        uint32 slow_key = r_u32(0x8009ecf0);
        do
        {
            uint32 word = r_u32(record);
            if (word == mode_key)
            {
                uint32 mode = r_u32(record + 12);
                if (mode == fast_key)
                    w_u32(0x80096a8c, 0);
                else if (mode == slow_key)
                    w_u32(0x80096a8c, 8);
            }
            uint32 step = r_u32(record + 8);
            record += step << 2;
        }
        while (record < end);
    }
    G3Function80013C94(camera, 16, frame);
    return G3Function80057C44(camera, command, frame);
}

uint32 G3Function8004DB64(uint32 camera, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004db64, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 mode = r_u32(0x80096a88);
    if (mode == 20)
        return 20;
    uint32 key = r_u32(command + 12);
    uint32 expected = r_u32(0x8009e958);
    if (key == expected)
        return G3Function80055BBC(camera, command, frame);
    expected = r_u32(0x8009e960);
    if (key == expected)
        return G3Function80051314(camera, command, frame);
    expected = r_u32(0x8009e968);
    if (key == expected)
        return G3Function8004FF60(camera, command, frame);
    expected = r_u32(0x8009e970);
    if (key == expected)
        return G3Function80057B70(camera, command, frame);
    expected = r_u32(0x8009e978);
    if (key != expected)
        return 0x8009e978;
    key = r_u32(command + 16);
    expected = r_u32(0x8009e97c);
    return key == expected ? G3Function8004DC7C(command) : expected;
}

uint32 G3Function8004DD0C(uint32 camera, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004dd0c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 word = r_u32(command + 12);
    uint32 expected = r_u32(0x8009e98c);
    bool matched = false;
    if (word == expected)
    {
        word = r_u32(command + 16);
        expected = r_u32(0x8009e990);
        matched = word == expected;
    }
    if (matched)
    {
        word = r_u32(command + 24);
        expected = r_u32(0x8009e998);
        matched = false;
        if (word == expected)
        {
            word = r_u32(command + 28);
            expected = r_u32(0x8009e99c);
            matched = word == expected;
        }
        if (matched)
        {
            uint32 result = r_u32(0x80096b28) | 0x100;
            w_u32(0x80096b28, result);
            return result;
        }
        word = r_u32(command + 24);
        expected = r_u32(0x8009e9a4);
        if (word != expected)
            return expected;
        uint32 result = r_u32(0x80096b28) | 0x800;
        w_u32(0x80096b28, result);
        return result;
    }
    word = r_u32(command + 12);
    expected = r_u32(0x8009e9ac);
    matched = false;
    if (word == expected)
    {
        word = r_u32(command + 16);
        expected = r_u32(0x8009e9b0);
        matched = word == expected;
    }
    if (matched)
    {
        word = r_u32(command + 24);
        expected = r_u32(0x8009e998);
        matched = false;
        if (word == expected)
        {
            word = r_u32(command + 28);
            expected = r_u32(0x8009e99c);
            matched = word == expected;
        }
        uint32 mask = 0xfffffeff;
        if (!matched)
        {
            word = r_u32(command + 24);
            expected = r_u32(0x8009e9a4);
            if (word != expected)
                return expected;
            mask = 0xfffff7ff;
        }
        uint32 result = r_u32(0x80096b28) & mask;
        w_u32(0x80096b28, result);
        return result;
    }
    word = r_u32(command + 12);
    expected = r_u32(0x8009e9b8);
    if (word != expected)
        return 0x8009e9b8;
    word = r_u32(command + 16);
    expected = r_u32(0x8009e9bc);
    if (word != expected)
        return expected;
    uint32 zoom = r_u32(command + 24);
    w_u32(0x80096a9c, zoom);
    G3Function80014900();
    uint32 mode = r_u32(0x80096a88);
    uint32 target = r_u32(0x80099654 + (mode << 3));
    if (target == 0)
        return 0;
    uint32 game = r_u32(0x80097048);
    zoom = r_u32(0x80096a9c);
    return G3DispatchStateEvent(target, camera, game, 13, zoom, frame);
}

uint32 G3Function80051314(uint32 camera, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80051314, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    bool has_tilt = false;
    bool has_distance = false;
    bool has_angle = false;
    bool has_bounds = false;
    uint32 tilt = 0;
    uint32 distance = 0;
    uint32 angle = 0;
    uint32 count = r_u32(command + 8);
    uint32 record = command + 12;
    uint32 end = command + (count << 2);
    while (record < end)
    {
        uint32 word = r_u32(record);
        uint32 expected = r_u32(0x8009ea70);
        if (word == expected)
        {
            uint32 shifted = r_u32(record + 12) << 12;
            sint64 product = (sint64)(sint32)shifted * (sint32)0xb60b60b7;
            uint32 high = (uint32)((uint64)product >> 32);
            uint32 sum = high + shifted;
            tilt = (uint32)((sint32)sum >> 8) - (uint32)((sint32)shifted >> 31);
            has_tilt = true;
            goto next_record;
        }
        expected = r_u32(0x8009ea78);
        if (word == expected)
        {
            distance = r_u16(record + 12);
            has_distance = true;
            goto next_record;
        }
        expected = r_u32(0x8009ea80);
        if (word == expected)
        {
            angle = r_u16(record + 12);
            has_angle = true;
            goto next_record;
        }
        expected = r_u32(0x8009ea88);
        if (word == expected)
        {
            uint32 mode = r_u32(record + 12);
            expected = r_u32(0x8009ea90);
            if (mode == expected)
                w_u32(0x80096a8c, 0);
            else
            {
                expected = r_u32(0x8009ea98);
                if (mode == expected)
                    w_u32(0x80096a8c, 8);
            }
            goto next_record;
        }
        expected = r_u32(0x8009eaa0);
        if (word == expected)
        {
            has_bounds = true;
            uint32 words[3];
            for (uint32 index = 0; index < 3; ++index)
            {
                uint32 source = record + 12 + index * 4;
                uint32 high_address = source + 3;
                uint32 high_lane = high_address & 3;
                uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
                uint32 low_lane = source & 3;
                uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
                words[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
            }
            for (uint32 index = 0; index < 3; ++index)
            {
                uint32 destination = frame + 0x10 + index * 4;
                uint32 high_address = destination + 3;
                uint32 high_lane = high_address & 3;
                w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
                uint32 low_lane = destination & 3;
                w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15u << low_lane) & 15);
            }
        }
    next_record:
        uint32 step = r_u32(record + 8);
        record += step << 2;
    }
    uint32 result = G3Function80013C94(camera, 21, frame);
    if (has_bounds)
    {
        uint32 words[3];
        for (uint32 index = 0; index < 3; ++index)
        {
            uint32 source = frame + 0x10 + index * 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low_lane = source & 3;
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
            words[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 index = 0; index < 3; ++index)
        {
            uint32 destination = 0x800a237e + index * 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            uint32 low_lane = destination & 3;
            w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15u << low_lane) & 15);
        }
    }
    if (has_tilt)
    {
        w_u16(0x800a2376, tilt);
        w_u16(0x800a2378, tilt);
    }
    if (has_distance)
    {
        w_u16(0x800a2374, distance);
        w_u16(0x800a237c, distance);
    }
    if (has_angle)
        w_u16(0x800a237a, angle);
    return result;
}

uint32 G3Function80055BBC(uint32 camera, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80055bbc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 inverse = 0;
    uint32 minimum = 0xffffffff;
    uint32 maximum = 0xffffffff;
    uint32 distance = r_u16(0x80096b68);
    uint32 tilt = r_u16(0x80096b6a);
    w_u32(0x80096a10, 0);
    w_u16(0x800a2474, distance);
    w_u16(0x800a2476, tilt);
    uint32 count = r_u32(command + 8);
    uint32 record = command + 12;
    uint32 end = command + (count << 2);
    uint32 path = 0;
    if (record < end)
    {
        uint32 maximum_key = r_u32(0x8009ebf0);
        uint32 path_key = r_u32(0x8009ebdc);
        uint32 inverse_key = r_u32(0x8009ebe4);
        uint32 minimum_key = r_u32(0x8009ebfc);
        uint32 distance_key = r_u32(0x8009ec08);
        uint32 tilt_key = r_u32(0x8009ec14);
        do
        {
            uint32 word = r_u32(record);
            uint32 second;
            uint32 expected;
            uint32 value;
            if (word == path_key)
            {
                path = r_u32(record + 12);
                goto next_record;
            }
            if (word == inverse_key)
            {
                second = r_u32(record + 4);
                expected = r_u32(0x8009ebe8);
                if (second == expected)
                {
                    inverse = 1;
                    goto next_record;
                }
            }
            word = r_u32(record);
            if (word == maximum_key)
            {
                second = r_u32(record + 4);
                expected = r_u32(0x8009ebf4);
                if (second == expected)
                {
                    maximum = r_u32(record + 12);
                    if ((sint32)maximum < 0)
                        maximum = uint32(0) - maximum;
                    goto next_record;
                }
                word = r_u32(record);
            }
            if (word == minimum_key)
            {
                second = r_u32(record + 4);
                expected = r_u32(0x8009ec00);
                if (second == expected)
                {
                    minimum = r_u32(record + 12);
                    if ((sint32)minimum < 0)
                        minimum = uint32(0) - minimum;
                    goto next_record;
                }
            }
            word = r_u32(record);
            if (word == distance_key)
            {
                second = r_u32(record + 4);
                expected = r_u32(0x8009ec0c);
                if (second == expected)
                {
                    value = r_u32(record + 12);
                    if ((sint32)value < 0)
                        value = uint32(0) - value;
                    w_u16(0x800a2474, value);
                    goto next_record;
                }
                word = r_u32(record);
            }
            if (word == tilt_key)
            {
                second = r_u32(record + 4);
                expected = r_u32(0x8009ec18);
                if (second == expected)
                {
                    value = r_u32(record + 12);
                    if ((sint32)value < 0)
                        value = uint32(0) - value;
                    w_u16(0x800a2476, uint32(0) - value);
                    goto next_record;
                }
            }
            word = r_u32(record);
            expected = r_u32(0x8009ec20);
            if (word == expected)
            {
                second = r_u32(record + 4);
                expected = r_u32(0x8009ec24);
                if (second == expected)
                {
                    value = r_u32(record + 12);
                    if ((sint32)value < 0)
                        value = uint32(0) - value;
                    w_u32(0x80096a10, value);
                    goto next_record;
                }
            }
            word = r_u32(record);
            expected = r_u32(0x8009ec2c);
            if (word == expected)
            {
                value = r_u32(record + 12);
                expected = r_u32(0x8009ec34);
                if (value == expected)
                    w_u32(0x80096a8c, 0);
                else
                {
                    expected = r_u32(0x8009ec3c);
                    if (value == expected)
                        w_u32(0x80096a8c, 8);
                    else
                    {
                        expected = r_u32(0x8009ec44);
                        if (value == expected)
                            w_u32(0x80096a8c, (uint32)-8);
                    }
                }
            }
        next_record:
            uint32 step = r_u32(record + 8);
            record += step << 2;
        }
        while (record < end);
    }
    if (path != 0)
        w_u32(0x80096a84, path);
    uint32 result = r_u32(0x80096a84);
    if (result == 0)
        return 0;
    G3Function80013C94(camera, 2, frame);
    w_u16(0x800a2472, inverse);
    if ((sint32)minimum >= 0)
        w_u16(0x800a245e, minimum);
    if ((sint32)maximum >= 0)
        w_u16(0x800a245c, maximum);
    sint32 signed_maximum = r_s16(0x800a245c);
    sint32 signed_minimum = r_s16(0x800a245e);
    uint32 minimum_bits = r_u16(0x800a245e);
    result = signed_maximum < signed_minimum;
    if (result != 0)
        w_u16(0x800a245c, minimum_bits);
    return result;
}
