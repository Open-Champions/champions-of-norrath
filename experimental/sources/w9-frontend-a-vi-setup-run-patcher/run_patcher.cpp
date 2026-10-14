// Observed: RunPatcher__7VISetup at 0x011503f8 is 1408 bytes. It returns 3 or 4. Names below
// are the image's; MainLoopBegin/MainLoopEnd are MainLoopBegin__7VISetup and MainLoopEnd__7VISetup
// called with this; "status" is a 16-byte block of four words filled by VIDnasGetStatus__FP14
// sceDNAS2Status, which returns nonzero on failure. "Window copy" is a 16-byte copy to this +
// 0x124e0.
//  1. The 64-bit word sLastTickTime is cleared, and the words at this + 0x153d8, 0x153dc, 0x153c0,
//     0x153d0 and 0x153d4 are set to 0. The status block is zeroed (memset 16). A retry flag
//     starts at 0 and a "went wrong" flag at 0.
//  2. SetRoot__15VIWindowManagerP5VIWnd(this + 0xaf64, this + 0x12060) and SetFocus(…, 1). The
//     status block (still zero) is window copied. MainLoopBegin/End run 20 times. VIDnasInit__Fv().
//  3. While VIDnasGetStatus returns nonzero: window copy the status, MainLoopBegin, MainLoopEnd;
//     the "step value" is -1 when MainLoopEnd returned a negative value and otherwise what
//     MainLoopBegin returned (the original's delay slot copies MainLoopBegin's result). A
//     negative step value runs VIDnasShutdown__Fv() and returns 4.
//  4. VIDnasAuthInstall__Fv(); a second 16-byte block (previous status) is zeroed; the start time
//     is VITimer::GetTime(). Then until finished: when VIDnasGetStatus returns nonzero, the
//     "went wrong" flag is set and "error getting status!\n" printed. Otherwise
//     GetThreadId(); SetAlarm(250, sAlarmCallback__FiUsPv, thread id); SleepThread();
//     ReleaseAlarm(alarm id); MainLoopBegin; MainLoopEnd. When memcmp of previous status and status
//     over 16 bytes is nonzero the previous status becomes the status and the start time
//     VITimer::GetTime(); then
//       - status word 0 is 6 and word 1 is -105 or -101: the "went wrong" flag is set, the status is
//         window copied, MainLoopBegin/End run twice, "server timeout\n" is printed; finished.
//       - otherwise the status is window copied and MainLoopBegin/End run twice, then by word 0:
//         5 or 8 print "finished successfully\n" and finish (flag stays 0); 6 or 7 set the flag,
//         print "error during process\n" and finish; any other value prints 18 dashes plus a
//         newline, "code     : %d\n" with word 0, "sub-code : %d\n" with word 1 and
//         "progress : %d\n" with word 2 and goes on.
//     When nothing finished: if the 32-bit difference VITimer::GetTime() - start time is greater
//     than 60000, the retry flag is set, "FORCED TIMEOUT!\n" printed and the flag cleared;
//     finished. Otherwise the loop repeats.
//  5. VIDnasShutdown(). A set retry flag restarts at step 2 (status block zeroed again, flags
//     cleared) without redoing step 1.
//  6. With the "went wrong" flag set: MainLoopBegin/End repeat while the step value is positive;
//     the result is 4. Otherwise MainLoopBegin/End run 60 times,
//     Begin__7VIFaderff(this + 0xae38, 1.0f, 0.5f) runs and the loop MainLoopBegin;
//     MainLoopEnd repeats until the float at fader + 8 equals 1.0f (checked first and after each pass): a
//     negative MainLoopEnd result returns 4 at once, otherwise the result is 3.
// Inferred: the status block is the sceDNAS2 status (code, sub-code, progress); the 60000
// threshold is in VITimer::GetTime units; 3 means the patcher finished, 4 a failure.
// Unknown: the member types, the words at this + 0x153c0 to 0x153d8, and the unit of GetTime.

extern "C" {
int printf(const char* format, ...);
void* memset(void* to, int value, unsigned size);
int memcmp(const void* left, const void* right, unsigned size);
void SetRoot__15VIWindowManagerP5VIWnd(void* manager, void* window);
void SetFocus__5VIWndP5VIWndb(void* manager, void* window, bool flag);
int MainLoopBegin__7VISetup(void* self);
int MainLoopEnd__7VISetup(void* self);
void VIDnasInit__Fv();
void VIDnasAuthInstall__Fv();
int VIDnasGetStatus__FP14sceDNAS2Status(void* status);
void VIDnasShutdown__Fv();
long long GetTime__7VITimer();
int GetThreadId();
int SetAlarm(unsigned short ticks, void (*callback)(int, unsigned short, void*), void* argument);
int ReleaseAlarm(int alarm);
int SleepThread();
void sAlarmCallback__FiUsPv(int id, unsigned short time, void* argument);
void Begin__7VIFaderff(void* fader, float from, float to);
extern long long sLastTickTime;
}

namespace {
struct Status {
    int word[4];
};

void run_frame(void* self) {
    MainLoopBegin__7VISetup(self);
    MainLoopEnd__7VISetup(self);
}

// MainLoopBegin runs first; the value tested is -1 when MainLoopEnd returns a negative value and
// otherwise the value MainLoopBegin returned.
int step_result(void* self) {
    const int begun = MainLoopBegin__7VISetup(self);
    return MainLoopEnd__7VISetup(self) < 0 ? -1 : begun;
}

void copy_to_window(unsigned char* base, const Status& status) {
    *reinterpret_cast<Status*>(base + 0x124e0) = status;
}
}

extern "C" int RunPatcher__7VISetup(void* self) {
    unsigned char* const base = static_cast<unsigned char*>(self);
    void* const manager = base + 0xaf64;
    void* const window = base + 0x12060;
    Status status;
    sLastTickTime = 0;
    *reinterpret_cast<int*>(base + 0x153d8) = 0;
    *reinterpret_cast<int*>(base + 0x153dc) = 0;
    *reinterpret_cast<int*>(base + 0x153c0) = 0;
    *reinterpret_cast<int*>(base + 0x153d0) = 0;
    *reinterpret_cast<int*>(base + 0x153d4) = 0;
    int went_wrong;
    for (;;) {
        int retry = 0;
        memset(&status, 0, sizeof status);
        went_wrong = 0;
        SetRoot__15VIWindowManagerP5VIWnd(manager, window);
        SetFocus__5VIWndP5VIWndb(manager, window, true);
        copy_to_window(base, status);
        for (int i = 0; i < 20; ++i) {
            run_frame(self);
        }
        VIDnasInit__Fv();
        while (VIDnasGetStatus__FP14sceDNAS2Status(&status) != 0) {
            copy_to_window(base, status);
            if (step_result(self) < 0) {
                VIDnasShutdown__Fv();
                return 4;
            }
        }
        VIDnasAuthInstall__Fv();
        Status previous;
        memset(&previous, 0, sizeof previous);
        long long started = GetTime__7VITimer();
        for (;;) {
            if (VIDnasGetStatus__FP14sceDNAS2Status(&status) != 0) {
                went_wrong = 1;
                printf("error getting status!\n");
                break;
            }
            const int alarm = SetAlarm(250, sAlarmCallback__FiUsPv, reinterpret_cast<void*>(GetThreadId()));
            SleepThread();
            ReleaseAlarm(alarm);
            run_frame(self);
            if (memcmp(&previous, &status, 16) != 0) {
                previous = status;
                started = GetTime__7VITimer();
                if (status.word[0] == 6 && (status.word[1] == -105 || status.word[1] == -101)) {
                    went_wrong = 1;
                    copy_to_window(base, status);
                    run_frame(self);
                    run_frame(self);
                    printf("server timeout\n");
                    break;
                }
                copy_to_window(base, status);
                run_frame(self);
                run_frame(self);
                const int code = status.word[0];
                if (code == 5 || code == 8) {
                    printf("finished successfully\n");
                    break;
                }
                if (code == 6 || code == 7) {
                    went_wrong = 1;
                    printf("error during process\n");
                    break;
                }
                printf("------------------\n");
                printf("code     : %d\n", status.word[0]);
                printf("sub-code : %d\n", status.word[1]);
                printf("progress : %d\n", status.word[2]);
            }
            if (static_cast<int>(GetTime__7VITimer() - started) > 60000) {
                retry = 1;
                printf("FORCED TIMEOUT!\n");
                went_wrong = 0;
                break;
            }
        }
        VIDnasShutdown__Fv();
        if (retry == 0) {
            break;
        }
    }
    if (went_wrong != 0) {
        while (step_result(self) > 0) {
        }
        return 4;
    }
    for (int i = 0; i < 60; ++i) {
        run_frame(self);
    }
    void* const fader = base + 0xae38;
    Begin__7VIFaderff(fader, 1.0f, 0.5f);
    while (*reinterpret_cast<float*>(base + 0xae38 + 8) != 1.0f) {
        if (step_result(self) < 0) {
            return 4;
        }
    }
    return 3;
}
