// Host check for m_cd. Not a build-plan input.
//   c++ -std=c++17 -o check host_check.cpp && ./check
#include <cstdio>
#include "eznet_ctl_client.cpp"
int main() {
    int bad = 0;
    if (sizeof m_cd != 64) { std::puts("size"); ++bad; }
    for (unsigned char b : m_cd) if (b) { std::puts("not zero"); ++bad; break; }
    return bad;
}
