#include <stdlib.h>
#include "game_functions.h"
#include "psx.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

void G3Function800657D8(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800657d8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (r_u16(game + 0x68b4) != 0x1b)
    {
        w_u32(0x8009efa4, 0);
        w_u32(0x8009efa8, 0);
        return;
    }
    uint32 timer = r_u32(0x8009efb4);
    if ((sint32)timer > 0)
    {
        --timer;
        w_u32(0x8009efb4, timer);
        if (timer == 0)
            G3Function8004DAFC();
        return;
    }
    uint32 position = G3Function8004D880(frame, 0);
    w_u32(0x8009efa8, position);
    if ((sint32)position < 0)
        w_u32(0x8009efa8, 0);
    if (r_u32(game + 0x69d8) == 0)
        return;
    if (r_u32(0x8009efa4) == 0)
    {
        uint32 context = r_u32(game + 0x67e4);
        G3Function8006A538(context, frame, 0);
        uint32 initialized = r_u32(0x8009efa4);
        w_u32(0x8009efac, 0);
        w_u32(0x8009efa4, initialized + 1);
    }
    uint32 object = r_u32(game + 0x69d8);
    if (r_u32(object + 0x128) == 0)
        return;
    uint32 flags = r_u32(object + 0x18);
    if ((flags & 0x400) != 0)
    {
        if ((r_u32(game + 0x98) & 0x4000) != 0)
        {
            w_u32(object + 0x18, flags & 0xfffffbff);
            w_u32(0x8009efac, 2);
            G3Function8003F128(0x827, 0);
        }
        else
        {
            G3Function80065948(frame);
            w_u32(0x8009efac, 1);
        }
        object = r_u32(game + 0x69d8);
        if ((r_u32(object + 0x18) & 0x400) != 0)
            return;
    }
    if (G3Function80012A14() == 0)
    {
        uint32 context = r_u32(game + 0x67e4);
        G3Function80069B48(context, 2, frame);
    }
    w_u32(0x8009efac, 2);
}

static void G3DiagnosticHeader(uint32 text, uint32 guest_stack, G3TextMeasureOperands *operands)
{
    w_u32(guest_stack, text);
    w_u32(guest_stack, text);
    G3Function80023030(text, 0, guest_stack + 4, 0, guest_stack - 0x18, operands);
}

static void G3DiagnosticText(uint32 text, uint32 guest_stack, G3TextMeasureOperands *operands)
{
    G3Function80020CBC(text, operands->first_glyph, operands->second_glyph, operands->third_glyph, guest_stack, operands);
}

uint32 G3Function80011FA0(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80011fa0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x58;
    G3TextMeasureOperands operands;
    w_u32(frame + 0x58, game);
    if ((r_u32(game + 0xe8) & 0x04000004) != 0)
    {
        G3DiagnosticHeader(0x8009d0d4, frame, &operands);
        operands.third_glyph = r_u32(frame + 0x58);
        if ((r_u32(operands.third_glyph + 0xe8) & 4) != 0)
        {
            uint32 idle = r_u32(operands.third_glyph + 0x657c);
            if (idle != 0)
            {
                uint32 numerator = idle * 100;
                sint64 product = (sint64)(sint32)numerator * 0x7dd493ad;
                operands.third_glyph = (uint32)((uint64)product >> 32);
                operands.first_glyph = (uint32)((sint32)numerator / 33333);
                G3DiagnosticText(0x8009d0d8, frame, &operands);
            }
            else
                G3DiagnosticText(0x8009d0e8, frame, &operands);
            operands.third_glyph = r_u32(frame + 0x58);
            operands.first_glyph = r_u32(operands.third_glyph + 0x6570);
            G3DiagnosticText(0x8009d0f4, frame, &operands);
            operands.third_glyph = r_u32(frame + 0x58);
            operands.first_glyph = r_u32(operands.third_glyph + 0x6560);
            G3DiagnosticText(0x8009d100, frame, &operands);
            operands.first_glyph = r_u32(0x800a2a64);
            G3DiagnosticText(0x8009d10c, frame, &operands);
            operands.third_glyph = r_u32(frame + 0x58);
            operands.first_glyph = r_u32(operands.third_glyph + 0x68f4);
            G3DiagnosticText(0x8009d118, frame, &operands);
            operands.third_glyph = r_u32(frame + 0x58);
        }
        if ((r_u32(operands.third_glyph + 0xe8) & 0x04000000) != 0)
        {
            uint32 player = r_u32(0x8009efc4);
            operands.first_glyph = (uint32)r_s16(player + 0x58);
            G3DiagnosticText(0x8009d124, frame, &operands);
            player = r_u32(0x8009efc4);
            operands.first_glyph = (uint32)r_s16(player + 0x5a);
            G3DiagnosticText(0x8009d130, frame, &operands);
            player = r_u32(0x8009efc4);
            operands.first_glyph = (uint32)r_s16(player + 0x5c);
            G3DiagnosticText(0x8009d13c, frame, &operands);
        }
    }
    uint32 current_game = r_u32(frame + 0x58);
    if ((r_u32(current_game + 0xe8) & 0x02000000) != 0)
    {
        G3DiagnosticHeader(0x8009d148, frame, &operands);
        operands.third_glyph = r_u32(frame + 0x58);
        uint32 scene = r_u32(operands.third_glyph + 0x48);
        operands.first_glyph = r_u16(scene + 0x4e);
        G3DiagnosticText(0x8009d154, frame, &operands);
        operands.third_glyph = r_u32(frame + 0x58);
        scene = r_u32(operands.third_glyph + 0x48);
        operands.first_glyph = r_u16(scene + 0x4c);
        G3DiagnosticText(0x8009d160, frame, &operands);
        operands.third_glyph = r_u32(frame + 0x58);
        uint32 camera = r_u32(operands.third_glyph + 0x4c);
        operands.first_glyph = r_u32(camera + 0x88);
        G3DiagnosticText(0x8009d16c, frame, &operands);
        operands.third_glyph = r_u32(frame + 0x58);
        scene = r_u32(operands.third_glyph + 0x48);
        operands.second_glyph = r_u32(operands.third_glyph + 8);
        operands.first_glyph = r_u32(scene + 0x50);
        G3DiagnosticText(0x8009d178, frame, &operands);
        operands.third_glyph = r_u32(frame + 0x58);
        uint32 buckets = r_u32(operands.third_glyph + 0x28);
        uint32 shift = r_u32(operands.third_glyph + 0x2c);
        operands.first_glyph = buckets << (shift & 31);
        G3DiagnosticText(0x8009d188, frame, &operands);
        operands.third_glyph = r_u32(frame + 0x58);
        operands.first_glyph = r_u32(operands.third_glyph + 0x2c);
        G3DiagnosticText(0x8009d194, frame, &operands);
        current_game = r_u32(frame + 0x58);
    }
    if ((r_u32(current_game + 0xe8) & 0x10000000) != 0)
    {
        G3DiagnosticHeader(0x8009d1a0, frame, &operands);
        uint32 index = 0;
        do
        {
            uint32 queued = G3Function800499FC(index & 0xffff);
            operands.first_glyph = (uint32)(sint16)index;
            operands.second_glyph = queued & 0xffff;
            G3DiagnosticText(0x8009d1a8, frame, &operands);
            ++index;
        } while ((sint16)index < 16);
    }
    current_game = r_u32(frame + 0x58);
    if ((r_u32(current_game + 0xe8) & 0x20000000) != 0)
    {
        G3Function80049778(frame + 0x10);
        for (uint32 group = 0; group < 2; ++group)
        {
            for (uint32 page = 0; page < 3; ++page)
            {
                G3DiagnosticHeader(0x8009d0d4, frame, &operands);
                if (group == 1)
                    G3DiagnosticText(0x8009d1b4, frame, &operands);
                if (page == 1)
                    G3DiagnosticText(0x8009d1c0, frame, &operands);
                if (page == 2)
                    G3DiagnosticText(0x8009d1d8, frame, &operands);
                G3DiagnosticText(group != 0 ? 0x8009d21c : 0x8009d204, frame, &operands);
                for (uint32 row = 0; row < 4; ++row)
                {
                    G3DiagnosticText(0x8009d0d4, frame, &operands);
                    G3DiagnosticText(group != 0 ? 0x8009d234 : 0x8009d098, frame, &operands);
                    for (uint32 column = 0; column < 8; ++column)
                    {
                        G3DiagnosticText(0x8009d240, frame, &operands);
                        if (page == 1)
                            G3DiagnosticText(0x8009d1c0, frame, &operands);
                        if (page == 2)
                            G3DiagnosticText(0x8009d1d8, frame, &operands);
                        if (row == 1)
                            G3DiagnosticText(0x8009d244, frame, &operands);
                        operands.third_glyph = 3;
                        if (row == 2)
                        {
                            G3DiagnosticText(0x8009d248, frame, &operands);
                            operands.third_glyph = 3;
                        }
                        if (row == 3)
                            G3DiagnosticText(0x8009d254, frame, &operands);
                        uint32 index = column + (page << 3);
                        uint32 voice = G3Function800497EC(index & 0xffff);
                        uint32 active = r_u8(frame + 0x10 + index);
                        bool visible = active != 0;
                        if (visible)
                        {
                            uint32 owner = r_u32(voice + 4);
                            visible = group == 0 ? owner != 0 : owner == 0;
                        }
                        if (!visible)
                            G3DiagnosticText(0x8009d26c, frame, &operands);
                        else
                        {
                            if (row == 0)
                            {
                                operands.first_glyph = r_u8(frame + 0x10 + index);
                                G3DiagnosticText(0x8009d264, frame, &operands);
                            }
                            if (row == 1)
                            {
                                operands.first_glyph = group == 0 ? r_u16(voice + 4) : r_u8(voice + 8) >> 4;
                                G3DiagnosticText(0x8009d268, frame, &operands);
                            }
                            operands.third_glyph = 3;
                            if (row == 2)
                            {
                                operands.first_glyph = group == 0 ? r_u8(voice + 9) : r_u8(voice + 8) & 15;
                                G3DiagnosticText(0x8009d264, frame, &operands);
                                operands.third_glyph = 3;
                            }
                            if (row == 3)
                            {
                                operands.first_glyph = r_u8(voice + (group == 0 ? 0x0e : 0x0a));
                                G3DiagnosticText(0x8009d264, frame, &operands);
                            }
                        }
                        G3DiagnosticText(0x8009d098, frame, &operands);
                    }
                }
            }
        }
        current_game = r_u32(frame + 0x58);
    }
    if ((r_u32(current_game + 0xe8) & 0x04000006) == 2)
    {
        G3DiagnosticHeader(0x8009d270, frame, &operands);
        for (uint32 group = 0; group < 2; ++group)
        {
            uint32 start = group == 0 ? 0 : r_u32(frame + 0x28);
            uint32 limit = group == 0 ? 11 : 21;
            w_u32(frame + 0x28, limit);
            operands.third_glyph = limit;
            G3DiagnosticText(0x8009d278, frame, &operands);
            operands.third_glyph = r_u32(frame + 0x28);
            uint32 index = start;
            uint32 entry = 0x80096044 + index * 12;
            while ((sint32)index < (sint32)operands.third_glyph)
            {
                if (group == 1)
                    G3DiagnosticText(0x8009d27c, frame, &operands);
                uint32 text = r_u32(entry);
                entry += 12;
                G3DiagnosticText(text, frame, &operands);
                G3DiagnosticText(0x8009d288, frame, &operands);
                operands.third_glyph = r_u32(frame + 0x28);
                ++index;
            }
            G3DiagnosticText(0x8009d278, frame, &operands);
            operands.third_glyph = r_u32(frame + 0x28);
            index = start;
            entry = 0x80096044 + index * 12;
            while ((sint32)index < (sint32)operands.third_glyph)
            {
                if (group == 1)
                    G3DiagnosticText(0x8009d28c, frame, &operands);
                uint32 value = r_u32(entry + 4);
                operands.first_glyph = r_u32(value);
                ++index;
                w_u32(entry + 8, operands.first_glyph);
                G3DiagnosticText(0x8009d294, frame, &operands);
                operands.third_glyph = r_u32(frame + 0x28);
                entry += 12;
            }
        }
        operands.third_glyph = r_u32(frame + 0x58);
        operands.first_glyph = r_u32(operands.third_glyph + 0x65d0);
        G3DiagnosticText(0x8009d2a0, frame, &operands);
        operands.third_glyph = r_u32(frame + 0x58);
        uint32 pool = r_u32(operands.third_glyph + 0x5c);
        operands.first_glyph = r_u32(pool) + 1;
        G3DiagnosticText(0x8009d2b0, frame, &operands);
    }
    current_game = r_u32(frame + 0x58);
    return G3Function80047394(current_game, frame);
}

uint32 G3Function80011AC4(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80011ac4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 player = r_u32(game + 0x50);
    uint32 base = r_u32(0x8009ceac);
    uint32 flags = r_u32(player + 0x10);
    w_u32(player + 0x10, flags | 0x100);
    uint32 count = G3Function80011A58(base);
    w_u8(0x8009ceb4, count + 0x41);
    uint32 last = count - 1;
    G3TextMeasureOperands operands;
    G3DiagnosticHeader(0x8009ceb0, frame, &operands);
    uint32 entry = base;
    uint32 row = 0;
    if (r_u32(entry) != 4)
    {
        do
        {
            G3DiagnosticText(0x8009d088, frame, &operands);
            uint32 selected = r_u32(0x8009cc00);
            uint32 indicator = selected == row ? 0x8009d08c : 0x8009d094;
            ++row;
            G3DiagnosticText(indicator, frame, &operands);
            uint32 text = r_u32(entry + 0x0c);
            entry += 24;
            G3DiagnosticText(text, frame, &operands);
            G3DiagnosticText(0x8009d098, frame, &operands);
        } while (r_u32(entry) != 4);
    }
    G3DiagnosticText(0x8009d09c, frame, &operands);
    entry = base;
    if (r_u32(entry) != 4)
    {
        do
        {
            G3DiagnosticText(0x8009d088, frame, &operands);
            uint32 kind = r_u32(entry);
            if (kind == 0)
            {
                uint32 value = r_u32(entry + 0x10);
                uint32 mask = r_u32(entry + 0x14);
                uint32 current = r_u32(value);
                G3DiagnosticText((current & mask) == mask ? 0x8009d0a4 : 0x8009d0b4, frame, &operands);
            }
            else if (kind == 1)
            {
                uint32 value = r_u32(entry + 0x10);
                operands.first_glyph = r_u32(value);
                G3DiagnosticText(0x8009d0c4, frame, &operands);
            }
            G3DiagnosticText(0x8009d098, frame, &operands);
            entry += 24;
        } while (r_u32(entry) != 4);
    }
    uint32 selected = r_u32(0x8009cc00);
    uint32 step = 1;
    if (r_u32(base + selected * 24) == 1)
    {
        uint32 held = r_u32(game + 0x70);
        if ((held & 0x200) != 0)
            step = 10;
        else if ((held & 0x400) != 0)
            step = 100;
        else if ((held & 0x800) != 0)
            step = 1000;
    }
    uint32 pressed = r_u32(game + 0x74);
    bool clamp = true;
    if ((pressed & 1) != 0)
    {
        uint32 index = r_u32(0x8009cc00);
        w_u32(0x8009cc00, (sint32)index > 0 ? index - 1 : last);
    }
    else if ((pressed & 2) != 0)
    {
        uint32 index = r_u32(0x8009cc00);
        w_u32(0x8009cc00, (sint32)index < (sint32)last ? index + 1 : 0);
    }
    else if ((pressed & 4) != 0 || (r_u32(game + 0x9c) & 0x80) != 0)
    {
        uint32 index = r_u32(0x8009cc00);
        entry = base + index * 24;
        uint32 kind = r_u32(entry);
        if (kind == 0)
        {
            uint32 value = r_u32(entry + 0x10);
            uint32 mask = r_u32(entry + 0x14);
            uint32 current = r_u32(value);
            w_u32(value, current | mask);
        }
        else if (kind == 2 && (r_u32(game + 0x9c) & 0x80) != 0)
        {
            uint32 target = r_u32(entry + 0x10);
            G3DispatchDebugMenuAction(target, entry, operands.first_glyph, operands.second_glyph, operands.third_glyph, frame);
        }
        else
        {
            index = r_u32(0x8009cc00);
            entry = base + index * 24;
            kind = r_u32(entry);
            if (kind == 1)
            {
                uint32 value = r_u32(entry + 0x10);
                uint32 current = r_u32(value);
                w_u32(value, current - step);
            }
            else if (kind == 3)
            {
                w_u32(base + count * 24 + 4, index);
                uint32 next = r_u32(entry + 0x10);
                w_u32(0x8009ceac, next);
                uint32 next_count = G3Function80011A58(next);
                next = r_u32(0x8009ceac);
                uint32 restored = r_u32(next + next_count * 24 + 4);
                w_u32(0x8009cc00, restored);
                return restored;
            }
        }
    }
    else if ((pressed & 8) != 0)
    {
        uint32 index = r_u32(0x8009cc00);
        entry = base + index * 24;
        uint32 kind = r_u32(entry);
        if (kind == 0)
        {
            uint32 value = r_u32(entry + 0x10);
            uint32 mask = r_u32(entry + 0x14);
            uint32 current = r_u32(value);
            w_u32(value, current & ~mask);
        }
        else if (kind == 1)
        {
            uint32 value = r_u32(entry + 0x10);
            uint32 current = r_u32(value);
            w_u32(value, current + step);
        }
        else
            clamp = false;
    }
    if (clamp)
    {
        uint32 index = r_u32(0x8009cc00);
        entry = base + index * 24;
        if (r_u32(entry) == 1)
        {
            uint32 value = r_u32(entry + 0x10);
            uint32 minimum = r_u32(entry + 4);
            uint32 current = r_u32(value);
            if ((sint32)current < (sint32)minimum)
                w_u32(value, minimum);
            index = r_u32(0x8009cc00);
            entry = base + index * 24;
            value = r_u32(entry + 0x10);
            uint32 maximum = r_u32(entry + 8);
            current = r_u32(value);
            if ((sint32)maximum < (sint32)current)
                w_u32(value, maximum);
        }
    }
    if (r_u32(0x8009ceac) == 0x80095a14)
    {
        G3Function8003EDE8(0xffffffff);
        return G3Function8003EE30(0xffffffff);
    }
    return 0x80095a14;
}

uint32 G3Lowercase800894BC(uint32 character)
{
    return (uint32)(sint32)(sint8)tolower((unsigned char)character);
}

static void G3ScriptFirstPlainText(uint32 text, uint32 guest_stack, G3TextMeasureOperands *operands)
{
    w_u32(guest_stack, text);
    w_u32(guest_stack + 4, 0);
    w_u32(guest_stack + 8, 0);
    w_u32(guest_stack, text);
    G3Function80023030(text, 0, guest_stack + 4, 0, guest_stack - 0x18, operands);
}

uint32 G3Function80018E40(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80018e40, "SLUS_008.06");
    uint32 frame = guest_stack - 0x190;
    uint32 player = r_u32(0x8009efc4);
    uint32 position = player + 0x58;
    G3FillMemory80086094(frame + 0x10, 0, 0x158);
    bool text_outputs = false;
    G3TextMeasureOperands operands;
    for (uint32 slot = 0x800a0860; slot < 0x800a08e0; slot += 16)
    {
        uint32 index = (uint32)r_s16(slot + 4);
        if (r_s32(0x800a0760 + (index << 2)) <= 0)
            continue;
        if (r_u32(slot + 8) == 0)
            continue;
        uint32 x = (uint32)r_s16(slot) << 9;
        x = (uint32)((sint32)x / 100);
        w_u32(0x800a3c54, x);
        uint32 y = (uint32)r_s16(slot + 2) * 240;
        y = (uint32)((sint32)y / 100);
        w_u32(0x800a3c58, y);
        uint32 product = (x - 256) * r_u32(slot + 0x0c);
        uint32 scaled = (uint32)((sint32)product / 320) + 256;
        w_u32(0x800a3c5c, scaled);
        product = (y - 120) * r_u32(slot + 0x0c);
        scaled = (uint32)((sint32)product / 320) + 120;
        w_u32(0x800a3c60, scaled);
        if (r_s16(slot + 6) != 0)
        {
            uint32 width = (uint32)r_s16(slot + 0x0c);
            G3Function80021A74(width);
            uint32 text = r_u32(slot + 8);
            G3Function80020CF4(0x8009d614, text, 0, 0, frame, &operands);
        }
        else
        {
            uint32 text = r_u32(slot + 8);
            if (text_outputs)
                G3Function80020CBC(text, operands.first_glyph, operands.second_glyph, operands.third_glyph, frame, &operands);
            else
                G3ScriptFirstPlainText(text, frame, &operands);
        }
        text_outputs = true;
        index = (uint32)r_s16(slot + 4);
        uint32 counter = 0x800a0760 + (index << 2);
        uint32 value = r_u32(counter);
        w_u32(counter, value - 1);
        index = (uint32)r_s16(slot + 4);
        if (r_s32(0x800a0760 + (index << 2)) <= 0)
            w_u32(slot + 8, 0);
    }
    for (uint32 slot = 0x800a08e0; slot < 0x800a0b60; slot += 0x28)
    {
        if (r_s32(slot) <= 0)
            continue;
        uint32 object = r_u32(slot + 4);
        uint32 expected = r_u32(slot + 0x24);
        uint32 identity = r_u32(object + 0x38);
        uint32 selected = frame + 0x10;
        if (identity == expected)
            selected = object;
        else
        {
            uint32 source = slot + 0x14;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 low_lane = source & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
            uint32 packed = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
            uint32 last = (uint32)r_s16(slot + 0x18);
            uint32 destination = frame + 0x68;
            high_address = destination + 3;
            high_lane = high_address & 3;
            low_lane = destination & 3;
            w_masked_u32(high_address & 0xfffffffc, packed >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            w_masked_u32(destination & 0xfffffffc, packed << (low_lane * 8), (15 << low_lane) & 15);
            w_u16(frame + 0x6c, last);
            uint32 words[2];
            for (uint32 word = 0; word < 2; ++word)
            {
                source = slot + 0x1a + (word << 2);
                high_address = source + 3;
                high_lane = high_address & 3;
                low_lane = source & 3;
                high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                low = r_masked_u32(source & 0xfffffffc, (15 << low_lane) & 15);
                words[word] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
            }
            for (uint32 word = 0; word < 2; ++word)
            {
                destination = frame + 0x70 + (word << 2);
                high_address = destination + 3;
                high_lane = high_address & 3;
                low_lane = destination & 3;
                w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                w_masked_u32(destination & 0xfffffffc, words[word] << (low_lane * 8), (15 << low_lane) & 15);
            }
            w_u32(selected + 0x38, 0xffffffff);
        }
        uint32 callback = r_u32(slot + 8);
        if (callback != 0)
        {
            uint32 payload = r_u32(slot + 0x0c);
            if (payload != 0 && G3DispatchTimedScriptPoll(callback, selected, payload, frame) != 0)
            {
                uint32 value = r_u32(slot);
                w_u32(slot, value + 1);
            }
        }
        uint32 value = r_u32(slot) - 1;
        w_u32(slot, value);
        if (value != 0)
            continue;
        sint32 tagged = r_s16(0x8009d352);
        if (tagged != -1)
        {
            uint32 ordinal = (slot - 0x800a08e0) * 0xcccccccd;
            if (((sint32)ordinal >> 3) == tagged)
                w_u16(0x8009d352, 0xffff);
        }
        uint32 sequence = r_u32(slot + 0x10);
        if (sequence != 0)
            G3Function80018DE0(selected, sequence, frame);
    }
    for (uint32 slot = 0x800a0b60; slot < 0x800a0c20; slot += 12)
    {
        uint32 sequence = r_u32(slot + 4);
        if (sequence != 0)
        {
            sint32 previous = r_s16(0x8009d348);
            sint32 threshold = r_s16(slot);
            if (previous < threshold)
            {
                sint32 current = r_s16(position + 4);
                if (current >= threshold)
                {
                    player = r_u32(0x8009efc4);
                    G3Function80018DE0(player, sequence, frame);
                }
            }
        }
        sequence = r_u32(slot + 8);
        if (sequence != 0)
        {
            sint32 previous = r_s16(0x8009d348);
            sint32 threshold = r_s16(slot);
            if (previous >= threshold)
            {
                sint32 current = r_s16(position + 4);
                if (current < threshold)
                {
                    player = r_u32(0x8009efc4);
                    G3Function80018DE0(player, sequence, frame);
                }
            }
        }
    }
    player = r_u32(0x8009efc4);
    uint32 value = r_u16(player + 0x58);
    w_u16(0x8009d344, value);
    value = r_u16(player + 0x5a);
    w_u16(0x8009d346, value);
    value = r_u16(player + 0x5c);
    w_u16(0x8009d348, value);
    return value;
}

uint32 G3FillMemory80086094(uint32 destination, uint32 value, uint32 bytes)
{
    if (destination == 0 || (sint32)bytes <= 0)
        return 0;
    memset(psx_addr(destination, bytes), (int)(value & 0xff), bytes);
    return destination;
}

uint32 G3SdkRandom80027FA8(void)
{
    uint32 seed = r_u32(0x8009d824);
    uint32 result = (uint32)psyq_rand_next(&seed);
    w_u32(0x8009d824, seed);
    return result;
}

void G3InitializeStatics8008AED8(void)
{
    if (r_u32(0x8009b6dc) == 0)
        w_u32(0x8009b6dc, 1);
}

uint32 G3SdkPrint80085E84(uint32 format)
{
    if (format == 0)
        return 0;
    uint32 bytes = 0;
    while (r_u8(format + bytes) != 0)
        ++bytes;
    {
#ifndef NDEBUG
    fputs((const char *)psx_addr(format, bytes + 1), stdout);
#endif
    }
    return 0;
}

uint32 G3PrintCameraDiagnostic(uint32 rotation, uint32 tilt, uint32 relative_x, uint32 guest_stack)
{
    sint32 camera_y = r_s32(guest_stack + 0x10);
    sint32 camera_z = r_s32(guest_stack + 0x14);
    sint32 camera_height = r_s32(guest_stack + 0x18);
    sint32 player_x = r_s32(guest_stack + 0x1c);
    sint32 player_y = r_s32(guest_stack + 0x20);
    sint32 player_z = r_s32(guest_stack + 0x24);
    sint32 player_height = r_s32(guest_stack + 0x28);
    const char *format = (const char *)psx_addr(0x8009ec94, 73);
    char text[256];
    sint32 count = sprintf(text, format, (sint32)rotation, (sint32)tilt, (sint32)relative_x, camera_y, camera_z, camera_height, player_x, player_y, player_z, player_height);
    {
#ifndef NDEBUG
    fputs(text, stdout);
#endif
    }
    // Original prnt counts converted characters and excludes literal text
    return (uint32)(count - 32);
}

uint32 G3Function8001D770(uint32 model, uint32 vertices, uint32 packets, uint32 order_table, uint32 red, uint32 green, uint32 blue, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8001d770, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    uint32 face = r_u32(model + 0x14);
    uint32 cursor = r_u32(packets + 4);
    uint32 count = (uint32)r_s16(model + 4);
    uint32 end = face + count * 12;
    w_u32(frame + 4, end);
    uint32 source = r_u32(model + 8);
    if (face >= end)
        return cursor;
    w_u32(frame + 8, (red & 0xff) >> 1);
    w_u32(frame + 0xc, (green & 0xff) >> 1);
    w_u32(frame + 0x10, (blue & 0xff) >> 1);
    do
    {
        if (r_u32(packets + 8) - 0x60 < cursor)
            return cursor;
        uint32 vertex0 = vertices + (r_u16(face) << 3);
        uint32 vertex1 = vertices + (r_u16(face + 2) << 3);
        uint32 vertex2 = vertices + (r_u16(face + 4) << 3);
        uint32 depth1 = r_u32(vertex1 + 4);
        uint32 depth0 = r_u32(vertex0 + 4);
        uint32 depth2 = r_u32(vertex2 + 4);
        uint32 sum = depth0 + depth1 + depth2;
        sint32 average = (sint32)sum / 3;
        uint32 flags = r_u8(face + 7);
        uint32 game = r_u32(0x80097048);
        uint32 shift = r_u32(game + 0x2c);
        uint32 bucket = (uint32)(average >> (shift & 31));
        // Capture the semantic texture field under the proposed separate-storage contract
        uint32 texture = r_u32(face + 8);
        if ((flags & 2) != 0)
            bucket += (uint32)r_s8(texture + 0xb) * 2;
        uint32 limit = r_u32(game + 0x28);
        if ((sint32)bucket < (sint32)limit && (sint32)bucket >= 0)
        {
            uint32 screen0 = r_u32(vertex0);
            uint32 screen1 = r_u32(vertex1);
            uint32 screen2 = r_u32(vertex2);
            xport_gte_write_data(12, screen0);
            xport_gte_write_data(14, screen2);
            xport_gte_write_data(13, screen1);
            xport_gte_execute(0x1400006);
            w_u32(frame, xport_gte_read_data(24));
            game = r_u32(0x80097048);
            bool reversed = r_u32(game) != 0;
            sint32 area = r_s32(frame);
            if ((!reversed && area < 0) || (reversed && area > 0))
            {
                flags = r_u8(face + 7);
                uint32 bytes;
                uint32 tag;
                if ((flags & 1) == 0)
                {
                    bool textured = (flags & 2) != 0;
                    w_u8(cursor + 3, textured ? 7 : 4);
                    w_u8(cursor + 7, textured ? 0x24 : 0x20);
                    w_u32(cursor + 8, xport_gte_read_data(12));
                    w_u32(cursor + (textured ? 0x10 : 0xc), xport_gte_read_data(13));
                    w_u32(cursor + (textured ? 0x18 : 0x10), xport_gte_read_data(14));
                    if ((r_u8(face + 7) & 8) != 0)
                        w_u32(cursor + 4, r_u32(textured ? texture + 0xc : face + 8));
                    else
                    {
                        uint32 normal = 0x80098838 + (r_u8(face + 6) << 3);
                        xport_gte_write_data(0, r_u32(normal));
                        xport_gte_write_data(1, r_u32(normal + 4));
                        xport_gte_write_data(6, r_u32(textured ? texture + 0xc : face + 8));
                        game = r_u32(0x80097048);
                        xport_gte_write_data(8, r_u32(game + 0x18));
                        xport_gte_execute(0xe80413);
                        w_u32(cursor + 4, xport_gte_read_data(22));
                    }
                    uint32 scaled_red = r_u8(cursor + 4) * red;
                    uint32 scaled_green = r_u8(cursor + 5) * green;
                    uint32 scaled_blue = r_u8(cursor + 6) * blue;
                    w_u8(cursor + 4, scaled_red >> 8);
                    w_u8(cursor + 5, scaled_green >> 8);
                    w_u8(cursor + 6, scaled_blue >> 8);
                    if (textured)
                    {
                        w_u32(cursor + 0xc, r_u32(texture));
                        w_u32(cursor + 0x14, r_u32(texture + 4));
                        uint32 uv2 = r_u32(texture + 8);
                        uint32 command = r_u8(cursor + 7) | 2;
                        uint32 page = (r_u16(cursor + 0x16) & 0xff9f) | 0x20;
                        w_u16(cursor + 0x16, page);
                        w_u8(cursor + 7, command);
                        w_u32(cursor + 0x1c, uv2);
                        w_u8(cursor + 7, r_u8(cursor + 7) & 0xfe);
                        bytes = 0x20;
                        tag = 0x07000000;
                    }
                    else
                    {
                        bytes = 0x14;
                        tag = 0x04000000;
                    }
                }
                else
                {
                    bool textured = (flags & 2) != 0;
                    w_u8(cursor + 3, textured ? 9 : 6);
                    w_u8(cursor + 7, textured ? 0x34 : 0x30);
                    if (!textured)
                        w_u32(cursor + 4, 0xe1000228);
                    w_u32(cursor + (textured ? 8 : 0xc), xport_gte_read_data(12));
                    w_u32(cursor + 0x14, xport_gte_read_data(13));
                    w_u32(cursor + (textured ? 0x20 : 0x1c), xport_gte_read_data(14));
                    uint32 normal0 = 0x80098838 + (r_u16(source + (r_u16(face) << 3) + 6) << 3);
                    uint32 normal1 = 0x80098838 + (r_u16(source + (r_u16(face + 2) << 3) + 6) << 3);
                    uint32 normal2 = 0x80098838 + (r_u16(source + (r_u16(face + 4) << 3) + 6) << 3);
                    xport_gte_write_data(0, r_u32(normal0));
                    xport_gte_write_data(1, r_u32(normal0 + 4));
                    xport_gte_write_data(2, r_u32(normal1));
                    xport_gte_write_data(3, r_u32(normal1 + 4));
                    xport_gte_write_data(4, r_u32(normal2));
                    xport_gte_write_data(5, r_u32(normal2 + 4));
                    xport_gte_write_data(6, r_u32(textured ? texture + 0xc : face + 8));
                    xport_gte_execute(0x118043f);
                    w_u32(cursor + (textured ? 4 : 8), xport_gte_read_data(20));
                    w_u32(cursor + 0x10, xport_gte_read_data(21));
                    w_u32(cursor + (textured ? 0x1c : 0x18), xport_gte_read_data(22));
                    if (textured)
                    {
                        w_u32(frame + 0x1c, r_u8(cursor + 4) * red);
                        uint32 green0 = r_u8(cursor + 5) * green;
                        uint32 blue0 = r_u8(cursor + 6) * blue;
                        uint32 red1 = r_u8(cursor + 0x10) * red;
                        uint32 green1 = r_u8(cursor + 0x11) * green;
                        w_u32(frame + 0x18, r_u8(cursor + 0x12) * blue);
                        uint32 red2 = r_u8(cursor + 0x1c) * red;
                        uint32 command = r_u8(cursor + 7) | 2;
                        uint32 green2_byte = r_u8(cursor + 0x1d);
                        w_u32(cursor + 0xc, r_u32(texture));
                        w_u32(cursor + 0x18, r_u32(texture + 4));
                        uint32 uv2 = r_u32(texture + 8);
                        uint32 page = r_u16(cursor + 0x1a);
                        w_u8(cursor + 7, command & 0xfe);
                        uint32 green2 = green2_byte * green;
                        w_u32(cursor + 0x24, uv2);
                        w_u16(cursor + 0x1a, (page & 0xff9f) | 0x20);
                        uint32 saved_red0 = r_u32(frame + 0x1c);
                        w_u8(cursor + 4, saved_red0 >> 8);
                        w_u8(cursor + 5, green0 >> 8);
                        uint32 blue2 = r_u8(cursor + 0x1e) * blue;
                        w_u8(cursor + 6, blue0 >> 8);
                        w_u8(cursor + 0x10, red1 >> 8);
                        w_u8(cursor + 0x11, green1 >> 8);
                        uint32 saved_blue1 = r_u32(frame + 0x18);
                        w_u8(cursor + 0x12, saved_blue1 >> 8);
                        w_u8(cursor + 0x1c, red2 >> 8);
                        w_u8(cursor + 0x1d, green2 >> 8);
                        w_u8(cursor + 0x1e, blue2 >> 8);
                        bytes = 0x28;
                        tag = 0x09000000;
                    }
                    else
                    {
                        uint32 red0 = r_u8(cursor + 8) * red;
                        uint32 green0 = r_u8(cursor + 9) * green;
                        uint32 blue0 = r_u8(cursor + 0xa) * blue;
                        uint32 red1 = r_u8(cursor + 0x10) * red;
                        uint32 green1 = r_u8(cursor + 0x11) * green;
                        uint32 blue1 = r_u8(cursor + 0x12) * blue;
                        uint32 red2 = r_u8(cursor + 0x18) * red;
                        uint32 green2_byte = r_u8(cursor + 0x19);
                        uint32 command = r_u8(cursor + 0xb) | 2;
                        w_u8(cursor + 0xb, command);
                        w_u8(cursor + 8, red0 >> 8);
                        w_u8(cursor + 9, green0 >> 8);
                        uint32 blue2_byte = r_u8(cursor + 0x1a);
                        uint32 green2 = green2_byte * green;
                        w_u8(cursor + 0xa, blue0 >> 8);
                        uint32 blue2 = blue2_byte * blue;
                        w_u8(cursor + 0x10, red1 >> 8);
                        w_u8(cursor + 0x11, green1 >> 8);
                        w_u8(cursor + 0x12, blue1 >> 8);
                        w_u8(cursor + 0x18, red2 >> 8);
                        w_u8(cursor + 0x19, green2 >> 8);
                        w_u8(cursor + 0x1a, blue2 >> 8);
                        bytes = 0x20;
                        tag = 0x07000000;
                    }
                }
                uint32 order = order_table + (bucket << 2);
                uint32 next = r_u32(order);
                w_u32(cursor, (next & 0xffffff) | tag);
                w_u32(order, cursor & 0xffffff);
                uint32 packet_count = r_u32(packets);
                cursor += bytes;
                w_u32(packets, packet_count + 1);
                if ((r_u8(face + 7) & 2) == 0)
                {
                    w_u32(cursor + 4, 0xe1000248);
                    w_u32(cursor + 0xc, xport_gte_read_data(12));
                    w_u32(cursor + 0x10, xport_gte_read_data(13));
                    w_u32(cursor + 0x14, xport_gte_read_data(14));
                    uint32 half_red = r_u8(frame + 8);
                    w_u8(cursor + 8, half_red);
                    uint32 half_green = r_u8(frame + 0xc);
                    w_u8(cursor + 9, half_green);
                    uint32 half_blue = r_u8(frame + 0x10);
                    w_u8(cursor + 0xb, 0x22);
                    w_u8(cursor + 0xa, half_blue);
                    next = r_u32(order);
                    w_u32(cursor, (next & 0xffffff) | 0x05000000);
                    w_u32(order, cursor & 0xffffff);
                    packet_count = r_u32(packets);
                    cursor += 0x18;
                }
                else
                {
                    w_u8(cursor + 3, 7);
                    w_u8(cursor + 7, 0x24);
                    w_u32(cursor + 8, xport_gte_read_data(12));
                    w_u32(cursor + 0x10, xport_gte_read_data(13));
                    w_u32(cursor + 0x18, xport_gte_read_data(14));
                    w_u16(cursor + 0x1e, r_u8(texture + 0xa));
                    w_u32(cursor + 0xc, r_u32(texture));
                    w_u32(cursor + 0x14, r_u32(texture + 4));
                    w_u32(cursor + 0x1c, r_u32(texture + 8));
                    game = r_u32(0x80097048);
                    uint32 page = r_u16(cursor + 0x16);
                    uint32 player = r_u32(game + 0x48);
                    uint32 lighting = r_u32(player + 0xf4);
                    uint32 palette = r_u16(lighting + 0x16);
                    uint32 half_red = r_u8(frame + 8);
                    uint32 command = r_u8(cursor + 7);
                    w_u8(cursor + 4, half_red);
                    uint32 half_green = r_u8(frame + 0xc);
                    w_u8(cursor + 5, half_green);
                    uint32 half_blue = r_u8(frame + 0x10);
                    w_u16(cursor + 0x16, (page & 0xff9f) | 0x40);
                    w_u8(cursor + 7, (command | 2) & 0xfe);
                    w_u16(cursor + 0xe, palette);
                    w_u8(cursor + 6, half_blue);
                    next = r_u32(order);
                    w_u32(cursor, (next & 0xffffff) | 0x07000000);
                    w_u32(order, cursor & 0xffffff);
                    packet_count = r_u32(packets);
                    cursor += 0x20;
                }
                w_u32(packets, packet_count + 1);
            }
        }
        face += 12;
        end = r_u32(frame + 4);
    } while (face < end);
    return cursor;
}

uint32 G3AirpodEvent(uint32 object, uint32 message, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_62");
    uint32 frame = guest_stack - 0x18;
    uint32 index = message - 1;
    if (index >= 22)
        return 0;
    uint32 target = r_u32(module_base + 0x550 + index * 4);
    switch (target - module_base)
    {
    case 0x34:
        G3AirpodInitialize(object, module_base, frame);
        return 1;
    case 0x44:
        G3AirpodUpdate(object, frame);
        return 1;
    case 0x54:
        G3AirpodCollide(object);
        return 1;
    case 0x64:
        G3Function8003A114(object + 0x104, target);
        return 1;
    case 0x74:
        return 0;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved airpod event target %08X\n", (unsigned int)target);
#endif
        }
        abort();
    }
}

void G3AirpodConfigure(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000088, "G3L_62");
    uint32 frame = guest_stack - 0x30;
    uint32 state = object + 0x104;
    w_u32(guest_stack + 4, commands);
    if (commands == 0)
        return;
    while (G3Function80085DFC(frame + 0x34, frame + 0x10, frame + 0x14, frame + 0x18) != 0)
    {
        uint32 kind = r_u16(frame + 0x10);
        uint32 index = (uint32)((sint32)((kind - 10) << 16) >> 16);
        if (index >= 45)
            continue;
        uint32 target = r_u32(module_base + 0x5a8 + index * 4);
        uint32 relative = target - module_base;
        switch (relative)
        {
        case 0xb8:
            continue;
        case 0x10c:
        {
            sint32 bytes = r_s16(frame + 0x18);
            uint32 points = r_u32(frame + 0x14);
            w_u32(state + 0x14, points);
            w_u16(state + 0x1c, (uint32)(bytes / 6));
            break;
        }
        case 0x140:
        case 0x1ac:
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 value = (uint32)r_s16(data) << 12;
            sint64 product = (sint64)(sint32)value * 0x51eb851f;
            uint32 high = (uint32)((uint64)product >> 32);
            uint32 quotient = (uint32)((sint32)high >> 5) - (uint32)((sint32)value >> 31);
            if (relative == 0x140)
                w_u16(state + 0x1e, quotient);
            else
                w_u32(state + 0x10, quotient);
            break;
        }
        case 0x170:
        case 0x184:
        case 0x198:
        case 0x210:
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 offset = relative == 0x170 ? 0x20 : relative == 0x184 ? 0x22 : relative == 0x198 ? 0x24 : 0x2a;
            w_u16(state + offset, r_u16(data));
            break;
        }
        case 0x1dc:
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 value = (uint32)r_s16(data) << 8;
            sint64 product = (sint64)(sint32)value * (sint32)0x88888889;
            uint32 high = (uint32)((uint64)product >> 32);
            uint32 sum = high + value;
            uint32 quotient = (uint32)((sint32)sum >> 5) - (uint32)((sint32)value >> 31);
            w_u16(state + 0x28, quotient);
            break;
        }
        default:
            {
#ifndef NDEBUG
            fprintf(stderr, "G3 unresolved airpod command target %08X\n", (unsigned int)target);
#endif
            }
            abort();
        }
    }
}

void G3AirpodInitialize(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000023c, "G3L_62");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    w_u16(state + 0x1e, 0x400);
    w_u16(state + 0x22, 5);
    w_u16(state + 0x24, 30);
    w_u32(state + 0x10, 0x800);
    w_u16(state + 0x28, 0xaa);
    w_u16(state + 0x1c, 0);
    w_u16(state + 0x20, 0);
    w_u16(state + 0x26, 0);
    w_u16(state + 0x2a, 10);
    uint32 resource = r_u32(object + 0x1c);
    uint32 commands = r_u32(resource + 0x1c);
    G3AirpodConfigure(object, commands, module_base, frame);
    commands = r_u32(object + 0x28);
    G3AirpodConfigure(object, commands, module_base, frame);
    w_u16(state + 0x2c, 0);
    w_u32(object + 0x104, 0);
    w_u16(state + 4, 0x90f);
    w_u16(state + 0xa, 100);
    w_u16(state + 8, 0);
    w_u16(state + 6, 0x157c);
    w_u16(state + 0xc, 0);
}

void G3AirpodUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000002ec, "G3L_62");
    uint32 frame = guest_stack - 0x40;
    uint32 state = object + 0x104;
    if (r_s16(state + 0x2c) != 0)
    {
        uint32 ratio = r_u32(state + 0x18);
        uint32 sound = (uint32)((sint32)(ratio * 400) >> 12) - 200;
        w_u16(state + 8, sound);
        G3Function80039F5C(0, state, (sint32)ratio < 0x1000 ? 1 : 0, frame);
        w_u16(state + 0x2c, 0);
    }
    else
        G3Function80039F5C(0, state, 0, frame);
    uint32 phase = r_u16(state + 0x26);
    uint32 increment = r_u16(state + 0x28);
    uint32 sum = phase + increment;
    sint32 emissions = (sint32)(sum << 16) >> 24;
    sint32 points_count = r_s16(state + 0x1c);
    w_u16(state + 0x26, sum & 0xff);
    if (points_count == 0 || emissions <= 0)
        return;
    w_u16(frame + 0x24, 0);
    w_u16(frame + 0x22, 0);
    w_u16(frame + 0x20, 0);
    uint32 velocity_z = r_u16(state + 0x20);
    w_u16(frame + 0x1a, 0);
    w_u16(frame + 0x18, 0);
    w_u16(frame + 0x24, velocity_z);
    w_u16(frame + 0x1c, r_u16(state + 0x22));
    do
    {
        --emissions;
        sint32 random = (sint32)G3SdkRandom80027FA8();
        sint32 count = r_s16(state + 0x1c);
        sint32 choice = count == 0 ? random : random % count;
        uint32 points = r_u32(state + 0x14);
        w_u16(frame + 0x28, r_u16(points + (uint32)choice * 6));
        random = (sint32)G3SdkRandom80027FA8();
        count = r_s16(state + 0x1c);
        choice = count == 0 ? random : random % count;
        points = r_u32(state + 0x14);
        w_u16(frame + 0x2a, r_u16(points + (uint32)choice * 6 + 2));
        random = (sint32)G3SdkRandom80027FA8();
        count = r_s16(state + 0x1c);
        choice = count == 0 ? random : random % count;
        points = r_u32(state + 0x14);
        uint32 picked_z = r_u16(points + (uint32)choice * 6 + 4);
        uint32 x = r_u16(frame + 0x28);
        w_u16(frame + 0x2c, picked_z);
        uint32 ox = r_u16(object + 0x58);
        w_u16(frame + 0x28, x + ox);
        uint32 y = r_u16(frame + 0x2a);
        uint32 oy = r_u16(object + 0x5a);
        w_u16(frame + 0x2a, y + oy);
        uint32 oz = r_u16(object + 0x5c);
        w_u16(frame + 0x2c, picked_z + oz);
        uint32 factor = (uint32)r_s16(state + 0x1e);
        uint32 lifetime = (uint32)r_s16(state + 0x24);
        w_u32(frame + 0x10, lifetime);
        uint32 scale = r_u32(state + 0x10);
        w_u32(frame + 0x14, scale);
        G3Function80045448(frame + 0x28, frame + 0x20, frame + 0x18, factor, frame);
    } while (emissions != 0);
}

void G3AirpodCollide(uint32 object)
{
    FUNCTION_MARKER(0x000004d0, "G3L_62");
    uint32 state = object + 0x104;
    uint32 player = r_u32(0x8009efc4);
    uint32 contact = r_u32(object + 0xb0);
    uint32 resource = r_u32(player + 0x1c);
    sint32 type = r_s16(contact + 4);
    uint32 player_data = r_u32(resource + 0x1c);
    if (type != 1)
        return;
    uint32 air = r_u16(player_data + 0x252);
    uint32 gain = r_u16(state + 0x2a);
    air += gain;
    w_u16(player_data + 0x252, air);
    sint32 maximum = r_s16(player_data + 0xb0);
    uint32 maximum_bits = r_u16(player_data + 0xb0);
    if (maximum < ((sint32)(air << 16) >> 16))
        w_u16(player_data + 0x252, maximum_bits);
    uint32 current = (uint32)r_s16(player_data + 0x252);
    sint32 denominator = r_s16(player_data + 0xb0);
    uint32 numerator = current << 12;
    uint32 ratio = denominator == 0 ? ((sint32)numerator < 0 ? 1u : 0xffffffffu) : (uint32)((sint64)(sint32)numerator / denominator);
    w_u16(state + 0x2c, (uint32)type);
    w_u32(state + 0x18, ratio);
}

uint32 G3AlfredEvent(uint32 object, uint32 message, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_57");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3AlfredInitialize(object, frame);
        return 1;
    case 2:
        G3AlfredUpdate(object, frame);
        return 1;
    case 3:
        G3AlfredCollide(object, frame);
        return 1;
    case 8:
    case 9:
    case 2020:
        w_u16(object + 0x11c, 1);
        w_u16(object + 0x120, 0);
        return 1;
    default:
        return 0;
    }
}

void G3AlfredConfigure(uint32 object, uint32 commands, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000000b0, "G3L_57");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    w_u32(guest_stack + 4, commands);
    if (commands == 0)
        return;
    while (G3Function80085DFC(frame + 0x2c, frame + 0x10, frame + 0x14, frame + 0x18) != 0)
    {
        sint32 kind = r_s16(frame + 0x10);
        if (kind == 0x31)
        {
            uint32 data = r_u32(frame + 0x14);
            uint32 angle = (uint32)r_s16(data) << 12;
            sint64 product = (sint64)(sint32)angle * (sint32)0x91a2b3c5;
            uint32 high = (uint32)((uint64)product >> 32);
            uint32 sum = high + angle;
            uint32 quotient = (uint32)((sint32)sum >> 11) - (uint32)((sint32)angle >> 31);
            w_u16(state + 0x16, quotient);
        }
        else if (kind == 0xa9)
        {
            uint32 data = r_u32(frame + 0x14);
            w_u16(state + 0x14, r_u16(data));
        }
        else if (kind == 0x25)
        {
            uint32 data = r_u32(frame + 0x14);
            w_u32(state, r_u32(data));
        }
    }
}

void G3AlfredInitialize(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000018c, "G3L_57");
    uint32 frame = guest_stack - 0x20;
    uint32 state = object + 0x104;
    w_u16(state + 0x14, 0);
    w_u16(state + 0x16, 0x5b);
    uint32 resource = r_u32(object + 0x1c);
    w_u32(object + 0x104, 0);
    uint32 commands = r_u32(resource + 0x1c);
    G3AlfredConfigure(object, commands, frame);
    commands = r_u32(object + 0x28);
    G3AlfredConfigure(object, commands, frame);
    w_u32(object + 0x150, 0);
    w_u16(object + 0x6e, 0);
    w_u16(object + 0x5e, 0);
    w_u16(state + 0xa, 14);
    w_u16(state + 8, 0);
    w_u16(state + 6, 0);
    w_u16(state + 4, 0);
    w_u16(state + 0x12, 0xffff);
    w_u16(state + 0x18, 0);
    resource = r_u32(object + 0x1c);
    uint32 models = r_u32(resource + 0xc);
    uint32 model = r_u32(models);
    G3Function80039834(model);
    uint32 flags = r_u32(object + 0x10);
    w_u16(object + 0x9e, 0);
    w_u32(object + 0x10, flags | 0x80);
}

void G3AlfredFacePlayer(uint32 object, uint32 distance_squared, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000023c, "G3L_57");
    uint32 player = r_u32(0x8009efc4);
    uint32 frame = guest_stack - 0x30;
    uint32 coordinate = r_u16(object + 0x58);
    uint32 target = r_u16(player + 0x58);
    uint32 delta_x = target - coordinate;
    sint32 x = (sint16)delta_x;
    uint32 square_x = (uint32)(x * x);
    w_u16(frame + 0x10, delta_x);
    target = r_u16(player + 0x5a);
    coordinate = r_u16(object + 0x5a);
    uint32 delta_y = target - coordinate;
    sint32 y = (sint16)delta_y;
    uint32 square_y = (uint32)(y * y);
    w_u16(frame + 0x12, delta_y);
    target = r_u16(player + 0x5c);
    coordinate = r_u16(object + 0x5c);
    uint32 delta_z = target - coordinate;
    sint32 z = (sint16)delta_z;
    uint32 square_z = (uint32)(z * z);
    uint32 state = object + 0x104;
    w_u16(frame + 0x14, delta_z);
    uint32 squared = square_x + square_y + square_z;
    uint32 angle = G3Function8002F240((uint32)y, (uint32)x, frame);
    uint32 rotated = (angle + 0x400) << 16;
    uint32 step = (uint32)r_s16(state + 0x16);
    G3Function80038AA8(object + 0x64, (uint32)((sint32)rotated >> 16), step);
    if (distance_squared != 0)
        w_u32(distance_squared, squared);
}

uint32 G3AlfredSpeechFrame(uint32 object)
{
    FUNCTION_MARKER(0x0000031c, "G3L_57");
    uint32 mode = r_u32(0x800aa7d4);
    uint32 count = r_u8(0x800aa7f0);
    uint32 state = object + 0x104;
    if (mode != 3 || r_u8(0x800aa7d3) != 2 || count == 0)
        return 0;
    uint32 game = r_u32(0x80097048);
    w_u32(object + 0xa4, object + 0x108);
    uint32 now = r_u32(game + 0x154);
    uint32 start = r_u32(0x800aa7e4);
    uint32 index = ((now - start) >> 1) + 1;
    if ((sint32)index < 0)
        index = 0;
    if ((sint32)index >= (sint32)count)
        index = count - 1;
    uint32 glyph = r_u8(0x800aa7f1 + index);
    w_u16(state + 4, glyph << 2);
    return 1;
}

void G3AlfredUpdate(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000003b4, "G3L_57");
    uint32 frame = guest_stack - 0x28;
    uint32 mode = r_u32(object + 0x150);
    uint32 state = object + 0x104;
    if (mode == 0)
    {
        G3AlfredFacePlayer(object, frame + 0x10, frame);
        G3Function80029F0C(object, 0xffffffff, frame);
        if (r_s16(state + 0x18) == 0)
            return;
        if (G3Function8003525C() != 0)
        {
            w_u32(object + 0x150, 1);
            w_u16(object + 0x6e, 0);
            w_u16(object + 0x5e, 0);
            uint32 key = (uint32)r_s16(state + 0x14);
            G3Function800359EC(key, 2, frame);
            w_u16(state + 0x1a, 0);
            return;
        }
        if (r_s16(state + 0x1c) == 0)
        {
            uint32 script = r_u32(object + 0x104);
            if (script != 0)
                G3Function80018DE0(object, script + 4, frame);
        }
        uint32 timer = r_u16(state + 0x1c) + 1;
        w_u16(state + 0x1c, timer);
        if ((sint16)timer >= 31)
            w_u16(state + 0x18, 0);
    }
    else if (mode == 1)
    {
        G3AlfredFacePlayer(object, 0, frame);
        G3Function80029F0C(object, 0xffffffff, frame);
        uint32 speaking = G3AlfredSpeechFrame(object);
        sint32 previous = r_s16(state + 0x1a);
        if (previous != 0)
        {
            if (speaking == 0)
            {
                w_u32(object + 0x150, 0);
                w_u16(state + 4, 0);
                w_u16(state + 0x18, 0);
            }
        }
        else if (speaking != 0)
        {
            uint32 script = r_u32(object + 0x104);
            if (script != 0)
                G3Function80018DE0(object, script + 4, frame);
        }
        w_u16(state + 0x1a, speaking);
    }
}

void G3AlfredCollide(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000524, "G3L_57");
    uint32 frame = guest_stack - 0x28;
    uint32 state = object + 0x104;
    uint32 contact = r_u32(object + 0xb0);
    uint32 other = r_u32(contact + 0x14);
    G3Function8003C548(contact, frame + 0x10, frame + 0x11);
    if (r_s16(state + 0x18) != 0)
        return;
    if (other != r_u32(0x8009efc4))
        return;
    uint32 second_type = r_u8(frame + 0x11);
    if (second_type - 6 >= 4)
        return;
    uint32 first_type = r_u8(frame + 0x10);
    if (first_type == 9 || first_type - 3 < 3)
    {
        w_u16(state + 0x18, 1);
        w_u16(state + 0x1c, 0);
    }
}

uint32 G3AllwhackEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000000, "G3L_55");
    uint32 frame = guest_stack - 0x18;
    uint32 index = message - 1;
    if (index >= 8) return 0;
    uint32 target = r_u32(module_base + 0x13b8 + index * 4) - module_base;
    switch (target)
    {
    case 0x3c: G3AllwhackCommit(payload); return 1;
    case 0x4c: G3AllwhackInitialize(object, game, module_base); return 1;
    case 0x5c: G3AllwhackUpdate(object, module_base, frame); return 1;
    case 0x6c: G3AllwhackHit(object, payload, frame); return 1;
    case 0x7c: G3AllwhackCollide(object, frame); return 1;
    case 0x8c: return 0;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "Unresolved Allwhack event target %08X\n", target + module_base);
#endif
        }
        abort();
    }
}

void G3AllwhackSetState(uint32 object, uint32 mode, uint32 at_end)
{
    FUNCTION_MARKER(0x000000a4, "G3L_55");
    uint32 state = object + 0x104;
    w_u32(object + 0x150, (sint32)mode < 2 ? mode : 0);
    uint32 actual_mode = r_u32(object + 0x150);
    uint32 animation = 0xffffffff;
    if (actual_mode == 0)
    {
        animation = r_u16(state + 4);
        w_u16(object + 0x6e, animation);
    }
    else if (actual_mode == 1)
    {
        animation = r_u16(state + 6);
        w_u16(object + 0x6e, animation);
        sint32 kind = r_s16(state);
        if (kind == 0 || kind == 4)
        {
            w_u32(object + 0x148, 0);
            w_u32(object + 0x14c, 0xffffffff);
        }
        else if (kind == 6) w_u32(object + 0x148, 0);
    }
    uint32 enabled = (~(uint32)(sint32)(sint16)animation) >> 31;
    w_u32(state + 8, enabled);
    uint32 descriptor = r_u32(object + 0x1c);
    if (r_s16(descriptor + 0xa) > 0)
    {
        uint32 frame = 0;
        if (enabled != 0 && at_end != 0)
        {
            uint32 index = r_u16(object + 0x6e);
            uint32 table = r_u32(descriptor + 0x10);
            uint32 sequence = r_u32(table + index * 4);
            frame = r_u16(sequence + 2) - 1;
        }
        w_u16(object + 0x5e, frame);
    }
}

void G3AllwhackInitialize(uint32 object, uint32 game, uint32 module_base)
{
    FUNCTION_MARKER(0x0000019c, "G3L_55");
    uint32 settings = r_u32(object + 0x28);
    uint32 state = object + 0x104;
    if (settings == 0) settings = r_u32(r_u32(object + 0x1c) + 0x1c);
    uint32 placement = r_u32(object + 0x20);
    if (placement != 0)
    {
        uint32 flags = r_u32(placement + 0x18);
        if ((flags & 0x800) != 0)
        {
            w_u32(placement + 0x18, flags & 0xfffff7ff);
            if (settings != 0) w_u16(settings + 2, 1);
        }
    }
    w_u16(state, settings != 0 ? r_u16(settings) : 0);
    w_u16(state + 2, settings != 0 ? r_u16(settings + 2) : 1);
    for (uint32 axis = 0; axis != 3; ++axis)
    {
        uint32 minimum = settings != 0 ? r_u16(settings + 4 + axis * 2) : r_u16(object + 0x58 + axis * 2) - 1500;
        w_u16(state + 0xc + axis * 2, minimum);
        uint32 maximum = settings != 0 ? r_u16(settings + 0xa + axis * 2) : r_u16(object + 0x58 + axis * 2) + 1500;
        w_u16(state + 0x12 + axis * 2, maximum);
    }
    sint32 kind = r_s16(state);
    w_u16(state + 4, 0xffff);
    w_u16(state + 6, 0);
    w_u32(state + 0x18, 0);
    w_u32(state + 0x1c, 0);
    w_u32(state + 0x20, 0);
    if (kind == 0 || kind == 2 || kind == 4)
    {
        uint32 count = G3RemainingPlacementCount80045FE8(object) + 1;
        sint32 adjusted = (sint32)(count + (count >> 31));
        w_u16(state + 2, (uint32)(adjusted >> 1));
    }
    else if (kind == 3 || kind == 5) w_u16(state + 2, G3RemainingPlacementCount80045FE8(object));
    else if (kind == 7) w_u16(state + 2, r_u16(game + 0x68e8));
    if (settings != 0) w_u16(settings + 2, r_u16(state + 2));
    uint32 unsigned_kind = (uint32)r_s16(state);
    if (unsigned_kind < 9)
    {
        uint32 target = r_u32(module_base + 0x13d8 + unsigned_kind * 4) - module_base;
        switch (target)
        {
        case 0x3e0:
        {
            w_u32(object + 0x148, 0xffffffff);
            uint32 flags = r_u32(object + 0x14);
            w_u32(object + 0x14c, 0);
            w_u32(object + 0x14, flags | 0x20000000);
            break;
        }
        case 0x400:
        {
            uint32 flags = r_u32(object + 0x10);
            w_u16(object + 0x68, 0x1900);
            w_u16(object + 0x6c, 0x1900);
            w_u32(object + 0x10, flags | 0x400);
            break;
        }
        case 0x41c:
        {
            w_u16(state + 6, 0xffff);
            uint32 descriptor = r_u32(object + 0x1c);
            w_u32(descriptor + 0x30, r_u32(descriptor + 0x30) | 0x8000);
            break;
        }
        case 0x440: w_u16(state + 4, 0); w_u16(state + 6, 1); break;
        case 0x44c: break;
        default:
            {
#ifndef NDEBUG
            fprintf(stderr, "Unresolved Allwhack initialization target %08X\n", target + module_base);
#endif
            }
            abort();
        }
    }
    uint32 mode = 0;
    if (r_s16(state + 2) <= 0 && r_s16(state) != 7) mode = 1;
    G3AllwhackSetState(object, mode, mode);
}

uint32 G3AllwhackDistanceXY(uint32 object, uint32 position)
{
    FUNCTION_MARKER(0x00001008, "G3L_55");
    uint32 coordinate = (uint32)r_s16(position);
    uint32 origin = (uint32)r_s16(object + 0x58);
    uint32 x = coordinate - origin;
    coordinate = (uint32)r_s16(position + 2);
    origin = (uint32)r_s16(object + 0x5a);
    uint32 y = coordinate - origin;
    return x * x + y * y;
}

void G3AllwhackCommit(uint32 placement)
{
    FUNCTION_MARKER(0x00001040, "G3L_55");
    uint32 flags = r_u32(placement + 0x18);
    if ((flags & 0x400) != 0) w_u32(placement + 0x18, (flags & 0xfffffbff) | 0x800);
}

void G3AllwhackBurst8(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000910, "G3L_55");
    uint32 frame = guest_stack - 0x40;
    uint32 x_trig = G3Function8005B470((uint32)r_s16(object + 0x64));
    sint32 x = (sint32)(x_trig * (uint32)(72)) / 4096;
    uint32 y_trig = G3Function8005B590((uint32)r_s16(object + 0x64));
    sint32 y = (sint32)(y_trig * (uint32)(72)) / 4096;
    uint32 px = r_u16(object + 0x58);
    uint32 pz = r_u16(object + 0x5c);
    w_u16(object + 0x58, px + (uint32)x);
    uint32 py = r_u16(object + 0x5a);
    w_u16(object + 0x5c, pz + 525);
    w_u16(object + 0x5a, py + (uint32)y);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x18, 0xfffffff8);
    w_u32(frame + 0x1c, 0);
    w_u32(frame + 0x20, 0);
    w_u32(frame + 0x24, 0);
    G3Function800460E8(object, 0, 0, 0, frame);
    w_u16(object + 0x58, r_u16(object + 0x58) - (uint32)x * 2);
    w_u16(object + 0x5a, r_u16(object + 0x5a) - (uint32)y * 2);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x18, 0xfffffff8);
    w_u32(frame + 0x1c, 0);
    w_u32(frame + 0x20, 0);
    w_u32(frame + 0x24, 0);
    G3Function800460E8(object, 0, 0, 0, frame);
    px = r_u16(object + 0x58);
    pz = r_u16(object + 0x5c);
    w_u16(object + 0x58, px + (uint32)x);
    py = r_u16(object + 0x5a);
    w_u16(object + 0x5c, pz - 525);
    w_u16(object + 0x5a, py + (uint32)y);
}

void G3AllwhackBurst2(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000bfc, "G3L_55");
    uint32 frame = guest_stack - 0x48;
    uint32 x_trig = G3Function8005B470((uint32)r_s16(object + 0x64));
    sint32 x = (sint32)(x_trig * (uint32)(272)) / 4096;
    uint32 y_trig = G3Function8005B590((uint32)r_s16(object + 0x64));
    sint32 y = (sint32)(y_trig * (uint32)(272)) / 4096;
    uint32 velocity_x = (uint32)(sint32)(sint16)(y / 14);
    uint32 velocity_y = (uint32)(sint32)(sint16)(-x / 14);
    uint32 px = r_u16(object + 0x58);
    uint32 pz = r_u16(object + 0x5c);
    w_u16(object + 0x58, px + (uint32)x);
    uint32 py = r_u16(object + 0x5a);
    w_u16(object + 0x5c, pz + 136);
    w_u16(object + 0x5a, py + (uint32)y);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x18, 0xfffffffb);
    w_u32(frame + 0x1c, 100);
    w_u32(frame + 0x20, 100);
    w_u32(frame + 0x24, 100);
    G3Function800460E8(object, velocity_x, velocity_y, 42, frame);
    w_u16(object + 0x58, r_u16(object + 0x58) - (uint32)x * 2);
    w_u16(object + 0x5a, r_u16(object + 0x5a) - (uint32)y * 2);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x18, 0xfffffffb);
    w_u32(frame + 0x1c, 100);
    w_u32(frame + 0x20, 100);
    w_u32(frame + 0x24, 100);
    G3Function800460E8(object, velocity_x, velocity_y, 42, frame);
    px = r_u16(object + 0x58);
    pz = r_u16(object + 0x5c);
    w_u16(object + 0x58, px + (uint32)x);
    py = r_u16(object + 0x5a);
    w_u16(object + 0x5c, pz - 136);
    w_u16(object + 0x5a, py + (uint32)y);
}

void G3AllwhackBurst0(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000a4c, "G3L_55");
    uint32 frame = guest_stack - 0x30;
    w_u16(object + 0x5c, r_u16(object + 0x5c) + 125);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x18, 0);
    w_u32(frame + 0x1c, 0);
    w_u32(frame + 0x20, 0);
    w_u32(frame + 0x24, 0);
    G3Function800460E8(object, 0, 0, 0, frame);
    w_u16(object + 0x5c, r_u16(object + 0x5c) - 250);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x18, 0);
    w_u32(frame + 0x1c, 0);
    w_u32(frame + 0x20, 0);
    w_u32(frame + 0x24, 0);
    G3Function800460E8(object, 0, 0, 0, frame);
    w_u16(object + 0x5c, r_u16(object + 0x5c) + 125);
}

void G3AllwhackBurst4(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000ae8, "G3L_55");
    uint32 frame = guest_stack - 0x38;
    uint32 x_trig = G3Function8005B590((uint32)r_s16(object + 0x64));
    sint32 x = (sint32)(x_trig * (uint32)(-240)) / 4096;
    uint32 y_trig = G3Function8005B470((uint32)r_s16(object + 0x64));
    sint32 y = (sint32)(y_trig * (uint32)(240)) / 4096;
    w_u16(object + 0x58, r_u16(object + 0x58) + (uint32)x);
    w_u16(object + 0x5a, r_u16(object + 0x5a) + (uint32)y);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x18, 0);
    w_u32(frame + 0x1c, 0);
    w_u32(frame + 0x20, 0);
    w_u32(frame + 0x24, 0);
    G3Function800460E8(object, 0, 0, 0, frame);
    w_u16(object + 0x5c, r_u16(object + 0x5c) - 250);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x18, 0);
    w_u32(frame + 0x1c, 0);
    w_u32(frame + 0x20, 0);
    w_u32(frame + 0x24, 0);
    G3Function800460E8(object, 0, 0, 0, frame);
    uint32 pz = r_u16(object + 0x5c);
    uint32 py = r_u16(object + 0x5a);
    w_u16(object + 0x5c, pz + 250);
    uint32 px = r_u16(object + 0x58);
    w_u16(object + 0x5a, py - (uint32)y);
    w_u16(object + 0x58, px - (uint32)x);
}

void G3AllwhackBurst5(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000da0, "G3L_55");
    uint32 frame = guest_stack - 0x40;
    uint32 x_trig = G3Function8005B590((uint32)r_s16(object + 0x64));
    sint32 x = (sint32)(x_trig * (uint32)(100)) / 4096;
    uint32 y_trig = G3Function8005B470((uint32)r_s16(object + 0x64));
    sint32 y = (sint32)(y_trig * (uint32)(-100)) / 4096;
    uint32 velocity_x_trig = G3Function8005B590((uint32)r_s16(object + 0x64));
    sint32 velocity_x = (sint32)(velocity_x_trig * (uint32)(20)) / 4096;
    uint32 velocity_y_trig = G3Function8005B470((uint32)r_s16(object + 0x64));
    sint32 velocity_y = (sint32)(velocity_y_trig * (uint32)(-20)) / 4096;
    uint32 px = r_u16(object + 0x58);
    uint32 pz = r_u16(object + 0x5c);
    w_u16(object + 0x58, px + (uint32)x);
    uint32 py = r_u16(object + 0x5a);
    w_u16(object + 0x5c, pz + 250);
    w_u16(object + 0x5a, py + (uint32)y);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x18, 0xfffffffb);
    w_u32(frame + 0x1c, 100);
    w_u32(frame + 0x20, 100);
    w_u32(frame + 0x24, 100);
    G3Function800460E8(object, (uint32)velocity_x, (uint32)velocity_y, 42, frame);
    pz = r_u16(object + 0x5c);
    py = r_u16(object + 0x5a);
    w_u16(object + 0x5c, pz - 250);
    px = r_u16(object + 0x58);
    w_u16(object + 0x5a, py - (uint32)y);
    w_u16(object + 0x58, px - (uint32)x);
}

void G3AllwhackBurst3(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000ee0, "G3L_55");
    uint32 frame = guest_stack - 0x40;
    uint32 x_trig = G3Function8005B590((uint32)r_s16(object + 0x64));
    sint32 x = (sint32)(x_trig * (uint32)(100)) / 4096;
    uint32 y_trig = G3Function8005B470((uint32)r_s16(object + 0x64));
    sint32 y = (sint32)(y_trig * (uint32)(-100)) / 4096;
    uint32 velocity_x_trig = G3Function8005B590((uint32)r_s16(object + 0x64));
    sint32 velocity_x = (sint32)(velocity_x_trig * (uint32)(60)) / 4096;
    uint32 velocity_y_trig = G3Function8005B470((uint32)r_s16(object + 0x64));
    sint32 velocity_y = (sint32)(velocity_y_trig * (uint32)(-60)) / 4096;
    uint32 px = r_u16(object + 0x58);
    uint32 py = r_u16(object + 0x5a);
    w_u16(object + 0x58, px + (uint32)x);
    w_u16(object + 0x5a, py + (uint32)y);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x18, 0xfffffffb);
    w_u32(frame + 0x1c, 100);
    w_u32(frame + 0x20, 100);
    w_u32(frame + 0x24, 100);
    G3Function800460E8(object, (uint32)velocity_x, (uint32)velocity_y, 48, frame);
    px = r_u16(object + 0x58);
    py = r_u16(object + 0x5a);
    w_u16(object + 0x58, px - (uint32)x);
    w_u16(object + 0x5a, py - (uint32)y);
}

void G3AllwhackHit(uint32 object, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000007c0, "G3L_55");
    uint32 frame = guest_stack - 0x20;
    sint32 kind = r_s16(object + 0x104);
    uint32 settings = r_u32(object + 0x28);
    uint32 state = object + 0x104;
    if (kind == 7) return;
    if (kind == 6 && payload != 0)
    {
        if (r_u32(object + 0x150) == 1) G3Function80038798(object, 0, 0, frame);
        return;
    }
    if (r_u32(object + 0x150) != 0) return;
    sint32 remaining = r_s16(state + 2);
    uint32 remaining_bits = r_u16(state + 2);
    if (remaining > 0)
    {
        w_u16(state + 2, remaining_bits - 1);
        if (settings != 0) w_u16(settings + 2, r_u16(settings + 2) - 1);
    }
    else if (r_s16(state) != 8) return;
    G3AllwhackSetState(object, 1, 0);
    kind = r_s16(state);
    if (kind == 2) G3AllwhackBurst2(object, frame);
    else if (kind == 0) G3AllwhackBurst0(object, frame);
    else if (kind == 4) G3AllwhackBurst4(object, frame);
    else if (kind == 8) G3AllwhackBurst8(object, frame);
}

void G3AllwhackChildren(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001068, "G3L_55");
    uint32 frame = guest_stack - 0x30;
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 settings = r_u32(descriptor + 0x1c);
    uint32 remaining = r_u16(settings + 2);
    uint32 state = object + 0x104;
    w_u16(state + 2, remaining);
    if ((sint16)remaining > 0 && r_u32(state + 0x18) == 0)
    {
        uint32 resource = G3Function8003AA48(module_base + 0x1410);
        w_u32(frame + 0x10, 0xffffff38);
        w_u32(frame + 0x14, 200);
        w_u32(frame + 0x18, 0xea6);
        G3AllwhackSpawnChild(object, resource, object + 0x11c, 0xfffffc7c, frame);
    }
    if (r_s16(state + 2) >= 2 && r_u32(state + 0x1c) == 0)
    {
        uint32 resource = G3Function8003AA48(module_base + 0x1410);
        w_u32(frame + 0x10, 0xffffff38);
        w_u32(frame + 0x14, 200);
        w_u32(frame + 0x18, 0x15a);
        G3AllwhackSpawnChild(object, resource, state + 0x1c, 900, frame);
    }
    if (r_s16(state + 2) >= 3 && r_u32(state + 0x20) == 0)
    {
        uint32 resource = G3Function8003AA48(module_base + 0x1410);
        w_u32(frame + 0x10, 0xffffff38);
        w_u32(frame + 0x14, 280);
        w_u32(frame + 0x18, 0);
        G3AllwhackSpawnChild(object, resource, state + 0x20, 0, frame);
    }
}

void G3AllwhackSpawnChild(uint32 object, uint32 resource, uint32 output, uint32 lateral, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000011a8, "G3L_55");
    uint32 frame = guest_stack - 0x30;
    uint32 forward = r_u32(guest_stack + 0x10);
    uint32 yaw_offset = r_u32(guest_stack + 0x18);
    if (resource == 0) return;
    uint32 child = G3Function8002A454(object, resource, frame);
    w_u32(output, child);
    if (child == 0) return;
    uint32 sine = G3Function8005B470((uint32)r_s16(object + 0x64));
    sint32 lateral_x = (sint32)(lateral * sine) / 4096;
    uint32 cosine = G3Function8005B590((uint32)r_s16(object + 0x64));
    sint32 lateral_y = (sint32)(lateral * cosine) / 4096;
    cosine = G3Function8005B590((uint32)r_s16(object + 0x64));
    sint32 forward_x = (sint32)(forward * cosine) / 4096;
    uint32 x = (uint32)lateral_x - (uint32)forward_x;
    sine = G3Function8005B470((uint32)r_s16(object + 0x64));
    sint32 forward_y = (sint32)(forward * sine) / 4096;
    uint32 y = (uint32)lateral_y + (uint32)forward_y;
    w_u16(child + 0x68, 0x1000);
    w_u16(child + 0x6a, 0x1000);
    w_u16(child + 0x6c, 0x1000);
    sint32 heading = r_s16(child + 0x64);
    heading += (sint16)yaw_offset;
    w_u16(child + 0x58, r_u16(child + 0x58) + x);
    uint32 py = r_u16(child + 0x5a);
    uint32 pz = r_u16(child + 0x5c);
    w_u16(child + 0x5a, py + y);
    uint32 height = r_u32(guest_stack + 0x14);
    w_u16(child + 0x5c, pz + height);
    w_u16(child + 0x64, (uint32)(heading % 4096));
}

void G3AllwhackCollide(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001314, "G3L_55");
    uint32 frame = guest_stack - 0x18;
    if (r_s16(object + 0x104) != 7) return;
    if (r_u32(object + 0x150) != 0) return;
    if (r_s16(object + 0x106) < 3) return;
    uint32 descriptor = r_u32(object + 0x1c);
    if ((r_u32(descriptor + 0x30) & 0x8000) == 0) return;
    G3AllwhackSetState(object, 1, 0);
    G3Function800193F8(0x8000, object, frame);
    descriptor = r_u32(object + 0x1c);
    w_u32(descriptor + 0x30, r_u32(descriptor + 0x30) & 0xffff7fff);
}

void G3AllwhackUpdate(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000494, "G3L_55");
    uint32 frame = guest_stack - 0x58;
    uint32 state = object + 0x104;
    uint32 enabled = r_u32(state + 8);
    uint32 settings = r_u32(object + 0x28);
    bool read_mode = true;
    bool skip_animation_effects = false;
    uint32 mode = 0;
    if (enabled != 0)
    {
        uint32 descriptor = r_u32(object + 0x1c);
        uint32 animation = r_u16(object + 0x6e);
        uint32 frame_bits = r_u16(object + 0x5e);
        uint32 animations = r_u32(descriptor + 0x10);
        uint32 sequence = r_u32(animations + animation * 4);
        sint32 old_frame = r_s16(object + 0x5e);
        sint32 length = r_s16(sequence + 2);
        if (old_frame + 1 < length)
        {
            descriptor = r_u32(object + 0x1c);
            w_u16(object + 0x5e, frame_bits + 1);
            if ((r_u32(descriptor + 0x30) & 0x2000) != 0) G3Function8003E314(object, frame);
        }
        else
        {
            mode = r_u32(object + 0x150);
            if (mode == 0)
            {
                if (r_s16(state + 4) < 0) read_mode = false;
                else w_u16(object + 0x5e, 0);
            }
            else if (mode == 1)
            {
                uint32 kind_bits = r_u16(state);
                sint32 kind = (sint16)kind_bits;
                if (kind_bits - 2 < 2 || kind == 5 || kind == 8)
                {
                    G3AllwhackSetState(object, 0, 0);
                    return;
                }
            }
            else skip_animation_effects = true;
        }
    }
    if (!skip_animation_effects)
    {
        if (read_mode) mode = r_u32(object + 0x150);
        if (mode == 1)
        {
            sint32 kind = r_s16(state);
            if (kind == 3)
            {
                if (r_s16(object + 0x5e) == 15) G3AllwhackBurst3(object, frame);
                kind = r_s16(state);
            }
            if (kind == 5 && r_s16(object + 0x5e) == 8) G3AllwhackBurst5(object, frame);
        }
    }
    sint32 kind = r_s16(state);
    if (kind == 1 && r_u32(object + 0x150) == 0)
    {
        uint32 player = r_u32(0x8009efc4);
        w_u16(object + 0x68, 0x1000);
        w_u16(object + 0x6c, 0x1000);
        uint32 distance = G3AllwhackDistanceXY(object, player + 0x58);
        if ((sint32)distance < 16000000)
        {
            uint32 resource = G3Function8003AA48(module_base + 0x13fc);
            uint32 child = G3Function8002A454(object, resource, frame);
            G3AllwhackSetState(object, 1, 0);
            w_u16(settings + 2, 0);
            w_u16(state + 2, 0);
            if (child != 0)
            {
                uint32 words[2];
                for (uint32 i = 0; i != 2; ++i)
                {
                    uint32 source = module_base + 0x1408 + i * 4;
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
                    uint32 destination = frame + 0x10 + i * 4;
                    uint32 high_address = destination + 3;
                    uint32 high_lane = high_address & 3;
                    uint32 lane = destination & 3;
                    w_masked_u32(high_address & 0xfffffffc, words[i] >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
                    w_masked_u32(destination & 0xfffffffc, words[i] << (lane * 8), (15u << lane) & 15);
                }
                G3Function8005BD00(object + 0x60, frame + 0x20);
                ApplyMatrixSV((MATRIX *)psx_addr(frame + 0x20, 20), (SVECTOR *)psx_addr(frame + 0x10, 8), (SVECTOR *)psx_addr(frame + 0x18, 6));
                for (uint32 axis = 0; axis != 3; ++axis)
                {
                    uint32 coordinate = r_u16(child + 0x58 + axis * 2);
                    uint32 offset = r_u16(frame + 0x18 + axis * 2);
                    w_u16(child + 0x58 + axis * 2, coordinate + offset);
                }
                uint32 source = child + 0x58;
                uint32 high_address = source + 3;
                uint32 high_lane = high_address & 3;
                uint32 lane = source & 3;
                uint32 high = r_masked_u32(high_address & 0xfffffffc, (1u << (high_lane + 1)) - 1);
                uint32 low = r_masked_u32(source & 0xfffffffc, (15u << lane) & 15);
                uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
                uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
                uint32 last = (uint32)r_s16(child + 0x5c);
                uint32 destination = child + 0x88;
                high_address = destination + 3;
                high_lane = high_address & 3;
                lane = destination & 3;
                w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1u << (high_lane + 1)) - 1);
                w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15u << lane) & 15);
                w_u16(child + 0x8c, last);
                w_u16(child + 0x60, 0);
                w_u16(child + 0x62, 0);
                sint32 heading = r_s16(object + 0x64);
                w_u16(child + 0x64, (uint32)(heading % 4096));
                uint32 first = r_u32(state + 0xc);
                uint32 second = r_u32(state + 0x10);
                uint32 third = r_u32(state + 0x14);
                w_u32(child + 0x104, first);
                w_u32(child + 0x108, second);
                w_u32(child + 0x10c, third);
                uint32 placement = r_u32(object + 0x20);
                w_u32(placement + 0x18, r_u32(placement + 0x18) | 0x400);
            }
        }
        w_u16(object + 0x68, 0x1900);
        w_u16(object + 0x6c, 0x1900);
        kind = r_s16(state);
    }
    if (kind == 7) G3AllwhackChildren(object, module_base, frame);
}
