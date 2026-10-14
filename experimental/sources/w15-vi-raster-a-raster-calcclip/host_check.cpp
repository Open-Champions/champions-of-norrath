// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
static unsigned char scratch[1 << 12];
extern "C" unsigned char* norrath_host_pointer(unsigned address) {
    if (address >= 0x70000000u) return scratch + (address - 0x70000000u);
    return arena + (address - arena_base);
}
static int identity_calls; static float* identity_arg;
extern "C" void Identity__10VIMatrix44(float* m) { ++identity_calls; identity_arg = m; for (int i = 0; i < 16; ++i) m[i] = i % 5 == 0 ? 1.0f : 0.0f; }
#include "calc_clip_to_screen.cpp"
#include <cstdio>
int main() {
    int* I = reinterpret_cast<int*>(arena + 17700);
    (void)I;
    auto wi = [](unsigned off, int v) { *reinterpret_cast<int*>(arena + off) = v; };
    auto wf = [](unsigned off, float v) { *reinterpret_cast<float*>(arena + off) = v; };
    wi(17716, 7); wi(17720, 9); wi(17724, 640); wi(17728, 448); wi(17748, 100); wi(17752, 60);
    wf(17544, 0.125f); wf(17772, 1.0f); wf(17776, 65000.0f);
    bool ok = CalcClipToScreen__8VIRaster(arena) == 0 && identity_calls == 1 && identity_arg == reinterpret_cast<float*>(scratch + 0xc0);
    float* m = reinterpret_cast<float*>(scratch + 0xc0);
    ok = ok && m[0] == 320.0f && m[5] == -224.0f && m[10] == (1.0f - 65000.0f) * 0.5f;
    ok = ok && m[12] == (2048.0f - 50.0f + 320.0f + 7.0f) && m[13] == (2048.0f - 30.0f + 224.0f + 9.0f);
    ok = ok && m[14] == (65000.0f - 1.0f) * 0.5f + 1.0f + 0.125f;
    ok = ok && m[15] == 1.0f && m[1] == 0.0f && m[11] == 0.0f;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
