#include <cstdio>
#include <cstdint>
#include <cstring>
extern "C" unsigned _sceCd_ee_read_mode = 0;
extern "C" int SCE_CD_debug = 0;
extern "C" int _sceCd_c_cb_sem = 0;
extern "C" unsigned norrath_archive_cd_command_count_view = 0;
extern "C" int _sceCd_ncmd_semid = 5;
extern "C" unsigned char _sceCd_ncmdsdata[64] = {};
extern "C" unsigned char _sceCd_rd_intr_data[144] = {};
extern "C" unsigned char _sceCd_Read_cur_pos[16] = {};
extern "C" unsigned char _sceCd_cd_ncmd[40] = {};
static int ready_result;
static int ready_calls;
static int prechk_command;
static int prechk_result;
static int rpc_result;
static unsigned rpc_function;
static unsigned rpc_mode;
static void* rpc_end;
static void* rpc_parameter;
static int signals;
static int flushes;
static void* flush_address[4];
static int flush_bytes[4];
static int prints;
extern "C" int sceCdNcmdDiskReady() { ++ready_calls; return ready_result; }
extern "C" int _sceCd_ncmd_prechk(int command) { prechk_command = command; return prechk_result; }
extern "C" void sceSifWriteBackDCache(void* address, int bytes) {
    flush_address[flushes] = address;
    flush_bytes[flushes] = bytes;
    ++flushes;
}
extern "C" int norrath_sce_printf(const char*, ...) { ++prints; return 0; }
extern "C" void _sceCd_cd_read_intr() {}
extern "C" int sceSifCallRpc(void*, unsigned function, unsigned mode, void*, int, void*, int,
                             void* end_function, void* end_parameter) {
    rpc_function = function;
    rpc_mode = mode;
    rpc_end = end_function;
    rpc_parameter = end_parameter;
    return rpc_result;
}
extern "C" int norrath_player_signal_semaphore(int semaphore) {
    if (semaphore != 5) return 1;
    ++signals;
    return 0;
}
extern "C" int norrath_archive_cd_read(unsigned, unsigned, void*, const unsigned char*);
static unsigned word_at(int offset) {
    return _sceCd_ncmdsdata[offset] | (_sceCd_ncmdsdata[offset + 1] << 8) |
           (_sceCd_ncmdsdata[offset + 2] << 16) | (_sceCd_ncmdsdata[offset + 3] << 24);
}
int main() {
    unsigned char mode[4] = {2, 0, 0, 0};
    unsigned char destination[16] = {};
    ready_result = 6;
    if (norrath_archive_cd_read(10, 1, destination, mode) != 0 || ready_calls != 1) return 1;
    _sceCd_ee_read_mode = 1u;
    ready_calls = 0;
    prechk_result = 0;
    if (norrath_archive_cd_read(10, 1, destination, mode) != 0 || ready_calls != 0 || prechk_command != 4) return 2;
    prechk_result = 1;
    rpc_result = -1;
    mode[2] = 0;
    flushes = 0;
    _sceCd_Read_cur_pos[0] = 9;
    if (norrath_archive_cd_read(10, 3, destination, mode) != 0 || signals != 1) return 3;
    if (word_at(0) != 10 || word_at(4) != 3) return 4;
    if (flush_bytes[0] != 3 * 2048 || flush_address[0] != destination) return 5;
    if (flush_bytes[1] != 144 || flush_bytes[2] != 24 || flush_bytes[3] != 4) return 6;
    if (_sceCd_Read_cur_pos[0] != 0 || norrath_archive_cd_command_count_view != 0 || _sceCd_c_cb_sem != 0) return 7;
    if (rpc_function != 1 || rpc_mode != 1 || rpc_end != reinterpret_cast<void*>(_sceCd_cd_read_intr)) return 8;
    if (rpc_parameter != _sceCd_rd_intr_data) return 9;
    rpc_result = 0;
    mode[2] = 1;
    flushes = 0;
    _sceCd_ee_read_mode = 1u | 2u;
    if (norrath_archive_cd_read(4, 2, destination, mode) != 1 || flushes != 3) return 10;
    if (flush_address[0] != _sceCd_rd_intr_data) return 11;
    mode[2] = 2;
    flushes = 0;
    _sceCd_ee_read_mode = 1u;
    SCE_CD_debug = 1;
    if (norrath_archive_cd_read(1, 2, destination, mode) != 1 || flush_bytes[0] != 2 * 2340) return 12;
    if (prints != 2 || _sceCd_c_cb_sem != 1) return 13;
    mode[2] = 9;
    flushes = 0;
    SCE_CD_debug = 0;
    if (norrath_archive_cd_read(1, 2, destination, mode) != 1 || flush_bytes[0] != 4096) return 14;
    std::puts("cd read contract passed");
    return 0;
}
