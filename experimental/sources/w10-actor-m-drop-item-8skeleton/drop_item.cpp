#include "../w10-actor-m-common/access.hpp"
// Observed dropItem__8Skeletoni at 0x245938, 976 bytes. Arguments: the object and an int kind. Nothing is
// returned. The object's 64-bit word at +552 holds bits that select things to drop.
//  1. A zero-terminated list of 64-bit masks (the file-local data object partlist.1069, 0x7d7f70, 160 bytes
//     in the image; here a local static) is filled by kind. Its first entry is always cleared first.
//     kind 1: 0xfe00<<40, 192.   kind 2: 0x80000, 0x100000, 0x200000, 0x10000010<<32.
//     kind 3: 0x10000, 0x20000, 0x40000, 0x20000020<<32.   kind 4: 2, 16384, 0x8000.
//     kind 5: 1, 4096, 8192.     kind 8: 192, 824, 3076, 0x10000, 0x20000, 0x80000, 0x100000, 1, 2, 4096,
//     16384, 0x40000, 0x200000, 0x8000, 8192, 0xfe00<<40.   Other kinds (including 6 and 7) leave the list empty.
//     (jump table at 0x474130, kinds 1 to 8.)
//  2. For each list entry m with (bits & m) != 0: an object of 584 bytes is allocated with blockAlloc__Fi; when it
//     succeeds, the PhProp constructor (symbol __6PhProp...Point3sP12_modelHeader...) runs on it with the
//     object's position (floats at +16..+27, copied), the halfword at +56, the model at table+12 (the table is
//     the word at +36), the animation state at +288, the three table words at +0, +8 and +16, the masked
//     bits (stored as a 64-bit stack value), 263, a null description and 1.0f. Whether or not the
//     allocation succeeded, m is cleared from the local copy of the bits.
//  3. The cleared bits are stored back at +552.
//  4. With w = the word at +976: when w is nonzero and (kind is 6 or bit 0x40000 of the bits is clear), or
//     when w is in 83..97 and bit 0x200000 is clear, one more object is made with the second PhProp
//     constructor (symbol __6PhProp...Point3PcN22...) at the position (x, y, z + 36.0f), the words
//     skeletonResMap[0], skeletonResMap[2w] and skeletonResMap[2w+2], 1, 16643, a null description and
//     1.0f; then the word at +976 is cleared whether or not the allocation worked.
//  5. With j = the word at +1120: nothing when j is zero; otherwise, when kind is 7 or bit 0x200000 of the word
//     at +552 is clear, the same second-constructor drop with j instead of w, then the word at +1120 is cleared.
// Inferred: PhProp is a physics prop, from the symbol. Unknown: what the bits and masks mean, what kinds 1 to 8
// are, what the table words are, and what the resource map pairs are.
extern "C" {
extern unsigned char skeletonResMap[];
void* blockAlloc__Fi(int size);
void PhPropWithModel(void* self, Point3 position, short angle, void* model, void* animation, char* a, char* b, char* c,
                             unsigned long long bits, int flag, void* description, float scale)
    __asm__("__6PhPropG6Point3sP12_modelHeaderP14AnimationStatePcN25liPQ24item11descriptionf");
void PhPropWithNames(void* self, Point3 position, char* a, char* b, char* c, long bits, int flag, void* description, float scale)
    __asm__("__6PhPropG6Point3PcN22liPQ24item11descriptionf");
}

namespace {
unsigned long long partlist[17];

inline void spawn_map_prop(unsigned char* o, unsigned index) {
    Point3 position;
    position.x = w10m::real(o, 16);
    position.y = w10m::real(o, 20);
    position.z = w10m::real(o, 24) + 36.0f;
    void* made = blockAlloc__Fi(584);
    if (made != nullptr) {
        const unsigned offset = index << 3;
        PhPropWithNames(made, position, reinterpret_cast<char*>(w10m::from_word(w10m::word(skeletonResMap, 0))),
                                reinterpret_cast<char*>(w10m::from_word(w10m::word(skeletonResMap, offset))),
                                reinterpret_cast<char*>(w10m::from_word(w10m::word(skeletonResMap, offset + 8u))), 1, 16643, nullptr,
                                1.0f);
    }
}
}  // namespace

extern "C" void dropItem__8Skeletoni(void* self, int kind) {
    unsigned char* o = static_cast<unsigned char*>(self);
    partlist[0] = 0;
    switch (kind) {
    case 1:
        partlist[0] = 0xfe00ull << 40; partlist[1] = 192; partlist[2] = 0;
        break;
    case 2:
        partlist[0] = 0x80000; partlist[1] = 0x100000; partlist[2] = 0x200000; partlist[3] = 0x10000010ull << 32; partlist[4] = 0;
        break;
    case 3:
        partlist[0] = 0x10000; partlist[1] = 0x20000; partlist[2] = 0x40000; partlist[3] = 0x20000020ull << 32; partlist[4] = 0;
        break;
    case 4:
        partlist[0] = 2; partlist[1] = 16384; partlist[2] = 0x8000; partlist[3] = 0;
        break;
    case 5:
        partlist[0] = 1; partlist[1] = 4096; partlist[2] = 8192; partlist[3] = 0;
        break;
    case 8:
        partlist[0] = 192; partlist[1] = 824; partlist[2] = 3076; partlist[3] = 0x10000; partlist[4] = 0x20000;
        partlist[5] = 0x80000; partlist[6] = 0x100000; partlist[7] = 1; partlist[8] = 2; partlist[9] = 4096;
        partlist[10] = 16384; partlist[11] = 0x40000; partlist[12] = 0x200000; partlist[13] = 0x8000; partlist[14] = 8192;
        partlist[15] = 0xfe00ull << 40; partlist[16] = 0;
        break;
    default:
        break;
    }
    unsigned long long bits = w10m::dword(o, 552);
    if (partlist[0] != 0) {
        const unsigned long long* entry = partlist;
        do {
            const unsigned long long mask = *entry;
            const unsigned long long hit = bits & mask;
            if (hit != 0) {
                void* made = blockAlloc__Fi(584);
                if (made != nullptr) {
                    unsigned char* table = w10m::from_word(w10m::word(o, 36));
                    PhPropWithModel(made, *reinterpret_cast<Point3*>(o + 16), w10m::half(o, 56), w10m::from_word(w10m::word(table, 12)), o + 288,
                                            reinterpret_cast<char*>(w10m::from_word(w10m::word(table, 0))),
                                            reinterpret_cast<char*>(w10m::from_word(w10m::word(table, 8))),
                                            reinterpret_cast<char*>(w10m::from_word(w10m::word(table, 16))), hit, 263, nullptr, 1.0f);
                }
                bits &= ~mask;
            }
            ++entry;
        } while (*entry != 0);
    }
    w10m::dword(o, 552) = bits;
    const unsigned w = w10m::word(o, 976);
    if ((w != 0 && (kind == 6 || (bits & 0x40000ull) == 0)) || (w - 83u < 15u && (bits & 0x200000ull) == 0)) {
        spawn_map_prop(o, w);
        w10m::word(o, 976) = 0;
    }
    if (w10m::word(o, 1120) != 0 && (kind == 7 || (w10m::dword(o, 552) & 0x200000ull) == 0)) {
        spawn_map_prop(o, w10m::word(o, 1120));
        w10m::word(o, 1120) = 0;
    }
}
