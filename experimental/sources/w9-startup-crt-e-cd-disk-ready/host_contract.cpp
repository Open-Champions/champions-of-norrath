#include <cstdio>
#include <cstring>
#define NORRATH_HOST_CONTRACT 1
#include "disk_ready.cpp"
extern "C" int SCE_CD_debug = 0; extern "C" int _sceCd_rcmd_semid = 4; extern "C" int _scmd_dr_bind = -1;
extern "C" alignas(16) int cddr[16] = {}; extern "C" int _dr_mode = 0;
extern "C" alignas(4) unsigned char _sceCd_rcmdrdata[0x100] = {};
static int sem_inits, poll_result = 4, signals, bound_calls, bind_ok_after = 0, server_word_after = 0, rpc_result = 0;
static int init_rpc_calls, prints, old_calls, old_mode = -1, spins_seen, wb_count, wb_value = -1, wb_size, bind_log_count;
static char printed[8][40]; static unsigned server_seen; static unsigned rpc_function, rpc_mode; static int rpc_send_size, rpc_recv_size;
static void* rpc_send; static void* rpc_recv; static void* rpc_end; static void* rpc_param; static int mode_at_rpc = -99;
extern "C" void cmd_sem_init() { ++sem_inits; }
extern "C" int PollSema(int id) { return id == 4 ? poll_result : -1; }
extern "C" int SignalSema(int id) { if (id == 4) ++signals; *reinterpret_cast<int*>(_sceCd_rcmdrdata) = 0x7777; return 0; }
extern "C" int scePrintf(const char* f, ...) { if (prints < 8) std::strncpy(printed[prints], f, 39); ++prints; return 0; }
extern "C" int sceSifInitRpc(int) { ++init_rpc_calls; return 0; }
extern "C" int sceSifBindRpc(void* client, unsigned server, unsigned mode) {
    ++bound_calls; server_seen = server;
    if (client != cddr || mode != 0) return -7;
    if (bound_calls <= bind_ok_after) return -1;
    if (bound_calls > bind_ok_after + server_word_after) cddr[9] = 1;
    return 0;
}
extern "C" void sceSifWriteBackDCache(void* a, int n) { ++wb_count; wb_value = _dr_mode; wb_size = n; if (a != &_dr_mode) wb_size = -1; }
extern "C" int sceSifCallRpc(void* client, unsigned f, unsigned m, void* send, int ss, void* recv, int rs, void* end, void* param) {
    rpc_function = f; rpc_mode = m; rpc_send = send; rpc_send_size = ss; rpc_recv = recv; rpc_recv_size = rs; rpc_end = end; rpc_param = param;
    mode_at_rpc = _dr_mode; *reinterpret_cast<int*>(_sceCd_rcmdrdata) = 42;
    return client == cddr ? rpc_result : -9;
}
extern "C" int sceCdDiskReady_old(int mode) { ++old_calls; old_mode = mode; return 31; }
int main() {
    // semaphore not available: busy value depends on mode 8
    poll_result = -3;
    if (sceCdDiskReady(0) != 6 || sem_inits != 1 || init_rpc_calls != 0) return 1;
    if (sceCdDiskReady(8) != -1) return 2;
    poll_result = 4;
    // already bound: no bind call at all
    _scmd_dr_bind = 0;
    if (sceCdDiskReady(2) != 42 || bound_calls != 0 || init_rpc_calls != 1 || signals != 1) return 3;
    if (_dr_mode != 2 || wb_count != 1 || wb_value != 2 || wb_size != 4 || mode_at_rpc != 2) return 4;
    if (rpc_function != 0 || rpc_mode != 0 || rpc_send != &_dr_mode || rpc_send_size != 4 || rpc_recv != _sceCd_rcmdrdata ||
        rpc_recv_size != 4 || rpc_end != nullptr || rpc_param != nullptr) return 5;
    // result is the word read before the signal released the buffer
    // first bind succeeds with the server pointer set
    _scmd_dr_bind = -1; bound_calls = 0; bind_ok_after = 0; server_word_after = 0; cddr[9] = 0;
    if (sceCdDiskReady(0) != 42 || _scmd_dr_bind != 0 || bound_calls != 1 || server_seen != 0x8000059cu) return 6;
    // two bind errors, then a server pointer after two empty answers
    _scmd_dr_bind = -1; bound_calls = 0; bind_ok_after = 2; server_word_after = 2; cddr[9] = 0; SCE_CD_debug = 1; prints = 0;
    if (sceCdDiskReady(0) != 42 || bound_calls != 5 || _scmd_dr_bind != 0) return 7;
    if (prints != 5) return 12;
    if (std::strcmp(printed[0], "NEW DiskReady Call\n") != 0 || std::strcmp(printed[1], "Libcdvd bind err CdDiskReady\n") != 0 ||
        std::strcmp(printed[prints - 1], "DiskReady ended\n") != 0) return 8;
    SCE_CD_debug = 0;
    // a server pointer that never appears falls back after the 18th answer
    _scmd_dr_bind = -1; bound_calls = 0; bind_ok_after = 0; server_word_after = 1000; cddr[9] = 0; signals = 0;
    if (sceCdDiskReady(5) != 31 || old_mode != 5 || bound_calls != 18 || signals != 1) return 9;
    // rpc failure: busy value and one signal
    _scmd_dr_bind = 0; rpc_result = -1; signals = 0;
    if (sceCdDiskReady(8) != -1 || signals != 1) return 10;
    if (sceCdDiskReady(1) != 6) return 11;
    std::puts("sceCdDiskReady contract passed");
    return 0;
}
