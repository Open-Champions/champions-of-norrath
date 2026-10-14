#include "anim_set_to_end.cpp"
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
static int updates; static unsigned char* updated;
extern "C" void updateAnimation__FP13animComponent(unsigned char* component) { ++updates; updated = component; }
int main() {
    unsigned char* state = arena_at(0x100);
    unsigned char* result = arena_at(0x1000);
    unsigned char* header = arena_at(0x900);
    unsigned char* link = arena_at(0xa00);
    unsigned char want[8 + 32 * 4];
    auto reset = [&] {
        for (unsigned i = 0; i != sizeof want; ++i) state[i] = 0x10 + i;
        for (unsigned i = 0; i != 1600; ++i) result[i] = 0x33;
        for (unsigned i = 0; i != 64; ++i) link[i] = 0x55;
        sw_(header + 4, 0xfffffff0u);   // -16 frames
        updates = 0;
    };
    // Zero components: only the result pointer is released.
    reset(); state[4] = 0; sw_(state + 0, norrath_host_address(result));
    std::memcpy(want, state, sizeof want); sw_(want + 0, 0);
    animSetToEnd__FP14AnimationState(state);
    if (same("empty", state, want, sizeof want) || updates) return 1;
    if (hw(result + 1552) != 0 || result[1551] != 0x33 || result[1556] != 0x33) return 2;
    // One component without the 0x800 flag: only float +24 is stored.
    reset(); state[4] = 2; sw_(state + 0, 0);
    unsigned char* comp = state + 8 + 32;
    sw_(comp + 0, norrath_host_address(header)); sh_(comp + 28, 0x7ff);
    std::memcpy(want, state, sizeof want); sf_(want + 8 + 32 + 24, -16.0f);
    animSetToEnd__FP14AnimationState(state);
    if (same("no flag", state, want, sizeof want) || updates) return 3;
    // With the flag: update runs on the last component and the link is adjusted.
    reset(); sw_(header + 4, 100); state[4] = 2; sw_(state + 0, 0);
    comp = state + 8 + 32;
    sw_(comp + 0, norrath_host_address(header)); sh_(comp + 28, 0x0800); sw_(comp + 4, norrath_host_address(link));
    sh_(link + 28, 0xfffb);        // -5
    sf_(link + 32, 10.0f); sf_(link + 36, 20.0f); sf_(link + 44, 0.5f); sf_(link + 48, 0.25f);
    unsigned char link_want[64]; std::memcpy(link_want, link, 64);
    // time = 100, d = 105, moved36 = 20 + 0.25*105, moved32 = 10 + 0.5*105
    sf_(link_want + 36, 20.0f - (20.0f + 0.25f * 105.0f));
    sf_(link_want + 32, 10.0f - (10.0f + 0.5f * 105.0f));
    std::memcpy(want, state, sizeof want); sf_(want + 8 + 32 + 24, 100.0f);
    animSetToEnd__FP14AnimationState(state);
    if (same("flagged state", state, want, sizeof want)) return 4;
    if (updates != 1 || updated != comp) return 5;
    if (same("link", link, link_want, 64)) return 6;
    // The first component is untouched when the count is two.
    return 0;
}
