// Host check only; not a build input. Build: c++ -std=c++17 -Wall -Wextra host_check.cpp
#include "on_get_user_stats.cpp"
#include <cstdio>
#include <cstring>
int main() {
    unsigned char buf[64], keep[64];
    for (int i = 0; i < 64; ++i) buf[i] = keep[i] = static_cast<unsigned char>(i * 7 + 3);
    onGetUserStats__Q212RealmService15RealmGatewayAPIUiiRCQ212RealmService9UserStatsPCPCQ212RealmService9UserStatsUiPv(7u, -5, *reinterpret_cast<const UserStats*>(buf), reinterpret_cast<const UserStats* const*>(buf), 7u, buf);
    if (std::memcmp(buf, keep, sizeof buf) != 0) { std::puts("FAIL: memory written"); return 1; }
    std::puts("ok");
    return 0;
}
