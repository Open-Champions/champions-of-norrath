#include <cstdio>
#include <cstring>
#define NORRATH_HOST_CONTRACT 1
#include "layer_search_file.cpp"
extern "C" int SCE_CD_debug = 0;
extern "C" int _sceCd_ncmd_semid = 7;
extern "C" int ncmd_sema_keep_cmd = 0;
extern "C" int _sf_bind = -1;
extern "C" int my_thid = 3;
extern "C" unsigned char my_th_info[4] = {};
extern "C" alignas(4) unsigned char cdrdsf[64] = {};
extern "C" alignas(4) unsigned char _sf_data[320] = {};
extern "C" alignas(4) unsigned char sfrdata[8] = {};
static int poll_hit = 1, sync_result = 0, rpc_result = 0, signals = 0, init_rpcs = 0, binds = 0, bind_fail_first = 0;
static int calls_rpc = 0, wb = 0, prints = 0, order_bad = 0, sem_inits = 0, refers = 0, bad = 0;
static unsigned bind_server;
static char last_search_print[300];
extern "C" void cmd_sem_init() { ++sem_inits; }
extern "C" int PollSema(int semaphore) { return poll_hit ? semaphore : -1; }
extern "C" int ReferThreadStatus(int thread, void* info) { if (thread == 3 && info == my_th_info) ++refers; return 0; }
extern "C" int sceCdSync(int mode) { if (mode != 1) bad = 1; return sync_result; }
extern "C" int norrath_player_signal_semaphore(int semaphore) { if (semaphore == 7) ++signals; return 0; }
extern "C" void sceSifInitRpc() { ++init_rpcs; }
extern "C" int sceSifBindRpc(void* client, unsigned server, unsigned mode) {
    ++binds; bind_server = server;
    if (client != cdrdsf || mode != 0) bad = 1;
    if (binds <= bind_fail_first) return -1;
    if (binds == bind_fail_first + 1) return 0;          // success but client word 36 still zero
    cdrdsf[36] = 1;
    return 0;
}
extern "C" void sceSifWriteBackDCache(void* address, int bytes) { if (address == _sf_data && bytes == 300) ++wb; else bad = 1; }
extern "C" int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                             void* receive, int receive_size, void* end, void* end_param) {
    ++calls_rpc;
    if (client != cdrdsf || function != 0 || mode != 0 || send != _sf_data || send_size != 300 ||
        receive != sfrdata || receive_size != 4 || end || end_param) bad = 1;
    if (wb != 1) order_bad = 1;
    std::strncpy(last_search_print, reinterpret_cast<char*>(_sf_data + 36), sizeof last_search_print - 1);
    if (rpc_result >= 0) {
        // The IOP rewrites the head of the shared buffer in place.
        for (int index = 0; index < 36; ++index) _sf_data[index] = static_cast<unsigned char>(0x40 + index);
        sfrdata[0] = 0x78; sfrdata[1] = 0x56; sfrdata[2] = 0x34; sfrdata[3] = 0x12;
    }
    return rpc_result;
}
extern "C" int scePrintf(const char*, ...) { ++prints; return 0; }
static void reset() {
    poll_hit = 1; sync_result = 0; rpc_result = 0; signals = init_rpcs = binds = bind_fail_first = 0;
    calls_rpc = wb = prints = order_bad = sem_inits = refers = bad = 0;
    _sf_bind = -1; ncmd_sema_keep_cmd = 0; std::memset(cdrdsf, 0, sizeof cdrdsf);
    std::memset(_sf_data, 0, sizeof _sf_data); std::memset(sfrdata, 0, sizeof sfrdata);
}
int main() {
    unsigned char file[48];
    const unsigned char name[] = "\\ABC.BIN;1";
    reset(); poll_hit = 0;
    if (sceCdLayerSearchFile(file, name, 0) != 0u || sem_inits != 1 || calls_rpc != 0 || signals != 0 || ncmd_sema_keep_cmd != 0) return 1;
    reset(); sync_result = 1;
    if (sceCdLayerSearchFile(file, name, 0) != 0u || signals != 1 || ncmd_sema_keep_cmd != 1 || refers != 1 || init_rpcs != 0) return 2;
    // Full path: bind retried (one failure, one success without the word, then ready), RPC succeeds.
    reset(); bind_fail_first = 1; SCE_CD_debug = 1;
    unsigned result = sceCdLayerSearchFile(file, name, 1);
    if (result != 0x12345678u || bad || order_bad) return 3;
    if (binds != 3 || bind_server != 0x80000597u || _sf_bind != 0 || init_rpcs != 1 || signals != 1) return 4;
    if (std::strcmp(reinterpret_cast<char*>(_sf_data + 36), "\\ABC.BIN;1") != 0 || last_search_print[0] != '\\') return 5;
    if (_sf_data[296] != 1 || _sf_data[292] != (reinterpret_cast<unsigned long>(_sf_data) & 0xff)) return 6;
    for (int index = 0; index < 36; ++index) if (file[index] != 0x40 + index) return 7;
    if (prints != 1 + 1 + 3) return 8;      // bind error, call, three result lines
    // Already bound: no bind, debug off: no print.
    reset(); _sf_bind = 0; SCE_CD_debug = 0;
    if (sceCdLayerSearchFile(file, name, 0) != 0x12345678u || binds != 0 || prints != 0) return 9;
    // A 256-byte name without terminator is cut and the last byte zeroed.
    reset(); _sf_bind = 0;
    unsigned char longname[300]; std::memset(longname, 'Q', sizeof longname); longname[299] = 0;
    sceCdLayerSearchFile(file, longname, 0);
    if (_sf_data[36 + 255] != 0 || _sf_data[291] != 0 || _sf_data[36 + 254] != 'Q') return 10;
    // Name of exactly 255 characters keeps its terminator.
    reset(); _sf_bind = 0;
    longname[255] = 0;
    sceCdLayerSearchFile(file, longname, 0);
    if (_sf_data[36 + 254] != 'Q' || _sf_data[36 + 255] != 0) return 11;
    // Empty name.
    reset(); _sf_bind = 0; _sf_data[36] = 9;
    const unsigned char empty[] = "";
    sceCdLayerSearchFile(file, empty, 0);
    if (_sf_data[36] != 0) return 12;
    // RPC failure.
    reset(); _sf_bind = 0; rpc_result = -1;
    std::memset(file, 0xee, sizeof file);
    if (sceCdLayerSearchFile(file, name, 0) != 0u || signals != 1 || file[0] != 0xee) return 13;
    std::puts("sceCdLayerSearchFile contract passed");
    return 0;
}
