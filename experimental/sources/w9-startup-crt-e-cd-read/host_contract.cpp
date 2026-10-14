#include <cstdio>
#include <cstring>
#include "cd_read.cpp"
extern "C" unsigned _sceCd_ee_read_mode = 0; extern "C" int SCE_CD_debug = 0;
extern "C" int _sceCd_c_cb_sem = 0; extern "C" int sceCdCbfunc_num = 0; extern "C" int _sceCd_ncmd_semid = 5;
extern "C" alignas(4) unsigned char _sceCd_ncmdsdata[0x1000] = {};
extern "C" alignas(4) unsigned char _sceCd_rd_intr_data[0x90] = {};
extern "C" alignas(4) unsigned char _sceCd_Read_cur_pos[0x10] = {};
extern "C" alignas(4) unsigned char _sceCd_cd_ncmd[40] = {};
static int ready = 0, prechk_arg = -1, prechk_result = 1, ready_calls = 0, rpc_result = 0, signals = 0, prints = 0;
static int wb_count = 0; static void* wb_addr[8]; static int wb_size[8];
static unsigned rpc_function, rpc_mode; static int rpc_send_size, rpc_recv_size; static void* rpc_send; static void* rpc_end;
static void* rpc_param; static char last_print[32]; static int sema_seen = -1;
extern "C" int sceCdNcmdDiskReady() { ++ready_calls; return ready; }
extern "C" int _sceCd_ncmd_prechk(int command) { prechk_arg = command; return prechk_result; }
extern "C" void _sceCd_cd_read_intr() {}
extern "C" int scePrintf(const char* f, ...) { ++prints; std::strncpy(last_print, f, 31); return 0; }
extern "C" int SignalSema(int s) { ++signals; sema_seen = s; return 0; }
extern "C" void sceSifWriteBackDCache(void* a, int n) { wb_addr[wb_count] = a; wb_size[wb_count] = n; ++wb_count; }
extern "C" int sceSifCallRpc(void* client, unsigned f, unsigned m, void* send, int ss, void* recv, int rs, void* end, void* param) {
    rpc_function = f; rpc_mode = m; rpc_send = send; rpc_send_size = ss; rpc_recv_size = rs; rpc_end = end; rpc_param = param;
    if (client != _sceCd_cd_ncmd || recv != nullptr) return -9;
    return rpc_result;
}
static unsigned word(const unsigned char* p) { return p[0] | (p[1] << 8) | (p[2] << 16) | (static_cast<unsigned>(p[3]) << 24); }
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
int main() {
    unsigned char buffer[16];
    unsigned char mode[3] = {5, 6, 0};
    ready = 6;
    if (sceCdRead(10, 3, buffer, mode) != 0 || ready_calls != 1 || prechk_arg != -1) return 1;
    _sceCd_ee_read_mode = 1;
    prechk_result = 0;
    if (sceCdRead(10, 3, buffer, mode) != 0 || ready_calls != 1 || prechk_arg != 4 || wb_count != 0) return 2;
    prechk_result = 1; _sceCd_ee_read_mode = 0; ready = 0;
    _sceCd_Read_cur_pos[0] = 0x55;
    if (sceCdRead(0x1234, 3, buffer, mode) != 1 || ready_calls != 2) return 3;
    if (word(_sceCd_ncmdsdata) != 0x1234 || word(_sceCd_ncmdsdata + 4) != 3 || word(_sceCd_ncmdsdata + 8) != bits(buffer) ||
        _sceCd_ncmdsdata[12] != 5 || _sceCd_ncmdsdata[13] != 6 || _sceCd_ncmdsdata[14] != 0 ||
        word(_sceCd_ncmdsdata + 16) != bits(_sceCd_rd_intr_data) || word(_sceCd_ncmdsdata + 20) != bits(_sceCd_Read_cur_pos)) return 4;
    if (wb_count != 4 || wb_addr[0] != buffer || wb_size[0] != 3 * 2048 || wb_addr[1] != _sceCd_rd_intr_data || wb_size[1] != 144 ||
        wb_addr[2] != _sceCd_ncmdsdata || wb_size[2] != 24 || wb_addr[3] != _sceCd_Read_cur_pos || wb_size[3] != 4) return 5;
    if (_sceCd_Read_cur_pos[0] != 0 || sceCdCbfunc_num != 1 || _sceCd_c_cb_sem != 1 || rpc_function != 1 || rpc_mode != 1 ||
        rpc_send != _sceCd_ncmdsdata || rpc_send_size != 24 || rpc_recv_size != 0 ||
        rpc_end != reinterpret_cast<void*>(_sceCd_cd_read_intr) || rpc_param != _sceCd_rd_intr_data || prints != 0) return 6;
    wb_count = 0; mode[2] = 1;
    sceCdRead(1, 2, buffer, mode);
    if (wb_size[0] != 2 * 2328) return 7;
    wb_count = 0; mode[2] = 2;
    sceCdRead(1, 2, buffer, mode);
    if (wb_size[0] != 2 * 2340) return 8;
    wb_count = 0; mode[2] = 9;
    sceCdRead(1, 2, buffer, mode);
    if (wb_size[0] != 2 * 2048) return 9;
    wb_count = 0; _sceCd_ee_read_mode = 2;
    sceCdRead(1, 2, buffer, mode);
    if (wb_count != 3 || wb_addr[0] != _sceCd_rd_intr_data) return 10;
    _sceCd_ee_read_mode = 0; SCE_CD_debug = 1; wb_count = 0;
    if (sceCdRead(1, 2, buffer, mode) != 1 || prints != 2 || std::strcmp(last_print, "cdread end\n") != 0) return 11;
    rpc_result = -1; signals = 0; wb_count = 0;
    if (sceCdRead(1, 2, buffer, mode) != 0 || sceCdCbfunc_num != 0 || _sceCd_c_cb_sem != 0 || signals != 1 || sema_seen != 5) return 12;
    std::puts("sceCdRead contract passed");
    return 0;
}
