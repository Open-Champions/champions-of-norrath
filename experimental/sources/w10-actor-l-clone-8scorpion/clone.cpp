// Observed clone__8Scorpion at 0x0011d568, 1356 bytes. Allocates 968 bytes with blockAlloc__Fi. A
// null allocation returns null and writes nothing. Otherwise, in the order of the image (n is the
// new object, s the source): the GameObject table pointer goes to n+72; the ListHead copy
// constructor runs for the heads at +0, +4 and +8; source bytes 12..41, 44..57 and 60..71 are
// copied; the Creature table pointer (0x4488f0) replaces n+72; source bytes 76, 80..85,
// 88..156, 160..173, 176..189, 192..205, 208..233 and 236..285 are copied; the AnimationState
// copy constructor (__15AnimationState8RC15AnimationState8) runs on the member at +288 from the
// source's; bytes 552..570 and 572..573 are copied; ListHead copy constructor at +576; bytes
// 580..749 copied; ListHead at +752; bytes 756..800 and 804..887 copied; ListHead at +888; bytes
// 892..959 copied; the Scorpion table pointer (0x44b630) replaces n+72; byte 960 is copied. The
// new object is returned. Bytes 42..43, 58..59, 77..79, 86..87, 157..159, 174..175, 190..191,
// 206..207, 234..235, 286..287, 571, 574..575, 750..755, 801..803, 888..891 (the list head
// itself), and 961..967 are left as the allocator returned them. Copy ranges were measured from
// the image's stores; the group copies are done with byte loops, not the image's word moves.
// Inferred: the members at +288 (animation state) and at +576, +752, +888 (list heads) from the
// callee symbols. Unknown: the meaning of the other fields. Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
void AnimationState_copy(void* destination, const void* source) __asm__("__15AnimationState8RC15AnimationState8");
extern const unsigned char norrath_game_object_vtable_view[];
extern const unsigned char norrath_actor_view_4488f0[];
extern const unsigned char norrath_ctors_lava_wraith_view_44b630[];
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__8Scorpion(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(968));
    if (n == nullptr) return nullptr;
    *reinterpret_cast<unsigned*>(n + 72) = low32(norrath_game_object_vtable_view);
    ListHead_copy(n, s);
    ListHead_copy(n + 4, s + 4);
    ListHead_copy(n + 8, s + 8);
    copy(n, s, 12, 41);
    copy(n, s, 44, 57);
    copy(n, s, 60, 71);
    *reinterpret_cast<unsigned*>(n + 72) = low32(norrath_actor_view_4488f0);
    copy(n, s, 76, 76);
    copy(n, s, 80, 85);
    copy(n, s, 88, 156);
    copy(n, s, 160, 173);
    copy(n, s, 176, 189);
    copy(n, s, 192, 205);
    copy(n, s, 208, 233);
    copy(n, s, 236, 285);
    AnimationState_copy(n + 288, s + 288);
    copy(n, s, 552, 570);
    copy(n, s, 572, 573);
    ListHead_copy(n + 576, s + 576);
    copy(n, s, 580, 749);
    ListHead_copy(n + 752, s + 752);
    copy(n, s, 756, 800);
    copy(n, s, 804, 887);
    ListHead_copy(n + 888, s + 888);
    copy(n, s, 892, 959);
    *reinterpret_cast<unsigned*>(n + 72) = low32(norrath_ctors_lava_wraith_view_44b630);
    copy(n, s, 960, 960);
    return n;
}
