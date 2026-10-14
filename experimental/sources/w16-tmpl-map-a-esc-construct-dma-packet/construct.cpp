// Observed: ConstructDMAPacket__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag at
// 0x01133890 is 912 bytes. With (cache, surface, info, cursor) it appends DMA packets at the
// address held in *cursor and advances *cursor. The cache is unused. Every "tag" below is a
// 16-byte DMA tag: halfword +0, byte +3 and word +4 are written, nothing else.
//  1. When surface word +40 is nonzero: a cnt-style tag (byte 16, QWC 5, word 0) followed by
//     80 bytes: a VIF/GIF header (+0 and +4 zero, +8 0x11000000, +12 0x50000004), a GIF tag
//     (0x1000000000008003, register descriptor 14), then three address/data pairs:
//     register 80 with data ((int64)info[12] >> 8) << 32 | (int64)info[4] << 56 | 1 << 48,
//     register 81 with 0, register 82 with 0x200000008 when info[20] is 20 and 0x1000000010
//     otherwise. It then writes a ref-style tag (byte 48, ADDR surface[56], QWC low half of
//     surface[60]). *cursor advances by 96 and then 16.
//  2. For i from 0 while i < info[28] (re-read every pass): the same 96-byte block, except word
//     +8 of the header is 0x11000000 only when surface word +40 is zero (0 otherwise) and the
//     first data pair is register 80 with ((int64)info[8+i] >> 8) << 32 | clamp(m >> 6) << 48 |
//     (int64)info[20] << 56, where m is surface[8] halved i times, clamp raises the value to 2
//     when info[20] is 19 or 20 and to 1 otherwise (signed compare); the third pair is register
//     82 with (int64)n << 32 | (int64)m, n being surface[12] halved i times; register 81 holds 0.
//     A ref-style tag follows (byte 48, ADDR surface[21 + 6i], QWC low half of surface[22 + 6i]).
//  3. A final 64-byte block: tag (byte 16, QWC 3, word 0), header words 0, 0, 0, 0x50000002, GIF
//     tag 0x1000000000008001 with descriptor 14, and the data pair (reg 63, data 1).
// It returns 0.
// Inferred: surface[40] chooses between a prepared-transfer and a plain texture upload; the
// registers 80/81/82 are the GS BITBLTBUF/TRXPOS/TRXREG addresses and 63 the TEXFLUSH
// address, which fits the pairs, but no names are claimed for the surface or info fields.
// Unknown: field names of the surface and info structures.
extern "C" {
static inline void store8(unsigned char* base, unsigned offset, unsigned value) {
    *(base + offset) = static_cast<unsigned char>(value);
}
static inline void store16(unsigned char* base, unsigned offset, unsigned value) {
    *reinterpret_cast<unsigned short*>(base + offset) = static_cast<unsigned short>(value);
}
static inline void store32(unsigned char* base, unsigned offset, unsigned value) {
    *reinterpret_cast<unsigned*>(base + offset) = value;
}
static inline void store64(unsigned char* base, unsigned offset, long long value) {
    *reinterpret_cast<long long*>(base + offset) = value;
}
static inline unsigned char* at(const unsigned* cursor) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(*cursor));
}
static inline int word_at(const void* base, unsigned offset) {
    return *reinterpret_cast<const int*>(static_cast<const unsigned char*>(base) + offset);
}

int ConstructDMAPacket__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag(
    void* cache, void* surface, void* info, unsigned* cursor) {
    (void)cache;
    if (word_at(surface, 40) != 0) {
        store8(at(cursor), 3, 16);
        store32(at(cursor), 4, 0);
        unsigned char* packet = at(cursor);
        *cursor = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(packet + 96));
        store16(packet, 0, 5);
        store32(packet + 16, 12, 0x50000004u);
        store32(packet + 16, 8, 0x11000000u);
        store32(packet + 16, 0, 0);
        store32(packet + 16, 4, 0);
        store64(packet + 32, 8, 14);
        store64(packet + 32, 0, 0x1000000000008003LL);
        const long long data = (static_cast<long long>(word_at(info, 12)) >> 8 << 32) |
                               (static_cast<long long>(word_at(info, 4)) << 56) | (1LL << 48);
        store64(packet + 48, 8, 80);
        store64(packet + 48, 0, data);
        store64(packet + 64, 8, 81);
        store64(packet + 64, 0, 0);
        store64(packet + 80, 8, 82);
        store64(packet + 80, 0, word_at(info, 20) == 20 ? 0x200000008LL : 0x1000000010LL);
        store8(at(cursor), 3, 48);
        store32(at(cursor), 4, static_cast<unsigned>(word_at(surface, 56)));
        unsigned char* ref = at(cursor);
        store16(ref, 0, static_cast<unsigned>(word_at(surface, 60)));
        *cursor = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(ref + 16));
    }
    int m = word_at(surface, 8);
    int n = word_at(surface, 12);
    for (int i = 0; i < word_at(info, 28); ++i) {
        store8(at(cursor), 3, 16);
        store32(at(cursor), 4, 0);
        unsigned char* packet = at(cursor);
        *cursor = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(packet + 96));
        store16(packet, 0, 5);
        store32(packet + 16, 0, 0);
        store32(packet + 16, 4, 0);
        store32(packet + 16, 12, 0x50000004u);
        store32(packet + 16, 8, word_at(surface, 40) != 0 ? 0u : 0x11000000u);
        store64(packet + 32, 8, 14);
        store64(packet + 32, 0, 0x1000000000008003LL);
        int clamp = m >> 6;
        const int kind = word_at(info, 20);
        const int minimum = static_cast<unsigned>(kind - 19) < 2u ? 2 : 1;
        if (clamp < minimum) clamp = minimum;
        const long long data = (static_cast<long long>(word_at(info, 32 + 4 * i)) >> 8 << 32) |
                               (static_cast<long long>(clamp) << 48) |
                               (static_cast<long long>(kind) << 56);
        store64(packet + 48, 8, 80);
        store64(packet + 48, 0, data);
        store64(packet + 64, 0, 0);
        store64(packet + 64, 8, 81);
        store64(packet + 80, 8, 82);
        store64(packet + 80, 0, (static_cast<long long>(n) << 32) | static_cast<long long>(m));
        n >>= 1;
        m >>= 1;
        store8(at(cursor), 3, 48);
        store32(at(cursor), 4, static_cast<unsigned>(word_at(surface, 84 + 24 * i)));
        unsigned char* ref = at(cursor);
        store16(ref, 0, static_cast<unsigned>(word_at(surface, 88 + 24 * i)));
        *cursor = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(ref + 16));
    }
    store8(at(cursor), 3, 16);
    store32(at(cursor), 4, 0);
    unsigned char* packet = at(cursor);
    *cursor = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(packet + 64));
    store16(packet, 0, 3);
    store32(packet + 16, 12, 0x50000002u);
    store32(packet + 16, 0, 0);
    store32(packet + 16, 4, 0);
    store32(packet + 16, 8, 0);
    store64(packet + 32, 8, 14);
    store64(packet + 32, 0, 0x1000000000008001LL);
    store64(packet + 48, 8, 63);
    store64(packet + 48, 0, 1);
    return 0;
}
}
