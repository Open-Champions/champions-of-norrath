// Host contract for animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi. Not an EE plan input.
// Build: c++ -std=c++17 -DNORRATH_HOST_ARENA host_check.cpp
#define NORRATH_HOST_ARENA
#include "../w9-animation-object-a-common/host_arena.hpp"
#include "../w9-animation-object-a-common/host_check.hpp"
#include "anim_add_hairy.cpp"
static void fill(unsigned char* p, unsigned n) { for (unsigned i = 0; i != n; ++i) p[i] = 0x40 + (i * 7) % 61; }
#define CALL(s, h, f) animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi(s, h, 111.0f, 2.5f, -3.0f, 4.75f, 8.0f, f)
int main() {
    unsigned char* state = arena_at(0x100);
    unsigned char* header = arena_at(0x900);
    unsigned char* link = arena_at(0xa00);
    const unsigned N = 8 + 32 * 4;
    unsigned char before[N], want[N];
    fill(state, N); state[4] = 1; state[5] = 3; std::memcpy(before, state, N);
    CALL(state, nullptr, 5);
    if (same("null header", state, before, N)) return 1;
    state[4] = 3; state[5] = 3; std::memcpy(before, state, N);
    CALL(state, header, 5);
    if (same("full", state, before, N)) return 2;
    state[4] = 0; state[5] = 0xff; std::memcpy(before, state, N);
    CALL(state, header, 5);
    if (same("negative capacity", state, before, N)) return 3;
    // Free slot 1 with a stale link.
    fill(state, N); state[4] = 1; state[5] = 3; sh_(state + 6, 0x0110);
    unsigned char* comp = state + 8 + 32;
    sw_(comp + 4, norrath_host_address(link));
    fill(link, 64); sw_(link + 20, 0xdeadbeef);
    unsigned char link_before[64]; std::memcpy(link_before, link, 64);
    std::memcpy(want, state, N);
    want[4] = 2; sh_(want + 6, 0x0110 | 0x8003);
    unsigned char* w = want + 8 + 32;
    sw_(w + 0, norrath_host_address(header)); sw_(w + 4, 0);
    sf_(w + 8, 2.5f); sf_(w + 12, -3.0f); sf_(w + 16, 4.75f); sf_(w + 20, 8.0f);
    sw_(w + 24, 0); sh_(w + 28, 0x8003); sh_(w + 30, 0);
    CALL(state, header, 0x18003);
    if (same("slot 1", state, want, N)) return 4;
    sw_(link_before + 20, 0);
    if (same("link", link, link_before, 64)) return 5;
    // Free slot 0, no link.
    fill(state, N); state[4] = 0; state[5] = 2; sw_(state + 8 + 4, 0);
    std::memcpy(want, state, N);
    want[4] = 1; sh_(want + 6, (hw(state + 6) & 0xffff) | 1);
    sw_(want + 8, norrath_host_address(header));
    sf_(want + 16, 2.5f); sf_(want + 20, -3.0f); sf_(want + 24, 4.75f); sf_(want + 28, 8.0f);
    sw_(want + 32, 0); sh_(want + 36, 1); sh_(want + 38, 0);
    CALL(state, header, 1);
    if (same("slot 0", state, want, N)) return 6;
    std::printf("animAddHairy check passed\n");
    return 0;
}
