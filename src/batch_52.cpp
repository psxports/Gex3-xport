#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void G3MstrdbgApplyTriggerFlags4708(uint32 trigger, uint32 state)
{
    FUNCTION_MARKER(0x00004708, "G3L_39");
    uint32 flags = r_u16(trigger + 8);
    uint32 mask;
    if ((flags & 2) != 0) mask = 0x800000;
    else if ((flags & 1) != 0) mask = 0x400000;
    else if ((flags & 4) != 0) mask = 0x100000;
    else return;
    w_u32(state + 0x18, r_u32(state + 0x18) | mask);
}

void G3MstrdbgTriggerProperty475C(uint32 trigger, uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000475c, "G3L_39");
    uint32 frame = guest_stack - 0x18;
    G3MstrdbgRunProperty0DA4(object, trigger + 8, module_base, frame);
}

uint32 G3MstrdbgCrossedFrame4784(uint32 previous, uint32 current, uint32 trigger_time, uint32 animation)
{
    FUNCTION_MARKER(0x00004784, "G3L_39");
    sint32 before = (sint16)previous;
    sint32 after = (sint16)current;
    sint32 start = r_s16(animation + 2) * 40;
    sint32 end = r_s16(animation + 4) * 40;
    sint32 time = (sint16)trigger_time;
    if (before < after && time <= after)
    {
        if (before < time) return 1;
        if ((sint16)start == time && before == (sint16)start) return 1;
        if (time < (sint16)start) return 1;
    }
    if (after >= before || time < after) return 0;
    if (time == (sint16)end) return before >= time ? 1 : 0;
    return time < before ? 1 : 0;
}

void G3MstrdbgTriggerFrame4878(uint32 previous, uint32 current, uint32 trigger, uint32 animation, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004878, "G3L_39");
    uint32 frame = guest_stack - 0x20;
    sint32 trigger_frame = r_s16(trigger + 2);
    uint32 object = r_u32(guest_stack + 0x10);
    uint32 time = (uint32)(sint16)(trigger_frame * 40);
    if (G3MstrdbgCrossedFrame4784((uint32)(sint16)previous, (uint32)(sint16)current, time, animation) == 0) return;
    sint32 kind = r_s16(trigger);
    switch (kind)
    {
    case 1: G3MstrdbgSpawnTrigger4500(trigger, object, object + 0x104, frame); break;
    case 3: G3MstrdbgPlayerFlag4644(trigger); break;
    case 4: G3MstrdbgPlayerDamage4694(frame); break;
    case 5: G3MstrdbgApplyTriggerFlags4708(trigger, object + 0x104); break;
    case 6: G3MstrdbgTriggerProperty475C(trigger, object, module_base, frame); break;
    }
}

void G3MstrdbgTriggerFrames4968(uint32 previous, uint32 current, uint32 animation, uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004968, "G3L_39");
    uint32 frame = guest_stack - 0x38;
    uint32 trigger = 0;
    uint32 count = 0;
    if (animation != 0) trigger = r_u32(animation + 0x14);
    while (trigger != 0 && count < 20)
    {
        w_u32(frame + 0x10, object);
        G3MstrdbgTriggerFrame4878((uint32)(sint16)previous, (uint32)(sint16)current, trigger, animation, module_base, frame);
        trigger = r_u32(trigger + 4);
        ++count;
    }
}

uint32 G3MstrdbgDecide4A04(uint32 object, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004a04, "G3L_39");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    uint32 attempts = 0;
    if ((r_u32(state + 0x18) & 0x8000000) == 0)
    {
        while (true)
        {
            G3MstrdbgSelectState4B00(state, configuration, model, module_base, frame);
            uint32 found = G3MstrdbgSelectAnimation2794(state + 0x20, state + 0x14, state, model);
            model = G3MstrdbgApplyTransition4FA4(state, configuration, model, object, module_base, frame);
            ++attempts;
            if ((found & 0xff) != 0 || attempts >= 15) break;
            if ((r_u32(state + 0x18) & 0x8000000) != 0) break;
        }
    }
    if (attempts >= 15) G3MstrdbgBrainLiteralDiagnostic(module_base + 0x88e8, frame);
    return model;
}

void G3MstrdbgSelectState4B00(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004b00, "G3L_39");
    uint32 frame = guest_stack - 0x28;
    uint32 attempts = 0;
    while (true)
    {
        uint32 previous = r_u8(state + 1);
        G3MstrdbgPrepareMode4BC4(state, configuration, model, module_base, frame);
        G3MstrdbgUpdateMainMode4C98(state, configuration, model, module_base, frame);
        if ((sint8)previous == r_s8(state + 1)) break;
        G3MstrdbgClearBehaviorFlag4F8C(state);
        ++attempts;
        if (attempts >= 15) break;
    }
    if (attempts >= 15) G3MstrdbgBrainTwoWordDiagnostic(module_base + 0x891c, configuration, model, frame);
    G3MstrdbgDispatchCommand4E38(state, configuration, model, module_base, frame);
}

void G3MstrdbgPrepareMode4BC4(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004bc4, "G3L_39");
    uint32 frame = guest_stack - 0x28;
    uint32 snapshot = r_u32(state + 0x1c);
    sint32 mode = r_s8(state);
    uint32 previous = r_u8(state);
    if (mode == 0)
    {
        w_u32(frame + 0x10, 100);
        if (G3MstrdbgShouldWake64C8(snapshot + 0x14, configuration, state, model, module_base, frame) != 0) w_u8(state, 1);
    }
    else if (mode == 1 && r_s8(state + 1) == 0x10) w_u8(state, 0);
    mode = r_s8(state);
    if (mode != (sint8)previous)
    {
        w_u32(state + 0x18, r_u32(state + 0x18) & 0xfeffffff);
        mode = r_s8(state);
        if (mode == 1)
        {
            w_u8(state + 1, 0xd);
            w_u8(state + 2, 0x1b);
        }
        else if (mode == 0)
        {
            w_u8(state + 1, 0xe);
            w_u8(state + 2, 0x1b);
        }
    }
}

void G3MstrdbgUpdateMainMode4C98(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004c98, "G3L_39");
    uint32 frame = guest_stack - 0x18;
    sint32 kind = r_s8(state);
    if (kind == 1) G3MstrdbgUpdateActive4CE0(state, configuration, model, module_base, frame);
    else if (kind == 0) G3MstrdbgUpdateWander4D60(state, configuration, model, module_base, frame);
}

void G3MstrdbgUpdateActive4CE0(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004ce0, "G3L_39");
    uint32 frame = guest_stack - 0x20;
    uint32 previous = r_u8(state + 1);
    G3MstrdbgUpdatePursuit53C8(state, configuration, model, module_base, frame);
    uint32 kind = r_u8(state + 1);
    if (kind - 6 < 2 && kind != previous && (sint8)previous != 9 && (sint8)previous != 0xa && (sint8)previous != 8)
    {
        w_u8(state + 0xb, 0);
        w_u8(state + 8, 0);
        w_u8(state + 9, 0);
    }
}

void G3MstrdbgUpdateWander4D60(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004d60, "G3L_39");
    uint32 frame = guest_stack - 0x20;
    sint32 kind = r_s8(state + 1);
    uint32 selected = r_u8(state + 1);
    switch (kind)
    {
    case 0:
        break;
    case 1:
        if ((r_u32(state + 0x18) & 0x1000000) != 0) selected = 2;
        break;
    case 2:
    case 0xe:
    case 0xf:
        if (r_s8(state + 2) != 0x1c)
        {
            if ((sint8)selected != r_s8(state + 1)) w_u8(state + 0xf, 1);
            return;
        }
        selected = r_u8(state + 0x3f);
        w_u8(state + 1, selected);
        w_u8(state + 2, 0x1b);
        w_u8(state + 0xf, 0);
        return;
    default:
        G3MstrdbgBrainTwoWordDiagnostic(module_base + 0x8950, configuration, model, frame);
        break;
    }
    if (r_s8(state + 2) == 0x1c)
    {
        w_u8(state + 1, selected);
        w_u8(state + 2, 0x1b);
        w_u8(state + 0xf, 0);
    }
    else if ((sint8)selected != r_s8(state + 1)) w_u8(state + 0xf, 1);
}

void G3MstrdbgDispatchCommand4E38(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004e38, "G3L_39");
    uint32 frame = guest_stack - 0x20;
    uint32 snapshot = r_u32(state + 0x1c);
    sint32 kind = r_s8(state + 1);
    uint32 values = snapshot + 0x14;
    uint32 angles = snapshot + 0x20;
    switch (kind)
    {
    case 0: G3MstrdbgCommandTwo59C0(state); break;
    case 1: G3MstrdbgSwitchCommand5BB0(state, state, model, state + 0x18, module_base, frame); break;
    case 2: G3MstrdbgFinishCommand5D9C(state); break;
    case 4:
    case 5: G3MstrdbgFinishCommand5910(state, state); break;
    case 6:
    case 7:
    case 0xc: G3MstrdbgSwitchCommand592C(state, state, model, module_base, frame); break;
    case 8:
        w_u32(frame + 0x10, state);
        G3MstrdbgSwitchTriggered5DB4(state, model, angles, module_base, frame);
        break;
    case 9: G3MstrdbgSwitchCommand5A70(state, state); break;
    case 0xa: G3MstrdbgSwitchCommand5A14(state, state); break;
    case 0xb: G3MstrdbgFinishCommand5DA8(state); break;
    case 0xd: G3MstrdbgSwitchCommand5B70(state); break;
    case 0xe: G3MstrdbgCommandFour59CC(state); break;
    case 0xf: G3MstrdbgCommandNineteen59F0(state); break;
    case 0x11: G3MstrdbgSwitchCommand5B08(state, state); break;
    default: G3Function80020CBC(module_base + 0x89bc, values, angles, model, frame, 0); break;
    }
}

uint32 G3MstrdbgClearBehaviorFlag4F8C(uint32 state)
{
    FUNCTION_MARKER(0x00004f8c, "G3L_39");
    uint32 flags = r_u32(state + 0x18) & 0xfeffffff;
    w_u32(state + 0x18, flags);
    return flags;
}

uint32 G3MstrdbgApplyTransition4FA4(uint32 state, uint32 configuration, uint32 model, uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004fa4, "G3L_39");
    uint32 frame = guest_stack - 0x28;
    if (r_s8(state + 1) == 0x11 && r_u8(state + 2) - (uint32)0x16 < 2)
    {
        G3MstrdbgRunSequence0D6C(object, model, frame);
        uint32 resource = r_u32(model + 0x18);
        model = r_u32(resource);
        w_u32(object + 0x24, model);
        w_u8(state + 0xb, 0);
        w_u8(state + 0x10, 0);
        w_u8(state + 8, 0);
        w_u8(state + 9, 0);
    }
    uint32 flags = r_u32(state + 0x18);
    w_u16(state + 0x44, 100);
    w_u16(state + 0x46, 100);
    w_u8(state + 0x37, 0);
    w_u32(state + 0x18, flags & 0xff3f7fff);
    w_u32(object + 0x14, r_u32(object + 0x14) & 0xf7ffffff);
    sint32 mode = r_s8(state + 2);
    if (mode == 0xc)
    {
        G3MstrdbgNoop08BC();
        mode = r_s8(state + 2);
    }
    if (mode == 0x14)
    {
        r_s8(state + 0x3e);
        G3MstrdbgNoop7E48();
        w_u8(state + 0x3e, 0xff);
        G3MstrdbgRunProperty0DA4(object, module_base + 0x83ac, module_base, frame);
        w_u32(object + 0x10, r_u32(object + 0x10) & 0xfffffbff);
        w_u32(state + 0x18, r_u32(state + 0x18) | 0x8000000);
    }
    uint32 resource = r_u32(model);
    sint32 kind;
    if ((r_u32(resource + 0x18) & 2) != 0)
    {
        mode = r_s8(state + 2);
        if (mode == 2)
        {
            G3MstrdbgEnterSecondary16B8(object, frame);
            kind = r_s8(state + 1);
        }
        else
        {
            kind = r_s8(state + 1);
            if (kind == 0xd && mode == 3)
            {
                G3MstrdbgEnterPrimary1668(object, frame);
                kind = r_s8(state + 1);
            }
        }
    }
    else kind = r_s8(state + 1);
    if (kind == 0x11) w_u32(state + 0x18, r_u32(state + 0x18) & 0xffefffff);
    kind = r_s8(state + 1);
    if (kind == 6) w_u32(state + 0x18, r_u32(state + 0x18) & 0xfffff7ff);
    else if (kind == 7) w_u32(state + 0x18, r_u32(state + 0x18) | 0x800);
    if (r_s8(state + 2) == 0x20)
    {
        kind = r_s8(state + 1);
        if (kind == 1)
        {
            uint32 minimum = G3MstrdbgMinimum672C(configuration);
            uint32 snapshot = r_u32(state + 0x1c);
            resource = r_u32(model);
            uint32 result = G3MstrdbgTurnToward7890(snapshot + 0xe, state + 0x24, resource + 0xc, minimum, frame);
            w_u8(state + 0xd, result);
        }
        else if (kind == 2)
        {
            uint32 minimum = G3MstrdbgMinimum672C(configuration);
            uint32 snapshot = r_u32(state + 0x1c);
            G3MstrdbgSelectHeading77E4(state + 0x24, object + 0x58, snapshot + 0xe, minimum, frame);
        }
        else if (kind == 0xb) G3MstrdbgUpdatePath6D80(state, model, frame);
    }
    flags = r_u32(state + 0x18);
    if ((flags & 0x200000) != 0)
    {
        uint32 count = r_u8(state + 9);
        w_u32(state + 0x18, flags & 0xfffffffb);
        if (count != 0xff) w_u8(state + 9, count + 1);
        G3MstrdbgRunProperty0DA4(object, module_base + 0x83b8, module_base, frame);
        if (r_s8(state + 2) == 0x1c) w_u8(state + 0x4e, 8);
    }
    if (r_s8(state + 1) == 0xa && r_s8(state + 2) == 0xc) G3MstrdbgRunProperty0DA4(object, module_base + 0x83b8, module_base, frame);
    if (r_u8(state + 1) >= 3) w_u32(state + 0x18, r_u32(state + 0x18) & 0xffffffbf);
    if (r_s8(state + 1) == 1 && r_s8(state + 2) == 0xd) w_u8(state + 7, r_u8(state + 7) + 1);
    if (G3MstrdbgFindTrigger44AC(0, r_u32(state + 0x14)) != 0)
    {
        w_u32(object + 0x14, r_u32(object + 0x14) | 0x8000000);
        uint32 count = r_u8(state + 8);
        if (count != 0xff) w_u8(state + 8, count + 1);
    }
    G3MstrdbgFindTrigger44AC(1, r_u32(state + 0x14));
    w_u8(state + 0x37, 0);
    if (r_s8(state + 1) == 0xb)
    {
        mode = r_s8(state + 2);
        if (mode != 0xe)
        {
            if (mode == 0x20) G3MstrdbgEnterSecondary16B8(object, frame);
            else if (mode == 0x12) G3MstrdbgEnterPrimary1668(object, frame);
        }
        if (r_s8(state + 1) == 0xb) w_u8(state + 0x10, 0);
    }
    return model;
}

void G3MstrdbgUpdatePursuit53C8(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000053c8, "G3L_39");
    uint32 frame = guest_stack - 0x48;
    uint32 snapshot = r_u32(state + 0x1c);
    uint32 distances = snapshot + 0x14;
    w_u16(frame + 0x18, r_u16(distances + 2));
    w_u16(frame + 0x1a, r_u16(distances + 0xa));
    uint32 current = r_u32(state + 0x1c);
    w_u16(frame + 0x1c, r_u16(current + 0x20));
    sint32 selected = r_s8(state + 1);
    if (selected != 0x11 && G3MstrdbgCheckAlternate6404(state, model, frame) != 0)
    {
        selected = 0x11;
        goto commit;
    }
    {
        uint32 flags = r_u32(state + 0x18);
        if ((flags & 0x200002) == 2)
        {
            w_u8(state + 2, 0x1c);
            r_s8(state + 0x3e);
            G3MstrdbgNoop7E48();
            w_u8(state + 0x3e, 0xff);
            selected = 0xa;
            goto commit;
        }
        if ((flags & 4) != 0)
        {
            uint32 resource = r_u32(model);
            if ((r_u32(resource + 0x18) & 0x100) == 0 && (flags & 0x200000) == 0)
            {
                w_u8(state + 2, 0x1c);
                selected = 9;
                goto commit;
            }
        }
    }
    switch ((sint8)(r_u8(state + 1) - 4))
    {
    case 0:
        w_u32(frame + 0x10, 100);
        if (G3MstrdbgShouldWake64C8(distances, configuration, state, model, module_base, frame) != 0) goto choose;
        if (r_s8(state + 2) == 0x1c)
        {
            selected = 0xc;
            goto store_mode;
        }
        goto changed;
    case 1:
        if (G3MstrdbgCheckWake6618(state, configuration, model, module_base, frame) == 0) { selected = 4; break; }
        if (r_u8(state + 0xc) >= 61) { selected = 4; break; }
        if (r_s8(state + 2) == 0x1c) goto choose_value;
        if (G3MstrdbgCheckSpecial63A8(state, r_u32(state + 0x1c), model, (uint32)r_s16(state + 0x42), frame) != 0) goto choose_maneuver;
        {
            uint32 candidate = G3MstrdbgSelectValue6E8C(model, 0, 0, 0, module_base, frame, false);
            w_u32(frame + 0x10, 0);
            w_u32(frame + 0x14, candidate);
            if (G3MstrdbgFindTriggered8094(0, frame + 0x18, model, state, module_base, frame) != 0) selected = (sint32)candidate;
        }
        break;
    case 2:
        if (G3MstrdbgCheckWake6618(state, configuration, model, module_base, frame) == 0) goto idle;
        if ((r_u32(state + 0x18) & 8) != 0) { selected = 5; break; }
        if (G3MstrdbgCheckRetreat628C(state, r_u32(state + 0x1c), model, (uint32)r_s16(state + 0x42), frame) != 0) { selected = 7; break; }
        if (G3MstrdbgCheckSpecial63A8(state, r_u32(state + 0x1c), model, (uint32)r_s16(state + 0x42), frame) != 0) goto choose_maneuver;
        goto triggered;
    case 3:
        if (G3MstrdbgCheckWake6618(state, configuration, model, module_base, frame) == 0) goto idle;
        if ((r_u32(state + 0x18) & 8) != 0) { selected = 5; break; }
        if (G3MstrdbgCheckAdvance6168(state, r_u32(state + 0x1c), model, (uint32)r_s16(state + 0x42), frame) != 0) { selected = 6; break; }
        if (G3MstrdbgCheckSpecial63A8(state, r_u32(state + 0x1c), model, (uint32)r_s16(state + 0x42), frame) != 0) goto choose_maneuver;
triggered:
        w_u32(frame + 0x10, 0);
        w_u32(frame + 0x14, (uint32)r_s8(state + 1));
        if (G3MstrdbgFindTriggered8094(state + 5, frame + 0x18, model, state, module_base, frame) != 0) selected = 8;
        break;
    case 4:
        if (r_s8(state + 2) == 0x1c)
        {
            selected = r_s8(state + 4);
            goto store_mode;
        }
        w_u32(frame + 0x10, 0);
        w_u32(frame + 0x14, (uint32)r_s8(state + 1));
        if (G3MstrdbgFindTriggered8094(frame + 0x20, frame + 0x18, model, state, module_base, frame) != 0)
        {
            uint32 new_index = (uint32)r_s8(frame + 0x20);
            uint32 table = r_u32(model + 8);
            uint32 old_index = (uint32)r_s8(state + 5);
            uint32 new_slot = r_u32(table + new_index * 4);
            uint32 old_slot = r_u32(table + old_index * 4);
            uint32 new_sequence = r_u32(new_slot);
            uint32 old_sequence = r_u32(old_slot);
            sint32 new_priority = r_s16(new_sequence + 4);
            sint32 old_priority = r_s16(old_sequence + 4);
            if (old_priority < new_priority) w_u8(state + 0xf, 1);
        }
        break;
    case 5:
        {
            sint32 previous = r_s8(state + 4);
            if (previous == 6) selected = 6;
            else if (previous == 7) selected = 7;
            else goto choose_value;
        }
        break;
    case 6:
        selected = 0xa;
        break;
    case 7:
        if (r_s8(state + 2) == 0x1c) goto choose_value;
        goto changed;
    case 8:
        w_u32(frame + 0x10, 100);
        if (G3MstrdbgShouldWake64C8(distances, configuration, state, model, module_base, frame) != 0) goto choose;
        if ((r_u32(state + 0x18) & 8) != 0) { selected = 4; break; }
        {
            uint32 distance = r_u16(distances);
            if ((sint32)G3MstrdbgMinimum672C(configuration) >= 401)
            {
                if ((sint32)distance < (sint32)G3MstrdbgMinimum672C(configuration)) selected = 0x10;
            }
            else if (distance < 400) selected = 0x10;
        }
        break;
    case 9:
        if (r_s8(state + 2) == 0x1c) goto choose;
        goto changed;
    case 0xc:
        break;
    case 0xd:
        goto choose_value;
    default:
        G3MstrdbgPursuitLiteralDiagnostic(module_base + 0x8a24, frame);
        break;
    }
    goto commit;
idle:
    selected = r_u32(r_u32(model)) == 1 ? 0xc : 4;
    goto commit;
choose:
    if (r_u32(r_u32(model)) == 1) goto choose_value;
    selected = (sint32)G3MstrdbgCheckTerrain649C(snapshot + 8, snapshot, state, frame);
    if (selected != 0) goto choose_value;
    selected = 5;
    goto commit;
choose_maneuver:
    selected = (sint32)G3MstrdbgSelectManeuver6E48(model, module_base, frame);
    goto commit;
choose_value:
    selected = (sint32)G3MstrdbgSelectValue6E8C(model, 0, 0, 0, module_base, frame, false);
commit:
    if (r_s8(state + 2) != 0x1c) goto changed;
store_mode:
    {
        uint32 previous = r_u8(state + 1);
        w_u8(state + 1, (uint32)selected);
        w_u8(state + 2, 0x1b);
        w_u8(state + 0xf, 0);
        w_u8(state + 4, previous);
    }
    return;
changed:
    if (selected != r_s8(state + 1)) w_u8(state + 0xf, 1);
}

uint32 G3MstrdbgFinishCommand5910(uint32 command, uint32 state)
{
    FUNCTION_MARKER(0x00005910, "G3L_39");
    uint32 flags = r_u32(state + 0x18) & 0xfffffff7;
    w_u32(state + 0x18, flags);
    w_u8(command + 2, 0x1c);
    return 0x1c;
}

void G3MstrdbgSwitchCommand592C(uint32 command, uint32 state, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000592c, "G3L_39");
    uint32 frame = guest_stack - 0x30;
    uint32 kind = (uint32)r_s8(command + 1);
    uint32 previous = (uint32)r_s8(command + 4);
    uint32 mode = (uint32)r_s8(state + 0x48);
    uint32 distance = G3MstrdbgSelectDistance6EE0(kind, previous, mode, model, module_base, frame);
    G3MstrdbgSelectAngles7420(frame + 0x18, frame + 0x1a, state);
    uint32 angle = (uint32)r_s16(frame + 0x18);
    uint32 target_distance = r_u16(frame + 0x1a);
    uint32 old_command = (uint32)r_s8(command + 2);
    w_u32(frame + 0x10, old_command);
    uint32 next = G3MstrdbgSelectCommand6C54(angle, target_distance, distance & 0xffff, model, frame);
    uint32 changed = r_u8(command + 0xf);
    w_u8(command + 2, next);
    if (changed != 0) w_u8(command + 2, 0x1c);
}

uint32 G3MstrdbgCommandTwo59C0(uint32 command)
{
    FUNCTION_MARKER(0x000059c0, "G3L_39");
    w_u8(command + 2, 2);
    return 2;
}

uint32 G3MstrdbgCommandFour59CC(uint32 command)
{
    FUNCTION_MARKER(0x000059cc, "G3L_39");
    uint32 result = r_s8(command + 2) == 0x1b ? 4 : 0x1c;
    w_u8(command + 2, result);
    return result;
}

uint32 G3MstrdbgCommandNineteen59F0(uint32 command)
{
    FUNCTION_MARKER(0x000059f0, "G3L_39");
    uint32 result = r_s8(command + 2) == 0x1b ? 0x13 : 0x1c;
    w_u8(command + 2, result);
    return result;
}

void G3MstrdbgSwitchCommand5A14(uint32 command, uint32 state)
{
    FUNCTION_MARKER(0x00005a14, "G3L_39");
    sint32 mode = r_s8(command + 2);
    if (mode == 0xc) w_u8(command + 2, 0x14);
    else if (mode == 0x1b)
    {
        w_u32(state + 0x18, r_u32(state + 0x18) | 0x200000);
        w_u8(command + 2, 0xc);
    }
}

void G3MstrdbgSwitchCommand5A70(uint32 command, uint32 state)
{
    FUNCTION_MARKER(0x00005a70, "G3L_39");
    sint32 mode = r_s8(command + 2);
    if (mode == 0x1b)
    {
        w_u32(state + 0x18, r_u32(state + 0x18) | 0x200000);
        w_u8(command + 2, 0xb);
    }
    else if (mode == 0xb) w_u8(command + 2, (r_u32(state + 0x18) & 4) != 0 ? 0x1b : 0x1d);
    else if (mode == 0x1d)
    {
        w_u32(state + 0x18, r_u32(state + 0x18) & 0xffdfffff);
        w_u8(command + 2, 0x1c);
    }
}

void G3MstrdbgSwitchCommand5B08(uint32 command, uint32 state)
{
    FUNCTION_MARKER(0x00005b08, "G3L_39");
    sint32 mode = r_s8(command + 2);
    if (mode == 0x16 || mode == 0x17) w_u8(command + 2, 0x1c);
    else if (mode == 0x1b) w_u8(command + 2, (r_u32(state + 0x18) & 0x800) != 0 ? 0x17 : 0x16);
}

void G3MstrdbgSwitchCommand5B70(uint32 command)
{
    FUNCTION_MARKER(0x00005b70, "G3L_39");
    sint32 mode = r_s8(command + 2);
    if (mode == 0x1b) w_u8(command + 2, 3);
    else if (mode == 3) w_u8(command + 2, 0x1c);
}

void G3MstrdbgSwitchCommand5BB0(uint32 command, uint32 state, uint32 model, uint32 flags, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00005bb0, "G3L_39");
    uint32 frame = guest_stack - 0x30;
    uint32 snapshot = r_u32(state + 0x1c);
    uint32 distances = snapshot + 0x14;
    uint32 angles = snapshot + 0x20;
    uint32 length = G3MstrdbgMotionDistance75D0(1, model, frame);
    sint32 threshold = (sint16)(length + 100) - 400;
    if (threshold < 400) threshold = 400;
    sint32 selection = (sint8)(r_u8(command + 2) - 1);
    switch (selection)
    {
    case 0:
    case 8:
    case 9:
        if ((sint32)r_u16(distances + 4) < (sint16)threshold)
        {
            w_u8(command + 2, 0x1f);
            break;
        }
        w_u8(command + 2, G3MstrdbgSelectTurn7720(1, (uint32)r_s16(angles + 4), model, frame));
        break;
    case 0x1f:
        w_u8(command + 2, G3MstrdbgSelectTurn7720(1, (uint32)r_s16(angles + 4), model, frame));
        break;
    case 0xc:
        {
            sint32 count = r_s8(command + 7);
            sint32 random = (sint32)G3SdkRandom80027FA8();
            if (count > random % 3 + 1) w_u8(command + 2, 0x20);
        }
        break;
    case 0x1a:
        w_u8(command + 2, 0x20);
        break;
    case 0x1e:
        {
            uint32 result = r_u8(command + 0xd);
            if (result == 0) w_u8(command + 2, 0x20);
            else if (result == 1)
            {
                w_u8(command + 7, 0);
                w_u8(command + 2, 0xd);
            }
        }
        break;
    case 0x1b:
        break;
    default:
        {
            uint32 motion_frame = frame - 0x50;
            sint32 x = r_s16(motion_frame + 0x10);
            sint32 z = r_s16(motion_frame + 0x14);
            sint32 y = r_s16(motion_frame + 0x12);
            if (x < 0) x = -x;
            if (z < 0) z = -z;
            if (y < 0) y = -y;
            sint32 smallest = x < y ? x : y;
            if (z < smallest) smallest = z;
            uint32 selector = (uint32)r_s8(motion_frame + 0x26);
            G3Function80020CBC(module_base + 0x8a8c, (uint32)smallest, selector, model, frame, 0);
        }
        break;
    }
    if (r_u8(command + 0xf) != 0) w_u8(command + 2, 0x1c);
    else if ((r_u32(state + 0x18) & 8) != 0 && r_s8(command + 2) != 0)
    {
        w_u8(command + 2, 0x20);
        w_u32(flags, r_u32(flags) & 0xfffffff7);
    }
}

uint32 G3MstrdbgFinishCommand5D9C(uint32 command)
{
    FUNCTION_MARKER(0x00005d9c, "G3L_39");
    w_u8(command + 2, 0x1c);
    return 0x1c;
}

uint32 G3MstrdbgFinishCommand5DA8(uint32 command)
{
    FUNCTION_MARKER(0x00005da8, "G3L_39");
    w_u8(command + 2, 0x1c);
    return 0x1c;
}

void G3MstrdbgSwitchTriggered5DB4(uint32 command, uint32 model, uint32 diagnostic_argument, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00005db4, "G3L_39");
    uint32 frame = guest_stack - 0x28;
    sint32 kind = r_s8(command + 2);
    uint32 state = r_u32(guest_stack + 0x10);
    if (kind == 0x1b)
    {
        w_u8(command + 2, 0x15);
        uint32 index = (uint32)r_s8(command + 5);
        w_u8(command + 6, 0);
        uint32 table = r_u32(model + 8);
        uint32 group = r_u32(table + index * 4);
        uint32 resource = r_u32(group);
        if (r_s16(resource) == 2 && (r_u16(resource + 8) & 2) != 0) w_u32(state + 0x18, r_u32(state + 0x18) | 0x200000);
        return;
    }
    if (kind == 0x15)
    {
        if (r_u8(command + 0xf) != 0)
        {
            uint32 animation = r_u32(state + 0x14);
            if ((r_u16(animation + 0x12) & 1) != 0)
            {
                w_u8(command + 2, 0x1d);
                return;
            }
        }
        uint32 next = (uint32)(r_s8(command + 6) + 1);
        uint32 index = (uint32)r_s8(command + 5);
        uint32 count = G3MstrdbgCountEntries81A4(index, model);
        uint32 animation = r_u32(state + 0x14);
        uint32 trigger = G3MstrdbgFindTrigger44AC(2, animation);
        if (trigger != 0)
        {
            sint32 condition = r_s16(trigger + 8);
            bool repeat = condition == 0;
            if (!repeat && condition == 1) repeat = (r_u32(state + 0x18) & 0x8000) != 0;
            if (repeat) next = (uint32)r_s16(trigger + 0xa);
        }
        w_u8(command + 6, next);
        if ((sint8)next >= (sint32)count) w_u8(command + 2, 0x1d);
        return;
    }
    if (kind != 0x1d)
    {
        G3Function80020CBC(module_base + 0x8b3c, model, diagnostic_argument, model, frame, 0);
        return;
    }
    uint32 index = (uint32)r_s8(command + 5);
    uint32 table = r_u32(model + 8);
    uint32 group = r_u32(table + index * 4);
    uint32 resource = r_u32(group);
    if (r_s16(resource) == 2 && (r_u16(resource + 8) & 1) != 0) w_u32(state + 0x18, r_u32(state + 0x18) & 0xfffffbff);
    uint32 flags = r_u32(state + 0x18);
    w_u32(state + 0x18, flags & 0xffdfffff);
    w_u8(command + 2, 0x1c);
}

uint32 G3MstrdbgTurnBias5FCC(uint32 value)
{
    FUNCTION_MARKER(0x00005fcc, "G3L_39");
    sint32 angle = (sint16)value;
    if (angle < 0)
        angle = -angle;
    angle = (sint16)angle;
    if (angle < 0x200)
        return 0xfffffff6;
    if (angle < 0x600)
        return 0;
    return 10;
}

uint32 G3MstrdbgSelectAlternate6010(uint32 state, uint32 snapshot, uint32 table, uint32 difference, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006010, "G3L_39");
    sint32 base = r_s16(table);
    uint32 angle = (uint32)r_s16(snapshot + 0x2a);
    uint32 counter = r_u8(guest_stack + 0x10);
    sint32 band = (sint16)G3MstrdbgTurnBias5FCC(angle);
    sint32 band_factor = r_s16(table + 4);
    uint32 score = (uint32)base + (uint32)(band * band_factor);
    sint32 counter_factor = r_s16(table + 2);
    score += counter * (uint32)counter_factor;
    sint32 health_factor = r_s16(table + 6);
    score += (uint32)((sint16)difference * health_factor);
    sint32 random = (sint32)G3SdkRandom80027FA8();
    sint32 random_factor = r_s16(table + 8);
    uint32 random_score = (uint32)((random % 20 - 10) * random_factor);
    uint32 distance = r_u16(snapshot + 0x16);
    sint32 distance_factor = r_s16(table + 0xa);
    sint32 distance_score = (sint32)(distance * (uint32)distance_factor);
    sint32 offset = r_s16(snapshot + 0x1e);
    sint32 offset_factor = r_s16(table + 0xc);
    if (offset < 0) offset = -offset;
    sint32 offset_score = offset * offset_factor;
    uint32 first_count = r_u8(state + 9);
    sint32 first_factor = r_s16(table + 0x10);
    uint32 first_score = first_count * (uint32)first_factor;
    uint32 second_count = r_u8(state + 8);
    sint32 second_factor = r_s16(table + 0x12);
    score += random_score;
    score += (uint32)(distance_score >> 8);
    score += (uint32)(offset_score >> 8);
    score += first_score;
    return score + second_count * (uint32)second_factor;
}

uint32 G3MstrdbgCheckAdvance6168(uint32 state, uint32 snapshot, uint32 model, uint32 health, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006168, "G3L_39");
    uint32 frame = guest_stack - 0x48;
    uint32 resource = r_u32(model);
    uint32 base = r_u16(resource + 8);
    uint32 settings = r_u32(model + 0xc);
    uint32 difference = base - health;
    if (settings == 0) return 1;
    uint32 narrowed = (uint32)(sint16)difference;
    w_u32(frame + 0x10, r_u8(state + 0xb));
    uint32 result = G3MstrdbgSelectAlternate6010(state, snapshot, settings + 0x28, narrowed, frame);
    if ((sint32)result < 0) return 0;
    settings = r_u32(model + 0xc);
    {
        uint32 source = settings + 0x10;
        uint32 destination = frame + 0x18;
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
        uint32 fourth_address = source + 12;
        uint32 fourth_high_address = fourth_address + 3;
        uint32 fourth_high_lane = fourth_high_address & 3;
        uint32 fourth_lane = fourth_address & 3;
        uint32 fourth_high = r_masked_u32(fourth_high_address & 0xfffffffc, (1 << (fourth_high_lane + 1)) - 1);
        uint32 fourth_low = r_masked_u32(fourth_address & 0xfffffffc, (15 << fourth_lane) & 15);
        uint32 fourth_keep = fourth_lane == 0 ? 0 : 0xffffffff << ((4 - fourth_lane) * 8);
        uint32 fourth_value = ((fourth_high << ((3 - fourth_high_lane) * 8)) & fourth_keep) | (fourth_low >> (fourth_lane * 8));
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
        fourth_address = destination + 12;
        fourth_high_address = fourth_address + 3;
        fourth_high_lane = fourth_high_address & 3;
        fourth_lane = fourth_address & 3;
        w_masked_u32(fourth_high_address & 0xfffffffc, fourth_value >> ((3 - fourth_high_lane) * 8), (1 << (fourth_high_lane + 1)) - 1);
        w_masked_u32(fourth_address & 0xfffffffc, fourth_value << (fourth_lane * 8), (15 << fourth_lane) & 15);
    }
    {
        uint32 source = settings + 0x20;
        uint32 destination = frame + 0x28;
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
    w_u16(frame + 0x1a, 0);
    w_u16(frame + 0x20, 0);
    w_u16(frame + 0x28, 0);
    w_u16(frame + 0x2a, 0);
    w_u32(frame + 0x10, r_u8(state + 0xb));
    result = G3MstrdbgSelectAlternate6010(state, snapshot, frame + 0x18, narrowed, frame);
    return result >> 31;
}

uint32 G3MstrdbgCheckRetreat628C(uint32 state, uint32 snapshot, uint32 model, uint32 health, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000628c, "G3L_39");
    uint32 frame = guest_stack - 0x48;
    uint32 resource = r_u32(model);
    uint32 base = r_u16(resource + 8);
    uint32 settings = r_u32(model + 0xc);
    uint32 difference = base - health;
    if (settings == 0) return 0;
    uint32 narrowed = (uint32)(sint16)difference;
    w_u32(frame + 0x10, r_u8(state + 0xb));
    uint32 result = G3MstrdbgSelectAlternate6010(state, snapshot, settings + 0x10, narrowed, frame);
    if ((sint32)result < 0) return 0;
    settings = r_u32(model + 0xc);
    {
        uint32 source = settings + 0x28;
        uint32 destination = frame + 0x18;
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
        uint32 fourth_address = source + 12;
        uint32 fourth_high_address = fourth_address + 3;
        uint32 fourth_high_lane = fourth_high_address & 3;
        uint32 fourth_lane = fourth_address & 3;
        uint32 fourth_high = r_masked_u32(fourth_high_address & 0xfffffffc, (1 << (fourth_high_lane + 1)) - 1);
        uint32 fourth_low = r_masked_u32(fourth_address & 0xfffffffc, (15 << fourth_lane) & 15);
        uint32 fourth_keep = fourth_lane == 0 ? 0 : 0xffffffff << ((4 - fourth_lane) * 8);
        uint32 fourth_value = ((fourth_high << ((3 - fourth_high_lane) * 8)) & fourth_keep) | (fourth_low >> (fourth_lane * 8));
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
        fourth_address = destination + 12;
        fourth_high_address = fourth_address + 3;
        fourth_high_lane = fourth_high_address & 3;
        fourth_lane = fourth_address & 3;
        w_masked_u32(fourth_high_address & 0xfffffffc, fourth_value >> ((3 - fourth_high_lane) * 8), (1 << (fourth_high_lane + 1)) - 1);
        w_masked_u32(fourth_address & 0xfffffffc, fourth_value << (fourth_lane * 8), (15 << fourth_lane) & 15);
    }
    {
        uint32 source = settings + 0x38;
        uint32 destination = frame + 0x28;
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
    w_u16(frame + 0x1a, 0);
    w_u16(frame + 0x20, 0);
    w_u16(frame + 0x28, 0);
    w_u16(frame + 0x2a, 0);
    w_u32(frame + 0x10, r_u8(state + 0xb));
    result = G3MstrdbgSelectAlternate6010(state, snapshot, frame + 0x18, narrowed, frame);
    return result >> 31;
}

uint32 G3MstrdbgCheckSpecial63A8(uint32 state, uint32 snapshot, uint32 model, uint32 health, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000063a8, "G3L_39");
    uint32 frame = guest_stack - 0x20;
    uint32 resource = r_u32(model);
    uint32 base = r_u16(resource + 8);
    uint32 settings = r_u32(model + 0x10);
    uint32 difference = base - health;
    if (settings == 0) return 0;
    w_u32(frame + 0x10, r_u8(state + 0x10));
    uint32 result = G3MstrdbgSelectAlternate6010(state, snapshot, settings + 0x10, (uint32)(sint16)difference, frame);
    return ~result >> 31;
}

uint32 G3MstrdbgCheckAlternate6404(uint32 state, uint32 model, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006404, "G3L_39");
    uint32 frame = guest_stack - 0x20;
    uint32 resource = r_u32(model);
    uint32 base = r_u16(resource + 8);
    uint32 health = r_u16(state + 0x42);
    uint32 snapshot = r_u32(state + 0x1c);
    uint32 difference = base - health;
    if (model == 0) return 0;
    uint32 alternate = r_u32(model + 0x18);
    if (alternate == 0) return 0;
    if ((r_u32(state + 0x18) & 0x100000) != 0) return 1;
    uint32 table = r_u32(alternate + 4);
    if (table == 0) return 0;
    uint32 counter = r_u8(state + 0xb);
    w_u32(frame + 0x10, counter);
    sint32 result = (sint32)G3MstrdbgSelectAlternate6010(state, snapshot, table, (uint32)(sint16)difference, frame);
    return result < 0 ? 0 : 1;
}

uint32 G3MstrdbgCheckTerrain649C(uint32 first, uint32 second, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000649c, "G3L_39");
    uint32 frame = guest_stack - 0x18;
    r_u32(state + 0x18);
    return G3MstrdbgAccept7E7C() & 0xff;
}

uint32 G3MstrdbgShouldWake64C8(uint32 settings, uint32 configuration, uint32 state, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000064c8, "G3L_39");
    uint32 frame = guest_stack - 0x30;
    uint32 percent = (uint32)r_s16(guest_stack + 0x10);
    uint32 range = r_u32(configuration);
    sint32 product = (sint32)(range * percent);
    sint32 threshold = product / 100;
    if ((r_u32(state + 0x18) & 6) != 0) return 1;
    if (G3MstrdbgCheckWake6618(state, configuration, model, module_base, frame) == 0) return 0;
    uint32 flags = r_u32(state + 0x18);
    if ((flags & 0x40) != 0) return 1;
    if ((flags & 0x1000000) != 0)
    {
        uint32 resource = r_u32(model);
        if ((r_u32(resource + 0x18) & 2) == 0) return 1;
    }
    if (r_s8(state) == 0 && r_s8(state + 1) == 0xf) return 0;
    if (G3MstrdbgCheckWake6618(state, configuration, model, module_base, frame) == 0) return 0;
    if ((sint32)r_u16(settings + 2) >= threshold) return 0;
    return r_u8(state + 0xc) < 0x3d ? 1 : 0;
}

uint32 G3MstrdbgCheckWake6618(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006618, "G3L_39");
    uint32 frame = guest_stack - 0x18;
    uint32 snapshot = r_u32(state + 0x1c);
    sint32 limit = r_s32(configuration + 0xc);
    uint32 distance = r_u16(snapshot + 0x1c);
    if (limit < (sint32)distance) return 0;
    uint32 flags = r_u32(state + 0x18);
    if ((flags & 0x1000) == 0) return 1;
    uint32 resource = r_u32(model);
    if (r_u32(resource) == 1) return 1;
    if ((flags & 0x40000) != 0) return 1;
    uint32 box = G3MstrdbgFindExtra0AEC(configuration, module_base + 0x8208, module_base, frame);
    sint32 coordinate = r_s16(snapshot);
    sint32 maximum = r_s16(box + 6);
    if (maximum < coordinate) return 0;
    if (coordinate < r_s16(box)) return 0;
    coordinate = r_s16(snapshot + 2);
    maximum = r_s16(box + 8);
    if (maximum < coordinate) return 0;
    if (coordinate < r_s16(box + 2)) return 0;
    maximum = r_s16(box + 0xa);
    coordinate = r_s16(snapshot + 4);
    if (maximum + 0x280 < coordinate) return 0;
    if (coordinate < r_s16(box + 4) - 0x280) return 0;
    return 1;
}

uint32 G3MstrdbgMinimum672C(uint32 range)
{
    FUNCTION_MARKER(0x0000672c, "G3L_39");
    sint32 first = r_s32(range + 8);
    sint32 second = r_s32(range + 0xc);
    return (uint32)(second < first ? second : first);
}

void G3MstrdbgExtendPath6750(uint32 output, uint32 state, uint32 model, uint32 angle, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006750, "G3L_39");
    uint32 frame = guest_stack - 0x40;
    uint32 snapshot = r_u32(state + 0x1c);
    uint32 x = r_u16(snapshot);
    uint32 origin_x = r_u16(snapshot + 8);
    w_u16(frame + 0x10, x - origin_x);
    snapshot = r_u32(state + 0x1c);
    uint32 y = r_u16(snapshot + 2);
    uint32 origin_y = r_u16(snapshot + 0xa);
    w_u16(frame + 0x12, y - origin_y);
    snapshot = r_u32(state + 0x1c);
    uint32 z = r_u16(snapshot + 4);
    uint32 origin_z = r_u16(snapshot + 0xc);
    w_u16(frame + 0x14, z - origin_z);
    G3Function8002F9A8(frame + 0x18, 1000, frame + 0x10, frame);
    if ((sint16)angle != 0) G3Function8002F8E4(frame + 0x18, (uint32)(sint16)angle);
    x = r_u16(frame + 0x10);
    uint32 extension_x = r_u16(frame + 0x18);
    uint32 extension_y = r_u16(frame + 0x1a);
    z = r_u16(frame + 0x14);
    uint32 extension_z = r_u16(frame + 0x1c);
    x += extension_x;
    y = r_u16(frame + 0x12);
    z += extension_z;
    w_u16(frame + 0x20, x);
    w_u16(frame + 0x24, z);
    w_u16(frame + 0x22, y + extension_y);
    snapshot = r_u32(state + 0x1c);
    origin_x = r_u16(snapshot + 8);
    w_u16(output, origin_x + x);
    snapshot = r_u32(state + 0x1c);
    y = r_u16(frame + 0x22);
    origin_y = r_u16(snapshot + 0xa);
    w_u16(output + 2, origin_y + y);
    snapshot = r_u32(state + 0x1c);
    z = r_u16(frame + 0x24);
    origin_z = r_u16(snapshot + 0xc);
    w_u16(output + 4, origin_z + z);
}

void G3MstrdbgFriction6870(uint32 velocity, uint32 time, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006870, "G3L_39");
    uint32 frame = guest_stack - 0x40;
    sint32 x = r_s8(velocity);
    w_u32(frame + 0x10, (uint32)x);
    sint32 y = r_s8(velocity + 1);
    w_u32(frame + 0x18, 0);
    w_u32(frame + 0x14, (uint32)y);
    sint32 length = (sint32)G3Function8002FBF8(frame + 0x10, frame);
    if (length == 0)
    {
        w_u8(velocity, 0);
        w_u8(velocity + 1, 0);
        return;
    }
    uint32 elapsed = (uint32)(sint16)time;
    uint32 product_x = r_u32(frame + 0x10) * elapsed;
    sint32 shifted_x = (sint32)product_x >> 3;
    sint32 quotient_x = shifted_x / length;
    uint32 product_y = r_u32(frame + 0x14) * elapsed;
    sint32 shifted_y = (sint32)product_y >> 3;
    sint32 quotient_y = shifted_y / length;
    uint32 product_z = r_u32(frame + 0x18) * elapsed;
    sint32 shifted_z = (sint32)product_z >> 3;
    sint32 quotient_z = shifted_z / length;
    w_u32(frame + 0x10, product_x);
    w_u32(frame + 0x10, (uint32)shifted_x);
    w_u32(frame + 0x14, product_y);
    w_u32(frame + 0x14, (uint32)shifted_y);
    w_u32(frame + 0x18, product_z);
    w_u32(frame + 0x18, (uint32)shifted_z);
    w_u32(frame + 0x10, (uint32)quotient_x);
    w_u32(frame + 0x14, (uint32)quotient_y);
    w_u32(frame + 0x18, (uint32)quotient_z);
    if (G3Function8002FBF8(frame + 0x10, frame) == 0)
    {
        x = r_s8(velocity);
        w_u32(frame + 0x10, x < 0 ? (uint32)-1 : 1);
        y = r_s8(velocity + 1);
        w_u32(frame + 0x14, y < 0 ? (uint32)-1 : 1);
    }
    uint32 x_bits = (uint32)r_s8(velocity);
    uint32 adjustment = r_u32(frame + 0x10);
    x = (sint32)(x_bits - adjustment);
    w_u32(frame + 0x20, (uint32)x);
    uint32 y_bits = (uint32)r_s8(velocity + 1);
    adjustment = r_u32(frame + 0x14);
    y = (sint32)(y_bits - adjustment);
    w_u32(frame + 0x24, (uint32)y);
    if (x < -127) x = -127;
    if (x > 127) x = 127;
    w_u32(frame + 0x20, (uint32)x);
    if (y < -127) y = -127;
    if (y > 127) y = 127;
    sint32 z = r_s32(frame + 0x28);
    w_u32(frame + 0x24, (uint32)y);
    if (z < -127) z = -127;
    if (z > 127) z = 127;
    uint32 byte = r_u8(frame + 0x20);
    w_u32(frame + 0x28, (uint32)z);
    w_u8(velocity, byte);
    w_u8(velocity + 1, r_u8(frame + 0x24));
}

void G3MstrdbgVelocityDisplacement6A34(uint32 output, uint32 velocity, uint32 time, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006a34, "G3L_39");
    sint32 elapsed = (sint16)time;
    sint32 x = r_s8(velocity);
    uint32 frame = guest_stack - 0x10;
    w_u32(frame, (uint32)x);
    sint32 product_x = x * elapsed;
    sint32 y = r_s8(velocity + 1);
    sint32 product_y = y * elapsed;
    w_u32(frame + 8, 0);
    w_u32(frame + 8, 0);
    w_u32(frame + 8, 0);
    w_u32(frame + 4, (uint32)y);
    x = product_x >> 6;
    w_u32(frame, (uint32)product_x);
    w_u32(frame, (uint32)x);
    w_u32(frame + 4, (uint32)product_y);
    w_u32(frame + 4, (uint32)(product_y >> 6));
    if (x < -32767) x = -32767;
    if (x > 32767) x = 32767;
    w_u16(output, (uint32)x);
    y = r_s32(frame + 4);
    if (y < -32767) y = -32767;
    if (y > 32767) y = 32767;
    w_u16(output + 2, (uint32)y);
    sint32 z = r_s32(frame + 8);
    if (z < -32767) z = -32767;
    if (z > 32767) z = 32767;
    w_u16(output + 4, (uint32)z);
}

void G3MstrdbgYawDisplacement6B10(uint32 output, uint32 state, uint32 time, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006b10, "G3L_39");
    uint32 frame = guest_stack - 0x28;
    G3MstrdbgSelectAngles7420(frame + 0x10, frame + 0x12, state);
    uint32 animation = r_u32(state + 0x14);
    sint32 divisor = r_s16(animation + 8);
    sint32 ratio = divisor == 0 ? 0 : 0x1000 / divisor;
    sint32 limit = (sint16)((sint16)ratio * (sint16)time / 40);
    sint32 target = r_s16(frame + 0x10);
    sint32 candidate = -limit;
    if (candidate < target) candidate = target;
    if (candidate < limit) limit = candidate;
    w_u16(output, 0);
    w_u16(output + 2, 0);
    w_u16(output + 4, (uint32)limit);
}

uint32 G3MstrdbgPositiveHalf6BF4(uint32 value)
{
    FUNCTION_MARKER(0x00006bf4, "G3L_39");
    return (sint16)value > 0 ? 1 : 0;
}

uint32 G3MstrdbgCheckDisplacement6C00(uint32 distance, uint32 table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006c00, "G3L_39");
    uint32 frame = guest_stack - 0x40;
    w_u8(frame + 0x22, 6);
    G3MstrdbgLookupMotion272C(frame + 0x20, frame + 0x10, table, frame);
    uint32 length = G3Function8002FB10(frame + 0x10);
    sint32 threshold = (sint32)(0 - length - 100);
    return (sint16)distance < threshold ? 1 : 0;
}

uint32 G3MstrdbgSelectCommand6C54(uint32 angle, uint32 distance, uint32 threshold, uint32 model, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006c54, "G3L_39");
    uint32 frame = guest_stack - 0x28;
    uint32 difference = distance - threshold;
    uint32 previous = r_u32(guest_stack + 0x10);
    uint32 command = G3MstrdbgSelectTurn7720(5, (uint32)(sint16)angle, model, frame) & 0xff;
    if (command == 5)
    {
        uint32 narrowed = (uint32)(sint16)difference;
        if (G3MstrdbgPositiveHalf6BF4(narrowed) == 0)
        {
            if (G3MstrdbgCheckDisplacement6C00(narrowed, model, frame) != 0) command = 6;
            else command = (threshold & 0xffff) == 0 ? 5 : 0;
        }
    }
    if (previous != 5 && previous != 0x18 && command == 5)
    {
        if (G3MstrdbgLookup2394(0x18, 0, 0, model) != 0) command = 0x18;
    }
    else if (previous != 0 && previous != 0x19 && command == 0)
    {
        if (G3MstrdbgLookup2394(0x19, 0, 0, model) != 0) command = 0x19;
    }
    return (uint32)(sint8)command;
}

void G3MstrdbgUpdatePath6D80(uint32 state, uint32 model, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006d80, "G3L_39");
    uint32 frame = guest_stack - 0x20;
    uint32 settings = r_u32(model + 0x10);
    uint32 kind = r_u32(settings + 8);
    if (kind == 1) G3MstrdbgExtendPath6750(state + 0x24, state, model, 0, frame);
    else if (kind == 2)
    {
        sint32 random = (sint32)G3SdkRandom80027FA8();
        sint32 angle = (random % 2 * 2 - 1) * 0x2aa;
        G3MstrdbgExtendPath6750(state + 0x24, state, model, (uint32)(sint16)angle, frame);
    }
    else
    {
        uint32 snapshot = r_u32(state + 0x1c);
        G3MstrdbgTurnToward7890(snapshot + 0xe, state + 0x24, settings, 4000, frame);
    }
}

uint32 G3MstrdbgSelectManeuver6E48(uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006e48, "G3L_39");
    uint32 frame = guest_stack - 0x18;
    uint32 settings = r_u32(model + 0x10);
    uint32 kind = r_u32(settings + 8);
    if (kind >= 3)
        G3MstrdbgPursuitLiteralDiagnostic(module_base + 0x8b64, frame);
    return 0xb;
}

uint32 G3MstrdbgSelectValue6E8C(uint32 model, uint32 diagnostic_first, uint32 diagnostic_second, uint32 diagnostic_third, uint32 module_base, uint32 guest_stack, bool preserve_diagnostic_homes)
{
    FUNCTION_MARKER(0x00006e8c, "G3L_39");
    uint32 frame = guest_stack - 0x18;
    uint32 resource = r_u32(model + 0xc);
    if (resource == 0)
        return 6;
    uint32 kind = r_u32(resource + 8);
    if (kind == 0)
        return 6;
    if (kind == 1)
        return 7;
    if (preserve_diagnostic_homes) G3Function80020CBC(module_base + 0x8b7c, diagnostic_first, diagnostic_second, diagnostic_third, frame, 0);
    else G3MstrdbgPursuitLiteralDiagnostic(module_base + 0x8b7c, frame);
    return 6;
}

uint32 G3MstrdbgSelectDistance6EE0(uint32 kind, uint32 previous, uint32 mode, uint32 model, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006ee0, "G3L_39");
    uint32 frame = guest_stack - 0x18;
    uint32 settings = r_u32(model + 0xc);
    bool close = kind == 6 || (kind == 8 && previous == 6);
    if (!close && kind != 7 && (kind != 8 || previous != 7))
    {
        if (kind != 5 || mode != 2)
            return 0;
        close = G3MstrdbgSelectValue6E8C(model, previous, mode, model, module_base, frame, true) == 6;
    }
    if (close)
        return settings == 0 ? 200 : r_u16(settings);
    return settings == 0 ? 1200 : r_u16(settings + 4);
}

void G3MstrdbgMeasureDistances6F84(uint32 output, uint32 origin, uint32 first, uint32 target, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00006f84, "G3L_39");
    uint32 frame = guest_stack - 0x40;
    uint32 peer = r_u32(guest_stack + 0x10);
    uint32 last = r_u32(guest_stack + 0x14);
    if (first != 0 && origin != 0)
    {
        sint32 x = r_s16(first);
        sint32 other_x = r_s16(origin);
        w_u32(frame + 0x10, (uint32)(x - other_x));
        sint32 y = r_s16(first + 2);
        sint32 other_y = r_s16(origin + 2);
        w_u32(frame + 0x18, 0);
        w_u32(frame + 0x14, (uint32)(y - other_y));
        sint32 distance = (sint32)G3Function8002FBF8(frame + 0x10, frame);
        if (distance < 0) distance = 0;
        if (distance > 0x10000) distance = 0x10000;
        w_u16(output + 0, (uint32)distance);
    }
    if (target != 0 && origin != 0)
    {
        sint32 x = r_s16(target);
        sint32 other_x = r_s16(origin);
        w_u32(frame + 0x10, (uint32)(x - other_x));
        sint32 y = r_s16(target + 2);
        sint32 other_y = r_s16(origin + 2);
        w_u32(frame + 0x18, 0);
        w_u32(frame + 0x14, (uint32)(y - other_y));
        sint32 distance = (sint32)G3Function8002FBF8(frame + 0x10, frame);
        if (distance < 0) distance = 0;
        if (distance > 0x10000) distance = 0x10000;
        w_u16(output + 4, (uint32)distance);
    }
    if (first != 0 && peer != 0)
    {
        sint32 x = r_s16(first);
        sint32 other_x = r_s16(peer);
        w_u32(frame + 0x10, (uint32)(x - other_x));
        sint32 y = r_s16(first + 2);
        sint32 other_y = r_s16(peer + 2);
        w_u32(frame + 0x18, 0);
        w_u32(frame + 0x14, (uint32)(y - other_y));
        sint32 distance = (sint32)G3Function8002FBF8(frame + 0x10, frame);
        if (distance < 0) distance = 0;
        if (distance > 0x10000) distance = 0x10000;
        w_u16(output + 8, (uint32)distance);
    }
    if (origin != 0 && peer != 0)
    {
        sint32 x = r_s16(origin);
        sint32 other_x = r_s16(peer);
        w_u32(frame + 0x10, (uint32)(x - other_x));
        sint32 y = r_s16(origin + 2);
        sint32 other_y = r_s16(peer + 2);
        w_u32(frame + 0x18, 0);
        w_u32(frame + 0x14, (uint32)(y - other_y));
        sint32 distance = (sint32)G3Function8002FBF8(frame + 0x10, frame);
        if (distance < 0) distance = 0;
        if (distance > 0x10000) distance = 0x10000;
        w_u16(output + 2, (uint32)distance);
        uint32 peer_z = r_u16(peer + 4);
        uint32 origin_z = r_u16(origin + 4);
        w_u16(output + 0xa, peer_z - origin_z);
    }
    if (origin == 0) return;
    if (origin != 0 && last != 0)
    {
        sint32 x = r_s16(origin);
        sint32 other_x = r_s16(last);
        w_u32(frame + 0x10, (uint32)(x - other_x));
        sint32 y = r_s16(origin + 2);
        sint32 other_y = r_s16(last + 2);
        w_u32(frame + 0x18, 0);
        w_u32(frame + 0x14, (uint32)(y - other_y));
        sint32 distance = (sint32)G3Function8002FBF8(frame + 0x10, frame);
        if (distance < 0) distance = 0;
        if (distance > 0x10000) distance = 0x10000;
        w_u16(output + 6, (uint32)distance);
    }
}

void G3MstrdbgMeasureAngles71CC(uint32 output, uint32 heading, uint32 yaw, uint32 origin, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000071cc, "G3L_39");
    uint32 frame = guest_stack - 0x40;
    uint32 first = r_u32(guest_stack + 0x10);
    uint32 second = r_u32(guest_stack + 0x14);
    uint32 third = r_u32(guest_stack + 0x18);
    uint32 fourth = r_u32(guest_stack + 0x1c);
    uint32 adjusted_yaw = yaw - 0x400;
    if (first != 0 && origin != 0)
    {
        uint32 x = r_u16(first);
        uint32 origin_x = r_u16(origin);
        uint32 delta_x = x - origin_x;
        w_u16(frame + 0x10, delta_x);
        uint32 y = r_u16(first + 2);
        uint32 origin_y = r_u16(origin + 2);
        uint32 delta_y = y - origin_y;
        w_u16(frame + 0x12, delta_y);
        uint32 z = r_u16(first + 4);
        uint32 origin_z = r_u16(origin + 4);
        w_u16(frame + 0x14, z - origin_z);
        uint32 angle = G3Function8002F0C4((uint32)(sint16)delta_y, (uint32)(sint16)delta_x, frame);
        uint32 difference = G3Function80038B48((uint32)(sint16)adjusted_yaw, (uint32)(sint16)angle);
        w_u16(output + 0, difference);
        difference = G3Function80038B48((uint32)(sint16)heading, (uint32)(sint16)(angle + 0x800));
        w_u16(output + 0xa, difference);
    }
    if (second != 0 && origin != 0)
    {
        uint32 x = r_u16(second);
        uint32 origin_x = r_u16(origin);
        uint32 delta_x = x - origin_x;
        w_u16(frame + 0x10, delta_x);
        uint32 y = r_u16(second + 2);
        uint32 origin_y = r_u16(origin + 2);
        uint32 delta_y = y - origin_y;
        w_u16(frame + 0x12, delta_y);
        uint32 z = r_u16(second + 4);
        uint32 origin_z = r_u16(origin + 4);
        w_u16(frame + 0x14, z - origin_z);
        uint32 angle = G3Function8002F0C4((uint32)(sint16)delta_y, (uint32)(sint16)delta_x, frame);
        uint32 difference = G3Function80038B48((uint32)(sint16)adjusted_yaw, (uint32)(sint16)angle);
        w_u16(output + 2, difference);
    }
    if (third != 0 && origin == 0) return;
    if (third != 0 && origin != 0)
    {
        uint32 x = r_u16(third);
        uint32 origin_x = r_u16(origin);
        uint32 delta_x = x - origin_x;
        w_u16(frame + 0x10, delta_x);
        uint32 y = r_u16(third + 2);
        uint32 origin_y = r_u16(origin + 2);
        uint32 delta_y = y - origin_y;
        w_u16(frame + 0x12, delta_y);
        uint32 z = r_u16(third + 4);
        uint32 origin_z = r_u16(origin + 4);
        w_u16(frame + 0x14, z - origin_z);
        uint32 angle = G3Function8002F0C4((uint32)(sint16)delta_y, (uint32)(sint16)delta_x, frame);
        uint32 difference = G3Function80038B48((uint32)(sint16)adjusted_yaw, (uint32)(sint16)angle);
        w_u16(output + 4, difference);
    }
    if (fourth != 0 && origin != 0)
    {
        uint32 x = r_u16(fourth);
        uint32 origin_x = r_u16(origin);
        uint32 delta_x = x - origin_x;
        w_u16(frame + 0x10, delta_x);
        uint32 y = r_u16(fourth + 2);
        uint32 origin_y = r_u16(origin + 2);
        uint32 delta_y = y - origin_y;
        w_u16(frame + 0x12, delta_y);
        uint32 z = r_u16(fourth + 4);
        uint32 origin_z = r_u16(origin + 4);
        w_u16(frame + 0x14, z - origin_z);
        uint32 angle = G3Function8002F0C4((uint32)(sint16)delta_y, (uint32)(sint16)delta_x, frame);
        uint32 difference = G3Function80038B48((uint32)(sint16)adjusted_yaw, (uint32)(sint16)angle);
        w_u16(output + 6, difference);
    }
}

void G3MstrdbgSelectAngles7420(uint32 first, uint32 second, uint32 state)
{
    FUNCTION_MARKER(0x00007420, "G3L_39");
    uint32 snapshot = r_u32(state + 0x1c);
    uint32 mode = r_u8(state + 1);
    bool directed = mode - 1 < 2;
    if (!directed && (sint8)mode == 0xb)
    {
        sint32 command = r_s8(state + 2);
        directed = command != 0x12 && command != 0xe;
    }
    if (directed)
    {
        w_u16(first, r_u16(snapshot + 0x24));
        w_u16(second, r_u16(snapshot + 0x18));
        return;
    }
    mode = r_u8(state + 1);
    if (mode - 4 < 2)
    {
        if (r_s8(state + 0x48) != 0)
        {
            w_u16(first, r_u16(snapshot + 0x26));
            w_u16(second, r_u16(snapshot + 0x1a));
            return;
        }
        if ((sint8)mode != 5)
        {
            if ((sint8)mode != 4) return;
            w_u16(first, r_u16(snapshot + 0x22));
            w_u16(second, r_u16(snapshot + 0x14));
            return;
        }
    }
    else if ((sint8)mode == 0xc)
    {
        w_u16(first, r_u16(snapshot + 0x22));
        w_u16(second, r_u16(snapshot + 0x14));
        return;
    }
    w_u16(first, r_u16(snapshot + 0x20));
    w_u16(second, r_u16(snapshot + 0x16));
}

void G3MstrdbgUpdateTarget7524(uint32 object, uint32 state, uint32 snapshot, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007524, "G3L_39");
    uint32 frame = guest_stack - 0x40;
    w_u16(frame + 0x20, r_u16(object + 0x60));
    w_u16(frame + 0x22, r_u16(object + 0x62));
    w_u16(frame + 0x24, r_u16(object + 0x64));
    uint32 current = r_u32(state + 0x1c);
    uint32 axis = state + 0x24;
    w_u32(frame + 0x10, snapshot);
    w_u32(frame + 0x14, output);
    G3MstrdbgMeasureDistances6F84(current + 0x14, current + 8, current + 0xe, axis, frame);
    current = r_u32(state + 0x1c);
    uint32 yaw = (uint32)r_s16(frame + 0x24);
    uint32 heading = (uint32)r_s16(current + 6);
    uint32 angles = current + 0x20;
    w_u32(frame + 0x10, snapshot);
    uint32 next = r_u32(state + 0x1c);
    w_u32(frame + 0x18, axis);
    w_u32(frame + 0x1c, output);
    w_u32(frame + 0x14, next + 0xe);
    G3MstrdbgMeasureAngles71CC(angles, heading, yaw, current + 8, frame);
}

uint32 G3MstrdbgMotionDistance75D0(uint32 kind, uint32 table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000075d0, "G3L_39");
    uint32 frame = guest_stack - 0x50;
    w_u8(frame + 0x22, kind);
    G3MstrdbgLookupMotion272C(frame + 0x20, frame + 0x10, table, frame);
    uint32 length = G3Function8002FB10(frame + 0x10);
    uint32 second = 0;
    if (kind == 5)
    {
        w_u8(frame + 0x22, kind);
        G3MstrdbgLookupMotion272C(frame + 0x20, frame + 0x10, table, frame);
        second = G3Function8002FB10(frame + 0x10);
    }
    return (uint32)(sint16)(length + second);
}

uint32 G3MstrdbgCanTurn766C(uint32 kind, uint32 angle, uint32 table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000766c, "G3L_39");
    uint32 frame = guest_stack - 0x30;
    uint32 animation = G3MstrdbgLookup2394(kind, 0, 0, table);
    if (animation == 0) return 0;
    G3MstrdbgAnimationMotion26C4(animation, frame + 0x10, frame);
    sint32 target = (sint16)angle;
    sint32 step = r_s16(frame + 0x1c);
    sint32 magnitude = target < 0 ? -target : target;
    sint32 step_magnitude = step < 0 ? -step : step;
    if (step_magnitude >= magnitude) return 0;
    return (target < 0) == (step < 0) ? 1 : 0;
}

uint32 G3MstrdbgSelectTurn7720(uint32 kind, uint32 angle, uint32 table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007720, "G3L_39");
    uint32 frame = guest_stack - 0x28;
    uint32 first;
    uint32 second;
    if (kind == 5)
    {
        first = 7;
        second = 8;
    }
    else if (kind == 1)
    {
        first = 9;
        second = 10;
    }
    else if (kind == 0xf)
    {
        first = 0x10;
        second = 0x11;
    }
    else return kind & 0xff;
    uint32 narrowed_angle = (uint32)(sint16)angle;
    if (G3MstrdbgCanTurn766C(first, narrowed_angle, table, frame) != 0) return first & 0xff;
    if (G3MstrdbgCanTurn766C(second, narrowed_angle, table, frame) != 0) return second & 0xff;
    return kind & 0xff;
}

void G3MstrdbgSelectHeading77E4(uint32 output, uint32 origin, uint32 target, uint32 minimum, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000077e4, "G3L_39");
    uint32 frame = guest_stack - 0x30;
    uint32 x = r_u16(target);
    uint32 origin_x = r_u16(origin);
    w_u16(frame + 0x10, x - origin_x);
    uint32 y = r_u16(target + 2);
    uint32 origin_y = r_u16(origin + 2);
    w_u16(frame + 0x12, y - origin_y);
    uint32 origin_z = r_u16(origin + 4);
    uint32 z = r_u16(target + 4);
    w_u16(frame + 0x14, z - origin_z);
    G3Function8002F9A8(frame + 0x18, (uint32)(sint16)minimum, frame + 0x10, frame);
    x = r_u16(target);
    uint32 delta = r_u16(frame + 0x18);
    w_u16(output, x + delta);
    y = r_u16(target + 2);
    delta = r_u16(frame + 0x1a);
    w_u16(output + 2, y + delta);
    z = r_u16(target + 4);
    delta = r_u16(frame + 0x1c);
    w_u16(output + 4, z + delta);
}

uint32 G3MstrdbgTurnToward7890(uint32 target, uint32 origin, uint32 settings, uint32 minimum, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007890, "G3L_39");
    uint32 frame = guest_stack - 0x28;
    uint32 distance = (sint32)minimum < 301 ? 0 : minimum - 300;
    sint32 random = (sint32)G3SdkRandom80027FA8();
    sint32 angle = random % 4096;
    w_u16(frame + 0x10, 0);
    w_u16(frame + 0x12, 0);
    w_u16(frame + 0x14, 0);
    w_u16(frame + 0x10, distance);
    G3Function8002F8E4(frame + 0x10, (uint32)(sint16)angle);
    uint32 position = r_u16(target);
    uint32 delta = r_u16(frame + 0x10);
    w_u16(origin, position + delta);
    position = r_u16(target + 2);
    delta = r_u16(frame + 0x12);
    w_u16(origin + 2, position + delta);
    position = r_u16(target + 4);
    delta = r_u16(frame + 0x14);
    w_u16(origin + 4, position + delta);
    return 0;
}

uint32 G3MstrdbgStepDisplacement7958(uint32 time, uint32 rotation, uint32 position, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007958, "G3L_39");
    uint32 frame = guest_stack - 0x38;
    uint32 animation = r_u32(state + 0x14);
    uint32 scale = (uint32)r_s16(state + 0x46);
    uint32 object = r_u32(guest_stack + 0x10);
    uint32 result = G3MstrdbgAdvanceBounded23F4(time, state + 0x20, animation, scale);
    animation = r_u32(state + 0x14);
    G3MstrdbgSampleMotion24CC(animation, 40, frame + 0x10, frame);
    G3MstrdbgScaleVector2648(frame + 0x10, object + 0x68);
    sint32 x = r_s16(frame + 0x10);
    sint32 factor = r_s16(state + 0x44);
    x = x * factor / 100;
    sint32 y = r_s16(frame + 0x12);
    w_u16(frame + 0x10, (uint32)x);
    factor = r_s16(state + 0x44);
    y = y * factor / 100;
    sint32 z = r_s16(frame + 0x14);
    w_u16(frame + 0x12, (uint32)y);
    factor = r_s16(state + 0x44);
    w_u16(frame + 0x14, (uint32)(z * factor / 100));
    uint32 angle = (uint32)r_s16(object + 0x64);
    G3Function8002F8E4(frame + 0x10, angle);
    w_u16(rotation, r_u16(frame + 0x18));
    w_u16(rotation + 2, r_u16(frame + 0x1a));
    w_u16(rotation + 4, r_u16(frame + 0x1c));
    w_u16(position, r_u16(frame + 0x10));
    w_u16(position + 2, r_u16(frame + 0x12));
    w_u16(position + 4, r_u16(frame + 0x14));
    return result;
}

uint32 G3MstrdbgStep7AC4(uint32 time, uint32 object, uint32 state, uint32 mode, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007ac4, "G3L_39");
    uint32 frame = guest_stack - 0x50;
    if (r_u32(state + 0x14) == 0) return 1;
    w_u32(frame + 0x10, object);
    uint32 result = G3MstrdbgStepDisplacement7958(time, frame + 0x28, frame + 0x18, state, frame);
    if (r_s16(state + 0x40) == 0) G3MstrdbgFriction6870(state + 0x22, 40, frame);
    G3MstrdbgVelocityDisplacement6A34(frame + 0x20, state + 0x22, 40, frame);
    uint32 snapshot = r_u32(state + 0x1c);
    r_s16(snapshot + 0x2e);
    G3MstrdbgYawDisplacement6B10(frame + 0x30, state, 40, frame);
    if (r_s16(state + 0x40) != 0)
    {
        w_u16(frame + 0x18, 0);
        w_u16(frame + 0x1a, 0);
        w_u16(frame + 0x1c, 0);
    }
    if (mode != 1)
    {
        uint32 position = r_u16(object + 0x58);
        uint32 delta = r_u16(frame + 0x18);
        w_u16(object + 0x58, position + delta);
        position = r_u16(object + 0x5a);
        delta = r_u16(frame + 0x1a);
        w_u16(object + 0x5a, position + delta);
        position = r_u16(object + 0x58);
        delta = r_u16(frame + 0x20);
        w_u16(object + 0x58, position + delta);
        position = r_u16(object + 0x5a);
        delta = r_u16(frame + 0x22);
        w_u16(object + 0x5a, position + delta);
        position = r_u16(object + 0x5c);
        delta = r_u16(frame + 0x24);
        w_u16(object + 0x5c, position + delta);
    }
    uint32 heading = r_u16(object + 0x64);
    uint32 rotation = r_u16(frame + 0x2c);
    heading += rotation;
    w_u16(object + 0x64, heading);
    rotation = r_u16(frame + 0x34);
    w_u16(object + 0x64, heading + rotation);
    return result;
}

uint32 G3MstrdbgEvent7C1C(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007c1c, "G3L_39");
    uint32 frame = guest_stack - 0x48;
    if (object == 0) return 0;
    uint32 model = r_u32(object + 0x24);
    uint32 state = object + 0x104;
    if (message != 1 && G3MstrdbgValidate072C(object, payload, module_base, frame) == 0) return 0;
    if (message != 4 && (r_u32(state + 0x18) & 0x8000000) != 0) return 0;
    if (message == 1)
    {
        G3MstrdbgInitialize0020(object, payload, module_base, frame);
        return 1;
    }
    if (message == 4)
    {
        G3MstrdbgNoop03D8();
        return 1;
    }
    r_u32(game + 0x4c);
    if (G3Function8004DACC() != 0 && (r_u32(r_u32(model) + 0x18) & 0x80) == 0) return 0;
    if ((r_u32(state + 0x18) & 0x80) != 0)
    {
        if (message != 2) return 0;
        G3MstrdbgUpdateDormancy12F8(object, module_base, frame);
        return 1;
    }
    switch (message)
    {
    case 2:
        G3MstrdbgUpdateDormancy12F8(object, module_base, frame);
        G3MstrdbgUpdate03E0(object, module_base, frame);
        return 1;
    case 3:
        G3MstrdbgCollide30E4(object, frame);
        return 1;
    case 8:
        return G3MstrdbgDamage322C(object, payload, frame);
    case 0x7dd:
        return G3MstrdbgRequestRemoval35A4(object, frame);
    case 0x13:
        G3MstrdbgRunProperty0DA4(object, module_base + 0x83ac, module_base, frame);
        return 0;
    case 0x7e4:
        G3MstrdbgInitializePacket31A0(frame + 0x10);
        return G3MstrdbgDamage322C(object, frame + 0x10, frame);
    default:
        return 0;
    }
}

void G3MstrdbgNoop7E48(void)
{
    FUNCTION_MARKER(0x00007e48, "G3L_39");
}

uint32 G3MstrdbgCopyVector7E50(uint32 output, uint32 source)
{
    FUNCTION_MARKER(0x00007e50, "G3L_39");
    uint32 value = r_u16(source);
    w_u16(output, value);
    value = r_u16(source + 2);
    w_u16(output + 2, value);
    value = r_u16(source + 4);
    w_u16(output + 4, value);
    return 2;
}

uint32 G3MstrdbgAccept7E7C(void)
{
    FUNCTION_MARKER(0x00007e7c, "G3L_39");
    return 1;
}

uint32 G3MstrdbgTriggerRange7E84(uint32 limits, uint32 position)
{
    FUNCTION_MARKER(0x00007e84, "G3L_39");
    uint32 distance = r_u16(position);
    sint32 minimum = r_s16(limits);
    if ((sint32)distance < minimum)
        return 0;
    sint32 maximum = r_s16(limits + 2);
    if (maximum < (sint32)distance)
        return 0;
    minimum = r_s16(limits + 4);
    sint32 z = r_s16(position + 4);
    if (z < minimum * 11)
        return 0;
    maximum = r_s16(limits + 6);
    if (maximum * 11 < z)
        return 0;
    sint32 y = r_s16(position + 2);
    minimum = r_s16(limits + 8);
    if (y < minimum)
        return 0;
    maximum = r_s16(limits + 0xa);
    return maximum < y ? 0 : 1;
}

uint32 G3MstrdbgTriggerFlags7F30(uint32 trigger, uint32 state)
{
    FUNCTION_MARKER(0x00007f30, "G3L_39");
    if ((r_u16(trigger + 8) & 1) != 0 && (r_u32(state + 0x18) & 0x400) != 0)
        return 1;
    if ((r_u16(trigger + 8) & 2) == 0)
        return 0;
    return (r_u32(state + 0x18) & 4) != 0 ? 1 : 0;
}

uint32 G3MstrdbgTriggerMatches7F8C(uint32 trigger, uint32 position, uint32 state, uint32 mask, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00007f8c, "G3L_39");
    uint32 frame = guest_stack - 0x18;
    uint32 mode = r_u32(guest_stack + 0x10);
    if (trigger == 0)
        return 0;
    if ((mask & 1) != 0 && (r_u16(trigger + 2) & 1) == 0)
        return 0;
    if ((mask & 8) != 0 && (r_u16(trigger + 2) & 8) == 0)
        return 0;
    if (mode == 6 && (r_u16(trigger + 2) & 2) != 0)
        return 0;
    if (mode == 7 && (r_u16(trigger + 2) & 2) == 0)
        return 0;
    if ((r_u16(trigger + 2) & 4) != 0 && (r_u32(state + 0x18) & 0x80000) == 0)
        return 0;
    sint32 kind = r_s16(trigger);
    if (kind == 0)
        return G3MstrdbgTriggerRange7E84(trigger + 8, position);
    if (kind == 2)
        return G3MstrdbgTriggerFlags7F30(trigger, state);
    return 0;
}

uint32 G3MstrdbgFindTriggered8094(uint32 output, uint32 position, uint32 settings, uint32 state, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00008094, "G3L_39");
    uint32 frame = guest_stack - 0x40;
    uint32 cursor = r_u32(settings + 8);
    uint32 mask = r_u32(guest_stack + 0x10);
    uint32 mode = r_u32(guest_stack + 0x14);
    if (cursor == 0)
        return 0;
    uint32 result = 0;
    uint32 index = 0;
    uint32 attempts = 0;
    uint32 entry = r_u32(cursor);
    if (entry == 0)
        return 0;
    entry = r_u32(cursor);
    while (true)
    {
        w_u32(frame + 0x10, mode);
        uint32 trigger = r_u32(entry);
        uint32 matched = G3MstrdbgTriggerMatches7F8C(trigger, position, state, mask, frame);
        cursor += 4;
        if (matched != 0)
        {
            result = 1;
            if (output != 0)
                w_u8(output, index);
            return result;
        }
        ++attempts;
        ++index;
        if ((sint16)attempts >= 10)
        {
            G3SdkPrint80085E84(module_base + 0x8bbc);
            return result;
        }
        entry = r_u32(cursor);
        if (entry == 0)
            return result;
    }
}
