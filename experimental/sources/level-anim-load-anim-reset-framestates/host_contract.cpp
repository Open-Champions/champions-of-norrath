#include "anim_reset_framestates.cpp"
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
int main() {
    unsigned char* state = arena_at(0x100);
    unsigned char* result = arena_at(0x1000);
    unsigned char* link0 = arena_at(0x1800);
    unsigned char* link2 = arena_at(0x1900);
    unsigned char before[8 + 32 * 4], want[8 + 32 * 4];
    for (unsigned i = 0; i != sizeof before; ++i) state[i] = 0x10 + i;
    for (unsigned i = 0; i != 1600; ++i) result[i] = 0x33;
    for (unsigned i = 0; i != 64; ++i) link0[i] = link2[i] = 0x77;
    state[4] = 3;                                // three components in use; slot 3 is stale
    sw_(state + 0, norrath_host_address(result));
    sw_(state + 8 + 4, norrath_host_address(link0));
    sw_(state + 8 + 32 + 4, 0);                  // slot 1 has no link
    sw_(state + 8 + 64 + 4, norrath_host_address(link2));
    sw_(state + 8 + 96 + 4, norrath_host_address(link2));   // beyond the count: untouched
    std::memcpy(want, state, sizeof want);
    sw_(want + 0, 0); sw_(want + 8 + 4, 0); sw_(want + 8 + 64 + 4, 0);
    unsigned char result_want[1600], l0[64], l2[64];
    std::memset(result_want, 0x33, 1600); sw_(result_want + 1552, 0);
    std::memset(l0, 0x77, 64); sw_(l0 + 20, 0);
    std::memset(l2, 0x77, 64); sw_(l2 + 20, 0);
    animResetFramestates__FP14AnimationState(state);
    if (same("state", state, want, sizeof want)) return 1;
    if (same("result", result, result_want, 1600)) return 2;
    if (same("link0", link0, l0, 64)) return 3;
    if (same("link2", link2, l2, 64)) return 4;
    // No result pointer, zero count: nothing changes.
    for (unsigned i = 0; i != sizeof before; ++i) state[i] = want[i] = 0x90 + i;
    state[4] = want[4] = 0; sw_(state, 0); sw_(want, 0);
    animResetFramestates__FP14AnimationState(state);
    if (same("idle", state, want, sizeof want)) return 5;
    // Negative count behaves as zero.
    state[4] = want[4] = 0xfe;
    animResetFramestates__FP14AnimationState(state);
    return same("negative", state, want, sizeof want) ? 6 : 0;
}
