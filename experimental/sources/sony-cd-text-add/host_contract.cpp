#include <cstdint>
#include <cstdio>
#include "text_add.cpp"
extern "C" unsigned strlen(const char* text) {
    unsigned length = 0;
    while (text[length] != 0) length += 1;
    return length;
}
int main() {
    unsigned char empty[4] = {};
    textAddText__FPv(empty);
    for (unsigned index = 0; index < 0x2000u; ++index)
        if (table[index] != 0u) return 1;
    alignas(2) unsigned char one[16] = {'a', 'b', 0, 0, 0, 0, 0};
    textAddText__FPv(one);
    const unsigned bits = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(one));
    if (table[7768] != bits) return 2;
    table[5745] = 1u;
    alignas(2) unsigned char other[16] = {'c', 'd', 0, 0, 0, 0, 0};
    textAddText__FPv(other);
    const unsigned other_bits = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(other));
    if (table[5746] != other_bits || table[5745] != 1u) return 3;
    alignas(2) unsigned char pair[24] = {'a', 'b', 0, 0, 1, 0, 0, 0, 'z', 0, 0, 0, 0, 0};
    for (unsigned index = 0; index < 0x2000u; ++index) table[index] = 0u;
    textAddText__FPv(pair);
    if (table[7768] != static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pair))) return 4;
    const unsigned z_bits = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pair + 8));
    int found = 0;
    for (unsigned index = 0; index < 0x2000u; ++index)
        if (table[index] == z_bits) found += 1;
    if (found != 1) return 5;
    std::puts("text add contract passed");
    return 0;
}
