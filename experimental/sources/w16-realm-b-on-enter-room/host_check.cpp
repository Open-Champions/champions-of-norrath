// Host check only; not a build input. Build: c++ -std=c++17 -Wall -Wextra host_check.cpp
#include "on_enter_room.cpp"
#include <cstdio>
#include <cstring>
int main() {
    unsigned char buf[64], keep[64];
    for (int i = 0; i < 64; ++i) buf[i] = keep[i] = static_cast<unsigned char>(i * 7 + 3);
    onEnterRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv(7u, -5, reinterpret_cast<const unsigned short*>(buf), reinterpret_cast<const unsigned short*>(buf), reinterpret_cast<const unsigned short* const*>(buf), 7u, reinterpret_cast<const unsigned short* const*>(buf), 7u, buf);
    if (std::memcmp(buf, keep, sizeof buf) != 0) { std::puts("FAIL: memory written"); return 1; }
    std::puts("ok");
    return 0;
}
