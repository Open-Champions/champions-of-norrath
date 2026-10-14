#include <cstdio>
#include <cstdint>
#include "sbuff.cpp"
int main() {
    static_assert(sizeof(sbuff) == 64, "64-byte receive span");
    if ((reinterpret_cast<std::uintptr_t>(sbuff) & 63u) != 0) return 1;
    for (unsigned char byte : sbuff) if (byte != 0) return 2;
    sbuff[63] = 7;
    if (sbuff[63] != 7) return 3;
    std::puts("sbuff contract passed");
    return 0;
}
