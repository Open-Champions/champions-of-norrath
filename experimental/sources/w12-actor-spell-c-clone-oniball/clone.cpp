// Observed clone__7OniBall at 0x00125390, 528 bytes.
// Allocates 184 bytes with blockAlloc__Fi. A null allocation returns null and writes nothing.
// Otherwise the steps below run in the order of the image (n is the new object, s the source;
// the ListHead copy constructor zeroes the destination word and does not read the source):
//  - the 10GameObject table pointer (_vt$10GameObject) goes to n+72
//  - the ListHead copy constructor runs for the heads at +0, +4 and +8
//  - source bytes 12..41, 44..57 and 60..71 are copied
//  - the 10Projectile table pointer (_vt$10Projectile) goes to n+72
//  - the ListHead copy constructor runs for the head at +76
//  - source byte 80 is copied
//  - the 7OniBall table pointer (_vt$7OniBall) goes to n+72
//  - source bytes 84..116 are copied (117..119 are not touched)
//  - source bytes 120..179 (a 60-byte block, passed on as an object at n+120) are copied; if either of its
//    first two words is non-zero in the copy, both are cleared and lightAdd__FP5Light is called with n+120
//  - source bytes 180..183 are copied.
// The new object is returned. Copy ranges and call order were measured by running the original on a
// pattern-filled source and read from its instructions. Inferred: the parent chain from the table pointers,
// and that the block at +120 is a Light because it is the argument of lightAdd__FP5Light. Unknown: the
// meaning of the other fields. Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
void lightAdd__FP5Light(void* light);
extern unsigned char vt_10GameObject[] __asm__("_vt$10GameObject");
extern unsigned char vt_10Projectile[] __asm__("_vt$10Projectile");
extern unsigned char vt_7OniBall[] __asm__("_vt$7OniBall");
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__7OniBall(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(184));
    if (n == nullptr) return nullptr;
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_10GameObject);
    ListHead_copy(n, s);
    ListHead_copy(n + 4, s + 4);
    ListHead_copy(n + 8, s + 8);
    copy(n, s, 12, 41);
    copy(n, s, 44, 57);
    copy(n, s, 60, 71);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_10Projectile);
    ListHead_copy(n + 76, s + 76);
    copy(n, s, 80, 80);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_7OniBall);
    copy(n, s, 84, 116);
    copy(n, s, 120, 179);
    unsigned* light = reinterpret_cast<unsigned*>(n + 120);
    if (light[0] != 0 || light[1] != 0) {
        light[0] = 0;
        light[1] = 0;
        lightAdd__FP5Light(n + 120);
    }
    copy(n, s, 180, 183);
    return n;
}
