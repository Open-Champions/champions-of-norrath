// Host contract for the storage of __ti10VIResource; not an EE input. Checks size and that it starts zeroed.
#include "typeinfo_node.cpp"
#include <cstdio>

int main() {
    int failures = 0;
    if (sizeof __ti10VIResource != 8) { std::fprintf(stderr, "fail: size\n"); ++failures; }
    for (unsigned i = 0; i < sizeof __ti10VIResource; ++i)
        if (__ti10VIResource[i] != 0) { std::fprintf(stderr, "fail: byte %u\n", i); ++failures; }
    if (!failures) std::puts("w16-m-data-a-ti-10viresource ok");
    return failures ? 1 : 0;
}
