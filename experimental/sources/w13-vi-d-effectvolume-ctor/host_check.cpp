// Host check only; not a build input. Fills an object with a pattern, runs the routine and compares every byte.
#include "ctor.cpp"
#include <cstdio>
#include <initializer_list>
#include <cstring>

static unsigned char object[0x100];

int main() {
    unsigned char expect[sizeof object];
    std::memset(object, 0x5a, sizeof object);
    std::memset(expect, 0x5a, sizeof expect);
    auto put = [&](int off, unsigned v) { std::memcpy(expect + off, &v, 4); };
    for (int off : {0, 4, 8, 12, 16, 20, 24, 28, 36, 52, 56, 60, 88}) put(off, 0);
    put(32, 1); put(72, 1);
    put(40, 0x3f800000u); put(44, 0x3f800000u); put(48, 0x41a00000u);
    for (int off : {64, 68, 76, 80}) put(off, 0xffffffffu);
    __14VIEffectVolume(object);
    bool ok = std::memcmp(object, expect, sizeof object) == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
