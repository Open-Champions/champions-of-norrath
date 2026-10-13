// Host check only; not a build input. Build as x86_64 with statics below 4 GiB. Fills the object with a
// pattern, runs the routine with recording stubs and checks the call order, the this+offset
// arguments and every stored word (and that neighbouring bytes are untouched). The same
// routine was also compared with the original in a private interpreter run.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char _vt$16VIWndReadMessage[8];
static unsigned char object[0x8000];
static std::string trace;
extern "C" {
void __5VIWnd(void* self) { trace += "__5VIWnd:" + std::to_string(static_cast<unsigned char*>(self) - object) + ";"; }
void __10VIUIWindow(void* self) { trace += "__10VIUIWindow:" + std::to_string(static_cast<unsigned char*>(self) - object) + ";"; }
}

int main() {
    std::memset(object, 0x5a, sizeof object);
    unsigned char expect[0x8000];
    std::memset(expect, 0x5a, sizeof expect);
    { unsigned v = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$16VIWndReadMessage)); std::memcpy(expect + 32, &v, 4); }
    { unsigned v = 0u; std::memcpy(expect + 408, &v, 4); }
    { unsigned v = 1u; std::memcpy(expect + 16804, &v, 4); }
    __16VIWndReadMessage(object);
    bool ok = trace == "__5VIWnd:0;__10VIUIWindow:40;" && std::memcmp(object, expect, sizeof object) == 0;
    if (!ok) std::printf("trace %s\n", trace.c_str());
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
