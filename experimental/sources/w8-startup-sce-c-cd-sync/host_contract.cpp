#include <cstdio>
#include <cstring>
#include "cd_sync.cpp"
extern "C" int SCE_CD_debug = 1;
extern "C" int _sceCd_c_cb_sem = 0;
extern "C" unsigned char _sceCd_cd_ncmd[40] = {};
static int delays, prints, checks, busy_checks, sem_clear_after = -1;
extern "C" int scePrintf(const char* format, ...) {
    if (std::strcmp(format, "N cmd wait\n") != 0) return -1;
    ++prints;
    return 0;
}
extern "C" void DelayThread(int microseconds) {
    if (microseconds != 4000) delays = -1000;
    ++delays;
    if (delays == sem_clear_after) _sceCd_c_cb_sem = 0;
}
extern "C" int sceSifCheckStatRpc(void* client) {
    if (client != _sceCd_cd_ncmd) return -7;
    ++checks;
    if (busy_checks > 0) { --busy_checks; return 1; }
    return 0;
}
static void reset(int debug, int sem, int busy, int clear_after) {
    SCE_CD_debug = debug; _sceCd_c_cb_sem = sem; busy_checks = busy;
    sem_clear_after = clear_after; delays = prints = checks = 0;
}
int main() {
    // Poll mode: callback word wins without consulting the RPC.
    reset(1, 5, 0, -1);
    if (sceCdSync(1) != 1 || checks != 0 || delays != 0 || prints != 0) return 1;
    reset(1, 0, 1, -1);
    if (sceCdSync(1) != 1 || checks != 1) return 2;
    reset(1, 0, 0, -1);
    if (sceCdSync(2) != 0 || checks != 1 || prints != 0) return 3;
    // Blocking mode: already idle, one print, no delay.
    reset(1, 0, 0, -1);
    if (sceCdSync(0) != 0 || prints != 1 || delays != 0 || checks != 1) return 4;
    // Debug off or negative: no print.
    reset(0, 0, 0, -1);
    if (sceCdSync(0) != 0 || prints != 0) return 5;
    reset(-3, 0, 0, -1);
    if (sceCdSync(0) != 0 || prints != 0) return 6;
    // RPC busy for 2 checks: two delays; the callback word is rechecked first.
    reset(1, 0, 2, -1);
    if (sceCdSync(0) != 0 || delays != 2 || checks != 3) return 7;
    // Callback word held for 3 delays: RPC not consulted while it is set.
    reset(1, 1, 0, 3);
    if (sceCdSync(0) != 0 || delays != 3 || checks != 1) return 8;
    std::puts("sceCdSync contract passed");
    return 0;
}
