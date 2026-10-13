// Observed iSetTimerAlarm at 0x002df3e8, 240 bytes. Arguments: a time value, a
// nonzero callback word, and a parameter word. In order:
//   - a zero callback word returns 0x80000016;
//   - the head of the free alarm list (g_pFreeAlarm, 0x008e4540) is taken and
//     the head moved to that record's first word; an empty list returns
//     0x80008005;
//   - iAllocTimerCounter (0x002de850) supplies a counter id. If it is negative
//     the record goes back on the free list (its first word set to the old
//     head, its word at +4 cleared) and the negative id is returned;
//   - otherwise the record gets the callback word at +8, the parameter at +12
//     and the counter id at +4; iSetTimerHandler (0x002deee8) is called with
//     (id, time, AlarmHandler at 0x002df258, record); iStartTimerCounter
//     (0x002deaa8) is called with the id; the result is the record address
//     shifted left 4, ORed with (id & 0xfe) and with 1.
// A record is read here as four words: next, counter id, callback, parameter.
// Unknown: the units of the time value, what a handle's low bits are used for
// by callers, the bodies of the three timer calls and of AlarmHandler.
extern "C" {
struct AlarmRecord {
    AlarmRecord* next;
    int counter;
    unsigned callback;
    unsigned parameter;
};

extern AlarmRecord* g_pFreeAlarm;
int iAllocTimerCounter();
int iSetTimerHandler(int counter, unsigned time, void (*handler)(), void* argument);
int iStartTimerCounter(int counter);
void AlarmHandler();

unsigned iSetTimerAlarm(unsigned time, unsigned callback, unsigned parameter) {
    if (callback == 0u) return 0x80000016u;
    AlarmRecord* const record = g_pFreeAlarm;
    if (record != 0) g_pFreeAlarm = record->next;
    if (record == 0) return 0x80008005u;
    const int counter = iAllocTimerCounter();
    if (counter < 0) {
        record->next = g_pFreeAlarm;
        g_pFreeAlarm = record;
        record->counter = 0;
        return static_cast<unsigned>(counter);
    }
    record->callback = callback;
    record->parameter = parameter;
    record->counter = counter;
    iSetTimerHandler(counter, time, AlarmHandler, record);
    iStartTimerCounter(counter);
    return (static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(record)) << 4) |
           (static_cast<unsigned>(counter) & 0xfeu) | 1u;
}
}
