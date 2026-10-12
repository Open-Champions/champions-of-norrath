/* Observed SetNextComp at 0x002de290, 400 bytes. The argument is a 64-bit
 * system time. The function returns at once when the word at g_Timer offset 28
 * is nonnegative. It then reads the word at 0x10001010 once and looks at the
 * alarm chain whose first element is the word at g_Timer offset 24.
 * An alarm's time is its 64-bit word at offset 32, plus its word at 16, minus
 * its word at 24. With an empty chain it calls SetT2_COMP(0) and then
 * SetT2_MODE with bit 0x800 of the mode word cleared.
 * Otherwise it starts from the first alarm's time and walks the chain: while
 * the next alarm's time is below the current time plus 29491 (unsigned), that
 * alarm's time becomes the current time. The walk stops at the first alarm
 * outside that window or at the end of the chain.
 * If the final time minus the argument, as a signed 64-bit value, is below
 * 29491, it reads the word at 0x10001000 and calls SetT2_COMP with that word
 * plus the 32-bit value of 29491 shifted right by four times the low two mode
 * bits, then SetT2_MODE. Otherwise it calls SetT2_MODE first and then
 * SetT2_COMP with the low 32 bits of the final time shifted right by four
 * times the low two mode bits. Both mode calls clear 0x800.
 * Inferred, from the public EE timer map: 0x10001000, 0x10001010 are the
 * timer-2 count and mode registers; SetT2_COMP writes the compare register.
 * Unknown: what the word at g_Timer offset 28 means beyond "nonnegative
 * returns" (cbTimerHandler stores an alarm identifier there while it runs
 * callbacks and -1 afterwards), and the meaning of the alarm words.
 */
#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned norrath_host_timer2_count();
extern "C" unsigned norrath_host_timer2_mode();
extern "C" unsigned char* norrath_host_arena;
#define NORRATH_TIMER2_COUNT() (norrath_host_timer2_count())
#define NORRATH_TIMER2_MODE() (norrath_host_timer2_mode())
#define NORRATH_ALARM(word) (reinterpret_cast<Alarm*>(norrath_host_arena + static_cast<unsigned>(word)))
#else
#define NORRATH_TIMER2_COUNT() (*(volatile unsigned*)0x10001000u)
#define NORRATH_TIMER2_MODE() (*(volatile unsigned*)0x10001010u)
#define NORRATH_ALARM(word) (reinterpret_cast<Alarm*>(static_cast<__INTPTR_TYPE__>(word)))
#endif
extern "C" {
struct Alarm {
    int next;
    int previous;
    unsigned word_8;
    unsigned word_12;
    unsigned long long word_16;
    unsigned long long word_24;
    unsigned long long word_32;
    int word_40;
    int word_44;
    int word_48;
};
struct TimerWords {
    unsigned long long head;
    int handler;
    int untouched;
    int field_16;
    unsigned counter;
    int first_alarm;
    int field_28;
};
extern TimerWords g_Timer;
void SetT2_COMP(unsigned value);
void SetT2_MODE(unsigned mode);
static inline unsigned long long alarm_time(const Alarm* alarm) {
    return alarm->word_32 + alarm->word_16 - alarm->word_24;
}
void SetNextComp(unsigned long long sample) {
    if (g_Timer.field_28 >= 0) return;
    const Alarm* first = NORRATH_ALARM(g_Timer.first_alarm);
    const unsigned mode = NORRATH_TIMER2_MODE();
    if (g_Timer.first_alarm == 0) {
        SetT2_COMP(0);
        SetT2_MODE(mode & 0xfffff7ffu);
        return;
    }
    unsigned long long latest = alarm_time(first);
    for (int next = first->next; next != 0; next = NORRATH_ALARM(next)->next) {
        const unsigned long long candidate = alarm_time(NORRATH_ALARM(next));
        if (!(candidate < latest + 29491ull)) break;
        latest = candidate;
    }
    const unsigned shift = (mode & 3u) * 4u;
    if (static_cast<long long>(latest - sample) < 29491) {
        const unsigned count = NORRATH_TIMER2_COUNT();
        SetT2_COMP(count + static_cast<int>(29491ull >> shift));
        SetT2_MODE(mode & 0xfffff7ffu);
    } else {
        SetT2_MODE(mode & 0xfffff7ffu);
        SetT2_COMP(static_cast<int>(latest >> shift));
    }
}
}
