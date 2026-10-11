// Observed InitTimer at 0x002ddf40. The caller passes 2. A nonnegative word
// at g_Timer + 8 returns 0x80008001 before any other store.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned norrath_host_timer2_mode;
#define NORRATH_TIMER2_MODE (norrath_host_timer2_mode)
#else
#define NORRATH_TIMER2_MODE (*(volatile unsigned*)0x10001010u)
#endif
extern "C" {
struct TimerWords {
    unsigned long long head;
    int handler;
    int untouched;
    int field_16;
    unsigned counter;
};
extern TimerWords g_Timer;
extern unsigned char g_CounterBuf[];
void cbTimerHandler();
void* memset(void* destination, int value, __SIZE_TYPE__ count);
void _InitAlarm();
int AddIntcHandler2(int cause, void (*handler)(), int third, int fourth);
int DIntr();
void EIntr();
void SetT2_COUNT(unsigned value);
void SetT2_COMP(unsigned value);
void SetT2_MODE(unsigned mode);
void EnableIntc(int cause);
unsigned InitTimer(unsigned selector) {
    if (g_Timer.handler >= 0) return 0x80008001u;
    g_Timer.head = 0;
    g_Timer.field_16 = 0;
    memset(g_CounterBuf, 0, 8192);
    g_Timer.counter = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(g_CounterBuf));
    // 128 observed 32-bit stores, 64 bytes apart, from offset 8128 down to 0.
    // The stored values are the low 32 bits of those addresses. The word at
    // offset 8128 is then cleared before _InitAlarm.
    unsigned char* slot = g_CounterBuf + 8128;
    unsigned char* value = g_CounterBuf + 8192;
    for (int step = 127; step >= 0; --step) {
        *reinterpret_cast<unsigned*>(slot) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(value));
        slot -= 64;
        value -= 64;
        (void)step;
    }
    *reinterpret_cast<unsigned*>(g_CounterBuf + 8128) = 0;
    _InitAlarm();
    int handler = AddIntcHandler2(11, cbTimerHandler, 0, 0);
    if (handler < 0) return 0x80009021u;
    g_Timer.handler = handler;
    int previous = DIntr();
    unsigned mode = NORRATH_TIMER2_MODE;
    mode = (mode & 0xfffffffcu) | selector | 0x300u;
    if ((mode & 0x80u) == 0) {
        mode |= 0xc80u;
        SetT2_COUNT(0);
        SetT2_COMP(0xffffu);
    }
    SetT2_MODE(mode);
    EnableIntc(11);
    if (previous != 0) EIntr();
    return 0;
}
}
