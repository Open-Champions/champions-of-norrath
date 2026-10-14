// Host contract for the storage of __ti19SpellSummonSkeleton; not an EE input. Checks size and that it starts zeroed.
#include "typeinfo_node.cpp"
#include <cstdio>

int main() {
    int failures = 0;
    if (sizeof __ti19SpellSummonSkeleton != 12) { std::fprintf(stderr, "fail: size\n"); ++failures; }
    for (unsigned i = 0; i < sizeof __ti19SpellSummonSkeleton; ++i)
        if (__ti19SpellSummonSkeleton[i] != 0) { std::fprintf(stderr, "fail: byte %u\n", i); ++failures; }
    if (!failures) std::puts("w15-m-rtti-a-ti-19spellsummonskeleton ok");
    return failures ? 1 : 0;
}
