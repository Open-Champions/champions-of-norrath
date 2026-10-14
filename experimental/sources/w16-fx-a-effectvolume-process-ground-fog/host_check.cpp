// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 12];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
#include "process_ground_fog.cpp"
#include <cmath>
#include <cstring>
static float boxSize[3] = {100.0f, 20.0f, 40.0f};
static int rolls[16], rollCount, rollUsed;
extern "C" void Size__C6VIBBox(float* r, const void*) { std::memcpy(r, boxSize, sizeof boxSize); }
extern "C" int rand(void) { return rolls[rollUsed++ % rollCount]; }
static float& f(unsigned char* o, int off) { return *reinterpret_cast<float*>(o + off); }
static unsigned& w(unsigned char* o, int off) { return *reinterpret_cast<unsigned*>(o + off); }
static bool near(float a, float b) { return std::fabs(a - b) <= 1e-4f * (1.0f + std::fabs(b)); }
static float bt(int r) { return ((r * 131) & 0xff) / 255.0f; }
int main() {
    alignas(8) static unsigned char o[128];
    unsigned char* records = arena + 0x100;
    w(o, 88) = 0x100; w(o, 84) = 2;
    f(o, 92) = 1.0f; f(o, 96) = 1.0f; w(o, 32) = 0;
    w(records, 0) = 0;
    w(records + 36, 0) = 1; f(records + 36, 28) = 0.9f; f(records + 36, 32) = 1.0f;
    for (int i = 0; i < 9; ++i) rolls[i] = i + 1;
    rolls[2] = 1 << 30; rolls[3] = 0; rollCount = 9;
    if (ProcessGroundFog__14VIEffectVolumef(o, 0.25f) != 0) return 1;
    if (rollUsed != 8 || w(o, 32) != 0) return 2;                       // flag clear: eight rolls, age starts at zero
    if (w(records, 0) != 1 || w(records, 8) != 0 || w(records, 28) != 0) return 3;
    if (!near(f(records, 20), 2.0f * (bt(1) + 2.0f)) || !near(f(records, 24), 20.0f * (bt(2) * 0.15f + 0.85f))) return 4;
    if (!near(f(records, 4), 100.0f * 1073741824.0f * 0x1p-32f) || w(records, 16) != (7u & 7u)) return 5;
    if (!near(f(records, 12), 40.0f * (5.0f - 6.0f) * 0x1p-32f) || !near(f(records, 32), bt(8) * 30.0f + 10.0f)) return 6;
    if (w(records + 36, 0) != 0 || !near(f(records + 36, 28), 1.15f)) return 7;
    // flag set: nine rolls and a random starting age
    w(o, 32) = 1; w(records, 0) = 0; w(records + 36, 0) = 2; rollUsed = 0;
    ProcessGroundFog__14VIEffectVolumef(o, 0.25f);
    if (rollUsed != 9 || !near(f(records, 28), f(records, 32) * bt(9)) || w(o, 32) != 0) return 8;
    // guards
    f(o, 96) = 0.0f; w(o, 32) = 5;
    if (ProcessGroundFog__14VIEffectVolumef(o, 0.25f) != 0 || w(o, 32) != 5) return 9;
    f(o, 96) = 1.0f; w(o, 88) = 0;
    if (ProcessGroundFog__14VIEffectVolumef(o, 0.25f) != -1) return 10;
    w(o, 88) = 0x100; w(o, 84) = 0;
    if (ProcessGroundFog__14VIEffectVolumef(o, 0.25f) != 0 || w(o, 32) != 0) return 11;
    return 0;
}
