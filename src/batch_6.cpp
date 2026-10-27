#include "xport.h"
#include "psx.h"
#include "game_functions.h"

uint32 G3Function80056894(uint32 object, uint32 game, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80056894, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 enabled = r_u32(0x800a2488);
    if (enabled == 0)
        return G3Function8005664C(payload, 0x800a2494, object, game, frame);
    G3Function80056A74(0x800a2494, frame);
    uint32 source = 0x800a248c;
    uint32 destination = payload;
    uint32 upper = r_u32((source + 3) & ~uint32(3));
    uint32 lower = r_u32(source & ~uint32(3));
    uint32 shift = (source & 3) << 3;
    uint32 value = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
    uint32 tail = (uint32)(sint32)r_s16(source + 4);
    uint32 high_address = destination + 3;
    uint32 high_byte = high_address & 3;
    uint32 preserve = high_byte == 3 ? 0 : uint32(0xffffffff) << ((high_byte + 1) << 3);
    uint32 previous = r_u32(high_address & ~uint32(3));
    w_u32(high_address & ~uint32(3), (previous & preserve) | (value >> ((3 - high_byte) << 3)));
    uint32 low_byte = destination & 3;
    preserve = low_byte == 0 ? 0 : uint32(0xffffffff) >> ((4 - low_byte) << 3);
    previous = r_u32(destination & ~uint32(3));
    w_u32(destination & ~uint32(3), (previous & preserve) | (value << (low_byte << 3)));
    w_u16(destination + 4, tail);
    uint32 result = G3Function8005A1D8(payload + 12, 0x800a2494, payload, frame);
    w_u32(payload + 20, result);
    return result;
}

uint32 G3Function8005A1D8(uint32 output, uint32 first, uint32 second, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005a1d8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    G3Function800144CC(output, first, second, frame);
    uint32 a = r_u16(first);
    uint32 b = r_u16(second);
    w_u16(frame + 0x10, a - b);
    a = r_u16(first + 2);
    b = r_u16(second + 2);
    w_u16(frame + 0x12, a - b);
    a = r_u16(first + 4);
    b = r_u16(second + 4);
    w_u16(frame + 0x14, a - b);
    return G3Function8002FA50(frame + 0x10, frame);
}

uint32 G3Function8002FA50(uint32 vector, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002fa50, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    sint32 x = r_s16(vector);
    uint32 xx = (uint32)(x * x);
    sint32 y = r_s16(vector + 2);
    uint32 yy = (uint32)(y * y);
    sint32 z = r_s16(vector + 4);
    uint32 zz = (uint32)(z * z);
    uint32 result = G3Function8002F448((xx + yy + zz) << 4, 4, frame);
    return (uint32)((sint32)(result + 8) >> 4);
}

uint32 G3Function8002F448(uint32 value, uint32 scale, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002f448, "SLUS_008.06");
    if (value == 0)
        return 0;
    uint32 leading = 31;
    uint32 mask = 0x80000000;
    if ((sint32)value >= 0)
    {
        do
        {
            mask = (uint32)((sint32)mask >> 1);
            --leading;
        } while ((mask & value) == 0);
    }
    uint32 exponent = leading >> 1;
    uint32 half_scale = (uint32)((sint32)(scale + (scale >> 31)) >> 1);
    uint32 root = uint32(1) << ((exponent + half_scale) & 31);
    uint32 place = root;
    uint32 trial = uint32(1) << ((exponent << 1) & 31);
    uint32 increment = trial;
    uint32 remaining = exponent - 1;
    uint32 remainder = value - trial;
    if (remaining != 0xffffffff)
    {
        increment = (uint32)((sint32)increment >> 2);
        do
        {
            uint32 candidate = remainder - (trial + increment);
            place = (uint32)((sint32)place >> 1);
            trial = (uint32)((sint32)trial >> 1);
            if ((sint32)candidate >= 0)
            {
                remainder = candidate;
                trial += increment;
                root |= place;
            }
            --remaining;
            increment = (uint32)((sint32)increment >> 2);
        } while (remaining != 0xffffffff);
    }
    remainder <<= scale & 31;
    trial <<= scale & 31;
    place = (uint32)((sint32)place >> 1);
    increment = uint32(1) << (scale & 31);
    if (place == 0)
        return root;
    increment = (uint32)((sint32)increment >> 2);
    do
    {
        uint32 candidate = remainder - (trial + increment);
        trial = (uint32)((sint32)trial >> 1);
        if ((sint32)candidate >= 0)
        {
            remainder = candidate;
            trial += increment;
            root |= place;
        }
        place = (uint32)((sint32)place >> 1);
        increment = (uint32)((sint32)increment >> 2);
    } while (place != 0);
    return root;
}

uint32 G3Function800564A4(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800564a4, "SLUS_008.06");
    uint32 mode = r_u32(0x800a258c);
    uint32 frame = guest_stack - 0x38;
    bool eligible = mode == 0 || mode == 3;
    if (eligible)
    {
        uint32 delay = r_u32(0x8009ec90);
        eligible = (sint32)delay >= 2;
    }
    if (eligible)
    {
        uint32 blocked = r_u32(0x80096b70);
        eligible = blocked == 0;
    }
    if (!eligible)
        w_u32(0x800a2554, 0);
    else
    {
        uint32 flags = r_u32(0x800a2574);
        w_u32(0x800a2574, flags & 0xfffffffd);
        if (G3Function80056464() != 0)
        {
            uint32 timer = r_u32(0x800a2554);
            uint32 delay = r_u32(0x8009ec90);
            if ((sint32)timer < (sint32)delay)
                timer = delay;
            w_u32(0x800a2554, timer);
        }
        uint32 blocked = r_u32(0x80096b58);
        if (blocked != 0)
            w_u32(0x800a2554, 0);
        uint32 timer = r_u32(0x800a2554);
        if ((sint32)timer > 0)
        {
            --timer;
            w_u32(0x800a2554, timer);
            if (timer == 0)
            {
                mode = r_u32(0x800a258c);
                if (mode == 3)
                {
                    G3Function80013C94(object, 19, frame);
                    G3Function80055544(frame);
                    uint32 index = r_u32(0x80096a88);
                    uint32 target = r_u32(0x80099650 + (index << 3) + 4);
                    if (target == 0)
                        return 0;
                    uint32 current_game = r_u32(0x80097048);
                    return G3DispatchStateEvent(target, object, current_game, 1, 0, frame);
                }
                flags = r_u32(0x800a2574);
                w_u32(0x800a2574, flags | 2);
            }
        }
    }
    G3Function8005664C(frame + 0x10, 0x800a2494, object, game, frame);
    G3Function8005A248(object, frame + 0x10);
    uint32 flags = r_u32(0x8009ea44);
    uint32 result = flags & 0x80;
    if (result != 0)
        return G3Function800563A0(object, frame);
    return result;
}

uint32 G3Function80056464(void)
{
    FUNCTION_MARKER(0x80056464, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 flags = r_u32(game + 0x70);
    if ((flags & 14) != 0)
        return 1;
    uint32 blocked = r_u32(0x80096b44);
    return blocked != 0 ? 1 : 0;
}

uint32 G3Function80055544(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80055544, "SLUS_008.06");
    uint32 mode = r_u32(0x80096a88);
    if (mode != 19)
        return 19;
    G3Function80016F00();
    w_u32(0x800a2458, 1);
    return 1;
}

uint32 G3Function80016F00(void)
{
    FUNCTION_MARKER(0x80016f00, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 flags = r_u32(game + 0x6834);
    w_u32(game + 0x6834, flags | 0x90);
    return 1;
}

uint32 G3Function8005A248(uint32 object, uint32 packed)
{
    FUNCTION_MARKER(0x8005a248, "SLUS_008.06");
    {
        uint32 source = packed;
        uint32 destination = object;
        uint32 upper = r_u32((source + 3) & ~uint32(3));
        uint32 lower = r_u32(source & ~uint32(3));
        uint32 shift = (source & 3) << 3;
        uint32 value = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
        uint32 tail = (uint32)(sint32)r_s16(source + 4);
        uint32 high_address = destination + 3;
        uint32 high_byte = high_address & 3;
        uint32 preserve = high_byte == 3 ? 0 : uint32(0xffffffff) << ((high_byte + 1) << 3);
        uint32 previous = r_u32(high_address & ~uint32(3));
        w_u32(high_address & ~uint32(3), (previous & preserve) | (value >> ((3 - high_byte) << 3)));
        uint32 low_byte = destination & 3;
        preserve = low_byte == 0 ? 0 : uint32(0xffffffff) >> ((4 - low_byte) << 3);
        previous = r_u32(destination & ~uint32(3));
        w_u32(destination & ~uint32(3), (previous & preserve) | (value << (low_byte << 3)));
        w_u16(destination + 4, tail);
    }
    uint32 source = packed + 12;
    uint32 upper = r_u32((source + 3) & ~uint32(3));
    uint32 lower = r_u32(source & ~uint32(3));
    uint32 shift = (source & 3) << 3;
    uint32 first = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
    source = packed + 16;
    upper = r_u32((source + 3) & ~uint32(3));
    lower = r_u32(source & ~uint32(3));
    shift = (source & 3) << 3;
    uint32 second = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
    for (uint32 field = 0; field < 2; ++field)
    {
        uint32 destination = object + 0x10 + field * 4;
        uint32 value = field == 0 ? first : second;
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
    uint32 x = r_u16(object + 0x10);
    uint32 y = r_u16(object + 0x12);
    uint32 z = r_u16(object + 0x14);
    w_u16(object + 0x1c, z);
    uint32 result = (uint32)(sint32)(sint16)(z & 0xffff);
    w_u16(object + 8, x);
    w_u16(object + 10, y);
    w_u16(object + 12, z);
    w_u16(object + 0x18, x);
    w_u16(object + 0x1a, y);
    w_u32(0x80096a90, result);
    return result;
}

uint32 G3Function800563A0(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800563a0, "SLUS_008.06");
    uint32 flags = r_u32(0x8009ea44);
    uint32 frame = guest_stack - 0x38;
    if ((flags & 0x80) == 0)
        return 0;
    uint32 rotation = (uint32)(sint32)r_s16(object + 0x14);
    uint32 tilt = (uint32)(sint32)r_s16(object + 0x10);
    uint32 coordinate = (uint32)(sint32)r_s16(object + 2);
    uint32 origin = (uint32)(sint32)r_s16(0x80096a56);
    uint32 camera_x = (uint32)(sint32)r_s16(object);
    w_u32(frame + 0x10, coordinate - origin);
    coordinate = (uint32)(sint32)r_s16(object + 4);
    origin = (uint32)(sint32)r_s16(0x80096a58);
    w_u32(frame + 0x14, coordinate - origin);
    coordinate = (uint32)(sint32)r_s16(object + 4);
    uint32 player = r_u32(0x8009efc4);
    w_u32(frame + 0x18, coordinate);
    coordinate = (uint32)(sint32)r_s16(player + 0x58);
    origin = (uint32)(sint32)r_s16(player + 0x70);
    w_u32(frame + 0x1c, coordinate - origin);
    coordinate = (uint32)(sint32)r_s16(player + 0x5a);
    origin = (uint32)(sint32)r_s16(player + 0x72);
    uint32 origin_x = (uint32)(sint32)r_s16(0x80096a54);
    w_u32(frame + 0x20, coordinate - origin);
    coordinate = (uint32)(sint32)r_s16(player + 0x5c);
    origin = (uint32)(sint32)r_s16(player + 0x74);
    uint32 relative_x = camera_x - origin_x;
    w_u32(frame + 0x24, coordinate - origin);
    coordinate = (uint32)(sint32)r_s16(player + 0x5c);
    w_u32(frame + 0x28, coordinate);
    return G3PrintCameraDiagnostic(rotation, tilt, relative_x, frame);
}

uint32 G3Function80056A74(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80056a74, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 x = r_u16(state);
    uint32 y = r_u16(state + 2);
    uint32 z = r_u16(state + 4);
    uint32 target_x = r_u16(state + 12);
    uint32 target_y = r_u16(state + 14);
    uint32 target_z = r_u16(state + 16);
    w_u16(state + 6, x);
    w_u16(state + 8, y);
    w_u16(state + 10, z);
    w_u16(state + 18, target_x);
    w_u16(state + 20, target_y);
    w_u16(state + 22, target_z);
    {
        uint32 source = 0x80096b38;
        uint32 destination = frame + 0x10;
        uint32 upper = r_u32((source + 3) & ~uint32(3));
        uint32 lower = r_u32(source & ~uint32(3));
        uint32 shift = (source & 3) << 3;
        uint32 value = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
        uint32 tail = (uint32)(sint32)r_s16(source + 4);
        uint32 high_address = destination + 3;
        uint32 high_byte = high_address & 3;
        uint32 preserve = high_byte == 3 ? 0 : uint32(0xffffffff) << ((high_byte + 1) << 3);
        uint32 previous = r_u32(high_address & ~uint32(3));
        w_u32(high_address & ~uint32(3), (previous & preserve) | (value >> ((3 - high_byte) << 3)));
        uint32 low_byte = destination & 3;
        preserve = low_byte == 0 ? 0 : uint32(0xffffffff) >> ((4 - low_byte) << 3);
        previous = r_u32(destination & ~uint32(3));
        w_u32(destination & ~uint32(3), (previous & preserve) | (value << (low_byte << 3)));
        w_u16(destination + 4, tail);
    }
    G3Function80050CE4(frame + 0x10, frame);
    uint32 flags = r_u32(0x8009ea44);
    if ((flags & 0x8000) == 0)
        G3Function80050B94(frame + 0x18, frame);
    else
    {
        uint32 source = frame + 0x10;
        uint32 destination = frame + 0x18;
        uint32 upper = r_u32((source + 3) & ~uint32(3));
        uint32 lower = r_u32(source & ~uint32(3));
        uint32 shift = (source & 3) << 3;
        uint32 value = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
        uint32 tail = (uint32)(sint32)r_s16(source + 4);
        uint32 high_address = destination + 3;
        uint32 high_byte = high_address & 3;
        uint32 preserve = high_byte == 3 ? 0 : uint32(0xffffffff) << ((high_byte + 1) << 3);
        uint32 previous = r_u32(high_address & ~uint32(3));
        w_u32(high_address & ~uint32(3), (previous & preserve) | (value >> ((3 - high_byte) << 3)));
        uint32 low_byte = destination & 3;
        preserve = low_byte == 0 ? 0 : uint32(0xffffffff) >> ((4 - low_byte) << 3);
        previous = r_u32(destination & ~uint32(3));
        w_u32(destination & ~uint32(3), (previous & preserve) | (value << (low_byte << 3)));
        w_u16(destination + 4, tail);
    }
    if (G3Function80058B54() != 0)
    {
        uint32 height = r_u16(frame + 0x14);
        w_u16(frame + 0x1c, height);
    }
    {
        uint32 source = frame + 0x10;
        uint32 destination = state + 12;
        uint32 upper = r_u32((source + 3) & ~uint32(3));
        uint32 lower = r_u32(source & ~uint32(3));
        uint32 shift = (source & 3) << 3;
        uint32 value = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
        uint32 tail = (uint32)(sint32)r_s16(source + 4);
        uint32 high_address = destination + 3;
        uint32 high_byte = high_address & 3;
        uint32 preserve = high_byte == 3 ? 0 : uint32(0xffffffff) << ((high_byte + 1) << 3);
        uint32 previous = r_u32(high_address & ~uint32(3));
        w_u32(high_address & ~uint32(3), (previous & preserve) | (value >> ((3 - high_byte) << 3)));
        uint32 low_byte = destination & 3;
        preserve = low_byte == 0 ? 0 : uint32(0xffffffff) >> ((4 - low_byte) << 3);
        previous = r_u32(destination & ~uint32(3));
        w_u32(destination & ~uint32(3), (previous & preserve) | (value << (low_byte << 3)));
        w_u16(destination + 4, tail);
    }
    target_x = r_u16(state + 12);
    uint32 offset_x = r_u16(state + 0x30);
    uint32 offset_y = r_u16(state + 0x32);
    uint32 offset_z = r_u16(state + 0x34);
    w_u16(state + 12, target_x + offset_x);
    target_y = r_u16(state + 14);
    target_z = r_u16(state + 16);
    w_u16(state + 14, target_y + offset_y);
    w_u16(state + 16, target_z + offset_z);
    {
        uint32 source = frame + 0x18;
        uint32 destination = state;
        uint32 upper = r_u32((source + 3) & ~uint32(3));
        uint32 lower = r_u32(source & ~uint32(3));
        uint32 shift = (source & 3) << 3;
        uint32 value = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
        uint32 tail = (uint32)(sint32)r_s16(source + 4);
        uint32 high_address = destination + 3;
        uint32 high_byte = high_address & 3;
        uint32 preserve = high_byte == 3 ? 0 : uint32(0xffffffff) << ((high_byte + 1) << 3);
        uint32 previous = r_u32(high_address & ~uint32(3));
        w_u32(high_address & ~uint32(3), (previous & preserve) | (value >> ((3 - high_byte) << 3)));
        uint32 low_byte = destination & 3;
        preserve = low_byte == 0 ? 0 : uint32(0xffffffff) >> ((4 - low_byte) << 3);
        previous = r_u32(destination & ~uint32(3));
        w_u32(destination & ~uint32(3), (previous & preserve) | (value << (low_byte << 3)));
        w_u16(destination + 4, tail);
    }
    uint32 current = r_u16(state);
    uint32 previous = r_u16(state + 6);
    uint32 dx = current - previous;
    w_u16(frame + 0x20, dx);
    current = r_u16(state + 2);
    previous = r_u16(state + 8);
    uint32 dy = current - previous;
    w_u16(frame + 0x22, dy);
    current = r_u16(state + 4);
    previous = r_u16(state + 10);
    uint32 dz = current - previous;
    w_u16(frame + 0x24, dz);
    uint32 result = ((dx | dy | dz) & 0xffff) != 0 ? 1 : 0;
    w_u32(state + 0x2c, result);
    return result;
}

uint32 G3Function80058B54(void)
{
    FUNCTION_MARKER(0x80058b54, "SLUS_008.06");
    uint32 distance = r_u32(0x800a24f8);
    if ((sint32)distance < 1300)
        return 1;
    uint32 enabled = r_u32(0x800a2590);
    if (enabled != 0)
        return 1;
    uint32 state = r_u32(0x80096b6c);
    return state != 0 ? 1 : 0;
}

uint32 G3Function80050CE4(uint32 vector, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80050ce4, "SLUS_008.06");
    uint32 x = (uint32)(sint32)r_s16(vector);
    uint32 frame = guest_stack - 0x60;
    w_u32(frame + 0x30, x);
    uint32 y = (uint32)(sint32)r_s16(vector + 2);
    w_u32(frame + 0x34, y);
    uint32 z = (uint32)(sint32)r_s16(vector + 4);
    w_u32(frame + 0x30, x << 16);
    uint32 owner = r_u32(0x8009ea6c);
    w_u32(frame + 0x34, y << 16);
    w_u32(frame + 0x38, z);
    w_u32(frame + 0x38, z << 16);
    x = r_u32(frame + 0x30);
    y = r_u32(frame + 0x34);
    z = r_u32(frame + 0x38);
    w_u32(owner, x);
    w_u32(owner + 4, y);
    w_u32(owner + 8, z);
    uint32 current = r_u32(owner);
    uint32 previous = r_u32(owner + 12);
    uint32 dx = current - previous;
    uint32 doubled_x = dx << 1;
    w_u32(frame, dx);
    current = r_u32(owner + 4);
    previous = r_u32(owner + 0x10);
    uint32 dy = current - previous;
    uint32 doubled_y = dy << 1;
    w_u32(frame + 4, dy);
    current = r_u32(owner + 8);
    previous = r_u32(owner + 0x14);
    w_u32(frame + 0x10, doubled_x);
    uint32 dz = current - previous;
    uint32 doubled_z = dz << 1;
    w_u32(frame + 0x14, doubled_y);
    w_u32(frame + 8, dz);
    w_u32(frame + 0x18, doubled_z);
    uint32 correction_x = (uint32)((sint32)doubled_x / 5);
    uint32 correction_y = (uint32)((sint32)doubled_y / 5);
    w_u32(frame + 0x10, correction_x);
    w_u32(frame + 0x14, correction_y);
    uint32 correction_z = (uint32)((sint32)doubled_z / 5);
    w_u32(frame + 0x18, correction_z);
    uint32 value = r_u32(owner + 0x18);
    w_u32(frame + 0x20, value);
    value = r_u32(owner + 0x1c);
    w_u32(frame + 0x24, value);
    value = r_u32(owner + 0x20);
    w_u32(frame + 0x28, value);
    value = r_u32(owner + 0x18);
    w_u32(owner + 0x18, value + correction_x);
    value = r_u32(owner + 0x1c);
    uint32 correction = r_u32(frame + 0x14);
    w_u32(owner + 0x1c, value + correction);
    value = r_u32(owner + 0x20);
    correction = r_u32(frame + 0x18);
    w_u32(owner + 0x20, value + correction);
    value = r_u32(owner + 0x18);
    previous = r_u32(frame + 0x20);
    w_u32(owner + 0x18, value - previous);
    value = r_u32(owner + 0x1c);
    previous = r_u32(frame + 0x24);
    uint32 increment_x = r_u32(owner + 0x18);
    w_u32(owner + 0x1c, value - previous);
    value = r_u32(owner + 0x20);
    previous = r_u32(frame + 0x28);
    uint32 base_x = r_u32(owner + 12);
    uint32 prospective_x = base_x + increment_x;
    w_u32(owner + 0x20, value - previous);
    w_u32(frame + 0x40, prospective_x);
    uint32 base_y = r_u32(owner + 0x10);
    value = r_u32(owner + 0x1c);
    uint32 prospective_y = base_y + value;
    w_u32(frame + 0x44, prospective_y);
    uint32 base_z = r_u32(owner + 0x14);
    value = r_u32(owner + 0x20);
    uint32 prospective_z = base_z + value;
    w_u32(frame + 0x48, prospective_z);
    current = r_u32(owner);
    uint32 offset_x = prospective_x - current;
    w_u32(frame + 0x50, offset_x);
    current = r_u32(owner + 4);
    uint32 offset_y = prospective_y - current;
    w_u32(frame + 0x54, offset_y);
    current = r_u32(owner + 8);
    uint32 offset_z = prospective_z - current;
    w_u32(frame + 0x58, offset_z);
    if ((sint32)offset_x < -13107200)
        offset_x = 0xff380000;
    if ((sint32)offset_x > 13107200)
        offset_x = 0x00c80000;
    w_u32(frame + 0x50, offset_x);
    if ((sint32)offset_y < -13107200)
        offset_y = 0xff380000;
    if ((sint32)offset_y > 13107200)
        offset_y = 0x00c80000;
    w_u32(frame + 0x54, offset_y);
    if ((sint32)offset_z < -13107200)
        offset_z = 0xff380000;
    if ((sint32)offset_z > 13107200)
        offset_z = 0x00c80000;
    w_u32(frame + 0x58, offset_z);
    current = r_u32(owner);
    w_u32(owner + 12, current + offset_x);
    current = r_u32(owner + 4);
    offset_y = r_u32(frame + 0x54);
    w_u32(owner + 0x10, current + offset_y);
    current = r_u32(owner + 8);
    offset_z = r_u32(frame + 0x58);
    uint32 result = current + offset_z;
    w_u32(owner + 0x14, result);
    return result;
}

uint32 G3Function8005664C(uint32 output, uint32 state, uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005664c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 flags = r_u32(0x800a24d0);
    w_u32(0x800a24d0, flags & 0xfff3f7ff);
    G3Function80056A74(state, frame);
    G3Function80057284(output, state, object, frame);
    uint32 distance = G3Function8005A1D8(output + 12, state + 12, output, frame);
    uint32 heading = r_u16(output + 12);
    w_u32(output + 20, distance);
    w_u16(0x800a252a, heading);
    uint32 angle = (uint32)(sint32)r_s16(output + 12);
    G3Function800594EC(angle, frame);
    flags = r_u32(0x8009ea44);
    if ((flags & 0x800) == 0)
    {
        distance = G3Function8005A1D8(output + 12, state + 12, output, frame);
        w_u32(output + 20, distance);
        uint32 control = r_u32(0x800a24d0);
        heading = (control & 0x200) != 0 ? r_u16(output + 12) : r_u16(0x80096b6a);
        flags = r_u32(0x8009ea44);
        if ((flags & 0x1000) != 0)
        {
            uint32 degrees = r_u32(0x8009ec84);
            uint32 scaled = degrees << 12;
            heading += (uint32)((sint32)scaled / 360);
        }
        heading &= 0xfff;
        if (heading >= 0x800)
            heading |= 0xfffff000;
        uint32 tilt = (uint32)(sint32)r_s16(output + 16);
        uint32 value = G3Function8005A088(output, state + 12, heading, tilt, frame);
        w_u16(output + 12, value);
    }
    uint32 tilt = (uint32)(sint32)r_s16(output + 16);
    w_u32(0x800a24ec, tilt);
    uint32 a = r_u16(output);
    uint32 b = r_u16(state);
    uint32 dx = a - b;
    sint32 signed_x = (sint16)(dx & 0xffff);
    uint32 xx = (uint32)(signed_x * signed_x);
    w_u16(frame + 0x10, dx);
    a = r_u16(output + 2);
    b = r_u16(state + 2);
    uint32 dy = a - b;
    sint32 signed_y = (sint16)(dy & 0xffff);
    uint32 yy = (uint32)(signed_y * signed_y);
    w_u16(frame + 0x12, dy);
    b = r_u16(state + 4);
    a = r_u16(output + 4);
    w_u16(frame + 0x14, a - b);
    uint32 squared = xx + yy;
    uint32 result;
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
    w_u32(0x800a24f8, result);
    uint32 source = state;
    uint32 destination = output + 6;
    uint32 upper = r_u32((source + 3) & ~uint32(3));
    uint32 lower = r_u32(source & ~uint32(3));
    uint32 shift = (source & 3) << 3;
    uint32 value = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
    uint32 tail = (uint32)(sint32)r_s16(source + 4);
    uint32 high_address = destination + 3;
    uint32 high_byte = high_address & 3;
    uint32 preserve = high_byte == 3 ? 0 : uint32(0xffffffff) << ((high_byte + 1) << 3);
    uint32 previous = r_u32(high_address & ~uint32(3));
    w_u32(high_address & ~uint32(3), (previous & preserve) | (value >> ((3 - high_byte) << 3)));
    uint32 low_byte = destination & 3;
    preserve = low_byte == 0 ? 0 : uint32(0xffffffff) >> ((4 - low_byte) << 3);
    previous = r_u32(destination & ~uint32(3));
    w_u32(destination & ~uint32(3), (previous & preserve) | (value << (low_byte << 3)));
    w_u16(destination + 4, tail);
    return result;
}

uint32 G3Function8002F368(uint32 value, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002f368, "SLUS_008.06");
    if (value == 0)
        return 0;
    uint32 leading = 31;
    uint32 mask = 0x80000000;
    if ((sint32)value >= 0)
    {
        do
        {
            mask = (uint32)((sint32)mask >> 1);
            --leading;
        } while ((mask & value) == 0);
    }
    uint32 exponent = leading >> 1;
    uint32 root = uint32(1) << ((exponent + 6) & 31);
    uint32 place = root;
    uint32 trial = uint32(1) << ((exponent << 1) & 31);
    uint32 increment = trial;
    uint32 remaining = exponent - 1;
    uint32 remainder = value - trial;
    if (remaining != 0xffffffff)
    {
        increment = (uint32)((sint32)increment >> 2);
        do
        {
            uint32 candidate = remainder - (trial + increment);
            place = (uint32)((sint32)place >> 1);
            trial = (uint32)((sint32)trial >> 1);
            if ((sint32)candidate >= 0)
            {
                remainder = candidate;
                trial += increment;
                root |= place;
            }
            --remaining;
            increment = (uint32)((sint32)increment >> 2);
        } while (remaining != 0xffffffff);
    }
    remainder <<= 12;
    trial <<= 12;
    increment = 4096;
    for (;;)
    {
        place = (uint32)((sint32)place >> 1);
        increment = (uint32)((sint32)increment >> 2);
        if (place == 0)
            break;
        uint32 candidate = remainder - (trial + increment);
        trial = (uint32)((sint32)trial >> 1);
        if ((sint32)candidate >= 0)
        {
            remainder = candidate;
            trial += increment;
            root |= place;
        }
    }
    return root;
}

uint32 G3Function800594EC(uint32 angle, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800594ec, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    bool direct = G3Function80058B54() != 0;
    if (!direct)
    {
        uint32 offset = r_u32(0x800a2534);
        direct = offset != 0;
    }
    uint32 target;
    uint32 limit;
    if (direct)
    {
        target = angle;
        limit = 32;
    }
    else
    {
        uint32 flags = r_u32(0x800a24d0);
        if ((flags & 0x80000) == 0)
            return 0;
        uint32 height = (uint32)(sint32)r_s16(0x800a2524);
        uint32 distance = (uint32)(sint32)r_s16(0x800a24cc);
        target = G3Function8002F240(uint32(0) - height, distance, frame);
        limit = r_u16(0x8009ec58);
    }
    sint32 step = (sint16)(limit & 0xffff);
    if (step == 0)
        return 0;
    uint32 current = (uint32)(sint32)r_s16(0x800a2526);
    uint32 requested = (uint32)(sint32)(sint16)(target & 0xffff);
    uint32 difference = G3Function80038B48(current, requested);
    sint32 delta = (sint16)(difference & 0xffff);
    if (delta != 0)
    {
        sint32 selected = -step;
        if (selected < delta)
            selected = delta;
        if (selected >= step)
            selected = step;
        uint32 previous = r_u16(0x800a2526);
        w_u16(0x800a2526, previous + (uint32)selected);
    }
    uint32 value = r_u16(0x800a2526);
    uint32 wrapped = value & 0xfff;
    w_u16(0x800a2526, wrapped);
    uint32 result = wrapped - 4096;
    if (wrapped >= 0x800)
        w_u16(0x800a2526, result);
    return result;
}

uint32 G3Function80038B48(uint32 first, uint32 second)
{
    FUNCTION_MARKER(0x80038b48, "SLUS_008.06");
    uint32 delta = ((second & 0xfff) - (first & 0xfff)) & 0xfff;
    if (delta >= 0x800)
        delta |= 0xf000;
    return (uint32)(sint32)(sint16)(delta & 0xffff);
}

uint32 G3Function8002F240(uint32 y, uint32 x, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002f240, "SLUS_008.06");
    if (x == 0)
        return (sint32)y < 0 ? uint32(0xfffffc00) : uint32(1024);
    if (y == 0)
        return (sint32)x < 1 ? uint32(2048) : uint32(0);
    uint32 result = (uint32)ratan2((sint32)y, (sint32)x);
    return (uint32)(sint32)(sint16)(result & 0xffff);
}

uint32 G3Function8005A088(uint32 output, uint32 target, uint32 angle, uint32 tilt, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005a088, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 output_x = r_u16(output);
    uint32 target_x = r_u16(target);
    w_u16(frame + 0x10, output_x - target_x);
    uint32 output_y = r_u16(output + 2);
    uint32 target_y = r_u16(target + 2);
    w_u16(frame + 0x12, output_y - target_y);
    uint32 target_z = r_u16(target + 4);
    uint32 output_z = r_u16(output + 4);
    w_u16(frame + 0x14, output_z - target_z);
    uint32 value = G3Function80059FD8(frame + 0x10, (uint32)(sint32)(sint16)(tilt & 0xffff), frame);
    uint32 left_width = r_u16(0x8009ede8);
    uint32 left_radius = r_u16(0x8009edf0);
    uint32 right_width = r_u16(0x8009ede4);
    uint32 left_center = angle - left_width;
    uint32 left_lower = left_center - left_radius;
    uint32 left_upper = left_radius + left_center;
    uint32 right_center = angle + right_width;
    uint32 right_radius = r_u16(0x8009edec);
    uint32 right_lower = right_center - right_radius;
    uint32 right_upper = right_radius + right_center;
    sint32 signed_value = (sint16)(value & 0xffff);
    uint32 result;
    if ((sint16)(left_lower & 0xffff) >= signed_value)
        result = value + left_width;
    else if ((sint16)(left_upper & 0xffff) >= signed_value)
    {
        sint32 delta = (sint16)((left_upper - value) & 0xffff);
        sint32 square = delta * delta;
        uint32 radius = r_u32(0x8009edf0);
        sint32 denominator = (sint32)(radius << 2);
        uint32 quotient = denominator == 0 ? uint32(0xffffffff) : (uint32)(square / denominator);
        result = angle - quotient;
    }
    else if ((sint16)(right_lower & 0xffff) >= signed_value)
        result = angle;
    else if ((sint16)(right_upper & 0xffff) < signed_value)
        result = value - right_width;
    else
    {
        sint32 delta = (sint16)((value - right_lower) & 0xffff);
        sint32 square = delta * delta;
        uint32 radius = r_u32(0x8009edec);
        sint32 denominator = (sint32)(radius << 2);
        uint32 quotient = denominator == 0 ? uint32(0xffffffff) : (uint32)(square / denominator);
        result = angle + quotient;
    }
    return (uint32)(sint32)(sint16)(result & 0xffff);
}

uint32 G3Function80059FD8(uint32 vector, uint32 tilt, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80059fd8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 shifted_angle = (uint32)(sint32)(sint16)((tilt + 1024) & 0xffff);
    w_u16(frame + 0x10, 0);
    w_u16(frame + 0x12, 0);
    w_u16(frame + 0x14, 0);
    w_u16(frame + 0x10, 4096);
    G3Function8002F8E4(frame + 0x10, shifted_angle);
    sint32 axis_x = r_s16(frame + 0x10);
    sint32 vector_x = r_s16(vector);
    uint32 product_x = (uint32)(axis_x * vector_x);
    sint32 axis_y = r_s16(frame + 0x12);
    sint32 vector_y = r_s16(vector + 2);
    uint32 product_y = (uint32)(axis_y * vector_y);
    sint32 vector_z = r_s16(vector + 4);
    sint32 axis_z = r_s16(frame + 0x14);
    uint32 product_z = (uint32)(axis_z * vector_z);
    uint32 projection = uint32(0) - (product_x + product_y + product_z);
    if ((sint32)projection < 0)
        projection += 4095;
    uint32 horizontal = (uint32)((sint32)projection >> 12);
    uint32 result = G3Function8002F240(uint32(0) - (uint32)vector_z, horizontal, frame);
    return (uint32)(sint32)(sint16)(result & 0xffff);
}

uint32 G3Function800144CC(uint32 output, uint32 first, uint32 second, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800144cc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    sint32 second_x = r_s16(second);
    sint32 first_x = r_s16(first);
    uint32 delta_x = (uint32)(second_x - first_x);
    uint32 square_x = delta_x * delta_x;
    w_u32(frame + 0x10, delta_x);
    sint32 second_y = r_s16(second + 2);
    sint32 first_y = r_s16(first + 2);
    uint32 delta_y = (uint32)(second_y - first_y);
    uint32 square_y = delta_y * delta_y;
    w_u32(frame + 0x14, delta_y);
    sint32 second_z = r_s16(second + 4);
    sint32 first_z = r_s16(first + 4);
    w_u32(frame + 0x20, delta_x);
    w_u32(frame + 0x24, delta_y);
    w_u32(frame + 0x28, 0);
    w_u32(frame + 0x18, (uint32)(second_z - first_z));
    uint32 square_sum = square_x + square_y;
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
    uint32 delta_z = r_u32(frame + 0x18);
    uint32 pitch = G3Function8002F240(delta_z, horizontal, frame);
    w_u16(output, uint32(0) - pitch);
    w_u16(output + 2, 0);
    uint32 saved_y = r_u32(frame + 0x14);
    uint32 saved_x = r_u32(frame + 0x10);
    uint32 yaw = G3Function8002F240(saved_y, saved_x, frame);
    uint32 result = yaw + 1024;
    w_u16(output + 4, result);
    return result;
}

uint32 G3Function80056934(uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80056934, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 kind = r_u32(payload + 4);
    if (kind != 16)
    {
        w_u32(0x800a2488, 0);
        return 16;
    }
    uint32 mode = r_u32(payload);
    if (mode != 6)
    {
        uint32 result = 20;
        if (mode == 5)
            result = 7;
        else if (mode == 7)
            result = 8;
        else if (mode == 8)
            result = 15;
        if (mode == 5 || mode == 7 || mode == 8 || mode == 15 || mode == 20)
        {
            w_u16(0x800a24c4, 0);
            w_u16(0x800a24c6, 0);
            w_u16(0x800a24c8, 0);
        }
        else
            w_u32(0x800a2488, 0);
        return result;
    }
    uint32 index = r_u32(0x80096a88);
    uint32 target = r_u32(0x80099650 + (index << 3) + 4);
    uint32 angle = 0;
    if (target != 0)
    {
        uint32 game = r_u32(0x80097048);
        uint32 object = r_u32(game + 0x4c);
        angle = G3DispatchStateEvent(target, object, game, 25, 0, frame);
    }
    uint32 opposite = (uint32)(sint32)(sint16)((angle + 2048) & 0xffff);
    uint32 controlled = r_u32(0x8009efc4);
    w_u32(0x800a24ec, (uint32)(sint32)(sint16)(angle & 0xffff));
    G3Function800772D8(controlled, opposite);
    G3Function800560FC(frame + 0x10, frame);
    uint32 blocked = G3Function80052B10(0, 0x800a24a0, frame + 0x10, frame);
    if (blocked != 0)
        return 1;
    w_u32(0x800a2488, 1);
    uint32 source = frame + 0x10;
    uint32 upper = r_u32((source + 3) & ~uint32(3));
    uint32 lower = r_u32(source & ~uint32(3));
    uint32 shift = (source & 3) << 3;
    uint32 value = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
    uint32 tail = (uint32)(sint32)r_s16(source + 4);
    uint32 previous = r_u32(0x800a248c);
    w_u32(0x800a248c, (previous & 0) | value);
    previous = r_u32(0x800a248c);
    w_u32(0x800a248c, (previous & 0) | value);
    w_u16(0x800a2490, tail);
    return 1;
}

uint32 G3Function800772D8(uint32 object, uint32 angle)
{
    FUNCTION_MARKER(0x800772d8, "SLUS_008.06");
    uint32 state = r_u32(object + 0x24);
    w_u16(state + 0xc6, angle);
    w_u16(state + 0xc8, angle);
    w_u16(object + 0x64, angle);
    return state;
}

uint32 G3Function8002F75C(uint32 vector, uint32 angle)
{
    FUNCTION_MARKER(0x8002f75c, "SLUS_008.06");
    uint32 signed_angle = (uint32)(sint32)(sint16)(angle & 0xffff);
    uint32 stored_y = r_u16(vector + 2);
    uint32 stored_z = r_u16(vector + 4);
    uint32 sine_word = G3Function8005B590(signed_angle);
    uint32 cosine_word = G3Function8005B470(signed_angle);
    sint32 sine = (sint16)(sine_word & 0xffff);
    sint32 cosine = (sint16)(cosine_word & 0xffff);
    sint32 y = (sint16)(stored_y & 0xffff);
    sint32 z = (sint16)(stored_z & 0xffff);
    uint32 rotated_y = (uint32)(cosine * y) - (uint32)(sine * z);
    uint32 rotated_z = (uint32)(sine * y) + (uint32)(cosine * z);
    w_u16(vector + 2, (uint32)((sint32)rotated_y >> 12));
    uint32 result = (uint32)((sint32)rotated_z >> 12);
    w_u16(vector + 4, result);
    return result;
}

uint32 G3Function80052B10(uint32 mode, uint32 origin, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80052b10, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 object = r_u32(0x80096b34);
    uint32 target = r_u32(object + 0xec);
    if (target == 0)
        return 0;
    uint32 game = r_u32(0x80097048);
    uint32 available = G3DispatchStateEvent(target, object, game, 2024, frame + 0x18, frame);
    if (available == 0)
        return 0;
    uint32 source = frame + 0x18;
    uint32 destination = frame + 0x10;
    uint32 upper = r_u32((source + 3) & ~uint32(3));
    uint32 lower = r_u32(source & ~uint32(3));
    uint32 shift = (source & 3) << 3;
    uint32 value = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
    uint32 tail = (uint32)(sint32)r_s16(source + 4);
    uint32 high_address = destination + 3;
    uint32 high_byte = high_address & 3;
    uint32 preserve = high_byte == 3 ? 0 : uint32(0xffffffff) << ((high_byte + 1) << 3);
    uint32 previous = r_u32(high_address & ~uint32(3));
    w_u32(high_address & ~uint32(3), (previous & preserve) | (value >> ((3 - high_byte) << 3)));
    uint32 low_byte = destination & 3;
    preserve = low_byte == 0 ? 0 : uint32(0xffffffff) >> ((4 - low_byte) << 3);
    previous = r_u32(destination & ~uint32(3));
    w_u32(destination & ~uint32(3), (previous & preserve) | (value << (low_byte << 3)));
    w_u16(destination + 4, tail);
    uint32 radius = (uint32)(sint32)r_s16(frame + 0x1e);
    uint32 angle = (uint32)(sint32)(sint16)(mode & 0xffff);
    return G3Function80052514(frame + 0x10, radius, angle, output, frame);
}

uint32 G3Function80059E70(uint32 object, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80059e70, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 current = (uint32)(sint32)r_s16(object + 0x18);
    uint32 target = (uint32)(sint32)r_s16(0x800a252a);
    uint32 difference = G3Function80038B48(current, target);
    if ((difference << 16) == 0)
        return difference << 16;
    uint32 state_x = r_u16(state);
    uint32 object_x = r_u16(object);
    uint32 delta_x = state_x - object_x;
    sint32 signed_x = (sint16)(delta_x & 0xffff);
    uint32 square_x = (uint32)(signed_x * signed_x);
    w_u16(frame + 0x20, delta_x);
    uint32 state_y = r_u16(state + 2);
    uint32 object_y = r_u16(object + 2);
    uint32 delta_y = state_y - object_y;
    sint32 signed_y = (sint16)(delta_y & 0xffff);
    uint32 square_y = (uint32)(signed_y * signed_y);
    w_u16(frame + 0x22, delta_y);
    uint32 state_z = r_u16(state + 4);
    uint32 object_z = r_u16(object + 4);
    uint32 delta_z = state_z - object_z;
    uint32 square_sum = square_x + square_y;
    w_u16(frame + 0x24, 0);
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
    w_u16(frame + 0x10, 0);
    w_u16(frame + 0x12, uint32(0) - distance);
    w_u16(frame + 0x14, delta_z);
    uint32 words[2];
    for (uint32 index = 0; index != 2; ++index)
    {
        uint32 source = frame + 0x10 + index * 4;
        uint32 upper = r_u32((source + 3) & ~uint32(3));
        uint32 lower = r_u32(source & ~uint32(3));
        uint32 shift = (source & 3) << 3;
        words[index] = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
    }
    for (uint32 index = 0; index != 2; ++index)
    {
        uint32 destination = frame + 0x18 + index * 4;
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
    G3Function8002F75C(frame + 0x18, (uint32)(sint32)(sint16)(difference & 0xffff));
    uint32 rotated = r_u16(frame + 0x1c);
    uint32 original = r_u16(frame + 0x14);
    w_u16(state + 0x36, 0);
    uint32 result = rotated - original;
    w_u16(state + 0x34, result);
    return result;
}

uint32 G3Function80057A34(uint32 object, uint32 game, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80057a34, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    uint32 object_x = r_u16(object);
    uint32 origin_x = r_u16(0x800a2494);
    w_u16(frame + 0x20, object_x - origin_x);
    uint32 object_y = r_u16(object + 2);
    uint32 origin_y = r_u16(0x800a2496);
    w_u16(frame + 0x22, object_y - origin_y);
    uint32 object_z = r_u16(object + 4);
    uint32 origin_z = r_u16(0x800a2498);
    w_u16(frame + 0x24, object_z - origin_z);
    uint32 angle = (uint32)(sint32)r_s16(payload + 6);
    G3Function8002F8E4(frame + 0x20, angle);
    G3Function80056A74(0x800a2494, frame);
    origin_x = r_u16(0x800a2494);
    uint32 local_x = r_u16(frame + 0x20);
    origin_y = r_u16(0x800a2496);
    uint32 local_y = r_u16(frame + 0x22);
    origin_z = r_u16(0x800a2498);
    uint32 local_z = r_u16(frame + 0x24);
    uint32 flags = r_u32(0x80096b28);
    uint32 end_x = origin_x + local_x;
    uint32 end_y = origin_y + local_y;
    uint32 end_z = origin_z + local_z;
    w_u16(frame + 0x20, end_x);
    w_u16(frame + 0x22, end_y);
    w_u16(frame + 0x24, end_z);
    w_u16(frame + 0x28, end_x);
    w_u16(frame + 0x2a, end_y);
    w_u16(frame + 0x2c, end_z);
    w_u16(frame + 0x30, origin_x);
    w_u16(frame + 0x32, origin_y);
    w_u16(frame + 0x34, origin_z);
    uint32 mask = (flags & 0x400) == 0 ? uint32(0x01d529e8) : uint32(0x01d52be8);
    uint32 current_game = r_u32(0x80097048);
    uint32 world_pointer = r_u32(current_game + 0x48);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, mask);
    w_u32(frame + 0x18, 0);
    uint32 world = r_u32(world_pointer);
    G3Function8006238C(world, frame + 0x20, frame + 0x30, 1, frame);
    uint32 x = r_u16(frame + 0x20);
    w_u16(object, x);
    uint32 y = r_u16(frame + 0x22);
    w_u16(object + 2, y);
    uint32 z = r_u16(frame + 0x24);
    w_u16(object + 4, z);
    return G3Function8005798C(object, frame);
}

uint32 G3Function8005798C(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005798c, "SLUS_008.06");
    for (uint32 index = 0; index != 2; ++index)
    {
        uint32 source = index == 0 ? object : uint32(0x800a2494);
        uint32 destination = index == 0 ? uint32(0x80096a54) : uint32(0x800a249a);
        uint32 upper = r_u32((source + 3) & ~uint32(3));
        uint32 lower = r_u32(source & ~uint32(3));
        uint32 shift = (source & 3) << 3;
        uint32 value = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
        uint32 tail = (uint32)(sint32)r_s16(source + 4);
        uint32 high_address = destination + 3;
        uint32 high_byte = high_address & 3;
        uint32 preserve = high_byte == 3 ? 0 : uint32(0xffffffff) << ((high_byte + 1) << 3);
        uint32 previous = r_u32(high_address & ~uint32(3));
        w_u32(high_address & ~uint32(3), (previous & preserve) | (value >> ((3 - high_byte) << 3)));
        uint32 low_byte = destination & 3;
        preserve = low_byte == 0 ? 0 : uint32(0xffffffff) >> ((4 - low_byte) << 3);
        previous = r_u32(destination & ~uint32(3));
        w_u32(destination & ~uint32(3), (previous & preserve) | (value << (low_byte << 3)));
        w_u16(destination + 4, tail);
    }
    G3Function800524E4();
    G3FillMemory80086094(0x800a2488, 0, 12);
    uint32 angle = r_u16(0x80096b6a);
    uint32 distance = r_u16(0x80096b68);
    w_u16(0x800a24cc, distance);
    w_u16(0x800a2526, angle);
    uint32 result = G3Function800571F8((uint32)(sint32)(sint16)(angle & 0xffff), (uint32)(sint32)(sint16)(distance & 0xffff));
    w_u16(0x800a2524, result);
    return result;
}

uint32 G3Function800524E4(void)
{
    FUNCTION_MARKER(0x800524e4, "SLUS_008.06");
    uint32 flags = r_u32(0x800a24d0);
    w_u32(0x800a2504, 0);
    w_u32(0x800a24e8, 0);
    uint32 result = flags & 0xffffca1f;
    w_u32(0x800a24d0, result);
    return result;
}

uint32 G3Function80057284(uint32 output, uint32 state, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80057284, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 initial_x = r_u16(object);
    w_u16(frame + 0x10, initial_x);
    uint32 initial_y = r_u16(object + 2);
    w_u16(frame + 0x12, initial_y);
    uint32 initial_z = r_u16(object + 4);
    w_u16(frame + 0x14, initial_z);
    uint32 bypass = r_u32(0x80096b6c);
    uint32 game = r_u32(0x80097048);
    if (bypass == 0)
    {
        uint32 flags = r_u32(game + 0x70);
        if ((flags & 12) == 0)
            w_u16(0x800a2580, 0);
        else
        {
            uint32 old_timer = r_u16(0x800a2580);
            uint32 incremented = old_timer + 1;
            w_u16(0x800a2580, incremented);
            sint32 timer = (sint16)(incremented & 0xffff);
            sint32 limit = r_s32(0x8009ec64);
            uint32 selected;
            if (timer > 0 ? timer < limit : limit > 0)
            {
                sint32 signed_timer = r_s16(0x800a2580);
                uint32 timer_bits = r_u16(0x800a2580);
                selected = signed_timer < 0 ? 0 : timer_bits;
            }
            else
                selected = r_u16(0x8009ec64);
            w_u16(0x800a2580, selected);
        }
        G3Function800587E0(frame + 0x10, state, frame + 0x18, frame);
        if (r_s16(frame + 0x18) != 0 || r_s16(frame + 0x1a) != 0 || r_s16(frame + 0x1c) != 0)
        {

            uint32 x = r_u16(frame + 0x10);
            uint32 dx = r_u16(frame + 0x18);
            uint32 dy = r_u16(frame + 0x1a);
            uint32 dz = r_u16(frame + 0x1c);
            w_u16(frame + 0x10, x + dx);
            uint32 y = r_u16(frame + 0x12);
            uint32 z = r_u16(frame + 0x14);
            w_u16(frame + 0x12, y + dy);
            w_u16(frame + 0x14, z + dz);
        }
        uint32 active = r_u32(state + 0x2c);
        if (active != 0)
        {
            uint32 mode = r_u32(0x800a24d0);
            if ((mode & 6) != 0)
            {
                uint32 diagnostics = r_u32(0x8009ea44);
                if ((diagnostics & 4) == 0)
                {
                    G3Function8005851C(object, state, frame + 0x18, frame);
                    {
                        uint32 x = r_u16(frame + 0x10);
                        uint32 dx = r_u16(frame + 0x18);
                        uint32 dy = r_u16(frame + 0x1a);
                        uint32 dz = r_u16(frame + 0x1c);
                        w_u16(frame + 0x10, x + dx);
                        uint32 y = r_u16(frame + 0x12);
                        uint32 z = r_u16(frame + 0x14);
                        w_u16(frame + 0x12, y + dy);
                        w_u16(frame + 0x14, z + dz);
                    }
                    G3Function80058680(object, state, frame + 0x18, frame);
                    {
                        uint32 x = r_u16(frame + 0x10);
                        uint32 dx = r_u16(frame + 0x18);
                        uint32 dy = r_u16(frame + 0x1a);
                        uint32 dz = r_u16(frame + 0x1c);
                        w_u16(frame + 0x10, x + dx);
                        uint32 y = r_u16(frame + 0x12);
                        uint32 z = r_u16(frame + 0x14);
                        w_u16(frame + 0x12, y + dy);
                        w_u16(frame + 0x14, z + dz);
                    }
                }
            }
        }
        G3Function80053C78(frame + 0x10, object, state, frame + 0x18, frame);
        if (r_s16(frame + 0x18) != 0 || r_s16(frame + 0x1a) != 0 || r_s16(frame + 0x1c) != 0)
        {
            uint32 x = r_u16(frame + 0x10);
            uint32 dx = r_u16(frame + 0x18);
            uint32 dy = r_u16(frame + 0x1a);
            uint32 z = r_u16(frame + 0x14);
            uint32 dz = r_u16(frame + 0x1c);
            uint32 y = r_u16(frame + 0x12);
            uint32 adjusted_x = x + dx;
            w_u16(frame + 0x10, adjusted_x);
            w_u16(frame + 0x14, z + dz);
            w_u16(frame + 0x12, y + dy);
            w_u16(object, adjusted_x);
            uint32 adjusted_y = r_u16(frame + 0x12);
            w_u16(object + 2, adjusted_y);
            uint32 adjusted_z = r_u16(frame + 0x14);
            w_u16(object + 4, adjusted_z);
            G3Function800524E4();
        }
        G3Function80058F48(frame + 0x10, state, frame + 0x18, frame);
        if (r_s16(frame + 0x18) != 0 || r_s16(frame + 0x1a) != 0 || r_s16(frame + 0x1c) != 0)
        {

            uint32 x = r_u16(frame + 0x10);
            uint32 dx = r_u16(frame + 0x18);
            uint32 dy = r_u16(frame + 0x1a);
            uint32 dz = r_u16(frame + 0x1c);
            w_u16(frame + 0x10, x + dx);
            uint32 y = r_u16(frame + 0x12);
            uint32 z = r_u16(frame + 0x14);
            w_u16(frame + 0x12, y + dy);
            w_u16(frame + 0x14, z + dz);
        }
        G3Function80059624(frame + 0x10, state, frame + 0x18, frame);
        if (r_s16(frame + 0x18) != 0 || r_s16(frame + 0x1a) != 0 || r_s16(frame + 0x1c) != 0)
        {

            uint32 x = r_u16(frame + 0x10);
            uint32 dx = r_u16(frame + 0x18);
            uint32 dy = r_u16(frame + 0x1a);
            uint32 dz = r_u16(frame + 0x1c);
            w_u16(frame + 0x10, x + dx);
            uint32 y = r_u16(frame + 0x12);
            uint32 z = r_u16(frame + 0x14);
            w_u16(frame + 0x12, y + dy);
            w_u16(frame + 0x14, z + dz);
        }
        G3Function800536AC(frame + 0x10, state, 0x800a2558, frame + 0x18, frame);
        if (r_s16(frame + 0x18) != 0 || r_s16(frame + 0x1a) != 0 || r_s16(frame + 0x1c) != 0)
        {

            uint32 x = r_u16(frame + 0x10);
            uint32 dx = r_u16(frame + 0x18);
            uint32 dy = r_u16(frame + 0x1a);
            uint32 dz = r_u16(frame + 0x1c);
            w_u16(frame + 0x10, x + dx);
            uint32 y = r_u16(frame + 0x12);
            uint32 z = r_u16(frame + 0x14);
            w_u16(frame + 0x12, y + dy);
            w_u16(frame + 0x14, z + dz);
        }
        G3Function80059200(frame + 0x10, state, frame + 0x18, frame);
        if (r_s16(frame + 0x18) != 0 || r_s16(frame + 0x1a) != 0 || r_s16(frame + 0x1c) != 0)
        {

            uint32 x = r_u16(frame + 0x10);
            uint32 dx = r_u16(frame + 0x18);
            uint32 dy = r_u16(frame + 0x1a);
            uint32 dz = r_u16(frame + 0x1c);
            w_u16(frame + 0x10, x + dx);
            uint32 y = r_u16(frame + 0x12);
            uint32 z = r_u16(frame + 0x14);
            w_u16(frame + 0x12, y + dy);
            w_u16(frame + 0x14, z + dz);
        }
        G3Function80059B18(frame + 0x10, state, frame + 0x18, frame);
        if (r_s16(frame + 0x18) != 0 || r_s16(frame + 0x1a) != 0 || r_s16(frame + 0x1c) != 0)
        {

            uint32 x = r_u16(frame + 0x10);
            uint32 dx = r_u16(frame + 0x18);
            uint32 dy = r_u16(frame + 0x1a);
            uint32 dz = r_u16(frame + 0x1c);
            w_u16(frame + 0x10, x + dx);
            uint32 y = r_u16(frame + 0x12);
            uint32 z = r_u16(frame + 0x14);
            w_u16(frame + 0x12, y + dy);
            w_u16(frame + 0x14, z + dz);
        }
        uint32 previous_flags = r_u32(0x800a24d0);
        w_u16(0x800a24d8, 0);
        w_u16(0x800a24da, 0);
        w_u16(0x800a24dc, 0);
        w_u32(0x800a24d0, previous_flags & 0xfffffff9);
        G3Function80057040(object, state, frame + 0x10, frame + 0x18, frame);
        if (r_s16(frame + 0x18) != 0 || r_s16(frame + 0x1a) != 0 || r_s16(frame + 0x1c) != 0)
        {
            uint32 lock = r_u32(0x800a259c);
            if (lock != 0)
            {
                uint32 enabled = r_u32(0x800a2594);
                if (enabled != 0)
                    w_u16(frame + 0x1c, 0);
            }

            uint32 x = r_u16(frame + 0x10);
            uint32 dx = r_u16(frame + 0x18);
            uint32 dy = r_u16(frame + 0x1a);
            uint32 dz = r_u16(frame + 0x1c);
            w_u16(frame + 0x10, x + dx);
            uint32 y = r_u16(frame + 0x12);
            uint32 z = r_u16(frame + 0x14);
            w_u16(frame + 0x12, y + dy);
            w_u16(frame + 0x14, z + dz);
        }
        G3Function80059D94(frame + 0x10, object, frame + 0x18, frame);
        if (r_s16(frame + 0x18) != 0 || r_s16(frame + 0x1a) != 0 || r_s16(frame + 0x1c) != 0)
        {

            uint32 x = r_u16(frame + 0x10);
            uint32 dx = r_u16(frame + 0x18);
            uint32 dy = r_u16(frame + 0x1a);
            uint32 dz = r_u16(frame + 0x1c);
            w_u16(frame + 0x10, x + dx);
            uint32 y = r_u16(frame + 0x12);
            uint32 z = r_u16(frame + 0x14);
            w_u16(frame + 0x12, y + dy);
            w_u16(frame + 0x14, z + dz);
        }
        G3Function80014F38(frame + 0x10, frame + 0x18, frame);
        if (r_s16(frame + 0x18) != 0 || r_s16(frame + 0x1a) != 0 || r_s16(frame + 0x1c) != 0)
        {

            uint32 x = r_u16(frame + 0x10);
            uint32 dx = r_u16(frame + 0x18);
            uint32 dy = r_u16(frame + 0x1a);
            uint32 dz = r_u16(frame + 0x1c);
            w_u16(frame + 0x10, x + dx);
            uint32 y = r_u16(frame + 0x12);
            uint32 z = r_u16(frame + 0x14);
            w_u16(frame + 0x12, y + dy);
            w_u16(frame + 0x14, z + dz);
        }
    }
    uint32 final_x = r_u16(frame + 0x10);
    w_u16(output, final_x);
    uint32 final_y = r_u16(frame + 0x12);
    w_u16(output + 2, final_y);
    uint32 final_z = r_u16(frame + 0x14);
    w_u16(output + 4, final_z);
    uint32 state_z = r_u16(state + 4);
    sint32 height = (sint16)((final_z - state_z) & 0xffff);
    if (height <= 0)
        return (uint32)height;
    sint32 offset = r_s32(0x800a2534);
    if (offset < 0)
        offset = 0;
    uint32 result = offset < height ? 1 : 0;
    w_u32(0x800a2534, (uint32)(offset < height ? offset : height));
    return result;
}

uint32 G3Function8005851C(uint32 object, uint32 state, uint32 delta, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005851c, "SLUS_008.06");
    w_u16(delta, 0);
    w_u16(delta + 2, 0);
    w_u16(delta + 4, 0);
    uint32 flags = r_u32(0x800a24d0);
    if ((flags & 4) != 0)
        w_u32(0x800a24d0, flags | 1);
    flags = r_u32(0x800a24d0);
    if ((flags & 1) == 0)
        return 0;
    uint32 angle = r_u32(0x8009ec80);
    uint32 sine = G3Function8005B590(angle);
    uint32 distance = (uint32)(sint32)r_s16(0x800a24cc);
    uint32 product = distance * sine;
    uint32 object_z = r_u16(object + 4);
    uint32 offset = r_u16(0x800a2534);
    uint32 bound = (uint32)((sint32)(uint32(0) - product) >> 12) + offset;
    uint32 state_z = r_u16(state + 4);
    uint32 difference = object_z - (state_z - 32);
    sint32 signed_difference = (sint16)(difference & 0xffff);
    sint32 signed_bound = (sint16)(bound & 0xffff);
    uint32 selected;
    if (signed_difference > 0 ? signed_difference >= signed_bound : signed_bound <= 0)
        selected = bound;
    else
        selected = (sint16)(difference & 0xffff) < 0 ? 0 : difference;
    state_z = r_u16(state + 4);
    object_z = r_u16(object + 4);
    uint32 correction = state_z + selected - object_z;
    sint32 signed_correction = (sint16)(correction & 0xffff);
    if (signed_correction >= 32)
        correction = 32;
    else if (signed_correction < 0)
        correction = 0;
    uint32 previous = r_u16(delta + 4);
    uint32 result = correction + previous;
    w_u16(delta + 4, result);
    return result;
}

uint32 G3Function80058680(uint32 object, uint32 state, uint32 delta, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80058680, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    w_u16(delta, 0);
    w_u16(delta + 2, 0);
    w_u16(delta + 4, 0);
    uint32 flags = r_u32(0x800a24d0);
    uint32 game = r_u32(0x80097048);
    if ((flags & 1) != 0)
        return 1;
    sint32 turn = r_s16(0x800a24d6);
    if (turn < 0)
        turn = -turn;
    bool active = false;
    if (turn < 100)
    {
        uint32 mode = r_u32(game + 0x70);
        active = (mode & 15) == 2;
    }
    if (!active)
    {
        uint32 diagnostics = r_u32(0x8009ea44);
        if ((diagnostics & 16) == 0)
            return 0;
    }
    uint32 direction = r_u16(0x800a24d4);
    uint32 words[2];
    for (uint32 index = 0; index != 2; ++index)
    {
        uint32 source = 0x800a24d8 + index * 4;
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
    uint32 angle = (direction & 0x8000) != 0 ? uint32(1024) : uint32(0xfffffc00);
    G3Function8002F8E4(frame + 0x10, angle);
    G3Function8002F9A8(frame + 0x10, 32, frame + 0x10, frame);
    uint32 x = r_u16(frame + 0x10);
    w_u16(delta, x);
    uint32 y = r_u16(frame + 0x12);
    w_u16(delta + 2, y);
    uint32 z = r_u16(frame + 0x14);
    w_u16(delta + 4, z);
    return z;
}

uint32 G3Function800587E0(uint32 position, uint32 state, uint32 delta, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800587e0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    w_u16(delta, 0);
    w_u16(delta + 2, 0);
    w_u16(delta + 4, 0);
    uint32 flags = r_u32(0x800a24d0);
    uint32 radius_bits = r_u16(0x800a2586);
    if ((flags & 0x200) == 0)
        return 0;
    w_u32(0x800a24d0, flags | 0x80000);
    G3Function800560FC(frame + 0x10, frame);
    uint32 target_x = r_u16(frame + 0x10);
    uint32 position_x = r_u16(position);
    uint32 target_y = r_u16(frame + 0x12);
    uint32 difference_x = target_x - position_x;
    w_u16(frame + 0x18, difference_x);
    uint32 position_y = r_u16(position + 2);
    uint32 difference_y = target_y - position_y;
    uint32 target_z = r_u16(frame + 0x14);
    w_u16(frame + 0x1a, difference_y);
    uint32 position_z = r_u16(position + 4);
    uint32 difference_z = target_z - position_z;
    w_u16(frame + 0x1c, difference_z);
    bool position_finished = false;
    if (((difference_x | difference_y | difference_z) & 0xffff) != 0)
    {
        sint32 x = r_s16(frame + 0x18);
        uint32 square_x = (uint32)(x * x);
        sint32 y = r_s16(frame + 0x1a);
        uint32 square_y = (uint32)(y * y);
        sint32 z = r_s16(frame + 0x1c);
        uint32 square_z = (uint32)(z * z);
        sint32 radius = (sint16)(radius_bits & 0xffff);
        sint32 square_radius = radius * radius;
        uint32 square_sum = square_x + square_y + square_z;
        if (square_radius < (sint32)square_sum)
            G3Function8002F9A8(frame + 0x18, (uint32)radius, frame + 0x18, frame);
        else
        {
            uint32 game = r_u32(0x80097048);
            uint32 controlled = r_u32(game + 0x4c);
            uint32 angle = r_u16(controlled + 0x18);
            position_finished = true;
            w_u16(0x800a2526, angle);
        }
        uint32 x_out = r_u16(frame + 0x18);
        w_u16(delta, x_out);
        uint32 y_out = r_u16(frame + 0x1a);
        w_u16(delta + 2, y_out);
        uint32 z_out = r_u16(frame + 0x1c);
        w_u16(delta + 4, z_out);
    }
    else
        position_finished = true;
    uint32 result = r_u32(0x800a24c4);
    bool offset_active = result != 0;
    if (!offset_active)
    {
        result = (uint32)(sint32)r_s16(0x800a24c8);
        offset_active = result != 0;
    }
    bool offset_finished = false;
    if (offset_active)
    {
        sint32 x = r_s16(0x800a24c4);
        uint32 square_x = (uint32)(x * x);
        sint32 y = r_s16(0x800a24c6);
        uint32 square_y = (uint32)(y * y);
        sint32 z = r_s16(0x800a24c8);
        uint32 square_z = (uint32)(z * z);
        uint32 limit_bits = r_u16(0x8009edd4);
        uint32 square_sum = square_x + square_y + square_z;
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
        sint32 limit = (sint16)(limit_bits & 0xffff);
        uint32 current_radius = (uint32)(sint32)r_s16(0x800a24ca);
        uint32 change = G3Function80064680((uint32)(sint32)(sint16)(distance & 0xffff), current_radius, (uint32)limit, frame);
        uint32 previous_radius = r_u16(0x800a24ca);
        sint32 increment = (sint16)(change & 0xffff);
        uint32 updated = previous_radius + (uint32)increment;
        w_u16(0x800a24ca, updated);
        sint32 selected = -limit;
        sint32 signed_updated = (sint16)(updated & 0xffff);
        if (selected < signed_updated)
            selected = signed_updated;
        if (selected >= limit)
            selected = limit;
        w_u16(0x800a24ca, (uint32)selected);
        sint32 half_limit = (limit + (limit < 0 ? 1 : 0)) >> 1;
        if (increment == 0 && half_limit >= (sint16)((uint32)selected & 0xffff))
        {
            offset_finished = true;
            w_u16(0x800a24c4, 0);
            w_u16(0x800a24c6, 0);
            w_u16(0x800a24c8, 0);
            w_u16(0x800a24ca, 0);
            result = 0;
        }
        else
        {
            sint32 signed_radius = r_s16(0x800a24ca);
            uint32 unsigned_radius = r_u16(0x800a24ca);
            if (signed_radius != 0)
            {
                uint32 negative_radius = uint32(0) - unsigned_radius;
                uint32 length = (uint32)(sint32)(sint16)(negative_radius & 0xffff);
                G3Function8002F9A8(frame + 0x18, length, 0x800a24c4, frame);
            }
            uint32 offset_x = r_u16(0x800a24c4);
            uint32 correction_x = r_u16(frame + 0x18);
            w_u16(0x800a24c4, offset_x - correction_x);
            uint32 offset_y = r_u16(0x800a24c6);
            uint32 correction_y = r_u16(frame + 0x1a);
            uint32 offset_z = r_u16(0x800a24c8);
            uint32 correction_z = r_u16(frame + 0x1c);
            result = offset_y - correction_y;
            w_u16(0x800a24c6, result);
            w_u16(0x800a24c8, offset_z - correction_z);
        }
    }
    else
        offset_finished = true;
    if (position_finished && offset_finished)
    {
        uint32 current_flags = r_u32(0x800a24d0);
        result = current_flags & 0xfffffdff;
        w_u32(0x800a24d0, result);
    }
    return result;
}

uint32 G3Function8002F9A8(uint32 output, uint32 length, uint32 vector, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002f9a8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 magnitude = G3Function8002FA50(vector, frame);
    sint32 numerator = (sint32)(length << 16) >> 4;
    uint32 factor = magnitude == 0 ? 0 : (uint32)(numerator / (sint32)magnitude);
    uint32 x = (uint32)(sint32)r_s16(vector);
    uint32 scaled_x = (uint32)((sint32)(factor * x) >> 12);
    w_u16(output, scaled_x);
    uint32 y = (uint32)(sint32)r_s16(vector + 2);
    uint32 scaled_y = (uint32)((sint32)(factor * y) >> 12);
    w_u16(output + 2, scaled_y);
    uint32 z = (uint32)(sint32)r_s16(vector + 4);
    uint32 result = (uint32)((sint32)(factor * z) >> 12);
    w_u16(output + 4, result);
    return result;
}

uint32 G3Function80064680(uint32 target, uint32 current, uint32 limit, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80064680, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    sint32 signed_target = (sint16)(target & 0xffff);
    sint32 signed_current = (sint16)(current & 0xffff);
    sint32 signed_limit = (sint16)(limit & 0xffff);
    uint32 first = G3Function80064638((uint32)signed_target, (uint32)signed_current, (uint32)signed_limit, 0, frame);
    sint32 increment = -signed_limit;
    if ((first & 0xff) != 0 ? signed_target < 0 : signed_current < 0)
        increment = signed_limit;
    uint32 second = G3Function80064638((uint32)signed_target, (uint32)signed_current, (uint32)signed_limit, (uint32)increment, frame);
    sint32 result = increment;
    if ((second & 0xff) == 0)
    {
        sint32 cancellation = -(signed_target + signed_current);
        if (cancellation < signed_limit && signed_target < signed_limit)
            result = cancellation;
        else
            result = (sint32)G3Function8006459C((uint32)signed_target, (uint32)signed_current, (uint32)signed_limit);
    }
    if (result < -32767)
        result = -32767;
    if (result > 32767)
        result = 32767;
    return (uint32)result;
}

uint32 G3Function80064638(uint32 target, uint32 current, uint32 limit, uint32 increment, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80064638, "SLUS_008.06");
    uint32 next_current = current + increment;
    sint32 signed_limit = (sint16)(limit & 0xffff);
    uint32 result = G3Function8006459C(target + next_current, next_current, (uint32)signed_limit);
    if ((sint32)result < 0)
        result = uint32(0) - result;
    return signed_limit >= (sint32)result ? 1 : 0;
}

uint32 G3Function8006459C(uint32 target, uint32 current, uint32 limit)
{
    FUNCTION_MARKER(0x8006459c, "SLUS_008.06");
    if (current == 0)
        return 0;
    bool same_sign = ((sint32)target < 0) == ((sint32)current < 0);
    uint32 denominator = (target << 1) - current;
    if (same_sign || denominator == 0)
    {
        sint32 edge = (sint16)(limit & 0xffff);
        uint32 amount = (uint32)(edge + 1);
        return (sint32)current < 0 ? amount : uint32(0) - amount;
    }
    uint32 square = current * current;
    return (uint32)((sint32)square / (sint32)denominator);
}

uint32 G3Function80059D94(uint32 position, uint32 object, uint32 delta, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80059d94, "SLUS_008.06");
    w_u16(delta, 0);
    w_u16(delta + 2, 0);
    w_u16(delta + 4, 0);
    uint32 blocked = r_u32(0x800a2598);
    if (blocked != 0)
        return blocked;
    uint32 enabled = r_u32(0x800a2594);
    if (enabled == 0)
        return 0;
    uint32 ceiling = r_u16(0x800a2582);
    uint32 clearance = r_u16(0x8009ec88);
    sint32 signed_position = r_s16(position + 4);
    uint32 position_z = r_u16(position + 4);
    uint32 threshold = ceiling - clearance;
    sint32 signed_threshold = (sint16)(threshold & 0xffff);
    if (signed_threshold >= signed_position)
        return 0;
    sint32 signed_object = r_s16(object + 4);
    uint32 object_z = r_u16(object + 4);
    uint32 result;
    if (signed_threshold >= signed_object)
        result = threshold - position_z;
    else
    {
        if (signed_position < signed_object)
            object_z = position_z;
        uint32 limit = r_u32(0x8009ec8c);
        sint32 difference = (sint16)((threshold - object_z) & 0xffff);
        uint32 selected = uint32(0) - limit;
        if ((sint32)selected < difference)
            selected = (uint32)difference;
        if ((sint32)selected >= (sint32)limit)
            selected = limit;
        result = object_z + selected - position_z;
    }
    w_u16(delta + 4, result);
    return result;
}

uint32 G3Function80014F38(uint32 position, uint32 delta, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80014f38, "SLUS_008.06");
    w_u16(delta, 0);
    w_u16(delta + 2, 0);
    w_u16(delta + 4, 0);
    uint32 flags = r_u32(0x80096b28);
    uint32 frame = guest_stack - 8;
    if ((flags & 0x200) == 0)
        return 0;
    sint32 lower_x = r_s16(0x80096b5c);
    uint32 upper = r_u32((position + 3) & ~uint32(3));
    uint32 lower = r_u32(position & ~uint32(3));
    uint32 shift = (position & 3) << 3;
    uint32 value = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
    uint32 tail = (uint32)(sint32)r_s16(position + 4);
    uint32 high_address = frame + 3;
    uint32 high_byte = high_address & 3;
    uint32 preserve = high_byte == 3 ? 0 : uint32(0xffffffff) << ((high_byte + 1) << 3);
    uint32 previous = r_u32(high_address & ~uint32(3));
    w_u32(high_address & ~uint32(3), (previous & preserve) | (value >> ((3 - high_byte) << 3)));
    uint32 low_byte = frame & 3;
    preserve = low_byte == 0 ? 0 : uint32(0xffffffff) >> ((4 - low_byte) << 3);
    previous = r_u32(frame & ~uint32(3));
    w_u32(frame & ~uint32(3), (previous & preserve) | (value << (low_byte << 3)));
    w_u16(frame + 4, tail);
    sint32 signed_x = r_s16(frame);
    uint32 selected_x = r_u16(0x80096b5c);
    uint32 x_bits = r_u16(frame);
    if (lower_x < signed_x)
        selected_x = x_bits;
    sint32 upper_x = r_s16(0x80096b62);
    uint32 clamped_x = (uint32)upper_x;
    if ((sint16)(selected_x & 0xffff) < upper_x)
        clamped_x = selected_x;
    uint32 selected_y = r_u16(0x80096b5e);
    uint32 y_bits = r_u16(frame + 2);
    sint32 lower_y = r_s16(0x80096b5e);
    w_u16(frame, clamped_x);
    if (lower_y < (sint16)(y_bits & 0xffff))
        selected_y = y_bits;
    sint32 upper_y = r_s16(0x80096b64);
    uint32 clamped_y = (uint32)upper_y;
    if ((sint16)(selected_y & 0xffff) < upper_y)
        clamped_y = selected_y;
    uint32 selected_z = r_u16(0x80096b60);
    uint32 z_bits = r_u16(frame + 4);
    sint32 lower_z = r_s16(0x80096b60);
    w_u16(frame + 2, clamped_y);
    if (lower_z < (sint16)(z_bits & 0xffff))
        selected_z = z_bits;
    sint32 upper_z = r_s16(0x80096b66);
    uint32 clamped_z = (uint32)upper_z;
    if ((sint16)(selected_z & 0xffff) < upper_z)
        clamped_z = selected_z;
    w_u16(frame + 4, clamped_z);
    uint32 position_x = r_u16(position);
    w_u16(delta, clamped_x - position_x);
    uint32 local_y = r_u16(frame + 2);
    uint32 position_y = r_u16(position + 2);
    w_u16(delta + 2, local_y - position_y);
    uint32 local_z = r_u16(frame + 4);
    uint32 position_z = r_u16(position + 4);
    uint32 result = local_z - position_z;
    w_u16(delta + 4, result);
    return result;
}

uint32 G3Function80059624(uint32 position, uint32 state, uint32 delta, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80059624, "SLUS_008.06");
    uint32 mode = r_u32(0x800a258c);
    uint32 frame = guest_stack - 0x18;
    w_u16(delta, 0);
    w_u16(delta + 2, 0);
    w_u16(delta + 4, 0);
    uint32 flags = r_u32(0x800a24d0);
    uint32 game = r_u32(0x80097048);
    if ((flags & 0x400) != 0)
        return 0x400;
    uint32 controlled = r_u32(0x8009efc4);
    uint32 movement = r_u32(controlled + 0x150);
    if (movement == 1)
        return 1;
    uint32 game_flags = r_u32(game + 0x70);
    if ((game_flags & 0x400) == 0)
        w_u32(0x800a254c, 0);
    else
    {
        uint32 counter = r_u32(0x800a254c);
        if (counter == 0)
        {
            uint32 angle = r_u32(0x800a24f8);
            w_u32(0x800a2548, angle);
        }
        uint32 input_flags = r_u32(0x80096b28);
        uint32 incremented = counter + 1;
        w_u32(0x800a254c, incremented);
        if ((input_flags & 0x100) == 0 && (sint32)incremented >= 10)
        {
            uint32 current_mode = r_u32(0x800a258c);
            mode = current_mode != 1 ? 1 : 0;
        }
    }
    if (mode == 1)
        return G3Function80059740(position, state, delta, 4096, frame);
    if (mode == 0 || mode == 3)
        return G3Function80059AF8(position, state, delta, frame);
    return 3;
}

uint32 G3Function80059AF8(uint32 position, uint32 state, uint32 delta, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80059af8, "SLUS_008.06");
    return G3Function800599E0(position, state, delta, guest_stack - 0x18);
}

uint32 G3Function80059200(uint32 position, uint32 state, uint32 delta, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80059200, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    w_u16(delta, 0);
    w_u16(delta + 2, 0);
    w_u16(delta + 4, 0);
    G3Function80058D50(position, state, frame);
    uint32 state_x = r_u16(state);
    uint32 position_x = r_u16(position);
    w_u16(frame + 0x10, state_x - position_x);
    uint32 state_y = r_u16(state + 2);
    uint32 position_y = r_u16(position + 2);
    w_u16(frame + 0x12, state_y - position_y);
    uint32 state_z = r_u16(state + 4);
    uint32 position_z = r_u16(position + 4);
    w_u16(frame + 0x14, state_z - position_z);
    G3Function80058BA4(frame + 0x10, position, state, frame);
    uint32 controlled = r_u32(0x8009efc4);
    uint32 movement = G3Function80078834(controlled, frame);
    uint32 selection = 0;
    uint32 active = r_u32(state + 0x2c);
    uint32 flags;
    if (active == 0)
        flags = r_u32(0x800a24d0);
    else
    {
        flags = r_u32(0x800a24d0);
        if ((flags & 6) == 0 && (movement & 0xffff) != 0)
        {
            selection = (flags & 0x800) == 0 ? 1 : 0;
            flags = r_u32(0x800a24d0);
        }
    }
    if ((flags & 0x4000) != 0)
        selection = 2;
    if ((flags & 2) == 0)
    {
        uint32 current_active = r_u32(state + 0x2c);
        if (current_active != 0 && (flags & 1) != 0)
            w_u32(0x800a24d0, flags & 0xfffffffe);
    }
    uint32 state_height = r_u16(state + 0x10);
    uint32 tilt_height = r_u16(0x800a2524);
    uint32 offset = r_u16(0x800a2534);
    position_z = r_u16(position + 4);
    uint32 difference = state_height + tilt_height + offset - position_z;
    uint32 limit_bits = r_u16((movement & 0xffff) != 0 ? 0x8009edc8 : 0x8009edc4);
    if ((difference & 0xffff) != 0 && selection != 0)
    {
        uint32 previous = r_u16(0x800a252c);
        uint32 increment = r_u16(0x8009ede0);
        uint32 updated = previous + increment;
        w_u16(0x800a252c, updated);
        sint32 signed_updated = (sint16)(updated & 0xffff);
        sint32 signed_limit = (sint16)(limit_bits & 0xffff);
        uint32 selected = limit_bits;
        if (signed_updated > 0 ? signed_updated < signed_limit : signed_limit > 0)
        {
            sint32 current_speed = r_s16(0x800a252c);
            selected = current_speed < 0 ? 0 : (uint32)current_speed;
        }
        w_u16(0x800a252c, selected);
    }
    else
    {
        uint32 previous = r_u16(0x800a252c);
        sint32 increment = r_s32(0x8009ede0);
        sint32 reduced = (sint32)(previous << 16) >> 17;
        w_u16(0x800a252c, (uint32)reduced);
        if (reduced < increment)
            w_u16(0x800a252c, 0);
    }
    sint32 absolute_difference = (sint16)(difference & 0xffff);
    sint32 speed = r_s16(0x800a252c);
    if (absolute_difference < 0)
        absolute_difference = -absolute_difference;
    if (absolute_difference < speed && (movement & 0xffff) != 0)
    {
        uint32 current_flags = r_u32(0x800a24d0);
        w_u32(0x800a24d0, current_flags & 0xffffbfff);
    }
    if (selection != 0)
    {
        w_u16(delta + 4, difference);
        sint32 current_speed = r_s16(0x800a252c);
        if (current_speed != 0)
        {
            sint32 selected = -current_speed;
            sint32 signed_difference = (sint16)(difference & 0xffff);
            if (selected < signed_difference)
                selected = signed_difference;
            if (selected >= current_speed)
                selected = current_speed;
            w_u16(delta + 4, (uint32)selected);
        }
        uint32 current_flags = r_u32(0x800a24d0);
        w_u32(0x800a24d0, current_flags | 0x80000);
    }
    G3Function80058ED0(state, frame);
    uint32 result = (uint32)(sint32)(sint16)(movement & 0xffff);
    w_u32(0x800a2530, result);
    return result;
}
