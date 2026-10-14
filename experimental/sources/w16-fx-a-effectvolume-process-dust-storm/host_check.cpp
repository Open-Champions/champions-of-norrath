// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 12];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
#include "process_dust_storm.cpp"
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
    unsigned char* rec = arena + 0x100;
    w(o, 88) = 0x100; w(o, 84) = 1;
    f(o, 92) = 1.0f; f(o, 96) = 1.0f; w(o, 32) = 0; f(o, 40) = 0.5f;
    for (int i = 0; i < 12; ++i) rolls[i] = i + 1;
    rolls[3] = 1 << 30; rolls[4] = 0; rolls[8] = 0x7f; rollCount = 12;
    w(rec, 0) = 0;
    if (ProcessDustStorm__14VIEffectVolumef(o, 0.1f) != 0 || rollUsed != 11 || w(o, 32) != 0) return 1;
    if (w(rec, 0) != 1 || w(rec, 32) != 0) return 2;
    if (!near(f(rec, 24), (bt(1) + 2.0f) * 2.0f * 0.75f) || !near(f(rec, 28), (bt(2) + 2.0f) * 2.0f * 0.75f)) return 3;
    if (!near(f(rec, 4), -50.0f - 100.0f * bt(3) / 4.0f) || !near(f(rec, 8), 20.0f * 1073741824.0f * 0x1p-32f)) return 4;
    if (!near(f(rec, 12), 40.0f * (6.0f - 7.0f) * 0x1p-32f)) return 5;
    if (!near(f(rec, 52), f(rec, 8)) || !near(f(rec, 56), f(rec, 12)) || !near(f(rec, 48), 50.0f + 100.0f * bt(8) / 4.0f)) return 6;
    if (w(rec, 20) != (0x7fu & 0x3c) || w(rec, 16) != w(rec, 20)) return 7;
    if (!near(f(rec, 36), bt(10) * 0.5f + 0.5f)) return 8;
    float u = (bt(11) * 0.7f + 1.0f) * ((1.0f - 0.5f) + 1.0f);
    if (!near(f(rec, 40), u) || !near(f(rec, 44), 1.0f / u)) return 9;
    // age pass: index follows age, mote dies at its lifetime
    f(rec, 32) = 0.0f; f(rec, 36) = 1.0f; f(rec, 40) = 2.0f; f(rec, 44) = 0.5f; w(rec, 20) = 4;
    ProcessDustStorm__14VIEffectVolumef(o, 0.5f);
    if (w(rec, 0) != 1 || !near(f(rec, 32), 0.5f) || w(rec, 16) != static_cast<unsigned>((static_cast<int>(0.5f * 1.0f * (0.5f * 60.0f)) + 4) % 60)) return 10;
    f(rec, 32) = 1.5f;
    ProcessDustStorm__14VIEffectVolumef(o, 0.5f);
    if (w(rec, 0) != 0 || w(rec, 16) != static_cast<unsigned>((static_cast<int>(2.0f * 1.0f * 30.0f) + 4) % 60)) return 11;
    // flag set: twelve rolls and a random starting age
    w(o, 32) = 1; w(rec, 0) = 0; rollUsed = 0;
    ProcessDustStorm__14VIEffectVolumef(o, 0.1f);
    if (rollUsed != 12 || !near(f(rec, 32), f(rec, 40) * bt(12)) || w(o, 32) != 0) return 12;
    // guards
    f(o, 96) = 0.0f; w(o, 32) = 5;
    if (ProcessDustStorm__14VIEffectVolumef(o, 0.1f) != 0 || w(o, 32) != 5) return 13;
    f(o, 96) = 1.0f; w(o, 88) = 0;
    if (ProcessDustStorm__14VIEffectVolumef(o, 0.1f) != -1) return 14;
    return 0;
}
