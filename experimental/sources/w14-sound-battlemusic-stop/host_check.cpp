// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include <vector>
#include <cstring>
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static unsigned flags;
static std::vector<int> log_;
extern "C" unsigned VIGetFeatureFlags__Fv() { log_.push_back(100); return flags; }
extern "C" void StopBgm__13VISoundDevicei(unsigned device, int index) { log_.push_back(200); log_.push_back(static_cast<int>(device)); log_.push_back(index); }
#include "stop.cpp"
int main() {
    unsigned m[10];
    unsigned* scene = reinterpret_cast<unsigned*>(arena + 0x1000);
    std::memset(arena, 0, sizeof arena);
    scene[19360 / 4 - 0] = 0;  // out of the 0x10000 arena? 19360 < 0x10000, fine
    auto reset = [&]() { for (int i = 0; i < 10; ++i) m[i] = 0x11110000u + i; log_.clear(); };
    // no scene
    reset(); m[0] = 0; flags = 0x100;
    Stop__13VIBattleMusicb(m, true);
    if (!log_.empty() || m[2] != 0x11110002u) return 1;
    // scene without device
    reset(); m[0] = 0x1000; flags = 0x100;
    Stop__13VIBattleMusicb(m, true);
    if (!log_.empty() || m[2] != 0x11110002u) return 2;
    // device, feature flag path
    scene[19360 / 4] = 0x4444;
    reset(); m[0] = 0x1000; flags = 0x100;
    Stop__13VIBattleMusicb(m, true);
    if (m[2] != 0 || m[5] != 0x11110005u) return 3;
    if (log_.size() != 4 || log_[0] != 100 || log_[1] != 200 || log_[2] != 0x4444 || log_[3] != 0) return 4;
    // flag clear, playback id -1
    reset(); m[0] = 0x1000; flags = 0xff; m[7] = 0xffffffffu;
    Stop__13VIBattleMusicb(m, true);
    if (m[2] != 0 || m[5] != 1 || log_.size() != 1) return 5;
    // flag clear, playback id set
    reset(); m[0] = 0x1000; flags = 0x0; m[7] = 5;
    Stop__13VIBattleMusicb(m, false);
    if (m[2] != 2 || m[5] != 0 || log_.size() != 1) return 6;
    return 0;
}
