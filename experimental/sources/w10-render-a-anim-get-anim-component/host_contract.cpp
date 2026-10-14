// Host contract for animGetAnimComponent__FP14AnimationStateP15AnimationHeader. Not an EE plan input.
#include "anim_get_anim_component.cpp"
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
int main() {
    unsigned char* state = arena_at(0x100);
    unsigned char* h1 = arena_at(0x900);
    unsigned char* h2 = arena_at(0xa00);
    unsigned char* h3 = arena_at(0xb00);
    for (unsigned i = 0; i != 8 + 32 * 5; ++i) state[i] = 0;
    sw_(state + 8, norrath_host_address(h1));
    sw_(state + 8 + 32, norrath_host_address(h2));
    sw_(state + 8 + 64, norrath_host_address(h1));   // duplicate of component 0
    sw_(state + 8 + 96, norrath_host_address(h3));   // beyond the count
    state[4] = 3;
    auto get = [&](unsigned char* s, unsigned char* h) { return animGetAnimComponent__FP14AnimationStateP15AnimationHeader(s, h); };
    if (get(state, h1) != state + 8) return 1;
    if (get(state, h2) != state + 40) return 2;
    if (get(state, h3) != nullptr) return 3;
    if (get(nullptr, h1) != nullptr || get(state, nullptr) != nullptr) return 4;
    state[4] = 0; if (get(state, h1) != nullptr) return 5;
    state[4] = 0xff; if (get(state, h1) != nullptr) return 6;       // signed -1
    state[4] = 4; if (get(state, h3) != state + 8 + 96) return 7;
    std::puts("animGetAnimComponent contract passed");
    return 0;
}
