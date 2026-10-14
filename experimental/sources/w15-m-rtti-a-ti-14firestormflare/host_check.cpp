// Host contract for the storage of __ti14FireStormFlare; not an EE input. Checks size and that it starts zeroed.
#include "typeinfo_node.cpp"
#include <cstdio>

int main() {
    int failures = 0;
    if (sizeof __ti14FireStormFlare != 12) { std::fprintf(stderr, "fail: size\n"); ++failures; }
    for (unsigned i = 0; i < sizeof __ti14FireStormFlare; ++i)
        if (__ti14FireStormFlare[i] != 0) { std::fprintf(stderr, "fail: byte %u\n", i); ++failures; }
    if (!failures) std::puts("w15-m-rtti-a-ti-14firestormflare ok");
    return failures ? 1 : 0;
}
