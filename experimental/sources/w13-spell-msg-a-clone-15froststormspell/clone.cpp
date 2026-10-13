// Observed clone__15FrostStormSpell at 0x00124240, 492 bytes.
// Allocates 172 bytes with blockAlloc__Fi. A null allocation returns null and writes nothing.
// Otherwise the steps below run in the order of the image (n is the new object, s the source;
// the ListHead copy constructor zeroes the destination word and does not read the source):
//  - the 10GameObject table pointer (_vt$10GameObject) goes to n+72
//  - the ListHead copy constructor runs for the heads at +0, +4 and +8
//  - source bytes 12..41, 44..57 and 60..71 are copied
//  - the 15FrostStormSpell table pointer (_vt$15FrostStormSpell) goes to n+72
//  - source bytes 76..107 are copied
//  - the ListHead copy constructor runs for the head at +108
//  - source bytes 112..171 (a 60-byte block, passed on as an object at n+112) are copied; if either of its
//    first two words is non-zero in the copy, both are cleared and lightAdd__FP5Light is called with n+112.
// The new object is returned. Copy ranges and call order were measured by running the original on a
// pattern-filled source and read from its instructions. Inferred: the parent chain from the table pointers,
// and that the block at +112 is a Light because it is the argument of lightAdd__FP5Light. Unknown: the
// meaning of the other fields. Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
void lightAdd__FP5Light(void* light);
extern unsigned char vt_10GameObject[] __asm__("_vt$10GameObject");
extern unsigned char vt_15FrostStormSpell[] __asm__("_vt$15FrostStormSpell");
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__15FrostStormSpell(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(172));
    if (n == nullptr) return nullptr;
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_10GameObject);
    ListHead_copy(n, s);
    ListHead_copy(n + 4, s + 4);
    ListHead_copy(n + 8, s + 8);
    copy(n, s, 12, 41);
    copy(n, s, 44, 57);
    copy(n, s, 60, 71);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_15FrostStormSpell);
    copy(n, s, 76, 107);
    ListHead_copy(n + 108, s + 108);
    copy(n, s, 112, 171);
    unsigned* light = reinterpret_cast<unsigned*>(n + 112);
    if (light[0] != 0 || light[1] != 0) {
        light[0] = 0;
        light[1] = 0;
        lightAdd__FP5Light(n + 112);
    }
    return n;
}
