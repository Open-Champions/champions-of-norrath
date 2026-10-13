// Host check only; not a build input. Build as x86_64 with statics below 4 GiB. Fills the object with a
// pattern, runs the routine with recording stubs and checks the call order, the this+offset
// arguments and every stored word (and that neighbouring bytes are untouched). The same
// routine was also compared with the original in a private interpreter run.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char _vt$12VIWndOptions[8];
static unsigned char object[0x8000];
static std::string trace;
extern "C" {
void __5VIWnd(void* self) { trace += "__5VIWnd:" + std::to_string(static_cast<unsigned char*>(self) - object) + ";"; }
void __10VIUIWindow(void* self) { trace += "__10VIUIWindow:" + std::to_string(static_cast<unsigned char*>(self) - object) + ";"; }
void __13VIBattleMusic(void* self) { trace += "__13VIBattleMusic:" + std::to_string(static_cast<unsigned char*>(self) - object) + ";"; }
void __12VIWndMessage(void* self) { trace += "__12VIWndMessage:" + std::to_string(static_cast<unsigned char*>(self) - object) + ";"; }
}

int main() {
    std::memset(object, 0x5a, sizeof object);
    unsigned char expect[0x8000];
    std::memset(expect, 0x5a, sizeof expect);
    { unsigned v = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$12VIWndOptions)); std::memcpy(expect + 32, &v, 4); }
    { unsigned v = 0u; std::memcpy(expect + 1196, &v, 4); }
    __12VIWndOptions(object);
    bool ok = trace == "__5VIWnd:0;__10VIUIWindow:40;__10VIUIWindow:408;__10VIUIWindow:776;__13VIBattleMusic:1200;__12VIWndMessage:1232;" && std::memcmp(object, expect, sizeof object) == 0;
    if (!ok) std::printf("trace %s\n", trace.c_str());
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
