#include <cstdio>
#include <cstring>
extern "C" signed char mt_tiledata[320];
signed char mt_tiledata[320];
#include "get_megatile_center.cpp"
static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
static void tile(int index, signed char b0, signed char b1, signed char b2, signed char b3, signed char b4, signed char b5,
                 signed char b6, signed char b7, signed char b8) {
    signed char* t = mt_tiledata + 10 * index;
    t[0] = b0; t[1] = b1; t[2] = b2; t[3] = b3; t[4] = b4; t[5] = b5; t[6] = b6; t[7] = b7; t[8] = b8; t[9] = 0;
}
static Point2 run(float x, float y, Point2* argument) {
    Point2 result = {-1.0f, -1.0f};
    *argument = {x * 720.0f, y * 720.0f};
    getMegatileCenter__FG6Point2(&result, argument);
    return result;
}
int main() {
    std::memset(mt_tiledata, 0, sizeof mt_tiledata);
    // Tile 0 covers x in [3, 7], y in [-2, 3]. Tile 1 covers x in [10, 12], y in [0, 2].
    // Tile 2 ends the list; tile 3 would cover x and y in [-50, -40] if the end were ignored.
    tile(0, 7, 6, 0, 5, 4, 8, 9, 3, -2);
    tile(1, 1, 2, 0, 2, 2, 3, 4, 10, 0);
    tile(2, 0, 0, -1, 0, 0, 0, 0, 0, 0);
    tile(3, 0, 0, 0, 10, 10, 0, 0, -50, -50);
    const float scale = __builtin_bit_cast(float, 0x3ab60b60u);
    Point2 argument;
    Point2 r = run(5.0f, 1.0f, &argument);
    expect(argument.x == 5.0f * 720.0f * scale && argument.y == 1.0f * 720.0f * scale, "argument is scaled in place");
    expect(r.x == 3600.0f && r.y == 360.0f, "tile 0 result");
    r = run(11.0f, 1.0f, &argument);
    expect(r.x == 7920.0f && r.y == 720.0f, "tile 1 result");
    r = run(0.0f, 0.0f, &argument);
    expect(r.x == argument.x && r.y == argument.y, "no match returns the scaled argument");
    r = run(-45.0f, -45.0f, &argument);
    expect(r.x == argument.x && r.y == argument.y, "the end record stops the search");
    r = run(5.0f, 5.0f, &argument);
    expect(r.x == argument.x && r.y == argument.y, "inside x but outside y does not match");
    mt_tiledata[2] = -1;
    r = run(5.0f, 1.0f, &argument);
    expect(r.x == argument.x && r.y == argument.y, "a -1 in the first record ends the list at once");
    return failures;
}
