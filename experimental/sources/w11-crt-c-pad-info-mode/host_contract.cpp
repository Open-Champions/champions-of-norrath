// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp pad_info_mode.cpp
#include <cstring>
#include <initializer_list>
extern "C" {
unsigned char norrath_libpad_port_info_view[224];
alignas(8) static unsigned char block[4][128];
static int calls, seen_port = -1, seen_slot = -1;
unsigned char* scePadGetDmaStr(int port, int slot) { ++calls; seen_port = port; seen_slot = slot; return block[port * 2 + slot]; }
int scePadInfoMode(int port, int slot, int selector, int index);
}
static void open_slot(int port, int slot) {
    reinterpret_cast<unsigned*>(norrath_libpad_port_info_view)[(port * 112 + slot * 28 + 16) / 4] = 1;
}
static void half(unsigned char* b, int at, unsigned short v) { std::memcpy(b + at, &v, 2); }
int main() {
    unsigned char* b = block[3];
    for (int i = 0; i < 128; ++i) b[i] = static_cast<unsigned char>(0x80 + i);
    b[114] = 1; b[113] = 0; b[100] = 3; b[101] = 0x5a; b[104] = 4; b[105] = 2;
    for (int i = 0; i < 6; ++i) half(b, 80 + 2 * i, static_cast<unsigned short>(0xa000 + i * 0x111));
    if (scePadInfoMode(1, 1, 3, 0) != 0 || calls != 0) return 1;  // closed
    open_slot(1, 1);
    if (scePadInfoMode(1, 0, 3, 0) != 0 || scePadInfoMode(0, 1, 3, 0) != 0 || calls != 0) return 2;
    if (scePadInfoMode(1, 1, 3, 0) != 2 || calls != 1 || seen_port != 1 || seen_slot != 1) return 3;
    if (scePadInfoMode(1, 1, 1, 0) != 5) return 4;                                  // 0x5a >> 4
    b[101] = 0xf7;
    if (scePadInfoMode(1, 1, 1, 0) != 15) return 5;                                 // high nibble, unsigned
    b[101] = 243;
    if (scePadInfoMode(1, 1, 1, 0) != 0) return 6;                                  // 243 is the empty marker
    if (scePadInfoMode(1, 1, 2, 0) != 0xa222) return 7;                             // halfword at 80 + 2 * 2
    b[105] = 5;
    if (scePadInfoMode(1, 1, 2, 0) != 0xa555) return 8;
    b[105] = 2;
    if (scePadInfoMode(1, 1, 4, -1) != 4) return 9;                                 // count
    if (scePadInfoMode(1, 1, 4, 0) != 0xa000 || scePadInfoMode(1, 1, 4, 3) != 0xa333) return 10;
    if (scePadInfoMode(1, 1, 4, 4) != 0 || scePadInfoMode(1, 1, 4, 99) != 0) return 11;
    unsigned short before; std::memcpy(&before, b + 76, 2);
    if (scePadInfoMode(1, 1, 4, -2) != before) return 20;                           // other negatives are in range
    half(b, 80, 0xffee);
    if (scePadInfoMode(1, 1, 4, 0) != 0xffee) return 12;                            // unsigned halfword
    b[104] = 200;
    if (scePadInfoMode(1, 1, 4, -1) != 200) return 13;                              // count is unsigned
    b[104] = 4;
    for (int sel : {0, 5, 6, -1}) if (scePadInfoMode(1, 1, sel, 0) != 0) return 14;
    b[100] = 1;                                                                     // selectors 2, 3, 4 refuse; 1 does not
    b[101] = 0x5a;
    if (scePadInfoMode(1, 1, 2, 0) != 0 || scePadInfoMode(1, 1, 3, 0) != 0 || scePadInfoMode(1, 1, 4, -1) != 0 ||
        scePadInfoMode(1, 1, 4, 0) != 0) return 15;
    if (scePadInfoMode(1, 1, 1, 0) != 5) return 16;
    b[100] = 3; b[113] = 2;
    for (int sel = 1; sel <= 4; ++sel) if (scePadInfoMode(1, 1, sel, 0) != 0) return 17;
    b[113] = 1; b[114] = 2;
    for (int sel = 1; sel <= 4; ++sel) if (scePadInfoMode(1, 1, sel, 0) != 0) return 18;
    b[114] = 1;
    if (scePadInfoMode(1, 1, 3, 0) != 2) return 19;                                 // byte 113 other than 2 is fine
    return 0;
}
