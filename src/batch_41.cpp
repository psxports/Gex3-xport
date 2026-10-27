#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32 G3CowboyBEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_14");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3CowboyBInitialize(object, module_base);
        return 1;
    case 2:
        G3CowboyBUpdate(object, game, module_base, frame);
        return 1;
    case 3:
        G3CowboyBCollide(object, game, frame);
        return 1;
    case 5:
    {
        uint32 data = r_u32(payload + 0x1c);
        if (data != 0)
            w_u16(data + 0x18, 0);
        return 1;
    }
    default:
        return 0;
    }
}

void G3CowboyBInitialize(uint32 object, uint32 module_base)
{
    FUNCTION_MARKER(0x0000009c, "G3L_14");
    uint32 resource = r_u32(object + 0x1c);
    uint32 custom = r_u32(object + 0x28);
    uint32 x = r_u16(object + 0x58);
    uint32 config = r_u32(resource + 0x1c);
    uint32 state = object + 0x104;
    w_u16(state + 0xc, x);
    uint32 y = r_u16(object + 0x5a);
    w_u16(state + 0xe, y);
    uint32 z = r_u16(object + 0x5c);
    w_u16(state + 0x10, z);
    uint32 parameters = custom == 0 ? config + 0x2c : custom;
    uint32 radius = (uint32)r_s16(parameters);
    w_u32(state, radius * radius);
    radius = (uint32)r_s16(parameters + 2);
    w_u32(state + 4, radius * radius);
    uint32 origins[3] = { x, y, z };
    for (uint32 index = 0; index != 9; ++index)
    {
        uint32 offset = r_u16(parameters + 4 + index * 2);
        w_u16(state + 0x12 + index * 2, origins[index % 3] + offset);
    }
    if (custom != 0)
    {
        sint32 health = r_s16(custom + 0x18);
        uint32 health_half = r_u16(custom + 0x18);
        if (health != 0)
            w_u16(state + 0x2a, health_half);
        else
        {
            health_half = r_u16(config + 0x22);
            w_u16(state + 0x2a, health_half);
        }
        w_u32(state + 0x2c, custom + 0x18);
        uint32 flag = r_u16(custom + 0x16);
        w_u16(state + 0x34, flag);
        if ((flag & 0xffff) != 0)
        {
            uint32 flags = r_u32(object + 0x14);
            w_u32(object + 0x14, flags | 0x40000000);
        }
    }
    else
    {
        uint32 health = r_u16(config + 0x22);
        w_u32(state + 0x2c, 0);
        w_u16(state + 0x34, 0);
        w_u16(state + 0x2a, health);
    }
    if (r_s16(config + 0x42) == 0)
    {
        sint32 turn = (sint32)((uint32)r_s16(config) << 12);
        sint32 threshold = (sint32)((uint32)r_s16(config + 2) << 12);
        sint32 projectile = (sint32)((uint32)r_s16(config + 0x10) << 12);
        sint32 projectile_angle = (sint32)((uint32)r_s16(config + 0x12) << 12);
        w_u16(config + 0x42, 1);
        sint32 rotation = (sint32)((uint32)r_s16(config + 0x24) << 12);
        w_u16(config, (uint32)(turn / 21600));
        w_u16(config + 2, (uint32)(threshold / 21600));
        sint32 scale = (sint32)((uint32)r_s16(config + 0x26) << 12);
        w_u16(config + 0x10, (uint32)(projectile / 21600));
        w_u16(config + 0x12, (uint32)(projectile_angle / 360));
        w_u16(config + 0x24, (uint32)(rotation / 21600));
        w_u16(config + 0x26, (uint32)(scale / 1000));
    }
    uint32 flags = r_u32(object + 0x10);
    uint32 yaw = (uint32)r_s16(object + 0x64);
    w_u32(object + 0x150, 0);
    w_u32(object + 0x10, flags | 0x800);
    w_u32(state + 8, yaw);
    uint32 random = G3SdkRandom80027FA8();
    w_u16(state + 0x26, 0);
    w_u16(state + 0x24, (uint32)((sint32)random % 3));
    uint32 projectile_resource = G3Function8003AA48(module_base + 0xc44);
    w_u32(state + 0x30, projectile_resource);
    if (projectile_resource == 0)
        G3SdkPrint80085E84(module_base + 0xc50);
}

void G3CowboyBCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000ae4, "G3L_14");
    uint32 frame = guest_stack - 0x38;
    uint32 state = object + 0x104;
    uint32 contact = r_u32(object + 0xb0);
    uint32 resource = r_u32(object + 0x1c);
    uint32 player = r_u32(game + 0x50);
    uint32 config = r_u32(resource + 0x1c);
    G3Function8003C548(contact, frame + 0x10, frame + 0x11);
    if (r_u32(contact + 0x14) != player)
        return;
    uint32 kind = r_u8(frame + 0x11);
    if (kind == 9 || kind - 3 < 3)
    {
        if (r_u32(object + 0x150) == 3)
        {
            G3Function80074044(object, game, frame);
            return;
        }
    }
    kind = r_u8(frame + 0x11);
    if (kind - 6 >= 4)
        return;
    uint32 mode = r_u32(object + 0x150);
    if (mode == 0 || mode == 5 || mode == 4)
        return;
    if ((r_u32(player + 0x154) & 0xa0) != 0)
        return;
    uint32 health = r_u16(state + 0x2a) - 1;
    if ((sint16)health > 0)
    {
        w_u32(object + 0x150, 5);
        w_u16(object + 0x6e, 0);
        w_u16(object + 0x5e, 0);
        uint32 persisted_health = r_u32(state + 0x2c);
        w_u16(state + 0x2a, health);
        if (persisted_health != 0)
            w_u16(persisted_health, health);
        uint32 cooldown = r_u16(config + 0x20);
        w_u16(state + 0x26, cooldown);
    }
    else
    {
        uint32 selector = (uint32)r_s16(config + 0x28);
        uint32 random_range = (uint32)r_s16(config + 0x2a);
        G3Function80038798(object, selector, random_range, frame);
    }
}

void G3CowboyBUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000434, "G3L_14");
    uint32 frame = guest_stack - 0x40;
    uint32 player = r_u32(game + 0x50);
    uint32 mode = r_u32(object + 0x150);
    uint32 player_position = player + 0x58;
    uint32 resource = r_u32(object + 0x1c);
    uint32 config = r_u32(resource + 0x1c);
    uint32 state = object + 0x104;
    if (mode >= 6)
        return;
    uint32 target = r_u32(module_base + 0xc74 + mode * 4);
    switch (target - module_base)
    {
    case 0x498:
    {
        sint32 timer = r_s16(state + 0x26);
        uint32 timer_half = r_u16(state + 0x26);
        if (timer > 0)
        {
            --timer_half;
            w_u16(state + 0x26, timer_half);
            if ((timer_half & 0xffff) != 0)
                return;
        }
        uint32 distance = 0;
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 origin = r_u16(state + 0xc + axis * 2);
            uint32 coordinate = r_u16(player_position + axis * 2);
            uint32 delta = origin - coordinate;
            uint32 signed_delta = (uint32)(sint32)(sint16)delta;
            distance += signed_delta * signed_delta;
            w_u16(frame + 0x10 + axis * 2, delta);
        }
        uint32 radius = r_u32(state);
        if ((sint32)radius < (sint32)distance)
            return;
        w_u32(object + 0x150, 1);
        uint32 flags = r_u32(object + 0x10);
        w_u16(object + 0x6e, 0);
        w_u16(object + 0x5e, 0);
        w_u32(object + 0x10, flags & 0xfffff7ff);
        uint32 selected = (uint32)r_s16(state + 0x24);
        uint32 source = state + selected * 6 + 0x12;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 lane = source & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
        uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
        uint32 position = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        uint32 z = (uint32)r_s16(source + 4);
        uint32 destination = object + 0x58;
        high_address = destination + 3;
        high_lane = high_address & 3;
        lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, position >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, position << (lane * 8), (15u << lane) & 15);
        w_u16(object + 0x5c, z);
        w_u16(object + 0x6c, 0);
        w_u16(object + 0x6a, 0);
        w_u16(object + 0x68, 0);
        return;
    }
    case 0x598:
    {
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 scale = r_u16(object + 0x68);
        uint32 speed = r_u16(config + 0x26);
        scale += speed;
        w_u16(object + 0x68, scale);
        if ((sint16)scale >= 0x1000)
        {
            w_u16(object + 0x6c, 0x1000);
            w_u16(object + 0x6a, 0x1000);
            w_u16(object + 0x68, 0x1000);
            w_u32(object + 0x150, 2);
            w_u16(object + 0x6e, 0);
            w_u16(object + 0x5e, 0);
            return;
        }
        scale = r_u16(object + 0x6a);
        speed = r_u16(config + 0x26);
        w_u16(object + 0x6a, scale + speed);
        scale = r_u16(object + 0x6c);
        speed = r_u16(config + 0x26);
        w_u16(object + 0x6c, scale + speed);
        uint32 yaw = r_u16(object + 0x64);
        uint32 rotation = r_u16(config + 0x24);
        w_u16(object + 0x64, yaw + rotation);
        return;
    }
    case 0x614:
    {
        uint32 distance = 0;
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 coordinate = r_u16(player_position + axis * 2);
            uint32 origin = r_u16(state + 0xc + axis * 2);
            uint32 delta = coordinate - origin;
            uint32 signed_delta = (uint32)(sint32)(sint16)delta;
            distance += signed_delta * signed_delta;
            w_u16(frame + 0x10 + axis * 2, delta);
        }
        uint32 radius = r_u32(state + 4);
        if ((sint32)distance >= (sint32)radius)
        {
            w_u32(object + 0x150, 4);
            return;
        }
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 player_x = r_u16(player_position);
        uint32 object_x = r_u16(object + 0x58);
        uint32 delta_x = player_x - object_x;
        w_u16(frame + 0x10, delta_x);
        uint32 player_y = r_u16(player_position + 2);
        uint32 object_y = r_u16(object + 0x5a);
        uint32 delta_y = player_y - object_y;
        w_u16(frame + 0x12, delta_y);
        uint32 player_z = r_u16(player_position + 4);
        uint32 object_z = r_u16(object + 0x5c);
        w_u16(frame + 0x14, player_z - object_z);
        uint32 angle = G3Function8002F240((uint32)(sint32)(sint16)delta_y, (uint32)(sint32)(sint16)delta_x, frame);
        uint32 yaw = (uint32)r_s16(object + 0x64);
        sint32 difference = (sint16)G3Function80038B48(yaw, (uint32)(sint32)(sint16)(angle + 0x400));
        if (difference < 0)
        {
            sint32 threshold = r_s16(config + 2);
            if (difference < -threshold)
            {
                yaw = r_u16(object + 0x64);
                uint32 speed = r_u16(config);
                w_u16(object + 0x64, yaw - speed);
                return;
            }
        }
        else
        {
            sint32 threshold = r_s16(config + 2);
            if (threshold < difference)
            {
                yaw = r_u16(object + 0x64);
                uint32 speed = r_u16(config);
                w_u16(object + 0x64, yaw + speed);
                return;
            }
        }
        sint32 timer = r_s16(state + 0x26);
        uint32 timer_half = r_u16(state + 0x26);
        bool attack = timer <= 0;
        if (!attack)
        {
            --timer_half;
            w_u16(state + 0x26, timer_half);
            attack = (timer_half & 0xffff) == 0;
        }
        if (attack)
        {
            w_u32(object + 0x150, 3);
            w_u16(object + 0x6e, 1);
            w_u16(object + 0x5e, 0);
            w_u16(state + 0x28, 0);
        }
        yaw = r_u16(object + 0x64);
        w_u16(object + 0x64, yaw + (uint32)difference);
        return;
    }
    case 0x7ac:
    {
        uint32 flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags & 0xffffffef);
        G3Function80029F0C(object, 0xffffffff, frame);
        if (r_s16(state + 0x28) == 0)
        {
            sint32 animation_frame = r_s16(object + 0x5e);
            sint32 fire_frame = r_s16(config + 4);
            if (animation_frame == fire_frame)
            {
                uint32 projectile_data = r_u32(state + 0x30);
                w_u16(state + 0x28, 1);
                uint32 projectile = G3Function8002A454(object, projectile_data, frame);
                if (projectile != 0)
                {
                    uint32 bone = (uint32)r_s16(config + 6);
                    uint32 matrices = r_u32(object + 0x30);
                    uint32 matrix = matrices + (bone << 5);
                    for (uint32 axis = 0; axis != 3; ++axis)
                    {
                        uint32 x = (uint32)r_s16(config + 8);
                        uint32 first = (uint32)r_s16(matrix + axis * 6);
                        uint32 product_x = x * first;
                        uint32 y = (uint32)r_s16(config + 0xa);
                        uint32 second = (uint32)r_s16(matrix + axis * 6 + 2);
                        uint32 product_y = y * second;
                        uint32 z = (uint32)r_s16(config + 0xc);
                        uint32 third = (uint32)r_s16(matrix + axis * 6 + 4);
                        uint32 product_z = z * third;
                        uint32 position = (uint32)((sint32)product_x >> 12) + (uint32)((sint32)product_y >> 12) + (uint32)((sint32)product_z >> 12);
                        w_u16(projectile + 0x58 + axis * 2, position);
                    }
                    for (uint32 axis = 0; axis != 3; ++axis)
                    {
                        uint32 position = r_u16(projectile + 0x58 + axis * 2);
                        uint32 translation = r_u16(matrix + 0x14 + axis * 4);
                        w_u16(projectile + 0x58 + axis * 2, position + translation);
                    }
                    uint32 value = r_u16(config + 0xe);
                    w_u16(frame + 0x18, value);
                    value = r_u16(config + 0x12);
                    w_u16(frame + 0x1a, value);
                    value = r_u16(config + 0x10);
                    w_u16(frame + 0x1c, value);
                    value = r_u16(config + 0x14);
                    w_u16(frame + 0x1e, value);
                    value = r_u16(config + 0x16);
                    w_u16(frame + 0x20, value);
                    value = r_u16(config + 0x18);
                    w_u16(frame + 0x22, value);
                    value = r_u16(config + 0x1a);
                    w_u16(frame + 0x26, value);
                    value = r_u16(config + 0x1c);
                    w_u16(frame + 0x24, value);
                    uint32 callback = r_u32(projectile + 0xec);
                    if (callback != 0)
                        G3DispatchStateEvent(callback, projectile, game, 0x7e1, frame + 0x18, frame);
                }
                else
                    G3SdkPrint80085E84(module_base + 0xc68);
            }
        }
        if ((r_u32(object + 0x14) & 0x10) != 0)
        {
            uint32 timer = r_u16(config + 0x1e);
            w_u16(state + 0x26, timer);
            w_u32(object + 0x150, 2);
            w_u16(object + 0x6e, 0);
            w_u16(object + 0x5e, 0);
        }
        return;
    }
    case 0xa18:
    {
        uint32 scale = r_u16(object + 0x68);
        uint32 speed = r_u16(config + 0x26);
        scale -= speed;
        w_u16(object + 0x68, scale);
        if ((sint16)scale <= 0)
        {
            uint32 flags = r_u32(object + 0x10);
            w_u32(object + 0x150, 0);
            w_u32(object + 0x10, flags | 0x800);
            uint32 random = G3SdkRandom80027FA8();
            uint32 selected = r_u16(state + 0x24);
            selected += 1 + (random & 1);
            w_u16(state + 0x24, selected);
            if ((sint16)selected >= 3)
                w_u16(state + 0x24, selected - 3);
            return;
        }
        scale = r_u16(object + 0x6a);
        speed = r_u16(config + 0x26);
        w_u16(object + 0x6a, scale - speed);
        scale = r_u16(object + 0x6c);
        speed = r_u16(config + 0x26);
        w_u16(object + 0x6c, scale - speed);
        uint32 yaw = r_u16(object + 0x64);
        uint32 rotation = r_u16(config + 0x24);
        w_u16(object + 0x64, yaw - rotation);
        return;
    }
    case 0xabc:
        w_u32(object + 0x150, 4);
        return;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved cowboyb state target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}

uint32 G3CrumblingPlatformEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_37");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3CrumblingPlatformInitialize(object, game, frame);
        return 1;
    case 2:
        G3CrumblingPlatformUpdate(object, game, frame);
        return 1;
    case 3:
        G3CrumblingPlatformCollide(object, game, frame);
        return 1;
    default:
        return 0;
    }
}

void G3CrumblingPlatformInitialize(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000088, "G3L_37");
    uint32 frame = guest_stack - 0x28;
    uint32 data = r_u32(object + 0x28);
    uint32 flags = r_u32(object + 0x10);
    w_u16(object + 0x6e, 0);
    w_u16(object + 0x5e, 0);
    w_u32(object + 0x10, (flags & 0xfffffbff) | 0x100000);
    uint32 state = object + 0x104;
    if (data == 0)
    {
        uint32 resource = r_u32(object + 0x1c);
        data = r_u32(resource + 0x1c);
    }
    if (data != 0)
        w_u16(state, r_u16(data));
    else
        w_u16(state, 256);
    if (data != 0)
        w_u16(state + 2, r_u16(data + 2));
    else
        w_u16(state + 2, 20);
    if (data != 0)
        w_u16(state + 4, r_u16(data + 4));
    else
        w_u16(state + 4, 0);
    if (data != 0)
        w_u16(state + 6, r_u16(data + 6));
    else
        w_u16(state + 6, 60);
    w_u16(state + 8, 0);
    w_u32(state + 0x44, 1);
    uint32 x = r_u16(object + 0x58);
    w_u16(frame + 0x18, x);
    w_u16(frame + 0x10, x);
    uint32 y = r_u16(object + 0x5a);
    w_u16(frame + 0x1a, y);
    w_u16(frame + 0x12, y);
    uint32 z = r_u16(object + 0x5c);
    w_u16(state + 0xa, z);
    w_u16(frame + 0x14, z);
    z = r_u16(object + 0x5c);
    w_u16(frame + 0x1c, z + 128);
    uint32 level = r_u32(game + 0x48);
    uint32 terrain = r_u32(level);
    G3Function800622EC(terrain, frame + 0x10, frame + 0x18, object, frame);
}

uint32 G3CrumblingPlatformMovePath(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000001a4, "G3L_37");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    uint32 path = G3Function800833EC(object);
    if (path == 0)
        return 0;
    uint32 point;
    if (r_u32(state + 0x44) != 0)
        point = G3Function8003FC34(path, object + 0x144, frame);
    else
        point = G3Function8003FC94(path, object + 0x144, frame);
    if (point == 0)
    {
        uint32 direction = r_u32(state + 0x44) == 0 ? 1 : 0;
        w_u32(state + 0x44, direction);
        if (direction != 0)
            point = G3Function8003FC34(path, state + 0x40, frame);
        else
            point = G3Function8003FC94(path, state + 0x40, frame);
        if (point == 0)
            return 0;
    }
    w_u16(object + 0x58, r_u16(point));
    w_u16(object + 0x5a, r_u16(point + 2));
    uint32 z = r_u16(point + 4);
    w_u16(object + 0x5c, z);
    return 1;
}

void G3CrumblingPlatformUpdate(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000280, "G3L_37");
    uint32 frame = guest_stack - 0xd8;
    uint32 state = object + 0x104;
    G3FillMemory80086094(frame + 0x10, 0, 8);
    uint32 resource = r_u32(object + 0x1c);
    if ((r_u32(resource + 0x30) & 0x2000) != 0)
        G3Function8003E314(object, frame);
    if ((r_u32(object + 0x10) & 0x800) != 0)
    {
        w_u16(object + 0x5c, r_u16(state + 0xa));
        uint32 next = r_u16(state + 8) - 1;
        w_u16(state + 8, next);
        if ((sint16)next <= 0)
        {
            w_u16(state + 8, 0);
            w_u16(object + 0x6e, 2);
            uint32 flags = r_u32(object + 0x10);
            w_u16(object + 0x5e, 0);
            w_u32(object + 0x10, flags & 0xfffff7ff);
        }
    }
    else
    {
        uint32 selected = r_u16(object + 0x6e);
        if (selected == 1)
        {
            resource = r_u32(object + 0x1c);
            uint32 animations = r_u32(resource + 0x10);
            uint32 animation = r_u32(animations + 4);
            uint32 rest_z = r_u16(state + 0xa);
            sint32 length = r_s16(animation + 2);
            uint32 next = r_u16(object + 0x5e) + 1;
            w_u16(object + 0x5c, rest_z);
            w_u16(object + 0x5e, next);
            if ((sint16)next >= length)
            {
                w_u16(state + 8, r_u16(state + 6));
                uint32 flags = r_u32(object + 0x10);
                w_u16(object + 0x6e, 2);
                w_u16(object + 0x5e, 0);
                w_u32(object + 0x10, flags | 0x800);
                return;
            }
        }
        else if (selected == 2)
        {
            resource = r_u32(object + 0x1c);
            uint32 animations = r_u32(resource + 0x10);
            uint32 animation = r_u32(animations + 8);
            uint32 rest_z = r_u16(state + 0xa);
            sint32 length = r_s16(animation + 2);
            uint32 next = r_u16(object + 0x5e) + 1;
            w_u16(object + 0x5c, rest_z);
            w_u16(object + 0x5e, next);
            if ((sint16)next >= length)
            {
                uint32 flags = r_u32(object + 0x10);
                w_u16(object + 0x6e, 0);
                w_u16(object + 0x5e, 0);
                w_u32(object + 0x10, flags & 0xfffffbff);
            }
        }
        else if (G3CrumblingPlatformMovePath(object, frame) == 0)
            w_u16(object + 0x5c, r_u16(state + 0xa));
    }
    if (r_s16(state + 4) > 0 && r_u32(state + 0x18) == 0 && r_u32(state + 0x1c) == 0 && r_u32(state + 0xc) == 0)
    {
        sint32 random = (sint32)G3SdkRandom80027FA8();
        uint32 angle = (uint32)(sint32)(sint16)(random % 4096);
        uint32 sine = G3Function8005B470(angle);
        uint32 amplitude = (uint32)r_s16(state + 4);
        uint32 product = sine * amplitude;
        if ((sint32)product < 0)
            product += 0xfff;
        uint32 x = (uint32)((sint32)product >> 12);
        uint32 cosine = G3Function8005B590(angle);
        amplitude = (uint32)r_s16(state + 4);
        product = cosine * amplitude;
        if ((sint32)product < 0)
            product += 0xfff;
        uint32 y = (uint32)((sint32)product >> 12);
        uint32 bound = amplitude << 1;
        sint32 speed = r_s32(state + 0x10);
        if ((sint32)(0 - bound) < speed && speed < (sint32)bound)
        {
            w_u32(state + 0x18, (uint32)((sint32)(x << 16) >> 11));
            amplitude = (uint32)r_s16(state + 4);
            w_u32(state + 0x1c, (uint32)((sint32)(y << 16) >> 11));
            if ((sint32)amplitude < 0)
                amplitude += 127;
            uint32 impulse = (uint32)((sint32)amplitude >> 7) + 1;
            w_u32(state + 0xc, 0 - (impulse << 12));
        }
    }
    uint32 angle_x = (uint32)r_s16(state + 0x38);
    uint32 old_vx = r_u32(state + 0x20);
    uint32 angle_y = (uint32)r_s16(state + 0x3a);
    uint32 acceleration_x = (((0 - angle_x) << 5) - (uint32)((sint32)old_vx >> 6)) << 1;
    uint32 old_vy = r_u32(state + 0x24);
    uint32 acceleration_y = (((0 - angle_y) << 5) - (uint32)((sint32)old_vy >> 6)) << 1;
    G3Function8003C614(object, game, frame);
    uint32 force_x = r_u32(state + 0x18);
    old_vx = r_u32(state + 0x20);
    uint32 limit = (uint32)r_s16(state + 2);
    acceleration_x += force_x;
    uint32 force_y = r_u32(state + 0x1c);
    w_u32(state + 0x20, old_vx + acceleration_x);
    old_vy = r_u32(state + 0x24);
    uint32 vx = r_u32(state + 0x20);
    acceleration_y += force_y;
    w_u32(state + 0x24, old_vy + acceleration_y);
    uint32 positive = limit << 12;
    uint32 negative = (0 - limit) << 12;
    if ((sint32)positive < (sint32)vx)
        w_u32(state + 0x20, positive);
    else if ((sint32)vx < (sint32)negative)
        w_u32(state + 0x20, negative);
    limit = (uint32)r_s16(state + 2);
    uint32 vy = r_u32(state + 0x24);
    positive = limit << 12;
    negative = (0 - limit) << 12;
    if ((sint32)positive < (sint32)vy)
        w_u32(state + 0x24, positive);
    else if ((sint32)vy < (sint32)negative)
        w_u32(state + 0x24, negative);
    uint32 old_x = r_u32(state + 0x28);
    vx = r_u32(state + 0x20);
    uint32 old_y = r_u32(state + 0x2c);
    vy = r_u32(state + 0x24);
    uint32 new_x = old_x + vx;
    limit = (uint32)r_s16(state);
    uint32 new_y = old_y + vy;
    w_u32(state + 0x28, new_x);
    w_u32(state + 0x2c, new_y);
    positive = limit << 12;
    negative = (0 - limit) << 12;
    if ((sint32)positive < (sint32)new_x)
        w_u32(state + 0x28, positive);
    else if ((sint32)new_x < (sint32)negative)
        w_u32(state + 0x28, negative);
    limit = (uint32)r_s16(state);
    new_y = r_u32(state + 0x2c);
    positive = limit << 12;
    negative = (0 - limit) << 12;
    if ((sint32)positive < (sint32)new_y)
        w_u32(state + 0x2c, positive);
    else if ((sint32)new_y < (sint32)negative)
        w_u32(state + 0x2c, negative);
    new_x = r_u32(state + 0x28);
    new_y = r_u32(state + 0x2c);
    w_u16(state + 0x3c, 0);
    w_u16(state + 0x38, (uint32)((sint32)new_x >> 12));
    w_u16(state + 0x3a, (uint32)((sint32)new_y >> 12));
    G3Function8005BD00(state + 0x38, frame + 0x20);
    G3Function8005BD00(frame + 0x10, frame + 0x40);
    uint32 intro = r_u32(object + 0x20);
    G3Function8005BD00(intro + 8, frame + 0x80);
    MulMatrix0((MATRIX *)psx_addr(frame + 0x40, 20), (MATRIX *)psx_addr(frame + 0x20, 20), (MATRIX *)psx_addr(frame + 0x60, 20));
    MulMatrix0((MATRIX *)psx_addr(frame + 0x80, 20), (MATRIX *)psx_addr(frame + 0x60, 20), (MATRIX *)psx_addr(frame + 0xa0, 20));
    G3Function800202A4(frame + 0xa0, object + 0x60, 0x15, frame);
    uint32 offset = r_u32(state + 0x14);
    uint32 velocity = r_u32(state + 0x10);
    uint32 acceleration = r_u32(state + 0xc);
    uint32 damping = (uint32)((sint32)(0 - offset) >> 4) - (uint32)((sint32)velocity >> 6);
    acceleration += damping;
    offset = r_u32(state + 0x14);
    velocity += acceleration;
    w_u32(state + 0x10, velocity);
    offset += velocity;
    w_u32(state + 0x14, offset);
    uint32 z = r_u16(object + 0x5c);
    w_u16(object + 0x5c, z + (uint32)((sint32)offset >> 12));
    uint32 contact = r_u32(state + 0x30);
    w_u32(state + 0x30, 0);
    w_u32(state + 0x18, 0);
    w_u32(state + 0x1c, 0);
    w_u32(state + 0xc, 0);
    w_u32(state + 0x34, contact);
}

void G3CrumblingPlatformCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000774, "G3L_37");
    uint32 frame = guest_stack - 0x60;
    uint32 contact = r_u32(object + 0xb0);
    uint32 state = object + 0x104;
    if (r_s16(contact + 6) != 4)
        return;
    uint32 other = r_u32(contact + 0x14);
    uint32 player = r_u32(game + 0x50);
    if (other != player)
        return;
    uint32 matrix = r_u32(object + 0x30);
    if (matrix == 0)
        return;
    G3Function800313E0(frame + 0x20, matrix, frame);
    uint32 first = r_u32(frame + 0x20);
    uint32 second = r_u32(frame + 0x24);
    xport_gte_write_control(0, first);
    xport_gte_write_control(1, second);
    uint32 third = r_u32(frame + 0x28);
    uint32 fourth = r_u32(frame + 0x2c);
    uint32 fifth = r_u32(frame + 0x30);
    xport_gte_write_control(2, third);
    xport_gte_write_control(3, fourth);
    xport_gte_write_control(4, fifth);
    uint32 tx = r_u32(frame + 0x34);
    uint32 ty = r_u32(frame + 0x38);
    xport_gte_write_control(5, tx);
    uint32 tz = r_u32(frame + 0x3c);
    xport_gte_write_control(6, ty);
    xport_gte_write_control(7, tz);
    // Uses corrected MAC output without a pad store
    RotTrans((SVECTOR *)psx_addr(contact + 0x18, 8), (VECTOR *)psx_addr(frame + 0x10, 12), (sint32 *)psx_addr(frame + 0x40, 4));
    uint32 y = r_u32(frame + 0x14);
    w_u32(state + 0x18, (0 - y) << 6);
    uint32 x = r_u32(frame + 0x10);
    uint32 previous = r_u32(state + 0x34);
    w_u32(state + 0x1c, x << 6);
    w_u32(state + 0xc, previous != 0 ? 0xfffff000 : 0xffffc000);
    other = r_u32(contact + 0x14);
    w_u32(state + 0x30, other);
    G3Function8003C864(object, game, frame);
    if (r_u16(object + 0x6e) == 0)
    {
        uint32 flags = r_u32(object + 0x10);
        w_u16(object + 0x6e, 1);
        w_u16(object + 0x5e, 0);
        w_u32(object + 0x10, flags | 0x400);
    }
}

uint32 G3CountdownEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_21");
    uint32 frame = guest_stack - 0x18;
    uint32 index = message - 1;
    if (index >= 12)
        return 0;
    uint32 target = r_u32(module_base + 0x774 + (index << 2));
    switch (target - module_base)
    {
    case 0x3c:
        G3CountdownInitialize(object, module_base, frame);
        return 1;
    case 0x4c:
        G3CountdownUpdate(object, game, frame);
        return 1;
    case 0x5c:
        G3CountdownConfigure(object, payload, module_base, frame);
        return 1;
    case 0x6c:
        G3CountdownPlacement(payload);
        return 1;
    case 0x7c:
        return 0;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved countdown event target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}

void G3CountdownPlacement(uint32 intro)
{
    FUNCTION_MARKER(0x00000094, "G3L_21");
    w_u32(intro + 0x18, r_u32(intro + 0x18) | 0x80);
}

uint32 G3CountdownRecalculate(uint32 state)
{
    FUNCTION_MARKER(0x000000a8, "G3L_21");
    uint32 minutes = (uint32)r_s16(state);
    uint32 seconds = (uint32)r_s16(state + 2);
    uint32 total = minutes * 60 + seconds;
    w_u32(state + 8, total);
    return total;
}

void G3CountdownInitialize(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000cc, "G3L_21");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    w_u16(state, 0);
    w_u16(state + 2, 10);
    uint32 properties = r_u32(object + 0x24);
    if (properties != 0)
        G3CountdownConfigure(object, properties, module_base, frame);
    properties = r_u32(object + 0x28);
    if (properties != 0)
        G3CountdownConfigure(object, properties, module_base, frame);
    G3CountdownRecalculate(state);
    uint32 resource = r_u32(object + 0x1c);
    w_u32(resource + 0x30, r_u32(resource + 0x30) | 8);
    w_u32(object + 0x10, r_u32(object + 0x10) | 0xc00);
}

void G3CountdownUpdate(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000164, "G3L_21");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    if (r_u32(state + 0x18) == 0)
    {
        uint32 intro = r_u32(object + 0x20);
        if (intro != 0 && (r_u32(intro + 0x18) & 0x80) != 0)
            return;
        if (r_s16(state + 4) >= 30)
            w_u16(state + 4, 0);
        sint32 total = r_s32(state + 8);
        sint32 minutes = total / 60;
        sint32 seconds = total % 60;
        bool expired = (sint16)minutes <= 0 && (sint16)seconds <= 0;
        if (expired || r_u32(state + 0x10) != 0)
        {
            uint32 sequence = r_u32(state + 0x14);
            if (sequence != 0)
                G3Function80018DE0(object, sequence + 4, frame);
            if (r_u32(state + 0x18) == 0)
            {
                uint32 flags = r_u32(object + 0x10);
                intro = r_u32(object + 0x20);
                w_u32(object + 0x10, flags & 0xfffffbff);
                w_u32(intro + 0x18, r_u32(intro + 0x18) | 0x80);
                return;
            }
        }
        if (G3Function8004DACC() == 0)
        {
            uint32 display_minutes = (uint32)(sint32)(sint16)minutes;
            uint32 display_seconds = (uint32)(sint32)(sint16)seconds;
            uint32 ticks = (uint32)r_s16(state + 4);
            G3Function8003A150(display_minutes, display_seconds, ticks, frame);
        }
        if (G3Function8004DACC() != 0)
            return;
        if ((r_u32(game + 0x69b8) & 0x10000) != 0)
            return;
        if (r_u32(game + 0x67c4) != 0)
            return;
        if (r_s16(state + 4) == 0)
            w_u32(state + 8, r_u32(state + 8) - 1);
        w_u16(state + 4, r_u16(state + 4) + 1);
        return;
    }
    uint32 player = r_u32(game + 0x50);
    w_u32(player + 0x10, r_u32(player + 0x10) | 0x100);
    sint32 completed = r_s16(state + 0x1a);
    uint32 raw_completed = r_u16(state + 0x1a);
    uint32 token;
    bool finished;
    if (completed != 0)
    {
        uint32 next = raw_completed - 1;
        w_u16(state + 0x1a, next);
        finished = (sint16)next == 0;
        if (!finished)
        {
            G3Function80021A74(320);
            token = 0x47;
        }
    }
    else
    {
        uint32 next = r_u16(state + 0x18) - 1;
        w_u16(state + 0x18, next);
        finished = (sint16)next == 0;
        if (!finished)
        {
            G3Function80021A74(450);
            token = 0x46;
        }
    }
    if (finished)
    {
        uint32 flags = r_u32(object + 0x10);
        uint32 intro = r_u32(object + 0x20);
        w_u32(object + 0x10, flags & 0xfffffbff);
        w_u32(intro + 0x18, r_u32(intro + 0x18) | 0x80);
        uint32 root = r_u32(0x8009d668);
        uint32 name = r_u32(0x8009d66c);
        G3Function80027644(root, name, game, frame);
    }
    else
    {
        uint32 text = G3Function80065D38(token);
        G3Function800234A0(text, 120, 0, 0, frame, 0);
    }
}

void G3CountdownConfigure(uint32 object, uint32 properties, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000003f8, "G3L_21");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    bool recalculate = false;
    w_u32(frame + 0x34, properties);
    if (properties == 0)
        return;
    for (;;)
    {
        uint32 current = r_u32(frame + 0x34);
        if (r_u32(current) == r_u32(module_base + 0x7a4) && r_u32(current + 4) == r_u32(module_base + 0x7a8))
            break;
        uint32 words = 2;
        uint32 data = current + 8;
        bool recognized = true;
        if (r_u32(current) == r_u32(module_base + 0x7b0) && r_u32(current + 4) == r_u32(module_base + 0x7b4))
        {
            recalculate = true;
            words = 3;
            w_u16(state, r_u16(current + 8));
        }
        else if (r_u32(current) == r_u32(module_base + 0x7bc) && r_u32(current + 4) == r_u32(module_base + 0x7c0))
        {
            sint32 value = r_s32(data);
            recalculate = true;
            ++words;
            w_u16(state + 2, (uint32)(value % 60));
        }
        else if ((r_u32(current) == r_u32(module_base + 0x7c8) && r_u32(current + 4) == r_u32(module_base + 0x7cc)) || (r_u32(current) == r_u32(module_base + 0x7d4) && r_u32(current + 4) == r_u32(module_base + 0x7d8)))
        {
            uint32 old_time = r_u32(state + 8);
            uint32 value = r_u32(data);
            ++words;
            w_u32(state + 8, old_time + value);
        }
        else if (r_u32(current) == r_u32(module_base + 0x7e0) && r_u32(current + 4) == r_u32(module_base + 0x7e4))
        {
            uint32 value = r_u32(data);
            ++words;
            w_u32(state + 0x14, value);
        }
        else if (r_u32(current) == r_u32(module_base + 0x7ec) && r_u32(current + 4) == r_u32(module_base + 0x7f0))
            w_u16(state + 0x18, 60);
        else if (r_u32(current) == r_u32(module_base + 0x7f8) && r_u32(current + 4) == r_u32(module_base + 0x7fc))
        {
            uint32 game = r_u32(0x80097048);
            uint32 receiver = G3Function8006C914(game, frame);
            if (receiver != 0)
            {
                uint32 target = r_u32(receiver + 0xec);
                if (target != 0)
                {
                    game = r_u32(0x80097048);
                    G3DispatchStateEvent(target, receiver, game, 2030, 0, frame);
                }
            }
            w_u16(state + 0x1a, 90);
        }
        else if (r_u32(current) == r_u32(module_base + 0x804) && r_u32(current + 4) == r_u32(module_base + 0x808))
        {
            G3Function8002A9AC(object, frame);
            return;
        }
        else if (r_u32(current) == r_u32(module_base + 0x810) && r_u32(current + 4) == r_u32(module_base + 0x814))
            w_u32(state + 0x10, 1);
        else
            recognized = false;
        if (recognized)
        {
            current = r_u32(frame + 0x34);
            uint32 bytes = (uint32)((sint32)(words << 16) >> 14);
            w_u32(frame + 0x34, current + bytes);
        }
        else if (G3Function80085B84(frame + 0x34, frame) == 0)
            break;
    }
    if (recalculate)
        G3CountdownRecalculate(state);
}

uint32 G3DoorEvent(uint32 object, uint32 game, uint32 message, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_07");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3DoorInitialize(object, frame);
        return 1;
    case 2:
        G3DoorUpdate(object, game, frame);
        return 1;
    default:
        return 0;
    }
}

void G3DoorInitialize(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000005c, "G3L_07");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0xfc;
    uint32 properties = r_u32(object + 0x28);
    w_u32(object + 0x40, 0);
    w_u16(object + 0x4e, 0);
    w_u16(object + 0x4c, 0);
    w_u8(state + 0x26, 0);
    uint32 source = object + 0x58;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 lane = source & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
    uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
    uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
    uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
    uint32 last = (uint32)r_s16(source + 4);
    uint32 destination = object + 0x88;
    high_address = destination + 3;
    high_lane = high_address & 3;
    lane = destination & 3;
    w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
    w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
    w_u16(destination + 4, last);
    if (properties != 0)
    {
        value = r_u16(properties);
        w_u16(object + 0x100, value);
    }
    else
        w_u16(object + 0x100, 0);
    for (uint32 pivot = 0; pivot != 3; ++pivot)
    {
        if (pivot == 0)
        {
            w_u16(frame + 0x10, 0);
            w_u16(frame + 0x12, 0x40);
        }
        else if (pivot == 1)
        {
            w_u16(frame + 0x10, 0);
            w_u16(frame + 0x12, (uint32)-0x40);
        }
        else
        {
            value = r_u16(object + 0x100);
            w_u16(frame + 0x10, (value & 4) != 0 ? 0x280 : 0x140);
            w_u16(frame + 0x12, 0);
        }
        w_u16(frame + 0x14, 0);
        uint32 yaw = (uint32)r_s16(object + 0x64);
        G3Function8002F8E4(frame + 0x10, yaw);
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 home = r_u16(object + 0x88 + axis * 2);
            uint32 rotated = r_u16(frame + 0x10 + axis * 2);
            w_u16(state + 0x14 + pivot * 6 + axis * 2, home + rotated);
        }
    }
    value = r_u16(object + 0x100);
    if ((value & 4) != 0)
    {
        w_u16(state + 6, 0x514);
        w_u16(state + 8, (uint32)-20);
        w_u16(state + 0x12, 2000);
    }
    else
    {
        w_u16(state + 6, 0x294);
        w_u16(state + 8, (uint32)-20);
        w_u16(state + 0x12, 1200);
    }
    w_u16(state + 0xa, 700);
    w_u16(state + 0xc, (uint32)-200);
    value = r_u16(object + 0x100);
    if ((value & 2) != 0)
    {
        w_u16(state + 0xe, 0x300);
        w_u16(state + 0x10, 0x200);
    }
    else
    {
        w_u16(state + 0xe, (uint32)-0x300);
        w_u16(state + 0x10, (uint32)-0x200);
    }
}

void G3DoorOpenNegative(uint32 object, uint32 pivot_xy, uint32 pivot_z, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000268, "G3L_07");
    uint32 frame = guest_stack - 0x20;
    w_u32(frame + 0x24, pivot_xy);
    w_u32(frame + 0x28, pivot_z);
    sint32 yaw = r_s16(object + 0x64);
    uint32 yaw_half = r_u16(object + 0x64);
    if (yaw >= -0x3ff)
    {
        w_u16(object + 0x64, yaw_half + (uint32)-32);
        w_u16(frame + 0x10, 0);
        w_u16(frame + 0x12, (uint32)64);
        w_u16(frame + 0x14, 0);
        uint32 angle = (uint32)r_s16(object + 0x64);
        G3Function8002F8E4(frame + 0x10, angle);
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 rotated = r_u16(frame + 0x10 + axis * 2);
            uint32 pivot = r_u16(frame + 0x24 + axis * 2);
            w_u16(object + 0x58 + axis * 2, rotated + pivot);
        }
    }
    else
        w_u8(object + 0x122, 4);
}

void G3DoorClosePositive(uint32 object, uint32 pivot_xy, uint32 pivot_z, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000310, "G3L_07");
    uint32 frame = guest_stack - 0x20;
    w_u32(frame + 0x24, pivot_xy);
    w_u32(frame + 0x28, pivot_z);
    sint32 yaw = r_s16(object + 0x64);
    uint32 yaw_half = r_u16(object + 0x64);
    if (yaw > 0)
    {
        w_u16(object + 0x64, yaw_half + (uint32)-32);
        w_u16(frame + 0x10, 0);
        w_u16(frame + 0x12, (uint32)-64);
        w_u16(frame + 0x14, 0);
        uint32 angle = (uint32)r_s16(object + 0x64);
        G3Function8002F8E4(frame + 0x10, angle);
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 rotated = r_u16(frame + 0x10 + axis * 2);
            uint32 pivot = r_u16(frame + 0x24 + axis * 2);
            w_u16(object + 0x58 + axis * 2, rotated + pivot);
        }
    }
    else
        w_u8(object + 0x122, 0);
}

void G3DoorOpenPositive(uint32 object, uint32 pivot_xy, uint32 pivot_z, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000003b0, "G3L_07");
    uint32 frame = guest_stack - 0x20;
    w_u32(frame + 0x24, pivot_xy);
    w_u32(frame + 0x28, pivot_z);
    sint32 yaw = r_s16(object + 0x64);
    uint32 yaw_half = r_u16(object + 0x64);
    if (yaw < 0x400)
    {
        w_u16(object + 0x64, yaw_half + (uint32)32);
        w_u16(frame + 0x10, 0);
        w_u16(frame + 0x12, (uint32)-64);
        w_u16(frame + 0x14, 0);
        uint32 angle = (uint32)r_s16(object + 0x64);
        G3Function8002F8E4(frame + 0x10, angle);
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 rotated = r_u16(frame + 0x10 + axis * 2);
            uint32 pivot = r_u16(frame + 0x24 + axis * 2);
            w_u16(object + 0x58 + axis * 2, rotated + pivot);
        }
    }
    else
        w_u8(object + 0x122, 4);
}

void G3DoorCloseNegative(uint32 object, uint32 pivot_xy, uint32 pivot_z, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000458, "G3L_07");
    uint32 frame = guest_stack - 0x20;
    w_u32(frame + 0x24, pivot_xy);
    w_u32(frame + 0x28, pivot_z);
    sint32 yaw = r_s16(object + 0x64);
    uint32 yaw_half = r_u16(object + 0x64);
    if (yaw < 0)
    {
        w_u16(object + 0x64, yaw_half + (uint32)32);
        w_u16(frame + 0x10, 0);
        w_u16(frame + 0x12, (uint32)64);
        w_u16(frame + 0x14, 0);
        uint32 angle = (uint32)r_s16(object + 0x64);
        G3Function8002F8E4(frame + 0x10, angle);
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 rotated = r_u16(frame + 0x10 + axis * 2);
            uint32 pivot = r_u16(frame + 0x24 + axis * 2);
            w_u16(object + 0x58 + axis * 2, rotated + pivot);
        }
    }
    else
        w_u8(object + 0x122, 0);
}

void G3DoorUpdate(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000004f8, "G3L_07");
    uint32 frame = guest_stack - 0x40;
    uint32 state = object + 0xfc;
    uint32 handle = r_u32(object + 0x40);
    if (handle != 0)
    {
        if (G3Function8003F088(handle) == 0)
            w_u32(object + 0x40, 0);
        else
        {
            w_u32(frame + 0x10, 0x157c);
            handle = r_u32(object + 0x40);
            G3Function8003E7E4(object, handle, 0, 0x5a, frame);
        }
    }
    uint32 flags = r_u8(state + 0x27);
    w_u8(state + 0x27, flags & 0xfe);
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        uint32 player = r_u32(game + 0x50);
        uint32 door_coordinate = r_u16(state + 0x20 + axis * 2);
        uint32 player_coordinate = r_u16(player + 0x58 + axis * 2);
        w_u16(frame + 0x20 + axis * 2, door_coordinate - player_coordinate);
    }
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        uint32 player = r_u32(game + 0x50);
        uint32 door_coordinate = r_u16(object + 0x88 + axis * 2);
        uint32 player_coordinate = r_u16(player + 0x58 + axis * 2);
        w_u16(frame + 0x18 + axis * 2, door_coordinate - player_coordinate);
    }
    uint32 placement = r_u32(object + 0x20);
    uint32 yaw = r_u16(placement + 0xc);
    G3Function8002F8E4(frame + 0x18, (uint32)(sint32)(sint16)(0u - yaw));
    uint32 local_x = (uint32)r_s16(frame + 0x18);
    uint32 local_z = (uint32)r_s16(frame + 0x1c);
    w_u16(frame + 0x18, 0u - local_x);
    uint32 local_y = (uint32)r_s16(frame + 0x1a);
    uint32 negative_z = 0u - local_z;
    w_u16(frame + 0x1c, negative_z);
    w_u16(frame + 0x1a, 0u - local_y);
    uint32 stage = r_u8(state + 0x26);
    if (stage != 1)
    {
        uint32 x = (uint32)r_s16(frame + 0x20);
        uint32 square_x = x * x;
        uint32 y = (uint32)r_s16(frame + 0x22);
        uint32 square_y = y * y;
        uint32 radius = (uint32)r_s16(state + 0x12);
        bool near = (sint32)(square_x + square_y) < (sint32)(radius * radius);
        if (near)
        {
            uint32 depth = (negative_z + 600) & 0xffff;
            bool accepted = false;
            if (depth < 0xa29)
            {
                uint32 options = r_u16(object + 0x100);
                accepted = (options & 0x20) == 0;
            }
            if (!accepted && depth < 0x759)
            {
                uint32 options = r_u16(object + 0x100);
                accepted = (options & 0x20) != 0;
            }
            if (accepted)
            {
                uint32 options = r_u16(object + 0x100);
                sint32 side = r_s16(frame + 0x1a);
                bool reverse = (options & 2) != 0 ? side < 0 : side > 0;
                uint32 current_stage = r_u8(state + 0x26);
                placement = r_u32(object + 0x20);
                uint32 list = r_u32(placement + 4);
                if (list == 0)
                {
                    if (current_stage == 0)
                        w_u16(object + 0xfc, reverse ? 1 : 2);
                    w_u8(state + 0x26, 1);
                }
                else
                {
                    uint32 index = 1;
                    uint32 count = r_u32(list);
                    if ((sint32)index < (sint32)(count + 1))
                    {
                        do
                        {
                            list = r_u32(placement + 4);
                            uint32 record = r_u32(list + index * 4);
                            uint32 linked = r_u32(record + 0x20);
                            w_u8(linked + 0x122, 1);
                            if (current_stage == 0)
                            {
                                uint32 first = reverse ? 2 : 1;
                                uint32 second = reverse ? 1 : 2;
                                if (linked == object)
                                {
                                    options = r_u16(object + 0x100);
                                    w_u16(object + 0xfc, (options & 2) != 0 ? first : second);
                                }
                                else
                                {
                                    uint32 linked_options = r_u16(linked + 0x100);
                                    bool same = false;
                                    if ((linked_options & 4) != 0)
                                    {
                                        sint32 linked_signed = (sint16)linked_options;
                                        options = r_u16(object + 0x100);
                                        sint32 own_signed = (sint16)options;
                                        same = linked_signed % 2 == own_signed % 2;
                                    }
                                    if (same)
                                        w_u16(linked + 0xfc, (linked_options & 2) != 0 ? first : second);
                                    else
                                    {
                                        options = r_u16(linked + 0x100);
                                        w_u16(linked + 0xfc, (options & 2) != 0 ? second : first);
                                    }
                                }
                            }
                            placement = r_u32(object + 0x20);
                            list = r_u32(placement + 4);
                            count = r_u32(list);
                            ++index;
                        } while ((sint32)index < (sint32)(count + 1));
                    }
                }
            }
        }
        stage = r_u8(state + 0x26);
    }
    if (stage == 1)
    {
        placement = r_u32(object + 0x20);
        uint32 list = r_u32(placement + 4);
        if (list != 0)
        {
            uint32 options = r_u16(object + 0x100);
            sint32 x = r_s16(frame + 0x18);
            sint32 upper_x = r_s16(state + 6);
            if (x <= upper_x)
            {
                sint32 lower_x = r_s16(state + 8);
                if (x >= lower_x)
                {
                    bool within_y;
                    if ((options & 2) != 0)
                    {
                        sint32 span = r_s16(state + 0x10);
                        sint32 y = r_s16(frame + 0x1a);
                        within_y = y >= -span && y < span;
                    }
                    else
                    {
                        sint32 y = r_s16(frame + 0x1a);
                        sint32 span = r_s16(state + 0x10);
                        within_y = span < y && y <= -span;
                    }
                    if (within_y)
                    {
                        sint32 z = r_s16(frame + 0x1c);
                        sint32 upper_z = r_s16(state + 0xa);
                        if (z <= upper_z)
                        {
                            sint32 lower_z = r_s16(state + 0xc);
                            if (z >= lower_z)
                            {
                                flags = r_u8(state + 0x27);
                                w_u8(state + 0x27, flags | 1);
                            }
                        }
                    }
                }
            }
        }
        sint32 direction = r_s16(object + 0xfc);
        if (direction == 2)
        {
            uint32 z = r_u16(state + 0x18);
            uint32 y = r_u16(state + 0x16);
            uint32 x = r_u16(state + 0x14);
            G3DoorOpenPositive(object, x | (y << 16), z, frame);
        }
        else if (direction == 1)
        {
            uint32 z = r_u16(state + 0x1e);
            uint32 y = r_u16(state + 0x1c);
            uint32 x = r_u16(state + 0x1a);
            G3DoorOpenNegative(object, x | (y << 16), z, frame);
        }
    }
    else if (stage == 0)
    {
        sint32 direction = r_s16(object + 0xfc);
        if (direction == 2)
        {
            uint32 z = r_u16(state + 0x18);
            uint32 y = r_u16(state + 0x16);
            uint32 x = r_u16(state + 0x14);
            G3DoorClosePositive(object, x | (y << 16), z, frame);
        }
        else if (direction == 1)
        {
            uint32 z = r_u16(state + 0x1e);
            uint32 y = r_u16(state + 0x1c);
            uint32 x = r_u16(state + 0x1a);
            G3DoorCloseNegative(object, x | (y << 16), z, frame);
        }
        if (r_s16(object + 0x4c) != 0)
        {
            sint32 delay = r_s16(object + 0x4e);
            uint32 delay_half = r_u16(object + 0x4e);
            if (delay != 0)
                w_u16(object + 0x4e, delay_half - 1);
            else
                w_u16(object + 0x4c, 0);
        }
    }
    else
    {
        if (stage == 3)
        {
            uint32 x = (uint32)r_s16(frame + 0x20);
            uint32 square_x = x * x;
            uint32 y = (uint32)r_s16(frame + 0x22);
            uint32 square_y = y * y;
            uint32 radius = (uint32)r_s16(state + 0x12);
            uint32 square_radius = radius * radius;
            if ((sint32)(square_radius * 3) < (sint32)(square_x + square_y))
            {
                w_u8(state + 0x26, 0);
                placement = r_u32(object + 0x20);
                uint32 list = r_u32(placement + 4);
                if (list != 0)
                {
                    uint32 index = 1;
                    uint32 count = r_u32(list);
                    if ((sint32)index < (sint32)(count + 1))
                    {
                        do
                        {
                            list = r_u32(placement + 4);
                            uint32 record = r_u32(list + index * 4);
                            uint32 linked = r_u32(record + 0x20);
                            w_u8(linked + 0x122, 0);
                            w_u16(linked + 0x4c, 1);
                            uint32 random = G3SdkRandom80027FA8();
                            w_u16(linked + 0x4e, random & 7);
                            placement = r_u32(object + 0x20);
                            list = r_u32(placement + 4);
                            count = r_u32(list);
                            ++index;
                        } while ((sint32)index < (sint32)(count + 1));
                    }
                }
                goto Finish;
            }
        }
        if (r_u8(state + 0x26) == 4)
            w_u8(state + 0x26, 3);
    }
Finish:
    flags = r_u8(state + 0x27);
    w_u8(state + 0x27, (flags & 1) != 0 ? flags | 2 : flags & 0xfd);
}

uint32 G3EbfireEvent(uint32 object, uint32 message, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_12");
    uint32 frame = guest_stack - 0x18;
    if (message == 1)
    {
        G3EbfireInitialize(object, module_base, frame);
        return 1;
    }
    if (message == 2)
    {
        G3EbfireUpdate(object, frame);
        return 1;
    }
    return 0;
}

void G3EbfireInitialize(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000005c, "G3L_12");
    uint32 frame = guest_stack - 0x20;
    uint32 data = r_u32(object + 0x28);
    uint32 state = object + 0x104;
    if (data == 0)
    {
        uint32 resource = r_u32(object + 0x1c);
        data = r_u32(resource + 0x1c);
    }
    uint32 resource = G3Function8003AA48(module_base + 0x43c);
    w_u32(object + 0x104, resource);
    w_u32(state + 4, 0);
    if (data != 0)
    {
        uint32 radius = r_u32(data);
        w_u32(state + 8, radius * radius);
    }
    else
        w_u32(state + 8, 0x5f5e10);
    w_u32(state + 0xc, data != 0 ? r_u32(data + 4) : 500);
    w_u32(state + 0x10, data != 0 ? r_u32(data + 8) : 55);
    w_u32(state + 0x14, data != 0 ? r_u32(data + 0xc) : 32);
    (void)frame;
}

void G3EbfireUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000012c, "G3L_12");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    if (r_u32(state + 0x14) != 0)
        G3EbfireRotate(object, frame);
    uint32 distance = G3EbfireDistance(object);
    uint32 radius_squared = r_u32(state + 8);
    if ((sint32)distance < (sint32)radius_squared)
        G3EbfireSpawn(object, frame);
}

uint32 G3EbfireDistance(uint32 object)
{
    FUNCTION_MARKER(0x00000194, "G3L_12");
    uint32 player = r_u32(0x8009efc4);
    uint32 x = (uint32)r_s16(object + 0x58);
    uint32 px = (uint32)r_s16(player + 0x58);
    uint32 dx = x - px;
    uint32 xx = dx * dx;
    uint32 y = (uint32)r_s16(object + 0x5a);
    uint32 py = (uint32)r_s16(player + 0x5a);
    uint32 dy = y - py;
    uint32 yy = dy * dy;
    uint32 pz = (uint32)r_s16(player + 0x5c);
    uint32 z = (uint32)r_s16(object + 0x5c);
    uint32 dz = z - pz;
    return xx + yy + dz * dz;
}

void G3EbfireRotate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000001f0, "G3L_12");
    uint32 frame = guest_stack - 0x68;
    uint32 state = object + 0x104;
    if (r_u32(state + 0x14) == 0)
        return;
    G3Function8002F718(frame + 0x10);
    G3Function8002F718(frame + 0x30);
    G3Function8005BB6C(r_u32(state + 0x14), frame + 0x30, frame);
    G3Function8005BB6C((uint32)r_s16(object + 0x64), frame + 0x10, frame);
    G3Function8005BA5C((uint32)r_s16(object + 0x62), frame + 0x10, frame);
    G3Function8005B94C((uint32)r_s16(object + 0x60), frame + 0x10, frame);
    MulMatrix2((MATRIX *)psx_addr(frame + 0x10, 20), (MATRIX *)psx_addr(frame + 0x30, 20));
    G3Function800202A4(frame + 0x30, frame + 0x50, 0x15, frame);
    w_u16(object + 0x60, r_u16(frame + 0x50));
    w_u16(object + 0x62, r_u16(frame + 0x52));
    w_u16(object + 0x64, r_u16(frame + 0x54));
}

void G3EbfireSpawn(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000002b8, "G3L_12");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    uint32 previous = r_u32(state + 4);
    uint32 offset = r_u32(state + 0xc);
    uint32 side = previous == 0 ? 1 : 0;
    w_u32(state + 4, side);
    if (side == 0)
        offset = 0 - offset;
    uint32 speed = r_u32(state + 0x10);
    if (side == 0)
        speed = 0 - speed;
    w_u16(frame + 0x10, 0);
    w_u16(frame + 0x12, offset);
    w_u16(frame + 0x14, 100);
    G3EbfireVector(object, frame + 0x10, frame);
    w_u16(frame + 0x18, 0);
    w_u16(frame + 0x1a, speed);
    w_u16(frame + 0x1c, 0);
    G3EbfireVector(object, frame + 0x18, frame);
    uint32 resource = r_u32(object + 0x104);
    if (resource == 0)
        return;
    uint32 child = G3Function8002A454(object, resource, frame);
    if (child == 0)
        return;
    w_u16(child + 0x68, 0x1000);
    w_u16(child + 0x6a, 0x1000);
    w_u16(child + 0x6c, 0x1000);
    uint32 x = r_u16(child + 0x58);
    w_u32(child + 0x20, 0);
    w_u16(child + 0x58, x + r_u16(frame + 0x10));
    w_u16(child + 0x5a, r_u16(child + 0x5a) + r_u16(frame + 0x12));
    w_u16(child + 0x5c, r_u16(child + 0x5c) + r_u16(frame + 0x14));
    w_u32(child + 0x104, (uint32)r_s16(frame + 0x18));
    w_u32(child + 0x108, (uint32)r_s16(frame + 0x1a));
    w_u32(child + 0x10c, (uint32)r_s16(frame + 0x1c));
}

void G3EbfireVector(uint32 object, uint32 vector, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000003d0, "G3L_12");
    uint32 frame = guest_stack - 0x48;
    G3Function8005BD00(object + 0x60, frame + 0x18);
    ApplyMatrixSV((MATRIX *)psx_addr(frame + 0x18, 20), (SVECTOR *)psx_addr(vector, 8), (SVECTOR *)psx_addr(frame + 0x10, 6));
    w_u16(vector, r_u16(frame + 0x10));
    w_u16(vector + 2, r_u16(frame + 0x12));
    w_u16(vector + 4, r_u16(frame + 0x14));
}

uint32 G3EgenEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_16");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3EgenInitialize(object, game, module_base, frame);
        return 1;
    case 2:
        G3EgenUpdate(object, game, frame);
        return 1;
    case 9:
        G3EgenCommand(object, payload, module_base, frame);
        return 1;
    case 16:
        G3EgenChildReleased(object);
        return 1;
    default:
        return 0;
    }
}

void G3EgenConfigure(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000a0, "G3L_16");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    uint32 value = G3Function80085AB0(commands, module_base + 0x50c, frame);
    if (value != 0)
    {
        uint32 resource = G3Function8003AA48(value);
        w_u32(object + 0x104, resource);
    }
    value = G3Function80085AB0(commands, module_base + 0x518, frame);
    if (value != 0)
        w_u16(state + 4, r_u16(value));
    value = G3Function80085AB0(commands, module_base + 0x524, frame);
    if (value != 0)
        w_u16(state + 8, r_u16(value));
    value = G3Function80085AB0(commands, module_base + 0x530, frame);
    if (value != 0)
        w_u16(state + 0xe, r_u16(value));
    if (G3Function80085AB0(commands, module_base + 0x53c, frame) != 0)
        G3Function80038798(object, 0, 0, frame);
}

void G3EgenInitialize(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000180, "G3L_16");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    w_u16(state + 8, 6000);
    w_u16(state + 4, 150);
    w_u16(state + 0xe, 3);
    w_u16(frame + 0x10, r_u16(object + 0x58));
    w_u16(frame + 0x12, r_u16(object + 0x5a));
    w_u16(frame + 0x14, r_u16(object + 0x5c));
    w_u16(frame + 0x18, r_u16(object + 0x58));
    w_u16(frame + 0x1a, r_u16(object + 0x5a));
    w_u16(frame + 0x1c, r_u16(object + 0x5c) - 2000);
    uint32 level = r_u32(game + 0x48);
    uint32 terrain = r_u32(level);
    if (G3Function800622EC(terrain, frame + 0x18, frame + 0x10, 0, frame) != 0)
        w_u16(state + 0xa, r_u16(frame + 0x1c));
    else
        w_u16(state + 0xa, r_u16(object + 0x8c) - 0x200);
    w_u16(state + 0xc, r_u16(object + 0x8c));
    w_u16(state + 0xa, r_u16(state + 0xa) - 0x80);
    uint32 commands = r_u32(object + 0x24);
    if (commands != 0)
        G3EgenConfigure(object, commands, module_base, frame);
    commands = r_u32(object + 0x28);
    if (commands != 0)
        G3EgenConfigure(object, commands, module_base, frame);
    uint32 current_game = r_u32(0x80097048);
    G3Function80082988(object, state + 0x18, current_game, frame);
    if (r_u32(state) == 0)
        G3Function8002A9AC(object, frame);
}

void G3EgenUpdate(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000002c0, "G3L_16");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    G3Function8003C8C0(object, object + 0x11c, game, frame);
    uint32 player = r_u32(game + 0x50);
    uint32 xy = r_u32(player + 0x58);
    uint32 z = (uint32)r_s16(player + 0x5c);
    w_u32(frame + 0x18, xy);
    w_u16(frame + 0x1c, z);
    uint32 x = r_u16(object + 0x88);
    uint32 px = r_u16(frame + 0x18);
    uint32 dx = x - px;
    sint32 signed_dx = (sint32)(dx << 16) >> 16;
    uint32 xx = (uint32)(signed_dx * signed_dx);
    w_u16(frame + 0x10, dx);
    uint32 y = r_u16(object + 0x8a);
    uint32 py = r_u16(frame + 0x1a);
    uint32 dy = y - py;
    sint32 signed_dy = (sint32)(dy << 16) >> 16;
    uint32 yy = (uint32)(signed_dy * signed_dy);
    w_u16(frame + 0x12, dy);
    uint32 player_z = r_u16(frame + 0x1c);
    z = r_u16(object + 0x8c);
    w_u16(frame + 0x14, z - player_z);
    sint32 radius = r_s16(state + 8);
    sint32 radius_squared = radius * radius;
    sint32 distance_squared = (sint32)(xx + yy);
    sint32 height = (sint32)(player_z << 16) >> 16;
    bool inside = false;
    if (radius_squared >= distance_squared)
    {
        sint32 lower = r_s16(state + 0xa);
        if (height >= lower)
        {
            sint32 upper = r_s16(state + 0xc);
            if (height <= upper)
                inside = true;
        }
    }
    if (inside)
        w_u32(state + 0x14, 1);
    else
    {
        radius = r_s16(state + 8) + 0x100;
        sint32 expanded_squared = radius * radius;
        bool outside = expanded_squared < distance_squared;
        if (!outside)
        {
            height = r_s16(frame + 0x1c);
            sint32 lower = r_s16(state + 0xa);
            outside = height < lower;
            if (!outside)
                outside = r_s16(state + 0xc) < height;
        }
        if (outside)
            w_u32(state + 0x14, 0);
    }
    bool immediate_tick = false;
    if (r_u32(state + 0x14) != 0)
    {
        sint32 children = r_s16(state + 0x10);
        sint32 maximum = r_s16(state + 0xe);
        if (children < maximum)
        {
            if (r_s16(state + 6) > 0)
                immediate_tick = true;
            else
            {
                uint32 resource = r_u32(state);
                uint32 interval = r_u16(state + 4);
                w_u16(state + 6, interval);
                if (G3Function8002A454(object, resource, frame) != 0)
                    w_u16(state + 0x10, r_u16(state + 0x10) + 1);
            }
        }
    }
    if (immediate_tick || r_s16(state + 6) > 0)
        w_u16(state + 6, r_u16(state + 6) - 1);
    sint32 children = r_s16(state + 0x10);
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x10, children != 0 ? flags | 0x400 : flags & 0xfffffbff);
}

void G3EgenChildReleased(uint32 object)
{
    FUNCTION_MARKER(0x000004cc, "G3L_16");
    uint32 state = object + 0x104;
    w_u16(state + 0x10, r_u16(state + 0x10) - 1);
}

void G3EgenCommand(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000004e4, "G3L_16");
    uint32 frame = guest_stack - 0x18;
    if (commands != 0)
        G3EgenConfigure(object, commands, module_base, frame);
}

uint32 G3FallgenEvent(uint32 object, uint32 game, uint32 message, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_64");
    uint32 frame = guest_stack - 0x18;
    if (message == 1)
    {
        G3FallgenInitialize(object, frame);
        return 1;
    }
    if (message == 2)
    {
        G3FallgenUpdate(object, game, frame);
        return 1;
    }
    return 0;
}

void G3FallgenInitialize(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000005c, "G3L_64");
    uint32 frame = guest_stack - 0x20;
    uint32 data = r_u32(object + 0x28);
    uint32 state = object + 0x128;
    uint32 total = 0;
    if (data != 0)
    {
        for (sint32 index = 0; index < r_s16(data + 0xc); ++index)
        {
            uint32 table = r_u32(data + 0x10);
            uint32 row = table + ((uint32)index << 3);
            total += (uint32)r_s16(row + 4);
            w_u16(row + 6, total);
        }
        w_u32(state + 4, total);
        sint32 upper = r_s16(data + 0xa);
        sint32 lower = r_s16(data + 8);
        if (upper != lower)
        {
            sint32 random = (sint32)G3SdkRandom80027FA8();
            lower = r_s16(data + 8);
            upper = r_s16(data + 0xa);
            sint32 range = upper - lower;
            sint32 remainder = range == 0 ? random : random % range;
            w_u32(state, (uint32)lower + (uint32)remainder);
        }
        else
            w_u32(state, (uint32)upper);
    }
    w_u32(object + 0x10, r_u32(object + 0x10) | 0x800);
    (void)frame;
}

uint32 G3FallgenSpawn(uint32 object, uint32 resource, uint32 player, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000140, "G3L_64");
    uint32 frame = guest_stack - 0x28;
    uint32 data = r_u32(object + 0x28);
    uint32 child;
    if (r_s16(data + 0xe) != 0)
    {
        child = G3Function8002A454(object, resource, frame);
        if (child == 0)
            return 0;
        uint32 height = r_u16(child + 0x5c);
        uint32 offset = r_u16(data);
        w_u16(child + 0x5c, height + offset);
    }
    else
    {
        uint32 x = (uint32)r_s16(player + 0x58);
        uint32 old_x = (uint32)r_s16(player + 0x70);
        uint32 dx = x - old_x;
        w_u32(frame + 0x10, dx);
        uint32 y = (uint32)r_s16(player + 0x5a);
        uint32 old_y = (uint32)r_s16(player + 0x72);
        uint32 dy = y - old_y;
        w_u32(frame + 0x14, dy);
        uint32 z = (uint32)r_s16(player + 0x5c);
        uint32 old_z = (uint32)r_s16(player + 0x74);
        w_u32(frame + 0x18, z - old_z);
        uint32 gain = (uint32)r_s16(data + 2);
        dx *= gain;
        w_u32(frame + 0x10, dx);
        gain = (uint32)r_s16(data + 2);
        dy *= gain;
        w_u32(frame + 0x18, 0);
        w_u32(frame + 0x14, dy);
        dx += (uint32)r_s16(player + 0x58);
        w_u32(frame + 0x10, dx);
        dy += (uint32)r_s16(player + 0x5a);
        w_u32(frame + 0x14, dy);
        z = (uint32)r_s16(player + 0x5c);
        w_u32(frame + 0x18, z);
        uint32 offset = (uint32)r_s16(data);
        w_u32(frame + 0x18, z + offset);
        child = G3Function8002A454(object, resource, frame);
        if (child == 0)
            return 0;
        w_u16(child + 0x58, r_u16(frame + 0x10));
        w_u16(child + 0x5a, r_u16(frame + 0x14));
        w_u16(child + 0x5c, r_u16(frame + 0x18));
    }
    w_u16(child + 0x14c, r_u16(data));
    return child;
}

void G3FallgenUpdate(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000288, "G3L_64");
    uint32 frame = guest_stack - 0x48;
    uint32 data = r_u32(object + 0x28);
    uint32 state = object + 0x128;
    if (data == 0)
        return;
    if (r_s32(state + 8) < 0)
        w_u32(object + 0x128, r_u32(object + 0x128) - 1);
    uint32 player = r_u32(game + 0x50);
    uint32 x = (uint32)r_s16(object + 0x58);
    uint32 px = (uint32)r_s16(player + 0x58);
    uint32 dx = x - px;
    uint32 xx = dx * dx;
    w_u32(frame + 0x10, dx);
    player = r_u32(game + 0x50);
    uint32 y = (uint32)r_s16(object + 0x5a);
    uint32 py = (uint32)r_s16(player + 0x5a);
    uint32 dy = y - py;
    uint32 yy = dy * dy;
    w_u32(frame + 0x14, dy);
    player = r_u32(game + 0x50);
    uint32 pz = (uint32)r_s16(player + 0x5c);
    uint32 z = (uint32)r_s16(object + 0x5c);
    uint32 dz = z - pz;
    uint32 zz = dz * dz;
    w_u32(frame + 0x18, dz);
    sint32 radius_squared = r_s32(data + 4);
    if ((sint32)(xx + yy + zz) < radius_squared && r_s32(object + 0x128) < 0)
    {
        uint32 cooldown = r_u32(state + 8);
        if ((sint32)cooldown >= 0)
        {
            w_u32(state + 8, cooldown - 1);
            return;
        }
        player = r_u32(game + 0x50);
        if (r_u32(player + 0x154) != 0x40000000)
        {
            sint32 random = (sint32)G3SdkRandom80027FA8();
            sint32 total = r_s32(state + 4);
            sint32 count = r_s16(data + 0xc);
            sint32 choice = total == 0 ? random : random % total;
            if (count > 0)
            {
                for (sint32 index = 0; ; ++index)
                {
                    uint32 table = r_u32(data + 0x10);
                    uint32 row = table + ((uint32)index << 3);
                    bool selected = choice < r_s16(row + 6);
                    if (selected)
                    {
                        random = (sint32)G3SdkRandom80027FA8();
                        sint32 lower = r_s16(data + 8);
                        sint32 upper = r_s16(data + 0xa);
                        sint32 range = upper - lower;
                        sint32 remainder = range == 0 ? random : random % range;
                        w_u32(state, (uint32)lower + (uint32)remainder);
                        table = r_u32(data + 0x10);
                        player = r_u32(game + 0x50);
                        uint32 resource = r_u32(table + ((uint32)index << 3));
                        G3FallgenSpawn(object, resource, player, frame);
                        G3Function800499E0(4, 1);
                    }
                    count = r_s16(data + 0xc);
                    if (index + 1 >= count || selected)
                        break;
                }
            }
        }
    }
    w_u32(state + 8, r_u32(state + 8) - 1);
}
