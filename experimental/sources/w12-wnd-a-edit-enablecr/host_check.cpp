// Host check only; not a build input. Also compared with the original in a private interpreter.
#include "enablecr.cpp"
#include <cstdio>
#include <cstring>
extern "C" unsigned char sKeyMap[66];
unsigned char sKeyMap[66];
int main() {
    unsigned char object[512];
    std::memset(object, 0x5a, sizeof object);
    std::memset(sKeyMap, 0x11, sizeof sKeyMap);
    EnableCr__9VIWndEditi(object, 3);
    bool ok = *reinterpret_cast<int*>(object + 440) == 3 && sKeyMap[64] == 6 && sKeyMap[63] == 0x11 && sKeyMap[65] == 0x11;
    EnableCr__9VIWndEditi(object, 0);
    ok = ok && *reinterpret_cast<int*>(object + 440) == 0 && sKeyMap[64] == 0 && sKeyMap[63] == 0x11;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
