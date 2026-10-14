#define NORRATH_VSLOTS_B_HOST 1
#include "counter_save.cpp"
#include <cstdio>
#include <cstring>
int main() {
    unsigned char object[80];
    unsigned char buffer[8];
    std::memset(object, 0x3c, sizeof object);
    std::memset(buffer, 0x5a, sizeof buffer);
    vslots_b::write_word(vslots_b::address(object) + 76u, 0xa1b2c3d4u);
    if (msg_save__7CounterPc(object, buffer) != 4) return 1;
    if (vslots_b::read_word(vslots_b::address(buffer)) != 0xa1b2c3d4u) return 1;
    if (buffer[4] != 0x5a || object[75] != 0x3c) return 1;
    if (vslots_b::read_word(vslots_b::address(object) + 76u) != 0xa1b2c3d4u) return 1;
    return 0;
}
