// Observed worldAllocateSegments__FP12_worldHeader at 0x00174cb0, 1600 bytes.
// Header words used: a record count at +0, a 60-byte record array pointer at +36, three
// words at +88/+92/+100 (a lower bound, an upper bound and a table of 8-byte entries), a
// word at +96 and an allocation pointer at +124.
// 1. For each record whose word at +36 has bit 11 (0x800) set, the string "%s.lmp" is
//    formatted with the record's first word (a name pointer) into a stack buffer of 160
//    bytes and passed to lumpLoad__FPc; the count is re-read after each load.
// 2. texLoadAlwaysLoadedPages__Fv is called.
// 3. When the word at +96 is zero the function ends; when the pointer at +124 is already
//    set it also ends. Otherwise it scans a 100-wide map: cell (row, column) is
//    texmap[100 * row + column - texmapstart] (a short; texmapstart and texmapend are shorts
//    bounding the valid indices, anything outside them counts as 0). A cell value t counts as
//    0 when it is outside [header+88, header+92]; otherwise it is the signed quotient
//    (word at +4 of entry (t - header+88) in the table at +100, plus 0x1ffff) / 0x20000.
//    For every interior cell, rows 1..98 and columns 1..98, the nine values of the 3x3
//    neighbourhood are summed and the largest sum (never below 0) is kept.
// 4. That maximum is stored at +96 and memAlloc__FiPc(maximum << 17, "segment cache") is
//    stored at +124.
// Inferred: the map assigns segment-sized chunks to grid cells and this sizes a cache for the
// most chunks any 3x3 block needs. Unknown: the exact meaning of the three header words and
// the table; texmap's declared extent in the image is 0x1900 bytes, short of 100 * 100
// cells, so the scan is reproduced as observed without claiming that every index is valid.
// Hosts: with NORRATH_HOST_ARENA stored 32-bit pointers are offsets into an arena.
#if defined(NORRATH_HOST_ARENA)
extern unsigned char* norrath_alloc_arena;
#endif
struct _worldHeader;
extern "C" {
extern short texmapstart;
extern short texmapend;
extern short texmap[];
int sprintf(char*, const char*, ...);
void lumpLoad__FPc(char*);
void texLoadAlwaysLoadedPages__Fv();
void* memAlloc__FiPc(int size, char* name);
}

namespace {
inline int word(const unsigned char* base, unsigned offset) { return *reinterpret_cast<const int*>(base + offset); }
inline unsigned char* link(const unsigned char* base, unsigned offset) {
    const unsigned bits = *reinterpret_cast<const unsigned*>(base + offset);
#if defined(NORRATH_HOST_ARENA)
    return norrath_alloc_arena + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
int cell_chunks(const unsigned char* header, int index, int first, int last) {
    if (index < first) return 0;
    if (last < index) return 0;
    const int t = texmap[index - first];
    if (t < word(header, 88)) return 0;
    if (word(header, 92) < t) return 0;
    const int entry = *reinterpret_cast<const int*>(link(header, 100) + (t - word(header, 88)) * 8 + 4);
    return (entry + 0x1ffff) / 0x20000;
}
}

extern "C" void worldAllocateSegments__FP12_worldHeader(_worldHeader* world) {
    unsigned char* header = reinterpret_cast<unsigned char*>(world);
    char name[160];
    for (int i = 0; i < word(header, 0); ++i) {
        unsigned char* record = link(header, 36) + i * 60;
        if (word(record, 36) & 0x800) {
            sprintf(name, "%s.lmp", reinterpret_cast<char*>(link(record, 0)));
            lumpLoad__FPc(name);
        }
    }
    texLoadAlwaysLoadedPages__Fv();
    if (word(header, 96) == 0) return;
    if (word(header, 124) != 0) return;
    const int first = texmapstart;
    const int last = texmapend;
    int best = 0;
    for (int row = 1; row < 99; ++row) {
        for (int column = 1; column < 99; ++column) {
            int sum = 0;
            for (int r = row - 1; r <= row + 1; ++r)
                for (int c = column - 1; c <= column + 1; ++c)
                    sum += cell_chunks(header, 100 * r + c, first, last);
            if (best < sum) best = sum;
        }
    }
    *reinterpret_cast<int*>(header + 96) = best;
    void* block = memAlloc__FiPc(best << 17, const_cast<char*>("segment cache"));
    *reinterpret_cast<unsigned*>(header + 124) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(block));
}
