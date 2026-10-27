#include "xport.h"
#include "psx.h"
#include "game_functions.h"

uint32 G3Function800740B0(uint32 object)
{
    FUNCTION_MARKER(0x800740b0, "SLUS_008.06");
    w_u32(object + 0x128, 3);
    return 3;
}

uint32 G3Function8007C734(uint32 object)
{
    FUNCTION_MARKER(0x8007c734, "SLUS_008.06");
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 extension = r_u32(descriptor + 0x1c);
    w_u32(object + 0x114, 200);
    uint32 flags = r_u32(object + 0x10);
    uint32 parent = r_u32(object + 0x2c);
    uint32 result = flags | 0x480;
    w_u32(object + 0x10, result);
    if (parent == 0)
        return result;
    for (uint32 copy = 0; copy < 2; ++copy)
    {
        uint32 source = copy == 0 ? parent + 0x58 : object + 0x58;
        uint32 destination = copy == 0 ? object + 0x120 : object + 0x88;
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
    descriptor = r_u32(object + 0x1c);
    uint32 data = r_u32(descriptor + 12);
    data = r_u32(data);
    uint32 motion = r_u32(data + 0x30);
    if (motion != 0)
    {
        flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags | 0x100000);
        w_u32(object + 0x128, 2);
        uint32 duration = extension != 0 ? r_u32(extension) : 240;
        w_u32(object + 0x10c, duration);
    }
    parent = r_u32(object + 0x2c);
    w_u32(object + 0x20, 0);
    result = r_u32(parent + 0x28);
    w_u32(object + 0x12c, result);
    return result;
}

uint32 G3Function80039AA4(uint32 level, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80039aa4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 game = r_u32(0x80097048);
    uint32 flags = r_u32(game + 0xe8);
    uint32 record = r_u32(level + 0xec);
    uint32 result = flags & 0x10;
    if (result != 0)
        return result;
    while (record != 0)
    {
        uint32 target = r_u32(record + 12);
        result = target;
        if (target == 0)
            break;
        game = r_u32(0x80097048);
        result = G3DispatchStateEvent(target, 0, game, 10, 0, frame);
        record = r_u32(record + 20);
    }
    return result;
}

uint32 G3Function80063BB4(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80063bb4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (message == 1)
    {
        G3Function80063C10(object);
        return 1;
    }
    if (message == 2)
    {
        G3Function80063C2C(object, frame);
        return 1;
    }
    return 0;
}

uint32 G3Function80063C10(uint32 object)
{
    FUNCTION_MARKER(0x80063c10, "SLUS_008.06");
    uint32 flags = r_u32(object + 0x10);
    w_u16(object + 0x9e, 0);
    uint32 result = flags | 0x100080;
    w_u32(object + 0x10, result);
    return result;
}

uint32 G3Function80063C2C(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80063c2c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 counter = r_u16(object + 0x9e) + 1;
    w_u16(object + 0x9e, counter);
    uint32 result = (uint32)(sint32)(sint16)(counter & 0xffff);
    if (result == 12)
        return G3Function8002A9AC(object, frame);
    return result;
}

uint32 G3Function8002A9AC(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002a9ac, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 flags = r_u32(object + 0x10);
    uint32 properties = r_u32(object + 0x18);
    w_u32(object + 0x10, (flags | 0x10) & 0xfffffbff);
    if ((properties & 0x200) == 0)
        G3Function8003772C(object, frame);
    uint32 game = r_u32(0x80097048);
    uint32 state = r_u32(game + 0x5c);
    return G3Function800292B4(state, object);
}

uint32 G3Function800292B4(uint32 state, uint32 object)
{
    FUNCTION_MARKER(0x800292b4, "SLUS_008.06");
    uint32 owner = r_u32(0x8009efc4);
    uint32 data = r_u32(owner + 0x24);
    uint32 result = r_u32(data + 0x1fc);
    if (object != result)
    {
        uint32 flags = r_u32(object + 0x10);
        result = flags | 0x20;
        w_u32(object + 0x10, result);
    }
    return result;
}

uint32 G3Function8003772C(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003772c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    return G3Function8003742C(object, frame);
}

uint32 G3Function800659B0(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800659b0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3Function80065A6C(object, frame);
        return 3;
    case 2:
        G3Function80065B18(object, game, frame);
        return 3;
    case 9:
        G3Function80065D14(object, game, payload, frame);
        return 3;
    case 16:
    {
        uint32 timer = r_u32(object + 0x114) - 1;
        w_u32(object + 0x114, timer);
        if ((sint32)timer <= 0)
        {
            w_u32(object + 0x114, 0);
            uint32 flags = r_u32(object + 0x10);
            w_u32(object + 0x10, flags & 0xfffffbff);
        }
        return 3;
    }
    default:
        return 0;
    }
}

uint32 G3Function8006B8CC(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006b8cc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (message != 2)
        return 0;
    G3Function8006BBF4(object, frame);
    return 1;
}

uint32 G3Function80076450(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80076450, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3Function8007657C(object, frame);
        return 1;
    case 2:
        G3Function8007662C(object, game, frame);
        return 1;
    case 3:
        G3Function80076B64(object, game, frame);
        return 1;
    case 5:
        G3Function80076568(payload);
        return 1;
    case 8:
        G3Function800769D0(object, payload, frame);
        return 1;
    default:
        return 0;
    }
}

uint32 G3Function8005A2C8(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005a2c8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (message != 2)
        return 0;
    G3Function8005A308(object, frame);
    return 1;
}

uint32 G3Function80076568(uint32 object)
{
    FUNCTION_MARKER(0x80076568, "SLUS_008.06");
    uint32 flags = r_u32(object + 0x18);
    uint32 result = flags & 0xfffff7ff;
    w_u32(object + 0x18, result);
    return result;
}

uint32 G3Function80045CAC(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80045cac, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    switch (message)
    {
    case 1:
        G3Function800463AC(object, frame);
        return 1;
    case 2:
        G3Function800465F8(object, game, frame);
        return 1;
    case 3:
    case 8:
    case 0x7e7:
        G3Function80046514(object);
        return 1;
    case 4:
        G3Function80046458(object, game, frame);
        return 1;
    case 10:
        G3Function80045E7C(game);
        return 1;
    case 11:
        G3Function80045E2C(game);
        return 1;
    case 15:
        if (payload != 0)
            w_u32(0x80098764, payload);
        return 1;
    case 0x7e1:
    {
        uint32 value = r_u32(payload);
        G3Function800462D4(game, value, frame);
        return 1;
    }
    case 0x7e4:
        G3Function80046514(object);
        G3Function800193F8(0x01000000, object, frame);
        return 1;
    default:
        return 0;
    }
}

uint32 G3Function800193F8(uint32 mask, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800193f8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 sequence = G3Function8001932C(mask, object, frame);
    if (sequence != 0)
        G3Function80018DE0(object, sequence, frame);
    return sequence;
}

uint32 G3Function80045E7C(uint32 game)
{
    FUNCTION_MARKER(0x80045e7c, "SLUS_008.06");
    uint32 level = r_u32(game + 0x48);
    uint32 next_id = r_u32(level + 0xe4);
    uint32 record = r_u32(level + 0x80);
    uint32 count = r_u32(level + 0x7c);
    ++next_id;
    uint32 target = r_u32(0x80098764);
    uint32 index = 0;
    if ((sint32)count > 0)
    {
        uint32 field = record + 0x2c;
        do
        {
            uint32 value = r_u32(record);
            if (value == target)
            {
                w_u16(field + 2, next_id);
                ++next_id;
                w_u16(field, 1);
            }
            field += 0x34;
            level = r_u32(game + 0x48);
            count = r_u32(level + 0x7c);
            ++index;
            record += 0x34;
        } while ((sint32)index < (sint32)count);
    }
    level = r_u32(game + 0x48);
    w_u32(game + 0x6820, 0);
    w_u32(game + 0x6830, 0);
    uint32 entries = r_u32(level + 0xe4);
    uint32 destination = r_u32(game + 0x68ac);
    G3FillMemory80086094(destination, 0, ((entries << 1) + entries) << 1);
    destination = r_u32(game + 0x68b0);
    return G3FillMemory80086094(destination, 0, 100);
}

uint32 G3Function80045E2C(uint32 game)
{
    FUNCTION_MARKER(0x80045e2c, "SLUS_008.06");
    uint32 cursor = r_u32(game + 0x68b0);
    w_u32(game + 0x6820, 0);
    for (uint32 index = 0; index < 100; ++index)
    {
        uint32 value = r_u8(cursor);
        if (value == 4)
        {
            uint32 count = r_u32(game + 0x6820);
            w_u32(game + 0x6820, count + 1);
        }
        else
            w_u8(cursor, 0);
        ++cursor;
    }
    return 0;
}

uint32 G3Function80046514(uint32 object)
{
    FUNCTION_MARKER(0x80046514, "SLUS_008.06");
    uint32 flags = r_u32(object + 0x128);
    uint32 result = flags & 9;
    if (result == 0)
    {
        w_u32(object + 0x128, flags | 8);
        w_u32(object + 0x138, 6);
        return 6;
    }
    return result;
}

uint32 G3Function800463AC(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800463ac, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    w_u32(object + 0x134, 0);
    w_u16(object + 0x13e, 0);
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x10, flags | 0x80);
    uint32 mode = r_u32(object + 0x128);
    uint32 parent = r_u32(object + 0x2c);
    w_u32(object + 0x128, mode & 0xfffffffe);
    if (parent != 0)
    {
        flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags | 0x100000);
    }
    flags = r_u32(object + 0x10);
    w_u32(object + 0x40, 0);
    w_u32(object + 0x10, flags | 0x40);
    G3Function80036D1C(object, 0x100, 0x500, frame);
    uint32 position = r_u16(object + 0x5c);
    uint32 baseline = r_u16(object + 0xe0);
    w_u16(object + 0x13c, position - baseline);
    flags = r_u32(object + 0x10);
    uint32 result = flags & 0xf7ffffff;
    w_u32(object + 0x10, result);
    return result;
}

uint32 G3Function80046458(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80046458, "SLUS_008.06");
    uint32 index = G3Function80046384(object);
    if (index == 0)
        return 0;
    uint32 level = r_u32(game + 0x48);
    uint32 count = r_u32(level + 0xe4);
    if ((sint32)count < (sint32)index)
        return 1;
    uint32 states = r_u32(game + 0x68b0);
    uint32 state = r_u8(states + index - 1);
    if (state == 3)
        return 4;
    if (state == 4)
        return 2;
    uint32 offset = ((index << 1) + index) << 1;
    w_u8(states + index - 1, 2);
    uint32 positions = r_u32(game + 0x68ac);
    uint32 coordinate = r_u16(object + 0x58);
    w_u16(positions + offset - 6, coordinate);
    positions = r_u32(game + 0x68ac);
    coordinate = r_u16(object + 0x5a);
    w_u16(positions + offset - 4, coordinate);
    positions = r_u32(game + 0x68ac);
    coordinate = r_u16(object + 0x5c);
    w_u16(positions + offset - 2, coordinate);
    return positions;
}

uint32 G3Function80046384(uint32 object)
{
    FUNCTION_MARKER(0x80046384, "SLUS_008.06");
    uint32 descriptor = r_u32(object + 0x20);
    if (descriptor != 0)
        return (uint32)(sint32)r_s16(descriptor + 0x2e);
    return (uint32)(sint32)r_s16(object + 0x13e);
}

uint32 G3Function8002AB10(uint32 records, uint32 count, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002ab10, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 flags = r_u32(game + 0xe8);
    uint32 frame = guest_stack - 0x20;
    if ((flags & 0x10) != 0)
        return flags & 0x10;
    uint32 end = records + count * uint32(0x34);
    while (records < end)
    {
        uint32 descriptor = r_u32(records);
        if (descriptor != 0)
        {
            uint32 target = r_u32(descriptor + 0x34);
            if (target != 0)
            {
                game = r_u32(0x80097048);
                G3DispatchStateEvent(target, 0, game, 12, records, frame);
            }
        }
        records += 0x34;
    }
    return 0;
}

uint32 G3Function80065D54(uint32 object, uint32 game, uint32 message, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80065d54, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (object == 0)
        return 1;
    uint32 extension = object + 0x104;
    uint32 descriptor = r_u32(object + 0x28);
    if (descriptor == 0)
        return 1;
    switch (message)
    {
    case 1:
        G3Function80066978(object, game, extension, descriptor, frame);
        return 1;
    case 2:
        G3Function80066AE8(object, game, extension, descriptor, frame);
        return 1;
    case 4:
    case 7:
        G3Function80066958(object, extension, frame);
        return 1;
    case 0x7e5:
        G3Function80066D68(object, game, extension, descriptor, frame);
        return 1;
    default:
        return 0;
    }
}

uint32 G3Function80066958(uint32 object, uint32 extension, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80066958, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    return G3Function8006688C(object, extension, frame);
}

uint32 G3Function80035E80(uint32 game)
{
    FUNCTION_MARKER(0x80035e80, "SLUS_008.06");
    uint32 level = r_u32(game + 0x48);
    uint32 cursor = r_u32(level + 0x3c);
    uint32 first = r_u32(cursor);
    if (first != cursor)
    {
        uint32 next;
        uint32 sentinel;
        do
        {
            uint32 object = r_u32(cursor);
            if (object != 0)
            {
                uint32 name = r_u32(object + 0x24);
                uint32 index = G3Function80035E14(name);
                w_u16(object + 6, (sint32)index > 0 ? index : uint32(0xffffffff));
            }
            cursor += 4;
            level = r_u32(game + 0x48);
            next = r_u32(cursor);
            sentinel = r_u32(level + 0x3c);
        } while (next != sentinel);
    }
    w_u16(0x800ad0c0, 0xffff);
    w_u16(0x800ad0c4, 0xffff);
    return 0x800aa7d0;
}

uint32 G3Function80035E14(uint32 name)
{
    FUNCTION_MARKER(0x80035e14, "SLUS_008.06");
    uint32 first = r_u32(0x80097844);
    if (first == 0)
        return 0xffffffff;
    uint32 key = r_u32(name);
    uint32 cursor = 0x80097844;
    uint32 index = 0;
    uint32 next;
    do
    {
        uint32 candidate = r_u32(cursor);
        uint32 prefix = r_u32(candidate);
        if (prefix == key)
        {
            uint32 suffix = r_u32(candidate + 4);
            uint32 expected = r_u32(name + 4);
            if (suffix == expected)
                return index;
        }
        cursor += 12;
        next = r_u32(cursor);
        ++index;
    } while (next != 0);
    return 0xffffffff;
}

uint32 G3Function80035AC8(void)
{
    FUNCTION_MARKER(0x80035ac8, "SLUS_008.06");
    w_u8(0x800aa7d0, 0);
    return 0x800b0000;
}

uint32 G3Function800810FC(void)
{
    FUNCTION_MARKER(0x800810fc, "SLUS_008.06");
    for (uint32 bank = 0; bank < 2; ++bank)
    {
        w_u32(0x800a2a94 + bank * 4, 0);
        uint32 offset = bank * 0xd0;
        for (uint32 slot = 0; slot < 4; ++slot)
        {
            w_u16(0x800a2aa2 + offset, 0);
            offset += 0x34;
        }
    }
    return 1;
}

uint32 G3Function80081154(void)
{
    FUNCTION_MARKER(0x80081154, "SLUS_008.06");
    for (uint32 bank = 0; bank < 2; ++bank)
    {
        w_u32(0x800a2a94 + bank * 4, 0);
        uint32 state = 0x800a2a8c + bank * 4;
        w_u8(state + 2, 0);
        w_u8(state + 1, 0);
        w_u8(state, 0);
    }
    return 1;
}

uint32 G3Function8008095C(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8008095c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3Function800809E4(object, game, frame);
        return 1;
    case 2:
        G3Function80080A58(object, game, frame);
        return 1;
    case 0x7ee:
        G3Function80080FC8(object);
        return 1;
    default:
        return 0;
    }
}

uint32 G3Function800809E4(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800809e4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    w_u32(object + 0x104, 0);
    w_u32(object + 0x108, 0);
    w_u32(object + 0x10c, 0);
    w_u32(object + 0x110, 0);
    w_u32(object + 0x114, 0);
    w_u32(frame + 0x10, 0x7ff8);
    G3Function8003E518(object, 0x81c, 90, 125, frame);
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x10, flags | 0x80);
    return G3Function80080A58(object, game, frame);
}

uint32 G3Function80080FC8(uint32 object)
{
    FUNCTION_MARKER(0x80080fc8, "SLUS_008.06");
    w_u32(object + 0x114, 1);
    return 1;
}

uint32 G3Function800751B4(uint32 guest_stack)
{
    FUNCTION_MARKER(0x800751b4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 object = r_u32(0x8009efc4);
    uint32 state = r_u32(object + 0x24);
    G3Function80074E08(object);
    G3Function80074BD0(object, state);
    G3Function80074B6C(state);
    G3Function80074F64(object, state, frame);
    return G3Function80074F28(object, frame);
}

uint32 G3Function80074B6C(uint32 state)
{
    FUNCTION_MARKER(0x80074b6c, "SLUS_008.06");
    G3FillMemory80086094(state + 0x13c, 0, 44);
    uint32 game = r_u32(0x80097048);
    w_u32(state + 0x13c, 0);
    uint32 level = r_u32(game + 0x48);
    uint32 value = r_u16(level + 0xb0);
    w_u16(state + 0xc6, value);
    level = r_u32(game + 0x48);
    value = r_u16(level + 0xb0);
    w_u16(state + 0xc8, value);
    return value;
}

uint32 G3Function80074E08(uint32 object)
{
    FUNCTION_MARKER(0x80074e08, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 saved_position = G3Function80023A68(game);
    game = r_u32(0x80097048);
    if (saved_position != 0)
    {
        uint32 value = r_u16(game + 0x67f0);
        w_u16(object + 0x58, value);
        value = r_u16(game + 0x67f2);
        w_u16(object + 0x5a, value);
        value = r_u16(game + 0x67f4);
        w_u16(object + 0x5c, value);
        uint32 level = r_u32(game + 0x48);
        value = r_u16(game + 0x67f6);
        w_u16(level + 0xb0, value);
    }
    else
    {
        uint32 flags = r_u32(game + 0xe8);
        if ((flags & 0x8000) != 0)
        {
            uint32 value = r_u16(game + 0x68dc);
            w_u16(object + 0x58, value);
            value = r_u16(game + 0x68de);
            w_u16(object + 0x5a, value);
            value = r_u16(game + 0x68e0);
            w_u16(object + 0x5c, value);
            uint32 level = r_u32(game + 0x48);
            w_u32(level + 0x6c, 0xffff0000);
        }
        else
        {
            uint32 level = r_u32(game + 0x48);
            uint32 value = r_u16(level + 0x28);
            w_u16(object + 0x58, value);
            level = r_u32(game + 0x48);
            value = r_u16(level + 0x2a);
            w_u16(object + 0x5a, value);
            level = r_u32(game + 0x48);
            value = r_u16(level + 0x2c);
            w_u16(object + 0x5c, value);
        }
    }
    uint32 x = r_u16(object + 0x58);
    uint32 y = r_u16(object + 0x5a);
    uint32 z = r_u16(object + 0x5c);
    w_u16(object + 0x70, x);
    w_u16(object + 0x72, y);
    w_u16(object + 0x74, z);
    return x;
}

uint32 G3Function80074BD0(uint32 object, uint32 state)
{
    FUNCTION_MARKER(0x80074bd0, "SLUS_008.06");
    uint32 mode = r_u32(object + 0x128);
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x128, mode & 0x04000080);
    uint32 properties = r_u32(object + 0x14);
    w_u32(object + 0x10, flags & 0x18320487);
    uint32 x = r_u16(object + 0x58);
    uint32 y = r_u16(object + 0x5a);
    uint32 z = r_u16(object + 0x5c);
    w_u32(object + 0x14, properties & 0x40);
    flags = r_u32(object + 0x10);
    w_u16(object + 0xde, y);
    uint32 game = r_u32(0x80097048);
    w_u32(object + 0x134, 0);
    w_u32(object + 0x20, 0);
    w_u32(object + 0x30, 0);
    w_u16(object + 0x60, 0);
    w_u16(object + 0x62, 0);
    w_u16(object + 0xdc, x);
    w_u16(object + 0xe0, z);
    properties = r_u32(object + 0x14);
    flags = (flags & 0xffbff6ff) | 0x480;
    w_u32(object + 0x10, flags);
    w_u32(object + 0x14, properties | 0x48);
    w_u32(object + 0x10, flags | 0x20000);
    properties = r_u32(object + 0x14);
    w_u32(object + 0x14, properties | 0x08000000);
    uint32 level = r_u32(game + 0x48);
    uint32 rotation = r_u16(level + 0xb0);
    w_u16(object + 0x6c, 4096);
    w_u16(object + 0x6a, 4096);
    w_u16(object + 0x68, 4096);
    w_u32(object + 12, 0);
    w_u16(object + 0x64, rotation);
    w_u32(object + 8, 0);
    w_u32(object, 0);
    w_u32(object + 4, 0);
    w_u32(object + 0x104, 0);
    w_u32(object + 0x108, 0);
    w_u32(object + 0x10c, 0);
    w_u32(object + 0x118, 0);
    w_u16(object + 0x6e, 0);
    w_u16(object + 0x5e, 0);
    w_u16(object + 0xa2, 0);
    w_u16(object + 0x9e, 0);
    w_u32(object + 0x150, 0);
    w_u32(object + 0x154, 2);
    uint32 index = (uint32)(sint32)r_s16(state + 0x20);
    uint32 table = r_u32(state + 0x2c);
    uint32 value = (uint32)(sint32)r_s16(table + (index << 1) - 2);
    w_u32(object + 0x11c, value);
    w_u32(object + 0x120, value);
    value = (uint32)(sint32)r_s16(state + 8);
    w_u32(object + 0xec, 0x8007801c);
    w_u16(object + 0x8e, 0);
    w_u16(object + 0xa0, 0);
    w_u32(object + 0xf0, 0x8006c110);
    w_u32(object + 0x124, value);
    value = r_u32(state);
    uint32 descriptor = r_u32(object + 0x1c);
    w_u32(object + 0xa4, value);
    uint32 chain = r_u32(descriptor + 12);
    uint32 model = r_u32(chain);
    w_u16(model + 0x26, 480);
    descriptor = r_u32(object + 0x1c);
    chain = r_u32(descriptor + 12);
    model = r_u32(chain);
    {
        uint32 source = 0x800a02c4;
        uint32 destination = model + 0x20;
        uint32 upper = r_u32((source + 3) & ~uint32(3));
        uint32 lower = r_u32(source & ~uint32(3));
        uint32 shift = (source & 3) << 3;
        uint32 word = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
        uint32 tail = (uint32)(sint32)r_s16(source + 4);
        uint32 high_address = destination + 3;
        uint32 high_byte = high_address & 3;
        uint32 preserve = high_byte == 3 ? 0 : uint32(0xffffffff) << ((high_byte + 1) << 3);
        uint32 previous = r_u32(high_address & ~uint32(3));
        w_u32(high_address & ~uint32(3), (previous & preserve) | (word >> ((3 - high_byte) << 3)));
        uint32 low_byte = destination & 3;
        preserve = low_byte == 0 ? 0 : uint32(0xffffffff) >> ((4 - low_byte) << 3);
        previous = r_u32(destination & ~uint32(3));
        w_u32(destination & ~uint32(3), (previous & preserve) | (word << (low_byte << 3)));
        w_u16(destination + 4, tail);
    }
    descriptor = r_u32(object + 0x1c);
    chain = r_u32(descriptor + 12);
    model = r_u32(chain);
    value = r_u16(0x800a02cc);
    w_u16(model + 0x28, value);
    descriptor = r_u32(object + 0x1c);
    chain = r_u32(descriptor + 12);
    model = r_u32(chain);
    value = r_u16(0x800a02ce);
    w_u16(model + 0x2a, value);
    descriptor = r_u32(object + 0x1c);
    chain = r_u32(descriptor + 12);
    model = r_u32(chain);
    value = r_u16(0x800a02d0);
    w_u16(model + 0x2c, value);
    value = r_u16(0x800a02cc);
    w_u16(object + 0xc8, value);
    value = r_u16(0x800a02ce);
    w_u16(object + 0xca, value);
    value = r_u16(0x800a02d0);
    w_u32(object + 0xf8, 0);
    w_u16(object + 0xcc, value);
    return value;
}

uint32 G3Function80074F28(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80074f28, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function80030454(object, frame);
    G3Function8003AA48(0x800a02d4);
    return G3Function80043E70(object, 0);
}

uint32 G3Function80043E70(uint32 object, uint32 use_first)
{
    FUNCTION_MARKER(0x80043e70, "SLUS_008.06");
    uint32 state = r_u32(object + 0x24);
    uint32 model = r_u32(state + 0x1a0);
    uint32 count = (uint32)(sint32)r_s16(model + 4);
    uint32 cursor = r_u32(model + 0x14);
    uint32 other = r_u32(0x8009874c);
    uint32 end = cursor + count * uint32(12);
    if (other != 0)
    {
        uint32 chain = r_u32(other + 12);
        uint32 second = r_u32(chain + 4);
        uint32 alternate = r_u32(second + 0x14);
        uint32 first = r_u32(chain);
        uint32 source = r_u32(first + 0x14);
        if (cursor >= end)
            return source;
        uint32 destination = cursor + 8;
        source += 8;
        do
        {
            uint32 destination_flags = r_u8(destination - 1);
            uint32 source_flags = r_u8(source - 1);
            if ((destination_flags & 0xf3) == (source_flags & 0xf3))
            {
                uint32 value = use_first != 0 ? r_u32(source) : r_u32(alternate + 8);
                w_u32(destination, value);
                source += 12;
                alternate += 12;
            }
            cursor += 12;
            destination += 12;
        } while (cursor < end);
        return 0;
    }
    if (cursor >= end)
        return 0;
    uint32 destination = cursor + 8;
    do
    {
        uint32 flags = r_u8(destination - 1);
        if ((flags & 2) != 0)
        {
            w_u8(destination - 1, 1);
            w_u32(destination, 0x30ffffff);
        }
        cursor += 12;
        destination += 12;
    } while (cursor < end);
    return 0;
}

uint32 G3Function80030454(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80030454, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 records = r_u32(descriptor + 0x10);
    uint32 chain = r_u32(descriptor + 12);
    uint32 count = (uint32)(sint32)r_s16(descriptor + 10);
    uint32 model = r_u32(chain);
    uint32 maximum = G3Function80030AEC(records, count);
    w_u16(object + 0x9c, maximum);
    uint32 available = r_u16(0x800a1554);
    uint32 used = r_u16(0x800a1552);
    uint32 capacity = (available - used) << 3;
    if ((sint32)capacity < (sint32)(sint16)(maximum & 0xffff))
        return 0;
    available = r_u16(0x800a1564);
    used = r_u16(0x800a1562);
    uint32 vertices = (uint32)(sint32)r_s16(model + 6);
    capacity = (available - used) << 3;
    if ((sint32)capacity < (sint32)vertices)
        return 0;
    G3Function80030900(object, model);
    count = (uint32)(sint32)r_s16(object + 0x9c);
    G3Function80030954(object, count);
    records = r_u32(descriptor + 0x10);
    uint32 first = r_u32(records);
    G3Function80030710(object, model, first, frame);
    return 1;
}

uint32 G3Function80030AEC(uint32 records, uint32 count)
{
    FUNCTION_MARKER(0x80030aec, "SLUS_008.06");
    sint32 length = (sint16)(count & 0xffff);
    uint32 maximum = 0;
    uint32 index = 0;
    if (length > 0)
    {
        do
        {
            uint32 record = r_u32(records);
            if (record != 0)
            {
                sint32 candidate = r_s16(record);
                bool larger = (sint32)(sint16)(maximum & 0xffff) < candidate;
                uint32 value = r_u16(record);
                if (larger)
                    maximum = value;
            }
            ++index;
            records += 4;
        } while ((sint32)(sint16)(index & 0xffff) < length);
    }
    return (uint32)(sint32)(sint16)(maximum & 0xffff);
}

void G3Function80030900(uint32 object, uint32 model)
{
    FUNCTION_MARKER(0x80030900, "SLUS_008.06");
    sint32 remaining = r_s16(model + 6);
    uint32 link = object + 0x94;
    while (remaining > 0)
    {
        remaining -= 8;
        uint32 node = G3Function800763E0(0x800a1560);
        w_u32(link, node);
        link = node;
    }
    w_u32(link, 0);
}

void G3Function80030954(uint32 object, uint32 count)
{
    FUNCTION_MARKER(0x80030954, "SLUS_008.06");
    uint32 remaining = count;
    uint32 link = object + 0x98;
    while ((sint32)remaining > 0)
    {
        remaining -= 8;
        uint32 node = G3Function800763E0(0x800a1550);
        w_u32(link, node);
        link = node;
    }
    w_u32(link, 0);
}

uint32 G3Function800763E0(uint32 pool)
{
    FUNCTION_MARKER(0x800763e0, "SLUS_008.06");
    uint32 index = r_u16(pool + 2);
    uint32 table = r_u32(pool + 8);
    uint32 element = r_u16(table + (index << 1));
    uint32 stride = r_u16(pool);
    uint32 offset = stride * element;
    w_u16(pool + 2, index + 1);
    uint32 storage = r_u32(pool + 12);
    return storage + offset;
}

uint32 G3Function80030A2C(uint32 data, uint32 bit_offset, uint32 reader)
{
    FUNCTION_MARKER(0x80030a2c, "SLUS_008.06");
    uint32 word_offset = (uint32)((sint32)bit_offset >> 5);
    uint32 cursor = data + (word_offset << 2);
    uint32 bits = 32 - (bit_offset & 31);
    w_u32(reader, cursor);
    uint32 word = r_u32(cursor);
    w_u32(reader + 8, bits);
    w_u32(reader + 4, word >> (bit_offset & 31));
    return bits;
}

uint32 G3Function80030A64(uint32 reader)
{
    FUNCTION_MARKER(0x80030a64, "SLUS_008.06");
    uint32 cursor = r_u32(reader);
    if (cursor == 0)
        return 0;
    uint32 word = r_u32(reader + 4);
    uint32 remaining = r_u32(reader + 8);
    uint32 result = word & 7;
    remaining -= 3;
    w_u32(reader + 4, word >> 3);
    w_u32(reader + 8, remaining);
    if ((sint32)remaining > 0)
        return result;
    cursor = r_u32(reader);
    w_u32(reader, cursor + 4);
    uint32 next = r_u32(cursor + 4);
    remaining = r_u32(reader + 8);
    w_u32(reader + 4, next);
    if ((sint32)remaining < 0)
    {
        result |= (next << ((remaining + 3) & 31)) & 7;
        w_u32(reader + 4, next >> ((uint32(0) - remaining) & 31));
    }
    remaining = r_u32(reader + 8);
    w_u32(reader + 8, remaining + 32);
    return result;
}

uint32 G3Function80030710(uint32 object, uint32 model, uint32 descriptor, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80030710, "SLUS_008.06");
    uint32 frame = guest_stack - 0x80;
    uint32 masks_left = 8;
    uint32 values_left = 8;
    uint32 bit_offset = 8;
    w_u32(frame + 0x84, model);
    w_u32(frame + 0x88, descriptor);
    uint32 mask_node = r_u32(object + 0x94);
    uint32 value_node = r_u32(object + 0x98);
    uint32 count = (uint32)(sint32)r_s16(model + 6);
    uint32 data = r_u32(descriptor + 4);
    uint32 mask_output = mask_node + 4;
    uint32 stride = (count * uint32(3) + 7) & 0xfffffff8;
    uint32 flags = r_u8(data);
    uint32 value_output = value_node + 4;
    if ((flags & 1) != 0)
    {
        G3Function80030A2C(data, 8, frame + 0x28);
        bit_offset = stride + 8;
    }
    else
        w_u32(frame + 0x28, 0);
    if ((flags & 2) != 0)
    {
        uint32 saved_descriptor = r_u32(frame + 0x88);
        data = r_u32(saved_descriptor + 4);
        uint32 start = bit_offset;
        bit_offset += stride;
        G3Function80030A2C(data, start, frame + 0x38);
    }
    else
        w_u32(frame + 0x38, 0);
    if ((flags & 4) != 0)
    {
        uint32 saved_descriptor = r_u32(frame + 0x88);
        data = r_u32(saved_descriptor + 4);
        G3Function80030A2C(data, bit_offset, frame + 0x48);
    }
    else
        w_u32(frame + 0x48, 0);
    uint32 saved_descriptor = r_u32(frame + 0x88);
    w_u32(frame + 0x18, saved_descriptor);
    uint32 result = r_u32(saved_descriptor + 8);
    uint32 saved_model = r_u32(frame + 0x84);
    w_u32(frame + 0x10, result);
    sint32 length = r_s16(saved_model + 6);
    if (length <= 0)
        return result;
    for (sint32 index = 0; index < length; ++index)
    {
        uint32 bits = G3Function80030A64(frame + 0x28);
        uint32 value = G3Function80030A64(frame + 0x48);
        bits |= value << 3;
        value = G3Function80030A64(frame + 0x38);
        bits |= value << 6;
        w_u16(mask_output, bits);
        mask_output += 2;
        while (bits != 0)
        {
            if ((bits & 1) != 0)
            {
                G3Function80030F94(frame + 0x10, value_output, frame);
                --values_left;
                value_output += 8;
                if (values_left == 0)
                {
                    value_node = r_u32(value_node);
                    if (value_node != 0)
                    {
                        value_output = value_node + 4;
                        values_left = 8;
                    }
                }
            }
            bits >>= 1;
        }
        --masks_left;
        if (masks_left == 0)
        {
            mask_node = r_u32(mask_node);
            if (mask_node != 0)
            {
                mask_output = mask_node + 4;
                masks_left = 8;
            }
        }
    }
    return 0;
}

uint32 G3Function80074F64(uint32 object, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80074f64, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 index = (uint32)(sint32)r_s16(state + 0x20);
    uint32 table = r_u32(state + 0x34);
    sint32 start = r_s16(state + 0x22);
    uint32 value = r_u16(table + (index << 1) - 2);
    sint32 finish = r_s16(state + 0x24);
    w_u16(state + 0xe4, value);
    sint32 divisor = r_s16(state + 0xe4);
    sint32 difference = finish - start;
    uint32 interval = divisor != 0 ? (uint32)(difference / divisor) : (difference < 0 ? uint32(1) : uint32(0xffffffff));
    sint32 short_interval = (sint16)(interval & 0xffff);
    uint32 phase = short_interval != 0 ? (uint32)(start / short_interval) : (start < 0 ? uint32(1) : uint32(0xffffffff));
    index = (uint32)(sint32)r_s16(state + 0x20);
    table = r_u32(state + 0x84);
    value = r_u16(table + (index << 1) - 2);
    uint32 controls = r_u32(state);
    w_u16(state + 0xe8, interval);
    w_u16(state + 0xea, value);
    w_u16(state + 0xe6, phase);
    w_u16(controls + 8, 0);
    controls = r_u32(state);
    w_u16(controls + 0x10, 0);
    controls = r_u32(state);
    w_u16(controls, 0);
    controls = r_u32(state);
    w_u16(controls + 2, 0);
    w_u16(state + 0xce, 145);
    w_u32(state + 0x124, 0);
    w_u32(state + 0x174, 0);
    w_u16(state + 0x178, 0);
    w_u32(state + 0x17c, 0);
    w_u32(state + 0x1fc, 0);
    w_u32(state + 0x230, 0);
    w_u16(state + 0xe2, 0);
    w_u16(state + 0xbc, 0);
    w_u16(state + 0x188, 4096);
    w_u32(state + 0x1b8, 0);
    w_u32(state + 0x1e8, 0);
    uint32 game = r_u32(0x80097048);
    w_u32(state + 0x1bc, 0);
    w_u32(state + 0x214, 0);
    w_u32(state + 0x220, 0);
    w_u32(state + 0x224, 0);
    w_u16(state + 0x256, 0);
    w_u16(state + 0x258, 0);
    w_u16(state + 0x254, 4096);
    w_u16(state + 0x25e, 0);
    w_u16(state + 0x260, 0);
    w_u16(state + 0x120, 0);
    w_u16(state + 0x264, 0);
    w_u16(state + 0x266, 0);
    w_u16(state + 0x268, 0);
    uint32 level = r_u32(game + 0x48);
    value = r_u32(level + 0x6c);
    w_u16(state + 0x9c, 0);
    w_u16(state + 0x9e, 0);
    w_u16(state + 0xa0, 0);
    w_u32(state + 0x1a8, 0);
    w_u32(state + 0x19c, 0);
    w_u32(state + 0x1a4, 0);
    w_u32(state + 0x198, 0);
    w_u32(state + 0x1ec, value);
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 chain = r_u32(descriptor + 12);
    uint32 owner = r_u32(0x800a0284);
    uint32 model = r_u32(chain);
    w_u32(state + 0x1ac, model);
    w_u32(state + 0x1a0, model);
    w_u32(owner + 0x10, 0);
    w_u32(owner + 12, 0);
    uint32 flags = r_u32(game + 0x6834);
    w_u32(game + 0x6834, flags & 0xffffffea);
    sint32 current = r_s16(state + 0x18);
    sint32 previous = r_s16(game + 0x681e);
    uint32 current_bits = r_u16(state + 0x18);
    uint32 previous_bits = r_u16(game + 0x681e);
    value = current < previous ? previous_bits : current_bits;
    w_u16(game + 0x681e, value);
    w_u16(game + 0x681c, value);
    w_u32(game + 0x6820, 0);
    value = (uint32)(sint32)r_s16(state + 4);
    uint32 lives = r_u32(game + 0x6818);
    w_u32(game + 0x67b8, value);
    if (lives == 0)
    {
        value = (uint32)(sint32)r_s16(state + 0x1a);
        w_u32(game + 0x6818, value);
    }
    uint32 name = state + 0x48;
    if (name != 0)
    {
        uint32 resource = G3Function8003AA48(name);
        if (resource != 0)
        {
            uint32 target = r_u32(resource + 0x34);
            if (target != 0)
            {
                game = r_u32(0x80097048);
                G3DispatchStateEvent(target, 0, game, 1, 0, frame);
            }
        }
    }
    uint32 initialized = r_u32(0x800a02e0);
    if (initialized == 0)
    {
        w_u32(0x800a02e0, 1);
        G3Function8006D9D8(frame);
        G3Function8006DB98();
    }
    return G3Function8006DC34();
}

uint32 G3Function8006D93C(uint32 type, uint32 model)
{
    FUNCTION_MARKER(0x8006d93c, "SLUS_008.06");
    uint32 count = (uint32)(sint32)r_s16(model + 6);
    uint32 cursor = r_u32(model + 0x18);
    uint32 end = cursor + count * uint32(24);
    uint32 index = 0;
    uint32 result = 0xffffffff;
    while (cursor < end)
    {
        uint32 data = r_u32(cursor + 0x14);
        if (data != 0)
        {
            uint32 entries = r_u32(data + 8);
            if (entries != 0)
            {
                uint32 entry = r_u32(data + 12);
                uint32 limit = entry + (entries << 4);
                while (entry < limit)
                {
                    uint32 value = r_u8(entry + 4);
                    if (value == type)
                        result = index;
                    entry += 16;
                }
            }
        }
        cursor += 24;
        ++index;
    }
    return result;
}

uint32 G3Function8006D9D8(uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006d9d8, "SLUS_008.06");
    uint32 object = r_u32(0x8009efc4);
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 chain = r_u32(descriptor + 12);
    uint32 model = r_u32(chain);
    uint32 state = r_u32(object + 0x24);
    uint32 found = G3Function8006D93C(9, model);
    w_u16(state + 0x1b2, found);
    uint32 index = (uint32)(sint32)(sint16)(found & 0xffff);
    if (index != 0xffffffff)
    {
        uint32 table = r_u32(model + 0x18);
        uint32 value = r_u32(table + index * uint32(24) + 20);
        w_u32(state + 0xf0, value);
    }
    found = G3Function8006D93C(8, model);
    w_u16(state + 0x1b4, found);
    index = (uint32)(sint32)(sint16)(found & 0xffff);
    if (index != 0xffffffff)
    {
        uint32 table = r_u32(model + 0x18);
        uint32 value = r_u32(table + index * uint32(24) + 20);
        w_u32(state + 0xf4, value);
    }
    found = G3Function8006D93C(0, model);
    w_u16(state + 0x1b0, found);
    index = (uint32)(sint32)(sint16)(found & 0xffff);
    uint32 result = index << 1;
    if (index != 0xffffffff)
    {
        uint32 table = r_u32(model + 0x18);
        uint32 entry = table + index * uint32(24);
        uint32 value = r_u32(entry + 20);
        w_u32(state + 0xf8, value);
        value = r_u32(entry + 20);
        uint32 data = r_u32(value + 12);
        result = r_u16(data + 12);
        w_u16(state + 0x18a, result);
    }
    return result;
}

uint32 G3Function8006DB98(void)
{
    FUNCTION_MARKER(0x8006db98, "SLUS_008.06");
    uint32 object = r_u32(0x8009efc4);
    uint32 state = r_u32(object + 0x24);
    uint32 first = (uint32)(sint32)r_s16(state + 0x1b2);
    uint32 model = r_u32(state + 0x1ac);
    if (first != 0xffffffff)
    {
        uint32 table = r_u32(model + 0x18);
        uint32 entry = table + first * uint32(24);
        w_u32(state + 0xfc, entry + 20);
        w_u32(entry + 20, 0);
    }
    uint32 second = (uint32)(sint32)r_s16(state + 0x1b4);
    if (first != 0xffffffff)
    {
        uint32 table = r_u32(model + 0x18);
        uint32 entry = table + second * uint32(24);
        w_u32(state + 0x100, entry + 20);
        w_u32(entry + 20, 0);
    }
    uint32 third = (uint32)(sint32)r_s16(state + 0x1b0);
    uint32 result = third << 1;
    if (third != 0xffffffff)
    {
        uint32 table = r_u32(model + 0x18);
        uint32 entry = table + third * uint32(24);
        result = entry + 20;
        w_u32(state + 0x104, result);
        w_u32(entry + 20, 0);
    }
    return result;
}

uint32 G3Function8006DC34(void)
{
    FUNCTION_MARKER(0x8006dc34, "SLUS_008.06");
    uint32 object = r_u32(0x8009efc4);
    uint32 state = r_u32(object + 0x24);
    uint32 model = r_u32(state + 0x1a8);
    if (model != 0)
    {
        uint32 first = (uint32)(sint32)r_s16(state + 0x1b2);
        uint32 table = r_u32(model + 0x18);
        uint32 second = (uint32)(sint32)r_s16(state + 0x1b4);
        w_u32(state + 0x108, table + first * uint32(24) + 20);
        model = r_u32(state + 0x1a8);
        table = r_u32(model + 0x18);
        uint32 third = (uint32)(sint32)r_s16(state + 0x1b0);
        w_u32(state + 0x10c, table + second * uint32(24) + 20);
        model = r_u32(state + 0x1a8);
        table = r_u32(model + 0x18);
        w_u32(state + 0x110, table + third * uint32(24) + 20);
    }
    else
    {
        uint32 first = r_u32(state + 0xfc);
        uint32 second = r_u32(state + 0x100);
        uint32 third = r_u32(state + 0x104);
        w_u32(state + 0x108, first);
        w_u32(state + 0x10c, second);
        w_u32(state + 0x110, third);
    }
    model = r_u32(state + 0x1a4);
    if (model != 0)
    {
        uint32 first = (uint32)(sint32)r_s16(state + 0x1b2);
        uint32 table = r_u32(model + 0x18);
        uint32 second = (uint32)(sint32)r_s16(state + 0x1b4);
        w_u32(state + 0x114, table + first * uint32(24) + 20);
        model = r_u32(state + 0x1a4);
        table = r_u32(model + 0x18);
        uint32 third = (uint32)(sint32)r_s16(state + 0x1b0);
        w_u32(state + 0x118, table + second * uint32(24) + 20);
        model = r_u32(state + 0x1a4);
        table = r_u32(model + 0x18);
        uint32 result = table + third * uint32(24) + 20;
        w_u32(state + 0x11c, result);
        return result;
    }
    uint32 first = r_u32(state + 0xfc);
    uint32 second = r_u32(state + 0x100);
    uint32 third = r_u32(state + 0x104);
    w_u32(state + 0x114, first);
    w_u32(state + 0x118, second);
    w_u32(state + 0x11c, third);
    return first;
}

uint32 G3Function80030E9C(uint32 reader, uint32 destination)
{
    FUNCTION_MARKER(0x80030e9c, "SLUS_008.06");
    w_u32(destination + 4, 0x80030bdc);
    uint32 descriptor = r_u32(reader + 8);
    uint32 count = (uint32)(sint32)r_s16(descriptor + 2);
    uint32 cursor = r_u32(reader);
    uint32 bytes = count << 1;
    w_u32(reader, cursor + bytes);
    return bytes;
}

uint32 G3Function80030EC8(uint32 reader, uint32 destination)
{
    FUNCTION_MARKER(0x80030ec8, "SLUS_008.06");
    uint32 cursor = r_u32(reader);
    uint32 value = r_u16(cursor);
    w_u16(destination + 2, value);
    w_u32(reader, cursor + 2);
    value = r_u16(destination + 2);
    w_u32(destination + 4, 0x80030c10);
    value = (uint32)((sint32)(value << 19) >> 19);
    w_u16(destination + 2, value);
    return 0x80030c10;
}

uint32 G3Function80030F00(uint32 reader, uint32 destination)
{
    FUNCTION_MARKER(0x80030f00, "SLUS_008.06");
    uint32 cursor = r_u32(reader);
    uint32 value = r_u8(cursor);
    w_u16(destination, value);
    cursor = r_u32(reader);
    value = r_u16(cursor + 2);
    w_u32(destination + 4, 0x80030c2c);
    w_u16(destination + 2, value);
    uint32 descriptor = r_u32(reader + 8);
    uint32 count = (uint32)(sint32)r_s16(descriptor + 2);
    cursor = r_u32(reader);
    uint32 bytes = ((uint32)((sint32)(count + 2) >> 2) << 1) + 4;
    w_u32(reader, cursor + bytes);
    return bytes;
}

uint32 G3Function80030F58(uint32 reader, uint32 destination)
{
    FUNCTION_MARKER(0x80030f58, "SLUS_008.06");
    uint32 cursor = r_u32(reader);
    uint32 header = r_u16(cursor);
    uint32 value = r_u16(cursor + 2);
    w_u16(destination, 0);
    w_u32(destination + 4, 0x80030d8c);
    w_u16(destination + 2, value);
    cursor = r_u32(reader);
    uint32 result = cursor + ((header & 0xfff) << 1);
    w_u32(reader, result);
    return result;
}

uint32 G3Function80030F94(uint32 reader, uint32 destination, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80030f94, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 cursor = r_u32(reader);
    uint32 header = r_u16(cursor);
    uint32 selector = (header >> 12) & 14;
    if (selector == 14)
        selector = 0;
    uint32 target = r_u32(0x80097828 + (selector << 2));
    return G3DispatchModelDecoder(target, reader, destination, selector, frame);
}

uint32 G3Function80030BDC(uint32 reader)
{
    FUNCTION_MARKER(0x80030bdc, "SLUS_008.06");
    uint32 index = (uint32)(sint32)r_s16(reader + 6);
    uint32 descriptor = r_u32(reader + 8);
    uint32 cursor = r_u32(reader);
    uint32 count = (uint32)(sint32)r_s16(descriptor + 2);
    uint32 value = r_u16(cursor + (index << 1));
    w_u32(reader, cursor + (count << 1));
    return (uint32)(sint32)(sint16)(value & 0xffff);
}

uint32 G3Function80030C10(uint32 reader)
{
    FUNCTION_MARKER(0x80030c10, "SLUS_008.06");
    uint32 cursor = r_u32(reader);
    uint32 state = r_u32(reader + 0x10);
    w_u32(reader, cursor + 2);
    return (uint32)(sint32)r_s16(state + 2);
}

uint32 G3Function80030C2C(uint32 reader)
{
    FUNCTION_MARKER(0x80030c2c, "SLUS_008.06");
    uint32 state = r_u32(reader + 0x10);
    uint32 position = (uint32)(sint32)r_s16(reader + 4);
    uint32 cursor = r_u32(reader);
    sint32 limit = r_s16(reader + 6);
    uint32 data = cursor + 4;
    uint32 accumulator = r_u16(state + 2);
    uint32 step = r_u16(state);
    while ((sint32)position < limit)
    {
        uint32 word_offset = (uint32)((sint32)position >> 2) << 1;
        uint32 word = r_u16(data + word_offset);
        uint32 packed = word >> ((position & 3) << 2);
        uint32 nibble = packed & 15;
        uint32 scale_offset = (uint32)((sint32)(step << 16) >> 15);
        uint32 change = r_u8(0x80097798 + nibble);
        uint32 scale = r_u16(0x800977a8 + scale_offset);
        step += (uint32)(sint32)(sint8)(change & 0xff);
        if ((sint32)(step << 16) < 0)
            step = 0;
        if ((sint32)(sint16)(step & 0xffff) >= 64)
            step = 63;
        bool subtract = (packed & 8) != 0;
        if (subtract)
            nibble = packed & 7;
        uint32 magnitude = scale >> 3;
        if ((nibble & 4) != 0)
            magnitude = scale + (scale >> 3);
        if ((nibble & 2) != 0)
            magnitude += scale >> 1;
        if ((nibble & 1) != 0)
            magnitude += (scale >> 2) + (scale & 1);
        accumulator = subtract ? accumulator - magnitude : accumulator + magnitude;
        ++position;
    }
    w_u16(state, step);
    w_u16(state + 2, accumulator);
    uint32 descriptor = r_u32(reader + 8);
    uint32 count = (uint32)(sint32)r_s16(descriptor + 2);
    cursor = r_u32(reader);
    uint32 bytes = ((uint32)((sint32)(count + 2) >> 2) << 1) + 4;
    w_u32(reader, cursor + bytes);
    return (uint32)(sint32)(sint16)(accumulator & 0xffff);
}

uint32 G3Function80030D8C(uint32 reader)
{
    FUNCTION_MARKER(0x80030d8c, "SLUS_008.06");
    uint32 position = (uint32)(sint32)r_s16(reader + 4);
    uint32 cursor = r_u32(reader);
    uint32 state = r_u32(reader + 0x10);
    uint32 header = r_u16(cursor);
    uint32 saved = r_u16(state);
    uint32 bytes = (header & 0xfff) << 1;
    uint32 segment = (uint32)((sint32)(saved << 16) >> 21);
    uint32 entry = r_u16(cursor + (segment << 1) + 4);
    uint32 phase = saved & 31;
    uint32 delta = (uint32)((sint32)(entry << 16) >> 21);
    sint32 limit = r_s16(reader + 6);
    uint32 duration = entry & 31;
    if ((sint32)position < limit)
    {
        ++phase;
        bool again;
        do
        {
            if ((phase & 0xffff) == duration)
            {
                ++segment;
                uint32 accumulator = r_u16(state + 2);
                w_u16(state + 2, accumulator + delta);
                cursor = r_u32(reader);
                uint32 offset = (uint32)((sint32)(segment << 16) >> 15);
                entry = r_u16(cursor + offset + 4);
                phase = 0;
                duration = entry & 31;
                delta = (uint32)((sint32)(entry << 16) >> 21);
            }
            limit = r_s16(reader + 6);
            ++position;
            again = (sint32)position < limit;
            ++phase;
        } while (again);
        --phase;
    }
    w_u16(state, phase | (segment << 5));
    cursor = r_u32(reader);
    w_u32(reader, cursor + bytes);
    uint32 fraction = phase & 0xffff;
    if (fraction == 0)
        return (uint32)(sint32)r_s16(state + 2);
    uint32 product = fraction * delta;
    uint32 adjustment = duration != 0 ? (uint32)((sint32)product / (sint32)duration) : ((sint32)product < 0 ? uint32(1) : uint32(0xffffffff));
    uint32 accumulator = r_u16(state + 2);
    uint32 result = accumulator + adjustment;
    return (uint32)(sint32)(sint16)(result & 0xffff);
}

uint32 G3Function8004D8B8(uint32 object, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004d8b8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    G3Function80013D38(object, frame);
    G3Function80012FA4(object, frame);
    uint32 value = r_u16(settings + 4);
    w_u16(0x80096a66, value);
    value = r_u16(settings + 4);
    w_u16(0x80096a68, value);
    return value;
}

uint32 G3Function80056064(void)
{
    FUNCTION_MARKER(0x80056064, "SLUS_008.06");
    G3FillMemory80086094(0x800a2494, 0, 248);
    G3FillMemory80086094(0x800a2488, 0, 12);
    G3FillMemory80086094(0x800a258c, 0, 24);
    uint32 enabled = r_u32(0x800a0628);
    G3Function80056044(enabled);
    w_u32(0x800a25a0, 2340);
    return 2340;
}

uint32 G3Function80056044(uint32 enabled)
{
    FUNCTION_MARKER(0x80056044, "SLUS_008.06");
    w_u32(0x800a258c, enabled != 0 ? 1 : 0);
    return 1;
}

uint32 G3Function80058298(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80058298, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    switch (message)
    {
    case 0:
        G3Function8005624C(object, frame);
        return 1;
    case 1:
        G3Function800564A4(object, game, frame);
        return 1;
    case 4:
        G3Function80056894(object, game, payload, frame);
        return 1;
    case 6:
        G3Function80056064();
        return 1;
    case 7:
    {
        uint32 mode = r_u32(0x80096a88);
        if (mode == 19)
            return 0;
        return G3Function80056924();
    }
    case 8:
    case 9:
        return 1;
    case 10:
        G3Function80056934(payload, frame);
        return 1;
    case 11:
        G3Function80057A34(object, game, payload, frame);
        return 1;
    case 12:
    case 13:
    {
        sint32 previous = r_s16(0x800a24cc);
        sint32 current = r_s16(0x80096b68);
        uint32 current_bits = r_u16(0x80096b68);
        if (previous != current)
        {
            w_u16(0x800a24cc, current_bits);
            G3Function80058798(0xffffffff);
        }
        uint32 angle = (uint32)(sint32)r_s16(0x80096b6a);
        uint32 distance = (uint32)(sint32)r_s16(0x800a24cc);
        uint32 value = G3Function800571F8(angle, distance);
        sint32 saved = r_s16(0x800a2524);
        if (saved == (sint32)(sint16)(value & 0xffff))
            return 1;
        w_u16(0x800a2524, value);
        G3Function80058798(0xffffffff);
        G3Function80059E70(object, 0x800a2494, frame);
        return 1;
    }
    case 16:
        G3Function8005617C(object, game, payload, frame);
        return 1;
    case 17:
    {
        uint32 mode = r_u32(0x80096a8c);
        w_u32(0x800a24f8, payload);
        w_u16(0x800a24cc, payload);
        w_u32(0x800a2548, payload);
        if (mode == 0)
            G3Function8005617C(object, game, 0, frame);
        return 1;
    }
    case 18:
    {
        uint32 distance = (uint32)(sint32)r_s16(0x800a24cc);
        uint32 angle = (uint32)(sint32)(sint16)(payload & 0xffff);
        uint32 value = G3Function800571F8(angle, distance);
        uint32 mode = r_u32(0x80096a8c);
        w_u16(0x800a2524, value);
        if (mode == 0)
            G3Function8005617C(object, game, 0, frame);
        else
        {
            G3Function80058798((sint32)mode < 51 ? uint32(0xffffffff) : uint32(32));
            G3Function80059E70(object, 0x800a2494, frame);
        }
        return 1;
    }
    case 19:
        w_u32(0x800a24ec, payload);
        G3Function8005617C(object, game, 0, frame);
        return 1;
    case 22:
    case 23:
        G3Function800560CC(object);
        return 1;
    case 24:
    {
        if (payload != 19 && payload != 6)
            return 0;
        uint32 player = r_u32(0x8009efc4);
        uint32 state = r_u32(player + 0x150);
        return state == 1 ? 1 : 0;
    }
    case 26:
        G3Function80056044(payload);
        return 1;
    default:
        return 0;
    }
}

uint32 G3Function80056924(void)
{
    FUNCTION_MARKER(0x80056924, "SLUS_008.06");
    uint32 mode = r_u32(0x80096a8c);
    return mode != 0 ? 1 : 0;
}

uint32 G3Function800560CC(uint32 source)
{
    FUNCTION_MARKER(0x800560cc, "SLUS_008.06");
    w_u32(0x800a2488, 1);
    uint32 destination = 0x800a248c;
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
    return 1;
}

uint32 G3Function80058798(uint32 duration)
{
    FUNCTION_MARKER(0x80058798, "SLUS_008.06");
    uint32 value = duration;
    if ((sint32)(duration << 16) <= 0)
        value = r_u16(0x8009edd0);
    w_u16(0x800a2586, value);
    uint32 flags = r_u32(0x800a24d0);
    uint32 result = flags | 0x200;
    w_u32(0x800a24d0, result);
    return result;
}

uint32 G3Function800571F8(uint32 angle, uint32 distance)
{
    FUNCTION_MARKER(0x800571f8, "SLUS_008.06");
    uint32 signed_angle = (uint32)(sint32)(sint16)(angle & 0xffff);
    uint32 sine = G3Function8005B590(signed_angle);
    uint32 cosine = G3Function8005B470(signed_angle);
    sint32 divisor = (sint16)(cosine & 0xffff);
    if (divisor == 0)
        return (uint32)(sint32)(sint16)(distance & 0xffff);
    sint32 scaled_distance = (sint16)(distance & 0xffff);
    sint32 signed_sine = (sint16)(sine & 0xffff);
    sint32 numerator = -(scaled_distance * signed_sine);
    uint32 quotient = (uint32)(numerator / divisor);
    return (uint32)(sint32)(sint16)(quotient & 0xffff);
}

uint32 G3Function8005624C(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005624c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    G3FillMemory80086094(0x800a2494, 0, 248);
    uint32 x = r_u16(0x80096b38);
    uint32 y = r_u16(0x80096b3a);
    uint32 z = r_u16(0x80096b3c);
    uint32 owner = r_u32(0x80096b34);
    w_u16(frame + 0x10, x);
    w_u16(frame + 0x12, y);
    w_u16(frame + 0x14, z);
    for (uint32 copy = 0; copy < 3; ++copy)
    {
        uint32 source = copy < 2 ? frame + 0x10 : owner + 0xdc;
        uint32 destination = copy == 0 ? 0x800a2494 : (copy == 1 ? 0x800a249a : 0x800a2540);
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
    G3Function80050C5C(frame + 0x10, frame);
    G3Function80050B94(0x800a2494, frame);
    uint32 distance_bits = r_u16(0x80096b68);
    uint32 angle = (uint32)(sint32)r_s16(0x80096b6a);
    uint32 value = (uint32)(sint32)r_s16(object + 0x14);
    w_u16(0x800a24cc, distance_bits);
    uint32 distance = (uint32)(sint32)(sint16)(distance_bits & 0xffff);
    w_u32(0x800a24ec, value);
    w_u32(0x800a24f8, distance);
    w_u32(0x800a2548, distance);
    value = r_u16(object + 0x18);
    w_u16(0x800a2526, value);
    value = r_u16(object + 0x18);
    w_u16(0x800a252a, value);
    uint32 projected = G3Function800571F8(angle, distance);
    w_u16(0x800a2524, projected);
    uint32 height = (uint32)(sint32)r_s16(0x800a2498);
    uint32 origin = (uint32)(sint32)r_s16(object + 4);
    uint32 offset = origin - ((uint32)(sint32)(sint16)(projected & 0xffff) + height);
    w_u32(0x800a2534, offset);
    if ((sint32)offset < -200)
        w_u32(0x800a2534, 0xffffff38);
    return 0xffffff38;
}

uint32 G3Function80050C5C(uint32 vector, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80050c5c, "SLUS_008.06");
    uint32 x = (uint32)(sint32)r_s16(vector);
    uint32 frame = guest_stack - 0x10;
    w_u32(frame, x);
    uint32 y = (uint32)(sint32)r_s16(vector + 2);
    w_u32(frame + 4, y);
    uint32 z = (uint32)(sint32)r_s16(vector + 4);
    w_u32(frame + 4, y << 16);
    uint32 owner = r_u32(0x8009ea6c);
    w_u32(frame, x << 16);
    w_u32(frame + 8, z);
    w_u32(frame + 8, z << 16);
    x = r_u32(frame);
    y = r_u32(frame + 4);
    z = r_u32(frame + 8);
    w_u32(owner, x);
    w_u32(owner + 4, y);
    w_u32(owner + 8, z);
    x = r_u32(frame);
    y = r_u32(frame + 4);
    z = r_u32(frame + 8);
    w_u32(owner + 12, x);
    w_u32(owner + 0x10, y);
    w_u32(owner + 0x14, z);
    w_u32(owner + 0x18, 0);
    w_u32(owner + 0x1c, 0);
    w_u32(owner + 0x20, 0);
    return owner;
}

uint32 G3Function80050B94(uint32 vector, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80050b94, "SLUS_008.06");
    uint32 owner = r_u32(0x8009ea6c);
    uint32 raw_x = r_u32(owner + 12);
    uint32 frame = guest_stack - 0x10;
    w_u32(frame, raw_x);
    sint32 x = (sint32)raw_x >> 16;
    uint32 raw_y = r_u32(owner + 0x10);
    w_u32(frame + 4, raw_y);
    uint32 raw_z = r_u32(owner + 0x14);
    sint32 y = (sint32)raw_y >> 16;
    w_u32(frame, (uint32)x);
    w_u32(frame + 4, (uint32)y);
    w_u32(frame + 8, raw_z);
    sint32 z = (sint32)raw_z >> 16;
    w_u32(frame + 8, (uint32)z);
    if (x < -32767)
        x = -32767;
    if (x > 32767)
        x = 32767;
    w_u16(vector, x);
    y = r_s32(frame + 4);
    if (y < -32767)
        y = -32767;
    if (y > 32767)
        y = 32767;
    w_u16(vector + 2, y);
    z = r_s32(frame + 8);
    if (z < -32767)
        z = -32767;
    uint32 result = z > 32767 ? 1 : 0;
    if (result != 0)
        z = 32767;
    w_u16(vector + 4, z);
    return result;
}

uint32 G3Function8005617C(uint32 object, uint32 game, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005617c, "SLUS_008.06");
    uint32 enabled = r_u32(0x800a2488);
    uint32 frame = guest_stack - 0x18;
    if (enabled != 0)
    {
        w_u32(0x800a2488, 0);
        for (uint32 copy = 0; copy < 3; ++copy)
        {
            uint32 source = copy == 0 ? 0x800a248c : (copy == 1 ? 0x80096b38 : 0x800a2494);
            uint32 destination = copy == 0 ? object : (copy == 1 ? 0x800a2494 : 0x800a249a);
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
    }
    else if (payload == 0)
        G3Function800560FC(object, frame);
    uint32 value = r_u16(object);
    w_u16(0x80096a54, value);
    value = r_u16(object + 2);
    w_u16(0x80096a56, value);
    value = r_u16(object + 4);
    w_u16(0x80096a58, value);
    return value;
}

uint32 G3Function800560FC(uint32 vector, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800560fc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 distance = r_u16(0x800a24cc);
    uint32 height = r_u16(0x800a2524);
    uint32 angle = (uint32)(sint32)r_s16(0x800a24ec);
    w_u16(frame + 0x10, 0);
    w_u16(frame + 0x12, uint32(0) - distance);
    w_u16(frame + 0x14, height);
    G3Function8002F8E4(frame + 0x10, angle);
    uint32 value = r_u16(frame + 0x10);
    uint32 origin = r_u16(0x800a2494);
    w_u16(vector, value + origin);
    value = r_u16(frame + 0x12);
    origin = r_u16(0x800a2496);
    w_u16(vector + 2, value + origin);
    value = r_u16(frame + 0x14);
    origin = r_u16(0x800a2498);
    uint32 result = value + origin;
    w_u16(vector + 4, result);
    return result;
}

uint32 G3Function8002F8E4(uint32 vector, uint32 angle)
{
    FUNCTION_MARKER(0x8002f8e4, "SLUS_008.06");
    uint32 signed_angle = (uint32)(sint32)(sint16)(angle & 0xffff);
    uint32 x_bits = r_u16(vector);
    uint32 y_bits = r_u16(vector + 2);
    uint32 first = G3Function8005B590(signed_angle);
    uint32 second = G3Function8005B470(signed_angle);
    sint32 x = (sint16)(x_bits & 0xffff);
    sint32 y = (sint16)(y_bits & 0xffff);
    sint32 a = (sint16)(first & 0xffff);
    sint32 b = (sint16)(second & 0xffff);
    uint32 bx = (uint32)(b * x);
    uint32 ay = (uint32)(a * y);
    uint32 ax = (uint32)(a * x);
    uint32 by = (uint32)(b * y);
    uint32 output_x = (uint32)((sint32)(bx - ay) >> 12);
    w_u16(vector, output_x);
    uint32 result = (uint32)((sint32)(ax + by) >> 12);
    w_u16(vector + 2, result);
    return result;
}
