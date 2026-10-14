// Observed StartTimerSystemTime at 0x002de180.
// The mode word is the absolute load at 0x10001010. Identifying that address
// as the EE timer-2 mode register is inferred from the public timer map.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned norrath_host_timer2_mode;
#define NORRATH_TIMER2_MODE (norrath_host_timer2_mode)
#else
#define NORRATH_TIMER2_MODE (*(volatile unsigned*)0x10001010u)
#endif
extern "C" {
int DIntr();
void EIntr();
void SetT2_MODE(unsigned mode);
unsigned iGetTimerSystemTime();
void SetNextComp(unsigned sample);
int StartTimerSystemTime() {
    int previous = DIntr();
    unsigned mode = NORRATH_TIMER2_MODE;
    if ((mode & 0x80u) != 0) {
        if (previous != 0) EIntr();
        return 1;
    }
    SetT2_MODE((mode & 0xfffff3ffu) | 0x80u);
    SetNextComp(iGetTimerSystemTime());
    if (previous != 0) EIntr();
    return 0;
}
}
