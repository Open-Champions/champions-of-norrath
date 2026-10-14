// Host check for initPostSelect__Fv. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "init_post_select.cpp"

#include <cstdio>
#include <cstring>

int curPlayer, selectMode, lockAlpha;
float delta;
unsigned char playerSelectInfo[41 * 4];
static int seenCurrent = 99, seenTarget = 99;
void initPosition__Fii(int current, int target) { seenCurrent = current; seenTarget = target; }

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

int main() {
    curPlayer = 2;
    playerSelectInfo[41 * 2 + 32] = 0xfd;   // signed byte
    playerSelectInfo[41 * 1 + 32] = 9;
    initPostSelect__Fv();
    CHECK(seenCurrent == -3 && seenTarget == 0);
    CHECK(selectMode == 7 && lockAlpha == 1);
    unsigned word; std::memcpy(&word, &delta, 4);
    CHECK(word == 0x3d5d2f1au);
    std::puts("ok");
    return 0;
}
