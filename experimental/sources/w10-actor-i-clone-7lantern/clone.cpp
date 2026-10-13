// Observed clone__7Lantern at 0x00123b68, 432 bytes.
// Allocates 152 bytes with blockAlloc__Fi. A null allocation returns null and writes nothing.
// Otherwise, in the order of the image (n is the new object, s the source): the GameObject table
// pointer (_vt$10GameObject) goes to n+72, the ListHead copy constructor runs for the heads at +0,
// +4 and +8 (it zeroes the destination word and does not read the source), source bytes 12..41,
// 44..57 and 60..71 are copied, the Lantern table pointer (_vt$7Lantern, 0x44a8a0) replaces n+72,
// and the 60-byte member at 76..135 is copied. When either of its first two words (n+76, n+80) is
// then nonzero, both are cleared and lightAdd__FP5Light runs on the member. Finally source bytes
// 136..149 are copied. Bytes 42..43 and 58..59 are left as the allocator returned them. The new
// object is returned.
// Copy ranges follow the loads and stores of the image's body. Inferred: the member at +76 is a
// light, from the callee's parameter type (Lamp and Candle have the same shape). Unknown: the
// meaning of the other fields. Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
void lightAdd__FP5Light(void* light);
extern const unsigned char norrath_game_object_vtable_view[];
extern const unsigned char norrath_ctors_ant_lantern_view_44a8a0[];
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__7Lantern(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(152));
    if (n == nullptr) return nullptr;
    *reinterpret_cast<unsigned*>(n + 72) = low32(norrath_game_object_vtable_view);
    ListHead_copy(n, s);
    ListHead_copy(n + 4, s + 4);
    ListHead_copy(n + 8, s + 8);
    copy(n, s, 12, 41);
    copy(n, s, 44, 57);
    copy(n, s, 60, 71);
    *reinterpret_cast<unsigned*>(n + 72) = low32(norrath_ctors_ant_lantern_view_44a8a0);
    copy(n, s, 76, 135);
    unsigned* light = reinterpret_cast<unsigned*>(n + 76);
    if (light[0] != 0 || light[1] != 0) {
        light[0] = 0;
        light[1] = 0;
        lightAdd__FP5Light(light);
    }
    copy(n, s, 136, 149);
    return n;
}
