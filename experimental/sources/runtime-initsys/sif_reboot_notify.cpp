// Observed sceSifInitRebootNotify at 0x002df5f8. The two reboot publication
// words are cleared, then 512 bytes at g_SysRebootBuf are filled with zero.
// That span matches the symbol's extent. Kernel reboot behavior is not claimed.
extern "C" {
extern void* g_pUserRebootBuf;
extern int g_iUserRebootSize;
extern unsigned char g_SysRebootBuf[];
void* memset(void* destination, int value, __SIZE_TYPE__ count);
void sceSifInitRebootNotify() {
    g_pUserRebootBuf = 0;
    g_iUserRebootSize = 0;
    memset(g_SysRebootBuf, 0, 512);
}
}
