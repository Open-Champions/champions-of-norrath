// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp sif_m_init_rpc.cpp
#include <cstdint>
#include <cstring>
extern "C" {
int _sceSifMInitCheck;
unsigned char norrath_sif_m_rpc_packet_pool_view[32];
unsigned char norrath_sif_m_rpc_packet_buffer_view[256];
static int events[16], event_count;
static int dintr_flag_seen, add_calls, send_calls, sreg_calls, sreg_zero_polls, reboot_calls;
static unsigned add_command, send_command, reboot_id;
static void* add_parameter; static void (*add_handler)(void*, void*);
static void (*reboot_handler)(void*, void*); static void* reboot_param;
static unsigned char pool_at_add[20], packet_at_send[24];
static void* send_packet; static int send_bytes; static void *send_src, *send_dst; static int send_data;
static unsigned sreg_index_bad;
void norrath_sif_m_request_end(void*, void*) {}
void _iopnotify(void*, void*) {}
int DIntr() { events[event_count++] = 1; dintr_flag_seen = _sceSifMInitCheck; return 1; }
void EIntr() { events[event_count++] = 2; }
int sceSifAddCmdHandler(unsigned command, void (*handler)(void*, void*), void* parameter) {
    events[event_count++] = 3; ++add_calls; add_command = command; add_handler = handler; add_parameter = parameter;
    std::memcpy(pool_at_add, norrath_sif_m_rpc_packet_pool_view, 20);
    return 0;
}
int sceSifSendCmd(unsigned command, void* packet, int packet_bytes, void* source, void* destination, int data_bytes) {
    events[event_count++] = 4; ++send_calls; send_command = command; send_packet = packet; send_bytes = packet_bytes;
    send_src = source; send_dst = destination; send_data = data_bytes;
    std::memcpy(packet_at_send, norrath_sif_m_rpc_packet_buffer_view + 64, 24);
    return 1;
}
unsigned sceSifGetSreg(unsigned index) {
    events[event_count++] = 5; if (index != 1) ++sreg_index_bad;
    return ++sreg_calls > sreg_zero_polls ? 1u : 0u;
}
int sceSifAddRebootNotifyHandler(unsigned identifier, void (*handler)(void*, void*), void* parameter) {
    events[event_count++] = 6; ++reboot_calls; reboot_id = identifier; reboot_handler = handler; reboot_param = parameter;
    return 1234;
}
int sceSifMInitRpc();
}
static unsigned word_at(const unsigned char* p) { return p[0] | (p[1] << 8) | (p[2] << 16) | (static_cast<unsigned>(p[3]) << 24); }
int main() {
    // already initialised: only the flag is read, its value comes back, nothing else runs
    _sceSifMInitCheck = 3;
    if (sceSifMInitRpc() != 3 || event_count != 0 || _sceSifMInitCheck != 3) return 1;
    // first run, the sreg stays zero for two polls
    _sceSifMInitCheck = 0; sreg_zero_polls = 2;
    std::memset(norrath_sif_m_rpc_packet_pool_view, 0xcc, sizeof norrath_sif_m_rpc_packet_pool_view);
    std::memset(norrath_sif_m_rpc_packet_buffer_view, 0xdd, sizeof norrath_sif_m_rpc_packet_buffer_view);
    if (sceSifMInitRpc() != 1234) return 2;
    if (_sceSifMInitCheck != 1 || dintr_flag_seen != 1) return 3;    // flag is already set when interrupts go off
    const int expected[] = {1, 3, 2, 4, 5, 5, 5, 6};
    if (event_count != 8) return 4;
    for (int i = 0; i < 8; ++i) if (events[i] != expected[i]) return 5;
    if (add_command != 0x80000018u || add_handler != norrath_sif_m_request_end || add_parameter != norrath_sif_m_rpc_packet_pool_view) return 6;
    // pool header at the time of registration
    if (word_at(pool_at_add) != 1 || word_at(pool_at_add + 8) != 32 || word_at(pool_at_add + 12) != 0 || word_at(pool_at_add + 16) != 0) return 7;
    const unsigned buffer_low = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_sif_m_rpc_packet_buffer_view));
    if (word_at(pool_at_add + 4) != (buffer_low | 0x20000000u)) return 8;
    if (norrath_sif_m_rpc_packet_pool_view[20] != 0xcc) return 9;      // exactly 20 bytes are written
    // the command packet sits 64 bytes into the buffer
    if (send_command != 0x80000001u || send_packet != norrath_sif_m_rpc_packet_buffer_view + 64 || send_bytes != 24 ||
        send_src || send_dst || send_data != 0) return 10;
    if (word_at(packet_at_send + 16) != 1 || word_at(packet_at_send + 20) != 1 || word_at(packet_at_send) != 0xdddddddd ||
        word_at(packet_at_send + 12) != 0xdddddddd) return 11;
    if (sreg_calls != 3 || sreg_index_bad != 0) return 12;
    if (reboot_id != 0x80000014u || reboot_handler != _iopnotify || reboot_param != nullptr) return 13;
    // second call is a no-op again
    event_count = 0;
    if (sceSifMInitRpc() != 1 || event_count != 0) return 14;
    return 0;
}
