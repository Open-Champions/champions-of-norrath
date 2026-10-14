// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include <vector>
#include <string>
#include <cstring>
#include <cstdint>
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static unsigned flags;
static int cached_result, play_result;
static std::vector<std::string> log_;
static float fbits(unsigned u) { float f; std::memcpy(&f, &u, 4); return f; }
extern "C" unsigned VIGetFeatureFlags__Fv() { log_.push_back("flags"); return flags; }
extern "C" unsigned Sound__13VISoundDevicei(unsigned d, int i) { log_.push_back("sound" + std::to_string(d) + "," + std::to_string(i)); return 0x2000; }
extern "C" int norrath_host_call_is_cached(unsigned s, unsigned f) { log_.push_back("cached" + std::to_string(s) + "," + std::to_string(f)); return cached_result; }
extern "C" int Play__13VISoundDeviceii12VISOUNDCLASS(unsigned d, int i, int u, int c) {
    log_.push_back("play" + std::to_string(d) + "," + std::to_string(i) + "," + std::to_string(u) + "," + std::to_string(c));
    return play_result;
}
static float last_volume;
extern "C" void SetVolume__13VISoundDeviceif(unsigned d, int id, float v) {
    last_volume = v;
    log_.push_back("vol" + std::to_string(d) + "," + std::to_string(id));
}
extern "C" void StopNow__13VIBattleMusic(void*) { log_.push_back("stopnow"); }
#include "process.cpp"
static unsigned bits(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
int main() {
    unsigned m[10];
    unsigned* scene = reinterpret_cast<unsigned*>(arena + 0x1000);
    unsigned* sound = reinterpret_cast<unsigned*>(arena + 0x2000);
    std::memset(arena, 0, sizeof arena);
    scene[19360 / 4] = 0x4444;
    sound[3] = 0x3000;  // table
    short* ent = reinterpret_cast<short*>(arena + 0x3000 + 48);
    ent[0] = 16; ent[1] = 0;
    *reinterpret_cast<unsigned*>(arena + 0x3000 + 52) = 0x5555;
    auto reset = [&](unsigned state) {
        for (int i = 0; i < 10; ++i) m[i] = 0x100 + i;
        m[0] = 0x1000; m[1] = 4; m[2] = state; m[7] = 0xffffffffu; m[6] = 3; m[5] = 0;
        m[3] = bits(0.25f); m[4] = bits(0.5f);
        log_.clear(); flags = 0; cached_result = 1; play_result = 9;
    };
    reset(1); flags = 0x100;
    if (Process__13VIBattleMusic(m) != 0 || log_.size() != 1) return 1;
    reset(1); m[0] = 0;
    if (Process__13VIBattleMusic(m) != 0 || log_.size() != 1) return 2;
    reset(1); m[1] = 0;
    if (Process__13VIBattleMusic(m) != 0 || log_.size() != 1 || m[2] != 1) return 3;
    // state 1, no playback yet, index -1
    reset(1); m[6] = 0xffffffffu;
    if (Process__13VIBattleMusic(m) != -1) return 14;
    if (log_.size() != 1 || m[7] != 0xffffffffu) return 4;
    // not cached
    reset(1); cached_result = 0;
    Process__13VIBattleMusic(m);
    if (log_.size() != 3 || log_[1] != "sound17476,3" || log_[2] != "cached8208,21845" || m[7] != 0xffffffffu) return 5;
    // start playback
    reset(1);
    Process__13VIBattleMusic(m);
    if (m[7] != 9 || m[3] != bits(0.75f) || m[2] != 1) return 6;
    if (log_.size() != 6 || log_[3] != "play17476,3,0,1" || log_[4] != "vol17476,9" || log_[5] != "vol17476,9") return 7;
    // existing playback, reaching 1.0
    reset(1); m[7] = 6; m[3] = bits(0.75f);
    Process__13VIBattleMusic(m);
    if (m[3] != bits(1.0f) || m[2] != 3 || last_volume != 1.0f || log_.size() != 2) return 8;
    reset(1); m[7] = 6; m[3] = bits(0.75f); m[4] = bits(0.75f);
    Process__13VIBattleMusic(m);
    if (m[3] != bits(1.0f) || m[2] != 3) return 9;
    // fade out
    reset(2); m[7] = 6; m[3] = bits(1.0f); m[4] = bits(0.5f);
    Process__13VIBattleMusic(m);
    if (m[3] != bits(1.0f - 0.5f * fbits(0x3dccccccu)) || m[2] != 2 || log_.size() != 2 || log_[1] != "vol17476,6") return 10;
    reset(2); m[7] = 6; m[3] = bits(1.0f); m[4] = bits(0.05f); m[5] = 1;
    Process__13VIBattleMusic(m);
    if (m[3] != bits(1.0f - (0.05f * fbits(0x3dccccccu)) * 10.0f) || m[2] != 2) return 11;
    reset(2); m[7] = 6; m[3] = bits(0.01f); m[4] = bits(0.5f);
    Process__13VIBattleMusic(m);
    if (m[2] != 0 || log_.size() != 2 || log_[1] != "stopnow") return 12;
    // idle and playing states do nothing
    for (unsigned s : {0u, 3u, 4u, 0xffffffffu}) {
        reset(s);
        unsigned before[10]; std::memcpy(before, m, sizeof m);
        if (Process__13VIBattleMusic(m) != 0 || log_.size() != 1 || std::memcmp(before, m, sizeof m)) return 13;
    }
    return 0;
}
