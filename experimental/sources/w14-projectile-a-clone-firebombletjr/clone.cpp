// Observed clone__13FireBombletJr at 0x00128830, 328 bytes.
// Allocates 112 bytes with blockAlloc__Fi. A null allocation returns null and writes nothing.
// Otherwise the steps below run in the order of the image (n is the new object, s the source;
// the ListHead copy constructor zeroes the destination word and does not read the source):
//  - the 10GameObject table pointer (_vt$10GameObject) goes to n+72
//  - the ListHead copy constructor runs for the heads at +0, +4 and +8
//  - source bytes 12..41 are copied
//  - source bytes 44..57 are copied
//  - source bytes 60..71 are copied
//  - the 13FireBombletJr table pointer (_vt$13FireBombletJr) goes to n+72
//  - source bytes 76..93 are copied
//  - source bytes 96..99 are copied
//  - source bytes 104..111 are copied.
// Bytes 94..95 and 100..103 and the table-pointer word aside, nothing else is written. The new object is returned. Copy ranges and call order were measured by running the
// original on a pattern-filled source (table pointer stores are only ordered relative to the
// calls). Inferred: the parent chain from the table pointers. Unknown: the meaning of the fields.
// Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
extern unsigned char vt_10GameObject[] __asm__("_vt$10GameObject");
extern unsigned char vt_13FireBombletJr[] __asm__("_vt$13FireBombletJr");
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__13FireBombletJr(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(112));
    if (n == nullptr) return nullptr;
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_10GameObject);
    ListHead_copy(n, s);
    ListHead_copy(n + 4, s + 4);
    ListHead_copy(n + 8, s + 8);
    copy(n, s, 12, 41);
    copy(n, s, 44, 57);
    copy(n, s, 60, 71);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_13FireBombletJr);
    copy(n, s, 76, 93);
    copy(n, s, 96, 99);
    copy(n, s, 104, 111);
    return n;
}
