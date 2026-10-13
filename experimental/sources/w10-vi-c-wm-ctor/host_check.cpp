// Host check only; not a build input. Callees are recorders that log the callee name, the
// offset of the object they received and any flag argument; the object is pre-filled with a
// pattern so that every byte the routine changes beyond the listed words would show. The same
// logic was compared with the original in a private interpreter run (identical call logs and
// changed memory for each scenario).
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char _vt$15VIWindowManager[8];
static unsigned char storage[0x1000];
static std::string trace;
static void rec(const char* name, void* self, int flag) {
    trace += name; trace += "@" + std::to_string(static_cast<unsigned char*>(self) - storage);
    if (flag >= 0) trace += "/" + std::to_string(flag);
    trace += ";";
}
extern "C" void __5VIWnd(void* self) { rec("__5VIWnd", self, -1); }

int main() {
    bool ok = true;
    std::memset(storage, 0x5a, sizeof storage);
    {
        __15VIWindowManager(storage);
        if (trace != "__5VIWnd@0;") { std::puts(trace.c_str()); ok = false; }
        { unsigned w; std::memcpy(&w, storage + 32, 4); if (w != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$15VIWindowManager))) ok = false; }
        for (int i = 0; i < 0x1000; ++i) if (!((i >= 32 && i < 36)) && storage[i] != 0x5a) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
