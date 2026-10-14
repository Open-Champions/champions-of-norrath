/* Observed iStartTimerCounter at 0x002deaa8, 172 bytes. The argument is a handle
 * from iAllocTimerCounter. The counter record address is the handle shifted right
 * 10 (logical, 32-bit) and left 6. In order:
 *   - a negative handle, or one whose low ten bits differ from the record's word
 *     at +8, returns 0x80008002;
 *   - a handle equal to the g_Timer word at offset 28 returns 0x80000010;
 *   - a record whose word at +12 has bit 0 set returns 1 with no stores;
 *   - otherwise iGetTimerSystemTime supplies a 64-bit time that is stored at
 *     +16, bit 0 of the +12 word is set (read after the call), and when bit 1 of
 *     that word is set InsertAlarm(record) and SetNextComp(time) are called.
 *     The result is 0.
 * Inferred: bit 0 of the +12 word means the counter is started, bit 1 means a
 * handler is installed (iSetTimerHandler sets it); the offset-28 word is the
 * handle whose handler is currently running (cbTimerHandler writes it).
 * Unknown: the units of the time value. Compiled output is not claimed to match
 * the original.
 */
#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned char* norrath_host_arena;
#define NORRATH_COUNTER(word) (reinterpret_cast<Counter*>(norrath_host_arena + (word)))
#else
#define NORRATH_COUNTER(word) (reinterpret_cast<Counter*>(static_cast<__INTPTR_TYPE__>(static_cast<int>(word))))
#endif
extern "C" {
struct Counter {
    unsigned next;
    unsigned unused_4;
    unsigned generation_word;
    unsigned flags;
    unsigned long long start_time;
};
struct TimerWords {
    unsigned long long head;
    int handler;
    unsigned generation;
    unsigned allocated;
    unsigned free_counters;
    unsigned first_alarm;
    int running_handle;
};
extern TimerWords g_Timer;
unsigned long long iGetTimerSystemTime();
void InsertAlarm(Counter* alarm);
void SetNextComp(unsigned long long sample);

int iStartTimerCounter(int handle) {
    Counter* const counter = NORRATH_COUNTER((static_cast<unsigned>(handle) >> 10) << 6);
    if (handle < 0 || (static_cast<unsigned>(handle) & 0x3ffu) != counter->generation_word)
        return static_cast<int>(0x80008002u);
    if (g_Timer.running_handle == handle) return static_cast<int>(0x80000010u);
    if ((counter->flags & 1u) != 0) return 1;
    const unsigned long long now = iGetTimerSystemTime();
    counter->start_time = now;
    const unsigned flags = counter->flags | 1u;
    counter->flags = flags;
    if ((flags & 2u) != 0) {
        InsertAlarm(counter);
        SetNextComp(now);
    }
    return 0;
}
}
