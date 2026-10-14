// Host contract for animSetToEndFullyBlended__FP14AnimationState. Not an EE plan input.
#include "anim_set_to_end_fully_blended.cpp"
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
int main() {
    unsigned char* state = arena_at(0x100);
    unsigned char* result = arena_at(0x1000);
    unsigned char* header = arena_at(0x900);
    unsigned char want[8 + 32 * 4];
    auto reset = [&] {
        for (unsigned i = 0; i != sizeof want; ++i) state[i] = 0x10 + i;
        for (unsigned i = 0; i != 1600; ++i) result[i] = 0x33;
        sw_(header + 4, 0xfffffff0u);
    };
    // No result record, no components: nothing changes.
    reset(); state[4] = 0; sw_(state + 0, 0);
    std::memcpy(want, state, sizeof want);
    animSetToEndFullyBlended__FP14AnimationState(state);
    if (same("nothing", state, want, sizeof want)) return 1;
    // A result record is released even with no components.
    reset(); state[4] = 0; sw_(state + 0, norrath_host_address(result));
    std::memcpy(want, state, sizeof want); sw_(want, 0);
    animSetToEndFullyBlended__FP14AnimationState(state);
    if (same("release", state, want, sizeof want)) return 2;
    if (hw(result + 1552) != 0 || result[1551] != 0x33 || result[1556] != 0x33) return 3;
    // Three components: only the last changes (count byte 3 -> index 2).
    reset(); state[4] = 3; sw_(state + 0, 0);
    unsigned char* comp = state + 8 + 64;
    sw_(comp, norrath_host_address(header));
    std::memcpy(want, state, sizeof want);
    sf_(want + 8 + 64 + 16, 1.0f); sf_(want + 8 + 64 + 8, 1.0f); sf_(want + 8 + 64 + 24, -16.0f);
    animSetToEndFullyBlended__FP14AnimationState(state);
    if (same("last", state, want, sizeof want)) return 4;
    std::puts("animSetToEndFullyBlended contract passed");
    return 0;
}
