// Observed lumpPartialLoad__FPcT0 at 0x00165768, 968 bytes. Arguments: a lump file name and the name
// of one resource inside it. Lump records are the 40-byte entries of lumps (0x006cb280): name text
// at +0, the data pointer at +16, a flag word at +20 (bit 0 set on records made here), the byte size
// at +24, zero at +28, a lock word at +32 and a flag word at +36. nmLumps is the record count.
//  1. The first record (in order) whose name is strcmp-equal to the lump name and whose flag word at
//     +20 has bit 0 clear is a fully loaded lump when its data pointer is nonzero: the result is then
//     lumpFindResource__FPcT0(lump, resource) (a tail call).
//  2. Otherwise the records are scanned for a name equal to the lump name (strcmp(lump, name)) with
//     bit 0 of the +20 flag set; if the text at the record's data pointer is strcmp-equal to the
//     resource name, the result is that pointer + 256 (the resource data of an earlier partial load).
//  3. Otherwise a block of 0x100000 bytes is allocated (memAlloc__FiPc, labelled with the next free
//     record's name field) and the first 0x8000 bytes of the lump file are read into it
//     (cdLoad__FPcPvii(lump, block, 0x8000, 0)). The block starts with a halfword entry count and
//     12-byte directory entries from +4: a name offset, a file offset and a size. When the last
//     entry's offset, plus 2175 and rounded down to 2048, is above 0x8000, the rest of the directory
//     is read to block + 0x8000 (length that value minus 0x8000, file offset 0x8000) and the count is
//     read again. Every entry's name offset is then turned into a pointer by adding the block.
//  4. The resource name is looked up by binary search over the entries (strcmp(resource, name)); with
//     no match the block is freed (memFree__FPv) and the result is zero.
//  5. With a match, the size is rounded up to 2048 (s), the resource is read to block + 256
//     (cdLoad(lump, block + 256, s, file offset)), the resource name is copied to the start of the
//     block, initFile__FPcPvi(resource, block + 256, s) returns n, the block is shrunk to
//     s - n + 256 bytes (memRealloc__FPvi), and a new record is appended: name = lump name, +16 =
//     block, +20 = 1, +24 = the new block size, +28 = 0, +32 = 0, +36 = 1; nmLumps increases by one.
//     The result is the block (not block + 256).
// Inferred: this loads one resource of a lump file on its own, keeping a 256-byte header with its
// name in front of the data. Unknown: the roles of the directory entry words beyond the names, what
// initFile's result means, and the meaning of the flag at +36.
extern "C" {
extern int nmLumps;
extern unsigned char lumps[];
int strcmp(const char* left, const char* right);
char* strcpy(char* to, const char* from);
void* lumpFindResource__FPcT0(char* lump, char* resource);
void* memAlloc__FiPc(int size, char* label);
void cdLoad__FPcPvii(char* name, void* destination, int size, int offset);
int initFile__FPcPvi(char* name, void* data, int size);
void memRealloc__FPvi(void* block, int size);
void memFree__FPv(void* block);
}

#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_base;
#endif

namespace {
// Pointer words of the image are 32-bit; a host check maps them to offsets in one arena.
inline unsigned char* at(unsigned word) {
#ifdef NORRATH_HOST_ARENA
    return norrath_host_base + word;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
inline unsigned wordOf(const void* pointer) {
#ifdef NORRATH_HOST_ARENA
    return static_cast<unsigned>(static_cast<const unsigned char*>(pointer) - norrath_host_base);
#else
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
#endif
}
inline unsigned& word(unsigned char* base, int offset) { return *reinterpret_cast<unsigned*>(base + offset); }
}

extern "C" void* lumpPartialLoad__FPcT0(char* lump, char* resource) {
    int index = 0;
    for (; index < nmLumps; ++index) {
        unsigned char* record = lumps + 40 * index;
        if (strcmp(reinterpret_cast<char*>(record), lump) == 0 && (word(record, 20) & 1) == 0) break;
    }
    unsigned loaded = 0;
    if (index < nmLumps) loaded = word(lumps + 40 * index, 16);
    if (loaded != 0) return lumpFindResource__FPcT0(lump, resource);

    for (index = 0; index < nmLumps; ++index) {
        unsigned char* record = lumps + 40 * index;
        if (strcmp(lump, reinterpret_cast<char*>(record)) == 0 && (word(record, 20) & 1) != 0) {
            if (strcmp(resource, reinterpret_cast<char*>(at(word(record, 16)))) == 0) return at(word(record, 16)) + 256;
        }
    }

    unsigned char* block = static_cast<unsigned char*>(memAlloc__FiPc(0x100000, reinterpret_cast<char*>(lumps + 40 * nmLumps)));
    cdLoad__FPcPvii(lump, block, 0x8000, 0);
    unsigned count = *reinterpret_cast<unsigned short*>(block);
    unsigned char* entries = block + 4;
    const int reach = (static_cast<int>(word(entries + 12 * count - 12, 0)) + 2175) & -2048;
    if (0x8000 < reach) {
        cdLoad__FPcPvii(lump, block + 0x8000, reach - 0x8000, 0x8000);
        count = *reinterpret_cast<unsigned short*>(block);
    }
    if (count != 0) {
        for (unsigned i = 0; i < count; ++i) word(entries + 12 * i, 0) = word(entries + 12 * i, 0) + wordOf(block);
    }
    int low = 0;
    int high = static_cast<int>(count) - 1;
    const int top = high;
    unsigned char* found = nullptr;
    for (;;) {
        const int middle = (low + high) >> 1;
        int order = 1;
        if (middle >= 0) {
            order = -1;
            if (!(top < middle)) order = strcmp(resource, reinterpret_cast<char*>(at(word(entries + 12 * middle, 0))));
        }
        unsigned char* entry = entries + 12 * middle;
        if (order == 0) {
            found = entry;
            break;
        }
        if (!(low < high)) break;
        if (order <= 0) high = middle - 1;
        else low = middle + 1;
    }
    if (found == nullptr) {
        memFree__FPv(block);
        return nullptr;
    }
    int size = static_cast<int>(word(found, 8));
    const int fileOffset = static_cast<int>(word(found, 4));
    unsigned char* data = block + 256;
    size = (size + 2047) & -2048;
    cdLoad__FPcPvii(lump, data, size, fileOffset);
    strcpy(reinterpret_cast<char*>(block), resource);
    const int used = initFile__FPcPvi(resource, data, size);
    const int newSize = size - used + 256;
    memRealloc__FPvi(block, newSize);
    unsigned char* record = lumps + 40 * nmLumps;
    strcpy(reinterpret_cast<char*>(record), lump);
    const int slot = nmLumps;
    nmLumps = slot + 1;
    record = lumps + 40 * slot;
    word(record, 20) = 1;
    word(record, 36) = 1;
    word(record, 24) = static_cast<unsigned>(newSize);
    word(record, 16) = wordOf(block);
    word(record, 28) = 0;
    word(record, 32) = 0;
    return block;
}
