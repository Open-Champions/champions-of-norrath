// Host contract for bFarFromHome__8Creature. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp
#define NORRATH_HOST_ARENA
#include "../w9-animation-object-a-common/host_arena.hpp"
#include "../w9-animation-object-a-common/host_check.hpp"
#include "b_far_from_home.cpp"
static int run(unsigned char* c, float px, float py, float pz, float hx, float hy, float hz, float r) {
    sf_(c + 16, px); sf_(c + 20, py); sf_(c + 24, pz);
    sf_(c + 776, hx); sf_(c + 780, hy); sf_(c + 784, hz); sf_(c + 788, r);
    unsigned char before[1024];
    std::memcpy(before, c, 1024);
    const int result = bFarFromHome__8Creature(c);
    if (same("no stores", c, before, 1024)) return -1;
    return result;
}
int main() {
    unsigned char* c = arena_at(0x100);
    std::memset(c, 0x11, 1024);
    int failures = 0;
    auto expect = [&](const char* what, int got, int want) {
        if (got != want) { std::printf("%s: got %d want %d\n", what, got, want); ++failures; }
    };
    expect("at home", run(c, 1, 2, 3, 1, 2, 3, 5), 0);
    expect("inside", run(c, 1, 2, 3, 2, 3, 4, 5), 0);
    // 3-4-5 triangle in x/y: exactly on the radius is not far (strict compare).
    expect("on radius", run(c, 3, 4, 0, 0, 0, 0, 5), 0);
    expect("just outside", run(c, 3, 4.01f, 0, 0, 0, 0, 5), 1);
    expect("z only", run(c, 0, 0, 6, 0, 0, 0, 5), 1);
    expect("z inside", run(c, 0, 0, 4, 0, 0, 0, 5), 0);
    expect("y only", run(c, 0, -7, 0, 0, 0, 0, 5), 1);
    expect("x negative", run(c, -6, 0, 0, 0, 0, 0, 5), 1);
    expect("home offset", run(c, 101, 102, 103, 100, 100, 100, 4), 0);
    expect("home offset far", run(c, 110, 100, 100, 100, 100, 100, 3), 1);
    // The radius is squared, so a negative radius behaves like its magnitude.
    expect("negative radius", run(c, 3, 0, 0, 0, 0, 0, -2), 1);
    expect("zero radius at home", run(c, 0, 0, 0, 0, 0, 0, 0), 0);
    expect("zero radius off home", run(c, 0.5f, 0, 0, 0, 0, 0, 0), 1);
    if (failures) return 1;
    std::printf("bFarFromHome check passed\n");
    return 0;
}
