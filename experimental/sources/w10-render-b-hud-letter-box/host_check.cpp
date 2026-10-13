// Host contract for hudLetterBox__Fv. Not an EE plan input.
#include <cstdio>
extern "C" {
int engineScreenSlide;
struct Call { int a, b, c, d, e; unsigned f; };
static Call calls[4];
static int n;
void rectangleDraw__FiiiiiUi(int x, int y, int w, int h, int l, unsigned c) {
    if (n < 4) calls[n] = {x, y, w, h, l, c};
    ++n;
    if (n == 1) engineScreenSlide = 100;  // must be read only after the first call
}
void hudLetterBox__Fv();
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    engineScreenSlide = 5;
    hudLetterBox__Fv();
    expect(n == 2, "two calls");
    expect(calls[0].a == 0 && calls[0].b == 0 && calls[0].c == 640 && calls[0].d == 58 && calls[0].e == 7 &&
               calls[0].f == 0x80000000u,
           "first bar");
    expect(calls[1].a == 0 && calls[1].b == 390 && calls[1].c == 640 && calls[1].d == 648 && calls[1].e == 7 &&
               calls[1].f == 0x80000000u,
           "second bar uses the slide after the first call");
    if (failures) return 1;
    std::printf("hudLetterBox contract passed\n");
    return 0;
}
