// Host check for resetName__Fv. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "reset_name.cpp"

#include <cstdio>

int curPlayer, namePos;
unsigned char playerSelectInfo[41 * 4];
void* memset(void* destination, int value, unsigned size) {
    unsigned char* p = static_cast<unsigned char*>(destination);
    for (unsigned i = 0; i < size; ++i) p[i] = static_cast<unsigned char>(value);
    return destination;
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

int main() {
    for (unsigned i = 0; i < sizeof playerSelectInfo; ++i) playerSelectInfo[i] = 0x55;
    curPlayer = 1; namePos = 9;
    resetName__Fv();
    CHECK(namePos == 0);
    for (int i = 0; i < 41 * 4; ++i) {
        bool cleared = i >= 41 && i < 41 + 32;
        CHECK(playerSelectInfo[i] == (cleared ? 0 : 0x55));
    }
    std::puts("ok");
    return 0;
}
