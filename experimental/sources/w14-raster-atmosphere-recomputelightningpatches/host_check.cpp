// Host check only; not a build input. rand, RotateHPR, the vector product and the matrix destructor are stubbed and log their calls.
#include "recompute.cpp"
#include <cstdio>
#include <cstring>
static int randCalls, rotCalls, mulCalls, dtorCalls;
static int script[64];
static float anglesSeen[14][3];
static float matrixSeen[14][16];
static int dtorFlags[14];
extern "C" int rand(void) { return script[randCalls++ % 64]; }
extern "C" void RotateHPR__10VIMatrix44RC7VIVect3(float* m, const float* a) {
    std::memcpy(matrixSeen[rotCalls], m, 64);
    for (int i = 0; i < 3; ++i) anglesSeen[rotCalls][i] = a[i];
    ++rotCalls;
}
extern "C" void Mul__C10VIMatrix44RC7VIVect4R7VIVect4(const float*, const float* in, float* out) {
    // the input must be (0, 100, 0, 1)
    if (in[0] != 0.0f || in[1] != 100.0f || in[2] != 0.0f || in[3] != 1.0f) randCalls = -1000;
    out[0] = 1.0f * mulCalls; out[1] = 2.0f; out[2] = 3.0f; out[3] = -9.0f;
    ++mulCalls;
}
extern "C" void _$_10VIMatrix44(float*, int flags) { dtorFlags[dtorCalls++] = flags; }
int main() {
    for (int i = 0; i < 64; ++i) script[i] = 137 * (i % 200) + 131 * (i % 3);
    alignas(16) static unsigned char atmosphere[4300];
    std::memset(atmosphere, 0x77, sizeof atmosphere);
    RecomputeLightningPatches__12VIAtmosphere(atmosphere);
    const float pi = __builtin_bit_cast(float, 0x40490fdau);
    bool ok = randCalls == 56 && rotCalls == 14 && mulCalls == 14 && dtorCalls == 14;
    const float* patches = reinterpret_cast<const float*>(atmosphere + 3936);
    for (int i = 0; i < 14 && ok; ++i) {
        // rand call order within a pass: r1 r2 r3 r4 at script[4*i .. 4*i+3]
        int r1 = script[(4 * i) % 64], r2 = script[(4 * i + 1) % 64], r3 = script[(4 * i + 2) % 64], r4 = script[(4 * i + 3) % 64];
        float h = static_cast<float>(128 - ((r1 / 137) & 0xff)) * pi * __builtin_bit_cast(float, 0x3b666666u);
        float p = static_cast<float>(128 - ((r2 / 137) & 0xff)) * pi * __builtin_bit_cast(float, 0x3b23d70au);
        float q = static_cast<float>(128 - ((r3 / 137) & 0xff)) * pi * __builtin_bit_cast(float, 0x3b23d70au);
        float w = static_cast<float>((r4 / 131) & 0xff) * __builtin_bit_cast(float, 0x3b808080u) * 20.0f + 70.0f;
        ok = ok && anglesSeen[i][0] == h && anglesSeen[i][1] == p && anglesSeen[i][2] == q && dtorFlags[i] == 2;
        for (int k = 0; k < 16; ++k) ok = ok && matrixSeen[i][k] == 0.0f;       // the matrix is zero when RotateHPR is called
        ok = ok && patches[4 * i] == 1.0f * i && patches[4 * i + 1] == 2.0f && patches[4 * i + 2] == 3.0f && patches[4 * i + 3] == w;
    }
    ok = ok && atmosphere[3935] == 0x77 && atmosphere[3936 + 224] == 0x77;      // exactly 224 bytes written
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
