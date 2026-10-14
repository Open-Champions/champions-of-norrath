// Host check for lightGetAmbient__FPs. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "light_get_ambient.cpp"

#include <cstdio>

unsigned char scene[64];

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

int main() {
    scene[47] = 9; scene[48] = 200; scene[49] = 1; scene[50] = 255; scene[51] = 128; scene[52] = 7;
    short out[6] = {-1, -1, -1, -1, -1, -1};
    lightGetAmbient__FPs(out);
    CHECK(out[0] == 200 && out[1] == 1 && out[2] == 255 && out[3] == 128);
    CHECK(out[4] == -1 && out[5] == -1);
    std::puts("ok");
    return 0;
}
