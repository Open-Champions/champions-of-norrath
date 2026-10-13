// Host contract for gameLineInitPathState__FP17gameLinePathStatePcP6Point3. Not an EE plan input.
// Build: c++ -std=c++17 -DNORRATH_HOST_ARENA host_check.cpp
#define NORRATH_HOST_ARENA
#include <cstring>
#include "../w9-animation-object-a-common/host_arena.hpp"
#include "../w9-animation-object-a-common/host_check.hpp"
// The unit calls memset by name; on the host that name is redirected to a recording stand-in.
#define memset norrath_host_memset
#include "game_line_init_path_state.cpp"
#undef memset
static unsigned memset_count;
static void* memset_target;
extern "C" void* norrath_host_memset(void* d, int v, __SIZE_TYPE__ n) {
    memset_target = d; memset_count = n;
    unsigned char* p = static_cast<unsigned char*>(d);
    for (unsigned i = 0; i != n; ++i) p[i] = static_cast<unsigned char>(v);
    return d;
}
int main() {
    unsigned char* state = arena_at(0x100);
    unsigned char* tag = arena_at(0x900);
    unsigned char* point = arena_at(0xa00);
    const unsigned N = 96;
    unsigned char before[N], want[N];
    for (unsigned i = 0; i != N; ++i) state[i] = 0x40 + i;
    std::memcpy(want, state, N);
    std::memset(want, 0, 56);
    sw_(want + 0, norrath_host_address(point));
    sw_(want + 20, norrath_host_address(tag));
    sw_(want + 28, 0xffffffffu);
    sw_(want + 32, 0xffffffffu);
    std::memcpy(before, state, N);
    gameLineInitPathState__FP17gameLinePathStatePcP6Point3(state, tag, point);
    if (memset_target != state || memset_count != 56) { std::printf("memset extent\n"); return 1; }
    if (same("state", state, want, N)) return 2;
    // Bytes at and beyond +56 keep their old contents.
    if (same("tail", state + 56, before + 56, N - 56)) return 3;
    // Null tag and point are stored as zero words.
    for (unsigned i = 0; i != N; ++i) state[i] = 0x99;
    gameLineInitPathState__FP17gameLinePathStatePcP6Point3(state, nullptr, nullptr);
    if (hw(state + 0) != 0 || hw(state + 20) != 0 || hw(state + 28) != 0xffffffffu || hw(state + 32) != 0xffffffffu) return 4;
    std::printf("gameLineInitPathState check passed\n");
    return 0;
}
