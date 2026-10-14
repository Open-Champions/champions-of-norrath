// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 12];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
#include "process_falling_leaves.cpp"
#include <cmath>
#include <cstring>
static float boxSize[3] = {100.0f, 20.0f, 40.0f};
static int rolls[16], rollCount, rollUsed;
extern "C" void Size__C6VIBBox(float* r, const void*) { std::memcpy(r, boxSize, sizeof boxSize); }
extern "C" int rand(void) { return rolls[rollUsed++ % rollCount]; }
static float& f(unsigned char* o, int off) { return *reinterpret_cast<float*>(o + off); }
static unsigned& w(unsigned char* o, int off) { return *reinterpret_cast<unsigned*>(o + off); }
static bool near(float a, float b) { return std::fabs(a - b) <= 1e-4f * (1.0f + std::fabs(b)); }
int main() {
    alignas(8) static unsigned char o[128];
    unsigned char* rec = arena + 0x100;
    w(o, 88) = 0x100; w(o, 84) = 1;
    f(o, 92) = 1.0f; f(o, 96) = 1.0f; w(o, 32) = 0;
    // spawn, flag clear: 7 rolls
    for (int i = 0; i < 7; ++i) rolls[i] = 100 * (i + 1);
    rolls[0] = 1 << 30; rolls[1] = 0; rolls[4] = 2; rolls[5] = 65; rolls[6] = 3; rollCount = 7;
    // calls: r1, r2, m1, m2, m3, m4, m5 -> rolls[0..6]
    w(rec, 0) = 0;
    if (ProcessFallingLeaves__14VIEffectVolumef(o, 0.1f) != 0 || rollUsed != 7) return 1;
    if (w(rec, 0) != 1 || !near(f(rec, 12), 100.0f * 1073741824.0f * 0x1p-32f) || !near(f(rec, 16), 10.0f)) return 2;
    if (!near(f(rec, 20), 40.0f * (300.0f - 400.0f) * 0x1p-32f)) return 3;
    if (!near(f(rec, 28), (((131 * 2) & 0xff) / 255.0f) * 0.8f + 0.5f) || !near(f(rec, 24), -10.0f) || !near(f(rec, 32), 1.0f)) return 4;
    if (!near(f(rec, 8), 5.0f) || w(rec, 4) != 1) return 5;
    // state 1 -> 2 when height drops below the floor value
    w(o, 32) = 0; w(rec, 0) = 1; f(rec, 8) = 0.0f; f(rec, 16) = -9.9f; f(rec, 24) = -10.0f; f(rec, 28) = 2.0f;
    ProcessFallingLeaves__14VIEffectVolumef(o, 0.1f);
    if (w(rec, 0) != 2 || !near(f(rec, 8), 3.0f) || !near(f(rec, 16), -10.1f)) return 6;
    // state 2 with angle zero fades +32 and dies below zero
    f(rec, 8) = 0.0f; f(rec, 32) = 0.05f;
    ProcessFallingLeaves__14VIEffectVolumef(o, 0.1f);
    if (w(rec, 0) != 0 || !near(f(rec, 32), 0.05f - 0.1f * 0.6f)) return 7;
    // state 1 angle wrap
    w(rec, 0) = 1; f(rec, 8) = 29.0f; f(rec, 16) = 5.0f;
    ProcessFallingLeaves__14VIEffectVolumef(o, 0.5f);
    if (w(rec, 8) != 0) return 8;
    // flag set: nine rolls
    w(o, 32) = 1; w(rec, 0) = 0; rollUsed = 0; rollCount = 9;
    for (int i = 0; i < 9; ++i) rolls[i] = i * 7;
    ProcessFallingLeaves__14VIEffectVolumef(o, 0.1f);
    if (rollUsed != 9 || w(o, 32) != 0) return 9;
    // guards
    f(o, 96) = 0.0f; w(o, 32) = 5;
    if (ProcessFallingLeaves__14VIEffectVolumef(o, 0.1f) != 0 || w(o, 32) != 5) return 10;
    f(o, 96) = 1.0f; w(o, 88) = 0;
    if (ProcessFallingLeaves__14VIEffectVolumef(o, 0.1f) != -1) return 11;
    return 0;
}
