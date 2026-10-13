// Host contract for animAddOneShot__FP14AnimationStateP15AnimationHeaderffi. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp
#define NORRATH_HOST_ARENA
#include "../w9-animation-object-a-common/host_arena.hpp"
#include "../w9-animation-object-a-common/host_check.hpp"
#include "anim_add_one_shot.cpp"
static void fill(unsigned char* p, unsigned n) { for (unsigned i = 0; i != n; ++i) p[i] = 0x40 + (i * 7) % 61; }
static float step_of(float rate) { return rate < 1.0f / 60.0f ? 1.0f : 1.0f / (rate * 60.0f); }
int main() {
    unsigned char* state = arena_at(0x100);
    unsigned char* header = arena_at(0x900);
    unsigned char* link = arena_at(0xa00);
    const unsigned N = 8 + 32 * 4;
    unsigned char before[N], want[N];
    int failures = 0;
    auto call = [&](unsigned char* h, float a, float b, int f) {
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(state, h, a, b, f);
    };
    fill(state, N); state[4] = 1; state[5] = 3; std::memcpy(before, state, N);
    call(nullptr, 1, 1, 5);
    if (same("null header", state, before, N)) return 1;

    // Free slot: slot 1, stale link, header length 90.
    fill(header, 16); sw_(header + 4, 90);
    const float cases[][2] = {{0.5f, 2.0f}, {0.01f, 0.0f}, {1.0f / 60.0f, 1.0f / 60.0f}, {0.2f, 0.016f}, {10.0f, 0.3f}};
    for (const auto& c : cases) {
        fill(state, N); state[4] = 1; state[5] = 3; sh_(state + 6, 0x0110);
        unsigned char* comp = state + 8 + 32;
        sw_(comp + 4, norrath_host_address(link));
        fill(link, 64); sw_(link + 20, 0xdeadbeef);
        unsigned char link_before[64]; std::memcpy(link_before, link, 64);
        std::memcpy(want, state, N);
        want[4] = 2; sh_(want + 6, 0x0110 | 0x8003);
        unsigned char* w = want + 8 + 32;
        const float s = step_of(c[1]);
        sw_(w + 0, norrath_host_address(header)); sw_(w + 4, 0); sw_(w + 8, 0);
        sf_(w + 12, step_of(c[0]));
        sf_(w + 16, 90.0f * (1.0f / 60.0f) * 60.0f * s);
        sf_(w + 20, -s);
        sw_(w + 24, 0); sh_(w + 28, 0x8003); sh_(w + 30, 0);
        call(header, c[0], c[1], 0x18003);
        if (same("free slot", state, want, N)) ++failures;
        sw_(link_before + 20, 0);
        if (same("link", link, link_before, 64)) ++failures;
    }
    // Full state: the last slot is reused and its link is cut first.
    fill(state, N); state[4] = 2; state[5] = 2; sh_(state + 6, 0);
    unsigned char* last = state + 8 + 32;
    sw_(last + 4, norrath_host_address(link));
    fill(link, 64); sw_(link + 20, 0x1234);
    std::memcpy(want, state, N);
    want[4] = 2; sh_(want + 6, 1);
    unsigned char* w = want + 8 + 32;
    sw_(w + 0, norrath_host_address(header)); sw_(w + 4, 0); sw_(w + 8, 0);
    sf_(w + 12, step_of(3.0f)); sf_(w + 16, 90.0f * (1.0f / 60.0f) * 60.0f * step_of(4.0f));
    sf_(w + 20, -step_of(4.0f)); sw_(w + 24, 0); sh_(w + 28, 1); sh_(w + 30, 0);
    unsigned char link_before[64]; std::memcpy(link_before, link, 64); sw_(link_before + 20, 0);
    call(header, 3.0f, 4.0f, 1);
    if (same("full state", state, want, N)) ++failures;
    if (same("full state link", link, link_before, 64)) ++failures;
    // Full state with no link in the last slot.
    fill(state, N); state[4] = 1; state[5] = 1; sw_(state + 12, 0);
    std::memcpy(want, state, N);
    want[4] = 1; sh_(want + 6, (hw(state + 6) & 0xffff) | 2);
    w = want + 8;
    sw_(w + 0, norrath_host_address(header)); sw_(w + 4, 0); sw_(w + 8, 0);
    sf_(w + 12, step_of(0.0f)); sf_(w + 16, 90.0f * (1.0f / 60.0f) * 60.0f * step_of(0.0f));
    sf_(w + 20, -step_of(0.0f)); sw_(w + 24, 0); sh_(w + 28, 2); sh_(w + 30, 0);
    call(header, 0.0f, 0.0f, 2);
    if (same("full, no link", state, want, N)) ++failures;
    if (failures) return 2;
    std::printf("animAddOneShot check passed\n");
    return 0;
}
