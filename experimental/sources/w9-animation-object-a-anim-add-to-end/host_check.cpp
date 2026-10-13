// Host contract for animAddToEnd__FP14AnimationStateP15AnimationHeaderi. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp
#define NORRATH_HOST_ARENA
#include "../w9-animation-object-a-common/host_arena.hpp"
#include "../w9-animation-object-a-common/host_check.hpp"
#include "anim_add_to_end.cpp"
static void fill(unsigned char* p, unsigned n) { for (unsigned i = 0; i != n; ++i) p[i] = 0x40 + (i * 7) % 61; }
typedef void (*Fn)(unsigned char*, unsigned char*, int);
int main() {
    unsigned char* state = arena_at(0x100);
    unsigned char* header = arena_at(0x900);
    unsigned char* prev_header = arena_at(0xa00);
    unsigned char* link = arena_at(0xb00);
    const unsigned N = 8 + 32 * 5;
    unsigned char before[N], want[N];
    // Null header: no stores.
    fill(state, N); state[4] = 2; state[5] = 5; std::memcpy(before, state, N);
    animAddToEnd__FP14AnimationStateP15AnimationHeaderi(state, nullptr, 5);
    if (same("null header", state, before, N)) return 1;
    // Full (equal) and negative capacity count as full.
    state[4] = 3; state[5] = 3; std::memcpy(before, state, N);
    animAddToEnd__FP14AnimationStateP15AnimationHeaderi(state, header, 5);
    if (same("full", state, before, N)) return 2;
    state[4] = 1; state[5] = 0xff; std::memcpy(before, state, N);
    animAddToEnd__FP14AnimationStateP15AnimationHeaderi(state, header, 5);
    if (same("negative capacity", state, before, N)) return 3;
    // Count 2: previous component is slot 1, new one is slot 2, stale link pointer present.
    fill(state, N); state[4] = 2; state[5] = 5; sh_(state + 6, 0x0110);
    unsigned char* prev = state + 8 + 32;
    unsigned char* comp = state + 8 + 64;
    sw_(prev + 0, norrath_host_address(prev_header));
    sf_(prev + 24, 10.5f);
    fill(prev_header, 16); sw_(prev_header + 4, 4);          // length 4 -> new start 6.5
    sw_(comp + 4, norrath_host_address(link));
    fill(link, 64); sw_(link + 20, 0xdeadbeef);
    unsigned char link_before[64]; std::memcpy(link_before, link, 64);
    std::memcpy(want, state, N);
    want[4] = 3; sh_(want + 6, 0x0110 | 0x8003);
    unsigned char* w = want + 8 + 64;
    sw_(w + 0, norrath_host_address(header)); sw_(w + 4, 0);
    sf_(w + 8, 1.0f); sw_(w + 12, 0); sf_(w + 16, 1.0f); sw_(w + 20, 0);
    sf_(w + 24, 6.5f); sh_(w + 28, 0x8003);                   // +30 keeps its filler bytes
    animAddToEnd__FP14AnimationStateP15AnimationHeaderi(state, header, 0x18003);
    if (same("slot 2", state, want, N)) return 4;
    sw_(link_before + 20, 0);
    if (same("link", link, link_before, 64)) return 5;
    // Count 1, no link, negative length word: start grows.
    fill(state, N); state[4] = 1; state[5] = 2; sh_(state + 6, 0);
    prev = state + 8; comp = state + 8 + 32;
    sw_(prev + 0, norrath_host_address(prev_header)); sf_(prev + 24, 2.0f);
    sw_(prev_header + 4, (unsigned)-3);
    sw_(comp + 4, 0);
    std::memcpy(want, state, N);
    want[4] = 2; sh_(want + 6, 1);
    w = want + 8 + 32;
    sw_(w + 0, norrath_host_address(header));
    sf_(w + 8, 1.0f); sw_(w + 12, 0); sf_(w + 16, 1.0f); sw_(w + 20, 0);
    sf_(w + 24, 5.0f); sh_(w + 28, 1);
    animAddToEnd__FP14AnimationStateP15AnimationHeaderi(state, header, 1);
    if (same("slot 1", state, want, N)) return 6;
    std::printf("animAddToEnd check passed\n");
    return 0;
}
