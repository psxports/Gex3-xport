#include "psx.h"
#include "xport.h"
#include "game_functions.h"

uint32 G3Function8006D020(uint32 game, uint32 guest_stack, G3TextMeasureOperands *operands)
{
    FUNCTION_MARKER(0x8006D020, "SLUS_008.06");
    uint32 frame = guest_stack - 0x78;
    uint32 level = r_u16(game + 0x68b4);
    G3Function80021A74(604);
    uint32 text = G3Function80065D38(1);
    G3Function800234A0(text, (uint32)-35, 0, 0, frame, 0);
    if (level - 11 < 5)
    {
        uint32 format = G3Function80065D38(4);
        G3SdkSprintfNoValues80088A1C(frame + 0x10, format);
    }
    else if (level - 20 < 3)
    {
        uint32 token = r_s16(0x80099708 + level * 28 + 4);
        uint32 name = G3Function80065D38(token);
        uint32 format = G3Function80065D38(2);
        G3SdkSprintf80088A1C(frame + 0x10, format, name);
    }
    else if (level - 16 < 4)
    {
        uint32 format = G3Function80065D38(5);
        G3SdkSprintfNoValues80088A1C(frame + 0x10, format);
    }
    else
    {
        uint32 count = 0;
        uint32 bits = r_u8(game + level + 0x67f8);
        uint32 category = G3Function80039010(level);
        while (bits != 0)
        {
            if ((bits & 1) != 0)
                ++count;
            bits >>= 1;
        }
        uint32 current_level = r_u16(game + 0x68b4);
        uint32 flags = r_u32(game + 0x6838);
        if ((flags & (1u << (current_level & 31))) != 0)
            ++count;
        uint32 format = G3Function80065D38(3);
        G3SdkSprintfTwoIntegers80088A1C(frame + 0x10, format, count, category + 1, 0);
    }
    return G3Function80023448(frame + 0x10, 86, 0, 0, frame, operands);
}

void G3Function8006D3F8(void)
{
    FUNCTION_MARKER(0x8006D3F8, "SLUS_008.06");
    for (uint32 offset = 8; offset < 32; offset += 4)
    {
        uint32 object = r_u32(0x800a2a6c + offset);
        if (object != 0)
        {
            uint32 angle = r_u16(object + 0x64);
            w_u16(object + 0x64, angle + 64);
        }
    }
}

uint32 G3Function8006D188(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006D188, "SLUS_008.06");
    uint32 frame = guest_stack - 0x58;
    uint32 resource = G3Function8003AA48(0x800a01e8);
    if (resource == 0)
        return 0;
    uint32 models = r_u32(resource + 0x0c);
    uint32 model = r_u32(models);
    uint32 words[2];
    words[0] = r_masked_u32(0x800a01f4, 15);
    words[0] = r_masked_u32(0x800a01f4, 15);
    words[1] = r_masked_u32(0x800a01f8, 15);
    words[1] = r_masked_u32(0x800a01f8, 15);
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 destination = frame + 0x30 + (index << 2);
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        uint32 low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15u << low_lane) & 15);
    }
    uint32 camera = r_u32(game + 0x4c);
    uint32 matrix = r_u32(camera + 0x80);
    SetRotMatrix((MATRIX *)psx_addr(matrix, 20));
    camera = r_u32(game + 0x4c);
    matrix = r_u32(camera + 0x80);
    SetTransMatrix((MATRIX *)psx_addr(matrix, 32));
    RotTrans((SVECTOR *)psx_addr(frame + 0x30, 8), (VECTOR *)psx_addr(frame + 0x20, 16), (sint32 *)psx_addr(frame + 0x38, 4));
    uint32 pool = r_u32(0x800a27fc);
    uint32 x = r_u16(frame + 0x20);
    uint32 y = r_u16(frame + 0x24);
    uint32 z = r_u16(frame + 0x28);
    w_u16(frame + 0x10, x);
    w_u16(frame + 0x12, y);
    w_u16(frame + 0x14, z);
    uint32 effect = G3Function800444E4(frame + 0x10, model, pool, 30, frame);
    if (effect == 0)
        return 0;
    sint32 random = (sint32)G3SdkRandom80027FA8();
    w_u16(frame + 0x18, 15 - random % 30);
    random = (sint32)G3SdkRandom80027FA8();
    w_u16(frame + 0x1a, 15 - random % 30);
    random = (sint32)G3SdkRandom80027FA8();
    uint32 remainder = (uint32)(random % 20);
    x = r_s16(frame + 0x18);
    y = r_s16(frame + 0x1a);
    z = 10u - remainder;
    w_u16(frame + 0x1c, z);
    G3Function800449F0(effect, x, y, (uint32)(sint32)(sint16)z, remainder);
    random = (sint32)G3SdkRandom80027FA8();
    x = 15u - (uint32)(random % 30);
    random = (sint32)G3SdkRandom80027FA8();
    y = 15u - (uint32)(random % 30);
    random = (sint32)G3SdkRandom80027FA8();
    remainder = (uint32)(random % 30);
    return G3Function80044A40(effect, x, y, 15u - remainder, remainder);
}

uint32 G3Function80027644(uint32 root, uint32 name, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80027644, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 result = r_s16(game + 0x6780);
    if (result != 0)
        return result;
    uint32 flags = r_u32(game + 0x6834);
    uint32 object = r_u32(game + 0x50);
    w_u32(game + 0x6834, flags | 1);
    G3Function8006F620(object, game, frame);
    G3Function8003EF1C();
    G3CopyString8008935C(frame + 0x10, root);
    uint32 length = G3StringLength8008929C(frame + 0x10);
    uint32 last = r_u8(frame + 0x10 + length - 1);
    if (last == 48)
    {
        length = G3StringLength8008929C(frame + 0x10);
        w_u8(frame + 0x10 + length - 1, 0);
    }
    uint32 level = G3LoadLevel80027348(frame + 0x10, name, game, frame);
    uint32 mode = r_s16(game + 0x6780);
    if (mode == 0)
    {
        uint32 previous = r_u16(game + 0x68b4);
        w_u16(game + 0x68b4, level);
        w_u16(game + 0x68b6, previous);
    }
    w_u16(game + 0x6782, 1);
    uint32 character = r_u8(name);
    uint32 cursor = name;
    while (character != 0)
    {
        character = r_u8(cursor);
        uint32 converted = G3Lowercase800894BC(character);
        w_u8(cursor, converted);
        ++cursor;
        character = r_u8(cursor);
    }
    uint32 comparison = G3CompareString80085EC4(name, 0x8009d7a4);
    w_u16(game + 0x6780, comparison == 0 ? 2 : 1);
    result = ((level - 23) & 0xffff) < 4;
    if (result != 0)
    {
        uint32 destination = r_u32(0x8009d668);
        G3CopyString8008935C(destination, frame + 0x10);
        destination = r_u32(0x8009d66c);
        return G3CopyString8008935C(destination, name);
    }
    return result;
}

uint32 G3Function8006F620(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006F620, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 data = r_u32(object + 0x24);
    uint32 linked = r_u32(data + 0x17c);
    if (linked != 0)
    {
        G3Function800740B0(linked);
        w_u32(data + 0x17c, 0);
        uint32 count = r_u16(game + 0x681c);
        sint32 limit = r_s16(game + 0x681e);
        uint32 next = count + 1;
        w_u16(game + 0x681c, next);
        bool clamp = limit < (sint32)(sint16)next;
        uint32 raw_limit = r_u16(game + 0x681e);
        if (clamp)
            w_u16(game + 0x681c, raw_limit);
        G3Function8007446C(game);
    }
    uint32 active = r_u32(data + 0x174);
    if (active != 0)
    {
        G3Function800737E4(object, game, frame);
        G3Function8007446C(game);
    }
    return G3Function8006FF1C(object, game, 0, frame);
}

void G3Function8006CC88(uint32 list)
{
    FUNCTION_MARKER(0x8006CC88, "SLUS_008.06");
    uint32 object = r_u32(list + 4);
    while (object != 0)
    {
        uint32 flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags | 0x800);
        object = r_u32(object + 8);
    }
}

uint32 G3Function8006FF1C(uint32 player, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006FF1C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 object = r_u32(game + 0x50);
    uint32 data = r_u32(object + 0x24);
    uint32 old_value = r_s16(data + 0x262);
    uint32 raw_value = r_u16(data + 0x262);
    uint32 changed = 0;
    if (old_value != 0)
        G3Function80043CF8(object, old_value, 0, frame, 0);
    uint32 held = r_u32(data + 0x50);
    if (held != 0)
        w_u32(data + 0x50, 0);
    uint32 flags = r_u32(player + 0x134);
    w_u32(player + 0x134, flags & 0xfffffffd);
    if (state == 0)
        state = 0x800a0260;
    uint32 first = r_u32(state);
    uint32 expected = r_u32(0x800a0260);
    bool default_state = first == expected;
    if (default_state)
    {
        uint32 second = r_u32(state + 4);
        expected = r_u32(0x800a0264);
        default_state = second == expected;
    }
    if (default_state)
    {
        G3Function8006DAF4();
        flags = r_u32(player + 0x128);
        if ((flags & 0x2000) != 0)
        {
            uint32 previous_name = r_u32(0x800a025c);
            uint32 previous = G3Function8003AA48(previous_name);
            G3Function8006F6D8(player, previous, 0, frame);
        }
        uint32 descriptor = r_u32(player + 0x1c);
        uint32 models = r_u32(descriptor + 0x0c);
        uint32 model = r_u32(data + 0x1a0);
        w_u32(models, model);
        model = r_u32(data + 0x1a0);
        uint32 second = r_u32(data + 0x19c);
        uint32 third = r_u32(data + 0x198);
        w_u32(data + 0x1ac, model);
        w_u32(data + 0x1a8, second);
        w_u32(data + 0x1a4, third);
        G3Function8006DB98();
        changed = 1;
        G3Function8006DC34();
        G3Function80043E70(player, 0);
        flags = r_u32(player + 0x128);
        w_u32(player + 0x128, flags & 0xffffdfff);
        w_u32(data + 0x184, 0);
        descriptor = r_u32(player + 0x1c);
        models = r_u32(descriptor + 0x0c);
        model = r_u32(data + 0x1a0);
        w_u32(models + 4, model);
        descriptor = r_u32(player + 0x1c);
        w_u16(descriptor + 8, 2);
        descriptor = r_u32(player + 0x1c);
        w_u32(0x800a025c, 0);
        w_u32(descriptor + 0x40, 0);
    }
    else
    {
        uint32 resource = G3Function8003AA48(state);
        bool costume = resource != 0;
        if (costume)
        {
            uint32 kind = r_u32(resource + 0x1c);
            expected = r_u32(0x800a021c);
            first = r_u32(kind);
            costume = first == expected;
            if (costume)
            {
                uint32 second = r_u32(kind + 4);
                expected = r_u32(0x800a0220);
                costume = second == expected;
            }
        }
        if (costume)
        {
            uint32 name = r_u32(resource + 0x24);
            expected = r_u32(0x800a026c);
            first = r_u32(name);
            bool special = false;
            if (first == expected)
            {
                uint32 second = r_u32(name + 4);
                expected = r_u32(0x800a0270);
                special = second == expected;
            }
            if (!special)
            {
                expected = r_u32(0x800a0278);
                if (first == expected)
                {
                    uint32 second = r_u32(name + 4);
                    expected = r_u32(0x800a027c);
                    special = second == expected;
                }
            }
            if (special)
            {
                flags = r_u32(player + 0x134);
                w_u32(player + 0x134, flags | 2);
            }
            uint32 previous_name = r_u32(0x800a025c);
            bool same = false;
            if (previous_name != 0)
            {
                name = r_u32(resource + 0x24);
                first = r_u32(previous_name);
                uint32 current = r_u32(name);
                if (first == current)
                {
                    uint32 second = r_u32(previous_name + 4);
                    current = r_u32(name + 4);
                    same = second == current;
                }
            }
            if (!same)
            {
                G3Function8006DAF4();
                flags = r_u32(player + 0x128);
                if ((flags & 0x2000) != 0)
                {
                    previous_name = r_u32(0x800a025c);
                    uint32 previous = G3Function8003AA48(previous_name);
                    G3Function8006F6D8(player, previous, 0, frame);
                }
                sint32 count = r_s16(resource + 8);
                if (count != 0)
                {
                    uint32 descriptor = r_u32(player + 0x1c);
                    uint32 source = r_u32(resource + 0x0c);
                    uint32 destination = r_u32(descriptor + 0x0c);
                    uint32 model = r_u32(source);
                    w_u32(destination, model);
                    source = r_u32(resource + 0x0c);
                    model = r_u32(source);
                    w_u32(data + 0x1ac, model);
                    source = r_u32(resource + 0x0c);
                    model = r_u32(source);
                    G3Function80039834(model);
                    count = r_s16(resource + 8);
                    bool second_model = count >= 2;
                    if (!second_model)
                    {
                        uint32 metadata = r_u32(resource + 0x40);
                        if (metadata != 0)
                            second_model = r_s16(metadata) > 0;
                    }
                    if (second_model)
                    {
                        descriptor = r_u32(player + 0x1c);
                        source = r_u32(resource + 0x0c);
                        destination = r_u32(descriptor + 0x0c);
                        model = r_u32(source + 4);
                        w_u32(destination + 4, model);
                        source = r_u32(resource + 0x0c);
                        model = r_u32(source + 4);
                        G3Function80039834(model);
                        source = r_u32(resource + 0x0c);
                        model = r_u32(source + 4);
                        w_u32(data + 0x1a8, model);
                        source = r_u32(resource + 0x0c);
                        model = r_u32(source + 4);
                        w_u32(data + 0x1a4, model);
                    }
                    else
                    {
                        source = r_u32(resource + 0x0c);
                        model = r_u32(source);
                        w_u32(data + 0x1a8, model);
                        source = r_u32(resource + 0x0c);
                        model = r_u32(source);
                        w_u32(data + 0x1a4, model);
                    }
                    uint32 metadata = r_u32(resource + 0x40);
                    if (metadata != 0)
                    {
                        uint32 groups = (uint32)r_s16(metadata);
                        uint32 models_count = (uint32)r_s16(resource + 8);
                        uint32 remaining = (groups + 1) * models_count;
                        while (remaining != 0)
                        {
                            uint32 offset = remaining << 2;
                            --remaining;
                            source = r_u32(resource + 0x0c);
                            descriptor = r_u32(player + 0x1c);
                            destination = r_u32(descriptor + 0x0c);
                            model = r_u32(source + offset - 4);
                            w_u32(destination + offset - 4, model);
                        }
                        descriptor = r_u32(player + 0x1c);
                        uint32 raw_count = r_u16(resource + 8);
                        w_u16(descriptor + 8, raw_count);
                    }
                    descriptor = r_u32(player + 0x1c);
                    metadata = r_u32(resource + 0x40);
                    w_u32(descriptor + 0x40, metadata);
                }
                changed = 1;
                G3Function8006DB98();
                G3Function8006DC34();
                G3Function8006F6D8(player, resource, 1, frame);
                flags = r_u32(player + 0x128);
                name = r_u32(resource + 0x24);
                w_u32(0x800a025c, name);
                w_u32(player + 0x128, flags | 0x2000);
                w_u32(data + 0x184, resource);
            }
        }
    }
    old_value = (uint32)(sint32)(sint16)raw_value;
    if (old_value != 0)
    {
        object = r_u32(game + 0x50);
        G3Function80043CF8(object, 0, old_value, frame, 0);
    }
    object = r_u32(game + 0x50);
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 models = r_u32(descriptor + 0x0c);
    uint32 model = r_u32(models);
    w_u16(model + 0x26, 480);
    object = r_u32(game + 0x50);
    descriptor = r_u32(object + 0x1c);
    models = r_u32(descriptor + 0x0c);
    model = r_u32(models);
    uint32 packed = r_masked_u32(0x800a02c4, 15);
    packed = r_masked_u32(0x800a02c4, 15);
    uint32 last = r_s16(0x800a02c8);
    uint32 destination = model + 0x20;
    uint32 high_address = destination + 3;
    uint32 high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, packed >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
    uint32 low_lane = destination & 3;
    w_masked_u32(destination & 0xfffffffc, packed << (low_lane * 8), (15u << low_lane) & 15);
    w_u16(model + 0x24, last);
    for (uint32 index = 0; index < 3; ++index)
    {
        object = r_u32(game + 0x50);
        descriptor = r_u32(object + 0x1c);
        models = r_u32(descriptor + 0x0c);
        model = r_u32(models);
        uint32 component = r_u16(0x800a02cc + (index << 1));
        w_u16(model + 0x28 + (index << 1), component);
    }
    for (uint32 index = 0; index < 3; ++index)
    {
        object = r_u32(game + 0x50);
        uint32 component = r_u16(0x800a02cc + (index << 1));
        w_u16(object + 0xc8 + (index << 1), component);
    }
    if (changed != 0)
    {
        if (held != 0)
            G3Function8002A9AC(held, frame);
    }
    else if (held != 0)
        w_u32(data + 0x50, held);
    first = r_u32(state);
    expected = r_u32(0x800a0260);
    default_state = first == expected;
    if (default_state)
    {
        uint32 second = r_u32(state + 4);
        expected = r_u32(0x800a0264);
        default_state = second == expected;
    }
    if (!default_state)
        G3CopyString8008935C(game + 0x68c0, state);
    return changed;
}

uint32 G3Function8006DAF4(void)
{
    FUNCTION_MARKER(0x8006DAF4, "SLUS_008.06");
    uint32 player = r_u32(0x8009efc4);
    uint32 data = r_u32(player + 0x24);
    uint32 result = 0;
    for (uint32 component = 0; component < 3; ++component)
    {
        for (uint32 index = 0; index < 3; ++index)
        {
            uint32 destination = r_u32(data + 0xfc + (component << 2) + index * 12);
            result = r_u32(data + 0xf0 + (component << 2));
            w_u32(destination, result);
        }
    }
    return result;
}

uint32 G3Function80043C68(uint32 old_value, uint32 new_value, uint32 model)
{
    FUNCTION_MARKER(0x80043C68, "SLUS_008.06");
    uint32 count = r_s16(model + 4);
    uint32 records = r_u32(model + 0x14);
    uint32 end = records + (((count << 1) + count) << 2);
    uint32 value_address = records + 8;
    while (records < end)
    {
        uint32 old_entry = 0x8009df80 + (old_value << 5);
        uint32 old_end = old_entry + 32;
        uint32 new_entry = 0x8009df80 + (new_value << 5);
        while (old_entry < old_end)
        {
            uint32 value = r_u32(value_address);
            uint32 expected = r_u32(old_entry);
            if (value == expected)
            {
                uint32 replacement = r_u32(new_entry);
                w_u32(value_address, replacement);
                break;
            }
            old_entry += 4;
            new_entry += 4;
        }
        records += 12;
        value_address += 12;
    }
    return 0;
}

void G3Function80043CF8(uint32 object, uint32 old_value, uint32 new_value, uint32 guest_stack, uint32 *return_value)
{
    FUNCTION_MARKER(0x80043CF8, "SLUS_008.06");
    uint32 data = r_u32(object + 0x24);
    uint32 current = r_s16(data + 0x262);
    if (current != old_value)
        return;
    uint32 model = r_u32(data + 0x1a4);
    w_u16(data + 0x262, new_value);
    if (model != 0)
    {
        uint32 result = G3Function80043C68(current, new_value, model);
        if (return_value != 0)
            *return_value = result;
    }
    model = r_u32(data + 0x1a8);
    if (model != 0)
    {
        uint32 result = G3Function80043C68(current, new_value, model);
        if (return_value != 0)
            *return_value = result;
    }
    model = r_u32(data + 0x1ac);
    if (model != 0)
    {
        uint32 result = G3Function80043C68(current, new_value, model);
        if (return_value != 0)
            *return_value = result;
    }
}

void G3Function80039834(uint32 model)
{
    FUNCTION_MARKER(0x80039834, "SLUS_008.06");
    uint32 records = r_u32(model + 0x1c);
    uint32 cursor = records + 4;
    if (records == 0)
        return;
    sint32 count = r_s32(records);
    if (count <= 0)
        return;
    uint32 index = 0;
    do
    {
        w_u32(cursor + 8, 1);
        ++index;
        count = r_s32(records);
        cursor += 12;
    } while ((sint32)(sint16)index < count);
}

uint32 G3Function800775C8(uint32 object, uint32 game)
{
    FUNCTION_MARKER(0x800775C8, "SLUS_008.06");
    uint32 player = r_u32(game + 0x50);
    uint32 flags = r_u32(object + 0x10);
    uint32 data = r_u32(player + 0x24);
    uint32 result = flags & 0xfffffbff;
    w_u32(object + 0x10, result);
    w_u32(data + 0x174, 0);
    w_u16(data + 0x178, 0);
    w_u16(data + 0x122, 0);
    w_u16(data + 0x1d6, 0);
    return result;
}

uint32 G3Function80081E08(uint32 linked, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80081E08, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 state = r_u32(linked + 0x11c);
    uint32 selection = state == 1 ? 4 : state == 2 ? 1 : 3;
    G3Function80046B00(8);
    G3Function800775C8(linked, game);
    uint32 player = r_u32(0x8009efc4);
    G3Function80043CF8(player, selection, 0, frame, 0);
    return G3Function8002A9AC(linked, frame);
}

uint32 G3Function8006F6D8(uint32 object, uint32 resource, uint32 enabled, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006F6D8, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x38;
    uint32 mode = 10;
    w_u32(frame + 0x40, enabled);
    uint32 player = r_u32(game + 0x50);
    uint32 data = r_u32(player + 0x24);
    uint32 count = 0;
    if (resource == 0)
        return count;
    uint32 commands = r_u32(resource + 0x1c);
    if (commands == 0)
        return count;
    uint32 first = r_u32(commands);
    uint32 expected = r_u32(0x800a021c);
    if (first != expected)
        return count;
    uint32 second = r_u32(commands + 4);
    expected = r_u32(0x800a0220);
    if (second != expected)
        return count;
    uint32 use_first = r_u32(commands + 8);
    G3Function80043E70(object, use_first);
    uint32 cursor = commands + 12;
    first = r_u32(commands + 12);
    expected = r_u32(0x800a0224);
    if (first == expected)
        return count;
    first = r_u32(cursor);
    const uint32 modes[10] = { 0, 6, 1, 2, 3, 4, 5, 7, 8, 9 };
    for (;;)
    {
        for (uint32 index = 0; index < 10; ++index)
        {
            expected = r_u32(0x800a0228 + (index << 2));
            if (first == expected)
            {
                uint32 selected = modes[index];
                if (mode != selected)
                    cursor += 4;
                mode = selected;
                break;
            }
        }
        uint32 value = r_u32(cursor);
        bool marker = false;
        for (uint32 index = 0; index < 10; ++index)
        {
            expected = r_u32(0x800a0228 + (index << 2));
            if (value == expected)
            {
                mode = index == 4 ? 10 : modes[index];
                marker = true;
                break;
            }
        }
        if (!marker)
        {
            uint32 key = r_u32(cursor);
            expected = r_u32(0x800a0224);
            if (key == expected)
                mode = 3;
            else
            {
                uint32 action = r_u32(frame + 0x40);
                if (action == 1)
                {
                    if (mode == 0)
                    {
                        uint32 descriptor = r_u32(object + 0x1c);
                        uint32 table = r_u32(descriptor + 0x10);
                        uint32 replacement = r_u32(cursor + 4);
                        uint32 previous = r_u32(table + (key << 2));
                        ++count;
                        w_u32(cursor + 8, previous);
                        descriptor = r_u32(object + 0x1c);
                        uint32 index = r_u32(cursor);
                        table = r_u32(descriptor + 0x10);
                        w_u32(table + (index << 2), replacement);
                        w_u16(object + 0x86, 0xffff);
                    }
                    if (mode == 6)
                    {
                        uint32 descriptor = r_u32(object + 0x1c);
                        uint32 index = r_u32(cursor);
                        uint32 replacement = r_u32(cursor + 4);
                        uint32 table = r_u32(descriptor + 0x2c);
                        uint32 previous = r_u32(table + (index << 2));
                        ++count;
                        w_u32(cursor + 8, previous);
                        descriptor = r_u32(object + 0x1c);
                        index = r_u32(cursor);
                        table = r_u32(descriptor + 0x2c);
                        w_u32(table + (index << 2), replacement);
                    }
                    if (mode == 2)
                    {
                        uint32 index = r_u32(cursor);
                        uint32 table = r_u32(data + 0x40);
                        uint32 replacement = r_u32(cursor + 4);
                        uint32 previous = r_u32(table + (index << 2));
                        index = r_u32(cursor);
                        ++count;
                        w_u32(cursor + 8, previous);
                        table = r_u32(data + 0x40);
                        w_u32(table + (index << 2), replacement);
                    }
                    if (mode == 3)
                    {
                        uint32 word = r_u32(cursor + 4);
                        w_u32(data + 0x48, word);
                        word = r_u32(cursor + 8);
                        w_u32(data + 0x4c, word);
                        word = r_u32(cursor);
                        w_u32(data + 0x44, word);
                        uint32 selected = G3Function8003AA48(data + 0x48);
                        if (selected != 0)
                        {
                            uint32 created = G3Function8002A454(0, selected, frame);
                            w_u32(data + 0x50, created);
                            uint32 flags = r_u32(created + 0x10);
                            w_u32(created + 0x10, flags | 0x800);
                            created = r_u32(data + 0x50);
                            flags = r_u32(created + 0x10);
                            w_u32(created + 0x10, flags | 0x400);
                        }
                        else
                            w_u32(data + 0x50, 0);
                        ++count;
                    }
                    if (mode == 4)
                    {
                        uint32 offset = r_u32(cursor);
                        uint32 address = data + offset;
                        if ((sint32)offset < 1000)
                        {
                            uint32 previous = r_s16(address);
                            uint32 replacement = r_u16(cursor + 4);
                            w_u32(cursor + 8, previous);
                            w_u16(address, replacement);
                        }
                        else
                        {
                            address = data + offset - 1000;
                            uint32 previous = r_u32(address);
                            uint32 replacement = r_u32(cursor + 4);
                            w_u32(cursor + 8, previous);
                            w_u32(address, replacement);
                        }
                        ++count;
                    }
                    if (mode == 5)
                    {
                        uint32 offset = r_u32(cursor);
                        if ((sint32)offset < 1000)
                        {
                            uint32 base = r_u32(data + 0x2c);
                            uint32 previous = r_s16(base + offset);
                            w_u32(cursor + 8, previous);
                            base = r_u32(data + 0x2c);
                            uint32 replacement = r_u16(cursor + 4);
                            w_u16(base + offset, replacement);
                        }
                        else
                        {
                            uint32 base = r_u32(data + 0x2c);
                            offset -= 1000;
                            uint32 previous = r_u32(base + offset);
                            w_u32(cursor + 8, previous);
                            base = r_u32(data + 0x2c);
                            uint32 replacement = r_u32(cursor + 4);
                            w_u32(base + offset, replacement);
                        }
                        ++count;
                    }
                    if (mode == 7)
                    {
                        uint32 flags = r_u32(player + 0x134);
                        w_u32(player + 0x134, (flags | 4) & 0xfffffff7);
                        for (uint32 index = 0; index < 8; ++index)
                        {
                            uint32 half = r_u16(cursor + (index << 1));
                            uint32 offset = index < 4 ? 0x240 : 0x242;
                            w_u16(data + offset + (index << 1), half);
                        }
                    }
                    if (mode == 8)
                    {
                        uint32 flags = r_u32(player + 0x18);
                        w_u32(player + 0x18, flags | 4);
                    }
                    if (mode == 9)
                    {
                        for (uint32 index = 0; index < 3; ++index)
                        {
                            uint32 half = r_u16(cursor + (index << 1));
                            w_u16(0x800ad0c4 + (index << 1), half);
                        }
                    }
                }
                else if (action == 0)
                {
                    if (mode == 0)
                    {
                        ++count;
                        uint32 descriptor = r_u32(object + 0x1c);
                        uint32 table = r_u32(descriptor + 0x10);
                        uint32 previous = r_u32(cursor + 8);
                        w_u32(table + (key << 2), previous);
                        w_u16(object + 0x86, 0xffff);
                        w_u32(cursor + 8, 0);
                    }
                    if (mode == 6)
                    {
                        ++count;
                        uint32 index = r_u32(cursor);
                        uint32 descriptor = r_u32(object + 0x1c);
                        uint32 previous = r_u32(cursor + 8);
                        uint32 table = r_u32(descriptor + 0x2c);
                        w_u32(table + (index << 2), previous);
                        w_u32(cursor + 8, 0);
                    }
                    if (mode == 2)
                    {
                        ++count;
                        uint32 index = r_u32(cursor);
                        uint32 table = r_u32(data + 0x40);
                        uint32 previous = r_u32(cursor + 8);
                        w_u32(table + (index << 2), previous);
                        w_u32(cursor + 8, 0);
                    }
                    if (mode == 3)
                    {
                        uint32 word = r_u32(0x800a0250);
                        w_u32(data + 0x48, word);
                        word = r_u32(0x800a0254);
                        ++count;
                        w_u32(data + 0x44, 0);
                        w_u32(data + 0x4c, word);
                    }
                    if (mode == 4)
                    {
                        uint32 offset = r_u32(cursor);
                        if ((sint32)offset < 1000)
                        {
                            uint32 previous = r_u16(cursor + 8);
                            w_u16(data + offset, previous);
                        }
                        else
                        {
                            uint32 previous = r_u32(cursor + 8);
                            w_u32(data + offset - 1000, previous);
                        }
                        w_u32(cursor + 8, 0);
                        ++count;
                    }
                    if (mode == 5)
                    {
                        uint32 offset = r_u32(cursor);
                        uint32 base = r_u32(data + 0x2c);
                        if ((sint32)offset < 1000)
                        {
                            uint32 previous = r_u16(cursor + 8);
                            w_u16(base + offset, previous);
                        }
                        else
                        {
                            uint32 previous = r_u32(cursor + 8);
                            w_u32(base + offset - 1000, previous);
                        }
                        w_u32(cursor + 8, 0);
                        ++count;
                    }
                    if (mode == 7)
                    {
                        uint32 flags = r_u32(player + 0x134);
                        w_u32(player + 0x134, flags & 0xfffffff3);
                    }
                    if (mode == 8)
                    {
                        uint32 flags = r_u32(player + 0x18);
                        w_u32(player + 0x18, flags & 0xfffffffb);
                    }
                    if (mode == 9)
                    {
                        sint32 current = r_s16(0x800ad0c4);
                        sint32 requested = r_s16(cursor);
                        if (current == requested)
                            w_u16(0x800ad0c4, 0xffff);
                    }
                }
                cursor += mode == 7 ? 16 : mode == 9 ? 8 : 12;
            }
        }
        first = r_u32(cursor);
        expected = r_u32(0x800a0224);
        if (first == expected)
            return count;
    }
}

uint32 G3Function800214EC(uint32 parameters, uint32 resource, uint32 *second_glyph, uint32 *third_glyph)
{
    FUNCTION_MARKER(0x800214EC, "SLUS_008.06");
    uint32 x = r_u32(parameters + 0x0c);
    uint32 y = r_u32(parameters + 0x10);
    return G3Function8002151C(x, y, parameters, resource, second_glyph, third_glyph);
}

uint32 G3Function8002151C(uint32 x, uint32 y, uint32 parameters, uint32 resource, uint32 *second_glyph, uint32 *third_glyph)
{
    FUNCTION_MARKER(0x8002151C, "SLUS_008.06");
    if (second_glyph != 0)
        *second_glyph = parameters;
    if (third_glyph != 0)
        *third_glyph = resource;
    uint32 game = r_u32(0x80097048);
    uint32 pool = r_u32(game + 0x68);
    uint32 order = r_u32(game + 0x38);
    uint32 packet = r_u32(pool + 4);
    do
    {
        if (resource == 0)
            break;
        uint32 end = r_u32(pool + 8);
        if (packet >= end - 192)
            break;
        uint32 models = r_u32(resource + 0x0c);
        uint32 model = r_u32(models);
        uint32 time = r_u32(parameters + 8);
        uint32 sequence = r_u32(model + 0x1c);
        G3Function80034FA4(sequence, time, second_glyph, third_glyph);
        models = r_u32(resource + 0x0c);
        model = r_u32(models);
        uint32 raw_count = r_u16(model + 4);
        sint32 half_count = (sint32)(sint16)raw_count / 2;
        uint32 index = r_u32(parameters + 4);
        if ((sint32)index >= half_count)
            break;
        uint32 records = r_u32(model + 0x14);
        uint32 texture = r_u32(records + index * 24 + 8);
        if (second_glyph != 0)
            *second_glyph = texture;
        for (uint32 word = 0; word < 3; ++word)
        {
            uint32 value = r_u32(texture + (word << 2));
            w_u32(packet + 0x0c + (word << 3), value);
        }
        uint32 bounds[4];
        for (uint32 pass = 0; pass < 4; ++pass)
        {
            uint32 offset = pass & 1;
            uint32 first = r_u8(texture + offset);
            uint32 second = r_u8(texture + offset + 4);
            uint32 third = r_u8(texture + offset + 8);
            uint32 selected;
            if (pass < 2)
                selected = first < second ? (first < third ? 0 : 8) : (second < third ? 4 : 8);
            else
                selected = second < first ? (third < first ? 0 : 8) : (third < second ? 4 : 8);
            bounds[pass] = r_u8(texture + offset + selected);
        }
        if (third_glyph != 0)
            *third_glyph = bounds[3];
        uint32 scale_y = r_u32(parameters + 0x1c);
        uint32 product_y = (bounds[3] - bounds[1]) * scale_y;
        uint32 scale_x = r_u32(parameters + 0x18);
        uint32 product_x = (bounds[2] - bounds[0]) * scale_x;
        uint32 scaled_y = (uint32)((sint32)product_y >> 13) * 320;
        uint32 extent_x = (uint32)((sint32)product_x >> 13);
        if ((sint32)scaled_y < 0)
            scaled_y += 511;
        uint32 extent_y = (uint32)((sint32)scaled_y >> 9);
        sint32 mirror = r_s16(parameters + 0x14);
        if (mirror == 0 || mirror == 1)
        {
            w_u8(packet + 0x0c, mirror == 0 ? bounds[0] : bounds[2]);
            w_u8(packet + 0x0d, bounds[1]);
            w_u8(packet + 0x14, mirror == 0 ? bounds[2] : bounds[0]);
            w_u8(packet + 0x15, bounds[1]);
            w_u8(packet + 0x1c, mirror == 0 ? bounds[2] : bounds[0]);
            w_u8(packet + 0x1d, bounds[3]);
        }
        w_u8(packet + 3, 7);
        w_u8(packet + 7, 0x25);
        if (r_s16(parameters + 0x16) != 0)
            w_u8(packet + 7, 0x27);
        w_u16(packet + 8, x - extent_x);
        w_u16(packet + 0x10, x + extent_x);
        w_u16(packet + 0x18, x + extent_x);
        w_u16(packet + 0x0a, y - extent_y);
        w_u16(packet + 0x12, y - extent_y);
        w_u16(packet + 0x1a, y + extent_y);
        uint32 depth = r_s16(0x800a3c76);
        uint32 head = r_u32(order + (depth << 2));
        w_u32(packet, (head & 0x00ffffff) | 0x07000000);
        depth = r_s16(0x800a3c76);
        game = r_u32(0x80097048);
        w_u32(order + (depth << 2), packet & 0x00ffffff);
        pool = r_u32(game + 0x68);
        uint32 primitives = r_u32(pool);
        w_u32(pool, primitives + 1);
        uint32 value = r_u32(texture);
        packet += 32;
        w_u32(packet + 0x0c, value);
        value = r_u32(texture + 4);
        w_u32(packet + 0x14, value);
        value = r_u32(texture + 8);
        w_u32(packet + 0x1c, value);
        mirror = r_s16(parameters + 0x14);
        if (mirror == 0 || mirror == 1)
        {
            w_u8(packet + 0x0c, mirror == 0 ? bounds[0] : bounds[2]);
            w_u8(packet + 0x0d, bounds[1]);
            w_u8(packet + 0x14, mirror == 0 ? bounds[2] : bounds[0]);
            w_u8(packet + 0x15, bounds[3]);
            w_u8(packet + 0x1c, mirror == 0 ? bounds[0] : bounds[2]);
            w_u8(packet + 0x1d, bounds[3]);
        }
        w_u8(packet + 3, 7);
        w_u8(packet + 7, 0x25);
        if (r_s16(parameters + 0x16) != 0)
            w_u8(packet + 7, 0x27);
        w_u16(packet + 0x0a, y - extent_y);
        w_u16(packet + 0x10, x + extent_x);
        w_u16(packet + 8, x - extent_x);
        w_u16(packet + 0x12, y + extent_y);
        w_u16(packet + 0x18, x - extent_x);
        w_u16(packet + 0x1a, y + extent_y);
        depth = r_s16(0x800a3c76);
        head = r_u32(order + (depth << 2));
        w_u32(packet, (head & 0x00ffffff) | 0x07000000);
        depth = r_s16(0x800a3c76);
        game = r_u32(0x80097048);
        w_u32(order + (depth << 2), packet & 0x00ffffff);
        pool = r_u32(game + 0x68);
        primitives = r_u32(pool);
        w_u32(pool, primitives + 1);
        pool = r_u32(game + 0x68);
        packet += 32;
        w_u32(pool + 4, packet);
    } while (false);
    uint32 count = r_u32(0x800a3c64);
    uint32 result = count << 3;
    if ((sint32)count < 511)
    {
        w_u8(0x800a2c48 + result, 32);
        result = r_u32(0x800a3c64) + 1;
        w_u32(0x800a3c64, result);
    }
    return result;
}

uint32 G3Function8003E518(uint32 object, uint32 sound, uint32 pitch, uint32 volume, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003E518, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 raw_radius = r_u32(frame + 0x48);
    if ((volume << 16) == 0)
        return 0;
    uint32 game = r_u32(0x80097048);
    uint32 camera = r_u32(game + 0x4c);
    uint32 mode = G3Function8004DABC(camera, frame);
    bool camera_relative = false;
    if (mode == 5)
    {
        game = r_u32(0x80097048);
        uint32 flags = r_u32(game + 0x6834);
        camera_relative = (flags & 0x10) != 0;
    }
    uint32 radius;
    uint32 dx;
    uint32 dy;
    uint32 dz;
    if (camera_relative)
    {
        radius = (uint32)((sint32)(raw_radius << 16) >> 15);
        uint32 x = r_s16(object + 0x58);
        uint32 listener_x = r_s16(camera);
        uint32 y = r_s16(object + 0x5a);
        uint32 listener_y = r_s16(camera + 2);
        dx = x - listener_x;
        uint32 z = r_s16(object + 0x5c);
        uint32 listener_z = r_s16(camera + 4);
        dy = y - listener_y;
        dz = z - listener_z;
    }
    else
    {
        game = r_u32(0x80097048);
        radius = (uint32)(sint32)(sint16)raw_radius;
        uint32 listener = r_u32(game + 0x50);
        uint32 x = r_s16(object + 0x58);
        uint32 listener_x = r_s16(listener + 0x58);
        uint32 y = r_s16(object + 0x5a);
        dx = x - listener_x;
        uint32 listener_y = r_s16(listener + 0x5a);
        uint32 z = r_s16(object + 0x5c);
        uint32 listener_z = r_s16(listener + 0x5c);
        dy = y - listener_y;
        dz = z - listener_z;
    }
    uint32 squares = dx * dx + dy * dy + dz * dz;
    uint32 distance = G3Function8002F448(squares, 0, frame);
    if ((sint32)radius < (sint32)distance)
        return 0;
    uint32 angle = G3Function8002F240(dy, dx, frame);
    uint32 divisor = (uint32)((sint64)(sint32)radius / (sint32)(sint16)volume);
    uint32 numerator = radius - distance;
    uint32 gain = divisor == 0 ? ((sint32)numerator < 0 ? 1u : 0xffffffff) : (uint32)((sint64)(sint32)numerator / (sint32)divisor);
    uint32 heading = r_u16(camera + 0x1c);
    uint32 relative = heading - angle + 0x1c00;
    uint32 quadrant = (relative & 0xfff) >> 10;
    uint32 fraction = relative & 0x3ff;
    if ((sint32)(sint16)gain >= 128)
        gain = 127;
    uint32 pan;
    switch (quadrant)
    {
    case 0:
        pan = 63 - (fraction >> 4);
        pitch = pitch - 15 + (fraction >> 6);
        break;
    case 1:
        pan = fraction >> 4;
        pitch += fraction >> 6;
        break;
    case 2:
        pan = (fraction >> 4) | 64;
        pitch = pitch + 15 - (fraction >> 6);
        break;
    default:
        pan = 127 - (fraction >> 4);
        pitch -= fraction >> 6;
        break;
    }
    numerator = distance << 8;
    uint32 attenuation = radius == 0 ? ((sint32)numerator < 0 ? 1u : 0xffffffff) : (uint32)((sint64)(sint32)numerator / (sint32)radius);
    uint32 signed_pan = (uint32)(sint32)(sint16)pan;
    if ((sint32)signed_pan < 64)
    {
        uint32 product = (63 - signed_pan) * attenuation;
        pan = 63 - (uint32)((sint32)product >> 8);
    }
    else
    {
        uint32 product = (signed_pan - 64) * attenuation;
        pan = (uint32)((sint32)product >> 8) + 64;
    }
    return G3Function8003F1C4(sound & 0xffff, gain & 0xffff, pan & 0xffff, (uint32)(sint32)(sint16)pitch);
}

uint32 G3Function800726FC(uint32 object, uint32 game, uint32 data, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800726FC, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 owner = r_u32(0x800a0284);
    G3Function800509E0(owner + 0x1c, game, frame);
    uint32 matrix = r_u32(data);
    w_u16(matrix + 8, 0);
    matrix = r_u32(data);
    w_u16(matrix, 0);
    matrix = r_u32(data);
    w_u16(matrix + 2, 0);
    w_u32(object + 0x154, 0x01000000);
    w_u16(object + 0x60, 0);
    w_u16(object + 0x62, 0);
    w_u32(object + 0x150, 0);
    w_u16(object + 0x6e, 38);
    w_u16(object + 0x5e, 0);
    for (uint32 index = 0; index < 8; ++index)
        w_u32(0x80099fb4 + index * 0x34 + 0x0c, 0);
    owner = r_u32(0x800a0284);
    w_u32(owner + 0x10, 0);
    w_u32(owner + 0x0c, 0);
    return owner;
}

uint32 G3Function800509E0(uint32 vector, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800509E0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 saved = r_u32(0x80096a5c);
    uint32 words[2];
    words[0] = r_masked_u32(0x8009ea64, 15);
    words[0] = r_masked_u32(0x8009ea64, 15);
    words[1] = r_masked_u32(0x8009ea68, 15);
    words[1] = r_masked_u32(0x8009ea68, 15);
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 destination = frame + 0x10 + (index << 2);
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        uint32 low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15u << low_lane) & 15);
    }
    w_u32(0x80096a80, 0);
    uint32 angle = G3Function8002F540(frame + 0x10, vector, frame);
    G3Function80050A78((uint32)(sint32)(sint16)angle, frame);
    G3Function8002F744(0x80096a72);
    return G3Function8002F718(saved);
}

uint32 G3Function80050A78(uint32 angle, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80050A78, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    sint32 scale = r_s16(0x80096a64);
    if (scale == 4096)
    {
        uint32 words[2];
        for (uint32 index = 0; index < 2; ++index)
        {
            uint32 source = 0x80096a72 + (index << 2);
            uint32 high = r_masked_u32((source + 3) & 0xfffffffc, 3);
            uint32 low = r_masked_u32(source & 0xfffffffc, 12);
            words[index] = (high << 16) | (low >> 16);
        }
        for (uint32 index = 0; index < 2; ++index)
        {
            uint32 destination = 0x80096a6a + (index << 2);
            w_masked_u32((destination + 3) & 0xfffffffc, words[index] >> 16, 3);
            w_masked_u32(destination & 0xfffffffc, words[index] << 16, 12);
        }
    }
    else
    {
        uint32 matrix = r_u32(0x80096a60);
        G3Function800202A4(matrix, frame + 0x10, 0x15, frame);
        G3Function8001FA7C(frame + 0x10, 0x80096a6a, frame);
    }
    if ((sint32)(angle << 16) < 0)
        angle = 0u - angle;
    uint32 signed_angle = (uint32)(sint32)(sint16)angle;
    uint32 scaled = ((signed_angle << 4) - signed_angle) << 4;
    if ((sint32)scaled < 0)
        scaled += 4095;
    uint32 divisor = (uint32)((sint32)scaled >> 12) + 16;
    if (divisor == 0)
        divisor = 1;
    uint32 quotient = (uint32)((sint64)(sint32)signed_angle / (sint32)divisor);
    w_u16(0x80096a7a, quotient);
    sint32 narrowed = (sint32)(sint16)quotient;
    uint32 result = narrowed == 0 ? 4096u : (uint32)((sint64)4096 / narrowed);
    w_u16(0x80096a7a, result);
    result = r_u16(0x80096a7a);
    w_u16(0x80096a64, result);
    return result;
}

uint32 G3Function8002F540(uint32 first, uint32 second, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002F540, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 left = r_s16(first);
    uint32 right = r_s16(second);
    uint32 product_x = left * right;
    if (left == right)
    {
        left = r_s16(first + 2);
        right = r_s16(second + 2);
        if (left == right)
        {
            left = r_s16(first + 4);
            right = r_s16(second + 4);
            if (left == right)
                return 0;
        }
        left = r_s16(first);
        right = r_s16(second);
        product_x = left * right;
    }
    left = r_s16(first + 2);
    right = r_s16(second + 2);
    uint32 product_y = left * right;
    left = r_s16(first + 4);
    right = r_s16(second + 4);
    uint32 product_z = left * right;
    uint32 rounded = product_x + product_y + product_z + 2048;
    sint32 dot = (sint32)rounded >> 12;
    if (dot > 4096)
        dot = 4096;
    if (dot < -4096)
        dot = -4096;
    uint32 square = (uint32)dot * (uint32)dot;
    uint32 remainder = 4096u - (uint32)((sint32)square >> 12);
    uint32 root = G3Function8002F368(remainder, frame);
    uint32 angle = G3Function8002F240(root, (uint32)dot, frame);
    return (uint32)(sint32)(sint16)angle;
}

uint32 G3Function8001FA7C(uint32 angles, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8001FA7C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    w_u32(frame + 0x64, output);
    for (uint32 index = 0; index < 4; ++index)
    {
        uint32 half = r_u16(angles + (index << 1));
        w_u16(frame + 0x20 + (index << 1), half);
    }
    uint32 words[2];
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 source = frame + 0x20 + (index << 2);
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
        uint32 low_mask = 0xffffffffu >> (low_lane * 8);
        words[index] = ((high << ((3 - high_lane) * 8)) & ~low_mask) | (low >> (low_lane * 8));
    }
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 destination = frame + 0x18 + (index << 2);
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        uint32 low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15u << low_lane) & 15);
    }
    uint32 controls = r_s16(frame + 0x1e);
    uint32 swap = controls & 1;
    uint32 repetition = (controls >> 1) & 1;
    uint32 parity = (controls >> 2) & 1;
    uint32 axis = r_u8(0x8009d618 + ((controls >> 3) & 3));
    uint32 next_axis = r_u8(0x8009d620 + axis + parity);
    uint32 last_axis = r_u8(0x8009d620 + axis - (parity - 1));
    w_u32(frame + 0x30, repetition);
    w_u32(frame + 0x28, next_axis);
    w_u32(frame + 0x2c, last_axis);
    if (swap == 1)
    {
        uint32 third = r_u16(frame + 0x1c);
        uint32 first = r_u16(frame + 0x18);
        w_u16(frame + 0x18, third);
        w_u16(frame + 0x1c, first);
    }
    if (parity == 1)
    {
        uint32 second = r_u16(frame + 0x1a);
        w_u16(frame + 0x1a, 0u - second);
    }
    uint32 x = r_u16(frame + 0x18);
    uint32 y = r_u16(frame + 0x1a);
    uint32 z = r_u16(frame + 0x1c);
    x = (uint32)((sint32)(x << 16) >> 17);
    y = (uint32)((sint32)(y << 16) >> 17);
    z = (uint32)((sint32)(z << 16) >> 17);
    uint32 cx = G3Function8005B470(x);
    uint32 cy = G3Function8005B470(y);
    uint32 cz = G3Function8005B470(z);
    uint32 sx = G3Function8005B590(x);
    uint32 sy = G3Function8005B590(y);
    uint32 sz = G3Function8005B590(z);
    uint32 product_cc = cx * cz;
    uint32 product_cs = cx * sz;
    uint32 product_sc = sx * cz;
    uint32 product_ss = sx * sz;
    repetition = r_u32(frame + 0x30);
    uint32 cc = (uint32)((sint32)product_cc >> 12);
    uint32 cs = (uint32)((sint32)product_cs >> 12);
    uint32 sc = (uint32)((sint32)product_sc >> 12);
    uint32 ss = (uint32)((sint32)product_ss >> 12);
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 scalar;
    if (repetition == 1)
    {
        first = cy * (cs + sc);
        second = sy * (cc + ss);
        third = sy * (cs - sc);
        scalar = cy * (cc - ss);
    }
    else
    {
        first = cy * sc - sy * cs;
        second = cy * ss + sy * cc;
        third = cy * cs - sy * sc;
        scalar = cy * cc + sy * ss;
    }
    w_u16(frame + 0x10 + (axis << 1), (uint32)((sint32)first >> 12));
    next_axis = r_u32(frame + 0x28);
    w_u16(frame + 0x10 + (next_axis << 1), (uint32)((sint32)second >> 12));
    last_axis = r_u32(frame + 0x2c);
    w_u16(frame + 0x10 + (last_axis << 1), (uint32)((sint32)third >> 12));
    uint32 destination = r_u32(frame + 0x64);
    w_u16(destination + 6, (uint32)((sint32)scalar >> 12));
    if (parity == 1)
    {
        next_axis = r_u32(frame + 0x28);
        uint32 address = frame + 0x10 + (next_axis << 1);
        uint32 component = r_u16(address);
        w_u16(address, 0u - component);
    }
    uint32 component = r_u16(frame + 0x10);
    destination = r_u32(frame + 0x64);
    w_u16(destination, component);
    component = r_u16(frame + 0x12);
    w_u16(destination + 2, component);
    component = r_u16(frame + 0x14);
    w_u16(destination + 4, component);
    return component;
}

uint32 G3Function800274D8(uint32 root, uint32 name, uint32 text)
{
    FUNCTION_MARKER(0x800274D8, "SLUS_008.06");
    uint32 character = r_u8(text);
    uint32 index = 0;
    while (character != 0 && index < 13)
    {
        uint32 name_byte = name + index;
        uint32 root_byte = root + index;
        w_u8(name_byte, 0);
        w_u8(root_byte, 0);
        character = r_u8(text);
        if (character == 32 || character == 9 || character == 10 || character == 95)
            break;
        if (character - 48u < 10)
            w_u8(name_byte, character);
        else
        {
            w_u8(root_byte, character);
            character = r_u8(text);
            w_u8(name_byte, character);
        }
        text += 1;
        character = r_u8(text);
        index += 1;
    }
    w_u8(root + index, 0);
    uint32 result = name + index;
    w_u8(result, 0);
    return result;
}

uint32 G3Function80012720(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80012720, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 index = r_u32(0x8009cc00);
    uint32 offset = (index << 1) + index;
    uint32 records = r_u32(0x8009ceac);
    uint32 text = r_u32(records + (offset << 3) + 0x0c);
    G3Function800274D8(frame + 0x10, frame + 0x20, text);
    uint32 game = r_u32(0x80097048);
    G3Function80027644(frame + 0x10, frame + 0x20, game, frame);
    game = r_u32(0x80097048);
    w_u16(game + 0x6786, 0);
    return game;
}

uint32 G3Function80027584(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80027584, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 offset = ((state << 3) - state) << 2;
    uint32 text = r_u32(0x80099708 + offset);
    G3Function800274D8(frame + 0x10, frame + 0x20, text);
    uint32 game = r_u32(0x80097048);
    return G3Function80027644(frame + 0x10, frame + 0x20, game, frame);
}

uint32 G3Function8001278C(uint32 guest_stack)
{
    FUNCTION_MARKER(0x8001278C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 count = 0;
    uint32 entry = 0x80096854;
    do
    {
        uint32 index = r_u32(0x8009cc00);
        uint32 offset = (index << 1) + index;
        uint32 records = r_u32(0x8009ceac);
        uint32 text = r_u32(records + (offset << 3) + 0x0c);
        uint32 comparison = G3CompareString80085EC4(text, entry);
        count += 1;
        if (comparison == 0)
        {
            uint32 level = r_u32(entry + 0x10);
            w_u32(0x800aa7b8, level);
            break;
        }
        entry += 0x14;
    } while (count < 13);
    w_u32(0x800aa7c0, 1);
    G3Function80027584(0x17, frame);
    uint32 game = r_u32(0x80097048);
    w_u16(game + 0x6786, 0);
    return game;
}

uint32 G3Function800277A4(uint32 game, uint32 name, uint32 enabled)
{
    FUNCTION_MARKER(0x800277A4, "SLUS_008.06");
    uint32 index = 0;
    uint32 entry = 0x80096dd4;
    do
    {
        uint32 name_word = r_u32(name);
        uint32 entry_word = r_u32(entry);
        if (name_word == entry_word)
        {
            name_word = r_u32(name + 4);
            entry_word = r_u32(entry + 4);
            if (name_word == entry_word)
            {
                uint32 result = game + ((index >> 5) << 2);
                uint32 bit = 1u << (index & 0x1f);
                uint32 flags = r_u32(result + 0x69c8);
                if (enabled == 1)
                    flags |= bit;
                else
                    flags &= ~bit;
                w_u32(result + 0x69c8, flags);
                return result;
            }
        }
        index += 1;
        entry += 0x10;
    } while (index < 0x27);
    return 0;
}

uint32 G3Function80027904(uint32 records, uint32 key_pointer)
{
    FUNCTION_MARKER(0x80027904, "SLUS_008.06");
    uint32 words = r_u32(records + 8);
    uint32 current = records + 0x0c;
    uint32 end = records + (words << 2);
    if (current >= end)
        return 0;
    uint32 key = r_u32(key_pointer);
    do
    {
        uint32 value = r_u32(current);
        if (value == key)
            return current;
        uint32 size = r_u32(current + 8);
        current += size << 2;
    } while (current < end);
    return 0;
}

uint32 G3Function80014F00(uint32 game)
{
    FUNCTION_MARKER(0x80014F00, "SLUS_008.06");
    uint32 flags = r_u32(game + 0xe8);
    if ((flags & 8) != 0)
        return (uint32)r_s16(0x80096a48);
    uint32 resource = r_u32(game + 0x4c);
    return (uint32)r_s16(resource + 0x1c);
}

void G3Function800151D4(uint32 placement, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800151D4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (placement == 0)
        return;
    uint32 flags = r_u32(placement + 0x18);
    w_u32(placement + 0x18, flags | 0x80);
    uint32 object = r_u32(placement + 0x20);
    if (object == 0 || (flags & 0x80) != 0)
        return;
    uint32 target = r_u32(object + 0xec);
    if (target != 0)
    {
        uint32 game = r_u32(0x80097048);
        G3DispatchStateEvent(target, object, game, 22, 0, frame);
    }
}

void G3Function80015238(uint32 placement, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80015238, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (placement == 0)
        return;
    uint32 flags = r_u32(placement + 0x18);
    uint32 object = r_u32(placement + 0x20);
    w_u32(placement + 0x18, flags & 0xffffff7f);
    if (object == 0 || (flags & 0x80) == 0)
        return;
    uint32 target = r_u32(object + 0xec);
    if (target != 0)
    {
        uint32 game = r_u32(0x80097048);
        G3DispatchStateEvent(target, object, game, 23, 0, frame);
    }
}

uint32 G3Function800152A4(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800152A4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 placement = r_u32(command + 4);
    if (placement != 0)
        G3Function800151D4(placement, frame);
    return 2;
}

uint32 G3Function800152D4(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800152D4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 placement = r_u32(command + 4);
    if (placement != 0)
        G3Function80015238(placement, frame);
    return 2;
}

uint32 G3Function80015304(uint32 object, uint32 kind, uint32 first, uint32 second, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80015304, "SLUS_008.06");
    uint32 payload = r_u32(guest_stack + 0x10);
    uint32 slot = 0x800a08e0;
    uint32 index = 0;
    while (slot < 0x800a0b60)
    {
        uint32 active = r_u32(slot);
        if (active == 0)
        {
            w_u32(slot, kind);
            w_u32(slot + 4, object);
            w_u32(slot + 8, first);
            w_u32(slot + 0x0c, second);
            w_u32(slot + 0x10, payload);
            uint32 source = object + 0x58;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
            uint32 packed = (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
            uint32 last_component = (uint32)r_s16(object + 0x5c);
            uint32 destination = slot + 0x14;
            high_address = destination + 3;
            high_lane = high_address & 3;
            low_lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, packed >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, packed << (low_lane * 8), (15u << low_lane) & 15);
            w_u16(slot + 0x18, last_component);
            uint32 words[2];
            for (uint32 word = 0; word < 2; ++word)
            {
                source = object + 0x60 + (word << 2);
                high_address = source + 3;
                high_lane = high_address & 3;
                low_lane = source & 3;
                high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
                low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
                words[word] = (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
            }
            for (uint32 word = 0; word < 2; ++word)
            {
                destination = slot + 0x1a + (word << 2);
                high_address = destination + 3;
                high_lane = high_address & 3;
                low_lane = destination & 3;
                w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
                w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15u << low_lane) & 15);
            }
            uint32 identity = r_u32(object + 0x38);
            sint32 select = r_s16(0x8009d350);
            w_u32(slot + 0x24, identity);
            if (select != 0)
            {
                w_u16(0x8009d352, index);
                w_u16(0x8009d350, 0);
            }
            return 0;
        }
        slot += 0x28;
        index += 1;
    }
    return 0;
}

uint32 G3Function800153C8(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800153C8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 kind = r_u32(command + 4);
    w_u32(frame + 0x10, command + 8);
    G3Function80015304(object, kind, 0, 0, frame);
    return 0;
}

uint32 G3Function800175B0(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800175B0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 list = r_u32(command + 4);
    sint32 count = r_s32(list);
    uint32 current = list + 4;
    uint32 index = 0;
    if (count > 0)
    {
        do
        {
            uint32 placement = r_u32(current);
            current += 4;
            index += 1;
            G3Function800151D4(placement, frame);
        } while ((sint32)index < count);
    }
    return 2;
}

uint32 G3Function80017618(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80017618, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 list = r_u32(command + 4);
    sint32 count = r_s32(list);
    uint32 current = list + 4;
    uint32 index = 0;
    if (count > 0)
    {
        do
        {
            uint32 placement = r_u32(current);
            current += 4;
            index += 1;
            G3Function80015238(placement, frame);
        } while ((sint32)index < count);
    }
    return 2;
}

uint32 G3Function8001553C(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8001553C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 placement = r_u32(command + 4);
    if (placement != 0 || object == 0)
    {
        object = r_u32(placement + 0x20);
        if (object == 0)
            return 3;
    }
    uint32 target = r_u32(object + 0xec);
    if (target != 0)
    {
        uint32 game = r_u32(0x80097048);
        uint32 payload = r_u32(command + 8);
        G3DispatchStateEvent(target, object, game, 9, payload, frame);
    }
    return 3;
}

uint32 G3Function800155DC(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x800155DC, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 resource = r_u32(game + 0x48);
    uint32 value = r_u32(command + 4);
    w_u32(resource + 0x6c, value);
    return 2;
}

uint32 G3Function80015600(uint32 object, uint32 command)
{
    FUNCTION_MARKER(0x80015600, "SLUS_008.06");
    uint32 value = r_u8(command + 4);
    w_u16(object + 0x8e, value);
    return 2;
}

uint32 G3Function800157DC(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800157DC, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x18;
    uint32 state = r_u32(command + 4);
    uint32 camera = r_u32(game + 0x4c);
    G3Function80013CDC(camera, state, frame);
    return 2;
}

uint32 G3Function8001586C(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x8001586C, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 direction = r_u32(command + 4);
    uint32 camera = r_u32(game + 0x4c);
    G3Function80014288(camera, direction);
    return 2;
}

uint32 G3Function800158AC(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800158AC, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 game = r_u32(0x80097048);
    uint32 index = r_u32(command + 4);
    uint32 value = r_u32(command + 8);
    uint32 camera = r_u32(game + 0x4c);
    G3Function800140E4(camera, index, value, frame);
    return 3;
}

uint32 G3Function8004D784(uint32 camera, uint32 game, uint32 delta, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004D784, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 flags = r_u32(0x80096b28);
    w_u32(0x80096b28, flags & 0xfffffdff);
    G3Function800145D0(0x80096b34, 1, frame);
    uint32 state = r_u32(0x80096a88);
    uint32 target = r_u32(0x80099650 + (state << 3) + 4);
    if (target == 0)
        return 0;
    uint32 result = G3DispatchStateEvent(target, camera, game, 11, delta, frame);
    if (result == 0)
    {
        for (uint32 offset = 0; offset < 6; offset += 2)
        {
            uint32 component = r_u16(camera + offset);
            uint32 change = r_u16(delta + offset);
            result = component + change;
            w_u16(camera + offset, result);
        }
    }
    return result;
}

uint32 G3Function80015B50(uint32 context, uint32 delta, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80015B50, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 game = r_u32(0x80097048);
    uint32 object = r_u32(game + 0x50);
    uint32 target = r_u32(object + 0xec);
    G3DispatchStateEvent(target, object, game, 13, delta, frame);
    game = r_u32(0x80097048);
    uint32 camera = r_u32(game + 0x4c);
    return G3Function8004D784(camera, game, delta, frame);
}

uint32 G3Function80015CCC(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80015CCC, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 value = r_u32(command + 4);
    uint32 camera = r_u32(game + 0x4c);
    G3Function8004D8B0(camera, value);
    return 2;
}

uint32 G3Function800499E0(uint32 index, uint32 value)
{
    FUNCTION_MARKER(0x800499E0, "SLUS_008.06");
    uint32 data = r_u32(0x80098ff0);
    w_u8(data + (index & 0xffff) + 0x564, value);
    return 0;
}

uint32 G3Function8003EDC4(uint32 index, uint32 value)
{
    FUNCTION_MARKER(0x8003EDC4, "SLUS_008.06");
    return G3Function800499E0(index & 0xffff, value & 0xffff);
}

uint32 G3Function8003EAB0(uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003EAB0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 kind = r_u32(command + 4);
    if (kind == 25)
    {
        uint32 sequence = r_u32(command + 8);
        uint32 index = r_u32(command + 0x0c);
        G3Function8003ED08(sequence, index, frame);
        return command + 0x10;
    }
    if (kind == 86)
    {
        uint32 index = (uint32)r_s16(command + 8);
        uint32 value = (uint32)r_s16(command + 0x0c);
        G3Function8003EDC4(index, value);
        return command + 0x10;
    }
    return command + 0x0c;
}

uint32 G3Function80015DF0(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80015DF0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 value = r_u32(command);
    w_u32(frame + 0x14, value);
    value = r_u32(command + 4);
    w_u32(frame + 0x18, value);
    value = r_u32(command + 8);
    w_u32(frame + 0x1c, value);
    G3Function8003EAB0(frame + 0x10, frame);
    return 3;
}

uint32 G3Function80015F4C(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80015F4C, "SLUS_008.06");
    uint32 count = r_u32(command + 4);
    if ((sint32)count <= 0)
        return 0;
    w_u32(command + 4, count - 1);
    return 2;
}

uint32 G3Function8001609C(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8001609C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 placement = r_u32(command + 4);
    if (placement == 0)
    {
        uint32 player = r_u32(0x8009efc4);
        uint32 value = r_u32(0x8009d34c);
        w_u32(player + 0x150, value);
        return 2;
    }
    uint32 flags = r_u32(placement + 0x18);
    w_u32(placement + 0x18, flags & 0xfffffeff);
    placement = r_u32(command + 4);
    uint32 object = r_u32(placement + 0x20);
    if (object != 0)
    {
        if ((flags & 0x100) != 0)
        {
            uint32 target = r_u32(object + 0xec);
            if (target != 0)
            {
                uint32 game = r_u32(0x80097048);
                G3DispatchStateEvent(target, object, game, 21, 0, frame);
            }
        }
        flags = r_u32(object + 0x10);
        flags &= 0xff7fffff;
        w_u32(object + 0x10, flags);
        if ((flags & 0x100000) == 0)
        {
            flags = r_u32(object + 0x14);
            if ((flags & 0x10000) == 0)
                w_u32(object + 0x14, flags | 0x1000);
        }
    }
    return 2;
}

uint32 G3Function800168E8(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x800168E8, "SLUS_008.06");
    uint32 target = r_u32(command + 4);
    uint32 flags = r_u16(target);
    w_u16(target, flags | 1);
    return 2;
}

uint32 G3Function80016908(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80016908, "SLUS_008.06");
    uint32 target = r_u32(command + 4);
    uint32 flags = r_u16(target);
    w_u16(target, flags & 0xfffe);
    return 2;
}

uint32 G3Function80016928(uint32 object)
{
    FUNCTION_MARKER(0x80016928, "SLUS_008.06");
    uint32 flags = r_u32(object + 0x18);
    if ((flags & 4) == 0)
    {
        flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags | 0x20000000);
    }
    return 1;
}

uint32 G3Function80016954(uint32 object)
{
    FUNCTION_MARKER(0x80016954, "SLUS_008.06");
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x10, flags & 0xdfffffff);
    return 1;
}

uint32 G3Function80016970(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80016970, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 value = r_u32(command + 4);
    w_u32(game + 0x6788, value);
    return 2;
}
