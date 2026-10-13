// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp sif_remove_reboot_notify_handler.cpp
#include <cstring>
extern "C" {
void* g_pUserRebootBuf;
int g_iUserRebootSize;
unsigned char g_SysRebootBuf[512];
int sceSifRemoveRebootNotifyHandler(unsigned identifier);
}
alignas(8) static unsigned char user[16 * 5];
static void fill() {
    std::memset(g_SysRebootBuf, 0xaa, sizeof g_SysRebootBuf);
    std::memset(user, 0xbb, sizeof user);
}
static unsigned word(const unsigned char* base, unsigned entry) {
    unsigned v; std::memcpy(&v, base + entry * 16, 4); return v;
}
int main() {
    g_pUserRebootBuf = user; g_iUserRebootSize = 5;
    fill();
    // user table: entry 3 loses only its first word
    if (sceSifRemoveRebootNotifyHandler(3) != 0 || word(user, 3) != 0) return 1;
    for (unsigned i = 0; i < 80; ++i) if (i / 16 != 3 || i % 16 >= 4) if (user[i] != 0xbb) return 2;
    for (unsigned char c : g_SysRebootBuf) if (c != 0xaa) return 3;
    // last valid and first invalid user index
    fill();
    if (sceSifRemoveRebootNotifyHandler(4) != 0 || word(user, 4) != 0) return 4;
    if (sceSifRemoveRebootNotifyHandler(5) != -105 || sceSifRemoveRebootNotifyHandler(0x7fffffff) != -105) return 5;
    // system table: top bit selects it and is dropped from the index
    fill();
    if (sceSifRemoveRebootNotifyHandler(0x80000005u) != 0 || word(g_SysRebootBuf, 5) != 0) return 6;
    for (unsigned i = 0; i < 512; ++i) if (i / 16 != 5 || i % 16 >= 4) if (g_SysRebootBuf[i] != 0xaa) return 7;
    for (unsigned char c : user) if (c != 0xbb) return 8;
    fill();
    if (sceSifRemoveRebootNotifyHandler(0x8000001fu) != 0 || word(g_SysRebootBuf, 31) != 0) return 9;
    if (sceSifRemoveRebootNotifyHandler(0x80000020u) != -105 || sceSifRemoveRebootNotifyHandler(0xffffffffu) != -105) return 10;
    // the system count is fixed at 32 whatever the user size says
    g_iUserRebootSize = 100; fill();
    if (sceSifRemoveRebootNotifyHandler(0x80000020u) != -105 || sceSifRemoveRebootNotifyHandler(0x80000001u) != 0) return 11;
    // an empty user table refuses everything, including index zero; a negative size counts as huge (unsigned compare)
    g_iUserRebootSize = 0; fill();
    if (sceSifRemoveRebootNotifyHandler(0) != -105) return 12;
    g_iUserRebootSize = -1; g_pUserRebootBuf = user; fill();
    if (sceSifRemoveRebootNotifyHandler(2) != 0 || word(user, 2) != 0) return 13;
    return 0;
}
