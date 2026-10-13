// Observed textureInitTexture__FP12_worldHeaderP8_texturePUcPc at 0x0016d278, 404 bytes. Arguments: a world
// header (never read), a texture record, a byte table (map) and a base address (base). It turns the stored
// 32-bit offsets of a texture record into addresses. Offsets are decimal bytes in the texture record.
// Address conversion, used throughout: a stored value v becomes
//     base + (map[v >> 17] << 17) + (v & 0x1ffff)
// where the shift is arithmetic and map is a table of bytes (the high bits pick a block through the table).
// 1. If the word at +28 is nonzero the function returns at once. Otherwise the halfword at +10 is set to
//    0xa7fb, the word at +28 is set to -1, and the word at +16 is replaced by its converted value.
// 2. With the halfword at +8 (flags) bit 0 set: the word that +16 now points to is converted in place and
//    the function returns.
// 3. Otherwise, with bit 0x100 set, the converted pointer is walked as a directory: its first word is
//    converted in place; then, while the signed byte at the cursor is not -1, four bytes (b0 b1 b2 b3, all
//    signed) are read and the cursor moves past them, and the words that follow are converted in place, one
//    word for each pair (y, x) with y from b1 to b3 and x from b0 to b2, row by row, advancing the cursor by
//    four for each. (If b3 < b1 or b2 < b0 the missing words are not there.) A running total starts at 72
//    and for every four-byte group gains 6 plus ((((b3 - b1) << 11) + 2048) * (b2 - b0 + 1)) / 128, with
//    the division truncating toward zero. On reaching a -1 byte the halfword at +6 receives the total.
// 4. With neither flag bit set nothing more happens. Nothing is returned.
// Inferred only: the directory rows look like rectangles of tiles and the total looks like a byte size.
// Unknown: the record's other fields and the meaning of the number 0xa7fb.
#if defined(NORRATH_HOST_TEXTURE)
extern unsigned char* norrath_texture_arena;
#endif

namespace {
struct Converter {
    const unsigned char* map;
    unsigned base;
    unsigned operator()(unsigned value) const {
        return base + (static_cast<unsigned>(map[static_cast<int>(value) >> 17]) << 17) + (value & 0x1ffffu);
    }
};
inline unsigned& word(unsigned bits) {
#if defined(NORRATH_HOST_TEXTURE)
    return *reinterpret_cast<unsigned*>(norrath_texture_arena + bits);
#else
    return *reinterpret_cast<unsigned*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
inline const signed char& sbyte(unsigned bits) {
#if defined(NORRATH_HOST_TEXTURE)
    return *reinterpret_cast<const signed char*>(norrath_texture_arena + bits);
#else
    return *reinterpret_cast<const signed char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
inline unsigned baseBits(const unsigned char* base) {
#if defined(NORRATH_HOST_TEXTURE)
    return static_cast<unsigned>(base - norrath_texture_arena);
#else
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(base));
#endif
}
}

extern "C" void textureInitTexture__FP12_worldHeaderP8_texturePUcPc(void* world, void* texture,
                                                                       const unsigned char* map,
                                                                       const unsigned char* base) {
    (void)world;
    unsigned char* t = static_cast<unsigned char*>(texture);
    unsigned* words = reinterpret_cast<unsigned*>(t);
    unsigned short* halves = reinterpret_cast<unsigned short*>(t);
    if (words[7] != 0) return;
    Converter convert = {map, baseBits(base)};
    halves[5] = 0xa7fb;
    words[7] = 0xffffffffu;
    unsigned flags = halves[4];
    unsigned directory = convert(words[4]);
    words[4] = directory;
    if (flags & 1) {
        word(directory) = convert(word(directory));
        return;
    }
    if (!(flags & 0x100)) return;
    int total = 72;
    word(directory) = convert(word(directory));
    unsigned cursor = directory + 4;
    while (sbyte(cursor) != -1) {
        int b0 = sbyte(cursor), b1 = sbyte(cursor + 1), b2 = sbyte(cursor + 2), b3 = sbyte(cursor + 3);
        cursor += 4;
        int size = (((b3 - b1) << 11) + 2048) * (b2 - b0 + 1);
        total += size / 128 + 6;
        for (int y = b1; y <= b3; ++y) {
            for (int x = b0; x <= b2; ++x) {
                word(cursor) = convert(word(cursor));
                cursor += 4;
            }
        }
    }
    halves[3] = static_cast<unsigned short>(total);
}
