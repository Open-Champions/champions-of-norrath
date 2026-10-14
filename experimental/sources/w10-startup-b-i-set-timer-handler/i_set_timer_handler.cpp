/* Observed iSetTimerHandler at 0x002deee8, 244 bytes. Arguments: a handle from
 * iAllocTimerCounter, a 64-bit value, a handler address and a handler argument.
 * The record is found from the handle as in iStartTimerCounter. In order:
 *   - a negative handle or one whose low ten bits differ from the record's word
 *     at +8 returns 0x80008002; a handle equal to the g_Timer word at offset 28
 *     returns 0x80000010;
 *   - when bit 1 of the record's +12 word is set, UnlinkAlarm(record) is called
 *     and the +12 word is read again;
 *   - the handler is stored at +40;
 *   - a zero handler clears bit 1 of the +12 word;
 *   - otherwise bit 1 is set, the 64-bit value goes to +32, the caller's gp
 *     register to +44, the handler argument to +48, and when bit 0 of the +12
 *     word (read back) is set InsertAlarm(record) is called;
 *   - in both of the last two cases SetNextComp(iGetTimerSystemTime()) is called
 *     and the result is 0.
 * Inferred: bit 1 means a handler is installed (see iStartTimerCounter); the gp
 * word lets the handler run with the installer's global pointer.
 * Unknown: the units of the 64-bit value. Compiled output is not claimed to match
 * the original.
 */
#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned char* norrath_host_arena;
#define NORRATH_COUNTER(word) (reinterpret_cast<Counter*>(norrath_host_arena + (word)))
extern "C" unsigned norrath_host_gp;
#define NORRATH_GP() (norrath_host_gp)
#else
#define NORRATH_COUNTER(word) (reinterpret_cast<Counter*>(static_cast<__INTPTR_TYPE__>(static_cast<int>(word))))
register unsigned norrath_gp_register asm("$28");
#define NORRATH_GP() (norrath_gp_register)
#endif
extern "C" {
struct Counter {
    unsigned next;
    unsigned unused_4;
    unsigned generation_word;
    unsigned flags;
    unsigned long long word_16;
    unsigned long long word_24;
    unsigned long long value;
    unsigned handler;
    unsigned handler_gp;
    unsigned handler_argument;
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
void UnlinkAlarm(Counter* alarm);
void SetNextComp(unsigned long long sample);

int iSetTimerHandler(int handle, unsigned long long value, unsigned handler, unsigned argument) {
    Counter* const counter = NORRATH_COUNTER((static_cast<unsigned>(handle) >> 10) << 6);
    if (handle < 0 || (static_cast<unsigned>(handle) & 0x3ffu) != counter->generation_word)
        return static_cast<int>(0x80008002u);
    if (g_Timer.running_handle == handle) return static_cast<int>(0x80000010u);
    if ((counter->flags & 2u) != 0) UnlinkAlarm(counter);
    unsigned flags = counter->flags;
    counter->handler = handler;
    if (handler == 0) {
        counter->flags = flags & ~2u;
    } else {
        counter->flags = flags | 2u;
        counter->value = value;
        counter->handler_gp = NORRATH_GP();
        const unsigned started = counter->flags & 1u;
        counter->handler_argument = argument;
        if (started != 0) InsertAlarm(counter);
    }
    SetNextComp(iGetTimerSystemTime());
    return 0;
}
}
