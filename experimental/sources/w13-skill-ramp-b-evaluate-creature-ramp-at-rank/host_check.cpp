// Host check. Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp evaluate_creature_ramp_at_rank.cpp
#include <cstdio>
#include <cstring>

extern "C" int evaluate_creature_ramp_at_rank(const void*, int, int) asm("evaluateCreatureRampAtRank__5Skillii");

static int failures = 0;
static void expect(bool ok, const char* what, int got) {
    if (!ok) { std::printf("FAIL %s (got %d)\n", what, got); ++failures; }
}

int main() {
    alignas(8) unsigned char object[128];
    std::memset(object, 0, sizeof object);
    auto set_h = [&](unsigned off, short v) { std::memcpy(object + off, &v, 2); };
    auto set_f = [&](unsigned off, float v) { std::memcpy(object + off, &v, 4); };
    // slot 1: threshold 2, base 10, step 2.5
    set_h(56 + 2, 2); set_h(66 + 2, 10); set_f(76 + 4, 2.5f);
    // slot 3: threshold -1 (negative), base -7, step -1.25
    set_h(56 + 6, -1); set_h(66 + 6, -7); set_f(76 + 12, -1.25f);
    int r = evaluate_creature_ramp_at_rank(object, 1, 0);
    expect(r == 0, "level 0 gives 0", r);
    r = evaluate_creature_ramp_at_rank(object, 1, 1);
    expect(r == 0, "below threshold gives 0", r);
    r = evaluate_creature_ramp_at_rank(object, 1, 2);
    expect(r == 10, "at threshold gives base", r);
    r = evaluate_creature_ramp_at_rank(object, 1, 3);
    expect(r == 13, "10+2.5 rounds up to 13", r);
    r = evaluate_creature_ramp_at_rank(object, 1, 4);
    expect(r == 15, "10+5 = 15", r);
    r = evaluate_creature_ramp_at_rank(object, 3, 0);
    expect(r == 0, "slot 3 level 0 gives 0", r);
    r = evaluate_creature_ramp_at_rank(object, 3, -2);
    expect(r == 0, "level -2 below threshold -1 gives 0", r);
    r = evaluate_creature_ramp_at_rank(object, 3, -1);
    expect(r == -7, "negative base at threshold", r);
    r = evaluate_creature_ramp_at_rank(object, 3, 1);
    expect(r == -10, "-7-2.5 rounds away from zero to -10", r);
    r = evaluate_creature_ramp_at_rank(object, 0, 5);
    expect(r == 0, "all-zero slot gives 0", r);
    unsigned char copy[128];
    std::memset(copy, 0, sizeof copy);
    set_h(56 + 2, 2); set_h(66 + 2, 10); set_f(76 + 4, 2.5f);
    set_h(56 + 6, -1); set_h(66 + 6, -7); set_f(76 + 12, -1.25f);
    std::memcpy(copy, object, sizeof copy);
    (void)evaluate_creature_ramp_at_rank(object, 1, 9);
    expect(std::memcmp(copy, object, sizeof copy) == 0, "object is not written", 0);
    if (!failures) std::printf("ok\n");
    return failures ? 1 : 0;
}
