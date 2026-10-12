// Host comparison only. Not an EE build input.
#include <cstdio>
#include <cstdlib>

extern "C" long long norrath_factory_strtol(const char* text, char** end, int base) {
    return std::strtoll(text, end, base);
}
#define atoi w8_atoi_under_test
#include "atoi.cpp"
#undef atoi

static int expect(const char* text, int value) {
    const int got = w8_atoi_under_test(text);
    if (got != value) { std::printf("atoi(\"%s\") got %d expect %d\n", text, got, value); return 1; }
    return 0;
}

int main() {
    int failures = 0;
    failures += expect("0", 0);
    failures += expect("42", 42);
    failures += expect("  -17xyz", -17);
    failures += expect("+9", 9);
    failures += expect("", 0);
    failures += expect("abc", 0);
    failures += expect("2147483647", 2147483647);
    failures += expect("-2147483648", -2147483647 - 1);
    failures += expect("4294967297", 1);     // low 32 bits of a 64-bit parse
    failures += expect("4294967295", -1);
    failures += expect("0x10", 0);           // base 10 only
    if (failures) return 1;
    std::printf("atoi cases passed\n");
    return 0;
}
