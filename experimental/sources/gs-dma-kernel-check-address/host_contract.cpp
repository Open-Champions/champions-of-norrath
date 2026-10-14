#include "check_address.cpp"

#include <cstdio>

int main() {
    if (CheckAddress(0x70001234u) != 0x80001234u) return 1;
    if (CheckAddress(0x7fffffffu) != 0x8fffffffu) return 2;
    if (CheckAddress(0x00100000u) != 0x00100000u) return 3;
    if (CheckAddress(0x80001234u) != 0x80001234u) return 4;
    if (CheckAddress(0x60000000u) != 0x60000000u) return 5;
    if (CheckAddress(0u) != 0u) return 6;
    std::puts("check address contract passed");
    return 0;
}
