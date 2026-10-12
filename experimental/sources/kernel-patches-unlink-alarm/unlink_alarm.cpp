/* Observed UnlinkAlarm at 0x002de4a8, 56 bytes. It removes the alarm in a0
 * from the chain whose first element is the word at g_Timer offset 24. Each
 * alarm carries a next word at offset 0 and a previous word at offset 4.
 * When the previous word is nonzero it receives the alarm's next word at
 * offset 0; otherwise g_Timer offset 24 receives it. When the next word is
 * nonzero, that alarm's previous word receives this alarm's previous word.
 * The alarm's previous word is then cleared; its next word is left as it
 * was. The old next word is returned.
 * Inferred: a doubly linked chain, ordered by InsertAlarm.
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
Alarm* UnlinkAlarm(Alarm* alarm) {
    Alarm* previous = NORRATH_ALARM(alarm->previous);
    Alarm* following = NORRATH_ALARM(alarm->next);
    if (previous != nullptr)
        previous->next = alarm->next;
    else
        g_Timer.first_alarm = alarm->next;
    if (following != nullptr)
        following->previous = alarm->previous;
    alarm->previous = 0;
    return following;
}
}
