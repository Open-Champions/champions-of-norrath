// Complete pinned _InitSys orchestration at 0x002dd8e8. Individual services
// remain source dependencies; their kernel/hardware effects are not fabricated.
extern "C" {
void supplement_crt0();
void InitSystemCallTableAddress();
void InitAlarm();
void InitTimer(int);
void StartTimerSystemTime();
void InitThread();
void InitExecPS2();
void InitTLBFunctions();
void sceSifInitRebootNotify();
void _InitSys() {
    supplement_crt0();
    InitSystemCallTableAddress();
    InitAlarm();
    InitTimer(2);
    StartTimerSystemTime();
    InitThread();
    InitExecPS2();
    InitTLBFunctions();
    sceSifInitRebootNotify();
}
}
