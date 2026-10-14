#include <cstdio>
#include <string>
#include <vector>
#include "scmd_prechk.cpp"

static std::vector<std::string> trace;
static int poll_result = 4;
static int sync_result = 0;
extern "C" int _sceCd_scmd_semid = 4;
extern "C" int scmd_sema_keep_cmd = 0;
extern "C" int SCE_CD_debug = 0;
extern "C" int _scmd_bind = -1;
extern "C" int my_thid = 8;
extern "C" unsigned char my_th_info[4] = {};
extern "C" unsigned char _sceCd_cd_scmd[40] = {};
extern "C" void cmd_sem_init() { trace.push_back("sem"); }
extern "C" int PollSema(int semaphore) { return semaphore == 4 ? poll_result : -1; }
extern "C" int ReferThreadStatus(int, void*) { return 0; }
extern "C" int sceCdSyncS(int mode) { return mode == 1 ? sync_result : -1; }
extern "C" int norrath_player_signal_semaphore(int semaphore) {
    trace.push_back("signal:" + std::to_string(semaphore));
    return 0;
}
extern "C" void sceSifInitRpc() { trace.push_back("init"); }
extern "C" int sceSifBindRpc(void* client, unsigned server, unsigned mode) {
    trace.push_back("bind:" + std::to_string(server) + ":" + std::to_string(mode));
    return client == _sceCd_cd_scmd ? 0 : -1;
}
extern "C" int scePrintf(const char* format, ...) {
    trace.push_back(format);
    return 0;
}
int main() {
    poll_result = 0;
    SCE_CD_debug = 1;
    if (_sceCd_scmd_prechk(2) != 0 || trace.back() != std::string("Scmd fail sema cur_cmd:%d keep_cmd:%d\n")) return 1;
    poll_result = 4;
    sync_result = 1;
    trace.clear();
    if (_sceCd_scmd_prechk(2) != 0 || trace.back() != "signal:4") return 2;
    sync_result = 0;
    _scmd_bind = 1;
    if (_sceCd_scmd_prechk(6) != 1 || scmd_sema_keep_cmd != 6) return 3;
    _scmd_bind = -1;
    _sceCd_cd_scmd[36] = 1;
    trace.clear();
    if (_sceCd_scmd_prechk(6) != 1 || _scmd_bind != 0 || trace.back() != "bind:2147485075:0") return 4;
    std::puts("scmd prechk contract passed");
    return 0;
}
