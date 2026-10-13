// Host check only; not a build input. Compared with the original in a private interpreter run
// (random object fills, identical call logs and changed memory).
#include "init.cpp"
#include <cstdio>
#include <cstring>
#include <string>
static unsigned char storage[0x4000];
static std::string trace;
static std::string at(void* p) { return std::to_string(static_cast<unsigned char*>(p) - storage); }
extern "C" void Init__12VIWndConnect(void* self) { trace += "connect@" + at(self) + ";"; }
extern "C" void Init__12VIWndMessage(void* self) { trace += "msg@" + at(self) + ";"; }
extern "C" void Init__12VIWndOptionsP7VIScene(void* self, void* scene) { trace += "options@" + at(self) + "," + at(scene) + ";"; }
extern "C" void AddChild__5VIWndP5VIWnd(void* self, void* child) { trace += "add@" + at(self) + "," + at(child) + ";"; }
int main() {
    std::memset(storage, 0x5a, sizeof storage);
    const int result = Init__11VIWndSplashP7VIScene(storage, storage + 3);
    bool ok = result == 0 && trace == "connect@416;add@0,416;msg@4776;add@0,4776;options@2408,3;add@0,2408;";
    unsigned scene;
    std::memcpy(&scene, storage + 36, 4);
    if (scene != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(storage + 3))) ok = false;
    for (int i = 0; i < 0x4000; ++i) {
        const bool zero = (i >= 408 && i < 412) || (i >= 444 && i < 448) || (i >= 2436 && i < 2440) ||
                          (i >= 4804 && i < 4808) || (i >= 9640 && i < 9668);
        const bool scene_word = i >= 36 && i < 40;
        if (zero ? storage[i] != 0 : (!scene_word && storage[i] != 0x5a)) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
