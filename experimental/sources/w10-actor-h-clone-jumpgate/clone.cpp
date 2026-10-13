// Observed clone__8JumpGate at 0x00128248, 660 bytes.
// Allocates 356 bytes with blockAlloc__Fi. A null allocation returns null and writes nothing.
// Otherwise, in the order of the image (n is the new object, s the source): the GameObject table
// pointer (_vt$10GameObject) goes to n+72, the ListHead copy constructor runs for the heads at +0,
// +4 and +8 (it zeroes the destination word and does not read the source), source bytes 12..41,
// 44..57 and 60..71 are copied, the JumpGate table pointer (_vt$8JumpGate) replaces n+72, then
// bytes 76..79, 80..143 and 144..207 (two 64-byte runs, copied byte by byte), byte 208 and bytes
// 212..215 are copied. The AnimationState copy constructor runs for the member at +216 (n+216,
// s+216). The 60-byte member at 256..315 is copied; when either of its first two words is then
// nonzero, both are cleared and lightAdd__FP5Light runs on it. Finally bytes 316..355 are copied.
// Bytes 42..43, 58..59, 209..211 and 217..255 are left as the allocator and the AnimationState
// constructor left them. The new object is returned.
// Copy ranges were measured by running the original on a pattern-filled source. Inferred: the
// member at +256 is a light (the callee's parameter type) and the member at +216 is an
// AnimationState (the constructor). Unknown: what the two 64-byte runs hold, and the fields in
// 316..355 (the destructor treats +316 as an FxCreatureState, but the clone copies it bytewise).
// Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
void AnimationState_copy(void* destination, const void* source) __asm__("__14AnimationStateRC14AnimationState");
void lightAdd__FP5Light(void* light);
extern unsigned char vt_GameObject[] __asm__("_vt$10GameObject");
extern unsigned char vt_JumpGate[] __asm__("_vt$8JumpGate");
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__8JumpGate(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(356));
    if (n == nullptr) return nullptr;
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_GameObject);
    ListHead_copy(n, s);
    ListHead_copy(n + 4, s + 4);
    ListHead_copy(n + 8, s + 8);
    copy(n, s, 12, 41);
    copy(n, s, 44, 57);
    copy(n, s, 60, 71);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_JumpGate);
    copy(n, s, 76, 79);
    copy(n, s, 80, 143);
    copy(n, s, 144, 207);
    copy(n, s, 208, 208);
    copy(n, s, 212, 215);
    AnimationState_copy(n + 216, s + 216);
    copy(n, s, 256, 315);
    unsigned* light = reinterpret_cast<unsigned*>(n + 256);
    if (light[0] != 0 || light[1] != 0) {
        light[0] = 0;
        light[1] = 0;
        lightAdd__FP5Light(light);
    }
    copy(n, s, 316, 355);
    return n;
}
