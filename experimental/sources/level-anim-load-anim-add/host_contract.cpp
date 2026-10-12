#include "anim_add.cpp"
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(unsigned char*, unsigned char*, int);
static void fill(unsigned char* p, unsigned n) { for (unsigned i = 0; i != n; ++i) p[i] = 0x40 + (i * 7) % 61; }
int main() {
    unsigned char* state = arena_at(0x100);
    unsigned char* header = arena_at(0x900);
    unsigned char* link = arena_at(0xa00);
    unsigned char before[8 + 32 * 4], want[8 + 32 * 4];
    // Null header: no stores at all.
    fill(state, sizeof before); state[4] = 1; state[5] = 3;
    std::memcpy(before, state, sizeof before);
    animAdd__FP14AnimationStateP15AnimationHeaderi(state, nullptr, 5);
    if (same("null header", state, before, sizeof before)) return 1;
    // Full: signed byte 4 equal to signed byte 5.
    state[4] = 3; state[5] = 3; std::memcpy(before, state, sizeof before);
    animAdd__FP14AnimationStateP15AnimationHeaderi(state, header, 5);
    if (same("full", state, before, sizeof before)) return 2;
    // Negative capacity counts as full (signed comparison).
    state[4] = 0; state[5] = 0xff; std::memcpy(before, state, sizeof before);
    animAdd__FP14AnimationStateP15AnimationHeaderi(state, header, 5);
    if (same("negative capacity", state, before, sizeof before)) return 3;
    // Free slot 1 with a stale link pointer.
    fill(state, sizeof before); state[4] = 1; state[5] = 3; sh_(state + 6, 0x0110);
    unsigned char* comp = state + 8 + 32;
    sw_(comp + 4, norrath_host_address(link));
    fill(link, 64); sw_(link + 20, 0xdeadbeef);
    unsigned char link_before[64]; std::memcpy(link_before, link, 64);
    std::memcpy(want, state, sizeof want);
    want[4] = 2; sh_(want + 6, 0x0110 | 0x8003);
    unsigned char* w = want + 8 + 32;
    sw_(w + 0, norrath_host_address(header)); sw_(w + 4, 0);
    sf_(w + 8, 1.0f); sf_(w + 12, 0.0f); sf_(w + 16, 1.0f); sf_(w + 20, 0.0f);
    sw_(w + 24, 0); sh_(w + 28, 0x8003); sh_(w + 30, 0);
    animAdd__FP14AnimationStateP15AnimationHeaderi(state, header, 0x18003);
    if (same("slot 1", state, want, sizeof want)) return 4;
    sw_(link_before + 20, 0);
    if (same("link", link, link_before, 64)) return 5;
    // Free slot 0 with no link: nothing else is written.
    fill(state, sizeof before); state[4] = 0; state[5] = 2; sw_(state + 8 + 4, 0);
    std::memcpy(want, state, sizeof want);
    want[4] = 1; sh_(want + 6, hw(state + 6) & 0xffff | 1);
    sw_(want + 8, norrath_host_address(header));
    sf_(want + 16, 1.0f); sf_(want + 20, 0.0f); sf_(want + 24, 1.0f); sf_(want + 28, 0.0f);
    sw_(want + 32, 0); sh_(want + 36, 1); sh_(want + 38, 0);
    animAdd__FP14AnimationStateP15AnimationHeaderi(state, header, 1);
    if (same("slot 0", state, want, sizeof want)) return 6;
    return 0;
}
