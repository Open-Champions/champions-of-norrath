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
extern "C" void Stop__13VISoundDevicei(unsigned device, int id) { log_.push_back(300); log_.push_back(static_cast<int>(device)); log_.push_back(id); }
#include "stop_now.cpp"
int main() {
    unsigned m[10];
    unsigned* scene = reinterpret_cast<unsigned*>(arena + 0x1000);
    std::memset(arena, 0, sizeof arena);
    auto reset = [&]() { for (int i = 0; i < 10; ++i) m[i] = 0x11110000u + i; log_.clear(); };
    reset(); m[0] = 0; flags = 0x100;
    StopNow__13VIBattleMusic(m);
    if (!log_.empty() || m[2] != 0x11110002u) return 1;
    reset(); m[0] = 0x1000;
    StopNow__13VIBattleMusic(m);
    if (!log_.empty() || m[2] != 0x11110002u) return 2;
    scene[19360 / 4] = 0x4444;
    reset(); m[0] = 0x1000; flags = 0x1ff;
    StopNow__13VIBattleMusic(m);
    if (m[2] != 0 || m[7] != 0x11110007u) return 3;
    if (log_.size() != 4 || log_[1] != 200 || log_[2] != 0x4444 || log_[3] != 0) return 4;
    reset(); m[0] = 0x1000; flags = 0; m[7] = 0xffffffffu;
    StopNow__13VIBattleMusic(m);
    if (m[2] != 0 || m[7] != 0xffffffffu || log_.size() != 1) return 5;
    reset(); m[0] = 0x1000; flags = 0xff; m[7] = 9;
    StopNow__13VIBattleMusic(m);
    if (m[2] != 0 || m[7] != 0xffffffffu || log_.size() != 4 || log_[1] != 300 || log_[2] != 0x4444 || log_[3] != 9) return 6;
    return 0;
}
