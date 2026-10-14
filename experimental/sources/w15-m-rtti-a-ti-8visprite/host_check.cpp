// Host contract for the storage of __ti8VISprite; not an EE input. Checks size and that it starts zeroed.
#include "typeinfo_node.cpp"
#include <cstddef>
#include <cstdio>

int main() {
    int failures = 0;
    if (sizeof __ti8VISprite != 12) { std::fprintf(stderr, "fail: size\n"); ++failures; }
    for (unsigned i = 0; i < sizeof __ti8VISprite; ++i)
        if (__ti8VISprite[i] != 0) { std::fprintf(stderr, "fail: byte %u\n", i); ++failures; }
    if (!failures) std::puts("w15-m-rtti-a-ti-8visprite ok");
    return failures ? 1 : 0;
}
