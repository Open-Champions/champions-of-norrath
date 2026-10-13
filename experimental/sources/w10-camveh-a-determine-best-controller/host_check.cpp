// Host contract for determineBestController__6Camera. Not an EE plan input.
#include "determine_best_controller.cpp"

#include <cstdio>
#include <cstring>

unsigned char pad[88 * 4];
int localPlayersAlive[4];
int localNmPlayersAlive;

static double asDouble(Double v) { double d; std::memcpy(&d, &v, 8); return d; }
static Double fromDouble(double d) { Double v; std::memcpy(&v, &d, 8); return v; }
Double fptodp(float v) { return fromDouble(v); }
int dpcmp(Double a, Double b) { double x = asDouble(a), y = asDouble(b); return x < y ? -1 : x > y ? 1 : 0; }
Double dpsub(Double a, Double b) { return fromDouble(asDouble(a) - asDouble(b)); }
Double dpadd(Double a, Double b) { return fromDouble(asDouble(a) + asDouble(b)); }
float dptofp(Double a) { return static_cast<float>(asDouble(a)); }

static int failures = 0;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; } }

static void setPad(int k, float x, float y, int flags) {
    std::memcpy(pad + 88 * k + 28, &x, 4);
    std::memcpy(pad + 88 * k + 32, &y, 4);
    std::memcpy(pad + 88 * k + 12, &flags, 4);
}

static int run(int cur, int n, int a, int b, int c) {
    unsigned char camera[128] = {};
    std::memcpy(camera + 68, &cur, 4);
    localNmPlayersAlive = n;
    localPlayersAlive[0] = a; localPlayersAlive[1] = b; localPlayersAlive[2] = c;
    return determineBestController__6Camera(camera);
}

int main() {
    for (int k = 0; k < 4; ++k) setPad(k, 0, 0, 0);
    // The current controller stays when its stick is used or its flag is set.
    setPad(1, 0.5f, 0, 0); setPad(2, 0, 2.0f, 0);
    expect(run(1, 3, 0, 1, 2) == 1, "x at 0.5 keeps current");
    setPad(1, -0.6f, 0, 0); expect(run(1, 3, 0, 1, 2) == 1, "negative x magnitude keeps current");
    setPad(1, 0.49f, 0, 0); setPad(2, 5.0f, 5.0f, 0);
    expect(run(1, 3, 0, 1, 2) == 2, "idle current loses to the busiest");
    setPad(1, 0, 0.9f, 0); expect(run(1, 3, 0, 1, 2) == 1, "y at 0.9 keeps current");
    setPad(1, 0, 0.89f, 0); expect(run(1, 3, 0, 1, 2) == 2, "y at 0.89 does not");
    setPad(1, 0, 0, 0x400); expect(run(1, 3, 0, 1, 2) == 1, "flag keeps current");
    // Current not in the list: nothing is kept.
    setPad(1, 0, 0, 0x400); setPad(0, 0.1f, 0, 0); setPad(2, 0.3f, 0, 0);
    expect(run(1, 2, 0, 2, 0) == 2, "current not listed");
    // Scoring: sum of magnitudes, +0.5 with the flag, strict improvement.
    setPad(0, 0.2f, 0.2f, 0); setPad(1, 0.1f, 0.0f, 0x400); setPad(2, 0.2f, 0.2f, 0);
    expect(run(3, 3, 0, 1, 2) == 1, "flag bonus wins");
    setPad(1, 0.1f, 0.0f, 0); expect(run(3, 3, 0, 1, 2) == 0, "ties keep the first");
    setPad(2, -0.3f, -0.2f, 0); expect(run(3, 3, 0, 1, 2) == 2, "absolute values add");
    // All scores zero: the first listed entry is returned; n == 0 still returns list[0].
    for (int k = 0; k < 4; ++k) setPad(k, 0, 0, 0);
    expect(run(3, 3, 2, 1, 0) == 2, "all zero gives list[0]");
    expect(run(0, 0, 3, 1, 0) == 3, "empty list gives list[0]");
    return failures ? 1 : 0;
}
