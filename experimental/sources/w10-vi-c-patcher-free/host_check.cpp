// Host check only; not a build input. Compared with the original in a private interpreter run
// (random object fills, identical call logs and changed memory).
#include "free.cpp"
#include <cstdio>
#include <string>
static unsigned char storage[0x4000];
static std::string trace;
static std::string at(void* p) { return std::to_string(static_cast<unsigned char*>(p) - storage); }
extern "C" void RemoveChild__5VIWndP5VIWnd(void* self, void* child) { trace += "rm@" + at(self) + "," + at(child) + ";"; }
extern "C" void Free__12VIWndMessage(void* self) { trace += "free@" + at(self) + ";"; }
int main() {
    Free__12VIWndPatcher(storage);
    const bool ok = trace == "rm@0,2312;rm@0,1176;free@1176;";
    if (!ok) std::puts(trace.c_str());
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
