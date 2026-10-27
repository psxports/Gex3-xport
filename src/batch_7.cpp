#include "psx.h"
#include "xport.h"
#include "game_functions.h"

uint32 G3Function80058D50(uint32 position, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80058d50, "SLUS_008.06");
    sint32 target_angle = r_s16(0x800a252a);
    sint32 current_angle = r_s16(0x800a2526);
    uint32 frame = guest_stack - 0x28;
    if (target_angle == current_angle)
        return (uint32)current_angle;
    uint32 flags = r_u32(0x800a24d0);
    if ((flags & 1) != 0)
        return 1;
    uint32 controlled = r_u32(0x8009efc4);
    uint32 movement = G3Function80078834(controlled, frame);
    if (movement == 0)
        return 0;
    uint32 state_x = r_u16(state);
    uint32 position_x = r_u16(position);
    uint32 delta_x = state_x - position_x;
    sint32 x = (sint16)(delta_x & 0xffff);
    uint32 square_x = (uint32)(x * x);
    w_u16(frame + 0x10, delta_x);
    uint32 state_y = r_u16(state + 2);
    uint32 position_y = r_u16(position + 2);
    uint32 delta_y = state_y - position_y;
    sint32 y = (sint16)(delta_y & 0xffff);
    uint32 square_y = (uint32)(y * y);
    w_u16(frame + 0x12, delta_y);
    uint32 state_z = r_u16(state + 4);
    uint32 position_z = r_u16(position + 4);
    uint32 saved_angle = r_u16(0x800a2526);
    uint32 delta_z = state_z - position_z;
    uint32 square_sum = square_x + square_y;
    w_u16(frame + 0x14, 0);
    uint32 horizontal;
    if ((sint32)square_sum > 0x08000000)
    {
        uint32 root = G3Function8002F368(square_sum, frame);
        if ((sint32)root < 0)
            root += 63;
        horizontal = (uint32)((sint32)root >> 6);
    }
    else if ((sint32)square_sum > 0x00080000)
    {
        uint32 root = G3Function8002F448(square_sum << 4, 4, frame);
        horizontal = (uint32)((sint32)(root + 8) >> 4);
    }
    else
    {
        uint32 root = G3Function8002F368(square_sum << 12, frame);
        horizontal = (uint32)((sint32)root >> 12);
    }
    uint32 angle = G3Function8002F240((uint32)(sint32)(sint16)(delta_z & 0xffff), horizontal, frame);
    uint32 difference = G3Function80038B48(angle & 0xfff, (uint32)(sint32)(sint16)(saved_angle & 0xffff));
    sint32 signed_difference = (sint16)(difference & 0xffff);
    sint32 threshold = r_s32(0x8009edcc);
    uint32 result = uint32(0) - (uint32)signed_difference;
    if (threshold < (sint32)result)
    {
        uint32 current_flags = r_u32(0x800a24d0);
        result = current_flags | 0x4000;
        w_u32(0x800a24d0, result);
    }
    return result;
}

uint32 G3Function80058ED0(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80058ed0, "SLUS_008.06");
    uint32 active = r_u32(state + 0x2c);
    if (active == 0)
        return 0;
    uint32 offset = r_u32(0x800a2534);
    if (offset == 0)
        return active;
    uint32 magnitude = (sint32)offset < 0 ? uint32(0) - offset : offset;
    if ((sint32)magnitude < 81)
    {
        w_u32(0x800a2534, 0);
        return 1;
    }
    uint32 result = (sint32)offset <= 0 ? offset + 80 : offset - 80;
    w_u32(0x800a2534, result);
    return result;
}

uint32 G3Function80078834(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80078834, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 movement = r_u32(object + 0x150);
    uint32 state = r_u32(object + 0x24);
    if (movement == 1)
        return 1;
    uint32 mode = r_u32(object + 0x154);
    if (mode == 0x8000)
        return 1;
    uint32 linked = r_u32(state + 0x1fc);
    if (linked != 0)
    {
        uint32 target = r_u32(linked + 0xec);
        if (target == 0)
            return 0;
        uint32 game = r_u32(0x80097048);
        return G3DispatchStateEvent(target, linked, game, 2035, 0, frame);
    }
    if (state == 0)
        return 0;
    uint32 flags = r_u16(state + 0xce);
    return (flags & 0x80) != 0 ? 1 : 0;
}

uint32 G3Function80058BA4(uint32 vector, uint32 position, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80058ba4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 saved_z = r_u16(vector + 4);
    uint32 words[2];
    for (uint32 index = 0; index != 2; ++index)
    {
        uint32 source = vector + index * 4;
        uint32 upper = r_u32((source + 3) & ~uint32(3));
        uint32 lower = r_u32(source & ~uint32(3));
        uint32 shift = (source & 3) << 3;
        words[index] = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
    }
    for (uint32 index = 0; index != 2; ++index)
    {
        uint32 destination = frame + 0x10 + index * 4;
        uint32 value = words[index];
        uint32 high_address = destination + 3;
        uint32 high_byte = high_address & 3;
        uint32 preserve = high_byte == 3 ? 0 : uint32(0xffffffff) << ((high_byte + 1) << 3);
        uint32 previous = r_u32(high_address & ~uint32(3));
        w_u32(high_address & ~uint32(3), (previous & preserve) | (value >> ((3 - high_byte) << 3)));
        uint32 low_byte = destination & 3;
        preserve = low_byte == 0 ? 0 : uint32(0xffffffff) >> ((4 - low_byte) << 3);
        previous = r_u32(destination & ~uint32(3));
        w_u32(destination & ~uint32(3), (previous & preserve) | (value << (low_byte << 3)));
    }
    sint32 x = r_s16(frame + 0x10);
    uint32 square_x = (uint32)(x * x);
    sint32 y = r_s16(frame + 0x12);
    uint32 square_y = (uint32)(y * y);
    uint32 square_sum = square_x + square_y;
    w_u16(frame + 0x14, 0);
    uint32 horizontal;
    if ((sint32)square_sum > 0x08000000)
    {
        uint32 root = G3Function8002F368(square_sum, frame);
        if ((sint32)root < 0)
            root += 63;
        horizontal = (uint32)((sint32)root >> 6);
    }
    else if ((sint32)square_sum > 0x00080000)
    {
        uint32 root = G3Function8002F448(square_sum << 4, 4, frame);
        horizontal = (uint32)((sint32)(root + 8) >> 4);
    }
    else
    {
        uint32 root = G3Function8002F368(square_sum << 12, frame);
        horizontal = (uint32)((sint32)root >> 12);
    }
    uint32 tilt = G3Function8002F240((uint32)(sint32)(sint16)(saved_z & 0xffff), horizontal, frame);
    uint32 limit = r_u32(0x8009ec50);
    uint32 angle = tilt & 0xfff;
    if ((sint32)limit < (sint32)angle || angle < 0x404)
    {
        uint32 previous = r_u32(0x800a253c);
        sint32 threshold = r_s32(0x8009edd8);
        uint32 incremented = previous + 1;
        w_u32(0x800a253c, incremented);
        if ((sint32)incremented >= threshold)
        {
            uint32 flags = r_u32(0x800a24d0);
            w_u32(0x800a24d0, flags | 0x4000);
        }
    }
    else
    {
        uint32 margin = r_u32(0x8009ec54);
        if ((sint32)angle < (sint32)(limit - margin))
            w_u32(0x800a253c, 0);
    }
    sint32 position_z = r_s16(position + 4);
    sint32 state_z = r_s16(state + 4);
    if (state_z >= position_z + 700 || state_z <= position_z - 2000)
    {
        uint32 flags = r_u32(0x800a24d0);
        uint32 result = flags | 0x4000;
        w_u32(0x800a24d0, result);
        return result;
    }
    return 1;
}

uint32 G3Function800599E0(uint32 position, uint32 state, uint32 delta, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800599e0, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x20;
    uint32 controlled = r_u32(game + 0x4c);
    uint32 active = r_u32(state + 0x2c);
    uint32 saved_angle = r_u16(controlled + 0x1c);
    if (active != 0)
    {
        uint32 flags = r_u32(0x800a2574);
        w_u32(0x800a2574, flags & 0xffffffcf);
    }
    uint32 fixed_scale = r_u32(0x80096b58);
    uint32 scale = 4096;
    if (fixed_scale == 0)
        scale = r_u32(0x800a25a0);
    G3Function80059740(position, state, delta, scale, frame);
    uint32 flags = r_u32(0x800a2574);
    if ((flags & 2) == 0)
        return uint32(0xfffffffd);
    w_u32(0x800a2574, flags & 0xfffffffd);
    uint32 object = r_u32(0x80096b34);
    uint32 facing = r_u16(object + 0x64);
    uint32 opposite = facing + 2048;
    uint32 difference = G3Function80038B48((uint32)(sint32)(sint16)(opposite & 0xffff), (uint32)(sint32)(sint16)(saved_angle & 0xffff));
    sint32 signed_difference = (sint16)(difference & 0xffff);
    if (signed_difference == 0)
        return uint32(0xffffffcf);
    uint32 current_flags = r_u32(0x800a2574);
    uint32 cleared = current_flags & 0xffffffcf;
    w_u32(0x800a2574, cleared);
    uint32 direction = signed_difference > 0 ? uint32(0x10) : uint32(0x20);
    w_u32(0x800a2574, cleared | direction);
    uint32 signed_opposite = (uint32)(sint32)(sint16)(opposite & 0xffff);
    uint32 duration = r_u32(0x8009ec70);
    uint32 result = signed_opposite - 1024;
    w_u32(0x800a2568, result);
    w_u32(0x800a256c, duration);
    return result;
}

uint32 G3Function8002FEE0(uint32 vector, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002fee0, "SLUS_008.06");
    uint32 magnitude = G3Function8002FA50(vector, guest_stack - 0x18);
    if (magnitude == 0)
        return 0;
    uint32 x = (uint32)(sint32)r_s16(vector);
    uint32 scaled_x = (uint32)((sint32)(x << 12) / (sint32)magnitude);
    uint32 y = (uint32)(sint32)r_s16(vector + 2);
    uint32 scaled_y = (uint32)((sint32)(y << 12) / (sint32)magnitude);
    uint32 z = (uint32)(sint32)r_s16(vector + 4);
    uint32 result = (uint32)((sint32)(z << 12) / (sint32)magnitude);
    w_u16(vector, scaled_x);
    w_u16(vector + 2, scaled_y);
    w_u16(vector + 4, result);
    return result;
}

uint32 G3Function80059740(uint32 position, uint32 state, uint32 delta, uint32 scale, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80059740, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    uint32 active = r_u32(state + 0x2c);
    if (active == 0)
        return 0;
    uint32 state_x = r_u16(state);
    uint32 position_x = r_u16(position);
    w_u16(frame + 0x10, state_x - position_x);
    uint32 state_y = r_u16(state + 2);
    uint32 position_y = r_u16(position + 2);
    w_u16(frame + 0x14, 0);
    w_u16(frame + 0x12, state_y - position_y);
    sint32 current_x = r_s16(state);
    sint32 previous_x = r_s16(state + 6);
    w_u32(frame + 0x18, (uint32)(current_x - previous_x));
    sint32 current_y = r_s16(state + 2);
    sint32 previous_y = r_s16(state + 8);
    w_u32(frame + 0x20, 0);
    w_u32(frame + 0x1c, (uint32)(current_y - previous_y));
    G3Function8002F8E4(frame + 0x10, uint32(0xfffffc00));
    G3Function8002FEE0(frame + 0x10, frame);
    uint32 axis_x = (uint32)(sint32)r_s16(frame + 0x10);
    uint32 motion_x = r_u32(frame + 0x18);
    uint32 product_x = motion_x * axis_x;
    uint32 axis_y_bits = r_u16(frame + 0x12);
    uint32 motion_y = r_u32(frame + 0x1c);
    uint32 axis_y = (uint32)(sint32)(sint16)(axis_y_bits & 0xffff);
    uint32 product_y = motion_y * axis_y;
    uint32 axis_z_bits = r_u16(frame + 0x14);
    uint32 motion_z = r_u32(frame + 0x20);
    uint32 axis_z = (uint32)(sint32)(sint16)(axis_z_bits & 0xffff);
    uint32 product_z = motion_z * axis_z;
    uint32 sum = product_x + product_y + product_z;
    if ((sint32)sum < 0)
        sum += 4095;
    uint32 projection = (uint32)((sint32)sum >> 12);
    uint32 flags = r_u32(0x8009ea44);
    if ((flags & 0x80000) != 0)
    {
        uint32 timer = r_u16(0x800a2580);
        uint32 begin = r_u16(0x8009ec60);
        uint32 elapsed = timer - begin;
        if ((elapsed & 0x8000) != 0)
            elapsed = 0;
        uint32 end = r_u16(0x8009ec64);
        sint32 duration = (sint16)((end - begin) & 0xffff);
        if (duration > 0)
        {
            uint32 coefficient = r_u32(0x8009ec68);
            uint32 scaled = projection * coefficient;
            scaled *= (uint32)(sint32)(sint16)(elapsed & 0xffff);
            projection = (uint32)((sint32)scaled / (duration * 100));
        }
    }
    uint32 negative_projection = uint32(0) - projection;
    uint32 numerators[3];
    uint32 quotients[3];
    for (uint32 index = 0; index != 3; ++index)
    {
        uint32 component;
        if (index == 0)
            component = (uint32)(sint32)r_s16(frame + 0x10);
        else
        {
            uint32 bits = r_u16(frame + 0x10 + index * 2);
            component = (uint32)(sint32)(sint16)(bits & 0xffff);
        }
        uint32 numerator = component * negative_projection;
        numerators[index] = numerator;
        if (scale == 0)
            quotients[index] = (sint32)numerator < 0 ? 1 : uint32(0xffffffff);
        else if (numerator == 0x80000000 && scale == 0xffffffff)
            quotients[index] = uint32(0x80000000);
        else
            quotients[index] = (uint32)((sint32)numerator / (sint32)scale);
    }
    for (uint32 index = 0; index != 3; ++index)
        w_u32(frame + 0x28 + index * 4, numerators[index]);
    for (uint32 index = 0; index != 3; ++index)
        w_u32(frame + 0x28 + index * 4, quotients[index]);
    sint32 x = (sint32)quotients[0];
    if (x < -32767)
        x = -32767;
    if (x > 32767)
        x = 32767;
    w_u16(delta, (uint32)x);
    sint32 y = r_s32(frame + 0x2c);
    if (y < -32767)
        y = -32767;
    if (y > 32767)
        y = 32767;
    w_u16(delta + 2, (uint32)y);
    sint32 z = r_s32(frame + 0x30);
    if (z < -32767)
        z = -32767;
    uint32 result = z > 32767 ? 1 : 0;
    if (result != 0)
        z = 32767;
    w_u16(delta + 4, (uint32)z);
    return result;
}

uint32 G3Function80059B18(uint32 position, uint32 state, uint32 delta, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80059b18, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    sint32 initial_radius = r_s16(0x800a24cc);
    (void)r_u32(0x80097048);
    uint32 square_radius = (uint32)(initial_radius * initial_radius);
    w_u16(delta, 0);
    w_u16(delta + 2, 0);
    w_u16(delta + 4, 0);
    uint32 flags = r_u32(0x800a24d0);
    if ((flags & 0x200) != 0)
        return 0x200;
    uint32 active = r_u32(state + 0x2c);
    if (active == 0)
        return 0;
    uint32 position_x = r_u16(position);
    uint32 state_x = r_u16(state);
    uint32 delta_x = position_x - state_x;
    sint32 x = (sint16)(delta_x & 0xffff);
    uint32 square_x = (uint32)(x * x);
    w_u16(frame + 0x10, delta_x);
    uint32 position_y = r_u16(position + 2);
    uint32 state_y = r_u16(state + 2);
    uint32 delta_y = position_y - state_y;
    sint32 y = (sint16)(delta_y & 0xffff);
    uint32 square_y = (uint32)(y * y);
    w_u16(frame + 0x14, 0);
    uint32 square_sum = square_x + square_y;
    w_u16(frame + 0x12, delta_y);
    if (square_sum == square_radius)
        return (uint32)y;
    if (x != 0 || y != 0)
    {
        uint32 radius = (uint32)(sint32)r_s16(0x800a24cc);
        G3Function8002F9A8(frame + 0x28, radius, frame + 0x10, frame);
        uint32 local_x = r_u16(frame + 0x28);
        state_x = r_u16(state);
        w_u16(frame + 0x28, local_x + state_x);
        uint32 local_y = r_u16(frame + 0x2a);
        state_y = r_u16(state + 2);
        w_u16(frame + 0x2a, local_y + state_y);
        uint32 local_z = r_u16(frame + 0x2c);
        uint32 state_z = r_u16(state + 4);
        w_u16(frame + 0x2c, local_z + state_z);
    }
    else
    {
        uint32 angle = (uint32)(sint32)r_s16(0x800a24ec);
        uint32 radius = r_u16(0x800a24cc);
        w_u16(frame + 0x28, 0);
        w_u16(frame + 0x2c, 0);
        w_u16(frame + 0x2a, uint32(0) - radius);
        G3Function8002F8E4(frame + 0x28, angle);
    }
    uint32 local_x = r_u16(frame + 0x28);
    position_x = r_u16(position);
    w_u16(frame + 0x18, local_x - position_x);
    uint32 local_y = r_u16(frame + 0x2a);
    position_y = r_u16(position + 2);
    w_u16(frame + 0x1c, 0);
    w_u16(frame + 0x1a, local_y - position_y);
    uint32 distance;
    if ((sint32)square_sum > 0x08000000)
    {
        uint32 root = G3Function8002F368(square_sum, frame);
        if ((sint32)root < 0)
            root += 63;
        distance = (uint32)((sint32)root >> 6);
    }
    else if ((sint32)square_sum > 0x00080000)
    {
        uint32 root = G3Function8002F448(square_sum << 4, 4, frame);
        distance = (uint32)((sint32)(root + 8) >> 4);
    }
    else
    {
        uint32 root = G3Function8002F368(square_sum << 12, frame);
        distance = (uint32)((sint32)root >> 12);
    }
    uint32 radius = r_u16(0x800a24cc);
    sint32 difference = (sint16)((radius - distance) & 0xffff);
    if (difference < 0)
        difference = -difference;
    sint32 limit = r_s32(0x8009ec5c);
    sint32 short_difference = (sint16)((uint32)difference & 0xffff);
    if (short_difference < limit)
        limit = short_difference;
    uint32 length = (uint32)(sint32)(sint16)((uint32)limit & 0xffff);
    G3Function8002F9A8(frame + 0x18, length, frame + 0x18, frame);
    uint32 previous_x = r_u16(delta);
    uint32 correction_x = r_u16(frame + 0x18);
    w_u16(delta, previous_x + correction_x);
    uint32 previous_y = r_u16(delta + 2);
    uint32 correction_y = r_u16(frame + 0x1a);
    w_u16(delta + 2, previous_y + correction_y);
    uint32 previous_z = r_u16(delta + 4);
    uint32 correction_z = r_u16(frame + 0x1c);
    uint32 result = previous_z + correction_z;
    w_u16(delta + 4, result);
    return result;
}

uint32 G3Function80058F48(uint32 position, uint32 state, uint32 delta, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80058f48, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    w_u16(delta, 0);
    w_u16(delta + 2, 0);
    w_u16(delta + 4, 0);
    uint32 object = r_u32(0x80096b34);
    uint32 movement = G3Function80078834(object, frame);
    uint32 controlled = r_u32(0x8009efc4);
    uint32 controlled_x = r_u16(controlled + 0xdc);
    w_u16(frame + 0x10, controlled_x);
    uint32 controlled_y = r_u16(controlled + 0xde);
    w_u16(frame + 0x12, controlled_y);
    uint32 controlled_z = r_u16(controlled + 0xe0);
    w_u16(frame + 0x14, controlled_z);
    if (movement == 0)
    {
        sint32 signed_z = r_s16(controlled + 0xe0);
        sint32 previous_z = r_s16(0x800a2544);
        uint32 previous_bits = r_u16(0x800a2544);
        if (signed_z < previous_z)
            w_u16(frame + 0x14, previous_bits);
    }
    uint32 local_x = r_u16(frame + 0x10);
    uint32 previous_x = r_u16(0x800a2540);
    uint32 local_y = r_u16(frame + 0x12);
    uint32 previous_y = r_u16(0x800a2542);
    uint32 local_z = r_u16(frame + 0x14);
    w_u16(frame + 0x20, local_x - previous_x);
    uint32 previous_z = r_u16(0x800a2544);
    w_u16(frame + 0x22, local_y - previous_y);
    uint32 height_change = local_z - previous_z;
    w_u16(frame + 0x24, height_change);
    sint32 absolute_change = (sint16)(height_change & 0xffff);
    if (absolute_change < 0)
        absolute_change = -absolute_change;
    if (absolute_change >= 61)
        w_u16(frame + 0x1c, 0);
    w_u16(0x800a2540, local_x);
    w_u16(0x800a2542, local_y);
    w_u16(0x800a2544, local_z);
    uint32 current_x = r_u16(state);
    uint32 old_x = r_u16(state + 6);
    w_u16(frame + 0x18, current_x - old_x);
    uint32 current_y = r_u16(state + 2);
    uint32 old_y = r_u16(state + 8);
    w_u16(frame + 0x1a, current_y - old_y);
    uint32 current_z = r_u16(state + 4);
    uint32 old_z = r_u16(state + 0xa);
    w_u16(frame + 0x1c, current_z - old_z);
    if (movement == 0)
    {
        uint32 previous_movement = r_u32(0x800a2530);
        if (previous_movement != 0)
        {
            uint32 game = r_u32(0x80097048);
            uint32 current_object = r_u32(game + 0x4c);
            uint32 saved_z = (uint32)(sint32)r_s16(current_object + 4);
            w_u32(0x800a2538, saved_z);
        }
    }
    uint32 motion_x = r_u16(frame + 0x18);
    w_u16(delta, motion_x);
    uint32 motion_y = r_u16(frame + 0x1a);
    w_u16(delta + 2, motion_y);
    uint32 direct = G3Function80058B54();
    if (direct != 0)
    {
        uint32 motion_z = r_u16(frame + 0x1c);
        w_u16(delta + 4, motion_z);
        return motion_z;
    }
    uint32 flags = r_u32(0x800a24d0);
    uint32 blocked = flags & 0x440;
    if (blocked != 0)
    {
        w_u16(delta + 4, 0);
        return blocked;
    }
    if (movement != 0)
    {
        sint32 motion_z = r_s16(frame + 0x1c);
        sint32 limit = r_s32(0x8009eddc);
        if (motion_z < 0)
            motion_z = -motion_z;
        sint32 short_motion = (sint16)((uint32)motion_z & 0xffff);
        if (short_motion < limit)
            limit = short_motion;
        limit = (sint16)((uint32)limit & 0xffff);
        uint32 change_bits = r_u16(frame + 0x24);
        w_u16(delta + 4, change_bits);
        sint32 change = (sint16)(change_bits & 0xffff);
        sint32 selected = -limit;
        if (selected < change)
            selected = change;
        uint32 result = selected < limit ? 1 : 0;
        if (result == 0)
            selected = limit;
        w_u16(delta + 4, (uint32)selected);
        return result;
    }
    if ((flags & 0x4000) != 0)
    {
        uint32 motion_z = r_u16(frame + 0x1c);
        w_u16(delta + 4, motion_z);
        return motion_z;
    }
    sint32 motion_z = r_s16(frame + 0x1c);
    if (motion_z >= 0)
        return 0;
    sint32 position_z = r_s16(position + 4);
    uint32 saved_z = r_u32(0x800a2538);
    uint32 result = (sint32)saved_z < position_z ? 1 : 0;
    uint32 correction = saved_z - (uint32)position_z;
    if (result == 0)
        return 0;
    result = (sint32)correction < motion_z ? 1 : 0;
    if (result != 0)
        correction = (uint32)motion_z;
    if ((sint32)correction > 0)
        correction = 0;
    w_u16(frame + 0x1c, correction);
    return result;
}

uint32 G3Function8005F3A8(uint32 object, uint32 delta)
{
    FUNCTION_MARKER(0x8005f3a8, "SLUS_008.06");
    uint32 matrix = r_u32(object + 0x30);
    if (matrix == 0)
        return 0;
    sint32 component = r_s16(delta);
    if (component == 0)
    {
        component = r_s16(delta + 2);
        if (component == 0)
        {
            component = r_s16(delta + 4);
            if (component == 0)
                return 0;
        }
    }
    uint32 flags = r_u32(object + 0x18);
    uint32 resource = r_u32(object + 0x1c);
    w_u32(object + 0x18, flags | 0x100);
    sint32 model_index = r_s16(object + 0xa2);
    uint32 models = r_u32(resource + 0xc);
    uint32 model = r_u32(models + ((uint32)model_index << 2));
    sint32 segments = r_s16(model + 6);
    matrix = r_u32(object + 0x30);
    uint32 end = matrix + ((uint32)segments << 5);
    uint32 result = matrix < end ? 1 : 0;
    uint32 translation = matrix + 0x1c;
    while (result != 0)
    {
        uint32 change = (uint32)r_s16(delta);
        uint32 value = r_u32(translation - 8);
        w_u32(translation - 8, value + change);
        change = (uint32)r_s16(delta + 2);
        value = r_u32(translation - 4);
        w_u32(translation - 4, value + change);
        change = (uint32)r_s16(delta + 4);
        value = r_u32(translation);
        matrix += 0x20;
        w_u32(translation, value + change);
        result = matrix < end ? 1 : 0;
        translation += 0x20;
    }
    return result;
}

uint32 G3Function8005F47C(uint32 object, uint32 delta, uint32 incoming_v0)
{
    FUNCTION_MARKER(0x8005f47c, "SLUS_008.06");
    uint32 matrix = r_u32(object + 0x34);
    if (matrix == 0)
        return incoming_v0;
    uint32 resource = r_u32(object + 0x1c);
    sint32 model_index = r_s16(object + 0xa2);
    uint32 models = r_u32(resource + 0xc);
    uint32 model = r_u32(models + ((uint32)model_index << 2));
    sint32 segments = r_s16(model + 6);
    uint32 end = matrix + ((uint32)segments << 5) + 0x20;
    uint32 result = matrix < end ? 1 : 0;
    uint32 translation = matrix + 0x1c;
    while (result != 0)
    {
        uint32 change = (uint32)r_s16(delta);
        uint32 value = r_u32(translation - 8);
        w_u32(translation - 8, value + change);
        change = (uint32)r_s16(delta + 2);
        value = r_u32(translation - 4);
        w_u32(translation - 4, value + change);
        change = (uint32)r_s16(delta + 4);
        value = r_u32(translation);
        matrix += 0x20;
        w_u32(translation, value + change);
        result = matrix < end ? 1 : 0;
        translation += 0x20;
    }
    return result;
}

uint32 G3Function8006C610(uint32 game)
{
    FUNCTION_MARKER(0x8006c610, "SLUS_008.06");
    uint32 value = r_u32(game + 0x6824);
    return G3Function80047EE4(4, value);
}

uint32 G3Function8007446C(uint32 game)
{
    FUNCTION_MARKER(0x8007446c, "SLUS_008.06");
    uint32 value = (uint32)r_s16(game + 0x681c);
    return G3Function80047EE4(7, value);
}

uint32 G3Function8007835C(uint32 object, uint32 position, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007835c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    w_u32(object + 0x154, 2);
    w_u16(object + 0x6e, 8);
    uint32 mode = r_u32(object + 0x128);
    uint32 flags = r_u32(object + 0x10);
    uint32 state = r_u32(object + 0x24);
    w_u32(object + 0x134, 0);
    w_u32(object + 0x150, 0);
    w_u16(object + 0x5e, 0);
    w_u16(object + 0x8e, 0);
    w_u32(object + 0x10c, 0);
    w_u32(object + 0x128, mode & 0x04000080);
    uint32 properties = r_u32(object + 0x14);
    w_u32(object + 0x10, flags & 0x18320487);
    uint32 related = r_u32(0x800a0284);
    w_u32(object + 0x14, properties & 0x40);
    w_u32(related + 0xc, 0);
    uint32 target = r_u16(position);
    uint32 current = r_u16(object + 0x58);
    w_u16(frame + 0x10, target - current);
    target = r_u16(position + 2);
    current = r_u16(object + 0x5a);
    w_u16(frame + 0x12, target - current);
    target = r_u16(position + 4);
    current = r_u16(object + 0x5c);
    w_u16(frame + 0x14, target - current);
    uint32 result = G3Function8005F3A8(object, frame + 0x10);
    G3Function8005F47C(object, frame + 0x10, result);
    flags = r_u32(game + 0x6834);
    w_u16(game + 0x67ee, 0);
    w_u32(game + 0x6834, flags & 0xffffffee);
    target = r_u16(position);
    w_u16(object + 0x58, target);
    target = r_u16(position + 2);
    w_u16(object + 0x5a, target);
    target = r_u16(position + 4);
    w_u16(object + 0x5c, target);
    target = r_u16(position);
    w_u16(object + 0x70, target);
    target = r_u16(position + 2);
    w_u16(object + 0x72, target);
    target = r_u16(position + 4);
    w_u16(object + 0x74, target);
    target = r_u16(position);
    w_u16(object + 0xdc, target);
    target = r_u16(position + 2);
    w_u16(object + 0xde, target);
    target = r_u16(position + 4);
    w_u16(object + 0x6c, 4096);
    w_u16(object + 0x6a, 4096);
    w_u16(object + 0x68, 4096);
    w_u16(object + 0x60, 0);
    w_u16(object + 0x62, 0);
    w_u16(object + 0xe0, target);
    uint32 level = r_u32(game + 0x48);
    uint32 current_game = r_u32(0x80097048);
    uint32 angle = r_u16(level + 0xb0);
    w_u16(object + 0x64, angle);
    level = r_u32(current_game + 0x48);
    angle = r_u16(level + 0xb0);
    w_u16(state + 0xc6, angle);
    w_u32(state + 0x1e8, 0);
    w_u32(state + 0x214, 0);
    w_u32(state + 0x220, 0);
    w_u32(state + 0x224, 0);
    w_u16(state + 0x256, 0);
    w_u16(state + 0x258, 0);
    w_u16(state + 0x254, 4096);
    w_u16(state + 0x120, 60);
    flags = r_u32(game + 0x6834);
    uint32 saved_mode = r_u16(game + 0x681e);
    w_u32(game + 0x6834, flags & 0xfffffffb);
    w_u16(game + 0x681c, saved_mode);
    G3Function8006C610(game);
    return G3Function8007446C(game);
}

uint32 G3Function80075218(uint32 object, uint32 position, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80075218, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 state = r_u32(object + 0x24);
    G3Function8007835C(object, position, game, frame);
    G3Function80074BD0(object, state);
    return G3Function80074B6C(state);
}

uint32 G3Function800786F0(uint32 object)
{
    FUNCTION_MARKER(0x800786f0, "SLUS_008.06");
    uint32 flags = r_u32(object + 0x10);
    if ((flags & 0x40000) != 0)
        return 0;
    uint32 mode = r_u32(object + 0x154);
    return (mode & 0x01881607) != 0 ? 1 : 0;
}

uint32 G3Function8006F358(uint32 state, uint32 payload, uint32 flag)
{
    FUNCTION_MARKER(0x8006f358, "SLUS_008.06");
    uint32 storage = 0;
    uint32 result = 0;
    if (flag == 2)
    {
        result = 3;
        storage = state + 0x20c;
    }
    else if (flag == 4)
    {
        result = 3;
        storage = state + 0x210;
    }
    else if (flag == 8)
        result = 1;
    else if (flag == 1)
    {
        result = 1;
        uint32 object = r_u32(0x8009efc4);
        uint32 game = r_u32(0x80097048);
        (void)game;
        if (G3Function800786F0(object) != 0)
        {
            result = 3;
            storage = state + 0x208;
        }
    }
    if (result == 0)
        return 0;
    uint32 flags = r_u32(state + 0x204);
    if ((flags & flag) != 0)
        return result;
    if (storage != 0)
        w_u32(storage, payload);
    flags = r_u32(state + 0x204);
    w_u32(state + 0x204, flags | flag);
    return 3;
}

uint32 G3Function80077648(uint32 vector)
{
    FUNCTION_MARKER(0x80077648, "SLUS_008.06");
    uint32 object = r_u32(0x8009efc4);
    uint32 state = r_u32(object + 0x24);
    uint32 value = r_u16(vector);
    w_u16(state + 0x264, value);
    value = r_u16(vector + 2);
    w_u16(state + 0x266, value);
    value = r_u16(vector + 4);
    w_u16(state + 0x268, value);
    return value;
}

uint32 G3Function8007801C(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007801c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 controlled = r_u32(0x8009efc4);
    uint32 state = r_u32(controlled + 0x24);
    switch (message)
    {
    case 2:
        G3Function800779B0(game, frame);
        return 3;
    case 3:
        G3Function8006E1B0(object, game, frame);
        return 3;
    case 6:
        return 3;
    case 8:
    {
        uint32 linked = r_u32(state + 0x1fc);
        if (linked == 0)
            G3Function80074380(object, game, payload, payload, frame);
        else
        {
            uint32 target = r_u32(linked + 0xec);
            if (target != 0)
                G3DispatchStateEvent(target, linked, game, 8, payload, frame);
        }
        return 3;
    }
    case 13:
        G3Function800777C0(object, game, payload, payload, frame);
        return 3;
    case 14:
        G3Function8006DFB8(object, game, payload, payload, frame);
        return 3;
    case 1001:
        G3Function8006F50C(object, game, message, payload, frame);
        return 3;
    case 1002:
        G3Function80076D3C(object, game, payload, payload, frame);
        return 3;
    case 1003:
        G3Function800770A0(object, game, payload, payload, frame);
        return 3;
    case 1004:
        return G3Function8006F358(state, payload, 1);
    case 1005:
        return G3Function8006F358(state, payload, 2);
    case 1006:
        return G3Function8006F358(state, payload, 4);
    case 1007:
        return G3Function8006F358(state, payload, 8);
    case 1008:
    {
        uint32 value = r_u16(state + 0x9c);
        uint32 change = r_u16(payload);
        w_u16(state + 0x9c, value + change);
        value = r_u16(state + 0x9e);
        change = r_u16(payload + 2);
        w_u16(state + 0x9e, value + change);
        value = r_u16(state + 0xa0);
        change = r_u16(payload + 4);
        w_u16(state + 0xa0, value + change);
        return 3;
    }
    case 1009:
        G3Function8007DE64(object, game, payload, payload, frame);
        return 3;
    case 1010:
        G3Function80080544(object, game, state, payload, frame);
        return 3;
    case 1011:
    {
        uint32 value = r_u16(payload);
        w_u16(state + 0x254, value);
        return 3;
    }
    case 2018:
    {
        uint32 value = r_u16(payload);
        w_u16(frame + 0x10, value);
        value = r_u16(payload + 2);
        w_u16(frame + 0x12, value);
        value = r_u16(payload + 4);
        w_u16(frame + 0x16, 0);
        w_u16(frame + 0x14, value);
        G3Function800777C0(controlled, game, frame + 0x10, payload, frame);
        return 3;
    }
    case 2019:
        G3Function80077648(payload);
        return 3;
    case 2024:
        G3Function800714E0(object, payload, message, payload, frame);
        return 3;
    case 2029:
        G3Function800788CC(object, payload, message, payload, frame);
        return 3;
    default:
        return 0;
    }
}

uint32 G3Function8003ACFC(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003acfc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    switch (message)
    {
    case 1:
    case 2:
    case 3:
    case 8:
    {
        if (message != 1)
        {
            uint32 flags = r_u16(object + 0x116);
            if ((flags & 0x100) != 0)
            {
                w_u32(frame + 0x10, 0);
                uint32 target = r_u32(object + 0x12c);
                if (G3DispatchObjectEvent(target, object, game, message, payload, 0, frame) == 0)
                    return 1;
            }
        }
        if (message == 1)
            G3Function8003B120(object, game, frame);
        else if (message == 2)
            G3Function8003B48C(object, game, frame);
        else if (message == 3)
            G3Function8003BA38(object, game, frame);
        else
            G3Function8003B8A8(object, game, payload, frame);
        uint32 flags = r_u16(object + 0x116);
        if ((flags & 0x100) != 0)
        {
            w_u32(frame + 0x10, 1);
            uint32 target = r_u32(object + 0x12c);
            G3DispatchObjectEvent(target, object, game, message, payload, 1, frame);
        }
        return 1;
    }
    case 5:
        G3Function8003CD64(payload, 8);
        return 1;
    case 9:
        G3Function8003C4F8(object, game, payload, frame);
        return 1;
    case 2010:
        G3Function8003B9A4(object, frame);
        return 1;
    case 2013:
    {
        uint32 resource = r_u32(object + 0x1c);
        uint32 flags = r_u32(resource + 0x3c);
        if ((flags & 1) == 0)
            return 0;
        uint32 selector = (uint32)r_s8(object + 0x115);
        G3Function80082648(object, selector, frame);
        return 1;
    }
    case 2015:
        G3Function8003C520(object, payload, frame);
        return 1;
    case 2018:
        G3Function8003CCE8(object, game, payload, frame);
        return 1;
    case 2020:
        G3Function800193F8(0x01000000, object, frame);
        return 1;
    default:
        return 0;
    }
}

uint32 G3Function8003B9A4(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003b9a4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function800193F8(0x8000, object, frame);
    uint32 resource = r_u32(object + 0x1c);
    uint32 flags = r_u32(resource);
    if ((flags & 0x4000) == 0)
        return 0;
    flags = r_u32(object + 0x10);
    if ((flags & 0x100000) != 0)
        return 0x10000;
    uint32 properties = r_u32(object + 0x14);
    if ((properties & 0x10000) == 0)
        w_u32(object + 0x14, properties | 0x1000);
    flags = r_u32(object + 0x10);
    if ((flags & 0x100000) != 0)
        return 0x02000000;
    uint32 result = flags | 0x02000000;
    w_u32(object + 0x10, result);
    return result;
}

uint32 G3Function8003C520(uint32 object, uint32 value, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003c520, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 selector = (uint32)(sint16)(value & 0xffff);
    return G3Function8008321C(object, selector, object + 0x104, frame);
}

uint32 G3Function8008345C(uint32 object, uint32 linked_output, uint32 model_output)
{
    FUNCTION_MARKER(0x8008345c, "SLUS_008.06");
    if (linked_output != 0)
        w_u32(linked_output, 0);
    if (model_output != 0)
        w_u32(model_output, 0);
    if (object != 0)
    {
        uint32 intro = r_u32(object + 0x20);
        if (intro != 0)
        {
            uint32 data = r_u32(intro + 0x24);
            if (data != 0)
                return data;
        }
    }
    uint32 linked = r_u32(object + 0x2c);
    if (linked != 0)
    {
        uint32 flags = r_u32(object + 0x10);
        if ((flags & 0x100000) == 0)
        {
            uint32 intro = r_u32(linked + 0x20);
            if (intro != 0)
            {
                uint32 data = r_u32(intro + 0x24);
                if (data != 0)
                {
                    if (linked_output != 0)
                        w_u32(linked_output, 1);
                    return data;
                }
            }
        }
    }
    uint32 resource = r_u32(object + 0x1c);
    uint32 models = r_u32(resource + 0xc);
    uint32 model = r_u32(models);
    if (model == 0)
        return 0;
    uint32 data = r_u32(model + 0x30);
    if (data != 0 && model_output != 0)
        w_u32(model_output, 1);
    return data;
}

uint32 G3Function8003CCE8(uint32 object, uint32 game, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003cce8, "SLUS_008.06");
    uint32 result = G3Function8008345C(object, 0, 0);
    if (result != 0)
        return result;
    uint32 value = r_u16(object + 0x58);
    uint32 change = r_u16(payload);
    w_u16(object + 0x58, value + change);
    value = r_u16(object + 0x5a);
    change = r_u16(payload + 2);
    w_u16(object + 0x5a, value + change);
    value = r_u16(object + 0x5c);
    change = r_u16(payload + 4);
    result = value + change;
    w_u16(object + 0x5c, result);
    return result;
}

uint32 G3Function8003CD64(uint32 payload, uint32 incoming_v0)
{
    FUNCTION_MARKER(0x8003cd64, "SLUS_008.06");
    if (payload == 0)
        return incoming_v0;
    uint32 resource = r_u32(payload);
    if (resource == 0)
        return 0;
    uint32 flags = r_u32(resource);
    uint32 result = flags & 0x800000;
    if (result != 0)
        return result;
    flags = r_u32(payload + 0x18);
    result = flags & 0xfffff7ff;
    w_u32(payload + 0x18, result);
    return result;
}

uint32 G3Function80083578(uint32 object)
{
    FUNCTION_MARKER(0x80083578, "SLUS_008.06");
    uint32 data = G3Function8008345C(object, 0, 0);
    if (data == 0)
        return 0;
    uint32 channel = r_u32(data);
    uint32 stride = 0x20;
    if (channel == 0)
    {
        channel = r_u32(data + 4);
        stride = 10;
        if (channel == 0)
        {
            channel = r_u32(data + 8);
            stride = 0x20;
            if (channel == 0)
                return 0;
        }
    }
    sint32 count = r_s16(channel + 4);
    uint32 cursor = r_u32(channel);
    uint32 sum = 0;
    uint32 index = 0;
    if (count > 0)
    {
        do
        {
            ++index;
            uint32 length = r_u16(cursor);
            cursor += stride;
            sum += length;
        } while ((sint16)(index & 0xffff) < count);
    }
    return (uint32)(sint32)(sint16)(sum & 0xffff);
}

uint32 G3Function8003B120(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003b120, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 resource = r_u32(object + 0x1c);
    sint32 model_index = r_s16(object + 0xa2);
    uint32 models = r_u32(resource + 0xc);
    uint32 model = r_u32(models + ((uint32)model_index << 2));
    uint32 data = r_u32(model + 0x1c);
    w_u8(object + 0x114, 1);
    w_u8(object + 0x115, 1);
    w_u16(object + 0x116, 1);
    w_u32(object + 0x128, 0xffffffff);
    w_u32(object + 0x12c, 0);
    if (data != 0)
    {
        uint32 value = r_u8(data + 0xc);
        w_u8(object + 0x11e, value);
        value = r_u8(data + 8);
        w_u8(object + 0x11d, value - 1);
    }
    uint32 commands = r_u32(object + 0x24);
    if (commands != 0)
        G3Function8003BA9C(object, commands, frame);
    commands = r_u32(object + 0x28);
    if (commands != 0)
        G3Function8003BA9C(object, commands, frame);
    uint32 intro = r_u32(object + 0x20);
    uint32 intro_flags = 0;
    if (intro != 0)
        intro_flags = r_u32(intro + 0x18);
    if (intro != 0 && (intro_flags & 0x800) != 0)
    {
        uint32 value = G3Function80083578(object);
        uint32 selector = (uint32)(sint16)(value & 0xffff);
        G3Function8008321C(object, selector, object + 0x104, frame);
        uint32 flags = r_u32(object + 0x10);
        w_u32(object + 0x10, (flags ^ 0x01000000) | 0x100000);
    }
    else
    {
        uint32 current_game = r_u32(0x80097048);
        G3Function80082988(object, object + 0x104, current_game, frame);
    }
    resource = r_u32(object + 0x1c);
    uint32 resource_flags = r_u32(resource + 0x30);
    if ((resource_flags & 0x200000) != 0)
    {
        uint32 flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags | 8);
    }
    uint32 flags = r_u16(object + 0x116);
    if ((flags & 0x80) != 0)
        return G3Function8003CDA8(object, game, 0x80, frame);
    return 0;
}

uint32 G3Function8008354C(uint32 object, uint32 linked_source, uint32 model_source)
{
    FUNCTION_MARKER(0x8008354c, "SLUS_008.06");
    if (linked_source != 0 || model_source != 0)
        return 0;
    uint32 intro = r_u32(object + 0x20);
    uint32 data = r_u32(intro + 0x24);
    return data + 0x10;
}

uint32 G3Function8008321C(uint32 object, uint32 selector, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8008321c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    uint32 result = G3Function8008345C(object, frame + 0x20, frame + 0x24);
    if (result == 0)
        return 0;
    uint32 data = result;
    if (state == 0)
    {
        uint32 linked_source = r_u32(frame + 0x20);
        uint32 model_source = r_u32(frame + 0x24);
        result = G3Function8008354C(object, linked_source, model_source);
        state = result;
        if (state == 0)
            return result;
    }
    uint32 rotation_state = state + 4;
    uint32 scale_state = state + 8;
    uint32 key = selector;
    result = (uint32)(sint16)(selector & 0xffff);
    if ((sint32)result == -1)
    {
        result = G3Function80083578(object);
        key = result;
    }
    uint32 position_channel = r_u32(data);
    uint32 rotation_channel = r_u32(data + 4);
    uint32 scale_channel = r_u32(data + 8);
    if (state != 0 && position_channel != 0)
    {
        G3Function8003F93C(position_channel, state, key & 0xffff, frame);
        G3Function80041008(position_channel, state, frame + 0x10, frame);
        result = r_u32(frame + 0x24);
        if (result != 0)
            result = G3Function80083038(object, frame + 0x10, result, frame);
        else
        {
            result = r_u16(frame + 0x10);
            w_u16(object + 0x58, result);
            result = r_u16(frame + 0x12);
            w_u16(object + 0x5a, result);
            result = r_u16(frame + 0x14);
            w_u16(object + 0x5c, result);
        }
    }
    if (rotation_state != 0 && rotation_channel != 0)
    {
        G3Function8003F93C(rotation_channel, rotation_state, key & 0xffff, frame);
        G3Function80041008(rotation_channel, rotation_state, object + 0x60, frame);
        result = r_u32(frame + 0x24);
        if (result != 0)
            result = G3Function800828D4(object, result, frame);
    }
    if (scale_state != 0 && scale_channel != 0)
    {
        G3Function8003F93C(scale_channel, scale_state, key & 0xffff, frame);
        G3Function80041008(scale_channel, scale_state, frame + 0x18, frame);
        result = r_u16(frame + 0x18);
        w_u16(object + 0x68, result);
        result = r_u16(frame + 0x1c);
        w_u16(object + 0x6a, result);
        result = r_u16(frame + 0x1a);
        w_u16(object + 0x6c, result);
    }
    return result;
}

uint32 G3Function8003CDA8(uint32 object, uint32 game, uint32 incoming_v0, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003cda8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 state = r_u32(object + 0x12c);
    if (state == 0)
        return incoming_v0;
    uint32 value = r_u32(state);
    if (value == 0)
        value = 256;
    uint32 next = r_u32(state + 4);
    w_u32(state, value);
    if (next == 0)
        next = 40;
    w_u32(state + 4, next);
    value = (uint32)r_s16(object + 0x5c);
    w_u32(state + 0xc, value);
    uint32 intro = r_u32(object + 0x20);
    value = 0;
    if (intro != 0)
        value = (uint32)r_s16(intro + 0xc);
    w_u32(state + 0x34, value);
    value = r_u16(object + 0x58);
    w_u16(frame + 0x18, value);
    w_u16(frame + 0x10, value);
    value = r_u16(object + 0x5a);
    w_u16(frame + 0x1a, value);
    w_u16(frame + 0x12, value);
    value = r_u16(object + 0x5c);
    w_u16(frame + 0x14, value);
    value = r_u16(object + 0x5c);
    w_u16(frame + 0x1c, value + 128);
    uint32 level = r_u32(game + 0x48);
    uint32 terrain = r_u32(level);
    return G3Function800622EC(terrain, frame + 0x10, frame + 0x18, object, frame);
}

uint32 G3Function8003F93C(uint32 channel, uint32 state, uint32 key, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003f93c, "SLUS_008.06");
    uint32 flags = r_u16(state + 2);
    w_u16(state, 0);
    w_u16(state + 2, flags & 1);
    if (channel == 0 || state == 0)
        return 0xffffffff;
    uint32 short_cursor = r_u32(channel);
    uint32 format = r_u8(channel + 6);
    uint32 long_cursor = short_cursor;
    uint32 total = 0;
    if ((key & 0xffff) != 0)
    {
        for (;;)
        {
            sint32 index = r_s16(state);
            sint32 count = r_s16(channel + 4);
            if (index >= count)
                break;
            uint32 difference = key - total;
            sint32 remaining = (sint16)(difference & 0xffff);
            uint32 cursor = format == 1 ? short_cursor : long_cursor;
            sint32 length = r_s16(cursor);
            if (remaining < length)
            {
                flags = r_u16(state + 2);
                w_u16(state + 2, (flags & 1) | (difference << 1));
                break;
            }
            length = r_s16(cursor);
            total = (total & 0xffff) + (uint32)length;
            index = r_s16(state);
            count = r_s16(channel + 4);
            sint32 numerator = index + 1;
            sint32 remainder = count == 0 ? numerator : numerator % count;
            w_u16(state, (uint32)remainder);
            long_cursor += 0x20;
            if ((remainder & 0xffff) != 0)
                short_cursor += 10;
            else
            {
                short_cursor = r_u32(channel);
                long_cursor = short_cursor;
            }
            if ((total & 0xffff) >= (key & 0xffff))
                break;
        }
    }
    flags = r_u16(state + 2);
    w_u16(state + 2, flags & 0xfffe);
    return 0;
}

uint32 G3Function80041008(uint32 channel, uint32 state, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80041008, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    if (channel == 0 || state == 0)
        return 0;
    uint32 format = r_u8(channel + 6);
    sint32 index = r_s16(state);
    sint32 count = r_s16(channel + 4);
    if (index >= count || index < 0)
        return 0;
    sint32 duration = 0x8000;
    uint32 flags = r_u16(state + 2);
    if ((flags & 1) == 0)
    {
        uint32 base = r_u32(channel);
        uint32 offset = (uint32)index * (format == 1 ? 10 : 32);
        duration = r_s16(base + offset);
    }
    uint32 fraction = 0;
    if (duration != 0)
        fraction = r_u16(state + 2);
    if (duration != 0 && (fraction & 0xfffe) != 0)
    {
        if (format == 1)
        {
            sint32 numerator = (sint32)((fraction >> 1) << 12);
            uint32 alpha = (uint32)(numerator / duration);
            index = r_s16(state);
            uint32 base = r_u32(channel);
            w_u32(frame + 0x10, 0);
            uint32 offset = (uint32)index * 10;
            G3Function80020714(alpha, base + offset + 2, base + offset + 12, frame + 0x18, frame);
        }
        else
        {
            sint32 numerator = (sint32)((fraction >> 1) << 15);
            uint32 alpha = (uint32)(numerator / duration);
            index = r_s16(state);
            uint32 base = r_u32(channel);
            uint32 current = base + ((uint32)index << 5);
            G3Function8003F290(output, alpha, current, current + 0x20, frame);
        }
    }
    else if (format == 1)
    {
        index = r_s16(state);
        uint32 base = r_u32(channel);
        uint32 current = base + (uint32)index * 10;
        uint32 words[2];
        for (uint32 i = 0; i < 2; ++i)
        {
            uint32 address = current + 2 + i * 4;
            uint32 high_address = address + 3;
            uint32 high_lane = high_address & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low_lane = address & 3;
            uint32 low = r_masked_u32(address & 0xfffffffc, (15 << low_lane) & 15);
            words[i] = low_lane == 0 ? low : (high << ((4 - low_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 i = 0; i < 2; ++i)
        {
            uint32 address = frame + 0x18 + i * 4;
            uint32 high_address = address + 3;
            uint32 high_lane = high_address & 3;
            w_masked_u32(high_address & 0xfffffffc, words[i] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            uint32 low_lane = address & 3;
            w_masked_u32(address & 0xfffffffc, words[i] << (low_lane * 8), (15 << low_lane) & 15);
        }
    }
    else
    {
        index = r_s16(state);
        uint32 base = r_u32(channel);
        uint32 value = r_u16(base + ((uint32)index << 5) + 2);
        w_u16(output, value);
        index = r_s16(state);
        base = r_u32(channel);
        value = r_u16(base + ((uint32)index << 5) + 4);
        w_u16(output + 2, value);
        index = r_s16(state);
        base = r_u32(channel);
        value = r_u16(base + ((uint32)index << 5) + 6);
        w_u16(output + 4, value);
    }
    if (format == 1)
    {
        G3Function80020258(frame + 0x18, frame + 0x20, 0x15, frame);
        uint32 words[2];
        for (uint32 i = 0; i < 2; ++i)
        {
            uint32 address = frame + 0x20 + i * 4;
            uint32 high_address = address + 3;
            uint32 high_lane = high_address & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low_lane = address & 3;
            uint32 low = r_masked_u32(address & 0xfffffffc, (15 << low_lane) & 15);
            words[i] = low_lane == 0 ? low : (high << ((4 - low_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 i = 0; i < 2; ++i)
        {
            uint32 address = output + i * 4;
            uint32 high_address = address + 3;
            uint32 high_lane = high_address & 3;
            w_masked_u32(high_address & 0xfffffffc, words[i] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            uint32 low_lane = address & 3;
            w_masked_u32(address & 0xfffffffc, words[i] << (low_lane * 8), (15 << low_lane) & 15);
        }
    }
    return 1;
}

uint32 G3Function8003F290(uint32 output, uint32 alpha, uint32 first, uint32 second, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003f290, "SLUS_008.06");
    if (alpha == 0)
    {
        uint32 value = r_u16(first + 2);
        w_u16(output, value);
        value = r_u16(first + 4);
        w_u16(output + 2, value);
        value = r_u16(first + 6);
        w_u16(output + 4, value);
        return value;
    }
    uint32 square = (uint32)((sint32)(alpha * alpha) >> 15);
    uint32 cube = (uint32)((sint32)(square * alpha) >> 15);
    uint32 first_weight = (cube << 1) - ((square << 1) + square) + 0x8000;
    uint32 second_weight = (square << 1) - (cube << 1) + square;
    uint32 first_tangent_weight = cube - (square << 1) + alpha;
    uint32 second_tangent_weight = cube - square;
    uint32 result = 0;
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 first_position = (uint32)r_s16(first + 2 + axis * 2);
        uint32 second_position = (uint32)r_s16(second + 2 + axis * 2);
        uint32 first_tangent = r_u32(first + 8 + axis * 4);
        uint32 second_tangent = r_u32(second + 0x14 + axis * 4);
        uint32 sum = first_position * first_weight + second_position * second_weight;
        sum += first_tangent * first_tangent_weight;
        sum += second_tangent * second_tangent_weight;
        result = (uint32)((sint32)sum >> 15);
        w_u16(output + axis * 2, result);
    }
    return result;
}

uint32 G3Function80020258(uint32 input, uint32 output, uint32 order, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80020258, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    G3Function80020608(frame + 0x10, input, frame);
    return G3Function800202A4(frame + 0x10, output, order, frame);
}

uint32 G3Function8003C864(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003c864, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 resource = r_u32(object + 0x1c);
    uint32 flags = r_u32(resource);
    if ((flags & 0x2000) == 0)
        return 0;
    uint32 controlled = r_u32(0x8009efc4);
    uint32 target = r_u32(controlled + 0xec);
    if (target == 0)
        return 0;
    return G3DispatchStateEvent(target, controlled, game, 1003, object, frame);
}

uint32 G3Function8003BA38(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003ba38, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 flags = r_u16(object + 0x116);
    if ((flags & 0x80) != 0)
        return G3Function8003D270(object, game, frame);
    G3Function8003C6E0(object, game, frame);
    return G3Function8003C864(object, game, frame);
}

uint32 G3Function80020608(uint32 matrix, uint32 quaternion, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80020608, "SLUS_008.06");
    sint32 w = r_s16(quaternion + 6);
    sint32 x = r_s16(quaternion);
    sint32 y = r_s16(quaternion + 2);
    sint32 z = r_s16(quaternion + 4);
    sint32 wx = (w * x) >> 11;
    sint32 wy = (w * y) >> 11;
    sint32 wz = (w * z) >> 11;
    sint32 xx = (x * x) >> 11;
    sint32 xy = (x * y) >> 11;
    sint32 yy = (y * y) >> 11;
    sint32 zz = (z * z) >> 11;
    sint32 xz = (x * z) >> 11;
    sint32 yz = (y * z) >> 11;
    w_u16(matrix, (uint32)(4096 - (yy + zz)));
    w_u16(matrix + 2, (uint32)(xy - wz));
    w_u16(matrix + 6, (uint32)(xy + wz));
    w_u16(matrix + 8, (uint32)(4096 - (xx + zz)));
    w_u16(matrix + 4, (uint32)(xz + wy));
    w_u16(matrix + 0xc, (uint32)(xz - wy));
    w_u16(matrix + 0x10, (uint32)(4096 - (xx + yy)));
    uint32 result = (uint32)(yz - wx);
    w_u16(matrix + 0xa, result);
    w_u16(matrix + 0xe, (uint32)(yz + wx));
    return result;
}

uint32 G3Function800202A4(uint32 matrix, uint32 output, uint32 order, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800202a4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    uint32 repeated_axis = (order >> 1) & 1;
    w_u32(frame + 0x14, (order >> 2) & 1);
    w_u32(frame + 0x50, order);
    uint32 i = r_u8(0x8009d618 + ((order >> 3) & 3));
    w_u32(frame + 0x18, order & 1);
    uint32 parity = r_u32(frame + 0x14);
    uint32 j = r_u8(0x8009d620 + i + parity);
    uint32 k = r_u8(0x8009d620 + i - (parity - 1));
    w_u32(frame + 0x10, k);
    uint32 result;
    uint32 i2 = i << 1;
    uint32 j2 = j << 1;
    if (repeated_axis == 1)
    {
        uint32 i6 = (i2 + i) << 1;
        uint32 first_address = matrix + j2 + i6;
        w_u32(frame + 0x1c, first_address);
        sint32 first = r_s16(first_address);
        k = r_u32(frame + 0x10);
        uint32 k2 = k << 1;
        uint32 second_address = matrix + k2 + i6;
        sint32 second = r_s16(second_address);
        uint32 squares = (uint32)(first * first) + (uint32)(second * second);
        uint32 length = G3Function8002F368((uint32)((sint32)squares >> 12), frame);
        if ((sint32)length >= 17)
        {
            first_address = r_u32(frame + 0x1c);
            uint32 arg1 = (uint32)r_s16(second_address);
            uint32 arg0 = (uint32)r_s16(first_address);
            result = G3Function8002F240(arg0, arg1, frame);
            w_u16(output, result);
            arg1 = (uint32)r_s16(matrix + i2 + i6);
            result = G3Function8002F240(length, arg1, frame);
            w_u16(output + 2, result);
            k = r_u32(frame + 0x10);
            arg1 = (uint32)r_s16(matrix + i2 + ((k2 + k) << 1));
            arg0 = (uint32)r_s16(matrix + i2 + ((j2 + j) << 1));
            result = G3Function8002F240(arg0, 0 - arg1, frame);
            w_u16(output + 4, result);
        }
        else
        {
            uint32 j6 = (j2 + j) << 1;
            uint32 arg0 = (uint32)r_s16(matrix + k2 + j6);
            uint32 arg1 = (uint32)r_s16(matrix + j2 + j6);
            result = G3Function8002F240(0 - arg0, arg1, frame);
            w_u16(output, result);
            arg1 = (uint32)r_s16(matrix + i2 + i6);
            result = G3Function8002F240(length, arg1, frame);
            w_u16(output + 2, result);
            w_u16(output + 4, 0);
        }
    }
    else
    {
        uint32 diagonal_address = matrix + i2 + ((i2 + i) << 1);
        sint32 first = r_s16(diagonal_address);
        uint32 j6 = (j2 + j) << 1;
        uint32 second_address = matrix + i2 + j6;
        sint32 second = r_s16(second_address);
        uint32 squares = (uint32)(first * first) + (uint32)(second * second);
        uint32 length = G3Function8002F368((uint32)((sint32)squares >> 12), frame);
        if ((sint32)length >= 17)
        {
            k = r_u32(frame + 0x10);
            uint32 k2 = k << 1;
            uint32 k6 = (k2 + k) << 1;
            uint32 next_address = matrix + i2 + k6;
            uint32 arg0 = (uint32)r_s16(matrix + j2 + k6);
            uint32 arg1 = (uint32)r_s16(matrix + k2 + k6);
            result = G3Function8002F240(arg0, arg1, frame);
            w_u16(output, result);
            arg0 = (uint32)r_s16(next_address);
            result = G3Function8002F240(0 - arg0, length, frame);
            w_u16(output + 2, result);
            arg0 = (uint32)r_s16(second_address);
            arg1 = (uint32)r_s16(diagonal_address);
            result = G3Function8002F240(arg0, arg1, frame);
            w_u16(output + 4, result);
        }
        else
        {
            k = r_u32(frame + 0x10);
            uint32 k2 = k << 1;
            uint32 arg0 = (uint32)r_s16(matrix + k2 + j6);
            uint32 arg1 = (uint32)r_s16(matrix + j2 + j6);
            result = G3Function8002F240(0 - arg0, arg1, frame);
            w_u16(output, result);
            k = r_u32(frame + 0x10);
            arg0 = (uint32)r_s16(matrix + i2 + ((k2 + k) << 1));
            result = G3Function8002F240(0 - arg0, length, frame);
            w_u16(output + 2, result);
            w_u16(output + 4, 0);
        }
    }
    parity = r_u32(frame + 0x14);
    if (parity == 1)
    {
        uint32 x = r_u16(output);
        uint32 z = r_u16(output + 4);
        w_u16(output, 0 - x);
        uint32 y = r_u16(output + 2);
        w_u16(output + 4, 0 - z);
        result = 0 - y;
        w_u16(output + 2, result);
    }
    uint32 swap_axes = r_u32(frame + 0x18);
    if (swap_axes == 1)
    {
        result = r_u16(output + 4);
        uint32 x = r_u16(output);
        w_u16(output, result);
        w_u16(output + 4, x);
    }
    uint32 saved_order = r_u16(frame + 0x50);
    w_u16(output + 6, saved_order);
    return result;
}

uint32 G3Function80020714(uint32 alpha, uint32 first, uint32 second, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80020714, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 dot = 0;
    for (uint32 component = 0; component < 4; ++component)
    {
        sint32 a = r_s16(first + component * 2);
        sint32 b = r_s16(second + component * 2);
        dot += (uint32)(a * b);
    }
    uint32 cosine = (uint32)((sint32)dot >> 12);
    bool negate_second = (sint16)(cosine & 0xffff) < 0;
    if (negate_second)
        cosine = 0 - cosine;
    cosine = (uint32)(sint16)(cosine & 0xffff);
    uint32 first_weight = 4096 - alpha;
    uint32 second_weight = alpha;
    if (4096 - (sint32)cosine > 0)
    {
        uint32 angle_word = G3Function8002F290(cosine, frame);
        uint32 angle = (uint32)(sint16)(angle_word & 0xffff);
        uint32 turns = r_u32(frame + 0x40);
        uint32 phase_word = angle_word + (turns << 12);
        uint32 sine = G3Function8005B590(angle);
        uint32 phase = (uint32)(sint16)(phase_word & 0xffff);
        uint32 interpolation_angle = (uint32)((sint32)(alpha * phase) >> 12);
        uint32 first_sine = G3Function8005B590(angle - interpolation_angle);
        sint32 numerator = (sint32)(first_sine << 12);
        sint32 denominator = (sint16)(sine & 0xffff);
        if (denominator == 0)
            first_weight = numerator < 0 ? 1 : 0xffffffff;
        else if (numerator == (-2147483647 - 1) && denominator == -1)
            first_weight = 0x80000000;
        else
            first_weight = (uint32)(numerator / denominator);
        uint32 second_sine = G3Function8005B590(interpolation_angle);
        numerator = (sint32)(second_sine << 12);
        if (denominator == 0)
            second_weight = numerator < 0 ? 1 : 0xffffffff;
        else if (numerator == (-2147483647 - 1) && denominator == -1)
            second_weight = 0x80000000;
        else
            second_weight = (uint32)(numerator / denominator);
    }
    if (negate_second)
        second_weight = 0 - second_weight;
    for (uint32 component = 0; component < 3; ++component)
    {
        uint32 a = (uint32)r_s16(first + component * 2);
        uint32 b = (uint32)r_s16(second + component * 2);
        uint32 first_term = (uint32)((sint32)(first_weight * a) >> 12);
        uint32 second_term = (uint32)((sint32)(second_weight * b) >> 12);
        w_u16(output + component * 2, first_term + second_term);
    }
    uint32 a = (uint32)r_s16(first + 6);
    uint32 b = (uint32)r_s16(second + 6);
    uint32 first_term = (uint32)((sint32)(first_weight * a) >> 12);
    uint32 second_term = (uint32)((sint32)(second_weight * b) >> 12);
    uint32 w_word = first_term + second_term;
    sint32 w = (sint16)(w_word & 0xffff);
    uint32 squares = (uint32)(w * w);
    sint32 x = r_s16(output);
    squares += (uint32)(x * x);
    sint32 y = r_s16(output + 2);
    squares += (uint32)(y * y);
    sint32 z = r_s16(output + 4);
    uint32 z_square = (uint32)(z * z);
    w_u16(output + 6, w_word);
    squares += z_square;
    squares += 2048;
    uint32 length = G3Function8002F368((uint32)((sint32)squares >> 12), frame);
    uint32 quotients[4];
    const uint32 offsets[4] = {6, 0, 2, 4};
    for (uint32 component = 0; component < 4; ++component)
    {
        sint32 value = r_s16(output + offsets[component]);
        sint32 numerator = (sint32)((uint32)value << 12);
        if (length == 0)
            quotients[component] = numerator < 0 ? 1 : 0xffffffff;
        else
            quotients[component] = (uint32)(numerator / (sint32)length);
    }
    for (uint32 component = 0; component < 4; ++component)
        w_u16(output + offsets[component], quotients[component]);
    return length;
}

uint32 G3Function8002F290(uint32 cosine, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002f290, "SLUS_008.06");
    uint32 angle = 512;
    uint32 step = 256;
    bool negative = (sint16)(cosine & 0xffff) < 0;
    uint32 magnitude = negative ? 0 - cosine : cosine;
    sint32 target = (sint16)(magnitude & 0xffff);
    if (target != 0)
    {
        while ((step & 0xffff) != 0)
        {
            uint32 argument = (uint32)(sint16)(angle & 0xffff);
            uint32 word = G3Function8005B470(argument);
            sint32 value = (sint16)(word & 0xffff);
            if (value < target)
                angle -= step;
            else
                angle += step;
            step = (uint32)((sint32)(step << 16) >> 17);
            if (value == target)
                break;
        }
    }
    if (negative)
        angle = 2048 - angle;
    return (uint32)(sint32)(sint16)(angle & 0xffff);
}

uint32 G3Function800828D4(uint32 object, uint32 incoming_v0, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800828d4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x68;
    uint32 intro = r_u32(object + 0x20);
    if (intro == 0)
        return incoming_v0;
    uint32 rotation = r_u32(intro + 8);
    if (rotation == 0)
    {
        sint32 z = r_s16(intro + 0xc);
        if (z == 0)
            return 0;
    }
    G3Function8005BD00(object + 0x60, frame + 0x10);
    G3Function8005BD00(intro + 8, frame + 0x30);
    MulMatrix0((MATRIX *)psx_addr(frame + 0x30, 20), (MATRIX *)psx_addr(frame + 0x10, 20), (MATRIX *)psx_addr(frame + 0x10, 20));
    G3Function800202A4(frame + 0x10, frame + 0x50, 0x15, frame);
    uint32 result = r_u16(frame + 0x50);
    w_u16(object + 0x60, result);
    result = r_u16(frame + 0x52);
    w_u16(object + 0x62, result);
    result = r_u16(frame + 0x54);
    w_u16(object + 0x64, result);
    return result;
}

uint32 G3Function80082988(uint32 object, uint32 state, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80082988, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 result = G3Function8008345C(object, frame + 0x18, frame + 0x1c);
    if (result == 0)
        return 0;
    uint32 data = result;
    if (state == 0)
    {
        uint32 linked_source = r_u32(frame + 0x18);
        uint32 model_source = r_u32(frame + 0x1c);
        result = G3Function8008354C(object, linked_source, model_source);
        state = result;
        if (state == 0)
            return result;
    }
    uint32 position_channel = r_u32(data);
    uint32 rotation_channel = r_u32(data + 4);
    uint32 scale_channel = r_u32(data + 8);
    uint32 scale_state = state + 8;
    if (rotation_channel != 0)
    {
        uint32 quaternion = G3Function8003FB7C(rotation_channel, state + 4, frame);
        G3Function80020258(quaternion, frame + 0x10, 0, frame);
        result = r_u16(frame + 0x10);
        w_u16(object + 0x60, result);
        result = r_u16(frame + 0x12);
        w_u16(object + 0x62, result);
        result = r_u16(frame + 0x14);
        uint32 model_source = r_u32(frame + 0x1c);
        w_u16(object + 0x64, result);
        if (model_source != 0)
            result = G3Function800828D4(object, result, frame);
    }
    if (position_channel != 0)
    {
        result = G3Function8003FBB0(position_channel, state, frame);
        uint32 vector = result;
        if (vector != 0)
        {
            uint32 model_source = r_u32(frame + 0x1c);
            if (model_source != 0)
            {
                uint32 base = r_u16(object + 0x88);
                uint32 value = r_u16(vector);
                w_u16(object + 0x58, base + value);
                base = r_u16(object + 0x8a);
                value = r_u16(vector + 2);
                w_u16(object + 0x5a, base + value);
                base = r_u16(object + 0x8c);
                value = r_u16(vector + 4);
                result = base + value;
            }
            else
            {
                result = r_u16(vector);
                w_u16(object + 0x58, result);
                result = r_u16(vector + 2);
                w_u16(object + 0x5a, result);
                result = r_u16(vector + 4);
            }
            w_u16(object + 0x5c, result);
        }
    }
    if (scale_channel != 0)
    {
        result = G3Function8003FBB0(scale_channel, scale_state, frame);
        uint32 vector = result;
        if (vector != 0)
        {
            result = r_u16(vector);
            w_u16(object + 0x68, result);
            result = r_u16(vector + 2);
            w_u16(object + 0x6a, result);
            result = r_u16(vector + 4);
            w_u16(object + 0x6c, result);
        }
    }
    return result;
}

uint32 G3Function80083038(uint32 object, uint32 vector, uint32 incoming_v0, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80083038, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    uint32 intro = r_u32(object + 0x20);
    bool rotated = false;
    if (intro != 0)
    {
        uint32 xy_rotation = r_u32(intro + 8);
        if (xy_rotation != 0)
            rotated = true;
        else
            rotated = r_s16(intro + 0xc) != 0;
    }
    if (!rotated)
    {
        uint32 base = r_u16(object + 0x88);
        uint32 value = r_u16(vector);
        w_u16(object + 0x58, base + value);
        base = r_u16(object + 0x8a);
        value = r_u16(vector + 2);
        w_u16(object + 0x5a, base + value);
        base = r_u16(object + 0x8c);
        value = r_u16(vector + 4);
        uint32 result = base + value;
        w_u16(object + 0x5c, result);
        return result;
    }
    uint32 x_word = r_u16(vector);
    w_u16(frame + 0x10, x_word);
    uint32 y_word = r_u16(vector + 2);
    w_u16(frame + 0x12, y_word);
    uint32 z_word = r_u16(vector + 4);
    w_u16(frame + 0x40, x_word);
    w_u16(frame + 0x42, y_word);
    w_u16(frame + 0x14, z_word);
    w_u16(frame + 0x44, z_word);
    intro = r_u32(object + 0x20);
    G3Function8005BD00(intro + 8, frame + 0x18);
    sint32 x = r_s16(frame + 0x40);
    sint32 m00 = r_s16(frame + 0x18);
    sint32 y = r_s16(frame + 0x42);
    sint32 m01 = r_s16(frame + 0x1a);
    sint32 z = r_s16(frame + 0x44);
    sint32 m02 = r_s16(frame + 0x1c);
    sint32 m10 = r_s16(frame + 0x1e);
    sint32 m11 = r_s16(frame + 0x20);
    sint32 m12 = r_s16(frame + 0x22);
    sint32 m20 = r_s16(frame + 0x24);
    sint32 m21 = r_s16(frame + 0x26);
    w_u32(frame + 0x34, 0);
    w_u32(frame + 0x30, 0);
    w_u32(frame + 0x2c, 0);
    sint32 m22 = r_s16(frame + 0x28);
    sint32 transformed_x = ((x * m00) >> 12) + ((y * m01) >> 12) + ((z * m02) >> 12);
    sint32 transformed_y = ((x * m10) >> 12) + ((y * m11) >> 12) + ((z * m12) >> 12);
    sint32 transformed_z = ((x * m20) >> 12) + ((y * m21) >> 12) + ((z * m22) >> 12);
    w_u16(frame + 0x38, (uint32)transformed_x);
    w_u16(frame + 0x3a, (uint32)transformed_y);
    w_u16(frame + 0x3c, (uint32)transformed_z);
    uint32 base_x = r_u16(object + 0x88);
    uint32 base_y = r_u16(object + 0x8a);
    w_u16(object + 0x58, (uint32)transformed_x + base_x);
    uint32 result = r_u16(frame + 0x3a);
    w_u16(object + 0x5a, result + base_y);
    result = r_u16(frame + 0x3c);
    uint32 base_z = r_u16(object + 0x8c);
    result += base_z;
    w_u16(object + 0x5c, result);
    return result;
}
