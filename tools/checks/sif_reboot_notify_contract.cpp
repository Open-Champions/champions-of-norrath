#include <cstdio>
static unsigned char buffer[512];
static int calls;
extern "C" void* g_pUserRebootBuf = &calls;
extern "C" int g_iUserRebootSize = 7;
extern "C" unsigned char g_SysRebootBuf[512] = {};
extern "C" void* memset(void* destination, int value, __SIZE_TYPE__ count) {
    ++calls;
    if (destination != g_SysRebootBuf || value != 0 || count != 512) return 0;
    for (unsigned i = 0; i < 512; ++i) g_SysRebootBuf[i] = 0;
    return destination;
}
extern "C" void sceSifInitRebootNotify();
int main() {
    for (int i = 0; i < 512; ++i) g_SysRebootBuf[i] = 9;
    sceSifInitRebootNotify();
    if (g_pUserRebootBuf != 0 || g_iUserRebootSize != 0 || calls != 1) return 1;
    for (int i = 0; i < 512; ++i) if (g_SysRebootBuf[i] != 0) return 2;
    (void)buffer;
    std::puts("reboot notification clear passed");
}
