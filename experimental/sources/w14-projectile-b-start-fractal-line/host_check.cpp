// Host contract for startFractalLine__FP4Boltfi; not an EE input. rand is replaced by a counter. The expected singles were worked
// out separately in double precision from the generator recurrence (state 0); the authored function was also compared with the
// original in a private R5900 run (48 scenarios over depth, starting index, length and generator state; every original word covered).
#include <cmath>
#include <cstring>
extern "C" {
int linePos;
int eRandState;
static int rand_calls;
int rand_stub() { return 0x1d + 8 * ++rand_calls; }
}
#define rand rand_stub
#include "start_fractal_line.cpp"
#undef rand

static bool near(float a, double b) { return std::fabs(a - b) < 2e-3; }
static bool entry_is(const unsigned char* bolt, int k, const double (&e)[6], int tag) {
    float f[6];
    int word;
    std::memcpy(f, bolt + 28 * k, sizeof f);
    std::memcpy(&word, bolt + 28 * k + 24, 4);
    for (int i = 0; i < 6; ++i)
        if (!near(f[i], e[i])) return false;
    return word == tag;
}
int main() {
    alignas(16) static unsigned char bolt[28 * 16];
    std::memset(bolt, 0, sizeof bolt);
    linePos = 0; eRandState = 0; rand_calls = 0;
    startFractalLine__FP4Boltfi(bolt, 64.0f, 0);
    const double first[6] = {-33.7852, -28.3438, 40.8984, 5.5271, 1.3504, 4.3934};
    if (!entry_is(bolt, 0, first, (0x1d + 8) & 7)) return 1;
    if (linePos != 1 || rand_calls != 1 || eRandState == 0) return 2;
    // depth 1: the root, then two children at half the length, consecutive generator states and consecutive entries
    std::memset(bolt, 0, sizeof bolt);
    linePos = 2; eRandState = 0; rand_calls = 0;
    startFractalLine__FP4Boltfi(bolt, 64.0f, 1);
    const double child1[6] = {-10.002, 8.9619, 0.4971, 1.5236, 1.0028, 0.6242};
    const double child2[6] = {7.2842, 24.6465, -31.5684, -0.6341, -1.1353, -3.67};
    if (!entry_is(bolt, 2, first, (0x1d + 8) & 7) || !entry_is(bolt, 3, child1, (0x1d + 16) & 7) || !entry_is(bolt, 4, child2, (0x1d + 24) & 7)) return 3;
    if (linePos != 5 || rand_calls != 3) return 4;
    // depth 3 makes 15 entries
    linePos = 0; rand_calls = 0;
    startFractalLine__FP4Boltfi(bolt, 8.0f, 3);
    if (linePos != 15 || rand_calls != 15) return 5;
    return 0;
}
