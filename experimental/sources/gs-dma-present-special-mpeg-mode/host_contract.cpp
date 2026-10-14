#include "special_mpeg_mode.cpp"

#include <cstdio>

int main() {
    if (specialMpegMode != 0) return 1;
    specialMpegMode = 1;
    if (specialMpegMode == 0) return 2;
    specialMpegMode = 0;
    std::puts("special mpeg mode contract passed");
    return 0;
}
