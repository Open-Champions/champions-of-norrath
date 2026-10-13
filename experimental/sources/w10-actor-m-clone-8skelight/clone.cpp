// Observed clone__8Skelight at 0x11e108 (1572 bytes). Allocates 1112 bytes with blockAlloc__Fi; a null allocation returns null
// and writes nothing. Otherwise, in the order of the image (n is the new object, s the source): the GameObject
// table (norrath_game_object_vtable_view) goes to n+72, the ListHead copy constructor runs for the heads at
// +0, +4 and +8, and source bytes are copied in spans measured from the image's stores (the spans are listed in
// the code). The Creature table (data view _vt$8Creature, norrath_actor_view_4488f0) replaces n+72 before the first
// large copy block, and the AnimationState copy constructor (symbol __15AnimationState8RC15AnimationState8) runs
// on n+288 from s+288. The remaining ListHead copy constructors are called at the offsets shown, each after
// the copies that precede it in the code, and the final table (_vt$8Skelight at 0x44fdf8) is stored last, after the final copy block.
// Bytes outside the spans (padding, the head words, the animation block) are left as the allocator and the
// callees leave them. The new object is returned.
// Inferred: the ListHead and AnimationState members are copy-constructed because the image calls those
// constructors with (new member, source member). Unknown: the meaning of the copied fields.
extern "C" {
void* blockAlloc__Fi(int size);
void ListHead_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
void AnimationState_copy(void* destination, const void* source) __asm__("__15AnimationState8RC15AnimationState8");
extern const unsigned char norrath_game_object_vtable_view[];
extern const unsigned char norrath_actor_view_4488f0[];
extern const unsigned char norrath_ctors_lava_wraith_view_44fdf8[];
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

inline __attribute__((always_inline)) void copy(unsigned char* to, const unsigned char* from, unsigned first, unsigned count) {
    for (unsigned i = 0; i < count; ++i) to[first + i] = from[first + i];
}
inline __attribute__((always_inline)) void set_table(unsigned char* object, const unsigned char* view) {
    const unsigned bits = low32(view);
    object[72] = static_cast<unsigned char>(bits);
    object[73] = static_cast<unsigned char>(bits >> 8);
    object[74] = static_cast<unsigned char>(bits >> 16);
    object[75] = static_cast<unsigned char>(bits >> 24);
}
}  // namespace

// The attribute keeps the reviewed compiler from turning the copy loops into memcpy calls.
extern "C" __attribute__((optimize("no-tree-loop-distribute-patterns")))
void* clone__8Skelight(void* source) {
    const unsigned char* s = static_cast<const unsigned char*>(source);
    unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(1112));
    if (n == nullptr) return nullptr;
    set_table(n, norrath_game_object_vtable_view);
    ListHead_copy(n, s);
    ListHead_copy(n + 4, s + 4);
    ListHead_copy(n + 8, s + 8);
    copy(n, s, 12, 30);
    copy(n, s, 44, 14);
    copy(n, s, 60, 12);
    set_table(n, norrath_actor_view_4488f0);
    copy(n, s, 76, 1);
    copy(n, s, 80, 6);
    copy(n, s, 88, 69);
    copy(n, s, 160, 14);
    copy(n, s, 176, 14);
    copy(n, s, 192, 14);
    copy(n, s, 208, 26);
    copy(n, s, 236, 50);
    AnimationState_copy(n + 288, s + 288);
    copy(n, s, 552, 19);
    copy(n, s, 572, 2);
    ListHead_copy(n + 576, s + 576);
    copy(n, s, 580, 170);
    ListHead_copy(n + 752, s + 752);
    copy(n, s, 756, 45);
    copy(n, s, 804, 84);
    ListHead_copy(n + 888, s + 888);
    copy(n, s, 892, 216);
    set_table(n, norrath_ctors_lava_wraith_view_44fdf8);
    return n;
}
