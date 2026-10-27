#include "xport.h"
#include "psx.h"
#include "game_functions.h"

uint32 G3Function8002AF90(uint32 command)
{
    FUNCTION_MARKER(0x8002af90, "SLUS_008.06");
    uint32 count = r_u32(command + 8);
    uint32 record = command + 12;
    uint32 end = command + (count << 2);
    uint32 object = 0;
    while (record < end)
    {
        uint32 word = r_u32(record);
        uint32 expected = r_u32(0x8009d8bc);
        if (word == expected)
        {
            uint32 step = r_u32(record + 8);
            record += step << 2;
            if (record < end)
                object = G3Function8003AA48(record);
        }
        else
        {
            expected = r_u32(0x8009d8c4);
            bool clear = word == expected;
            bool selected = clear;
            if (!selected)
            {
                expected = r_u32(0x8009d8d8);
                selected = word == expected;
            }
            if (selected)
            {
                word = r_u32(record + 12);
                expected = r_u32(0x8009d8cc);
                if (word == expected)
                {
                    word = r_u32(record + 16);
                    expected = r_u32(0x8009d8d0);
                    if (word == expected && object != 0)
                    {
                        uint32 flags = r_u32(object + 0x30);
                        w_u32(object + 0x30, clear ? flags & 0xff7fffff : flags | 0x00800000);
                    }
                }
            }
        }
        uint32 step = r_u32(record + 8);
        record += step << 2;
    }
    return 0;
}

uint32 G3Function80027960(uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80027960, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 word = r_u32(command + 12);
    uint32 expected = r_u32(0x8009d7ac);
    if (word == expected)
    {
        word = r_u32(command + 16);
        expected = r_u32(0x8009d7b0);
        if (word == expected)
        {
            uint32 game = r_u32(0x80097048);
            G3Function800277A4(game, command + 24, 1);
            return 0;
        }
    }
    word = r_u32(command + 12);
    expected = r_u32(0x8009d7b8);
    if (word == expected)
    {
        uint32 second = r_u32(command + 16);
        expected = r_u32(0x8009d7bc);
        if (second == expected)
        {
            uint32 game = r_u32(0x80097048);
            G3Function800277A4(game, command + 24, 0);
            return 0;
        }
    }
    else
        word = r_u32(command + 12);
    expected = r_u32(0x8009d7c4);
    if (word == expected)
    {
        uint32 game = r_u32(0x80097048);
        uint32 valid = G3Function8002786C(game, command + 24, frame + 0x10, 0);
        if (valid == 0)
            return 0xffffffff;
        uint32 branch = G3Function80027904(command, 0x8009d7cc);
        uint32 value = r_u32(frame + 0x10);
        if (branch != 0)
            return value != 0 ? r_u32(branch + 12) : 0xfffffffe;
        return value != 0 ? 0xfffffffe : 0xffffffff;
    }
    expected = r_u32(0x8009d7d4);
    if (word != expected)
        return 0;
    word = r_u32(command + 16);
    expected = r_u32(0x8009d7d8);
    if (word != expected)
        return 0;
    uint32 placement = r_u32(command + 24);
    if (placement == 0)
        return 0;
    uint32 bit = G3Function80039DFC(placement);
    uint32 game = r_u32(0x80097048);
    uint32 index = r_u16(game + 0x68b4);
    sint32 flags = r_s16(game + (index << 1) + 0x6874);
    return (((uint32)(flags >> (bit & 31))) & 1) != 0 ? 0xfffffffe : 0xffffffff;
}

uint32 G3Function80057C44(uint32 camera, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80057c44, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    const uint32 keys[15] = {0x8009ecf8, 0x8009ed04, 0x8009ed10, 0x8009ed1c, 0x8009ed28, 0x8009ed34, 0x8009ed40, 0x8009ed4c, 0x8009ed68, 0x8009ed74, 0x8009ed80, 0x8009ed8c, 0x8009ed98, 0x8009eda4, 0x8009edb0};
    uint32 word = 0;
    bool previous_first_match = false;
    for (uint32 operation = 0; operation < 15; ++operation)
    {
        if (operation == 0 || (operation & 1) != 0 || !previous_first_match)
            word = r_u32(command + 12);
        uint32 expected = r_u32(keys[operation]);
        previous_first_match = word == expected;
        if (!previous_first_match)
            continue;
        uint32 second = r_u32(command + 16);
        expected = r_u32(keys[operation] + 4);
        if (second != expected)
            continue;
        switch (operation)
        {
        case 0:
        {
            w_u32(0x800a2590, 1);
            uint32 result = r_u16(camera + 0x18);
            w_u16(0x800a2526, result);
            return result;
        }
        case 1:
            w_u32(0x800a2590, 0);
            return 0x800a0000;
        case 2:
        {
            uint32 distance = r_u32(command + 24);
            uint32 current = r_u32(0x800a24f8);
            if ((sint32)distance >= (sint32)current)
                return 0;
            w_u32(0x800a24f8, distance);
            uint32 value = r_u16(command + 24);
            w_u16(0x800a24cc, value);
            value = r_u32(command + 24);
            w_u32(0x800a2548, value);
            return value;
        }
        case 3:
        {
            uint32 game = r_u32(0x80097048);
            uint32 result = G3Function80023A68(game);
            if (result != 0)
                return result;
            uint32 source = r_u32(command + 24);
            if (source == 0)
                return 0;
            for (uint32 axis = 0; axis < 3; ++axis)
            {
                uint32 target = r_u16(source + axis * 2);
                uint32 current = r_u16(camera + axis * 2);
                w_u16(frame + 0x10 + axis * 2, target - current);
            }
            uint32 target_angle = r_u16(source + 0xc);
            uint32 current_angle = r_u16(camera + 0x1c);
            game = r_u32(0x80097048);
            w_u16(frame + 0x16, target_angle - current_angle);
            return G3Function80057A34(camera, game, frame + 0x10, frame);
        }
        case 4:
        case 5:
        {
            uint32 game = r_u32(0x80097048);
            uint32 result = G3Function80023A68(game);
            if (result != 0)
                return result;
            uint32 source = r_u32(command + 24);
            if (source == 0)
                return 0;
            uint32 offset = operation == 4 ? 0 : 0x10;
            for (uint32 axis = 0; axis < 3; ++axis)
            {
                uint32 value = r_u16(source + offset + axis * 2);
                w_u16(camera + axis * 2, value);
            }
            return G3Function8005798C(camera, frame);
        }
        case 6:
        {
            uint32 tilt = r_u16(0x80096b6a);
            uint32 distance = r_u16(0x80096b68);
            w_u16(0x800a24cc, distance);
            w_u16(0x800a2526, tilt);
            uint32 result = G3Function800571F8((uint32)(sint32)(sint16)tilt, (uint32)(sint32)(sint16)distance);
            w_u16(0x800a2524, result);
            return result;
        }
        case 7:
        {
            uint32 mode = r_u32(command + 24);
            expected = r_u32(0x8009ed58);
            if (mode == expected)
            {
                w_u32(0x800a258c, 2);
                return 2;
            }
            expected = r_u32(0x8009ed60);
            w_u32(0x800a258c, mode == expected ? 1 : 0);
            return 1;
        }
        case 8:
        case 13:
        {
            uint32 result = r_u32(command + 24) == 0;
            w_u32(operation == 8 ? 0x800a2598 : 0x800a259c, result);
            return result;
        }
        case 9:
        {
            uint32 result = r_u32(0x80096b28) | 0x200;
            w_u32(0x80096b28, result);
            uint32 words[3];
            for (uint32 index = 0; index < 3; ++index)
            {
                uint32 source = command + 24 + index * 4;
                uint32 high_address = source + 3;
                uint32 high_lane = high_address & 3;
                uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
                uint32 low_lane = source & 3;
                uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
                words[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
            }
            for (uint32 index = 0; index < 3; ++index)
            {
                uint32 destination = 0x80096b5c + index * 4;
                uint32 high_address = destination + 3;
                uint32 high_lane = high_address & 3;
                w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
                uint32 low_lane = destination & 3;
                w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15u << low_lane) & 15);
            }
            return result;
        }
        case 10:
        case 11:
        case 12:
        {
            uint32 flags = r_u32(0x80096b28);
            uint32 result = operation == 10 ? flags & 0xfffffdff : (operation == 11 ? flags | 0x400 : flags & 0xfffffbff);
            w_u32(0x80096b28, result);
            return result;
        }
        case 14:
        {
            uint32 mode = r_u32(command + 24);
            expected = r_u32(0x8009edbc);
            uint32 result = mode == expected ? 1638 : 2340;
            w_u32(0x800a25a0, result);
            return result;
        }
        }
    }
    return previous_first_match ? 0x800a0000 : 0x8009edb0;
}

uint32 G3Function800158E4(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800158e4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 data = command + 4;
    uint32 word = r_u32(command + 4);
    uint32 expected = r_u32(0x8009d358);
    uint32 count = r_u32(data + 8);
    bool previous_first_match = false;
    for (uint32 operation = 0; operation < 7; ++operation)
    {
        if (operation != 0)
        {
            if ((operation & 1) != 0 || !previous_first_match)
                word = r_u32(data);
            expected = r_u32(0x8009d358 + operation * 12);
        }
        previous_first_match = word == expected;
        if (!previous_first_match)
            continue;
        uint32 second = r_u32(data + 4);
        expected = r_u32(0x8009d35c + operation * 12);
        if (second != expected)
            continue;
        uint32 game = r_u32(0x80097048);
        uint32 camera = r_u32(game + 0x4c);
        switch (operation)
        {
        case 0:
            G3Function8004F044(camera, data, frame);
            break;
        case 1:
            G3Function8004DB64(camera, data, frame);
            break;
        case 2:
            G3Function80057C44(camera, data, frame);
            break;
        case 3:
            G3Function8004DD0C(camera, data, frame);
            break;
        case 4:
            G3Function8002AD3C(data, frame);
            break;
        case 5:
            G3Function8002AF90(data);
            break;
        case 6:
        {
            uint32 branch = G3Function80027960(data, frame);
            if (branch == 0xffffffff)
                return 0;
            if (branch != 0 && branch != 0xfffffffe)
                G3Function80018DE0(object, branch + 4, frame);
            break;
        }
        }
        return count + 1;
    }
    return count + 1;
}

uint32 G3Function8004F584(uint32 guest_stack, G3TextMeasureOperands *operands)
{
    FUNCTION_MARKER(0x8004f584, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 mode = r_u32(0x80096a88);
    if (mode != 5)
        return 0xffffffff;
    uint32 descriptor = r_u32(0x800a22ac);
    uint32 path = r_u32(descriptor);
    if (operands != 0)
        operands->first_glyph = 0x800a22c0;
    return G3Function8003F8A4(path, 0x800a22c0, frame, operands) & 0xffff;
}

uint32 G3Function8004D880(uint32 guest_stack, G3TextMeasureOperands *operands)
{
    FUNCTION_MARKER(0x8004d880, "SLUS_008.06");
    return G3Function8004F584(guest_stack - 0x18, operands);
}

uint32 G3Function8004F5D4(void)
{
    FUNCTION_MARKER(0x8004f5d4, "SLUS_008.06");
    w_u32(0x800a22f0, 1);
    return 1;
}

uint32 G3Function8004DAFC(void)
{
    FUNCTION_MARKER(0x8004dafc, "SLUS_008.06");
    return G3Function8004F5D4();
}

uint32 G3Function800697F0(uint32 context)
{
    FUNCTION_MARKER(0x800697f0, "SLUS_008.06");
    return context == 0 ? 0 : r_u32(context);
}

uint32 G3Function8006A5C0(uint32 context)
{
    FUNCTION_MARKER(0x8006a5c0, "SLUS_008.06");
    return G3Function800697F0(context);
}

uint32 G3Function8006B83C(uint32 context)
{
    FUNCTION_MARKER(0x8006b83c, "SLUS_008.06");
    uint32 game = G3Function8006A5C0(context);
    w_u32(game + 0x6818, 5);
    w_u32(game + 0x69e0, 0);
    w_u16(game + 0x681c, 4);
    w_u16(game + 0x681e, 4);
    w_u32(game + 0x6824, 0);
    w_u32(game + 0x6828, 0);
    w_u32(game + 0x682c, 0);
    w_u32(game + 0x6830, 0);
    w_u32(game + 0x6838, 0);
    G3FillMemory80086094(game + 0x69c8, 0, 16);
    G3FillMemory80086094(game + 0x683c, 0, 56);
    G3FillMemory80086094(game + 0x6874, 0, 56);
    return G3FillMemory80086094(game + 0x67f8, 0, 28);
}

void G3Function80069848(uint32 context, uint32 callback, uint32 data, uint32 argument, uint32 guest_stack, uint32 *return_value)
{
    FUNCTION_MARKER(0x80069848, "SLUS_008.06");
    sint32 index = r_s32(context + 4);
    if (context == 0 || callback == 0)
        return;
    if (index < 0)
    {
        if (return_value != 0)
            *return_value = 1;
        return;
    }
    uint32 offset = (uint32)index << 4;
    if (index >= 8)
    {
        if (return_value != 0)
            *return_value = offset;
        return;
    }
    uint32 entry = context + offset + 8;
    w_u32(entry, callback);
    w_u32(entry + 4, data);
    w_u32(entry + 8, argument);
    uint32 extra = r_u32(guest_stack + 0x10);
    w_u32(entry + 0xc, extra);
    if (return_value != 0)
        *return_value = entry;
}

void G3Function80069894(uint32 context, uint32 callback, uint32 data, uint32 argument, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80069894, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    if (context != 0 && callback != 0)
    {
        w_u32(context + 4, 0);
        w_u32(frame + 0x10, 0);
        G3Function80069848(context, callback, data, argument, frame, 0);
    }
}

uint32 G3Function80069984(uint32 context, uint32 value)
{
    FUNCTION_MARKER(0x80069984, "SLUS_008.06");
    uint32 selected = 0x800;
    if ((sint32)value >= 0x800)
        selected = (sint32)value <= 0x1000 ? value : 0x1000;
    uint32 previous = r_u32(context + 0xb4);
    w_u32(context + 0xb4, selected);
    return previous;
}

uint32 G3Function8006A538(uint32 context, uint32 guest_stack, G3TextMeasureOperands *operands)
{
    FUNCTION_MARKER(0x8006a538, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function8006B83C(context);
    uint32 argument = r_u32(0x800a0060);
    G3Function80069894(context, 0x8006a350, 0, argument, frame);
    uint32 result = G3Function80069984(context, 0xaf0);
    if (operands != 0)
    {
        operands->first_glyph = 0xaf0;
        operands->second_glyph = 0;
        operands->third_glyph = argument;
    }
    return result;
}

uint32 G3Function80069A1C(uint32 context, uint32 incoming_v0)
{
    FUNCTION_MARKER(0x80069a1c, "SLUS_008.06");
    if (context == 0)
        return incoming_v0;
    uint32 destination = context + 0x1c;
    for (uint32 index = 0; index < 8; ++index)
    {
        w_u32(destination + 0x88, 0);
        destination -= 4;
    }
    w_u32(context + 0xa8, 0);
    w_u32(context + 0xac, 0);
    w_u32(context + 0xb0, 0);
    return destination;
}

uint32 G3Function800275E0(uint32 guest_stack)
{
    FUNCTION_MARKER(0x800275e0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 game = r_u32(0x80097048);
    w_u8(game + 0x68f9, 1);
    uint32 index = r_u32(0x8009d7a0) + 1;
    w_u32(0x8009d7a0, index);
    if (index == 3)
        w_u32(0x8009d7a0, 0);
    index = r_u32(0x8009d7a0);
    uint32 state = r_u32(0x8009d698 + (index << 2));
    return G3Function80027584(state, frame);
}

uint32 G3Function80075B54(uint32 effect, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80075b54, "SLUS_008.06");
    uint32 frame = guest_stack - 0x88;
    uint32 selector = (uint32)r_s16(effect + 0x22);
    uint32 direction = 0x80098838 + (selector << 3);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 value = r_u16(direction + axis * 2);
        w_u16(frame + 0x10 + axis * 2, value);
    }
    uint32 radius = r_u16(effect + 0x20);
    w_u16(frame + 0x1a, 0);
    w_u16(frame + 0x1c, 0);
    w_u16(frame + 0x18, radius);
    G3Function8002FF58(frame + 0x30, frame + 0x10, frame);
    TransposeMatrix((MATRIX *)psx_addr(frame + 0x30, 18), (MATRIX *)psx_addr(frame + 0x50, 18));
    for (uint32 phase = 0; phase < 2; ++phase)
    {
        uint32 matrix = frame + 0x30 + phase * 0x20;
        uint32 first = r_u32(matrix);
        uint32 second = r_u32(matrix + 4);
        xport_gte_write_control(0, first);
        xport_gte_write_control(1, second);
        uint32 third = r_u32(matrix + 8);
        uint32 fourth = r_u32(matrix + 0xc);
        uint32 fifth = r_u32(matrix + 0x10);
        xport_gte_write_control(2, third);
        xport_gte_write_control(3, fourth);
        xport_gte_write_control(4, fifth);
        uint32 vector = r_u32(frame + 0x18);
        xport_gte_write_data(0, vector);
        vector = r_u32(frame + 0x1c);
        xport_gte_write_data(1, vector);
        xport_gte_execute(0x486012);
        vector = xport_gte_read_data(25);
        w_u32(frame + 0x20, vector);
        vector = xport_gte_read_data(26);
        w_u32(frame + 0x24, vector);
        vector = xport_gte_read_data(27);
        w_u32(frame + 0x28, vector);
        if (phase == 0)
        {
            first = r_u16(frame + 0x20);
            second = r_u16(frame + 0x24);
            third = r_u16(frame + 0x28);
            w_u16(frame + 0x18, first);
            w_u16(frame + 0x1a, second);
            w_u16(frame + 0x1c, third);
            uint32 angle = (uint32)r_s16(effect + 0x24);
            G3Function8002F8E4(frame + 0x18, angle);
        }
    }
    uint32 linked = r_u32(effect + 4);
    uint32 position = r_u16(linked + 0x58);
    uint32 rotated = r_u16(frame + 0x20);
    uint32 offset = r_u16(effect + 0x28);
    linked = r_u32(effect + 4);
    w_u16(effect + 8, offset + position + rotated);
    position = r_u16(linked + 0x5a);
    rotated = r_u16(frame + 0x24);
    offset = r_u16(effect + 0x2a);
    w_u16(effect + 0xa, offset + position + rotated);
    position = r_u16(linked + 0x5c);
    rotated = r_u16(frame + 0x28);
    offset = r_u16(effect + 0x2c);
    w_u16(effect + 0xc, offset + position + rotated);
    uint32 angle = r_u16(effect + 0x24);
    uint32 change = r_u16(effect + 0x26);
    uint32 result = angle + change;
    w_u16(effect + 0x24, result);
    return result;
}

uint32 G3Function80075F28(uint32 effect, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80075f28, "SLUS_008.06");
    uint32 frame = guest_stack - 0x78;
    sint32 scale = r_s16(effect + 0x26);
    sint32 component = r_s16(effect + 0x28);
    uint32 product = (uint32)(scale * component);
    if ((sint32)product < 0)
        product += 0xfff;
    uint32 origin = r_u16(effect + 0x20);
    w_u16(effect + 0x20, origin + (uint32)((sint32)product >> 12));
    scale = r_s16(effect + 0x26);
    component = r_s16(effect + 0x2a);
    uint32 y_product = (uint32)(scale * component);
    if ((sint32)y_product < 0)
        y_product += 0xfff;
    component = r_s16(effect + 0x2c);
    uint32 z_product = (uint32)(scale * component);
    origin = r_u16(effect + 0x22);
    w_u16(effect + 0x22, origin + (uint32)((sint32)y_product >> 12));
    if ((sint32)z_product < 0)
        z_product += 0xfff;
    origin = r_u16(effect + 0x24);
    uint32 radius = r_u16(effect + 0x2e);
    w_u16(effect + 0x24, origin + (uint32)((sint32)z_product >> 12));
    w_u16(frame + 0x10, radius);
    w_u16(frame + 0x12, 0);
    w_u16(frame + 0x14, 0);
    uint32 angle = (uint32)r_s16(effect + 0x30);
    G3Function8002F8E4(frame + 0x10, angle);
    G3Function8002FF58(frame + 0x28, effect + 0x28, frame);
    TransposeMatrix((MATRIX *)psx_addr(frame + 0x28, 18), (MATRIX *)psx_addr(frame + 0x48, 18));
    uint32 first = r_u32(frame + 0x48);
    uint32 second = r_u32(frame + 0x4c);
    xport_gte_write_control(0, first);
    xport_gte_write_control(1, second);
    uint32 third = r_u32(frame + 0x50);
    uint32 fourth = r_u32(frame + 0x54);
    uint32 fifth = r_u32(frame + 0x58);
    xport_gte_write_control(2, third);
    xport_gte_write_control(3, fourth);
    xport_gte_write_control(4, fifth);
    uint32 vector = r_u32(frame + 0x10);
    xport_gte_write_data(0, vector);
    vector = r_u32(frame + 0x14);
    xport_gte_write_data(1, vector);
    xport_gte_execute(0x486012);
    vector = xport_gte_read_data(25);
    w_u32(frame + 0x18, vector);
    vector = xport_gte_read_data(26);
    w_u32(frame + 0x1c, vector);
    vector = xport_gte_read_data(27);
    w_u32(frame + 0x20, vector);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        origin = r_u16(effect + 0x20 + axis * 2);
        uint32 rotated = r_u16(frame + 0x18 + axis * 4);
        w_u16(effect + 8 + axis * 2, origin + rotated);
    }
    angle = r_u16(effect + 0x30);
    uint32 change = r_u16(effect + 0x32);
    uint32 result = angle + change;
    w_u16(effect + 0x30, result);
    return result;
}

uint32 G3Function800760C8(uint32 effect, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800760c8, "SLUS_008.06");
    uint32 linked = r_u32(effect + 4);
    uint32 value = r_u16(linked + 0x58);
    w_u16(effect + 8, value);
    value = r_u16(linked + 0x5a);
    w_u16(effect + 0xa, value);
    value = r_u16(linked + 0x5c);
    w_u16(effect + 0xc, value);
    return value;
}

uint32 G3Function800635FC(uint32 context, uint32 level, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800635fc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 object = r_u32(context + 0x14);
    uint32 flags = G3Function80063594(object);
    uint32 callback = r_u32(context + 8);
    if (callback != 0)
        return G3Function80063664(context, level, flags, 0, frame);
    return 0;
}

void G3Function8005EB00(uint32 object, uint32 level, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005eb00, "SLUS_008.06");
    uint32 frame = guest_stack - 0x68;
    w_u32(frame + 0x6c, level);
    w_u32(frame + 0x24, 0x1f800148);
    if (r_u32(object + 0x30) == 0)
        return;
    if (r_u32(object + 0x34) == 0)
        return;
    uint32 callback = r_u32(object + 0xec);
    w_u32(frame + 0x38, callback);
    if (callback == 0)
        return;
    uint32 flags = G3Function80063594(object);
    uint32 matrix = r_u32(object + 0x30);
    w_u32(frame + 0x28, matrix);
    uint32 resource = r_u32(object + 0x1c);
    uint32 selector = (uint32)r_s16(object + 0xa2);
    uint32 models = r_u32(resource + 0xc);
    uint32 model = r_u32(models + (selector << 2));
    w_u32(frame + 0x34, model);
    uint32 previous_matrix = r_u32(object + 0x34);
    uint32 segment = r_u32(model + 0x18);
    uint32 count = (uint32)r_s16(model + 6);
    w_u32(frame + 0x3c, flags);
    uint32 end = segment + count * 24;
    w_u32(frame + 0x2c, end);
    if (segment >= end)
        return;
    do
    {
        uint32 collision = r_u32(segment + 0x14);
        if (collision != 0)
        {
            count = r_u32(collision + 8);
            if (count != 0)
            {
                uint32 primitive = r_u32(collision + 0xc);
                uint32 primitive_end = primitive + (count << 4);
                w_u32(frame + 0x30, primitive_end);
                if (primitive < primitive_end)
                {
                    uint32 metadata = primitive + 4;
                    do
                    {
                        uint32 type = r_u8(metadata + 1);
                        if (type == 9 || type - 3 < 3)
                        {
                            uint32 output = 0;
                            for (uint32 phase = 0; phase < 2; ++phase)
                            {
                                matrix = phase == 0 ? r_u32(frame + 0x28) : previous_matrix;
                                uint32 first = r_u32(matrix);
                                uint32 second = r_u32(matrix + 4);
                                xport_gte_write_control(0, first);
                                xport_gte_write_control(1, second);
                                uint32 third = r_u32(matrix + 8);
                                uint32 fourth = r_u32(matrix + 0xc);
                                uint32 fifth = r_u32(matrix + 0x10);
                                xport_gte_write_control(2, third);
                                xport_gte_write_control(3, fourth);
                                xport_gte_write_control(4, fifth);
                                first = r_u32(matrix + 0x14);
                                second = r_u32(matrix + 0x18);
                                xport_gte_write_control(5, first);
                                third = r_u32(matrix + 0x1c);
                                xport_gte_write_control(6, second);
                                xport_gte_write_control(7, third);
                                uint32 vector = r_u32(primitive + 8);
                                xport_gte_write_data(0, vector);
                                vector = r_u32(primitive + 0xc);
                                xport_gte_write_data(1, vector);
                                xport_gte_execute(0x480012);
                                uint32 destination = 0x1f800158;
                                if (phase == 0)
                                {
                                    output = r_u32(frame + 0x24);
                                    destination = output;
                                }
                                vector = xport_gte_read_data(25);
                                w_u32(destination, vector);
                                vector = xport_gte_read_data(26);
                                w_u32(destination + 4, vector);
                                vector = xport_gte_read_data(27);
                                w_u32(destination + 8, vector);
                                uint32 gte_flags = xport_gte_read_control(31);
                                w_u32(frame + 0x20, gte_flags);
                            }
                            uint32 x = r_u16(output);
                            uint32 y = r_u16(output + 4);
                            uint32 z = r_u16(output + 8);
                            w_u16(0x1f8001b8, x);
                            w_u16(0x1f8001ba, y);
                            w_u16(0x1f8001bc, z);
                            x = r_u16(0x1f800158);
                            uint32 radius = r_u16(metadata + 0xa);
                            y = r_u16(0x1f80015c);
                            z = r_u16(0x1f800160);
                            w_u16(0x1f800168, x);
                            w_u16(0x1f80016a, y);
                            w_u16(0x1f80016c, z);
                            w_u32(0x1f800170, 0x1f8001b8);
                            w_u32(0x1f800174, 0x1f800168);
                            w_u32(0x1f800184, object);
                            w_u16(0x1f8001be, radius);
                            model = r_u32(frame + 0x34);
                            uint32 base = r_u32(model + 0x18);
                            uint32 index = segment - base;
                            index = (index << 2) + index;
                            index += index << 4;
                            index += index << 8;
                            index += index << 16;
                            index = (uint32)((sint32)(0u - index) >> 3);
                            w_u32(0x1f800180, index);
                            uint32 id = r_u8(metadata);
                            w_u32(0x1f800188, primitive);
                            w_u32(0x1f80017c, id);
                            uint32 object_flags = r_u32(object + 0x10);
                            bool correct_position = false;
                            if ((object_flags & 0x20000) != 0)
                            {
                                uint32 game = r_u32(0x80097048);
                                uint32 mode = r_u32(game + 0x69b8);
                                correct_position = (mode & 0x10) == 0;
                            }
                            if (!correct_position)
                            {
                                callback = r_u32(frame + 0x38);
                                w_u32(0x1f800178, callback);
                                level = r_u32(frame + 0x6c);
                                G3Function800635FC(0x1f800170, level, frame);
                            }
                            else
                            {
                                level = r_u32(frame + 0x6c);
                                flags = r_u32(frame + 0x3c);
                                w_u32(frame + 0x14, 0);
                                w_u32(frame + 0x10, flags);
                                G3Function80060F3C(frame + 0x18, 0x1f800170, object, level, frame);
                                if (r_s16(frame + 0x18) != 0 || r_s16(frame + 0x1a) != 0 || r_s16(frame + 0x1c) != 0)
                                {
                                    x = r_u16(object + 0x58);
                                    uint32 delta = r_u16(frame + 0x18);
                                    w_u16(object + 0x58, x + delta);
                                    y = r_u16(object + 0x5a);
                                    delta = r_u16(frame + 0x1a);
                                    w_u16(object + 0x5a, y + delta);
                                    z = r_u16(object + 0x5c);
                                    delta = r_u16(frame + 0x1c);
                                    uint32 game = r_u32(0x80097048);
                                    (void)game;
                                    w_u16(object + 0x5c, z + delta);
                                    G3Function8005F3A8(object, frame + 0x18);
                                    callback = r_u32(object + 0xec);
                                    if (callback != 0)
                                    {
                                        game = r_u32(0x80097048);
                                        G3DispatchStateEvent(callback, object, game, 17, frame + 0x18, frame);
                                    }
                                }
                            }
                        }
                        metadata += 0x10;
                        primitive_end = r_u32(frame + 0x30);
                        primitive += 0x10;
                    } while (primitive < primitive_end);
                }
            }
        }
        matrix = r_u32(frame + 0x28);
        segment += 0x18;
        w_u32(frame + 0x28, matrix + 0x20);
        end = r_u32(frame + 0x2c);
        previous_matrix += 0x20;
    } while (segment < end);
}

uint32 G3Function800634FC(uint32 position, uint32 radius, uint32 bounds)
{
    FUNCTION_MARKER(0x800634fc, "SLUS_008.06");
    sint32 extent = (sint32)(radius << 16) >> 16;
    sint32 coordinate = r_s16(position + 4);
    sint32 limit = r_s16(bounds + 0xa);
    if (coordinate - extent >= limit)
        return 0;
    limit = r_s16(bounds + 4);
    if (limit >= coordinate + extent)
        return 0;
    coordinate = r_s16(position + 2);
    limit = r_s16(bounds + 8);
    if (coordinate - extent >= limit)
        return 0;
    limit = r_s16(bounds + 2);
    if (limit >= coordinate + extent)
        return 0;
    coordinate = r_s16(position);
    limit = r_s16(bounds + 6);
    if (coordinate - extent >= limit)
        return 0;
    limit = r_s16(bounds);
    return limit < coordinate + extent ? 1 : 0;
}

uint32 G3Function80063664(uint32 context, uint32 level, uint32 flags, uint32 argument3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80063664, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    w_u32(frame + 0x60, context);
    context = r_u32(frame + 0x60);
    w_u32(frame + 0x64, level);
    w_u32(frame + 0x68, flags);
    w_u32(frame + 0x6c, argument3);
    uint32 callback = r_u32(context + 8);
    w_u32(frame + 0x24, callback);
    context = r_u32(frame + 0x60);
    uint32 object = r_u32(context + 0x14);
    uint32 primitive = r_u32(context + 0x18);
    w_u32(frame + 0x28, primitive);
    context = r_u32(frame + 0x60);
    uint32 current = r_u32(context);
    uint32 previous = r_u32(context + 4);
    w_u32(frame + 0x30, previous);
    G3FillMemory80086094(frame + 0x18, 0, 8);
    previous = r_u32(frame + 0x30);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        sint32 first = r_s16(current + axis * 2);
        sint32 second = r_s16(previous + axis * 2);
        w_u16(0x1f80014c + axis * 2, (uint32)((first + second) >> 1));
    }
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 first = r_u16(current + axis * 2);
        uint32 second = r_u16(previous + axis * 2);
        w_u16(0x1f800154 + axis * 2, first - second);
    }
    uint32 distance = G3Function8002FB10(0x1f800154);
    level = r_u32(frame + 0x64);
    uint32 radius = r_u16(current + 6);
    uint32 value = r_u32(level + 0x40);
    w_u32(0x1f800144, value);
    value = r_u32(level + 0x30);
    w_u32(frame + 0x2c, 0);
    w_u32(0x1f800148, value);
    value = r_u16(current);
    uint32 event_count = 0;
    w_u16(0x1f800100, value);
    uint32 expanded = (uint32)((sint32)((distance << 3) + distance) >> 3);
    value = r_u16(current + 2);
    w_u16(0x1f800102, value);
    value = r_u16(current + 4);
    expanded += radius;
    w_u16(0x1f800104, value);
    w_u32(0x1f8002d0, 0x1f8002d0);
    uint32 root = r_u32(level);
    uint32 pending = 0x1f8002d4;
    w_u32(pending, root);
    if (root == pending)
        return r_u32(frame + 0x2c);
    w_u32(frame + 0x34, (uint32)((sint32)(expanded << 16) >> 16));
    do
    {
        uint32 node = r_u32(pending);
        uint32 node_flags = r_u8(node + 0xc);
        pending -= 4;
        if ((node_flags & 2) != 0)
        {
            uint32 index = 0;
            primitive = r_u32(node + 0x10);
            uint32 count = r_u8(node + 0xd);
            w_u8(node + 0xc, node_flags | 0x40);
            if (count != 0)
            {
                uint32 metadata = primitive + 6;
                do
                {
                    uint32 type = r_u16(metadata);
                    flags = r_u32(frame + 0x68);
                    if (((1u << (type & 31)) & flags) == 0)
                    {
                        previous = r_u32(frame + 0x30);
                        w_u32(0x1f800140, primitive);
                        argument3 = r_u32(frame + 0x6c);
                        w_u32(frame + 0x10, frame + 0x20);
                        w_u32(frame + 0x14, argument3);
                        uint32 result = G3Function80062C4C(current, previous, 0x1f800138, 0x1f800128, frame);
                        if ((sint32)result > 0)
                        {
                            type = r_u16(metadata);
                            uint32 category = type & 0xe;
                            bool contact_event = true;
                            if ((type & 1) != 0 && (category == 4 || category == 10))
                            {
                                uint32 player = r_u32(0x8009efc4);
                                bool allow_sequence = true;
                                if (object == player)
                                {
                                    context = r_u32(frame + 0x60);
                                    allow_sequence = r_u32(context + 0xc) == 0;
                                }
                                if (allow_sequence && ((category == 4 && result == 2) || category == 10))
                                {
                                    sint32 sequence_index = r_s16(metadata + 4);
                                    if (sequence_index >= 0)
                                    {
                                        level = r_u32(frame + 0x64);
                                        uint32 sequences = r_u32(level + 0x3c);
                                        uint32 first_seen = r_u32(frame + 0x18);
                                        uint32 sequence = r_u32(sequences + ((uint32)sequence_index << 2));
                                        if (first_seen != sequence)
                                        {
                                            uint32 second_seen = r_u32(frame + 0x1c);
                                            sint32 slot = (sint32)(event_count << 16) >> 16;
                                            if (second_seen != sequence && slot < 2 && object != 0)
                                            {
                                                ++event_count;
                                                w_u32(frame + 0x18 + ((uint32)slot << 2), sequence);
                                                sequence_index = r_s16(metadata + 4);
                                                sequences = r_u32(level + 0x3c);
                                                sequence = r_u32(sequences + ((uint32)sequence_index << 2));
                                                context = r_u32(frame + 0x60);
                                                G3Function80018DE0(object, sequence + 4, frame);
                                            }
                                        }
                                    }
                                }
                                for (uint32 axis = 0; axis < 3; ++axis)
                                {
                                    value = r_u16(0x1f800100 + axis * 2);
                                    w_u16(current + axis * 2, value);
                                }
                                contact_event = false;
                            }
                            else if ((type & 1) != 0 && category == 12)
                            {
                                level = r_u32(frame + 0x64);
                                uint32 sequence_index = (uint32)r_s16(metadata + 4);
                                uint32 sequences = r_u32(level + 0x3c);
                                uint32 sequence = r_u32(sequences + (sequence_index << 2));
                                w_u32(object + 0xf4, sequence + 4);
                                contact_event = false;
                            }
                            if (contact_event)
                            {
                                w_u32(0x1f800108, 0);
                                uint32 kind = result == 2 ? 0x10 : (result == 3 ? 0x20 : 8);
                                w_u32(0x1f800108, kind);
                                w_u16(0x1f80010c, 1);
                                w_u16(0x1f80010e, 3);
                                uint32 original_primitive = r_u32(frame + 0x28);
                                uint32 original_x = r_u16(0x1f800100);
                                w_u32(0x1f800114, primitive);
                                w_u32(0x1f800118, object);
                                w_u32(0x1f80011c, 0);
                                w_u32(0x1f800110, original_primitive);
                                uint32 coordinate = r_u16(current);
                                w_u16(0x1f800130, coordinate - original_x);
                                coordinate = r_u16(current + 2);
                                uint32 original_coordinate = r_u16(0x1f800102);
                                w_u16(0x1f800132, coordinate - original_coordinate);
                                coordinate = r_u16(current + 4);
                                original_coordinate = r_u16(0x1f800104);
                                uint32 detail = r_u16(frame + 0x20);
                                w_u16(0x1f800120, detail);
                                w_u16(0x1f800134, coordinate - original_coordinate);
                                callback = r_u32(frame + 0x24);
                                if (callback != 0)
                                {
                                    if (object != 0)
                                        w_u32(object + 0xb0, 0x1f800108);
                                    uint32 game = r_u32(0x80097048);
                                    callback = r_u32(frame + 0x24);
                                    G3DispatchStateEvent(callback, object, game, 3, 0x1f800108, frame);
                                }
                                w_u32(frame + 0x2c, 1);
                            }
                        }
                        else if (result == 0xffffffff)
                        {
                            metadata += 0xc;
                            for (;;)
                            {
                                bool more = (sint32)index < (sint32)count;
                                primitive += 0xc;
                                if (!more || (r_s16(metadata) & 0x8000) == 0)
                                    break;
                                ++index;
                                metadata += 0xc;
                            }
                            metadata -= 0xc;
                            primitive -= 0xc;
                        }
                        argument3 = r_u32(frame + 0x6c);
                        if ((argument3 & 8) != 0)
                        {
                            for (uint32 axis = 0; axis < 3; ++axis)
                            {
                                value = r_u16(0x1f800100 + axis * 2);
                                w_u16(current + axis * 2, value);
                            }
                        }
                    }
                    ++index;
                    metadata += 0xc;
                    primitive += 0xc;
                } while ((sint32)index < (sint32)count);
            }
        }
        else
        {
            uint32 first = r_u32(node + 0x10);
            uint32 search_radius = r_u32(frame + 0x34);
            uint32 second = r_u32(node + 0x14);
            if (G3Function800634FC(0x1f80014c, search_radius, first) != 0)
            {
                pending += 4;
                w_u32(pending, first);
            }
            search_radius = r_u32(frame + 0x34);
            if (G3Function800634FC(0x1f80014c, search_radius, second) != 0)
            {
                pending += 4;
                w_u32(pending, second);
            }
        }
        value = r_u32(pending);
    } while (value != pending);
    return r_u32(frame + 0x2c);
}

uint32 G3Function80062BEC(uint32 vector, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80062bec, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 x = r_u32(vector);
    uint32 xx = x * x;
    uint32 y = r_u32(vector + 4);
    uint32 yy = y * y;
    uint32 z = r_u32(vector + 8);
    uint32 zz = z * z;
    uint32 root = G3Function8002F448((xx + yy + zz) << 4, 4, frame);
    return (uint32)((sint32)(root + 8) >> 4);
}

uint32 G3Function8006340C(uint32 current, uint32 previous, uint32 geometry, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006340c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    w_u32(frame + 0x10, frame + 0x18);
    w_u32(frame + 0x14, 0);
    return G3Function80062C4C(current, previous, geometry, output, frame);
}

uint32 G3Function80063434(uint32 output, uint32 normal, uint32 distance, uint32 matrix)
{
    FUNCTION_MARKER(0x80063434, "SLUS_008.06");
    uint32 first = r_u32(matrix);
    uint32 second = r_u32(matrix + 4);
    xport_gte_write_control(0, first);
    xport_gte_write_control(1, second);
    uint32 third = r_u32(matrix + 8);
    uint32 fourth = r_u32(matrix + 0xc);
    uint32 fifth = r_u32(matrix + 0x10);
    xport_gte_write_control(2, third);
    xport_gte_write_control(3, fourth);
    xport_gte_write_control(4, fifth);
    uint32 vector = r_u32(normal);
    xport_gte_write_data(0, vector);
    vector = r_u32(normal + 4);
    xport_gte_write_data(1, vector);
    xport_gte_execute(0x486012);
    vector = xport_gte_read_data(25);
    w_u32(0x1f800000, vector);
    vector = xport_gte_read_data(26);
    w_u32(0x1f800004, vector);
    vector = xport_gte_read_data(27);
    w_u32(0x1f800008, vector);
    uint32 transformed = r_u32(0x1f800000);
    uint32 difference = distance - transformed;
    w_u32(0x1f800000, difference);
    uint32 result = 0;
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 component = (uint32)r_s16(normal + axis * 2);
        if (axis != 0)
            difference = r_u32(0x1f800000);
        uint32 product = component * difference;
        uint32 origin = r_u16(matrix + axis * 2);
        result = origin + (uint32)((sint32)product >> 12);
        w_u16(output + axis * 2, result);
    }
    return result;
}

uint32 G3Function80062C4C(uint32 current, uint32 previous, uint32 geometry, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80062c4c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    w_u32(frame + 0x6c, output);
    uint32 face = r_u32(geometry + 8);
    uint32 vertices = r_u32(geometry + 0x10);
    uint32 detail_output = r_u32(frame + 0x70);
    uint32 first_index = (uint32)r_s16(face);
    w_u32(detail_output, 0xffffffff);
    uint32 coordinate = r_u16(current);
    uint32 first = vertices + (first_index << 3);
    uint32 origin = r_u16(first);
    uint32 flags = r_u32(frame + 0x74);
    w_u16(0x1f800060, coordinate - origin);
    for (uint32 axis = 1; axis < 3; ++axis)
    {
        coordinate = r_u16(current + axis * 2);
        origin = r_u16(first + axis * 2);
        w_u16(0x1f800060 + axis * 2, coordinate - origin);
    }
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        coordinate = r_u16(previous + axis * 2);
        origin = r_u16(first + axis * 2);
        w_u16(0x1f800066 + axis * 2, coordinate - origin);
    }
    origin = r_u16(first);
    w_u16(0x1f80006c, origin);
    origin = r_u16(first + 2);
    w_u32(frame + 0x30, 0);
    w_u16(0x1f80006e, origin);
    origin = r_u16(first + 4);
    w_u32(frame + 0x34, 0);
    w_u16(0x1f800070, origin);
    uint32 normal_index;
    uint32 normals;
    if ((flags & 1) != 0)
    {
        normal_index = r_u16(face + 8);
        normals = r_u32(geometry + 0xc);
        normal_index = (uint32)((sint32)((0u - normal_index) << 16) >> 16);
    }
    else
    {
        normal_index = (uint32)r_s16(face + 8);
        normals = r_u32(geometry + 0xc);
    }
    G3Function8005F304(normal_index, normals, 0x1f800054, frame);
    uint32 matrix_first = r_u32(0x1f800060);
    uint32 matrix_second = r_u32(0x1f800064);
    xport_gte_write_control(0, matrix_first);
    xport_gte_write_control(1, matrix_second);
    uint32 matrix_third = r_u32(0x1f800068);
    uint32 matrix_fourth = r_u32(0x1f80006c);
    uint32 matrix_fifth = r_u32(0x1f800070);
    xport_gte_write_control(2, matrix_third);
    xport_gte_write_control(3, matrix_fourth);
    xport_gte_write_control(4, matrix_fifth);
    uint32 vector = r_u32(0x1f800054);
    xport_gte_write_data(0, vector);
    vector = r_u32(0x1f800058);
    xport_gte_write_data(1, vector);
    xport_gte_execute(0x486012);
    vector = xport_gte_read_data(25);
    w_u32(0x1f800080, vector);
    vector = xport_gte_read_data(26);
    w_u32(0x1f800084, vector);
    vector = xport_gte_read_data(27);
    w_u32(0x1f800088, vector);
    if (r_u32(0x8009efa0) != 0)
    {
        sint32 start_distance = r_s32(0x1f800084);
        if (start_distance <= 0 && r_s32(0x1f800080) > 0)
            return 0xffffffff;
    }
    else
    {
        sint32 end_distance = r_s32(0x1f800080);
        sint32 start_distance = r_s32(0x1f800084);
        if (end_distance >= start_distance)
            return 0xffffffff;
    }
    uint32 radius = r_u16(current + 6);
    sint32 end_distance = r_s32(0x1f800080);
    if (end_distance >= (sint32)radius)
        return 0xffffffff;
    if (end_distance >= 0)
    {
        uint32 plane = r_u32(0x1f800088);
        G3Function80063434(0x1f8000a8, 0x1f800054, plane, current);
    }
    else
    {
        if (r_s32(0x1f800084) <= 0)
            return 0;
        uint32 game = r_u32(0x80097048);
        uint32 plane = r_u32(0x1f800088);
        w_u32(frame + 0x10, plane);
        uint32 target = r_u32(game + 0x65e8);
        if (G3DispatchLinePlane(target, 0x1f8000a8, previous, current, 0x1f800054, frame) == 0)
            return 0;
    }
    uint32 second_index = (uint32)r_s16(face + 2);
    vertices = r_u32(geometry + 0x10);
    uint32 third_index = (uint32)r_s16(face + 4);
    w_u32(frame + 0x10, 0x1f800054);
    if (G3Function8005F164(first, vertices + (second_index << 3), vertices + (third_index << 3), 0x1f8000a8, frame) != 0)
    {
        uint32 material = r_u16(face + 6);
        uint32 category = material & 0xe;
        if ((material & 1) != 0 && (category == 4 || category == 12))
        {
            if (r_s32(0x1f800080) > 0)
                return 0;
            if (r_s32(0x1f800084) < 0)
                return 0;
        }
        else
        {
            coordinate = r_u16(0x1f8000a8);
            output = r_u32(frame + 0x6c);
            w_u16(output, coordinate);
            for (uint32 axis = 1; axis < 3; ++axis)
            {
                coordinate = r_u16(0x1f8000a8 + axis * 2);
                w_u16(output + axis * 2, coordinate);
            }
            for (uint32 axis = 0; axis < 3; ++axis)
            {
                uint32 normal = (uint32)r_s16(0x1f800054 + axis * 2);
                radius = r_u16(current + 6);
                uint32 product = normal * radius;
                origin = r_u16(0x1f8000a8 + axis * 2);
                w_u16(current + axis * 2, origin + (uint32)((sint32)product >> 12));
            }
        }
        return 2;
    }
    uint32 nearest_squared = 0xffffffff;
    for (uint32 edge = 0; edge < 3; ++edge)
    {
        uint32 candidate = frame + 0x18 + edge * 8;
        uint32 start_index = (uint32)r_s16(face + edge * 2);
        vertices = r_u32(geometry + 0x10);
        uint32 start = vertices + (start_index << 3);
        uint32 end_index = (uint32)r_s16(face + ((edge + 1) % 3) * 2);
        uint32 game = r_u32(0x80097048);
        uint32 target = r_u32(game + 0x65ec);
        uint32 parameter = G3DispatchNearestPoint(target, candidate, start, vertices + (end_index << 3), current, frame);
        sint32 point_component = r_s16(current);
        sint32 candidate_component = r_s16(candidate);
        uint32 delta = (uint32)(point_component - candidate_component);
        uint32 xx = delta * delta;
        candidate_component = r_s16(candidate + 2);
        point_component = r_s16(current + 2);
        delta = (uint32)(point_component - candidate_component);
        uint32 yy = delta * delta;
        candidate_component = r_s16(candidate + 4);
        point_component = r_s16(current + 4);
        delta = (uint32)(point_component - candidate_component);
        uint32 zz = delta * delta;
        uint32 squared = xx + yy + zz;
        if (squared <= nearest_squared)
        {
            nearest_squared = squared;
            w_u32(frame + 0x30, edge);
            w_u32(frame + 0x34, parameter);
        }
    }
    radius = r_u16(current + 6);
    if (nearest_squared >= radius * radius)
        return 0;
    uint32 chosen = r_u32(frame + 0x30);
    detail_output = r_u32(frame + 0x70);
    w_u32(detail_output, chosen);
    uint32 material = r_u16(face + 6);
    uint32 category = material & 0xe;
    if ((material & 1) == 0 || (category != 4 && category != 12))
    {
        chosen = r_u32(frame + 0x30);
        output = r_u32(frame + 0x6c);
        uint32 candidate = frame + 0x18 + (chosen << 3);
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            coordinate = r_u16(candidate + axis * 2);
            w_u16(output + axis * 2, coordinate);
        }
        sint32 current_component = r_s16(current);
        sint32 candidate_component = r_s16(candidate);
        sint32 dx = current_component - candidate_component;
        w_u32(0x1f800090, (uint32)dx);
        candidate_component = r_s16(candidate + 2);
        current_component = r_s16(current + 2);
        sint32 dy = current_component - candidate_component;
        w_u32(0x1f800094, (uint32)dy);
        candidate_component = r_s16(candidate + 4);
        current_component = r_s16(current + 4);
        sint32 dz = current_component - candidate_component;
        w_u32(0x1f800098, (uint32)dz);
        radius = r_u16(current + 6);
        uint32 denominator;
        uint32 expansion = 0;
        if (radius >= 0xc9)
        {
            denominator = G3Function80062BEC(0x1f800090, frame) << 5;
            expansion = 2;
        }
        else
        {
            uint32 small = (uint32)(dx < 0 ? -dx : dx);
            uint32 middle = (uint32)(dy < 0 ? -dy : dy);
            uint32 large = (uint32)(dz < 0 ? -dz : dz);
            uint32 temporary;
            if (small > middle)
            {
                temporary = small;
                small = middle;
                middle = temporary;
            }
            if (middle > large)
            {
                temporary = middle;
                middle = large;
                large = temporary;
            }
            if (small > middle)
            {
                temporary = small;
                small = middle;
                middle = temporary;
            }
            denominator = ((large << 4) - large) * 2 + middle * 12 + small * 9;
        }
        if (denominator != 0)
        {
            uint32 quotients[3];
            for (uint32 axis = 0; axis < 3; ++axis)
            {
                radius = r_u16(current + 6);
                uint32 difference = r_u32(0x1f800090 + axis * 4);
                uint32 product = difference * (radius + expansion);
                w_u32(0x1f800090 + axis * 4, product);
                uint32 numerator = product << 5;
                quotients[axis] = numerator == 0x80000000 && denominator == 0xffffffff ? 0x80000000 : (uint32)((sint32)numerator / (sint32)denominator);
            }
            w_u32(0x1f800090, quotients[0]);
            w_u32(0x1f800094, quotients[1]);
            w_u32(0x1f800098, quotients[2]);
        }
        else
        {
            uint32 normal = (uint32)r_s16(0x1f800054);
            radius = r_u16(current + 6);
            uint32 product = normal * radius;
            uint32 next_normal = r_u16(0x1f800056);
            w_u32(0x1f800090, (uint32)((sint32)product >> 12));
            radius = r_u16(current + 6);
            normal = (uint32)((sint32)(next_normal << 16) >> 16);
            product = normal * radius;
            w_u32(0x1f800094, (uint32)((sint32)product >> 12));
            normal = (uint32)r_s16(0x1f800058);
            radius = r_u16(current + 6);
            product = normal * radius;
            w_u32(0x1f800098, (uint32)((sint32)product >> 12));
        }
        chosen = r_u32(frame + 0x30);
        candidate = frame + 0x18 + (chosen << 3);
        for (uint32 axis = 0; axis < 3; ++axis)
        {
            uint32 correction = r_u16(0x1f800090 + axis * 4);
            origin = r_u16(candidate + axis * 2);
            w_u16(current + axis * 2, correction + origin);
        }
    }
    uint32 parameter = r_u32(frame + 0x34);
    return parameter - 1 < 0xfff ? 1 : 3;
}

uint32 G3Function80060044(void)
{
    FUNCTION_MARKER(0x80060044, "SLUS_008.06");
    w_u32(0x800a27f8, 0);
    return G3FillMemory80086094(0x800a25c8, 0, 0x230);
}

uint32 G3Function80060074(uint32 object, uint32 game, uint32 message, uint32 contact, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80060074, "SLUS_008.06");
    uint32 player = r_u32(0x8009efc4);
    uint32 frame = guest_stack - 0x28;
    uint32 geometry = r_u32(contact + 0xc);
    if (object == player)
    {
        uint32 callback = r_u32(object + 0xec);
        w_u32(frame + 0x10, 0);
        if (callback != 0)
            G3DispatchStateEvent(callback, object, game, 14, frame + 0x10, frame);
        if (r_u32(frame + 0x10) == 1)
            return 1;
    }
    uint32 count = r_u32(0x800a27f8);
    if ((sint32)count < 20)
    {
        uint32 record = 0x800a25c8 + count * 28;
        uint32 value = (uint32)r_s16(contact + 0x28);
        w_u32(record, value);
        value = (uint32)r_s16(contact + 0x2a);
        w_u32(record + 4, value);
        value = (uint32)r_s16(contact + 0x2c);
        w_u32(record + 8, value);
        value = (uint32)r_s16(contact + 0x18);
        w_u32(record + 0x18, geometry);
        w_u32(record + 0x14, value);
        if (geometry != 0)
        {
            value = (uint32)r_s16(geometry + 8);
            w_u32(record + 0x10, value);
        }
        uint32 flags = r_u32(contact);
        if ((flags & 8) != 0)
            w_u32(record + 0xc, 2);
        else if ((flags & 0x10) != 0)
            w_u32(record + 0xc, 0);
        else if ((flags & 0x20) != 0)
            w_u32(record + 0xc, 3);
        count = r_u32(0x800a27f8);
        w_u32(0x800a27f8, count + 1);
    }
    return 1;
}

void G3Function80060EA8(uint32 output, uint32 position, uint32 radius, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80060ea8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 count = r_u32(0x800a27f8);
    if ((sint32)count <= 0)
    {
        w_u32(output, 0);
        w_u32(output + 4, 0);
        w_u32(output + 8, 0);
        return;
    }
    uint32 planar = 0;
    uint32 record = 0x800a25c8;
    uint32 index = 0;
    do
    {
        if (r_u32(record + 0xc) == 0)
            ++planar;
        ++index;
        record += 0x1c;
    } while ((sint32)index < (sint32)count);
    if ((sint32)planar > 0)
        G3Function8006083C(output, position, radius, index, frame);
    else
        G3Function80060C8C(output, position, radius, index, frame);
}

uint32 G3Function80060F3C(uint32 output, uint32 context, uint32 argument2, uint32 argument3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80060f3c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    uint32 position = r_u32(context);
    uint32 flags = r_u32(frame + 0x5c);
    uint32 mask = r_u32(frame + 0x58);
    uint32 value = (uint32)r_s16(position);
    w_u32(frame + 0x20, value);
    position = r_u32(context);
    value = (uint32)r_s16(position + 2);
    w_u32(frame + 0x24, value);
    position = r_u32(context);
    value = (uint32)r_s16(position + 4);
    w_u32(frame + 0x28, value);
    w_u32(context + 8, 0x80060074);
    G3Function80060044();
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    uint32 root = r_u32(level);
    G3Function80063664(context, root, mask, flags | 8, frame);
    position = r_u32(context);
    uint32 radius = r_u16(position + 6);
    G3Function80060EA8(frame + 0x10, frame + 0x20, radius, frame);
    uint32 x = r_u32(frame + 0x10);
    bool tiny = x + 2 < 5;
    if (tiny)
    {
        sint32 y = r_s32(frame + 0x14);
        tiny = y >= -2 && y < 3;
    }
    if (tiny)
    {
        sint32 z = r_s32(frame + 0x18);
        tiny = z >= -2 && z < 3;
    }
    if (tiny)
    {
        w_u32(frame + 0x10, 0);
        w_u32(frame + 0x14, 0);
        w_u32(frame + 0x18, 0);
    }
    value = r_u16(frame + 0x10);
    w_u16(output, value);
    value = r_u16(frame + 0x14);
    w_u16(output + 2, value);
    value = r_u16(frame + 0x18);
    w_u16(output + 4, value);
    return value;
}

uint32 G3Function80060B88(uint32 first, uint32 second, uint32 angle, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80060b88, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 x = r_u32(first);
    uint32 other_x = r_u32(second);
    uint32 xx = x * other_x;
    uint32 y = r_u32(first + 4);
    uint32 other_y = r_u32(second + 4);
    uint32 yy = y * other_y;
    uint32 z = r_u32(first + 8);
    uint32 other_z = r_u32(second + 8);
    uint32 dot = xx + yy + z * other_z;
    uint32 first_length = G3Function8002FAB0(first, frame);
    uint32 second_length = G3Function8002FAB0(second, frame);
    if (first_length == 0 || second_length == 0)
        return 0;
    uint32 numerator = dot;
    if ((sint32)dot <= 0xffff)
        numerator <<= 12;
    uint32 normalized = (uint32)((sint64)(sint32)numerator / (sint32)first_length);
    if ((sint32)dot > 0xffff)
        normalized <<= 12;
    normalized = (uint32)((sint64)(sint32)normalized / (sint32)second_length);
    uint32 scaled_angle = angle << 12;
    sint64 product = (sint64)(sint32)scaled_angle * (sint32)0xb60b60b7;
    uint32 high = (uint32)(product >> 32);
    uint32 adjusted = high + scaled_angle;
    uint32 native_angle = (uint32)((sint32)adjusted >> 8) - (uint32)((sint32)scaled_angle >> 31);
    uint32 threshold = G3Function8005B470(native_angle);
    return (sint32)normalized >= (sint32)threshold ? 1 : 0;
}

uint32 G3Function80060C8C(uint32 output, uint32 position, uint32 radius, uint32 argument3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80060c8c, "SLUS_008.06");
    uint32 count = r_u32(0x800a27f8);
    uint32 frame = guest_stack - 0x38;
    uint32 first_index = 0;
    if ((sint32)(count - 1) > 0)
    {
        uint32 first = 0x800a25c8;
        do
        {
            uint32 second_index = first_index + 1;
            if ((sint32)second_index < (sint32)count)
            {
                uint32 second = 0x800a25c8 + second_index * 28;
                do
                {
                    uint32 first_kind = r_u32(first + 0xc);
                    if (first_kind - 2 < 2)
                    {
                        uint32 second_kind = r_u32(second + 0xc);
                        if (second_kind - 2 < 2 && G3Function80060B88(first, second, 30, frame) != 0)
                        {
                            uint32 first_length = G3Function8002FBF8(first, frame);
                            uint32 second_length = G3Function8002FBF8(second, frame);
                            if ((sint32)first_length < (sint32)second_length)
                                w_u32(first + 0xc, 1);
                            else
                                w_u32(second + 0xc, 1);
                        }
                    }
                    uint32 current_count = r_u32(0x800a27f8);
                    ++second_index;
                    second += 0x1c;
                    if ((sint32)second_index >= (sint32)current_count)
                        break;
                } while (true);
            }
            count = r_u32(0x800a27f8);
            ++first_index;
            first += 0x1c;
        } while ((sint32)first_index < (sint32)(count - 1));
        first_index = 0;
    }
    uint32 result = r_u32(0x800a27f8);
    uint32 accepted = first_index;
    w_u32(output, 0);
    w_u32(output + 4, 0);
    w_u32(output + 8, 0);
    if ((sint32)result > 0)
    {
        count = result;
        uint32 record = 0x800a25c8;
        do
        {
            uint32 kind = r_u32(record + 0xc);
            if (kind - 2 < 2)
            {
                uint32 sum = r_u32(output);
                uint32 value = r_u32(record);
                w_u32(output, sum + value);
                sum = r_u32(output + 4);
                value = r_u32(record + 4);
                w_u32(output + 4, sum + value);
                sum = r_u32(output + 8);
                value = r_u32(record + 8);
                ++accepted;
                w_u32(output + 8, sum + value);
            }
            ++first_index;
            result = (sint32)first_index < (sint32)count ? 1 : 0;
            record += 0x1c;
        } while (result != 0);
    }
    if ((sint32)accepted > 0)
    {
        uint32 x = r_u32(output);
        x = (uint32)((sint64)(sint32)x / (sint32)accepted);
        uint32 y = r_u32(output + 4);
        y = (uint32)((sint64)(sint32)y / (sint32)accepted);
        uint32 z = r_u32(output + 8);
        result = (uint32)((sint64)(sint32)z / (sint32)accepted);
        w_u32(output, x);
        w_u32(output + 4, y);
        w_u32(output + 8, result);
    }
    else
    {
        w_u32(output, 0);
        w_u32(output + 4, 0);
        w_u32(output + 8, 0);
    }
    return result;
}

uint32 G3Function8006083C(uint32 output, uint32 position, uint32 radius, uint32 argument3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006083c, "SLUS_008.06");
    uint32 count = r_u32(0x800a27f8);
    uint32 frame = guest_stack - 0x68;
    uint32 group = 0;
    w_u32(frame + 0x68, output);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x18, 0);
    w_u32(frame + 0x20, 0);
    w_u32(frame + 0x24, 0);
    w_u32(frame + 0x28, 0);
    w_u32(0x8009ef9c, 0);
    uint32 accepted = 0;
    if ((sint32)count > 0)
    {
        uint32 index = 0;
        uint32 record = 0x800a25c8;
        do
        {
            uint32 kind = r_u32(record + 0xc);
            if (kind == 2)
            {
                uint32 edge = r_u32(record + 0x14);
                uint32 match = G3Function80060188(index, edge, 0x800a25c8, count, frame);
                if ((sint32)match >= 0)
                {
                    uint32 first_face = r_u32(record + 0x18);
                    uint32 second_face = r_u32(0x800a25c8 + match * 28 + 0x18);
                    if (G3Function800602E0(first_face, second_face, frame) != 0)
                    {
                        uint32 face = r_u32(record + 0x18);
                        w_u32(record + 0xc, 0);
                        G3Function80060468(position, radius, face, record, frame);
                    }
                }
            }
            ++index;
            count = r_u32(0x800a27f8);
            record += 0x1c;
        } while ((sint32)index < (sint32)count);
    }
    count = r_u32(0x800a27f8);
    if ((sint32)(count - 1) > 0)
    {
        uint32 index = 0;
        uint32 record = 0x800a25c8;
        do
        {
            uint32 kind = r_u32(record + 0xc);
            if (kind == 0)
            {
                uint32 other_index = index + 1;
                if ((sint32)other_index < (sint32)count)
                {
                    uint32 inner_count = r_u32(0x800a27f8);
                    uint32 other = record + 0x1c;
                    do
                    {
                        uint32 other_kind = r_u32(other + 0xc);
                        if (other_kind == 0)
                        {
                            uint32 first_group = r_u32(record + 0x10);
                            uint32 other_group = r_u32(other + 0x10);
                            if (first_group == other_group)
                                w_u32(other + 0xc, 1);
                        }
                        ++other_index;
                        other += 0x1c;
                    } while ((sint32)other_index < (sint32)inner_count);
                }
            }
            ++index;
            count = r_u32(0x800a27f8);
            record += 0x1c;
        } while ((sint32)index < (sint32)(count - 1));
    }
    count = r_u32(0x800a27f8);
    if ((sint32)count > 0)
    {
        uint32 index = 0;
        uint32 record = 0x800a25c8;
        do
        {
            uint32 kind = r_u32(record + 0xc);
            if (kind == 0)
            {
                uint32 next_group = r_u32(record + 0x10);
                if (group != next_group)
                {
                    if (group != 0)
                        w_u32(0x8009ef9c, 1);
                    group = r_u32(record + 0x10);
                }
                uint32 sum = r_u32(frame + 0x20);
                uint32 coordinate = r_u32(record);
                w_u32(frame + 0x20, sum + coordinate);
                sum = r_u32(frame + 0x24);
                coordinate = r_u32(record + 4);
                w_u32(frame + 0x24, sum + coordinate);
                sum = r_u32(frame + 0x28);
                coordinate = r_u32(record + 8);
                ++accepted;
                w_u32(frame + 0x28, sum + coordinate);
            }
            ++index;
            record += 0x1c;
        } while ((sint32)index < (sint32)count);
    }
    uint32 averages[3];
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 sum = r_u32(frame + 0x20 + axis * 4);
        if (accepted == 0)
            averages[axis] = (sint32)sum < 0 ? 1 : 0xffffffff;
        else
            averages[axis] = (uint32)((sint64)(sint32)sum / (sint32)accepted);
    }
    w_u32(frame + 0x10, averages[0]);
    w_u32(frame + 0x14, averages[1]);
    w_u32(frame + 0x18, averages[2]);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 origin = r_u32(position + axis * 4);
        w_u32(frame + 0x30 + axis * 4, origin + averages[axis]);
    }
    if ((sint32)accepted >= 2)
    {
        count = r_u32(0x800a27f8);
        G3Function800607C8(frame + 0x30, 0x800a25c8, count, radius, frame);
    }
    uint32 corrected = r_u32(frame + 0x30);
    uint32 original = r_u32(position);
    output = r_u32(frame + 0x68);
    w_u32(output, corrected - original);
    corrected = r_u32(frame + 0x34);
    original = r_u32(position + 4);
    w_u32(output + 4, corrected - original);
    corrected = r_u32(frame + 0x38);
    original = r_u32(position + 8);
    uint32 result = corrected - original;
    w_u32(output + 8, result);
    return result;
}

uint32 G3Function80060188(uint32 index, uint32 edge, uint32 records, uint32 count, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80060188, "SLUS_008.06");
    uint32 advanced = edge + 1;
    sint64 product = (sint64)(sint32)advanced * 0x55555556;
    uint32 high = (uint32)(product >> 32);
    uint32 quotient = high - (uint32)((sint32)advanced >> 31);
    uint32 face = r_u32(records + index * 28 + 0x18);
    uint32 first = r_u16(face + (edge << 1));
    uint32 next = advanced - quotient * 3;
    uint32 second = r_u16(face + (next << 1));
    if ((sint32)count <= 0)
        return 0xffffffff;
    uint32 candidate = 0;
    uint32 record = records;
    do
    {
        if (candidate != index)
        {
            uint32 other = r_u32(record + 0x18);
            uint32 middle = r_u16(other + 2);
            uint32 start = r_u16(other);
            uint32 end = r_u16(other + 4);
            if ((first == start && (second == middle || second == end)) || (first == middle && (second == start || second == end)) || (first == end && (second == start || second == middle)))
                return candidate;
        }
        ++candidate;
        record += 0x1c;
    } while ((sint32)candidate < (sint32)count);
    return 0xffffffff;
}

uint32 G3Function80060280(uint32 first, uint32 second)
{
    FUNCTION_MARKER(0x80060280, "SLUS_008.06");
    for (uint32 index = 0; index < 3; ++index)
    {
        uint32 vertex = r_u16(first + index * 2);
        bool found = false;
        for (uint32 other = 0; other < 3; ++other)
        {
            uint32 candidate = r_u16(second + other * 2);
            if (vertex == candidate)
            {
                found = true;
                break;
            }
        }
        if (!found)
            return index;
    }
    return 0xffffffff;
}

uint32 G3Function800602E0(uint32 first, uint32 second, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800602e0, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x38;
    uint32 level = r_u32(game + 0x48);
    uint32 root = r_u32(level);
    uint32 vertices = r_u32(root + 0x30);
    uint32 index = G3Function80060280(first, second);
    if ((sint32)index < 0)
        return 0;
    uint32 selected = first + (index << 1);
    uint32 other = first + (((index + 1) % 3) << 1);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 first_index = r_u16(selected);
        uint32 second_index = r_u16(other);
        sint32 coordinate = r_s16(vertices + (first_index << 3) + axis * 2);
        sint32 other_coordinate = r_s16(vertices + (second_index << 3) + axis * 2);
        if (axis == 2)
            game = r_u32(0x80097048);
        w_u32(frame + 0x10 + axis * 4, (uint32)(coordinate - other_coordinate));
    }
    level = r_u32(game + 0x48);
    root = r_u32(level);
    uint32 normal_index = (uint32)r_s16(second + 8);
    uint32 normals = r_u32(root + 0x40);
    G3Function8005F304(normal_index, normals, frame + 0x20, frame);
    uint32 dot = 0;
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 normal = (uint32)r_s16(frame + 0x20 + axis * 2);
        uint32 difference = r_u32(frame + 0x10 + axis * 4);
        dot += normal * difference;
    }
    return (sint32)dot >= 0x3001 ? 1 : 0;
}

uint32 G3Function80060468(uint32 position, uint32 radius, uint32 face, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80060468, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    uint32 root = r_u32(level);
    uint32 index = r_u16(face);
    uint32 normal_index = (uint32)r_s16(face + 8);
    uint32 vertices = r_u32(root + 0x30);
    uint32 normals = r_u32(root + 0x40);
    uint32 vertex = vertices + (index << 3);
    G3Function8005F304(normal_index, normals, frame + 0x30, frame);
    uint32 vx = (uint32)r_s16(vertex);
    uint32 nx = (uint32)r_s16(frame + 0x30);
    uint32 xx = vx * nx;
    uint32 vy = (uint32)r_s16(vertex + 2);
    uint32 ny = (uint32)r_s16(frame + 0x32);
    uint32 yy = vy * ny;
    uint32 nz = (uint32)r_s16(frame + 0x34);
    uint32 vz = (uint32)r_s16(vertex + 4);
    uint32 zz = vz * nz;
    uint32 coordinate = r_u32(position);
    uint32 dx = coordinate - vx;
    uint32 relative_x = dx * nx;
    w_u32(frame + 0x20, dx);
    vy = (uint32)r_s16(vertex + 2);
    coordinate = r_u32(position + 4);
    uint32 dy = coordinate - vy;
    uint32 relative_y = dy * ny;
    w_u32(frame + 0x24, dy);
    vz = (uint32)r_s16(vertex + 4);
    coordinate = r_u32(position + 8);
    uint32 dz = coordinate - vz;
    uint32 relative_z = dz * nz;
    w_u32(frame + 0x28, dz);
    uint32 plane = (uint32)((sint32)xx >> 12) + (uint32)((sint32)yy >> 12) + (uint32)((sint32)zz >> 12);
    uint32 relative = (uint32)((sint32)relative_x >> 12) + (uint32)((sint32)relative_y >> 12) + (uint32)((sint32)relative_z >> 12);
    if ((sint32)relative >= (sint32)radius)
    {
        w_u32(output, 0);
        w_u32(output + 4, 0);
        w_u32(output + 8, 0);
        return 0;
    }
    coordinate = r_u16(position);
    w_u16(frame + 0x40, coordinate);
    coordinate = r_u16(position + 4);
    w_u16(frame + 0x42, coordinate);
    coordinate = r_u16(position + 8);
    w_u16(frame + 0x44, coordinate);
    G3Function80063434(frame + 0x38, frame + 0x30, plane, frame + 0x40);
    nx = (uint32)r_s16(frame + 0x30);
    xx = nx * radius;
    ny = (uint32)r_s16(frame + 0x32);
    yy = ny * radius;
    nz = (uint32)r_s16(frame + 0x34);
    zz = nz * radius;
    uint32 projected_x = (uint32)r_s16(frame + 0x38);
    uint32 corrected_x = (uint32)((sint32)(xx << 4) >> 16) + projected_x;
    uint32 projected_y = (uint32)r_s16(frame + 0x3a);
    uint32 corrected_y = (uint32)((sint32)(yy << 4) >> 16) + projected_y;
    uint32 projected_z = (uint32)r_s16(frame + 0x3c);
    w_u32(frame + 0x10, corrected_x);
    w_u32(frame + 0x14, corrected_y);
    uint32 corrected_z = (uint32)((sint32)(zz << 4) >> 16) + projected_z;
    w_u32(frame + 0x18, corrected_z);
    coordinate = r_u32(position);
    w_u32(output, corrected_x - coordinate);
    corrected_y = r_u32(frame + 0x14);
    coordinate = r_u32(position + 4);
    w_u32(output + 4, corrected_y - coordinate);
    corrected_z = r_u32(frame + 0x18);
    coordinate = r_u32(position + 8);
    w_u32(output + 8, corrected_z - coordinate);
    return 1;
}

uint32 G3Function80060684(uint32 position, uint32 records, uint32 count, uint32 radius, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80060684, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    uint32 accepted = 0;
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x18, 0);
    if ((sint32)count > 0)
    {
        uint32 index = 0;
        uint32 record = records;
        do
        {
            uint32 kind = r_u32(record + 0xc);
            if (kind == 0)
            {
                uint32 face = r_u32(record + 0x18);
                G3Function80060468(position, radius, face, frame + 0x20, frame);
                ++accepted;
                uint32 sum_x = r_u32(frame + 0x10);
                uint32 dx = r_u32(frame + 0x20);
                uint32 dy = r_u32(frame + 0x24);
                uint32 dz = r_u32(frame + 0x28);
                w_u32(frame + 0x10, sum_x + dx);
                uint32 sum_y = r_u32(frame + 0x14);
                uint32 sum_z = r_u32(frame + 0x18);
                w_u32(frame + 0x14, sum_y + dy);
                w_u32(frame + 0x18, sum_z + dz);
            }
            ++index;
            record += 0x1c;
        } while ((sint32)index < (sint32)count);
    }
    uint32 averages[3];
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 sum = r_u32(frame + 0x10 + axis * 4);
        if (accepted == 0)
            averages[axis] = (sint32)sum < 0 ? 1 : 0xffffffff;
        else
            averages[axis] = (uint32)((sint64)(sint32)sum / (sint32)accepted);
    }
    w_u32(frame + 0x10, averages[0]);
    w_u32(frame + 0x14, averages[1]);
    w_u32(frame + 0x18, averages[2]);
    uint32 coordinate = r_u32(position);
    w_u32(position, coordinate + averages[0]);
    coordinate = r_u32(position + 4);
    uint32 correction = r_u32(frame + 0x14);
    w_u32(position + 4, coordinate + correction);
    coordinate = r_u32(position + 8);
    correction = r_u32(frame + 0x18);
    uint32 result = coordinate + correction;
    w_u32(position + 8, result);
    return result;
}
