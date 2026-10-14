// Host check only; not a build input.
#include "onmessage.cpp"
#include <cstdio>
#include <initializer_list>
#include <cstring>

unsigned char sKeyMap[66];
int sCompactKeyboard;
static int base_calls, time_calls;
extern "C" unsigned long long GetTime__7VITimer() { ++time_calls; return 0x1122334455667788ull; }
extern "C" void OnMessage__5VIWndP5VIWndiii(void*, void*, int, int, void*) { ++base_calls; }

int main() {
    bool ok = true;
    alignas(8) unsigned char object[512];
    for (int flag : {0, 9}) {
        std::memset(object, 0x5a, sizeof object);
        std::memset(sKeyMap, 0x11, sizeof sKeyMap);
        sCompactKeyboard = 0; base_calls = time_calls = 0;
        reinterpret_cast<int*>(object)[110] = flag;
        unsigned char before[512];
        std::memcpy(before, object, sizeof object);
        OnMessage__9VIWndEditP5VIWndiii(object, object + 1, 295, 3, object + 4);  // p2 is not this
        ok = ok && std::memcmp(before, object, sizeof object) == 0 && !base_calls && !time_calls && sCompactKeyboard == 0;
        OnMessage__9VIWndEditP5VIWndiii(object, object + 1, 294, 3, object + 4);
        ok = ok && base_calls == 1 && std::memcmp(before, object, sizeof object) == 0;
        OnMessage__9VIWndEditP5VIWndiii(object, object + 1, 295, 3, object);
        ok = ok && base_calls == 1 && time_calls == 1 && sCompactKeyboard == 1 && sKeyMap[64] == (flag ? 6 : 0) && sKeyMap[63] == 0x11 && sKeyMap[65] == 0x11;
        const int* w = reinterpret_cast<int*>(object);
        for (int i = 0; i < 128; ++i) {
            bool cleared = i == 82 || i == 83 || i == 86 || i == 87 || i == 88 || i == 90 || i == 91 || i == 96 || i == 97 || i == 11 || i == 12 || i == 13;
            if (cleared) ok = ok && w[i] == 0;
            else if (i == 92) ok = ok && w[i] == 1;
            else if (i == 94) ok = ok && *reinterpret_cast<const unsigned long long*>(w + 94) == 0x1122334455667788ull;
            else if (i == 95) continue;
            else ok = ok && w[i] == (i == 110 ? flag : 0x5a5a5a5a);
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
