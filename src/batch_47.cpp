#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void G3HitchupCollide(void)
{
    FUNCTION_MARKER(0x0000059c, "G3L_60");
}

uint32 G3IceCubeEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_00");
    uint32 frame = guest_stack - 0x20;
    switch (message)
    {
    case 1:
        G3IceCubeInitialize(object);
        return 1;
    case 2:
        G3IceCubeUpdate(object, game, module_base, frame);
        return 1;
    case 3:
        G3IceCubeCollide(object, game, module_base, frame);
        return 1;
    case 8:
        if ((r_u32(object + 0x14) & 2) == 0)
            G3IceCubeBreak(object, game, 3, module_base, frame);
        return 1;
    case 2013:
        G3IceCubeRestore(object);
        return 1;
    case 4:
    {
        uint32 restored_z = G3IceCubeDetach(object, frame);
        G3Function8003A114(object + 0x138, restored_z);
        G3Function80038798(object, 0, 0, frame);
        return 1;
    }
    default:
        return 0;
    }
}

void G3IceCubeInitialize(uint32 object)
{
    FUNCTION_MARKER(0x00000120, "G3L_00");
    uint32 x = r_u16(object + 0x58);
    uint32 y = r_u16(object + 0x5a);
    uint32 z = r_u16(object + 0x5c);
    w_u32(object + 0x110, 0xfffffff6);
    w_u16(object + 0x68, 0x1900);
    w_u16(object + 0x6a, 0x1900);
    w_u16(object + 0x6c, 0x1900);
    w_u32(object + 0x114, 0);
    w_u16(object + 0x70, x);
    w_u16(object + 0x72, y);
    w_u16(object + 0x74, z);
    uint32 state = object + 0x114;
    w_u32(state + 0x20, 300);
    w_u16(state + 0x28, 0x90f);
    w_u16(state + 0x2e, 100);
    w_u32(state + 4, 0);
    w_u32(state + 0xc, 0);
    w_u32(state + 0x10, 0);
    w_u32(state + 0x14, 0);
    w_u32(state + 0x18, 0);
    w_u32(state + 0x1c, 0);
    w_u16(state + 0x34, 0);
    w_u32(state + 0x24, 0);
    w_u16(state + 0x2c, 0);
    w_u16(state + 0x2a, 0x157c);
    w_u16(state + 0x30, 0);
}

void G3IceCubeRestore(uint32 object)
{
    FUNCTION_MARKER(0x000001a4, "G3L_00");
    uint32 x = r_u16(object + 0x70);
    uint32 y = r_u16(object + 0x72);
    uint32 z = r_u16(object + 0x74);
    w_u32(object + 0x134, 300);
    w_u16(object + 0x58, x);
    w_u16(object + 0x5a, y);
    w_u16(object + 0x5c, z);
}

uint32 G3IceCubeDetach(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000001c8, "G3L_00");
    uint32 frame = guest_stack - 0x20;
    uint32 flags = r_u32(object + 0x10);
    uint32 attached = r_u32(object + 0x114);
    w_u32(object + 0x10, flags & 0xfffffbff);
    flags = r_u32(attached + 0x10);
    uint32 state = object + 0x114;
    w_u32(attached + 0x10, flags & 0xfffffbff);
    attached = r_u32(object + 0x114);
    uint32 mask = r_u32(state + 0x18);
    G3Function80037150(attached, mask, frame);
    attached = r_u32(object + 0x114);
    uint32 callback = r_u32(state + 0xc);
    w_u32(attached + 0xec, callback);
    attached = r_u32(object + 0x114);
    w_u16(attached + 0x68, r_u16(state + 0x14));
    attached = r_u32(object + 0x114);
    w_u16(attached + 0x6a, r_u16(state + 0x14));
    attached = r_u32(object + 0x114);
    w_u16(attached + 0x6c, r_u16(state + 0x14));
    attached = r_u32(object + 0x114);
    uint32 z = r_u16(object + 0x5c);
    w_u16(attached + 0x5c, z);
    return z;
}

void G3IceCubeUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000274, "G3L_00");
    uint32 frame = guest_stack - 0x40;
    uint32 sound = object + 0x138;
    uint32 attached = r_u32(object + 0x114);
    uint32 state = object + 0x114;
    uint32 resource = r_u32(attached + 0x1c);
    uint32 request = (uint32)r_s16(state + 0x34);
    uint32 name = r_u32(resource + 0x24);
    G3Function80039F5C(object, sound, request, frame);
    uint32 flags = r_u32(object + 0x10);
    attached = r_u32(object + 0x114);
    w_u32(object + 0x10, flags | 0x400);
    flags = r_u32(attached + 0x10);
    w_u32(attached + 0x10, flags | 0x400);
    uint32 lifetime = r_u32(state + 0x20) - 1;
    w_u32(state + 0x20, lifetime);
    if ((sint32)lifetime <= 0)
    {
        uint32 x = r_u16(object + 0x58);
        w_u16(frame + 0x20, x);
        w_u16(frame + 0x18, x);
        uint32 y = r_u16(object + 0x5a);
        w_u16(frame + 0x22, y);
        w_u16(frame + 0x1a, y);
        uint32 z = r_u16(object + 0x5c);
        w_u16(frame + 0x1c, z + 0x100);
        z = r_u16(object + 0x5c);
        w_u16(frame + 0x24, z - 0x100);
        uint32 expected = r_u32(module_base + 0xac8);
        uint32 actual = r_u32(name);
        bool myth = false;
        if (expected == actual)
        {
            expected = r_u32(module_base + 0xacc);
            actual = r_u32(name + 4);
            myth = expected == actual;
        }
        bool ground = false;
        if (!myth)
        {
            uint32 level = r_u32(game + 0x48);
            uint32 terrain = r_u32(level);
            ground = G3Function800622EC(terrain, frame + 0x20, frame + 0x18, object, frame) != 0;
        }
        if (ground)
        {
            uint32 restored_z = G3IceCubeDetach(object, frame);
            G3Function8003A114(sound, restored_z);
            G3Function80038798(object, 0, 0, frame);
        }
        else
            G3IceCubeBreak(object, game, 0, module_base, frame);
        return;
    }
    uint32 velocity = r_u32(object + 0x10c);
    uint32 gravity = r_u32(object + 0x110);
    velocity += gravity;
    w_u32(object + 0x10c, velocity);
    if ((sint32)velocity < -128)
        velocity = 0xffffff80;
    uint32 x = r_u16(object + 0x58);
    uint32 y = r_u16(object + 0x5a);
    uint32 z = r_u16(object + 0x5c);
    w_u32(object + 0x10c, velocity);
    uint32 dy = r_u16(object + 0x108);
    w_u16(object + 0x74, z);
    uint32 dx = r_u16(object + 0x104);
    w_u16(object + 0x72, y);
    w_u16(object + 0x5a, y + dy);
    uint32 dz = r_u16(object + 0x10c);
    attached = r_u32(object + 0x114);
    w_u16(object + 0x70, x);
    w_u16(object + 0x58, x + dx);
    z = r_u16(object + 0x5c);
    uint32 moved_x = r_u16(object + 0x58);
    w_u16(object + 0x5c, z + dz);
    w_u16(attached + 0x58, moved_x);
    attached = r_u32(object + 0x114);
    w_u16(attached + 0x5a, r_u16(object + 0x5a));
    z = r_u16(object + 0x5c);
    attached = r_u32(object + 0x114);
    w_u16(attached + 0x5c, z + 37);
    attached = r_u32(object + 0x114);
    uint32 mask = r_u32(state + 0x18);
    G3Function800370E8(attached, mask, frame);
    uint32 yaw = r_u16(object + 0x64);
    uint32 turn = r_u16(state + 8);
    attached = r_u32(object + 0x114);
    yaw += turn;
    w_u16(object + 0x64, yaw);
    w_u16(attached + 0x64, yaw);
    turn = r_u32(state + 8);
    if (turn != 0)
        w_u32(state + 8, (sint32)turn < 0 ? turn + 1 : turn - 1);
    w_u16(state + 0x34, 0);
}

void G3IceCubeBreak(uint32 object, uint32 game, uint32 pieces, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000004bc, "G3L_00");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x114;
    uint32 resource = G3Function8003AA48(module_base + 0xad4);
    G3IceCubeDetach(object, frame);
    uint32 attached = r_u32(object + 0x114);
    uint32 callback = r_u32(attached + 0xec);
    uint32 handled = 0;
    if (callback != 0)
        handled = G3DispatchStateEvent(callback, attached, game, 19, 0, frame);
    uint32 release_result;
    if (handled != 0)
        release_result = handled;
    else
    {
        uint32 remaining = pieces - 1;
        if (resource != 0 && remaining != 0xffffffff)
        {
            do
            {
                attached = r_u32(state);
                G3Function8002A454(attached, resource, frame);
                --remaining;
            } while (remaining != 0xffffffff);
        }
        attached = r_u32(state);
        uint32 flags = r_u32(attached + 0x14);
        w_u32(attached + 0x14, flags | 4);
        attached = r_u32(state);
        release_result = G3Function80038864(attached, frame);
    }
    G3Function8003A114(state + 0x24, release_result);
    G3Function80038798(object, 0, 0, frame);
}

void G3IceCubeCollide(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000005b4, "G3L_00");
    uint32 frame = guest_stack - 0x40;
    uint32 state = object + 0x114;
    uint32 contact = r_u32(object + 0xb0);
    uint32 player = r_u32(game + 0x50);
    uint32 other = r_u32(contact + 0x14);
    uint32 player_data = r_u32(player + 0x24);
    uint32 delta = contact + 0x28;
    if (other == player)
    {
        sint32 original_kind = r_s16(contact + 6);
        if (original_kind != 1)
            return;
        uint32 surface = r_u32(contact + 0xc);
        if (r_u8(surface + 4) != 0)
            return;
        uint32 y = (uint32)r_s16(object + 0x5a);
        uint32 player_y = (uint32)r_s16(other + 0x5a);
        uint32 x = (uint32)r_s16(object + 0x58);
        uint32 player_x = (uint32)r_s16(other + 0x58);
        uint32 heading = G3Function8002F240(y - player_y, x - player_x, frame);
        uint32 facing = r_u16(player_data + 0xc8);
        uint32 angle_bits = (heading + 0x400 - facing) & 0xfff;
        if (angle_bits >= 0x800)
            angle_bits |= 0xf000;
        if (r_s16(contact + 6) == original_kind)
        {
            sint32 angle = (sint16)angle_bits;
            if (angle < 0)
                angle = -angle;
            if (angle > 0x300)
                return;
        }
        if (r_s16(player_data + 0xe0) < 3)
            return;
        sint32 abs_x = r_s16(contact + 0x28);
        sint32 abs_z = r_s16(delta + 4);
        if (abs_x < 0)
            abs_x = -abs_x;
        sint32 abs_y = r_s16(delta + 2);
        if (abs_z < 0)
            abs_z = -abs_z;
        if (abs_y < 0)
            abs_y = -abs_y;
        if (abs_x > abs_y)
        {
            sint32 swap = abs_x;
            abs_x = abs_y;
            abs_y = swap;
        }
        if (abs_y > abs_z)
        {
            sint32 swap = abs_y;
            abs_y = abs_z;
            abs_z = swap;
        }
        if (abs_x > abs_y)
        {
            sint32 swap = abs_x;
            abs_x = abs_y;
            abs_y = swap;
        }
        uint32 magnitude = (uint32)((30 * abs_z + 12 * abs_y + 9 * abs_x) >> 5);
        uint32 angle = (uint32)r_s16(player_data + 0xc8) - 0x400;
        uint32 sine = G3Function8005B470(angle);
        uint32 product = sine * magnitude;
        w_u16(frame + 0x10, (uint32)((sint32)product >> 12));
        angle = (uint32)r_s16(player_data + 0xc8) - 0x400;
        uint32 cosine = G3Function8005B590(angle);
        product = cosine * magnitude;
        uint32 dx = r_u16(frame + 0x10);
        w_u16(frame + 0x14, 0);
        w_u16(frame + 0x12, (uint32)((sint32)product >> 12));
        x = r_u16(object + 0x58);
        w_u16(object + 0x58, x + dx);
        y = r_u16(object + 0x5a);
        uint32 dy = r_u16(frame + 0x12);
        w_u16(object + 0x5a, y + dy);
        G3Function8005F3A8(object, frame + 0x10);
        uint32 callback = r_u32(player + 0xec);
        if (callback != 0)
        {
            G3DispatchStateEvent(callback, player, game, 1005, 3, frame);
            callback = r_u32(player + 0xec);
            if (callback != 0)
                G3DispatchStateEvent(callback, player, game, 1007, 0, frame);
        }
        uint32 pushed = r_u32(state + 0x1c);
        w_u16(state + 0x34, 1);
        if (pushed != 0)
        {
            w_u32(state + 0x1c, 0);
            w_u16(delta, r_u16(frame + 0x10));
            w_u16(delta + 2, r_u16(frame + 0x12));
        }
        else
        {
            w_u16(delta, 0);
            w_u16(delta + 2, 0);
            w_u16(delta + 4, 0);
        }
        return;
    }
    uint32 saved_z_delta = r_u16(delta + 4);
    if (other != 0)
    {
        uint32 resource = r_u32(other + 0x1c);
        if (resource != 0)
        {
            uint32 name = r_u32(resource + 0x20);
            uint32 expected = r_u32(module_base + 0xae0);
            uint32 actual = r_u32(name);
            bool cube = actual == expected;
            if (cube)
            {
                actual = r_u32(name + 4);
                expected = r_u32(module_base + 0xae4);
                cube = actual == expected;
            }
            if (cube)
            {
                uint32 pair = r_u32(contact + 4);
                if (pair == 0x20001)
                {
                    w_u16(contact + 0x28, 0);
                    w_u16(delta + 2, 0);
                    pair = r_u32(contact + 4);
                }
                if (pair == 0x10002)
                    w_u16(delta + 4, 0);
                pair = r_u32(contact + 4);
                if (pair == 0x10001)
                {
                    sint32 vertical = r_s16(delta + 4);
                    if (vertical < 0)
                        vertical = -vertical;
                    if (vertical >= 9)
                    {
                        w_u16(contact + 0x28, 0);
                        w_u16(delta + 2, 0);
                        w_u16(delta + 4, 0);
                    }
                }
            }
            else
            {
                other = r_u32(contact + 0x14);
                resource = r_u32(other + 0x1c);
                name = r_u32(resource + 0x24);
                expected = r_u32(module_base + 0xaec);
                actual = r_u32(name);
                bool spit = actual == expected;
                if (spit)
                {
                    actual = r_u32(name + 4);
                    expected = r_u32(module_base + 0xaf0);
                    spit = actual == expected;
                }
                if (spit)
                {
                    uint32 x = r_u16(object + 0x70);
                    uint32 y = r_u16(object + 0x72);
                    uint32 z = r_u16(object + 0x74);
                    w_u16(object + 0x58, x);
                    w_u16(object + 0x5a, y);
                    w_u16(object + 0x5c, z);
                    return;
                }
            }
        }
    }
    uint32 kind = r_u16(contact + 6);
    if (kind - 2 < 2)
    {
        sint32 vx = r_s32(object + 0x104);
        w_u32(object + 0x10c, 0);
        if (vx > 0)
            w_u32(object + 0x104, vx < 6 ? 0 : (uint32)vx - 5);
        vx = r_s32(object + 0x104);
        if (vx < 0)
            w_u32(object + 0x104, vx < -5 ? (uint32)vx + 5 : 0);
        sint32 vy = r_s32(object + 0x108);
        if (vy > 0)
        {
            w_u32(object + 0x108, vy < 6 ? 0 : (uint32)vy - 5);
            vy = r_s32(object + 0x108);
        }
        if (vy < 0)
            w_u32(object + 0x108, vy < -5 ? (uint32)vy + 5 : 0);
    }
    sint32 dx = r_s16(delta);
    if (dx < 0)
        dx = -dx;
    bool pushed = dx >= 9;
    if (!pushed)
    {
        sint32 dy = r_s16(delta + 2);
        if (dy < 0)
            dy = -dy;
        pushed = dy >= 9;
    }
    if (pushed)
        w_u32(state + 0x1c, 1);
    uint32 x = r_u16(object + 0x58);
    uint32 offset_x = r_u16(delta);
    w_u16(object + 0x58, x + offset_x);
    uint32 y = r_u16(object + 0x5a);
    uint32 offset_y = r_u16(delta + 2);
    w_u16(object + 0x5a, y + offset_y);
    uint32 z = r_u16(object + 0x5c);
    uint32 offset_z = r_u16(delta + 4);
    w_u16(object + 0x5c, z + offset_z);
    G3Function8005F3A8(object, delta);
    w_u16(delta + 4, saved_z_delta);
}

uint32 G3KangaEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_03");
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
    case 1: G3KangaInitialize(object, game, state, frame); return 1;
    case 2: return 1;
    case 3: G3KangaCollide(object, game, state, settings, frame); return 1;
    case 0x7ea: G3KangaUpdate(object, game, state, settings, module_base, frame); return 1;
    case 0x7e2:
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 coordinate = r_u16(object + 0x58 + axis * 2);
            uint32 offset = r_u16(payload + axis * 2);
            w_u16(object + 0x58 + axis * 2, coordinate + offset);
        }
        return 1;
    case 0x7f1:
    case 0x7f2:
        if (r_s16(state + 2) != 0) return 0;
        break;
    case 0x7f3: break;
    default: return 0;
    }
    uint32 player = r_u32(0x8009efc4);
    sint32 z = r_s16(object + 0x5c);
    sint32 floor = r_s16(player + 0xe0);
    sint32 distance = z - floor;
    if (distance < 0) distance = -distance;
    if (distance >= 5) return 0;
    if (message != 0x7f3 && r_s16(state + 0xa) != 0) return 0;
    return 1;
}

void G3KangaInitialize(uint32 object, uint32 game, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000001c4, "G3L_03");
    uint32 frame = guest_stack - 0x18;
    w_u32(object + 0x10, r_u32(object + 0x10) | 0x100000);
    G3Function80082988(object, 0, game, frame);
    w_u16(state, 0);
    w_u16(state + 0xe, 0);
    w_u16(state + 0x18, 0);
    w_u16(state + 0x1a, 0);
    w_u16(state + 0x1c, 0);
    w_u16(state + 6, 0);
    w_u8(state + 0x33, 0);
    w_u8(state + 0x34, 0);
}

void G3KangaDust(uint32 position, uint32 heading, uint32 speed, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000224, "G3L_03");
    uint32 frame = guest_stack - 0x40;
    uint32 source = module_base + 0x1484;
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

uint32 G3KangaSteer(uint32 game, uint32 object, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000370, "G3L_03");
    uint32 frame = guest_stack - 0x30;
    uint32 heading = 0;
    uint32 target = 0;
    uint32 y = r_u32(game + 0xa8);
    bool input = y != 0;
    if (!input) input = r_u32(game + 0xa4) != 0;
    if (input)
    {
        uint32 x = r_u32(game + 0xa4);
        heading = G3Function8002F240(0 - y, x, frame) + 0x400;
        if ((uint16)heading == 0) heading = 0x1000;
        y = r_u32(game + 0xa8);
        uint32 y_squared = y * y;
        x = r_u32(game + 0xa4);
        if ((sint32)(y_squared + x * x) < 0x2400)
        {
            uint32 player = r_u32(0x8009efc4);
            w_u32(player + 0x128, r_u32(player + 0x128) | 0x20);
        }
    }
    uint32 result = (uint32)(sint32)(sint16)heading;
    if (result != 0) target = heading + G3Function80014F00(game);
    if (r_s16(state + 2) == 5) target = 0;
    uint32 signed_target = (uint32)(sint32)(sint16)target;
    if (signed_target != 0)
    {
        uint32 step = (uint32)r_s16(settings + 4);
        G3Function80038AA8(state + 4, signed_target, step);
        uint32 current = (uint32)r_s16(state + 4);
        step = (uint32)r_s16(settings + 6);
        G3Function80038AA8(object + 0x64, current, step);
    }
    w_u16(state + 8, target);
    return result;
}

void G3KangaSpeed(uint32 heading, uint32 state, uint32 settings)
{
    FUNCTION_MARKER(0x000004c0, "G3L_03");
    bool accelerate = (uint16)heading != 0;
    if (accelerate) accelerate = r_u8(state + 0x31) != 0;
    if (accelerate)
    {
        sint32 count = r_s16(settings + 0x10);
        uint32 table = r_u32(settings + 0x1c);
        sint32 maximum = r_s16(table + (uint32)count * 2 - 2) - 1;
        sint32 speed = r_s16(state + 0xa);
        uint32 speed_bits = r_u16(state + 0xa);
        if (speed < maximum) w_u16(state + 0xa, speed_bits + 1);
    }
    else
    {
        sint32 speed = r_s16(state + 0xa);
        sint32 step = r_s16(settings + 0x20);
        uint32 step_bits = r_u16(settings + 0x20);
        uint32 speed_bits = r_u16(state + 0xa);
        if (step < speed) w_u16(state + 0xa, speed_bits - step_bits);
        else w_u16(state + 0xa, 0);
    }
}

void G3KangaAnimate(uint32 object, uint32 game, uint32 state, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000540, "G3L_03");
    uint32 frame = guest_stack - 0x20;
    sint32 speed = r_s16(state + 0xa);
    uint32 player = r_u32(game + 0x50);
    if (speed != 0)
    {
        uint32 descriptor = r_u32(object + 0x1c);
        if (r_u32(descriptor + 0x10) != 0)
        {
            G3Function80029F0C(object, 0xffffffff, frame);
            if ((r_u32(object + 0x14) & 0x10) != 0)
            {
                w_u16(object + 0x6e, 0);
                w_u16(player + 0x6e, 49);
                w_u8(state + 0x34, 0);
            }
        }
        w_u16(player + 0x5e, r_u16(object + 0x5e));
        uint32 jump_bits = r_u16(state + 2);
        if (jump_bits - 4 >= 2 && r_u8(state + 0x34) == 0) w_u16(object + 0x6e, 0);
        sint32 jump = r_s16(state + 2);
        if (jump != 0 && jump != 4 && jump != 5 && r_u8(state + 0x34) == 0) w_u16(object + 0x6e, 1);
        uint32 phase = r_u16(player + 0x5e) & 7;
        uint32 random = G3SdkRandom80027FA8();
        if (phase == (random & 7) && r_s16(state + 2) == 0)
        {
            uint32 heading = (uint32)(sint32)(sint16)(r_u16(object + 0x64) + 0x400);
            G3KangaDust(object + 0x58, heading, 4, module_base, frame);
        }
    }
    else
    {
        sint32 jump = r_s16(state + 2);
        if (jump != 0 && jump != 4 && jump != 5 && r_u8(state + 0x34) == 0) w_u16(object + 0x6e, 1);
        uint32 descriptor = r_u32(object + 0x1c);
        if (r_u32(descriptor + 0x10) != 0)
        {
            G3Function80029F0C(object, 0xffffffff, frame);
            if ((r_u32(object + 0x14) & 0x10) != 0)
            {
                uint32 random = G3SdkRandom80027FA8() & 0xfff;
                uint32 animation = random < 0x199 ? 6 : random < 0x332 ? 7 : random < 0x4cb ? 8 : 5;
                w_u16(object + 0x6e, animation);
                w_u16(player + 0x6e, 41);
                w_u8(state + 0x34, 0);
            }
        }
        w_u16(player + 0x5e, r_u16(object + 0x5e));
    }
}

void G3KangaMove(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000072c, "G3L_03");
    uint32 frame = guest_stack - 0x60;
    uint32 player = r_u32(0x8009efc4);
    uint32 player_data = r_u32(player + 0x24);
    uint32 heading = G3KangaSteer(game, object, state, settings, frame);
    w_u16(player_data + 0xbc, heading);
    G3KangaSpeed((uint32)(sint32)(sint16)heading, state, settings);
    if (r_u8(state + 0x31) != 0)
    {
        G3FillMemory80086094(frame + 0x10, 0, 8);
        uint32 thresholds = r_u32(settings + 0x1c);
        sint32 speed = r_s16(state + 0xa);
        sint32 threshold = r_s16(thresholds);
        uint32 index = 0;
        if (speed >= threshold)
        {
            do
            {
                ++index;
                sint32 offset = (sint32)(sint16)index * 2;
                threshold = r_s16(thresholds + (uint32)offset);
            } while (speed >= threshold);
        }
        sint32 offset = (sint32)(sint16)index * 2;
        w_u16(state + 0xc, index);
        uint32 steps = r_u32(settings + 0x14);
        uint32 step = r_u16(steps + (uint32)offset);
        w_u16(frame + 0x12, 0 - step);
        G3Function8002F718(frame + 0x20);
        G3Function8005BB6C((uint32)r_s16(state + 4), frame + 0x20, frame);
        G3Function8005BA5C((uint32)r_s16(object + 0x62), frame + 0x20, frame);
        G3Function8005B94C((uint32)r_s16(object + 0x60), frame + 0x20, frame);
        ApplyMatrixSV((MATRIX *)psx_addr(frame + 0x20, 20), (SVECTOR *)psx_addr(frame + 0x10, 8), (SVECTOR *)psx_addr(frame + 0x18, 6));
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 coordinate = r_u16(object + 0x58 + axis * 2);
            uint32 change = r_u16(frame + 0x18 + axis * 2);
            w_u16(object + 0x58 + axis * 2, coordinate + change);
        }
        for (uint32 axis = 0; axis != 3; ++axis) w_u16(player_data + 0x26e + axis * 2, r_u16(frame + 0x18 + axis * 2));
    }
    if (r_s16(state + 2) == 5)
    {
        uint32 coordinate = r_u16(object + 0x58);
        uint32 change = r_u16(state + 0x2c);
        w_u16(object + 0x58, coordinate + change);
        coordinate = r_u16(object + 0x5a);
        change = r_u16(state + 0x2e);
        w_u16(object + 0x5a, coordinate + change);
    }
    G3KangaAnimate(object, game, state, module_base, frame);
}

void G3KangaFooting(uint32 game, uint32 state)
{
    FUNCTION_MARKER(0x00000918, "G3L_03");
    uint32 player = r_u32(0x8009efc4);
    uint32 player_data = r_u32(player + 0x24);
    w_u8(state + 0x31, 1);
    uint32 face = r_u32(player_data + 0x140);
    if (face != 0)
    {
        if (r_s16(player_data + 0x14c) >= 0xb51)
        {
            w_u8(state + 0x32, 1);
            return;
        }
        uint32 material = r_u16(face + 6);
        uint32 level = r_u32(game + 0x48);
        uint32 table = r_u32(level + 0xf0);
        uint32 flags = r_u32(table + ((material & 0x1e0) >> 1));
        if ((flags & 8) != 0)
        {
            w_u8(state + 0x32, 1);
            return;
        }
    }
    w_u8(state + 0x32, 0);
}

void G3KangaFollowPlatform(uint32 object, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000990, "G3L_03");
    uint32 frame = guest_stack - 0x20;
    if (r_u8(state + 0x32) != 0) return;
    uint32 player = r_u32(0x8009efc4);
    sint32 z = r_s16(object + 0x5c);
    sint32 floor = r_s16(player + 0xe0);
    sint32 difference = z - floor;
    if (difference < 0) difference = -difference;
    if (difference >= 5) return;
    player = r_u32(0x8009efc4);
    uint32 scale = r_u8(settings + 0x28) * 8;
    G3Function80036278(player + 0xb4, frame + 0x10, scale, frame);
    if ((sint32)G3Function8002FA50(frame + 0x10, frame) < 100)
    {
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 coordinate = r_u16(object + 0x58 + axis * 2);
            uint32 change = r_u16(frame + 0x10 + axis * 2);
            w_u16(object + 0x58 + axis * 2, coordinate + change);
        }
    }
}

void G3KangaReact(uint32 object, uint32 state)
{
    FUNCTION_MARKER(0x00000a64, "G3L_03");
    if (r_u8(state + 0x33) == 0) return;
    w_u16(object + 0x6e, 4);
    w_u16(object + 0x5e, 0);
    w_u16(state + 2, 5);
    w_u8(state + 0x33, 0);
}

void G3KangaRequestJump(uint32 object, uint32 game, uint32 state, uint32 settings)
{
    FUNCTION_MARKER(0x00000a90, "G3L_03");
    uint32 player = r_u32(0x8009efc4);
    uint32 controls = (r_u32(player + 0x10) & 0x100) != 0 ? 0x800a0d28 : game + 0x70;
    if ((r_u32(controls + 4) & 0x80) == 0) return;
    if (r_u8(state + 0x32) == 0 || r_s16(state + 2) != 0 || r_u8(state + 0x34) != 0) return;
    sint32 offset = (r_s16(state + 0xc) >> 1) * 2;
    uint32 thresholds = r_u32(settings + 0x1c);
    sint32 speed = r_s16(state + 0xa);
    sint32 threshold = r_s16(thresholds + (uint32)offset);
    if (speed < threshold)
    {
        uint32 index = r_u8(settings + 0x27);
        w_u16(state + 0xc, index);
        thresholds = r_u32(settings + 0x1c);
        w_u16(state + 0xa, r_u16(thresholds + index * 2) - 1);
    }
    sint32 index = r_s16(state + 0xc);
    uint32 jumps = r_u32(settings + 0x18);
    uint32 velocity = r_u16(jumps + (uint32)index * 2);
    w_u16(state + 2, 1);
    w_u16(state + 0xe, velocity);
    w_u16(object + 0x5e, 0);
}

void G3KangaRequestAttack(uint32 object, uint32 game, uint32 state)
{
    FUNCTION_MARKER(0x00000b84, "G3L_03");
    uint32 player = r_u32(0x8009efc4);
    uint32 controls = (r_u32(player + 0x10) & 0x100) != 0 ? 0x800a0d28 : game + 0x70;
    if (r_u8(state + 0x34) != 0 || (r_u32(controls + 4) & 0x20) == 0) return;
    sint32 jump = r_s16(state + 2);
    uint32 animation = jump == 0 || jump == 4 || jump == 5 ? 2 : 3;
    w_u16(object + 0x6e, animation);
    w_u16(object + 0x5e, 0);
    w_u8(state + 0x34, 1);
    w_u8(state + 0x35, 0);
    player = r_u32(0x8009efc4);
    w_u16(state + 0xa, 0);
    w_u32(player + 0x138, r_u32(player + 0x138) + 1);
}

void G3KangaJump(uint32 object, uint32 state, uint32 settings, uint32 module_base)
{
    FUNCTION_MARKER(0x00000c30, "G3L_03");
    uint32 mode = (uint32)r_s16(state + 2);
    if (mode >= 6) return;
    uint32 target = r_u32(module_base + 0x148c + mode * 4) - module_base;
    bool integrate = false;
    switch (target)
    {
    case 0xc60:
        if ((r_u32(object + 0x14) & 0x10) != 0) w_u16(state + 2, 0);
    case 0xc78:
    {
        uint32 player = r_u32(0x8009efc4);
        sint32 player_z = r_s16(player + 0x5c);
        sint32 floor = r_s16(player + 0xe0);
        if (player_z - 0x40 < floor && floor < player_z + 0x100)
        {
            sint32 object_z = r_s16(object + 0x5c);
            sint32 difference = object_z - floor;
            if (difference < 0) difference = -difference;
            if (difference >= 5)
            {
                player = r_u32(0x8009efc4);
                w_u16(object + 0x5c, r_u16(player + 0xe0));
            }
            w_u16(state + 0xe, 0);
        }
        else
        {
            uint32 z = r_u16(object + 0x5c);
            uint32 velocity = r_u16(state + 0xe);
            w_u16(object + 0x5c, z + velocity);
            velocity = r_u16(state + 0xe);
            uint32 gravity = r_u16(settings + 0xc);
            w_u16(state + 0xe, velocity + gravity);
        }
        if (r_u8(state + 0x34) != 0 && r_s16(state + 2) == 0) w_u16(state + 0xa, 0);
        return;
    }
    case 0xd48:
        w_u16(state + 2, 2);
        integrate = true;
        break;
    case 0xd54:
    {
        uint32 player = r_u32(0x8009efc4);
        sint32 player_z = r_s16(player + 0x5c);
        sint32 floor = r_s16(player + 0xe0);
        uint32 floor_bits = r_u16(player + 0xe0);
        if (player_z - 0x40 < floor && floor < player_z + 0x100)
        {
            w_u16(object + 0x5c, floor_bits);
            sint32 speed = r_s16(state + 0xa);
            w_u16(state + 0xe, 0);
            w_u16(state + 2, 0);
            w_u16(object + 0x6e, speed != 0 ? 0 : 5);
            w_u8(state + 0x34, 0);
        }
        integrate = true;
        break;
    }
    case 0xdd8: return;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "Unresolved Kanga jump target %08X\n", target + module_base);
#endif
        }
        abort();
    }
    if (integrate)
    {
        uint32 z = r_u16(object + 0x5c);
        uint32 velocity = r_u16(state + 0xe);
        w_u16(object + 0x5c, z + velocity);
        velocity = r_u16(state + 0xe);
        uint32 gravity = r_u16(settings + 0xc);
        w_u16(state + 0xe, velocity + gravity);
    }
}

void G3KangaRide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000de0, "G3L_03");
    uint32 frame = guest_stack - 0x28;
    uint32 player = r_u32(game + 0x50);
    G3KangaFooting(game, state);
    G3KangaMove(object, game, state, settings, module_base, frame);
    G3KangaJump(object, state, settings, module_base);
    G3KangaRequestJump(object, game, state, settings);
    G3KangaRequestAttack(object, game, state);
    G3KangaReact(object, state);
    G3KangaFollowPlatform(object, state, settings, frame);
    for (uint32 axis = 0; axis != 3; ++axis) w_u16(player + 0x58 + axis * 2, r_u16(object + 0x58 + axis * 2));
}

void G3KangaUpdate(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000ed0, "G3L_03");
    uint32 frame = guest_stack - 0x18;
    uint32 player = r_u32(0x8009efc4);
    uint32 mode = r_u32(player + 0x150);
    if (mode - 4 >= 2)
    {
        uint32 level = r_u32(game + 0x48);
        sint32 z = r_s16(player + 0x5c);
        sint32 minimum = r_s32(level + 0x6c);
        if (z < minimum)
        {
            G3Function80074AB0(player, game, frame);
            return;
        }
    }
    w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
    sint32 activity = r_s16(state);
    if (activity == 0 || activity == 4)
    {
        if (r_u8(settings + 8) != 0) G3Function80029F0C(object, 0xffffffff, frame);
    }
    else if (activity == 2)
    {
        player = r_u32(0x8009efc4);
        w_u32(object + 0xf8, r_u32(player + 0xf8));
        uint32 words[2];
        for (uint32 i = 0; i != 2; ++i)
        {
            uint32 source = player + 0xfc + i * 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            words[i] = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        }
        for (uint32 i = 0; i != 2; ++i)
        {
            uint32 destination = object + 0xfc + i * 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, words[i] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, words[i] << (lane * 8), (15u << lane) & 15);
        }
        G3KangaRide(object, game, state, settings, module_base, frame);
        player = r_u32(0x8009efc4);
        w_u32(player + 0x10, r_u32(player + 0x10) | 0x800);
    }
}

void G3KangaCollide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000ff8, "G3L_03");
    uint32 frame = guest_stack - 0x60;
    uint32 contact = r_u32(object + 0xb0);
    uint32 player = r_u32(0x8009efc4);
    sint32 activity = r_s16(state);
    uint32 other = r_u32(contact + 0x14);
    uint32 player_data = r_u32(player + 0x24);
    uint32 delta = contact + 0x28;
    bool skip_player_check = false;
    if (activity == 0)
    {
        if (other == player)
        {
            if (r_s16(contact + 6) == 1)
            {
                uint32 descriptor = r_u32(contact + 0xc);
                if (r_u8(descriptor + 4) == 0 && r_u32(player_data + 0x1fc) == 0)
                {
                    w_u16(state, 2);
                    w_u16(state + 4, r_u16(object + 0x64));
                    w_u16(object + 0x6e, 0);
                    w_u16(object + 0x5e, 0);
                    G3Function8006F50C(object, game, state, settings, frame);
                    w_u8(player_data + 0x200, r_u8(settings + 0xb));
                    return;
                }
            }
        }
        else skip_player_check = true;
    }
    if (!skip_player_check && other == r_u32(0x8009efc4)) return;
    if (other != 0)
    {
        G3Function8003C548(contact, frame + 0x30, frame + 0x31);
        uint32 first = r_u8(frame + 0x30);
        if (first - 6 < 4)
        {
            uint32 second = r_u8(frame + 0x31);
            if ((second == 9 || second - 3 < 3) && r_u8(state + 0x34) != 0)
            {
                player = r_u32(0x8009efc4);
                w_u32(frame + 0x18, 1);
                w_u16(frame + 0x28, 0);
                w_u16(frame + 0x2a, 0);
                w_u32(frame + 0x2c, contact);
                w_u32(contact + 0x10, player);
                uint32 target = r_u32(other + 0xec);
                if (target != 0) G3DispatchStateEvent(target, other, game, 8, frame + 0x18, frame);
                if (r_u8(state + 0x35) == 0)
                {
                    G3Function8003F128(0x810, 0);
                    uint32 animation = r_u16(object + 0x6e);
                    uint32 pool;
                    uint32 x;
                    uint32 y;
                    if (animation == 3)
                    {
                        player = r_u32(0x8009efc4);
                        uint32 matrix = r_u32(object + 0x30);
                        pool = r_u32(0x800a27fc);
                        uint32 z = r_u32(matrix + 0x3bc);
                        w_u32(frame + 0x10, z);
                        x = r_u32(matrix + 0x3b4);
                        y = r_u32(matrix + 0x3b8);
                    }
                    else
                    {
                        player = r_u32(0x8009efc4);
                        uint32 matrix = r_u32(object + 0x30);
                        pool = r_u32(0x800a27fc);
                        uint32 first_x = r_u32(matrix + 0x294);
                        uint32 second_x = r_u32(matrix + 0x314);
                        uint32 first_y = r_u32(matrix + 0x298);
                        x = (uint32)((sint32)(first_x + second_x) >> 1);
                        uint32 second_y = r_u32(matrix + 0x318);
                        uint32 second_z = r_u32(matrix + 0x31c);
                        y = (uint32)((sint32)(first_y + second_y) >> 1);
                        uint32 first_z = r_u32(matrix + 0x29c);
                        w_u32(frame + 0x10, (uint32)((sint32)(first_z + second_z) >> 1));
                    }
                    G3Function800440D0(player, pool, x, y, frame);
                    w_u8(state + 0x35, 1);
                    w_u32(frame + 0x10, 6);
                    w_u32(frame + 0x14, 0);
                    G3Function8008194C(0, 1, 0xffffffff, 0x333, frame);
                }
            }
        }
        uint32 second = r_u8(frame + 0x31);
        if (second - 6 < 4)
        {
            first = r_u8(frame + 0x30);
            if ((first == 9 || first - 3 < 3) && r_u8(state + 0x33) == 0 && r_s16(state + 2) != 5)
            {
                w_u16(state + 0xa, 0);
                w_u8(state + 0x33, 1);
                uint32 source = r_u32(other + 0x2c);
                uint32 reaction_y;
                if (source != 0)
                {
                    uint32 source_x = (uint32)r_s16(source + 0x58);
                    uint32 source_y = (uint32)r_s16(source + 0x5a);
                    uint32 other_y = (uint32)r_s16(other + 0x5a);
                    uint32 other_x = (uint32)r_s16(other + 0x58);
                    uint32 angle = G3Function8002F240(source_y - other_y, source_x - other_x, frame);
                    angle = (uint32)(sint32)(sint16)(angle + 0x500);
                    uint32 sine = G3Function8005B470(angle);
                    w_u16(state + 0x2c, (uint32)((sint32)(sine * 10) >> 12));
                    uint32 cosine = G3Function8005B590(angle);
                    reaction_y = (uint32)((sint32)(cosine * 10) >> 12);
                }
                else if ((G3SdkRandom80027FA8() & 1) != 0)
                {
                    sint32 y = r_s16(delta + 2);
                    w_u16(state + 0x2c, (uint32)((-y) >> 1));
                    reaction_y = (uint32)((sint32)(sint16)r_u16(delta) >> 1);
                }
                else
                {
                    sint32 y = (sint16)r_u16(delta + 2);
                    w_u16(state + 0x2c, (uint32)(y >> 1));
                    sint32 x = r_s16(delta);
                    reaction_y = (uint32)((-x) >> 1);
                }
                w_u16(state + 0x2e, reaction_y);
                w_u16(state + 0xa, 0);
            }
        }
    }
    sint32 kind = r_s16(contact + 6);
    uint32 descriptor = r_u32(contact + 0xc);
    bool translate = false;
    if (kind == 1) translate = (r_u32(descriptor) & 0x2000) != 0;
    if (!translate && kind == 5) translate = (r_u16(descriptor) & 0x2000) != 0;
    if (!translate && kind == 2) translate = (r_u16(descriptor + 6) & 15) != 8;
    if (!translate && kind == 3) translate = true;
    if (translate)
    {
        for (uint32 axis = 0; axis != 3; ++axis)
        {
            uint32 coordinate = r_u16(object + 0x58 + axis * 2);
            uint32 change = r_u16(delta + axis * 2);
            w_u16(object + 0x58 + axis * 2, coordinate + change);
        }
        G3Function8005F3A8(object, delta);
        if (r_s16(state) != 0)
        {
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
        }
    }
}

uint32 G3PlayerLauncherState80077578(uint32 object)
{
    uint32 state = r_u32(object + 0x24);
    if (state == 0)
        return 0;
    return (uint32)r_s16(state + 0x122);
}

uint32 G3LauncherEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_70");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3LauncherInitialize(object, frame);
        return 1;
    case 2:
        G3LauncherUpdate(object, game, module_base, frame);
        return 1;
    case 3:
        G3LauncherCollide(object, game, module_base, frame);
        return 1;
    default:
        return 0;
    }
}

void G3LauncherConfigure(uint32 object, uint32 commands, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000007c, "G3L_70");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    w_u32(guest_stack + 4, commands);
    if (commands == 0)
        return;
    w_u32(state + 0x18, 1);
    while (G3Function80085DFC(frame + 0x2c, frame + 0x10, frame + 0x14, frame + 0x18) != 0)
    {
        sint32 kind = r_s16(frame + 0x10);
        switch (kind)
        {
        case 0x11:
        {
            uint32 data = r_u32(frame + 0x14);
            w_u32(state + 0x14, (uint32)r_s16(data));
            break;
        }
        case 0x12:
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0x20, r_u16(data));
            break;
        }
        case 0x13:
            w_u32(state + 0x18, r_u32(state + 0x18) & 0xfffffffe);
            break;
        case 0x14:
            w_u32(state + 0x18, r_u32(state + 0x18) | 2);
            break;
        case 0x20:
        {
            uint32 data = r_u32(frame + 0x14);
            sint32 value = r_s16(data);
            w_u32(state + 0x1c, (uint32)((value * 4096) / 100));
            break;
        }
        case 0x21:
            w_u32(state + 0x18, r_u32(state + 0x18) | 0x10);
            break;
        case 0x97:
            w_u32(state + 0x18, r_u32(state + 0x18) | 0x48);
            break;
        case 5:
            w_u32(state + 0x18, r_u32(state + 0x18) | 0x100);
            break;
        }
    }
}

void G3LauncherInitialize(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000001e0, "G3L_70");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    w_u32(state + 0x10, 0);
    uint32 x = r_u16(object + 0x58);
    uint32 y = r_u16(object + 0x5a);
    uint32 z = r_u16(object + 0x5c);
    uint32 flags = r_u32(object + 0x10);
    w_u16(object + 0x5e, 0);
    w_u32(object + 0x10, flags | 0x800);
    w_u16(object + 0x88, x);
    w_u16(object + 0x8a, y);
    w_u16(object + 0x8c, z);
    w_u32(state + 0x14, 0x500);
    w_u16(state + 0x20, 0x80);
    w_u32(state + 0x1c, 0x1000);
    w_u32(state + 4, 4);
    w_u32(state + 8, 20);
    w_u32(state + 0x18, 0);
    uint32 resource = r_u32(object + 0x1c);
    uint32 commands = r_u32(resource + 0x1c);
    G3LauncherConfigure(object, commands, frame);
    commands = r_u32(object + 0x28);
    G3LauncherConfigure(object, commands, frame);
    if ((r_u32(state + 0x18) & 2) != 0)
    {
        w_u32(state + 0xc, 0x200);
        flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags & 0xfffff7ff);
    }
    w_u16(state + 0x22, r_u16(state + 0x20));
    w_u32(object + 0x150, 0);
}

void G3LauncherUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000002bc, "G3L_70");
    uint32 frame = guest_stack - 0x40;
    uint32 state = object + 0x104;
    uint32 flags = r_u32(state + 0x18);
    uint32 player = r_u32(game + 0x50);
    uint32 player_data = r_u32(player + 0x24);
    if ((flags & 0x80) != 0)
    {
        G3Function80029F0C(object, 0xffffffff, frame);
        if ((r_u32(object + 0x14) & 0x10) != 0)
        {
            flags = r_u32(state + 0x18);
            w_u32(state + 0x18, flags & 0xffffff7f);
            flags = r_u32(object + 0x14);
            w_u16(object + 0x5e, 0);
            w_u32(object + 0x14, flags & 0xffffffef);
        }
    }
    if (r_u32(object + 0x150) == 0 && (r_u32(state + 0x18) & 0x30) == 0x30)
    {
        bool activate = (r_u32(game + 0x74) & 0x80) != 0;
        if (!activate)
        {
            uint32 player_flags = r_u32(player + 0x154);
            activate = player_flags == 0x20 || player_flags == 0x80;
        }
        if (activate)
        {
            if (G3LauncherHasBusyPeer(object, module_base) == 0)
            {
                if (r_s16(player_data + 0xbc) != 0 && (r_u32(state + 0x18) & 0x100) == 0)
                    G3LauncherStartJump(object, game, frame);
                else
                    G3LauncherStartLift(object, game, frame);
            }
            flags = r_u32(state + 0x18);
            if ((flags & 0x40) != 0)
            {
                w_u32(state + 0x18, flags | 0x80);
                flags = r_u32(object + 0x14);
                w_u16(object + 0x5e, 0);
                w_u32(object + 0x14, flags & 0xffffffef);
            }
        }
    }
    uint32 depression = r_u32(state);
    if ((sint32)depression > 0)
    {
        uint32 step = r_u32(state + 4);
        depression -= step;
        w_u32(state, depression);
        if ((sint32)depression <= 0)
        {
            if ((r_u32(state + 0x18) & 2) == 0)
            {
                flags = r_u32(object + 0x10);
                w_u32(object + 0x10, flags | 0x800);
            }
            uint32 source = object + 0x88;
            uint32 destination = object + 0x58;
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
            w_u32(state, 0);
        }
        for (uint32 component = 0; component != 3; ++component)
        {
            uint32 amount = r_u32(state);
            uint32 amplitude = r_u32(state + 0xc);
            sint32 product = (sint32)(amount * amplitude);
            sint32 divisor = r_s32(state + 8);
            uint32 quotient = divisor == 0 ? (product < 0 ? 1 : 0xffffffff) : (uint32)((sint64)product / divisor);
            w_u16(object + 0x68 + component * 2, quotient + 0x1000);
        }
    }
    uint32 mode = r_u32(object + 0x150);
    if (mode == 2)
    {
        flags = r_u32(player + 0x128);
        w_u32(player + 0x128, flags | 0x40);
        w_u16(player_data + 0x188, r_u16(state + 0x1c));
        uint32 count = r_u32(object + 0x154);
        w_u32(object + 0x154, count + 1);
        w_u16(player_data + 0xe2, r_u16(player_data + 0xe4) - 1);
        mode = r_u32(object + 0x150);
    }
    if (mode == 1)
    {
        sint32 speed = r_s16(state + 0x20);
        uint32 height = r_u32(state + 0x14);
        uint32 quotient = speed == 0 ? ((sint32)height < 0 ? 1 : 0xffffffff) : (uint32)((sint64)(sint32)height / speed);
        uint32 terminal = (uint32)speed * (quotient - (uint32)(speed / 32));
        uint32 progress = r_u32(state + 0x10);
        bool finished = (sint32)(progress + (uint32)speed) >= (sint32)height;
        uint32 lift = finished ? height - progress : (uint32)speed;
        w_u16(frame + 0x12, 0);
        w_u16(frame + 0x10, 0);
        w_u16(frame + 0x14, lift);
        uint32 callback = r_u32(player + 0xec);
        if (callback != 0)
            G3DispatchStateEvent(callback, player, game, 2018, frame + 0x10, frame);
        if (finished)
        {
            w_u32(object + 0x150, 3);
            w_u32(state + 0x10, 0);
            flags = r_u32(player + 0x10);
            w_u32(player + 0x10, flags & 0xffbfffff);
        }
        else
        {
            progress = r_u32(state + 0x10) + lift;
            w_u32(state + 0x10, progress);
            if ((sint32)terminal < (sint32)progress)
            {
                sint32 current_speed = r_s16(state + 0x20);
                uint32 speed_bits = r_u16(state + 0x20);
                if (32 < current_speed)
                    w_u16(state + 0x20, speed_bits - 32);
            }
        }
    }
    mode = r_u32(object + 0x150);
    if ((sint32)mode >= 2)
    {
        sint32 count = r_s32(object + 0x154);
        if (count >= 5 || mode != 2)
        {
            bool release = G3Function80078834(player, frame) != 0;
            if (!release)
                release = (r_u32(player + 0x154) & 0x8000) != 0;
            if (release)
            {
                flags = r_u32(object + 0x10);
                w_u32(object + 0x10, flags & 0xfffffbff);
                flags = r_u32(player + 0x128);
                flags &= 0xffffffbf;
                flags &= 0xfffffbff;
                w_u32(player + 0x128, flags);
                w_u32(object + 0x150, 0);
                w_u32(object + 0x154, 0);
            }
        }
    }
    if (r_s32(object + 0x150) > 0)
    {
        uint32 resource = r_u32(0x80098754);
        if (resource != 0)
        {
            uint32 owner = r_u32(resource + 0xc);
            uint32 pool = r_u32(0x800a27fc);
            uint32 effect_resource = r_u32(owner);
            G3Function800444E4(player + 0x58, effect_resource, pool, 10, frame);
        }
    }
    flags = r_u32(state + 0x18);
    w_u32(state + 0x18, flags & 0xffffffdf);
}

void G3LauncherCollide(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000073c, "G3L_70");
    uint32 frame = guest_stack - 0x20;
    uint32 mode = r_u32(object + 0x150);
    uint32 player = r_u32(game + 0x50);
    uint32 contact = r_u32(object + 0xb0);
    uint32 state = object + 0x104;
    if (mode - 1 < 2 || r_u32(object + 0x114) != 0)
        return;
    uint32 other = r_u32(contact + 0x14);
    if (other != player || r_s16(contact + 4) != 5)
        return;
    uint32 primitive = r_u32(contact + 8);
    if (r_u8(primitive + 2) != 0)
        return;
    if ((r_u32(state + 0x18) & 0x10) != 0)
    {
        uint32 flags = r_u32(player + 0x128);
        w_u32(player + 0x128, flags | 0x200);
        flags = r_u32(state + 0x18);
        w_u32(state + 0x18, flags | 0x20);
        return;
    }
    bool activate = false;
    if (G3PlayerLauncherState80077578(other) != 0)
        activate = r_u32(state + 0x18) == 0;
    if (!activate)
        activate = (r_u32(state + 0x18) & 1) != 0;
    if (activate && G3LauncherHasBusyPeer(object, module_base) == 0)
        G3LauncherStartLift(object, game, frame);
}

void G3LauncherPrepare(uint32 object, uint32 game)
{
    FUNCTION_MARKER(0x00000858, "G3L_70");
    uint32 flags = r_u32(object + 0x10);
    uint32 player = r_u32(game + 0x50);
    w_u32(object + 0x10, flags & 0xfffff7ff);
    uint32 state = object + 0x104;
    if ((r_u32(state + 0x18) & 8) == 0)
    {
        w_u32(state, r_u32(state + 8));
        w_u16(object + 0x58, r_u16(player + 0x58));
        w_u16(object + 0x5a, r_u16(player + 0x5a));
    }
}

uint32 G3LauncherHasBusyPeer(uint32 object, uint32 module_base)
{
    FUNCTION_MARKER(0x000008b0, "G3L_70");
    uint32 intro = r_u32(object + 0x20);
    if (intro == 0)
        return 0;
    uint32 list = r_u32(intro + 4);
    if (list == 0)
        return 0;
    uint32 cursor = list + 4;
    sint32 count = r_s32(list);
    if (count <= 0)
        return 0;
    uint32 first_name = r_u32(module_base + 0xaa4);
    sint32 index = 0;
    do
    {
        uint32 entry = r_u32(cursor);
        uint32 definition = r_u32(entry);
        uint32 name = r_u32(definition + 0x20);
        if (r_u32(name) == first_name)
        {
            uint32 last_name = r_u32(name + 4);
            uint32 expected = r_u32(module_base + 0xaa8);
            if (last_name == expected)
            {
                uint32 peer = r_u32(entry + 0x20);
                if (peer != 0)
                {
                    uint32 mode = r_u32(peer + 0x150);
                    if (mode - 1 < 2)
                        return 1;
                    if (r_u32(peer + 0x114) != 0)
                        return 1;
                }
            }
        }
        ++index;
        cursor += 4;
    } while (index < count);
    return 0;
}

void G3LauncherStartJump(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000980, "G3L_70");
    uint32 frame = guest_stack - 0x20;
    uint32 player = r_u32(game + 0x50);
    uint32 flags = r_u32(object + 0x10);
    uint32 player_data = r_u32(player + 0x24);
    w_u32(object + 0x10, flags | 0x400);
    G3LauncherPrepare(object, game);
    w_u32(object + 0x150, 2);
    flags = r_u32(player + 0x128);
    uint32 jump = (uint32)r_s16(player_data + 6);
    w_u32(player + 0x128, flags | 0x400);
    w_u32(player + 0x118, jump);
}

void G3LauncherStartLift(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000009e4, "G3L_70");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    uint32 player = r_u32(game + 0x50);
    uint32 flags = r_u32(object + 0x10);
    uint32 player_data = r_u32(player + 0x24);
    w_u32(object + 0x10, flags | 0x400);
    G3LauncherPrepare(object, game);
    uint32 first_frame = r_u16(player_data + 0x20);
    uint32 second_frame = r_u16(player_data + 0xe4);
    w_u16(player_data + 0xe0, first_frame - 1);
    w_u16(player_data + 0xe2, second_frame - 1);
    w_u32(object + 0x150, 1);
    w_u16(state + 0x20, r_u16(state + 0x22));
    flags = r_u32(player + 0x10);
    w_u32(player + 0x10c, 0);
    w_u32(player + 0x118, 0);
    w_u32(player + 0x10, flags | 0x400000);
    G3Function8006F468(game, frame);
    w_u16(player_data + 0xd0, 0);
    w_u16(player_data + 0xd2, 0);
    w_u16(player_data + 0xd4, 0);
    w_u16(player_data + 0xd8, 0);
    w_u16(player_data + 0xda, 0);
    w_u16(player_data + 0xdc, 0);
}

uint32 G3LeafGeneratorEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_74");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3LeafGeneratorInitialize(object, module_base, frame);
        return 1;
    case 2:
        G3LeafGeneratorUpdate(object, game, module_base, frame);
        return 1;
    case 3:
        G3LeafGeneratorCollide(object, game, frame);
        return 1;
    default:
        return 0;
    }
}

void G3LeafGeneratorInitialize(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000088, "G3L_74");
    uint32 frame = guest_stack - 0x28;
    uint32 source = module_base + 0x76c;
    uint32 destination = frame + 0x10;
    uint32 first_address = source;
    uint32 first_high_address = first_address + 3;
    uint32 first_high_lane = first_high_address & 3;
    uint32 first_lane = first_address & 3;
    uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
    uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
    uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
    uint32 first = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
    uint32 second_address = source + 4;
    uint32 second_high_address = second_address + 3;
    uint32 second_high_lane = second_high_address & 3;
    uint32 second_lane = second_address & 3;
    uint32 second_high = r_masked_u32(second_high_address & 0xfffffffc, (1 << (second_high_lane + 1)) - 1);
    uint32 second_low = r_masked_u32(second_address & 0xfffffffc, (15 << second_lane) & 15);
    uint32 second_keep = second_lane == 0 ? 0 : 0xffffffff << ((4 - second_lane) * 8);
    uint32 second = ((second_high << ((3 - second_high_lane) * 8)) & second_keep) | (second_low >> (second_lane * 8));
    uint32 last = (uint32)r_s8(source + 8);
    uint32 first_output = destination;
    uint32 first_output_high = first_output + 3;
    uint32 first_output_lane = first_output & 3;
    uint32 first_output_high_lane = first_output_high & 3;
    w_masked_u32(first_output_high & 0xfffffffc, first >> ((3 - first_output_high_lane) * 8), (1 << (first_output_high_lane + 1)) - 1);
    w_masked_u32(first_output & 0xfffffffc, first << (first_output_lane * 8), (15 << first_output_lane) & 15);
    uint32 second_output = destination + 4;
    uint32 second_output_high = second_output + 3;
    uint32 second_output_lane = second_output & 3;
    uint32 second_output_high_lane = second_output_high & 3;
    w_masked_u32(second_output_high & 0xfffffffc, second >> ((3 - second_output_high_lane) * 8), (1 << (second_output_high_lane + 1)) - 1);
    w_masked_u32(second_output & 0xfffffffc, second << (second_output_lane * 8), (15 << second_output_lane) & 15);
    w_u8(destination + 8, last);
    uint32 resource = r_u32(object + 0x1c);
    uint32 name = r_u32(object + 0x28);
    uint32 defaults = r_u32(resource + 0x1c);
    uint32 state = object + 0x128;
    if (name == 0)
    {
        name = defaults;
        if (defaults == 0)
        {
            w_u32(object + 0x28, frame + 0x10);
            name = frame + 0x10;
        }
    }
    uint32 selected = G3Function8003AA48(name);
    w_u32(state, selected);
    uint32 random = G3SdkRandom80027FA8();
    w_u32(state + 0x1c, (random & 15) + 1);
}

uint32 G3LeafEffectInitialize(uint32 effect, uint32 callback, uint32 storage, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000011c, "G3L_74");
    uint32 frame = guest_stack - 0x18;
    uint32 position = r_u32(guest_stack + 0x18);
    uint32 data = r_u32(guest_stack + 0x10);
    uint32 x = r_u16(position);
    uint32 first_index = r_u16(data);
    uint32 second_index = r_u16(data + 2);
    uint32 origin_index = r_u16(data + 4);
    w_u16(effect + 0x1c, x);
    w_u16(effect + 0x1e, r_u16(position + 2));
    w_u16(effect + 0x20, r_u16(position + 4));
    uint32 vertices = r_u32(guest_stack + 0x14);
    uint32 first = vertices + (first_index << 3);
    uint32 origin = vertices + (origin_index << 3);
    uint32 a = (uint32)r_s16(first);
    uint32 b = (uint32)r_s16(origin);
    uint32 factor = r_u32(0x8009da44);
    w_u16(effect + 0x24, (a - b) * factor);
    for (uint32 axis = 1; axis < 3; ++axis)
    {
        a = (uint32)r_s16(first + axis * 2);
        b = (uint32)r_s16(origin + axis * 2);
        w_u16(effect + 0x24 + axis * 2, (a - b) * factor);
    }
    uint32 second = vertices + (second_index << 3);
    for (uint32 axis = 0; axis < 2; ++axis)
    {
        a = (uint32)r_s16(second + axis * 2);
        b = (uint32)r_s16(origin + axis * 2);
        w_u16(effect + 0x2c + axis * 2, (a - b) * factor);
    }
    a = (uint32)r_s16(second + 4);
    b = (uint32)r_s16(origin + 4);
    uint32 product = (a - b) * factor;
    uint32 lifetime = r_u16(guest_stack + 0x28);
    w_u16(effect + 0x34, 0);
    w_u16(effect + 0x30, product);
    w_u16(effect + 0x36, 0);
    w_u16(effect + 0x38, 0);
    uint32 color;
    if ((r_u8(data + 7) & 2) != 0)
    {
        w_u32(effect + 0xc, r_u32(effect + 0xc) | 1);
        uint32 texture = r_u32(data + 8);
        w_u32(effect + 8, texture);
        color = (r_u32(texture + 0xc) & 0x03ffffff) | 0x24000000;
    }
    else
    {
        w_u32(effect + 0xc, r_u32(effect + 0xc) & 0xfffffffe);
        color = (r_u32(data + 8) & 0x03ffffff) | 0x20000000;
    }
    w_u32(effect + 0x18, color);
    uint32 vertical = r_u16(0x8009da48);
    uint32 phase = r_u32(0x8009da48);
    w_u32(effect + 0x10, callback);
    w_u32(effect + 0x14, storage);
    w_u16(effect + 0x4c, 0);
    w_u16(effect + 0x4e, 0);
    w_u16(effect + 0x50, 0);
    w_u16(effect + 0x52, 0);
    w_u16(effect + 0x54, 0);
    w_u16(effect + 0x22, lifetime);
    w_u16(effect + 0x56, vertical - 2);
    w_u32(0x8009da48, phase ^ 1);
    uint32 random = G3SdkRandom80027FA8();
    w_u16(effect + 0x44, (random & 63) - 16);
    random = G3SdkRandom80027FA8();
    w_u16(effect + 0x46, (random & 63) - 16);
    random = G3SdkRandom80027FA8();
    uint32 transform = r_u32(effect + 0x14);
    w_u16(effect + 0x48, (random & 63) - 16);
    G3Function8002F718(transform + 0xc);
    random = G3SdkRandom80027FA8();
    transform = r_u32(effect + 0x14);
    G3Function8005B94C(random & 0xfff, transform + 0xc, frame);
    random = G3SdkRandom80027FA8();
    transform = r_u32(effect + 0x14);
    G3Function8005BA5C(random & 0xfff, transform + 0xc, frame);
    random = G3SdkRandom80027FA8();
    transform = r_u32(effect + 0x14);
    return G3Function8005BB6C(random & 0xfff, transform + 0xc, frame);
}

uint32 G3LeafEffectUpdate(uint32 effect, uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000378, "G3L_74");
    uint32 frame = guest_stack - 0x18;
    G3Function8004197C(effect, pool, frame);
    uint32 transform = r_u32(effect + 0x14);
    uint32 angle = (uint32)r_s16(effect + 0x44);
    G3Function8005B94C(angle, transform + 0xc, frame);
    transform = r_u32(effect + 0x14);
    angle = (uint32)r_s16(effect + 0x46);
    G3Function8005BA5C(angle, transform + 0xc, frame);
    transform = r_u32(effect + 0x14);
    angle = (uint32)r_s16(effect + 0x48);
    return G3Function8005BB6C(angle, transform + 0xc, frame);
}

void G3LeafGeneratorEmit(uint32 object, uint32 position, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000003cc, "G3L_74");
    uint32 frame = guest_stack - 0x38;
    uint32 resource = r_u32(object + 0x128);
    if (resource == 0)
        return;
    uint32 models = r_u32(resource + 0xc);
    uint32 descriptor = r_u32(models);
    uint32 random = G3SdkRandom80027FA8();
    w_u32(0x8009da44, (random & 3) + 2);
    uint32 pool = r_u32(0x800a27fc);
    uint32 data = r_u32(descriptor + 0x14);
    w_u32(frame + 0x18, module_base + 0x11c);
    w_u32(frame + 0x1c, module_base + 0x378);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, pool);
    w_u32(frame + 0x20, 40);
    G3Function8004257C(descriptor, data + 0xc, position, 0, frame);
}

void G3LeafGeneratorUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000046c, "G3L_74");
    uint32 frame = guest_stack - 0x28;
    uint32 resource = r_u32(object + 0x1c);
    uint32 animations = r_u32(resource + 0x10);
    uint32 state = object + 0x128;
    if (animations != 0)
        G3Function80029F0C(object, 0xffffffff, frame);
    if (r_u32(state + 0x14) != 0)
    {
        if (r_u32(state + 0x18) == 0)
        {
            for (uint32 index = 0; index < 5; ++index)
                G3LeafGeneratorEmit(object, state + 4, module_base, frame);
        }
        else
        {
            uint32 previous = r_u32(state + 0xc);
            uint32 current = r_u32(state + 4);
            bool moved = previous != current;
            if (!moved)
            {
                sint32 previous_z = r_s16(state + 0x10);
                sint32 current_z = r_s16(state + 8);
                moved = previous_z != current_z;
            }
            if (moved)
            {
                uint32 cooldown = r_u32(state + 0x1c) - 1;
                w_u32(state + 0x1c, cooldown);
                if ((sint32)cooldown < 0)
                {
                    G3LeafGeneratorEmit(object, object + 0x12c, module_base, frame);
                    uint32 random = G3SdkRandom80027FA8();
                    w_u32(state + 0x1c, (random & 15) + 1);
                }
            }
        }
    }
    uint32 source = state + 4;
    uint32 destination = state + 0xc;
    uint32 first_address = source;
    uint32 first_high_address = first_address + 3;
    uint32 first_high_lane = first_high_address & 3;
    uint32 first_lane = first_address & 3;
    uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
    uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
    uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
    uint32 first = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
    uint32 second_address = source + 4;
    uint32 second_high_address = second_address + 3;
    uint32 second_high_lane = second_high_address & 3;
    uint32 second_lane = second_address & 3;
    uint32 second_high = r_masked_u32(second_high_address & 0xfffffffc, (1 << (second_high_lane + 1)) - 1);
    uint32 second_low = r_masked_u32(second_address & 0xfffffffc, (15 << second_lane) & 15);
    uint32 second_keep = second_lane == 0 ? 0 : 0xffffffff << ((4 - second_lane) * 8);
    uint32 second = ((second_high << ((3 - second_high_lane) * 8)) & second_keep) | (second_low >> (second_lane * 8));
    uint32 first_output = destination;
    uint32 first_output_high = first_output + 3;
    uint32 first_output_lane = first_output & 3;
    uint32 first_output_high_lane = first_output_high & 3;
    w_masked_u32(first_output_high & 0xfffffffc, first >> ((3 - first_output_high_lane) * 8), (1 << (first_output_high_lane + 1)) - 1);
    w_masked_u32(first_output & 0xfffffffc, first << (first_output_lane * 8), (15 << first_output_lane) & 15);
    uint32 second_output = destination + 4;
    uint32 second_output_high = second_output + 3;
    uint32 second_output_lane = second_output & 3;
    uint32 second_output_high_lane = second_output_high & 3;
    w_masked_u32(second_output_high & 0xfffffffc, second >> ((3 - second_output_high_lane) * 8), (1 << (second_output_high_lane + 1)) - 1);
    w_masked_u32(second_output & 0xfffffffc, second << (second_output_lane * 8), (15 << second_output_lane) & 15);
    uint32 contacted = r_u32(state + 0x14);
    w_u32(state + 0x14, 0);
    w_u32(state + 0x18, contacted);
}

void G3LeafGeneratorCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000598, "G3L_74");
    uint32 frame = guest_stack - 0x38;
    uint32 contact = r_u32(object + 0xb0);
    uint32 player = r_u32(game + 0x50);
    uint32 other = r_u32(contact + 0x14);
    uint32 state = object + 0x128;
    if (other != player)
        return;
    sint32 kind = r_s16(contact + 6);
    if (kind == 4)
    {
        w_u16(state + 4, r_u16(contact + 0x18));
        w_u16(state + 6, r_u16(contact + 0x1a));
        uint32 z = r_u16(contact + 0x1c);
        w_u32(state + 0x14, 1);
        w_u16(state + 8, z);
        return;
    }
    if (kind != 1)
        return;
    sint32 primitive_kind = r_s16(contact + 4);
    if (primitive_kind == 2)
    {
        w_u32(state + 0x14, 1);
        return;
    }
    uint32 primitive;
    uint32 type;
    if (primitive_kind == 1)
    {
        primitive = r_u32(contact + 8);
        type = r_u8(primitive + 4);
    }
    else if (primitive_kind == 5)
    {
        primitive = r_u32(contact + 8);
        type = r_u8(primitive + 2);
    }
    else
        return;
    if (type - 7 >= 3)
        return;
    uint32 matrix = r_u32(0x800a25ac);
    uint32 first = r_u32(matrix);
    uint32 second = r_u32(matrix + 4);
    xport_gte_write_control(0, first);
    xport_gte_write_control(1, second);
    uint32 third = r_u32(matrix + 8);
    uint32 fourth = r_u32(matrix + 0xc);
    uint32 fifth = r_u32(matrix + 0x10);
    xport_gte_write_control(2, third);
    xport_gte_write_control(3, fourth);
    xport_gte_write_control(4, fifth);
    uint32 tx = r_u32(matrix + 0x14);
    uint32 ty = r_u32(matrix + 0x18);
    xport_gte_write_control(5, tx);
    uint32 tz = r_u32(matrix + 0x1c);
    xport_gte_write_control(6, ty);
    xport_gte_write_control(7, tz);
    // Uses corrected MAC output without a pad store
    RotTrans((SVECTOR *)psx_addr(0x800a25a4, 8), (VECTOR *)psx_addr(frame + 0x10, 12), (sint32 *)psx_addr(frame + 0x20, 4));
    w_u16(state + 4, r_u16(frame + 0x10));
    w_u16(state + 6, r_u16(frame + 0x14));
    uint32 z = r_u16(frame + 0x18);
    w_u32(state + 0x14, 1);
    w_u16(state + 8, z);
}

uint32 G3LennyBEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_78");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3LennyBInitialize(object, frame);
        return 1;
    case 2:
        G3LennyBUpdate(object, game, module_base, frame);
        return 1;
    case 3:
        G3LennyBCollide(object, game, frame);
        return 1;
    case 9:
        G3LennyBActivate(object);
        return 1;
    default:
        return 0;
    }
}

void G3LennyBConfigure(uint32 object, uint32 commands, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000094, "G3L_78");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    w_u32(frame + 0x34, commands);
    if (commands == 0)
        return;
    while (G3Function80085DFC(frame + 0x34, frame + 0x10, frame + 0x14, frame + 0x18) != 0)
    {
        sint32 kind = r_s16(frame + 0x10);
        switch (kind)
        {
        case 1:
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 value = r_u16(data);
            w_u16(state + 0x2a, value);
            value = r_u16(data + 2);
            w_u16(state + 0x2c, value);
            for (uint32 axis = 0; axis != 2; ++axis)
            {
                uint32 radius = (uint32)r_s16(data + 4 + axis * 2);
                w_u32(state + axis * 4, radius * radius);
            }
            break;
        }
        case 0x87:
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 value = r_u32(data);
            w_u32(state + 8, value);
            break;
        }
        case 0x88:
        case 0x31:
        case 0x55:
        {
            uint32 data = r_u32(frame + 0x14);
            sint32 scaled = (sint32)((uint32)r_s16(data) << 12);
            uint32 offset = kind == 0x88 ? 0x18 : kind == 0x31 ? 0x1c : 0x34;
            w_u16(state + offset, (uint32)(scaled / 3600));
            break;
        }
        case 0x5a:
        {
            uint32 source = r_u32(frame + 0x14);
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            uint32 last = (uint32)r_s16(source + 4);
            uint32 destination = state + 0x1e;
            high_address = destination + 3;
            high_lane = high_address & 3;
            lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
            w_u16(destination + 4, last);
            break;
        }
        case 0x52:
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 game = r_u32(0x80097048);
            uint32 resource = G3FindLevelResource800397B0(data, game);
            w_u32(state + 0xc, resource);
            break;
        }
        case 0x5b:
        case 0x5c:
        case 0x76:
        case 0x9d:
        case 0x89:
        case 0x8e:
        case 0xa0:
        case 0x69:
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 value = r_u16(data);
            uint32 offset = 0;
            switch (kind)
            {
            case 0x5b: offset = 0x26; break;
            case 0x5c: offset = 0x24; break;
            case 0x76: offset = 0x28; break;
            case 0x9d: offset = 0x32; break;
            case 0x89: offset = 0x32; value = 0u - value; break;
            case 0x8e: offset = 0x30; break;
            case 0xa0: offset = 0x36; break;
            case 0x69: offset = 0x14; break;
            }
            w_u16(state + offset, value);
            break;
        }
        }
    }
}
