// Observed playerInitArmorCache__Fv at 0x00195358, 516 bytes, 32-byte frame. Nothing is returned.
//  1. small = nmPlayers >= 2 or the signed byte coreForceSmallCharacters nonzero. The equipment cache
//     file name is "eqcache2.hdr" when small, else "eqcache.hdr".
//  2. cdDirectoryLookup__FPcPiT1i(name, 0, &size, 0) gives the file size; size is rounded up to a
//     multiple of 2048, memAlloc__FiPc(size, "eqCache header") is called, and the result is stored in
//     eqLumpHeader BEFORE cdLoad__FPcPvii(name, memory, size, 0) reads the file into it.
//  3. eqGroup = eqLumpHeader + 4 and nmEqGroups = the word at the start of the loaded file. When the
//     count is positive, each 12-byte group record i = 0.. (nmEqGroups, re-read each time) has its
//     words 0 and 1 increased by the address in eqLumpHeader (also re-read), turning offsets into
//     addresses.
//  4. The same steps for "wcache.hdr" with the label "wCache header", into wLumpHeader, wGroup and
//     nmWGroups.
//  5. For k = 0..19 the byte at the cache array (image symbol cache at 0x007cb770, 3,520 bytes) offset
//     1844 + 88 * k is set to 1 (the original does it as 4 passes of 5 stores).
// Inferred: reading the header lumps that describe the equipment and weapon armour groups, and
// marking the second half of the 40-record, 88-byte armour cache, offset 84 of each record, in use or
// valid. Unknown: the group record's third word, and what the byte at record offset 84 means.
extern "C" {
extern int nmPlayers;
extern signed char coreForceSmallCharacters;
extern void* eqLumpHeader;
extern void* wLumpHeader;
extern unsigned eqGroup;
extern unsigned wGroup;
extern int nmEqGroups;
extern int nmWGroups;
extern unsigned char armorCache[] __asm__("norrath_player_armor_cache_view_7cb770");
int cdDirectoryLookup__FPcPiT1i(char* name, int* sector, int* size, int mode);
void* memAlloc__FiPc(int bytes, char* label);
void cdLoad__FPcPvii(char* name, void* destination, int size, int offset);
}

namespace {
inline unsigned word(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

// Loads one header file and returns its memory; the caller's lump global is stored before the read.
inline void* loadHeader(char* name, char* label, void*& lump) {
    int size;
    cdDirectoryLookup__FPcPiT1i(name, nullptr, &size, 0);
    size = (size + 2047) & -2048;
    void* memory = memAlloc__FiPc(size, label);
    lump = memory;
    cdLoad__FPcPvii(name, memory, size, 0);
    return memory;
}
}  // namespace

extern "C" void playerInitArmorCache__Fv() {
    bool small = false;
    if (nmPlayers >= 2) small = true;
    else if (coreForceSmallCharacters != 0) small = true;
    loadHeader(const_cast<char*>(small ? "eqcache2.hdr" : "eqcache.hdr"), const_cast<char*>("eqCache header"), eqLumpHeader);
    eqGroup = word(eqLumpHeader) + 4;
    nmEqGroups = *static_cast<int*>(eqLumpHeader);
    if (nmEqGroups > 0) {
        int index = 0;
        do {
            unsigned* group = reinterpret_cast<unsigned*>(static_cast<__UINTPTR_TYPE__>(eqGroup) + 12 * index);
            ++index;
            const unsigned base = word(eqLumpHeader);
            group[1] += base;
            group[0] += base;
        } while (index < nmEqGroups);
    }
    loadHeader(const_cast<char*>("wcache.hdr"), const_cast<char*>("wCache header"), wLumpHeader);
    wGroup = word(wLumpHeader) + 4;
    nmWGroups = *static_cast<int*>(wLumpHeader);
    if (nmWGroups > 0) {
        int index = 0;
        do {
            unsigned* group = reinterpret_cast<unsigned*>(static_cast<__UINTPTR_TYPE__>(wGroup) + 12 * index);
            ++index;
            const unsigned base = word(wLumpHeader);
            group[1] += base;
            group[0] += base;
        } while (index < nmWGroups);
    }
    for (int k = 0; k < 20; ++k) armorCache[1844 + 88 * k] = 1;
}
