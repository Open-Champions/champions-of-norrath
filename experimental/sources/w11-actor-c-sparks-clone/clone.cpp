// Observed clone__6Sparks at 0x00124f60, 528 bytes.
// Allocates 184 bytes with blockAlloc__Fi. A null allocation returns null and writes nothing.
// Otherwise, in the order of the image (n is the new object, s the source):
//  - the GameObject table pointer (_vt$10GameObject) goes to n+72, then the ListHead copy
//    constructor runs for the heads at +0, +4 and +8 (it zeroes the destination word and does
//    not read the source);
//  - source bytes 12..41, 44..57 and 60..71 are copied; the Projectile table pointer
//    (_vt$10Projectile) replaces n+72; the ListHead copy constructor runs for the head at +76;
//  - the Sparks table pointer (_vt$6Sparks) replaces n+72; source bytes 80, 84..87, 88..99 and
//    100..159 are copied;
//  - the 60 bytes at +100..159 are a member of the type Light by the same pattern as the StaticFire
//    clone: when either of its first two words is non-zero in the copy, both are cleared and
//    lightAdd__FP5Light is called with the member's address;
//  - source bytes 160..183 are copied (six words).
// Bytes of the new object not named above are left as the allocator returned them (42..43, 58..59
// and 81..83). The new object is returned. Copy ranges and call order were read from the image and
// confirmed by running the original on a pattern-filled source. Inferred: the parent chain from the
// table pointers and that the member at +100 is a light. Unknown: the meaning of the other fields.
// Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
void lightAdd__FP5Light(void* light);
extern unsigned char vt_GameObject[] __asm__("_vt$10GameObject");
extern unsigned char vt_Projectile[] __asm__("_vt$10Projectile");
extern unsigned char vt_Sparks[] __asm__("_vt$6Sparks");
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__6Sparks(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(184));
    if (n == nullptr) return nullptr;
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_GameObject);
    ListHead_copy(n, s);
    ListHead_copy(n + 4, s + 4);
    ListHead_copy(n + 8, s + 8);
    copy(n, s, 12, 41);
    copy(n, s, 44, 57);
    copy(n, s, 60, 71);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_Projectile);
    ListHead_copy(n + 76, s + 76);
    copy(n, s, 80, 80);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_Sparks);
    copy(n, s, 84, 87);
    copy(n, s, 88, 99);
    copy(n, s, 100, 159);
    unsigned* light = reinterpret_cast<unsigned*>(n + 100);
    if (light[0] != 0 || light[1] != 0) {
        light[0] = 0;
        light[1] = 0;
        lightAdd__FP5Light(light);
    }
    copy(n, s, 160, 183);
    return n;
}
