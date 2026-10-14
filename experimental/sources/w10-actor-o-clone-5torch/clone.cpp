// Observed clone__5Torch at 0x00122ea8, 440 bytes.
// Allocates 140 bytes with blockAlloc__Fi. A null allocation returns null and writes nothing.
// Otherwise the steps below run in the order of the image (n is the new object, s the source;
// the ListHead copy constructor zeroes the destination word and does not read the source):
//  - the 10GameObject table pointer (_vt$10GameObject) goes to n+72
//  - the ListHead copy constructor runs for the heads at +0, +4 and +8
//  - source bytes 12..41, 44..57 and 60..71 are copied
//  - the 5Torch table pointer (_vt$5Torch) goes to n+72
//  - source bytes 76..135 are copied (the Light member that the destructor tears down at +76)
//  - when either of the words at n+76 and n+80 is nonzero, both are cleared and lightAdd__FP5Light
//    is called with the address n+76
//  - source bytes 136, 137 and 138 are copied.
// Bytes of the new object not named above (42..43, 58..59, 72..75 aside from the table pointer,
// 139) are left as the allocator returned them. The new object is returned. Copy ranges were read
// from the instruction sequence of the image and then checked by running it. Inferred: the parent
// chain from the table pointers and that the +76 member is a Light. Unknown: the meaning of the
// other fields. Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
void lightAdd__FP5Light(void* light);
extern unsigned char vt_10GameObject[] __asm__("_vt$10GameObject");
extern unsigned char vt_5Torch[] __asm__("_vt$5Torch");
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__5Torch(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(140));
    if (n == nullptr) return nullptr;
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_10GameObject);
    ListHead_copy(n, s);
    ListHead_copy(n + 4, s + 4);
    ListHead_copy(n + 8, s + 8);
    copy(n, s, 12, 41);
    copy(n, s, 44, 57);
    copy(n, s, 60, 71);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_5Torch);
    copy(n, s, 76, 135);
    unsigned* light = reinterpret_cast<unsigned*>(n + 76);
    if (light[0] != 0 || light[1] != 0) {
        light[0] = 0;
        light[1] = 0;
        lightAdd__FP5Light(light);
    }
    copy(n, s, 136, 138);
    return n;
}
