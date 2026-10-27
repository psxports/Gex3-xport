#include "xport.h"
#include "psx.h"
#include "game_functions.h"

uint32 G3Function8007855C(uint32 contact, uint32 game)
{
    FUNCTION_MARKER(0x8007855c, "SLUS_008.06");
    uint32 owner = r_u32(contact + 0x14);
    uint32 player = r_u32(game + 0x50);
    if (owner != player)
        return 0;
    if (r_s16(contact + 6) != 1)
        return 0;
    uint32 data = r_u32(contact + 0xc);
    uint32 type = r_u8(data + 5);
    return type - 6 < 4 ? 1 : 0;
}

uint32 G3Function80076B64(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80076b64, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 source = r_u32(object + 0x28);
    uint32 contact = r_u32(object + 0xb0);
    uint32 player = r_u32(game + 0x50);
    uint32 global_player = r_u32(0x8009efc4);
    uint32 previous = r_u32(object + 0x11c);
    uint32 result = r_u32(global_player + 0x138);
    uint32 state = r_u32(player + 0x24);
    if (previous == result)
        return result;
    result = r_u32(object + 0x114);
    if (result != 0)
        return result;
    result = G3Function8007855C(contact, game);
    if (result == 0)
        return result;
    if ((r_u16(object + 0x106) & 1) == 0)
    {
        uint32 settings = r_u32(object + 0x20);
        uint32 flags = r_u32(settings + 0x18);
        w_u32(settings + 0x18, flags | 0x800);
    }
    uint32 flags = r_u32(object + 0x14);
    w_u32(object + 0x14, flags & 0xffffffef);
    w_u32(object + 0x114, 1);
    w_u16(object + 0x9e, 0);
    uint32 random = G3SdkRandom80027FA8();
    uint32 pitch = (uint32)((sint32)(((random & 0x1f) - 0xf) << 16) >> 16);
    w_u32(frame + 0x10, 2500);
    G3Function8003E518(object, 0x80d, pitch, 0x6e, frame);
    if (r_s16(object + 0x104) == 5 && r_u32(object + 0x118) == 0)
    {
        w_u32(object + 0x118, 1);
        w_u16(object + 0x110, 0);
        uint32 value = r_u32(source + 4);
        G3Function80039E48(object, game, value, frame);
    }
    if (r_u32(state + 0x17c) != 0)
    {
        uint32 owner = r_u32(contact + 0x14);
        player = r_u32(game + 0x50);
        if (owner == player)
        {
            flags = r_u32(owner + 0x128);
            w_u32(owner + 0x128, flags | 0x800);
        }
    }
    global_player = r_u32(0x8009efc4);
    result = r_u32(global_player + 0x138);
    w_u32(object + 0x11c, result);
    return result;
}

uint32 G3Function800777C0(uint32 object, uint32 game, uint32 delta, uint32 argument3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800777c0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 result = r_u32(object + 0x154);
    uint32 state = r_u32(object + 0x24);
    if (result == 0x2000000)
        return result;
    if (r_s16(delta) == 0 && r_s16(delta + 2) == 0 && r_s16(delta + 4) == 0)
        return 0;
    uint32 angle = r_u16(state + 0xc6);
    uint32 change = r_u16(delta + 6);
    angle += change;
    w_u16(state + 0xc6, angle);
    w_u16(state + 0xc8, angle);
    for (uint32 coordinate = 0; coordinate < 3; ++coordinate)
    {
        uint32 position = r_u16(object + 0x58 + (coordinate << 1));
        change = r_u16(delta + (coordinate << 1));
        w_u16(object + 0x58 + (coordinate << 1), position + change);
    }
    result = G3Function8005F3A8(object, delta);
    G3Function8005F47C(object, delta, result);
    uint32 linked = r_u32(state + 0x1fc);
    if (linked != 0)
    {
        uint32 position = r_u16(linked + 0x58);
        change = r_u16(delta);
        w_u16(linked + 0x58, position + change);
        linked = r_u32(state + 0x1fc);
        change = r_u16(delta + 2);
        position = r_u16(linked + 0x5a);
        w_u16(linked + 0x5a, position + change);
        linked = r_u32(state + 0x1fc);
        change = r_u16(delta + 4);
        position = r_u16(linked + 0x5c);
        w_u16(linked + 0x5c, position + change);
        linked = r_u32(state + 0x1fc);
        result = G3Function8005F3A8(linked, delta);
        linked = r_u32(state + 0x1fc);
        G3Function8005F47C(linked, delta, result);
    }
    uint32 camera = r_u32(game + 0x4c);
    G3Function8004D8A0(camera, 0);
    if (r_u32(object + 0x150) == 6)
    {
        uint32 current_game = r_u32(0x80097048);
        camera = r_u32(current_game + 0x4c);
        G3Function80051FE4(camera, frame);
    }
    uint32 position_xy_source = object + 0x58;
    uint32 position_xy_high_lane = (position_xy_source + 3) & 3;
    uint32 position_xy_low_lane = position_xy_source & 3;
    uint32 position_xy_high = r_masked_u32((position_xy_source + 3) & 0xfffffffc, (1 << (position_xy_high_lane + 1)) - 1);
    uint32 position_xy_low = r_masked_u32(position_xy_source & 0xfffffffc, (0xf << position_xy_low_lane) & 0xf);
    uint32 position_xy = position_xy_high << ((3 - position_xy_high_lane) * 8);
    if (position_xy_low_lane == 0)
        position_xy = position_xy_low;
    else
        position_xy = (position_xy & (0xffffffff << ((4 - position_xy_low_lane) * 8))) | (position_xy_low >> (position_xy_low_lane * 8));
    uint32 position_z = (uint32)(sint32)r_s16(object + 0x5c);
    uint32 position_xy_destination = object + 0x70;
    uint32 position_xy_store_high_lane = (position_xy_destination + 3) & 3;
    uint32 position_xy_store_low_lane = position_xy_destination & 3;
    w_masked_u32((position_xy_destination + 3) & 0xfffffffc, position_xy >> ((3 - position_xy_store_high_lane) * 8), (1 << (position_xy_store_high_lane + 1)) - 1);
    w_masked_u32(position_xy_destination & 0xfffffffc, position_xy << (position_xy_store_low_lane * 8), (0xf << position_xy_store_low_lane) & 0xf);
    w_u16(object + 0x74, position_z);
    uint32 rotation_xy_source = object + 0x60;
    uint32 rotation_xy_high_lane = (rotation_xy_source + 3) & 3;
    uint32 rotation_xy_low_lane = rotation_xy_source & 3;
    uint32 rotation_xy_high = r_masked_u32((rotation_xy_source + 3) & 0xfffffffc, (1 << (rotation_xy_high_lane + 1)) - 1);
    uint32 rotation_xy_low = r_masked_u32(rotation_xy_source & 0xfffffffc, (0xf << rotation_xy_low_lane) & 0xf);
    uint32 rotation_xy = rotation_xy_high << ((3 - rotation_xy_high_lane) * 8);
    if (rotation_xy_low_lane == 0)
        rotation_xy = rotation_xy_low;
    else
        rotation_xy = (rotation_xy & (0xffffffff << ((4 - rotation_xy_low_lane) * 8))) | (rotation_xy_low >> (rotation_xy_low_lane * 8));
    uint32 rotation_z_source = object + 0x64;
    uint32 rotation_z_high_lane = (rotation_z_source + 3) & 3;
    uint32 rotation_z_low_lane = rotation_z_source & 3;
    uint32 rotation_z_high = r_masked_u32((rotation_z_source + 3) & 0xfffffffc, (1 << (rotation_z_high_lane + 1)) - 1);
    uint32 rotation_z_low = r_masked_u32(rotation_z_source & 0xfffffffc, (0xf << rotation_z_low_lane) & 0xf);
    uint32 rotation_z = rotation_z_high << ((3 - rotation_z_high_lane) * 8);
    if (rotation_z_low_lane == 0)
        rotation_z = rotation_z_low;
    else
        rotation_z = (rotation_z & (0xffffffff << ((4 - rotation_z_low_lane) * 8))) | (rotation_z_low >> (rotation_z_low_lane * 8));
    uint32 rotation_xy_destination = object + 0x78;
    uint32 rotation_xy_store_high_lane = (rotation_xy_destination + 3) & 3;
    uint32 rotation_xy_store_low_lane = rotation_xy_destination & 3;
    w_masked_u32((rotation_xy_destination + 3) & 0xfffffffc, rotation_xy >> ((3 - rotation_xy_store_high_lane) * 8), (1 << (rotation_xy_store_high_lane + 1)) - 1);
    w_masked_u32(rotation_xy_destination & 0xfffffffc, rotation_xy << (rotation_xy_store_low_lane * 8), (0xf << rotation_xy_store_low_lane) & 0xf);
    uint32 rotation_z_destination = object + 0x7c;
    uint32 rotation_z_store_high_lane = (rotation_z_destination + 3) & 3;
    uint32 rotation_z_store_low_lane = rotation_z_destination & 3;
    w_masked_u32((rotation_z_destination + 3) & 0xfffffffc, rotation_z >> ((3 - rotation_z_store_high_lane) * 8), (1 << (rotation_z_store_high_lane + 1)) - 1);
    w_masked_u32(rotation_z_destination & 0xfffffffc, rotation_z << (rotation_z_store_low_lane * 8), (0xf << rotation_z_store_low_lane) & 0xf);
    result = r_u16(object + 0x68);
    uint32 scale_y = r_u16(object + 0x6a);
    uint32 scale_z = r_u16(object + 0x6c);
    uint32 x = r_u16(object + 0x58);
    uint32 y = r_u16(object + 0x5a);
    uint32 z = r_u16(object + 0x5c);
    w_u16(object + 0x80, result);
    w_u16(object + 0x82, scale_y);
    w_u16(object + 0x84, scale_z);
    w_u16(object + 0xdc, x);
    w_u16(object + 0xde, y);
    w_u16(object + 0xe0, z);
    w_u32(state + 0x13c, 0);
    return result;
}

uint32 G3Function800769D0(uint32 object, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800769d0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 source = r_u32(object + 0x28);
    uint32 player = r_u32(0x8009efc4);
    uint32 previous = r_u32(object + 0x11c);
    uint32 current = r_u32(player + 0x138);
    uint32 state = r_u32(player + 0x24);
    if (previous != current && r_u32(object + 0x114) == 0)
    {
        if ((r_u16(object + 0x106) & 1) == 0)
        {
            uint32 settings = r_u32(object + 0x20);
            uint32 flags = r_u32(settings + 0x18);
            w_u32(settings + 0x18, flags | 0x800);
        }
        uint32 flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags & 0xffffffef);
        w_u32(object + 0x114, 1);
        w_u16(object + 0x9e, 0);
        uint32 random = G3SdkRandom80027FA8();
        uint32 pitch = (uint32)((sint32)(((random & 0x1f) - 0xf) << 16) >> 16);
        w_u32(frame + 0x10, 2500);
        G3Function8003E518(object, 0x80d, pitch, 0x6e, frame);
        if (r_s16(object + 0x104) == 5 && r_u32(object + 0x118) == 0)
        {
            uint32 game = r_u32(0x80097048);
            w_u32(object + 0x118, 1);
            w_u16(object + 0x110, 0);
            uint32 value = r_u32(source + 4);
            G3Function80039E48(object, game, value, frame);
        }
        if (r_u32(state + 0x17c) != 0)
        {
            uint32 game = r_u32(0x80097048);
            player = r_u32(game + 0x50);
            flags = r_u32(player + 0x128);
            w_u32(player + 0x128, flags | 0x800);
        }
        player = r_u32(0x8009efc4);
        current = r_u32(player + 0x138);
        w_u32(object + 0x11c, current);
    }
    uint32 result = (uint32)(sint32)r_s16(payload + 0x10);
    if (result == 0)
        return G3Function800193F8(0x10000, object, frame);
    return result;
}

uint32 G3Function800772F0(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800772f0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 player = r_u32(game + 0x50);
    uint32 target = r_u32(object + 0xec);
    uint32 state = r_u32(player + 0x24);
    uint32 kind = 0;
    if (target != 0)
        kind = G3DispatchStateEvent(target, object, game, 0x7d4, 0, frame);
    switch (kind)
    {
    case 0:
    {
        uint32 next = r_u16(game + 0x681c);
        uint32 current_game = r_u32(0x80097048);
        ++next;
        w_u16(game + 0x681c, next);
        sint32 maximum = r_s16(current_game + 0x681e);
        uint32 raw_maximum = r_u16(current_game + 0x681e);
        if (maximum < ((sint32)(next << 16) >> 16))
            w_u16(game + 0x681c, raw_maximum);
        G3Function8007446C(game);
        if (G3Function8003F0C8(0x818) == 0)
            G3Function8003F128(0x818, 0);
        break;
    }
    case 4:
    {
        uint32 next = r_u32(game + 0x6818) + 1;
        w_u32(game + 0x6818, next);
        if (next == 100)
            w_u32(game + 0x6818, 99);
        if (G3Function8003F0C8(0x817) == 0)
            G3Function8003F128(0x817, 0);
        next = r_u32(game + 0x6818);
        G3Function80047EE4(0, next);
        break;
    }
    case 1:
    case 2:
    case 6:
    {
        if (r_u32(state + 0x174) != 0)
        {
            player = r_u32(0x8009efc4);
            G3Function800737E4(player, game, frame);
        }
        target = r_u32(object + 0xec);
        uint32 value = G3DispatchStateEvent(target, object, game, 0x7ef, 0, frame);
        player = r_u32(0x8009efc4);
        G3Function80081D14(player, game, kind, value, frame);
        w_u16(state + 0x1f2, 0x1e00);
        if (kind != 6)
        {
            uint32 sound = kind == 2 ? 0x825 : 0x80f;
            if (G3Function8003F0C8(sound) == 0)
                G3Function8003F128(sound, 0);
        }
        break;
    }
    default:
        break;
    }
    return G3Function8002A9AC(object, frame);
}

uint32 G3Function80076F60(uint32 face, uint32 object)
{
    FUNCTION_MARKER(0x80076f60, "SLUS_008.06");
    uint32 result = 0;
    uint32 owner = r_u32(object + 0x1c);
    uint32 model_index = (uint32)(sint32)r_s16(object + 0xa2);
    uint32 models = r_u32(owner + 0xc);
    uint32 model = r_u32(models + (model_index << 2));
    uint32 count = (uint32)(sint32)r_s16(model + 6);
    uint32 entry = r_u32(model + 0x18);
    uint32 end = entry + count * 24;
    uint32 index = 0;
    while (entry < end)
    {
        uint32 groups = r_u32(entry + 0x14);
        if (groups != 0)
        {
            uint32 faces = r_u32(groups + 8);
            if (faces != 0)
            {
                uint32 current = r_u32(groups + 0xc);
                uint32 limit = current + (faces << 4);
                while (current < limit)
                {
                    if (current == face)
                        result = index;
                    current += 16;
                }
            }
            faces = r_u32(groups);
            if (faces != 0)
            {
                uint32 current = r_u32(groups + 4);
                uint32 limit = current + faces * 12;
                while (current < limit)
                {
                    if (current == face)
                        result = index;
                    current += 12;
                }
            }
            faces = r_u32(groups + 0x10);
            if (faces != 0)
            {
                uint32 current = r_u32(groups + 0x14);
                uint32 limit = current + faces * 28;
                while (current < limit)
                {
                    if (current == face)
                        result = index;
                    current += 28;
                }
            }
        }
        entry += 24;
        ++index;
    }
    return result;
}

uint32 G3Function800770A0(uint32 argument0, uint32 game, uint32 object, uint32 argument3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800770a0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x68;
    uint32 contact = r_u32(object + 0xb0);
    uint32 player = r_u32(game + 0x50);
    uint32 owner = r_u32(contact + 0x14);
    if (owner != player)
        return 4;
    if (r_s16(contact + 6) != 4)
        return 4;
    uint32 state = r_u32(owner + 0x24);
    uint32 matrix = r_u32(object + 0x30);
    uint32 previous_index = r_u16(state + 0x25c);
    w_u16(state + 0x25c, 0);
    uint32 resource = r_u32(object + 0x1c);
    if (r_u32(resource + 0x10) != 0)
    {
        uint32 face = r_u32(contact + 8);
        uint32 index = G3Function80076F60(face, object);
        matrix += (uint32)((sint32)(index << 16) >> 11);
        w_u16(state + 0x25c, index);
    }
    bool update = true;
    if (r_s16(state + 0xbc) == 0)
    {
        uint32 flags = r_u32(state + 0xb8);
        if ((flags & 2) != 0 && r_u32(state + 0x1e8) == object && r_s16(state + 0x25c) == ((sint32)(previous_index << 16) >> 16) && (flags & 4) == 0)
            update = false;
    }
    if (update)
    {
        G3Function800313E0(frame + 0x10, matrix, frame);
        uint32 flags = r_u32(state + 0xb8);
        bool incremental = (flags & 2) != 0 && r_u32(state + 0x1e8) == object && r_s16(state + 0x25c) == ((sint32)(previous_index << 16) >> 16) && (flags & 0x14) == 0;
        if (incremental)
        {
            uint32 first_source = state + 0x26e;
            uint32 first_high_lane = (first_source + 3) & 3;
            uint32 first_low_lane = first_source & 3;
            uint32 first_high = r_masked_u32((first_source + 3) & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
            uint32 first_low = r_masked_u32(first_source & 0xfffffffc, (0xf << first_low_lane) & 0xf);
            uint32 first = first_high << ((3 - first_high_lane) * 8);
            if (first_low_lane == 0)
                first = first_low;
            else
                first = (first & (0xffffffff << ((4 - first_low_lane) * 8))) | (first_low >> (first_low_lane * 8));
            uint32 second_source = state + 0x272;
            uint32 second_high_lane = (second_source + 3) & 3;
            uint32 second_low_lane = second_source & 3;
            uint32 second_high = r_masked_u32((second_source + 3) & 0xfffffffc, (1 << (second_high_lane + 1)) - 1);
            uint32 second_low = r_masked_u32(second_source & 0xfffffffc, (0xf << second_low_lane) & 0xf);
            uint32 second = second_high << ((3 - second_high_lane) * 8);
            if (second_low_lane == 0)
                second = second_low;
            else
                second = (second & (0xffffffff << ((4 - second_low_lane) * 8))) | (second_low >> (second_low_lane * 8));
            uint32 first_destination = frame + 0x48;
            uint32 first_store_high_lane = (first_destination + 3) & 3;
            uint32 first_store_low_lane = first_destination & 3;
            w_masked_u32((first_destination + 3) & 0xfffffffc, first >> ((3 - first_store_high_lane) * 8), (1 << (first_store_high_lane + 1)) - 1);
            w_masked_u32(first_destination & 0xfffffffc, first << (first_store_low_lane * 8), (0xf << first_store_low_lane) & 0xf);
            uint32 second_destination = frame + 0x4c;
            uint32 second_store_high_lane = (second_destination + 3) & 3;
            uint32 second_store_low_lane = second_destination & 3;
            w_masked_u32((second_destination + 3) & 0xfffffffc, second >> ((3 - second_store_high_lane) * 8), (1 << (second_store_high_lane + 1)) - 1);
            w_masked_u32(second_destination & 0xfffffffc, second << (second_store_low_lane * 8), (0xf << second_store_low_lane) & 0xf);
            G3Function8005ADA0(frame + 0x10, frame + 0x48, frame + 0x38);
            uint32 position = r_u16(state + 0x128);
            uint32 change = r_u16(frame + 0x38);
            w_u16(state + 0x128, position + change);
            position = r_u16(state + 0x12a);
            change = r_u16(frame + 0x3c);
            w_u16(state + 0x12a, position + change);
            G3Function8005ADA0(frame + 0x10, contact + 0x18, frame + 0x38);
        }
        else
        {
            G3Function8005ADA0(frame + 0x10, contact + 0x18, frame + 0x38);
            uint32 position = r_u16(frame + 0x24);
            uint32 change = r_u16(frame + 0x38);
            w_u16(state + 0x128, position + change);
            position = r_u16(frame + 0x28);
            change = r_u16(frame + 0x3c);
            w_u16(state + 0x12a, position + change);
        }
        uint32 position = r_u16(frame + 0x2c);
        uint32 change = r_u16(frame + 0x40);
        w_u16(state + 0x12c, position + change);
        uint32 angle = r_u16(state + 0xc6);
        w_u16(state + 0xca, angle);
        G3Function800202A4(matrix, frame + 0x30, 0x15, frame);
        angle = r_u16(frame + 0x34);
        w_u16(state + 0xcc, angle);
    }
    uint32 flags = r_u32(state + 0xb8);
    w_u32(state + 0xb8, flags | 1);
    uint32 result = r_u32(object + 0x10) | 0x800000;
    w_u32(object + 0x10, result);
    w_u32(state + 0x1e8, object);
    return result;
}

uint32 G3Function80039E48(uint32 object, uint32 game, uint32 value, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80039e48, "SLUS_008.06");
    uint32 settings = r_u32(object + 0x20);
    uint32 angle = (uint32)(sint32)r_s16(settings + 0xc);
    uint32 sine = G3Function8005B470(angle);
    uint32 offset = (uint32)((sint32)(sine << 8) >> 12);
    uint32 position = r_u16(object + 0x58);
    uint32 camera = r_u32(game + 0x48);
    w_u16(camera + 0x28, position + offset);
    settings = r_u32(object + 0x20);
    angle = (uint32)(sint32)r_s16(settings + 0xc);
    uint32 cosine = G3Function8005B590(angle);
    offset = (uint32)((sint32)(cosine << 8) >> 12);
    position = r_u16(object + 0x5a);
    camera = r_u32(game + 0x48);
    w_u16(camera + 0x2a, position + offset);
    camera = r_u32(game + 0x48);
    position = r_u16(object + 0x5c);
    w_u16(camera + 0x2c, position);
    camera = r_u32(game + 0x48);
    w_u32(camera + 0x74, value);
    uint32 entries = r_u32(game + 0x68b0);
    for (uint32 index = 0; index < 100; ++index)
    {
        uint32 type = r_u8(entries + index);
        if (type == 3)
            w_u8(entries + index, 4);
    }
    return 0;
}

uint32 G3Function80077594(uint32 object, uint32 mode, uint32 game)
{
    FUNCTION_MARKER(0x80077594, "SLUS_008.06");
    uint32 player = r_u32(game + 0x50);
    uint32 flags = r_u32(object + 0x10);
    uint32 state = r_u32(player + 0x24);
    w_u32(object + 0x10, flags | 0x400);
    uint32 result = r_u32(state + 0x174);
    if (result == 0)
    {
        w_u32(state + 0x174, object);
        w_u16(state + 0x178, mode);
    }
    return result;
}

uint32 G3Function80081D14(uint32 object, uint32 game, uint32 kind, uint32 value, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80081d14, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 player = r_u32(game + 0x50);
    uint32 state = r_u32(player + 0x24);
    uint32 resource_index;
    uint32 selection;
    if (kind == 1)
    {
        resource_index = 0x21;
        selection = 1;
    }
    else if (kind == 2)
    {
        resource_index = 0x1f;
        selection = 4;
    }
    else
    {
        resource_index = 0x1b;
        selection = 3;
    }
    uint32 result = 0x800986e8 + (resource_index << 3);
    uint32 resource = r_u32(result + 4);
    if (resource == 0)
        return result;
    result = G3Function8002A454(object, resource, frame);
    if (result == 0)
        return result;
    uint32 effect = result;
    result = G3Function80077594(effect, 0, game);
    player = r_u32(game + 0x50);
    uint32 previous = (uint32)(sint32)r_s16(state + 0x262);
    G3Function80043CF8(player, previous, selection, frame, &result);
    if (value != 0)
    {
        result = r_u16(value + 2) & 2;
        if (result != 0)
        {
            result = r_u32(value + 8);
            w_u32(effect + 0x118, result);
        }
    }
    return result;
}

uint32 G3Function8007F2D0(uint32 controls, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007f2d0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    if ((r_u32(object + 0x10) & 0x100) != 0)
    {
        w_u32(controls + 0xc, 0);
        w_u32(controls + 0x10, 0);
    }
    uint32 y = r_u32(controls + 0x10);
    if (y == 0 && r_u32(controls + 0xc) == 0)
        return 0;
    uint32 x = r_u32(controls + 0xc);
    uint32 angle = G3Function8002F240(0 - y, x, frame) + 0x400;
    if ((angle << 16) == 0)
        angle = 0x1000;
    y = r_u32(controls + 0x10);
    uint32 squared_y = y * y;
    x = r_u32(controls + 0xc);
    uint32 squared_x = x * x;
    if ((sint32)(squared_y + squared_x) < 0x2400)
    {
        uint32 flags = r_u32(object + 0x128);
        w_u32(object + 0x128, flags | 0x20);
    }
    uint32 game = r_u32(0x80097048);
    uint32 flags = r_u32(game + 0x70);
    w_u32(game + 0x70, flags | 0x80000000);
    return (uint32)((sint32)(angle << 16) >> 16);
}

uint32 G3Function8004D9E0(void)
{
    FUNCTION_MARKER(0x8004d9e0, "SLUS_008.06");
    return r_u32(0x80096a80) != 0 ? 1 : 0;
}

uint32 G3Function8007F3C0(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007f3c0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 heading = 0;
    uint32 input_heading = G3Function8007F2D0(game + 0x98, object, frame);
    if ((r_u32(object + 0x154) & 0xfed0b100) != 0)
    {
        uint32 orientation = r_u32(state);
        w_u16(state + 0xbc, 0);
        w_u16(state + 0xc2, 0);
        w_u16(orientation + 2, 0);
        return orientation;
    }
    if ((input_heading << 16) != 0)
        heading = input_heading + G3Function80014F00(game);
    if (r_u32(object + 0x154) == 0x800)
    {
        uint32 scratch = r_u32(0x800a0284);
        uint32 scale = (uint32)(sint32)r_s16(state + 0x14);
        G3Function80036278(scratch + 0x14, state + 0x18e, scale, frame);
        scratch = r_u32(0x800a0284);
        w_u16(state + 0x192, 0);
        if (r_s16(scratch + 0x18) < 0xffa)
        {
            uint32 x = (uint32)(sint32)r_s16(state + 0x18e);
            uint32 y = (uint32)(sint32)r_s16(state + 0x190);
            uint32 root = G3Function8002F368((x * x + y * y) << 12, frame);
            y = (uint32)(sint32)r_s16(state + 0x190);
            x = (uint32)(sint32)r_s16(state + 0x18e);
            uint32 length = (uint32)((sint32)root >> 13);
            uint32 direction = G3Function8002F240(y, x, frame) + 0x400;
            uint32 short_heading = (uint32)((sint32)(heading << 16) >> 16);
            if (short_heading != 0)
            {
                direction = (uint32)((sint32)(direction << 16) >> 16);
                uint32 difference = G3Function80038B48(short_heading, direction);
                bool negative = (sint32)(difference << 16) < 0;
                difference = G3Function80038B48(short_heading, direction);
                if (negative)
                    difference = 0 - difference;
                uint32 angle = short_heading - 0x400;
                w_u16(state + 0x196, difference);
                uint32 sine = G3Function8005B470(angle);
                uint32 product = sine * length;
                x = r_u16(state + 0x18e);
                w_u16(state + 0x18e, x + (uint32)((sint32)product >> 12));
                uint32 cosine = G3Function8005B590(angle);
                product = cosine * length;
                y = r_u16(state + 0x190);
                w_u16(state + 0x190, y + (uint32)((sint32)product >> 12));
            }
            else
                w_u16(state + 0x196, 0);
            y = (uint32)(sint32)r_s16(state + 0x190);
            x = (uint32)(sint32)r_s16(state + 0x18e);
            heading = G3Function8002F240(y, x, frame) + 0x400;
        }
        else
            w_u16(state + 0x196, 0x1000);
    }
    uint32 motion;
    uint32 short_heading = (uint32)((sint32)(heading << 16) >> 16);
    if (short_heading != 0)
    {
        bool camera_heading = r_u32(object + 0x150) == 1;
        if (!camera_heading)
        {
            camera_heading = (r_u16(state + 0xce) & 1) == 0;
            if (!camera_heading)
            {
                motion = r_u32(object + 0x154);
                camera_heading = motion == 0x10;
            }
        }
        if (camera_heading)
        {
            r_u32(game + 0x4c);
            uint32 mode = G3Function8004D9C0();
            bool fixed_heading = mode == 1 && r_u32(object + 0x150) == 1;
            if (fixed_heading)
            {
                r_u32(game + 0x4c);
                uint32 fixed = G3Function8004D9E0();
                w_u16(state + 0xc8, (fixed != 0 ? input_heading : heading) + 0x1000);
                motion = r_u32(object + 0x154);
            }
            else
            {
                motion = r_u32(object + 0x154);
                if (motion != 0x40000)
                {
                    uint32 target = heading + 0x1000;
                    if ((r_u32(object + 0x134) & 8) != 0 && r_u32(object + 0x150) == 0 && motion == 0x40)
                        target = r_u16(state + 0xc6);
                    w_u16(state + 0xc8, target);
                    motion = r_u32(object + 0x154);
                }
            }
        }
        else
        {
            if ((motion & 0xc16) != 0)
            {
                uint32 maximum = (uint32)(sint32)r_s16(state + 0xe4);
                uint32 minimum = (uint32)(sint32)r_s16(state + 0xe2);
                uint32 addition = (uint32)(sint32)r_s16(state + 0xe6);
                uint32 factor = (uint32)(sint32)r_s16(state + 0xe8);
                uint32 product = (maximum - minimum + addition) * factor;
                uint32 step = (uint32)((sint32)(product << 16) >> 16);
                G3Function80038AA8(state + 0xc8, short_heading, step);
                uint32 target = r_u16(state + 0xc8);
                w_u16(state + 0xc8, target + 0x1000);
            }
            motion = r_u32(object + 0x154);
            if (motion == 0x80000)
            {
                w_u16(state + 0xc8, heading);
                motion = r_u32(object + 0x154);
            }
        }
    }
    else
        motion = r_u32(object + 0x154);
    if (motion != 0x40 && (r_u16(state + 0xce) & 1) != 0)
    {
        uint32 target = r_u16(state + 0xc8);
        w_u16(state + 0xc6, target);
    }
    else if ((r_u32(object + 0x134) & 8) != 0 && r_u32(object + 0x150) == 0 && r_u32(object + 0x154) == 0x40)
    {
        short_heading = (uint32)((sint32)(heading << 16) >> 16);
        if (short_heading != 0)
        {
            uint32 current = (uint32)(sint32)r_s16(state + 0xc6);
            uint32 difference = G3Function80038B48(current, short_heading);
            sint32 numerator = r_s16(state + 0x24e);
            sint32 denominator = r_s16(state + 0xe4);
            sint32 quotient = denominator != 0 ? numerator / denominator : (numerator < 0 ? 1 : -1);
            uint32 factor = (uint32)(sint32)r_s16(state + 0xe2);
            uint32 product = factor * (uint32)quotient;
            uint32 target = 0 - difference;
            sint32 limit = (sint32)(product << 16) >> 16;
            if ((sint32)(product << 16) < (sint32)(target << 16))
                target = product;
            if (((sint32)(target << 16) >> 16) < -limit)
                target = 0 - product;
            target = (uint32)((sint32)((0 - target) << 16) >> 16);
            uint32 orientation = r_u32(state);
            uint32 step = (uint32)(sint32)r_s16(state + 0x250);
            G3Function80038A4C(orientation + 2, target, step);
            step = (uint32)(sint32)r_s16(state + 0x24c);
            G3Function80038AA8(state + 0xc6, short_heading, step);
        }
        else
        {
            uint32 orientation = r_u32(state);
            uint32 step = (uint32)(sint32)r_s16(state + 0x250);
            G3Function80038A4C(orientation + 2, 0, step);
        }
    }
    else
    {
        uint32 target = (uint32)(sint32)r_s16(state + 0xc8);
        G3Function80038AA8(state + 0xc6, target, 0x80);
    }
    if ((r_u16(state + 0xce) & 1) != 0 && (r_u32(object + 0x154) & 0x1028613) == 0)
    {
        uint32 current = (uint32)(sint32)r_s16(state + 0xc6);
        short_heading = (uint32)((sint32)(heading << 16) >> 16);
        uint32 difference = G3Function80038B48(current, short_heading);
        sint32 denominator = r_s16(state + 0xe4);
        sint32 quotient = denominator != 0 ? 300 / denominator : -1;
        uint32 factor = (uint32)(sint32)r_s16(state + 0xe2);
        uint32 product = factor * (uint32)quotient;
        uint32 target = 0 - difference;
        sint32 limit = (sint32)(product << 16) >> 16;
        if ((sint32)(product << 16) < (sint32)(target << 16))
            target = product;
        if (((sint32)(target << 16) >> 16) < -limit)
            target = 0 - product;
        uint32 orientation = r_u32(state);
        w_u16(orientation + 2, 0 - target);
    }
    else if ((r_u32(object + 0x134) & 8) == 0)
    {
        uint32 orientation = r_u32(state);
        w_u16(orientation + 2, 0);
    }
    uint32 result = r_u16(state + 0xce) & 1;
    if (result != 0)
    {
        result = (uint32)(sint32)r_s16(state + 0xbc);
        if (result != 0)
        {
            result = r_u16(state + 0xc0);
            w_u16(state + 0xbe, result);
        }
        w_u16(state + 0xc0, input_heading);
    }
    w_u16(state + 0xbc, input_heading);
    w_u16(state + 0xc2, heading);
    return result;
}

uint32 G3Function80079178(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80079178, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    sint32 maximum = r_s16(state + 0x1e);
    sint32 current = r_s16(state + 0xe0);
    w_u16(state + 0x1d8, 0);
    if (current < maximum - 1)
        w_u16(object + 0x6e, 8);
    else
    {
        w_u32(object + 0x154, 4);
        w_u16(object + 0x6e, 9);
        w_u16(state + 0xc4, 0);
    }
    G3Function8007CF04(object, game, state, controls, frame);
    G3Function8007D99C(object, game, state, controls, frame);
    G3Function8007CE9C(object, game, state, controls, frame);
    G3Function8007CD24(object, game, state, controls, frame);
    G3Function8007D0D0(object, game, state, controls, frame);
    G3Function8007D190(object, game, state, controls, frame);
    G3Function8007D244(object, game, state, controls, frame);
    uint32 result = G3Function8007DB40(object, game, state, controls, frame);
    G3Function8007DB80(object, game, state, controls, frame, result);
    return G3Function8007D7A8(object, game, state, controls, frame);
}

uint32 G3Function8007CF04(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007cf04, "SLUS_008.06");
    bool stop = false;
    if ((r_u32(controls + 4) & 0x8e0) == 0)
        stop = (r_u32(controls) & 0x8000000f) == 0;
    if (!stop)
    {
        uint32 player = r_u32(0x8009efc4);
        stop = (r_u32(player + 0x10) & 0x100) != 0;
        if (!stop)
            stop = r_s16(game + 0x6786) == 4;
    }
    if (stop)
    {
        w_u16(object + 0x6e, 0);
        w_u16(object + 0x5e, 0);
        w_u32(object + 0x154, 1);
    }
    return 1;
}

uint32 G3Function8007D99C(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007d99c, "SLUS_008.06");
    if ((r_u16(state + 0xce) & 2) == 0 && (r_u32(state + 0x204) & 8) == 0)
        return 0;
    if ((r_u32(object + 0x128) & 0x8000) != 0)
        return 0x400;
    w_u32(object + 0x154, 0x400);
    w_u16(object + 0x6e, 0x18);
    return 0x18;
}

uint32 G3Function8007CE9C(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007ce9c, "SLUS_008.06");
    if ((r_u16(state + 0xce) & 1) != 0)
        return 0x40;
    w_u32(object + 0x154, 0x40);
    w_u16(object + 0x6e, 0x13);
    w_u16(object + 0x5e, 0);
    uint32 flags = r_u16(state + 0xce);
    uint32 position = r_u16(object + 0x5c);
    w_u16(state + 0x18c, position);
    if ((flags & 0x100) != 0)
        w_u16(state + 0x130, 0);
    else
    {
        uint32 value = r_u16(state + 0xe);
        w_u16(state + 0x130, value);
    }
    w_u16(state + 0xc4, 0);
    uint32 result = r_u32(object + 0x134) & 0xfffffff7;
    w_u32(object + 0x134, result);
    return result;
}

uint32 G3Function8007CD24(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007cd24, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 result = G3Function800786F0(object);
    if (result == 0)
        return result;
    if ((r_u32(controls + 4) & 0x80) != 0)
    {
        if ((r_u32(object + 0x128) & 0x200) != 0)
        {
            uint32 count = r_u16(state + 0x20);
            w_u16(state + 0xe0, count - 1);
        }
        else
        {
            sint32 current = r_s16(state + 0xe0);
            sint32 minimum = r_s16(state + 0x28);
            uint32 raw_minimum = r_u16(state + 0x28);
            if (current < minimum)
                w_u16(state + 0xe0, raw_minimum);
        }
        uint32 index = (uint32)(sint32)r_s16(state + 0xe0);
        uint32 table = r_u32(state + 0x34);
        uint32 value = r_u16(table + (index << 1));
        w_u16(state + 0xe2, value - 1);
        index = (uint32)(sint32)r_s16(state + 0xe0);
        table = r_u32(state + 0x30);
        value = (uint32)(sint32)r_s16(table + (index << 1));
        w_u32(object + 0x10c, value);
        if ((r_u32(controls) & 0x800) != 0 && r_u32(object + 0x154) == 4)
            G3Function80078790(object, game, state, frame);
        else
            G3Function80078740(object, game, state, frame);
    }
    uint32 flags = r_u32(state + 0x204);
    result = flags & 1;
    if (result != 0)
    {
        w_u32(state + 0x204, flags & 0xfffffffe);
        G3Function80078740(object, game, state, frame);
        uint32 speed = r_u32(state + 0x208);
        result = (sint32)speed < 0x100 ? 1 : 0;
        if (result != 0)
            speed = 0x100;
        w_u32(object + 0x10c, speed);
    }
    return result;
}

uint32 G3Function8007D0D0(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007d0d0, "SLUS_008.06");
    if ((r_u32(controls + 4) & 0x20) == 0)
        return 0;
    uint32 kind = r_u32(object + 0x150);
    if (kind == 0)
    {
        uint32 animation = (r_u32(object + 0x154) & 6) != 0 ? 0x1a : 0xb;
        w_u16(object + 0x6e, animation);
    }
    else if (kind == 1)
        w_u16(object + 0x6e, 0xf);
    else if (kind == 3)
        w_u16(object + 0x6e, 0x40);
    for (uint32 index = 0; index < 3; ++index)
    {
        uint32 destination = r_u32(state + 0xfc + index * 12);
        uint32 value = r_u32(state + 0xf0);
        w_u32(destination, value);
    }
    w_u32(object + 0x154, 0x10);
    uint32 result = r_u32(object + 0x128);
    w_u16(object + 0x5e, 0);
    result &= 0xfffffff7;
    uint32 count = r_u32(object + 0x138);
    result &= 0xfffeffff;
    w_u32(object + 0x128, result);
    w_u32(object + 0x138, count + 1);
    return result;
}

uint32 G3Function8007D190(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007d190, "SLUS_008.06");
    if ((r_u32(controls + 4) & 0x40) == 0 && (r_u32(object + 0x128) & 0x800) == 0)
        return 0x22;
    w_u16(object + 0x6e, 0x22);
    w_u32(object + 0x154, 0x100000);
    uint32 result = r_u32(object + 0x128);
    w_u16(object + 0x5e, 0);
    result &= 0xfffff7ff;
    w_u32(object + 0x128, result);
    w_u16(state + 0x1b6, 0);
    w_u16(state + 0x22a, 0);
    return result;
}

uint32 G3Function8007D244(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007d244, "SLUS_008.06");
    if ((r_u32(controls + 4) & 0x800) == 0)
        return 0;
    if ((r_u32(object + 0x128) & 0x8000) != 0)
        return 0x1000;
    w_u32(object + 0x154, 0x1000);
    w_u16(object + 0x6e, 0x1b);
    w_u16(object + 0x5e, 0);
    return 0x1b;
}

uint32 G3Function8007DB40(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007db40, "SLUS_008.06");
    if ((r_u16(state + 0xce) & 0x40) == 0)
        return 0;
    if ((r_u32(object + 0x128) & 0x8000) != 0)
        return 0x800;
    w_u32(object + 0x154, 0x800);
    w_u16(object + 0x6e, 0x17);
    w_u16(object + 0x5e, 0);
    return 0x17;
}

uint32 G3Function8007DB80(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack, uint32 incoming_v0)
{
    FUNCTION_MARKER(0x8007db80, "SLUS_008.06");
    uint32 face = r_u32(object + 0xf8);
    if (face == 0)
        return incoming_v0;
    uint32 result = r_u32(object + 0x18) & 1;
    if (result != 0)
        return result;
    result = r_u32(object + 0x10c);
    if ((sint32)result > 0)
        return result;
    sint32 height = r_s16(object + 0x100);
    sint32 offset = r_s16(state + 0x88);
    sint32 previous = r_s16(object + 0xe0);
    sint32 threshold = height - offset;
    if (previous >= threshold)
    {
        if (r_s16(object + 0x5c) >= threshold)
            return 0;
    }
    uint32 material = r_u16(face + 6);
    uint32 scene = r_u32(game + 0x48);
    uint32 table = r_u32(scene + 0xf0);
    uint32 flags = r_u32(table + ((material & 0x1e0) >> 1));
    if ((flags & 0x40) != 0)
        return 2;
    uint32 position = r_u16(object + 0x5c);
    w_u32(object + 0x150, 2);
    w_u32(object + 0x154, 1);
    w_u16(object + 0x6e, 0x39);
    w_u16(object + 0x5e, 0);
    w_u32(object + 0x10c, 0xffffffc4);
    w_u16(state + 0x26c, position);
    return 0xffffffc4;
}

uint32 G3Function8007D7A8(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007d7a8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 result = r_u16(state + 0xce) & 1;
    if (result == 0)
        return result;
    result = r_u32(object + 0x128) & 0x2000000;
    if (result == 0)
        return result;
    w_u32(object + 0x154, 0x10000000);
    w_u16(object + 0x6e, 0);
    w_u16(object + 0x5e, 0);
    w_u16(state + 0x1b6, 0);
    G3Function800359EC(0x118, 0, frame);
    uint32 camera = r_u32(game + 0x4c);
    return G3Function80013CDC(camera, 8, frame);
}

uint32 G3Function8007FE80(uint32 object, uint32 game, uint32 state)
{
    FUNCTION_MARKER(0x8007fe80, "SLUS_008.06");
    sint32 moving = r_s16(state + 0xbc);
    uint32 motion = r_u32(object + 0x154);
    if (moving != 0 && (motion & 0xcc400800) == 0 && r_s16(game + 0x6786) == 0)
    {
        if ((motion & 0x402) != 0 && (r_u16(state + 0xce) & 2) != 0)
        {
            uint32 table = r_u32(state + 0x34);
            w_u16(state + 0xe0, 4);
            uint32 value = r_u16(table + 8);
            w_u16(state + 0xe2, value);
        }
        else
        {
            sint32 maximum = r_s16(state + 0xe4);
            sint32 current = r_s16(state + 0xe2);
            uint32 next = r_u16(state + 0xe2) + 1;
            if (current < maximum - 1)
                w_u16(state + 0xe2, next);
            if ((r_u32(object + 0x128) & 0x20) != 0)
            {
                uint32 count = r_u16(state + 0x20);
                uint32 table = r_u32(state + 0x34);
                uint32 offset = (uint32)((sint32)(count << 16) >> 17) << 1;
                sint32 current_speed = r_s16(state + 0xe2);
                sint32 limit = r_s16(table + offset);
                uint32 raw_limit = r_u16(table + offset);
                if (current_speed + 1 >= limit)
                    w_u16(state + 0xe2, raw_limit - 1);
            }
        }
    }
    else
    {
        if ((motion & 0x17) != 0 && motion != 0x40000)
        {
            w_u16(state + 0xe2, 0);
            w_u16(state + 0xe0, 0);
        }
        else
        {
            motion = r_u32(object + 0x154);
            if (motion != 0x100)
                goto slope;
            sint32 current = r_s16(state + 0xe2);
            sint32 deceleration = r_s16(state + 0x1c);
            uint32 raw_current = r_u16(state + 0xe2);
            uint32 raw_deceleration = r_u16(state + 0x1c);
            if (deceleration < current)
                w_u16(state + 0xe2, raw_current - raw_deceleration);
            else
            {
                w_u16(state + 0xe2, 0);
                w_u16(state + 0xe0, 0);
            }
        }
    }
    motion = r_u32(object + 0x154);
slope:
    if (motion != 0x800)
        return 0x1000;
    sint32 angle = r_s16(state + 0x196);
    if (angle == 0x1000)
    {
        uint32 result = r_s16(state + 0xe2) < 3 ? 1 : 0;
        if (result == 0)
            return result;
    }
    sint32 magnitude = angle < 0 ? -angle : angle;
    if (magnitude >= 0x439)
    {
        sint32 current = r_s16(state + 0xe2);
        uint32 raw_current = r_u16(state + 0xe2);
        if (current >= 3)
        {
            uint32 result = raw_current - 1;
            if (current >= 4)
                w_u16(state + 0xe2, result);
            return result;
        }
    }
    sint32 maximum = r_s16(state + 0xe4);
    sint32 current = r_s16(state + 0xe2);
    uint32 result = r_u16(state + 0xe2) + 1;
    if (current < maximum - 1)
        w_u16(state + 0xe2, result);
    return result;
}

uint32 G3Function8007F934(uint32 object)
{
    FUNCTION_MARKER(0x8007f934, "SLUS_008.06");
    uint32 result = 0;
    for (uint32 axis = 0; axis < 2; ++axis)
    {
        uint32 velocity_address = object + 0x104 + (axis << 2);
        result = r_u32(velocity_address);
        if (result == 0)
            continue;
        uint32 position_address = object + 0x58 + (axis << 1);
        uint32 position = r_u16(position_address);
        uint32 component = r_u16(velocity_address);
        uint32 velocity = r_u32(velocity_address);
        w_u16(position_address, position + component);
        sint32 quarter = (sint32)(0 - velocity) >> 2;
        if ((sint32)velocity > 0)
        {
            result = velocity + (uint32)quarter;
            if (quarter >= 0)
                result = velocity - 1;
            w_u32(velocity_address, result);
            if ((sint32)result < 0)
                w_u32(velocity_address, 0);
        }
        else
        {
            result = velocity + (uint32)quarter;
            if (quarter <= 0)
                result = velocity + 1;
            w_u32(velocity_address, result);
            if ((sint32)result > 0)
                w_u32(velocity_address, 0);
        }
    }
    return result;
}

uint32 G3Function8007FA24(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007fa24, "SLUS_008.06");
    uint32 frame = guest_stack - 0x68;
    r_u32(game + 0x4c);
    uint32 camera_matrix = G3Function8004D9F0();
    G3FillMemory80086094(frame + 0x10, 0, 8);
    G3FillMemory80086094(frame + 0x18, 0, 8);
    uint32 index = 0;
    bool active = r_s16(state + 0xbc) != 0;
    if (!active)
    {
        active = (r_u32(object + 0x134) & 8) != 0 && r_u32(object + 0x150) == 0 && r_u32(object + 0x154) == 0x40;
        if (!active)
            active = (r_u32(object + 0x154) & 0x40900) != 0;
    }
    if (!active)
        return G3Function8007F934(object);
    if ((r_u32(object + 0x154) & 0xcdc0f000) != 0)
        return G3Function8007F934(object);
    sint32 maximum = r_s16(state + 0x254);
    uint32 raw_maximum = r_u16(state + 0x254);
    if (maximum != 0x1000)
    {
        if (maximum < r_s16(state + 0xe2))
            w_u16(state + 0xe2, raw_maximum);
        w_u16(state + 0x254, 0x1000);
    }
    uint32 flags = r_u32(state + 0x204);
    if ((flags & 2) != 0)
    {
        index = r_u16(state + 0x20c);
        uint32 table = r_u32(state + 0x34);
        w_u32(state + 0x204, flags & 0xfffffffd);
        uint32 offset = (uint32)((sint32)(index << 16) >> 15);
        uint32 address = table + offset;
        w_u16(state + 0xe0, index);
        uint32 value = r_u16(address);
        w_u16(state + 0xe2, value - 1);
    }
    else
    {
        uint32 table = r_u32(state + 0x34);
        sint32 speed = r_s16(state + 0xe2);
        sint32 limit = r_s16(table);
        if (speed >= limit)
        {
            do
            {
                ++index;
                uint32 offset = (uint32)((sint32)(index << 16) >> 15);
                limit = r_s16(table + offset);
            } while (speed >= limit);
        }
    }
    if ((r_u32(object + 0x134) & 8) != 0)
    {
        uint32 speed = r_u16(state + 0x248);
        w_u16(frame + 0x12, 0 - speed);
        uint32 target;
        if (r_s16(state + 0xbc) != 0)
        {
            uint32 first = (uint32)(sint32)r_s16(state + 0xc2);
            uint32 second = (uint32)(sint32)r_s16(state + 0xc8);
            uint32 difference = G3Function80038B48(first, second);
            bool negative = (sint32)(difference << 16) < 0;
            first = (uint32)(sint32)r_s16(state + 0xc2);
            second = (uint32)(sint32)r_s16(state + 0xc8);
            difference = G3Function80038B48(first, second);
            if (negative)
                difference = 0 - difference;
            sint32 magnitude = (sint32)(difference << 16) >> 16;
            if (magnitude < 0x200)
                target = (uint32)(sint32)r_s16(state + 0x246);
            else if (magnitude < 0x601)
                target = (uint32)(sint32)r_s16(state + 0x244);
            else
                target = (uint32)(sint32)r_s16(state + 0x242);
        }
        else
            target = (uint32)(sint32)r_s16(state + 0x244);
        uint32 step = (uint32)(sint32)r_s16(state + 0x24a);
        G3Function80038A4C(state + 0x248, target, step);
        w_u16(state + 0xe0, index);
    }
    else
    {
        uint32 table = r_u32(state + 0x2c);
        uint32 offset = (uint32)((sint32)(index << 16) >> 15);
        uint32 value = r_u16(table + offset);
        w_u16(frame + 0x12, 0 - value);
        w_u16(state + 0xe0, index);
    }
    if (r_u32(object + 0x154) == 0x40000)
    {
        sint32 speed = r_s16(frame + 0x12);
        sint32 scale = r_s16(state + 0x12);
        w_u16(frame + 0x12, (speed * scale) >> 12);
    }
    if ((r_u32(object + 0x128) & 0x40) != 0)
    {
        sint32 speed = r_s16(frame + 0x12);
        sint32 scale = r_s16(state + 0x188);
        w_u16(frame + 0x12, (speed * scale) >> 12);
    }
    if (r_s16(state + 0x1d6) != 0)
    {
        sint32 speed = r_s16(frame + 0x12);
        w_u16(frame + 0x12, (speed * 5461) >> 12);
    }
    G3Function8002F718(frame + 0x20);
    uint32 angle = (uint32)(sint32)r_s16(state + 0xc8);
    G3Function8005BB6C(angle, frame + 0x20, frame);
    MulMatrix0((MATRIX *)psx_addr(camera_matrix, 20), (MATRIX *)psx_addr(frame + 0x20, 20), (MATRIX *)psx_addr(frame + 0x20, 20));
    ApplyMatrixSV((MATRIX *)psx_addr(frame + 0x20, 20), (SVECTOR *)psx_addr(frame + 0x10, 8), (SVECTOR *)psx_addr(frame + 0x18, 6));
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 value = r_u16(frame + 0x18 + (axis << 1));
        w_u16(state + 0x26e + (axis << 1), value);
    }
    sint32 x = r_s16(object + 0x58);
    sint32 dx = r_s16(frame + 0x18);
    sint32 dz = r_s16(frame + 0x1c);
    w_u32(frame + 0x40, x);
    x += dx;
    sint32 y = r_s16(object + 0x5a);
    sint32 dy = r_s16(frame + 0x1a);
    w_u32(frame + 0x44, y);
    sint32 z = r_s16(object + 0x5c);
    y += dy;
    w_u32(frame + 0x40, x);
    w_u32(frame + 0x44, y);
    w_u32(frame + 0x48, z);
    z += dz;
    w_u32(frame + 0x48, z);
    if (x >= 30168)
        w_u32(frame + 0x40, 30167);
    if (y >= 30168)
        w_u32(frame + 0x44, 30167);
    if (z >= 30168)
        w_u32(frame + 0x48, 30167);
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 address = frame + 0x40 + (axis << 2);
        if (r_s32(address) < -30168)
            w_u32(address, 0xffff8a28);
    }
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 position = r_u16(frame + 0x40 + (axis << 2));
        w_u16(object + 0x58 + (axis << 1), position);
    }
    return G3Function8007F934(object);
}

uint32 G3Function8007F214(uint32 object)
{
    FUNCTION_MARKER(0x8007f214, "SLUS_008.06");
    uint32 owner = r_u32(object + 0x1c);
    uint32 velocity = r_u32(object + 0x10c);
    uint32 acceleration = r_u32(object + 0x118);
    uint32 data = r_u32(owner + 0x1c);
    uint32 flags = r_u32(object + 0x134);
    velocity += acceleration;
    w_u32(object + 0x10c, velocity);
    uint32 limit;
    bool clamp;
    if ((flags & 8) != 0 && r_u32(object + 0x150) == 0 && r_u32(object + 0x154) == 0x40)
    {
        limit = 0 - (uint32)(sint32)r_s16(data + 0x240);
        clamp = (sint32)velocity < (sint32)limit;
    }
    else
    {
        limit = r_u32(object + 0x124);
        velocity = r_u32(object + 0x10c);
        limit = 0 - limit;
        clamp = (sint32)velocity < (sint32)limit;
    }
    if (clamp)
        w_u32(object + 0x10c, limit);
    if ((r_u16(data + 0xce) & 4) != 0 && r_s32(object + 0x10c) < -90)
        w_u32(object + 0x10c, 0xffffffa6);
    uint32 position = r_u16(object + 0x5c);
    uint32 component = r_u16(object + 0x10c);
    uint32 result = position + component;
    w_u16(object + 0x5c, result);
    return result;
}

uint32 G3Function8007E1C8(uint32 state, uint32 yaw, uint32 pitch, uint32 yaw_step, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007e1c8, "SLUS_008.06");
    uint32 pitch_step = r_u32(guest_stack + 0x10);
    uint32 target = (uint32)((sint32)(yaw << 16) >> 16);
    uint32 step = (uint32)((sint32)(yaw_step << 16) >> 16);
    G3Function80038A4C(state + 0x1b6, target, step);
    uint32 angle = r_u16(state + 0x1b6) & 0xfff;
    w_u16(state + 0x1b6, angle);
    if (angle >= 0x800)
        w_u16(state + 0x1b6, angle - 0x1000);
    target = (uint32)((sint32)(pitch << 16) >> 16);
    step = (uint32)((sint32)(pitch_step << 16) >> 16);
    G3Function80038A4C(state + 0x22a, target, step);
    angle = r_u16(state + 0x22a) & 0xfff;
    uint32 result = angle < 0x800 ? 1 : 0;
    w_u16(state + 0x22a, angle);
    if (angle >= 0x800)
    {
        result = angle - 0x1000;
        w_u16(state + 0x22a, result);
    }
    return result;
}

uint32 G3Function8007E270(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007e270, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    uint32 linked = r_u32(state + 0x230);
    uint32 motion = r_u32(object + 0x154);
    if (linked == 0 || motion == 0x100000 || motion == 0x8000)
    {
        if (motion == 0x8000)
        {
            w_u16(state + 0x22a, 0);
            w_u16(state + 0x1b6, 0);
        }
        return 0x8000;
    }
    linked = r_u32(state + 0x230);
    uint32 position = r_u16(object + 0x58);
    uint32 target_position = r_u16(linked + 0x58);
    w_u16(frame + 0x20, target_position - position);
    linked = r_u32(state + 0x230);
    position = r_u16(object + 0x5a);
    target_position = r_u16(linked + 0x5a);
    w_u16(frame + 0x24, 0);
    w_u16(frame + 0x22, target_position - position);
    uint32 source = frame + 0x20;
    uint32 high_lane = (source + 3) & 3;
    uint32 low_lane = source & 3;
    uint32 high = r_masked_u32((source + 3) & 0xfffffffc, (1 << (high_lane + 1)) - 1);
    uint32 low = r_masked_u32(source & 0xfffffffc, (0xf << low_lane) & 0xf);
    uint32 word = high << ((3 - high_lane) * 8);
    if (low_lane == 0)
        word = low;
    else
        word = (word & (0xffffffff << ((4 - low_lane) * 8))) | (low >> (low_lane * 8));
    uint32 z = (uint32)(sint32)r_s16(frame + 0x24);
    uint32 destination = frame + 0x18;
    high_lane = (destination + 3) & 3;
    low_lane = destination & 3;
    w_masked_u32((destination + 3) & 0xfffffffc, word >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    w_masked_u32(destination & 0xfffffffc, word << (low_lane * 8), (0xf << low_lane) & 0xf);
    w_u16(frame + 0x1c, z);
    uint32 y = (uint32)(sint32)r_s16(frame + 0x1a);
    uint32 x = (uint32)(sint32)r_s16(frame + 0x18);
    uint32 angle = G3Function8002F0C4(y, x, frame);
    uint32 orientation = r_u16(object + 0x64);
    uint32 yaw = (angle - (orientation - 0x400)) & 0xfff;
    if (yaw >= 0x800)
        yaw |= 0xf000;
    uint32 length = G3Function8002FB10(frame + 0x18);
    linked = r_u32(state + 0x230);
    position = r_u16(object + 0x5c);
    target_position = r_u16(linked + 0x5c);
    z = target_position - position;
    uint32 vertical = (uint32)((sint32)(z << 16) >> 16);
    w_u16(frame + 0x1c, z);
    angle = G3Function8002F0C4(vertical, length, frame);
    uint32 pitch = angle & 0xfff;
    if (pitch >= 0x800)
        pitch |= 0xf000;
    length = G3Function8002FB10(frame + 0x18);
    if ((sint32)length >= 2501)
    {
        pitch = 0;
        yaw = 0;
    }
    w_u32(frame + 0x10, 0x20);
    yaw = (uint32)((sint32)(yaw << 16) >> 16);
    pitch = (uint32)((sint32)(pitch << 16) >> 16);
    G3Function8007E1C8(state, yaw, pitch, 0x80, frame);
    G3Function8007DEAC(object, state + 0x22a, state + 0x1b6, game, frame);
    length = G3Function8002FB10(frame + 0x18);
    uint32 result = (sint32)length < 2501 ? 1 : 0;
    if (result == 0)
    {
        result = (uint32)(sint32)r_s16(state + 0x1b6);
        if (result == 0)
        {
            result = (uint32)(sint32)r_s16(state + 0x22a);
            if (result == 0)
                w_u32(state + 0x230, 0);
        }
    }
    return result;
}

uint32 G3Function8007DB14(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007db14, "SLUS_008.06");
    if ((r_u16(state + 0xce) & 0x10) != 0)
        return 0x80000;
    w_u32(object + 0x154, 0x80000);
    w_u16(object + 0x5e, 0);
    w_u16(object + 0x6e, 0x21);
    return 0x21;
}

uint32 G3Function8007E444(uint32 object, uint32 game, uint32 state, uint32 argument3, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007e444, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    uint32 camera = r_u32(game + 0x4c);
    G3FillMemory80086094(frame + 0x28, 0, 16);
    uint32 position = (uint32)(sint32)r_s16(object + 0x58);
    uint32 target = (uint32)(sint32)r_s16(camera);
    w_u32(frame + 0x28, target - position);
    target = (uint32)(sint32)r_s16(camera + 2);
    position = (uint32)(sint32)r_s16(object + 0x5a);
    w_u32(frame + 0x2c, target - position);
    uint32 x = r_u32(frame + 0x28);
    uint32 y = r_u32(frame + 0x2c);
    uint32 z = r_u32(frame + 0x30);
    uint32 padding = r_u32(frame + 0x34);
    w_u32(frame + 0x18, x);
    w_u32(frame + 0x1c, y);
    w_u32(frame + 0x20, z);
    w_u32(frame + 0x24, padding);
    y = r_u32(frame + 0x1c);
    x = r_u32(frame + 0x18);
    uint32 angle = G3Function8002F0C4(y, x, frame);
    uint32 orientation = r_u16(object + 0x64);
    uint32 yaw = (angle - (orientation - 0x400)) & 0xfff;
    if (yaw >= 0x800)
        yaw |= 0xf000;
    w_u32(frame + 0x10, 0x800);
    yaw = (uint32)((sint32)(yaw << 16) >> 16);
    G3Function8007E1C8(state, yaw, 0, 0x80, frame);
    return G3Function8007DEAC(object, state + 0x22a, state + 0x1b6, game, frame);
}

uint32 G3Function8007A2FC(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007a2fc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 mode = r_u32(game + 0x67c4);
    if (mode != 0)
    {
        if (mode - 6 < 0x22)
        {
            uint32 player = r_u32(game + 0x50);
            return G3Function8007E444(player, game, state, 0, frame);
        }
        w_u32(frame + 0x10, 0x40);
        G3Function8007E1C8(state, 0, 0, 500, frame);
        uint32 player = r_u32(game + 0x50);
        return G3Function8007DEAC(player, state + 0x22a, state + 0x1b6, game, frame);
    }
    if (r_s16(state + 0xbc) != 0 && r_s16(game + 0x6786) == 0)
    {
        w_u32(object + 0x154, 2);
        w_u16(object + 0x6e, 8);
        w_u16(object + 0x5e, 0);
    }
    else if ((r_u32(object + 0x14) & 0x10) != 0)
    {
        uint32 animation = r_u8(state + 0x170);
        w_u16(object + 0x6e, animation);
        uint32 previous = (uint32)(sint32)r_s16(state + 0x170);
        uint32 attempts = 4;
        if (animation == previous)
        {
            while (attempts != 0)
            {
                uint32 table = r_u32(state + 0x74);
                sint32 roll = (sint32)G3SdkRandom80027FA8() % 100;
                uint32 sum = 0;
                uint32 index = 0;
                if (roll > 0)
                {
                    do
                    {
                        uint32 weight = (uint32)(sint32)r_s16(table);
                        if (weight == 0)
                            break;
                        sum += weight;
                        table += 2;
                        ++index;
                    } while ((sint32)sum < roll);
                }
                if (roll != 0)
                    --index;
                animation = index & 0xff;
                w_u16(object + 0x6e, animation);
                previous = (uint32)(sint32)r_s16(state + 0x170);
                --attempts;
                if (animation != previous)
                    break;
            }
        }
        if (attempts == 0)
        {
            animation = r_u8(state + 0x170);
            w_u16(object + 0x6e, animation);
        }
        if ((r_u32(object + 0x10) & 0x200) == 0 && r_u16(object + 0x6e) == 6)
            w_u16(object + 0x6e, 0);
        if (r_u32(0x800aa7d4) == 3)
            w_u16(object + 0x6e, 0);
        animation = r_u16(object + 0x6e);
        w_u16(state + 0x170, animation);
    }
    G3Function8007CE9C(object, game, state, controls, frame);
    G3Function8007CD24(object, game, state, controls, frame);
    G3Function8007D0D0(object, game, state, controls, frame);
    G3Function8007D190(object, game, state, controls, frame);
    G3Function8007D244(object, game, state, controls, frame);
    G3Function8007DB40(object, game, state, controls, frame);
    uint32 result = r_u32(object + 0x10) & 0x100;
    if (result == 0)
        result = G3Function8007DB14(object, game, state, controls, frame);
    G3Function8007DB80(object, game, state, controls, frame, result);
    return G3Function8007D7A8(object, game, state, controls, frame);
}

uint32 G3Function800796E0(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800796e0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 counter = r_u16(state + 0xc4) + 1;
    w_u16(state + 0xc4, counter);
    uint32 flags = r_u32(object + 0x134);
    uint32 animation = r_u16(object + 0x6e);
    if ((flags & 8) != 0)
    {
        if (animation == 0x3d)
        {
            uint32 animation_flags = r_u32(object + 0x14);
            if ((animation_flags & 0x10) != 0)
            {
                w_u32(object + 0x14, animation_flags & 0xffffffef);
                w_u16(object + 0x6e, 0x3e);
                w_u16(object + 0x5e, 0);
            }
        }
    }
    else if (animation == 0x43)
    {
        uint32 animation_flags = r_u32(object + 0x14);
        if ((animation_flags & 0x10) != 0)
        {
            w_u32(object + 0x14, animation_flags & 0xffffffef);
            w_u16(object + 0x6e, 0x13);
            w_u16(object + 0x5e, 0);
        }
    }
    else
    {
        bool reset = false;
        if ((flags & 0x200) != 0)
        {
            if (animation != 0x46)
            {
                w_u16(object + 0x6e, 0x46);
                reset = true;
            }
        }
        else if (animation == 0x46 || (animation == 0x3f && (r_u32(object + 0x14) & 0x10) != 0))
        {
            w_u16(object + 0x6e, 0x13);
            reset = true;
        }
        if (reset)
        {
            uint32 animation_flags = r_u32(object + 0x14);
            w_u16(object + 0x5e, 0);
            w_u16(object + 0xa2, 0);
            w_u32(object + 0x14, animation_flags & 0xffffffef);
        }
    }
    G3Function8007D9EC(object, game, state, controls, frame);
    G3Function8007D818(object, game, state, controls, frame);
    G3Function8007DB40(object, game, state, controls, frame);
    G3Function8007D0D0(object, game, state, controls, frame);
    if ((r_u32(object + 0x134) & 0x200) == 0)
        G3Function8007D1E8(object, game, state, controls, frame);
    G3Function8007D4EC(object, game, state, controls, frame);
    G3Function8007DC44(object, game, state, controls, frame);
    sint32 duration = r_s16(state + 0x130);
    sint32 minimum = r_s16(state + 0x0e);
    if (duration < minimum || r_u32(object + 0x154) == 0x200)
        G3Function8007CD24(object, game, state, controls, frame);
    counter = r_u16(state + 0x130) + 1;
    w_u16(state + 0x130, counter);
    G3Function8007DB80(object, game, state, controls, frame, counter);
    if (r_u32(object + 0x150) == 0 && r_u32(object + 0x154) == 0x40)
        return 0x40;
    uint32 result = r_u32(object + 0x134) & 0xfffffff7;
    w_u32(object + 0x134, result);
    w_u16(state + 0x1d8, 0);
    w_u16(object + 0xa2, 0);
    return result;
}

uint32 G3Function8007D9EC(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007d9ec, "SLUS_008.06");
    uint32 flags = r_u32(object + 0x134);
    uint32 result = flags & 1;
    if (result != 0)
        return result;
    result = r_u16(state + 0xce) & 2;
    if (result == 0)
        return result;
    sint32 x = r_s16(object + 0x58);
    uint32 matrix = r_u32(object + 0x30);
    sint32 old_x = r_s16(state + 0x134);
    sint32 matrix_x = r_s16(matrix + 2);
    uint32 product_x = (uint32)(x - old_x) * (uint32)matrix_x;
    sint32 old_y = r_s16(state + 0x136);
    sint32 y = r_s16(object + 0x5a);
    sint32 matrix_y = r_s16(matrix + 8);
    uint32 product_y = (uint32)(y - old_y) * (uint32)matrix_y;
    sint32 projection = (sint32)(product_x + product_y) >> 12;
    sint32 maximum = r_s16(state + 0x1e);
    sint32 current = r_s16(state + 0xe0);
    result = projection < -0x100 ? 1 : 0;
    if (current < maximum - 1 || result == 0)
        return result;
    sint32 timer = r_s16(state + 0x1d8);
    result = flags & 8;
    if (timer < 0x1f)
        return result;
    if (result != 0)
        return 0x08000000;
    w_u32(object + 0x154, 0x08000000);
    w_u16(object + 0x6e, 0x30);
    w_u16(object + 0x5e, 0);
    uint32 raw_x = r_u16(matrix + 2);
    sint32 normal_x = (sint32)(raw_x << 16) >> 22;
    w_u16(state + 0xd0, (uint32)normal_x);
    matrix = r_u32(object + 0x30);
    uint32 raw_y = r_u16(matrix + 8);
    raw_x = r_u16(state + 0xd0);
    w_u16(state + 0xd4, 0);
    w_u16(state + 0xdc, 0);
    w_u16(state + 0xe2, 0);
    w_u16(state + 0xe0, 0);
    sint32 normal_y = (sint32)(raw_y << 16) >> 22;
    uint32 velocity_x = 0 - (uint32)((sint32)(raw_x << 16) >> 20);
    w_u16(state + 0xd2, (uint32)normal_y);
    uint32 velocity_y = 0 - (uint32)((sint32)((uint32)normal_y << 16) >> 20);
    w_u16(state + 0xd8, velocity_x);
    result = r_u32(state);
    w_u16(state + 0xda, velocity_y);
    w_u16(result + 2, 0);
    return result;
}

uint32 G3Function8007D818(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007d818, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    if ((r_u16(state + 0xce) & 1) != 0)
    {
        w_u16(object + 0x5e, 0);
        bool active = (r_u32(controls + 4) & 0x8e0) != 0;
        if (!active)
            active = (r_u32(controls) & 0x8000000f) != 0;
        if (active)
        {
            uint32 player = r_u32(0x8009efc4);
            active = (r_u32(player + 0x10) & 0x100) == 0;
        }
        if (active || r_s16(state + 0xbc) != 0)
        {
            w_u32(object + 0x154, 2);
            w_u16(object + 0x6e, 8);
        }
        else
        {
            w_u32(object + 0x154, 0x200);
            sint32 origin = r_s16(state + 0x18c);
            sint32 z = r_s16(object + 0x5c);
            if (origin - z < 0x401)
                w_u16(object + 0x6e, 0x14);
            else
            {
                w_u16(object + 0x6e, 0x25);
                w_u32(frame + 0x10, 8);
                w_u32(frame + 0x14, 0);
                G3Function8008194C(0, 1, 0xffffffff, 0x1000, frame);
            }
        }
        uint32 pool = r_u32(0x800a27fc);
        G3Function80073560(object, pool, frame);
        uint32 result = r_u16(state + 0xce) & 0x100;
        if (result != 0)
            return result;
        sint32 x = r_s16(object + 0x58);
        sint32 old_x = r_s16(state + 0x134);
        uint32 delta_x = (uint32)(x - old_x);
        uint32 square_x = delta_x * delta_x;
        sint32 y = r_s16(object + 0x5a);
        sint32 old_y = r_s16(state + 0x136);
        uint32 delta_y = (uint32)(y - old_y);
        uint32 square_y = delta_y * delta_y;
        uint32 root = G3Function8002F368(square_x + square_y, frame);
        result = (uint32)((sint32)root >> 6);
        w_u16(state + 0x16e, result);
        return result;
    }
    sint32 z = r_s16(object + 0x5c);
    sint32 origin = r_s16(state + 0x138);
    sint32 maximum = r_s16(state + 0x132);
    sint32 delta = z - origin;
    uint32 result = maximum < delta ? 1 : 0;
    if (result != 0)
        w_u16(state + 0x132, (uint32)delta);
    return result;
}

uint32 G3Function8007D1E8(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007d1e8, "SLUS_008.06");
    uint32 result = r_u32(controls + 4) & 0x80;
    if (result == 0)
        return result;
    if ((r_u32(object + 0x128) & 0x8000) != 0)
        return 0x20;
    uint32 count = r_u32(object + 0x138);
    w_u32(object + 0x154, 0x20);
    w_u16(object + 0x6e, 0x19);
    uint32 flags = r_u32(object + 0x128);
    w_u16(object + 0x5e, 0);
    w_u32(object + 0x138, count + 1);
    result = flags & 0xfffffff7;
    w_u32(object + 0x128, result);
    return result;
}

uint32 G3Function8007D4EC(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007d4ec, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    uint32 result = r_u16(state + 0xce) & 1;
    if (result != 0)
        return result;
    result = r_u32(object + 0x128) & 0x81000000;
    if (result != 0)
        return result;
    sint32 height = r_s16(object + 0x5c);
    sint32 ground = r_s16(object + 0xe0);
    result = height - ground < 0x201 ? 1 : 0;
    if (result != 0)
        return result;
    sint32 angle = r_s16(state + 0xc6);
    uint32 sine = G3Function8005B470((uint32)(angle - 0x400));
    uint32 product = sine * 368;
    uint32 x = r_u16(object + 0x58);
    w_u16(frame + 0x10, x + (uint32)((sint32)product >> 12));
    angle = r_s16(state + 0xc6);
    uint32 cosine = G3Function8005B590((uint32)(angle - 0x400));
    product = cosine * 368;
    uint32 y = r_u16(object + 0x5a);
    w_u16(frame + 0x12, y + (uint32)((sint32)product >> 12));
    uint32 z = r_u16(object + 0x5c);
    w_u16(frame + 0x14, z);
    {
        uint32 source = frame + 0x10;
        uint32 high_lane = (source + 3) & 3;
        uint32 low_lane = source & 3;
        uint32 high = r_masked_u32((source + 3) & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (0xf << low_lane) & 0xf);
        uint32 xy = high << ((3 - high_lane) * 8);
        if (low_lane == 0)
            xy = low;
        else
            xy = (xy & (0xffffffff << ((4 - low_lane) * 8))) | (low >> (low_lane * 8));
        uint32 z = (uint32)(sint32)r_s16(source + 4);
        uint32 destination = frame + 0x18;
        uint32 store_high_lane = (destination + 3) & 3;
        uint32 store_low_lane = destination & 3;
        w_masked_u32((destination + 3) & 0xfffffffc, xy >> ((3 - store_high_lane) * 8), (1 << (store_high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, xy << (store_low_lane * 8), (0xf << store_low_lane) & 0xf);
        w_u16(destination + 4, z);
    }
    z = r_u16(frame + 0x1c);
    w_u16(frame + 0x1c, z + 0x96);
    {
        uint32 source = frame + 0x10;
        uint32 high_lane = (source + 3) & 3;
        uint32 low_lane = source & 3;
        uint32 high = r_masked_u32((source + 3) & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (0xf << low_lane) & 0xf);
        uint32 xy = high << ((3 - high_lane) * 8);
        if (low_lane == 0)
            xy = low;
        else
            xy = (xy & (0xffffffff << ((4 - low_lane) * 8))) | (low >> (low_lane * 8));
        uint32 z = (uint32)(sint32)r_s16(source + 4);
        uint32 destination = frame + 0x20;
        uint32 store_high_lane = (destination + 3) & 3;
        uint32 store_low_lane = destination & 3;
        w_masked_u32((destination + 3) & 0xfffffffc, xy >> ((3 - store_high_lane) * 8), (1 << (store_high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, xy << (store_low_lane * 8), (0xf << store_low_lane) & 0xf);
        w_u16(destination + 4, z);
    }
    result = G3Function8007D2F0(frame + 0x18, frame + 0x20, state, 1, frame);
    if (result == 0)
        return result;
    {
        uint32 source = frame + 0x20;
        uint32 high_lane = (source + 3) & 3;
        uint32 low_lane = source & 3;
        uint32 high = r_masked_u32((source + 3) & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (0xf << low_lane) & 0xf);
        uint32 xy = high << ((3 - high_lane) * 8);
        if (low_lane == 0)
            xy = low;
        else
            xy = (xy & (0xffffffff << ((4 - low_lane) * 8))) | (low >> (low_lane * 8));
        uint32 z = (uint32)(sint32)r_s16(source + 4);
        uint32 destination = frame + 0x28;
        uint32 store_high_lane = (destination + 3) & 3;
        uint32 store_low_lane = destination & 3;
        w_masked_u32((destination + 3) & 0xfffffffc, xy >> ((3 - store_high_lane) * 8), (1 << (store_high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, xy << (store_low_lane * 8), (0xf << store_low_lane) & 0xf);
        w_u16(destination + 4, z);
    }
    {
        uint32 source = object + 0x58;
        uint32 high_lane = (source + 3) & 3;
        uint32 low_lane = source & 3;
        uint32 high = r_masked_u32((source + 3) & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (0xf << low_lane) & 0xf);
        uint32 xy = high << ((3 - high_lane) * 8);
        if (low_lane == 0)
            xy = low;
        else
            xy = (xy & (0xffffffff << ((4 - low_lane) * 8))) | (low >> (low_lane * 8));
        uint32 z = (uint32)(sint32)r_s16(source + 4);
        uint32 destination = frame + 0x18;
        uint32 store_high_lane = (destination + 3) & 3;
        uint32 store_low_lane = destination & 3;
        w_masked_u32((destination + 3) & 0xfffffffc, xy >> ((3 - store_high_lane) * 8), (1 << (store_high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, xy << (store_low_lane * 8), (0xf << store_low_lane) & 0xf);
        w_u16(destination + 4, z);
    }
    z = r_u16(frame + 0x1c);
    w_u16(frame + 0x1c, z + 0x96);
    {
        uint32 source = frame + 0x10;
        uint32 high_lane = (source + 3) & 3;
        uint32 low_lane = source & 3;
        uint32 high = r_masked_u32((source + 3) & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (0xf << low_lane) & 0xf);
        uint32 xy = high << ((3 - high_lane) * 8);
        if (low_lane == 0)
            xy = low;
        else
            xy = (xy & (0xffffffff << ((4 - low_lane) * 8))) | (low >> (low_lane * 8));
        uint32 z = (uint32)(sint32)r_s16(source + 4);
        uint32 destination = frame + 0x20;
        uint32 store_high_lane = (destination + 3) & 3;
        uint32 store_low_lane = destination & 3;
        w_masked_u32((destination + 3) & 0xfffffffc, xy >> ((3 - store_high_lane) * 8), (1 << (store_high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, xy << (store_low_lane * 8), (0xf << store_low_lane) & 0xf);
        w_u16(destination + 4, z);
    }
    z = r_u16(frame + 0x24);
    w_u16(frame + 0x24, z + 0x96);
    result = G3Function8007D2F0(frame + 0x18, frame + 0x20, state, 0, frame);
    if (result != 0)
    {
        w_u32(state + 0xec, 0);
        return result;
    }
    z = r_u16(frame + 0x2c);
    w_u16(object + 0x5c, z);
    {
        uint32 source = object + 0x58;
        uint32 high_lane = (source + 3) & 3;
        uint32 low_lane = source & 3;
        uint32 high = r_masked_u32((source + 3) & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (0xf << low_lane) & 0xf);
        uint32 xy = high << ((3 - high_lane) * 8);
        if (low_lane == 0)
            xy = low;
        else
            xy = (xy & (0xffffffff << ((4 - low_lane) * 8))) | (low >> (low_lane * 8));
        uint32 z = (uint32)(sint32)r_s16(source + 4);
        uint32 destination = frame + 0x18;
        uint32 store_high_lane = (destination + 3) & 3;
        uint32 store_low_lane = destination & 3;
        w_masked_u32((destination + 3) & 0xfffffffc, xy >> ((3 - store_high_lane) * 8), (1 << (store_high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, xy << (store_low_lane * 8), (0xf << store_low_lane) & 0xf);
        w_u16(destination + 4, z);
    }
    z = r_u16(frame + 0x1c);
    w_u16(frame + 0x1c, z - 0x20);
    {
        uint32 source = frame + 0x10;
        uint32 high_lane = (source + 3) & 3;
        uint32 low_lane = source & 3;
        uint32 high = r_masked_u32((source + 3) & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (0xf << low_lane) & 0xf);
        uint32 xy = high << ((3 - high_lane) * 8);
        if (low_lane == 0)
            xy = low;
        else
            xy = (xy & (0xffffffff << ((4 - low_lane) * 8))) | (low >> (low_lane * 8));
        uint32 z = (uint32)(sint32)r_s16(source + 4);
        uint32 destination = frame + 0x20;
        uint32 store_high_lane = (destination + 3) & 3;
        uint32 store_low_lane = destination & 3;
        w_masked_u32((destination + 3) & 0xfffffffc, xy >> ((3 - store_high_lane) * 8), (1 << (store_high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, xy << (store_low_lane * 8), (0xf << store_low_lane) & 0xf);
        w_u16(destination + 4, z);
    }
    z = r_u16(object + 0x5c);
    w_u16(frame + 0x24, z - 0x20);
    result = G3Function8007D2F0(frame + 0x18, frame + 0x20, state, 0, frame);
    if (result != 0)
    {
        angle = r_s16(state + 0xc6);
        sine = G3Function8005B470((uint32)(angle - 0x400));
        product = sine * 240;
        x = r_u16(frame + 0x20);
        w_u16(object + 0x58, x - (uint32)((sint32)product >> 12));
        angle = r_s16(state + 0xc6);
        cosine = G3Function8005B590((uint32)(angle - 0x400));
        product = cosine * 240;
        y = r_u16(frame + 0x22);
        w_u16(object + 0x5a, y - (uint32)((sint32)product >> 12));
    }
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x154, 0x8000);
    w_u32(object + 0x150, 0);
    w_u16(object + 0x6e, 0x1f);
    w_u16(object + 0x5e, 0);
    w_u32(object + 0x10c, 0);
    w_u32(object + 0x118, 0);
    w_u32(object + 0x10, flags | 0x100);
    result = r_u32(state);
    w_u16(state + 0xe2, 0);
    w_u16(state + 0xe0, 0);
    w_u16(result + 2, 0);
    return result;
}
