// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp pad_set_req_state.cpp
extern "C" {
unsigned char norrath_libpad_port_info_view[224];
static unsigned char block[4][128];
static int calls, seen_port = -1, seen_slot = -1;
unsigned char* scePadGetDmaStr(int port, int slot) { ++calls; seen_port = port; seen_slot = slot; return block[port * 2 + slot]; }
int scePadSetReqState(int port, int slot, int state);
}
static void open_slot(int port, int slot) {
    reinterpret_cast<unsigned*>(norrath_libpad_port_info_view)[(port * 112 + slot * 28 + 16) / 4] = 1;
}
int main() {
    for (auto& b : block) for (unsigned char& c : b) c = 0x33;
    // closed: no lookup, no store
    if (scePadSetReqState(1, 1, 5) != 0 || calls != 0 || block[3][113] != 0x33) return 1;
    open_slot(1, 1);
    if (scePadSetReqState(1, 0, 5) != 0 || scePadSetReqState(0, 1, 5) != 0 || calls != 0) return 2;  // neighbours still closed
    if (scePadSetReqState(1, 1, 0x1a5) != 1 || calls != 1 || seen_port != 1 || seen_slot != 1) return 3;
    if (block[3][113] != 0xa5) return 4;               // low byte only, at offset 113
    for (int i = 0; i < 128; ++i) if (i != 113 && block[3][i] != 0x33) return 5;
    for (int b = 0; b < 3; ++b) for (unsigned char c : block[b]) if (c != 0x33) return 6;
    return 0;
}
