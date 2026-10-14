// Host check for boxDraw__FiiiiiUii. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "box_draw.cpp"

#include <cstdio>
#include <vector>

struct Rect { int a, b, c, d, fifth; unsigned color; };
static std::vector<Rect> rects;
extern "C" void rectangleDraw__FiiiiiUi(int x0, int y0, int x1, int y1, int fifth, unsigned color) {
    rects.push_back({x0, y0, x1, y1, fifth, color});
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)
static bool is(const Rect& r, int a, int b, int c, int d) { return r.a == a && r.b == b && r.c == c && r.d == d && r.fifth == 7 && r.color == 0xff00ffu; }

int main() {
    boxDraw__FiiiiiUii(10, 20, 50, 60, 7, 0xff00ffu, 0);
    CHECK(rects.size() == 4);
    CHECK(is(rects[0], 10, 20, 50, 21) && is(rects[1], 10, 59, 50, 60) && is(rects[2], 10, 20, 11, 60) && is(rects[3], 49, 20, 50, 60));
    rects.clear();
    boxDraw__FiiiiiUii(10, 20, 50, 60, 7, 0xff00ffu, 3);
    CHECK(rects.size() == 4);
    CHECK(is(rects[0], 11, 20, 49, 21) && is(rects[1], 11, 59, 49, 60) && is(rects[2], 10, 21, 11, 59) && is(rects[3], 49, 21, 50, 59));
    std::puts("ok");
    return 0;
}
