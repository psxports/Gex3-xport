#include "xport.h"
#include "psx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct G3LevelCodeBinding
{
    uint32 identity_id;
    uint32 module_base;
    uint32 code_offset;
    uint32 code_bytes;
    uint32 record_offset;
    uint32 allocation_depth;
};

struct G3LevelResourceRecord
{
    uint32 code_offset;
    uint32 code_bytes;
    uint32 relocation_bytes;
    uint32 identity_id;
};

struct G3LevelResource
{
    uint32 archive_entry;
    uint32 first_record;
    uint32 record_count;
};

static const G3LevelResourceRecord g3_level_resource_records[290] =
{
    {216, 1401, 80, 72},
    {1620, 1041, 76, 79},
    {2664, 3948, 104, 45},
    {6612, 1560, 80, 28},
    {8172, 35850, 4080, 39},
    {44024, 1349, 128, 16},
    {45376, 32452, 2692, 65},
    {77828, 3636, 140, 13},
    {456, 2841, 464, 67},
    {3300, 3948, 104, 45},
    {7248, 1824, 104, 69},
    {9072, 1280, 40, 54},
    {10352, 1768, 80, 44},
    {12120, 1401, 80, 72},
    {13524, 2884, 164, 1},
    {16408, 2140, 152, 76},
    {18548, 35481, 4372, 33},
    {54032, 4420, 224, 78},
    {58452, 1736, 80, 52},
    {60188, 3049, 84, 2},
    {63240, 320, 28, 20},
    {63560, 3478, 172, 30},
    {67040, 4556, 548, 49},
    {71596, 2352, 280, 18},
    {73948, 1480, 92, 57},
    {75428, 708, 68, 40},
    {360, 3948, 104, 45},
    {4308, 1768, 80, 44},
    {6076, 1041, 76, 79},
    {7120, 1824, 104, 69},
    {8944, 941, 76, 36},
    {9888, 1736, 80, 52},
    {11624, 1401, 80, 72},
    {13028, 1156, 40, 64},
    {14184, 3049, 84, 2},
    {17236, 6225, 232, 27},
    {23464, 1349, 128, 16},
    {24816, 35481, 4372, 33},
    {60300, 4556, 548, 49},
    {64856, 1480, 92, 57},
    {144, 1401, 80, 72},
    {1548, 3948, 104, 45},
    {5496, 1480, 92, 57},
    {6976, 2073, 400, 21},
    {9052, 5284, 212, 3},
    {72, 1300, 84, 66},
    {1372, 1856, 168, 42},
    {288, 3948, 104, 45},
    {4236, 1560, 80, 28},
    {5796, 35481, 4372, 33},
    {41280, 1140, 44, 23},
    {42420, 1401, 80, 72},
    {43824, 1444, 68, 53},
    {45268, 2073, 400, 21},
    {47344, 2884, 164, 1},
    {50228, 2968, 164, 8},
    {53196, 1480, 92, 57},
    {54676, 2888, 172, 56},
    {192, 1401, 80, 72},
    {1596, 2805, 180, 0},
    {4404, 1768, 80, 44},
    {6172, 1280, 40, 54},
    {7452, 1480, 92, 57},
    {8932, 1849, 108, 63},
    {10784, 336, 32, 35},
    {360, 1768, 80, 44},
    {2128, 1824, 104, 69},
    {3952, 2604, 128, 46},
    {6556, 3948, 104, 45},
    {10504, 1736, 80, 52},
    {12240, 4541, 908, 58},
    {16784, 35481, 4372, 33},
    {52268, 1401, 80, 72},
    {53672, 2073, 400, 21},
    {55748, 4948, 256, 77},
    {60696, 2352, 280, 18},
    {63048, 7648, 424, 38},
    {70696, 1480, 92, 57},
    {72176, 5568, 572, 47},
    {168, 1401, 80, 72},
    {1572, 35481, 4372, 33},
    {37056, 2073, 400, 21},
    {39132, 3478, 172, 30},
    {42612, 2572, 148, 24},
    {45184, 7036, 388, 17},
    {480, 3948, 104, 45},
    {4428, 2805, 180, 0},
    {7236, 1768, 80, 44},
    {9004, 1736, 80, 52},
    {10740, 5568, 572, 47},
    {16308, 1401, 80, 72},
    {17712, 2236, 84, 37},
    {19948, 2073, 400, 21},
    {22024, 4948, 256, 77},
    {26972, 7648, 424, 38},
    {34620, 1048, 60, 31},
    {35668, 35481, 4372, 33},
    {71152, 5265, 412, 11},
    {76420, 3478, 172, 30},
    {79900, 1909, 108, 74},
    {81812, 2572, 148, 24},
    {84384, 32452, 2692, 65},
    {116836, 348, 24, 34},
    {117184, 1480, 92, 57},
    {144, 1401, 80, 72},
    {1548, 1849, 108, 63},
    {3400, 1736, 80, 52},
    {5136, 2420, 100, 22},
    {7556, 2884, 164, 1},
    {168, 1401, 80, 72},
    {1572, 3948, 104, 45},
    {5520, 1041, 76, 79},
    {6564, 2073, 400, 21},
    {8640, 6225, 232, 27},
    {14868, 35481, 4372, 33},
    {408, 1608, 88, 43},
    {2016, 3948, 104, 45},
    {5964, 1824, 104, 69},
    {7788, 35481, 4372, 33},
    {43272, 2733, 144, 70},
    {46008, 1401, 80, 72},
    {47412, 1308, 156, 71},
    {48720, 2884, 164, 1},
    {51604, 1980, 156, 15},
    {53584, 3478, 172, 30},
    {57064, 3781, 248, 51},
    {60848, 5265, 412, 11},
    {66116, 2572, 148, 24},
    {68688, 7036, 388, 17},
    {75724, 348, 24, 34},
    {76072, 1480, 92, 57},
    {288, 3948, 104, 45},
    {4236, 1140, 44, 23},
    {5376, 7648, 424, 38},
    {13024, 3478, 172, 30},
    {16504, 1401, 80, 72},
    {17908, 2073, 400, 21},
    {19984, 32452, 2692, 65},
    {52436, 1736, 80, 52},
    {54172, 2420, 100, 22},
    {56592, 1480, 92, 57},
    {58072, 320, 28, 20},
    {336, 3948, 104, 45},
    {4284, 1560, 80, 28},
    {5844, 1480, 92, 57},
    {7324, 2281, 228, 75},
    {9608, 4948, 256, 77},
    {14556, 3601, 240, 6},
    {18160, 1308, 156, 71},
    {19468, 1733, 152, 32},
    {21204, 1401, 80, 72},
    {22608, 3478, 172, 30},
    {26088, 35481, 4372, 33},
    {61572, 2884, 164, 1},
    {64456, 3580, 328, 81},
    {216, 1401, 80, 72},
    {1620, 1480, 92, 57},
    {3100, 1849, 108, 63},
    {4952, 336, 32, 35},
    {5288, 1736, 80, 52},
    {7024, 5145, 372, 55},
    {12172, 4948, 256, 77},
    {17120, 3478, 172, 30},
    {144, 1401, 80, 72},
    {1548, 3948, 104, 45},
    {5496, 7712, 280, 5},
    {13208, 3478, 172, 30},
    {16688, 2073, 400, 21},
    {360, 1401, 80, 72},
    {1764, 1280, 40, 54},
    {3044, 1768, 80, 44},
    {4812, 1560, 80, 28},
    {6372, 1824, 104, 69},
    {8196, 3478, 172, 30},
    {11676, 3212, 164, 14},
    {14888, 35481, 4372, 33},
    {50372, 3672, 164, 7},
    {54044, 3049, 84, 2},
    {57096, 1444, 80, 60},
    {58540, 5265, 412, 11},
    {63808, 2968, 164, 8},
    {66776, 2352, 280, 18},
    {432, 2281, 228, 75},
    {2716, 3948, 104, 45},
    {6664, 1824, 104, 69},
    {8488, 2720, 180, 59},
    {11208, 1608, 88, 43},
    {12816, 7712, 280, 5},
    {20528, 7648, 424, 38},
    {28176, 3478, 172, 30},
    {31656, 1401, 80, 72},
    {33060, 3916, 192, 10},
    {36976, 2420, 100, 22},
    {39396, 1093, 72, 12},
    {40492, 1444, 80, 60},
    {41936, 35481, 4372, 33},
    {77420, 2968, 164, 8},
    {80388, 3661, 212, 61},
    {84052, 1480, 92, 57},
    {72, 1401, 80, 72},
    {1476, 32452, 2692, 65},
    {168, 1401, 80, 72},
    {1572, 1480, 92, 57},
    {3052, 3948, 104, 45},
    {7000, 2073, 400, 21},
    {9076, 2884, 164, 1},
    {11960, 7341, 372, 73},
    {240, 1401, 80, 72},
    {1644, 3948, 104, 45},
    {5592, 1480, 92, 57},
    {7072, 1041, 76, 79},
    {8116, 1628, 368, 62},
    {9744, 336, 32, 35},
    {10080, 2884, 164, 1},
    {12964, 5265, 412, 11},
    {18232, 1849, 108, 63},
    {408, 2281, 228, 75},
    {2692, 7084, 460, 29},
    {9776, 1804, 132, 50},
    {11580, 1280, 40, 54},
    {12860, 1768, 80, 44},
    {14628, 3948, 104, 45},
    {18576, 6440, 300, 25},
    {25016, 5145, 372, 55},
    {30164, 1401, 80, 72},
    {31568, 1093, 72, 12},
    {32664, 2420, 100, 22},
    {35084, 1156, 40, 64},
    {36240, 436, 44, 19},
    {36676, 7648, 424, 38},
    {44324, 1480, 92, 57},
    {45804, 1972, 100, 4},
    {336, 3948, 104, 45},
    {4284, 1041, 76, 79},
    {5328, 1401, 80, 72},
    {6732, 1528, 296, 26},
    {8260, 7648, 424, 38},
    {15908, 6320, 380, 41},
    {22228, 2352, 280, 18},
    {24580, 5265, 412, 11},
    {29848, 3636, 140, 13},
    {33484, 1484, 80, 9},
    {34968, 908, 60, 48},
    {35876, 1480, 92, 57},
    {37356, 708, 68, 40},
    {288, 1788, 260, 68},
    {2076, 3948, 104, 45},
    {6024, 1401, 80, 72},
    {7428, 2884, 164, 1},
    {10312, 5145, 372, 55},
    {15460, 7648, 424, 38},
    {23108, 1444, 80, 60},
    {24552, 35481, 4372, 33},
    {60036, 1484, 80, 9},
    {61520, 348, 24, 34},
    {61868, 1048, 60, 31},
    {192, 3948, 104, 45},
    {4140, 1401, 80, 72},
    {5544, 35481, 4372, 33},
    {41028, 5145, 372, 55},
    {46176, 2572, 148, 24},
    {48748, 1480, 92, 57},
    {50228, 1788, 260, 68},
    {168, 1788, 260, 68},
    {1956, 3948, 104, 45},
    {5904, 3478, 172, 30},
    {9384, 1401, 80, 72},
    {10788, 1736, 80, 52},
    {12524, 35481, 4372, 33},
    {288, 1788, 260, 68},
    {2076, 3948, 104, 45},
    {6024, 1401, 80, 72},
    {7428, 2073, 400, 21},
    {9504, 3049, 84, 2},
    {12556, 1736, 80, 52},
    {14292, 320, 28, 20},
    {14612, 3478, 172, 30},
    {18092, 2352, 280, 18},
    {20444, 7648, 424, 38},
    {28092, 35481, 4372, 33},
    {264, 1401, 80, 72},
    {1668, 1251, 84, 80},
    {2920, 2281, 228, 75},
    {5204, 2841, 464, 67},
    {8048, 1768, 80, 44},
    {9816, 3948, 104, 45},
    {13764, 1308, 156, 71},
    {15072, 32452, 2692, 65},
    {47524, 3601, 240, 6},
    {51128, 1480, 92, 57}
};

static const G3LevelResource g3_level_resources[29] =
{
    {14, 0, 8},
    {20, 8, 18},
    {25, 26, 14},
    {29, 40, 5},
    {32, 45, 2},
    {41, 47, 11},
    {42, 58, 7},
    {51, 65, 14},
    {52, 79, 6},
    {59, 85, 19},
    {65, 104, 5},
    {73, 109, 6},
    {79, 115, 16},
    {84, 131, 11},
    {89, 142, 13},
    {90, 155, 8},
    {99, 163, 5},
    {102, 168, 14},
    {103, 182, 17},
    {105, 199, 2},
    {119, 201, 6},
    {124, 207, 9},
    {126, 216, 16},
    {135, 232, 13},
    {140, 245, 11},
    {141, 256, 7},
    {142, 263, 6},
    {143, 269, 11},
    {153, 280, 10}
};

/* Native binding storage uses the existing XCRT heap */
class G3LevelCodeBindings
{
    G3LevelCodeBinding *entries;
    size_t count;
    size_t capacity;
    G3LevelCodeBindings(const G3LevelCodeBindings &);
    G3LevelCodeBindings &operator=(const G3LevelCodeBindings &);

public:
    G3LevelCodeBindings() : entries(0), count(0), capacity(0) {}
    ~G3LevelCodeBindings() { free(entries); }
    size_t Size() const { return count; }
    bool Empty() const { return count == 0; }
    void Clear() { count = 0; }
    G3LevelCodeBinding *Begin() { return entries; }
    G3LevelCodeBinding *End() { return entries == 0 ? 0 : entries + count; }
    G3LevelCodeBinding &operator[](size_t index) { return entries[index]; }
    void Append(const G3LevelCodeBinding *source, size_t added)
    {
        if (added == 0) return;
        const size_t maximum = (size_t)-1 / sizeof(G3LevelCodeBinding);
        if (added > maximum - count) abort();
        size_t needed = count + added;
        if (needed > capacity)
        {
            size_t grown = capacity <= maximum / 2 ? capacity * 2 : maximum;
            if (grown < needed) grown = needed;
            void *allocation = realloc(entries, grown * sizeof(G3LevelCodeBinding));
            if (allocation == 0) abort();
            entries = (G3LevelCodeBinding *)allocation;
            capacity = grown;
        }
        memcpy(entries + count, source, added * sizeof(G3LevelCodeBinding));
        count = needed;
    }
    G3LevelCodeBinding *Erase(G3LevelCodeBinding *entry)
    {
        size_t index = entry - entries;
        memmove(entry, entry + 1, (count - index - 1) * sizeof(G3LevelCodeBinding));
        --count;
        return entries + index;
    }
};

static G3LevelCodeBindings g3_level_code_bindings;

static void G3LevelCodeFailure(const char *reason, uint32 address)
{
    {
#ifndef NDEBUG
    fprintf(stderr, "G3 level code binding failed: %s at %08X\n", reason, (unsigned int)address);
#endif
    }
    abort();
}

static uint32 G3LevelCodeRamOffset(uint32 address, uint32 bytes)
{
    uint32 physical = address & 0x1fffffff;
    uint32 segment = address & 0xe0000000;
    if (address >= PSX_DRAM_SIZE && !((segment == 0x80000000 || segment == 0xa0000000) && physical < 0x800000))
        G3LevelCodeFailure("non-RAM module range", address);
    uint32 offset = physical & (PSX_DRAM_SIZE - 1);
    if (bytes > PSX_DRAM_SIZE - offset)
        G3LevelCodeFailure("module range crosses RAM boundary", address);
    return offset;
}

static bool G3LevelCodeOverlaps(uint32 first, uint32 first_bytes, uint32 second, uint32 second_bytes)
{
    return first_bytes != 0 && second_bytes != 0 && first < second + second_bytes && second < first + first_bytes;
}

void G3LevelCodeRegistryReset(void)
{
    g3_level_code_bindings.Clear();
}

void G3LevelCodeInvalidateSpan(uint32 address, uint32 bytes)
{
    if (bytes == 0 || g3_level_code_bindings.Empty()) return;
    uint32 offset = G3LevelCodeRamOffset(address, bytes);
    G3LevelCodeBinding *entry = g3_level_code_bindings.Begin();
    while (entry != g3_level_code_bindings.End())
    {
        if (G3LevelCodeOverlaps(offset, bytes, entry->code_offset, entry->code_bytes) ||
            G3LevelCodeOverlaps(offset, bytes, entry->record_offset, 24))
            entry = g3_level_code_bindings.Erase(entry);
        else
            ++entry;
    }
}

void G3LevelCodePop(uint32 previous_depth)
{
    if (previous_depth == 0) return;
    G3LevelCodeBinding *entry = g3_level_code_bindings.Begin();
    while (entry != g3_level_code_bindings.End())
    {
        if (entry->allocation_depth >= previous_depth)
            entry = g3_level_code_bindings.Erase(entry);
        else
            ++entry;
    }
}

void G3LevelCodeRegister(uint32 archive_entry, uint32 module, uint32 allocation_end, uint32 allocation_depth)
{
    uint32 root = G3LevelCodeRamOffset(module, 24);
    uint32 end = G3LevelCodeRamOffset(allocation_end, 0);
    if (end < root + 24) G3LevelCodeFailure("invalid module allocation", module);
    uint32 sentinel = root;
    while (r_u32(module + sentinel - root + 12) != 0)
    {
        if (end - sentinel < 48) G3LevelCodeFailure("missing module sentinel", module + sentinel - root);
        sentinel += 24;
    }
    if (sentinel == root) return;
    const G3LevelResource *resource = 0;
    for (uint32 i = 0; i < 29; ++i)
    {
        if (g3_level_resources[i].archive_entry == archive_entry)
        {
            resource = &g3_level_resources[i];
            break;
        }
    }
    if (resource == 0 || (sentinel - root) / 24 != resource->record_count)
        G3LevelCodeFailure("unknown level resource or record count", module);
    uint32 relocation = r_u32(module + sentinel - root + 20);
    uint32 relocation_offset = G3LevelCodeRamOffset(relocation, 4);
    if (relocation_offset < sentinel + 24 || relocation_offset > end - 4)
        G3LevelCodeFailure("invalid relocation region", relocation);
    G3LevelCodeBindings pending;
    for (uint32 record = root; record < sentinel; record += 24)
    {
        uint32 descriptor = module + record - root;
        uint32 code = r_u32(descriptor + 12);
        uint32 code_bytes = r_u32(descriptor + 20);
        uint32 code_offset = G3LevelCodeRamOffset(code, code_bytes);
        if (code_offset < sentinel + 24 || code_offset > relocation_offset || code_bytes > relocation_offset - code_offset)
            G3LevelCodeFailure("invalid code extent", code);
        uint32 relocation_start = G3LevelCodeRamOffset(relocation, 4);
        if (relocation_start > end - 4) G3LevelCodeFailure("invalid relocation cursor", relocation);
        uint32 relocation_bytes = r_u32(relocation);
        if (relocation_bytes > end - relocation_start - 4)
            G3LevelCodeFailure("invalid relocation extent", relocation);
        uint32 record_index = (record - root) / 24;
        const G3LevelResourceRecord *source = &g3_level_resource_records[resource->first_record + record_index];
        if (code_offset - root != source->code_offset || code_bytes != source->code_bytes || relocation_bytes != source->relocation_bytes)
            G3LevelCodeFailure("level resource record layout differs", descriptor);
        G3LevelCodeBinding binding;
        binding.identity_id = source->identity_id;
        binding.module_base = code;
        binding.code_offset = code_offset;
        binding.code_bytes = code_bytes;
        binding.record_offset = record;
        binding.allocation_depth = allocation_depth;
        for (uint32 i = 0; i < pending.Size(); ++i)
        {
            if (G3LevelCodeOverlaps(binding.code_offset, binding.code_bytes, pending[i].code_offset, pending[i].code_bytes))
                G3LevelCodeFailure("overlapping module code records", code);
        }
        pending.Append(&binding, 1);
        relocation += relocation_bytes + 4;
    }
    uint32 final_offset = G3LevelCodeRamOffset(relocation, 4);
    if (final_offset > end - 4 || r_u32(relocation) != 0)
        G3LevelCodeFailure("missing final relocation stream", relocation);
    for (uint32 i = 0; i < pending.Size(); ++i)
    {
        G3LevelCodeInvalidateSpan(pending[i].module_base, pending[i].code_bytes);
        G3LevelCodeInvalidateSpan(module + pending[i].record_offset - root, 24);
    }
    g3_level_code_bindings.Append(pending.Begin(), pending.Size());
}

bool G3LevelCodeResolve(uint32 target, uint32 *identity_id, uint32 *module_base, uint32 *block_offset)
{
    uint32 physical = target & 0x1fffffff;
    uint32 segment = target & 0xe0000000;
    if (target >= PSX_DRAM_SIZE && !((segment == 0x80000000 || segment == 0xa0000000) && physical < 0x800000))
        return false;
    uint32 offset = physical & (PSX_DRAM_SIZE - 1);
    for (uint32 i = 0; i < g3_level_code_bindings.Size(); ++i)
    {
        const G3LevelCodeBinding *binding = &g3_level_code_bindings[i];
        if (offset >= binding->code_offset && offset - binding->code_offset < binding->code_bytes)
        {
            *identity_id = binding->identity_id;
            *module_base = binding->module_base;
            *block_offset = offset - binding->code_offset;
            return true;
        }
    }
    return false;
}
