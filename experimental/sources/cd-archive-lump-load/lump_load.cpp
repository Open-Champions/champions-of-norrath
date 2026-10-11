// Observed lumpLoad__FPc at 0x00165b30, 652 bytes. The 128 records of lumps
// at 0x006cb280 are norrath_constructor_lump_cache_view. nmLumps at
// 0x00483f8c is norrath_constructor_lump_count_view. A record whose flags at
// offset 20 have bit 0 clear contributes the pointer at offset 16. A nonzero
// pointer returns immediately. Otherwise cdDirectoryLookup writes the sector
// at frame+0 and the size at frame+4. vamplord.lmp sets noNewVags when
// engineCurrentLevelName is not gothic2. superorc.lmp sets it when that name
// is not ruinint1. The allocation is (size + 2047) masked to 2048 plus 256,
// labeled with the next record. cdLoad receives a zero length and offset.
// lumpLoadLump, lumpLoadSize, lumpLoadName, and lumpLoadCompletionFlag are
// then stored and cdProcess runs. A miss prints the text at 0x00463458 and
// stores 0 at address 0. Both paths scan the cache again and return that pointer.
extern "C" {
extern unsigned char norrath_constructor_lump_cache_view[];
extern unsigned char norrath_constructor_lump_count_view[4];
extern unsigned noNewVags;
extern unsigned char lumpLoadLump[4];
extern unsigned char lumpLoadSize[4];
extern unsigned char lumpLoadCompletionFlag;
extern unsigned char lumpLoadName[];
extern char engineCurrentLevelName[];
int strcmp(const char* left, const char* right);
char* strcpy(char* destination, const char* source);
int cdDirectoryLookup__FPcPiT1i(char* name, int* sector, int* bytes, int mode);
void* memAlloc__FiPc(int bytes, char* label);
void cdLoad__FPcPvii(char* name, void* destination, int requested, int byte_offset);
void cdProcess__Fv();
int scePrintf(const char* format, ...);
#if defined(NORRATH_HOST_CONTRACT)
void norrath_host_store_absolute(unsigned address, unsigned value);
#endif
static unsigned load_word(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static void store_absolute(unsigned address, unsigned value) {
#if defined(NORRATH_HOST_CONTRACT)
    norrath_host_store_absolute(address, value);
#else
    *reinterpret_cast<volatile unsigned*>(static_cast<__UINTPTR_TYPE__>(address)) = value;
#endif
}
static void* resident_lump(char* name) {
    unsigned index = 0u;
    unsigned offset = 0u;
    unsigned char* cache = norrath_constructor_lump_cache_view;
    for (;;) {
        const unsigned count = load_word(norrath_constructor_lump_count_view);
        if (!(static_cast<int>(index) < static_cast<int>(count))) return nullptr;
        char* record_name = reinterpret_cast<char*>(cache + offset);
        offset += 40u;
        if (strcmp(record_name, name) != 0) {
            index += 1u;
            continue;
        }
        const unsigned flags = load_word(cache + index * 40u + 20u);
        if (((flags ^ 1u) & 1u) == 0u) {
            index += 1u;
            continue;
        }
        if (!(static_cast<int>(index) < static_cast<int>(load_word(norrath_constructor_lump_count_view))))
            return nullptr;
        const unsigned pointer = load_word(cache + index * 40u + 16u);
        if (pointer == 0u) return nullptr;
        return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(pointer));
    }
}
void* lumpLoad__FPc(char* name) {
    void* resident = resident_lump(name);
    if (resident != nullptr) return resident;
    int sector_word = 0;
    int size_word = 0;
    if (cdDirectoryLookup__FPcPiT1i(name, &sector_word, &size_word, 0) != 0) {
        if (strcmp(name, "vamplord.lmp") == 0 && strcmp(engineCurrentLevelName, "gothic2") != 0)
            noNewVags = 1u;
        if (strcmp(name, "superorc.lmp") == 0 && strcmp(engineCurrentLevelName, "ruinint1") != 0)
            noNewVags = 1u;
        const unsigned count = load_word(norrath_constructor_lump_count_view);
        const unsigned masked = (static_cast<unsigned>(size_word) + 2047u) & 0xfffff800u;
        char* const label = reinterpret_cast<char*>(norrath_constructor_lump_cache_view + count * 40u);
        void* const buffer = memAlloc__FiPc(static_cast<int>(masked + 256u), label);
        cdLoad__FPcPvii(name, buffer, 0, 0);
        store_word(lumpLoadLump, static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(buffer)));
        store_word(lumpLoadSize, static_cast<unsigned>(size_word));
        lumpLoadCompletionFlag = 1;
        strcpy(reinterpret_cast<char*>(lumpLoadName), name);
        cdProcess__Fv();
        noNewVags = 0u;
    } else {
        scePrintf("!!!!!!lump not found\n");
        store_absolute(0u, 0u);
    }
    return resident_lump(name);
}
}
