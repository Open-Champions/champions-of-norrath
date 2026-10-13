// Observed clone__4Lamp at 0x00123d30, 432 bytes.
// Allocates 140 bytes with blockAlloc__Fi. A null allocation returns null and writes nothing.
// Otherwise, in the order of the image (n is the new object, s the source): the GameObject table
// pointer (_vt$10GameObject) goes to n+72, the ListHead copy constructor runs for the heads at +0,
// +4 and +8 (it zeroes the destination word and does not read the source), source bytes 12..41,
// 44..57 and 60..71 are copied, the Lamp table pointer (_vt$4Lamp) replaces n+72, and the 60-byte
// member at 76..135 is copied. When either of its first two words (n+76, n+80) is then nonzero,
// both are cleared and lightAdd__FP5Light runs on the member. Finally source bytes 136 and 137 are
// copied. Bytes 42..43, 58..59 and 138..139 are left as the allocator returned them. The new
// object is returned.
// Copy ranges were measured by running the original on a pattern-filled source. Inferred: the
// member at +76 is a light, from the callee's parameter type (the same shape is used by Candle).
// Unknown: the meaning of the other fields. Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
void lightAdd__FP5Light(void* light);
extern unsigned char vt_GameObject[] __asm__("_vt$10GameObject");
extern unsigned char vt_Lamp[] __asm__("_vt$4Lamp");
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__4Lamp(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(140));
    if (n == nullptr) return nullptr;
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_GameObject);
    ListHead_copy(n, s);
    ListHead_copy(n + 4, s + 4);
    ListHead_copy(n + 8, s + 8);
    copy(n, s, 12, 41);
    copy(n, s, 44, 57);
    copy(n, s, 60, 71);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_Lamp);
    copy(n, s, 76, 135);
    unsigned* light = reinterpret_cast<unsigned*>(n + 76);
    if (light[0] != 0 || light[1] != 0) {
        light[0] = 0;
        light[1] = 0;
        lightAdd__FP5Light(light);
    }
    copy(n, s, 136, 137);
    return n;
}
