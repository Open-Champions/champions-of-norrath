/* Observed iGetTimerSystemTime at 0x002de780, 76 bytes. It reads the word at
 * 0x10001000, then the word at 0x10001010, then the 64-bit word at the start
 * of g_Timer. When bit 0x800 of the second word is set it reads the first word
 * again and adds 1 to its local copy of the 64-bit word; g_Timer is not
 * written. The result is ((g_Timer word << 16) | first word) shifted left by 0
 * when the low two bits of the second word are 0, otherwise by 2 shifted left
 * by those two bits (4, 8, or 16). It is returned as a full 64-bit value.
 * Inferred, from the public EE timer map: 0x10001000 and 0x10001010 are the
 * timer-2 count and mode registers, bit 0x800 is the overflow flag, and the
 * g_Timer word counts overflows.
 * Unknown: why the shift for low bits 3 is 16 here while SetNextComp and
 * cbTimerHandler shift by 12 for the same bits.
 */
#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned norrath_host_timer2_count();
extern "C" unsigned norrath_host_timer2_mode();
#define NORRATH_TIMER2_COUNT() (norrath_host_timer2_count())
#define NORRATH_TIMER2_MODE() (norrath_host_timer2_mode())
#else
#define NORRATH_TIMER2_COUNT() (*(volatile unsigned*)0x10001000u)
#define NORRATH_TIMER2_MODE() (*(volatile unsigned*)0x10001010u)
#endif
extern "C" {
struct TimerWords {
    unsigned long long head;
    int handler;
    int untouched;
    int field_16;
    unsigned counter;
    unsigned field_24;
    int field_28;
};
extern TimerWords g_Timer;
unsigned long long iGetTimerSystemTime() {
    unsigned count = NORRATH_TIMER2_COUNT();
    const unsigned mode = NORRATH_TIMER2_MODE();
    unsigned long long overflows = g_Timer.head;
    if ((mode & 0x800u) != 0) {
        count = NORRATH_TIMER2_COUNT();
        overflows += 1;
    }
    const unsigned divider = mode & 3u;
    const unsigned shift = divider != 0 ? (2u << divider) : 0u;
    return ((overflows << 16) | count) << shift;
}
}
