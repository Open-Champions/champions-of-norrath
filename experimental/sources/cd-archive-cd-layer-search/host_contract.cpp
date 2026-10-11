#include <cstdio>
#include <cstdlib>
#include <cstring>
extern "C" int _sceCd_ncmd_semid = 6;
extern "C" int ncmd_sema_keep_cmd = 0;
extern "C" int _sf_bind = 1;
extern "C" int SCE_CD_debug = 0;
extern "C" int my_thid = 3;
extern "C" unsigned char my_th_info[8] = {};
extern "C" unsigned char cdrdsf[64] = {};
extern "C" unsigned char _sf_data[300] = {};
extern "C" unsigned char sfrdata[4] = {};
static int inits;
static int polls;
static int poll_result;
static int sync_mode;
static int sync_result;
static int signals;
static int binds;
static int rpc_result;
static int flushes;
extern "C" void cmd_sem_init() { ++inits; }
extern "C" int PollSema(int semaphore) {
    if (semaphore != 6) return -1;
    ++polls;
    return poll_result;
}
extern "C" int ReferThreadStatus(int thread, void* info) {
    if (thread != 3 || info != my_th_info) return -1;
    return 0;
}
extern "C" int norrath_archive_cd_sync(int mode) {
    sync_mode = mode;
    return sync_result;
}
extern "C" void sceSifInitRpc() {}
extern "C" int sceSifBindRpc(void*, unsigned server, unsigned) {
    ++binds;
    if (server != 0x80000597u) return -1;
    return 0;
}
extern "C" void sceSifWriteBackDCache(void* address, int bytes) {
    if (address != _sf_data || bytes != 300) std::_Exit(40);
    ++flushes;
}
extern "C" int sceSifCallRpc(void* client, unsigned function, unsigned, void* send, int send_size,
                             void* receive, int receive_size, void*, void*) {
    if (client != cdrdsf || function != 0 || send != _sf_data || send_size != 300) return -1;
    if (receive != sfrdata || receive_size != 4) return -1;
    return rpc_result;
}
extern "C" int norrath_sce_printf(const char*, ...) { return 0; }
extern "C" int norrath_player_signal_semaphore(int semaphore) {
    if (semaphore != 6) return -1;
    ++signals;
    return 0;
}
extern "C" int norrath_archive_cd_layer_search(unsigned char*, const char*, int);
static void put_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
int main() {
    unsigned char file[36] = {};
    poll_result = 1;
    if (norrath_archive_cd_layer_search(file, "A", 1) != 0 || signals != 0 || polls != 1) return 1;
    poll_result = 6;
    sync_result = 1;
    if (norrath_archive_cd_layer_search(file, "A", 1) != 0 || signals != 1 || sync_mode != 1) return 2;
    sync_result = 0;
    rpc_result = -3;
    if (norrath_archive_cd_layer_search(file, "BG", 0) != 0 || signals != 2) return 3;
    if (std::strcmp(reinterpret_cast<char*>(_sf_data + 36), "BG") != 0) return 4;
    rpc_result = 0;
    for (int i = 0; i < 36; ++i) _sf_data[i] = static_cast<unsigned char>(i + 1);
    put_word(sfrdata, 0x15u);
    if (norrath_archive_cd_layer_search(file, "\\BG\\DATA", 1) != 0x15 || signals != 3) return 5;
    if (std::memcmp(file, _sf_data, 36) != 0) return 6;
    unsigned layer = _sf_data[296] | (_sf_data[297] << 8) | (_sf_data[298] << 16) | (_sf_data[299] << 24);
    if (layer != 1u || ncmd_sema_keep_cmd != 1 || inits != 4 || flushes != 2) return 7;
    char full[257];
    for (int i = 0; i < 256; ++i) full[i] = 'A';
    full[256] = 0;
    if (norrath_archive_cd_layer_search(file, full, 2) != 0x15) return 8;
    if (_sf_data[291] != 0 || _sf_data[36] != 'A') return 9;
    _sf_bind = -1;
    cdrdsf[36] = 1;
    if (norrath_archive_cd_layer_search(file, "Z", 0) != 0x15 || binds != 1 || _sf_bind != 0) return 10;
    std::puts("layer search contract passed");
    return 0;
}
