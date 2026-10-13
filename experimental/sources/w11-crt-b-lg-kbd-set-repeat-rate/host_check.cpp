// Host comparison only. Not an EE build input.
#include <cstdio>
#include <cstring>
extern "C" { alignas(8) unsigned char GLibData[0x668]; }
#include "lg_kbd_set_repeat_rate.cpp"
static int failures;
static void expect(unsigned a, unsigned b, int flag, unsigned long long rate, int delay) {
    std::memset(GLibData, 0x5a, sizeof GLibData);
    unsigned char before[sizeof GLibData];
    std::memcpy(before, GLibData, sizeof before);
    int r = lgKbdSetRepeatRate(a, b);
    int f, d; unsigned long long v;
    std::memcpy(&f, GLibData + 96, 4); std::memcpy(&d, GLibData + 120, 4); std::memcpy(&v, GLibData + 112, 8);
    bool changed_ok = true;
    for (unsigned i = 0; i < sizeof before; ++i) {
        bool owned = (i >= 96 && i < 100) || (flag && ((i >= 112 && i < 120) || (i >= 120 && i < 124)));
        if (!owned && before[i] != GLibData[i]) changed_ok = false;
    }
    bool ok = r == 0 && f == flag && changed_ok && (!flag || (v == rate && d == delay));
    if (!ok) { std::printf("(%u,%u) wrong: flag %d rate %llu delay %d\n", a, b, f, v, d); ++failures; }
}
int main() {
    expect(0, 0, 0, 0, 0);
    expect(0, 50, 0, 0, 0);
    expect(500, 0, 0, 0, 0);
    expect(500, 50, 1, 50, 500);
    expect(99, 19, 1, 20, 100);
    expect(100, 20, 1, 20, 100);
    expect(3000, 2000, 1, 2000, 3000);
    expect(3001, 2001, 1, 2000, 3000);
    expect(0xffffffffu, 0xffffffffu, 1, 2000, 3000);
    expect(1, 1, 1, 20, 100);
    expect(2999, 1999, 1, 1999, 2999);
    if (failures) return 2;
    std::puts("lgKbdSetRepeatRate cases passed");
    return 0;
}
