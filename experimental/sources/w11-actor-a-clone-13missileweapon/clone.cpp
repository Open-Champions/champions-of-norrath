// Observed clone__13MissileWeapon at 0x00125d90, 980 bytes.
// Allocates 452 bytes with blockAlloc__Fi. A null allocation returns null and writes nothing.
// Otherwise the steps below run in the order of the image (n is the new object, s the source;
// the ListHead copy constructor zeroes the destination word and does not read the source):
//  - the 10GameObject table pointer (_vt$10GameObject) goes to n+72
//  - the ListHead copy constructor runs for the heads at +0, +4 and +8
//  - source bytes 12..41, 44..57 and 60..71 are copied
//  - the 10Projectile table pointer (_vt$10Projectile) goes to n+72
//  - the ListHead copy constructor runs for the head at +76; source byte 80 is copied
//  - the 13MissileWeapon table pointer (_vt$13MissileWeapon) goes to n+72
//  - five 24-byte elements start at +84 (+84, +108, +132, +156, +180; the destructor tears down the same array):
//    for each, element bytes 0..7 are copied, the ListHead copy constructor runs for the head at element+8,
//    and element bytes 12..23 are copied
//  - source bytes 204..205, 208..248, 250..254 and 256..291 are copied
//  - the ListHead copy constructor runs for the head at +292
//  - source bytes 296..427 are copied (the Light member at +368 that the destructor tears down starts inside it)
//  - when either of the words at n+368 and n+372 is nonzero, both are cleared and lightAdd__FP5Light
//    is called with the address n+368
//  - source bytes 428..451 are copied.
// Bytes not named above (42..43, 58..59, 72..75 aside from the table pointers, 81..83, 206..207, 249, 255,
// 292..295 apart from the head) are left as the allocator returned them.
// The new object is returned. Copy ranges and call order were read from the instruction sequence of the
// image and then checked by running it. Inferred: the parent chain from the table pointers and that the
// member copied at the light offset is a Light (from lightAdd's name). Unknown: the meaning of the
// other fields. Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
void lightAdd__FP5Light(void* light);
extern unsigned char vt_10GameObject[] __asm__("_vt$10GameObject");
extern unsigned char vt_10Projectile[] __asm__("_vt$10Projectile");
extern unsigned char vt_13MissileWeapon[] __asm__("_vt$13MissileWeapon");
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__13MissileWeapon(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(452));
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
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_13MissileWeapon);
    copy(n, s, 84, 91);
    ListHead_copy(n + 92, s + 92);
    copy(n, s, 96, 107);
    copy(n, s, 108, 115);
    ListHead_copy(n + 116, s + 116);
    copy(n, s, 120, 131);
    copy(n, s, 132, 139);
    ListHead_copy(n + 140, s + 140);
    copy(n, s, 144, 155);
    copy(n, s, 156, 163);
    ListHead_copy(n + 164, s + 164);
    copy(n, s, 168, 179);
    copy(n, s, 180, 187);
    ListHead_copy(n + 188, s + 188);
    copy(n, s, 192, 203);
    copy(n, s, 204, 205);
    copy(n, s, 208, 248);
    copy(n, s, 250, 254);
    copy(n, s, 256, 291);
    ListHead_copy(n + 292, s + 292);
    copy(n, s, 296, 367);
    copy(n, s, 368, 427);
    unsigned* light = reinterpret_cast<unsigned*>(n + 368);
    if (light[0] != 0 || light[1] != 0) {
        light[0] = 0;
        light[1] = 0;
        lightAdd__FP5Light(light);
    }
    copy(n, s, 428, 451);
    return n;
}
