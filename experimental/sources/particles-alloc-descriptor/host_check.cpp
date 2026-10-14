// Host contract for allocDescriptor__FP11ParticleDef. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" int allocDescriptor__FP11ParticleDef(unsigned char* descriptor);
extern "C" unsigned char descListMark[128] = {};
extern "C" unsigned char descList[32 * 64] = {};

namespace {
unsigned read_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}

void write_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}

void fill_marks(unsigned first) {
    for (int index = 0; index < 32; ++index) write_word(descListMark + index * 4, 0xffffffffu);
    if (first > 0u) {
        for (unsigned index = 0; index < first; ++index) write_word(descListMark + index * 4, 7u);
    }
}
}

int main() {
    unsigned char descriptor[80];
    std::memset(descriptor, 0x3c, sizeof descriptor);
    std::memset(descList, 0x11, sizeof descList);
    fill_marks(0);
    if (allocDescriptor__FP11ParticleDef(descriptor) != 1) return 1;
    if (read_word(descListMark) != 1u || read_word(descriptor + 4) != 0u) return 1;
    if (std::memcmp(descList, descriptor + 16, 64) != 0) return 1;
    if (descList[64] != 0x11) return 1;

    std::memset(descList, 0x11, sizeof descList);
    fill_marks(31);
    std::memset(descriptor, 0x5a, sizeof descriptor);
    if (allocDescriptor__FP11ParticleDef(descriptor) != 1) return 1;
    if (read_word(descListMark + 31 * 4) != 1u || read_word(descriptor + 4) != 31u) return 1;
    if (std::memcmp(descList + 31 * 64, descriptor + 16, 64) != 0) return 1;
    if (descriptor[0] != 0x5a || descriptor[15] != 0x5a) return 1;

    fill_marks(32);
    const unsigned kept = read_word(descriptor + 4);
    if (allocDescriptor__FP11ParticleDef(descriptor) != 0) return 1;
    if (read_word(descriptor + 4) != kept) return 1;
    for (int index = 0; index < 32; ++index) {
        if (read_word(descListMark + index * 4) != 7u) return 1;
    }
    return 0;
}
