#include "xport.h"
#include "psx.h"
#include "game_functions.h"

uint32 G3Function8007B108(uint32 player, uint32 object, uint32 value, uint32 animation, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007b108, "SLUS_008.06");
    uint32 resource = r_u32(player + 0x1c);
    sint32 limit = r_s16(resource + 0xa);
    uint32 current = r_u32(0x8009efc4);
    uint32 data = r_u32(current + 0x24);
    uint32 selected = (sint16)animation < limit ? animation : 0;
    if ((sint16)value < 0)
        return G3Function8007B188(player);
    uint32 flags = r_u32(current + 0x134) | 0x20;
    w_u32(current + 0x134, flags);
    w_u16(data + 0x238, value);
    w_u16(data + 0x23a, selected);
    w_u32(data + 0x23c, 0);
    w_u16(current + 0x5e, 0);
    w_u16(current + 0x6e, selected);
    return flags;
}

uint32 G3Function8007B188(uint32 player)
{
    FUNCTION_MARKER(0x8007b188, "SLUS_008.06");
    uint32 flags = r_u32(player + 0x134);
    uint32 data = r_u32(player + 0x24);
    flags &= 0xffffffdf;
    w_u32(player + 0x134, flags);
    w_u32(data + 0x234, 0);
    w_u32(data + 0x23c, 0);
    return flags;
}

uint32 G3Function800245E8(uint32 selection, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800245e8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 root;
    uint32 name;
    if (selection != 0)
    {
        root = r_u32(0x8009d668);
        name = r_u32(0x8009d66c);
    }
    else
    {
        uint32 game = r_u32(0x80097048);
        G3Function80023AAC(game, frame);
        root = r_u32(0x8009d684);
        name = r_u32(0x8009d688);
    }
    uint32 game = r_u32(0x80097048);
    return G3Function80027644(root, name, game, frame);
}

uint32 G3Function80027C48(uint32 owner, uint32 offset)
{
    FUNCTION_MARKER(0x80027c48, "SLUS_008.06");
    uint32 packet = r_u32(owner + 4);
    uint32 result = r_u32(packet);
    uint32 sentinel = owner & 0xffffff;
    uint32 next = result & 0xffffff;
    while (next != sentinel)
    {
        uint32 adjusted = next + offset;
        uint32 header = r_u32(packet);
        w_u32(packet, (header & 0xff000000) | (adjusted & 0xffffff));
        result = r_u32(adjusted);
        packet = adjusted;
        next = result & 0xffffff;
    }
    return result;
}

void G3Function80028524(void)
{
    FUNCTION_MARKER(0x80028524, "SLUS_008.06");
    PadStopCom();
}

void G3Function8002E218(void)
{
    FUNCTION_MARKER(0x8002e218, "SLUS_008.06");
}

uint32 G3Function80027B4C(uint32 owner, uint32 current, uint32 stop, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80027b4c, "SLUS_008.06");
    G3Function800866D8(owner, 2);
    uint32 boundary = (r_u32(owner) & 0xffffff) | 0x80000000;
    uint32 tail = owner + 4;
    bool gap = true;
    while (current != boundary)
    {
        if (current == stop)
            break;
        uint32 header = r_u32(current);
        if ((header & 0xff000000) != 0)
        {
            if (gap)
            {
                uint32 previous = r_u32(tail);
                w_u32(tail, (previous & 0xff000000) | (current & 0xffffff));
            }
            gap = false;
            tail = current;
        }
        else
            gap = true;
        current = (r_u32(current) & 0xffffff) | 0x80000000;
    }
    uint32 result = owner & 0xffffff;
    uint32 header = r_u32(tail);
    w_u32(tail, (header & 0xff000000) | result);
    return result;
}

uint32 G3Function80027CA4(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80027ca4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 game = r_u32(0x80097048);
    uint32 page = 1 - r_u32(game + 0x58);
    uint32 result = r_u32(game + 0x6834) & 0x400000;
    if (result != 0)
        return result;
    (uint32)DrawSync(0);
    uint32 first = r_u32(0x800a0c7c);
    uint32 second = r_u32(0x800a0c80);
    game = r_u32(0x80097048);
    w_u32(0x800a0c30, first);
    w_u32(0x800a0c34, second);
    uint32 count = r_u32(game + 0x28);
    uint32 table = r_u32(game + 0x3c);
    table += (page * (count + 0x80)) << 2;
    G3Function80027B4C(game + 0x40, table + (count << 2) + 0x1fc, table + 0x200, frame);
    uint32 words;
    if (page == 0)
    {
        first = r_u32(0x800a0c7c);
        second = r_u32(0x800a0c80);
        uint32 end = r_u32(second + 4);
        words = (uint32)((sint32)(end - 0xc - second) >> 2);
        memmovePSX(first + 0xc, second + 0xc, (sint32)(words << 2));
        first = r_u32(0x800a0c7c);
        game = r_u32(0x80097048);
        second = r_u32(0x800a0c80);
        G3Function80027C48(game + 0x40, first - second);
    }
    else
    {
        first = r_u32(0x800a0c7c);
        uint32 end = r_u32(first + 4);
        words = (uint32)((sint32)(end - 0xc - first) >> 2);
    }
    first = r_u32(0x800a0c7c);
    game = r_u32(0x80097048);
    first += (words << 2) + 0xc;
    w_u32(0x800a0c7c, first);
    uint32 size = r_u32(game + 0x6c);
    uint32 offset = (uint32)((sint32)((size << 1) - words) >> 1);
    offset = (offset << 2) - 0x24;
    second = first + offset;
    w_u32(0x800a0c80, second);
    w_u32(first + 8, second);
    first = r_u32(0x800a0c7c);
    w_u32(first + 4, first + 0xc);
    first = r_u32(0x800a0c7c);
    w_u32(first, 0);
    second = r_u32(0x800a0c80);
    w_u32(second + 8, second + offset);
    second = r_u32(0x800a0c80);
    w_u32(second + 4, second + 0xc);
    second = r_u32(0x800a0c80);
    w_u32(second, 0);
    uint32 selected = r_u32(0x800a0c7c + (page << 2));
    w_u32(game + 0x68, selected);
    result = r_u32(game + 0x6834) | 0x400000;
    w_u32(game + 0x6834, result);
    return result;
}

uint32 G3Function80027E6C(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80027e6c, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 result = r_u32(game + 0x6834) & 0x400000;
    if (result == 0)
        return 0;
    (uint32)DrawSync(0);
    game = r_u32(0x80097048);
    G3Function800866D8(game + 0x40, 2);
    uint32 first = r_u32(0x800a0c30);
    uint32 second = r_u32(0x800a0c34);
    game = r_u32(0x80097048);
    w_u32(0x800a0c7c, first);
    w_u32(0x800a0c80, second);
    w_u32(second, 0);
    w_u32(first, 0);
    uint32 size = r_u32(game + 0x6c);
    first = r_u32(0x800a0c7c);
    w_u32(first + 8, first + (size << 2) + 0xc);
    first = r_u32(0x800a0c7c);
    w_u32(first + 4, first + 0xc);
    size = r_u32(game + 0x6c);
    second = r_u32(0x800a0c80);
    w_u32(second + 8, second + (size << 2) + 0xc);
    second = r_u32(0x800a0c80);
    w_u32(second + 4, second + 0xc);
    uint32 page = 1 - r_u32(game + 0x58);
    uint32 flags = r_u32(game + 0x6834);
    result = r_u32(0x800a0c7c + (page << 2));
    w_u32(game + 0x6834, flags & 0xffbfffff);
    w_u32(game + 0x68, result);
    return result;
}

void G3Function8002ED54(void)
{
    FUNCTION_MARKER(0x8002ed54, "SLUS_008.06");
    DrawSyncCallback(0);
    VSyncCallback(0);
    CdSyncCallback(0);
    CdReadyCallback(0);
    G3EnterCritical8008A05C();
    G3StopRootCounter8008963C(0xf2000000);
    uint32 handle = r_u32(0x800a3d90);
    DisableEvent((sint32)handle);
    handle = r_u32(0x800a3d90);
    CloseEvent((sint32)handle);
    G3ExitCritical8008A03C();
}

uint32 G3Function8002F820(uint32 vector, uint32 angle)
{
    FUNCTION_MARKER(0x8002f820, "SLUS_008.06");
    uint32 x = r_u16(vector);
    uint32 z = r_u16(vector + 4);
    uint32 signed_angle = (uint32)(sint32)(sint16)angle;
    uint32 sine = G3Function8005B590(signed_angle);
    uint32 cosine = G3Function8005B470(signed_angle);
    x = (uint32)(sint32)(sint16)x;
    z = (uint32)(sint32)(sint16)z;
    sine = (uint32)(sint32)(sint16)sine;
    cosine = (uint32)(sint32)(sint16)cosine;
    uint32 first = cosine * x + sine * z;
    uint32 second = cosine * z - sine * x;
    w_u16(vector, (uint32)((sint32)first >> 12));
    uint32 result = (uint32)((sint32)second >> 12);
    w_u16(vector + 4, result);
    return result;
}

uint32 G3Function8002FDE4(uint32 vector, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002fde4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 first = r_u32(vector);
    uint32 square = first * first;
    uint32 second = r_u32(vector + 4);
    uint32 value = (square + second * second) << 4;
    uint32 result = G3Function8002F448(value, 4, frame);
    return (uint32)((sint32)(result + 8) >> 4);
}

uint32 G3Function8002FE30(uint32 left, uint32 right, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002fe30, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    for (uint32 component = 0; component < 3; ++component)
    {
        uint32 first = (uint32)r_s16(left + component * 2);
        uint32 second = (uint32)r_s16(right + component * 2);
        w_u32(frame + 0x10 + component * 4, first - second);
    }
    return G3Function8002FBF8(frame + 0x10, frame);
}

uint32 G3Function8002FE88(uint32 left, uint32 right, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002fe88, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    for (uint32 component = 0; component < 3; ++component)
    {
        uint32 first = (uint32)r_s16(left + component * 2);
        uint32 second = (uint32)r_s16(right + component * 2);
        w_u32(frame + 0x10 + component * 4, first - second);
    }
    return G3Function8002FD3C(frame + 0x10);
}

uint32 G3Function80036168(uint32 object)
{
    FUNCTION_MARKER(0x80036168, "SLUS_008.06");
    uint32 flags = r_u16(object + 6);
    if (((0x1040104 >> (flags & 0x1f)) & 1) != 0)
        return 1;
    if ((flags & 1) == 0)
        return 0;
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    uint32 table = r_u32(level + 0xf0);
    return r_u32(table + ((flags & 0x1e0) >> 1)) & 1;
}

void G3Function800361D4(void)
{
    FUNCTION_MARKER(0x800361d4, "SLUS_008.06");
}

uint32 G3Function800361DC(uint32 object, uint32 animation, uint32 frame_value, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800361dc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function80037348(object, animation, frame_value, frame);
    return 1;
}

uint32 G3Function80037348(uint32 object, uint32 animation, uint32 frame_value, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80037348, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (object == 0)
        return 0xffffffff;
    uint32 resource = r_u32(object + 0x1c);
    sint32 limit = r_s16(resource + 0xa);
    if ((sint32)animation >= limit)
        return 0xffffffff;
    uint32 result = animation & 0xffff;
    w_u16(object + 0x6e, animation);
    uint32 flags = r_u32(object + 0x14);
    w_u32(object + 0x14, flags & 0xffffffef);
    G3Function80037258(object, frame_value, 0, frame);
    return result;
}

uint32 G3Function80037258(uint32 object, uint32 requested, uint32 repeat, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80037258, "SLUS_008.06");
    if (object == 0)
        return 0xffffffff;
    uint32 result = requested;
    uint32 length = G3Function800371B8(object);
    if (requested == 0xffffffff || length == 0xffffffff)
        return result;
    uint32 current = (uint32)r_s16(object + 0x5e);
    if (current == requested && requested == length - 1)
    {
        uint32 flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags | 0x10);
    }
    if ((sint32)requested >= (sint32)length)
    {
        uint32 flags = r_u32(object + 0x14);
        result = length - 1;
        w_u32(object + 0x14, flags | 0x10);
    }
    if (repeat != 0)
    {
        uint32 flags = r_u32(object + 0x14);
        if ((flags & 0x10) != 0)
        {
            w_u32(object + 0x14, flags & 0xffffffef);
            result = 0;
            requested = 0;
        }
    }
    if ((sint32)requested < (sint32)length)
        w_u16(object + 0x5e, result);
    return result;
}

uint32 G3Function800371B8(uint32 object)
{
    FUNCTION_MARKER(0x800371b8, "SLUS_008.06");
    if (object == 0)
        return 0xffffffff;
    uint32 resource = r_u32(object + 0x1c);
    if (resource == 0)
        return 0xffffffff;
    uint32 animation = r_u16(object + 0x6e);
    sint32 limit = r_s16(resource + 0xa);
    if ((sint32)animation >= limit)
        return 0xffffffff;
    uint32 table = r_u32(resource + 0x10);
    if (table == 0)
        return 0xffffffff;
    uint32 data = r_u32(table + (animation << 2));
    return (uint32)r_s16(data + 2);
}

uint32 G3Function80037210(uint32 object)
{
    FUNCTION_MARKER(0x80037210, "SLUS_008.06");
    if (object == 0)
        return 0xffffffff;
    uint32 next = (uint32)r_s16(object + 0x5e) + 1;
    uint32 length = G3Function800371B8(object);
    return (sint32)next < (sint32)length ? next : length - 1;
}

uint32 G3Function800373AC(uint32 object, uint32 repeat, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800373ac, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    if (object == 0)
        return 0xffffffff;
    uint32 next = G3Function80037210(object);
    G3Function80037258(object, next, repeat, frame);
    uint32 resource = r_u32(object + 0x1c);
    uint32 flags = r_u32(resource + 0x30);
    if ((flags & 0x2000) != 0)
        G3Function8003E314(object, frame);
    return next;
}

uint32 G3Function80038864(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80038864, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 flags = r_u32(object + 0x18);
    w_u32(object + 0x18, flags | 0x200);
    uint32 carry = G3Function800376A4(object, frame);
    uint32 linked = r_u32(object + 0x20);
    if (linked != 0)
    {
        carry = r_u32(linked + 0x18) | 0x10;
        w_u32(linked + 0x18, carry);
    }
    G3Function800374FC(object, carry, frame);
    return G3Function800376D8(object, frame);
}

uint32 G3Function80036908(uint32 value, uint32 angle)
{
    FUNCTION_MARKER(0x80036908, "SLUS_008.06");
    sint32 signed_angle = (sint16)angle;
    if (signed_angle == 0)
        return (uint32)(sint32)(sint16)value;
    if (signed_angle < 0)
        signed_angle = -signed_angle;
    signed_angle = (sint16)signed_angle;
    if (signed_angle >= 0x401)
        return 0;
    sint32 divisor = (sint32)G3Function8005B590((uint32)signed_angle);
    sint32 numerator = (sint32)(value << 16) >> 4;
    sint32 quotient = divisor == 0 ? (numerator < 0 ? 1 : -1) : numerator / divisor;
    return (uint32)quotient * (uint32)quotient;
}

uint32 G3Function80039C48(uint32 object, uint32 radius, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80039c48, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    for (uint32 phase = 0; phase < 4; ++phase)
    {
        if (phase == 3)
        {
            sint32 signed_radius = (sint16)radius;
            uint32 span = signed_radius < 0x80 ? 0x80 : (uint32)signed_radius;
            uint32 height = (uint32)r_s16(frame + 0x14);
            w_u16(frame + 0x14, height + span);
            height = (uint32)r_s16(frame + 0x1c);
            w_u16(frame + 0x1c, height - span);
            uint32 game = r_u32(0x80097048);
            uint32 level = r_u32(game + 0x48);
            uint32 terrain = r_u32(level);
            uint32 polygon = G3Function800622EC(terrain, frame + 0x18, frame + 0x10, object, frame);
            uint32 end_height = r_u16(frame + 0x1c);
            uint32 object_height = r_u16(object + 0x5c);
            sint32 difference = (sint16)(end_height - object_height);
            if (difference < 0)
                difference = -difference;
            if (signed_radius < difference || polygon == 0)
                return 0;
            if (G3Function80036168(polygon) != 0)
                return 0;
            if (output == 0)
                return 1;
        }
        if (phase == 2 && output == 0)
            continue;
        uint32 source = phase == 0 ? object + 0x58 : frame + 0x18;
        uint32 destination = phase == 0 ? frame + 0x18 : (phase == 1 ? frame + 0x10 : output);
        uint32 words[2];
        for (uint32 index = 0; index < 2; ++index)
        {
            uint32 position = source + index * 4;
            uint32 high_address = position + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = position & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(position & 0xfffffffc, (15 << low_lane) & 15);
            words[index] = (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 index = 0; index < 2; ++index)
        {
            uint32 position = destination + index * 4;
            uint32 high_address = position + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = position & 3;
            w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            w_masked_u32(position & 0xfffffffc, words[index] << (low_lane * 8), (15 << low_lane) & 15);
        }
    }
    return 1;
}

uint32 G3Function8003D6F0(uint32 object, uint32 data, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003d6f0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 flags = r_u32(object + 0x14);
    if ((flags & 0x400) == 0)
        return 0;
    w_u32(object + 0x14, flags & 0xfffffbff);
    w_u16(object + 0x4e, 4);
    uint32 variance = r_u8(data + 7);
    uint32 volume = r_u8(data + 6);
    if (variance != 0)
    {
        sint32 random = (sint32)G3Function80027FDC();
        sint32 divisor = (sint32)(r_u8(data + 7) << 1);
        sint32 remainder = divisor == 0 ? random : random % divisor;
        uint32 offset = r_u8(data + 7);
        volume += offset - (uint32)remainder;
    }
    variance = r_u16(data + 0xa);
    uint32 pitch = r_u16(data + 8);
    if (variance != 0)
    {
        sint32 random = (sint32)G3Function80027FDC();
        uint32 offset = r_u16(data + 0xa);
        sint32 divisor = (sint32)(offset << 1);
        sint32 remainder = divisor == 0 ? random : random % divisor;
        pitch += offset - (uint32)remainder;
    }
    uint32 radius = (uint32)r_s16(data + 4);
    uint32 sound = r_u16(data + 2);
    w_u32(frame + 0x10, radius);
    return G3Function8003E518(object, sound, (uint32)(sint32)(sint16)pitch, (uint32)(sint32)(sint16)volume, frame);
}

uint32 G3Function8003D7F4(uint32 object, uint32 data, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003d7f4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 flags = r_u32(object + 0x14);
    if ((flags & 0x400000) == 0)
        return 0;
    w_u32(object + 0x14, flags & 0xffbfffff);
    w_u16(object + 0x4e, 4);
    uint32 variance = r_u8(data + 7);
    uint32 volume = r_u8(data + 6);
    if (variance != 0)
    {
        sint32 random = (sint32)G3Function80027FDC();
        sint32 divisor = (sint32)(r_u8(data + 7) << 1);
        sint32 remainder = divisor == 0 ? random : random % divisor;
        uint32 offset = r_u8(data + 7);
        volume += offset - (uint32)remainder;
    }
    variance = r_u16(data + 0xa);
    uint32 pitch = r_u16(data + 8);
    if (variance != 0)
    {
        sint32 random = (sint32)G3Function80027FDC();
        uint32 offset = r_u16(data + 0xa);
        sint32 divisor = (sint32)(offset << 1);
        sint32 remainder = divisor == 0 ? random : random % divisor;
        pitch += offset - (uint32)remainder;
    }
    uint32 radius = (uint32)r_s16(data + 4);
    uint32 sound = r_u16(data + 2);
    w_u32(frame + 0x10, radius);
    return G3Function8003E518(object, sound, (uint32)(sint32)(sint16)pitch, (uint32)(sint32)(sint16)volume, frame);
}

void G3Function8003E30C(void)
{
    FUNCTION_MARKER(0x8003e30c, "SLUS_008.06");
}

void G3Function8003EB28(void)
{
    FUNCTION_MARKER(0x8003eb28, "SLUS_008.06");
}

uint32 G3Function8003ECE0(void)
{
    FUNCTION_MARKER(0x8003ece0, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    w_u8(game + 0x67df, 0);
    return G3Function800487DC();
}

uint32 G3Function8003EEAC(uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003eeac, "SLUS_008.06");
    while (SpuIsTransferCompleted(0) == 0)
    {
    }
    G3Function80049144();
    return G3Function80049A60();
}

uint32 G3Function8003EEE4(uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003eee4, "SLUS_008.06");
    while (SpuIsTransferCompleted(0) == 0)
    {
    }
    G3Function800490BC();
    return G3Function80049B44();
}

uint32 G3Function8003EF64(uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003ef64, "SLUS_008.06");
    G3Function8003EF1C();
    return G3Function800490BC();
}

uint32 G3Function80049A60(void)
{
    FUNCTION_MARKER(0x80049a60, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 flags = r_u16(audio + 0x12);
    if ((flags & 8) != 0)
        return 0;
    w_u16(audio + 0x12, flags | 0xc);
    for (uint32 index = 0; index < 24; ++index)
    {
        audio = r_u32(0x80098ff0);
        uint32 voice = audio + 0x248 + index * 0x1c;
        flags = r_u16(voice + 0x12);
        if ((flags & 1) != 0)
            continue;
        w_u16(voice + 0x12, flags & 0xfffd);
        SpuVoiceAttr attributes = {0};
        attributes.voice = 1 << index;
        attributes.mask = SPU_VOICE_PITCH;
        SpuGetVoiceAttr(&attributes);
        w_u16(audio + 0x500 + index * 2, attributes.pitch);
        SpuSetVoicePitch((sint32)index, 0);
    }
    return 0;
}

uint32 G3Function80049B44(void)
{
    FUNCTION_MARKER(0x80049b44, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 flags = r_u16(audio + 0x12);
    if ((flags & 8) == 0)
        return 0;
    w_u16(audio + 0x12, flags & 0xfff3);
    for (uint32 index = 0; index < 24; ++index)
    {
        audio = r_u32(0x80098ff0);
        uint32 voice_flags = audio + 0x25a + index * 0x1c;
        if ((r_u16(voice_flags) & 1) != 0)
            continue;
        if ((r_u16(voice_flags) & 2) != 0)
            continue;
        uint32 pitch = r_u16(audio + 0x500 + index * 2);
        SpuSetVoicePitch((sint32)index, (uint16)pitch);
    }
    return 0;
}

void G3Function80049DF4(void)
{
    FUNCTION_MARKER(0x80049df4, "SLUS_008.06");
}

void G3Function8004AAFC(void)
{
    FUNCTION_MARKER(0x8004aafc, "SLUS_008.06");
}

uint32 G3Function8004AB04(uint32 command, uint32 bank)
{
    FUNCTION_MARKER(0x8004ab04, "SLUS_008.06");
    uint32 channel = r_u8(command + 4);
    uint32 index = r_u8(command + 6);
    channel &= 0xf;
    if (index >= 0x10)
        return 0x800a0000;
    uint32 audio = r_u32(0x80098ff0);
    uint32 table = r_u32(audio + 0x53c);
    uint32 value = r_u8(table + index + 0x50);
    uint32 result = bank + channel;
    w_u8(result + 0x136, value);
    return result;
}

uint32 G3Function8004AB98(uint32 command, uint32 bank)
{
    FUNCTION_MARKER(0x8004ab98, "SLUS_008.06");
    uint32 channel = r_u8(command + 4);
    uint32 value = r_u8(command + 6);
    channel &= 0xf;
    w_u8(bank + channel + 0x156, value);
    uint32 flags = r_u16(bank + 0xf8);
    if (((flags >> channel) & 1) == 0)
        return 0;
    return G3Function8004A184(bank, channel);
}

void G3Function8004AC30(void)
{
    FUNCTION_MARKER(0x8004ac30, "SLUS_008.06");
}

uint32 G3Function8004AD00(uint32 command, uint32 bank)
{
    FUNCTION_MARKER(0x8004ad00, "SLUS_008.06");
    uint32 value = r_u8(command + 5);
    if (value >= 0x10)
        return 0;
    w_u8(bank + 0xed, value);
    return 1;
}

uint32 G3Function8004AEC4(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004aec4, "SLUS_008.06");
    uint32 index = r_u8(context + 0xec);
    return G3Function800493C0(index);
}

uint32 G3Function8004AEE8(uint32 command, uint32 context, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004aee8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 index = r_u8(context + 0xec);
    return G3Function80049578(index, frame);
}

uint32 G3Function8004AF0C(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004af0c, "SLUS_008.06");
    uint32 index = r_u8(context + 0xec);
    return G3Function800495CC(index);
}

uint32 G3Function8004AF74(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004af74, "SLUS_008.06");
    uint32 value = r_u8(command + 5);
    uint32 bank = r_u32(context + 0xe8);
    w_u8(bank + 0xfb, value);
    bank = r_u32(context + 0xe8);
    return G3Function8004A504(bank);
}

uint32 G3Function8004AFA8(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004afa8, "SLUS_008.06");
    uint32 bank = r_u32(context + 0xe8);
    uint32 channel = r_u8(context + 0xed);
    uint32 value = r_u8(command + 5);
    w_u8(bank + channel + 0x146, value);
    bank = r_u32(context + 0xe8);
    channel = r_u8(context + 0xed);
    return G3Function8004A184(bank, channel);
}

uint32 G3Function8004AFE4(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004afe4, "SLUS_008.06");
    uint32 bank = r_u32(context + 0xe8);
    uint32 channel = r_u8(context + 0xed);
    uint32 value = r_u8(command + 5);
    w_u8(bank + channel + 0x156, value);
    bank = r_u32(context + 0xe8);
    channel = r_u8(context + 0xed);
    return G3Function8004A184(bank, channel);
}

uint32 G3Function8004B020(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b020, "SLUS_008.06");
    uint32 bank = r_u32(context + 0xe8);
    uint32 channel = r_u8(context + 0xed);
    uint32 flags = r_u16(bank + 0xf8);
    uint32 result = flags | (uint32(1) << (channel & 31));
    w_u16(bank + 0xf8, result);
    return result;
}

uint32 G3Function8004B040(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b040, "SLUS_008.06");
    uint32 channel = r_u8(context + 0xed);
    uint32 bank = r_u32(context + 0xe8);
    uint32 flags = r_u16(bank + 0xf8);
    uint32 result = flags & ~(uint32(1) << (channel & 31));
    w_u16(bank + 0xf8, result);
    return result;
}

uint32 G3Function8004B064(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b064, "SLUS_008.06");
    uint32 channel = r_u8(context + 0xed);
    uint32 index = r_u8(context + 0xec);
    uint32 mask = (uint32(1) << (channel & 31)) & 0xffff;
    return G3Function80049810(index, mask);
}

uint32 G3Function8004B094(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b094, "SLUS_008.06");
    uint32 channel = r_u8(context + 0xed);
    uint32 index = r_u8(context + 0xec);
    uint32 mask = (uint32(1) << (channel & 31)) & 0xffff;
    return G3Function80049900(index, mask);
}

uint32 G3Function8004B0C4(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b0c4, "SLUS_008.06");
    uint32 high = r_u8(command + 6);
    uint32 low = r_u8(command + 5);
    uint32 index = r_u8(context + 0xec);
    return G3Function80049810(index, low | (high << 8));
}

uint32 G3Function80049578(uint32 index, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80049578, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 count = r_u8(audio + 4);
    index &= 0xffff;
    if (index < count)
    {
        uint32 bank = r_u32(audio + 0x20 + (index << 2));
        uint32 flags = r_u16(bank + 0xf2);
        w_u16(bank + 0xf2, flags & 0xfffe);
        G3Function800496BC(index);
    }
    return 0;
}

uint32 G3Function800495CC(uint32 index)
{
    FUNCTION_MARKER(0x800495cc, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 count = r_u8(audio + 4);
    index &= 0xffff;
    if (index >= count)
        return 0;
    uint32 bank = r_u32(audio + 0x20 + (index << 2));
    if (r_u16(bank + 0xee) == 0xff)
        return 0;
    uint32 flags4 = r_u16(bank + 0xf4);
    uint32 flags2 = r_u16(bank + 0xf2);
    w_u16(bank + 0xf4, flags4 | 1);
    w_u16(bank + 0xf2, flags2 | 1);
    return 0;
}

uint32 G3Function80049810(uint32 index, uint32 mask)
{
    FUNCTION_MARKER(0x80049810, "SLUS_008.06");
    uint32 selected = mask & 0xffff;
    uint32 audio = r_u32(0x80098ff0);
    uint32 bank = r_u32(audio + 0x20 + ((index & 0xffff) << 2));
    uint32 flags = r_u16(bank + 0xf6);
    w_u16(bank + 0xf6, flags | mask);
    uint32 voices = 0;
    for (uint32 channel = 0; channel < 16; ++channel)
    {
        if (((selected >> channel) & 1) == 0)
            continue;
        for (uint32 voice_index = 0; voice_index < 24; ++voice_index)
        {
            audio = r_u32(0x80098ff0);
            uint32 voice = audio + 0x248 + voice_index * 0x1c;
            uint32 bank_flags = r_u8(bank + 0xfd);
            uint32 voice_flags = r_u8(voice + 8);
            if (voice_flags == (bank_flags | channel))
            {
                uint32 voice_mask = r_u32(voice);
                w_u8(voice + 8, 0xff);
                voices |= voice_mask;
            }
        }
    }
    if (voices != 0)
    {
        audio = r_u32(0x80098ff0);
        flags = r_u32(audio + 0x530);
        w_u32(audio + 0x530, flags | voices);
        flags = r_u32(audio + 0x534);
        w_u32(audio + 0x534, flags & ~voices);
    }
    return 0;
}

uint32 G3Function80049900(uint32 index, uint32 mask)
{
    FUNCTION_MARKER(0x80049900, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 bank = r_u32(audio + 0x20 + ((index & 0xffff) << 2));
    uint32 flags = r_u16(bank + 0xf6);
    w_u16(bank + 0xf6, flags & ~mask);
    return 0;
}

uint32 G3Function8003DA70(uint32 object, uint32 data, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003da70, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 flags = r_u32(object + 0x14);
    uint32 selector = (flags >> 18) & 1;
    if ((flags & 0x10000) != 0)
    {
        uint32 handle = r_u32(object + 0x50);
        if (G3Function8003F0A8(handle) != 0)
        {
            handle = r_u32(object + 0x50);
            if (G3Function8003F088(handle) == 0)
                return 0;
            flags = r_u32(object + 0x14);
            if ((flags & 0x8000) == 0)
            {
                uint32 result = flags | 0x8000;
                w_u32(object + 0x14, result);
                return result;
            }
            w_u32(object + 0x14, flags & 0xffff7fff);
            uint32 pitch = (uint32)r_s16(data + 0xe + selector * 2);
            uint32 radius = (uint32)r_s16(data + 6 + selector * 2);
            uint32 volume = r_u8(data + 0xa + selector);
            w_u32(frame + 0x10, radius);
            handle = r_u32(object + 0x50);
            uint32 result = G3Function8003E7E4(object, handle, pitch, volume, frame);
            if (result != 0)
                return result;
            handle = r_u32(object + 0x50);
            G3Function8003F18C(handle);
        }
        flags = r_u32(object + 0x14);
        uint32 result = flags & 0xfffcffff;
        w_u32(object + 0x14, result);
        return result;
    }
    if ((flags & 0x20000) == 0)
    {
        w_u32(object + 0x14, flags | 0x20000);
        uint32 delay = r_u8(data + 0xc + selector);
        w_u32(object + 0x54, delay);
        return delay;
    }
    if ((flags & 0x1000) == 0)
        return 0;
    uint32 delay = r_u32(object + 0x54);
    if (delay != 0)
    {
        uint32 result = delay - 1;
        w_u32(object + 0x54, result);
        return result;
    }
    uint32 sound = r_u16(data + 2 + selector * 2);
    uint32 pitch = (uint32)r_s16(data + 0xe + selector * 2);
    uint32 volume = r_u8(data + 0xa + selector);
    uint32 radius = (uint32)r_s16(data + 6 + selector * 2);
    w_u32(frame + 0x10, radius);
    uint32 result = G3Function8003E518(object, sound, pitch, volume, frame);
    w_u32(object + 0x50, result);
    if (result != 0)
    {
        flags = r_u32(object + 0x14);
        result = ((flags | 0x10000) & 0xffffefff) ^ 0x40000;
        w_u32(object + 0x14, result);
    }
    return result;
}

uint32 G3Function8003DE64(uint32 object, uint32 data, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003de64, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 flags = r_u32(object + 0x14);
    uint32 request = data + 2;
    if ((flags & 0x2000) != 0)
    {
        uint32 handle = r_u32(object + 0x44);
        if (G3Function8003F0A8(handle) == 0)
        {
            flags = r_u32(object + 0x14);
            w_u32(object + 0x14, flags & 0xffffdfff);
        }
        else
        {
            handle = r_u32(object + 0x44);
            if (G3Function8003F088(handle) != 0)
            {
                flags = r_u32(object + 0x14);
                if ((flags & 0x8000) == 0)
                    w_u32(object + 0x14, flags | 0x8000);
                else
                {
                    w_u32(object + 0x14, flags & 0xffff7fff);
                    uint32 radius = (uint32)r_s16(request + 2);
                    uint32 pitch = (uint32)r_s16(object + 0x4a);
                    uint32 volume = (uint32)r_s16(object + 0x48);
                    w_u32(frame + 0x10, radius);
                    handle = r_u32(object + 0x44);
                    if (G3Function8003E7E4(object, handle, pitch, volume, frame) == 0)
                    {
                        flags = r_u32(object + 0x14);
                        handle = r_u32(object + 0x44);
                        w_u32(object + 0x14, flags & 0xffffdfff);
                        G3Function8003F18C(handle);
                        w_u32(object + 0x44, 0);
                    }
                }
            }
        }
    }
    sint32 timer = r_s16(object + 0x4c);
    uint32 unsigned_timer = r_u16(object + 0x4c);
    uint32 result = unsigned_timer - 1;
    if (timer != 0)
    {
        w_u16(object + 0x4c, result);
        return result;
    }
    uint32 threshold = r_u16(request + 0xc);
    if (threshold >= 0x2711)
    {
        uint32 sound = r_u16(request);
        result = G3Function8003F108(sound);
        threshold = r_u16(request + 0xc) - 0x2710;
        result = (sint32)result < (sint32)threshold ? 1 : 0;
        if (result == 0)
            return result;
    }
    uint32 handle = r_u32(object + 0x44);
    result = G3Function8003F0A8(handle);
    if (result != 0)
        return result;
    uint32 volume = r_u8(request + 4);
    w_u16(object + 0x48, volume);
    if (r_u8(request + 5) != 0)
    {
        result = G3Function80027FDC();
        uint32 divisor = r_u8(request + 5) << 1;
        uint32 remainder = divisor == 0 ? result : (uint32)((sint32)result % (sint32)divisor);
        uint32 amplitude = r_u8(request + 5);
        volume = r_u16(object + 0x48);
        w_u16(object + 0x48, volume + amplitude - remainder);
    }
    uint32 pitch = r_u16(request + 6);
    w_u16(object + 0x4a, pitch);
    if (r_u16(request + 8) != 0)
    {
        result = G3Function80027FDC();
        uint32 amplitude = r_u16(request + 8);
        uint32 divisor = amplitude << 1;
        uint32 remainder = divisor == 0 ? result : (uint32)((sint32)result % (sint32)divisor);
        pitch = r_u16(object + 0x4a);
        w_u16(object + 0x4a, pitch + amplitude - remainder);
    }
    uint32 sound = r_u16(request);
    pitch = (uint32)r_s16(object + 0x4a);
    volume = (uint32)r_s16(object + 0x48);
    uint32 radius = (uint32)r_s16(request + 2);
    w_u32(frame + 0x10, radius);
    result = G3Function8003E518(object, sound, pitch, volume, frame);
    w_u32(object + 0x44, result);
    if (result == 0)
        return result;
    flags = r_u32(object + 0x14);
    w_u32(object + 0x14, flags | 0x2000);
    result = r_u16(request + 0xc);
    uint32 delay = r_u16(request + 0xa);
    if (result != 0)
    {
        result = G3Function80027FDC();
        uint32 amplitude = r_u16(request + 0xc);
        uint32 divisor = amplitude << 1;
        uint32 remainder = divisor == 0 ? result : (uint32)((sint32)result % (sint32)divisor);
        delay += amplitude - remainder;
    }
    w_u16(object + 0x4c, delay);
    return result;
}

uint32 G3Function80049534(uint32 index)
{
    FUNCTION_MARKER(0x80049534, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 count = r_u8(audio + 4);
    index &= 0xffff;
    if (index < count)
    {
        uint32 bank = r_u32(audio + 0x20 + index * 4);
        uint32 flags = r_u16(bank + 0xf4);
        w_u16(bank + 0xf4, flags & 0xffdf);
    }
    return 0;
}

uint32 G3Function800494E0(uint32 index)
{
    FUNCTION_MARKER(0x800494e0, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 count = r_u8(audio + 4);
    index &= 0xffff;
    if (index < count)
    {
        uint32 bank = r_u32(audio + 0x20 + index * 4);
        uint32 flags = r_u16(bank + 0xf4);
        w_u16(bank + 0xf4, flags | 0x20);
        G3Function800496BC(index);
    }
    return 0;
}

uint32 G3Function8003F004(uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003f004, "SLUS_008.06");
    for (uint32 index = 2; index < 4; ++index)
        G3Function80049534(index);
    return 0;
}

uint32 G3Function8003F040(uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003f040, "SLUS_008.06");
    G3Function800494E0(1);
    return G3Function8004BC18();
}

uint32 G3Function8003F068(uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003f068, "SLUS_008.06");
    return G3Function80049534(1);
}

uint32 G3Function80049628(uint32 index)
{
    FUNCTION_MARKER(0x80049628, "SLUS_008.06");
    index &= 0xffff;
    uint32 audio = r_u32(0x80098ff0);
    uint32 bank = r_u32(audio + 0x20 + index * 4);
    return r_u16(bank + 0xf2);
}

uint32 G3Function8004B0F4(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b0f4, "SLUS_008.06");
    uint32 low_index = r_u8(command + 5);
    uint32 high_index = r_u8(command + 6);
    if (low_index >= 128)
        return 0;
    if (high_index >= 128)
        return 0x800a0000;
    uint32 audio = r_u32(0x80098ff0);
    uint32 index = r_u8(context + 0xec);
    uint32 high = r_u8(audio + 0x564 + high_index);
    uint32 low = r_u8(audio + 0x564 + low_index);
    uint32 mask = low | (high << 8);
    G3Function80049900(index, (~mask) & 0xffff);
    index = r_u8(context + 0xec);
    return G3Function80049810(index, mask);
}

uint32 G3Function8004B174(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b174, "SLUS_008.06");
    uint32 high = r_u8(command + 6);
    uint32 low = r_u8(command + 5);
    uint32 index = r_u8(context + 0xec);
    return G3Function80049900(index, low | (high << 8));
}

uint32 G3Function8003EF8C(uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003ef8c, "SLUS_008.06");
    for (uint32 index = 2; index < 4; ++index)
    {
        G3Function800494E0(index);
        if ((G3Function80049628(index) & 0xffff) == 1)
        {
            G3Function800493C0(index);
            G3Function8004933C(index);
        }
    }
    return 0;
}

uint32 G3Function8003F400(uint32 descriptor, uint32 limit)
{
    FUNCTION_MARKER(0x8003f400, "SLUS_008.06");
    uint32 values = r_u32(descriptor);
    uint32 sum = 0;
    if (descriptor == 0)
        return 0;
    uint32 kind = r_u8(descriptor + 6);
    sint32 count = r_s16(descriptor + 4);
    if (count <= 0)
        return 0;
    sint32 signed_limit = (sint16)(limit & 0xffff);
    uint32 stride = kind == 1 ? 10 : 32;
    uint32 index = 0;
    do
    {
        if ((sint16)(index & 0xffff) >= signed_limit)
            break;
        uint32 value = r_u16(values);
        sum += value;
        values += stride;
        ++index;
    } while ((sint16)(index & 0xffff) < count);
    return (uint32)(sint32)(sint16)(sum & 0xffff);
}

uint32 G3Function8004B1C4(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b1c4, "SLUS_008.06");
    uint32 owner = r_u32(context + 0xe8);
    uint32 channel = r_u8(context + 0xed);
    uint32 value = r_u8(command + 5);
    uint32 result = owner + channel;
    w_u8(result + 0x126, value);
    return result;
}

uint32 G3Function8004B214(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b214, "SLUS_008.06");
    uint32 index = r_u8(command + 5);
    if (index >= 128)
        return 0x800a0000;
    uint32 owner = r_u32(context + 0xe8);
    uint32 audio = r_u32(0x80098ff0);
    uint32 value = r_u8(owner + 0xee);
    uint32 result = audio + index;
    w_u8(result + 0x564, value);
    return result;
}

uint32 G3Function8004B244(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b244, "SLUS_008.06");
    uint32 first = r_u8(command + 5);
    uint32 second = r_u8(command + 6);
    uint32 third = r_u8(command + 7);
    if (first >= 128 || second >= 128 || third >= 128)
        return 0;
    uint32 owner = r_u32(context + 0xe8);
    uint32 audio = r_u32(0x80098ff0);
    uint32 value = r_u32(owner + 0x14);
    w_u8(audio + first + 0x564, value);
    audio = r_u32(0x80098ff0);
    w_u8(audio + second + 0x564, value >> 8);
    audio = r_u32(0x80098ff0);
    uint32 result = audio + third;
    w_u8(result + 0x564, value >> 16);
    return result;
}

uint32 G3Function8004B2B4(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b2b4, "SLUS_008.06");
    uint32 index = r_u8(command + 5);
    if (index >= 128)
        return 0x800a0000;
    uint32 owner = r_u32(context + 0xe8);
    uint32 audio = r_u32(0x80098ff0);
    uint32 value = r_u8(owner + 0xf2);
    uint32 result = audio + index;
    w_u8(result + 0x564, value);
    return result;
}

uint32 G3Function8004B2E4(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b2e4, "SLUS_008.06");
    uint32 first = r_u8(command + 5);
    uint32 second = r_u8(command + 6);
    if (first >= 128 || second >= 128)
        return 0;
    uint32 owner = r_u32(context + 0xe8);
    uint32 audio = r_u32(0x80098ff0);
    uint32 value = r_u8(owner + 0xf6);
    w_u8(audio + first + 0x564, value);
    owner = r_u32(context + 0xe8);
    audio = r_u32(0x80098ff0);
    uint32 result = r_u16(owner + 0xf6) >> 8;
    w_u8(audio + second + 0x564, result);
    return result;
}

uint32 G3Function8004B338(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b338, "SLUS_008.06");
    uint32 index = r_u8(command + 5);
    uint32 channel = r_u8(context + 0xed);
    if (index >= 128)
        return 0x800a0000;
    uint32 owner = r_u32(context + 0xe8);
    uint32 audio = r_u32(0x80098ff0);
    uint32 value = r_u8(owner + channel + 0x146);
    uint32 result = audio + index;
    w_u8(result + 0x564, value);
    return result;
}

uint32 G3Function8004B36C(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b36c, "SLUS_008.06");
    uint32 index = r_u8(command + 5);
    uint32 channel = r_u8(context + 0xed);
    if (index >= 128)
        return 0x800a0000;
    uint32 owner = r_u32(context + 0xe8);
    uint32 audio = r_u32(0x80098ff0);
    uint32 value = r_u8(owner + channel + 0x156);
    uint32 result = audio + index;
    w_u8(result + 0x564, value);
    return result;
}

uint32 G3Function8004B3A8(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b3a8, "SLUS_008.06");
    uint32 index = r_u8(command + 5);
    uint32 channel = r_u8(context + 0xed);
    if (index >= 128)
        return 0x800a0000;
    uint32 owner = r_u32(context + 0xe8);
    uint32 audio = r_u32(0x80098ff0);
    uint32 value = r_u8(owner + channel + 0x126);
    uint32 result = audio + index;
    w_u8(result + 0x564, value);
    return result;
}

uint32 G3Function8004B3EC(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b3ec, "SLUS_008.06");
    uint32 index = r_u8(command + 5);
    if (index >= 128)
        return 0x800a0000;
    uint32 owner = r_u32(context + 0xe8);
    uint32 audio = r_u32(0x80098ff0);
    uint32 value = r_u8(owner + 0xfa);
    uint32 result = audio + index;
    w_u8(result + 0x564, value);
    return result;
}

uint32 G3Function8004B41C(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b41c, "SLUS_008.06");
    uint32 index = r_u8(command + 5);
    if (index >= 128)
        return 0x800a0000;
    uint32 owner = r_u32(context + 0xe8);
    uint32 audio = r_u32(0x80098ff0);
    uint32 value = r_u8(owner + 0xfb);
    uint32 result = audio + index;
    w_u8(result + 0x564, value);
    return result;
}

uint32 G3Function8004B478(uint32 command)
{
    FUNCTION_MARKER(0x8004b478, "SLUS_008.06");
    uint32 source = r_u8(command + 5);
    uint32 destination = r_u8(command + 6);
    if (source >= 128)
        return 0;
    if (destination >= 128)
        return 0x800a0000;
    uint32 audio = r_u32(0x80098ff0);
    uint32 value = r_u8(audio + source + 0x564);
    uint32 result = audio + destination;
    w_u8(result + 0x564, value);
    return result;
}

uint32 G3Function8004B4F0(uint32 command)
{
    FUNCTION_MARKER(0x8004b4f0, "SLUS_008.06");
    uint32 index = r_u8(command + 6);
    uint32 operand = r_u8(command + 5);
    if (index >= 128)
        return 0x800a0000;
    uint32 audio = r_u32(0x80098ff0);
    uint32 value = r_u8(audio + index + 0x564);
    uint32 result = value - operand;
    w_u8(audio + index + 0x564, result);
    return result;
}

uint32 G3Function8004B528(uint32 command)
{
    FUNCTION_MARKER(0x8004b528, "SLUS_008.06");
    uint32 index = r_u8(command + 6);
    uint32 operand = r_u8(command + 5);
    if (index >= 128)
        return 0x800a0000;
    uint32 audio = r_u32(0x80098ff0);
    uint32 value = r_u8(audio + index + 0x564);
    uint32 result = operand | value;
    w_u8(audio + index + 0x564, result);
    return result;
}

uint32 G3Function8004B560(uint32 command)
{
    FUNCTION_MARKER(0x8004b560, "SLUS_008.06");
    uint32 index = r_u8(command + 6);
    uint32 operand = r_u8(command + 5);
    if (index >= 128)
        return 0x800a0000;
    uint32 audio = r_u32(0x80098ff0);
    uint32 value = r_u8(audio + index + 0x564);
    uint32 result = value & ~operand;
    w_u8(audio + index + 0x564, result);
    return result;
}

uint32 G3Function8004B6A0(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b6a0, "SLUS_008.06");
    uint32 label = r_u8(command + 5);
    uint32 owner = r_u32(context + 0xe8);
    return G3Function8004B598(owner, label);
}

uint32 G3Function8004B6C8(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b6c8, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 index = r_u8(command + 5);
    uint32 value = r_u8(audio + index + 0x564);
    uint32 operand = r_u8(command + 6);
    uint32 label = r_u8(command + 7);
    uint32 result = operand;
    if (value == operand)
        return G3Function8004B598(context, label);
    return result;
}

uint32 G3Function8004B710(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b710, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 index = r_u8(command + 5);
    uint32 value = r_u8(audio + index + 0x564);
    uint32 operand = r_u8(command + 6);
    uint32 label = r_u8(command + 7);
    uint32 result = operand;
    if (value != operand)
        return G3Function8004B598(context, label);
    return result;
}

uint32 G3Function8004B7A4(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b7a4, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 index = r_u8(command + 5);
    uint32 value = r_u8(audio + index + 0x564);
    uint32 operand = r_u8(command + 6);
    uint32 label = r_u8(command + 7);
    uint32 result = operand < value ? 1 : 0;
    if (result != 0)
        return G3Function8004B598(context, label);
    return result;
}

uint32 G3Function8004B7F0(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b7f0, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 index = r_u8(command + 5);
    uint32 value = r_u8(audio + index + 0x564);
    uint32 operand = r_u8(command + 6);
    uint32 label = r_u8(command + 7);
    uint32 result = operand < value ? 1 : 0;
    if (result == 0)
        return G3Function8004B598(context, label);
    return result;
}

uint32 G3Function8004B83C(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b83c, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 index = r_u8(command + 5);
    uint32 value = r_u8(audio + index + 0x564);
    uint32 operand = r_u8(command + 6);
    uint32 label = r_u8(command + 7);
    uint32 result = value < operand ? 1 : 0;
    if (result == 0)
        return G3Function8004B598(context, label);
    return result;
}

uint32 G3Function8004B888(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b888, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 index = r_u8(command + 5);
    uint32 value = r_u8(audio + index + 0x564);
    uint32 operand = r_u8(command + 6);
    uint32 label = r_u8(command + 7);
    uint32 result = value & operand;
    if (result == operand)
        return G3Function8004B598(context, label);
    return result;
}

uint32 G3Function8004B8D4(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b8d4, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 index = r_u8(command + 5);
    uint32 value = r_u8(audio + index + 0x564);
    uint32 operand = r_u8(command + 6);
    uint32 label = r_u8(command + 7);
    uint32 result = ~(value & operand);
    if (result == operand)
        return G3Function8004B598(context, label);
    return result;
}

uint32 G3Function8004B924(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b924, "SLUS_008.06");
    uint32 flags = r_u16(context + 0xf4);
    uint32 result = flags | 2;
    w_u16(context + 0xf4, result);
    return result;
}

uint32 G3Function8004B94C(uint32 command, uint32 context)
{
    FUNCTION_MARKER(0x8004b94c, "SLUS_008.06");
    uint32 flags = r_u16(context + 0xf4);
    uint32 result = flags | 8;
    w_u16(context + 0xf4, result);
    return result;
}

uint32 G3Function8004B9C0(uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004b9c0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    w_u8(frame + 0x10, 0);
    uint32 high = r_u8(command + 8);
    uint32 low = r_u8(command + 9);
    uint32 handle = G3Function8004BFBC(low | (high << 8));
    w_u32(frame + 0x14, handle);
    w_u32(frame + 0x18, 0);
    uint32 volume = r_u8(command + 5);
    w_u8(frame + 0x11, volume);
    uint32 pan = r_u8(command + 6);
    w_u8(frame + 0x12, pan);
    uint32 pitch = r_u8(command + 7);
    w_u16(frame + 0x1c, (pitch - 128) << 5);
    uint32 audio = r_u32(0x80098ff0);
    uint32 index = r_u8(command + 0xa);
    w_u8(audio + index + 0x564, handle);
    audio = r_u32(0x80098ff0);
    w_u8(audio + index + 0x565, handle >> 8);
    audio = r_u32(0x80098ff0);
    w_u8(audio + index + 0x566, handle >> 16);
    audio = r_u32(0x80098ff0);
    w_u8(audio + index + 0x567, handle >> 24);
    return G3Function8004C154(frame + 0x10);
}

uint32 G3Function8004BA78(uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004ba78, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    w_u8(frame + 0x10, 1);
    uint32 audio = r_u32(0x80098ff0);
    uint32 index = r_u8(command + 5);
    uint32 second = r_u8(audio + index + 0x565);
    uint32 first = r_u8(audio + index + 0x564);
    uint32 third = r_u8(audio + index + 0x566);
    uint32 fourth = r_u8(audio + index + 0x567);
    uint32 handle = first | (second << 8) | (third << 16) | (fourth << 24);
    w_u32(frame + 0x14, handle);
    return G3Function8004C2C0(frame + 0x10);
}

uint32 G3Function8004BAE8(uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004bae8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    w_u8(frame + 0x10, 2);
    uint32 audio = r_u32(0x80098ff0);
    uint32 index = r_u8(command + 8);
    uint32 second = r_u8(audio + index + 0x565);
    uint32 first = r_u8(audio + index + 0x564);
    uint32 third = r_u8(audio + index + 0x566);
    uint32 fourth = r_u8(audio + index + 0x567);
    uint32 handle = first | (second << 8) | (third << 16) | (fourth << 24);
    w_u32(frame + 0x14, handle);
    uint32 volume = r_u8(command + 5);
    w_u8(frame + 0x11, volume);
    uint32 pan = r_u8(command + 6);
    w_u8(frame + 0x12, pan);
    return G3Function8004C42C(frame + 0x10);
}

void G3Function8004D860(uint32 camera, uint32 target)
{
    FUNCTION_MARKER(0x8004d860, "SLUS_008.06");
    w_u32(0x80096af8, target);
}

void G3Function8004D870(uint32 camera, uint32 target)
{
    FUNCTION_MARKER(0x8004d870, "SLUS_008.06");
    w_u32(0x80096afc, target);
}

uint32 G3Function8004D9B0(void)
{
    FUNCTION_MARKER(0x8004d9b0, "SLUS_008.06");
    return r_u32(0x80096a5c);
}

void G3Function8004D9D0(uint32 object, uint32 value, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004d9d0, "SLUS_008.06");
    w_u16(0x80096a64, value);
}

uint32 G3Function8004DAE4(void)
{
    FUNCTION_MARKER(0x8004dae4, "SLUS_008.06");
    uint32 state = r_u32(0x80096a88);
    return state == 6 ? 1 : 0;
}

uint32 G3Function8005177C(uint32 position, uint32 argument1, uint32 target, uint32 offset, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005177c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    G3Function80013C94(position, 7, frame);
    uint32 value = r_u16(position);
    uint32 origin = r_u16(0x80096b38);
    w_u16(0x800a239c, value - origin);
    value = r_u16(position + 2);
    origin = r_u16(0x80096b3a);
    w_u16(0x800a239e, value - origin);
    value = r_u16(position + 4);
    origin = r_u16(0x80096b3c);
    uint32 difference = value - origin;
    w_u16(0x800a23a0, difference);
    uint32 height = r_u16(offset + 4);
    w_u32(0x800a2398, target);
    uint32 result = difference + height;
    w_u16(0x800a23a0, result);
    return result;
}

uint32 G3Function8005820C(uint32 enabled, uint32 value)
{
    FUNCTION_MARKER(0x8005820c, "SLUS_008.06");
    if (enabled != 0)
    {
        uint32 flags = r_u32(0x80096a94);
        if ((flags & 1) == 0)
        {
            uint32 saved = r_u16(0x800969c4);
            w_u16(0x800a24cc, saved);
            G3Function80058798(0xffffffff);
        }
        uint32 previous = r_u32(0x800a258c);
        w_u16(0x800a2582, value);
        w_u16(0x800a2584, value);
        w_u32(0x800a258c, 0);
        uint32 result = previous + 1;
        w_u32(0x800a2594, result);
        return result;
    }
    uint32 previous = r_u32(0x800a2594);
    w_u32(0x800a2594, 0);
    uint32 result = previous - 1;
    w_u32(0x800a258c, result);
    return result;
}

uint32 G3Function8005AB7C(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005ab7c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x80;
    {
        uint32 copied[2];
        for (uint32 word = 0; word < 2; ++word)
        {
            uint32 source = object + 0x114 + word * 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (0xf << low_lane) & 0xf);
            copied[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 word = 0; word < 2; ++word)
        {
            uint32 destination = frame + 0x10 + word * 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, copied[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, copied[word] << (low_lane * 8), (0xf << low_lane) & 0xf);
        }
    }
    G3Function8002FF58(frame + 0x18, object + 0x11c, frame);
    TransposeMatrix((MATRIX *)psx_addr(frame + 0x18, 20), (MATRIX *)psx_addr(frame + 0x38, 20));
    for (uint32 pass = 0; pass < 2; ++pass)
    {
        uint32 matrix = frame + 0x18 + pass * 0x20;
        uint32 first = r_u32(matrix);
        uint32 second = r_u32(matrix + 4);
        xport_gte_write_control(0, first);
        xport_gte_write_control(1, second);
        first = r_u32(matrix + 8);
        second = r_u32(matrix + 0xc);
        uint32 third = r_u32(matrix + 0x10);
        xport_gte_write_control(2, first);
        xport_gte_write_control(3, second);
        xport_gte_write_control(4, third);
        xport_gte_write_data(0, r_u32(frame + 0x10));
        xport_gte_write_data(1, r_u32(frame + 0x14));
        xport_gte_execute(0x486012);
        for (uint32 axis = 0; axis < 3; ++axis)
            w_u32(frame + 0x58 + axis * 4, xport_gte_read_data(25 + axis));
        if (pass == 0)
        {
            first = r_u16(frame + 0x58);
            second = r_u16(frame + 0x5c);
            third = r_u16(frame + 0x60);
            w_u16(frame + 0x10, first);
            w_u16(frame + 0x12, second);
            w_u16(frame + 0x14, third);
            uint32 angle = (uint32)r_s16(object + 0x122);
            G3Function8002F8E4(frame + 0x10, angle);
        }
    }
    uint32 result = r_u16(frame + 0x58);
    w_u16(object + 0x114, result);
    result = r_u16(frame + 0x5c);
    w_u16(object + 0x116, result);
    result = r_u16(frame + 0x60);
    w_u16(object + 0x118, result);
    return result;
}

uint32 G3Function8005A308(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005a308, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x10, flags | 0x800);
    uint32 lifetime = r_u16(state + 0xc) - 1;
    w_u16(state + 0xc, lifetime);
    if ((sint32)(lifetime << 16) <= 0)
        return G3Function8005A544(frame);
    uint32 follower = r_u32(state + 4);
    uint32 owner = r_u32(follower + 0x1c);
    sint32 count = r_s16(owner + 0xa);
    if (count > 0)
    {
        uint32 leader = r_u32(object + 0x104);
        uint32 animation = r_u16(leader + 0x6e);
        w_u16(follower + 0x6e, (sint32)animation < count ? animation : 0);
        follower = r_u32(state + 4);
        leader = r_u32(object + 0x104);
        owner = r_u32(follower + 0x1c);
        animation = r_u16(follower + 0x6e);
        uint32 table = r_u32(owner + 0x10);
        uint32 selected = r_u32(table + animation * 4);
        sint32 current = r_s16(leader + 0x5e);
        sint32 frames = r_s16(selected + 2);
        uint32 unsigned_current = r_u16(leader + 0x5e);
        w_u16(follower + 0x5e, current < frames ? unsigned_current : 0);
    }
    follower = r_u32(state + 4);
    uint32 leader = r_u32(state);
    {
        uint32 copied[1];
        for (uint32 word = 0; word < 1; ++word)
        {
            uint32 source = leader + 0x58 + word * 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (0xf << low_lane) & 0xf);
            copied[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        uint32 height = (uint32)r_s16(leader + 0x5c);
        for (uint32 word = 0; word < 1; ++word)
        {
            uint32 destination = follower + 0x58 + word * 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, copied[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, copied[word] << (low_lane * 8), (0xf << low_lane) & 0xf);
        }
        w_u16(follower + 0x5c, height);
    }
    follower = r_u32(state + 4);
    leader = r_u32(state);
    {
        uint32 copied[2];
        for (uint32 word = 0; word < 2; ++word)
        {
            uint32 source = leader + 0x60 + word * 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (0xf << low_lane) & 0xf);
            copied[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 word = 0; word < 2; ++word)
        {
            uint32 destination = follower + 0x60 + word * 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, copied[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, copied[word] << (low_lane * 8), (0xf << low_lane) & 0xf);
        }
    }
    if (r_s16(state + 0x1e) != 0)
        G3Function8005AB7C(object, frame);
    if (r_s16(state + 0xe) != 0)
    {
    {
        uint32 copied[2];
        for (uint32 word = 0; word < 2; ++word)
        {
            uint32 source = state + 0x10 + word * 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (0xf << low_lane) & 0xf);
            copied[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 word = 0; word < 2; ++word)
        {
            uint32 destination = frame + 0x18 + word * 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, copied[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, copied[word] << (low_lane * 8), (0xf << low_lane) & 0xf);
        }
    }
        leader = r_u32(state);
        uint32 angle = (uint32)r_s16(leader + 0x64);
        G3Function8002F8E4(frame + 0x18, angle);
        G3Function8005AAAC(object, frame + 0x18);
    }
    uint32 distance = r_u16(state + 0xa);
    uint32 step = r_u16(state + 8);
    sint32 signed_step = r_s16(state + 8);
    w_u16(state + 0xa, distance + step);
    uint32 vector = signed_step > 0 ? state + 0x28 : state + 0x38;
    distance = (uint32)r_s16(state + 0xa);
    G3Function8002F9A8(frame + 0x10, distance, vector, frame);
    leader = r_u32(state);
    {
        uint32 copied[1];
        for (uint32 word = 0; word < 1; ++word)
        {
            uint32 source = leader + 0x58 + word * 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (0xf << low_lane) & 0xf);
            copied[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        uint32 height = (uint32)r_s16(leader + 0x5c);
        for (uint32 word = 0; word < 1; ++word)
        {
            uint32 destination = object + 0x58 + word * 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, copied[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, copied[word] << (low_lane * 8), (0xf << low_lane) & 0xf);
        }
        w_u16(object + 0x5c, height);
    }
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 position = r_u16(object + 0x58 + axis * 2);
        uint32 offset = r_u16(frame + 0x10 + axis * 2);
        w_u16(object + 0x58 + axis * 2, position + offset);
    }
    return G3Function8005AB48(object);
}

uint32 G3Function80065A6C(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80065a6c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 result = r_u32(object + 0x10) | 0x100800;
    uint32 descriptor = r_u32(object + 0x28);
    uint32 state = object + 0x104;
    w_u32(object + 0x10, result);
    w_u32(state + 0xc, 0);
    w_u32(state + 4, 0);
    w_u32(state + 0x10, 0);
    if (descriptor == 0)
        return result;
    uint32 end = r_u32(descriptor + 4);
    if (end != 0 && (r_u16(descriptor + 0x10) & 4) == 0)
        w_u32(state + 0xc, 1);
    else
    {
        uint32 delay = r_u32(descriptor);
        w_u32(object + 0x104, delay);
    }
    uint32 flags = r_u16(descriptor + 0x10);
    result = flags & 2;
    if ((flags & 1) != 0 && result != 0)
    {
        uint32 pool = r_u32(0x800a27fc);
        result = G3Function800415CC(pool, frame);
        w_u32(state + 4, result);
    }
    return result;
}

uint32 G3Function80065B18(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80065b18, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 descriptor = r_u32(object + 0x28);
    uint32 state = object + 0x104;
    uint32 resource = G3Function8003AA48(descriptor + 8);
    if (descriptor == 0 || resource == 0)
        return G3Function80038798(object, 0, 0, frame);
    uint32 mode = r_u32(state + 0xc);
    if (mode == 1)
    {
        uint32 result = r_u32(object + 0x104) + 1;
        w_u32(object + 0x104, result);
        uint32 end = r_u32(descriptor + 4);
        if (result == end)
        {
            result = r_u32(descriptor);
            w_u32(object + 0x104, result);
            w_u32(state + 0xc, 0);
        }
        return result;
    }
    uint32 timer = r_u32(object + 0x104) + 1;
    w_u32(object + 0x104, timer);
    uint32 delay = r_u32(descriptor);
    if ((sint32)timer < (sint32)delay)
        return 1;
    w_u32(object + 0x104, 0);
    if (r_u8(descriptor + 0x12) != 0)
    {
        uint32 sequence = r_u32(descriptor + 0x14);
        if (sequence != 0)
            G3Function80018DE0(object, sequence + 4, frame);
    }
    uint32 flags = r_u16(descriptor + 0x10);
    if ((flags & 1) != 0)
    {
        uint32 binding = G3Function8008345C(object, 0, 0);
        uint32 storage = r_u32(state + 4);
        uint32 list = r_u32(resource + 0xc);
        uint32 pool = r_u32(0x800a27fc);
        uint32 entry = r_u32(list);
        return G3Function80044564(binding, entry, storage, pool, frame);
    }
    uint32 created = G3Function8002A454(object, resource, frame);
    if (created == 0)
        return 0;
    flags = r_u32(created + 0x14);
    w_u32(created + 0x14, flags | 0x10000000);
    uint32 target = G3Function80039B78(0x8009efb8);
    w_u32(created + 0xec, target);
    G3Function8003A9F8(created, frame);
    flags = r_u16(descriptor + 0x10);
    if ((flags & 4) != 0)
    {
        uint32 notified = r_u32(state + 8);
        if (notified == 0)
        {
            uint32 player = r_u32(0x8009efc4);
            w_u32(state + 8, 1);
            target = r_u32(player + 0xec);
            if (target != 0)
            {
                uint32 payload = r_u32(descriptor + 4);
                G3DispatchStateEvent(target, player, game, 0x7df, payload, frame);
            }
        }
    }
    uint32 count = r_u32(state + 0x10);
    w_u32(state + 0x10, count + 1);
    flags = r_u32(object + 0x10);
    uint32 result = flags | 0x400;
    w_u32(object + 0x10, result);
    return result;
}
