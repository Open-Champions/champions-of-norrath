// Observed loseBodyPart__10FireBeetlei at 0x00211790, 340 bytes. Let R be the block at the word object+36 and
// M its word at +12. A 64-bit value "mask" starts at 0. For (part - 2) below 6 (unsigned):
//     part 2: word +244 |= 0x2,  mask = 4        part 3: |= 0x4,  mask = 6
//     part 4: |= 0x8,  mask = 64                 part 5: |= 0x10, mask = 3072
//     part 6: |= 0x20, mask = 128                part 7: |= 0x40, mask = 768
// Other parts change nothing. Then, always: blockAlloc(616); when a block comes back the GutChunk constructor
// __8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif runs on it with: a copy of the point at +16,
// the halfword at +56, M, the animation state at +288, the words R+0, R+8 and R+16, mask, 7, 2 and 1.0, and
// the float at block+452 gets 2.0 added. Finally the 64-bit modelGetActiveChunks(M, word +244) is stored at
// +552. Unlike the Anti version the chunks are not compared before and after; mask is a fixed constant per
// part. Unknown: what the masks select (they are the chunk bits of the effect, inferred) and the ints 7 and
// 2. Nothing here ran on the EE.
#include "../w11-actor-a-common/access.hpp"

extern "C" {
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int mask);
void* blockAlloc__Fi(int size);
void __8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif(void* self, Point3 position, short angle, void* model,
                                                                    void* animation, char* a, char* b, char* c,
                                                                    unsigned long long chunks, int type, int two, float scale);

void loseBodyPart__10FireBeetlei(void* self, int part) {
    using namespace w11a;
    unsigned char* const o = static_cast<unsigned char*>(self);
    static const unsigned short masks[6] = {4, 6, 64, 3072, 128, 768};
    unsigned long long mask = 0;
    if (static_cast<unsigned>(part - 2) < 6u) {
        word(o, 244) |= 2u << (part - 2);
        mask = masks[part - 2];
    }
    unsigned char* const chunk = static_cast<unsigned char*>(blockAlloc__Fi(616));
    if (chunk != nullptr) {
        unsigned char* const r = from_word(word(o, 36));
        __8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif(
            chunk, *reinterpret_cast<Point3*>(o + 16), half(o, 56), from_word(word(r, 12)), o + 288,
            reinterpret_cast<char*>(from_word(word(r, 0))), reinterpret_cast<char*>(from_word(word(r, 8))),
            reinterpret_cast<char*>(from_word(word(r, 16))), mask, 7, 2, 1.0f);
        real(chunk, 452) += 2.0f;
    }
    unsigned char* const r2 = from_word(word(o, 36));
    dword(o, 552) = modelGetActiveChunks__FP12_modelHeaderi(from_word(word(r2, 12)), static_cast<int>(word(o, 244)));
}
}
