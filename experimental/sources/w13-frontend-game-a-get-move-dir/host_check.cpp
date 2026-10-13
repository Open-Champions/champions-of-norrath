// Host check for getMoveDir__FiiiP12screenStruct. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin host_check.cpp
#include "get_move_dir.cpp"

#include <cstdio>
#include <cstring>
#include <initializer_list>

extern "C" unsigned char netLocalPad[4 * 88] = {};

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

static int call(int pad, unsigned bits, int columns, int rows, int latch, int row, int col, int* latchAfter) {
    std::memset(netLocalPad, 0, sizeof netLocalPad);
    unsigned other = 0xffffffffu;  // other pads and other words must be ignored
    for (int p = 0; p < 4; ++p)
        if (p != pad) std::memcpy(netLocalPad + p * 88 + 8, &other, 4);
    std::memcpy(netLocalPad + pad * 88 + 8, &bits, 4);
    std::memcpy(netLocalPad + pad * 88 + 12, &other, 4);
    int screen[10] = {};
    screen[5] = latch; screen[6] = row; screen[7] = col;
    screen[0] = 0x1111; screen[9] = 0x2222;
    int r = getMoveDir__FiiiP12screenStruct(pad, columns, rows, screen);
    *latchAfter = screen[5];
    if (screen[0] != 0x1111 || screen[9] != 0x2222 || screen[6] != row || screen[7] != col) return 1000;
    return r;
}

int main() {
    int after;
    // 1: either button of the first pair; row limit is rows - 2
    for (unsigned b : {0x4000u, 0x200000u, 0x4000u | 0x200000u}) {
        CHECK(call(0, b, 5, 6, 0, 4, 0, &after) == 1 && after == 1);
        CHECK(call(0, b, 5, 6, 0, 5, 0, &after) == 0 && after == 0);
        CHECK(call(0, b, 5, 6, 0, 0, 0, &after) == 1);
        CHECK(call(0, b, 5, 6, 3, 0, 0, &after) == 3 && after == 3);   // latched: kept, returned
    }
    // -1: needs row > 0
    for (unsigned b : {0x1000u, 0x100000u}) {
        CHECK(call(1, b, 5, 6, 0, 1, 0, &after) == -1 && after == -1);
        CHECK(call(1, b, 5, 6, 0, 0, 0, &after) == 0 && after == 0);
        CHECK(call(1, b, 5, 6, 2, 3, 0, &after) == 2);
    }
    // 4: needs column < columns - 1
    for (unsigned b : {0x2000u, 0x800000u}) {
        CHECK(call(2, b, 5, 6, 0, 0, 3, &after) == 4 && after == 4);
        CHECK(call(2, b, 5, 6, 0, 0, 4, &after) == 0 && after == 0);
        CHECK(call(2, b, 5, 6, -1, 0, 0, &after) == -1);
    }
    // 2: needs column > 0
    for (unsigned b : {0x8000u, 0x400000u}) {
        CHECK(call(3, b, 5, 6, 0, 0, 1, &after) == 2 && after == 2);
        CHECK(call(3, b, 5, 6, 0, 0, 0, &after) == 0 && after == 0);
        CHECK(call(3, b, 5, 6, 7, 0, 3, &after) == 7);
    }
    // no relevant buttons: the latch is returned unchanged
    CHECK(call(0, 0x0, 5, 6, 0, 2, 2, &after) == 0);
    CHECK(call(0, ~(0x4000u | 0x200000u | 0x1000u | 0x100000u | 0x2000u | 0x800000u | 0x8000u | 0x400000u), 5, 6, 9, 2, 2, &after) == 9);
    // priority: the first pair wins over the later ones, the second over the third and fourth
    CHECK(call(0, 0x4000u | 0x1000u, 5, 6, 0, 3, 3, &after) == 1);
    CHECK(call(0, 0x1000u | 0x2000u | 0x8000u, 5, 6, 0, 3, 3, &after) == -1);
    CHECK(call(0, 0x2000u | 0x8000u, 5, 6, 0, 3, 3, &after) == 4);
    // when the winning case cannot apply, a later pair does not take over
    CHECK(call(0, 0x4000u | 0x8000u, 5, 6, 0, 5, 3, &after) == 0 && after == 0);
    CHECK(call(0, 0x1000u | 0x2000u, 5, 6, 0, 0, 0, &after) == 0 && after == 0);
    CHECK(call(0, 0x2000u | 0x8000u, 5, 6, 0, 0, 4, &after) == 0 && after == 0);
    // signed comparisons: a negative row or column counts as not above zero, and the limit can be small
    CHECK(call(0, 0x1000u, 5, 6, 0, -3, 0, &after) == 0);
    CHECK(call(0, 0x8000u, 5, 6, 0, 0, -3, &after) == 0);
    CHECK(call(0, 0x4000u, 5, 1, 0, -1, 0, &after) == 1);
    CHECK(call(0, 0x2000u, 0, 6, 0, 0, -2, &after) == 4);
    std::puts("get move dir passed");
    return 0;
}
