// Host check for getHairStyle__Fi. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "get_hair_style.cpp"

#include <cstdio>

signed char hairStyleSetting[16];
unsigned char targets[636 * 16];

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

int main() {
    hairStyleSetting[2] = 3; targets[636 * 2 + 18] = 4;
    CHECK(getHairStyle__Fi(2) == 7);
    hairStyleSetting[5] = -2; targets[636 * 5 + 18] = 0xfe;   // both bytes are signed
    CHECK(getHairStyle__Fi(5) == -4);
    hairStyleSetting[7] = 127; targets[636 * 7 + 18] = 127;
    CHECK(getHairStyle__Fi(7) == 254);
    CHECK(getHairStyle__Fi(0) == 0);
    std::puts("ok");
    return 0;
}
