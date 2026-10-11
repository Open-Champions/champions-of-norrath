#define NORRATH_VSLOTS_B_HOST 1
#include "lever_save.cpp"
#include <cstdio>
#include <cstring>
int main() {
    unsigned char object[84];
    unsigned char buffer[12];
    std::memset(object, 0x44, sizeof object);
    std::memset(buffer, 0x99, sizeof buffer);
    vslots_b::write_word(vslots_b::address(object) + 76u, 0x7fc00001u);
    vslots_b::write_word(vslots_b::address(object) + 12u, 0x11223344u);
    object[80] = 0xab;
    object[81] = 0xcd;
    if (msg_save__5LeverPc(object, buffer) != 10) return 1;
    if (vslots_b::read_word(vslots_b::address(buffer)) != 0x7fc00001u) return 1;
    if (vslots_b::read_word(vslots_b::address(buffer) + 4u) != 0x11223344u) return 1;
    if (buffer[8] != 0xab || buffer[9] != 0xcd || buffer[10] != 0x99) return 1;
    if (object[11] != 0x44 || object[82] != 0x44) return 1;
    return 0;
}
