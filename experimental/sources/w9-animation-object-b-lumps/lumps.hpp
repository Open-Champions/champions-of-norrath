#pragma once
// Observed lumps at 0x6cb280 in .bss. The next symbol, discardable, begins at
// 0x6cc680, so the extent is 5120 bytes. lumpAdd__FPvPCc indexes it with a stride of
// 40 bytes (index * 5 * 8), which makes 128 entries. Entry layout, from the stores
// lumpAdd makes: +0 a name copied with strcpy (16 bytes before the next field),
// +16 the lump address, and words at +20, +24, +28, +32 and +36 that lumpAdd zeroes.
// nmLumps (0x483f8c) is the live entry count and is defined elsewhere.
// Inferred: the capacity is 128 by arithmetic, not from a bounds check.
// Unknown: the meaning of the five zeroed words and the name capacity.
struct LumpEntry {
    char name[16];
    void* lump;
    int word20;
    int word24;
    int word28;
    int word32;
    int word36;
};
#if defined(__mips__)
static_assert(sizeof(LumpEntry) == 40 && __builtin_offsetof(LumpEntry, lump) == 16 && __builtin_offsetof(LumpEntry, word36) == 36, "entry layout");
#endif
extern "C" LumpEntry lumps[128];
