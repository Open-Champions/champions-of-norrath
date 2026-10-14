// Host check only; not a build input.
#include "process.cpp"
#include <cstring>
#include <string>
static long long now; static std::string tr; static float lastdt;
extern "C" long long GetTime__7VITimer() { return now; }
extern "C" void AllocParticles__14VIEffectVolume(void*) { tr += "A;"; }
extern "C" void ProcessFallingLeaves__14VIEffectVolumef(void*, float dt) { tr += "1;"; lastdt = dt; }
extern "C" void ProcessLightShafts__14VIEffectVolumef(void*, float dt) { tr += "2;"; lastdt = dt; }
extern "C" void ProcessGroundFog__14VIEffectVolumef(void*, float dt) { tr += "3;"; lastdt = dt; }
extern "C" void ProcessDustStorm__14VIEffectVolumef(void*, float dt) { tr += "4;"; lastdt = dt; }
int main() {
    alignas(8) unsigned char o[128];
    auto reset = [&](unsigned type, unsigned flag, long long last) {
        std::memset(o, 0x5a, sizeof o);
        std::memcpy(o + 36, &type, 4); std::memcpy(o + 72, &flag, 4); std::memcpy(o, &last, 8); tr.clear();
    };
    auto stamp = [&]() { long long v; std::memcpy(&v, o, 8); return v; };
    reset(2, 1, 1000); now = 1030;                       // dt = 0.03 s: dispatch and stamp
    if (Process__14VIEffectVolumef(o, 99.0f) != 0 || tr != "A;2;" || stamp() != 1030) return 1;
    if (lastdt < 0.0299f || lastdt > 0.0301f) return 2;
    unsigned flag; std::memcpy(&flag, o + 72, 4); if (flag != 0) return 3;
    reset(3, 0, 1000); now = 1010;                       // 0.01 s: below 1/60, nothing
    if (Process__14VIEffectVolumef(o, 0) != 0 || !tr.empty() || stamp() != 1000) return 4;
    reset(1, 1, 1000); now = 1010;                       // allocation still happens when the step is skipped
    Process__14VIEffectVolumef(o, 0); if (tr != "A;" || stamp() != 1000) return 5;
    reset(4, 0, 1000); now = 1000 + 5000;                // long step is capped near 0.1
    Process__14VIEffectVolumef(o, 0); if (tr != "4;" || lastdt != 0x1.99999800p-4f || stamp() != 6000) return 6;
    reset(1, 0, 1000); now = 1017;                       // 0.017 >= 1/60
    Process__14VIEffectVolumef(o, 0); if (tr != "1;") return 7;
    reset(1, 0, 1000); now = 1016;                       // 0.016 < 1/60
    Process__14VIEffectVolumef(o, 0); if (!tr.empty()) return 8;
    reset(0, 0, 1000); now = 1100;                       // type 0: stamped, nothing called
    Process__14VIEffectVolumef(o, 0); if (!tr.empty() || stamp() != 1100) return 9;
    reset(5, 0, 1000); now = 1100; Process__14VIEffectVolumef(o, 0); if (!tr.empty()) return 10;
    return 0;
}
