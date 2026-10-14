// Observed: SetObjectBlends__8VIRasteriP10VIMatrix44 at 0x0110abf8 is 372 bytes. Arguments: the
// raster object (a0), a count n (a1) and the address of n 64-byte matrices (a2). The raster object
// holds a write pointer at offset 19492 (call it P). With q = n / 32 (signed), r the remainder
// n - 32*q, and k = 4*n + q + 1:
//   1. at the old P: byte 3 = 16, word at offset 4 = 0, halfword at offset 0 = k; P advances by
//      (k + 1) * 16.
//   2. for each of the q full chunks, at the current write address w (first w = old P + 16): words
//      0 and 1 = 0, word 2 = 0x10000000, word 3 = (the static _8VIRaster$ObjectBlendsBuffer, 0x011b019c,
//      + 4*done) | 0x6c800000, then memcpy(w + 16, matrices + 64*done, 2048) and w advances by 2064;
//      done advances by 32.
//   3. if r != 0: the same four header words at w, with word 3 = (ObjectBlendsBuffer + 4*done) |
//      (r << 18) | 0x6c000000, then memcpy(w + 16, matrices + 64*done, 64*r).
// It returns 0. P itself is not touched after step 1.
// Inferred: this builds a DMA chain tag (halfword count, ID in the top byte) followed by one
// header and data block per batch of 32 matrices; the 0x10000000 and 0x6c.. words look like VIF
// codes (a flush, then an unpack whose count is 4 per matrix). The +1 in k reserves a header
// even when r is zero, and nothing is written there. Negative n is not meaningful and would loop.
// Unknown: the consumer of the chain, the meaning of the static, and the 16-bit truncation of k
// (more than 16383 matrices would overflow it).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern unsigned ObjectBlendsBuffer __asm__("_8VIRaster$ObjectBlendsBuffer");

extern "C" {
void* memcpy(void* dest, const void* src, unsigned long count);

int SetObjectBlends__8VIRasteriP10VIMatrix44(unsigned char* self, int count, const unsigned char* matrices) {
    unsigned* slot = reinterpret_cast<unsigned*>(self + 19492);
    int chunks = count / 32;
    int words = count * 4 + chunks + 1;
    at(*slot)[3] = 16;
    *reinterpret_cast<unsigned*>(at(*slot) + 4) = 0;
    unsigned base = *slot;
    *reinterpret_cast<unsigned short*>(at(base)) = static_cast<unsigned short>(words);
    *slot = base + 16 + words * 16;
    unsigned write = base + 16;
    int done = 0;
    for (int i = chunks; i != 0; --i) {
        unsigned* header = reinterpret_cast<unsigned*>(at(write));
        header[0] = 0;
        header[1] = 0;
        header[2] = 0x10000000u;
        header[3] = (ObjectBlendsBuffer + done * 4) | 0x6c800000u;
        memcpy(at(write + 16), matrices + done * 64, 2048);
        write += 16 + 2048;
        done += 32;
    }
    int rest = count - done;
    if (rest != 0) {
        unsigned* header = reinterpret_cast<unsigned*>(at(write));
        header[0] = 0;
        header[2] = 0x10000000u;
        header[1] = 0;
        header[3] = ((ObjectBlendsBuffer + done * 4) | (static_cast<unsigned>(rest) << 18)) | 0x6c000000u;
        memcpy(at(write + 16), matrices + done * 64, static_cast<unsigned long>(rest * 64));
    }
    return 0;
}
}
