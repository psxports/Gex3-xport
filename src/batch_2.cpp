#include <stdio.h>
#include <stdlib.h>
#include "xport.h"
#include "psx.h"
#include "psx_spu.h"
#include "game_functions.h"

uint32 G3Function80022FD0(uint32 text, G3TextMeasureOperands *operands)
{
    FUNCTION_MARKER(0x80022fd0, "SLUS_008.06");
    uint32 character = r_u8(text);
    uint32 width = 0;
    while (character != 0)
    {
        character = r_u8(text);
        if (operands != 0)
            operands->second_glyph = 8;
        uint32 advance = G3Function80022EB4(0, character, 8, operands);
        ++text;
        character = r_u8(text);
        width += advance;
    }
    return width;
}

uint32 G3Function80022EB4(uint32 mode, uint32 character, uint32 default_width, G3TextMeasureOperands *operands)
{
    FUNCTION_MARKER(0x80022eb4, "SLUS_008.06");
    if (operands != 0)
        operands->first_glyph = character;
    if (mode != 0)
        return default_width;
    character &= 0xff;
    if (character == 0x20)
        return default_width;
    uint32 glyph = G3Function80020E74(character);
    if (glyph == 0xffffffff)
        return default_width;
    uint32 mapping = 0x80096ce4 + (glyph << 1) + glyph;
    uint32 second = r_u8(mapping + 1);
    sint32 first = r_s8(mapping);
    uint32 third = r_u8(mapping + 2);
    if (operands != 0)
    {
        operands->second_glyph = second;
        operands->third_glyph = third;
    }
    sint32 first_width = 8;
    if (first >= 0)
        first_width = r_s8(0x80096c00 + ((uint32)first << 2) + 2);
    if (operands != 0)
        operands->first_glyph = (uint32)first_width;
    sint32 second_index = (sint8)second;
    sint32 second_width = first_width;
    if (second_index >= 0)
        second_width = r_s8(0x80096c00 + ((uint32)second_index << 2) + 2);
    sint32 third_index = (sint8)third;
    sint32 third_width = first_width;
    if (third_index >= 0)
        third_width = r_s8(0x80096c00 + ((uint32)third_index << 2) + 2);
    sint32 widest = first_width;
    if (second_width > widest)
        widest = second_width;
    if (third_width > widest)
        widest = third_width;
    return (uint32)(widest + 1);
}

uint32 G3Function80020E74(uint32 character)
{
    FUNCTION_MARKER(0x80020e74, "SLUS_008.06");
    character &= 0xff;
    if (((character - 0x61) & 0xff) < 0x1a)
        return character - 0x61;
    if (((character - 0x30) & 0xff) < 0x0a)
        return character - 0x16;
    if (((character - 0x41) & 0xff) < 0x1a)
        return character - 0x0d;
    switch (character)
    {
    case '!': return 0x25;
    case '/': return 0x26;
    case '-': return 0x27;
    case '"': return 0x28;
    case '%': return 0x29;
    case ':': return 0x2a;
    case '(': return 0x2b;
    case ')': return 0x2c;
    case '+': return 0x2d;
    case '=': return 0x2e;
    case '*': return 0x2f;
    case '?': return 0x30;
    case '~': return 0x31;
    case ',': return 0x32;
    case '\'': return 0x33;
    default: return 0x24;
    }
}

uint32 G3Function800213A8(uint32 x, uint32 y, uint32 mode)
{
    FUNCTION_MARKER(0x800213a8, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 texture_x = (uint32)(sint32)r_s16(0x800a3c70);
    uint32 texture_y = (uint32)(sint32)r_s16(0x800a3c72);
    texture_x += 0x0d;
    uint32 buffer = r_u32(game + 0x68);
    uint32 order_table = r_u32(game + 0x38);
    uint32 limit = r_u32(buffer + 8);
    uint32 packet = r_u32(buffer + 4);
    texture_y += 2;
    if (limit - 0x30 < packet)
        return 1;
    uint32 top = (uint32)((sint32)x >> 8);
    uint32 left = x & 0xff;
    uint32 horizontal_scale;
    uint32 vertical_scale;
    if (mode != 0)
    {
        horizontal_scale = 0x2a;
        vertical_scale = 0x1e;
    }
    else
    {
        top += 3;
        horizontal_scale = 8;
        sint32 alternate_height = r_s16(0x800a3c7a);
        vertical_scale = alternate_height == 0 ? 0x0c : 0x12;
    }
    uint32 width = (y & 0xff) * horizontal_scale;
    w_u8(packet + 3, 9);
    w_u8(packet + 7, 0x2f);
    uint32 rows = (uint32)((sint32)y >> 8);
    w_u8(packet + 0x0c, texture_x);
    w_u8(packet + 0x0d, texture_y);
    w_u8(packet + 0x14, texture_x);
    w_u8(packet + 0x15, texture_y);
    w_u8(packet + 0x1c, texture_x);
    w_u8(packet + 0x1d, texture_y);
    w_u8(packet + 0x24, texture_x);
    w_u8(packet + 0x25, texture_y);
    w_u16(packet + 8, left);
    w_u16(packet + 0x0a, top);
    w_u16(packet + 0x12, top);
    w_u16(packet + 0x18, left);
    uint32 right = left + (width << 1);
    w_u16(packet + 0x10, right);
    w_u16(packet + 0x20, right);
    uint32 bottom = top + rows * vertical_scale;
    w_u16(packet + 0x1a, bottom);
    w_u16(packet + 0x22, bottom);
    uint32 texture_page = r_u16(0x800a3c6c);
    w_u16(packet + 0x16, texture_page);
    uint32 palette = r_u16(0x800a3c6e);
    w_u16(packet + 0x0e, palette);
    uint32 tag = r_u32(order_table);
    w_u32(packet, (tag & 0x00ffffff) | 0x09000000);
    game = r_u32(0x80097048);
    w_u32(order_table, packet & 0x00ffffff);
    buffer = r_u32(game + 0x68);
    w_u32(buffer + 4, packet + 0x28);
    return buffer;
}

uint32 G3Function80022E60(uint32 character, uint32 x_pointer, uint32 y_pointer, uint32 reset_x, uint32 line_height)
{
    FUNCTION_MARKER(0x80022e60, "SLUS_008.06");
    if ((character & 0x7f) != 0x0a)
        return 0;
    w_u32(x_pointer, reset_x);
    uint32 y = r_u32(y_pointer);
    w_u32(y_pointer, y + line_height);
    return 1;
}

void G3Function80023030(uint32 text, uint32 mode, uint32 arguments, uint32 incoming_third_glyph, uint32 guest_stack, G3TextMeasureOperands *operands)
{
    FUNCTION_MARKER(0x80023030, "SLUS_008.06");
    if (operands != 0)
    {
        operands->first_glyph = mode;
        operands->second_glyph = arguments;
        operands->third_glyph = incoming_third_glyph;
    }
    uint32 frame = guest_stack - 0x50;
    uint32 color = 0;
    uint32 flags = 0;
    w_u32(frame + 0x54, mode);
    if (text == 0)
        return;
    uint32 x_pointer;
    uint32 y_pointer;
    uint32 default_width;
    uint32 line_height;
    uint32 initial_x = 0;
    if (mode != 0)
    {
        default_width = 0x2a;
        line_height = 0x26;
        x_pointer = 0x800a3c5c;
        y_pointer = 0x800a3c60;
        uint32 saved_mode = r_u32(0x8009d628);
        uint32 reset_x = r_u32(x_pointer);
        flags = saved_mode == 0 ? 1 : 3;
        w_u32(frame + 0x18, reset_x);
    }
    else
    {
        default_width = 8;
        sint32 alternate_height = r_s16(0x800a3c7a);
        line_height = alternate_height == 0 ? 0x0c : 0x12;
        x_pointer = 0x800a3c54;
        y_pointer = 0x800a3c58;
        uint32 reset_x = r_u32(x_pointer);
        initial_x = reset_x;
        w_u32(frame + 0x18, reset_x);
    }
    if (operands != 0)
    {
        operands->first_glyph = line_height;
        operands->second_glyph = default_width;
        if (mode == 0)
            operands->third_glyph = initial_x;
    }
    uint32 character = r_u8(text);
    if (character == 0)
        return;
    w_u32(frame + 0x1c, default_width);
    w_u32(frame + 0x20, default_width << 2);
    w_u32(frame + 0x24, line_height);
    do
    {
        switch (character)
        {
        case '%':
        {
            if (operands != 0)
                operands->first_glyph = 0x8009d650;
            ++text;
            uint32 specifier = r_u8(text);
            if (specifier == 's')
            {
                arguments += 4;
                uint32 string = r_u32(arguments - 4);
                uint32 next = r_u8(string);
                while (next != 0)
                {
                    uint32 current_mode = r_u32(frame + 0x54);
                    uint32 handled = 0;
                    if (current_mode != 0)
                    {
                        next = r_u8(string);
                        uint32 reset_x = r_u32(frame + 0x18);
                        uint32 height = r_u32(frame + 0x24);
                        w_u32(frame + 0x10, height);
                        handled = G3Function80022E60(next, x_pointer, y_pointer, reset_x, height);
                        if (operands != 0)
                            operands->third_glyph = reset_x;
                    }
                    if (handled == 0)
                    {
                        next = r_u8(string);
                        w_u32(frame + 0x10, flags);
                        uint32 x = r_u32(x_pointer);
                        uint32 y = r_u32(y_pointer);
                        G3Function80020C48(next, x, y, color & 0xff, flags);
                        if (operands != 0)
                            operands->third_glyph = color & 0xff;
                    }
                    current_mode = r_u32(frame + 0x54);
                    next = r_u8(string);
                    if (operands != 0)
                        operands->second_glyph = default_width;
                    uint32 advance = G3Function80022EB4(current_mode, next, default_width, operands);
                    uint32 x = r_u32(x_pointer);
                    ++string;
                    w_u32(x_pointer, x + advance);
                    next = r_u8(string);
                }
            }
            else if (specifier == 'd' || specifier == 'x')
            {
                uint32 format = specifier == 'd' ? 0x8009d650 : 0x8009d654;
                if (operands != 0)
                    operands->first_glyph = format;
                arguments += 4;
                uint32 value = r_u32(arguments - 4);
                sprintfPSX(0x800a3c44, format, value);
                uint32 string = 0x800a3c44;
                uint32 next = r_u8(string);
                while (next != 0)
                {
                    next = r_u8(string);
                    w_u32(frame + 0x10, flags);
                    uint32 x = r_u32(x_pointer);
                    uint32 y = r_u32(y_pointer);
                    G3Function80020C48(next, x, y, color & 0xff, flags);
                    if (operands != 0)
                        operands->third_glyph = color & 0xff;
                    uint32 current_mode = r_u32(frame + 0x54);
                    next = r_u8(string);
                    if (operands != 0)
                        operands->second_glyph = default_width;
                    uint32 advance = G3Function80022EB4(current_mode, next, default_width, operands);
                    x = r_u32(x_pointer);
                    ++string;
                    w_u32(x_pointer, x + advance);
                    next = r_u8(string);
                }
            }
            else if (specifier == '%')
            {
                w_u32(frame + 0x10, flags);
                uint32 x = r_u32(x_pointer);
                uint32 y = r_u32(y_pointer);
                G3Function80020C48(0x25, x, y, color & 0xff, flags);
                if (operands != 0)
                    operands->third_glyph = color & 0xff;
                uint32 current_mode = r_u32(frame + 0x54);
                uint32 current = r_u8(text);
                if (operands != 0)
                    operands->second_glyph = default_width;
                uint32 advance = G3Function80022EB4(current_mode, current, default_width, operands);
                x = r_u32(x_pointer);
                w_u32(x_pointer, x + advance);
            }
            ++text;
            break;
        }
        case 0x0a:
        {
            sint32 preserve_indent = r_s16(0x800a3c78);
            if (preserve_indent != 0)
            {
                uint32 reset_x = r_u32(frame + 0x18);
                w_u32(x_pointer, reset_x);
            }
            else
                w_u32(x_pointer, character);
            uint32 y = r_u32(y_pointer);
            uint32 height = r_u32(frame + 0x24);
            w_u32(y_pointer, y + height);
            ++text;
            break;
        }
        case '@':
        {
            uint32 rows = r_u8(text + 1);
            uint32 columns = r_u8(text + 2);
            uint32 current = r_u8(text);
            w_u32(frame + 0x10, flags);
            uint32 x = r_u32(x_pointer);
            uint32 y = r_u32(y_pointer);
            rows -= 0x40;
            columns -= 0x40;
            G3Function80020C48(current, x, y, 0, flags);
            current = r_u8(text);
            text += 2;
            w_u32(frame + 0x10, flags);
            G3Function80020C48(current, columns & 0xff, rows & 0xff, 0, flags);
            if (operands != 0)
            {
                operands->first_glyph = columns & 0xff;
                operands->second_glyph = rows & 0xff;
                operands->third_glyph = 0;
            }
            ++text;
            break;
        }
        case '#':
        {
            uint32 digit = r_u8(text + 1);
            ++text;
            color = digit - 0x30;
            ++text;
            break;
        }
        case '$':
        {
            uint32 current_mode = r_u32(frame + 0x54);
            if (current_mode != 0)
            {
                w_u32(frame + 0x18, 0x28);
                w_u32(x_pointer, 0x28);
                w_u32(y_pointer, 0x1e);
                G3Function80021AAC();
                if (operands != 0)
                {
                    operands->first_glyph = 0x140;
                    operands->second_glyph = 0;
                    operands->third_glyph = 0;
                }
            }
            else
            {
                w_u32(frame + 0x18, 0x0a);
                w_u32(x_pointer, 0x0a);
                w_u32(y_pointer, 0x10);
            }
            ++text;
            break;
        }
        case ' ':
        case '_':
        case 9:
        {
            uint32 current = character;
            if (character != 9)
                current = r_u8(text);
            uint32 x = r_u32(x_pointer);
            uint32 advance = r_u32(frame + (current == 9 ? 0x20 : 0x1c));
            w_u32(x_pointer, x + advance);
            ++text;
            break;
        }
        default:
        {
            uint32 current = r_u8(text);
            w_u32(frame + 0x10, flags);
            uint32 x = r_u32(x_pointer);
            uint32 y = r_u32(y_pointer);
            G3Function80020C48(current, x, y, color & 0xff, flags);
            if (operands != 0)
                operands->third_glyph = color & 0xff;
            uint32 current_mode = r_u32(frame + 0x54);
            current = r_u8(text);
            if (operands != 0)
                operands->second_glyph = default_width;
            uint32 advance = G3Function80022EB4(current_mode, current, default_width, operands);
            x = r_u32(x_pointer);
            w_u32(x_pointer, x + advance);
            ++text;
            break;
        }
        }
        character = r_u8(text);
    } while (character != 0);
}

void G3Function80020CBC(uint32 text, uint32 argument_1, uint32 argument_2, uint32 argument_3, uint32 guest_stack, G3TextMeasureOperands *operands)
{
    FUNCTION_MARKER(0x80020cbc, "SLUS_008.06");
    w_u32(guest_stack, text);
    w_u32(guest_stack + 4, argument_1);
    w_u32(guest_stack + 8, argument_2);
    w_u32(guest_stack + 0x0c, argument_3);
    uint32 frame = guest_stack - 0x18;
    w_u32(frame + 0x18, text);
    G3Function80023030(text, 0, frame + 0x1c, argument_3, frame, operands);
}

uint32 G3Function80021AAC(void)
{
    FUNCTION_MARKER(0x80021aac, "SLUS_008.06");
    return G3Function80021A74(0x140);
}

uint32 G3Function800210F4(uint32 character, uint32 x, uint32 y)
{
    FUNCTION_MARKER(0x800210f4, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 buffer = r_u32(game + 0x68);
    uint32 limit = r_u32(buffer + 8);
    uint32 packet = r_u32(buffer + 4);
    if (limit - 0x30 < packet)
        return 1;
    uint32 mapped = G3Function80020E74(character & 0xff);
    uint32 mapping = 0x80096ce4 + (mapped << 1) + mapped;
    uint32 second_raw = r_u8(mapping + 1);
    uint32 third_raw = r_u8(mapping + 2);
    sint32 first_index = r_s8(mapping);
    uint32 first_raw = r_u8(mapping);
    sint32 first_width = 8;
    if (first_index >= 0)
    {
        first_width = r_s8(0x80096c00 + ((uint32)first_index << 2) + 2);
        if (first_width < 0)
            first_width = -first_width;
    }
    sint32 second_index = (sint8)second_raw;
    sint32 second_width = first_width;
    if (second_index >= 0)
    {
        second_width = r_s8(0x80096c00 + ((uint32)second_index << 2) + 2);
        if (second_width < 0)
            second_width = -second_width;
    }
    sint32 third_index = (sint8)third_raw;
    sint32 third_width = first_width;
    if (third_index >= 0)
    {
        third_width = r_s8(0x80096c00 + ((uint32)third_index << 2) + 2);
        if (third_width < 0)
            third_width = -third_width;
    }
    sint32 widest = first_width;
    if (second_width > widest)
        widest = second_width;
    if (third_width > widest)
        widest = third_width;
    first_index = (sint8)first_raw;
    sint32 first_height = 0x0c;
    if (first_index >= 0)
    {
        first_height = r_s8(0x80096c00 + ((uint32)first_index << 2) + 3);
        if (first_height < 0)
            first_height = -first_height;
    }
    if (first_index >= 0)
        G3Function80020FCC((uint32)first_index, x + ((uint32)(widest - first_width) >> 1), y);
    uint32 second_x;
    if ((character & 0xff) == 0x71)
    {
        uint32 metric_address = 0x80096c00 + ((uint32)second_index << 2) + 2;
        uint32 metric_raw = r_u8(metric_address);
        sint32 metric = (sint8)metric_raw;
        sint32 half = (metric + (metric < 0 ? 1 : 0)) >> 1;
        second_x = x + (uint32)widest - (uint32)half - 2;
    }
    else
        second_x = x + ((uint32)(widest - second_width) >> 1);
    uint32 second_y = y;
    if ((character & 0xff) == 0x41)
        second_y += 2;
    if ((character & 0xff) == 0x27)
        y += 3;
    if (second_index >= 0)
        G3Function80020FCC((uint32)second_index, second_x, second_y);
    if (third_index >= 0)
        return G3Function80020FCC((uint32)third_index, x + ((uint32)(widest - third_width) >> 1), y - (uint32)first_height - 1);
    return third_raw << 24;
}

uint32 G3Function80020FCC(uint32 glyph, uint32 x, uint32 y)
{
    FUNCTION_MARKER(0x80020fcc, "SLUS_008.06");
    uint32 metric = 0x80096c00 + (glyph << 2);
    uint32 glyph_u = r_u8(metric);
    uint32 texture_u = r_u16(0x800a3c70);
    uint32 glyph_v = r_u8(metric + 1);
    uint32 game = r_u32(0x80097048);
    sint32 width = r_s8(metric + 2);
    sint32 height = r_s8(metric + 3);
    uint32 u = (uint32)(sint32)(sint8)glyph_u + texture_u;
    if (width < 0)
        width = -width;
    if (height < 0)
        height = -height;
    uint32 texture_v = r_u16(0x800a3c72);
    uint32 buffer = r_u32(game + 0x68);
    uint32 order_table = r_u32(game + 0x38);
    uint32 v = (uint32)(sint32)(sint8)glyph_v + texture_v;
    uint32 packet = r_u32(buffer + 4);
    w_u8(packet + 3, 9);
    w_u8(packet + 7, 0x2d);
    uint32 right_u = u + (uint32)width;
    w_u8(packet + 0x0d, v);
    w_u8(packet + 0x15, v);
    uint32 bottom_v = v + (uint32)height;
    uint32 top = y - ((uint32)height - 0x0c);
    uint32 right = x + (uint32)width;
    uint32 bottom = y + 0x0c;
    w_u8(packet + 0x0c, u);
    w_u8(packet + 0x14, right_u);
    w_u8(packet + 0x1c, u);
    w_u8(packet + 0x1d, bottom_v);
    w_u8(packet + 0x24, right_u);
    w_u8(packet + 0x25, bottom_v);
    w_u16(packet + 8, x);
    w_u16(packet + 0x0a, top);
    w_u16(packet + 0x10, right);
    w_u16(packet + 0x12, top);
    w_u16(packet + 0x18, x);
    w_u16(packet + 0x1a, bottom);
    w_u16(packet + 0x20, right);
    w_u16(packet + 0x22, bottom);
    uint32 texture_page = r_u16(0x800a3c6c);
    w_u16(packet + 0x16, texture_page);
    uint32 palette = r_u16(0x800a3c6e);
    w_u16(packet + 0x0e, palette);
    uint32 tag = r_u32(order_table);
    w_u32(packet, (tag & 0x00ffffff) | 0x09000000);
    game = r_u32(0x80097048);
    w_u32(order_table, packet & 0x00ffffff);
    buffer = r_u32(game + 0x68);
    w_u32(buffer + 4, packet + 0x28);
    return buffer;
}

uint32 G3Function8002F718(uint32 matrix)
{
    FUNCTION_MARKER(0x8002f718, "SLUS_008.06");
    w_u16(matrix, 0x1000);
    w_u16(matrix + 2, 0);
    w_u16(matrix + 4, 0);
    w_u16(matrix + 6, 0);
    w_u16(matrix + 8, 0x1000);
    w_u16(matrix + 0x0a, 0);
    w_u16(matrix + 0x0c, 0);
    w_u16(matrix + 0x0e, 0);
    w_u16(matrix + 0x10, 0x1000);
    return 0x1000;
}

uint32 G3Function80022408(uint32 character, uint32 x, uint32 y, uint32 color, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80022408, "SLUS_008.06");
    uint32 frame = guest_stack - 0x128;
    uint32 object = 0;
    bool bounce = false;
    uint32 bounce_word = r_masked_u32(0x8009d630, 0x0f);
    bounce_word = r_masked_u32(0x8009d630, 0x0f);
    sint32 bounce_tail = r_s8(0x8009d634);
    w_masked_u32(frame + 0xb8, bounce_word, 0x0f);
    w_masked_u32(frame + 0xb8, bounce_word, 0x0f);
    w_u8(frame + 0xbc, (uint32)bounce_tail);
    uint32 font = r_u32(0x8009d6a4);
    uint32 angles = r_u32(font + 0x1c);
    if (angles == 0)
        return font;
    w_u16(frame + 0x22, 0xf000);
    w_u16(frame + 0x18, 0x1000);
    w_u16(frame + 0x18, 0x1999);
    w_u16(frame + 0x1a, 0);
    w_u16(frame + 0x1c, 0);
    w_u16(frame + 0x1e, 0);
    w_u16(frame + 0x20, 0);
    w_u16(frame + 0x24, 0);
    w_u16(frame + 0x26, 0x1000);
    w_u16(frame + 0x28, 0);
    w_u32(frame + 0x2c, 0);
    w_u32(frame + 0x30, 0);
    w_u32(frame + 0x34, 0);
    w_u16(frame + 0x1a, 0);
    w_u16(frame + 0x1c, 0);
    character &= 0xff;
    uint32 glyph;
    if (((character - 0x30) & 0xff) < 0x0a)
        glyph = character - 0x30;
    else if (((character - 0x61) & 0xff) < 0x1a)
        glyph = character - 0x57;
    else
    {
        switch (character)
        {
        case '!': glyph = 0x24; break;
        case ',': glyph = 0x25; break;
        case '*':
        case '+': glyph = 0x27; break;
        case '?': glyph = 0x28; break;
        case ':': glyph = 0x29; break;
        case '^': glyph = 0x2a; break;
        case '\'': glyph = 0x2b; break;
        case '`': glyph = 0x2c; break;
        case '~': glyph = 0x2d; break;
        case 'Q': glyph = 0x2e; break;
        case 'E': glyph = 0x2f; break;
        case 'B':
            G3SdkPrint80085E84(0x8009d638);
            glyph = 0x26;
            break;
        default: glyph = 0x26; break;
        }
    }
    w_u16(frame + 0x10, 0);
    uint32 angle = r_u16(angles + (glyph << 1));
    w_u16(frame + 0x14, 0);
    w_u16(frame + 0x12, angle);
    if (character == 0x2d)
    {
        glyph = 1;
        w_u16(frame + 0x12, 0x400);
        w_u16(frame + 0x14, 0);
    }
    uint32 game = r_u32(0x80097048);
    uint32 frames = r_u32(game + 0x14c);
    uint32 phase = frames + (uint32)((sint32)y >> 1);
    uint32 phase_index = phase & 0x7f;
    color &= 0xff;
    if (color == 1)
    {
        w_u16(frame + 0x10, 0xff9c);
        uint32 step = (phase_index << 5) & 0x7ff;
        angle = r_u16(frame + 0x12);
        if (step < 0x200)
            angle += step;
        else if (step < 0x600)
            angle -= step - 0x400;
        else if (step < 0x0a00)
            angle = angle - 0x800 + step;
        else if (step < 0x0e00)
            angle -= step - 0x0c00;
        else
            angle += step;
        w_u16(frame + 0x12, angle);
        bounce = true;
    }
    else if (color == 2)
    {
        uint32 oscillation = phase_index << 7;
        uint32 cosine = G3Function8005B470(oscillation);
        uint32 scaled = cosine * 150;
        uint32 old_angle = r_u16(frame + 0x10);
        w_u16(frame + 0x10, old_angle + (uint32)((sint32)scaled >> 12));
        uint32 sine = G3Function8005B590(oscillation);
        scaled = sine * 150;
        old_angle = r_u16(frame + 0x12);
        w_u16(frame + 0x12, old_angle + (uint32)((sint32)scaled >> 12));
    }
    else
        w_u16(frame + 0x10, 0xff9c);
    uint32 half_phase = (uint32)((sint32)phase >> 1);
    if (bounce)
    {
        uint32 index = (half_phase & 4) != 0 ? 8 - (half_phase & 7) : half_phase & 7;
        uint32 offset = r_u8(frame + 0xb8 + index);
        y += offset;
    }
    G3Function8002F718(frame + 0x98);
    uint32 depth = r_u32(0x8009d62c);
    w_u32(frame + 0xac, x - 0x100);
    w_u32(frame + 0xb0, y - 0x78);
    w_u32(frame + 0xb4, depth);
    for (uint32 index = 0; index < 3; ++index)
    {
        uint32 address = frame + 0xac + (index << 2);
        uint32 value = r_u32(address);
        w_u32(address, value * 5);
    }
    w_u32(frame + 0x94, 0);
    w_u32(frame + 0x90, 0);
    w_u32(frame + 0x8c, 0);
    G3Function8005BD00(frame + 0x10, frame + 0x78);
    uint32 rotation_0 = r_u32(frame + 0x78);
    uint32 rotation_1 = r_u32(frame + 0x7c);
    uint32 rotation_2 = r_u32(frame + 0x80);
    uint32 rotation_3 = r_u32(frame + 0x84);
    uint32 rotation_4 = r_u32(frame + 0x88);
    w_u32(frame + 0x58, rotation_0);
    w_u32(frame + 0x5c, rotation_1);
    w_u32(frame + 0x60, rotation_2);
    w_u32(frame + 0x64, rotation_3);
    w_u32(frame + 0x68, rotation_4);
    for (uint32 pass = 0; pass < 2; ++pass)
    {
        uint32 matrix = frame + (pass == 0 ? 0x18 : 0x98);
        uint32 input = frame + (pass == 0 ? 0x78 : 0x38);
        uint32 output = frame + (pass == 0 ? 0x38 : 0x78);
        SetRotMatrix((MATRIX *)psx_addr(matrix, 20));
        for (uint32 column = 0; column < 3; ++column)
        {
            uint32 offset = column << 1;
            uint32 first = r_u16(input + offset);
            uint32 second = r_u16(input + offset + 6);
            uint32 third = r_u16(input + offset + 0x0c);
            xport_gte_write_data(9, first);
            xport_gte_write_data(10, second);
            xport_gte_write_data(11, third);
            xport_gte_execute(0x49e012);
            first = xport_gte_read_data(9);
            second = xport_gte_read_data(10);
            third = xport_gte_read_data(11);
            w_u16(output + offset, first);
            w_u16(output + offset + 6, second);
            w_u16(output + offset + 0x0c, third);
        }
        SetTransMatrix((MATRIX *)psx_addr(matrix, 32));
        uint32 vector = frame + (pass == 0 ? 0x8c : 0x4c);
        uint32 transformed = frame + (pass == 0 ? 0x4c : 0x8c);
        uint32 vector_y = r_u16(vector + 4);
        uint32 vector_x = r_u16(vector);
        xport_gte_write_data(0, vector_x | (vector_y << 16));
        uint32 vector_z = r_u32(vector + 8);
        xport_gte_write_data(1, vector_z);
        xport_gte_execute(0x480012);
        w_u32(transformed, xport_gte_read_data(25));
        w_u32(transformed + 4, xport_gte_read_data(26));
        w_u32(transformed + 8, xport_gte_read_data(27));
    }
    uint32 result = glyph << 16;
    sint32 signed_glyph = (sint16)(glyph & 0xffff);
    if (signed_glyph >= 0)
    {
        font = r_u32(0x8009d6a4);
        sint32 count = r_s16(font + 8);
        result = (uint32)signed_glyph << 2;
        if (signed_glyph < count)
        {
            uint32 table = r_u32(font + 0x0c);
            result += table;
            object = r_u32(result);
        }
    }
    if (object != 0)
    {
        w_u16(frame + 0xe2, 0xf6c4);
        w_u16(frame + 0xec, 0xfc18);
        w_u16(frame + 0xf0, 0xf768);
        w_u16(frame + 0xe0, 0x093c);
        w_u16(frame + 0xe4, 0x093c);
        w_u16(frame + 0xe6, 0);
        w_u16(frame + 0xe8, 0);
        w_u16(frame + 0xea, 0);
        w_u16(frame + 0xee, 0);
        for (uint32 index = 0; index < 9; ++index)
            w_u16(frame + 0xc0 + (index << 1), 0x0fff);
        MulMatrix0((MATRIX *)psx_addr(frame + 0xe0, 20), (MATRIX *)psx_addr(frame + 0x58, 20), (MATRIX *)psx_addr(frame + 0x58, 20));
        SetLightMatrix((MATRIX *)psx_addr(frame + 0x58, 18));
        SetColorMatrix((MATRIX *)psx_addr(frame + 0xc0, 20));
        SetRotMatrix((MATRIX *)psx_addr(frame + 0x78, 20));
        SetTransMatrix((MATRIX *)psx_addr(frame + 0x78, 32));
        game = r_u32(0x80097048);
        uint32 primitives = r_u32(object + 0x18);
        uint32 buffer = r_u32(game + 0x68);
        uint32 order_table = r_u32(game + 0x38);
        result = G3Function80021ACC(primitives, buffer, object, order_table, frame);
        game = r_u32(0x80097048);
        buffer = r_u32(game + 0x68);
        w_u32(buffer + 4, result);
    }
    return result;
}

uint32 G3Function8005B470(uint32 angle)
{
    FUNCTION_MARKER(0x8005b470, "SLUS_008.06");
    angle &= 0x0fff;
    uint32 index;
    uint32 component;
    bool negative;
    switch (angle >> 9)
    {
    case 0: return G3Function8005B494(angle, 0x8009e050);
    case 1: index = 0x400 - angle; component = 0; negative = false; break;
    case 2: index = angle - 0x400; component = 0; negative = true; break;
    case 3: index = 0x800 - angle; component = 2; negative = true; break;
    case 4: index = angle - 0x800; component = 2; negative = true; break;
    case 5: index = 0x0c00 - angle; component = 0; negative = true; break;
    case 6: index = angle - 0x0c00; component = 0; negative = false; break;
    default: index = 0x1000 - angle; component = 2; negative = false; break;
    }
    sint32 value = r_s16(0x8009e050 + (index << 2) + component);
    return (uint32)(negative ? -value : value);
}

uint32 G3Function8005B590(uint32 angle)
{
    FUNCTION_MARKER(0x8005b590, "SLUS_008.06");
    angle &= 0x0fff;
    uint32 index;
    uint32 component;
    bool negative;
    switch (angle >> 9)
    {
    case 0: return G3Function8005B5B4(angle, 0x8009e050);
    case 1: index = 0x400 - angle; component = 2; negative = false; break;
    case 2: index = angle - 0x400; component = 2; negative = false; break;
    case 3: index = 0x800 - angle; component = 0; negative = false; break;
    case 4: index = angle - 0x800; component = 0; negative = true; break;
    case 5: index = 0x0c00 - angle; component = 2; negative = true; break;
    case 6: index = angle - 0x0c00; component = 2; negative = true; break;
    default: index = 0x1000 - angle; component = 0; negative = true; break;
    }
    sint32 value = r_s16(0x8009e050 + (index << 2) + component);
    return (uint32)(negative ? -value : value);
}

uint32 G3Function8005B494(uint32 angle, uint32 table)
{
    FUNCTION_MARKER(0x8005b494, "SLUS_008.06");
    uint32 offset = angle << 2;
    uint32 address = table + offset;
    if ((~(table ^ offset) & (table ^ address) & 0x80000000) != 0)
        xport_mips_overflow_exception(0x8005b498);
    return (uint32)(sint32)r_s16(address + 2);
}

uint32 G3Function8005B5B4(uint32 angle, uint32 table)
{
    FUNCTION_MARKER(0x8005b5b4, "SLUS_008.06");
    uint32 offset = angle << 2;
    uint32 address = table + offset;
    if ((~(table ^ offset) & (table ^ address) & 0x80000000) != 0)
        xport_mips_overflow_exception(0x8005b5b8);
    return (uint32)(sint32)r_s16(address);
}

G3AnglePair G3Function8005B6B0(uint32 angle)
{
    FUNCTION_MARKER(0x8005b6b0, "SLUS_008.06");
    angle &= 0x0fff;
    uint32 index;
    bool swap;
    bool negate_cosine;
    bool negate_sine;
    switch (angle >> 9)
    {
    case 0: return G3Function8005B6D4(angle, 0x8009e050);
    case 1: index = 0x400 - angle; swap = true; negate_cosine = false; negate_sine = false; break;
    case 2: index = angle - 0x400; swap = true; negate_cosine = true; negate_sine = false; break;
    case 3: index = 0x800 - angle; swap = false; negate_cosine = true; negate_sine = false; break;
    case 4: index = angle - 0x800; swap = false; negate_cosine = true; negate_sine = true; break;
    case 5: index = 0x0c00 - angle; swap = true; negate_cosine = true; negate_sine = true; break;
    case 6: index = angle - 0x0c00; swap = true; negate_cosine = false; negate_sine = true; break;
    default: index = 0x1000 - angle; swap = false; negate_cosine = false; negate_sine = true; break;
    }
    uint32 sample = r_u32(0x8009e050 + (index << 2));
    G3AnglePair result;
    result.cosine = swap ? sample & 0xffff : sample >> 16;
    result.sine = swap ? sample >> 16 : sample & 0xffff;
    if (negate_cosine)
        result.cosine = 0 - result.cosine;
    if (negate_sine)
        result.sine = 0 - result.sine;
    return result;
}

G3AnglePair G3Function8005B6D4(uint32 angle, uint32 table)
{
    FUNCTION_MARKER(0x8005b6d4, "SLUS_008.06");
    uint32 offset = angle << 2;
    uint32 address = table + offset;
    if ((~(table ^ offset) & (table ^ address) & 0x80000000) != 0)
        xport_mips_overflow_exception(0x8005b6d8);
    uint32 sample = r_u32(address);
    G3AnglePair result;
    result.cosine = sample >> 16;
    result.sine = sample & 0xffff;
    return result;
}

void G3Function8005BD00(uint32 rotation, uint32 matrix)
{
    FUNCTION_MARKER(0x8005bd00, "SLUS_008.06");
    uint32 packed = r_u32(rotation);
    uint32 angle_z = (uint32)(sint32)r_s16(rotation + 4);
    uint32 angle_y = packed >> 16;
    uint32 angle_x = packed & 0xffff;
    uint32 zero_mask = 0;
    if (angle_y == 0)
        zero_mask = 2;
    if (angle_x == 0)
        zero_mask |= 1;
    if (angle_z == 0)
        zero_mask |= 4;
    if (zero_mask == 0)
    {
        G3AnglePair x_pair = G3Function8005B6B0(angle_x);
        G3AnglePair y_pair = G3Function8005B6B0(angle_y);
        G3AnglePair z_pair = G3Function8005B6B0(angle_z);
        xport_gte_write_data(9, y_pair.cosine);
        xport_gte_write_data(10, z_pair.cosine);
        xport_gte_write_data(11, z_pair.sine);
        xport_gte_write_data(8, x_pair.cosine);
        uint32 first_product = y_pair.cosine * z_pair.cosine;
        xport_gte_execute(0x198003d);
        w_u16(matrix + 4, y_pair.sine);
        uint32 first_y = xport_gte_read_data(10);
        uint32 first_x = xport_gte_read_data(9);
        uint32 first_z = xport_gte_read_data(11);
        w_u16(matrix + 0x10, first_x);
        xport_gte_write_data(9, y_pair.cosine);
        xport_gte_write_data(10, z_pair.cosine);
        xport_gte_write_data(11, z_pair.sine);
        xport_gte_write_data(8, x_pair.sine);
        uint32 second_product = y_pair.cosine * z_pair.sine;
        xport_gte_execute(0x198003d);
        w_u16(matrix, (uint32)((sint32)first_product >> 12));
        uint32 second_x = xport_gte_read_data(9);
        uint32 second_y = xport_gte_read_data(10);
        uint32 second_z = xport_gte_read_data(11);
        w_u16(matrix + 0x0a, 0 - second_x);
        xport_gte_write_data(9, first_y);
        xport_gte_write_data(10, first_z);
        xport_gte_write_data(11, second_y);
        xport_gte_write_data(8, y_pair.sine);
        uint32 third_product = second_z * y_pair.sine;
        xport_gte_execute(0x198003d);
        w_u16(matrix + 2, 0 - (uint32)((sint32)second_product >> 12));
        uint32 third_x = xport_gte_read_data(9);
        uint32 third_y = xport_gte_read_data(10);
        uint32 third_z = xport_gte_read_data(11);
        uint32 product_shift = (uint32)((sint32)third_product >> 12);
        w_u16(matrix + 6, third_z + first_z);
        w_u16(matrix + 0x0c, second_z - third_x);
        w_u16(matrix + 8, first_y - product_shift);
        w_u16(matrix + 0x0e, third_y + second_y);
        return;
    }
    if (zero_mask == 1 || zero_mask == 2)
    {
        G3AnglePair first = G3Function8005B6B0(zero_mask == 1 ? angle_y : angle_x);
        G3AnglePair second = G3Function8005B6B0(angle_z);
        xport_gte_write_control(2, first.sine & 0xffff);
        xport_gte_write_control(0, first.cosine & 0xffff);
        xport_gte_write_control(4, first.cosine & 0xffff);
        xport_gte_write_control(1, 0);
        xport_gte_write_control(3, 0);
        xport_gte_write_data(9, second.cosine);
        xport_gte_write_data(10, second.cosine);
        xport_gte_write_data(11, second.sine);
        uint32 product = first.sine * second.sine;
        xport_gte_execute(0x49e012);
        if (zero_mask == 1)
        {
            w_u32(matrix + 4, (first.sine & 0xffff) | (second.sine << 16));
            w_u32(matrix + 8, second.cosine & 0xffff);
            w_u32(matrix + 0x10, first.cosine & 0xffff);
            uint32 product_shift = (uint32)((sint32)product >> 12);
            uint32 ir_x = xport_gte_read_data(9);
            uint32 ir_y = xport_gte_read_data(10);
            uint32 ir_z = xport_gte_read_data(11);
            uint32 first_word = (ir_x & 0xffff) | ((0 - ir_z) << 16);
            uint32 last_word = (product_shift << 16) | ((0 - ir_y) & 0xffff);
            w_u32(matrix, first_word);
            w_u32(matrix + 0x0c, last_word);
        }
        else
        {
            w_u32(matrix, ((0 - second.sine) << 16) | (second.cosine & 0xffff));
            uint32 product_shift = (uint32)((sint32)product >> 12);
            uint32 ir_z = xport_gte_read_data(11);
            w_u32(matrix + 4, ir_z << 16);
            uint32 ir_x = xport_gte_read_data(9);
            w_u32(matrix + 8, ((0 - first.sine) << 16) | (ir_x & 0xffff));
            uint32 ir_y = xport_gte_read_data(10);
            w_u32(matrix + 0x0c, (product_shift & 0xffff) | (ir_y << 16));
            w_u32(matrix + 0x10, first.cosine & 0xffff);
        }
        return;
    }
    if (zero_mask == 4)
    {
        G3AnglePair y_pair = G3Function8005B6B0(angle_y);
        uint32 saved_cosine = y_pair.cosine & 0xffff;
        G3AnglePair x_pair = G3Function8005B6B0(angle_x);
        xport_gte_write_control(0, x_pair.cosine & 0xffff);
        xport_gte_write_control(4, x_pair.cosine & 0xffff);
        uint32 masked_sine = x_pair.sine & 0xffff;
        xport_gte_write_control(2, masked_sine);
        xport_gte_write_control(1, 0);
        xport_gte_write_control(3, 0);
        xport_gte_write_data(9, y_pair.cosine);
        xport_gte_write_data(10, y_pair.cosine);
        xport_gte_write_data(11, y_pair.sine);
        uint32 product = y_pair.sine * x_pair.sine;
        xport_gte_execute(0x49e012);
        w_u32(matrix, saved_cosine);
        uint32 product_shift = (uint32)((sint32)product >> 12);
        w_u32(matrix + 4, (product_shift << 16) | (y_pair.sine & 0xffff));
        uint32 ir_y = xport_gte_read_data(10);
        w_u32(matrix + 8, (x_pair.cosine & 0xffff) | ((0 - ir_y) << 16));
        uint32 ir_z = xport_gte_read_data(11);
        w_u32(matrix + 0x0c, ((0 - ir_z) & 0xffff) | (masked_sine << 16));
        uint32 ir_x = xport_gte_read_data(9);
        w_u32(matrix + 0x10, ir_x & 0xffff);
        return;
    }
    if (zero_mask == 3)
    {
        w_u32(matrix + 0x0c, 0);
        G3AnglePair pair = G3Function8005B6B0(angle_z);
        w_u32(matrix + 4, pair.sine << 16);
        w_u32(matrix + 0x10, 0x1000);
        uint32 masked_cosine = pair.cosine & 0xffff;
        w_u32(matrix + 8, masked_cosine);
        w_u32(matrix, masked_cosine | ((0 - pair.sine) << 16));
        return;
    }
    if (zero_mask == 6)
    {
        w_u32(matrix + 4, 0);
        G3AnglePair pair = G3Function8005B6B0(angle_x);
        w_u32(matrix + 0x0c, pair.sine << 16);
        w_u32(matrix, 0x1000);
        uint32 masked_cosine = pair.cosine & 0xffff;
        w_u32(matrix + 0x10, masked_cosine);
        w_u32(matrix + 8, masked_cosine | ((0 - pair.sine) << 16));
        return;
    }
    if (zero_mask == 5)
    {
        G3AnglePair pair = G3Function8005B6B0(angle_y);
        w_u32(matrix + 8, 0x1000);
        uint32 masked_sine = pair.sine & 0xffff;
        uint32 masked_cosine = pair.cosine & 0xffff;
        w_u32(matrix + 4, masked_sine);
        w_u32(matrix, masked_cosine);
        w_u32(matrix + 0x10, masked_cosine);
        w_u32(matrix + 0x0c, (0 - pair.sine) & 0xffff);
        return;
    }
    w_u32(matrix, 0x1000);
    w_u32(matrix + 4, 0);
    w_u32(matrix + 8, 0x1000);
    w_u32(matrix + 0x0c, 0);
    w_u32(matrix + 0x10, 0x1000);
}

uint32 G3Function80021ACC(uint32 primitives, uint32 buffer, uint32 object, uint32 order_table, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80021acc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x70;
    uint32 packet = r_u32(buffer + 4);
    uint32 first_index = (uint32)(sint32)r_s16(primitives + 4);
    w_u32(frame + 0x4c, 0x20007af8);
    uint32 list = r_u32(object + 0x14);
    uint32 vertices = r_u32(object + 8);
    uint32 cursor = list + (((first_index << 1) + first_index) << 2);
    uint32 last_index = (uint32)(sint32)r_s16(primitives + 6);
    uint32 game = r_u32(0x80097048);
    uint32 last = list + (((last_index << 1) + last_index) << 2);
    uint32 game_flags = r_u32(game + 0x6834);
    bool textured = false;
    uint32 environment_index = 0;
    if ((game_flags & 0x8000) == 0 && r_u32(0x8009d628) != 0)
    {
        uint32 level = r_u32(game + 0x48);
        sint32 count = r_s32(level + 0xf8);
        if (count >= 2)
        {
            environment_index = 2;
            textured = true;
        }
        else if (count > 0)
        {
            environment_index = 1;
            textured = true;
        }
    }
    uint32 descriptor = 0;
    uint32 scale = 0;
    uint32 weight = 0;
    uint32 inverse_weight = 0;
    uint32 half_width = 0;
    uint32 half_height = 0;
    uint32 center_x = 0;
    uint32 center_y = 0;
    if (textured)
    {
        game = r_u32(0x80097048);
        uint32 level = r_u32(game + 0x48);
        uint32 environments = r_u32(level + 0xf4);
        descriptor = environments + (((environment_index << 2) + environment_index) << 2);
        scale = (uint32)(sint32)r_s16(descriptor + 0x12);
        weight = (uint32)(sint32)r_s16(descriptor + 0x10);
        uint32 right = r_u8(descriptor + 4);
        uint32 left = r_u8(descriptor);
        uint32 top = r_u8(descriptor + 1);
        inverse_weight = 0x1000 - weight;
        uint32 width = right - left + 1;
        half_width = (uint32)((sint32)(width + (width >> 31)) >> 1);
        uint32 bottom = r_u8(descriptor + 5);
        center_x = left + half_width;
        uint32 height = bottom - top + 1;
        half_height = (uint32)((sint32)(height + (height >> 31)) >> 1);
        center_y = top + half_height;
    }
    if (last < cursor)
        return packet;
    uint32 indices = cursor + 4;
    uint32 packet_fields = packet + 7;
    do
    {
        uint32 limit = r_u32(buffer + 8);
        if (limit - 0xc0 < packet)
            return packet;
        uint32 first = r_u16(cursor);
        uint32 second = r_u16(indices - 2);
        uint32 third = r_u16(indices);
        uint32 positions[3] = { vertices + (first << 3), vertices + (second << 3), vertices + (third << 3) };
        for (uint32 vertex = 0; vertex < 3; ++vertex)
        {
            uint32 packed_xy = r_u32(positions[vertex]);
            xport_gte_write_data(vertex << 1, packed_xy);
            uint32 packed_z = r_u32(positions[vertex] + 4);
            xport_gte_write_data((vertex << 1) + 1, packed_z);
        }
        xport_gte_execute(0x280030);
        xport_gte_execute(0x1400006);
        w_u32(frame + 0x48, xport_gte_read_data(24));
        sint32 orientation = r_s32(frame + 0x48);
        if (orientation < 0)
        {
            w_u32(packet + 8, xport_gte_read_data(12));
            w_u32(packet + 0x10, xport_gte_read_data(13));
            w_u32(packet + 0x18, xport_gte_read_data(14));
            if (textured)
            {
                w_u32(packet_fields - 3, 0x24808080);
                for (uint32 vertex = 0; vertex < 3; ++vertex)
                {
                    for (uint32 component = 0; component < 3; ++component)
                    {
                        uint32 index_address = vertex == 0 ? cursor : indices + ((vertex - 1) << 1) - 2;
                        uint32 index = r_u16(index_address);
                        uint32 normal_index = r_u16(vertices + (index << 3) + 6);
                        uint32 normal_address = 0x80098838 + (normal_index << 3) + (component << 1);
                        uint32 normal = (uint32)(sint32)r_s16(normal_address);
                        uint32 first_product = weight * normal;
                        uint32 material_index = r_u8(indices + 2);
                        uint32 material_address = 0x80098838 + (material_index << 3) + (component << 1);
                        uint32 material = (uint32)(sint32)r_s16(material_address);
                        uint32 mixed = first_product + inverse_weight * material;
                        if ((sint32)mixed < 0)
                            mixed += 0x0fff;
                        uint32 value = (uint32)((sint32)mixed >> 12);
                        w_u16(frame + 0x30 + (vertex << 3) + (component << 1), value);
                    }
                }
                for (uint32 vertex = 0; vertex < 3; ++vertex)
                {
                    uint32 normal = frame + 0x30 + (vertex << 3);
                    uint32 packed_xy = r_u32(normal);
                    xport_gte_write_data(0, packed_xy);
                    uint32 packed_z = r_u32(normal + 4);
                    xport_gte_write_data(1, packed_z);
                    xport_gte_execute(0x486012);
                    uint32 transformed = frame + (vertex << 4);
                    w_u32(transformed, xport_gte_read_data(25));
                    w_u32(transformed + 4, xport_gte_read_data(26));
                    w_u32(transformed + 8, xport_gte_read_data(27));
                }
                for (uint32 vertex = 0; vertex < 3; ++vertex)
                {
                    for (uint32 component = 0; component < 2; ++component)
                    {
                        uint32 value = r_u32(frame + (vertex << 4) + (component << 2));
                        uint32 product = scale * value;
                        if ((sint32)product < 0)
                            product += 0x0fff;
                        uint32 first_shift = (uint32)((sint32)product >> 12);
                        uint32 half_span = component == 0 ? half_width : half_height;
                        product = first_shift * half_span;
                        if ((sint32)product < 0)
                            product += 0x0fff;
                        uint32 second_shift = (uint32)((sint32)product >> 12);
                        uint32 center = component == 0 ? center_x : center_y;
                        w_u8(packet_fields + 5 + (vertex << 3) + component, center + second_shift);
                    }
                }
                uint32 palette = r_u16(descriptor + 2);
                w_u16(packet_fields + 7, palette);
                uint32 texture_page = r_u16(descriptor + 6);
                w_u8(packet_fields - 4, 7);
                w_u8(packet_fields, 0x24);
                w_u16(packet_fields + 0x0f, texture_page);
                uint32 tag = r_u32(order_table);
                packet_fields += 0x20;
                w_u32(packet, (tag & 0x00ffffff) | 0x07000000);
                w_u32(order_table, packet & 0x00ffffff);
                uint32 count = r_u32(buffer);
                packet += 0x20;
                w_u32(buffer, count + 1);
            }
            else
            {
                first = r_u16(cursor);
                uint32 first_normal = r_u16(vertices + (first << 3) + 6);
                second = r_u16(indices - 2);
                uint32 second_normal = r_u16(vertices + (second << 3) + 6);
                third = r_u16(indices);
                uint32 third_normal = r_u16(vertices + (third << 3) + 6);
                uint32 normals[3] = { 0x80098838 + (first_normal << 3), 0x80098838 + (second_normal << 3), 0x80098838 + (third_normal << 3) };
                for (uint32 vertex = 0; vertex < 3; ++vertex)
                {
                    uint32 packed_xy = r_u32(normals[vertex]);
                    xport_gte_write_data(vertex << 1, packed_xy);
                    uint32 packed_z = r_u32(normals[vertex] + 4);
                    xport_gte_write_data((vertex << 1) + 1, packed_z);
                }
                uint32 rgb = r_u32(frame + 0x4c);
                xport_gte_write_data(6, rgb);
                xport_gte_write_data(8, 0);
                xport_gte_execute(0x0f80416);
                w_u32(packet + 4, xport_gte_read_data(20));
                w_u32(packet + 0x0c, xport_gte_read_data(21));
                w_u32(packet + 0x14, xport_gte_read_data(22));
                w_u8(packet_fields - 4, 6);
                w_u8(packet_fields, 0x30);
                packet_fields += 0x1c;
                uint32 tag = r_u32(order_table);
                w_u32(packet, (tag & 0x00ffffff) | 0x06000000);
                w_u32(order_table, packet & 0x00ffffff);
                uint32 count = r_u32(buffer);
                packet += 0x1c;
                w_u32(buffer, count + 1);
            }
        }
        cursor += 0x0c;
        indices += 0x0c;
    } while (last >= cursor);
    return packet;
}

void G3Function80020CF4(uint32 text, uint32 argument_1, uint32 argument_2, uint32 argument_3, uint32 guest_stack, G3TextMeasureOperands *operands)
{
    FUNCTION_MARKER(0x80020cf4, "SLUS_008.06");
    w_u32(guest_stack, text);
    w_u32(guest_stack + 4, argument_1);
    w_u32(guest_stack + 8, argument_2);
    w_u32(guest_stack + 0x0c, argument_3);
    uint32 frame = guest_stack - 0x18;
    w_u32(frame + 0x18, text);
    G3Function80023030(text, 1, frame + 0x1c, argument_3, frame, operands);
}

uint32 G3Function80022E90(uint32 x, uint32 y)
{
    FUNCTION_MARKER(0x80022e90, "SLUS_008.06");
    uint32 signed_x = (uint32)(sint32)(sint16)(x & 0xffff);
    uint32 signed_y = (uint32)(sint32)(sint16)(y & 0xffff);
    w_u32(0x800a3c5c, signed_x);
    w_u32(0x800a3c60, signed_y);
    return 0x800a2c44;
}

uint32 G3Function80038F18(uint32 level, uint32 count_pointer)
{
    FUNCTION_MARKER(0x80038f18, "SLUS_008.06");
    if (count_pointer != 0)
        w_u32(count_pointer, 0x1c);
    if (level >= 0x1c)
        level = 0;
    uint32 entry = 0x80099708 + (((level << 3) - level) << 2);
    uint32 token = (uint32)(sint32)r_s16(entry + 4);
    return G3Function80065D38(token);
}

uint32 G3Function8002D588(uint32 incoming_second_glyph, uint32 incoming_third_glyph, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002d588, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 saved_game = r_u32(0x80097048);
    uint32 level = r_u16(saved_game + 0x68b4);
    uint32 count = G3Function80039010(level);
    uint32 game = r_u32(0x80097048);
    uint32 order_table = r_u32(game + 0x38);
    uint32 saved_flags = r_u32(game + 0x6834);
    if ((count << 16) == 0)
        count = 1;
    w_u32(game + 0x6834, 0);
    G3Function80028F84(saved_game, incoming_second_glyph, incoming_third_glyph, frame);
    game = r_u32(0x80097048);
    w_u32(game + 0x6834, saved_flags);
    uint32 controls = saved_game + 0x70;
    uint32 direction = r_u32(controls + 4);
    uint32 previous = r_u16(0x8009d8e0);
    if ((direction & 1) != 0)
    {
        if ((sint32)(previous << 16) > 0)
            w_u16(0x8009d8e0, previous - 1);
        else
            w_u16(0x8009d8e0, 0);
    }
    else if ((direction & 2) != 0)
    {
        sint32 signed_previous = (sint16)previous;
        sint32 signed_count = (sint16)(count & 0xffff);
        uint32 next = signed_previous < signed_count - 1 ? previous + 1 : count - 1;
        w_u16(0x8009d8e0, next);
    }
    game = r_u32(0x80097048);
    level = r_u16(game + 0x68b4);
    if (level - 0x10 < 4)
    {
        G3Function8002786C(game, 0x8009d8ec, frame + 0x10, 0);
        game = r_u32(0x80097048);
        G3Function8002786C(game, 0x8009d8f8, frame + 0x14, 0);
        sint32 selection = r_s16(0x8009d8e0);
        if (selection == 1)
        {
            uint32 first_unlocked = r_u32(frame + 0x10);
            if (first_unlocked == 0)
            {
                direction = r_u32(controls + 4);
                if ((direction & 2) != 0)
                    G3Function8002D550((uint32)(sint32)(sint16)(count & 0xffff));
                else
                    w_u16(0x8009d8e0, 0);
                selection = r_s16(0x8009d8e0);
            }
        }
        if (selection == 2)
        {
            uint32 second_unlocked = r_u32(frame + 0x14);
            if (second_unlocked == 0)
            {
                direction = r_u32(controls + 4);
                if ((direction & 2) != 0)
                {
                    uint32 first_unlocked = r_u32(frame + 0x10);
                    w_u16(0x8009d8e0, first_unlocked != 0 ? 1 : 0);
                }
                else
                    G3Function8002D550((uint32)(sint32)(sint16)(count & 0xffff));
            }
        }
    }
    uint32 signed_previous = (uint32)(sint32)(sint16)previous;
    sint32 selection = r_s16(0x8009d8e0);
    if ((sint32)signed_previous != selection)
        G3Function8003F128(0x829, 0);
    selection = r_s16(0x8009d8e0);
    uint32 signed_count = (uint32)(sint32)(sint16)(count & 0xffff);
    uint32 current_row = G3Function8002D03C((uint32)selection, signed_count, frame);
    uint32 previous_row = G3Function8002D03C(signed_previous, signed_count, frame);
    current_row = (uint32)(sint32)(sint16)(current_row & 0xffff);
    previous_row = (uint32)(sint32)(sint16)(previous_row & 0xffff);
    if (current_row != previous_row)
    {
        G3Function8002CF7C(current_row, previous_row, 0, order_table);
        G3Function8002CF7C(current_row, previous_row, 0x100, order_table);
    }
    uint32 result = r_u32(saved_game + 0x9c) & 0x80;
    if (result != 0)
    {
        G3Function8003F128(0x827, 0);
        selection = r_s16(0x8009d8e0);
        w_u32(saved_game + 0x68f0, (uint32)(selection + 1));
        result = 0x100;
        w_u16(0x800a3da2, result);
    }
    return result;
}

uint32 G3Function8002D550(uint32 count)
{
    FUNCTION_MARKER(0x8002d550, "SLUS_008.06");
    sint32 maximum = (sint16)(count & 0xffff);
    --maximum;
    sint32 selection = r_s16(0x8009d8e0);
    uint32 raw_selection = r_u16(0x8009d8e0);
    uint32 result = raw_selection + 1;
    if (selection < maximum)
        w_u16(0x8009d8e0, result);
    else
        w_u16(0x8009d8e0, 0);
    return result;
}

uint32 G3Function8002D03C(uint32 selection, uint32 count, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002d03c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x10;
    uint32 value;
    if ((count & 0xffff) < 2)
        value = 0x78;
    else if ((sint16)(count & 0xffff) == 2)
    {
        w_u16(frame, 0x72);
        w_u16(frame + 2, 0x0a6);
        uint32 offset = (uint32)(sint32)(sint16)(selection & 0xffff) << 1;
        value = r_u16(frame + offset);
    }
    else
    {
        uint32 entries = r_masked_u32(0x8009d8e4, 0x0f);
        entries = r_masked_u32(0x8009d8e4, 0x0f);
        sint32 final_entry = r_s16(0x8009d8e8);
        w_masked_u32(frame + 8, entries, 0x0f);
        w_masked_u32(frame + 8, entries, 0x0f);
        w_u16(frame + 0x0c, (uint32)final_entry);
        uint32 offset = (uint32)(sint32)(sint16)(selection & 0xffff) << 1;
        value = r_u16(frame + offset + 8);
    }
    return (uint32)(sint32)(sint16)((value - 9) & 0xffff);
}

uint32 G3Function80039010(uint32 level)
{
    FUNCTION_MARKER(0x80039010, "SLUS_008.06");
    if (level >= 0x1c)
        level = 0;
    uint32 entry = 0x80099708 + (((level << 3) - level) << 2);
    return (uint32)(sint32)r_s16(entry + 0x0a);
}

uint32 G3Function8002CF7C(uint32 first, uint32 second, uint32 y, uint32 order_table)
{
    FUNCTION_MARKER(0x8002cf7c, "SLUS_008.06");
    uint32 signed_second = (uint32)(sint32)(sint16)(second & 0xffff);
    uint32 signed_y = (uint32)(sint32)(sint16)(y & 0xffff);
    uint32 old_row = (signed_second + signed_y) << 16;
    G3Function8002CF1C(0, old_row | 0x15, 0x200010, order_table);
    G3Function8002CF1C(0, old_row | 0x25, 0x200010, order_table);
    G3Function8002CF1C(0, old_row | 0x35, 0x200010, order_table);
    uint32 signed_first = (uint32)(sint32)(sint16)(first & 0xffff);
    uint32 new_row = (signed_first + signed_y) << 16;
    return G3Function8002CF1C(old_row | 0x15, new_row | 0x15, 0x200030, order_table);
}

uint32 G3Function8002CF1C(uint32 source, uint32 destination, uint32 dimensions, uint32 order_table)
{
    FUNCTION_MARKER(0x8002cf1c, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 buffer = r_u32(game + 0x68);
    uint32 packet = r_u32(buffer + 4);
    w_u32(packet + 4, 0x01000000);
    w_u32(packet + 8, 0x80000000);
    w_u32(packet + 0x0c, source);
    w_u32(packet + 0x10, destination);
    w_u32(packet + 0x14, dimensions);
    uint32 link = r_u32(order_table);
    w_u32(packet, (link & 0x00ffffff) | 0x05000000);
    w_u32(order_table, packet & 0x00ffffff);
    buffer = r_u32(game + 0x68);
    w_u32(buffer + 4, packet + 0x18);
    return buffer;
}

uint32 G3Function8002786C(uint32 game, uint32 name, uint32 value_pointer, uint32 *matched_entry)
{
    FUNCTION_MARKER(0x8002786c, "SLUS_008.06");
    w_u32(value_pointer, 0);
    uint32 index = 0;
    uint32 entry = 0x80096dd4;
    do
    {
        uint32 first_name_word = r_u32(name);
        uint32 first_entry_word = r_u32(entry);
        if (first_name_word == first_entry_word)
        {
            uint32 second_name_word = r_u32(name + 4);
            uint32 second_entry_word = r_u32(entry + 4);
            if (second_name_word == second_entry_word)
            {
                uint32 flags = r_u32(game + ((index >> 5) << 2) + 0x69c8);
                uint32 bit = 1;
                bit <<= index & 0x1f;
                w_u32(value_pointer, flags & bit);
                if (matched_entry != 0)
                    *matched_entry = entry;
                return 1;
            }
        }
        ++index;
        entry += 0x10;
    } while (index < 0x27);
    if (matched_entry != 0)
        *matched_entry = entry;
    return 0;
}

uint32 G3Function8003F128(uint32 sound, G3TextMeasureOperands *operands)
{
    FUNCTION_MARKER(0x8003f128, "SLUS_008.06");
    sound &= 0xffff;
    if (sound == 0)
        return 0;
    uint32 game = r_u32(0x80097048);
    if (r_u8(game + 0x67df) == 0)
        return 0;
    uint32 enabled = r_u8(game + 0x67dc);
    if (operands != 0)
        operands->first_glyph = 0x5a;
    if (enabled == 0)
        return 0;
    uint32 handle = G3Function8004BB6C(sound, 0x5a, 0x40, 0);
    if (operands != 0)
    {
        operands->second_glyph = 0x40;
        operands->third_glyph = handle;
    }
    return handle;
}

uint32 G3Function8004BB6C(uint32 sound, uint32 volume, uint32 pan, uint32 option)
{
    FUNCTION_MARKER(0x8004bb6c, "SLUS_008.06");
    uint32 handle = G3Function8004BFBC(sound & 0xffff);
    uint32 signed_option = (uint32)(sint32)(sint16)(option & 0xffff);
    G3Function8004C008(0, volume & 0xff, pan & 0xff, handle, 0, signed_option);
    return handle;
}

uint32 G3Function8004BFBC(uint32 sound)
{
    FUNCTION_MARKER(0x8004bfbc, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 serial = r_u16(audio + 0x244);
    uint32 next = serial + 1;
    w_u16(audio + 0x244, next);
    if ((next & 0xffff) == 0)
        w_u16(audio + 0x244, serial + 2);
    audio = r_u32(0x80098ff0);
    serial = r_u16(audio + 0x244);
    return (serial << 16) | (sound & 0xffff);
}

void G3Function8004C008(uint32 command, uint32 volume, uint32 pan, uint32 handle, uint32 data, uint32 option)
{
    FUNCTION_MARKER(0x8004c008, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 count = r_u16(audio + 0x242);
    if (count >= 0x1e)
    {
        command = 4;
        if (count >= 0x1f)
            return;
    }
    G3EnterCritical8008A05C();
    audio = r_u32(0x80098ff0);
    uint32 index = r_u8(audio + 0x240);
    uint32 entry = audio + (index << 4) + 0x40;
    w_u8(entry, command);
    w_u8(entry + 1, volume);
    w_u8(entry + 2, pan);
    audio = r_u32(0x80098ff0);
    w_u32(entry + 4, handle);
    w_u16(entry + 0x0c, option);
    w_u32(entry + 8, data);
    index = r_u8(audio + 0x240) + 1;
    w_u8(audio + 0x240, index);
    if ((index & 0xff) == 0x20)
    {
        uint32 current_audio = r_u32(0x80098ff0);
        w_u8(current_audio + 0x240, 0);
    }
    audio = r_u32(0x80098ff0);
    count = r_u16(audio + 0x242);
    w_u16(audio + 0x242, count + 1);
    G3ExitCritical8008A03C();
}

uint32 G3Function8003EDE8(uint32 volume)
{
    FUNCTION_MARKER(0x8003ede8, "SLUS_008.06");
    if (volume == 0xffffffff)
    {
        uint32 game = r_u32(0x80097048);
        volume = r_u32(game + 0x67d0);
    }
    uint32 game = r_u32(0x80097048);
    w_u32(game + 0x67d0, volume);
    return G3Function80048718(volume & 0xffff);
}

uint32 G3Function8003EE30(uint32 volume)
{
    FUNCTION_MARKER(0x8003ee30, "SLUS_008.06");
    if (volume == 0xffffffff)
    {
        uint32 game = r_u32(0x80097048);
        volume = r_u32(game + 0x67d4);
    }
    uint32 game = r_u32(0x80097048);
    w_u32(game + 0x67d4, volume);
    return G3Function80048700(volume & 0xffff);
}

uint32 G3Function8003EE78(uint32 volume)
{
    FUNCTION_MARKER(0x8003ee78, "SLUS_008.06");
    if (volume == 0xffffffff)
    {
        uint32 game = r_u32(0x80097048);
        volume = r_u32(game + 0x67d8);
    }
    uint32 game = r_u32(0x80097048);
    uint32 enabled = (sint32)volume > 0 ? 1 : 0;
    w_u32(game + 0x67d8, volume);
    w_u8(game + 0x67de, enabled);
    return enabled;
}

uint32 G3Function80048718(uint32 volume)
{
    FUNCTION_MARKER(0x80048718, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    w_u8(audio + 0x11, volume);
    audio = r_u32(0x80098ff0);
    if (r_u8(audio + 4) != 0)
    {
        uint32 index = 0;
        uint32 count;
        do
        {
            audio = r_u32(0x80098ff0);
            uint32 bank = r_u32(audio + ((index & 0xffff) << 2) + 0x20);
            ++index;
            G3Function8004A504(bank);
            audio = r_u32(0x80098ff0);
            count = r_u8(audio + 4);
        } while ((index & 0xffff) < count);
    }
    return 0;
}

uint32 G3Function80048700(uint32 volume)
{
    FUNCTION_MARKER(0x80048700, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    w_u8(audio + 0x10, volume);
    return 0;
}

uint32 G3Function8004A504(uint32 bank)
{
    FUNCTION_MARKER(0x8004a504, "SLUS_008.06");
    uint32 voice = 0;
    do
    {
        uint32 audio = r_u32(0x80098ff0);
        uint32 entry = audio + (((voice << 3) - voice) << 2) + 0x248;
        uint32 channel_flags = r_u8(entry + 8);
        uint32 bank_flags = r_u8(bank + 0xfd);
        uint32 channel = channel_flags & 0x0f;
        if ((channel_flags & 0xf0) == bank_flags)
        {
            uint32 gain = r_u8(entry + 0x0d);
            uint32 left = gain * gain;
            gain = r_u8(entry + 0x0d);
            uint32 right = gain * gain;
            uint32 mode = r_u16(audio + 0x12);
            if ((mode & 1) == 0)
            {
                uint32 pan = r_u8(bank + channel + 0x156);
                if (pan >= 0x41)
                {
                    uint32 attenuation = (0x7f - pan) & 0xff;
                    uint32 product = (uint32)(sint32)(sint16)(right & 0xffff) * attenuation;
                    product *= attenuation;
                    left = (uint32)((sint32)product >> 12) & 0xffff;
                }
                else if (pan < 0x3f)
                {
                    pan = r_u8(bank + channel + 0x156);
                    uint32 product = (uint32)(sint32)(sint16)(left & 0xffff) * pan;
                    product *= pan;
                    right = (uint32)((sint32)product >> 12) & 0xffff;
                }
            }
            uint32 tone = r_u32(entry + 0x14);
            gain = r_u8(tone + 2);
            uint32 squared = gain * gain;
            uint32 left_product = (uint32)(sint32)(sint16)(left & 0xffff) * squared;
            uint32 right_product = (uint32)(sint32)(sint16)(right & 0xffff) * squared;
            audio = r_u32(0x80098ff0);
            left = (left_product >> 14) & 0xffff;
            right = (right_product >> 14) & 0xffff;
            mode = r_u16(audio + 0x12);
            if ((mode & 1) == 0)
            {
                tone = r_u32(entry + 0x14);
                uint32 pan = r_u8(tone + 3);
                if (pan >= 0x41)
                {
                    uint32 attenuation = (0x7f - pan) & 0xff;
                    uint32 product = (uint32)(sint32)(sint16)(right & 0xffff) * attenuation;
                    product *= attenuation;
                    left = (uint32)((sint32)product >> 12) & 0xffff;
                }
                else if (pan < 0x3f)
                {
                    pan = r_u8(tone + 3);
                    uint32 product = (uint32)(sint32)(sint16)(left & 0xffff) * pan;
                    product *= pan;
                    right = (uint32)((sint32)product >> 12) & 0xffff;
                }
            }
            gain = r_u8(bank + channel + 0x146);
            squared = gain * gain;
            left_product = (uint32)(sint32)(sint16)(left & 0xffff) * squared;
            right_product = (uint32)(sint32)(sint16)(right & 0xffff) * squared;
            left = (left_product >> 14) & 0xffff;
            right = (right_product >> 14) & 0xffff;
            uint32 instrument = r_u32(entry + 0x18);
            gain = r_u8(instrument + 4);
            squared = gain * gain;
            left_product = (uint32)(sint32)(sint16)(left & 0xffff) * squared;
            right_product = (uint32)(sint32)(sint16)(right & 0xffff) * squared;
            left = (left_product >> 14) & 0xffff;
            right = (right_product >> 14) & 0xffff;
            gain = r_u8(bank + 0xfa);
            squared = gain * gain;
            left_product = (uint32)(sint32)(sint16)(left & 0xffff) * squared;
            right_product = (uint32)(sint32)(sint16)(right & 0xffff) * squared;
            left = (left_product >> 14) & 0xffff;
            right = (right_product >> 14) & 0xffff;
            uint32 master = r_u32(bank + 0x100);
            gain = r_u8(master);
            squared = gain * gain;
            left_product = (uint32)(sint32)(sint16)(left & 0xffff) * squared;
            right_product = (uint32)(sint32)(sint16)(right & 0xffff) * squared;
            left = (left_product >> 14) & 0xffff;
            right = (right_product >> 14) & 0xffff;
            SpuSetVoiceVolume((sint32)voice, (sint16)left, (sint16)right);
        }
        ++voice;
    } while (voice < 0x18);
    return 0;
}

uint32 G3Function8003EB30(uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003eb30, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    w_u16(frame + 0x10, 1);
    w_u16(frame + 0x12, 4);
    G3Function80048344(frame + 0x10);
    G3Function80048368(frame + 0x10, 0x800a1594, frame);
    uint32 game = r_u32(0x80097048);
    w_u32(game + 0x67cc, 0x3fff);
    G3Function8003EDE8(0x68);
    G3Function8003EE30(0x74);
    G3Function8003EE78(0x68);
    game = r_u32(0x80097048);
    w_u8(game + 0x67dc, 1);
    game = r_u32(0x80097048);
    w_u8(game + 0x67dd, 1);
    game = r_u32(0x80097048);
    w_u8(game + 0x67de, 1);
    game = r_u32(0x80097048);
    w_u8(game + 0x67df, 0);
    return G3Function800487A0(1, 1);
}

uint32 G3Function80048344(uint32 configuration)
{
    FUNCTION_MARKER(0x80048344, "SLUS_008.06");
    uint32 banks = r_u16(configuration + 2);
    uint32 triple = (banks << 1) + banks;
    return (((triple << 4) - triple) << 3) + 0x5e4;
}

uint32 G3Function80048368(uint32 configuration, uint32 buffer, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80048368, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    G3EnterCritical8008A05C();
    uint32 bytes = G3Function80048344(configuration);
    w_u32(0x80098ff0, buffer);
    if (buffer == 0)
        return 0x1009;
    G3FillMemory80086094(buffer, 0, bytes);
    SpuInit();
    w_u32(frame + 0x10, 0x0f);
    w_u16(frame + 0x18, 0);
    w_u16(frame + 0x1a, 0);
    w_u16(frame + 0x14, 0);
    w_u16(frame + 0x16, 0);
    SpuSetCommonAttr((SpuCommonAttr *)psx_addr(frame + 0x10, sizeof(SpuCommonAttr)));
    spu_native_set_mute(0);
    uint32 bank_count = r_u16(configuration + 2);
    if (bank_count != 0)
    {
        uint32 index = 0;
        do
        {
            uint32 slot = index & 0xffff;
            uint32 triple = (slot << 1) + slot;
            uint32 offset = (((triple << 4) - triple) << 3) + 0x5e4;
            uint32 audio = r_u32(0x80098ff0);
            uint32 bank = audio + offset;
            w_u32(audio + (slot << 2) + 0x20, bank);
            w_u8(bank + 0xf0, index);
            w_u8(bank + 0xfd, index << 4);
            w_u8(bank + 0xfa, 0x7f);
            w_u8(bank + 0xfb, 0x3f);
            audio = r_u32(0x80098ff0);
            w_u16(bank + 0xee, 0xff);
            w_u32(bank + 0x100, audio + 0x11);
            uint32 channel = 0;
            do
            {
                uint32 channel_slot = channel & 0xffff;
                ++channel;
                w_u8(bank + channel_slot + 0x126, 0xff);
                w_u8(bank + channel_slot + 0x136, 0);
                w_u8(bank + channel_slot + 0x146, 0x7f);
                w_u8(bank + channel_slot + 0x156, 0x3f);
                w_u16(bank + (channel_slot << 1) + 0x106, 0x2000);
            } while ((channel & 0xffff) < 0x10);
            w_u8(bank + 0xec, index);
            ++index;
            w_u32(bank + 0xe8, bank);
            bank_count = r_u16(configuration + 2);
        } while ((index & 0xffff) < bank_count);
    }
    uint32 audio = r_u32(0x80098ff0);
    w_u16(audio + 0x244, 0x3039);
    w_u8(audio + 0x246, 0x7f);
    audio = r_u32(0x80098ff0);
    uint32 value = r_u8(configuration + 2);
    w_u8(audio + 4, value);
    audio = r_u32(0x80098ff0);
    value = r_u8(configuration);
    w_u8(audio + 5, value);
    audio = r_u32(0x80098ff0);
    w_u8(audio + 0x10, 0x7f);
    audio = r_u32(0x80098ff0);
    w_u8(audio + 0x11, 0x7f);
    audio = r_u32(0x80098ff0);
    w_u32(audio + 0x558, 0);
    w_u32(audio + 0x554, 0);
    uint32 voice = 0;
    do
    {
        uint32 slot = voice & 0xffff;
        uint32 offset = ((slot << 3) - slot) << 2;
        audio = r_u32(0x80098ff0);
        uint32 entry = audio + offset;
        w_u8(entry + 0x250, 0xff);
        audio = r_u32(0x80098ff0);
        uint32 bit = 1;
        bit <<= slot & 0x1f;
        w_u32(entry + 0x248, bit);
        w_u8(audio + offset + 0x258, voice);
        ++voice;
    } while ((voice & 0xffff) < 0x18);
    audio = r_u32(0x80098ff0);
    uint32 timer = r_u8(audio + 5);
    w_u32(audio + 0x530, 0);
    w_u32(audio + 0x534, 0);
    w_u32(audio + 0x538, 0);
    if (timer < 4)
    {
        timer = r_u8(audio + 5);
        uint32 period = r_u16(0x80098fd8 + (timer << 1));
        G3Function800485C8(0x80048870, period);
    }
    audio = r_u32(0x80098ff0);
    w_u16(audio + 0x12, 0);
    G3ExitCritical8008A03C();
    return 0;
}

uint32 G3Function800487A0(uint32 bank, uint32 selector)
{
    FUNCTION_MARKER(0x800487a0, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 target = r_u32(audio + ((bank & 0xffff) << 2) + 0x20);
    selector &= 0xffff;
    if (selector == 0)
        w_u32(target + 0x100, audio + 0x11);
    else if (selector == 1)
        w_u32(target + 0x100, audio + 0x10);
    return 0;
}

void G3Function800485C8(uint32 callback, uint32 period)
{
    G3EnterCritical8008A05C();
    if (callback != 0x80048870)
    {
        {
#ifndef NDEBUG
        fprintf(stderr, "G3 unresolved audio timer callback %08X\n", (unsigned int)callback);
#endif
        }
        abort();
    }
    sint32 event = OpenEvent(0xf2000001, 2, 0x1000, G3NativeAudioEvent);
    w_u32(0x800ad0d0, (uint32)event);
    EnableEvent(event);
    G3StartAudioFieldClock(period);
    G3ExitCritical8008A03C();
}

uint32 G3Function80048648(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80048648, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 audio = r_u32(0x80098ff0);
    w_u16(audio + 6, 0);
    w_u32(frame + 0x10, 0x3fcf);
    w_u16(frame + 0x18, 0);
    w_u16(frame + 0x1a, 0);
    w_u16(frame + 0x14, 0);
    w_u16(frame + 0x16, 0);
    w_u16(frame + 0x20, 0);
    w_u16(frame + 0x22, 0);
    w_u32(frame + 0x24, 0);
    w_u32(frame + 0x28, 0);
    w_u16(frame + 0x2c, 0);
    w_u16(frame + 0x2e, 0);
    w_u32(frame + 0x30, 0);
    w_u32(frame + 0x34, 0);
    SpuSetCommonAttr((SpuCommonAttr *)psx_addr(frame + 0x10, sizeof(SpuCommonAttr)));
    return 0;
}

uint32 G3Function800486AC(uint32 volume)
{
    FUNCTION_MARKER(0x800486ac, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    w_u16(audio + 6, volume);
    uint32 signed_volume = (uint32)(sint32)(sint16)(volume & 0xffff);
    G3Function8008C1E4(signed_volume, signed_volume);
    return 0;
}

uint32 G3Function800486E0(uint32 target, uint32 step, uint32 callback)
{
    FUNCTION_MARKER(0x800486e0, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    w_u16(audio + 8, step);
    w_u16(audio + 0x0a, target);
    w_u32(audio + 0x0c, callback);
    return 0;
}

uint32 G3Function80049BF8(uint32 output, uint32 bank)
{
    FUNCTION_MARKER(0x80049bf8, "SLUS_008.06");
    uint32 cursor = r_u32(bank + 0xe4);
    uint32 delta = r_u8(cursor);
    ++cursor;
    if ((delta & 0x80) != 0)
    {
        delta &= 0x7f;
        uint32 part;
        do
        {
            part = r_u8(cursor);
            delta = (delta << 7) | (part & 0x7f);
            ++cursor;
        } while ((part & 0x80) != 0);
    }
    uint32 flags = r_u16(bank + 0xf4);
    if ((flags & 1) != 0)
    {
        flags = r_u16(bank + 0xf4);
        uint32 timestamp = r_u32(bank);
        w_u32(bank + 0xdc, timestamp);
        w_u32(bank + 0xe0, timestamp);
        w_u16(bank + 0xf4, flags & 0xfffe);
    }
    w_u32(output, delta);
    uint32 timestamp = r_u32(bank + 0xdc);
    w_u32(bank + 0xdc, timestamp + delta);
    uint32 status = r_u8(cursor);
    uint32 bytes;
    if (status == 0xff)
    {
        ++cursor;
        uint32 command = r_u8(cursor);
        ++cursor;
        w_u8(output + 4, command);
        bytes = r_u8(cursor);
        ++cursor;
        if (command == 0x44)
        {
            flags = r_u16(bank + 0xf4);
            w_u16(bank + 0xf4, flags | 0x10);
        }
    }
    else
    {
        if ((status & 0x80) != 0)
        {
            ++cursor;
            w_u8(output + 4, status);
            w_u8(bank + 0xfc, status);
        }
        else
        {
            status = r_u8(bank + 0xfc);
            w_u8(output + 4, status);
        }
        status = r_u8(output + 4);
        bytes = r_u8(0x80099038 + ((status >> 4) & 7));
    }
    uint32 remaining = bytes - 1;
    uint32 index = 0;
    while ((remaining & 0xffff) != 0xffff)
    {
        uint32 destination_offset = index;
        ++index;
        uint32 value = r_u8(cursor);
        ++cursor;
        --remaining;
        w_u8(output + (destination_offset & 0xffff) + 5, value);
    }
    return cursor;
}

uint32 G3Function8004CC40(uint32 command)
{
    FUNCTION_MARKER(0x8004cc40, "SLUS_008.06");
    uint32 voice = 0;
    uint32 attributes = r_u32(command + 4);
    uint32 bit = 1;
    uint32 mask = r_u32(attributes);
    do
    {
        if ((bit & mask) != 0)
            break;
        ++voice;
        bit <<= 1;
    } while (voice < 0x18);
    sint16 left = r_s16(attributes + 8);
    sint16 right = r_s16(attributes + 0x0a);
    SpuSetVoiceVolume((sint32)voice, left, right);
    uint32 pitch = r_u16(attributes + 0x14);
    SpuSetVoicePitch((sint32)voice, (uint16)pitch);
    uint32 start = r_u32(attributes + 0x1c);
    SpuSetVoiceStartAddr((sint32)voice, start);
    uint32 adsr_1 = r_u16(attributes + 0x3a);
    uint32 adsr_2 = r_u16(attributes + 0x3c);
    return G3Function8004CDE4(voice, adsr_1, adsr_2);
}
