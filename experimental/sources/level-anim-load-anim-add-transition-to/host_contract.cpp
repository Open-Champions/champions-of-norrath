#include "anim_add_transition_to.cpp"
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
static void fill(unsigned char* p, unsigned n) { for (unsigned i = 0; i != n; ++i) p[i] = 0x40 + (i * 7) % 61; }
static unsigned char* g_state; static unsigned char* g_header;
static int run(const char* what, unsigned char count, unsigned char cap, float rate, int flags,
               unsigned char* link, int expect_index, float expect_step,
               bool clear_last_link) {
    unsigned char want[8 + 32 * 4];
    fill(g_state, sizeof want); g_state[4] = count; g_state[5] = cap; sh_(g_state + 6, 0x0021);
    for (int s = 0; s != 4; ++s) sw_(g_state + 8 + 32 * s + 4, 0);
    if (link) {
        const int slot = clear_last_link ? count - 1 : expect_index;
        sw_(g_state + 8 + 32 * slot + 4, norrath_host_address(link));
    }
    std::memcpy(want, g_state, sizeof want);
    want[4] = expect_index + 1; sh_(want + 6, 0x0021 | (flags & 0xffff));
    unsigned char* w = want + 8 + 32 * expect_index;
    sw_(w + 0, norrath_host_address(g_header)); sw_(w + 4, 0); sw_(w + 8, 0);
    sf_(w + 12, expect_step); sf_(w + 16, 1.0f); sw_(w + 20, 0);
    sw_(w + 24, 0); sh_(w + 28, flags & 0xffff); sh_(w + 30, 0);
    if (clear_last_link && link) sw_(want + 8 + 32 * (count - 1) + 4, 0);
    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(g_state, g_header, rate, flags);
    if (same(what, g_state, want, sizeof want)) return 1;
    if (link && (hw(link + 20) != 0)) { std::printf("%s: link +20 not cleared\n", what); return 1; }
    return 0;
}
int main() {
    g_state = arena_at(0x100); g_header = arena_at(0x900);
    unsigned char* link = arena_at(0xa00);
    // Null header: no stores.
    fill(g_state, 8 + 32 * 4); g_state[4] = 1; g_state[5] = 3;
    unsigned char before[8 + 32 * 4]; std::memcpy(before, g_state, sizeof before);
    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(g_state, nullptr, 0.3f, 2);
    if (same("null", g_state, before, sizeof before)) return 1;
    for (int pass = 0; pass != 1; ++pass) {
        // First slot: the rate is forced to 0.0, so the step is 1.0.
        if (run("first slot", 0, 3, 500.0f, 0x12345, nullptr, 0, 1.0f, false)) return 2;
        // Second slot, ordinary rate below 435: step stays 1.0.
        if (run("below threshold", 1, 3, 0.3f, 4, nullptr, 1, 1.0f, false)) return 3;
        // Rate at and above the literal divides.
        if (run("at threshold", 1, 3, 435.0f, 4, nullptr, 1, 1.0f / (435.0f * 60.0f), false)) return 4;
        if (run("above threshold", 2, 3, 600.0f, 8, nullptr, 2, 1.0f / (600.0f * 60.0f), false)) return 5;
        // A stale link in the slot being filled is cleared.
        fill(link, 64); sw_(link + 20, 1);
        if (run("stale link", 1, 3, 1000.0f, 8, link, 1, 1.0f / (1000.0f * 60.0f), false)) return 6;
        // Full state reuses the last slot and releases its link.
        fill(link, 64); sw_(link + 20, 1);
        if (run("full, last slot", 3, 3, 800.0f, 2, link, 2, 1.0f / (800.0f * 60.0f), true)) return 7;
        // Full with capacity 1 reuses slot 0, where the rate is forced to 0.
        if (run("full, capacity one", 1, 1, 900.0f, 2, nullptr, 0, 1.0f, false)) return 8;
    }
    return 0;
}
