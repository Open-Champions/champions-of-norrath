#include <cstdio>
extern "C" unsigned _sceCd_ee_read_mode = 0;
extern "C" int SCE_CD_debug = 0;
extern "C" int _sceCd_c_cb_sem = 7;
extern "C" unsigned norrath_archive_cd_command_count_view = 4;
extern "C" int _sceCd_ncmd_semid = 5;
extern "C" unsigned char _sceCd_ncmdsdata[64] = {};
extern "C" unsigned char _sceCd_rd_intr_data[8] = {};
extern "C" unsigned char _sceCd_Read_cur_pos[8] = {};
extern "C" unsigned char _sceCd_cd_ncmd[8] = {};
static int ready_calls;
static int ready_result;
static int prechk_command;
static int prechk_result;
static int rpc_result;
static unsigned rpc_function;
static void* rpc_parameter;
static int flushes;
static int flush_bytes;
static void* flush_address;
static int prints;
static int signals;
extern "C" int sceCdNcmdDiskReady() { ++ready_calls; return ready_result; }
extern "C" int _sceCd_ncmd_prechk(int command) { prechk_command = command; return prechk_result; }
extern "C" void sceSifWriteBackDCache(void* address, int bytes) {
    ++flushes;
    flush_address = address;
    flush_bytes = bytes;
}
extern "C" int norrath_sce_printf(const char*, ...) { ++prints; return 0; }
extern "C" void _sceCd_cd_callback() {}
extern "C" int sceSifCallRpc(void*, unsigned function, unsigned, void*, int, void*, int, void*, void* parameter) {
    rpc_function = function;
    rpc_parameter = parameter;
    return rpc_result;
}
extern "C" int norrath_player_signal_semaphore(int) { ++signals; return 0; }
extern "C" int norrath_archive_cd_read_iop(unsigned, unsigned, void*, const unsigned char*);
int main() {
    unsigned char mode[3] = {1, 2, 3};
    unsigned char destination[4] = {};
    ready_result = 6;
    if (norrath_archive_cd_read_iop(8, 2, destination, mode) != 0 || ready_calls != 1 || flushes != 0) return 1;
    _sceCd_ee_read_mode = 1u;
    prechk_result = 0;
    if (norrath_archive_cd_read_iop(8, 2, destination, mode) != 0 || prechk_command != 5) return 2;
    prechk_result = 1;
    rpc_result = -2;
    if (norrath_archive_cd_read_iop(8, 2, destination, mode) != 0 || signals != 1) return 3;
    if (flushes != 1 || flush_bytes != 24 || flush_address != _sceCd_ncmdsdata) return 4;
    if (rpc_function != 13u || rpc_parameter != &norrath_archive_cd_command_count_view) return 5;
    if (_sceCd_ncmdsdata[12] != 1 || _sceCd_ncmdsdata[13] != 2 || _sceCd_ncmdsdata[14] != 3) return 6;
    if (norrath_archive_cd_command_count_view != 0 || _sceCd_c_cb_sem != 0) return 7;
    rpc_result = 0;
    SCE_CD_debug = 1;
    _sceCd_Read_cur_pos[0] = 0x5a;
    if (norrath_archive_cd_read_iop(8, 2, destination, mode) != 1 || prints != 1) return 8;
    if (_sceCd_Read_cur_pos[0] != 0x5a || _sceCd_c_cb_sem != 1) return 9;
    std::puts("cd read iop contract passed");
    return 0;
}
