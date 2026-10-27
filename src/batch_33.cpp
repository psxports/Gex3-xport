#include "game_functions.h"
#include "psx.h"
#include <stdio.h>

uint32 G3Function80071834(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80071834, "SLUS_008.06");
    uint32 frame = guest_stack - 0x58;
    uint32 data = r_u32(object + 0x24);
    if (G3Function80071490(object) != 0)
    {
        uint32 camera = r_u32(game + 0x4c);
        (void)camera;
        uint32 camera_matrix = G3Function8004D9B0();
        uint32 state = r_u32(0x800a0284);
        uint32 direction = state + 0x14;
        if (G3Function800715BC(frame + 0x30) != 0)
            G3Function800500C0(direction, frame + 0x30, game, frame);
        G3Function8002F718(frame + 0x10);
        uint32 angle = (uint32)r_s16(data + 0xc6);
        G3Function8005BB6C(angle, frame + 0x10, frame);
        MulMatrix0((MATRIX *)psx_addr(camera_matrix, 20), (MATRIX *)psx_addr(frame + 0x10, 20), (MATRIX *)psx_addr(frame + 0x10, 20));
        return G3Function800202A4(frame + 0x10, object + 0x60, 0x15, frame);
    }
    uint32 state = r_u32(0x800a0284);
    uint32 result = r_u32(state + 0xc);
    if (result != 0)
    {
        G3Function800726FC(object, game, data, frame);
        G3Function80071640(object, frame);
        result = r_u16(data + 0xc6);
        w_u16(object + 0x64, result);
    }
    return result;
}

uint32 G3Function8007193C(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007193c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    uint32 matrix = r_u32(object + 0x30);
    uint32 camera = r_u32(game + 0x4c);
    (void)camera;
    uint32 data = r_u32(object + 0x24);
    uint32 column_x = r_u16(matrix);
    w_u16(frame + 0x10, column_x);
    matrix = r_u32(object + 0x30);
    uint32 column_y = r_u16(matrix + 6);
    w_u16(frame + 0x12, column_y);
    matrix = r_u32(object + 0x30);
    uint32 state = r_u32(0x800a0284);
    uint32 column_z = r_u16(matrix + 0xc);
    w_u16(frame + 0x14, column_z);
    uint32 direction = state + 0x14;
    uint32 direction_x = (uint32)r_s16(direction);
    uint32 dot = (uint32)(sint32)(sint16)column_x * direction_x;
    uint32 component = (uint32)r_s16(direction + 2);
    dot += (uint32)(sint32)(sint16)column_y * component;
    component = (uint32)r_s16(direction + 4);
    dot += (uint32)(sint32)(sint16)column_z * component;
    dot = (uint32)((sint32)dot >> 12);
    uint32 product = dot * direction_x;
    uint32 x = column_x - (uint32)((sint32)product >> 12);
    w_u16(frame + 0x10, x);
    component = (uint32)r_s16(direction + 2);
    product = dot * component;
    uint32 y = column_y - (uint32)((sint32)product >> 12);
    w_u16(frame + 0x12, y);
    component = (uint32)r_s16(direction + 4);
    product = dot * component;
    uint32 z = column_z - (uint32)((sint32)product >> 12);
    w_u16(frame + 0x14, z);
    bool special_mode = false;
    for (uint32 pass = 0; pass < 3; ++pass)
    {
        uint32 local = frame + 0x10;
        if (pass == 1)
        {
            uint32 first_y = (uint32)r_s16(frame + 0x12);
            component = (uint32)r_s16(direction + 4);
            product = first_y * component;
            uint32 first_z = (uint32)r_s16(frame + 0x14);
            component = (uint32)r_s16(direction + 2);
            uint32 difference = product - first_z * component;
            uint32 first_x = (uint32)r_s16(frame + 0x10);
            x = (uint32)((sint32)difference >> 12);
            w_u16(frame + 0x18, x);
            component = (uint32)r_s16(direction + 4);
            product = first_x * component;
            component = (uint32)r_s16(direction);
            difference = product - first_z * component;
            y = 0 - (uint32)((sint32)difference >> 12);
            w_u16(frame + 0x1a, y);
            component = (uint32)r_s16(direction + 2);
            product = first_x * component;
            component = (uint32)r_s16(direction);
            difference = product - first_y * component;
            z = (uint32)((sint32)difference >> 12);
            w_u16(frame + 0x1c, z);
            local = frame + 0x18;
        }
        else if (pass == 2)
        {
            if (G3Function8004D9C0() != 1)
                break;
            special_mode = true;
            uint32 direction_z = (uint32)r_s16(direction + 4);
            component = (uint32)r_s16(direction);
            product = direction_z * component;
            x = 0 - (uint32)((sint32)product >> 12);
            w_u16(frame + 0x20, x);
            component = (uint32)r_s16(direction + 2);
            product = direction_z * component;
            y = 0 - (uint32)((sint32)product >> 12);
            w_u16(frame + 0x22, y);
            component = (uint32)r_s16(direction + 4);
            product = direction_z * component;
            z = 0x1000 - (uint32)((sint32)product >> 12);
            w_u16(frame + 0x24, z);
            local = frame + 0x20;
        }
        sint32 absolute_x = (sint16)x;
        sint32 absolute_y = (sint16)y;
        sint32 absolute_z = (sint16)z;
        if (absolute_x < 0)
            absolute_x = -absolute_x;
        if (absolute_y < 0)
            absolute_y = -absolute_y;
        if (absolute_z < 0)
            absolute_z = -absolute_z;
        sint32 minimum = (sint16)absolute_x;
        sint32 middle = (sint16)absolute_y;
        sint32 maximum = (sint16)absolute_z;
        if (minimum > middle)
        {
            sint32 temporary = minimum;
            minimum = middle;
            middle = temporary;
        }
        if (middle > maximum)
        {
            sint32 temporary = middle;
            middle = maximum;
            maximum = temporary;
        }
        if (minimum > middle)
        {
            sint32 temporary = minimum;
            minimum = middle;
            middle = temporary;
        }
        sint32 length = (30 * maximum + 12 * middle + 9 * minimum) / 8;
        if (length != 0)
        {
            uint32 numerator = (uint32)r_s16(local) << 14;
            uint32 normalized_x = (uint32)((sint32)numerator / length);
            numerator = (uint32)r_s16(local + 2) << 14;
            uint32 normalized_y = (uint32)((sint32)numerator / length);
            numerator = (uint32)r_s16(local + 4) << 14;
            uint32 normalized_z = (uint32)((sint32)numerator / length);
            w_u16(local, normalized_x);
            w_u16(local + 2, normalized_y);
            w_u16(local + 4, normalized_z);
        }
    }
    uint32 result;
    if (special_mode)
    {
        uint32 angle = G3Function8002F540(frame + 0x18, frame + 0x20, frame);
        result = (uint32)((sint32)(angle << 16) >> 16) + 0x1800;
    }
    else
    {
        uint32 second_x = r_u16(frame + 0x18);
        uint32 first_x = r_u16(frame + 0x10);
        w_u16(frame + 0x2a, second_x);
        uint32 first_y = r_u16(frame + 0x12);
        w_u16(frame + 0x28, first_x);
        direction_x = r_u16(direction);
        uint32 second_y = r_u16(frame + 0x1a);
        w_u16(frame + 0x2e, first_y);
        w_u16(frame + 0x30, second_y);
        w_u16(frame + 0x2c, direction_x);
        uint32 direction_y = r_u16(direction + 2);
        uint32 first_z = r_u16(frame + 0x14);
        uint32 second_z = r_u16(frame + 0x1c);
        w_u16(frame + 0x34, first_z);
        w_u16(frame + 0x36, second_z);
        w_u16(frame + 0x32, direction_y);
        uint32 direction_z = r_u16(direction + 4);
        w_u16(frame + 0x38, direction_z);
        G3Function800202A4(frame + 0x28, frame + 0x48, 0x15, frame);
        result = (uint32)r_s16(frame + 0x4c);
    }
    w_u16(data + 0xc6, result);
    w_u16(data + 0xc8, result);
    return result;
}

uint32 G3Function80070B54(uint32 game, uint32 animations, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80070b54, "SLUS_008.06");
    uint32 frame = guest_stack - 0xb0;
    uint32 level_pointer = r_u32(game + 0x48);
    uint32 view = r_u32(game + 0x50);
    uint32 level = r_u32(level_pointer);
    uint32 angles_pointer = r_u32(view + 0x24);
    uint32 attempt = 0;
    G3FillMemory80086094(frame + 0x68, 0, 8);
    for (;;)
    {
        uint32 player = r_u32(0x8009efc4);
        G3Function8005BD00(player + 0x60, frame + 0x10);
        uint32 state = r_u32(0x800a0284);
        if (r_u32(state + 0xc) != 0)
        {
            uint32 component = r_u16(state + 0x14);
            w_u16(frame + 0x14, component);
            component = r_u16(state + 0x16);
            w_u16(frame + 0x1a, component);
            component = r_u16(state + 0x18);
            w_u16(frame + 0x20, component);
        }
        for (uint32 index = 0; index < 14; ++index)
        {
            uint32 record = animations + index * 0x34;
            ApplyMatrixSV((MATRIX *)psx_addr(frame + 0x10, 20), (SVECTOR *)psx_addr(record, 8), (SVECTOR *)psx_addr(record + 0x24, 6));
            player = r_u32(0x8009efc4);
            for (uint32 axis = 0; axis < 3; ++axis)
            {
                uint32 component = r_u16(record + 0x24 + axis * 2);
                uint32 position = r_u16(player + 0x58 + axis * 2);
                w_u16(record + 0x24 + axis * 2, component + position);
            }
        }
        uint32 column = r_u16(frame + 0x14);
        state = r_u32(0x800a0284);
        uint32 origin = r_u16(state + 0x24);
        w_u16(state + 0x24, origin - (uint32)((sint32)(column << 16) >> 21));
        column = r_u16(frame + 0x1a);
        origin = r_u16(state + 0x26);
        w_u16(state + 0x26, origin - (uint32)((sint32)(column << 16) >> 21));
        column = r_u16(frame + 0x20);
        origin = r_u16(state + 0x28);
        w_u16(state + 0x28, origin - (uint32)((sint32)(column << 16) >> 21));
        uint32 flags = r_u32(player + 0x10);
        w_u32(player + 0x10, flags | 0x40);
        for (uint32 index = 0; index < 14; ++index)
        {
            uint32 record = animations + index * 0x34;
            uint32 x = r_u16(record + 0x24);
            uint32 y = r_u16(record + 0x26);
            uint32 z = r_u16(record + 0x28);
            w_u16(record + 0x2c, x);
            w_u16(record + 0x2e, y);
            w_u16(record + 0x30, z);
        }
        state = r_u32(0x800a0284);
        uint32 component = r_u16(state + 0x24);
        w_u32(state + 0xc, 0);
        w_u16(frame + 0x60, component);
        component = r_u16(state + 0x26);
        w_u16(frame + 0x62, component);
        uint32 origin_z = r_u16(state + 0x28);
        player = r_u32(0x8009efc4);
        w_u32(frame + 0x44, state + 0x24);
        w_u32(frame + 0x48, animations + 0x24);
        w_u32(frame + 0x54, 0x7f);
        w_u32(frame + 0x58, player);
        w_u16(frame + 0x64, origin_z);
        G3Function80061CA4(frame + 0x30, game, frame);
        G3Function8006F11C(frame + 0x30, frame + 0x68, frame);
        uint32 kind = r_u32(frame + 0x30);
        if (kind == 3)
        {
            state = r_u32(0x800a0284);
            uint32 face = r_u32(frame + 0x34);
            w_u32(state + 0xc, face);
            face = r_u32(frame + 0x34);
            uint32 normals = r_u32(level + 0x40);
            uint32 normal = (uint32)r_s16(face + 8);
            G3Function8005F304(normal, normals, state + 0x14, frame);
        }
        else if (kind == 2 || kind == 5)
        {
            state = r_u32(0x800a0284);
            uint32 face = kind == 2 ? r_u32(frame + 0x34) : 0x800a0288;
            w_u32(state + 0xc, face);
            uint32 words[2];
            for (uint32 index = 0; index < 2; ++index)
            {
                words[index] = r_masked_u32(frame + 0x3c + index * 4, 15);
                words[index] = r_masked_u32(frame + 0x3c + index * 4, 15);
            }
            for (uint32 index = 0; index < 2; ++index)
            {
                uint32 destination = state + 0x14 + index * 4;
                uint32 high_address = destination + 3;
                uint32 high_lane = high_address & 3;
                w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                uint32 low_lane = destination & 3;
                w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15 << low_lane) & 15);
            }
        }
        else if (kind == 0)
        {
            state = r_u32(0x800a0284);
            w_u32(state + 0xc, 0);
            w_u16(state + 0x14, 0);
            w_u16(state + 0x16, 0);
            w_u16(state + 0x18, 0);
        }
        G3Function800708FC(animations, level, frame);
        state = r_u32(0x800a0284);
        if (r_u32(state + 0xc) == 0)
        {
            uint32 angles = r_u32(angles_pointer);
            w_u16(angles + 8, 0);
            angles = r_u32(angles_pointer);
            w_u16(angles, 0);
            angles = r_u32(angles_pointer);
            w_u16(angles + 2, 0);
        }
        else
        {
            component = r_u16(state + 0x14);
            w_u16(frame + 0x70, component);
            component = r_u16(state + 0x16);
            w_u16(frame + 0x72, component);
            component = r_u16(state + 0x18);
            w_u16(frame + 0x74, component);
            uint32 first_mid[3];
            uint32 second_mid[3];
            for (uint32 axis = 0; axis < 3; ++axis)
            {
                uint32 first = (uint32)r_s16(animations + 0x15c + axis * 2);
                uint32 second = (uint32)r_s16(animations + 0x190 + axis * 2);
                uint32 base = r_u16(animations + 0x190 + axis * 2);
                uint32 difference = first - second;
                first_mid[axis] = base + (uint32)((sint32)difference >> 1);
                w_u16(frame + 0x78 + axis * 2, first_mid[axis]);
            }
            for (uint32 axis = 0; axis < 3; ++axis)
            {
                origin = r_u16(state + 0x24 + axis * 2);
                first_mid[axis] -= origin;
                w_u16(frame + 0x78 + axis * 2, first_mid[axis]);
            }
            for (uint32 axis = 0; axis < 3; ++axis)
            {
                uint32 first = (uint32)r_s16(animations + 0x1c4 + axis * 2);
                uint32 second = (uint32)r_s16(animations + 0x1f8 + axis * 2);
                uint32 base = r_u16(animations + 0x1f8 + axis * 2);
                uint32 difference = first - second;
                second_mid[axis] = base + (uint32)((sint32)difference >> 1);
                w_u16(frame + 0x80 + axis * 2, second_mid[axis]);
            }
            for (uint32 axis = 0; axis < 3; ++axis)
            {
                origin = r_u16(state + 0x24 + axis * 2);
                second_mid[axis] -= origin;
                w_u16(frame + 0x80 + axis * 2, second_mid[axis]);
            }
            for (uint32 pass = 0; pass < 2; ++pass)
            {
                uint32 local = frame + (pass == 0 ? 0x78 : 0x80);
                sint32 absolute_x = pass == 0 ? (sint16)first_mid[0] : r_s16(local);
                sint32 absolute_y = pass == 0 ? (sint16)first_mid[1] : r_s16(local + 2);
                sint32 absolute_z = pass == 0 ? (sint16)first_mid[2] : r_s16(local + 4);
                if (absolute_x < 0)
                    absolute_x = -absolute_x;
                if (absolute_y < 0)
                    absolute_y = -absolute_y;
                if (absolute_z < 0)
                    absolute_z = -absolute_z;
                sint32 minimum = (sint16)absolute_x;
                sint32 middle = (sint16)absolute_y;
                sint32 maximum = (sint16)absolute_z;
                if (minimum > middle)
                {
                    sint32 temporary = minimum;
                    minimum = middle;
                    middle = temporary;
                }
                if (middle > maximum)
                {
                    sint32 temporary = middle;
                    middle = maximum;
                    maximum = temporary;
                }
                if (minimum > middle)
                {
                    sint32 temporary = minimum;
                    minimum = middle;
                    middle = temporary;
                }
                sint32 length = (30 * maximum + 12 * middle + 9 * minimum) / 8;
                if (length != 0)
                {
                    uint32 numerator = (uint32)r_s16(local) << 14;
                    uint32 x = (uint32)((sint32)numerator / length);
                    numerator = (uint32)r_s16(local + 2) << 14;
                    uint32 y = (uint32)((sint32)numerator / length);
                    numerator = (uint32)r_s16(local + 4) << 14;
                    uint32 z = (uint32)((sint32)numerator / length);
                    w_u16(local, x);
                    w_u16(local + 2, y);
                    w_u16(local + 4, z);
                }
            }
            if (r_u32(0x80099fc0) != 0 && r_u32(0x80099ff4) != 0)
            {
                uint32 angle = G3Function8002F540(frame + 0x70, frame + 0x78, frame);
                uint32 angles = r_u32(angles_pointer);
                w_u16(angles + 8, angle - 0x400);
            }
            else
            {
                uint32 angles = r_u32(angles_pointer);
                w_u16(angles + 8, 0);
            }
            if (r_u32(0x8009a028) != 0 && r_u32(0x8009a05c) != 0)
            {
                uint32 angle = G3Function8002F540(frame + 0x70, frame + 0x80, frame);
                uint32 angles = r_u32(angles_pointer);
                uint32 previous = r_u16(angles + 8);
                angle -= 0x400;
                w_u16(angles + 8, previous + angle);
                angles = r_u32(angles_pointer);
                w_u16(angles, 0 - angle);
            }
            uint32 angles = r_u32(angles_pointer);
            state = r_u32(0x800a0284);
            w_u16(angles + 2, 0);
            player = r_u32(0x8009efc4);
            sint32 target = r_s16(state + 0x24);
            sint32 current = r_s16(player + 0x58);
            sint32 delta = current - target;
            if (delta < 0)
                delta = -delta;
            if (delta >= 5)
            {
                state = r_u32(0x800a0284);
                player = r_u32(0x8009efc4);
                component = r_u16(state + 0x24);
                w_u16(player + 0x58, component);
            }
            for (uint32 axis = 1; axis < 3; ++axis)
            {
                player = r_u32(0x8009efc4);
                state = r_u32(0x800a0284);
                current = r_s16(player + 0x58 + axis * 2);
                target = r_s16(state + 0x24 + axis * 2);
                delta = current - target;
                if (delta < 0)
                    delta = -delta;
                if (delta >= 5)
                {
                    state = r_u32(0x800a0284);
                    player = r_u32(0x8009efc4);
                    component = r_u16(state + 0x24 + axis * 2);
                    w_u16(player + 0x58 + axis * 2, component);
                }
            }
        }
        player = r_u32(0x8009efc4);
        flags = r_u32(player + 0x10);
        state = r_u32(0x800a0284);
        w_u32(player + 0x10, flags & 0xffffffbf);
        uint32 result = r_u32(state + 0xc);
        if (result != 0 || attempt > 0)
            return result;
        G3Function80070524(game, animations, frame);
        ++attempt;
        if (attempt >= 2)
            return 0;
    }
}

uint32 G3Function800172DC(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800172dc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 selector_word = r_u32(command + 4);
    if ((sint32)selector_word < 0)
    {
        uint32 selector = r_u16(command + 6);
        uint32 value = r_u32(command + 8);
        uint32 key = r_u16(command + 4);
        selector &= 0x7fff;
        if ((sint32)value > 0x7fffffbc && (sint32)value <= 0x7ffffffc)
        {
            uint32 offset = (0x7ffffffc - value) << 2;
            value = r_u32(0x800a0760 + offset);
        }
        uint32 target = r_u32(0x8009d354 + (selector << 2));
        key = (uint32)((sint32)(key << 16) >> 16);
        G3DispatchScriptValue(target, key, value, command, selector, frame);
    }
    else
    {
        uint32 value = r_u32(command + 8);
        (void)value;
        G3Function800361D4();
    }
    return 3;
}

uint32 G3Function8004ABE8(uint32 command, uint32 context, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004abe8, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 frame = guest_stack - 0x18;
    uint32 target = r_u32(audio + 0x554);
    if (target == 0)
        return audio;
    uint32 parameter = r_u8(command + 4);
    uint32 owner = r_u32(audio + 0x55c);
    uint32 channel = r_u8(context + 0xf0);
    uint32 value = r_u8(command + 6);
    return G3DispatchAudioParameter(target, owner, channel, parameter & 0xf, value, frame);
}

uint32 G3Function8004B960(uint32 command, uint32 context, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004b960, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function80048FA4(context);
    uint32 channel = r_u8(context + 0xf0);
    G3Function800496BC(channel);
    uint32 audio = r_u32(0x80098ff0);
    uint32 target = r_u32(audio + 0x558);
    if (target == 0)
        return audio;
    uint32 owner = r_u32(audio + 0x560);
    channel = r_u8(context + 0xf0);
    return G3DispatchAudioParameter(target, owner, channel, 0, 0x800a0000, frame);
}

uint32 G3Function800188E4(uint32 guest_stack)
{
    FUNCTION_MARKER(0x800188e4, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x18;
    uint32 player = r_u32(game + 0x50);
    uint32 data = r_u32(player + 0x24);
    uint32 object = r_u32(data + 0x1fc);
    if (object != 0)
    {
        uint32 target = r_u32(object + 0xec);
        if (target != 0)
            G3DispatchStateEvent(target, object, game, 0x7eb, player, frame);
    }
    return 1;
}

uint32 G3Function80018980(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80018980, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x18;
    uint32 object = G3Function8006C914(game, frame);
    if (object != 0)
    {
        uint32 target = r_u32(object + 0xec);
        if (target != 0)
        {
            game = r_u32(0x80097048);
            G3DispatchStateEvent(target, object, game, 0x7ee, 0, frame);
        }
    }
    return 1;
}

void G3Function8007AE0C(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007ae0c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 linked = r_u32(state + 0x1b8);
    if (linked != 0)
    {
        uint32 target = r_u32(linked + 0xec);
        if (target != 0)
        {
            G3DispatchStateEvent(target, linked, game, 2, 0, frame);
            uint32 current_game = r_u32(0x80097048);
            uint32 flags = r_u32(current_game + 0xe8);
            w_u32(current_game + 0xe8, flags | 0x08000000);
        }
    }
    w_u16(state + 0xe2, 0);
    w_u16(state + 0xe0, 0);
}

void G3Function8004AA4C(void)
{
    FUNCTION_MARKER(0x8004aa4c, "SLUS_008.06");
}

void G3Function8004AAAC(void)
{
    FUNCTION_MARKER(0x8004aaac, "SLUS_008.06");
}

void G3Function8004ADC4(void)
{
    FUNCTION_MARKER(0x8004adc4, "SLUS_008.06");
}

void G3Function8004ADCC(void)
{
    FUNCTION_MARKER(0x8004adcc, "SLUS_008.06");
}

void G3Function8004AE40(void)
{
    FUNCTION_MARKER(0x8004ae40, "SLUS_008.06");
}

void G3Function8004AF30(void)
{
    FUNCTION_MARKER(0x8004af30, "SLUS_008.06");
}

void G3Function8004AF38(void)
{
    FUNCTION_MARKER(0x8004af38, "SLUS_008.06");
}

void G3Function8004B1A4(void)
{
    FUNCTION_MARKER(0x8004b1a4, "SLUS_008.06");
}

void G3Function8004B1AC(void)
{
    FUNCTION_MARKER(0x8004b1ac, "SLUS_008.06");
}

void G3Function8004B1B4(void)
{
    FUNCTION_MARKER(0x8004b1b4, "SLUS_008.06");
}

void G3Function8004B1BC(void)
{
    FUNCTION_MARKER(0x8004b1bc, "SLUS_008.06");
}

void G3Function8004B1DC(void)
{
    FUNCTION_MARKER(0x8004b1dc, "SLUS_008.06");
}

void G3Function8004B1E4(void)
{
    FUNCTION_MARKER(0x8004b1e4, "SLUS_008.06");
}

void G3Function8004B1EC(void)
{
    FUNCTION_MARKER(0x8004b1ec, "SLUS_008.06");
}

void G3Function8004B1F4(void)
{
    FUNCTION_MARKER(0x8004b1f4, "SLUS_008.06");
}

void G3Function8004B1FC(void)
{
    FUNCTION_MARKER(0x8004b1fc, "SLUS_008.06");
}

void G3Function8004B204(void)
{
    FUNCTION_MARKER(0x8004b204, "SLUS_008.06");
}

void G3Function8004B20C(void)
{
    FUNCTION_MARKER(0x8004b20c, "SLUS_008.06");
}

void G3Function8004B3A0(void)
{
    FUNCTION_MARKER(0x8004b3a0, "SLUS_008.06");
}

void G3Function8004B3DC(void)
{
    FUNCTION_MARKER(0x8004b3dc, "SLUS_008.06");
}

void G3Function8004B3E4(void)
{
    FUNCTION_MARKER(0x8004b3e4, "SLUS_008.06");
}

void G3Function8004B658(void)
{
    FUNCTION_MARKER(0x8004b658, "SLUS_008.06");
}

void G3Function8004B660(void)
{
    FUNCTION_MARKER(0x8004b660, "SLUS_008.06");
}

void G3Function8004B668(void)
{
    FUNCTION_MARKER(0x8004b668, "SLUS_008.06");
}

void G3Shutdown8002EDC0(uint32 guest_stack)
{
    uint32 frame = guest_stack - 0x20;
    uint32 words[2];
    for (uint32 i = 0; i < 2; ++i)
    {
        uint32 source = 0x800101d8 + (i << 2);
        words[i] = r_masked_u32(source, 15);
        words[i] = r_masked_u32(source, 15);
    }
    for (uint32 i = 0; i < 2; ++i)
    {
        uint32 destination = frame + 0x10 + (i << 2);
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        uint32 low_lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, words[i] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, words[i] << (low_lane * 8), (15 << low_lane) & 15);
    }
    G3Function8003EF1C();
    G3Function8003ECE0();
    SetDispMask(0);
    ClearImage((PSX_RECT *)psx_addr(frame + 0x10, 8), 0, 0, 0);
    DrawSync(0);
    {
#ifndef NDEBUG
    printf("--->DEMO: Cleanup!\n");
#endif
    }
    G3Function8002ED54();
    G3Function80028524();
    VSync(5);
    StopCallback();
    {
#ifndef NDEBUG
    printf("--->DEMO: Bye!\n");
#endif
    }
}

/* Native SDK pointer adapter */
uint32 G3LoadImage800865C0(uint32 rectangle, uint32 pixels)
{
    PSX_RECT *native_rectangle = (PSX_RECT *)psx_addr(rectangle, 8);
    sint32 width = native_rectangle->w;
    sint32 height = native_rectangle->h;
    uint32 bytes = 0;
    if (width > 0 && height > 0)
    {
        uint32 count = (uint32)width * (uint32)height;
        bytes = ((count + 1) / 2) * 4;
    }
    return (uint32)LoadImagePSX(native_rectangle, (uint32 *)psx_addr(pixels, bytes));
}

uint32 G3Function80045A28(uint32 position, uint32 velocity, uint32 lifetime, uint32 red, uint32 creation_gate, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80045a28, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    if (creation_gate != 0)
        return creation_gate;
    uint32 pool = r_u32(0x800a27fc);
    uint32 storage = G3Function800415CC(pool, frame);
    if (storage == 0)
        return 0x1000;
    w_u32(storage + 0x0c, 0x1000);
    pool = r_u32(0x800a27fc);
    w_u32(storage + 0x1c, 0x1000);
    w_u32(storage + 0x14, 0x1000);
    w_u32(storage + 0x18, 0);
    w_u32(storage + 0x10, 0);
    uint32 effect = G3Function8004161C(pool, frame);
    if (effect == 0)
        return 0x10000;
    uint32 anchor = r_u32(0x800a27fc) + 0x11c10;
    uint32 flags = r_u32(effect + 0x0c);
    w_u32(effect + 0x0c, flags | 0x20000);
    for (uint32 group = 0; group < 3; ++group)
    {
        uint32 input = group == 0 ? velocity : position;
        uint32 output = effect + 0x24 + (group << 3);
        uint32 words[2];
        for (uint32 word = 0; word < 2; ++word)
        {
            uint32 source = input + (word << 2);
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
            uint32 upper = high << ((3 - high_lane) * 8);
            uint32 preserve = low_lane == 0 ? 0 : uint32(0xffffffff) << ((4 - low_lane) * 8);
            words[word] = (upper & preserve) | (low >> (low_lane * 8));
        }
        for (uint32 word = 0; word < 2; ++word)
        {
            uint32 destination = output + (word << 2);
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15 << low_lane) & 15);
        }
    }
    uint32 blue = r_u32(guest_stack + 0x14);
    uint32 green = r_u32(guest_stack + 0x10);
    w_u32(effect + 0x14, storage);
    w_u16(effect + 0x22, lifetime);
    uint32 color = (blue << 16) + (green << 8) + red;
    w_u32(effect + 0x18, color);
    w_u32(effect + 0x10, 0x800459d8);
    return G3Function80045B74(anchor, effect);
}

/* Extracted-file backend initialization boundary */
uint32 G3ReinitializeFileIo80064894(void)
{
    w_u32(0x800ad174, 0);
    w_u32(0x800ad170, 0);
    return 1;
}

uint32 G3Function8006A6C4(uint32 game, uint32 y, uint32 text, uint32 incoming_third_glyph, uint32 guest_stack, G3TextMeasureOperands *operands)
{
    FUNCTION_MARKER(0x8006a6c4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    G3TextMeasureOperands measured;
    measured.first_glyph = y;
    measured.second_glyph = text;
    measured.third_glyph = incoming_third_glyph;
    uint32 width = G3Function80022FD0(text, &measured);
    uint32 centered = (0x200 - width) << 15;
    uint32 x = (uint32)((sint32)centered >> 16);
    uint32 signed_y = (uint32)((sint32)(y << 16) >> 16);
    G3Function80022E30(x, signed_y);
    G3Function80020CBC(text, signed_y, measured.second_glyph, measured.third_glyph, frame, operands);
    return width;
}

uint32 G3Function800234A0(uint32 text, uint32 position, uint32 incoming_second_glyph, uint32 incoming_third_glyph, uint32 guest_stack, G3TextMeasureOperands *operands)
{
    FUNCTION_MARKER(0x800234a0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    uint32 color = 0;
    G3TextMeasureOperands current;
    current.second_glyph = incoming_second_glyph;
    current.third_glyph = incoming_third_glyph;
    G3CopyString8008935C(frame + 0x10, text);
    uint32 line = frame + 0x10;
    uint32 terminator;
    do
    {
        uint32 character = r_u8(line);
        uint32 end = line;
        if (character != 0)
        {
            character = r_u8(end);
            for (;;)
            {
                if (character == 0x0a)
                    break;
                ++end;
                character = r_u8(end);
                if (character == 0)
                    break;
            }
        }
        terminator = r_u8(end);
        w_u8(end, 0);
        uint32 first = r_u8(line);
        uint32 content = line;
        if (first == '#')
        {
            color = r_u8(line + 1);
            content = line + 2;
        }
        uint32 length = G3StringLength8008929C(content);
        uint32 spacing = (length - 1) * 42;
        uint32 centered = (0x200 - spacing) << 15;
        uint32 x = (uint32)((sint32)centered >> 16);
        uint32 y = (uint32)((sint32)(position << 16) >> 16);
        G3Function80022E90(x, y);
        current.first_glyph = y;
        if (color == '1' || color == '2')
        {
            uint32 format = color == '1' ? 0x8009d658 : 0x8009d660;
            G3Function80020CF4(format, content, current.second_glyph, current.third_glyph, frame, &current);
        }
        else if (color == 0)
            G3Function80020CF4(line, y, current.second_glyph, current.third_glyph, frame, &current);
        line = end + 1;
        position += 0x26;
    } while (terminator != 0);
    if (operands != 0)
        *operands = current;
    return 0;
}

uint32 G3Function80069B48(uint32 context, uint32 incoming_v0, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80069b48, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    sint32 index = r_s32(context + 4);
    if (context == 0)
        return incoming_v0;
    if (index < 0)
        return 1;
    uint32 offset = (uint32)index << 4;
    if (index >= 8)
        return 0;
    uint32 entry = context + offset + 8;
    G3Function80069A1C(context, 1);
    G3ResetMenuText(frame);
    (void)r_u32(context + 0xa8);
    (void)r_u32(context + 0xac);
    G3Function8006AC48(context, frame);
    uint32 selected = r_u32(entry + 4);
    uint32 callback = r_u32(entry);
    return G3DispatchMenuPageCallback(callback, context, selected, 512, 0, frame);
}

uint32 G3Function80084A64(uint32 models, uint32 level, uint32 camera, uint32 packets, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80084a64, "SLUS_008.06");
    uint32 frame = guest_stack - 0x58;
    uint32 textures = r_u32(level + 0x44);
    w_u32(0x1f800170, textures);
    uint32 vertices = r_u32(level + 0x30);
    G3TerrainRenderState render;
    render.colors = r_u32(level + 0x34);
    uint32 order_table = r_u32(guest_stack + 0x10);
    render.packet_cursor = r_u32(packets + 4);
    uint32 matrix = r_u32(camera + 0x78);
    render.packet_limit = r_u32(packets + 8);
    uint32 game = r_u32(0x80097048);
    uint32 first = r_u32(matrix);
    uint32 second = r_u32(matrix + 4);
    uint32 third = r_u32(matrix + 8);
    uint32 fourth = r_u32(matrix + 0xc);
    xport_gte_write_control(0, first);
    xport_gte_write_control(1, second);
    xport_gte_write_control(2, third);
    xport_gte_write_control(3, fourth);
    first = r_u32(matrix + 0x10);
    second = r_u32(matrix + 0x14);
    third = r_u32(matrix + 0x18);
    fourth = r_u32(matrix + 0x1c);
    xport_gte_write_control(4, first);
    xport_gte_write_control(5, second);
    xport_gte_write_control(6, third);
    xport_gte_write_control(7, fourth);
    uint32 shift = r_u32(game + 0x2c);
    w_u32(0x1f800168, shift);
    uint32 value = r_u32(game + 0xc);
    w_u32(0x1f8000b0, (value >> (shift & 31)) << 2);
    value = r_u32(game + 0x10);
    w_u32(0x1f8000b4, value);
    value = r_u32(game + 0x30);
    w_u32(0x1f800164, value << 2);
    value = r_u32(game + 8);
    w_u32(0x1f8000b8, (value >> (shift & 31)) << 2);
    value = r_u32(game + 0x20);
    w_u32(0x1f80015c, value);
    value = r_u32(game + 0x24);
    w_u32(0x1f800160, value);
    uint32 count = r_u32(models);
    render.model_entry = models + 4;
    render.model_end = models + (count << 2) + 4;
    render.packet_limit -= 0x16c0;
    while (render.model_entry < render.model_end)
    {
        if ((sint32)(render.packet_cursor - render.packet_limit) < 0)
            G3Function800838DC(&render, vertices, order_table, 0x80084b98, frame);
        render.model_entry += 4;
    }
    return render.packet_cursor;
}

uint32 G3Function80084C68(G3TrianglePacketState *triangle, uint32 flag_mask, uint32 packet_tag, uint32 order_table, uint32 scratch, uint32 return_site, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80084c68, "SLUS_008.06");
    uint32 flags = xport_gte_read_control(31);
    triangle->bound = 0x1000000;
    if ((flags & flag_mask) == 0)
    {
        xport_gte_execute(0x1400006);
        triangle->bound = 0x1000200;
        uint32 first = xport_gte_read_data(12);
        triangle->projection = xport_gte_read_data(13);
        uint32 third = xport_gte_read_data(14);
        uint32 outside = first & triangle->projection & third & 0x80008000;
        first = triangle->bound - first;
        if (outside != 0)
            return 0;
        outside = first & 0x80008000;
        triangle->projection = triangle->bound - triangle->projection;
        outside &= triangle->projection;
        third = triangle->bound - third;
        if ((outside & third) != 0)
            return 0;
        uint32 facing = xport_gte_read_data(24);
        if ((triangle->face_flags & 0x400) != 0)
            facing = 0u - facing;
        if ((sint32)facing >= 0)
            return 1;
        first = xport_gte_read_data(17);
        uint32 second = xport_gte_read_data(18);
        triangle->projection = xport_gte_read_data(19);
        uint32 shift = r_u32(scratch + 0x168);
        if ((sint32)second >= (sint32)first)
            first = second;
        if ((sint32)first >= (sint32)triangle->projection)
            triangle->projection = first;
        triangle->projection >>= shift & 31;
        triangle->depth_offset = triangle->projection << 2;
        uint32 bias = (uint32)r_s8(scratch + 0xcc);
        triangle->projection = r_u32(scratch + 0x164);
        triangle->depth_offset += bias << 4;
        if ((sint32)triangle->depth_offset >= (sint32)triangle->projection || (sint32)triangle->depth_offset < 0)
            return 0;
        triangle->projection = xport_gte_read_data(12);
        triangle->bound = xport_gte_read_data(13);
        third = xport_gte_read_data(14);
        uint32 y0 = (uint32)((sint32)triangle->projection >> 16);
        uint32 y1 = (uint32)((sint32)triangle->bound >> 16);
        uint32 y2 = (uint32)((sint32)third >> 16);
        uint32 difference01 = y0 - y1;
        uint32 difference21 = y2 - y1;
        uint32 difference02 = y0 - y2;
        triangle->edge_extent = difference01 + 0x1ff;
        triangle->projection <<= 16;
        if ((sint32)triangle->edge_extent < 0)
            goto fallback;
        triangle->edge_extent = difference01 - 0x1ff;
        triangle->bound <<= 16;
        if ((sint32)triangle->edge_extent > 0)
            goto fallback;
        triangle->edge_extent = difference02 + 0x1ff;
        third <<= 16;
        if ((sint32)triangle->edge_extent < 0)
            goto fallback;
        triangle->edge_extent = difference02 - 0x1ff;
        triangle->projection = (uint32)((sint32)triangle->projection >> 16);
        if ((sint32)triangle->edge_extent > 0)
            goto fallback;
        triangle->edge_extent = difference21 + 0x1ff;
        triangle->bound = (uint32)((sint32)triangle->bound >> 16);
        if ((sint32)triangle->edge_extent < 0)
            goto fallback;
        triangle->edge_extent = difference21 - 0x1ff;
        third = (uint32)((sint32)third >> 16);
        if ((sint32)triangle->edge_extent > 0)
            goto fallback;
        difference01 = triangle->projection - triangle->bound;
        triangle->edge_extent = difference01 + 0x3ff;
        difference02 = triangle->projection - third;
        if ((sint32)triangle->edge_extent < 0)
            goto fallback;
        triangle->edge_extent = difference01 - 0x3ff;
        difference21 = third - triangle->bound;
        if ((sint32)triangle->edge_extent > 0)
            goto fallback;
        triangle->edge_extent = difference02 + 0x3ff;
        bool rejected = (sint32)triangle->edge_extent < 0;
        triangle->edge_extent = difference02 - 0x3ff;
        if (rejected)
            goto fallback;
        rejected = (sint32)triangle->edge_extent > 0;
        triangle->edge_extent = difference21 + 0x3ff;
        if (rejected)
            goto fallback;
        rejected = (sint32)triangle->edge_extent < 0;
        triangle->edge_extent = difference21 - 0x3ff;
        if (rejected || (sint32)triangle->edge_extent > 0)
            goto fallback;
        uint32 order = order_table + triangle->depth_offset;
        uint32 previous = r_u32(order);
        uint32 link = triangle->packet_cursor & triangle->address_mask;
        w_u32(order, link);
        w_u32(triangle->packet_cursor, previous | packet_tag);
        w_u32(triangle->packet_cursor + 8, xport_gte_read_data(12));
        w_u32(triangle->packet_cursor + 0x14, xport_gte_read_data(13));
        w_u32(triangle->packet_cursor + 0x20, xport_gte_read_data(14));
        triangle->packet_cursor += 0x28;
        return 0;
    }
fallback:
    w_u32(scratch + 0xdc, triangle->packet_cursor);
    w_u32(scratch + 0xe0, triangle->first_green);
    w_u32(scratch + 0xe4, triangle->edge_extent);
    w_u32(scratch + 0xe8, triangle->second_red);
    w_u32(scratch + 0xec, triangle->projection);
    w_u32(scratch + 0xf0, triangle->face_flags);
    w_u32(scratch + 0xf4, triangle->bound);
    w_u32(scratch + 0xf8, triangle->address_mask);
    w_u32(scratch + 0xfc, triangle->depth_offset);
    w_u32(scratch + 0x100, return_site);
    w_u32(scratch + 0x104, triangle->colors);
    uint32 replacement = G3Function8001F3BC(triangle->packet_cursor, order_table, 0, guest_stack);
    triangle->packet_cursor = r_u32(0x1f8000dc);
    triangle->first_green = r_u32(0x1f8000e0);
    triangle->edge_extent = r_u32(0x1f8000e4);
    triangle->second_red = r_u32(0x1f8000e8);
    triangle->projection = r_u32(0x1f8000ec);
    triangle->face_flags = r_u32(0x1f8000f0);
    triangle->bound = r_u32(0x1f8000f4);
    triangle->address_mask = r_u32(0x1f8000f8);
    triangle->depth_offset = r_u32(0x1f8000fc);
    (void)r_u32(0x1f800100);
    triangle->colors = r_u32(0x1f800104);
    if (replacement == 0)
        return 1;
    triangle->packet_cursor = replacement;
    return 0;
}

void G3Function800838DC(G3TerrainRenderState *render, uint32 vertices, uint32 order_table, uint32 return_site, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800838dc, "SLUS_008.06");
    uint32 scratch = 0x1f800000;
    uint32 model, face, face_end, face_word, face_flags, texture;
    uint32 vertex_arg, vertex0, vertex1, packed, z_word;
    uint32 clip_work, projection, screen0, screen1, screen2;
    uint32 uv1, uv2, uv3, flat_color, area, flags, carry;
    uint32 depth_work, depth_bucket, shift, table, interpolation;
    uint32 value, other, first, second, third, link, previous;
    uint32 address_mask, continuation, replacement;
    sint32 count;
    bool quad;
    w_u32(scratch + 0xa4, return_site);
    w_u32(scratch + 0x88, render->model_entry);
    w_u32(scratch + 0x94, render->packet_limit);
    w_u32(scratch + 0xa0, render->model_end);
    model = r_u32(render->model_entry);
    w_u32(scratch + 0xac, 0);
    count = r_s8(model + 0xd);
    face = r_u32(model + 0x10);
    if (count == 0)
        goto finish;
    face_end = face + (uint32)count * 12;
    address_mask = 0xffffff;
    continuation = return_site;
face_start:
    face_word = r_u32(face + 4);
    face_flags = face_word >> 16;
    if ((face_flags & 1) != 0)
        goto next_face;
    vertex_arg = face_word & 0xffff;
    if ((face_flags & 0x200) != 0)
    {
        clip_work = r_u32(scratch + 0xa8);
        texture = r_u32(scratch + 0xd8);
        vertex_arg <<= 3;
        if (clip_work != 0)
            goto project_three;
        vertex_arg += vertices;
        xport_gte_write_data(0, r_u32(vertex_arg));
        xport_gte_write_data(1, r_u32(vertex_arg + 4));
        clip_work = r_u32(scratch + 0xd4);
        w_u32(scratch + 0xd4, texture);
        texture = r_u16(vertex_arg + 6);
        xport_gte_execute(0x180001);
        texture = r_u32(render->colors + (texture << 2));
        w_u32(scratch + 0xd0, clip_work);
        w_u32(scratch + 0xd8, texture);
        goto texture_setup;
    }
    vertex_arg <<= 3;
project_three:
    vertex_arg += vertices;
    packed = r_u32(face);
    xport_gte_write_data(4, r_u32(vertex_arg));
    xport_gte_write_data(5, r_u32(vertex_arg + 4));
    vertex0 = vertices + ((packed & 0xffff) << 3);
    xport_gte_write_data(0, r_u32(vertex0));
    xport_gte_write_data(1, r_u32(vertex0 + 4));
    vertex1 = vertices + ((packed >> 16) << 3);
    z_word = r_u32(vertex1 + 4);
    xport_gte_write_data(2, r_u32(vertex1));
    xport_gte_write_data(3, z_word);
    texture = render->colors + ((z_word >> 16) << 2);
    xport_gte_execute(0x280030);
    value = r_u16(vertex0 + 6);
    texture = r_u32(texture);
    clip_work = r_u32(render->colors + (value << 2));
    value = r_u16(vertex_arg + 6);
    w_u32(scratch + 0xd4, texture);
    texture = r_u32(render->colors + (value << 2));
    w_u32(scratch + 0xd0, clip_work);
    w_u32(scratch + 0xd8, texture);
texture_setup:
    texture = r_u16(face + 0xa);
    clip_work = r_u32(scratch + 0x170);
    texture += clip_work;
    flags = xport_gte_read_control(31);
    clip_work = r_u32(texture);
    uv1 = r_u32(texture + 4);
    xport_gte_execute(0x1400006);
    w_u32(render->packet_cursor + 0xc, clip_work);
    uv2 = r_u32(texture + 8);
    if ((face_flags & 0x2000) != 0)
        goto paired_face;
    quad = false;
    area = xport_gte_read_data(24);
    if ((face_flags & 0x400) != 0)
        area = 0u - area;
    if ((sint32)area >= 0)
    {
        w_u32(scratch + 0xa8, 0);
        goto next_face;
    }
    screen0 = xport_gte_read_data(12);
    screen1 = xport_gte_read_data(13);
    screen2 = xport_gte_read_data(14);
    if ((screen0 & screen1 & screen2 & 0x80008000) != 0)
        goto clear_strip;
    value = (0x1000200 - screen0) & 0x80008000;
    projection = 0x1000200 - screen1;
    depth_work = 0x1000200 - screen2;
    if ((value & projection & depth_work) != 0)
        goto clear_strip;
    value = (uint32)r_s8(scratch + 0xd3);
    carry = r_u32(scratch + 0xac);
    w_u32(scratch + 0xac, 0);
    w_u32(scratch + 0xa8, 0);
    if ((value & 2) != 0)
    {
        vertex_arg = flags >> 12;
        if (carry != 0)
            goto direct_fallback;
        value = vertex_arg & 0x20;
        vertex_arg = 0x500;
        if (value != 0)
            goto direct_fallback;
        depth_bucket = xport_gte_read_data(17);
        depth_work = xport_gte_read_data(18);
        projection = xport_gte_read_data(19);
        w_u32(scratch + 0x2c, depth_bucket);
        w_u32(scratch + 0x34, depth_work);
        w_u32(scratch + 0x3c, projection);
        if ((sint32)(depth_bucket - vertex_arg) < 0 || (sint32)(depth_work - vertex_arg) < 0 || (sint32)(projection - vertex_arg) < 0)
            goto direct_fallback;
    }
    else
    {
        area += 20000;
        if (carry != 0)
            goto subdivide;
        vertex_arg = flags >> 12;
        if ((sint32)area < 0)
            goto subdivide;
        value = vertex_arg & 0x20;
        vertex_arg = 0x500;
        if (value != 0)
            goto subdivide;
        depth_bucket = xport_gte_read_data(17);
        depth_work = xport_gte_read_data(18);
        projection = xport_gte_read_data(19);
        w_u32(scratch + 0x2c, depth_bucket);
        w_u32(scratch + 0x34, depth_work);
        w_u32(scratch + 0x3c, projection);
        if ((sint32)(depth_bucket - vertex_arg) < 0 || (sint32)(depth_work - vertex_arg) < 0 || (sint32)(projection - vertex_arg) < 0)
            goto subdivide;
    }
    shift = r_u32(scratch + 0x168);
    if ((sint32)depth_work >= (sint32)depth_bucket)
        depth_bucket = depth_work;
    if ((sint32)depth_bucket >= (sint32)projection)
        projection = depth_bucket;
    depth_work = (projection >> (shift & 31)) << 2;
    projection = r_u32(scratch + 0x164);
    depth_bucket = depth_work + ((uint32)((sint32)uv2 >> 24) << 4);
    if ((sint32)depth_bucket < 0)
        goto next_face;
    w_u32(render->packet_cursor + 0x18, uv1);
    if ((sint32)depth_bucket >= (sint32)projection)
        goto next_face;
    value = r_u32(scratch + 0xb0);
    w_u32(render->packet_cursor + 0x24, uv2);
    projection = r_u32(scratch + 0xd8);
    if ((sint32)depth_work >= (sint32)value)
        goto fog_triangle;
    second = r_u32(scratch + 0xd4);
    first = r_u32(scratch + 0xd0);
    w_u32(render->packet_cursor + 0x1c, projection);
    w_u32(render->packet_cursor + 0x10, second);
    w_u32(render->packet_cursor + 4, first);
    w_u32(render->packet_cursor + 0x20, screen2);
    table = order_table + depth_bucket;
    previous = r_u32(table);
    link = render->packet_cursor & address_mask;
    w_u32(table, link);
    w_u32(render->packet_cursor, previous | 0x9000000);
    w_u32(render->packet_cursor + 8, screen0);
    w_u32(render->packet_cursor + 0x14, screen1);
    render->packet_cursor += 0x28;
    goto next_face;
paired_face:
    quad = true;
    area = xport_gte_read_data(24);
    if ((face_flags & 0x400) != 0)
        area = 0u - area;
    if ((sint32)area >= 0)
    {
        area -= 30;
        w_u32(scratch + 0xa8, scratch);
        if ((sint32)area >= 0)
            face += 12;
        goto next_face;
    }
    area -= 30;
    face += 12;
    screen0 = xport_gte_read_data(12);
    screen1 = xport_gte_read_data(13);
    screen2 = xport_gte_read_data(14);
    face_word = r_u32(face + 4);
    vertex_arg = vertices + ((face_word & 0xffff) << 3);
    xport_gte_write_data(0, r_u32(vertex_arg));
    xport_gte_write_data(1, r_u32(vertex_arg + 4));
    value = r_u32(scratch + 0xd8);
    clip_work = r_u32(scratch + 0xd4);
    w_u32(scratch + 0xd4, value);
    w_u32(scratch + 0xc4, xport_gte_read_data(8));
    value = r_u16(vertex_arg + 6);
    xport_gte_execute(0x180001);
    value = r_u32(render->colors + (value << 2));
    flat_color = r_u32(scratch + 0xd0);
    w_u32(scratch + 0xd0, clip_work);
    w_u32(scratch + 0xd8, value);
    clip_work = r_u32(scratch + 0x170);
    value = r_u16(face + 0xa);
    uv3 = r_u32(clip_work + value + 8);
    value = xport_gte_read_data(14);
    if ((screen0 & screen1 & screen2 & value & 0x80008000) != 0)
        goto clear_strip;
    other = (0x1000200 - screen0) & 0x80008000;
    projection = 0x1000200 - screen1;
    clip_work = 0x1000200 - screen2;
    other &= projection & clip_work;
    clip_work = 0x1000200 - value;
    if ((other & clip_work) != 0)
        goto clear_strip;
    face -= 12;
    w_u32(scratch + 0xac, 0);
    value = (uint32)r_s8(scratch + 0xd3);
    if ((value & 2) != 0)
    {
        flags = xport_gte_read_control(31);
        if (((flags >> 12) & 0x20) != 0)
            goto restore_fallback_colors;
        value = xport_gte_read_data(16);
        other = xport_gte_read_data(17);
        if ((sint32)(value - 0x500) < 0)
            goto restore_fallback_colors;
        value = xport_gte_read_data(18);
        if ((sint32)(other - 0x500) < 0)
            goto restore_fallback_colors;
        other = xport_gte_read_data(19);
        if ((sint32)(value - 0x500) < 0 || (sint32)(other - 0x500) < 0)
            goto restore_fallback_colors;
    }
    else
    {
        area += 20000;
        if ((sint32)area <= 0)
        {
            w_u32(scratch + 0xac, clip_work);
            goto restore_split_colors;
        }
        flags = xport_gte_read_control(31);
        if ((flags & 0x60000) != 0)
            goto next_face;
    }
    face += 12;
    first = xport_gte_read_data(16);
    depth_bucket = xport_gte_read_data(17);
    depth_work = xport_gte_read_data(18);
    projection = xport_gte_read_data(19);
    if ((sint32)depth_work >= (sint32)depth_bucket)
        depth_bucket = depth_work;
    shift = r_u32(scratch + 0x168);
    if ((sint32)depth_bucket >= (sint32)first)
        first = depth_bucket;
    if ((sint32)first >= (sint32)projection)
        projection = first;
    value = r_u32(scratch + 0x164);
    depth_work = (projection >> (shift & 31)) << 2;
    depth_bucket = depth_work + ((uint32)((sint32)uv2 >> 24) << 4);
    if ((sint32)depth_bucket < 0 || (sint32)depth_bucket >= (sint32)value)
        goto next_face;
    value = r_u32(scratch + 0xb0);
    vertex_arg = flat_color | 0x8000000;
    if ((sint32)depth_work >= (sint32)value)
        goto fog_quad;
    w_u32(render->packet_cursor + 0x18, uv1);
    w_u32(render->packet_cursor + 0x24, uv2);
    w_u32(render->packet_cursor + 0x30, uv3);
    w_u32(render->packet_cursor + 4, vertex_arg);
    projection = r_u32(scratch + 0xd8);
    second = r_u32(scratch + 0xd4);
    first = r_u32(scratch + 0xd0);
    w_u32(render->packet_cursor + 0x28, projection);
    w_u32(render->packet_cursor + 0x1c, second);
    w_u32(render->packet_cursor + 0x10, first);
    w_u32(render->packet_cursor + 0x20, screen2);
    table = order_table + depth_bucket;
    previous = r_u32(table);
    link = render->packet_cursor & address_mask;
    w_u32(table, link);
    w_u32(render->packet_cursor, previous | 0xc000000);
    w_u32(render->packet_cursor + 8, screen0);
    w_u32(render->packet_cursor + 0x14, screen1);
    w_u32(render->packet_cursor + 0x2c, xport_gte_read_data(14));
    render->packet_cursor += 0x34;
    goto next_face;
fog_triangle:
    xport_gte_write_data(6, r_u32(scratch + 0xd8));
    w_u32(scratch + 0xc4, xport_gte_read_data(8));
    table = order_table + depth_bucket;
    xport_gte_execute(0x780010);
    previous = r_u32(table);
    w_u32(table, render->packet_cursor & address_mask);
    w_u32(render->packet_cursor, previous | 0x9000000);
    w_u32(render->packet_cursor + 0x1c, xport_gte_read_data(22));
    shift = r_u32(scratch + 0x168);
    value = xport_gte_read_data(18);
    table = r_u32(scratch + 0xb4);
    interpolation = (uint32)r_s16(table + ((value >> (shift & 31)) << 1));
    xport_gte_write_data(6, r_u32(scratch + 0xd4));
    xport_gte_write_data(8, interpolation);
    w_u32(scratch + 0xc0, interpolation);
    xport_gte_execute(0x780010);
    w_u32(render->packet_cursor + 0x20, screen2);
    w_u32(render->packet_cursor + 0x14, screen1);
    value = xport_gte_read_data(17);
    w_u32(render->packet_cursor + 0x10, xport_gte_read_data(22));
    interpolation = (uint32)r_s16(table + ((value >> (shift & 31)) << 1));
    xport_gte_write_data(6, r_u32(scratch + 0xd0));
    xport_gte_write_data(8, interpolation);
    vertex_arg = 0x32ffffff;
    xport_gte_execute(0x780010);
    w_u32(render->packet_cursor + 8, screen0);
    value = r_u32(scratch + 0xb8);
    depth_work -= value;
    render->packet_cursor += 0x28;
    goto overlay_decision;
fog_quad:
    w_u32(render->packet_cursor + 0x24, uv2);
    w_u32(scratch + 0xc8, xport_gte_read_data(8));
    xport_gte_write_data(6, r_u32(scratch + 0xd8));
    table = order_table + depth_bucket;
    xport_gte_execute(0x780010);
    previous = r_u32(table);
    w_u32(table, render->packet_cursor & address_mask);
    w_u32(render->packet_cursor, previous | 0xc000000);
    w_u32(render->packet_cursor + 0x28, xport_gte_read_data(22));
    xport_gte_write_data(8, r_u32(scratch + 0xc4));
    xport_gte_write_data(6, r_u32(scratch + 0xd4));
    xport_gte_execute(0x780010);
    table = r_u32(scratch + 0xb4);
    w_u32(render->packet_cursor + 8, screen0);
    w_u32(render->packet_cursor + 0x20, screen2);
    w_u32(render->packet_cursor + 0x14, screen1);
    shift = r_u32(scratch + 0x168);
    value = xport_gte_read_data(17);
    w_u32(render->packet_cursor + 0x1c, xport_gte_read_data(22));
    interpolation = (uint32)r_s16(table + ((value >> (shift & 31)) << 1));
    xport_gte_write_data(6, r_u32(scratch + 0xd0));
    xport_gte_write_data(8, interpolation);
    w_u32(scratch + 0xc0, interpolation);
    xport_gte_execute(0x780010);
    w_u32(render->packet_cursor + 0x30, uv3);
    w_u32(render->packet_cursor + 0x2c, xport_gte_read_data(14));
    w_u32(render->packet_cursor + 0x18, uv1);
    value = xport_gte_read_data(16);
    w_u32(render->packet_cursor + 0x10, xport_gte_read_data(22));
    interpolation = (uint32)r_s16(table + ((value >> (shift & 31)) << 1));
    xport_gte_write_data(6, vertex_arg);
    xport_gte_write_data(8, interpolation);
    w_u32(scratch + 0xbc, interpolation);
    xport_gte_execute(0x780010);
    vertex_arg = 0x3affffff;
    value = r_u32(scratch + 0xb8);
    depth_work -= value;
    render->packet_cursor += 0x34;
overlay_decision:
    if ((sint32)depth_work <= 0)
        goto finish_base_color;
    if ((face_flags & 0x4000) != 0)
        goto textured_overlay;
    xport_gte_write_data(6, vertex_arg);
    value = xport_gte_read_data(22) | 0x2000000;
    w_u32(render->packet_cursor - (quad ? 0x30 : 0x24), value);
    xport_gte_execute(0x780010);
    value = ((uv1 >> 16) & 0xff9f) | 0x20;
    w_u16(render->packet_cursor - (quad ? 0x1a : 0xe), value);
    vertex_arg = r_u32(scratch + 0xc0);
    w_u32(render->packet_cursor + 0xc, screen0);
    if (quad)
        w_u32(render->packet_cursor + 0x24, xport_gte_read_data(14));
    w_u32(render->packet_cursor + 8, xport_gte_read_data(22));
    xport_gte_write_data(8, vertex_arg);
    xport_gte_execute(0x780010);
    vertex_arg = r_u32(scratch + 0xc4);
    w_u32(render->packet_cursor + 0x14, screen1);
    w_u32(render->packet_cursor + 4, 0xe1000640);
    w_u32(render->packet_cursor + 0x10, xport_gte_read_data(22));
    xport_gte_write_data(8, vertex_arg);
    link = render->packet_cursor & address_mask;
    table = order_table + depth_bucket;
    xport_gte_execute(0x780010);
    if (quad)
        vertex_arg = r_u32(scratch + 0xc8);
    previous = r_u32(table);
    w_u32(table, link);
    if (quad)
    {
        w_u32(render->packet_cursor + 0x18, xport_gte_read_data(22));
        xport_gte_write_data(8, vertex_arg);
        previous |= 0x9000000;
        xport_gte_execute(0x780010);
        w_u32(render->packet_cursor, previous);
        w_u32(render->packet_cursor + 0x1c, screen2);
        w_u32(render->packet_cursor + 0x20, xport_gte_read_data(22));
        render->packet_cursor += 0x28;
    }
    else
    {
        w_u32(render->packet_cursor, previous | 0x7000000);
        w_u32(render->packet_cursor + 0x1c, screen2);
        w_u32(render->packet_cursor + 0x18, xport_gte_read_data(22));
        render->packet_cursor += 0x20;
    }
    goto next_face;
textured_overlay:
    if (quad)
    {
        value = (uint32)r_s16(face + 6);
        table = r_u32(scratch + 0x160);
    }
    else
    {
        table = r_u32(scratch + 0x160);
        value = (uint32)r_s16(face + 6);
    }
    value = ((value & 0x1e0) >> 5) << 4;
    value = r_u32(table + value);
    vertex_arg = quad ? 0x3e808080 : 0x36808080;
    if ((value & 0x100) == 0)
        goto finish_base_color;
    xport_gte_write_data(6, vertex_arg);
    vertex_arg = xport_gte_read_data(22) | 0x2000000;
    if (quad)
        xport_gte_execute(0x780010);
    w_u32(render->packet_cursor - (quad ? 0x30 : 0x24), vertex_arg);
    if (!quad)
        xport_gte_execute(0x780010);
    value = ((uv1 >> 16) & 0xff9f) | 0x20;
    w_u16(render->packet_cursor - (quad ? 0x1a : 0xe), value);
    if (quad)
        value = (value & 0xff9f) | 0x40;
    w_u32(render->packet_cursor + 4, xport_gte_read_data(22));
    vertex_arg = r_u32(scratch + 0xc0);
    if (!quad)
        value = (value & 0xff9f) | 0x40;
    w_u16(render->packet_cursor + 0x1a, value);
    if (!quad)
        xport_gte_write_data(8, vertex_arg);
    w_u32(render->packet_cursor + 8, screen0);
    w_u32(render->packet_cursor + 0x14, screen1);
    if (quad)
    {
        w_u32(render->packet_cursor + 0x20, screen2);
        xport_gte_write_data(8, vertex_arg);
        vertex_arg = (uint32)r_s16(render->packet_cursor - 0x28);
        screen0 = r_u32(scratch + 0x15c);
        xport_gte_execute(0x780010);
        screen1 = (uint32)r_s16(render->packet_cursor - 0x1c);
        screen0 = (uint32)r_s16(screen0 + 0x16);
        screen2 = r_u32(render->packet_cursor - 0x10);
        w_u16(render->packet_cursor + 0xc, vertex_arg);
        w_u16(render->packet_cursor + 0x18, screen1);
        w_u16(render->packet_cursor + 0xe, screen0);
        w_u32(render->packet_cursor + 0x24, screen2);
        w_u32(render->packet_cursor + 0x10, xport_gte_read_data(22));
        vertex_arg = r_u32(scratch + 0xc4);
        xport_gte_write_data(8, vertex_arg);
        vertex_arg = r_u32(render->packet_cursor - 4);
        xport_gte_execute(0x780010);
        w_u32(render->packet_cursor + 0x30, vertex_arg);
        screen0 = r_u32(render->packet_cursor - 8);
        w_u32(render->packet_cursor + 0x1c, xport_gte_read_data(22));
        vertex_arg = r_u32(scratch + 0xc8);
        w_u32(render->packet_cursor + 0x2c, screen0);
        xport_gte_write_data(8, vertex_arg);
    }
    else
    {
        xport_gte_execute(0x780010);
        w_u32(render->packet_cursor + 0x20, screen2);
        vertex_arg = (uint32)r_s16(render->packet_cursor - 0x1c);
        screen0 = r_u32(scratch + 0x15c);
        screen1 = (uint32)r_s16(render->packet_cursor - 0x10);
        screen0 = (uint32)r_s16(screen0 + 0x16);
        screen2 = r_u32(render->packet_cursor - 4);
        w_u32(render->packet_cursor + 0x10, xport_gte_read_data(22));
        w_u16(render->packet_cursor + 0xc, vertex_arg);
        vertex_arg = r_u32(scratch + 0xc4);
        w_u16(render->packet_cursor + 0x18, screen1);
        xport_gte_write_data(8, vertex_arg);
        w_u16(render->packet_cursor + 0xe, screen0);
        w_u32(render->packet_cursor + 0x24, screen2);
    }
    xport_gte_execute(0x780010);
    link = render->packet_cursor & address_mask;
    table = order_table + depth_bucket;
    previous = r_u32(table);
    w_u32(table, link);
    w_u32(render->packet_cursor, previous | (quad ? 0xc000000 : 0x9000000));
    w_u32(render->packet_cursor + (quad ? 0x28 : 0x1c), xport_gte_read_data(22));
    render->packet_cursor += quad ? 0x34 : 0x28;
    goto next_face;
finish_base_color:
    w_u32(render->packet_cursor - (quad ? 0x30 : 0x24), xport_gte_read_data(22));
    goto next_face;
restore_fallback_colors:
    value = r_u32(scratch + 0xd4);
    clip_work = r_u32(scratch + 0xd0);
    w_u32(scratch + 0xd8, value);
    w_u32(scratch + 0xd4, clip_work);
    w_u32(scratch + 0xd0, flat_color);
    goto direct_fallback;
restore_split_colors:
    value = r_u32(scratch + 0xd4);
    clip_work = r_u32(scratch + 0xd0);
    w_u32(scratch + 0xd8, value);
    w_u32(scratch + 0xd4, clip_work);
    w_u32(scratch + 0xd0, flat_color);
subdivide:
    {
        vertex_arg = (uint32)r_s16(face + 4);
        packed = r_u32(face);
        vertex_arg = vertices + (vertex_arg << 3);
        vertex0 = vertices + ((packed & 0xffff) << 3);
        vertex1 = vertices + ((packed >> 16) << 3);
        uint32 packed0 = r_u32(vertex0);
        uint32 z0 = (uint32)r_s16(vertex0 + 4);
        uint32 packed1 = r_u32(vertex1);
        uint32 z1 = (uint32)r_s16(vertex1 + 4);
        uint32 packed2 = r_u32(vertex_arg);
        uint32 z2 = (uint32)r_s16(vertex_arg + 4);
        w_u32(scratch + 0x30, packed1);
        w_u32(scratch + 0x34, z1);
        w_u32(scratch + 0x28, packed0);
        w_u32(scratch + 0x2c, z0);
        w_u32(scratch + 0x38, packed2);
        w_u32(scratch + 0x3c, z2);
        uint32 x0 = (uint32)(sint32)(sint16)packed0;
        uint32 y0 = (uint32)((sint32)packed0 >> 16);
        uint32 x1 = (uint32)(sint32)(sint16)packed1;
        uint32 y1 = (uint32)((sint32)packed1 >> 16);
        uint32 x2 = (uint32)(sint32)(sint16)packed2;
        uint32 y2 = (uint32)((sint32)packed2 >> 16);
        w_u16(scratch + 0x40, (uint32)((sint32)(x0 + x1) >> 1));
        w_u16(scratch + 0x42, (uint32)((sint32)(y0 + y1) >> 1));
        w_u16(scratch + 0x44, (uint32)((sint32)(z0 + z1) >> 1));
        w_u16(scratch + 0x48, (uint32)((sint32)(x1 + x2) >> 1));
        w_u16(scratch + 0x4a, (uint32)((sint32)(y1 + y2) >> 1));
        w_u16(scratch + 0x4c, (uint32)((sint32)(z1 + z2) >> 1));
        w_u16(scratch + 0x50, (uint32)((sint32)(x2 + x0) >> 1));
        w_u16(scratch + 0x52, (uint32)((sint32)(y2 + y0) >> 1));
        w_u16(scratch + 0x54, (uint32)((sint32)(z2 + z0) >> 1));
        uint32 u0 = r_u32(texture);
        uint32 u1 = r_u32(texture + 4);
        uint32 u2 = r_u32(texture + 8);
        w_u32(scratch + 0x58, u0);
        w_u32(scratch + 0x5c, u1);
        w_u32(scratch + 0x60, u2);
        w_u8(scratch + 0x64, ((u0 & 0xff) + (u1 & 0xff)) >> 1);
        w_u8(scratch + 0x65, (((u0 >> 8) & 0xff) + ((u1 >> 8) & 0xff)) >> 1);
        w_u8(scratch + 0x68, ((u1 & 0xff) + (u2 & 0xff)) >> 1);
        w_u8(scratch + 0x69, (((u1 >> 8) & 0xff) + ((u2 >> 8) & 0xff)) >> 1);
        w_u8(scratch + 0x6c, ((u2 & 0xff) + (u0 & 0xff)) >> 1);
        w_u8(scratch + 0x6d, (((u2 >> 8) & 0xff) + ((u0 >> 8) & 0xff)) >> 1);
        uint32 c0 = r_u32(scratch + 0xd0);
        uint32 c1 = r_u32(scratch + 0xd4);
        uint32 c2 = r_u32(scratch + 0xd8);
        w_u32(scratch + 0x7c, c0);
        w_u32(scratch + 0x80, c1);
        w_u32(scratch + 0x84, c2);
        w_u8(scratch + 0x73, c2 >> 24);
        w_u8(scratch + 0x77, c2 >> 24);
        w_u8(scratch + 0x7b, c2 >> 24);
        for (uint32 pair = 0; pair < 3; ++pair)
        {
            uint32 left = pair == 1 ? c1 : c0;
            uint32 right = pair == 0 ? c1 : c2;
            for (uint32 component = 0; component < 3; ++component)
                w_u8(scratch + 0x70 + pair * 4 + component, (((left >> (component * 8)) & 0xff) + ((right >> (component * 8)) & 0xff)) >> 1);
        }
        face_flags = (uint32)r_s16(face + 6);
        w_u32(scratch + 0xa8, scratch);
        first = (uint32)r_s16(scratch + 0x5e);
        second = (uint32)r_s16(scratch + 0x5a);
        value = (uint32)r_s8(texture + 0xb);
        w_u16(scratch + 0x66, first);
        w_u16(scratch + 0x6a, first);
        w_u16(scratch + 0x6e, second);
        w_u8(scratch + 0xcc, value);
        G3TrianglePacketState triangle;
        triangle.packet_cursor = render->packet_cursor;
        triangle.first_green = (c0 >> 8) & 0xff;
        triangle.edge_extent = (c0 >> 16) & 0xff;
        triangle.second_red = c1 & 0xff;
        triangle.projection = (c1 >> 8) & 0xff;
        triangle.face_flags = face_flags;
        triangle.address_mask = address_mask;
        triangle.depth_offset = vertex_arg;
        triangle.colors = render->colors;
        uint32 flag_mask = 0x66000;
        static const uint32 vertex_offsets[7][3] = {{0x28,0x40,0x50},{0x50,0x48,0x38},{0x40,0x48,0x50},{0x40,0x30,0x48},{0x28,0x30,0x40},{0x28,0x50,0x38},{0x48,0x30,0x38}};
        static const uint32 uv_offsets[7][3] = {{0x58,0x64,0x6c},{0x6c,0x68,0x60},{0x64,0x68,0x6c},{0x64,0x5c,0x68},{0x58,0x5c,0x64},{0x58,0x6c,0x60},{0x68,0x5c,0x60}};
        static const uint32 color_offsets[7][3] = {{0x7c,0x70,0x78},{0x78,0x74,0x84},{0x70,0x74,0x78},{0x70,0x80,0x74},{0x7c,0x80,0x70},{0x7c,0x78,0x84},{0x74,0x80,0x84}};
        static const uint32 continuations[7] = {0x800846c8,0x8008472c,0x8008479c,0x80084800,0x80084874,0x800848dc,0x80084938};
        for (uint32 part = 0; part < 7; ++part)
        {
            for (uint32 vertex = 0; vertex < 3; ++vertex)
            {
                xport_gte_write_data(vertex * 2, r_u32(scratch + vertex_offsets[part][vertex]));
                xport_gte_write_data(vertex * 2 + 1, r_u32(scratch + vertex_offsets[part][vertex] + 4));
            }
            xport_gte_execute(0x280030);
            first = r_u32(scratch + uv_offsets[part][0]);
            second = r_u32(scratch + uv_offsets[part][1]);
            third = r_u32(scratch + uv_offsets[part][2]);
            w_u32(triangle.packet_cursor + 0xc, first);
            w_u32(triangle.packet_cursor + 0x18, second);
            w_u32(triangle.packet_cursor + 0x24, third);
            first = r_u32(scratch + color_offsets[part][0]);
            second = r_u32(scratch + color_offsets[part][1]);
            third = r_u32(scratch + color_offsets[part][2]);
            w_u32(triangle.packet_cursor + 4, first);
            w_u32(triangle.packet_cursor + 0x10, second);
            w_u32(triangle.packet_cursor + 0x1c, third);
            continuation = continuations[part];
            uint32 result = G3Function80084C68(&triangle, flag_mask, 0x9000000, order_table, scratch, continuation, guest_stack);
            render->packet_cursor = triangle.packet_cursor;
            render->colors = triangle.colors;
            address_mask = triangle.address_mask;
            if (part == 1)
            {
                first = (uint32)r_s16(scratch + 0x5a);
                if ((sint32)result > 0)
                    goto next_face;
                second = (uint32)r_s16(scratch + 0x5e);
                w_u16(scratch + 0x66, first);
                w_u16(scratch + 0x6e, second);
            }
            else if (part == 3)
            {
                flag_mask = 0x80000000;
                if ((sint32)result > 0)
                    goto next_face;
                value = (uint32)r_s8(scratch + 0xcc);
                w_u8(scratch + 0xcc, value + 1);
            }
            else if (part < 4 && (sint32)result > 0)
                goto next_face;
            else if (part == 4)
            {
                first = (uint32)r_s16(scratch + 0x5a);
                w_u16(scratch + 0x6a, first);
            }
        }
        goto next_face;
    }
direct_fallback:
    vertex_arg = (uint32)r_s16(face + 4);
    packed = r_u32(face);
    face_flags = (uint32)r_s16(face + 6);
    value = (uint32)r_s8(texture + 0xb);
    vertex_arg = vertices + (vertex_arg << 3);
    vertex0 = vertices + ((packed & 0xffff) << 3);
    w_u32(scratch + 0xa8, scratch);
    vertex1 = vertices + ((packed >> 16) << 3);
    w_u32(scratch + 0xcc, value);
    xport_gte_write_data(0, r_u32(vertex0));
    xport_gte_write_data(1, r_u32(vertex0 + 4));
    xport_gte_write_data(2, r_u32(vertex1));
    xport_gte_write_data(3, r_u32(vertex1 + 4));
    xport_gte_write_data(4, r_u32(vertex_arg));
    xport_gte_write_data(5, r_u32(vertex_arg + 4));
    value = r_u32(texture + 8);
    first = r_u32(texture);
    second = r_u32(texture + 4);
    w_u32(render->packet_cursor + 0x24, value);
    w_u32(render->packet_cursor + 0xc, first);
    w_u32(render->packet_cursor + 0x18, second);
    clip_work = r_u32(scratch + 0xd8);
    screen2 = r_u32(scratch + 0xd4);
    value = r_u32(scratch + 0xd0);
    w_u32(render->packet_cursor + 0x1c, clip_work);
    w_u32(render->packet_cursor + 0x10, screen2);
    w_u32(render->packet_cursor + 4, value);
    w_u32(scratch + 0xdc, render->packet_cursor);
    w_u32(scratch + 0xe0, screen0);
    w_u32(scratch + 0xe4, screen1);
    w_u32(scratch + 0xe8, screen2);
    w_u32(scratch + 0xec, projection);
    w_u32(scratch + 0xf0, face_flags);
    w_u32(scratch + 0xf4, clip_work);
    w_u32(scratch + 0xf8, address_mask);
    w_u32(scratch + 0xfc, vertex_arg);
    w_u32(scratch + 0x100, continuation);
    w_u32(scratch + 0x104, render->colors);
    replacement = G3Function8001F3BC(render->packet_cursor, order_table, 0xffffffff, guest_stack);
    render->packet_cursor = r_u32(0x1f8000dc);
    screen0 = r_u32(0x1f8000e0);
    screen1 = r_u32(0x1f8000e4);
    screen2 = r_u32(0x1f8000e8);
    projection = r_u32(0x1f8000ec);
    face_flags = r_u32(0x1f8000f0);
    clip_work = r_u32(0x1f8000f4);
    address_mask = r_u32(0x1f8000f8);
    vertex_arg = r_u32(0x1f8000fc);
    continuation = r_u32(0x1f800100);
    render->colors = r_u32(0x1f800104);
    if (replacement != 0)
        render->packet_cursor = replacement;
    goto next_face;
clear_strip:
    w_u32(scratch + 0xac, 0);
    w_u32(scratch + 0xa8, 0);
next_face:
    face += 12;
    if (face < face_end)
        goto face_start;
finish:
    (void)r_u32(scratch + 0xa4);
    render->packet_limit = r_u32(scratch + 0x94);
    render->model_entry = r_u32(scratch + 0x88);
    render->model_end = r_u32(scratch + 0xa0);
}

uint32 G3Function8006A824(uint32 game, uint32 y, uint32 text, uint32 selected, uint32 size, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006a824, "SLUS_008.06");
    uint32 frame = guest_stack - 0x90;
    uint32 length = G3StringLength8008929C(text);
    if ((sint32)length > 16)
    {
        uint32 split = text + 16;
        while (text < split)
        {
            uint32 character = r_u8(split);
            if (character == 0x20)
            {
                w_u8(split, 0);
                w_u32(frame + 0x10, size);
                uint32 lines = G3Function8006A824(game, y, text, selected, size, frame);
                ++split;
                w_u32(frame + 0x10, size);
                uint32 remaining = G3Function8006A824(game, y + 0x20, split, selected, size, frame);
                w_u8(split - 1, character);
                return lines + remaining;
            }
            --split;
        }
    }
    G3Function80021A74((uint32)(sint32)(sint16)size);
    uint32 text_end, source_end;
    G3Function8006A774(text, frame + 0x18, &text_end, &source_end);
    length = G3StringLength8008929C(text);
    if (selected != 0)
    {
        uint32 first = (uint32)r_s8(0x800a006c);
        uint32 second = (uint32)r_s8(0x800a006d);
        uint32 third = (uint32)r_s8(0x800a006e);
        w_u8(frame + 0x38, first);
        w_u8(frame + 0x39, second);
        w_u8(frame + 0x3a, third);
        uint32 width = (length - 1) * 42;
        uint32 centered = (uint32)((sint32)(0x200 - width) >> 1);
        G3FillMemory80086094(frame + 0x3b, 0, 33);
        G3CopyString8008935C(frame + 0x3a, text);
        uint32 signed_x = (uint32)(sint32)(sint16)centered;
        uint32 signed_y = (uint32)(sint32)(sint16)y;
        G3Function80022E90(signed_x, signed_y);
        uint32 unused_fill_count = frame + 0x3b == 0 ? 33 : 0;
        G3Function80020CF4(frame + 0x38, signed_y, unused_fill_count, source_end, frame, 0);
    }
    else
    {
        G3Function800209F8(0);
        G3Function800234A0(text, y, text_end, source_end, frame, 0);
        G3Function800209F8(1);
    }
    return 1;
}

void G3Function8002CD84(uint32 parameters, uint32 text, uint32 incoming_second_glyph, uint32 incoming_third_glyph, uint32 guest_stack, uint32 *second_glyph, uint32 *third_glyph)
{
    FUNCTION_MARKER(0x8002cd84, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x20;
    uint32 level = r_u16(game + 0x68b4);
    uint32 second = incoming_second_glyph;
    uint32 third = incoming_third_glyph;
    if (level - 11 < 12)
    {
        level = r_u16(game + 0x68b4);
        uint32 complete = G3Function80039040(frame, &second, &third);
        if (complete != 0)
        {
            game = r_u32(0x80097048);
            level = r_u16(game + 0x68b4);
            uint32 category = G3Function80039010(level);
            uint32 heading = G3Function80065D38(0x47);
            G3TextMeasureOperands operands;
            G3Function80023448(heading, category == 3 ? 0x54 : 0xb4, second, third, frame, &operands);
            second = operands.second_glyph;
            third = operands.third_glyph;
        }
    }
    else
    {
        w_u32(parameters + 0x0c, 100);
        w_u16(parameters + 0x14, 0);
        w_u16(parameters + 0x16, 0);
        G3Function80021A3C(parameters, &second, &third);
    }
    game = r_u32(0x80097048);
    level = r_u16(game + 0x68b4);
    uint32 y;
    if (level - 16 < 4)
        y = (uint32)r_s16(parameters + 0x10);
    else
        y = (uint32)(sint32)(sint16)(r_u16(parameters + 0x10) - 12);
    G3Function80022E30(0x82, y);
    w_u16(0x800a3c78, 1);
    w_u16(0x800a3c7a, 1);
    G3TextMeasureOperands operands;
    G3Function80020CBC(text, y, second, third, frame, &operands);
    if (second_glyph != 0)
        *second_glyph = operands.second_glyph;
    if (third_glyph != 0)
        *third_glyph = operands.third_glyph;
    w_u16(0x800a3c78, 0);
    w_u16(0x800a3c7a, 0);
}

uint32 G3Function8002D0D4(uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002d0d4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x58;
    uint32 game = r_u32(0x80097048);
    uint32 level = (uint32)r_s16(game + 0x68b4);
    uint32 category = G3Function80039010(level);
    uint32 entry = 0x80099708 + level * 28;
    uint32 current_level = r_u16(game + 0x68b4);
    uint32 token = (uint32)r_s16(entry + 6);
    uint32 names = 0x80099716 + current_level * 28;
    uint32 first_text = G3Function80065D38(token);
    token = (uint32)r_s16(entry + 8);
    uint32 second_text = G3Function80065D38(token);
    uint32 length = G3StringLength8008929C(first_text);
    uint32 first_x = (uint32)(sint32)(sint16)(0x168 - length * 21);
    G3Function80022E90(first_x, 0xffffffe2);
    G3Function80021A74(0x21c);
    G3Function80020CF4(first_text, 0x21c, 0, 0, frame, 0);
    uint32 product = ((first_x - 0x100) * 5) << 6;
    uint32 high = (uint32)(((sint64)(sint32)product * (sint32)0xf2b9d649) >> 32);
    first_x = (uint32)((sint32)(high + product) >> 9) - (uint32)((sint32)product >> 31) + 0x100;
    length = G3StringLength8008929C(second_text);
    uint32 second_x = (uint32)(sint32)(sint16)(0x168 - length * 21);
    G3Function80022E90(second_x, 0);
    G3Function80021A74(0x21c);
    G3TextMeasureOperands operands;
    G3Function80020CF4(second_text, 0x21c, 0, 0, frame, &operands);
    product = ((second_x - 0x100) * 5) << 6;
    high = (uint32)(((sint64)(sint32)product * (sint32)0xf2b9d649) >> 32);
    second_x = (uint32)((sint32)(high + product) >> 9) - (uint32)((sint32)product >> 31) + 0x100;
    uint32 left = first_x;
    if ((sint32)(second_x << 16) < (sint32)(first_x << 16))
        left = second_x;
    uint32 heading = G3Function80038F18(level, 0);
    operands.first_glyph = 0;
    operands.second_glyph = high;
    uint32 width = G3Function80022FD0(heading, &operands);
    uint32 heading_x = 0x12c - (uint32)((sint32)width >> 1);
    if ((sint32)(sint16)heading_x < 0x92)
        heading_x = 0x92;
    uint32 signed_heading_x = (uint32)(sint32)(sint16)heading_x;
    G3Function80022E30(signed_heading_x, 0x44);
    G3Function80020CBC(heading, 0x44, operands.second_glyph, operands.third_glyph, frame, 0);
    if ((sint32)signed_heading_x < (sint32)(sint16)left)
        left = heading_x;
    w_u16(0x800a3c76, 10);
    w_u32(frame + 0x10, 26);
    w_u32(frame + 0x14, 12);
    current_level = r_u16(game + 0x68b4);
    uint32 x = (uint32)(sint32)(sint16)left - 0x50;
    token = (uint32)r_s16(0x80099708 + current_level * 28 + 0x0c);
    w_u32(frame + 0x20, 50);
    w_u32(frame + 0x1c, x);
    w_u32(frame + 0x2c, 0x1000);
    w_u32(frame + 0x28, 0x1000);
    w_u16(frame + 0x24, 0);
    w_u16(frame + 0x26, 0);
    w_u32(frame + 0x18, token - 1);
    uint32 second, third;
    G3Function80021A3C(frame + 0x10, &second, &third);
    w_u16(0x800a3c76, 0);
    w_u16(0x8009d8e0, 0);
    uint32 narrowed_category = (uint32)(sint32)(sint16)category;
    if (narrowed_category == 0)
    {
        current_level = r_u16(game + 0x68b4);
        uint32 flags = r_u32(game + 0x6838);
        w_u32(frame + 0x14, (flags & (1u << (current_level & 31))) != 0 ? 0 : 2);
        w_u32(frame + 0x20, 0x78);
        token = (uint32)r_s16(names);
        heading = G3Function80065D38(token);
        G3Function8002CEBC(frame + 0x10, heading, second, third, frame, &second, &third);
    }
    else if (narrowed_category == 1)
    {
        uint32 bits = r_u8(game + level + 0x67f8);
        w_u32(frame + 0x20, 0x78);
        w_u32(frame + 0x14, 2 - ((bits & 1) << 1));
        token = (uint32)r_s16(names);
        heading = G3Function80065D38(token);
        G3Function8002CEBC(frame + 0x10, heading, second, third, frame, &second, &third);
    }
    else if (narrowed_category == 2)
    {
        uint32 bits = r_u8(game + level + 0x67f8);
        w_u32(frame + 0x20, 0x72);
        w_u32(frame + 0x14, 2 - ((bits & 1) << 1));
        token = (uint32)r_s16(names);
        heading = G3Function80065D38(token);
        G3Function8002CEBC(frame + 0x10, heading, second, third, frame, &second, &third);
        bits = r_u8(game + level + 0x67f8);
        w_u32(frame + 0x20, 0xa6);
        w_u32(frame + 0x14, 2 - (bits & 2));
        token = (uint32)r_s16(names + 2);
        heading = G3Function80065D38(token);
        G3Function8002CD84(frame + 0x10, heading, second, third, frame, &second, &third);
    }
    else if (narrowed_category == 3)
    {
        uint32 bits = r_u8(game + level + 0x67f8);
        w_u32(frame + 0x20, 100);
        w_u32(frame + 0x14, 2 - ((bits & 1) << 1));
        token = (uint32)r_s16(names);
        heading = G3Function80065D38(token);
        G3Function8002CEBC(frame + 0x10, heading, second, third, frame, &second, &third);
        bits = r_u8(game + level + 0x67f8);
        w_u32(frame + 0x20, 0x8c);
        w_u32(frame + 0x14, 2 - (bits & 2));
        token = (uint32)r_s16(names + 2);
        heading = G3Function80065D38(token);
        G3Function8002CD84(frame + 0x10, heading, second, third, frame, &second, &third);
        bits = r_u8(game + level + 0x67f8);
        w_u32(frame + 0x20, 0xb4);
        w_u32(frame + 0x14, 2 - ((bits >> 1) & 2));
        token = (uint32)r_s16(names + 4);
        heading = G3Function80065D38(token);
        G3Function8002CD84(frame + 0x10, heading, second, third, frame, &second, &third);
    }
    token = 0x45;
    if (r_s16(names + 2) == 0)
    {
        if (r_s16(names + 4) == 0)
            token = 0xb2;
    }
    heading = G3Function80065D38(token);
    return G3Function80023448(heading, 0xd0, second, third, frame, 0);
}
