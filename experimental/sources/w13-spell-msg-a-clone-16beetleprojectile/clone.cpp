// Observed clone__16beetleProjectile at 0x001266d0, 1008 bytes.
// Allocates 460 bytes with blockAlloc__Fi. A null allocation returns null and writes nothing.
// Otherwise the steps below run in the order of the image (n is the new object, s the source;
// the ListHead copy constructor zeroes the destination word and does not read the source):
//  - the 10GameObject table pointer (_vt$10GameObject) goes to n+72
//  - the ListHead copy constructor runs for the heads at +0, +4 and +8
//  - source bytes 12..41, 44..57 and 60..71 are copied
//  - the 10Projectile table pointer (_vt$10Projectile) goes to n+72
//  - the ListHead copy constructor runs for the head at +76
//  - source byte 80 is copied
//  - the 13MissileWeapon table pointer (_vt$13MissileWeapon) goes to n+72
//  - five 24-byte records at +84, +108, +132, +156 and +180 are each handled the same way: the first
//    8 bytes are copied, the ListHead copy constructor runs for the head at record+8, then record
//    bytes 12..23 are copied
//  - source bytes 204..205, 208..247, 248, 250..254 and 256..291 are copied (206..207, 249 and 255
//    are not touched)
//  - the ListHead copy constructor runs for the head at +292
//  - source bytes 296..367 are copied
//  - source bytes 368..427 (a 60-byte block, passed on as an object at n+368) are copied; if either of its
//    first two words is non-zero in the copy, both are cleared and lightAdd__FP5Light is called with n+368
//  - source bytes 428..451 are copied
//  - the 16beetleProjectile table pointer (_vt$16beetleProjectile) goes to n+72
//  - the ListHead copy constructor runs for the head at +452
//  - source bytes 456..459 are copied.
// The new object is returned. Copy ranges and call order were measured by running the original on a
// pattern-filled source and read from its instructions. Inferred: the parent chain from the table pointers,
// that the block at +368 is a Light because it is the argument of lightAdd__FP5Light, and that the five
// records are an array (they are produced by one loop). Unknown: the meaning of the fields.
// Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
void lightAdd__FP5Light(void* light);
extern unsigned char vt_10GameObject[] __asm__("_vt$10GameObject");
extern unsigned char vt_10Projectile[] __asm__("_vt$10Projectile");
extern unsigned char vt_13MissileWeapon[] __asm__("_vt$13MissileWeapon");
extern unsigned char vt_16beetleProjectile[] __asm__("_vt$16beetleProjectile");
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__16beetleProjectile(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(460));
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
    for (unsigned record = 84; record != 204; record += 24) {
        copy(n, s, record, record + 7);
        ListHead_copy(n + record + 8, s + record + 8);
        copy(n, s, record + 12, record + 23);
    }
    copy(n, s, 204, 205);
    copy(n, s, 208, 248);
    copy(n, s, 250, 254);
    copy(n, s, 256, 291);
    ListHead_copy(n + 292, s + 292);
    copy(n, s, 296, 427);
    unsigned* light = reinterpret_cast<unsigned*>(n + 368);
    if (light[0] != 0 || light[1] != 0) {
        light[0] = 0;
        light[1] = 0;
        lightAdd__FP5Light(n + 368);
    }
    copy(n, s, 428, 451);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_16beetleProjectile);
    ListHead_copy(n + 452, s + 452);
    copy(n, s, 456, 459);
    return n;
}
