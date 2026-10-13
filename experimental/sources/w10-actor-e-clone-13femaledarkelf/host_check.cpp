// Host contract for clone__13FemaleDarkElf. Includes the source; not an EE input.
#include "clone.cpp"
#include <cstdio>
#include <cstring>
#include <vector>
struct Range { unsigned first, last; };
static const Range copied[] = {{12, 41}, {44, 57}, {60, 71}, {76, 76}, {80, 85}, {88, 156}, {160, 173}, {176, 189}, {192, 205}, {208, 233}, {236, 285}, {552, 570}, {572, 573}, {580, 749}, {756, 800}, {804, 887}, {892, 960}, {964, 1107}};
extern "C" const unsigned char norrath_game_object_vtable_view[4] = {};
extern "C" const unsigned char norrath_actor_view_4488f0[4] = {};
extern "C" const unsigned char norrath_ctors_ant_lantern_view_450530[4] = {};
static unsigned char block[1112 + 64];
static int alloc_size = 0; static void* alloc_result = block;
static std::vector<int> events;  // list-head copies are 0..., animation copy is -1
extern "C" void* blockAlloc__Fi(int size) { alloc_size = size; return alloc_result; }
extern "C" void host_list_head_copy(void* destination, const void* source) __asm__("__8ListHeadRC8ListHead");
extern "C" void host_list_head_copy(void* destination, const void* source) {
    events.push_back(static_cast<int>(static_cast<unsigned char*>(destination) - block));
    (void)source;
    std::memset(destination, 0, 4);
}
extern "C" void host_animation_copy(void* destination, const void* source) __asm__("__15AnimationState8RC15AnimationState8");
extern "C" void host_animation_copy(void* destination, const void* source) {
    events.push_back(-static_cast<int>(static_cast<unsigned char*>(destination) - block) - 1);
    std::memcpy(destination, source, 264);
}
static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
int main() {
    static unsigned char source[1112];
    for (unsigned i = 0; i < sizeof source; ++i) source[i] = static_cast<unsigned char>((i * 37u + 11u) | 1u);
    std::memset(block, 0xa5, sizeof block);
    void* result = clone__13FemaleDarkElf(source);
    if (result != block) return 1;
    if (alloc_size != 1112) return 2;
    // list heads at 0, 4, 8, the animation state at 288, list heads at 576, 752, 888, in that order
    const int want[] = {0, 4, 8, -289, 576, 752, 888};
    if (events.size() != 7) return 3;
    for (int i = 0; i < 7; ++i) if (events[i] != want[i]) return 4;
    std::vector<unsigned char> covered(sizeof source, 0);
    for (const Range& r : copied) for (unsigned i = r.first; i <= r.last; ++i) {
        covered[i] = 1;
        if (block[i] != source[i]) { std::printf("byte %u\n", i); return 5; }
    }
    for (unsigned i = 288; i < 288 + 264; ++i) if (block[i] != source[i]) return 6;
    for (unsigned i : {0u, 4u, 8u, 576u, 752u, 888u}) if (word(block + i) != 0u) return 7;
    // the table word is the class table (the host stubs are 64-bit, so compare the low 32 bits)
    if (word(block + 72) != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(norrath_ctors_ant_lantern_view_450530))) return 8;
    for (unsigned i = 0; i < sizeof source; ++i) {
        const bool listed = covered[i] || (i >= 288 && i < 288 + 264) || (i >= 72 && i < 76) || i == 0 || i == 4 || i == 8 || i == 576 || i == 752 || i == 888 ||
                            (i < 12) || (i >= 576 && i < 580) || (i >= 752 && i < 756) || (i >= 888 && i < 892);
        if (!listed && block[i] != 0xa5) { std::printf("stray store %u\n", i); return 9; }
    }
    // a null allocation returns null and touches nothing
    std::memset(block, 0xa5, sizeof block); events.clear(); alloc_result = nullptr;
    if (clone__13FemaleDarkElf(source) != nullptr || !events.empty()) return 10;
    for (unsigned char b : block) if (b != 0xa5) return 11;
    return 0;
}
