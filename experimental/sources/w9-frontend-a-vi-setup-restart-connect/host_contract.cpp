#include "restart_connect.cpp"

#include <cstdio>
#include <string>

static std::string trace;
static unsigned char* object_base;

static std::string at(void* pointer) { return std::to_string(static_cast<unsigned char*>(pointer) - object_base); }

extern "C" void SetRoot__15VIWindowManagerP5VIWnd(void* object, void* a0) {
    trace += "SetRoot__15VIWindowManagerP5VIWnd@" + at(object) + "," + at(a0) + ";";
}
extern "C" void SetFocus__5VIWndP5VIWndb(void* object, void* a0, bool a1) {
    trace += "SetFocus__5VIWndP5VIWndb@" + at(object) + "," + at(a0) + "," + std::to_string(static_cast<int>(a1)) + ";";
}
extern "C" void Free__12VIWndPatcher(void* object) {
    trace += "Free__12VIWndPatcher@" + at(object) + ";";
}
extern "C" void Init__12VIWndPatcher(void* object) {
    trace += "Init__12VIWndPatcher@" + at(object) + ";";
}
extern "C" void RestartConnect__11VIWndSplash(void* object) {
    trace += "RestartConnect__11VIWndSplash@" + at(object) + ";";
}

int main() {
    static unsigned char storage[0x20000];
    object_base = storage;
    RestartConnect__7VISetup(storage);
    if (trace != "SetRoot__15VIWindowManagerP5VIWnd@44900,45032;SetFocus__5VIWndP5VIWndb@44900,45032,1;Free__12VIWndPatcher@54704;Init__12VIWndPatcher@54704;RestartConnect__11VIWndSplash@45032;") {
        std::puts(trace.c_str());
        return 1;
    }
    std::puts("vi setup restart connect passed");
    return 0;
}
