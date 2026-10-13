// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include <string>
#include <vector>
#include <cstring>
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static std::vector<std::string> log_;
static int remove_result;
static unsigned records[3] = {0x100, 0x200, 0x300};
extern "C" unsigned FirstPlayback__13VISoundDevice(void*) { log_.push_back("first"); return records[0]; }
extern "C" unsigned NextPlayback__13VISoundDeviceP10VIPlayback(void*, unsigned p) {
    log_.push_back("next" + std::to_string(p));
    for (int i = 0; i < 3; ++i) if (records[i] == p) return i == 2 ? 0 : records[i + 1];
    return 0;
}
extern "C" void Stop__13VISoundDevicei(void*, int v) { log_.push_back("stop" + std::to_string(v)); }
extern "C" int Remove__12VIDictionaryR10VIResource(void*, unsigned r) { log_.push_back("remove" + std::to_string(r)); return remove_result; }
extern "C" void norrath_host_call_destructor(unsigned self, unsigned fn, int flags) {
    log_.push_back("dtor" + std::to_string(self) + "," + std::to_string(fn) + "," + std::to_string(flags));
}
#include "release_sound.cpp"
int main() {
    unsigned char dev[4096];
    std::memset(dev, 0, sizeof dev);
    unsigned* slots = reinterpret_cast<unsigned*>(dev + 2584);
    unsigned* s = reinterpret_cast<unsigned*>(arena + 0x2000);
    for (int i = 0; i < 3; ++i) {
        unsigned* r = reinterpret_cast<unsigned*>(arena + records[i]);
        r[28 / 4] = (i == 1) ? 7 : 5;
        r[32 / 4] = 40 + i;
    }
    s[3] = 0x3000;
    short* ent = reinterpret_cast<short*>(arena + 0x3000 + 8);
    ent[0] = 24; ent[1] = 0;
    *reinterpret_cast<unsigned*>(arena + 0x3000 + 12) = 0x7777;
    auto reset = [&](unsigned count, unsigned link) { s[0] = link; s[1] = count; slots[5] = 0x2000; log_.clear(); remove_result = 0; };
    // still referenced
    reset(3, 0x10);
    if (ReleaseSound__13VISoundDeviceiR12VIDictionary(dev, 5, nullptr) != 0 || s[1] != 2 || !log_.empty() || slots[5] != 0x2000) return 1;
    // last reference, no dictionary link
    reset(1, 0);
    if (ReleaseSound__13VISoundDeviceiR12VIDictionary(dev, 5, nullptr) != 0 || s[1] != 0 || slots[5] != 0) return 2;
    std::vector<std::string> want = {"first", "next256", "stop40", "next512", "next768", "stop42", "dtor8216,30583,3"};
    want[6] = "dtor" + std::to_string(0x2000 + 24) + "," + std::to_string(0x7777) + ",3";
    if (log_ != want) return 3;
    // dictionary link, removal fails
    reset(1, 0x10); remove_result = -3;
    if (ReleaseSound__13VISoundDeviceiR12VIDictionary(dev, 5, nullptr) != -1 || slots[5] != 0x2000 || log_.size() != 1 || log_[0] != "remove8192") return 4;
    // dictionary link, removal succeeds
    reset(0, 0x10); remove_result = 0;
    if (ReleaseSound__13VISoundDeviceiR12VIDictionary(dev, 5, nullptr) != 0 || slots[5] != 0 || log_[0] != "remove8192" || log_.size() != 8) return 5;
    // negative counts also release
    reset(0xffffffffu, 0);
    if (ReleaseSound__13VISoundDeviceiR12VIDictionary(dev, 5, nullptr) != 0 || s[1] != 0xfffffffeu || slots[5] != 0) return 6;
    return 0;
}
