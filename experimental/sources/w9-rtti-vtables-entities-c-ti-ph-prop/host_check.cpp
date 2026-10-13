// Host check only. Not an EE build input.
#include "storage.cpp"
#include <cstdio>
int main() {
    int bad = 0;
    if (sizeof(norrath_ti_6PhProp) != 12) { std::printf("size\n"); ++bad; }
    for (unsigned i = 0; i < 12; ++i) if (norrath_ti_6PhProp[i] != 0) { std::printf("byte %u\n", i); ++bad; }
    if (reinterpret_cast<unsigned long>(norrath_ti_6PhProp) % 4 != 0) { std::printf("alignment\n"); ++bad; }
    std::printf(bad ? "failed\n" : "norrath_ti_6PhProp passed\n");
    return bad != 0;
}
