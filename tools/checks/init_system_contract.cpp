#include <cstdio>
static unsigned trace[9], count;
extern "C" {
void _InitSys();
void supplement_crt0() { trace[count++] = 1; }
void InitSystemCallTableAddress() { trace[count++] = 2; }
void InitAlarm() { trace[count++] = 3; }
void InitTimer(int timer) { if (timer != 2) __builtin_trap(); trace[count++] = 4; }
void StartTimerSystemTime() { trace[count++] = 5; }
void InitThread() { trace[count++] = 6; }
void InitExecPS2() { trace[count++] = 7; }
void InitTLBFunctions() { trace[count++] = 8; }
void sceSifInitRebootNotify() { trace[count++] = 9; }
}
int main() {
    _InitSys();
    if (count != 9) return 1;
    for (unsigned i = 0; i < count; ++i) if (trace[i] != i + 1) return 2;
    std::puts("system initialization call order and timer argument passed");
}
