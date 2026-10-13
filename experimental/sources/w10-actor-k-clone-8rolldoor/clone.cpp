// Observed clone__8RollDoor at 0x001277b8, 304 bytes.
// Allocates 92 bytes with blockAlloc__Fi. A null allocation returns null and writes nothing.
// Otherwise, in the order of the image (n is the new object, s the source): the GameObject table
// pointer (_vt$10GameObject) goes to n+72, the ListHead copy constructor runs for the heads at
// +0, +4 and +8 (it zeroes the destination word and does not read the source), then source bytes
// 12..41, 44..57, 60..71 and 76..91 are copied, with the RollDoor table pointer (_vt$8RollDoor)
// replacing n+72 part-way through. Bytes 42..43, 58..59 and 73..75 and the heads are not copied
// from the source. The new object is returned. The ranges come from the load and store widths
// in the body; what the fields mean is unknown. Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
extern unsigned char vt_GameObject[] __asm__("_vt$10GameObject");
extern unsigned char vt_RollDoor[] __asm__("_vt$8RollDoor");
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__8RollDoor(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(92));
    if (n == nullptr) return nullptr;
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_GameObject);
    ListHead_copy(n, s);
    ListHead_copy(n + 4, s + 4);
    ListHead_copy(n + 8, s + 8);
    copy(n, s, 12, 41);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_RollDoor);
    copy(n, s, 44, 57);
    copy(n, s, 60, 71);
    copy(n, s, 76, 91);
    return n;
}
