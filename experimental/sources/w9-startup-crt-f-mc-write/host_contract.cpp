// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp mc_write.cpp
#include <cstdint>
extern "C" {
int mcClientID[16], sifParamOrd[12], retval[4], mcRunCmdNo, semaidRegFunc = 6;
static int poll_result, rpc_result, signals, rpc_calls, flush_calls, flush_before_rpc;
static int copied_at_call[2];
static unsigned rpc_command, rpc_mode;
static void* rpc_client; static const void* rpc_send; static int rpc_send_size, rpc_recv_size;
static const void* rpc_recv; static void (*rpc_done)(void*); static void* rpc_done_arg;
int PollSema(int id) { return id == 6 ? poll_result : -1; }
int SignalSema(int id) { if (id != 6) return -1; ++signals; return 0; }
int FlushCache(int mode) { ++flush_calls; return mode; }
void mceSifRpcEndFunc(void*) {}
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size, void* receive,
                  int receive_size, void (*done)(void*), void* done_arg) {
    ++rpc_calls; flush_before_rpc = flush_calls;
    copied_at_call[0] = sifParamOrd[8]; copied_at_call[1] = sifParamOrd[9];
    rpc_client = client; rpc_command = command; rpc_mode = mode; rpc_send = send; rpc_send_size = send_size;
    rpc_recv = receive; rpc_recv_size = receive_size; rpc_done = done; rpc_done_arg = done_arg;
    return rpc_result;
}
int sceMcWrite(int fd, const void* buffer, int size);
}
static int word_of(const void* p) { return static_cast<int>(static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p))); }
static void reset() {
    signals = rpc_calls = flush_calls = 0; mcRunCmdNo = 0; poll_result = 0; rpc_result = 0;
    for (int& w : sifParamOrd) w = 0x77777777;
}
static bool rpc_shape_ok(const void*) {
    return rpc_client == mcClientID && rpc_command == 6 && rpc_mode == 1 && rpc_send == sifParamOrd &&
           rpc_send_size == 48 && rpc_recv == retval && rpc_recv_size == 4 && rpc_done == mceSifRpcEndFunc &&
           rpc_done_arg == nullptr && flush_before_rpc == 1;
}
// A 16-byte-aligned scratch area whose start is chosen at a given offset.
alignas(64) static unsigned char area[128];
int main() {
    unsigned char* base = area;
    mcClientID[9] = 0;
    reset();
    if (sceMcWrite(3, base, 16) != -100 || rpc_calls != 0 || sifParamOrd[0] != 0x77777777) return 1;
    mcClientID[9] = 1;
    reset(); poll_result = -1;
    if (sceMcWrite(3, base, 16) != -200 || rpc_calls != 0 || signals != 0 || sifParamOrd[0] != 0x77777777) return 2;
    // small write (<= 16 bytes): everything goes in the head, nothing in the body
    for (int i = 0; i < 128; ++i) area[i] = static_cast<unsigned char>(0x40 + i);
    reset();
    if (sceMcWrite(9, base + 3, 11) != 0 || rpc_calls != 1 || !rpc_shape_ok(base)) return 3;
    if (sifParamOrd[0] != 9 || sifParamOrd[5] != 11 || sifParamOrd[6] != 0 || sifParamOrd[3] != 0) return 4;
    {
        const unsigned char* dst = reinterpret_cast<const unsigned char*>(sifParamOrd) + 32;
        for (int i = 0; i < 11; ++i) if (dst[i] != base[3 + i]) return 5;
        if (dst[11] != 0x77) return 6;  // exactly size bytes
    }
    if (mcRunCmdNo != 6 || signals != 0) return 7;
    // size 17 starting 4 bytes past a 16-byte boundary: 12-byte head up to the boundary
    reset();
    if (sceMcWrite(2, base + 4, 17) != 0 || !rpc_shape_ok(base)) return 8;
    if (sifParamOrd[5] != 12 || sifParamOrd[6] != word_of(base + 16) || sifParamOrd[3] != 5) return 9;
    {
        const unsigned char* dst = reinterpret_cast<const unsigned char*>(sifParamOrd) + 32;
        for (int i = 0; i < 12; ++i) if (dst[i] != base[4 + i]) return 10;
    }
    // aligned start: head is empty, no copy, the body is the whole buffer
    reset();
    if (sceMcWrite(2, base + 32, 40) != 0 || !rpc_shape_ok(base)) return 11;
    if (sifParamOrd[5] != 0 || sifParamOrd[6] != word_of(base + 32) || sifParamOrd[3] != 40) return 12;
    if (sifParamOrd[8] != 0x77777777) return 13;
    // start one byte past the boundary: 15-byte head
    reset();
    sceMcWrite(2, base + 33, 100);
    if (sifParamOrd[5] != 15 || sifParamOrd[6] != word_of(base + 48) || sifParamOrd[3] != 85) return 14;
    // start 15 past the boundary: 1-byte head
    reset();
    sceMcWrite(2, base + 47, 20);
    if (sifParamOrd[5] != 1 || sifParamOrd[6] != word_of(base + 48) || sifParamOrd[3] != 19) return 15;
    // failure: semaphore is given back and the command number stays
    reset(); rpc_result = -3;
    if (sceMcWrite(2, base, 40) != -3 || mcRunCmdNo != 0 || signals != 1) return 16;
    return 0;
}
