#include "xport.h"
#include "psx.h"
#include "psx_gte.h"
#include "game_functions.h"

uint32 G3Function80011970(uint32 game, uint32 context, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80011970, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    sint32 mode = r_s16(game + 0x6786);
    bool callbacks = mode == 0;
    if (!callbacks)
    {
        sint32 state = r_s16(game + 0x67ee);
        callbacks = state == 1 || mode == 4;
    }
    if (callbacks)
    {
        G3Function800126EC();
        G3Function800126DC();
        G3Function800126BC();
        G3Function800126C4();
        G3Function800126D4();
        G3Function800126CC();
        G3Function80012F94();
        G3Function80012F9C();
        mode = r_s16(game + 0x6786);
    }
    if (mode == 0)
        return G3Function80011FA0(game, frame);
    sint32 state = r_s16(game + 0x67ee);
    if (state == 1 || mode == 6 || mode == 4)
        return G3Function80011FA0(game, frame);
    return 4;
}

void G3Function80011F98(void)
{
    FUNCTION_MARKER(0x80011f98, "SLUS_008.06");
}

void G3Function8001294C(void)
{
    FUNCTION_MARKER(0x8001294c, "SLUS_008.06");
}

void G3Function800126BC(void)
{
    FUNCTION_MARKER(0x800126bc, "SLUS_008.06");
}

void G3Function800126C4(void)
{
    FUNCTION_MARKER(0x800126c4, "SLUS_008.06");
}

void G3Function800126CC(void)
{
    FUNCTION_MARKER(0x800126cc, "SLUS_008.06");
}

void G3Function800126D4(void)
{
    FUNCTION_MARKER(0x800126d4, "SLUS_008.06");
}

void G3Function800126DC(void)
{
    FUNCTION_MARKER(0x800126dc, "SLUS_008.06");
}

void G3Function800126EC(void)
{
    FUNCTION_MARKER(0x800126ec, "SLUS_008.06");
}

void G3Function80012F94(void)
{
    FUNCTION_MARKER(0x80012f94, "SLUS_008.06");
}

void G3Function80012F9C(void)
{
    FUNCTION_MARKER(0x80012f9c, "SLUS_008.06");
}

uint32 G3Function80011430(uint32 index, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80011430, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 entry = 0x800961ac + (index << 4);
    uint32 target = r_u32(entry + 4);
    uint32 mask = r_u32(entry + 8);
    uint32 value = r_u32(target);
    w_u32(target, value ^ mask);
    uint32 bit = uint32(1) << (index & 0x1f);
    if (bit == 1)
    {
        uint32 object = r_u32(0x8009efc4);
        G3Function80018CC4(object, 0, frame);
    }
    else if (bit == 2 || bit == 4 || bit == 8)
    {
        uint32 level = bit == 2 ? 0x11 : (bit == 4 ? 0x12 : 0x13);
        w_u32(0x800aa7b8, level);
        w_u32(0x800aa7c0, 1);
        return G3Function80027584(0x17, frame);
    }
    else if (bit == 0x10)
        w_u32(0x8009ceac, 0x800965fc);
    else if (bit == 0x20 || bit == 0x40 || bit == 0x80)
    {
        uint32 name = bit == 0x20 ? 0x8009d030 : (bit == 0x40 ? 0x8009d048 : 0x8009d060);
        uint32 game = r_u32(0x80097048);
        G3Function800277A4(game, name, 1);
        name = bit == 0x20 ? 0x8009d03c : (bit == 0x40 ? 0x8009d054 : 0x8009d06c);
        game = r_u32(0x80097048);
        G3Function800277A4(game, name, 1);
    }
    else if (bit == 0x100)
    {
        uint32 game = r_u32(0x80097048);
        w_u16(game + 0x681e, 8);
        w_u16(game + 0x681c, 8);
        G3Function80047EE4(7, 8);
    }
    else if (bit == 0x200)
    {
        uint32 game = r_u32(0x80097048);
        uint32 lives = r_u32(game + 0x6818) + 0x0a;
        w_u32(game + 0x6818, lives);
        if ((sint32)lives >= 0x64)
            w_u32(game + 0x6818, 0x63);
        game = r_u32(0x80097048);
        uint32 data = r_u32(0x80098814);
        uint32 object = r_u32(game + 0x50);
        G3Function8002A454(object, data, frame);
    }
    else if (bit == 0x800)
        w_u32(0x8009ceac, 0x8009629c);
    else if (bit == 0x1000)
    {
        G3SdkPrint80085E84(0x8009d078);
        w_u32(0x8009ceac, 0x80095924);
    }
    else if (bit == 0x4000)
        w_u32(0x8009ceac, 0x80096824);
    return 1;
}

uint32 G3Function8003F1C4(uint32 sound, uint32 volume, uint32 pan, uint32 option)
{
    FUNCTION_MARKER(0x8003f1c4, "SLUS_008.06");
    sound &= 0xffff;
    if (sound == 0)
        return 0;
    uint32 game = r_u32(0x80097048);
    uint32 enabled = r_u8(game + 0x67df);
    if (enabled == 0)
        return 0;
    enabled = r_u8(game + 0x67dc);
    if (enabled == 0)
        return 0;
    uint32 pitch = (uint32)(sint32)(sint16)(option & 0xffff);
    return G3Function8004BB6C(sound, volume & 0xffff, pan & 0xffff, pitch);
}

uint32 G3Function80049778(uint32 output)
{
    FUNCTION_MARKER(0x80049778, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 offset = 0;
    do
    {
        uint32 count = offset == 0 ? 4 : 2;
        uint32 values[4];
        uint32 index = 0;
        do
        {
            uint32 source = audio + 0x4e8 + offset + (index << 2);
            uint32 upper = r_u32((source + 3) & ~uint32(3));
            uint32 lower = r_u32(source & ~uint32(3));
            uint32 shift = (source & 3) << 3;
            values[index] = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
            ++index;
        } while (index < count);
        index = 0;
        do
        {
            uint32 destination = output + offset + (index << 2);
            uint32 value = values[index];
            uint32 high_address = destination + 3;
            uint32 high_byte = high_address & 3;
            uint32 preserve = high_byte == 3 ? 0 : uint32(0xffffffff) << ((high_byte + 1) << 3);
            uint32 previous = r_u32(high_address & ~uint32(3));
            w_u32(high_address & ~uint32(3), (previous & preserve) | (value >> ((3 - high_byte) << 3)));
            uint32 low_byte = destination & 3;
            preserve = low_byte == 0 ? 0 : uint32(0xffffffff) >> ((4 - low_byte) << 3);
            previous = r_u32(destination & ~uint32(3));
            w_u32(destination & ~uint32(3), (previous & preserve) | (value << (low_byte << 3)));
            ++index;
        } while (index < count);
        offset += 0x10;
    } while (offset < 0x18);
    return audio;
}

uint32 G3Function800497EC(uint32 voice)
{
    FUNCTION_MARKER(0x800497ec, "SLUS_008.06");
    voice &= 0xffff;
    uint32 audio = r_u32(0x80098ff0);
    return audio + voice * 28 + 0x248;
}

uint32 G3Function800499FC(uint32 index)
{
    FUNCTION_MARKER(0x800499fc, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    return r_u8(audio + (index & 0xffff) + 0x564);
}

uint32 G3Function800126F4(void)
{
    FUNCTION_MARKER(0x800126f4, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 block = r_u32(game + 0x68);
    uint32 end = r_u32(block + 4);
    uint32 used = end - 0x0c - block;
    w_u32(game + 0x68f4, used);
    return used;
}

uint32 G3Function80012A14(void)
{
    FUNCTION_MARKER(0x80012a14, "SLUS_008.06");
    sint32 count = r_s32(0x800a058c);
    return count >= 0x10 ? 1 : 0;
}

uint32 G3Function8001292C(uint32 game)
{
    FUNCTION_MARKER(0x8001292c, "SLUS_008.06");
    uint32 object = r_u32(0x8009efc4);
    uint32 flags = r_u32(object + 0x10) | 0x100;
    w_u32(object + 0x10, flags);
    return flags;
}

uint32 G3Function80013864(uint32 object, uint32 state, uint32 transition, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80013864, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    uint32 effect = 0;
    bool popped = false;
    if ((sint32)state < 0)
    {
        uint32 count = r_u32(0x80096b24);
        if (count == 0)
            return 0;
        --count;
        w_u32(0x80096b24, count);
        uint32 slot = 0x80096b14 + (count << 2);
        state = r_u32(slot);
        popped = true;
        w_u32(slot, 0);
    }
    uint32 previous_state = r_u32(0x80096a88);
    if (state - 0x0a < 2)
    {
        w_u32(0x80096a7c, state == 0x0a ? 1 : 0);
        return 0;
    }
    uint32 target = r_u32(0x80099654 + (state << 3));
    if (target == 0)
        return 0;
    if (!popped)
    {
        target = r_u32(0x80099654 + (previous_state << 3));
        if (target != 0)
        {
            uint32 game = r_u32(0x80097048);
            uint32 rejected = G3DispatchStateEvent(target, object, game, 0x18, state, frame);
            if (rejected != 0)
                return 0;
        }
    }
    uint32 current = r_u32(0x80096a88);
    if (state == current)
    {
        uint32 game = r_u32(0x80097048);
        target = r_u32(0x80099654 + (state << 3));
        uint32 accepted = G3DispatchStateEvent(target, object, game, 0x15, 0, frame);
        if (accepted == 0)
            return 0;
    }
    current = r_u32(0x80096a88);
    w_u32(frame + 0x14, state);
    w_u32(frame + 0x10, current);
    G3Function80053F98(object, 0, 0x0a, frame + 0x10, frame);
    bool push = !popped && (state == 5 || state == 7 || state == 8 || state == 0x0f || state == 0x14 || state == 6);
    if (push)
    {
        current = r_u32(0x80096a88);
        target = r_u32(0x80099654 + (current << 3));
        uint32 accepted = 0;
        if (target != 0)
        {
            uint32 game = r_u32(0x80097048);
            accepted = G3DispatchStateEvent(target, object, game, 0x16, state, frame);
        }
        if (accepted == 0)
            return 0;
        uint32 count = r_u32(0x80096b24);
        if ((sint32)count >= 4)
            return 0;
        current = r_u32(0x80096a88);
        w_u32(0x80096b14 + (count << 2), current);
        count = r_u32(0x80096b24);
        w_u32(0x80096b24, count + 1);
    }
    else
    {
        current = r_u32(0x80096a88);
        target = r_u32(0x80099654 + (current << 3));
        if (target != 0)
        {
            uint32 game = r_u32(0x80097048);
            G3DispatchStateEvent(target, object, game, 2, state, frame);
        }
    }
    if (transition != 0)
    {
        target = r_u32(0x80099654 + (state << 3));
        if (target != 0)
        {
            uint32 game = r_u32(0x80097048);
            effect = G3DispatchStateEvent(target, object, game, 7, frame + 0x18, frame);
        }
    }
    if (!popped)
    {
        uint32 game = r_u32(0x80097048);
        current = r_u32(0x80096a88);
        target = r_u32(0x80099654 + (state << 3));
        G3DispatchStateEvent(target, object, game, 0, current, frame);
    }
    if (effect != 0)
    {
        uint32 game = r_u32(0x80097048);
        target = r_u32(0x800996dc);
        G3DispatchStateEvent(target, object, game, 0, state, frame);
        G3Function8004E2D0(transition);
        G3Function80015098(0x11);
        uint32 direction = r_u32(0x80096a8c);
        if ((sint32)direction < 0)
            w_u32(0x80096a8c, uint32(0) - direction);
    }
    else
    {
        G3Function80015098(state);
        if (previous_state != 0x13)
        {
            current = r_u32(0x80096a88);
            target = r_u32(0x80099654 + (current << 3));
            if (target != 0)
            {
                uint32 game = r_u32(0x80097048);
                G3DispatchStateEvent(target, object, game, 0x10, 0, frame);
            }
        }
    }
    return 1;
}

uint32 G3Function80013C40(uint32 value)
{
    FUNCTION_MARKER(0x80013c40, "SLUS_008.06");
    if ((sint32)value < 0)
        return 0xffffffff;
    return value != 0 ? 1 : 0;
}

uint32 G3Function80013C54(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80013c54, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 direction = r_u32(0x80096a8c);
    uint32 transition = G3Function80013C40(direction);
    return G3Function80013864(object, 0xffffffff, transition, frame);
}

uint32 G3Function80013C94(uint32 object, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80013c94, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 direction = r_u32(0x80096a8c);
    uint32 transition = G3Function80013C40(direction);
    return G3Function80013864(object, state, transition, frame);
}

uint32 G3Function80013CDC(uint32 object, uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80013cdc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 current = r_u32(0x80096a88);
    w_u32(frame + 0x1c, state);
    if (current == 6)
        return 6;
    if (current == 0x14)
        return 0x14;
    uint32 allowed = G3Function80053F40(frame + 0x1c);
    if (allowed == 0)
        return 0;
    state = r_u32(frame + 0x1c);
    return G3Function80013C94(object, state, frame);
}

uint32 G3Function80014900(void)
{
    FUNCTION_MARKER(0x80014900, "SLUS_008.06");
    uint32 flags = r_u32(0x80096a94);
    if ((flags & 1) == 0)
    {
        uint32 index = r_u32(0x80096a9c);
        uint32 value = r_u16(0x800969c4 + (index << 2));
        w_u16(0x80096b68, value);
    }
    if ((flags & 2) == 0)
    {
        uint32 index = r_u32(0x80096a9c);
        uint32 value = r_u16(0x800969e8 + (index << 2));
        w_u16(0x80096b6a, value);
        return value;
    }
    return flags & 2;
}

uint32 G3Function800140E4(uint32 object, uint32 index, uint32 value, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800140e4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    w_u32(0x80096994 + (index << 2), value);
    G3Function80014900();
    uint32 state = r_u32(0x80096a88);
    uint32 target = r_u32(0x80099654 + (state << 3));
    if (target == 0)
        return 0;
    uint32 game = r_u32(0x80097048);
    return G3DispatchStateEvent(target, object, game, 0x0d, index, frame);
}

void G3Function80014288(uint32 object, uint32 direction)
{
    FUNCTION_MARKER(0x80014288, "SLUS_008.06");
    w_u32(0x80096a8c, direction);
}

uint32 G3Function80014298(uint32 object, uint32 value, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80014298, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 game = r_u32(0x80097048);
    uint32 result = G3Function80053F98(object, game, 0x13, value & 0x0fff, frame);
    w_u16(object + 0x1c, value);
    w_u16(object + 0x14, value);
    w_u16(object + 0x0c, value);
    w_u32(0x80096a90, (uint32)(sint32)(sint16)(value & 0xffff));
    return result;
}

uint32 G3Function8005ADA0(uint32 matrix, uint32 input, uint32 output)
{
    FUNCTION_MARKER(0x8005ada0, "SLUS_008.06");
    uint32 first = r_u32(matrix);
    uint32 second = r_u32(matrix + 4);
    uint32 third = r_u32(matrix + 8);
    uint32 fourth = r_u32(matrix + 0x0c);
    uint32 fifth = r_u16(matrix + 0x10);
    xport_gte_write_control(0, first);
    xport_gte_write_control(1, second);
    xport_gte_write_control(2, third);
    xport_gte_write_control(3, fourth);
    xport_gte_write_control(4, fifth);
    uint32 vector = r_u32(input);
    xport_gte_write_data(0, vector);
    vector = r_u32(input + 4);
    xport_gte_write_data(1, vector);
    xport_gte_execute(0x486012);
    uint32 result = xport_gte_read_data(25);
    w_u32(output, result);
    result = xport_gte_read_data(26);
    w_u32(output + 4, result);
    result = xport_gte_read_data(27);
    w_u32(output + 8, result);
    return output;
}

uint32 G3Function80014164(uint32 object)
{
    FUNCTION_MARKER(0x80014164, "SLUS_008.06");
    uint32 matrix = r_u32(object + 0x80);
    ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(0x80096a14, 8), (SVECTOR *)psx_addr(object + 0x20, 6));
    matrix = r_u32(object + 0x80);
    ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(0x80096a1c, 8), (SVECTOR *)psx_addr(object + 0x26, 6));
    matrix = r_u32(object + 0x80);
    ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(0x80096a24, 8), (SVECTOR *)psx_addr(object + 0x2c, 6));
    matrix = r_u32(object + 0x80);
    ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(0x80096a2c, 8), (SVECTOR *)psx_addr(object + 0x40, 6));
    matrix = r_u32(object + 0x80);
    ApplyMatrixSV((MATRIX *)psx_addr(matrix, 20), (SVECTOR *)psx_addr(0x80096a34, 8), (SVECTOR *)psx_addr(object + 0x46, 6));
    G3Function8005ADA0(object + 0x20, object, object + 0x60);
    return G3Function8005ADA0(object + 0x40, object, object + 0x6c);
}

uint32 G3Function80053F40(uint32 state)
{
    FUNCTION_MARKER(0x80053f40, "SLUS_008.06");
    uint32 value = r_u32(state);
    if (value == 0 || value == 0x10)
        value = 0x10;
    else if ((sint32)value < 0 || (value >= 0x10 && value != 0x12))
    {
        w_u32(state, 0);
        return 0;
    }
    w_u32(state, value);
    return 1;
}

void G3Function80015098(uint32 state)
{
    FUNCTION_MARKER(0x80015098, "SLUS_008.06");
    w_u32(0x80096a88, state);
}

uint32 G3Function80053F98(uint32 object, uint32 sender, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80053f98, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 index = 0;
    do
    {
        uint32 target = r_u32(0x80099654 + (index << 3));
        if (target != 0)
            G3DispatchStateEvent(target, object, sender, message, payload, frame);
        ++index;
    } while (index < 0x17);
    return 0;
}

uint32 G3Function8002F744(uint32 vector)
{
    FUNCTION_MARKER(0x8002f744, "SLUS_008.06");
    w_u16(vector + 4, 0);
    w_u16(vector + 2, 0);
    w_u16(vector, 0);
    w_u16(vector + 6, 0x1000);
    return 0x1000;
}

uint32 G3Function80013D38(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80013d38, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 sum_x = 0;
    uint32 sum_y = 0;
    uint32 sum_z = 0;
    G3FillMemory80086094(object, 0, 0x94);
    G3FillMemory80086094(0x80096994, 0, 0x1e0);
    G3Function80053F98(object, 0, 6, 0, frame);
    uint32 index = 0;
    do
    {
        uint32 point = 0x800a06e0 + (index << 3);
        uint32 random = G3SdkRandom80027FA8();
        uint32 x = (uint32)((sint32)random % 256) - 0x80;
        w_u16(point, x);
        random = G3SdkRandom80027FA8();
        uint32 y = (uint32)((sint32)random % 256) - 0x80;
        w_u16(point + 2, y);
        random = G3SdkRandom80027FA8();
        uint32 z = (uint32)((sint32)random % 256) - 0x80;
        sum_z += (uint32)(sint32)(sint16)(z & 0xffff);
        x = (uint32)(sint32)r_s16(point);
        w_u16(point + 4, z);
        y = (uint32)(sint32)r_s16(point + 2);
        sum_x += x;
        sum_y += y;
        ++index;
    } while (index < 0x0f);
    w_u16(0x800a0758, uint32(0) - sum_x);
    w_u16(0x800a075a, uint32(0) - sum_y);
    w_u16(0x800a075c, uint32(0) - sum_z);
    uint32 first = r_u32(0x8009d330);
    uint32 second = r_u32(0x8009d334);
    uint32 third = r_u32(0x8009d338);
    uint32 matrix = r_u32(0x8009d340);
    w_u32(0x800969c4, 0x44c);
    w_u32(0x800969c8, 0x640);
    w_u32(0x800969cc, 0x8ca);
    w_u32(0x800969e8, 0xfd2);
    w_u32(0x800969ec, 0xf7d);
    w_u32(0x800969f0, 0xf55);
    w_u32(object + 0x84, 0x32);
    w_u32(object + 0x88, 0x2fa8);
    uint32 other_matrix = r_u32(0x8009d33c);
    w_u32(object + 0x78, first);
    w_u32(object + 0x7c, second);
    w_u32(object + 0x80, third);
    w_u32(0x80096a60, other_matrix);
    w_u32(0x80096a5c, matrix);
    w_u16(0x80096a64, 0x1000);
    w_u16(0x80096a7a, 0x1000);
    G3Function8002F718(matrix);
    other_matrix = r_u32(0x80096a60);
    G3Function8002F718(other_matrix);
    G3Function8002F744(0x80096a6a);
    G3Function8002F744(0x80096a72);
    w_u32(0x80096a8c, 0x10);
    w_u32(0x80096994, 0x200);
    w_u32(0x80096998, 0x1000);
    uint32 first_limit = r_u16(0x800969c8);
    uint32 second_limit = r_u16(0x800969ec);
    w_u32(0x80096a9c, 1);
    w_u16(0x80096b68, first_limit);
    w_u16(0x80096b6a, second_limit);
    w_u16(object + 0x18, second_limit);
    uint32 copy = 0;
    do
    {
        uint32 values[2];
        uint32 word = 0;
        do
        {
            uint32 source = object + 0x18 + (word << 2);
            uint32 upper = r_u32((source + 3) & ~uint32(3));
            uint32 lower = r_u32(source & ~uint32(3));
            uint32 shift = (source & 3) << 3;
            values[word] = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
            ++word;
        } while (word < 2);
        word = 0;
        do
        {
            uint32 destination = object + 0x10 - (copy << 3) + (word << 2);
            uint32 value = values[word];
            uint32 high_address = destination + 3;
            uint32 high_byte = high_address & 3;
            uint32 preserve = high_byte == 3 ? 0 : uint32(0xffffffff) << ((high_byte + 1) << 3);
            uint32 previous = r_u32(high_address & ~uint32(3));
            w_u32(high_address & ~uint32(3), (previous & preserve) | (value >> ((3 - high_byte) << 3)));
            uint32 low_byte = destination & 3;
            preserve = low_byte == 0 ? 0 : uint32(0xffffffff) >> ((4 - low_byte) << 3);
            previous = r_u32(destination & ~uint32(3));
            w_u32(destination & ~uint32(3), (previous & preserve) | (value << (low_byte << 3)));
            ++word;
        } while (word < 2);
        ++copy;
    } while (copy < 2);
    w_u32(0x80096a7c, 0);
    w_u32(0x800969b4, 0x0bb8);
    w_u16(0x80096b06, 0x1000);
    w_u16(0x80096b04, 0x1000);
    w_u16(0x80096b02, 0x1000);
    G3Function8001380C(object, 0x140, frame);
    uint32 player = r_u32(0x8009efc4);
    w_u32(0x80096b34, player);
    G3Function800145D0(0x80096b34, 1, frame);
    G3Function80013864(object, 0x10, 0, frame);
    uint32 game = r_u32(0x80097048);
    uint32 enabled = G3Function80023A68(game);
    if (enabled != 0)
    {
        game = r_u32(0x80097048);
        uint32 level = r_u32(game + 0x48);
        uint32 camera = r_u32(game + 0x4c);
        uint32 scale = (uint32)(sint32)r_s16(level + 0xb0);
        G3Function80014298(camera, scale, frame);
    }
    uint32 state = r_u32(0x80096a88);
    uint32 target = r_u32(0x80099654 + (state << 3));
    if (target != 0)
    {
        game = r_u32(0x80097048);
        G3DispatchStateEvent(target, object, game, 0x10, 0, frame);
    }
    w_u32(0x80096a7c, 1);
    w_u32(0x80096b70, 0x64);
    return 0x64;
}

uint32 G3Function8001380C(uint32 object, uint32 width, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8001380c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    SetGeomScreen((sint32)width);
    w_u32(object + 0x90, width);
    G3Function80012FA4(object, frame);
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    return G3Function8002C50C(level);
}

uint32 G3Function800145D0(uint32 record, uint32 reset, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800145d0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    {
        uint32 source = record + 4;
        uint32 destination = record + 0x0a;
        uint32 upper = r_u32((source + 3) & ~uint32(3));
        uint32 lower = r_u32(source & ~uint32(3));
        uint32 shift = (source & 3) << 3;
        uint32 value = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
        uint32 tail = (uint32)(sint32)r_s16(source + 4);
        uint32 high_address = destination + 3;
        uint32 high_byte = high_address & 3;
        uint32 preserve = high_byte == 3 ? 0 : uint32(0xffffffff) << ((high_byte + 1) << 3);
        uint32 previous = r_u32(high_address & ~uint32(3));
        w_u32(high_address & ~uint32(3), (previous & preserve) | (value >> ((3 - high_byte) << 3)));
        uint32 low_byte = destination & 3;
        preserve = low_byte == 0 ? 0 : uint32(0xffffffff) >> ((4 - low_byte) << 3);
        previous = r_u32(destination & ~uint32(3));
        w_u32(destination & ~uint32(3), (previous & preserve) | (value << (low_byte << 3)));
        w_u16(destination + 4, tail);
    }
    uint32 object = r_u32(record);
    uint32 target = r_u32(object + 0xec);
    uint32 accepted = 0;
    if (target != 0)
    {
        uint32 game = r_u32(0x80097048);
        accepted = G3DispatchStateEvent(target, object, game, 0x7ed, record + 4, frame);
    }
    if (accepted == 0)
    {
        object = r_u32(record);
        {
            uint32 source = object + 0x58;
            uint32 destination = record + 4;
            uint32 upper = r_u32((source + 3) & ~uint32(3));
            uint32 lower = r_u32(source & ~uint32(3));
            uint32 shift = (source & 3) << 3;
            uint32 value = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
            uint32 tail = (uint32)(sint32)r_s16(source + 4);
            uint32 high_address = destination + 3;
            uint32 high_byte = high_address & 3;
            uint32 preserve = high_byte == 3 ? 0 : uint32(0xffffffff) << ((high_byte + 1) << 3);
            uint32 previous = r_u32(high_address & ~uint32(3));
            w_u32(high_address & ~uint32(3), (previous & preserve) | (value >> ((3 - high_byte) << 3)));
            uint32 low_byte = destination & 3;
            preserve = low_byte == 0 ? 0 : uint32(0xffffffff) >> ((4 - low_byte) << 3);
            previous = r_u32(destination & ~uint32(3));
            w_u32(destination & ~uint32(3), (previous & preserve) | (value << (low_byte << 3)));
            w_u16(destination + 4, tail);
        }
    }
    if (reset != 0)
    {
        uint32 source = record + 4;
        uint32 destination = record + 0x0a;
        uint32 upper = r_u32((source + 3) & ~uint32(3));
        uint32 lower = r_u32(source & ~uint32(3));
        uint32 shift = (source & 3) << 3;
        uint32 value = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
        uint32 tail = (uint32)(sint32)r_s16(source + 4);
        uint32 high_address = destination + 3;
        uint32 high_byte = high_address & 3;
        uint32 preserve = high_byte == 3 ? 0 : uint32(0xffffffff) << ((high_byte + 1) << 3);
        uint32 previous = r_u32(high_address & ~uint32(3));
        w_u32(high_address & ~uint32(3), (previous & preserve) | (value >> ((3 - high_byte) << 3)));
        uint32 low_byte = destination & 3;
        preserve = low_byte == 0 ? 0 : uint32(0xffffffff) >> ((4 - low_byte) << 3);
        previous = r_u32(destination & ~uint32(3));
        w_u32(destination & ~uint32(3), (previous & preserve) | (value << (low_byte << 3)));
        w_u16(destination + 4, tail);
    }
    uint32 current = r_u16(record + 4);
    uint32 previous = r_u16(record + 0x0a);
    uint32 delta_x = current - previous;
    w_u16(frame + 0x10, delta_x);
    sint32 x = (sint16)(delta_x & 0xffff);
    current = r_u16(record + 6);
    previous = r_u16(record + 0x0c);
    if (x < 0)
        x = -x;
    uint32 delta_y = current - previous;
    w_u16(frame + 0x12, delta_y);
    current = r_u16(record + 8);
    previous = r_u16(record + 0x0e);
    uint32 delta_z = current - previous;
    w_u16(frame + 0x14, delta_z);
    uint32 moved = 1;
    if (x < 3)
    {
        sint32 y = (sint16)(delta_y & 0xffff);
        if (y < 0)
            y = -y;
        if (y < 3)
        {
            sint32 z = (sint16)(delta_z & 0xffff);
            if (z < 0)
                z = -z;
            if (z < 3)
                moved = 0;
        }
    }
    w_u32(record + 0x10, moved);
    return 1;
}

uint32 G3Function80023A68(uint32 game)
{
    FUNCTION_MARKER(0x80023a68, "SLUS_008.06");
    uint32 level = r_u16(game + 0x68b4);
    if (level - 0x17 >= 4)
        return 0;
    sint32 first = r_s16(game + 0x67f0);
    if (first == 0)
        return 0;
    sint32 second = r_s16(game + 0x67f2);
    return second != 0 ? 1 : 0;
}

uint32 G3Function8002C50C(uint32 level)
{
    FUNCTION_MARKER(0x8002c50c, "SLUS_008.06");
    uint32 near_distance = r_u16(level + 0x4c);
    uint32 far_distance = r_u16(level + 0x4e);
    uint32 table = r_u32(level + 0x54);
    if (near_distance == far_distance)
        return far_distance;
    uint32 numerator = near_distance * far_distance;
    uint32 denominator = near_distance - far_distance;
    uint32 quotient = denominator == 0 ? ((sint32)numerator < 0 ? 1 : uint32(0xffffffff)) : (uint32)((sint64)(sint32)numerator / (sint32)denominator);
    uint32 game = r_u32(0x80097048);
    uint32 camera = r_u32(game + 0x4c);
    uint32 width = r_u32(camera + 0x90);
    uint32 limit = (uint32)((sint32)((width << 15) - width) >> 8);
    uint32 depth = 0 - quotient;
    uint32 negative_limit = 0 - limit;
    if ((sint32)limit < (sint32)depth || (sint32)depth < (sint32)negative_limit)
    {
        if ((sint32)depth < (sint32)negative_limit)
            limit = negative_limit;
        numerator = limit * near_distance;
        denominator = limit - near_distance;
        quotient = denominator == 0 ? ((sint32)numerator < 0 ? 1 : uint32(0xffffffff)) : (uint32)((sint64)(sint32)numerator / (sint32)denominator);
        w_u16(level + 0x4e, quotient);
        near_distance = r_u16(level + 0x4c);
        far_distance = r_u16(level + 0x4e);
        numerator = near_distance * far_distance;
        denominator = near_distance - far_distance;
        quotient = denominator == 0 ? ((sint32)numerator < 0 ? 1 : uint32(0xffffffff)) : (uint32)((sint64)(sint32)numerator / (sint32)denominator);
        depth = 0 - quotient;
    }
    far_distance = r_u16(level + 0x4e);
    near_distance = r_u16(level + 0x4c);
    game = r_u32(0x80097048);
    numerator = near_distance << 12;
    denominator = near_distance - far_distance;
    w_u32(game + 0x0c, far_distance);
    sint32 override_distance = r_s16(level + 0x50);
    uint32 scale = denominator == 0 ? ((sint32)numerator < 0 ? 1 : uint32(0xffffffff)) : (uint32)((sint64)(sint32)numerator / (sint32)denominator);
    if (override_distance == -1)
    {
        uint32 value = r_u32(level + 0x50);
        w_u32(game + 8, value);
    }
    else
        w_u32(game + 8, far_distance);
    depth <<= 12;
    uint32 length = r_u32(level + 0x100);
    uint32 shift = r_u32(level + 0x104);
    uint32 end = (length << (shift & 31)) + 0x620;
    uint32 distance = 1;
    w_u16(table, 0);
    if ((sint32)distance < (sint32)end)
    {
        do
        {
            quotient = distance == 0 ? ((sint32)depth < 0 ? 1 : uint32(0xffffffff)) : (uint32)((sint64)(sint32)depth / (sint32)distance);
            uint32 value = scale + quotient;
            if ((sint32)value >= 0x1001)
                value = 0x1000;
            if ((sint32)value < 0)
                value = 0;
            shift = r_u32(level + 0x104);
            uint32 index = (uint32)((sint32)distance >> (shift & 31));
            w_u16(table + (index << 1), value);
            shift = r_u32(level + 0x104);
            distance += uint32(1) << (shift & 31);
        } while ((sint32)distance < (sint32)end);
    }
    game = r_u32(0x80097048);
    camera = r_u32(game + 0x4c);
    width = r_u32(camera + 0x90);
    quotient = width == 0 ? ((sint32)depth < 0 ? 1 : uint32(0xffffffff)) : (uint32)((sint64)(sint32)depth / (sint32)width);
    gte_SetDQA((sint32)quotient >> 4);
    gte_SetDQB((sint32)(scale << 12));
    return camera;
}

uint32 G3Function80012FA4(uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80012fa4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 width = r_u16(object + 0x90);
    w_u16(frame, 0xff60);
    w_u16(frame + 0x10, 0xff60);
    w_u16(frame + 8, 0xa0);
    w_u16(frame + 0x18, 0xa0);
    w_u16(frame + 2, 0xff88);
    w_u16(frame + 0x0a, 0xff88);
    w_u16(frame + 0x12, 0x78);
    w_u16(frame + 0x1a, 0x78);
    uint32 product = 240 * (uint32)(sint32)(sint16)width;
    w_u16(0x80096a14, 0);
    w_u16(0x80096a16, 0);
    w_u16(0x80096a1e, 0);
    w_u16(0x80096a20, 9);
    w_u16(0x80096a18, 0x1000);
    w_u16(frame + 4, width);
    w_u16(frame + 0x0c, width);
    w_u16(frame + 0x14, width);
    w_u16(frame + 0x1c, width);
    sint32 component = (sint32)product >> 12;
    sint32 magnitude = component < 0 ? -component : component;
    w_u16(0x80096a1c, (uint32)component);
    sint32 maximum = magnitude > 9 ? magnitude : 9;
    sint32 middle = magnitude > 9 ? 9 : magnitude;
    sint32 divisor = (30 * maximum + 12 * middle) / 8;
    if (divisor != 0)
    {
        sint32 x = r_s16(0x80096a1c);
        uint32 normalized_x = (uint32)(x * 0x4000 / divisor);
        sint32 y = r_s16(0x80096a1e);
        uint32 normalized_y = (uint32)(y * 0x4000 / divisor);
        sint32 z = r_s16(0x80096a20);
        uint32 normalized_z = (uint32)(z * 0x4000 / divisor);
        w_u16(0x80096a1c, normalized_x);
        w_u16(0x80096a1e, normalized_y);
        w_u16(0x80096a20, normalized_z);
    }
    uint32 result = 0;
    for (uint32 plane = 0; plane < 3; ++plane)
    {
        uint32 first = plane == 0 ? frame + 8 : plane == 1 ? frame : frame + 0x18;
        uint32 second = plane == 0 ? frame + 0x18 : plane == 1 ? frame + 8 : frame + 0x10;
        uint32 output = 0x80096a24 + plane * 8;
        uint32 first_y_bits = plane == 1 ? r_u16(first + 2) : (uint32)(sint32)r_s16(first + 2);
        sint32 second_z = r_s16(second + 4);
        sint32 first_y = (sint16)(first_y_bits & 0xffff);
        uint32 first_product = (uint32)first_y * (uint32)second_z;
        uint32 first_z_bits = plane == 1 ? r_u16(first + 4) : (uint32)(sint32)r_s16(first + 4);
        sint32 first_z = (sint16)(first_z_bits & 0xffff);
        sint32 second_y = r_s16(second + 2);
        uint32 second_product = (uint32)first_z * (uint32)second_y;
        sint32 first_x = r_s16(first);
        uint32 third_product = (uint32)first_x * (uint32)second_z;
        sint32 second_x = r_s16(second);
        uint32 fourth_product = (uint32)first_z * (uint32)second_x;
        uint32 fifth_product = (uint32)first_x * (uint32)second_y;
        uint32 x_bits = (uint32)((sint32)(first_product - second_product) >> 12);
        uint32 sixth_product = (uint32)first_y * (uint32)second_x;
        sint32 absolute_x = (sint16)(x_bits & 0xffff);
        if (absolute_x < 0)
            absolute_x = -absolute_x;
        uint32 y_bits = 0 - (uint32)((sint32)(third_product - fourth_product) >> 12);
        sint32 absolute_y = (sint16)(y_bits & 0xffff);
        if (absolute_y < 0)
            absolute_y = -absolute_y;
        w_u16(output, x_bits);
        w_u16(output + 2, y_bits);
        uint32 z_bits = (uint32)((sint32)(fifth_product - sixth_product) >> 12);
        sint32 absolute_z = (sint16)(z_bits & 0xffff);
        if (absolute_z < 0)
            absolute_z = -absolute_z;
        sint32 minimum = (sint16)absolute_x;
        middle = (sint16)absolute_y;
        maximum = (sint16)absolute_z;
        w_u16(output + 4, z_bits);
        if (minimum > middle)
        {
            sint32 temporary = minimum;
            minimum = middle;
            middle = temporary;
        }
        if (middle > maximum)
        {
            sint32 temporary = middle;
            middle = maximum;
            maximum = temporary;
        }
        if (minimum > middle)
        {
            sint32 temporary = minimum;
            minimum = middle;
            middle = temporary;
        }
        result = (uint32)(9 * minimum);
        divisor = (30 * maximum + 12 * middle + (sint32)result) / 8;
        if (divisor != 0)
        {
            sint32 x = r_s16(output);
            uint32 normalized_x = (uint32)(x * 0x4000 / divisor);
            sint32 y = r_s16(output + 2);
            uint32 normalized_y = (uint32)(y * 0x4000 / divisor);
            sint32 z = r_s16(output + 4);
            result = (uint32)(z * 0x4000 / divisor);
            w_u16(output, normalized_x);
            w_u16(output + 2, normalized_y);
            w_u16(output + 4, result);
        }
    }
    return result;
}

uint32 G3Function8001471C(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8001471c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    sint32 mode = r_s16(game + 0x6786);
    uint32 camera = r_u32(game + 0x4c);
    if (mode == 4)
        return 4;
    G3Function800145D0(0x80096b34, 0, frame);
    uint32 coordinate = r_u16(camera);
    w_u16(0x80096a54, coordinate);
    coordinate = r_u16(camera + 2);
    w_u16(0x80096a56, coordinate);
    coordinate = r_u16(camera + 4);
    w_u16(0x80096a58, coordinate);
    uint32 state = r_u32(0x80096a88);
    uint32 target = r_u32(0x80099654 + (state << 3));
    if (target != 0)
    {
        uint32 current_game = r_u32(0x80097048);
        G3DispatchStateEvent(target, camera, current_game, 1, 0, frame);
    }
    uint32 count = r_u32(0x80096af0);
    if ((sint32)count > 0)
    {
        for (uint32 component = 0; component < 3; ++component)
        {
            if (component != 0)
                count = r_u32(0x80096af0);
            sint32 index = (sint32)count % 16;
            sint32 displacement = r_s16(0x800a06e0 + (uint32)index * 8 + component * 2);
            uint32 amplitude = r_u32(0x80096af4);
            uint32 product = (uint32)displacement * amplitude;
            coordinate = r_u16(camera + component * 2);
            uint32 delta = (uint32)((sint32)product >> 12);
            w_u16(camera + component * 2, coordinate + delta);
        }
        count = r_u32(0x80096af0);
        w_u32(0x80096af0, count - 1);
    }
    uint32 timer = r_u32(0x80096b70);
    w_u32(0x80096b50, 0);
    w_u32(0x80096b4c, 0);
    w_u32(0x80096b54, 0);
    uint32 result = timer - 1;
    if (timer != 0)
        w_u32(0x80096b70, result);
    return result;
}

uint32 G3Function80038F6C(uint32 name, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80038f6c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 index = 0;
    for (;;)
    {
        uint32 length = G3StringLength8008929C(name);
        if (index >= length)
            break;
        uint32 character = r_u8(name + index);
        uint32 converted = G3Lowercase800894BC(character);
        uint32 destination = frame + 0x10 + index;
        ++index;
        w_u8(destination, converted);
    }
    w_u8(frame + 0x10 + index, 0);
    uint32 entry = 0x80099708;
    index = 0;
    do
    {
        uint32 candidate = r_u32(entry);
        uint32 comparison = G3CompareString80085EC4(frame + 0x10, candidate);
        if (comparison == 0)
            return index;
        ++index;
        entry += 0x1c;
    } while (index < 0x1c);
    return 0xffffffff;
}

uint32 G3Function80020A04(void)
{
    FUNCTION_MARKER(0x80020a04, "SLUS_008.06");
    w_u32(0x800a3c54, 10);
    w_u32(0x800a3c58, 16);
    w_u32(0x800a3c5c, 40);
    w_u32(0x800a3c60, 30);
    w_u32(0x800a3c64, 0);
    w_u32(0x800a3c68, 0);
    w_u16(0x800a3c76, 0);
    w_u16(0x800a3c78, 0);
    w_u16(0x800a3c7a, 0);
    return 0x800a2c44;
}

uint32 G3Function800351EC(uint32 descriptor, uint32 data, uint32 base)
{
    FUNCTION_MARKER(0x800351ec, "SLUS_008.06");
    uint32 cursor = r_u32(descriptor + 4);
    uint32 count = r_u32(descriptor);
    uint32 end = cursor + (count << 2);
    if (cursor < end)
    {
        do
        {
            uint32 offset = r_u32(cursor);
            if ((sint32)offset < 0)
                offset += 3;
            uint32 aligned = (uint32)((sint32)offset >> 2) << 2;
            uint32 address = data + aligned;
            uint32 value = r_u32(address);
            cursor += 4;
            w_u32(address, value + base);
        } while (cursor < end);
    }
    return 0;
}

uint32 G3LoadObject8002C760(uint32 filename, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002c760, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 file = G3LoadFile80064A7C(filename, frame);
    w_u32(frame + 0x14, file + 4);
    uint32 count = r_u32(file);
    uint32 rounded = count + 0x200;
    w_u32(frame + 0x10, count);
    if ((sint32)rounded < 0)
        rounded = count + 0x3ff;
    uint32 sectors = (uint32)((sint32)rounded >> 9);
    uint32 words = sectors << 9;
    uint32 bytes = sectors << 11;
    G3Function800351EC(frame + 0x10, file + bytes, file);
    uint32 size = r_u32(0x800a2a64);
    uint32 end = file + (((uint32)((sint32)size >> 2) - words) << 2);
    uint32 cursor = file;
    if (cursor < end)
    {
        do
        {
            uint32 value = r_u32(cursor + bytes);
            w_u32(cursor, value);
            cursor += 4;
        } while (cursor < end);
    }
    G3MemPackRelease80048248(words << 2);
    return file;
}

uint32 G3Function80028498(void)
{
    FUNCTION_MARKER(0x80028498, "SLUS_008.06");
    PadInitDirectPSX(0x800a0cbc, 0x800a0d04);
    G3Function80081C58();
    PadStartCom();
    G3FillMemory80086094(0x800a0d28, 0, 0x10);
    G3FillMemory80086094(0x800a0cbc, 0, 0x22);
    G3FillMemory80086094(0x800a0d04, 0, 0x22);
    w_u16(0x800a0c9a, 0xffff);
    w_u8(0x800a0c98, 0);
    w_u16(0x800a0ce2, 0xffff);
    w_u8(0x800a0ce0, 0);
    return 0xffff;
}

uint32 G3Function80035348(void)
{
    FUNCTION_MARKER(0x80035348, "SLUS_008.06");
    w_u32(0x800ad074, 0);
    G3Function80049934(0x80035250);
    SpuSetTransferMode(0);
    w_u32(0x800aa7d8, 0xffffffff);
    w_u32(0x800aa7dc, 0xffffffff);
    w_u32(0x800aa7e0, 0xffffffff);
    w_u8(0x800aa7d1, 0);
    w_u8(0x800aa7d0, 0);
    uint32 result = G3Function800356E4();
    w_u16(0x800ad0c0, 0xffff);
    w_u16(0x800ad0c4, 0xffff);
    return result;
}

uint32 G3Function800356E4(void)
{
    FUNCTION_MARKER(0x800356e4, "SLUS_008.06");
    w_u32(0x800acfec, 450);
    w_u32(0x800aa7d4, 0);
    w_u32(0x800ad008, 0);
    w_u32(0x800ad004, 0);
    w_u32(0x800ad010, 0);
    w_u32(0x800ad00c, 0);
    w_u32(0x800acff0, 0);
    w_u8(0x800aa7d2, 0);
    w_u32(0x800aa7d8, 0xffffffff);
    uint32 node = 0x800ad014;
    for (uint32 index = 0; index < 8; ++index)
    {
        w_u32(node + 4, 0);
        w_u32(node, 0);
        G3Function80045B74(0x800ad004, node);
        node += 12;
    }
    return 0;
}

uint32 G3Function80045B74(uint32 anchor, uint32 node)
{
    FUNCTION_MARKER(0x80045b74, "SLUS_008.06");
    w_u32(node, anchor);
    uint32 next = r_u32(anchor + 4);
    w_u32(node + 4, next);
    next = r_u32(anchor + 4);
    if (next != 0)
        w_u32(next, node);
    w_u32(anchor + 4, node);
    return next;
}

uint32 G3Function80035250(uint32 request)
{
    FUNCTION_MARKER(0x80035250, "SLUS_008.06");
    w_u32(0x800ad074, request);
    return 0x800b0000;
}

uint32 G3Function8002E59C(uint32 filename, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002e59c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    G3MemPackPush8004829C();
    uint32 image = G3LoadFile80064A7C(filename, frame);
    VSync(0);
    if (image != 0)
    {
        w_u32(frame + 0x10, 0x100);
        G3Function80065770(image, 0, 0, 0x200, frame);
        w_u32(frame + 0x10, 0x100);
        G3Function80065770(image, 0, 0xf0, 0x200, frame);
    }
    return G3MemPackPop80048320();
}

uint32 G3Function80065770(uint32 image, uint32 x, uint32 y, uint32 width, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80065770, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 height = r_u32(guest_stack + 0x10);
    w_u16(frame + 0x10, x);
    w_u16(frame + 0x12, y);
    w_u16(frame + 0x14, width);
    w_u16(frame + 0x16, height);
    (uint32)DrawSync(0);
    G3LoadImage800865C0(frame + 0x10, image + 0x14);
    return (uint32)DrawSync(0);
}

uint32 G3Function80028544(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80028544, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x68;
    w_u8(game + 0x68f8, 0);
    game = r_u32(0x80097048);
    uint32 pending = r_u8(game + 0x68f9);
    if (pending == 0)
        return pending;
    sprintfPSX(frame + 0x10, 0x8009d83c, game + 0x66e0);
    uint32 data = G3LoadFile80064A7C(frame + 0x10, frame);
    game = r_u32(0x80097048);
    w_u32(0x8009d830, data);
    w_u32(0x8009d834, data);
    w_u16(0x8009d82c, 0xffff);
    w_u16(0x8009d82e, 0);
    w_u8(game + 0x68f8, 1);
    game = r_u32(0x80097048);
    w_u8(game + 0x68f9, 0);
    return G3Function80027168(game);
}

uint32 G3Function80027168(uint32 incoming_v0)
{
    FUNCTION_MARKER(0x80027168, "SLUS_008.06");
    uint32 owner = r_u32(0x8009efc4);
    if (owner == 0)
        return incoming_v0;
    uint32 object = r_u32(owner + 0x1c);
    uint32 attributes = r_u32(object + 0x1c);
    uint32 game = r_u32(0x80097048);
    uint32 value = (uint32)(sint32)r_s16(attributes + 0x1a);
    w_u32(game + 0x6818, value);
    object = r_u32(owner + 0x1c);
    attributes = r_u32(object + 0x1c);
    value = r_u16(attributes + 0x18);
    w_u16(game + 0x681c, value);
    return value;
}

uint32 G3Function80065674(uint32 filename, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80065674, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 data = G3LoadFile80064A7C(filename, frame);
    w_u32(frame + 0x10, 0);
    return G3Function800656AC(data, 0x200, 0, 0, frame);
}

uint32 G3Function800656AC(uint32 data, uint32 bank, uint32 unused_a2, uint32 palette_x, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800656ac, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 block = data + 8;
    uint32 flags = r_u32(block - 4);
    if (flags == 8)
    {
        uint32 palette_y = r_u32(guest_stack + 0x10);
        w_u16(frame + 0x10, palette_x);
        w_u16(frame + 0x12, palette_y);
        w_u16(frame + 0x14, 16);
        w_u16(frame + 0x16, 1);
        (uint32)DrawSync(0);
        G3LoadImage800865C0(frame + 0x10, block + 12);
        block += 0x2c;
    }
    uint32 x = r_u16(block + 4);
    x = (x & 0x1ff) + 0x200;
    w_u16(frame + 0x10, x);
    if (bank == 0)
        w_u16(frame + 0x10, 0);
    uint32 y = r_u16(block + 6);
    w_u16(frame + 0x12, y);
    uint32 width = r_u16(block + 8);
    w_u16(frame + 0x14, width);
    uint32 height = r_u16(block + 10);
    w_u16(frame + 0x16, height);
    (uint32)DrawSync(0);
    G3LoadImage800865C0(frame + 0x10, block + 12);
    return (uint32)DrawSync(0);
}

uint32 G3Function8003A114(uint32 handle_pointer, uint32 incoming_v0)
{
    FUNCTION_MARKER(0x8003a114, "SLUS_008.06");
    uint32 handle = r_u32(handle_pointer);
    if (handle == 0)
        return incoming_v0;
    uint32 result = G3Function8003F18C(handle);
    w_u32(handle_pointer, 0);
    return result;
}

uint32 G3Function8003F18C(uint32 handle)
{
    FUNCTION_MARKER(0x8003f18c, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 enabled = r_u8(game + 0x67df);
    if (enabled != 0)
        return G3Function8004BBD8(handle);
    return enabled;
}

uint32 G3Function80046AD0(void)
{
    FUNCTION_MARKER(0x80046ad0, "SLUS_008.06");
    uint32 record = 0x800a218c;
    for (uint32 index = 0; index < 12; ++index)
    {
        w_u16(record + 4, 4);
        w_u16(record, 0);
        record += 16;
    }
    return 0;
}

void G3Function8002C704(uint32 object, uint32 filename, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002c704, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 archive_entry = G3FindArchiveFile800654AC(filename);
    uint32 module = G3LoadObject8002C760(filename, frame);
    w_u32(object + 0xec, module);
    uint32 depth = r_u32(0x800a2960);
    G3LevelCodeRegister(archive_entry, module, r_u32(0x800a2964 + (depth << 3)), depth);
    uint32 bytes = G3Function80039980(module);
    G3MemPackRelease80048248(bytes);
    (uint32)DrawSync(0);
    G3EnterCritical8008A05C();
    FlushCache();
    G3ExitCritical8008A03C();
}

uint32 G3Function80039980(uint32 module)
{
    FUNCTION_MARKER(0x80039980, "SLUS_008.06");
    uint32 descriptor = module;
    uint32 marker = r_u32(module + 12);
    if (marker != 0)
    {
        descriptor += 24;
        do
        {
            marker = r_u32(descriptor + 12);
            descriptor += 24;
        } while (marker != 0);
        descriptor -= 24;
    }
    uint32 cursor = r_u32(descriptor + 20);
    uint32 start = cursor;
    uint32 index = 0;
    while (cursor != 0)
    {
        uint32 bytes = r_u32(cursor);
        if (bytes == 0)
            break;
        uint32 signed_index = (uint32)(sint32)(sint16)(index & 0xffff);
        uint32 offset = ((signed_index << 1) + signed_index) << 3;
        uint32 base = r_u32(module + offset + 12);
        G3RelocateModule800398BC(cursor + 4, base);
        bytes = r_u32(cursor);
        cursor += bytes + 4;
        ++index;
    }
    return cursor - start;
}

void G3Function8001946C(uint32 zsf3, uint32 zsf4)
{
    FUNCTION_MARKER(0x8001946c, "SLUS_008.06");
    xport_gte_write_control(29, zsf3);
    xport_gte_write_control(30, zsf4);
}

uint32 G3Function80030414(void)
{
    FUNCTION_MARKER(0x80030414, "SLUS_008.06");
    G3Function8007633C(0x800a1550, 128, 68);
    return G3Function8007633C(0x800a1560, 96, 20);
}

uint32 G3Function8007633C(uint32 pool, uint32 count, uint32 element_bytes)
{
    FUNCTION_MARKER(0x8007633c, "SLUS_008.06");
    uint32 storage = G3MemPackAllocate80048190(count * element_bytes);
    w_u32(pool + 12, storage);
    uint32 indices = G3MemPackAllocate80048190(count << 1);
    w_u32(pool + 8, indices);
    w_u16(pool, element_bytes);
    w_u16(pool + 4, count);
    return G3Function800763A4(pool);
}

uint32 G3Function800763A4(uint32 pool)
{
    FUNCTION_MARKER(0x800763a4, "SLUS_008.06");
    uint32 count = r_u16(pool + 4);
    w_u16(pool + 2, 0);
    uint32 index = 0;
    if (count != 0)
    {
        do
        {
            uint32 indices = r_u32(pool + 8);
            w_u16(indices + (index << 1), index);
            count = r_u16(pool + 4);
            ++index;
        } while ((sint32)index < (sint32)count);
    }
    return 0;
}

uint32 G3Function80023850(uint32 game, uint32 level)
{
    FUNCTION_MARKER(0x80023850, "SLUS_008.06");
    uint32 allocation = G3MemPackAllocate80048190(0x40c);
    w_u32(0x800a0c2c, allocation);
    allocation = G3MemPackAllocate80048190(0x7648);
    w_u32(0x800a0c28, allocation);
    allocation = G3MemPackAllocate80048190(0x3dc);
    w_u32(0x800a0c78, allocation);
    allocation = G3MemPackAllocate80048190(8);
    w_u32(0x800a0c20, allocation);
    allocation = G3MemPackAllocate80048190(0x11c20);
    w_u32(0x800a27fc, allocation);
    w_u32(0x800a2a68, allocation);
    uint32 count = 500;
    G3Function800752B8();
    allocation = G3MemPackAllocate80048190(0x804);
    uint32 length = r_u32(level + 0x100);
    w_u32(0x800a0c24, allocation);
    w_u32(game + 0x28, length);
    uint32 bytes = (length + 0x80) << 3;
    uint32 shift = r_u32(level + 0x104);
    w_u32(game + 0x2c, shift);
    allocation = G3MemPackAllocate80048190(bytes);
    w_u32(game + 0x3c, allocation);
    w_u32(game + 0x38, allocation + 0x200);
    shift = r_u32(level + 0x104);
    length = r_u32(game + 0x28);
    bytes = (length + (uint32)(sint32(0x620) >> (shift & 31))) << 1;
    allocation = G3MemPackAllocate80048190(bytes);
    w_u32(level + 0x54, allocation);
    uint32 current_level = r_u32(game + 0x48);
    uint32 entries = r_u32(current_level + 0xe4);
    allocation = G3MemPackAllocate80048190(((entries << 1) + entries) << 1);
    w_u32(game + 0x68ac, allocation);
    allocation = G3MemPackAllocate80048190(100);
    w_u32(game + 0x68b0, allocation);
    uint32 version = r_u32(level + 8);
    if (version == 2)
        count = r_u32(level + 0x10c);
    allocation = G3MemPackAllocate80048190(((count << 1) + count) << 2);
    w_u32(0x800a2188, allocation);
    allocation = G3MemPackAllocate80048190(count << 3);
    uint32 depth = r_u32(0x800a2960);
    uint32 remaining = r_u32(0x800a2968 + (depth << 3));
    w_u32(0x800a0c20, allocation);
    uint32 half = (uint32)(((sint32)remaining) / 2);
    uint32 slots = half - 12;
    if ((sint32)slots < 0)
        slots = half - 9;
    uint32 pool_bytes = (uint32)((sint32)remaining >> 1);
    pool_bytes &= 0xfffffffc;
    slots = (uint32)((sint32)slots >> 2);
    allocation = G3MemPackAllocate80048190(pool_bytes);
    w_u32(0x800a0c7c, allocation);
    allocation = G3MemPackAllocate80048190(pool_bytes);
    w_u32(0x800a0c80, allocation);
    w_u32(game + 0x6c, slots);
    {
#ifndef NDEBUG
    printf("pmem=%d\n", (sint32)remaining >> 1);
#endif
    }
    count = r_u32(game + 0x6c);
    uint32 pool = r_u32(0x800a0c7c);
    w_u32(pool + 8, pool + (count << 2) + 12);
    pool = r_u32(0x800a0c7c);
    w_u32(pool + 4, pool + 12);
    count = r_u32(game + 0x6c);
    pool = r_u32(0x800a0c80);
    w_u32(pool + 8, pool + (count << 2) + 12);
    pool = r_u32(0x800a0c80);
    uint32 result = pool + 12;
    w_u32(pool + 4, result);
    return result;
}

uint32 G3Function800752B8(void)
{
    FUNCTION_MARKER(0x800752b8, "SLUS_008.06");
    uint32 storage = G3MemPackAllocate80048190(0x1100);
    w_u32(0x800a02e4, storage);
    return storage;
}

uint32 G3Function800752D8(void)
{
    FUNCTION_MARKER(0x800752d8, "SLUS_008.06");
    uint32 storage = r_u32(0x800a02e4);
    G3Function80075264(storage, 24, 128);
    storage = r_u32(0x800a02e4);
    return G3Function80075264(storage + 0xc10, 52, 24);
}

uint32 G3Function80075264(uint32 pool, uint32 stride, uint32 count)
{
    FUNCTION_MARKER(0x80075264, "SLUS_008.06");
    uint32 result = pool + 16;
    w_u32(pool, result);
    uint32 cursor = result;
    uint32 index = 0;
    w_u32(pool + 12, count);
    uint32 limit = count - 1;
    w_u32(pool + 4, 0);
    w_u32(pool + 8, stride);
    if ((sint32)limit > 0)
    {
        do
        {
            uint32 current_stride = r_u32(pool + 8);
            ++index;
            uint32 next = cursor + current_stride;
            w_u32(cursor, next);
            uint32 current_count = r_u32(pool + 12);
            result = (sint32)index < (sint32)(current_count - 1) ? 1 : 0;
            cursor = next;
        } while (result != 0);
    }
    w_u32(cursor, 0);
    return result;
}

uint32 G3Function800414B0(uint32 storage)
{
    FUNCTION_MARKER(0x800414b0, "SLUS_008.06");
    w_u32(storage + 0x2c04, 0);
    w_u32(storage + 0x2c00, 0);
    w_u32(storage + 0x2c0c, 0);
    w_u32(storage + 0x2c08, 0);
    w_u32(storage + 0x11c14, 0);
    w_u32(storage + 0x11c10, 0);
    w_u32(storage + 0x11c1c, 0);
    w_u32(storage + 0x11c18, 0);
    uint32 end = storage + 0x2c00;
    uint32 cursor = storage;
    if (cursor < end)
    {
        do
        {
            G3Function80045B74(storage + 0x2c08, cursor);
            cursor += 0x2c;
        } while (cursor < end);
    }
    cursor = storage + 0x2c10;
    end = storage + 0x11c10;
    if (cursor < end)
    {
        do
        {
            G3Function80045B74(storage + 0x11c18, cursor);
            cursor += 0x78;
        } while (cursor < end);
    }
    return 0;
}

uint32 G3Function80029010(uint32 state, uint32 records)
{
    FUNCTION_MARKER(0x80029010, "SLUS_008.06");
    w_u32(records, 88);
    uint32 previous_offset = 4;
    uint32 next_offset = 0x2b4;
    uint32 record = records + 0x158;
    for (uint32 index = 1; index < 87; ++index)
    {
        uint32 previous = records + previous_offset;
        previous_offset += 0x158;
        uint32 next = records + next_offset;
        next_offset += 0x158;
        w_u32(record + 12, next);
        w_u32(record + 16, previous);
        record += 0x158;
    }
    w_u32(records + 12, records + 0x15c);
    w_u32(records + 0x74f8, records + 0x7394);
    w_u32(records + 16, 0);
    w_u32(records + 0x74f4, 0);
    w_u32(records + 0x7644, records + 4);
    w_u32(state + 8, records);
    w_u32(state, 0);
    w_u32(state + 4, 0);
    for (uint32 index = 0; index < 128; ++index)
    {
        w_u32(state + 16, 0);
        w_u32(state + 12, 0);
        state += 8;
    }
    return 0;
}

uint32 G3Function80039A3C(uint32 level)
{
    FUNCTION_MARKER(0x80039a3c, "SLUS_008.06");
    uint32 records = r_u32(level + 0xec);
    G3Function80039884(records);
    uint32 count = G3Function80039884(0x800994a0);
    uint32 index = (uint32)(sint32)(sint16)(count & 0xffff) - 1;
    uint32 offset = ((index << 1) + index) << 3;
    records = r_u32(level + 0xec);
    w_u32(0x800994a0 + offset + 20, records);
    w_u32(level + 0xec, 0x800994a0);
    return records;
}

uint32 G3Function80039884(uint32 records)
{
    FUNCTION_MARKER(0x80039884, "SLUS_008.06");
    uint32 marker = r_u32(records + 12);
    uint32 count = 0;
    if (marker != 0)
    {
        do
        {
            uint32 next = records + 24;
            w_u32(records + 20, next);
            records = next;
            marker = r_u32(records + 12);
            ++count;
        } while (marker != 0);
    }
    return (uint32)(sint32)(sint16)(count & 0xffff);
}

uint32 G3Function8003ACC8(uint32 level, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003acc8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function8003AC34();
    G3Function8003AADC(level, frame);
    return G3Function8003AC68(level);
}

uint32 G3Function8003AC34(void)
{
    FUNCTION_MARKER(0x8003ac34, "SLUS_008.06");
    uint32 record = 0x800986e8;
    uint32 name = r_u32(record);
    while (name != 0)
    {
        w_u32(record + 4, 0);
        record += 8;
        name = r_u32(record);
    }
    return name;
}

uint32 G3Function8003AADC(uint32 level, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003aadc, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 name = r_u32(level + 0x90);
    uint32 character = r_u8(name);
    uint32 cursor = r_u32(level + 0x3c);
    uint32 offset = 0;
    if (character != 0x5f)
    {
        uint32 address = name;
        do
        {
            character = r_u8(address);
            w_u8(address, character | 0x20);
            name = r_u32(level + 0x90);
            ++offset;
            address = name + offset;
            character = r_u8(address);
        } while (character != 0x5f);
    }
    uint32 table = 0x800986e8;
    uint32 marker = r_u32(table);
    if (marker != 0)
    {
        do
        {
            name = r_u32(table);
            uint32 resource = G3Function8003AA48(name);
            w_u32(table + 4, resource);
            table += 8;
            marker = r_u32(table);
        } while (marker != 0);
    }
    uint32 icon = r_u32(0x800987bc);
    if (icon == 0)
    {
        icon = G3Function8003AA48(0x8009dd50);
        w_u32(0x800987bc, icon);
        if (icon == 0)
        {
            icon = G3Function8003AA48(0x8009dd5c);
            w_u32(0x800987bc, icon);
            if (icon == 0)
            {
                icon = G3Function8003AA48(0x8009dd68);
                w_u32(0x800987bc, icon);
            }
        }
    }
    for (;;)
    {
        uint32 resource = r_u32(cursor);
        uint32 sentinel = r_u32(level + 0x3c);
        if (resource == sentinel)
            return sentinel;
        resource = r_u32(cursor);
        if (resource != 0)
        {
            name = r_u32(resource + 0x20);
            uint32 target = G3Function80039B78(name);
            w_u32(resource + 0x34, target);
            if (target != 0)
            {
                uint32 game = r_u32(0x80097048);
                G3DispatchStateEvent(target, 0, game, 15, resource, frame);
            }
        }
        cursor += 4;
    }
}

uint32 G3Function8003AC68(uint32 level)
{
    FUNCTION_MARKER(0x8003ac68, "SLUS_008.06");
    uint32 cursor = r_u32(level + 0x3c);
    uint32 result = r_u32(cursor);
    if (result == cursor)
        return result;
    uint32 resource = r_u32(cursor);
    do
    {
        if (resource != 0)
        {
            uint32 flags = r_u32(resource);
            if ((flags & 0x40000000) != 0)
                w_u32(resource, flags | 2);
        }
        cursor += 4;
        resource = r_u32(cursor);
        result = r_u32(level + 0x3c);
    } while (resource != result);
    return result;
}

uint32 G3Function8003AA48(uint32 name)
{
    FUNCTION_MARKER(0x8003aa48, "SLUS_008.06");
    if (name == 0)
        return 0;
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    uint32 cursor = r_u32(level + 0x3c);
    uint32 sentinel = cursor;
    uint32 resource = r_u32(cursor);
    while (resource != sentinel)
    {
        resource = r_u32(cursor);
        if (resource != 0)
        {
            uint32 candidate = r_u32(resource + 0x24);
            uint32 first = r_u32(name);
            uint32 second = r_u32(candidate);
            if (first == second)
            {
                first = r_u32(name + 4);
                second = r_u32(candidate + 4);
                cursor += 4;
                if (first == second)
                    return resource;
            }
            else
                cursor += 4;
        }
        else
            cursor += 4;
        resource = r_u32(cursor);
    }
    return 0;
}

uint32 G3Function80039B78(uint32 name)
{
    FUNCTION_MARKER(0x80039b78, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    uint32 record = r_u32(level + 0xec);
    while (record != 0)
    {
        uint32 target = r_u32(record + 12);
        if (target == 0)
            return 0;
        uint32 first = r_u32(record);
        uint32 second = r_u32(name);
        if (first == second)
        {
            first = r_u32(record + 4);
            second = r_u32(name + 4);
            if (first == second)
                return target;
        }
        record = r_u32(record + 20);
    }
    return 0;
}

uint32 G3Function800647A4(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800647a4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (message != 3)
        return 0;
    G3Function800647E4(object, game, frame);
    return 1;
}

uint32 G3Function800647E4(uint32 object, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800647e4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 parameters = r_u32(object + 0xb0);
    uint32 player = r_u32(game + 0x50);
    uint32 kind = r_u32(parameters + 4);
    uint32 player_data = r_u32(player + 0x24);
    if (kind != 0x40005)
        return 0x40005;
    uint32 result = r_u32(player_data + 0x1fc);
    if (result != 0)
        return result;
    uint32 detail = r_u32(parameters + 8);
    uint32 enabled = r_u8(detail + 2);
    if (enabled != 1)
        return 1;
    uint32 parent = r_u32(object + 0x2c);
    uint32 parent_data = r_u32(parent + 0x20);
    uint32 flags = r_u32(parent_data + 0x18);
    result = flags & 0x100;
    if (result != 0)
        return result;
    uint32 target = r_u32(parent + 0xec);
    result = G3DispatchStateEvent(target, parent, game, 0x7e5, 0, frame);
    if (result != 0)
        w_u16(object + 0xa2, enabled);
    return result;
}

uint32 G3Function8007C5D8(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007c5d8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    switch (message)
    {
    case 1:
        G3Function8007C734(object);
        return 1;
    case 2:
        G3Function8007C800(object, game, frame);
        return 1;
    case 3:
        G3Function8007CC8C(object, game);
        return 1;
    case 0x7d3:
        G3Function8007C6B8(object, payload);
        return 1;
    case 0x7d4:
        return G3Function8007C728(object);
    case 0x7ef:
        return G3Function8007C71C(object);
    default:
        return 0;
    }
}

uint32 G3Function8007C71C(uint32 object)
{
    FUNCTION_MARKER(0x8007c71c, "SLUS_008.06");
    return r_u32(object + 0x12c);
}

uint32 G3Function8007C728(uint32 object)
{
    FUNCTION_MARKER(0x8007c728, "SLUS_008.06");
    return r_u32(object + 0x118);
}

uint32 G3Function8007C6B8(uint32 object, uint32 value)
{
    FUNCTION_MARKER(0x8007c6b8, "SLUS_008.06");
    w_u32(object + 0x118, value);
    uint32 data = r_u32(object + 0x1c);
    data = r_u32(data + 12);
    data = r_u32(data);
    data = r_u32(data + 0x1c);
    uint32 scale = r_u32(data + 12);
    uint32 first = value * scale;
    w_u16(object + 0x11c, first);
    scale = r_u32(data + 12);
    uint32 result = (value + 1) * scale - 1;
    w_u16(object + 0x11e, result);
    w_u16(object + 0x9e, result);
    return result;
}

uint32 G3Function8007CC8C(uint32 object, uint32 game)
{
    FUNCTION_MARKER(0x8007cc8c, "SLUS_008.06");
    uint32 parameters = r_u32(object + 0xb0);
    uint32 collided = r_u32(parameters + 20);
    uint32 player = r_u32(game + 0x50);
    if (collided != player)
        return player;
    uint32 mode = r_u32(object + 0x128);
    uint32 result = (sint32)mode < 3 ? 1 : 0;
    if (result == 0)
        return result;
    uint32 player_data = r_u32(collided + 0x24);
    uint32 previous = r_u32(player_data + 0x17c);
    if (previous == object)
        return result;
    if (previous != 0)
        G3Function800740B0(previous);
    w_u32(player_data + 0x17c, object);
    w_u32(object + 0x128, 4);
    w_u32(object + 0x108, 0);
    return 4;
}
