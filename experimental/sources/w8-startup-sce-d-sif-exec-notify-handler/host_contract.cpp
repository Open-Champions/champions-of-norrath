#define NORRATH_HOST_CONTRACT 1
#include "sif_exec_notify_handler.cpp"

#include <cstdio>

unsigned norrath_host_gp = 0x1000;
alignas(16) unsigned char g_SysRebootBuf[32 * sizeof(NotifyEntry)]; /* 512 bytes on the EE */
void* g_pUserRebootBuf;
int g_iUserRebootSize;

struct Call { unsigned argument; void* data; unsigned gp; };
static Call calls[40];
static int n;
static void record(unsigned argument, void* data) {
    if (n < 40) calls[n] = Call{argument, data, norrath_host_gp};
    n += 1;
}
static void other(unsigned, void*) { n += 100; }

int main() {
    NotifyEntry user[4] = {};
    int tag[4];
    user[0] = NotifyEntry{record, &tag[0], 0x2000, 0};
    user[2] = NotifyEntry{record, &tag[2], 0x3000, 0};
    user[3] = NotifyEntry{other, nullptr, 0x4000, 0};
    g_pUserRebootBuf = user;
    g_iUserRebootSize = 3; /* the fourth entry is beyond the count */
    sceSifExecNotifyHandler(0, 77);
    if (n != 2 || calls[0].argument != 77 || calls[0].data != &tag[0] || calls[0].gp != 0x2000) return 1;
    if (calls[1].data != &tag[2] || calls[1].gp != 0x3000 || norrath_host_gp != 0x1000) return 2;
    n = 0;
    g_iUserRebootSize = 0;
    sceSifExecNotifyHandler(0, 1);
    g_iUserRebootSize = -4;
    sceSifExecNotifyHandler(0, 1);
    if (n != 0 || norrath_host_gp != 0x1000) return 3;
    /* nonzero selects the system table, fixed 32 entries, and ignores the user table */
    NotifyEntry* sys = reinterpret_cast<NotifyEntry*>(g_SysRebootBuf);
    int stag[2];
    sys[0] = NotifyEntry{record, &stag[0], 0x5000, 0};
    sys[31] = NotifyEntry{record, &stag[1], 0x6000, 0};
    g_iUserRebootSize = 4;
    sceSifExecNotifyHandler(5, 9);
    if (n != 2 || calls[0].argument != 9 || calls[0].data != &stag[0] || calls[0].gp != 0x5000) return 4;
    if (calls[1].data != &stag[1] || calls[1].gp != 0x6000 || norrath_host_gp != 0x1000) return 5;
    std::puts("sif exec notify handler contract passed");
    return 0;
}
