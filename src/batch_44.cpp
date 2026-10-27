#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void G3GatorCollide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000016c8, "G3L_73");
    uint32 frame = guest_stack - 0x58;
    uint32 contact = r_u32(object + 0xb0);
    uint32 player = r_u32(0x8009efc4);
    sint32 mode = r_s16(state);
    uint32 other = r_u32(contact + 0x14);
    uint32 model = r_u32(player + 0x24);
    uint32 delta = contact + 0x28;
    if (mode == 0 && other != player) goto classify;
    if (mode == 0)
    {
        sint32 kind = r_s16(contact + 6);
        if (kind == 1)
        {
            uint32 descriptor = r_u32(contact + 0xc);
            if (r_u8(descriptor + 4) == 0 && r_u32(model + 0x1fc) == 0)
            {
                w_u16(state, r_u8(settings + 0xa) != 0 ? 1 : 2);
                w_u16(state + 4, r_u16(object + 0x64));
                uint32 animations = r_u32(settings);
                uint32 animation = r_u8(animations + 1);
                w_u16(object + 0x5e, 0);
                w_u16(object + 0x6e, animation);
                G3Function8006F50C(object, game, state, settings, frame);
                w_u8(model + 0x200, r_u8(settings + 0xb));
                return;
            }
        }
    }
    player = r_u32(0x8009efc4);
    if (other == player) return;
classify:
    G3Function8003C548(contact, frame + 0x28, frame + 0x29);
    if (r_s8(frame + 0x28) == 5)
    {
        uint32 second = r_u8(frame + 0x29);
        if ((second == 9 || second - (uint32)3 < 3) && other != 0)
        {
            uint32 descriptor = r_u32(other + 0x1c);
            uint32 name = r_u32(descriptor + 0x24);
            uint32 expected_first = r_u32(module_base + 0x1c8c);
            uint32 actual_first = r_u32(name);
            bool match = false;
            if (actual_first == expected_first)
            {
                uint32 actual_second = r_u32(name + 4);
                uint32 expected_second = r_u32(module_base + 0x1c90);
                match = actual_second == expected_second;
            }
            if (!match)
            {
                expected_first = r_u32(module_base + 0x1c98);
                if (actual_first == expected_first)
                {
                    uint32 actual_second = r_u32(name + 4);
                    uint32 expected_second = r_u32(module_base + 0x1c9c);
                    match = actual_second == expected_second;
                }
            }
            if (match)
            {
                if (r_s16(state + 2) != 4) return;
                w_u32(frame + 0x10, 1);
                w_u16(frame + 0x20, 0);
                w_u16(frame + 0x22, 0);
                w_u32(frame + 0x24, contact);
                uint32 target = r_u32(other + 0xec);
                if (target != 0) G3DispatchStateEvent(target, other, game, 8, frame + 0x10, frame);
                return;
            }
        }
        if (r_s8(frame + 0x28) == 5) goto check_projectile;
    }
    {
        sint32 kind = r_s16(contact + 6);
        uint32 descriptor = r_u32(contact + 0xc);
        bool separate = false;
        if (kind == 3 && (r_u16(descriptor + 6) & 0xf) != 8) separate = true;
        if (!separate && kind == 1 && (r_u32(descriptor) & 0x2000) != 0) separate = true;
        if (!separate && kind == 5 && (r_u16(descriptor) & 0x2000) != 0) separate = true;
        if (!separate && kind == 2 && (r_u16(descriptor + 6) & 0xf) != 8) separate = true;
        if (separate)
        {
            uint32 position = r_u16(object + 0x58);
            uint32 component = r_u16(delta);
            w_u16(object + 0x58, position + component);
            position = r_u16(object + 0x5a);
            component = r_u16(delta + 2);
            w_u16(object + 0x5a, position + component);
            position = r_u16(object + 0x5c);
            component = r_u16(delta + 4);
            w_u16(object + 0x5c, position + component);
            G3Function8005F3A8(object, delta);
            player = r_u32(0x8009efc4);
            component = r_u16(delta);
            position = r_u16(player + 0x58);
            w_u16(player + 0x58, position + component);
            position = r_u16(player + 0x5a);
            component = r_u16(delta + 2);
            w_u16(player + 0x5a, position + component);
            position = r_u16(player + 0x5c);
            component = r_u16(delta + 4);
            w_u16(player + 0x5c, position + component);
            G3Function8005F3A8(player, delta);
        }
    }
check_projectile:
    if (other != 0)
    {
        uint32 descriptor = r_u32(other + 0x1c);
        uint32 name = r_u32(descriptor + 0x24);
        uint32 expected_first = r_u32(module_base + 0x1c8c);
        uint32 actual_first = r_u32(name);
        bool match = false;
        if (actual_first == expected_first)
        {
            uint32 actual_second = r_u32(name + 4);
            uint32 expected_second = r_u32(module_base + 0x1c90);
            match = actual_second == expected_second;
        }
        if (!match)
        {
            expected_first = r_u32(module_base + 0x1ca4);
            if (actual_first == expected_first)
            {
                uint32 actual_second = r_u32(name + 4);
                uint32 expected_second = r_u32(module_base + 0x1ca8);
                match = actual_second == expected_second;
            }
        }
        if (!match || r_s8(frame + 0x28) == 5 || r_u8(state + 0x2f) != 0) return;
        uint32 parent = r_u32(other + 0x2c);
        if (parent != 0)
        {
            sint32 parent_x = r_s16(parent + 0x58);
            sint32 parent_y = r_s16(parent + 0x5a);
            sint32 other_y = r_s16(other + 0x5a);
            sint32 other_x = r_s16(other + 0x58);
            uint32 angle = G3Function8002F240((uint32)(parent_y - other_y), (uint32)(parent_x - other_x), frame);
            if ((G3SdkRandom80027FA8() & 1) != 0) angle += 0x400;
            else angle -= 0x400;
            angle = (uint32)(sint16)angle;
            uint32 product = G3Function8005B470(angle) * 10;
            w_u16(state + 0x32, (uint32)((sint32)product >> 12));
            product = G3Function8005B590(angle) * 10;
            w_u16(state + 0x34, (uint32)((sint32)product >> 12));
        }
        else if ((G3SdkRandom80027FA8() & 1) != 0)
        {
            sint32 y = r_s16(delta + 2);
            w_u16(state + 0x32, (uint32)((-y) >> 1));
            uint32 x = r_u16(delta);
            w_u16(state + 0x34, (uint32)((sint16)x >> 1));
        }
        else
        {
            uint32 y = r_u16(delta + 2);
            w_u16(state + 0x32, (uint32)((sint16)y >> 1));
            sint32 x = r_s16(delta);
            w_u16(state + 0x34, (uint32)((-x) >> 1));
        }
        G3Function80038798(other, 0, 0, frame);
        w_u16(state + 0xa, 0);
        w_u8(state + 0x2f, 1);
    }
}

void G3GatorReverseCollision(uint32 object, uint32 game, uint32 state, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001b7c, "G3L_73");
    uint32 frame = guest_stack - 0x38;
    uint32 contact = r_u32(object + 0xb0);
    G3Function8003C548(contact, frame + 0x18, frame + 0x19);
    if (r_s8(frame + 0x18) == 5)
    {
        uint32 x = 0 - (uint32)r_u16(payload);
        w_u16(frame + 0x10, x);
        w_u16(frame + 0x12, 0 - (uint32)r_u16(payload + 2));
        w_u16(frame + 0x14, 0 - (uint32)r_u16(payload + 4));
        w_u16(object + 0x58, r_u16(object + 0x58) + x);
        uint32 position = r_u16(object + 0x5a);
        uint32 component = r_u16(frame + 0x12);
        w_u16(object + 0x5a, position + component);
        position = r_u16(object + 0x5c);
        component = r_u16(frame + 0x14);
        w_u16(object + 0x5c, position + component);
        G3Function8005F3A8(object, frame + 0x10);
    }
    w_u16(state + 0xa, 0);
}

void G3GatorSplash800458F8(uint32 position, uint32 guest_stack)
{
    uint32 parameters = r_u32(0x80098784);
    uint32 frame = guest_stack - 0x18;
    G3Function80045804(position, parameters, frame);
}

uint32 G3GangCannonEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_76");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3GangCannonInitialize(object, module_base, frame);
        return 1;
    case 2:
        G3GangCannonUpdate(object, game, module_base, frame);
        return 1;
    case 3:
        G3GangCannonCollide(object, game, frame);
        return 1;
    case 7:
        G3GangCannonDisable(object, game, frame);
        return 1;
    default:
        return 0;
    }
}

void G3GangCannonConfigure(uint32 object, uint32 properties, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000a0, "G3L_76");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    w_u32(frame + 0x2c, properties);
    if (properties == 0)
        return;
    while (G3Function80085DFC(frame + 0x2c, frame + 0x10, frame + 0x14, frame + 0x18) != 0)
    {
        if (r_s16(frame + 0x10) == 1)
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 value = (uint32)r_s16(data);
            w_u32(state, value * value);
            value = (uint32)r_s16(data + 2);
            w_u32(state + 4, value * value);
            value = (uint32)r_s16(data + 4);
            w_u32(state + 8, value * value);
        }
    }
}

void G3GangCannonInitialize(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000013c, "G3L_76");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    w_u32(state, 0x3d090);
    w_u32(state + 4, 0x6f3c90);
    w_u32(state + 8, 0xd0e490);
    w_u32(state + 0xc, 0);
    uint32 resource = r_u32(object + 0x1c);
    G3GangCannonConfigure(object, r_u32(resource + 0x1c), frame);
    G3GangCannonConfigure(object, r_u32(object + 0x28), frame);
    uint32 intro = r_u32(object + 0x20);
    w_u32(object + 0x150, 0);
    w_u16(object + 0x6e, 0);
    w_u16(object + 0x5e, 0);
    uint32 table = r_u32(intro + 4);
    if (table != 0)
    {
        uint32 cursor = table + 4;
        sint32 count = r_s32(table);
        if (count > 0)
        {
            uint32 name = module_base + 0x830;
            uint32 first = r_u32(name);
            for (sint32 index = 0; index < count; ++index)
            {
                uint32 entry = r_u32(cursor);
                uint32 own = r_u32(object + 0x20);
                if (entry != own)
                {
                    uint32 descriptor = r_u32(entry);
                    uint32 entry_name = r_u32(descriptor + 0x20);
                    if (r_u32(entry_name) == first)
                    {
                        uint32 second = r_u32(entry_name + 4);
                        if (second == r_u32(name + 4))
                        {
                            w_u32(state + 0xc, entry);
                            break;
                        }
                    }
                }
                cursor += 4;
            }
        }
    }
}

void G3GangCannonEnable(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000250, "G3L_76");
    uint32 frame = guest_stack - 0x20;
    w_u16(frame + 0x10, 1);
    w_u16(frame + 0x12, 1);
    uint32 entry = r_u32(object + 0x110);
    if (entry != 0)
    {
        uint32 receiver = r_u32(entry + 0x20);
        if (receiver != 0)
        {
            uint32 target = r_u32(receiver + 0xec);
            if (target != 0)
                G3DispatchStateEvent(target, receiver, game, 9, frame + 0x10, frame);
        }
    }
}

void G3GangCannonDisable(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000002ac, "G3L_76");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    G3FillMemory80086094(frame + 0x10, 0, 4);
    uint32 entry = r_u32(state + 0xc);
    if (entry != 0)
    {
        uint32 receiver = r_u32(entry + 0x20);
        if (receiver != 0)
        {
            uint32 target = r_u32(receiver + 0xec);
            if (target != 0)
                G3DispatchStateEvent(target, receiver, game, 9, frame + 0x10, frame);
        }
    }
}

void G3GangCannonUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000324, "G3L_76");
    uint32 frame = guest_stack - 0x38;
    uint32 mode = r_u32(object + 0x150);
    uint32 player = r_u32(game + 0x50);
    uint32 state = object + 0x104;
    if (mode >= 7)
        return;
    uint32 target = r_u32(module_base + 0x840 + (mode << 2));
    bool face_player = false;
    switch (target - module_base)
    {
    case 0x378:
    {
        uint32 px = r_u16(player + 0x58);
        uint32 x = r_u16(object + 0x58);
        uint32 dx = px - x;
        uint32 sx = (uint32)(sint32)(sint16)dx;
        uint32 xx = sx * sx;
        w_u16(frame + 0x10, dx);
        uint32 py = r_u16(player + 0x5a);
        uint32 y = r_u16(object + 0x5a);
        uint32 dy = py - y;
        uint32 sy = (uint32)(sint32)(sint16)dy;
        uint32 yy = sy * sy;
        w_u16(frame + 0x12, dy);
        uint32 pz = r_u16(player + 0x5c);
        uint32 z = r_u16(object + 0x5c);
        uint32 dz = pz - z;
        uint32 sz = (uint32)(sint32)(sint16)dz;
        uint32 zz = sz * sz;
        w_u16(frame + 0x14, dz);
        sint32 threshold = r_s32(state + 4);
        uint32 distance = xx + yy + zz;
        if (threshold >= (sint32)distance)
        {
            w_u32(object + 0x150, 1);
            w_u16(object + 0x6e, 0);
            w_u16(object + 0x5e, 0);
            face_player = true;
        }
        break;
    }
    case 0x404:
    {
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 entry = r_u32(state + 0xc);
        if (entry != 0)
        {
            uint32 receiver = r_u32(entry + 0x20);
            if (receiver != 0)
            {
                uint32 callback = r_u32(receiver + 0xec);
                if (callback != 0)
                    G3DispatchStateEvent(callback, receiver, game, 1000, frame + 0x18, frame);
                uint32 rx = r_u16(frame + 0x18);
                uint32 x = r_u16(object + 0x58);
                uint32 ry = r_u16(frame + 0x1a);
                uint32 dx = rx - x;
                w_u16(frame + 0x10, dx);
                uint32 y = r_u16(object + 0x5a);
                uint32 dy = ry - y;
                w_u16(frame + 0x12, dy);
                uint32 rz = r_u16(frame + 0x1c);
                uint32 z = r_u16(object + 0x5c);
                uint32 dz = rz - z;
                w_u16(frame + 0x14, dz);
                uint32 angle = G3Function8002F240((uint32)(sint32)(sint16)dy, (uint32)(sint32)(sint16)dx, frame);
                w_u16(object + 0x64, angle + 0x400);
            }
        }
        uint32 px = r_u16(player + 0x58);
        uint32 x = r_u16(object + 0x58);
        uint32 dx = px - x;
        uint32 sx = (uint32)(sint32)(sint16)dx;
        uint32 xx = sx * sx;
        w_u16(frame + 0x10, dx);
        uint32 py = r_u16(player + 0x5a);
        uint32 y = r_u16(object + 0x5a);
        uint32 dy = py - y;
        uint32 sy = (uint32)(sint32)(sint16)dy;
        uint32 yy = sy * sy;
        w_u16(frame + 0x12, dy);
        uint32 pz = r_u16(player + 0x5c);
        uint32 z = r_u16(object + 0x5c);
        uint32 dz = pz - z;
        uint32 sz = (uint32)(sint32)(sint16)dz;
        uint32 zz = sz * sz;
        w_u16(frame + 0x14, dz);
        uint32 distance = xx + yy + zz;
        sint32 far_threshold = r_s32(state + 8);
        uint32 next;
        if ((sint32)distance >= far_threshold)
            next = 3;
        else
        {
            sint32 near_threshold = r_s32(state);
            if (near_threshold < (sint32)distance)
                break;
            next = 4;
        }
        w_u32(object + 0x150, next);
        w_u16(object + 0x6e, 2);
        w_u16(object + 0x5e, 0);
        G3GangCannonDisable(object, game, frame);
        break;
    }
    case 0x55c:
    {
        w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 resource = r_u32(object + 0x1c);
        uint32 selected = r_u16(object + 0x6e);
        uint32 animations = r_u32(resource + 0x10);
        uint32 animation = r_u32(animations + (selected << 2));
        sint32 last = r_s16(animation + 2) - 1;
        sint32 current = r_s16(object + 0x5e);
        if (current == last)
        {
            w_u32(object + 0x150, 0);
        }
        break;
    }
    case 0x5b0:
    {
        w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 resource = r_u32(object + 0x1c);
        uint32 selected = r_u16(object + 0x6e);
        uint32 animations = r_u32(resource + 0x10);
        uint32 animation = r_u32(animations + (selected << 2));
        sint32 last = r_s16(animation + 2) - 1;
        sint32 current = r_s16(object + 0x5e);
        if (current == last)
        {
            w_u32(object + 0x150, 5);
        }
        break;
    }
    case 0x6e8:
    {
        w_u32(object + 0x14, r_u32(object + 0x14) & 0xffffffef);
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 resource = r_u32(object + 0x1c);
        uint32 selected = r_u16(object + 0x6e);
        uint32 animations = r_u32(resource + 0x10);
        uint32 animation = r_u32(animations + (selected << 2));
        sint32 last = r_s16(animation + 2) - 1;
        sint32 current = r_s16(object + 0x5e);
        if (current == last)
        {
            w_u32(object + 0x150, 2);
            w_u16(object + 0x6e, 1);
            w_u16(object + 0x5e, 0);
            G3GangCannonEnable(object, game, frame);
        }
        break;
    }
    case 0x604:
    {
        uint32 px = r_u16(player + 0x58);
        uint32 x = r_u16(object + 0x58);
        uint32 dx = px - x;
        uint32 sx = (uint32)(sint32)(sint16)dx;
        uint32 xx = sx * sx;
        w_u16(frame + 0x10, dx);
        uint32 py = r_u16(player + 0x5a);
        uint32 y = r_u16(object + 0x5a);
        uint32 dy = py - y;
        uint32 sy = (uint32)(sint32)(sint16)dy;
        uint32 yy = sy * sy;
        w_u16(frame + 0x12, dy);
        uint32 pz = r_u16(player + 0x5c);
        uint32 z = r_u16(object + 0x5c);
        uint32 dz = pz - z;
        uint32 sz = (uint32)(sint32)(sint16)dz;
        uint32 zz = sz * sz;
        w_u16(frame + 0x14, dz);
        uint32 distance = xx + yy + zz;
        sint32 near_threshold = r_s32(state);
        if (near_threshold < (sint32)distance)
        {
            w_u32(object + 0x150, 6);
            w_u16(object + 0x6e, 0);
            w_u16(object + 0x5e, 0);
            face_player = true;
        }
        break;
    }
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved gngcan update target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
    if (face_player)
    {
        uint32 px = r_u16(player + 0x58);
        uint32 x = r_u16(object + 0x58);
        uint32 dx = px - x;
        w_u16(frame + 0x10, dx);
        uint32 py = r_u16(player + 0x5a);
        uint32 y = r_u16(object + 0x5a);
        uint32 dy = py - y;
        w_u16(frame + 0x12, dy);
        uint32 pz = r_u16(player + 0x5c);
        uint32 z = r_u16(object + 0x5c);
        uint32 dz = pz - z;
        w_u16(frame + 0x14, dz);
        uint32 angle = G3Function8002F240((uint32)(sint32)(sint16)dy, (uint32)(sint32)(sint16)dx, frame);
        w_u16(object + 0x64, angle + 0x400);
    }
}

void G3GangCannonCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000076c, "G3L_76");
    uint32 frame = guest_stack - 0x28;
    uint32 contact = r_u32(object + 0xb0);
    G3Function8003C548(contact, frame + 0x10, frame + 0x11);
    uint32 other = r_u32(contact + 0x14);
    uint32 player = r_u32(0x8009efc4);
    if (other != player)
        return;
    uint32 second = r_u8(frame + 0x11);
    if (second - 6 >= 4)
        return;
    uint32 first = r_u8(frame + 0x10);
    if (first != 9 && first - 3 >= 3)
        return;
    uint32 mode = r_u32(object + 0x150);
    if (mode == 0 || mode == 5)
        return;
    G3GangCannonDisable(object, game, frame);
    G3Function80038798(object, 0, 0, frame);
}

uint32 G3GuyEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_29");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1: G3GuyInitialize(object, module_base, frame); return 1;
    case 2: G3GuyUpdate(object, game, module_base, frame); return 1;
    case 3: G3GuyCollide(object, game, frame); return 1;
    case 8: G3GuyDamage(object, module_base, frame); return 1;
    default: return 0;
    }
}

void G3GuyInitialize(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000a0, "G3L_29");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    uint32 commands = r_u32(object + 0x28);
    w_u16(state + 0xc, 0);
    w_u16(state + 0xe, 0);
    w_u16(state + 0x10, 0xffff);
    w_u16(state + 0x12, 0xffff);
    w_u16(state + 0x14, 0xffff);
    w_u16(state + 0x16, 0);
    w_u16(state + 0x18, commands != 0 ? r_u16(commands) : 0);
    w_u16(state + 0x1a, commands != 0 ? r_u16(commands + 2) : 0);
    w_u16(state + 0x1c, commands != 0 ? r_u16(commands + 4) : 3);
    w_u16(state + 0x1e, commands != 0 ? r_u16(commands + 6) : 0);
    w_u16(state, commands != 0 ? r_u16(commands + 8) : r_u16(object + 0x58) - 1600);
    w_u16(state + 6, commands != 0 ? r_u16(commands + 0xe) : r_u16(object + 0x58) + 1600);
    w_u16(state + 2, commands != 0 ? r_u16(commands + 0xa) : r_u16(object + 0x5a) - 1600);
    w_u16(state + 8, commands != 0 ? r_u16(commands + 0x10) : r_u16(object + 0x5a) + 1600);
    w_u16(state + 4, commands != 0 ? r_u16(commands + 0xc) : r_u16(object + 0x5c));
    w_u16(state + 0xa, commands != 0 ? r_u16(commands + 0x12) : r_u16(object + 0x5c) + 800);
    w_u32(state + 0x20, 0);
    w_u32(state + 0x24, 0);
    uint32 attachment = r_u32(object + 0x20);
    uint32 index = attachment != 0 && r_u32(attachment + 4) != 0 ? G3AttachmentIndex80037028(object) : 0xffffffff;
    w_u32(state + 0x28, index);
    if (commands != 0)
    {
        sint32 health = r_s16(state + 0x1c);
        uint32 bits = r_u16(state + 0x1c);
        w_u16(state + 0x1c, health <= 0 ? 1 : bits);
    }
    if (r_u32(object + 0x2c) != 0)
    {
        uint32 resource = G3Function8003AA48(module_base + 0x19a0);
        uint32 descriptor = r_u32(object + 0x1c);
        if (resource == descriptor)
        {
            uint32 parent = r_u32(object + 0x2c);
            uint32 first = r_u32(parent + 0x104);
            uint32 second = r_u32(parent + 0x108);
            uint32 third = r_u32(parent + 0x10c);
            w_u32(state, first);
            w_u32(state + 4, second);
            w_u32(state + 8, third);
            w_u16(state + 0x18, 2);
            w_u16(state + 0x1c, 1);
            w_u16(state + 0x1e, 0);
            G3GuySetMode(object, 2, 0, module_base, frame);
        }
        else
        {
            w_u16(state + 0x18, 1);
            w_u16(state + 0x1c, 1);
            w_u32(state + 0x20, resource);
            G3GuySetMode(object, 0, 0, module_base, frame);
            uint32 flags = r_u32(object + 0x10);
            uint32 z = r_u16(object + 0x5c);
            w_u32(object + 0x10, flags | 0x400);
            w_u16(object + 0x74, z);
        }
    }
    else
    {
        w_u32(state + 0x20, G3Function8003AA48(module_base + 0x19ac));
        G3GuySetMode(object, 0, 1, module_base, frame);
    }
}

uint32 G3GuyTable(uint32 variant, uint32 mode, uint32 phase, uint32 module_base)
{
    FUNCTION_MARKER(0x00000344, "G3L_29");
    return module_base + 0x19cc + (((variant * 5 + mode) * 4 + phase) * 8);
}

void G3GuySetMode(uint32 object, uint32 mode, uint32 force, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000036c, "G3L_29");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    uint32 entering = G3GuyTable((uint32)r_s16(state + 0x18), mode, 2, module_base);
    uint32 row = G3GuyTable((uint32)r_s16(state + 0x18), mode, 0, module_base);
    if (r_s16(entering) >= 0 && force == 0) row = entering;
    uint32 phase = r_s16(entering) >= 0 && force == 0 ? 2 : 0;
    w_u16(state + 0x16, phase);
    uint32 previous = r_u16(state + 0x14);
    uint32 flags = r_u16(state + 0xc);
    w_u16(state + 0x14, 0xffff);
    w_u16(state + 0x10, 0xffff);
    w_u16(state + 0x12, previous);
    if ((flags & 1) != 0 && r_s16(state + 0x16) == 0)
    {
        uint32 variant = (uint32)r_s16(state + 0x18);
        w_u16(state + 0xc, flags & 0xfffe);
        row = G3GuyTable(variant, mode, 1, module_base);
        if (r_s16(row) >= 0) w_u16(state + 0x16, 1);
        else row = G3GuyTable((uint32)r_s16(state + 0x18), mode, 0, module_base);
    }
    w_u32(object + 0x150, mode);
    w_u16(object + 0x6e, r_u16(row));
    w_u16(object + 0x5e, r_u16(row + 2));
    (void)frame;
}

void G3GuyDestroy(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000004a0, "G3L_29");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    if (r_s16(state + 0x18) == 0)
    {
        sint32 index = r_s32(state + 0x28);
        if (index >= 0)
        {
            uint32 sum = (uint32)index + 1;
            uint32 adjusted = sum + (sum >> 31);
            uint32 remainder = sum - ((uint32)((sint32)adjusted >> 1) << 1);
            uint32 other = G3AttachedObject80036FE4(object, (sint32)remainder);
            uint32 target = r_u32(other + 0xec);
            if (target != 0)
            {
                uint32 game = r_u32(0x80097048);
                G3DispatchStateEvent(target, other, game, 8, 0, frame);
            }
        }
    }
    w_u32(object + 0x10, r_u32(object + 0x10) & 0xfffffbff);
    uint32 selector = (uint32)r_s16(state + 0x1e);
    G3Function80038798(object, selector, 0, frame);
    uint32 attachment = r_u32(object + 0x20);
    if (attachment != 0) w_u32(attachment + 0x18, r_u32(attachment + 0x18) | 0x10);
}

void G3GuyAnimate(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000568, "G3L_29");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    uint32 variant = (uint32)r_s16(state + 0x18);
    uint32 mode = r_u32(object + 0x150);
    uint32 phase = (uint32)r_s16(state + 0x16);
    uint32 row = G3GuyTable(variant, mode, phase, module_base);
    sint32 limit = r_s16(row + 4);
    bool changed = false;
    bool finished;
    if (limit <= 0)
    {
        uint32 descriptor = r_u32(object + 0x1c);
        uint32 index = (uint32)r_s16(row);
        uint32 table = r_u32(descriptor + 0x10);
        uint32 animation = r_u32(table + index * 4);
        limit = r_s16(animation + 2) - 1;
    }
    uint32 flags = r_u16(row + 6);
    uint32 bits = r_u16(object + 0x5e);
    if ((flags & 1) != 0)
    {
        --bits;
        finished = (bits & 0xffff) == 0;
    }
    else
    {
        ++bits;
        finished = (sint16)bits >= limit;
    }
    w_u16(object + 0x5e, bits);
    uint32 descriptor = r_u32(object + 0x1c);
    if ((r_u32(descriptor + 0x30) & 0x2000) != 0) G3Function8003E314(object, frame);
    if (finished && (r_u16(state + 0xc) & 2) != 0)
    {
        G3GuyDestroy(object, frame);
        return;
    }
    sint32 remaining = r_s16(state + 0x10);
    uint32 remaining_bits = r_u16(state + 0x10);
    if (remaining > 0 && finished && r_s16(state + 0x16) == 0) w_u16(state + 0x10, remaining_bits - 1);
    phase = (uint32)r_s16(state + 0x16);
    switch (phase)
    {
    case 0:
        if (r_s16(state + 0x12) >= 0 && r_s16(state + 0x10) <= 0)
        {
            if ((r_u16(row + 6) & 2) != 0 && !finished) goto finish;
            variant = (uint32)r_s16(state + 0x18);
            mode = r_u32(object + 0x150);
            row = G3GuyTable(variant, mode, 3, module_base);
            if (r_s16(row) < 0) goto change_state;
            w_u16(state + 0x16, 3);
            goto apply_row;
        }
        if (!finished) goto finish;
        flags = r_u16(state + 0xc);
        if ((flags & 1) == 0) goto apply_row;
        variant = (uint32)r_s16(state + 0x18);
        w_u16(state + 0xc, flags & 0xfffe);
        mode = r_u32(object + 0x150);
        row = G3GuyTable(variant, mode, 1, module_base);
        if (r_s16(row) >= 0) w_u16(state + 0x16, 1);
        else
        {
            variant = (uint32)r_s16(state + 0x18);
            mode = r_u32(object + 0x150);
            row = G3GuyTable(variant, mode, 0, module_base);
        }
        goto apply_row;
    case 1:
    case 2:
        if (!finished) goto finish;
        flags = r_u16(state + 0xc);
        w_u16(state + 0x16, 0);
        if ((flags & 1) != 0)
        {
            variant = (uint32)r_s16(state + 0x18);
            w_u16(state + 0xc, flags & 0xfffe);
            mode = r_u32(object + 0x150);
            uint32 candidate = G3GuyTable(variant, mode, 1, module_base);
            if (r_s16(candidate) >= 0) w_u16(state + 0x16, 1);
        }
        variant = (uint32)r_s16(state + 0x18);
        mode = r_u32(object + 0x150);
        phase = (uint32)r_s16(state + 0x16);
        row = G3GuyTable(variant, mode, phase, module_base);
        goto apply_row;
    case 3:
        if (finished) goto change_state;
        goto finish;
    default: goto finish;
    }
apply_row:
    w_u16(object + 0x6e, r_u16(row));
    w_u16(object + 0x5e, r_u16(row + 2));
    goto finish;
change_state:
    changed = true;
    G3GuySetMode(object, (uint32)r_s16(state + 0x12), 0, module_base, frame);
finish:
    if (changed && r_s16(state + 0x18) == 1 && r_u32(object + 0x150) == 0)
    {
        row = G3GuyTable(1, 0, 0, module_base);
        w_u16(object + 0x6e, r_u16(row));
        w_u16(object + 0x5e, r_u16(row + 2));
        w_u16(state + 0x16, 0);
    }
}

void G3GuyIdle(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000008b0, "G3L_29");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    if (r_s16(state + 0x16) == 2)
    {
        uint32 attachment = r_u32(object + 0x20);
        G3GuyTurn(object, (uint32)r_s16(attachment + 0xc), 128);
    }
    uint32 player = r_u32(0x8009efc4);
    if (G3GuyContains(object, player + 0x58) != 0)
    {
        if (r_s16(state + 0x1a) != 0) w_u16(state + 0x12, 2);
        else
        {
            w_u16(state + 0x12, 1);
            w_u16(state + 0x14, 2);
        }
    }
    (void)frame;
}

void G3GuyApproach(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000944, "G3L_29");
    uint32 frame = guest_stack - 0x38;
    uint32 state = object + 0x104;
    uint32 attachment = r_u32(object + 0x20);
    {
        uint32 source = attachment + 0x10;
        uint32 destination = frame + 0x18;
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
    if (r_s16(state + 0x12) == 2)
    {
        uint32 player = r_u32(0x8009efc4);
        {
        uint32 source = player + 0x58;
        uint32 destination = frame + 0x18;
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
    sint32 next = r_s16(state + 0x12);
    if (next == 0)
    {
        if (r_s16(state + 0x16) == 3)
        {
            attachment = r_u32(object + 0x20);
            G3GuyTurn(object, (uint32)r_s16(attachment + 0xc), 96);
        }
    }
    else
    {
        w_u16(frame + 0x20, 24);
        w_u16(frame + 0x22, 24);
        w_u16(frame + 0x24, 24);
        w_u32(frame + 0x10, 256);
        G3Function8003639C(object, 128, frame + 0x20, frame + 0x18, 256, frame);
        if (r_s16(state + 0x12) == -1 && G3GuyNear(object, frame + 0x18, 256) != 0) w_u16(state + 0x12, 0);
    }
}

void G3GuySpawnProjectile(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000a58, "G3L_29");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    sint32 heading = r_s16(object + 0x64);
    sint32 sum = heading + 0xc00;
    sint32 adjusted = sum >= 0 ? sum : heading + 0x1bff;
    uint32 wrapped = (uint32)sum - ((uint32)(adjusted >> 12) << 12);
    uint32 resource = r_u32(state + 0x20);
    if (resource == 0) return;
    uint32 shot = G3Function8002A454(object, resource, frame);
    if (shot == 0) return;
    uint32 angle = (uint32)(sint16)wrapped;
    w_u32(shot + 0x20, 0);
    w_u16(shot + 0x68, 0x1000);
    w_u16(shot + 0x6a, 0x1000);
    w_u16(shot + 0x6c, 0x1000);
    sint32 product = (sint32)(G3Function8005B470(angle) * 900);
    sint32 offset = product / 4096;
    w_u16(shot + 0x58, r_u16(shot + 0x58) + (uint32)offset);
    product = (sint32)(G3Function8005B590(angle) * 900);
    offset = product / 4096;
    uint32 y = r_u16(shot + 0x5a);
    uint32 z = r_u16(shot + 0x5c);
    w_u16(shot + 0x5a, y + (uint32)offset);
    w_u16(shot + 0x5c, z + 200);
    product = (sint32)(G3Function8005B470(angle) * 80);
    w_u32(shot + 0x104, (uint32)(product / 4096));
    product = (sint32)(G3Function8005B590(angle) * 80);
    w_u32(shot + 0x108, (uint32)(product / 4096));
    w_u32(shot + 0x10c, 5);
    product = (sint32)(G3Function8005B470(angle) * 5);
    w_u32(shot + 0x110, (uint32)(product / 4096));
    product = (sint32)(G3Function8005B590(angle) * 5);
    w_u32(shot + 0x114, (uint32)(product / 4096));
    w_u32(shot + 0x118, 0);
}

void G3GuyRanged(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000be0, "G3L_29");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    uint32 player = r_u32(0x8009efc4);
    uint32 angle = G3GuyAngleTo(object, player + 0x58, frame);
    G3GuyTurn(object, (uint32)(sint16)angle, 64);
    if (r_s16(state + 0x16) == 0 && r_s16(object + 0x5e) == 8) G3GuySpawnProjectile(object, frame);
    player = r_u32(0x8009efc4);
    if (G3GuyContains(object, player + 0x58) == 0) w_u16(state + 0x12, r_s16(state + 0x1a) != 0 ? 0 : 1);
    else
    {
        player = r_u32(0x8009efc4);
        w_u16(state + 0x12, (sint32)G3GuyDistance(object, player + 0x58) <= 0xc5c10 ? 3 : 2);
    }
}

void G3GuyMelee(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000cbc, "G3L_29");
    uint32 frame = guest_stack - 0x20;
    uint32 player = r_u32(0x8009efc4);
    uint32 angle = G3GuyAngleTo(object, player + 0x58, frame);
    G3GuyTurn(object, (uint32)(sint16)angle, 128);
    player = r_u32(0x8009efc4);
    w_u16(object + 0x116, (sint32)G3GuyDistance(object, player + 0x58) <= 0xc5c10 ? 3 : 2);
}

void G3GuyPlayerContact(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000d40, "G3L_29");
    uint32 frame = guest_stack - 0x28;
    uint32 mode = r_u32(object + 0x150);
    uint32 contact = r_u32(object + 0xb0);
    if (mode != 3) return;
    G3Function8003C548(contact, frame + 0x10, frame + 0x11);
    uint32 first = r_u8(frame + 0x10);
    if (first - 6 >= 4) return;
    uint32 second = r_u8(frame + 0x11);
    if (second == 9 || second - 3 < 3)
    {
        uint32 other = r_u32(contact + 0x14);
        uint32 player = r_u32(0x8009efc4);
        if (other == player) G3Function80074044(object, game, frame);
    }
}

void G3GuyHurt(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000de8, "G3L_29");
    uint32 frame = guest_stack - 0x18;
    uint32 state = object + 0x104;
    uint32 mode = r_u32(object + 0x150);
    uint32 commands = r_u32(object + 0x28);
    if (mode == 4) return;
    w_u16(state + 0x1c, r_u16(state + 0x1c) - 1);
    if (commands != 0) w_u16(commands + 4, r_u16(commands + 4) - 1);
    if (r_s16(state + 0x1c) <= 0)
    {
        w_u32(object + 0x10, r_u32(object + 0x10) | 0x400);
        w_u16(state + 0xc, r_u16(state + 0xc) | 3);
        if (commands != 0) w_u16(commands + 4, 0);
    }
    w_u16(state + 0x12, 4);
    w_u16(state + 0x14, 2);
    G3GuySetMode(object, (uint32)r_s16(state + 0x12), 0, module_base, frame);
}

void G3GuyChase(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000e8c, "G3L_29");
    uint32 frame = guest_stack - 0x20;
    uint32 player = r_u32(0x8009efc4);
    sint32 angle = (sint16)G3GuyAngleTo(object, player + 0x58, frame);
    G3GuyTurn(object, (uint32)angle, 128);
    sint32 sum = angle + 0xc00;
    if (r_s16(object + 0x11a) == 0)
    {
        sint32 adjusted = sum >= 0 ? sum : angle + 0x1bff;
        angle = (sint16)((uint32)sum - ((uint32)(adjusted >> 12) << 12));
        sint32 product = (sint32)(G3Function8005B470((uint32)angle) * 70);
        w_u16(object + 0x58, r_u16(object + 0x58) + (uint32)(product / 4096));
        product = (sint32)(G3Function8005B590((uint32)angle) * 70);
        w_u16(object + 0x5a, r_u16(object + 0x5a) + (uint32)(product / 4096));
    }
}

void G3GuyEndTornado(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000f70, "G3L_29");
    uint32 frame = guest_stack - 0x20;
    uint32 row = G3GuyTable(2, 2, 3, module_base);
    w_u16(object + 0x6e, r_u16(row));
    w_u16(object + 0x5e, r_u16(row + 2));
    w_u16(object + 0x11a, 3);
    w_u32(object + 0x10, r_u32(object + 0x10) | 0x1400);
    w_u16(object + 0x110, r_u16(object + 0x110) | 2);
    (void)frame;
}

void G3GuyTornadoContact(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000fec, "G3L_29");
    uint32 frame = guest_stack - 0x30;
    sint32 phase = r_s16(object + 0x11a);
    uint32 contact = r_u32(object + 0xb0);
    if (phase != 0) return;
    uint32 player = r_u32(0x8009efc4);
    sint32 object_x = r_s16(object + 0x58);
    sint32 player_x = r_s16(player + 0x58);
    sint32 x_offset = (player_x - object_x) / 4;
    uint32 velocity_x = r_u32(player + 0x104);
    sint32 player_y = r_s16(player + 0x5a);
    w_u32(player + 0x104, velocity_x + (uint32)x_offset);
    sint32 object_y = r_s16(object + 0x5a);
    uint32 velocity_y = r_u32(player + 0x108);
    w_u32(player + 0x108, velocity_y + (uint32)((player_y - object_y) / 4));
    w_u32(frame + 0x10, 1);
    w_u16(frame + 0x20, 0);
    w_u16(frame + 0x22, 0);
    w_u32(frame + 0x24, contact);
    uint32 other = r_u32(contact + 0x14);
    uint32 target = r_u32(other + 0xec);
    if (target != 0) G3DispatchStateEvent(target, other, game, 8, frame + 0x10, frame);
}

void G3GuyWanderTornado(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000010a8, "G3L_29");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    sint32 timer = r_s16(state + 0xe);
    uint32 timer_bits = r_u16(state + 0xe);
    if (timer != 0) w_u16(state + 0xe, timer_bits - 1);
    uint32 mode = r_u32(object + 0x150);
    if (mode == 0 && r_s16(state + 0x16) == 2) return;
    if (r_s16(state + 0xe) == 0)
    {
        uint32 player = r_u32(0x8009efc4);
        if ((sint32)G3GuyDistance(object, player + 0x58) <= 0x77a0ff)
        {
            G3GuySetMode(object, 2, 0, module_base, frame);
            return;
        }
    }
    w_u16(state + 0x12, r_u32(object + 0x150) == 0 ? 1 : 0);
    if (r_u16(object + 0x5e) - 12u >= 9) return;
    uint32 player = r_u32(0x8009efc4);
    sint32 angle = (sint16)G3GuyAngleTo(object, player + 0x58, frame);
    sint32 sum = angle + 0xc00;
    sint32 adjusted = sum >= 0 ? sum : angle + 0x1bff;
    uint32 wrapped = (uint32)sum - ((uint32)(adjusted >> 12) << 12);
    G3GuyTurn(object, (uint32)(sint16)wrapped, 128);
    if (G3GuyNear(object, object + 0x88, 128) != 0) return;
    angle = (sint16)G3GuyAngleTo(object, object + 0x88, frame);
    sum = angle + 0xc00;
    adjusted = sum >= 0 ? sum : angle + 0x1bff;
    wrapped = (uint32)sum - ((uint32)(adjusted >> 12) << 12);
    angle = (sint16)wrapped;
    sint32 product = (sint32)(G3Function8005B470((uint32)angle) * 80);
    w_u16(object + 0x58, r_u16(object + 0x58) + (uint32)(product / 4096));
    product = (sint32)(G3Function8005B590((uint32)angle) * 80);
    w_u16(object + 0x5a, r_u16(object + 0x5a) + (uint32)(product / 4096));
}

void G3GuyActiveTornado(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000126c, "G3L_29");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    if (r_s16(state + 0x16) == 3) w_u32(object + 0x10, r_u32(object + 0x10) & 0xffffe7ff);
    uint32 effect;
    if (r_s16(state + 0x16) == 0)
    {
        w_u32(object + 0x10, r_u32(object + 0x10) | 0x1800);
        effect = r_u32(state + 0x24);
        if (effect != 0) goto follow_effect;
        effect = G3Function8002A454(object, r_u32(state + 0x20), frame);
        w_u32(state + 0x24, effect);
        w_u16(state + 0xe, 90);
    }
    effect = r_u32(state + 0x24);
    if (effect == 0) return;
follow_effect:
    {
        uint32 source = effect + 0x58;
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
    uint32 player = r_u32(0x8009efc4);
    uint32 timer = r_u16(state + 0xe);
    w_u16(state + 0xe, timer - 1);
    uint32 distance = G3GuyDistance(object, player + 0x58);
    if ((sint32)distance > 0x77a0ff || r_s16(state + 0xe) <= 0)
    {
        G3GuyEndTornado(r_u32(state + 0x24), module_base, frame);
        w_u16(state + 0x12, 0);
        w_u32(state + 0x24, 0);
        w_u16(state + 0xe, 120);
    }
}

void G3GuyHurtTornado(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000138c, "G3L_29");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    uint32 mode = r_u32(object + 0x150);
    uint32 commands = r_u32(object + 0x28);
    if (mode == 4 || mode == 2) return;
    w_u16(state + 0x1c, r_u16(state + 0x1c) - 1);
    if (commands != 0) w_u16(commands + 4, r_u16(commands + 4) - 1);
    if (r_s16(state + 0x1c) <= 0)
    {
        uint32 flags = r_u32(object + 0x10);
        uint32 parent = r_u32(object + 0x2c);
        w_u32(object + 0x10, flags | 0x400);
        if (parent != 0)
        {
            uint32 attachment = r_u32(parent + 0x20);
            if (attachment != 0)
            {
                w_u32(attachment + 0x18, r_u32(attachment + 0x18) & 0xfffffbff);
                parent = r_u32(object + 0x2c);
                w_u32(parent + 0x10, r_u32(parent + 0x10) & 0xfffffbff);
                parent = r_u32(object + 0x2c);
                G3Function80038798(parent, 0, 0, frame);
            }
        }
        if (commands != 0) w_u16(commands + 4, 0);
    }
    w_u16(state + 0x12, 4);
    G3GuySetMode(object, 4, 0, module_base, frame);
}

void G3GuyShrink(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001488, "G3L_29");
    uint32 frame = guest_stack - 0x18;
    sint32 scale = r_s16(object + 0x6c);
    uint32 bits = r_u16(object + 0x6c);
    if (scale >= 201) w_u16(object + 0x6c, bits - 200);
    else
    {
        w_u32(object + 0x10, r_u32(object + 0x10) & 0xfffffbff);
        G3Function80038798(object, (uint32)r_s16(object + 0x122), 0, frame);
    }
}

void G3GuyUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000014dc, "G3L_29");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    G3GuyAnimate(object, module_base, frame);
    switch (r_u32(object + 0x150))
    {
    case 0:
        if (r_s16(state + 0x18) == 0) G3GuyIdle(object, frame);
        if (r_s16(state + 0x18) == 1) G3GuyWanderTornado(object, module_base, frame);
        break;
    case 1:
        if (r_s16(state + 0x18) == 0) G3GuyApproach(object, frame);
        if (r_s16(state + 0x18) == 1) G3GuyWanderTornado(object, module_base, frame);
        break;
    case 2:
    {
        if (r_s16(state + 0x18) == 0) G3GuyRanged(object, frame);
        sint32 variant = r_s16(state + 0x18);
        if (variant == 1)
        {
            G3GuyActiveTornado(object, module_base, frame);
            variant = r_s16(state + 0x18);
        }
        if (variant == 2) G3GuyChase(object, frame);
        break;
    }
    case 3: G3GuyMelee(object, frame); break;
    case 4:
        if (r_s16(state + 0x18) == 1) G3GuyShrink(object, frame);
        break;
    }
    G3GuyClampBounds(object);
}

uint32 G3GuyContains(uint32 object, uint32 point)
{
    FUNCTION_MARKER(0x00001628, "G3L_29");
    uint32 state = object + 0x104;
    sint32 x = r_s16(point);
    sint32 minimum = r_s16(state);
    if (minimum >= x) return 0;
    sint32 maximum = r_s16(state + 6);
    if (x >= maximum) return 0;
    sint32 y = r_s16(point + 2);
    minimum = r_s16(state + 2);
    if (minimum >= y) return 0;
    maximum = r_s16(state + 8);
    return y < maximum ? 1 : 0;
}

void G3GuyClampBounds(uint32 object)
{
    FUNCTION_MARKER(0x00001684, "G3L_29");
    uint32 state = object + 0x104;
    sint32 position = r_s16(object + 0x58);
    sint32 boundary = r_s16(state);
    uint32 bits = r_u16(state);
    if (position < boundary) w_u16(object + 0x58, bits);
    position = r_s16(object + 0x58);
    boundary = r_s16(state + 6);
    bits = r_u16(state + 6);
    if (boundary < position) w_u16(object + 0x58, bits);
    position = r_s16(object + 0x5a);
    boundary = r_s16(state + 2);
    bits = r_u16(state + 2);
    if (position < boundary) w_u16(object + 0x5a, bits);
    position = r_s16(object + 0x5a);
    boundary = r_s16(state + 8);
    bits = r_u16(state + 8);
    if (boundary < position) w_u16(object + 0x5a, bits);
    if (r_s16(state + 0x18) == 1) w_u16(object + 0x5c, r_u16(object + 0x74));
}

uint32 G3GuyNear(uint32 object, uint32 point, uint32 radius)
{
    FUNCTION_MARKER(0x00001728, "G3L_29");
    uint32 x = (uint32)r_s16(point);
    sint32 object_x = r_s16(object + 0x58);
    if ((sint32)(x - radius) >= object_x) return 0;
    if (object_x >= (sint32)(x + radius)) return 0;
    uint32 y = (uint32)r_s16(point + 2);
    sint32 object_y = r_s16(object + 0x5a);
    if ((sint32)(y - radius) >= object_y) return 0;
    return object_y < (sint32)(y + radius) ? 1 : 0;
}

void G3GuyTurn(uint32 object, uint32 target, uint32 step)
{
    FUNCTION_MARKER(0x00001774, "G3L_29");
    sint32 current = r_s16(object + 0x64);
    sint32 destination = (sint16)target;
    if (destination < current && current - destination > 2048) target += 0x1000;
    else
    {
        current = r_s16(object + 0x64);
        destination = (sint16)target;
        if (current < destination && destination - current > 2048) target -= 0x1000;
    }
    sint32 signed_step = (sint16)step;
    destination = (sint16)target;
    current = r_s16(object + 0x64);
    uint32 current_bits = r_u16(object + 0x64);
    if (current + signed_step < destination) w_u16(object + 0x64, current_bits + step);
    else if (destination < current - signed_step) w_u16(object + 0x64, current_bits - step);
    else w_u16(object + 0x64, target);
    current = r_s16(object + 0x64);
    sint32 sum = current + 0x1000;
    sint32 adjusted = sum >= 0 ? sum : current + 0x1fff;
    w_u16(object + 0x64, (uint32)sum - ((uint32)(adjusted >> 12) << 12));
}

uint32 G3GuyAngleTo(uint32 object, uint32 point, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001844, "G3L_29");
    uint32 frame = guest_stack - 0x18;
    sint32 object_x = r_s16(object + 0x58);
    sint32 object_y = r_s16(object + 0x5a);
    sint32 point_y = r_s16(point + 2);
    sint32 point_x = r_s16(point);
    uint32 angle = G3Function8002F240((uint32)(object_y - point_y), (uint32)(object_x - point_x), frame);
    return (uint32)(sint16)(angle + 0xc00);
}

uint32 G3GuyDistance(uint32 object, uint32 point)
{
    FUNCTION_MARKER(0x00001884, "G3L_29");
    sint32 point_x = r_s16(point);
    sint32 object_x = r_s16(object + 0x58);
    uint32 x = (uint32)(point_x - object_x);
    uint32 square_x = x * x;
    sint32 point_y = r_s16(point + 2);
    sint32 object_y = r_s16(object + 0x5a);
    uint32 y = (uint32)(point_y - object_y);
    return square_x + y * y;
}

void G3GuyDamage(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000018bc, "G3L_29");
    uint32 frame = guest_stack - 0x20;
    if (r_s16(object + 0x11c) == 0) G3GuyHurt(object, module_base, frame);
    if (r_s16(object + 0x11c) == 1) G3GuyHurtTornado(object, module_base, frame);
}

void G3GuyCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001920, "G3L_29");
    uint32 frame = guest_stack - 0x20;
    uint32 contact = r_u32(object + 0xb0);
    uint32 other = r_u32(contact + 0x14);
    uint32 player = r_u32(0x8009efc4);
    if (other != player) return;
    if (r_s16(object + 0x11c) == 0) G3GuyPlayerContact(object, game, frame);
    if (r_s16(object + 0x11c) == 2) G3GuyTornadoContact(object, game, frame);
}

uint32 G3GxSpitEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_44");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3GxSpitInitialize(object);
        return 1;
    case 2:
        G3GxSpitUpdate(object, frame);
        return 1;
    case 3:
    {
        uint32 resource = r_u32(object + 0x1c);
        if (resource == r_u32(0x800987ec))
            G3GxSpitIceCollide(object, game, frame);
        else if (resource == r_u32(0x800987fc))
            G3GxSpitFireCollide(object, game, frame);
        else
            G3GxSpitDefaultCollide(object, game, frame);
        return 1;
    }
    default:
        return 0;
    }
}

void G3GxSpitInitialize(uint32 object)
{
    FUNCTION_MARKER(0x000000d0, "G3L_44");
    w_u32(object + 0x12c, 0);
}

void G3GxSpitTrail(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000d8, "G3L_44");
    uint32 resource = r_u32(0x80098724);
    uint32 frame = guest_stack - 0x50;
    if (resource == 0)
        return;
    uint32 models = r_u32(resource + 0xc);
    uint32 pool = r_u32(0x800a27fc);
    uint32 model = r_u32(models);
    uint32 storage = G3Function800415CC(pool, frame);
    if (model == 0 || storage == 0)
        return;
    w_u16(frame + 0x30, 0x1408);
    w_u16(frame + 0x32, 0x1408);
    w_u16(frame + 0x34, 0x1408);
    w_u16(frame + 0x18, r_u16(object + 0x58));
    w_u16(frame + 0x1a, r_u16(object + 0x5a));
    w_u16(frame + 0x1c, r_u16(object + 0x5c));
    uint32 x = 0 - r_u32(object + 0x104);
    if ((sint32)x < 0)
        x += 3;
    w_u16(frame + 0x20, (uint32)((sint32)x >> 2));
    uint32 y = 0 - r_u32(object + 0x108);
    if ((sint32)y < 0)
        y += 3;
    w_u16(frame + 0x22, (uint32)((sint32)y >> 2));
    w_u16(frame + 0x24, 0);
    w_u16(frame + 0x20, 0);
    w_u16(frame + 0x22, 0);
    w_u16(frame + 0x24, 0);
    w_u16(frame + 0x28, 0);
    w_u16(frame + 0x2a, 0);
    w_u16(frame + 0x2c, 0);
    sint32 random = (sint32)G3SdkRandom80027FA8();
    uint32 count = (uint32)(sint32)(sint16)(random % 4 + 8);
    pool = r_u32(0x800a27fc);
    w_u32(frame + 0x10, storage);
    uint32 effect = G3Function80044688(frame + 0x18, model, pool, count, frame);
    if (effect != 0)
    {
        G3Function80044A90(effect, frame + 0x20, frame + 0x28);
        G3Function80044878(effect, frame + 0x30, 0);
    }
}

void G3GxSpitUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000230, "G3L_44");
    uint32 frame = guest_stack - 0x18;
    w_u16(object + 0x9e, r_u16(object + 0x9e) + 1);
    uint32 lifetime = r_u32(object + 0x128);
    if (lifetime != 0)
    {
        if ((lifetime & 3) == 0)
            G3GxSpitTrail(object, frame);
        lifetime = r_u32(object + 0x128);
        uint32 x = r_u16(object + 0x58);
        uint32 vx = r_u16(object + 0x104);
        uint32 vz = r_u16(object + 0x10c);
        w_u32(object + 0x128, lifetime - 1);
        uint32 y = r_u16(object + 0x5a);
        uint32 vy = r_u16(object + 0x108);
        w_u16(object + 0x58, x + vx);
        uint32 z = r_u16(object + 0x5c);
        w_u16(object + 0x5a, y + vy);
        w_u16(object + 0x5c, z + vz);
    }
    else
    {
        uint32 counter = r_u32(object + 0x12c) - 1;
        w_u32(object + 0x12c, counter);
        if (counter == 0xffffffff)
            G3Function80038798(object, 0, 0, frame);
    }
}

void G3GxSpitDefaultCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000002ec, "G3L_44");
    uint32 frame = guest_stack - 0x50;
    uint32 contact = r_u32(object + 0xb0);
    uint32 lifetime = r_u32(object + 0x128);
    uint32 other = r_u32(contact + 0x14);
    if (lifetime == 0)
        return;
    if (other != 0)
    {
        G3Function8003C548(contact, frame + 0x28, frame + 0x29);
        uint32 type = r_u8(frame + 0x29);
        if (type != 9 && type - 3 >= 3)
            return;
        w_u32(frame + 0x10, 1);
        w_u16(frame + 0x20, 0);
        w_u16(frame + 0x22, 0);
        w_u32(frame + 0x24, contact);
        uint32 descriptor = r_u32(other + 0x1c);
        if ((r_u32(descriptor + 0x3c) & 0x200) != 0)
            G3Function800193F8(0x200, other, frame);
        uint32 target = r_u32(other + 0xec);
        if (target != 0)
            G3DispatchStateEvent(target, other, game, 8, frame + 0x10, frame);
    }
    w_u32(object + 0x12c, 16);
    w_u32(object + 0x128, 0);
    w_u16(object + 0xa2, 1);
    w_u16(object + 0x9e, 0);
}

void G3GxSpitIceCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000400, "G3L_44");
    uint32 frame = guest_stack - 0x50;
    uint32 contact = r_u32(object + 0xb0);
    uint32 lifetime = r_u32(object + 0x128);
    uint32 other = r_u32(contact + 0x14);
    if (lifetime == 0)
        return;
    if (other != 0)
    {
        G3Function8003C548(contact, frame + 0x28, frame + 0x29);
        uint32 type = r_u8(frame + 0x29);
        if (type != 9 && type - 3 >= 3)
            return;
        w_u32(frame + 0x10, 1);
        w_u16(frame + 0x20, 0);
        w_u16(frame + 0x22, 0);
        w_u32(frame + 0x24, contact);
        uint32 descriptor = r_u32(other + 0x1c);
        if ((r_u32(descriptor + 0x3c) & 0x200) != 0)
            G3Function800193F8(0x200, other, frame);
        uint32 target = r_u32(other + 0xec);
        uint32 result = 0;
        if (target != 0)
            result = G3DispatchStateEvent(target, other, game, 2013, frame + 0x10, frame);
        if (target == 0 || result == 0)
        {
            descriptor = r_u32(other + 0x1c);
            if ((r_u32(descriptor + 0x3c) & 1) != 0)
                G3Function80082648(other, 0, frame);
            else
            {
                target = r_u32(other + 0xec);
                if (target != 0)
                    G3DispatchStateEvent(target, other, game, 8, frame + 0x10, frame);
            }
        }
    }
    w_u32(object + 0x12c, 16);
    w_u32(object + 0x128, 0);
    w_u16(object + 0xa2, 1);
    w_u16(object + 0x9e, 0);
}

void G3GxSpitFireCollide(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000564, "G3L_44");
    uint32 frame = guest_stack - 0x50;
    uint32 contact = r_u32(object + 0xb0);
    uint32 lifetime = r_u32(object + 0x128);
    uint32 other = r_u32(contact + 0x14);
    if (lifetime == 0)
        return;
    if (other != 0 && r_u32(other + 0xec) != 0)
    {
        G3Function8003C548(contact, frame + 0x28, frame + 0x29);
        uint32 type = r_u8(frame + 0x29);
        if (type != 9 && type - 3 >= 3)
            return;
        w_u32(frame + 0x10, 0x7fff);
        w_u16(frame + 0x20, 0);
        w_u16(frame + 0x22, 0);
        w_u32(frame + 0x24, contact);
        G3Function800374FC(other, 0x7fff, frame);
        uint32 descriptor = r_u32(other + 0x1c);
        if ((r_u32(descriptor + 0x3c) & 0x200) != 0)
            G3Function800193F8(0x200, other, frame);
        uint32 target = r_u32(other + 0xec);
        uint32 result = 0;
        if (target != 0)
            result = G3DispatchStateEvent(target, other, game, 2012, frame + 0x10, frame);
        if (target == 0 || result == 0)
        {
            descriptor = r_u32(other + 0x1c);
            if ((r_u32(descriptor + 0x3c) & 1) != 0)
                G3Function80082854(other, frame);
            else
            {
                target = r_u32(other + 0xec);
                if (target != 0)
                    G3DispatchStateEvent(target, other, game, 8, frame + 0x10, frame);
            }
        }
    }
    w_u32(object + 0x12c, 16);
    w_u32(object + 0x128, 0);
    w_u16(object + 0xa2, 1);
    w_u16(object + 0x9e, 0);
}

uint32 G3HankEvent0000(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1: G3HankInitialize3A74(object, module_base, frame); break;
    case 2: G3HankUpdate7E08(object, game, module_base, frame); break;
    case 3: G3HankContact81E8(object, game, module_base, frame); break;
    case 5:
    {
        uint32 value = r_u32(payload + 0x1c);
        if (r_s16(value) == 1)
            w_u16(value, 2);
        break;
    }
    case 7:
    {
        uint32 special = r_u32(object + 0x11c);
        if (special != 0)
            G3HankNotifyDisable41A4(r_u32(special), frame);
        break;
    }
    case 8: case 0x7e4: G3HankDamage80F4(object, module_base, frame); break;
    case 9: G3HankCommand8458(object, payload, module_base); break;
    default: return 0;
    }
    return 1;
}

uint32 G3HankNextField0140(uint32 cursor, uint32 remaining, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000140, "G3L_33");
    uint32 frame = guest_stack - 0x30;
    uint32 result = G3Function80085DFC(cursor, frame + 0x10, frame + 0x14, frame + 0x18);
    if (result == 0)
    {
        w_u32(remaining, 0);
        return 0;
    }
    uint32 kind = r_u16(frame + 0x10);
    if (kind - 0x42 < 0xd)
    {
        w_u16(cursor + 4, kind);
        w_u16(cursor + 6, 0xffff);
        return 0;
    }
    uint32 data = r_u32(frame + 0x14);
    uint32 bytes = r_u16(frame + 0x18);
    w_u16(cursor + 6, kind);
    w_u32(cursor + 8, data);
    w_u16(cursor + 0xc, bytes);
    return 1;
}

void G3HankLoadCore01CC(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000001cc, "G3L_33");
    uint32 frame = guest_stack - 0x30;
    uint32 initial = r_u32(state);
    if (initial == 0)
    {
        uint32 channel = G3Function800833EC(state - 0x104);
        w_u32(settings, channel);
        if (channel == 0)
        {
            while (G3HankNextField0140(cursor, remaining, frame) != 0) {}
            return;
        }
        uint32 position = G3Function8003FBB0(channel, settings + 4, frame);
        uint32 own = r_u16(state - 0xac);
        uint32 origin = r_u16(position);
        w_u16(settings + 8, own - origin);
        own = r_u16(state - 0xaa);
        origin = r_u16(position + 2);
        w_u16(settings + 0xa, own - origin);
        own = r_u16(state - 0xa8);
        origin = r_u16(position + 4);
        w_u16(settings + 0x1a, 0xf);
        w_u16(settings + 0x20, 0x800);
        w_u16(settings + 0x22, 0x28);
        w_u16(settings + 0x44, 5);
        w_u16(settings + 0x46, 0x38);
        w_u16(settings + 0xe, 0);
        w_u16(settings + 0x12, 0xffff);
        w_u16(settings + 0x10, 0xffff);
        w_u16(settings + 0x14, 0xffff);
        w_u16(settings + 0x16, 0x50);
        w_u16(settings + 0x24, 0x50);
        w_u16(settings + 0x34, 0);
        w_u16(settings + 0x36, 0);
        w_u16(settings + 0x40, 0xffff);
        w_u16(settings + 0x42, 0);
        w_u16(settings + 0xc, own - origin);
        w_u32(state, 0x29a);
    }
    else if (initial == 0x29a) w_u32(state, 0x16062);
    do
    {
        sint32 kind = (sint16)(r_u16(cursor + 6) - 0x2f);
        switch (kind)
        {
        case 0:
            w_u16(settings + 0x1a, r_u16(r_u32(cursor + 8)));
            break;
        case 2:
            {
                uint32 data = r_u32(cursor + 8);
                sint32 value = r_s16(data) * 4096 / 3600;
                w_u16(settings + 0x16, (uint32)value);
            }
            break;
        case 0x20:
            {
                uint32 data = r_u32(cursor + 8);
                sint32 value = r_s16(data) * 4096 / 100;
                w_u16(settings + 0x20, (uint32)value);
            }
            break;
        case 0x22:
            w_u16(settings + 0xe, r_u16(r_u32(cursor + 8)));
            w_u16(settings + 0x10, r_u16(r_u32(cursor + 8) + 2));
            if (r_s16(cursor + 0xc) >= 5) w_u16(settings + 0x12, r_u16(r_u32(cursor + 8) + 4));
            break;
        case 0x32:
            w_u16(settings + 0x14, r_u16(r_u32(cursor + 8)));
            break;
        case 0x36:
            w_u16(settings + 0x22, r_u16(r_u32(cursor + 8)));
            break;
        case 0x37:
            {
                uint32 data = r_u32(cursor + 8);
                sint32 value = r_s16(data) * 4096 / 3600;
                w_u16(settings + 0x24, (uint32)value);
            }
            break;
        case 0x50:
        case 0x51:
            {
                sint32 bytes = kind == 0x50 ? (sint16)r_u16(cursor + 0xc) : -r_s16(cursor + 0xc);
                w_u16(settings + 0x34, (uint32)(bytes / 6));
                w_u32(settings + 0x38, r_u32(cursor + 8));
            }
            break;
        case 0x56:
        case 0x57:
            {
                sint32 bytes = kind == 0x56 ? (sint16)r_u16(cursor + 0xc) : -r_s16(cursor + 0xc);
                w_u16(settings + 0x36, (uint32)(bytes / 6));
                w_u32(settings + 0x3c, r_u32(cursor + 8));
            }
            break;
        case 0x6b:
            w_u16(settings + 0x40, r_u16(r_u32(cursor + 8)));
            break;
        case 0x77:
            w_u16(settings + 0x42, 1);
            break;
        case 0x7c:
            w_u16(settings + 0x44, r_u16(r_u32(cursor + 8)));
            break;
        case 0x7d:
            {
                uint32 data = r_u32(cursor + 8);
                sint32 value = r_s16(data) * 4096 / 3600;
                w_u16(settings + 0x46, (uint32)value);
            }
            break;
        }
    } while (G3HankNextField0140(cursor, remaining, frame) != 0);
}

void G3HankLoadIdle0584(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000584, "G3L_33");
    uint32 frame = guest_stack - 0x28;
    uint32 initial = r_u32(state + 4);
    if (initial == 0)
    {
        w_u16(settings + 0x48, 0);
        w_u16(settings + 0x4e, 0xffff);
        w_u16(settings + 0x4c, 0xffff);
        w_u16(settings + 0x4a, 0xffff);
        w_u16(settings + 0x50, 0xb4);
        w_u16(settings + 0x5c, 0x1e);
        w_u16(settings + 0x60, 0xb4);
        w_u16(settings + 0x52, 0);
        w_u16(settings + 0x54, 0);
        w_u16(settings + 0x56, 0);
        w_u16(settings + 0x58, 0);
        uint32 heading = r_u16(state - 0xa0);
        w_u16(settings + 0x68, 0x28);
        w_u16(settings + 0x64, 0);
        w_u16(settings + 0x6a, 0xffff);
        w_u16(settings + 0x66, heading);
        w_u32(state + 4, 0x29a);
    }
    else if (initial == 0x29a) w_u32(state + 4, 0x16062);
    do
    {
        sint32 kind = r_s16(cursor + 6);
        switch (kind)
        {
        case 0x29: w_u16(settings + 0x60, r_u16(r_u32(cursor + 8))); break;
        case 0x31:
            w_u16(settings + 0x68, (uint32)(r_s16(r_u32(cursor + 8)) * 4096 / 3600));
            break;
        case 0x51:
            {
                uint32 value = r_u16(r_u32(cursor + 8));
                uint32 bytes = r_u16(cursor + 0xc);
                w_u16(settings + 0x48, value);
                sint32 count = (sint16)bytes >> 1;
                if (count >= 2) w_u16(settings + 0x4a, r_u16(r_u32(cursor + 8) + 2));
                if (count >= 3) w_u16(settings + 0x4c, r_u16(r_u32(cursor + 8) + 4));
                if (count >= 4) w_u16(settings + 0x4e, r_u16(r_u32(cursor + 8) + 6));
                if (count >= 5) w_u16(settings + 0x6a, r_u16(r_u32(cursor + 8) + 8));
            }
            break;
        case 0x62: w_u16(settings + 0x5c, r_u16(r_u32(cursor + 8))); break;
        case 0x63:
            {
                sint32 value = r_s16(r_u32(cursor + 8)) * 4096 / 100;
                w_u16(settings + 0x52, (uint32)value);
                value = r_s16(r_u32(cursor + 8) + 2) * 4096 / 100;
                w_u16(settings + 0x54, (uint32)value);
                value += r_s16(r_u32(cursor + 8) + 4) * 4096 / 100;
                w_u16(settings + 0x56, (uint32)value);
                value += r_s16(r_u32(cursor + 8) + 6) * 4096 / 100;
                w_u16(settings + 0x58, (uint32)value);
            }
            break;
        case 0x64: w_u16(settings + 0x50, r_u16(r_u32(cursor + 8))); break;
        case 0x78: w_u16(settings + 0x64, 1); break;
        }
    } while (G3HankNextField0140(cursor, remaining, frame) != 0);
}

void G3HankLoadPursuit08A4(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000008a4, "G3L_33");
    uint32 frame = guest_stack - 0x28;
    uint32 initial = r_u32(state + 8);
    if (initial == 0)
    {
        w_u32(settings + 0x70, 0x5f5e10);
        w_u16(settings + 0x74, 0xffff);
        w_u16(settings + 0x7e, 0xf);
        w_u16(settings + 0x7a, 10);
        w_u16(settings + 0x78, 0x28);
        w_u32(settings + 0x6c, 0x3d0900);
        w_u16(settings + 0x76, 0);
        w_u16(settings + 0x84, 0);
        w_u32(state + 8, 0x29a);
    }
    else if (initial == 0x29a) w_u32(state + 8, 0x16062);
    do
    {
        sint32 kind = r_s16(cursor + 6);
        switch (kind)
        {
        case 1:
            {
                sint32 distance = r_s16(r_u32(cursor + 8));
                w_u32(settings + 0x6c, (uint32)(distance * distance));
                distance = r_s16(r_u32(cursor + 8) + 2);
                w_u32(settings + 0x70, (uint32)(distance * distance));
            }
            break;
        case 0x2f: w_u16(settings + 0x7e, r_u16(r_u32(cursor + 8))); break;
        case 0x31:
            w_u16(settings + 0x78, (uint32)(r_s16(r_u32(cursor + 8)) * 4096 / 3600));
            break;
        case 0x32:
            w_u16(settings + 0x7a, (uint32)(r_s16(r_u32(cursor + 8)) * 4096 / 3600));
            break;
        case 0x51:
            w_u16(settings + 0x76, r_u16(r_u32(cursor + 8)));
            if (r_s16(cursor + 0xc) >= 4) w_u16(settings + 0x74, r_u16(r_u32(cursor + 8) + 2));
            break;
        case 0x85:
        case 0x86:
            {
                sint32 bytes = kind == 0x85 ? (sint16)r_u16(cursor + 0xc) : -r_s16(cursor + 0xc);
                w_u16(settings + 0x84, (uint32)(bytes / 6));
                w_u32(settings + 0x88, r_u32(cursor + 8));
            }
            break;
        }
    } while (G3HankNextField0140(cursor, remaining, frame) != 0);
}

void G3HankLoadAttack0B30(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000b30, "G3L_33");
    uint32 frame = guest_stack - 0x20;
    uint32 initial = r_u32(state + 0xc);
    if (initial == 0)
    {
        w_u32(settings + 0x8c, 0xf4240);
        w_u16(settings + 0x92, 0xffff);
        w_u16(settings + 0x94, 2);
        w_u16(settings + 0x90, 0);
        w_u16(settings + 0x96, 0);
        w_u32(state + 0xc, 0x29a);
    }
    else if (initial == 0x29a) w_u32(state + 0xc, 0x16062);
    do
    {
        switch (r_s16(cursor + 6))
        {
        case 1:
            {
                sint32 distance = r_s16(r_u32(cursor + 8));
                w_u32(settings + 0x8c, (uint32)(distance * distance));
            }
            break;
        case 0x33:
            w_u16(settings + 0x94, (uint32)(r_s16(r_u32(cursor + 8)) * 4096 / 3600));
            break;
        case 0x51:
            w_u16(settings + 0x90, r_u16(r_u32(cursor + 8)));
            w_u16(settings + 0x92, r_u16(r_u32(cursor + 8) + 2));
            break;
        case 0x5e: w_u16(settings + 0x96, r_u16(r_u32(cursor + 8))); break;
        }
    } while (G3HankNextField0140(cursor, remaining, frame) != 0);
}

void G3HankLoadLeap0C9C(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000c9c, "G3L_33");
    uint32 frame = guest_stack - 0x28;
    uint32 initial = r_u32(state + 0x10);
    if (initial == 0)
    {
        w_u32(settings + 0xa0, 0x3d0900);
        w_u32(settings + 0x9c, 0xf4240);
        w_u16(settings + 0xa6, 0x4);
        w_u16(settings + 0xac, 0x155);
        w_u16(settings + 0xae, 0x28);
        w_u16(settings + 0xb0, 0x96);
        w_u16(settings + 0xb2, 0xa);
        w_u16(settings + 0xb4, 0x5);
        w_u16(settings + 0xb6, 0xc8);
        w_u16(settings + 0xbe, 0x320);
        w_u32(settings + 0x98, 0x0);
        w_u16(settings + 0xa4, 0x0);
        w_u16(settings + 0xa8, 0x0);
        w_u16(settings + 0xaa, 0x32);
        w_u16(settings + 0xb8, 0x32);
        w_u16(settings + 0xba, 0x0);
        w_u16(settings + 0xbc, 0x0);
        w_u16(settings + 0xc0, 0x0);
        w_u16(settings + 0xc2, 0x0);
        w_u32(state + 0x10, 0x29a);
    }
    else if (initial == 0x29a) w_u32(state + 0x10, 0x16062);
    do
    {
        switch ((sint16)(r_u16(cursor + 6) - 1))
        {
        case 0:
            {
                sint32 distance = r_s16(r_u32(cursor + 8));
                w_u32(settings + 0x9c, (uint32)(distance * distance));
                distance = r_s16(r_u32(cursor + 8) + 2);
                w_u32(settings + 0xa0, (uint32)(distance * distance));
            }
            break;
        case 0x51:
            {
                uint32 data = r_u32(cursor + 8);
                uint32 game = r_u32(0x80097048);
                uint32 resource = G3FindLevelResource800397B0(data, game);
                w_u32(settings + 0x98, resource);
            }
            break;
        case 0xc: w_u16(settings + 0xb0, r_u16(r_u32(cursor + 8))); break;
        case 0x50: w_u16(settings + 0xa4, r_u16(r_u32(cursor + 8))); break;
        case 0x52: w_u16(settings + 0xaa, r_u16(r_u32(cursor + 8))); break;
        case 0x55: w_u16(settings + 0xb2, r_u16(r_u32(cursor + 8))); break;
        case 0x56: w_u16(settings + 0xb4, r_u16(r_u32(cursor + 8))); break;
        case 0x57: w_u16(settings + 0xb6, r_u16(r_u32(cursor + 8))); break;
        case 0x58: w_u16(settings + 0xb8, r_u16(r_u32(cursor + 8))); break;
        case 0x5a: w_u16(settings + 0xc0, r_u16(r_u32(cursor + 8))); break;
        case 0x5b: w_u16(settings + 0xc2, r_u16(r_u32(cursor + 8))); break;
        case 0x5d: w_u16(settings + 0xa8, r_u16(r_u32(cursor + 8))); break;
        case 0x32: w_u16(settings + 0xa6, (uint32)(r_s16(r_u32(cursor + 8)) * 4096 / 3600)); break;
        case 0x53: w_u16(settings + 0xac, (uint32)(r_s16(r_u32(cursor + 8)) * 4096 / 3600)); break;
        case 0x54: w_u16(settings + 0xae, (uint32)(r_s16(r_u32(cursor + 8)) * 4096 / 3600)); break;
        case 0x59:
            {
                uint32 source = r_u32(cursor + 8);
                uint32 destination = settings + 0xba;
                uint32 address = source;
                uint32 high_address = address + 3;
                uint32 high_lane = high_address & 3;
                uint32 lane = address & 3;
                uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                uint32 low = r_masked_u32(address & 0xfffffffc, (15 << lane) & 15);
                uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
                uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
                uint32 z = (uint32)r_s16(source + 4);
                address = destination;
                high_address = address + 3;
                high_lane = high_address & 3;
                lane = address & 3;
                w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                w_masked_u32(address & 0xfffffffc, value << (lane * 8), (15 << lane) & 15);
                w_u16(destination + 4, z);
            }
            break;
        }
    } while (G3HankNextField0140(cursor, remaining, frame) != 0);
}
