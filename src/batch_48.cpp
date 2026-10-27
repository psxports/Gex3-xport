#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void G3LennyBInitialize(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000003d0, "G3L_78");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    w_u32(state, 0x005f5e10);
    w_u32(state + 4, 0x00895440);
    w_u32(state + 8, 0);
    uint32 yaw = r_u16(object + 0x64);
    w_u16(state + 0x1c, 0x5b);
    w_u16(state + 0x22, 0x4b0);
    w_u16(state + 0x26, 10);
    w_u16(state + 0x28, (uint32)-7);
    w_u16(state + 0x2a, 0x280);
    w_u16(state + 0x2c, 0xa00);
    w_u16(state + 0x32, 0x50);
    w_u16(state + 0x36, 2);
    w_u16(state + 0x1a, 0);
    w_u16(state + 0x1e, 0);
    w_u16(state + 0x20, 0);
    w_u16(state + 0x24, 0);
    w_u16(state + 0x2e, 0);
    w_u16(state + 0x30, 0);
    w_u16(state + 0x34, 0);
    w_u16(state + 0x14, 3);
    w_u32(state + 0x10, 0);
    w_u16(state + 0x18, yaw);
    w_u16(state + 0x16, yaw);
    uint32 resource = r_u32(object + 0x1c);
    uint32 commands = r_u32(resource + 0x1c);
    G3LennyBConfigure(object, commands, frame);
    commands = r_u32(object + 0x28);
    if (commands != 0)
    {
        if (r_s16(commands + 2) == 0)
        {
            sint32 health = r_s16(commands);
            uint32 health_half = r_u16(commands);
            if (health != 0)
                w_u16(state + 0x14, health_half);
            else
            {
                health_half = r_u16(state + 0x14);
                w_u16(commands, health_half);
            }
            w_u32(state + 0x10, commands);
        }
        commands = r_u32(object + 0x28);
        G3LennyBConfigure(object, commands, frame);
    }
    resource = r_u32(object + 0x1c);
    w_u32(object + 0x150, 6);
    w_u16(object + 0x6e, 1);
    uint32 animations = r_u32(resource + 0x10);
    uint32 animation = r_u32(animations + 4);
    uint32 length = r_u16(animation + 2);
    w_u16(object + 0x5e, length - 1);
}

void G3LennyBPredictPoint(uint32 output, uint32 factor)
{
    FUNCTION_MARKER(0x0000052c, "G3L_78");
    uint32 player = r_u32(0x8009efc4);
    uint32 state = r_u32(player + 0x24);
    uint32 index = (uint32)r_s16(state + 0xe0);
    uint32 table = r_u32(state + 0x2c);
    uint32 sample = (uint32)r_s16(table + index * 2);
    uint32 product = sample * (uint32)(sint32)(sint16)factor;
    uint32 yaw = r_u16(player + 0x64);
    uint32 heading = (uint32)(sint32)(sint16)(yaw - 0x400);
    uint32 distance = (uint32)((sint32)product >> 8);
    uint32 cosine = G3Function8005B470(heading);
    product = distance * cosine;
    player = r_u32(0x8009efc4);
    uint32 x = r_u16(player + 0xdc);
    w_u16(output, x + (uint32)((sint32)product >> 12));
    uint32 sine = G3Function8005B590(heading);
    product = distance * sine;
    player = r_u32(0x8009efc4);
    uint32 y = r_u16(player + 0xde);
    w_u16(output + 2, y + (uint32)((sint32)product >> 12));
    uint32 z = r_u16(player + 0xe0);
    w_u16(output + 4, z);
}

uint32 G3LennyBActivate(uint32 object)
{
    FUNCTION_MARKER(0x00001118, "G3L_78");
    w_u16(object + 0x11e, 1);
    return 1;
}

void G3LennyBUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000604, "G3L_78");
    uint32 player = r_u32(0x8009efc4);
    uint32 frame = guest_stack - 0x60;
    uint32 mode = r_u32(object + 0x150);
    uint32 player_position = player + 0x58;
    uint32 state = object + 0x104;
    if (mode >= 8)
        return;
    uint32 target = r_u32(module_base + 0x1124 + mode * 4) - module_base;
    switch (target)
    {
    case 0x668:
    case 0x724:
    {
        uint32 distance = 0;
        uint32 dx = 0;
        uint32 dy = 0;
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 other = r_u16(player_position + axis * 2);
            uint32 own = r_u16(object + 0x58 + axis * 2);
            uint32 delta = other - own;
            uint32 signed_delta = (uint32)(sint32)(sint16)delta;
            distance += signed_delta * signed_delta;
            if (axis == 0)
                dx = signed_delta;
            else if (axis == 1)
                dy = signed_delta;
            w_u16(frame + 0x20 + axis * 2, delta);
        }
        if (target == 0x668)
        {
            uint32 heading = G3Function8002F240(dy, dx, frame);
            uint32 step = (uint32)r_s16(state + 0x1c);
            G3Function80038AA8(object + 0x64, (uint32)(sint32)(sint16)(heading + 0x400), step);
        }
        sint32 radius = r_s32(state + 4);
        if (radius < (sint32)distance)
        {
            w_u32(object + 0x150, 5);
            return;
        }
        if (target == 0x668)
        {
            if (r_s16(state + 0x1a) != 0)
                w_u32(object + 0x150, 1);
            return;
        }
        uint32 yaw = (uint32)r_s16(state + 0x18);
        uint32 step = (uint32)r_s16(state + 0x1c);
        if (G3Function80038AA8(object + 0x64, yaw, step) == 0)
            return;
        uint32 sequence = r_u32(state + 8);
        if (sequence != 0)
            G3Function80018DE0(object, sequence + 4, frame);
        w_u32(object + 0x150, 2);
        w_u16(object + 0x6e, 0);
        w_u16(object + 0x5e, 0);
        w_u16(state + 0x1a, 0);
        return;
    }
    case 0x7f4:
    {
        uint32 flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags & ~0x10u);
        G3Function80029F0C(object, 0xffffffff, frame);
        if ((r_u32(object + 0x14) & 0x10) != 0)
            w_u32(object + 0x150, 3);
        return;
    }
    case 0x82c:
    {
        uint32 other = r_u16(player_position);
        uint32 own = r_u16(object + 0x58);
        uint32 dx = other - own;
        w_u16(frame + 0x20, dx);
        other = r_u16(player_position + 2);
        own = r_u16(object + 0x5a);
        uint32 dy = other - own;
        w_u16(frame + 0x22, dy);
        other = r_u16(player_position + 4);
        own = r_u16(object + 0x5c);
        w_u16(frame + 0x24, other - own);
        uint32 heading = G3Function8002F240((uint32)(sint32)(sint16)dy, (uint32)(sint32)(sint16)dx, frame);
        uint32 step = (uint32)r_s16(state + 0x1c);
        uint32 aligned = G3Function80038AA8(object + 0x64, (uint32)(sint32)(sint16)(heading + 0x400), step);
        if (aligned != 0)
        {
            w_u32(object + 0x150, 4);
            w_u16(object + 0x6e, 1);
            w_u16(object + 0x5e, 0);
            w_u16(state + 0x2e, 0);
        }
        return;
    }
    case 0x8ac:
    {
        uint32 flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags & ~0x10u);
        G3Function80029F0C(object, 0xffffffff, frame);
        if (r_s16(state + 0x2e) == 0)
        {
            uint32 other = r_u16(player_position);
            uint32 own = r_u16(object + 0x58);
            uint32 dx = other - own;
            w_u16(frame + 0x20, dx);
            other = r_u16(player_position + 2);
            own = r_u16(object + 0x5a);
            uint32 dy = other - own;
            w_u16(frame + 0x22, dy);
            other = r_u16(player_position + 4);
            own = r_u16(object + 0x5c);
            w_u16(frame + 0x24, other - own);
            uint32 heading = G3Function8002F240((uint32)(sint32)(sint16)dy, (uint32)(sint32)(sint16)dx, frame);
            uint32 step = (uint32)r_s16(state + 0x1c);
            G3Function80038AA8(object + 0x64, (uint32)(sint32)(sint16)(heading + 0x400), step);
            sint32 animation_frame = r_s16(object + 0x5e);
            sint32 fire_frame = r_s16(state + 0x26);
            if (animation_frame >= fire_frame)
            {
                uint32 resource = r_u32(state + 0xc);
                w_u16(state + 0x2e, 1);
                if (resource != 0)
                {
                    uint32 projectile = G3Function8002A454(object, resource, frame);
                    if (projectile != 0)
                    {
                        uint32 bone = (uint32)r_s16(state + 0x24);
                        uint32 matrix = r_u32(object + 0x30);
                        matrix += bone << 5;
                        for (uint32 axis = 0; axis != 3; ++axis)
                        {
                            uint32 x = (uint32)r_s16(state + 0x1e);
                            uint32 a = (uint32)r_s16(matrix + axis * 6);
                            uint32 product_x = x * a;
                            uint32 y = (uint32)r_s16(state + 0x20);
                            uint32 b = (uint32)r_s16(matrix + axis * 6 + 2);
                            uint32 product_y = y * b;
                            uint32 z = (uint32)r_s16(state + 0x22);
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
                        w_u16(frame + 0x32, 0);
                        w_u16(frame + 0x30, 0);
                        value = r_u16(state + 0x28);
                        w_u16(frame + 0x34, value);
                        uint32 callback = r_u32(projectile + 0xec);
                        if (callback != 0)
                            G3DispatchStateEvent(callback, projectile, game, 0x7e0, frame + 0x30, frame);
                        uint32 factor = (uint32)r_s16(state + 0x30);
                        G3LennyBPredictPoint(frame + 0x28, factor);
                        uint32 speed = (uint32)r_s16(state + 0x32);
                        uint32 gravity = (uint32)r_s16(state + 0x28);
                        uint32 minimum = (uint32)r_s16(state + 0x2a);
                        w_u32(frame + 0x10, minimum);
                        uint32 maximum = (uint32)r_s16(state + 0x2c);
                        w_u32(frame + 0x18, frame + 0x3a);
                        w_u32(frame + 0x1c, frame + 0x38);
                        w_u32(frame + 0x14, maximum);
                        if (G3Function80036B80(projectile + 0x58, frame + 0x28, speed, gravity, minimum, maximum, frame + 0x3a, frame + 0x38, frame) == 0)
                        {
                            w_u16(frame + 0x38, 0);
                            w_u16(frame + 0x3a, 500);
                        }
                        uint32 target_y = (uint32)r_s16(frame + 0x2a);
                        uint32 projectile_y = (uint32)r_s16(projectile + 0x5a);
                        uint32 target_x = (uint32)r_s16(frame + 0x28);
                        uint32 projectile_x = (uint32)r_s16(projectile + 0x58);
                        uint32 projectile_heading = G3Function8002F240(target_y - projectile_y, target_x - projectile_x, frame);
                        callback = r_u32(projectile + 0xec);
                        w_u16(projectile + 0x64, projectile_heading + 0x400);
                        if (callback != 0)
                        {
                            G3DispatchStateEvent(callback, projectile, game, 0x7e9, frame + 0x38, frame);
                            callback = r_u32(projectile + 0xec);
                            if (callback != 0)
                            {
                                value = (uint32)r_s16(state + 0x34);
                                G3DispatchStateEvent(callback, projectile, game, 0x7d8, value, frame);
                                callback = r_u32(projectile + 0xec);
                                if (callback != 0)
                                {
                                    value = (uint32)r_s16(state + 0x36);
                                    G3DispatchStateEvent(callback, projectile, game, 0x7f0, value, frame);
                                }
                            }
                        }
                    }
                }
            }
        }
        if ((r_u32(object + 0x14) & 0x10) == 0)
            return;
        uint32 distance = 0;
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 other = r_u16(player_position + axis * 2);
            uint32 own = r_u16(object + 0x58 + axis * 2);
            uint32 delta = other - own;
            uint32 signed_delta = (uint32)(sint32)(sint16)delta;
            distance += signed_delta * signed_delta;
            w_u16(frame + 0x20 + axis * 2, delta);
        }
        sint32 radius = r_s32(state + 4);
        w_u32(object + 0x150, radius < (sint32)distance ? 5 : 0);
        return;
    }
    case 0xc64:
    {
        uint32 yaw = (uint32)r_s16(state + 0x16);
        uint32 step = (uint32)r_s16(state + 0x1c);
        if (G3Function80038AA8(object + 0x64, yaw, step) != 0)
            w_u32(object + 0x150, 6);
        return;
    }
    case 0xc84:
    {
        uint32 distance = 0;
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 other = r_u16(player_position + axis * 2);
            uint32 own = r_u16(object + 0x58 + axis * 2);
            uint32 delta = other - own;
            uint32 signed_delta = (uint32)(sint32)(sint16)delta;
            distance += signed_delta * signed_delta;
            w_u16(frame + 0x20 + axis * 2, delta);
        }
        sint32 radius = r_s32(state);
        if ((sint32)distance < radius)
            w_u32(object + 0x150, 0);
        return;
    }
    case 0xd10:
    {
        uint32 flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags & ~0x10u);
        G3Function80029F0C(object, 0xffffffff, frame);
        if ((r_u32(object + 0x14) & 0x10) != 0)
        {
            uint32 resource = r_u32(object + 0x1c);
            w_u32(object + 0x150, 0);
            w_u16(object + 0x6e, 1);
            uint32 animations = r_u32(resource + 0x10);
            uint32 animation = r_u32(animations + 4);
            uint32 length = r_u16(animation + 2);
            w_u16(object + 0x5e, length - 1);
        }
        return;
    }
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "Unresolved LennyB update target %08x\n", (unsigned int)(target + module_base));
#endif
        }
        abort();
    }
}

void G3LennyBCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000d90, "G3L_78");
    uint32 frame = guest_stack - 0x40;
    uint32 state = object + 0x104;
    uint32 contact = r_u32(object + 0xb0);
    G3Function8003C548(contact, frame + 0x20, frame + 0x21);
    uint32 other = r_u32(contact + 0x14);
    uint32 player = r_u32(0x8009efc4);
    if (other != player)
        return;
    if (r_u8(frame + 0x20) - 6u < 4)
    {
        uint32 kind = r_u8(frame + 0x21);
        if (kind == 9 || kind - 3u < 3)
        {
            G3Function80074044(object, game, frame);
            return;
        }
    }
    if (r_u8(frame + 0x21) - 6u >= 4)
        return;
    uint32 kind = r_u8(frame + 0x20);
    if (kind != 9 && kind - 3u >= 3)
        return;
    uint32 mode = r_u32(object + 0x150);
    if (mode == 7 || mode == 8)
        return;
    if (mode - 2 < 2 || (mode == 4 && r_s16(state + 0x2e) == 0))
    {
        uint32 resource = r_u32(state + 0xc);
        if (resource != 0)
        {
            uint32 projectile = G3Function8002A454(object, resource, frame);
            if (projectile != 0)
            {
                uint32 bone = (uint32)r_s16(state + 0x24);
                uint32 matrix = r_u32(object + 0x30);
                matrix += bone << 5;
                for (uint32 axis = 0; axis != 3; ++axis)
                {
                    uint32 x = (uint32)r_s16(state + 0x1e);
                    uint32 a = (uint32)r_s16(matrix + axis * 6);
                    uint32 product_x = x * a;
                    uint32 y = (uint32)r_s16(state + 0x20);
                    uint32 b = (uint32)r_s16(matrix + axis * 6 + 2);
                    uint32 product_y = y * b;
                    uint32 z = (uint32)r_s16(state + 0x22);
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
                w_u16(frame + 0x10, 0);
                value = r_u16(state + 0x28);
                w_u16(frame + 0x14, value);
                uint32 callback = r_u32(projectile + 0xec);
                if (callback != 0)
                    G3DispatchStateEvent(callback, projectile, game, 0x7e0, frame + 0x10, frame);
                w_u16(frame + 0x18, 20);
                w_u16(frame + 0x1a, 0);
                callback = r_u32(projectile + 0xec);
                if (callback != 0)
                {
                    G3DispatchStateEvent(callback, projectile, game, 0x7e9, frame + 0x18, frame);
                    callback = r_u32(projectile + 0xec);
                    if (callback != 0)
                    {
                        G3DispatchStateEvent(callback, projectile, game, 0x7d8, 25, frame);
                        callback = r_u32(projectile + 0xec);
                        if (callback != 0)
                            G3DispatchStateEvent(callback, projectile, game, 0x7f0, 2, frame);
                    }
                }
            }
        }
    }
    uint32 health = r_u16(state + 0x14);
    uint32 persisted = r_u32(state + 0x10);
    w_u16(state + 0x14, health - 1);
    if (persisted != 0)
        w_u16(persisted, health - 1);
    if (r_s16(state + 0x14) <= 0)
    {
        w_u32(object + 0x150, 8);
        G3Function80038798(object, 0, 0, frame);
    }
    else
    {
        w_u32(object + 0x150, 7);
        w_u16(object + 0x6e, 2);
        w_u16(object + 0x5e, 0);
    }
}

uint32 G3LitballEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_36");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3LitballInitialize(object, module_base, frame);
        return 1;
    case 2:
        G3LitballUpdate(object, game, frame);
        return 1;
    case 3:
        G3LitballCollide(object, game, frame);
        return 1;
    default:
        return 0;
    }
}

void G3LitballConfigure(uint32 state, uint32 commands, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000088, "G3L_36");
    uint32 frame = guest_stack - 0x20;
    uint32 value = G3Function80085AB0(commands, module_base + 0x398, frame);
    if (value != 0)
        w_u16(state, r_u16(value));
    value = G3Function80085AB0(commands, module_base + 0x3a4, frame);
    if (value != 0)
        w_u16(state + 8, r_u16(value));
}

void G3LitballInitialize(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000f8, "G3L_36");
    uint32 frame = guest_stack - 0x18;
    uint32 state = object + 0x104;
    w_u16(object + 0x104, 90);
    w_u16(state + 6, 0x40);
    w_u16(state + 4, 0xfffa);
    w_u16(state + 8, 0x20);
    w_u32(state + 0x10, 0);
    w_u32(state + 0xc, 0x20);
    uint32 commands = r_u32(object + 0x24);
    if (commands != 0)
        G3LitballConfigure(state, commands, module_base, frame);
    w_u32(object + 0x10, r_u32(object + 0x10) | 0x400);
}

void G3LitballUpdate(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000170, "G3L_36");
    uint32 frame = guest_stack - 0x40;
    uint32 lifetime = r_u16(object + 0x104) - 1;
    w_u16(object + 0x104, lifetime);
    uint32 state = object + 0x104;
    if ((sint32)(lifetime << 16) <= 0)
    {
        w_u32(object + 0x10, r_u32(object + 0x10) & 0xfffffbff);
        G3Function80038798(object, 0, 0, frame);
        return;
    }
    if (r_u32(state + 0x10) == 0)
    {
        uint32 radius = (uint32)r_s16(state + 6);
        if (G3Function80039C48(object, radius, frame + 0x18, frame) == 0)
        {
            uint32 height = r_u16(object + 0x5c);
            uint32 velocity = r_u16(state + 2);
            w_u16(object + 0x5c, height + velocity);
            velocity = r_u16(state + 2);
            uint32 gravity = r_u16(state + 4);
            w_u16(state + 2, velocity + gravity);
        }
        else
        {
            w_u32(object + 0x10, r_u32(object + 0x10) & 0xfffffbff);
            w_u32(state + 0x10, 1);
            w_u16(state + 2, 0);
        }
        return;
    }
    uint32 xy = r_u32(object + 0x58);
    uint32 z = (uint32)r_s16(object + 0x5c);
    w_u32(frame + 0x20, xy);
    w_u16(frame + 0x24, z);
    w_u16(frame + 0x28, r_u16(state + 8));
    w_u16(frame + 0x2a, r_u16(state + 8));
    w_u16(frame + 0x2c, r_u16(state + 8));
    uint32 player = r_u32(game + 0x50);
    w_u32(frame + 0x10, 0x100);
    G3Function8003639C(object, 0x80, frame + 0x28, player + 0x58, 0x100, frame);
    uint32 radius = (uint32)r_s16(state + 6);
    if (G3Function80039C48(object, radius, 0, frame) == 0)
    {
        xy = r_u32(frame + 0x20);
        z = (uint32)r_s16(frame + 0x24);
        w_u32(object + 0x58, xy);
        w_u16(object + 0x5c, z);
    }
}

void G3LitballCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000002cc, "G3L_36");
    uint32 frame = guest_stack - 0x48;
    uint32 contact = r_u32(object + 0xb0);
    uint32 other = r_u32(contact + 0x14);
    G3Function8003C548(contact, frame + 0x28, frame + 0x29);
    uint32 first_type = r_u8(frame + 0x28);
    if (first_type - 6 >= 4)
        return;
    uint32 second_type = r_u8(frame + 0x29);
    if (second_type != 9 && second_type - 3 >= 3)
        return;
    w_u32(frame + 0x10, 1);
    w_u16(frame + 0x20, 0);
    w_u16(frame + 0x22, 0);
    w_u32(frame + 0x24, contact);
    uint32 target = r_u32(other + 0xec);
    if (target != 0)
        G3DispatchStateEvent(target, other, game, 8, frame + 0x10, frame);
    G3Function80038798(object, 0, 0, frame);
}

uint32 G3LookyEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_53");
    uint32 frame = guest_stack - 0x18;
    if (message == 1)
    {
        G3LookyInitialize(object, module_base);
        return 1;
    }
    if (message == 2)
    {
        G3LookyUpdate(object, game, module_base, frame);
        return 1;
    }
    return 0;
}

void G3LookyInitialize(uint32 object, uint32 module_base)
{
    FUNCTION_MARKER(0x0000005c, "G3L_53");
    uint32 state = object + 0x128;
    uint32 data = r_u32(object + 0x28);
    w_u32(object + 0x128, 0);
    w_u16(state + 4, 0x200);
    w_u16(state + 6, 250);
    w_u32(state + 8, 7000);
    w_u16(state + 0xc, 1000);
    w_u16(state + 0xe, 0);
    w_u16(state + 0x10, 0);
    w_u16(state + 0x14, 0);
    w_u16(state + 0x16, 1);
    w_u32(state + 0x18, data != 0 ? r_u32(data) : 0xffffffff);
    uint32 flags = r_u32(object + 0x10);
    w_u16(object + 0xce, 0);
    w_u32(object + 0xf0, module_base + 0x36c);
    w_u32(object + 0x10, flags | 0xc00);
    if (r_s16(state + 0x16) == 0)
        w_u32(object + 0x18, r_u32(object + 0x18) | 0x400);
}

void G3LookyUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000f8, "G3L_53");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x128;
    G3LookyPosition(object, game, module_base, frame);
    uint32 counter = r_u32(object + 0x128);
    w_u32(object + 0x150, 0);
    w_u16(object + 0xce, 0);
    w_u32(object + 0x128, counter + 1);
    sint32 script_index = r_s32(state + 0x18);
    if (script_index > 0 && r_u32(0x800a0760 + ((uint32)script_index << 2)) != 0)
        return;
    if (r_s16(state + 0x16) == 0)
        return;
    uint32 camera = r_u32(game + 0x4c);
    if (G3Function8004DABC(camera, frame) != 6)
    {
        w_u32(object + 0x18, r_u32(object + 0x18) & 0xfffffbff);
        w_u16(state + 0x14, 0);
        return;
    }
    camera = r_u32(game + 0x4c);
    if (G3Function8004DB2C(camera, frame) != 0)
    {
        w_u32(object + 0x18, r_u32(object + 0x18) | 0x400);
        if (r_s16(state + 0x14) != 0)
        {
            uint32 pitch = r_u16(state + 0x10) + 0x60;
            w_u16(state + 0x10, pitch);
            if ((sint32)(pitch << 16) > 0)
                w_u16(state + 0x10, 0);
            uint32 height = r_u16(state + 6) - 10;
            w_u16(state + 6, height);
            if (((sint32)(height << 16) >> 16) < 250)
                w_u16(state + 6, 250);
            uint32 width = r_u16(state + 4) - 28;
            w_u16(state + 4, width);
            if (((sint32)(width << 16) >> 16) < 0x200)
                w_u16(state + 4, 0x200);
        }
        else
        {
            w_u16(state + 0x14, 1);
            w_u16(state + 0x10, 0xfc00);
            w_u16(state + 6, 340);
            w_u16(state + 4, 0x300);
        }
    }
    if (r_s16(state + 0x10) == 0 && r_u32(state + 4) == 0x00fa0200)
    {
        w_u32(object + 0x150, 1);
        w_u16(object + 0xce, 0x100);
    }
}

void G3LookyPosition(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000002a4, "G3L_53");
    uint32 frame = guest_stack - 0x58;
    uint32 camera = r_u32(game + 0x4c);
    uint32 first = r_u32(module_base + 0x59c);
    uint32 second = r_u32(module_base + 0x5a0);
    w_u32(frame + 0x10, first);
    w_u32(frame + 0x14, second);
    if (object == 0)
        return;
    uint32 matrix = r_u32(camera + 0x7c);
    G3Function800313E0(frame + 0x20, matrix, frame);
    ApplyMatrixSV((MATRIX *)psx_addr(frame + 0x20, 20), (SVECTOR *)psx_addr(frame + 0x10, 8), (SVECTOR *)psx_addr(frame + 0x18, 6));
    uint32 x = r_u16(frame + 0x18);
    uint32 cx = r_u16(camera);
    w_u16(object + 0x58, x + cx);
    uint32 y = r_u16(frame + 0x1a);
    uint32 cy = r_u16(camera + 2);
    w_u16(object + 0x5a, y + cy);
    uint32 z = r_u16(frame + 0x1c);
    uint32 cz = r_u16(camera + 4);
    w_u16(object + 0x5c, z + cz);
}

uint32 G3LookyRender(uint32 object, uint32 vertices, uint32 packets, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000036c, "G3L_53");
    uint32 frame = guest_stack - 0x88;
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 index = (uint32)r_s16(object + 0xa2);
    uint32 table = r_u32(descriptor + 0xc);
    uint32 state = object + 0x128;
    uint32 model = r_u32(table + (index << 2));
    uint32 width = r_u16(state + 4);
    uint32 height = r_u16(state + 6);
    uint32 scale = r_u32(state + 8);
    uint32 xy = r_u32(object + 0x58);
    uint32 z = (uint32)r_s16(object + 0x5c);
    w_u32(frame + 0x58, xy);
    w_u16(frame + 0x5c, z);
    uint32 angles_xy = r_u32(object + 0x60);
    uint32 angles_z = r_u32(object + 0x64);
    w_u32(frame + 0x60, angles_xy);
    w_u32(frame + 0x64, angles_z);
    w_u16(frame + 0x30, r_u16(state + 0xc));
    w_u16(frame + 0x32, r_u16(state + 0xe));
    w_u16(frame + 0x34, r_u16(state + 0x10));
    descriptor = r_u32(object + 0x1c);
    w_u16(object + 0x58, 0);
    w_u16(object + 0x5a, 0);
    w_u16(object + 0x5c, 0);
    w_u16(object + 0x60, 0);
    w_u16(object + 0x62, 0);
    w_u16(object + 0x64, 0);
    uint32 animated = r_u32(descriptor + 0x10);
    uint32 matrices = r_u32(object + 0x30);
    if (animated != 0)
        G3Function800328A0(object, descriptor, matrices, model, frame);
    else
        G3Function800315E8(object, matrices, model, frame);
    xy = r_u32(frame + 0x58);
    z = (uint32)r_s16(frame + 0x5c);
    w_u32(object + 0x58, xy);
    w_u16(object + 0x5c, z);
    angles_xy = r_u32(frame + 0x60);
    angles_z = r_u32(frame + 0x64);
    w_u32(object + 0x60, angles_xy);
    w_u32(object + 0x64, angles_z);
    descriptor = r_u32(object + 0x1c);
    index = (uint32)r_s16(object + 0xa2);
    table = r_u32(descriptor + 0xc);
    model = r_u32(table + (index << 2));
    uint32 time = (uint32)r_s16(object + 0x9e);
    uint32 sequence = r_u32(model + 0x1c);
    G3Function80034FA4(sequence, time, 0, 0);
    G3Function8002B168(object, frame);
    G3Function8005BD00(frame + 0x30, frame + 0x38);
    uint32 half_scale = (uint32)((sint32)(scale + (scale >> 31)) >> 1);
    w_u32(frame + 0x28, scale);
    w_u32(frame + 0x24, scale);
    w_u32(frame + 0x20, scale);
    w_u32(frame + 0x28, half_scale);
    G3Function8005ADF0(frame + 0x38, frame + 0x20);
    uint32 horizontal = width - 0x100;
    uint32 vertical = height - 0x78;
    horizontal = (uint32)((sint32)(horizontal << 16) >> 16);
    vertical = (uint32)((sint32)(vertical << 16) >> 16);
    w_u32(frame + 0x4c, (uint32)((sint32)(horizontal + (horizontal >> 31)) >> 1));
    w_u32(frame + 0x54, 200);
    uint32 order_table = r_u32(guest_stack + 0x10);
    w_u32(frame + 0x50, (uint32)((sint32)(vertical + (vertical >> 31)) >> 1));
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x10, order_table);
    w_u32(frame + 0x18, 1200);
    G3Function80033BA8(object, frame + 0x38, vertices, packets, order_table, 0, 1200, frame);
    return 0;
}

uint32 G3MissileEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_80");
    uint32 frame = guest_stack - 0x18;
    if (object == 0)
        return 0;
    uint32 data = r_u32(object + 0x24);
    if (data == 0 || r_u32(data + 0x1c) != 0x12b9b0a1)
    {
        uint32 text = module_base + (data == 0 ? 0x484 : 0x4b0);
        G3Function80020CBC(text, game, message, payload, frame, 0);
        return 0;
    }
    switch (message)
    {
    case 1:
        G3MissileInitialize(object);
        return 1;
    case 2:
        G3MissileUpdate(object, frame);
        return 1;
    case 3:
        G3MissileCollide(object, frame);
        return 1;
    case 4:
        return 1;
    default:
        return 0;
    }
}

void G3MissileOrientation(uint32 output, uint32 heading, uint32 pitch, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000d8, "G3L_80");
    uint32 frame = guest_stack - 0x48;
    w_u16(frame + 0x10, pitch);
    w_u16(frame + 0x12, 0);
    w_u16(frame + 0x14, heading + 0x400);
    w_u16(frame + 0x16, 0);
    G3Function8001FE14(frame + 0x10, frame + 0x18, frame);
    G3Function800202A4(frame + 0x18, frame + 0x10, 0x14, frame);
    w_u16(output, r_u16(frame + 0x14));
    w_u16(output + 2, r_u16(frame + 0x12));
    w_u16(output + 4, r_u16(frame + 0x10));
}

void G3MissileInitialize(uint32 object)
{
    FUNCTION_MARKER(0x00000158, "G3L_80");
    uint32 state = object + 0x104;
    w_u32(state + 0x10, 0);
    w_u32(state + 0x14, 0);
}

void G3MissileTrajectory(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000168, "G3L_80");
    uint32 frame = guest_stack - 0x38;
    uint32 player = r_u32(0x8009efc4);
    uint32 x = (uint32)r_s16(object + 0x58);
    uint32 px = (uint32)r_s16(player + 0x58);
    uint32 data = r_u32(object + 0x24);
    uint32 dx = px - x;
    w_u32(frame + 0x10, dx);
    uint32 py = (uint32)r_s16(player + 0x5a);
    uint32 y = (uint32)r_s16(object + 0x5a);
    uint32 dy = py - y;
    w_u32(frame + 0x14, dy);
    uint32 pz = (uint32)r_s16(player + 0x5c);
    uint32 z = (uint32)r_s16(object + 0x5c);
    uint32 state = object + 0x104;
    w_u32(frame + 0x18, pz - z);
    uint32 heading = G3Function8002F240(dy, dx, frame);
    w_u16(object + 0x104, heading);
    w_u16(object + 0x64, heading + 0x400);
    uint32 cosine = G3Function8005B590(r_u32(data + 4));
    uint32 sine = G3Function8005B470(r_u32(data + 4));
    uint32 speed = r_u32(data);
    uint32 numerator = speed * cosine;
    uint32 vertical = sine == 0 ? ((sint32)numerator < 0 ? 1u : 0xffffffffu) : (uint32)((sint64)(sint32)numerator / (sint32)sine);
    w_u32(state + 4, vertical << 16);
    uint32 length = G3Function8002FDE4(frame + 0x10, frame);
    speed = r_u32(data);
    uint32 duration = speed == 0 ? ((sint32)length < 0 ? 1u : 0xffffffffu) : (uint32)((sint64)(sint32)length / (sint32)speed);
    player = r_u32(0x8009efc4);
    w_u32(state + 8, duration);
    pz = (uint32)r_s16(player + 0x5c);
    z = (uint32)r_s16(object + 0x5c);
    vertical = r_u32(state + 4);
    uint32 height = (pz - z) << 8;
    if ((sint32)vertical < 0)
        vertical += 0xff;
    uint32 displacement = (uint32)((sint32)vertical >> 8) * duration;
    numerator = (height - displacement) << 9;
    uint32 acceleration = duration == 0 ? ((sint32)numerator < 0 ? 1u : 0xffffffffu) : (uint32)((sint64)(sint32)numerator / (sint32)duration);
    acceleration = duration == 0 ? ((sint32)acceleration < 0 ? 1u : 0xffffffffu) : (uint32)((sint64)(sint32)acceleration / (sint32)duration);
    w_u32(state + 0xc, acceleration);
}

void G3MissileExplode(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000002ac, "G3L_80");
    uint32 frame = guest_stack - 0x18;
    uint32 data = r_u32(object + 0x24);
    uint32 resource = G3Function8003AA48(data + 0x14);
    if (resource != 0)
        G3Function8002A454(object, resource, frame);
    G3Function80038798(object, 0, 0, frame);
}

void G3MissileUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000002f8, "G3L_80");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    uint32 initialized = r_u32(state + 0x10);
    uint32 data = r_u32(object + 0x24);
    if (initialized == 0)
    {
        G3MissileTrajectory(object, frame);
        w_u32(state + 0x10, 1);
    }
    w_u16(frame + 0x10, 0);
    w_u16(frame + 0x12, 0);
    w_u16(frame + 0x14, 0);
    w_u16(frame + 0x10, r_u16(data));
    uint32 heading = (uint32)r_s16(object + 0x104);
    G3Function8002F8E4(frame + 0x10, heading);
    uint32 x = r_u16(object + 0x58);
    uint32 dx = r_u16(frame + 0x10);
    w_u16(object + 0x58, x + dx);
    uint32 y = r_u16(object + 0x5a);
    uint32 dy = r_u16(frame + 0x12);
    w_u16(object + 0x5a, y + dy);
    uint32 z = r_u16(object + 0x5c);
    uint32 dz = r_u16(frame + 0x14);
    w_u16(object + 0x5c, z + dz);
    uint32 velocity = r_u32(state + 4);
    uint32 acceleration = r_u32(state + 0xc);
    velocity += acceleration;
    w_u32(state + 4, velocity);
    if ((sint32)velocity < 0)
        velocity += 0xffff;
    z = r_u16(object + 0x5c);
    uint32 vertical_step = (uint32)((sint32)velocity >> 16);
    w_u16(object + 0x5c, z + vertical_step);
    uint32 speed = r_u32(data);
    uint32 angle = G3Function8002F240(vertical_step, speed, frame);
    uint32 pitch = (uint32)((sint32)(0 - (angle << 16)) >> 16);
    heading = (uint32)r_s16(object + 0x104);
    G3MissileOrientation(object + 0x60, heading, pitch, frame);
    uint32 trail = r_u32(state + 0x14) + 1;
    w_u32(state + 0x14, trail);
    sint32 interval = r_s32(data + 8);
    if ((sint32)trail >= interval)
    {
        w_u32(state + 0x14, 0);
        uint32 resource = G3Function8003AA48(data + 0xc);
        if (resource != 0)
            G3Function8002A454(object, resource, frame);
    }
    uint32 elapsed = r_u32(state + 0x18);
    sint32 duration = r_s32(state + 8);
    ++elapsed;
    w_u32(state + 0x18, elapsed);
    if (duration < (sint32)elapsed)
        G3MissileExplode(object, frame);
}

void G3MissileCollide(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000464, "G3L_80");
    uint32 frame = guest_stack - 0x18;
    G3MissileExplode(object, frame);
}

uint32 G3MonsterEntry0000(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_65");
    uint32 frame = guest_stack - 0x18;
    return G3MonsterEvent0020(object, game, message, payload, module_base, frame);
}

uint32 G3MonsterEvent0020(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000020, "G3L_65");
    uint32 frame = guest_stack - 0x48;
    if (object == 0) return 0;
    uint32 model = r_u32(object + 0x24);
    uint32 state = object + 0x104;
    if (message != 1 && G3MonsterValidate0938(object) == 0) return 0;
    if (message != 4 && (r_u32(state + 0x18) & 0x8000000) != 0) return 0;
    if (message == 1)
    {
        G3MonsterInitialize024C(object, module_base, frame);
        return 1;
    }
    if (message == 4)
    {
        G3MonsterNoop0604();
        return 1;
    }
    r_u32(game + 0x4c);
    if (G3Function8004DACC() != 0)
    {
        uint32 resource = r_u32(model);
        if ((r_u32(resource + 0x18) & 0x80) == 0) return 0;
    }
    if ((r_u32(state + 0x18) & 0x80) != 0)
    {
        if (message != 2) return 0;
        G3MonsterUpdateRange1408(object, frame);
        return 1;
    }
    switch (message)
    {
    case 2:
        G3MonsterUpdateRange1408(object, frame);
        G3MonsterUpdate060C(object, module_base, frame);
        return 1;
    case 3:
        G3MonsterCollide313C(object, frame);
        return 1;
    case 8: return G3MonsterDamage3284(object, payload, frame);
    case 0x7e4:
        G3MonsterInitializePacket31F8(frame + 0x10);
        return G3MonsterDamage3284(object, frame + 0x10, frame);
    case 0x7dd: return G3MonsterRequestRemoval35FC(object, frame);
    case 0x13:
        G3MonsterRunProperty0EB4(object, module_base + 0x7bd0, module_base, frame);
        return 0;
    default: return 0;
    }
}

void G3MonsterInitialize024C(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000024c, "G3L_65");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    uint32 flags = r_u32(state + 0x18);
    uint32 model = r_u32(object + 0x24);
    w_u32(state + 0x18, flags & 0xfffffffe);
    if (G3MonsterValidate0938(object) == 0) return;
    G3FillMemory80086094(state, 0, 0x54);
    if (r_u32(model) == 0)
    {
        w_u32(model + 0x268, 100);
        w_u32(model + 0x264, 1);
        w_u32(model + 0x26c, 1);
        uint32 first = r_u32(module_base + 0x7bdc);
        uint32 second = r_u32(module_base + 0x7be0);
        w_u32(model + 0x270, first);
        w_u32(model + 0x274, second);
        w_u32(model + 0x290, 0x12b9b0a1);
        w_u32(model, model + 0x264);
    }
    w_u32(model + 0x254, 2500);
    w_u32(model + 0x258, 1000);
    w_u32(model + 0x25c, 2000);
    w_u32(model + 0x260, 4000);
    if (r_u32(object + 0x28) == 0)
    {
        uint32 configuration = model + 0x254;
        if (model != 0)
        {
            uint32 resource = r_u32(model);
            if (resource != 0)
            {
                uint32 configured = r_u32(resource + 0x1c);
                if (configured != 0) configuration = configured;
            }
        }
        w_u32(object + 0x28, configuration);
    }
    uint32 initialized = r_u32(model + 0x28);
    uint32 configuration = r_u32(object + 0x28);
    if (initialized == 0) G3MonsterInitializeChain191C(r_u32(object + 0x1c), model, module_base);
    uint32 minimum = G3MonsterMinimum5E34(configuration);
    w_u8(state + 0x3f, minimum != 0 ? 1 : 0);
    w_u8(state + 1, minimum != 0 ? 1 : 0);
    w_u8(state + 4, 0x12);
    w_u8(state + 2, 0x1c);
    w_u16(state + 0x44, 100);
    w_u16(state + 0x46, 100);
    flags = r_u32(state + 0x18);
    w_u8(state, 0);
    w_u8(state + 0xd, 0);
    w_u8(state + 0x3e, 0xff);
    w_u32(state + 0x18, flags | 0x100);
    uint32 resource = r_u32(model);
    w_u16(state + 0x42, r_u16(resource + 8));
    uint32 render_flags = r_u32(object + 0x14);
    flags = r_u32(object + 0x10);
    w_u32(object + 0x14, render_flags & 0xf7ffffff);
    w_u32(object + 0x10, flags | 0x400);
    {
        uint32 source = object + 0x58;
        uint32 destination = state + 0x2a;
        uint32 first_address = source;
        uint32 first_high_address = first_address + 3;
        uint32 first_high_lane = first_high_address & 3;
        uint32 first_lane = first_address & 3;
        uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
        uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
        uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
        uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
        uint32 z = (uint32)r_s16(source + 4);
        first_address = destination;
        first_high_address = first_address + 3;
        first_high_lane = first_high_address & 3;
        first_lane = first_address & 3;
        w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
        w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
        w_u16(destination + 4, z);
    }
    {
        uint32 source = object + 0x58;
        uint32 destination = state + 0x30;
        uint32 first_address = source;
        uint32 first_high_address = first_address + 3;
        uint32 first_high_lane = first_high_address & 3;
        uint32 first_lane = first_address & 3;
        uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
        uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
        uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
        uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
        uint32 z = (uint32)r_s16(source + 4);
        first_address = destination;
        first_high_address = first_address + 3;
        first_high_lane = first_high_address & 3;
        first_lane = first_address & 3;
        w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
        w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
        w_u16(destination + 4, z);
    }
    G3MonsterPackCoordinates149C(object + 0x58, state + 0x12);
    uint32 player = r_u32(0x8009efc4);
    w_u32(state + 0x50, r_u32(player + 0x138) - 1);
    if ((r_u32(object + 0x10) & 2) != 0)
    {
        w_u8(state + 1, 15);
        w_u8(state + 2, 0x1b);
    }
    uint32 property = G3MonsterFindProperty0C60(configuration, module_base + 0x7be8, module_base);
    flags = r_u32(state + 0x18);
    w_u32(state + 0x18, property != 0 ? flags | 0x1000 : flags & 0xffffefff);
    property = G3MonsterFindProperty0C60(configuration, module_base + 0x7bf4, module_base);
    flags = r_u32(state + 0x18);
    w_u32(state + 0x18, property != 0 ? flags | 0x20 : flags & 0xffffffdf);
    property = G3MonsterFindProperty0C60(configuration, module_base + 0x7c00, module_base);
    flags = r_u32(state + 0x18);
    w_u32(state + 0x18, property != 0 ? flags & 0xffffdfdf : flags | 0x2000);
    property = G3MonsterFindProperty0C60(configuration, module_base + 0x7c0c, module_base);
    render_flags = r_u32(object + 0x14);
    w_u32(object + 0x14, property != 0 ? render_flags | 0x20 : render_flags & 0xffffffdf);
    flags = r_u32(state + 0x18);
    w_u32(state + 0x18, property != 0 ? flags | 0x4000 : flags & 0xffffbfff);
    player = r_u32(0x8009efc4);
    G3MonsterUpdateLineFlag1874(object + 0x58, player + 0x58, state, frame);
    resource = r_u32(model);
    if ((r_u32(resource + 0x18) & 2) != 0 && r_s8(state + 1) == 0) G3MonsterMoveReduced1780(object, frame);
    else G3MonsterMoveDefault1730(object, frame);
    resource = r_u32(model);
    if ((r_u32(resource + 0x18) & 0x40) != 0) G3Function80047EE4(10, (uint32)r_s16(state + 0x42));
}

void G3MonsterNoop0604()
{
    FUNCTION_MARKER(0x00000604, "G3L_65");
}

void G3MonsterUpdate060C(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000060c, "G3L_65");
    uint32 frame = guest_stack - 0x78;
    uint32 state = object + 0x104;
    uint32 model = r_u32(object + 0x24);
    uint32 configuration = r_u32(object + 0x28);
    G3MonsterPrePhysics3F9C(object, model, module_base, frame);
    uint32 player = r_u32(0x8009efc4);
    {
        uint32 source = object + 0x58;
        uint32 destination = state + 0x2a;
        uint32 first_address = source;
        uint32 first_high_address = first_address + 3;
        uint32 first_high_lane = first_high_address & 3;
        uint32 first_lane = first_address & 3;
        uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
        uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
        uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
        uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
        uint32 z = (uint32)r_s16(source + 4);
        first_address = destination;
        first_high_address = first_address + 3;
        first_high_lane = first_high_address & 3;
        first_lane = first_address & 3;
        w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
        w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
        w_u16(destination + 4, z);
    }
    G3MonsterCaptureState0EF8(frame + 0x10, object, player + 0x58);
    G3MonsterCopyStateVector117C(state, frame + 0x40, frame);
    G3MonsterUpdateTarget6BD8(object, state, r_u32(state + 0x1c), frame + 0x40, frame);
    G3MonsterUpdateStall1550(state, object + 0x58, frame);
    w_u16(frame + 0x48, 40);
    uint32 iteration = 0;
    while ((sint16)iteration < 15 && (r_u32(state + 0x18) & 0x8000000) == 0)
    {
        uint32 resource = r_u32(model);
        uint32 mode = r_u32(resource);
        uint32 previous = r_u16(state + 0x20);
        uint32 result = G3MonsterStep7178(frame + 0x48, object, state, mode, frame);
        uint32 current = (uint32)r_s16(state + 0x20);
        uint32 animation = r_u32(state + 0x14);
        G3MonsterTriggerFrames7B2C((uint32)(sint16)previous, current, animation, object, module_base, frame);
        if (result != 0 || G3MonsterShouldTransition0F7C(state, model, configuration, module_base, frame) != 0)
        {
            model = G3MonsterTransition414C(object, configuration, model, module_base, frame);
            animation = r_u32(state + 0x14);
            w_u16(object + 0x6e, r_u8(animation));
        }
        ++iteration;
        if (result == 0 && r_s16(frame + 0x48) <= 0) break;
    }
    uint32 phase = r_u16(state + 0x20);
    sint32 animation_frame = (sint16)phase / 40;
    uint32 descriptor = r_u32(object + 0x1c);
    w_u16(object + 0x5e, (uint32)animation_frame & 0xff);
    if ((r_u32(descriptor + 0x30) & 0x2000) != 0) G3Function8003E314(object, frame);
    uint32 game = r_u32(0x80097048);
    uint32 lists = r_u32(game + 0x5c);
    uint32 head = r_u32(lists + 8);
    uint32 snapshot = r_u32(state + 0x1c);
    uint32 slot = object - 4 - head;
    uint32 stagger = (uint32)((sint32)(slot * 0x2fa0be83) >> 3);
    G3MonsterStaggerLine18C0(object + 0x58, snapshot, state, stagger, frame);
    G3MonsterUpdateCounters0A38(state, state, object, frame);
    G3MonsterPostPhysics4094(object, model, module_base, frame);
    if (r_s16(state + 0x34) < -30000 && r_s16(object + 0x5c) > 32000)
    {
        r_s8(state + 0x3e);
        G3MonsterNoop72D0();
        w_u8(state + 0x3e, 0xff);
        w_u32(object + 0x10, r_u32(object + 0x10) & 0xfffffbff);
        w_u32(state + 0x18, r_u32(state + 0x18) | 0x8000000);
    }
    game = r_u32(0x80097048);
    if ((r_u32(game + 0x69b8) & 1) != 0)
    {
        r_u32(object + 0x1c);
        G3MonsterNoop7BC8();
    }
    uint32 flags = r_u32(state + 0x18);
    w_u32(state + 0x1c, 0);
    w_u32(state + 0x18, flags & 0xfffffeff);
    {
        uint32 source = object + 0x58;
        uint32 destination = state + 0x30;
        uint32 first_address = source;
        uint32 first_high_address = first_address + 3;
        uint32 first_high_lane = first_high_address & 3;
        uint32 first_lane = first_address & 3;
        uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
        uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
        uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
        uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
        uint32 z = (uint32)r_s16(source + 4);
        first_address = destination;
        first_high_address = first_address + 3;
        first_high_lane = first_high_address & 3;
        first_lane = first_address & 3;
        w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
        w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
        w_u16(destination + 4, z);
    }
    if ((r_u32(state + 0x18) & 0x8000000) != 0) G3Function80038798(object, 0, 0, frame);
}

uint32 G3MonsterValidate0938(uint32 object)
{
    FUNCTION_MARKER(0x00000938, "G3L_65");
    uint32 flags = r_u32(object + 0x11c);
    uint32 model = r_u32(object + 0x24);
    if ((flags & 1) != 0) return 1;
    if (model == 0) return 0;
    uint32 identity = r_u32(model + 0x294);
    if (identity != 0x12b9b0a1) return 0;
    uint32 resource = r_u32(model);
    if (resource != 0 && r_u32(resource + 0x2c) != identity) return 0;
    resource = r_u32(model + 0xc);
    if (resource != 0 && r_u32(resource + 0x40) != 0x12b9b0a1) return 0;
    resource = r_u32(model + 0x10);
    if (resource != 0 && r_u32(resource + 0x28) != 0x12b9b0a1) return 0;
    resource = r_u32(model + 0x18);
    if (resource != 0 && r_u32(resource + 0x10) != 0x12b9b0a1) return 0;
    resource = r_u32(model);
    if (resource != 0 && r_s32(resource) >= 2) return 0;
    w_u32(object + 0x11c, r_u32(object + 0x11c) | 1);
    return 1;
}

void G3MonsterNoop0A30()
{
    FUNCTION_MARKER(0x00000a30, "G3L_65");
}

void G3MonsterUpdateCounters0A38(uint32 counters, uint32 state, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000a38, "G3L_65");
    uint32 frame = guest_stack - 0x20;
    uint32 timer = r_u8(state + 0x4e);
    w_u8(state + 0x4e, timer != 0 ? timer - 1 : 0);
    if (r_u8(state + 0x36) == 1) G3MonsterMoveDefault1730(object, frame);
    timer = r_u8(state + 0x36);
    w_u8(state + 0x36, timer != 0 ? timer - 1 : 0);
    if ((r_u32(state + 0x18) & 0x2000000) != 0)
    {
        uint32 count = r_u8(counters + 0xc);
        if (count < 0xff) w_u8(counters + 0xc, count + 1);
    }
    else w_u8(counters + 0xc, 0);
    if (r_u8(counters + 0xc) == 1)
    {
        uint32 snapshot = r_u32(state + 0x1c);
        uint32 distance = r_u16(snapshot + 0x1c) >> 6;
        if (distance >= 0x100) distance = 0xff;
        snapshot = r_u32(state + 0x1c);
        w_u8(state + 0x4c, distance);
        w_u8(state + 0x4d, (r_u16(snapshot + 0x2c) & 0xfff) >> 4);
    }
    if (r_u8(counters + 0xc) >= 2)
    {
        sint32 previous = (sint32)r_u8(state + 0x4c) - 2;
        if (previous < 0) previous = 0;
        uint32 snapshot = r_u32(state + 0x1c);
        uint32 distance = r_u16(snapshot + 0x1c) >> 6;
        if ((sint32)distance < (sint16)previous)
        {
            w_u32(state + 0x18, r_u32(state + 0x18) & 0xfdffffff);
            w_u8(counters + 0xc, 0);
        }
        snapshot = r_u32(state + 0x1c);
        uint32 angle = r_u8(state + 0x4d);
        uint32 difference = (r_u16(snapshot + 0x2c) - (angle << 4)) & 0xfff;
        if (difference >= 0x800) difference |= 0xf000;
        sint32 magnitude = (sint16)difference;
        if (magnitude < 0) magnitude = -magnitude;
        if (magnitude > 0x200)
        {
            w_u32(state + 0x18, r_u32(state + 0x18) & 0xfdffffff);
            w_u8(counters + 0xc, 0);
        }
    }
    uint32 count = r_u8(counters + 0xa) + 1;
    w_u8(counters + 0xa, count);
    if ((count & 0xff) >= 30)
    {
        count = r_u8(counters + 0xb);
        if (count != 0xff) w_u8(counters + 0xb, count + 1);
        count = r_u8(counters + 0x10);
        if (count != 0xff) w_u8(counters + 0x10, count + 1);
        w_u8(counters + 0xa, 0);
    }
    timer = r_u8(state + 0x37);
    if (timer != 0) w_u8(state + 0x37, timer - 1);
}

uint32 G3MonsterFindProperty0C60(uint32 resource, uint32 name, uint32 module_base)
{
    FUNCTION_MARKER(0x00000c60, "G3L_65");
    if (resource == 0) return 0;
    uint32 first = r_u32(resource + 0x10);
    uint32 cursor = resource + 0x10;
    if (first == 0x12b9b0a1)
    {
        cursor = resource + 0x14;
        first = r_u32(cursor);
    }
    uint32 expected = r_u32(module_base + 0x7c18);
    if (first != expected) return 0;
    uint32 second = r_u32(cursor + 4);
    expected = r_u32(module_base + 0x7c1c);
    cursor += 8;
    if (second != expected) return 0;
    for (;;)
    {
        first = r_u32(cursor);
        expected = r_u32(module_base + 0x7c24);
        if (first == expected)
        {
            second = r_u32(cursor + 4);
            expected = r_u32(module_base + 0x7c28);
            if (second == expected) return 0;
        }
        expected = r_u32(name);
        if (first == expected)
        {
            second = r_u32(cursor + 4);
            expected = r_u32(name + 4);
            if (second == expected) return cursor + 8;
        }
        expected = r_u32(module_base + 0x7bd0);
        if (first == expected)
        {
            second = r_u32(cursor + 4);
            expected = r_u32(module_base + 0x7bd4);
            if (second == expected)
            {
                cursor += 12;
                continue;
            }
        }
        const uint32 tag_offsets[5] = { 0x7c30, 0x7bf4, 0x7c0c, 0x7c00, 0x7be8 };
        const uint32 advances[5] = { 12, 8, 8, 8, 20 };
        uint32 advance = 0;
        for (uint32 index = 0; index < 5; ++index)
        {
            first = r_u32(cursor);
            expected = r_u32(module_base + tag_offsets[index]);
            if (first == expected)
            {
                second = r_u32(cursor + 4);
                expected = r_u32(module_base + tag_offsets[index] + 4);
                if (second == expected)
                {
                    advance = advances[index];
                    break;
                }
            }
        }
        if (advance != 0)
        {
            cursor += advance;
            continue;
        }
        first = r_u32(cursor);
        expected = r_u32(module_base + 0x7c3c);
        if (first != expected) return 0;
        cursor += 12;
    }
}

void G3MonsterRunSequence0E7C(uint32 object, uint32 model, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000e7c, "G3L_65");
    uint32 frame = guest_stack - 0x18;
    uint32 resource = r_u32(model + 0x18);
    uint32 sequence = r_u32(resource + 8);
    if (sequence != 0) G3Function80018DE0(object, sequence + 4, frame);
}

void G3MonsterRunProperty0EB4(uint32 object, uint32 name, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000eb4, "G3L_65");
    uint32 frame = guest_stack - 0x18;
    uint32 resource = r_u32(object + 0x28);
    uint32 property = G3MonsterFindProperty0C60(resource, name, module_base);
    if (property != 0) G3Function80018DE0(object, r_u32(property) + 4, frame);
}

void G3MonsterCaptureState0EF8(uint32 snapshot, uint32 object, uint32 position)
{
    FUNCTION_MARKER(0x00000ef8, "G3L_65");
    uint32 player = r_u32(0x8009efc4);
    w_u32(object + 0x120, snapshot);
    {
        uint32 source = position;
        uint32 copy_destination = snapshot;
        uint32 first_address = source;
        uint32 first_high_address = first_address + 3;
        uint32 first_high_lane = first_high_address & 3;
        uint32 first_lane = first_address & 3;
        uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
        uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
        uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
        uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
        uint32 z = (uint32)r_s16(source + 4);
        first_address = copy_destination;
        first_high_address = first_address + 3;
        first_high_lane = first_high_address & 3;
        first_lane = first_address & 3;
        w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
        w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
        w_u16(copy_destination + 4, z);
    }
    uint32 angle = r_u16(player + 0x64);
    uint32 snapshot_destination = r_u32(object + 0x120);
    w_u16(snapshot_destination + 6, angle - 0x400);
    snapshot_destination = r_u32(object + 0x120);
    {
        uint32 source = object + 0x58;
        uint32 copy_destination = snapshot_destination + 8;
        uint32 first_address = source;
        uint32 first_high_address = first_address + 3;
        uint32 first_high_lane = first_high_address & 3;
        uint32 first_lane = first_address & 3;
        uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
        uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
        uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
        uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
        uint32 z = (uint32)r_s16(source + 4);
        first_address = copy_destination;
        first_high_address = first_address + 3;
        first_high_lane = first_high_address & 3;
        first_lane = first_address & 3;
        w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
        w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
        w_u16(copy_destination + 4, z);
    }
    snapshot_destination = r_u32(object + 0x120);
    {
        uint32 source = object + 0x88;
        uint32 copy_destination = snapshot_destination + 0xe;
        uint32 first_address = source;
        uint32 first_high_address = first_address + 3;
        uint32 first_high_lane = first_high_address & 3;
        uint32 first_lane = first_address & 3;
        uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
        uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
        uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
        uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
        uint32 z = (uint32)r_s16(source + 4);
        first_address = copy_destination;
        first_high_address = first_address + 3;
        first_high_lane = first_high_address & 3;
        first_lane = first_address & 3;
        w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
        w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
        w_u16(copy_destination + 4, z);
    }
    angle = r_u16(object + 0x64);
    snapshot_destination = r_u32(object + 0x120);
    w_u16(snapshot_destination + 0x2e, angle - 0x400);
}

uint32 G3MonsterShouldTransition0F7C(uint32 state, uint32 model, uint32 configuration, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000f7c, "G3L_65");
    uint32 frame = guest_stack - 0x38;
    uint32 snapshot = r_u32(state + 0x1c);
    uint32 settings = snapshot + 0x14;
    if ((r_u32(state + 0x18) & 6) != 0)
    {
        sint32 mode = r_s8(state + 2);
        if (mode != 0xc && mode != 0x14) return 1;
    }
    if (model != 0)
    {
        uint32 resource = r_u32(model + 0x18);
        if (resource != 0 && (r_u32(resource + 0xc) & 1) != 0 && G3MonsterCheckAlternate5B0C(state, model, frame) != 0) return 1;
    }
    if ((r_u32(state + 0x18) & 0x200000) != 0) return 0;
    if (r_s8(state) == 0)
    {
        w_u32(frame + 0x10, 70);
        if (G3MonsterShouldWake5BD0(settings, configuration, state, model, module_base, frame) != 0) return 1;
    }
    snapshot = r_u32(state + 0x1c);
    w_u16(frame + 0x18, r_u16(snapshot + 0x16));
    snapshot = r_u32(state + 0x1c);
    w_u16(frame + 0x1a, r_u16(snapshot + 0x1e));
    snapshot = r_u32(state + 0x1c);
    w_u16(frame + 0x1c, r_u16(snapshot + 0x20));
    if (r_s8(state) != 1) return 0;
    uint32 kind = r_u8(state + 1);
    if (kind - 6 < 2)
    {
        w_u32(frame + 0x10, 1);
        w_u32(frame + 0x14, (uint32)r_s8(state + 1));
        return G3MonsterFindGroup751C(0, frame + 0x18, model, state, frame) != 0 ? 1 : 0;
    }
    if ((sint8)kind != 8) return 0;
    uint32 index = (uint32)r_s8(state + 5);
    uint32 table = r_u32(model + 8);
    uint32 group = r_u32(table + index * 4);
    uint32 resource = r_u32(group);
    if ((r_u16(resource + 2) & 8) != 0) return 0;
    w_u32(frame + 0x10, 8);
    w_u32(frame + 0x14, (uint32)r_s8(state + 1));
    return G3MonsterFindGroup751C(0, frame + 0x18, model, state, frame) != 0 ? 1 : 0;
}

void G3MonsterCopyStateVector117C(uint32 state, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000117c, "G3L_65");
    uint32 frame = guest_stack - 0x30;
    uint32 snapshot = r_u32(state + 0x1c);
    sint32 mode = r_s8(state + 1);
    sint32 previous = r_s8(state + 0x48);
    if (mode == 5)
    {
        {
            uint32 source = snapshot;
            uint32 destination = frame + 0x18;
            uint32 first_address = source;
            uint32 first_high_address = first_address + 3;
            uint32 first_high_lane = first_high_address & 3;
            uint32 first_lane = first_address & 3;
            uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
            uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
            uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
            uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
            uint32 z = (uint32)r_s16(source + 4);
            first_address = destination;
            first_high_address = first_address + 3;
            first_high_lane = first_high_address & 3;
            first_lane = first_address & 3;
            w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
            w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
            w_u16(destination + 4, z);
        }
    }
    if (r_s8(state + 1) == 4)
    {
        snapshot = r_u32(state + 0x1c);
        {
            uint32 source = snapshot + 0xe;
            uint32 destination = frame + 0x18;
            uint32 first_address = source;
            uint32 first_high_address = first_address + 3;
            uint32 first_high_lane = first_high_address & 3;
            uint32 first_lane = first_address & 3;
            uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
            uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
            uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
            uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
            uint32 z = (uint32)r_s16(source + 4);
            first_address = destination;
            first_high_address = first_address + 3;
            first_high_lane = first_high_address & 3;
            first_lane = first_address & 3;
            w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
            w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
            w_u16(destination + 4, z);
        }
    }
    uint32 mode_bits = r_u8(state + 1);
    if (mode_bits - 4 < 2)
    {
        r_s8(state + 0x3e);
        w_u32(frame + 0x10, (sint8)mode_bits == 5 ? 0 : 1);
        uint32 result = G3MonsterCopyVector72D8(output, frame + 0x18);
        w_u8(state + 0x48, result);
        if (r_s8(state + 0x48) != 0 && previous == 0) w_u32(state + 0x18, r_u32(state + 0x18) | 0x200);
    }
}

uint32 G3MonsterCheckRange1278(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001278, "G3L_65");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x30;
    uint32 level = r_u32(game + 0x48);
    uint32 limit = r_u16(level + 0x4c);
    uint32 player = r_u32(0x8009efc4);
    uint32 camera = r_u32(game + 0x4c);
    uint32 camera_limit = r_u32(camera + 0x88);
    if ((sint32)camera_limit < (sint32)limit) limit = camera_limit;
    uint32 coordinate = r_u16(player + 0x58);
    uint32 origin = r_u16(object + 0x58);
    w_u16(frame + 0x10, coordinate - origin);
    coordinate = r_u16(player + 0x5a);
    origin = r_u16(object + 0x5a);
    uint32 threshold = limit + 2000;
    w_u16(frame + 0x12, coordinate - origin);
    coordinate = r_u16(player + 0x5c);
    origin = r_u16(object + 0x5c);
    w_u16(frame + 0x14, coordinate - origin);
    sint32 distance = (sint32)G3Function8002FB10(frame + 0x10);
    sint32 player_z = r_s16(player + 0x5c);
    sint32 object_z = r_s16(object + 0x5c);
    sint32 forced = r_s16(object + 0x144);
    sint32 height = player_z - object_z;
    if (height < 0) height = -height;
    uint32 result = 0;
    bool clear_flag = false;
    if (forced == 0 && r_s8(object + 0x106) != 0xc)
    {
        if (distance >= 4000 || height >= 2000)
        {
            uint32 flags = r_u32(object + 0x10);
            if ((flags & 0x200) == 0)
            {
                result = 1;
                if (distance >= (sint32)threshold && (distance >= 15000 || height >= 8000 || (flags & 2) == 0)) clear_flag = true;
            }
        }
    }
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x10, clear_flag ? flags & 0xfffffbff : flags | 0x400);
    return result;
}

uint32 G3MonsterUpdateRange1408(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001408, "G3L_65");
    uint32 frame = guest_stack - 0x20;
    uint32 result = G3MonsterCheckRange1278(object, frame);
    uint32 flags = r_u32(object + 0x11c);
    if ((flags & 0x80) != 0)
    {
        if (result == 0)
        {
            G3MonsterResume16CC(object, frame);
            w_u32(object + 0x11c, r_u32(object + 0x11c) & 0xffffff7f);
        }
    }
    else if (result == 1)
    {
        G3MonsterSuspend1710(object, frame);
        w_u32(object + 0x11c, r_u32(object + 0x11c) | 0x80);
    }
    return result;
}

void G3MonsterPackCoordinates149C(uint32 source, uint32 destination)
{
    FUNCTION_MARKER(0x0000149c, "G3L_65");
    w_u8(destination, r_u16(source) >> 4);
    w_u8(destination + 1, r_u16(source + 2) >> 4);
}

uint32 G3MonsterPackedDistance14C0(uint32 position, uint32 packed, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000014c0, "G3L_65");
    uint32 frame = guest_stack - 0x20;
    G3MonsterPackCoordinates149C(position, frame + 0x10);
    sint32 first = (sint32)r_u8(packed);
    sint32 current_x = (sint32)r_u8(frame + 0x10);
    sint32 current_y = (sint32)r_u8(frame + 0x11);
    first -= current_x;
    sint32 second = (sint32)r_u8(packed + 1) - current_y;
    if (first < 0) first = -first;
    if (second < 0) second = -second;
    if (second < first)
    {
        sint32 swap = first;
        first = second;
        second = swap;
    }
    return (uint32)((second * 30 + first * 12) >> 1);
}

void G3MonsterUpdateStall1550(uint32 state, uint32 position, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001550, "G3L_65");
    uint32 frame = guest_stack - 0x20;
    uint32 timer = r_u8(state + 0x49);
    if (timer != 0) w_u8(state + 0x49, timer - 1);
    if ((r_u32(state + 0x18) & 0x10) != 0 && r_u8(state + 0x49) == 0)
    {
        uint32 count = r_u8(state + 0x4a);
        if (count < 0xff) w_u8(state + 0x4a, count + 1);
    }
    if ((sint32)G3MonsterPackedDistance14C0(position, state + 0x12, frame) > 0xa0)
    {
        G3MonsterPackCoordinates149C(position, state + 0x12);
        w_u8(state + 0x4a, 0);
    }
    if (r_u8(state + 0x4a) >= 0x10)
    {
        uint32 flags = r_u32(state + 0x18);
        w_u8(state + 0x4a, 0);
        w_u8(state + 0x49, 0x3c);
        w_u32(state + 0x18, flags | 8);
    }
    w_u32(state + 0x18, r_u32(state + 0x18) & 0xffffffef);
}

void G3MonsterMoveList1634(uint32 object, uint32 index)
{
    FUNCTION_MARKER(0x00001634, "G3L_65");
    G3Function80045BA0(object);
    uint32 game = r_u32(0x80097048);
    uint32 lists = r_u32(game + 0x5c);
    G3Function80045B74(lists + index * 8 + 0xc, object);
}

uint32 G3MonsterSelectValue1684(uint32 object)
{
    FUNCTION_MARKER(0x00001684, "G3L_65");
    uint32 model = r_u32(object + 0x24);
    uint32 resource = r_u32(model);
    if (r_u32(resource) == 1) return 0x4c;
    return (r_u32(object + 0x11c) & 0x2000) != 0 ? 0x5f : 0x5e;
}

void G3MonsterResume16CC(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000016cc, "G3L_65");
    uint32 frame = guest_stack - 0x18;
    if ((r_u32(object + 0x11c) & 0x4000000) != 0) G3MonsterMoveReduced1780(object, frame);
    else G3MonsterMoveDefault1730(object, frame);
}

void G3MonsterSuspend1710(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001710, "G3L_65");
    G3MonsterMoveList1634(object, 0);
}

void G3MonsterMoveDefault1730(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001730, "G3L_65");
    uint32 index = G3MonsterSelectValue1684(object);
    G3MonsterMoveList1634(object, index);
    w_u32(object + 0x11c, r_u32(object + 0x11c) & 0xfbffffff);
}

void G3MonsterMoveReduced1780(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001780, "G3L_65");
    uint32 index = G3MonsterSelectValue1684(object);
    G3MonsterMoveList1634(object, index & 0xffffffb3);
    w_u32(object + 0x11c, r_u32(object + 0x11c) | 0x4000000);
}

uint32 G3MonsterClearLine17E0(uint32 start, uint32 end, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000017e0, "G3L_65");
    uint32 frame = guest_stack - 0x38;
    w_u16(frame + 0x20, r_u16(start));
    w_u16(frame + 0x22, r_u16(start + 2));
    uint32 start_z = r_u16(start + 4);
    w_u16(frame + 0x24, start_z);
    w_u16(frame + 0x28, r_u16(end));
    uint32 y = r_u16(end + 2);
    start_z += 400;
    w_u16(frame + 0x2a, y);
    uint32 end_z = r_u16(end + 4);
    uint32 game = r_u32(0x80097048);
    w_u16(frame + 0x24, start_z);
    w_u16(frame + 0x2c, end_z);
    w_u16(frame + 0x2c, end_z + 400);
    uint32 level = r_u32(game + 0x48);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0x1c00be0);
    w_u32(frame + 0x18, 4);
    uint32 world = r_u32(level);
    return G3Function8006238C(world, frame + 0x28, frame + 0x20, 0, frame) == 0 ? 1 : 0;
}

void G3MonsterUpdateLineFlag1874(uint32 start, uint32 end, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001874, "G3L_65");
    uint32 frame = guest_stack - 0x18;
    uint32 clear = G3MonsterClearLine17E0(start, end, frame);
    uint32 flags = r_u32(state + 0x18);
    w_u32(state + 0x18, clear != 0 ? flags | 0x80000 : flags & 0xfff7ffff);
}

void G3MonsterStaggerLine18C0(uint32 start, uint32 end, uint32 state, uint32 phase, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000018c0, "G3L_65");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x18;
    uint32 tick = r_u32(game + 0x14c) & 0xf;
    sint32 remainder = (sint32)phase % 16;
    if (remainder < 0) remainder = -remainder;
    if (tick == (uint32)remainder) G3MonsterUpdateLineFlag1874(start, end, state, frame);
}

void G3MonsterInitializeChain191C(uint32 descriptor, uint32 model, uint32 module_base)
{
    FUNCTION_MARKER(0x0000191c, "G3L_65");
    uint32 identity = r_u32(model + 0x28);
    while (identity != 0x26a0d355)
    {
        G3MonsterInitializeResource2380(descriptor, model, module_base);
        uint32 next = r_u32(model + 0x18);
        w_u32(model + 0x28, 0x26a0d355);
        if (next != 0)
        {
            next = r_u32(next);
            if (next != 0) model = next;
        }
        identity = r_u32(model + 0x28);
    }
}

void G3MonsterQuarterTurn19A8(uint32 vector)
{
    FUNCTION_MARKER(0x000019a8, "G3L_65");
    uint32 x = r_u16(vector);
    uint32 y = r_u16(vector + 2);
    w_u16(vector, y);
    w_u16(vector + 2, 0 - x);
}

uint32 G3MonsterResolveAnimation19C0(uint32 animation, uint32 descriptor)
{
    FUNCTION_MARKER(0x000019c0, "G3L_65");
    if (animation == 0) return 0;
    sint32 index = r_s16(animation);
    if (index < 0) return 0;
    if (index >= r_s16(descriptor + 0xa)) return 0;
    if (r_s16(animation + 4) == -1)
    {
        uint32 table = r_u32(descriptor + 0x10);
        uint32 sequence = r_u32(table + (uint32)index * 4);
        w_u16(animation + 4, r_u16(sequence + 2) - 1);
    }
    return 1;
}

void G3MonsterValidateAnimations1A30(uint32 descriptor, uint32 model)
{
    FUNCTION_MARKER(0x00001a30, "G3L_65");
    for (uint32 index = 0; index < 0x30; ++index)
    {
        uint32 slot = model + 0x2c + index * 4;
        uint32 animation = r_u32(slot);
        if (animation != 0 && (G3MonsterResolveAnimation19C0(animation, descriptor) & 0xff) == 0) w_u32(slot, 0);
    }
}

void G3MonsterClearBytes1ABC(uint32 destination)
{
    FUNCTION_MARKER(0x00001abc, "G3L_65");
    for (uint32 index = 0; index < 0x18; ++index) w_u8(destination + index, 0);
}

void G3MonsterClearPointers1AEC(uint32 destination)
{
    FUNCTION_MARKER(0x00001aec, "G3L_65");
    for (uint32 index = 0; index < 0x30; ++index) w_u32(destination + 0x2c + index * 4, 0);
}

void G3MonsterBindAnimations1B24(uint32 model, uint32 module_base)
{
    FUNCTION_MARKER(0x00001b24, "G3L_65");
    if (model == 0) return;
    uint32 cursor = r_u32(model + 4);
    if (cursor == 0) return;
    const uint32 slots[24] = { 0, 0x14, 0x1c, 0x20, 0x2c, 0x30, 8, 0x34, 0xc, 0x10, 0x18, 4, 0x24, 0x28, 0x38, 0x3c, 0x40, 0x44, 0x48, 0x4c, 0x58, 0x5c, 0x60, 0x64 };
    for (;;)
    {
        uint32 first = r_u32(cursor);
        uint32 expected = r_u32(module_base + 0x7c24);
        if (first == expected)
        {
            uint32 second = r_u32(cursor + 4);
            expected = r_u32(module_base + 0x7c28);
            if (second == expected) return;
        }
        for (uint32 index = 0; index < 24; ++index)
        {
            first = r_u32(cursor);
            expected = r_u32(module_base + 0x7c44 + index * 12);
            if (first == expected)
            {
                uint32 second = r_u32(cursor + 4);
                expected = r_u32(module_base + 0x7c48 + index * 12);
                if (second == expected)
                {
                    w_u32(model + 0x2c + slots[index], cursor + 8);
                    break;
                }
            }
        }
        cursor += 0x20;
    }
}

void G3MonsterClearCommands2060(uint32 table)
{
    FUNCTION_MARKER(0x00002060, "G3L_65");
    for (uint32 index = 0; index < 15; ++index) G3MonsterClearBytes1ABC(table + 0xec + index * 0x18);
}

void G3MonsterBuildDefaults20B0(uint32 model)
{
    FUNCTION_MARKER(0x000020b0, "G3L_65");
    if (r_u32(model + 0x2c) == 0)
    {
        w_u16(model + 0xf0, 0xffff);
        w_u16(model + 0xf2, 100);
        w_u16(model + 0xf4, 0x50);
        w_u16(model + 0xf6, 1);
        w_u32(model + 0x2c, model + 0xec);
    }
    if (r_u32(model + 0x40) == 0)
    {
        {
            uint32 copy_source = r_u32(model + 0x2c);
            uint32 destination = model + 0x104;
            uint32 first = r_u32(copy_source);
            uint32 second = r_u32(copy_source + 4);
            uint32 third = r_u32(copy_source + 8);
            uint32 fourth = r_u32(copy_source + 12);
            w_u32(destination, first);
            w_u32(destination + 4, second);
            w_u32(destination + 8, third);
            w_u32(destination + 12, fourth);
            first = r_u32(copy_source + 0x10);
            second = r_u32(copy_source + 0x14);
            w_u32(destination + 0x10, first);
            w_u32(destination + 0x14, second);
        }
        w_u16(model + 0x110, 300);
        w_u32(model + 0x40, model + 0x104);
    }
    if (r_u32(model + 0x58) == 0) w_u32(model + 0x58, model + 0xec);
    {
        uint32 copy_source = r_u32(model + 0x2c);
        uint32 destination = model + 0x164;
        uint32 first = r_u32(copy_source);
        uint32 second = r_u32(copy_source + 4);
        uint32 third = r_u32(copy_source + 8);
        uint32 fourth = r_u32(copy_source + 12);
        w_u32(destination, first);
        w_u32(destination + 4, second);
        w_u32(destination + 8, third);
        w_u32(destination + 12, fourth);
        first = r_u32(copy_source + 0x10);
        second = r_u32(copy_source + 0x14);
        w_u32(destination + 0x10, first);
        w_u32(destination + 0x14, second);
    }
    {
        uint32 copy_source = r_u32(model + 0x40);
        uint32 destination = model + 0x14c;
        uint32 first = r_u32(copy_source);
        uint32 second = r_u32(copy_source + 4);
        uint32 third = r_u32(copy_source + 8);
        uint32 fourth = r_u32(copy_source + 12);
        w_u32(destination, first);
        w_u32(destination + 4, second);
        w_u32(destination + 8, third);
        w_u32(destination + 12, fourth);
        first = r_u32(copy_source + 0x10);
        second = r_u32(copy_source + 0x14);
        w_u32(destination + 0x10, first);
        w_u32(destination + 0x14, second);
    }
    sint32 rate = r_s16(model + 0x152);
    uint32 source = r_u32(model + 0x40);
    w_u16(model + 0x152, (uint32)(rate * 60 / 100));
    {
        uint32 copy_source = source;
        uint32 destination = model + 0x17c;
        uint32 first = r_u32(copy_source);
        uint32 second = r_u32(copy_source + 4);
        uint32 third = r_u32(copy_source + 8);
        uint32 fourth = r_u32(copy_source + 12);
        w_u32(destination, first);
        w_u32(destination + 4, second);
        w_u32(destination + 8, third);
        w_u32(destination + 12, fourth);
        first = r_u32(copy_source + 0x10);
        second = r_u32(copy_source + 0x14);
        w_u32(destination + 0x10, first);
        w_u32(destination + 0x14, second);
    }
    w_u16(model + 0x182, 0 - (uint32)r_s16(model + 0x182));
}
