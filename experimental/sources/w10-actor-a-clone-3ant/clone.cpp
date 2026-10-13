// Observed clone__3Ant at 0x00116c70, 1400 bytes.
// Allocates 992 bytes with blockAlloc__Fi. A null allocation returns null and writes nothing.
// Otherwise, in the order of the image (n is the new object, s the source):
//  - the GameObject table pointer (_vt$10GameObject) goes to n+72, then the ListHead copy
//    constructor runs for the heads at +0, +4 and +8 (it zeroes the destination word and does
//    not read the source);
//  - source bytes 12..41, 44..57 and 60..71 are copied; the Creature table pointer
//    (_vt$8Creature) replaces n+72; then 76, 80..85, 88..156, 160..173, 176..189, 192..205,
//    208..233 and 236..285 are copied;
//  - the AnimationState8 copy constructor runs for the member at +288 (it copies 264 bytes);
//  - 552..570 and 572..573 are copied; the ListHead copy constructor runs for +576;
//  - 580..749 are copied; the ListHead copy constructor runs for +752;
//  - 756..800 and 804..887 are copied; the ListHead copy constructor runs for +888;
//  - 892..959 are copied; the Ant table pointer (_vt$3Ant) replaces n+72; then 960, 964..968
//    and 972..987 are copied.
// Bytes of the new object not named above are left as the allocator returned them (0..11 are
// the list heads, the rest are gaps of 1 to 4 bytes between fields, 286..551 is the
// AnimationState8 member and 574..579 and 750..755 are other gaps). The new object is returned.
// Copy ranges were measured by running the original on a pattern-filled source; the field
// boundaries inside each range are not established. Inferred: Ant's parent chain from the
// table pointers. Unknown: the meaning of the fields. Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
void AnimationState8_copy(void* destination, const void* source) __asm__("__15AnimationState8RC15AnimationState8");
extern unsigned char vt_GameObject[] __asm__("_vt$10GameObject");
extern unsigned char vt_Creature[] __asm__("_vt$8Creature");
extern unsigned char vt_Ant[] __asm__("_vt$3Ant");
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__3Ant(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(992));
    if (n == nullptr) return nullptr;
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_GameObject);
    ListHead_copy(n, s);
    ListHead_copy(n + 4, s + 4);
    ListHead_copy(n + 8, s + 8);
    copy(n, s, 12, 41);
    copy(n, s, 44, 57);
    copy(n, s, 60, 71);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_Creature);
    copy(n, s, 76, 76);
    copy(n, s, 80, 85);
    copy(n, s, 88, 156);
    copy(n, s, 160, 173);
    copy(n, s, 176, 189);
    copy(n, s, 192, 205);
    copy(n, s, 208, 233);
    copy(n, s, 236, 285);
    AnimationState8_copy(n + 288, s + 288);
    copy(n, s, 552, 570);
    copy(n, s, 572, 573);
    ListHead_copy(n + 576, s + 576);
    copy(n, s, 580, 749);
    ListHead_copy(n + 752, s + 752);
    copy(n, s, 756, 800);
    copy(n, s, 804, 887);
    ListHead_copy(n + 888, s + 888);
    copy(n, s, 892, 959);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_Ant);
    copy(n, s, 960, 960);
    copy(n, s, 964, 968);
    copy(n, s, 972, 987);
    return n;
}
