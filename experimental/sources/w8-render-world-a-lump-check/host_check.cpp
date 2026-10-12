#define NORRATH_W8_HOST 1
#include <cstdio>
unsigned norrath_w8_lump_check_steps;
extern "C" int nmLumps;
int nmLumps;
#include "lump_check.cpp"
static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
static unsigned run(int count) {
    nmLumps = count;
    norrath_w8_lump_check_steps = 0;
    lumpCheck__Fv();
    expect(nmLumps == count, "nmLumps is not modified");
    return norrath_w8_lump_check_steps;
}
int main() {
    expect(run(0) == 0, "zero lumps: no iterations");
    expect(run(-3) == 0, "negative count: no iterations");
    expect(run(1) == 1, "one lump: one iteration");
    expect(run(37) == 37, "37 lumps: 37 iterations");
    return failures ? 1 : 0;
}
