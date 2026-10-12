#include <cstdio>
#include <cstdint>
#include "sbuffin.cpp"
int main() {
    static_assert(sizeof(sbuffin) == 64, "64-byte spacing");
    if ((reinterpret_cast<std::uintptr_t>(sbuffin) & 63u) != 0) return 1;
    for (unsigned char byte : sbuffin) if (byte != 0) return 2;
    sbuffin[63] = 7;
    if (sbuffin[63] != 7) return 3;
    std::puts("sbuffin contract passed");
    return 0;
}
