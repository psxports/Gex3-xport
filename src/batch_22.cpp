#include "xport.h"
#include "psx.h"
#include "game_functions.h"

uint32 G3Function8004502C(uint32 effect, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004502c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 remaining = r_u16(effect + 0x48);
    uint32 object = r_u32(effect + 0x5c);
    --remaining;
    w_u16(effect + 0x48, remaining);
    if ((sint16)remaining <= 0)
        return G3Function80041578(effect, pool, frame);
    uint32 target = r_u32(object + 0xec);
    if (target != 0)
        return G3DispatchTimedObjectCallback(target, object, 0, 1000, effect, frame);
    return target;
}

uint32 G3Function800453A0(uint32 position, uint32 velocity, uint32 data, uint32 angle, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800453a0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 parameters = r_u32(0x80098784);
    w_u32(frame + 0x14, 2);
    w_u32(frame + 0x10, parameters);
    return G3Function80045288(position, velocity, data, angle, frame);
}

uint32 G3Function800453D0(uint32 position, uint32 velocity, uint32 data, uint32 angle, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800453d0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 parameters = r_u32(0x80098784);
    w_u32(frame + 0x14, 3);
    w_u32(frame + 0x10, parameters);
    uint32 effect = G3Function80045288(position, velocity, data, angle, frame);
    if (effect == 0)
        return 0;
    uint32 flags = r_u32(effect + 0xc);
    w_u16(effect + 0x26, 0);
    w_u16(effect + 0x36, 0);
    w_u32(effect + 0xc, flags | 0x2000);
    uint32 random = G3SdkRandom80027FA8();
    w_u16(effect + 0x48, (random & 0x1f) + 0x20);
    w_u16(effect + 0x4a, 0x2000);
    w_u32(effect + 0x10, 0x80044f14);
    return 0x80044f14;
}

uint32 G3Function80045448(uint32 position, uint32 velocity, uint32 data, uint32 factor, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80045448, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 lifetime = r_u32(guest_stack + 0x10);
    uint32 size = r_u32(guest_stack + 0x14);
    uint32 parameters = r_u32(0x80098784);
    w_u32(frame + 0x14, 3);
    w_u32(frame + 0x10, parameters);
    uint32 effect = G3Function80045288(position, velocity, data, 0, frame);
    if (effect == 0)
        return 0;
    uint32 game = r_u32(0x80097048);
    uint32 storage = r_u32(effect + 0x14);
    uint32 transform = r_u32(game + 0x4c);
    G3Function80041CE4(storage + 0xc, transform, effect + 0x1c, 0, frame);
    uint32 flags = r_u32(effect + 0xc);
    w_u16(effect + 0x26, size);
    w_u16(effect + 0x36, size);
    w_u16(effect + 0x48, lifetime);
    w_u32(effect + 0x10, 0x80044f7c);
    w_u16(effect + 0x58, factor);
    w_u32(effect + 0xc, flags | 0x2000);
    return 0x80044f7c;
}

uint32 G3Function800454F4(uint32 position, uint32 velocity, uint32 data, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800454f4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 lifetime = r_u32(guest_stack + 0x10);
    uint32 size = r_u32(guest_stack + 0x14);
    uint32 parameters = r_u32(0x80098784);
    w_u32(frame + 0x14, 3);
    w_u32(frame + 0x10, parameters);
    uint32 effect = G3Function80045288(position, velocity, data, 0, frame);
    if (effect == 0)
        return 0;
    uint32 game = r_u32(0x80097048);
    uint32 storage = r_u32(effect + 0x14);
    uint32 transform = r_u32(game + 0x4c);
    G3Function80041CE4(storage + 0xc, transform, effect + 0x1c, 0, frame);
    uint32 flags = r_u32(effect + 0xc);
    w_u16(effect + 0x26, size);
    w_u16(effect + 0x36, size);
    w_u16(effect + 0x48, lifetime);
    w_u32(effect + 0x10, 0x8004502c);
    w_u32(effect + 0x5c, object);
    w_u32(effect + 0xc, flags | 0x2000);
    return 0x8004502c;
}

void G3Function800455A0(uint32 position, sint32 count, uint32 radius, uint32 parameters, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800455a0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    uint32 selector = 0;
    if (parameters == 0)
    {
        parameters = r_u32(0x80098784);
        selector = 2;
    }
    if (parameters == 0 || count <= 0)
        return;
    sint32 signed_radius = (sint16)radius;
    sint32 diameter = signed_radius * 2;
    sint32 vertical_range = diameter / 3;
    sint32 vertical_base = (signed_radius * 5) / 3;
    for (sint32 index = 0; index < count; ++index)
    {
        uint32 random = G3SdkRandom80027FA8();
        uint32 remainder = diameter == 0 ? random : (uint32)((sint64)(sint32)random % diameter);
        w_u16(frame + 0x18, remainder - radius);
        random = G3SdkRandom80027FA8();
        remainder = diameter == 0 ? random : (uint32)((sint64)(sint32)random % diameter);
        w_u16(frame + 0x1a, remainder - radius);
        random = G3SdkRandom80027FA8();
        remainder = vertical_range == 0 ? random : (uint32)((sint64)(sint32)random % vertical_range);
        w_u16(frame + 0x1c, remainder + (uint32)vertical_base);
        uint32 angle = (uint32)r_s16(position + 4);
        w_u32(frame + 0x10, parameters);
        w_u32(frame + 0x14, selector);
        G3Function80045288(position, frame + 0x18, 0x8009e038, angle, frame);
    }
}

uint32 G3Function800456EC(uint32 effect, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800456ec, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function8004197C(effect, pool, frame);
    const uint32 offsets[12] = {0x24, 0x26, 0x28, 0x2c, 0x2e, 0x30, 0x34, 0x36, 0x38, 0x3c, 0x3e, 0x40};
    sint32 coordinate = r_s16(effect + offsets[0]);
    sint32 result = 0;
    for (uint32 index = 0; index < 12; ++index)
    {
        sint32 product = coordinate * 7;
        sint32 next = index + 1 < 12 ? r_s16(effect + offsets[index + 1]) : 0;
        result = product >> 3;
        w_u16(effect + offsets[index], (uint32)result);
        coordinate = next;
    }
    return (uint32)result;
}

void G3Function80045900(uint32 position, uint32 parameters, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80045900, "SLUS_008.06");
    G3Function80045804(position, parameters, guest_stack - 0x18);
}

uint32 G3Function800459D8(uint32 effect, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800459d8, "SLUS_008.06");
    uint32 paused = r_u32(0x8009e040);
    uint32 frame = guest_stack - 0x18;
    if (paused != 0)
        return paused;
    sint32 lifetime = r_s16(effect + 0x22);
    uint32 current = r_u16(effect + 0x22);
    if (lifetime > 0)
        w_u16(effect + 0x22, current - 1);
    sint32 result = r_s16(effect + 0x22);
    if (result == 0)
        return G3Function80041578(effect, pool, frame);
    return (uint32)result;
}

uint32 G3Function80043F7C(uint32 effect, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80043f7c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    sint32 lifetime = r_s16(effect + 0x22);
    uint32 current = r_u16(effect + 0x22);
    if (lifetime > 0)
    {
        w_u16(effect + 0x22, current - 1);
        lifetime = r_s16(effect + 0x22);
    }
    if (lifetime == 0)
        return G3Function80041578(effect, pool, frame);
    uint32 flags = r_u32(effect + 0xc);
    uint32 first_step = r_u16(effect + 0x44);
    uint32 second_size = r_u16(effect + 0x36);
    uint32 second_step = r_u16(effect + 0x48);
    w_u32(effect + 0xc, flags | 0x2000);
    uint32 first_size = r_u16(effect + 0x26);
    w_u16(effect + 0x36, second_size - second_step);
    w_u16(effect + 0x26, first_size - first_step);
    G3Function80044B60(effect, frame);
    if ((r_u32(effect + 0xc) & 4) != 0)
        G3Function80041880(effect, pool, frame);
    uint32 storage = r_u32(effect + 0x14);
    uint32 storage_flags = r_u32(storage + 8);
    uint32 result = storage_flags | 2;
    if ((storage_flags & 2) != 0)
        return result;
    w_u32(storage + 8, result);
    uint32 game = r_u32(0x80097048);
    storage = r_u32(effect + 0x14);
    uint32 transform = r_u32(game + 0x4c);
    return G3Function80041CE4(storage + 0xc, transform, effect + 0x1c, 0, frame);
}

uint32 G3Function80044068(uint32 effect, uint32 size, uint32 divisor, uint32 lifetime)
{
    FUNCTION_MARKER(0x80044068, "SLUS_008.06");
    uint32 flags = r_u32(effect + 0xc);
    w_u32(effect + 0xc, flags | 0x2000);
    w_u16(effect + 0x26, r_u16(size));
    uint32 second = r_u16(size + 4);
    sint32 denominator = (sint16)divisor;
    w_u16(effect + 0x36, second);
    sint32 first = r_s16(size);
    uint32 first_step = denominator == 0 ? (first < 0 ? 1 : uint32(0xffffffff)) : (uint32)(first / denominator);
    w_u16(effect + 0x44, first_step);
    sint32 second_value = r_s16(size + 4);
    uint32 second_step = denominator == 0 ? (second_value < 0 ? 1 : uint32(0xffffffff)) : (uint32)(second_value / denominator);
    w_u16(effect + 0x22, lifetime);
    w_u32(effect + 0x10, 0x80043f7c);
    w_u16(effect + 0x48, second_step);
    return 0x80043f7c;
}

uint32 G3Function80073604(uint32, uint32, uint32 flags)
{
    FUNCTION_MARKER(0x80073604, "SLUS_008.06");
    w_u32(flags, 0);
    uint32 parameters = r_u32(0x8009873c);
    if (parameters == 0)
        return 0;
    uint32 descriptors = r_u32(parameters + 0xc);
    return r_u32(descriptors);
}

void G3Function80044A90(uint32 effect, uint32 rotation, uint32 velocity)
{
    FUNCTION_MARKER(0x80044a90, "SLUS_008.06");
    if (effect == 0)
        return;
    for (uint32 node = 0; node < 2; ++node)
    {
        uint32 flags = r_u32(effect + 0xc);
        w_u32(effect + 0xc, flags & 0xfffffffd);
        for (uint32 axis = 0; axis < 3; ++axis)
            w_u16(effect + 0x4c + axis * 2, r_u16(rotation + axis * 2));
        for (uint32 axis = 0; axis < 3; ++axis)
            w_u16(effect + 0x52 + axis * 2, r_u16(velocity + axis * 2));
        if (node == 0)
        {
            effect = r_u32(effect + 4);
            if (effect == 0)
                return;
        }
    }
}

void G3Function80044878(uint32 effect, uint32 size, uint32 *return_value)
{
    FUNCTION_MARKER(0x80044878, "SLUS_008.06");
    if (effect == 0 || size == 0)
        return;
    for (uint32 node = 0; node < 2; ++node)
    {
        sint32 coordinate = r_s16(effect + 0x24);
        for (uint32 axis = 0; axis < 6; ++axis)
        {
            sint32 scale = r_s16(size + (axis & 1) * 4);
            sint32 product = coordinate * scale;
            sint32 next = axis + 1 < 6 ? r_s16(effect + 0x28 + axis * 4) : 0;
            uint32 result = (uint32)(product >> 12);
            w_u16(effect + 0x24 + axis * 4, result);
            if (axis == 5 && return_value != 0)
                *return_value = result;
            coordinate = next;
        }
        if (node == 0)
        {
            effect = r_u32(effect + 4);
            if (effect == 0)
                return;
        }
    }
}

uint32 G3Function800440D0(uint32 object, uint32 pool, uint32 x, uint32 y, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800440d0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x68;
    uint32 resource = G3Function80073604(object, frame + 0x30, frame + 0x38);
    uint32 storage = G3Function800415CC(pool, frame);
    if (resource == 0 || storage == 0)
        return storage;
    uint32 sized = r_u32(frame + 0x38);
    uint32 z = r_u32(guest_stack + 0x10);
    w_u16(frame + 0x18, x);
    w_u16(frame + 0x1a, y);
    w_u16(frame + 0x20, 0);
    w_u16(frame + 0x22, 0);
    w_u16(frame + 0x24, 0);
    w_u16(frame + 0x28, 0);
    w_u16(frame + 0x2a, 0);
    w_u16(frame + 0x2c, 0);
    w_u16(frame + 0x1c, z);
    if (sized == 0)
    {
        w_u16(frame + 0x30, 0x1000);
        w_u16(frame + 0x32, 0);
        w_u16(frame + 0x34, 0x1000);
    }
    uint32 game = r_u32(0x80097048);
    uint32 angle = r_u8(game + 0x14c) << 4;
    for (uint32 index = 0; index < 8; ++index)
    {
        uint32 sine = G3Function8005B470(angle);
        w_u16(frame + 0x22, 0);
        uint32 product = 12 * sine;
        w_u16(frame + 0x20, (uint32)((sint32)product >> 12));
        uint32 cosine = G3Function8005B590(angle);
        product = 12 * cosine;
        w_u16(frame + 0x24, (uint32)((sint32)product >> 12));
        for (uint32 plane = 0; plane < 2; ++plane)
        {
            if (plane != 0)
            {
                uint32 coordinate = r_u16(frame + 0x20);
                w_u16(frame + 0x20, 0);
                w_u16(frame + 0x22, coordinate);
            }
            w_u32(frame + 0x10, storage);
            uint32 effect = G3Function80044688(frame + 0x18, resource, pool, 0xffffffff, frame);
            if (effect == 0)
                return 0;
            G3Function80044A90(effect, frame + 0x20, frame + 0x28);
            sized = r_u32(frame + 0x38);
            if (sized != 0)
            {
                G3Function80044878(effect, frame + 0x30, 0);
                sized = r_u32(frame + 0x38);
            }
            if (sized == 0)
            {
                G3Function80044068(effect, frame + 0x30, 15, 10);
                effect = r_u32(effect + 4);
                if (effect != 0)
                    G3Function80044068(effect, frame + 0x30, 15, 10);
            }
        }
        angle += 0x200;
    }
    return 0;
}

void G3Function80044360(uint32 object, uint32 resource, uint32 center_resource, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80044360, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    if (resource == 0 || center_resource == 0)
        return;
    G3Function800444E4(object + 0x58, center_resource, pool, 3, frame);
    uint32 angle = 0;
    for (uint32 index = 0; index < 6; ++index)
    {
        uint32 cosine = G3Function8005B590(angle);
        uint32 sine = G3Function8005B470(angle);
        uint32 radius = r_u32(0x8009e024);
        uint32 x_product = radius * sine;
        uint32 y_product = radius * cosine;
        uint32 speed = r_u32(0x8009e02c);
        uint32 first_product = speed * sine;
        uint32 coordinate = r_u16(object + 0x58);
        w_u16(frame + 0x10, coordinate + (uint32)((sint32)x_product >> 12));
        uint32 second_product = speed * cosine;
        coordinate = r_u16(object + 0x5a);
        w_u16(frame + 0x12, coordinate + (uint32)((sint32)y_product >> 12));
        coordinate = r_u16(object + 0x5c);
        uint32 height = r_u16(0x8009e028);
        w_u16(frame + 0x14, coordinate + height);
        uint32 first = (uint32)((sint32)first_product >> 12);
        uint32 second = (uint32)((sint32)second_product >> 12);
        uint32 effect = G3Function800444E4(frame + 0x10, resource, pool, 0xffffffff, frame);
        if (effect != 0)
        {
            uint32 flags = r_u32(effect + 0xc);
            uint32 rotation = r_u16(0x8009e030);
            uint32 velocity = r_u16(0x8009e034);
            uint32 child = r_u32(effect + 4);
            w_u16(effect + 0x4c, first);
            w_u16(effect + 0x4e, second);
            w_u32(effect + 0xc, flags & 0xfffffffd);
            w_u16(effect + 0x50, rotation);
            w_u16(effect + 0x56, velocity);
            if (child != 0)
            {
                flags = r_u32(child + 0xc);
                w_u16(child + 0x4c, first);
                w_u16(child + 0x4e, second);
                w_u16(child + 0x50, rotation);
                w_u16(child + 0x56, velocity);
                w_u32(child + 0xc, flags & 0xfffffffd);
            }
        }
        angle += 0x2aa;
    }
}

uint32 G3Function80044564(uint32 binding, uint32 resource, uint32 storage, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80044564, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    if (storage == 0)
    {
        storage = G3Function800415CC(pool, frame);
        if (storage == 0)
            return 0;
    }
    uint32 channel = r_u32(binding);
    uint32 position = r_u32(channel);
    w_u32(frame + 0x10, storage);
    uint32 effect = G3Function80044688(position + 2, resource, pool, 0xfffffffe, frame);
    if (effect == 0)
        return 0;
    uint32 flags = r_u32(effect + 0xc);
    uint32 child = r_u32(effect + 4);
    w_u32(effect + 0x4c, binding);
    w_u16(effect + 0x22, 0xfffe);
    w_u32(effect + 0xc, flags | 0x200);
    if (child != 0)
    {
        flags = r_u32(child + 0xc);
        w_u32(child + 0x4c, binding);
        w_u16(child + 0x22, 0xfffe);
        w_u32(child + 0xc, flags | 0x200);
    }
    for (uint32 state = 0; state < 2; ++state)
    {
        channel = r_u32(binding + state * 8);
        if (channel != 0)
        {
            uint32 offset = 0x50 + state * 4;
            G3Function8003FBB0(channel, effect + offset, frame);
            if (child != 0)
            {
                uint32 source = effect + offset;
                uint32 high_address = source + 3;
                uint32 high_lane = high_address & 3;
                uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                uint32 low_lane = source & 3;
                uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
                uint32 value = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
                uint32 destination = child + offset;
                high_address = destination + 3;
                high_lane = high_address & 3;
                w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                low_lane = destination & 3;
                w_masked_u32(destination & 0xfffffffc, value << (low_lane * 8), (15 << low_lane) & 15);
            }
        }
    }
    return effect;
}

uint32 G3Function800462D4(uint32 game, uint32 value, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800462d4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 data = r_u32(0x80098764);
    uint32 object = G3Function8002A454(0, data, frame);
    if (object == 0)
        return 0;
    uint32 positions = r_u32(game + 0x68ac);
    w_u32(object + 0x138, 0x1c2);
    uint32 flags = r_u32(object + 0x128);
    w_u32(object + 0x128, flags | 4);
    w_u16(object + 0x13e, value + 1);
    uint32 position = positions + value * 6;
    w_u16(object + 0x58, r_u16(position));
    w_u16(object + 0x5a, r_u16(position + 2));
    w_u16(object + 0x5c, r_u16(position + 4));
    uint32 states = r_u32(game + 0x68b0);
    uint32 result = states + value;
    w_u8(result, 1);
    return result;
}

uint32 G3Function80046540(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80046540, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 result = r_u32(object + 0x128) & 1;
    if (result != 0)
        return result;
    uint32 index = G3Function80046384(object);
    if (index != 0 && (sint32)index < 101)
    {
        uint32 states = r_u32(game + 0x68b0);
        w_u8(states + index - 1, 3);
    }
    uint32 flags = r_u32(object + 0x128);
    w_u32(object + 0x128, (flags | 1) & 0xfffffff7);
    result = r_u32(object + 0x40);
    if (result == 0)
    {
        w_u32(frame + 0x10, 1500);
        result = G3Function8003E518(object, 0x802, 0, 127, frame);
        w_u32(object + 0x40, result);
    }
    return result;
}

uint32 G3Function800465F8(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800465f8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 index = G3Function80046384(object);
    if (index != 0 && (sint32)index < 101)
    {
        uint32 states = r_u32(game + 0x68b0);
        if (r_u8(states + index - 1) == 4)
        {
            uint32 result = ((r_u32(object + 0x10) | 0x10) & 0xfffffbff) | 0x20;
            w_u32(object + 0x10, result);
            return result;
        }
    }
    if (r_u32(object + 0x40) != 0)
    {
        w_u32(frame + 0x10, 1500);
        uint32 handle = r_u32(object + 0x40);
        G3Function8003E7E4(object, handle, 0, 127, frame);
    }
    w_u16(object + 0x9e, r_u16(object + 0x9e) + 1);
    uint32 flags = r_u32(object + 0x128);
    if (flags == 0)
        return flags & 8;
    if ((flags & 8) != 0)
    {
        uint32 remaining = r_u32(object + 0x138) - 1;
        w_u32(object + 0x138, remaining);
        if (remaining == 0xffffffff)
            G3Function80046540(object, game, frame);
    }
    else if ((flags & 1) != 0)
    {
        uint32 z = (uint32)r_s16(object + 0x5c);
        uint32 pool = r_u32(0x800a27fc);
        uint32 x = (uint32)r_s16(object + 0x58);
        uint32 y = (uint32)r_s16(object + 0x5a);
        w_u32(frame + 0x10, z);
        uint32 player = r_u32(game + 0x50);
        G3Function800440D0(player, pool, x, y, frame);
        uint32 count = r_u32(game + 0x6820) + 1;
        w_u32(game + 0x6820, count);
        bool award = false;
        if (count == 50 && r_u32(game + 0x6830) == 0)
        {
            uint32 lives = r_u32(game + 0x6818);
            w_u32(game + 0x6830, 1);
            ++lives;
            w_u32(game + 0x6818, lives);
            if (lives == 100)
                w_u32(game + 0x6818, 99);
            player = r_u32(game + 0x50);
            uint32 data = r_u32(0x80098814);
            G3Function8002A454(player, data, frame);
            award = true;
        }
        else if (r_u32(game + 0x6820) == 100)
        {
            G3Function8006C914(game, frame);
            award = true;
        }
        if (award)
        {
            G3Function800359EC(0x120, 3, frame);
            if (r_u32(game + 0x6820) != 100)
                G3Function80047EE4(1, r_u32(game + 0x6820));
        }
        else
            G3Function80047EE4(1, r_u32(game + 0x6820));
        uint32 object_flags = r_u32(object + 0x10);
        w_u32(object + 0x10, object_flags | 0x10);
        G3Function800193F8(0x40000, object, frame);
        return G3Function8002A9AC(object, frame);
    }
    if ((r_u32(object + 0x128) & 2) == 0)
        return 0;
    uint32 vx = r_u32(object + 0x104);
    uint32 ax = r_u32(object + 0x110);
    uint32 ay = r_u32(object + 0x114);
    uint32 az = r_u32(object + 0x118);
    w_u32(object + 0x104, vx + ax);
    uint32 vy = r_u32(object + 0x108);
    uint32 vz = r_u32(object + 0x10c);
    w_u32(object + 0x108, vy + ay);
    uint32 x = r_u16(object + 0x58);
    vx = r_u16(object + 0x104);
    w_u32(object + 0x10c, vz + az);
    uint32 y = r_u16(object + 0x5a);
    vy = r_u16(object + 0x108);
    w_u16(object + 0x58, x + vx);
    uint32 z = r_u16(object + 0x5c);
    w_u16(object + 0x5a, y + vy);
    vz = r_u16(object + 0x10c);
    uint32 floor = r_u16(object + 0xe0);
    z += vz;
    w_u16(object + 0x5c, z);
    sint32 ceiling = r_s16(object + 0x13c);
    if (ceiling + 0x300 < (sint16)(z - floor))
    {
        ax = r_u32(object + 0x110);
        ay = r_u32(object + 0x114);
        w_u32(object + 0x110, 0 - ax);
        vx = r_u32(object + 0x104);
        w_u32(object + 0x114, 0 - ay);
        vy = r_u32(object + 0x108);
        w_u32(object + 0x104, 0 - vx);
        w_u32(object + 0x108, 0 - vy);
        uint32 source = object + 0x70;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
        uint32 value = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        uint32 restored_z = (uint32)r_s16(object + 0x74);
        uint32 destination = object + 0x58;
        high_address = destination + 3;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, value << (low_lane * 8), (15 << low_lane) & 15);
        w_u16(object + 0x5c, restored_z);
        x = r_u16(object + 0x58);
        vx = r_u16(object + 0x104);
        y = r_u16(object + 0x5a);
        vy = r_u16(object + 0x108);
        w_u16(object + 0x58, x + vx);
        w_u16(object + 0x5a, y + vy);
    }
    else
    {
        if (r_s16(object + 0xb8) < 0xb50)
        {
            uint32 drift_x = r_u16(object + 0xb4);
            x = r_u16(object + 0x58);
            uint32 drift_y = r_u16(object + 0xb6);
            w_u16(object + 0x58, x + (uint32)((sint16)drift_x >> 10));
            y = r_u16(object + 0x5a);
            uint32 drift_z = r_u16(object + 0xb8);
            w_u16(object + 0x5a, y + (uint32)((sint16)drift_y >> 10));
            z = r_u16(object + 0x5c);
            w_u16(object + 0x5c, z + (uint32)((sint16)drift_z >> 10));
        }
        sint32 height = r_s16(object + 0x5c);
        sint32 ground = r_s16(object + 0xe0);
        bool settle;
        if (height - ground < 128 && r_s32(object + 0x118) >= 0)
            settle = true;
        else
        {
            if (height - ground < 128)
            {
                vz = r_u32(object + 0x10c);
                uint32 rounded = vz + (vz >> 31);
                az = r_u32(object + 0x118);
                w_u32(object + 0x10c, 0 - (uint32)((sint32)rounded >> 1));
                w_u32(object + 0x118, az + 2);
            }
            settle = r_s32(object + 0x118) >= 0;
        }
        if (settle)
        {
            ax = r_u32(object + 0x110);
            ay = r_u32(object + 0x114);
            height = r_s16(object + 0x5c);
            ground = r_s16(object + 0xe0);
            az = r_u32(object + 0x118);
            vx = r_u32(object + 0x104);
            vy = r_u32(object + 0x108);
            vz = r_u32(object + 0x10c);
            z = r_u16(object + 0x5c);
            w_u32(object + 0x110, (uint32)((sint32)ax > 0) - (ax >> 31));
            w_u32(object + 0x114, (uint32)((sint32)ay > 0) - (ay >> 31));
            w_u32(object + 0x118, (uint32)((sint32)az > 0) - (az >> 31));
            w_u32(object + 0x104, (uint32)((sint32)vx > 0) - (vx >> 31));
            w_u32(object + 0x118, 0);
            w_u32(object + 0x10c, (uint32)((sint32)vz > 0) - (vz >> 31));
            w_u32(object + 0x108, (uint32)((sint32)vy > 0) - (vy >> 31));
            w_u32(object + 0x10c, 0);
            if (height - ground < 128)
                w_u16(object + 0x5c, z + 1);
            else if (height - ground >= 129)
                w_u16(object + 0x5c, z - 1);
            else
                w_u32(object + 0x128, r_u32(object + 0x128) & 0xfffffffd);
        }
    }
    uint32 result = r_u32(object + 0x128) & 2;
    if (result != 0)
        return result;
    uint32 model = r_u32(object + 0x1c);
    w_u32(object + 0x110, 0);
    w_u32(object + 0x104, 0);
    w_u32(object + 0x114, 0);
    w_u32(object + 0x108, 0);
    w_u32(object + 0x118, 0);
    w_u32(object + 0x10c, 0);
    uint32 model_flags = r_u32(model + 0x30);
    w_u32(model + 0x30, model_flags & 0xffffffbf);
    G3Function8002A748(object, frame);
    result = r_u32(object + 0x128) & 0xfffffffd;
    w_u32(object + 0x128, result);
    return result;
}

uint32 G3Function80047DF4(uint32 state, uint32 level)
{
    FUNCTION_MARKER(0x80047df4, "SLUS_008.06");
    if (level >= 28)
        level = 0;
    uint32 game = r_u32(0x80097048);
    uint32 bits = r_u8(game + level + 0x67f8);
    uint32 count = 0;
    while (bits != 0)
    {
        bits &= bits - 1;
        ++count;
    }
    game = r_u32(0x80097048);
    uint32 flags = r_u32(game + 0x6838);
    if ((flags & (uint32(1) << level)) != 0)
        ++count;
    uint32 descriptor = 0x80099708 + level * 28;
    w_u16(state + 0xc, count);
    uint32 total = r_u16(descriptor + 0xa);
    w_u16(state + 6, total + 1);
    bits = (uint32)r_s16(game + level * 2 + 0x683c);
    count = 0;
    while (bits != 0)
    {
        bits &= bits - 1;
        ++count;
    }
    game = r_u32(0x80097048);
    w_u16(state + 0xe, count);
    uint32 result = game + level * 2;
    bits = (uint32)r_s16(result + 0x6874);
    count = 0;
    while (bits != 0)
    {
        result = bits - 1;
        bits &= result;
        ++count;
    }
    w_u16(state, count);
    return result;
}

uint32 G3Function80047EE4(uint32 kind, uint32 value)
{
    FUNCTION_MARKER(0x80047ee4, "SLUS_008.06");
    uint32 state = 0x800a218c + (kind << 4);
    sint32 phase = r_s16(state + 4);
    uint32 current = r_u16(state + 4);
    if (phase == 5)
        return 5;
    if (current - 3 < 2)
    {
        w_u16(state + 6, 144);
        uint32 elapsed = phase == 3 ? 72 - r_u16(state + 8) : 72;
        w_u16(state + 8, elapsed);
        w_u16(state + 4, 1);
        w_u16(state + 0xc, 0);
        if (kind == 6)
            w_u16(state + 8, value << 2);
    }
    else if (phase == 2)
        w_u16(state + 0xa, 90);
    uint32 result = 11;
    if (kind == 11)
        result = G3Function80047DF4(state, value);
    w_u16(state + 2, value);
    if (kind == 0)
    {
        result = value - 1;
        w_u16(state + 2, result);
    }
    return result;
}

uint32 G3Function8006C914(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006c914, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 level = r_u16(game + 0x68b4);
    uint32 previous_count = r_u32(game + 0x682c);
    uint32 previous_flags = r_u32(game + 0x6838);
    uint32 result = 0;
    if (level - 11 < 5)
    {
        uint32 lives = r_u32(game + 0x6818);
        w_u32(game + 0x6830, 1);
        ++lives;
        w_u32(game + 0x6818, lives);
        if (lives == 100)
            w_u32(game + 0x6818, 99);
        uint32 player = r_u32(game + 0x50);
        uint32 data = r_u32(0x80098814);
        G3Function8002A454(player, data, frame);
    }
    else if (((level - 20) & 0xffff) < 3)
    {
        uint32 shift = r_u16(game + 0x68b4) & 31;
        w_u32(game + 0x6838, previous_flags | (uint32(1) << shift));
        result = G3Function8006C6B4(game, 0, frame);
        uint32 player = r_u32(0x8009efc4);
        w_u16(player + 0x6e, 0);
        w_u32(player + 0x154, 0x02000000);
        if (r_u16(game + 0x68b4) == 22)
            w_u32(game + 0x682c, r_u32(game + 0x682c) + 1);
    }
    else
    {
        uint32 repeat = 1;
        if (level < 28)
        {
            uint32 shift = r_u16(game + 0x68b4) & 31;
            uint32 bit = uint32(1) << shift;
            if ((previous_flags & bit) == 0)
            {
                repeat = 0;
                uint32 count = r_u32(game + 0x682c);
                w_u32(game + 0x6838, previous_flags | bit);
                w_u32(game + 0x682c, count + 1);
            }
        }
        result = G3Function8006C6B4(game, repeat, frame);
    }
    uint32 camera = r_u32(game + 0x4c);
    G3Function80051FE4(camera, frame);
    G3Function8006C774(game, previous_count, previous_flags, frame);
    return result;
}

uint32 G3Function80046B70(uint32 x, uint32 y, uint32 model, uint32 scale, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80046b70, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x128;
    uint32 angle = r_u32(guest_stack + 0x10);
    uint32 render_game = r_u32(0x80097048);
    uint32 packets = r_u32(game + 0x68);
    uint32 order_table = r_u32(game + 0x38);
    w_u32(frame + 0xf0, order_table);
    uint32 words[2];
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 source = 0x8009e854 + word * 4;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
        words[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 destination = frame + 0x18 + word * 4;
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        uint32 low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15 << low_lane) & 15);
    }
    uint32 matrices = r_u32(render_game + 0x6558);
    uint32 vertices = r_u32(render_game + 0x64);
    w_u32(frame + 0xf8, vertices);
    uint32 count = (uint32)r_s16(model + 4);
    uint32 packet_limit = r_u32(packets + 8);
    uint32 source = r_u32(model + 8);
    uint32 cursor = r_u32(packets + 4);
    uint32 result = count * 48;
    w_u32(frame + 0xf4, source);
    if (cursor + result >= packet_limit)
        return result;
    count = (uint32)r_s16(model + 6);
    uint32 matrix_limit = r_u32(render_game + 0x655c);
    result = matrices + (count << 5) + 0x20;
    angle &= 0xfff;
    if (matrix_limit < result)
        return result;
    x -= 0x100;
    y -= 0x78;
    w_u16(frame + 0x1c, angle);
    G3Function8005BD00(frame + 0x18, frame + 0x30);
    uint32 half_scale = (uint32)((sint32)(scale + (scale >> 31)) >> 1);
    w_u32(frame + 0x28, scale);
    w_u32(frame + 0x24, scale);
    w_u32(frame + 0x20, scale);
    w_u32(frame + 0x28, half_scale);
    G3Function8005ADF0(frame + 0x30, frame + 0x20);
    w_u32(frame + 0x44, (uint32)((sint32)(sint16)x / 2));
    w_u32(frame + 0x48, (uint32)((sint32)(sint16)y / 2));
    w_u32(frame + 0x4c, 200);
    w_u32(frame + 0x80, 0x1000);
    w_u32(frame + 0x78, 0x1000);
    w_u32(frame + 0x70, 0x1000);
    w_u32(frame + 0x7c, 0);
    w_u32(frame + 0x74, 0);
    G3Function8005BB6C(angle, frame + 0x70, frame);
    w_u16(frame + 0x92, 0xeef4);
    w_u16(frame + 0x90, 0x93c);
    w_u16(frame + 0x94, 0x93c);
    for (uint32 element = 0; element < 6; ++element)
        w_u16(frame + 0x96 + element * 2, 0);
    for (uint32 element = 0; element < 9; ++element)
        w_u16(frame + 0xb0 + element * 2, element % 3 == 1 ? 0x800 : 0xfff);
    MulMatrix0((MATRIX *)psx_addr(frame + 0x90, 20), (MATRIX *)psx_addr(frame + 0x70, 20), (MATRIX *)psx_addr(frame + 0x50, 20));
    SetLightMatrix((MATRIX *)psx_addr(frame + 0x50, 20));
    SetColorMatrix((MATRIX *)psx_addr(frame + 0xb0, 20));
    uint32 base = matrices;
    if (r_u16(model) < 0x72)
        w_u32(frame + 0xf8, 0x1f80006c);
    if (r_s16(model + 6) > 0)
    {
        sint32 index = 0;
        uint32 offset = 0;
        uint32 translation = base + 0x1c;
        do
        {
            uint32 table = r_u32(model + 0x18);
            uint32 bone = table + offset;
            uint32 flags = r_u32(bone);
            if ((flags & 2) == 0)
            {
                for (uint32 group = 0; group < 2; ++group)
                {
                    uint32 row[4];
                    for (uint32 word = 0; word < 4; ++word)
                        row[word] = r_u32(frame + 0x30 + group * 16 + word * 4);
                    for (uint32 word = 0; word < 4; ++word)
                        w_u32(matrices + group * 16 + word * 4, row[word]);
                }
            }
            else
            {
                w_u16(frame + 0x1c, 0);
                G3Function8005BD00(frame + 0x18, matrices);
                G3Function8005ADF0(matrices, frame + 0x20);
            }
            G3Function8005ADA0(frame + 0x30, bone + 0xc, matrices + 0x14);
            uint32 coordinate = r_u32(translation - 8);
            uint32 adjustment = r_u32(frame + 0x44);
            matrices += 0x20;
            w_u32(translation - 8, coordinate + adjustment);
            coordinate = r_u32(translation - 4);
            adjustment = r_u32(frame + 0x48);
            offset += 0x18;
            w_u32(translation - 4, coordinate + adjustment);
            coordinate = r_u32(translation);
            adjustment = r_u32(frame + 0x4c);
            ++index;
            w_u32(translation, coordinate + adjustment);
            count = (uint32)r_s16(model + 6);
            translation += 0x20;
        } while (index < (sint32)count);
    }
    uint32 restore = r_u32(guest_stack + 0x14);
    w_u32(frame + 0xe0, 0x1000);
    w_u32(frame + 0xd8, 0x1000);
    w_u32(frame + 0xd0, 0x1000);
    w_u32(frame + 0xdc, 0);
    w_u32(frame + 0xd4, 0);
    w_u32(frame + 0xec, 0);
    w_u32(frame + 0xe8, 0);
    w_u32(frame + 0xe4, 0);
    w_u32(0x8009d918, restore != 0 ? 1 : 0);
    source = r_u32(frame + 0xf4);
    vertices = r_u32(frame + 0xf8);
    w_u32(frame + 0x10, base);
    G3Function800194C4(source, vertices, model, frame + 0xd0, base, frame);
    restore = r_u32(guest_stack + 0x14);
    uint32 enabled = 0;
    if (restore != 0)
    {
        game = r_u32(0x80097048);
        uint32 camera = r_u32(game + 0x48);
        uint32 other_game = r_u32(0x80097048);
        uint32 matrix = r_u32(camera + 0xf4);
        uint32 row[4];
        for (uint32 word = 0; word < 4; ++word)
            row[word] = r_u32(matrix + 0x3c + word * 4);
        for (uint32 word = 0; word < 4; ++word)
            w_u32(matrix + word * 4, row[word]);
        uint32 last = r_u32(matrix + 0x4c);
        w_u32(matrix + 0x10, last);
        camera = r_u32(other_game + 0x48);
        matrix = r_u32(camera + 0xf4);
        sint32 numerator = r_s16(matrix + 0x12) * 4096;
        uint32 quotient = scale == 0 ? (numerator < 0 ? 1 : uint32(0xffffffff)) : (uint32)((sint64)numerator / (sint32)scale);
        enabled = 1;
        w_u16(matrix + 0x12, quotient);
    }
    G3Function80033A54(enabled);
    vertices = r_u32(frame + 0xf8);
    game = r_u32(0x80097048);
    order_table = r_u32(frame + 0xf0);
    uint32 target = r_u32(game + 0x65d8);
    result = G3DispatchPolygonRender(target, model, vertices, packets, order_table, frame);
    w_u32(packets + 4, result);
    return result;
}

uint32 G3Function80047190(uint32 state, uint32 parameters, uint32 single_position, uint32 double_position)
{
    FUNCTION_MARKER(0x80047190, "SLUS_008.06");
    w_u32(parameters, 5);
    w_u32(parameters + 4, 0);
    sint32 current = r_s16(state);
    w_u32(parameters + 0xc, current < 10 ? single_position : double_position);
    current = r_s16(state);
    sint32 target = r_s16(state + 2);
    if (current != target)
    {
        sint32 phase = r_s16(state + 4);
        if (phase == 2 && r_s16(state + 0xc) == 0)
        {
            w_u16(state + 0xc, 1);
            w_u32(parameters + 0x1c, 0x800);
        }
        else
        {
            sint32 step = r_s16(state + 0xc);
            if (step == 1)
            {
                w_u16(state + 0xc, 2);
                w_u32(parameters + 0x1c, 0xfffff800);
            }
            else if (step == 2)
            {
                w_u16(state + 0xc, 0);
                w_u32(parameters + 0x1c, 0x1000);
                target = r_s16(state + 2);
                current = r_s16(state);
                uint32 destination = r_u16(state + 2);
                uint32 value = r_u16(state);
                w_u16(state, current < target ? value + 1 : destination);
            }
        }
        if (r_s16(state + 4) == 2)
            w_u16(state + 0xa, 90);
    }
    current = r_s16(state);
    w_u16(parameters + 0x14, 0);
    w_u16(parameters + 0x16, 0);
    w_u32(parameters + 8, (uint32)current);
    G3Function80021A3C(parameters, 0, 0);
    current = (sint16)r_u16(state);
    if (current >= 10)
    {
        target = (sint16)r_u16(state + 2);
        sint32 tens = current / 10;
        sint32 target_tens = target / 10;
        if ((sint16)tens == (sint16)target_tens || (sint16)(current - tens * 10) != 9)
            w_u32(parameters + 0x1c, 0x1000);
        w_u32(parameters + 0xc, single_position);
        current = (sint16)r_u16(state);
        w_u32(parameters + 8, (uint32)(sint32)(sint16)(current / 10));
        G3Function80021A3C(parameters, 0, 0);
    }
    w_u32(parameters + 0x1c, 0x1000);
    return 0x1000;
}

uint32 G3Function80046FD0(uint32 x, uint32 y, sint32 value, sint32 total, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80046fd0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    G3Function800210F4(0x2f, x, y);
    if (value >= 10)
    {
        w_u32(frame + 0x10, 5);
        w_u32(frame + 0x1c, x - 36);
        w_u32(frame + 0x14, 0);
        w_u32(frame + 0x20, y);
        w_u32(frame + 0x2c, 0x1000);
        w_u32(frame + 0x28, 0x1000);
        w_u16(frame + 0x24, 0);
        w_u16(frame + 0x26, 0);
        w_u32(frame + 0x18, (uint32)(value / 10));
        G3Function80021A3C(frame + 0x10, 0, 0);
    }
    w_u32(frame + 0x10, 5);
    w_u32(frame + 0x1c, x - 16);
    w_u32(frame + 0x2c, 0x1000);
    w_u32(frame + 0x28, 0x1000);
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x20, y);
    w_u16(frame + 0x24, 0);
    w_u16(frame + 0x26, 0);
    w_u32(frame + 0x18, (uint32)(value % 10));
    G3Function80021A3C(frame + 0x10, 0, 0);
    if (total >= 10)
    {
        uint32 position_x = r_u32(frame + 0x1c);
        uint32 position_y = r_u32(frame + 0x20);
        w_u32(frame + 0x20, position_y + 10);
        w_u32(frame + 0x1c, position_x + 40);
        w_u32(frame + 0x18, (uint32)(total / 10));
        G3Function80021A3C(frame + 0x10, 0, 0);
        position_x = r_u32(frame + 0x1c);
        position_y = r_u32(frame + 0x20);
        w_u32(frame + 0x1c, position_x - 20);
        w_u32(frame + 0x20, position_y - 10);
    }
    uint32 position_x = r_u32(frame + 0x1c);
    uint32 position_y = r_u32(frame + 0x20);
    w_u32(frame + 0x1c, position_x + 40);
    w_u32(frame + 0x20, position_y + 10);
    w_u32(frame + 0x18, (uint32)(total % 10));
    return G3Function80021A3C(frame + 0x10, 0, 0);
}

uint32 G3Function80047FC8(uint32 level)
{
    FUNCTION_MARKER(0x80047fc8, "SLUS_008.06");
    for (uint32 index = 0; index < 12; ++index)
    {
        uint32 state = 0x800a218c + index * 16;
        sint32 phase = r_s16(state + 4);
        if (level == 0xffffffff ? phase < 0 : phase > 0)
            w_u16(state + 4, 0 - (uint32)phase);
    }
    if (level == 0xffffffff)
    {
        w_u16(0x800a2240, 5);
        return 5;
    }
    w_u16(0x800a2240, 4);
    return G3Function80047EE4(11, level);
}

uint32 G3Function80047394(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80047394, "SLUS_008.06");
    uint32 frame = guest_stack - 0xa0;
    w_u32(guest_stack, game);
    uint32 state = 0x800a218c;
    for (uint32 index = 0; index < 12; ++index, state += 16)
    {
        w_u32(frame + 0x30, 0x1000);
        w_u32(frame + 0x34, 0x1000);
        sint32 phase = r_s16(state + 4);
        if (phase <= 0 || phase == 4 || phase == 5)
            continue;
        if (phase == 1 || phase == 3)
        {
            if (index - 1 < 4)
            {
                uint32 remaining = r_u16(state + 6) - 8;
                w_u16(state + 6, remaining);
                if ((sint16)remaining <= 0)
                {
                    uint32 next = r_u16(state + 4);
                    w_u16(state + 6, 0);
                    w_u16(state + 0xa, 90);
                    w_u16(state + 4, next + 1);
                }
            }
            else
            {
                uint32 remaining = r_u16(state + 8) - 4;
                w_u16(state + 8, remaining);
                if ((sint16)remaining <= 0)
                {
                    uint32 next = r_u16(state + 4);
                    w_u16(state + 8, 0);
                    w_u16(state + 4, next + 1);
                    w_u16(state + 0xa, index == 6 ? 20 : 90);
                }
            }
        }
        else if (phase == 2)
        {
            uint32 remaining = r_u16(state + 0xa) - 1;
            w_u16(state + 0xa, remaining);
            if ((remaining & 0xffff) == 0)
            {
                uint32 next = r_u16(state + 4);
                w_u16(state + 8, 72);
                w_u16(state + 6, 144);
                w_u16(state + 4, next + 1);
            }
        }
        phase = r_s16(state + 4);
        if (phase == 4)
            continue;
        if (index - 1 < 4)
        {
            if (phase == 1)
                w_u32(frame + 0x24, (uint32)r_s16(state + 6) + 432);
            else if (phase == 3)
                w_u32(frame + 0x24, 576 - (uint32)r_s16(state + 6));
            else if (phase == 2)
                w_u32(frame + 0x24, 432);
        }
        else
        {
            if (phase == 1)
                w_u32(frame + 0x28, 36 - (uint32)r_s16(state + 8));
            else if (phase == 3)
                w_u32(frame + 0x28, (uint32)r_s16(state + 8) - 36);
            else if (phase == 2)
                w_u32(frame + 0x28, 36);
        }
        switch (index)
        {
        case 0:
        {
            G3Function80047190(state, frame + 0x18, 83, 105);
            uint32 advanced = r_u16(state + 0xe) + 96;
            sint32 angle = (sint16)advanced;
            w_u16(state + 0xe, advanced);
            angle %= 4096;
            w_u16(state + 0xe, (uint32)angle);
            uint32 resource = r_u32(0x80098814);
            if (resource != 0)
            {
                uint32 y = r_u16(frame + 0x28);
                uint32 table = r_u32(resource + 0xc);
                w_u32(frame + 0x10, (uint32)angle);
                w_u32(frame + 0x14, 0);
                uint32 model = r_u32(table);
                G3Function80046B70(0xfffffffc, (uint32)(sint32)(sint16)(y - 18), model, 0x288, frame);
            }
            w_u16(state, r_u16(state + 2));
            break;
        }
        case 1:
        {
            uint32 x = r_u32(frame + 0x24);
            w_u32(frame + 0x28, 36);
            G3Function80047190(state, frame + 0x18, x + 28, x + 50);
            x = r_u32(frame + 0x24);
            w_u32(frame + 0x30, 3456);
            w_u32(frame + 0x34, 3456);
            w_u32(frame + 0x18, 15);
            uint32 y = r_u32(frame + 0x28);
            w_u32(frame + 0x1c, 0);
            w_u16(frame + 0x2c, 1);
            w_u16(frame + 0x2e, 0);
            w_u32(frame + 0x24, x - 38);
            uint32 global_game = r_u32(0x80097048);
            w_u32(frame + 0x28, y + 2);
            w_u32(frame + 0x20, r_u32(global_game + 0x14c));
            G3Function80021A3C(frame + 0x18, 0, 0);
            w_u32(frame + 0x1c, 1);
            w_u16(frame + 0x2c, 0);
            G3Function80021A3C(frame + 0x18, 0, 0);
            break;
        }
        case 2:
        case 3:
        case 4:
        {
            uint32 advanced = r_u16(state + 0xe) + (index == 2 ? uint32(0xffffffac) : index == 3 ? 128 : 96);
            sint32 angle = (sint16)advanced;
            w_u16(state + 0xe, advanced);
            angle %= 4096;
            w_u16(state + 0xe, (uint32)angle);
            uint32 resource = r_u32(index == 2 ? 0x8009876c : index == 3 ? 0x80098824 : 0x80098774);
            w_u32(frame + 0x28, index == 2 ? 108 : index == 3 ? 144 : 72);
            if (resource != 0)
            {
                uint32 x = r_u16(frame + 0x24);
                uint32 y = index == 2 ? (uint32)r_s16(frame + 0x28) : r_u16(frame + 0x28);
                uint32 table = r_u32(resource + 0xc);
                uint32 current_angle = (uint32)r_s16(state + 0xe);
                w_u32(frame + 0x14, index == 3 ? 0 : 1);
                x = (uint32)(sint32)(sint16)(x + (index == 2 ? 30 : index == 3 ? 29 : 28));
                y += index == 3 ? 28 : index == 4 ? uint32(0xfffffff8) : 0;
                w_u32(frame + 0x10, current_angle);
                uint32 model = r_u32(table);
                G3Function80046B70(x, (uint32)(sint32)(sint16)y, model, index == 3 ? 450 : 480, frame);
            }
            uint32 x = r_u32(frame + 0x24);
            G3Function80047190(state, frame + 0x18, x + 28, x + 50);
            break;
        }
        case 5:
        {
            uint32 current_game = r_u32(guest_stack);
            if (r_s16(current_game + 0x6786) == 6)
                break;
            uint32 global_game = r_u32(0x80097048);
            w_u32(frame + 0x18, 15);
            w_u32(frame + 0x1c, 0);
            uint32 value = r_u32(global_game + 0x14c);
            w_u32(frame + 0x24, 310);
            w_u32(frame + 0x28, 210);
            w_u16(frame + 0x2c, 1);
            w_u16(frame + 0x2e, 0);
            w_u32(frame + 0x20, value);
            w_u16(state + 8, 1000);
            G3Function80021A3C(frame + 0x18, 0, 0);
            w_u32(frame + 0x1c, 1);
            w_u16(frame + 0x2c, 0);
            G3Function80021A3C(frame + 0x18, 0, 0);
            break;
        }
        case 7:
        {
            sint32 value = r_s16(state + 2);
            uint32 global_game = r_u32(0x80097048);
            sint32 total = r_s16(global_game + 0x681e);
            if (value >= 9)
                value = 8;
            if (value < 0)
                value = 0;
            uint32 x = 170;
            w_u16(frame + 0x2c, 0);
            w_u32(frame + 0x18, 5);
            w_u32(frame + 0x1c, 1);
            for (sint32 item = 1; item < 5; ++item, x += 56)
            {
                w_u32(frame + 0x24, x);
                if (total >= item + 4)
                {
                    w_u32(frame + 0x20, 1);
                    w_u16(frame + 0x2e, value < item + 4);
                    G3Function80021A3C(frame + 0x18, 0, 0);
                }
                w_u32(frame + 0x20, 0);
                w_u16(frame + 0x2e, value < item);
                G3Function80021A3C(frame + 0x18, 0, 0);
            }
            break;
        }
        case 8:
            w_u32(frame + 0x18, 29);
            w_u32(frame + 0x24, 256);
            w_u32(frame + 0x28, 120);
            w_u32(frame + 0x1c, 0);
            w_u32(frame + 0x20, 0);
            w_u32(frame + 0x30, 6144);
            w_u32(frame + 0x34, 6144);
            w_u16(frame + 0x2c, 0);
            w_u16(frame + 0x2e, 0);
            G3Function80021A3C(frame + 0x18, 0, 0);
            break;
        case 10:
        {
            w_u16(state + 0xa, 2);
            w_u32(frame + 0x18, 40);
            w_u32(frame + 0x24, 168);
            w_u32(frame + 0x28, 220);
            w_u32(frame + 0x1c, 0);
            w_u32(frame + 0x30, 6144);
            w_u32(frame + 0x34, 6144);
            w_u16(frame + 0x2c, 0);
            w_u32(frame + 0x20, 0);
            w_u16(frame + 0x2e, 0);
            G3Function80021A3C(frame + 0x18, 0, 0);
            w_u32(frame + 0x24, r_u32(frame + 0x24) + 16);
            for (sint32 item = 0; item < 5; ++item)
            {
                sint32 count = r_s16(state + 2);
                if (item < count)
                    w_u32(frame + 0x20, 2);
                else
                {
                    sint32 timer = r_s16(state + 0xc);
                    if (timer == 0 || count != item)
                        w_u32(frame + 0x20, 11);
                    else
                    {
                        sint32 phase = (timer + 1) >> 1;
                        uint32 sprite = phase < 9 ? (uint32)(12 - phase) : (phase & 1) != 0 ? 2 : 3;
                        w_u32(frame + 0x20, sprite);
                        w_u16(state + 0xc, r_u16(state + 0xc) - 1);
                    }
                }
                G3Function80021A3C(frame + 0x18, 0, 0);
                w_u32(frame + 0x24, r_u32(frame + 0x24) + 32);
            }
            w_u32(frame + 0x20, 1);
            G3Function80021A3C(frame + 0x18, 0, 0);
            sint32 count = r_s16(state + 2);
            if (count < 5 && r_s16(state) != count)
                w_u16(state + 0xc, 30);
            w_u16(state, r_u16(state + 2));
            break;
        }
        case 11:
        {
            uint32 current_game = r_u32(guest_stack);
            sint32 mode = r_s16(current_game + 0x6786);
            uint32 level = (uint32)r_s16(state + 2);
            if (mode != 6)
            {
                G3Function80047FC8(0xffffffff);
                break;
            }
            w_u16(state + 0xa, 2);
            G3Function80021A74(640);
            uint32 descriptor = 0x80099708 + level * 28;
            for (uint32 line = 0; line < 2; ++line)
            {
                uint32 token = (uint32)r_s16(descriptor + 6 + line * 2);
                if (token != 0)
                {
                    uint32 text = G3Function80065D38(token);
                    uint32 formatted_length = sprintfPSX(frame + 0x38, 0x8009e85c, text);
                    uint32 copied_destination = frame + 0x3a;
                    uint32 third_glyph = copied_destination;
                    if (copied_destination < text)
                        third_glyph += formatted_length - 2;
                    G3Function800234A0(frame + 0x38, line == 0 ? 0xffffffc4 : 0xffffffec, 0xffffffff, third_glyph, frame, 0);
                }
            }
            uint32 text = G3Function80038F18(level, 0);
            G3Function80023448(text, 62, 0, 0, frame, 0);
            for (uint32 row = 0; row < 3; ++row)
            {
                uint32 angle_address = row == 0 ? 0x800a21ca : row == 1 ? 0x800a21da : 0x800a21ba;
                uint32 advanced = r_u16(angle_address) + (row == 0 ? 128 : row == 1 ? uint32(0xffffffa0) : 84);
                sint32 angle = (sint16)advanced;
                w_u16(angle_address, advanced);
                angle %= 4096;
                w_u32(frame + 0x24, 170);
                uint32 y = row == 0 ? 98 : r_u32(frame + 0x28) + 34;
                w_u16(angle_address, (uint32)angle);
                uint32 resource = r_u32(row == 0 ? 0x80098824 : row == 1 ? 0x80098774 : 0x8009876c);
                w_u32(frame + 0x28, y);
                if (resource != 0)
                {
                    uint32 x = r_u16(frame + 0x24);
                    y = r_u16(frame + 0x28);
                    uint32 table = r_u32(resource + 0xc);
                    w_u32(frame + 0x10, (uint32)angle);
                    w_u32(frame + 0x14, row == 0 ? 0 : 1);
                    uint32 model = r_u32(table);
                    x = (uint32)(sint32)(sint16)(x + (row == 0 ? 29 : row == 1 ? 28 : 30));
                    y = (uint32)(sint32)(sint16)(y + (row == 0 ? 8 : row == 1 ? 2 : 14));
                    G3Function80046B70(x, y, model, row == 0 ? 450 : 480, frame);
                }
                y = r_u32(frame + 0x28);
                if (row == 0)
                {
                    sint32 value = r_s16(state + 0xc);
                    uint32 x = r_u32(frame + 0x24);
                    sint32 total = r_s16(state + 6);
                    G3Function80046FD0(x + 112, y, value, total, frame);
                }
                else
                {
                    uint32 x = r_u32(frame + 0x24);
                    sint32 value = row == 1 ? r_s16(state + 0xe) : r_s16(state);
                    G3Function80046FD0(x + 112, y, value, row == 1 ? 10 : 3, frame);
                }
            }
            break;
        }
        default:
            break;
        }
    }
    return 0;
}

uint32 G3Function80040280(uint32 channel, uint32 position, uint32 state, uint32 incoming_s3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80040280, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    uint32 best = 0x7fffffff;
    uint32 closest = incoming_s3;
    sint32 count = r_s16(channel + 4);
    uint32 table = r_u32(channel);
    if (count > 0)
    {
        sint32 index = 0;
        uint32 node = table + 6;
        do
        {
            sint32 point = r_s16(position);
            sint32 coordinate = r_s16(node - 4);
            uint32 dx = (uint32)(point - coordinate);
            w_u32(frame + 0x18, dx);
            point = r_s16(position + 2);
            coordinate = r_s16(node - 2);
            uint32 dy = (uint32)(point - coordinate);
            w_u32(frame + 0x1c, dy);
            coordinate = r_s16(node);
            point = r_s16(position + 4);
            uint32 dz = (uint32)(point - coordinate);
            w_u32(frame + 0x20, dz);
            uint32 distance = dx * dx + dy * dy + dz * dz;
            if ((sint32)distance < (sint32)best)
            {
                best = distance;
                closest = (uint32)index;
            }
            count = r_s16(channel + 4);
            ++index;
            node += 0x20;
        } while (index < count);
    }
    w_u32(frame + 0x30, best);
    uint32 neighbor;
    if (closest == 0)
        neighbor = 1;
    else
    {
        count = r_s16(channel + 4);
        if (closest == (uint32)count - 1)
            neighbor = (uint32)count - 2;
        else
        {
            uint32 distances[2];
            for (uint32 side = 0; side < 2; ++side)
            {
                table = r_u32(channel);
                uint32 node = table + (closest << 5) + (side == 0 ? uint32(0xffffffe0) : 0x20);
                uint32 square[3];
                for (uint32 axis = 0; axis < 3; ++axis)
                {
                    sint32 point;
                    sint32 coordinate;
                    if (axis == 0 || (side == 0 && axis == 1))
                    {
                        point = r_s16(position + axis * 2);
                        coordinate = r_s16(node + 2 + axis * 2);
                    }
                    else
                    {
                        coordinate = r_s16(node + 2 + axis * 2);
                        point = r_s16(position + axis * 2);
                    }
                    uint32 difference = (uint32)(point - coordinate);
                    square[axis] = difference * difference;
                    w_u32(frame + 0x18 + axis * 4, difference);
                }
                distances[side] = square[0] + square[1] + square[2];
            }
            neighbor = (sint32)distances[1] < (sint32)distances[0] ? closest + 1 : closest - 1;
        }
    }
    if ((sint32)neighbor < (sint32)closest)
    {
        w_u16(frame + 0x28, neighbor);
        w_u32(frame + 0x34, 1);
    }
    else
    {
        w_u16(frame + 0x28, closest);
        w_u32(frame + 0x34, 0);
    }
    uint32 original_state = r_u32(frame + 0x28);
    w_u32(frame + 0x28, original_state & 0x1ffff);
    sint32 segment = r_s16(frame + 0x28);
    table = r_u32(channel);
    sint32 samples = r_s16(table + ((uint32)segment << 5));
    w_u32(frame + 0x28, original_state & 0xffff);
    uint32 best_sample = 0;
    for (sint32 sample = 0; sample < samples; ++sample)
    {
        G3Function80041008(channel, frame + 0x28, 0x800a2140, frame);
        uint32 current = r_u32(frame + 0x28);
        w_u32(frame + 0x28, (current & 0x1ffff) | (((current >> 17) + 1) << 17));
        sint32 point = r_s16(position);
        sint32 coordinate = r_s16(0x800a2140);
        uint32 dx = (uint32)(point - coordinate);
        w_u32(frame + 0x18, dx);
        point = r_s16(position + 2);
        coordinate = r_s16(0x800a2142);
        uint32 dy = (uint32)(point - coordinate);
        coordinate = r_s16(0x800a2144);
        w_u32(frame + 0x1c, dy);
        point = r_s16(position + 4);
        uint32 dz = (uint32)(point - coordinate);
        w_u32(frame + 0x20, dz);
        uint32 distance = dx * dx + dy * dy + dz * dz;
        if ((sint32)distance < (sint32)best)
        {
            uint32 words[2];
            for (uint32 word = 0; word < 2; ++word)
            {
                uint32 source = 0x800a2140 + word * 4;
                uint32 high_address = source + 3;
                uint32 high_lane = high_address & 3;
                uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                uint32 low_lane = source & 3;
                uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
                words[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
            }
            for (uint32 word = 0; word < 2; ++word)
            {
                uint32 destination = frame + 0x10 + word * 4;
                uint32 high_address = destination + 3;
                uint32 high_lane = high_address & 3;
                w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                uint32 low_lane = destination & 3;
                w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15 << low_lane) & 15);
            }
            best = distance;
            best_sample = (uint32)sample;
        }
    }
    uint32 before = r_u32(frame + 0x34);
    bool use_neighbor = false;
    bool include_sample = true;
    if (before != 0)
    {
        uint32 first_distance = r_u32(frame + 0x30);
        use_neighbor = (sint32)best < (sint32)first_distance;
        include_sample = use_neighbor;
    }
    uint32 flags = r_u16(state + 2);
    w_u16(state, use_neighbor ? neighbor : closest);
    flags &= 1;
    if (include_sample)
        flags |= best_sample << 1;
    w_u16(state + 2, flags);
    flags = r_u16(state + 2);
    w_u16(state + 2, flags & 0xfffe);
    G3Function80041008(channel, state, 0x800a2140, frame);
    return 0x800a2140;
}

uint32 G3Function80040A40(uint32 channel, uint32 position, uint32 output_state, uint32 output_position, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80040a40, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 nearest = G3Function80040280(channel, position, frame + 0x20, output_state, frame);
    uint32 words[2];
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 source = nearest + word * 4;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
        words[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 destination = frame + 0x18 + word * 4;
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        uint32 low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15 << low_lane) & 15);
    }
    w_u32(frame + 0x10, output_state);
    w_u32(frame + 0x14, output_position);
    return G3Function80040654(channel, position, frame + 0x18, frame + 0x20, frame);
}

uint32 G3Function80040654(uint32 channel, uint32 position, uint32 initial_position, uint32 initial_state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80040654, "SLUS_008.06");
    uint32 frame = guest_stack - 0x68;
    {
        uint32 words[2];
        for (uint32 word = 0; word < 2; ++word)
        {
            uint32 source = initial_position + word * 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low_lane = source & 3;
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
            words[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 word = 0; word < 2; ++word)
        {
            uint32 destination = frame + 0x10 + word * 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            uint32 low_lane = destination & 3;
            w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15 << low_lane) & 15);
        }
    }
    {
        uint32 words[2];
        for (uint32 word = 0; word < 2; ++word)
        {
            uint32 source = frame + 0x10 + word * 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low_lane = source & 3;
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
            words[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 word = 0; word < 2; ++word)
        {
            uint32 destination = frame + 0x20 + word * 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            uint32 low_lane = destination & 3;
            w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15 << low_lane) & 15);
        }
    }
    {
        uint32 words[1];
        for (uint32 word = 0; word < 1; ++word)
        {
            uint32 source = initial_state + word * 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low_lane = source & 3;
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
            words[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 word = 0; word < 1; ++word)
        {
            uint32 destination = frame + 0x38 + word * 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            uint32 low_lane = destination & 3;
            w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15 << low_lane) & 15);
        }
    }
    {
        uint32 words[1];
        for (uint32 word = 0; word < 1; ++word)
        {
            uint32 source = frame + 0x38 + word * 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low_lane = source & 3;
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
            words[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 word = 0; word < 1; ++word)
        {
            uint32 destination = frame + 0x40 + word * 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            uint32 low_lane = destination & 3;
            w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15 << low_lane) & 15);
        }
    }
    uint32 start_x = r_u16(frame + 0x10);
    uint32 point_x = r_u16(position);
    uint32 dx = start_x - point_x;
    uint32 output_state = r_u32(guest_stack + 0x10);
    uint32 output_position = r_u32(guest_stack + 0x14);
    w_u16(frame + 0x28, dx);
    uint32 start_y = r_u16(frame + 0x12);
    uint32 point_y = r_u16(position + 2);
    uint32 dy = start_y - point_y;
    w_u16(frame + 0x2c, 0);
    uint32 square_x = (uint32)(sint32)(sint16)dx;
    uint32 square_y = (uint32)(sint32)(sint16)dy;
    uint32 distance = square_x * square_x + square_y * square_y;
    w_u16(frame + 0x2a, dy);
    uint32 result = 1;
    uint32 vector_source = frame + 0x20;
    uint32 state_source = frame + 0x38;
    if ((sint32)distance >= 26)
    {
        result = 0;
        if (G3Function80041230(channel, frame + 0x40, frame) != 0)
        {
            G3Function80041008(channel, frame + 0x40, frame + 0x18, frame);
            uint32 next_x = r_u16(frame + 0x18);
            point_x = r_u16(position);
            uint32 next_y = r_u16(frame + 0x1a);
            dx = next_x - point_x;
            w_u16(frame + 0x30, dx);
            point_y = r_u16(position + 2);
            sint32 old_x = r_s16(frame + 0x28);
            dy = next_y - point_y;
            sint32 signed_y = (sint16)dy;
            sint32 first_cross = old_x * signed_y;
            sint32 old_y = r_s16(frame + 0x2a);
            sint32 signed_x = (sint16)dx;
            sint32 second_cross = signed_x * old_y;
            square_x = (uint32)signed_x;
            square_y = (uint32)signed_y;
            w_u16(frame + 0x34, 0);
            w_u16(frame + 0x32, dy);
            {
                uint32 words[2];
                for (uint32 word = 0; word < 2; ++word)
                {
                    uint32 source = frame + 0x18 + word * 4;
                    uint32 high_address = source + 3;
                    uint32 high_lane = high_address & 3;
                    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                    uint32 low_lane = source & 3;
                    uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
                    words[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
                }
                for (uint32 word = 0; word < 2; ++word)
                {
                    uint32 destination = frame + 0x10 + word * 4;
                    uint32 high_address = destination + 3;
                    uint32 high_lane = high_address & 3;
                    w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                    uint32 low_lane = destination & 3;
                    w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15 << low_lane) & 15);
                }
            }
            {
                uint32 words[2];
                for (uint32 word = 0; word < 2; ++word)
                {
                    uint32 source = frame + 0x30 + word * 4;
                    uint32 high_address = source + 3;
                    uint32 high_lane = high_address & 3;
                    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                    uint32 low_lane = source & 3;
                    uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
                    words[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
                }
                for (uint32 word = 0; word < 2; ++word)
                {
                    uint32 destination = frame + 0x28 + word * 4;
                    uint32 high_address = destination + 3;
                    uint32 high_lane = high_address & 3;
                    w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                    uint32 low_lane = destination & 3;
                    w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15 << low_lane) & 15);
                }
            }
            uint32 side = second_cross < first_cross;
            distance = square_x * square_x + square_y * square_y;
            for (;;)
            {
                sint32 current_segment = r_s16(frame + 0x40);
                sint32 original_segment = r_s16(frame + 0x38);
                if (current_segment == original_segment)
                {
                    uint32 current_flags = r_u16(frame + 0x42);
                    uint32 original_flags = r_u16(frame + 0x3a);
                    if ((current_flags & 0xfffe) == (original_flags & 0xfffe))
                        break;
                }
                if (G3Function80041230(channel, frame + 0x40, frame) == 0)
                    break;
                G3Function80041008(channel, frame + 0x40, frame + 0x18, frame);
                next_x = r_u16(frame + 0x18);
                point_x = r_u16(position);
                next_y = r_u16(frame + 0x1a);
                dx = next_x - point_x;
                w_u16(frame + 0x30, dx);
                point_y = r_u16(position + 2);
                old_x = r_s16(frame + 0x28);
                dy = next_y - point_y;
                signed_y = (sint16)dy;
                first_cross = old_x * signed_y;
                old_y = r_s16(frame + 0x2a);
                signed_x = (sint16)dx;
                second_cross = signed_x * old_y;
                square_x = (uint32)signed_x;
                square_y = (uint32)signed_y;
                w_u16(frame + 0x32, dy);
                uint32 next_side = second_cross < first_cross;
                uint32 next_distance = square_x * square_x + square_y * square_y;
                if (next_side != side && (sint32)distance < (sint32)next_distance)
                {
                    result = 1;
                    vector_source = frame + 0x18;
                    state_source = frame + 0x40;
                    break;
                }
                distance = next_distance;
                {
                    uint32 words[2];
                    for (uint32 word = 0; word < 2; ++word)
                    {
                        uint32 source = frame + 0x18 + word * 4;
                        uint32 high_address = source + 3;
                        uint32 high_lane = high_address & 3;
                        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                        uint32 low_lane = source & 3;
                        uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
                        words[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
                    }
                    for (uint32 word = 0; word < 2; ++word)
                    {
                        uint32 destination = frame + 0x10 + word * 4;
                        uint32 high_address = destination + 3;
                        uint32 high_lane = high_address & 3;
                        w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                        uint32 low_lane = destination & 3;
                        w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15 << low_lane) & 15);
                    }
                }
                {
                    uint32 words[2];
                    for (uint32 word = 0; word < 2; ++word)
                    {
                        uint32 source = frame + 0x30 + word * 4;
                        uint32 high_address = source + 3;
                        uint32 high_lane = high_address & 3;
                        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                        uint32 low_lane = source & 3;
                        uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
                        words[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
                    }
                    for (uint32 word = 0; word < 2; ++word)
                    {
                        uint32 destination = frame + 0x28 + word * 4;
                        uint32 high_address = destination + 3;
                        uint32 high_lane = high_address & 3;
                        w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                        uint32 low_lane = destination & 3;
                        w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15 << low_lane) & 15);
                    }
                }
                side = next_side;
            }
        }
    }
    if (output_position != 0)
    {
        uint32 words[2];
        for (uint32 word = 0; word < 2; ++word)
        {
            uint32 source = vector_source + word * 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low_lane = source & 3;
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
            words[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 word = 0; word < 2; ++word)
        {
            uint32 destination = output_position + word * 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            uint32 low_lane = destination & 3;
            w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15 << low_lane) & 15);
        }
    }
    if (output_state != 0)
    {
        uint32 words[1];
        for (uint32 word = 0; word < 1; ++word)
        {
            uint32 source = state_source + word * 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low_lane = source & 3;
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
            words[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 word = 0; word < 1; ++word)
        {
            uint32 destination = output_state + word * 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            uint32 low_lane = destination & 3;
            w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15 << low_lane) & 15);
        }
    }
    return result;
}

void G3Function80042ACC(uint32 owner, uint32 game, uint32 order_table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80042acc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    w_u32(guest_stack, owner);
    uint32 context = r_u32(game + 0x68);
    uint32 effect = r_u32(owner + 0x11c14);
    uint32 camera = r_u32(game + 0x4c);
    uint32 packet = r_u32(context + 4);
    uint32 view = r_u32(camera + 0x78);
    while (effect != 0)
    {
        uint32 limit = r_u32(context + 8);
        if (packet + 0x28 >= limit)
            break;
        uint32 flags = r_u32(effect + 0xc);
        uint32 next = r_u32(effect + 4);
        if ((flags & 0x10) == 0)
        {
            if ((flags & 0x20) != 0)
            {
                uint32 pool = r_u32(guest_stack);
                G3Function80041DE0(effect, pool, frame);
                if ((r_u32(effect + 0xc) & 0x10) != 0)
                {
                    effect = next;
                    continue;
                }
            }
            do
            {
                for (uint32 axis = 0; axis < 3; ++axis)
                {
                    uint32 storage = r_u32(effect + 0x14);
                    uint32 position = (uint32)r_s16(effect + 0x1c + axis * 2);
                    w_u32(storage + 0x20 + axis * 4, position);
                }
                uint32 first = r_u32(view);
                uint32 second = r_u32(view + 4);
                xport_gte_write_control(0, first);
                xport_gte_write_control(1, second);
                first = r_u32(view + 8);
                second = r_u32(view + 0xc);
                uint32 third = r_u32(view + 0x10);
                xport_gte_write_control(2, first);
                xport_gte_write_control(3, second);
                xport_gte_write_control(4, third);
                for (uint32 column = 0; column < 3; ++column)
                {
                    uint32 storage = r_u32(effect + 0x14) + 0xc + column * 2;
                    first = r_u16(storage);
                    second = r_u16(storage + 6);
                    third = r_u16(storage + 0xc);
                    xport_gte_write_data(9, first);
                    xport_gte_write_data(10, second);
                    xport_gte_write_data(11, third);
                    xport_gte_execute(0x49e012);
                    first = xport_gte_read_data(9);
                    second = xport_gte_read_data(10);
                    third = xport_gte_read_data(11);
                    w_u16(0x1f800000 + column * 2, first);
                    w_u16(0x1f800006 + column * 2, second);
                    w_u16(0x1f80000c + column * 2, third);
                }
                first = r_u32(view + 0x14);
                second = r_u32(view + 0x18);
                xport_gte_write_control(5, first);
                third = r_u32(view + 0x1c);
                xport_gte_write_control(6, second);
                xport_gte_write_control(7, third);
                uint32 storage = r_u32(effect + 0x14) + 0x20;
                second = r_u16(storage + 4);
                first = r_u16(storage);
                xport_gte_write_data(0, first | (second << 16));
                xport_gte_write_data(1, r_u32(storage + 8));
                xport_gte_execute(0x480012);
                w_u32(0x1f800014, xport_gte_read_data(25));
                w_u32(0x1f800018, xport_gte_read_data(26));
                w_u32(0x1f80001c, xport_gte_read_data(27));
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
                first = r_u32(0x1f800014);
                second = r_u32(0x1f800018);
                xport_gte_write_control(5, first);
                third = r_u32(0x1f80001c);
                xport_gte_write_control(6, second);
                xport_gte_write_control(7, third);
                flags = r_u32(effect + 0xc);
                uint32 vertices = effect + 0x24;
                if ((flags & 0x12000) != 0)
                {
                    uint32 scale_offset = (flags & 0x2000) != 0 ? 0x26 : 0x28;
                    for (uint32 vertex = 0; vertex < 3; ++vertex)
                    {
                        sint32 coordinate = r_s16(effect + 0x24 + vertex * 8);
                        sint32 scale = r_s16(effect + scale_offset);
                        w_u16(frame + 0x10 + vertex * 8, (uint32)((coordinate * scale) >> 12));
                    }
                    if ((flags & 0x2000) != 0)
                    {
                        for (uint32 vertex = 0; vertex < 2; ++vertex)
                        {
                            sint32 coordinate = r_s16(effect + 0x28 + vertex * 8);
                            sint32 scale = r_s16(effect + 0x36);
                            w_u16(frame + 0x14 + vertex * 8, (uint32)((coordinate * scale) >> 12));
                        }
                        sint32 coordinate = r_s16(effect + 0x38);
                        sint32 scale = r_s16(effect + 0x36);
                        sint32 product = coordinate * scale;
                        w_u16(frame + 0x12, 0);
                        w_u16(frame + 0x1a, 0);
                        w_u16(frame + 0x22, 0);
                        w_u16(frame + 0x24, (uint32)(product >> 12));
                    }
                    else
                    {
                        for (uint32 vertex = 0; vertex < 2; ++vertex)
                        {
                            sint32 coordinate = r_s16(effect + 0x26 + vertex * 8);
                            sint32 scale = r_s16(effect + 0x38);
                            w_u16(frame + 0x12 + vertex * 8, (uint32)((coordinate * scale) >> 12));
                        }
                        sint32 coordinate = r_s16(effect + 0x36);
                        sint32 scale = r_s16(effect + 0x38);
                        sint32 product = coordinate * scale;
                        w_u16(frame + 0x14, 0);
                        w_u16(frame + 0x1c, 0);
                        w_u16(frame + 0x24, 0);
                        w_u16(frame + 0x22, (uint32)(product >> 12));
                    }
                    vertices = frame + 0x10;
                }
                for (uint32 vertex = 0; vertex < 3; ++vertex)
                {
                    xport_gte_write_data(vertex * 2, r_u32(vertices + vertex * 8));
                    xport_gte_write_data(vertex * 2 + 1, r_u32(vertices + vertex * 8 + 4));
                }
                xport_gte_execute(0x280030);
                flags = r_u32(effect + 0xc);
                uint32 packet_size;
                uint32 tag;
                uint32 link_depth;
                if ((flags & 1) != 0)
                {
                    uint32 texture = effect + 0x60;
                    if ((flags & 0x804) == 0)
                        texture = r_u32(effect + 8);
                    sint32 depth;
                    if ((r_u32(effect + 0xc) & 0x4000) != 0)
                    {
                        w_u32(frame + 0x28, xport_gte_read_data(17));
                        w_u32(frame + 0x2c, xport_gte_read_data(18));
                        w_u32(frame + 0x30, xport_gte_read_data(19));
                        sint32 depth0 = r_s32(frame + 0x28);
                        sint32 depth1 = r_s32(frame + 0x2c);
                        uint32 current_game = r_u32(0x80097048);
                        uint32 shift = r_u32(current_game + 0x2c);
                        sint32 depth2 = r_s32(frame + 0x30);
                        sint32 maximum = depth1 < depth0 ? depth0 : depth1;
                        depth = (depth2 < maximum ? maximum : depth2) >> (shift & 31);
                    }
                    else
                    {
                        w_u32(frame + 0x34, xport_gte_read_data(19));
                        uint32 current_game = r_u32(0x80097048);
                        depth = r_s32(frame + 0x34);
                        uint32 shift = r_u32(current_game + 0x2c);
                        depth >>= shift & 31;
                    }
                    w_u32(frame + 0x34, (uint32)depth);
                    if ((r_u32(effect + 0xc) & 0x8000) != 0)
                        w_u32(frame + 0x34, r_u32(frame + 0x34) - 0x14);
                    depth = r_s32(frame + 0x34);
                    if (depth <= 0)
                        break;
                    uint32 current_game = r_u32(0x80097048);
                    if (depth >= r_s32(current_game + 0x28))
                        break;
                    if ((r_u32(effect + 0xc) & 8) != 0)
                    {
                        w_u32(packet + 8, xport_gte_read_data(12));
                        w_u32(packet + 0x10, xport_gte_read_data(13));
                        w_u32(packet + 0x18, xport_gte_read_data(14));
                        flags = r_u32(effect + 0xc);
                        uint32 fourth = effect + 0x3c;
                        if ((flags & 0x12000) != 0)
                        {
                            sint32 coordinate = r_s16(effect + 0x3c);
                            if ((flags & 0x2000) != 0)
                            {
                                sint32 scale = r_s16(effect + 0x26);
                                w_u16(frame + 0x10, (uint32)((coordinate * scale) >> 12));
                                coordinate = r_s16(effect + 0x40);
                                scale = r_s16(effect + 0x36);
                                sint32 product = coordinate * scale;
                                w_u16(frame + 0x12, 0);
                                w_u16(frame + 0x14, (uint32)(product >> 12));
                            }
                            else
                            {
                                sint32 scale = r_s16(effect + 0x28);
                                sint32 product = coordinate * scale;
                                w_u16(frame + 0x14, 0);
                                w_u16(frame + 0x10, (uint32)(product >> 12));
                                coordinate = r_s16(effect + 0x3e);
                                scale = r_s16(effect + 0x38);
                                w_u16(frame + 0x12, (uint32)((coordinate * scale) >> 12));
                            }
                            fourth = frame + 0x10;
                        }
                        xport_gte_write_data(0, r_u32(fourth));
                        xport_gte_write_data(1, r_u32(fourth + 4));
                        xport_gte_execute(0x180001);
                        uint32 u0 = r_u8(texture);
                        uint32 u1 = r_u8(texture + 4);
                        uint32 min_u = r_u8(texture + 8);
                        if (u0 < u1)
                        {
                            if (u0 < min_u)
                                min_u = u0;
                        }
                        else
                        {
                            if (u1 < min_u)
                                min_u = u1;
                            u0 = r_u8(texture);
                            u1 = r_u8(texture + 4);
                        }
                        uint32 max_u = r_u8(texture + 8);
                        uint32 larger_u = u1 < u0 ? u0 : u1;
                        if (max_u < larger_u)
                            max_u = larger_u;
                        uint32 v0 = r_u8(texture + 1);
                        uint32 v1 = r_u8(texture + 5);
                        uint32 min_v = r_u8(texture + 9);
                        uint32 smaller_v = v0 < v1 ? v0 : v1;
                        if (smaller_v < min_v)
                            min_v = smaller_v;
                        v0 = r_u8(texture + 1);
                        v1 = r_u8(texture + 5);
                        uint32 max_v = r_u8(texture + 9);
                        uint32 larger_v = v1 < v0 ? v0 : v1;
                        if (max_v < larger_v)
                            max_v = larger_v;
                        w_u8(packet + 0xc, min_u);
                        w_u8(packet + 0xd, min_v);
                        w_u8(packet + 0x14, max_u);
                        w_u8(packet + 0x15, min_v);
                        w_u8(packet + 0x1c, min_u);
                        w_u8(packet + 0x1d, max_v);
                        w_u8(packet + 0x24, max_u);
                        w_u8(packet + 0x25, max_v);
                        w_u16(packet + 0xe, r_u16(texture + 2));
                        w_u16(packet + 0x16, r_u16(texture + 6));
                        w_u32(packet + 0x20, xport_gte_read_data(14));
                        if ((r_u32(effect + 0xc) & 0x1000) != 0)
                        {
                            uint32 left0 = r_u8(packet + 0xc);
                            uint32 right0 = r_u8(packet + 0x14);
                            uint32 left1 = r_u8(packet + 0x1c);
                            left0 ^= right0;
                            uint32 right1 = r_u8(packet + 0x24);
                            w_u8(packet + 0xc, left0);
                            left0 ^= right0;
                            left1 ^= right1;
                            w_u8(packet + 0x1c, left1);
                            left1 ^= right1;
                            right0 = r_u8(packet + 0xc);
                            right1 = r_u8(packet + 0x1c);
                            w_u8(packet + 0x14, left0);
                            w_u8(packet + 0x24, left1);
                            w_u8(packet + 0xc, right0 ^ left0);
                            w_u8(packet + 0x1c, right1 ^ left1);
                        }
                        w_u32(packet + 4, r_u32(effect + 0x18));
                        link_depth = r_u32(frame + 0x34);
                        packet_size = 0x28;
                        tag = 0x09000000;
                    }
                    else
                    {
                        w_u32(packet + 8, xport_gte_read_data(12));
                        w_u32(packet + 0x10, xport_gte_read_data(13));
                        w_u32(packet + 0x18, xport_gte_read_data(14));
                        w_u32(packet + 0xc, r_u32(texture));
                        w_u32(packet + 0x14, r_u32(texture + 4));
                        w_u32(packet + 0x1c, r_u32(texture + 8));
                        if ((r_u32(effect + 0xc) & 0x1000) != 0)
                        {
                            uint32 u0 = r_u8(texture);
                            uint32 u1 = r_u8(texture + 4);
                            uint32 min_u;
                            if (u0 < u1 && u0 < r_u8(texture + 8))
                                min_u = r_u8(texture);
                            else
                            {
                                if (u0 >= u1 && u1 < r_u8(texture + 8))
                                    min_u = r_u8(texture + 4);
                                else
                                    min_u = r_u8(texture + 8);
                                u0 = r_u8(texture);
                                u1 = r_u8(texture + 4);
                            }
                            uint32 max_u;
                            if (u1 < u0)
                            {
                                if (r_u8(texture + 8) < u0)
                                    max_u = r_u8(texture);
                                else
                                    max_u = r_u8(texture + 8);
                            }
                            else
                            {
                                if (r_u8(texture + 8) < u1)
                                    max_u = r_u8(texture + 4);
                                else
                                    max_u = r_u8(texture + 8);
                            }
                            uint32 value = r_u8(texture) == min_u ? max_u : min_u;
                            w_u8(packet + 0xc, value);
                            value = r_u8(texture + 4) == min_u ? max_u : min_u;
                            w_u8(packet + 0x14, value);
                            value = r_u8(texture + 8) == min_u ? max_u : min_u;
                            w_u8(packet + 0x1c, value);
                        }
                        w_u32(packet + 4, r_u32(effect + 0x18));
                        link_depth = r_u32(frame + 0x34);
                        packet_size = 0x20;
                        tag = 0x07000000;
                    }
                }
                else
                {
                    w_u32(frame + 0x34, xport_gte_read_data(19));
                    uint32 current_game = r_u32(0x80097048);
                    sint32 depth = r_s32(frame + 0x34);
                    uint32 shift = r_u32(current_game + 0x2c);
                    w_u32(frame + 0x34, (uint32)(depth >> (shift & 31)));
                    depth = r_s32(frame + 0x34);
                    if (depth <= 0)
                        break;
                    current_game = r_u32(0x80097048);
                    if (depth >= r_s32(current_game + 0x28))
                        break;
                    if ((r_u32(effect + 0xc) & 0x20000) != 0)
                    {
                        w_u32(packet + 8, xport_gte_read_data(12));
                        w_u32(packet + 0xc, xport_gte_read_data(13));
                        w_u32(packet + 0x10, xport_gte_read_data(14));
                        w_u32(packet + 4, r_u32(effect + 0x18));
                        w_u8(packet + 7, 0x48);
                        link_depth = r_u32(frame + 0x34);
                        w_u32(packet + 0x14, 0x55555555);
                        packet_size = 0x18;
                        tag = 0x05000000;
                    }
                    else
                    {
                        w_u32(packet + 4, r_u32(effect + 0x18));
                        if ((r_u8(packet + 7) & 0x30) == 0x30)
                        {
                            w_u32(packet + 8, xport_gte_read_data(12));
                            w_u32(packet + 0x10, xport_gte_read_data(13));
                            w_u32(packet + 0x18, xport_gte_read_data(14));
                            packet_size = 0x1c;
                            tag = 0x06000000;
                        }
                        else
                        {
                            w_u32(packet + 8, xport_gte_read_data(12));
                            w_u32(packet + 0xc, xport_gte_read_data(13));
                            w_u32(packet + 0x10, xport_gte_read_data(14));
                            packet_size = 0x14;
                            tag = 0x04000000;
                        }
                        link_depth = r_u32(frame + 0x34);
                    }
                }
                uint32 entry = order_table + (link_depth << 2);
                uint32 previous = r_u32(entry);
                w_u32(packet, (previous & 0xffffff) | tag);
                w_u32(entry, packet & 0xffffff);
                uint32 count = r_u32(context);
                packet += packet_size;
                w_u32(context, count + 1);
            } while (false);
        }
        flags = r_u32(effect + 0xc);
        w_u32(effect + 0xc, flags & 0xfffedfff);
        effect = next;
    }
    w_u32(context + 4, packet);
}
