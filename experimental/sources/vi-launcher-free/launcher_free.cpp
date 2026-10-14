// Observed Free__10VILauncher at 0x0106bd80, 356 bytes. It returns.
// The launcher pointer is not read. When gPowerOffTid and gPowerOffSid
// are both something other than -1, gPowerOffTerminate becomes 1,
// SignalSema receives the semaphore id, and ReferThreadStatus(thread, 0)
// repeats until it returns 16. Any other status calls
// RotateThreadReadyQueue(1) first. DeleteThread and DeleteSema then run,
// and both ids become -1. scePadEnd, sceMcEnd, "UNLOADING MODULES\n",
// exitEzNetCnf, and exitEzNetCtl follow. Each sModuleId word from index
// 15 down through 0 is left unchanged when it is negative. A nonnegative
// id is printed with "unload %x\n", stopped with a zero length and a null
// argument, and unloaded. A negative stop or unload result is printed.
// The slot then becomes -1. The thread and module words are defined by
// Init__10VILauncherb.
extern "C" {
struct VILauncher;
extern int gPowerOffTid;
extern int gPowerOffSid;
extern int gPowerOffTerminate;
extern int sModuleId[16];

void SignalSema(int identifier);
void RotateThreadReadyQueue(int priority);
int ReferThreadStatus(int thread, void* status);
void DeleteThread(int thread);
void DeleteSema(int identifier);
void scePadEnd();
void sceMcEnd();
int printf(const char* text, ...);
int exitEzNetCnf();
int exitEzNetCtl();
int sceSifStopModule(int identifier, int argument_length, const char* argument, int* result);
int sceSifUnloadModule(int identifier);

void Free__10VILauncher(VILauncher* object) {
    (void)object;
    if (gPowerOffTid != -1 && gPowerOffSid != -1) {
        gPowerOffTerminate = 1;
        SignalSema(gPowerOffSid);
        int status;
        do {
            status = ReferThreadStatus(gPowerOffTid, nullptr);
            if (status != 16) RotateThreadReadyQueue(1);
        } while (status != 16);
        DeleteThread(gPowerOffTid);
        DeleteSema(gPowerOffSid);
        gPowerOffSid = -1;
        gPowerOffTid = -1;
    }
    scePadEnd();
    sceMcEnd();
    printf("UNLOADING MODULES\n");
    exitEzNetCnf();
    exitEzNetCtl();
    for (int index = 15; index >= 0; --index) {
        int identifier = sModuleId[index];
        if (identifier < 0) continue;
        printf("unload %x\n", identifier);
        int stopped_result = 0;
        int stopped = sceSifStopModule(identifier, 0, nullptr, &stopped_result);
        if (stopped < 0) printf("stop failed %d\n", stopped);
        int unloaded = sceSifUnloadModule(identifier);
        if (unloaded < 0) printf("unload failed %d\n", unloaded);
        sModuleId[index] = -1;
    }
}
}
