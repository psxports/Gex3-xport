#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void G3UtakerInitialize(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000300, "G3L_61");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    w_u32(state + 8, 0x00895440);
    w_u16(state + 0x14, 0x5b);
    w_u16(state + 0x1c, 0x4b0);
    w_u16(state + 0x20, 10);
    w_u16(state + 0x22, 0x50);
    w_u16(state + 0x2a, 0x1e);
    w_u16(state + 0x30, 100);
    w_u16(state + 0x2e, 0x96);
    w_u32(state + 4, 0x005f5e10);
    w_u16(state + 0x18, 0);
    w_u16(state + 0x1a, 0);
    w_u16(state + 0x1e, 0);
    w_u16(state + 0x26, 0);
    w_u16(state + 0x24, 0);
    w_u16(state + 0x2c, 3);
    w_u32(state + 0xc, 0);
    uint32 resource = r_u32(object + 0x1c);
    uint32 commands = r_u32(resource + 0x1c);
    G3UtakerConfigure(object, commands, frame);
    commands = r_u32(object + 0x28);
    if (commands != 0)
    {
        if (r_s16(commands + 2) == 0)
        {
            sint32 health = r_s16(commands);
            uint32 health_half = r_u16(commands);
            if (health != 0)
                w_u16(state + 0x2c, health_half);
            else
            {
                health_half = r_u16(state + 0x2c);
                w_u16(commands, health_half);
            }
            w_u32(state + 0xc, commands);
            commands += 4;
        }
        G3UtakerConfigure(object, commands, frame);
    }
    uint32 yaw = r_u16(object + 0x64);
    w_u32(object + 0x150, 0);
    w_u16(object + 0x6e, 4);
    w_u16(object + 0x5e, 0);
    w_u16(state + 0x16, yaw);
}

void G3UtakerUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000041c, "G3L_61");
    uint32 frame = guest_stack - 0x38;
    uint32 flags = r_u32(object + 0x10);
    uint32 mode = r_u32(object + 0x150);
    w_u32(object + 0x10, flags | 0x400);
    uint32 state = object + 0x104;
    if (mode >= 7)
        return;
    uint32 target = r_u32(module_base + 0xe28 + mode * 4) - module_base;
    switch (target)
    {
    case 0x47c:
    {
        flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags & ~0x400u);
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 player = r_u32(0x8009efc4);
        uint32 own = r_u16(object + 0x58);
        uint32 other = r_u16(player + 0x58);
        uint32 dx = other - own;
        uint32 x = (uint32)(sint32)(sint16)dx;
        uint32 square_x = x * x;
        w_u16(frame + 0x10, dx);
        other = r_u16(player + 0x5a);
        own = r_u16(object + 0x5a);
        uint32 dy = other - own;
        uint32 y = (uint32)(sint32)(sint16)dy;
        uint32 square_y = y * y;
        w_u16(frame + 0x12, dy);
        other = r_u16(player + 0x5c);
        own = r_u16(object + 0x5c);
        uint32 dz = other - own;
        uint32 z = (uint32)(sint32)(sint16)dz;
        uint32 square_z = z * z;
        w_u16(frame + 0x14, dz);
        sint32 radius = r_s32(state + 4);
        uint32 distance = square_x + square_y + square_z;
        if ((sint32)distance < radius)
        {
            w_u32(object + 0x150, 1);
            w_u16(state + 0x28, 0);
        }
        return;
    }
    case 0x830:
    {
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 timer = r_u16(state + 0x28);
        uint32 player = r_u32(0x8009efc4);
        w_u16(state + 0x28, timer + 1);
        uint32 other = r_u16(player + 0x58);
        uint32 own = r_u16(object + 0x58);
        uint32 dx = other - own;
        uint32 x = (uint32)(sint32)(sint16)dx;
        uint32 square_x = x * x;
        w_u16(frame + 0x10, dx);
        other = r_u16(player + 0x5a);
        own = r_u16(object + 0x5a);
        uint32 dy = other - own;
        uint32 y = (uint32)(sint32)(sint16)dy;
        uint32 square_y = y * y;
        w_u16(frame + 0x12, dy);
        other = r_u16(player + 0x5c);
        own = r_u16(object + 0x5c);
        uint32 dz = other - own;
        uint32 z = (uint32)(sint32)(sint16)dz;
        uint32 square_z = z * z;
        w_u16(frame + 0x14, dz);
        uint32 distance = square_x + square_y + square_z;
        uint32 heading = G3Function8002F240(y, x, frame);
        uint32 step = (uint32)r_s16(state + 0x14);
        G3Function80038AA8(object + 0x64, (uint32)(sint32)(sint16)(heading + 0x400), step);
        sint32 radius = r_s32(state + 8);
        if ((sint32)distance >= radius)
        {
            w_u32(object + 0x150, 0);
            return;
        }
        radius = r_s32(state);
        if ((sint32)distance < radius)
        {
            w_u32(object + 0x150, 6);
            w_u16(object + 0x6e, 0);
            w_u16(object + 0x5e, 0);
            return;
        }
        sint32 elapsed = r_s16(state + 0x28);
        sint32 delay = r_s16(state + 0x2a);
        if (elapsed < delay)
            return;
        w_u32(object + 0x150, 2);
        w_u16(object + 0x6e, 5);
        w_u16(object + 0x5e, 0);
        w_u16(state + 0x24, 0);
        return;
    }
    case 0x52c:
    {
        flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags & ~0x10u);
        G3Function80029F0C(object, 0xffffffff, frame);
        if (r_s16(state + 0x24) == 0)
        {
            uint32 player = r_u32(0x8009efc4);
            uint32 own = r_u16(object + 0x58);
            uint32 other = r_u16(player + 0x58);
            uint32 dx = other - own;
            w_u16(frame + 0x10, dx);
            other = r_u16(player + 0x5a);
            own = r_u16(object + 0x5a);
            uint32 dy = other - own;
            w_u16(frame + 0x12, dy);
            other = r_u16(player + 0x5c);
            own = r_u16(object + 0x5c);
            w_u16(frame + 0x14, other - own);
            uint32 heading = G3Function8002F240((uint32)(sint32)(sint16)dy, (uint32)(sint32)(sint16)dx, frame);
            uint32 step = (uint32)r_s16(state + 0x14);
            G3Function80038AA8(object + 0x64, (uint32)(sint32)(sint16)(heading + 0x400), step);
            sint32 animation_frame = r_s16(object + 0x5e);
            sint32 fire_frame = r_s16(state + 0x20);
            if (animation_frame >= fire_frame)
            {
                uint32 resource = r_u32(state + 0x10);
                w_u16(state + 0x24, 1);
                if (resource != 0)
                {
                    uint32 projectile = G3Function8002A454(object, resource, frame);
                    if (projectile != 0)
                    {
                        uint32 bone = (uint32)r_s16(state + 0x1e);
                        uint32 matrix = r_u32(object + 0x30);
                        matrix += bone << 5;
                        for (uint32 axis = 0; axis != 3; ++axis)
                        {
                            uint32 x = (uint32)r_s16(state + 0x18);
                            uint32 a = (uint32)r_s16(matrix + axis * 6);
                            uint32 product_x = x * a;
                            uint32 y = (uint32)r_s16(state + 0x1a);
                            uint32 b = (uint32)r_s16(matrix + axis * 6 + 2);
                            uint32 product_y = y * b;
                            uint32 z = (uint32)r_s16(state + 0x1c);
                            uint32 c = (uint32)r_s16(matrix + axis * 6 + 4);
                            uint32 product_z = z * c;
                            uint32 value = (uint32)((sint32)product_x >> 12) + (uint32)((sint32)product_y >> 12) + (uint32)((sint32)product_z >> 12);
                            w_u16(projectile + 0x58 + axis * 2, value);
                        }
                        uint32 value = r_u16(projectile + 0x58);
                        uint32 translation = r_u16(matrix + 0x14);
                        w_u16(projectile + 0x58, value + translation);
                        value = r_u16(projectile + 0x5a);
                        translation = r_u16(matrix + 0x18);
                        w_u16(projectile + 0x5a, value + translation);
                        value = r_u16(projectile + 0x5c);
                        translation = r_u16(matrix + 0x1c);
                        player = r_u32(0x8009efc4);
                        w_u16(projectile + 0x5c, value + translation);
                        uint32 player_x = (uint32)r_s16(player + 0x58);
                        uint32 projectile_x = (uint32)r_s16(projectile + 0x58);
                        uint32 distance_x = player_x - projectile_x;
                        uint32 square_x = distance_x * distance_x;
                        uint32 player_y = (uint32)r_s16(player + 0x5a);
                        uint32 projectile_y = (uint32)r_s16(projectile + 0x5a);
                        uint32 distance_y = player_y - projectile_y;
                        uint32 range = G3Function8002F448(square_x + distance_y * distance_y, 0, frame);
                        if (range == 0)
                            w_u16(frame + 0x1a, 0);
                        else
                        {
                            player = r_u32(0x8009efc4);
                            uint32 height = (uint32)r_s16(state + 0x30);
                            uint32 player_z = (uint32)r_s16(player + 0x5c);
                            uint32 projectile_z = (uint32)r_s16(projectile + 0x5c);
                            uint32 speed = (uint32)r_s16(state + 0x22);
                            sint32 numerator = (sint32)(speed * (player_z + height - projectile_z));
                            sint64 quotient = (sint64)numerator / (sint64)(sint32)range;
                            w_u16(frame + 0x1a, (uint32)quotient);
                        }
                        value = r_u16(state + 0x22);
                        w_u16(frame + 0x18, value);
                        value = r_u16(state + 0x26);
                        w_u16(frame + 0x1c, value);
                        value = r_u16(state + 0x2e);
                        w_u16(frame + 0x1e, value);
                        uint32 callback = r_u32(projectile + 0xec);
                        if (callback != 0)
                            G3DispatchStateEvent(callback, projectile, game, 0x7e1, frame + 0x18, frame);
                    }
                }
            }
        }
        if ((r_u32(object + 0x14) & 0x10) == 0)
            return;
        w_u32(object + 0x150, 3);
        w_u16(object + 0x6e, 1);
        w_u16(object + 0x5e, 0);
        return;
    }
    case 0xa04:
    {
        uint32 player = r_u32(0x8009efc4);
        uint32 own = r_u16(object + 0x58);
        uint32 other = r_u16(player + 0x58);
        uint32 dx = other - own;
        w_u16(frame + 0x10, dx);
        other = r_u16(player + 0x5a);
        own = r_u16(object + 0x5a);
        uint32 dy = other - own;
        w_u16(frame + 0x12, dy);
        other = r_u16(player + 0x5c);
        own = r_u16(object + 0x5c);
        w_u16(frame + 0x14, other - own);
        uint32 heading = G3Function8002F240((uint32)(sint32)(sint16)dy, (uint32)(sint32)(sint16)dx, frame);
        uint32 step = (uint32)r_s16(state + 0x14);
        G3Function80038AA8(object + 0x64, (uint32)(sint32)(sint16)(heading + 0x400), step);
    }
    // The close-range facing state continues through the animation transition
    case 0xa6c:
    case 0x950:
    case 0x998:
        flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags & ~0x10u);
        G3Function80029F0C(object, 0xffffffff, frame);
        if ((r_u32(object + 0x14) & 0x10) == 0)
            return;
        if (target == 0x950 || target == 0x998)
        {
            if (r_s16(state + 0x2c) <= 0)
            {
                G3Function80038798(object, 0, 0, frame);
                return;
            }
            if (target == 0x998)
            {
                w_u32(object + 0x150, 3);
                w_u16(object + 0x6e, 1);
                w_u16(object + 0x5e, 0);
                return;
            }
        }
        w_u32(object + 0x150, 1);
        w_u16(object + 0x6e, 4);
        w_u16(object + 0x5e, 0);
        w_u16(state + 0x28, 0);
        return;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "Unresolved Utaker update target %08x\n", (unsigned int)(target + module_base));
#endif
        }
        abort();
    }
}

void G3UtakerCollide(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000ad0, "G3L_61");
    uint32 frame = guest_stack - 0x38;
    uint32 contact = r_u32(object + 0xb0);
    uint32 other = r_u32(contact + 0x14);
    uint32 state = object + 0x104;
    G3Function8003C548(contact, frame + 0x18, frame + 0x19);
    uint32 player = r_u32(0x8009efc4);
    if (other == player && r_u8(frame + 0x18) - 6u < 4)
    {
        uint32 kind = r_u8(frame + 0x19);
        if (kind == 9 || kind - 3u < 3)
        {
            G3Function80074044(object, game, frame);
            return;
        }
    }
    uint32 resource = r_u32(other + 0x1c);
    uint32 name = r_u32(resource + 0x20);
    uint32 expected = r_u32(module_base + 0xe44);
    uint32 actual = r_u32(name);
    if (actual != expected)
        return;
    actual = r_u32(name + 4);
    expected = r_u32(module_base + 0xe48);
    if (actual != expected)
        return;
    if (r_u32(other + 0x150) != 1)
        return;
    if (r_u32(object + 0x150) == 2 && r_s16(object + 0x5e) >= 14 && r_s16(state + 0x24) == 0)
    {
        resource = r_u32(state + 0x10);
        if (resource != 0)
        {
            uint32 projectile = G3Function8002A454(object, resource, frame);
            if (projectile != 0)
            {
                uint32 bone = (uint32)r_s16(state + 0x1e);
                uint32 matrix = r_u32(object + 0x30);
                matrix += bone << 5;
                for (uint32 axis = 0; axis != 3; ++axis)
                {
                    uint32 x = (uint32)r_s16(state + 0x18);
                    uint32 a = (uint32)r_s16(matrix + axis * 6);
                    uint32 product_x = x * a;
                    uint32 y = (uint32)r_s16(state + 0x1a);
                    uint32 b = (uint32)r_s16(matrix + axis * 6 + 2);
                    uint32 product_y = y * b;
                    uint32 z = (uint32)r_s16(state + 0x1c);
                    uint32 c = (uint32)r_s16(matrix + axis * 6 + 4);
                    uint32 product_z = z * c;
                    uint32 value = (uint32)((sint32)product_x >> 12) + (uint32)((sint32)product_y >> 12) + (uint32)((sint32)product_z >> 12);
                    w_u16(projectile + 0x58 + axis * 2, value);
                }
                uint32 value = r_u16(projectile + 0x58);
                uint32 translation = r_u16(matrix + 0x14);
                w_u16(projectile + 0x58, value + translation);
                value = r_u16(projectile + 0x5a);
                translation = r_u16(matrix + 0x18);
                w_u16(projectile + 0x5a, value + translation);
                value = r_u16(projectile + 0x5c);
                translation = r_u16(matrix + 0x1c);
                w_u16(projectile + 0x5c, value + translation);
                w_u16(frame + 0x12, 0);
                w_u16(frame + 0x10, 20);
                w_u16(frame + 0x14, (uint32)-5);
                value = r_u16(state + 0x2e);
                w_u16(frame + 0x16, value);
                uint32 callback = r_u32(projectile + 0xec);
                if (callback != 0)
                    G3DispatchStateEvent(callback, projectile, game, 0x7e1, frame + 0x10, frame);
            }
        }
    }
    uint32 health = r_u16(state + 0x2c);
    uint32 persisted = r_u32(state + 0xc);
    w_u16(state + 0x2c, health - 1);
    if (persisted != 0)
        w_u16(persisted, health - 1);
    uint32 mode = r_u32(object + 0x150);
    if ((mode == 2 && r_s16(object + 0x5e) >= 14) || (mode == 3 && r_s16(object + 0x5e) < 6))
    {
        w_u32(object + 0x150, 5);
        w_u16(object + 0x6e, 3);
    }
    else
    {
        w_u32(object + 0x150, 4);
        w_u16(object + 0x6e, 2);
    }
    w_u16(object + 0x5e, 0);
}

uint32 G3VaultCodeEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_68");
    uint32 frame = guest_stack - 0x18;
    uint32 data = 0;
    uint32 state = 0;
    if (object != 0)
    {
        uint32 resource = r_u32(object + 0x1c);
        state = object + 0x128;
        data = r_u32(resource + 0x1c);
    }
    if (data == 0)
        return 1;
    if (message == 1)
    {
        G3VaultCodeInitialize(state, data);
        return 1;
    }
    if (message == 2)
    {
        G3VaultCodeUpdate(object, game, state, data, module_base, frame);
        return 1;
    }
    return 0;
}

uint32 G3AttachmentCount80036FC0(uint32 object)
{
    uint32 intro = r_u32(object + 0x20);
    uint32 entries = r_u32(intro + 4);
    return entries != 0 ? r_u32(entries) : 0;
}

void G3VaultCodeInitialize(uint32 state, uint32 data)
{
    FUNCTION_MARKER(0x0000007c, "G3L_68");
    w_u8(state + 9, 1);
    w_u8(state, 0);
    w_u8(state + 1, 0);
    w_u16(state + 2, 0);
    w_u8(state + 8, 0);
    w_u16(state + 0xa, 0);
    w_u32(state + 4, 60);
    for (uint32 index = 0; index < 8; ++index)
        w_u8(state + index + 0xc, r_u8(data + index + 0xc));
}

void G3VaultCodeCollect(uint32 object, uint32 state, uint32 data, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000cc, "G3L_68");
    uint32 frame = guest_stack - 0x28;
    sint32 count = (sint32)G3AttachmentCount80036FC0(object);
    if (count != r_s16(data) + 1)
        w_u8(state + 1, 1);
    for (sint32 index = 0; index < count; ++index)
    {
        uint32 linked = G3AttachedObject80036FE4(object, index);
        if (linked != 0 && linked != object)
        {
            uint32 slot = (uint32)r_s16(state + 2);
            w_u32(state + (slot << 2) + 0x14, linked);
            slot = (uint32)r_s16(state + 2);
            uint32 symbol = r_u8(data + slot + 0xc);
            w_u8(state + slot + 0xc, symbol);
            w_u32(linked + 0x10, r_u32(linked + 0x10) | 0x80);
            w_u16(state + 2, r_u16(state + 2) + 1);
        }
    }
    if (r_s16(state + 2) == r_s16(data))
        w_u8(state, 1);
    else
        w_u8(state + 1, 1);
}

void G3VaultCodeUpdate(uint32 object, uint32 game, uint32 state, uint32 data, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000001d4, "G3L_68");
    uint32 frame = guest_stack - 0x28;
    if (r_u8(state + 1) != 0)
        return;
    if (r_u8(state) == 0)
    {
        G3VaultCodeCollect(object, state, data, frame);
        return;
    }
    uint32 delay = r_u32(state + 4);
    if ((sint32)delay > 0)
        w_u32(state + 4, delay - 1);
    else
    {
        sint32 period = r_s16(data + 0xa);
        sint32 elapsed = r_s16(state + 0xa);
        uint32 next = r_u16(state + 0xa) + 1;
        if (elapsed < period)
        {
            w_u16(state + 0xa, next);
            period = r_s16(data + 0xa);
            if ((sint16)next == period)
            {
                uint32 blink = r_u8(state + 9);
                w_u16(state + 0xa, 0);
                w_u8(state + 9, blink ^ 1);
            }
        }
        uint32 result = G3VaultCodeInput(game, state, data, module_base, frame);
        if ((sint32)result >= 0)
        {
            G3Function80011430(result, frame);
            G3Function8003F1C4(0x802, 0x74, 0x40, 0x64);
        }
        if ((sint32)result >= 0 || result == 0xfffffffe)
        {
            uint32 level = r_u32(game + 0x48);
            uint32 scripts = r_u32(level + 0xc4);
            uint32 player = r_u32(game + 0x50);
            G3Function80018DE0(player, scripts + 4, frame);
        }
    }
    G3VaultCodeDisplay(state);
}

void G3VaultCodeDisplay(uint32 state)
{
    FUNCTION_MARKER(0x00000314, "G3L_68");
    uint32 cursor = state;
    sint32 index = 0;
    while (index < r_s16(state + 2))
    {
        uint32 symbol = (uint32)r_s8(state + (uint32)index + 0xc);
        uint32 linked = r_u32(cursor + 0x14);
        w_u16(linked + 0x9e, symbol);
        linked = r_u32(cursor + 0x14);
        uint32 flags = r_u32(linked + 0x10);
        ++index;
        w_u32(linked + 0x10, flags | 0x200000);
        cursor += 4;
    }
    if (r_s8(state + 9) != 0)
    {
        uint32 selected = (uint32)r_s8(state + 8);
        uint32 linked = r_u32(state + (selected << 2) + 0x14);
        w_u32(linked + 0x10, r_u32(linked + 0x10) & 0xffdfffff);
    }
}

uint32 G3VaultCodeCheck(uint32 state, uint32 data, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000003b0, "G3L_68");
    uint32 frame = guest_stack - 0x20;
    uint32 offset = 0;
    sint32 index = 0;
    while (index < r_s16(data + 2))
    {
        uint32 codes = r_u32(data + 0x14);
        uint32 code = codes + offset;
        uint32 symbol = 0;
        while (symbol < 8)
        {
            sint32 actual = r_s8(state + symbol + 0xc);
            uint32 expected = r_u8(code + symbol);
            if ((uint32)actual != expected)
                break;
            ++symbol;
        }
        ++index;
        if (symbol == 8)
        {
            G3Function8003F128(0x827, 0);
            w_u8(state + 1, 1);
            codes = r_u32(data + 0x14);
            return r_u32(codes + offset + 8);
        }
        offset += 0xc;
    }
    G3Function8003F128(0x800, 0);
    return 0xffffffff;
}

uint32 G3VaultCodeInput(uint32 game, uint32 state, uint32 data, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000484, "G3L_68");
    uint32 frame = guest_stack - 0x20;
    uint32 previous = r_u32(module_base + 0x6f4);
    uint32 current = r_u32(game + 0x98);
    w_u32(module_base + 0x6f4, current);
    w_u32(module_base + 0x6f8, previous);
    if ((current & 0x80) != 0 && (previous & 0x80) == 0)
        return G3VaultCodeCheck(state, data, frame);
    if ((r_u32(module_base + 0x6f4) & 0x10) != 0 && (r_u32(module_base + 0x6f8) & 0x10) == 0)
    {
        G3Function8003F128(0x800, 0);
        w_u8(state + 1, 1);
        return 0xfffffffe;
    }
    if ((r_u32(module_base + 0x6f4) & 8) != 0 && (r_u32(module_base + 0x6f8) & 8) == 0)
    {
        G3Function8003F128(0x829, 0);
        uint32 next = r_u8(state + 8) + 1;
        sint32 count = r_s16(state + 2);
        w_u8(state + 8, next);
        if ((sint8)next >= count)
            w_u8(state + 8, 0);
        return 0xffffffff;
    }
    if ((r_u32(module_base + 0x6f4) & 4) != 0 && (r_u32(module_base + 0x6f8) & 4) == 0)
    {
        G3Function8003F128(0x829, 0);
        uint32 next = r_u8(state + 8) - 1;
        w_u8(state + 8, next);
        if ((sint8)next < 0)
            w_u8(state + 8, next + r_u8(state + 2));
        return 0xffffffff;
    }
    if ((r_u32(module_base + 0x6f4) & 1) != 0 && (r_u32(module_base + 0x6f8) & 1) == 0)
    {
        G3Function8003F128(0x802, 0);
        uint32 selected = (uint32)r_s8(state + 8);
        uint32 cell = state + selected + 0xc;
        w_u8(cell, r_u8(cell) + 1);
        selected = (uint32)r_s8(state + 8);
        sint32 count = r_s16(data + 8);
        cell = state + selected + 0xc;
        if (r_s8(cell) >= count)
            w_u8(cell, 0);
        return 0xffffffff;
    }
    if ((r_u32(module_base + 0x6f4) & 2) != 0 && (r_u32(module_base + 0x6f8) & 2) == 0)
    {
        G3Function8003F128(0x802, 0);
        uint32 selected = (uint32)r_s8(state + 8);
        uint32 cell = state + selected + 0xc;
        w_u8(cell, r_u8(cell) - 1);
        selected = (uint32)r_s8(state + 8);
        cell = state + selected + 0xc;
        sint32 symbol = r_s8(cell);
        uint32 raw_symbol = r_u8(cell);
        if (symbol < 0)
            w_u8(cell, raw_symbol + r_u8(data + 8));
    }
    return 0xffffffff;
}

uint32 G3WakballEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_46");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3WakballInitialize(object, game, frame);
        return 1;
    case 2:
        G3WakballUpdate(object, frame);
        return 1;
    case 3:
        G3WakballCollide(object, game, frame);
        return 1;
    case 2017:
        G3WakballLaunch(object, payload, frame);
        return 1;
    default:
        return 0;
    }
}

void G3WakballConfigure(uint32 object, uint32 commands, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000a0, "G3L_46");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    w_u32(guest_stack + 4, commands);
    if (commands == 0)
        return;
    while (G3Function80085DFC(frame + 0x34, frame + 0x10, frame + 0x14, frame + 0x18) != 0)
    {
        sint32 kind = r_s16(frame + 0x10);
        if (kind == 0x1e)
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0x16, r_u16(data));
        }
        else if (kind == 0x15)
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0x1a, r_u16(data));
        }
        else if (kind == 0x95)
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0x1c, r_u16(data));
        }
        else if (kind == 0x52)
        {
            uint32 name = r_u32(frame + 0x14);
            uint32 game = r_u32(0x80097048);
            uint32 resource = G3FindLevelResource800397B0(name, game);
            w_u32(state, resource);
        }
    }
}

void G3WakballInitialize(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000188, "G3L_46");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    w_u16(state + 0x16, 30);
    w_u16(state + 0x1c, 10);
    w_u16(state + 0x1a, 80);
    uint32 resource = r_u32(object + 0x1c);
    uint32 commands = r_u32(resource + 0x1c);
    G3WakballConfigure(object, commands, frame);
    w_u16(state + 6, 0);
    w_u16(state + 4, 0);
    w_u16(state + 8, 25);
    uint32 owner = r_u32(object + 0x2c);
    if (owner != 0)
    {
        uint32 target = r_u32(owner + 0xec);
        if (target != 0)
            G3DispatchStateEvent(target, owner, game, 2006, object, frame);
    }
    uint32 angle_x = r_u16(object + 0x60);
    w_u32(object + 0x150, 0);
    w_u16(state + 0xc, angle_x);
    w_u16(state + 0xe, r_u16(object + 0x62));
    uint32 angle_z = r_u16(object + 0x64);
    w_u16(state + 0x1e, 0);
    w_u16(state + 0x20, 2);
    w_u16(state + 0x24, 0);
    w_u16(state + 0x22, 0);
    w_u16(state + 0x10, angle_z);
}

void G3WakballLaunch(uint32 object, uint32 data, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000258, "G3L_46");
    uint32 frame = guest_stack - 0x20;
    uint32 angle = (uint32)(sint32)(sint16)(r_u16(object + 0x64) - 0x400);
    uint32 state = object + 0x104;
    uint32 sine = G3Function8005B470(angle);
    uint32 speed = (uint32)r_s16(data + 4);
    uint32 product = speed * sine;
    w_u16(state + 4, (uint32)((sint32)product >> 12));
    uint32 cosine = G3Function8005B590(angle);
    speed = (uint32)r_s16(data + 4);
    product = speed * cosine;
    w_u16(state + 6, (uint32)((sint32)product >> 12));
    w_u16(state + 8, r_u16(data + 2));
    w_u16(state + 0x14, r_u16(data));
    w_u16(state + 0x1e, r_u16(data + 6));
    w_u16(state + 0x20, r_u16(data + 8));
    w_u16(state + 0x1a, r_u16(data + 4));
    w_u16(state + 0x22, r_u16(data + 0xa));
    w_u16(state + 0x24, r_u16(data + 0xc));
}

void G3WakballDestroy(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000334, "G3L_46");
    uint32 frame = guest_stack - 0x18;
    uint32 resource = r_u32(object + 0x104);
    if (resource != 0)
    {
        uint32 owner = r_u32(resource + 0xc);
        uint32 effect_resource = r_u32(owner);
        uint32 dimensions = r_u32(effect_resource + 0x1c);
        uint32 width = r_u32(dimensions + 8);
        uint32 height = r_u32(dimensions + 0xc);
        uint32 pool = r_u32(0x800a27fc);
        uint32 count = (uint32)(sint32)(sint16)(width * height);
        G3Function800444E4(object + 0x58, effect_resource, pool, count, frame);
    }
    G3Function80038798(object, 0, 0, frame);
}

void G3WakballUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000003b4, "G3L_46");
    uint32 frame = guest_stack - 0x70;
    sint32 mode = r_s32(object + 0x150);
    uint32 state = object + 0x104;
    if (mode == 0 || mode == 2)
    {
        uint32 x = r_u16(object + 0x58);
        uint32 dx = r_u16(state + 4);
        w_u16(object + 0x58, x + dx);
        uint32 y = r_u16(object + 0x5a);
        uint32 dy = r_u16(state + 6);
        w_u16(object + 0x5a, y + dy);
        uint32 z = r_u16(object + 0x5c);
        uint32 dz = r_u16(state + 8);
        w_u16(object + 0x5c, z + dz);
        dz = r_u16(state + 8);
        uint32 gravity = r_u16(state + 0x14);
        w_u16(state + 8, dz + gravity);
        sint32 turn = r_s16(state + 0x1e);
        uint32 turn_bits = r_u16(state + 0x1e);
        if (turn == 0)
            return;
        uint32 axis = (uint32)r_s16(state + 0x20);
        uint32 angle_pointer = state + (axis << 1) + 0xc;
        uint32 angle = r_u16(angle_pointer);
        uint32 matrix = mode == 0 ? frame + 0x10 : frame + 0x38;
        uint32 output = mode == 0 ? frame + 0x30 : frame + 0x58;
        w_u16(angle_pointer, angle + turn_bits);
        G3Function8002F718(matrix);
        G3Function8005B94C((uint32)r_s16(state + 0xc), matrix, frame);
        G3Function8005BA5C((uint32)r_s16(state + 0xe), matrix, frame);
        G3Function8005BB6C((uint32)r_s16(state + 0x10), matrix, frame);
        G3Function800202A4(matrix, output, 0x15, frame);
        w_u16(object + 0x60, r_u16(output));
        w_u16(object + 0x62, r_u16(output + 2));
        w_u16(object + 0x64, r_u16(output + 4));
    }
    else if (mode == 1)
    {
        uint32 timer = r_u16(state + 0x18) - 1;
        w_u16(state + 0x18, timer);
        if ((sint16)timer <= 0)
            G3WakballDestroy(object, frame);
    }
    else if (mode == 3)
        G3WakballDestroy(object, frame);
}

void G3WakballCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000610, "G3L_46");
    uint32 frame = guest_stack - 0x48;
    uint32 contact = r_u32(object + 0xb0);
    uint32 state = object + 0x104;
    G3Function8003C548(contact, frame + 0x28, frame + 0x29);
    uint32 other = r_u32(contact + 0x14);
    uint32 player = r_u32(0x8009efc4);
    if (other == player)
    {
        uint32 first = r_u8(frame + 0x28);
        if (first - 6 < 4)
        {
            uint32 second = r_u8(frame + 0x29);
            if ((second == 9 || second - 3 < 3) && r_u32(object + 0x150) != 1)
            {
                G3Function80074044(object, game, frame);
                w_u32(object + 0x150, 3);
                return;
            }
        }
        uint32 second = r_u8(frame + 0x29);
        if (second - 6 >= 4)
            return;
        first = r_u8(frame + 0x28);
        if (first != 9 && first - 3 >= 3)
            return;
        sint32 mode = r_s32(object + 0x150);
        if (mode < 0 || mode >= 2)
            return;
        uint32 owner = r_u32(object + 0x2c);
        w_u32(object + 0x150, 2);
        uint32 angle;
        if (owner != 0)
        {
            uint32 speed = (uint32)r_s16(state + 0x1a);
            uint32 gravity = (uint32)r_s16(state + 0x14);
            w_u32(frame + 0x14, 32000);
            w_u32(frame + 0x18, frame + 0x2a);
            w_u32(frame + 0x10, 0);
            w_u32(frame + 0x1c, frame + 0x2c);
            if (G3Function80036B80(object + 0x58, owner + 0x58, speed, gravity, 0, 32000, frame + 0x2a, frame + 0x2c, frame) == 0)
            {
                w_u32(object + 0x150, 2);
                return;
            }
            uint32 x = (uint32)r_s16(object + 0x58);
            player = r_u32(0x8009efc4);
            uint32 y = (uint32)r_s16(object + 0x5a);
            uint32 player_y = (uint32)r_s16(player + 0x5a);
            uint32 player_x = (uint32)r_s16(player + 0x58);
            uint32 heading = G3Function8002F240(y - player_y, x - player_x, frame);
            sint32 outgoing = (sint16)heading;
            uint32 owner_y = (uint32)r_s16(owner + 0x5a);
            y = (uint32)r_s16(object + 0x5a);
            uint32 owner_x = (uint32)r_s16(owner + 0x58);
            x = (uint32)r_s16(object + 0x58);
            heading = G3Function8002F240(owner_y - y, owner_x - x, frame);
            sint32 target_angle = (sint16)heading;
            sint32 difference = outgoing - target_angle;
            sint32 limit = r_s16(state + 0x22);
            sint32 magnitude = difference < 0 ? -difference : difference;
            if (magnitude < limit)
            {
                uint32 factor = (uint32)r_s16(state + 0x24);
                sint32 product = (sint32)((uint32)difference * factor);
                difference = product / limit;
            }
            angle = (uint32)difference + (uint32)target_angle;
        }
        else
        {
            uint32 speed = r_u16(state + 0x1a);
            uint32 vertical = r_u16(state + 0x1c);
            w_u16(frame + 0x2c, speed);
            w_u16(frame + 0x2a, vertical);
            uint32 x = (uint32)r_s16(object + 0x58);
            player = r_u32(0x8009efc4);
            uint32 y = (uint32)r_s16(object + 0x5a);
            uint32 player_y = (uint32)r_s16(player + 0x5a);
            uint32 player_x = (uint32)r_s16(player + 0x58);
            uint32 heading = G3Function8002F240(y - player_y, x - player_x, frame);
            angle = (uint32)(sint32)(sint16)heading;
        }
        uint32 sine = G3Function8005B470(angle);
        uint32 speed = (uint32)r_s16(frame + 0x2c);
        uint32 product = speed * sine;
        w_u16(state + 4, (uint32)((sint32)product >> 12));
        uint32 cosine = G3Function8005B590(angle);
        speed = (uint32)r_s16(frame + 0x2c);
        product = speed * cosine;
        uint32 vertical = r_u16(frame + 0x2a);
        w_u16(state + 8, vertical);
        w_u16(state + 6, (uint32)((sint32)product >> 12));
        return;
    }
    uint32 owner = r_u32(object + 0x2c);
    if (other == owner)
    {
        if (r_u32(object + 0x150) == 2)
            w_u32(object + 0x150, 3);
        return;
    }
    uint32 second = r_u8(frame + 0x29);
    if (second - 6 < 4)
    {
        uint32 first = r_u8(frame + 0x28);
        if (first == 9 || first - 3 < 3)
        {
            w_u32(object + 0x150, 3);
            return;
        }
    }
    sint32 kind = r_s16(contact + 6);
    uint32 surface = r_u32(contact + 0xc);
    uint32 delta = contact + 0x28;
    bool correct_position = false;
    if (kind == 3)
        correct_position = (r_u16(surface + 6) & 0xf) != 8;
    else if (kind == 1)
        correct_position = (r_u32(surface) & 0x2000) != 0;
    else if (kind == 5)
        correct_position = (r_u16(surface) & 0x2000) != 0;
    else if (kind == 2)
        correct_position = (r_u16(surface + 6) & 0xf) != 8;
    if (correct_position)
    {
        uint32 x = r_u16(object + 0x58);
        uint32 dx = r_u16(delta);
        w_u16(object + 0x58, x + dx);
        uint32 y = r_u16(object + 0x5a);
        uint32 dy = r_u16(delta + 2);
        w_u16(object + 0x5a, y + dy);
        uint32 z = r_u16(object + 0x5c);
        uint32 dz = r_u16(delta + 4);
        w_u16(object + 0x5c, z + dz);
        G3Function8005F3A8(object, delta);
    }
    sint32 mode = r_s32(object + 0x150);
    if (mode == 1)
        return;
    if (mode == 0)
    {
        if (G3Function800388C8(contact, frame + 0x20, game, frame) != 0 && r_s16(frame + 0x24) >= 0xe67)
        {
            w_u32(object + 0x150, 1);
            w_u16(state + 0x18, r_u16(state + 0x16));
        }
        else
            w_u32(object + 0x150, 3);
    }
    else if (mode == 2)
        w_u32(object + 0x150, 3);
}

uint32 G3WaterFanEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_56");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3WaterFanInitialize(object, module_base, frame);
        return 1;
    case 2:
        G3WaterFanUpdate(object, game, frame);
        return 1;
    case 3:
        G3WaterFanCollide();
        return 1;
    case 1000:
        G3WaterFanAffect(object, payload, frame);
        return 1;
    default:
        return 0;
    }
}

void G3WaterFanConfigure(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000094, "G3L_56");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    w_u32(guest_stack + 4, commands);
    if (commands == 0)
        return;
    w_u16(state + 0x14, 0);
    while (G3Function80085DFC(frame + 0x34, frame + 0x10, frame + 0x14, frame + 0x18) != 0)
    {
        uint32 kind = (uint32)(sint32)(sint16)(r_u16(frame + 0x10) - 5);
        if (kind >= 12)
            continue;
        uint32 target = r_u32(module_base + 0xb18 + kind * 4);
        switch (target - module_base)
        {
        case 0x11c:
            w_u16(state + 0x14, 1);
            break;
        case 0x128:
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 radius = (uint32)r_s16(data);
            uint32 inner = radius * radius;
            w_u32(state, inner);
            radius = (uint32)r_s16(data + 2);
            uint32 outer = radius * radius;
            w_u32(state + 4, outer);
            w_u32(state + 8, outer - inner);
            w_u16(state + 0x16, r_u16(data));
            break;
        }
        case 0x16c:
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0xc, r_u16(data));
            uint32 outer = r_u16(data + 2);
            uint32 inner = r_u16(state + 0xc);
            w_u16(state + 0xe, outer);
            w_u16(state + 0x10, outer - inner);
            break;
        }
        case 0x198:
        case 0x1ac:
        case 0x1f0:
        case 0x204:
        case 0x218:
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 value = r_u16(data);
            uint32 offset = target - module_base == 0x198 ? 0x12 :
                target - module_base == 0x1ac ? 0x16 :
                target - module_base == 0x1f0 ? 0x1a :
                target - module_base == 0x204 ? 0x1c : 0x1e;
            w_u16(state + offset, value);
            break;
        }
        case 0x1c0:
        case 0x22c:
        case 0x290:
        {
            uint32 data = r_u32(frame + 0x14);
            sint32 value = r_s16(data);
            uint32 fixed = (uint32)((value * 4096) / 100);
            if (target - module_base == 0x1c0)
                w_u16(state + 0x18, fixed);
            else
                w_u32(state + (target - module_base == 0x22c ? 0x24 : 0x28), fixed);
            break;
        }
        case 0x25c:
        {
            uint32 data = r_u32(frame + 0x14);
            sint32 value = r_s16(data);
            w_u16(state + 0x22, (uint32)((value * 256) / 60));
            break;
        }
        default:
            {
#ifndef NDEBUG
            fprintf(stderr, "G3 unresolved waterfan property target %08X\n", (unsigned int)target);
#endif
            }
            abort();
        }
    }
}

void G3WaterFanInitialize(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000002d8, "G3L_56");
    uint32 frame = guest_stack - 0x20;
    uint32 flags = r_u32(object + 0x10);
    uint32 state = object + 0x104;
    w_u32(object + 0x104, 0x9c40);
    w_u32(object + 0x10, flags | 0x800);
    w_u32(state + 4, 0x27100);
    uint32 inner = r_u32(object + 0x104);
    w_u16(state + 0xc, 400);
    w_u16(state + 0x12, 40);
    w_u16(state + 0x14, 1);
    w_u16(state + 0x16, 350);
    w_u16(state + 0x18, 0x400);
    w_u16(state + 0x1a, 100);
    w_u16(state + 0x1c, 5);
    w_u16(state + 0x1e, 45);
    w_u32(state + 0x24, 0x800);
    w_u32(state + 0x28, 0x1000);
    w_u16(state + 0x22, 0x100);
    uint32 inner_height = r_u16(state + 0xc);
    w_u16(state + 0xe, 1000);
    w_u16(state + 0x20, 0);
    w_u32(state + 8, 0x27100 - inner);
    w_u16(state + 0x10, 1000 - inner_height);
    uint32 resource = r_u32(object + 0x1c);
    uint32 commands = r_u32(resource + 0x1c);
    G3WaterFanConfigure(object, commands, module_base, frame);
    commands = r_u32(object + 0x28);
    G3WaterFanConfigure(object, commands, module_base, frame);
    if (r_s16(state + 0x14) != 0)
    {
        uint32 player = r_u32(0x8009efc4);
        uint32 player_data = r_u32(player + 0x24);
        uint32 strength = r_u16(state + 0x12);
        uint32 gravity = r_u16(player_data + 4);
        w_u16(state + 0x12, strength - gravity);
    }
}

uint32 G3WaterFanForceAt(uint32 object, uint32 position, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000003f0, "G3L_56");
    uint32 frame = guest_stack - 0x60;
    uint32 matrix = r_u32(object + 0x30);
    uint32 state = object + 0x104;
    if (matrix != 0)
    {
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 coordinate = r_u16(position + axis * 2);
            uint32 origin = r_u16(object + 0x58 + axis * 2);
            w_u16(frame + 0x10 + axis * 2, coordinate - origin);
        }
        TransposeMatrix((MATRIX *)psx_addr(matrix, 18), (MATRIX *)psx_addr(frame + 0x28, 18));
        ApplyMatrixSV((MATRIX *)psx_addr(frame + 0x28, 20), (SVECTOR *)psx_addr(frame + 0x10, 8), (SVECTOR *)psx_addr(frame + 0x18, 6));
        sint32 height = r_s16(frame + 0x1c);
        if (height >= 0)
        {
            sint32 maximum_height = r_s16(state + 0xe);
            if (height < maximum_height)
            {
                uint32 x = (uint32)r_s16(frame + 0x18);
                uint32 y = (uint32)r_s16(frame + 0x1a);
                uint32 outer = r_u32(state + 4);
                uint32 distance = x * x + y * y;
                if ((sint32)distance < (sint32)outer)
                {
                    uint32 inner = r_u32(object + 0x104);
                    uint32 force = (uint32)r_s16(state + 0x12);
                    uint32 radial_falloff = outer - distance;
                    if ((sint32)inner < (sint32)distance)
                    {
                        sint32 product = (sint32)(force * radial_falloff);
                        sint32 divisor = r_s32(state + 8);
                        force = divisor == 0 ? (product < 0 ? 1 : 0xffffffff) : (uint32)((sint64)product / divisor);
                    }
                    sint32 minimum_height = r_s16(state + 0xc);
                    uint32 vertical_falloff = (uint32)maximum_height - (uint32)height;
                    if (minimum_height < height)
                    {
                        sint32 product = (sint32)(force * vertical_falloff);
                        sint32 divisor = r_s16(state + 0x10);
                        force = divisor == 0 ? (product < 0 ? 1 : 0xffffffff) : (uint32)((sint64)product / divisor);
                    }
                    w_u16(frame + 0x22, 0);
                    w_u16(frame + 0x20, 0);
                    w_u16(frame + 0x24, force);
                    ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(frame + 0x20, 8), (SVECTOR *)psx_addr(frame + 0x20, 6));
                    w_u16(output, r_u16(frame + 0x20));
                    w_u16(output + 2, r_u16(frame + 0x22));
                    w_u16(output + 4, r_u16(frame + 0x24));
                    return 1;
                }
            }
        }
    }
    w_u16(output + 4, 0);
    w_u16(output + 2, 0);
    w_u16(output, 0);
    return 0;
}

void G3WaterFanUpdate(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000598, "G3L_56");
    uint32 frame = guest_stack - 0x68;
    uint32 player = r_u32(0x8009efc4);
    uint32 state = object + 0x104;
    sint32 upright = r_s16(state + 0x14);
    uint32 matrix = r_u32(object + 0x30);
    uint32 player_data = r_u32(player + 0x24);
    if (upright != 0)
    {
        uint32 player_x = r_u16(player + 0x58);
        uint32 x = r_u16(object + 0x58);
        uint32 dx = player_x - x;
        w_u16(frame + 0x18, dx);
        uint32 player_y = r_u16(player + 0x5a);
        uint32 y = r_u16(object + 0x5a);
        uint32 dy = player_y - y;
        w_u16(frame + 0x1a, dy);
        uint32 player_z = r_u16(player + 0x5c);
        uint32 z = r_u16(object + 0x5c);
        uint32 dz = player_z - z;
        w_u16(frame + 0x1c, dz);
        sint32 height = (sint16)dz;
        if (height >= 0)
        {
            sint32 maximum_height = r_s16(state + 0xe);
            if (height < maximum_height)
            {
                uint32 sx = (uint32)(sint32)(sint16)dx;
                uint32 sy = (uint32)(sint32)(sint16)dy;
                uint32 outer = r_u32(state + 4);
                uint32 distance = sx * sx + sy * sy;
                if ((sint32)distance < (sint32)outer)
                {
                    uint32 inner = r_u32(object + 0x104);
                    uint32 force = (uint32)r_s16(state + 0x12);
                    uint32 radial_falloff = outer - distance;
                    if ((sint32)inner < (sint32)distance)
                    {
                        sint32 product = (sint32)(force * radial_falloff);
                        sint32 divisor = r_s32(state + 8);
                        force = divisor == 0 ? (product < 0 ? 1 : 0xffffffff) : (uint32)((sint64)product / divisor);
                    }
                    sint32 minimum_height = r_s16(state + 0xc);
                    uint32 vertical_falloff = (uint32)maximum_height - (uint32)height;
                    if (minimum_height < height)
                    {
                        sint32 product = (sint32)(force * vertical_falloff);
                        sint32 divisor = r_s16(state + 0x10);
                        force = divisor == 0 ? (product < 0 ? 1 : 0xffffffff) : (uint32)((sint64)product / divisor);
                    }
                    uint32 callback = r_u32(player + 0xec);
                    if (callback != 0)
                    {
                        uint32 gravity = (uint32)r_s16(player_data + 4);
                        G3DispatchStateEvent(callback, player, game, 1006, force + gravity, frame);
                    }
                }
            }
        }
    }
    else if (G3WaterFanForceAt(object, player + 0x58, frame + 0x20, frame) != 0)
    {
        uint32 current_player = r_u32(0x8009efc4);
        uint32 callback = r_u32(current_player + 0xec);
        if (callback != 0)
            G3DispatchStateEvent(callback, current_player, game, 1008, frame + 0x20, frame);
    }
    uint32 phase = r_u16(state + 0x20);
    uint32 increment = r_u16(state + 0x22);
    phase += increment;
    sint32 particles = (sint32)(phase << 16) >> 24;
    w_u16(state + 0x20, phase & 0xff);
    while (particles > 0)
    {
        sint32 random_radius = (sint32)G3SdkRandom80027FA8();
        sint32 radius = r_s16(state + 0x16);
        uint32 radial = radius == 0 ? (uint32)random_radius : (uint32)((sint64)random_radius % radius);
        sint32 random_angle = (sint32)G3SdkRandom80027FA8();
        uint32 angle = (uint32)(random_angle % 4096);
        uint32 sine = G3Function8005B470(angle);
        uint32 product = radial * sine;
        uint32 position = upright != 0 ? frame + 0x30 : frame + 0x38;
        w_u16(position, (uint32)((sint32)product >> 12));
        uint32 cosine = G3Function8005B590(angle);
        product = radial * cosine;
        w_u16(position + 4, 1);
        if (upright != 0)
        {
            uint32 dx = r_u16(frame + 0x30);
            uint32 dy = (uint32)((sint32)product >> 12);
            w_u16(frame + 0x32, dy);
            uint32 x = r_u16(object + 0x58);
            w_u16(frame + 0x30, dx + x);
            uint32 y = r_u16(object + 0x5a);
            w_u16(frame + 0x32, dy + y);
            uint32 z = r_u16(object + 0x5c);
            --particles;
            w_u16(frame + 0x2c, 0);
            w_u16(frame + 0x2a, 0);
            w_u16(frame + 0x28, 0);
            w_u16(frame + 0x34, z + 1);
            uint32 speed = r_u16(state + 0x1a);
            w_u16(frame + 0x22, 0);
            w_u16(frame + 0x20, 0);
            w_u16(frame + 0x2c, speed);
            w_u16(frame + 0x24, r_u16(state + 0x1c));
            w_u32(frame + 0x10, (uint32)r_s16(state + 0x1e));
            w_u32(frame + 0x14, r_u32(state + 0x24));
            G3Function800454F4(frame + 0x30, frame + 0x28, frame + 0x20, object, frame);
        }
        else
        {
            w_u16(frame + 0x3a, (uint32)((sint32)product >> 12));
            if (matrix != 0)
                ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(frame + 0x38, 8), (SVECTOR *)psx_addr(frame + 0x38, 6));
            uint32 dx = r_u16(frame + 0x38);
            uint32 x = r_u16(object + 0x58);
            w_u16(frame + 0x38, dx + x);
            uint32 dy = r_u16(frame + 0x3a);
            uint32 y = r_u16(object + 0x5a);
            w_u16(frame + 0x3a, dy + y);
            uint32 dz = r_u16(frame + 0x3c);
            uint32 z = r_u16(object + 0x5c);
            w_u16(frame + 0x34, 0);
            w_u16(frame + 0x32, 0);
            w_u16(frame + 0x30, 0);
            w_u16(frame + 0x3c, dz + z);
            w_u16(frame + 0x34, r_u16(state + 0x1a));
            if (matrix != 0)
                ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(frame + 0x30, 8), (SVECTOR *)psx_addr(frame + 0x30, 6));
            w_u16(frame + 0x2a, 0);
            w_u16(frame + 0x28, 0);
            w_u16(frame + 0x2c, r_u16(state + 0x1c));
            w_u32(frame + 0x10, (uint32)r_s16(state + 0x1e));
            uint32 scale = r_u32(state + 0x24);
            --particles;
            w_u32(frame + 0x14, scale);
            G3Function800454F4(frame + 0x38, frame + 0x30, frame + 0x28, object, frame);
        }
    }
}

void G3WaterFanAffect(uint32 object, uint32 effect, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000009d4, "G3L_56");
    uint32 frame = guest_stack - 0x30;
    uint32 position = effect + 0x1c;
    uint32 state = object + 0x104;
    uint32 output = r_u32(effect + 0x14);
    uint32 flags = r_u32(effect + 0xc);
    uint32 game = r_u32(0x80097048);
    w_u32(effect + 0xc, flags | 0x2000);
    uint32 camera = r_u32(game + 0x4c);
    G3Function80041CE4(output + 0xc, camera, position, 0, frame);
    G3Function80044B60(effect, frame);
    if (G3WaterFanForceAt(object, position, frame + 0x10, frame) != 0)
    {
        uint32 x = (uint32)r_s16(frame + 0x10);
        uint32 factor = r_u32(state + 0x28);
        uint32 product = x * factor;
        uint32 y = (uint32)r_s16(frame + 0x12);
        w_u16(frame + 0x10, (uint32)((sint32)product >> 12));
        factor = r_u32(state + 0x28);
        product = y * factor;
        uint32 z = (uint32)r_s16(frame + 0x14);
        w_u16(frame + 0x12, (uint32)((sint32)product >> 12));
        factor = r_u32(state + 0x28);
        product = z * factor;
        w_u16(frame + 0x14, (uint32)((sint32)product >> 12));
    }
    sint32 old_x = r_s16(effect + 0x4c);
    sint32 damping = r_s16(state + 0x18);
    uint32 damped = G3Function80039DBC(old_x, damping);
    uint32 force = r_u16(frame + 0x10);
    sint32 old_y = r_s16(effect + 0x4e);
    w_u16(effect + 0x4c, force + damped);
    damping = r_s16(state + 0x18);
    damped = G3Function80039DBC(old_y, damping);
    force = r_u16(frame + 0x12);
    sint32 old_z = r_s16(effect + 0x50);
    w_u16(effect + 0x4e, force + damped);
    damping = r_s16(state + 0x18);
    damped = G3Function80039DBC(old_z, damping);
    force = r_u16(frame + 0x14);
    w_u16(effect + 0x50, force + damped);
}

void G3WaterFanCollide(void)
{
    FUNCTION_MARKER(0x00000b10, "G3L_56");
    // Original body contains only JR RA and its NOP delay slot
}

uint32 G3ZiplineEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_40");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3ZiplineInitialize(object, frame);
        return 1;
    case 2:
        G3ZiplineUpdate(object);
        return 1;
    case 3:
        G3ZiplineCollide(object, game, frame);
        return 1;
    case 1000:
        w_u32(payload, r_u32(object + 0x10c));
        return 1;
    case 1001:
    {
        uint32 sound = r_u32(object + 0x104);
        if (sound != 0)
        {
            G3Function8003F18C(sound);
            w_u32(object + 0x104, 0);
        }
        return 1;
    }
    default:
        return 0;
    }
}

void G3ZiplineConfigure(uint32 object, uint32 commands, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000c4, "G3L_40");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    w_u32(guest_stack + 4, commands);
    if (commands == 0)
        return;
    while (G3Function80085DFC(frame + 0x2c, frame + 0x10, frame + 0x14, frame + 0x18) != 0)
    {
        sint32 kind = r_s16(frame + 0x10);
        if (kind == 0x25)
        {
            uint32 data = r_u32(frame + 0x14);
            w_u32(state + 4, r_u32(data));
        }
        else if (kind == 0x26)
        {
            uint32 data = r_u32(frame + 0x14);
            w_u32(state + 8, r_u32(data));
        }
        else if (kind == 0x27)
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0xe, r_u16(data));
        }
    }
}

void G3ZiplineInitialize(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000180, "G3L_40");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    w_u32(state + 8, 0);
    w_u32(state + 4, 0);
    w_u16(state + 0xe, 60);
    uint32 resource = r_u32(object + 0x1c);
    uint32 commands = r_u32(resource + 0x1c);
    G3ZiplineConfigure(object, commands, frame);
    commands = r_u32(object + 0x28);
    G3ZiplineConfigure(object, commands, frame);
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x10, flags | 0x00100c00);
    w_u16(state + 0xc, 0);
    w_u32(object + 0x104, 0);
}

void G3ZiplineUpdate(uint32 object)
{
    FUNCTION_MARKER(0x000001f8, "G3L_40");
    uint32 state = object + 0x104;
    sint32 timer = r_s16(state + 0xc);
    uint32 value = r_u16(state + 0xc);
    if (timer > 0)
        w_u16(state + 0xc, value - 1);
}

void G3ZiplineCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000218, "G3L_40");
    uint32 frame = guest_stack - 0x28;
    uint32 player = r_u32(game + 0x50);
    uint32 flags = r_u32(player + 0x128);
    uint32 player_state = r_u32(player + 0x24);
    uint32 state = object + 0x104;
    if ((flags & 1) != 0)
        return;
    if (r_s16(state + 0xc) != 0)
        return;
    uint32 script = r_u32(state + 4);
    if (script != 0)
        G3Function80018DE0(object, script + 4, frame);
    uint32 cooldown = r_u16(state + 0xe);
    w_u16(state + 0xc, cooldown);
    w_u32(player_state + 0x124, object);
    flags = r_u32(player + 0x128);
    w_u32(player + 0x128, flags & 0xfffffffd);
    G3Function8007F1A4(player, frame);
    uint32 sound = G3Function8003F128(0x934, 0);
    w_u32(object + 0x104, sound);
}
