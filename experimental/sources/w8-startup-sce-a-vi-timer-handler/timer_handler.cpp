/* Observed Handler__7VITimeri at 0x011345b8, 56 bytes, in the 0x01000000
 * module image. It ignores its argument. The 64-bit word _7VITimer$SystemTime
 * (0x01228a48) is increased by 2. The word _7VITimer$TimerMode (0x01228a50),
 * read before that addition, is stored to the 32-bit location 0x10000010 (the
 * same location Init__7VITimer writes 1473 to). Then a sync and an ei
 * instruction run and the result is 0. What the location 0x10000010 is, and
 * why the time advances by 2, are unknown. Compiled output is not claimed to
 * match the original.
 */
#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_host_store32(unsigned address, unsigned value);
extern "C" void norrath_host_sync();
extern "C" void norrath_host_ei();
#define NORRATH_STORE32(address, value) norrath_host_store32((address), (value))
#define NORRATH_SYNC() norrath_host_sync()
#define NORRATH_EI() norrath_host_ei()
#else
#define NORRATH_STORE32(address, value)                             \
    do {                                                            \
        *reinterpret_cast<volatile unsigned*>(address) = (value);  \
    } while (0)
#define NORRATH_SYNC() asm volatile("sync" ::: "memory")
#define NORRATH_EI() asm volatile("ei" ::: "memory")
#endif

extern "C" {
unsigned long long norrath_vi_timer_system_time __asm__("_7VITimer$SystemTime");
int norrath_vi_timer_mode __asm__("_7VITimer$TimerMode");

int Handler__7VITimeri(int) {
    const int mode = norrath_vi_timer_mode;
    norrath_vi_timer_system_time += 2;
    NORRATH_STORE32(0x10000010u, static_cast<unsigned>(mode));
    NORRATH_SYNC();
    NORRATH_EI();
    return 0;
}
}
