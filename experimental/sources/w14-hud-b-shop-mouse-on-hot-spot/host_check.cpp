// Host check for shopMouseOnHotSpot__FP12screenStruct. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "shop_mouse_on_hot_spot.cpp"

#include <cstdio>
#include <cstring>

int shopRunning;

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

static int call(int base, float horizontal, float vertical, int running) {
    alignas(8) unsigned char screen[96];
    std::memset(screen, 0xa5, sizeof screen);
    std::memcpy(screen, &base, 4);
    std::memcpy(screen + 68, &horizontal, 4);
    std::memcpy(screen + 72, &vertical, 4);
    shopRunning = running;
    unsigned char copy[96];
    std::memcpy(copy, screen, sizeof copy);
    int r = shopMouseOnHotSpot__FP12screenStruct(screen);
    if (std::memcmp(copy, screen, sizeof copy) != 0) return -1000;
    return r;
}

int main() {
    // vertical limit
    CHECK(call(0, 200.f, 97.9f, 0) == (200 - 16) / 47);
    CHECK(call(0, 200.f, 98.f, 0) == -1);
    CHECK(call(0, 200.f, 98.f, 1) == -1);
    CHECK(call(0, 200.f, -50.f, 0) == (200 - 16) / 47);
    // tab bar, not running: left 16, width 47, cap 5
    CHECK(call(0, 15.f, 0.f, 0) == 0);
    CHECK(call(0, 16.f + 46.f, 0.f, 0) == 0);
    CHECK(call(0, 16.f + 47.f, 0.f, 0) == 1);
    CHECK(call(0, 16.f + 47.f * 5, 0.f, 0) == 5);
    CHECK(call(0, 16.f + 47.f * 6, 0.f, 0) == 5);
    CHECK(call(0, 1000.f, 0.f, 0) == 5);
    // running: left 91, cap 2
    CHECK(call(0, 90.f, 0.f, 1) == 0);
    CHECK(call(0, 91.f + 47.f, 0.f, 1) == 1);
    CHECK(call(0, 91.f + 94.f, 0.f, 1) == 2);
    CHECK(call(0, 91.f + 141.f, 0.f, 1) == 2);
    CHECK(call(0, 20.f, 0.f, 1) == 0);
    // the integer at offset 0 is subtracted, and fractions truncate toward zero
    CHECK(call(100, 100.f + 16.f + 47.f, 0.f, 0) == 1);
    CHECK(call(100, 100.f + 16.f + 46.9f, 0.f, 0) == 0);
    CHECK(call(-40, 23.f, 0.f, 0) == 1);
    CHECK(call(30, 30.f + 15.5f, 0.f, 0) == 0);
    CHECK(call(0, 16.f - 10.f, 0.f, 0) == 0);  // negative offset below the first tab: 0 / 47 style
    CHECK(call(0, -500.f, 0.f, 0) == 0);
    std::puts("ok");
    return 0;
}
