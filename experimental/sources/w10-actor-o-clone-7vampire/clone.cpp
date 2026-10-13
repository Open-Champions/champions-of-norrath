// Observed clone__7Vampire at 0x00120990, 1348 bytes.
// Allocates 960 bytes with blockAlloc__Fi. A null allocation returns null and writes nothing.
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
//  - the 8Creature table pointer (_vt$8Creature) goes to n+72
//  - source bytes 76..76 are copied
//  - source bytes 80..85 are copied
//  - source bytes 88..156 are copied
//  - source bytes 160..173 are copied
//  - source bytes 176..189 are copied
//  - source bytes 192..205 are copied
//  - source bytes 208..233 are copied
//  - source bytes 236..285 are copied
//  - the AnimationState8 copy constructor runs for the member at +288
//  - source bytes 552..570 are copied
//  - source bytes 572..573 are copied
//  - the ListHead copy constructor runs for the head at +576
//  - source bytes 580..749 are copied
//  - the ListHead copy constructor runs for the head at +752
//  - source bytes 756..800 are copied
//  - source bytes 804..887 are copied
//  - the ListHead copy constructor runs for the head at +888
//  - the 7Vampire table pointer (_vt$7Vampire) goes to n+72
//  - source bytes 892..959 are copied.
// Bytes of the new object not named above are left as the allocator returned them (gaps of 1 to 8 bytes between fields). The new object is returned. Copy ranges and call order were measured by running the
// original on a pattern-filled source (table pointer stores are only ordered relative to the
// calls). Inferred: the parent chain from the table pointers. Unknown: the meaning of the fields.
// Nothing here ran on the EE.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
void AnimationState8_copy(void* destination, const void* source) __asm__("__15AnimationState8RC15AnimationState8");
extern unsigned char vt_10GameObject[] __asm__("_vt$10GameObject");
extern unsigned char vt_8Creature[] __asm__("_vt$8Creature");
extern unsigned char vt_7Vampire[] __asm__("_vt$7Vampire");
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned last) {
    for (unsigned i = first; i <= last; ++i) to[i] = from[i];
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__7Vampire(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(960));
    if (n == nullptr) return nullptr;
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_10GameObject);
    ListHead_copy(n, s);
    ListHead_copy(n + 4, s + 4);
    ListHead_copy(n + 8, s + 8);
    copy(n, s, 12, 41);
    copy(n, s, 44, 57);
    copy(n, s, 60, 71);
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_8Creature);
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
    *reinterpret_cast<unsigned*>(n + 72) = low32(vt_7Vampire);
    copy(n, s, 892, 959);
    return n;
}
