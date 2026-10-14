// Host check only; not a build input. Callees are recorders that log the callee name, the
// offset of the object they received and any flag argument; the object is pre-filled with a
// pattern so that every byte the routine changes beyond the listed words would show. The same
// logic was compared with the original in a private interpreter run (identical call logs and
// changed memory for each scenario).
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>


static unsigned char storage[0x1000];
static std::string trace;
static void rec(const char* name, void* self, int flag) {
    trace += name; trace += "@" + std::to_string(static_cast<unsigned char*>(self) - storage);
    if (flag >= 0) trace += "/" + std::to_string(flag);
    trace += ";";
}
extern "C" void __13VIStringTable(void* self) { rec("__13VIStringTable", self, -1); }
extern "C" void __6VIFont(void* self) { rec("__6VIFont", self, -1); }

int main() {
    bool ok = true;
    std::memset(storage, 0x5a, sizeof storage);
    {
        __4VIUI(storage);
        if (trace != "__13VIStringTable@0;__13VIStringTable@16;__13VIStringTable@32;__6VIFont@56;") { std::puts(trace.c_str()); ok = false; }
        { int w; std::memcpy(&w, storage + 72, 4); if (w != 0) ok = false; }
        { int w; std::memcpy(&w, storage + 76, 4); if (w != 0) ok = false; }
        { int w; std::memcpy(&w, storage + 80, 4); if (w != 0) ok = false; }
        for (int i = 0; i < 0x1000; ++i) if (!((i >= 72 && i < 76) || (i >= 76 && i < 80) || (i >= 80 && i < 84)) && storage[i] != 0x5a) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
