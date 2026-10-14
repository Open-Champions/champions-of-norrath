// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include <vector>
#include <string>
#include <cstring>
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static unsigned flags;
static int rolls;
static std::vector<std::string> log_;
static bool stop_now_sets;
extern "C" unsigned VIGetFeatureFlags__Fv() { log_.push_back("flags"); return flags; }
extern "C" int rand() { log_.push_back("rand"); return rolls; }
extern "C" void Stop__13VIBattleMusicb(void*, bool b) { log_.push_back(b ? "stop1" : "stop0"); }
extern "C" void StopNow__13VIBattleMusic(void* m) { log_.push_back("stopnow"); if (stop_now_sets) static_cast<unsigned*>(m)[2] = 3; }
extern "C" void Preload__13VIBattleMusici(void*, int t) { log_.push_back("preload" + std::to_string(t)); }
extern "C" void PlayBgm__13VISoundDeviceiPCc(unsigned d, int i, const char* p) { log_.push_back("play" + std::to_string(d) + "," + std::to_string(i) + "," + p); }
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include "play.cpp"
int main() {
    unsigned m[10];
    unsigned* scene = reinterpret_cast<unsigned*>(arena + 0x1000);
    std::memset(arena, 0, sizeof arena);
    auto reset = [&]() { for (int i = 0; i < 10; ++i) m[i] = 0x100 + i; log_.clear(); m[0] = 0x1000; stop_now_sets = false; };
    reset(); m[0] = 0; flags = 0x100;
    if (Play__13VIBattleMusici(m, 3) != 0 || !log_.empty()) return 1;
    reset();
    if (Play__13VIBattleMusici(m, 3) != 0 || !log_.empty()) return 2;
    scene[19360 / 4] = 0x4444;
    // feature flag path
    reset(); flags = 0x100; m[2] = 0; rolls = 20;
    Play__13VIBattleMusici(m, 9);
    if (m[2] != 3 || m[1] != 5) return 3;
    if (log_.size() != 3 || log_[2] != "play17476,0,bgm\\battle05.bgm") return 4;
    reset(); flags = 0x100; m[2] = 1; rolls = 20;
    Play__13VIBattleMusici(m, 9);
    if (m[2] != 1 || m[1] != 0x101) return 5;
    reset(); flags = 0x100; m[1] = 0x100; Play__13VIBattleMusici(m, 12);
    if (m[1] != 12 || log_.back() != "play17476,0,bgm\\battle12.bgm") return 6;
    reset(); flags = 0x100; m[1] = 7; Play__13VIBattleMusici(m, -1);
    if (m[1] != 7 || log_.back() != "play17476,0,bgm\\battle07.bgm") return 7;
    reset(); flags = 0x100; m[1] = 0; Play__13VIBattleMusici(m, -1);
    if (log_.back() != "stop0") return 8;
    reset(); flags = 0x100; m[1] = 4; Play__13VIBattleMusici(m, 0);
    if (m[1] != 0 || log_.back() != "stop0") return 9;
    // plain path
    reset(); flags = 0; m[1] = 0xffffffffu;
    if (Play__13VIBattleMusici(m, -1) != -1) return 16;
    if (m[5] != 0 || m[2] != 0x102 || log_.size() != 1) return 10;
    reset(); flags = 0; m[1] = 6; m[2] = 0; m[7] = 0xffffffffu; Play__13VIBattleMusici(m, -1);
    if (m[1] != 6 || m[2] != 1 || m[3] != 0 || log_.size() != 1) return 11;
    reset(); flags = 0; m[1] = 6; m[2] = 0; m[7] = 3; stop_now_sets = true; Play__13VIBattleMusici(m, 8);
    if (m[1] != 8 || m[2] != 3 || m[3] != 0) return 12;
    if (log_.size() != 3 || log_[1] != "preload8" || log_[2] != "stopnow") return 13;
    reset(); flags = 0; m[1] = 6; m[2] = 3; Play__13VIBattleMusici(m, 6);
    if (m[2] != 3 || m[3] != 0x103 || log_.size() != 1) return 14;
    reset(); flags = 0; m[1] = 6; m[2] = 2; Play__13VIBattleMusici(m, 6);
    if (m[2] != 1 || m[5] != 0) return 15;
    return 0;
}
