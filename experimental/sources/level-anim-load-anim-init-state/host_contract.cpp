#include "anim_init_state.cpp"
#include "../level-anim-load-common/host_check.hpp"
int main() {
    unsigned char state[16], want[16];
    for (int i = 0; i != 16; ++i) state[i] = want[i] = 0xa0 + i;
    want[4] = 0;
    animInitState__FP14AnimationState(state);
    return same("state", state, want, 16);
}
