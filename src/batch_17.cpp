#include "xport.h"
#include "psx.h"
#include "psx_gte.h"
#include "game_functions.h"

uint32 G3Function8002BEEC(uint32 object, uint32 sphere, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002BEEC, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    G3FillMemory80086094(frame + 0x18, 0, 8);
    for (uint32 component = 0; component < 3; ++component)
    {
        uint32 offset = component << 1;
        uint32 center = r_u16(sphere + offset);
        uint32 position = r_u16(object + 0x58 + offset);
        w_u16(frame + 0x18 + offset, center - position);
    }
    uint32 words[2];
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 source = frame + 0x18 + (word << 2);
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 low_lane = source & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
        words[word] = (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 destination = frame + 0x10 + (word << 2);
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        uint32 low_lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15u << low_lane) & 15);
    }
    uint32 x = (uint32)r_s16(frame + 0x10);
    uint32 xx = x * x;
    uint32 y = (uint32)r_s16(frame + 0x12);
    uint32 yy = y * y;
    uint32 z = (uint32)r_s16(frame + 0x14);
    uint32 zz = z * z;
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 index = (uint32)r_s16(object + 0xa2);
    uint32 models = r_u32(descriptor + 0x0c);
    uint32 model = r_u32(models + (index << 2));
    uint32 radius = r_u16(sphere + 6);
    uint32 model_radius = (uint32)r_s16(model + 0x26);
    radius += model_radius;
    uint32 limit = radius * radius;
    uint32 distance = xx + yy + zz;
    return (sint32)distance < (sint32)limit ? 1 : 0;
}

void G3Function8002BFF0(uint32 context, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002BFF0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 object = r_u32(pool + 4);
    while (object != 0)
    {
        w_u32(object + 0xd0, 0);
        for (uint32 pass = 0; pass < 2; ++pass)
        {
            uint32 light = r_u32(context + (pass == 0 ? 0x3d4 : 0x298));
            while (light != 0)
            {
                if (G3Function8002BEEC(object, light + 0x0c, frame) != 0)
                {
                    sint32 magnitudes[3];
                    for (uint32 component = 0; component < 3; ++component)
                    {
                        uint32 offset = component << 1;
                        uint32 position = r_u16(object + 0x58 + offset);
                        uint32 center = r_u16(light + (pass == 0 ? 0x14 : 0x0c) + offset);
                        uint32 difference = position - center;
                        w_u16(frame + 0x10 + offset, difference);
                        sint32 narrowed = (sint16)difference;
                        uint32 magnitude = narrowed < 0 ? 0u - (uint32)narrowed : (uint32)narrowed;
                        magnitudes[component] = (sint16)magnitude;
                    }
                    for (uint32 i = 1; i < 3; ++i)
                    {
                        for (uint32 j = i; j != 0 && magnitudes[j] < magnitudes[j - 1]; --j)
                        {
                            sint32 temporary = magnitudes[j];
                            magnitudes[j] = magnitudes[j - 1];
                            magnitudes[j - 1] = temporary;
                        }
                    }
                    uint32 weighted = (uint32)magnitudes[2] * 30u + (uint32)magnitudes[1] * 12u + (uint32)magnitudes[0] * 9u;
                    if ((sint32)weighted < 0)
                        weighted += 7;
                    sint32 length = (sint32)weighted >> 3;
                    if (length != 0)
                    {
                        uint32 normalized[3];
                        for (uint32 component = 0; component < 3; ++component)
                        {
                            uint32 numerator = (uint32)r_s16(frame + 0x10 + (component << 1)) << 14;
                            normalized[component] = (uint32)((sint64)(sint32)numerator / length);
                        }
                        for (uint32 component = 0; component < 3; ++component)
                            w_u16(frame + 0x10 + (component << 1), normalized[component]);
                    }
                    uint32 dot = 0;
                    bool accepted = true;
                    if (pass == 0)
                    {
                        uint32 products = 0;
                        for (uint32 component = 0; component < 3; ++component)
                        {
                            uint32 direction = (uint32)r_s16(frame + 0x10 + (component << 1));
                            uint32 axis = (uint32)r_s16(light + 0x1a + (component << 1));
                            products += direction * axis;
                        }
                        uint32 threshold = (uint32)r_s16(light + 0x20);
                        dot = (uint32)((sint32)products >> 12);
                        accepted = (sint32)dot >= (sint32)threshold;
                    }
                    if (accepted)
                    {
                        w_u32(object + 0xd0, light);
                        uint32 direction = r_u16(frame + 0x10);
                        w_u16(object + 0xd4, 0u - direction);
                        direction = r_u16(frame + 0x12);
                        w_u16(object + 0xd6, 0u - direction);
                        direction = r_u16(frame + 0x14);
                        if (pass == 0)
                        {
                            w_u16(object + 0xd8, 0u - direction);
                            uint32 threshold = (uint32)r_s16(light + 0x20);
                            uint32 scale = (uint32)r_s16(light + 0x22);
                            uint32 intensity = (dot - threshold) * scale;
                            w_u16(object + 0xda, (sint32)intensity < 0x1001 ? intensity : 0x1000);
                        }
                        else
                        {
                            w_u16(object + 0xda, 0x1000);
                            w_u16(object + 0xd8, 0u - direction);
                        }
                    }
                }
                light = r_u32(light + 4);
            }
        }
        object = r_u32(object + 8);
    }
}

uint32 G3Function8002DE24(uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002DE24, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 words[2];
    words[0] = r_masked_u32(0x800100f4, 15);
    words[0] = r_masked_u32(0x800100f4, 15);
    words[1] = r_masked_u32(0x800100f8, 15);
    words[1] = r_masked_u32(0x800100f8, 15);
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 destination = frame + 0x10 + (word << 2);
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        uint32 low_lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15u << low_lane) & 15);
    }
    (uint32)DrawSync(0);
    VSync(0);
    return (uint32)ClearImage((PSX_RECT *)psx_addr(frame + 0x10, 8), 0, 0, 0);
}

uint32 G3Function800388C8(uint32 contact, uint32 output, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800388C8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    sint32 type = r_s16(contact + 6);
    if (type == 1)
        return 0;
    if (type == 3)
    {
        uint32 face = r_u32(contact + 0x0c);
        uint32 flags = r_u16(face + 6);
        if ((flags & 0x0f) != 8)
        {
            uint32 level = r_u32(game + 0x48);
            uint32 terrain = r_u32(level);
            uint32 index = (uint32)r_s16(face + 8);
            uint32 normals = r_u32(terrain + 0x40);
            G3Function8005F304(index, normals, output, frame);
            return 1;
        }
        type = r_s16(contact + 6);
    }
    if (type == 2)
    {
        uint32 face = r_u32(contact + 0x0c);
        uint32 flags = r_u16(face + 6);
        if ((flags & 0x0f) != 8)
        {
            uint32 object = r_u32(contact + 0x14);
            uint32 descriptor = r_u32(object + 0x1c);
            uint32 model_index = (uint32)r_s16(object + 0xa2);
            uint32 models = r_u32(descriptor + 0x0c);
            uint32 model = r_u32(models + (model_index << 2));
            uint32 index = (uint32)r_s16(face + 8);
            uint32 normals = r_u32(model + 0x10);
            G3Function8005F304(index, normals, frame + 0x10, frame);
            uint32 matrix = r_u32(object + 0x30);
            ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(frame + 0x10, 8), (SVECTOR *)psx_addr(output, 6));
            return 1;
        }
    }
    uint32 packed_type = r_u32(contact + 4);
    if (packed_type != 0x00050001)
        return 0;
    uint32 object = r_u32(contact + 0x14);
    uint32 matrix = r_u32(object + 0x30);
    ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(contact + 0x18, 8), (SVECTOR *)psx_addr(output, 6));
    return 1;
}

uint32 G3Function80038D80(uint32 position, uint32 target, uint32 threshold, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80038D80, "SLUS_008.06");
    uint32 next = r_u16(target);
    uint32 current = r_u16(position);
    uint32 difference = next - current;
    uint32 x = (uint32)(sint32)(sint16)difference;
    uint32 xx = x * x;
    uint32 frame = guest_stack - 0x10;
    w_u16(frame, difference);
    next = r_u16(target + 2);
    current = r_u16(position + 2);
    difference = next - current;
    uint32 y = (uint32)(sint32)(sint16)difference;
    uint32 yy = y * y;
    w_u16(frame + 2, difference);
    current = r_u16(position + 4);
    next = r_u16(target + 4);
    difference = next - current;
    w_u16(frame + 4, difference);
    uint32 z_term = (uint32)((sint32)(difference << 16) >> 15);
    uint32 metric = xx + yy + z_term;
    bool near = (sint32)threshold >= (sint32)metric;
    for (uint32 copy = near ? 1 : 0; copy < 2; ++copy)
    {
        uint32 source = copy == 0 ? position : target;
        uint32 destination = copy == 0 ? frame + 8 : position;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 low_lane = source & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
        uint32 packed = (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        uint32 z = (uint32)r_s16(source + 4);
        high_address = destination + 3;
        high_lane = high_address & 3;
        low_lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, packed >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, packed << (low_lane * 8), (15u << low_lane) & 15);
        w_u16(destination + 4, z);
    }
    if (near)
        return 1;
    do
    {
        for (uint32 component = 0; component < 3; ++component)
        {
            uint32 offset = component << 1;
            uint32 value = (uint32)r_s16(position + offset);
            uint32 original = (uint32)r_s16(frame + 8 + offset);
            uint32 sum = value + original;
            w_u16(position + offset, (uint32)((sint32)sum >> 1));
        }
        next = r_u16(frame + 8);
        current = r_u16(position);
        difference = next - current;
        x = (uint32)(sint32)(sint16)difference;
        xx = x * x;
        w_u16(frame, difference);
        next = r_u16(frame + 0x0a);
        current = r_u16(position + 2);
        difference = next - current;
        y = (uint32)(sint32)(sint16)difference;
        yy = y * y;
        w_u16(frame + 2, difference);
        current = r_u16(position + 4);
        next = r_u16(frame + 0x0c);
        difference = next - current;
        w_u16(frame + 4, difference);
        z_term = (uint32)((sint32)(difference << 16) >> 15);
        metric = xx + yy + z_term;
    } while ((sint32)threshold < (sint32)metric);
    return 0;
}

uint32 G3Function800314D0(uint32 output, uint32 matrix, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800314D0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 scale0 = G3Function80031474(matrix, 0, frame);
    w_u32(frame + 0x10, scale0);
    uint32 scale1 = G3Function80031474(matrix, 1, frame);
    w_u32(frame + 0x14, scale1);
    uint32 scale2 = G3Function80031474(matrix, 2, frame);
    scale0 = r_u32(frame + 0x10);
    w_u32(frame + 0x18, scale2);
    if (scale0 != 0)
        w_u32(frame + 0x10, (uint32)((sint64)0x01000000 / (sint32)scale0));
    scale1 = r_u32(frame + 0x14);
    if (scale1 != 0)
        w_u32(frame + 0x14, (uint32)((sint64)0x01000000 / (sint32)scale1));
    if (scale2 != 0)
        w_u32(frame + 0x18, (uint32)((sint64)0x01000000 / (sint32)scale2));
    for (uint32 component = 0; component < 3; ++component)
    {
        uint32 offset = 0x14 + (component << 2);
        uint32 value = r_u32(matrix + offset);
        w_u32(output + offset, value);
    }
    for (uint32 word = 0; word < 5; ++word)
    {
        uint32 offset = word << 2;
        uint32 value = r_u32(matrix + offset);
        w_u32(output + offset, value);
    }
    return G3ScaleMatrixL8008BCD8(output, frame + 0x10);
}

uint32 G3Function8002DE88(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002DE88, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    uint32 words[2];
    words[0] = r_masked_u32(0x800100fc, 15);
    words[0] = r_masked_u32(0x800100fc, 15);
    words[1] = r_masked_u32(0x80010100, 15);
    words[1] = r_masked_u32(0x80010100, 15);
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 destination = frame + 0x10 + (word << 2);
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        uint32 low_lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15u << low_lane) & 15);
    }
    G3Function8001E1E8(frame + 0x18, 2, frame + 0x10);
    w_u8(frame + 0x22, 0x10);
    w_u8(frame + 0x21, 0x10);
    w_u8(frame + 0x20, 0x10);
    uint32 buffer = r_u32(game + 0x58);
    w_u16(frame + 0x12, buffer != 0 ? 0x100 : 0);
    MoveImage((PSX_RECT *)psx_addr(frame + 0x10, 8), 0, (sint32)((1u - buffer) << 8));
    G3Function80022E00(frame);
    for (uint32 iteration = 0; iteration < 17; ++iteration)
    {
        buffer ^= 1;
        G3PutDrawEnv80086850(0x800a3cd4 + buffer * 92u);
        uint32 display = r_u32(game + 0x54);
        w_u32(game + 0x54, 1u - display);
        G3DrawPrimitive80086784(frame + 0x18);
        (uint32)DrawSync(0);
        VSync(0);
        display = r_u32(game + 0x54);
        G3PutDispEnv80086910(0x800a3cac + display * 20u);
    }
    uint32 result = G3Function8002DE24(frame);
    w_u32(game + 0x58, buffer);
    return result;
}

uint32 G3Function8002DFD8(uint32 state, uint32 game)
{
    FUNCTION_MARKER(0x8002DFD8, "SLUS_008.06");
    if (r_u32(state + 0x10) != 0)
    {
        w_u32(state + 0x10, 0);
        w_u32(state, 6);
        return 6;
    }
    w_u32(state + 8, 0xffffffff);
    if (r_u16(game + 0x68f8) != 0)
        return 0x1b;
    uint32 level = r_u16(game + 0x68b6);
    bool selected = false;
    if (level == 0x1b && r_s16(game + 0x69dc) != (sint32)level)
    {
        w_u32(state + 8, 1);
        selected = true;
    }
    if (!selected)
    {
        level = r_u16(game + 0x68b6);
        if (level < 0x0b)
        {
            level = r_u16(game + 0x68b6);
            if (r_u8(game + level + 0x67f8) != 0)
            {
                w_u32(state + 8, level + 3);
                selected = true;
            }
        }
    }
    if (!selected)
    {
        level = r_u16(game + 0x68b6);
        if (level - 0x14 < 3)
        {
            level = r_u16(game + 0x68b6);
            uint32 flags = r_u32(game + 0x6838);
            if ((flags & (1u << (level & 31))) != 0)
            {
                w_u32(state + 8, level - 6);
                selected = true;
            }
        }
    }
    if (!selected)
    {
        sint32 previous = r_s16(game + 0x69dc);
        if (previous == 0x13)
            w_u32(state + 8, 0x11);
        else if (previous == 0x11)
            w_u32(state + 8, 0x12);
        else if (previous == 0x10)
            w_u32(state + 8, 0x13);
        else if (previous == 0x1b)
        {
            w_u32(state + 8, 0x16);
            w_u16(game + 0x69dc, previous);
        }
    }
    uint32 selection = r_u32(state + 8);
    if (selection == 0xffffffff)
        return 0xffffffff;
    uint32 previous = r_u32(state + 0x0c);
    if (selection == previous)
        return previous;
    uint32 seen = r_u32(game + 0x69e0);
    if ((seen & (1u << (selection & 31))) != 0)
        return 6;
    selection = r_u32(state + 8);
    w_u32(state, 6);
    if (selection == 0x10)
        return 0x10;
    seen = r_u32(game + 0x69e0);
    w_u32(game + 0x69e0, seen | (1u << (selection & 31)));
    selection = r_u32(state + 8);
    w_u32(state + 0x0c, selection);
    return selection;
}

uint32 G3Function80028004(uint32 output, uint32 input, uint32 index)
{
    FUNCTION_MARKER(0x80028004, "SLUS_008.06");
    uint32 offset = index * 20u;
    uint32 source = input + offset;
    uint32 destination = output + offset;
    uint32 y = r_u32(source + 0x10);
    uint32 x = r_u32(source + 0x0c);
    if ((sint32)y < -96)
        w_u32(source, r_u32(source) | 1);
    else if ((sint32)y >= 97)
        w_u32(source, r_u32(source) | 2);
    if ((sint32)x < -96)
        w_u32(source, r_u32(source) | 4);
    else if ((sint32)x >= 97)
        w_u32(source, r_u32(source) | 8);
    uint32 history = 0x800a0c84 + (index << 2);
    uint32 previous = r_u32(history);
    uint32 buttons = r_u32(source);
    w_u32(source + 4, ~previous & buttons);
    w_u32(history, buttons);
    buttons = r_u32(source);
    if ((buttons & 1) != 0)
        y = 0xffffff80;
    else if ((buttons & 2) != 0)
        y = 0x7f;
    buttons = r_u32(source);
    if ((buttons & 4) != 0)
        x = 0xffffff80;
    else if ((buttons & 8) != 0)
        x = 0x7f;
    uint32 game = r_u32(0x80097048);
    w_u32(destination + 0x0c, x);
    w_u32(destination + 0x10, y);
    uint32 flags = r_u32(game + 0x6834);
    if ((flags & 0x10) != 0)
    {
        uint32 replay = game + offset;
        previous = r_u32(destination);
        buttons = r_u32(replay + 0xc0);
        w_u32(destination + 4, ~previous & buttons);
        buttons = r_u32(replay + 0xc0);
        w_u32(destination, buttons);
        game = r_u32(0x80097048);
        replay = game + offset;
        uint32 axis = r_u32(replay + 0xcc);
        w_u32(source + 0x0c, axis);
        axis = r_u32(replay + 0xd0);
        w_u32(source + 0x10, axis);
        buttons = r_u32(replay + 0xc0);
        if ((buttons & 1) != 0)
            w_u32(source + 0x10, 0xffffff80);
        else if ((buttons & 2) != 0)
            w_u32(source + 0x10, 0x7f);
        game = r_u32(0x80097048);
        buttons = r_u32(game + offset + 0xc0);
        if ((buttons & 4) != 0)
        {
            w_u32(source + 0x0c, 0xffffff80);
            return 0xffffff80;
        }
        if ((buttons & 8) != 0)
        {
            w_u32(source + 0x0c, 0x7f);
            return 0x7f;
        }
        return 0;
    }
    if ((flags & 1) != 0)
    {
        G3FillMemory80086094(game + 0x70, 0, 0x28);
        game = r_u32(0x80097048);
        return G3FillMemory80086094(game + 0x98, 0, 0x28);
    }
    buttons = r_u32(source);
    sint32 mode = r_s16(game + 0x67ec);
    uint32 mapped = buttons & 0xffffff1f;
    if (mode == 1)
    {
        if ((buttons & 0x20) != 0)
            mapped |= 0x40;
        if ((buttons & 0x40) != 0)
            mapped |= 0x80;
        if ((buttons & 0x80) != 0)
            mapped |= 0x20;
    }
    else if (mode == 2)
    {
        if ((buttons & 0x20) != 0)
            mapped |= 0x80;
        if ((buttons & 0x40) != 0)
            mapped |= 0x20;
        if ((buttons & 0x80) != 0)
            mapped |= 0x40;
    }
    else if (mode == 0)
        mapped = buttons;
    else
        mapped = r_u32(source);
    previous = r_u32(destination);
    w_u32(destination, mapped);
    w_u32(destination + 4, ~previous & mapped);
    buttons = r_u32(source);
    y = 0;
    if ((buttons & 1) != 0)
        y = 0xffffff80;
    else if ((buttons & 2) != 0)
        y = 0x7f;
    buttons = r_u32(source);
    x = 0;
    if ((buttons & 4) != 0)
        x = 0xffffff80;
    else if ((buttons & 8) != 0)
        x = 0x7f;
    if (x != 0 || y != 0)
    {
        w_u32(source + 0x0c, x);
        w_u32(source + 0x10, y);
        return source;
    }
    return index << 2;
}

uint32 G3Function800285D0(uint32 output, uint32 current, uint32 previous)
{
    FUNCTION_MARKER(0x800285D0, "SLUS_008.06");
    const uint32 raw_bits[14] = {0x10, 0x80, 0x20, 0x40, 0x1000, 0x8000, 0x2000, 0x4000, 0x400, 0x800, 0x100, 0x200, 8, 1};
    const uint32 game_bits[14] = {1, 4, 8, 2, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x4000, 0x8000};
    uint32 result = previous & 1;
    for (uint32 button = 0; button < 14; ++button)
    {
        uint32 raw = raw_bits[button];
        uint32 mapped = game_bits[button];
        if ((current & raw) == 0)
        {
            uint32 held = r_u32(output);
            w_u32(output, held | mapped);
            if ((previous & raw) != 0)
            {
                uint32 pressed = r_u32(output + 4) | mapped;
                w_u32(output + 4, pressed);
                if (button == 13)
                    result = pressed;
            }
        }
        else if ((previous & raw) == 0)
        {
            uint32 released = r_u32(output + 8) | mapped;
            w_u32(output + 8, released);
            if (button == 13)
                result = released;
        }
    }
    return result;
}

uint32 G3Function80028A78(uint32 buttons)
{
    FUNCTION_MARKER(0x80028A78, "SLUS_008.06");
    uint32 pressed = ~buttons;
    uint32 mapped = ((pressed & 0x0800) << 1) | ((pressed & 0x0400) << 5);
    mapped |= (pressed & 0x0200) << 1;
    mapped |= (pressed & 0x8000) >> 6;
    mapped |= (pressed & 0x1000) >> 1;
    return ~(mapped | (pressed & 0x61f9)) & 0xffff;
}

void G3Function800271C0(void)
{
    FUNCTION_MARKER(0x800271C0, "SLUS_008.06");
    uint32 player = r_u32(0x8009efc4);
    if (player != 0)
    {
        uint32 descriptor = r_u32(player + 0x1c);
        w_u32(player + 0xa4, 0);
        uint32 data = r_u32(descriptor + 0x1c);
        uint32 state = r_u32(data);
        w_u16(state + 0x10, 0);
    }
}

uint32 G3Function80028AC4(uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80028AC4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    w_u32(output, 0);
    w_u32(output + 0x14, 0);
    uint32 game = r_u32(0x80097048);
    w_u32(output + 8, 0);
    w_u32(output + 0x1c, 0);
    w_u16(0x800a0c94, 0);
    w_u16(0x800a0c96, 0);
    w_u32(output + 0x0c, 0);
    w_u32(output + 0x10, 0);
    w_u32(output + 0x20, 0);
    w_u32(output + 0x24, 0);
    if (r_u8(game + 0x68f8) != 0)
    {
        uint32 delay = r_u32(game + 0x67c4);
        if ((sint32)delay > 0)
            return delay;
        if (r_u16(0x8009d82e) == 0)
        {
            uint32 cursor = r_u32(0x8009d834);
            uint32 count = r_u16(cursor);
            uint32 buttons = r_u16(cursor + 2);
            w_u32(0x8009d834, cursor + 4);
            w_u16(0x8009d82e, count);
            w_u16(0x8009d82c, buttons);
            if (count == 0x8000)
            {
                uint32 root = r_u32(0x8009d684);
                uint32 name = r_u32(0x8009d688);
                w_u16(0x8009d82c, 0xffff);
                w_u16(0x8009d82e, 0x7fff);
                G3Function80027644(root, name, game, frame);
                G3Function800271C0();
                uint32 current_game = r_u32(0x80097048);
                w_u8(current_game + 0x68f8, 0);
            }
            count = r_u16(0x8009d82e);
            if (count > 0x8000)
            {
                w_u8(0x800a0c98, 0xff);
                w_u16(0x8009d82e, count & 0x7fff);
            }
            else
                w_u8(0x800a0c98, 0);
        }
        uint32 buttons = r_u16(0x8009d82c);
        game = r_u32(0x80097048);
        w_u16(0x800a0c9a, buttons);
        uint32 count = r_u16(0x8009d82e);
        uint32 elapsed = r_u32(game + 0x14c);
        w_u16(0x8009d82e, count - 1);
        if (elapsed >= 5 && r_u8(0x800a0cbc) == 0 && (~r_u16(0x800a0cbe) & 0x7fff) != 0)
        {
            uint32 root = r_u32(0x8009d684);
            uint32 name = r_u32(0x8009d688);
            G3Function80027644(root, name, game, frame);
            G3Function800271C0();
            uint32 current_game = r_u32(0x80097048);
            w_u8(current_game + 0x68f8, 0);
        }
    }
    else
    {
        for (uint32 controller = 0; controller < 2; ++controller)
        {
            uint32 source = controller == 0 ? 0x800a0cbc : 0x800a0d04;
            uint32 destination = controller == 0 ? 0x800a0c98 : 0x800a0ce0;
            for (uint32 block = 0; block < 2; ++block)
            {
                uint32 word0 = r_u32(source);
                uint32 word1 = r_u32(source + 4);
                uint32 word2 = r_u32(source + 8);
                uint32 word3 = r_u32(source + 0x0c);
                w_u32(destination, word0);
                w_u32(destination + 4, word1);
                w_u32(destination + 8, word2);
                w_u32(destination + 0x0c, word3);
                source += 0x10;
                destination += 0x10;
            }
            uint32 tail = (uint32)r_s16(source);
            w_u16(destination, tail);
        }
    }
    if (r_u8(0x800a0c98) != 0xff)
    {
        uint32 buttons = r_u16(0x800a0c9a);
        w_u16(0x800a0c94, buttons);
        uint32 old_type = r_u8(0x800a0c90);
        w_u32(0x800a058c, 0);
        if (old_type == 0x53)
        {
            buttons = r_u16(0x800a0c94);
            w_u16(0x800a0c94, G3Function80028A78(buttons));
        }
        buttons = r_u16(0x800a0c94);
        uint32 previous = r_u16(0x8009d844);
        G3Function800285D0(output, buttons, previous);
        uint32 type = r_u8(0x800a0c99);
        buttons = r_u16(0x800a0c94);
        w_u8(0x800a0c90, type);
        w_u16(0x8009d844, buttons);
        if (type == 0x73 || type == 0x53)
        {
            uint32 x = r_u8(0x800a0c9e);
            uint32 y = r_u8(0x800a0c9f);
            if (x - 0x4a < 0x6d)
                x = 0x80;
            if (y - 0x4a < 0x6d)
                y = 0x80;
            w_u32(output + 0x0c, x - 0x80);
            w_u32(output + 0x10, y - 0x80);
        }
    }
    else
    {
        game = r_u32(0x80097048);
        sint32 mode = r_s16(game + 0x6786);
        if (mode == 0 || mode == 6)
        {
            uint32 missing = r_u32(0x800a058c);
            w_u32(0x800a058c, missing + 1);
        }
    }
    if (r_u8(0x800a0ce0) == 0xff)
        return 0xff;
    uint32 buttons = r_u16(0x800a0ce2);
    w_u16(0x800a0c96, buttons);
    uint32 old_type = r_u8(0x800a0c91);
    if (old_type == 0x53)
    {
        buttons = r_u16(0x800a0c96);
        w_u16(0x800a0c96, G3Function80028A78(buttons));
    }
    buttons = r_u16(0x800a0c96);
    uint32 previous = r_u16(0x8009d846);
    G3Function800285D0(output + 0x14, buttons, previous);
    uint32 type = r_u8(0x800a0ce1);
    buttons = r_u16(0x800a0c96);
    w_u8(0x800a0c91, type);
    w_u16(0x8009d846, buttons);
    if (type != 0x73 && type != 0x53)
        return 0x73;
    uint32 x = r_u8(0x800a0ce6);
    uint32 y = r_u8(0x800a0ce7);
    if (x - 0x61 < 0x3f)
        x = 0x80;
    if (y - 0x61 < 0x3f)
        y = 0x80;
    w_u32(output + 0x20, x - 0x80);
    w_u32(output + 0x24, y - 0x80);
    return y - 0x80;
}

uint32 G3Function80080610(uint32 object, uint32 game, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80080610, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (event == 1)
    {
        w_u32(object + 0x114, 0);
        return 1;
    }
    if (event == 2)
    {
        G3Function800806A8(object, game, frame);
        return 1;
    }
    if (event == 3)
    {
        if ((r_u32(object + 0x10) & 2) == 0)
            G3Function8008082C(object, game, frame);
        return 1;
    }
    return 0;
}

uint32 G3Function80081C84(uint32 object, uint32 game, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80081C84, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    if (event == 1)
    {
        G3Function80081E90(object);
        return 1;
    }
    if (event == 2)
    {
        uint32 camera = r_u32(game + 0x4c);
        (void)camera;
        if (G3Function8004DACC() == 0)
            G3Function80081FAC(object, game, frame);
        return 1;
    }
    return 0;
}

uint32 G3Function800836B0(uint32 object, uint32 game, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800836B0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (event != 2)
        return 0;
    G3Function800837A8(object, game, frame);
    return 1;
}

uint32 G3Function800837A8(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800837A8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 owner = r_u32(object + 0x128);
    uint32 matrices = r_u32(owner + 0x30);
    if (matrices != 0)
    {
        uint32 index = (uint32)r_s16(object + 0x134);
        uint32 position = r_u16(matrices + (index << 5) + 0x14);
        owner = r_u32(object + 0x128);
        w_u16(object + 0x58, position);
        index = (uint32)r_s16(object + 0x134);
        matrices = r_u32(owner + 0x30);
        position = r_u16(matrices + (index << 5) + 0x18);
        owner = r_u32(object + 0x128);
        w_u16(object + 0x5a, position);
        index = (uint32)r_s16(object + 0x134);
        matrices = r_u32(owner + 0x30);
        position = r_u16(matrices + (index << 5) + 0x1c);
        uint32 height = r_u16(object + 0x130);
        w_u16(object + 0x5c, position + height);
    }
    uint32 rotation = r_u16(object + 0x64);
    w_u16(object + 0x64, rotation + 0x60);
    uint32 counter = r_u16(object + 0x136) - 1;
    w_u16(object + 0x136, counter);
    if ((sint16)counter == -1)
        return G3Function8002A9AC(object, frame);
    uint32 placement = r_u32(object + 0x12c);
    if (placement != 0 && (r_u32(placement + 0x18) & 8) == 0)
        return G3Function8002A9AC(object, frame);
    owner = r_u32(object + 0x128);
    uint32 player = r_u32(game + 0x50);
    if (owner == player)
    {
        uint32 state = r_u16(owner + 0x6e);
        if (state != 0x28)
        {
            uint32 mode = r_u32(owner + 0x154);
            if (mode != 0x40000000)
                return G3Function8002A9AC(object, frame);
        }
    }
    uint32 flags = r_u32(object + 0x10) & 0xfffff7ff;
    w_u32(object + 0x10, flags);
    return flags;
}

uint32 G3Function800806A8(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800806A8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    G3Function8003C8C0(object, object + 0x104, game, frame);
    uint32 resource = G3Function8003AA48(0x800a0324);
    if (resource == 0)
        return 1;
    if (r_s16(object + 0xa2) == 1)
        return 1;
    uint32 models = r_u32(resource + 0x0c);
    uint32 model = r_u32(models);
    uint32 x = r_u16(object + 0x58);
    w_u16(frame + 0x10, x);
    uint32 y = r_u16(object + 0x5a);
    w_u16(frame + 0x12, y);
    uint32 z = r_u16(object + 0x5c);
    uint32 pool = r_u32(0x800a27fc);
    w_u16(frame + 0x14, z + 0x60);
    uint32 effect = G3Function800444E4(frame + 0x10, model, pool, 30, frame);
    if (effect == 0)
        return 0;
    sint32 random = (sint32)G3SdkRandom80027FA8();
    w_u16(frame + 0x18, 15 - random % 30);
    random = (sint32)G3SdkRandom80027FA8();
    w_u16(frame + 0x1a, 15 - random % 30);
    random = (sint32)G3SdkRandom80027FA8();
    x = (uint32)r_s16(frame + 0x18);
    y = (uint32)r_s16(frame + 0x1a);
    z = (uint32)(random % 40 + 10);
    w_u16(frame + 0x1c, z);
    uint32 result = G3Function800449F0(effect, x, y, (uint32)(sint32)(sint16)z, z);
    return G3Function80044A40(effect, 0, 0, 0xfffffffe, result);
}

uint32 G3Function8008082C(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8008082C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 result = r_u32(object + 0x114);
    if (result != 0)
        return result;
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 name = r_u32(descriptor + 0x24);
    uint32 reference = r_u32(0x800a0330);
    uint32 name_word = r_u32(name);
    if (name_word == reference)
    {
        name_word = r_u32(name + 4);
        reference = r_u32(0x800a0334);
        if (name_word == reference)
        {
            w_u32(object + 0x114, 1);
            G3Function800193F8(0x40000, object, frame);
            return G3Function8002A9AC(object, frame);
        }
    }
    result = G3Function8006C6B4(game, 0, frame);
    if (result == 0)
        return 0;
    uint32 placement = r_u32(object + 0x20);
    if (placement != 0)
    {
        uint32 ordinal = G3Function80039DFC(placement);
        uint32 level = r_u16(game + 0x68b4);
        uint32 bits = r_u8(game + level + 0x67f8);
        w_u8(game + level + 0x67f8, bits | (1u << (ordinal & 31)));
    }
    w_u32(object + 0x114, 1);
    uint32 previous_count = r_u32(game + 0x682c);
    uint32 count = previous_count + 1;
    w_u32(game + 0x682c, count);
    G3Function80047EE4(3, count);
    G3Function800193F8(0x40000, object, frame);
    G3Function8002A9AC(object, frame);
    uint32 flags = r_u32(game + 0x6838);
    return G3Function8006C774(game, previous_count, flags, frame);
}

uint32 G3Function8006C6B4(uint32 game, uint32 model, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006C6B4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 resource = G3Function8006C634();
    if (resource == 0)
        return 0;
    uint32 owner = r_u32(game + 0x50);
    uint32 object = G3Function8002A454(owner, resource, frame);
    uint32 callback = G3Function80039B78(0x800a0124);
    if (object == 0)
        return 0;
    if (callback != 0)
    {
        uint32 current_game = r_u32(0x80097048);
        w_u32(object + 0xec, callback);
        G3DispatchStateEvent(callback, object, current_game, 1, 0, frame);
        w_u16(object + 0xa2, model);
        return object;
    }
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x10, ((flags | 0x10) & 0xfffffbff) | 0x20);
    return 0;
}

uint32 G3Function8006C774(uint32 game, uint32 previous_count, uint32 previous_flags, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006C774, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    bool awarded = false;
    uint32 count = r_u32(game + 0x682c);
    if (count != previous_count && count == 5)
    {
        G3Function800277A4(game, 0x800a0130, 1);
        w_u32(0x80096004, 99);
        awarded = true;
    }
    uint32 level = r_u16(game + 0x68b4);
    uint32 result = 1;
    if (level - 20 < 3)
    {
        level = r_u16(game + 0x68b4);
        uint32 bit = 1u << (level & 31);
        result = previous_flags & bit;
        if (result == 0)
        {
            uint32 flags = r_u32(game + 0x6838);
            result = 20;
            if ((flags & bit) != 0)
            {
                if (level == 20)
                {
                    G3Function800277A4(game, 0x800a013c, 1);
                    G3Function800277A4(game, 0x800a0148, 1);
                }
                level = r_u16(game + 0x68b4);
                result = 22;
                if (level == 21)
                {
                    G3Function800277A4(game, 0x800a0154, 1);
                    G3Function800277A4(game, 0x800a0160, 1);
                    level = r_u16(game + 0x68b4);
                    result = 22;
                }
                if (level == 22)
                {
                    G3Function800277A4(game, 0x800a016c, 1);
                    result = G3Function800277A4(game, 0x800a0178, 1);
                }
            }
        }
    }
    if (awarded)
    {
        level = r_u16(game + 0x68b4);
        result = level - 23 < 4;
        if (result != 0)
        {
            uint32 level_data = r_u32(game + 0x48);
            result = r_u32(level_data + 0xc0);
            if (result != 0)
            {
                G3Function80027118(game, frame);
                level_data = r_u32(game + 0x48);
                uint32 sequence = r_u32(level_data + 0xc0);
                uint32 player = r_u32(game + 0x50);
                return G3Function80018DE0(player, sequence + 4, frame);
            }
        }
    }
    return result;
}

uint32 G3Function80081E90(uint32 object)
{
    FUNCTION_MARKER(0x80081E90, "SLUS_008.06");
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 result = r_u32(0x800987e4);
    uint32 properties = r_u32(descriptor + 0x1c);
    uint32 index = 28;
    if (descriptor == result)
    {
        result = 1;
        w_u32(object + 0x11c, 1);
        index = 32;
    }
    else
    {
        result = r_u32(0x800987f4);
        if (descriptor == result)
        {
            result = 2;
            w_u32(object + 0x11c, 2);
            index = 34;
        }
        else
            w_u32(object + 0x11c, 0);
    }
    result = G3Function80085CAC(object + 0x104, properties, 0x800a0428, 100, result);
    result = G3Function80085CAC(object + 0x10c, properties, 0x800a0434, 256, result);
    result = G3Function80085CAC(object + 0x110, properties, 0x800a0440, 8000, result);
    G3Function80085CAC(object + 0x118, properties, 0x800a044c, 2700, result);
    w_u32(object + 0x114, 0);
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x10, flags | 0x800);
    descriptor = r_u32(object + 0x1c);
    w_u32(0x800987d4, descriptor);
    result = r_u32(0x800986e8 + (index << 3) + 4);
    w_u32(0x800987dc, result);
    return result;
}

uint32 G3Function80085CAC(uint32 destination, uint32 properties, uint32 key, uint32 fallback, uint32 incoming_v0)
{
    FUNCTION_MARKER(0x80085CAC, "SLUS_008.06");
    uint32 result = incoming_v0;
    bool found = false;
    if (properties == 0)
    {
        w_u32(destination, fallback);
        return result;
    }
    for (;;)
    {
        uint32 word = r_u32(properties);
        uint32 terminator = r_u32(0x800a04ac);
        result = 0x800a04ac;
        if (word == terminator)
        {
            word = r_u32(properties + 4);
            result = r_u32(0x800a04b0);
            if (word == result)
            {
                if (!found)
                    w_u32(destination, fallback);
                return result;
            }
        }
        if (found)
            return result;
        word = r_u32(properties);
        result = r_u32(key);
        if (word == result)
        {
            word = r_u32(properties + 4);
            result = r_u32(key + 4);
            if (word == result)
            {
                result = r_u32(properties + 8);
                found = true;
                w_u32(destination, result);
            }
        }
        properties += 12;
    }
}

uint32 G3Function80081FAC(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80081FAC, "SLUS_008.06");
    uint32 frame = guest_stack - 0x68;
    w_u32(frame + 0x68, object);
    w_u32(frame + 0x6c, game);
    uint32 player = r_u32(game + 0x50);
    uint32 flags = r_u32(player + 0x10);
    w_u32(frame + 0x30, 0x800a0d28);
    uint32 data = r_u32(player + 0x24);
    if ((flags & 0x100) == 0)
        w_u32(frame + 0x30, game + 0x70);
    object = r_u32(frame + 0x68);
    uint32 state = object + 0x104;
    uint32 timer = r_u32(state + 0x14);
    uint32 seconds = (uint32)((sint32)timer / 30);
    object = r_u32(frame + 0x68);
    flags = r_u32(object + 0x10);
    game = r_u32(frame + 0x6c);
    uint32 camera = r_u32(game + 0x4c);
    w_u32(frame + 0x34, camera);
    if ((flags & 0x20) == 0 && ((sint32)timer >= 300 || (timer & 4) != 0))
        G3Function8003A150(0, seconds, timer, frame);
    uint32 current_game = r_u32(0x80097048);
    flags = r_u32(current_game + 0x6834);
    w_u32(current_game + 0x6834, flags | 0x1000000);
    bool camera_mode = false;
    if (r_u32(player + 0x150) == 6)
    {
        game = r_u32(frame + 0x6c);
        camera = r_u32(game + 0x4c);
        camera_mode = G3Function8004DB2C(camera, frame) != 0;
    }
    if (camera_mode)
        G3Function80047EE4(8, 0);
    else
        G3Function80046B00(8);
    if (r_u32(state + 0x10) == 0)
    {
        bool launch = false;
        if (r_u32(player + 0x150) == 6)
        {
            uint32 input = r_u32(frame + 0x30);
            launch = (r_u32(input + 4) & 0x40) != 0;
        }
        if (!launch)
        {
            if (r_u16(player + 0x6e) == 0x22 && r_s16(player + 0x5e) >= 6 && r_u32(data + 0x17c) == 0)
                launch = true;
            else
                launch = r_s16(data + 0x25e) == 6;
        }
        if (launch)
        {
            uint32 resource = r_u32(0x800987dc);
            if (resource != 0 && r_u32(player + 0x30) != 0)
            {
                object = r_u32(frame + 0x68);
                uint32 projectile = G3Function8002A454(object, resource, frame);
                uint32 matrices = r_u32(player + 0x30);
                w_u32(frame + 0x38, matrices + 0x1c0);
                if (projectile != 0)
                {
                    w_u32(state + 0x10, 19);
                    flags = r_u32(projectile + 0x10);
                    w_u32(projectile + 0x10, flags | 0x480);
                    uint32 mode = r_u32(player + 0x150);
                    uint32 pitch = 0;
                    uint32 yaw;
                    uint32 velocity_z;
                    if (mode - 1 < 3)
                    {
                        matrices = r_u32(player + 0x30);
                        uint32 direction_y = 0u - (uint32)r_s16(matrices + 8);
                        uint32 direction_x = 0u - (uint32)r_s16(matrices + 2);
                        yaw = G3Function8002F0C4(direction_y, direction_x, frame);
                        matrices = r_u32(player + 0x30);
                        uint32 direction_z = 0u - (uint32)r_s16(matrices + 0x0e);
                        uint32 angle = G3Function8002F0C4(0x1000, direction_z, frame);
                        matrices = r_u32(player + 0x30);
                        direction_x = 0u - (uint32)r_s16(matrices + 2);
                        uint32 speed = r_u32(state);
                        w_u32(projectile + 0x104, (uint32)((sint32)(direction_x * speed) >> 12));
                        matrices = r_u32(player + 0x30);
                        direction_y = 0u - (uint32)r_s16(matrices + 8);
                        speed = r_u32(state);
                        w_u32(projectile + 0x108, (uint32)((sint32)(direction_y * speed) >> 12));
                        matrices = r_u32(player + 0x30);
                        direction_z = 0u - (uint32)r_s16(matrices + 0x0e);
                        speed = r_u32(state);
                        velocity_z = direction_z * speed;
                        pitch = 0u - (angle + 0xc00);
                    }
                    else
                    {
                        yaw = r_u16(player + 0x64) - 0x400;
                        if (mode == 6)
                        {
                            camera = r_u32(frame + 0x34);
                            uint32 camera_yaw = r_u16(camera + 0x1c);
                            pitch = r_u16(camera + 0x18);
                            yaw = camera_yaw + 0x400;
                        }
                        else
                        {
                            uint32 player_reference = r_u32(player + 0xa4);
                            uint32 state_reference = r_u32(data);
                            if (player_reference != state_reference)
                            {
                                uint32 offset_yaw;
                                if (mode == 0)
                                {
                                    uint32 offset_pitch = (uint32)r_s16(data + 0x22c);
                                    uint32 triple_pitch = offset_pitch * 3u;
                                    offset_yaw = (uint32)r_s16(data + 0x22e);
                                    pitch = triple_pitch >> 2;
                                    offset_yaw = (uint32)((sint32)(offset_yaw * 3u) >> 2);
                                }
                                else
                                {
                                    offset_yaw = r_u16(data + 0x22e);
                                    pitch = r_u16(data + 0x22c);
                                }
                                yaw = offset_yaw + yaw;
                            }
                        }
                        uint32 signed_pitch = (uint32)(sint32)(sint16)pitch;
                        uint32 cosine_pitch = G3Function8005B470(signed_pitch);
                        uint32 signed_yaw = (uint32)(sint32)(sint16)yaw;
                        uint32 cosine_yaw = G3Function8005B470(signed_yaw);
                        uint32 speed = r_u32(state);
                        uint32 horizontal = (uint32)((sint32)(speed * cosine_pitch) >> 12);
                        w_u32(projectile + 0x104, (uint32)((sint32)(horizontal * cosine_yaw) >> 12));
                        cosine_pitch = G3Function8005B470(signed_pitch);
                        uint32 sine_yaw = G3Function8005B590(signed_yaw);
                        speed = r_u32(state);
                        horizontal = (uint32)((sint32)(speed * cosine_pitch) >> 12);
                        w_u32(projectile + 0x108, (uint32)((sint32)(horizontal * sine_yaw) >> 12));
                        uint32 sine_pitch = G3Function8005B590(signed_pitch);
                        speed = r_u32(state);
                        velocity_z = speed * sine_pitch;
                    }
                    w_u32(projectile + 0x10c, (uint32)((sint32)velocity_z >> 12));
                    mode = r_u32(player + 0x150);
                    if (mode == 0 || mode == 2 || mode == 3 || mode == 1)
                    {
                        uint32 origin = r_u32(frame + 0x38);
                        uint32 coordinate = r_u16(origin + 0x14);
                        w_u16(projectile + 0x58, coordinate);
                        coordinate = r_u16(origin + 0x18);
                        w_u16(projectile + 0x5a, coordinate);
                        coordinate = r_u16(origin + 0x1c);
                        w_u16(projectile + 0x5c, coordinate);
                        if (r_u32(player + 0x150) == 1)
                        {
                            uint32 velocity_x = r_u32(projectile + 0x104);
                            uint32 x = r_u16(projectile + 0x58);
                            uint32 velocity_y = r_u32(projectile + 0x108);
                            w_u16(projectile + 0x58, x + (velocity_x << 1));
                            uint32 y = r_u16(projectile + 0x5a);
                            velocity_z = r_u32(projectile + 0x10c);
                            w_u16(projectile + 0x5a, y + (velocity_y << 1));
                            uint32 z = r_u16(projectile + 0x5c);
                            w_u16(projectile + 0x5c, z + (velocity_z << 1));
                        }
                        else
                        {
                            uint32 signed_yaw = (uint32)(sint32)(sint16)yaw;
                            uint32 cosine = G3Function8005B470(signed_yaw);
                            uint32 offset = r_u32(state + 8);
                            uint32 x = r_u16(projectile + 0x58);
                            uint32 delta = (uint32)((sint32)(offset * cosine) >> 12);
                            w_u16(projectile + 0x58, x + delta);
                            uint32 sine = G3Function8005B590(signed_yaw);
                            offset = r_u32(state + 8);
                            uint32 y = r_u16(projectile + 0x5a);
                            delta = (uint32)((sint32)(offset * sine) >> 12);
                            w_u16(projectile + 0x5a, y + delta);
                        }
                    }
                    else
                    {
                        camera = r_u32(frame + 0x34);
                        G3Function80052310(camera, projectile + 0x58);
                    }
                    w_u32(frame + 0x20, 0x1000);
                    w_u32(frame + 0x18, 0x1000);
                    w_u32(frame + 0x10, 0x1000);
                    w_u32(frame + 0x1c, 0);
                    w_u32(frame + 0x14, 0);
                    G3Function8005B94C((uint32)(sint32)(sint16)pitch, frame + 0x10, frame);
                    uint32 angle = (uint32)(sint32)(sint16)yaw - 0x400;
                    G3Function8005BB6C(angle, frame + 0x10, frame);
                    G3Function800202A4(frame + 0x10, projectile + 0x60, 0x15, frame);
                    uint32 distance = r_u32(state + 0x0c);
                    uint32 speed = r_u32(state);
                    uint32 lifetime;
                    if (speed == 0)
                        lifetime = (sint32)distance < 0 ? 1u : 0xffffffff;
                    else if (distance == 0x80000000 && speed == 0xffffffff)
                        lifetime = 0x80000000;
                    else
                        lifetime = (uint32)((sint32)distance / (sint32)speed);
                    w_u32(projectile + 0x128, lifetime);
                    uint32 launches = r_u32(player + 0x138);
                    w_u32(player + 0x138, launches + 1);
                }
            }
        }
    }
    uint32 player_state = r_u16(player + 0x6e);
    bool decrement_counter = false;
    if (player_state != 0x22)
    {
        uint32 input = r_u32(frame + 0x30);
        if ((r_u32(input + 4) & 0x40) != 0 && r_s16(data + 0x25e) == 0)
        {
            decrement_counter = r_u32(state + 0x10) != 0;
            if (!decrement_counter && player_state != 0x30)
                w_u16(data + 0x260, 6);
        }
    }
    if (!decrement_counter)
        decrement_counter = r_u32(state + 0x10) != 0;
    if (decrement_counter)
    {
        if (r_u32(player + 0x150) == 6)
        {
            uint32 counter = r_u32(state + 0x10);
            w_u32(state + 0x10, counter - 1);
        }
        else if (r_u16(player + 0x6e) == 0x22 && r_s16(player + 0x5e) < 6)
            w_u32(state + 0x10, 0);
        uint32 counter = r_u32(state + 0x10);
        if (counter != 0 && r_u32(player + 0x150) != 6 && r_u16(player + 0x6e) != 0x22)
            w_u32(state + 0x10, counter - 1);
    }
    timer = r_u32(state + 0x14) - 1;
    w_u32(state + 0x14, timer);
    if (timer == 0)
    {
        uint32 private_state = r_u32(player + 0x24);
        (void)private_state;
        object = r_u32(frame + 0x68);
        game = r_u32(frame + 0x6c);
        return G3Function80081E08(object, game, frame);
    }
    return timer;
}

uint32 G3Function80052310(uint32 camera, uint32 output)
{
    FUNCTION_MARKER(0x80052310, "SLUS_008.06");
    if (r_u32(0x80096a88) != 6 || output == 0)
        return 0;
    uint32 coordinate = r_u16(camera);
    w_u16(output, coordinate);
    coordinate = r_u16(camera + 2);
    w_u16(output + 2, coordinate);
    coordinate = r_u16(camera + 4);
    w_u16(output + 4, coordinate);
    return 1;
}

uint32 G3Function80082648(uint32 object, uint32 selector, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80082648, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 resource = r_u32(0x80098794);
    uint32 contact = r_u32(object + 0xb0);
    if (object == 0 || resource == 0)
        return 0;
    uint32 created = G3Function8002A454(object, resource, frame);
    if (created == 0)
        return 0;
    w_u32(created + 0x114, object);
    uint32 placement = r_u32(object + 0x20);
    if (placement != 0 && (r_u32(placement + 0x18) & 0x4000) != 0)
        selector = 0;
    w_u32(created + 0x118, selector);
    uint32 callback = r_u32(object + 0xec);
    w_u32(created + 0x120, callback);
    uint32 coordinate = (uint32)r_s16(object + 0x5c);
    w_u32(created + 0x124, coordinate);
    uint32 scale = (uint32)r_s16(object + 0x6c);
    w_u32(created + 0x128, scale);
    uint32 mask = G3Function80037094(object);
    w_u32(created + 0x12c, mask);
    uint32 created_descriptor = r_u32(created + 0x1c);
    uint32 created_index = (uint32)r_s16(created + 0xa2);
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 created_models = r_u32(created_descriptor + 0x0c);
    uint32 models = r_u32(descriptor + 0x0c);
    uint32 index = (uint32)r_s16(object + 0xa2);
    uint32 created_model = r_u32(created_models + (created_index << 2));
    uint32 model = r_u32(models + (index << 2));
    uint32 created_extent = (uint32)r_s16(created_model + 0x26);
    uint32 extent = (uint32)r_s16(model + 0x26);
    uint32 numerator = created_extent << 12;
    uint32 ratio;
    if (extent == 0)
        ratio = (sint32)numerator < 0 ? 1u : 0xffffffff;
    else
        ratio = (uint32)((sint32)numerator / (sint32)extent);
    if ((sint32)ratio >= 0x1001)
        ratio = 0x1000;
    scale = (uint32)r_s16(object + 0x6c);
    uint32 product = ratio * scale;
    if ((sint32)product < 0)
        product += 0xfff;
    scale = (uint32)((sint32)product >> 12);
    G3Function80039BF4(object, 0, frame);
    uint32 x = (uint32)r_s16(object + 0x58);
    uint32 previous_x = (uint32)r_s16(object + 0x70);
    w_u16(object + 0x68, scale);
    w_u16(object + 0x6a, scale);
    w_u16(object + 0x6c, scale);
    w_u32(created + 0x104, x - previous_x);
    uint32 y = (uint32)r_s16(object + 0x5a);
    uint32 previous_y = (uint32)r_s16(object + 0x72);
    w_u32(created + 0x10c, 100);
    uint32 player = r_u32(0x8009efc4);
    w_u32(created + 0x108, y - previous_y);
    y = r_u16(object + 0x5a);
    uint32 player_y = r_u16(player + 0x5a);
    uint32 player_x = r_u16(player + 0x58);
    x = r_u16(object + 0x58);
    uint32 direction_x = 0;
    uint32 direction_y = 0;
    if (contact != 0)
    {
        direction_x = r_u16(contact + 0x28);
        direction_y = r_u16(contact + 0x2a);
    }
    uint32 delta_y = (uint32)(sint32)(sint16)(y - player_y);
    uint32 delta_x = (uint32)(sint32)(sint16)(player_x - x);
    direction_x = (uint32)(sint32)(sint16)direction_x;
    direction_y = (uint32)(sint32)(sint16)direction_y;
    uint32 dot = delta_y * direction_x + delta_x * direction_y;
    w_u32(created + 0x11c, (sint32)dot > 0 ? 100u : 0xffffff9c);
    return created;
}

uint32 G3Function80084E1C(uint32 object, uint32 game, uint32 event, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80084E1C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 configuration = 0;
    if (object != 0)
        configuration = r_u32(object + 0x28);
    if (configuration == 0)
        return 1;
    if (event == 1)
    {
        G3Function8008504C(object, game, configuration, payload, frame);
        return 1;
    }
    if (event == 2)
    {
        G3Function80085340(object, game, configuration, payload, frame);
        return 1;
    }
    if (event == 4 || event == 7)
    {
        G3Function80084FC8(object, frame);
        return 1;
    }
    if (event == 2021)
        return G3Function800858C8(object, game, configuration, frame);
    return 0;
}

uint32 G3Function80084FC8(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80084FC8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 first = r_u32(object + 0x128);
    uint32 second = r_u32(object + 0x12c);
    uint32 third = r_u32(object + 0x130);
    uint32 result = first;
    if (first != 0)
    {
        uint32 flags = r_u32(first + 0x10);
        w_u32(first + 0x10, flags & 0xfffffbff);
        result = G3Function8002A9AC(first, frame);
    }
    if (second != 0)
    {
        uint32 flags = r_u32(second + 0x10);
        w_u32(second + 0x10, flags & 0xfffffbff);
        result = G3Function8002A9AC(second, frame);
    }
    if (third != 0)
    {
        uint32 flags = r_u32(third + 0x10);
        w_u32(third + 0x10, flags & 0xfffffbff);
        result = G3Function8002A9AC(third, frame);
    }
    return result;
}

uint32 G3Function800858C8(uint32 object, uint32 game, uint32 configuration, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800858C8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 gate = (uint32)r_s16(configuration + 4);
    if (gate != 0 && r_u32(0x800a0760 + (gate << 2)) == 0)
        return 0;
    if (r_u32(object + 0x154) == 3)
        return 0;
    uint32 level = r_u16(game + 0x68b4);
    if (level < 28)
    {
        level = r_u16(game + 0x68b4);
        uint32 ordinal = (uint32)r_s16(configuration);
        uint32 bits = r_u8(game + level + 0x67f8);
        if (((bits >> (ordinal & 31)) & 1) == 0)
        {
            uint32 count = r_u32(game + 0x682c);
            uint32 flags = r_u32(game + 0x6838);
            w_u32(game + 0x682c, count + 1);
            G3Function8006C774(game, count, flags, frame);
        }
        level = r_u16(game + 0x68b4);
        ordinal = (uint32)r_s16(configuration);
        bits = r_u8(game + level + 0x67f8);
        w_u8(game + level + 0x67f8, bits | (1u << (ordinal & 31)));
        ordinal = r_u16(configuration);
        w_u16(game + 0x69ac, ordinal);
    }
    if (r_u32(object + 0x154) != 0)
        return 1;
    uint32 player = r_u32(game + 0x50);
    uint32 linked = r_u32(object + 0x128);
    uint32 data = r_u32(player + 0x24);
    if (linked != 0)
    {
        uint32 pool = r_u32(0x800a27fc);
        uint32 x = (uint32)r_s16(linked + 0x58);
        uint32 z = (uint32)r_s16(linked + 0x5c);
        uint32 y = (uint32)r_s16(linked + 0x5a);
        w_u32(frame + 0x10, z + 0x40);
        player = r_u32(game + 0x50);
        G3Function800440D0(player, pool, x, y, frame);
    }
    w_u32(frame + 0x10, 1500);
    G3Function8003E518(object, 0x802, 0xffffff9c, 127, frame);
    w_u32(object + 0x154, 1);
    w_u16(object + 0x9e, 0);
    player = r_u32(game + 0x50);
    uint32 flags = r_u32(player + 0x128);
    w_u32(player + 0x128, flags | 0x2000000);
    w_u32(data + 0x1bc, object);
    player = r_u32(game + 0x50);
    flags = r_u32(player + 0x134);
    w_u32(player + 0x134, flags & 0xffffffbf);
    player = r_u32(game + 0x50);
    flags = r_u32(player + 0x134);
    w_u32(player + 0x134, flags & 0xffffff7f);
    linked = r_u32(object + 0x12c);
    if (linked != 0)
    {
        flags = r_u32(linked + 0x10);
        w_u32(linked + 0x10, flags & 0xfffffbff);
        G3Function8002A9AC(linked, frame);
    }
    return 1;
}

uint32 G3Function80055F18(uint32 object, uint32 event, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80055F18, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    switch (event)
    {
    case 0:
        G3Function800555AC();
        return 1;
    case 1:
        G3Function80055B40(object, frame);
        return 1;
    case 4:
    {
        uint32 word = r_masked_u32(0x80096b38, 15);
        word = r_masked_u32(0x80096b38, 15);
        uint32 z = (uint32)r_s16(0x80096b3c);
        uint32 high_address = frame + 0x13;
        uint32 high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        uint32 low_address = frame + 0x10;
        uint32 low_lane = low_address & 3;
        w_masked_u32(low_address & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
        w_u16(frame + 0x14, z);
        z = r_u16(frame + 0x14);
        w_u16(frame + 0x14, z - 0x100);
        G3Function80055990(payload, frame + 0x10, frame);
        return 1;
    }
    case 6:
        G3Function80055580();
        return 1;
    case 7:
        return G3Function80055BAC();
    case 17:
    {
        uint32 mode = r_u32(0x80096a8c);
        w_u16(0x800a2474, payload);
        if (mode == 0)
            w_u16(0x800a2478, payload);
        return 1;
    }
    case 18:
    {
        uint32 mode = r_u32(0x80096a8c);
        w_u16(0x800a2476, payload);
        if (mode == 0)
            w_u16(0x800a247a, payload);
        return 1;
    }
    case 21:
    case 22:
        return 1;
    case 24:
        return payload == 0x13;
    default:
        return 0;
    }
}

uint32 G3Function80055580(void)
{
    FUNCTION_MARKER(0x80055580, "SLUS_008.06");
    return G3FillMemory80086094(0x800a245c, 0, 44);
}

uint32 G3Function80055B40(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80055B40, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 word = r_masked_u32(0x80096b38, 15);
    word = r_masked_u32(0x80096b38, 15);
    uint32 z = (uint32)r_s16(0x80096b3c);
    uint32 high_address = frame + 0x2b;
    uint32 high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
    uint32 low_address = frame + 0x28;
    uint32 low_lane = low_address & 3;
    w_masked_u32(low_address & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
    w_u16(frame + 0x2c, z);
    z = r_u16(frame + 0x2c);
    w_u16(frame + 0x2c, z - 0x100);
    G3Function80055990(frame + 0x10, frame + 0x28, frame);
    G3Function8005A248(object, frame + 0x10);
    return G3Function80055A24();
}

uint32 G3Function80055BAC(void)
{
    FUNCTION_MARKER(0x80055BAC, "SLUS_008.06");
    return r_u32(0x80096a8c) != 0;
}

uint32 G3Function80055990(uint32 state, uint32 position, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80055990, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 high_address = position + 3;
    uint32 high_lane = high_address & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
    uint32 low_lane = position & 3;
    uint32 low = r_masked_u32(position & 0xfffffffc, (15u << low_lane) & 15);
    uint32 word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    uint32 z = (uint32)r_s16(position + 4);
    high_address = state + 9;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
    uint32 low_address = state + 6;
    low_lane = low_address & 3;
    w_masked_u32(low_address & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
    w_u16(state + 0x0a, z);
    z = r_u16(state + 0x0a);
    w_u16(state + 0x0a, z + 0x100);
    G3Function80055940(state, state + 0x0c, state + 6, frame);
    uint32 target = r_u16(state + 6);
    uint32 origin = r_u16(state);
    w_u16(frame + 0x10, target - origin);
    target = r_u16(state + 8);
    origin = r_u16(state + 2);
    w_u16(frame + 0x12, target - origin);
    target = r_u16(state + 0x0a);
    origin = r_u16(state + 4);
    w_u16(frame + 0x14, target - origin);
    uint32 distance = G3Function8002FA50(frame + 0x10, frame);
    w_u32(state + 0x14, distance);
    return distance;
}

uint32 G3Function80055A24(void)
{
    FUNCTION_MARKER(0x80055A24, "SLUS_008.06");
    uint32 mode = r_u32(0x80096a8c);
    if ((sint32)mode < 0)
        mode = 0u - mode;
    sint32 divisor = (sint16)mode;
    if (divisor == 0)
    {
        uint32 distance = r_u32(0x80096a10);
        w_u32(0x800a247c, distance);
        return distance;
    }
    sint32 angle_limit = 1800 / divisor;
    uint32 first = r_u16(0x800a2478);
    uint32 second = r_u16(0x800a247a);
    uint32 second_target = r_u16(0x800a2476);
    uint32 distance = r_u32(0x800a247c);
    uint32 distance_target = r_u32(0x80096a10);
    uint32 second_delta = second - second_target;
    uint32 distance_delta = distance - distance_target;
    sint32 distance_limit = 300 / divisor;
    uint32 first_target = r_u16(0x800a2474);
    sint32 first_delta = (sint16)(first - first_target);
    sint32 first_step = -angle_limit;
    if (first_step < first_delta)
        first_step = first_delta;
    if (first_step < angle_limit)
        angle_limit = first_step;
    sint32 second_step = -distance_limit;
    if (second_step < (sint16)second_delta)
        second_step = (sint16)second_delta;
    sint32 selected_second = distance_limit;
    if (second_step < selected_second)
        selected_second = second_step;
    sint32 distance_step = -distance_limit;
    if (distance_step < (sint32)distance_delta)
        distance_step = (sint32)distance_delta;
    if (distance_step < distance_limit)
        distance_limit = distance_step;
    w_u16(0x800a2478, first - (uint32)angle_limit);
    w_u16(0x800a247a, second - (uint32)selected_second);
    uint32 result = distance - (uint32)(sint32)(sint16)distance_limit;
    w_u32(0x800a247c, result);
    return result;
}

uint32 G3Function800555AC(void)
{
    FUNCTION_MARKER(0x800555AC, "SLUS_008.06");
    uint32 source = r_u32(0x80096a84);
    if (source != 0)
    {
        uint32 value = r_u16(source);
        w_u16(0x800a2464, value);
        value = r_u16(source + 2);
        w_u16(0x800a2466, value);
        value = r_u16(source + 4);
        w_u16(0x800a2468, value);
        value = r_u16(source + 8);
        w_u16(0x800a246a, value);
        value = r_u16(source + 0x0a);
        w_u16(0x800a246c, value);
        value = r_u16(source + 0x0c);
        w_u16(0x800a246e, value);
        sint32 first = r_s16(source + 0x14);
        sint32 second = r_s16(source + 4);
        uint32 selected = r_u16(source + 0x14);
        uint32 candidate = r_u16(source + 4);
        if (second < first)
            selected = candidate;
        w_u16(0x800a2462, selected);
        first = r_s16(source + 0x14);
        second = r_s16(source + 4);
        selected = r_u16(source + 0x14);
        candidate = r_u16(source + 4);
        if (first < second)
            selected = candidate;
        w_u16(0x800a2460, selected);
    }
    uint32 first = r_u16(0x800a2474);
    uint32 second = r_u16(0x800a2476);
    uint32 distance = r_u32(0x80096a10);
    w_u16(0x800a2478, first);
    w_u16(0x800a247a, second);
    w_u32(0x800a247c, distance);
    return first;
}

uint32 G3Function80055674(uint32 output, uint32 position, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80055674, "SLUS_008.06");
    uint32 pitch = 0u - (uint32)r_s16(0x800a247a);
    uint32 flags = r_u32(0x80096a94);
    uint32 frame = guest_stack - 0x40;
    if ((flags & 1) != 0)
    {
        uint32 word = r_masked_u32(0x800a2464, 15);
        word = r_masked_u32(0x800a2464, 15);
        uint32 z = (uint32)r_s16(0x800a2468);
        uint32 high_address = output + 3;
        uint32 high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        uint32 low_lane = output & 3;
        w_masked_u32(output & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
        w_u16(output + 4, z);
        return 1;
    }
    uint32 yaw = r_u16(0x800a2478);
    sint32 reverse = r_s16(0x800a2472);
    if (reverse == 1)
        yaw = 0u - (uint32)(sint32)(sint16)yaw;
    uint32 coordinate = r_u16(position);
    uint32 origin = r_u16(0x800a2464);
    w_u16(frame + 0x10, coordinate - origin);
    coordinate = r_u16(position + 2);
    origin = r_u16(0x800a2466);
    w_u16(frame + 0x14, 0);
    w_u16(frame + 0x12, coordinate - origin);
    uint32 length = (uint32)(sint32)(sint16)G3Function8002FD98(frame + 0x10, frame);
    uint32 requested = length - (uint32)(sint32)(sint16)yaw;
    uint32 lower = (uint32)r_s16(0x800a245e);
    if ((sint32)lower < (sint32)requested)
        lower = requested;
    uint32 limited = (uint32)r_s16(0x800a245c);
    if ((sint32)lower < (sint32)limited)
        limited = lower;
    uint32 adjustment = limited - length;
    if ((sint32)adjustment < 0)
        adjustment = 0u - adjustment;
    uint32 signed_pitch = (uint32)(sint32)(sint16)pitch;
    uint32 sine = G3Function8005B590(signed_pitch);
    uint32 cosine = G3Function8005B470(signed_pitch);
    uint32 product = (uint32)(sint32)(sint16)adjustment * sine;
    uint32 height;
    if (cosine == 0)
        height = (sint32)product < 0 ? 1u : 0xffffffff;
    else if (product == 0x80000000 && cosine == 0xffffffff)
        height = 0x80000000;
    else
        height = (uint32)((sint32)product / (sint32)cosine);
    uint32 lower_z = (uint32)r_s16(0x800a2462);
    uint32 target_z = r_u16(position + 4);
    uint32 candidate = target_z + height;
    if ((sint32)lower_z < (sint16)candidate)
        lower_z = candidate;
    uint32 final_z = (uint32)r_s16(0x800a2460);
    if ((sint16)lower_z < (sint32)final_z)
        final_z = lower_z;
    uint32 ratio = 0;
    if (length != 0)
        ratio = (uint32)((sint32)(limited << 12) / (sint32)length);
    uint32 delta_x = (uint32)r_s16(frame + 0x10);
    uint32 product_x = ratio * delta_x;
    uint32 delta_y = (uint32)r_s16(frame + 0x12);
    uint32 product_y = ratio * delta_y;
    uint32 delta_z = (uint32)r_s16(frame + 0x14);
    uint32 product_z = ratio * delta_z;
    w_u16(frame + 0x1a, (uint32)((sint32)product_y >> 12));
    uint32 base_x = r_u16(0x800a2464);
    uint32 projected_x = (uint32)((sint32)product_x >> 12);
    w_u16(frame + 0x18, projected_x);
    uint32 result_x = base_x + projected_x;
    w_u16(frame + 0x1c, (uint32)((sint32)product_z >> 12));
    w_u16(output, result_x);
    uint32 base_y = r_u16(0x800a2466);
    uint32 projected_y = r_u16(frame + 0x1a);
    w_u16(output + 4, final_z);
    uint32 result = base_y + projected_y;
    w_u16(output + 2, result);
    return result;
}

uint32 G3Function800558B4(uint32 output, uint32 state, uint32 position, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800558B4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function8005A1D8(output, position, state, frame);
    uint32 angle = r_u16(output);
    uint32 offset = r_u16(0x800a247c);
    w_u16(output, angle + offset);
    uint32 flags = r_u32(0x80096a94);
    if ((flags & 2) != 0)
    {
        angle = r_u16(0x800a246a);
        w_u16(output, angle);
    }
    flags = r_u32(0x80096a94);
    uint32 result = flags & 4;
    if (result != 0)
    {
        result = r_u16(0x800a246e);
        w_u16(output + 4, result);
    }
    return result;
}

uint32 G3Function80055940(uint32 state, uint32 rotation, uint32 position, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80055940, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    G3Function80055674(state, position, frame);
    return G3Function800558B4(rotation, state, position, frame);
}

uint32 G3Function8004F5E4(uint32 object, uint32 game, uint32 event, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004F5E4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    switch (event)
    {
    case 0:
        G3Function8004EF14(frame);
        return 1;
    case 1:
        G3Function8004EA60(object, game, frame);
        return 1;
    case 2:
        if (r_u32(0x800a22f4) == 0)
        {
            uint32 count = r_u32(0x80096b24);
            if (count != 0)
            {
                --count;
                w_u32(0x80096b24, count);
                w_u32(0x80096b14 + (count << 2), 0);
            }
            G3Function8004EFCC(game, frame);
        }
        return 1;
    case 4:
        G3Function8004F488(payload, frame);
        return 1;
    case 6:
        G3Function8004EA20();
        return 1;
    case 7:
        return 1;
    case 11:
    {
        G3Function80013C54(object, frame);
        uint32 mode = r_u32(0x80096a88);
        uint32 target = r_u32(0x80099650 + (mode << 3) + 4);
        if (target != 0)
        {
            uint32 current_game = r_u32(0x80097048);
            G3DispatchStateEvent(target, object, current_game, 11, payload, frame);
        }
        return 1;
    }
    case 24:
        return payload == 19 || payload == 6;
    default:
        return 0;
    }
}
