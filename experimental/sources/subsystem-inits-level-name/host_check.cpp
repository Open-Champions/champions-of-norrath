// Host contract for engineCurrentLevelName. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" char engineCurrentLevelName[80];

int main() {
    char zeros[80] = {};
    if (std::memcmp(engineCurrentLevelName, zeros, 80) != 0) {
        std::fprintf(stderr, "fail: level name bytes\n");
        return 1;
    }
    engineCurrentLevelName[0] = 'a';
    engineCurrentLevelName[79] = 'z';
    if (engineCurrentLevelName[0] != 'a' || engineCurrentLevelName[79] != 'z') {
        std::fprintf(stderr, "fail: level name extent\n");
        return 1;
    }
    return 0;
}
