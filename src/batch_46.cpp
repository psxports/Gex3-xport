#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void G3HankUpdateAttachment5000(uint32 state, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00005000, "G3L_33");
    uint32 frame = guest_stack - 0x40;
    uint32 player = r_u32(0x8009efc4);
    uint32 core = r_u32(state);
    uint32 count = (uint32)r_s16(core + 0x34);
    uint32 elapsed = r_u16(core + 0x1c);
    uint32 ranges = r_u32(core + 0x38);
    w_u16(core + 0x1c, elapsed + 1);
    uint32 distance = 0;
    if (G3HankAnimationMatches45E8(state, count, ranges) != 0)
        distance = r_u16(core + 0x22);
    count = (uint32)r_s16(core + 0x36);
    ranges = r_u32(core + 0x3c);
    uint32 rotation = 0;
    if (G3HankAnimationMatches45E8(state, count, ranges) != 0)
        rotation = r_u16(core + 0x24);
    w_u32(frame + 0x10, frame + 0x20);
    uint32 arrived = G3Function80036738(state - 0x104, core + 0x26, (uint32)(sint16)distance, (uint32)(sint16)rotation, frame + 0x20, frame);
    uint32 selector = 0xffffffff;
    if (G3HankUpdateFloor2E68(state, module_base, frame) != 0)
    {
        uint32 progressed = r_u32(frame + 0x20);
        if (progressed != 0 && arrived == 0)
            selector = (uint32)r_s16(core + 0x14);
    }
    G3HankUpdateAnimation2A1C(state, selector, frame + 0x24, frame);
    uint32 mode = r_u32(state + 0x4c);
    if (mode == 0x10 || mode == 0x12 || mode == 0x13)
        return;
    if (arrived != 0)
    {
        if (r_s16(core + 0x32) != 0)
        {
            uint32 source = core + 0x2c;
            uint32 destination = core + 0x26;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 lane = source & 3;
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            uint32 z = (uint32)r_s16(source + 4);
            high_address = destination + 3;
            high_lane = high_address & 3;
            w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            lane = destination & 3;
            w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15 << lane) & 15);
            w_u16(destination + 4, z);
            w_u16(core + 0x32, 0);
        }
        else if (r_u32(frame + 0x24) != 0)
            G3HankEnterAttached3020(state, frame);
        else
            G3HankFinishAnimation2CFC(state);
    }
    uint32 coordinate = r_u16(player + 0x58);
    uint32 own = r_u16(state - 0xac);
    w_u16(frame + 0x18, coordinate - own);
    coordinate = r_u16(player + 0x5a);
    own = r_u16(state - 0xaa);
    w_u16(frame + 0x1a, coordinate - own);
    coordinate = r_u16(player + 0x5c);
    own = r_u16(state - 0xa8);
    w_u16(frame + 0x1c, coordinate - own);
    sint32 squared = (sint32)G3HankDistanceSquared47F8(state, frame + 0x18);
    uint32 settings = r_u32(state + 0x1c);
    uint32 next = 0;
    if (settings != 0)
    {
        sint32 counter = r_s16(settings + 0x3c);
        bool available = counter <= 0;
        if (!available)
            available = counter < r_s16(settings + 0x3a);
        if (available && squared < r_s32(settings + 8))
            next = 0xb;
    }
    if (next == 0)
    {
        settings = r_u32(state + 0x2c);
        if (settings != 0 && squared < r_s32(settings + 4))
            next = 0xe;
    }
    if (next != 0)
    {
        w_u16(state + 0x34, next);
        G3HankFinishAnimation2CFC(state);
    }
    sint32 pending = r_s16(state + 0x34);
    if (pending >= 0 && r_u32(frame + 0x24) != 0)
    {
        if (pending == 0xb)
            G3HankEnterMove3614(state, frame);
        else if (pending == 0xe)
            G3HankEnterRun36F4(state, frame);
        w_u16(state + 0x34, 0xffff);
    }
}

void G3HankUpdatePursuit5284(uint32 state, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00005284, "G3L_33");
    uint32 frame = guest_stack - 0x38;
    uint32 settings = r_u32(state + 8);
    uint32 elapsed = r_u16(settings + 0x10);
    uint32 attack_elapsed = r_u16(settings + 0x14);
    w_u16(settings + 0x10, elapsed + 1);
    uint32 player = r_u32(0x8009efc4);
    w_u16(settings + 0x14, attack_elapsed + 1);
    G3HankUpdateAnimation2A1C(state, 0xffffffff, frame + 0x18, frame);
    uint32 own = r_u16(state - 0xac);
    uint32 x = r_u16(player + 0x58);
    x -= own;
    w_u16(frame + 0x10, x);
    uint32 y = r_u16(player + 0x5a);
    own = r_u16(state - 0xaa);
    y -= own;
    w_u16(frame + 0x12, y);
    uint32 z = r_u16(player + 0x5c);
    own = r_u16(state - 0xa8);
    w_u16(frame + 0x14, z - own);
    uint32 heading = G3Function8002F240((uint32)(sint16)y, (uint32)(sint16)x, frame);
    uint32 count = (uint32)r_s16(settings + 0x18);
    uint32 ranges = r_u32(settings + 0x1c);
    if (G3HankAnimationMatches45E8(state, count, ranges) != 0)
    {
        uint32 step = (uint32)r_s16(settings + 0xc);
        G3Function80038AA8(state - 0xa0, (uint32)(sint16)(heading + 0x400), step);
    }
    G3HankUpdateFloor2E68(state, module_base, frame);
    uint32 mode = r_u32(state + 0x4c);
    if (mode == 0x10 || mode == 0x12 || mode == 0x13)
        return;
    if (G3HankAtOrigin486C(state, 0) != 0)
    {
        uint32 flags = r_u32(state - 0xf4);
        w_u32(state - 0xf4, flags & 0xfffffbff);
    }
    sint32 distance = (sint32)G3HankDistanceSquared47F8(state, frame + 0x10);
    uint32 current_angle = r_u16(state - 0xa0);
    uint32 difference = G3Function80038B48((uint32)(sint16)(current_angle - 0x400), (uint32)(sint16)heading);
    bool negative = (sint16)difference < 0;
    current_angle = r_u16(state - 0xa0);
    difference = G3Function80038B48((uint32)(sint16)(current_angle - 0x400), (uint32)(sint16)heading);
    uint32 absolute = negative ? 0 - difference : difference;
    uint32 requested = 0xffffffff;
    uint32 attack = r_u32(state + 0xc);
    if (attack != 0 && distance < r_s32(attack))
    {
        sint32 angle_limit = r_s16(attack + 8);
        if ((sint16)absolute < angle_limit)
        {
            sint32 elapsed = r_s16(settings + 0x14);
            sint32 interval = r_s16(attack + 0xa);
            if (elapsed >= interval)
                requested = 7;
        }
    }
    const uint32 attack_slots[3] = {0x18, 0x10, 0x14};
    const uint32 attack_modes[3] = {0xa, 8, 9};
    for (uint32 index = 0; index < 3 && requested == 0xffffffff; ++index)
    {
        attack = r_u32(state + attack_slots[index]);
        if (attack == 0)
            continue;
        sint32 maximum = r_s32(attack + 8);
        if (distance >= maximum)
            continue;
        sint32 minimum = r_s32(attack + 4);
        if (distance < minimum)
            continue;
        sint32 angle_limit = r_s16(attack + 0xe);
        if ((sint16)absolute >= angle_limit)
            continue;
        sint32 elapsed = r_s16(settings + 0x14);
        sint32 interval = r_s16(attack + 0x10);
        if (elapsed >= interval)
            requested = attack_modes[index];
    }
    if (requested == 0xffffffff)
    {
        attack = r_u32(state + 0x1c);
        if (attack != 0)
        {
            sint32 maximum = r_s32(attack + 8);
            if (distance < maximum)
            {
                sint32 minimum = r_s32(attack + 4);
                if (distance >= minimum)
                {
                    uint32 step = (uint32)r_s16(attack + 0x1a);
                    uint32 bounds = r_u32(attack + 0x34);
                    if (G3HankProbeMove4720(state, step, bounds, module_base, frame) != 0)
                        requested = 0xb;
                }
            }
        }
    }
    if (requested == 0xffffffff)
    {
        attack = r_u32(state + 0x2c);
        if (attack != 0)
        {
            sint32 maximum = r_s32(attack + 4);
            if (distance < maximum)
            {
                sint32 minimum = r_s32(attack);
                if (distance >= minimum)
                    requested = 0xe;
            }
        }
    }
    if (requested == 0xffffffff && distance >= r_s32(settings + 4))
    {
        uint32 core = r_u32(state);
        if (core != 0)
            requested = r_u32(state + 0x1c) != 0 ? 1 : 0;
        else
        {
            uint32 idle = r_u32(state + 4);
            if (idle != 0)
                requested = r_s16(idle + 0x1c) != 0 ? 3 : 2;
        }
    }
    if (requested != 0xffffffff)
    {
        w_u16(state + 0x34, requested);
        G3HankFinishAnimation2CFC(state);
    }
    if (r_s16(state + 0x34) < 0)
    {
        sint32 threshold = r_s16(settings + 0xe);
        if ((sint16)absolute < threshold)
        {
            if (r_s16(settings + 0x16) != 0)
            {
                sint32 elapsed = r_s16(settings + 0x10);
                sint32 interval = r_s16(settings + 0x12);
                if (elapsed >= interval)
                {
                    if (r_u32(frame + 0x18) == 0)
                        G3HankFinishAnimation2CFC(state);
                    else
                    {
                        uint32 animation = (uint32)r_s16(settings + 0xa);
                        G3HankSelectAnimation27EC(state, animation, 1, frame);
                        w_u16(settings + 0x16, 0);
                        w_u16(settings + 0x10, 0);
                    }
                }
            }
        }
        else
        {
            sint32 animation = r_s16(settings + 8);
            if (animation >= 0 && r_s16(settings + 0x16) == 0)
            {
                sint32 elapsed = r_s16(settings + 0x10);
                sint32 interval = r_s16(settings + 0x12);
                if (elapsed >= interval)
                {
                    if (r_u32(frame + 0x18) == 0)
                        G3HankFinishAnimation2CFC(state);
                    else
                    {
                        G3HankSelectAnimation27EC(state, (uint32)animation, 1, frame);
                        w_u16(settings + 0x16, 1);
                        w_u16(settings + 0x10, 0);
                    }
                }
            }
        }
        if (r_s16(state + 0x34) < 0)
            return;
    }
    if (r_u32(frame + 0x18) != 0)
    {
        sint32 pending = r_s16(state + 0x34);
        switch (pending)
        {
        case 0: G3HankEnterAttached3020(state, frame); break;
        case 1: G3HankEnterAttachment308C(state, frame); break;
        case 2: G3HankEnterIdle34B0(state, frame); break;
        case 3: G3HankEnterAlert3404(state, frame); break;
        case 7: G3HankEnterAttack3440(state, frame); break;
        case 8: G3HankEnterLeap356C(state, frame); break;
        case 9: G3HankEnterSpecial35A4(state, frame); break;
        case 0xa:
        {
            G3HankEnterFall35DC(state, frame);
            uint32 special = r_u32(state + 0x18);
            uint32 owner = r_u32(special);
            G3HankNotifyEnable4150(owner, frame);
            break;
        }
        case 0xb: G3HankEnterMove3614(state, frame); break;
        case 0xe: G3HankEnterRun36F4(state, frame); break;
        }
        w_u16(state + 0x34, 0xffff);
    }
}

void G3HankUpdateAlert5878(uint32 state, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00005878, "G3L_33");
    uint32 frame = guest_stack - 0x38;
    uint32 player = r_u32(0x8009efc4);
    uint32 idle = r_u32(state + 4);
    G3HankUpdateAnimation2A1C(state, 0xffffffff, frame + 0x18, frame);
    uint32 heading = (uint32)r_s16(idle + 0x1e);
    uint32 step = (uint32)r_s16(idle + 0x20);
    uint32 turned = G3Function80038AA8(state - 0xa0, heading, step);
    G3HankUpdateFloor2E68(state, module_base, frame);
    uint32 mode = r_u32(state + 0x4c);
    if (mode == 0x10 || mode == 0x12 || mode == 0x13)
        return;
    uint32 flags = r_u32(state - 0xf4);
    w_u32(state - 0xf4, flags & 0xfffffbff);
    uint32 coordinate = r_u16(player + 0x58);
    uint32 own = r_u16(state - 0xac);
    w_u16(frame + 0x10, coordinate - own);
    coordinate = r_u16(player + 0x5a);
    own = r_u16(state - 0xaa);
    w_u16(frame + 0x12, coordinate - own);
    coordinate = r_u16(player + 0x5c);
    own = r_u16(state - 0xa8);
    w_u16(frame + 0x14, coordinate - own);
    sint32 distance = (sint32)G3HankDistanceSquared47F8(state, frame + 0x10);
    uint32 settings = r_u32(state + 0x1c);
    uint32 next = 0;
    if (settings != 0)
    {
        sint32 maximum = r_s32(settings + 8);
        if (distance < maximum)
        {
            sint32 minimum = r_s32(settings + 4);
            if (distance >= minimum)
                next = 0xb;
        }
    }
    if (next == 0)
    {
        settings = r_u32(state + 0x2c);
        if (settings != 0)
        {
            sint32 maximum = r_s32(settings + 4);
            if (distance < maximum)
            {
                sint32 minimum = r_s32(settings);
                if (distance >= minimum)
                    next = 0xe;
            }
        }
    }
    if (next == 0)
    {
        settings = r_u32(state + 8);
        if (settings != 0 && distance < r_s32(settings))
            next = 6;
    }
    if (next == 0 && turned != 0)
        next = 2;
    if (next != 0)
    {
        w_u16(state + 0x34, next);
        G3HankFinishAnimation2CFC(state);
    }
    sint32 pending = r_s16(state + 0x34);
    if (pending >= 0 && r_u32(frame + 0x18) != 0)
    {
        switch (pending)
        {
        case 2: G3HankEnterIdle34B0(state, frame); break;
        case 6: G3HankEnterPursuit33BC(state, frame); break;
        case 0xb: G3HankEnterMove3614(state, frame); break;
        case 0xe: G3HankEnterRun36F4(state, frame); break;
        }
        w_u16(state + 0x34, 0xffff);
    }
}

void G3HankChooseAfterAnimation5A9C(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00005a9c, "G3L_33");
    uint32 frame = guest_stack - 0x28;
    uint32 player = r_u32(0x8009efc4);
    uint32 own = r_u16(state - 0xac);
    uint32 coordinate = r_u16(player + 0x58);
    uint32 pursuit = r_u32(state + 8);
    w_u16(frame + 0x10, coordinate - own);
    coordinate = r_u16(player + 0x5a);
    own = r_u16(state - 0xaa);
    w_u16(frame + 0x12, coordinate - own);
    coordinate = r_u16(player + 0x5c);
    own = r_u16(state - 0xa8);
    w_u16(frame + 0x14, coordinate - own);
    sint32 distance = (sint32)G3HankDistanceSquared47F8(state, frame + 0x10);
    sint32 maximum = r_s32(pursuit + 4);
    if (distance >= maximum)
    {
        if (r_u32(state) != 0)
        {
            if (r_u32(state + 0x1c) != 0)
                G3HankEnterAttachment308C(state, frame);
            else
                G3HankEnterAttached3020(state, frame);
        }
        else
        {
            uint32 idle = r_u32(state + 4);
            if (idle == 0)
                w_u32(state + 0x4c, 0x16);
            else if (r_s16(idle + 0x1c) != 0)
                G3HankEnterAlert3404(state, frame);
            else
                G3HankEnterIdle34B0(state, frame);
        }
        return;
    }
    uint32 settings = r_u32(state + 0x1c);
    if (settings != 0)
    {
        maximum = r_s32(settings + 8);
        if (distance < maximum)
        {
            sint32 minimum = r_s32(settings + 4);
            if (distance >= minimum)
            {
                G3HankEnterMove3614(state, frame);
                return;
            }
        }
    }
    settings = r_u32(state + 0x2c);
    if (settings != 0)
    {
        maximum = r_s32(settings + 4);
        if (distance < maximum)
        {
            sint32 minimum = r_s32(settings);
            if (distance >= minimum)
            {
                G3HankEnterRun36F4(state, frame);
                return;
            }
        }
    }
    G3HankEnterPursuit33BC(state, frame);
}

void G3HankUpdateWait5C44(uint32 state, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00005c44, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    G3HankUpdateAnimation2A1C(state, 0xffffffff, 0, frame);
    G3HankUpdateFloor2E68(state, module_base, frame);
    uint32 mode = r_u32(state + 0x4c);
    if (mode == 0x10 || mode == 0x12 || mode == 0x13)
        return;
    if (G3HankAnimationReady2CC4(state) != 0)
        G3HankChooseAfterAnimation5A9C(state, frame);
}

void G3HankUpdateIdle5CB0(uint32 state, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00005cb0, "G3L_33");
    uint32 frame = guest_stack - 0x38;
    uint32 idle = r_u32(state + 4);
    uint32 elapsed = r_u16(idle + 0x12);
    uint32 duration = r_u16(idle + 0x16);
    w_u16(idle + 0x12, elapsed + 1);
    uint32 player = r_u32(0x8009efc4);
    w_u16(idle + 0x16, duration + 1);
    G3HankUpdateFloor2E68(state, module_base, frame);
    uint32 mode = r_u32(state + 0x4c);
    if (mode == 0x10 || mode == 0x12 || mode == 0x13)
        return;
    G3HankUpdateAnimation2A1C(state, 0xffffffff, frame + 0x18, frame);
    if (G3HankAtOrigin486C(state, 0) != 0)
    {
        uint32 flags = r_u32(state - 0xf4);
        w_u32(state - 0xf4, flags & 0xfffffbff);
    }
    uint32 coordinate = r_u16(player + 0x58);
    uint32 own = r_u16(state - 0xac);
    w_u16(frame + 0x10, coordinate - own);
    coordinate = r_u16(player + 0x5a);
    own = r_u16(state - 0xaa);
    w_u16(frame + 0x12, coordinate - own);
    coordinate = r_u16(player + 0x5c);
    own = r_u16(state - 0xa8);
    w_u16(frame + 0x14, coordinate - own);
    sint32 distance = (sint32)G3HankDistanceSquared47F8(state, frame + 0x10);
    uint32 settings = r_u32(state + 0x1c);
    uint32 next = 0;
    if (settings != 0)
    {
        sint32 maximum = r_s32(settings + 8);
        if (distance < maximum)
        {
            sint32 minimum = r_s32(settings + 4);
            if (distance >= minimum)
                next = 0xb;
        }
    }
    if (next == 0)
    {
        settings = r_u32(state + 0x2c);
        if (settings != 0)
        {
            sint32 maximum = r_s32(settings + 4);
            if (distance < maximum)
            {
                sint32 minimum = r_s32(settings);
                if (distance >= minimum)
                    next = 0xe;
            }
        }
    }
    if (next == 0)
    {
        settings = r_u32(state + 8);
        if (settings != 0 && distance < r_s32(settings))
            next = 6;
    }
    if (next != 0)
    {
        w_u16(state + 0x34, next);
        G3HankFinishAnimation2CFC(state);
    }
    if (r_s16(state + 0x34) < 0)
    {
        if (r_s16(idle + 0x1a) != 0)
        {
            sint32 elapsed = r_s16(idle + 0x16);
            sint32 limit = r_s16(idle + 0x18);
            if (elapsed >= limit)
            {
                if (r_u32(state) != 0)
                {
                    w_u16(state + 0x34, 0);
                    G3HankFinishAnimation2CFC(state);
                }
                else
                    w_u16(idle + 0x16, 0);
            }
            else
            {
                elapsed = r_s16(idle + 0x12);
                limit = r_s16(idle + 0x14);
                if (elapsed >= limit)
                {
                    uint32 random = G3SdkRandom80027FA8();
                    sint32 sample = (sint32)(random & 0xfff);
                    uint32 selected = 0;
                    for (uint32 index = 0; index < 3; ++index)
                    {
                        sint32 threshold = r_s16(idle + 0xc + index * 2);
                        if (sample < threshold)
                        {
                            selected = index + 1;
                            break;
                        }
                    }
                    if (selected != 0)
                    {
                        uint32 animation = (uint32)r_s16(idle + selected * 2);
                        G3HankSelectAnimation27EC(state, animation, 1, frame);
                        w_u16(idle + 0x1a, 0);
                    }
                    w_u16(idle + 0x12, 0);
                }
            }
        }
        else if (G3HankAnimationReady2CC4(state) != 0)
        {
            sint32 elapsed = r_s16(idle + 0x16);
            sint32 limit = r_s16(idle + 0x18);
            if (elapsed >= limit)
            {
                if (r_u32(state) != 0)
                    G3HankEnterAttached3020(state, frame);
                else
                {
                    uint32 animation = (uint32)r_s16(idle);
                    G3HankSelectAnimation27EC(state, animation, 1, frame);
                    w_u16(idle + 0x12, 0);
                    w_u16(idle + 0x16, 0);
                    w_u16(idle + 0x1a, 1);
                }
            }
            else
            {
                uint32 random = G3SdkRandom80027FA8();
                sint32 sample = (sint32)(random & 0xfff);
                uint32 selected = 0;
                for (uint32 index = 0; index < 3; ++index)
                {
                    sint32 threshold = r_s16(idle + 0xc + index * 2);
                    if (sample < threshold)
                    {
                        selected = index + 1;
                        break;
                    }
                }
                if (selected != 0)
                {
                    uint32 animation = (uint32)r_s16(idle + selected * 2);
                    G3HankSelectAnimation27EC(state, animation, 1, frame);
                    w_u16(idle + 0x1a, 0);
                }
                else
                {
                    uint32 animation = (uint32)r_s16(idle);
                    G3HankSelectAnimation27EC(state, animation, 1, frame);
                    w_u16(idle + 0x12, 0);
                    w_u16(idle + 0x1a, 1);
                }
            }
        }
        if (r_s16(state + 0x34) < 0)
            return;
    }
    if (r_u32(frame + 0x18) != 0)
    {
        sint32 pending = r_s16(state + 0x34);
        switch (pending)
        {
        case 0: G3HankEnterAttached3020(state, frame); break;
        case 6: G3HankEnterPursuit33BC(state, frame); break;
        case 0xb: G3HankEnterMove3614(state, frame); break;
        case 0xe: G3HankEnterRun36F4(state, frame); break;
        }
        w_u16(state + 0x34, 0xffff);
    }
}

void G3HankUpdateShooter60C8(uint32 state, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000060c8, "G3L_33");
    uint32 frame = guest_stack - 0x30;
    uint32 settings = r_u32(state + 0x10);
    G3HankUpdateFloor2E68(state, module_base, frame);
    uint32 mode = r_u32(state + 0x4c);
    if (mode == 0x10 || mode == 0x12 || mode == 0x13)
        return;
    G3HankUpdateAnimation2A1C(state, 0xffffffff, 0, frame);
    if (r_s16(settings + 0x2c) == 0)
    {
        uint32 script = r_u32(state + 0x28);
        bool ready = true;
        if (script != 0 && r_u32(script + 4) != 0)
        {
            sint32 current = r_s16(script + 0xe);
            sint32 trigger = r_s16(script + 0x20);
            ready = current == trigger;
        }
        if (ready)
        {
            sint32 phase = r_s16(state - 0xa6);
            sint32 trigger = r_s16(settings + 0x28);
            if (phase >= trigger)
            {
                uint32 resource = r_u32(settings);
                w_u16(settings + 0x2c, 1);
                uint32 spawned = G3Function8002A454(state - 0x104, resource, frame);
                if (spawned != 0)
                {
                    sint32 bone = r_s16(settings + 0x2a);
                    uint32 matrices = r_u32(state - 0xd4);
                    uint32 matrix = matrices + (uint32)bone * 0x20;
                    for (uint32 row = 0; row < 3; ++row)
                    {
                        sint32 x = r_s16(settings + 0x22);
                        sint32 xx = r_s16(matrix + row * 6);
                        sint32 y = r_s16(settings + 0x24);
                        sint32 yy = r_s16(matrix + row * 6 + 2);
                        sint32 z = r_s16(settings + 0x26);
                        sint32 zz = r_s16(matrix + row * 6 + 4);
                        sint32 value = ((x * xx) >> 12) + ((y * yy) >> 12) + ((z * zz) >> 12);
                        w_u16(spawned + 0x58 + row * 2, (uint32)value);
                    }
                    for (uint32 axis = 0; axis < 3; ++axis)
                    {
                        uint32 value = r_u16(spawned + 0x58 + axis * 2);
                        uint32 translation = r_u16(matrix + 0x14 + axis * 4);
                        w_u16(spawned + 0x58 + axis * 2, value + translation);
                    }
                    for (uint32 half = 0; half < 8; ++half)
                    {
                        uint32 value = r_u16(settings + 0x12 + half * 2);
                        w_u16(frame + 0x10 + half * 2, value);
                    }
                    uint32 target = r_u32(spawned + 0xec);
                    if (target != 0)
                        G3DispatchStateEvent(target, spawned, game, 0x7e1, frame + 0x10, frame);
                }
            }
        }
    }
    if (G3HankAnimationReady2CC4(state) != 0)
        G3HankChooseAfterAnimation5A9C(state, frame);
}

void G3HankPlayerOffset6388(uint32 output, uint32 scale, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006388, "G3L_33");
    uint32 player = r_u32(0x8009efc4);
    uint32 descriptor = r_u32(player + 0x24);
    sint32 index = r_s16(descriptor + 0xe0);
    uint32 values = r_u32(descriptor + 0x2c);
    sint32 coefficient = r_s16(values + (uint32)index * 2);
    uint32 angle_half = r_u16(player + 0x64);
    uint32 angle = (uint32)(sint16)(angle_half - 0x400);
    sint32 distance = (coefficient * (sint32)(sint16)scale) >> 8;
    uint32 cosine = G3Function8005B470(angle);
    uint32 product = (uint32)distance * cosine;
    player = r_u32(0x8009efc4);
    uint32 origin = r_u16(player + 0xdc);
    w_u16(output, origin + (uint32)((sint32)product >> 12));
    uint32 sine = G3Function8005B590(angle);
    product = (uint32)distance * sine;
    player = r_u32(0x8009efc4);
    origin = r_u16(player + 0xde);
    w_u16(output + 2, origin + (uint32)((sint32)product >> 12));
    origin = r_u16(player + 0xe0);
    w_u16(output + 4, origin);
}

void G3HankUpdateLeap6460(uint32 state, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006460, "G3L_33");
    uint32 frame = guest_stack - 0x58;
    uint32 settings = r_u32(state + 0x14);
    uint32 pursuit = r_u32(state + 8);
    G3HankUpdateFloor2E68(state, module_base, frame);
    uint32 mode = r_u32(state + 0x4c);
    if (mode == 0x10 || mode == 0x12 || mode == 0x13)
        return;
    G3HankUpdateAnimation2A1C(state, 0xffffffff, 0, frame);
    if (r_s16(settings + 0x20) == 0)
    {
        uint32 script = r_u32(state + 0x28);
        bool ready = true;
        if (script != 0 && r_u32(script + 4) != 0)
        {
            sint32 current = r_s16(script + 0xe);
            sint32 trigger = r_s16(script + 0x20);
            ready = current == trigger;
        }
        if (ready)
        {
            uint32 player = r_u32(0x8009efc4);
            uint32 own = r_u16(state - 0xac);
            uint32 x = r_u16(player + 0x58);
            x -= own;
            w_u16(frame + 0x20, x);
            uint32 y = r_u16(player + 0x5a);
            own = r_u16(state - 0xaa);
            y -= own;
            w_u16(frame + 0x22, y);
            uint32 z = r_u16(player + 0x5c);
            own = r_u16(state - 0xa8);
            w_u16(frame + 0x24, z - own);
            uint32 heading = G3Function8002F240((uint32)(sint16)y, (uint32)(sint16)x, frame);
            uint32 step = (uint32)r_s16(pursuit + 0xc);
            G3Function80038AA8(state - 0xa0, (uint32)(sint16)(heading + 0x400), step);
            sint32 phase = r_s16(state - 0xa6);
            sint32 trigger = r_s16(settings + 0x18);
            if (phase >= trigger)
            {
                uint32 resource = r_u32(settings);
                w_u16(settings + 0x20, 1);
                if (resource != 0)
                {
                    uint32 spawned = G3Function8002A454(state - 0x104, resource, frame);
                    if (spawned != 0)
                    {
                        sint32 bone = r_s16(settings + 0x1a);
                        uint32 matrices = r_u32(state - 0xd4);
                        uint32 matrix = matrices + (uint32)bone * 0x20;
                        for (uint32 row = 0; row < 3; ++row)
                        {
                            sint32 x = r_s16(settings + 0x12);
                            sint32 xx = r_s16(matrix + row * 6);
                            sint32 y = r_s16(settings + 0x14);
                            sint32 yy = r_s16(matrix + row * 6 + 2);
                            sint32 z = r_s16(settings + 0x16);
                            sint32 zz = r_s16(matrix + row * 6 + 4);
                            sint32 value = ((x * xx) >> 12) + ((y * yy) >> 12) + ((z * zz) >> 12);
                            w_u16(spawned + 0x58 + row * 2, (uint32)value);
                        }
                        for (uint32 axis = 0; axis < 3; ++axis)
                        {
                            uint32 value = r_u16(spawned + 0x58 + axis * 2);
                            uint32 translation = r_u16(matrix + 0x14 + axis * 4);
                            w_u16(spawned + 0x58 + axis * 2, value + translation);
                        }
                        w_u16(frame + 0x32, 0);
                        w_u16(frame + 0x30, 0);
                        uint32 value = r_u16(settings + 0x1e);
                        w_u16(frame + 0x34, value);
                        uint32 target = r_u32(spawned + 0xec);
                        if (target != 0)
                            G3DispatchStateEvent(target, spawned, game, 0x7e0, frame + 0x30, frame);
                        uint32 scale = (uint32)r_s16(settings + 0x26);
                        G3HankPlayerOffset6388(frame + 0x28, scale, frame);
                        uint32 speed = (uint32)r_s16(settings + 0x1c);
                        uint32 gravity = (uint32)r_s16(settings + 0x1e);
                        uint32 minimum = (uint32)r_s16(settings + 0x22);
                        w_u32(frame + 0x10, minimum);
                        uint32 maximum = (uint32)r_s16(settings + 0x24);
                        w_u32(frame + 0x18, frame + 0x3a);
                        w_u32(frame + 0x1c, frame + 0x38);
                        w_u32(frame + 0x14, maximum);
                        if (G3Function80036B80(spawned + 0x58, frame + 0x28, speed, gravity, minimum, maximum, frame + 0x3a, frame + 0x38, frame) == 0)
                        {
                            w_u16(frame + 0x38, 0);
                            w_u16(frame + 0x3a, 500);
                        }
                        sint32 destination_y = r_s16(frame + 0x2a);
                        sint32 source_y = r_s16(spawned + 0x5a);
                        sint32 destination_x = r_s16(frame + 0x28);
                        sint32 source_x = r_s16(spawned + 0x58);
                        heading = G3Function8002F240((uint32)(destination_y - source_y), (uint32)(destination_x - source_x), frame);
                        target = r_u32(spawned + 0xec);
                        w_u16(spawned + 0x64, heading + 0x400);
                        if (target != 0)
                        {
                            G3DispatchStateEvent(target, spawned, game, 0x7e9, frame + 0x38, frame);
                            target = r_u32(spawned + 0xec);
                            if (target != 0)
                            {
                                uint32 payload = (uint32)r_s16(settings + 0x28);
                                G3DispatchStateEvent(target, spawned, game, 0x7d8, payload, frame);
                                target = r_u32(spawned + 0xec);
                                if (target != 0)
                                {
                                    payload = (uint32)r_s16(settings + 0x2a);
                                    G3DispatchStateEvent(target, spawned, game, 0x7f0, payload, frame);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (G3HankAnimationReady2CC4(state) != 0)
        G3HankChooseAfterAnimation5A9C(state, frame);
}

void G3HankUpdateFall6828(uint32 state, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006828, "G3L_33");
    uint32 frame = guest_stack - 0x40;
    uint32 settings = r_u32(state + 0x18);
    uint32 player = r_u32(0x8009efc4);
    uint32 elapsed = r_u16(settings + 0x14);
    w_u16(settings + 0x14, elapsed + 1);
    G3HankUpdateFloor2E68(state, module_base, frame);
    uint32 mode = r_u32(state + 0x4c);
    if (mode == 0x10 || mode == 0x12 || mode == 0x13)
        return;
    G3HankUpdateAnimation2A1C(state, 0xffffffff, frame + 0x20, frame);
    uint32 owner = r_u32(settings);
    uint32 object = r_u32(owner + 0x20);
    if (object != 0)
    {
        uint32 target = r_u32(object + 0xec);
        if (target != 0)
            G3DispatchStateEvent(target, object, game, 0x3e8, frame + 0x10, frame);
        uint32 x = r_u16(frame + 0x10);
        uint32 own = r_u16(state - 0xac);
        uint32 y = r_u16(frame + 0x12);
        x -= own;
        w_u16(frame + 0x18, x);
        own = r_u16(state - 0xaa);
        y -= own;
        w_u16(frame + 0x1a, y);
        uint32 z = r_u16(frame + 0x14);
        own = r_u16(state - 0xa8);
        w_u16(frame + 0x1c, z - own);
        uint32 heading = G3Function8002F240((uint32)(sint16)y, (uint32)(sint16)x, frame);
        w_u16(state - 0xa0, heading + 0x400);
    }
    uint32 coordinate = r_u16(player + 0x58);
    uint32 own = r_u16(state - 0xac);
    w_u16(frame + 0x18, coordinate - own);
    coordinate = r_u16(player + 0x5a);
    own = r_u16(state - 0xaa);
    w_u16(frame + 0x1a, coordinate - own);
    coordinate = r_u16(player + 0x5c);
    own = r_u16(state - 0xa8);
    w_u16(frame + 0x1c, coordinate - own);
    sint32 distance = (sint32)G3HankDistanceSquared47F8(state, frame + 0x18);
    sint32 limit = r_s16(settings + 0x12);
    bool finish = false;
    if (limit != 0)
        finish = r_s16(settings + 0x14) >= limit;
    if (!finish)
    {
        sint32 maximum = r_s32(settings + 8);
        finish = distance >= maximum;
        if (!finish)
            finish = distance < r_s32(settings + 4);
    }
    if (finish)
    {
        w_u16(state + 0x34, 6);
        G3HankFinishAnimation2CFC(state);
    }
    if (r_s16(state + 0x34) >= 0)
    {
        G3HankEnterPursuit33BC(state, frame);
        owner = r_u32(settings);
        G3HankNotifyDisable41A4(owner, frame);
        w_u16(state + 0x34, 0xffff);
    }
}

void G3HankUpdateMove69F4(uint32 state, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000069f4, "G3L_33");
    uint32 frame = guest_stack - 0x48;
    uint32 player = r_u32(0x8009efc4);
    uint32 settings = r_u32(state + 0x1c);
    uint32 count = (uint32)r_s16(settings + 0x28);
    uint32 elapsed = r_u16(settings + 0x3a);
    uint32 ranges = r_u32(settings + 0x2c);
    w_u16(settings + 0x3a, elapsed + 1);
    uint32 enabled = G3HankAnimationMatches45E8(state, count, ranges);
    uint32 distance_step = 0;
    if (enabled != 0)
        distance_step = r_u16(settings + 0x1a);
    count = (uint32)r_s16(settings + 0x2a);
    ranges = r_u32(settings + 0x30);
    uint32 turn = 0;
    if (G3HankAnimationMatches45E8(state, count, ranges) != 0)
        turn = r_u16(settings + 0x1c);
    w_u32(frame + 0x10, 0);
    G3Function80036988(state - 0x104, player + 0x58, (uint32)(sint16)distance_step, (uint32)(sint16)turn, 0, frame);
    uint32 clamped = 0;
    bool advance = false;
    if (G3HankUpdateFloor2E68(state, module_base, frame) != 0 && enabled != 0)
    {
        uint32 bounds = r_u32(settings + 0x34);
        clamped = G3HankClampBounds4204(state, bounds);
        advance = clamped == 0;
    }
    if (advance)
    {
        uint32 selector = (uint32)r_s16(settings + 0x18);
        G3HankUpdateAnimation2A1C(state, selector, frame + 0x20, frame);
    }
    else
    {
        G3HankUpdateAnimation2A1C(state, 0xffffffff, frame + 0x20, frame);
        if (enabled != 0)
        {
            if (r_s16(settings + 0x1e) != 0)
            {
                G3HankChooseDeath4520(state, 1, 1, module_base, frame);
                return;
            }
            if (r_u32(state + 8) != 0)
            {
                w_u16(state + 0x34, 6);
                G3HankFinishAnimation2CFC(state);
            }
        }
    }
    uint32 mode = r_u32(state + 0x4c);
    if (mode == 0x10 || mode == 0x12 || mode == 0x13)
        return;
    if (clamped != 0)
    {
        uint32 core = r_u32(state);
        if (core != 0)
        {
            if (r_s16(core + 0x40) > 0)
            {
                G3HankEnterAttachment308C(state, frame);
                w_u16(state + 0x34, 0xffff);
                return;
            }
        }
        else if (r_u32(state + 4) != 0 && r_s16(settings + 0x3c) > 0)
        {
            G3HankEnterWait36BC(state, frame);
            w_u16(state + 0x34, 0xffff);
            return;
        }
    }
    uint32 coordinate = r_u16(player + 0x58);
    uint32 own = r_u16(state - 0xac);
    w_u16(frame + 0x18, coordinate - own);
    coordinate = r_u16(player + 0x5a);
    own = r_u16(state - 0xaa);
    w_u16(frame + 0x1a, coordinate - own);
    coordinate = r_u16(player + 0x5c);
    own = r_u16(state - 0xa8);
    w_u16(frame + 0x1c, coordinate - own);
    sint32 distance = (sint32)G3HankDistanceSquared47F8(state, frame + 0x18);
    sint32 minimum = r_s32(settings);
    uint32 requested = 0xffffffff;
    bool check_pursuit;
    if (distance < minimum)
    {
        sint32 animation = r_s16(settings + 0x12);
        if (animation < 0)
            check_pursuit = true;
        else
        {
            sint32 elapsed = r_s16(settings + 0x3a);
            sint32 interval = r_s16(settings + 0x38);
            if (elapsed >= interval)
            {
                requested = 0xc;
                check_pursuit = false;
            }
            else
                check_pursuit = r_s16(settings + 0x12) < 0;
        }
    }
    else
        check_pursuit = r_s16(settings + 0x12) < 0;
    if (requested == 0xffffffff && check_pursuit && distance < r_s32(settings + 4))
    {
        uint32 pursuit = r_u32(state + 8);
        if (pursuit != 0 && distance < r_s32(pursuit))
            requested = 6;
    }
    if (requested == 0xffffffff && distance >= r_s32(settings + 0xc))
    {
        coordinate = r_u16(player + 0x58);
        own = r_u16(state - 0xac);
        w_u16(frame + 0x18, coordinate - own);
        coordinate = r_u16(player + 0x5a);
        own = r_u16(state - 0xaa);
        w_u16(frame + 0x1a, coordinate - own);
        coordinate = r_u16(player + 0x5c);
        own = r_u16(state - 0xa8);
        w_u16(frame + 0x1c, coordinate - own);
        distance = (sint32)G3HankDistanceSquared47F8(state, frame + 0x18);
        uint32 pursuit = r_u32(state + 8);
        if (pursuit != 0 && distance < r_s32(pursuit))
            requested = 6;
        else if (r_u32(state) != 0)
            requested = 1;
        else
        {
            uint32 idle = r_u32(state + 4);
            if (idle != 0)
                requested = r_s16(settings + 0x20) != 0 ? 4 : 2;
            else if (r_u32(state + 8) != 0)
                requested = 6;
        }
    }
    if (requested != 0xffffffff)
    {
        w_u16(state + 0x34, requested);
        G3HankFinishAnimation2CFC(state);
    }
    if (r_s16(settings + 0x14) >= 0 && distance < r_s32(settings + 4))
    {
        uint32 current_player = r_u32(0x8009efc4);
        uint32 flags = r_u32(current_player + 0x154);
        if ((flags & 0x10) != 0)
        {
            G3HankEnterTurn3688(state, frame);
            w_u16(state + 0x34, 0xffff);
            return;
        }
    }
    sint32 pending = r_s16(state + 0x34);
    uint32 pending_half = r_u16(state + 0x34);
    if (pending < 0 || r_u32(frame + 0x20) == 0)
        return;
    switch ((sint16)(pending_half - 1))
    {
    case 0: G3HankEnterAttachment308C(state, frame); break;
    case 1: G3HankEnterIdle34B0(state, frame); break;
    case 3: G3HankEnterWait36BC(state, frame); break;
    case 5: G3HankEnterPursuit33BC(state, frame); break;
    case 0xb: G3HankEnterStop3654(state, frame); break;
    case 0xc: G3HankEnterTurn3688(state, frame); break;
    }
    w_u16(state + 0x34, 0xffff);
}

void G3HankUpdateStop6E40(uint32 state, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006e40, "G3L_33");
    uint32 frame = guest_stack - 0x38;
    uint32 settings = r_u32(state + 0x1c);
    uint32 player = r_u32(0x8009efc4);
    uint32 count = (uint32)r_s16(settings + 0x28);
    uint32 ranges = r_u32(settings + 0x2c);
    uint32 enabled = G3HankAnimationMatches45E8(state, count, ranges);
    uint32 distance_step = 0;
    if (enabled != 0)
        distance_step = r_u16(settings + 0x1a);
    count = (uint32)r_s16(settings + 0x2a);
    ranges = r_u32(settings + 0x30);
    uint32 turn = 0;
    if (G3HankAnimationMatches45E8(state, count, ranges) != 0)
        turn = r_u16(settings + 0x1c);
    w_u32(frame + 0x10, 0);
    G3Function80036988(state - 0x104, player + 0x58, (uint32)(sint16)distance_step, (uint32)(sint16)turn, 0, frame);
    uint32 selector = 0xffffffff;
    if (G3HankUpdateFloor2E68(state, module_base, frame) != 0 && enabled != 0)
    {
        uint32 bounds = r_u32(settings + 0x34);
        if (G3HankClampBounds4204(state, bounds) == 0)
            selector = (uint32)r_s16(settings + 0x18);
    }
    G3HankUpdateAnimation2A1C(state, selector, 0, frame);
    uint32 mode = r_u32(state + 0x4c);
    if (mode == 0x10 || mode == 0x12 || mode == 0x13)
        return;
    uint32 coordinate = r_u16(player + 0x58);
    uint32 own = r_u16(state - 0xac);
    w_u16(frame + 0x18, coordinate - own);
    coordinate = r_u16(player + 0x5a);
    own = r_u16(state - 0xaa);
    w_u16(frame + 0x1a, coordinate - own);
    coordinate = r_u16(player + 0x5c);
    own = r_u16(state - 0xa8);
    w_u16(frame + 0x1c, coordinate - own);
    sint32 distance = (sint32)G3HankDistanceSquared47F8(state, frame + 0x18);
    if (G3HankAnimationReady2CC4(state) != 0)
    {
        if (r_s16(settings + 0x1e) != 0)
            G3HankChooseDeath4520(state, 1, 1, module_base, frame);
        else
        {
            uint32 pursuit = r_u32(state + 8);
            if (pursuit != 0 && distance < r_s32(pursuit) && distance < r_s32(settings + 4))
                G3HankEnterPursuit33BC(state, frame);
            else
                G3HankEnterMove3614(state, frame);
        }
    }
}

void G3HankUpdateTurn7018(uint32 state, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007018, "G3L_33");
    uint32 frame = guest_stack - 0x30;
    uint32 settings = r_u32(state + 0x1c);
    uint32 player = r_u32(0x8009efc4);
    uint32 count = (uint32)r_s16(settings + 0x28);
    uint32 ranges = r_u32(settings + 0x2c);
    uint32 enabled = G3HankAnimationMatches45E8(state, count, ranges);
    uint32 distance_step = 0;
    if (enabled != 0)
        distance_step = r_u16(settings + 0x1a);
    count = (uint32)r_s16(settings + 0x2a);
    ranges = r_u32(settings + 0x30);
    uint32 turn = 0;
    if (G3HankAnimationMatches45E8(state, count, ranges) != 0)
        turn = r_u16(settings + 0x1c);
    w_u32(frame + 0x10, 0);
    G3Function80036988(state - 0x104, player + 0x58, (uint32)(sint16)distance_step, (uint32)(sint16)turn, 0, frame);
    uint32 selector = 0xffffffff;
    if (G3HankUpdateFloor2E68(state, module_base, frame) != 0 && enabled != 0)
    {
        uint32 bounds = r_u32(settings + 0x34);
        if (G3HankClampBounds4204(state, bounds) == 0)
            selector = (uint32)r_s16(settings + 0x18);
    }
    G3HankUpdateAnimation2A1C(state, selector, 0, frame);
    uint32 mode = r_u32(state + 0x4c);
    if (mode == 0x10 || mode == 0x12 || mode == 0x13)
        return;
    if (G3HankAnimationReady2CC4(state) != 0)
        G3HankEnterMove3614(state, frame);
}

void G3HankUpdateReturn7140(uint32 state, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007140, "G3L_33");
    uint32 frame = guest_stack - 0x40;
    uint32 player = r_u32(0x8009efc4);
    uint32 settings = r_u32(state + 0x1c);
    uint32 count = (uint32)r_s16(settings + 0x28);
    uint32 elapsed = r_u16(settings + 0x3a);
    uint32 ranges = r_u32(settings + 0x2c);
    w_u16(settings + 0x3a, elapsed + 1);
    uint32 distance_step = 0;
    if (G3HankAnimationMatches45E8(state, count, ranges) != 0)
        distance_step = r_u16(settings + 0x1a);
    count = (uint32)r_s16(settings + 0x2a);
    ranges = r_u32(settings + 0x30);
    uint32 turn = 0;
    if (G3HankAnimationMatches45E8(state, count, ranges) != 0)
        turn = r_u16(settings + 0x1c);
    w_u32(frame + 0x10, frame + 0x20);
    uint32 arrived = G3Function80036738(state - 0x104, settings + 0x22, (uint32)(sint16)distance_step, (uint32)(sint16)turn, frame + 0x20, frame);
    uint32 selector = 0xffffffff;
    if (G3HankUpdateFloor2E68(state, module_base, frame) != 0)
    {
        uint32 progressed = r_u32(frame + 0x20);
        if (progressed != 0 && arrived == 0)
            selector = (uint32)r_s16(settings + 0x18);
    }
    G3HankUpdateAnimation2A1C(state, selector, frame + 0x24, frame);
    uint32 mode = r_u32(state + 0x4c);
    if (mode == 0x10 || mode == 0x12 || mode == 0x13)
        return;
    uint32 coordinate = r_u16(player + 0x58);
    uint32 own = r_u16(state - 0xac);
    w_u16(frame + 0x18, coordinate - own);
    coordinate = r_u16(player + 0x5a);
    own = r_u16(state - 0xaa);
    w_u16(frame + 0x1a, coordinate - own);
    coordinate = r_u16(player + 0x5c);
    own = r_u16(state - 0xa8);
    w_u16(frame + 0x1c, coordinate - own);
    sint32 distance = (sint32)G3HankDistanceSquared47F8(state, frame + 0x18);
    sint32 interval = r_s16(settings + 0x3c);
    bool available = interval <= 0;
    if (!available)
        available = interval < r_s16(settings + 0x3a);
    uint32 requested = 0;
    if (available && distance < r_s32(settings + 8))
        requested = 0xb;
    else if (arrived != 0)
    {
        uint32 idle = r_u32(state + 4);
        if (idle == 0)
        {
            w_u32(state + 0x4c, 0x16);
            return;
        }
        requested = r_s16(idle + 0x1c) != 0 ? 3 : 2;
    }
    if (requested != 0)
    {
        w_u16(state + 0x34, requested);
        G3HankFinishAnimation2CFC(state);
    }
    sint32 pending = r_s16(state + 0x34);
    if (pending >= 0 && r_u32(frame + 0x24) != 0)
    {
        switch (pending)
        {
        case 2: G3HankEnterIdle34B0(state, frame); break;
        case 3: G3HankEnterAlert3404(state, frame); break;
        case 0xb: G3HankEnterMove3614(state, frame); break;
        }
        w_u16(state + 0x34, 0xffff);
    }
}

void G3HankUpdateRun7394(uint32 state, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007394, "G3L_33");
    uint32 player = r_u32(0x8009efc4);
    uint32 frame = guest_stack - 0x50;
    uint32 own = r_u16(state - 0xac);
    uint32 coordinate = r_u16(player + 0x58);
    sint32 x = (sint16)(own - coordinate);
    own = r_u16(state - 0xaa);
    coordinate = r_u16(player + 0x5a);
    sint32 y = (sint16)(own - coordinate);
    uint32 settings = r_u32(state + 0x2c);
    uint32 squared = (uint32)(x * x) + (uint32)(y * y);
    uint32 length = G3Function8002F448(squared, 0, frame);
    sint32 divisor = (sint16)length;
    uint32 dx;
    uint32 dy;
    if (divisor == 0)
    {
        uint32 angle = (uint32)(sint16)(r_u16(state - 0xa0) - 0x400);
        uint32 cosine = G3Function8005B470(angle);
        dx = (cosine * 625) >> 11;
        uint32 sine = G3Function8005B590(angle);
        dy = (sine * 625) >> 11;
    }
    else
    {
        dx = (uint32)((x * 1250) / divisor);
        dy = (uint32)((y * 1250) / divisor);
    }
    uint32 bounds = r_u32(settings + 0x18);
    if (bounds != 0)
    {
        uint32 lower_x = r_u16(bounds);
        uint32 own_x = r_u16(state - 0xac);
        uint32 lower_y = r_u16(bounds + 2);
        uint32 own_y = r_u16(state - 0xaa);
        lower_x -= own_x;
        lower_y -= own_y;
        uint32 upper_x = r_u16(bounds + 6);
        uint32 upper_y = r_u16(bounds + 8);
        upper_x -= own_x;
        upper_y -= own_y;
        if ((((lower_x - dx) ^ (upper_x - dx)) & ((lower_y - dy) ^ (upper_y - dy)) & 0x8000) == 0)
        {
            if ((((lower_x + dy) ^ (upper_x + dy)) & ((lower_y - dx) ^ (upper_y - dx)) & 0x8000) != 0)
            {
                uint32 previous_x = dx;
                dx = 0 - dy;
                dy = previous_x;
            }
            else if ((((lower_x - dy) ^ (upper_x - dy)) & ((lower_y + dx) ^ (upper_y + dx)) & 0x8000) != 0)
            {
                uint32 previous_y = dy;
                dy = 0 - dx;
                dx = previous_y;
            }
            else
            {
                dx = 0 - dx;
                dy = 0 - dy;
            }
        }
    }
    own = r_u16(state - 0xac);
    w_u16(frame + 0x20, own + dx);
    own = r_u16(state - 0xaa);
    w_u16(frame + 0x22, own + dy);
    own = r_u16(state - 0xa8);
    w_u16(frame + 0x24, own);
    uint32 count = (uint32)r_s16(settings + 0xc);
    uint32 ranges = r_u32(settings + 0x10);
    uint32 enabled = G3HankAnimationMatches45E8(state, count, ranges);
    uint32 distance_step = 0;
    if (enabled != 0)
        distance_step = r_u16(settings + 0x1c);
    count = (uint32)r_s16(settings + 0xe);
    ranges = r_u32(settings + 0x14);
    uint32 turn = 0;
    if (G3HankAnimationMatches45E8(state, count, ranges) != 0)
        turn = r_u16(settings + 0x1e);
    w_u32(frame + 0x10, 0);
    G3Function80036988(state - 0x104, frame + 0x20, (uint32)(sint16)distance_step, (uint32)(sint16)turn, 0, frame);
    uint32 selector = 0xffffffff;
    if (G3HankUpdateFloor2E68(state, module_base, frame) != 0 && enabled != 0)
    {
        bounds = r_u32(settings + 0x18);
        if (G3HankClampBounds4204(state, bounds) == 0)
            selector = (uint32)r_s16(settings + 0x22);
    }
    G3HankUpdateAnimation2A1C(state, selector, frame + 0x28, frame);
    uint32 mode = r_u32(state + 0x4c);
    if (mode == 0x10 || mode == 0x12 || mode == 0x13)
        return;
    if ((sint32)squared >= r_s32(settings + 8))
    {
        coordinate = r_u16(player + 0x58);
        own = r_u16(state - 0xac);
        w_u16(frame + 0x18, coordinate - own);
        coordinate = r_u16(player + 0x5a);
        own = r_u16(state - 0xaa);
        w_u16(frame + 0x1a, coordinate - own);
        coordinate = r_u16(player + 0x5c);
        own = r_u16(state - 0xa8);
        w_u16(frame + 0x1c, coordinate - own);
        sint32 distance = (sint32)G3HankDistanceSquared47F8(state, frame + 0x18);
        uint32 pursuit = r_u32(state + 8);
        uint32 requested;
        if (pursuit != 0 && distance < r_s32(pursuit))
            requested = 6;
        else if (r_u32(state) != 0)
            requested = 1;
        else
        {
            uint32 idle = r_u32(state + 4);
            if (idle != 0)
                requested = r_s16(settings + 0x26) != 0 ? 5 : 2;
            else if (r_u32(state + 8) != 0)
                requested = 6;
            else
            {
                w_u32(state + 0x4c, 0x16);
                return;
            }
        }
        w_u16(state + 0x34, requested);
        G3HankFinishAnimation2CFC(state);
    }
    sint32 pending = r_s16(state + 0x34);
    if (pending >= 0 && r_u32(frame + 0x28) != 0)
    {
        switch (pending)
        {
        case 1: G3HankEnterAttachment308C(state, frame); break;
        case 2: G3HankEnterIdle34B0(state, frame); break;
        case 5: G3HankEnterPause372C(state, frame); break;
        case 6: G3HankEnterPursuit33BC(state, frame); break;
        }
        w_u16(state + 0x34, 0xffff);
    }
}

void G3HankUpdatePause7824(uint32 state, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007824, "G3L_33");
    uint32 frame = guest_stack - 0x40;
    uint32 settings = r_u32(state + 0x2c);
    uint32 player = r_u32(0x8009efc4);
    uint32 count = (uint32)r_s16(settings + 0xc);
    uint32 ranges = r_u32(settings + 0x10);
    uint32 distance_step = 0;
    if (G3HankAnimationMatches45E8(state, count, ranges) != 0)
        distance_step = r_u16(settings + 0x1c);
    count = (uint32)r_s16(settings + 0xe);
    ranges = r_u32(settings + 0x14);
    uint32 turn = 0;
    if (G3HankAnimationMatches45E8(state, count, ranges) != 0)
        turn = r_u16(settings + 0x1e);
    w_u32(frame + 0x10, frame + 0x20);
    uint32 arrived = G3Function80036738(state - 0x104, settings + 0x28, (uint32)(sint16)distance_step, (uint32)(sint16)turn, frame + 0x20, frame);
    uint32 selector = 0xffffffff;
    if (G3HankUpdateFloor2E68(state, module_base, frame) != 0)
    {
        uint32 progressed = r_u32(frame + 0x20);
        if (progressed != 0 && arrived == 0)
            selector = (uint32)r_s16(settings + 0x22);
    }
    G3HankUpdateAnimation2A1C(state, selector, frame + 0x24, frame);
    uint32 mode = r_u32(state + 0x4c);
    if (mode == 0x10 || mode == 0x12 || mode == 0x13)
        return;
    uint32 coordinate = r_u16(player + 0x58);
    uint32 own = r_u16(state - 0xac);
    w_u16(frame + 0x18, coordinate - own);
    coordinate = r_u16(player + 0x5a);
    own = r_u16(state - 0xaa);
    w_u16(frame + 0x1a, coordinate - own);
    coordinate = r_u16(player + 0x5c);
    own = r_u16(state - 0xa8);
    w_u16(frame + 0x1c, coordinate - own);
    sint32 distance = (sint32)G3HankDistanceSquared47F8(state, frame + 0x18);
    uint32 requested = 0;
    if (distance < r_s32(settings + 4))
        requested = 0xe;
    else if (arrived != 0)
    {
        uint32 idle = r_u32(state + 4);
        if (idle == 0)
        {
            w_u32(state + 0x4c, 0x16);
            return;
        }
        requested = r_s16(idle + 0x1c) != 0 ? 3 : 2;
    }
    if (requested != 0)
    {
        w_u16(state + 0x34, requested);
        G3HankFinishAnimation2CFC(state);
    }
    sint32 pending = r_s16(state + 0x34);
    if (pending >= 0 && r_u32(frame + 0x24) != 0)
    {
        switch (pending)
        {
        case 2: G3HankEnterIdle34B0(state, frame); break;
        case 3: G3HankEnterAlert3404(state, frame); break;
        case 0xe: G3HankEnterRun36F4(state, frame); break;
        }
        w_u16(state + 0x34, 0xffff);
    }
}

void G3HankUpdateHurt7A48(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007a48, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    G3HankUpdateAnimation2A1C(state, 0xffffffff, 0, frame);
    if (G3HankAnimationReady2CC4(state) != 0)
        G3HankChooseInitial3898(state, frame);
}

void G3HankUpdateKnockback7A8C(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007a8c, "G3L_33");
    uint32 frame = guest_stack - 0x20;
    uint32 settings = r_u32(state + 0x20);
    G3HankUpdateAnimation2A1C(state, 0xffffffff, 0, frame);
    uint32 height = r_u16(state - 0xa8);
    uint32 velocity = r_u16(settings + 0x12);
    uint32 old_floor = r_u16(settings + 0x14);
    uint32 next_height = height + velocity;
    w_u16(state - 0xa8, next_height);
    velocity = r_u16(settings + 0x12);
    uint32 acceleration = r_u16(settings + 0x10);
    velocity += acceleration;
    uint32 floor = r_u16(settings + 0x14);
    uint32 old_delta = height - old_floor;
    uint32 next_delta = next_height - floor;
    w_u16(settings + 0x12, velocity);
    if ((next_delta & 0xffff) == 0 || ((old_delta ^ next_delta) & 0x8000) != 0)
    {
        floor = r_u16(settings + 0x14);
        w_u16(state - 0xa8, floor);
        if (r_s16(settings + 8) >= 0)
            G3HankEnterHurt3760(state, frame);
        else
            G3HankChooseInitial3898(state, frame);
    }
    else
    {
        sint32 limit = r_s16(settings + 0xe);
        if ((limit > 0 && limit < (sint16)velocity) || (limit < 0 && (sint16)velocity < limit))
            w_u16(settings + 0x12, r_u16(settings + 0xe));
    }
}

void G3HankUpdateDying7B90(uint32 state, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007b90, "G3L_33");
    uint32 frame = guest_stack - 0x20;
    uint32 settings = r_u32(state + 0x24);
    G3HankUpdateFloor2E68(state, module_base, frame);
    uint32 mode = r_u32(state + 0x4c);
    if (mode == 0x10 || mode == 0x12 || mode == 0x13)
        return;
    G3HankUpdateAnimation2A1C(state, 0xffffffff, 0, frame);
    if (G3HankAnimationReady2CC4(state) != 0)
    {
        sint32 previous = r_s16(settings + 0x12);
        switch (previous)
        {
        case 0: G3HankEnterAttached3020(state, frame); break;
        case 1: G3HankEnterAttachment308C(state, frame); break;
        case 2: G3HankEnterIdle34B0(state, frame); break;
        case 3: G3HankEnterAlert3404(state, frame); break;
        case 4: G3HankEnterWait36BC(state, frame); break;
        case 5: G3HankEnterPause372C(state, frame); break;
        case 6: case 7: case 8: case 9: case 10:
            G3HankEnterPursuit33BC(state, frame);
            break;
        case 0xb: case 0xc: case 0xd:
            G3HankEnterMove3614(state, frame);
            break;
        case 0xe: G3HankEnterRun36F4(state, frame); break;
        }
        w_u16(state + 0x34, 0xffff);
    }
}

void G3HankUpdateDeath7CC0(uint32 state, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007cc0, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    G3HankUpdateAnimation2A1C(state, 0xffffffff, 0, frame);
    G3HankUpdateFloor2E68(state, module_base, frame);
    if (G3HankAnimationReady2CC4(state) != 0)
        G3HankChooseDeath4520(state, 0, 1, module_base, frame);
}

void G3HankUpdateDeathTail7D10(uint32 state, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007d10, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    G3HankUpdateAnimation2A1C(state, 0xffffffff, 0, frame);
    G3HankUpdateFloor2E68(state, module_base, frame);
}

void G3HankUpdateRemoved7D44(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007d44, "G3L_33");
    uint32 frame = guest_stack - 0x20;
    if (G3HankAtOrigin486C(state, 0) != 0)
    {
        uint32 flags = r_u32(state - 0xf4);
        w_u32(state - 0xf4, flags & 0xfffffbff);
    }
    uint32 settings = r_u32(state + 0x24);
    uint32 elapsed = r_u16(settings + 0x10);
    sint32 limit = r_s16(settings + 0xe);
    ++elapsed;
    w_u16(settings + 0x10, elapsed);
    if ((sint16)elapsed < limit)
        return;
    uint32 flags = r_u32(state - 0xf4);
    uint32 descriptor = r_u32(state - 0xe4);
    w_u32(state - 0xf4, flags & 0xfffff7ff);
    uint32 source = descriptor + 0x10;
    uint32 destination = state - 0xac;
    uint32 high_address = source + 3;
    uint32 high_lane = high_address & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
    uint32 lane = source & 3;
    uint32 low = r_masked_u32(source & 0xfffffffc, (15 << lane) & 15);
    uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
    uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
    uint32 z = (uint32)r_s16(source + 4);
    high_address = destination + 3;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    lane = destination & 3;
    w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15 << lane) & 15);
    w_u16(destination + 4, z);
    G3HankChooseHurt3994(state, 1, frame);
    uint32 pair = r_u32(settings);
    if (pair != 0)
        w_u16(pair, r_u16(pair + 2));
}

void G3HankUpdate7E08(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007e08, "G3L_33");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    uint32 flags = r_u32(state - 0xf4);
    uint32 player = r_u32(0x8009efc4);
    uint32 marked_flags = flags | 0x400;
    w_u32(state - 0xf4, marked_flags);
    sint32 player_x = r_s16(player + 0x58);
    sint32 own_x = r_s16(state - 0xac);
    uint32 dx = (uint32)(player_x - own_x);
    uint32 squared_x = dx * dx;
    sint32 player_y = r_s16(player + 0x5a);
    sint32 own_y = r_s16(state - 0xaa);
    uint32 dy = (uint32)(player_y - own_y);
    sint32 maximum = r_s32(state + 0x38);
    uint32 squared = squared_x + dy * dy;
    if (maximum < (sint32)squared)
        w_u32(state - 0xf4, marked_flags & 0xfffffbff);
    uint32 mode = r_u32(state + 0x4c);
    switch (mode)
    {
    case 0: G3HankUpdateAttached4AA8(state, module_base, frame); break;
    case 1: G3HankUpdateAttachment5000(state, module_base, frame); break;
    case 2: G3HankUpdateIdle5CB0(state, module_base, frame); break;
    case 3: G3HankUpdateAlert5878(state, module_base, frame); break;
    case 4: G3HankUpdateReturn7140(state, module_base, frame); break;
    case 5: G3HankUpdatePause7824(state, module_base, frame); break;
    case 6: G3HankUpdatePursuit5284(state, module_base, frame); break;
    case 7: G3HankUpdateWait5C44(state, module_base, frame); break;
    case 8: G3HankUpdateShooter60C8(state, game, module_base, frame); break;
    case 9: G3HankUpdateLeap6460(state, game, module_base, frame); break;
    case 0xa: G3HankUpdateFall6828(state, game, module_base, frame); break;
    case 0xb: G3HankUpdateMove69F4(state, module_base, frame); break;
    case 0xc: G3HankUpdateStop6E40(state, module_base, frame); break;
    case 0xd: G3HankUpdateTurn7018(state, module_base, frame); break;
    case 0xe: G3HankUpdateRun7394(state, module_base, frame); break;
    case 0xf: G3HankUpdateDying7B90(state, module_base, frame); break;
    case 0x10: G3HankUpdateDeath7CC0(state, module_base, frame); break;
    case 0x11: G3HankUpdateDeathTail7D10(state, module_base, frame); break;
    case 0x12:
    {
        flags = r_u32(state - 0xf4);
        uint32 settings = r_u32(state + 0x24);
        w_u32(state - 0xf4, flags & 0xfffffbff);
        if (settings != 0)
        {
            sint32 radius = r_s16(settings + 0x28);
            if (radius > 0)
            {
                player = r_u32(0x8009efc4);
                own_x = r_s16(object + 0x58);
                player_x = r_s16(player + 0x58);
                dx = (uint32)(player_x - own_x);
                player_y = r_s16(player + 0x5a);
                own_y = r_s16(object + 0x5a);
                dy = (uint32)(player_y - own_y);
                sint32 own_z = r_s16(object + 0x5c);
                sint32 player_z = r_s16(player + 0x5c);
                uint32 dz = (uint32)(player_z - own_z);
                squared = dx * dx + dy * dy + dz * dz;
                if (radius * radius >= (sint32)squared)
                    G3Function80074044(object, game, frame);
            }
            uint32 selector = (uint32)r_s16(settings + 0x24);
            uint32 random_range = (uint32)r_s16(settings + 0x26);
            G3Function80038798(object, selector, random_range, frame);
        }
        else
            G3Function80038798(object, 0, 0, frame);
        break;
    }
    case 0x13: G3HankUpdateRemoved7D44(state, frame); break;
    case 0x14: G3HankUpdateHurt7A48(state, frame); break;
    case 0x15: G3HankUpdateKnockback7A8C(state, frame); break;
    default:
        G3Function80020CBC(module_base + 0x8a18, game, squared_x, marked_flags, frame, 0);
        break;
    }
}

void G3HankDamage80F4(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000080f4, "G3L_33");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    uint32 mode = r_u32(state + 0x4c);
    if (mode == 0x14 || mode == 0xf || mode == 0x10 || mode == 0x12 || mode == 0x11 || mode == 0x13)
        return;
    uint32 special = r_u32(state + 0x18);
    if (special != 0)
        G3HankNotifyDisable41A4(r_u32(special), frame);
    uint32 settings = r_u32(state + 0x24);
    if (settings == 0)
    {
        G3HankChooseDeath4520(state, 0, 0, module_base, frame);
        return;
    }
    uint32 count = (uint32)r_s16(settings + 0x16);
    uint32 ranges = r_u32(settings + 0x1c);
    if (G3HankAnimationMatches45E8(state, count, ranges) == 0)
        return;
    uint32 health = r_u32(settings);
    if (health != 0)
    {
        uint32 value = r_u16(health) - 1;
        w_u16(health, value);
        if ((sint16)value > 0)
        {
            G3HankEnterDying37E0(state, frame);
            return;
        }
    }
    G3HankChooseDeath4520(state, 1, 1, module_base, frame);
}

void G3HankContact81E8(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000081e8, "G3L_33");
    uint32 frame = guest_stack - 0x30;
    uint32 mode = r_u32(object + 0x150);
    uint32 contact = r_u32(object + 0xb0);
    uint32 state = object + 0x104;
    if (mode == 0x10 || mode == 0x12 || mode == 0x13)
        return;
    G3Function8003C548(contact, frame + 0x10, frame + 0x11);
    uint32 owner = r_u32(contact + 0x14);
    uint32 player = r_u32(0x8009efc4);
    if (owner != player)
    {
        if (r_u32(state + 0x4c) != 0)
        {
            uint32 own = r_u16(state - 0xac);
            uint32 delta = r_u16(contact + 0x28);
            w_u16(state - 0xac, own + delta);
            own = r_u16(state - 0xaa);
            delta = r_u16(contact + 0x2a);
            w_u16(state - 0xaa, own + delta);
            G3Function8005F3A8(object, contact + 0x28);
        }
        return;
    }
    uint32 first = r_u8(frame + 0x10);
    if (first - 6 < 4)
    {
        uint32 second = r_u8(frame + 0x11);
        if (second == 9 || second - 3 < 3)
        {
            mode = r_u32(state + 0x4c);
            if (mode != 0x11)
            {
                uint32 settings = r_u32(state + 0x24);
                bool enabled = true;
                if (settings != 0)
                {
                    uint32 count = (uint32)r_s16(settings + 0x14);
                    uint32 ranges = r_u32(settings + 0x18);
                    enabled = G3HankAnimationMatches45E8(state, count, ranges) != 0;
                }
                if (enabled)
                    G3Function80074044(object, game, frame);
                return;
            }
        }
    }
    uint32 second = r_u8(frame + 0x11);
    if (second - 6 >= 4)
        return;
    first = r_u8(frame + 0x10);
    if (first != 9 && first - 3 >= 3)
        return;
    uint32 special = r_u32(state + 0x18);
    if (special != 0)
        G3HankNotifyDisable41A4(r_u32(special), frame);
    mode = r_u32(state + 0x4c);
    if (mode == 0x11)
    {
        uint32 settings = r_u32(state + 0x24);
        if (settings != 0 && r_s16(settings + 0xc) != 0)
            G3HankChooseDeath4520(state, 0, 0, module_base, frame);
        return;
    }
    if (mode == 0x14 || mode == 0xf || mode == 0x10 || mode == 0x13)
        return;
    uint32 settings = r_u32(state + 0x24);
    if (settings == 0)
    {
        G3HankChooseDeath4520(state, 0, 0, module_base, frame);
        return;
    }
    uint32 count = (uint32)r_s16(settings + 0x16);
    uint32 ranges = r_u32(settings + 0x1c);
    if (G3HankAnimationMatches45E8(state, count, ranges) == 0)
        return;
    uint32 health = r_u32(settings);
    if (health != 0)
    {
        uint32 value = r_u16(health) - 1;
        w_u16(health, value);
        if ((sint16)value > 0)
        {
            G3HankEnterDying37E0(state, frame);
            return;
        }
    }
    G3HankChooseDeath4520(state, 1, 1, module_base, frame);
}

void G3HankCommand8458(uint32 object, uint32 command, uint32 module_base)
{
    FUNCTION_MARKER(0x00008458, "G3L_33");
    uint32 first = r_u32(command);
    uint32 expected = r_u32(module_base + 0x8a90);
    if (first != expected) return;
    uint32 second = r_u32(command + 4);
    expected = r_u32(module_base + 0x8a94);
    if (second == expected) w_u32(object + 0x150, 0x12);
}

uint32 G3HitchupEvent(uint32 object, uint32 message, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_60");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3HitchupInitialize(object, frame);
        return 1;
    case 2:
        G3HitchupUpdate(object, frame);
        return 1;
    case 3:
        G3HitchupCollide();
        return 1;
    default:
        return 0;
    }
}

void G3HitchupConfigure(uint32 object, uint32 commands, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000007c, "G3L_60");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    w_u32(guest_stack + 4, commands);
    if (commands == 0)
        return;
    while (G3Function80085DFC(frame + 0x2c, frame + 0x10, frame + 0x14, frame + 0x18) != 0)
    {
        sint32 kind = r_s16(frame + 0x10);
        if (kind == 0x52)
            w_u32(state, r_u32(frame + 0x14));
        else if (kind == 0x5a)
        {
            uint32 source = r_u32(frame + 0x14);
            uint32 xy = r_u32(source);
            uint32 z = (uint32)r_s16(source + 4);
            w_u32(state + 0xe, xy);
            w_u16(state + 0x12, z);
        }
        else if (kind == 0xa4)
        {
            uint32 source = r_u32(frame + 0x14);
            w_u16(state + 0x14, r_u16(source));
        }
        else if (kind == 0xa5)
        {
            uint32 source = r_u32(frame + 0x14);
            sint32 radius = r_s16(source);
            w_u32(state + 4, (uint32)(radius * radius));
        }
        else if (kind == 0x28)
        {
            sint32 bytes = r_s16(frame + 0x18);
            uint32 boxes = r_u32(frame + 0x14);
            w_u32(state + 8, boxes);
            w_u16(state + 0xc, (uint32)(bytes / 12));
        }
    }
}

void G3HitchupInitialize(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000001ac, "G3L_60");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    w_u16(state + 0x12, 0);
    w_u16(state + 0x10, 0);
    w_u16(state + 0xe, 0);
    w_u16(state + 0x14, 0);
    w_u32(object + 0x104, 0);
    w_u32(state + 4, 64000000);
    w_u32(state + 8, 0);
    uint32 resource = r_u32(object + 0x1c);
    uint32 commands = r_u32(resource + 0x1c);
    G3HitchupConfigure(object, commands, frame);
    commands = r_u32(object + 0x28);
    G3HitchupConfigure(object, commands, frame);
    w_u16(state + 0x16, 0);
}

void G3HitchupUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000220, "G3L_60");
    uint32 player = r_u32(0x8009efc4);
    uint32 frame = guest_stack - 0x40;
    uint32 state = object + 0x104;
    sint32 processed = r_s16(state + 0x16);
    uint32 player_state = r_u32(player + 0x24);
    if (processed != 0)
        return;
    w_u16(state + 0x16, 1);
    if (r_u32(player_state + 0x1fc) != 0)
        return;
    if (r_u32(object + 0x104) == 0)
        return;
    uint32 game = r_u32(0x80097048);
    uint32 lists = r_u32(game + 0x5c);
    uint32 target = r_u32(lists + 4);
    while (target != 0)
    {
        uint32 resource = r_u32(target + 0x1c);
        uint32 expected = r_u32(state);
        uint32 name = r_u32(resource + 0x24);
        uint32 first = r_u32(expected);
        uint32 other = r_u32(name);
        if (first == other)
        {
            first = r_u32(expected + 4);
            other = r_u32(name + 4);
            if (first == other)
                break;
        }
        target = r_u32(target + 8);
    }
    if (target == 0)
        return;
    uint32 boxes = r_u32(state + 8);
    bool inside = false;
    if (boxes != 0)
    {
        sint32 count = r_s16(state + 0xc);
        if (count > 0)
        {
            sint32 x = r_s16(target + 0x58);
            for (sint32 index = 0; index < count; ++index)
            {
                uint32 box = boxes + (uint32)index * 12;
                if (x < r_s16(box) || r_s16(box + 6) < x)
                    continue;
                sint32 y = r_s16(target + 0x5a);
                if (y < r_s16(box + 2) || r_s16(box + 8) < y)
                    continue;
                sint32 z = r_s16(target + 0x5c);
                if (z < r_s16(box + 4) || r_s16(box + 10) < z)
                    continue;
                inside = true;
                break;
            }
        }
    }
    else
    {
        uint32 x = r_u16(object + 0x58);
        uint32 tx = r_u16(target + 0x58);
        uint32 dx = x - tx;
        sint32 component = (sint32)(dx << 16) >> 16;
        uint32 xx = (uint32)(component * component);
        w_u16(frame + 0x10, dx);
        uint32 y = r_u16(object + 0x5a);
        uint32 ty = r_u16(target + 0x5a);
        uint32 dy = y - ty;
        component = (sint32)(dy << 16) >> 16;
        uint32 yy = (uint32)(component * component);
        w_u16(frame + 0x12, dy);
        uint32 z = r_u16(object + 0x5c);
        uint32 tz = r_u16(target + 0x5c);
        uint32 dz = z - tz;
        component = (sint32)(dz << 16) >> 16;
        uint32 zz = (uint32)(component * component);
        w_u16(frame + 0x14, dz);
        sint32 limit = r_s32(state + 4);
        inside = limit >= (sint32)(xx + yy + zz);
    }
    if (inside || (r_u32(target + 0x10) & 0x200) != 0)
        return;
    uint32 yaw = (uint32)r_s16(object + 0x64);
    uint32 sine = G3Function8005B470(yaw);
    uint32 cosine = G3Function8005B590(yaw);
    uint32 xy = r_u32(target + 0x58);
    uint32 z = (uint32)r_s16(target + 0x5c);
    w_u32(frame + 0x18, xy);
    w_u16(frame + 0x1c, z);
    uint32 first_offset = (uint32)r_s16(state + 0xe);
    uint32 first_product = sine * first_offset;
    uint32 second_offset = (uint32)r_s16(state + 0x10);
    uint32 second_product = cosine * second_offset;
    uint32 x = r_u16(object + 0x58);
    x += (uint32)((sint32)first_product >> 12);
    x -= (uint32)((sint32)second_product >> 12);
    w_u16(target + 0x58, x);
    first_offset = (uint32)r_s16(state + 0xe);
    first_product = cosine * first_offset;
    second_offset = (uint32)r_s16(state + 0x10);
    second_product = sine * second_offset;
    uint32 y = r_u16(object + 0x5a);
    y += (uint32)((sint32)first_product >> 12);
    y += (uint32)((sint32)second_product >> 12);
    w_u16(target + 0x5a, y);
    z = r_u16(object + 0x5c);
    uint32 z_offset = r_u16(state + 0x12);
    w_u16(target + 0x5c, z + z_offset);
    x = r_u16(target + 0x58);
    uint32 old_x = r_u16(frame + 0x18);
    w_u16(frame + 0x20, x - old_x);
    y = r_u16(target + 0x5a);
    uint32 old_y = r_u16(frame + 0x1a);
    w_u16(frame + 0x22, y - old_y);
    z = r_u16(target + 0x5c);
    uint32 old_z = r_u16(frame + 0x1c);
    w_u16(frame + 0x24, z - old_z);
    G3Function8005F3A8(target, frame + 0x20);
    xy = r_u32(target + 0x58);
    z = (uint32)r_s16(target + 0x5c);
    w_u32(target + 0x70, xy);
    w_u16(target + 0x74, z);
    w_u16(target + 0x62, 0);
    w_u16(target + 0x60, 0);
    yaw = r_u16(object + 0x64);
    uint32 yaw_offset = r_u16(state + 0x14);
    w_u16(target + 0x64, yaw + yaw_offset);
}
