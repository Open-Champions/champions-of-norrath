// Host contract for the storage of __ti9exception; not an EE input. Checks size and that it starts zeroed.
#include "typeinfo_node.cpp"
#include <cstdio>

int main() {
    int failures = 0;
    if (sizeof __ti9exception != 8) { std::fprintf(stderr, "fail: size\n"); ++failures; }
    for (unsigned i = 0; i < sizeof __ti9exception; ++i)
        if (__ti9exception[i] != 0) { std::fprintf(stderr, "fail: byte %u\n", i); ++failures; }
    if (!failures) std::puts("w16-m-data-a-ti-9exception ok");
    return failures ? 1 : 0;
}
