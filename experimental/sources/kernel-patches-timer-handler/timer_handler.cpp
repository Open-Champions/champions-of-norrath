/* Observed cbTimerHandler at 0x002de4e0, 672 bytes. InitTimer registers it
 * with AddIntcHandler2(11, ...). Its third incoming argument is kept and
 * passed on to every alarm callback as the fifth argument. It returns 0.
 * When bit 0x400 of the word at 0x10001010 is clear, or the chain whose first
 * element is the word at g_Timer offset 24 is empty, it skips to the tail.
 * Otherwise each pass reads the word at 0x10001000, then the word at
 * 0x10001010, and takes the first alarm's time (its word at 32, plus the word
 * at 16, minus the word at 24). When bit 0x800 of the mode word is set it
 * adds 1 to the 64-bit word at the start of g_Timer, calls SetT2_MODE with
 * bit 0x400 cleared, and reads the count word again. The current time is
 * ((g_Timer word << 16) | count) shifted left by four times the low two mode
 * bits. The pass ends the whole loop when that time is below the alarm's time.
 * Otherwise it unlinks the alarm with UnlinkAlarm, stores
 * (alarm address << 4) | alarm word 8 at g_Timer offset 28, loads the alarm's
 * word at offset 44 into the gp register, and calls the function at offset 40
 * with: that identifier; the alarm's 64-bit word at 32; the current time plus
 * word 24 minus word 16; the alarm's word at 48; and the third incoming
 * argument. The 64-bit result decides the alarm's fate:
 *   0: clear bit 2 of the alarm's word at offset 12;
 *   all ones: push the alarm on the chain at g_Timer offset 20, clear its
 *     words at 8 and 12;
 *   otherwise: add the result, but at least 14745, to the alarm's word at 32
 *     and re-insert it with InsertAlarm.
 * The loop continues with the alarm that UnlinkAlarm returned.
 * Tail: store -1 at g_Timer offset 28. Read count and mode again, handle
 * the overflow bit as above, call SetNextComp with the current time, read
 * the count word, read the mode word, handle the overflow bit once more,
 * execute sync and ei, and return 0.
 * gp is not restored. Whether the kernel's interrupt return restores it is
 * unknown.
 * Inferred, from the public EE timer map: 0x10001000 and 0x10001010 are the
 * timer-2 count and mode registers, 0x400 is the compare flag, 0x800 the
 * overflow flag.
 * Unknown: the meaning of the alarm words, and the interrupt-context rules.
 */
#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned norrath_host_timer2_count();
extern "C" unsigned norrath_host_timer2_mode();
extern "C" unsigned char* norrath_host_arena;
extern "C" void norrath_host_set_gp(int value);
struct Alarm;
typedef unsigned long long (*AlarmCallback)(unsigned identifier, unsigned long long value,
                                            unsigned long long elapsed, int argument, int third);
extern "C" AlarmCallback norrath_host_callback(unsigned word);
extern "C" void norrath_host_sync_and_enable();
#define NORRATH_TIMER2_COUNT() (norrath_host_timer2_count())
#define NORRATH_TIMER2_MODE() (norrath_host_timer2_mode())
#define NORRATH_ALARM(word) (static_cast<unsigned>(word) == 0 ? nullptr : reinterpret_cast<Alarm*>(norrath_host_arena + static_cast<unsigned>(word)))
#define NORRATH_ALARM_WORD(alarm) (static_cast<unsigned>(reinterpret_cast<unsigned char*>(alarm) - norrath_host_arena))
#define NORRATH_CALLBACK(word) (norrath_host_callback(word))
#define NORRATH_SET_GP(value) (norrath_host_set_gp(value))
#define NORRATH_SYNC_AND_ENABLE() (norrath_host_sync_and_enable())
#else
#define NORRATH_TIMER2_COUNT() (*(volatile unsigned*)0x10001000u)
#define NORRATH_TIMER2_MODE() (*(volatile unsigned*)0x10001010u)
#define NORRATH_ALARM(word) (reinterpret_cast<Alarm*>(static_cast<__INTPTR_TYPE__>(word)))
#define NORRATH_ALARM_WORD(alarm) (static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(alarm)))
static inline void norrath_set_gp(int value) {
    asm volatile("move $28, %0" : : "r"(value) : "memory");
}
static inline void norrath_sync_and_enable() {
    asm volatile("sync\n\tei" : : : "memory");
}
#define NORRATH_CALLBACK(word) (reinterpret_cast<AlarmCallback>(static_cast<__UINTPTR_TYPE__>(word)))
#define NORRATH_SET_GP(value) (norrath_set_gp(value))
#define NORRATH_SYNC_AND_ENABLE() (norrath_sync_and_enable())
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
    unsigned word_40;
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
#if !defined(NORRATH_HOST_CONTRACT)
typedef unsigned long long (*AlarmCallback)(unsigned identifier, unsigned long long value,
                                            unsigned long long elapsed, int argument, int third);
#endif
Alarm* UnlinkAlarm(Alarm* alarm);
void InsertAlarm(Alarm* alarm);
void SetNextComp(unsigned long long sample);
void SetT2_MODE(unsigned mode);
static inline unsigned long long alarm_time(const Alarm* alarm) {
    return alarm->word_32 + alarm->word_16 - alarm->word_24;
}
static inline unsigned long long system_time(unsigned count, unsigned mode) {
    return (((g_Timer.head << 16) | count) << ((mode & 3u) * 4u));
}
int cbTimerHandler(int first, int second, int third) {
    (void)first;
    (void)second;
    Alarm* alarm = nullptr;
    if ((NORRATH_TIMER2_MODE() & 0x400u) != 0)
        alarm = NORRATH_ALARM(g_Timer.first_alarm);
    while (alarm != nullptr) {
        unsigned count = NORRATH_TIMER2_COUNT();
        const unsigned mode = NORRATH_TIMER2_MODE();
        const unsigned long long due = alarm_time(alarm);
        if ((mode & 0x800u) != 0) {
            g_Timer.head += 1;
            SetT2_MODE(mode & 0xfffffbffu);
            count = NORRATH_TIMER2_COUNT();
        }
        const unsigned long long now = system_time(count, mode);
        if (now < due) break;
        Alarm* following = UnlinkAlarm(alarm);
        const unsigned long long due_word = alarm->word_24;
        const unsigned long long start_word = alarm->word_16;
        g_Timer.field_28 = static_cast<int>((NORRATH_ALARM_WORD(alarm) << 4) | alarm->word_8);
        const unsigned long long elapsed = now + due_word - start_word;
        NORRATH_SET_GP(alarm->word_44);
        const unsigned long long result = NORRATH_CALLBACK(alarm->word_40)(
            (NORRATH_ALARM_WORD(alarm) << 4) | alarm->word_8, alarm->word_32, elapsed, alarm->word_48, third);
        if (result == 0) {
            alarm->word_12 &= 0xfffffffdu;
        } else if (result == ~0ull) {
            alarm->next = static_cast<int>(g_Timer.counter);
            g_Timer.counter = NORRATH_ALARM_WORD(alarm);
            alarm->word_8 = 0;
            alarm->word_12 = 0;
        } else {
            const unsigned long long step = result > 14744ull ? result : 14745ull;
            alarm->word_32 += step;
            InsertAlarm(alarm);
        }
        alarm = following;
    }
    g_Timer.field_28 = -1;
    unsigned count = NORRATH_TIMER2_COUNT();
    unsigned mode = NORRATH_TIMER2_MODE();
    if ((mode & 0x800u) != 0) {
        g_Timer.head += 1;
        SetT2_MODE(mode & 0xfffffbffu);
        count = NORRATH_TIMER2_COUNT();
    }
    SetNextComp(system_time(count, mode));
    (void)NORRATH_TIMER2_COUNT();
    mode = NORRATH_TIMER2_MODE();
    if ((mode & 0x800u) != 0) {
        g_Timer.head += 1;
        SetT2_MODE(mode & 0xfffffbffu);
        (void)NORRATH_TIMER2_COUNT();
    }
    NORRATH_SYNC_AND_ENABLE();
    return 0;
}
}
