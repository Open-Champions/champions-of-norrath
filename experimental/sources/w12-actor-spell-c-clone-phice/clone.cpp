// Observed clone__5PhIce at 0x00114f70, 1012 bytes.
// Allocates 592 bytes with blockAlloc__Fi. A null allocation returns null and writes nothing.
// Otherwise the steps below run in the order of the image (n is the new object, s the source;
// the ListHead copy constructor zeroes the destination word and does not read the source, the
// AnimationState8 copy constructor copies 264 bytes):
//  - the 10GameObject table pointer (_vt$10GameObject) goes to n+72
//  - the ListHead copy constructor runs for the head at +0
//  - the ListHead copy constructor runs for the head at +4
//  - the ListHead copy constructor runs for the head at +8
//  - source bytes 12..41 are copied
//  - source bytes 44..57 are copied
//  - source bytes 60..71 are copied
//  - source bytes 76..227 are copied
//  - source bytes 232..321 are copied
//  - source bytes 328..570 are copied
//  - source bytes 572..579 are copied
//  - source bytes 584..591 are copied
//  - the 5PhIce table pointer (_vt$5PhIce) goes to n+72.
// The new object is returned. Copy ranges and call order were measured by running the
// original on a pattern-filled source (table pointer stores are only ordered relative to the
// calls). Inferred: the parent chain from the table pointers. Unknown: the meaning of the fields.
// Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
extern unsigned char vt_10GameObject[] __asm__("_vt$10GameObject");
extern unsigned char vt_5PhIce[] __asm__("_vt$5PhIce");
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__5PhIce(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(592));
    if (n == nullptr) return nullptr;
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_10GameObject);
    ListHead_copy(n, s);
    ListHead_copy(n + 4, s + 4);
    ListHead_copy(n + 8, s + 8);
    copy(n, s, 12, 41);
    copy(n, s, 44, 57);
    copy(n, s, 60, 71);
    copy(n, s, 76, 227);
    copy(n, s, 232, 321);
    copy(n, s, 328, 570);
    copy(n, s, 572, 579);
    copy(n, s, 584, 591);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_5PhIce);
    return n;
}
