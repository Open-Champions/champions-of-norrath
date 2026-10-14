// Host check for the configuration and random-state words. Not a build-plan input.
//   c++ -std=c++17 -o check host_check.cpp && ./check
// The expected bytes are the initial bytes observed in the pinned image.
#include <cstdio>
#include <cstring>
#include "config_rand_words.cpp"

int main() {
    int bad = 0;
    const unsigned char want_config[8] = {0x1c, 0x02, 0, 0, 0, 0, 0, 0};
    if (std::memcmp(gConfig, want_config, sizeof want_config) != 0) { std::puts("gConfig bytes"); ++bad; }
    // The time-zone reader takes the signed halfword at offset 0, which is 540.
    if (*reinterpret_cast<const short*>(gConfig) != 540) { std::puts("gConfig halfword"); ++bad; }
    if (gConfig[6] != 0) { std::puts("gConfig summer byte"); ++bad; }
    if (localRandState != 0x000e18cd) { std::puts("localRandState"); ++bad; }
    if (iRandState != 0) { std::puts("iRandState"); ++bad; }
    if (bad == 0) std::puts("config and random words passed");
    return bad;
}
