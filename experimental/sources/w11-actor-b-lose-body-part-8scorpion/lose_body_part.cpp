// Observed loseBodyPart__8Scorpioni at 0x001f2e00, 260 bytes. part is the second argument. Steps, in order:
//   1. before = modelGetActiveChunks__FP12_modelHeaderi(M, word at +244), where M is the word at 12 of the
//      record pointed to by the word at +36;
//   2. the word at +244 gets bit `part` set (1 << part);
//   3. after = modelGetActiveChunks(M, new +244) and the doubleword at +552 is set to it;
//   4. gone = before & ~after (64-bit);
//   5. a 616-byte block is allocated with blockAlloc__Fi; if that succeeded it is constructed as a
//      GutChunk (name __8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif) with: a stack copy of
//      the position at +16, the halfword at +56, M, the animation state at +288, the three words at 0, 8
//      and 16 of the record at +36, `gone`, 14, 2 and the float 1.0; then the float at the block's +452
//      grows by 2.0.
// Returns nothing. Inferred: the chunk mask is the set of model pieces that disappear when the part is
// shed, and the new object is a flying gib; both are readings of the names. Unknown: what each of the
// three record words is (the constructor takes them as strings). Nothing here ran on the EE.
#include "../w11-actor-b-common/access.hpp"

extern "C" {
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int selected);
void* blockAlloc__Fi(int size);
void __8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif(void* self, Point3 position, short angle, void* model, void* animation,
                                                                    char* a, char* b, char* c, long long chunks, int d, int e, float scale);

void loseBodyPart__8Scorpioni(void* self, int part) {
    using namespace w11b;
    unsigned char* o = static_cast<unsigned char*>(self);
    unsigned char* record = from_word(word(o, 36));
    void* model = from_word(word(record, 12));
    const unsigned long long before = modelGetActiveChunks__FP12_modelHeaderi(model, sword(o, 244));
    const unsigned mask = word(o, 244) | (1u << part);
    word(o, 244) = mask;
    const unsigned long long after = modelGetActiveChunks__FP12_modelHeaderi(from_word(word(from_word(word(o, 36)), 12)), mask);
    dword(o, 552) = after;
    const unsigned long long gone = before & ~after;
    unsigned char* chunk = static_cast<unsigned char*>(blockAlloc__Fi(616));
    if (!chunk) return;
    unsigned char* r = from_word(word(o, 36));
    Point3 position = *reinterpret_cast<Point3*>(o + 16);
    __8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif(
        chunk, position, half(o, 56), from_word(word(r, 12)), o + 288,
        reinterpret_cast<char*>(from_word(word(r, 0))), reinterpret_cast<char*>(from_word(word(r, 8))),
        reinterpret_cast<char*>(from_word(word(r, 16))), static_cast<long long>(gone), 14, 2, 1.0f);
    real(chunk, 452) = real(chunk, 452) + 2.0f;
}
}
