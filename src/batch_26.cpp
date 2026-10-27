#include "xport.h"
#include "psx.h"
#include "game_functions.h"

uint32 G3Function8007DC44(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007dc44, "SLUS_008.06");
    uint32 flags = r_u32(object + 0x134);
    uint32 result = flags & 4;
    if (result == 0)
        return result;
    uint32 active = r_u32(object + 0x150);
    result = flags & 8;
    if (active != 0)
        return result;
    if (result != 0)
    {
        if ((r_u32(controls) & 0x800) != 0)
            return 0xfffffff7;
        uint32 selector = r_u32(object + 0x154);
        w_u32(object + 0x134, flags & 0xfffffff7);
        if (selector == 0x40)
            w_u16(object + 0x6e, 0x3f);
        w_u16(object + 0x5e, 0);
        w_u16(state + 0x1d8, 0);
        uint32 animation_flags = r_u32(object + 0x14);
        uint32 current_game = r_u32(0x80097048);
        w_u16(object + 0xa2, 0);
        w_u32(object + 0x14, animation_flags & 0xffffffef);
        uint32 level = r_u16(current_game + 0x68b4);
        if (level == 0x16)
            return 0x16;
        return G3Function8003F1C4(0x81f, 0x3c, 0x40, 0xffffffec);
    }
    if ((r_u32(controls) & 0x800) != 0)
    {
        w_u32(object + 0x134, flags | 8);
        sint32 index = r_s16(state + 0xe0);
        uint32 table = r_u32(state + 0x2c);
        uint32 value = r_u16(table + ((uint32)index << 1));
        w_u16(state + 0x248, value);
        w_u32(object + 0x154, 0x40);
        w_u16(object + 0x6e, 0x3d);
        uint32 animation_flags = r_u32(object + 0x14);
        uint32 model = r_u32(object + 0x1c);
        w_u16(object + 0x5e, 0);
        w_u32(object + 0x14, animation_flags & 0xffffffef);
        if (r_s16(model + 8) >= 2)
        {
            uint32 linked = r_u32(state + 0x184);
            if (linked != 0 && r_s16(linked + 8) >= 2)
                w_u16(object + 0xa2, 1);
        }
        uint32 current_game = r_u32(0x80097048);
        uint32 level = r_u16(current_game + 0x68b4);
        if (level == 0x16)
            return 0x16;
        return G3Function8003F1C4(0x81f, 0x5a, 0x40, 0);
    }
    result = r_u32(object + 0x14) & 0x10;
    if (result == 0)
        return result;
    uint32 animation = r_u16(object + 0x6e);
    if (animation < 0x3d)
        return 1;
    if (animation < 0x40)
    {
        w_u16(object + 0x6e, 0x13);
        w_u16(object + 0x5e, 0);
    }
    return 0x13;
}

uint32 G3Function800792BC(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800792bc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 timer = r_u16(state + 0x1d8);
    uint32 previous = r_u16(state + 0xc4);
    w_u16(state + 0x1d8, timer + 1);
    w_u16(state + 0xc4, previous + 1);
    if (((sint32)(previous << 16) >> 16) < 3)
    {
        w_u32(frame + 0x10, 0);
        G3Function80072D00(object, game, state, controls, frame);
    }
    sint32 maximum = r_s16(state + 0x1e);
    sint32 current = r_s16(state + 0xe0);
    if (current < maximum - 1)
    {
        w_u32(object + 0x154, 2);
        w_u16(object + 0x6e, 8);
        w_u16(object + 0x5e, 0);
    }
    else if (r_u16(object + 0x6e) == 0x10 && r_s16(object + 0x5e) >= 0x0f)
    {
        w_u16(object + 0x6e, 9);
        w_u16(object + 0x5e, 0);
    }
    if ((r_u32(object + 0x14) & 0x10) != 0)
    {
        w_u16(object + 0x6e, 9);
        uint32 angle = r_u16(state + 0xc8);
        w_u16(state + 0xc6, angle);
    }
    G3Function8007CE9C(object, game, state, controls, frame);
    G3Function8007CF70(object, game, state, controls, frame);
    G3Function8007D0D0(object, game, state, controls, frame);
    G3Function8007CD24(object, game, state, controls, frame);
    uint32 result = G3Function8007DB40(object, game, state, controls, frame);
    G3Function8007DB80(object, game, state, controls, frame, result);
    return G3Function8007D7A8(object, game, state, controls, frame);
}

uint32 G3Function8007CF70(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007cf70, "SLUS_008.06");
    sint32 current = r_s16(state + 0xe0);
    sint32 minimum = r_s16(state + 0x26);
    if (current < minimum)
        return 1;
    uint32 result = r_u32(object + 0x128) & 0x8000;
    if (result != 0)
        return result;
    bool active = (r_u32(controls + 4) & 0x8e0) != 0;
    if (!active)
        active = (r_u32(controls) & 0x8000000f) != 0;
    if (active)
    {
        uint32 player = r_u32(0x8009efc4);
        active = (r_u32(player + 0x10) & 0x100) == 0;
    }
    if (!active)
    {
        w_u32(object + 0x154, 0x100);
        w_u16(object + 0x6e, 0x11);
        w_u16(object + 0x5e, 0);
        return 0x11;
    }
    sint32 second = r_s16(state + 0xbe);
    if (second == 0)
        return 0x100;
    sint32 first = r_s16(state + 0xbc);
    if (first == 0)
        return 0x100;
    uint32 difference = G3Function80038B48((uint32)first, (uint32)second);
    bool negative = (sint32)(difference << 16) < 0;
    first = r_s16(state + 0xbc);
    second = r_s16(state + 0xbe);
    difference = G3Function80038B48((uint32)first, (uint32)second);
    sint32 signed_difference = (sint32)(difference << 16) >> 16;
    if (negative)
        signed_difference = -signed_difference;
    result = signed_difference < 0x58e ? 1 : 0;
    if (result != 0)
        return result;
    uint32 current_game = r_u32(0x80097048);
    if ((r_u32(current_game + 0x6834) & 0x10) != 0)
        return 0x800000;
    w_u32(object + 0x154, 0x800000);
    w_u16(object + 0x6e, 0x24);
    w_u16(object + 0x5e, 0);
    return 0x24;
}

uint32 G3Function80078740(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80078740, "SLUS_008.06");
    w_u32(object + 0x154, 8);
    w_u16(object + 0x6e, 0x0a);
    uint32 flags = r_u32(object + 0x14);
    w_u16(object + 0x5e, 0);
    w_u32(object + 0x14, flags & 0xffffffef);
    w_u16(state + 0xc4, 0);
    uint32 position = r_u16(object + 0x58);
    w_u16(state + 0x134, position);
    position = r_u16(object + 0x5a);
    w_u16(state + 0x136, position);
    position = r_u16(object + 0x5c);
    w_u16(state + 0x132, 0);
    w_u16(state + 0x138, position);
    return position;
}

uint32 G3Function80079430(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80079430, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 counter = r_u16(state + 0xc4);
    w_u16(state + 0xc4, counter + 1);
    if ((r_u32(object + 0x14) & 0x10) != 0 || r_s32(object + 0x10c) < 0 || (r_u32(object + 0x134) & 0x200) != 0 || (r_u32(controls) & 0x800) != 0)
    {
        uint32 model = r_u32(object + 0x1c);
        uint32 animation = r_u16(object + 0x6e);
        uint32 animations = r_u32(model + 0x10);
        uint32 data = r_u32(animations + (animation << 2));
        uint32 frames = r_u16(data + 2);
        w_u16(object + 0x5e, frames - 1);
        G3Function8007CE9C(object, game, state, controls, frame);
    }
    G3Function8007D9EC(object, game, state, controls, frame);
    if (r_s16(state + 0xc4) >= 2)
    {
        G3Function8007D818(object, game, state, controls, frame);
        G3Function8007DB40(object, game, state, controls, frame);
    }
    uint32 result = r_u32(object + 0x134) & 0x200;
    if (result != 0)
        return result;
    G3Function8007D0D0(object, game, state, controls, frame);
    result = r_s32(object + 0x10c) < 0x20 ? 1 : 0;
    if (result != 0)
        return G3Function8007D1E8(object, game, state, controls, frame);
    return result;
}

uint32 G3Function8007D2F0(uint32 start, uint32 end, uint32 state, uint32 mode, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007d2f0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    uint32 player = r_u32(0x8009efc4);
    w_u32(frame + 0x28, start);
    w_u32(frame + 0x34, 0x3f);
    w_u32(frame + 0x24, end);
    w_u32(frame + 0x38, player);
    uint32 flags = r_u32(player + 0x10);
    uint32 game = r_u32(0x80097048);
    w_u32(player + 0x10, flags | 0x40);
    G3Function80061CA4(frame + 0x10, game, frame);
    player = r_u32(0x8009efc4);
    flags = r_u32(player + 0x10);
    w_u32(player + 0x10, flags & 0xffffffbf);
    bool terrain = false;
    if (mode != 0)
    {
        if (r_s16(frame + 0x20) < 0xfc3)
            return 0;
        uint32 kind = r_u32(frame + 0x10);
        bool allowed = false;
        if (kind - 2 < 2)
        {
            uint32 face = r_u32(frame + 0x14);
            uint32 material = r_u16(face + 6) & 0x1f;
            allowed = ((0xc400c4 >> material) & 1) == 0;
        }
        if (!allowed && kind == 5)
        {
            uint32 face = r_u32(frame + 0x14);
            allowed = (r_u16(face) & 0x2000) != 0;
        }
        if (!allowed)
            return 0;
        kind = r_u32(frame + 0x10);
        if (kind == 3)
            terrain = true;
        else
        {
            uint32 owner = r_u32(frame + 0x18);
            uint32 matrix = r_u32(owner + 0x30);
            if (r_s16(matrix + 0x12) == 1)
                return 0;
            terrain = r_u32(frame + 0x10) == 3;
        }
    }
    else
    {
        if (r_u32(frame + 0x10) == 0)
            return 0;
        terrain = r_u32(frame + 0x10) == 3;
    }
    if (terrain)
    {
        uint32 face = r_u32(frame + 0x14);
        game = r_u32(0x80097048);
        uint32 material = r_u16(face + 6);
        uint32 scene = r_u32(game + 0x48);
        uint32 table = r_u32(scene + 0xf0);
        uint32 entry = r_u32(table + ((material & 0x1e0) >> 1));
        if ((entry & 0x200) != 0)
            return 0;
    }
    if (mode == 0)
        return 1;
    uint32 kind = r_u32(frame + 0x10);
    if (kind == 2 || kind == 5)
    {
        uint32 owner = r_u32(frame + 0x18);
        w_u32(state + 0xec, owner);
        owner = r_u32(frame + 0x18);
        uint32 model = r_u32(owner + 0x1c);
        if (model == 0)
            return 1;
        return (r_u32(model) & 0x2000) != 0 ? 1 : 0;
    }
    w_u32(state + 0xec, 0);
    return 1;
}

uint32 G3Function80078E40(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80078e40, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 selector = r_u32(object + 0x154);
    if (selector == 0x80)
    {
        if ((r_u16(state + 0xce) & 1) != 0 && (r_s16(object + 0x5e) != 1 || r_u16(object + 0x6e) != 0x12))
        {
            G3Function8007D818(object, game, state, controls, frame);
            G3Function8007DB40(object, game, state, controls, frame);
        }
        else if (r_s32(object + 0x10c) < 0x20 && (r_u32(controls + 4) & 0x80) != 0)
        {
            w_u32(object + 0x154, 0x20);
            w_u16(object + 0x6e, 0x19);
            uint32 flags = r_u32(object + 0x128);
            w_u16(object + 0x5e, 0);
            w_u32(object + 0x128, flags & 0xfffffff7);
        }
        else if ((r_u32(object + 0x14) & 0x10) != 0)
        {
            w_u16(object + 0x6e, 0x13);
            w_u16(object + 0x5e, 0);
            G3Function8007CE9C(object, game, state, controls, frame);
        }
        else if (r_s16(object + 0x5e) == 1 && r_u16(object + 0x6e) == 0x12)
        {
            uint32 flags = r_u32(object + 0x128);
            sint32 index = r_s16(state + 0xe0);
            uint32 table = r_u32(state + 0x30);
            sint32 speed = r_s16(table + ((uint32)index << 1));
            if ((flags & 0x600) != 0)
                w_u32(object + 0x10c, (uint32)speed);
            else
            {
                sint32 scale = r_s16(state + 0x0c);
                uint32 product = (uint32)speed * (uint32)scale;
                w_u32(object + 0x10c, (uint32)((sint32)product >> 12));
            }
        }
    }
    else if (selector == 0x20)
    {
        if ((r_u32(object + 0x14) & 0x10) != 0)
        {
            uint32 model = r_u32(object + 0x1c);
            uint32 animation = r_u16(object + 0x6e);
            uint32 animations = r_u32(model + 0x10);
            uint32 data = r_u32(animations + (animation << 2));
            uint32 frames = r_u16(data + 2);
            w_u16(object + 0x5e, frames - 1);
        }
        if ((r_u16(state + 0xce) & 1) != 0)
        {
            if ((r_u32(controls) & 0x80) != 0)
            {
                uint32 pool = r_u32(0x800a27fc);
                w_u32(object + 0x154, 0x80);
                w_u16(object + 0x6e, 0x12);
                w_u16(object + 0x5e, 0);
                w_u32(object + 0x10c, 0);
                G3Function80073560(object, pool, frame);
            }
            else
                G3Function8007D818(object, game, state, controls, frame);
            G3Function8007DB40(object, game, state, controls, frame);
            w_u32(frame + 0x10, 6);
            w_u32(frame + 0x14, 0);
            G3Function8008194C(0, 1, 0xffffffff, 0x333, frame);
        }
        uint32 destination = r_u32(state + 0xfc);
        uint32 value = r_u32(state + 0xf0);
        w_u32(destination, value);
        destination = r_u32(state + 0x108);
        value = r_u32(state + 0xf0);
        w_u32(destination, value);
        destination = r_u32(state + 0x114);
        value = r_u32(state + 0xf0);
        w_u32(destination, value);
        G3Function8007D4EC(object, game, state, controls, frame);
    }
    G3Function8007D9EC(object, game, state, controls, frame);
    uint32 result = G3Function8007D0D0(object, game, state, controls, frame);
    G3Function8007DB80(object, game, state, controls, frame, result);
    G3Function8007D7A8(object, game, state, controls, frame);
    sint32 z = r_s16(object + 0x5c);
    sint32 origin = r_s16(state + 0x138);
    sint32 maximum = r_s16(state + 0x132);
    sint32 delta = z - origin;
    result = maximum < delta ? 1 : 0;
    if (result != 0)
        w_u16(state + 0x132, (uint32)delta);
    return result;
}

uint32 G3Function80080A58(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80080a58, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 timer = r_u32(object + 0x108);
    uint32 player = r_u32(game + 0x50);
    w_u32(object + 0x108, timer + 1);
    uint32 rotation = r_u16(object + 0x64);
    uint32 phase = r_u32(object + 0x104);
    w_u16(object + 0x64, rotation + 0x60);
    if (phase == 0)
    {
        timer = r_u32(object + 0x108);
        w_u16(frame + 0x1c, timer * 12 + 0x96);
        timer = r_u32(object + 0x108);
        uint32 radius = timer << 5;
        if ((sint32)(0x10 - timer) < 0)
            radius = (0x20 - timer) << 5;
        uint32 angle = r_u32(object + 0x10c);
        uint32 product = G3Function8005B470(angle) * radius;
        w_u16(frame + 0x18, (uint32)((sint32)product >> 12));
        angle = r_u32(object + 0x10c);
        product = G3Function8005B590(angle) * radius;
        w_u16(frame + 0x1a, (uint32)((sint32)product >> 12));
        uint32 offset = r_u16(frame + 0x18);
        uint32 position = r_u16(player + 0x58);
        w_u16(object + 0x58, offset + position);
        offset = r_u16(frame + 0x1a);
        position = r_u16(player + 0x5a);
        w_u16(object + 0x5a, offset + position);
        offset = r_u16(frame + 0x1c);
        position = r_u16(player + 0x5c);
        uint32 current_game = r_u32(0x80097048);
        uint32 z = offset + position;
        w_u16(object + 0x5c, z);
        if (r_u16(current_game + 0x68b4) == 0x0f)
            w_u16(object + 0x5c, z + 0x78);
        timer = r_u32(object + 0x108);
        uint32 scale = timer * 120 + 0x100;
        if ((sint32)scale >= 0x1001)
            scale = 0x1000;
        w_u16(object + 0x68, scale);
        w_u16(object + 0x6a, scale);
        w_u16(object + 0x6c, scale);
        w_u32(frame + 0x10, 1);
        G3Function80080DE4(object, game, 1, 0x800, frame);
        if (r_u32(object + 0x108) == 0x20)
            w_u32(object + 0x104, 1);
        uint32 result = r_u32(object + 0x10c) + 0x80;
        w_u32(object + 0x10c, result);
        return result;
    }
    if (phase != 1)
        return 1;
    w_u16(frame + 0x18, 0);
    w_u16(frame + 0x1a, 0);
    uint32 distance = r_u32(object + 0x110);
    w_u16(frame + 0x1c, (distance << 1) + 0x216);
    uint32 matrix = r_u32(player + 0x30);
    ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(frame + 0x18, 8), (SVECTOR *)psx_addr(object + 0x58, 6));
    uint32 position = r_u16(object + 0x58);
    uint32 origin = r_u16(player + 0x58);
    w_u16(object + 0x58, position + origin);
    position = r_u16(object + 0x5a);
    origin = r_u16(player + 0x5a);
    uint32 z = r_u16(object + 0x5c);
    w_u16(object + 0x5a, position + origin);
    origin = r_u16(player + 0x5c);
    uint32 current_game = r_u32(0x80097048);
    z += origin;
    w_u16(object + 0x5c, z);
    if (r_u16(current_game + 0x68b4) == 0x0f)
        w_u16(object + 0x5c, z + 0x78);
    w_u32(frame + 0x10, 0x14);
    G3Function80080DE4(object, game, 1, 0x800, frame);
    timer = r_u32(object + 0x108);
    if (timer != 0x40)
    {
        if ((timer & 3) != 0)
            return 0x7ff8;
        w_u32(frame + 0x10, 0x7ff8);
        return G3Function8003E518(object, 0x802, 0x1f4, 0x7d, frame);
    }
    uint32 first_model = r_u32(0x8009876c);
    uint32 model = r_u32(object + 0x1c);
    uint32 pitch;
    if (first_model == model)
        pitch = 0x64;
    else
        pitch = r_u32(0x80098774) == model ? 0xc8 : 0;
    w_u32(frame + 0x10, 0x7ff8);
    G3Function8003E518(object, 0x827, pitch, 0x7d, frame);
    w_u32(frame + 0x10, 0x1e);
    G3Function80080DE4(object, game, 0x18, 0x1000, frame);
    G3Function800193F8(0x40000, object, frame);
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x10, ((flags | 0x10) & 0xfffffbff) | 0x20);
    if (r_u32(object + 0x114) != 0)
    {
        uint32 player_state = r_u32(player + 0x24);
        w_u32(player + 0x154, 0x20000000);
        w_u32(player_state + 0x1bc, 0);
        w_u16(player_state + 0xc4, 0);
    }
    uint32 first_resource = r_u32(0x80098824);
    model = r_u32(object + 0x1c);
    if (first_resource == model)
    {
        uint32 value = r_u32(game + 0x682c);
        return G3Function80047EE4(3, value);
    }
    uint32 result = r_u32(0x80098814);
    if (result == model)
    {
        uint32 value = r_u32(game + 0x6818);
        result = G3Function80047EE4(0, value);
    }
    return result;
}

uint32 G3Function80080DE4(uint32 object, uint32 game, uint32 count, uint32 size, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80080de4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    uint32 first_model = r_u32(0x8009873c);
    uint32 spread = r_u32(guest_stack + 0x10);
    if (first_model == 0)
        return 0x800a0000;
    uint32 first_pointer = r_u32(first_model + 0x0c);
    uint32 second_model = r_u32(0x8009881c);
    uint32 first_resource = r_u32(first_pointer);
    if (second_model == 0)
        return first_pointer;
    uint32 second_pointer = r_u32(second_model + 0x0c);
    uint32 pool = r_u32(0x800a27fc);
    uint32 second_resource = r_u32(second_pointer);
    uint32 owner = G3Function800415CC(pool, frame);
    if (first_resource == 0 || second_resource == 0 || owner == 0)
        return owner;
    if (spread == 0)
        spread = 1;
    uint32 remaining = count - 1;
    w_u16(frame + 0x30, size);
    w_u16(frame + 0x32, size);
    w_u16(frame + 0x34, size);
    if (remaining == 0xffffffff)
        return 0xffffffff;
    uint32 negative_spread = 0 - spread;
    uint32 center = (uint32)((sint32)(negative_spread + (negative_spread >> 31)) >> 1);
    uint32 alternate = 0;
    do
    {
        uint32 position = r_u16(object + 0x58);
        w_u16(frame + 0x18, position);
        position = r_u16(object + 0x5a);
        w_u16(frame + 0x1a, position);
        position = r_u16(object + 0x5c);
        w_u16(frame + 0x1c, position);
        sint32 random = (sint32)G3SdkRandom80027FA8();
        sint32 remainder = spread == 0xffffffff ? 0 : random % (sint32)spread;
        w_u16(frame + 0x20, center + (uint32)remainder);
        random = (sint32)G3SdkRandom80027FA8();
        remainder = spread == 0xffffffff ? 0 : random % (sint32)spread;
        w_u16(frame + 0x22, center + (uint32)remainder);
        random = (sint32)G3SdkRandom80027FA8();
        remainder = spread == 0xffffffff ? 0 : random % (sint32)spread;
        w_u16(frame + 0x28, 0);
        w_u16(frame + 0x2a, 0);
        w_u16(frame + 0x2c, 0);
        w_u16(frame + 0x24, center + (uint32)remainder);
        random = (sint32)G3SdkRandom80027FA8();
        uint32 lifetime = (uint32)(random % 8 + 0x14);
        lifetime = (uint32)((sint32)(lifetime << 16) >> 16);
        uint32 resource = alternate == 0 ? first_resource : second_resource;
        pool = r_u32(0x800a27fc);
        w_u32(frame + 0x10, owner);
        uint32 effect = G3Function80044688(frame + 0x18, resource, pool, lifetime, frame);
        if (effect != 0)
        {
            G3Function80044A90(effect, frame + 0x20, frame + 0x28);
            G3Function80044878(effect, frame + 0x30, 0);
        }
        --remaining;
        alternate ^= 1;
    } while (remaining != 0xffffffff);
    return 0xffffffff;
}

uint32 G3Function8005E7B4(uint32 output, uint32 box, uint32 point)
{
    FUNCTION_MARKER(0x8005e7b4, "SLUS_008.06");
    uint32 result = 1;
    for (uint32 offset = 0; offset < 6; offset += 2)
    {
        sint32 coordinate = r_s16(point + offset);
        sint32 lower = r_s16(box + 4 + offset);
        uint32 raw_coordinate = r_u16(point + offset);
        uint32 bound = r_u16(box + 4 + offset);
        bool clamped = coordinate < lower;
        if (!clamped)
        {
            sint32 upper = r_s16(box + 0x0a + offset);
            bound = r_u16(box + 0x0a + offset);
            clamped = upper < coordinate;
        }
        if (clamped)
        {
            w_u16(output + offset, bound);
            result = 0;
        }
        else
            w_u16(output + offset, raw_coordinate);
    }
    return result;
}

uint32 G3Function8005E888(uint32 point, uint32 center, uint32 normal)
{
    FUNCTION_MARKER(0x8005e888, "SLUS_008.06");
    sint32 coordinate = r_s16(point);
    sint32 origin = r_s16(center);
    sint32 dx = coordinate - origin;
    uint32 square_x = (uint32)dx * (uint32)dx;
    w_u32(0x1f800000, (uint32)dx);
    coordinate = r_s16(point + 2);
    origin = r_s16(center + 2);
    sint32 dy = coordinate - origin;
    uint32 square_y = (uint32)dy * (uint32)dy;
    w_u32(0x1f800004, (uint32)dy);
    coordinate = r_s16(point + 4);
    origin = r_s16(center + 4);
    sint32 dz = coordinate - origin;
    uint32 square_z = (uint32)dz * (uint32)dz;
    w_u32(0x1f800008, (uint32)dz);
    uint32 radius = r_u16(point + 6);
    if (square_x + square_y + square_z >= radius * radius)
        return 0;
    uint32 minimum = (uint32)(dx < 0 ? -dx : dx);
    uint32 middle = (uint32)(dy < 0 ? -dy : dy);
    uint32 maximum = (uint32)(dz < 0 ? -dz : dz);
    uint32 temporary;
    if (minimum < middle)
    {
        if (maximum < minimum)
        {
            temporary = minimum;
            minimum = maximum;
            maximum = temporary;
            temporary = middle;
            middle = maximum;
            maximum = temporary;
        }
        else if (maximum < middle)
        {
            temporary = middle;
            middle = maximum;
            maximum = temporary;
        }
    }
    else if (maximum < middle)
    {
        temporary = minimum;
        minimum = maximum;
        maximum = temporary;
    }
    else
    {
        temporary = minimum;
        minimum = middle;
        middle = temporary;
        if (maximum < middle)
        {
            temporary = middle;
            middle = maximum;
            maximum = temporary;
        }
    }
    uint32 weighted = maximum * 30 + middle * 12 + minimum * 9;
    sint32 length = (sint32)weighted >> 5;
    if (length == 0)
        return 0;
    uint32 value = r_u32(0x1f800000);
    w_u16(normal, (uint32)((sint32)(value << 12) / length));
    value = r_u32(0x1f800004);
    w_u16(normal + 2, (uint32)((sint32)(value << 12) / length));
    value = r_u32(0x1f800008);
    w_u16(normal + 4, (uint32)((sint32)(value << 12) / length));
    radius = r_u16(point + 6);
    value = r_u32(0x1f800000);
    uint32 product = value * radius;
    w_u32(0x1f800000, product);
    uint32 x = (uint32)((sint32)(product << 5) / (sint32)weighted);
    radius = r_u16(point + 6);
    value = r_u32(0x1f800004);
    product = value * radius;
    w_u32(0x1f800004, product);
    uint32 y = (uint32)((sint32)(product << 5) / (sint32)weighted);
    radius = r_u16(point + 6);
    value = r_u32(0x1f800008);
    product = value * radius;
    w_u32(0x1f800008, product);
    uint32 z = (uint32)((sint32)(product << 5) / (sint32)weighted);
    w_u32(0x1f800000, x);
    w_u32(0x1f800004, y);
    x = r_u16(0x1f800000);
    w_u32(0x1f800008, z);
    value = r_u16(center);
    w_u16(point, value + x);
    value = r_u16(center + 2);
    y = r_u16(0x1f800004);
    w_u16(point + 2, value + y);
    value = r_u16(center + 4);
    z = r_u16(0x1f800008);
    w_u16(point + 4, value + z);
    return 1;
}

uint32 G3Function8005E3C8(uint32 box, uint32 point, uint32 previous, uint32 normal, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005e3c8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x78;
    if (G3Function8005E7B4(frame + 0x20, box, point) != 0)
    {
        w_u32(frame + 0x10, point);
        w_u32(frame + 0x14, previous);
        w_u32(frame + 0x18, box);
        uint32 result = G3Function8005F9F4(frame + 0x28, frame + 0x38, frame + 0x30, frame + 0x40, frame);
        if (result != 0)
        {
            uint32 value = r_u16(frame + 0x38);
            w_u16(normal, value);
            value = r_u16(frame + 0x3a);
            w_u16(normal + 2, value);
            value = r_u16(frame + 0x3c);
            w_u16(normal + 4, value);
            uint32 x = r_u16(frame + 0x28);
            uint32 y = r_u16(frame + 0x2a);
            uint32 z = r_u16(frame + 0x2c);
            w_u16(0x800a25a4, x);
            w_u16(0x800a25a6, y);
            w_u16(0x800a25a8, z);
            sint32 direction = r_s16(normal);
            uint32 radius = r_u16(point + 6);
            uint32 product = (uint32)direction * radius;
            w_u16(point, x + (uint32)((sint32)product >> 12));
            direction = r_s16(normal + 2);
            radius = r_u16(point + 6);
            product = (uint32)direction * radius;
            y = r_u16(frame + 0x2a);
            w_u16(point + 2, y + (uint32)((sint32)product >> 12));
            direction = r_s16(normal + 4);
            radius = r_u16(point + 6);
            product = (uint32)direction * radius;
            z = r_u16(frame + 0x2c);
            w_u16(point + 4, z + (uint32)((sint32)product >> 12));
            return result;
        }
        uint32 current_x = r_u16(point);
        uint32 previous_x = r_u16(previous);
        uint32 delta = current_x - previous_x;
        w_u16(frame + 0x50, delta);
        sint32 dx = (sint32)(delta << 16) >> 16;
        uint32 current_y = r_u16(point + 2);
        uint32 previous_y = r_u16(previous + 2);
        uint32 absolute_x = (uint32)(dx < 0 ? -dx : dx);
        delta = current_y - previous_y;
        w_u16(frame + 0x52, delta);
        sint32 dy = (sint32)(delta << 16) >> 16;
        uint32 current_z = r_u16(point + 4);
        uint32 previous_z = r_u16(previous + 4);
        uint32 absolute_y = (uint32)(dy < 0 ? -dy : dy);
        w_u16(frame + 0x58, absolute_x);
        w_u16(frame + 0x5a, absolute_y);
        delta = current_z - previous_z;
        w_u16(frame + 0x54, delta);
        sint32 dz = (sint32)(delta << 16) >> 16;
        uint32 absolute_z = (uint32)(dz < 0 ? -dz : dz);
        sint32 low_x = (sint32)(absolute_x << 16) >> 16;
        w_u16(frame + 0x5c, absolute_z);
        if ((sint32)(absolute_x << 16) >= (sint32)(absolute_y << 16) && low_x >= ((sint32)(absolute_z << 16) >> 16))
        {
            uint32 bound = r_u16(box + (dx < 0 ? 0x0a : 4));
            uint32 radius = r_u16(point + 6);
            uint32 direction;
            if (dx < 0)
            {
                w_u16(point, bound + radius);
                direction = 0x1000;
            }
            else
            {
                w_u16(point, bound - radius);
                direction = 0xfffff000;
            }
            w_u16(normal, direction);
            w_u16(normal + 2, 0);
            w_u16(normal + 4, 0);
            return 1;
        }
        sint32 y_magnitude = r_s16(frame + 0x5a);
        sint32 x_magnitude = r_s16(frame + 0x58);
        bool use_y = y_magnitude >= x_magnitude;
        if (use_y)
            use_y = y_magnitude >= r_s16(frame + 0x5c);
        if (use_y)
        {
            sint32 displacement = r_s16(frame + 0x52);
            uint32 bound = r_u16(box + (displacement < 0 ? 0x0c : 6));
            uint32 radius = r_u16(point + 6);
            uint32 direction;
            if (displacement < 0)
            {
                w_u16(point + 2, bound + radius);
                direction = 0x1000;
            }
            else
            {
                w_u16(point + 2, bound - radius);
                direction = 0xfffff000;
            }
            w_u16(normal, 0);
            w_u16(normal + 2, direction);
            w_u16(normal + 4, 0);
            return 1;
        }
        sint32 displacement = r_s16(frame + 0x54);
        uint32 direction;
        if (displacement > 0)
        {
            uint32 bound = r_u16(box + 8);
            uint32 radius = r_u16(point + 6);
            w_u16(point + 4, bound - radius);
            direction = 0xfffff000;
        }
        else
        {
            uint32 owner = r_u32(0x8009ee10);
            sint32 scale = owner != 0 ? r_s16(owner + 0x6c) : 0x1000;
            uint32 base;
            uint32 change;
            if (owner != 0 && scale != 0x1000)
            {
                sint32 ratio = scale == 0 ? -1 : 0x1000000 / scale;
                sint32 bound = r_s16(box + 0x0e);
                uint32 radius = r_u16(point + 6);
                sint32 z = r_s16(point + 4);
                uint32 product = ((uint32)bound + radius - (uint32)z) * (uint32)ratio;
                base = r_u16(point + 4);
                change = (uint32)((sint32)product >> 12);
            }
            else
            {
                base = r_u16(box + 0x0e);
                change = r_u16(point + 6);
            }
            w_u16(point + 4, base + change);
            direction = 0x1000;
        }
        w_u16(normal, 0);
        w_u16(normal + 2, 0);
        w_u16(normal + 4, direction);
        return 1;
    }
    if (G3Function8005E888(point, frame + 0x20, frame + 0x48) != 0)
    {
        uint32 x = r_u16(frame + 0x20);
        uint32 y = r_u16(frame + 0x22);
        uint32 z = r_u16(frame + 0x24);
        uint32 value = r_u16(frame + 0x48);
        w_u16(0x800a25a4, x);
        w_u16(0x800a25a6, y);
        w_u16(0x800a25a8, z);
        w_u16(normal, value);
        value = r_u16(frame + 0x4a);
        w_u16(normal + 2, value);
        value = r_u16(frame + 0x4c);
        w_u16(normal + 4, value);
        return 1;
    }
    w_u32(frame + 0x10, point);
    w_u32(frame + 0x14, previous);
    w_u32(frame + 0x18, box);
    uint32 result = G3Function8005F9F4(frame + 0x28, frame + 0x38, frame + 0x30, frame + 0x40, frame);
    uint32 x = r_u16(frame + 0x28);
    uint32 y = r_u16(frame + 0x2a);
    uint32 z = r_u16(frame + 0x2c);
    uint32 value = r_u16(frame + 0x38);
    w_u16(0x800a25a4, x);
    w_u16(0x800a25a6, y);
    w_u16(0x800a25a8, z);
    w_u16(normal, value);
    value = r_u16(frame + 0x3a);
    w_u16(normal + 2, value);
    value = r_u16(frame + 0x3c);
    w_u16(normal + 4, value);
    return result;
}

uint32 G3Function8003D4E4(uint32 object, uint32 descriptor, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003d4e4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 flags = r_u32(object + 0x14);
    if ((flags & 0x200) == 0)
        return 0;
    w_u32(object + 0x14, flags & 0xfffffdff);
    w_u16(object + 0x4e, 4);
    uint32 volume_range = r_u8(descriptor + 7);
    uint32 volume = r_u8(descriptor + 6);
    if (volume_range != 0)
    {
        sint32 random = (sint32)G3Function80027FDC();
        uint32 divisor = r_u8(descriptor + 7) << 1;
        sint32 remainder = divisor == 0 ? random : random % (sint32)divisor;
        volume_range = r_u8(descriptor + 7);
        volume += volume_range - (uint32)remainder;
    }
    uint32 pitch_range = r_u16(descriptor + 0x0a);
    uint32 pitch = r_u16(descriptor + 8);
    if (pitch_range != 0)
    {
        sint32 random = (sint32)G3Function80027FDC();
        pitch_range = r_u16(descriptor + 0x0a);
        uint32 divisor = pitch_range << 1;
        sint32 remainder = divisor == 0 ? random : random % (sint32)divisor;
        pitch += pitch_range - (uint32)remainder;
    }
    pitch = (uint32)((sint32)(pitch << 16) >> 16);
    sint32 option = r_s16(descriptor + 4);
    uint32 sound = r_u16(descriptor + 2);
    volume = (uint32)((sint32)(volume << 16) >> 16);
    w_u32(frame + 0x10, (uint32)option);
    return G3Function8003E518(object, sound, pitch, volume, frame);
}

uint32 G3Function80079F40(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80079f40, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    bool active = (r_u32(object + 0x14) & 0x10) != 0;
    if (!active)
        active = r_u32(state + 0x204) != 0;
    if (!active)
    {
        active = (r_u32(controls + 4) & 0x8e0) != 0;
        if (!active)
            active = (r_u32(controls) & 0x8000000f) != 0;
        if (active)
        {
            uint32 player = r_u32(0x8009efc4);
            active = (r_u32(player + 0x10) & 0x100) == 0;
        }
    }
    if (active)
    {
        uint32 y = r_u32(game + 0xa8);
        uint32 x = r_u32(game + 0xa4);
        uint32 heading = G3Function8002F240(0 - y, x, frame) + 0x400;
        sint32 target = r_s16(state + 0xbe);
        sint32 angle = (sint32)(heading << 16) >> 16;
        bool turn = false;
        if (target != 0 && angle != 0)
        {
            uint32 difference = G3Function80038B48((uint32)angle, (uint32)target);
            bool negative = (sint32)(difference << 16) < 0;
            target = r_s16(state + 0xbe);
            difference = G3Function80038B48((uint32)angle, (uint32)target);
            sint32 distance = (sint32)(difference << 16) >> 16;
            if (negative)
                distance = -distance;
            if (distance >= 0x58e)
            {
                uint32 current_game = r_u32(0x80097048);
                turn = (r_u32(current_game + 0x6834) & 0x10) == 0;
            }
        }
        if (turn)
        {
            uint32 camera = r_u32(game + 0x4c);
            uint32 adjustment = G3Function8004DB1C();
            w_u16(state + 0xc2, (heading + adjustment) & 0xfff);
            w_u16(state + 0xbc, heading);
            w_u32(object + 0x154, 0x800000);
            w_u16(object + 0x6e, 0x24);
        }
        else
        {
            w_u32(object + 0x154, 2);
            w_u16(object + 0x6e, 8);
            w_u16(state + 0xe0, 0);
            w_u16(state + 0xe2, 0);
        }
        G3Function8007CF04(object, game, state, controls, frame);
        G3Function8007CD24(object, game, state, controls, frame);
        G3Function8007D0D0(object, game, state, controls, frame);
        G3Function8007D244(object, game, state, controls, frame);
    }
    if ((r_u16(object + 0x5e) & 1) != 0)
    {
        w_u32(frame + 0x10, 0x0a);
        G3Function80072D00(object, game, state, controls, frame);
    }
    uint32 result = G3Function8007DB40(object, game, state, controls, frame);
    return G3Function8007DB80(object, game, state, controls, frame, result);
}

uint32 G3Function80078C04(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80078c04, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    if (r_u32(object + 0x150) == 0 && (r_u32(object + 0x14) & 0x10) != 0)
    {
        w_u32(object + 0x154, 2);
        w_u16(object + 0x6e, 8);
        uint32 flags = r_u32(object + 0x128);
        w_u16(object + 0x5e, 0);
        w_u32(object + 0x128, flags & 0xfffffff7);
        G3Function8007CF04(object, game, state, controls, frame);
        G3Function8007CE9C(object, game, state, controls, frame);
    }
    else
    {
        uint32 mode = r_u32(object + 0x150);
        uint32 animation = 0;
        if (mode == 1 && (r_u32(object + 0x14) & 0x10) != 0)
            animation = 0x0c;
        else if (mode == 3 && (r_u32(object + 0x14) & 0x10) != 0)
            animation = 0x3b;
        if (animation != 0)
        {
            w_u32(object + 0x154, 2);
            w_u16(object + 0x6e, animation);
            uint32 flags = r_u32(object + 0x128);
            w_u16(object + 0x5e, 0);
            w_u32(object + 0x128, flags & 0xfffffff7);
        }
        else
        {
            uint32 destination = r_u32(state + 0xfc);
            uint32 value = r_u32(state + 0xf0);
            w_u32(destination, value);
            destination = r_u32(state + 0x108);
            value = r_u32(state + 0xf0);
            w_u32(destination, value);
            destination = r_u32(state + 0x114);
            value = r_u32(state + 0xf0);
            w_u32(destination, value);
        }
    }
    if (r_s16(object + 0x5e) >= 0x0a || r_u32(object + 0x154) != 0x10)
        G3Function8007D0D0(object, game, state, controls, frame);
    if ((r_u32(object + 0x128) & 0x10000) != 0)
    {
        uint32 level = r_u8(game + 0x68b4);
        if (r_s16(0x80099708 + level * 28 + 0x14) != -1)
            G3Function800358C0(1, 0x19, frame);
    }
    if (r_u32(object + 0x150) == 3)
        return 3;
    return G3Function8007DB80(object, game, state, controls, frame, 3);
}

uint32 G3Function8004DB1C(void)
{
    FUNCTION_MARKER(0x8004db1c, "SLUS_008.06");
    return r_u32(0x80096a90);
}

uint32 G3Function80052CE8(uint32 search, uint32 direction)
{
    FUNCTION_MARKER(0x80052ce8, "SLUS_008.06");
    sint32 signed_direction = (sint32)(direction << 16) >> 16;
    uint32 increment = signed_direction == 1 ? 0x155 : 0xfffffeab;
    uint32 limit = signed_direction == 1 ? 0x400 : 0xfffffc00;
    w_u32(search + 8, increment);
    w_u32(search + 0x0c, limit);
    w_u32(search + 0x10, increment);
    uint32 value = r_u32(search + 8);
    w_u32(search + 4, value);
    uint32 result = r_u32(0x800a24d0) | 0x60;
    w_u32(0x800a24d0, result);
    return result;
}

uint32 G3Function80052BAC(uint32 search, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80052bac, "SLUS_008.06");
    uint32 step = r_u32(search + 0x10);
    uint32 current = r_u32(search + 4);
    uint32 next = current + step;
    w_u32(search + 4, next);
    if ((sint32)step > 0)
    {
        sint32 limit = r_s32(search + 0x0c);
        if ((sint32)next < limit)
            return 1;
    }
    else
    {
        if ((sint32)step == 0)
            return 0;
        sint32 limit = r_s32(search + 0x0c);
        if (limit < (sint32)next)
            return 1;
    }
    uint32 flags = r_u32(0x800a24d0);
    if ((flags & 0x80) == 0)
    {
        w_u32(0x800a24d0, flags | 0x80);
        uint32 reversed = 0 - r_u32(search + 0x10);
        w_u32(search + 0x10, reversed);
        uint32 increment = (sint32)reversed > 0 ? 0x155 : 0xfffffeab;
        uint32 limit = (sint32)reversed > 0 ? 0x400 : 0xfffffc00;
        w_u32(search + 8, increment);
        w_u32(search + 0x0c, limit);
        uint32 result = r_u32(search + 8);
        w_u32(search + 4, result);
        return result;
    }
    if ((flags & 0x100) == 0)
    {
        w_u32(0x800a24d0, flags | 0x100);
        uint32 reversed = 0 - r_u32(search + 0x10);
        w_u32(search + 0x10, reversed);
        uint32 increment = (sint32)reversed > 0 ? 0x400 : 0xfffffc00;
        uint32 limit = (sint32)reversed > 0 ? 0x0c00 : 0xfffff400;
        w_u32(search + 8, increment);
        w_u32(search + 0x0c, limit);
        uint32 result = r_u32(search + 8);
        w_u32(search + 4, result);
        return result;
    }
    if ((flags & 0x2000) != 0)
        return 0x2000;
    uint32 diagnostic = r_u32(0x8009ea44);
    w_u32(0x800a24d0, flags | 0x2000);
    uint32 result = diagnostic & 0x80;
    if (result != 0)
        result = G3SdkPrint80085E84(0x8009eae4);
    return result;
}

uint32 G3Function80079E70(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80079e70, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    w_u16(state + 0xe2, 0);
    w_u16(state + 0xe0, 0);
    if ((r_u32(object + 0x14) & 0x10) != 0 || r_u32(controls) != 0)
    {
        w_u32(object + 0x154, 2);
        w_u16(object + 0x6e, 8);
        w_u16(object + 0x5e, 0);
        G3Function8007CF04(object, game, state, controls, frame);
    }
    uint32 result = G3Function8007CD24(object, game, state, controls, frame);
    G3Function8007DB80(object, game, state, controls, frame, result);
    return G3Function8007D7A8(object, game, state, controls, frame);
}

uint32 G3Function800787E0(uint32 value, uint32 target, uint32 step)
{
    FUNCTION_MARKER(0x800787e0, "SLUS_008.06");
    sint32 signed_step = (sint32)(step << 16) >> 16;
    sint32 signed_target = (sint32)(target << 16) >> 16;
    sint32 current = r_s16(value);
    uint32 raw = r_u16(value);
    if (current + signed_step < signed_target)
        w_u16(value, raw + step);
    current = r_s16(value);
    bool decrease = signed_target < current - signed_step;
    raw = r_u16(value);
    uint32 result = raw - step;
    if (decrease)
        w_u16(value, result);
    return result;
}

uint32 G3Function80079BC0(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80079bc0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 scene = r_u32(game + 0x48);
    uint32 linked = r_u32(state + 0x1bc);
    uint32 sequence = r_u32(scene + 0x78);
    uint32 matrix = r_u32(linked + 0x30);
    G3Function800737E4(object, game, frame);
    if (matrix != 0)
    {
        sint32 component = r_s16(matrix + 2);
        uint32 position = r_u16(linked + 0x58);
        uint32 product = (uint32)component * 575;
        uint32 target = position - (uint32)((sint32)product >> 12);
        target = (uint32)((sint32)(target << 16) >> 16);
        G3Function800787E0(object + 0x58, target, 0x10);
        component = r_s16(matrix + 8);
        position = r_u16(linked + 0x5a);
        product = (uint32)component * 575;
        target = position - (uint32)((sint32)product >> 12);
        target = (uint32)((sint32)(target << 16) >> 16);
        G3Function800787E0(object + 0x5a, target, 0x10);
        position = r_u16(linked + 0x5c);
        w_u16(object + 0x5c, position + 0x1e);
    }
    uint32 current_linked = r_u32(state + 0x1bc);
    uint32 rotation = r_u32(current_linked + 0x20);
    uint32 angle = r_u16(rotation + 0x0c) - 0x800;
    angle = (uint32)((sint32)(angle << 16) >> 16);
    G3Function80038AA8(object + 0x64, angle, 0x40);
    bool use_camera = r_u32(0x800aa7d4) != 0;
    if (!use_camera && G3Function8003525C() != 0)
        use_camera = r_s16(0x800ad0ca) != 0x118;
    uint32 result;
    if (use_camera)
        result = G3Function8007E444(object, game, state, controls, frame);
    else
    {
        G3Function800356E4();
        G3Function80035A80();
        uint32 owner = r_u32(state);
        w_u32(object + 0xa4, owner);
        result = r_u16(object + 0x6e);
        if (result != 0x27)
        {
            w_u16(object + 0x6e, 0x27);
            w_u16(object + 0x5e, 0);
        }
        else
        {
            result = r_u32(object + 0x14) & 0x10;
            if (result != 0)
            {
                uint32 flags = r_u32(object + 0x10);
                w_u32(object + 0x10, flags | 0x800);
                result = r_u32(object + 0x128) & 0x02000000;
                if (result != 0)
                {
                    bool transition = sequence == 0;
                    if (sequence != 0)
                    {
                        flags = r_u32(object + 0x134);
                        if ((flags & 0x40) == 0)
                        {
                            w_u32(object + 0x134, flags | 0x40);
                            uint32 linked_flags = r_u32(linked + 0x10);
                            w_u32(linked + 0x10, linked_flags | 0x400);
                            result = G3Function80018DE0(linked, sequence + 4, frame);
                        }
                        else
                        {
                            result = 0x20000000;
                            transition = (flags & 0x80) != 0;
                        }
                    }
                    if (transition)
                    {
                        w_u32(object + 0x154, 0x20000000);
                        w_u16(state + 0xc4, 0);
                        w_u16(state + 0x1ce, 0);
                        w_u16(state + 0x1d0, 0);
                        w_u16(state + 0x1d2, 0);
                        result = r_u32(object + 0x134) & 0xffffff3f;
                        w_u32(object + 0x134, result);
                        if (linked != 0)
                        {
                            result = r_u32(linked + 0x10) & 0xfffffbff;
                            w_u32(linked + 0x10, result);
                        }
                    }
                }
            }
        }
    }
    if (sequence == 0)
    {
        result = r_u32(0x80097048);
        uint32 flags = r_u32(result + 0xe8);
        w_u32(result + 0xe8, flags | 0x08000000);
        current_linked = r_u32(state + 0x1bc);
        if (current_linked != 0)
        {
            result = r_u32(current_linked + 0xec);
            if (result != 0)
                result = G3DispatchStateEvent(result, current_linked, game, 2, 0, frame);
        }
    }
    return result;
}

uint32 G3Function80039040(uint32 guest_stack, uint32 *second_glyph, uint32 *third_glyph)
{
    FUNCTION_MARKER(0x80039040, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x20;
    uint32 level = r_u16(game + 0x68b4);
    w_u32(frame + 0x10, 0);
    if (level - 0x14 < 3)
    {
        level = r_u16(game + 0x68b4);
        uint32 flags = r_u32(game + 0x6838);
        uint32 mask = (uint32)1 << (level & 31);
        w_u32(frame + 0x10, flags & mask);
        return r_u32(frame + 0x10);
    }
    uint32 name;
    if (((level - 0x10) & 0xffff) < 4)
    {
        if (level == 0x13)
            name = 0x8009dae0;
        else if (level == 0x11)
            name = 0x8009daec;
        else if (level == 0x10)
            name = 0x8009daf8;
        else if (level == 0x12)
            name = 0x8009db04;
        else
            return r_u32(frame + 0x10);
    }
    else
    {
        if (((level - 0x0b) & 0xffff) >= 5)
            return r_u32(frame + 0x10);
        level = r_u16(game + 0x68b4);
        uint32 variant = r_u32(game + 0x69f8);
        uint32 index = (level - 0x0b) * 3 + variant;
        name = r_u32(0x80099d08 + (index << 3) + 4);
    }
    if (second_glyph != 0)
        *second_glyph = frame + 0x10;
    G3Function8002786C(game, name, frame + 0x10, third_glyph);
    return r_u32(frame + 0x10);
}

uint32 G3Function800833EC(uint32 object)
{
    FUNCTION_MARKER(0x800833ec, "SLUS_008.06");
    uint32 data = G3Function8008345C(object, 0, 0);
    if (data != 0)
        return r_u32(data);
    return 0;
}

uint32 G3Function8007993C(uint32 object, uint32 game, uint32 state)
{
    FUNCTION_MARKER(0x8007993c, "SLUS_008.06");
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 position = r_u16(object + 0x58 + axis * 2);
        uint32 velocity = r_u16(state + 0xd0 + axis * 2);
        w_u16(object + 0x58 + axis * 2, position + velocity);
    }
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        if (r_s16(state + 0xd8 + axis * 2) == 0)
            w_u16(state + 0xd0 + axis * 2, 0);
    }
    uint32 result = 0;
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        sint32 velocity = r_s16(state + 0xd0 + axis * 2);
        sint32 acceleration = r_s16(state + 0xd8 + axis * 2);
        uint32 raw_acceleration = r_u16(state + 0xd8 + axis * 2);
        sint32 magnitude = velocity + acceleration;
        if (magnitude < 0)
            magnitude = -magnitude;
        if (acceleration < 0)
            acceleration = -acceleration;
        bool advancing = acceleration < magnitude;
        uint32 raw_velocity = r_u16(state + 0xd0 + axis * 2);
        result = raw_velocity + raw_acceleration;
        if (advancing)
            w_u16(state + 0xd0 + axis * 2, result);
        else
        {
            w_u16(state + 0xd0 + axis * 2, 0);
            w_u16(state + 0xd8 + axis * 2, 0);
        }
    }
    return result;
}

uint32 G3Function8007A894(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007a894, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 timer = r_u16(state + 0xc4);
    w_u16(state + 0xc4, timer + 1);
    G3Function8007993C(object, game, state);
    if (r_s16(state + 0xc4) >= 2 && r_u32(state + 0xd0) == 0 && r_s16(state + 0xd4) == 0)
    {
        G3Function8007D818(object, game, state, controls, frame);
        uint32 result = G3Function8007DB40(object, game, state, controls, frame);
        G3Function8007DB80(object, game, state, controls, frame, result);
        G3Function8007D0D0(object, game, state, controls, frame);
        G3Function8007D1E8(object, game, state, controls, frame);
    }
    if (r_u16(object + 0x6e) == 0x2e)
    {
        w_u32(frame + 0x10, 10);
        return G3Function80072E6C(object, game, frame);
    }
    return 10;
}

uint32 G3Function80072E6C(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80072e6c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    r_masked_u32(0x800a029c, 15);
    uint32 size = r_masked_u32(0x800a029c, 15);
    sint32 size_z_initial = r_s16(0x800a02a0);
    uint32 destination = frame + 0x18;
    uint32 high_address = destination + 3;
    uint32 high_lane = high_address & 3;
    uint32 low_lane = destination & 3;
    w_masked_u32(high_address & 0xfffffffc, size >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    w_masked_u32(destination & 0xfffffffc, size << (low_lane * 8), (0xf << low_lane) & 0xf);
    w_u16(frame + 0x1c, size_z_initial);
    uint32 result = r_u32(game + 0x14c);
    uint32 resource = r_u32(0x800987ac);
    result &= 1;
    if (result != 0)
        return result;
    result = G3SdkRandom80027FA8();
    if (resource == 0)
        return result;
    uint32 random = result;
    uint32 jitter_x = random & 0x1f;
    sint32 shifted_random = (sint32)(random << 16);
    uint32 scale_delta = random & 0xfff;
    uint32 x = r_u16(object + 0x58);
    uint32 size_z = r_u16(frame + 0x1c);
    uint32 jitter_y = (shifted_random >> 21) & 0x1f;
    size_z += scale_delta;
    uint32 jitter_z = (shifted_random >> 26) & 0xf;
    w_u16(frame + 0x10, x + jitter_x - 0xf);
    uint32 y = r_u16(object + 0x5a);
    w_u16(frame + 0x12, y + jitter_y - 0xf);
    uint32 z = r_u16(object + 0x5c);
    uint32 size_x = r_u16(frame + 0x18);
    w_u16(frame + 0x1c, size_z);
    size_x += scale_delta;
    w_u16(frame + 0x14, z + jitter_z + 0x80 - 7);
    w_u16(frame + 0x18, size_x);
    if ((random & 4) != 0)
        w_u16(frame + 0x18, 0 - size_x);
    if ((random & 8) != 0)
        w_u16(frame + 0x1c, 0 - size_z);
    uint32 models = r_u32(resource + 0xc);
    uint32 pool = r_u32(0x800a27fc);
    uint32 model = r_u32(models);
    uint32 effect = G3Function800444E4(frame + 0x10, model, pool, 0x1e, frame);
    if (effect == 0)
        return 0;
    result = G3Function800449F0(effect, 0, 0, 0x10, effect);
    G3Function80044878(effect, frame + 0x18, &result);
    return result;
}

uint32 G3Function8007A7F0(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007a7f0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 result = r_u16(state + 0xce) & 0x40;
    if (result == 0)
    {
        w_u32(object + 0x154, 0x100);
        result = 0x11;
        w_u16(object + 0x6e, result);
        w_u16(object + 0x5e, 0);
    }
    G3Function8007DB80(object, game, state, controls, frame, result);
    result = r_u16(object + 0x5e) & 7;
    if (result != 0)
        return result;
    w_u32(frame + 0x10, 0);
    return G3Function80072D00(object, game, state, controls, frame);
}

uint32 G3Function80079A90(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80079a90, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    if ((r_u32(object + 0x134) & 1) != 0)
    {
        w_u32(object + 0x154, 0x40);
        w_u16(object + 0x6e, 0x13);
        w_u16(object + 0x5e, 0);
        return G3Function8007D7A8(object, game, state, controls, frame);
    }
    if ((r_u32(object + 0x14) & 0x10) == 0)
        return G3Function8007993C(object, game, state);
    uint32 resource = r_u32(object + 0x1c);
    uint32 animation = r_u16(object + 0x6e);
    uint32 animations = r_u32(resource + 0x10);
    uint32 descriptor = r_u32(animations + animation * 4);
    uint32 length = r_u16(descriptor + 2);
    w_u16(object + 0x5e, length - 1);
    G3Function8007DB40(object, game, state, controls, frame);
    uint32 result = r_u16(state + 0xce) & 1;
    if (result == 0)
        return result;
    w_u32(object + 0x154, 1);
    w_u16(object + 0x6e, 0x28);
    w_u16(object + 0x5e, 0);
    G3Function8007D7A8(object, game, state, controls, frame);
    w_u32(frame + 0x10, 0x2000);
    w_u32(frame + 0x14, 0x28);
    return G3Function800836F0(object, 0x14, 0x2000, 0xd, frame);
}

uint32 G3Function800836F0(uint32 object, uint32 mode, uint32 scale, uint32 kind, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800836f0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 vertical_scale = r_u32(guest_stack + 0x10);
    uint32 resource = r_u32(0x800987b4);
    uint32 duration = r_u32(guest_stack + 0x14);
    if (resource == 0)
        return 0x800a0000;
    uint32 parent = r_u32(0x8009efc4);
    uint32 created = G3Function8002A454(parent, resource, frame);
    if (created == 0)
        return 0;
    w_u32(created + 0x128, object);
    uint32 intro = r_u32(object + 0x20);
    w_u16(created + 0x130, mode);
    w_u16(created + 0x132, scale);
    w_u16(created + 0x134, kind);
    w_u16(created + 0x136, duration);
    w_u32(created + 0x12c, intro);
    uint32 flags = r_u32(created + 0x10);
    w_u16(created + 0x6a, scale);
    w_u16(created + 0x68, scale);
    w_u16(created + 0x6c, vertical_scale);
    uint32 result = flags | 0x800;
    w_u32(created + 0x10, result);
    return result;
}

uint32 G3Function80074194(uint32 object, uint32 game, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80074194, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 player = r_u32(0x8009efc4);
    uint32 result = (uint32)r_s16(game + 0x681c);
    uint32 flags = r_u32(player + 0x154);
    uint32 state = r_u32(player + 0x24);
    if ((flags & 0xc6400000) != 0)
        return result;
    if (r_s16(state + 0x120) != 0)
        return result;
    if (r_u32(state + 0x1b8) != 0)
        return result;
    sint32 height = r_s16(player + 0x5c);
    sint32 target_height = r_s16(player + 0xe0);
    if (height - 0x500 >= target_height)
        return result;
    result = G3Function8007408C(player, game, frame);
    if (result == 0)
        return result;
    w_u32(frame + 0x10, 0x2000);
    w_u32(frame + 0x14, 0x28);
    G3Function800836F0(player, 0x14, 0x2000, 0xd, frame);
    w_u16(player + 0x6e, 0x28);
    w_u32(player + 0x154, 0x400000);
    w_u16(state + 0x1d4, payload);
    w_u16(state + 0xc4, 0);
    uint32 new_height = r_u16(player + 0xe0);
    w_u16(player + 0x5c, new_height);
    uint32 source = player + 0x58;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
    uint32 low_lane = source & 3;
    uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
    uint32 position = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    sint32 z = r_s16(player + 0x5c);
    uint32 destination = state + 0x1f4;
    high_address = destination + 3;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, position >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    low_lane = destination & 3;
    w_masked_u32(destination & 0xfffffffc, position << (low_lane * 8), (15 << low_lane) & 15);
    w_u16(state + 0x1f8, z);
    return result;
}

uint32 G3Function80015F70(uint32 object, uint32 parameters, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80015f70, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 entry = r_u32(parameters + 4);
    if (entry == 0)
    {
        uint32 player = r_u32(0x8009efc4);
        uint32 mode = r_u32(player + 0x150);
        if (mode != 7)
        {
            if (mode == 6)
            {
                uint32 game = r_u32(0x80097048);
                uint32 camera = r_u32(game + 0x4c);
                G3Function80051FE4(camera, frame);
            }
            player = r_u32(0x8009efc4);
            mode = r_u32(player + 0x150);
            w_u32(0x8009d34c, mode);
        }
        player = r_u32(0x8009efc4);
        w_u32(player + 0x150, 7);
    }
    else
    {
        uint32 flags = r_u32(entry + 0x18);
        w_u32(entry + 0x18, flags | 0x100);
        entry = r_u32(parameters + 4);
        uint32 linked = r_u32(entry + 0x20);
        if (linked != 0)
        {
            if ((flags & 0x100) == 0)
            {
                uint32 target = r_u32(linked + 0xec);
                if (target != 0)
                {
                    uint32 game = r_u32(0x80097048);
                    G3DispatchStateEvent(target, linked, game, 0x14, 0, frame);
                }
            }
            if ((r_u32(linked + 0x14) & 0x10000) != 0)
            {
                uint32 handle = r_u32(linked + 0x50);
                G3Function8003F18C(handle);
                flags = r_u32(linked + 0x14);
                w_u32(linked + 0x14, flags & 0xfffcffff);
            }
        }
    }
    return 2;
}

void G3Function8005AECC(uint32 level, uint32 camera, uint32 context, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005aecc, "SLUS_008.06");
    uint32 first = r_u32(camera + 0x20);
    uint32 second = r_u32(camera + 0x24);
    uint32 third = r_u32(camera + 0x28);
    uint32 fourth = r_u32(camera + 0x2c);
    xport_gte_write_control(0, first);
    xport_gte_write_control(1, second);
    xport_gte_write_control(2, third);
    xport_gte_write_control(3, fourth);
    first = r_u32(camera + 0x30);
    second = r_u32(camera + 0x40);
    third = r_u32(camera + 0x44);
    fourth = r_u32(camera + 0x48);
    xport_gte_write_control(4, first);
    xport_gte_write_control(8, second);
    xport_gte_write_control(9, third);
    xport_gte_write_control(10, fourth);
    first = r_u32(camera + 0x4c);
    second = r_u32(camera + 0x50);
    xport_gte_write_control(11, first);
    xport_gte_write_control(12, second);
    xport_gte_write_data(0, r_u32(camera));
    xport_gte_write_data(1, r_u32(camera + 4));
    xport_gte_execute(0x486012);
    uint32 node = r_u32(level);
    uint32 pending = 0x1f800040;
    w_u32(0x1f800020, xport_gte_read_data(25));
    w_u32(0x1f800024, xport_gte_read_data(26));
    w_u32(0x1f800028, xport_gte_read_data(27));
    xport_gte_execute(0x4a6012);
    w_u32(0x1f800080, 0);
    uint32 limit = r_u32(camera + 0x88);
    w_u32(pending, pending);
    w_u32(0x1f80002c, xport_gte_read_data(25));
    w_u32(0x1f800030, xport_gte_read_data(26));
    w_u32(0x1f800034, xport_gte_read_data(27));
    for (;;)
    {
        uint32 flags = r_u8(node + 0xc);
        uint32 minimum = r_u32(node);
        bool visible = false;
        if ((flags & 4) == 0)
        {
            uint32 middle = r_u32(node + 4);
            sint32 min_x = (sint32)(minimum << 16) >> 16;
            sint32 max_x = (sint32)middle >> 16;
            w_u16(0x1f800038, (min_x + max_x) >> 1);
            sint32 min_y = (sint32)minimum >> 16;
            uint32 maximum = r_u32(node + 8);
            sint32 min_z = (sint32)(middle << 16) >> 16;
            sint32 max_z = (sint32)maximum >> 16;
            w_u16(0x1f80003c, (min_z + max_z) >> 1);
            sint32 max_y = (sint32)(maximum << 16) >> 16;
            w_u16(0x1f80003a, (max_y + min_y) >> 1);
            xport_gte_write_data(0, r_u32(0x1f800038));
            xport_gte_write_data(1, r_u32(0x1f80003c));
            xport_gte_execute(0x486012);
            uint32 radius = r_u16(node + 0xe);
            uint32 origin = r_u32(0x1f800020);
            uint32 projected = xport_gte_read_data(25);
            uint32 negative_radius = 0 - radius;
            uint32 distance = projected - origin;
            uint32 upper = limit - negative_radius;
            if ((sint32)(distance - negative_radius) > 0)
            {
                uint32 side = distance - upper;
                origin = r_u32(0x1f800024);
                if ((sint32)side < 0)
                {
                    projected = xport_gte_read_data(26);
                    distance = projected - origin;
                    side = distance - negative_radius;
                    origin = r_u32(0x1f800028);
                    if ((sint32)side > 0)
                    {
                        projected = xport_gte_read_data(27);
                        distance = projected - origin;
                        if ((sint32)(distance - negative_radius) > 0)
                        {
                            xport_gte_execute(0x4a6012);
                            origin = r_u32(0x1f80002c);
                            projected = xport_gte_read_data(25);
                            distance = projected - origin;
                            side = distance - negative_radius;
                            origin = r_u32(0x1f800030);
                            if ((sint32)side > 0)
                            {
                                projected = xport_gte_read_data(26);
                                distance = projected - origin;
                                visible = (sint32)(distance - negative_radius) > 0;
                            }
                        }
                    }
                }
            }
        }
        if (visible)
        {
            uint32 count = r_u32(0x1f800080);
            if ((flags & 1) != 0)
            {
                pending += 4;
                uint32 right = r_u32(node + 0x14);
                node = r_u32(node + 0x10);
                w_u32(pending, right);
                continue;
            }
            w_u32(context + (count << 2) + 4, node);
            ++count;
            w_u32(0x1f800080, count);
            node = r_u32(pending);
            if ((sint32)(count - 0x800) >= 0)
            {
                uint32 final_count = r_u32(0x1f800080);
                w_u32(context, final_count);
                return;
            }
            uint32 previous_pending = pending;
            pending -= 4;
            if (node == previous_pending)
            {
                w_u32(context, count);
                return;
            }
            continue;
        }
        node = r_u32(pending);
        uint32 previous_pending = pending;
        pending -= 4;
        if (node == previous_pending)
        {
            uint32 count = r_u32(0x1f800080);
            w_u32(context, count);
            return;
        }
    }
}

uint32 G3Function8008004C(uint32 object, uint32 game, uint32 state, uint32 controls)
{
    FUNCTION_MARKER(0x8008004c, "SLUS_008.06");
    uint32 result = r_u32(object + 0x10) & 0x400000;
    if (result != 0)
        return result;
    result = (uint32)r_s16(state + 4);
    uint32 motion = r_u32(object + 0x154);
    w_u32(object + 0x118, result);
    if (motion == 0x40000)
    {
        result = (uint32)r_s16(state + 0xa);
        w_u32(object + 0x118, result);
    }
    if ((r_u32(object + 0x154) & 0xe8) != 0 && (r_u32(controls) & 0x80) != 0)
    {
        result = (uint32)r_s16(state + 6);
        w_u32(object + 0x118, result);
    }
    if ((r_u32(object + 0x128) & 0x400) != 0)
    {
        result = (uint32)r_s16(state + 6);
        w_u32(object + 0x118, result);
    }
    if ((r_u32(object + 0x154) & 0x200000) != 0 && r_u16(object + 0x6e) == 0x2e)
    {
        result = (uint32)r_s16(state + 0x16);
        w_u32(object + 0x118, result);
        return result;
    }
    uint32 flags = r_u32(state + 0x204);
    result = 0xfffffffb;
    if ((flags & 4) != 0)
    {
        uint32 value = r_u32(state + 0x210);
        result &= flags;
        w_u32(state + 0x204, result);
        w_u32(object + 0x118, value);
    }
    return result;
}

uint32 G3Function80052454(uint32 destination, uint32 angle, uint32 source, uint32 offset)
{
    FUNCTION_MARKER(0x80052454, "SLUS_008.06");
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 component = r_u16(source + axis * 2);
        w_u16(destination + axis * 2, component);
    }
    uint32 signed_angle = (uint32)((sint32)(angle << 16) >> 16);
    G3Function8002F8E4(destination, signed_angle);
    uint32 result = 0;
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 component = r_u16(destination + axis * 2);
        uint32 delta = r_u16(offset + axis * 2);
        result = component + delta;
        w_u16(destination + axis * 2, result);
    }
    return result;
}

uint32 G3Function8003D5E8(uint32 object, uint32 descriptor, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003d5e8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 flags = r_u32(object + 0x14);
    if ((flags & 0x100000) == 0)
        return 0;
    w_u32(object + 0x14, flags & 0xffefffff);
    w_u16(object + 0x4e, 4);
    uint32 variation = r_u8(descriptor + 7);
    uint32 volume = r_u8(descriptor + 6);
    if (variation != 0)
    {
        uint32 random = G3Function80027FDC();
        uint32 width = r_u8(descriptor + 7) * 2;
        uint32 remainder = width != 0 ? random % width : random;
        uint32 center = r_u8(descriptor + 7);
        volume += center - remainder;
    }
    variation = r_u16(descriptor + 0xa);
    uint32 pitch = r_u16(descriptor + 8);
    if (variation != 0)
    {
        uint32 random = G3Function80027FDC();
        uint32 width = r_u16(descriptor + 0xa);
        uint32 divisor = width * 2;
        uint32 remainder = divisor != 0 ? random % divisor : random;
        pitch += width - remainder;
    }
    pitch = (uint32)((sint32)(pitch << 16) >> 16);
    volume = (uint32)((sint32)(volume << 16) >> 16);
    uint32 distance = (uint32)r_s16(descriptor + 4);
    uint32 sound = r_u16(descriptor + 2);
    w_u32(frame + 0x10, distance);
    return G3Function8003E518(object, sound, pitch, volume, frame);
}

uint32 G3Function8003D8FC(uint32 object, uint32 descriptor, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003d8fc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 flags = r_u32(object + 0x14);
    if ((flags & 0x10000) != 0)
    {
        uint32 handle = r_u32(object + 0x50);
        if (G3Function8003F0A8(handle) == 0)
        {
            uint32 result = r_u32(object + 0x14) & 0xfffcffff;
            w_u32(object + 0x14, result);
            return result;
        }
        handle = r_u32(object + 0x50);
        uint32 result = G3Function8003F088(handle);
        if (result == 0)
            return result;
        flags = r_u32(object + 0x14);
        if ((flags & 0x8000) == 0)
        {
            result = flags | 0x8000;
            w_u32(object + 0x14, result);
            return result;
        }
        w_u32(object + 0x14, flags & 0xffff7fff);
        uint32 distance = (uint32)r_s16(descriptor + 4);
        uint32 pitch = (uint32)r_s16(descriptor + 8);
        uint32 volume = r_u8(descriptor + 6);
        w_u32(frame + 0x10, distance);
        handle = r_u32(object + 0x50);
        result = G3Function8003E7E4(object, handle, pitch, volume, frame);
        if (result != 0)
            return result;
        handle = r_u32(object + 0x50);
        G3Function8003F18C(handle);
        result = r_u32(object + 0x14) & 0xfffcffff;
        w_u32(object + 0x14, result);
        return result;
    }
    if ((flags & 0x20000) == 0)
    {
        w_u32(object + 0x14, flags | 0x20000);
        uint32 delay = r_u8(descriptor + 7);
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
    uint32 sound = r_u16(descriptor + 2);
    uint32 pitch = (uint32)r_s16(descriptor + 8);
    uint32 volume = r_u8(descriptor + 6);
    uint32 distance = (uint32)r_s16(descriptor + 4);
    w_u32(frame + 0x10, distance);
    uint32 result = G3Function8003E518(object, sound, pitch, volume, frame);
    w_u32(object + 0x50, result);
    if (result != 0)
    {
        result = (r_u32(object + 0x14) | 0x10000) & 0xffffefff;
        w_u32(object + 0x14, result);
    }
    return result;
}

uint32 G3Function8001CE1C(uint32 mesh, uint32 camera, uint32 context, uint32 ordering_table)
{
    FUNCTION_MARKER(0x8001ce1c, "SLUS_008.06");
    uint32 packet = r_u32(context + 4);
    uint32 game = r_u32(0x80097048);
    uint32 table_bytes = r_u32(game + 0x28) << 2;
    uint32 near_distance = r_u32(game + 0x34);
    w_u32(0x1f800014, table_bytes);
    w_u32(0x1f800018, ordering_table);
    uint32 final_bucket = ordering_table + table_bytes - 4;
    w_u32(0x1f80001c, near_distance >> 4);
    uint32 shift = r_u32(game + 0x2c) & 31;
    w_u32(0x1f800020, ((near_distance - 1000 + 2500) >> shift) << 2);
    w_u32(0x1f800024, 0x9c >> shift);
    sint32 camera_x = r_s16(camera);
    sint32 camera_y = r_s16(camera + 2);
    uint32 packed_camera = ((uint32)(camera_y >> 4) << 16) | ((uint32)(camera_x >> 4) & 0xffff);
    xport_gte_write_data(0, packed_camera);
    xport_gte_write_data(1, (uint32)(r_s16(camera + 4) >> 4));
    for (uint32 matrix = 0; matrix < 2; ++matrix)
    {
        uint32 source = camera + 0x20 + matrix * 0x20;
        uint32 first = r_u32(source);
        uint32 second = r_u32(source + 4);
        xport_gte_write_control(0, first);
        xport_gte_write_control(1, second);
        first = r_u32(source + 8);
        second = r_u32(source + 0xc);
        uint32 third = r_u32(source + 0x10);
        xport_gte_write_control(2, first);
        xport_gte_write_control(3, second);
        xport_gte_write_control(4, third);
        xport_gte_execute(0x486012);
        uint32 destination = matrix == 0 ? 0x1f800000 : 0x1f80000c;
        w_u32(destination, xport_gte_read_data(25));
        w_u32(destination + 4, xport_gte_read_data(26));
        if (matrix == 0)
            w_u32(destination + 8, xport_gte_read_data(27));
    }
    uint32 transform = r_u32(camera + 0x78);
    uint32 first = r_u32(transform);
    uint32 second = r_u32(transform + 4);
    uint32 third = r_u32(transform + 8);
    xport_gte_write_control(0, first);
    xport_gte_write_control(1, second);
    xport_gte_write_control(2, third);
    first = r_u32(transform + 0xc);
    second = r_u32(transform + 0x10);
    third = r_u32(transform + 0x14);
    xport_gte_write_control(3, first);
    xport_gte_write_control(4, second);
    xport_gte_write_control(5, (uint32)((sint32)third >> 4));
    first = (uint32)(r_s32(transform + 0x18) >> 4);
    second = (uint32)(r_s32(transform + 0x1c) >> 4);
    xport_gte_write_control(6, first);
    xport_gte_write_control(7, second);
    for (uint32 matrix = 0; matrix < 2; ++matrix)
    {
        uint32 source = camera + 0x20 + matrix * 0x20;
        uint32 control = matrix == 0 ? 8 : 16;
        first = r_u32(source);
        second = r_u32(source + 4);
        xport_gte_write_control(control, first);
        xport_gte_write_control(control + 1, second);
        first = r_u32(source + 8);
        second = r_u32(source + 0xc);
        third = r_u32(source + 0x10);
        xport_gte_write_control(control + 2, first);
        xport_gte_write_control(control + 3, second);
        xport_gte_write_control(control + 4, third);
    }
    uint32 count = r_u32(mesh + 0x20);
    uint32 group = r_u32(mesh + 0x24);
    if (count != 0)
    {
        uint32 group_end = group + count * 0x18;
        do
        {
            uint32 flags = r_u32(group);
            do
            {
                if ((flags & 1) != 0 || (flags >> 16) == 0)
                    break;
                xport_gte_write_data(0, r_u32(group + 0xc));
                xport_gte_write_data(1, r_u32(group + 0x10));
                xport_gte_execute(0x4a6012);
                uint32 origin = r_u32(0x1f800000);
                uint32 negative_radius = 0 - (uint32)r_s16(group + 0x12);
                uint32 projected_x = xport_gte_read_data(25);
                uint32 projected_y = xport_gte_read_data(26);
                uint32 projected_z = xport_gte_read_data(27);
                uint32 distance = projected_x - origin;
                if ((sint32)negative_radius >= (sint32)distance)
                    break;
                uint32 threshold = r_u32(0x1f80001c);
                bool depth_pass = (flags & 2) == 0 || (sint32)(threshold + negative_radius) < (sint32)distance;
                origin = r_u32(0x1f800004);
                if (!depth_pass)
                    break;
                bool y_pass = (sint32)negative_radius < (sint32)(projected_y - origin);
                origin = r_u32(0x1f800008);
                if (!y_pass)
                    break;
                bool z_pass = (sint32)negative_radius < (sint32)(projected_z - origin);
                xport_gte_execute(0x4c6012);
                if (!z_pass)
                    break;
                origin = r_u32(0x1f80000c);
                projected_x = xport_gte_read_data(25);
                projected_y = xport_gte_read_data(26);
                bool light_x_pass = (sint32)negative_radius < (sint32)(projected_x - origin);
                origin = r_u32(0x1f800010);
                if (!light_x_pass || (sint32)negative_radius >= (sint32)(projected_y - origin))
                    break;
                uint32 mode = (flags & 2) != 0 ? 2 : ((flags & 4) != 0 ? 4 : 0);
                uint32 triangle = r_u32(group + 8);
                uint32 triangle_end = triangle + (flags >> 16) * 12;
                uint32 vertices;
                uint32 colors = 0;
                if (mode != 0)
                {
                    uint32 color_offset = r_u32(group + 0x14);
                    vertices = r_u32(group + 4);
                    colors = vertices + (color_offset << 3);
                }
                else
                    vertices = r_u32(group + 4);
                do
                {
                    do
                    {
                        uint32 indices = r_u32(triangle);
                        uint32 index_z = (uint32)r_s16(triangle + 4);
                        uint32 vertex_x = vertices + ((indices & 0xffff) << 3);
                        uint32 color_x = 0;
                        uint32 color_y_address = 0;
                        if (mode != 0)
                        {
                            color_x = r_u32(colors + ((indices & 0xffff) << 2));
                            color_y_address = colors + ((indices >> 16) << 2);
                        }
                        uint32 vertex_y = vertices + ((indices >> 16) << 3);
                        uint32 material = r_u32(triangle + 8);
                        uint32 color_z_address = colors + (index_z << 2);
                        uint32 vertex_z = vertices + (index_z << 3);
                        xport_gte_write_data(0, r_u32(vertex_x));
                        xport_gte_write_data(1, r_u32(vertex_x + 4));
                        xport_gte_write_data(2, r_u32(vertex_y));
                        xport_gte_write_data(3, r_u32(vertex_y + 4));
                        xport_gte_write_data(4, r_u32(vertex_z));
                        xport_gte_write_data(5, r_u32(vertex_z + 4));
                        if (material == 0)
                            break;
                        xport_gte_execute(0x280030);
                        uint32 texture_y = r_u32(material + 4);
                        uint32 texture_x = r_u32(material);
                        uint32 texture_z = r_u32(material + 8);
                        w_u32(packet + 0xc, texture_x);
                        w_u32(packet + (mode == 0 ? 0x14 : 0x18), texture_y);
                        w_u32(packet + (mode == 0 ? 0x1c : 0x24), texture_z);
                        if ((xport_gte_read_control(31) & 0x7e046000) != 0)
                            break;
                        uint32 color_y = 0;
                        uint32 color_z = 0;
                        uint32 bucket = final_bucket;
                        if (mode == 2)
                        {
                            xport_gte_execute(0x1400006);
                            color_y = r_u32(color_y_address);
                            if ((sint32)xport_gte_read_data(24) >= 0)
                                break;
                            xport_gte_execute(0x158002d);
                            uint32 maximum_depth = r_u32(0x1f800014);
                            uint32 table = r_u32(0x1f800018);
                            uint32 bias = r_u32(0x1f800024);
                            uint32 depth = xport_gte_read_data(7);
                            uint32 minimum_depth = r_u32(0x1f800020);
                            uint32 offset = (depth + bias) << 6;
                            bucket = table + offset;
                            if ((sint32)offset >= (sint32)maximum_depth || (sint32)offset < (sint32)minimum_depth)
                                break;
                        }
                        uint32 command = r_u32(material + 0xc);
                        if (mode == 2)
                            color_z = r_u32(color_z_address);
                        else if (mode == 4)
                        {
                            color_y = r_u32(color_y_address);
                            color_z = r_u32(color_z_address);
                        }
                        uint32 screen_x = xport_gte_read_data(12);
                        uint32 screen_y = xport_gte_read_data(13);
                        uint32 screen_z = xport_gte_read_data(14);
                        if ((screen_x & screen_y & screen_z & 0x80008000) != 0)
                            break;
                        if (((0x01000200 - screen_y) & (0x01000200 - screen_x) & (0x01000200 - screen_z) & 0x80008000) != 0)
                            break;
                        w_u32(packet + 8, screen_x);
                        w_u32(packet + (mode == 0 ? 0x10 : 0x14), screen_y);
                        w_u32(packet + (mode == 0 ? 0x18 : 0x20), screen_z);
                        if (mode == 0)
                            w_u32(packet + 4, command);
                        else
                            w_u32(packet + 0x10, color_y);
                        uint32 previous = r_u32(bucket);
                        w_u32(packet, previous | (mode == 0 ? 0x07000000 : 0x09000000));
                        w_u32(bucket, packet & 0x00ffffff);
                        if (mode != 0)
                        {
                            w_u32(packet + 4, color_x | (command & 0x03000000));
                            w_u32(packet + 0x1c, color_z);
                        }
                        packet += mode == 0 ? 0x20 : 0x28;
                    }
                    while (false);
                    triangle += 12;
                }
                while (triangle < triangle_end);
            }
            while (false);
            group += 0x18;
        }
        while (group < group_end);
    }
    w_u32(context + 4, packet);
    return 0;
}
