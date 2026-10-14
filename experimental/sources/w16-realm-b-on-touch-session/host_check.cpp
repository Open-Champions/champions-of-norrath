// Host check only; not a build input. Build: c++ -std=c++17 -Wall -Wextra host_check.cpp
#include "on_touch_session.cpp"
#include <cstdio>
#include <initializer_list>
#include <cstring>
static int timeCalls;
extern "C" unsigned long long GetTime__7VITimer() { ++timeCalls; return 0x1122334455667788ull; }
int main() {
    bool ok = true;
    for (int b : {-1, 0, 1, 2, 3, 100}) {
        alignas(8) unsigned char object[9760];
        std::memset(object, 0x5a, sizeof object);
        unsigned char before[9760];
        std::memcpy(before, object, sizeof object);
        timeCalls = 0;
        onTouchSession__16VIRealmInterfaceUiiPv(object, 77, b, nullptr);
        if (b == 1) {
            ok = ok && timeCalls == 0 && std::memcmp(object, before, sizeof object) == 0;
        } else {
            unsigned long long stamp;
            std::memcpy(&stamp, object + 9728, 8);
            std::memcpy(before + 9728, &stamp, 8);
            ok = ok && timeCalls == 1 && stamp == 0x1122334455667788ull && std::memcmp(object, before, sizeof object) == 0;
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
