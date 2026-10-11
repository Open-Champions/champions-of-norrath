#include <cstdio>
extern "C" unsigned norrath_host_timer2_mode = 0;
static int previous, eintr, mode_calls;
static unsigned programmed, sample = 99;
extern "C" int DIntr() { return previous; }
extern "C" void EIntr() { ++eintr; }
extern "C" void SetT2_MODE(unsigned mode) { ++mode_calls; programmed = mode; }
extern "C" unsigned iGetTimerSystemTime() { return sample; }
extern "C" void SetNextComp(unsigned value) { if (value != sample) __builtin_trap(); }
extern "C" int StartTimerSystemTime();
int main() {
    previous = 1;
    norrath_host_timer2_mode = 0x80;
    if (StartTimerSystemTime() != 1 || eintr != 1 || mode_calls != 0) return 1;
    eintr = 0;
    previous = 0;
    if (StartTimerSystemTime() != 1 || eintr != 0) return 2;
    norrath_host_timer2_mode = 0x00000c00;
    previous = 1;
    if (StartTimerSystemTime() != 0 || programmed != ((0x00000c00u & 0xfffff3ffu) | 0x80u) || eintr != 1) return 3;
    std::puts("timer system-time start passed");
}
