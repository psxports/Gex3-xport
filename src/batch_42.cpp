#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32 G3FallingObjectEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_50");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3FallingObjectInitialize(object, game, module_base, frame);
        return 1;
    case 2:
        G3FallingObjectUpdate(object, game, module_base, frame);
        return 1;
    case 3:
        G3FallingObjectCollide(object, game, frame);
        return 1;
    default:
        return 0;
    }
}

void G3FallingObjectConfigure(uint32 state, uint32 commands, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000088, "G3L_50");
    uint32 frame = guest_stack - 0x20;
    uint32 value = G3Function80085AB0(commands, module_base + 0x6a0, frame);
    if (value != 0)
        w_u16(state + 0x18, r_u16(value));
    value = G3Function80085AB0(commands, module_base + 0x6ac, frame);
    if (value != 0)
        w_u16(state + 0x1c, r_u16(value));
    value = G3Function80085AB0(commands, module_base + 0x6b8, frame);
    if (value != 0)
        w_u16(state + 0x1e, r_u16(value));
    value = G3Function80085AB0(commands, module_base + 0x6c4, frame);
    if (value != 0)
        w_u32(state + 0x14, G3Function8003AA48(value));
}

void G3FallingObjectInitialize(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000138, "G3L_50");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x128;
    uint32 settings = object + 0x140;
    w_u16(settings, 500);
    w_u16(settings + 4, 0xfffb);
    w_u16(settings + 2, 0);
    w_u16(settings + 6, 0xffe2);
    uint32 commands = r_u32(object + 0x24);
    if (commands != 0)
        G3FallingObjectConfigure(state, commands, module_base, frame);
    uint32 resource = r_u32(object + 0x1c);
    w_u32(resource, r_u32(resource) | 0x400);
    uint32 flags = r_u32(object + 0x10);
    uint32 radius = r_u16(object + 0x140);
    w_u32(object + 0x10, flags | 0x40);
    w_u16(object + 0x14e, radius);
    w_u32(object + 0x10c, (uint32)r_s16(settings + 6));
    uint32 acceleration = (uint32)r_s16(settings + 4);
    w_u32(object + 0x118, acceleration);
    uint32 effect_resource = G3Function8003AA48(module_base + 0x6d0);
    w_u32(state + 0xc, effect_resource);
    w_u32(state + 0x10, 3);
    uint32 owner = r_u32(object + 0x2c);
    w_u16(object + 0xe0, 0x8000);
    if (owner != 0)
    {
        uint32 target = r_u32(owner + 0xec);
        if (target != 0)
            G3DispatchStateEvent(target, owner, game, 2014, 30, frame);
    }
}

void G3FallingObjectEmit(uint32 position, uint32 resource, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000022c, "G3L_50");
    uint32 frame = guest_stack - 0x50;
    if (resource == 0)
        return;
    uint32 models = r_u32(resource + 0xc);
    uint32 descriptor = r_u32(models);
    uint32 vector = module_base + 0x6dc;
    for (uint32 index = 0; index < 6; ++index)
    {
        w_u16(frame + 0x28, r_u16(vector) << 1);
        w_u16(frame + 0x2a, r_u16(vector + 2) << 1);
        uint32 z = r_u16(vector + 4);
        uint32 pool = r_u32(0x800a27fc);
        w_u16(frame + 0x2c, z << 1);
        w_u32(frame + 0x10, 0);
        w_u32(frame + 0x14, pool);
        w_u32(frame + 0x18, 0x80044bc4);
        w_u32(frame + 0x1c, 0x80041de0);
        w_u32(frame + 0x20, 15);
        uint32 data = r_u32(descriptor + 0x14);
        uint32 effect = G3Function8004257C(descriptor, data, position, frame + 0x28, frame);
        uint32 animation = r_u32(descriptor + 0x1c);
        if (animation != 0 && effect != 0)
        {
            uint32 flags = r_u32(effect + 0xc);
            uint32 source = r_u32(effect + 8);
            w_u32(effect + 0xc, flags | 4);
            uint32 a = r_u32(source);
            uint32 b = r_u32(source + 4);
            uint32 c = r_u32(source + 8);
            uint32 d = r_u32(source + 0xc);
            w_u32(effect + 0x60, a);
            w_u32(effect + 0x64, b);
            w_u32(effect + 0x68, c);
            w_u32(effect + 0x6c, d);
            animation = r_u32(descriptor + 0x1c);
            w_u16(effect + 0x74, 0);
            w_u32(effect + 0x70, animation + 4);
        }
        vector += 8;
    }
}

void G3FallingObjectUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000035c, "G3L_50");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x128;
    uint32 settings = object + 0x140;
    uint32 mode = r_u32(state);
    if (mode == 0)
    {
        uint32 velocity = r_u32(object + 0x10c);
        uint32 acceleration = r_u32(object + 0x118);
        w_u32(object + 0x10c, velocity + acceleration);
        uint32 z = r_u16(object + 0x5c);
        uint32 delta = r_u16(object + 0x10c);
        w_u16(object + 0x5c, z + delta);
        uint32 linked = r_u32(state + 8);
        if (linked != 0)
            w_u16(linked + 0x5c, r_u16(linked + 0x5c) + delta);
        sint32 height = r_s16(object + 0x5c);
        sint32 floor = r_s16(object + 0xe0);
        if (height < floor)
        {
            uint32 parameters = r_u32(state + 0x14);
            G3Function800455A0(object + 0xdc, 32, 30, parameters, frame);
            uint32 effect_resource = r_u32(state + 0xc);
            G3FallingObjectEmit(object + 0x58, effect_resource, module_base, frame);
            w_u32(state + 4, 30);
            height = r_s16(object + 0x5c);
            floor = r_s16(object + 0xe0);
            w_u32(state, 1);
            sint32 sum = height + floor;
            w_u16(object + 0x5c, (uint32)((sum + (sum < 0 ? 1 : 0)) >> 1));
            linked = r_u32(state + 8);
            if (linked != 0)
            {
                w_u16(linked + 0x58, r_u16(object + 0xdc));
                linked = r_u32(state + 8);
                w_u16(linked + 0x5a, r_u16(object + 0xde));
                linked = r_u32(state + 8);
                w_u16(linked + 0x5c, r_u16(object + 0xe0));
            }
        }
    }
    else if (mode == 1)
    {
        bool finished = false;
        uint32 resource = r_u32(object + 0x1c);
        if (r_s16(resource + 0xa) != 0)
        {
            G3Function80029F0C(object, 0xffffffff, frame);
            resource = r_u32(object + 0x1c);
            uint32 selected = r_u16(object + 0x6e);
            uint32 animations = r_u32(resource + 0x10);
            uint32 animation = r_u32(animations + (selected << 2));
            sint32 last = r_s16(animation + 2) - 1;
            finished = r_s16(object + 0x5e) == last;
        }
        else
            finished = true;
        if (finished)
        {
            if (r_s16(settings + 2) == 2)
            {
                w_u16(object + 0x6e, 1);
                uint32 repeats = r_u32(state + 0x10) - 1;
                w_u32(state + 0x10, repeats);
                if ((sint32)repeats > 0)
                    return;
            }
            w_u32(state, 2);
        }
    }
    else if (mode == 2)
    {
        uint32 linked = r_u32(state + 8);
        if (linked != 0)
        {
            w_u16(linked + 0x58, r_u16(object + 0xdc));
            linked = r_u32(state + 8);
            w_u16(linked + 0x5a, r_u16(object + 0xde));
            linked = r_u32(state + 8);
            w_u16(linked + 0x5c, r_u16(object + 0xe0));
        }
        uint32 x = r_u16(object + 0x68);
        uint32 y = r_u16(object + 0x6a);
        w_u16(object + 0x68, x - 800);
        uint32 z = r_u16(object + 0x6c);
        w_u16(object + 0x6a, y - 800);
        uint32 radius = r_u16(object + 0x140);
        sint32 new_x = r_s16(object + 0x68);
        w_u16(object + 0x6c, z - 800);
        uint32 old_radius = r_u16(object + 0x14e);
        uint32 next_radius = old_radius - (uint32)((sint32)(radius << 16) >> 19);
        w_u16(object + 0x14e, next_radius);
        if (new_x < 0 || (sint16)next_radius < 0)
        {
            w_u16(object + 0x68, 0);
            w_u16(object + 0x6a, 0);
            w_u16(object + 0x6c, 0);
            w_u16(object + 0x14e, 1);
            G3Function8002A9AC(object, frame);
        }
    }
}

void G3FallingObjectCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000604, "G3L_50");
    uint32 frame = guest_stack - 0x20;
    uint32 contact = r_u32(object + 0xb0);
    uint32 other = r_u32(contact + 0x14);
    uint32 player = r_u32(game + 0x50);
    if (other != player || r_u32(object + 0x128) != 0)
        return;
    if (r_s16(other + 0x6c) != 0x1000)
        return;
    uint32 owner = r_u32(object + 0x2c);
    if (owner != 0)
    {
        uint32 target = r_u32(owner + 0xec);
        if (target != 0)
            G3DispatchStateEvent(target, owner, game, 2014, 60, frame);
    }
    G3Function800740BC(object, game, 60, frame);
}

uint32 G3FanEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_77");
    uint32 frame = guest_stack - 0x18;
    uint32 index = message - 1;
    if (index >= 22) return 0;
    uint32 target = r_u32(module_base + 0x12fc + index * 4) - module_base;
    switch (target)
    {
    case 0x3c: G3FanInitialize(object); return 1;
    case 0x4c: G3FanUpdate(object, game, frame); return 1;
    case 0x5c: G3FanRelease(object); return 1;
    case 0x6c:
    {
        uint32 resource = G3Function8003AA48(module_base + 0x12ec);
        if (resource != 0)
        {
            uint32 data = r_u32(resource + 0x1c);
            if (data != 0) w_u32(data, 0);
        }
        return 1;
    }
    case 0x98: G3FanDisengage(object); return 1;
    case 0xa8: return 0;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "Unresolved Fan event target %08X\n", target + module_base);
#endif
        }
        abort();
    }
}

uint32 G3FanEngage(uint32 object)
{
    FUNCTION_MARKER(0x000000c0, "G3L_77");
    uint32 descriptor = r_u32(object + 0x1c);
    sint32 engaged = r_s16(object + 0x14e);
    uint32 counter = r_u32(descriptor + 0x1c);
    if (engaged != 0) return 0;
    w_u16(object + 0x14e, 1);
    if (counter != 0)
    {
        uint32 count = r_u32(counter) + 1;
        w_u32(counter, count);
        if (count == 1)
        {
            uint32 player = r_u32(0x8009efc4);
            w_u32(player + 0x134, r_u32(player + 0x134) | 0x201);
        }
    }
    return 1;
}

uint32 G3FanDisengage(uint32 object)
{
    FUNCTION_MARKER(0x00000120, "G3L_77");
    uint32 descriptor = r_u32(object + 0x1c);
    sint32 engaged = r_s16(object + 0x14e);
    uint32 counter = r_u32(descriptor + 0x1c);
    if (engaged == 0) return 0;
    w_u16(object + 0x14e, 0);
    if (counter != 0)
    {
        uint32 count = r_u32(counter) - 1;
        w_u32(counter, count);
        if (count == 0)
        {
            uint32 player = r_u32(0x8009efc4);
            w_u32(player + 0x134, r_u32(player + 0x134) & 0xfffffdfe);
        }
    }
    return 1;
}

void G3FanRelease(uint32 object)
{
    FUNCTION_MARKER(0x00000188, "G3L_77");
    uint32 handle = r_u32(object + 0x124);
    if (handle != 0)
    {
        G3Function8003F18C(handle);
        w_u32(object + 0x124, 0);
        G3FanDisengage(object);
    }
}

void G3FanInitialize(uint32 object)
{
    FUNCTION_MARKER(0x000001d8, "G3L_77");
    uint32 state = object + 0x104;
    uint32 settings = object + 0x13c;
    w_u32(object + 0x10, r_u32(object + 0x10) | 0x100c00);
    uint32 commands = r_u32(object + 0x28);
    w_u32(object + 0x150, 0);
    if (commands != 0)
    {
        uint32 words[4];
        for (uint32 i = 0; i != 4; ++i)
        {
            uint32 source = commands + i * 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            words[i] = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        }
        for (uint32 i = 0; i != 4; ++i)
        {
            uint32 destination = settings + i * 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, words[i] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, words[i] << (lane * 8), (15u << lane) & 15);
        }
        w_u16(object + 0x14c, r_u16(commands + 0x10));
    }
    else
    {
        w_u16(settings + 8, 0x118);
        w_u16(settings + 6, 0x800);
        w_u16(settings + 0xc, 0x10e);
        w_u16(settings + 0xe, 5);
        w_u16(settings + 4, 0);
        w_u16(settings + 2, 0);
        w_u16(settings + 0xa, 0xbd6);
        w_u16(settings + 0x10, 0);
    }
    sint32 response = r_s16(settings + 0xe);
    uint32 clamped = r_u16(settings + 0xe);
    if (response < 3) clamped = 3;
    else if (response >= 5) clamped = 5;
    sint32 off_time = r_s16(settings + 2);
    w_u16(settings + 0xe, clamped);
    if (off_time == 0) w_u32(state + 4, (uint32)r_s16(settings + 0xc));
    if (r_s16(settings) != 0) w_u32(object + 0x150, 1);
    else w_u32(state, (uint32)r_s16(settings + 2));
    w_u16(state + 8, 0x40);
    sint32 rate = r_s16(settings + 0xa);
    sint32 whole = rate / 100;
    sint32 fraction = rate % 100;
    if (whole >= 100) whole = 99;
    w_u16(state + 0xe, (uint32)((whole * 256) / 30));
    uint32 main_rate = r_u16(state + 0xe);
    w_u16(state + 0xc, 0);
    w_u16(state + 0x10, 0);
    w_u16(state + 0x18, (uint32)((sint32)(sint16)main_rate >> 3));
    sint32 extra_rate = (fraction * 256) / 30;
    w_u16(state + 0x12, (uint32)extra_rate);
    w_u16(state + 0x1a, (uint32)((sint32)(sint16)extra_rate >> 3));
    sint32 delay = r_s16(settings + 0x10);
    whole = delay / 100;
    fraction = delay % 100;
    if (whole >= 100) whole = 99;
    w_u16(settings + 0x10, (uint32)(whole * 256 + fraction));
    w_u32(state + 0x14, 0 - (uint32)whole);
    sint32 width = r_s16(settings + 6);
    sint32 height = r_s16(settings + 8);
    uint32 radius = (uint32)(height < width ? width : height);
    w_u32(state + 0x1c, radius * radius);
    if (r_s16(object + 0x62) != 0)
    {
        w_u16(object + 0x64, 0);
        w_u16(object + 0x60, 0);
    }
    w_u32(state + 0x20, 0);
    w_u32(state + 0x2c, 0);
    w_u32(state + 0x28, 100);
    uint32 random = G3SdkRandom80027FA8();
    w_u32(state + 0x24, (random & 0x3f) - 0x20);
    w_u32(state + 0x30, 0);
    w_u32(state + 0x34, 0);
    w_u16(state + 0x4a, 0);
}

void G3FanFinish(void)
{
    FUNCTION_MARKER(0x000012e4, "G3L_77");
}

void G3FanUpdate(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000004c0, "G3L_77");
    uint32 frame = guest_stack - 0x30;
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 player = r_u32(game + 0x50);
    uint32 origin = r_u16(object + 0x58);
    uint32 x = origin - r_u16(player + 0x58);
    sint32 count = r_s16(descriptor + 8);
    uint32 resources = r_u32(descriptor + 0xc);
    uint32 model = r_u32(resources + (uint32)count * 4 - 4);
    w_u16(frame + 0x10, x);
    player = r_u32(game + 0x50);
    origin = r_u16(object + 0x5a);
    uint32 y = origin - r_u16(player + 0x5a);
    w_u16(frame + 0x12, y);
    player = r_u32(game + 0x50);
    origin = r_u16(object + 0x5c);
    uint32 z = origin - r_u16(player + 0x5c);
    uint32 state = object + 0x104;
    uint32 settings = object + 0x13c;
    w_u16(frame + 0x14, z);
    sint32 radius_squared = r_s32(state + 0x1c);
    x = (uint32)(sint32)(sint16)x;
    y = (uint32)(sint32)(sint16)y;
    z = (uint32)(sint32)(sint16)z;
    sint32 distance = (sint32)(x * x + y * y + z * z) >> 8;
    sint32 far_threshold = (radius_squared >> 5) + (radius_squared >> 8);
    if (distance < (radius_squared >> 6))
        w_u32(object + 0x10, r_u32(object + 0x10) | 0x400);
    else if (distance > far_threshold)
        w_u32(object + 0x10, r_u32(object + 0x10) & 0xfffffbff);
    uint32 mode = r_u32(object + 0x150);
    if (mode == 1)
    {
        uint32 timer = r_u32(state) + 1;
        w_u32(state, timer);
        if ((sint32)timer >= r_s16(settings))
        {
            w_u32(state, (uint32)r_s16(settings + 2));
            w_u32(object + 0x150, 0);
        }
    }
    else if (mode == 0)
    {
        uint32 timer = r_u32(state) + 1;
        w_u32(state, timer);
        if ((sint32)timer >= r_s16(settings + 2))
        {
            w_u32(object + 0x150, 2);
            w_u32(state, 0);
            w_u32(state + 0x34, 1);
        }
    }
    else if (mode == 2)
    {
        if (r_s16(settings + 2) != 0)
        {
            sint32 duration = r_s16(settings + 4);
            if (r_s32(state) < (sint32)(sint16)(duration / 3))
            {
                uint32 speed = r_u32(state + 4);
                sint32 maximum = r_s16(settings + 0xc);
                uint32 accelerated = speed + 14;
                if ((sint32)accelerated < maximum) w_u32(state + 4, accelerated);
                else
                {
                    w_u32(state + 4, (uint32)maximum);
                    w_u32(state + 0x30, 1);
                }
            }
            if (r_s16(settings + 2) != 0)
            {
                duration = r_s16(settings + 4);
                if ((duration * 2) / 3 < r_s32(state))
                {
                    uint32 decelerated = r_u32(state + 4) - 14;
                    if ((sint32)decelerated > 0)
                    {
                        uint32 handle = r_u32(state + 0x20);
                        w_u32(state + 4, decelerated);
                        w_u32(state + 0x30, 0);
                        if (handle != 0)
                        {
                            w_u32(state + 0x34, 2);
                            G3Function8003F18C(handle);
                            w_u32(state + 0x20, 0);
                        }
                    }
                    else w_u32(state + 4, 0);
                }
            }
        }
        uint32 timer = r_u32(state) + 1;
        w_u32(state, timer);
        if (r_s16(settings + 2) != 0 && (sint32)timer >= r_s16(settings + 4))
        {
            w_u32(object + 0x150, 0);
            w_u32(state, 0);
        }
        uint32 increment;
        uint32 accumulated;
        if (r_s16(state + 0x18) != 0)
        {
            sint32 random = (sint32)G3SdkRandom80027FA8();
            sint32 divisor = r_s16(state + 0x18) * 2;
            uint32 remainder = divisor == 0 ? (uint32)random : (uint32)((sint64)random % divisor);
            increment = r_u16(state + 0xe) + remainder;
            uint32 spread = r_u16(state + 0x18);
            accumulated = r_u16(state + 0xc);
            increment -= spread;
        }
        else
        {
            accumulated = r_u16(state + 0xc);
            increment = r_u16(state + 0xe);
        }
        w_u16(state + 0xc, accumulated + increment);
        uint32 rate = r_u16(state + 0xc);
        sint32 emissions = (sint32)(sint16)rate >> 8;
        w_u16(state + 0xc, rate & 0xff);
        sint32 maximum = r_s16(settings + 0xc);
        if (r_s32(state + 4) != maximum) emissions >>= 1;
        if (emissions > 0)
        {
            sint32 variants = r_s16(model + 4) - 1;
            if (variants >= 2)
            {
                do
                {
                    --emissions;
                    sint32 random = (sint32)G3SdkRandom80027FA8();
                    G3FanEmit(object, state, (uint32)(random % variants), frame);
                } while (emissions != 0);
            }
            else
            {
                do
                {
                    G3FanEmit(object, state, 0, frame);
                    --emissions;
                } while (emissions != 0);
            }
        }
        G3FanAffectPlayer(object, game, state, frame);
    }
    G3FanFinish();
}

void G3FanEmit(uint32 object, uint32 state, uint32 index, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000008b0, "G3L_77");
    uint32 frame = guest_stack - 0x68;
    uint32 descriptor = r_u32(object + 0x1c);
    sint32 radius = r_s16(state + 0x40);
    sint32 count = r_s16(descriptor + 8);
    uint32 resources = r_u32(descriptor + 0xc);
    uint32 model = r_u32(resources + (uint32)count * 4 - 4);
    uint32 records = r_u32(model + 0x14);
    w_u16(frame + 0x38, 0);
    uint32 data = records + index * 12;
    sint32 random = (sint32)G3SdkRandom80027FA8();
    sint32 divisor = radius * 2;
    uint32 remainder = divisor == 0 ? (uint32)random : (uint32)((sint64)random % divisor);
    w_u16(frame + 0x3a, remainder - (uint32)radius);
    random = (sint32)G3SdkRandom80027FA8();
    remainder = divisor == 0 ? (uint32)random : (uint32)((sint64)random % divisor);
    w_u16(frame + 0x3c, remainder - (uint32)radius);
    uint32 matrix = r_u32(object + 0x30);
    ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(frame + 0x38, 8), (SVECTOR *)psx_addr(frame + 0x40, 6));
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        uint32 coordinate = r_u16(frame + 0x40 + axis * 2);
        uint32 origin = r_u16(object + 0x58 + axis * 2);
        w_u16(frame + 0x40 + axis * 2, coordinate + origin);
    }
    random = (sint32)G3SdkRandom80027FA8();
    sint32 speed = (sint32)(sint16)(random % 20 + 50);
    sint32 extent = r_s16(state + 0x3e);
    sint32 tilt = r_s16(object + 0x62);
    uint32 lifetime = (uint32)(extent / speed);
    if (tilt == 0)
    {
        uint32 sine = G3Function8005B470((uint32)r_s16(object + 0x64));
        uint32 current_speed = r_u32(state + 4);
        sint32 product = (sint32)((uint32)speed * current_speed);
        divisor = r_s16(state + 0x44);
        uint32 scaled = divisor == 0 ? (product < 0 ? 1 : 0xffffffff) : (uint32)((sint64)product / divisor);
        w_u16(frame + 0x28, (uint32)((sint32)(scaled * sine) >> 12));
        uint32 cosine = G3Function8005B590((uint32)r_s16(object + 0x64));
        current_speed = r_u32(state + 4);
        product = (sint32)((uint32)speed * current_speed);
        divisor = r_s16(state + 0x44);
        scaled = divisor == 0 ? (product < 0 ? 1 : 0xffffffff) : (uint32)((sint64)product / divisor);
        w_u16(frame + 0x2c, 0);
        w_u16(frame + 0x2a, (uint32)((sint32)(scaled * cosine) >> 12));
    }
    else
    {
        w_u16(frame + 0x28, 0);
        w_u16(frame + 0x2a, 0);
        uint32 cosine = G3Function8005B590(0 - (uint32)r_s16(object + 0x62));
        uint32 current_speed = r_u32(state + 4);
        sint32 product = (sint32)((uint32)speed * current_speed);
        divisor = r_s16(state + 0x44);
        uint32 scaled = divisor == 0 ? (product < 0 ? 1 : 0xffffffff) : (uint32)((sint64)product / divisor);
        w_u16(frame + 0x2c, (uint32)((sint32)(scaled * cosine) >> 12));
    }
    w_u16(frame + 0x30, 0);
    w_u16(frame + 0x32, 0);
    w_u16(frame + 0x34, 0);
    w_u32(frame + 0x10, frame + 0x30);
    w_u32(frame + 0x18, 0x80042230);
    w_u32(frame + 0x1c, 0x80041de0);
    uint32 owner = r_u32(0x800a27fc);
    w_u32(frame + 0x20, (uint32)(sint32)(sint16)lifetime);
    w_u32(frame + 0x14, owner);
    G3Function8004257C(model, data, frame + 0x40, frame + 0x28, frame);
}

void G3FanPlayerParticles(uint32 object, uint32 game, uint32 state, uint32 position, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000b34, "G3L_77");
    uint32 frame = guest_stack - 0x90;
    uint32 descriptor = r_u32(object + 0x1c);
    sint32 count = r_s16(descriptor + 8);
    uint32 resources = r_u32(descriptor + 0xc);
    uint32 model = r_u32(resources + (uint32)count * 4 - 4);
    sint32 variants = r_s16(model + 4);
    sint32 radius = r_s16(state + 0x40);
    uint32 records = r_u32(model + 0x14);
    w_u32(frame + 0x50, records + (uint32)variants * 12 - 12);
    sint32 divisor = r_s16(state + 0x44);
    sint32 current_speed = r_s32(state + 4);
    uint32 scale = divisor == 0 ? (current_speed < 0 ? 1 : 0xffffffff) : (uint32)((sint64)current_speed / divisor);
    sint32 spread = r_s16(state + 0x1a);
    w_u32(frame + 0x60, scale);
    uint32 increment;
    uint32 accumulated;
    if (spread != 0)
    {
        sint32 random = (sint32)G3SdkRandom80027FA8();
        divisor = r_s16(state + 0x1a) * 2;
        uint32 remainder = divisor == 0 ? (uint32)random : (uint32)((sint64)random % divisor);
        increment = r_u16(state + 0x12) + remainder;
        uint32 fresh_spread = r_u16(state + 0x1a);
        accumulated = r_u16(state + 0x10);
        increment -= fresh_spread;
    }
    else
    {
        accumulated = r_u16(state + 0x10);
        increment = r_u16(state + 0x12);
    }
    w_u16(state + 0x10, accumulated + increment);
    uint32 rate = r_u16(state + 0x10);
    sint32 emissions = (sint32)(sint16)rate >> 8;
    w_u16(state + 0x10, rate & 0xff);
    if (emissions <= 0) return;
    uint32 player = r_u32(game + 0x50);
    sint32 vertical_speed = r_s32(player + 0x10c) >> 2;
    w_u32(frame + 0x64, (uint32)vertical_speed);
    w_u16(frame + 0x58, (uint32)((emissions * 2) / 3));
    do
    {
        w_u16(frame + 0x38, r_u16(position));
        sint32 random = (sint32)G3SdkRandom80027FA8();
        uint32 coordinate = r_u16(position + 2);
        w_u16(frame + 0x3a, coordinate + (uint32)(random % 256) - 0x80);
        random = (sint32)G3SdkRandom80027FA8();
        coordinate = r_u16(position + 4);
        uint32 z = coordinate + (uint32)(random % 256) - 0x80;
        w_u16(frame + 0x3c, z);
        if (r_s16(object + 0x62) != 0)
            w_u16(frame + 0x38, r_u16(frame + 0x38) - 0x80);
        uint32 threshold = r_u16(frame + 0x58);
        if ((uint32)(uint16)emissions == threshold) radius >>= 2;
        if (r_s16(frame + 0x3a) <= radius && (sint32)(sint16)z <= radius)
        {
            uint32 matrix = r_u32(object + 0x30);
            ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(frame + 0x38, 8), (SVECTOR *)psx_addr(frame + 0x48, 6));
            for (uint32 axis = 0; axis != 3; ++axis)
            {
                uint32 origin = r_u16(object + 0x58 + axis * 2);
                uint32 transformed = r_u16(frame + 0x48 + axis * 2);
                w_u16(frame + 0x40 + axis * 2, origin + transformed);
            }
            uint32 random_bits = G3SdkRandom80027FA8();
            scale = r_u32(frame + 0x60);
            uint32 speed = ((random_bits & 7) + 10) * scale;
            sint32 tilt = r_s16(object + 0x62);
            if (tilt != 0)
            {
                w_u16(frame + 0x28, 0);
                w_u16(frame + 0x2a, 0);
                uint32 cosine = G3Function8005B590(0 - (uint32)r_s16(object + 0x62));
                uint32 product = (uint32)(sint32)(sint16)speed * (cosine * 6);
                sint32 rise = (sint32)product >> 12;
                uint32 player_speed = r_u32(frame + 0x64);
                w_u16(frame + 0x2c, (uint32)rise + player_speed);
            }
            else
            {
                uint32 z_offset = (uint32)r_s16(frame + 0x3c);
                uint32 y_offset = (uint32)r_s16(frame + 0x3a);
                uint32 direction = G3Function8002F240(z_offset, y_offset, frame);
                uint32 sine = G3Function8005B470((uint32)r_s16(object + 0x64));
                uint32 cosine = G3Function8005B590((uint32)r_s16(object + 0x64));
                speed = (uint32)(sint32)(sint16)speed;
                w_u16(frame + 0x28, (uint32)((sint32)(speed * (sine + cosine)) >> 12));
                cosine = G3Function8005B590((uint32)r_s16(object + 0x64));
                direction = (uint32)(sint32)(sint16)direction;
                sine = G3Function8005B470(direction);
                w_u16(frame + 0x2a, (uint32)((sint32)(speed * (cosine + sine * 2)) >> 12));
                cosine = G3Function8005B590(direction);
                w_u16(frame + 0x2c, (uint32)((sint32)(speed * (cosine * 2)) >> 12));
            }
            w_u16(frame + 0x34, 0xffff);
            w_u16(frame + 0x30, 0);
            w_u16(frame + 0x32, 0);
            w_u32(frame + 0x10, frame + 0x30);
            w_u32(frame + 0x18, 0x80042230);
            uint32 owner = r_u32(0x800a27fc);
            w_u32(frame + 0x1c, 0x80041de0);
            w_u32(frame + 0x14, owner);
            uint32 speed_bits = r_u32(state + 4);
            divisor = r_s16(state + 0x44);
            sint32 product = (sint32)(speed_bits * 15);
            uint32 lifetime = divisor == 0 ? (product < 0 ? 1 : 0xffffffff) : (uint32)((sint64)product / divisor);
            uint32 data = r_u32(frame + 0x50);
            w_u32(frame + 0x20, (uint32)(sint32)(sint16)lifetime);
            G3Function8004257C(model, data, frame + 0x40, frame + 0x28, frame);
        }
        --emissions;
    } while ((emissions & 0xffff) != 0);
}

void G3FanAffectPlayer(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000f30, "G3L_77");
    uint32 frame = guest_stack - 0x78;
    uint32 affected = 0;
    uint32 player = r_u32(0x8009efc4);
    uint32 origin = r_u16(object + 0x58);
    uint32 coordinate = r_u16(player + 0x58);
    uint32 matrix = r_u32(object + 0x30);
    w_u16(frame + 0x10, coordinate - origin);
    coordinate = r_u16(player + 0x5a);
    origin = r_u16(object + 0x5a);
    w_u16(frame + 0x12, coordinate - origin);
    origin = r_u16(object + 0x5c);
    coordinate = r_u16(player + 0x5c);
    w_u16(frame + 0x14, coordinate - (origin - 0x80));
    TransposeMatrix((MATRIX *)psx_addr(matrix, 18), (MATRIX *)psx_addr(frame + 0x20, 18));
    ApplyMatrixSV((MATRIX *)psx_addr(frame + 0x20, 20), (SVECTOR *)psx_addr(frame + 0x10, 8), (SVECTOR *)psx_addr(frame + 0x18, 6));
    sint32 radius = r_s16(state + 0x40);
    sint32 y = r_s16(frame + 0x1a);
    if (y >= -radius && y <= radius)
    {
        sint32 z = r_s16(frame + 0x1c);
        if (z >= -radius && z <= radius)
        {
            if (r_s16(object + 0x62) != 0)
            {
                sint32 extent = r_s16(state + 0x3e);
                sint32 x = r_s16(frame + 0x18);
                if (x >= -(extent >> 1) && x <= extent * 2)
                {
                    if (G3FanEngage(object) == 0)
                    {
                        sint32 position = r_s16(frame + 0x18);
                        sint32 previous = r_s16(state + 0xa);
                        affected = (uint32)position - (uint32)previous;
                    }
                    w_u16(state + 0xa, r_u16(frame + 0x18));
                    sint32 maximum = r_s16(state + 0x44);
                    if (r_s32(state + 4) != maximum) return;
                    uint32 old_timer = r_u32(state + 0x14);
                    extent = r_s16(state + 0x3e);
                    x = r_s16(frame + 0x18);
                    uint32 timer = old_timer + 1;
                    uint32 distance = (uint32)extent - (uint32)x;
                    w_u32(state + 0x14, timer);
                    uint32 force;
                    if ((sint32)old_timer < 0)
                    {
                        force = r_u8(state + 0x48);
                        if (timer == 0) w_u32(state + 0x14, 12);
                    }
                    else
                    {
                        uint32 wave = G3Function8005B590(timer * 80);
                        sint32 divided = (sint32)wave / 70;
                        sint32 displacement = (sint32)((uint32)divided + distance) >> 5;
                        if (displacement < -10) displacement = -10;
                        if (displacement > 8) displacement = 8;
                        force = (uint32)displacement;
                    }
                    uint32 shift = (uint32)r_s16(state + 0x46);
                    player = r_u32(0x8009efc4);
                    uint32 target = r_u32(player + 0xec);
                    uint32 payload = force - (uint32)((sint32)affected >> (shift & 31));
                    if (target != 0)
                    {
                        G3DispatchStateEvent(target, player, game, 0x3ee, payload, frame);
                        player = r_u32(0x8009efc4);
                        target = r_u32(player + 0xec);
                        if (target != 0) G3DispatchStateEvent(target, player, game, 0x3ed, 6, frame);
                    }
                    player = r_u32(0x8009efc4);
                    sint32 speed = r_s32(player + 0x10c);
                    sint32 limit = r_s32(player + 0x124);
                    affected = 1;
                    if (limit < speed) w_u32(player + 0x10c, (uint32)limit);
                }
            }
            else
            {
                sint32 x = r_s16(frame + 0x18);
                if (x > 0)
                {
                    sint32 extent = r_s16(state + 0x3e);
                    if (x < extent || extent == 0)
                    {
                        player = r_u32(0x8009efc4);
                        if (r_u32(player + 0x150) == 1)
                            w_u32(player + 0x128, r_u32(player + 0x128) | 0x100);
                        else
                        {
                            G3FanEngage(object);
                            sint32 maximum = r_s16(state + 0x44);
                            if (r_s32(state + 4) != maximum) return;
                            uint32 heading = (uint32)r_s16(object + 0x64);
                            uint32 sine = G3Function8005B470(heading);
                            uint32 strength = (uint32)r_s16(state + 8);
                            w_u16(frame + 0x44, (uint32)((sint32)(sine * strength) >> 12));
                            uint32 cosine = G3Function8005B590(heading);
                            strength = (uint32)r_s16(state + 8);
                            affected = 1;
                            w_u16(frame + 0x48, 0);
                            w_u16(frame + 0x46, (uint32)((sint32)(cosine * strength) >> 12));
                            G3Function8006F55C(object, frame + 0x40, frame);
                        }
                    }
                }
            }
            if (affected != 0)
            {
                G3FanPlayerParticles(object, game, state, frame + 0x18, frame);
                player = r_u32(0x8009efc4);
                if (r_u32(player + 0x150) == 6)
                {
                    uint32 global_game = r_u32(0x80097048);
                    G3Function80051FE4(r_u32(global_game + 0x4c), frame);
                }
                return;
            }
        }
    }
    sint32 delay = (sint32)(sint16)r_u16(state + 0x48) >> 8;
    w_u32(state + 0x14, 0 - (uint32)delay);
    G3FanDisengage(object);
}

uint32 G3FishEvent(uint32 object, uint32 message, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_01");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3FishInitialize(object, frame);
        return 1;
    case 2:
        G3FishUpdate(object, frame);
        return 1;
    case 8:
        G3FishDamage(object, frame);
        return 1;
    default:
        return 0;
    }
}

void G3FishSelect(uint32 object, uint32 mode)
{
    FUNCTION_MARKER(0x00000088, "G3L_01");
    w_u32(object + 0x150, mode);
    w_u16(object + 0x6e, mode);
    w_u16(object + 0x5e, 0);
    uint32 state = object + 0x104;
    if (mode == 0)
        w_u32(state + 8, 1);
}

void G3FishInitialize(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000ac, "G3L_01");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    uint32 data = r_u32(object + 0x28);
    w_u32(state + 8, 0);
    w_u16(state + 0x12, 0);
    w_u16(state + 0x10, 30);
    w_u32(state + 0x14, 0x9c400);
    w_u32(state + 0x18, 0);
    if (data != 0)
        w_u16(state + 0x1c, r_u16(data + 0x0));
    else
    {
        uint32 intro = r_u32(object + 0x20);
        uint32 position = r_u16(intro + 0x10);
        w_u16(state + 0x1c, position - 0x400);
    }
    if (data != 0)
        w_u16(state + 0x22, r_u16(data + 0x6));
    else
    {
        uint32 intro = r_u32(object + 0x20);
        uint32 position = r_u16(intro + 0x10);
        w_u16(state + 0x22, position + 0x400);
    }
    if (data != 0)
        w_u16(state + 0x1e, r_u16(data + 0x2));
    else
    {
        uint32 intro = r_u32(object + 0x20);
        uint32 position = r_u16(intro + 0x12);
        w_u16(state + 0x1e, position - 0x400);
    }
    if (data != 0)
        w_u16(state + 0x24, r_u16(data + 0x8));
    else
    {
        uint32 intro = r_u32(object + 0x20);
        uint32 position = r_u16(intro + 0x12);
        w_u16(state + 0x24, position + 0x400);
    }
    if (data != 0)
        w_u16(state + 0x20, r_u16(data + 0x4));
    else
    {
        uint32 intro = r_u32(object + 0x20);
        uint32 position = r_u16(intro + 0x14);
        w_u16(state + 0x20, position - 0x400);
    }
    if (data != 0)
        w_u16(state + 0x26, r_u16(data + 0xa));
    else
    {
        uint32 intro = r_u32(object + 0x20);
        uint32 position = r_u16(intro + 0x14);
        w_u16(state + 0x26, position + 0x400);
    }
    if (data != 0)
        w_u16(state + 0xc, r_u16(data + 0xc));
    else
        w_u16(state + 0xc, 20);
    if (data != 0)
        w_u16(state + 0xe, r_u16(data + 0xe));
    else
        w_u16(state + 0xe, 60);
    uint32 remaining = G3RemainingPlacementCount80045FE8(object);
    w_u16(state + 0x28, remaining);
    w_u16(state + 0x2a, 1);
    if (data != 0)
        w_u16(data + 0x10, r_u16(state + 0x28));
    G3FishSelect(object, 0);
}

uint32 G3FishInsideXY(uint32 object, uint32 dx, uint32 dy)
{
    FUNCTION_MARKER(0x00000268, "G3L_01");
    uint32 state = object + 0x104;
    uint32 x = r_u16(object + 0x58);
    uint32 y = r_u16(object + 0x5a);
    x += dx;
    y += dy;
    sint32 minimum_x = r_s16(state + 0x1c);
    sint32 candidate_x = (sint16)x;
    if (candidate_x < minimum_x)
        return 0;
    if (r_s16(state + 0x22) < candidate_x)
        return 0;
    sint32 minimum_y = r_s16(state + 0x1e);
    sint32 candidate_y = (sint16)y;
    if (candidate_y < minimum_y)
        return 0;
    return r_s16(state + 0x24) < candidate_y ? 0 : 1;
}

void G3FishEvade(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000002d4, "G3L_01");
    uint32 frame = guest_stack - 0x58;
    uint32 intro = r_u32(object + 0x20);
    uint32 source = intro + 0x10;
    uint32 destination = frame + 0x18;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 lane = source & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
    uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
    uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
    uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
    uint32 tail = (uint32)r_s16(source + 4);
    uint32 output_high = destination + 3;
    uint32 output_high_lane = output_high & 3;
    uint32 output_lane = destination & 3;
    w_masked_u32(output_high & 0xfffffffc, value >> ((3 - output_high_lane) * 8), (1u << (output_high_lane + 1)) - 1);
    w_masked_u32(destination & 0xfffffffc, value << (output_lane * 8), (15u << output_lane) & 15);
    w_u16(destination + 4, tail);
    uint32 state = object + 0x104;
    if (G3FishInsideXY(object, 0, 0) != 0)
    {
        uint32 player = r_u32(0x8009efc4);
        uint32 x = r_u16(object + 0x58);
        uint32 player_x = r_u16(player + 0x58);
        w_u16(frame + 0x20, x - player_x);
        uint32 y = r_u16(object + 0x5a);
        uint32 player_y = r_u16(player + 0x5a);
        w_u16(frame + 0x24, 0);
        w_u16(frame + 0x22, y - player_y);
        G3Function8002F9A8(frame + 0x28, 400, frame + 0x20, frame);
        uint32 dx = (uint32)r_s16(frame + 0x28);
        uint32 dy = (uint32)r_s16(frame + 0x2a);
        uint32 dx_bits = r_u16(frame + 0x28);
        uint32 dy_bits = r_u16(frame + 0x2a);
        if (G3FishInsideXY(object, dx, dy) == 0)
        {
            uint32 negative_y = 0 - dy_bits;
            if (G3FishInsideXY(object, (uint32)(sint32)(sint16)negative_y, dx) != 0)
            {
                w_u16(frame + 0x28, negative_y);
                w_u16(frame + 0x2a, dx_bits);
            }
            else
            {
                uint32 negative_x = 0 - dx_bits;
                if (G3FishInsideXY(object, dy, (uint32)(sint32)(sint16)negative_x) != 0)
                {
                    w_u16(frame + 0x28, dy_bits);
                    w_u16(frame + 0x2a, negative_x);
                }
                else
                {
                    w_u16(frame + 0x28, negative_x);
                    w_u16(frame + 0x2a, negative_y);
                }
            }
        }
        x = r_u16(object + 0x58);
        dx = r_u16(frame + 0x28);
        w_u16(frame + 0x18, x + dx);
        y = r_u16(object + 0x5a);
        dy = r_u16(frame + 0x2a);
        w_u16(frame + 0x1a, y + dy);
        w_u16(frame + 0x1c, r_u16(object + 0x5c));
    }
    w_u16(frame + 0x30, r_u16(state + 0x12));
    w_u16(frame + 0x32, r_u16(state + 0x12));
    w_u16(frame + 0x34, r_u16(state + 0x12));
    w_u32(frame + 0x10, 0x100);
    G3Function8003639C(object, 0x80, frame + 0x30, frame + 0x18, 0x100, frame);
}

uint32 G3FishIdle(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000047c, "G3L_01");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    G3Function80029F0C(object, 0xffffffff, frame);
    if (r_s16(object + 0x5e) == 0 && r_u32(state + 0x18) != 0)
        return 1;
    w_u16(state + 0x12, r_u16(state + 0xc));
    G3FishRoam(object, frame);
    G3FishClamp(object);
    return 0;
}

uint32 G3FishAlert(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000004ec, "G3L_01");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    G3Function80029F0C(object, 0xffffffff, frame);
    if (r_s16(object + 0x5e) == 0 && r_u32(state + 0x18) == 0)
        return 0;
    w_u16(state + 0x12, r_u16(state + 0xe));
    G3FishEvade(object, frame);
    G3FishClamp(object);
    return 1;
}

uint32 G3FishDead(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000574, "G3L_01");
    uint32 frame = guest_stack - 0x18;
    uint32 resource = r_u32(object + 0x1c);
    uint32 animation = r_u16(object + 0x6e);
    uint32 animations = r_u32(resource + 0x10);
    uint32 descriptor = r_u32(animations + animation * 4);
    uint32 length = (uint32)r_s16(descriptor + 2);
    uint32 current = (uint32)r_s16(object + 0x5e);
    length -= 1;
    uint32 state = object + 0x104;
    if (current == length)
    {
        uint32 z = r_u16(object + 0x5c);
        uint32 rise = r_u16(state + 0x10);
        sint32 scale_x = r_s16(object + 0x68);
        w_u16(object + 0x5c, z + rise);
        uint32 scale_bits = r_u16(object + 0x68);
        if (scale_x > 96)
        {
            w_u16(object + 0x68, scale_bits - 48);
            uint32 scale_y = r_u16(object + 0x6a);
            uint32 scale_z = r_u16(object + 0x6c);
            w_u16(object + 0x6a, scale_y - 48);
            w_u16(object + 0x6c, scale_z - 48);
        }
        else
            G3Function80038798(object, 0, 0, frame);
    }
    else
        G3Function80029F0C(object, 0xffffffff, frame);
    return 2;
}

void G3FishUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000624, "G3L_01");
    uint32 frame = guest_stack - 0x18;
    uint32 player = r_u32(0x8009efc4);
    uint32 x = (uint32)r_s16(object + 0x58);
    uint32 player_x = (uint32)r_s16(player + 0x58);
    uint32 dx = player_x - x;
    uint32 player_y = (uint32)r_s16(player + 0x5a);
    uint32 y = (uint32)r_s16(object + 0x5a);
    uint32 dy = player_y - y;
    uint32 z = (uint32)r_s16(object + 0x5c);
    uint32 player_z = (uint32)r_s16(player + 0x5c);
    uint32 dz = player_z - z;
    uint32 mode = r_u32(object + 0x150);
    uint32 state = object + 0x104;
    uint32 range = r_u32(state + 0x14);
    uint32 distance = dx * dx + dy * dy + dz * dz;
    w_u32(state + 0x18, (sint32)distance < (sint32)range ? 1 : 0);
    uint32 next = mode;
    if (mode == 0)
        next = G3FishIdle(object, frame);
    else if (mode == 1)
        next = G3FishAlert(object, frame);
    else if (mode == 2)
        next = G3FishDead(object, frame);
    if (next != r_u32(object + 0x150))
        G3FishSelect(object, next);
}

void G3FishDamage(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000730, "G3L_01");
    uint32 frame = guest_stack - 0x48;
    uint32 mode = r_u32(object + 0x150);
    uint32 data = r_u32(object + 0x28);
    uint32 state = object + 0x104;
    if (mode == 2)
        return;
    sint32 remaining = r_s16(state + 0x28);
    uint32 remaining_bits = r_u16(state + 0x28);
    if (remaining <= 0)
        return;
    w_u16(state + 0x28, remaining_bits - 1);
    if (data != 0)
        w_u16(data + 0x10, r_u16(data + 0x10) - 1);
    if (r_s16(state + 0x28) == 0)
    {
        G3FishSelect(object, 2);
        uint32 intro = r_u32(object + 0x20);
        if (intro != 0)
            w_u32(intro + 0x18, r_u32(intro + 0x18) | 0x10);
    }
    if (r_s16(state + 0x2a) <= 0)
        return;
    uint32 index = 0;
    do
    {
        sint32 random_x = (sint32)G3SdkRandom80027FA8();
        uint32 dx = (uint32)(random_x % 8 + 4);
        sint32 random_y = (sint32)G3SdkRandom80027FA8();
        uint32 dy = (uint32)(random_y % 8 + 4);
        if ((G3SdkRandom80027FA8() & 1) == 0)
            dx = 0 - dx;
        if ((G3SdkRandom80027FA8() & 1) == 0)
            dy = 0 - dy;
        w_u32(frame + 0x10, 0);
        w_u32(frame + 0x14, 0);
        w_u32(frame + 0x18, 0xfffffffb);
        w_u32(frame + 0x1c, 100);
        w_u32(frame + 0x20, 100);
        w_u32(frame + 0x24, 100);
        G3Function800460E8(object, dx, dy, 0xffffffe0, frame);
        sint32 count = r_s16(state + 0x2a);
        ++index;
        if ((sint32)index >= count)
            break;
    } while (true);
}

void G3FishClamp(uint32 object)
{
    FUNCTION_MARKER(0x000008b8, "G3L_01");
    uint32 state = object + 0x104;
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        uint32 position = object + 0x58 + axis * 2;
        sint32 value = r_s16(position);
        sint32 minimum = r_s16(state + 0x1c + axis * 2);
        uint32 bound = r_u16(state + 0x1c + axis * 2);
        if (value < minimum)
            w_u16(position, bound);
        value = r_s16(position);
        sint32 maximum = r_s16(state + 0x22 + axis * 2);
        bound = r_u16(state + 0x22 + axis * 2);
        if (maximum < value)
            w_u16(position, bound);
    }
}

uint32 G3FishNearTarget(uint32 object, uint32 target, uint32 radius)
{
    FUNCTION_MARKER(0x00000984, "G3L_01");
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        uint32 center = (uint32)r_s16(target + axis * 2);
        sint32 value = r_s16(object + 0x58 + axis * 2);
        if ((sint32)(center - radius) >= value)
            return 0;
        if (value >= (sint32)(center + radius))
            return 0;
    }
    return 1;
}

void G3FishRoam(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000009f4, "G3L_01");
    uint32 frame = guest_stack - 0x40;
    uint32 state = object + 0x104;
    sint32 maximum_x = r_s16(state + 0x22);
    sint32 minimum_x = r_s16(state + 0x1c);
    sint32 minimum_y = r_s16(state + 0x1e);
    sint32 half_x = (maximum_x - minimum_x) / 2;
    sint32 maximum_y = r_s16(state + 0x24);
    sint32 width = (sint16)half_x;
    sint32 half_y = (maximum_y - minimum_y) / 2;
    if (width == 0)
        return;
    sint32 height = (sint16)half_y;
    if (height == 0)
        return;
    if (r_u32(state + 8) != 0)
    {
        w_u32(state + 8, 0);
        sint32 random_x = (sint32)G3SdkRandom80027FA8();
        uint32 remainder_x = (uint32)((sint64)random_x % width);
        uint32 x = r_u16(state + 0x1c);
        w_u16(object + 0x104, x + (uint32)(width / 2) + remainder_x);
        sint32 random_y = (sint32)G3SdkRandom80027FA8();
        uint32 remainder_y = (uint32)((sint64)random_y % height);
        uint32 y = r_u16(state + 0x1e);
        w_u16(state + 2, y + (uint32)(height / 2) + remainder_y);
        w_u16(state + 4, r_u16(object + 0x5c));
    }
    w_u16(frame + 0x18, r_u16(state + 0x12));
    w_u16(frame + 0x1a, r_u16(state + 0x12));
    w_u16(frame + 0x1c, r_u16(state + 0x12));
    w_u32(frame + 0x10, 0x100);
    G3Function8003639C(object, 0x80, frame + 0x18, state, 0x100, frame);
    uint32 arrived = G3FishNearTarget(object, state, 0x100);
    w_u32(state + 8, arrived);
}

uint32 G3LogoEvent(uint32 module_base, uint32 object, uint32 game, uint32 message, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_66");
    uint32 frame = guest_stack - 0x18;
    if (message == 1)
    {
        G3LogoInitialize(module_base, object, game);
        return 1;
    }
    if (message == 2)
    {
        G3LogoUpdate(module_base, object, game, frame);
        return 1;
    }
    return 0;
}

void G3LogoInitialize(uint32 module_base, uint32 object, uint32 game)
{
    FUNCTION_MARKER(0x0000005c, "G3L_66");
    uint32 resource = r_u32(object + 0x1c);
    uint32 data = r_u32(resource + 0x1c);
    uint32 state = object + 0x128;
    w_u32(object + 0x128, 0);
    w_u32(state + 4, 0);
    if (data != 0)
        w_u32(state + 8, r_u32(data));
    else
        w_u32(state + 8, 0);
    w_u16(state + 0xc, data != 0 ? r_u16(data + 4) : 0x100);
    w_u16(state + 0xe, data != 0 ? r_u16(data + 6) : 0x28);
    w_u16(state + 0x10, data != 0 ? r_u16(data + 8) : 0x444);
    w_u16(state + 0x12, data != 0 ? r_u16(data + 0xa) : 0x578);
    w_u32(object + 0xf0, module_base + 0x2b0);
    uint32 flags = r_u32(object + 0x10);
    uint32 secondary = r_u32(object + 0x18);
    w_u16(object + 0x5e, 0);
    w_u32(object + 0x10, flags | 0xc00);
    w_u32(object + 0x18, secondary | 0x400);
    if (r_u32(state + 8) == 0)
        w_u32(game + 0x69d8, object);
}

void G3LogoUpdate(uint32 module_base, uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000012c, "G3L_66");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x128;
    uint32 counter = r_u32(state + 4) + 1;
    w_u32(state + 4, counter);
    if ((sint32)counter >= 0x97 && r_u32(state + 8) == 2)
    {
        G3Function8002A9AC(object, frame);
        return;
    }
    G3LogoPosition(module_base, object, game, frame);
    uint32 resource = r_u32(object + 0x1c);
    uint32 sequences = r_u32(resource + 0x10);
    if (sequences == 0)
        return;
    uint32 sequence = r_u32(sequences);
    sint32 current = r_s16(object + 0x5e);
    sint32 count = r_s16(sequence + 2);
    if (current + 1 < count)
        G3Function80029F0C(object, 0xffffffff, frame);
    else
        w_u32(state, 1);
}

void G3LogoPosition(uint32 module_base, uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000001e8, "G3L_66");
    uint32 frame = guest_stack - 0x58;
    uint32 camera = r_u32(game + 0x4c);
    {
        uint32 words[2];
        for (uint32 index = 0; index < 2; ++index)
        {
            uint32 source = module_base + 0x50c + (index << 2);
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
            words[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 index = 0; index < 2; ++index)
        {
            uint32 destination = frame + 0x10 + (index << 2);
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15u << low_lane) & 15);
        }
    }
    if (object == 0)
        return;
    uint32 transform = r_u32(camera + 0x7c);
    G3Function800313E0(frame + 0x20, transform, frame);
    ApplyMatrixSV((MATRIX *)psx_addr(frame + 0x20, 20), (SVECTOR *)psx_addr(frame + 0x10, 8), (SVECTOR *)psx_addr(frame + 0x18, 6));
    uint32 component = r_u16(frame + 0x18);
    uint32 origin = r_u16(camera);
    w_u16(object + 0x58, component + origin);
    component = r_u16(frame + 0x1a);
    origin = r_u16(camera + 2);
    w_u16(object + 0x5a, component + origin);
    component = r_u16(frame + 0x1c);
    origin = r_u16(camera + 4);
    w_u16(object + 0x5c, component + origin);
}

uint32 G3LogoRender(uint32 object, uint32 vertices, uint32 packets, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000002b0, "G3L_66");
    uint32 frame = guest_stack - 0x90;
    uint32 state = object + 0x128;
    uint32 resource = r_u32(object + 0x1c);
    sint32 model_index = r_s16(object + 0xa2);
    uint32 screen_x = r_u16(state + 0xc);
    uint32 screen_y = r_u16(state + 0xe);
    sint32 scale = r_s16(state + 0x12);
    uint32 models = r_u32(resource + 0xc);
    uint32 model = r_u32(models + ((uint32)model_index << 2));
    G3FillMemory80086094(frame + 0x28, 0, 8);
    w_u16(frame + 0x28, r_u16(state + 0x10));
    {
        uint32 words[2];
        for (uint32 index = 0; index < 2; ++index)
        {
            uint32 source = frame + 0x28 + (index << 2);
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
            words[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 index = 0; index < 2; ++index)
        {
            uint32 destination = frame + 0x20 + (index << 2);
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15u << low_lane) & 15);
        }
    }
    {
        uint32 source = object + 0x58;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 low_lane = source & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
        uint32 word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        uint32 tail = (uint32)r_s16(source + 4);
        uint32 destination = frame + 0x28;
        high_address = destination + 3;
        high_lane = high_address & 3;
        low_lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
        w_u16(destination + 4, tail);
    }
    {
        uint32 words[2];
        for (uint32 index = 0; index < 2; ++index)
        {
            uint32 source = object + 0x60 + (index << 2);
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
            words[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 index = 0; index < 2; ++index)
        {
            uint32 destination = frame + 0x60 + (index << 2);
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15u << low_lane) & 15);
        }
    }
    resource = r_u32(object + 0x1c);
    w_u16(object + 0x58, 0);
    w_u16(object + 0x5a, 0);
    w_u16(object + 0x5c, 0);
    w_u16(object + 0x60, 0);
    w_u16(object + 0x62, 0);
    w_u16(object + 0x64, 0);
    if (r_u32(resource + 0x10) != 0)
    {
        uint32 matrices = r_u32(object + 0x30);
        G3Function800328A0(object, resource, matrices, model, frame);
    }
    else
    {
        uint32 matrices = r_u32(object + 0x30);
        G3Function800315E8(object, matrices, model, frame);
    }
    {
        uint32 source = frame + 0x28;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 low_lane = source & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
        uint32 word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        uint32 tail = (uint32)r_s16(source + 4);
        uint32 destination = object + 0x58;
        high_address = destination + 3;
        high_lane = high_address & 3;
        low_lane = destination & 3;
        w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(destination & 0xfffffffc, word << (low_lane * 8), (15u << low_lane) & 15);
        w_u16(destination + 4, tail);
    }
    {
        uint32 words[2];
        for (uint32 index = 0; index < 2; ++index)
        {
            uint32 source = frame + 0x60 + (index << 2);
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << low_lane) & 15);
            words[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 index = 0; index < 2; ++index)
        {
            uint32 destination = object + 0x60 + (index << 2);
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15u << low_lane) & 15);
        }
    }
    resource = r_u32(object + 0x1c);
    model_index = r_s16(object + 0xa2);
    models = r_u32(resource + 0xc);
    model = r_u32(models + ((uint32)model_index << 2));
    sint32 time = r_s16(object + 0x9e);
    uint32 sequence = r_u32(model + 0x1c);
    G3Function80034FA4(sequence, (uint32)time, 0, 0);
    G3Function8002B168(object, frame);
    G3Function8005BD00(frame + 0x20, frame + 0x40);
    uint32 half_scale = (uint32)scale + ((uint32)scale >> 31);
    half_scale = (uint32)((sint32)half_scale >> 1);
    w_u32(frame + 0x38, (uint32)scale);
    w_u32(frame + 0x34, (uint32)scale);
    w_u32(frame + 0x30, (uint32)scale);
    w_u32(frame + 0x38, half_scale);
    G3Function8005ADF0(frame + 0x40, frame + 0x30);
    uint32 shifted_x = (screen_x - 0x100) << 16;
    uint32 centered_x = (uint32)((sint32)shifted_x >> 16) + (shifted_x >> 31);
    centered_x = (uint32)((sint32)centered_x >> 1);
    uint32 shifted_y = (screen_y - 0x78) << 16;
    w_u32(frame + 0x54, centered_x);
    uint32 centered_y = (uint32)((sint32)shifted_y >> 16) + (shifted_y >> 31);
    w_u32(frame + 0x5c, 200);
    uint32 order_table = r_u32(guest_stack + 0x10);
    centered_y = (uint32)((sint32)centered_y >> 1);
    w_u32(frame + 0x58, centered_y);
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x10, order_table);
    w_u32(frame + 0x18, 1200);
    G3Function80033BA8(object, frame + 0x40, vertices, packets, order_table, 0, 1200, frame);
    return 0;
}

uint32 G3IntroEvent(uint32 module_base, uint32 object, uint32 message, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_42");
    uint32 frame = guest_stack - 0x20;
    if (message == 1)
    {
        G3IntroInitialize(object);
        return 1;
    }
    if (message != 2)
        return 0;
    uint32 state = object + 0x128;
    if (r_u32(0x8009efac) == 2 && r_u32(state + 4) != 1)
    {
        uint32 flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags & 0xfffffbff);
        G3Function8002A9AC(object, frame);
        return 1;
    }
    uint32 mode = r_u32(state + 4);
    if (mode == 0)
        G3IntroPhaseZero(object, frame);
    else if (mode == 1)
        G3IntroPhaseOne(module_base, object, frame);
    else if (mode == 2)
        G3IntroPhaseTwo(object, frame);
    uint32 resource = r_u32(object + 0x1c);
    uint32 animation = r_u16(object + 0x6e);
    sint32 count = r_s16(resource + 0xa);
    if ((sint32)animation >= count)
        w_u16(object + 0x6e, 0);
    return 1;
}

void G3IntroInitialize(uint32 object)
{
    FUNCTION_MARKER(0x00000118, "G3L_42");
    uint32 resource = r_u32(object + 0x1c);
    uint32 data = r_u32(resource + 0x1c);
    uint32 state = object + 0x128;
    w_u32(state + 4, data != 0 ? r_u32(data) : 0);
    uint32 mode = r_u32(state + 4);
    w_u32(state, 0);
    w_u32(state + 0x10, 0);
    w_u32(state + 8, 0);
    w_u32(state + 0xc, 0);
    if ((sint32)mode >= 3)
        w_u32(state + 4, 0);
    uint32 flags = r_u32(object + 0x10);
    w_u16(object + 0x6e, 0);
    w_u16(object + 0x5e, 0);
    w_u32(object + 0x150, 0);
    w_u32(object + 0x10, flags | 0x400);
    if (r_u32(state + 4) == 0)
        w_u16(object + 0x5e, 0x40);
    if (r_u32(state + 4) == 2)
    {
        uint32 data = r_u32(object + 0x28);
        if (data != 0)
        {
            w_u32(state + 0x10, r_u32(data + 8));
            w_u32(state + 8, r_u32(data));
            w_u32(state + 0xc, r_u32(data + 4));
        }
        flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags | 0x800);
    }
}

void G3IntroPhaseZero(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000001e8, "G3L_42");
    uint32 frame = guest_stack - 0x18;
    uint32 resource = r_u32(object + 0x1c);
    uint32 animation = r_u16(object + 0x6e);
    uint32 table = r_u32(resource + 0x10);
    uint32 counter = r_u32(object + 0x128);
    uint32 sequence = r_u32(table + (animation << 2));
    sint32 count = r_s16(sequence + 2);
    uint32 state = object + 0x128;
    if ((sint32)counter >= 0xfb)
    {
        uint32 flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags & 0xfffffbff);
    }
    else
    {
        uint32 clock = r_u32(0x8009efa8);
        if (clock - 0x349 < 0x4a)
        {
            uint32 intro = r_u32(object + 0x20);
            uint32 delta = clock - 0x348;
            uint32 origin = r_u16(intro + 0x14);
            w_u16(object + 0x5c, origin + (delta << 2) + delta);
        }
        clock = r_u32(0x8009efa8);
        if (clock == 1000 && r_u16(object + 0x6e) != 2)
            w_u16(object + 0x6e, 2);
        else if (r_s16(object + 0x5e) < count - 1)
            G3Function80029F0C(object, 0xffffffff, frame);
        else
        {
            animation = r_u16(object + 0x6e);
            w_u16(object + 0x5e, 0);
            if (animation < 2 && r_s32(state) >= 0x3d)
                w_u16(object + 0x6e, animation + 1);
        }
    }
    counter = r_u32(state);
    w_u32(state, counter + 1);
}

void G3IntroPhaseOne(uint32 module_base, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000304, "G3L_42");
    uint32 frame = guest_stack - 0x20;
    uint32 resource = r_u32(object + 0x1c);
    uint32 animation = r_u16(object + 0x6e);
    uint32 table = r_u32(resource + 0x10);
    uint32 clock = r_u32(0x8009efa8);
    uint32 sequence = r_u32(table + (animation << 2));
    sint32 count = r_s16(sequence + 2);
    uint32 counter = r_u32(object + 0x128) + 1;
    w_u32(object + 0x128, counter);
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x10, clock - 0xfb < 0x3b ? flags | 0x800 : flags & 0xfffff7ff);
    if (r_s32(0x8009efa8) < 5 && r_u32(object + 0x150) == 0 && r_u16(object + 0x6e) == 0)
    {
        w_u16(object + 0x6e, 3);
        w_u32(object + 0x150, 1);
        w_u16(object + 0x5e, 0);
        return;
    }
    if (r_s16(object + 0x5e) < count - 1)
    {
        G3Function80029F0C(object, 0xffffffff, frame);
        return;
    }
    uint32 index = r_u32(object + 0x150);
    w_u16(object + 0x5e, 0);
    if (index >= 5)
        return;
    uint32 target = r_u32(module_base + 0x72c + (index << 2));
    switch (target - module_base)
    {
    case 0x418:
        if (object == 0)
            return;
        animation = r_u16(object + 0x6e);
        if (animation == 1)
        {
            w_u16(object + 0x6e, 3);
            w_u32(object + 0x150, 1);
            return;
        }
        if (r_s32(0x8009efa8) >= 0x551 && animation == 0)
            w_u16(object + 0x6e, 1);
        return;
    case 0x464:
    {
        clock = r_u32(0x8009efa8);
        if (clock - 0xfb < 0x27)
        {
            sint32 random = (sint32)G3SdkRandom80027FA8();
            if (random % 64 < 16)
            {
                w_u32(object + 0x150, 3);
                w_u16(object + 0x6e, 8);
            }
            return;
        }
        if ((sint32)clock < 0x1ae)
        {
            w_u16(object + 0x6e, 0);
            return;
        }
        sint32 random = (sint32)G3SdkRandom80027FA8();
        sint32 selected = random % 128;
        if (selected < 48)
        {
            w_u32(object + 0x150, 2);
            w_u16(object + 0x6e, 2);
        }
        else if (selected < 64)
        {
            w_u32(object + 0x150, 2);
            w_u16(object + 0x6e, 7);
        }
        else if (selected < 110)
        {
            w_u32(object + 0x150, 2);
            w_u16(object + 0x6e, 3);
        }
        else
        {
            w_u32(object + 0x150, 4);
            w_u16(object + 0x6e, 4);
        }
        return;
    }
    case 0x654:
        w_u16(object + 0x6e, 0);
        w_u32(object + 0x150, 1);
        return;
    case 0x548:
        animation = r_u16(object + 0x6e);
        if (animation == 9)
        {
            if (r_u32(0x8009efa8) == 250)
            {
                w_u16(object + 0x6e, 0);
                w_u32(object + 0x150, 1);
            }
            else
                w_u16(object + 0x5e, (uint32)(count - 1));
            return;
        }
        if (r_u32(0x8009efa8) - 0x28 < 0x19b)
            return;
        if ((sint32)G3SdkRandom80027FA8() % 64 < 24)
            w_u16(object + 0x6e, 9);
        return;
    case 0x5c0:
        animation = r_u16(object + 0x6e);
        if (animation == 6)
        {
            if (r_u32(0x8009efa8) - 0xc9 < 0x45)
            {
                w_u16(object + 0x6e, 0);
                w_u32(object + 0x150, 1);
            }
            return;
        }
        if (r_u32(0x8009efa8) - 0x119 < 0x22)
        {
            w_u16(object + 0x6e, 6);
            return;
        }
        if (animation == 4)
        {
            w_u16(object + 0x6e, 5);
            return;
        }
        if (animation != 5)
            return;
        if ((sint32)G3SdkRandom80027FA8() % 64 < 16)
        {
            w_u16(object + 0x6e, 0);
            w_u32(object + 0x150, 1);
        }
        else
            w_u16(object + 0x6e, 4);
        return;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved rezntro switch target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}

void G3IntroPhaseTwo(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000067c, "G3L_42");
    uint32 frame = guest_stack - 0x18;
    uint32 resource = r_u32(object + 0x1c);
    uint32 animation = r_u16(object + 0x6e);
    uint32 flags = r_u32(object + 0x10);
    uint32 table = r_u32(resource + 0x10);
    uint32 sequence = r_u32(table + (animation << 2));
    sint32 count = r_s16(sequence + 2);
    uint32 state = object + 0x128;
    if ((flags & 0x800) != 0)
    {
        uint32 clock = r_u32(0x8009efa8);
        uint32 trigger = r_u32(state + 8);
        if (clock == trigger)
        {
            w_u32(object + 0x10, flags & 0xfffff7ff);
            w_u16(object + 0x6e, 0);
            w_u16(object + 0x5e, 0);
        }
    }
    else if (r_s16(object + 0x5e) + 1 < count)
        G3Function80029F0C(object, 0xffffffff, frame);
    else
        w_u32(object + 0x10, flags | 0x800);
    uint32 counter = r_u32(state);
    w_u32(state, counter + 1);
}

uint32 G3FunPlatformEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_22");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3FunPlatformInitialize(object, game, frame);
        return 1;
    case 2:
        G3FunPlatformUpdate(object, game, frame);
        return 1;
    case 3:
        G3FunPlatformCollide(object, game, frame);
        return 1;
    default:
        return 0;
    }
}

void G3FunPlatformInitialize(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000088, "G3L_22");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    uint32 flags = r_u32(object + 0x10);
    uint32 data = r_u32(object + 0x28);
    w_u32(object + 0x10, flags | 0x100000);
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
    uint32 repeat = data != 0 ? (r_s16(data + 6) == 0 ? 1 : 0) : 1;
    w_u16(state + 6, repeat);
    uint32 direction = data == 0 || r_s16(data + 6) != -1 ? 1 : 0;
    w_u8(state + 8, direction);
    direction = data == 0 || r_s16(data + 6) != -1 ? 1 : 0;
    w_u8(state + 9, direction);
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

void G3FunPlatformMovePath(uint32 object, uint32 path, uint32 cursor, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000001cc, "G3L_22");
    uint32 frame = guest_stack - 0x38;
    uint32 channel = r_u32(guest_stack + 0x10);
    uint32 state = object + 0x104;
    uint32 selected_state = state + channel;
    uint32 mode = r_u8(selected_state + 8);
    uint32 data = r_u32(object + 0x28);
    if (mode == 0)
    {
        uint32 flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags & 0xfffffbff);
        uint32 index = r_u16(path + 4) - 1;
        w_u16(cursor, index);
        uint32 row;
        bool update_cursor = true;
        if (channel == 0)
            row = r_u32(path) + (uint32)((sint32)(index << 16) >> 11);
        else if (channel == 1)
            row = r_u32(path) + (uint32)(sint32)(sint16)index * 10;
        else
            update_cursor = false;
        if (update_cursor)
        {
            uint32 segments = r_u16(row);
            uint32 fraction = r_u16(cursor + 2);
            w_u16(cursor + 2, (fraction & 1) | ((segments - 1) << 1));
        }
    }
    else if (mode == 1)
    {
        if (G3Function80041230(path, cursor, frame) == 0)
        {
            if (r_s16(state + 6) != 0)
            {
                G3Function8004136C(path, cursor, frame);
                w_u8(selected_state + 8, 2);
            }
            else
            {
                uint32 flags = r_u32(object + 0x10);
                w_u32(object + 0x10, flags & 0xfffffbff);
                w_u8(selected_state + 8, 0);
                if (data != 0)
                    w_u16(data + 6, 0xffff);
            }
        }
    }
    else if (mode == 2)
    {
        if (G3Function8004136C(path, cursor, frame) == 0)
        {
            uint32 flags = r_u32(object + 0x10);
            w_u32(object + 0x10, flags & 0xfffffbff);
            G3Function80041230(path, cursor, frame);
            w_u8(selected_state + 8, 1);
        }
    }
    if (r_s16(cursor) == 0)
    {
        uint32 flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags & 0xfffffbff);
    }
    G3Function80041008(path, cursor, output, frame);
}
