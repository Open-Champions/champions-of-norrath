// Host check only; not a build input. Build as x86_64 with statics below 4 GiB. Covers the four
// combinations of (message parent equals this, word at +1196 nonzero).
#include "free.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static unsigned char object[2048];
static std::string trace;
static std::string off(void* p) { return std::to_string(static_cast<unsigned char*>(p) - object); }
extern "C" {
void RemoveChild__5VIWndP5VIWnd(void* self, void* child) {
    trace += "rm:" + off(self) + "," + (static_cast<unsigned char*>(child) >= object && static_cast<unsigned char*>(child) < object + 2048 ? off(child) : std::to_string(reinterpret_cast<__UINTPTR_TYPE__>(child))) + ";";
}
void Free__12VIWndMessage(void* self) { trace += "msgfree:" + off(self) + ";"; }
void Free__13VIBattleMusic(void* self) { trace += "music:" + off(self) + ";"; }
}

int main() {
    bool ok = true;
    for (int mask = 0; mask < 4; ++mask) {
        std::memset(object, 0x5a, sizeof object);
        static unsigned char expect[2048];
        std::memset(expect, 0x5a, sizeof expect);
        unsigned parent = (mask & 1) ? static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(object)) : 0x1234u;
        unsigned child = (mask & 2) ? 0x2000u : 0u;
        std::memcpy(object + 1252, &parent, 4); std::memcpy(expect + 1252, &parent, 4);
        std::memcpy(object + 1196, &child, 4);
        unsigned z = 0;
        std::memcpy(expect + 1196, mask & 2 ? &z : &child, 4);
        trace.clear();
        Free__12VIWndOptions(object);
        std::string want;
        if (mask & 1) want += "rm:0,1232;msgfree:1232;";
        if (mask & 2) want += "rm:0,8192;";
        want += "music:1200;";
        ok = ok && trace == want && std::memcmp(object, expect, sizeof object) == 0;
        if (trace != want) std::printf("got %s want %s\n", trace.c_str(), want.c_str());
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
