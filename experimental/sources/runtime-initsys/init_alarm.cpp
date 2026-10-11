// Observed InitAlarm at 0x002dddc8. Bit 0x100 of the word loaded from
// 0x10001810 skips the copy. That address is inferred as EE timer-3 mode
// from the public timer map; the load and the bit test are observed.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned norrath_host_timer3_mode;
#define NORRATH_TIMER3_MODE (norrath_host_timer3_mode)
#else
#define NORRATH_TIMER3_MODE (*(volatile unsigned*)0x10001810u)
#endif
extern "C" {
void kernel_alarm_setup(unsigned first, unsigned second);
void kernel_alarm_copy(unsigned destination, const void* source, unsigned bytes);
unsigned kernel_alarm_get_entry(unsigned identifier);
int FlushCache(int mode);
extern unsigned kernel_alarm_sysentry[];
extern unsigned char srcfile[];
extern unsigned char eenull[];
void InitAlarm() {
    if ((NORRATH_TIMER3_MODE & 0x100u) != 0) return;
    kernel_alarm_setup(kernel_alarm_sysentry[0], kernel_alarm_sysentry[1]);
    // Observed destinations and lengths. 1856 and 40 match the srcfile and
    // eenull symbol spans.
    kernel_alarm_copy(0x80076000u, srcfile, 1856);
    kernel_alarm_copy(0x00082000u, eenull, 40);
    FlushCache(0);
    FlushCache(2);
    kernel_alarm_setup(kernel_alarm_sysentry[2], kernel_alarm_sysentry[3]);
    unsigned* cursor = kernel_alarm_sysentry + 4;
    int gate = 2;
    do {
        unsigned identifier = *cursor;
        ++gate;
        unsigned entry = kernel_alarm_get_entry(identifier);
        kernel_alarm_setup(*cursor, entry);
        cursor += 2;
    } while (gate < 8);
}
}
