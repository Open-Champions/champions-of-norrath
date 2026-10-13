// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_CONTRACT -o /tmp/c host_contract.cpp pad_set_act_direct.cpp
extern "C" {
unsigned char norrath_libpad_port_info_view[256];
static unsigned char block[4][128];
static unsigned request_object[2][8];
static int send_result, send_calls, send_port = -1, send_slot = -1, stat_calls;
static unsigned request_at_send[8];
unsigned* norrath_host_request_object(unsigned word) { return request_object[word - 1]; }
unsigned char* scePadGetDmaStr(int port, int slot) { ++stat_calls; return block[port * 2 + slot]; }
int _send_to_iop(int port, int slot) {
    ++send_calls; send_port = port; send_slot = slot;
    for (int i = 0; i < 8; ++i) request_at_send[i] = request_object[0][i];
    return send_result;
}
int scePadSetActDirect(int port, int slot, const unsigned char* data);
}
static unsigned* info_of(int port, int slot) {
    return reinterpret_cast<unsigned*>(norrath_libpad_port_info_view + slot * 28 + port * 112);
}
int main() {
    const unsigned char data[8] = {11, 22, 33, 44, 55, 66, 77, 88};
    info_of(0, 1)[1] = 1;   // port 0 slot 1 -> request_object[0]
    // block 1 (port 0, slot 1) is not in the right state: nothing happens
    block[1][114] = 0;
    send_result = 1;
    if (scePadSetActDirect(0, 1, data) != 0 || send_calls != 0 || request_object[0][1] != 0) return 1;
    block[1][114] = 2;
    if (scePadSetActDirect(0, 1, data) != 0 || send_calls != 0) return 2;
    // state 1: six data bytes at +12, mode word 1 at +4, length word 6 at +8
    block[1][114] = 1;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(request_object[0]);
    for (int i = 0; i < 32; ++i) bytes[i] = 0xa5;
    if (scePadSetActDirect(0, 1, data) != 1 || send_calls != 1 || send_port != 0 || send_slot != 1) return 3;
    if (request_object[0][1] != 1 || request_object[0][2] != 6) return 4;
    for (int i = 0; i < 6; ++i) if (bytes[12 + i] != data[i]) return 5;
    for (int i = 18; i < 32; ++i) if (bytes[i] != 0xa5) return 6;  // only the six bytes are written
    if (request_object[0][0] != 0xa5a5a5a5u) return 6;
    if (request_at_send[1] != 1 || request_at_send[2] != 6) return 7;  // stored before the send
    // the result is exactly "send returned 1"
    send_result = 2;
    if (scePadSetActDirect(0, 1, data) != 0) return 8;
    send_result = 0;
    if (scePadSetActDirect(0, 1, data) != 0) return 9;
    send_result = -1;
    if (scePadSetActDirect(0, 1, data) != 0) return 10;
    return 0;
}
