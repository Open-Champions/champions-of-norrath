// __15AnimationState8RC15AnimationState8 at 0x163010, 588 bytes, read from the
// pinned SLUS-20565 image. Nothing here has run on the EE.
//
// Observed, in the image's order. The byte at offset 5 of the new object is set
// to 1, then the first 264 bytes of the source (8 + 8 * 32: a header and eight
// 32-byte entries) are copied over the new object, and the word at 0 is set to
// zero. The byte at 5 is then read back as a signed value (so the copied count
// counts). For each index below that count, with entry = object + 8 + 32 * index:
//   - if the source entry's pointer (source + 12 + 32 * index) is null, or the
//     halfword at entry + 28 of the new object has none of the bits 0x4800, the
//     new entry's pointer at entry + 4 is set to zero;
//   - otherwise getFreeFrameState__Fv is called, its result is stored at entry + 4,
//     and in that block: offset 20 gets the address of the entry, offsets 8, 0 and
//     4 get the same words of the source block, and the 3840 bytes from offset 28
//     up to 3868 are copied from the source block.
// The store of 1 at offset 5 happens before the copy and is overwritten by it, so it
// has no effect on the result; it is kept to follow the image.
// The image has two copy paths for each block copy (one when both addresses are
// 8-byte aligned, one byte-wise-safe); they do the same thing and this source has one.
// Inferred: the count byte and the entry pointers because of how they are used.
// Unknown: the meaning of the other header bytes, the entry fields and the flags.
extern "C" {
void* getFreeFrameState__Fv();

static void norrath_copy_bytes(unsigned char* to, const unsigned char* from, unsigned count) {
    for (unsigned i = 0; i < count; ++i) to[i] = from[i];
}

static unsigned norrath_load_word(const unsigned char* at) {
    return static_cast<unsigned>(at[0]) | (static_cast<unsigned>(at[1]) << 8) | (static_cast<unsigned>(at[2]) << 16)
        | (static_cast<unsigned>(at[3]) << 24);
}

static void norrath_store_word(unsigned char* at, unsigned value) {
    at[0] = static_cast<unsigned char>(value);
    at[1] = static_cast<unsigned char>(value >> 8);
    at[2] = static_cast<unsigned char>(value >> 16);
    at[3] = static_cast<unsigned char>(value >> 24);
}

void __15AnimationState8RC15AnimationState8(unsigned char* self, const unsigned char* source) {
    self[5] = 1;
    norrath_copy_bytes(self, source, 264);
    norrath_store_word(self, 0);
    const int count = static_cast<signed char>(self[5]);
    for (int index = 0; index < count; ++index) {
        unsigned char* entry = self + 8 + 32 * index;
        const unsigned char* source_frame = reinterpret_cast<const unsigned char*>(norrath_load_word(source + 12 + 32 * index));
        const unsigned flags = static_cast<unsigned>(entry[28]) | (static_cast<unsigned>(entry[29]) << 8);
        if (source_frame == nullptr || (flags & 0x4800u) == 0) {
            norrath_store_word(entry + 4, 0);
            continue;
        }
        unsigned char* frame = static_cast<unsigned char*>(getFreeFrameState__Fv());
        norrath_store_word(entry + 4, reinterpret_cast<unsigned long>(frame));
        norrath_store_word(frame + 20, reinterpret_cast<unsigned long>(entry));
        norrath_store_word(frame + 8, norrath_load_word(source_frame + 8));
        norrath_store_word(frame + 0, norrath_load_word(source_frame + 0));
        norrath_store_word(frame + 4, norrath_load_word(source_frame + 4));
        norrath_copy_bytes(frame + 28, source_frame + 28, 3840);
    }
}
}
