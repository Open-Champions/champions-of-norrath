// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp pad_info_act.cpp
extern "C" {
unsigned char norrath_libpad_port_info_view[224];
alignas(8) static unsigned char block[4][256];  // room for index -2 .. 12 below
static int calls, seen_port = -1, seen_slot = -1;
unsigned char* scePadGetDmaStr(int port, int slot) { ++calls; seen_port = port; seen_slot = slot; return block[port * 2 + slot] + 64; }
int scePadInfoAct(int port, int slot, int index, int selector);
}
static void open_slot(int port, int slot) {
    reinterpret_cast<unsigned*>(norrath_libpad_port_info_view)[(port * 112 + slot * 28 + 16) / 4] = 1;
}
int main() {
    unsigned char* b = block[3] + 64;
    for (int i = 0; i < 128; ++i) b[i] = static_cast<unsigned char>(0x80 + i);  // distinct bytes
    b[114] = 1; b[100] = 2; b[106] = 3;
    if (scePadInfoAct(1, 1, 0, 1) != 0 || calls != 0) return 1;  // closed
    open_slot(1, 1);
    if (scePadInfoAct(1, 0, 0, 1) != 0 || scePadInfoAct(0, 1, 0, 1) != 0 || calls != 0) return 2;
    if (scePadInfoAct(1, 1, 0, 1) != b[48] || calls != 1 || seen_port != 1 || seen_slot != 1) return 3;
    if (scePadInfoAct(1, 1, 0, 2) != b[49] || scePadInfoAct(1, 1, 0, 3) != b[50] || scePadInfoAct(1, 1, 0, 4) != b[51]) return 4;
    if (scePadInfoAct(1, 1, 2, 1) != b[56] || scePadInfoAct(1, 1, 2, 4) != b[59]) return 5;   // 4 bytes per index
    if (scePadInfoAct(1, 1, 1, 0) != 0 || scePadInfoAct(1, 1, 1, 5) != 0 || scePadInfoAct(1, 1, 1, -1) != 0) return 6;
    if (scePadInfoAct(1, 1, 3, 1) != 0 || scePadInfoAct(1, 1, 100, 1) != 0) return 7;       // index >= count
    if (scePadInfoAct(1, 1, -1, 1) != 3 || scePadInfoAct(1, 1, -1, 9) != 3) return 8;        // -1 returns the count
    if (scePadInfoAct(1, 1, -2, 2) != b[41]) return 9;                                       // other negatives are in range
    b[106] = 200;
    if (scePadInfoAct(1, 1, 1, 1) != b[52] || scePadInfoAct(1, 1, -1, 0) != 200) return 10;  // count is unsigned
    b[114] = 0;
    if (scePadInfoAct(1, 1, 0, 1) != 0 || scePadInfoAct(1, 1, -1, 1) != 0) return 11;
    b[114] = 1; b[100] = 1;
    if (scePadInfoAct(1, 1, 0, 1) != 0 || scePadInfoAct(1, 1, -1, 1) != 0) return 12;
    b[100] = 255;
    if (scePadInfoAct(1, 1, 0, 1) != b[48]) return 13;
    return 0;
}
