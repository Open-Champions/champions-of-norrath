/* Observed InsertAlarm at 0x002de420, 136 bytes. It links the alarm in a0
 * into the chain whose first element is the word at g_Timer offset 24. An
 * alarm's time is its 64-bit word at offset 32, plus its word at 16, minus its
 * word at 24. The walk starts at the first element and moves to the next
 * word at offset 0 until it meets an alarm whose time is strictly greater
 * than the new alarm's time (unsigned), or the end. An alarm with an equal
 * time is passed, so the new alarm goes after it.
 * The new alarm's previous word (offset 4) takes the last alarm passed (0 if
 * none) and its next word (offset 0) takes the alarm that stopped the walk
 * (0 if none). When that alarm exists its previous word takes the new alarm.
 * When an alarm was passed its next word takes the new alarm; otherwise
 * g_Timer offset 24 does. Nothing is returned.
 * Inferred: the chain is ordered by ascending time. UnlinkAlarm is the
 * inverse for one alarm.
 * Unknown: none for the stores above.
 */
#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned char* norrath_host_arena;
#define NORRATH_ALARM(word) (static_cast<unsigned>(word) == 0 ? nullptr : reinterpret_cast<Alarm*>(norrath_host_arena + static_cast<unsigned>(word)))
#define NORRATH_ALARM_WORD(alarm) (static_cast<unsigned>(reinterpret_cast<unsigned char*>(alarm) - norrath_host_arena))
#else
#define NORRATH_ALARM(word) (reinterpret_cast<Alarm*>(static_cast<__INTPTR_TYPE__>(word)))
#define NORRATH_ALARM_WORD(alarm) (static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(alarm)))
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
static inline unsigned long long alarm_time(const Alarm* alarm) {
    return alarm->word_32 + alarm->word_16 - alarm->word_24;
}
void InsertAlarm(Alarm* alarm) {
    const unsigned long long time = alarm_time(alarm);
    Alarm* passed = nullptr;
    Alarm* stopped = NORRATH_ALARM(g_Timer.first_alarm);
    while (stopped != nullptr && !(time < alarm_time(stopped))) {
        passed = stopped;
        stopped = NORRATH_ALARM(passed->next);
    }
    alarm->previous = passed != nullptr ? static_cast<int>(NORRATH_ALARM_WORD(passed)) : 0;
    alarm->next = stopped != nullptr ? static_cast<int>(NORRATH_ALARM_WORD(stopped)) : 0;
    if (stopped != nullptr)
        stopped->previous = static_cast<int>(NORRATH_ALARM_WORD(alarm));
    if (passed != nullptr)
        passed->next = static_cast<int>(NORRATH_ALARM_WORD(alarm));
    else
        g_Timer.first_alarm = static_cast<int>(NORRATH_ALARM_WORD(alarm));
}
}
