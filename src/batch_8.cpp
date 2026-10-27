#include "psx.h"
#include "xport.h"
#include "game_functions.h"

uint32 G3Function8003FB7C(uint32 channel, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003fb7c, "SLUS_008.06");
    if (channel == 0)
        return 0;
    uint32 flags = r_u16(state + 2);
    w_u16(state, 0);
    flags &= 1;
    w_u16(state + 2, flags);
    w_u16(state + 2, flags & 0xfffe);
    return r_u32(channel) + 2;
}

uint32 G3Function8003FBB0(uint32 channel, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003fbb0, "SLUS_008.06");
    if (channel == 0)
        return 0;
    uint32 flags = r_u16(state + 2);
    w_u16(state, 0);
    flags &= 1;
    w_u16(state + 2, flags);
    w_u16(state + 2, flags & 0xfffe);
    return r_u32(channel) + 2;
}

uint32 G3Function8003C548(uint32 contact, uint32 first_type, uint32 second_type)
{
    FUNCTION_MARKER(0x8003c548, "SLUS_008.06");
    w_u8(first_type, 0xff);
    w_u8(second_type, 0xff);
    sint32 kind = r_s16(contact + 4);
    if (kind == 5)
    {
        uint32 data = r_u32(contact + 8);
        w_u8(first_type, r_u8(data + 3));
    }
    else if (kind == 1)
    {
        uint32 data = r_u32(contact + 8);
        w_u8(first_type, r_u8(data + 5));
    }
    else if (kind == 2)
    {
        uint32 data = r_u32(contact + 8);
        w_u8(first_type, r_u8(data + 0xb));
    }
    kind = r_s16(contact + 6);
    uint32 result = 2;
    if (kind == 5)
    {
        uint32 data = r_u32(contact + 0xc);
        result = r_u8(data + 3);
        w_u8(second_type, result);
    }
    else if (kind == 1)
    {
        uint32 data = r_u32(contact + 0xc);
        result = r_u8(data + 5);
        w_u8(second_type, result);
    }
    else if (kind == 2)
    {
        uint32 data = r_u32(contact + 0xc);
        result = r_u8(data + 0xb);
        w_u8(second_type, result);
    }
    return result;
}

uint32 G3Function8003C6E0(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003c6e0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    uint32 contact = r_u32(object + 0xb0);
    uint32 linked = r_u32(contact + 0x14);
    G3Function8003C548(contact, frame + 0x28, frame + 0x29);
    uint32 first_type = r_u8(frame + 0x28);
    w_u32(frame + 0x10, 1);
    w_u16(frame + 0x20, 0);
    w_u16(frame + 0x22, 0);
    w_u32(frame + 0x24, contact);
    bool notify_linked = false;
    if (first_type - 6 < 4)
    {
        uint32 second_type = r_u8(frame + 0x29);
        if (second_type == 9 || second_type - 3 < 3)
            notify_linked = true;
        else
            notify_linked = r_s16(contact + 6) == 4;
    }
    if (notify_linked)
    {
        uint32 descriptor = r_u32(object + 0x1c);
        uint32 flags = r_u32(descriptor + 0x30);
        uint32 type = 0;
        if (flags & 1)
            type = 3;
        else if (flags & 2)
            type = 4;
        else if (flags & 0x08000000)
            type = 7;
        else
        {
            uint32 secondary_flags = r_u32(descriptor + 0x3c);
            if (secondary_flags & 8)
                type = 9;
            else if (secondary_flags & 4)
                type = 8;
        }
        if (type != 0)
            w_u16(frame + 0x20, type);
        uint32 target = r_u32(linked + 0xec);
        if (target == 0)
            return target;
        return G3DispatchStateEvent(target, linked, game, 8, frame + 0x10, frame);
    }
    uint32 flags = r_u16(object + 0x116);
    if ((flags & 0x200) == 0)
        return 9;
    first_type = r_u8(frame + 0x28);
    uint32 result = first_type - 3;
    if (first_type != 9)
    {
        result = result < 3;
        if (result == 0)
            return result;
    }
    sint32 kind = r_s16(contact + 6);
    if (kind == 3)
        return 3;
    uint32 target = r_u32(object + 0xec);
    if (target == 0)
        return target;
    return G3DispatchStateEvent(target, object, game, 8, frame + 0x10, frame);
}

uint32 G3Function8003B8A8(uint32 object, uint32 game, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003b8a8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 contact = r_u32(payload + 0x14);
    uint32 linked = r_u32(contact + 0x10);
    uint32 controlled = r_u32(game + 0x50);
    bool player_contact = linked == controlled;
    if (player_contact)
    {
        uint32 current = r_u32(linked + 0x138);
        uint32 previous = r_u32(object + 0x128);
        if (previous == current)
            return previous;
        w_u32(object + 0x128, current);
    }
    uint32 flags = r_u16(object + 0x116);
    if ((flags & 8) == 0 && (!player_contact || (flags & 0x40) == 0))
        G3Function8003CBC0(object, object + 0x114, payload, frame);
    sint32 mode = r_s8(object + 0x114);
    if (mode <= 0)
    {
        uint32 selector = (uint32)r_s8(object + 0x115);
        return G3Function80038798(object, selector, 0, frame);
    }
    sint32 side = r_s16(payload + 0x10);
    uint32 mask;
    if (side == 0)
        mask = 0x10000;
    else if (side == 1)
        mask = 0x20000;
    else
        return 0xffffffff;
    return G3Function800193F8(mask, object, frame);
}

uint32 G3Function8003CBC0(uint32 object, uint32 state, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003cbc0, "SLUS_008.06");
    uint32 contact = r_u32(payload + 0x14);
    sint32 kind = r_s16(contact + 6);
    uint32 data = r_u32(contact + 8);
    uint32 value;
    if (kind == 5)
        value = (uint32)r_s16(data);
    else
        value = r_u32(data);
    uint32 blocked = value & 0x8000;
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 flags = r_u32(descriptor);
    if ((flags & 0x20000000) != 0)
    {
        uint32 payload_flags = r_u16(payload + 0x12);
        if ((payload_flags & 1) == 0)
            blocked = 1;
    }
    descriptor = r_u32(object + 0x1c);
    flags = r_u32(descriptor);
    if (flags & 0x02000000)
    {
        sint32 side = r_s16(payload + 0x10);
        if (side != 2)
            blocked = 1;
    }
    else if (flags & 0x01000000)
    {
        sint32 side = r_s16(payload + 0x10);
        if (side != 0)
            blocked = 1;
    }
    else if (flags & 0x04000000)
    {
        sint32 side = r_s16(payload + 0x10);
        if (side != 1 && side != 3)
            blocked = 1;
    }
    if (blocked != 0)
        return 0;
    if (state != 0)
    {
        uint32 previous = r_u8(state);
        uint32 decrement = r_u8(payload);
        uint32 remaining = previous - decrement;
        w_u8(state, remaining);
        if ((sint32)(remaining << 24) <= 0)
            w_u8(state, 0);
    }
    return 1;
}

uint32 G3Function80038798(uint32 object, uint32 selector, uint32 random_range, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80038798, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 flags = r_u32(object + 0x18);
    uint32 descriptor = r_u32(object + 0x1c);
    w_u32(object + 0x18, flags | 0x200);
    G3Function800376A4(object, frame);
    uint32 intro = r_u32(object + 0x20);
    if (intro != 0)
    {
        uint32 intro_flags = r_u32(intro + 0x18);
        w_u32(intro + 0x18, intro_flags | 0x10);
    }
    if (random_range != 0)
    {
        sint32 random = (sint32)G3SdkRandom80027FA8();
        sint32 divisor = (sint32)(random_range + 1);
        sint32 remainder;
        if (divisor == 0)
            remainder = random;
        else if ((uint32)random == 0x80000000 && divisor == -1)
            remainder = 0;
        else
            remainder = random % divisor;
        selector += (uint32)remainder;
    }
    uint32 resource_flags = r_u32(descriptor + 0x30);
    if ((resource_flags & 8) == 0)
        return G3Function80038318(object, selector, frame);
    G3Function800374FC(object, resource_flags & 8, frame);
    return G3Function800376D8(object, frame);
}

uint32 G3Function8003742C(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003742c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 result = r_u32(descriptor + 0x30) & 0x1000;
    if (result == 0)
        return result;
    uint32 flags = r_u32(object + 0x14);
    w_u32(object + 0x14, flags | 0x100000);
    G3Function8003D390(object, frame);
    flags = r_u32(object + 0x14);
    if ((flags & 0x2000) == 0)
        return 0xffffdfff;
    uint32 handle = r_u32(object + 0x44);
    w_u32(object + 0x14, flags & 0xffffdfff);
    result = G3Function8003F18C(handle);
    w_u32(object + 0x44, 0);
    return result;
}

uint32 G3Function800374A4(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800374a4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 result = r_u32(object + 0x14) & 0x04000000;
    if (result == 0)
    {
        G3Function800193F8(0x40000, object, frame);
        result = r_u32(object + 0x14) | 0x04000000;
        w_u32(object + 0x14, result);
    }
    return result;
}

uint32 G3Function8003763C(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003763c, "SLUS_008.06");
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 result = r_u32(descriptor + 0x10);
    if (result == 0)
        return result;
    result = r_u16(object + 0x5e) - 1;
    w_u16(object + 0x5e, result);
    result <<= 16;
    if ((sint32)result < 0)
    {
        descriptor = r_u32(object + 0x1c);
        uint32 index = r_u16(object + 0x6e);
        uint32 table = r_u32(descriptor + 0x10);
        uint32 entry = r_u32(table + (index << 2));
        result = r_u16(entry + 2) - 1;
        w_u16(object + 0x5e, result);
    }
    return result;
}

uint32 G3Function800376A4(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800376a4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function8003742C(object, frame);
    G3Function800374A4(object, frame);
    return G3Function8003763C(object, frame);
}

uint32 G3Function800376D8(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800376d8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 intro = r_u32(object + 0x20);
    if (intro != 0)
    {
        uint32 flags = r_u32(intro + 0x18);
        if ((flags & 4) != 0)
            G3Function800392A0(object, frame);
    }
    return G3Function8002A9AC(object, frame);
}

uint32 G3Function800374FC(uint32 object, uint32 incoming_v0, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800374fc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    uint32 intro = r_u32(object + 0x20);
    if (intro == 0)
        return incoming_v0;
    uint32 result = (uint32)r_s16(intro + 0x2e);
    if (result == 0)
        return result;
    sint32 count = r_s16(intro + 0x2c);
    if (count <= 0)
        return result;
    for (sint32 index = 0; index < count; ++index)
    {
        sint32 random = (sint32)G3SdkRandom80027FA8();
        uint32 x = (uint32)(random % 30) - 15;
        random = (sint32)G3SdkRandom80027FA8();
        w_u32(frame + 0x10, 0);
        w_u32(frame + 0x14, 0);
        w_u32(frame + 0x18, 0xfffffffa);
        w_u32(frame + 0x1c, 0);
        w_u32(frame + 0x20, 0);
        w_u32(frame + 0x24, 0);
        uint32 y = (uint32)(random % 30) - 15;
        uint32 spawned = G3Function800460E8(object, x, y, 30, frame);
        if (spawned != 0)
        {
            uint32 z = r_u16(spawned + 0x5c);
            w_u16(spawned + 0x5c, z + 0x100);
        }
    }
    return 0;
}

uint32 G3Function80046064(uint32 object)
{
    FUNCTION_MARKER(0x80046064, "SLUS_008.06");
    uint32 intro = r_u32(object + 0x20);
    if (intro == 0)
        return 0xffffffff;
    sint32 offset = r_s16(intro + 0x2e);
    if (offset == 0)
        return 0xffffffff;
    uint32 game = r_u32(0x80097048);
    uint32 table = r_u32(game + 0x68b0);
    uint32 first = table + (uint32)offset;
    sint32 count = r_s16(intro + 0x2c);
    uint32 cursor = first - 1;
    if (count <= 0)
        return 0xffffffff;
    sint32 index = 0;
    do
    {
        uint32 occupied = r_u8(cursor);
        if (occupied == 0)
        {
            w_u8(cursor, 1);
            return (uint32)index;
        }
        ++index;
        ++cursor;
    } while (index < count);
    return 0xffffffff;
}

uint32 G3Function8002A454(uint32 object, uint32 data, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002a454, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    if (data == 0)
        return 0;
    uint32 game = r_u32(0x80097048);
    uint32 pool = r_u32(game + 0x5c);
    uint32 created = G3Function800290A0(pool, frame);
    if (created == 0)
        return 0;
    G3Function8002A7EC(created, data, frame);
    uint32 animation = r_u32(data + 0x10);
    if (animation != 0 && G3Function80030454(created, frame) == 0)
    {
        game = r_u32(0x80097048);
        pool = r_u32(game + 0x5c);
        G3Function800292F0(pool, created, frame);
        return 0;
    }
    uint32 flags = r_u32(created + 0x10);
    w_u32(created + 0x10, flags | 2);
    if (object != 0)
    {
        w_u32(created + 0x2c, object);
        for (uint32 copy = 0; copy < 3; ++copy)
        {
            uint32 source = (copy == 1 ? created : object) + 0x58;
            uint32 destination = created + (copy == 0 ? 0x58 : (copy == 1 ? 0x88 : 0x70));
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low_lane = source & 3;
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
            uint32 word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
            uint32 z = (uint32)r_s16(source + 4);
            high_address = destination + 3;
            high_lane = high_address & 3;
            w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            low_lane = destination & 3;
            w_masked_u32(destination & 0xfffffffc, word << (low_lane * 8), (15 << low_lane) & 15);
            w_u16(destination + 4, z);
        }
        uint32 rotation[2];
        for (uint32 index = 0; index < 2; ++index)
        {
            uint32 source = object + 0x60 + (index << 2);
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low_lane = source & 3;
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
            rotation[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 index = 0; index < 2; ++index)
        {
            uint32 destination = created + 0x60 + (index << 2);
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            w_masked_u32(high_address & 0xfffffffc, rotation[index] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            uint32 low_lane = destination & 3;
            w_masked_u32(destination & 0xfffffffc, rotation[index] << (low_lane * 8), (15 << low_lane) & 15);
        }
        w_u16(created + 0x7c, 0);
        w_u16(created + 0x7a, 0);
        w_u16(created + 0x78, 0);
        uint32 source = object + 0x68;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
        uint32 word = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        uint32 z = (uint32)r_s16(object + 0x6c);
        uint32 destination = created + 0x68;
        high_address = destination + 3;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, word >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, word << (low_lane * 8), (15 << low_lane) & 15);
        w_u16(created + 0x6c, z);
        w_u16(created + 0x84, 0xffff);
        w_u16(created + 0x82, 0xffff);
        w_u16(created + 0x80, 0xffff);
        uint32 value = r_u16(object + 0x8e);
        w_u16(created + 0x8e, value);
        value = r_u32(object + 0x38);
        w_u32(created + 0x3c, value);
        value = r_u32(object + 0x10);
        if ((value & 0x20000000) != 0)
        {
            flags = r_u32(created + 0x10);
            w_u32(created + 0x10, flags | 0x20000000);
        }
    }
    else
    {
        w_u16(created + 0x68, 0x1000);
        w_u16(created + 0x6a, 0x1000);
        w_u16(created + 0x6c, 0x1000);
    }
    uint32 descriptor = r_u32(created + 0x1c);
    uint32 models = r_u32(descriptor + 0xc);
    uint32 model = r_u32(models);
    descriptor = r_u32(created + 0x1c);
    uint32 value = r_u16(model + 0x28);
    w_u16(created + 0xc8, value);
    models = r_u32(descriptor + 0xc);
    model = r_u32(models);
    value = r_u16(model + 0x2a);
    w_u16(created + 0xca, value);
    models = r_u32(descriptor + 0xc);
    model = r_u32(models);
    value = r_u16(model + 0x2c);
    w_u16(created + 0xcc, value);
    flags = r_u32(descriptor);
    if ((flags & 0x100) != 0)
        G3Function8002A748(created, frame);
    if (G3Function8008345C(created, 0, 0) == 0)
    {
        flags = r_u32(created + 0x10);
        w_u32(created + 0x10, flags | 0x100000);
    }
    game = r_u32(0x80097048);
    pool = r_u32(game + 0x5c);
    G3Function8002924C(pool, created, frame);
    uint32 callback = r_u32(data + 0x34);
    w_u32(created + 0xec, callback);
    G3Function8003A9F8(created, frame);
    flags = r_u32(data + 0x30);
    if ((flags & 0x20000000) != 0)
        G3Function800193F8(0x20000000, created, frame);
    return created;
}

uint32 G3Function800460E8(uint32 object, uint32 x, uint32 y, uint32 z, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800460e8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 first = r_u32(guest_stack + 0x10);
    uint32 second = r_u32(guest_stack + 0x14);
    uint32 intro = r_u32(object + 0x20);
    uint32 third = r_u32(guest_stack + 0x18);
    if (intro == 0)
        return 0;
    if (r_s16(intro + 0x2e) == 0)
        return 0;
    uint32 resource = r_u32(0x80098764);
    if (resource == 0)
        return 0;
    uint32 descriptor = r_u32(object + 0x1c);
    if (descriptor == resource)
        return 0;
    uint32 slot = G3Function80046064(object);
    if (slot == 0xffffffff)
        return 0;
    uint32 height = r_u16(object + 0x5c);
    w_u16(object + 0x5c, height + 0x80);
    uint32 created = G3Function8002A454(object, resource, frame);
    height = r_u16(object + 0x5c);
    w_u16(object + 0x5c, height - 0x80);
    if (created == 0)
        return created;
    uint32 state = created + 0x128;
    w_u32(created + 0x20, 0);
    w_u16(created + 0x68, 0x1000);
    w_u16(created + 0x6a, 0x1000);
    w_u16(created + 0x6c, 0x1000);
    if (x != 0 || y != 0 || z != 0 || first != 0 || second != 0 || third != 0)
    {
        descriptor = r_u32(created + 0x1c);
        w_u32(created + 0x104, x);
        w_u32(created + 0x110, first);
        w_u32(created + 0x108, y);
        w_u32(created + 0x114, second);
        w_u32(created + 0x10c, z);
        w_u32(created + 0x118, third);
        uint32 flags = r_u32(descriptor);
        if ((flags & 0x100) != 0)
        {
            uint32 node = r_u32(created + 0xe4);
            if (node != 0)
            {
                w_u16(node + 0x22, 0);
                w_u32(created + 0xe4, 0);
            }
            node = r_u32(created + 0xe8);
            if (node != 0)
            {
                w_u16(node + 0x22, 0);
                w_u32(created + 0xe8, 0);
            }
        }
        flags = r_u32(created + 0x14);
        w_u32(created + 0x14, flags | 0x40);
        flags = r_u32(state);
        w_u32(state, flags | 2);
    }
    uint32 flags = r_u32(state);
    w_u32(state + 0x10, 0x1c2);
    w_u32(state, flags | 4);
    intro = r_u32(object + 0x20);
    uint32 offset = r_u16(intro + 0x2e);
    w_u16(state + 0x16, offset + slot);
    return created;
}

uint32 G3Function8003A9F8(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003a9f8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 target = r_u32(object + 0xec);
    if (target == 0)
        return target;
    uint32 game = r_u32(0x80097048);
    G3DispatchStateEvent(target, object, game, 1, 0, frame);
    uint32 result = r_u32(object + 0x14) | 0x200000;
    w_u32(object + 0x14, result);
    return result;
}

uint32 G3Function800290A0(uint32 pool, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800290a0, "SLUS_008.06");
    uint32 arena = r_u32(pool + 8);
    uint32 available = r_u32(arena);
    if (available == 0)
        return 0;
    w_u32(arena, available - 1);
    arena = r_u32(pool + 8);
    uint32 object = r_u32(arena + 0x7644);
    uint32 next = r_u32(object + 8);
    w_u32(arena + 0x7644, next);
    uint32 head = r_u32(pool + 4);
    w_u32(pool + 4, object);
    w_u32(object + 8, head);
    if (head != 0)
        w_u32(head + 0xc, object);
    w_u32(object + 0xc, 0);
    uint32 active = r_u32(pool);
    w_u32(pool, active + 1);
    return object;
}

uint32 G3Function800292F0(uint32 pool, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800292f0, "SLUS_008.06");
    uint32 previous = r_u32(object + 0xc);
    uint32 next = r_u32(object + 8);
    if (previous != 0)
        w_u32(previous + 8, next);
    else
        w_u32(pool + 4, next);
    next = r_u32(object + 8);
    if (next != 0)
    {
        previous = r_u32(object + 0xc);
        w_u32(next + 0xc, previous);
    }
    uint32 active = r_u32(pool);
    uint32 arena = r_u32(pool + 8);
    w_u32(pool, active - 1);
    uint32 available = r_u32(arena);
    w_u32(arena, available + 1);
    uint32 result = r_u32(pool + 8);
    uint32 head = r_u32(result + 0x7644);
    w_u32(result + 0x7644, object);
    w_u32(object + 0xc, 0);
    w_u32(object + 8, head);
    if (head != 0)
        w_u32(head + 0xc, object);
    return result;
}

uint32 G3Function8002924C(uint32 pool, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002924c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 category = G3Function8002910C(object, frame);
    uint32 anchor = pool + (category << 3) + 0xc;
    return G3Function80045B74(anchor, object);
}

uint32 G3Function8002A7EC(uint32 object, uint32 data, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002a7ec, "SLUS_008.06");
    G3FillMemory80086094(object + 0x10, 0, 0x148);
    uint32 serial = r_u32(0x8009d848);
    w_u32(object + 0x1c, data);
    w_u32(object + 0x38, serial);
    uint32 private_data = r_u32(data + 0x1c);
    w_u16(object + 0x9e, 0);
    w_u16(object + 0xce, 0);
    w_u32(object + 0x24, private_data);
    uint32 flags = r_u32(data);
    w_u32(0x8009d848, serial + 1);
    if ((flags & 0x80) != 0)
    {
        flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags | 8);
    }
    uint32 descriptor = r_u32(object + 0x1c);
    flags = r_u32(descriptor);
    if ((flags & 0x200) != 0)
    {
        flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags | 0x40);
    }
    descriptor = r_u32(object + 0x1c);
    flags = r_u32(descriptor);
    if ((flags & 0x10000) != 0)
    {
        flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags | 0x10000000);
    }
    descriptor = r_u32(object + 0x1c);
    flags = r_u32(descriptor);
    if ((flags & 0x84000) == 0)
    {
        flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags | 0x02000000);
    }
    descriptor = r_u32(object + 0x1c);
    flags = r_u32(descriptor + 0x30);
    if ((flags & 0x40000000) != 0)
    {
        flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags | 0x80000000);
    }
    uint32 result = r_u32(object + 0x14) | 0x08000000;
    w_u32(object + 0x14, result);
    return result;
}

uint32 G3Function8002A748(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002a748, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    for (uint32 group = 0; group < 2; ++group)
    {
        uint32 first = group == 0 ? 0 : 3;
        uint32 count = group == 0 ? 3 : 1;
        uint32 words[3];
        for (uint32 index = 0; index < count; ++index)
        {
            uint32 source = 0x8009d864 + ((first + index) << 2);
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low_lane = source & 3;
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
            words[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
        }
        for (uint32 index = 0; index < count; ++index)
        {
            uint32 destination = frame + 0x18 + ((first + index) << 2);
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            uint32 low_lane = destination & 3;
            w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15 << low_lane) & 15);
        }
    }
    uint32 flags = r_u32(object + 0x10);
    uint32 game = r_u32(0x800a27fc);
    w_u32(object + 0x10, flags | 0x40);
    w_u32(frame + 0x10, object + 0xe8);
    G3Function80043720(object, frame + 0x18, game, object + 0xe4, frame);
    uint32 result = r_u32(object + 0x10) & 0xffffffbf;
    w_u32(object + 0x10, result);
    return result;
}

uint32 G3Function8002910C(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002910c, "SLUS_008.06");
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 resource_flags = r_u32(descriptor);
    uint32 result = 0;
    if ((resource_flags & 0x10) != 0)
        result = (r_u32(object + 0x10) & 0x8000) == 0;
    if ((resource_flags & 4) != 0 && (r_u32(object + 0x10) & 0x2000) == 0)
        result |= 2;
    descriptor = r_u32(object + 0x1c);
    resource_flags = r_u32(descriptor);
    if ((resource_flags & 8) != 0 && (r_u32(object + 0x10) & 0x4000) == 0)
        result |= 4;
    descriptor = r_u32(object + 0x1c);
    resource_flags = r_u32(descriptor);
    if ((resource_flags & 2) != 0 && (r_u32(object + 0x10) & 0x1000) == 0)
        result |= 0x40;
    descriptor = r_u32(object + 0x1c);
    resource_flags = r_u32(descriptor);
    if ((resource_flags & 0x10000000) != 0 && (r_u32(object + 0x10) & 0x10000) == 0)
        result |= 8;
    descriptor = r_u32(object + 0x1c);
    resource_flags = r_u32(descriptor);
    if ((resource_flags & 0x08000000) != 0)
        return (result | 0x20) & 0xffffffef & 0xfffffff7;
    if ((resource_flags & 1) != 0)
        return result | 0x10;
    return result & 0xfffffffd & 0xfffffffe;
}

uint32 G3Function80029294(uint32 pool, uint32 object)
{
    FUNCTION_MARKER(0x80029294, "SLUS_008.06");
    uint32 head = r_u32(pool + 4);
    w_u32(pool + 4, object);
    w_u32(object + 0xc, 0);
    w_u32(object + 8, head);
    if (head != 0)
        w_u32(head + 0xc, object);
    return head;
}

uint32 G3Function80043720(uint32 object, uint32 configuration, uint32 game, uint32 first_output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80043720, "SLUS_008.06");
    uint32 frame = guest_stack - 0x80;
    uint32 second_output = r_u32(guest_stack + 0x10);
    G3FillMemory80086094(frame + 0x48, 0, 8);
    uint32 value = r_u16(object + 0x58);
    w_u16(frame + 0x48, value);
    value = r_u16(object + 0x5a);
    w_u16(frame + 0x4a, value);
    value = r_u16(object + 0x5c);
    w_u16(frame + 0x4c, value - 0x1000);
    uint32 words[2];
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 source = frame + 0x48 + (index << 2);
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low_lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
        words[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 index = 0; index < 2; ++index)
    {
        uint32 destination = frame + 0x40 + (index << 2);
        uint32 high_address = destination + 3;
        uint32 high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        uint32 low_lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, words[index] << (low_lane * 8), (15 << low_lane) & 15);
    }
    w_u32(frame + 0x24, frame + 0x40);
    w_u32(frame + 0x28, object + 0x58);
    w_u32(frame + 0x38, object);
    w_u32(frame + 0x34, 9);
    uint32 flags = r_u32(object + 0x10);
    uint32 current_game = r_u32(0x80097048);
    w_u32(object + 0x10, flags | 0x40);
    G3Function80061CA4(frame + 0x10, current_game, frame);
    flags = r_u32(object + 0x10);
    w_u32(object + 0x10, flags & 0xffffffbf);
    uint32 kind = r_u32(frame + 0x10);
    if (kind != 3)
        return 3;
    sint32 normal_z = r_s16(frame + 0x20);
    if (normal_z < 0x100)
        return 1;
    uint32 coordinates[8];
    coordinates[0] = r_u16(configuration);
    sint32 normal_x = r_s16(frame + 0x1c);
    w_u16(frame + 0x48, coordinates[0]);
    coordinates[1] = r_u16(configuration + 2);
    w_u16(frame + 0x4a, coordinates[1]);
    coordinates[2] = r_u16(configuration + 4);
    w_u16(frame + 0x50, coordinates[2]);
    coordinates[3] = r_u16(configuration + 6);
    w_u16(frame + 0x52, coordinates[3]);
    coordinates[4] = r_u16(configuration + 8);
    w_u16(frame + 0x58, coordinates[4]);
    coordinates[5] = r_u16(configuration + 0xa);
    w_u16(frame + 0x5a, coordinates[5]);
    coordinates[6] = r_u16(configuration + 0xc);
    w_u16(frame + 0x60, coordinates[6]);
    coordinates[7] = r_u16(configuration + 0xe);
    w_u16(frame + 0x62, coordinates[7]);
    sint32 normal_y = r_s16(frame + 0x1e);
    sint32 heights[4];
    for (uint32 index = 0; index < 4; ++index)
    {
        sint32 x = (sint16)coordinates[index * 2];
        sint32 y = (sint16)coordinates[index * 2 + 1];
        sint32 numerator = (-x * normal_x) - (y * normal_y);
        heights[index] = numerator / normal_z;
    }
    w_u16(frame + 0x4c, (uint32)heights[0]);
    w_u16(frame + 0x54, (uint32)heights[1]);
    w_u16(frame + 0x5c, (uint32)heights[2]);
    w_u16(frame + 0x64, (uint32)heights[3]);
    uint32 result = G3Function800415CC(game, frame);
    uint32 matrix = result;
    if (matrix == 0)
        return result;
    G3Function8002F718(matrix + 0xc);
    w_u32(matrix + 0x20, 0);
    w_u32(matrix + 0x24, 0);
    w_u32(matrix + 0x28, 0);
    result = G3Function8004161C(game, frame);
    w_u32(first_output, result);
    result = G3Function8004161C(game, frame);
    w_u32(second_output, result);
    uint32 first_node = r_u32(first_output);
    if (first_node == 0 || result == 0)
    {
        if (first_node != 0)
            result = G3Function80045B74(game + 0x11c18, first_node);
        uint32 second_node = r_u32(second_output);
        if (second_node == 0)
            return result;
        return G3Function80045B74(game + 0x11c18, second_node);
    }
    const uint32 first_offsets[12] = {0x40, 0x42, 0x44, 0x50, 0x52, 0x54, 0x58, 0x5a, 0x5c, 0x48, 0x4a, 0x4c};
    const uint32 second_offsets[12] = {0x40, 0x42, 0x44, 0x58, 0x5a, 0x5c, 0x60, 0x62, 0x64, 0x48, 0x4a, 0x4c};
    const uint32 destinations[12] = {0x1c, 0x1e, 0x20, 0x24, 0x26, 0x28, 0x2c, 0x2e, 0x30, 0x34, 0x36, 0x38};
    for (uint32 group = 0; group < 2; ++group)
    {
        uint32 output = group == 0 ? first_output : second_output;
        const uint32 *offsets = group == 0 ? first_offsets : second_offsets;
        for (uint32 index = 0; index < 12; ++index)
        {
            uint32 node = group == 0 && index == 0 ? first_node : r_u32(output);
            value = r_u16(frame + offsets[index]);
            w_u16(node + destinations[index], value);
        }
        uint32 node = r_u32(output);
        w_u32(node + 0x14, matrix);
        node = r_u32(output);
        w_u32(node + 0x10, 0x800436f0);
        node = r_u32(output);
        w_u16(node + 0x22, 1);
    }
    uint32 resource = r_u32(0x80098704);
    if (resource != 0)
    {
        uint32 node = r_u32(first_output);
        flags = r_u32(node + 0xc);
        w_u32(node + 0xc, flags | 1);
        node = r_u32(second_output);
        flags = r_u32(node + 0xc);
        w_u32(node + 0xc, flags | 1);
        resource = r_u32(0x80098704);
        uint32 models = r_u32(resource + 0xc);
        uint32 model = r_u32(models);
        uint32 texture = r_u32(model + 0x14);
        node = r_u32(first_output);
        value = r_u32(texture + 8);
        w_u32(node + 8, value);
        node = r_u32(second_output);
        value = r_u32(texture + 0x14);
        w_u32(node + 8, value);
        node = r_u32(first_output);
        w_u32(node + 0x18, 0x26808080);
        node = r_u32(second_output);
        w_u32(node + 0x18, 0x26808080);
    }
    uint32 anchor = game + 0x11c10;
    uint32 node = r_u32(first_output);
    G3Function80045B74(anchor, node);
    node = r_u32(second_output);
    return G3Function80045B74(anchor, node);
}

uint32 G3Function800415CC(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800415cc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 node = G3Function80045C0C(game + 0x2c08, frame);
    if (node != 0)
    {
        w_u32(node + 8, 1);
        G3Function80045B74(game + 0x2c00, node);
    }
    return node;
}

uint32 G3Function8004161C(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004161c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 node = G3Function80045C0C(game + 0x11c18, frame);
    if (node != 0)
        G3FillMemory80086094(node, 0, 0x78);
    return node;
}

uint32 G3Function80045C0C(uint32 anchor, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80045c0c, "SLUS_008.06");
    uint32 node = r_u32(anchor + 4);
    if (node != 0)
        G3Function80045BA0(node);
    return node;
}

uint32 G3Function80045BA0(uint32 node)
{
    FUNCTION_MARKER(0x80045ba0, "SLUS_008.06");
    uint32 previous = r_u32(node);
    uint32 result;
    bool linked = false;
    if (previous != 0)
    {
        result = r_u32(node + 4);
        if (result != 0)
        {
            w_u32(previous + 4, result);
            uint32 next = r_u32(node + 4);
            result = r_u32(node);
            w_u32(next, result);
            linked = true;
        }
    }
    else
    {
        result = r_u32(node + 4);
        if (result != 0)
        {
            w_u32(result, 0);
            linked = true;
        }
    }
    if (!linked)
    {
        result = r_u32(node);
        if (result != 0)
            w_u32(result + 4, 0);
    }
    w_u32(node + 4, 0);
    w_u32(node, 0);
    return result;
}

uint32 G3Function8003D270(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003d270, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    uint32 contact = r_u32(object + 0xb0);
    uint32 result = (uint32)r_s16(contact + 6);
    uint32 state = r_u32(object + 0x12c);
    if (result != 4)
        return result;
    uint32 linked = r_u32(contact + 0x14);
    result = r_u32(game + 0x50);
    if (linked != result)
        return result;
    uint32 matrix = r_u32(object + 0x30);
    if (matrix == 0)
        return result;
    G3Function800313E0(frame + 0x20, matrix, frame);
    SetRotMatrix((MATRIX *)psx_addr(frame + 0x20, 20));
    SetTransMatrix((MATRIX *)psx_addr(frame + 0x20, 32));
    VECTOR transformed;
    RotTrans((SVECTOR *)psx_addr(contact + 0x18, 8), &transformed, 0);
    w_u32(frame + 0x10, xport_gte_read_data(25));
    w_u32(frame + 0x14, xport_gte_read_data(26));
    w_u32(frame + 0x18, xport_gte_read_data(27));
    w_u32(frame + 0x40, xport_gte_read_control(31));
    result = r_u32(frame + 0x14);
    w_u32(state + 0x1c, (0 - result) << 6);
    result = r_u32(frame + 0x10);
    uint32 mode = r_u32(state + 0x3c);
    w_u32(state + 0x20, result << 6);
    w_u32(state + 0x10, mode != 0 ? 0xfffff000 : 0xffffc000);
    linked = r_u32(contact + 0x14);
    w_u32(state + 0x38, linked);
    return G3Function8003C864(object, game, frame);
}

uint32 G3Function800313E0(uint32 output, uint32 matrix, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800313e0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    sint32 mode = r_s16(matrix + 0x12);
    uint32 source = matrix;
    if (mode == 1)
    {
        G3Function800314D0(frame + 0x20, matrix, frame);
        source = frame + 0x20;
    }
    TransposeMatrix((MATRIX *)psx_addr(source, 18), (MATRIX *)psx_addr(output, 18));
    uint32 value = r_u32(matrix + 0x14);
    w_u32(frame + 0x10, 0 - value);
    value = r_u32(matrix + 0x18);
    w_u32(frame + 0x14, 0 - value);
    value = r_u32(matrix + 0x1c);
    w_u32(frame + 0x18, 0 - value);
    VECTOR translation;
    ApplyMatrixLV((MATRIX *)psx_addr(output, 18), (VECTOR *)psx_addr(frame + 0x10, 12), &translation);
    w_u32(output + 0x14, (uint32)translation.vx);
    w_u32(output + 0x18, (uint32)translation.vy);
    w_u32(output + 0x1c, (uint32)translation.vz);
    return output + 0x14;
}

uint32 G3Function80031474(uint32 matrix, uint32 column, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80031474, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 address = matrix + (column << 1);
    sint32 component = r_s16(address);
    uint32 squared = (uint32)(component * component);
    component = r_s16(address + 6);
    squared += (uint32)(component * component);
    component = r_s16(address + 0xc);
    squared += (uint32)(component * component);
    uint32 root = G3Function8002F368(squared, frame);
    return (uint32)((sint32)root >> 6);
}

uint32 G3Function800436F0(uint32 node, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800436f0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 result = (uint32)r_s16(node + 0x22);
    if (result != 0)
        return result;
    return G3Function80041578(node, game, frame);
}

uint32 G3Function80041578(uint32 node, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80041578, "SLUS_008.06");
    uint32 flags = r_u32(node + 0xc);
    w_u32(node + 0xc, flags | 0x10);
    G3Function80045BA0(node);
    return G3Function80045B74(game + 0x11c18, node);
}

uint32 G3Function80061CA4(uint32 request, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80061ca4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 flags = r_u32(request + 0x24);
    uint32 level = r_u32(game + 0x48);
    uint32 pool = r_u32(game + 0x5c);
    uint32 terrain = r_u32(level);
    if ((flags & 1) != 0)
    {
        uint32 first = r_u32(request + 0x14);
        uint32 second = r_u32(request + 0x18);
        uint32 object = r_u32(request + 0x28);
        uint32 hit = G3Function800622EC(terrain, first, second, object, frame);
        if (hit != 0)
        {
            w_u32(request, 3);
            w_u32(request + 4, hit);
            w_u32(request + 8, 0);
            uint32 normal_index = (uint32)r_s16(hit + 8);
            uint32 normals = r_u32(terrain + 0x40);
            G3Function8005F304(normal_index, normals, request + 0xc, frame);
        }
        else
        {
            w_u32(request, 0);
            w_u32(request + 4, 0);
            w_u32(request + 8, 0);
            w_u16(request + 0xc, 0);
            w_u16(request + 0xe, 0);
            w_u16(request + 0x10, 0);
        }
    }
    flags = r_u32(request + 0x24);
    if ((flags & 8) != 0)
    {
        for (uint32 bucket = 0x40; bucket < 0x80; ++bucket)
        {
            uint32 object = r_u32(pool + (bucket << 3) + 0x10);
            while (object != 0)
            {
                G3Function80061A7C(request, object, frame);
                object = r_u32(object + 4);
            }
        }
        return 0;
    }
    bool partitioned = (flags & 0x10) != 0;
    uint32 passes = partitioned ? 2 : 1;
    for (uint32 pass = 0; pass < passes; ++pass)
    {
        uint32 table;
        uint32 enabled;
        if (partitioned && pass == 0)
        {
            table = 0x8009ee54;
            enabled = flags & 2;
        }
        else
        {
            if (partitioned)
                flags = r_u32(request + 0x24);
            table = partitioned ? 0x8009ee14 : 0x8009ee94;
            enabled = flags & 4;
        }
        if (enabled != 0)
        {
            for (uint32 index = 0; index < 0x10; ++index)
            {
                uint32 bucket = r_u32(table + (index << 2));
                uint32 object = r_u32(pool + (bucket << 3) + 0x10);
                while (object != 0)
                {
                    G3Function80061A7C(request, object, frame);
                    object = r_u32(object + 4);
                }
            }
        }
    }
    return 0;
}

uint32 G3Function8005F304(uint32 index, uint32 normals, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005f304, "SLUS_008.06");
    sint32 selected = (sint16)(index & 0xffff);
    bool negative = selected < 0;
    uint32 magnitude = (uint32)(negative ? -selected : selected);
    uint32 cursor = normals + magnitude * 6;
    uint32 encoded = r_u16(cursor);
    sint32 x = (sint32)(encoded << 18) >> 18;
    if (negative)
        x = -x;
    w_u16(output, (uint32)x);
    uint32 value = r_u16(cursor + 2);
    w_u16(output + 2, negative ? 0 - value : value);
    value = r_u16(cursor + 4);
    w_u16(output + 4, negative ? 0 - value : value);
    return encoded >> 14;
}

uint32 G3Function80061A7C(uint32 request, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80061a7c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 result = r_u32(object + 0x30);
    if (result == 0)
        return result;
    uint32 flags = r_u32(request + 0x24);
    if ((flags & 0x80) == 0)
    {
        uint32 object_flags = r_u32(object + 0x10);
        result = flags & 0x40;
        if ((object_flags & 0x40) != 0)
            return result;
        if (result != 0)
        {
            uint32 descriptor = r_u32(object + 0x1c);
            result = r_u32(descriptor) & 0x200000;
            if (result != 0)
                return result;
        }
    }
    uint32 start = r_u32(request + 0x14);
    uint32 end = r_u32(request + 0x18);
    for (uint32 axis = 0; axis < 2; ++axis)
    {
        uint32 offset = axis << 1;
        sint32 first = r_s16(start + offset);
        sint32 last = r_s16(end + offset);
        uint32 last_word = r_u16(end + offset);
        sint32 maximum = first < last ? (sint16)last_word : first;
        sint32 extent = r_s16(object + 0xc8 + offset);
        uint32 center = r_u32(object + 0xbc + (axis << 2));
        sint32 upper = (sint32)(center + (uint32)extent);
        if (maximum < upper)
            upper = maximum;
        first = r_s16(start + offset);
        sint32 minimum = last < first ? (sint16)last_word : first;
        sint32 lower = (sint32)(center - (uint32)extent);
        if (lower < minimum)
            lower = minimum;
        if (upper < lower)
            return 1;
    }
    sint32 first = r_s16(start + 4);
    sint32 last = r_s16(end + 4);
    bool choose_end = first < last;
    uint32 last_word = r_u16(end + 4);
    sint32 maximum = choose_end ? (sint16)last_word : first;
    sint32 extent = r_s16(object + 0xcc);
    uint32 center = r_u32(object + 0xc4);
    sint32 upper = (sint32)(center + (uint32)extent);
    if (maximum < upper)
        upper = maximum;
    start = r_u32(request + 0x14);
    end = r_u32(request + 0x18);
    first = r_s16(start + 4);
    last = r_s16(end + 4);
    choose_end = last < first;
    last_word = r_u16(end + 4);
    sint32 minimum = choose_end ? (sint16)last_word : first;
    sint32 lower = (sint32)(center - (uint32)extent);
    if (lower < minimum)
        lower = minimum;
    if (upper < lower)
        return 1;
    return G3Function80061118(request, object, frame);
}

uint32 G3Function80063594(uint32 object)
{
    FUNCTION_MARKER(0x80063594, "SLUS_008.06");
    uint32 result = 0x1c003c8;
    if (object != 0)
    {
        uint32 descriptor = r_u32(object + 0x1c);
        uint32 flags = r_u32(descriptor);
        if ((flags & 0x40000000) != 0)
            result = 0x1c003c0;
        flags = r_u32(object + 0x14);
        if ((flags & 0x20) != 0)
            result = 0xfffffff7;
        uint32 player = r_u32(0x8009efc4);
        if (object != player)
            result |= 0x2820;
    }
    return result;
}

uint32 G3Function800622C0(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800622c0, "SLUS_008.06");
    uint32 result = G3Function80063594(object);
    return (result & 0xff3fff3f) | 0x2020;
}

uint32 G3Function800622EC(uint32 terrain, uint32 start, uint32 end, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800622ec, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 flags = G3Function800622C0(object, frame);
    w_u32(frame + 0x10, object);
    w_u32(frame + 0x14, flags);
    w_u32(frame + 0x18, 0);
    return G3Function8006238C(terrain, start, end, 1, frame);
}

uint32 G3Function80062358(uint32 terrain, uint32 start, uint32 end, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80062358, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    w_u32(frame + 0x10, object);
    w_u32(frame + 0x14, 0x1c029e8);
    w_u32(frame + 0x18, 0);
    return G3Function8006238C(terrain, start, end, 1, frame);
}

uint32 G3Function8006238C(uint32 world, uint32 end, uint32 start, uint32 direction, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006238c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    uint32 object = r_u32(guest_stack + 0x10);
    w_u32(frame + 0x60, world);
    w_u32(frame + 0x6c, direction);
    w_u32(frame + 0x28, 0);
    w_u32(0x1f8000a0, 0x1f8000a0);
    world = r_u32(frame + 0x60);
    uint32 root = r_u32(world);
    uint32 cursor = 0x1f8000a4;
    w_u32(cursor, root);
    if (root == cursor)
        return r_u32(frame + 0x28);
    w_u32(frame + 0x2c, 0x1f800060);
    w_u32(frame + 0x30, 0x1f800074);
    do
    {
        uint32 node = r_u32(cursor);
        uint32 node_flags = r_u8(node + 0xc);
        cursor -= 4;
        if ((node_flags & 2) != 0)
        {
            uint32 index = 0;
            uint32 face = r_u32(node + 0x10);
            uint32 count = r_u8(node + 0xd);
            w_u8(node + 0xc, node_flags | 0x40);
            while (index < count)
            {
                bool first_small = false;
                {
                    uint32 face_flags = r_u16(face + 6);
                    uint32 mask = r_u32(frame + 0x74);
                    if (((uint32(1) << (face_flags & 0x1f)) & mask) != 0)
                        goto NextFace;
                    uint32 normal_index = (uint32)r_s16(face + 8);
                    world = r_u32(frame + 0x60);
                    uint32 normal = r_u32(frame + 0x2c);
                    uint32 normals = r_u32(world + 0x40);
                    G3Function8005F304(normal_index, normals, normal, frame);
                    world = r_u32(frame + 0x60);
                    uint32 vertex_index = r_u16(face);
                    uint32 vertices = r_u32(world + 0x30);
                    uint32 vertex = vertices + (vertex_index << 3);
                    for (uint32 point = 0; point < 2; ++point)
                    {
                        uint32 source = point == 0 ? end : start;
                        for (uint32 axis = 0; axis < 3; ++axis)
                        {
                            uint32 offset = axis << 1;
                            uint32 value = r_u16(source + offset);
                            uint32 base = r_u16(vertex + offset);
                            w_u16(0x1f800040 + point * 6 + offset, value - base);
                        }
                    }
                    for (uint32 axis = 0; axis < 3; ++axis)
                    {
                        uint32 value = r_u16(vertex + (axis << 1));
                        w_u16(0x1f80004c + (axis << 1), value);
                    }
                    SetRotMatrix((MATRIX *)psx_addr(0x1f800040, 20));
                    normal = r_u32(frame + 0x2c);
                    xport_gte_write_data(0, r_u32(normal));
                    xport_gte_write_data(1, r_u32(normal + 4));
                    xport_gte_execute(0x486012);
                    w_u32(0x1f800068, xport_gte_read_data(25));
                    w_u32(0x1f80006c, xport_gte_read_data(26));
                    w_u32(0x1f800070, xport_gte_read_data(27));
                    uint32 current_direction = r_u32(frame + 0x6c);
                    if (current_direction != 0)
                    {
                        sint32 first_distance = r_s32(0x1f800068);
                        if (first_distance > 0)
                        {
                            first_small = first_distance < 6;
                            goto ForwardTolerance;
                        }
                        if (r_s32(0x1f80006c) < 0)
                            goto RecheckForward;
                    }
                    else
                    {
                        if (r_s32(0x1f800068) < 0)
                            goto RecheckDirection;
                        if (r_s32(0x1f80006c) > 0)
                            goto RecheckDirection;
                    }
                    uint32 point_output = r_u32(frame + 0x30);
                    uint32 plane_constant = r_u32(0x1f800070);
                    normal = r_u32(frame + 0x2c);
                    w_u32(frame + 0x10, plane_constant);
                    if (G3Function8005F518(point_output, start, end, normal, frame) == 0)
                        goto RecheckDirection;
                    uint32 second_index = r_u16(face + 2);
                    world = r_u32(frame + 0x60);
                    uint32 third_index = r_u16(face + 4);
                    point_output = r_u32(frame + 0x30);
                    normal = r_u32(frame + 0x2c);
                    vertices = r_u32(world + 0x30);
                    w_u32(frame + 0x10, normal);
                    if (G3Function8005F164(vertex, vertices + (second_index << 3), vertices + (third_index << 3), point_output, frame) == 0)
                        goto NextFace;
                    face_flags = r_u16(face + 6);
                    if ((face_flags & 0xe) == 6 && object != 0)
                    {
                        uint32 game = r_u32(0x80097048);
                        uint32 level = r_u32(game + 0x48);
                        uint32 materials = r_u32(level + 0xf0);
                        uint32 material = r_u32(materials + ((face_flags & 0x1e0) >> 1));
                        uint32 flags = r_u32(object + 0x10);
                        flags = (material & 0x10) != 0 ? flags | 0x40000000 : flags & 0xbfffffff;
                        w_u32(object + 0x10, flags);
                        uint32 player = r_u32(0x8009efc4);
                        bool is_player = object == player;
                        bool save_contact = !is_player;
                        if (is_player)
                        {
                            uint32 x = r_u16(0x1f800074);
                            sint32 start_x = r_s16(start);
                            uint32 y = r_u16(0x1f800076);
                            uint32 dx = (uint32)(sint32)(sint16)x - (uint32)start_x;
                            w_u32(frame + 0x18, dx);
                            sint32 start_y = r_s16(start + 2);
                            uint32 z = r_u16(0x1f800078);
                            uint32 dy = (uint32)(sint32)(sint16)y - (uint32)start_y;
                            w_u32(frame + 0x1c, dy);
                            sint32 start_z = r_s16(start + 4);
                            uint32 dz = (uint32)(sint32)(sint16)z - (uint32)start_z;
                            w_u32(frame + 0x20, dz);
                            uint32 mode = r_u32(object + 0x150);
                            uint32 squared = dx * dx + dy * dy + dz * dz;
                            save_contact = mode == 2 || (sint32)squared <= 0xffff;
                        }
                        if (save_contact)
                        {
                            w_u32(object + 0xf8, face);
                            uint32 contact_words[2];
                            for (uint32 word = 0; word < 2; ++word)
                            {
                                uint32 source = 0x1f800074 + (word << 2);
                                uint32 high_address = source + 3;
                                uint32 high_lane = high_address & 3;
                                uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                                uint32 low_lane = source & 3;
                                uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
                                contact_words[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
                            }
                            for (uint32 word = 0; word < 2; ++word)
                            {
                                uint32 destination = object + 0xfc + (word << 2);
                                uint32 high_address = destination + 3;
                                uint32 high_lane = high_address & 3;
                                w_masked_u32(high_address & 0xfffffffc, contact_words[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                                uint32 low_lane = destination & 3;
                                w_masked_u32(destination & 0xfffffffc, contact_words[word] << (low_lane * 8), (15 << low_lane) & 15);
                            }
                        }
                        if (is_player)
                        {
                            face_flags = r_u16(face + 6);
                            if ((face_flags & 1) == 0)
                            {
                                game = r_u32(0x80097048);
                                level = r_u32(game + 0x48);
                                materials = r_u32(level + 0xf0);
                                material = r_u32(materials + ((face_flags & 0x1e0) >> 1));
                                if ((material & 1) != 0)
                                {
                                    uint32 value = r_u32(0x1f800074);
                                    w_u32(end, value);
                                    value = r_u32(0x1f800078);
                                    w_u32(frame + 0x28, face);
                                    w_u32(end + 4, value);
                                }
                            }
                        }
                        goto NextFace;
                    }
                    face_flags = r_u16(face + 6);
                    if ((face_flags & 1) != 0 && (face_flags & 0xe) == 0xa && object != 0)
                    {
                        sint32 event_index = r_s16(face + 0xa);
                        if (event_index >= 0)
                        {
                            sint32 height = r_s16(object + 0x5c);
                            uint32 hit_z = r_u16(0x1f800078);
                            if (height - 8 < (sint16)hit_z)
                            {
                                world = r_u32(frame + 0x60);
                                uint32 sequences = r_u32(world + 0x3c);
                                uint32 sequence = r_u32(sequences + ((uint32)event_index << 2));
                                G3Function80018DE0(object, sequence + 4, frame);
                            }
                            goto NextFace;
                        }
                    }
                    {
                        uint32 value = r_u32(0x1f800074);
                        w_u32(frame + 0x28, face);
                        w_u32(end, value);
                        uint32 saved_face = r_u32(frame + 0x28);
                        value = r_u32(0x1f800078);
                        w_u32(end + 4, value);
                        face = saved_face;
                    }
                    goto SkipGroup;
                }
RecheckDirection:
                if (r_u32(frame + 0x6c) == 0)
                    goto ReverseTolerance;
RecheckForward:
                first_small = r_s32(0x1f800068) < 6;
ForwardTolerance:
                if (!first_small)
                    goto SkipGroup;
                if (r_s32(0x1f80006c) < -5)
                    goto SkipGroup;
                if (r_u32(frame + 0x6c) != 0)
                    goto NextFace;
ReverseTolerance:
                if (r_s32(0x1f800068) < -5)
                    goto SkipGroup;
                if (r_s32(0x1f80006c) < 6)
                    goto NextFace;
SkipGroup:
                face += 0xc;
                while (index < count)
                {
                    if ((r_s16(face + 6) & 0x8000) == 0)
                        break;
                    ++index;
                    face += 0xc;
                }
                face -= 0xc;
NextFace:
                ++index;
                face += 0xc;
            }
        }
        else
        {
            uint32 children[2];
            children[0] = r_u32(node + 0x10);
            sint32 first_end_x = r_s16(end);
            sint32 first_high_x = r_s16(children[0] + 6);
            children[1] = r_u32(node + 0x14);
            for (uint32 child = 0; child < 2; ++child)
            {
                uint32 bounds = children[child];
                bool intersects = true;
                for (uint32 axis = 0; axis < 3; ++axis)
                {
                    uint32 offset = axis << 1;
                    sint32 endpoint = child == 0 && axis == 0 ? first_end_x : r_s16(end + offset);
                    sint32 high = child == 0 && axis == 0 ? first_high_x : r_s16(bounds + 6 + offset);
                    if (high < endpoint)
                    {
                        sint32 other = r_s16(start + offset);
                        if (high < other)
                        {
                            intersects = false;
                            break;
                        }
                    }
                    sint32 low = r_s16(bounds + offset);
                    if (endpoint < low)
                    {
                        sint32 other = r_s16(start + offset);
                        if (other < low)
                        {
                            intersects = false;
                            break;
                        }
                    }
                }
                if (intersects)
                {
                    cursor += 4;
                    w_u32(cursor, bounds);
                }
            }
        }
    } while (r_u32(cursor) != cursor);
    return r_u32(frame + 0x28);
}

uint32 G3Function8005F008(uint32 triangle, sint32 x, sint32 y)
{
    FUNCTION_MARKER(0x8005f008, "SLUS_008.06");
    x = (sint16)x;
    y = (sint16)y;
    uint32 previous = triangle + 8;
    uint32 current = triangle;
    bool inside = false;
    bool crossing_seen = false;
    bool previous_above = r_s16(previous + 2) >= y;
    for (uint32 remaining = 3; remaining != 0; --remaining)
    {
        sint32 current_y = r_s16(current + 2);
        bool current_above = current_y >= y;
        if (previous_above != current_above)
        {
            sint32 previous_x = r_s16(previous);
            sint32 current_x = r_s16(current);
            bool previous_right = previous_x >= x;
            if (previous_right == (current_x >= x))
            {
                if (x == previous_x && x == current_x)
                    return 1;
                if (previous_right)
                    inside = !inside;
            }
            else
            {
                sint32 previous_y = r_s16(previous + 2);
                uint32 dy = (uint32)(previous_y - current_y);
                uint32 first_product = (uint32)(current_x - x) * dy;
                uint32 second_product = (uint32)(current_y - y) * (uint32)(previous_x - current_x);
                uint32 determinant = first_product - second_product;
                if (determinant == 0)
                    return 1;
                if ((sint32)(determinant ^ dy) >= 0)
                    inside = !inside;
            }
            if (crossing_seen)
                return inside;
            crossing_seen = true;
        }
        else if (previous_above)
        {
            sint32 previous_y = r_s16(previous + 2);
            if (y == previous_y)
            {
                if (y == current_y)
                {
                    sint32 previous_x = r_s16(previous);
                    sint32 current_x = r_s16(current);
                    if ((previous_x < x) != (current_x < x))
                        return 1;
                    if (x == previous_x || x == current_x)
                        return 1;
                }
                else
                {
                    sint32 previous_x = r_s16(previous);
                    if (x == previous_x)
                        return 1;
                }
            }
        }
        previous_above = current_above;
        previous = current;
        current += 4;
    }
    return inside;
}

uint32 G3Function8005F164(uint32 first, uint32 second, uint32 third, uint32 point, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005f164, "SLUS_008.06");
    uint32 normal = r_u32(guest_stack + 0x10);
    sint32 nx = r_s16(normal);
    sint32 ny = r_s16(normal + 2);
    if (nx < 0)
        nx = -nx;
    if (ny < 0)
        ny = -ny;
    sint32 nz = r_s16(normal + 4);
    if (nz < 0)
        nz = -nz;
    uint32 first_axis = 0;
    uint32 second_axis = 2;
    if (ny < nx)
    {
        if (nz < nx)
        {
            first_axis = 2;
            second_axis = 4;
        }
    }
    else if (nz < ny)
        second_axis = 4;
    w_u16(0x1f800000, r_u16(first + first_axis));
    w_u16(0x1f800002, r_u16(first + second_axis));
    w_u16(0x1f800004, r_u16(second + first_axis));
    w_u16(0x1f800006, r_u16(second + second_axis));
    w_u16(0x1f800008, r_u16(third + first_axis));
    w_u16(0x1f80000a, r_u16(third + second_axis));
    uint32 x = r_u16(point + first_axis);
    uint32 y = r_u16(point + second_axis);
    return G3Function8005F008(0x1f800000, (sint16)x, (sint16)y);
}

uint32 G3Function8005F518(uint32 output, uint32 start, uint32 end, uint32 normal, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005f518, "SLUS_008.06");
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 offset = axis << 1;
        uint32 first = r_u16(start + offset);
        w_u16(0x1f800000 + offset, first);
        uint32 last = r_u16(end + offset);
        w_u16(0x1f800006 + offset, last - first);
    }
    SetRotMatrix((MATRIX *)psx_addr(0x1f800000, 20));
    xport_gte_write_data(0, r_u32(normal));
    xport_gte_write_data(1, r_u32(normal + 4));
    xport_gte_execute(0x486012);
    w_u32(0x1f80000c, xport_gte_read_data(25));
    w_u32(0x1f800010, xport_gte_read_data(26));
    w_u32(0x1f800014, xport_gte_read_data(27));
    sint32 denominator = r_s32(0x1f800010);
    if (denominator == 0)
        return 0;
    uint32 first_distance = r_u32(0x1f80000c);
    uint32 plane = r_u32(guest_stack + 0x10);
    uint32 numerator_word = plane - first_distance;
    sint32 numerator = (sint32)numerator_word;
    if (denominator > 0)
    {
        if (numerator < 0 || denominator < numerator)
            return 0;
    }
    else if (numerator > 0 || numerator < denominator)
        return 0;
    sint32 shifted = (sint32)(numerator_word << 16);
    sint32 quotient;
    if (shifted == (sint32)0x80000000 && denominator == -1)
        quotient = shifted;
    else
        quotient = shifted / denominator;
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        uint32 offset = axis << 1;
        uint32 delta_word = r_u16(0x1f800006 + offset);
        sint32 delta = (sint16)delta_word;
        uint32 product = (uint32)delta * (uint32)quotient;
        uint32 first = r_u16(start + offset);
        sint32 displacement = (sint32)product >> 16;
        w_u16(output + offset, first + (uint32)displacement);
    }
    return 1;
}

uint32 G3Function80061118(uint32 request, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80061118, "SLUS_008.06");
    uint32 frame = guest_stack - 0x70;
    w_u32(frame + 0x34, 0);
    uint32 descriptor = r_u32(object + 0x1c);
    sint32 model_index = r_s16(object + 0xa2);
    uint32 flags = r_u32(object + 0x14);
    uint32 models = r_u32(descriptor + 0xc);
    uint32 model = r_u32(models + ((uint32)model_index << 2));
    flags &= 0x20000000;
    w_u32(frame + 0x30, model);
    if (flags != 0)
        w_u32(frame + 0x34, r_u32(object + 0x144));
    w_u32(frame + 0x38, 0);
    if (flags != 0)
        w_u32(frame + 0x38, r_u32(object + 0x148));
    w_u32(frame + 0x3c, 0);
    if (flags != 0)
        w_u32(frame + 0x3c, r_u32(object + 0x14c));
    model = r_u32(frame + 0x30);
    uint32 matrix = r_u32(object + 0x30);
    sint32 segment_count = r_s16(model + 6);
    uint32 segment = r_u32(model + 0x18);
    uint32 segment_end = segment + (uint32)segment_count * 24;
    w_u32(frame + 0x28, segment_end);
    if (segment >= segment_end)
        return 0;
    w_u32(frame + 0x40, 0x1f8000c0);
    do
    {
        uint32 collision = r_u32(segment + 0x14);
        if (collision != 0)
        {
            G3Function800313E0(0x1f8000a0, matrix, frame);
            w_u32(0x1f8000d4, r_u32(matrix + 0x14));
            w_u32(0x1f8000d8, r_u32(matrix + 0x18));
            w_u32(0x1f8000dc, r_u32(matrix + 0x1c));
            TransposeMatrix((MATRIX *)psx_addr(0x1f8000a0, 18), (MATRIX *)psx_addr(0x1f8000c0, 18));
            SetRotMatrix((MATRIX *)psx_addr(0x1f8000a0, 20));
            SetTransMatrix((MATRIX *)psx_addr(0x1f8000a0, 32));
            for (uint32 endpoint = 0; endpoint < 2; ++endpoint)
            {
                uint32 input = r_u32(request + 0x14 + (endpoint << 2));
                xport_gte_write_data(0, r_u32(input));
                xport_gte_write_data(1, r_u32(input + 4));
                xport_gte_execute(0x480012);
                uint32 output = endpoint == 0 ? 0x1f800110 : 0x1f8000e0;
                w_u32(output, xport_gte_read_data(25));
                w_u32(output + 4, xport_gte_read_data(26));
                w_u32(output + 8, xport_gte_read_data(27));
                w_u32(0x1f800134, xport_gte_read_control(31));
            }
            uint32 start_x = r_u16(0x1f8000e0);
            uint32 start_y = r_u16(0x1f8000e4);
            uint32 start_z = r_u16(0x1f8000e8);
            uint32 end_x = r_u16(0x1f800110);
            uint32 end_y = r_u16(0x1f800114);
            uint32 end_z = r_u16(0x1f800118);
            w_u16(0x1f80011c, start_x);
            w_u16(0x1f80011e, start_y);
            w_u16(0x1f800120, start_z);
            w_u16(0x1f800124, end_x);
            w_u16(0x1f800126, end_y);
            w_u16(0x1f800128, end_z);
            uint32 sphere_count = r_u32(collision + 8);
            if (sphere_count != 0 && (r_u32(frame + 0x34) & 1) == 0 && (r_u32(request + 0x24) & 0x20) == 0)
            {
                uint32 sphere = r_u32(collision + 0xc);
                uint32 sphere_limit = sphere + (sphere_count << 4);
                w_u32(frame + 0x2c, sphere_limit);
                uint32 radius_address = sphere + 0xe;
                while (sphere < sphere_limit)
                {
                    if ((r_u32(sphere) & 0x2000) != 0)
                    {
                        uint32 type = r_u8(radius_address - 9);
                        if (type == 9 || type - 3 < 3 || type < 3)
                        {
                            uint32 game = r_u32(0x80097048);
                            uint32 target = r_u32(game + 0x65ec);
                            G3DispatchNearestPoint(target, 0x1f80012c, 0x1f80011c, 0x1f800124, sphere + 8, frame);
                            sint32 point_x = r_s16(0x1f80012c);
                            sint32 center_x = r_s16(radius_address - 6);
                            uint32 dx = (uint32)(point_x - center_x);
                            uint32 square_x = dx * dx;
                            w_u32(0x1f800104, dx);
                            sint32 point_y = r_s16(0x1f80012e);
                            sint32 center_y = r_s16(radius_address - 4);
                            uint32 dy = (uint32)(point_y - center_y);
                            uint32 square_y = dy * dy;
                            w_u32(0x1f800108, dy);
                            sint32 center_z = r_s16(radius_address - 2);
                            sint32 point_z = r_s16(0x1f800130);
                            uint32 dz = (uint32)(point_z - center_z);
                            uint32 square_z = dz * dz;
                            w_u32(0x1f80010c, dz);
                            uint32 radius = r_u16(radius_address);
                            if (square_x + square_y + square_z < radius * radius)
                            {
                                point_x = r_s16(0x1f800124);
                                center_x = r_s16(radius_address - 6);
                                w_u32(0x1f800104, (uint32)(point_x - center_x));
                                point_y = r_s16(0x1f800126);
                                sint32 ax = r_s32(0x1f800104);
                                center_y = r_s16(radius_address - 4);
                                if (ax < 0)
                                    ax = -ax;
                                w_u32(0x1f800108, (uint32)(point_y - center_y));
                                point_z = r_s16(0x1f800128);
                                sint32 ay = r_s32(0x1f800108);
                                center_z = r_s16(radius_address - 2);
                                if (ay < 0)
                                    ay = -ay;
                                sint32 az = point_z - center_z;
                                sint32 signed_z = az;
                                if (az < 0)
                                    az = -az;
                                w_u32(0x1f80010c, (uint32)signed_z);
                                if (ax > ay)
                                {
                                    sint32 temporary = ax;
                                    ax = ay;
                                    ay = temporary;
                                }
                                if (ay > az)
                                {
                                    sint32 temporary = ay;
                                    ay = az;
                                    az = temporary;
                                }
                                if (ax > ay)
                                {
                                    sint32 temporary = ax;
                                    ax = ay;
                                    ay = temporary;
                                }
                                uint32 weighted = (uint32)az * 30 + (uint32)ay * 12 + (uint32)ax * 9;
                                sint32 distance = (sint32)weighted >> 5;
                                if (distance != 0)
                                {
                                    sint32 normal_x = (sint32)(r_u32(0x1f800104) << 12) / distance;
                                    sint32 normal_y = (sint32)(r_u32(0x1f800108) << 12) / distance;
                                    sint32 normal_z = (sint32)(r_u32(0x1f80010c) << 12) / distance;
                                    w_u32(0x1f8000ec, (uint32)normal_x);
                                    w_u32(0x1f8000f0, (uint32)normal_y);
                                    w_u32(0x1f8000f4, (uint32)normal_z);
                                    uint32 scaled[3];
                                    for (uint32 axis = 0; axis < 3; ++axis)
                                    {
                                        radius = r_u16(radius_address);
                                        uint32 difference = r_u32(0x1f800104 + (axis << 2));
                                        uint32 product = difference * (radius - (uint32)distance);
                                        w_u32(0x1f800104 + (axis << 2), product);
                                        scaled[axis] = (uint32)((sint32)(product << 5) / (sint32)weighted);
                                    }
                                    uint32 x = r_u16(0x1f800124);
                                    w_u32(0x1f800104, scaled[0]);
                                    uint32 delta_x = r_u16(0x1f800104);
                                    w_u16(0x1f800124, x + delta_x);
                                    uint32 y = r_u16(0x1f800126);
                                    uint32 z = r_u16(0x1f800128);
                                    w_u32(0x1f800108, scaled[1]);
                                    w_u32(0x1f80010c, scaled[2]);
                                    uint32 delta_y = r_u16(0x1f800108);
                                    uint32 delta_z = r_u16(0x1f80010c);
                                    w_u16(0x1f800126, y + delta_y);
                                    w_u16(0x1f800128, z + delta_z);
                                    uint32 rotation = r_u32(frame + 0x40);
                                    SetRotMatrix((MATRIX *)psx_addr(rotation, 20));
                                    SetTransMatrix((MATRIX *)psx_addr(matrix, 32));
                                    xport_gte_write_data(0, r_u32(0x1f800124));
                                    xport_gte_write_data(1, r_u32(0x1f800128));
                                    xport_gte_execute(0x480012);
                                    w_u32(0x1f800110, xport_gte_read_data(25));
                                    w_u32(0x1f800114, xport_gte_read_data(26));
                                    w_u32(0x1f800118, xport_gte_read_data(27));
                                    w_u32(0x1f800134, xport_gte_read_control(31));
                                    rotation = r_u32(frame + 0x40);
                                    VECTOR normal;
                                    ApplyMatrixLV((MATRIX *)psx_addr(rotation, 18), (VECTOR *)psx_addr(0x1f8000ec, 12), &normal);
                                    w_u32(0x1f8000f8, (uint32)normal.vx);
                                    w_u32(0x1f8000fc, (uint32)normal.vy);
                                    w_u32(0x1f800100, (uint32)normal.vz);
                                    for (uint32 axis = 0; axis < 3; ++axis)
                                    {
                                        uint32 output = r_u32(request + 0x14);
                                        uint32 value = r_u16(0x1f800110 + (axis << 2));
                                        w_u16(output + (axis << 1), value);
                                    }
                                    w_u16(request + 0xc, r_u16(0x1f8000f8));
                                    w_u16(request + 0xe, r_u16(0x1f8000fc));
                                    uint32 last_normal = r_u16(0x1f800100);
                                    w_u32(request, 1);
                                    w_u32(request + 4, sphere);
                                    w_u32(request + 8, object);
                                    w_u16(request + 0x10, last_normal);
                                }
                            }
                        }
                    }
                    sphere_limit = r_u32(frame + 0x2c);
                    sphere += 0x10;
                    radius_address += 0x10;
                }
            }
            if (r_u32(collision) != 0 && (r_u32(frame + 0x38) & 1) == 0)
            {
                uint32 current_model = r_u32(frame + 0x30);
                w_u32(frame + 0x10, frame + 0x18);
                uint32 hit = G3Function80061EE0(0x1f800124, 0x1f80011c, collision, current_model, frame);
                if (hit != 0)
                {
                    ApplyMatrixSV((MATRIX *)psx_addr(0x1f8000c0, 20), (SVECTOR *)psx_addr(frame + 0x18, 8), (SVECTOR *)psx_addr(request + 0xc, 6));
                    w_u32(request, 2);
                    w_u32(request + 4, hit);
                    w_u32(request + 8, object);
                }
            }
            uint32 box_count = r_u32(collision + 0x10);
            if (box_count != 0 && (r_u32(frame + 0x3c) & 1) == 0)
            {
                uint32 box = r_u32(collision + 0x14);
                uint32 limit = box + box_count * 28;
                uint32 bounds = box + 8;
                while (box < limit)
                {
                    sint32 component = r_s16(bounds + 0xe);
                    sint32 scale = r_s16(object + 0x68);
                    sint32 product = component * scale;
                    component = r_s16(bounds + 0x10);
                    w_u16(bounds + 2, (uint32)(product >> 12));
                    scale = r_s16(object + 0x6a);
                    product = component * scale;
                    component = r_s16(bounds + 0x12);
                    w_u16(bounds + 4, (uint32)(product >> 12));
                    scale = r_s16(object + 0x6c);
                    product = component * scale;
                    component = r_s16(bounds + 8);
                    w_u16(bounds + 6, (uint32)(product >> 12));
                    scale = r_s16(object + 0x68);
                    product = component * scale;
                    component = r_s16(bounds + 0xa);
                    w_u16(bounds - 4, (uint32)(product >> 12));
                    scale = r_s16(object + 0x6a);
                    product = component * scale;
                    component = r_s16(bounds + 0xc);
                    w_u16(bounds - 2, (uint32)(product >> 12));
                    scale = r_s16(object + 0x6c);
                    product = component * scale;
                    box += 0x1c;
                    w_u16(bounds, (uint32)(product >> 12));
                    bounds += 0x1c;
                }
                uint32 hit = G3Function8006218C(0x1f800124, 0x1f80011c, collision, frame + 0x20, frame);
                if (hit != 0)
                {
                    ApplyMatrixSV((MATRIX *)psx_addr(0x1f8000c0, 20), (SVECTOR *)psx_addr(frame + 0x20, 8), (SVECTOR *)psx_addr(request + 0xc, 6));
                    w_u32(request, 5);
                    w_u32(request + 4, hit);
                    w_u32(request + 8, object);
                }
            }
            bool changed = false;
            for (uint32 axis = 0; axis < 3; ++axis)
            {
                sint32 local_coordinate = r_s16(0x1f800124 + (axis << 1));
                sint32 original_coordinate = r_s32(0x1f800110 + (axis << 2));
                if (local_coordinate != original_coordinate)
                {
                    changed = true;
                    break;
                }
            }
            if (changed)
            {
                w_u16(request + 0x1c, r_u16(0x1f800124));
                w_u16(request + 0x1e, r_u16(0x1f800126));
                w_u16(request + 0x20, r_u16(0x1f800128));
                uint32 rotation = r_u32(frame + 0x40);
                SetRotMatrix((MATRIX *)psx_addr(rotation, 20));
                SetTransMatrix((MATRIX *)psx_addr(matrix, 32));
                xport_gte_write_data(0, r_u32(0x1f800124));
                xport_gte_write_data(1, r_u32(0x1f800128));
                xport_gte_execute(0x480012);
                w_u32(0x1f800110, xport_gte_read_data(25));
                w_u32(0x1f800114, xport_gte_read_data(26));
                w_u32(0x1f800118, xport_gte_read_data(27));
                w_u32(0x1f800134, xport_gte_read_control(31));
                for (uint32 axis = 0; axis < 3; ++axis)
                {
                    uint32 output = r_u32(request + 0x14);
                    uint32 value = r_u16(0x1f800110 + (axis << 2));
                    w_u16(output + (axis << 1), value);
                }
            }
        }
        w_u32(frame + 0x34, (uint32)(r_s32(frame + 0x34) >> 1));
        w_u32(frame + 0x38, (uint32)(r_s32(frame + 0x38) >> 1));
        sint32 final_mask = r_s32(frame + 0x3c);
        segment += 0x18;
        w_u32(frame + 0x3c, (uint32)(final_mask >> 1));
        uint32 limit = r_u32(frame + 0x28);
        matrix += 0x20;
        if (segment >= limit)
            break;
    } while (true);
    return 0;
}
