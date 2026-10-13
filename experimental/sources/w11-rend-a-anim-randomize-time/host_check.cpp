#include <cstdio>
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
extern "C" {
int eRandState;
static int updateCalls;
static unsigned char* updateArg;
void updateAnimation__FP13animComponent(unsigned char* c) { ++updateCalls; updateArg = c; }
void animRandomizeTime__FP14AnimationState(unsigned char* state);
}
int main() {
    unsigned char* state = arena_at(0x100);
    unsigned char* header = arena_at(0x1000);
    unsigned char* p = arena_at(0x2000);
    // Count zero: nothing happens, the generator is not advanced.
    eRandState = 7;
    animRandomizeTime__FP14AnimationState(state);
    if (eRandState != 7 || updateCalls != 0) return 1;
    // One component without bit 0x800: only the generator and component+24 change.
    state[4] = 1;
    sw_(state + 8, norrath_host_address(header));
    sw_(state + 12, norrath_host_address(p));
    sw_(header + 4, 1000);
    sh_(state + 8 + 28, 0x7ff);
    eRandState = 12345;
    animRandomizeTime__FP14AnimationState(state);
    unsigned next = 12345u * 1664525u + 1013904223u;
    if ((unsigned)eRandState != next) return 2;
    float expect = (float)(int)(next >> 16) * (1.0f / 65536.0f) * 1000.0f;
    if (lf_(state + 8 + 24) != expect) return 3;
    if (updateCalls != 0) return 4;
    // Component 3 of 3 with bit 0x800: updateAnimation runs and the offsets are re-derived.
    state[4] = 3;
    unsigned char* comp = state + 8 + 2 * 32;
    sw_(comp, norrath_host_address(header));
    sw_(comp + 4, norrath_host_address(p));
    sh_(comp + 28, 0x800);
    sh_(p + 28, 10);
    sf_(p + 32, 5.0f); sf_(p + 36, 7.0f); sf_(p + 44, 0.5f); sf_(p + 48, 2.0f);
    animRandomizeTime__FP14AnimationState(state);
    if (updateCalls != 1 || updateArg != comp) return 5;
    float t = lf_(comp + 24);
    float d = t - 10.0f;
    float a = 7.0f, b = 5.0f;
    if (lf_(p + 36) != a - (a + 2.0f * d)) return 6;
    if (lf_(p + 32) != b - (b + 0.5f * d)) return 7;
    // The first component was not touched by the third.
    if (lf_(state + 8 + 24) != expect) return 8;
    std::puts("animRandomizeTime contract passed");
    return 0;
}
