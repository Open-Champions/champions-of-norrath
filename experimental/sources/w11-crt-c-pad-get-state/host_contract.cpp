// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp pad_get_state.cpp
extern "C" {
unsigned char norrath_libpad_port_info_view[224];
static unsigned char block[4][128];
static int calls, seen_port = -1, seen_slot = -1;
unsigned char* scePadGetDmaStr(int port, int slot) { ++calls; seen_port = port; seen_slot = slot; return block[port * 2 + slot]; }
int scePadGetState(int port, int slot);
}
static void open_slot(int port, int slot) {
    reinterpret_cast<unsigned*>(norrath_libpad_port_info_view)[(port * 112 + slot * 28 + 16) / 4] = 1;
}
int main() {
    for (auto& b : block) for (unsigned char& c : b) c = 7;
    if (scePadGetState(1, 1) != 99 || calls != 0) return 1;  // closed: marker, no lookup
    open_slot(1, 1);
    if (scePadGetState(1, 0) != 99 || scePadGetState(0, 1) != 99 || calls != 0) return 2;  // neighbours closed
    block[3][112] = 3; block[3][113] = 2;
    if (scePadGetState(1, 1) != 3 || calls != 1 || seen_port != 1 || seen_slot != 1) return 3;
    block[3][112] = 6; block[3][113] = 2;
    if (scePadGetState(1, 1) != 5) return 4;                  // the 6 / 2 pair
    block[3][113] = 1;
    if (scePadGetState(1, 1) != 6) return 5;                  // 6 with another second byte stays 6
    block[3][112] = 5; block[3][113] = 2;
    if (scePadGetState(1, 1) != 5) return 6;
    block[3][112] = 0xfe; block[3][113] = 2;
    if (scePadGetState(1, 1) != 0xfe) return 7;               // unsigned byte
    block[3][112] = 0; block[3][113] = 2;
    if (scePadGetState(1, 1) != 0) return 8;
    return 0;
}
