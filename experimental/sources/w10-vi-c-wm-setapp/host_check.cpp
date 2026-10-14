// Host check only; not a build input. Compared with the original in a private interpreter run
// (random object fills, identical call logs and changed memory).
#include "setapp.cpp"
#include <cstdio>
#include <cstring>
int main() {
    unsigned char storage[64];
    std::memset(storage, 0x5a, sizeof storage);
    SetAppPtr__5VIWndPv(storage, storage + 7);
    unsigned word;
    std::memcpy(&word, storage + 24, 4);
    bool ok = word == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(storage + 7));
    for (int i = 0; i < 64; ++i)
        if ((i < 24 || i >= 28) && storage[i] != 0x5a) ok = false;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
