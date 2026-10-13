// Host check only; not a build input. Compared with the original in a private interpreter run
// (random object fills, identical call logs and changed memory).
#include "init.cpp"
#include <cstdio>
#include <cstring>
#include <string>
static unsigned char storage[0x4000];
static std::string trace;
static std::string at(void* p) { return std::to_string(static_cast<unsigned char*>(p) - storage); }
extern "C" void Init__12VIWndMessage(void* self) { trace += "msg@" + at(self) + ";"; }
extern "C" void Init__16VIWndReadMessage(void* self) { trace += "read@" + at(self) + ";"; }
extern "C" void AddChild__5VIWndP5VIWnd(void* self, void* child) { trace += "add@" + at(self) + "," + at(child) + ";"; }
int main() {
    std::memset(storage, 0x5a, sizeof storage);
    const int result = Init__12VIWndPatcher(storage);
    bool ok = result == 0 && trace == "msg@1176;add@0,1176;read@2312;add@0,2312;";
    for (int i = 0; i < 0x4000; ++i) {
        const bool zeroed = (i >= 1144 && i < 1152) || (i >= 1204 && i < 1208) || (i >= 2340 && i < 2344);
        const bool plain_zero = (i >= 1144 && i < 1148) || (i >= 1148 && i < 1152) || (i >= 1204 && i < 1208) || (i >= 2340 && i < 2344);
        if (zeroed ? (!plain_zero || storage[i] != 0) : storage[i] != 0x5a) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
