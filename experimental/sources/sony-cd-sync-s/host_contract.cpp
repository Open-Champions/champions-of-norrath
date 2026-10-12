#include <cstdio>
#include <vector>
#include "sync_s.cpp"
static int checks = 0;
static int delays = 0;
static int busy = 2;
extern "C" int SCE_CD_debug = 1;
extern "C" unsigned char _sceCd_cd_scmd[4] = {};
extern "C" int scePrintf(const char* format, ...) { return format[0] == 'S' ? 0 : 1; }
extern "C" void DelayThread(int microseconds) {
    if (microseconds != 4000) delays = -100;
    delays += 1;
}
extern "C" int sceSifCheckStatRpc(void* client) {
    if (client != _sceCd_cd_scmd) return -1;
    checks += 1;
    if (busy == 0) return 0;
    busy -= 1;
    return 1;
}
int main() {
    if (sceCdSyncS(1) != 1 || delays != 0) return 1;
    busy = 2;
    checks = 0;
    if (sceCdSyncS(0) != 0 || delays != 2 || checks != 3) return 2;
    std::puts("sceCdSyncS contract passed");
    return 0;
}
