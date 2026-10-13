// Host check only; not a build input. Build: c++ -std=c++17 -arch x86_64 -Wl,-pagezero_size,0x1000 host_check.cpp
// (statics must sit below 4 GiB because mUI and the device are 32-bit words).
#include "increment.cpp"
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
    Increment__12VIWndOptions(window);
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
    options[7] = 0;
    ok = ok && run(1) == "s11;" && options[7] == 1;
    flags = 0; options[4] = 2;
    ok = ok && run(2) == "s11;" && options[4] == 0;  // (2 + 1) % 3
    flags = 0x100; options[4] = 9;
    ok = ok && run(2) == "s11;" && options[4] == 0;  // (9 + 1) % 10
    vol(0) = 0.5f; vol(1) = 0.5f;
    ok = ok && run(3) == "s11;v0=60;" && vol(0) > 0.59f && vol(0) < 0.61f;
    vol(0) = 1.0f;
    ok = ok && run(3) == "s9;v0=100;" && vol(0) == 1.0f;
    vol(1) = 0.88f;
    ok = ok && run(4) == "s11;v1=98;";
    vol(1) = 0.93f;
    ok = ok && run(4) == "s11;v1=100;" && vol(1) == 1.0f;
    options[5] = 1;
    ok = ok && run(5) == "m0;s11;" && options[5] == 0;
    options[145] = 0;
    ok = ok && run(6) == "s11;" && options[145] == 1;
    options[146] = 1;
    ok = ok && run(7) == "s11;" && options[146] == 0;
    for (int row = 8; row < 12; ++row) {
        window[1174 + row - 8] = 1;
        ok = ok && run(row) == "s11;" && window[1174 + row - 8] == 0;
    }
    // Unknown row: no sound, but the top-end clamp still runs (above 0.95 becomes 1.0).
    vol(0) = 0.96f; vol(1) = 0.9f;
    ok = ok && run(12) == "" && vol(0) == 1.0f && vol(1) == 0.9f;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
