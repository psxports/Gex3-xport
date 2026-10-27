#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32 G3SnowWeaponEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_15");
    uint32 frame = guest_stack - 0x18;
    uint32 index = message - 1;
    if (index >= 8)
        return 0;
    uint32 target = r_u32(module_base + 0x79c + (index << 2));
    switch (target - module_base)
    {
    case 0x3c:
        G3SnowWeaponPlacement(payload);
        return 1;
    case 0x4c:
        G3SnowWeaponInitialize(object);
        return 1;
    case 0x5c:
        G3SnowWeaponUpdate(object, frame);
        return 1;
    case 0x6c:
        G3SnowWeaponTrigger(object);
        return 1;
    case 0x7c:
        G3SnowWeaponContact();
        return 1;
    case 0x8c:
        return 0;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved snow96w event target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}

uint32 G3RemainingPlacementCount80045FE8(uint32 object)
{
    uint32 intro = r_u32(object + 0x20);
    if (intro == 0)
        return 0;
    uint32 offset = (uint32)r_s16(intro + 0x2e);
    if (offset == 0)
        return 0;
    uint32 game = r_u32(0x80097048);
    uint32 saved = r_u32(game + 0x68b0);
    uint32 begin = saved + offset;
    sint32 count = r_s16(intro + 0x2c);
    uint32 cursor = begin - 1;
    uint32 remaining = 0;
    for (sint32 index = 0; index < count; ++index)
    {
        if (r_u8(cursor) == 0)
            ++remaining;
        ++cursor;
    }
    return remaining;
}

void G3SnowWeaponSelect(uint32 object, uint32 mode, uint32 at_end)
{
    FUNCTION_MARKER(0x000000a4, "G3L_15");
    uint32 state = object + 0x104;
    if ((sint32)mode < 2)
        w_u32(object + 0x150, mode);
    else
        w_u32(object + 0x150, 0);
    mode = r_u32(object + 0x150);
    uint32 selected = 0xffffffff;
    if (mode == 0)
    {
        selected = r_u16(state + 6);
        w_u16(object + 0x6e, selected);
    }
    else if (mode == 1)
    {
        selected = r_u16(state + 4);
        w_u16(object + 0x6e, selected);
    }
    uint32 enabled = (sint16)selected >= 0 ? 1 : 0;
    w_u32(state + 0xc, enabled);
    uint32 resource = r_u32(object + 0x1c);
    if (r_s16(resource + 0xa) > 0)
    {
        uint32 frame = 0;
        if (enabled != 0 && at_end != 0)
        {
            uint32 animation = r_u16(object + 0x6e);
            uint32 table = r_u32(resource + 0x10);
            uint32 record = r_u32(table + (animation << 2));
            frame = r_u16(record + 2) - 1;
        }
        w_u16(object + 0x5e, frame);
    }
}

void G3SnowWeaponInitialize(uint32 object)
{
    FUNCTION_MARKER(0x00000168, "G3L_15");
    uint32 custom = r_u32(object + 0x28);
    uint32 resource = r_u32(object + 0x1c);
    uint32 intro = r_u32(object + 0x20);
    uint32 defaults = r_u32(resource + 0x1c);
    uint32 state = object + 0x104;
    if (intro != 0)
    {
        uint32 flags = r_u32(intro + 0x18);
        if ((flags & 0x800) != 0)
        {
            w_u32(intro + 0x18, flags & 0xfffff7ff);
            if (custom != 0)
                w_u16(custom + 2, 1);
        }
    }
    if (custom != 0)
        w_u16(state, r_u16(custom));
    else if (defaults != 0)
        w_u16(state, r_u16(defaults));
    else
        w_u16(state, 0);
    if (custom != 0)
        w_u16(state + 2, r_u16(custom + 2));
    else if (defaults != 0)
        w_u16(state + 2, r_u16(defaults + 2));
    else
        w_u16(state + 2, 0);
    uint32 type = r_u16(state);
    w_u16(state + 4, 0);
    w_u16(state + 6, 0xffff);
    w_u16(state + 8, 0xffff);
    w_u16(state + 0xa, 0xffff);
    if (type < 2)
        w_u16(state + 2, G3RemainingPlacementCount80045FE8(object));
    if (custom != 0)
        w_u16(custom + 2, r_u16(state + 2));
    if (r_s16(state) == 1)
    {
        w_u16(state + 4, 4);
        w_u16(state + 6, 0);
        w_u16(state + 8, 1);
        w_u16(state + 0xa, 3);
    }
    G3SnowWeaponSelect(object, 0, 0);
}

void G3SnowWeaponUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000002a4, "G3L_15");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    if (r_u32(state + 0xc) != 0)
    {
        uint32 resource = r_u32(object + 0x1c);
        uint32 selected = r_u16(object + 0x6e);
        uint32 old_frame = r_u16(object + 0x5e);
        uint32 animations = r_u32(resource + 0x10);
        uint32 animation = r_u32(animations + (selected << 2));
        sint32 current = r_s16(object + 0x5e);
        sint32 length = r_s16(animation + 2);
        if (current + 1 < length)
        {
            resource = r_u32(object + 0x1c);
            w_u16(object + 0x5e, old_frame + 1);
            if ((r_u32(resource + 0x30) & 0x2000) != 0)
                G3Function8003E314(object, frame);
        }
        else
        {
            uint32 mode = r_u32(object + 0x150);
            if (mode == 1)
            {
                if (r_s16(state) == 1 && selected == 4)
                {
                    w_u16(object + 0x6e, 2);
                    w_u16(object + 0x5e, 0);
                }
                else
                {
                    G3SnowWeaponSelect(object, 0, 0);
                    return;
                }
            }
            else if (mode == 0)
            {
                sint32 random = (sint32)G3SdkRandom80027FA8();
                sint32 choice = random % 128;
                sint32 first = r_s16(state + 8);
                uint32 next = r_u16(state + 8);
                bool chosen = false;
                if (first >= 0 && choice < 32 && r_s16(state + 2) > 0)
                    chosen = true;
                if (!chosen)
                {
                    sint32 second = r_s16(state + 0xa);
                    next = r_u16(state + 0xa);
                    if (second >= 0 && choice < 64 && r_s16(state + 2) > 0)
                        chosen = true;
                }
                if (chosen)
                {
                    w_u16(object + 0x6e, next);
                    w_u16(object + 0x5e, 0);
                }
                else
                {
                    next = r_u16(state + 6);
                    w_u16(object + 0x5e, 0);
                    w_u16(object + 0x6e, next);
                }
            }
        }
    }
    if (r_u32(object + 0x150) == 1)
    {
        if (r_s16(state) == 0 && r_s16(object + 0x5e) == 7)
            G3SnowWeaponEmitSnow(object, frame);
        if (r_s16(state) == 1 && r_u16(object + 0x6e) == 2 && r_s16(object + 0x5e) == 5)
            G3SnowWeaponEmitFire(object, frame);
    }
}

void G3SnowWeaponEmitSnow(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000494, "G3L_15");
    uint32 frame = guest_stack - 0x40;
    uint32 angle = (uint32)r_s16(object + 0x64);
    uint32 product = G3Function8005B590(angle) * 100;
    if ((sint32)product < 0)
        product += 0xfff;
    uint32 offset_x = (uint32)((sint32)product >> 12);
    angle = (uint32)r_s16(object + 0x64);
    product = 0 - G3Function8005B470(angle) * 100;
    if ((sint32)product < 0)
        product += 0xfff;
    uint32 offset_y = (uint32)((sint32)product >> 12);
    angle = (uint32)r_s16(object + 0x64);
    product = G3Function8005B590(angle) * 20;
    if ((sint32)product < 0)
        product += 0xfff;
    uint32 velocity_x = (uint32)((sint32)product >> 12);
    angle = (uint32)r_s16(object + 0x64);
    product = G3Function8005B470(angle) * (uint32)(sint32)-20;
    if ((sint32)product < 0)
        product += 0xfff;
    uint32 velocity_y = (uint32)((sint32)product >> 12);
    uint32 x = r_u16(object + 0x58);
    uint32 z = r_u16(object + 0x5c);
    w_u16(object + 0x58, x + offset_x);
    uint32 y = r_u16(object + 0x5a);
    w_u16(object + 0x5c, z + 250);
    w_u16(object + 0x5a, y + offset_y);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x18, 0xfffffffb);
    w_u32(frame + 0x1c, 100);
    w_u32(frame + 0x20, 100);
    w_u32(frame + 0x24, 100);
    G3Function800460E8(object, velocity_x, velocity_y, 42, frame);
    z = r_u16(object + 0x5c);
    y = r_u16(object + 0x5a);
    w_u16(object + 0x5c, z - 250);
    x = r_u16(object + 0x58);
    w_u16(object + 0x5a, y - offset_y);
    w_u16(object + 0x58, x - offset_x);
}

void G3SnowWeaponEmitFire(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000005d4, "G3L_15");
    uint32 frame = guest_stack - 0x40;
    uint32 angle = (uint32)r_s16(object + 0x64);
    uint32 product = G3Function8005B590(angle) * 120;
    if ((sint32)product < 0)
        product += 0xfff;
    uint32 offset_x = (uint32)((sint32)product >> 12);
    angle = (uint32)r_s16(object + 0x64);
    product = 0 - G3Function8005B470(angle) * 120;
    if ((sint32)product < 0)
        product += 0xfff;
    uint32 offset_y = (uint32)((sint32)product >> 12);
    angle = (uint32)r_s16(object + 0x64);
    product = G3Function8005B590(angle) * 30;
    if ((sint32)product < 0)
        product += 0xfff;
    uint32 velocity_x = (uint32)((sint32)product >> 12);
    angle = (uint32)r_s16(object + 0x64);
    product = G3Function8005B470(angle) * (uint32)(sint32)-30;
    if ((sint32)product < 0)
        product += 0xfff;
    uint32 velocity_y = (uint32)((sint32)product >> 12);
    uint32 x = r_u16(object + 0x58);
    uint32 z = r_u16(object + 0x5c);
    w_u16(object + 0x58, x + offset_x);
    uint32 y = r_u16(object + 0x5a);
    w_u16(object + 0x5c, z + 100);
    w_u16(object + 0x5a, y + offset_y);
    w_u32(frame + 0x18, 0xfffffffb);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x1c, 100);
    w_u32(frame + 0x20, 100);
    w_u32(frame + 0x24, 100);
    G3Function800460E8(object, velocity_x, velocity_y, 8, frame);
    z = r_u16(object + 0x5c);
    y = r_u16(object + 0x5a);
    w_u16(object + 0x5c, z - 100);
    x = r_u16(object + 0x58);
    w_u16(object + 0x5a, y - offset_y);
    w_u16(object + 0x58, x - offset_x);
}

void G3SnowWeaponContact(void)
{
    FUNCTION_MARKER(0x00000710, "G3L_15");
}

void G3SnowWeaponTrigger(uint32 object)
{
    FUNCTION_MARKER(0x00000718, "G3L_15");
    uint32 mode = r_u32(object + 0x150);
    uint32 custom = r_u32(object + 0x28);
    uint32 state = object + 0x104;
    if (mode != 0)
        return;
    sint32 remaining = r_s16(state + 2);
    uint32 raw_remaining = r_u16(state + 2);
    if (remaining <= 0)
        return;
    w_u16(state + 2, raw_remaining - 1);
    if (custom != 0)
        w_u16(custom + 2, r_u16(custom + 2) - 1);
    G3SnowWeaponSelect(object, 1, 0);
}

void G3SnowWeaponPlacement(uint32 intro)
{
    FUNCTION_MARKER(0x00000774, "G3L_15");
    uint32 flags = r_u32(intro + 0x18);
    if ((flags & 0x400) != 0)
        w_u32(intro + 0x18, (flags & 0xfffffbff) | 0x800);
}

uint32 G3SnowboardEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_17");
    uint32 frame = guest_stack - 0x20;
    uint32 settings = 0;
    uint32 state = 0;
    if (object != 0)
    {
        uint32 descriptor = r_u32(object + 0x1c);
        state = object + 0x104;
        settings = r_u32(descriptor + 0x1c);
    }
    if (settings == 0) return 1;
    switch (message)
    {
    case 1: G3SnowboardInitialize(state, module_base); return 1;
    case 2:
        if (r_s16(game + 0x67ee) != 1)
        {
            G3SnowboardUpdate(object, game, state, settings, module_base, frame);
            return 1;
        }
        if (r_s16(state) != 2) return 1;
        {
            uint32 player = r_u32(game + 0x50);
        {
            uint32 source = player + 0x58;
            uint32 destination = object + 0x58;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            uint32 z = (uint32)r_s16(source + 4);
            high_address = destination + 3;
            high_lane = high_address & 3;
            lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
            w_u16(destination + 4, z);
        }
        }
        return 1;
    case 3:
    case 0x12:
        if (r_s16(game + 0x67ee) != 1) G3SnowboardCollide(object, game, state, settings, frame);
        return 1;
    case 4: G3Function8003A114(module_base + 0x1b6c, 4); return 1;
    case 8:
        w_u32(frame + 0x10, payload);
        G3SnowboardDamage(object, game, state, frame);
        return 1;
    case 0x7eb: w_u8(state + 0x30, 1); return 1;
    case 0x7f1: return 0;
    case 0x7f2: return r_s16(state + 2) == 0 ? 1 : 0;
    case 0x7f3:
    {
        uint32 player = r_u32(0x8009efc4);
        sint32 z = r_s16(object + 0x5c);
        sint32 floor = r_s16(player + 0xe0);
        sint32 difference = z - floor;
        if (difference < 0) difference = -difference;
        return difference < 5 ? 1 : 0;
    }
    default: return 0;
    }
}

void G3SnowboardInitialize(uint32 state, uint32 module_base)
{
    FUNCTION_MARKER(0x000001ec, "G3L_17");
    w_u16(state, 0);
    w_u16(state + 0xe, 0);
    w_u16(state + 0x18, 0);
    w_u16(state + 0x1a, 0);
    w_u16(state + 0x1c, 0);
    w_u16(state + 6, 0);
    w_u8(state + 0x2f, 0);
    w_u8(state + 0x30, 0);
    uint32 audio = module_base + 0x1b6c;
    w_u32(audio, 0);
    w_u16(audio + 4, 0x90e);
    w_u16(audio + 0xa, 40);
    w_u16(audio + 8, 0);
    w_u16(audio + 6, 0x157c);
    w_u16(audio + 0xc, 0);
}

void G3SnowboardDust(uint32 position, uint32 heading, uint32 speed, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000023c, "G3L_17");
    uint32 frame = guest_stack - 0x38;
    uint32 source = module_base + 0x1b30;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 lane = source & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
    uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
    uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
    uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
    uint32 last = (uint32)r_s16(source + 4);
    uint32 destination = frame + 0x10;
    high_address = destination + 3;
    high_lane = high_address & 3;
    lane = destination & 3;
    w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
    w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
    w_u16(frame + 0x14, last);
    uint32 resource = r_u32(0x80098724);
    if (resource == 0) return;
    heading = (uint32)(sint32)(sint16)heading;
    uint32 sine = G3Function8005B470(heading);
    uint32 x = (uint32)((sint32)(sine * speed) >> 12);
    uint32 cosine = G3Function8005B590(heading);
    uint32 y = (uint32)((sint32)(cosine * speed) >> 12);
    uint32 random = G3SdkRandom80027FA8();
    uint32 origin = r_u16(position);
    w_u16(frame + 0x18, origin + x * 8 + (random & 31) - 7);
    random = G3SdkRandom80027FA8();
    origin = r_u16(position + 2);
    w_u16(frame + 0x1a, origin + y * 8 + (random & 31) - 7);
    random = G3SdkRandom80027FA8();
    origin = r_u16(position + 4);
    w_u16(frame + 0x1c, origin + (random & 15) + 5);
    uint32 models = r_u32(resource + 0xc);
    uint32 model = r_u32(models);
    uint32 pool = r_u32(0x800a27fc);
    uint32 effect = G3Function800444E4(frame + 0x18, model, pool, 20, frame);
    if (effect != 0)
    {
        G3Function80044878(effect, frame + 0x10, 0);
    }
}

uint32 G3SnowboardSteer(uint32 game, uint32 object, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000368, "G3L_17");
    uint32 frame = guest_stack - 0x30;
    uint32 input_heading = 0;
    uint32 desired = 0;
    uint32 y = r_u32(game + 0xa8);
    bool input = y != 0;
    if (!input) input = r_u32(game + 0xa4) != 0;
    if (input)
    {
        uint32 x = r_u32(game + 0xa4);
        input_heading = G3Function8002F240(0u - y, x, frame) + 0x400;
        if ((input_heading & 0xffff) == 0) input_heading = 0x1000;
        y = r_u32(game + 0xa8);
        uint32 yy = y * y;
        x = r_u32(game + 0xa4);
        if ((sint32)(yy + x * x) < 0x2400)
        {
            uint32 player = r_u32(0x8009efc4);
            w_u32(player + 0x128, r_u32(player + 0x128) | 0x20);
        }
    }
    if ((input_heading & 0xffff) != 0) desired = input_heading + G3Function80014F00(game);
    if (r_s16(state + 2) == 3) desired = 0;
    else
    {
        uint32 player = r_u32(game + 0x50);
        uint32 face = r_u32(player + 0xa8);
        if (face != 0 && (r_u16(face + 6) & 15) == 2) desired = 0;
    }
    uint32 player = r_u32(game + 0x50);
    if (r_s16(player + 0xb8) < 4000)
    {
        uint32 normal_y = (uint32)r_s16(player + 0xb6);
        uint32 normal_x = (uint32)r_s16(player + 0xb4);
        uint32 downhill = G3Function8002F0C4(normal_y, normal_x, frame);
        downhill = (uint32)(sint32)(sint16)(downhill + 0x400);
        G3Function80038AA8(state + 4, downhill, 16);
        if ((desired & 0xffff) == 0) desired = r_u16(state + 4);
    }
    if ((desired & 0xffff) != 0)
    {
        uint32 mode = r_u16(state + 2);
        if (mode - 1u >= 2)
        {
            uint32 step = (uint32)r_s16(settings);
            G3Function80038AA8(state + 4, (uint32)(sint32)(sint16)desired, step);
        }
        uint32 step = (uint32)r_s16(settings + 2);
        G3Function80038AA8(object + 0x64, (uint32)(sint32)(sint16)desired, step);
    }
    sint32 speed = r_s16(state + 0xa);
    w_u16(state + 8, desired);
    if (speed == 0) w_u16(state + 4, r_u16(object + 0x64));
    return (uint32)(sint32)(sint16)input_heading;
}

void G3SnowboardSpeed(uint32 input, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000570, "G3L_17");
    uint32 frame = guest_stack - 0x28;
    if (r_s16(state + 2) != 0) return;
    if (r_u8(state + 0x2e) != 0)
    {
        w_u16(state + 0xa, 2);
        w_u8(state + 0x2e, 0);
    }
    bool slippery = false;
    uint32 game = r_u32(0x80097048);
    uint32 player = r_u32(game + 0x50);
    if (r_s16(player + 0xb8) < 4000)
    {
        uint32 normal_y = (uint32)r_s16(player + 0xb6);
        uint32 normal_x = (uint32)r_s16(player + 0xb4);
        uint32 downhill = G3Function8002F0C4(normal_y, normal_x, frame);
        downhill = (uint32)(sint32)(sint16)(downhill + 0x400);
        uint32 heading = (uint32)r_s16(state + 4);
        uint32 turn = G3Function80038B48(heading, downhill);
        bool negative = (sint16)turn < 0;
        heading = (uint32)r_s16(state + 4);
        turn = G3Function80038B48(heading, downhill);
        if (negative) turn = 0u - turn;
        if ((sint16)turn < 0x200) input = 1;
    }
    else
    {
        uint32 face = r_u32(player + 0xa8);
        if (face != 0 && (r_u16(face + 6) & 15) == 2)
        {
            sint32 speed = r_s16(state + 0xa);
            slippery = true;
            if (speed == 0) w_u16(state + 0xa, 2);
        }
    }
    if (slippery) return;
    bool accelerate = (input & 0xffff) != 0;
    if (accelerate) accelerate = r_u8(state + 0x2c) != 0;
    if (accelerate)
    {
        sint32 count = r_s16(settings + 0xc);
        uint32 table = r_u32(settings + 0x18);
        sint32 maximum = r_s16(table + (uint32)count * 2 - 2);
        sint32 speed = r_s16(state + 0xa);
        --maximum;
        uint32 bits = r_u16(state + 0xa);
        if (speed < maximum) w_u16(state + 0xa, bits + 1);
    }
    else
    {
        sint32 speed = r_s16(state + 0xa);
        sint32 step = r_s16(settings + 0x1c);
        uint32 step_bits = r_u16(settings + 0x1c);
        uint32 speed_bits = r_u16(state + 0xa);
        w_u16(state + 0xa, step < speed ? speed_bits - step_bits : 2);
    }
}

void G3SnowboardAnimate(uint32 object, uint32 game, uint32 state, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000734, "G3L_17");
    uint32 frame = guest_stack - 0x28;
    uint32 player = r_u32(game + 0x50);
    G3Function8002A1C8(player, 0xffffffff, frame);
    uint32 original_descriptor = r_u32(player + 0x1c);
    uint32 descriptor = G3Function8003AA48(module_base + 0x1b38);
    uint32 original_animation = r_u16(player + 0x6e);
    if (descriptor != 0)
    {
        w_u32(player + 0x1c, descriptor);
        bool remap = true;
        uint32 animation = 0;
        if (original_animation == 0x31) animation = 0;
        else if (original_animation == 0x32) animation = 1;
        else if (original_animation == 0x33) animation = 2;
        else if (original_animation == 0x29) animation = 3;
        else remap = false;
        if (remap)
        {
            w_u16(player + 0x6e, animation);
            G3Function8003E314(player, frame);
        }
        w_u32(player + 0x1c, original_descriptor);
        w_u16(player + 0x6e, original_animation);
    }
    uint32 animation = r_u16(player + 0x6e);
    if (animation == 0x29 || animation == 0x33)
    {
        if ((r_u32(player + 0x14) & 0x10) == 0) return;
        w_u16(player + 0x6e, 0x31);
        w_u16(player + 0x5e, 0);
    }
    else if (r_s16(state + 0xa) > 2)
    {
        if (r_s16(state + 2) == 0)
        {
            uint32 heading = (uint32)(sint32)(sint16)(r_u16(object + 0x64) + 0x400);
            G3SnowboardDust(object + 0x58, heading, 4, module_base, frame);
        }
        if ((r_u32(player + 0x14) & 0x10) == 0) return;
        uint32 cycle = r_u8(state + 0x2f) + 1;
        w_u8(state + 0x2f, cycle);
        w_u16(player + 0x6e, (cycle & 255) == 3 ? 0x32 : 0x31);
        return;
    }
    else w_u16(player + 0x6e, 0x31);
    w_u8(state + 0x2f, 0);
}

void G3SnowboardBank(uint32 object, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000008d8, "G3L_17");
    uint32 frame = guest_stack - 0x68;
    uint32 desired = (uint32)r_s16(state + 8);
    uint32 target = 0;
    if (desired != 0)
    {
        uint32 heading = (uint32)r_s16(object + 0x64);
        uint32 turn = G3Function80038B48(heading, desired);
        sint32 speed = r_s16(state + 0xa);
        sint32 scale = r_s16(settings + 0x24);
        sint32 numerator = speed * scale;
        sint32 denominator = r_s16(settings + 0x26);
        sint32 quotient = denominator == 0 ? (numerator < 0 ? 1 : -1) : numerator / denominator;
        target = 0u - turn;
        if ((sint16)quotient < (sint16)target) target = (uint32)quotient;
        if ((sint16)target < -(sint32)(sint16)quotient) target = 0u - (uint32)quotient;
    }
    G3Function80038AA8(state + 6, (uint32)(sint32)(sint16)target, 32);
    G3Function8005BD00(object + 0x60, frame + 0x10);
    w_u16(frame + 0x50, 0);
    uint32 bank = r_u16(state + 6);
    w_u16(frame + 0x54, 0);
    w_u16(frame + 0x52, 0u - bank);
    G3Function8005BD00(frame + 0x50, frame + 0x30);
    MulMatrix0((MATRIX *)psx_addr(frame + 0x10, 20), (MATRIX *)psx_addr(frame + 0x30, 20), (MATRIX *)psx_addr(frame + 0x30, 20));
    G3Function800202A4(frame + 0x30, object + 0x60, 0x15, frame);
}

void G3SnowboardAlign(uint32 object, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000009fc, "G3L_17");
    uint32 frame = guest_stack - 0x50;
    uint32 game = r_u32(0x80097048);
    {
        uint32 source = module_base + 0x1b44;
        uint32 destination = frame + 0x30;
        uint32 high_address0 = source + 3;
        uint32 high_lane0 = high_address0 & 3;
        uint32 lane0 = (source + 0) & 3;
        uint32 high0 = r_masked_u32(high_address0 & 0xfffffffc, (1u << (high_lane0 + 1)) - 1);
        uint32 low0 = r_masked_u32((source + 0) & 0xfffffffc, (15u << lane0) & 15);
        uint32 keep0 = lane0 == 0 ? 0 : 0xffffffff << ((4 - lane0) * 8);
        uint32 value0 = ((high0 << ((3 - high_lane0) * 8)) & keep0) | (low0 >> (lane0 * 8));
        uint32 high_address1 = source + 7;
        uint32 high_lane1 = high_address1 & 3;
        uint32 lane1 = (source + 4) & 3;
        uint32 high1 = r_masked_u32(high_address1 & 0xfffffffc, (1u << (high_lane1 + 1)) - 1);
        uint32 low1 = r_masked_u32((source + 4) & 0xfffffffc, (15u << lane1) & 15);
        uint32 keep1 = lane1 == 0 ? 0 : 0xffffffff << ((4 - lane1) * 8);
        uint32 value1 = ((high1 << ((3 - high_lane1) * 8)) & keep1) | (low1 >> (lane1 * 8));
        high_address0 = destination + 3;
        high_lane0 = high_address0 & 3;
        lane0 = (destination + 0) & 3;
        w_masked_u32(high_address0 & 0xfffffffc, value0 >> ((3 - high_lane0) * 8), (1u << (high_lane0 + 1)) - 1);
        w_masked_u32((destination + 0) & 0xfffffffc, value0 << (lane0 * 8), (15u << lane0) & 15);
        high_address1 = destination + 7;
        high_lane1 = high_address1 & 3;
        lane1 = (destination + 4) & 3;
        w_masked_u32(high_address1 & 0xfffffffc, value1 >> ((3 - high_lane1) * 8), (1u << (high_lane1 + 1)) - 1);
        w_masked_u32((destination + 4) & 0xfffffffc, value1 << (lane1 * 8), (15u << lane1) & 15);
    }
    uint32 player = r_u32(game + 0x50);
    sint32 z = r_s16(object + 0x5c);
    sint32 floor = r_s16(player + 0xe0);
    sint32 difference = z - floor;
    if (difference < 0) difference = -difference;
    uint32 normal = frame + 0x30;
    if (difference < 65)
    {
        game = r_u32(0x80097048);
        player = r_u32(game + 0x50);
        normal = player + 0xb4;
    }
    if (r_s16(normal + 4) < 0xb50) normal = frame + 0x30;
    if (r_s16(state + 0x28) == 0)
    {
        bool changed = r_s16(state + 0x18) != r_s16(normal);
        if (!changed) changed = r_s16(state + 0x1a) != r_s16(normal + 2);
        if (!changed) changed = r_s16(state + 0x1c) != r_s16(normal + 4);
        if (changed)
        {
            w_u16(state + 0x28, 1);
            {
                uint32 source = state + 0x18;
                uint32 destination = state + 0x10;
                uint32 high_address0 = source + 3;
                uint32 high_lane0 = high_address0 & 3;
                uint32 lane0 = (source + 0) & 3;
                uint32 high0 = r_masked_u32(high_address0 & 0xfffffffc, (1u << (high_lane0 + 1)) - 1);
                uint32 low0 = r_masked_u32((source + 0) & 0xfffffffc, (15u << lane0) & 15);
                uint32 keep0 = lane0 == 0 ? 0 : 0xffffffff << ((4 - lane0) * 8);
                uint32 value0 = ((high0 << ((3 - high_lane0) * 8)) & keep0) | (low0 >> (lane0 * 8));
                uint32 high_address1 = source + 7;
                uint32 high_lane1 = high_address1 & 3;
                uint32 lane1 = (source + 4) & 3;
                uint32 high1 = r_masked_u32(high_address1 & 0xfffffffc, (1u << (high_lane1 + 1)) - 1);
                uint32 low1 = r_masked_u32((source + 4) & 0xfffffffc, (15u << lane1) & 15);
                uint32 keep1 = lane1 == 0 ? 0 : 0xffffffff << ((4 - lane1) * 8);
                uint32 value1 = ((high1 << ((3 - high_lane1) * 8)) & keep1) | (low1 >> (lane1 * 8));
                high_address0 = destination + 3;
                high_lane0 = high_address0 & 3;
                lane0 = (destination + 0) & 3;
                w_masked_u32(high_address0 & 0xfffffffc, value0 >> ((3 - high_lane0) * 8), (1u << (high_lane0 + 1)) - 1);
                w_masked_u32((destination + 0) & 0xfffffffc, value0 << (lane0 * 8), (15u << lane0) & 15);
                high_address1 = destination + 7;
                high_lane1 = high_address1 & 3;
                lane1 = (destination + 4) & 3;
                w_masked_u32(high_address1 & 0xfffffffc, value1 >> ((3 - high_lane1) * 8), (1u << (high_lane1 + 1)) - 1);
                w_masked_u32((destination + 4) & 0xfffffffc, value1 << (lane1 * 8), (15u << lane1) & 15);
            }
            {
                uint32 source = normal;
                uint32 destination = state + 0x18;
                uint32 high_address0 = source + 3;
                uint32 high_lane0 = high_address0 & 3;
                uint32 lane0 = (source + 0) & 3;
                uint32 high0 = r_masked_u32(high_address0 & 0xfffffffc, (1u << (high_lane0 + 1)) - 1);
                uint32 low0 = r_masked_u32((source + 0) & 0xfffffffc, (15u << lane0) & 15);
                uint32 keep0 = lane0 == 0 ? 0 : 0xffffffff << ((4 - lane0) * 8);
                uint32 value0 = ((high0 << ((3 - high_lane0) * 8)) & keep0) | (low0 >> (lane0 * 8));
                uint32 high_address1 = source + 7;
                uint32 high_lane1 = high_address1 & 3;
                uint32 lane1 = (source + 4) & 3;
                uint32 high1 = r_masked_u32(high_address1 & 0xfffffffc, (1u << (high_lane1 + 1)) - 1);
                uint32 low1 = r_masked_u32((source + 4) & 0xfffffffc, (15u << lane1) & 15);
                uint32 keep1 = lane1 == 0 ? 0 : 0xffffffff << ((4 - lane1) * 8);
                uint32 value1 = ((high1 << ((3 - high_lane1) * 8)) & keep1) | (low1 >> (lane1 * 8));
                high_address0 = destination + 3;
                high_lane0 = high_address0 & 3;
                lane0 = (destination + 0) & 3;
                w_masked_u32(high_address0 & 0xfffffffc, value0 >> ((3 - high_lane0) * 8), (1u << (high_lane0 + 1)) - 1);
                w_masked_u32((destination + 0) & 0xfffffffc, value0 << (lane0 * 8), (15u << lane0) & 15);
                high_address1 = destination + 7;
                high_lane1 = high_address1 & 3;
                lane1 = (destination + 4) & 3;
                w_masked_u32(high_address1 & 0xfffffffc, value1 >> ((3 - high_lane1) * 8), (1u << (high_lane1 + 1)) - 1);
                w_masked_u32((destination + 4) & 0xfffffffc, value1 << (lane1 * 8), (15u << lane1) & 15);
            }
            uint32 angle = G3Function8002F540(state + 0x18, state + 0x10, frame);
            uint32 y = (uint32)r_s16(state + 0x1a);
            uint32 old_z = (uint32)r_s16(state + 0x14);
            uint32 yz = y * old_z;
            uint32 z_axis = (uint32)r_s16(state + 0x1c);
            uint32 old_y = (uint32)r_s16(state + 0x12);
            uint32 zy = z_axis * old_y;
            uint32 x = (uint32)r_s16(state + 0x18);
            old_z = (uint32)r_s16(state + 0x14);
            uint32 xz = x * old_z;
            z_axis = (uint32)r_s16(state + 0x1c);
            uint32 old_x = (uint32)r_s16(state + 0x10);
            uint32 zx = z_axis * old_x;
            x = (uint32)r_s16(state + 0x18);
            old_y = (uint32)r_s16(state + 0x12);
            uint32 xy = x * old_y;
            y = (uint32)r_s16(state + 0x1a);
            old_x = (uint32)r_s16(state + 0x10);
            uint32 yx = y * old_x;
            w_u16(state + 0x2a, angle);
            w_u16(state + 0x20, 0u - (uint32)((sint32)(yz - zy) >> 12));
            w_u16(state + 0x22, (uint32)((sint32)(xz - zx) >> 12));
            w_u16(state + 0x24, 0u - (uint32)((sint32)(xy - yx) >> 12));
        }
        if (r_s16(state + 0x28) == 0)
        {
            uint32 y = (uint32)r_s16(normal + 2);
            uint32 z_axis = (uint32)r_s16(normal + 4);
            uint32 angle = G3Function8002F0C4(y, z_axis, frame);
            w_u16(object + 0x60, 0u - angle);
            uint32 x = (uint32)r_s16(normal);
            uint32 root = G3Function8002F368((uint32)((sint32)(0x1000000u - x * x) >> 12), frame);
            x = (uint32)r_s16(normal);
            angle = G3Function8002F0C4(x, root, frame);
            w_u16(object + 0x62, angle);
            return;
        }
    }
    uint32 progress = r_u16(state + 0x28);
    uint32 step = r_u16(settings + 0x1e);
    progress += step;
    w_u16(state + 0x28, progress);
    sint32 limit = r_s16(state + 0x2a);
    uint32 limit_bits = r_u16(state + 0x2a);
    if ((sint16)progress >= limit) w_u16(state + 0x28, limit_bits);
    w_u16(frame + 0x14, r_u16(state + 0x10));
    w_u16(frame + 0x1a, r_u16(state + 0x12));
    w_u16(frame + 0x20, r_u16(state + 0x14));
    uint32 angle = (uint32)r_s16(state + 0x28);
    G3Function8002F630(state + 0x20, frame + 0x10, angle, frame);
    uint32 y = (uint32)r_s16(frame + 0x1a);
    uint32 z_axis = (uint32)r_s16(frame + 0x20);
    angle = G3Function8002F0C4(y, z_axis, frame);
    w_u16(object + 0x60, 0u - angle);
    uint32 x = (uint32)r_s16(frame + 0x14);
    uint32 root = G3Function8002F368((uint32)((sint32)(0x1000000u - x * x) >> 12), frame);
    x = (uint32)r_s16(frame + 0x14);
    angle = G3Function8002F0C4(x, root, frame);
    w_u16(object + 0x62, angle);
    sint32 current = r_s16(state + 0x28);
    limit = r_s16(state + 0x2a);
    if (current == limit) w_u16(state + 0x28, 0);
}

void G3SnowboardMove(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000d40, "G3L_17");
    uint32 frame = guest_stack - 0x58;
    uint32 input = G3SnowboardSteer(game, object, state, settings, frame);
    G3SnowboardSpeed((uint32)(sint32)(sint16)input, state, settings, frame);
    if (r_u8(state + 0x2c) != 0)
    {
        G3FillMemory80086094(frame + 0x10, 0, 8);
        uint32 table = r_u32(settings + 0x18);
        sint32 speed = r_s16(state + 0xa);
        sint32 threshold = r_s16(table);
        uint32 index = 0;
        while (speed >= threshold)
        {
            ++index;
            uint32 offset = (uint32)((sint32)(index << 16) >> 15);
            threshold = r_s16(table + offset);
        }
        uint32 offset = (uint32)((sint32)(index << 16) >> 15);
        w_u16(state + 0xc, index);
        table = r_u32(settings + 0x10);
        uint32 stride = r_u16(table + offset);
        w_u16(frame + 0x12, 0u - stride);
        G3Function8002F718(frame + 0x20);
        G3Function8005BB6C((uint32)r_s16(state + 4), frame + 0x20, frame);
        G3Function8005BA5C((uint32)r_s16(object + 0x62), frame + 0x20, frame);
        G3Function8005B94C((uint32)r_s16(object + 0x60), frame + 0x20, frame);
        ApplyMatrixSV((MATRIX *)psx_addr(frame + 0x20, 20), (SVECTOR *)psx_addr(frame + 0x10, 8), (SVECTOR *)psx_addr(frame + 0x18, 6));
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 position = r_u16(object + 0x58 + axis * 2);
            uint32 delta = r_u16(frame + 0x18 + axis * 2);
            w_u16(object + 0x58 + axis * 2, position + delta);
        }
    }
    G3SnowboardAlign(object, state, settings, module_base, frame);
    G3SnowboardAnimate(object, game, state, module_base, frame);
}

void G3SnowboardFooting(uint32 state)
{
    FUNCTION_MARKER(0x00000ecc, "G3L_17");
    w_u8(state + 0x2d, 1);
    w_u8(state + 0x2c, 1);
}

void G3SnowboardFollowPlatform(uint32 object, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000edc, "G3L_17");
    uint32 frame = guest_stack - 0x20;
    if (r_u8(state + 0x2d) != 0) return;
    uint32 player = r_u32(0x8009efc4);
    sint32 z = r_s16(object + 0x5c);
    sint32 floor = r_s16(player + 0xe0);
    sint32 difference = z - floor;
    if (difference < 0) difference = -difference;
    if (difference >= 5) return;
    player = r_u32(0x8009efc4);
    uint32 scale = (uint32)r_s16(settings + 0x22);
    G3Function80036278(player + 0xb4, frame + 0x10, scale, frame);
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        uint32 position = r_u16(object + 0x58 + axis * 2);
        uint32 delta = r_u16(frame + 0x10 + axis * 2);
        w_u16(object + 0x58 + axis * 2, position + delta);
    }
}

void G3SnowboardRequestJump(uint32 object, uint32 game, uint32 state, uint32 settings)
{
    FUNCTION_MARKER(0x00000f98, "G3L_17");
    uint32 player = r_u32(0x8009efc4);
    uint32 controls = (r_u32(player + 0x10) & 0x100) == 0 ? game + 0x70 : 0x800a0d28;
    if ((r_u32(controls + 4) & 0x80) == 0)
    {
        player = r_u32(game + 0x50);
        sint32 z = r_s16(object + 0x5c);
        sint32 floor = r_s16(player + 0xe0);
        sint32 difference = z - floor;
        if (difference < 0) difference = -difference;
        if (difference < 129) return;
    }
    if (r_u8(state + 0x2d) == 0 || r_s16(state + 2) != 0) return;
    uint32 index = r_u16(state + 0xc);
    uint32 table = r_u32(settings + 0x18);
    uint32 offset = (uint32)((sint32)(index << 16) >> 17) * 2;
    sint32 speed = r_s16(state + 0xa);
    sint32 threshold = r_s16(table + offset);
    if (speed < threshold)
    {
        index = r_u16(settings + 0x20);
        w_u16(state + 0xc, index);
        table = r_u32(settings + 0x18);
        offset = (uint32)((sint32)(index << 16) >> 15);
        w_u16(state + 0xa, r_u16(table + offset) - 1);
    }
    sint32 jump_index = r_s16(state + 0xc);
    table = r_u32(settings + 0x14);
    uint32 velocity = r_u16(table + (uint32)jump_index * 2);
    w_u16(state + 2, 1);
    w_u16(state + 0xe, velocity);
    player = r_u32(game + 0x50);
    w_u16(player + 0x6e, 0x29);
    player = r_u32(game + 0x50);
    w_u16(player + 0x5e, 0);
}

void G3SnowboardTrick(uint32 game)
{
    FUNCTION_MARKER(0x000010d0, "G3L_17");
    uint32 player = r_u32(0x8009efc4);
    uint32 controls = (r_u32(player + 0x10) & 0x100) == 0 ? game + 0x70 : 0x800a0d28;
    uint32 buttons = r_u32(controls + 4);
    player = r_u32(game + 0x50);
    uint32 player_state = r_u32(player + 0x24);
    bool active = false;
    if ((buttons & 0x20) != 0)
    {
        active = r_u16(player + 0x6e) == 0x33;
        if (!active)
        {
            uint32 flags = r_u32(player + 0x128);
            w_u16(player + 0x6e, 0x33);
            w_u16(player + 0x5e, 0);
            uint32 count = r_u32(player + 0x138);
            w_u32(player + 0x128, flags & 0xfffefff7);
            w_u32(player + 0x138, count + 1);
        }
    }
    if (!active) active = r_u16(player + 0x6e) == 0x33;
    if (!active) return;
    uint32 destination = r_u32(player_state + 0xfc);
    uint32 value = r_u32(player_state + 0xf0);
    w_u32(destination, value);
    destination = r_u32(player_state + 0x108);
    value = r_u32(player_state + 0xf0);
    w_u32(destination, value);
    destination = r_u32(player_state + 0x114);
    value = r_u32(player_state + 0xf0);
    w_u32(destination, value);
}

void G3SnowboardGround(uint32 object, uint32 game, uint32 state, uint32 settings)
{
    FUNCTION_MARKER(0x00001198, "G3L_17");
    sint32 mode = r_s16(state + 2);
    if (mode == 0)
    {
        uint32 player = r_u32(0x8009efc4);
        sint32 player_z = r_s16(player + 0x5c);
        sint32 floor = r_s16(player + 0xe0);
        if (player_z - 64 < floor && floor < player_z + 256)
        {
            sint32 z = r_s16(object + 0x5c);
            sint32 difference = z - floor;
            if (difference < 0) difference = -difference;
            if (difference >= 5)
            {
                player = r_u32(0x8009efc4);
                w_u16(object + 0x5c, r_u16(player + 0xe0));
            }
            w_u16(state + 0xe, 0);
            return;
        }
    }
    else if (mode == 1) w_u16(state + 2, 2);
    else if (mode == 2)
    {
        uint32 player = r_u32(0x8009efc4);
        sint32 player_z = r_s16(player + 0x5c);
        sint32 floor = r_s16(player + 0xe0);
        uint32 floor_bits = r_u16(player + 0xe0);
        if (player_z - 64 < floor && floor < player_z + 256)
        {
            w_u16(object + 0x5c, floor_bits);
            w_u16(state + 0xe, 0);
            w_u16(state + 2, 0);
        }
    }
    else if (mode == 3)
    {
        uint32 player = r_u32(game + 0x50);
        if ((r_u32(player + 0x14) & 0x10) != 0) w_u16(state + 2, 0);
        return;
    }
    else return;
    uint32 z = r_u16(object + 0x5c);
    uint32 velocity = r_u16(state + 0xe);
    w_u16(object + 0x5c, z + velocity);
    velocity = r_u16(state + 0xe);
    uint32 gravity = r_u16(settings + 8);
    w_u16(state + 0xe, velocity + gravity);
}

void G3SnowboardReleaseAttachment(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000012fc, "G3L_17");
    uint32 player = r_u32(0x8009efc4);
    uint32 frame = guest_stack - 0x18;
    uint32 controls = (r_u32(player + 0x10) & 0x100) == 0 ? game + 0x70 : 0x800a0d28;
    uint32 buttons = r_u32(controls + 4);
    player = r_u32(game + 0x50);
    uint32 player_state = r_u32(player + 0x24);
    if ((buttons & 0x40) == 0) return;
    uint32 attachment = r_u32(player_state + 0x17c);
    if (attachment != 0)
    {
        G3Function800772F0(attachment, game, frame);
        w_u32(player_state + 0x17c, 0);
    }
}

void G3SnowboardRide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001374, "G3L_17");
    uint32 frame = guest_stack - 0x30;
    uint32 player = r_u32(game + 0x50);
    uint32 player_state = r_u32(player + 0x24);
    G3SnowboardFooting(state);
    G3SnowboardMove(object, game, state, settings, module_base, frame);
    G3SnowboardRequestJump(object, game, state, settings);
    G3SnowboardTrick(game);
    G3SnowboardReleaseAttachment(game, frame);
    G3SnowboardGround(object, game, state, settings);
    G3SnowboardFollowPlatform(object, state, settings, frame);
    if (r_u8(state + 0x30) != 0) w_u16(state, 3);
    w_u16(player + 0x58, r_u16(object + 0x58));
    w_u16(player + 0x5a, r_u16(object + 0x5a));
    if (r_s16(state) == 3)
    {
        uint32 heading = (uint32)r_s16(player + 0x64) - 0x400;
        uint32 sine = G3Function8005B470(heading);
        heading = (uint32)r_s16(player + 0x64) - 0x400;
        w_u32(player + 0x104, (uint32)((sint32)sine >> 5));
        uint32 cosine = G3Function8005B590(heading);
        w_u32(player + 0x108, (uint32)((sint32)cosine >> 5));
        w_u32(player + 0x10c, 150);
        w_u32(player_state + 0x1fc, 0);
    }
}

void G3SnowboardUpdate(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000014c0, "G3L_17");
    uint32 frame = guest_stack - 0x30;
    sint32 mode = r_s16(state);
    switch (mode)
    {
    case 0:
    case 4: break;
    case 1:
    {
        uint32 player = r_u32(game + 0x50);
        w_u16(player + 0x5e, 0);
        player = r_u32(0x8009efc4);
        w_u16(state, 2);
        w_u16(state + 2, 0);
        for (uint32 axis = 0; axis != 3; ++axis)
            w_u16(state + 0x18 + axis * 2, r_u16(player + 0xb4 + axis * 2));
        G3Function8006FF1C(player, game, module_base + 0x1b38, frame);
        w_u32(object + 0x10, r_u32(object + 0x10) | 0x800);
        break;
    }
    case 2:
    {
        G3SnowboardRide(object, game, state, settings, module_base, frame);
        if (r_s16(settings + 4) != 0) G3SnowboardBank(object, state, settings, frame);
        uint32 player = r_u32(game + 0x50);
        uint32 face = r_u32(player + 0xa8);
        bool slippery = face != 0;
        if (slippery) slippery = (r_u16(face + 6) & 15) == 2;
        uint32 flags = r_u32(object + 0x10);
        w_u32(object + 0x10, slippery ? flags | 0x20000000 : flags & 0xdfffffff);
        player = r_u32(game + 0x50);
        flags = r_u32(player + 0x10);
        w_u32(player + 0x10, slippery ? flags | 0x20000000 : flags & 0xdfffffff);
        G3Function800358C0(0x6a, 600, frame);
        break;
    }
    case 3:
    {
        uint32 player = r_u32(0x8009efc4);
        w_u16(state, 0);
        G3Function8006FF1C(player, game, module_base + 0x1b4c, frame);
        uint32 flags = r_u32(object + 0x10);
        player = r_u32(0x8009efc4);
        w_u16(object + 0x60, 0);
        w_u16(object + 0x62, 0);
        w_u32(object + 0x10, flags & 0xfffff7ff);
        w_u16(player + 0x60, 0);
        w_u16(player + 0x62, 0);
        break;
    }
    default: break;
    }
    if ((uint32)mode < 5) mode = r_s16(state);
    uint32 request = 0;
    if (mode == 2 && r_s16(state + 2) == 0)
    {
        uint32 player = r_u32(0x8009efc4);
        if (r_u16(player + 0x6e) != 0x33)
        {
            uint32 x = r_u16(object + 0x58);
            uint32 previous = r_u16(object + 0x70);
            x -= previous;
            uint32 signed_x = (uint32)(sint32)(sint16)x;
            uint32 xx = signed_x * signed_x;
            sint32 count = r_s16(settings + 0xc);
            uint32 speeds = r_u32(settings + 0x10);
            uint32 maximum = (uint32)r_s16(speeds + (uint32)count * 2 - 2);
            w_u16(frame + 0x10, x);
            uint32 y = r_u16(object + 0x5a);
            previous = r_u16(object + 0x72);
            y -= previous;
            uint32 signed_y = (uint32)(sint32)(sint16)y;
            uint32 yy = signed_y * signed_y;
            w_u16(frame + 0x12, y);
            uint32 z = r_u16(object + 0x5c);
            previous = r_u16(object + 0x74);
            z -= previous;
            uint32 signed_z = (uint32)(sint32)(sint16)z;
            uint32 zz = signed_z * signed_z;
            uint32 denominator = maximum * maximum;
            uint32 distance = xx + yy + zz;
            sint32 numerator = (sint32)(distance * 400);
            sint32 quotient = denominator == 0 ? (numerator < 0 ? 1 : -1) : numerator / (sint32)denominator;
            w_u16(frame + 0x14, z);
            request = ((sint32)denominator >> 5) < (sint32)distance ? 1 : 0;
            w_u16(module_base + 0x1b74, (uint32)quotient - 200);
        }
    }
    G3Function80039F5C(0, module_base + 0x1b6c, request, frame);
}

void G3SnowboardCollide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000017b4, "G3L_17");
    uint32 frame = guest_stack - 0x40;
    uint32 contact = r_u32(object + 0xb0);
    uint32 player = r_u32(0x8009efc4);
    sint32 mode = r_s16(state);
    uint32 other = r_u32(contact + 0x14);
    uint32 player_state = r_u32(player + 0x24);
    uint32 delta = contact + 0x28;
    bool skip_player_check = mode == 0 && other != player;
    if (mode == 0 && other == player && r_u32(other + 0x154) != 0x4000000)
    {
        sint32 kind = r_s16(contact + 6);
        if (kind == 1)
        {
            uint32 descriptor = r_u32(contact + 0xc);
            if (r_u8(descriptor + 4) == 0 && r_u32(player_state + 0x1fc) == 0 && r_u8(state + 0x30) == 0)
            {
                w_u16(state, kind);
                w_u16(state + 4, r_u16(object + 0x64));
                G3Function8006F50C(object, game, state, settings, frame);
                w_u8(player_state + 0x200, r_u8(settings + 6));
                player = r_u32(game + 0x50);
                w_u16(player + 0x6e, 0x31);
                player = r_u32(game + 0x50);
                w_u16(player + 0x5e, 0);
                return;
            }
        }
    }
    if (!skip_player_check && other == r_u32(0x8009efc4)) return;
    sint32 side = r_s16(contact + 4);
    sint32 kind = r_s16(contact + 6);
    uint32 first_descriptor = r_u32(contact + 8);
    uint32 descriptor = r_u32(contact + 0xc);
    bool translate = false;
    if (kind == 3) translate = (r_u16(descriptor + 6) & 15) != 8;
    if (!translate && kind == 1) translate = (r_u32(descriptor) & 0x2000) != 0;
    if (!translate && kind == 5) translate = (r_u16(descriptor) & 0x2000) != 0;
    if (!translate && kind == 2) translate = (r_u16(descriptor + 6) & 15) != 8;
    if (translate)
    {
        if (r_s16(state + 2) == 2)
        {
            translate = side == 1;
            if (translate) translate = r_u8(first_descriptor + 4) != 2;
        }
    }
    if (translate)
    {
        bool reset_speed = true;
        if (kind == 3)
        {
            uint32 level = r_u32(game + 0x48);
            uint32 mesh = r_u32(level);
            uint32 index = (uint32)r_s16(descriptor + 8);
            uint32 normals = r_u32(mesh + 0x40);
            G3Function8005F304(index, normals, frame + 0x10, frame);
            if (r_s16(frame + 0x14) > 1400) reset_speed = false;
        }
        if (kind == 1 && (r_u32(descriptor) & 0x2000) == 0) reset_speed = false;
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 coordinate = r_u16(object + 0x58 + axis * 2);
            uint32 change = r_u16(delta + axis * 2);
            w_u16(object + 0x58 + axis * 2, coordinate + change);
        }
        G3Function8005F3A8(object, delta);
        player = r_u32(0x8009efc4);
        uint32 change = r_u16(delta);
        uint32 coordinate = r_u16(player + 0x58);
        w_u16(player + 0x58, coordinate + change);
        for (uint32 axis = 1; axis != 3; ++axis)
        {
            coordinate = r_u16(player + 0x58 + axis * 2);
            change = r_u16(delta + axis * 2);
            w_u16(player + 0x58 + axis * 2, coordinate + change);
        }
        G3Function8005F3A8(player, delta);
        if (reset_speed) w_u8(state + 0x2e, 1);
    }
    if (other != 0)
    {
        uint32 target = r_u32(other + 0xec);
        if (target != 0)
        {
            player = r_u32(0x8009efc4);
            G3DispatchStateEvent(target, other, game, 0x7da, player, frame);
        }
    }
}

void G3SnowboardDamage(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001aac, "G3L_17");
    uint32 frame = guest_stack - 0x18;
    uint32 payload = r_u32(frame + 0x28);
    uint32 contact = r_u32(payload + 0x14);
    uint32 other = r_u32(contact + 0x14);
    if (other == object || r_s16(state + 2) == 3) return;
    uint32 player = r_u32(0x8009efc4);
    if (r_u16(player + 0x6e) == 0x33) return;
    player = r_u32(game + 0x50);
    uint32 player_state = r_u32(player + 0x24);
    w_u16(state + 2, 3);
    w_u16(player + 0x6e, 16);
    w_u16(player + 0x5e, 0);
    w_u16(player_state + 0x120, 60);
    G3Function80073D44(game, frame);
}

uint32 G3SpnplatEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_32");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3SpnplatInitialize(object, module_base, frame);
        return 1;
    case 2:
        G3SpnplatUpdate(object, frame);
        return 1;
    case 3:
        G3SpnplatCollide(object, game, frame);
        return 1;
    case 9:
        G3SpnplatConfigure(object, payload, module_base, frame);
        return 1;
    default:
        return 0;
    }
}

void G3SpnplatInitialize(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000a0, "G3L_32");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    w_u32(object + 0x104, 1);
    w_u16(state + 0xc, 1300);
    w_u16(state + 0xe, 24);
    uint32 commands = r_u32(object + 0x24);
    if (commands != 0)
        G3SpnplatConfigure(object, commands, module_base, frame);
    commands = r_u32(object + 0x28);
    if (commands != 0)
        G3SpnplatConfigure(object, commands, module_base, frame);
    w_u32(object + 0x150, 0);
}

void G3SpnplatUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000118, "G3L_32");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    uint32 radius = (uint32)r_s16(state + 0xc);
    uint32 intro = r_u32(object + 0x20);
    uint32 step = (uint32)r_s16(state + 0xe);
    if (intro == 0)
    {
        G3Function8002A9AC(object, frame);
        return;
    }
    uint32 buddy_intro = r_u32(state + 0x18);
    if (buddy_intro == 0)
    {
        G3Function8002A9AC(object, frame);
        return;
    }
    if (r_u32(state + 0x14) == 0)
    {
        uint32 buddy = r_u32(buddy_intro + 0x20);
        w_u32(state + 0x14, buddy);
        if (buddy == 0)
            return;
    }
    uint32 mode = r_u32(object + 0x150);
    uint32 buddy = r_u32(state + 0x14);
    if (mode == 1 || mode == 2)
    {
        uint32 bx = (uint32)r_s16(buddy + 0x58);
        uint32 by = (uint32)r_s16(buddy + 0x5a);
        uint32 y = (uint32)r_s16(object + 0x5a);
        uint32 x = (uint32)r_s16(object + 0x58);
        uint32 heading = G3Function8002F240(by - y, bx - x, frame);
        uint32 angle = (uint32)((sint32)(heading << 16) >> 16);
        angle = mode == 1 ? angle + step : angle - step;
        uint32 sine = G3Function8005B470(angle);
        uint32 product = radius * sine;
        x = r_u16(object + 0x58);
        w_u16(buddy + 0x58, x + (uint32)((sint32)product >> 12));
        uint32 cosine = G3Function8005B590(angle);
        product = radius * cosine;
        y = r_u16(object + 0x5a);
        w_u16(buddy + 0x5a, y + (uint32)((sint32)product >> 12));
        w_u32(object + 0x150, 0);
    }
    else if (mode == 3)
    {
        sint32 scale = r_s16(object + 0x68);
        uint32 scale_bits = r_u16(object + 0x68);
        if (scale - 0x80 < 2)
        {
            intro = r_u32(object + 0x20);
            w_u16(object + 0x68, 0);
            w_u16(object + 0x6a, 0);
            w_u16(object + 0x6c, 0);
            uint32 xy = r_u32(intro + 0x10);
            uint32 z = (uint32)r_s16(intro + 0x14);
            w_u32(object + 0x58, xy);
            w_u16(object + 0x5c, z);
            w_u32(object + 0x150, 4);
        }
        else
        {
            w_u16(object + 0x68, scale_bits - 0x80);
            uint32 y = r_u16(object + 0x6a);
            uint32 z = r_u16(object + 0x6c);
            w_u16(object + 0x6a, y - 0x80);
            w_u16(object + 0x6c, z - 0x80);
        }
    }
    else if (mode == 4)
    {
        sint32 scale = r_s16(object + 0x68);
        uint32 scale_bits = r_u16(object + 0x68);
        if (scale + 0x80 < 0x1000)
        {
            w_u16(object + 0x68, scale_bits + 0x80);
            uint32 y = r_u16(object + 0x6a);
            uint32 z = r_u16(object + 0x6c);
            w_u16(object + 0x6a, y + 0x80);
            w_u16(object + 0x6c, z + 0x80);
        }
        else
        {
            w_u16(object + 0x68, 0x1000);
            w_u16(object + 0x6a, 0x1000);
            w_u16(object + 0x6c, 0x1000);
            w_u32(object + 0x150, 0);
        }
    }
    uint32 touched = r_u32(object + 0x154);
    w_u32(state + 4, touched == 1 ? touched : 0);
    if (r_s16(state + 0x10) != 0)
    {
        sint32 velocity = r_s16(state + 0x12);
        uint32 velocity_bits = r_u16(state + 0x12);
        uint32 z = r_u16(object + 0x5c);
        intro = r_u32(object + 0x20);
        uint32 position = z + velocity_bits;
        w_u16(object + 0x5c, position);
        sint32 base = r_s16(intro + 0x14);
        sint32 amplitude = r_s16(state + 0x10);
        sint32 boundary = velocity < 0 ? base - amplitude : base + amplitude;
        uint32 base_bits = r_u16(intro + 0x14);
        uint32 amplitude_bits = r_u16(state + 0x10);
        uint32 clamp = velocity < 0 ? base_bits - amplitude_bits : base_bits + amplitude_bits;
        sint32 signed_position = (sint32)(position << 16) >> 16;
        if ((velocity < 0 && signed_position <= boundary) || (velocity >= 0 && signed_position >= boundary))
        {
            w_u16(object + 0x5c, clamp);
            w_u16(state + 0x12, 0 - r_u16(state + 0x12));
        }
    }
    w_u32(state + 8, 0);
    w_u32(object + 0x154, 0);
}

void G3SpnplatCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000043c, "G3L_32");
    uint32 frame = guest_stack - 0x18;
    uint32 mode = r_u32(object + 0x150);
    uint32 contact = r_u32(object + 0xb0);
    uint32 state = object + 0x104;
    if (mode - 3 < 2)
        return;
    uint32 other = r_u32(contact + 0x14);
    uint32 player = r_u32(game + 0x50);
    if (other == player)
    {
        uint32 direction = r_u32(object + 0x104);
        w_u32(object + 0x154, 1);
        w_u32(object + 0x150, direction);
        return;
    }
    if (r_s16(contact + 6) != 3 || r_u32(state + 8) != 0)
        return;
    w_u32(state + 8, 1);
    uint32 dz = r_u16(contact + 0x2c);
    uint32 z = r_u16(object + 0x5c);
    w_u16(object + 0x5c, z + (dz << 1));
    if (r_u32(contact + 0x28) != 0)
    {
        uint32 dx = r_u16(contact + 0x28);
        uint32 x = r_u16(object + 0x58);
        uint32 buddy = r_u32(state + 0x14);
        w_u16(object + 0x58, x + (dx << 1));
        uint32 dy = r_u16(contact + 0x2a);
        uint32 y = r_u16(object + 0x5a);
        w_u16(object + 0x5a, y + (dy << 1));
        uint32 direction = r_u32(buddy + 0x104);
        w_u32(buddy + 0x104, direction == 1 ? 2 : 1);
        uint32 current_buddy = r_u32(state + 0x14);
        direction = r_u32(buddy + 0x104);
        w_u32(current_buddy + 0x150, direction);
    }
    G3Function8005F3A8(object, contact + 0x28);
    (void)frame;
}

void G3SpnplatConfigure(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000544, "G3L_32");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    if (commands == 0)
        return;
    if (G3Function80085AB0(commands, module_base + 0x68c, frame) != 0)
    {
        uint32 intro = r_u32(object + 0x20);
        sint32 x = r_s16(object + 0x58);
        sint32 original_x = r_s16(intro + 0x10);
        if (x != original_x)
        {
            sint32 y = r_s16(object + 0x5a);
            sint32 original_y = r_s16(intro + 0x12);
            if (y != original_y && r_u32(object + 0x150) != 3)
                w_u32(object + 0x150, 3);
        }
    }
    uint32 value = G3Function80085AB0(commands, module_base + 0x698, frame);
    if (value != 0)
        w_u16(state + 0xc, r_u16(value));
    value = G3Function80085AB0(commands, module_base + 0x6a4, frame);
    if (value != 0)
        w_u16(state + 0xe, r_u16(value));
    value = G3Function80085AB0(commands, module_base + 0x6b0, frame);
    if (value != 0)
    {
        sint32 amplitude = r_s16(value);
        sint32 velocity = r_s16(state + 0x12);
        if (amplitude < 0)
            amplitude = -amplitude;
        w_u16(state + 0x10, (uint32)amplitude);
        if (velocity == 0)
        {
            uint32 intro = r_u32(object + 0x20);
            sint32 z = r_s16(object + 0x5c);
            sint32 original_z = r_s16(intro + 0x14);
            if (z == original_z)
                w_u16(state + 0x12, 0xfffc);
        }
    }
    value = G3Function80085AB0(commands, module_base + 0x6bc, frame);
    if (value != 0)
        w_u32(state + 0x18, r_u32(value));
}

uint32 G3SuckyEvent(uint32 object, uint32 game, uint32 message, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_23");
    uint32 frame = guest_stack - 0x18;
    if (message == 1)
    {
        G3SuckyInitialize(object);
        return 1;
    }
    if (message == 2)
    {
        G3SuckyUpdate(object, game, frame);
        return 1;
    }
    return 0;
}

void G3SuckyInitialize(uint32 object)
{
    FUNCTION_MARKER(0x0000005c, "G3L_23");
    uint32 data = r_u32(object + 0x28);
    uint32 state = object + 0x128;
    w_u16(object + 0x128, data != 0 ? r_u16(data + 4) : 0);
    w_u16(state + 2, data != 0 ? r_u16(data + 6) : 0);
    w_u16(state + 4, data != 0 ? r_u16(data + 8) : 0);
    w_u32(state + 8, 0);
    w_u32(state + 0xc, 12);
    w_u32(state + 0x10, 0);
    w_u32(state + 0x14, data != 0 ? r_u32(data) : 150);
}

void G3SuckyUpdate(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000d8, "G3L_23");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x128;
    uint32 player = r_u32(game + 0x50);
    uint32 previous = r_u32(state + 8);
    uint32 player_state = r_u32(player + 0x24);
    w_u32(state + 8, 0);
    uint32 camera = r_u32(game + 0x4c);
    if (G3Function8004DABC(camera, frame) == 6)
    {
        camera = r_u32(game + 0x4c);
        if (G3Function8004DB2C(camera, frame) != 0)
        {
            uint32 count = previous + 1;
            sint32 threshold = r_s32(state + 0xc);
            w_u32(state + 8, count);
            if (threshold < (sint32)count && r_s32(object + 0x14) < 0 && G3SuckyProject(object, game, frame) != 0)
            {
                player = r_u32(game + 0x50);
                if (r_u32(player + 0xf4) == 0 && r_s16(player_state + 0x120) == 0)
                {
                    w_u16(player_state + 0x120, 35);
                    uint32 result = G3Function8001932C(0x40000000, object, frame);
                    player = r_u32(game + 0x50);
                    w_u32(player + 0xf4, result);
                    camera = r_u32(game + 0x4c);
                    G3Function80052060(camera, 30, frame);
                    uint32 duration = r_u32(state + 0x14);
                    w_u32(state + 8, 0);
                    w_u32(state + 0x10, duration);
                }
            }
        }
    }
    sint32 remaining = r_s32(state + 0x10);
    uint32 flags;
    if (remaining > 0)
    {
        w_u32(state + 0x10, (uint32)remaining - 1);
        w_u32(player_state + 0x234, object);
        flags = r_u32(object + 0x10) | 0x400;
    }
    else
    {
        w_u32(player_state + 0x234, 0);
        flags = r_u32(object + 0x10) & 0xfffffbff;
    }
    w_u32(object + 0x10, flags);
}

uint32 G3SuckyProject(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000224, "G3L_23");
    uint32 matrix = r_u32(object + 0x30);
    uint32 frame = guest_stack - 0x40;
    for (uint32 group = 0; group < 2; ++group)
    {
        uint32 offset = group * 16;
        uint32 first = r_u32(matrix + offset);
        uint32 second = r_u32(matrix + offset + 4);
        uint32 third = r_u32(matrix + offset + 8);
        uint32 fourth = r_u32(matrix + offset + 12);
        w_u32(frame + 0x20 + offset, first);
        w_u32(frame + 0x24 + offset, second);
        w_u32(frame + 0x28 + offset, third);
        w_u32(frame + 0x2c + offset, fourth);
    }
    uint32 offset = (uint32)r_s16(object + 0x128);
    w_u32(frame + 0x34, r_u32(frame + 0x34) + offset);
    offset = (uint32)r_s16(object + 0x12a);
    w_u32(frame + 0x38, r_u32(frame + 0x38) + offset);
    offset = (uint32)r_s16(object + 0x12c);
    w_u32(frame + 0x3c, r_u32(frame + 0x3c) + offset);
    uint32 camera = r_u32(game + 0x4c);
    uint32 view = r_u32(camera + 0x78);
    uint32 first = r_u32(view);
    uint32 second = r_u32(view + 4);
    xport_gte_write_control(0, first);
    xport_gte_write_control(1, second);
    uint32 third = r_u32(view + 8);
    uint32 fourth = r_u32(view + 12);
    uint32 fifth = r_u32(view + 16);
    xport_gte_write_control(2, third);
    xport_gte_write_control(3, fourth);
    xport_gte_write_control(4, fifth);
    for (uint32 column = 0; column < 3; ++column)
    {
        uint32 source = frame + 0x20 + column * 2;
        first = r_u16(source);
        second = r_u16(source + 6);
        third = r_u16(source + 12);
        xport_gte_write_data(9, first);
        xport_gte_write_data(10, second);
        xport_gte_write_data(11, third);
        xport_gte_execute(0x49e012);
        first = xport_gte_read_data(9);
        second = xport_gte_read_data(10);
        third = xport_gte_read_data(11);
        uint32 destination = frame + column * 2;
        w_u16(destination, first);
        w_u16(destination + 6, second);
        w_u16(destination + 12, third);
    }
    camera = r_u32(game + 0x4c);
    view = r_u32(camera + 0x78);
    uint32 tx = r_u32(view + 0x14);
    uint32 ty = r_u32(view + 0x18);
    xport_gte_write_control(5, tx);
    uint32 tz = r_u32(view + 0x1c);
    xport_gte_write_control(6, ty);
    xport_gte_write_control(7, tz);
    uint32 y = r_u16(frame + 0x38);
    uint32 x = r_u16(frame + 0x34);
    xport_gte_write_data(0, x | (y << 16));
    xport_gte_write_data(1, r_u32(frame + 0x3c));
    xport_gte_execute(0x480012);
    w_u32(frame + 0x14, xport_gte_read_data(25));
    w_u32(frame + 0x18, xport_gte_read_data(26));
    w_u32(frame + 0x1c, xport_gte_read_data(27));
    uint32 depth = r_u32(frame + 0x1c);
    uint32 projected_x = r_u32(frame + 0x14);
    uint32 projected_y = r_u32(frame + 0x18);
    if (depth - 0x141 >= 0x81f)
        return 0;
    sint32 screen_x = (sint32)(((projected_x << 2) + projected_x) << 6) / (sint32)depth;
    sint32 screen_y = (sint32)(((projected_y << 2) + projected_y) << 6) / (sint32)depth;
    return screen_x > -50 && screen_x < 50 && screen_y > -50 && screen_y < 50 ? 1 : 0;
}

uint32 G3TankEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_27");
    uint32 frame = guest_stack - 0x18;
    uint32 settings = 0;
    uint32 state = 0;
    if (object != 0)
    {
        uint32 descriptor = r_u32(object + 0x1c);
        state = object + 0x104;
        settings = r_u32(descriptor + 0x1c);
    }
    if (settings == 0) return 1;
    switch (message)
    {
    case 1: G3TankInitialize(object, state, settings); return 1;
    case 2: G3TankCleanup(object, state); return 1;
    case 3: G3TankCollide(object, game, state, settings, module_base, frame); return 1;
    case 4: G3Function80081B7C(0, 0x5d50, frame); return 1;
    case 0x7ea: G3TankUpdate(object, game, state, settings, module_base, frame); return 1;
    case 0x7e2:
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 coordinate = r_u16(object + 0x58 + axis * 2);
            uint32 change = r_u16(payload + axis * 2);
            w_u16(object + 0x58 + axis * 2, coordinate + change);
        }
        return 1;
    case 0x7ed:
    {
        uint32 source = object + 0x58;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 lane = source & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
        uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
        uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
        uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        uint32 last = (uint32)r_s16(object + 0x5c);
        high_address = payload + 3;
        high_lane = high_address & 3;
        lane = payload & 3;
        w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
        w_masked_u32(payload & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
        w_u16(payload + 4, last);
        w_u16(payload + 4, r_u16(payload + 4) + 800);
        return 1;
    }
    case 0x7eb:
        w_u16(object + 0x58, r_u16(object + 0x58) - 700);
        w_u8(state + 0x2f, 1);
        return 1;
    case 0x7f1:
    case 0x7f2:
        if (r_s16(state) != 1) return 0;
        break;
    case 0x7f3: break;
    default: return 0;
    }
    uint32 player = r_u32(0x8009efc4);
    sint32 z = r_s16(object + 0x5c);
    sint32 floor = r_s16(player + 0xe0);
    sint32 difference = z - floor;
    if (difference < 0) difference = -difference;
    if (difference >= 5) return 0;
    if (message != 0x7f3 && r_s16(state + 6) != 0) return 0;
    return 1;
}

void G3TankInitialize(uint32 object, uint32 state, uint32 settings)
{
    FUNCTION_MARKER(0x0000024c, "G3L_27");
    w_u16(state, 0);
    w_u16(state + 0xa, 0);
    w_u16(state + 0x16, 0);
    w_u16(state + 0x18, 0);
    w_u16(state + 0x1a, 0);
    w_u16(state + 0xc, 0);
    w_u8(state + 0x30, 0);
    uint32 animations = r_u32(settings);
    uint32 animation = r_u8(animations + 2);
    uint32 flags = r_u32(object + 0x10);
    w_u16(object + 0x5e, 0);
    w_u32(object + 0x10, flags | 0x20080);
    w_u16(object + 0x6e, animation);
}

uint32 G3TankSteer(uint32 game, uint32 object, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000003dc, "G3L_27");
    uint32 frame = guest_stack - 0x30;
    uint32 target = 0;
    sint32 direction = 0;
    uint32 y = r_u32(game + 0xa8);
    bool input = y != 0;
    if (!input) input = r_u32(game + 0xa4) != 0;
    if (input)
    {
        sint32 x = r_s32(game + 0xa4);
        if (x < -64)
        {
            uint32 current = r_u16(state + 2);
            uint32 step = r_u16(settings + 0x30);
            target = current + step;
        }
        else if (x > 64)
        {
            uint32 current = r_u16(state + 2);
            uint32 step = r_u16(settings + 0x30);
            target = current - step + 0x1000;
        }
        sint32 forward = r_s32(game + 0xa8);
        if (forward < -64) direction = 1;
        else if (forward > 64) direction = -1;
        uint32 signed_target = (uint32)(sint32)(sint16)target;
        if (signed_target != 0)
        {
            uint32 step = (uint32)r_s16(settings + 0x30);
            G3Function80038AA8(state + 2, signed_target, step);
            uint32 heading = (uint32)r_s16(state + 2);
            step = (uint32)r_s16(settings + 0x32);
            G3Function80038AA8(object + 0x64, heading, step);
        }
        if (r_u8(state + 0x30) == 0)
        {
            w_u32(frame + 0x10, 0xffffffff);
            w_u32(frame + 0x14, 1);
            G3Function8008194C(0, 1, 0x5d50, 0x2b8, frame);
            w_u8(state + 0x30, 1);
        }
    }
    else if (r_u8(state + 0x30) != 0)
    {
        G3Function80081B7C(0, 0x5d50, frame);
        w_u8(state + 0x30, 0);
    }
    w_u16(state + 4, target);
    return (uint32)direction;
}

void G3TankSpeed(uint32 input, uint32 state, uint32 settings)
{
    FUNCTION_MARKER(0x00000540, "G3L_27");
    sint32 direction = (sint16)input;
    bool accelerate = false;
    if (direction > 0) accelerate = r_u8(state + 0x2d) == 0;
    else if (direction < 0) accelerate = r_u8(state + 0x2d) != 0;
    if (accelerate) accelerate = r_u8(state + 0x2a) != 0;
    if (accelerate)
    {
        sint32 count = r_s16(settings + 0x36);
        uint32 table = r_u32(settings + 0x3c);
        sint32 maximum = r_s16(table + (uint32)count * 2 - 2) - 1;
        sint32 speed = r_s16(state + 6);
        uint32 speed_bits = r_u16(state + 6);
        if (speed < maximum) w_u16(state + 6, speed_bits + 1);
    }
    else
    {
        sint32 speed = r_s16(state + 6);
        sint32 step = r_s16(settings + 0x40);
        uint32 step_bits = r_u16(settings + 0x40);
        uint32 speed_bits = r_u16(state + 6);
        if (step < speed)
        {
            w_u16(state + 6, speed_bits - step_bits);
            return;
        }
        w_u16(state + 6, 0);
        if (direction < 0 && r_u8(state + 0x2d) == 0) w_u8(state + 0x2d, 1);
        else w_u8(state + 0x2d, 0);
    }
}

void G3TankAnimate(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000614, "G3L_27");
    uint32 frame = guest_stack - 0x20;
    sint32 speed = r_s16(state + 6);
    uint32 player = r_u32(game + 0x50);
    if (speed != 0)
    {
        w_u16(object + 0x9e, r_u16(object + 0x9e) + 1);
        uint32 phase = r_u16(player + 0x5e) & 7;
        uint32 random = G3SdkRandom80027FA8();
        if (phase == (random & 7))
        {
            uint32 heading = (uint32)(sint32)(sint16)(r_u16(object + 0x64) + 0x600);
            G3TankDust(object + 0x58, heading, 16, module_base, frame);
            heading = (uint32)(sint32)(sint16)(r_u16(object + 0x64) + 0x200);
            G3TankDust(object + 0x58, heading, 16, module_base, frame);
        }
    }
    G3Function8002A1C8(object, 0xffffffff, frame);
    if ((r_u32(object + 0x14) & 0x10) != 0)
    {
        uint32 animations = r_u32(settings);
        w_u16(object + 0x6e, r_u8(animations));
    }
}

void G3TankIdle(uint32 object, uint32 state, uint32 settings)
{
    FUNCTION_MARKER(0x00000ea0, "G3L_27");
    uint32 animations = r_u32(settings);
    uint32 animation = r_u8(animations + 1);
    w_u16(object + 0x5e, 0);
    w_u16(object + 0x6e, animation);
    w_u8(state + 0x2c, 0);
}

void G3TankGround(uint32 object, uint32 state, uint32 settings)
{
    FUNCTION_MARKER(0x000010ac, "G3L_27");
    uint32 player = r_u32(0x8009efc4);
    sint32 player_z = r_s16(player + 0x5c);
    sint32 floor = r_s16(player + 0xe0);
    if (player_z - 64 < floor && floor < player_z + 256)
    {
        sint32 difference = r_s16(object + 0x5c) - floor;
        if (difference < 0) difference = -difference;
        if (difference >= 5)
        {
            player = r_u32(0x8009efc4);
            w_u16(object + 0x5c, r_u16(player + 0xe0));
        }
        w_u16(state + 0xa, 0);
    }
    else
    {
        uint32 z = r_u16(object + 0x5c);
        uint32 velocity = r_u16(state + 0xa);
        w_u16(object + 0x5c, z + velocity);
        velocity = r_u16(state + 0xa);
        uint32 gravity = r_u16(settings + 0x34);
        w_u16(state + 0xa, velocity + gravity);
    }
    if (r_s16(state + 0xa) < -100) w_u16(state + 0xa, 0xff9c);
}

void G3TankCleanup(uint32 object, uint32 state)
{
    FUNCTION_MARKER(0x00001564, "G3L_27");
    w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
    if (r_s16(state) == 2)
    {
        uint32 timer = r_u8(state + 0x2e) - 1;
        w_u8(state + 0x2e, timer);
        if ((timer & 255) == 0) w_u16(state, 0);
    }
}

void G3TankDust(uint32 position, uint32 heading, uint32 speed, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000290, "G3L_27");
    uint32 frame = guest_stack - 0x40;
    uint32 source = module_base + 0x1838;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 lane = source & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
    uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
    uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
    uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
    uint32 last = (uint32)r_s16(source + 4);
    uint32 destination = frame + 0x10;
    high_address = destination + 3;
    high_lane = high_address & 3;
    lane = destination & 3;
    w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
    w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
    w_u16(frame + 0x14, last);
    uint32 resource = r_u32(0x80098724);
    if (resource == 0) return;
    heading = (uint32)(sint32)(sint16)heading;
    uint32 sine = G3Function8005B470(heading);
    uint32 x = (uint32)((sint32)(sine * speed) >> 12);
    uint32 cosine = G3Function8005B590(heading);
    uint32 y = (uint32)((sint32)(cosine * speed) >> 12);
    uint32 random = G3SdkRandom80027FA8();
    uint32 origin = r_u16(position);
    w_u16(frame + 0x18, origin + x * 8 + (random & 31) - 7);
    random = G3SdkRandom80027FA8();
    origin = r_u16(position + 2);
    w_u16(frame + 0x1a, origin + y * 8 + (random & 31) - 7);
    random = G3SdkRandom80027FA8();
    origin = r_u16(position + 4);
    w_u16(frame + 0x1c, origin + (random & 15) + 5);
    uint32 models = r_u32(resource + 0xc);
    uint32 pool = r_u32(0x800a27fc);
    uint32 model = r_u32(models);
    uint32 effect = G3Function800444E4(frame + 0x18, model, pool, 20, frame);
    if (effect != 0)
    {
        G3Function800449F0(effect, x, y, 8, effect);
        G3Function80044878(effect, frame + 0x10, 0);
    }
}
