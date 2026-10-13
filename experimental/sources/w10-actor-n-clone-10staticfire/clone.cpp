// Observed clone__10StaticFire at 0x00128978, 636 bytes.
// Allocates 1192 bytes with blockAlloc__Fi. A null allocation returns null and writes nothing.
// Otherwise, in the order of the image (n is the new object, s the source):
//  - the GameObject table pointer (_vt$10GameObject) goes to n+72, then the ListHead copy
//    constructor runs for the heads at +0, +4 and +8 (it zeroes the destination word and does
//    not read the source);
//  - the StaticFire table pointer (_vt$10StaticFire) replaces n+72 (only ordered relative to
//    the calls); source bytes 12..41, 44..57, 60..71, 76..112, 114..127 and 128..187 are copied;
//  - the 60 bytes at +128..187 are the member whose destructor flag-2 call _$_10StaticFire makes
//    (_$_5Light); when either of its first two words is non-zero in the copy, both are cleared and
//    lightAdd__FP5Light is called with the member's address;
//  - source bytes 188..1187 are copied one at a time, then the word at 1188.
// Bytes of the new object not named above are left as the allocator returned them (42..43,
// 58..59 and 113). The new object is returned. Copy ranges and call order were measured by running
// the original on a pattern-filled source. Inferred: that the member at +128 is a light (from the
// destructor call and the lightAdd argument type). Unknown: the meaning of the other fields.
// Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
void lightAdd__FP5Light(void* light);
extern unsigned char vt_GameObject[] __asm__("_vt$10GameObject");
extern unsigned char vt_StaticFire[] __asm__("_vt$10StaticFire");
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__10StaticFire(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(1192));
    if (n == nullptr) return nullptr;
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_GameObject);
    ListHead_copy(n, s);
    ListHead_copy(n + 4, s + 4);
    ListHead_copy(n + 8, s + 8);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_StaticFire);
    copy(n, s, 12, 41);
    copy(n, s, 44, 57);
    copy(n, s, 60, 71);
    copy(n, s, 76, 112);
    copy(n, s, 114, 127);
    copy(n, s, 128, 187);
    unsigned* light = reinterpret_cast<unsigned*>(n + 128);
    if (light[0] != 0 || light[1] != 0) {
        light[0] = 0;
        light[1] = 0;
        lightAdd__FP5Light(light);
    }
    copy(n, s, 188, 1187);
    copy(n, s, 1188, 1191);
    return n;
}
