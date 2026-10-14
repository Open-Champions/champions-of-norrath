// Host contract for animGlobalSync__FP14AnimationState. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp
#define NORRATH_HOST_ARENA
#include <cmath>
#include <vector>
#include "../w9-animation-object-a-common/host_arena.hpp"
#include "../w9-animation-object-a-common/host_check.hpp"
extern "C" int frameNm = 0;
static std::vector<unsigned char*> updated;
static std::vector<std::pair<float, float>> fmod_calls;
static float seen_at_update;
extern "C" float fmodf(float a, float b) { fmod_calls.push_back({a, b}); return (float)std::fmod((double)a, (double)b); }
extern "C" void updateAnimation__FP13animComponent(unsigned char* c) { updated.push_back(c); seen_at_update = lf_(c + 24); }
#include "anim_global_sync.cpp"
static void fill(unsigned char* p, unsigned n) { for (unsigned i = 0; i != n; ++i) p[i] = 0x40 + (i * 7) % 61; }
int main() {
    unsigned char* state = arena_at(0x100);
    unsigned char* h0 = arena_at(0x900);
    unsigned char* h1 = arena_at(0x980);
    unsigned char* l0 = arena_at(0xa00);
    unsigned char* l1 = arena_at(0xa80);
    const unsigned N = 8 + 32 * 3;
    unsigned char before[N];
    int failures = 0;
    // Count zero or negative: nothing is called or stored.
    for (unsigned c : {0u, 0x80u, 0xffu}) {
        fill(state, N); state[4] = c; std::memcpy(before, state, N);
        updated.clear(); fmod_calls.clear();
        animGlobalSync__FP14AnimationState(state);
        if (same("empty", state, before, N) || !updated.empty() || !fmod_calls.empty()) { std::printf("empty %u\n", c); ++failures; }
    }
    // Two components, only the second flagged 0x800.
    frameNm = 1000;
    fill(state, N); state[4] = 2;
    fill(h0, 16); sw_(h0 + 4, 90);
    fill(h1, 16); sw_(h1 + 4, 400);
    unsigned char* c0 = state + 8; unsigned char* c1 = state + 8 + 32;
    sw_(c0 + 0, norrath_host_address(h0)); sw_(c0 + 4, 0); sh_(c0 + 28, 0x07ff);
    sw_(c1 + 0, norrath_host_address(h1)); sw_(c1 + 4, norrath_host_address(l1)); sh_(c1 + 28, 0x0800);
    fill(l0, 64); fill(l1, 64);
    sh_(l1 + 28, (unsigned)-5); sf_(l1 + 32, 3.0f); sf_(l1 + 36, -2.0f); sf_(l1 + 44, 0.5f); sf_(l1 + 48, 0.25f);
    unsigned char l0_before[64]; std::memcpy(l0_before, l0, 64);
    std::memcpy(before, state, N);
    updated.clear(); fmod_calls.clear();
    animGlobalSync__FP14AnimationState(state);
    const float now = 1000.0f * (1.0f / 60.0f) * 60.0f;
    sf_(before + 8 + 24, (float)std::fmod((double)now, 90.0));
    const float t1 = (float)std::fmod((double)now, 400.0);
    sf_(before + 8 + 32 + 24, t1);
    if (same("components", state, before, N)) ++failures;
    if (same("unlinked record untouched", l0, l0_before, 64)) ++failures;
    if (fmod_calls.size() != 2 || fmod_calls[0] != std::make_pair(now, 90.0f) || fmod_calls[1] != std::make_pair(now, 400.0f)) { std::printf("fmod calls\n"); ++failures; }
    if (updated.size() != 1 || updated[0] != c1) { std::printf("updates\n"); ++failures; }
    if (seen_at_update != t1) { std::printf("update before time store\n"); ++failures; }
    const float tt = t1 - (-5.0f);
    if (lf_(l1 + 36) != -2.0f - (-2.0f + 0.25f * tt)) { std::printf("link+36 %f\n", lf_(l1 + 36)); ++failures; }
    if (lf_(l1 + 32) != 3.0f - (3.0f + 0.5f * tt)) { std::printf("link+32 %f\n", lf_(l1 + 32)); ++failures; }
    // Flag test is on bit 0x0800 only.
    frameNm = 7; fill(state, N); state[4] = 1;
    sw_(state + 8, norrath_host_address(h0)); sh_(state + 8 + 28, 0xf7ff);
    updated.clear();
    animGlobalSync__FP14AnimationState(state);
    if (!updated.empty()) { std::printf("flag mask\n"); ++failures; }
    if (failures) return 1;
    std::printf("animGlobalSync check passed\n");
    return 0;
}
