// Host contract for animRemoveNonpermanentAnimations__FP14AnimationState. Not an EE plan input.
#include "anim_remove_nonpermanent.cpp"
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
int main() {
    unsigned char* state = arena_at(0x100);
    unsigned char* link[5];
    for (int i = 0; i != 5; ++i) link[i] = arena_at(0x1000 + 0x100 * i);
    for (unsigned i = 0; i != 8 + 32 * 5; ++i) state[i] = 0;
    for (int i = 0; i != 5; ++i) for (int j = 0; j != 64; ++j) link[i][j] = 0x77;
    // Components 0..4: permanence 0,1,0,1,1; each has a distinct header and link.
    const unsigned flags[5] = {0x0000, 0x1000, 0x0800, 0x1800, 0x1001};
    for (int i = 0; i != 5; ++i) {
        unsigned char* c = state + 8 + 32 * i;
        sw_(c, 0xaa00 + i);
        sw_(c + 4, norrath_host_address(link[i]));
        sh_(c + 28, flags[i]);
        for (int j = 8; j != 28; ++j) c[j] = 0x40 + i;
    }
    state[4] = 5;
    unsigned char before[8 + 32 * 5]; std::memcpy(before, state, sizeof before);
    animRemoveNonpermanentAnimations__FP14AnimationState(state);
    if (state[4] != 3) return 1;
    // Kept order: 1, 3, 4 packed into slots 0, 1, 2.
    const int from[3] = {1, 3, 4};
    for (int k = 0; k != 3; ++k) {
        unsigned char* c = state + 8 + 32 * k;
        if (hw(c) != 0xaa00u + from[k] || hw(c + 28) != flags[from[k]] || c[10] != 0x40 + from[k]) return 2;
        if (hw(c + 4) != norrath_host_address(link[from[k]])) return 3;
        if (hw(link[from[k]] + 20) != norrath_host_address(c)) return 4;
    }
    // Dropped components 0 and 2: link back pointers cleared, and their slot +4 words
    // read before overwriting. Slot 2 now holds component 4, so check link[2] and link[0].
    if (hw(link[0] + 20) != 0 || hw(link[2] + 20) != 0) return 5;
    if (link[0][19] != 0x77 || link[0][24] != 0x77) return 6;
    // Slot 1's old image held component 1 whose +4 word was cleared when it moved to slot 0.
    // Untouched tail: slot 3 (original component 3) keeps its +4 word? It moved, so zero.
    if (hw(state + 8 + 96 + 4) != 0 || hw(state + 8 + 32 + 4) != norrath_host_address(link[3])) return 7;
    // Slot 4 (original component 4) moved from index 4 to 2: its source +4 word is cleared.
    if (hw(state + 8 + 128 + 4) != 0) return 8;
    // Nothing kept: count goes to zero and links are all released.
    for (int i = 0; i != 5; ++i) { sh_(state + 8 + 32 * i + 28, 0); sw_(state + 8 + 32 * i + 4, norrath_host_address(link[i])); sw_(link[i] + 20, 0x5555); }
    state[4] = 5;
    animRemoveNonpermanentAnimations__FP14AnimationState(state);
    if (state[4] != 0) return 9;
    for (int i = 0; i != 5; ++i) if (hw(link[i] + 20) != 0) return 10;
    // Nothing dropped: nothing moves and no link is rewritten.
    for (int i = 0; i != 5; ++i) { sh_(state + 8 + 32 * i + 28, 0x1000); sw_(state + 8 + 32 * i + 4, norrath_host_address(link[i])); sw_(link[i] + 20, 0x5555); }
    state[4] = 5; std::memcpy(before, state, sizeof before);
    animRemoveNonpermanentAnimations__FP14AnimationState(state);
    if (same("all kept", state, before, sizeof before)) return 11;
    for (int i = 0; i != 5; ++i) if (hw(link[i] + 20) != 0x5555) return 12;
    std::puts("animRemoveNonpermanentAnimations contract passed");
    return 0;
}
