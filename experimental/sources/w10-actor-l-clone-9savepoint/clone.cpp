// Observed clone__9Savepoint at 0x00128048, 508 bytes. Allocates 208 bytes with blockAlloc__Fi. A
// null allocation returns null and writes nothing. Otherwise, in the order of the image (n is the
// new object, s the source): the GameObject table pointer goes to n+72, the ListHead copy
// constructor runs for the heads at +0, +4 and +8, source bytes 12..41, 44..57 and 60..71 are
// copied, the Savepoint table pointer (0x449550) replaces n+72, the AnimationState copy
// constructor runs on n+76 from s+76, source bytes 120..195 are copied, and the 60-byte member
// at +136 is checked: when either of its first two words is nonzero both are cleared and
// lightAdd__FP5Light runs on the member. Finally source bytes 196..201 and 204..207 are copied.
// Bytes 42..43, 58..59 and 202..203 are left as the allocator returned them. The new object is
// returned. Copy ranges were measured from the image's stores. Inferred: the member at +136 is
// a light, from the callee's parameter type. Unknown: the meaning of the other fields. Nothing
// here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
void AnimationState_copy(void* destination, const void* source) __asm__("__14AnimationStateRC14AnimationState");
void lightAdd__FP5Light(void* light);
extern const unsigned char norrath_game_object_vtable_view[];
extern const unsigned char norrath_ctors_lava_wraith_view_449550[];
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__9Savepoint(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(208));
    if (n == nullptr) return nullptr;
    *reinterpret_cast<unsigned*>(n + 72) = low32(norrath_game_object_vtable_view);
    ListHead_copy(n, s);
    ListHead_copy(n + 4, s + 4);
    ListHead_copy(n + 8, s + 8);
    copy(n, s, 12, 41);
    copy(n, s, 44, 57);
    copy(n, s, 60, 71);
    *reinterpret_cast<unsigned*>(n + 72) = low32(norrath_ctors_lava_wraith_view_449550);
    AnimationState_copy(n + 76, s + 76);
    copy(n, s, 120, 195);
    unsigned* light = reinterpret_cast<unsigned*>(n + 136);
    if (light[0] != 0 || light[1] != 0) {
        light[0] = 0;
        light[1] = 0;
        lightAdd__FP5Light(light);
    }
    copy(n, s, 196, 201);
    copy(n, s, 204, 207);
    return n;
}
