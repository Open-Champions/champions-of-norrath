// Observed lumpLoadGlob__FPc at 0x00165f50, 792 bytes. Loads every lump of one packed file.
// The argument is the file name. In order:
//  1. cdLoad(name, globDir, 2048, 0) reads the first 2048 bytes into globDir (0x006d8780,
//     2080 bytes before the next symbol), then FlushCache(0) is called. The directory is a run
//     of 40-byte records: a name at +0 and the lump's byte size in the word at +36. The run
//     ends at the first record whose first (signed) byte is zero; an empty first record loads nothing.
//  2. For each record: padded = (size + 2047) & -2048. memAlloc(padded + 256, &lumps[nmLumps])
//     (the label is the table slot's name field, not yet written); a null result stores zero
//     through address 0 (the same idiom as lumpAdd). cdLoad(name, block, padded, offset) with the
//     file offset starting at 2048 and growing by padded; then soundGetFreeRam() is saved.
//  3. noNewVags is set to 1 when the record is vamplord.lmp and engineCurrentLevelName is not
//     gothic2, or superorc.lmp and it is not ruinint1. initLump(block) runs, noNewVags is set
//     back to 0, and memRealloc(block, size - initLump result + 256) shrinks the block.
//  4. With e = lumps[nmLumps]: word24 = that new size, word28 = saved RAM - soundGetFreeRam(),
//     word32 = 0, word20 = 0. The name field becomes "faydark1.lmp" when the record is
//     gaydark1.lmp, "faydark2.lmp" for gaydark2.lmp (13 bytes copied from the literal), else a
//     strcpy of the record name. nmLumps is read again for each use.
//  5. word36 = 1, the lump pointer (+16) is the block, and nmLumps is incremented and stored.
// There is no bound on the directory length or on nmLumps.
// Inferred: the directory sits in the first 2048 bytes and every lump after it is sector-aligned,
// from the offsets used; gaydark/faydark are renamed on load, from the literals only.
// Unknown: why those two names are renamed; what the lump table words 20, 24, 28, 32 and 36 are for
// (this routine fills them; lumpAdd zeroes them); the byte at the record's tail beyond +36.
#include "../w9-animation-object-b-lumps/lumps.hpp"

extern "C" {
extern char globDir[];
extern int nmLumps;
extern int noNewVags;
extern char engineCurrentLevelName[];
void cdLoad__FPcPvii(char* name, void* destination, int requested, int byte_offset);
void FlushCache(int mode);
void* memAlloc__FiPc(int bytes, char* label);
int initLump__FPv(void* lump);
void* memRealloc__FPvi(void* lump, int size);
int soundGetFreeRam__Fv();
int strcmp(const char* left, const char* right);
char* strcpy(char* destination, const char* source);
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_host_store_absolute(unsigned address, unsigned value);
#endif

namespace {
int record_size(const char* record) {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(record) + 36;
    return static_cast<int>(static_cast<unsigned>(p[0]) | (static_cast<unsigned>(p[1]) << 8) | (static_cast<unsigned>(p[2]) << 16) |
                            (static_cast<unsigned>(p[3]) << 24));
}
void store_zero_at_null() {
#if defined(NORRATH_HOST_CONTRACT)
    norrath_host_store_absolute(0u, 0u);
#else
    *static_cast<volatile int*>(nullptr) = 0;
#endif
}
void copy_literal_13(char* destination, const char* literal) {
    for (int i = 0; i < 13; ++i) destination[i] = literal[i];
}
}  // namespace

extern "C" void lumpLoadGlob__FPc(char* name) {
    cdLoad__FPcPvii(name, globDir, 2048, 0);
    FlushCache(0);
    int index = 0;
    int offset = 2048;
    if (globDir[0] == 0) return;
    for (;;) {
        const char* record = globDir + 40 * index;
        const int size = record_size(record);
        const int padded = (size + 2047) & -2048;
        void* block = memAlloc__FiPc(padded + 256, lumps[nmLumps].name);
        if (block == nullptr) store_zero_at_null();
        cdLoad__FPcPvii(name, block, padded, offset);
        offset += padded;
        const int ram_before = soundGetFreeRam__Fv();
        if (strcmp(record, "vamplord.lmp") == 0 && strcmp(engineCurrentLevelName, "gothic2") != 0) noNewVags = 1;
        if (strcmp(record, "superorc.lmp") == 0 && strcmp(engineCurrentLevelName, "ruinint1") != 0) noNewVags = 1;
        const int consumed = initLump__FPv(block);
        noNewVags = 0;
        const int new_size = size - consumed + 256;
        memRealloc__FPvi(block, new_size);
        LumpEntry* entry = &lumps[nmLumps];
        entry->word24 = new_size;
        entry->word28 = ram_before - soundGetFreeRam__Fv();
        entry->word32 = 0;
        entry->word20 = 0;
        if (strcmp(record, "gaydark1.lmp") == 0) {
            copy_literal_13(lumps[nmLumps].name, "faydark1.lmp");
        } else if (strcmp(record, "gaydark2.lmp") == 0) {
            copy_literal_13(lumps[nmLumps].name, "faydark2.lmp");
        } else {
            strcpy(lumps[nmLumps].name, record);
        }
        ++index;
        entry = &lumps[nmLumps];
        entry->word36 = 1;
        entry->lump = block;
        nmLumps = nmLumps + 1;
        if (globDir[40 * index] == 0) break;
    }
}
