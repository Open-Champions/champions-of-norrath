// allocDescriptor__FP11ParticleDef at 0x001872c0, 384 bytes, read from the
// pinned SLUS-20565 image. Fresh compilation is not an original-byte match.
// No descriptor has been allocated on the EE.
//
// descListMark at 0x00780280 is 32 words. The first word equal to 0xffffffff
// is the chosen slot; 32 means none. The chosen word becomes 1. The
// descriptor's word at +4 receives the slot index. Sixty-four bytes at
// descriptor+16 are copied to descList at 0x00780300 plus index times 64.
// The value in v0 is 1 when a slot is taken and 0 when every word is taken.
extern "C" {
extern unsigned char descListMark[];
extern unsigned char descList[];

static unsigned read_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}

static void write_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}

int allocDescriptor__FP11ParticleDef(unsigned char* descriptor) {
    unsigned index = 0u;
    if (read_word(descListMark) != 0xffffffffu) {
        index = 1u;
        while (index < 32u && read_word(descListMark + index * 4u) != 0xffffffffu) ++index;
    }
    if (index == 32u) return 0;
    write_word(descListMark + index * 4u, 1u);
    write_word(descriptor + 4, index);
    for (unsigned byte = 0; byte < 64u; ++byte) {
        descList[index * 64u + byte] = descriptor[16u + byte];
    }
    return 1;
}
}
