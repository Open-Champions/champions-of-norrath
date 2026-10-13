#include "stats_screen.cpp"
#include <cstdio>

int main() {
    int bad = sizeof(statsScreen) != 2 * 116;
    for (unsigned i = 0; i < sizeof(statsScreen); ++i) bad |= statsScreen[i] != 0;
    std::printf(bad ? "FAIL\n" : "ok: 232 zero bytes\n");
    return bad;
}
