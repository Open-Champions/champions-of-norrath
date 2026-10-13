// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_CONTRACT -o /tmp/c host_contract.cpp disk_ready_old.cpp
#include <cstdarg>
#include <initializer_list>
#include <cstdio>
#include <cstring>
extern "C" {
int SCE_CD_debug, _sceCd_scmd_semid = 55, _scmd_dr_bind, cddr[10], _dr_mode;
unsigned char _sceCd_scmdrdata[4];
static int ev[64], evn, polled_ok = 1, sync_result, bind_plan[8], bind_n, bind_calls, rpc_result, signals, sig_id;
static char printed[8][64]; static int printed_n;
static unsigned bind_server[8]; static void* bind_client; static int wb_size; static void* wb_addr;
static unsigned rpc_fn, rpc_mode, send_size_seen, recv_size_seen; static void *rpc_client_seen, *rpc_send_seen, *rpc_recv_seen, *rpc_end, *rpc_param;
void cmd_sem_init() { ev[evn++] = 1; }
int PollSema(int id) { ev[evn++] = 2; return polled_ok ? id : -1; }
int SignalSema(int id) { ev[evn++] = 3; ++signals; sig_id = id; return 0; }
int sceCdSyncS(int mode) { ev[evn++] = 4; return mode == 1 ? sync_result : -99; }
int scePrintf(const char* format, ...) {
    ev[evn++] = 5; va_list a; va_start(a, format); std::vsnprintf(printed[printed_n < 8 ? printed_n : 7], 64, format, a); va_end(a); ++printed_n; return 0;
}
void sceSifInitRpc(int mode) { ev[evn++] = mode == 0 ? 6 : 99; }
int sceSifBindRpc(void* client, unsigned server, unsigned mode) {
    ev[evn++] = 7; bind_client = client; if (bind_calls < 8) bind_server[bind_calls] = server + mode;
    const int plan = bind_plan[bind_n < 7 ? bind_n : 7]; ++bind_n; ++bind_calls;
    cddr[9] = plan > 0 ? 1 : 0;
    return plan < 0 ? -1 : 0;
}
void sceSifWriteBackDCache(void* a, int n) { ev[evn++] = 8; wb_addr = a; wb_size = n; }
int sceSifCallRpc(void* client, unsigned fn, unsigned mode, void* send, int ss, void* recv, int rs, void* end, void* param) {
    ev[evn++] = 9; rpc_client_seen = client; rpc_fn = fn; rpc_mode = mode; rpc_send_seen = send; send_size_seen = ss; rpc_recv_seen = recv;
    recv_size_seen = rs; rpc_end = end; rpc_param = param; return rpc_result;
}
int sceCdDiskReady_old(int mode);
}
static void reset() {
    evn = 0; printed_n = 0; signals = 0; bind_calls = 0; bind_n = 0; polled_ok = 1; sync_result = 0; rpc_result = 0;
    SCE_CD_debug = 0; _scmd_dr_bind = -1; cddr[9] = 0; _dr_mode = 0x77;
    for (int& p : bind_plan) p = 1;
    const unsigned value = 0x12345678u; std::memcpy(_sceCd_scmdrdata, &value, 4);
}
static bool events(std::initializer_list<int> list) {
    if (static_cast<int>(list.size()) != evn) return false;
    int i = 0; for (int e : list) if (ev[i++] != e) return false; return true;
}
int main() {
    // semaphore not obtained: 6 for every mode, nothing else
    reset(); polled_ok = 0;
    if (sceCdDiskReady_old(8) != 6 || !events({1, 2})) return 1;
    // controller busy: semaphore given back, -1 for mode 8 and 6 for the rest
    reset(); sync_result = 2;
    if (sceCdDiskReady_old(8) != -1 || !events({1, 2, 4, 3}) || sig_id != 55) return 2;
    reset(); sync_result = 2;
    if (sceCdDiskReady_old(7) != 6 || !events({1, 2, 4, 3})) return 3;
    // already bound: straight to the call
    reset(); _scmd_dr_bind = 0;
    if (sceCdDiskReady_old(5) != 0x12345678 || !events({1, 2, 4, 6, 8, 9, 3})) return 4;
    if (_dr_mode != 5 || wb_addr != &_dr_mode || wb_size != 4) return 5;
    if (rpc_client_seen != cddr || rpc_fn != 0 || rpc_mode != 0 || rpc_send_seen != &_dr_mode || send_size_seen != 4 ||
        rpc_recv_seen != _sceCd_scmdrdata || recv_size_seen != 4 || rpc_end != nullptr || rpc_param != nullptr) return 6;
    // debug prints
    reset(); _scmd_dr_bind = 0; SCE_CD_debug = 1;
    sceCdDiskReady_old(1);
    if (printed_n != 2 || std::strcmp(printed[0], "OLD DiskReady Call\n") != 0 || std::strcmp(printed[1], "DiskReady ended\n") != 0) return 7;
    if (!events({5, 1, 2, 4, 6, 8, 9, 5, 3})) return 8;       // the result is read before the semaphore is given back
    reset(); _scmd_dr_bind = 0; SCE_CD_debug = -1;
    sceCdDiskReady_old(1);
    if (printed_n != 0) return 9;                                // only a positive debug word prints
    // first bind: success with the bound flag stores 0 in the bind word
    reset(); bind_plan[0] = 1;
    if (sceCdDiskReady_old(3) != 0x12345678 || _scmd_dr_bind != 0 || bind_calls != 1 || bind_server[0] != 0x8000059au || bind_client != cddr) return 10;
    if (!events({1, 2, 4, 6, 7, 8, 9, 3})) return 11;
    // bind errors and unbound replies are retried, with the error text under debug
    reset(); SCE_CD_debug = 1; bind_plan[0] = -1; bind_plan[1] = 0; bind_plan[2] = 1;
    sceCdDiskReady_old(3);
    if (bind_calls != 3 || _scmd_dr_bind != 0) return 12;
    if (printed_n != 3 || std::strcmp(printed[1], "Libcdvd bind err CdDiskReady\n") != 0) return 13;
    // failed call: semaphore given back, busy value
    reset(); _scmd_dr_bind = 0; rpc_result = -1;
    if (sceCdDiskReady_old(8) != -1 || !events({1, 2, 4, 6, 8, 9, 3})) return 14;
    reset(); _scmd_dr_bind = 0; rpc_result = -1; SCE_CD_debug = 1;
    if (sceCdDiskReady_old(2) != 6 || printed_n != 1) return 15;  // no "ended" text on failure
    // the received word is returned as a signed value
    reset(); _scmd_dr_bind = 0; { const unsigned v = 0xfffffff0u; std::memcpy(_sceCd_scmdrdata, &v, 4); }
    if (sceCdDiskReady_old(0) != -16) return 16;
    return 0;
}
