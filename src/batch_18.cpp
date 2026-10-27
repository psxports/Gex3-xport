#include "xport.h"
#include "psx.h"
#include "game_functions.h"

uint32 G3Function8004EA20(void)
{
    FUNCTION_MARKER(0x8004EA20, "SLUS_008.06");
    G3FillMemory80086094(0x800a22b4, 0, 68);
    return G3FillMemory80086094(0x800a22a4, 0, 16);
}

uint32 G3Function8004EF14(uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004EF14, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 result = G3FillMemory80086094(0x800a22b4, 0, 68);
    uint32 record = r_u32(0x800a22ac);
    uint32 sample = 0;
    if (record != 0)
    {
        uint32 channel = r_u32(record);
        sample = G3Function8003FBB0(channel, 0x800a22c0, frame);
        record = r_u32(0x800a22ac);
        channel = r_u32(record + 4);
        result = G3Function8003FB7C(channel, 0x800a22c4, frame);
    }
    if (sample != 0)
    {
        result = r_u16(sample);
        uint32 second_record = r_u32(0x800a22b0);
        w_u16(0x800a22e0, result);
        result = r_u16(sample + 2);
        w_u16(0x800a22e2, result);
        result = r_u16(sample + 4);
        w_u16(0x800a22e4, result);
        if (second_record != 0)
        {
            uint32 channel = r_u32(second_record);
            result = G3Function8003FBB0(channel, 0x800a22d0, frame);
        }
    }
    return result;
}

uint32 G3Function8004EFCC(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004EFCC, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function80016F24();
    uint32 sequence = r_u32(0x800a22a4);
    if (sequence != 0)
    {
        w_u32(0x800a22a4, 0);
        uint32 player = r_u32(game + 0x50);
        G3Function80018DE0(player, sequence + 4, frame);
    }
    uint32 result = r_u32(0x800a22b4);
    if (result != 0)
    {
        result = r_u32(0x80097048);
        uint32 flags = r_u32(result + 0x6834);
        w_u32(result + 0x6834, flags & 0xffffff7f);
    }
    return result;
}

uint32 G3Function8004F488(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004F488, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 record = r_u32(0x800a22ac);
    uint32 sample = 0;
    if (record != 0)
    {
        uint32 channel = r_u32(record);
        sample = G3Function8003FBB0(channel, 0x800a22c0, frame);
    }
    if (sample != 0)
    {
        for (uint32 index = 0; index < 3; ++index)
        {
            uint32 coordinate = r_u16(sample + index * 2);
            w_u16(state + index * 2, coordinate);
        }
    }
    record = r_u32(0x800a22b0);
    sample = 0;
    if (record != 0)
    {
        uint32 channel = r_u32(record);
        sample = G3Function8003FBB0(channel, 0x800a22d0, frame);
    }
    if (sample != 0)
    {
        for (uint32 index = 0; index < 3; ++index)
        {
            uint32 coordinate = r_u16(sample + index * 2);
            w_u16(state + 6 + index * 2, coordinate);
        }
    }
    else
    {
        uint32 player = r_u32(0x8009efc4);
        uint32 position = player + 0x58;
        uint32 high_address = position + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low_lane = position & 3;
        uint32 low = r_masked_u32(position & 0xfffffffc, (15u << low_lane) & 15);
        uint32 word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        uint32 z = (uint32)r_s16(player + 0x5c);
        high_address = state + 9;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        uint32 low_address = state + 6;
        low_lane = low_address & 3;
        w_masked_u32(low_address & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
        w_u16(state + 0x0a, z);
    }
    uint32 result = G3Function8005A1D8(state + 0x0c, state + 6, state, frame);
    w_u32(state + 0x14, result);
    return result;
}

uint32 G3Function8004EA60(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004EA60, "SLUS_008.06");
    uint32 frame = guest_stack - 0xa8;
    uint32 record = r_u32(0x800a22ac);
    uint32 second_record = r_u32(0x800a22b0);
    bool finished = record == 0;
    if (!finished)
        finished = r_u32(record) == 0;
    if (!finished)
    {
        uint32 sequence = r_u32(0x800a22a8);
        if (sequence != 0)
        {
            w_u32(0x800a22a8, 0);
            uint32 player = r_u32(game + 0x50);
            G3Function80018DE0(player, sequence + 4, frame);
        }
        if (r_u32(0x800a22b4) != 0)
        {
            uint32 buttons = r_u32(game + 0x98);
            if ((buttons & 0x4000) != 0 || r_u32(0x800a22f0) != 0)
            {
                w_u32(0x800a22f0, 0);
                finished = true;
            }
            else
                G3Function8004F3D4(game, frame);
        }
        if (!finished)
        {
            uint32 path = r_u32(record);
            uint32 sample = G3Function8003FC34(path, 0x800a22c0, frame);
            if (sample == 0)
            {
                if (r_s16(0x800a22e8) != 0)
                {
                    uint32 restart = r_u16(0x800a22ea);
                    w_u16(0x800a22c0, restart);
                    uint32 cursor = r_u32(0x800a22c0);
                    w_u32(0x800a22c0, cursor & 0x1ffff);
                    path = r_u32(record);
                    sample = G3Function8003FC34(path, 0x800a22c0, frame);
                }
                finished = sample == 0;
            }
            if (!finished)
            {
                if (r_u32(0x800a22b8) == 0)
                {
                    for (uint32 index = 0; index < 3; ++index)
                    {
                        uint32 coordinate = r_u16(sample + index * 2);
                        w_u16(frame + 0x10 + index * 2, coordinate);
                    }
                }
                else
                {
                    uint32 x = r_u16(0x800a22e0);
                    uint32 y = r_u16(0x800a22e2);
                    uint32 z = r_u16(0x800a22e4);
                    w_u16(frame + 0x10, x);
                    w_u16(frame + 0x12, y);
                    w_u16(frame + 0x14, z);
                }
                if (r_u32(0x800a22b8) != 0)
                {
                    for (uint32 index = 0; index < 3; ++index)
                    {
                        uint32 coordinate = r_u16(sample + index * 2);
                        w_u16(0x800a22e0 + index * 2, coordinate);
                    }
                }
                else if (second_record != 0)
                {
                    path = r_u32(second_record);
                    uint32 second_sample = G3Function8003FC34(path, 0x800a22d0, frame);
                    if (second_sample == 0)
                    {
                        if (r_s16(0x800a22e8) != 0)
                        {
                            uint32 restart = r_u16(0x800a22ea);
                            w_u16(0x800a22d0, restart);
                            uint32 cursor = r_u32(0x800a22d0);
                            w_u32(0x800a22d0, cursor & 0x1ffff);
                            path = r_u32(second_record);
                            second_sample = G3Function8003FC34(path, 0x800a22d0, frame);
                        }
                        finished = second_sample == 0;
                    }
                    if (!finished)
                    {
                        for (uint32 index = 0; index < 3; ++index)
                        {
                            uint32 coordinate = r_u16(second_sample + index * 2);
                            w_u16(frame + 0x16 + index * 2, coordinate);
                        }
                    }
                }
                else
                {
                    uint32 player = r_u32(0x8009efc4);
                    uint32 position = player + 0x58;
                    uint32 high_address = position + 3;
                    uint32 high_lane = high_address & 3;
                    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
                    uint32 low_lane = position & 3;
                    uint32 low = r_masked_u32(position & 0xfffffffc, (15u << low_lane) & 15);
                    uint32 word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
                    uint32 z = (uint32)r_s16(player + 0x5c);
                    high_address = frame + 0x19;
                    high_lane = high_address & 3;
                    w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
                    uint32 low_address = frame + 0x16;
                    low_lane = low_address & 3;
                    w_masked_u32(low_address & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
                    w_u16(frame + 0x1a, z);
                }
            }
        }
    }
    if (finished)
    {
        if (r_u32(0x800a22bc) != 0)
        {
            uint32 count = r_u32(0x80096b24);
            if (count != 0)
            {
                uint32 mode = r_u32(0x80096a88);
                uint32 current_target = r_u32(0x80099650 + (mode << 3) + 4);
                if (current_target != 0)
                {
                    uint32 previous_mode = r_u32(0x80096b14 + ((count - 1) << 2));
                    uint32 current_game = r_u32(0x80097048);
                    uint32 previous_target = r_u32(0x80099650 + (previous_mode << 3) + 4);
                    G3DispatchStateEvent(previous_target, object, current_game, 23, 0, frame);
                }
            }
        }
        w_u32(0x800a22f4, 1);
        G3Function80013C54(object, frame);
        uint32 mode_argument = r_u32(0x80096a88);
        (void)mode_argument;
        G3Function8004EFCC(game, frame);
        uint32 mode = r_u32(0x80096a88);
        uint32 target = r_u32(0x80099650 + (mode << 3) + 4);
        if (target == 0)
            return 0;
        uint32 current_game = r_u32(0x80097048);
        return G3DispatchStateEvent(target, object, current_game, 1, 0, frame);
    }
    uint32 result = G3Function8005A1D8(frame + 0x1c, frame + 0x16, frame + 0x10, frame);
    w_u32(frame + 0x24, result);
    uint32 path = r_u32(record + 4);
    if (path != 0)
    {
        uint32 available = G3Function80041230(path, 0x800a22c4, frame);
        if (available == 0)
        {
            if (r_s16(0x800a22e8) != 0)
            {
                uint32 restart = r_u16(0x800a22ea);
                w_u16(0x800a22c4, restart);
                uint32 cursor = r_u32(0x800a22c4);
                w_u32(0x800a22c4, cursor & 0x1ffff);
                path = r_u32(record + 4);
                available = G3Function80041230(path, 0x800a22c4, frame);
            }
            if (available == 0)
                return G3Function8005A248(object, frame + 0x10);
        }
        path = r_u32(record + 4);
        G3Function80041008(path, 0x800a22c4, frame + 0x28, frame);
        G3Function8005BD00(frame + 0x28, frame + 0x30);
        uint32 first = r_u16(frame + 0x30);
        uint32 second = r_u16(frame + 0x36);
        uint32 third = r_u16(frame + 0x3c);
        uint32 fourth = r_u16(frame + 0x3e);
        w_u16(frame + 0x50, first);
        first = r_u16(frame + 0x34);
        w_u16(frame + 0x56, second);
        second = r_u16(frame + 0x3a);
        w_u16(frame + 0x5c, third);
        third = r_u16(frame + 0x38);
        w_u16(frame + 0x52, 0u - first);
        first = r_u16(frame + 0x40);
        w_u16(frame + 0x58, 0u - second);
        second = r_u16(frame + 0x32);
        w_u16(frame + 0x60, fourth);
        w_u16(frame + 0x5a, third);
        w_u16(frame + 0x5e, 0u - first);
        w_u16(frame + 0x54, second);
        TransposeMatrix((MATRIX *)psx_addr(frame + 0x50, 18), (MATRIX *)psx_addr(frame + 0x70, 18));
        G3Function800202A4(frame + 0x70, frame + 0x28, 0x15, frame);
        first = r_u16(frame + 0x28);
        second = r_u16(frame + 0x2c);
        w_u16(frame + 0x1c, 0u - first);
        first = r_u16(frame + 0x2a);
        w_u16(frame + 0x20, 0u - second);
        w_u16(frame + 0x1e, 0u - first);
    }
    return G3Function8005A248(object, frame + 0x10);
}

uint32 G3Function8004F3D4(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004F3D4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 result = r_u32(0x800a22b4);
    if (result == 0)
        return result;
    result = r_u32(0x800a22ec);
    if (result == 0)
        return result;
    result = r_u32(game + 0x98) & 0xf0;
    if (result == 0)
        return result;
    uint32 record = r_u32(0x800a22ac);
    if (record != 0)
    {
        uint32 path = r_u32(record);
        G3Function8003FC34(path, 0x800a22c0, frame);
        result = r_u32(0x800a22ac);
        path = r_u32(result + 4);
        if (path != 0)
            result = G3Function8003FC34(path, 0x800a22c4, frame);
    }
    record = r_u32(0x800a22b0);
    if (record != 0)
    {
        uint32 path = r_u32(record);
        result = G3Function8003FC34(path, 0x800a22d0, frame);
    }
    return result;
}

uint32 G3Function8005235C(uint32 object, uint32 game, uint32 event, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005235C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    switch (event)
    {
    case 0:
        G3Function80051890(object, frame);
        return 1;
    case 1:
        G3Function80051A10(object, game, frame);
        return 1;
    case 2:
        G3Function8005210C(0x800523cc);
        return 1;
    case 6:
        G3Function80051864();
        return 1;
    case 22:
        return payload == 5;
    case 24:
        return payload == 19 || payload == 6;
    case 25:
        if (r_u32(0x800a23b0) != 0 && r_u32(0x800a23ac) != 0)
            return (uint32)r_s16(0x800a23c8);
        return (uint32)r_s16(0x800a23e0);
    default:
        return 0;
    }
}

uint32 G3Function80051864(void)
{
    FUNCTION_MARKER(0x80051864, "SLUS_008.06");
    return G3FillMemory80086094(0x800a23a4, 0, 72);
}

uint32 G3Function8005180C(uint32 object, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005180C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (event == 1)
    {
        G3Function80051748(object, frame);
        return 1;
    }
    if (event == 6)
    {
        G3Function8005169C();
        return 1;
    }
    return 0;
}

uint32 G3Function8005169C(void)
{
    FUNCTION_MARKER(0x8005169C, "SLUS_008.06");
    return G3FillMemory80086094(0x800a238c, 0, 24);
}

uint32 G3Function8005210C(uint32 incoming_v0)
{
    FUNCTION_MARKER(0x8005210C, "SLUS_008.06");
    return incoming_v0;
}

uint32 G3Function80051890(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80051890, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    G3FillMemory80086094(0x800a23a4, 0, 72);
    w_u32(0x800a23b8, 32);
    uint32 word = r_masked_u32(0x80096b38, 15);
    word = r_masked_u32(0x80096b38, 15);
    uint32 z = (uint32)r_s16(0x80096b3c);
    w_masked_u32(0x800a23a4, word, 15);
    w_masked_u32(0x800a23a4, word, 15);
    w_u16(0x800a23a8, z);
    z = r_u16(0x800a23a8);
    w_u16(0x800a23a8, z + 50);
    uint32 coordinate = r_u16(object);
    w_u16(frame + 0x28, coordinate);
    coordinate = r_u16(object + 2);
    w_u16(frame + 0x2a, coordinate);
    coordinate = r_u16(object + 4);
    w_u16(frame + 0x2c, coordinate);
    if (r_u32(0x8009eac0) == 0)
    {
        uint32 game = r_u32(0x80097048);
        w_u32(frame + 0x10, 0);
        uint32 level = r_u32(game + 0x48);
        uint32 terrain = r_u32(level);
        w_u32(frame + 0x18, 0x01d528e8);
        w_u32(frame + 0x1c, 0);
        w_u32(frame + 0x14, terrain);
        G3Function80061078(frame + 0x20, 0x800a23a4, frame + 0x28, 408, frame);
        if (r_s16(frame + 0x20) != 0 || r_s16(frame + 0x22) != 0 || r_s16(frame + 0x24) != 0)
        {
            uint32 x = r_u16(0x800a23a4);
            uint32 delta_x = r_u16(frame + 0x20);
            uint32 delta_y = r_u16(frame + 0x22);
            uint32 delta_z = r_u16(frame + 0x24);
            w_u16(0x800a23a4, x + delta_x);
            uint32 y = r_u16(0x800a23a6);
            z = r_u16(0x800a23a8);
            w_u16(0x800a23a6, y + delta_y);
            w_u16(0x800a23a8, z + delta_z);
        }
    }
    uint32 owner = r_u32(0x80096b34);
    w_u32(0x800a23b4, 1);
    uint32 words[2];
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 source = object + 0x18 + index * 4;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
        words[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 destination = 0x800a23c4 + index * 4;
        w_masked_u32(destination, words[index], 15);
        w_masked_u32(destination, words[index], 15);
    }
    uint32 angle = (r_u16(owner + 0x64) + 0x800) & 0xfff;
    w_u16(0x800a23e0, angle);
    return angle;
}

uint32 G3Function80051748(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80051748, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    G3Function800516C8(frame + 0x10, frame);
    return G3Function8005A248(object, frame + 0x10);
}

uint32 G3Function800516C8(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800516C8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function8002F8E4(0x800a239c, 64);
    uint32 offset = r_u16(0x800a239c);
    uint32 coordinate = r_u16(0x80096b38);
    w_u16(state, offset + coordinate);
    offset = r_u16(0x800a239e);
    coordinate = r_u16(0x80096b3a);
    w_u16(state + 2, offset + coordinate);
    offset = r_u16(0x800a23a0);
    coordinate = r_u16(0x80096b3c);
    w_u16(state + 4, offset + coordinate);
    return G3Function800144CC(state + 0x0c, 0x80096b38, state, frame);
}

uint32 G3Function80061078(uint32 output, uint32 position, uint32 target, uint32 radius, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80061078, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    uint32 argument4 = r_u32(guest_stack + 0x10);
    uint32 argument6 = r_u32(guest_stack + 0x18);
    uint32 argument7 = r_u32(guest_stack + 0x1c);
    uint32 high_address = position + 3;
    uint32 high_lane = high_address & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
    uint32 low_lane = position & 3;
    uint32 low = r_masked_u32(position & 0xfffffffc, (15u << low_lane) & 15);
    uint32 word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    uint32 z = (uint32)r_s16(position + 4);
    high_address = frame + 0x1b;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
    uint32 low_address = frame + 0x18;
    low_lane = low_address & 3;
    w_masked_u32(low_address & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
    w_u16(frame + 0x1c, z);
    w_u16(frame + 0x1e, radius);
    uint32 coordinate = r_u16(target);
    w_u16(frame + 0x40, coordinate);
    coordinate = r_u16(target + 2);
    w_u16(frame + 0x42, coordinate);
    coordinate = r_u16(target + 4);
    w_u32(frame + 0x20, frame + 0x18);
    w_u32(frame + 0x24, frame + 0x40);
    w_u32(frame + 0x28, 0x80060074);
    w_u32(frame + 0x2c, 0);
    w_u32(frame + 0x30, 0);
    w_u32(frame + 0x34, argument4);
    w_u32(frame + 0x38, 0);
    w_u16(frame + 0x44, coordinate);
    w_u32(frame + 0x10, argument6);
    w_u32(frame + 0x14, argument7);
    uint32 argument5 = r_u32(guest_stack + 0x14);
    return G3Function80060F3C(output, frame + 0x20, argument4, argument5, frame);
}

uint32 G3Function80051A10(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80051A10, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    uint32 positioned = r_u32(0x800a23b0);
    w_u16(frame + 0x34, 0);
    w_u16(frame + 0x36, 0);
    w_u16(frame + 0x38, 0);
    uint32 ready = 0;
    if (positioned != 0)
        ready = r_u32(0x800a23ac);
    uint32 source = positioned == 0 ? object : 0x800a23a4;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
    uint32 low_lane = source & 3;
    uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
    uint32 word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    uint32 z = (uint32)r_s16(source + 4);
    high_address = frame + 0x2b;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
    uint32 low_address = frame + 0x28;
    low_lane = low_address & 3;
    w_masked_u32(low_address & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
    w_u16(frame + 0x2c, z);
    uint32 completion_flag = 0;
    if (positioned == 0)
    {
        uint32 coordinate = r_u16(0x800a23a4);
        uint32 delta_x = coordinate - r_u16(object);
        sint32 x = (sint16)delta_x;
        uint32 squared_x = (uint32)(x * x);
        w_u16(frame + 0x18, delta_x);
        coordinate = r_u16(0x800a23a6);
        uint32 delta_y = coordinate - r_u16(object + 2);
        sint32 y = (sint16)delta_y;
        uint32 squared_y = (uint32)(y * y);
        w_u16(frame + 0x1a, delta_y);
        coordinate = r_u16(0x800a23a8);
        uint32 delta_z = coordinate - r_u16(object + 4);
        sint32 signed_z = (sint16)delta_z;
        uint32 squared_z = (uint32)(signed_z * signed_z);
        uint32 distance = r_u32(0x8009eaac);
        uint32 squared_limit = distance * distance;
        uint32 squared_length = squared_x + squared_y + squared_z;
        w_u16(frame + 0x1c, delta_z);
        if ((sint32)squared_limit < (sint32)squared_length)
            G3Function8002F9A8(frame + 0x20, (uint32)r_s16(0x8009eaac), frame + 0x18, frame);
        else
        {
            uint32 words[2];
            for (uint32 index = 0; index < 2; ++index)
            {
                source = frame + 0x18 + index * 4;
                high_address = source + 3;
                high_lane = high_address & 3;
                high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
                low_lane = source & 3;
                low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
                words[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
            }
            for (uint32 index = 0; index < 2; ++index)
            {
                uint32 destination = frame + 0x20 + index * 4;
                high_address = destination + 3;
                high_lane = high_address & 3;
                w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
                low_lane = destination & 3;
                w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15u << low_lane) & 15);
            }
            w_u32(0x800a23b0, 1);
        }
        if (r_s16(frame + 0x20) != 0 || r_s16(frame + 0x22) != 0 || r_s16(frame + 0x24) != 0)
        {
            uint32 x_position = r_u16(frame + 0x28);
            delta_x = r_u16(frame + 0x20);
            delta_y = r_u16(frame + 0x22);
            delta_z = r_u16(frame + 0x24);
            w_u16(frame + 0x28, x_position + delta_x);
            uint32 y_position = r_u16(frame + 0x2a);
            z = r_u16(frame + 0x2c);
            w_u16(frame + 0x2a, y_position + delta_y);
            w_u16(frame + 0x2c, z + delta_z);
        }
        if (r_u32(0x800a23b4) == 0)
            completion_flag = 0x800a23b4;
    }
    else if (ready == 0)
        completion_flag = 0x800a23ac;
    else
    {
        uint32 input_pitch = r_u32(game + 0xa4);
        uint32 input_yaw = r_u32(game + 0xa8);
        if (input_pitch != 0 || input_yaw != 0)
        {
            uint32 speed = r_u32(0x800a23bc);
            uint32 limit = r_u32(0x8009eab4);
            if ((sint32)limit >= (sint32)speed)
            {
                uint32 acceleration = r_u32(0x8009eabc);
                w_u32(0x800a23bc, speed + acceleration);
                speed = r_u32(0x800a23bc);
            }
            if ((sint32)speed < 0)
                speed = 0;
            uint32 clamped = (sint32)speed < (sint32)limit ? speed : limit;
            uint32 pitch = r_u16(0x800a23e4);
            uint32 yaw = r_u16(0x800a23e8);
            w_u32(0x800a23bc, clamped);
            w_u16(frame + 0x48, 0);
            w_u16(frame + 0x4a, 0);
            w_u16(frame + 0x4c, 0);
            w_u16(frame + 0x42, 0);
            w_u16(frame + 0x40, pitch);
            w_u16(frame + 0x44, yaw);
            if ((sint32)input_pitch < 0)
                w_u16(frame + 0x4c, limit << 1);
            else if ((sint32)input_pitch > 0)
                w_u16(frame + 0x4c, 0u - (limit << 1));
            if ((sint32)input_yaw < 0)
            {
                uint32 yaw_limit = r_u32(0x8009eab8);
                uint32 direction = r_u16(frame + 0x48);
                w_u16(frame + 0x48, direction - (yaw_limit << 1));
            }
            else if ((sint32)input_yaw > 0)
            {
                uint32 yaw_limit = r_u32(0x8009eab8);
                uint32 direction = r_u16(frame + 0x48);
                w_u16(frame + 0x48, direction + (yaw_limit << 1));
            }
            uint32 length = G3Function8002FA50(frame + 0x48, frame);
            speed = r_u32(0x800a23bc);
            if ((sint32)speed < (sint32)length)
                G3Function8002F9A8(0x800a23cc, (uint32)r_s16(0x800a23bc), frame + 0x48, frame);
            pitch = r_u16(frame + 0x40);
            uint32 delta = r_u16(0x800a23cc);
            uint32 roll = r_u16(frame + 0x42);
            uint32 delta_roll = r_u16(0x800a23ce);
            uint32 player = r_u32(0x8009efc4);
            pitch += delta;
            roll += delta_roll;
            yaw = r_u16(frame + 0x44);
            delta = r_u16(0x800a23d0);
            w_u16(frame + 0x40, pitch);
            w_u16(frame + 0x42, roll);
            w_u16(0x800a23e4, pitch);
            w_u16(0x800a23e6, roll);
            yaw += delta;
            w_u16(frame + 0x44, yaw);
            w_u16(0x800a23e8, yaw);
            if (r_u32(player + 0xf8) != 0 && (r_u32(player + 0x18) & 1) == 0 && (sint32)(pitch << 16) > 0)
                w_u16(0x800a23e4, 0);
        }
        else
        {
            uint32 speed = r_u32(0x800a23bc);
            if (speed != 0)
            {
                speed = (uint32)((sint32)speed >> 1);
                w_u32(0x800a23bc, speed);
                uint32 magnitude = (sint32)speed < 0 ? 0u - speed : speed;
                if ((sint32)magnitude < 2)
                    w_u32(0x800a23bc, 0);
            }
            if (r_u32(0x800a23cc) != 0 || r_s16(0x800a23d0) != 0)
            {
                uint32 x = (uint32)((sint32)(r_u16(0x800a23cc) << 16) >> 17);
                uint32 y = (uint32)((sint32)(r_u16(0x800a23ce) << 16) >> 17);
                z = (uint32)((sint32)(r_u16(0x800a23d0) << 16) >> 17);
                uint32 pitch = r_u16(0x800a23e4);
                w_u16(0x800a23e4, pitch + x);
                uint32 roll = r_u16(0x800a23e6);
                w_u16(0x800a23cc, x);
                uint32 yaw = r_u16(0x800a23e8);
                w_u16(0x800a23ce, y);
                w_u16(0x800a23d0, z);
                w_u16(0x800a23e6, roll + y);
                w_u16(0x800a23e8, yaw + z);
                uint32 squared = x * x + y * y + z * z;
                if ((sint32)squared < 4)
                {
                    w_u16(0x800a23cc, 0);
                    w_u16(0x800a23ce, 0);
                    w_u16(0x800a23d0, 0);
                }
            }
        }
        uint32 pitch = r_u16(0x800a23e4);
        uint32 player = r_u32(0x8009efc4);
        w_u16(0x800a23c4, pitch);
        uint32 yaw = r_u16(0x800a23e8);
        uint32 player_angle = r_u16(player + 0x64);
        w_u16(0x800a23c8, yaw + player_angle + 0x800);
        G3Function8007DEAC(player, 0x800a23e4, 0x800a23e8, game, frame);
    }
    if (completion_flag != 0)
    {
        uint32 target = (uint32)r_s16(0x800a23e0);
        w_u32(frame + 0x10, 0x800a23d8);
        uint32 limit = (uint32)r_s16(0x8009eab0);
        w_u32(frame + 0x14, limit);
        G3Function8005222C(1, 0x800a23c8, target, 0x800a23d0, frame);
        target = (uint32)r_s16(0x800a23dc);
        w_u32(frame + 0x10, 0x800a23d4);
        w_u32(frame + 0x14, 64);
        G3Function8005222C(1, 0x800a23c4, target, 0x800a23cc, frame);
        if (r_u32(0x800a23cc) == 0 && r_s16(0x800a23d0) == 0)
            w_u32(completion_flag, 1);
    }
    uint32 words[2];
    for (uint32 index = 0; index < 2; ++index)
    {
        source = 0x800a23c4 + index * 4;
        words[index] = r_masked_u32(source, 15);
        words[index] = r_masked_u32(source, 15);
    }
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 destination = frame + 0x34 + index * 4;
        high_address = destination + 3;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15u << low_lane) & 15);
    }
    return G3Function8005A248(object, frame + 0x28);
}

uint32 G3Function8005222C(uint32 mode, uint32 angle, uint32 target, uint32 velocity, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005222C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 current = (uint32)r_s16(angle);
    uint32 acceleration = r_u32(guest_stack + 0x10);
    uint32 limit = r_u16(guest_stack + 0x14);
    w_u32(frame + 0x20, current);
    uint32 value = (uint32)r_s16(velocity);
    target &= 0xfff;
    w_u32(frame + 0x24, value);
    value = (uint32)r_s16(acceleration);
    current &= 0xfff;
    w_u32(frame + 0x20, current);
    w_u32(frame + 0x28, value);
    if ((sint32)(target - current) >= 0x800)
        w_u32(frame + 0x20, current + 0x1000);
    else if ((sint32)(current - target) >= 0x801)
        target += 0x1000;
    w_u32(frame + 0x10, frame + 0x28);
    limit = (uint32)(sint16)limit;
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x18, limit);
    G3Function80052114(mode, frame + 0x20, target, frame + 0x24, frame, limit);
    value = r_u16(frame + 0x24);
    w_u16(velocity, value);
    value = r_u16(frame + 0x28);
    w_u16(acceleration, value);
    value = r_u32(frame + 0x20) & 0xfff;
    w_u32(frame + 0x20, value);
    value = r_u16(frame + 0x20);
    w_u16(angle, value);
    return value;
}

uint32 G3Function80052114(uint32 mode, uint32 value, uint32 target, uint32 velocity, uint32 guest_stack, uint32 incoming_v0)
{
    FUNCTION_MARKER(0x80052114, "SLUS_008.06");
    uint32 acceleration = r_u32(guest_stack + 0x10);
    uint32 limit = r_u32(guest_stack + 0x18);
    uint32 delta_limit = r_u32(guest_stack + 0x14);
    if (limit == 0)
    {
        w_u32(value, target);
        return incoming_v0;
    }
    if ((sint32)limit < 0)
        limit = (uint32)(-4096 / (sint32)limit);
    uint32 updated_velocity;
    if (mode == 0)
    {
        uint32 position = r_u32(value);
        uint32 old_velocity = r_u32(velocity);
        uint32 delta = target - position;
        uint32 change = (uint32)((sint32)delta >> 2) - old_velocity;
        w_u32(acceleration, change);
        old_velocity = r_u32(velocity);
        updated_velocity = old_velocity + change;
    }
    else if (mode == 1)
    {
        uint32 position = r_u32(value);
        uint32 delta = target - position;
        if (delta_limit != 0)
        {
            uint32 lower = 0u - delta_limit;
            if ((sint32)lower < (sint32)delta)
                lower = delta;
            delta = (sint32)lower < (sint32)delta_limit ? lower : delta_limit;
        }
        uint32 magnitude = (sint32)delta < 0 ? 0u - delta : delta;
        if ((sint32)limit >= (sint32)magnitude)
        {
            w_u32(value, target);
            w_u32(acceleration, 0);
            w_u32(velocity, 0);
            return 0;
        }
        w_u32(acceleration, delta);
        uint32 old_velocity = r_u32(velocity);
        uint32 lower = 0u - limit;
        uint32 sum = old_velocity + delta;
        w_u32(velocity, sum);
        if ((sint32)lower < (sint32)sum)
            lower = sum;
        updated_velocity = (sint32)lower < (sint32)limit ? lower : limit;
    }
    else
        return 1;
    w_u32(velocity, updated_velocity);
    uint32 position = r_u32(value);
    uint32 result = position + updated_velocity;
    w_u32(value, result);
    return result;
}

uint32 G3Function8007DEAC(uint32 object, uint32 pitch, uint32 yaw, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007DEAC, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 state = r_u32(object + 0x24);
    G3Function8007DE0C(pitch, yaw);
    (void)r_u32(game + 0x4c);
    G3Function8004DA94(frame + 0x10, frame + 0x14);
    sint32 count = r_s16(frame + 0x14);
    if (count > 0)
    {
        uint32 command = r_u32(frame + 0x10);
        for (sint32 index = 0; index < count; ++index)
        {
            w_u16(command, 0);
            w_u16(command + 2, 0);
            w_u16(command + 4, 0);
            command += 8;
        }
    }
    sint32 angle = r_s16(pitch);
    uint32 commands = r_u32(frame + 0x10);
    for (uint32 index = 0; index < 4; ++index)
    {
        if (index != 0)
            angle = r_s16(pitch);
        sint32 scaled = -3 * angle;
        if (scaled < 0)
            scaled += 15;
        w_u16(commands + index * 0x10, (uint32)(scaled >> 4));
        w_u16(commands + index * 0x10 + 6, 10 + index);
        angle = r_s16(yaw);
        scaled = 3 * angle;
        if (scaled < 0)
            scaled += 15;
        w_u16(commands + index * 0x10 + 0x0c, (uint32)(scaled >> 4));
        w_u16(commands + index * 0x10 + 0x0e, 0x100a + index);
    }
    uint32 tuning = r_u32(state);
    uint32 parameter = r_u16(tuning + 0x10);
    w_u16(commands + 0x46, 14);
    w_u16(commands + 0x44, 0);
    w_u16(commands + 0x4e, 0xffff);
    w_u16(commands + 0x40, parameter);
    w_u32(object + 0xa4, commands);
    uint32 value = r_u16(pitch);
    w_u16(state + 0x22c, value);
    value = r_u16(yaw);
    w_u16(state + 0x22e, value);
    return value;
}

uint32 G3Function8007DE0C(uint32 pitch, uint32 yaw)
{
    FUNCTION_MARKER(0x8007DE0C, "SLUS_008.06");
    sint32 angle = r_s16(pitch);
    if (angle < -0x300)
        w_u16(pitch, 0xfffffd00);
    else if (angle >= 0x201)
        w_u16(pitch, 0x200);
    angle = r_s16(yaw);
    if (angle >= 0x801)
    {
        w_u16(yaw, 0x800);
        return 0x800;
    }
    if (angle < -0x800)
        w_u16(yaw, 0xfffff800);
    return 0xfffff800;
}

uint32 G3Function8004DA94(uint32 output, uint32 count)
{
    FUNCTION_MARKER(0x8004DA94, "SLUS_008.06");
    if (output != 0)
        w_u32(output, 0x80096aa0);
    if (count != 0)
        w_u16(count, 10);
    return 10;
}

uint32 G3Function8004E5F8(uint32 object, uint32 game, uint32 event, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004E5F8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    switch (event)
    {
    case 0:
        G3Function8004DF88(object, payload);
        return 1;
    case 1:
        G3Function8004E03C(object, game, frame);
        return 1;
    case 2:
    {
        uint32 previous_mode = r_u32(0x800a2250);
        G3Function8004E1F4(object, frame);
        if (payload == previous_mode)
            return 1;
        uint32 mode = r_u32(0x80096a88);
        uint32 target = r_u32(0x80099654 + (mode << 3));
        if (target == 0)
            return 0;
        return G3DispatchStateEvent(target, object, game, 2, payload, frame);
    }
    case 6:
        G3Function8004DF5C();
        return 1;
    case 11:
        G3Function8004E260(object, payload, frame);
        return 1;
    case 22:
    {
        G3Function8004E1F4(object, frame);
        uint32 mode = r_u32(0x80096a88);
        uint32 target = r_u32(0x80099654 + (mode << 3));
        if (target == 0)
            return 0;
        return G3DispatchStateEvent(target, object, game, 22, payload, frame);
    }
    case 24:
    {
        uint32 mode = r_u32(0x800a2250);
        uint32 target = r_u32(0x80099654 + (mode << 3));
        return G3DispatchStateEvent(target, object, game, 24, payload, frame);
    }
    default:
        return 0;
    }
}

uint32 G3Function8004DF5C(void)
{
    FUNCTION_MARKER(0x8004DF5C, "SLUS_008.06");
    return G3FillMemory80086094(0x800a224c, 0, 68);
}

uint32 G3Function8004DF88(uint32 object, uint32 mode)
{
    FUNCTION_MARKER(0x8004DF88, "SLUS_008.06");
    G3FillMemory80086094(0x800a224c, 0, 68);
    uint32 previous_mode = r_u32(0x80096a88);
    w_u32(0x800a2250, mode);
    w_u16(0x800a226c, 0);
    w_u32(0x800a2270, 0x1000);
    w_u32(0x800a224c, previous_mode);
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
        uint32 destination = 0x800a2282 + index * 4;
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        uint32 low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15u << low_lane) & 15);
    }
    uint32 high_address = object + 3;
    uint32 high_lane = high_address & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
    uint32 low_lane = object & 3;
    uint32 low = r_masked_u32(object & 0xfffffffc, (15u << low_lane) & 15);
    uint32 word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    uint32 z = (uint32)r_s16(object + 4);
    w_masked_u32(0x800a227c, word, 15);
    w_masked_u32(0x800a227c, word, 15);
    w_u16(0x800a2280, z);
    w_u32(0x800a2274, 1);
    w_u32(0x800a228c, 30);
    return 30;
}

uint32 G3Function8004E03C(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004E03C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x70;
    uint32 mode = r_u32(0x800a2250);
    uint32 target = r_u32(0x80099654 + (mode << 3));
    w_u32(frame + 0x58, 0);
    G3DispatchStateEvent(target, object, game, 4, frame + 0x28, frame);
    uint32 coordinate = r_u16(frame + 0x28);
    uint32 delta_x = coordinate - r_u16(object);
    sint32 x = (sint16)delta_x;
    uint32 squared_x = (uint32)(x * x);
    w_u16(frame + 0x20, delta_x);
    coordinate = r_u16(frame + 0x2a);
    uint32 delta_y = coordinate - r_u16(object + 2);
    sint32 y = (sint16)delta_y;
    uint32 squared_y = (uint32)(y * y);
    w_u16(frame + 0x22, delta_y);
    coordinate = r_u16(frame + 0x2c);
    uint32 delta_z = coordinate - r_u16(object + 4);
    sint32 z = (sint16)delta_z;
    uint32 squared_z = (uint32)(z * z);
    uint32 requested_angle = (uint32)r_s16(frame + 0x34);
    w_u16(frame + 0x24, delta_z);
    uint32 current_angle = (uint32)r_s16(object + 0x18);
    uint32 squared_length = squared_x + squared_y + squared_z;
    uint32 difference = G3Function80038B48(current_angle, requested_angle);
    bool close = false;
    if (r_u32(0x800a2274) != 0 && (sint32)squared_length < 64)
    {
        sint32 signed_difference = (sint16)difference;
        uint32 magnitude = signed_difference < 0 ? 0u - (uint32)signed_difference : (uint32)signed_difference;
        if ((sint32)magnitude < 8)
        {
            w_u32(frame + 0x58, 1);
            close = true;
        }
    }
    if (!close)
    {
        uint32 progress = r_u32(0x800a2278);
        w_u32(frame + 0x10, frame + 0x40);
        w_u32(frame + 0x14, frame + 0x4c);
        w_u32(frame + 0x1c, frame + 0x58);
        w_u32(frame + 0x18, progress);
        uint32 result = G3Function8004E2F4(frame + 0x28, frame + 0x34, 0x800a227c, 0x800a2282, frame);
        if (result == 0)
            return result;
    }
    uint32 completed = r_u32(frame + 0x58);
    w_u32(0x800a2274, 0);
    if (completed != 0)
    {
        uint32 words[4];
        for (uint32 index = 0; index < 4; ++index)
            words[index] = r_u32(frame + 0x28 + index * 4);
        for (uint32 index = 0; index < 4; ++index)
            w_u32(frame + 0x40 + index * 4, words[index]);
        words[0] = r_u32(frame + 0x38);
        words[1] = r_u32(frame + 0x3c);
        w_u32(frame + 0x50, words[0]);
        w_u32(frame + 0x54, words[1]);
        G3Function8004E1F4(object, frame);
    }
    return G3Function8005A248(object, frame + 0x40);
}

uint32 G3Function8004E1F4(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004E1F4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 state = r_u32(0x800a2250);
    G3Function80015098(state);
    uint32 mode = r_u32(0x80096a88);
    uint32 target = r_u32(0x80099654 + (mode << 3));
    if (target == 0)
        return target;
    uint32 game = r_u32(0x80097048);
    uint32 payload = r_u32(0x80096a8c);
    return G3DispatchStateEvent(target, object, game, 16, payload, frame);
}

uint32 G3Function8004E260(uint32 object, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004E260, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 state = r_u32(0x800a2250);
    G3Function80015098(state);
    uint32 mode = r_u32(0x80096a88);
    uint32 target = r_u32(0x80099654 + (mode << 3));
    if (target == 0)
        return target;
    uint32 game = r_u32(0x80097048);
    return G3DispatchStateEvent(target, object, game, 11, payload, frame);
}

uint32 G3Function8004E2F4(uint32 position, uint32 rotation, uint32 previous_position, uint32 previous_rotation, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004E2F4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x58;
    uint32 output_position = r_u32(guest_stack + 0x10);
    uint32 output_rotation = r_u32(guest_stack + 0x14);
    uint32 gated = r_u32(guest_stack + 0x18);
    uint32 completed = r_u32(guest_stack + 0x1c);
    w_u32(completed, 0);
    if (gated != 0 && r_u32(0x80096b44) == 0)
        return 0;
    for (uint32 index = 0; index < 3; ++index)
    {
        uint32 coordinate = r_u16(position + index * 2);
        uint32 previous = r_u16(previous_position + index * 2);
        w_u16(frame + 0x18 + index * 2, coordinate - previous);
    }
    for (uint32 index = 0; index < 3; ++index)
    {
        uint32 angle = r_u16(rotation + index * 2);
        uint32 previous = r_u16(previous_rotation + index * 2);
        w_u16(frame + 0x20 + index * 2, angle - previous);
    }
    uint32 distance = G3Function8002FA50(frame + 0x18, frame);
    uint32 angle = r_u16(frame + 0x20) & 0xfff;
    w_u16(frame + 0x20, angle);
    if (angle >= 0x800)
        w_u16(frame + 0x20, angle - 0x1000);
    angle = r_u16(frame + 0x24) & 0xfff;
    w_u16(frame + 0x24, angle);
    if (angle >= 0x800)
        w_u16(frame + 0x24, angle - 0x1000);
    if (distance == 0 && r_s16(frame + 0x20) == 0 && r_s16(frame + 0x22) == 0 && r_s16(frame + 0x24) == 0)
    {
        w_u32(completed, 1);
        return 1;
    }
    uint32 duration = r_u32(0x800a228c);
    uint32 step = duration == 0 ? 0xffffffff : (uint32)(4096 / (sint32)duration);
    uint32 game = r_u32(0x80097048);
    uint32 focus = r_u32(game + 0x4c);
    uint32 previous = r_u16(previous_position);
    uint32 coordinate = r_u16(focus);
    w_u16(frame + 0x28, coordinate - previous);
    focus = r_u32(game + 0x4c);
    previous = r_u16(previous_position + 2);
    coordinate = r_u16(focus + 2);
    uint32 rate = (uint32)r_s16(0x800a226c);
    w_u16(frame + 0x2a, coordinate - previous);
    focus = r_u32(game + 0x4c);
    previous = r_u16(previous_position + 4);
    coordinate = r_u16(focus + 4);
    uint32 progress = (uint32)r_s16(0x800a2270);
    w_u16(frame + 0x2c, coordinate - previous);
    uint32 change = G3Function80064680(progress, rate, step, frame);
    change = (uint32)(sint16)change;
    uint32 magnitude = (sint32)change < 0 ? 0u - change : change;
    if ((sint32)magnitude < 9)
    {
        w_u32(completed, 1);
        return 1;
    }
    progress = r_u32(0x800a2270) + change;
    uint32 weight = 0x1000u - progress;
    uint32 product = weight * distance;
    w_u32(0x800a2270, progress);
    if ((sint32)product < 0)
        product += 0xfff;
    uint32 length = (uint32)((sint32)(product << 4) >> 16);
    G3Function8002F9A8(frame + 0x10, length, frame + 0x18, frame);
    for (uint32 index = 0; index < 3; ++index)
    {
        coordinate = r_u16(frame + 0x10 + index * 2);
        previous = r_u16(0x800a227c + index * 2);
        w_u16(output_position + index * 2, coordinate + previous);
    }
    uint32 pitch = (uint32)r_s16(frame + 0x20) * weight;
    if ((sint32)pitch < 0)
        pitch += 0xfff;
    uint32 yaw = (uint32)r_s16(frame + 0x24) * weight;
    pitch = (uint32)((sint32)pitch >> 12);
    w_u16(frame + 0x30, pitch);
    w_u16(frame + 0x32, 0);
    if ((sint32)yaw < 0)
        yaw += 0xfff;
    yaw = (uint32)((sint32)yaw >> 12);
    w_u16(frame + 0x34, yaw);
    previous = r_u16(previous_rotation);
    w_u16(output_rotation, pitch + previous);
    coordinate = r_u16(frame + 0x32);
    previous = r_u16(previous_rotation + 2);
    w_u16(output_rotation + 2, coordinate + previous);
    coordinate = r_u16(frame + 0x34);
    previous = r_u16(previous_rotation + 4);
    w_u16(output_rotation + 4, coordinate + previous);
    return 1;
}

uint32 G3Function8005402C(void)
{
    FUNCTION_MARKER(0x8005402C, "SLUS_008.06");
    return 0;
}

uint32 G3Function80050024(uint32 object, uint32 game, uint32 event, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80050024, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    switch (event)
    {
    case 0:
        G3Function8004F8C8(object, frame);
        return 1;
    case 1:
        G3Function8004FE9C(object, frame);
        return 1;
    case 4:
        G3Function8004F790(payload, frame);
        return 1;
    case 6:
        G3Function8004F764();
        return 1;
    case 7:
    case 8:
    case 22:
        return 1;
    case 24:
        return payload == 19;
    default:
        return 0;
    }
}

uint32 G3Function8004F764(void)
{
    FUNCTION_MARKER(0x8004F764, "SLUS_008.06");
    return G3FillMemory80086094(0x800a22f8, 0, 40);
}

uint32 G3Function8004E2D0(uint32 transition)
{
    FUNCTION_MARKER(0x8004E2D0, "SLUS_008.06");
    w_u32(0x800a2278, transition == 0xffffffff);
    return 1;
}

uint32 G3Function8004F790(uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004F790, "SLUS_008.06");
    uint32 path_reference = r_u32(0x80096af8);
    uint32 frame = guest_stack - 0x30;
    if (path_reference != 0)
    {
        uint32 x = r_u16(0x80096b38);
        uint32 y = r_u16(0x80096b3a);
        uint32 z = r_u16(0x80096b3c);
        w_u16(frame + 0x20, x);
        w_u16(frame + 0x22, y);
        w_u16(frame + 0x24, z);
        w_u32(frame + 0x10, 0xffffffff);
        uint32 path = r_u32(path_reference);
        G3Function8004FBB8(frame + 0x18, path, 0x800a22fc, frame + 0x20, frame);
        for (uint32 index = 0; index < 3; ++index)
        {
            uint32 coordinate = r_u16(frame + 0x18 + index * 2);
            w_u16(output + index * 2, coordinate);
        }
        for (uint32 index = 0; index < 3; ++index)
        {
            uint32 coordinate = r_u16(frame + 0x20 + index * 2);
            w_u16(output + 6 + index * 2, coordinate);
        }
    }
    uint32 result = G3Function8005A1D8(output + 0x0c, output + 6, output, frame);
    w_u32(output + 0x14, result);
    return result;
}

uint32 G3Function8004F8C8(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004F8C8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    G3FillMemory80086094(0x800a22f8, 0, 40);
    uint32 path = r_u32(0x800a2300);
    if (path == 0)
    {
        uint32 reference = r_u32(0x80096af8);
        if (reference != 0)
        {
            path = r_u32(reference);
            w_u32(0x800a2300, path);
        }
        path = r_u32(0x800a2300);
        if (path == 0)
            return path;
    }
    uint32 flags = r_u32(0x800a22fc);
    path = r_u32(0x800a2300);
    w_u32(0x800a22fc, flags | 0x10000);
    uint32 high_address = object + 3;
    uint32 high_lane = high_address & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
    uint32 low_lane = object & 3;
    uint32 low = r_masked_u32(object & 0xfffffffc, (15u << low_lane) & 15);
    uint32 word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    uint32 z = (uint32)r_s16(object + 4);
    high_address = frame + 0x31;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
    uint32 low_address = frame + 0x2e;
    low_lane = low_address & 3;
    w_masked_u32(low_address & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
    w_u16(frame + 0x32, z);
    uint32 angle = r_u16(object + 0x10);
    w_u16(frame + 0x34, angle);
    uint32 selected = G3Function8004002C(path, 0x8004f85c, frame + 0x28, frame);
    uint32 owner = r_u32(0x80096b34);
    uint32 words[4];
    for (uint32 index = 0; index < 4; ++index)
        words[index] = r_u32(selected + index * 4);
    for (uint32 index = 0; index < 4; ++index)
        w_u32(frame + 0x18 + index * 4, words[index]);
    high_address = frame + 0x1b;
    high_lane = high_address & 3;
    high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
    low_address = frame + 0x18;
    low_lane = low_address & 3;
    low = r_masked_u32(low_address & 0xfffffffc, (15u << low_lane) & 15);
    word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    w_masked_u32(0x800a22fc, word, 15);
    w_masked_u32(0x800a22fc, word, 15);
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 source = frame + 0x1c + index * 4;
        high_address = source + 3;
        high_lane = high_address & 3;
        high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        low_lane = source & 3;
        low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
        words[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 destination = frame + 0x38 + index * 4;
        high_address = destination + 3;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15u << low_lane) & 15);
    }
    uint32 coordinate = r_u16(object);
    uint32 previous = r_u16(owner + 0x58);
    uint32 delta_x = coordinate - previous;
    sint32 x = (sint16)delta_x;
    uint32 squared_x = (uint32)(x * x);
    w_u16(frame + 0x10, delta_x);
    coordinate = r_u16(object + 2);
    previous = r_u16(owner + 0x5a);
    uint32 delta_y = coordinate - previous;
    sint32 y = (sint16)delta_y;
    uint32 squared_y = (uint32)(y * y);
    w_u16(frame + 0x12, delta_y);
    coordinate = r_u16(object + 4);
    previous = r_u16(owner + 0x5c);
    uint32 delta_z = coordinate - previous;
    sint32 signed_z = (sint16)delta_z;
    uint32 squared = squared_x + squared_y + (uint32)(signed_z * signed_z);
    w_u16(frame + 0x14, delta_z);
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
    w_u16(0x800a2304, result);
    return result;
}

uint32 G3Function8004FE9C(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004FE9C, "SLUS_008.06");
    uint32 path = r_u32(0x800a2300);
    uint32 frame = guest_stack - 0x48;
    if (path == 0)
        return path;
    uint32 owner = r_u32(0x80096b34);
    for (uint32 index = 0; index < 3; ++index)
    {
        uint32 coordinate = r_u16(owner + 0x58 + index * 2);
        w_u16(frame + 0x30 + index * 2, coordinate);
    }
    w_u32(frame + 0x10, 100);
    G3Function8004FBB8(frame + 0x18, path, 0x800a22fc, frame + 0x30, frame);
    uint32 result = G3Function8005A1D8(frame + 0x24, frame + 0x30, frame + 0x18, frame);
    w_u32(frame + 0x2c, result);
    G3Function8005A248(object, frame + 0x18);
    owner = r_u32(0x80096b34);
    uint32 angle = (r_u16(owner + 0x64) - 0x400) & 0xfff;
    result = G3Function80040220(frame + 0x18, frame);
    return G3Function80038B48(angle, (uint32)(sint16)result);
}

uint32 G3Function80040220(uint32 position, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80040220, "SLUS_008.06");
    uint32 global_x = r_u16(0x800a2180);
    uint32 frame = guest_stack - 0x20;
    uint32 coordinate = r_u16(position);
    uint32 global_y = r_u16(0x800a2182);
    uint32 x = global_x - coordinate;
    w_u16(frame + 0x10, x);
    coordinate = r_u16(position + 2);
    uint32 y = global_y - coordinate;
    w_u16(frame + 0x12, y);
    uint32 global_z = r_u16(0x800a2184);
    coordinate = r_u16(position + 4);
    w_u16(frame + 0x14, global_z - coordinate);
    uint32 result = G3Function8002F240((uint32)(sint16)y, (uint32)(sint16)x, frame);
    return (uint32)(sint16)result;
}

uint32 G3Function8004002C(uint32 path, uint32 callback, uint32 context, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004002C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    w_u16(frame + 0x18, 0);
    uint32 state = r_u32(frame + 0x18) & 0x1ffff;
    w_u32(frame + 0x18, state);
    uint32 count = r_u16(path + 4);
    w_u16(frame + 0x20, count - 1);
    state = r_u32(frame + 0x20) & 0x1ffff;
    w_u32(frame + 0x20, state);
    w_u32(frame + 0x10, frame + 0x18);
    w_u32(frame + 0x14, frame + 0x20);
    uint32 result = G3Function8003FCF4(path, callback, context, 0x666, frame);
    for (uint32 pass = 0; pass < 2; ++pass)
    {
        uint32 first;
        uint32 second;
        if (r_s16(0x800a2170) < 0)
        {
            first = 0x800a2150;
            second = 0x800a2160;
        }
        else if (r_s16(0x800a2160) < 0)
        {
            first = 0x800a2170;
            second = 0x800a2150;
        }
        else
        {
            first = 0x800a2170;
            second = 0x800a2160;
        }
        for (uint32 index = 0; index < 2; ++index)
        {
            uint32 source = index == 0 ? first : second;
            uint32 word = r_masked_u32(source, 15);
            word = r_masked_u32(source, 15);
            uint32 destination = frame + 0x18 + index * 8;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            uint32 low_lane = destination & 3;
            w_masked_u32(destination & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
        }
        w_u32(frame + 0x10, frame + 0x18);
        w_u32(frame + 0x14, frame + 0x20);
        result = G3Function8003FCF4(path, callback, context, pass == 0 ? 0x199 : 40, frame);
    }
    return result;
}

uint32 G3Function8004FBB8(uint32 output, uint32 path, uint32 state, uint32 focus, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004FBB8, "SLUS_008.06");
    uint32 radius = (uint32)r_s16(0x800a2304);
    uint32 frame = guest_stack - 0x80;
    uint32 limit = r_u32(guest_stack + 0x10);
    uint32 game = r_u32(0x80097048);
    w_u32(frame + 0x28, radius);
    for (uint32 index = 0; index < 3; ++index)
    {
        uint32 coordinate = r_u16(focus + index * 2);
        w_u16(frame + 0x2c + index * 2, coordinate);
    }
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 target = r_u32(game + 0x4c);
        uint32 coordinate = r_u16(target + index * 2);
        w_u16(frame + 0x34 + index * 2, coordinate);
    }
    uint32 target = r_u32(game + 0x4c);
    uint32 coordinate = r_u16(target + 4);
    w_u32(frame + 0x44, state);
    w_u16(frame + 0x38, coordinate);
    uint32 selected = G3Function8004002C(path, 0x8004faac, frame + 0x28, frame);
    game = r_u32(0x80097048);
    uint32 words[4];
    for (uint32 index = 0; index < 4; ++index)
        words[index] = r_u32(selected + index * 4);
    for (uint32 index = 0; index < 4; ++index)
        w_u32(frame + 0x18 + index * 4, words[index]);
    uint32 squares[3];
    for (uint32 index = 0; index < 3; ++index)
    {
        target = r_u32(game + 0x4c);
        coordinate = r_u16(frame + 0x1c + index * 2);
        uint32 previous = r_u16(target + index * 2);
        uint32 delta = coordinate - previous;
        sint32 signed_delta = (sint16)delta;
        squares[index] = (uint32)(signed_delta * signed_delta);
        w_u16(frame + 0x10 + index * 2, delta);
    }
    sint32 signed_limit = (sint16)limit;
    uint32 result = squares[0] + squares[1];
    uint32 squared = result + squares[2];
    bool bounded = false;
    if (signed_limit > 0)
    {
        uint32 squared_limit = (uint32)(signed_limit * signed_limit);
        result = (sint32)squared_limit < (sint32)squared;
        bounded = result != 0;
    }
    uint32 copy_source;
    if (bounded)
    {
        sint32 selected_index = r_s16(frame + 0x18);
        sint32 current_index = r_s16(state);
        bool forward;
        if (selected_index == current_index)
        {
            uint32 selected_state = r_u32(frame + 0x18);
            uint32 current_fraction = r_u16(state + 2);
            forward = (current_fraction >> 1) < (selected_state >> 17);
        }
        else
            forward = current_index < selected_index;
        uint32 distance = forward ? (uint32)signed_limit : 0u - (uint32)signed_limit;
        result = G3Function80040B70(path, distance, state, frame);
        if (result != 0)
            copy_source = result;
        else
        {
            game = r_u32(0x80097048);
            for (uint32 index = 0; index < 3; ++index)
            {
                target = r_u32(game + 0x4c);
                coordinate = r_u16(frame + 0x1c + index * 2);
                uint32 previous = r_u16(target + index * 2);
                w_u16(frame + 0x10 + index * 2, coordinate - previous);
            }
            G3Function8002F9A8(frame + 0x10, (uint32)signed_limit, frame + 0x10, frame);
            game = r_u32(0x80097048);
            for (uint32 index = 0; index < 3; ++index)
            {
                target = r_u32(game + 0x4c);
                coordinate = r_u16(frame + 0x10 + index * 2);
                uint32 previous = r_u16(target + index * 2);
                result = coordinate + previous;
                w_u16(output + index * 2, result);
            }
            return result;
        }
    }
    else
    {
        uint32 source = frame + 0x18;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
        uint32 word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        high_address = state + 3;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        low_lane = state & 3;
        w_masked_u32(state & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
        copy_source = frame + 0x1c;
    }
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 source = copy_source + index * 4;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
        words[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 destination = output + index * 4;
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        uint32 low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15u << low_lane) & 15);
    }
    return result;
}

uint32 G3Function8004F85C(uint32 position, uint32 context, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004F85C, "SLUS_008.06");
    uint32 coordinate = r_u16(context + 6);
    uint32 previous = r_u16(position);
    uint32 delta_x = coordinate - previous;
    sint32 x = (sint16)delta_x;
    uint32 squared_x = (uint32)(x * x);
    uint32 frame = guest_stack - 8;
    w_u16(frame, delta_x);
    coordinate = r_u16(context + 8);
    previous = r_u16(position + 2);
    uint32 delta_y = coordinate - previous;
    sint32 y = (sint16)delta_y;
    uint32 squared_y = (uint32)(y * y);
    w_u16(frame + 2, delta_y);
    previous = r_u16(position + 4);
    coordinate = r_u16(context + 0x0a);
    w_u16(frame + 4, coordinate - previous);
    return squared_x + squared_y;
}

uint32 G3Function8004FAAC(uint32 position, uint32 context, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004FAAC, "SLUS_008.06");
    uint32 coordinate = r_u16(position);
    uint32 previous = r_u16(context + 4);
    uint32 delta_x = coordinate - previous;
    sint32 x = (sint16)delta_x;
    uint32 squared_x = (uint32)(x * x);
    return G3Function8004FAC8(position, context, delta_x, squared_x, guest_stack);
}

uint32 G3Function8004FAC8(uint32 position, uint32 context, uint32 incoming_v1, uint32 incoming_lo, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004FAC8, "SLUS_008.06");
    uint32 frame = guest_stack - 8;
    w_u16(frame, incoming_v1);
    uint32 coordinate = r_u16(position + 2);
    uint32 previous = r_u16(context + 6);
    uint32 delta_y = coordinate - previous;
    sint32 y = (sint16)delta_y;
    uint32 squared_y = (uint32)(y * y);
    w_u16(frame + 4, 0);
    w_u16(frame + 2, delta_y);
    uint32 radius = r_u32(context);
    uint32 squared_radius = radius * radius;
    coordinate = r_u16(position);
    previous = r_u16(context + 0x0c);
    uint32 delta_x = coordinate - previous;
    sint32 x = (sint16)delta_x;
    uint32 squared_x = (uint32)(x * x);
    w_u16(frame, delta_x);
    previous = r_u16(context + 0x0e);
    coordinate = r_u16(position + 2);
    w_u16(frame + 4, 0);
    delta_y = coordinate - previous;
    w_u16(frame + 2, delta_y);
    radius = r_u32(context);
    sint64 product = (sint64)(sint32)radius * 0x55555556;
    uint32 high_product = (uint32)(product >> 32);
    y = (sint16)delta_y;
    uint32 squared_second_y = (uint32)(y * y);
    uint32 sign = (uint32)((sint32)radius >> 31);
    uint32 third = high_product - sign;
    uint32 squared_third = third * third;
    uint32 result = incoming_lo + squared_y - squared_radius;
    if ((sint32)result < 0)
        result = 0u - result;
    uint32 second_distance = squared_x + squared_second_y;
    uint32 half = radius - sign;
    if ((sint32)squared_third < (sint32)second_distance)
    {
        half = (uint32)((sint32)half >> 1);
        result += half * half;
    }
    return result;
}

uint32 G3Function8003FCF4(uint32 path, uint32 callback, uint32 context, uint32 step, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003FCF4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    w_u32(guest_stack + 4, callback);
    uint32 lower = r_u32(guest_stack + 0x10);
    uint32 upper = r_u32(guest_stack + 0x14);
    bool previous_best = false;
    w_u32(guest_stack + 8, context);
    uint32 points = r_u32(path);
    w_u16(0x800a2150, 0xffff);
    uint32 packed = r_u32(0x800a2150);
    w_u32(0x800a215c, 0x7fffffff);
    w_u16(0x800a2154, 0);
    w_u16(0x800a2156, 0);
    w_u16(0x800a2158, 0);
    w_u32(0x800a2150, packed | 0xfffe0000);
    for (uint32 block = 0; block < 2; ++block)
    {
        uint32 source = 0x800a2150 + block * 0x10;
        uint32 destination = source + 0x10;
        for (uint32 pair = 0; pair < 2; ++pair)
        {
            uint32 first_word = r_u32(source + pair * 8);
            uint32 second_word = r_u32(source + pair * 8 + 4);
            w_u32(destination + pair * 8, first_word);
            w_u32(destination + pair * 8 + 4, second_word);
        }
    }
    uint32 words[4];
    for (uint32 index = 0; index < 3; ++index)
        words[index] = r_u32(0x800a2170 + index * 4);
    for (uint32 index = 0; index < 3; ++index)
        w_u32(frame + 0x20 + index * 4, words[index]);
    words[3] = r_u32(0x800a217c);
    w_u32(frame + 0x2c, words[3]);
    uint32 fraction = r_u16(lower + 2) >> 1;
    uint32 segment = (uint32)r_s16(lower);
    sint32 upper_segment = r_s16(upper);
    w_u16(frame + 0x30, step);
    if (upper_segment < (sint32)segment)
        return 0x800a2150;
    for (;;)
    {
        w_u16(frame + 0x10, segment);
        if ((sint32)fraction <= 0x7fff)
        {
            sint32 signed_segment = (sint16)segment;
            uint32 offset = (uint32)signed_segment << 5;
            uint32 first = points + offset;
            uint32 second = points + offset + 0x20;
            for (;;)
            {
                packed = r_u32(frame + 0x10);
                packed = (packed & 0x1ffff) | (fraction << 17);
                w_u32(frame + 0x10, packed);
                if ((packed & 0xfffe0000) == 0)
                {
                    for (uint32 index = 0; index < 3; ++index)
                    {
                        uint32 coordinate = r_u16(first + 2 + index * 2);
                        w_u16(frame + 0x14 + index * 2, coordinate);
                    }
                }
                else
                {
                    uint32 alpha = (packed >> 17) << 15;
                    if ((sint32)alpha < 0)
                        alpha += 0x7fff;
                    G3Function8003F290(frame + 0x14, alpha >> 15, first, second, frame);
                }
                uint32 current_context = r_u32(guest_stack + 8);
                uint32 target = r_u32(guest_stack + 4);
                uint32 score = G3DispatchPathScore(target, frame + 0x14, frame + 0x10, current_context, frame);
                w_u32(frame + 0x1c, score);
                if (previous_best)
                {
                    previous_best = false;
                    for (uint32 index = 0; index < 3; ++index)
                        words[index] = r_u32(frame + 0x10 + index * 4);
                    for (uint32 index = 0; index < 3; ++index)
                        w_u32(0x800a2160 + index * 4, words[index]);
                    words[3] = r_u32(frame + 0x1c);
                    w_u32(0x800a216c, words[3]);
                }
                score = r_u32(frame + 0x1c);
                uint32 best = r_u32(0x800a215c);
                if ((sint32)score < (sint32)best)
                {
                    previous_best = true;
                    for (uint32 index = 0; index < 3; ++index)
                        words[index] = r_u32(frame + 0x20 + index * 4);
                    for (uint32 index = 0; index < 3; ++index)
                        w_u32(0x800a2170 + index * 4, words[index]);
                    words[3] = r_u32(frame + 0x2c);
                    w_u32(0x800a217c, words[3]);
                    for (uint32 index = 0; index < 3; ++index)
                        words[index] = r_u32(frame + 0x10 + index * 4);
                    for (uint32 index = 0; index < 3; ++index)
                        w_u32(0x800a2150 + index * 4, words[index]);
                    words[3] = r_u32(frame + 0x1c);
                    w_u32(0x800a215c, words[3]);
                    w_u16(0x800a2160, 0xffff);
                }
                uint32 current_step = r_u16(frame + 0x30);
                upper_segment = r_s16(upper);
                fraction += (uint32)(sint16)current_step;
                if (signed_segment >= upper_segment)
                {
                    uint32 upper_fraction = r_u16(upper + 2) >> 1;
                    if ((sint32)fraction >= (sint32)upper_fraction)
                        break;
                }
                for (uint32 index = 0; index < 4; ++index)
                    words[index] = r_u32(frame + 0x10 + index * 4);
                for (uint32 index = 0; index < 4; ++index)
                    w_u32(frame + 0x20 + index * 4, words[index]);
                if ((sint32)fraction > 0x7fff)
                    break;
            }
        }
        segment += 1;
        upper_segment = r_s16(upper);
        fraction = 0;
        if (upper_segment < (sint32)(sint16)segment)
            break;
    }
    return 0x800a2150;
}

uint32 G3Function8003FBE4(uint32 path, uint32 state)
{
    FUNCTION_MARKER(0x8003FBE4, "SLUS_008.06");
    if (path == 0)
        return 0;
    uint32 fraction = r_u16(state + 2) & 1;
    w_u16(state + 2, fraction);
    uint32 count = r_u16(path + 4);
    fraction = r_u16(state + 2) & 0xfffe;
    w_u16(state, count - 1);
    w_u16(state + 2, fraction);
    uint32 segment = (uint32)r_s16(state);
    uint32 points = r_u32(path);
    return points + (segment << 5) + 2;
}

uint32 G3Function80040B70(uint32 path, uint32 distance, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80040B70, "SLUS_008.06");
    uint32 frame = guest_stack - 0x58;
    uint32 stride = 0xfffffe00;
    w_u32(guest_stack + 4, distance);
    w_u32(guest_stack + 8, state);
    if ((sint32)distance > 0)
        stride = 0x200;
    if (path == 0)
        return 0;
    uint32 current_state = r_u32(guest_stack + 8);
    if (current_state == 0)
        return 0;
    uint32 requested = r_u32(guest_stack + 4);
    if (requested == 0)
        return 0;
    current_state = r_u32(guest_stack + 8);
    uint32 fraction = r_u16(current_state + 2) >> 1;
    uint32 segment = (uint32)r_s16(current_state);
    sint32 count = r_s16(path + 4);
    bool crossed = false;
    uint32 last_length = 0;
    if ((sint32)segment >= count - 1)
    {
        uint32 flags = r_u32(path + 4);
        if ((flags & 0x06000000) != 0)
        {
            fraction = 0;
            segment = 0;
        }
        else
        {
            requested = r_u32(guest_stack + 4);
            if ((sint32)requested > 0)
            {
                uint32 endpoint = G3Function8003FBE4(path, current_state);
                uint32 words[2];
                for (uint32 index = 0; index < 2; ++index)
                {
                    uint32 source = endpoint + index * 4;
                    uint32 high_address = source + 3;
                    uint32 high_lane = high_address & 3;
                    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
                    uint32 low_lane = source & 3;
                    uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
                    words[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
                }
                for (uint32 index = 0; index < 2; ++index)
                {
                    w_masked_u32(0x800a2148 + index * 4, words[index], 15);
                    w_masked_u32(0x800a2148 + index * 4, words[index], 15);
                }
                return 0x800a2148;
            }
        }
    }
    w_u32(frame + 0x28, 0);
    uint32 points = r_u32(path);
    uint32 offset = (uint32)((sint32)(segment << 16) >> 11);
    uint32 first = points + offset;
    uint32 second = points + offset + 0x20;
    if (fraction == 0)
    {
        uint32 source = first + 2;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
        uint32 word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        uint32 z = (uint32)r_s16(first + 6);
        high_address = frame + 0x23;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        uint32 destination = frame + 0x20;
        low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
        w_u16(frame + 0x24, z);
    }
    else
    {
        uint32 alpha = fraction << 15;
        if ((sint32)alpha < 0)
            alpha += 0x7fff;
        G3Function8003F290(frame + 0x20, (uint32)((sint32)alpha >> 15), first, second, frame);
    }
    requested = r_u32(guest_stack + 4);
    uint32 magnitude = (sint32)requested < 0 ? 0u - requested : requested;
    w_u32(frame + 0x2c, magnitude);
    fraction += stride;
    bool finished = false;
    for (;;)
    {
        uint32 accumulated = r_u32(frame + 0x28);
        crossed = false;
        w_u32(frame + 0x28, accumulated + last_length);
        uint32 source = frame + 0x20;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
        uint32 word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        uint32 z = (uint32)r_s16(frame + 0x24);
        high_address = frame + 0x1b;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        uint32 destination = frame + 0x18;
        low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
        w_u16(frame + 0x1c, z);
        for (;;)
        {
            uint32 sample_node = 0;
            bool copy_node = false;
            bool end_node = false;
            uint32 end_segment = 0;
            requested = r_u32(guest_stack + 4);
            if ((sint32)requested > 0 && (sint32)fraction > 0x7fff)
            {
                crossed = true;
                segment += 1;
                count = r_s16(path + 4);
                uint32 unsigned_count = r_u16(path + 4);
                bool inside = (sint32)(sint16)segment < count - 1;
                fraction = 0;
                if (!inside)
                {
                    uint32 flags = r_u32(path + 4);
                    segment = 0;
                    if ((flags & 0x06000000) == 0)
                    {
                        sample_node = second;
                        copy_node = true;
                        end_node = true;
                        end_segment = unsigned_count - 1;
                    }
                }
                if (!end_node)
                {
                    first = second;
                    points = r_u32(path);
                    offset = (uint32)((sint32)(segment << 16) >> 11);
                    sample_node = first;
                    copy_node = true;
                }
            }
            else
            {
                requested = r_u32(guest_stack + 4);
                if ((sint32)requested < 0)
                {
                    if ((sint32)fraction < 0)
                    {
                        fraction += 0x8000;
                        segment -= 1;
                        crossed = true;
                        if ((sint32)(segment << 16) < 0)
                        {
                            uint32 flags = r_u32(path + 4);
                            if ((flags & 0x06000000) == 0)
                            {
                                fraction = 0;
                                segment = 0;
                                finished = true;
                                break;
                            }
                            segment = r_u16(path + 4) - 2;
                        }
                        second = first;
                        points = r_u32(path);
                        offset = (uint32)((sint32)(segment << 16) >> 11);
                        first = points + offset;
                    }
                    else if (fraction == 0)
                    {
                        sample_node = first;
                        copy_node = true;
                    }
                }
            }
            if (copy_node)
            {
                source = sample_node + 2;
                high_address = source + 3;
                high_lane = high_address & 3;
                high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
                low_lane = source & 3;
                low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
                word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
                z = (uint32)r_s16(sample_node + 6);
                high_address = frame + 0x23;
                high_lane = high_address & 3;
                w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
                destination = frame + 0x20;
                low_lane = destination & 3;
                w_masked_u32(destination & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
                w_u16(frame + 0x24, z);
                if (end_node)
                {
                    segment = end_segment;
                    finished = true;
                    break;
                }
                if ((sint32)requested > 0)
                    second = points + offset;
            }
            else
            {
                uint32 alpha = fraction << 15;
                if ((sint32)alpha < 0)
                    alpha += 0x7fff;
                G3Function8003F290(frame + 0x20, (uint32)((sint32)alpha >> 15), first, second, frame);
            }
            uint32 deltas[3];
            uint32 squared = 0;
            for (uint32 index = 0; index < 3; ++index)
            {
                uint32 previous = r_u16(frame + 0x18 + index * 2);
                uint32 current = r_u16(frame + 0x20 + index * 2);
                deltas[index] = previous - current;
                sint32 delta = (sint16)deltas[index];
                squared += (uint32)(delta * delta);
            }
            w_u16(frame + 0x10, deltas[0]);
            w_u16(frame + 0x12, deltas[1]);
            w_u16(frame + 0x14, deltas[2]);
            if ((sint32)squared < 2 && !crossed)
            {
                stride <<= 1;
                fraction += stride;
                continue;
            }
            uint32 root = G3Function8002F368(squared << 12, frame);
            last_length = (uint32)((sint32)root >> 12);
            accumulated = r_u32(frame + 0x28);
            magnitude = r_u32(frame + 0x2c);
            uint32 travelled = accumulated + last_length;
            uint32 error = travelled - magnitude;
            if ((sint32)error < 0)
                error = 0u - error;
            if ((sint32)error <= 2)
            {
                finished = true;
                break;
            }
            fraction += stride;
            if ((sint32)magnitude >= (sint32)travelled)
                break;
            uint32 stride_magnitude = (sint32)stride < 0 ? 0u - stride : stride;
            fraction -= stride;
            if ((sint32)stride_magnitude < 3)
            {
                finished = true;
                break;
            }
            stride = (uint32)((sint32)stride >> 1);
            if (crossed)
            {
                finished = true;
                break;
            }
            fraction -= stride;
        }
        if (finished)
            break;
    }
    current_state = r_u32(guest_stack + 8);
    uint32 packed_fraction = r_u16(current_state + 2);
    w_u16(current_state, segment);
    w_u16(current_state + 2, (packed_fraction & 1) | (fraction << 1));
    for (uint32 index = 0; index < 3; ++index)
    {
        uint32 coordinate = r_u16(frame + 0x20 + index * 2);
        w_u16(0x800a2148 + index * 2, coordinate);
    }
    return crossed ? 0 : 0x800a2148;
}
