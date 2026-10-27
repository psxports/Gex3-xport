#include "xport.h"
#include "psx.h"
#include "psx_gte.h"
#include "game_functions.h"
#include <string.h>

uint32 G3Function80016A44(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80016A44, "SLUS_008.06");
    uint32 index = r_u32(command + 4);
    uint32 value = r_u32(command + 8);
    w_u32(0x800a0760 + (index << 2), value);
    return 3;
}

uint32 G3Function80016B6C(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80016B6C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 index = r_u8(command + 4);
    uint32 value = r_u32(0x800a0760 + (index << 2));
    uint32 sequence = r_u32(command + 8);
    if (value != 0)
    {
        if (sequence == 0)
            return 3;
        G3Function80018DE0(object, sequence, frame);
        return 0;
    }
    return sequence != 0 ? 3 : 0;
}

uint32 G3Function80016CD0(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80016CD0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 index = r_u8(command + 4);
    uint32 expected = r_u8(command + 5);
    uint32 value = r_u32(0x800a0760 + (index << 2));
    uint32 sequence = r_u32(command + 8);
    if (value == expected)
    {
        if (sequence == 0)
            return 3;
        G3Function80018DE0(object, sequence, frame);
        return 0;
    }
    return sequence != 0 ? 3 : 0;
}

uint32 G3Function80016DBC(uint32 object, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80016DBC, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 sequence = r_u32(command + 4);
    G3Function80018DE0(object, sequence + 4, frame);
    return 2;
}

uint32 G3Function80016F24(void)
{
    FUNCTION_MARKER(0x80016F24, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 flags = r_u32(game + 0x6834);
    w_u32(game + 0x6834, flags & 0xffffff6f);
    return 1;
}

uint32 G3Function8001727C(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8001727C, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x20;
    uint32 player = r_u32(game + 0x50);
    uint32 amount = r_u32(command + 8);
    uint32 state = r_u32(command + 4);
    if (amount != 0)
        G3Function8005A5F4(player, amount, frame);
    game = r_u32(0x80097048);
    G3Function8006FF1C(player, game, state, frame);
    return 3;
}

uint32 G3Function80017D80(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80017D80, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 value = r_u16(command + 6);
    w_u16(game + 0xf0, value);
    value = (uint32)r_s16(command + 6);
    if ((sint32)value < 0)
        value = 0u - value;
    w_u16(game + 0xf2, value);
    value = r_u16(command + 4);
    w_u16(game + 0xf4, value);
    return 2;
}

uint32 G3Function80017FF8(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80017FF8, "SLUS_008.06");
    uint32 index = r_u32(command + 4);
    uint32 game = r_u32(0x80097048);
    uint32 value = r_u32(command + 8);
    w_u32(game + (index << 2) + 0x68cc, value);
    return 3;
}

uint32 G3Function8005A5F4(uint32 object, uint32 amount, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005A5F4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    for (uint32 pair = 0; pair < 2; ++pair)
    {
        uint32 source = 0x8009edf4 + (pair << 3);
        uint32 words[2];
        words[0] = r_masked_u32(source, 15);
        words[0] = r_masked_u32(source, 15);
        words[1] = r_masked_u32(source + 4, 15);
        words[1] = r_masked_u32(source + 4, 15);
        for (uint32 word = 0; word < 2; ++word)
        {
            uint32 destination = frame + 0x18 + (pair << 3) + (word << 2);
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15u << low_lane) & 15);
        }
    }
    if (amount == 0)
        return 0;
    bool negative = (sint32)amount < 0;
    uint32 parameters = frame + (negative ? 0x18 : 0x20);
    sint32 numerator = negative ? -720 : 720;
    sint32 step = numerator / (sint32)amount;
    w_u32(frame + 0x10, (uint32)step);
    uint32 angle = negative ? 360u : (uint32)-360;
    uint32 result = G3Function8005A700(object, parameters, amount, angle, frame);
    if (result != 0)
    {
        uint32 data = result + 0x104;
        w_u16(data + 0x0e, 1);
        w_u16(data + 0x18, 0x1000);
        w_u16(data + 0x1a, 0);
        w_u16(data + 0x1c, 0);
        numerator = negative ? 1272 : -1272;
        uint32 rate;
        if (step == 0)
            rate = numerator < 0 ? 1u : 0xffffffff;
        else
            rate = (uint32)(numerator / step);
        w_u16(data + 0x1e, rate);
    }
    return result;
}

uint32 G3Function8005A700(uint32 object, uint32 parameters, uint32 amount, uint32 angle, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005A700, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    uint32 player = r_u32(0x8009efc4);
    uint32 player_data = r_u32(player + 0x24);
    uint32 resource_copy = r_u32(player_data + 0xb4);
    uint32 effect_resource = G3Function8003AA48(0x8009ee04);
    uint32 words[2];
    words[0] = r_masked_u32(0x8009edf4, 15);
    words[0] = r_masked_u32(0x8009edf4, 15);
    words[1] = r_masked_u32(0x8009edf8, 15);
    words[1] = r_masked_u32(0x8009edf8, 15);
    for (uint32 word = 0; word < 2; ++word)
    {
        uint32 destination = frame + 0x10 + (word << 2);
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        uint32 low_lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15u << low_lane) & 15);
    }
    if (amount == 0)
        return 0;
    if (parameters == 0)
        parameters = frame + 0x10;
    G3Function8005A544(frame);
    if (object == 0)
        return 0;
    uint32 resource = r_u32(object + 0x1c);
    if (resource == 0 || effect_resource == 0)
        return 0;
    uint32 duplicate = G3Function8002A454(object, resource, frame);
    uint32 effect = G3Function8002A454(object, effect_resource, frame);
    if (duplicate == 0 || effect == 0)
        return effect;
    uint32 data = effect + 0x104;
    uint32 flags = r_u32(effect + 0x10);
    w_u32(data, object);
    w_u32(effect + 0x10, flags | 0x400);
    w_u32(data + 4, duplicate);
    w_u16(data + 8, amount);
    w_u16(data + 0x0a, angle);
    uint32 step = r_u32(frame + 0x58);
    uint32 magnitude = (sint32)angle < 0 ? 0u - angle : angle;
    w_u16(data + 0x0e, 0);
    w_u16(data + 0x0c, step);
    uint32 component = r_u16(parameters);
    w_u16(data + 0x10, component);
    component = r_u16(parameters + 2);
    w_u16(data + 0x12, component);
    component = r_u16(parameters + 4);
    w_u16(data + 0x1e, 0);
    w_u16(data + 0x14, component);
    magnitude = (uint32)(sint32)(sint16)(magnitude & 0xffff);
    G3Function8002F9A8(frame + 0x18, magnitude, parameters, frame);
    for (uint32 offset = 0; offset < 6; offset += 2)
    {
        uint32 position = r_u16(effect + 0x58 + offset);
        uint32 change = r_u16(frame + 0x18 + offset);
        uint32 value = (sint32)angle < 0 ? position - change : position + change;
        w_u16(effect + 0x58 + offset, value);
    }
    w_u32(duplicate + 0xec, 0);
    component = r_u16(object + 0x6e);
    w_u16(duplicate + 0x6e, component);
    component = r_u16(object + 0x5e);
    w_u16(duplicate + 0x5e, component);
    uint32 source = object + 0x58;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 low_lane = source & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
    uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
    uint32 packed = (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    component = (uint32)r_s16(object + 0x5c);
    uint32 destination = duplicate + 0x58;
    high_address = destination + 3;
    high_lane = high_address & 3;
    low_lane = destination & 3;
    w_masked_u32(high_address & 0xfffffffc, packed >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
    w_masked_u32(destination & 0xfffffffc, packed << (low_lane * 8), (15u << low_lane) & 15);
    w_u16(duplicate + 0x5c, component);
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
        destination = duplicate + 0x60 + (word << 2);
        high_address = destination + 3;
        high_lane = high_address & 3;
        low_lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15u << low_lane) & 15);
    }
    component = r_u16(object + 0xce);
    w_u16(duplicate + 0xce, component);
    flags = r_u32(object + 0x18);
    if ((flags & 0x10) != 0)
    {
        flags = r_u32(duplicate + 0x18);
        w_u32(duplicate + 0x18, flags | 0x10);
    }
    G3Function8005AAAC(effect, parameters);
    G3Function8005AB48(effect);
    flags = r_u32(duplicate + 0x10);
    w_u32(duplicate + 0x10, flags | 0x40000000);
    flags = r_u32(object + 0x10);
    w_u32(object + 0x10, flags | 0x40000000);
    flags = r_u32(duplicate + 0x18);
    w_u32(duplicate + 0x18, flags | 1);
    flags = r_u32(object + 0x18);
    w_u32(object + 0x18, flags | 1);
    w_u32(duplicate + 0xf8, data + 0x20);
    w_u32(object + 0xf8, data + 0x30);
    w_u32(resource_copy + 0x1ec, effect);
    source = r_u32(duplicate + 0x1c);
    destination = resource_copy;
    uint32 end = source + 0x40;
    do
    {
        uint32 block[4];
        for (uint32 word = 0; word < 4; ++word)
            block[word] = r_u32(source + (word << 2));
        for (uint32 word = 0; word < 4; ++word)
            w_u32(destination + (word << 2), block[word]);
        source += 0x10;
        destination += 0x10;
    } while (source != end);
    uint32 last = r_u32(source);
    w_u32(destination, last);
    resource = r_u32(duplicate + 0x1c);
    uint32 extra = r_u32(resource + 0x40);
    uint32 count = (uint32)r_s16(resource + 8);
    if (extra != 0)
        count += (uint32)r_s16(extra);
    uint32 model_copy = resource_copy + 0x44;
    source = r_u32(resource + 0x0c);
    uint32 copy_bytes = count << 2;
    if (model_copy != 0 && (sint32)copy_bytes > 0)
        memcpy(psx_addr(model_copy, copy_bytes), psx_addr(source, copy_bytes), copy_bytes);
    resource = r_u32(duplicate + 0x1c);
    sint32 animation_count = r_s16(resource + 0x0a);
    if (animation_count > 0)
    {
        source = r_u32(resource + 0x10);
        uint32 animation_copy = resource_copy + 0x84;
        uint32 animation_bytes = (uint32)animation_count << 2;
        if (animation_copy != 0)
            memcpy(psx_addr(animation_copy, animation_bytes), psx_addr(source, animation_bytes), animation_bytes);
    }
    w_u32(duplicate + 0x1c, resource_copy);
    w_u32(resource_copy + 0x0c, model_copy);
    resource = r_u32(duplicate + 0x1c);
    animation_count = r_s16(resource + 0x0a);
    uint32 animations = animation_count > 0 ? resource_copy + 0x84 : 0;
    w_u32(resource + 0x10, animations);
    return effect;
}

uint32 G3Function8005A544(uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005A544, "SLUS_008.06");
    uint32 player = r_u32(0x8009efc4);
    uint32 frame = guest_stack - 0x20;
    uint32 player_data = r_u32(player + 0x24);
    uint32 resource_copy = r_u32(player_data + 0xb4);
    uint32 effect = r_u32(resource_copy + 0x1ec);
    if (effect == 0)
        return player_data;
    uint32 owner = r_u32(effect + 0x104);
    uint32 flags = r_u32(owner + 0x10);
    w_u32(owner + 0x10, flags & 0xbfffffff);
    owner = r_u32(effect + 0x104);
    flags = r_u32(owner + 0x18);
    w_u32(owner + 0x18, flags & 0xfffffffe);
    owner = r_u32(effect + 0x104);
    w_u32(owner + 0xf8, 0);
    uint32 duplicate = r_u32(effect + 0x108);
    G3Function8002A9AC(duplicate, frame);
    flags = r_u32(effect + 0x10);
    w_u32(effect + 0x10, flags & 0xfffffbff);
    uint32 result = G3Function8002A9AC(effect, frame);
    w_u32(resource_copy + 0x1ec, 0);
    return result;
}

uint32 G3Function8005AAAC(uint32 object, uint32 vector)
{
    FUNCTION_MARKER(0x8005AAAC, "SLUS_008.06");
    uint32 result = 0;
    for (uint32 offset = 0; offset < 6; offset += 2)
    {
        result = (uint32)r_s16(object + 0x10c);
        uint32 component = r_u16(vector + offset);
        if ((sint32)result <= 0)
            component = 0u - component;
        w_u16(object + 0x12c + offset, component);
    }
    for (uint32 offset = 0; offset < 6; offset += 2)
    {
        result = (uint32)r_s16(object + 0x10c);
        uint32 component = r_u16(vector + offset);
        if ((sint32)result > 0)
            component = 0u - component;
        w_u16(object + 0x13c + offset, component);
    }
    return result;
}

uint32 G3Function8005AB48(uint32 object)
{
    FUNCTION_MARKER(0x8005AB48, "SLUS_008.06");
    uint32 component = 0;
    for (uint32 offset = 0; offset < 6; offset += 2)
    {
        component = r_u16(object + 0x58 + offset);
        w_u16(object + 0x134 + offset, component);
        w_u16(object + 0x124 + offset, component);
    }
    return component;
}

uint32 G3Function80017EE0(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80017EE0, "SLUS_008.06");
    uint32 placement = r_u32(command + 8);
    if (placement != 0 && r_u32(placement + 0x20) == 0)
        return 3;
    uint32 game = r_u32(0x80097048);
    uint32 value = r_u32(command + 4);
    w_u32(game + 0x67c4, value);
    value = r_u32(command + 4);
    if ((sint32)value < 0)
        value = 0u - value;
    w_u32(game + 0x67c8, value);
    placement = r_u32(command + 8);
    w_u32(game + 0x68bc, placement);
    return 3;
}

uint32 G3Function80017F40(uint32 context, uint32 command, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80017F40, "SLUS_008.06");
    uint32 player = r_u32(0x8009efc4);
    uint32 frame = guest_stack - 0x20;
    uint32 target = r_u16(command + 4);
    uint32 position = r_u16(player + 0x58);
    w_u16(frame + 0x10, target - position);
    target = r_u16(command + 6);
    position = r_u16(player + 0x5a);
    w_u16(frame + 0x12, target - position);
    target = r_u16(command + 8);
    position = r_u16(player + 0x5c);
    w_u16(frame + 0x16, 0);
    w_u16(frame + 0x14, target - position);
    G3Function80015B50(context, frame + 0x10, frame);
    return 3;
}

uint32 G3Function80018280(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80018280, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 value = r_u32(command + 4);
    w_u32(game + 0x69c0, value);
    return 2;
}

uint32 G3Function80018508(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80018508, "SLUS_008.06");
    uint32 current = r_u32(command + 4);
    if (current == 0)
        return 2;
    uint32 entry = r_u32(current);
    while (entry != 0)
    {
        entry = r_u32(current);
        uint32 flags = r_u8(entry + 0x0c);
        current += 4;
        w_u8(entry + 0x0c, flags | 4);
        entry = r_u32(current);
    }
    return 2;
}

uint32 G3Function80018558(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x80018558, "SLUS_008.06");
    uint32 current = r_u32(command + 4);
    uint32 entry = r_u32(current);
    while (entry != 0)
    {
        entry = r_u32(current);
        uint32 flags = r_u8(entry + 0x0c);
        current += 4;
        w_u8(entry + 0x0c, flags & 0xfb);
        entry = r_u32(current);
    }
    return 2;
}

uint32 G3Function800185A0(uint32 context, uint32 command)
{
    FUNCTION_MARKER(0x800185A0, "SLUS_008.06");
    uint32 placement = r_u32(command + 4);
    uint32 object;
    if (placement == 0)
        object = r_u32(0x8009efc4);
    else
    {
        object = r_u32(placement + 0x20);
        if (object == 0)
            return 3;
    }
    uint32 value = r_u8(command + 8);
    w_u16(object + 0x8e, value);
    return 3;
}

uint32 G3Function800185E4(uint32 object, uint32 command)
{
    FUNCTION_MARKER(0x800185E4, "SLUS_008.06");
    uint32 value = r_u32(command + 4);
    w_u16(object + 0xce, (sint32)value < 0 ? 0 : value);
    return 2;
}

uint32 G3Function8001887C(uint32 object, uint32 command)
{
    FUNCTION_MARKER(0x8001887C, "SLUS_008.06");
    uint32 resource = r_u32(object + 0x1c);
    uint32 animation = r_u16(command + 4);
    uint32 key = r_u16(command + 6);
    if (resource != 0)
    {
        uint32 animations = r_u32(resource + 0x10);
        if (animations != 0)
        {
            sint32 count = r_s16(resource + 0x0a);
            sint32 index = (sint16)animation;
            if (index < count)
            {
                uint32 entry = r_u32(animations + ((uint32)index << 2));
                sint32 keys = r_s16(entry + 2);
                if ((sint16)key < keys)
                {
                    w_u16(object + 0x6e, animation);
                    w_u16(object + 0x5e, key);
                }
            }
        }
    }
    return 2;
}

uint32 G3Function800192D8(uint32 entries, uint32 mask)
{
    FUNCTION_MARKER(0x800192D8, "SLUS_008.06");
    while (entries != 0)
    {
        uint32 flags = r_u32(entries);
        if (flags == 0)
            return 0;
        if ((flags & mask) != 0)
        {
            uint32 sequence = r_u32(entries + 4);
            return sequence != 0 ? sequence + 4 : 0;
        }
        entries += 8;
    }
    return 0;
}

uint32 G3Function8001932C(uint32 mask, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8001932C, "SLUS_008.06");
    if (mask == 0)
        return 0;
    uint32 result = 0;
    uint32 placement = r_u32(object + 0x20);
    if (placement != 0)
    {
        uint32 entries = r_u32(placement + 0x28);
        if (entries != 0)
            result = G3Function800192D8(entries, mask);
    }
    if (result != 0)
        return result;
    uint32 resource = r_u32(object + 0x1c);
    uint32 flags = r_u32(resource + 0x30);
    bool search = (flags & 0x203fc000) != 0;
    if (!search)
    {
        uint32 secondary = r_u32(resource + 0x3c);
        search = (secondary & 0x200) != 0 || (flags & 0x40000000) != 0;
    }
    if (search)
    {
        resource = r_u32(object + 0x1c);
        uint32 entries = r_u32(resource + 0x38);
        if (entries != 0)
            return G3Function800192D8(entries, mask);
    }
    return 0;
}

uint32 G3Function8001E1E8(uint32 packet, uint32 mode, uint32 vertices)
{
    FUNCTION_MARKER(0x8001E1E8, "SLUS_008.06");
    uint32 draw_mode = (mode << 5) | 0xe1000208;
    w_u32(packet, 0x06000000);
    w_u32(packet + 4, draw_mode);
    w_u8(packet + 0x0b, 0x2a);
    w_u8(packet + 0x0a, 0);
    w_u8(packet + 9, 0);
    w_u8(packet + 8, 0);
    uint32 coordinate = r_u16(vertices);
    w_u16(packet + 0x14, coordinate);
    w_u16(packet + 0x0c, coordinate);
    coordinate = r_u16(vertices + 2);
    w_u16(packet + 0x12, coordinate);
    w_u16(packet + 0x0e, coordinate);
    coordinate = r_u16(vertices + 4);
    w_u16(packet + 0x18, coordinate);
    w_u16(packet + 0x10, coordinate);
    coordinate = r_u16(vertices + 6);
    w_u16(packet + 0x16, coordinate);
    w_u16(packet + 0x1a, coordinate);
    return coordinate;
}

uint32 G3Function80021A3C(uint32 parameters, uint32 *second_glyph, uint32 *third_glyph)
{
    FUNCTION_MARKER(0x80021A3C, "SLUS_008.06");
    uint32 index = r_u32(parameters);
    uint32 resource = r_u32(0x800986e8 + (index << 3) + 4);
    return G3Function800214EC(parameters, resource, second_glyph, third_glyph);
}

uint32 G3Function80023AAC(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80023AAC, "SLUS_008.06");
    uint32 active_game = r_u32(0x80097048);
    w_u16(active_game + 0x67f0, 0);
    w_u16(active_game + 0x67f2, 0);
    w_u16(active_game + 0x67f4, 0);
    return active_game;
}

uint32 G3Function8002A6F4(uint32 object, uint32 index, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002A6F4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 resource = r_u32(0x800986e8 + (index << 3) + 4);
    if ((sint32)index < 41 && resource != 0)
        return G3Function8002A454(object, resource, frame);
    return 0;
}

uint32 G3Function8002A930(uint32 placement, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002A930, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 list = r_u32(placement + 4);
    if (list == 0)
        return G3Function80029938(placement, frame);
    uint32 result = 0;
    uint32 remaining = r_u32(list);
    uint32 current = list + 4;
    while (remaining != 0)
    {
        uint32 entry = r_u32(current);
        if (entry != 0)
            result = G3Function80029938(entry, frame);
        remaining -= 1;
        current += 4;
    }
    return result;
}

uint32 G3Function80029938(uint32 placement, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80029938, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 flags = r_u32(placement + 0x18);
    uint32 original_resource = r_u32(placement);
    if ((flags & 0x18) != 0)
        return r_u32(placement + 0x20);
    if (original_resource == 0)
        return 0;
    uint32 game = r_u32(0x80097048);
    uint32 pool = r_u32(game + 0x5c);
    uint32 object = G3Function800290A0(pool, frame);
    if (object == 0)
        return 0;
    G3Function8002A7EC(object, original_resource, frame);
    uint32 animations = r_u32(original_resource + 0x10);
    if (animations != 0 && G3Function80030454(object, frame) == 0)
    {
        game = r_u32(0x80097048);
        pool = r_u32(game + 0x5c);
        G3Function800292F0(pool, object, frame);
        return 0;
    }
    flags = r_u32(placement + 0x18);
    w_u32(placement + 0x20, object);
    w_u32(placement + 0x18, flags | 8);
    w_u32(object + 0x20, placement);
    uint32 sequence = r_u32(placement + 0x1c);
    w_u32(object + 0x28, sequence);
    uint32 source = placement + 0x10;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 low_lane = source & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
    uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
    uint32 packed = (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    uint32 component = (uint32)r_s16(placement + 0x14);
    uint32 destination = object + 0x58;
    high_address = destination + 3;
    high_lane = high_address & 3;
    low_lane = destination & 3;
    w_masked_u32(high_address & 0xfffffffc, packed >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
    w_masked_u32(destination & 0xfffffffc, packed << (low_lane * 8), (15u << low_lane) & 15);
    w_u16(object + 0x5c, component);
    uint32 words[2];
    for (uint32 word = 0; word < 2; ++word)
    {
        source = placement + 8 + (word << 2);
        high_address = source + 3;
        high_lane = high_address & 3;
        low_lane = source & 3;
        high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
        words[word] = (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 word = 0; word < 2; ++word)
    {
        destination = object + 0x60 + (word << 2);
        high_address = destination + 3;
        high_lane = high_address & 3;
        low_lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15u << low_lane) & 15);
    }
    w_u16(object + 0x7c, 0);
    w_u16(object + 0x7a, 0);
    w_u16(object + 0x78, 0);
    w_u16(object + 0x84, 0xffff);
    w_u16(object + 0x82, 0xffff);
    w_u16(object + 0x80, 0xffff);
    for (uint32 copy = 0; copy < 2; ++copy)
    {
        source = copy == 0 ? object + 0x58 : placement + 0x10;
        destination = object + (copy == 0 ? 0x88 : 0x70);
        high_address = source + 3;
        high_lane = high_address & 3;
        low_lane = source & 3;
        high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
        packed = (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        component = (uint32)r_s16(source + 4);
        high_address = destination + 3;
        high_lane = high_address & 3;
        low_lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, packed >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, packed << (low_lane * 8), (15u << low_lane) & 15);
        w_u16(destination + 4, component);
    }
    uint32 resource = r_u32(object + 0x1c);
    uint32 models = r_u32(resource + 0x0c);
    uint32 model = r_u32(models);
    resource = r_u32(object + 0x1c);
    component = r_u16(model + 0x28);
    w_u16(object + 0xc8, component);
    models = r_u32(resource + 0x0c);
    model = r_u32(models);
    component = r_u16(model + 0x2a);
    w_u16(object + 0xca, component);
    models = r_u32(resource + 0x0c);
    model = r_u32(models);
    component = r_u16(model + 0x2c);
    w_u16(object + 0xcc, component);
    flags = r_u32(resource);
    if ((flags & 0x100) != 0)
        G3Function8002A748(object, frame);
    w_u16(object + 0x68, 0x1000);
    w_u16(object + 0x6a, 0x1000);
    w_u16(object + 0x6c, 0x1000);
    flags = r_u32(placement + 0x18);
    if ((flags & 0x200) != 0)
    {
        flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags | 0x20000000);
    }
    flags = r_u32(placement + 0x18);
    if ((flags & 0x2000) != 0)
    {
        flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags | 0x400);
    }
    resource = r_u32(placement);
    flags = r_u32(resource);
    if ((flags & 0x80) != 0)
    {
        flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags | 8);
    }
    resource = r_u32(placement);
    flags = r_u32(resource);
    if ((flags & 0x800) != 0)
    {
        flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags | 0x800);
    }
    if (G3Function8008345C(object, 0, 0) == 0)
    {
        flags = r_u32(object + 0x10);
        w_u32(object + 0x10, (flags & 0xfdffffff) | 0x100000);
    }
    component = r_u16(placement + 0x0e);
    game = r_u32(0x80097048);
    w_u16(object + 0x8e, component);
    pool = r_u32(game + 0x5c);
    G3Function8002924C(pool, object, frame);
    uint32 callback = r_u32(original_resource + 0x34);
    w_u32(object + 0xec, callback);
    resource = r_u32(placement);
    flags = r_u32(resource);
    if ((flags & 0x1000) != 0)
    {
        game = r_u32(0x80097048);
        uint32 level = r_u32(game + 0x48);
        uint32 terrain = r_u32(level);
        uint32 axis = 0;
        do
        {
            uint32 extent = 0x8009d84c + (axis << 3);
            for (uint32 offset = 0; offset < 6; offset += 2)
            {
                uint32 position = r_u16(object + 0x58 + offset);
                uint32 radius = r_u16(extent + offset);
                w_u16(frame + 0x10 + offset, position - radius);
            }
            for (uint32 offset = 0; offset < 6; offset += 2)
            {
                uint32 position = r_u16(object + 0x58 + offset);
                uint32 radius = r_u16(extent + offset);
                w_u16(frame + 0x18 + offset, position + radius);
            }
            for (uint32 word = 0; word < 2; ++word)
            {
                source = frame + 0x18 + (word << 2);
                high_address = source + 3;
                high_lane = high_address & 3;
                low_lane = source & 3;
                high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
                low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
                words[word] = (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
            }
            for (uint32 word = 0; word < 2; ++word)
            {
                destination = frame + 0x20 + (word << 2);
                high_address = destination + 3;
                high_lane = high_address & 3;
                low_lane = destination & 3;
                w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
                w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15u << low_lane) & 15);
            }
            G3Function800622EC(terrain, frame + 0x18, frame + 0x10, object, frame);
            if (r_u32(object + 0xf8) != 0)
                break;
            G3Function800622EC(terrain, frame + 0x10, frame + 0x20, object, frame);
            if (r_u32(object + 0xf8) != 0)
                break;
            axis += 1;
        } while (axis < 3);
    }
    G3Function8003A9F8(object, frame);
    flags = r_u32(original_resource + 0x30);
    if ((flags & 0x20000000) != 0)
        G3Function800193F8(0x20000000, object, frame);
    return object;
}

uint32 G3Function8002B428(uint32 level, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002B428, "SLUS_008.06");
    uint32 color = r_u32(level + 4);
    uint32 red = r_u8(color);
    uint32 green = r_u8(color + 1);
    uint32 blue = r_u8(color + 2);
    SetBackColor((sint32)(red >> 1), (sint32)(green >> 1), (sint32)(blue >> 1));
    uint32 high_address = color + 3;
    uint32 high_lane = high_address & 3;
    uint32 low_lane = color & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
    uint32 low = r_masked_u32(color & 0xfffffffc, (15u << low_lane) & 15);
    uint32 packed = (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    w_masked_u32(0x800a0d38, packed, 15);
    w_masked_u32(0x800a0d38, packed, 15);
    return packed;
}

uint32 G3Function80045C44(uint32 destination, uint32 source, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80045C44, "SLUS_008.06");
    uint32 first = r_u32(source + 4);
    while (first != 0)
    {
        uint32 node = r_u32(source + 4);
        G3Function80045BA0(node);
        G3Function80045B74(destination, node);
        first = r_u32(source + 4);
    }
    return 0;
}

uint32 G3Function8002BD74(uint32 camera, uint32 scene, uint32 active, uint32 inactive, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002BD74, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    G3FillMemory80086094(frame + 0x10, 0, 8);
    uint32 node = r_u32(active + 4);
    while (node != 0)
    {
        uint32 next = r_u32(node + 4);
        if (G3Function8003A4B8(node + 0x0c, camera, frame) == 0)
        {
            G3Function80045BA0(node);
            uint32 flags = r_u8(node + 0x0b);
            w_u8(node + 0x0b, flags & 0xef);
            if ((flags & 0x20) != 0)
                G3Function80045B74(frame + 0x10, node);
        }
        node = next;
    }
    node = r_u32(inactive + 4);
    while (node != 0)
    {
        uint32 flags = r_u8(node + 0x0b);
        uint32 next = r_u32(node + 4);
        if ((flags & 0x20) != 0 && G3Function8003A4B8(node + 0x0c, camera, frame) != 0)
        {
            G3Function80045BA0(node);
            G3Function80045B74(active, node);
            flags = r_u8(node + 0x0b);
            w_u8(node + 0x0b, flags | 0x10);
        }
        node = next;
    }
    return G3Function80045C44(inactive, frame + 0x10, frame);
}

void G3Function8002BE94(uint32 camera, uint32 scene, uint32 context, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002BE94, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    G3Function8002BD74(camera, scene, context + 0x3d0, context + 0x3c0, frame);
    G3Function8002BD74(camera, scene, context + 0x294, context + 0x284, frame);
}

uint32 G3Function8003A4B8(uint32 point, uint32 camera, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003A4B8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    for (uint32 pass = 0; pass < 2; ++pass)
    {
        uint32 matrix = camera + 0x20 + (pass << 5);
        uint32 control0 = r_u32(matrix);
        uint32 control1 = r_u32(matrix + 4);
        xport_gte_write_control(0, control0);
        xport_gte_write_control(1, control1);
        uint32 control2 = r_u32(matrix + 8);
        uint32 control3 = r_u32(matrix + 0x0c);
        uint32 control4 = r_u32(matrix + 0x10);
        xport_gte_write_control(2, control2);
        xport_gte_write_control(3, control3);
        xport_gte_write_control(4, control4);
        xport_gte_write_data(0, r_u32(point));
        xport_gte_write_data(1, r_u32(point + 4));
        xport_gte_execute(0x486012);
        uint32 output = frame + (pass << 4);
        w_u32(output, xport_gte_read_data(25));
        w_u32(output + 4, xport_gte_read_data(26));
        w_u32(output + 8, xport_gte_read_data(27));
        if (pass == 0)
        {
            uint32 x = r_u32(frame);
            uint32 origin = r_u32(camera + 0x60);
            x -= origin;
            w_u32(frame, x);
            uint32 radius = r_u16(point + 6);
            uint32 lower = 0u - radius;
            if ((sint32)lower >= (sint32)x)
                return 0;
            uint32 upper = r_u32(camera + 0x88);
            upper += radius;
            if ((sint32)x >= (sint32)upper)
                return 0;
            uint32 y = r_u32(frame + 4);
            origin = r_u32(camera + 0x64);
            if ((sint32)lower >= (sint32)(y - origin))
                return 0;
            uint32 z = r_u32(frame + 8);
            origin = r_u32(camera + 0x68);
            if ((sint32)lower >= (sint32)(z - origin))
                return 0;
        }
        else
        {
            uint32 x = r_u32(frame + 0x10);
            uint32 origin = r_u32(camera + 0x6c);
            uint32 radius = r_u16(point + 6);
            uint32 lower = 0u - radius;
            if ((sint32)lower >= (sint32)(x - origin))
                return 0;
            uint32 y = r_u32(frame + 0x14);
            origin = r_u32(camera + 0x70);
            return (sint32)lower < (sint32)(y - origin) ? 1 : 0;
        }
    }
    return 0;
}

uint32 G3Function8002CEBC(uint32 parameters, uint32 text, uint32 incoming_second_glyph, uint32 incoming_third_glyph, uint32 guest_stack, uint32 *second_glyph, uint32 *third_glyph)
{
    FUNCTION_MARKER(0x8002CEBC, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function8002CD84(parameters, text, incoming_second_glyph, incoming_third_glyph, frame, 0, 0);
    uint32 position = r_u32(parameters + 0x10);
    w_u32(parameters + 4, 6);
    w_u32(parameters + 0x0c, 45);
    w_u16(parameters + 0x14, 0);
    w_u16(parameters + 0x16, 0);
    w_u32(parameters + 0x10, position + 5);
    G3Function80021A3C(parameters, second_glyph, third_glyph);
    position = r_u32(parameters + 0x10) - 5;
    w_u32(parameters + 0x10, position);
    return position;
}

uint32 G3Function8002FAB0(uint32 vector, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002FAB0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 x = r_u32(vector);
    uint32 xx = x * x;
    uint32 y = r_u32(vector + 4);
    uint32 yy = y * y;
    uint32 z = r_u32(vector + 8);
    uint32 zz = z * z;
    uint32 squared = xx + yy + zz;
    uint32 root = G3Function8002F448(squared << 4, 4, frame);
    return (uint32)((sint32)(root + 8) >> 4);
}

uint32 G3Function8002FB10(uint32 vector)
{
    FUNCTION_MARKER(0x8002FB10, "SLUS_008.06");
    uint32 x = (uint32)r_s16(vector);
    uint32 z = (uint32)r_s16(vector + 4);
    if ((sint32)x < 0)
        x = 0u - x;
    uint32 y = (uint32)r_s16(vector + 2);
    if ((sint32)z < 0)
        z = 0u - z;
    if ((sint32)y < 0)
        y = 0u - y;
    if (x > y)
    {
        uint32 swap = x;
        x = y;
        y = swap;
    }
    if (y > z)
    {
        uint32 swap = y;
        y = z;
        z = swap;
    }
    if (x > y)
    {
        uint32 swap = x;
        x = y;
        y = swap;
    }
    uint32 weighted = z * 30 + y * 12 + x * 9;
    return (uint32)((sint32)weighted >> 5);
}

uint32 G3Function8002FD98(uint32 vector, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002FD98, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 x = (uint32)r_s16(vector);
    uint32 xx = x * x;
    uint32 y = (uint32)r_s16(vector + 2);
    uint32 yy = y * y;
    uint32 root = G3Function8002F448((xx + yy) << 4, 4, frame);
    return (uint32)((sint32)(root + 8) >> 4);
}

uint32 G3Function8003774C(uint32 object, uint32 game, uint32 event, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003774C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (event != 2)
        return 0;
    G3Function80037F5C(object, game, frame);
    return 1;
}

uint32 G3Function80038A4C(uint32 value_pointer, uint32 target, uint32 step)
{
    FUNCTION_MARKER(0x80038A4C, "SLUS_008.06");
    uint32 value = r_u16(value_pointer);
    sint32 difference = (sint16)((target - value) & 0xffff);
    sint32 signed_step = (sint16)(step & 0xffff);
    if (difference < -signed_step)
    {
        w_u16(value_pointer, value - step);
        return 0;
    }
    if (signed_step < difference)
    {
        w_u16(value_pointer, value + step);
        return 0;
    }
    w_u16(value_pointer, target);
    return 1;
}

uint32 G3Function80039DFC(uint32 placement)
{
    FUNCTION_MARKER(0x80039DFC, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    uint32 current = r_u32(level + 0x80);
    uint32 resource = r_u32(placement);
    uint32 count = 0;
    while (current != placement)
    {
        uint32 candidate = r_u32(current);
        if (candidate == resource)
            count += 1;
        current += 0x34;
    }
    return count;
}

uint32 G3Function8002AA0C(uint32 placement, uint32 camera, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002AA0C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 descriptor = r_u32(placement);
    G3FillMemory80086094(frame + 0x18, 0, 8);
    for (uint32 component = 0; component < 3; ++component)
    {
        uint32 offset = component << 1;
        uint32 position = r_u16(placement + 0x10 + offset);
        uint32 origin = r_u16(camera + offset);
        w_u16(frame + 0x18 + offset, position - origin);
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
    uint32 result = xx + yy;
    uint32 distance = result + zz;
    if (descriptor != 0)
    {
        uint32 radius = (uint32)r_s16(descriptor + 0x16);
        uint32 limit = radius * radius;
        result = (sint32)distance < (sint32)limit ? 1 : 0;
        if (result != 0)
            return G3Function8002A930(placement, frame);
    }
    return result;
}

uint32 G3Function8003A618(uint32 leaf, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003A618, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 count = (uint32)r_s16(leaf + 0x0c);
    uint32 current = r_u32(leaf + 8);
    uint32 end = current + (count << 2);
    while (current < end)
    {
        uint32 placement = r_u32(current);
        uint32 flags = r_u32(placement + 0x18);
        if ((flags & 0x98) == 0 && r_u32(placement) != 0)
        {
            uint32 component = r_u16(placement + 0x10);
            w_u16(frame + 0x10, component);
            placement = r_u32(current);
            component = r_u16(placement + 0x12);
            w_u16(frame + 0x12, component);
            placement = r_u32(current);
            component = r_u16(placement + 0x14);
            w_u16(frame + 0x14, component);
            placement = r_u32(current);
            uint32 descriptor = r_u32(placement);
            uint32 models = r_u32(descriptor + 0x0c);
            uint32 model = r_u32(models);
            uint32 camera = r_u32(0x800a158c);
            uint32 radius = r_u16(model + 0x26);
            w_u16(frame + 0x16, radius);
            if (G3Function8003A4B8(frame + 0x10, camera, frame) != 0)
            {
                placement = r_u32(current);
                camera = r_u32(0x800a158c);
                G3Function8002AA0C(placement, camera, frame);
            }
        }
        current += 4;
    }
    count = (uint32)r_s16(leaf + 0x14);
    current = r_u32(leaf + 0x10);
    end = current + (count << 2);
    while (current < end)
    {
        uint32 light = r_u32(current);
        uint32 flags = r_u8(light + 0x0b);
        if ((flags & 0x10) == 0 && (flags & 0xc0) != 0)
        {
            uint32 context = r_u32(0x800a1590);
            uint32 anchor = context + ((flags & 0x40) != 0 ? 0x294 : 0x3d0);
            G3Function80045B74(anchor, light);
            light = r_u32(current);
            flags = r_u8(light + 0x0b);
            w_u8(light + 0x0b, flags | 0x10);
        }
        current += 4;
    }
    return 0;
}

uint32 G3Function8003A7A8(uint32 root, uint32 camera, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003A7A8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x428;
    uint32 component = r_u16(camera);
    uint32 stack = frame + 0x10;
    w_u16(0x1f800018, component);
    component = r_u16(camera + 2);
    w_u32(frame + 0x10, stack);
    w_u16(0x1f80001a, component);
    component = r_u16(camera + 4);
    stack = frame + 0x14;
    w_u32(stack, root);
    w_u16(0x1f80001c, component);
    if (root == stack)
        return component;
    for (;;)
    {
        uint32 node = r_u32(stack);
        stack -= 4;
        bool visible = true;
        for (uint32 pass = 0; pass < 2; ++pass)
        {
            uint32 matrix = camera + 0x20 + (pass << 5);
            uint32 control0 = r_u32(matrix);
            uint32 control1 = r_u32(matrix + 4);
            xport_gte_write_control(0, control0);
            xport_gte_write_control(1, control1);
            uint32 control2 = r_u32(matrix + 8);
            uint32 control3 = r_u32(matrix + 0x0c);
            uint32 control4 = r_u32(matrix + 0x10);
            xport_gte_write_control(2, control2);
            xport_gte_write_control(3, control3);
            xport_gte_write_control(4, control4);
            xport_gte_write_data(0, r_u32(node));
            xport_gte_write_data(1, r_u32(node + 4));
            xport_gte_execute(0x486012);
            uint32 output = 0x1f800000 + pass * 0x0c;
            w_u32(output, xport_gte_read_data(25));
            w_u32(output + 4, xport_gte_read_data(26));
            w_u32(output + 8, xport_gte_read_data(27));
            uint32 x = r_u32(output);
            uint32 origin = r_u32(camera + 0x60 + pass * 0x0c);
            if (pass == 0)
            {
                x -= origin;
                w_u32(output, x);
                uint32 radius = r_u16(node + 6);
                uint32 lower = 0u - radius;
                if ((sint32)lower >= (sint32)x)
                {
                    visible = false;
                    break;
                }
                uint32 upper = r_u32(camera + 0x88);
                upper += radius;
                if ((sint32)x >= (sint32)upper)
                {
                    visible = false;
                    break;
                }
                uint32 y = r_u32(output + 4);
                origin = r_u32(camera + 0x64);
                if ((sint32)lower >= (sint32)(y - origin))
                {
                    visible = false;
                    break;
                }
                uint32 z = r_u32(output + 8);
                origin = r_u32(camera + 0x68);
                if ((sint32)lower >= (sint32)(z - origin))
                {
                    visible = false;
                    break;
                }
            }
            else
            {
                uint32 radius = r_u16(node + 6);
                uint32 lower = 0u - radius;
                if ((sint32)lower >= (sint32)(x - origin))
                {
                    visible = false;
                    break;
                }
                uint32 y = r_u32(output + 4);
                origin = r_u32(camera + 0x70);
                if ((sint32)lower >= (sint32)(y - origin))
                {
                    visible = false;
                    break;
                }
            }
        }
        if (visible)
        {
            uint32 flags = r_u16(node + 0x0e);
            if ((flags & 1) != 0)
            {
                uint32 child = r_u32(node + 0x18);
                stack += 4;
                w_u32(stack, child);
                child = r_u32(node + 0x14);
                stack += 4;
                w_u32(stack, child);
            }
            else
                G3Function8003A618(node, frame);
        }
        uint32 result = r_u32(stack);
        if (result == stack)
            return result;
    }
}

uint32 G3Function8003A9C0(uint32 level, uint32 camera, uint32 context, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003A9C0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 root = r_u32(level + 0x4c);
    if (root == 0)
        return 0;
    w_u32(0x800a1590, context);
    w_u32(0x800a158c, camera);
    return G3Function8003A7A8(root, camera, frame);
}

uint32 G3Function80039F5C(uint32 owner, uint32 state, uint32 request, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80039F5C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 handle = r_u32(state);
    bool create = handle == 0 && request != 0;
    if (handle != 0)
    {
        uint32 flags = r_u16(state + 0x0c);
        if ((flags & 1) != 0)
        {
            uint32 playing = G3Function8003F0A8(handle);
            create = playing == 0 && request != 0;
            if (!create)
                handle = r_u32(state);
        }
        if (!create)
        {
            if (G3Function8003F088(handle) == 0)
                return 0;
            uint32 updated;
            if (owner != 0)
            {
                uint32 radius = (uint32)r_s16(state + 6);
                uint32 pitch = (uint32)r_s16(state + 8);
                uint32 volume = (uint32)r_s16(state + 0x0a);
                w_u32(frame + 0x10, radius);
                handle = r_u32(state);
                updated = G3Function8003E7E4(owner, handle, pitch, volume, frame);
            }
            else
            {
                handle = r_u32(state);
                uint32 volume = r_u16(state + 0x0a);
                uint32 pitch = (uint32)r_s16(state + 8);
                updated = G3Function8003F22C(handle, volume, 0x40, pitch);
            }
            if (updated == 0 || request == 0)
            {
                handle = r_u32(state);
                G3Function8003F18C(handle);
                w_u32(state, 0);
            }
            return 0;
        }
    }
    if (!create)
        return 0;
    if (owner != 0)
    {
        uint32 sound = r_u16(state + 4);
        uint32 pitch = (uint32)r_s16(state + 8);
        uint32 volume = (uint32)r_s16(state + 0x0a);
        uint32 radius = (uint32)r_s16(state + 6);
        w_u32(frame + 0x10, radius);
        handle = G3Function8003E518(owner, sound, pitch, volume, frame);
    }
    else
    {
        uint32 sound = r_u16(state + 4);
        uint32 volume = r_u16(state + 0x0a);
        uint32 pitch = (uint32)r_s16(state + 8);
        handle = G3Function8003F1C4(sound, volume, 0x40, pitch);
    }
    w_u32(state, handle);
    return 1;
}

uint32 G3Function8003E7E4(uint32 object, uint32 handle, uint32 pitch, uint32 volume, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003E7E4, "SLUS_008.06");
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
    return G3Function8003F22C(handle, gain & 0xffff, pan & 0xffff, (uint32)(sint32)(sint16)pitch);
}

void G3Function800297A8(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800297A8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 pool = r_u32(game + 0x5c);
    uint32 object = r_u32(pool + 4);
    uint32 camera = r_u32(game + 0x4c);
    while (object != 0)
    {
        uint32 placement = r_u32(object + 0x20);
        bool remove = false;
        if (placement != 0)
        {
            uint32 flags = r_u32(placement + 0x18);
            if ((flags & 0x80) != 0)
                remove = (r_u32(object + 0x10) & 2) == 0;
        }
        if (!remove)
        {
            uint32 position = r_u16(object + 0x58);
            uint32 origin = r_u16(camera);
            w_u16(0x1f800000, position - origin);
            uint32 x = (uint32)r_s16(0x1f800000);
            uint32 xx = x * x;
            origin = r_u16(camera + 2);
            position = r_u16(object + 0x5a);
            uint32 difference = position - origin;
            w_u16(0x1f800002, difference);
            uint32 y = (uint32)(sint32)(sint16)difference;
            uint32 yy = y * y;
            position = r_u16(object + 0x5c);
            origin = r_u16(camera + 4);
            difference = position - origin;
            uint32 z = (uint32)(sint32)(sint16)difference;
            uint32 zz = z * z;
            w_u16(0x1f800004, difference);
            uint32 flags = r_u32(object + 0x10);
            uint32 radius;
            if ((flags & 0x200) != 0)
            {
                uint32 descriptor = r_u32(object + 0x1c);
                radius = (uint32)r_s16(descriptor + 0x1a);
            }
            else
            {
                flags = r_u32(object + 0x14);
                if ((flags & 0x80000) == 0)
                {
                    uint32 descriptor = r_u32(object + 0x1c);
                    radius = (uint32)r_s16(descriptor + 0x18);
                }
                else
                    radius = r_u32(game + 0x68d8);
            }
            uint32 limit = radius * radius;
            uint32 distance = xx + yy + zz;
            remove = (sint32)limit < (sint32)distance;
        }
        if (remove)
            G3Function800292B4(pool, object);
        object = r_u32(object + 8);
    }
    pool = r_u32(game + 0x5c);
    G3Function80029690(pool, 0, frame);
}

uint32 G3Function8002BCA8(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002BCA8, "SLUS_008.06");
    uint32 current = state + 4;
    uint32 end = state + 0x284;
    w_u32(state + 0x288, 0);
    w_u32(state + 0x284, 0);
    w_u32(state + 0x290, 0);
    w_u32(state + 0x28c, 0);
    w_u32(state + 0x298, 0);
    w_u32(state + 0x294, 0);
    while (current < end)
    {
        G3Function80045B74(state + 0x28c, current);
        current += 0x14;
    }
    current = state + 0x2a0;
    end = state + 0x3c0;
    w_u32(state + 0x3c4, 0);
    w_u32(state + 0x3c0, 0);
    w_u32(state + 0x3cc, 0);
    w_u32(state + 0x3c8, 0);
    w_u32(state + 0x3d4, 0);
    w_u32(state + 0x3d0, 0);
    while (current < end)
    {
        G3Function80045B74(state + 0x3c8, current);
        current += 0x24;
    }
    w_u32(state + 0x3d8, 0);
    return 0;
}

void G3Function80029D88(uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80029D88, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 game = r_u32(0x80097048);
    uint32 flags = r_u32(game + 0xe8);
    uint32 object = r_u32(pool + 4);
    if ((flags & 0x08000010) != 0)
        return;
    while (object != 0)
    {
        game = r_u32(0x80097048);
        uint32 player = r_u32(game + 0x50);
        uint32 placement = r_u32(object + 0x20);
        bool process = object != player;
        if (placement != 0 && (r_u32(placement + 0x18) & 0x100) != 0)
            process = false;
        flags = r_u32(object + 0x14);
        if ((flags & 4) != 0)
            process = false;
        placement = r_u32(object + 0x20);
        if (placement != 0 && (r_u32(placement + 0x18) & 0x80) != 0)
        {
            flags = r_u32(object + 0x10);
            if ((flags & 0x402) == 0)
                process = false;
        }
        if (process)
        {
            uint32 target = r_u32(object + 0xec);
            if (target != 0)
            {
                game = r_u32(0x80097048);
                G3DispatchStateEvent(target, object, game, 2, 0, frame);
            }
            uint32 descriptor = r_u32(object + 0x1c);
            flags = r_u32(descriptor + 0x30);
            if ((flags & 0x1000) != 0)
                G3Function8003D390(object, frame);
        }
        flags = r_u32(object + 0x10);
        if ((flags & 0x80) == 0)
        {
            uint32 counter = r_u16(object + 0x9e);
            w_u16(object + 0x9e, counter + 1);
        }
        uint32 descriptor = r_u32(object + 0x1c);
        if (descriptor != 0 && (r_u32(descriptor + 0x30) & 0x10000000) != 0)
            G3Function8007E100(object, frame);
        object = r_u32(object + 8);
    }
}

uint32 G3Function8007E100(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007E100, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x30;
    uint32 player = r_u32(game + 0x50);
    uint32 data = r_u32(player + 0x24);
    uint32 result = r_u32(data + 0x230);
    if (result != 0)
        return result;
    player = r_u32(0x8009efc4);
    for (uint32 component = 0; component < 3; ++component)
    {
        uint32 offset = component << 1;
        uint32 position = r_u16(object + 0x58 + offset);
        uint32 origin = r_u16(player + 0x58 + offset);
        w_u16(frame + 0x18 + offset, position - origin);
    }
    uint32 source = frame + 0x18;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 low_lane = source & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
    uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
    uint32 packed = (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    uint32 z = (uint32)r_s16(frame + 0x1c);
    uint32 destination = frame + 0x10;
    high_address = destination + 3;
    high_lane = high_address & 3;
    low_lane = destination & 3;
    w_masked_u32(high_address & 0xfffffffc, packed >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
    w_masked_u32(destination & 0xfffffffc, packed << (low_lane * 8), (15u << low_lane) & 15);
    w_u16(frame + 0x14, z);
    uint32 distance = G3Function8002FB10(frame + 0x10);
    result = (sint32)distance < 2000 ? 1 : 0;
    if (result != 0)
    {
        w_u32(data + 0x230, object);
        w_u16(data + 0x1b6, 0);
        w_u16(data + 0x22a, 0);
    }
    return result;
}

uint32 G3Function8006F25C(uint32 request)
{
    FUNCTION_MARKER(0x8006F25C, "SLUS_008.06");
    uint32 player = r_u32(0x8009efc4);
    uint32 data = r_u32(player + 0x24);
    uint32 end = request + 0x20;
    uint32 destination = data + 0x13c;
    do
    {
        uint32 word0 = r_u32(request);
        uint32 word1 = r_u32(request + 4);
        uint32 word2 = r_u32(request + 8);
        uint32 word3 = r_u32(request + 0x0c);
        w_u32(destination, word0);
        w_u32(destination + 4, word1);
        w_u32(destination + 8, word2);
        w_u32(destination + 0x0c, word3);
        request += 0x10;
        destination += 0x10;
    } while (request != end);
    uint32 word0 = r_u32(request);
    uint32 word1 = r_u32(request + 4);
    uint32 word2 = r_u32(request + 8);
    w_u32(destination, word0);
    w_u32(destination + 4, word1);
    w_u32(destination + 8, word2);
    return destination;
}

void G3Function8002B9D4(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002B9D4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x58;
    uint32 pool = r_u32(game + 0x5c);
    uint32 object = r_u32(pool + 4);
    while (object != 0)
    {
        uint32 flags = r_u32(object + 0x14);
        bool probe = false;
        if ((flags & 0x40) != 0)
        {
            flags = r_u32(object + 0x10);
            probe = (flags & 0x200) != 0 && (flags & 0x18000000) != 0x08000000;
        }
        if (!probe)
        {
            uint32 player = r_u32(0x8009efc4);
            if (object == player && r_u32(object + 0x150) != 7)
                probe = (r_u32(object + 0x128) & 0x1000) == 0;
        }
        if (probe)
        {
            flags = r_u32(object + 0x10);
            if ((flags & 0x10000000) != 0)
            {
                for (uint32 component = 0; component < 3; ++component)
                {
                    uint32 matrix = r_u32(object + 0x30);
                    uint32 position = r_u16(matrix + 0x34 + (component << 2));
                    w_u16(frame + 0x40 + (component << 1), position);
                    w_u16(frame + 0x48 + (component << 1), position);
                }
            }
            else
            {
                for (uint32 point = 0; point < 2; ++point)
                {
                    uint32 source = object + 0x58;
                    uint32 high_address = source + 3;
                    uint32 high_lane = high_address & 3;
                    uint32 low_lane = source & 3;
                    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
                    uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
                    uint32 packed = (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
                    uint32 z = (uint32)r_s16(object + 0x5c);
                    uint32 destination = frame + 0x40 + (point << 3);
                    high_address = destination + 3;
                    high_lane = high_address & 3;
                    low_lane = destination & 3;
                    w_masked_u32(high_address & 0xfffffffc, packed >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
                    w_masked_u32(destination & 0xfffffffc, packed << (low_lane * 8), (15u << low_lane) & 15);
                    w_u16(destination + 4, z);
                }
            }
            uint32 descriptor = r_u32(object + 0x1c);
            flags = r_u32(descriptor);
            uint32 z = r_u16(frame + 0x44);
            if ((flags & 0x400) != 0)
            {
                uint32 height = r_u16(object + 0x14c);
                z -= height;
            }
            else
                z -= 0x500;
            w_u16(frame + 0x44, z);
            z = r_u16(frame + 0x4c);
            uint32 player = r_u32(0x8009efc4);
            w_u16(frame + 0x4c, z + 0x100);
            if (object == player)
            {
                w_u32(frame + 0x34, 0x1f);
                uint32 mode = r_u32(object + 0x150);
                if (mode == 2)
                    w_u16(frame + 0x4c, z + 0x200);
            }
            else
                w_u32(frame + 0x34, 0x17);
            w_u32(frame + 0x24, frame + 0x40);
            w_u32(frame + 0x38, object);
            w_u32(frame + 0x28, frame + 0x48);
            flags = r_u32(object + 0x10);
            uint32 current_game = r_u32(0x80097048);
            w_u32(object + 0x10, flags | 0x40);
            G3Function80061CA4(frame + 0x10, current_game, frame);
            flags = r_u32(object + 0x10) & 0xffffffbf;
            w_u32(object + 0x10, flags);
            uint32 type = r_u32(frame + 0x10);
            if (type != 0)
            {
                if (type == 1)
                {
                    w_u16(object + 0xb4, 0);
                    w_u16(object + 0xb6, 0);
                    w_u16(object + 0xb8, 0x1000);
                }
                else
                {
                    bool special = false;
                    if (type == 3)
                    {
                        uint32 face = r_u32(frame + 0x14);
                        special = (r_u16(face + 6) & 0x801) == 0x800;
                    }
                    if (special)
                        flags |= 0x200000;
                    else
                        flags = r_u32(object + 0x10) & 0xffdfffff;
                    w_u32(object + 0x10, flags);
                    uint32 normal = r_u16(frame + 0x1c);
                    w_u16(object + 0xb4, normal);
                    normal = r_u16(frame + 0x1e);
                    w_u16(object + 0xb6, normal);
                    normal = r_u16(frame + 0x20);
                    w_u16(object + 0xb8, normal);
                }
            }
            player = r_u32(0x8009efc4);
            if (object == player)
                G3Function8006F25C(frame + 0x10);
            uint32 source = frame + 0x40;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
            uint32 packed = (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
            z = (uint32)r_s16(frame + 0x44);
            uint32 destination = object + 0xdc;
            high_address = destination + 3;
            high_lane = high_address & 3;
            low_lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, packed >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, packed << (low_lane * 8), (15u << low_lane) & 15);
            w_u16(object + 0xe0, z);
        }
        flags = r_u32(object + 0x10) & 0xf7ffffff;
        w_u32(object + 0x10, flags);
        object = r_u32(object + 8);
    }
}

void G3Function8002AD3C(uint32 record, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002AD3C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 token = record + 0x0c;
    uint32 command = r_u32(token);
    uint32 expected = r_u32(0x8009d874);
    if (command == expected)
    {
        uint32 second = r_u32(token + 4);
        expected = r_u32(0x8009d878);
        if (second == expected)
        {
            uint32 placement = r_u32(token + 0x0c);
            if (placement != 0)
            {
                uint32 flags = r_u32(placement + 0x18);
                uint32 object = r_u32(placement + 0x20);
                w_u32(placement + 0x18, flags | 0x2000);
                if (object != 0)
                {
                    flags = r_u32(object + 0x10);
                    w_u32(object + 0x10, flags | 0x400);
                }
            }
            return;
        }
    }
    command = r_u32(token);
    expected = r_u32(0x8009d880);
    if (command == expected)
    {
        uint32 second = r_u32(token + 4);
        expected = r_u32(0x8009d884);
        if (second == expected)
        {
            uint32 placement = r_u32(token + 0x0c);
            if (placement != 0)
            {
                uint32 flags = r_u32(placement + 0x18);
                uint32 object = r_u32(placement + 0x20);
                w_u32(placement + 0x18, flags & 0xffffdfff);
                if (object != 0)
                {
                    flags = r_u32(object + 0x10);
                    w_u32(object + 0x10, flags & 0xfffffbff);
                }
            }
            return;
        }
    }
    else
        command = r_u32(token);
    expected = r_u32(0x8009d88c);
    if (command == expected)
    {
        uint32 second = r_u32(token + 4);
        expected = r_u32(0x8009d890);
        if (second == expected)
        {
            uint32 placement = r_u32(token + 0x0c);
            if (placement == 0)
                return;
            uint32 object = r_u32(placement + 0x20);
            if (object == 0)
                return;
            uint32 offset = r_u32(token + 8);
            token += offset << 2;
            command = r_u32(token);
            expected = r_u32(0x8009d898);
            if (command != expected)
                return;
            second = r_u32(token + 4);
            expected = r_u32(0x8009d89c);
            if (second != expected)
                return;
            uint32 sequence = r_u32(token + 0x0c);
            G3Function80018DE0(object, sequence + 4, frame);
            return;
        }
    }
    command = r_u32(token);
    expected = r_u32(0x8009d8a4);
    if (command == expected)
    {
        uint32 second = r_u32(token + 4);
        expected = r_u32(0x8009d8a8);
        if (second == expected)
        {
            uint32 placement = r_u32(token + 0x0c);
            if (placement != 0)
            {
                uint32 flags = r_u32(placement + 0x18);
                uint32 object = r_u32(placement + 0x20);
                w_u32(placement + 0x18, flags & 0xffffdfff);
                if (object != 0)
                {
                    flags = r_u32(object + 0x10);
                    w_u32(object + 0x10, flags & 0xfffffbff);
                    object = r_u32(placement + 0x20);
                    G3Function80038798(object, 0, 0, frame);
                }
            }
            return;
        }
    }
    else
        command = r_u32(token);
    expected = r_u32(0x8009d8b0);
    if (command == expected)
    {
        uint32 second = r_u32(token + 4);
        expected = r_u32(0x8009d8b4);
        if (second == expected)
        {
            uint32 placement = r_u32(token + 0x0c);
            if (placement != 0)
            {
                uint32 flags = r_u32(placement + 0x18);
                w_u32(placement + 0x18, flags & 0xfffffe7f);
                G3Function80029938(placement, frame);
            }
        }
    }
}
