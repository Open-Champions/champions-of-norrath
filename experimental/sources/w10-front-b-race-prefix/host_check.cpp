#include "race_prefix.cpp"
#include <cstdio>
#include <cstring>

int main() {
    const char* want[10] = {"bm", "bf", "wem", "wef", "hem", "hef", "em", "ef", "dem", "def"};
    int bad = sizeof(racePrefix) / sizeof(racePrefix[0]) != 10;
    for (int i = 0; i < 10; ++i) bad |= std::strcmp(racePrefix[i], want[i]) != 0;
    std::printf(bad ? "FAIL\n" : "ok: ten prefixes\n");
    return bad;
}
