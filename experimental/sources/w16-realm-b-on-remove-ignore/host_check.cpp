// Host check only; not a build input. Build: c++ -std=c++17 -Wall -Wextra host_check.cpp
#include "on_remove_ignore.cpp"
#include <cstdio>
#include <cstring>
int main() {
    unsigned char buf[64], keep[64];
    for (int i = 0; i < 64; ++i) buf[i] = keep[i] = static_cast<unsigned char>(i * 7 + 3);
    onRemoveIgnore__Q212RealmService15RealmGatewayAPIUiiPv(7u, -5, buf);
    if (std::memcmp(buf, keep, sizeof buf) != 0) { std::puts("FAIL: memory written"); return 1; }
    std::puts("ok");
    return 0;
}
