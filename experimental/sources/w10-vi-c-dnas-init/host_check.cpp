// Host check only; not a build input. Compared with the original in a private interpreter run
// (random object fills, identical call logs and changed memory).
#include "init.cpp"
#include <cstdio>
#include <cstring>
static unsigned char storage[0x1000];
static int step;
static bool ok = true;
extern "C" void Init__12VIWndMessage(void* self) { if (self != storage + 1176 || step++ != 0) ok = false; }
extern "C" void AddChild__5VIWndP5VIWnd(void* self, void* child) {
    int word;
    std::memcpy(&word, storage + 1176 + 28, 4);
    if (self != storage || child != storage + 1176 || step++ != 1 || word != 0) ok = false;
}
int main() {
    std::memset(storage, 0x5a, sizeof storage);
    const int result = Init__9VIWndDnasP7VIScene(storage, storage + 5);
    unsigned scene; int minus_one;
    std::memcpy(&scene, storage + 776, 4);
    std::memcpy(&minus_one, storage + 780, 4);
    if (result != 0 || step != 2 || scene != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(storage + 5)) || minus_one != -1) ok = false;
    for (int i = 0; i < 0x1000; ++i) {
        const bool written = (i >= 776 && i < 784) || (i >= 1204 && i < 1208);
        if (!written && storage[i] != 0x5a) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
