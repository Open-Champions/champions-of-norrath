#include <cstdio>
#include <cstdint>
#include <cstring>
#define fabsf my_fabsf
#include "fabsf.cpp"
static uint32_t bits(float f) { uint32_t u; std::memcpy(&u, &f, 4); return u; }
static float flt(uint32_t u) { float f; std::memcpy(&f, &u, 4); return f; }
int main() {
    const uint32_t samples[] = {0u, 0x80000000u, 1u, 0x80000001u, 0x007fffffu, 0x807fffffu,
                                0x3f800000u, 0xbf800000u, 0x7f7fffffu, 0xff7fffffu, 0x7f800000u,
                                0xff800000u, 0x7fc00000u, 0xffc00001u, 0x7fa5a5a5u, 0xffa5a5a5u};
    for (uint32_t s : samples) {
        if (bits(fabsf(flt(s))) != (s & 0x7fffffffu)) { std::printf("fabsf wrong for %08x\n", s); return 1; }
    }
    uint32_t x = 12345;
    for (int i = 0; i < 100000; ++i) {
        x = x * 1664525u + 1013904223u;
        if (bits(fabsf(flt(x))) != (x & 0x7fffffffu)) { std::printf("fabsf wrong for %08x\n", x); return 1; }
    }
    std::puts("fabsf clears exactly the sign bit");
    return 0;
}
