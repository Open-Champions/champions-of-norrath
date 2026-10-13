// Host check only; not a build input. Compared with the original in a private interpreter run
// (random object fills, identical call logs and changed memory).
#include "free.cpp"
#include <cstdio>
#include <string>
static unsigned char storage[0x1000];
static std::string trace;
extern "C" void RemoveChild__5VIWndP5VIWnd(void* self, void* child) {
    trace += "remove@" + std::to_string(static_cast<unsigned char*>(self) - storage) + "," +
             std::to_string(static_cast<unsigned char*>(child) - storage) + ";";
}
extern "C" void Free__12VIWndMessage(void* self) {
    trace += "free@" + std::to_string(static_cast<unsigned char*>(self) - storage) + ";";
}
int main() {
    Free__9VIWndDnas(storage);
    const bool ok = trace == "remove@0,1176;free@1176;";
    if (!ok) std::puts(trace.c_str());
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
