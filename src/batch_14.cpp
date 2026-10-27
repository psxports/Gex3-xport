#include <string.h>
#include "xport.h"
#include "psx.h"
#include "game_functions.h"

uint32 G3Function8001E7F4(uint32 x0, uint32 y0, uint32 x1, uint32 y1, uint32 pool, uint32 order_table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8001E7F4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 end = r_u32(pool + 8);
    uint32 packet = r_u32(pool + 4);
    if (packet >= end - 0x30)
        return 0;
    uint32 game = r_u32(0x80097048);
    uint32 index = r_u32(game + 0x58);
    uint32 origin = r_u16(0x800a3cd4 + index * 92);
    w_u16(frame + 0x10, origin + x0);
    index = r_u32(game + 0x58);
    origin = r_u16(0x800a3cd6 + index * 92);
    w_u16(frame + 0x14, x1 - x0);
    w_u16(frame + 0x16, y1 - y0);
    w_u16(frame + 0x12, origin + y0);
    SetDrawArea((DR_AREA *)psx_addr(packet, 12), (const PSX_RECT *)psx_addr(frame + 0x10, 8));
    game = r_u32(0x80097048);
    uint32 count = r_u32(game + 0x28);
    uint32 link = r_u32(order_table + (count << 2) - 4);
    w_u32(packet, (link & 0xffffff) | 0x02000000);
    count = r_u32(game + 0x28);
    w_u32(order_table + (count << 2) - 4, packet & 0xffffff);
    uint32 next = r_u32(pool + 4) + 0xc;
    uint32 used = r_u32(pool) + 1;
    w_u32(pool + 4, next);
    w_u32(pool, used);
    return next;
}

uint32 G3Function8001E340(uint32 radius, uint32 pool, uint32 order_table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8001E340, "SLUS_008.06");
    uint32 frame = guest_stack - 0x1f8;
    w_u32(frame + 0x200, order_table);
    for (uint32 index = 0; index != 33; ++index)
    {
        w_u16(frame + 0x30 + index * 2, index << 4);
        w_u16(frame + 0x78 + index * 2, (index * 15) / 2);
    }
    sint32 signed_radius = (sint32)(radius << 16) >> 16;
    for (uint32 index = 0; index != 65; ++index)
    {
        uint32 value = G3Function8005B470(2048 - index * 32);
        sint32 component = (sint32)(value << 16) >> 16;
        uint32 product = (uint32)component * (uint32)signed_radius;
        sint32 scaled = (sint32)(product << 9) / 320;
        w_u16(frame + 0xc0 + index * 2, (uint32)(scaled >> 12) + 256);
        value = 0 - G3Function8005B590(1024 + index * 32);
        component = (sint32)(value << 16) >> 16;
        product = (uint32)component * (uint32)signed_radius;
        w_u16(frame + 0x148 + index * 2, (uint32)((sint32)product >> 12) + 120);
    }
    for (uint32 index = 0; index != 32; ++index)
    {
        uint32 x0 = r_s16(frame + 0x30 + index * 2);
        uint32 x1 = r_s16(frame + 0x30 + (index + 1) * 2);
        uint32 color = r_u32(0x80097048) + 0xec;
        w_u32(frame + 0x10, 0);
        uint32 x2 = r_s16(frame + 0xc0 + (index + 16) * 2);
        w_u32(frame + 0x14, x2);
        uint32 mirror = index <= 16 ? 16 - index : index - 16;
        uint32 y2 = r_s16(frame + 0x148 + mirror * 2);
        w_u32(frame + 0x18, y2);
        uint32 x3 = r_s16(frame + 0xc0 + (index + 17) * 2);
        w_u32(frame + 0x1c, x3);
        mirror = index <= 15 ? 15 - index : index - 15;
        uint32 y3 = r_s16(frame + 0x148 + mirror * 2);
        uint32 order = r_u32(frame + 0x200);
        w_u32(frame + 0x24, pool);
        w_u32(frame + 0x28, order);
        w_u32(frame + 0x20, y3);
        G3Function8001E140(color, x0, 0, x1, 0, x2, y2, x3, y3, pool, order);
    }
    for (uint32 index = 0; index != 32; ++index)
    {
        uint32 color = r_u32(0x80097048) + 0xec;
        uint32 x0 = r_s16(frame + 0xc0 + (index + 16) * 2);
        uint32 mirror = index < 16 ? index + 48 : 80 - index;
        uint32 y0 = r_s16(frame + 0x148 + mirror * 2);
        uint32 x1 = r_s16(frame + 0xc0 + (index + 17) * 2);
        mirror = index >= 15 ? index - 15 : 15 - index;
        uint32 y1 = r_s16(frame + 0x148 + (64 - mirror) * 2);
        uint32 order = r_u32(frame + 0x200);
        w_u32(frame + 0x10, y1);
        uint32 x2 = r_s16(frame + 0x30 + index * 2);
        w_u32(frame + 0x18, 240);
        w_u32(frame + 0x14, x2);
        uint32 x3 = r_s16(frame + 0x30 + (index + 1) * 2);
        w_u32(frame + 0x20, 240);
        w_u32(frame + 0x24, pool);
        w_u32(frame + 0x28, order);
        w_u32(frame + 0x1c, x3);
        G3Function8001E140(color, x0, y0, x1, y1, x2, 240, x3, 240, pool, order);
    }
    for (uint32 index = 0; index != 32; ++index)
    {
        uint32 color = r_u32(0x80097048) + 0xec;
        uint32 y0 = r_s16(frame + 0x78 + index * 2);
        uint32 mirror = index <= 16 ? 16 - index : index - 16;
        uint32 x1 = r_s16(frame + 0xc0 + mirror * 2);
        uint32 y1 = r_s16(frame + 0x148 + (index + 16) * 2);
        w_u32(frame + 0x14, 0);
        w_u32(frame + 0x10, y1);
        uint32 y2 = r_s16(frame + 0x78 + (index + 1) * 2);
        w_u32(frame + 0x18, y2);
        mirror = index <= 15 ? 15 - index : index - 15;
        uint32 x3 = r_s16(frame + 0xc0 + mirror * 2);
        uint32 order = r_u32(frame + 0x200);
        w_u32(frame + 0x1c, x3);
        uint32 y3 = r_s16(frame + 0x148 + (index + 17) * 2);
        w_u32(frame + 0x24, pool);
        w_u32(frame + 0x28, order);
        w_u32(frame + 0x20, y3);
        G3Function8001E140(color, 0, y0, x1, y1, 0, y2, x3, y3, pool, order);
    }
    for (uint32 index = 0; index != 32; ++index)
    {
        uint32 color = r_u32(0x80097048) + 0xec;
        uint32 mirror = index < 16 ? index + 48 : 80 - index;
        uint32 x0 = r_s16(frame + 0xc0 + mirror * 2);
        uint32 y0 = r_s16(frame + 0x148 + (index + 16) * 2);
        uint32 y1 = r_s16(frame + 0x78 + index * 2);
        uint32 order = r_u32(frame + 0x200);
        w_u32(frame + 0x10, y1);
        mirror = index >= 15 ? index - 15 : 15 - index;
        uint32 x2 = r_s16(frame + 0xc0 + (64 - mirror) * 2);
        w_u32(frame + 0x14, x2);
        uint32 y2 = r_s16(frame + 0x148 + (index + 17) * 2);
        w_u32(frame + 0x1c, 512);
        w_u32(frame + 0x18, y2);
        uint32 y3 = r_s16(frame + 0x78 + (index + 1) * 2);
        w_u32(frame + 0x24, pool);
        w_u32(frame + 0x28, order);
        w_u32(frame + 0x20, y3);
        G3Function8001E140(color, x0, y0, 512, y1, x2, y2, 512, y3, pool, order);
    }
    return 0;
}

// Native arithmetic helper for the transition owner's signed DIV sites
static uint32 G3TransitionQuotient(uint32 numerator, sint32 divisor)
{
    if (divisor == 0)
        return (sint32)numerator < 0 ? 1 : 0xffffffff;
    return (uint32)((sint64)(sint32)numerator / divisor);
}

// Native call plumbing for the owner's shared quad submission tails
static void G3SubmitTransitionQuad(uint32 frame, uint32 color, uint32 x0, uint32 y0, uint32 x1, uint32 y1, uint32 x2, uint32 y2, uint32 x3, uint32 y3, uint32 pool, uint32 order)
{
    w_u32(frame + 0x28, order);
    w_u32(frame + 0x24, pool);
    G3Function8001E140(color, x0, y0, x1, y1, x2, y2, x3, y3, pool, order);
}

void G3Function8002463C(uint32 game, uint32 order_table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002463C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x68;
    uint32 left = 0;
    uint32 top = 0;
    uint32 right = 0;
    uint32 bottom = 0;
    w_u32(frame + 0x6c, order_table);
    (void)r_u32(game + 0x4c);
    G3Function8004D908(frame + 0x38);
    sint32 initial_count = r_s16(game + 0xf0);
    if (initial_count > 0 || initial_count < -1)
    {
        bool opening = initial_count > 0;
        sint32 mode = r_s16(game + 0xf4);
        switch (mode)
        {
        case 0:
        {
            uint32 numerator;
            sint32 duration;
            if (opening)
            {
                duration = r_s16(game + 0xf2);
                uint32 count = r_s16(game + 0xf0);
                numerator = ((uint32)duration - count) * 200;
            }
            else
            {
                uint32 count = r_s16(game + 0xf0);
                numerator = (0 - count) * 200;
                duration = r_s16(game + 0xf2);
            }
            uint32 radius = (uint32)(sint32)(sint16)G3TransitionQuotient(numerator, duration);
            uint32 order = r_u32(frame + 0x6c);
            uint32 pool = r_u32(game + 0x68);
            G3Function8001E340(radius, pool, order, frame);
            break;
        }
        case 1:
        case 2:
        case 3:
        case 4:
        {
            uint32 numerator;
            sint32 duration;
            if (mode == 1 || mode == 3)
            {
                duration = r_s16(game + 0xf2);
                uint32 count = r_s16(game + 0xf0);
                numerator = opening ? (uint32)duration - count : (uint32)duration + count;
                numerator *= mode == 1 ? 512 : 240;
            }
            else
            {
                uint32 count = r_s16(game + 0xf0);
                duration = r_s16(game + 0xf2);
                numerator = (opening ? count : 0 - count) * (mode == 2 ? 512 : 240);
            }
            uint32 boundary = G3TransitionQuotient(numerator, duration);
            uint32 narrowed = (uint32)(sint32)(sint16)boundary;
            uint32 order = r_u32(frame + 0x6c);
            uint32 pool;
            if ((mode == 1 && opening) || (mode == 2 && !opening))
            {
                left = boundary; right = 512; bottom = 240;
                w_u32(frame + 0x10, 0);
                w_u32(frame + 0x18, 240);
                w_u32(frame + 0x20, 240);
                w_u32(frame + 0x1c, 512);
                w_u32(frame + 0x14, narrowed);
                pool = r_u32(game + 0x68);
                G3SubmitTransitionQuad(frame, game + 0xec, narrowed, 0, 512, 0, narrowed, 240, 512, 240, pool, order);
            }
            else if ((mode == 2 && opening) || (mode == 1 && !opening))
            {
                right = boundary; bottom = 240;
                w_u32(frame + 0x10, 0);
                w_u32(frame + 0x14, 0);
                w_u32(frame + 0x18, 240);
                w_u32(frame + 0x20, 240);
                w_u32(frame + 0x1c, narrowed);
                pool = r_u32(game + 0x68);
                G3SubmitTransitionQuad(frame, game + 0xec, 0, 0, narrowed, 0, 0, 240, narrowed, 240, pool, order);
            }
            else if ((mode == 3 && opening) || (mode == 4 && !opening))
            {
                top = boundary; right = 512; bottom = 240;
                w_u32(frame + 0x14, 0);
                w_u32(frame + 0x18, 240);
                if (opening)
                {
                    w_u32(frame + 0x1c, 512);
                    w_u32(frame + 0x20, 240);
                }
                else
                {
                    w_u32(frame + 0x20, 240);
                    w_u32(frame + 0x1c, 512);
                }
                w_u32(frame + 0x10, narrowed);
                pool = r_u32(game + 0x68);
                G3SubmitTransitionQuad(frame, game + 0xec, 0, narrowed, 512, narrowed, 0, 240, 512, 240, pool, order);
            }
            else
            {
                right = 512; bottom = boundary;
                w_u32(frame + 0x10, 0);
                w_u32(frame + 0x14, 0);
                w_u32(frame + 0x1c, 512);
                w_u32(frame + 0x18, narrowed);
                w_u32(frame + 0x20, narrowed);
                pool = r_u32(game + 0x68);
                G3SubmitTransitionQuad(frame, game + 0xec, 0, 0, 512, 0, 0, narrowed, 512, narrowed, pool, order);
            }
            break;
        }
        case 5:
        case 6:
        case 7:
        case 8:
        {
            if (!opening)
                break;
            sint32 count = r_s16(game + 0xf0);
            sint32 duration = r_s16(game + 0xf2);
            if (count == duration)
            {
                uint32 index = r_u32(game + 0x58);
                uint32 other = 0x800a3cd4 + (1 - index) * 92;
                uint32 x = r_s16(other);
                uint32 y = r_s16(other + 2);
                MoveImage((PSX_RECT *)psx_addr(0x800a3cd4 + index * 92, 8), (sint32)x, (sint32)y);
                if (mode == 5)
                    duration = r_s16(game + 0xf2);
            }
            uint32 numerator;
            if (mode == 5)
            {
                count = r_s16(game + 0xf0);
                numerator = ((uint32)duration - (uint32)count) << 9;
            }
            else if (mode == 7)
            {
                duration = r_s16(game + 0xf2);
                count = r_s16(game + 0xf0);
                numerator = ((uint32)duration - (uint32)count) * 240;
            }
            else
            {
                count = r_s16(game + 0xf0);
                duration = r_s16(game + 0xf2);
                numerator = (uint32)count * (mode == 6 ? 512 : 240);
            }
            uint32 boundary = G3TransitionQuotient(numerator, duration);
            uint32 narrowed = (uint32)(sint32)(sint16)boundary;
            uint32 order = r_u32(frame + 0x6c);
            uint32 pool = r_u32(game + 0x68);
            w_u32(frame + 0x14, order);
            w_u32(frame + 0x10, pool);
            right = 512; bottom = 240;
            if (mode == 5)
            {
                right = boundary;
                G3Function8001E7F4(0, 0, narrowed, 240, pool, order, frame);
            }
            else if (mode == 6)
            {
                left = boundary;
                G3Function8001E7F4(narrowed, 0, 512, 240, pool, order, frame);
            }
            else if (mode == 7)
            {
                bottom = boundary;
                G3Function8001E7F4(0, 0, 512, narrowed, pool, order, frame);
            }
            else
            {
                top = boundary;
                G3Function8001E7F4(0, narrowed, 512, 240, pool, order, frame);
            }
            break;
        }
        case 9:
        {
            uint32 count = r_s16(game + 0xf0);
            sint32 duration = r_s16(game + 0xf2);
            uint32 magnitude = (sint32)count < 0 ? 0 - count : count;
            if ((opening ? count : magnitude) == (uint32)duration)
            {
                w_u16(game + 0xf0, opening ? 30 : 0xffffffe2);
                w_u16(game + 0xf2, 30);
                if (opening)
                    count = r_s16(game + 0xf0);
            }
            right = 512;
            uint32 table_index = opening ? count : (uint32)r_s16(game + 0xf0) + 30;
            uint32 value = r_u16(0x8009d71c + table_index * 2);
            uint32 order;
            if (opening)
                order = r_u32(frame + 0x6c);
            w_u16(frame + 0x3a, value);
            uint32 signed_value = (uint32)(sint32)(sint16)value;
            uint32 upper = (uint32)((sint32)((4096 - signed_value) * 15) >> 9);
            uint32 lower = (uint32)((sint32)((signed_value + 4096) * 15) >> 9);
            w_u32(frame + 0x10, 0);
            w_u32(frame + 0x14, 0);
            w_u32(frame + 0x18, upper);
            w_u32(frame + 0x1c, 512);
            w_u32(frame + 0x20, upper);
            uint32 pool = r_u32(game + 0x68);
            if (!opening)
                order = r_u32(frame + 0x6c);
            G3SubmitTransitionQuad(frame, game + 0xec, 0, 0, 512, 0, 0, upper, 512, upper, pool, order);
            if (opening && (sint32)count >= 15)
            {
                signed_value = r_s16(frame + 0x3a);
                order = r_u32(frame + 0x6c);
                w_u32(frame + 0x14, 0);
                w_u32(frame + 0x18, 240);
                w_u32(frame + 0x1c, 512);
                w_u32(frame + 0x20, 240);
                lower = (uint32)((sint32)((signed_value + 4096) * 15) >> 9);
                w_u32(frame + 0x10, lower);
                pool = r_u32(game + 0x68);
                G3SubmitTransitionQuad(frame, game + 0xec, 0, lower, 512, lower, 0, 240, 512, 240, pool, order);
                signed_value = r_s16(frame + 0x3a);
                upper = (uint32)((sint32)((4096 - signed_value) * 15) >> 9);
                lower = (uint32)((sint32)((signed_value + 4096) * 15) >> 9);
            }
            else
            {
                w_u32(frame + 0x10, lower);
                w_u32(frame + 0x14, 0);
                w_u32(frame + 0x18, 240);
                w_u32(frame + 0x1c, 512);
                w_u32(frame + 0x20, 240);
                pool = r_u32(game + 0x68);
                order = r_u32(frame + 0x6c);
                G3SubmitTransitionQuad(frame, game + 0xec, 0, lower, 512, lower, 0, 240, 512, 240, pool, order);
            }
            top = upper; bottom = lower;
            if (opening && (sint32)count < 15)
                break;
            count = r_s16(game + 0xf0);
            if (opening || (sint32)count >= -15)
            {
                value = r_u16(0x8009d71c + (count + (opening ? 0xfffffff1 : 15)) * 2);
                if (opening)
                    order = r_u32(frame + 0x6c);
                w_u16(frame + 0x38, value);
                uint32 edge = (uint32)((sint32)(4096 - (uint32)(sint32)(sint16)value) >> 4);
                w_u32(frame + 0x10, upper);
                w_u32(frame + 0x14, 0);
                w_u32(frame + 0x18, lower);
                w_u32(frame + 0x1c, edge);
                w_u32(frame + 0x20, lower);
                pool = r_u32(game + 0x68);
                if (!opening)
                    order = r_u32(frame + 0x6c);
                G3SubmitTransitionQuad(frame, game + 0xec, 0, upper, edge, upper, 0, lower, edge, lower, pool, order);
                value = r_s16(frame + 0x38);
                order = r_u32(frame + 0x6c);
                w_u32(frame + 0x10, upper);
                w_u32(frame + 0x18, lower);
                w_u32(frame + 0x1c, 512);
                w_u32(frame + 0x20, lower);
                left = (uint32)((sint32)(value + 4096) >> 4);
                w_u32(frame + 0x14, left);
                pool = r_u32(game + 0x68);
                G3SubmitTransitionQuad(frame, game + 0xec, left, upper, 512, upper, left, lower, 512, lower, pool, order);
                if (!opening)
                    count = r_s16(game + 0xf0);
            }
            if (!opening && count == 1)
            {
                value = r_s16(frame + 0x38);
                w_u32(frame + 0x10, upper);
                w_u32(frame + 0x18, lower);
                w_u32(frame + 0x20, lower);
                left = (uint32)((sint32)(4096 - value) >> 4);
                right = (uint32)((sint32)(value + 4096) >> 4);
                w_u32(frame + 0x14, left);
                w_u32(frame + 0x1c, right);
                pool = r_u32(game + 0x68);
                order = r_u32(frame + 0x6c);
                G3SubmitTransitionQuad(frame, game + 0xec, left, upper, right, upper, left, lower, right, lower, pool, order);
            }
            break;
        }
        case 10:
        {
            uint32 count;
            sint32 duration;
            if (opening)
            {
                count = r_s16(game + 0xf0);
                duration = r_s16(game + 0xf2);
            }
            else
            {
                duration = r_s16(game + 0xf2);
                count = (uint32)duration + (uint32)r_s16(game + 0xf0);
            }
            uint32 quotient = G3TransitionQuotient(count * 255, duration);
            uint32 order = r_u32(frame + 0x6c);
            w_u32(frame + 0x10, 0);
            w_u32(frame + 0x18, 512);
            w_u32(frame + 0x14, 240);
            w_u32(frame + 0x1c, 240);
            w_u32(frame + 0x2c, 2);
            uint32 rgb = (uint32)(sint32)(sint16)quotient;
            w_u32(frame + 0x20, rgb);
            w_u32(frame + 0x24, rgb);
            w_u32(frame + 0x28, rgb);
            uint32 pool = r_u32(game + 0x68);
            w_u32(frame + 0x34, order);
            w_u32(frame + 0x30, pool);
            G3Function8001E25C(0, 0, 512, 0, 0, 240, 512, 240, rgb, rgb, rgb, 2, pool, order);
            break;
        }
        case 11:
        {
            uint32 count;
            sint32 duration;
            if (opening)
            {
                count = r_s16(game + 0xf0);
                duration = r_s16(game + 0xf2);
            }
            else
            {
                duration = r_s16(game + 0xf2);
                count = (uint32)duration + (uint32)r_s16(game + 0xf0);
            }
            uint32 thickness = G3TransitionQuotient(count * 45, duration);
            uint32 narrowed = (uint32)(sint32)(sint16)thickness;
            right = 512; bottom = 240;
            uint32 order = r_u32(frame + 0x6c);
            w_u32(frame + 0x10, 0);
            w_u32(frame + 0x14, 0);
            w_u32(frame + 0x1c, 512);
            w_u32(frame + 0x18, narrowed);
            w_u32(frame + 0x20, narrowed);
            uint32 pool = r_u32(game + 0x68);
            G3SubmitTransitionQuad(frame, game + 0xec, 0, 0, 512, 0, 0, narrowed, 512, narrowed, pool, order);
            top = 240 - thickness;
            narrowed = (uint32)(sint32)(sint16)top;
            w_u32(frame + 0x10, narrowed);
            w_u32(frame + 0x14, 0);
            w_u32(frame + 0x18, 240);
            w_u32(frame + 0x1c, 512);
            w_u32(frame + 0x20, 240);
            pool = r_u32(game + 0x68);
            order = r_u32(frame + 0x6c);
            G3SubmitTransitionQuad(frame, game + 0xec, 0, narrowed, 512, narrowed, 0, 240, 512, 240, pool, order);
            break;
        }
        default:
            break;
        }
        uint32 count = r_u16(game + 0xf0);
        w_u16(game + 0xf0, count + (opening ? 0xffffffff : 1));
    }
    else if (initial_count == -1)
    {
        sint32 mode = r_s16(game + 0xf4);
        if (mode == 11)
        {
            w_u32(frame + 0x10, 0);
            w_u32(frame + 0x14, 0);
            w_u32(frame + 0x18, 45);
            w_u32(frame + 0x1c, 512);
            w_u32(frame + 0x20, 45);
            uint32 pool = r_u32(game + 0x68);
            uint32 order = r_u32(frame + 0x6c);
            G3SubmitTransitionQuad(frame, game + 0xec, 0, 0, 512, 0, 0, 45, 512, 45, pool, order);
        }
        uint32 y = mode == 11 ? 195 : 0;
        w_u32(frame + 0x10, y);
        w_u32(frame + 0x14, 0);
        w_u32(frame + 0x18, 240);
        w_u32(frame + 0x1c, 512);
        w_u32(frame + 0x20, 240);
        uint32 pool = r_u32(game + 0x68);
        uint32 order = r_u32(frame + 0x6c);
        G3SubmitTransitionQuad(frame, game + 0xec, 0, y, 512, y, 0, 240, 512, 240, pool, order);
    }
    else
    {
        w_u16(frame + 0x3c, 4096);
        w_u16(frame + 0x3a, 4096);
        w_u16(frame + 0x38, 4096);
    }
    (void)r_u32(game + 0x4c);
    G3Function8004D938(frame + 0x38);
    w_u16(0x800a0c42, top);
    w_u16(0x800a0c46, top);
    w_u16(0x800a0c4a, bottom);
    w_u16(0x800a0c4e, bottom);
    w_u16(0x800a0c44, right);
    w_u16(0x800a0c4c, right);
    w_u16(0x800a0c40, left);
    w_u16(0x800a0c48, left);
}

void G3Function80026A9C(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80026A9C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 resource = r_u32(0x80098764);
    if (resource == 0)
        return;
    uint32 name = r_u32(resource + 0x20);
    uint32 target = G3Function80039B78(name);
    uint32 types = r_u32(game + 0x68b0);
    w_u32(frame + 0x18, 0);
    uint32 level = r_u32(game + 0x48);
    uint32 count = r_u32(level + 0xe4);
    uint32 camera = r_u32(game + 0x4c);
    if (count == 0)
        return;
    for (;;)
    {
        if (r_u8(types) == 2)
        {
            uint32 index = r_u32(frame + 0x18);
            uint32 positions = r_u32(game + 0x68ac);
            uint32 point = positions + index * 6;
            uint32 first = r_u16(camera);
            uint32 second = r_u16(point);
            uint32 difference = first - second;
            sint32 component = (sint32)(difference << 16) >> 16;
            uint32 distance = (uint32)component * (uint32)component;
            w_u16(frame + 0x10, difference);
            first = r_u16(camera + 2);
            second = r_u16(point + 2);
            difference = first - second;
            component = (sint32)(difference << 16) >> 16;
            uint32 square_y = (uint32)component * (uint32)component;
            w_u16(frame + 0x12, difference);
            first = r_u16(camera + 4);
            second = r_u16(point + 4);
            difference = first - second;
            component = (sint32)(difference << 16) >> 16;
            uint32 square_z = (uint32)component * (uint32)component;
            w_u16(frame + 0x14, difference);
            uint32 radius = r_s16(resource + 0x16);
            distance += square_y;
            distance += square_z;
            if (radius * radius >= distance)
            {
                uint32 current_game = r_u32(0x80097048);
                G3DispatchStateEvent(target, 0, current_game, 2017, frame + 0x18, frame);
            }
        }
        level = r_u32(game + 0x48);
        uint32 index = r_u32(frame + 0x18);
        count = r_u32(level + 0xe4);
        ++index;
        w_u32(frame + 0x18, index);
        ++types;
        if (index >= count)
            break;
    }
}

uint32 G3Function8007620C(uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007620C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 player = r_u32(0x8009efc4);
    uint32 state = r_u32(player + 0x150);
    uint32 data = r_u32(player + 0x24);
    if (state != 0)
        return 0;
    if ((r_u32(data + 0xb8) & 2) != 0)
        return 0;
    if ((r_u16(data + 0xce) & 4) != 0)
        return 0;
    uint32 flags = r_u32(player + 0x154);
    if (flags == 1)
        return 1;
    if (flags != 0x80000000)
        return 0;
    uint32 linked = r_u32(data + 0x1fc);
    if (linked == 0)
        return 0;
    uint32 target = r_u32(linked + 0xec);
    if (target == 0)
        return 0;
    uint32 game = r_u32(0x80097048);
    return G3DispatchStateEvent(target, linked, game, 2033, 0, frame) != 0 ? 1 : 0;
}

uint32 G3Function800762C0(uint32 object)
{
    FUNCTION_MARKER(0x800762C0, "SLUS_008.06");
    uint32 player = r_u32(0x8009efc4);
    uint32 data = r_u32(player + 0x24);
    w_u32(object + 0x150, 6);
    uint32 linked = r_u32(data + 0x1fc);
    if (linked == 0)
        w_u16(object + 0x6e, 0);
    return linked;
}

uint32 G3Function80052088(uint32 camera, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80052088, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 saved = r_u16(0x80096a8c);
    w_u32(0x80096a8c, 8);
    uint32 result = G3Function80013C54(camera, frame);
    w_u32(0x80096a8c, (uint32)(sint32)(sint16)saved);
    return result;
}

uint32 G3Function80055014(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80055014, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 owner = r_u32(0x80096b34);
    uint32 target_angle = r_u16(owner + 0x64);
    uint32 game = r_u32(0x80097048);
    target_angle = (target_angle + 2048) & 0xfff;
    uint32 camera = r_u32(game + 0x4c);
    uint32 camera_angle = r_u16(camera + 0x1c) & 0xfff;
    uint32 difference = G3Function80038B48(camera_angle, target_angle);
    if (target_angle == camera_angle)
        return 0;
    uint32 point = r_u32(0x8009ebd8);
    owner = r_u32(0x80096b34);
    game = r_u32(0x80097048);
    w_u32(frame + 0x14, 1);
    w_u32(frame + 0x10, owner + 0x60);
    camera = r_u32(game + 0x4c);
    if (G3Function80054034(point + 0x12, point + 0x18, camera, 0x80096b38, owner + 0x60, 1, frame) == 0)
        return 0;
    sint32 angle_distance = (sint32)(difference << 16) >> 16;
    if (angle_distance < 0)
        angle_distance = -angle_distance;
    if (angle_distance >= 256)
        return 1;
    game = r_u32(0x80097048);
    point = r_u32(0x8009ebd8);
    camera = r_u32(game + 0x4c);
    uint32 first = r_u16(point + 0x12);
    uint32 second = r_u16(camera);
    w_u16(frame + 0x18, first - second);
    camera = r_u32(game + 0x4c);
    first = r_u16(point + 0x14);
    second = r_u16(camera + 2);
    w_u16(frame + 0x1a, first - second);
    camera = r_u32(game + 0x4c);
    first = r_u16(point + 0x16);
    second = r_u16(camera + 4);
    w_u16(frame + 0x1c, first - second);
    return (sint32)G3Function8002FA50(frame + 0x18, frame) < 257 ? 1 : 0;
}

uint32 G3Function80051FE4(uint32 camera, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80051FE4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 mode = r_u32(0x80096a88);
    uint32 result = 6;
    if (mode == 6)
    {
        G3Function80052088(camera, frame);
        if (G3Function80055014(frame) != 0)
            G3Function80013C94(camera, 19, frame);
        uint32 game = r_u32(0x80097048);
        result = r_u32(game + 0x6834) & 0xff7fffff;
        w_u32(game + 0x6834, result);
    }
    w_u32(0x80096a98, 0);
    w_u32(0x80096b48, 0);
    return result;
}

uint32 G3Function80014970(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80014970, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 mode = r_u32(0x80096a88);
    uint32 camera = r_u32(game + 0x4c);
    uint32 controls = game + 0x70;
    if (mode == 8)
        return 8;
    if ((r_u32(0x80096a94) & 4) == 0 && (r_u32(object + 0x10) & 0x100) == 0)
    {
        uint32 target = r_u32(0x80099654 + mode * 8);
        bool changed = false;
        bool permitted = false;
        if (target != 0)
        {
            uint32 current_game = r_u32(0x80097048);
            permitted = G3DispatchStateEvent(target, camera, current_game, 9, mode, frame) != 0;
        }
        if (permitted)
        {
            if ((r_u32(controls) & 0x100) != 0)
            {
                changed = true;
                w_u32(0x80096b50, 1);
            }
            if ((r_u32(controls) & 0x200) != 0)
            {
                changed = true;
                w_u32(0x80096b4c, 1);
            }
        }
        if (changed)
        {
            uint32 timeout = r_u32(0x80096b58);
            sint32 minimum = r_s32(0x8009ec74);
            w_u32(0x80096b54, 1);
            if ((sint32)timeout < minimum)
                timeout = (uint32)minimum;
            w_u32(0x80096b58, timeout);
        }
        uint32 timeout = r_u32(0x80096b58);
        if (timeout != 0)
            w_u32(0x80096b58, timeout - 1);
    }
    if (r_s16(game + 0x67ee) == 1)
    {
        uint32 input = r_u32(controls);
        if ((input & 0x4001) == 0x4001)
        {
            uint32 height = r_u16(object + 0x5c);
            w_u32(object + 0x10c, 0);
            w_u16(object + 0x5c, height + 100);
        }
        else if ((input & 0x4002) == 0x4002)
        {
            uint32 height = r_u16(object + 0x5c);
            w_u32(object + 0x10c, 0);
            w_u16(object + 0x5c, height - 100);
        }
    }
    bool cycle_held = false;
    if ((r_u32(object + 0x10) & 0x100) == 0 && (r_u32(0x80096b28) & 0x800) == 0)
    {
        if ((r_u32(controls) & 0x400) != 0)
        {
            uint32 count = r_u32(0x80096b0c);
            w_u32(0x80096b0c, count + 1);
            cycle_held = true;
        }
        else
        {
            uint32 count = r_u32(0x80096b0c);
            if (count != 0 && (sint32)count < 10)
            {
                mode = r_u32(0x80096a88);
                uint32 target = r_u32(0x80099654 + mode * 8);
                bool permitted = false;
                if (target != 0)
                {
                    uint32 current_game = r_u32(0x80097048);
                    permitted = G3DispatchStateEvent(target, camera, current_game, 8, mode, frame) != 0;
                }
                if (permitted)
                {
                    count = r_u32(0x80096a9c) + 1;
                    w_u32(0x80096a9c, count);
                    if ((sint32)count >= 3)
                        w_u32(0x80096a9c, 0);
                    G3Function80014900();
                    mode = r_u32(0x80096a88);
                    target = r_u32(0x80099654 + mode * 8);
                    if (target != 0)
                    {
                        uint32 current_game = r_u32(0x80097048);
                        uint32 selection = r_u32(0x80096a9c);
                        G3DispatchStateEvent(target, camera, current_game, 12, selection, frame);
                    }
                }
            }
        }
    }
    if (!cycle_held)
        w_u32(0x80096b0c, 0);
    bool recover = true;
    uint32 timer;
    if ((r_u32(controls) & 0x10) != 0 && (r_u32(object + 0x10) & 0x100) == 0)
    {
        timer = r_u32(0x80096b48);
        if (timer == 0)
        {
            if (r_u32(0x80096b44) == 0)
                recover = false;
            else
                timer = r_u32(0x80096b48);
        }
    }
    else
        timer = r_u32(0x80096b48);
    if (recover)
    {
        if ((sint32)timer > 0)
            w_u32(0x80096b48, timer - 1);
        if (r_u32(0x80096b48) == 0 || r_s32(0x80096a98) < 10)
        {
            w_u16(0x80096b00, 1);
            G3Function80051FE4(camera, frame);
        }
    }
    else if (r_s16(0x80096b00) != 0 && r_u32(0x80096a88) != 5)
    {
        uint32 count = r_u32(0x80096a98) + 1;
        w_u32(0x80096a98, count);
        if (count == 10 && G3Function8007620C(frame) != 0 && (r_u32(0x80096a94) & 0x200) == 0)
        {
            G3Function80013C94(camera, 6, frame);
            if (r_u32(0x80096a88) == 6)
            {
                G3Function800762C0(object);
                uint32 current_game = r_u32(0x80097048);
                uint32 flags = r_u32(current_game + 0x6834);
                w_u32(current_game + 0x6834, flags | 0x800000);
            }
        }
    }
    uint32 first = r_u16(object + 0x58);
    uint32 second = r_u16(object + 0x70);
    w_u16(frame + 0x10, first - second);
    first = r_u16(object + 0x5a);
    second = r_u16(object + 0x72);
    w_u16(frame + 0x14, 0);
    w_u16(frame + 0x12, first - second);
    uint32 result = r_u32(object + 0x10) & 0x100;
    if (result == 0)
    {
        result = r_u32(0x80096b28) & 0x100;
        if (result == 0)
        {
            if ((r_u32(controls) & 0x10) != 0)
            {
                result = G3Function80078834(object, frame);
                if (result != 0)
                {
                    result = r_s16(frame + 0x10);
                    if (result == 0)
                    {
                        result = r_s16(frame + 0x12);
                        if (result == 0)
                        {
                            result = r_s16(frame + 0x14);
                            if (result == 0)
                            {
                                result = r_u32(controls) & 15;
                                if (result == 0)
                                {
                                    result = r_s16(0x80096b00);
                                    if (result != 0)
                                    {
                                        result = r_u32(0x80096b10) + 1;
                                        w_u32(0x80096b10, result);
                                    }
                                    return result;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                uint32 count = r_u32(0x80096b10);
                result = (sint32)count < 10 ? 1 : 0;
                if (count != 0 && result != 0)
                {
                    result = r_u32(0x80096a94) & 1;
                    if (result == 0)
                    {
                        result = G3Function80055014(frame);
                        if (result != 0)
                        {
                            G3Function80013C94(camera, 19, frame);
                            result = G3Function80055544(frame);
                        }
                    }
                }
            }
        }
    }
    w_u32(0x80096b10, 0);
    return result;
}

void G3Function800775F4(uint32 object, uint32 data)
{
    FUNCTION_MARKER(0x800775F4, "SLUS_008.06");
    uint32 linked = r_u32(data + 0x174);
    if (linked == 0)
        return;
    uint32 segment = r_s16(data + 0x178);
    uint32 matrices = r_u32(object + 0x30);
    uint32 matrix = matrices + (segment << 5);
    uint32 value = r_u16(matrix + 0x14);
    w_u16(linked + 0x58, value);
    linked = r_u32(data + 0x174);
    value = r_u16(matrix + 0x18);
    w_u16(linked + 0x5a, value);
    linked = r_u32(data + 0x174);
    value = r_u16(matrix + 0x1c);
    w_u16(linked + 0x5c, value);
}

uint32 G3Function8002A1C8(uint32 object, uint32 rate, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002A1C8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 models = r_u32(descriptor + 0x10);
    uint32 step = 1;
    if (models == 0)
    {
        uint32 flags = r_u32(object + 0x14);
        w_u32(object + 0x14, flags | 0x10);
        return r_u32(object + 0x14) & 0x10;
    }
    uint32 magnitude = (sint32)rate < 0 ? 0 - rate : rate;
    if ((sint32)magnitude >= 1001)
    {
        rate += (sint32)rate > 0 ? 0xfffffc18 : 1000;
        step = 0xffffffff;
    }
    descriptor = r_u32(object + 0x1c);
    uint32 selector = r_u16(object + 0x6e);
    models = r_u32(descriptor + 0x10);
    uint32 model = r_u32(models + (selector << 2));
    sint32 frame_count = r_s16(model + 2);
    if ((sint32)rate > 0)
    {
        uint32 first = r_s16(object + 0x58);
        uint32 second = r_s16(object + 0x70);
        w_u32(frame + 0x20, first - second);
        first = r_s16(object + 0x5a);
        second = r_s16(object + 0x72);
        w_u32(frame + 0x24, first - second);
        first = r_s16(object + 0x5c);
        second = r_s16(object + 0x74);
        w_u32(frame + 0x28, first - second);
        uint32 x = r_u32(frame + 0x20);
        uint32 y = r_u32(frame + 0x24);
        uint32 z = r_u32(frame + 0x28);
        w_u32(frame + 0x10, x);
        w_u32(frame + 0x14, y);
        w_u32(frame + 0x18, z);
        uint32 distance = G3Function8002FBF8(frame + 0x10, frame);
        uint32 accumulated = r_u16(object + 0xa0) + distance;
        w_u16(object + 0xa0, accumulated);
        sint32 signed_accumulated = (sint32)(accumulated << 16) >> 16;
        while ((sint32)rate < signed_accumulated)
        {
            uint32 next = r_u16(object + 0x5e) + step;
            w_u16(object + 0x5e, next);
            if (((sint32)(next << 16) >> 16) >= frame_count)
            {
                uint32 flags = r_u32(object + 0x14);
                w_u16(object + 0x5e, 0);
                w_u32(object + 0x14, flags | 0x10);
            }
            if (r_s16(object + 0x5e) < 0)
            {
                uint32 flags = r_u32(object + 0x14);
                w_u16(object + 0x5e, (uint32)frame_count - 1);
                w_u32(object + 0x14, flags | 0x10);
            }
            accumulated = r_u16(object + 0xa0);
            descriptor = r_u32(object + 0x1c);
            w_u16(object + 0xa0, accumulated - rate);
            if ((r_u32(descriptor + 0x30) & 0x2000) != 0)
                G3Function8003E314(object, frame);
            signed_accumulated = r_s16(object + 0xa0);
        }
    }
    else
    {
        uint32 next = r_u16(object + 0xa0) - 1;
        w_u16(object + 0xa0, next);
        if ((sint32)(next << 16) > 0)
            w_u16(object + 0xa0, 0);
        if ((sint32)rate >= r_s16(object + 0xa0))
        {
            next = r_u16(object + 0x5e) + step;
            w_u16(object + 0x5e, next);
            if (((sint32)(next << 16) >> 16) >= frame_count)
            {
                uint32 flags = r_u32(object + 0x14);
                w_u16(object + 0x5e, 0);
                w_u32(object + 0x14, flags | 0x10);
            }
            if (r_s16(object + 0x5e) < 0)
            {
                uint32 flags = r_u32(object + 0x14);
                w_u16(object + 0x5e, (uint32)frame_count - 1);
                w_u32(object + 0x14, flags | 0x10);
            }
            descriptor = r_u32(object + 0x1c);
            w_u16(object + 0xa0, 0);
            if ((r_u32(descriptor + 0x30) & 0x2000) != 0)
                G3Function8003E314(object, frame);
        }
    }
    return r_u32(object + 0x14) & 0x10;
}

uint32 G3Function8006BFB8(uint32 object, uint32 data)
{
    FUNCTION_MARKER(0x8006BFB8, "SLUS_008.06");
    uint32 animation = r_u16(object + 0x6e);
    uint32 table = r_u32(data + 0x40);
    uint32 command = r_u32(table + (animation << 2));
    sint32 key = r_s16(object + 0x5e);
    if (command == 0)
    {
        uint32 first = r_u32(0x800a00d8);
        w_u32(data + 0x58, first);
        uint32 second = r_u32(0x800a00dc);
        w_u32(data + 0x54, 0);
        w_u32(data + 0x60, 0);
        w_u32(data + 0x68, first);
        w_u32(data + 0x5c, second);
        uint32 result = r_u32(0x800a00dc);
        w_u32(data + 0x64, 0);
        w_u32(data + 0x70, 0);
        w_u32(data + 0x6c, result);
        return result;
    }
    uint32 accumulated = 0;
    if (key > 0)
    {
        for (;;)
        {
            uint32 duration = r_u32(command);
            accumulated += duration;
            if (duration == 0)
                break;
            bool more = (sint32)accumulated < key;
            command += 0x24;
            if (!more)
                break;
        }
    }
    if (key != 0)
        command -= 0x24;
    uint32 first = r_u32(0x800a00cc);
    uint32 second = r_u32(command + 8);
    bool new_animation = first == second;
    if (new_animation)
    {
        first = r_u32(0x800a00d0);
        second = r_u32(command + 0xc);
        new_animation = first == second;
    }
    if (new_animation)
    {
        uint32 result = r_u32(command + 4) << 4;
        uint32 slot = data + result;
        uint32 linked = r_u32(slot + 0x50);
        uint32 requested = r_u32(command + 0x14);
        if (linked == 0)
            return result;
        result = r_u16(linked + 0x6e);
        if (result == requested)
            return result;
        w_u16(linked + 0x6e, requested);
        linked = r_u32(slot + 0x50);
        w_u16(linked + 0x5e, 0);
        linked = r_u32(slot + 0x50);
        result = r_u16(object + 0x6e);
        w_u32(linked + 0x104, result);
        return result;
    }
    uint32 value = r_u32(command + 8);
    w_u32(data + 0x58, value);
    value = r_u32(command + 0xc);
    w_u32(data + 0x5c, value);
    value = r_u32(command + 4);
    w_u32(data + 0x54, value);
    value = r_u32(command + 0x18);
    w_u32(data + 0x68, value);
    value = r_u32(command + 0x1c);
    w_u32(data + 0x6c, value);
    value = r_u32(command + 0x14);
    w_u32(data + 0x64, value);
    return value;
}

void G3Function80026C00(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80026C00, "SLUS_008.06");
    uint32 player = r_u32(0x8009efc4);
    uint32 frame = guest_stack - 0x20;
    w_u32(game + 0x6588, 1);
    uint32 state = r_u32(player + 0x150);
    if (state != 7)
    {
        G3Function800779B0(game, frame);
        w_u32(game + 0x6588, 1);
    }
}

uint32 G3Function80080134(uint32 object, uint32 data)
{
    FUNCTION_MARKER(0x80080134, "SLUS_008.06");
    sint32 timer = r_s16(data + 0x120);
    uint32 raw_timer = r_u16(data + 0x120);
    uint32 next = raw_timer - 1;
    if (timer == 0)
        return 0;
    w_u16(data + 0x120, next);
    uint32 state = r_u32(object + 0x154);
    uint32 result = next & 2;
    if (state == 0x04000000)
        return result;
    uint32 flags = r_u32(object + 0x10);
    result = result != 0 ? flags | 0x800 : flags & 0xfffff7ff;
    w_u32(object + 0x10, result);
    return result;
}

void G3Function8006F34C(uint32 animation, uint32 key, uint32 duration)
{
    FUNCTION_MARKER(0x8006F34C, "SLUS_008.06");
    w_u16(animation + 0xc, key);
    w_u16(animation + 0xe, duration);
}

uint32 G3Function800779B0(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800779B0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 object = r_u32(game + 0x50);
    uint32 object_flags = r_u32(object + 0x10);
    uint32 data = r_u32(object + 0x24);
    uint32 controls = (object_flags & 0x100) != 0 ? 0x800a0d28 : game + 0x70;
    uint32 current_game = r_u32(0x80097048);
    uint32 flags = r_u32(current_game + 0xe8);
    if ((flags & 0x8000) != 0)
    {
        uint32 level = r_u32(game + 0x48);
        w_u32(level + 0x6c, 0xffff0000);
        flags = r_u32(current_game + 0xe8);
        w_u32(current_game + 0xe8, flags & 0xffff7fff);
    }
    if (r_s32(game + 0x6818) <= 0)
        return 6;
    if (r_s16(game + 0x6786) == 6)
        return 1;
    if (r_s16(game + 0x67ee) == 1)
    {
        G3Function80014970(object, game, frame);
        w_u16(data + 0xe2, 0);
        w_u16(data + 0xe0, 0);
        uint32 state = r_u32(object + 0x154);
        if (state == 0x20000000)
            return G3Function8006D8C4(object, game, data, controls, frame);
        return 0x20000000;
    }
    uint32 flags14 = r_u32(object + 0x14);
    uint32 flags128 = r_u32(object + 0x128);
    w_u32(object + 0x14, flags14 & 0xffffffef);
    w_u32(object + 0x128, flags128 & 0xfbffffff);
    uint32 value = r_u16(data + 0x1c8);
    uint32 animation = r_u16(object + 0x6e);
    w_u16(object + 0x70, value);
    value = r_u16(data + 0x1ca);
    w_u16(object + 0x72, value);
    value = r_u16(data + 0x1cc);
    w_u16(object + 0x74, value);
    bool advance = true;
    uint32 rate;
    if (animation == 12 && (r_u32(data + 0xb8) & 1) == 0)
        rate = 30;
    else if (r_u32(object + 0x150) == 1 && r_u32(object + 0x154) == 0x01000000)
        rate = 1001;
    else if (r_u32(object + 0x150) == 0 && (r_u32(object + 0x154) & 6) != 0)
        rate = r_s16(data + 0xa6);
    else
    {
        uint32 state = r_u32(object + 0x150);
        rate = 0xffffffff;
        if (state - 2 >= 2)
        {
            if (r_u32(object + 0x154) == 0x80000000 && state != 4)
                advance = false;
            else
                rate = r_s16(data + 0xa8);
        }
    }
    if (advance)
        G3Function8002A1C8(object, rate, frame);
    flags = r_u32(data + 0xb8);
    uint32 cleared = flags & 0xffffffeb;
    w_u32(data + 0xb8, cleared);
    w_u32(data + 0xb8, (flags & 1) != 0 ? cleared | 2 : cleared & 0xfffffffd);
    flags = r_u32(data + 0xb8);
    w_u32(data + 0xb8, flags & 0xfffffffe);
    flags = r_u32(object + 0x128);
    sint32 height = r_s16(object + 0x5c);
    sint32 previous_height = r_s16(object + 0x74);
    flags &= 0xffffffdf;
    w_u32(object + 0x128, flags);
    w_u32(object + 0x128, previous_height < height ? flags | 0x80000000 : flags & 0x7fffffff);
    uint32 x = r_u16(object + 0x58);
    uint32 y = r_u16(object + 0x5a);
    uint32 z = r_u16(object + 0x5c);
    w_u16(object + 0x70, x);
    w_u16(object + 0x72, y);
    w_u16(object + 0x74, z);
    w_u16(data + 0x1c8, x);
    value = r_u16(object + 0x5a);
    w_u16(data + 0x1ca, value);
    value = r_u16(object + 0x5c);
    w_u16(data + 0x1cc, value);
    value = r_u16(object + 0x60);
    w_u16(data + 0x1c0, value);
    value = r_u16(object + 0x62);
    w_u16(data + 0x1c2, value);
    value = r_u16(object + 0x64);
    w_u16(data + 0x1c4, value);
    G3Function800775F4(object, data);
    G3Function8006BFB8(object, data);
    G3Function80080134(object, data);
    uint32 destination = r_u32(data + 0x100);
    w_u32(destination, 0);
    destination = r_u32(data + 0x10c);
    w_u32(destination, 0);
    destination = r_u32(data + 0x118);
    w_u32(destination, 0);
    destination = r_u32(data + 0xfc);
    w_u32(destination, 0);
    destination = r_u32(data + 0x108);
    w_u32(destination, 0);
    destination = r_u32(data + 0x114);
    w_u32(destination, 0);
    destination = r_u32(data + 0x104);
    uint32 channel = r_u32(data + 0xf8);
    w_u32(destination, channel);
    destination = r_u32(data + 0x110);
    channel = r_u32(data + 0xf8);
    w_u32(destination, channel);
    destination = r_u32(data + 0x11c);
    channel = r_u32(data + 0xf8);
    w_u32(destination, channel);
    channel = r_u32(data + 0xf8);
    uint32 key = r_s16(data + 0x18a);
    uint32 keylist = r_u32(channel + 0xc);
    G3Function8006F34C(keylist, key, 102);
    value = r_u32(data);
    w_u32(object + 0xa4, value);
    w_u16(data + 0x26e, 0);
    w_u16(data + 0x270, 0);
    w_u16(data + 0x272, 0);
    uint32 state = r_u32(object + 0x150);
    if (state < 9)
    {
        uint32 target = r_u32(0x8009a154 + (state << 2));
        if (target != 0)
        {
            if (state != 0)
            {
                flags = r_u32(object + 0x134);
                w_u32(object + 0x134, flags & 0xfffffff7);
            }
            state = r_u32(object + 0x150);
            target = r_u32(0x8009a154 + (state << 2));
            G3DispatchPlayerUpdate(target, object, game, data, controls, frame);
        }
    }
    uint32 result = r_u32(game + 0x6818);
    if ((sint32)result <= 0)
        return result;
    result = r_u32(object + 0x128) & 0x04000000;
    if (result != 0)
        return result;
    G3Function80014970(object, game, frame);
    if (((r_u16(data + 0xce) & 1) != 0 && r_u32(object + 0x154) != 0x10000) || r_u32(object + 0x150) == 1)
    {
        flags = r_u32(object + 0x10);
        w_u32(object + 0x10, flags & 0xfffffeff);
    }
    current_game = r_u32(0x80097048);
    bool state_captured = false;
    if ((r_u32(current_game + 0xe8) & 0x08000010) == 0)
    {
        sint32 timer = r_s16(data + 0x122);
        uint32 raw_timer = r_u16(data + 0x122);
        if (timer > 0 && r_u32(object + 0x150) != 7)
        {
            uint32 next = raw_timer - 1;
            w_u16(data + 0x122, next);
            if ((next << 16) == 0)
                G3Function800737E4(object, game, frame);
        }
        if (r_s16(data + 0x1d6) > 0)
        {
            state = r_u32(object + 0x150);
            if (state == 7)
                state_captured = true;
            else
            {
                uint32 counter = r_u16(data + 0x1f0);
                uint32 pattern = r_u16(data + 0x1f2);
                uint32 next = counter + 1;
                w_u16(data + 0x1f0, next);
                if (((sint32)(next << 16) >> 16) == (sint32)(pattern >> 8))
                {
                    w_u32(frame + 0x10, 2500);
                    G3Function8003E518(object, 0x819, 0, 250, frame);
                    pattern = r_u16(data + 0x1f2);
                    w_u16(data + 0x1f0, 0);
                    if ((pattern >> 8) >= 4)
                        w_u16(data + 0x1f2, pattern - 0x55);
                }
                next = r_u16(data + 0x1d6) - 1;
                w_u16(data + 0x1d6, next);
                if ((next << 16) == 0)
                {
                    w_u32(frame + 0x10, 2500);
                    G3Function8003E518(object, 0x806, 0xffffff80, 100, frame);
                    G3Function800737E4(object, game, frame);
                }
            }
        }
    }
    if (!state_captured)
        state = r_u32(object + 0x150);
    if (state != 1)
    {
        value = r_u16(data + 0xce);
        w_u16(data + 0xce, value & 0xff80);
    }
    state = r_u32(object + 0x150);
    if (state - 4 >= 2)
    {
        uint32 level = r_u32(game + 0x48);
        height = r_s16(object + 0x5c);
        sint32 floor = r_s32(level + 0x6c);
        if (height < floor)
            G3Function80074AB0(object, game, frame);
    }
    G3Function8007E548(object, game, data, frame);
    flags = r_u32(object + 0x128);
    w_u32(object + 0x128, flags & 0xbffffdff);
    flags = r_u32(data + 0x204);
    w_u32(data + 0x204, flags & 0xfffffff7);
    if (r_s16(game + 0x681c) == 1)
        G3Function8007446C(game);
    return G3Function8006C498(object, game, data, frame);
}

void G3Function800737E4(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800737E4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 data = r_u32(object + 0x24);
    uint32 linked = r_u32(data + 0x174);
    if (linked == 0)
        return;
    uint32 descriptor = r_u32(linked + 0x1c);
    uint32 name = r_u32(descriptor + 0x20);
    uint32 expected = r_u32(0x800a02ac);
    uint32 actual = r_u32(name);
    if (actual != expected)
        return;
    actual = r_u32(name + 4);
    expected = r_u32(0x800a02b0);
    if (actual == expected)
        G3Function80081E08(linked, game, frame);
}

uint32 G3Function8007E548(uint32 object, uint32 game, uint32 data, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007E548, "SLUS_008.06");
    uint32 phase = r_u32(0x800aa7d4);
    if (phase == 3 && r_u8(0x800aa7d3) == 0)
    {
        uint32 count = r_u8(0x800aa7f0);
        if (count != 0)
        {
            uint32 current = r_u32(game + 0x154);
            uint32 start = r_u32(0x800aa7e4);
            uint32 index = ((current - start) >> 1) + 1;
            if ((sint32)index < 0)
                index = 0;
            if ((sint32)index >= (sint32)count)
                index = count - 1;
            uint32 value = r_u8(0x800aa7f1 + index);
            uint32 matrix = r_u32(data);
            w_u16(matrix + 0x10, value << 2);
        }
    }
    else
    {
        uint32 matrix = r_u32(data);
        w_u16(matrix + 0x10, 0);
        matrix = r_u32(data);
        w_u16(matrix + 0x18, 0);
    }
    sint32 current = r_s16(data + 0x25e);
    uint32 raw_current = r_u16(data + 0x25e);
    if (current == 0)
    {
        sint32 target = r_s16(data + 0x260);
        if (target == 0)
            return 0;
    }
    uint32 animation = r_u16(object + 0x6e);
    if (animation == 34)
        return 34;
    sint32 target = r_s16(data + 0x260);
    if (current != target)
    {
        uint32 next = current < target ? raw_current + 1 : raw_current - 1;
        w_u16(data + 0x25e, next);
        uint32 value = r_s16(data + 0x25e);
        uint32 matrix = r_u32(data);
        w_u16(matrix + 0x10, value * 125);
    }
    else if (current != 0)
        w_u16(data + 0x260, 0);
    uint32 value = r_s16(data + 0x25e);
    uint32 matrix = r_u32(data);
    w_u16(matrix + 0x10, value * 125);
    value = r_s16(data + 0x25e);
    matrix = r_u32(data);
    uint32 result = 0 - value * 125;
    w_u16(matrix + 0x18, result);
    w_u16(data + 0x1b6, 0);
    w_u16(data + 0x22a, 0);
    return result;
}

uint32 G3Function8006CBB4(uint32 data)
{
    FUNCTION_MARKER(0x8006CBB4, "SLUS_008.06");
    uint32 linked = r_u32(data + 0x1bc);
    if (linked == 0)
        return 1;
    uint32 descriptor = r_u32(linked + 0x1c);
    uint32 name = r_u32(descriptor + 0x24);
    uint32 first = r_u32(name);
    uint32 expected = r_u32(0x800a018c);
    if (first == expected)
        return 1;
    expected = r_u32(0x800a0194);
    return first == expected ? 1 : 0;
}

uint32 G3Function8006D8C4(uint32 object, uint32 game, uint32 data, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006D8C4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    if (G3Function8006CBB4(data) != 0)
        return G3Function8006D6A0(object, game, data, controls, frame);
    return G3Function8006D820(object, game, data, controls, frame);
}

uint32 G3Function80046B00(uint32 index)
{
    FUNCTION_MARKER(0x80046B00, "SLUS_008.06");
    w_u16(0x800a2190 + (index << 4), 4);
    return 4;
}

uint32 G3Function80043D98(uint32 object, uint32 selection, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80043D98, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 descriptor = r_u32(object + 0x1c);
    uint32 models = r_u32(descriptor + 0xc);
    uint32 data = r_u32(object + 0x24);
    uint32 model = r_u32(models);
    uint32 result = r_u32(data + 0x1a8);
    uint32 owner = r_u32(0x8009874c);
    bool matches = model == result;
    if (!matches)
    {
        result = r_u32(data + 0x1a4);
        matches = model == result;
    }
    if (matches)
    {
        result = r_u32(model + 0x1c);
        if (result != 0)
        {
            uint32 kind = r_u32(result);
            result = (sint32)selection < 2 ? 1 : 0;
            if (kind != 3 && result == 0)
                selection = 0;
        }
    }
    if (owner != 0)
    {
        models = r_u32(owner + 0xc);
        model = r_u32(models);
        uint32 sequence = r_u32(model + 0x1c);
        G3Function80034FA4(sequence, selection, 0, 0);
        models = r_u32(owner + 0xc);
        model = r_u32(models + 4);
        sequence = r_u32(model + 0x1c);
        result = G3Function80034FA4(sequence, selection, 0, 0);
    }
    w_u16(object + 0x9e, selection);
    return result;
}

uint32 G3Function80073630(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80073630, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 state = r_u32(object + 0x150);
    uint32 data = r_u32(object + 0x24);
    if (state - 4 < 2)
        return 1;
    uint32 flags = r_u32(game + 0xe8);
    if ((flags & 0x40000) == 0)
    {
        uint32 level = r_u32(game + 0x48);
        sint32 height = r_s16(object + 0x5c);
        sint32 floor = r_s32(level + 0x6c);
        if (height >= floor)
            return 3;
    }
    if (state == 3 && r_s16(data + 0x252) <= 0)
        w_u16(data + 0x252, 0xffffffff);
    else
    {
        w_u16(data + 0x252, 187);
        if (r_u32(object + 0x150) == 1)
            G3Function800726FC(object, game, data, frame);
    }
    w_u32(object + 0x150, 4);
    sint32 choice = (sint32)G3SdkRandom80027FA8() % 7;
    switch (choice)
    {
    case 0: w_u16(object + 0x6e, 13); break;
    case 1: w_u16(object + 0x6e, 45); break;
    case 2: w_u16(object + 0x6e, 73); break;
    case 3: w_u16(object + 0x6e, 74); break;
    case 4: w_u16(object + 0x6e, 75); break;
    case 5: w_u16(object + 0x6e, 76); break;
    case 6: w_u16(object + 0x6e, 77); break;
    default: break;
    }
    if (r_u32(data + 0x1fc) != 0)
        w_u16(object + 0x6e, 76);
    w_u16(object + 0x5e, 0);
    w_u32(object + 0x10c, 0);
    w_u32(object + 0x118, 0);
    uint32 matrix = r_u32(data);
    w_u16(data + 0xe2, 0);
    w_u16(data + 0xe0, 0);
    w_u16(matrix + 2, 0);
    flags = r_u32(object + 0x128);
    w_u32(object + 0x128, flags & 0xffffffef);
    flags = r_u32(game + 0x6834);
    w_u32(game + 0x6834, flags | 4);
    G3Function80046B00(5);
    return G3Function80046B00(6);
}

uint32 G3Function80074AB0(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80074AB0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    G3Function80073630(object, game, frame);
    uint32 flags = r_u32(game + 0x6834);
    uint32 enabled = r_u8(game + 0x67df);
    w_u32(game + 0x6834, flags | 0x20000);
    if (enabled != 0)
    {
        uint32 current_game = r_u32(0x80097048);
        if (r_u16(current_game + 0x67dc) != 0)
            G3Function8003EF1C();
    }
    uint32 current_game = r_u32(0x80097048);
    w_u16(current_game + 0x681c, 0);
    w_u16(game + 0xf0, 0xffffffe2);
    w_u16(game + 0xf2, 30);
    w_u16(game + 0xf4, 0);
    w_u16(object + 0x6e, 19);
    uint32 camera = r_u32(game + 0x4c);
    uint32 result = G3Function8004DABC(camera, frame);
    if (result != 2)
    {
        camera = r_u32(game + 0x4c);
        result = G3Function80013CDC(camera, 8, frame);
    }
    return result;
}

uint32 G3Function8006C498(uint32 object, uint32 game, uint32 data, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006C498, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 first = r_u32(game + 0x68c0);
    uint32 expected = r_u32(0x800a00e8);
    bool special = false;
    if (first == expected)
    {
        uint32 second = r_u32(game + 0x68c4);
        expected = r_u32(0x800a00ec);
        special = second == expected;
    }
    if (!special)
    {
        expected = r_u32(0x800a00f4);
        if (first == expected)
        {
            uint32 second = r_u32(game + 0x68c4);
            expected = r_u32(0x800a00f8);
            special = second == expected;
        }
    }
    if (special)
    {
        uint32 result = r_u16(object + 0x9e) + 1;
        w_u16(object + 0x9e, result);
        return result;
    }
    uint32 animation = r_u16(object + 0x6e);
    uint32 table = r_u32(data + 0x3c);
    uint32 events = r_u32(table + (animation << 2));
    if (events != 0)
    {
        uint32 key = r_s16(object + 0x5e);
        uint32 event = r_u8(events + key);
        if (event != 95)
        {
            uint32 selection = event == 67 ? 1 : event == 72 ? 2 : 0;
            return G3Function80043D98(object, selection, frame);
        }
    }
    uint32 phase = r_u32(0x800a00e4);
    if (phase != 0)
    {
        uint32 next = phase - 1;
        w_u32(0x800a00e4, next);
        if (next == 2)
            return G3Function80043D98(object, 1, frame);
        if (next == 1)
            return G3Function80043D98(object, 2, frame);
        if (next == 0)
            return G3Function80043D98(object, 0, frame);
        return 1;
    }
    uint32 random = G3SdkRandom80027FA8();
    if ((random & 31) == 0)
    {
        G3Function80043D98(object, 1, frame);
        w_u32(0x800a00e4, 3);
        return 3;
    }
    return G3Function80043D98(object, 0, frame);
}

uint32 G3Function8006CC04(uint32 game, uint32 phase, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006CC04, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 scaled = (phase << 8) - phase;
    if ((sint32)scaled < 0)
        scaled += 7;
    w_u32(frame + 0x18, 512);
    uint32 rgb = (uint32)((sint32)(scaled << 13) >> 16);
    w_u32(frame + 0x20, rgb);
    w_u32(frame + 0x24, rgb);
    w_u32(frame + 0x28, rgb);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 240);
    w_u32(frame + 0x1c, 240);
    w_u32(frame + 0x2c, 2);
    uint32 pool = r_u32(game + 0x68);
    w_u32(frame + 0x30, pool);
    uint32 order = r_u32(game + 0x38) + 4;
    w_u32(frame + 0x34, order);
    return G3Function8001E25C(0, 0, 512, 0, 0, 240, 512, 240, rgb, rgb, rgb, 2, pool, order);
}

uint32 G3Function8006D670(uint32 game)
{
    FUNCTION_MARKER(0x8006D670, "SLUS_008.06");
    uint32 buttons = r_u32(game + 0x9c);
    if ((buttons & 0x40) != 0)
        return 3;
    if ((buttons & 0x80) != 0)
        return 2;
    return 1;
}

uint32 G3Function8006D6A0(uint32 object, uint32 game, uint32 data, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006D6A0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    sint32 initial_phase = r_s16(data + 0xc4);
    uint32 raw_phase = r_u16(data + 0xc4);
    uint32 status = 1;
    if (initial_phase < 9)
    {
        uint32 next = raw_phase + 1;
        w_u16(data + 0xc4, next);
        uint32 phase = (uint32)(sint32)(sint16)next;
        if ((sint32)phase < 9)
            G3Function8006CC04(game, phase, frame);
    }
    sint32 phase = r_s16(data + 0xc4);
    if (phase == 8)
    {
        G3Function8006CF20(game, frame);
        G3Function80035AA4();
        G3Function800359EC(0x120, 3, frame);
        phase = r_s16(data + 0xc4);
    }
    if (phase >= 9)
    {
        G3Function8006D4D0(object, game, frame);
        status = G3Function8006D670(game);
    }
    w_u16(object + 0x5e, 0);
    uint32 flags = r_u32(game + 0xe8);
    w_u16(game + 0x6786, 8);
    w_u32(game + 0xe8, flags | 0x08000000);
    if (status != 2)
        return 2;
    flags = r_u32(object + 0x128);
    w_u32(object + 0x128, flags & 0xfdffffff);
    uint32 camera = r_u32(game + 0x4c);
    G3Function80013C54(camera, frame);
    uint32 level = r_u16(game + 0x68b4);
    uint32 root;
    uint32 name;
    if (level == 22)
    {
        w_u16(game + 0x67f0, 0);
        w_u16(game + 0x67f2, 0);
        w_u16(game + 0x67f4, 0);
        root = r_u32(0x8009d684);
        name = r_u32(0x8009d688);
    }
    else
    {
        root = r_u32(0x8009d668);
        name = r_u32(0x8009d66c);
    }
    G3Function80027644(root, name, game, frame);
    uint32 result = r_u16(game + 0x69de);
    w_u16(game + 0x69dc, result);
    return result;
}

uint32 G3Function8006D820(uint32 object, uint32 game, uint32 data, uint32 controls, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006D820, "SLUS_008.06");
    uint32 frame = guest_stack - 0x70;
    w_u16(object + 0x5e, 0);
    w_u16(game + 0x6786, 8);
    uint32 flags = r_u32(game + 0xe8);
    w_u32(game + 0xe8, flags | 0x08000000);
    flags = r_u32(object + 0x128);
    w_u32(object + 0x128, flags & 0xfdffffff);
    uint32 camera = r_u32(game + 0x4c);
    G3Function80013C54(camera, frame);
    uint32 linked = r_u32(data + 0x1bc);
    uint32 descriptor = r_u32(linked + 0x28);
    uint32 name = descriptor + 4;
    uint32 suffix = r_s16(descriptor + 2);
    G3SdkSprintfStringInteger80088A1C(frame + 0x10, 0x800a0208, name, suffix);
    G3Function80027644(name, frame + 0x10, game, frame);
    uint32 result = r_u16(game + 0x69de);
    w_u16(game + 0x69dc, result);
    return result;
}

uint32 G3Function80046B38(void)
{
    FUNCTION_MARKER(0x80046B38, "SLUS_008.06");
    for (uint32 index = 0; index < 12; ++index)
        G3Function80046B00(index);
    return 0;
}

uint32 G3Function8006CF20(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006CF20, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 camera = r_u32(game + 0x4c);
    uint32 level = r_u16(game + 0x68b4);
    G3Function80013CDC(camera, 8, frame);
    G3Function80046B38();
    memset(psx_addr(0x800a2a6c, 32), 0, 32);
    uint32 list = r_u32(0x800a0c2c);
    G3Function8006CC88(list);
    uint32 flags = r_u32(game + 0x6834);
    uint32 storage = r_u32(0x800a2a68);
    w_u32(game + 0x6834, flags | 0x200);
    G3Function800414B0(storage);
    w_u32(game + 0x6790, 1);
    w_u32(game + 0x6788, 0);
    w_u8(0x800a3d9a, 64);
    w_u8(0x800a3d98, 0);
    w_u8(0x800a3d99, 0);
    if (level - 11 < 5 || level - 20 < 3)
        G3Function8006CCC0(0x800a2a6c, frame + 0x10, frame);
    else
    {
        G3Function8006CD64(game, 0x800a2a6c, frame + 0x10, frame);
        G3Function8006CE84(0x800a2a6c, frame + 0x10, frame);
    }
    return G3Function8003EF1C();
}

uint32 G3Function8006C634(void)
{
    FUNCTION_MARKER(0x8006C634, "SLUS_008.06");
    uint32 resource = G3Function8003AA48(0x800a0100);
    if (resource == 0)
    {
        resource = G3Function8003AA48(0x800a010c);
        if (resource == 0)
            resource = G3Function8003AA48(0x800a0118);
    }
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u16(game + 0x68b4);
    if (level - 11 < 5)
        resource = 0;
    return resource;
}

uint32 G3Function8006CCC0(uint32 state, uint32 position, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006CCC0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 resource = G3Function8006C634();
    w_u16(position + 2, 208);
    w_u16(position, 0);
    w_u16(position + 4, 1333);
    uint32 result = G3Function8006CA8C(resource, position, 0, frame);
    w_u32(state + 0x18, result);
    return result;
}

uint32 G3Function8006CE84(uint32 state, uint32 position, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006CE84, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 first = G3Function8003AA48(0x800a01d0);
    uint32 second = G3Function8003AA48(0x800a01dc);
    w_u16(position + 4, 4000);
    w_u16(position + 2, 640);
    w_u16(position, 480);
    uint32 result = G3Function8006CA8C(first, position, 0, frame);
    w_u32(state + 0x18, result);
    w_u16(position, (uint32)-480);
    result = G3Function8006CA8C(second, position, 0, frame);
    w_u32(state + 0x1c, result);
    return result;
}

uint32 G3Function8006CA8C(uint32 resource, uint32 position, uint32 model, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006CA8C, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x38;
    uint32 camera = r_u32(game + 0x4c);
    uint32 matrix = r_u32(camera + 0x80);
    SetRotMatrix((MATRIX *)psx_addr(matrix, 20));
    game = r_u32(0x80097048);
    camera = r_u32(game + 0x4c);
    matrix = r_u32(camera + 0x80);
    SetTransMatrix((MATRIX *)psx_addr(matrix, 32));
    RotTrans((SVECTOR *)psx_addr(position, 8), (VECTOR *)psx_addr(frame + 0x10, 16), (sint32 *)psx_addr(frame + 0x20, 4));
    uint32 player = r_u32(0x8009efc4);
    uint32 created = G3Function8002A454(player, resource, frame);
    if (created != 0)
    {
        uint32 handler = G3Function80039B78(0x800a0184);
        w_u32(created + 0xec, handler);
        w_u16(created + 0x68, 4096);
        w_u16(created + 0x6a, 4096);
        w_u16(created + 0x6c, 4096);
        uint32 flags = r_u32(created + 0x10);
        w_u32(created + 0x10, flags | 0x00100000);
        uint32 component = r_u16(frame + 0x10);
        game = r_u32(0x80097048);
        w_u16(created + 0x58, component);
        component = r_u16(frame + 0x14);
        w_u16(created + 0x5a, component);
        component = r_u16(frame + 0x18);
        w_u16(created + 0x5c, component);
        camera = r_u32(game + 0x4c);
        uint32 angle = r_u16(camera + 0x0a);
        w_u16(created + 0x62, 0);
        w_u16(created + 0x60, 0u - angle);
        uint32 random = G3SdkRandom80027FA8();
        w_u16(created + 0x64, random & 0x7ff);
        flags = r_u32(created + 0x14);
        w_u16(created + 0xa2, model);
        w_u32(created + 0x14, flags & 0xffffffbf);
    }
    return created;
}

uint32 G3Function8006CD18(uint32 game, uint32 index)
{
    FUNCTION_MARKER(0x8006CD18, "SLUS_008.06");
    uint32 level = r_u16(game + 0x68b4);
    if ((sint32)index >= 3)
    {
        uint32 flags = r_u32(game + 0x6838);
        return (flags & (1u << (level & 31))) == 0;
    }
    uint32 flags = r_u8(game + level + 0x67f8);
    uint32 shift = index & 31;
    uint32 selected = flags & (1u << shift);
    return 1u - (uint32)((sint32)selected >> shift);
}

uint32 G3Function8006CD64(uint32 game, uint32 state, uint32 position, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8006CD64, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 level = r_u16(game + 0x68b4);
    uint32 category = G3Function80039010(level);
    uint32 resource = G3Function8006C634();
    uint32 offset = (category - 1) << 2;
    w_u16(position + 2, (uint32)-480);
    w_u16(position + 4, 4000);
    for (uint32 index = 0; index < 4; ++index)
    {
        uint32 x = r_u16(0x800a01a0 + offset);
        w_u16(position, x);
        uint32 model = G3Function8006CD18(game, index);
        uint32 created = G3Function8006CA8C(resource, position, model, frame);
        w_u32(state + 8 + (index << 2), created);
        offset += 12;
    }
    return 0;
}

