// Host check only; not a build input. Callees are recorders that log the callee name, the
// offset of the object they received and any flag argument; the object is pre-filled with a
// pattern so that every byte the routine changes beyond the listed words would show. The same
// logic was compared with the original in a private interpreter run (identical call logs and
// changed memory for each scenario).
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char _vt$9VIWndEula[8];
static unsigned char storage[0x1000];
static std::string trace;
static void rec(const char* name, void* self, int flag) {
    trace += name; trace += "@" + std::to_string(static_cast<unsigned char*>(self) - storage);
    if (flag >= 0) trace += "/" + std::to_string(flag);
    trace += ";";
}
extern "C" void Free__9VIWndEula(void* self) { rec("Free__9VIWndEula", self, -1); }
extern "C" void _$_12VIWndMessage(void* self, int flags) { rec("_$_12VIWndMessage", self, flags); }
extern "C" void _$_10VIUIWindow(void* self, int flags) { rec("_$_10VIUIWindow", self, flags); }
extern "C" void _$_5VIWnd(void* self, int flags) { rec("_$_5VIWnd", self, flags); }
extern "C" void __builtin_delete(void* pointer) { rec("delete", pointer, -1); }

int main() {
    bool ok = true;
    for (int flags = 0; flags < 4; ++flags) {
        trace.clear();
        std::memset(storage, 0x5a, sizeof storage);
        dtor_9VIWndEula(storage, flags);
        const std::string want = std::string("Free__9VIWndEula@0;_$_12VIWndMessage@408/2;_$_10VIUIWindow@40/2;_$_5VIWnd@0/0;") + ((flags & 1) ? "delete@0;" : "");
        if (trace != want) { std::puts(trace.c_str()); ok = false; }
        { unsigned w; std::memcpy(&w, storage + 32, 4); if (w != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$9VIWndEula))) ok = false; }
        for (int i = 0; i < 0x1000; ++i) if (!((i >= 32 && i < 36)) && storage[i] != 0x5a) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
