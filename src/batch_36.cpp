#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32 G3AnimatedBombEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_75");
    uint32 frame = guest_stack - 0x18;
    uint32 state = object + 0x128;
    if (message == 1)
    {
        G3AnimatedBombInitialize(object, module_base, frame);
        return 1;
    }
    uint32 target;
    if (message == 2)
        target = r_u32(state + 4);
    else if (message == 3)
        target = r_u32(state + 8);
    else if (message == 8)
        target = r_u32(state + 0xc);
    else
        return 0;
    if (target == 0)
        return 1;
    switch (target - module_base)
    {
    case 0x2e4:
        G3AnimatedBombUpdate(object, module_base, frame);
        break;
    case 0x440:
        G3AnimatedBombCollide(object, game, module_base, frame);
        break;
    case 0x640:
        G3AnimatedBombDamage(object, module_base, frame);
        break;
    case 0x660:
        G3AnimatedBombParticleUpdate(object, frame);
        break;
    case 0x71c:
        G3AnimatedBombParticleCollide(object, game, frame);
        break;
    case 0x77c:
        G3AnimatedBombFragmentUpdate(object, frame);
        break;
    case 0x864:
        G3AnimatedBombFragmentCollide(object, game, frame);
        break;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved anibomb callback target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
    return 1;
}

void G3AnimatedBombInitialize(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000ac, "G3L_75");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x128;
    uint32 resource = r_u32(object + 0x1c);
    uint32 data = r_u32(resource + 0x1c);
    if (data != 0)
        w_u32(state, r_u32(data));
    else
        w_u32(state, 0);
    uint32 type = r_u32(state);
    if (type == 0)
    {
        w_u32(object + 0x150, 0);
        w_u32(object + 0x154, 72);
        uint32 random = G3AnimatedBombRandom(256);
        w_u32(object + 0x110, (uint32)(sint32)(sint16)random);
        random = G3AnimatedBombRandom(256);
        w_u32(object + 0x114, (uint32)(sint32)(sint16)random);
        w_u32(object + 0x118, 0xfffffff6);
        w_u32(state + 4, module_base + 0x2e4);
        w_u32(state + 8, module_base + 0x440);
        w_u32(state + 0xc, module_base + 0x640);
    }
    else if (type == 1)
    {
        w_u32(object + 0x150, 7);
        w_u32(object + 0x154, 1);
        w_u32(state + 4, module_base + 0x660);
        w_u32(state + 8, module_base + 0x71c);
        w_u32(state + 0xc, 0);
    }
    else if (type == 2)
    {
        w_u32(object + 0x150, 72);
        w_u32(object + 0x154, 8);
        w_u32(state + 4, module_base + 0x77c);
        w_u32(state + 8, module_base + 0x864);
        w_u32(state + 0xc, 0);
    }
}

uint32 G3AnimatedBombRandom(uint32 bound)
{
    FUNCTION_MARKER(0x000001e4, "G3L_75");
    sint32 range = (sint16)bound;
    if (range <= 0)
        return 0;
    sint32 random = (sint32)G3SdkRandom80027FA8();
    uint32 value = (uint32)(random % (range * 2)) - bound;
    return (uint32)(sint32)(sint16)value;
}

void G3AnimatedBombBurst(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000244, "G3L_75");
    uint32 frame = guest_stack - 0x18;
    uint32 resource = G3Function8003AA48(module_base + 0x8e0);
    uint32 z = r_u16(object + 0x5c);
    w_u16(object + 0x5c, z + 120);
    if (resource != 0)
    {
        uint32 models = r_u32(resource + 0xc);
        uint32 model = r_u32(models);
        uint32 metadata = r_u32(model + 0x1c);
        uint32 first = r_u32(metadata + 8);
        uint32 second = r_u32(metadata + 0xc);
        uint32 product = first * second;
        uint32 pool = r_u32(0x800a27fc);
        uint32 count = (uint32)(sint32)(sint16)product;
        G3Function800444E4(object + 0x58, model, pool, count, frame);
    }
    z = r_u16(object + 0x5c);
    w_u16(object + 0x5c, z - 120);
    G3Function80038798(object, 0, 0, frame);
}

void G3AnimatedBombUpdate(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000002e4, "G3L_75");
    uint32 frame = guest_stack - 0x18;
    uint32 mode = r_u32(object + 0x150);
    if (mode == 0)
    {
        uint32 x = r_u16(object + 0x58);
        uint32 vx = r_u16(object + 0x104);
        uint32 velocity_z = r_u32(object + 0x10c);
        uint32 vy = r_u16(object + 0x108);
        uint32 vz = r_u16(object + 0x10c);
        w_u16(object + 0x58, x + vx);
        uint32 y = r_u16(object + 0x5a);
        uint32 z = r_u16(object + 0x5c);
        w_u16(object + 0x5a, y + vy);
        uint32 rx = r_u16(object + 0x60);
        uint32 vrx = r_u16(object + 0x110);
        w_u16(object + 0x5c, z + vz);
        uint32 ry = r_u16(object + 0x62);
        uint32 vry = r_u16(object + 0x114);
        w_u16(object + 0x60, rx + vrx);
        w_u16(object + 0x62, ry + vry);
        if ((sint32)velocity_z >= -399)
            velocity_z += r_u32(object + 0x118);
        uint32 lifetime = r_u32(object + 0x154);
        w_u32(object + 0x10c, velocity_z);
        if ((sint32)lifetime > 0)
            w_u32(object + 0x154, lifetime - 1);
        else
            G3AnimatedBombBurst(object, module_base, frame);
    }
    else if (mode == 1)
    {
        uint32 lifetime = r_u32(object + 0x154);
        if ((sint32)lifetime > 0)
            w_u32(object + 0x154, lifetime - 1);
        else
        {
            w_u32(object + 0x150, 2);
            w_u32(object + 0x154, 20);
        }
    }
    else if (mode == 2)
    {
        uint32 z = r_u16(object + 0x5c);
        uint32 low_lifetime = r_u16(object + 0x154);
        uint32 lifetime = r_u32(object + 0x154);
        z += 20 - low_lifetime;
        uint32 current_lifetime = r_u32(object + 0x154);
        w_u16(object + 0x5c, z);
        uint32 rotation = (20 - current_lifetime) * 40;
        w_u16(object + 0x64, r_u16(object + 0x64) + rotation);
        if ((sint32)lifetime > 0)
            w_u32(object + 0x154, lifetime - 1);
        else
            G3AnimatedBombBurst(object, module_base, frame);
    }
}

void G3AnimatedBombCollide(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000440, "G3L_75");
    uint32 frame = guest_stack - 0x40;
    uint32 contact = r_u32(object + 0xb0);
    uint32 player = r_u32(0x8009efc4);
    uint32 other = r_u32(contact + 0x14);
    if (other == player)
    {
        G3AnimatedBombBurst(object, module_base, frame);
        player = r_u32(0x8009efc4);
        w_u32(frame + 0x10, 1);
        w_u16(frame + 0x20, 0);
        w_u16(frame + 0x22, 0);
        w_u32(frame + 0x24, contact);
        uint32 target = r_u32(player + 0xec);
        if (target != 0)
            G3DispatchStateEvent(target, player, game, 8, frame + 0x10, frame);
        return;
    }
    if (r_s16(contact + 6) != 3 || r_u32(object + 0x150) != 0)
        return;
    uint32 velocity_z = r_u32(object + 0x10c);
    if (velocity_z + 24 < 49)
    {
        sint32 rx = r_s16(object + 0x60);
        w_u32(object + 0x150, 1);
        w_u32(object + 0x104, 0);
        w_u32(object + 0x108, 0);
        w_u32(object + 0x10c, 0);
        w_u32(object + 0x110, 0);
        w_u32(object + 0x114, 0);
        w_u32(object + 0x118, 0);
        w_u32(object + 0x124, 0);
        w_u32(object + 0x154, 12);
        sint32 ry = r_s16(object + 0x62);
        w_u16(object + 0x60, (uint32)(rx % 4096));
        w_u16(object + 0x62, (uint32)(ry % 4096));
        return;
    }
    uint32 magnitude = r_u32(object + 0x124);
    uint32 half = (uint32)((sint32)(magnitude + (magnitude >> 31)) >> 1);
    sint32 random = (sint32)G3SdkRandom80027FA8();
    uint32 angle = (uint32)(random % 4096);
    uint32 product = half * G3Function8005B470(angle);
    if ((sint32)product < 0)
        product += 0xfff;
    w_u32(object + 0x104, (uint32)((sint32)product >> 12));
    product = half * G3Function8005B590(angle);
    if ((sint32)product < 0)
        product += 0xfff;
    uint32 bound = (uint32)((sint32)(half << 19) >> 16);
    velocity_z = r_u32(object + 0x10c);
    w_u32(object + 0x108, (uint32)((sint32)product >> 12));
    uint32 vertical_half = (uint32)((sint32)(velocity_z + (velocity_z >> 31)) >> 1);
    w_u32(object + 0x10c, 0 - vertical_half);
    uint32 spin = G3AnimatedBombRandom(bound);
    w_u32(object + 0x110, (uint32)(sint32)(sint16)spin);
    spin = G3AnimatedBombRandom(bound);
    w_u32(object + 0x114, (uint32)(sint32)(sint16)spin);
    w_u32(object + 0x124, half);
}

void G3AnimatedBombDamage(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000640, "G3L_75");
    uint32 frame = guest_stack - 0x18;
    G3AnimatedBombBurst(object, module_base, frame);
}

void G3AnimatedBombParticleUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000660, "G3L_75");
    uint32 frame = guest_stack - 0x18;
    uint32 mode = r_u32(object + 0x150);
    uint32 scale_step = (sint32)mode > 0 ? 256 : 1024;
    if ((sint32)mode > 0)
    {
        w_u32(object + 0x150, mode - 1);
        uint32 x = r_u16(object + 0x58);
        uint32 vx = r_u16(object + 0x104);
        uint32 y = r_u16(object + 0x5a);
        uint32 vy = r_u16(object + 0x108);
        w_u16(object + 0x58, x + vx);
        uint32 z = r_u16(object + 0x5c);
        uint32 vz = r_u16(object + 0x10c);
        w_u16(object + 0x5a, y + vy);
        uint32 sx = r_u16(object + 0x68);
        w_u16(object + 0x5c, z + vz);
        uint32 sy = r_u16(object + 0x6a);
        w_u16(object + 0x68, sx + scale_step);
        uint32 sz = r_u16(object + 0x6c);
        w_u16(object + 0x6a, sy + scale_step);
        w_u16(object + 0x6c, sz + scale_step);
    }
    else
    {
        uint32 lifetime = r_u32(object + 0x154);
        if ((sint32)lifetime > 0)
            w_u32(object + 0x154, lifetime - 1);
        else
            G3Function8002A9AC(object, frame);
    }
}

void G3AnimatedBombParticleCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000071c, "G3L_75");
    uint32 frame = guest_stack - 0x30;
    uint32 contact = r_u32(object + 0xb0);
    uint32 player = r_u32(0x8009efc4);
    uint32 other = r_u32(contact + 0x14);
    if (other != player)
        return;
    w_u32(frame + 0x10, 1);
    w_u16(frame + 0x20, 7);
    w_u16(frame + 0x22, 0);
    w_u32(frame + 0x24, contact);
    uint32 target = r_u32(other + 0xec);
    if (target != 0)
        G3DispatchStateEvent(target, other, game, 8, frame + 0x10, frame);
}

void G3AnimatedBombFragmentUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000077c, "G3L_75");
    uint32 frame = guest_stack - 0x18;
    G3Function80029F0C(object, 0xffffffff, frame);
    uint32 mode = r_u32(object + 0x150);
    if ((sint32)mode > 0)
    {
        uint32 next = mode - 1;
        w_u32(object + 0x150, next);
        if (next == 0)
        {
            G3Function8002A9AC(object, frame);
            return;
        }
        uint32 vx = r_u32(object + 0x104);
        uint32 ax = r_u32(object + 0x110);
        uint32 vy = r_u32(object + 0x108);
        uint32 ay = r_u32(object + 0x114);
        w_u32(object + 0x104, vx + ax);
        uint32 vz = r_u32(object + 0x10c);
        uint32 az = r_u32(object + 0x118);
        w_u32(object + 0x108, vy + ay);
        uint32 x = r_u16(object + 0x58);
        vx = r_u16(object + 0x104);
        w_u32(object + 0x10c, vz + az);
        uint32 y = r_u16(object + 0x5a);
        vy = r_u16(object + 0x108);
        w_u16(object + 0x58, x + vx);
        uint32 z = r_u16(object + 0x5c);
        vz = r_u16(object + 0x10c);
        w_u16(object + 0x5a, y + vy);
        w_u16(object + 0x5c, z + vz);
    }
    else
    {
        uint32 next = r_u32(object + 0x154) - 1;
        w_u32(object + 0x154, next);
        if (next == 0)
        {
            G3Function8002A9AC(object, frame);
            return;
        }
        uint32 x = r_u16(object + 0x68);
        uint32 z = r_u16(object + 0x6c);
        w_u16(object + 0x68, x + 1024);
        uint32 y = r_u16(object + 0x6a);
        w_u16(object + 0x6c, z + 1024);
        w_u16(object + 0x6a, y + 1024);
    }
}

void G3AnimatedBombFragmentCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000864, "G3L_75");
    uint32 frame = guest_stack - 0x30;
    uint32 contact = r_u32(object + 0xb0);
    uint32 player = r_u32(0x8009efc4);
    uint32 other = r_u32(contact + 0x14);
    if (other == player)
    {
        w_u32(frame + 0x10, 1);
        w_u16(frame + 0x20, 7);
        w_u16(frame + 0x22, 0);
        w_u32(frame + 0x24, contact);
        uint32 target = r_u32(other + 0xec);
        if (target != 0)
            G3DispatchStateEvent(target, other, game, 8, frame + 0x10, frame);
    }
    if (r_s32(object + 0x150) > 0)
        w_u32(object + 0x150, 0);
}

uint32 G3AniguntEvent(uint32 object, uint32 message, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_06");
    uint32 frame = guest_stack - 0x18;
    uint32 state = object + 0x104;
    switch (message)
    {
    case 1:
        G3AniguntInitialize(object, module_base);
        return 1;
    case 2:
    {
        uint32 kind = r_u32(state + 0x18);
        if (kind == 0)
            G3AniguntUpdate(object, frame);
        else if (kind == 1)
            G3AniguntUpdateReversible(object, frame);
        return 1;
    }
    case 3:
        if (r_u32(state + 0x18) == 1)
            G3AniguntCollide(object, frame);
        return 1;
    default:
        return 0;
    }
}

void G3AniguntSetAnimation(uint32 object, uint32 animation)
{
    FUNCTION_MARKER(0x000000c4, "G3L_06");
    uint32 flags = r_u32(object + 0x14);
    w_u16(object + 0x6e, animation);
    w_u16(object + 0x5e, 0);
    w_u32(object + 0x14, flags & 0xffffffef);
}

void G3AniguntInitialize(uint32 object, uint32 module_base)
{
    FUNCTION_MARKER(0x000000e0, "G3L_06");
    uint32 state = object + 0x104;
    uint32 custom = r_u32(object + 0x28);
    w_u32(object + 0x150, 0);
    G3AniguntSetAnimation(object, 0);
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        for (uint32 side = 0; side != 2; ++side)
        {
            uint32 value;
            if (custom != 0)
                value = r_u16(custom + axis * 2 + side * 6);
            else
            {
                value = r_u16(object + 0x58 + axis * 2);
                if (axis != 2)
                    value += side == 0 ? 0xfffff510 : 0xaf0;
                else if (side != 0)
                    value += 0x578;
            }
            w_u16(state + axis * 2 + side * 6, value);
        }
    }
    uint32 burst_resource = G3Function8003AA48(module_base + 0xdfc);
    w_u32(state + 0xc, burst_resource);
    uint32 bomb_resource = G3Function8003AA48(module_base + 0xe08);
    w_u32(state + 0x10, bomb_resource);
    w_u32(state + 0x14, 0);
    w_u32(state + 0x18, 0);
    uint32 resource = r_u32(object + 0x1c);
    uint32 name = r_u32(resource + 0x24);
    uint32 letter = r_u8(name + 6);
    if (letter != 't' && letter == 'f')
        w_u32(state + 0x18, 1);
}

void G3AniguntUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000598, "G3L_06");
    uint32 frame = guest_stack - 0x18;
    if (r_u32(object + 0x150) == 0)
        G3AniguntUpdateIdle(object, frame);
    if (r_u32(object + 0x150) == 1)
        G3AniguntUpdateActive(object, frame);
}

uint32 G3AniguntPlayerBelow(uint32 object)
{
    FUNCTION_MARKER(0x000005e8, "G3L_06");
    uint32 player = r_u32(0x8009efc4);
    sint32 object_z = r_s16(object + 0x5c);
    sint32 player_z = r_s16(player + 0x5c);
    return player_z < object_z - 200 ? 1 : 0;
}

uint32 G3AniguntContainsXY(uint32 object, uint32 point)
{
    FUNCTION_MARKER(0x00000604, "G3L_06");
    uint32 state = object + 0x104;
    sint32 x = r_s16(point);
    sint32 lower_x = r_s16(state);
    if (x <= lower_x)
        return 0;
    sint32 upper_x = r_s16(state + 6);
    if (x >= upper_x)
        return 0;
    sint32 y = r_s16(point + 2);
    sint32 lower_y = r_s16(state + 2);
    if (y <= lower_y)
        return 0;
    sint32 upper_y = r_s16(state + 8);
    return y < upper_y ? 1 : 0;
}

void G3AniguntUpdateReversible(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000009c8, "G3L_06");
    uint32 frame = guest_stack - 0x18;
    uint32 player = r_u32(0x8009efc4);
    if (G3AniguntContainsXY(object, player + 0x58) != 0)
        G3AniguntAnimateInside(object, frame);
    else
        G3AniguntAnimateOutside(object, frame);
}

void G3AniguntCollide(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000b88, "G3L_06");
    uint32 frame = guest_stack - 0x30;
    uint32 contact = r_u32(object + 0xb0);
    uint32 player = r_u32(0x8009efc4);
    uint32 other = r_u32(contact + 0x14);
    if (other != player)
        return;
    w_u32(frame + 0x10, 1);
    w_u16(frame + 0x20, 7);
    w_u16(frame + 0x22, 0);
    w_u32(frame + 0x24, contact);
    uint32 callback = r_u32(other + 0xec);
    if (callback != 0)
    {
        uint32 game = r_u32(0x80097048);
        G3DispatchStateEvent(callback, other, game, 8, frame + 0x10, frame);
    }
}

void G3AniguntUpdateIdle(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000024c, "G3L_06");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    uint32 player = r_u32(0x8009efc4);
    uint32 inside = G3AniguntContainsXY(object, player + 0x58);
    uint32 below = G3AniguntPlayerBelow(object);
    sint32 timer = r_s32(state + 0x14);
    if (timer > 0)
        w_u32(state + 0x14, (uint32)timer - 1);
    if (below != 0 && inside != 0)
    {
        w_u32(object + 0x150, 1);
        G3AniguntSetAnimation(object, 3);
        w_u32(state + 0x14, 0);
        return;
    }
    uint32 animation = r_u16(object + 0x6e);
    switch (animation)
    {
    case 0:
        if (inside == 0 && r_s16(object + 0x5e) == 0)
            return;
        if (G3Function80029F0C(object, 0xffffffff, frame) != 0)
        {
            G3AniguntSetAnimation(object, 1);
            w_u32(state + 0x14, 0);
        }
        return;
    case 1:
        if (G3Function80029F0C(object, 0xffffffff, frame) != 0)
        {
            G3AniguntSetAnimation(object, inside == 0 ? 2 : 1);
            return;
        }
        if (r_u32(state + 0x14) == 0)
        {
            uint32 resource = r_u32(object + 0x1c);
            uint32 current_animation = r_u16(object + 0x6e);
            uint32 animations = r_u32(resource + 0x10);
            uint32 data = r_u32(animations + current_animation * 4);
            uint32 numerator = (uint32)r_s16(object + 0x5e) << 12;
            sint32 divisor = r_s16(data + 2);
            uint32 phase = divisor == 0 ? ((sint32)numerator < 0 ? 1 : 0xffffffff) : (uint32)((sint64)(sint32)numerator / divisor);
            G3AniguntShootSpread(object, (uint32)(sint32)(sint16)phase, frame);
            w_u32(state + 0x14, 2);
        }
        return;
    case 2:
    case 7:
        if (G3Function80029F0C(object, 0xffffffff, frame) != 0)
            G3AniguntSetAnimation(object, 0);
        return;
    default:
        return;
    }
}

void G3AniguntUpdateActive(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000003f4, "G3L_06");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    uint32 player = r_u32(0x8009efc4);
    uint32 inside = G3AniguntContainsXY(object, player + 0x58);
    uint32 below = G3AniguntPlayerBelow(object);
    if (below == 0 && inside != 0)
    {
        w_u32(object + 0x150, 0);
        G3AniguntSetAnimation(object, 7);
        w_u32(state + 0x14, 0);
        return;
    }
    uint32 animation = r_u16(object + 0x6e);
    switch (animation)
    {
    case 3:
        if (G3Function80029F0C(object, 0xffffffff, frame) != 0)
            G3AniguntSetAnimation(object, 4);
        return;
    case 4:
    {
        uint32 timer = r_u32(state + 0x14);
        if (timer != 0)
        {
            w_u32(state + 0x14, timer - 1);
            return;
        }
        if (inside == 0 && r_s16(object + 0x5e) == 0)
            return;
        if (G3Function80029F0C(object, 0xffffffff, frame) != 0)
        {
            G3AniguntSetAnimation(object, 5);
            w_u32(state + 0x14, 3);
        }
        return;
    }
    case 5:
    {
        if (r_s16(object + 0x5e) == 1)
        {
            uint32 offset = (uint32)r_s16(state + 0x14);
            G3AniguntShootBomb(object, offset, frame);
        }
        if (G3Function80029F0C(object, 0xffffffff, frame) == 0)
            return;
        uint32 timer = r_u32(state + 0x14) - 1;
        w_u32(state + 0x14, timer);
        G3AniguntSetAnimation(object, inside != 0 && timer != 0 ? 5 : 6);
        return;
    }
    case 6:
        if (G3Function80029F0C(object, 0xffffffff, frame) != 0)
        {
            G3AniguntSetAnimation(object, 4);
            w_u32(state + 0x14, 0x30);
        }
        return;
    default:
        return;
    }
}

void G3AniguntAnimateInside(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000a18, "G3L_06");
    uint32 frame = guest_stack - 0x18;
    uint32 resource = r_u32(object + 0x1c);
    uint32 animation = r_u16(object + 0x6e);
    uint32 animations = r_u32(resource + 0x10);
    uint32 data = r_u32(animations + animation * 4);
    sint32 count = r_s16(data + 2);
    sint32 last = count - 1;
    if (animation == 0)
    {
        sint32 current = r_s16(object + 0x5e);
        uint32 current_half = r_u16(object + 0x5e);
        if (current < last)
            w_u16(object + 0x5e, current_half + 1);
        resource = r_u32(object + 0x1c);
        if ((r_u32(resource + 0x30) & 0x2000) != 0)
            G3Function8003E314(object, frame);
    }
    else if (animation == 1)
    {
        uint32 current_half = r_u16(object + 0x5e);
        w_u16(object + 0x6e, 0);
        w_u16(object + 0x5e, (uint32)last - current_half);
        return;
    }
    if (r_s16(object + 0x5e) >= 26)
        G3AniguntShootAlternating(object, frame);
}

void G3AniguntAnimateOutside(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000ae4, "G3L_06");
    uint32 frame = guest_stack - 0x18;
    uint32 resource = r_u32(object + 0x1c);
    uint32 animation = r_u16(object + 0x6e);
    uint32 animations = r_u32(resource + 0x10);
    uint32 data = r_u32(animations + animation * 4);
    sint32 count = r_s16(data + 2);
    sint32 last = count - 1;
    if (animation == 1)
    {
        sint32 current = r_s16(object + 0x5e);
        uint32 current_half = r_u16(object + 0x5e);
        if (current < last)
            w_u16(object + 0x5e, current_half + 1);
        resource = r_u32(object + 0x1c);
        if ((r_u32(resource + 0x30) & 0x2000) != 0)
            G3Function8003E314(object, frame);
    }
    else if (animation == 0)
    {
        uint32 current_half = r_u16(object + 0x5e);
        w_u16(object + 0x6e, 1);
        w_u16(object + 0x5e, (uint32)last - current_half);
    }
}

void G3AniguntShootSpread(uint32 object, uint32 offset, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000660, "G3L_06");
    uint32 frame = guest_stack - 0x28;
    sint32 signed_offset = (sint16)offset;
    sint32 yaw = r_s16(object + 0x64);
    sint32 direction = (yaw + signed_offset + 0xc00) % 4096;
    sint32 perpendicular = (direction + 0x400) % 4096;
    uint32 resource = r_u32(object + 0x110);
    if (resource == 0)
        return;
    uint32 projectile = G3Function8002A454(object, resource, frame);
    if (projectile == 0)
        return;
    uint32 angle = (uint32)direction;
    w_u32(projectile + 0x20, 0);
    w_u16(projectile + 0x68, 0x1000);
    w_u16(projectile + 0x6a, 0x1000);
    w_u16(projectile + 0x6c, 0x1000);
    uint32 sine = G3Function8005B470(angle);
    sint32 product = (sint32)(sine * 525);
    uint32 x = r_u16(projectile + 0x58);
    w_u16(projectile + 0x58, x + (uint32)(product / 4096));
    uint32 cosine = G3Function8005B590(angle);
    product = (sint32)(cosine * 525);
    uint32 y = r_u16(projectile + 0x5a);
    uint32 z = r_u16(projectile + 0x5c);
    w_u16(projectile + 0x5a, y + (uint32)(product / 4096));
    w_u16(projectile + 0x5c, z + 220);
    sine = G3Function8005B470(angle);
    product = (sint32)(sine * 40);
    w_u32(projectile + 0x104, (uint32)(product / 4096));
    cosine = G3Function8005B590(angle);
    product = (sint32)(cosine * 40);
    w_u32(projectile + 0x108, (uint32)(product / 4096));
    angle = (uint32)(sint32)(sint16)perpendicular;
    sine = G3Function8005B470(angle);
    product = (sint32)(sine * 40);
    uint32 velocity_x = r_u32(projectile + 0x104);
    w_u32(projectile + 0x104, velocity_x + (uint32)(product / 4096));
    cosine = G3Function8005B590(angle);
    product = (sint32)(cosine * 40);
    w_u32(projectile + 0x10c, 3);
    uint32 velocity_y = r_u32(projectile + 0x108);
    w_u32(projectile + 0x108, velocity_y + (uint32)(product / 4096));
}

void G3AniguntShootBomb(uint32 object, uint32 offset, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000824, "G3L_06");
    uint32 frame = guest_stack - 0x28;
    sint32 signed_offset = (sint16)offset;
    sint32 yaw = r_s16(object + 0x64);
    sint32 direction = (yaw + signed_offset + 0xc00) % 4096;
    uint32 resource = r_u32(object + 0x114);
    uint32 speed_word = ((uint32)signed_offset << 4) + 20;
    if (resource == 0)
        return;
    uint32 projectile = G3Function8002A454(object, resource, frame);
    if (projectile == 0)
        return;
    uint32 angle = (uint32)(sint32)(sint16)direction;
    w_u32(projectile + 0x20, 0);
    uint32 sine = G3Function8005B470(angle);
    sint32 product = (sint32)(sine * 425);
    uint32 x = r_u16(projectile + 0x58);
    w_u16(projectile + 0x58, x + (uint32)(product / 4096));
    uint32 cosine = G3Function8005B590(angle);
    product = (sint32)(cosine * 425);
    uint32 y = r_u16(projectile + 0x5a);
    uint32 z = r_u16(projectile + 0x5c);
    w_u16(projectile + 0x5a, y + (uint32)(product / 4096));
    w_u16(projectile + 0x5c, z - 200);
    sint32 random = (sint32)G3SdkRandom80027FA8();
    uint32 pitch = r_u16(projectile + 0x60);
    w_u16(projectile + 0x60, pitch + (uint32)(random % 4096));
    random = (sint32)G3SdkRandom80027FA8();
    uint32 projectile_yaw = r_u16(projectile + 0x64);
    w_u16(projectile + 0x64, projectile_yaw + (uint32)(random % 4096));
    sine = G3Function8005B470(angle);
    uint32 speed = (uint32)(sint32)(sint16)speed_word;
    product = (sint32)(speed * sine);
    w_u32(projectile + 0x104, (uint32)(product / 4096));
    cosine = G3Function8005B590(angle);
    product = (sint32)(speed * cosine);
    w_u32(projectile + 0x108, (uint32)(product / 4096));
    w_u32(projectile + 0x10c, 0);
    w_u32(projectile + 0x124, speed);
}

void G3AniguntShootAlternating(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000bf0, "G3L_06");
    uint32 frame = guest_stack - 0x28;
    sint32 yaw = r_s16(object + 0x64);
    sint32 direction = (yaw + 0xe00) % 4096;
    uint32 state = object + 0x104;
    uint32 previous = r_u32(state + 0x14);
    w_u32(state + 0x14, previous == 0 ? 1 : 0);
    uint32 resource = r_u32(state + 0xc);
    if (resource == 0)
        return;
    uint32 projectile = G3Function8002A454(object, resource, frame);
    if (projectile == 0)
        return;
    uint32 angle = (uint32)(sint32)(sint16)direction;
    w_u32(projectile + 0x20, 0);
    w_u16(projectile + 0x68, 3000);
    w_u16(projectile + 0x6a, 3000);
    w_u16(projectile + 0x6c, 3000);
    uint32 sine = G3Function8005B470(angle);
    sint32 product = (sint32)(sine * 300);
    uint32 x = r_u16(projectile + 0x58);
    w_u16(projectile + 0x58, x + (uint32)(product / 4096));
    uint32 cosine = G3Function8005B590(angle);
    product = (sint32)(cosine * 300);
    uint32 y = r_u16(projectile + 0x5a);
    uint32 z = r_u16(projectile + 0x5c);
    w_u16(projectile + 0x5a, y + (uint32)(product / 4096));
    w_u16(projectile + 0x5c, z + 320);
    sine = G3Function8005B470(angle);
    product = (sint32)(sine * 60);
    w_u32(projectile + 0x104, (uint32)(product / 4096));
    cosine = G3Function8005B590(angle);
    product = (sint32)(cosine * 60);
    w_u32(projectile + 0x108, (uint32)(product / 4096));
    w_u32(projectile + 0x10c, 3);
    w_u32(projectile + 0x150, 20);
    if (r_u32(state + 0x14) != 0)
    {
        cosine = G3Function8005B590(angle);
        product = (sint32)(cosine * 100);
        x = r_u16(projectile + 0x58);
        w_u16(projectile + 0x58, x + (uint32)(product / 4096));
        sine = G3Function8005B470(angle);
        product = (sint32)(0 - sine * 100);
    }
    else
    {
        cosine = G3Function8005B590(angle);
        product = (sint32)(0 - cosine * 100);
        x = r_u16(projectile + 0x58);
        w_u16(projectile + 0x58, x + (uint32)(product / 4096));
        sine = G3Function8005B470(angle);
        product = (sint32)(sine * 100);
    }
    y = r_u16(projectile + 0x5a);
    w_u16(projectile + 0x5a, y + (uint32)(product / 4096));
}

uint32 G3AttachEvent(uint32 object, uint32 message, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_20");
    uint32 frame = guest_stack - 0x18;
    if (message == 1)
    {
        G3AttachInitialize(object, frame);
        return 1;
    }
    if (message == 2)
    {
        G3AttachUpdate(object, frame);
        return 1;
    }
    return 0;
}

void G3AttachInitialize(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000005c, "G3L_20");
    uint32 frame = guest_stack - 0x18;
    uint32 intro = r_u32(object + 0x20);
    uint32 entries = r_u32(intro + 4);
    uint32 state = object + 0x104;
    uint32 index = entries != 0 ? G3AttachmentIndex80037028(object) : 0xffffffff;
    w_u32(state, index);
    w_u32(state + 4, 0);
    (void)frame;
}

void G3AttachUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000ac, "G3L_20");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    uint32 linked = r_u32(state + 4);
    if (linked == 0)
    {
        uint32 next = r_u32(object + 0x104) + 1;
        sint32 selector = (sint32)next % 2;
        linked = G3AttachedObject80036FE4(object, selector);
        w_u32(state + 4, linked);
        if (linked == 0)
            return;
        linked = r_u32(state + 4);
    }
    uint32 component = r_u16(linked + 0x58);
    uint32 position = linked + 0x58;
    w_u16(object + 0x58, component);
    component = r_u16(position + 2);
    w_u16(object + 0x5a, component);
    w_u16(object + 0x5c, r_u16(position + 4));
    (void)frame;
}

uint32 G3AttachedObject80036FE4(uint32 object, sint32 index)
{
    uint32 intro = r_u32(object + 0x20);
    uint32 entries = r_u32(intro + 4);
    if (entries == 0)
        return 0;
    sint32 count = r_s32(entries);
    if (index >= count)
        return 0;
    uint32 entry = r_u32(entries + ((uint32)index << 2) + 4);
    return r_u32(entry + 0x20);
}

uint32 G3AttachmentIndex80037028(uint32 object)
{
    uint32 intro = r_u32(object + 0x20);
    uint32 entries = r_u32(intro + 4);
    if (entries == 0)
        return 0;
    sint32 count = r_s32(entries);
    uint32 cursor = entries + 4;
    uint32 result = 0xffffffff;
    sint32 index = 0;
    while (index < count)
    {
        uint32 entry = r_u32(cursor);
        if (r_u32(entry + 0x20) == object)
            result = (uint32)index;
        ++index;
        cursor += 4;
    }
    return result;
}

uint32 G3ClackerEvent(uint32 object, uint32 game, uint32 message, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_35");
    uint32 frame = guest_stack - 0x18;
    if (message == 1)
    {
        G3ClackerInitialize(object);
        return 1;
    }
    if (message == 2)
    {
        G3ClackerUpdate(object, game, frame);
        return 1;
    }
    if (message == 3)
        G3ClackerCollide(object);
    return 0;
}

void G3ClackerInitialize(uint32 object)
{
    FUNCTION_MARKER(0x00000080, "G3L_35");
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x128, 0);
    flags |= 0x80;
    w_u32(object + 0x10, flags);
    w_u32(object + 0x10, flags | 0x40);
}

void G3ClackerCollide(uint32 object)
{
    FUNCTION_MARKER(0x0000009c, "G3L_35");
    uint32 contact = r_u32(object + 0xb0);
    uint32 other = r_u32(contact + 0x14);
    uint32 player = r_u32(0x8009efc4);
    if (other == player && r_u32(object + 0x128) == 0)
        w_u32(object + 0x128, 1);
}

void G3ClackerUpdate(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000d8, "G3L_35");
    uint32 frame = guest_stack - 0x20;
    uint32 rotation = r_u16(object + 0x64);
    uint32 collided = r_u32(object + 0x128);
    w_u16(object + 0x64, rotation + 0x80);
    if (collided == 0)
        return;
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x10, flags | 0x10);
    G3Function800193F8(0x40000, object, frame);
    G3Function8002A9AC(object, frame);
    uint32 level = r_u16(game + 0x68b4);
    w_u16(game + 0x69de, level);
    G3Function800359EC(0x120, 3, frame);
}

uint32 G3ShittrnEvent(uint32 module_base, uint32 object, uint32 message, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_34");
    uint32 frame = guest_stack - 0x18;
    if (object == 0)
        return 1;
    if (message != 2)
        return 0;
    G3ShittrnUpdate(module_base, object, frame);
    return 1;
}

void G3ShittrnUpdate(uint32 module_base, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000048, "G3L_34");
    uint32 frame = guest_stack - 0x30;
    w_u16(frame + 0x10, r_u16(object + 0x58));
    w_u16(frame + 0x12, r_u16(object + 0x5a));
    w_u16(frame + 0x14, r_u16(object + 0x5c));
    for (uint32 index = 0; index < 2; ++index)
    {
        sint32 random = (sint32)G3SdkRandom80027FA8();
        w_u16(frame + 0x18, (uint32)(random % 60 - 30));
        random = (sint32)G3SdkRandom80027FA8();
        w_u16(frame + 0x1a, (uint32)(random % 60 - 30));
        random = (sint32)G3SdkRandom80027FA8();
        uint32 angle = (uint32)r_s16(frame + 0x14);
        w_u16(frame + 0x1c, (uint32)(random % 30 + 25));
        G3Function800453A0(frame + 0x10, frame + 0x18, module_base + 0x154, angle, frame);
    }
}

uint32 G3BarrelEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_11");
    uint32 frame = guest_stack - 0x30;
    if (message == 11) G3BarrelRestorePlacements(game, module_base);
    else if (message == 12) G3BarrelRestorePosition(payload);
    uint32 settings = 0;
    uint32 state = 0;
    if (object != 0)
    {
        uint32 descriptor = r_u32(object + 0x1c);
        state = object + 0x128;
        settings = r_u32(descriptor + 0x1c);
    }
    if (settings == 0) return 1;
    uint32 index = message - 1;
    if (index >= 22) return 0;
    uint32 target = r_u32(module_base + 0x13dc + index * 4) - module_base;
    switch (target)
    {
    case 0xac: G3BarrelInitialize(object, state, settings, module_base); return 1;
    case 0xc8: G3BarrelUpdate(object, game, state, settings, module_base, frame); return 1;
    case 0xe4: G3BarrelCollide(object, game, state, settings, module_base, frame); return 1;
    case 0x100: G3BarrelStopFall(object, state); return 1;
    case 0x11c:
    {
        sint32 active = r_s16(state + 0x24);
        if (active != 0) G3Function8003A114(state + 0x14, (uint32)active);
        return 1;
    }
    case 0x13c: return 0;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "Unresolved Barrel event target %08X\n", target + module_base);
#endif
        }
        abort();
    }
}

void G3BarrelRestorePosition(uint32 placement)
{
    FUNCTION_MARKER(0x00000168, "G3L_11");
    uint32 destination = r_u32(placement + 0x30);
    if (destination == 0) return;
    uint32 source = placement + 0x10;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 lane = source & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
    uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
    uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
    uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
    uint32 last = (uint32)r_s16(placement + 0x14);
    high_address = destination + 3;
    high_lane = high_address & 3;
    lane = destination & 3;
    w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
    w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
    w_u16(destination + 4, last);
}

void G3BarrelInitialize(uint32 object, uint32 state, uint32 settings, uint32 module_base)
{
    FUNCTION_MARKER(0x00000198, "G3L_11");
    uint32 descriptor = r_u32(object + 0x1c);
    sint32 animated = r_s16(settings + 0xc);
    uint32 name = r_u32(descriptor + 0x24);
    if (animated != 0)
    {
        w_u16(object + 0x9e, 1);
        uint32 flags = r_u32(object + 0x10);
        descriptor = r_u32(object + 0x1c);
        w_u32(object + 0x10, flags | 0x80);
        uint32 models = r_u32(descriptor + 0xc);
        G3Function80039834(r_u32(models));
    }
    w_u32(object + 0x10, r_u32(object + 0x10) | 0x20000);
    w_u16(state, 0);
    w_u16(state + 2, 0);
    w_u16(state + 4, 0);
    w_u16(state + 6, 0);
    w_u16(state + 0xe, 0);
    uint32 first = r_u32(name);
    uint32 expected = r_u32(module_base + 0x1434);
    uint32 sound = 0;
    if (first == expected)
    {
        uint32 second = r_u32(name + 4);
        uint32 expected_second = r_u32(module_base + 0x1438);
        if (second == expected_second) sound = 0x907;
    }
    else first = r_u32(name);
    if (sound == 0)
    {
        expected = r_u32(module_base + 0x1440);
        if (first == expected)
        {
            uint32 second = r_u32(name + 4);
            uint32 expected_second = r_u32(module_base + 0x1444);
            if (second == expected_second) sound = 0x90f;
        }
        if (sound == 0)
        {
            expected = r_u32(module_base + 0x144c);
            if (first == expected)
            {
                uint32 second = r_u32(name + 4);
                uint32 expected_second = r_u32(module_base + 0x1450);
                if (second == expected_second) sound = 0x90f;
            }
        }
        if (sound == 0)
        {
            expected = r_u32(module_base + 0x1458);
            if (first == expected)
            {
                uint32 second = r_u32(name + 4);
                uint32 expected_second = r_u32(module_base + 0x145c);
                if (second == expected_second) sound = 0x90f;
            }
        }
        if (sound == 0)
        {
            first = r_u32(name);
            expected = r_u32(module_base + 0x1464);
            if (first == expected)
            {
                uint32 second = r_u32(name + 4);
                uint32 expected_second = r_u32(module_base + 0x1468);
                if (second == expected_second) sound = 0x904;
            }
        }
    }
    if (sound != 0)
    {
        w_u16(state + 0x18, sound);
        w_u16(state + 0x1e, 100);
        w_u16(state + 0x1a, 0x157c);
        w_u32(state + 0x14, 0);
        w_u16(state + 0x1c, 0);
        w_u16(state + 0x20, 0);
        w_u16(state + 0x24, 1);
    }
    else
    {
        w_u32(state + 0x14, 0);
        w_u16(state + 0x18, 0);
        w_u16(state + 0x1c, 0);
        w_u16(state + 0x1e, 0);
        w_u16(state + 0x1a, 0);
        w_u16(state + 0x20, 0);
        w_u16(state + 0x24, 0);
    }
}

uint32 G3BarrelOverlaps(uint32 object, uint32 other, uint32 settings)
{
    FUNCTION_MARKER(0x00000358, "G3L_11");
    sint32 z = r_s16(object + 0x5c);
    sint32 top = r_s16(settings + 0x14);
    sint32 other_z = r_s16(other + 0x5c);
    if (other_z >= z + top) return 0;
    sint32 bottom = r_s16(settings + 0x16);
    if (z + bottom >= other_z) return 0;
    uint32 x = (uint32)r_s16(object + 0x58);
    uint32 other_x = (uint32)r_s16(other + 0x58);
    x -= other_x;
    uint32 y = (uint32)r_s16(object + 0x5a);
    uint32 other_y = (uint32)r_s16(other + 0x5a);
    y -= other_y;
    sint32 radius_squared = r_s32(settings + 0x10);
    return (sint32)(x * x + y * y) < radius_squared;
}

uint32 G3BarrelFindLinked(uint32 object, uint32 settings, uint32 module_base)
{
    FUNCTION_MARKER(0x000003d0, "G3L_11");
    uint32 linked = 0;
    sint32 count = (sint32)G3AttachmentCount80036FC0(object);
    if (count == 0) return 0;
    if (r_u32(settings + 0x10) == 0 || count <= 0) return 0;
    for (sint32 index = 0; index < count; ++index)
    {
        uint32 other = G3AttachedObject80036FE4(object, index);
        if (other == 0 || other == object) continue;
        uint32 descriptor = r_u32(other + 0x1c);
        uint32 name = r_u32(descriptor + 0x20);
        uint32 expected = r_u32(module_base + 0x1470);
        uint32 first = r_u32(name);
        uint32 other_settings = r_u32(descriptor + 0x1c);
        if (first != expected) continue;
        uint32 second = r_u32(name + 4);
        uint32 expected_second = r_u32(module_base + 0x1474);
        if (second != expected_second) continue;
        if (r_s16(other_settings + 6) != 0 && r_s16(other + 0x12e) == 0) linked = other;
    }
    return linked;
}

uint32 G3BarrelMoveAllowed(uint32 object, uint32 delta)
{
    FUNCTION_MARKER(0x00000b7c, "G3L_11");
    uint32 settings = r_u32(object + 0x28);
    if (settings == 0) return 1;
    uint32 allowed = 1;
    sint32 x = r_s16(object + 0x58);
    sint32 dx = r_s16(delta);
    sint32 maximum = r_s16(settings + 0xa);
    x += dx;
    if (maximum < x) allowed = 0;
    sint32 minimum = r_s16(settings + 4);
    if (x < minimum) allowed = 0;
    sint32 y = r_s16(object + 0x5a);
    sint32 dy = r_s16(delta + 2);
    maximum = r_s16(settings + 0xc);
    y += dy;
    if (maximum < y) allowed = 0;
    minimum = r_s16(settings + 6);
    if (y < minimum) allowed = 0;
    return allowed;
}

void G3BarrelStopFall(uint32 object, uint32 state)
{
    FUNCTION_MARKER(0x000012f8, "G3L_11");
    uint32 placement = r_u32(object + 0x20);
    if (placement != 0 && (r_u32(placement + 0x18) & 0x100) != 0) return;
    w_u16(state + 4, 0);
}

void G3BarrelRestorePlacements(uint32 game, uint32 module_base)
{
    FUNCTION_MARKER(0x00001328, "G3L_11");
    uint32 level = r_u32(game + 0x48);
    uint32 count = r_u32(level + 0x7c);
    uint32 data = r_u32(level);
    uint32 record = r_u32(data + 0x14);
    uint32 end = record + count * 0x34;
    uint32 destination = record + 0x10;
    while (record < end)
    {
        uint32 descriptor = r_u32(record);
        if (descriptor != 0)
        {
            uint32 name = r_u32(descriptor + 0x20);
            uint32 expected = r_u32(module_base + 0x1470);
            uint32 first = r_u32(name);
            uint32 source = r_u32(destination + 0x20);
            if (first == expected)
            {
                uint32 second = r_u32(name + 4);
                uint32 expected_second = r_u32(module_base + 0x1474);
                if (second == expected_second && source != 0)
                {
                    uint32 high_address = source + 3;
                    uint32 high_lane = high_address & 3;
                    uint32 lane = source & 3;
                    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
                    uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
                    uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
                    uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
                    uint32 last = (uint32)r_s16(source + 4);
                    high_address = destination + 3;
                    high_lane = high_address & 3;
                    lane = destination & 3;
                    w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
                    w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
                    w_u16(destination + 4, last);
                }
            }
        }
        record += 0x34;
        destination += 0x34;
    }
}

uint32 G3PlayerAttacksContact800785AC(uint32 contact, uint32 game)
{
    uint32 owner = r_u32(contact + 0x14);
    uint32 player = r_u32(game + 0x50);
    if (owner != player) return 0;
    if (r_u32(owner + 0x154) != 16) return 0;
    if (r_s16(contact + 6) != 1) return 0;
    uint32 descriptor = r_u32(contact + 0xc);
    uint32 kind = r_u8(descriptor + 5);
    return kind - 6 < 4;
}

void G3BarrelUpdate(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000004dc, "G3L_11");
    uint32 frame = guest_stack - 0x58;
    w_u32(frame + 0x5c, game);
    uint32 floating = r_u16(settings + 6);
    uint32 gravity = r_u16(settings + 2);
    sint32 sound_active = r_s16(state + 0x24);
    uint32 custom = r_u32(object + 0x28);
    if (sound_active != 0) G3Function80039F5C(object, state + 0x14, (uint32)r_s16(state + 0xe), frame);
    uint32 linked = G3BarrelFindLinked(object, settings, module_base);
    if (custom != 0)
    {
        gravity = r_u16(custom);
        sint32 custom_float = r_s16(custom + 2);
        floating = r_u16(custom + 2);
        if (custom_float == 0)
        {
            for (uint32 axis = 0; axis != 2; ++axis)
            {
                sint32 coordinate = r_s16(object + 0x58 + axis * 2);
                sint32 maximum = r_s16(custom + 0xa + axis * 2);
                uint32 maximum_bits = r_u16(custom + 0xa + axis * 2);
                if (maximum < coordinate)
                {
                    w_u16(object + 0x58 + axis * 2, maximum_bits);
                    w_u16(state, 1);
                }
                coordinate = r_s16(object + 0x58 + axis * 2);
                sint32 minimum = r_s16(custom + 4 + axis * 2);
                uint32 minimum_bits = r_u16(custom + 4 + axis * 2);
                if (coordinate < minimum)
                {
                    w_u16(object + 0x58 + axis * 2, minimum_bits);
                    w_u16(state, 1);
                }
            }
            gravity = r_u16(settings + 2);
        }
    }
    bool apply_fall = true;
    if (floating != 0 && r_s16(state + 6) == 0) apply_fall = false;
    if (apply_fall)
    {
        if ((r_u32(object + 0x10) & 0x8000) == 0)
        {
            uint32 actual_game = r_u32(frame + 0x5c);
            if (r_u32(actual_game + 0x14c) >= 2)
            {
                uint32 z = r_u16(object + 0x5c);
                uint32 velocity = r_u16(state + 4);
                w_u16(object + 0x5c, z + velocity);
                velocity = r_u16(state + 4) + gravity;
                w_u16(state + 4, velocity);
                sint32 terminal = r_s16(settings + 4);
                uint32 terminal_bits = r_u16(settings + 4);
                if ((sint16)velocity < terminal) w_u16(state + 4, terminal_bits);
            }
        }
        if (r_s16(state + 6) != 0)
        {
            if (r_s16(state + 4) < 0)
            {
                uint32 falling = r_u32(state + 0x10);
                w_u32(falling + 0x10, r_u32(falling + 0x10) & 0xfffff7ff);
                sint32 phase = r_s16(state + 6);
                if (phase < 30)
                {
                    uint32 scale = r_u16(0x8009da4c + (uint32)phase * 2);
                    falling = r_u32(state + 0x10);
                    w_u16(falling + 0x68, scale);
                    falling = r_u32(state + 0x10);
                    w_u16(falling + 0x6a, scale);
                    falling = r_u32(state + 0x10);
                    w_u16(falling + 0x6c, scale);
                    uint32 next = r_u16(state + 6);
                    uint32 step = r_u16(settings + 0xa);
                    w_u16(state + 6, next + step);
                }
            }
            uint32 placement = r_u32(object + 0x20);
            sint32 z = r_s16(object + 0x5c);
            sint32 original_z = r_s16(placement + 0x14);
            if (z < original_z)
            {
                uint32 falling = r_u32(state + 0x10);
                w_u32(falling + 0x10, r_u32(falling + 0x10) & 0xfffffbff);
                falling = r_u32(state + 0x10);
                G3Function80037150(falling, 0xf000, frame);
                w_u32(object + 0x10c, 0);
                w_u16(object + 0x5c, r_u16(state + 0xc));
                falling = r_u32(state + 0x10);
                w_u16(state + 6, 0);
                w_u16(falling + 0x68, 0x1000);
                falling = r_u32(state + 0x10);
                w_u16(falling + 0x6a, 0x1000);
                falling = r_u32(state + 0x10);
                w_u16(falling + 0x6c, 0x1000);
            }
        }
    }
    if (r_s16(state + 2) != 0)
    {
        G3Function80039F5C(object, state + 0x14, 1, frame);
        uint32 countdown = r_u16(state + 2) - 1;
        w_u16(state + 2, countdown);
        if ((uint16)countdown == 0)
        {
            uint32 actual_game = r_u32(frame + 0x5c);
            uint32 list = r_u32(actual_game + 0x5c);
            uint32 other = r_u32(list + 4);
            G3Function8003A114(state + 0x14, list);
            w_u32(frame + 0x10, 0x157c);
            G3Function8003E518(object, 0x833, 0, 0x7f, frame);
            w_u32(frame + 0x10, 8);
            w_u32(frame + 0x18, object + 0x58);
            w_u32(frame + 0x24, 600);
            uint32 player = r_u32(0x8009efc4);
            w_u32(frame + 0x20, 0x157c);
            w_u32(frame + 0x14, 0);
            w_u32(frame + 0x28, 1);
            w_u32(frame + 0x1c, player + 0x58);
            G3Function80081A90(0, 1, 0xffffffff, 0x1000, frame);
            if (linked != 0)
            {
                uint32 linked_descriptor = r_u32(linked + 0x1c);
                uint32 linked_settings = r_u32(linked_descriptor + 0x1c);
                uint32 initial_velocity = r_u16(linked_settings + 6);
                w_u16(linked + 0x12e, 1);
                w_u16(linked + 0x12c, initial_velocity);
                uint32 initial_z = r_u16(linked + 0x5c);
                w_u32(linked + 0x138, object);
                w_u16(linked + 0x134, initial_z);
            }
            while (other != 0)
            {
                if (other != object && G3BarrelOverlaps(object, other, settings) != 0)
                {
                    uint32 descriptor = r_u32(other + 0x1c);
                    uint32 name = r_u32(descriptor + 0x20);
                    uint32 expected = r_u32(module_base + 0x1470);
                    uint32 first = r_u32(name);
                    uint32 other_settings = r_u32(descriptor + 0x1c);
                    bool is_barrel = first == expected;
                    if (is_barrel)
                    {
                        uint32 second = r_u32(name + 4);
                        uint32 expected_second = r_u32(module_base + 0x1474);
                        is_barrel = second == expected_second;
                    }
                    if (is_barrel)
                    {
                        if ((r_u32(descriptor + 0x30) & 0x4000000) != 0)
                        {
                            if (r_s16(settings + 0xc) != 0)
                            {
                                w_u16(other + 0x12a, r_u16(other_settings + 0xc));
                                w_u16(other + 0x130, r_u16(other_settings + 0xe));
                                w_u16(other + 0x132, r_u16(other_settings + 0xe));
                            }
                            else G3Function80038798(other, 0, 0, frame);
                        }
                    }
                    else
                    {
                        descriptor = r_u32(other + 0x1c);
                        if ((r_u32(descriptor + 0x30) & 0x4000000) != 0) G3Function80038798(other, 0, 0, frame);
                    }
                }
                other = r_u32(other + 8);
            }
            if (linked != 0)
            {
                uint32 resource = G3Function8003AA48(module_base + 0x147c);
                w_u32(object + 0x10, r_u32(object + 0x10) | 0xc00);
                G3Function800370E8(object, 0xf000, frame);
                if (resource != 0)
                {
                    w_u16(object + 0x5c, r_u16(object + 0x5c) + 500);
                    uint32 models = r_u32(resource + 0xc);
                    uint32 model = r_u32(models);
                    uint32 data = r_u32(model + 0x1c);
                    uint32 rows = r_u32(data + 8);
                    uint32 columns = r_u32(data + 0xc);
                    uint32 pool = r_u32(0x800a27fc);
                    uint32 amount = (uint32)(sint32)(sint16)(rows * columns);
                    G3Function800444E4(object + 0x58, model, pool, amount, frame);
                    uint32 z = r_u16(object + 0x5c);
                    pool = r_u32(0x800a27fc);
                    w_u16(object + 0x5c, z - 500);
                    w_u32(frame + 0x18, 0x80041de0);
                    w_u32(frame + 0x10, pool);
                    w_u32(frame + 0x14, 0);
                    w_u32(frame + 0x1c, 8);
                    G3Function800425CC(object, 0, 0, 0, frame);
                }
            }
            else G3Function80038798(object, 0, 0, frame);
            actual_game = r_u32(frame + 0x5c);
            player = r_u32(actual_game + 0x50);
            if (G3BarrelOverlaps(object, player, settings) != 0)
                G3Function80074044(object, r_u32(frame + 0x5c), frame);
        }
        else
        {
            uint32 interval = r_u16(state + 8) - 1;
            w_u16(state + 8, interval);
            if ((uint16)interval == 0)
            {
                sint32 limit = r_s16(state + 0xa);
                uint32 limit_bits = r_u16(state + 0xa);
                if (limit >= 2) w_u16(state + 0xa, limit_bits - 1);
                w_u16(state + 8, r_u16(state + 0xa));
                w_u16(object + 0x9e, r_u16(object + 0x9e) + 1);
            }
        }
    }
    if (r_s16(settings + 0x18) != 0)
    {
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 placement = r_u32(object + 0x20);
            uint32 coordinate = r_u16(object + 0x58 + axis * 2);
            w_u16(placement + 0x10 + axis * 2, coordinate);
        }
    }
    if (linked != 0)
    {
        w_u16(linked + 0x58, r_u16(object + 0x58));
        w_u16(linked + 0x5a, r_u16(object + 0x5a));
    }
    if (r_s16(settings + 0xc) == 0) w_u16(object + 0x9e, r_u16(object + 0x9e) + 1);
    w_u16(state + 0xe, 0);
}

void G3BarrelCollide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000c10, "G3L_11");
    uint32 frame = guest_stack - 0x60;
    w_u32(frame + 0x6c, settings);
    uint32 contact = r_u32(object + 0xb0);
    uint32 player = r_u32(game + 0x50);
    uint32 placement = r_u32(object + 0x20);
    uint32 player_data = r_u32(player + 0x24);
    uint32 delta = contact + 0x28;
    if (placement != 0 && (r_u32(placement + 0x18) & 0x100) != 0) return;
    uint32 linked = G3BarrelFindLinked(object, r_u32(frame + 0x6c), module_base);
    uint32 other = r_u32(contact + 0x14);
    uint32 actual_player = r_u32(game + 0x50);
    if (other == actual_player)
    {
        uint32 descriptor = r_u32(object + 0x1c);
        if ((r_u32(descriptor) & 0x20000000) != 0 && (r_u32(player + 0x134) & 2) == 0) return;
        sint32 kind = r_s16(contact + 6);
        if (kind != 1) return;
        uint32 contact_data = r_u32(contact + 0xc);
        if (r_u8(contact_data + 4) == 0)
        {
            uint32 object_y = (uint32)r_s16(object + 0x5a);
            uint32 player_y = (uint32)r_s16(player + 0x5a);
            uint32 object_x = (uint32)r_s16(object + 0x58);
            uint32 player_x = (uint32)r_s16(player + 0x58);
            uint32 angle = G3Function8002F240(object_y - player_y, object_x - player_x, frame);
            uint32 heading = r_u16(player_data + 0xc8);
            uint32 difference = (angle + 0x400 - heading) & 0xfff;
            if (difference >= 0x800) difference |= 0xf000;
            bool facing = true;
            if (r_s16(contact + 6) == 1)
            {
                sint32 signed_angle = (sint16)difference;
                if (signed_angle < 0) signed_angle = -signed_angle;
                facing = signed_angle <= 0x300;
            }
            if (facing)
            {
                uint32 actual_settings = r_u32(frame + 0x6c);
                sint32 capability = r_s16(player_data + 0xe0);
                sint32 resistance = r_s16(actual_settings);
                if (capability >= resistance && resistance != 0)
                {
                    sint32 x = r_s16(contact + 0x28);
                    sint32 z = r_s16(delta + 4);
                    if (x < 0) x = -x;
                    sint32 y = r_s16(delta + 2);
                    if (z < 0) z = -z;
                    if (y < 0) y = -y;
                    if (x > y) { sint32 temporary = x; x = y; y = temporary; }
                    if (y > z) { sint32 temporary = y; y = z; z = temporary; }
                    if (x > y) { sint32 temporary = x; x = y; y = temporary; }
                    uint32 speed = (uint32)((z * 30 + y * 12 + x * 9) >> 5);
                    uint32 sine = G3Function8005B470((uint32)r_s16(player_data + 0xc8) - 0x400);
                    w_u16(frame + 0x10, (uint32)((sint32)(sine * speed) >> 12));
                    uint32 cosine = G3Function8005B590((uint32)r_s16(player_data + 0xc8) - 0x400);
                    w_u16(frame + 0x14, 0);
                    w_u16(frame + 0x12, (uint32)((sint32)(cosine * speed) >> 12));
                    if (G3BarrelMoveAllowed(object, frame + 0x10) != 0)
                    {
                        uint32 coordinate = r_u16(object + 0x58);
                        uint32 offset = r_u16(frame + 0x10);
                        w_u16(object + 0x58, coordinate + offset);
                        coordinate = r_u16(object + 0x5a);
                        offset = r_u16(frame + 0x12);
                        w_u16(object + 0x5a, coordinate + offset);
                        G3Function8005F3A8(object, frame + 0x10);
                        if (linked != 0)
                        {
                            coordinate = r_u16(linked + 0x58);
                            offset = r_u16(frame + 0x10);
                            w_u16(linked + 0x58, coordinate + offset);
                            coordinate = r_u16(linked + 0x5a);
                            offset = r_u16(frame + 0x12);
                            w_u16(linked + 0x5a, coordinate + offset);
                            G3Function8005F3A8(linked, frame + 0x10);
                        }
                    }
                    else w_u16(state, 1);
                    uint32 target = r_u32(player + 0xec);
                    if (target != 0)
                    {
                        actual_settings = r_u32(frame + 0x6c);
                        uint32 payload = (uint32)r_s16(actual_settings);
                        G3DispatchStateEvent(target, player, game, 0x3ed, payload, frame);
                        target = r_u32(player + 0xec);
                        if (target != 0) G3DispatchStateEvent(target, player, game, 0x3ef, 0, frame);
                    }
                    if (r_s16(state) != 0)
                    {
                        w_u16(state, 0);
                        w_u16(delta, r_u16(frame + 0x10));
                        w_u16(delta + 2, r_u16(frame + 0x12));
                    }
                    else
                    {
                        w_u16(delta, 0);
                        w_u16(delta + 2, 0);
                        w_u16(delta + 4, 0);
                        w_u16(state + 0xe, 1);
                    }
                }
            }
            kind = r_s16(contact + 6);
        }
        if (kind != 1) return;
        contact_data = r_u32(contact + 0xc);
        if (r_u8(contact_data + 4) != 9) return;
        uint32 actual_settings = r_u32(frame + 0x6c);
        if (r_s16(actual_settings + 0xc) == 0) return;
        if (r_s16(state + 2) != 0) return;
        uint32 origin = r_u32(contact + 0x10);
        descriptor = r_u32(origin + 0x1c);
        uint32 name = r_u32(descriptor + 0x24);
        uint32 expected = r_u32(module_base + 0x1488);
        uint32 first = r_u32(name);
        if (first == expected)
        {
            uint32 second = r_u32(name + 4);
            uint32 expected_second = r_u32(module_base + 0x148c);
            if (second == expected_second) return;
        }
        if (G3PlayerAttacksContact800785AC(contact, game) == 0) return;
        actual_settings = r_u32(frame + 0x6c);
        w_u16(state + 2, r_u16(actual_settings + 0xc));
        w_u16(state + 8, r_u16(actual_settings + 0xe));
        uint32 interval = r_u16(actual_settings + 0xe);
        w_u16(state + 0x18, 0x829);
        w_u16(state + 0x1e, 0x7f);
        w_u32(state + 0x14, 0);
        w_u16(state + 0x1c, 0);
        w_u16(state + 0x1a, 0x157c);
        w_u16(state + 0x20, 1);
        w_u16(state + 0xa, interval);
        return;
    }
    uint32 old_z = r_u16(delta + 4);
    G3Function8003C548(contact, frame + 0x30, frame + 0x31);
    uint32 first_type = r_u8(frame + 0x30);
    if (first_type - 6 < 4)
    {
        uint32 second_type = r_u8(frame + 0x31);
        if (second_type == 9 || second_type - 3 < 3)
        {
            other = r_u32(contact + 0x14);
            w_u32(frame + 0x18, 1);
            w_u16(frame + 0x28, 0);
            w_u16(frame + 0x2a, 0);
            w_u32(frame + 0x2c, contact);
            uint32 target = r_u32(other + 0xec);
            if (target != 0) G3DispatchStateEvent(target, other, game, 8, frame + 0x18, frame);
        }
    }
    other = r_u32(contact + 0x14);
    if (other != 0)
    {
        uint32 descriptor = r_u32(other + 0x1c);
        uint32 name = r_u32(descriptor + 0x20);
        uint32 expected = r_u32(module_base + 0x1470);
        uint32 first = r_u32(name);
        bool is_barrel = first == expected;
        if (is_barrel)
        {
            uint32 second = r_u32(name + 4);
            uint32 expected_second = r_u32(module_base + 0x1474);
            is_barrel = second == expected_second;
        }
        if (is_barrel)
        {
            uint32 combination = r_u32(contact + 4);
            if (combination == 0x20001)
            {
                w_u16(delta, 0);
                w_u16(delta + 2, 0);
                combination = r_u32(contact + 4);
            }
            if (combination == 0x10002) w_u16(delta + 4, 0);
            combination = r_u32(contact + 4);
            if (combination == 0x10001)
            {
                sint32 vertical = r_s16(delta + 4);
                if (vertical < 0) vertical = -vertical;
                if (vertical >= 9)
                {
                    w_u16(delta, 0);
                    w_u16(delta + 2, 0);
                    w_u16(delta + 4, 0);
                }
            }
        }
    }
    uint32 kind = r_u16(contact + 6);
    if (kind - 2 < 2) w_u16(state + 4, 0);
    sint32 x = r_s16(delta);
    if (x < 0) x = -x;
    bool moving = x >= 9;
    if (!moving)
    {
        sint32 y = r_s16(delta + 2);
        if (y < 0) y = -y;
        moving = y >= 9;
    }
    if (moving) w_u16(state, 1);
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        uint32 coordinate = r_u16(object + 0x58 + axis * 2);
        uint32 offset = r_u16(delta + axis * 2);
        w_u16(object + 0x58 + axis * 2, coordinate + offset);
    }
    G3Function8005F3A8(object, delta);
    if (linked != 0)
    {
        w_u16(frame + 0x10, r_u16(delta));
        uint32 dy = r_u16(delta + 2);
        w_u16(frame + 0x14, 0);
        w_u16(frame + 0x12, dy);
        uint32 coordinate = r_u16(linked + 0x58);
        uint32 offset = r_u16(delta);
        w_u16(linked + 0x58, coordinate + offset);
        coordinate = r_u16(linked + 0x5a);
        offset = r_u16(delta + 2);
        w_u16(linked + 0x5a, coordinate + offset);
        uint32 actual_settings = r_u32(frame + 0x6c);
        coordinate = r_u16(object + 0x5c);
        offset = r_u16(actual_settings + 8);
        w_u16(linked + 0x5c, coordinate + offset);
        G3Function8005F3A8(linked, frame + 0x10);
    }
    w_u16(delta + 4, old_z);
}

uint32 G3BirdApproachHeight80036E9C(uint32 speed, uint32 target, uint32 object)
{
    sint32 current = r_s16(object + 0x5c);
    sint32 target_height = (sint16)target;
    sint32 difference = target_height - current;
    uint32 current_bits = r_u16(object + 0x5c);
    uint32 distance_bits = difference >= 0 ? target - current_bits : current_bits - target;
    sint32 distance = (sint16)distance_bits;
    sint32 signed_speed = (sint16)speed;
    uint32 step;
    if (signed_speed * 4 < distance)
        step = (uint32)(signed_speed + (signed_speed < 0 ? 1 : 0)) >> 1;
    else if (signed_speed * 2 < distance)
        step = (uint32)(signed_speed + (signed_speed < 0 ? 3 : 0)) >> 2;
    else if (signed_speed < distance)
        step = (uint32)(signed_speed + (signed_speed < 0 ? 7 : 0)) >> 3;
    else
    {
        sint32 half = (signed_speed + (signed_speed < 0 ? 1 : 0)) >> 1;
        if (half < distance)
            step = (uint32)(signed_speed + (signed_speed < 0 ? 15 : 0)) >> 4;
        else
        {
            sint32 quarter = (signed_speed + (signed_speed < 0 ? 3 : 0)) >> 2;
            if (quarter < distance)
                step = (uint32)(signed_speed + (signed_speed < 0 ? 15 : 0)) >> 4;
            else step = distance > 0 ? 1 : 0;
        }
    }
    current = r_s16(object + 0x5c);
    current_bits = r_u16(object + 0x5c);
    uint32 result = current < target_height ? current_bits + step : current_bits - step;
    w_u16(object + 0x5c, result);
    return result;
}

uint32 G3BirdEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_38");
    uint32 frame = guest_stack - 0x18;
    if (object != 0 && r_u32(object + 0x28) == 0) return 1;
    switch (message)
    {
    case 1: G3BirdInitialize(object, module_base, frame); return 1;
    case 2: G3BirdUpdate(object, game, module_base, frame); return 1;
    case 3: G3BirdCollide(object, game, frame); return 1;
    case 8: G3BirdDamage(object, game, module_base, frame); return 1;
    default: return 0;
    }
}

void G3BirdInitialize(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000b8, "G3L_38");
    uint32 frame = guest_stack - 0x30;
    uint32 commands = r_u32(object + 0x28);
    uint32 flags = r_u32(object + 0x10);
    uint32 attachment = r_u32(object + 0x20);
    w_u16(object + 0x60, 0);
    w_u32(object + 0x10, flags | 0x100000);
    w_u16(attachment + 8, 0);
    attachment = r_u32(object + 0x20);
    uint32 state = object + 0x104;
    w_u16(object + 0x62, 0);
    w_u16(attachment + 0xa, 0);
    w_u16(state + 0xa, 0);
    w_u16(state + 0xc, 0);
    w_u16(state + 0xe, 0);
    w_u16(state + 0x10, 0xffff);
    w_u16(state + 0x12, 0xffff);
    w_u16(state + 0x14, 0xffff);
    w_u16(state + 0x16, 0);
    w_u16(state + 0x18, 0);
    w_u16(state + 0x1a, commands != 0 ? r_u16(commands) : 0);
    w_u16(state + 0x1c, commands != 0 ? r_u16(commands + 2) : 0);
    w_u16(state + 0x1e, commands != 0 ? r_u16(commands + 4) : 1);
    w_u16(state + 0x20, commands != 0 ? r_u16(commands + 6) : 8);
    w_u16(state + 0x22, commands != 0 ? r_u16(commands + 8) : 5000);
    w_u16(state + 0x24, commands != 0 ? r_u16(commands + 0xa) : 0x30);
    w_u16(state + 0x26, commands != 0 ? r_u16(commands + 0xc) : 1600);
    w_u16(state + 0x28, commands != 0 ? r_u16(commands + 0xe) : 90);
    w_u16(state + 0x2a, commands != 0 ? r_u16(commands + 0x10) : 0);
    w_u16(state + 0x2c, commands != 0 ? r_u16(commands + 0x12) : 2);
    w_u16(state + 0x38, commands != 0 ? r_u16(commands + 0x20) : r_u16(object + 0x58) - 3600);
    w_u16(state + 0x3e, commands != 0 ? r_u16(commands + 0x26) : r_u16(object + 0x58) + 3600);
    w_u16(state + 0x3a, commands != 0 ? r_u16(commands + 0x22) : r_u16(object + 0x5a) - 3600);
    w_u16(state + 0x40, commands != 0 ? r_u16(commands + 0x28) : r_u16(object + 0x5a) + 3600);
    w_u16(state + 0x3c, commands != 0 ? r_u16(commands + 0x24) : r_u16(object + 0x5c));
    w_u16(state + 0x42, commands != 0 ? r_u16(commands + 0x2a) : r_u16(object + 0x5c) + 1800);
    w_u16(state + 0x2e, commands != 0 ? r_u16(commands + 0x14) : 16);
    w_u16(state + 0x30, commands != 0 ? r_u16(commands + 0x16) : 0x30);
    w_u32(state + 0x34, 0);
    w_u32(state + 0x44, 0);
    if (commands != 0)
    {
        for (uint32 index = 0; index < 8; ++index) w_u8(frame + 0x10 + index, r_u8(commands + 0x18 + index));
        w_u8(frame + 0x18, 0);
        uint32 resource = G3Function8003AA48(frame + 0x10);
        uint32 size_bits = r_u16(state + 0x30);
        sint32 size = r_s16(state + 0x30);
        w_u32(state + 0x34, resource);
        if (size <= 0) size_bits = 0x30;
        uint32 health_bits = r_u16(state + 0x1e);
        sint32 health = r_s16(state + 0x1e);
        w_u16(state + 0x30, size_bits);
        if (health <= 0) health_bits = 1;
        w_u16(state + 0x1e, health_bits);
        if (r_s16(commands + 0x2c) != 0)
        {
            w_u16(state + 0x1c, 1);
            uint32 component = r_u16(commands + 0x2c);
            attachment = r_u32(object + 0x20);
            w_u16(object + 0x58, component);
            w_u16(attachment + 0x10, component);
            component = r_u16(commands + 0x2e);
            attachment = r_u32(object + 0x20);
            w_u16(object + 0x5a, component);
            w_u16(attachment + 0x12, component);
            component = r_u16(commands + 0x30);
            attachment = r_u32(object + 0x20);
            w_u16(object + 0x5c, component);
            w_u16(attachment + 0x14, component);
        }
    }
    uint32 mode = r_s16(state + 0x1c) != 0 ? 0 : 2;
    G3BirdSetMode(object, mode, 1, module_base, frame);
}

uint32 G3BirdTable(uint32 variant, uint32 mode, uint32 phase, uint32 module_base)
{
    FUNCTION_MARKER(0x00000408, "G3L_38");
    return module_base + 0x1a20 + (((variant * 5 + mode) * 4 + phase) * 8);
}

void G3BirdSetMode(uint32 object, uint32 mode, uint32 force, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000430, "G3L_38");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    uint32 entering = G3BirdTable((uint32)r_s16(state + 0x1a), mode, 2, module_base);
    uint32 row = G3BirdTable((uint32)r_s16(state + 0x1a), mode, 0, module_base);
    if (r_s16(entering) >= 0 && force == 0) row = entering;
    uint32 phase = r_s16(entering) >= 0 && force == 0 ? 2 : 0;
    w_u16(state + 0x18, phase);
    uint32 previous = r_u16(state + 0x14);
    w_u16(state + 0x14, 0xffff);
    w_u16(state + 0x10, 0xffff);
    w_u16(state + 0x12, previous);
    w_u32(object + 0x150, mode);
    w_u16(object + 0x6e, r_u16(row));
    w_u16(object + 0x5e, r_u16(row + 2));
    switch (mode)
    {
    case 0: w_u16(state + 0xc, 1); break;
    case 1: w_u16(state + 0x10, 2); break;
    case 3:
    {
        uint32 duration = r_u16(state + 0x20);
        uint32 cooldown = r_u16(state + 0x2a);
        sint32 variant = r_s16(state + 0x1a);
        w_u16(state + 0xc, 0);
        w_u16(state + 0x16, duration);
        w_u16(state + 0xe, cooldown);
        if (variant == 1) w_u16(state + 0x10, 1);
        break;
    }
    case 4: w_u16(state + 0x10, r_s16(state + 0x1a) == 2 ? 4 : 1); break;
    }
    (void)frame;
}

void G3BirdAnimate(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000590, "G3L_38");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    uint32 variant = (uint32)r_s16(state + 0x1a);
    uint32 mode = r_u32(object + 0x150);
    uint32 phase = (uint32)r_s16(state + 0x18);
    uint32 row = G3BirdTable(variant, mode, phase, module_base);
    sint32 limit = r_s16(row + 4);
    if (limit <= 0)
    {
        uint32 descriptor = r_u32(object + 0x1c);
        uint32 index = (uint32)r_s16(row);
        uint32 table = r_u32(descriptor + 0x10);
        uint32 animation = r_u32(table + index * 4);
        limit = r_s16(animation + 2) - 1;
    }
    uint32 next_frame = r_u16(object + 0x5e) + 1;
    bool finished = (sint16)next_frame >= limit;
    w_u16(object + 0x5e, next_frame);
    uint32 descriptor = r_u32(object + 0x1c);
    if ((r_u32(descriptor + 0x30) & 0x2000) != 0) G3Function8003E314(object, frame);
    sint32 remaining = r_s16(state + 0x10);
    uint32 remaining_bits = r_u16(state + 0x10);
    if (remaining > 0 && finished && r_s16(state + 0x18) == 0) w_u16(state + 0x10, remaining_bits - 1);
    phase = (uint32)r_s16(state + 0x18);
    switch (phase)
    {
    case 0:
        if (r_s16(state + 0x12) >= 0 && r_s16(state + 0x10) <= 0)
        {
            variant = (uint32)r_s16(state + 0x1a);
            mode = r_u32(object + 0x150);
            row = G3BirdTable(variant, mode, 3, module_base);
            if (r_s16(row) < 0) goto change_state;
            w_u16(state + 0x18, 3);
        }
        else
        {
            if (!finished) return;
            if ((G3SdkRandom80027FA8() & 7) == 0)
            {
                variant = (uint32)r_s16(state + 0x1a);
                mode = r_u32(object + 0x150);
                row = G3BirdTable(variant, mode, 1, module_base);
                if (r_s16(row) >= 0) w_u16(state + 0x18, 1);
                else
                {
                    variant = (uint32)r_s16(state + 0x1a);
                    mode = r_u32(object + 0x150);
                    row = G3BirdTable(variant, mode, 0, module_base);
                }
            }
        }
        goto apply_row;
    case 1:
    case 2:
        if (!finished) return;
        variant = (uint32)r_s16(state + 0x1a);
        mode = r_u32(object + 0x150);
        row = G3BirdTable(variant, mode, 0, module_base);
        w_u16(state + 0x18, 0);
        goto apply_row;
    case 3:
        if (finished) goto change_state;
        return;
    default: return;
    }
apply_row:
    w_u16(object + 0x6e, r_u16(row));
    w_u16(object + 0x5e, r_u16(row + 2));
    return;
change_state:
    G3BirdSetMode(object, (uint32)r_s16(state + 0x12), 0, module_base, frame);
}
