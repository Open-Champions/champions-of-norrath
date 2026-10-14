// Observed Init__7VITimer at 0x011344f8, 124 bytes. It returns.
// _7VITimer$SystemTime at 0x01228a48 is cleared before DisableIntc(9).
// _7VITimer$TimerMode at 0x01228a50 receives 1473. The same 1473 is stored
// at 0x10000010 and 18750 is stored at 0x10000020. AddIntcHandler receives
// cause 9, Handler__7VITimeri, and -1. Its result is stored in
// _7VITimer$HandlerID at 0x01228a40 before EnableIntc(9).
// _7VITimer$Ready at 0x01228a54 then receives 1.
// What the two absolute stores program is unknown.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_host_store32(unsigned address, unsigned value);
#define NORRATH_STORE32(address, value) norrath_host_store32((address), (value))
#else
#define NORRATH_STORE32(address, value)                                    \
    do {                                                                   \
        *reinterpret_cast<volatile unsigned*>(address) = (value);         \
    } while (0)
#endif

extern "C" {
int norrath_vi_timer_handler_id __asm__("_7VITimer$HandlerID");
unsigned long long norrath_vi_timer_system_time __asm__("_7VITimer$SystemTime");
int norrath_vi_timer_mode __asm__("_7VITimer$TimerMode");
int norrath_vi_timer_ready __asm__("_7VITimer$Ready");

void DisableIntc(int cause);
int AddIntcHandler(int cause, void (*handler)(int), int next);
void EnableIntc(int cause);
void Handler__7VITimeri(int cause);

void Init__7VITimer() {
    norrath_vi_timer_system_time = 0;
    DisableIntc(9);
    norrath_vi_timer_mode = 1473;
    NORRATH_STORE32(0x10000010u, 1473u);
    NORRATH_STORE32(0x10000020u, 18750u);
    norrath_vi_timer_handler_id = AddIntcHandler(9, Handler__7VITimeri, -1);
    EnableIntc(9);
    norrath_vi_timer_ready = 1;
}
}
