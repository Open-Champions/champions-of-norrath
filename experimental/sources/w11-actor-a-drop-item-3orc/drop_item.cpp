// Observed dropItem__3Orci at 0x00229858, 964 bytes. Arguments: the object o and an int kind. Nothing is
// returned. Let bits be the word at o+244 (kept as old) and group the block named by the word at o+36.
// Locals start as: mask 0 (64-bit), flags 0, index 0 and selector -1.
//  1. kind 1: bits |= 0x10000000; selector 8; mask starts at 0x60406 when old has bit 0x1, else 0x20406; then for
//     each set bit of old: 0x2 -> mask ^= 2, mask |= 0x80000; 0x4 -> mask ^= 6, |= 0x300000; 0x8 -> ^= 6,
//     |= 0xc00000; 0x10 -> ^= 6, |= 0x3000000; 0x20 -> ^= 6, |= 0xc000000; 0x40 -> ^= 6, |= 0x30000000;
//     0x100 -> |= bit 31; 0x200 -> |= bit 32; 0x400 -> |= bit 33; 0x800 -> |= bit 34.
//     kind 2: bits |= 0x08000000, mask 0x800, selector 6.   kind 3: bits |= 0x04000000, mask 0x10, selector 7.
//     kind 4: bits |= 0x02000000, mask 0x1000, selector 4.  kind 5: bits |= 0x01000000, mask 0x200, selector 5.
//     kind 6: index is the word at o+972, which is cleared; selector 1 and flags 8192 when index is 2, else
//     selector 0 and flags 16384; flags |= 0x100.
//     kind 7: index is the word at o+1112, which is cleared; flags 16384; selector 3.
//     Other kinds change nothing here.
//  2. A Point3 first is a copy of the position at o+16. When selector is not -1,
//     modelGetCharAttachmentPos(model = word +12 of group, a copy of the position, (halfword at +56) - 16385
//     truncated to a short, the animation state at o+288, selector, &first, &second, 1.0) is called.
//  3. Kinds 1 to 5: blockAlloc(616). When it succeeds the GutChunk constructor runs on it with a copy of the
//     position, the halfword at +56, the model at group+12, o+288, the group words +0, +8, +16, the 64-bit
//     mask, the int 7, the int 0 and 1.0; then the float at block+452 gets 2.0 added.
//     Other kinds, when index is nonzero: flags |= 1, blockAlloc(584), and when that succeeds the PhProp
//     constructor (three-name form) runs with first, T[0], T[8*index], T[8*index+136] (T is the word at
//     o+1136; each read is a word), the long 1, flags, a null description and 1.0.
//  4. The word at o+244 gets bits, and the 64-bit word at o+552 gets modelGetActiveChunks(model at group+12, bits).
// Inferred: the GutChunk is a body-part pickup; the bits are per-part flags. Unknown: the meaning of the bits,
// the masks and the selector values, and what the tables at o+1136 contain. Nothing here ran on the EE.
#include "../w11-actor-a-common/access.hpp"

extern "C" {
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, Point3 position, short a, void* animation, int b, Point3* first, Point3* second, float f);
void* blockAlloc__Fi(int size);
void __8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif(void* self, Point3 position, short angle, void* model, void* animation,
                                                                    char* a, char* b, char* c, unsigned long long chunks, int type, int two,
                                                                    float scale);
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, Point3 position, char* a, char* b, char* c, long n, int flags,
                                                    void* description, float f);
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int bits);

void dropItem__3Orci(void* self, int kind) {
    using namespace w11a;
    unsigned char* const o = static_cast<unsigned char*>(self);
    const unsigned old = word(o, 244);
    unsigned bits = old;
    unsigned long long mask = 0;
    int flags = 0;
    unsigned index = 0;
    int selector = -1;
    switch (kind) {
    case 1:
        bits |= 0x10000000u;
        mask = (old & 1u) ? 0x60406ull : 0x20406ull;
        if (old & 2u) { mask ^= 2; mask |= 0x80000; }
        if (old & 4u) { mask ^= 6; mask |= 0x300000; }
        if (old & 8u) { mask ^= 6; mask |= 0xc00000; }
        if (old & 0x10u) { mask ^= 6; mask |= 0x3000000; }
        if (old & 0x20u) { mask ^= 6; mask |= 0xc000000; }
        if (old & 0x40u) { mask ^= 6; mask |= 0x30000000; }
        if (old & 0x100u) mask |= 0x8000ull << 16;
        if (old & 0x200u) mask |= 0x8000ull << 17;
        if (old & 0x400u) mask |= 0x8000ull << 18;
        if (old & 0x800u) mask |= 0x8000ull << 19;
        selector = 8;
        break;
    case 2: bits |= 0x08000000u; mask = 0x800; selector = 6; break;
    case 3: bits |= 0x04000000u; mask = 0x10; selector = 7; break;
    case 4: bits |= 0x02000000u; mask = 0x1000; selector = 4; break;
    case 5: bits |= 0x01000000u; mask = 0x200; selector = 5; break;
    case 6:
        index = word(o, 972);
        selector = 0;
        word(o, 972) = 0;
        if (index == 2) { flags = 8192; selector = 1; } else flags = 16384;
        flags |= 0x100;
        break;
    case 7:
        index = word(o, 1112);
        flags = 16384;
        word(o, 1112) = 0;
        selector = 3;
        break;
    default:
        break;
    }
    Point3 first = *reinterpret_cast<Point3*>(o + 16);
    Point3 second;
    if (selector != -1)
        modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
            from_word(word(from_word(word(o, 36)), 12)), *reinterpret_cast<Point3*>(o + 16),
            static_cast<short>(static_cast<unsigned short>(half(o, 56)) - 16385), o + 288, selector, &first, &second, 1.0f);
    if (kind >= 1 && kind <= 5) {
        void* const block = blockAlloc__Fi(616);
        if (block != nullptr) {
            unsigned char* const group = from_word(word(o, 36));
            __8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif(
                block, *reinterpret_cast<Point3*>(o + 16), half(o, 56), from_word(word(group, 12)), o + 288,
                reinterpret_cast<char*>(from_word(word(group, 0))), reinterpret_cast<char*>(from_word(word(group, 8))),
                reinterpret_cast<char*>(from_word(word(group, 16))), mask, 7, 0, 1.0f);
            real(static_cast<unsigned char*>(block), 452) += 2.0f;
        }
    } else if (index != 0) {
        flags |= 1;
        void* const block = blockAlloc__Fi(584);
        if (block != nullptr) {
            unsigned char* const table = from_word(word(o, 1136));
            unsigned char* const entry = table + index * 8u;
            __6PhPropG6Point3PcN22liPQ24item11descriptionf(
                block, first, reinterpret_cast<char*>(from_word(word(table, 0))), reinterpret_cast<char*>(from_word(word(entry, 0))),
                reinterpret_cast<char*>(from_word(word(entry, 136))), 1, flags, nullptr, 1.0f);
        }
    }
    word(o, 244) = bits;
    dword(o, 552) = modelGetActiveChunks__FP12_modelHeaderi(from_word(word(from_word(word(o, 36)), 12)), static_cast<int>(bits));
}
}
