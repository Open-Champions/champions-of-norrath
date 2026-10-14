// Observed: End__12VIPrimBuffer at 0x01101ca0 is 1184 bytes. The type word at offset 8 selects
// through a nine-entry jump table: types 0, 2, 4, 5 and 8 are handled; 1, 3, 6, 7 and anything
// above 8 return -1 and change nothing. Notation: n = word at offset 72, c = cursor at offset 80,
// m = word at offset 64; V = static _12VIPrimBuffer$PrimVQStart (read once, kept); R = array of
// 16-byte records at (pointer at offset 96); N = array of 28-byte records at (pointer at 92);
// M = array of 16-byte records at (pointer at 88); count = N[n] word 0. For a handled type:
//   * R[V] word 3 = 0x6c018000 for types 2, 4, 5, 8; for type 0 it is
//     ((count << 18) + 0x10000) | 0x6c008000, with count read before the store;
//   * R[V+1] word 0 = N[n] word 0, and R[V+1] words 1..3 = 0 (the next record, reached as words 4..7);
//   * R[c] words 0..2 = 0, then R[c] word 3 = strip | 0x14000000 where strip is the static word
//     _8VIRaster$RStripOffset (type 0), RStrip2Offset (2), RStrip3Offset (4), RStrip4Offset (5) or
//     RStrip6Offset (8), read at the end of the sequence;
//   * the cursor becomes c + 1; M[m] word 1 is incremented; the word at offset 72 is incremented.
// It returns 0. Memory is re-read between stores in the original's order. R[V] and R[c] may be the
// same or adjacent records; the sequence above is kept in the original's order for that reason.
// Inferred: Begin reserves two header records at V and End fills them with a GIF/DMA-style tag
// plus the primitive count, then appends a pointer-style word for a strip routine and counts the
// primitive in the current material. Unknown: tag field meanings, why 0x6c008000/0x6c018000 differ,
// the strip words' contents (initialised elsewhere), and the real names.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern int PrimVQStart __asm__("_12VIPrimBuffer$PrimVQStart");
extern unsigned RStripOffset __asm__("_8VIRaster$RStripOffset");
extern unsigned RStrip2Offset __asm__("_8VIRaster$RStrip2Offset");
extern unsigned RStrip3Offset __asm__("_8VIRaster$RStrip3Offset");
extern unsigned RStrip4Offset __asm__("_8VIRaster$RStrip4Offset");
extern unsigned RStrip6Offset __asm__("_8VIRaster$RStrip6Offset");

namespace {
inline unsigned* word_array(unsigned char* self, int pointer_offset, int index, int stride) {
    return reinterpret_cast<unsigned*>(at(*reinterpret_cast<unsigned*>(self + pointer_offset)) + index * stride);
}
inline int word(unsigned char* self, int offset) { return *reinterpret_cast<int*>(self + offset); }
}

extern "C" {
int End__12VIPrimBuffer(unsigned char* self) {
    unsigned type = *reinterpret_cast<unsigned*>(self + 8);
    if (type > 8 || !((1u << type) & 0x135u))
        return -1;
    int vq = PrimVQStart;
    unsigned* header = word_array(self, 96, vq, 16);
    if (type == 0) {
        unsigned count = word_array(self, 92, word(self, 72), 28)[0];
        header[3] = ((count << 18) + 0x10000u) | 0x6c008000u;
    } else {
        header[3] = 0x6c018000u;
    }
    word_array(self, 96, vq, 16)[4] = word_array(self, 92, word(self, 72), 28)[0];
    word_array(self, 96, vq, 16)[5] = 0;
    word_array(self, 96, vq, 16)[6] = 0;
    word_array(self, 96, vq, 16)[7] = 0;
    word_array(self, 96, word(self, 80), 16)[0] = 0;
    word_array(self, 96, word(self, 80), 16)[1] = 0;
    word_array(self, 96, word(self, 80), 16)[2] = 0;
    unsigned strip;
    switch (type) {
    case 0: strip = RStripOffset; break;
    case 2: strip = RStrip2Offset; break;
    case 4: strip = RStrip3Offset; break;
    case 5: strip = RStrip4Offset; break;
    default: strip = RStrip6Offset; break;
    }
    word_array(self, 96, word(self, 80), 16)[3] = strip | 0x14000000u;
    *reinterpret_cast<int*>(self + 80) = word(self, 80) + 1;
    unsigned* material = word_array(self, 88, word(self, 64), 16);
    material[1] = material[1] + 1;
    *reinterpret_cast<int*>(self + 72) = word(self, 72) + 1;
    return 0;
}
}
