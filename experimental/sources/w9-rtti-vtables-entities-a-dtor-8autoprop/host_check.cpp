// Host check only. Not an EE build input.
#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" void dtor_8AutoProp(void* object, int flags) asm("_$_8AutoProp");
extern "C" unsigned char norrath_ctors_ant_lantern_view_44ec40[8] = {};
extern "C" unsigned char norrath_rtti_entities_a_view_44ed78[8] = {};
extern "C" unsigned char norrath_actors_b_view_4844a0[4] = {0x40, 0x30, 0x20, 0x00};
extern "C" void animation_state_destructor(void* state, int flags) asm("_$_14AnimationState");

static int failures = 0, step = 0;
static int release_step, anim_step, base_step, free_step, free_calls;
static void* release_world;
static int release_index;
static unsigned vt_at_release, vt_at_anim;
static unsigned char* anim_object;
static int anim_flags, base_flags;
static unsigned char* base_object;
static unsigned char* freed;
static unsigned char* current;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
extern "C" int worldReleaseDoor__FP12_worldHeaderi(void* world, int index) {
    release_step = ++step; release_world = world; release_index = index; vt_at_release = read_word(current + 72);
    return 0;
}
extern "C" void animation_state_destructor(void* state, int flags) {
    anim_step = ++step; anim_object = static_cast<unsigned char*>(state); anim_flags = flags; vt_at_anim = read_word(current + 72);
}
extern "C" void norrath_vt_dtor_10GameObject(void* object, int flags) {
    base_step = ++step; base_object = static_cast<unsigned char*>(object); base_flags = flags;
}
extern "C" void norrath_block_free(void* object) { free_step = ++step; ++free_calls; freed = static_cast<unsigned char*>(object); }

int main() {
    unsigned char object[512];
    const unsigned want_first = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_ctors_ant_lantern_view_44ec40));
    const unsigned want_second = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_rtti_entities_a_view_44ed78));
    const int flags_to_try[] = {0, 1, 2, 3, 5, -1, -2};
    const int indexes[] = {-1, 0, 7, 0x7fffffff};
    for (int flags : flags_to_try)
        for (int index : indexes) {
            std::memset(object, 0x33, sizeof object);
            unsigned char before[512];
            const unsigned stored = static_cast<unsigned>(index);
            std::memcpy(object + 396, &stored, 4);
            std::memcpy(before, object, sizeof object);
            current = object;
            step = release_step = anim_step = base_step = free_step = free_calls = 0;
            freed = nullptr;
            dtor_8AutoProp(object, flags);
            expect(release_step == 1 && release_index == index, "door released first with the +396 word");
            expect(static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(release_world)) == 0x00203040u, "world word passed");
            expect(vt_at_release == want_first, "own vtable stored before the release");
            expect(anim_step == 2 && anim_object == object + 152 && anim_flags == 2 && vt_at_anim == want_second, "animation state destroyed with flag 2 after the second vtable store");
            expect(base_step == 3 && base_object == object && base_flags == 0, "base destructor with flag 0");
            expect(free_calls == ((flags & 1) ? 1 : 0) && (!(flags & 1) || (free_step == 4 && freed == object)), "free last, only for bit 0");
            expect(std::memcmp(object, before, 72) == 0 && std::memcmp(object + 76, before + 76, 436) == 0, "only +72 written");
            expect(read_word(object + 72) == want_second, "final vtable word");
        }
    if (failures == 0) std::printf("8AutoProp destructor cases passed\n");
    return failures == 0 ? 0 : 1;
}
