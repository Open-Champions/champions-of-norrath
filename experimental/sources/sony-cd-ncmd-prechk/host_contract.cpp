#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include "ncmd_prechk.cpp"

static std::vector<std::string> trace;
static int poll_result = 3;
static int sync_result = 0;
static int bind_result = 0;
static int binds = 0;
extern "C" int _sceCd_ncmd_semid = 3;
extern "C" int ncmd_sema_keep_cmd = 0;
extern "C" int SCE_CD_debug = 0;
extern "C" int _ncmd_bind = -1;
extern "C" int my_thid = 8;
extern "C" unsigned char my_th_info[4] = {};
extern "C" unsigned char _sceCd_cd_ncmd[40] = {};
extern "C" void cmd_sem_init() { trace.push_back("sem"); }
extern "C" int PollSema(int semaphore) {
    trace.push_back("poll:" + std::to_string(semaphore));
    return poll_result;
}
extern "C" int ReferThreadStatus(int thread, void* info) {
    trace.push_back(thread == 8 && info == my_th_info ? "refer" : "refer-bad");
    return 0;
}
extern "C" int norrath_archive_cd_sync(int mode) {
    trace.push_back("sync:" + std::to_string(mode));
    return sync_result;
}
extern "C" int norrath_player_signal_semaphore(int semaphore) {
    trace.push_back("signal:" + std::to_string(semaphore));
    return 0;
}
extern "C" void sceSifInitRpc() { trace.push_back("init"); }
extern "C" int sceSifBindRpc(void* client, unsigned server, unsigned mode) {
    binds += 1;
    trace.push_back("bind:" + std::to_string(server) + ":" + std::to_string(mode));
    if (client != _sceCd_cd_ncmd) trace.push_back("bind-bad");
    return bind_result;
}
extern "C" int scePrintf(const char* format, ...) {
    trace.push_back(std::string("print:") + format);
    return 0;
}
int main() {
    poll_result = 1;
    if (_sceCd_ncmd_prechk(4) != 0 || trace[0] != "sem" || trace[1] != "poll:3") return 1;
    SCE_CD_debug = 1;
    ncmd_sema_keep_cmd = 9;
    trace.clear();
    if (_sceCd_ncmd_prechk(4) != 0) return 2;
    if (trace.back() != "print:Ncmd fail sema cur_cmd:%d keep_cmd:%d\n") return 3;
    poll_result = 3;
    sync_result = 1;
    trace.clear();
    if (_sceCd_ncmd_prechk(4) != 0 || trace.back() != "signal:3") return 4;
    sync_result = 0;
    _ncmd_bind = 2;
    trace.clear();
    if (_sceCd_ncmd_prechk(4) != 1 || ncmd_sema_keep_cmd != 4) return 5;
    if (trace[2] != "refer" || trace[3] != "sync:1" || trace[4] != "init") return 6;
    _ncmd_bind = -1;
    bind_result = 0;
    _sceCd_cd_ncmd[36] = 1;
    trace.clear();
    binds = 0;
    if (_sceCd_ncmd_prechk(7) != 1 || binds != 1 || _ncmd_bind != 0) return 7;
    if (trace.back() != "bind:2147485077:0") return 8;
    std::puts("ncmd prechk contract passed");
    return 0;
}
