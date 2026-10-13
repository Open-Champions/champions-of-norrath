// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp pad_get_button_mask.cpp
extern "C" {
unsigned char norrath_libpad_port_info_view[224];
static unsigned char block[128];
static int calls, seen_port = -1, seen_slot = -1;
unsigned char* scePadGetDmaStr(int port, int slot) { ++calls; seen_port = port; seen_slot = slot; return block; }
int scePadGetButtonMask(int port, int slot);
}
static void good() {
    for (unsigned char& c : block) c = 0;
    block[114] = 1; block[100] = 2; block[102] = 2;
    block[121] = 0x11; block[122] = 0x22; block[123] = 0x33; block[124] = 0x44;
    calls = 0;
}
int main() {
    reinterpret_cast<unsigned*>(norrath_libpad_port_info_view)[(112 + 28 + 16) / 4] = 1;   // port 1 slot 1 open
    good();
    if (scePadGetButtonMask(0, 1) != 0 || scePadGetButtonMask(1, 0) != 0 || calls != 0) return 1;   // closed
    if (scePadGetButtonMask(1, 1) != 0x44332211 || calls != 1 || seen_port != 1 || seen_slot != 1) return 2;
    // byte 121 is the least significant, byte 124 the most significant, signed result
    block[124] = 0x80; block[123] = 0; block[122] = 0; block[121] = 1;
    if (scePadGetButtonMask(1, 1) != static_cast<int>(0x80000001u)) return 3;
    block[121] = 0xff; block[122] = 0xff; block[123] = 0xff; block[124] = 0x00;
    if (scePadGetButtonMask(1, 1) != 0xffffff) return 4;
    good(); block[121] = 0xff; block[122] = 0xff; block[123] = 3; block[124] = 0;
    if (scePadGetButtonMask(1, 1) != 0x3ffff) return 5;
    // each gate returns 0 on its own
    good(); block[114] = 0; if (scePadGetButtonMask(1, 1) != 0) return 6;
    good(); block[114] = 2; if (scePadGetButtonMask(1, 1) != 0) return 7;
    good(); block[100] = 1; if (scePadGetButtonMask(1, 1) != 0) return 8;
    good(); block[102] = 1; if (scePadGetButtonMask(1, 1) != 0) return 9;
    good(); block[100] = 0xff; block[102] = 0xff; if (scePadGetButtonMask(1, 1) != 0x44332211) return 10;   // unsigned compares
    good(); block[100] = 2; block[102] = 2; if (scePadGetButtonMask(1, 1) != 0x44332211) return 11;      // 2 passes
    return 0;
}
