#include "../w10-actor-q-common/q.hpp"
// Observed dropItem__14WoodElfSoldieri at 0x00249038, 976 bytes.
// Does nothing unless the argument is 1. Then the word at +964 (a variant 0 to 3, re-read for each
// test) selects what is dropped. Each step allocates a 584-byte block with blockAlloc__Fi and, when that
// succeeds, builds a PhProp on it from: a copy of the Point3 at +16, the signed halfword at +56, and
// m[12], the animation state at +288, m[0], m[8], m[16] with m the pointer stored at +36, then a 64-bit
// value, 16385 and no description, and the float 1.0. The steps are
//   variant 0: set bit 0x800 of the word at +244; pieces 0x34000000 and 0x40000000
//   variant 1: set bit 0x1000 of +244;            pieces 0x01800000 and 0x02000000
//   variant 2: set bit 0x2000 of +244;            pieces 0x34000000 and 0x00180000
//   variant 3: set bit 0x1000 of +244;            pieces 0x01800000 and 0x02000000
// (the flag bit is written before the first allocation of its variant, and the second allocation is
// made even when the first one failed). At the end modelGetActiveChunks__FP12_modelHeaderi(m[12], the
// word at +244) is stored as a doubleword at +552, with m re-read from +36. Nothing is returned.
// Inferred: the pieces are body parts or gibs of the soldier by variant. Unknown: the meaning of the
// 64-bit values (probably masks), of 16385, and of the variant.
// Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void __6PhPropG6Point3sP12_modelHeaderP14AnimationStatePcN25liPQ24item11descriptionf(
    void* self, const float* position, short heading, void* model, void* animation, char* first, char* second,
    char* third, long long value, int flags, void* description, float scale);
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int requested);
}

namespace {
void make_piece(void* self, long long value) {
    using namespace w10q;
    void* block = blockAlloc__Fi(584);
    if (block == nullptr) return;
    float position[3] = {real(self, 16), real(self, 20), real(self, 24)};
    unsigned char* m = link(self, 36);
    __6PhPropG6Point3sP12_modelHeaderP14AnimationStatePcN25liPQ24item11descriptionf(
        block, position, static_cast<short>(shalf(self, 56)), link(m, 12), bytes(self) + 288,
        reinterpret_cast<char*>(link(m, 0)), reinterpret_cast<char*>(link(m, 8)), reinterpret_cast<char*>(link(m, 16)), value,
        16385, nullptr, 1.0f);
}
}  // namespace

extern "C" void dropItem__14WoodElfSoldieri(void* self, int which) {
    using namespace w10q;
    if (which != 1) return;
    if (word(self, 964) == 0u) {
        word(self, 244) = word(self, 244) | 0x800u;
        make_piece(self, 0x34000000ll);
        make_piece(self, 0x40000000ll);
    }
    if (word(self, 964) == 1u) {
        word(self, 244) = word(self, 244) | 0x1000u;
        make_piece(self, 0x01800000ll);
        make_piece(self, 0x02000000ll);
    }
    if (word(self, 964) == 2u) {
        word(self, 244) = word(self, 244) | 0x2000u;
        make_piece(self, 0x34000000ll);
        make_piece(self, 0x00180000ll);
    }
    if (word(self, 964) == 3u) {
        word(self, 244) = word(self, 244) | 0x1000u;
        make_piece(self, 0x01800000ll);
        make_piece(self, 0x02000000ll);
    }
    unsigned char* m = link(self, 36);
    *reinterpret_cast<Double*>(bytes(self) + 552) = modelGetActiveChunks__FP12_modelHeaderi(link(m, 12), static_cast<int>(word(self, 244)));
}
