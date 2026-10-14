// Host contract for the storage of __ti10bad_typeid; not an EE input. Checks size and that it starts zeroed.
#include "typeinfo_node.cpp"
#include <cstdio>

int main() {
    int failures = 0;
    if (sizeof __ti10bad_typeid != 12) { std::fprintf(stderr, "fail: size\n"); ++failures; }
    for (unsigned i = 0; i < sizeof __ti10bad_typeid; ++i)
        if (__ti10bad_typeid[i] != 0) { std::fprintf(stderr, "fail: byte %u\n", i); ++failures; }
    if (!failures) std::puts("w15-m-rtti-a-ti-10bad-typeid ok");
    return failures ? 1 : 0;
}
