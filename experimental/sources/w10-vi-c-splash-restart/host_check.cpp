// Host check only; not a build input. Compared with the original in a private interpreter run
// (random object fills, identical call logs and changed memory).
#include "restart.cpp"
#include <cstdio>
#include <cstring>
static unsigned char storage[0x4000];
static int calls;
extern "C" void EndConnect__12VIWndConnect(void* self) { if (self == storage + 416) ++calls; }
int main() {
    std::memset(storage, 0x5a, sizeof storage);
    RestartConnect__11VIWndSplash(storage);
    bool ok = calls == 1;
    for (int i = 0; i < 0x4000; ++i) {
        const bool zero = (i >= 444 && i < 448) || (i >= 2436 && i < 2440) || (i >= 4804 && i < 4808);
        if (zero ? storage[i] != 0 : storage[i] != 0x5a) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
