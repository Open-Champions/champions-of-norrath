// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp pad_get_req_state.cpp
extern "C" {
unsigned char norrath_libpad_port_info_view[224];
static unsigned char block[4][128];
static int calls, seen_port = -1, seen_slot = -1;
unsigned char* scePadGetDmaStr(int port, int slot) { ++calls; seen_port = port; seen_slot = slot; return block[port * 2 + slot]; }
int scePadGetReqState(int port, int slot);
}
static void open_slot(int port, int slot) {
    reinterpret_cast<unsigned*>(norrath_libpad_port_info_view)[(port * 112 + slot * 28 + 16) / 4] = 1;
}
int main() {
    for (auto& b : block) for (unsigned char& c : b) c = 0x44;
    block[3][113] = 0xc8;
    if (scePadGetReqState(1, 1) != 0 || calls != 0) return 1;  // closed: 0, no lookup
    open_slot(1, 1);
    if (scePadGetReqState(1, 0) != 0 || scePadGetReqState(0, 1) != 0 || calls != 0) return 2;
    if (scePadGetReqState(1, 1) != 0xc8 || calls != 1 || seen_port != 1 || seen_slot != 1) return 3;  // unsigned byte 113
    block[3][112] = 9; block[3][114] = 9; block[3][113] = 0;
    if (scePadGetReqState(1, 1) != 0) return 4;
    return 0;
}
