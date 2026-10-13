// Host check only; not a build input. Callees are recorders that log the callee name, the
// offset of the object they received and any flag argument; the object is pre-filled with a
// pattern so that every byte the routine changes beyond the listed words would show. The same
// logic was compared with the original in a private interpreter run (identical call logs and
// changed memory for each scenario).
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char _vt$12VIWndPatcher[8];
static unsigned char storage[0x4000];
static std::string trace;
static void rec(const char* name, void* self, int flag) {
    trace += name; trace += "@" + std::to_string(static_cast<unsigned char*>(self) - storage);
    if (flag >= 0) trace += "/" + std::to_string(flag);
    trace += ";";
}
extern "C" void __5VIWnd(void* self) { rec("__5VIWnd", self, -1); }
extern "C" void __10VIUIWindow(void* self) { rec("__10VIUIWindow", self, -1); }
extern "C" void __12VIWndMessage(void* self) { rec("__12VIWndMessage", self, -1); }
extern "C" void __16VIWndReadMessage(void* self) { rec("__16VIWndReadMessage", self, -1); }

int main() {
    bool ok = true;
    std::memset(storage, 0x5a, sizeof storage);
    {
        __12VIWndPatcher(storage);
        if (trace != "__5VIWnd@0;__10VIUIWindow@40;__10VIUIWindow@408;__10VIUIWindow@776;__12VIWndMessage@1176;__16VIWndReadMessage@2312;") { std::puts(trace.c_str()); ok = false; }
        { unsigned w; std::memcpy(&w, storage + 32, 4); if (w != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$12VIWndPatcher))) ok = false; }
        for (int i = 0; i < 0x4000; ++i) if (!((i >= 32 && i < 36)) && storage[i] != 0x5a) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
