#include <cstdio>
#include <cstring>
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
extern "C" {
static int resetCalls;
static unsigned char* resetArg;
void animResetFramestates__FP14AnimationState(unsigned char* s) { ++resetCalls; resetArg = s; }
void animRemove__FP14AnimationStateP15AnimationHeaderf(unsigned char* state, unsigned char* header, float t);
}
int main() {
    unsigned char* st = arena_at(0x100);
    unsigned char* hdr = arena_at(0x2000);
    unsigned char* other = arena_at(0x2100);
    auto comp = [&](int i) { return st + 8 + i * 32; };
    auto build = [&] {
        std::memset(st, 0, 400);
        st[4] = 4;
        for (int i = 0; i != 4; ++i) {
            sw_(comp(i), norrath_host_address(i == 1 || i == 2 ? hdr : other));
            sf_(comp(i) + 8, 2.0f); sf_(comp(i) + 16, 3.0f); sw_(comp(i) + 20, 0x99); sh_(comp(i) + 28, 0xffff);
            sw_(comp(i) + 24, 100 + i);
        }
        resetCalls = 0;
    };
    // t == 0: both matching components are removed (the second is examined again after the shift).
    build();
    animRemove__FP14AnimationStateP15AnimationHeaderf(st, hdr, 0.0f);
    if (st[4] != 2 || resetCalls != 2 || resetArg != st) return 1;
    if (hw(comp(0) + 24) != 100 || hw(comp(1) + 24) != 103) return 2;
    if (hw(comp(1)) != norrath_host_address(other)) return 3;
    // t != 0: components stay, are clamped and faded.
    build();
    animRemove__FP14AnimationStateP15AnimationHeaderf(st, hdr, 0.5f);
    if (st[4] != 4 || resetCalls != 0) return 4;
    for (int i = 1; i != 3; ++i) {
        if (lf_(comp(i) + 8) != 1.0f || lf_(comp(i) + 16) != 1.0f) return 5;
        if (lf_(comp(i) + 12) != -(1.0f / 30.0f)) return 6;
        if (hw(comp(i) + 20) != 0 || (hw(comp(i) + 28) & 0xffff) != 0xeffb) return 7;
    }
    if (lf_(comp(0) + 8) != 2.0f || hw(comp(0) + 20) != 0x99 || (hw(comp(0) + 28) & 0xffff) != 0xffff) return 8;
    // Small t (below 1/60) uses rate 1.
    build();
    animRemove__FP14AnimationStateP15AnimationHeaderf(st, hdr, 0.001f);
    if (lf_(comp(1) + 12) != -1.0f) return 9;
    // Values already below the clamp are kept.
    build();
    sf_(comp(1) + 8, 0.25f); sf_(comp(1) + 16, -4.0f);
    animRemove__FP14AnimationStateP15AnimationHeaderf(st, hdr, 2.0f);
    if (lf_(comp(1) + 8) != 0.25f || lf_(comp(1) + 16) != -4.0f || lf_(comp(1) + 12) != -(1.0f / 120.0f)) return 10;
    // A null header and a non-positive count do nothing.
    build();
    animRemove__FP14AnimationStateP15AnimationHeaderf(st, nullptr, 0.0f);
    if (st[4] != 4 || resetCalls != 0) return 11;
    st[4] = 0;
    animRemove__FP14AnimationStateP15AnimationHeaderf(st, hdr, 0.0f);
    if (resetCalls != 0) return 12;
    std::puts("animRemove contract passed");
    return 0;
}
