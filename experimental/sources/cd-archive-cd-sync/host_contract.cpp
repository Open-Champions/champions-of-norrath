#include <cstdio>
#include <cstdlib>
#include <cstring>
extern "C" int SCE_CD_debug = 0;
extern "C" int _sceCd_c_cb_sem = 0;
extern "C" unsigned char _sceCd_cd_ncmd[40] = {};
static int delays;
static int prints;
static int rpc_result;
static int clear_sem_after;
static int clear_rpc_after;
static const char* printed;
extern "C" int norrath_sce_printf(const char* format, ...) {
    ++prints;
    printed = format;
    return 0;
}
extern "C" void DelayThread(int microseconds) {
    if (microseconds != 4000) std::printf("bad delay %d\n", microseconds);
    if (microseconds != 4000) std::_Exit(20);
    ++delays;
    if (clear_sem_after != 0 && delays >= clear_sem_after) _sceCd_c_cb_sem = 0;
    if (clear_rpc_after != 0 && delays >= clear_rpc_after) rpc_result = 0;
}
extern "C" int sceSifCheckStatRpc(void* client) {
    if (client != _sceCd_cd_ncmd) std::_Exit(21);
    return rpc_result;
}
extern "C" int norrath_archive_cd_sync(int mode);
int main() {
    rpc_result = 0;
    if (norrath_archive_cd_sync(1) != 0 || delays != 0 || prints != 0) return 1;
    _sceCd_c_cb_sem = 3;
    if (norrath_archive_cd_sync(1) != 1) return 2;
    _sceCd_c_cb_sem = 0;
    rpc_result = 4;
    if (norrath_archive_cd_sync(2) != 1) return 3;
    rpc_result = 0;
    if (norrath_archive_cd_sync(2) != 0) return 4;
    _sceCd_c_cb_sem = 1;
    SCE_CD_debug = 1;
    delays = 0;
    rpc_result = 0;
    clear_sem_after = 2;
    clear_rpc_after = 0;
    if (norrath_archive_cd_sync(0) != 0 || delays != 2 || prints != 1) return 5;
    if (std::strcmp(printed, "N cmd wait\n") != 0) return 6;
    prints = 0;
    delays = 0;
    SCE_CD_debug = 0;
    _sceCd_c_cb_sem = 0;
    rpc_result = 1;
    clear_sem_after = 0;
    clear_rpc_after = 1;
    if (norrath_archive_cd_sync(0) != 0 || delays != 1 || prints != 0) return 7;
    SCE_CD_debug = -3;
    delays = 0;
    rpc_result = 0;
    clear_rpc_after = 0;
    if (norrath_archive_cd_sync(0) != 0 || prints != 0 || delays != 0) return 8;
    std::puts("cd sync contract passed");
    return 0;
}
