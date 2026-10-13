// Observed: RunPatcher__7VISetup at 0x011503f8 is 1408 bytes. It returns 4 on an error
// or an early exit of the host loop and 3 once the closing fade has completed. Byte
// offsets are from the object. The DNAS status is a 16-byte record of four words: the
// state at 0, a code at 4 and a progress value at 8.
//  Set-up: sLastTickTime (a 64-bit local at 0x011ee180) = 0; words 0x153d8, 0x153dc,
//  0x153c0, 0x153d0 and 0x153d4 = 0. Then the attempt below runs; it is repeated from
//  its first line when a forced timeout occurs.
//  Attempt:
//   1. The status record is cleared and a "retry" flag cleared;
//      SetRoot__15VIWindowManagerP5VIWnd(this + 0xaf64, this + 0x12060) and
//      SetFocus__5VIWndP5VIWndb(manager, window, true); the status record is copied to
//      this + 0x124e0; MainLoopBegin__7VISetup(this) and MainLoopEnd__7VISetup(this) run
//      20 times; VIDnasInit__Fv().
//   2. While VIDnasGetStatus__FP14sceDNAS2Status(status) is nonzero: the status is copied
//      to this + 0x124e0, MainLoopBegin, then MainLoopEnd; if either result is negative
//      (the original moves the Begin result into a saved register in the delay slot of the
//      End call and tests it with the End sign) it calls VIDnasShutdown__Fv() and returns 4.
//   3. VIDnasAuthInstall__Fv(); a second record "last" is cleared; start = GetTime__7VITimer().
//   4. Loop: if VIDnasGetStatus is nonzero it prints "error getting status!\n", marks the
//      run failed and leaves the loop. Otherwise GetThreadId, SetAlarm(250,
//      sAlarmCallback__FiUsPv, thread), SleepThread, ReleaseAlarm(alarm), then
//      MainLoopBegin and MainLoopEnd. If memcmp(last, status, 16) differs: last = status,
//      start = GetTime__7VITimer(), then by state S and code C:
//        S == 6 and C is -105 or -101: status copied to this + 0x124e0, run marked failed,
//          two Begin/End pairs, "server timeout\n", leave loop.
//        otherwise status copied to this + 0x124e0 and two Begin/End pairs, then S == 5 or
//          S == 8: "finished successfully\n", leave loop; S == 6 or 7: run failed, "error
//          during process\n", leave loop; else print "------------------\n", "code     :
//          %d\n" (S), "sub-code : %d\n" (C) and "progress : %d\n" (word 8).
//      Whether or not the record changed, if (int)(GetTime() - start) > 60000 it stores
//      retry = 1, prints "FORCED TIMEOUT!\n", clears the failed mark and leaves the loop.
//   5. VIDnasShutdown__Fv(); if retry was set, the attempt restarts.
//  Finish: if the run failed, MainLoopBegin and MainLoopEnd repeat while MainLoopEnd is not
//  negative and MainLoopBegin's result is positive, then return 4. Otherwise 60 Begin/End pairs run, Begin__7VIFaderff(
//  this + 0xae38, 1.0f, 0.5f) is called and the loop of Begin/End pairs runs until
//  either result is negative (return 4) or the float at 8 of the fader equals 1.0f (return 3);
//  that float is also tested once before the loop.
// Inferred: this is the DNAS authentication/patch wait screen; states 5 and 8 are success
// and 6 and 7 failure. Unknown: the meaning of codes -105 and -101, and the field roles of
// the words cleared at the start.
extern "C" {
extern unsigned long long sLastTickTime;

int printf(const char* format, ...);
void* memset(void* destination, int value, unsigned count);
int memcmp(const void* a, const void* b, unsigned count);
long long GetTime__7VITimer();
int GetThreadId();
int SetAlarm(unsigned short ticks, void (*callback)(int, unsigned short, void*), int thread);
int SleepThread();
int ReleaseAlarm(int alarm);
void sAlarmCallback__FiUsPv(int id, unsigned short time, void* arg);
int MainLoopBegin__7VISetup(void* setup);
int MainLoopEnd__7VISetup(void* setup);
int VIDnasInit__Fv();
int VIDnasGetStatus__FP14sceDNAS2Status(void* status);
int VIDnasAuthInstall__Fv();
int VIDnasShutdown__Fv();
void SetRoot__15VIWindowManagerP5VIWnd(void* manager, void* window);
void SetFocus__5VIWndP5VIWndb(void* window, void* target, bool flag);
void Begin__7VIFaderff(void* fader, float a, float b);

struct DnasStatus {
    int word[4];
};

int RunPatcher__7VISetup(void* object) {
    char* base = static_cast<char*>(object);
    char* manager = base + 0xaf64;
    char* window = base + 0x12060;
    DnasStatus* shown = reinterpret_cast<DnasStatus*>(base + 0x124e0);
    alignas(16) DnasStatus status;
    alignas(16) DnasStatus last;
    int retry;
    bool failed;
    sLastTickTime = 0;
    *reinterpret_cast<int*>(base + 0x153d8) = 0;
    *reinterpret_cast<int*>(base + 0x153dc) = 0;
    *reinterpret_cast<int*>(base + 0x153c0) = 0;
    *reinterpret_cast<int*>(base + 0x153d0) = 0;
    *reinterpret_cast<int*>(base + 0x153d4) = 0;
    do {
        retry = 0;
        memset(&status, 0, 16);
        failed = false;
        SetRoot__15VIWindowManagerP5VIWnd(manager, window);
        SetFocus__5VIWndP5VIWndb(manager, window, true);
        *shown = status;
        for (int count = 0; count < 20; ++count) {
            MainLoopBegin__7VISetup(object);
            MainLoopEnd__7VISetup(object);
        }
        VIDnasInit__Fv();
        while (VIDnasGetStatus__FP14sceDNAS2Status(&status) != 0) {
            *shown = status;
            int begun = MainLoopBegin__7VISetup(object);
            int ended = MainLoopEnd__7VISetup(object);
            if (ended < 0 || begun < 0) {
                VIDnasShutdown__Fv();
                return 4;
            }
        }
        VIDnasAuthInstall__Fv();
        memset(&last, 0, 16);
        long long start = GetTime__7VITimer();
        while (true) {
            if (VIDnasGetStatus__FP14sceDNAS2Status(&status) != 0) {
                failed = true;
                printf("error getting status!\n");
                break;
            }
            int alarm = SetAlarm(250, sAlarmCallback__FiUsPv, GetThreadId());
            SleepThread();
            ReleaseAlarm(alarm);
            MainLoopBegin__7VISetup(object);
            MainLoopEnd__7VISetup(object);
            if (memcmp(&last, &status, 16) != 0) {
                last = status;
                start = GetTime__7VITimer();
                if (status.word[0] == 6 && (status.word[1] == -105 || status.word[1] == -101)) {
                    *shown = status;
                    failed = true;
                    MainLoopBegin__7VISetup(object);
                    MainLoopEnd__7VISetup(object);
                    MainLoopBegin__7VISetup(object);
                    MainLoopEnd__7VISetup(object);
                    printf("server timeout\n");
                    break;
                }
                *shown = status;
                MainLoopBegin__7VISetup(object);
                MainLoopEnd__7VISetup(object);
                MainLoopBegin__7VISetup(object);
                MainLoopEnd__7VISetup(object);
                if (status.word[0] == 5 || status.word[0] == 8) {
                    printf("finished successfully\n");
                    break;
                }
                if (static_cast<unsigned>(status.word[0] - 6) < 2u) {
                    failed = true;
                    printf("error during process\n");
                    break;
                }
                printf("------------------\n");
                printf("code     : %d\n", status.word[0]);
                printf("sub-code : %d\n", status.word[1]);
                printf("progress : %d\n", status.word[2]);
            }
            if (static_cast<int>(GetTime__7VITimer() - start) > 60000) {
                retry = 1;
                printf("FORCED TIMEOUT!\n");
                failed = false;
                break;
            }
        }
        VIDnasShutdown__Fv();
    } while (retry != 0);
    if (failed) {
        while (true) {
            int begun = MainLoopBegin__7VISetup(object);
            int ended = MainLoopEnd__7VISetup(object);
            if (ended < 0 || begun <= 0) return 4;
        }
    }
    for (int count = 0; count < 60; ++count) {
        MainLoopBegin__7VISetup(object);
        MainLoopEnd__7VISetup(object);
    }
    float* fader = reinterpret_cast<float*>(base + 0xae38);
    Begin__7VIFaderff(base + 0xae38, 1.0f, 0.5f);
    if (fader[2] == 1.0f) return 3;
    while (true) {
        int begun = MainLoopBegin__7VISetup(object);
        int ended = MainLoopEnd__7VISetup(object);
        if (ended < 0 || begun < 0) return 4;
        if (fader[2] == 1.0f) return 3;
    }
}
}
