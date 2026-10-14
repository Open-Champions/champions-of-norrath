#define NORRATH_VSLOTS_B_HOST 1
#include "counter_load.cpp"
#include <cstdio>
#include <cstring>
int main() {
    unsigned char object[84];
    unsigned char buffer[8];
    std::memset(object, 0x2e, sizeof object);
    std::memset(buffer, 0x70, sizeof buffer);
    vslots_b::write_word(vslots_b::address(buffer), 0x01020304u);
    if (msg_load__7CounterPc(object, buffer) != 4) return 1;
    if (vslots_b::read_word(vslots_b::address(object) + 76u) != 0x01020304u) return 1;
    if (object[75] != 0x2e || object[80] != 0x2e) return 1;
    if (buffer[4] != 0x70) return 1;
    return 0;
}
