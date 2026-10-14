// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include <string>
#include <vector>
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static std::vector<std::string> log_;
static unsigned flags;
static int parseResult;
extern "C" unsigned VIGetFeatureFlags__Fv() { return flags; }
extern "C" int sprintf(char* d, const char* format, ...) {
    __builtin_va_list ap;
    __builtin_va_start(ap, format);
    int n = __builtin_vsnprintf(d, 256, format, ap);
    __builtin_va_end(ap);
    return n;
}
extern "C" void Unload__13VIBattleMusic(unsigned char* m) { log_.push_back("unload"); *reinterpret_cast<int*>(m + 24) = -1; }
extern "C" void __10VIESFParse(void*) { log_.push_back("ctor"); }
extern "C" int ParseSound__10VIESFParsePCcR7VIScene(void*, const char* name, unsigned char* scene) {
    log_.push_back(std::string("parse:") + name + (scene == arena + 0x2000 ? ":scene" : ":?"));
    return parseResult;
}
extern "C" void _$_10VIESFParse(void*, int f) { log_.push_back("dtor" + std::to_string(f)); }
extern "C" unsigned Sound__13VISoundDevicei(unsigned char*, int handle) { log_.push_back("sound" + std::to_string(handle)); return 0x99; }
extern "C" void SetRepeat__4VIXmi(unsigned sound, int repeat) { log_.push_back("repeat" + std::to_string(sound) + "," + std::to_string(repeat)); }
#include "preload.cpp"
static unsigned& w(unsigned address) { return *reinterpret_cast<unsigned*>(arena + address); }
int main() {
    unsigned char* music = arena + 0x1000;
    // the scene at 0x2000 holds the device address at +19360
    flags = 0x100;
    w(0x1000) = 0x2000; w(0x2000 + 19360) = 0x3000;
    if (Preload__13VIBattleMusici(music, 3) != 0 || !log_.empty()) return 1;
    flags = 0;
    w(0x1000) = 0;
    if (Preload__13VIBattleMusici(music, 3) != 0 || !log_.empty()) return 2;
    w(0x1000) = 0x2000; w(0x2000 + 19360) = 0;
    if (Preload__13VIBattleMusici(music, 3) != 0 || !log_.empty()) return 3;
    w(0x2000 + 19360) = 0x3000;
    parseResult = 5;
    if (Preload__13VIBattleMusici(music, 3) != 0) return 4;
    if (log_.size() != 6 || log_[0] != "unload" || log_[1] != "ctor" || log_[2] != "parse:bgm\\battle3.esf:scene") return 5;
    if (log_[3] != "sound5" || log_[4] != "repeat153,1" || log_[5] != "dtor2") return 6;
    if (w(0x1000 + 4) != 3 || w(0x1000 + 24) != 5) return 7;
    // parse failure: handle stored, no repeat, result -1
    log_.clear();
    parseResult = -1;
    if (Preload__13VIBattleMusici(music, 12) != -1 || log_.size() != 4 || log_[2] != "parse:bgm\\battle12.esf:scene" || log_[3] != "dtor2") return 8;
    if (w(0x1000 + 4) != 12 || static_cast<int>(w(0x1000 + 24)) != -1) return 9;
    // track zero: only unload; result 0
    log_.clear();
    if (Preload__13VIBattleMusici(music, 0) != 0 || log_.size() != 1 || w(0x1000 + 4) != 0) return 10;
    // negative track: stale handle decides the result
    log_.clear();
    w(0x1000 + 24) = 0;
    if (Preload__13VIBattleMusici(music, -1) != -1 || log_.size() != 1 || static_cast<int>(w(0x1000 + 4)) != -1) return 11;
    return 0;
}
