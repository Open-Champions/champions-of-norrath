// Observed Init__10VILauncherb at 0x0106bc68, 276 bytes. It returns 0.
// The bool argument is not read. Words at offsets 0 and 4 are cleared
// before sceCdReadClock. srand receives byte 1 of that record. All 16
// words of sModuleId, from 0x011e5d20 through the next symbol 64 bytes
// later, are set to -1. sModuleId was local. It is published global so
// Free can share it. gPatchPresent, gPowerOffTid, and gPowerOffSid are
// then 0, -1, and -1. gPowerOffSid's file bytes are already -1.
// VIDisplayLoadingScreen(0), "za\n", "zb\n", initEzNetCtl, and "zc\n"
// follow. The current thread priority becomes 2. sceMcGetInfo is called
// with five zeros, then sceMcSync(0, stack, stack+4) and scePadInit(0).
// Bytes at offsets 8 and 264 are cleared before VIDisplayLoadingScreen(1).
extern "C" {
struct VILauncher;
int sceCdReadClock(unsigned char* record);
void srand(unsigned seed);
void VIDisplayLoadingScreen__Fb(int argument);
int printf(const char* text, ...);
void initEzNetCtl();
int GetThreadId();
void ChangeThreadPriority(int thread, int priority);
int sceMcGetInfo(int first, int second, int third, int fourth, int fifth);
int sceMcSync(int mode, void* first, void* second);
void scePadInit(int mode);

int gPatchPresent;
int gPowerOffTid;
int gPowerOffSid = -1;
int gPowerOffTerminate;
int sModuleId[16];

int Init__10VILauncherb(VILauncher* object, int network_up) {
    (void)network_up;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(object);
    unsigned char clock_record[8];
    *reinterpret_cast<unsigned*>(bytes) = 0;
    *reinterpret_cast<unsigned*>(bytes + 4) = 0;
    sceCdReadClock(clock_record);
    srand(clock_record[1]);
    for (int index = 15; index >= 0; --index) sModuleId[index] = -1;
    gPatchPresent = 0;
    gPowerOffTid = -1;
    gPowerOffSid = -1;
    VIDisplayLoadingScreen__Fb(0);
    printf("za\n");
    printf("zb\n");
    initEzNetCtl();
    printf("zc\n");
    ChangeThreadPriority(GetThreadId(), 2);
    sceMcGetInfo(0, 0, 0, 0, 0);
    int sync_first = 0;
    int sync_second = 0;
    sceMcSync(0, &sync_first, &sync_second);
    scePadInit(0);
    bytes[8] = 0;
    bytes[264] = 0;
    VIDisplayLoadingScreen__Fb(1);
    return 0;
}
}
