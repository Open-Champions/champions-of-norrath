// Host comparison only. Not an EE build input.
#include <cstdio>
#include <cstring>
#include "klibinfo.cpp"
int main() {
    if (sizeof __ps2_klibinfo__ != 16) { std::puts("size"); return 1; }
    if (std::memcmp(__ps2_klibinfo__, "PsIIlibkernl2800", 16) != 0) { std::puts("bytes"); return 2; }
    if (std::memcmp(__ps2_klibinfo__ + 12, "2800", 4) != 0) { std::puts("version bytes"); return 3; }
    std::puts("klibinfo cases passed");
    return 0;
}
