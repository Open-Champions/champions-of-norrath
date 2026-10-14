// Observed clone__11FireBomblet at 0x00128610, 540 bytes.
// Allocates 184 bytes with blockAlloc__Fi. A null allocation returns null and writes nothing.
// Otherwise the steps below run in the order of the image (n is the new object, s the source;
// the ListHead copy constructor zeroes the destination word and does not read the source):
//  - the 10GameObject table pointer (_vt$10GameObject) goes to n+72
//  - the ListHead copy constructor runs for the heads at +0, +4 and +8
//  - source bytes 12..41, 44..57 and 60..71 are copied
//  - the 11FireBomblet table pointer (_vt$11FireBomblet) goes to n+72
//  - source bytes 76..147 are copied (the Light member at +88 that the destructor tears down starts inside it)
//  - when either of the words at n+88 and n+92 is nonzero, both are cleared and lightAdd__FP5Light
//    is called with the address n+88
//  - source bytes 148..157 and 160..183 are copied.
// Bytes not named above (42..43, 58..59, 72..75 aside from the table pointer, 158..159) are left as the
// allocator returned them.
// The new object is returned. Copy ranges and call order were read from the instruction sequence of the
// image and then checked by running it. Inferred: the parent chain from the table pointers and that the
// member copied at the light offset is a Light (from lightAdd's name). Unknown: the meaning of the
// other fields. Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
void lightAdd__FP5Light(void* light);
extern unsigned char vt_10GameObject[] __asm__("_vt$10GameObject");
extern unsigned char vt_11FireBomblet[] __asm__("_vt$11FireBomblet");
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__11FireBomblet(void* source) {
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
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_11FireBomblet);
    copy(n, s, 76, 147);
    unsigned* light = reinterpret_cast<unsigned*>(n + 88);
    if (light[0] != 0 || light[1] != 0) {
        light[0] = 0;
        light[1] = 0;
        lightAdd__FP5Light(light);
    }
    copy(n, s, 148, 157);
    copy(n, s, 160, 183);
    return n;
}
