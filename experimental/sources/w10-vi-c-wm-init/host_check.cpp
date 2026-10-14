// Host check only; not a build input. Compared with the original in a private interpreter run
// (random object fills, identical call logs and changed memory).
#define memset memset_hook
#include "init.cpp"
#undef memset
#include <cstdio>
#include <cstring>
static unsigned char storage[256];
static int calls;
extern "C" void* memset_hook(void* destination, int value, unsigned count) {
    ++calls;
    if (destination != storage + 48 || value != 0 || count != 84) calls = 100;
    unsigned char* bytes = static_cast<unsigned char*>(destination);
    for (unsigned i = 0; i < count; ++i) bytes[i] = static_cast<unsigned char>(value);
    return destination;
}
int main() {
    for (int i = 0; i < 256; ++i) storage[i] = 0x5a;
    const int result = Init__15VIWindowManagerP5VIPadP10VIKeyboard(storage, storage + 1, storage + 2);
    unsigned pad, keyboard;
    std::memcpy(&pad, storage + 40, 4);
    std::memcpy(&keyboard, storage + 44, 4);
    bool ok = result == 0 && calls == 1 && pad == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(storage + 1)) &&
              keyboard == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(storage + 2));
    for (int i = 0; i < 256; ++i) {
        const bool cleared = i >= 48 && i < 132;
        const bool stored = i >= 40 && i < 48;
        if (cleared ? storage[i] != 0 : (!stored && storage[i] != 0x5a)) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
