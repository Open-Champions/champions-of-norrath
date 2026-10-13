// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "set_weapon.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
static unsigned rolls[2];
static int calls = 0;
extern "C" unsigned eRand__10GameObject(void*) { return rolls[calls++ & 1]; }
static unsigned expected(unsigned variant, unsigned a, unsigned b, bool* second, bool* flag) {
    *second = false; *flag = false;
    if (variant == 4) { *flag = true; return 2; }
    if (variant == 0) return (a & 8) ? 1 : (a & 0x16) ? 4 : 3;
    if (variant == 1) return (a & 0x16) ? 5 : (a & 0x32) ? 7 : 14;
    if (variant == 2 || variant == 3) { *second = true; return b % 3 + 14; }
    return 0x11223344u;
}
int main() {
    int bad = 0;
    const unsigned samples[] = {0, 1, 2, 4, 8, 0x10, 0x20, 0x16, 0x32, 0x40, 7, 0xffffffffu, 0x80000000u, 5, 6};
    for (unsigned variant : {0u, 1u, 2u, 3u, 4u, 5u, 9u, 0xffffffffu, 0xfffffffeu}) {
        for (unsigned a : samples) for (unsigned b : samples) {
            unsigned char object[1200];
            std::memset(object, 0x5a, sizeof object);
            rtti_c::write_word(rtti_c::address(object) + 1116u, variant);
            rtti_c::write_word(rtti_c::address(object) + 972u, 0x11223344u);
            rtti_c::write_word(rtti_c::address(object) + 1124u, 0x55667788u);
            rolls[0] = a; rolls[1] = b; calls = 0;
            norrath_orc_set_weapon(object);
            bool second, flag;
            const unsigned want = expected(variant, a, b, &second, &flag);
            if (rtti_c::read_word(rtti_c::address(object) + 972u) != want) { std::printf("v%u %x %x store\n", variant, a, b); ++bad; }
            if (rtti_c::read_word(rtti_c::address(object) + 1124u) != (flag ? 1u : 0x55667788u)) { std::printf("flag\n"); ++bad; }
            if (calls != (second ? 2 : 1)) { std::printf("v%u calls %d\n", variant, calls); ++bad; }
        }
    }
    std::printf(bad ? "failed\n" : "norrath_orc_set_weapon passed\n");
    return bad != 0;
}
