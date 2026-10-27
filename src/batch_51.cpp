#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void G3MstrdbgRunSequence0D6C(uint32 object, uint32 model, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000d6c, "G3L_39");
    uint32 frame = guest_stack - 0x18;
    uint32 resource = r_u32(model + 0x18);
    uint32 sequence = r_u32(resource + 8);
    if (sequence != 0) G3Function80018DE0(object, sequence + 4, frame);
}

void G3MstrdbgRunProperty0DA4(uint32 object, uint32 name, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000da4, "G3L_39");
    uint32 frame = guest_stack - 0x18;
    uint32 resource = r_u32(object + 0x28);
    uint32 property = G3MstrdbgFindExtra0AEC(resource, name, module_base, frame);
    if (property != 0) G3Function80018DE0(object, r_u32(property) + 4, frame);
}

uint32 G3MstrdbgBuildContext0DE8(uint32 context, uint32 object, uint32 target)
{
    FUNCTION_MARKER(0x00000de8, "G3L_39");
    uint32 player = r_u32(0x8009efc4);
    uint32 state = object + 0x104;
    w_u32(state + 0x1c, context);
    {
        uint32 source = target;
        uint32 destination = context;
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
    uint32 angle = r_u16(player + 0x64);
    uint32 output = r_u32(state + 0x1c);
    w_u16(output + 6, angle - 0x400);
    output = r_u32(state + 0x1c);
    {
        uint32 source = object + 0x58;
        uint32 destination = output + 8;
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
    output = r_u32(state + 0x1c);
    {
        uint32 source = object + 0x88;
        uint32 destination = output + 0xe;
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
    angle = r_u16(object + 0x64);
    output = r_u32(state + 0x1c);
    uint32 result = angle - 0x400;
    w_u16(output + 0x2e, result);
    return result;
}

uint32 G3MstrdbgNeedsDecision0E6C(uint32 state, uint32 settings, uint32 configuration, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000e6c, "G3L_39");
    uint32 frame = guest_stack - 0x38;
    uint32 context = r_u32(state + 0x1c);
    uint32 parameters = context + 0x14;
    if ((r_u32(state + 0x18) & 6) != 0)
    {
        sint32 command = r_s8(state + 2);
        if (command != 0xc && command != 0x14)
            return 1;
    }
    if (settings != 0)
    {
        uint32 alternate = r_u32(settings + 0x18);
        if (alternate != 0 && (r_u32(alternate + 0xc) & 1) != 0)
        {
            if (G3MstrdbgCheckAlternate6404(state, settings, frame) != 0)
                return 1;
        }
    }
    if ((r_u32(state + 0x18) & 0x200000) != 0)
        return 0;
    if (r_s8(state) == 0)
    {
        w_u32(frame + 0x10, 0x46);
        if (G3MstrdbgShouldWake64C8(parameters, configuration, state, settings, module_base, frame) != 0)
            return 1;
    }
    context = r_u32(state + 0x1c);
    w_u16(frame + 0x18, r_u16(context + 0x16));
    context = r_u32(state + 0x1c);
    w_u16(frame + 0x1a, r_u16(context + 0x1e));
    context = r_u32(state + 0x1c);
    w_u16(frame + 0x1c, r_u16(context + 0x20));
    if (r_s8(state) != 1)
        return 0;
    uint32 mode = r_u8(state + 1);
    if (mode - 6 < 2)
    {
        w_u32(frame + 0x10, 1);
        uint32 current_mode = (uint32)r_s8(state + 1);
        w_u32(frame + 0x14, current_mode);
        return G3MstrdbgFindTriggered8094(0, frame + 0x18, settings, state, module_base, frame) != 0 ? 1 : 0;
    }
    if ((sint8)mode != 8)
        return 0;
    sint32 index = r_s8(state + 5);
    uint32 table = r_u32(settings + 8);
    uint32 sequence = r_u32(table + (uint32)index * 4);
    uint32 trigger = r_u32(sequence);
    if ((r_u16(trigger + 2) & 8) != 0)
        return 0;
    w_u32(frame + 0x10, 8);
    uint32 current_mode = (uint32)r_s8(state + 1);
    w_u32(frame + 0x14, current_mode);
    return G3MstrdbgFindTriggered8094(0, frame + 0x18, settings, state, module_base, frame) != 0 ? 1 : 0;
}

void G3MstrdbgUpdateHeading106C(uint32 state, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000106c, "G3L_39");
    uint32 frame = guest_stack - 0x30;
    uint32 context = r_u32(state + 0x1c);
    sint32 mode = r_s8(state + 1);
    sint32 previous = r_s8(state + 0x48);
    if (mode == 5)
    {
        uint32 source = context;
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
    if (r_s8(state + 1) == 4)
    {
        uint32 current = r_u32(state + 0x1c);
    {
        uint32 source = current + 0xe;
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
    uint32 current_mode = r_u8(state + 1);
    if (current_mode - 4 < 2)
    {
        uint32 source = (uint32)r_s8(state + 0x3e);
        w_u32(frame + 0x10, current_mode == 5 ? 0 : 1);
        uint32 result = G3MstrdbgCopyVector7E50(output, source);
        w_u8(state + 0x48, result);
        if (r_s8(state + 0x48) != 0 && previous == 0)
        {
            uint32 flags = r_u32(state + 0x18);
            w_u32(state + 0x18, flags | 0x200);
        }
    }
}

uint32 G3MstrdbgCheckRange1168(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001168, "G3L_39");
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

uint32 G3MstrdbgUpdateDormancy12F8(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000012f8, "G3L_39");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    uint32 result = G3MstrdbgCheckRange1168(object, frame);
    uint32 flags = r_u32(state + 0x18);
    if ((flags & 0x80) != 0)
    {
        if (result != 0) goto diagnostic;
        G3MstrdbgSelectList1604(object, frame);
        w_u32(state + 0x18, r_u32(state + 0x18) & 0xffffff7f);
    }
    else if (result == 1)
    {
        G3MstrdbgEnterListZero1648(object, frame);
        w_u32(state + 0x18, r_u32(state + 0x18) | 0x80);
    }
    if ((r_u32(state + 0x18) & 0x80) == 0) return result;
diagnostic:
    if ((r_u32(r_u32(0x80097048) + 0x69b8) & 1) != 0)
    {
        uint32 descriptor = r_u32(object + 0x1c);
        G3MstrdbgBrainStringDiagnostic(module_base + 0x83f0, r_u32(descriptor + 0x24), frame);
    }
    return result;
}

uint32 G3MstrdbgPackCoordinates13D4(uint32 position, uint32 output)
{
    FUNCTION_MARKER(0x000013d4, "G3L_39");
    uint32 x = r_u16(position);
    w_u8(output, x >> 4);
    uint32 y = r_u16(position + 2);
    w_u8(output + 1, y >> 4);
    return y >> 4;
}

uint32 G3MstrdbgApproximateDistance13F8(uint32 position, uint32 packed, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000013f8, "G3L_39");
    uint32 frame = guest_stack - 0x20;
    G3MstrdbgPackCoordinates13D4(position, frame + 0x10);
    uint32 packed_x = r_u8(packed);
    uint32 x = r_u8(frame + 0x10);
    uint32 y = r_u8(frame + 0x11);
    sint32 dx = (sint32)packed_x - (sint32)x;
    uint32 packed_y = r_u8(packed + 1);
    if (dx < 0)
        dx = -dx;
    sint32 dy = (sint32)packed_y - (sint32)y;
    if (dy < 0)
        dy = -dy;
    if (dy < dx)
    {
        sint32 previous_x = dx;
        dx = dy;
        dy = previous_x;
    }
    return (uint32)((dy * 30 + dx * 12) >> 1);
}

void G3MstrdbgUpdateMovementWatch1488(uint32 state, uint32 position, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001488, "G3L_39");
    uint32 frame = guest_stack - 0x20;
    uint32 countdown = r_u8(state + 0x49);
    if (countdown != 0)
        w_u8(state + 0x49, countdown - 1);
    if ((r_u32(state + 0x18) & 0x10) != 0 && r_u8(state + 0x49) == 0)
    {
        uint32 stalled = r_u8(state + 0x4a);
        if (stalled < 0xff)
            w_u8(state + 0x4a, stalled + 1);
    }
    uint32 distance = G3MstrdbgApproximateDistance13F8(position, state + 0x12, frame);
    if ((sint32)distance >= 0xa1)
    {
        G3MstrdbgPackCoordinates13D4(position, state + 0x12);
        w_u8(state + 0x4a, 0);
    }
    if (r_u8(state + 0x4a) >= 0x10)
    {
        uint32 flags = r_u32(state + 0x18);
        w_u8(state + 0x4a, 0);
        w_u8(state + 0x49, 0x3c);
        w_u32(state + 0x18, flags | 8);
    }
    uint32 flags = r_u32(state + 0x18);
    w_u32(state + 0x18, flags & 0xffffffef);
}

uint32 G3MstrdbgMoveList156C(uint32 object, uint32 index, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000156c, "G3L_39");
    G3Function80045BA0(object);
    uint32 game = r_u32(0x80097048);
    uint32 lists = r_u32(game + 0x5c);
    return G3Function80045B74(lists + index * 8 + 0xc, object);
}

uint32 G3MstrdbgListKind15BC(uint32 object)
{
    FUNCTION_MARKER(0x000015bc, "G3L_39");
    uint32 descriptor = r_u32(object + 0x24);
    uint32 data = r_u32(descriptor);
    uint32 kind = r_u32(data);
    if (kind == 1)
        return 0x4c;
    uint32 flags = r_u32(object + 0x11c);
    return (flags & 0x2000) != 0 ? 0x5f : 0x5e;
}

uint32 G3MstrdbgSelectList1604(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001604, "G3L_39");
    uint32 frame = guest_stack - 0x18;
    uint32 flags = r_u32(object + 0x11c);
    if ((flags & 0x4000000) != 0)
        return G3MstrdbgEnterSecondary16B8(object, frame);
    return G3MstrdbgEnterPrimary1668(object, frame);
}

uint32 G3MstrdbgEnterListZero1648(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001648, "G3L_39");
    uint32 frame = guest_stack - 0x18;
    return G3MstrdbgMoveList156C(object, 0, frame);
}

uint32 G3MstrdbgEnterPrimary1668(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001668, "G3L_39");
    uint32 frame = guest_stack - 0x20;
    uint32 kind = G3MstrdbgListKind15BC(object);
    G3MstrdbgMoveList156C(object, kind, frame);
    uint32 flags = r_u32(object + 0x11c);
    flags &= 0xfbffffff;
    w_u32(object + 0x11c, flags);
    return flags;
}

uint32 G3MstrdbgEnterSecondary16B8(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000016b8, "G3L_39");
    uint32 frame = guest_stack - 0x20;
    uint32 kind = G3MstrdbgListKind15BC(object);
    G3MstrdbgMoveList156C(object, kind & 0xffffffb3, frame);
    uint32 flags = r_u32(object + 0x11c);
    flags |= 0x4000000;
    w_u32(object + 0x11c, flags);
    return flags;
}

uint32 G3MstrdbgSightClear1718(uint32 start, uint32 end, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001718, "G3L_39");
    uint32 frame = guest_stack - 0x38;
    uint32 value = r_u16(start);
    w_u16(frame + 0x20, value);
    value = r_u16(start + 2);
    w_u16(frame + 0x22, value);
    uint32 start_z = r_u16(start + 4);
    w_u16(frame + 0x24, start_z);
    value = r_u16(end);
    w_u16(frame + 0x28, value);
    value = r_u16(end + 2);
    w_u16(frame + 0x2a, value);
    uint32 end_z = r_u16(end + 4);
    uint32 game = r_u32(0x80097048);
    w_u16(frame + 0x24, start_z + 0x190);
    w_u16(frame + 0x2c, end_z);
    w_u16(frame + 0x2c, end_z + 0x190);
    uint32 geometry = r_u32(game + 0x48);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0x1c00be0);
    w_u32(frame + 0x18, 4);
    uint32 world = r_u32(geometry);
    return G3Function8006238C(world, frame + 0x28, frame + 0x20, 0, frame) == 0 ? 1 : 0;
}

uint32 G3MstrdbgUpdateSight17AC(uint32 start, uint32 end, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000017ac, "G3L_39");
    uint32 frame = guest_stack - 0x18;
    uint32 visible = G3MstrdbgSightClear1718(start, end, frame);
    uint32 flags = r_u32(state + 0x18);
    flags = visible != 0 ? flags | 0x80000 : flags & 0xfff7ffff;
    w_u32(state + 0x18, flags);
    return flags;
}

void G3MstrdbgStaggerSight17F8(uint32 start, uint32 end, uint32 state, uint32 phase, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000017f8, "G3L_39");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x18;
    uint32 tick = r_u32(game + 0x14c) & 0xf;
    sint32 remainder = (sint32)phase % 16;
    if (remainder < 0) remainder = -remainder;
    if (tick == (uint32)remainder) G3MstrdbgUpdateSight17AC(start, end, state, frame);
}

void G3MstrdbgSetupChain1854(uint32 resource, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001854, "G3L_39");
    uint32 frame = guest_stack - 0x20;
    if (r_u32(settings + 0x28) == 0x26a0d355)
        return;
    do
    {
        G3MstrdbgSetupAnimations2328(resource, settings, module_base, frame);
        uint32 extra = r_u32(settings + 0x18);
        w_u32(settings + 0x28, 0x26a0d355);
        if (extra != 0)
        {
            uint32 next = r_u32(extra);
            if (next != 0)
                settings = next;
        }
    }
    while (r_u32(settings + 0x28) != 0x26a0d355);
}

uint32 G3MstrdbgRotateQuarter18E0(uint32 vector)
{
    FUNCTION_MARKER(0x000018e0, "G3L_39");
    uint32 x = r_u16(vector);
    uint32 y = r_u16(vector + 2);
    uint32 negative_x = 0 - x;
    w_u16(vector, y);
    w_u16(vector + 2, negative_x);
    return negative_x;
}

uint32 G3MstrdbgValidateRange18F8(uint32 state, uint32 settings)
{
    FUNCTION_MARKER(0x000018f8, "G3L_39");
    if (state == 0)
        return 0;
    sint32 index = r_s16(state);
    if (index < 0)
        return 0;
    sint32 count = r_s16(settings + 0xa);
    if (index >= count)
        return 0;
    if (r_s16(state + 4) == -1)
    {
        uint32 table = r_u32(settings + 0x10);
        uint32 entry = r_u32(table + (uint32)index * 4);
        uint32 value = r_u16(entry + 2);
        w_u16(state + 4, value - 1);
    }
    return 1;
}

void G3MstrdbgValidateAnimations1968(uint32 resource, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001968, "G3L_39");
    uint32 frame = guest_stack - 0x28;
    for (uint32 index = 0; index < 0x30; ++index)
    {
        uint32 slot = settings + 0x2c + index * 4;
        uint32 animation = r_u32(slot);
        if (animation == 0)
            continue;
        uint32 valid = G3MstrdbgValidateRange18F8(animation, resource);
        if ((valid & 0xff) != 0)
            continue;
        w_u32(slot, 0);
        sint32 number = r_s16(animation);
        {
#ifndef NDEBUG
        fprintf(stdout, (const char *)psx_addr(module_base + 0x840c, 34), number);
#endif
        }
        uint32 name = r_u32(resource + 0x24);
        uint32 bytes = 0;
        while (r_u8(name + bytes) != 0)
            ++bytes;
        {
#ifndef NDEBUG
        fprintf(stdout, (const char *)psx_addr(module_base + 0x8430, 15), (const char *)psx_addr(name, bytes + 1));
#endif
        }
    }
}

uint32 G3MstrdbgClearFlags1A24(uint32 flags)
{
    FUNCTION_MARKER(0x00001a24, "G3L_39");
    for (uint32 index = 0; index < 0x18; ++index)
        w_u8(flags + index, 0);
    return 0;
}

uint32 G3MstrdbgClearSlots1A54(uint32 state)
{
    FUNCTION_MARKER(0x00001a54, "G3L_39");
    for (uint32 index = 0; index < 0x30; ++index)
        w_u32(state + 0x2c + index * 4, 0);
    return 0;
}

void G3MstrdbgBindAnimations1A8C(uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001a8c, "G3L_39");
    uint32 frame = guest_stack - 0x20;
    if (settings == 0)
        return;
    uint32 entry = r_u32(settings + 4);
    if (entry == 0)
        return;
    uint32 slots = settings + 0x2c;
    const uint32 outputs[24] = {0, 0x14, 0x1c, 0x20, 0x2c, 0x30, 8, 0x34, 0xc, 0x10, 0x18, 4, 0x24, 0x28, 0x38, 0x3c, 0x40, 0x44, 0x48, 0x4c, 0x58, 0x5c, 0x60, 0x64};
    while (true)
    {
        uint32 first = r_u32(entry);
        uint32 marker = r_u32(module_base + 0x83a0);
        if (first == marker)
        {
            uint32 second = r_u32(entry + 4);
            marker = r_u32(module_base + 0x83a4);
            if (second == marker)
                return;
        }
        uint32 payload = entry + 8;
        bool bound = false;
        for (uint32 index = 0; index < 24; ++index)
        {
            first = r_u32(entry);
            uint32 tag = module_base + 0x8440 + index * 0xc;
            marker = r_u32(tag);
            if (first != marker)
                continue;
            uint32 second = r_u32(entry + 4);
            marker = r_u32(tag + 4);
            if (second != marker)
                continue;
            w_u32(slots + outputs[index], payload);
            bound = true;
            break;
        }
        if (!bound)
            G3MstrdbgExtraAnimationDiagnostic(entry, module_base, frame);
        entry += 0x20;
    }
}

uint32 G3MstrdbgClearAnimationBlocks2008(uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002008, "G3L_39");
    uint32 frame = guest_stack - 0x20;
    uint32 block = settings + 0xec;
    for (uint32 index = 0; index < 15; ++index)
    {
        G3MstrdbgClearFlags1A24(block);
        block += 0x18;
    }
    return 0;
}

uint32 G3MstrdbgDefaultAnimations2058(uint32 settings)
{
    FUNCTION_MARKER(0x00002058, "G3L_39");
    if (r_u32(settings + 0x2c) == 0)
    {
        w_u16(settings + 0xf0, 0xffff);
        w_u16(settings + 0xf2, 100);
        w_u16(settings + 0xf4, 0x50);
        w_u16(settings + 0xf6, 1);
        w_u32(settings + 0x2c, settings + 0xec);
    }
    if (r_u32(settings + 0x40) == 0)
    {
        uint32 source = r_u32(settings + 0x2c);
        uint32 destination = settings + 0x104;
        uint32 words[4];
        for (uint32 word = 0; word < 4; ++word)
            words[word] = r_u32(source + word * 4);
        for (uint32 word = 0; word < 4; ++word)
            w_u32(destination + word * 4, words[word]);
        uint32 first = r_u32(source + 0x10);
        uint32 second = r_u32(source + 0x14);
        w_u32(destination + 0x10, first);
        w_u32(destination + 0x14, second);
        w_u16(settings + 0x110, 300);
        w_u32(settings + 0x40, settings + 0x104);
    }
    if (r_u32(settings + 0x58) == 0)
        w_u32(settings + 0x58, settings + 0xec);
    const uint32 sources[3] = {0x2c, 0x40, 0x40};
    const uint32 destinations[3] = {0x164, 0x14c, 0x17c};
    for (uint32 index = 0; index < 3; ++index)
    {
        uint32 source;
        if (index == 2)
        {
            sint32 percentage = r_s16(settings + 0x152);
            source = r_u32(settings + 0x40);
            w_u16(settings + 0x152, (uint32)((percentage * 60) / 100));
        }
        else
            source = r_u32(settings + sources[index]);
        uint32 destination = settings + destinations[index];
        uint32 words[4];
        for (uint32 word = 0; word < 4; ++word)
            words[word] = r_u32(source + word * 4);
        for (uint32 word = 0; word < 4; ++word)
            w_u32(destination + word * 4, words[word]);
        uint32 first = r_u32(source + 0x10);
        uint32 second = r_u32(source + 0x14);
        w_u32(destination + 0x10, first);
        w_u32(destination + 0x14, second);
    }
    uint32 negative = 0 - (uint32)r_s16(settings + 0x182);
    w_u16(settings + 0x182, negative);
    return negative;
}

void G3MstrdbgBindDefaults21E0(uint32 settings)
{
    FUNCTION_MARKER(0x000021e0, "G3L_39");
    if (r_u32(settings + 0x34) == 0)
        w_u32(settings + 0x34, settings + 0x164);
    if (r_u32(settings + 0x30) == 0)
        w_u32(settings + 0x30, settings + 0x14c);
    if (r_u32(settings + 0x44) == 0)
        w_u32(settings + 0x44, settings + 0x17c);
    if (r_u32(settings + 0x88) == 0)
        w_u32(settings + 0x88, r_u32(settings + 0x84));
}

void G3MstrdbgValidateSequence2244(uint32 sequence, uint32 resource, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002244, "G3L_39");
    if (sequence == 0)
        return;
    uint32 entries = sequence + 8;
    if (r_u32(entries) == 0)
        return;
    uint32 index = 0;
    uint32 next;
    do
    {
        uint32 entry = r_u32(entries + (uint32)(sint16)index * 4);
        G3MstrdbgValidateRange18F8(entry, resource);
        ++index;
        next = r_u32(entries + (uint32)(sint16)index * 4);
    } while (next != 0);
}

void G3MstrdbgValidateSequences22C4(uint32 resource, uint32 settings, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000022c4, "G3L_39");
    uint32 frame = guest_stack - 0x20;
    uint32 cursor = r_u32(settings + 8);
    if (cursor == 0)
        return;
    uint32 next = r_u32(cursor);
    while (next != 0)
    {
        uint32 sequence = r_u32(cursor);
        cursor += 4;
        G3MstrdbgValidateSequence2244(sequence, resource, frame);
        next = r_u32(cursor);
    }
}

void G3MstrdbgSetupAnimations2328(uint32 resource, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002328, "G3L_39");
    uint32 frame = guest_stack - 0x20;
    G3MstrdbgClearSlots1A54(settings);
    G3MstrdbgBindAnimations1A8C(settings, module_base, frame);
    G3MstrdbgClearAnimationBlocks2008(settings, frame);
    G3MstrdbgDefaultAnimations2058(settings);
    G3MstrdbgBindDefaults21E0(settings);
    G3MstrdbgValidateAnimations1968(resource, settings, module_base, frame);
    G3MstrdbgValidateSequences22C4(resource, settings, frame);
}

uint32 G3MstrdbgLookup2394(uint32 kind, uint32 first_index, uint32 second_index, uint32 table)
{
    FUNCTION_MARKER(0x00002394, "G3L_39");
    if (kind != 0x15) return r_u32(table + kind * 4 + 0x2c);
    uint32 groups = r_u32(table + 8);
    if (groups == 0) return 0;
    uint32 group = r_u32(groups + first_index * 4);
    uint32 elements = group + 8;
    if (group == 0 || elements == 0) return 0;
    return r_u32(elements + second_index * 4);
}

uint32 G3MstrdbgAdvanceBounded23F4(uint32 step, uint32 position, uint32 settings, uint32 scale)
{
    FUNCTION_MARKER(0x000023f4, "G3L_39");
    sint32 rate = r_s16(settings + 6);
    sint32 factor = rate * (sint16)scale / 100;
    uint32 amount = (uint32)r_s16(step) * (uint32)factor;
    sint32 delta = (sint32)amount / 100;
    uint32 value = r_u16(position) + (uint32)delta;
    w_u16(position, value);
    uint32 limit;
    bool clamp;
    if (factor >= 0)
    {
        sint32 maximum = r_s16(settings + 4);
        limit = (uint32)((maximum + 1) * 40 - 1);
        clamp = (sint32)(limit << 16) < (sint32)(value << 16);
    }
    else
    {
        sint32 minimum = r_s16(settings + 2);
        limit = (uint32)(minimum * 40);
        clamp = (sint32)(value << 16) < (sint32)(limit << 16);
    }
    if (clamp) w_u16(position, limit);
    w_u16(step, 0);
    return clamp ? 1 : 0;
}

void G3MstrdbgSampleMotion24CC(uint32 animation, uint32 time, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000024cc, "G3L_39");
    uint32 speed_bits = r_u16(animation + 6);
    sint32 elapsed = (sint16)time;
    w_u16(output, 0);
    w_u16(output + 2, 0);
    w_u16(output + 4, 0);
    w_u16(output + 8, 0);
    w_u16(output + 0xa, 0);
    w_u16(output + 0xc, 0);
    sint32 turn = (sint16)((uint32)r_s16(animation + 0x10) * 11);
    sint32 numerator = turn * elapsed;
    uint32 end = r_u16(animation + 4);
    uint32 begin = r_u16(animation + 2);
    sint32 span = (sint16)(end - begin + 1);
    sint32 divisor = (sint16)(span * 40);
    sint32 quotient;
    if (divisor == 0) quotient = numerator < 0 ? 1 : -1;
    else if (numerator == (sint32)0x80000000 && divisor == -1) quotient = numerator;
    else quotient = numerator / divisor;
    sint32 speed = (sint16)speed_bits;
    w_u16(output + 0xc, (uint32)((sint16)quotient * speed / 100));
    numerator = r_s16(animation + 0xc) * elapsed;
    if (divisor == 0) quotient = numerator < 0 ? 1 : -1;
    else if (numerator == (sint32)0x80000000 && divisor == -1) quotient = numerator;
    else quotient = numerator / divisor;
    w_u16(output, (uint32)((sint16)quotient * speed / 100));
    numerator = r_s16(animation + 0xe) * elapsed;
    if (divisor == 0) quotient = numerator < 0 ? 1 : -1;
    else if (numerator == (sint32)0x80000000 && divisor == -1) quotient = numerator;
    else quotient = numerator / divisor;
    w_u16(output + 2, (uint32)((sint16)quotient * speed / 100));
    G3MstrdbgRotateQuarter18E0(output);
}

void G3MstrdbgScaleVector2648(uint32 vector, uint32 scale)
{
    FUNCTION_MARKER(0x00002648, "G3L_39");
    sint32 x = r_s16(vector);
    sint32 factor = r_s16(scale);
    sint32 product = x * factor;
    if (product < 0) product += 0xfff;
    sint32 y = r_s16(vector + 2);
    w_u16(vector, (uint32)(product >> 12));
    factor = r_s16(scale + 2);
    product = y * factor;
    if (product < 0) product += 0xfff;
    sint32 z = r_s16(vector + 4);
    w_u16(vector + 2, (uint32)(product >> 12));
    factor = r_s16(scale + 4);
    product = z * factor;
    if (product < 0) product += 0xfff;
    w_u16(vector + 4, (uint32)(product >> 12));
}

void G3MstrdbgAnimationMotion26C4(uint32 animation, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000026c4, "G3L_39");
    w_u16(output, 0);
    w_u16(output + 2, 0);
    w_u16(output + 4, 0);
    w_u16(output + 8, 0);
    w_u16(output + 0xa, 0);
    w_u16(output + 0xc, 0);
    uint32 turn = (uint32)r_s16(animation + 0x10) * 11;
    w_u16(output + 0xc, turn);
    w_u16(output, r_u16(animation + 0xc));
    w_u16(output + 2, r_u16(animation + 0xe));
    G3MstrdbgRotateQuarter18E0(output);
}

void G3MstrdbgLookupMotion272C(uint32 state, uint32 output, uint32 table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000272c, "G3L_39");
    uint32 frame = guest_stack - 0x18;
    uint32 kind = (uint32)r_s8(state + 2);
    uint32 first = (uint32)r_s8(state + 5);
    uint32 second = (uint32)r_s8(state + 6);
    uint32 animation = G3MstrdbgLookup2394(kind, first, second, table);
    if (animation == 0)
    {
        w_u16(output, 0);
        w_u16(output + 2, 0);
        w_u16(output + 4, 0);
        w_u16(output + 8, 0);
        w_u16(output + 0xa, 0);
        w_u16(output + 0xc, 0);
    }
    else G3MstrdbgAnimationMotion26C4(animation, output, frame);
}

uint32 G3MstrdbgSelectAnimation2794(uint32 phase, uint32 output, uint32 state, uint32 model)
{
    FUNCTION_MARKER(0x00002794, "G3L_39");
    uint32 kind = (uint32)r_s8(state + 2);
    uint32 first_index = (uint32)r_s8(state + 5);
    uint32 second_index = (uint32)r_s8(state + 6);
    uint32 animation = G3MstrdbgLookup2394(kind, first_index, second_index, model);
    if (animation != 0)
    {
        w_u32(output, animation);
        sint32 direction = r_s16(animation + 6);
        uint32 value;
        if (direction >= 0) value = (uint32)(r_s16(animation + 2) * 40);
        else value = (uint32)((r_s16(animation + 4) + 1) * 40 - 1);
        w_u16(phase, value);
    }
    return animation != 0 ? 1 : 0;
}

void G3MstrdbgProjectSeparation2830(uint32 output, uint32 vector, uint32 normal, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002830, "G3L_39");
    uint32 frame = guest_stack - 0x40;
    sint32 first = r_s16(normal);
    sint32 second = r_s16(vector);
    uint32 dot = (uint32)(first * second);
    first = r_s16(normal + 2);
    second = r_s16(vector + 2);
    dot += (uint32)(first * second);
    first = r_s16(normal + 4);
    second = r_s16(vector + 4);
    dot += (uint32)(first * second);
    uint32 length = G3Function8002FA50(normal, frame);
    if ((sint32)dot < 0)
    {
        uint32 magnitude = 0;
        if (length != 0)
        {
            magnitude = dot == 0x80000000 && length == 0xffffffff ? 0x80000000 : (uint32)((sint32)dot / (sint32)length);
            if ((sint32)magnitude < 0) magnitude = 0 - magnitude;
        }
        uint32 x = (uint32)r_s16(normal);
        uint32 product_x = x * magnitude;
        uint32 quotient_x = length == 0 ? ((sint32)product_x < 0 ? 1 : 0xffffffff) : product_x == 0x80000000 && length == 0xffffffff ? 0x80000000 : (uint32)((sint32)product_x / (sint32)length);
        w_u32(frame + 0x10, x);
        uint32 y = (uint32)r_s16(normal + 2);
        uint32 product_y = y * magnitude;
        uint32 quotient_y = length == 0 ? ((sint32)product_y < 0 ? 1 : 0xffffffff) : product_y == 0x80000000 && length == 0xffffffff ? 0x80000000 : (uint32)((sint32)product_y / (sint32)length);
        w_u32(frame + 0x14, y);
        uint32 z = (uint32)r_s16(normal + 4);
        uint32 product_z = z * magnitude;
        uint32 quotient_z = length == 0 ? ((sint32)product_z < 0 ? 1 : 0xffffffff) : product_z == 0x80000000 && length == 0xffffffff ? 0x80000000 : (uint32)((sint32)product_z / (sint32)length);
        w_u32(frame + 0x10, product_x);
        w_u32(frame + 0x14, product_y);
        w_u32(frame + 0x18, z);
        w_u32(frame + 0x18, product_z);
        w_u32(frame + 0x10, quotient_x);
        w_u32(frame + 0x14, quotient_y);
        w_u32(frame + 0x18, quotient_z);
        sint32 projected_x = (sint32)quotient_x;
        if (projected_x < -32767) projected_x = -32767;
        if (projected_x > 32767) projected_x = 32767;
        w_u16(frame + 0x20, (uint32)projected_x);
        sint32 projected_y = (sint32)quotient_y;
        if (projected_y < -32767) projected_y = -32767;
        if (projected_y > 32767) projected_y = 32767;
        w_u16(frame + 0x22, (uint32)projected_y);
        sint32 projected_z = (sint32)quotient_z;
        if (projected_z < -32767) projected_z = -32767;
        if (projected_z > 32767) projected_z = 32767;
        w_u16(frame + 0x24, (uint32)projected_z);
        w_u16(output, r_u16(vector) + (uint32)projected_x);
        uint32 position = r_u16(vector + 2);
        uint32 offset = r_u16(frame + 0x22);
        w_u16(output + 2, position + offset);
        position = r_u16(vector + 4);
        offset = r_u16(frame + 0x24);
        w_u16(output + 4, position + offset);
    }
    else
    {
        w_u16(output, r_u16(vector));
        w_u16(output + 2, r_u16(vector + 2));
        w_u16(output + 4, r_u16(vector + 4));
    }
}

uint32 G3MstrdbgFacingContact2A2C(uint32 rotation, uint32 vector, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002a2c, "G3L_39");
    uint32 frame = guest_stack - 0x18;
    sint32 z = r_s16(vector + 4);
    sint32 x = r_s16(vector);
    if (z < 0) z = -z;
    if (x < 0) x = -x;
    if (x < z)
    {
        sint32 y = r_s16(vector + 2);
        if (y < 0) y = -y;
        if (y < z) return 0;
    }
    uint32 heading = r_u16(rotation + 4);
    uint32 y = (uint32)r_s16(vector + 2);
    uint32 nx = (uint32)r_s16(vector);
    uint32 angle = G3Function8002F0C4(y, nx, frame);
    uint32 difference = (heading - 0x400 - angle - 0x800) & 0xfff;
    if (difference >= 0x800) difference |= 0xf000;
    sint32 magnitude = (sint16)difference;
    if (magnitude < 0) magnitude = -magnitude;
    return magnitude < 0x371 ? 1 : 0;
}

void G3MstrdbgDeflectVelocity2ADC(uint32 velocity, uint32 delta, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002adc, "G3L_39");
    uint32 frame = guest_stack - 0x28;
    uint32 bits = r_u8(velocity);
    w_u16(frame + 0x10, (uint32)(sint8)bits);
    bits = r_u8(velocity + 1);
    w_u16(frame + 0x14, 0);
    w_u16(frame + 0x12, (uint32)(sint8)bits);
    G3MstrdbgProjectSeparation2830(frame + 0x18, frame + 0x10, delta, frame);
    sint32 x = r_s16(frame + 0x18);
    uint32 x_bits = r_u16(frame + 0x18);
    if (x < -127) x_bits = (uint32)-127;
    else if (x >= 127) x_bits = 127;
    sint32 y = r_s16(frame + 0x1a);
    uint32 y_bits = r_u16(frame + 0x1a);
    w_u16(frame + 0x18, x_bits);
    if (y < -127) y_bits = (uint32)-127;
    else if (y >= 127) y_bits = 127;
    bits = r_u8(frame + 0x18);
    w_u16(frame + 0x1a, y_bits);
    w_u8(velocity, bits);
    w_u8(velocity + 1, r_u8(frame + 0x1a));
}

uint32 G3MstrdbgClassifyContact2BB4(uint32 output, uint32 contact, uint32 side)
{
    FUNCTION_MARKER(0x00002bb4, "G3L_39");
    uint32 object;
    uint32 kind;
    uint32 descriptor;
    if (side == 0)
    {
        object = r_u32(contact + 0x10);
        kind = r_u16(contact + 4);
        descriptor = r_u32(contact + 8);
    }
    else
    {
        object = r_u32(contact + 0x14);
        kind = r_u16(contact + 6);
        descriptor = r_u32(contact + 0xc);
    }
    w_u32(output + 4, object);
    if ((sint16)kind == 1)
    {
        w_u8(output, r_u8(descriptor + 4));
        w_u8(output + 1, r_u8(descriptor + 5));
    }
    else if ((sint16)kind == 5)
    {
        w_u8(output, r_u8(descriptor + 2));
        w_u8(output + 1, r_u8(descriptor + 3));
    }
    else
    {
        w_u32(output + 4, 0);
        return 0;
    }
    return r_u32(output + 4) != 0 ? 1 : 0;
}

void G3MstrdbgBuildDamagePacket2C48(uint32 packet, uint32 object, uint32 target_position, uint32 contact, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002c48, "G3L_39");
    uint32 frame = guest_stack - 0x30;
    uint32 trigger = r_u32(guest_stack + 0x10);
    sint32 damage = r_s16(trigger + 8);
    w_u16(packet + 0x10, 0);
    w_u32(packet, (uint32)damage);
    sint32 kind = r_s16(trigger + 0x10);
    if (kind == 1) w_u16(packet + 0x10, 7);
    else if (kind == 2) w_u16(packet + 0x10, 3);
    uint32 coordinate = r_u16(target_position);
    uint32 origin = r_u16(object + 0x58);
    w_u16(frame + 0x10, coordinate - origin);
    coordinate = r_u16(target_position + 2);
    origin = r_u16(object + 0x5a);
    w_u16(frame + 0x14, 0);
    w_u16(frame + 0x12, coordinate - origin);
    uint32 magnitude_bits = r_u16(trigger + 0xa);
    sint32 length = (sint16)magnitude_bits / 5;
    G3Function8002F9A8(frame + 0x18, (uint32)(sint16)length, frame + 0x10, frame);
    w_u32(packet + 4, (uint32)r_s16(frame + 0x18));
    w_u32(packet + 8, (uint32)r_s16(frame + 0x1a));
    uint32 x = r_u32(packet + 4);
    uint32 z = (uint32)r_s16(frame + 0x1c);
    w_u32(packet + 4, x << 16);
    uint32 y = r_u32(packet + 8);
    w_u32(packet + 0x14, contact);
    w_u32(packet + 0xc, z);
    w_u32(packet + 0xc, z << 16);
    w_u32(packet + 8, y << 16);
}

void G3MstrdbgAttackContact2D50(uint32 own_class, uint32 other_class, uint32 contact, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002d50, "G3L_39");
    uint32 frame = guest_stack - 0x50;
    uint32 other = r_u32(contact + 0x14);
    uint32 player = r_u32(0x8009efc4);
    if (other != player) return;
    uint32 state = object + 0x104;
    uint32 guard = ~(r_u32(state + 0x18) & 0x8000);
    if (guard == 0) return;
    if (r_u8(own_class + 1) - (uint32)6 >= 4) return;
    uint32 kind = r_u8(other_class + 1);
    if (kind != 9 && kind - 3 >= 3) return;
    uint32 trigger = G3MstrdbgFindTrigger44AC(0, r_u32(state + 0x14));
    if (trigger == 0) return;
    uint32 own_kind = r_u8(own_class);
    sint32 required_kind = r_s16(trigger + 0xc);
    if (own_kind != (uint32)required_kind) return;
    player = r_u32(0x8009efc4);
    w_u32(frame + 0x10, trigger);
    G3MstrdbgBuildDamagePacket2C48(frame + 0x18, object, player + 0x58, contact, frame);
    if (r_s16(trigger + 8) != 0)
    {
        player = r_u32(0x8009efc4);
        uint32 target = r_u32(player + 0xec);
        if (target != 0) G3DispatchStateEvent(target, player, r_u32(0x80097048), 8, frame + 0x18, frame);
    }
    w_u32(state + 0x18, r_u32(state + 0x18) | 0x8000);
    w_u32(object + 0x14, r_u32(object + 0x14) & 0xf7ffffff);
    if ((r_u16(trigger + 0xe) & 4) != 0)
    {
        G3MstrdbgEnterSecondary16B8(object, frame);
        w_u8(state + 0x36, 15);
    }
}

void G3MstrdbgSeparateContact2EC4(uint32 contact, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002ec4, "G3L_39");
    uint32 frame = guest_stack - 0x58;
    w_u16(frame + 0x28, r_u16(object + 0x58));
    w_u16(frame + 0x2a, r_u16(object + 0x5a));
    w_u16(frame + 0x2c, r_u16(object + 0x5c));
    w_u16(frame + 0x10, r_u16(contact + 0x28));
    uint32 offset = r_u16(contact + 0x2a);
    w_u16(frame + 0x14, 0);
    w_u16(frame + 0x12, offset);
    uint32 other = r_u32(contact + 0x14);
    uint32 state = object + 0x104;
    bool direct = true;
    if (other != 0)
    {
        uint32 descriptor = r_u32(other + 0x1c);
        bool query = (r_u32(descriptor) & 1) != 0;
        if (!query) query = other == r_u32(0x8009efc4);
        if (query) direct = G3Function8008345C(other, 0, 0) != 0;
    }
    uint32 result_z;
    if (direct)
    {
        w_u16(frame + 0x18, r_u16(contact + 0x28));
        w_u16(frame + 0x1a, r_u16(contact + 0x2a));
        w_u16(frame + 0x1c, r_u16(contact + 0x2c));
        if (G3MstrdbgFacingContact2A2C(object + 0x60, frame + 0x18, frame) != 0) w_u32(state + 0x18, r_u32(state + 0x18) | 0x10);
        if ((r_u32(state + 0x20) & 0xffff0000) != 0) G3MstrdbgDeflectVelocity2ADC(state + 0x22, frame + 0x10, frame);
        uint32 position = r_u16(object + 0x58);
        offset = r_u16(frame + 0x10);
        w_u16(object + 0x58, position + offset);
        position = r_u16(object + 0x5a);
        offset = r_u16(frame + 0x12);
        w_u16(object + 0x5a, position + offset);
        position = r_u16(object + 0x5c);
        offset = r_u16(frame + 0x14);
        result_z = position + offset;
    }
    else
    {
        uint32 position = r_u16(object + 0x58);
        uint32 previous = r_u16(state + 0x2a);
        w_u16(frame + 0x38, position - previous);
        position = r_u16(object + 0x5a);
        previous = r_u16(state + 0x2c);
        w_u16(frame + 0x3a, position - previous);
        position = r_u16(object + 0x5c);
        previous = r_u16(state + 0x2e);
        w_u16(frame + 0x3c, position - previous);
        G3MstrdbgProjectSeparation2830(frame + 0x40, frame + 0x38, frame + 0x10, frame);
        previous = r_u16(state + 0x2a);
        offset = r_u16(frame + 0x40);
        w_u16(object + 0x58, previous + offset);
        previous = r_u16(state + 0x2c);
        offset = r_u16(frame + 0x42);
        w_u16(object + 0x5a, previous + offset);
        previous = r_u16(state + 0x2e);
        offset = r_u16(frame + 0x44);
        result_z = previous + offset;
    }
    w_u16(object + 0x5c, result_z);
    uint32 coordinate = r_u16(object + 0x58);
    uint32 previous = r_u16(frame + 0x28);
    r_u32(0x80097048);
    uint32 x = coordinate - previous;
    w_u16(frame + 0x20, x);
    coordinate = r_u16(object + 0x5a);
    previous = r_u16(frame + 0x2a);
    uint32 previous_z = r_u16(frame + 0x2c);
    uint32 y = coordinate - previous;
    w_u16(frame + 0x22, y);
    coordinate = r_u16(object + 0x5c);
    w_u16(frame + 0x30, x);
    w_u16(frame + 0x32, y);
    uint32 z = coordinate - previous_z;
    w_u16(frame + 0x24, z);
    w_u16(frame + 0x34, z);
    G3Function8005F3A8(object, frame + 0x30);
}

void G3MstrdbgCollide30E4(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000030e4, "G3L_39");
    uint32 frame = guest_stack - 0x30;
    uint32 contact = r_u32(object + 0xb0);
    G3MstrdbgClassifyContact2BB4(frame + 0x10, contact, 0);
    G3MstrdbgClassifyContact2BB4(frame + 0x18, contact, 1);
    if (r_u8(frame + 0x11) - (uint32)6 < 4 && (r_u32(object + 0x14) & 0x8000000) != 0) G3MstrdbgAttackContact2D50(frame + 0x10, frame + 0x18, contact, object, frame);
    uint32 kind = r_u8(frame + 0x11);
    if (kind == 9 || kind - 3 < 3) G3MstrdbgSeparateContact2EC4(contact, object, frame);
}

void G3MstrdbgInitializePacket31A0(uint32 packet)
{
    FUNCTION_MARKER(0x000031a0, "G3L_39");
    G3FillMemory80086094(packet, 0, 0x18);
    w_u32(packet, 1);
}

uint32 G3MstrdbgCheckFlags31D4(uint32 state, uint32 model)
{
    FUNCTION_MARKER(0x000031d4, "G3L_39");
    uint32 flags = r_u32(state + 0x18);
    uint32 resource = r_u32(model);
    uint32 resource_flags = r_u32(resource + 0x18);
    uint32 result = (flags & 0x4000000) != 0 ? 1 : 0;
    if ((resource_flags & 8) != 0 && (flags & 0x800000) == 0) result = 1;
    if ((r_u32(state + 0x18) & 0x400000) != 0) result = 1;
    return result;
}

uint32 G3MstrdbgDamage322C(uint32 object, uint32 packet, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000322c, "G3L_39");
    uint32 frame = guest_stack - 0x38;
    uint32 model = r_u32(object + 0x24);
    uint32 state = object + 0x104;
    if (packet == 0) return 0;
    uint32 contact = r_u32(packet + 0x14);
    if (contact != 0)
    {
        uint32 attacker = r_u32(contact + 0x10);
        uint32 player = r_u32(0x8009efc4);
        if (attacker == player)
        {
            uint32 serial = r_u32(attacker + 0x138);
            uint32 previous = r_u32(state + 0x50);
            if (serial == previous) return 0;
            w_u32(state + 0x50, serial);
        }
    }
    if (G3MstrdbgCheckFlags31D4(state, model) != 0) return 0;
    if (r_s8(state + 1) == 0xa) return 0;
    uint32 resource = r_u32(model);
    if ((r_u32(resource + 0x18) & 0x20) == 0)
    {
        if ((r_u32(state + 0x18) & 0x200000) != 0) return 0;
        if (r_u8(state + 0x4e) != 0) return 0;
    }
    resource = r_u32(model);
    uint32 flags = r_u32(resource + 0x18);
    uint32 damage = r_u16(packet);
    if ((flags & 0x10) != 0 && r_s16(packet + 0x10) != 1) damage = 0;
    resource = r_u32(model);
    uint32 divisor = r_u32(resource + 4);
    if ((sint32)divisor <= 0)
    {
        w_u32(frame + 0x10, 0);
        w_u32(frame + 0x14, 0);
        w_u32(frame + 0x18, 0);
    }
    else
    {
        uint32 numerator = r_u32(packet + 4);
        uint32 quotient = (uint32)((sint32)numerator / (sint32)divisor);
        w_u32(frame + 0x10, quotient);
        resource = r_u32(model);
        numerator = r_u32(packet + 8);
        divisor = r_u32(resource + 4);
        quotient = divisor == 0 ? ((sint32)numerator < 0 ? 1 : 0xffffffff) : numerator == 0x80000000 && divisor == 0xffffffff ? 0x80000000 : (uint32)((sint32)numerator / (sint32)divisor);
        w_u32(frame + 0x14, quotient);
        resource = r_u32(model);
        numerator = r_u32(packet + 0xc);
        divisor = r_u32(resource + 4);
        quotient = divisor == 0 ? ((sint32)numerator < 0 ? 1 : 0xffffffff) : numerator == 0x80000000 && divisor == 0xffffffff ? 0x80000000 : (uint32)((sint32)numerator / (sint32)divisor);
        w_u32(frame + 0x18, quotient);
    }
    uint32 x = r_u32(frame + 0x10);
    uint32 z = r_u32(frame + 0x18);
    w_u32(frame + 0x10, (uint32)((sint32)x >> 10));
    uint32 y = r_u32(frame + 0x14);
    w_u32(frame + 0x18, (uint32)((sint32)z >> 10));
    w_u32(frame + 0x14, (uint32)((sint32)y >> 10));
    uint32 length = G3Function8002FAB0(frame + 0x10, frame);
    if ((sint32)length >= 0x80)
    {
        x = r_u32(frame + 0x10) * 127;
        uint32 scaled_x = (uint32)((sint32)x / (sint32)length);
        y = r_u32(frame + 0x14) * 127;
        uint32 scaled_y = (uint32)((sint32)y / (sint32)length);
        z = r_u32(frame + 0x18) * 127;
        uint32 scaled_z = (uint32)((sint32)z / (sint32)length);
        w_u32(frame + 0x10, x);
        w_u32(frame + 0x14, y);
        w_u32(frame + 0x18, z);
        w_u32(frame + 0x10, scaled_x);
        w_u32(frame + 0x14, scaled_y);
        w_u32(frame + 0x18, scaled_z);
    }
    sint32 component = r_s32(frame + 0x10);
    if (component < -127) component = -127;
    if (component >= 128) component = 127;
    sint32 next_component = r_s32(frame + 0x14);
    w_u32(frame + 0x10, (uint32)component);
    if (next_component < -127) next_component = -127;
    if (next_component >= 128) next_component = 127;
    component = r_s32(frame + 0x18);
    w_u32(frame + 0x14, (uint32)next_component);
    if (component < -127) component = -127;
    if (component >= 128) component = 127;
    x = r_u8(frame + 0x10);
    w_u32(frame + 0x18, (uint32)component);
    w_u8(state + 0x22, x);
    w_u8(state + 0x23, r_u8(frame + 0x14));
    w_u16(state + 0x40, r_u16(frame + 0x18));
    sint32 health = r_s16(state + 0x42);
    uint32 health_bits = r_u16(state + 0x42);
    if (health > 0) w_u16(state + 0x42, health_bits - damage);
    if (r_s16(state + 0x42) < 0) w_u16(state + 0x42, 0);
    resource = r_u32(model);
    if ((r_u32(resource + 0x18) & 0x40) != 0) G3Function80047EE4(10, (uint32)r_s16(state + 0x42));
    if (r_s16(state + 0x42) == 0) w_u32(state + 0x18, r_u32(state + 0x18) | 2);
    else
    {
        resource = r_u32(model);
        sint32 threshold = r_s32(resource + 0x14);
        if ((sint16)damage < threshold) return 1;
        w_u32(state + 0x18, r_u32(state + 0x18) | 4);
    }
    return 1;
}

uint32 G3MstrdbgRequestRemoval35A4(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000035a4, "G3L_39");
    uint32 frame = guest_stack - 0x18;
    uint32 descriptor = r_u32(object + 0x1c);
    if ((r_u32(descriptor + 0x3c) & 1) != 0) G3Function80082648(object, 0, frame);
    w_u32(object + 0x11c, r_u32(object + 0x11c) | 0x400);
    return 1;
}

void G3MstrdbgTraceFloor35F8(uint32 output, uint32 object, uint32 top, uint32 bottom, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000035f8, "G3L_39");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x40;
    uint32 level = r_u32(game + 0x48);
    uint32 x = r_u16(object + 0x58);
    uint32 world = r_u32(level);
    w_u16(frame + 0x20, x);
    w_u16(frame + 0x22, r_u16(object + 0x5a));
    uint32 z = r_u16(object + 0x5c);
    w_u16(frame + 0x24, z);
    w_u16(frame + 0x28, r_u16(object + 0x58));
    w_u16(frame + 0x2a, r_u16(object + 0x5a));
    uint32 raised_z = r_u16(object + 0x5c);
    w_u16(frame + 0x24, z - bottom);
    w_u16(frame + 0x2c, raised_z);
    w_u16(frame + 0x2c, raised_z + top);
    uint32 flags = r_u32(object + 0x14);
    uint32 mask = (flags & 0x20) != 0 ? 0xfffffff7 : 0x1c00be0;
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, mask);
    w_u32(frame + 0x18, 0);
    uint32 face = G3Function8006238C(world, frame + 0x20, frame + 0x28, 1, frame);
    w_u32(output + 4, face);
    if (face != 0)
    {
        w_u32(output, 1);
        face = r_u32(output + 4);
        uint32 normals = r_u32(world + 0x40);
        uint32 index = (uint32)r_s16(face + 8);
        G3Function8005F304(index, normals, output + 8, frame);
        {
            uint32 source = frame + 0x20;
            uint32 destination = output + 0x10;
            uint32 first_address = source + 0;
            uint32 first_high_address = first_address + 3;
            uint32 first_high_lane = first_high_address & 3;
            uint32 first_lane = first_address & 3;
            uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
            uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
            uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
            uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
            uint32 second_address = source + 4;
            uint32 second_high_address = second_address + 3;
            uint32 second_high_lane = second_high_address & 3;
            uint32 second_lane = second_address & 3;
            uint32 second_high = r_masked_u32(second_high_address & 0xfffffffc, (1 << (second_high_lane + 1)) - 1);
            uint32 second_low = r_masked_u32(second_address & 0xfffffffc, (15 << second_lane) & 15);
            uint32 second_keep = second_lane == 0 ? 0 : 0xffffffff << ((4 - second_lane) * 8);
            uint32 second_value = ((second_high << ((3 - second_high_lane) * 8)) & second_keep) | (second_low >> (second_lane * 8));
            first_address = destination + 0;
            first_high_address = first_address + 3;
            first_high_lane = first_high_address & 3;
            first_lane = first_address & 3;
            w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
            w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
            second_address = destination + 4;
            second_high_address = second_address + 3;
            second_high_lane = second_high_address & 3;
            second_lane = second_address & 3;
            w_masked_u32(second_high_address & 0xfffffffc, second_value >> ((3 - second_high_lane) * 8), (1 << (second_high_lane + 1)) - 1);
            w_masked_u32(second_address & 0xfffffffc, second_value << (second_lane * 8), (15 << second_lane) & 15);
        }
    }
}

void G3MstrdbgBoxFloor36F8(uint32 output, uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000036f8, "G3L_39");
    uint32 frame = guest_stack - 0x20;
    uint32 configuration = r_u32(object + 0x28);
    uint32 property = G3MstrdbgFindExtra0AEC(configuration, module_base + 0x8208, module_base, frame);
    w_u32(output, 2);
    w_u32(output + 4, 0);
    w_u16(output + 8, 0);
    w_u16(output + 0xa, 0);
    w_u16(output + 0xc, 0x1000);
    w_u16(output + 0x10, r_u16(object + 0x58));
    w_u16(output + 0x12, r_u16(object + 0x5a));
    w_u16(output + 0x14, r_u16(object + 0x5c));
    w_u16(output + 0x14, r_u16(property + 4));
}

void G3MstrdbgQueryFloor3780(uint32 output, uint32 object, uint32 top, uint32 bottom, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003780, "G3L_39");
    uint32 frame = guest_stack - 0x18;
    if ((r_u32(object + 0x11c) & 0x1000) == 0) G3MstrdbgTraceFloor35F8(output, object, top, bottom, frame);
    else
    {
        r_u32(guest_stack + 0x10);
        G3MstrdbgBoxFloor36F8(output, object, module_base, frame);
    }
}

void G3MstrdbgQueryTerrain37C8(uint32 output, uint32 object, uint32 position, uint32 bottom, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000037c8, "G3L_39");
    uint32 frame = guest_stack - 0x40;
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    uint32 world = r_u32(level);
    w_u16(frame + 0x20, r_u16(object + 0x58));
    w_u16(frame + 0x22, r_u16(object + 0x5a));
    uint32 z = r_u16(object + 0x5c);
    w_u16(frame + 0x24, z);
    w_u16(frame + 0x28, r_u16(position));
    uint32 y = r_u16(position + 2);
    w_u16(frame + 0x2a, y);
    uint32 raised_z = r_u16(position + 4);
    w_u16(frame + 0x24, z - bottom);
    w_u16(frame + 0x2c, raised_z);
    w_u16(frame + 0x2c, raised_z + 10);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0x1c00bea);
    w_u32(frame + 0x18, 0);
    uint32 face = G3Function8006238C(world, frame + 0x20, frame + 0x28, 0, frame);
    w_u32(output + 4, face);
    if (face != 0)
    {
        w_u32(output, 1);
        face = r_u32(output + 4);
        uint32 normals = r_u32(world + 0x40);
        G3Function8005F304((uint32)r_s16(face + 8), normals, output + 8, frame);
        {
            uint32 source = frame + 0x20;
            uint32 destination = output + 0x10;
            uint32 first_address = source + 0;
            uint32 first_high_address = first_address + 3;
            uint32 first_high_lane = first_high_address & 3;
            uint32 first_lane = first_address & 3;
            uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
            uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
            uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
            uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
            uint32 second_address = source + 4;
            uint32 second_high_address = second_address + 3;
            uint32 second_high_lane = second_high_address & 3;
            uint32 second_lane = second_address & 3;
            uint32 second_high = r_masked_u32(second_high_address & 0xfffffffc, (1 << (second_high_lane + 1)) - 1);
            uint32 second_low = r_masked_u32(second_address & 0xfffffffc, (15 << second_lane) & 15);
            uint32 second_keep = second_lane == 0 ? 0 : 0xffffffff << ((4 - second_lane) * 8);
            uint32 second_value = ((second_high << ((3 - second_high_lane) * 8)) & second_keep) | (second_low >> (second_lane * 8));
            first_address = destination + 0;
            first_high_address = first_address + 3;
            first_high_lane = first_high_address & 3;
            first_lane = first_address & 3;
            w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
            w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
            second_address = destination + 4;
            second_high_address = second_address + 3;
            second_high_lane = second_high_address & 3;
            second_lane = second_address & 3;
            w_masked_u32(second_high_address & 0xfffffffc, second_value >> ((3 - second_high_lane) * 8), (1 << (second_high_lane + 1)) - 1);
            w_masked_u32(second_address & 0xfffffffc, second_value << (second_lane * 8), (15 << second_lane) & 15);
        }
    }
}

void G3MstrdbgCaptureFloor38B0(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000038b0, "G3L_39");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x38;
    uint32 level = r_u32(game + 0x48);
    uint32 x = r_u16(object + 0x58);
    uint32 world = r_u32(level);
    w_u16(frame + 0x20, x);
    w_u16(frame + 0x22, r_u16(object + 0x5a));
    uint32 z = r_u16(object + 0x5c);
    w_u16(frame + 0x24, z);
    w_u16(frame + 0x28, r_u16(object + 0x58));
    w_u16(frame + 0x2a, r_u16(object + 0x5a));
    uint32 top = r_u16(object + 0x5c);
    w_u16(frame + 0x24, z - 0x500);
    w_u16(frame + 0x2c, top);
    w_u16(frame + 0x2c, top + 0x100);
    w_u32(frame + 0x14, 0xff3fff3f);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x18, 0);
    uint32 face = G3Function8006238C(world, frame + 0x20, frame + 0x28, 0, frame);
    if (face != 0)
    {
        w_u32(object + 0xf8, face);
        {
            uint32 source = frame + 0x20;
            uint32 destination = object + 0xfc;
            uint32 first_address = source + 0;
            uint32 first_high_address = first_address + 3;
            uint32 first_high_lane = first_high_address & 3;
            uint32 first_lane = first_address & 3;
            uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
            uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
            uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
            uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
            uint32 second_address = source + 4;
            uint32 second_high_address = second_address + 3;
            uint32 second_high_lane = second_high_address & 3;
            uint32 second_lane = second_address & 3;
            uint32 second_high = r_masked_u32(second_high_address & 0xfffffffc, (1 << (second_high_lane + 1)) - 1);
            uint32 second_low = r_masked_u32(second_address & 0xfffffffc, (15 << second_lane) & 15);
            uint32 second_keep = second_lane == 0 ? 0 : 0xffffffff << ((4 - second_lane) * 8);
            uint32 second_value = ((second_high << ((3 - second_high_lane) * 8)) & second_keep) | (second_low >> (second_lane * 8));
            first_address = destination + 0;
            first_high_address = first_address + 3;
            first_high_lane = first_high_address & 3;
            first_lane = first_address & 3;
            w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
            w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
            second_address = destination + 4;
            second_high_address = second_address + 3;
            second_high_lane = second_high_address & 3;
            second_lane = second_address & 3;
            w_masked_u32(second_high_address & 0xfffffffc, second_value >> ((3 - second_high_lane) * 8), (1 << (second_high_lane + 1)) - 1);
            w_masked_u32(second_address & 0xfffffffc, second_value << (second_lane * 8), (15 << second_lane) & 15);
        }
    }
}

void G3MstrdbgResolveFloor3970(uint32 object, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003970, "G3L_39");
    uint32 frame = guest_stack - 0x48;
    uint32 state = object + 0x104;
    uint32 floor = frame + 0x18;
    w_u32(floor, 0);
    w_u32(frame + 0x10, model);
    G3MstrdbgQueryFloor3780(floor, object, 0x100, 0x500, module_base, frame);
    bool grounded = false;
    if (r_u32(floor) != 0)
    {
        sint32 z = r_s16(object + 0x5c);
        sint32 floor_z = r_s16(floor + 0x14);
        uint32 floor_bits = r_u16(floor + 0x14);
        if (z - floor_z < 31 && r_s16(state + 0x40) < 10)
        {
            w_u16(object + 0x5c, floor_bits);
            grounded = true;
        }
    }
    if (grounded) w_u16(state + 0x40, 0);
    else
    {
        uint32 velocity = r_u16(state + 0x40) - 9;
        w_u16(state + 0x40, velocity);
        if ((sint16)velocity < -100) w_u16(state + 0x40, (uint32)-100);
    }
    uint32 game = r_u32(0x80097048);
    sint32 velocity = r_s16(state + 0x40);
    sint32 step = r_s16(game + 0x69bc);
    sint32 movement = velocity * step / 40;
    w_u16(object + 0x5c, r_u16(object + 0x5c) + (uint32)movement);
    if (r_u32(floor) == 0) w_u32(object + 0xa8, 0);
    else w_u32(object + 0xa8, r_u32(floor + 4));
    uint32 descriptor = r_u32(object + 0x1c);
    if ((r_u32(descriptor) & 0x200) == 0 || r_u32(floor) == 0) return;
    uint32 resource = r_u32(model);
    if ((r_u32(resource + 0x18) & 1) != 0)
    {
        w_u32(floor, 0);
        G3MstrdbgQueryTerrain37C8(floor, object, floor + 0x10, 0x500, frame);
    }
    uint32 kind = r_u32(floor);
    if (kind == 0) return;
    if (kind == 1)
    {
        uint32 face = r_u32(floor + 4);
        if ((r_u16(face + 6) & 0x801) == 0x800) w_u32(object + 0x10, r_u32(object + 0x10) | 0x200000);
    }
    w_u16(object + 0xb4, r_u16(floor + 8));
    w_u16(object + 0xb6, r_u16(floor + 0xa));
    w_u16(object + 0xb8, r_u16(floor + 0xc));
    w_u16(object + 0xdc, r_u16(floor + 0x10));
    w_u16(object + 0xde, r_u16(floor + 0x12));
    w_u16(object + 0xe0, r_u16(floor + 0x14));
}

uint32 G3MstrdbgClampBox3BB0(uint32 position, uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003bb0, "G3L_39");
    uint32 frame = guest_stack - 0x40;
    uint32 configuration = r_u32(object + 0x28);
    uint32 radius = r_u16(position + 6);
    uint32 property = G3MstrdbgFindExtra0AEC(configuration, module_base + 0x8208, module_base, frame);
    {
        uint32 source = property;
        uint32 destination = frame + 0x10;
        uint32 first_address = source + 0;
        uint32 first_high_address = first_address + 3;
        uint32 first_high_lane = first_high_address & 3;
        uint32 first_lane = first_address & 3;
        uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
        uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
        uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
        uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
        uint32 second_address = source + 4;
        uint32 second_high_address = second_address + 3;
        uint32 second_high_lane = second_high_address & 3;
        uint32 second_lane = second_address & 3;
        uint32 second_high = r_masked_u32(second_high_address & 0xfffffffc, (1 << (second_high_lane + 1)) - 1);
        uint32 second_low = r_masked_u32(second_address & 0xfffffffc, (15 << second_lane) & 15);
        uint32 second_keep = second_lane == 0 ? 0 : 0xffffffff << ((4 - second_lane) * 8);
        uint32 second_value = ((second_high << ((3 - second_high_lane) * 8)) & second_keep) | (second_low >> (second_lane * 8));
        uint32 third_address = source + 8;
        uint32 third_high_address = third_address + 3;
        uint32 third_high_lane = third_high_address & 3;
        uint32 third_lane = third_address & 3;
        uint32 third_high = r_masked_u32(third_high_address & 0xfffffffc, (1 << (third_high_lane + 1)) - 1);
        uint32 third_low = r_masked_u32(third_address & 0xfffffffc, (15 << third_lane) & 15);
        uint32 third_keep = third_lane == 0 ? 0 : 0xffffffff << ((4 - third_lane) * 8);
        uint32 third_value = ((third_high << ((3 - third_high_lane) * 8)) & third_keep) | (third_low >> (third_lane * 8));
        first_address = destination + 0;
        first_high_address = first_address + 3;
        first_high_lane = first_high_address & 3;
        first_lane = first_address & 3;
        w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
        w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
        second_address = destination + 4;
        second_high_address = second_address + 3;
        second_high_lane = second_high_address & 3;
        second_lane = second_address & 3;
        w_masked_u32(second_high_address & 0xfffffffc, second_value >> ((3 - second_high_lane) * 8), (1 << (second_high_lane + 1)) - 1);
        w_masked_u32(second_address & 0xfffffffc, second_value << (second_lane * 8), (15 << second_lane) & 15);
        third_address = destination + 8;
        third_high_address = third_address + 3;
        third_high_lane = third_high_address & 3;
        third_lane = third_address & 3;
        w_masked_u32(third_high_address & 0xfffffffc, third_value >> ((3 - third_high_lane) * 8), (1 << (third_high_lane + 1)) - 1);
        w_masked_u32(third_address & 0xfffffffc, third_value << (third_lane * 8), (15 << third_lane) & 15);
    }
    uint32 minimum_x = r_u16(frame + 0x10) + radius;
    w_u16(frame + 0x10, minimum_x);
    uint32 minimum_y = r_u16(frame + 0x12);
    uint32 maximum_x = r_u16(frame + 0x16);
    minimum_y += radius;
    w_u16(frame + 0x12, minimum_y);
    uint32 maximum_y = r_u16(frame + 0x18);
    maximum_x -= radius;
    w_u16(frame + 0x16, maximum_x);
    w_u16(frame + 0x18, maximum_y - radius);
    sint32 signed_x = r_s16(position);
    uint32 x = r_u16(position);
    uint32 offset_x = 0;
    uint32 offset_y = 0;
    if (signed_x < (sint16)minimum_x) offset_x = minimum_x - x;
    else if ((sint16)maximum_x < signed_x) offset_x = maximum_x - x;
    minimum_y = r_u16(frame + 0x12);
    sint32 signed_y = r_s16(position + 2);
    uint32 y = r_u16(position + 2);
    if (signed_y < (sint16)minimum_y) offset_y = minimum_y - y;
    else
    {
        sint32 signed_maximum = r_s16(frame + 0x18);
        maximum_y = r_u16(frame + 0x18);
        if (signed_maximum < signed_y) offset_y = maximum_y - y;
    }
    x = r_u16(position);
    y = r_u16(position + 2);
    w_u16(position, x + offset_x);
    w_u16(position + 2, y + offset_y);
    return (sint16)offset_x != 0 || (sint16)offset_y != 0 ? 1 : 0;
}

uint32 G3MstrdbgTraceHorizontal3D34(uint32 position, uint32 previous, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003d34, "G3L_39");
    uint32 frame = guest_stack - 0x38;
    uint32 game = r_u32(0x80097048);
    w_u32(frame + 0x10, position);
    w_u32(frame + 0x14, previous);
    w_u32(frame + 0x18, 0);
    w_u32(frame + 0x1c, 0);
    w_u32(frame + 0x20, 0);
    w_u32(frame + 0x24, 0);
    w_u32(frame + 0x28, 0);
    uint32 level = r_u32(game + 0x48);
    uint32 world = r_u32(level);
    return G3Function80063664(frame + 0x10, world, 0x1c003e0, 1, frame);
}

void G3MstrdbgResolveHorizontal3D8C(uint32 object, uint32 radius, uint32 height, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003d8c, "G3L_39");
    uint32 frame = guest_stack - 0x38;
    uint32 mode = r_u32(guest_stack + 0x10);
    {
        uint32 source = object + 0x134;
        uint32 destination = frame + 0x10;
        uint32 first_address = source + 0;
        uint32 first_high_address = first_address + 3;
        uint32 first_high_lane = first_high_address & 3;
        uint32 first_lane = first_address & 3;
        uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
        uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
        uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
        uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
        uint32 z = (uint32)r_s16(source + 4);
        first_address = destination + 0;
        first_high_address = first_address + 3;
        first_high_lane = first_high_address & 3;
        first_lane = first_address & 3;
        w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
        w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
        w_u16(destination + 4, z);
    }
    w_u16(frame + 0x1e, radius);
    {
        uint32 source = object + 0x58;
        uint32 destination = frame + 0x18;
        uint32 first_address = source + 0;
        uint32 first_high_address = first_address + 3;
        uint32 first_high_lane = first_high_address & 3;
        uint32 first_lane = first_address & 3;
        uint32 first_high = r_masked_u32(first_high_address & 0xfffffffc, (1 << (first_high_lane + 1)) - 1);
        uint32 first_low = r_masked_u32(first_address & 0xfffffffc, (15 << first_lane) & 15);
        uint32 first_keep = first_lane == 0 ? 0 : 0xffffffff << ((4 - first_lane) * 8);
        uint32 first_value = ((first_high << ((3 - first_high_lane) * 8)) & first_keep) | (first_low >> (first_lane * 8));
        uint32 z = (uint32)r_s16(source + 4);
        first_address = destination + 0;
        first_high_address = first_address + 3;
        first_high_lane = first_high_address & 3;
        first_lane = first_address & 3;
        w_masked_u32(first_high_address & 0xfffffffc, first_value >> ((3 - first_high_lane) * 8), (1 << (first_high_lane + 1)) - 1);
        w_masked_u32(first_address & 0xfffffffc, first_value << (first_lane * 8), (15 << first_lane) & 15);
        w_u16(destination + 4, z);
    }
    uint32 z = r_u16(frame + 0x1c);
    uint32 previous_z = r_u16(frame + 0x14);
    uint32 raised_z = z + height;
    w_u16(frame + 0x1c, raised_z);
    w_u16(frame + 0x14, previous_z + height);
    uint32 result = 0;
    if (mode == 0) result = G3MstrdbgTraceHorizontal3D34(frame + 0x18, frame + 0x10, frame);
    else if (mode == 1) result = G3MstrdbgClampBox3BB0(frame + 0x18, object, module_base, frame);
    if (result != 0)
    {
        uint32 x = r_u16(frame + 0x18);
        uint32 old_x = r_u16(object + 0x58);
        uint32 y = r_u16(frame + 0x1a);
        w_u16(frame + 0x20, x - old_x);
        z = r_u16(frame + 0x1c);
        uint32 old_y = r_u16(object + 0x5a);
        w_u16(frame + 0x24, z - raised_z);
        w_u16(frame + 0x22, y - old_y);
        if (G3MstrdbgFacingContact2A2C(object + 0x60, frame + 0x20, frame) != 0) w_u32(object + 0x11c, r_u32(object + 0x11c) | 0x10);
        w_u16(object + 0x58, r_u16(frame + 0x18));
        w_u16(object + 0x5a, r_u16(frame + 0x1a));
    }
}

void G3MstrdbgResolveSides3EBC(uint32 object, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003ebc, "G3L_39");
    uint32 frame = guest_stack - 0x28;
    if ((r_u32(object + 0x11c) & 0x20) != 0)
    {
        w_u32(frame + 0x10, 0);
        G3MstrdbgResolveHorizontal3D8C(object, 100, (uint32)-200, model, module_base, frame);
    }
    if ((r_u32(object + 0x11c) & 0x1000) != 0)
    {
        w_u32(frame + 0x10, 1);
        G3MstrdbgResolveHorizontal3D8C(object, 250, 400, model, module_base, frame);
    }
}

void G3MstrdbgUpdateFloor3F44(uint32 object, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003f44, "G3L_39");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    uint32 x = r_u16(object + 0x58);
    uint32 previous_x = r_u16(state + 0x2a);
    uint32 delta_x = x - previous_x;
    w_u16(frame + 0x10, delta_x);
    uint32 y = r_u16(object + 0x5a);
    uint32 previous_y = r_u16(state + 0x2c);
    uint32 delta_y = y - previous_y;
    w_u16(frame + 0x12, delta_y);
    uint32 z = r_u16(object + 0x5c);
    uint32 previous_z = r_u16(state + 0x2e);
    uint32 delta_z = z - previous_z;
    w_u16(frame + 0x14, delta_z);
    if ((sint16)delta_x != 0 || (sint16)delta_y != 0 || (sint16)delta_z != 0 || r_s16(state + 0x40) != 0 || (r_u32(state + 0x18) & 0x100) != 0)
    {
        uint32 basic = r_u32(settings);
        if (r_u32(basic) != 1)
            G3MstrdbgResolveFloor3970(object, settings, module_base, frame);
        if ((r_u32(state + 0x18) & 0x100) != 0)
            G3MstrdbgCaptureFloor38B0(object, frame);
    }
    uint32 flags = r_u32(object + 0x10);
    w_u32(object + 0x10, flags | 0x08000000);
}

void G3MstrdbgPostPhysics403C(uint32 object, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000403c, "G3L_39");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    uint32 position = r_u16(object + 0x58);
    uint32 origin = r_u16(state + 0x30);
    uint32 x = position - origin;
    w_u16(frame + 0x10, x);
    position = r_u16(object + 0x5a);
    origin = r_u16(state + 0x32);
    uint32 y = position - origin;
    w_u16(frame + 0x12, y);
    position = r_u16(object + 0x5c);
    origin = r_u16(state + 0x34);
    uint32 z = position - origin;
    w_u16(frame + 0x14, z);
    bool changed = (sint16)x != 0 || (sint16)y != 0 || (sint16)z != 0;
    if (!changed) changed = r_s16(state + 0x40) != 0;
    if (!changed) changed = (r_u32(state + 0x18) & 0x100) != 0;
    if (changed)
    {
        uint32 resource = r_u32(model);
        if (r_u32(resource) != 1) G3MstrdbgResolveSides3EBC(object, model, module_base, frame);
    }
}

void G3MstrdbgPrintCommand40F4(uint32 command, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000040f4, "G3L_39");
    uint32 frame = guest_stack - 0x18;
    uint32 kind = command & 0xff;
    if (kind >= 34) return;
    const uint32 text_offsets[34] = {0x8584, 0x858c, 0x85ac, 0x85b4, 0x0, 0x85bc, 0x85c4, 0x85cc, 0x85d8, 0x85e4, 0x85f8, 0x861c, 0x8628, 0x8638, 0x865c, 0x866c, 0x8680, 0x8694, 0x86a8, 0x86b8, 0x8630, 0x86c8, 0x86e0, 0x86f8, 0x0, 0x0, 0x0, 0x864c, 0x8654, 0x0, 0x8644, 0x863c, 0x8594, 0x860c};
    uint32 offset = text_offsets[kind];
    if (offset != 0) G3MstrdbgLiteralDiagnostic(module_base + offset, frame);
}

void G3MstrdbgPrintMode4298(uint32 mode, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004298, "G3L_39");
    uint32 frame = guest_stack - 0x18;
    uint32 kind = mode & 0xff;
    if (kind >= 19) return;
    const uint32 text_offsets[19] = {0x87a4, 0x87b0, 0x87bc, 0x87c8, 0x87d4, 0x87f0, 0x8800, 0x880c, 0x885c, 0x8818, 0x8628, 0x8824, 0x87e0, 0x8830, 0x0, 0x883c, 0x8654, 0x0, 0x884c};
    uint32 offset = text_offsets[kind];
    if (offset != 0) G3MstrdbgLiteralDiagnostic(module_base + offset, frame);
}

void G3MstrdbgPrintBehavior43AC(uint32 descriptor, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000043ac, "G3L_39");
    uint32 frame = guest_stack - 0x28;
    uint32 settings = r_u32(descriptor + 0x1c);
    if (r_u32(settings + 0x18) == 0) return;
    uint32 resource = r_u32(model);
    uint32 first = r_u32(resource + 0xc);
    uint32 second = r_u32(resource + 0x10);
    w_u8(frame + 0x18, 0);
    w_u32(frame + 0x10, first);
    w_u32(frame + 0x14, second);
    G3MstrdbgStringDiagnostic(module_base + 0x88b8, frame + 0x10, frame);
    G3MstrdbgLiteralDiagnostic(module_base + 0x88bc, frame);
}

void G3MstrdbgPrintState4414(uint32 descriptor, uint32 state, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004414, "G3L_39");
    uint32 frame = guest_stack - 0x28;
    uint32 name = r_u32(descriptor + 0x24);
    G3MstrdbgNameDiagnostic(module_base + 0x88c0, name, model, frame);
    G3MstrdbgLiteralDiagnostic(module_base + 0x88bc, frame);
    G3MstrdbgPrintBehavior43AC(descriptor, model, module_base, frame);
    G3MstrdbgPrintMode4298(r_u8(state + 1), module_base, frame);
    G3MstrdbgLiteralDiagnostic(module_base + 0x88bc, frame);
    G3MstrdbgPrintCommand40F4(r_u8(state + 2), module_base, frame);
    G3MstrdbgLiteralDiagnostic(module_base + 0x88c8, frame);
}

uint32 G3MstrdbgFindTrigger44AC(uint32 kind, uint32 animation)
{
    FUNCTION_MARKER(0x000044ac, "G3L_39");
    if (animation == 0) return 0;
    uint32 trigger = r_u32(animation + 0x14);
    for (uint32 index = 0; trigger != 0 && index < 20; ++index)
    {
        if ((uint32)r_s16(trigger) == kind) return trigger;
        trigger = r_u32(trigger + 4);
    }
    return 0;
}

void G3MstrdbgSpawnTrigger4500(uint32 trigger, uint32 object, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004500, "G3L_39");
    uint32 frame = guest_stack - 0x38;
    sint32 count = r_s16(trigger + 0xe);
    if (count <= 0) count = 1;
    uint32 resource = G3Function8003AA48(trigger + 0x10);
    if (resource == 0) return;
    for (sint32 index = 0; index < count; ++index)
    {
        uint32 child = G3Function8002A454(object, resource, frame);
        if (child != 0)
        {
            uint32 heading = r_u16(object + 0x64);
            uint32 x = r_u16(trigger + 8);
            heading -= 0x400;
            w_u16(frame + 0x10, x);
            w_u16(frame + 0x12, r_u16(trigger + 0xa));
            w_u16(frame + 0x14, r_u16(trigger + 0xc));
            G3Function8002F8E4(frame + 0x10, (uint32)(sint16)heading);
            uint32 position = r_u16(child + 0x58);
            uint32 delta = r_u16(frame + 0x10);
            w_u16(child + 0x58, position + delta);
            position = r_u16(child + 0x5a);
            delta = r_u16(frame + 0x12);
            w_u16(child + 0x5a, position + delta);
            position = r_u16(child + 0x5c);
            delta = r_u16(frame + 0x14);
            w_u16(child + 0x5c, position + delta);
            x = r_u16(child + 0x58);
            uint32 y = r_u16(child + 0x5a);
            uint32 z = r_u16(child + 0x5c);
            w_u16(child + 0x88, x);
            w_u16(child + 0x8a, y);
            w_u16(child + 0x8c, z);
        }
    }
    uint32 counter = r_u8(state + 8);
    if (counter != 0xff) w_u8(state + 8, counter + 1);
}

void G3MstrdbgPlayerFlag4644(uint32 trigger)
{
    FUNCTION_MARKER(0x00004644, "G3L_39");
    sint32 enabled = r_s16(trigger + 8);
    uint32 player = r_u32(0x8009efc4);
    uint32 flags = r_u32(player + 0x10);
    w_u32(player + 0x150, enabled != 0 ? 7 : 0);
    w_u32(player + 0x10, enabled != 0 ? flags | 0x800 : flags & 0xfffff7ff);
}

void G3MstrdbgPlayerDamage4694(uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004694, "G3L_39");
    uint32 frame = guest_stack - 0x30;
    uint32 player = r_u32(0x8009efc4);
    uint32 model = r_u32(player + 0x24);
    w_u16(model + 0x120, 0);
    w_u32(player + 0x154, r_u32(player + 0x154) & 0x39bfffff);
    G3MstrdbgInitializePacket31A0(frame + 0x10);
    player = r_u32(0x8009efc4);
    w_u16(frame + 0x20, 3);
    uint32 target = r_u32(player + 0xec);
    if (target != 0) G3DispatchStateEvent(target, player, r_u32(0x80097048), 8, frame + 0x10, frame);
}
