#include "xport.h"
#include "psx.h"
#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void G3HankLoadSpecial0FCC(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00000fcc, "G3L_33");
    uint32 frame = guest_stack - 0x20;
    uint32 initial = r_u32(state + 0x14);
    if (initial == 0)
    {
        w_u32(settings + 0xd0, 0x3d0900);
        w_u16(settings + 0xd6, 0x4);
        w_u16(settings + 0xde, 0x320);
        w_u16(settings + 0xe4, 0x50);
        w_u16(settings + 0xec, 0x7d00);
        w_u16(settings + 0xf2, 0x2);
        w_u32(settings + 0xc8, 0x0);
        w_u32(settings + 0xcc, 0xf4240);
        w_u16(settings + 0xd4, 0x0);
        w_u16(settings + 0xd8, 0x0);
        w_u16(settings + 0xda, 0x0);
        w_u16(settings + 0xdc, 0x0);
        w_u16(settings + 0xe0, 0x0);
        w_u16(settings + 0xe2, 0x0);
        w_u16(settings + 0xe6, 0x0);
        w_u16(settings + 0xee, 0x0);
        w_u16(settings + 0xea, 0x0);
        w_u16(settings + 0xf0, 0x0);
        w_u32(state + 0x14, 0x29a);
    }
    else if (initial == 0x29a) w_u32(state + 0x14, 0x16062);
    do
    {
        switch (r_s16(cursor + 6))
        {
        case 1:
            {
                sint32 distance = r_s16(r_u32(cursor + 8));
                w_u32(settings + 0xcc, (uint32)(distance * distance));
                distance = r_s16(r_u32(cursor + 8) + 2);
                w_u32(settings + 0xd0, (uint32)(distance * distance));
                if (r_s16(cursor + 0xc) >= 5)
                {
                    w_u16(settings + 0xea, r_u16(r_u32(cursor + 8) + 4));
                    if (r_s16(cursor + 0xc) >= 7) w_u16(settings + 0xec, r_u16(r_u32(cursor + 8) + 6));
                }
            }
            break;
        case 0x52:
            {
                uint32 data = r_u32(cursor + 8);
                uint32 game = r_u32(0x80097048);
                uint32 resource = G3FindLevelResource800397B0(data, game);
                w_u32(settings + 0xc8, resource);
            }
            break;
        case 0x51: w_u16(settings + 0xd4, r_u16(r_u32(cursor + 8))); break;
        case 0x76: w_u16(settings + 0xe6, r_u16(r_u32(cursor + 8))); break;
        case 0x5b: w_u16(settings + 0xe0, r_u16(r_u32(cursor + 8))); break;
        case 0x5c: w_u16(settings + 0xe2, r_u16(r_u32(cursor + 8))); break;
        case 0x9d: w_u16(settings + 0xe4, r_u16(r_u32(cursor + 8))); break;
        case 0x5e: w_u16(settings + 0xd8, r_u16(r_u32(cursor + 8))); break;
        case 0xa0: w_u16(settings + 0xf2, r_u16(r_u32(cursor + 8))); break;
        case 0x89: w_u16(settings + 0xe4, 0 - r_u16(r_u32(cursor + 8))); break;
        case 0x33: w_u16(settings + 0xd6, (uint32)(r_s16(r_u32(cursor + 8)) * 4096 / 3600)); break;
        case 0x55: w_u16(settings + 0xf0, (uint32)(r_s16(r_u32(cursor + 8)) * 4096 / 3600)); break;
        case 0x5a:
            {
                uint32 source = r_u32(cursor + 8);
                uint32 destination = settings + 0xda;
                uint32 address = source;
                uint32 high_address = address + 3;
                uint32 high_lane = high_address & 3;
                uint32 lane = address & 3;
                uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                uint32 low = r_masked_u32(address & 0xfffffffc, (15 << lane) & 15);
                uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
                uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
                uint32 z = (uint32)r_s16(source + 4);
                address = destination;
                high_address = address + 3;
                high_lane = high_address & 3;
                lane = address & 3;
                w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                w_masked_u32(address & 0xfffffffc, value << (lane * 8), (15 << lane) & 15);
                w_u16(destination + 4, z);
            }
            break;
        }
    } while (G3HankNextField0140(cursor, remaining, frame) != 0);
}

void G3HankLoadShooter1364(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001364, "G3L_33");
    uint32 frame = guest_stack - 0x20;
    uint32 initial = r_u32(state + 0x18);
    if (initial == 0)
    {
        uint32 owner = r_u32(state - 0xe4);
        uint32 table = r_u32(owner + 4);
        uint32 target = 0;
        if (table != 0)
        {
            sint32 count = r_s32(table);
            uint32 entry = table + 4;
            if (count > 0)
            {
                uint32 first_name = r_u32(module_base + 0x8804);
                for (sint32 index = 0; index < count; ++index)
                {
                    uint32 candidate = r_u32(entry);
                    if (candidate != owner)
                    {
                        uint32 resource = r_u32(candidate);
                        uint32 name = r_u32(resource + 0x20);
                        uint32 first = r_u32(name);
                        if (first == first_name)
                        {
                            uint32 second = r_u32(name + 4);
                            uint32 second_name = r_u32(module_base + 0x8808);
                            if (second == second_name) { target = candidate; break; }
                        }
                    }
                    entry += 4;
                }
            }
        }
        if (target == 0)
        {
            while (G3HankNextField0140(cursor, remaining, frame) != 0) {}
            return;
        }
        w_u32(settings + 0xf4, target);
        w_u32(settings + 0xfc, 0x3d0900);
        w_u16(settings + 0x102, 10);
        w_u32(settings + 0xf8, 0xf4240);
        w_u16(settings + 0x100, 0);
        w_u16(settings + 0x104, 0);
        w_u16(settings + 0x106, 0);
        w_u32(state + 0x18, 0x29a);
    }
    else if (initial == 0x29a) w_u32(state + 0x18, 0x16062);
    do
    {
        switch (r_s16(cursor + 6))
        {
        case 1:
            {
                sint32 distance = r_s16(r_u32(cursor + 8));
                w_u32(settings + 0xf8, (uint32)(distance * distance));
                distance = r_s16(r_u32(cursor + 8) + 2);
                w_u32(settings + 0xfc, (uint32)(distance * distance));
            }
            break;
        case 0x33: w_u16(settings + 0x102, (uint32)(r_s16(r_u32(cursor + 8)) * 4096 / 3600)); break;
        case 0x51: w_u16(settings + 0x100, r_u16(r_u32(cursor + 8))); break;
        case 0x5d: w_u16(settings + 0x106, r_u16(r_u32(cursor + 8))); break;
        case 0x5e: w_u16(settings + 0x104, r_u16(r_u32(cursor + 8))); break;
        }
    } while (G3HankNextField0140(cursor, remaining, frame) != 0);
}

void G3HankLoadHurt15E4(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000015e4, "G3L_33");
    uint32 frame = guest_stack - 0x28;
    uint32 initial = r_u32(state + 0x20);
    if (initial == 0)
    {
        uint32 player = r_u32(0x8009efc4);
        {
            uint32 source = state - 0xac;
            uint32 destination = settings + 0x14c;
            uint32 address = source;
            uint32 high_address = address + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = address & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(address & 0xfffffffc, (15 << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            uint32 z = (uint32)r_s16(source + 4);
            address = destination;
            high_address = address + 3;
            high_lane = high_address & 3;
            lane = address & 3;
            w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            w_masked_u32(address & 0xfffffffc, value << (lane * 8), (15 << lane) & 15);
            w_u16(destination + 4, z);
        }
        w_u16(settings + 0x156, 0xffff);
        w_u16(settings + 0x154, 0xffff);
        w_u16(settings + 0x158, 0);
        uint32 descriptor = r_u32(player + 0x1c);
        uint32 data = r_u32(descriptor + 0x1c);
        uint32 value = r_u16(data + 4);
        w_u16(settings + 0x15a, 0);
        w_u16(settings + 0x162, 0);
        w_u16(settings + 0x15c, value);
        w_u32(state + 0x20, 0x29a);
    }
    else if (initial == 0x29a) w_u32(state + 0x20, 0x16062);
    w_u16(settings + 0x152, 0);
    do
    {
        switch ((sint16)(r_u16(cursor + 6) - 0x5f))
        {
        case 0:
            {
                uint32 source = r_u32(cursor + 8);
                uint32 destination = settings + 0x14c;
                uint32 address = source;
                uint32 high_address = address + 3;
                uint32 high_lane = high_address & 3;
                uint32 lane = address & 3;
                uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                uint32 low = r_masked_u32(address & 0xfffffffc, (15 << lane) & 15);
                uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
                uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
                uint32 z = (uint32)r_s16(source + 4);
                address = destination;
                high_address = address + 3;
                high_lane = high_address & 3;
                lane = address & 3;
                w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                w_masked_u32(address & 0xfffffffc, value << (lane * 8), (15 << lane) & 15);
                w_u16(destination + 4, z);
            }
            w_u16(settings + 0x152, 1);
            break;
        case 8: w_u16(settings + 0x154, r_u16(r_u32(cursor + 8))); break;
        case 0xc: w_u16(settings + 0x156, r_u16(r_u32(cursor + 8))); break;
        case 0xd: w_u16(settings + 0x15c, r_u16(r_u32(cursor + 8))); break;
        case 0xe: w_u16(settings + 0x158, r_u16(r_u32(cursor + 8))); break;
        case 0xf: w_u16(settings + 0x162, 1); break;
        case 0x14: w_u16(settings + 0x15a, r_u16(r_u32(cursor + 8))); break;
        }
    } while (G3HankNextField0140(cursor, remaining, frame) != 0);
}

void G3HankLoadMove178C(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000178c, "G3L_33");
    uint32 frame = guest_stack - 0x28;
    uint32 initial = r_u32(state + 0x1c);
    if (initial == 0)
    {
        w_u32(settings + 0x114, 0x3d0900);
        w_u32(settings + 0x118, 0x5f5e10);
        w_u16(settings + 0x126, 0x32);
        w_u16(settings + 0x128, 0x50);
        w_u32(settings + 0x10c, 0x9c400);
        w_u32(settings + 0x110, 0x15f900);
        w_u16(settings + 0x11c, 0x0);
        w_u16(settings + 0x122, 0xffff);
        w_u16(settings + 0x120, 0xffff);
        w_u16(settings + 0x11e, 0xffff);
        w_u16(settings + 0x124, 0xffff);
        w_u16(settings + 0x12a, 0x0);
        w_u16(settings + 0x12c, 0x0);
        {
            uint32 source = state - 0xac;
            uint32 destination = settings + 0x12e;
            uint32 address = source;
            uint32 high_address = address + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = address & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(address & 0xfffffffc, (15 << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            uint32 z = (uint32)r_s16(source + 4);
            address = destination;
            high_address = address + 3;
            high_lane = high_address & 3;
            lane = address & 3;
            w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            w_masked_u32(address & 0xfffffffc, value << (lane * 8), (15 << lane) & 15);
            w_u16(destination + 4, z);
        }
        w_u32(settings + 0x140, 0x0);
        w_u16(settings + 0x134, 0x0);
        w_u16(settings + 0x136, 0x0);
        w_u16(settings + 0x144, 0x0);
        w_u16(settings + 0x148, 0xffff);
        w_u32(state + 0x1c, 0x29a);
    }
    else if (initial == 0x29a) w_u32(state + 0x1c, 0x16062);
    do
    {
        sint32 kind = r_s16(cursor + 6);
        switch (kind)
        {
        case 1:
            {
                sint32 distance = r_s16(r_u32(cursor + 8) + 0);
                w_u32(settings + 0x10c, (uint32)(distance * distance));
            }
            {
                sint32 distance = r_s16(r_u32(cursor + 8) + 2);
                w_u32(settings + 0x110, (uint32)(distance * distance));
            }
            {
                sint32 distance = r_s16(r_u32(cursor + 8) + 4);
                w_u32(settings + 0x114, (uint32)(distance * distance));
            }
            {
                sint32 distance = r_s16(r_u32(cursor + 8) + 6);
                w_u32(settings + 0x118, (uint32)(distance * distance));
            }
            break;
        case 0x51:
            w_u16(settings + 0x11c, r_u16(r_u32(cursor + 8)));
            if (r_s16(cursor + 0xc) >= 3)
            {
                w_u16(settings + 0x11e, r_u16(r_u32(cursor + 8) + 2));
                if (r_s16(cursor + 0xc) >= 5)
                {
                    w_u16(settings + 0x120, r_u16(r_u32(cursor + 8) + 4));
                    if (r_s16(cursor + 0xc) >= 7) w_u16(settings + 0x122, r_u16(r_u32(cursor + 8) + 6));
                }
            }
            break;
        case 0x31: w_u16(settings + 0x128, (uint32)(r_s16(r_u32(cursor + 8)) * 4096 / 3600)); break;
        case 0x50: w_u16(settings + 0x126, r_u16(r_u32(cursor + 8))); break;
        case 0x61: w_u16(settings + 0x124, r_u16(r_u32(cursor + 8))); break;
        case 0x5e: w_u16(settings + 0x144, r_u16(r_u32(cursor + 8))); break;
        case 0x9a: w_u16(settings + 0x148, r_u16(r_u32(cursor + 8))); break;
        case 0x74: w_u16(settings + 0x12a, 1); break;
        case 0x77: w_u16(settings + 0x12c, 1); break;
        case 0x28: w_u32(settings + 0x140, r_u32(cursor + 8)); break;
        case 0x7f:
        case 0x80:
            {
                sint32 bytes = kind == 0x7f ? (sint16)r_u16(cursor + 0xc) : -r_s16(cursor + 0xc);
                w_u16(settings + 0x134, (uint32)(bytes / 6));
                w_u32(settings + 0x138, r_u32(cursor + 8));
            }
            break;
        case 0x85:
        case 0x86:
            {
                sint32 bytes = kind == 0x85 ? (sint16)r_u16(cursor + 0xc) : -r_s16(cursor + 0xc);
                w_u16(settings + 0x136, (uint32)(bytes / 6));
                w_u32(settings + 0x13c, r_u32(cursor + 8));
            }
            break;
        }
    } while (G3HankNextField0140(cursor, remaining, frame) != 0);
}

void G3HankLoadRun1BCC(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001bcc, "G3L_33");
    uint32 frame = guest_stack - 0x28;
    uint32 initial = r_u32(state + 0x2c);
    if (initial == 0)
    {
        w_u32(settings + 0x1c8, 0xf4240);
        w_u32(settings + 0x1cc, 0x15f900);
        w_u16(settings + 0x1e0, 0x32);
        w_u16(settings + 0x1e2, 0x50);
        w_u32(settings + 0x1c4, 0x0);
        w_u16(settings + 0x1e4, 0x0);
        w_u16(settings + 0x1e8, 0xffff);
        w_u16(settings + 0x1e6, 0xffff);
        w_u16(settings + 0x1ea, 0x0);
        {
            uint32 source = state - 0xac;
            uint32 destination = settings + 0x1ec;
            uint32 address = source;
            uint32 high_address = address + 3;
            uint32 high_lane = high_address & 3;
            uint32 lane = address & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 low = r_masked_u32(address & 0xfffffffc, (15 << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            uint32 z = (uint32)r_s16(source + 4);
            address = destination;
            high_address = address + 3;
            high_lane = high_address & 3;
            lane = address & 3;
            w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            w_masked_u32(address & 0xfffffffc, value << (lane * 8), (15 << lane) & 15);
            w_u16(destination + 4, z);
        }
        w_u32(settings + 0x1dc, 0);
        w_u16(settings + 0x1d0, 0);
        w_u16(settings + 0x1d2, 0);
        w_u32(state + 0x2c, 0x29a);
    }
    else if (initial == 0x29a) w_u32(state + 0x2c, 0x16062);
    do
    {
        sint32 kind = r_s16(cursor + 6);
        switch (kind)
        {
        case 1:
            {
                sint32 distance = r_s16(r_u32(cursor + 8) + 0);
                w_u32(settings + 0x1c4, (uint32)(distance * distance));
            }
            {
                sint32 distance = r_s16(r_u32(cursor + 8) + 2);
                w_u32(settings + 0x1c8, (uint32)(distance * distance));
            }
            {
                sint32 distance = r_s16(r_u32(cursor + 8) + 4);
                w_u32(settings + 0x1cc, (uint32)(distance * distance));
            }
            break;
        case 0x51:
            w_u16(settings + 0x1e4, r_u16(r_u32(cursor + 8)));
            if (r_s16(cursor + 0xc) >= 3) w_u16(settings + 0x1e8, r_u16(r_u32(cursor + 8) + 2));
            break;
        case 0x31: w_u16(settings + 0x1e2, (uint32)(r_s16(r_u32(cursor + 8)) * 4096 / 3600)); break;
        case 0x50: w_u16(settings + 0x1e0, r_u16(r_u32(cursor + 8))); break;
        case 0x61: w_u16(settings + 0x1e6, r_u16(r_u32(cursor + 8))); break;
        case 0x77: w_u16(settings + 0x1ea, 1); break;
        case 0x28: w_u32(settings + 0x1dc, r_u32(cursor + 8)); break;
        case 0x7f:
        case 0x80:
            {
                sint32 bytes = kind == 0x7f ? (sint16)r_u16(cursor + 0xc) : -r_s16(cursor + 0xc);
                w_u16(settings + 0x1d0, (uint32)(bytes / 6));
                w_u32(settings + 0x1d4, r_u32(cursor + 8));
            }
            break;
        case 0x85:
        case 0x86:
            {
                sint32 bytes = kind == 0x85 ? (sint16)r_u16(cursor + 0xc) : -r_s16(cursor + 0xc);
                w_u16(settings + 0x1d2, (uint32)(bytes / 6));
                w_u32(settings + 0x1d8, r_u32(cursor + 8));
            }
            break;
        }
    } while (G3HankNextField0140(cursor, remaining, frame) != 0);
}

void G3HankLoadDeath1EEC(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00001eec, "G3L_33");
    uint32 frame = guest_stack - 0x30;
    uint32 initial = r_u32(state + 0x24);
    if (initial == 0)
    {
        w_u16(settings + 0x16c, 0xffff);
        w_u16(settings + 0x16e, 0xffff);
        w_u16(settings + 0x170, 0xffff);
        w_u16(settings + 0x172, 0xffff);
        w_u16(settings + 0x176, 0xffff);
        w_u32(settings + 0x168, 0x0);
        w_u16(settings + 0x174, 0x0);
        w_u16(settings + 0x17e, 0x0);
        w_u16(settings + 0x17c, 0x0);
        w_u32(settings + 0x188, 0x0);
        w_u16(settings + 0x18e, 0x0);
        w_u16(settings + 0x18c, 0x0);
        w_u16(settings + 0x190, 0x0);
        w_u32(state + 0x24, 0x29a);
    }
    else if (initial == 0x29a) w_u32(state + 0x24, 0x16062);
    sint32 resource_count = 0;
    sint32 vector_count = 0;
    sint32 index_count = 0;
    do
    {
        sint32 kind = r_s16(cursor + 6);
        switch (kind)
        {
        case 0x51:
            w_u16(settings + 0x170, r_u16(r_u32(cursor + 8)));
            w_u16(settings + 0x16c, r_u16(r_u32(cursor + 8) + 2));
            if (r_s16(cursor + 0xc) >= 5)
            {
                w_u16(settings + 0x16e, r_u16(r_u32(cursor + 8) + 4));
                if (r_s16(cursor + 0xc) >= 7) w_u16(settings + 0x172, r_u16(r_u32(cursor + 8) + 6));
            }
            break;
        case 2:
            w_u16(settings + 0x18c, r_u16(r_u32(cursor + 8)));
            if (r_s16(cursor + 0xc) >= 3) w_u16(settings + 0x18e, r_u16(r_u32(cursor + 8) + 2));
            break;
        case 0x68: w_u16(settings + 0x176, r_u16(r_u32(cursor + 8))); break;
        case 0x69: w_u32(settings + 0x168, r_u32(cursor + 8)); break;
        case 0x6a: w_u16(settings + 0x174, 1); break;
        case 0x9c: w_u16(settings + 0x190, r_u16(r_u32(cursor + 8))); break;
        case 0x7a:
        case 0x7b:
            {
                sint32 bytes = kind == 0x7a ? (sint16)r_u16(cursor + 0xc) : -r_s16(cursor + 0xc);
                w_u16(settings + 0x17c, (uint32)(bytes / 6));
                w_u32(settings + 0x180, r_u32(cursor + 8));
            }
            break;
        case 0x7c:
        case 0x7d:
            {
                sint32 bytes = kind == 0x7c ? (sint16)r_u16(cursor + 0xc) : -r_s16(cursor + 0xc);
                w_u16(settings + 0x17e, (uint32)(bytes / 6));
                w_u32(settings + 0x184, r_u32(cursor + 8));
            }
            break;
        case 0x7e:
            {
                uint32 data = r_u32(cursor + 8);
                uint32 game = r_u32(0x80097048);
                uint32 resource = G3FindLevelResource800397B0(data, game);
                w_u32(settings + 0x188, resource);
            }
            break;
        case 0x52:
            {
                uint32 bytes = r_u16(cursor + 0xc);
                uint32 data = r_u32(cursor + 8);
                resource_count = (sint16)bytes >> 3;
                w_u32(settings + 0x194, data);
            }
            break;
        case 0x5a:
            {
                uint32 bytes = r_u16(cursor + 0xc);
                uint32 data = r_u32(cursor + 8);
                w_u32(settings + 0x198, data);
                vector_count = (sint16)bytes / 6;
            }
            break;
        case 0x5c:
            {
                uint32 bytes = r_u16(cursor + 0xc);
                uint32 data = r_u32(cursor + 8);
                index_count = (sint16)bytes >> 1;
                w_u32(settings + 0x19c, data);
            }
            break;
        }
    } while (G3HankNextField0140(cursor, remaining, frame) != 0);
    sint32 count = resource_count < vector_count ? resource_count : vector_count;
    if (index_count < count) count = index_count;
    if (count < 0) count = 0;
    w_u16(settings + 0x192, (uint32)count);
}

void G3HankLoadAnimationList2348(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002348, "G3L_33");
    uint32 frame = guest_stack - 0x28;
    uint32 initial = r_u32(state + 0x28);
    if (initial == 0)
    {
        w_u32(settings + 0x1a0, r_u32(cursor));
        while (G3HankNextField0140(cursor, remaining, frame) != 0) {}
        uint32 next = r_u32(remaining);
        if (next != 0) w_u16(settings + 0x1bc, r_u16(cursor + 4));
        else w_u16(settings + 0x1bc, 0xffff);
        w_u32(state + 0x28, 0x29a);
    }
    else if (initial == 0x29a)
    {
        w_u32(state + 0x28, 0x16062);
        while (G3HankNextField0140(cursor, remaining, frame) != 0) {}
    }
}

void G3HankLoadBounds240C(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000240c, "G3L_33");
    uint32 frame = guest_stack - 0x28;
    uint32 initial = r_u32(state + 0x30);
    if (initial == 0)
    {
        w_u16(settings + 0x1f4, 9000);
        w_u16(settings + 0x1f8, 8000);
        w_u16(settings + 0x200, (uint32)-32000);
        w_u16(settings + 0x1f6, (uint32)-9000);
        w_u16(settings + 0x1fa, (uint32)-9000);
        w_u32(state + 0x30, 0x29a);
    }
    else if (initial == 0x29a) w_u32(state + 0x30, 0x16062);
    do
    {
        switch (r_s16(cursor + 6))
        {
        case 0x6c: w_u16(settings + 0x1fa, r_u16(r_u32(cursor + 8))); break;
        case 0x92: w_u16(settings + 0x1f4, r_u16(r_u32(cursor + 8))); break;
        case 0x93: w_u16(settings + 0x1f6, r_u16(r_u32(cursor + 8))); break;
        case 0x94: w_u16(settings + 0x1f8, r_u16(r_u32(cursor + 8))); break;
        case 0xa2:
            {
                uint32 data = r_u32(cursor + 8);
                uint32 z = r_u16(state - 0xa8);
                uint32 offset = r_u16(data);
                w_u16(settings + 0x200, z + offset);
            }
            break;
        }
    } while (G3HankNextField0140(cursor, remaining, frame) != 0);
}

uint32 G3HankLoadConfiguration2564(uint32 state, uint32 resource, uint32 settings, uint32 bytes, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002564, "G3L_33");
    uint32 frame = guest_stack - 0x40;
    w_u32(frame + 0x20, 1);
    w_u32(frame + 0x10, resource);
    w_u16(frame + 0x1e, bytes);
    for (;;)
    {
        G3HankNextField0140(frame + 0x10, frame + 0x20, frame);
        if (r_u32(frame + 0x20) == 0) return r_u32(frame + 0x10);
        sint32 kind = r_s16(frame + 0x16);
        if (kind == -1) break;
        if (kind == 0x98) w_u16(state + 0x36, 1);
        else if (kind == 0x9b)
        {
            uint32 data = r_u32(frame + 0x18);
            sint32 distance = r_s16(data);
            w_u32(state + 0x38, (uint32)(distance * distance));
        }
    }
    for (;;)
    {
        sint32 kind = (sint16)(r_u16(frame + 0x14) - 0x42);
        switch (kind)
        {
        case 0: G3HankLoadCore01CC(state, settings, frame + 0x10, frame + 0x20, frame); break;
        case 1: G3HankLoadIdle0584(state, settings, frame + 0x10, frame + 0x20, frame); break;
        case 2: G3HankLoadPursuit08A4(state, settings, frame + 0x10, frame + 0x20, frame); break;
        case 3: G3HankLoadAttack0B30(state, settings, frame + 0x10, frame + 0x20, frame); break;
        case 4: G3HankLoadLeap0C9C(state, settings, frame + 0x10, frame + 0x20, frame); break;
        case 5: G3HankLoadSpecial0FCC(state, settings, frame + 0x10, frame + 0x20, frame); break;
        case 6: G3HankLoadShooter1364(state, settings, frame + 0x10, frame + 0x20, module_base, frame); break;
        case 7: G3HankLoadHurt15E4(state, settings, frame + 0x10, frame + 0x20, frame); break;
        case 8: G3HankLoadMove178C(state, settings, frame + 0x10, frame + 0x20, frame); break;
        case 9: G3HankLoadDeath1EEC(state, settings, frame + 0x10, frame + 0x20, frame); break;
        case 0xa: G3HankLoadAnimationList2348(state, settings, frame + 0x10, frame + 0x20, frame); break;
        case 0xb: G3HankLoadRun1BCC(state, settings, frame + 0x10, frame + 0x20, frame); break;
        case 0xc: G3HankLoadBounds240C(state, settings, frame + 0x10, frame + 0x20, frame); break;
        default: w_u32(frame + 0x20, 0); break;
        }
        if (r_u32(frame + 0x20) == 0) return r_u32(frame + 0x10);
    }
}

void G3HankSelectAnimation27EC(uint32 state, uint32 animation, uint32 preserve, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000027ec, "G3L_33");
    uint32 frame = guest_stack - 0x40;
    if ((sint16)animation < 1000)
    {
        uint32 script = r_u32(state + 0x28);
        w_u16(state - 0x96, animation);
        if (script != 0) w_u32(script + 4, 0);
        w_u16(state - 0xa6, 0);
        return;
    }
    uint32 script = r_u32(state + 0x28);
    uint32 selected = animation - 1000;
    if (script == 0)
    {
        w_u16(state - 0x96, selected);
        w_u16(state - 0xa6, 0);
        return;
    }
    w_u32(frame + 0x18, r_u32(script));
    sint32 index = (sint16)selected;
    if (index >= 0)
    {
        sint32 count = 0;
        do
        {
            uint32 stop = (uint32)r_s16(script + 0x1c);
            w_u32(frame + 0x10, stop);
            uint32 result = G3Function80085D40(frame + 0x18, 0x81, frame + 0x1c, frame + 0x20, stop, frame);
            ++count;
            if (result == 0)
            {
                w_u16(state - 0x96, 0);
                w_u16(state - 0xa6, 0);
                w_u32(script + 4, 0);
                return;
            }
        } while (count <= index);
    }
    uint32 keep = preserve & 1;
    w_u16(script + 0x1e, 0);
    w_u16(script + 0xe, 0);
    w_u16(script + 0x1a, 0);
    w_u16(script + 0x18, 0);
    uint32 data = r_u32(frame + 0x1c);
    uint32 bytes = r_u16(frame + 0x20);
    w_u16(script + 0x12, 0xffff);
    w_u16(script + 0x10, 0xffff);
    w_u16(script + 0x20, 0xffff);
    w_u32(script + 8, 0);
    w_u32(script + 4, data);
    w_u16(script + 0xc, (uint32)((sint16)bytes >> 1));
    for (;;)
    {
        uint32 result = G3Function80085DFC(frame + 0x18, frame + 0x22, frame + 0x1c, frame + 0x20);
        if (result == 0) break;
        sint32 kind = r_s16(frame + 0x22);
        uint32 kind_bits = r_u16(frame + 0x22);
        if (kind == 0x81 || kind_bits - 0x42 < 0xd) break;
        switch ((sint16)(kind_bits - 0x82))
        {
        case 0:
            data = r_u32(frame + 0x1c);
            w_u16(script + 0x10, r_u16(data));
            w_u16(script + 0x12, r_u16(data + 2));
            {
                uint32 end = r_u16(data + 4);
                w_u16(script + 0x14, end);
                w_u16(script + 0x16, end);
            }
            break;
        case 2:
            if (keep != 0) w_u16(script + 0xe, r_u16(r_u32(frame + 0x1c)));
            break;
        case 0xa: w_u16(script + 0x1e, 1); break;
        case 0xb: w_u16(script + 0x1e, 2); break;
        case 0x15: w_u16(script + 0x20, r_u16(r_u32(frame + 0x1c))); break;
        case 0x2b: w_u32(script + 8, r_u32(frame + 0x1c)); break;
        }
    }
    uint32 offset = (uint32)r_s16(script + 0xe);
    uint32 table = r_u32(script + 4);
    uint32 sequence = r_u16(table + offset * 2);
    w_u16(state - 0xa6, 0);
    w_u16(state - 0x96, sequence);
    w_u16(state - 0xa6, 0);
}

void G3HankUpdateAnimation2A1C(uint32 state, uint32 selector, uint32 completed, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002a1c, "G3L_33");
    uint32 frame = guest_stack - 0x40;
    uint32 flags = r_u32(state - 0xf0);
    w_u32(state - 0xf0, flags & 0xffffffef);
    G3Function80029F0C(state - 0x104, (uint32)(sint16)selector, frame);
    uint32 settings = r_u32(state + 0x28);
    if (settings == 0 || r_u32(settings + 4) == 0)
    {
        if (completed != 0)
            w_u32(completed, 1);
        return;
    }
    uint32 sound = r_u32(settings + 8);
    if (sound != 0)
    {
        sint32 current = r_s16(settings + 0xe);
        sint32 trigger = r_s16(sound);
        if (current == trigger)
        {
            sint32 phase = r_s16(state - 0xa6);
            sint32 sound_phase = r_s16(sound + 2);
            if (phase == sound_phase)
            {
                uint32 mode = (uint32)r_s16(sound + 4);
                sint32 percent = r_s16(sound + 6);
                uint32 parameter = (uint32)r_s16(sound + 8);
                w_u32(frame + 0x18, state - 0xac);
                uint32 player = r_u32(0x8009efc4);
                w_u32(frame + 0x14, 0);
                w_u32(frame + 0x10, parameter);
                w_u32(frame + 0x1c, player + 0x58);
                parameter = (uint32)r_s16(sound + 0xa);
                w_u32(frame + 0x20, parameter);
                parameter = (uint32)r_s16(sound + 0xc);
                w_u32(frame + 0x28, 0);
                w_u32(frame + 0x24, parameter);
                uint32 strength = (uint32)(sint16)((percent * 4096) / 100);
                G3Function80081A90(0, mode, 0xffffffff, strength, frame);
            }
        }
    }
    flags = r_u32(state - 0xf0);
    if ((flags & 0x10) == 0)
        w_u16(settings + 0x1a, 0);
    else
    {
        uint32 mode = r_u32(state + 0x4c);
        if (mode == 8)
            w_u16(r_u32(state + 0x10) + 0x2c, 0);
        else if (mode == 9)
            w_u16(r_u32(state + 0x14) + 0x20, 0);
        sint32 current = r_s16(settings + 0xe);
        sint32 end = r_s16(settings + 0x12);
        bool repeat = false;
        if (current == end && r_s16(settings + 0x18) == 0)
        {
            if (r_s16(settings + 0x14) <= 0)
                repeat = true;
            else
            {
                uint32 counter = r_u16(settings + 0x16) - 1;
                w_u16(settings + 0x16, counter);
                repeat = (sint16)counter > 0;
            }
        }
        if (repeat)
            w_u16(settings + 0xe, r_u16(settings + 0x10));
        else
        {
            uint32 next = r_u16(settings + 0xe);
            sint32 count = r_s16(settings + 0xc);
            ++next;
            w_u16(settings + 0xe, next);
            sint32 index = (sint16)next;
            if (index >= count)
            {
                uint32 reset = r_u16(settings + 0x14);
                w_u16(settings + 0x18, 0);
                w_u16(settings + 0x1a, 1);
                w_u16(settings + 0xe, 0);
                w_u16(settings + 0x16, reset);
            }
            else if (index == r_s16(settings + 0x10) && r_s16(settings + 0x18) != 0)
            {
                sint32 kind = r_s16(settings + 0x1e);
                bool finish = kind == 1;
                if (kind == 2)
                {
                    uint32 resume = r_u16(settings + 0x12) + 1;
                    if ((sint16)resume < count)
                        w_u16(settings + 0xe, resume);
                    else
                        finish = true;
                }
                if (finish)
                {
                    w_u16(settings + 0x18, 0);
                    w_u16(settings + 0x1a, 1);
                    w_u16(settings + 0xe, 0);
                }
            }
        }
        sint32 index = r_s16(settings + 0xe);
        uint32 animations = r_u32(settings + 4);
        uint32 animation = r_u16(animations + (uint32)index * 2);
        w_u16(state - 0xa6, 0);
        w_u16(state - 0x96, animation);
    }
    if (completed != 0)
        w_u32(completed, (uint32)r_s16(settings + 0x1a));
}

uint32 G3HankAnimationReady2CC4(uint32 state)
{
    FUNCTION_MARKER(0x00002cc4, "G3L_33");
    uint32 script = r_u32(state + 0x28);
    if (script != 0 && r_u32(script + 4) != 0) return (uint32)r_s16(script + 0x1a);
    return r_u32(state - 0xf0) & 0x10;
}

void G3HankFinishAnimation2CFC(uint32 state)
{
    FUNCTION_MARKER(0x00002cfc, "G3L_33");
    uint32 script = r_u32(state + 0x28);
    if (script != 0 && r_u32(script + 4) != 0) w_u16(script + 0x18, 1);
}

uint32 G3HankTraceVertical2D28(uint32 state, uint32 settings, uint32 floor, uint32 relative_floor, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002d28, "G3L_33");
    uint32 frame = guest_stack - 0x30;
    uint32 coordinate = r_u16(state - 0xac);
    w_u16(frame + 0x18, coordinate);
    w_u16(frame + 0x10, coordinate);
    coordinate = r_u16(state - 0xaa);
    w_u16(frame + 0x1a, coordinate);
    w_u16(frame + 0x12, coordinate);
    coordinate = r_u16(settings + 8);
    uint32 low = r_u16(settings);
    w_u16(frame + 0x14, coordinate + low);
    coordinate = r_u16(settings + 8);
    uint32 high = r_u16(settings + 2);
    w_u16(frame + 0x1c, coordinate + high);
    uint32 game = r_u32(0x80097048);
    uint32 level = r_u32(game + 0x48);
    uint32 world = r_u32(level);
    if (G3Function800622EC(world, frame + 0x18, frame + 0x10, state - 0x104, frame) == 0) return 0;
    if (floor != 0) w_u16(floor, r_u16(frame + 0x1c));
    if (relative_floor != 0)
    {
        uint32 height = r_u16(frame + 0x1c);
        uint32 origin = r_u16(settings + 8);
        w_u16(relative_floor, height - origin);
    }
    return 1;
}

uint32 G3HankSnapFloor2DFC(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002dfc, "G3L_33");
    uint32 frame = guest_stack - 0x28;
    uint32 settings = r_u32(state + 0x30);
    if (settings == 0) return 1;
    uint32 z = r_u16(state - 0xa8);
    w_u16(settings + 8, z);
    if (G3HankTraceVertical2D28(state, settings, frame + 0x10, 0, frame) == 0) return 0;
    z = r_u16(frame + 0x10);
    w_u16(settings + 8, z);
    w_u16(state - 0xa8, z);
    w_u16(settings + 0xa, 0);
    return 1;
}

uint32 G3HankUpdateFloor2E68(uint32 state, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00002e68, "G3L_33");
    uint32 frame = guest_stack - 0x30;
    uint32 settings = r_u32(state + 0x30);
    uint32 result = 1;
    if (settings == 0)
        return result;
    if (G3HankTraceVertical2D28(state, settings, frame + 0x10, frame + 0x12, frame) == 0)
    {
        uint32 source = state - 0x94;
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
        if (G3HankTraceVertical2D28(state, settings, frame + 0x10, frame + 0x12, frame) == 0)
        {
            uint32 mode = r_u32(state + 0x4c);
            if (mode == 0x10 || mode == 0x12 || mode == 0x13)
                return 0;
            uint32 velocity = r_u16(settings + 0xa);
            uint32 acceleration = r_u16(settings + 6);
            uint32 height = r_u16(settings + 8);
            velocity += acceleration;
            height += velocity;
            w_u16(settings + 0xa, velocity);
            w_u16(settings + 8, height);
            w_u16(state - 0xa8, height);
            sint32 minimum = r_s16(settings + 0xc);
            if (minimum >= (sint16)height)
                G3HankChooseDeath4520(state, 0, 0, module_base, frame);
            return 0;
        }
        result = 0;
    }
    sint32 relative = r_s16(frame + 0x12);
    uint32 relative_half = r_u16(frame + 0x12);
    if (relative == 0)
        w_u16(settings + 0xa, 0);
    else if (relative > 0)
    {
        sint32 step = r_s16(settings + 4);
        uint32 step_half = r_u16(settings + 4);
        if (step < relative)
        {
            uint32 height = r_u16(settings + 8);
            w_u16(frame + 0x10, height + step_half);
            w_u16(settings + 0xa, step_half);
        }
        else
            w_u16(settings + 0xa, relative_half);
    }
    else
    {
        uint32 velocity = r_u16(settings + 0xa);
        uint32 acceleration = r_u16(settings + 6);
        uint32 height = r_u16(settings + 8);
        velocity += acceleration;
        height += velocity;
        w_u16(settings + 0xa, velocity);
        sint32 floor = r_s16(frame + 0x10);
        if ((sint16)height < floor)
            w_u16(settings + 0xa, relative_half);
        else
            w_u16(frame + 0x10, height);
    }
    uint32 floor = r_u16(frame + 0x10);
    w_u16(settings + 8, floor);
    w_u16(state - 0xa8, floor);
    return result;
}

void G3HankEnterAttached3020(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003020, "G3L_33");
    uint32 frame = guest_stack - 0x20;
    uint32 previous = r_u32(state + 0x4c);
    uint32 settings = r_u32(state);
    bool change = previous != 1;
    if (!change)
    {
        uint32 current = r_u16(state - 0x96);
        sint32 animation = r_s16(settings + 0xe);
        change = current != (uint32)animation;
    }
    if (change) G3HankSelectAnimation27EC(state, (uint32)r_s16(settings + 0xe), 0, frame);
    w_u32(state + 0x4c, 0);
    w_u16(settings + 0x1c, 0);
    w_u16(settings + 0x18, 0);
    w_u16(settings + 0x1e, 0);
}

void G3HankEnterAttachment308C(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000308c, "G3L_33");
    uint32 frame = guest_stack - 0x80;
    uint32 core = r_u32(state);
    w_u32(state + 0x4c, 1);
    G3HankSelectAnimation27EC(state, (uint32)r_s16(core + 0x12), 0, frame);
    sint32 factor = r_s16(core + 0x20);
    if (factor <= 0 || factor >= 0x1000)
    {
        uint32 position = frame + (factor <= 0 ? 0x18 : 0x28);
        uint32 own = r_u16(state - 0xac);
        uint32 origin = r_u16(core + 8);
        w_u16(position, own - origin);
        own = r_u16(state - 0xaa);
        origin = r_u16(core + 0xa);
        w_u16(position + 2, own - origin);
        own = r_u16(state - 0xa8);
        origin = r_u16(core + 0xc);
        w_u16(position + 4, own - origin);
        uint32 channel = r_u32(core);
        uint32 sample;
        if (factor <= 0)
        {
            G3Function80040A40(channel, position, core + 4, frame + 0x20, frame);
            sample = frame + 0x20;
        }
        else
            sample = G3Function80040280(channel, position, frame + 0x30, 1, frame);
        uint32 value = r_u16(sample);
        origin = r_u16(core + 8);
        w_u16(core + 0x26, value + origin);
        value = r_u16(sample + 2);
        origin = r_u16(core + 0xa);
        w_u16(core + 0x28, value + origin);
        value = r_u16(sample + 4);
        origin = r_u16(core + 0xc);
        w_u16(core + 0x32, 0);
        w_u16(core + 0x2a, value + origin);
    }
    else
    {
        uint32 own = r_u16(state - 0xac);
        uint32 origin = r_u16(core + 8);
        w_u16(frame + 0x48, own - origin);
        own = r_u16(state - 0xaa);
        origin = r_u16(core + 0xa);
        w_u16(frame + 0x4a, own - origin);
        own = r_u16(state - 0xa8);
        origin = r_u16(core + 0xc);
        w_u16(frame + 0x4c, own - origin);
        uint32 channel = r_u32(core);
        uint32 sample = G3Function80040280(channel, frame + 0x48, frame + 0x60, 1, frame);
        uint32 words[2];
        for (uint32 word = 0; word < 2; ++word)
        {
            uint32 source = sample + word * 4;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 lane = source & 3;
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            words[word] = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        }
        for (uint32 word = 0; word < 2; ++word)
        {
            uint32 destination = frame + 0x40 + word * 4;
            uint32 high_address = destination + 3;
            uint32 high_lane = high_address & 3;
            w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            uint32 lane = destination & 3;
            w_masked_u32(destination & 0xfffffffc, words[word] << (lane * 8), (15 << lane) & 15);
        }
        uint32 x = r_u16(frame + 0x40);
        own = r_u16(state - 0xac);
        uint32 y = r_u16(frame + 0x42);
        x -= own;
        w_u16(frame + 0x58, x);
        own = r_u16(state - 0xaa);
        uint32 z = r_u16(frame + 0x44);
        y -= own;
        w_u16(frame + 0x5a, y);
        own = r_u16(state - 0xa8);
        z -= own;
        w_u16(frame + 0x5c, z);
        origin = r_u16(core + 8);
        x += origin;
        w_u16(frame + 0x58, x);
        origin = r_u16(core + 0xa);
        y += origin;
        w_u16(frame + 0x5a, y);
        origin = r_u16(core + 0xc);
        z += origin;
        w_u16(frame + 0x5c, z);
        factor = r_s16(core + 0x20);
        sint32 scaled_x = ((sint32)(sint16)x * factor) >> 12;
        w_u16(frame + 0x58, (uint32)scaled_x);
        factor = r_s16(core + 0x20);
        sint32 scaled_y = ((sint32)(sint16)y * factor) >> 12;
        w_u16(frame + 0x5a, (uint32)scaled_y);
        own = r_u16(state - 0xac);
        w_u16(core + 0x26, own + (uint32)scaled_x);
        own = r_u16(state - 0xaa);
        y = r_u16(frame + 0x5a);
        origin = r_u16(core + 8);
        w_u16(core + 0x28, own + y);
        own = r_u16(state - 0xa8);
        z = r_u16(frame + 0x5c);
        x = r_u16(core + 0x26);
        x -= origin;
        w_u16(core + 0x2a, own + z);
        w_u16(frame + 0x50, x);
        y = r_u16(core + 0x28);
        origin = r_u16(core + 0xa);
        w_u16(frame + 0x52, y - origin);
        z = r_u16(core + 0x2a);
        origin = r_u16(core + 0xc);
        w_u16(frame + 0x54, z - origin);
        w_u32(frame + 0x10, core + 4);
        w_u32(frame + 0x14, frame + 0x38);
        channel = r_u32(core);
        G3Function80040654(channel, frame + 0x50, frame + 0x40, frame + 0x60, frame);
        x = r_u16(frame + 0x38);
        origin = r_u16(core + 8);
        w_u16(core + 0x2c, x + origin);
        y = r_u16(frame + 0x3a);
        origin = r_u16(core + 0xa);
        w_u16(core + 0x2e, y + origin);
        z = r_u16(frame + 0x3c);
        origin = r_u16(core + 0xc);
        w_u16(core + 0x32, 1);
        w_u16(core + 0x30, z + origin);
    }
    w_u16(core + 0x1c, 0);
}

void G3HankEnterPursuit33BC(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000033bc, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    uint32 previous = r_u32(state + 0x4c);
    uint32 settings = r_u32(state + 8);
    w_u32(state + 0x4c, 6);
    uint32 animation = (uint32)r_s16(settings + 0xa);
    G3HankSelectAnimation27EC(state, animation, previous - 6 < 10 ? 1 : 0, frame);
    w_u16(settings + 0x10, 0);
    w_u16(settings + 0x14, 0);
    w_u16(settings + 0x16, 0);
}

void G3HankEnterAlert3404(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003404, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    uint32 settings = r_u32(state + 4);
    w_u32(state + 0x4c, 3);
    sint32 animation = r_s16(settings + 0x22);
    if (animation >= 0) G3HankSelectAnimation27EC(state, (uint32)animation, 0, frame);
}

void G3HankEnterAttack3440(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003440, "G3L_33");
    uint32 frame = guest_stack - 0x20;
    uint32 settings = r_u32(state + 0xc);
    w_u32(state + 0x4c, 7);
    sint32 animation = r_s16(settings + 6);
    if (animation >= 0 && (G3SdkRandom80027FA8() & 1) != 0) animation = r_s16(settings + 6);
    else animation = r_s16(settings + 4);
    G3HankSelectAnimation27EC(state, (uint32)animation, 0, frame);
}

void G3HankEnterIdle34B0(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000034b0, "G3L_33");
    uint32 frame = guest_stack - 0x20;
    uint32 settings = r_u32(state + 4);
    w_u32(state + 0x4c, 2);
    sint32 random = (sint32)(G3SdkRandom80027FA8() & 0xfff);
    uint32 animation;
    bool resting = false;
    if (random < r_s16(settings + 0xc)) animation = (uint32)r_s16(settings + 2);
    else if (random < r_s16(settings + 0xe)) animation = (uint32)r_s16(settings + 4);
    else if (random < r_s16(settings + 0x10)) animation = (uint32)r_s16(settings + 6);
    else
    {
        animation = (uint32)r_s16(settings);
        resting = true;
    }
    G3HankSelectAnimation27EC(state, animation, 0, frame);
    w_u16(settings + 0x1a, resting ? 1 : 0);
    w_u16(settings + 0x12, 0);
    w_u16(settings + 0x16, 0);
}

void G3HankEnterLeap356C(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000356c, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    uint32 settings = r_u32(state + 0x10);
    w_u32(state + 0x4c, 8);
    uint32 animation = (uint32)r_s16(settings + 0xc);
    G3HankSelectAnimation27EC(state, animation, 0, frame);
    w_u16(settings + 0x2c, 0);
}

void G3HankEnterSpecial35A4(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000035a4, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    uint32 settings = r_u32(state + 0x14);
    w_u32(state + 0x4c, 9);
    uint32 animation = (uint32)r_s16(settings + 0xc);
    G3HankSelectAnimation27EC(state, animation, 0, frame);
    w_u16(settings + 0x20, 0);
}

void G3HankEnterFall35DC(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000035dc, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    uint32 settings = r_u32(state + 0x18);
    w_u32(state + 0x4c, 10);
    uint32 animation = (uint32)r_s16(settings + 0xc);
    G3HankSelectAnimation27EC(state, animation, 0, frame);
    w_u16(settings + 0x14, 0);
}

void G3HankEnterMove3614(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003614, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    uint32 previous = r_u32(state + 0x4c);
    uint32 settings = r_u32(state + 0x1c);
    w_u32(state + 0x4c, 0xb);
    uint32 animation = (uint32)r_s16(settings + 0x10);
    G3HankSelectAnimation27EC(state, animation, previous - 6 < 10 ? 1 : 0, frame);
    w_u16(settings + 0x3a, 0);
}

void G3HankEnterStop3654(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003654, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    w_u32(state + 0x4c, 0xc);
    uint32 settings = r_u32(state + 0x1c);
    uint32 animation = (uint32)r_s16(settings + 0x12);
    G3HankSelectAnimation27EC(state, animation, 0, frame);
}

void G3HankEnterTurn3688(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003688, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    w_u32(state + 0x4c, 0xd);
    uint32 settings = r_u32(state + 0x1c);
    uint32 animation = (uint32)r_s16(settings + 0x14);
    G3HankSelectAnimation27EC(state, animation, 0, frame);
}

void G3HankEnterWait36BC(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000036bc, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    uint32 settings = r_u32(state + 0x1c);
    w_u32(state + 0x4c, 4);
    uint32 animation = (uint32)r_s16(settings + 0x16);
    G3HankSelectAnimation27EC(state, animation, 0, frame);
    w_u16(settings + 0x3a, 0);
}

void G3HankEnterRun36F4(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000036f4, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    uint32 previous = r_u32(state + 0x4c);
    w_u32(state + 0x4c, 0xe);
    uint32 settings = r_u32(state + 0x2c);
    uint32 animation = (uint32)r_s16(settings + 0x20);
    G3HankSelectAnimation27EC(state, animation, previous - 6 < 10 ? 1 : 0, frame);
}

void G3HankEnterPause372C(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000372c, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    w_u32(state + 0x4c, 0x5);
    uint32 settings = r_u32(state + 0x2c);
    uint32 animation = (uint32)r_s16(settings + 0x24);
    G3HankSelectAnimation27EC(state, animation, 0, frame);
}

void G3HankEnterHurt3760(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003760, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    w_u32(state + 0x4c, 0x14);
    uint32 settings = r_u32(state + 0x20);
    uint32 animation = (uint32)r_s16(settings + 0x8);
    G3HankSelectAnimation27EC(state, animation, 0, frame);
}

void G3HankEnterKnockback3794(uint32 state, uint32 direction, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003794, "G3L_33");
    uint32 frame = guest_stack - 0x20;
    uint32 settings = r_u32(state + 0x20);
    w_u32(state + 0x4c, 0x15);
    uint32 animation = (uint32)r_s16(settings + 0xa);
    G3HankSelectAnimation27EC(state, animation, 0, frame);
    uint32 duration = r_u16(settings + 0xc);
    w_u16(settings + 0x14, direction);
    w_u16(settings + 0x12, duration);
}

void G3HankEnterDying37E0(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000037e0, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    uint32 settings = r_u32(state + 0x24);
    uint32 previous = r_u16(state + 0x4c);
    w_u16(settings + 0x12, previous);
    w_u32(state + 0x4c, 0xf);
    uint32 animation = (uint32)r_s16(settings + 4);
    G3HankSelectAnimation27EC(state, animation, 0, frame);
}

void G3HankEnterDeath381C(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000381c, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    w_u32(state + 0x4c, 0x10);
    uint32 settings = r_u32(state + 0x24);
    uint32 animation = (uint32)r_s16(settings + 0x8);
    G3HankSelectAnimation27EC(state, animation, 0, frame);
}

void G3HankEnterDeathTail3850(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003850, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    w_u32(state + 0x4c, 0x11);
    uint32 settings = r_u32(state + 0x24);
    uint32 animation = (uint32)r_s16(settings + 0xa);
    G3HankSelectAnimation27EC(state, animation, 0, frame);
}

void G3HankEnterRemoved3884(uint32 state)
{
    FUNCTION_MARKER(0x00003884, "G3L_33");
    uint32 settings = r_u32(state + 0x24);
    w_u32(state + 0x4c, 0x13);
    w_u16(settings + 0x10, 0);
}

void G3HankChooseInitial3898(uint32 state, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003898, "G3L_33");
    uint32 frame = guest_stack - 0x20;
    uint32 settings = r_u32(state);
    if (settings != 0)
    {
        uint32 hurt = r_u32(state + 0x20);
        if (hurt != 0 && r_s16(hurt + 6) != 0 && r_s16(hurt + 0x18) != 0) G3HankEnterAttachment308C(state, frame);
        else
        {
            G3HankEnterAttached3020(state, frame);
            w_u16(settings + 4, 0);
            w_u32(settings + 4, r_u16(settings + 4));
        }
    }
    else if (r_u32(state + 4) != 0) G3HankEnterIdle34B0(state, frame);
    else if (r_u32(state + 8) != 0) G3HankEnterPursuit33BC(state, frame);
    else if (r_u32(state + 0x1c) != 0) G3HankEnterMove3614(state, frame);
    else w_u32(state + 0x4c, 0x16);
    G3HankSnapFloor2DFC(state, frame);
}

void G3HankChooseHurt3994(uint32 state, uint32 forced, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003994, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    uint32 hurt = r_u32(state + 0x20);
    if (hurt != 0)
    {
        if (r_s16(hurt + 0x16) == 0 && forced == 0) w_u16(hurt + 0x18, 0);
        else
        {
            sint32 relocate = r_s16(hurt + 6);
            w_u16(hurt + 0x18, 1);
            if (relocate != 0)
            {
                uint32 old_z = r_u16(state - 0xa8);
                {
                    uint32 source = hurt;
                    uint32 destination = state - 0xac;
                    uint32 address = source;
                    uint32 high_address = address + 3;
                    uint32 high_lane = high_address & 3;
                    uint32 lane = address & 3;
                    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                    uint32 low = r_masked_u32(address & 0xfffffffc, (15 << lane) & 15);
                    uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
                    uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
                    uint32 z = (uint32)r_s16(source + 4);
                    address = destination;
                    high_address = address + 3;
                    high_lane = high_address & 3;
                    lane = address & 3;
                    w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                    w_masked_u32(address & 0xfffffffc, value << (lane * 8), (15 << lane) & 15);
                    w_u16(destination + 4, z);
                }
                if (r_s16(hurt + 0xa) >= 0)
                {
                    sint32 target_z = r_s16(hurt + 4);
                    if (target_z != (sint16)old_z)
                    {
                        G3HankEnterKnockback3794(state, (uint32)(sint16)old_z, frame);
                        return;
                    }
                }
            }
            if (r_s16(hurt + 8) >= 0)
            {
                G3HankEnterHurt3760(state, frame);
                return;
            }
        }
    }
    G3HankChooseInitial3898(state, frame);
}

void G3HankInitialize3A74(uint32 object, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00003a74, "G3L_33");
    uint32 frame = guest_stack - 0x228;
    uint32 resource = r_u32(object + 0x28);
    uint32 state = object + 0x104;
    if (resource == 0)
    {
        w_u32(state + 0x4c, 0x16);
        return;
    }
    w_u32(state, 0);
    w_u16(state + 0x34, 0xffff);
    for (uint32 slot = 4; slot <= 0x30; slot += 4)
        w_u32(state + slot, 0);
    w_u16(state + 0x36, 0);
    w_u32(state + 0x38, 0x3d090000);
    uint32 descriptor = r_u32(object + 0x1c);
    resource = r_u32(descriptor + 0x1c);
    G3HankLoadConfiguration2564(state, resource, frame + 0x10, 0xffffffff, module_base, frame);
    w_u32(frame + 0x178, 0);
    resource = r_u32(object + 0x28);
    if (r_s16(resource + 2) != 0)
    {
        w_u32(state + 0x4c, 0x16);
        return;
    }
    uint32 initial = r_u16(resource);
    w_u16(resource, 1);
    uint32 destination = G3HankLoadConfiguration2564(state, resource + 4, frame + 0x10, (uint32)(sint16)initial, module_base, frame);
    const uint32 sources[13] = {0x10, 0x58, 0x7c, 0x9c, 0xa8, 0xd8, 0x104, 0x11c, 0x15c, 0x178, 0x1b0, 0x1d4, 0x204};
    const uint32 sizes[13] = {0x48, 0x24, 0x20, 0xc, 0x30, 0x2c, 0x18, 0x40, 0x1c, 0x38, 0x24, 0x30, 0x10};
    for (uint32 slot = 0; slot < 13; ++slot)
    {
        if (r_u32(state + slot * 4) != 0x16062)
        {
            w_u32(state + slot * 4, 0);
            continue;
        }
        bool masked = slot == 8 || slot == 12 || (slot == 1 && (destination & 3) != 0);
        uint32 copied = 0;
        while (copied < sizes[slot])
        {
            uint32 count = (sizes[slot] - copied) / 4;
            if (count > 4)
                count = 4;
            bool masked_group = masked || (slot == 1 && copied == 0x20);
            uint32 words[4];
            for (uint32 word = 0; word < count; ++word)
            {
                uint32 source = frame + sources[slot] + copied + word * 4;
                if (masked_group)
                {
                    uint32 high_address = source + 3;
                    uint32 high_lane = high_address & 3;
                    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                    uint32 lane = source & 3;
                    uint32 low = r_masked_u32(source & 0xfffffffc, (15 << lane) & 15);
                    uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
                    words[word] = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
                }
                else
                    words[word] = r_u32(source);
            }
            for (uint32 word = 0; word < count; ++word)
            {
                uint32 output = destination + copied + word * 4;
                if (masked_group)
                {
                    uint32 high_address = output + 3;
                    uint32 high_lane = high_address & 3;
                    w_masked_u32(high_address & 0xfffffffc, words[word] >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                    uint32 lane = output & 3;
                    w_masked_u32(output & 0xfffffffc, words[word] << (lane * 8), (15 << lane) & 15);
                }
                else
                    w_u32(output, words[word]);
            }
            copied += count * 4;
        }
        w_u32(state + slot * 4, destination);
        destination += sizes[slot];
    }
    sint32 initial_mode = (sint16)initial;
    uint32 flags = r_u32(state - 0xf4);
    w_u32(state - 0xf4, flags | 0x400);
    G3HankChooseHurt3994(state, initial_mode != 1 ? 1 : 0, frame);
    uint32 death = r_u32(state + 0x24);
    if (death == 0)
        return;
    uint32 pair = r_u32(death);
    if (pair == 0)
        return;
    if (initial_mode == 0)
        w_u16(pair + 2, r_u16(pair));
    else if (initial_mode == 2)
        w_u16(pair, r_u16(pair + 2));
}

void G3HankNotifyEnable4150(uint32 owner, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004150, "G3L_33");
    uint32 frame = guest_stack - 0x20;
    w_u16(frame + 0x10, 1);
    w_u16(frame + 0x12, 1);
    uint32 object = r_u32(owner + 0x20);
    if (object != 0)
    {
        uint32 target = r_u32(object + 0xec);
        if (target != 0) G3DispatchStateEvent(target, object, r_u32(0x80097048), 9, frame + 0x10, frame);
    }
}

void G3HankNotifyDisable41A4(uint32 owner, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000041a4, "G3L_33");
    uint32 frame = guest_stack - 0x20;
    G3FillMemory80086094(frame + 0x10, 0, 4);
    uint32 object = r_u32(owner + 0x20);
    if (object != 0)
    {
        uint32 target = r_u32(object + 0xec);
        if (target != 0) G3DispatchStateEvent(target, object, r_u32(0x80097048), 9, frame + 0x10, frame);
    }
}

uint32 G3HankClampBounds4204(uint32 state, uint32 bounds)
{
    FUNCTION_MARKER(0x00004204, "G3L_33");
    if (bounds == 0)
        return 0;
    sint32 own = r_s16(state - 0xac);
    sint32 limit = r_s16(bounds);
    uint32 half = r_u16(bounds);
    uint32 changed = 0;
    bool clamp = own < limit;
    if (!clamp)
    {
        limit = r_s16(bounds + 6);
        half = r_u16(bounds + 6);
        clamp = limit < own;
    }
    if (clamp)
    {
        w_u16(state - 0xac, half);
        changed = 1;
    }
    own = r_s16(state - 0xaa);
    limit = r_s16(bounds + 2);
    half = r_u16(bounds + 2);
    if (own < limit)
    {
        w_u16(state - 0xaa, half);
        return 1;
    }
    limit = r_s16(bounds + 8);
    half = r_u16(bounds + 8);
    if (limit < own)
    {
        w_u16(state - 0xaa, half);
        return 1;
    }
    return changed;
}

void G3HankSpawnDeath428C(uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x0000428c, "G3L_33");
    uint32 frame = guest_stack - 0x38;
    uint32 previous_name = module_base + 0x8954;
    uint32 resource = 0;
    uint32 effect = r_u32(settings + 0x20);
    if (effect != 0)
    {
        uint32 descriptor = r_u32(effect + 0xc);
        uint32 data = r_u32(descriptor);
        uint32 dimensions = r_u32(data + 0x1c);
        uint32 width = r_u32(dimensions + 8);
        uint32 height = r_u32(dimensions + 0xc);
        uint32 pool = r_u32(0x800a27fc);
        G3Function800444E4(state - 0xac, data, pool, (uint32)(sint16)(width * height), frame);
    }
    sint32 remaining = r_s16(settings + 0x2a);
    if (remaining <= 0)
        return;
    uint32 name = r_u32(settings + 0x2c);
    uint32 position = r_u32(settings + 0x30);
    uint32 bone = r_u32(settings + 0x34);
    while (true)
    {
        uint32 previous = r_u32(previous_name);
        uint32 current = r_u32(name);
        bool same = previous == current;
        if (same)
        {
            previous = r_u32(previous_name + 4);
            current = r_u32(name + 4);
            same = previous == current;
        }
        if (!same)
        {
            uint32 game = r_u32(0x80097048);
            resource = G3FindLevelResource800397B0(name, game);
        }
        if (resource != 0)
        {
            uint32 spawned = G3Function8002A454(state - 0x104, resource, frame);
            if (spawned != 0)
            {
                sint32 bone_index = r_s16(bone);
                uint32 matrices = r_u32(state - 0xd4);
                uint32 matrix = matrices + (uint32)bone_index * 0x20;
                uint32 translated_x = 0;
                for (uint32 row = 0; row < 3; ++row)
                {
                    sint32 x = r_s16(position);
                    sint32 xx = r_s16(matrix + row * 6);
                    sint32 y = r_s16(position + 2);
                    sint32 yy = r_s16(matrix + row * 6 + 2);
                    sint32 z = r_s16(position + 4);
                    sint32 zz = r_s16(matrix + row * 6 + 4);
                    sint32 value = ((x * xx) >> 12) + ((y * yy) >> 12) + ((z * zz) >> 12);
                    if (row == 2)
                        translated_x = r_u16(spawned + 0x58);
                    w_u16(spawned + 0x58 + row * 2, (uint32)value);
                }
                uint32 translation = r_u16(matrix + 0x14);
                w_u16(spawned + 0x58, translated_x + translation);
                uint32 value = r_u16(spawned + 0x5a);
                translation = r_u16(matrix + 0x18);
                w_u16(spawned + 0x5a, value + translation);
                value = r_u16(spawned + 0x5c);
                translation = r_u16(matrix + 0x1c);
                w_u16(spawned + 0x5c, value + translation);
            }
        }
        --remaining;
        previous_name = name;
        if (remaining <= 0)
            break;
        name += 8;
        position += 6;
        bone += 2;
    }
}

void G3HankChooseDeath4520(uint32 state, uint32 primary, uint32 secondary, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004520, "G3L_33");
    uint32 frame = guest_stack - 0x18;
    uint32 settings = r_u32(state + 0x24);
    if (settings != 0)
    {
        if (primary != 0 && r_s16(settings + 8) >= 0)
        {
            G3HankEnterDeath381C(state, frame);
            return;
        }
        if (secondary != 0 && r_s16(settings + 0xa) >= 0)
        {
            G3HankEnterDeathTail3850(state, frame);
            return;
        }
        if (r_s16(settings + 0xe) >= 0)
        {
            G3HankSpawnDeath428C(state, settings, module_base, frame);
            uint32 flags = r_u32(state - 0xf4);
            w_u32(state - 0xf4, flags | 0x800);
            G3HankEnterRemoved3884(state);
            return;
        }
        G3HankSpawnDeath428C(state, settings, module_base, frame);
    }
    w_u32(state + 0x4c, 0x12);
}

uint32 G3HankAnimationMatches45E8(uint32 state, uint32 count, uint32 ranges)
{
    FUNCTION_MARKER(0x000045e8, "G3L_33");
    sint32 signed_count = (sint16)count;
    uint32 animation_half = r_u16(state - 0x96);
    uint32 phase_half = r_u16(state - 0xa6);
    if (signed_count == 0)
        return 1;
    bool exclude = signed_count < 0;
    sint32 remaining = exclude ? (sint16)(0 - count) : signed_count;
    if (remaining <= 0)
        return 1;
    sint32 animation = (sint16)animation_half;
    sint32 phase = (sint16)phase_half;
    for (sint32 index = 0; index < remaining; ++index)
    {
        sint32 candidate = r_s16(ranges);
        if (animation == candidate)
        {
            sint32 first = r_s16(ranges + 2);
            if (first < 0)
                return exclude ? 0 : 1;
            if (phase >= first)
            {
                sint32 last = r_s16(ranges + 4);
                if (last >= phase)
                    return exclude ? 0 : 1;
            }
        }
        ranges += 6;
    }
    return exclude ? 1 : 0;
}

uint32 G3HankProbeMove4720(uint32 state, uint32 distance, uint32 bounds, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004720, "G3L_33");
    uint32 frame = guest_stack - 0x38;
    for (uint32 copy = 0; copy < 2; ++copy)
    {
        uint32 source = copy == 0 ? state - 0xac : frame + 0x10;
        uint32 destination = copy == 0 ? frame + 0x10 : state - 0xac;
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
        if (copy == 0)
        {
            G3Function800361FC(state - 0x104, (uint32)(sint16)distance);
            uint32 settings = r_u32(state + 0x30);
            uint32 allowed = 1;
            if (settings != 0)
            {
                uint32 height = r_u16(settings + 8);
                uint32 velocity = r_u16(settings + 0xa);
                allowed = G3HankUpdateFloor2E68(state, module_base, frame);
                w_u16(settings + 8, height);
                w_u16(settings + 0xa, velocity);
            }
            if (allowed != 0 && bounds != 0)
                allowed = G3HankClampBounds4204(state, bounds) == 0 ? 1 : 0;
            distance = allowed;
        }
    }
    return distance;
}

uint32 G3HankDistanceSquared47F8(uint32 state, uint32 vector)
{
    FUNCTION_MARKER(0x000047f8, "G3L_33");
    sint32 include_height = r_s16(state + 0x36);
    sint32 x = r_s16(vector);
    uint32 result = (uint32)(x * x);
    sint32 y = r_s16(vector + 2);
    result += (uint32)(y * y);
    if (include_height != 0)
    {
        sint32 z = r_s16(vector + 4);
        result += (uint32)(z * z);
    }
    return result;
}

uint32 G3HankAtOrigin486C(uint32 state, uint32 include_height)
{
    FUNCTION_MARKER(0x0000486c, "G3L_33");
    uint32 object = state - 0x104;
    uint32 descriptor = r_u32(object + 0x20);
    if (descriptor == 0)
        return 0;
    uint32 origin = r_u32(descriptor + 0x10);
    uint32 position = r_u32(object + 0x58);
    if (origin != position)
        return 0;
    if (include_height != 0)
    {
        sint32 origin_z = r_s16(descriptor + 0x14);
        sint32 z = r_s16(object + 0x5c);
        if (origin_z != z)
            return 0;
    }
    return 1;
}

void G3HankAdvanceChannel48BC(uint32 channel, uint32 initial, uint32 count, uint32 output, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000048bc, "G3L_33");
    uint32 frame = guest_stack - 0x20;
    uint32 channel_count = r_u16(channel + 4);
    uint32 high_address = initial + 3;
    uint32 high_lane = high_address & 3;
    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
    uint32 lane = initial & 3;
    uint32 low = r_masked_u32(initial & 0xfffffffc, (15 << lane) & 15);
    uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
    uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
    uint32 destination = frame + 0x10;
    high_address = destination + 3;
    high_lane = high_address & 3;
    w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
    lane = destination & 3;
    w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15 << lane) & 15);
    sint32 index = r_s16(frame + 0x10);
    uint32 data = r_u32(channel);
    uint32 last = r_u16(data + (uint32)index * 0x20) - 1;
    if ((sint16)count > 0)
    {
        sint32 final_channel = (sint16)(channel_count - 2);
        do
        {
            uint32 packed = r_u32(frame + 0x10);
            uint32 point = packed >> 17;
            if (point == (uint32)(sint16)last)
            {
                w_u32(frame + 0x10, packed & 0x1ffff);
                index = r_s16(frame + 0x10);
                uint32 index_half = r_u16(frame + 0x10);
                w_u16(frame + 0x10, index == final_channel ? 0 : index_half + 1);
                index = r_s16(frame + 0x10);
                data = r_u32(channel);
                last = r_u16(data + (uint32)index * 0x20) - 1;
            }
            else
                w_u32(frame + 0x10, (packed & 0x1ffff) | ((point + 1) << 17));
            --count;
        } while ((sint16)count > 0);
    }
    G3Function80041008(channel, frame + 0x10, output, frame);
}

uint32 G3HankTurnToward49B4(uint32 angle, uint32 target, uint32 step, uint32 tolerance, uint32 guest_stack)
{
    FUNCTION_MARKER(0x000049b4, "G3L_33");
    uint32 current = (uint32)r_s16(angle);
    uint32 difference = G3Function80038B48(current, (uint32)(sint16)target);
    sint32 delta = (sint16)difference;
    sint32 radius = (sint16)tolerance;
    if (delta >= -radius && delta <= radius)
        return 1;
    uint32 adjustment;
    uint32 result = 1;
    if (delta > 0)
    {
        adjustment = difference - tolerance;
        if ((sint16)step < (sint16)adjustment)
        {
            adjustment = step;
            result = 0;
        }
    }
    else
    {
        adjustment = difference + tolerance;
        if ((sint16)adjustment < -(sint32)(sint16)step)
        {
            adjustment = 0 - step;
            result = 0;
        }
    }
    current = r_u16(angle);
    w_u16(angle, current + adjustment);
    return result;
}

void G3HankUpdateAttached4AA8(uint32 state, uint32 module_base, uint32 guest_stack)
{
    FUNCTION_MARKER(0x00004aa8, "G3L_33");
    uint32 frame = guest_stack - 0x58;
    uint32 core = r_u32(state);
    uint32 moving = 0;
    uint32 elapsed = r_u16(core + 0x18);
    uint32 idle_elapsed = r_u16(core + 0x1c);
    w_u16(core + 0x18, elapsed + 1);
    uint32 player = r_u32(0x8009efc4);
    w_u16(core + 0x1c, idle_elapsed + 1);
    {
        uint32 source = core + 4;
        uint32 destination = frame + 0x18;
        uint32 high_address = source + 3;
        uint32 high_lane = high_address & 3;
        uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
        uint32 lane = source & 3;
        uint32 low = r_masked_u32(source & 0xfffffffc, (15 << lane) & 15);
        uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
        uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
        
        high_address = destination + 3;
        high_lane = high_address & 3;
        w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
        lane = destination & 3;
        w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15 << lane) & 15);
        
    }
    uint32 channel = r_u32(core);
    uint32 point = G3Function8003FC34(channel, frame + 0x18, frame);
    uint32 player_position = player + 0x58;
    if (point != 0)
    {
        uint32 coordinate = r_u16(point);
        uint32 origin = r_u16(core + 8);
        w_u16(frame + 0x20, coordinate + origin);
        coordinate = r_u16(point + 2);
        origin = r_u16(core + 0xa);
        w_u16(frame + 0x22, coordinate + origin);
        coordinate = r_u16(point + 4);
        origin = r_u16(core + 0xc);
        w_u16(frame + 0x24, coordinate + origin);
        {
            uint32 source = core + 4;
            uint32 destination = frame + 0x30;
            uint32 high_address = source + 3;
            uint32 high_lane = high_address & 3;
            uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
            uint32 lane = source & 3;
            uint32 low = r_masked_u32(source & 0xfffffffc, (15 << lane) & 15);
            uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
            uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
            
            high_address = destination + 3;
            high_lane = high_address & 3;
            w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
            lane = destination & 3;
            w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15 << lane) & 15);
            
        }
        channel = r_u32(core);
        uint32 count = (uint32)r_s16(core + 0x44);
        G3HankAdvanceChannel48BC(channel, frame + 0x30, count, frame + 0x28, frame);
        uint32 x = r_u16(frame + 0x28);
        origin = r_u16(core + 8);
        uint32 y = r_u16(frame + 0x2a);
        x += origin;
        w_u16(frame + 0x28, x);
        origin = r_u16(core + 0xa);
        y += origin;
        uint32 z = r_u16(frame + 0x2c);
        w_u16(frame + 0x2a, y);
        origin = r_u16(core + 0xc);
        w_u16(frame + 0x2c, z + origin);
        coordinate = r_u16(state - 0xac);
        x -= coordinate;
        w_u16(frame + 0x10, x);
        coordinate = r_u16(state - 0xaa);
        y -= coordinate;
        w_u16(frame + 0x12, y);
        uint32 heading;
        if (((x | y) & 0xffff) != 0)
            heading = G3Function8002F240((uint32)(sint16)y, (uint32)(sint16)x, frame) + 0x400;
        else
            heading = r_u16(state - 0xa0);
        if (r_s16(core + 0x42) != 0)
            moving = 1;
        else
        {
            count = (uint32)r_s16(core + 0x36);
            uint32 ranges = r_u32(core + 0x3c);
            if (G3HankAnimationMatches45E8(state, count, ranges) != 0)
            {
                uint32 step = (uint32)r_s16(core + 0x16);
                uint32 tolerance = (uint32)r_s16(core + 0x46);
                if (G3HankTurnToward49B4(state - 0xa0, (uint32)(sint16)heading, step, tolerance, frame) != 0)
                    moving = 1;
            }
        }
        if (moving != 0)
        {
            count = (uint32)r_s16(core + 0x34);
            uint32 ranges = r_u32(core + 0x38);
            if (G3HankAnimationMatches45E8(state, count, ranges) != 0)
            {
                if (r_s16(frame + 0x18) == 0)
                {
                    uint32 flags = r_u32(state - 0xf4);
                    w_u32(state - 0xf4, flags & 0xfffffbff);
                }
                {
                    uint32 source = frame + 0x18;
                    uint32 destination = core + 4;
                    uint32 high_address = source + 3;
                    uint32 high_lane = high_address & 3;
                    uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                    uint32 lane = source & 3;
                    uint32 low = r_masked_u32(source & 0xfffffffc, (15 << lane) & 15);
                    uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
                    uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
                    
                    high_address = destination + 3;
                    high_lane = high_address & 3;
                    w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                    lane = destination & 3;
                    w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15 << lane) & 15);
                    
                }
                {
                    uint32 source = frame + 0x20;
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
                }
            }
        }
        if (G3HankUpdateFloor2E68(state, module_base, frame) == 0)
        {
            {
                uint32 source = frame + 0x30;
                uint32 destination = core + 4;
                uint32 high_address = source + 3;
                uint32 high_lane = high_address & 3;
                uint32 high = r_masked_u32(high_address & 0xfffffffc, (1 << (high_lane + 1)) - 1);
                uint32 lane = source & 3;
                uint32 low = r_masked_u32(source & 0xfffffffc, (15 << lane) & 15);
                uint32 keep = lane == 0 ? 0 : 0xffffffff << ((4 - lane) * 8);
                uint32 value = ((high << ((3 - high_lane) * 8)) & keep) | (low >> (lane * 8));
                
                high_address = destination + 3;
                high_lane = high_address & 3;
                w_masked_u32(high_address & 0xfffffffc, value >> ((3 - high_lane) * 8), (1 << (high_lane + 1)) - 1);
                lane = destination & 3;
                w_masked_u32(destination & 0xfffffffc, value << (lane * 8), (15 << lane) & 15);
                
            }
        }
    }
    uint32 mode = r_u32(state + 0x4c);
    if (mode == 0x10 || mode == 0x12 || mode == 0x13)
        return;
    uint32 selector = 0xffffffff;
    if (r_s16(core + 0x1e) != 0)
        selector = (uint32)r_s16(core + 0x14);
    G3HankUpdateAnimation2A1C(state, selector, frame + 0x38, frame);
    uint32 coordinate = r_u16(player_position);
    uint32 own = r_u16(state - 0xac);
    w_u16(frame + 0x10, coordinate - own);
    coordinate = r_u16(player_position + 2);
    own = r_u16(state - 0xaa);
    w_u16(frame + 0x12, coordinate - own);
    coordinate = r_u16(player_position + 4);
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
    if (next == 0)
    {
        settings = r_u32(state + 4);
        if (settings != 0)
        {
            sint32 elapsed = r_s16(core + 0x1c);
            sint32 interval = r_s16(settings + 8);
            if (elapsed >= interval)
            {
                uint32 random = G3SdkRandom80027FA8();
                sint32 probability = r_s16(settings + 0xa);
                if ((sint32)(random & 0xfff) < probability)
                    next = 2;
                else
                    w_u16(core + 0x1c, 0);
            }
        }
    }
    if (next != 0)
    {
        w_u16(state + 0x34, next);
        G3HankFinishAnimation2CFC(state);
    }
    if (r_s16(state + 0x34) < 0)
    {
        if (r_s16(core + 0x10) < 0)
            w_u16(core + 0x1e, moving);
        else
        {
            sint32 active = r_s16(core + 0x1e);
            if ((moving != 0 && active == 0) || (moving == 0 && active != 0))
            {
                sint32 elapsed = r_s16(core + 0x18);
                sint32 interval = r_s16(core + 0x1a);
                if (elapsed >= interval)
                {
                    if (r_u32(frame + 0x38) == 0)
                        G3HankFinishAnimation2CFC(state);
                    else
                    {
                        uint32 animation = (uint32)r_s16(core + (moving != 0 ? 0xe : 0x10));
                        w_u16(core + 0x18, 0);
                        w_u16(core + 0x1e, moving != 0 ? 1 : 0);
                        G3HankSelectAnimation27EC(state, animation, 1, frame);
                    }
                }
            }
        }
        if (r_s16(state + 0x34) < 0)
            return;
    }
    if (r_u32(frame + 0x38) != 0)
    {
        sint32 pending = r_s16(state + 0x34);
        switch (pending)
        {
        case 6: G3HankEnterPursuit33BC(state, frame); break;
        case 2: G3HankEnterIdle34B0(state, frame); break;
        case 0xb: G3HankEnterMove3614(state, frame); break;
        case 0xe: G3HankEnterRun36F4(state, frame); break;
        }
        w_u16(state + 0x34, 0xffff);
    }
}
