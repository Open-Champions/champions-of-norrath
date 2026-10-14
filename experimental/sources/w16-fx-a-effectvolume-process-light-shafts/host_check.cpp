// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 12];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
#include "process_light_shafts.cpp"
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
    unsigned char* records = arena + 0x100;
    w(o, 88) = 0x100;
    f(o, 92) = 1.0f; f(o, 96) = 1.0f; f(o, 40) = 0.0f; w(o, 32) = 7;
    w(o, 84) = 3;
    // record 0 new, record 1 alive and expiring, record 2 alive and young
    w(records, 0) = 0;
    w(records + 32, 0) = 1; f(records + 32, 24) = 0.9f; f(records + 32, 28) = 1.0f;
    w(records + 64, 0) = 1; f(records + 64, 24) = 0.1f; f(records + 64, 28) = 5.0f;
    rolls[0] = 1 << 30; rolls[1] = 0; rolls[2] = 0; rolls[3] = 1 << 30; rolls[4] = 1; rolls[5] = 2; rolls[6] = 3; rollCount = 7;
    if (ProcessLightShafts__14VIEffectVolumef(o, 0.25f) != 0) return 1;
    if (rollUsed != 7 || w(o, 32) != 0) return 2;
    if (w(records, 0) != 1 || !near(f(records, 4), 100.0f * 1073741824.0f * 0x1p-32f) || !near(f(records, 8), 10.0f) || !near(f(records, 12), -40.0f * 0.25f)) return 3;
    if (w(records, 24) != 0 || !near(f(records, 16), (131 * 1 & 0xff) / 255.0f * 0.4f * 0.5f + 0.75f)) return 4;
    if (!near(f(records, 20), 20.0f * (((131 * 2) & 0xff) / 255.0f * 0.2f + 0.8f))) return 5;
    if (!near(f(records, 28), 60.0f + (((131 * 3) & 0xff) / 255.0f) * 12.0f + 4.0f)) return 6;
    if (w(records + 32, 0) != 0 || !near(f(records + 32, 24), 1.15f)) return 7;
    if (w(records + 64, 0) != 1 || !near(f(records + 64, 24), 0.35f)) return 8;
    // guards
    f(o, 96) = 0.0f; w(o, 32) = 7; rollUsed = 0;
    if (ProcessLightShafts__14VIEffectVolumef(o, 0.25f) != 0 || w(o, 32) != 7) return 9;
    f(o, 96) = 1.0f; w(o, 88) = 0;
    if (ProcessLightShafts__14VIEffectVolumef(o, 0.25f) != -1) return 10;
    w(o, 88) = 0x100; w(o, 84) = 0; w(o, 32) = 7;
    if (ProcessLightShafts__14VIEffectVolumef(o, 0.25f) != 0 || w(o, 32) != 0) return 11;
    return 0;
}
