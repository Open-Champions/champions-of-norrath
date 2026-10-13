// Observed loseBodyPart__3Anti at 0x00206a78, 400 bytes. Let R be the block at the word object+36 and M its
// word at +12. In order:
//  1. before = modelGetActiveChunks(M, word +244) (a 64-bit mask).
//  2. For part < 8 (unsigned) the bit 2 << part is set in the word at +244; larger parts change nothing.
//  3. The 64-bit result of modelGetActiveChunks(M, word +244) is stored at +552; the chunks that were
//     active before and are not now (before & ~after) are kept.
//  4. blockAlloc(616). When a block comes back, the GutChunk constructor __8GutChunkG6Point3sP12_modelHeader
//     P14AnimationStatePcN25liif runs on it with: a copy of the point at +16, the halfword at +56, M, the
//     animation state at +288, the words R+0, R+8 and R+16, the lost chunks, 14, 2 and 1.0; then the float at
//     block+452 gets 2.0 added.
// Inferred: the constructor builds a flying body-part effect from the lost chunks. Unknown: what the
// constructor's int arguments (14 and 2) and the float at block+452 are. Nothing here ran on the EE.
#include "../w11-actor-a-common/access.hpp"

extern "C" {
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int mask);
void* blockAlloc__Fi(int size);
void __8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif(void* self, Point3 position, short angle, void* model,
                                                                    void* animation, char* a, char* b, char* c,
                                                                    unsigned long long chunks, int type, int two, float scale);

void loseBodyPart__3Anti(void* self, int part) {
    using namespace w11a;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const r = from_word(word(o, 36));
    const unsigned long long before = modelGetActiveChunks__FP12_modelHeaderi(from_word(word(r, 12)), static_cast<int>(word(o, 244)));
    if (static_cast<unsigned>(part) < 8u) word(o, 244) |= 2u << part;
    unsigned char* const r2 = from_word(word(o, 36));
    const unsigned long long after = modelGetActiveChunks__FP12_modelHeaderi(from_word(word(r2, 12)), static_cast<int>(word(o, 244)));
    dword(o, 552) = after;
    unsigned char* const chunk = static_cast<unsigned char*>(blockAlloc__Fi(616));
    if (chunk != nullptr) {
        unsigned char* const r3 = from_word(word(o, 36));
        __8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif(
            chunk, *reinterpret_cast<Point3*>(o + 16), half(o, 56), from_word(word(r3, 12)), o + 288,
            reinterpret_cast<char*>(from_word(word(r3, 0))), reinterpret_cast<char*>(from_word(word(r3, 8))),
            reinterpret_cast<char*>(from_word(word(r3, 16))), before & ~after, 14, 2, 1.0f);
        real(chunk, 452) += 2.0f;
    }
}
}
