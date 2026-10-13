// Observed clone__9WaveMaker at 0x001160e0, 420 bytes.
// Allocates 1896 bytes with blockAlloc__Fi. A null allocation returns null and writes nothing.
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
//  - the 9WaveMaker table pointer (_vt$9WaveMaker) goes to n+72
//  - source bytes 76..1895 are copied.
// Bytes 42..43 and 58..59 are not written. The bytes from 76 to the end are copied by a loop of 50 steps of 36 bytes followed by 20 individually copied tail bytes in the image; the result is a plain copy. The new object is returned. Copy ranges and call order were measured by running the
// original on a pattern-filled source (table pointer stores are only ordered relative to the
// calls). Inferred: the parent chain from the table pointers. Unknown: the meaning of the fields.
// Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
extern unsigned char vt_10GameObject[] __asm__("_vt$10GameObject");
extern unsigned char vt_9WaveMaker[] __asm__("_vt$9WaveMaker");
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__9WaveMaker(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(1896));
    if (n == nullptr) return nullptr;
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_10GameObject);
    ListHead_copy(n, s);
    ListHead_copy(n + 4, s + 4);
    ListHead_copy(n + 8, s + 8);
    copy(n, s, 12, 41);
    copy(n, s, 44, 57);
    copy(n, s, 60, 71);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_9WaveMaker);
    copy(n, s, 76, 1895);
    return n;
}
