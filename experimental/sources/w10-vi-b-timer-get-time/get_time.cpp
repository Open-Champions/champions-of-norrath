// Observed GetTime__7VITimer at 0x011345f0, 12 bytes. It returns the 64-bit
// word _7VITimer$SystemTime (0x01228a48), the same word Init__7VITimer clears.
// Which unit advances it is not established here.
extern "C" {
extern unsigned long long norrath_vi_timer_system_time __asm__("_7VITimer$SystemTime");

long long GetTime__7VITimer() {
    return static_cast<long long>(norrath_vi_timer_system_time);
}
}
