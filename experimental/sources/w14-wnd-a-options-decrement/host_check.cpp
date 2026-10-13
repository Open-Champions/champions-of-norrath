// Host check only; not a build input. Build: c++ -std=c++17 -arch x86_64 -Wl,-pagezero_size,0x1000 host_check.cpp
// (statics must sit below 4 GiB because mUI and the device are 32-bit words).
#include "decrement.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned _5VIWnd$mUI;
alignas(8) static unsigned char window[1300];
alignas(8) static unsigned char options[160];
alignas(8) static unsigned char ui[128];
static int device_tag = 1;
static unsigned flags;
static std::string trace;
extern "C" {
unsigned char* VIGetGameOptions__Fv() { return options; }
unsigned VIGetFeatureFlags__Fv() { return flags; }
void SoundPlay__4VIUI11VIUISoundId(unsigned u, int s) { trace += (u == _5VIWnd$mUI ? "s" : "s?") + std::to_string(s) + ";"; }
void SetSoundClassVolume__13VISoundDevice12VISOUNDCLASSf(void* d, int c, float v) {
    trace += std::string(d == &device_tag ? "v" : "v?") + std::to_string(c) + "=" + std::to_string(static_cast<int>(v * 100 + 0.5f)) + ";";
}
void SetOutputMode__13VISoundDevicei(void* d, int m) { trace += std::string(d == &device_tag ? "m" : "m?") + std::to_string(m) + ";"; }
}

static std::string run(int row) {
    trace.clear();
    *reinterpret_cast<unsigned*>(window + 1152) = row;
    Decrement__12VIWndOptions(window);
    return trace;
}
static float& vol(int i) { return *reinterpret_cast<float*>(options + 8 + 4 * i); }

int main() {
    bool ok = true;
    if (reinterpret_cast<unsigned long>(ui) > 0xffffffffUL || reinterpret_cast<unsigned long>(&device_tag) > 0xffffffffUL) {
        std::puts("FAIL (statics above 4 GiB)");
        return 1;
    }
    _5VIWnd$mUI = static_cast<unsigned>(reinterpret_cast<unsigned long>(ui));
    *reinterpret_cast<unsigned*>(ui + 76) = static_cast<unsigned>(reinterpret_cast<unsigned long>(&device_tag));
    options[6] = 1;
    ok = ok && run(0) == "s11;" && options[6] == 0;
    ok = ok && run(0) == "s11;" && options[6] == 1;
    options[7] = 0;
    ok = ok && run(1) == "s11;" && options[7] == 1;
    flags = 0; options[4] = 0;
    ok = ok && run(2) == "s11;" && options[4] == 2;  // (0 + 3 - 1) % 3
    flags = 0x100; options[4] = 0;
    ok = ok && run(2) == "s11;" && options[4] == 9;  // (0 + 10 - 1) % 10
    vol(0) = 0.5f; vol(1) = 0.5f;
    ok = ok && run(3) == "s11;v0=40;" && vol(0) > 0.39f && vol(0) < 0.41f;
    vol(0) = 0.0f;
    ok = ok && run(3) == "s9;v0=0;" && vol(0) == 0.0f;
    vol(1) = 0.12f;
    ok = ok && run(4) == "s11;v1=2;" && vol(1) == 0.0f;  // reported before the final floor clamp zeroes it
    options[5] = 0;
    ok = ok && run(5) == "m1;s11;" && options[5] == 1;
    options[145] = 0;
    ok = ok && run(6) == "s11;" && options[145] == 1;
    options[146] = 1;
    ok = ok && run(7) == "s11;" && options[146] == 0;
    for (int row = 8; row < 12; ++row) {
        window[1174 + row - 8] = 0;
        ok = ok && run(row) == "s11;" && window[1174 + row - 8] == 1;
    }
    // Unknown row: no sound, but the low-volume clamp still runs (0.04 is below the 0.05-ish floor).
    vol(0) = 0.04f; vol(1) = 0.9f;
    ok = ok && run(12) == "" && vol(0) == 0.0f && vol(1) == 0.9f;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
