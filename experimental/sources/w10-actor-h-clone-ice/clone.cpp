// Observed clone__3Ice at 0x00115648, 312 bytes.
// Allocates 104 bytes with blockAlloc__Fi. A null allocation returns null and writes nothing.
// Otherwise, in the order of the image (n is the new object, s the source): the GameObject table
// pointer (_vt$10GameObject) goes to n+72, the ListHead copy constructor runs for the heads at +0,
// +4 and +8 (it zeroes the destination word and does not read the source), source bytes 12..41,
// 44..57 and 60..71 are copied, the Ice table pointer (_vt$3Ice) replaces n+72, and source bytes
// 76..103 are copied. Bytes 42..43 and 58..59 are left as the allocator returned them. The new
// object is returned.
// Copy ranges were measured by running the original on a pattern-filled source; the field
// boundaries inside each range are not established. Inferred: Ice derives from GameObject (its
// table pointers). Unknown: the meaning of the fields. Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
extern unsigned char vt_GameObject[] __asm__("_vt$10GameObject");
extern unsigned char vt_Ice[] __asm__("_vt$3Ice");
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__3Ice(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(104));
    if (n == nullptr) return nullptr;
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_GameObject);
    ListHead_copy(n, s);
    ListHead_copy(n + 4, s + 4);
    ListHead_copy(n + 8, s + 8);
    copy(n, s, 12, 41);
    copy(n, s, 44, 57);
    copy(n, s, 60, 71);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_Ice);
    copy(n, s, 76, 103);
    return n;
}
