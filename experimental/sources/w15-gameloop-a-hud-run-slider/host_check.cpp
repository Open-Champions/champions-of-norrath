// Host check for hudRunSlider__FiiiiiUifPii. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_ARENA host_check.cpp
#include <cstdio>
#include <vector>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "hud_run_slider.cpp"

unsigned char pad[88 * 4 + 16];
struct Rect { int a, b, c, d, fifth; unsigned color; };
static std::vector<Rect> rects;
extern "C" void rectangleDraw__FiiiiiUi(int a, int b, int c, int d, int fifth, unsigned color) { rects.push_back({a, b, c, d, fifth, color}); }

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)
static bool is(const Rect& r, int a, int b, int c, int d, unsigned color) { return r.a == a && r.b == b && r.c == c && r.d == d && r.fifth == 5 && r.color == color; }

int main() {
    using namespace norrath_aoc;
    int result = 99;
    // x 100, y 50, width 220, height 31 (cy = 65), fraction 0.5: px = trunc(200 * 0.5) = 100
    int r = hudRunSlider__FiiiiiUifPii(100, 50, 220, 31, 5, 0xc0112233u, 0.5f, &result, -1);
    CHECK(r == 0 && result == 0 && rects.size() == 4);
    CHECK(is(rects[0], 206, 53, 214, 77, 0x80606060u) && is(rects[1], 207, 54, 215, 78, 0x40000000u));
    CHECK(is(rects[2], 110, 64, 310, 66, 0xc0000000u) && is(rects[3], 100, 50, 320, 81, 0xc0112233u));
    rects.clear();
    store_word(pad + 16 + 88 * 2, 0x00008000u);
    CHECK(hudRunSlider__FiiiiiUifPii(0, 0, 40, 10, 5, 0, 0.99f, &result, 2) == -1);   // trunc(20 * 0.99) = 19
    CHECK(rects[0].a == 25 && rects[0].c == 33);
    store_word(pad + 16 + 88 * 2, 0x00402000u);
    CHECK(hudRunSlider__FiiiiiUifPii(0, 0, 40, 10, 5, 0, 0.0f, &result, 2) == 1);     // 0x400000 is in the first mask, 0x2000 in the second: the second test wins
    store_word(pad + 16 + 88 * 2, 0x00010000u);
    CHECK(hudRunSlider__FiiiiiUifPii(0, 0, 40, 10, 5, 0, 0.0f, &result, 2) == 0);     // in neither mask
    store_word(pad + 16 + 88 * 2, 0x00802000u);
    CHECK(hudRunSlider__FiiiiiUifPii(0, 0, 40, 10, 5, 0, 0.0f, &result, 2) == 1);
    store_word(pad + 16 + 88 * 2, 0x00800000u);
    CHECK(hudRunSlider__FiiiiiUifPii(0, 0, 40, 10, 5, 0, 0.0f, &result, 2) == 1);
    std::puts("ok");
    return 0;
}
