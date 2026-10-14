// Observed worldReset__FP12_worldHeader at 0x0017c448, 612 bytes.
// The header is read as 32-bit fields; all table fields hold addresses.
// Steps, in order (field offsets are decimal):
// 1. h[52] rows by h[48] columns of words at the address in h[56]: each word
//    is replaced by its low byte (zero extended). Row-major, rows*cols words.
// 2. For each of h[0] records of 60 bytes at the address in h[36]: if bit 0x80
//    of the word at record+36 is set and record+0 holds a nonzero pointer p,
//    and the word at p+8 is nonzero (q), then the word at q+4768 and the word
//    at p+8 are cleared. If bit 0x800 of that same flag word is set, the word
//    at record+4 is cleared (h[0] is re-read afterwards).
// 3. For each of h[80] door records of 28 bytes at h[84]: clear bit 0 of the
//    half at +6. For j below the signed half at +4: record index word(+0)+j
//    of the 60-byte table gets word(+8) at +40, word(+12) at +44, word(+16)
//    at +48, half(+20) at +54, half(+22) at +56; then the half at +16 of the
//    36-byte record at h[32] + signed half(+24)*36 has bit 1 cleared.
// 4. 64 records of 48 bytes at worldAnimation have the byte at +4 and the
//    word at +40 cleared.
// 5. Water globals are set: worldWaterWavyness 0.0234375f, worldWaterBumpAmount
//    41.0f, worldWaterBumpRadius 4.0f, worldWaterBumpTimer 6, worldWaterAlpha 98,
//    worldWaterColor 0x00808080, and worldWaterWavyness2, worldWaterAlpha2,
//    worldWaterColor2 zero. Nothing is returned.
// The water globals and worldAnimation are defined by other packages and
// stay unresolved imports. The record meanings are not established here.
#if defined(NORRATH_HOST_WORLD)
extern unsigned char* norrath_world_arena;
#endif

extern "C" {
extern float worldWaterWavyness;
extern float worldWaterBumpAmount;
extern float worldWaterBumpRadius;
extern int worldWaterBumpTimer;
extern int worldWaterAlpha;
extern unsigned worldWaterColor;
extern float worldWaterWavyness2;
extern int worldWaterAlpha2;
extern unsigned worldWaterColor2;
extern unsigned char worldAnimation[];
}

namespace {
inline unsigned word(const unsigned char* base, unsigned offset) {
    return *reinterpret_cast<const unsigned*>(base + offset);
}
inline int half(const unsigned char* base, unsigned offset) {
    return *reinterpret_cast<const short*>(base + offset);
}
inline unsigned char* address(unsigned bits) {
#if defined(NORRATH_HOST_WORLD)
    return norrath_world_arena + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
inline unsigned char* table(const unsigned char* header, unsigned offset) {
    return address(word(header, offset));
}
}

extern "C" void worldReset__FP12_worldHeader(void* world) {
    unsigned char* header = static_cast<unsigned char*>(world);

    const int rows = static_cast<int>(word(header, 52));
    const int columns = static_cast<int>(word(header, 48));
    for (int row = 0, base = 0; row < rows; ++row, base += columns) {
        if (columns <= 0) continue;
        unsigned* cell = reinterpret_cast<unsigned*>(table(header, 56)) + base;
        for (int c = 0; c < columns; ++c) {
            cell[c] = static_cast<unsigned char>(cell[c]);
        }
    }

    for (int i = 0; i < static_cast<int>(word(header, 0)); ++i) {
        unsigned char* record = table(header, 36) + i * 60;
        const unsigned flags = word(record, 36);
        if ((flags & 0x80u) != 0) {
            unsigned char* owner = address(word(record, 0));
            const unsigned patch = word(owner, 8);
            if (patch != 0u) {
                *reinterpret_cast<unsigned*>(address(patch) + 4768) = 0u;
                *reinterpret_cast<unsigned*>(owner + 8) = 0u;
            }
        }
        if ((flags & 0x800u) != 0) {
            *reinterpret_cast<unsigned*>(record + 4) = 0u;
        }
    }

    const int doors = static_cast<int>(word(header, 80));
    for (int d = 0; d < doors; ++d) {
        unsigned char* door = table(header, 84) + d * 28;
        *reinterpret_cast<unsigned short*>(door + 6) =
            static_cast<unsigned short>(*reinterpret_cast<unsigned short*>(door + 6) & 0xfffeu);
        for (int j = 0; j < half(door, 4); ++j) {
            unsigned char* record = table(header, 36) + (static_cast<int>(word(door, 0)) + j) * 60;
            *reinterpret_cast<unsigned*>(record + 40) = word(door, 8);
            *reinterpret_cast<unsigned*>(record + 44) = word(door, 12);
            *reinterpret_cast<unsigned*>(record + 48) = word(door, 16);
            *reinterpret_cast<unsigned short*>(record + 54) = *reinterpret_cast<unsigned short*>(door + 20);
            *reinterpret_cast<unsigned short*>(record + 56) = *reinterpret_cast<unsigned short*>(door + 22);
            unsigned short* item = reinterpret_cast<unsigned short*>(table(header, 32) + half(door, 24) * 36 + 16);
            *item = static_cast<unsigned short>(*item & 0xfffdu);
        }
    }

    for (int i = 0; i < 64; ++i) {
        unsigned char* record = worldAnimation + i * 48;
        record[4] = 0;
        *reinterpret_cast<unsigned*>(record + 40) = 0u;
    }

    worldWaterWavyness = 0.0234375f;
    worldWaterBumpAmount = 41.0f;
    worldWaterBumpRadius = 4.0f;
    worldWaterBumpTimer = 6;
    worldWaterAlpha = 98;
    worldWaterColor = 0x00808080u;
    worldWaterWavyness2 = 0.0f;
    worldWaterAlpha2 = 0;
    worldWaterColor2 = 0u;
}
