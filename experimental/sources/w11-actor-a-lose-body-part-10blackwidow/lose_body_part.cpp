// Observed loseBodyPart__10BlackWidowi at 0x0020b118, 468 bytes. Let R be the block at the word object+36 and
// M its word at +12. In order:
//  1. before = modelGetActiveChunks(M, word +244); a flag "spawn" starts at 1.
//  2. For part < 12 (unsigned) the bit 1 << part is set in the word at +244; for parts 8..11 spawn becomes 0.
//     Larger parts change nothing and keep spawn at 1.
//  3. The 64-bit result of modelGetActiveChunks(M, word +244) is stored at +552; lost = before & ~after.
//  4. Only when spawn is set: type is 14, or 270 when bit 24 (0x01000000) of the 64-bit word at +160 is set;
//     blockAlloc(616); when a block comes back the GutChunk constructor
//     __8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif runs on it with: a copy of the point at
//     +16, the halfword at +56, M, the animation state at +288, the words R+0, R+8 and R+16, lost, type, 2 and
//     the float at +224.
// Unlike the Anti and FireBeetle versions it does not add to the float at block+452. Inferred: parts 8..11
// are bits that do not produce a chunk effect. Unknown: what bit 24 of +160 and the float at +224 are.
// Nothing here ran on the EE.
#include "../w11-actor-a-common/access.hpp"

extern "C" {
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int mask);
void* blockAlloc__Fi(int size);
void __8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif(void* self, Point3 position, short angle, void* model,
                                                                    void* animation, char* a, char* b, char* c,
                                                                    unsigned long long chunks, int type, int two, float scale);

void loseBodyPart__10BlackWidowi(void* self, int part) {
    using namespace w11a;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const r = from_word(word(o, 36));
    const unsigned long long before = modelGetActiveChunks__FP12_modelHeaderi(from_word(word(r, 12)), static_cast<int>(word(o, 244)));
    bool spawn = true;
    if (static_cast<unsigned>(part) < 12u) {
        word(o, 244) |= 1u << part;
        if (part >= 8) spawn = false;
    }
    unsigned char* const r2 = from_word(word(o, 36));
    const unsigned long long after = modelGetActiveChunks__FP12_modelHeaderi(from_word(word(r2, 12)), static_cast<int>(word(o, 244)));
    dword(o, 552) = after;
    if (spawn) {
        const int type = (dword(o, 160) & 0x01000000ull) != 0 ? 270 : 14;
        unsigned char* const chunk = static_cast<unsigned char*>(blockAlloc__Fi(616));
        if (chunk != nullptr) {
            unsigned char* const r3 = from_word(word(o, 36));
            __8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif(
                chunk, *reinterpret_cast<Point3*>(o + 16), half(o, 56), from_word(word(r3, 12)), o + 288,
                reinterpret_cast<char*>(from_word(word(r3, 0))), reinterpret_cast<char*>(from_word(word(r3, 8))),
                reinterpret_cast<char*>(from_word(word(r3, 16))), before & ~after, type, 2, real(o, 224));
        }
    }
}
}
