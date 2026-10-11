#include <cstdint>
#include <cstdio>

extern "C" int norrath_factory_mbtowc_reentrant(unsigned char*, unsigned char*, const char*, unsigned, unsigned char*);

static int fail(const char* message) {
    std::printf("%s\n", message);
    return 1;
}

int main() {
    unsigned char wide[4] = {0xff, 0xff, 0xff, 0xff};
    unsigned char reent[4] = {1, 2, 3, 4};
    unsigned char state[4] = {9, 9, 9, 9};
    if (norrath_factory_mbtowc_reentrant(reent, wide, nullptr, 0, state) != 0) return fail("null input with zero maximum");
    if (wide[0] != 0xff) return fail("null input stored");
    if (norrath_factory_mbtowc_reentrant(reent, wide, nullptr, 4, state) != 0) return fail("null input");
    const char* text = "A";
    if (norrath_factory_mbtowc_reentrant(reent, wide, text, 0, state) != -1) return fail("zero maximum");
    if (wide[0] != 0xff) return fail("zero maximum stored");
    if (norrath_factory_mbtowc_reentrant(reent, wide, text, 1, state) != 1) return fail("letter result");
    if (wide[0] != 'A' || wide[1] != 0 || wide[2] != 0 || wide[3] != 0) return fail("letter word");
    const char nul = 0;
    if (norrath_factory_mbtowc_reentrant(reent, wide, &nul, 8, state) != 0) return fail("nul result");
    if (wide[0] != 0) return fail("nul word");
    const char high = static_cast<char>(0xff);
    if (norrath_factory_mbtowc_reentrant(reent, wide, &high, 3, state) != 1) return fail("high result");
    if (wide[0] != 0xff || wide[1] != 0 || wide[2] != 0 || wide[3] != 0) return fail("high word");
    if (norrath_factory_mbtowc_reentrant(reent, nullptr, text, 2, state) != 1) return fail("null wide");
    if (state[0] != 9 || reent[0] != 1) return fail("unused inputs changed");
    std::printf("mbtowc cases passed\n");
    return 0;
}
