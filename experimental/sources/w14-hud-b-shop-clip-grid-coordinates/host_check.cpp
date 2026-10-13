// Host check for shopClipGridCoordinates__FPiN30. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "shop_clip_grid_coordinates.cpp"

#include <cstdio>

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

static int clamp(int v) { return v < 105 ? 105 : v > 600 ? 600 : v; }

int main() {
    const int values[] = {-100000, -1, 0, 104, 105, 106, 300, 599, 600, 601, 100000};
    for (int a : values) {
        for (int b : values) {
            int first = a, second = b, third = a + 7, fourth = b;
            shopClipGridCoordinates__FPiN30(&first, &second, &third, &fourth);
            CHECK(first == a && third == a + 7);
            CHECK(second == clamp(b) && fourth == clamp(b));
            int x = a, y = b, z = 5, w = a;
            shopClipGridCoordinates__FPiN30(&x, &y, &z, &w);
            CHECK(x == a && z == 5 && y == clamp(b) && w == clamp(a));
        }
        int same = a, other = 3;
        shopClipGridCoordinates__FPiN30(&other, &same, &other, &same);
        CHECK(same == clamp(a) && other == 3);
    }
    std::puts("ok");
    return 0;
}
