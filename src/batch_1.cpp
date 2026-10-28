#include "xport.h"
#include "psx.h"
#include "psx_spu.h"
#include "game_functions.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>

uint32 G3ClearBss80019440(void)
{
    uint32 address = 0x800a04d8;

    FUNCTION_MARKER(0x80019440, "SLUS_008.06");
    do
    {
        w_u32(address, 0);
        address += 4;
    } while (address < 0x800b339c);
    return address;
}

sint32 G3MemPackInit80048130(void)
{
    FUNCTION_MARKER(0x80048130, "SLUS_008.06");
    G3LevelCodeRegistryReset();
    uint32 free_bytes = 0x801fc000 - 0x800b339c;

    w_u32(0x800a2958, 0x800b339c);
    w_u32(0x800a295c, free_bytes);
    w_u32(0x800a2960, 0);
    w_u32(0x800a2964, 0x800b339c);
    w_u32(0x800a2968, free_bytes);
    {
#ifndef NDEBUG
        return printf("MemPack init: %d free bytes\n", (sint32)free_bytes);
#else
        return 33;
#endif
    }
}

uint32 G3ConsolePut80094E58(uint32 character)
{
    uint32 count;
    uint32 column;

    FUNCTION_MARKER(0x80094e58, "SLUS_008.06");
    if ((character & 0xff) == 9)
    {
        do
        {
            G3ConsolePut80094E58(32);
            column = r_u32(0x8009cba0) & 7;
        } while (column != 0);
        return column;
    }
    if ((character & 0xff) == 10)
    {
        G3ConsolePut80094E58(13);
        w_u32(0x8009cba0, 0);
    }
    else if ((r_u8(0x8009a48d + (character & 0xff)) & 0x97) != 0)
    {
        column = r_u32(0x8009cba0);
        w_u32(0x8009cba0, column + 1);
    }
    count = r_u32(0x8009cba4);
    if ((sint32)count >= 32)
    {
        writePSX(1, psx_addr(0x800b3324, count), count);
        w_u32(0x8009cba4, 0);
    }
    count = r_u32(0x8009cba4);
    w_u8(0x800b3324 + count, character);
    count += 1;
    w_u32(0x8009cba4, count);
    return count;
}

void G3ConsoleFlush80094F54(void)
{
    uint32 count;

    FUNCTION_MARKER(0x80094f54, "SLUS_008.06");
    count = r_u32(0x8009cba4);
    if ((sint32)count > 0)
    {
        writePSX(1, psx_addr(0x800b3324, count), count);
        w_u32(0x8009cba4, 0);
    }
}

uint32 G3CdClearPending80064DE8(void)
{
    FUNCTION_MARKER(0x80064de8, "SLUS_008.06");
    uint32 pending = r_u32(0x800ad170);
    if (pending != 0)
        w_u32(0x800ad170, 0);
    return pending;
}

uint32 G3InitializeState8002DA58(uint32 state)
{
    FUNCTION_MARKER(0x8002da58, "SLUS_008.06");
    w_u32(state, 1);
    w_u32(state + 4, 0);
    w_u32(state + 8, 0xffffffff);
    w_u32(state + 0x10, 0);
    w_u32(state + 0x0c, 0);
    w_u32(state + 0x18, 0);
    return 0xffffffff;
}

uint32 G3InitializeLevel8002DA7C(uint32 configuration, uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002da7c, "SLUS_008.06");
    /* The original overwrites A0 before its first use */
    uint32 frame = guest_stack - 0x18;
    uint32 flags = r_u32(game + 0xe8);
    w_u32(game + 0xe8, flags | 0x40000);
    uint32 level = G3LoadLevel80027348(0x800100e8, 0x800100ec, game, frame);
    w_u16(game + 0x68b4, level);
    w_u8(game + 0x68fb, 1);
    w_u8(game + 0x68e3, 1);
    w_u32(game + 0x68f0, 0xffffffff);
    flags = r_u32(game + 0xe8) | 0x80000;
    w_u32(game + 0xe8, flags);
    return flags;
}

uint32 G3EventCallback8002EBE0(void)
{
    FUNCTION_MARKER(0x8002ebe0, "SLUS_008.06");
    return 0;
}

uint32 G3MemPackPush8004829C(void)
{
    FUNCTION_MARKER(0x8004829c, "SLUS_008.06");
    uint32 depth = r_u32(0x800a2960);
    if ((sint32)depth >= 32)
        return 0;
    uint32 value = r_u32(0x800a2964 + (depth << 3));
    w_u32(0x800a2964 + ((depth + 1) << 3), value);
    depth = r_u32(0x800a2960);
    value = r_u32(0x800a2968 + (depth << 3));
    w_u32(0x800a2968 + ((depth + 1) << 3), value);
    depth = r_u32(0x800a2960) + 1;
    w_u32(0x800a2960, depth);
    return depth;
}

uint32 G3MemPackPop80048320(void)
{
    FUNCTION_MARKER(0x80048320, "SLUS_008.06");
    uint32 depth = r_u32(0x800a2960);
    uint32 result = depth - 1;
    if (depth != 0)
    {
        w_u32(0x800a2960, result);
        G3LevelCodePop(depth);
    }
    return result;
}

uint32 G3MemPackAllocate80048190(uint32 bytes)
{
    FUNCTION_MARKER(0x80048190, "SLUS_008.06");
    /* SRA by two followed by SLL by two clears the low two bits */
    uint32 rounded = (bytes + 3) & 0xfffffffc;
    uint32 depth = r_u32(0x800a2960);
    uint32 allocation = r_u32(0x800a2964 + (depth << 3));
    w_u32(0x800a2964 + (depth << 3), allocation + rounded);
    depth = r_u32(0x800a2960);
    uint32 remaining = r_u32(0x800a2968 + (depth << 3));
    w_u32(0x800a2968 + (depth << 3), remaining - rounded);
    depth = r_u32(0x800a2960);
    remaining = r_u32(0x800a2968 + (depth << 3));
    if ((sint32)remaining < 0)
        {
#ifndef NDEBUG
        printf("ERROR: MemPack - out of memory by %d bytes\n", (sint32)(0 - remaining));
#endif
        }
    w_u32(0x800a2a64, rounded);
    G3LevelCodeInvalidateSpan(allocation, rounded);
    return allocation;
}

uint32 G3Main8002EBE8(uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002ebe8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    G3InitializeStatics8008AED8();
    uint32 first = r_u32(0x800101ac);
    uint32 second = r_u32(0x800101b0);
    uint32 third = r_u32(0x800101b4);
    w_u32(frame + 0x10, first);
    w_u32(frame + 0x14, second);
    w_u32(frame + 0x18, third);
    first = r_u32(0x800101b8);
    second = r_u32(0x800101bc);
    uint32 terminator = r_u8(0x800101c0);
    w_u32(frame + 0x1c, first);
    w_u32(frame + 0x20, second);
    w_u8(frame + 0x24, terminator);
    G3ClearBss80019440();
    w_u8(0x8009704c, 0);
    sint32 event = OpenEvent(0xf0000010, 0x1000, 0x1000, G3NativeMainEvent);
    EnableEvent(event);
    G3MemPackInit80048130();
    G3BigfileInitialize80064900(0x800101c4, frame);
    w_u16(0x800aa66a, 0xff);
    w_u16(0x800aa668, 0xff);
    w_u32(0x800a3e9c, 0);
    w_u32(0x800a3dfc, 0);
    w_u32(0x800a3e08, 0);
    w_u32(0x800a3e0c, 0);
    w_u32(0x800aa5cc, 5);
    G3InitializeLevel8002DA7C(frame + 0x10, 0x800a3db4, frame);
    w_u32(0x800aa594, 0);
    G3InitializeState8002DA58(0x800aa7b0);
    uint32 exiting;
    do
    {
        uint32 state = r_u32(0x800aa7b0);
        uint32 handler = r_u32(0x80097714 + (state << 3));
        G3DispatchMainState(handler, 0x800aa7b0, frame);
        exiting = r_u32(0x800aa7c8);
        w_u32(0x800aa7b4, state);
    } while (exiting == 0);
    G3Shutdown8002EDC0(frame);
    return 0;
}

static char g3_bigfile_path[260];

static bool G3ReadBigfileRequest(uint32 request)
{
    uint64 offset = (uint64)r_u32(request + 0x0c) * 0x800;
    uint64 bytes = (uint64)r_u32(request + 0x10) * 0x800;
    if (g3_bigfile_path[0] == 0 || offset > LONG_MAX || bytes > 0xffffffff)
    {
        {
#ifndef NDEBUG
        fprintf(stderr, "BIGFILE read has an invalid path or range\n");
#endif
        }
        return false;
    }
    FILE *file = xport_fopen(g3_bigfile_path, "rb");
    if (file == 0)
    {
        {
#ifndef NDEBUG
        fprintf(stderr, "BIGFILE open failed: %s\n", g3_bigfile_path);
#endif
        }
        return false;
    }
    long length = -1;
    bool valid = fseek(file, 0, SEEK_END) == 0;
    if (valid)
        length = ftell(file);
    valid = valid && length >= 0 && offset <= (uint64)length && bytes <= (uint64)length - offset;
    if (valid)
        valid = fseek(file, (long)offset, SEEK_SET) == 0;
    size_t received = 0;
    if (valid && bytes != 0)
    {
        uint32 buffer = r_u32(request + 4);
        received = xport_fread(psx_addr(buffer, (size_t)bytes), 1, (size_t)bytes, file);
        valid = received == (size_t)bytes;
#if !defined(XPORT_XCRT)
        valid = valid && ferror(file) == 0;
#endif
    }
    if (fclose(file) != 0)
        valid = false;
    if (!valid)
        {
#ifndef NDEBUG
        fprintf(stderr, "BIGFILE seek/range/read failed: %s, sector %u, sectors %u, received %u\n", g3_bigfile_path, r_u32(request + 0x0c), r_u32(request + 0x10), (uint32)received);
#endif
        }
    return valid;
}

uint32 G3QueueRead800652C8(uint32 sector_offset, uint32 input, uint32 output, uint32 sectors, uint32 option_1c, uint32 option_18, uint32 option_20)
{
    FUNCTION_MARKER(0x800652c8, "SLUS_008.06");
    uint32 index = r_u32(0x800ad16c);
    uint32 request = 0x800ad0d4 + ((index << 3) + index) * 4;
    while (r_u32(request + 0x14) != 0)
        G3CdPump80064F44();
    // CdlLOC bytes are unused by the native file boundary
    w_u32(request, 0);
    w_u32(request + 0x10, sectors);
    w_u32(request + 4, input);
    w_u32(request + 8, output);
    w_u32(request + 0x0c, sector_offset);
    w_u32(request + 0x14, 1);
    w_u32(request + 0x1c, option_1c);
    w_u32(request + 0x18, option_18);
    w_u32(request + 0x20, option_20);
    index = r_u32(0x800ad16c);
    w_u32(0x800ad16c, (index + 1) & 3);
    G3CdPump80064F44();
    return index;
}

uint32 G3BigfileInitialize80064900(uint32 filename, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80064900, "SLUS_008.06");
    uint32 frame = guest_stack - 0x50;
    g3_bigfile_path[0] = 0;
    char path[260] = "DATA/";
    uint32 input = r_u8(filename) == '\\' ? 1 : 0;
    uint32 output = 5;
    for (;; ++input)
    {
        uint32 value = r_u8(filename + input);
        if (value == 0 || value == ';')
            break;
        if (output + 1 >= sizeof(path))
            {
#ifndef NDEBUG
        return (uint32)printf("BIGFILE path is too long\n");
#else
        return 25;
#endif
    }
        path[output++] = value == '\\' ? '/' : (char)value;
    }
    path[output] = 0;
    size_t file_bytes = 0;
    if (!xport_file_size(path, &file_bytes))
        {
#ifndef NDEBUG
        return (uint32)printf("CD ERROR: bigfile '%s' not found\n", path);
#else
        return (uint32)strlen(path) + 31;
#endif
    }
    if (file_bytes < 0x2000)
        {
#ifndef NDEBUG
        return (uint32)printf("BIGFILE directory is truncated: %s\n", path);
#else
        return (uint32)strlen(path) + 33;
#endif
    }
    memcpy(g3_bigfile_path, path, output + 1);
    w_u32(0x800ad174, 0);
    w_u32(0x800ad170, 0);
    // Archive offsets are relative to the extracted file
    w_u32(0x800ad1ac, 0);
    uint32 request = 0x800ad0d4;
    for (uint32 index = 0; index < 4; ++index)
    {
        w_u32(request + 0x14, 0);
        request += 0x24;
    }
    w_u32(0x800ad164, 0x800ad0d4);
    w_u32(0x800ad168, 0);
    w_u32(0x800ad16c, 0);
    G3MemPackPush8004829C();
    uint32 buffer = G3MemPackAllocate80048190(0x2000);
    w_u32(frame + 0x10, 0);
    w_u32(frame + 0x14, 0);
    w_u32(frame + 0x18, 0);
    uint32 queued = G3QueueRead800652C8(0, buffer, buffer, 4, 0, 0, 0);
    request = 0x800ad0d4 + ((queued << 3) + queued) * 4;
    while (r_u32(request + 0x14) != 0)
        G3CdPump80064F44();
    G3MemPackPop80048320();
    uint32 entries = r_u32(buffer);
    if (entries > (0x2000 - 4) / 24)
        {
#ifndef NDEBUG
        return (uint32)printf("BIGFILE directory count exceeds its read buffer\n");
#else
        return 48;
#endif
    }
    uint32 bytes = ((entries << 1) + entries) << 3;
    w_u32(0x800ad1a8, entries);
    return memmovePSX(0x800ad1b0, buffer + 4, (sint32)bytes);
}



uint32 G3CdChecksum80064EA8(uint32 request, uint32 sectors)
{
    FUNCTION_MARKER(0x80064ea8, "SLUS_008.06");
    uint32 mode = r_u32(request + 0x1c);
    uint32 count;
    uint32 stride;
    if (mode == 1)
    {
        count = sectors << 9;
        stride = 4;
    }
    else if (mode == 2)
    {
        count = sectors;
        stride = 0x800;
    }
    else
        return 2;
    if ((sint32)count <= 0)
        return 2;
    uint32 index = 0;
    do
    {
        ++index;
        uint32 cursor = r_u32(0x800ad1a4);
        uint32 sum = r_u32(0x800ad1a0);
        uint32 word = r_u32(cursor);
        w_u32(0x800ad1a4, cursor + stride);
        w_u32(0x800ad1a0, sum + word);
    } while ((sint32)index < (sint32)count);
    return 0;
}

uint32 G3CdPump80064F44(void)
{
    FUNCTION_MARKER(0x80064f44, "SLUS_008.06");
    uint32 request = r_u32(0x800ad164);
    if (r_u32(request + 0x14) == 3)
    {
        uint32 total = r_u32(request + 0x10);
        if (r_u32(0x800ad180) != total)
            return 0;
        uint32 sectors = r_u32(0x800ad17c);
        if (sectors != 0)
        {
            uint32 cursor = r_u32(0x800ad178);
            w_u32(0x800ad17c, 0);
            w_u32(0x800ad178, cursor + (sectors << 11));
            if (r_u32(request + 0x1c) != 0)
                G3CdChecksum80064EA8(request, sectors);
            if (r_u32(request + 0x20) != 0)
            {
                uint32 pending = r_u32(0x800ad198);
                uint32 limit = r_u32(0x800ad19c);
                uint32 bytes = (sectors << 11) + pending;
                if ((sint32)limit < (sint32)bytes)
                    bytes = limit;
                uint32 residue = G3UnpackStream80065554(bytes, 4);
                limit = r_u32(0x800ad19c);
                w_u32(0x800ad198, residue);
                w_u32(0x800ad19c, limit + residue - bytes);
            }
        }
        uint32 pending = r_u32(0x800ad198);
        if (pending != 0)
            w_u32(0x800ad198, G3UnpackStream80065554(pending, 1));
        bool checksum_ok = true;
        if (r_u32(request + 0x1c) != 0)
        {
            uint32 sum = r_u32(0x800ad1a0);
            checksum_ok = sum == r_u32(request + 0x18);
        }
        if (checksum_ok)
        {
            w_u32(request + 0x14, 0);
            uint32 index = r_u32(0x800ad168);
            w_u32(0x800ad174, 0);
            index = (index + 1) & 3;
            request = 0x800ad0d4 + ((index << 3) + index) * 4;
            w_u32(0x800ad168, index);
            w_u32(0x800ad164, request);
        }
        else
        {
            {
#ifndef NDEBUG
            printf("CD ERROR: checksum error\n");
#endif
            }
            w_u32(request + 0x14, 4);
        }
    }
    else if (r_u32(request + 0x14) == 1)
    {
        w_u32(request + 0x14, 3);
        w_u32(0x800ad180, 0);
        uint32 sectors = r_u32(request + 0x10);
        uint32 buffer = r_u32(request + 4);
        w_u32(0x800ad174, sectors == 0 ? 0 : buffer + (sectors << 11) - 4);
        if (r_u32(request + 0x20) != 0)
        {
            buffer = r_u32(request + 4);
            w_u32(0x800ad188, buffer);
            uint32 output = r_u32(request + 8);
            w_u32(0x800ad190, 0);
            w_u32(0x800ad198, 0);
            w_u32(0x800ad18c, output);
            uint32 bytes = r_u32(request + 0x20);
            w_u32(0x800ad19c, bytes);
        }
        w_u32(0x800ad1a0, 0);
        buffer = r_u32(request + 4);
        w_u32(0x800ad1a4, buffer);
        sectors = r_u32(request + 0x10);
        w_u32(0x800ad17c, sectors);
        w_u32(0x800ad170, 1);
        w_u32(0x800ad178, buffer + 0x7fc);
        G3CdClearPending80064DE8();
        // Publish received sectors only after the complete file range arrives
        if (G3ReadBigfileRequest(request))
            w_u32(0x800ad180, r_u32(request + 0x10));
        else
            w_u32(request + 0x14, 4);
    }
    if (r_u32(request + 0x14) == 4)
    {
        {
#ifndef NDEBUG
        printf("CD ERROR: seek/read/checksum error\n");
#endif
        }
        w_u32(request + 0x14, 1);
        return 1;
    }
    return r_u32(request + 0x14) == 3 ? 0 : 0x800b0000;
}

uint32 G3ElapsedMicroseconds8003A3E4(uint32 timestamp)
{
    FUNCTION_MARKER(0x8003a3e4, "SLUS_008.06");
    if (r_u32(0x8009db68) == 0)
        return 0;
    uint32 low = G3ReadRootCounter800895D4(0xf2000000) & 0xffff;
    uint32 high = r_u32(0x80096b74) & 0xffff;
    uint32 old_high = timestamp >> 16;
    uint32 high_delta;
    if (high < old_high)
        high_delta = high + 0x10000 - old_high;
    else
        high_delta = high - old_high;
    uint32 old_low = timestamp & 0xffff;
    uint32 low_delta;
    if (low < old_low)
    {
        low_delta = low + 0xffff - old_low;
        --high_delta;
    }
    else
        low_delta = low - old_low;
    if (high_delta >= 0x11a5)
        return 0x41828f;
    uint32 low_scaled = (low_delta << 3) - low_delta;
    uint32 high_scaled = (high_delta << 3) - high_delta;
    uint32 ticks = (((high_scaled << 16) - high_scaled) + low_scaled) << 1;
    uint64 product = (uint64)ticks * 0x10624dd3;
    return (uint32)(product >> 38);
}

uint32 G3UnpackStream80065554(uint32 bytes, uint32 margin)
{
    FUNCTION_MARKER(0x80065554, "SLUS_008.06");
    uint32 input = r_u32(0x800ad188);
    uint32 output = r_u32(0x800ad18c);
    uint32 bits = r_u32(0x800ad190);
    uint32 flags = r_u32(0x800ad194);
    uint32 end = input + bytes;
    uint32 limit = end - margin;
    if (limit >= input)
    {
        do
        {
            if (bits == 0)
            {
                uint32 low = r_u8(input);
                ++input;
                uint32 high = r_u8(input);
                ++input;
                bits = 16;
                flags = low | (high << 8);
            }
            if ((flags & 1) != 0)
            {
                uint32 header = r_u8(input);
                ++input;
                uint32 low_offset = r_u8(input);
                ++input;
                uint32 distance = ((header & 0xf0) << 4) + low_offset;
                uint32 source = output - distance;
                uint32 copies = (header & 0x0f) + 1;
                do
                {
                    uint32 value = r_u8(source);
                    ++source;
                    w_u8(output, value);
                    ++output;
                    --copies;
                } while (copies != 0);
            }
            else
            {
                uint32 value = r_u8(input);
                ++input;
                w_u8(output, value);
                ++output;
            }
            flags >>= 1;
            --bits;
        } while (limit >= input);
    }
    uint32 residue = end - input;
    w_u32(0x800ad188, input);
    w_u32(0x800ad18c, output);
    w_u32(0x800ad190, bits);
    w_u32(0x800ad194, flags);
    if ((sint32)residue < 0)
        return 0;
    return residue;
}

void G3SetRandomState80027F9C(uint32 state)
{
    FUNCTION_MARKER(0x80027f9c, "SLUS_008.06");
    w_u32(0x8009d824, state);
}

uint32 G3PresentDisplay8002DAEC(void)
{
    FUNCTION_MARKER(0x8002daec, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 index = r_u32(game + 0x54);
    uint32 offset = ((((index << 1) + index) << 3) - index) << 2;
    G3PutDrawEnv80086850(0x800a3cd4 + offset);
    G3DrawPrimitive80086784(0x800a3da4);
    (uint32)DrawSync(0);
    game = r_u32(0x80097048);
    index = r_u32(game + 0x54);
    offset = ((index << 2) + index) << 2;
    G3PutDispEnv80086910(0x800a3cac + offset);
    return G3SetDisplayMask80086314(1);
}

uint32 G3InitializeDisplay8002DB7C(uint32 width, uint32 height)
{
    FUNCTION_MARKER(0x8002db7c, "SLUS_008.06");
    ResetGraph(0);
    SetGraphDebug(0);
    SetDefDrawEnv((DRAWENV *)psx_addr(0x800a3cd4, 28), 0, 0, (sint32)width, (sint32)height);
    SetDefDispEnv((DISPENV *)psx_addr(0x800a3cac, 20), 0, 0, (sint32)width, (sint32)height);
    SetDefDrawEnv((DRAWENV *)psx_addr(0x800a3d30, 28), 0, 0x100, (sint32)width, (sint32)height);
    SetDefDispEnv((DISPENV *)psx_addr(0x800a3cc0, 20), 0, 0x100, (sint32)width, (sint32)height);
    w_u8(0x800a3d46, 1);
    w_u8(0x800a3cea, 1);
    w_u8(0x800a3d47, 1);
    w_u8(0x800a3ceb, 1);
    w_u8(0x800a3d48, 0);
    w_u8(0x800a3cec, 0);
    w_u8(0x800a3d97, 3);
    w_u8(0x800a3d9b, 2);
    w_u16(0x800a3d9c, 0);
    w_u16(0x800a3d9e, 0);
    w_u16(0x800a3da0, 0x200);
    w_u16(0x800a3da2, 0xf0);
    w_u8(0x800a3d98, 0);
    w_u8(0x800a3d99, 0);
    w_u8(0x800a3d9a, 0);
    w_u8(0x800a3da7, 3);
    w_u8(0x800a3dab, 0x60);
    w_u16(0x800a3dac, 0);
    w_u16(0x800a3dae, 0);
    w_u16(0x800a3db0, 0x200);
    w_u16(0x800a3db2, 0xf0);
    w_u8(0x800a3da8, 0);
    w_u8(0x800a3da9, 0);
    w_u8(0x800a3daa, 0);
    return G3PresentDisplay8002DAEC();
}

uint32 G3StateStartup8002E220(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002e220, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    for (uint32 group = 0; group < 2; ++group)
    {
        uint32 words[3];
        uint32 first_word = group == 0 ? 0 : 3;
        uint32 count = group == 0 ? 3 : 2;
        for (uint32 index = 0; index < count; ++index)
        {
            uint32 source = 0x80010104 + (first_word + index) * 4;
            uint32 upper = r_u32((source + 3) & ~uint32(3));
            uint32 lower = r_u32(source & ~uint32(3));
            uint32 shift = (source & 3) << 3;
            words[index] = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
        }
        uint32 tail = 0;
        if (group != 0)
            tail = (uint32)(sint32)r_s8(0x80010118);
        for (uint32 index = 0; index < count; ++index)
        {
            uint32 destination = frame + 0x10 + (first_word + index) * 4;
            uint32 high_address = destination + 3;
            uint32 high_byte = high_address & 3;
            uint32 preserve = high_byte == 3 ? 0 : uint32(0xffffffff) << ((high_byte + 1) << 3);
            uint32 previous = r_u32(high_address & ~uint32(3));
            w_u32(high_address & ~uint32(3), (previous & preserve) | (words[index] >> ((3 - high_byte) << 3)));
            uint32 low_byte = destination & 3;
            preserve = low_byte == 0 ? 0 : uint32(0xffffffff) >> ((4 - low_byte) << 3);
            previous = r_u32(destination & ~uint32(3));
            w_u32(destination & ~uint32(3), (previous & preserve) | (words[index] << (low_byte << 3)));
        }
        if (group != 0)
            w_u8(frame + 0x24, tail);
    }
    uint32 first = (uint32)(sint32)r_s8(0x80010119);
    uint32 second = (uint32)(sint32)r_s8(0x8001011a);
    w_u8(frame + 0x25, first);
    w_u8(frame + 0x26, second);
    G3InitializeDisplay8002DB7C(0x200, 0xf0);
    InitGeom();
    SetGeomOffset(0x100, 0x78);
    SetGeomScreen(0x140);
    G3InitializeClock8002DCC4();
    G3Function80020A04();
    uint32 object = G3LoadObject8002C760(frame + 0x10, frame);
    uint32 owner = r_u32(0x8009efc4);
    w_u32(owner + 0x1c, object);
    VSyncCallback(G3VBlankCallback);
    DrawSyncCallback(G3DrawCompleteCallback);
    G3Function80028498();
    uint32 game = r_u32(0x80097048);
    if ((r_u32(game + 0x69b8) & 0x1000) == 0)
    {
        G3Function8003EB30(frame);
        G3Function80035348();
    }
    game = r_u32(0x80097048);
    G3Function8002E15C(game);
    G3SetRandomState80027F9C(0);
    w_u32(state + 8, 0);
    w_u32(state, 6);
    return 6;
}

uint32 G3InitializeClock8002DCC4(void)
{
    FUNCTION_MARKER(0x8002dcc4, "SLUS_008.06");
    G3EnterCritical8008A05C();
    sint32 event = OpenEvent(0xf2000000, 2, 0x1000, G3NativeClockEvent);
    w_u32(0x800a3d90, (uint32)event);
    EnableEvent(event);
    G3SetRootCounter80089538(0xf2000000, 0xffff, 0x1001);
    G3StartRootCounter8008960C(0xf2000000);
    G3ExitCritical8008A03C();
    w_u32(0x8009db68, 1);
    return 1;
}

uint32 G3OnVSync8002DD34(void)
{
    FUNCTION_MARKER(0x8002dd34, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 frames = r_u32(game + 0x150);
    w_u32(game + 0x150, frames + 1);
    uint32 ticks = r_u32(game + 0x154);
    uint32 environment = r_u32(game + 0x13c);
    uint32 result = ticks + 1;
    w_u32(game + 0x154, result);
    if (environment != 0)
    {
        frames = r_u32(game + 0x150);
        result = frames < 2;
        if (result == 0)
        {
            G3PutDispEnv80086910(environment);
            game = r_u32(0x80097048);
            w_u32(game + 0x13c, 0);
            w_u32(game + 0x150, 0);
            result = game;
        }
    }
    return result;
}

uint32 G3OnDrawComplete8002DDA8(void)
{
    FUNCTION_MARKER(0x8002dda8, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 destination = r_u32(game + 0x140);
    if (destination == 0)
        return 0;
    uint32 timestamp = r_u32(game + 0x144);
    uint32 elapsed = G3ElapsedDrawTime8003A2F8(timestamp);
    game = r_u32(0x80097048);
    destination = r_u32(game + 0x140);
    w_u32(destination, elapsed);
    game = r_u32(0x80097048);
    uint32 index = r_u32(game + 0x54);
    w_u32(game + 0x140, 0);
    uint32 offset = ((index << 2) + index) << 2;
    uint32 base = r_u32(game + 0x148);
    w_u32(game + 0x13c, base + offset);
    return offset;
}

void G3OnRootCounter8001947C(void)
{
    FUNCTION_MARKER(0x8001947c, "SLUS_008.06");
    uint32 high = r_u32(0x80096b74);
    if (high == 0x7fffffff)
    {
        xport_mips_overflow_exception(0x80019498);
        return;
    }
    w_u32(0x80096b74, (high + 1) & 0xffff);
}

uint32 G3ElapsedDrawTime8003A2F8(uint32 timestamp)
{
    FUNCTION_MARKER(0x8003a2f8, "SLUS_008.06");
    uint32 low = G3ReadRootCounter800895D4(0xf2000000) & 0xffff;
    uint32 old_high = timestamp >> 16;
    uint32 high = r_u32(0x80096b74) & 0xffff;
    uint32 high_delta;
    if (high < old_high)
        high_delta = high + 0x10000 - old_high;
    else
        high_delta = high - old_high;
    uint32 old_low = timestamp & 0xffff;
    uint32 low_delta;
    if (low < old_low)
    {
        low_delta = low + 0xffff - old_low;
        --high_delta;
    }
    else
        low_delta = low - old_low;
    uint32 result;
    if (high_delta >= 0x8d3)
        result = 0x41828f;
    else
    {
        uint32 low_scaled = (((low_delta << 3) - low_delta) << 2) + low_delta;
        uint32 high_scaled = (((high_delta << 3) - high_delta) << 2) + high_delta;
        uint32 ticks = low_scaled + (high_scaled << 16) - high_scaled;
        uint64 product = (uint64)ticks * 0x10624dd3;
        result = (uint32)(product >> 38);
    }
    if (r_u32(0x8009db68) == 0)
        return 0;
    return result;
}

uint32 G3Function8002E15C(uint32 game)
{
    FUNCTION_MARKER(0x8002e15c, "SLUS_008.06");
    uint32 context_bytes = G3GetSaveContextSize800697D0();
    uint32 card_bytes = G3GetMemoryCardSize8006898C();
    G3MemPackPush8004829C();
    uint32 context = G3MemPackAllocate80048190(context_bytes);
    uint32 card = G3MemPackAllocate80048190(card_bytes);
    if (context != 0 && card != 0)
    {
        w_u32(game + 0x67e4, context);
        w_u32(game + 0x67e8, card);
        G3InitializeSaveContext800697D8(context, game);
        G3InitializeMemoryCard80068A7C(card, game);
        uint32 bytes = G3GetSaveBufferSize80068B48(card);
        uint32 buffer = G3MemPackAllocate80048190(bytes);
        if (buffer != 0)
        {
            G3BindSaveBuffer80068B88(card, buffer);
            return buffer;
        }
    }
    uint32 result = G3MemPackPop80048320();
    w_u32(game + 0x67e8, 0);
    w_u32(game + 0x67e4, 0);
    return result;
}

uint32 G3GetSaveContextSize800697D0(void)
{
    FUNCTION_MARKER(0x800697d0, "SLUS_008.06");
    return 0xb8;
}

uint32 G3GetMemoryCardSize8006898C(void)
{
    FUNCTION_MARKER(0x8006898c, "SLUS_008.06");
    return G3GetCardCoreSize80066E98() + 0x4c;
}

uint32 G3InitializeSaveContext800697D8(uint32 context, uint32 game)
{
    FUNCTION_MARKER(0x800697d8, "SLUS_008.06");
    if (context != 0)
    {
        w_u32(context, game);
        w_u32(context + 4, 0xffffffff);
    }
    return 0xffffffff;
}

uint32 G3GetSaveBufferSize80068B48(uint32 card)
{
    FUNCTION_MARKER(0x80068b48, "SLUS_008.06");
    if (card == 0)
        return 0;
    uint32 core = r_u32(card);
    if (core == 0)
        return 0;
    return G3GetCardBufferSize800675D4(core);
}

void G3BindSaveBuffer80068B88(uint32 card, uint32 buffer)
{
    FUNCTION_MARKER(0x80068b88, "SLUS_008.06");
    if (card != 0 && buffer != 0)
        w_u32(card + 4, buffer);
}

uint32 G3GetCardCoreSize80066E98(void)
{
    FUNCTION_MARKER(0x80066e98, "SLUS_008.06");
    uint32 first = G3Function80067940();
    uint32 second = G3Function80067C7C();
    uint32 third = G3Function80067F64();
    return second + 0x2c + first + third;
}

uint32 G3GetCardBufferSize800675D4(uint32 core)
{
    FUNCTION_MARKER(0x800675d4, "SLUS_008.06");
    return G3Function80067070(core);
}

uint32 G3Function80067940(void)
{
    FUNCTION_MARKER(0x80067940, "SLUS_008.06");
    return 0x84;
}

uint32 G3Function80067C7C(void)
{
    FUNCTION_MARKER(0x80067c7c, "SLUS_008.06");
    return 0x1c;
}

uint32 G3Function80067F64(void)
{
    FUNCTION_MARKER(0x80067f64, "SLUS_008.06");
    return 0x3c;
}

uint32 G3SaveDataSize8006B2F4(void)
{
    FUNCTION_MARKER(0x8006b2f4, "SLUS_008.06");
    return 0xe0;
}

uint32 G3ResetMemoryCard80068A60(uint32 card)
{
    FUNCTION_MARKER(0x80068a60, "SLUS_008.06");
    w_u32(card + 8, 0);
    w_u32(card + 0x0c, 0);
    w_u32(card + 0x38, 0xffffffff);
    w_u32(card + 0x3c, 0);
    w_u32(card + 0x40, 0);
    return 0xffffffff;
}

uint32 G3InitializeMemoryCard80068A7C(uint32 card, uint32 game)
{
    FUNCTION_MARKER(0x80068a7c, "SLUS_008.06");
    uint32 bytes = G3SaveDataSize8006B2F4();
    if (card == 0)
        return bytes;
    uint32 core = card + 0x4c;
    uint32 title = r_u32(0x8009ff90);
    uint32 option5 = r_u32(0x8009ffa8);
    uint32 option6 = r_u32(0x8009ffac);
    uint32 option14 = r_u32(0x8009ffb0);
    uint32 serial = r_u32(0x8009ffa0);
    uint32 option4 = r_u32(0x8009ffa4);
    G3ConfigureCardCore80066EDC(core, title, serial, option4, option5, option6, 0x800a0034, 0x8009ffb4, bytes, 0x8006b3a0, 0x8006b58c, 0x8006b77c, card, option14);
    w_u32(card + 0x48, game);
    w_u32(card, core);
    w_u32(card + 4, 0);
    w_u32(card + 0x30, 0);
    return G3ResetMemoryCard80068A60(card);
}

uint32 G3ConfigureCardCore80066EDC(uint32 core, uint32 title, uint32 serial, uint32 option4, uint32 option5, uint32 option6, uint32 palette, uint32 pixels, uint32 data_bytes, uint32 callback1, uint32 callback2, uint32 callback3, uint32 owner, uint32 option14)
{
    FUNCTION_MARKER(0x80066edc, "SLUS_008.06");
    uint32 header = core + 0x2c;
    uint32 header_bytes = G3Function80067940();
    uint32 data = header + header_bytes;
    uint32 data_size = G3Function80067C7C();
    uint32 io = data + data_size;
    G3FillMemory80086094(core, 0, 0x2c);
    w_u32(core + 4, 6);
    w_u32(core + 8, 1);
    w_u32(core + 0x20, header);
    w_u32(core + 0x24, data);
    w_u32(core + 0x1c, io);
    w_u32(core + 0x0c, option14);
    G3InitializeCardHeader80067948(header, title, serial, option4, option5, option6, palette, pixels);
    G3InitializeCardData80067C84(data, data_bytes, callback1, callback2, callback3, owner);
    G3InitializeCardIo80067F6C(io, 0x80067020, core);
    w_u32(core + 0x28, 0);
    uint32 flags = r_u32(core) | 1;
    w_u32(core, flags);
    return flags;
}

uint32 G3InitializeCardHeader80067948(uint32 header, uint32 title, uint32 serial, uint32 option4, uint32 option5, uint32 option6, uint32 palette, uint32 pixels)
{
    FUNCTION_MARKER(0x80067948, "SLUS_008.06");
    G3FillMemory80086094(header, 0, 0x84);
    w_u8(header + 4, 0x42);
    uint32 code = G3CardBlockCode80067924(option4);
    w_u8(header + 5, code);
    G3CopyString8008935C(header + 6, serial);
    w_u8(header + 0x1c, 0x53);
    w_u8(header + 0x1d, 0x43);
    w_u8(header + 0x1e, option6 + 0x10);
    w_u8(header + 0x1f, option5);
    G3EncodeCardTitle80068920(title, header + 0x20);
    w_u32(header + 0x7c, palette);
    w_u32(header + 0x80, pixels);
    uint32 flags = r_u32(header) | 1;
    w_u32(header, flags);
    return flags;
}

uint32 G3InitializeCardData80067C84(uint32 data, uint32 bytes, uint32 callback1, uint32 callback2, uint32 callback3, uint32 owner)
{
    FUNCTION_MARKER(0x80067c84, "SLUS_008.06");
    uint32 rounded = ((bytes + 0x83) >> 7) << 7;
    G3FillMemory80086094(data, 0, 0x1c);
    w_u32(data + 4, bytes);
    w_u32(data + 8, rounded);
    w_u32(data + 0x0c, callback1);
    w_u32(data + 0x10, callback2);
    w_u32(data + 0x14, callback3);
    w_u32(data + 0x18, owner);
    uint32 flags = r_u32(data) | 1;
    w_u32(data, flags);
    return flags;
}

uint32 G3InitializeCardIo80067F6C(uint32 io, uint32 callback, uint32 owner)
{
    G3FillMemory80086094(io, 0, 0x3c);
    w_u32(io + 4, 1);
    w_u32(io + 8, 1);
    w_u32(io + 0x10, callback);
    w_u32(io + 0x14, owner);
    uint32 flags = r_u32(io) | 1;
    w_u32(io, flags);
    return flags;
}

uint32 G3CardBlockCode80067924(uint32 blocks)
{
    FUNCTION_MARKER(0x80067924, "SLUS_008.06");
    return r_u8(0x80099dcc + (blocks << 3));
}

uint32 G3EncodeCardTitle80068920(uint32 source, uint32 destination)
{
    FUNCTION_MARKER(0x80068920, "SLUS_008.06");
    for (;;)
    {
        uint32 character = r_u8(source);
        ++source;
        if (character == 0)
            break;
        uint32 code = G3CardCharacter8006883C(character);
        uint32 swapped = ((code & 0xff00) >> 8) | ((code & 0xff) << 8);
        w_u16(destination, swapped);
        destination += 2;
    }
    w_u16(destination, 0);
    return 0;
}

uint32 G3CardCharacter8006883C(uint32 character)
{
    FUNCTION_MARKER(0x8006883c, "SLUS_008.06");
    character &= 0xff;
    if (character < 0x20 || character >= 0x7f)
        return 0;
    uint32 group = 0;
    uint32 offset = 0;
    if (character < 0x30)
        offset = 1;
    else if (character < 0x3a)
        group = 0;
    else if (character < 0x41)
        offset = 0x0b;
    else if (character < 0x5b)
        group = 1;
    else if (character < 0x61)
        offset = 0x25;
    else if (character < 0x7b)
        group = 2;
    else
        offset = 0x3f;
    if (offset != 0)
        return r_u16(0x80099de8 + ((character - (offset + 0x1f)) << 1));
    uint32 table = 0x8009ff6c + (group << 2);
    uint32 base = r_u16(table);
    uint32 first = r_u8(table + 2);
    return (base + character - first) & 0xffff;
}

uint32 G3Function80067070(uint32 core)
{
    FUNCTION_MARKER(0x80067070, "SLUS_008.06");
    uint32 header = r_u32(core + 0x20);
    uint32 header_bytes = G3HeaderBytes80067A3C(header);
    uint32 data = r_u32(core + 0x24);
    uint32 data_bytes = G3CardDataBytes80067D28(data);
    uint32 copies = r_u32(core + 0x0c);
    return header_bytes + copies * data_bytes;
}

uint32 G3HeaderBytes80067A3C(uint32 header)
{
    FUNCTION_MARKER(0x80067a3c, "SLUS_008.06");
    return (r_u8(header + 0x1e) << 7) - 0x780;
}

uint32 G3CardDataBytes80067D28(uint32 data)
{
    FUNCTION_MARKER(0x80067d28, "SLUS_008.06");
    return r_u32(data + 8);
}

/* Native copy helper stages each original sixteen-byte transfer group */
static void G3CopyCardChunk(uint32 destination, uint32 source, bool paired)
{
    uint32 chunk[4];
    for (uint32 index = 0; index < 4; ++index)
    {
        uint32 address = source + index * 4;
        if (!paired)
            chunk[index] = r_u32(address);
        else
        {
            uint32 high_address = address + 3;
            uint32 high_offset = high_address & 3;
            uint32 offset = address & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_offset + 1)) - 1);
            uint32 low = r_masked_u32(address & 0xfffffffc, (0x0f << offset) & 0x0f);
            chunk[index] = offset == 0 ? low : (high << ((4 - offset) * 8)) | (low >> (offset * 8));
        }
    }
    for (uint32 index = 0; index < 4; ++index)
    {
        uint32 address = destination + index * 4;
        if (!paired)
            w_u32(address, chunk[index]);
        else
        {
            uint32 high_address = address + 3;
            uint32 high_offset = high_address & 3;
            uint32 offset = address & 3;
            w_masked_u32(high_address & 0xfffffffc, chunk[index] >> ((3 - high_offset) * 8), (1 << (high_offset + 1)) - 1);
            w_masked_u32(address & 0xfffffffc, chunk[index] << (offset * 8), (0x0f << offset) & 0x0f);
        }
    }
}

uint32 G3WriteCardHeader80067A50(uint32 header, uint32 buffer)
{
    FUNCTION_MARKER(0x80067a50, "SLUS_008.06");
    uint32 frame_output = buffer + 0x80;
    uint32 source = header + 0x1c;
    uint32 frames = r_u8(header + 0x1e) - 0x10;
    bool paired = ((source | buffer) & 3) != 0;
    for (uint32 offset = 0; offset < 0x60; offset += 0x10)
        G3CopyCardChunk(buffer + offset, source + offset, paired);
    uint32 palette = r_u32(header + 0x7c);
    G3CopyCardChunk(buffer + 0x60, palette, true);
    G3CopyCardChunk(buffer + 0x70, palette + 0x10, true);
    if ((sint32)frames <= 0)
        return palette;
    for (uint32 frame = 0; (sint32)frame < (sint32)frames; ++frame)
    {
        uint32 pixels = r_u32(header + 0x80);
        paired = ((pixels | frame_output) & 3) != 0;
        for (uint32 offset = 0; offset < 0x80; offset += 0x10)
            G3CopyCardChunk(frame_output + offset, pixels + offset, paired);
        w_u32(header + 0x80, pixels + 0x80);
        frame_output += 0x80;
    }
    return 0;
}

uint32 G3CardChecksum80067C48(uint32 buffer, uint32 bytes)
{
    FUNCTION_MARKER(0x80067c48, "SLUS_008.06");
    uint32 sum = 0;
    if ((sint32)bytes > 0)
    {
        uint32 index = 0;
        do
        {
            uint32 value = r_u8(buffer + index);
            ++index;
            sum += value;
        } while ((sint32)index < (sint32)bytes);
    }
    return ((sum & 0xffff) ^ 0x8000) - 0x8000;
}

uint32 G3ValidateCardData80067D34(uint32 data, uint32 buffer)
{
    FUNCTION_MARKER(0x80067d34, "SLUS_008.06");
    if (r_u16(buffer) == 0)
        return 0;
    uint32 bytes = r_u32(data + 4);
    uint32 checksum = G3CardChecksum80067C48(buffer + 4, bytes);
    sint32 expected = r_s16(buffer + 2);
    if ((sint32)checksum != expected)
        return 0;
    return r_u16(buffer);
}

uint32 G3StateRun8002E370(uint32 state, uint32 incoming_second_glyph, uint32 incoming_third_glyph, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002e370, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 game = r_u32(0x80097048);
    G3Function80027B20(game, incoming_second_glyph, incoming_third_glyph, frame);
    G3Function80035AD4(frame);
    game = r_u32(0x80097048);
    if (r_s16(game + 0x6780) == 0)
        return 0;
    sint32 mode = r_s16(game + 0x6786);
    if (mode != 0 && mode != 8 && mode != 9)
        return 9;
    game = r_u32(0x80097048);
    w_u16(game + 0x6786, 0);
    G3Function8002DE88(game, frame);
    G3Function8003EF1C();
    G3MemPackPop80048320();
    game = r_u32(0x80097048);
    if (r_s16(game + 0x6780) == 2)
    {
        w_u32(state, 6);
        w_u32(state + 8, 0x10);
        return 0x10;
    }
    w_u32(state, 3);
    return G3Function8002DFD8(state, game);
}

uint32 G3StateWarning8002E7EC(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002e7ec, "SLUS_008.06");
    uint32 frame = guest_stack - 0x48;
    for (uint32 group = 0; group < 2; ++group)
    {
        uint32 words[3];
        for (uint32 index = 0; index < 3; ++index)
        {
            uint32 source = 0x80010138 + group * 12 + index * 4;
            uint32 upper = r_u32((source + 3) & ~uint32(3));
            uint32 lower = r_u32(source & ~uint32(3));
            uint32 shift = (source & 3) << 3;
            words[index] = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
        }
        for (uint32 index = 0; index < 3; ++index)
        {
            uint32 destination = frame + 0x18 + group * 12 + index * 4;
            uint32 high_address = destination + 3;
            uint32 high_byte = high_address & 3;
            uint32 preserve = high_byte == 3 ? 0 : uint32(0xffffffff) << ((high_byte + 1) << 3);
            uint32 previous = r_u32(high_address & ~uint32(3));
            w_u32(high_address & ~uint32(3), (previous & preserve) | (words[index] >> ((3 - high_byte) << 3)));
            uint32 low_byte = destination & 3;
            preserve = low_byte == 0 ? 0 : uint32(0xffffffff) >> ((4 - low_byte) << 3);
            previous = r_u32(destination & ~uint32(3));
            w_u32(destination & ~uint32(3), (previous & preserve) | (words[index] << (low_byte << 3)));
        }
    }
    uint32 terminator = (uint32)(sint32)r_s8(0x80010150);
    w_u8(frame + 0x30, terminator);
    (uint32)DrawSync(0);
    G3MemPackPush8004829C();
    uint32 game = r_u32(0x80097048);
    uint32 index = r_u32(game + 0x54);
    uint32 y = ((index << 4) - index) << 4;
    uint32 file = G3LoadFile80064A7C(frame + 0x18, frame);
    w_u32(frame + 0x10, 0);
    G3Function800656AC(file, 0, y, 0, frame);
    G3MemPackPop80048320();
    game = r_u32(0x80097048);
    w_u32(game + 0x154, 0);
    w_u32(state, 5);
    return 5;
}

uint32 G3StateLoad8002E61C(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002e61c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    uint32 pending = r_u32(0x800aa7d4);
    for (uint32 group = 0; group < 2; ++group)
    {
        uint32 words[3];
        for (uint32 index = 0; index < 3; ++index)
        {
            uint32 source = 0x8001011c + group * 12 + index * 4;
            uint32 upper = r_u32((source + 3) & ~uint32(3));
            uint32 lower = r_u32(source & ~uint32(3));
            uint32 shift = (source & 3) << 3;
            words[index] = shift == 0 ? lower : (upper << (32 - shift)) | (lower >> shift);
        }
        for (uint32 index = 0; index < 3; ++index)
        {
            uint32 destination = frame + 0x10 + group * 12 + index * 4;
            uint32 high_address = destination + 3;
            uint32 high_byte = high_address & 3;
            uint32 preserve = high_byte == 3 ? 0 : uint32(0xffffffff) << ((high_byte + 1) << 3);
            uint32 previous = r_u32(high_address & ~uint32(3));
            w_u32(high_address & ~uint32(3), (previous & preserve) | (words[index] >> ((3 - high_byte) << 3)));
            uint32 low_byte = destination & 3;
            preserve = low_byte == 0 ? 0 : uint32(0xffffffff) >> ((4 - low_byte) << 3);
            previous = r_u32(destination & ~uint32(3));
            w_u32(destination & ~uint32(3), (previous & preserve) | (words[index] << (low_byte << 3)));
        }
    }
    uint32 terminator = (uint32)(sint32)r_s8(0x80010134);
    w_u8(frame + 0x28, terminator);
    if (pending != 0)
    {
        do
        {
            G3Function80035AD4(frame);
            pending = r_u32(0x800aa7d4);
        } while (pending != 0);
    }
    uint32 game = r_u32(0x80097048);
    if (r_u32(game + 0x48) != 0 && (r_u32(game + 0x69b8) & 0x1000) == 0)
        G3Function8002E43C(frame);
    if (r_u32(state + 4) == 6)
    {
        G3PresentDisplay8002DAEC();
        G3Function8002E59C(frame + 0x10, frame);
    }
    (uint32)DrawSync(0);
    game = r_u32(0x80097048);
    w_u32(game + 0x14c, 0);
    G3MemPackPush8004829C();
    game = r_u32(0x80097048);
    if ((r_u32(game + 0x69b8) & 0x1000) == 0)
        G3Function8003EBE0(game + 0x6690, frame);
    G3Function80028544(frame);
    game = r_u32(0x80097048);
    G3Function80023AC8(game + 0x65f0, game + 0x6640, game, frame);
    game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    uint32 object = r_u32(level + 0xa4);
    G3Function80020A44(object);
    game = r_u32(0x80097048);
    if (r_s16(game + 0x6780) == 2)
    {
        uint32 flags = r_u32(game + 0x6834);
        w_u16(game + 0x6786, 9);
        w_u32(game + 0x6834, flags | 1);
    }
    game = r_u32(0x80097048);
    w_u16(game + 0x6780, 0);
    w_u32(state, 2);
    return G3PresentDisplay8002DAEC();
}

static sint32 G3PlayFullscreenMovie(uint32 movie)
{
    // Adapt the original ISO pathname to the extracted DATA tree
    char path[80] = "DATA/";
    uint32 input = r_u8(movie) == '\\' ? 1 : 0;
    uint32 output = 5;
    for (;; ++input)
    {
        uint32 value = r_u8(movie + input);
        if (value == 0 || value == ';')
            break;
        if (output + 1 >= sizeof(path))
        {
            {
#ifndef NDEBUG
            fprintf(stderr, "Fullscreen movie pathname is too long\n");
#endif
            }
            return -1;
        }
        path[output++] = value == '\\' ? '/' : (char)value;
    }
    path[output] = 0;
    switch (xport_fmv_play(path))
    {
    case 1:
        return 0;
    case 2:
        return 1;
    default:
        {
#ifndef NDEBUG
        fprintf(stderr, "Fullscreen movie playback failed: %s\n", path);
#endif
        }
        return -1;
    }
}

uint32 G3StateMovie8002E924(uint32 state, uint32 guest_stack)
{
    uint32 frame = guest_stack - 0x68;
    uint32 index = r_u32(state + 8);
    for (uint32 path = 0; path < 2; ++path)
    {
        uint32 source = 0x80010154 + path * 0x20;
        uint32 destination = frame + 0x10 + path * 0x20;
        for (uint32 group = 0; group < 2; ++group)
        {
            uint32 offset = group * 0x0c;
            uint32 first = r_u32(source + offset);
            uint32 second = r_u32(source + offset + 4);
            uint32 third = r_u32(source + offset + 8);
            w_u32(destination + offset, first);
            w_u32(destination + offset + 4, second);
            w_u32(destination + offset + 8, third);
        }
        uint32 first = r_u32(source + 0x18);
        uint32 second = r_u32(source + 0x1c);
        w_u32(destination + 0x18, first);
        w_u32(destination + 0x1c, second);
    }
    uint32 movie = 0x80097050 + (((index << 3) + index) << 3);
    uint32 name_bytes = 0;
    while (r_u8(movie + name_bytes) != 0)
        ++name_bytes;
    ++name_bytes;
    {
#ifndef NDEBUG
    if (index != 0)
        printf("Playing Movie: \"%s\"\n", (const char *)psx_addr(movie, name_bytes));
#endif
    }
    G3MemPackPush8004829C();
    if (r_u32(state + 4) == 6)
    {
        uint32 bytes = G3FileBytes80064D38(frame + 0x10);
        G3MemPackAllocate80048190(bytes);
        bytes = G3FileBytes80064D38(frame + 0x30);
        G3MemPackAllocate80048190(bytes);
    }
    else
    {
        uint32 relocations = G3LoadFile80064A7C(frame + 0x10, frame);
        uint32 code = G3LoadFile80064A7C(frame + 0x30, frame);
        G3RelocateModule800398BC(relocations, code);
        uint32 game = r_u32(0x80097048);
        w_u32(0x800a3d8c, code);
        w_u32(game + 0x48, 0);
    }
    (uint32)DrawSync(0);
    VSync(0);
    // Replace the cinema module player through the shared FMV boundary
    if (index != 0)
        G3PlayFullscreenMovie(movie);
    G3MemPackPop80048320();
    index = r_u32(state + 8);
    uint32 descriptor = 0x80097050 + (((index << 3) + index) << 3);
    sint32 next = r_s16(descriptor + 0x46);
    if (next < 0)
        w_u32(state, 3);
    else if (index == 0x10)
    {
        uint32 game = r_u32(0x80097048);
        if (r_s32(game + 0x682c) >= 0x32)
            w_u32(state + 8, 0x15);
        else
            w_u32(state + 8, (uint32)r_s16(0x80097516));
    }
    else
        w_u32(state + 8, (uint32)next);
    return G3PresentDisplay8002DAEC();
}

uint32 G3FileBytes80064D38(uint32 filename)
{
    FUNCTION_MARKER(0x80064d38, "SLUS_008.06");
    uint32 index = G3FindArchiveFile800654AC(filename);
    uint32 count = r_u32(0x800ad1a8);
    if (index == count)
        return 0;
    uint32 offset = ((index << 1) + index) << 3;
    return r_u32(0x800ad1b4 + offset);
}

uint32 G3RelocateModule800398BC(uint32 relocations, uint32 code)
{
    FUNCTION_MARKER(0x800398bc, "SLUS_008.06");
    if (r_u32(relocations) == 0xffffffff)
        return 0xffffffff;
    uint32 jump_base = (code << 4) >> 6;
    uint32 entry = r_u32(relocations);
    relocations += 4;
    do
    {
        uint32 target = code + (entry & 0xfffffffc);
        switch (entry & 3)
        {
        case 0:
        {
            uint32 value = r_u32(target);
            w_u32(target, value + code);
            break;
        }
        case 1:
        {
            uint32 addend = r_u32(relocations);
            relocations += 4;
            uint32 value = (code + addend + 0x8000) >> 16;
            w_u16(target, value);
            break;
        }
        case 2:
        {
            uint32 value = r_u16(target);
            w_u16(target, value + code);
            break;
        }
        case 3:
        {
            uint32 value = r_u32(target);
            w_u32(target, value + jump_base);
            break;
        }
        }
        entry = r_u32(relocations);
        relocations += 4;
    } while (entry != 0xffffffff);
    return 0xffffffff;
}

uint32 G3ArchiveNameHash80065448(uint32 filename)
{
    FUNCTION_MARKER(0x80065448, "SLUS_008.06");
    uint32 sum = 0;
    uint32 mixed = 0;
    uint32 index = 0;
    uint32 character = r_u8(filename);
    while (character != 0)
    {
        uint32 value = character;
        if (character - 0x61 < 0x1a)
            value &= 0xdf;
        uint32 product = value * index;
        sum += value;
        ++filename;
        ++index;
        character = r_u8(filename);
        mixed ^= product;
    }
    return (index << 24) | (sum << 12) | mixed;
}

uint32 G3FindArchiveFile800654AC(uint32 filename)
{
    FUNCTION_MARKER(0x800654ac, "SLUS_008.06");
    uint32 hash = G3ArchiveNameHash80065448(filename);
    uint32 low = 0;
    uint32 high = r_u32(0x800ad1a8);
    if ((sint32)high >= 0)
    {
        do
        {
            uint32 sum = low + high;
            uint32 middle = (sum >> 1) | (sum & 0x80000000);
            uint32 offset = ((middle << 1) + middle) << 3;
            uint32 candidate = r_u32(0x800ad1b0 + offset);
            if ((sint32)hash < (sint32)candidate)
                high = middle - 1;
            else
            {
                low = middle + 1;
                if ((sint32)candidate >= (sint32)hash)
                    return middle;
            }
        } while ((sint32)high >= (sint32)low);
    }
    uint32 path_bytes = 0;
    while (r_u8(filename + path_bytes) != 0)
        ++path_bytes;
    ++path_bytes;
    {
#ifndef NDEBUG
    printf("CD ERROR: file %s not in bigfile\n", (const char *)psx_addr(filename, path_bytes));
#endif
    }
    return r_u32(0x800ad1a8);
}

uint32 G3LoadFile80064A7C(uint32 filename, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80064a7c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x40;
    while (r_u32(0x800aa7d4) != 0)
        G3Function80035AD4(frame);
    uint32 index = G3FindArchiveFile800654AC(filename);
    uint32 offset = ((index << 1) + index) << 3;
    uint32 entry = 0x800ad1b0 + offset;
    uint32 file_bytes = r_u32(entry + 4);
    uint32 packed_bytes = r_u32(entry + 8);
    uint32 sectors = (uint32)((sint32)(file_bytes + 0x7ff) >> 11);
    uint32 read_sectors;
    if (packed_bytes != 0)
    {
        ++sectors;
        read_sectors = (uint32)((sint32)(packed_bytes + 0x7ff) >> 11);
    }
    else
        read_sectors = sectors;
    uint32 allocated_bytes = sectors << 11;
    uint32 output = G3MemPackAllocate80048190(allocated_bytes);
    uint32 input = output + ((sectors - read_sectors) << 11);
    uint32 path_bytes = 0;
    while (r_u8(filename + path_bytes) != 0)
        ++path_bytes;
    ++path_bytes;
    {
#ifndef NDEBUG
    printf("Loading:%s\n", (const char *)psx_addr(filename, path_bytes));
#endif
    }
    uint32 checksum = r_u32(entry + 0x10);
    uint32 file_offset = r_u32(entry + 0x0c);
    uint32 sector_offset = (uint32)((sint32)file_offset >> 11);
    uint32 request = G3QueueRead800652C8(sector_offset, input, output, read_sectors, 1, checksum, packed_bytes);
    G3WaitArchiveRead8002C830(request, frame);
    file_bytes = r_u32(entry + 4);
    if ((sint32)file_bytes < (sint32)allocated_bytes)
        G3MemPackRelease80048248(allocated_bytes - file_bytes);
    return output;
}

uint32 G3MemPackRelease80048248(uint32 bytes)
{
    FUNCTION_MARKER(0x80048248, "SLUS_008.06");
    uint32 depth = r_u32(0x800a2960);
    uint32 cursor = r_u32(0x800a2964 + (depth << 3));
    uint32 rounded = bytes & 0xfffffffc;
    w_u32(0x800a2964 + (depth << 3), cursor - rounded);
    depth = r_u32(0x800a2960);
    uint32 remaining = r_u32(0x800a2968 + (depth << 3));
    remaining += rounded;
    w_u32(0x800a2968 + (depth << 3), remaining);
    G3LevelCodeInvalidateSpan(cursor - rounded, rounded);
    return remaining;
}

uint32 G3WaitArchiveRead8002C830(uint32 request, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8002c830, "SLUS_008.06");
    uint32 frame = guest_stack - 0x38;
    uint32 game = r_u32(0x80097048);
    uint32 flags = r_u32(game + 0x6834);
    w_u32(game + 0x6834, flags | 0x8000);
    uint32 level = r_u32(game + 0x48);
    uint32 saved_count = r_u32(game + 0x28);
    if (level != 0)
    {
        uint32 old_base = r_u32(0x8009d6a4);
        if (old_base != 0x801e4ad0)
        {
            G3Function8002D814(0x801e4ad0, old_base, 0x7530);
            game = r_u32(0x80097048);
            w_u32(0x8009d6a4, 0x801e4ad0);
            w_u32(0x800a0c7c, 0x801f4000);
            w_u32(0x800a0c80, 0x801ec000);
            w_u32(0x801f4008, 0x801fbfdc);
            w_u32(0x801ec008, 0x801f3fdc);
            w_u32(game + 0x68f0, 0);
            w_u8(0x800a3d98, 0);
            w_u8(0x800a3d99, 0);
            w_u8(0x800a3d9a, 0);
        }
    }
    bool waiting = true;
    if (request != 0xffffffff)
        waiting = G3Function80065650(request) != 0;
    while (waiting)
    {
        waiting = false;
        if (request != 0xffffffff)
        {
            G3CdPump80064F44();
            waiting = G3Function80065650(request) != 0;
        }
        game = r_u32(0x80097048);
        level = r_u32(game + 0x48);
        if (level == 0 || r_u32(0x8009d6a4) != 0x801e4ad0)
        {
            game = r_u32(0x80097048);
            w_u32(game + 0x68f0, 0xffffffff);
        }
        else
        {
            uint32 index = r_u32(game + 0x58);
            w_u32(game + 0x28, 0xc00);
            index = 1 - index;
            uint32 packet = 0x801ccad0 + (((index << 1) + index) << 12);
            w_u32(game + 0x58, index);
            w_u32(game + 0x38, packet);
            uint32 buffer = r_u32(0x800a0c7c + (index << 2));
            w_u32(game + 0x68, buffer);
            w_u32(buffer + 4, buffer + 0x0c);
            buffer = r_u32(game + 0x68);
            w_u32(buffer, 0);
            uint32 previous_level = r_u16(game + 0x68b6);
            bool draw_text = previous_level - 0x17 >= 4;
            if (!draw_text)
            {
                uint32 current_level = r_u16(game + 0x68b4);
                draw_text = current_level - 0x17 < 4 || current_level == 0x1b;
                if (!draw_text)
                    draw_text = r_u32(game + 0x68f0) != 0;
            }
            if (draw_text)
            {
                game = r_u32(0x80097048);
                uint32 mode = r_u32(game + 0x6818);
                G3Function80021A74(0x140);
                uint32 text = G3Function80065D38(mode == 0 ? 0x43 : 0x25);
                G3Function800234A0(text, 0x78, 0, 0, frame, 0);
            }
            G3Function800866D8(packet, 0xc00);
            (uint32)DrawSync(0);
            game = r_u32(0x80097048);
            uint32 frames = r_u32(game + 0x14c);
            if (frames - 1 < 2)
            {
                previous_level = r_u16(game + 0x68b6);
                bool special_transition = false;
                bool show_level = true;
                if (previous_level - 0x17 < 4)
                {
                    uint32 current_level = r_u16(game + 0x68b4);
                    if (current_level - 0x17 >= 4)
                    {
                        show_level = false;
                        if (current_level != 0x1b)
                        {
                            G3Function8002D0D4(frame);
                            special_transition = true;
                        }
                    }
                }
                if (show_level)
                {
                    game = r_u32(0x80097048);
                    uint32 current_level = r_u16(game + 0x68b4);
                    if (current_level != 0x1b && r_u32(game + 0x6818) != 0)
                    {
                        current_level = r_u16(game + 0x68b4);
                        uint32 text = G3Function80038F18(current_level, 0);
                        G3Function80023448(text, 0xa0, 0, 0, frame, 0);
                    }
                }
                if (!special_transition)
                {
                    game = r_u32(0x80097048);
                    w_u32(game + 0x68f0, 0xffffffff);
                }
                w_u16(0x800a3da2, 0x100);
            }
            else if (frames >= 3)
            {
                previous_level = r_u16(game + 0x68b6);
                bool special_transition = false;
                if (previous_level - 0x17 < 4)
                {
                    uint32 current_level = r_u16(game + 0x68b4);
                    if (current_level - 0x17 >= 4 && current_level != 0x1b && r_u32(game + 0x68f0) == 0)
                    {
                        w_u16(0x800a3da2, 2);
                        G3Function8002D588(0, 0, frame);
                        special_transition = true;
                    }
                }
                if (!special_transition)
                {
                    game = r_u32(0x80097048);
                    sint32 status = r_s32(game + 0x68f0);
                    if (status > 0)
                    {
                        w_u16(0x800a3da2, 0x100);
                        w_u16(0x800a3da0, 0x200);
                    }
                    else
                        w_u16(0x800a3da2, 0xa0);
                }
            }
            game = r_u32(0x80097048);
            index = r_u32(game + 0x58);
            w_u16(0x800a3d9e, index << 8);
            uint32 tag = r_u32(packet + 0x2ffc);
            w_u32(0x800a3d94, (tag & 0x00ffffff) | 0x03000000);
            w_u32(packet + 0x2ffc, 0x000a3d94);
            G3Function80022E00(frame);
            uint32 busy;
            do
            {
                game = r_u32(0x80097048);
                uint32 pending = r_u32(game + 0x140);
                busy = G3Function800194B8(pending);
                if (busy != 0)
                    psx_native_poll_events();
            } while (busy != 0);
            game = r_u32(0x80097048);
            index = r_u32(game + 0x58);
            uint32 environment_offset = (((((index << 1) + index) << 3) - index) << 2);
            G3PutDrawEnv80086850(0x800a3cd4 + environment_offset);
            do
            {
                game = r_u32(0x80097048);
                uint32 pending = r_u32(game + 0x13c);
                busy = G3Function800194B8(pending);
                if (busy != 0)
                    psx_native_poll_events();
            } while (busy != 0);
            game = r_u32(0x80097048);
            uint32 display_index = r_u32(game + 0x54);
            w_u32(game + 0x140, game + 0x6570);
            w_u32(game + 0x54, 1 - display_index);
            G3Function800867E0(packet + 0x2ffc);
            game = r_u32(0x80097048);
            frames = r_u32(game + 0x14c);
            w_u32(game + 0x14c, frames + 1);
        }
    }
    (uint32)DrawSync(0);
    game = r_u32(0x80097048);
    w_u32(game + 0x28, saved_count);
    w_u16(0x800a3da2, 0xf0);
    flags = r_u32(game + 0x6834) & 0xffff7fff;
    w_u32(game + 0x6834, flags);
    return flags;
}

uint32 G3Function80065650(uint32 request)
{
    FUNCTION_MARKER(0x80065650, "SLUS_008.06");
    uint32 offset = ((request << 3) + request) << 2;
    return r_u32(0x800ad0d4 + offset + 0x14);
}

uint32 G3Function800194B8(uint32 value)
{
    FUNCTION_MARKER(0x800194b8, "SLUS_008.06");
    return value;
}

uint32 G3Function8002D814(uint32 destination, uint32 source, uint32 bytes)
{
    FUNCTION_MARKER(0x8002d814, "SLUS_008.06");
    if (destination != 0 && (sint32)bytes > 0)
        memcpy(psx_addr(destination, bytes), psx_addr(source, bytes), bytes);
    uint32 first_table = r_u32(destination + 0x0c);
    uint32 second_table = r_u32(destination + 0x1c);
    first_table = first_table - source + destination;
    second_table = second_table - source + destination;
    w_u32(destination + 0x0c, first_table);
    w_u32(destination + 0x1c, second_table);
    sint32 count = r_s16(source + 8);
    if (count <= 0)
        return (uint32)count;
    uint32 index = 0;
    do
    {
        uint32 table = r_u32(destination + 0x0c);
        uint32 offset = index << 2;
        uint32 slot = table + offset;
        uint32 original = r_u32(slot);
        if (original < destination)
        {
            w_u32(slot, original - source);
            table = r_u32(destination + 0x0c);
            slot = table + offset;
            uint32 object = r_u32(slot);
            w_u32(slot, object + destination);
            const uint32 fields[4] = { 8, 0x14, 0x18, 0x1c };
            for (uint32 field_index = 0; field_index < 4; ++field_index)
            {
                table = r_u32(destination + 0x0c);
                object = r_u32(table + offset);
                uint32 value = r_u32(object + fields[field_index]);
                w_u32(object + fields[field_index], value - source);
                table = r_u32(destination + 0x0c);
                object = r_u32(table + offset);
                value = r_u32(object + fields[field_index]);
                w_u32(object + fields[field_index], value + destination);
            }
            count = r_s16(source + 8);
            if (count > 0)
            {
                uint32 alias_index = 0;
                do
                {
                    table = r_u32(destination + 0x0c);
                    uint32 alias_offset = alias_index << 2;
                    slot = table + alias_offset;
                    uint32 alias = r_u32(slot);
                    if (alias == original)
                    {
                        w_u32(slot, alias - source);
                        table = r_u32(destination + 0x0c);
                        slot = table + alias_offset;
                        alias = r_u32(slot);
                        w_u32(slot, alias + destination);
                    }
                    count = r_s16(source + 8);
                    ++alias_index;
                } while ((sint32)alias_index < count);
                count = r_s16(source + 8);
            }
        }
        else
            count = r_s16(source + 8);
        ++index;
    } while ((sint32)index < count);
    return 0;
}

uint32 G3Function80021A74(uint32 width)
{
    FUNCTION_MARKER(0x80021a74, "SLUS_008.06");
    uint32 signed_width = (uint32)(sint32)(sint16)(width & 0xffff);
    return G3Function80020C48(0x7c, signed_width, 0, 0, 1);
}

uint32 G3Function80065D38(uint32 token)
{
    FUNCTION_MARKER(0x80065d38, "SLUS_008.06");
    return r_u32(0x80099a18 + (token << 2));
}

uint32 G3Function80020C48(uint32 command, uint32 x, uint32 y, uint32 option, uint32 enabled)
{
    FUNCTION_MARKER(0x80020c48, "SLUS_008.06");
    uint32 count = r_u32(0x800a3c64);
    uint32 offset = count << 3;
    uint32 entry = 0x800a2c44 + offset;
    if ((sint32)count >= 0x1ff)
        return offset;
    if ((command & 0xff) == 0x40)
    {
        w_u16(entry, x & 0xff);
        w_u16(entry + 2, y & 0xff);
        w_u8(entry + 4, command);
    }
    else
    {
        w_u16(entry, x);
        w_u16(entry + 2, y);
        w_u8(entry + 4, command);
        w_u8(entry + 5, option);
        w_u8(entry + 6, enabled);
    }
    count = r_u32(0x800a3c64);
    ++count;
    w_u32(0x800a3c64, count);
    return count;
}

uint32 G3Function80022E00(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80022e00, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    G3Function80022CAC(frame);
    G3Function80022BE4(frame);
    return G3Function80022DEC();
}

uint32 G3Function80022DEC(void)
{
    FUNCTION_MARKER(0x80022dec, "SLUS_008.06");
    uint32 saved_count = r_u32(0x800a3c68);
    w_u32(0x800a3c64, saved_count);
    return 0x800a2c44;
}

uint32 G3Function80022BE4(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80022be4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 count = r_u32(0x800a3c64);
    w_u32(0x800a3c54, 0x0a);
    w_u32(0x800a3c58, 0x10);
    if (count == 0)
        return 0x10;
    uint32 index = count - 1;
    if ((sint32)index < 0)
        return 0x10;
    uint32 result = index << 3;
    do
    {
        uint32 entry = 0x800a2c44 + result;
        uint32 command = r_u8(entry + 4);
        if (command != 0x20)
        {
            if (command == 0x40)
            {
                --index;
                uint32 previous_y = (uint32)(sint32)r_s16(entry - 6);
                uint32 previous_x = (uint32)(sint32)r_s16(entry - 8);
                uint32 current_y = (uint32)(sint32)r_s16(entry + 2);
                uint32 x = previous_x + (previous_y << 8);
                uint32 current_x = (uint32)(sint32)r_s16(entry);
                uint32 y = current_x + (current_y << 8);
                G3Function800213A8(x, y, 0);
            }
            else if ((r_u8(entry + 6) & 1) == 0)
                G3Function80020D2C(entry, frame);
        }
        --index;
        result = index << 3;
    } while ((sint32)index >= 0);
    return result;
}

void G3Function80022CAC(uint32 guest_stack)
{
    FUNCTION_MARKER(0x80022cac, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 game = r_u32(0x80097048);
    uint32 configuration = r_u32(game + 0x4c);
    uint32 width = r_u32(configuration + 0x90);
    if (width != 0x140)
        SetGeomScreen(0x140);
    SetBackColor(0x20, 0x20, 0x20);
    uint32 count = r_u32(0x800a3c64);
    w_u32(0x800a3c5c, 0x28);
    w_u32(0x800a3c60, 0x1e);
    if ((sint32)count > 0)
    {
        uint32 index = 0;
        uint32 entry = 0x800a2c44;
        do
        {
            uint32 command = r_u8(entry + 4);
            if (command != 0x20)
            {
                if (command == 0x7c)
                {
                    sint32 value = r_s16(entry);
                    w_u32(0x8009d62c, (uint32)value);
                }
                else if (command != 0x40 && (r_u8(entry + 6) & 1) != 0)
                {
                    G3Function80020D2C(entry, frame);
                    w_u8(entry + 4, 0x20);
                }
            }
            entry += 8;
            count = r_u32(0x800a3c64);
            ++index;
        } while ((sint32)index < (sint32)count);
    }
    game = r_u32(0x80097048);
    configuration = r_u32(game + 0x4c);
    width = r_u32(configuration + 0x90);
    if (width != 0x140)
        SetGeomScreen((sint32)width);
    G3Function8002B484();
}

void G3Function8002B484(void)
{
    FUNCTION_MARKER(0x8002b484, "SLUS_008.06");
    uint32 red = r_u8(0x800a0d38);
    uint32 green = r_u8(0x800a0d39);
    uint32 blue = r_u8(0x800a0d3a);
    SetBackColor(red >> 1, green >> 1, blue >> 1);
}

uint32 G3Function80020D2C(uint32 entry, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80020d2c, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 flags = r_u8(entry + 6);
    uint32 character = r_u8(entry + 4);
    uint32 x = (uint32)(sint32)r_s16(entry);
    uint32 y = (uint32)(sint32)r_s16(entry + 2);
    uint32 saved_mode = r_u32(0x8009d628);
    if ((flags & 1) == 0 || r_u32(0x8009d6a4) == 0)
        return G3Function800210F4(character, x, y);
    w_u32(0x8009d628, (flags >> 1) & 1);
    uint32 letter = (character - 0x41) & 0xff;
    uint32 result;
    if (letter >= 0x1a)
    {
        uint32 color = r_u8(entry + 5);
        result = G3Function80022408(character, x, y, color, frame);
    }
    else if (letter == 0)
    {
        uint32 glyph = r_u8(0x80096b98);
        uint32 height = (uint32)(sint32)r_s16(0x80096b9a);
        uint32 color = r_u8(entry + 5);
        G3Function80022408(glyph, x, y - height, color, frame);
        glyph = r_u8(0x80096b99);
        height = (uint32)(sint32)r_s16(0x80096b9a);
        color = r_u8(entry + 5);
        result = G3Function80022408(glyph, x, y + 0x0f - height, color, frame);
    }
    else
    {
        uint32 mapping = 0x80096b98 + (letter << 2);
        uint32 glyph = r_u8(mapping);
        uint32 color = r_u8(entry + 5);
        G3Function80022408(glyph, x, y, color, frame);
        sint32 second_glyph = r_s8(mapping + 1);
        if (second_glyph < 0)
            result = (uint32)second_glyph;
        else
        {
            glyph = r_u8(mapping + 1);
            uint32 height = (uint32)(sint32)r_s16(mapping + 2);
            color = r_u8(entry + 5);
            result = G3Function80022408(glyph, x, y + height, color, frame);
        }
    }
    w_u32(0x8009d628, saved_mode);
    return result;
}

uint32 G3Function80022E30(uint32 x, uint32 y)
{
    FUNCTION_MARKER(0x80022e30, "SLUS_008.06");
    sint32 signed_x = (sint16)(x & 0xffff);
    if (signed_x > 0)
        w_u32(0x800a3c54, (uint32)signed_x);
    sint32 signed_y = (sint16)(y & 0xffff);
    if (signed_y > 0)
        w_u32(0x800a3c58, (uint32)signed_y);
    return 0x800a0000;
}
