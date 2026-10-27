#include "psx.h"
#include "xport.h"
#include "game_functions.h"
#include <string.h>

void G3Function800328A0(uint32 object, uint32 descriptor, uint32 matrices, uint32 model, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800328A0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x60;
    w_u32(frame + 0x68, matrices);
    w_u32(frame + 0x6c, model);
    w_u32(frame + 0x18, 0x1f8000f0);
    w_u32(frame + 0x20, 0);
    uint32 animation_index = r_u16(object + 0x6e);
    sint32 animation_count = r_s16(descriptor + 0xa);
    uint32 adjustment = r_u32(object + 0xa4);
    uint32 count_bits = r_u16(descriptor + 0xa);
    if ((sint32)animation_index >= animation_count)
        w_u16(object + 0x6e, count_bits - 1);
    animation_index = r_u16(object + 0x6e);
    uint32 animations = r_u32(descriptor + 0x10);
    uint32 animation = r_u32(animations + (animation_index << 2));
    sint32 requested_frame = r_s16(object + 0x5e);
    sint32 frame_count = r_s16(animation + 2);
    uint32 frame_bits = r_u16(animation + 2);
    if (requested_frame >= frame_count)
        w_u16(object + 0x5e, frame_bits - 1);
    uint32 selected = r_u16(object + 0x6e);
    uint32 previous = r_u16(object + 0x86);
    if (selected != previous)
        G3Function80030520(object, r_u32(frame + 0x6c), animation, frame);
    requested_frame = r_s16(object + 0x5e);
    sint32 previous_frame = r_s16(object + 0x76);
    if (requested_frame < previous_frame)
        G3Function800305E8(object, r_u32(frame + 0x6c), animation, frame);
    w_u32(0x1f8000c0, animation);
    w_u32(0x1f8000b8, r_u32(animation + 8));
    w_u16(0x1f8000bc, r_u16(object + 0x76));
    uint32 current_frame = r_u16(object + 0x5e);
    w_u32(frame + 0x30, 8);
    w_u16(0x1f8000be, current_frame);
    uint32 channel_blocks = r_u32(object + 0x98);
    w_u32(0x1f8000cc, 8);
    w_u32(0x1f8000c4, channel_blocks);
    w_u32(0x1f8000c8, channel_blocks + 4);
    uint32 mask_blocks = r_u32(object + 0x94);
    uint32 working = r_u32(frame + 0x68);
    w_u32(frame + 0x28, mask_blocks);
    current_frame = r_u16(object + 0x5e);
    uint32 previous_matrices = r_u32(object + 0x30);
    w_u16(object + 0x76, current_frame);
    w_u32(object + 0x34, previous_matrices);
    uint32 current_model = r_u32(frame + 0x6c);
    uint32 count = r_u16(current_model + 6);
    w_u32(frame + 0x2c, mask_blocks + 4);
    if (count < 0x17)
        working = 0x1f8000f0;
    sint32 rotation_z = r_s16(object + 0x64);
    w_u32(object + 0x30, working + 0x20);
    if (rotation_z != 0 || r_u32(object + 0x60) != 0)
        G3Function8005BD00(object + 0x60, working);
    else
    {
        w_u32(working + 0x10, 0x1000);
        w_u32(working + 8, 0x1000);
        w_u32(working, 0x1000);
        w_u32(working + 0xc, 0);
        w_u32(working + 4, 0);
    }
    if (r_s16(object + 0x6c) != 0x1000 || r_u32(object + 0x68) != 0x10001000)
    {
        w_u32(0x1f800028, (uint32)r_s16(object + 0x68));
        w_u32(0x1f80002c, (uint32)r_s16(object + 0x6a));
        uint32 scale_z = (uint32)r_s16(object + 0x6c);
        w_u32(frame + 0x20, 1);
        w_u32(0x1f800030, scale_z);
        G3Function8005ADF0(working, 0x1f800028);
    }
    w_u32(working + 0x14, (uint32)r_s16(object + 0x58));
    w_u32(working + 0x18, (uint32)r_s16(object + 0x5a));
    w_u32(working + 0x1c, (uint32)r_s16(object + 0x5c));
    current_model = r_u32(frame + 0x6c);
    uint32 output = working + 0x20;
    sint32 segment_count = r_s16(current_model + 6);
    uint32 records = r_u32(current_model + 0x18);
    uint32 segment_index = 0;
    w_u32(frame + 0x1c, records);
    if (segment_count > 0)
    {
        w_u32(frame + 0x34, records + 0xc);
        uint32 translation_end = working + 0x3c;
        uint32 adjustment_type = adjustment + 6;
        do
        {
            uint32 mask_cursor = r_u32(frame + 0x2c);
            uint32 translation = r_u32(frame + 0x34);
            uint32 flags = (uint32)r_s16(mask_cursor);
            uint32 parent_index = (uint32)r_s16(translation + 6);
            w_u32(frame + 0x2c, mask_cursor + 2);
            uint32 parent = working + (parent_index << 5) + 0x20;
            w_u32(frame + 0x24, parent);
            if ((flags & 0x1c7) != 0 || adjustment != 0)
            {
                if ((flags & 7) != 0)
                {
                    for (uint32 axis = 0; axis < 3; ++axis)
                    {
                        uint32 angle = (flags & (1u << axis)) != 0 ? G3Function80030B54(0x1f8000b8, frame) : 0;
                        w_u16(0x1f800010 + axis * 2, angle);
                    }
                    G3Function8005BC88(0x1f800010, output, frame);
                }
                else
                {
                    w_u32(translation_end - 0xc, 0x1000);
                    w_u32(translation_end - 0x14, 0x1000);
                    w_u32(output, 0x1000);
                    w_u32(translation_end - 0x10, 0);
                    w_u32(translation_end - 0x18, 0);
                }
                if (adjustment != 0 && (uint32)r_s16(adjustment_type) == segment_index)
                {
                    uint32 angle = (uint32)r_s16(adjustment);
                    if (angle != 0)
                        G3Function8005B94C(angle, output, frame);
                    angle = (uint32)r_s16(adjustment_type - 4);
                    if (angle != 0)
                        G3Function8005BA5C(angle, output, frame);
                    angle = (uint32)r_s16(adjustment_type - 2);
                    if (angle != 0)
                        G3Function8005BB6C(angle, output, frame);
                    adjustment_type += 8;
                    sint32 next_type = r_s16(adjustment_type);
                    adjustment += 8;
                    if (next_type == -1)
                    {
                        adjustment_type = 6;
                        adjustment = 0;
                    }
                }
                if ((flags & 0x1c0) != 0)
                {
                    for (uint32 axis = 0; axis < 3; ++axis)
                    {
                        if ((flags & (0x40u << axis)) != 0)
                        {
                            uint32 value = G3Function80030B54(0x1f8000b8, frame);
                            value = (uint32)((sint32)(value << 16) >> 14);
                            w_u32(0x1f800028 + axis * 4, value);
                            if (value != 0x1000)
                                w_u32(frame + 0x20, 1);
                        }
                        else
                            w_u32(0x1f800028 + axis * 4, 0x1000);
                    }
                    if (r_u32(frame + 0x20) != 0)
                        G3Function8005ADF0(output, 0x1f800028);
                }
                parent = r_u32(frame + 0x24);
                MulMatrix2((MATRIX *)psx_addr(parent, 20), (MATRIX *)psx_addr(output, 20));
                if (adjustment != 0 && (uint32)r_s16(adjustment_type) == segment_index + 0x1000)
                {
                    uint32 angle = (uint32)r_s16(adjustment);
                    if (angle != 0)
                        G3Function8005B94C(angle, output, frame);
                    angle = (uint32)r_s16(adjustment_type - 4);
                    if (angle != 0)
                        G3Function8005BA5C(angle, output, frame);
                    angle = (uint32)r_s16(adjustment_type - 2);
                    if (angle != 0)
                        G3Function8005BB6C(angle, output, frame);
                    adjustment_type += 8;
                    sint32 next_type = r_s16(adjustment_type);
                    adjustment += 8;
                    if (next_type == -1)
                    {
                        adjustment_type = 6;
                        adjustment = 0;
                    }
                }
                uint32 segment = r_u32(frame + 0x1c);
                if ((r_u32(segment) & 3) != 0)
                {
                    uint32 scaled = r_u32(frame + 0x20);
                    w_u32(frame + 0x10, scaled);
                    G3Function80031A14(segment, output, output, object, scaled, frame);
                }
                w_u16(translation_end - 0xa, r_u16(frame + 0x20));
            }
            else
            {
                for (uint32 group = 0; group < 2; ++group)
                {
                    uint32 offset = group * 0x10;
                    uint32 first = r_u32(parent + offset);
                    uint32 second = r_u32(parent + offset + 4);
                    uint32 third = r_u32(parent + offset + 8);
                    uint32 fourth = r_u32(parent + offset + 0xc);
                    w_u32(output + offset, first);
                    w_u32(output + offset + 4, second);
                    w_u32(output + offset + 8, third);
                    w_u32(output + offset + 0xc, fourth);
                }
            }
            if ((flags & 0x38) != 0)
            {
                for (uint32 axis = 0; axis < 3; ++axis)
                {
                    uint32 value;
                    if ((flags & (8u << axis)) != 0)
                        value = G3Function80030B54(0x1f8000b8, frame);
                    else
                        value = r_u16(r_u32(frame + 0x34) + axis * 2);
                    w_u16(0x1f800090 + axis * 2, value);
                }
            }
            parent = r_u32(frame + 0x24);
            uint32 control0 = r_u32(parent);
            uint32 control1 = r_u32(parent + 4);
            xport_gte_write_control(0, control0);
            xport_gte_write_control(1, control1);
            uint32 control2 = r_u32(parent + 8);
            uint32 control3 = r_u32(parent + 0xc);
            uint32 control4 = r_u32(parent + 0x10);
            xport_gte_write_control(2, control2);
            xport_gte_write_control(3, control3);
            xport_gte_write_control(4, control4);
            translation = (flags & 0x38) != 0 ? 0x1f800090 : r_u32(frame + 0x34);
            xport_gte_write_data(0, r_u32(translation));
            xport_gte_write_data(1, r_u32(translation + 4));
            xport_gte_execute(0x486012);
            w_u32(output + 0x14, xport_gte_read_data(25));
            w_u32(output + 0x18, xport_gte_read_data(26));
            w_u32(output + 0x1c, xport_gte_read_data(27));
            output += 0x20;
            uint32 x = r_u32(translation_end - 8);
            parent = r_u32(frame + 0x24);
            translation = r_u32(frame + 0x34);
            uint32 parent_x = r_u32(parent + 0x14);
            w_u32(frame + 0x34, translation + 0x18);
            w_u32(translation_end - 8, x + parent_x);
            uint32 y = r_u32(translation_end - 4);
            uint32 segment = r_u32(frame + 0x1c);
            uint32 parent_y = r_u32(parent + 0x18);
            w_u32(frame + 0x1c, segment + 0x18);
            w_u32(translation_end - 4, y + parent_y);
            uint32 z = r_u32(translation_end);
            uint32 remaining = r_u32(frame + 0x30);
            uint32 parent_z = r_u32(parent + 0x1c);
            --remaining;
            w_u32(frame + 0x30, remaining);
            w_u32(translation_end, z + parent_z);
            translation_end += 0x20;
            if (remaining == 0)
            {
                uint32 block = r_u32(frame + 0x28);
                block = r_u32(block);
                w_u32(frame + 0x28, block);
                if (block != 0)
                {
                    w_u32(frame + 0x2c, block + 4);
                    w_u32(frame + 0x30, 8);
                }
            }
            current_model = r_u32(frame + 0x6c);
            segment_count = r_s16(current_model + 6);
            ++segment_index;
        } while ((sint32)segment_index < segment_count);
    }
    current_model = r_u32(frame + 0x6c);
    if (r_u16(current_model + 6) < 0x17)
    {
        uint32 destination = r_u32(frame + 0x68);
        w_u32(object + 0x30, destination + 0x20);
        segment_count = r_s16(current_model + 6);
        segment_index = 0;
        if (segment_count >= 0)
        {
            destination = r_u32(frame + 0x68);
            uint32 source = r_u32(frame + 0x18);
            do
            {
                for (uint32 group = 0; group < 2; ++group)
                {
                    uint32 offset = group * 0x10;
                    uint32 first = r_u32(source + offset);
                    uint32 second = r_u32(source + offset + 4);
                    uint32 third = r_u32(source + offset + 8);
                    uint32 fourth = r_u32(source + offset + 0xc);
                    w_u32(destination + offset, first);
                    w_u32(destination + offset + 4, second);
                    w_u32(destination + offset + 8, third);
                    w_u32(destination + offset + 0xc, fourth);
                }
                destination += 0x20;
                current_model = r_u32(frame + 0x6c);
                segment_count = r_s16(current_model + 6);
                ++segment_index;
                source += 0x20;
            } while (segment_count >= (sint32)segment_index);
        }
    }
}

void G3Function80030520(uint32 object, uint32 model, uint32 animation, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80030520, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    sint32 channels = r_s16(animation);
    sint32 needed = channels + 7;
    if (needed < 0)
        needed = channels + 14;
    sint32 capacity = r_s16(object + 0x9c);
    sint32 available = capacity + 7;
    if (available < 0)
        available = capacity + 14;
    if ((available >> 3) < (needed >> 3))
    {
        uint32 previous = r_u16(object + 0x86);
        uint32 descriptor = r_u32(object + 0x1c);
        w_u16(object + 0x6e, previous);
        uint32 table = r_u32(descriptor + 0x10);
        animation = r_u32(table + (previous << 2));
    }
    w_u16(object + 0x86, r_u16(object + 0x6e));
    G3Function80030710(object, model, animation, frame);
    w_u16(object + 0x76, 0);
}

void G3Function8005BC88(uint32 rotation, uint32 matrix, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8005BC88, "SLUS_008.06");
    uint32 frame = guest_stack - 0x14;
    uint32 x = (uint32)r_s16(rotation);
    uint32 y = (uint32)r_s16(rotation + 2);
    uint32 z = (uint32)r_s16(rotation + 4);
    G3Function8005B8D4(x, matrix);
    if (y != 0)
        G3Function8005BA5C(y, matrix, frame);
    if (z != 0)
        G3Function8005BB6C(z, matrix, frame);
}

uint32 G3Function8005B8D4(uint32 angle, uint32 matrix)
{
    FUNCTION_MARKER(0x8005B8D4, "SLUS_008.06");
    G3AnglePair pair = G3Function8005B6B0(angle);
    uint32 negative_sine = (0u - pair.sine) << 16;
    uint32 cosine = pair.cosine & 0xffff;
    uint32 packed = cosine | negative_sine;
    uint32 sine = pair.sine << 16;
    w_u32(matrix, 0x1000);
    w_u32(matrix + 4, 0);
    w_u32(matrix + 8, packed);
    w_u32(matrix + 0xc, sine);
    w_u32(matrix + 0x10, cosine);
    return pair.cosine;
}

void G3Function800305E8(uint32 object, uint32 model, uint32 animation, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800305E8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    uint32 mask_remaining = 8;
    uint32 channel_remaining = 8;
    w_u32(frame + 0x50, object);
    uint32 mask_block = r_u32(object + 0x94);
    uint32 channel_block = r_u32(object + 0x98);
    w_u32(frame + 0x18, animation);
    uint32 cursor = r_u32(animation + 8);
    uint32 mask_cursor = mask_block + 4;
    w_u32(frame + 0x10, cursor);
    sint32 count = r_s16(model + 6);
    uint32 channel = channel_block + 4;
    uint32 index = 0;
    if (count > 0)
    {
        do
        {
            sint32 flags = r_s16(mask_cursor);
            mask_cursor += 2;
            while (flags != 0)
            {
                if ((flags & 1) != 0)
                {
                    G3Function80030F94(frame + 0x10, channel, frame);
                    --channel_remaining;
                    channel += 8;
                    if (channel_remaining == 0)
                    {
                        channel_block = r_u32(channel_block);
                        if (channel_block != 0)
                        {
                            channel = channel_block + 4;
                            channel_remaining = 8;
                        }
                    }
                }
                flags >>= 1;
            }
            --mask_remaining;
            if (mask_remaining == 0)
            {
                mask_block = r_u32(mask_block);
                if (mask_block != 0)
                {
                    mask_cursor = mask_block + 4;
                    mask_remaining = 8;
                }
            }
            ++index;
        } while ((sint32)index < count);
    }
    uint32 owner = r_u32(frame + 0x50);
    w_u16(owner + 0x76, 0);
}

uint32 G3Function80030B54(uint32 context, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80030B54, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 channel = r_u32(context + 0x10);
    uint32 target = r_u32(channel + 4);
    uint32 value = G3DispatchChannelValue(target, context, frame);
    channel = r_u32(context + 0x10);
    uint32 remaining = r_u32(context + 0x14);
    channel += 8;
    --remaining;
    w_u32(context + 0x10, channel);
    w_u32(context + 0x14, remaining);
    if (remaining == 0)
    {
        uint32 block = r_u32(context + 0xc);
        uint32 next = r_u32(block);
        w_u32(context + 0xc, next);
        if (next != 0)
        {
            w_u32(context + 0x10, next + 4);
            w_u32(context + 0x14, 8);
        }
    }
    return (uint32)((sint32)(value << 16) >> 16);
}

void G3Function80026D54(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80026D54, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    w_u32(game + 0x659c, 1);
    G3Function800322CC(game, frame);
    w_u32(game + 0x659c, 1);
}

void G3Function8004D908(uint32 destination)
{
    FUNCTION_MARKER(0x8004D908, "SLUS_008.06");
    uint32 high = r_masked_u32(0x80096b04, 3);
    uint32 low = r_masked_u32(0x80096b00, 12);
    uint32 words[2];
    words[0] = (high << 16) | (low >> 16);
    high = r_masked_u32(0x80096b08, 3);
    low = r_masked_u32(0x80096b04, 12);
    words[1] = (high << 16) | (low >> 16);
    for (uint32 index = 0; index != 2; ++index)
    {
        uint32 address = destination + index * 4;
        uint32 high_address = address + 3;
        uint32 lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - lane) * 8), (1 << (lane + 1)) - 1);
        lane = address & 3;
        w_masked_u32(address & 0xfffffffc, words[index] << (lane * 8), (15 << lane) & 15);
    }
}

uint32 G3Function8004D9C0(void)
{
    FUNCTION_MARKER(0x8004D9C0, "SLUS_008.06");
    return r_u32(0x80096a7c);
}

uint32 G3Function8004D9F0(void)
{
    FUNCTION_MARKER(0x8004D9F0, "SLUS_008.06");
    return r_u32(0x80096a60);
}

void G3Function8004DA00(uint32 destination)
{
    FUNCTION_MARKER(0x8004DA00, "SLUS_008.06");
    if (destination != 0)
    {
        r_masked_u32(0x80096a3c, 15);
        uint32 word = r_masked_u32(0x80096a3c, 15);
        sint32 z = r_s16(0x80096a40);
        uint32 high_address = destination + 3;
        uint32 lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, word >> ((3 - lane) * 8), (1 << (lane + 1)) - 1);
        lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, word << (lane * 8), (15 << lane) & 15);
        w_u16(destination + 4, z);
    }
}

void G3Function8004DA30(uint32 destination)
{
    FUNCTION_MARKER(0x8004DA30, "SLUS_008.06");
    if (destination != 0)
    {
        r_masked_u32(0x80096a44, 15);
        uint32 words[2];
        words[0] = r_masked_u32(0x80096a44, 15);
        r_masked_u32(0x80096a48, 15);
        words[1] = r_masked_u32(0x80096a48, 15);
        for (uint32 index = 0; index != 2; ++index)
        {
            uint32 address = destination + index * 4;
            uint32 high_address = address + 3;
            uint32 lane = high_address & 3;
            w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - lane) * 8), (1 << (lane + 1)) - 1);
            lane = address & 3;
            w_masked_u32(address & 0xfffffffc, words[index] << (lane * 8), (15 << lane) & 15);
        }
    }
}

uint32 G3Function8004DA68(uint32 position)
{
    FUNCTION_MARKER(0x8004DA68, "SLUS_008.06");
    w_u16(0x80096a4c, r_u16(position));
    w_u16(0x80096a4e, r_u16(position + 2));
    uint32 z = r_u16(position + 4);
    w_u16(0x80096a50, z);
    return z;
}

void G3Function80030FF0(uint32 camera, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80030FF0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x98;
    uint32 matrix = r_u32(camera + 0x78);
    memset(psx_addr(frame + 0x58, 8), 0, 8);
    w_u16(frame + 0x58, 0u - r_u16(camera));
    w_u16(frame + 0x5a, 0u - r_u16(camera + 2));
    uint32 z = r_u16(camera + 4);
    uint32 game = r_u32(0x80097048);
    w_u16(frame + 0x5c, 0u - z);
    uint32 words[2];
    for (uint32 index = 0; index != 2; ++index)
    {
        uint32 address = frame + 0x58 + index * 4;
        uint32 high_address = address + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low_lane = address & 3;
        uint32 low = r_masked_u32(address & 0xfffffffc, (15 << low_lane) & 15);
        words[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    for (uint32 index = 0; index != 2; ++index)
    {
        uint32 address = frame + 0x50 + index * 4;
        uint32 high_address = address + 3;
        uint32 lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, words[index] >> ((3 - lane) * 8), (1 << (lane + 1)) - 1);
        lane = address & 3;
        w_masked_u32(address & 0xfffffffc, words[index] << (lane * 8), (15 << lane) & 15);
    }
    if ((r_u32(game + 0xe8) & 8) == 0)
    {
        if (G3Function8004D9C0() == 0)
        {
            uint32 source = G3Function8004D9F0();
            TransposeMatrix((MATRIX *)psx_addr(source, 18), (MATRIX *)psx_addr(frame + 0x10, 18));
        }
        else
            G3Function8002F718(frame + 0x10);
        G3Function8005BB6C(0u - (uint32)r_s16(camera + 0xc), frame + 0x10, frame);
        G3Function8005BA5C(0u - (uint32)r_s16(camera + 0xa), frame + 0x10, frame);
        G3Function8005B94C(0u - (uint32)r_s16(camera + 8), frame + 0x10, frame);
        w_u16(frame + 0x50, 0u - r_u16(camera));
        w_u16(frame + 0x52, 0u - r_u16(camera + 2));
        w_u16(frame + 0x54, 0u - r_u16(camera + 4));
    }
    else
    {
        G3Function8004DA00(frame + 0x78);
        G3Function8004DA30(frame + 0x70);
        G3Function8002F718(frame + 0x10);
        G3Function8005BB6C(0u - (uint32)r_s16(frame + 0x74), frame + 0x10, frame);
        G3Function8005BA5C(0u - (uint32)r_s16(frame + 0x72), frame + 0x10, frame);
        G3Function8005B94C(0u - (uint32)r_s16(frame + 0x70), frame + 0x10, frame);
        uint32 x = r_u16(frame + 0x78);
        z = r_u16(frame + 0x7c);
        w_u16(frame + 0x50, 0u - x);
        uint32 y = r_u16(frame + 0x7a);
        w_u16(frame + 0x54, 0u - z);
        w_u16(frame + 0x52, 0u - y);
        G3Function8004DA68(camera);
        w_u16(camera, r_u16(frame + 0x78));
        w_u16(camera + 2, r_u16(frame + 0x7a));
        w_u16(camera + 4, r_u16(frame + 0x7c));
    }
    w_u16(frame + 0x30, 0x1000);
    w_u16(frame + 0x32, 0);
    w_u16(frame + 0x34, 0);
    w_u16(frame + 0x36, 0);
    w_u16(frame + 0x38, 0);
    w_u16(frame + 0x3a, 0xf000);
    w_u16(frame + 0x3c, 0);
    w_u16(frame + 0x3e, 0x1000);
    w_u16(frame + 0x40, 0);
    MulMatrix0((MATRIX *)psx_addr(frame + 0x30, 20), (MATRIX *)psx_addr(frame + 0x10, 20), (MATRIX *)psx_addr(matrix, 20));
    uint32 second_matrix = r_u32(camera + 0x7c);
    MulMatrix0((MATRIX *)psx_addr(frame + 0x30, 20), (MATRIX *)psx_addr(frame + 0x10, 20), (MATRIX *)psx_addr(second_matrix, 20));
    sint32 row_x = r_s16(matrix) * 512;
    sint32 row_y = r_s16(matrix + 2) * 512;
    sint32 row_z = r_s16(matrix + 4) * 512;
    w_u16(matrix, row_x / 320);
    w_u16(matrix + 2, row_y / 320);
    w_u16(matrix + 4, row_z / 320);
    G3Function8004D908(frame + 0x58);
    sint32 element = r_s16(matrix);
    for (uint32 index = 0; index != 9; ++index)
    {
        sint32 scale = r_s16(frame + 0x58 + (index / 3) * 2);
        uint32 product = (uint32)element * (uint32)scale;
        sint32 next = 0;
        if (index != 8)
            next = r_s16(matrix + (index + 1) * 2);
        w_u16(matrix + index * 2, (sint32)product >> 12);
        element = next;
    }
    G3Function8005ADA0(matrix, frame + 0x50, frame + 0x60);
    G3Function8008BCA8(matrix, frame + 0x60);
    second_matrix = r_u32(camera + 0x7c);
    G3Function8005ADA0(second_matrix, frame + 0x50, frame + 0x60);
    second_matrix = r_u32(camera + 0x7c);
    G3Function8008BCA8(second_matrix, frame + 0x60);
}

uint32 G3Function8004D968(uint32 camera, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004D968, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function80030FF0(camera, frame);
    uint32 matrix = r_u32(camera + 0x7c);
    w_u16(matrix + 0x12, 0);
    uint32 inverse = r_u32(camera + 0x80);
    matrix = r_u32(camera + 0x7c);
    G3Function800313E0(inverse, matrix, frame);
    return G3Function80014164(camera);
}

void G3Function80026904(void)
{
    FUNCTION_MARKER(0x80026904, "SLUS_008.06");
}

uint32 G3Function8002690C(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002690C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 flags = r_u32(game + 0x6834);
    if ((flags & 0x40000) == 0)
        return 0xfffb0000;
    flags &= 0xfffbffff;
    w_u32(game + 0x6834, flags);
    return G3Function80024254(game, flags, frame);
}

uint32 G3Function800269A0(uint32 game)
{
    FUNCTION_MARKER(0x800269A0, "SLUS_008.06");
    uint32 global_game = r_u32(0x80097048);
    uint32 index = r_u32(global_game + 0x58);
    w_u32(game + 0x6558, global_game + index * 0x3200 + 0x158);
    index = r_u32(global_game + 0x58);
    uint32 limit = global_game + index * 0x3200 + 0x3338;
    index = r_u32(game + 0x58);
    w_u32(game + 0x655c, limit);
    uint32 pool = r_u32(0x800a0c7c + index * 4);
    w_u32(game + 0x68, pool);
    w_u32(pool + 4, pool + 0xc);
    pool = r_u32(game + 0x68);
    w_u32(pool, 0);
    return pool;
}

void G3Function80026E48(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80026E48, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    w_u32(game + 0x6580, 1);
    G3Function80029784(game, frame);
    w_u32(game + 0x6580, 1);
}

uint32 G3Function80026E80(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80026E80, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    w_u32(game + 0x6584, 1);
    G3Function8001471C(game, frame);
    uint32 camera = r_u32(game + 0x4c);
    uint32 result = G3Function8004D968(camera, frame);
    w_u32(game + 0x6584, 1);
    return result;
}

uint32 G3Function80026EC4(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80026EC4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    w_u32(game + 0x65bc, 1);
    uint32 result = G3Function80011854(game, frame);
    w_u32(game + 0x65bc, 1);
    return result;
}

void G3Function80026EFC(void)
{
    FUNCTION_MARKER(0x80026EFC, "SLUS_008.06");
}

uint32 G3Function80026F04(uint32 game)
{
    FUNCTION_MARKER(0x80026F04, "SLUS_008.06");
    sint32 value = r_s16(game + 0x6782);
    if (value != 0)
        w_u16(game + 0x6782, 0);
    return (uint32)value;
}

void G3Function80026F20(void)
{
    FUNCTION_MARKER(0x80026F20, "SLUS_008.06");
}

uint32 G3Function80026F28(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80026F28, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function80026904();
    G3Function8002690C(game, frame);
    G3Function80026948(game, frame);
    G3Function800269A0(game);
    uint32 flags = r_u32(game + 0x6834) & 0xfeffffff;
    uint32 buffer_index = r_u32(game + 0x58);
    w_u16(game + 0xf6, 0);
    w_u32(game + 0x6834, flags);
    w_u32(game + 0x58, 1u - buffer_index);
    memset(psx_addr(game + 0xc0, 0x28), 0, 0x28);
    G3Function8007776C(frame);
    G3Function800271F8(game, frame);
    G3Function80026A34(game, frame);
    if (r_s16(game + 0x6786) != 6 && r_s16(game + 0x6780) == 0)
    {
        w_u32(game + 0x69ec, 0);
        G3Function80026A9C(game, frame);
        G3Function80026C00(game, frame);
        G3Function80026C54(game, frame);
        G3Function80026CAC(game, frame);
        G3Function80026CEC(game, frame);
        G3Function80035F38(game, frame);
    }
    G3Function80026D54(game, frame);
    if (r_s16(game + 0x6786) != 6 && r_s16(game + 0x6780) == 0)
    {
        G3Function80026D8C(game, frame);
        G3Function8002B0F0(game);
        G3Function80026E48(game, frame);
        G3Function80026E80(game, frame);
    }
    G3Function80026EC4(game, frame);
    G3Function800657D8(game, frame);
    G3Function80026EFC();
    G3Function80026F04(game);
    if (r_s16(game + 0x6780) == 0)
        G3Function800268B8(game, frame);
    else
    {
        G3Function80022DEC();
        w_u32(game + 0x58, 1u - r_u32(game + 0x58));
    }
    G3Function80026F20();
    uint32 count = r_u32(game + 0x14c);
    flags = r_u32(game + 0xe8) & 0xf7ffffff;
    w_u32(game + 0x14c, count + 1);
    w_u32(game + 0xe8, flags);
    G3Function800812A0(frame);
    uint32 request = r_u32(game + 0x6834) & 0x1000000;
    uint32 result = G3Function80039F5C(0, 0x8009d6b4, request, frame);
    if (result != 0)
    {
        sint32 value = r_s16(0x8009d6bc);
        w_u16(0x8009d6bc, value == 4000 ? 6000 : 4000);
        result = 6000;
    }
    return result;
}

uint32 G3Function8002B0F0(uint32 game)
{
    FUNCTION_MARKER(0x8002B0F0, "SLUS_008.06");
    uint32 result = r_u32(game + 0x5c);
    uint32 object = r_u32(result + 4);
    while (object != 0)
    {
        result = r_u16(object + 0x58);
        uint32 position_y = r_u16(object + 0x5a);
        uint32 position_z = r_u16(object + 0x5c);
        uint32 rotation_x = r_u16(object + 0x60);
        uint32 rotation_y = r_u16(object + 0x62);
        uint32 rotation_z = r_u16(object + 0x64);
        uint32 scale_x = r_u16(object + 0x68);
        uint32 scale_y = r_u16(object + 0x6a);
        uint32 scale_z = r_u16(object + 0x6c);
        w_u16(object + 0x70, result);
        w_u16(object + 0x72, position_y);
        w_u16(object + 0x74, position_z);
        w_u16(object + 0x78, rotation_x);
        w_u16(object + 0x7a, rotation_y);
        w_u16(object + 0x7c, rotation_z);
        w_u16(object + 0x80, scale_x);
        w_u16(object + 0x82, scale_y);
        w_u16(object + 0x84, scale_z);
        object = r_u32(object + 8);
    }
    return result;
}

void G3Function80029784(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80029784, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 pool = r_u32(game + 0x5c);
    G3Function80029690(pool, 1, frame);
}

void G3Function80029690(uint32 pool, uint32 mark_resource, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80029690, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 object = r_u32(pool + 4);
    if (object != 0)
    {
        do
        {
            uint32 flags = r_u32(object + 0x10);
            uint32 next = r_u32(object + 8);
            if ((flags & 0x400) == 0)
            {
                if ((flags & 0x20) != 0)
                    G3Function80029574(pool, object, frame);
            }
            else if ((flags & 0x20) != 0)
                w_u32(object + 0x10, flags & 0xffffffdf);
            object = next;
        } while (object != 0);
        object = r_u32(pool + 4);
    }
    while (object != 0)
    {
        uint32 flags = r_u32(object + 0x10);
        uint32 next = r_u32(object + 8);
        if ((flags & 0x20) != 0)
        {
            uint32 resource = r_u32(object + 0x20);
            if (resource != 0 && mark_resource != 0)
                w_u32(resource + 0x18, r_u32(resource + 0x18) | 0x10);
            G3Function8002937C(pool, object, frame);
        }
        object = next;
    }
}

uint32 G3Function80029574(uint32 pool, uint32 object, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80029574, "SLUS_008.06");
    uint32 flags = r_u32(object + 0x10);
    if ((flags & 2) != 0)
        return G3Function800292B4(pool, object);
    uint32 resource = r_u32(object + 0x20);
    uint32 table = r_u32(resource + 4);
    if (table == 0 || (flags & 0x10) != 0)
        return G3Function800292B4(pool, object);
    uint32 matches = 0;
    uint32 remaining = r_u32(table);
    uint32 cursor = table + 4;
    while (remaining != 0)
    {
        uint32 entry = r_u32(cursor);
        bool matched = entry == 0;
        if (entry != 0)
        {
            uint32 entry_resource = r_u32(entry + 0x20);
            matched = entry_resource == 0;
            if (entry_resource != 0)
                matched = (r_u32(entry_resource + 0x10) & 0x420) == 0x20;
        }
        if (matched)
            ++matches;
        --remaining;
        cursor += 4;
    }
    uint32 result = r_u32(object + 0x20);
    table = r_u32(result + 4);
    remaining = r_u32(table);
    cursor = table + 4;
    if (remaining != matches && remaining != 0)
    {
        do
        {
            result = r_u32(cursor);
            if (result != 0)
            {
                uint32 entry_resource = r_u32(result + 0x20);
                if (entry_resource != 0)
                {
                    result = r_u32(entry_resource + 0x10) & 0xffffffdf;
                    w_u32(entry_resource + 0x10, result);
                }
            }
            --remaining;
            cursor += 4;
        } while (remaining != 0);
    }
    return result;
}

void G3Function80026C54(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80026C54, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    sint32 mode = r_s16(game + 0x6786);
    w_u32(game + 0x6594, 1);
    if (mode != 6)
    {
        uint32 owner = r_u32(0x800a2a68);
        G3Function800429B4(owner, frame);
        G3Function800756C0(frame);
    }
    w_u32(game + 0x6594, 1);
}

void G3Function80026CEC(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80026CEC, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 pool = r_u32(game + 0x5c);
    w_u32(game + 0x6568, 1);
    G3Function80029D88(pool, frame);
    G3Function80029784(game, frame);
    uint32 player = r_u32(0x8009efc4);
    if ((r_u32(player + 0x18) & 1) == 0)
        w_u32(player + 0xf8, 0);
    w_u32(game + 0x6568, 1);
}

void G3Function80026D8C(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80026D8C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 flags = r_u32(game + 0xe8) & 0x08000010;
    w_u32(game + 0x65c4, 1);
    bool process_objects = true;
    if (flags == 0)
    {
        if (r_s16(game + 0x6786) == 6)
            process_objects = false;
        else
            G3Function80018E40(frame);
    }
    if (process_objects && r_s16(game + 0x6786) != 6)
    {
        uint32 pool = r_u32(game + 0x5c);
        G3Function8005DF0C(pool, frame);
        uint32 scene = r_u32(game + 0x48);
        pool = r_u32(game + 0x5c);
        uint32 level = r_u32(scene);
        G3Function8005EF40(pool, level, frame);
    }
    G3Function8002B9D4(game, frame);
    if (r_s16(game + 0x6786) != 6 && r_s16(game + 0x67ee) != 1)
        G3Function8006EB20(game, frame);
    w_u32(game + 0x65c4, 1);
}

void G3Function80026948(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80026948, "SLUS_008.06");
    uint32 timestamp = r_u32(game + 0x6590);
    uint32 elapsed = G3ElapsedDrawTime8003A2F8(timestamp);
    w_u32(game + 0x6578, elapsed);
    uint32 low = G3ReadRootCounter800895D4(0xf2000000);
    w_u32(game + 0x6564, 1);
    uint32 high = r_u32(0x80096b74);
    timestamp = (low & 0xffff) | (high << 16);
    w_u32(game + 0x6590, timestamp);
}

void G3Function80026A34(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80026A34, "SLUS_008.06");
    uint32 count = r_u32(game + 0x28);
    uint32 index = r_u32(game + 0x58);
    uint32 offset = index * (count + 0x80);
    uint32 table = r_u32(game + 0x3c);
    count = r_u32(game + 0x28) + 0x80;
    w_u32(game + 0x65cc, 1);
    table += offset << 2;
    w_u32(game + 0x38, table + 0x200);
    G3Function800866D8(table, count);
    w_u32(game + 0x65cc, 1);
}

uint32 G3Function8007767C(void)
{
    FUNCTION_MARKER(0x8007767C, "SLUS_008.06");
    uint32 player = r_u32(0x8009efc4);
    uint32 state = r_u32(player + 0x24);
    uint32 delta = state + 0x264;
    if (r_s16(delta) == 0 && r_s16(delta + 2) == 0 && r_s16(delta + 4) == 0)
        return 0;
    player = r_u32(0x8009efc4);
    uint32 amount = r_u16(delta);
    uint32 position = r_u16(player + 0x58);
    w_u16(player + 0x58, position + amount);
    position = r_u16(player + 0x5a);
    amount = r_u16(delta + 2);
    w_u16(player + 0x5a, position + amount);
    position = r_u16(player + 0x5c);
    amount = r_u16(delta + 4);
    uint32 result = position + amount;
    w_u16(player + 0x5c, result);
    uint32 linked = r_u32(state + 0x1fc);
    if (linked != 0)
    {
        position = r_u16(linked + 0x58);
        amount = r_u16(delta);
        w_u16(linked + 0x58, position + amount);
        linked = r_u32(state + 0x1fc);
        amount = r_u16(delta + 2);
        position = r_u16(linked + 0x5a);
        w_u16(linked + 0x5a, position + amount);
        linked = r_u32(state + 0x1fc);
        amount = r_u16(delta + 4);
        position = r_u16(linked + 0x5c);
        result = position + amount;
        w_u16(linked + 0x5c, result);
    }
    w_u16(state + 0x264, 0);
    w_u16(state + 0x266, 0);
    w_u16(state + 0x268, 0);
    return result;
}

void G3Function8007776C(uint32 guest_stack)
{
    FUNCTION_MARKER(0x8007776C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function8007767C();
    uint32 player = r_u32(0x8009efc4);
    uint32 sequence = r_u32(player + 0xf4);
    if (sequence != 0)
    {
        G3Function80018DE0(player, sequence, frame);
        player = r_u32(0x8009efc4);
        w_u32(player + 0xf4, 0);
    }
}

uint32 G3Function8004DACC(void)
{
    FUNCTION_MARKER(0x8004DACC, "SLUS_008.06");
    return r_u32(0x80096a88) == 5 ? 1 : 0;
}

uint32 G3Function80027118(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80027118, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (r_s16(game + 0x6786) == 4)
        G3Function80011A84(frame);
    uint32 owner = r_u32(game + 0x50);
    w_u16(game + 0x6786, 0);
    uint32 flags = r_u32(owner + 0x10);
    w_u32(owner + 0x10, flags & 0xfffffeff);
    return owner;
}

void G3Function800271F8(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800271F8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (((r_u32(game + 0xe8) & 0x40000) == 0 || (r_u32(game + 0x6900) & 0x4810) != 0) && (r_u32(game + 0x74) & 0x8000) != 0)
    {
        sint32 mode = r_s16(game + 0x6786);
        if (mode == 0)
            w_u16(game + 0x6786, 4);
        else if (mode == 7)
        {
            G3Function800128D4(frame);
            w_u16(game + 0x6786, 0);
        }
        else if (mode != 6)
            G3Function80027118(game, frame);
    }
    G3Function80012A28(frame);
    uint32 global_game = r_u32(0x80097048);
    if ((r_u32(global_game + 0x9c) & 0x4000) != 0 && (r_u32(game + 0xe8) & 0x40000) != 0)
    {
        r_u32(game + 0x4c);
        if (G3Function8004DACC() == 0 && r_s16(game + 0x6786) == 0)
            G3Function80012AB4(game, frame);
    }
    if ((r_u32(game + 0x6834) & 0x200000) != 0)
    {
        G3Function80012C5C(game, frame);
        uint32 flags = r_u32(game + 0x6834);
        w_u32(game + 0x6834, flags & 0xffdfffff);
    }
    if (r_s16(game + 0x6780) != 0)
        G3Function80027118(game, frame);
}

void G3Function80026680(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80026680, "SLUS_008.06");
    uint32 result;
    do
    {
        uint32 completion = r_u32(game + 0x140);
        result = G3Function800194B8(completion);
        if (result != 0)
            psx_native_poll_events();
    } while (result != 0);
}

void G3Function800266B4(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800266B4, "SLUS_008.06");
    uint32 index = r_u32(game + 0x58);
    G3PutDrawEnv80086850(0x800a3cd4 + index * 92);
}

void G3Function800266F8(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800266F8, "SLUS_008.06");
    uint32 low = G3ReadRootCounter800895D4(0xf2000000) & 0xffff;
    uint32 high = r_u32(0x80096b74);
    uint32 mode = r_u32(game + 0x150);
    w_u32(game + 0x657c, low | (high << 16));
    if (mode < 2)
    {
        uint32 result;
        do
        {
            uint32 completion = r_u32(game + 0x13c);
            result = G3Function800194B8(completion);
            if (result != 0)
                psx_native_poll_events();
        } while (result != 0);
    }
    else
    {
        uint32 environment = r_u32(game + 0x13c);
        if (environment != 0)
        {
            w_u32(game + 0x13c, 0);
            w_u32(game + 0x150, 0);
            G3PutDispEnv80086910(environment);
        }
    }
    uint32 timestamp = r_u32(game + 0x657c);
    uint32 elapsed = G3ElapsedDrawTime8003A2F8(timestamp);
    w_u32(game + 0x657c, elapsed);
}

void G3Function8002678C(void)
{
    FUNCTION_MARKER(0x8002678C, "SLUS_008.06");
}

void G3Function80026794(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80026794, "SLUS_008.06");
    uint32 order_table = r_u32(game + 0x38);
    uint32 low = G3ReadRootCounter800895D4(0xf2000000) & 0xffff;
    r_u32(game + 0x28);
    w_u32(game + 0x140, game + 0x6570);
    uint32 high = r_u32(0x80096b74);
    uint32 index = r_u32(game + 0x54);
    w_u32(game + 0x144, low | (high << 16));
    w_u32(game + 0x54, 1u - index);
    G3Function800126F4();
    uint32 flags = r_u32(game + 0x6834);
    if ((flags & 2) != 0)
    {
        w_u32(game + 0x6834, flags & 0xfffffffd);
        uint32 link = r_u32(order_table);
        w_u32(0x800a0c64, (link & 0xffffff) | 0x04000000);
        w_u32(order_table, 0x000a0c64);
        w_u32(0x800a0c50, 0x040a0c64);
        w_u32(order_table, 0x000a0c50);
        uint32 first = r_u8(0x800a0c6b);
        uint32 second = r_u8(0x800a0c57);
        w_u8(0x800a0c6b, first | 2);
        w_u8(0x800a0c57, second | 2);
    }
    if (r_s16(game + 0x6786) == 6)
        G3Function800867E0(game + 0x44);
    uint32 count = r_u32(game + 0x28);
    uint32 table = r_u32(game + 0x38);
    G3Function800867E0(table + (count << 2) - 4);
}

void G3Function800268B8(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800268B8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function800265AC(game, frame);
    G3Function80026680(game, frame);
    G3Function800266B4(game, frame);
    G3Function800266F8(game, frame);
    G3Function8002678C();
    G3Function80026794(game, frame);
}

uint32 G3Function80011A58(uint32 entry)
{
    FUNCTION_MARKER(0x80011A58, "SLUS_008.06");
    uint32 index = 0;
    while (entry != 0)
    {
        uint32 type = r_u32(entry);
        entry += 0x18;
        if (type == 4)
            break;
        ++index;
    }
    return index;
}

void G3Function80011A84(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80011A84, "SLUS_008.06");
    uint32 entries = r_u32(0x8009ceac);
    uint32 index = G3Function80011A58(entries);
    entries = r_u32(0x8009ceac);
    uint32 value = r_u32(0x8009cc00);
    w_u32(entries + index * 0x18 + 4, value);
}

void G3Function800128D4(uint32 guest_stack)
{
    FUNCTION_MARKER(0x800128D4, "SLUS_008.06");
    SetDefDispEnv((DISPENV *)psx_addr(0x800a3cac, 20), 0, 0, 512, 240);
    SetDefDispEnv((DISPENV *)psx_addr(0x800a3cc0, 20), 0, 256, 512, 240);
}

void G3Function80012A28(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80012A28, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    if (G3Function80012A14() != 0)
    {
        G3Function80021A74(540);
        uint32 text = G3Function80065D38(64);
        G3TextMeasureOperands operands;
        G3Function800234A0(text, 200, 0, 0, frame, &operands);
        text = G3Function80065D38(65);
        G3Function800234A0(text, 240, operands.second_glyph, operands.third_glyph, frame, 0);
        uint32 game = r_u32(0x80097048);
        if (r_s16(game + 0x6786) == 0 && r_u16(game + 0x68b4) != 27)
            G3Function80012AB4(game, frame);
    }
}

void G3Function80012AB4(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80012AB4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    if ((r_u32(game + 0xe8) & 0x40000) == 0)
        return;
    uint32 player = r_u32(0x8009efc4);
    uint32 state = r_u32(player + 0x150);
    if (state == 4 || state == 5)
        return;
    if (r_u32(game + 0x67c4) != 0 || state == 6)
        return;
    if ((r_u32(game + 0x6834) & 0x80) != 0)
        return;
    if (r_u16(game + 0x68b4) == 255 && r_u32(0x800a058c) == 0)
        return;
    G3Function8003EEAC(frame);
    G3Function8003556C();
    if (r_u16(game + 0x68b4) != 255)
        G3Function8003F1C4(0x81b, 0x74, 0x40, 0);
    uint32 owner = r_u32(game + 0x50);
    player = r_u32(0x8009efc4);
    uint32 saved_flags = r_u32(owner + 0x10);
    uint32 player_flags = r_u32(player + 0x10);
    state = r_u32(player + 0x150);
    w_u32(player + 0x150, 7);
    w_u32(player + 0x10, player_flags | 0x100);
    uint32 resource = r_u32(game + 0x67e4);
    w_u32(0x8009cc04, 0);
    w_u32(0x800a0528, saved_flags);
    w_u32(0x800a0544, state);
    G3Function8006A580(resource, frame);
    uint32 level = r_u16(game + 0x68b4);
    w_u16(game + 0x6786, 6);
    if (level < 255)
        w_u32(game + 0xe8, r_u32(game + 0xe8) | 0x10);
    G3Function8006C610(game);
    uint32 value = r_u32(game + 0x6818);
    G3Function80047EE4(0, value);
    G3Function8007446C(game);
    uint32 input = r_u32(game + 0x74);
    uint32 count = r_u32(game + 0x14c);
    w_u32(0x800a0548, input);
    w_u32(0x800a054c, r_u32(game + 0x70));
    w_u32(0x800a0550, r_u32(game + 0xa4));
    value = r_u32(game + 0xa8);
    w_u32(0x800a04f0, 0);
    w_u32(0x800a050c, count);
    w_u32(0x800a0554, value);
    G3Function80027CA4(frame);
    G3Function80081038(frame);
}

void G3Function80012C5C(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80012C5C, "SLUS_008.06");
    uint32 active = r_u32(0x800a04f0);
    uint32 frame = guest_stack - 0x18;
    if (active == 0 || r_s16(game + 0x6786) != 6)
        return;
    G3Function80027E6C(frame);
    uint32 global_game = r_u32(0x80097048);
    uint32 player = r_u32(0x8009efc4);
    uint32 flags = r_u32(global_game + 0xe8);
    uint32 state = r_u32(0x800a0544);
    w_u32(global_game + 0xe8, flags & 0xffffffef);
    w_u32(player + 0x150, state);
    flags = r_u32(player + 0x10);
    uint32 saved_flags = r_u32(0x800a0528);
    w_u32(player + 0x10, flags & 0xfffffeff);
    w_u32(player + 0x10, saved_flags);
    G3Function80027118(game, frame);
    w_u32(0x8009cc04, 0);
    G3Function8003EEE4(frame);
    global_game = r_u32(0x80097048);
    uint32 count = r_u32(0x800a050c);
    w_u16(game + 0x6786, 0);
    w_u32(global_game + 0x14c, count);
    w_u32(game + 0x74, r_u32(0x800a0548));
    w_u32(game + 0xa4, r_u32(0x800a0550));
    w_u32(game + 0xa8, r_u32(0x800a0554));
    G3Function8003F1C4(0x81b, 0x74, 0x40, 0xffffff9c);
    G3Function800810A8(frame);
}

void G3Function80081038(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80081038, "SLUS_008.06");
    for (uint32 group = 0; group != 2; ++group)
    {
        uint32 control = 0x800a2a8c + group * 4;
        w_u8(control + 1, 0);
        w_u8(control, 0);
        for (uint32 index = 0; index != 4; ++index)
        {
            uint32 address = 0x800a2aa2 + group * 0xd0 + index * 0x34;
            uint32 flags = r_u16(address);
            if ((flags & 1) != 0)
                w_u16(address, flags | 4);
        }
    }
}

void G3Function800810A8(uint32 guest_stack)
{
    FUNCTION_MARKER(0x800810A8, "SLUS_008.06");
    for (uint32 group = 0; group != 2; ++group)
    {
        for (uint32 index = 0; index != 4; ++index)
        {
            uint32 address = 0x800a2aa2 + group * 0xd0 + index * 0x34;
            uint32 flags = r_u16(address);
            w_u16(address, flags & 0xfffb);
        }
    }
}

uint32 G3Function80025FC4(uint32 game)
{
    FUNCTION_MARKER(0x80025FC4, "SLUS_008.06");
    w_u32(game + 0x65d4, 0xcd);
    w_u32(game + 0x65d8, 0x80063c6c);
    w_u32(game + 0x65dc, 0x80019fc8);
    w_u32(game + 0x65e0, 0x80084a64);
    w_u32(game + 0x65e4, 0x8001ce1c);
    w_u32(game + 0x65e8, 0x8005f518);
    w_u32(game + 0x65ec, 0x8005fc70);
    return 0x8005fc70;
}

void G3Function80026018(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80026018, "SLUS_008.06");
    uint32 time = r_u32(game + 0x14c);
    uint32 scene = r_u32(game + 0x48);
    uint32 packets = r_u32(game + 0x68);
    uint32 order_table = r_u32(game + 0x38);
    uint32 level = r_u32(scene);
    w_u32(game + 0x65a0, 1);
    uint32 sequence = r_u32(level + 0x48);
    G3Function80035044(sequence, time, packets, order_table);
    sequence = r_u32(scene + 0x84);
    time = r_u32(game + 0x14c);
    packets = r_u32(game + 0x68);
    G3Function80035044(sequence, time, packets, order_table);
    w_u32(game + 0x65a0, 1);
}

void G3Function8002BD64(void)
{
    FUNCTION_MARKER(0x8002BD64, "SLUS_008.06");
}

void G3Function8002BD6C(void)
{
    FUNCTION_MARKER(0x8002BD6C, "SLUS_008.06");
}

void G3Function80026090(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80026090, "SLUS_008.06");
    uint32 frame = guest_stack - 0x30;
    uint32 context = r_u32(game + 0x138);
    uint32 scene = r_u32(game + 0x48);
    uint32 camera = r_u32(game + 0x4c);
    r_u32(scene);
    w_u32(game + 0x65a4, 1);
    G3Function8002BD64();
    w_u32(context + 0x3d8, 0);
    G3Function8002BE94(camera, scene, context, frame);
    uint32 pool = r_u32(game + 0x5c);
    G3Function8002BFF0(context, pool, frame);
    G3Function8002BD6C();
    w_u32(game + 0x65a4, 1);
}

uint32 G3Function80027F5C(void)
{
    FUNCTION_MARKER(0x80027F5C, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 result = r_u32(game + 0x38) - 0x200;
    w_u32(game + 0x38, result);
    return result;
}

uint32 G3Function80027F7C(void)
{
    FUNCTION_MARKER(0x80027F7C, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 result = r_u32(game + 0x38) + 0x200;
    w_u32(game + 0x38, result);
    return result;
}

void G3Function80026128(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80026128, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 order_table = r_u32(game + 0x38);
    G3Function80011970(game, order_table, frame);
    G3Function80022E00(frame);
    uint32 value = r_u32(game + 0x65bc);
    w_u32(game + 0x65c0, value + 1);
}

void G3Function8002616C(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002616C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 order_table = r_u32(game + 0x38);
    G3Function8002463C(game, order_table, frame);
}

void G3Function80026190(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80026190, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 scene = r_u32(game + 0x48);
    uint32 disabled = r_u32(game + 0x6790);
    w_u32(game + 0x65a8, 1);
    uint32 value = r_u16(scene + 0x4e);
    w_u32(game + 0x34, value);
    if (disabled == 0)
    {
        uint32 target = r_u32(game + 0x65e4);
        uint32 camera = r_u32(game + 0x4c);
        uint32 packets = r_u32(game + 0x68);
        uint32 order_table = r_u32(game + 0x38);
        G3DispatchSceneRender(target, scene, camera, packets, order_table, frame);
    }
    w_u32(game + 0x65a8, 1);
}

void G3Function800261F0(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800261F0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 scene = r_u32(game + 0x48);
    w_u32(game + 0x65ac, 1);
    uint32 red = r_u8(scene + 0x48);
    uint32 green = r_u8(scene + 0x49);
    uint32 blue = r_u8(scene + 0x4a);
    uint32 order_table = r_u32(game + 0x38);
    SetFarColor(red, green, blue);
    G3Function8003420C(game, order_table, frame);
    red = r_u8(scene + 0x40);
    green = r_u8(scene + 0x41);
    blue = r_u8(scene + 0x42);
    SetFarColor(red, green, blue);
    w_u32(game + 0x65ac, 1);
    w_u32(game + 0x65b0, 1);
    G3Function800297A8(game, frame);
    w_u32(game + 0x65b0, 1);
}

void G3Function80026280(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80026280, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 scene = r_u32(game + 0x48);
    uint32 camera = r_u32(game + 0x4c);
    uint32 context = r_u32(0x800a0c24);
    uint32 level = r_u32(scene);
    w_u32(game + 0x6574, 1);
    G3Function8005AECC(level, camera, context, frame);
    w_u32(game + 0x6574, 1);
}

void G3Function800262C8(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800262C8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 first_scene = r_u32(game + 0x48);
    uint32 second_scene = r_u32(game + 0x48);
    uint32 camera = r_u32(game + 0x4c);
    uint32 level = r_u32(first_scene);
    w_u32(game + 0x656c, 1);
    uint32 value = r_u32(first_scene + 0xf4);
    w_u32(game + 0x20, value);
    value = r_u32(second_scene + 0xf0);
    uint32 shift = r_u32(game + 0x2c);
    w_u32(game + 0x24, value);
    sint32 depth = r_s32(camera + 0x88);
    uint32 scene = r_u32(game + 0x48);
    depth >>= shift & 31;
    w_u32(game + 0x30, depth);
    sint32 limit = r_s32(scene + 0x100);
    uint32 order_table = r_u32(game + 0x38);
    if (limit < depth)
        w_u32(game + 0x30, limit);
    if ((r_u32(game + 0x6834) & 0x200) == 0)
    {
        uint32 context = r_u32(0x800a0c24);
        w_u32(frame + 0x10, order_table);
        uint32 target = r_u32(game + 0x65e0);
        uint32 packets = r_u32(game + 0x68);
        uint32 result = G3DispatchTerrainRender(target, context, level, camera, packets, order_table, frame);
        packets = r_u32(game + 0x68);
        w_u32(packets + 4, result);
    }
    w_u32(game + 0x656c, 1);
}

void G3Function80026384(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80026384, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 scene = r_u32(game + 0x48);
    uint32 camera = r_u32(game + 0x4c);
    uint32 context = r_u32(game + 0x138);
    uint32 level = r_u32(scene);
    w_u32(game + 0x65b4, 1);
    G3Function8003A9C0(level, camera, context, frame);
    w_u32(game + 0x65b4, 1);
}

void G3Function800263CC(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800263CC, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 flags = r_u32(game + 0x6834);
    uint32 order_table = r_u32(game + 0x38);
    w_u32(game + 0x65b8, 1);
    if ((flags & 0x1000) == 0)
    {
        uint32 owner = r_u32(0x800a2a68);
        G3Function80042ACC(owner, game, order_table, frame);
        G3Function80075418(game, order_table, frame);
    }
    w_u32(game + 0x65b8, 1);
}

void G3Function80026438(void)
{
    FUNCTION_MARKER(0x80026438, "SLUS_008.06");
}

uint32 G3Function80026440(uint32 game)
{
    FUNCTION_MARKER(0x80026440, "SLUS_008.06");
    sint32 display = r_s16(game + 0xf4);
    uint32 order_table = r_u32(game + 0x38);
    uint32 packet;
    uint32 length;
    if (display < 5 || display == 11 || r_s16(game + 0xf0) == 0)
    {
        uint32 index = r_u32(game + 0x58);
        w_u16(0x800a3d9e, index << 8);
        if (r_s16(game + 0x6786) == 6)
            return 6;
        packet = 0x800a3d94;
        length = 0x03000000;
    }
    else
    {
        if (r_s16(game + 0x6786) == 6)
            return 6;
        packet = 0x800a0c38;
        length = 0x05000000;
    }
    uint32 count = r_u32(game + 0x28);
    uint32 link = r_u32(order_table + (count << 2) - 4);
    w_u32(packet, (link & 0xffffff) | length);
    count = r_u32(game + 0x28);
    uint32 result = order_table + (count << 2);
    w_u32(result - 4, packet & 0xffffff);
    return result;
}

void G3Function8002651C(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002651C, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    w_u32(game + 0x65c8, 1);
    uint32 flags = r_u32(game + 0x6834);
    uint32 order_table = r_u32(game + 0x38);
    uint32 camera = r_u32(game + 0x4c);
    bool render = (flags & 0x80000) != 0;
    if (!render)
    {
        uint32 player = r_u32(0x8009efc4);
        if (r_u32(player + 0x150) == 3)
        {
            uint32 state = r_u32(player + 0x24);
            sint32 height = r_s16(camera + 4);
            sint32 limit = r_s16(state + 0x228);
            render = height < limit;
        }
    }
    if (render)
    {
        uint32 packets = r_u32(game + 0x68);
        G3Function8001E924(packets, order_table, frame);
    }
    w_u32(game + 0x65c8, 1);
}

void G3Function800265AC(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800265AC, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function80025FC4(game);
    if (r_s16(game + 0x6786) == 6)
    {
        G3Function80026128(game, frame);
        G3Function80026440(game);
    }
    else
    {
        G3Function80026018(game, frame);
        G3Function80026090(game, frame);
        G3Function80027F5C();
        G3Function80026128(game, frame);
        G3Function80027F7C();
        G3Function8002616C(game, frame);
        G3Function80026190(game, frame);
        G3Function800261F0(game, frame);
        G3Function80026280(game, frame);
        G3Function800262C8(game, frame);
        G3Function80026384(game, frame);
        G3Function800263CC(game, frame);
        G3Function80026438();
        G3Function80025BA8(game, frame);
        G3Function80026440(game);
        G3Function8002651C(game, frame);
    }
    w_u32(game + 0x6560, 1);
}

void G3Function80025BA8(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80025BA8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    bool active = r_s32(game + 0x67c4) >= 2;
    if (active)
    {
        r_u32(game + 0x4c);
        if (G3Function8004DACC() != 0)
        {
            uint32 level = r_u16(game + 0x68b4);
            active = level == 20 || level == 21;
        }
    }
    if (!active)
    {
        if (r_u32(game + 0x67c4) == 1)
        {
            uint32 owner = r_u32(game + 0x50);
            w_u32(owner + 0x10, r_u32(owner + 0x10) & 0xfffffeff);
            owner = r_u32(game + 0x50);
            w_u32(owner + 0x14, r_u32(owner + 0x14) & 0xffffff7f);
            owner = r_u32(game + 0x50);
            w_u32(owner + 0x14, r_u32(owner + 0x14) & 0xfffffeff);
            owner = r_u32(game + 0x50);
            w_u32(owner + 0x14, r_u32(owner + 0x14) | 0x40);
            uint32 completed = r_u32(game + 0x69f4);
            w_u32(game + 0x67c4, 0);
            ++completed;
            w_u32(game + 0x69f4, completed);
            G3Function8004DF4C(0, completed);
        }
        return;
    }
    uint32 owner = r_u32(game + 0x50);
    uint32 duration = r_u32(game + 0x67c8);
    uint32 state = r_u32(owner + 0x24);
    if (duration == 99)
    {
        w_u32(owner + 0x10, r_u32(owner + 0x10) | 0x800);
        w_u16(state + 0x120, 0);
        w_u32(game + 0x67c4, 0);
        return;
    }
    uint32 flags = r_u32(owner + 0x10);
    uint32 player = r_u32(0x8009efc4);
    w_u32(owner + 0x10, flags | 0x100);
    w_u32(player + 0x14, r_u32(player + 0x14) | 0x180);
    uint32 current = r_u32(game + 0x67c4);
    duration = r_u32(game + 0x67c8);
    if (current == duration)
    {
        owner = r_u32(game + 0x50);
        w_u32(owner + 0x14, r_u32(owner + 0x14) & 0xffffffbf);
        owner = r_u32(game + 0x50);
        w_u32(owner + 0x10, r_u32(owner + 0x10) & 0xfffff7ff);
        w_u16(state + 0x120, 0);
        w_u16(state + 0x1b6, 0);
    }
    if (r_s32(game + 0x67c4) >= 31)
    {
        player = r_u32(0x8009efc4);
        w_u16(frame + 0x18, r_u16(player + 0x58));
        w_u16(frame + 0x1a, r_u16(player + 0x5a));
        w_u16(frame + 0x1c, r_u16(player + 0x5c) + 0x80);
        sint32 count = r_s32(game + 0x67c4);
        w_u32(frame + 0x10, frame + 0x18);
        w_u32(frame + 0x14, 0x80);
        uint32 resource = r_u32(0x800a27fc);
        G3Function800258D0(resource, game, (uint32)(count / 10), 0x80, frame + 0x18, 0x80, frame);
        uint32 global_game = r_u32(0x80097048);
        if ((r_u32(global_game + 0x6834) & 0x10000) == 0 && G3Function8003F0C8(0x81c) == 0)
            G3Function8003F1C4(0x81c, 0x5c, 0x40, 0xfffffed4);
    }
    duration = r_u32(game + 0x67c8);
    current = r_u32(game + 0x67c4);
    if ((sint32)current < 10)
    {
        owner = r_u32(game + 0x50);
        w_u32(owner + 0x14, r_u32(owner + 0x14) & 0xffffff7f);
        owner = r_u32(game + 0x50);
        w_u32(owner + 0x14, r_u32(owner + 0x14) & 0xfffffeff);
    }
    else
    {
        uint32 phase = (sint32)(duration - 10) < (sint32)current ? duration - 10 : current;
        phase -= 10;
        uint32 half = (uint32)((sint32)(duration - 20) / 2);
        sint32 distance = (sint32)(phase - half);
        uint32 amount = distance >= 0 ? half - (uint32)distance : phase;
        uint32 numerator = (amount << 8) - amount;
        uint32 green;
        if (half == 0)
            green = (sint32)numerator < 0 ? 1 : 0xffffffff;
        else if (numerator == 0x80000000 && half == 0xffffffff)
            green = 0x80000000;
        else
            green = (uint32)((sint32)numerator / (sint32)half);
        current = r_u32(game + 0x67c4);
        amount = half - current;
        numerator = (amount << 8) - amount;
        uint32 alpha;
        if (half == 0)
            alpha = (sint32)numerator < 0 ? 1 : 0xffffffff;
        else if (numerator == 0x80000000 && half == 0xffffffff)
            alpha = 0x80000000;
        else
            alpha = (uint32)((sint32)numerator / (sint32)half);
        w_u8(game + 0x67bc, 0);
        w_u8(game + 0x67be, 0);
        w_u8(game + 0x67bd, green);
        w_u8(game + 0x67c0, (sint32)alpha < 0 ? 0 : alpha);
        uint32 value = r_u8(game + 0x67c0);
        w_u8(game + 0x67c1, value);
        w_u8(game + 0x67c2, value);
    }
    if (r_u16(game + 0x68b4) - 23u >= 4 && r_u32(game + 0x69f4) == 0 && r_u8(game + 0x68f8) == 0)
    {
        uint32 level = r_u8(game + 0x68b4);
        sint32 key = r_s16(0x80099708 + level * 28 + 0x18);
        if (key != -1)
        {
            current = r_u32(game + 0x67c4);
            if (current == 10)
                G3Function800359EC((uint32)key, 0, frame);
            else if (current == 6 && r_u32(0x800aa7d4) != 0)
                w_u32(game + 0x67c4, 8);
        }
    }
    if (r_s16(game + 0x6786) == 0)
    {
        current = r_u32(game + 0x67c4);
        uint32 next = current - 1;
        w_u32(game + 0x67c4, next);
        if ((sint32)next >= 4)
            w_u32(game + 0x67c4, current - 2);
    }
    if ((r_u32(game + 0x9c) & 0x4080) != 0)
        w_u32(game + 0x67c4, 1);
}

void G3Function800258D0(uint32 resource, uint32 game, uint32 count, uint32 vertical_range, uint32 position, uint32 horizontal_range, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800258D0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x78;
    uint32 context = G3Function800415CC(resource, frame);
    uint32 descriptor = r_u32(game + 0x68b8);
    if (descriptor == 0 || context == 0 || (sint32)count <= 0)
        return;
    uint32 diameter = vertical_range << 1;
    uint32 index = 0;
    do
    {
        uint32 random = G3SdkRandom80027FA8();
        uint32 horizontal_diameter = horizontal_range << 1;
        uint32 remainder = horizontal_diameter == 0 ? random : (uint32)((sint32)random % (sint32)horizontal_diameter);
        uint32 height_offset = remainder - horizontal_range;
        uint32 magnitude;
        do
        {
            for (uint32 component = 0; component != 3; ++component)
            {
                random = G3SdkRandom80027FA8();
                remainder = diameter == 0 ? random : (uint32)((sint32)random % (sint32)diameter);
                w_u32(frame + 0x28 + component * 4, remainder - vertical_range);
            }
            magnitude = G3Function8002FBF8(frame + 0x28, frame);
        } while (magnitude == 0);
        uint32 x = r_u32(frame + 0x28);
        uint32 y = r_u32(frame + 0x2c);
        uint32 z = r_u32(frame + 0x30);
        uint32 numerators[8];
        numerators[0] = (0u - x) << 9;
        numerators[1] = (0u - y) << 9;
        numerators[2] = (0u - z) << 9;
        numerators[3] = ((0u - y) * 5) << 10;
        numerators[4] = (x * 5) << 10;
        numerators[5] = x * vertical_range;
        numerators[6] = y * vertical_range;
        numerators[7] = z * vertical_range;
        uint32 quotients[8];
        for (uint32 component = 0; component != 8; ++component)
        {
            uint32 numerator = numerators[component];
            quotients[component] = numerator == 0x80000000 && magnitude == 0xffffffff ? 0x80000000 : (uint32)((sint32)numerator / (sint32)magnitude);
        }
        w_u16(frame + 0x4a, quotients[4]);
        w_u32(frame + 0x28, quotients[5]);
        uint32 offset = r_u16(frame + 0x28);
        w_u16(frame + 0x4c, 0);
        w_u16(frame + 0x38, quotients[0]);
        w_u16(frame + 0x3a, quotients[1]);
        w_u16(frame + 0x3c, quotients[2]);
        w_u16(frame + 0x48, quotients[3]);
        w_u32(frame + 0x2c, quotients[6]);
        w_u32(frame + 0x30, quotients[7]);
        uint32 coordinate = r_u16(position);
        w_u16(frame + 0x40, offset + coordinate);
        offset = r_u16(frame + 0x2c);
        coordinate = r_u16(position + 2);
        w_u16(frame + 0x42, offset + coordinate);
        offset = r_u16(frame + 0x30);
        coordinate = r_u16(position + 4);
        w_u16(frame + 0x44, offset + coordinate);
        descriptor = r_u32(game + 0x68b8);
        uint32 table = r_u32(descriptor + 0xc);
        uint32 effect_resource = r_u32(0x800a27fc);
        uint32 owner = r_u32(table);
        w_u32(frame + 0x10, frame + 0x38);
        w_u32(frame + 0x18, 0x80044bc4);
        w_u32(frame + 0x1c, 0x80025644);
        w_u32(frame + 0x14, effect_resource);
        w_u32(frame + 0x20, context);
        w_u32(frame + 0x24, 30);
        uint32 data = r_u32(owner + 0x14);
        uint32 effect = G3Function8004242C(owner, data, frame + 0x40, frame + 0x48, frame);
        if (effect != 0)
        {
            w_u16(effect + 0x44, r_u16(position));
            w_u16(effect + 0x46, r_u16(position + 2));
            w_u16(effect + 0x48, r_u16(position + 4) + height_offset);
        }
        ++index;
    } while ((sint32)index < (sint32)count);
}

void G3Function8004D938(uint32 source)
{
    FUNCTION_MARKER(0x8004D938, "SLUS_008.06");
    uint32 words[2];
    for (uint32 index = 0; index != 2; ++index)
    {
        uint32 address = source + index * 4;
        uint32 high_address = address + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 low_lane = address & 3;
        uint32 low = r_masked_u32(address & 0xfffffffc, (15 << low_lane) & 15);
        words[index] = low_lane == 0 ? low : (high << ((3 - high_lane) * 8)) | (low >> (low_lane * 8));
    }
    w_masked_u32(0x80096b04, words[0] >> 16, 3);
    w_masked_u32(0x80096b00, words[0] << 16, 12);
    w_masked_u32(0x80096b08, words[1] >> 16, 3);
    w_masked_u32(0x80096b04, words[1] << 16, 12);
}

uint32 G3Function8001E140(uint32 color, uint32 x0, uint32 y0, uint32 x1, uint32 y1, uint32 x2, uint32 y2, uint32 x3, uint32 y3, uint32 pool, uint32 order)
{
    FUNCTION_MARKER(0x8001E140, "SLUS_008.06");
    uint32 end = r_u32(pool + 8);
    uint32 packet = r_u32(pool + 4);
    if (packet >= end - 0x30)
        return 0;
    uint32 rgb = r_u32(color);
    w_u16(packet + 8, x0);
    w_u16(packet + 0xa, y0);
    w_u16(packet + 0xc, x1);
    w_u16(packet + 0xe, y1);
    w_u16(packet + 0x10, x2);
    w_u16(packet + 0x12, y2);
    w_u16(packet + 0x14, x3);
    w_u16(packet + 0x16, y3);
    w_u32(packet + 4, rgb);
    w_u8(packet + 7, 0x28);
    uint32 link = r_u32(order);
    w_u32(packet, (link & 0xffffff) | 0x05000000);
    w_u32(order, packet & 0xffffff);
    uint32 next = r_u32(pool + 4) + 0x18;
    uint32 count = r_u32(pool) + 1;
    w_u32(pool + 4, next);
    w_u32(pool, count);
    return next;
}

uint32 G3Function8001E25C(uint32 x0, uint32 y0, uint32 x1, uint32 y1, uint32 x2, uint32 y2, uint32 x3, uint32 y3, uint32 red, uint32 green, uint32 blue, uint32 blend, uint32 pool, uint32 order)
{
    FUNCTION_MARKER(0x8001E25C, "SLUS_008.06");
    uint32 end = r_u32(pool + 8);
    uint32 packet = r_u32(pool + 4);
    if (packet >= end - 0x30)
        return 0;
    uint32 mode = (uint32)((sint32)(blend << 16) >> 11) | 0xe1000208;
    w_u32(packet + 4, mode);
    w_u8(packet + 0xb, 0x2a);
    w_u8(packet + 8, red);
    w_u8(packet + 9, green);
    w_u8(packet + 0xa, blue);
    w_u16(packet + 0xc, x0);
    w_u16(packet + 0xe, y0);
    w_u16(packet + 0x10, x1);
    w_u16(packet + 0x12, y1);
    w_u16(packet + 0x14, x2);
    w_u16(packet + 0x16, y2);
    w_u16(packet + 0x18, x3);
    w_u16(packet + 0x1a, y3);
    uint32 link = r_u32(order);
    w_u32(packet, (link & 0xffffff) | 0x06000000);
    w_u32(order, packet & 0xffffff);
    uint32 next = r_u32(pool + 4) + 0x1c;
    uint32 count = r_u32(pool) + 1;
    w_u32(pool + 4, next);
    w_u32(pool, count);
    return next;
}
