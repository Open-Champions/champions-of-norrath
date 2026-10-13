// Host contract for animPlaySound__FPvP6Point3P9animSoundi. Not an EE plan input.
// Build with -ffp-contract=off. The generator steps are repeated here independently.
#include <cmath>
#include <cstdio>
#include <cstring>
struct Point3 { float x, y, z; };
extern "C" {
int eRandState;
float pitchTab[130];
float groupVolumeScale[300];
static int calls; static void* seenSound; static Point3* seenPos; static float seenPitch, seenVolume, seenDelay;
void soundPlaySound__FP10_vagHeaderP6Point3fff(void* s, Point3* p, float pitch, float vol, float delay) {
    ++calls; seenSound = s; seenPos = p; seenPitch = pitch; seenVolume = vol; seenDelay = delay;
}
void animPlaySound__FPvP6Point3P9animSoundi(void*, Point3*, const unsigned char*, int);
}
static int rng(unsigned& s) { s = s * 0x19660du + 0x3c6ef35fu; return static_cast<int>(s >> 16); }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    for (int i = 0; i < 130; ++i) pitchTab[i] = 0.5f + 0.01f * i;
    for (int i = 0; i < 300; ++i) groupVolumeScale[i] = 1.0f + i;
    unsigned char rec[32];
    Point3 pos{1, 2, 3};
    int dummy;
    void* snd = &dummy;
    auto set = [&](int b10, int b11, int b12, int b13, int b14) {
        std::memset(rec, 0, sizeof rec);
        rec[10] = static_cast<unsigned char>(b10); rec[11] = static_cast<unsigned char>(b11);
        rec[12] = static_cast<unsigned char>(b12); rec[13] = static_cast<unsigned char>(b13);
        rec[14] = static_cast<unsigned char>(b14);
    };
    // null sound does nothing and draws no random numbers
    set(0, 5, 5, 5, 5); eRandState = 77; calls = 0;
    animPlaySound__FPvP6Point3P9animSoundi(nullptr, &pos, rec, 0);
    expect(calls == 0 && eRandState == 77, "null sound");
    // no randomness: centre values
    set(0, 0, 0, 0, 0);
    animPlaySound__FPvP6Point3P9animSoundi(snd, &pos, rec, 3);
    expect(calls == 1 && seenSound == snd && seenPos == &pos, "arguments forwarded");
    expect(seenPitch == pitchTab[64] && seenVolume == pitchTab[64] * 4.0f && seenDelay == 0.0f && eRandState == 77, "centre values");
    set(10, 0, -20, 0, 0);
    animPlaySound__FPvP6Point3P9animSoundi(snd, &pos, rec, 0);
    expect(seenPitch == pitchTab[74] && seenVolume == pitchTab[64 - 40] * 1.0f, "signed bytes");
    // clamping on both ends
    set(127, 0, 127, 0, 0);
    animPlaySound__FPvP6Point3P9animSoundi(snd, &pos, rec, 0);
    expect(seenPitch == pitchTab[127] && seenVolume == pitchTab[127], "upper clamp");
    set(-128, 0, -128, 0, 0);
    animPlaySound__FPvP6Point3P9animSoundi(snd, &pos, rec, 0);
    expect(seenPitch == pitchTab[0] && seenVolume == pitchTab[0], "lower clamp");
    // all three random draws, in the documented order
    set(4, 6, 8, 3, 9);
    eRandState = 123456; unsigned s = 123456u;
    int d1 = rng(s) % (3 * 4 + 1) - 3 * 2;
    int d2 = rng(s) % (6 * 16) - 6 * 8;
    int d3 = rng(s) % (9 + 1);
    animPlaySound__FPvP6Point3P9animSoundi(snd, &pos, rec, 2);
    float wantPitch = pitchTab[68] * powf(1.0594630241394043f, static_cast<float>(d2) * 0.125f);
    expect(seenPitch == wantPitch, "random pitch");
    expect(seenVolume == pitchTab[8 * 2 + d1 + 64] * 3.0f, "random volume");
    expect(seenDelay == static_cast<float>(d3) * (1.0f / 60.0f) || std::fabs(seenDelay - d3 / 60.0f) < 1e-6f, "random delay");
    expect(static_cast<unsigned>(eRandState) == s, "generator state after three draws");
    // only the draws whose guards hold are taken
    set(0, 0, 0, 2, 0);
    eRandState = 99; s = 99u; (void)rng(s);
    animPlaySound__FPvP6Point3P9animSoundi(snd, &pos, rec, 0);
    expect(static_cast<unsigned>(eRandState) == s, "one draw when only b13 is set");
    set(0, 0, 0, -1, -3);
    eRandState = 5; s = 5u; (void)rng(s);
    animPlaySound__FPvP6Point3P9animSoundi(snd, &pos, rec, 0);
    expect(static_cast<unsigned>(eRandState) == s, "negative b13 draws nothing, negative b14 draws once");
    if (failures) return 1;
    std::printf("animPlaySound contract passed\n");
    return 0;
}
