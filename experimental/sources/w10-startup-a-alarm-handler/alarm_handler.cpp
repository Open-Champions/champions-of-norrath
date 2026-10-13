// Observed AlarmHandler at 0x002df258, 92 bytes (a local symbol in the image; it must be global
// here because iSetTimerAlarm passes its address to iSetTimerHandler). Its fourth argument is the
// alarm record that iSetTimerAlarm built (four words: next, counter id, callback, parameter).
// It calls the record's callback (word at +8) with the first argument masked to 0xfe, ORed with
// the record address shifted left 4 and ORed with 1 (the handle iSetTimerAlarm returned), the
// second and third arguments passed on unchanged, and the record's parameter (word at +12) as
// the fourth. A nonzero callback result is returned as it is. A zero result puts the record back
// on the head of the free list (g_pFreeAlarm, 0x008e4540): the record's first word takes the old
// head, the head takes the record, the counter word at +4 is cleared, and the function returns -1.
// Inferred only: a zero callback result meaning "done, release the alarm". Unknown: what calls
// this with the second and third arguments. Nothing was executed on the EE.
extern "C" {
struct AlarmRecord {
    AlarmRecord* next;
    int counter;
    unsigned callback;
    unsigned parameter;
};

extern AlarmRecord* g_pFreeAlarm;

#ifndef NORRATH_HOST_CHECK
static inline int call_callback(unsigned word, unsigned handle, int arg1, int arg2, unsigned parameter) {
    return reinterpret_cast<int (*)(unsigned, int, int, unsigned)>(word)(handle, arg1, arg2, parameter);
}
#endif

int AlarmHandler(unsigned id, int arg1, int arg2, AlarmRecord* record) {
    unsigned handle = (static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(record)) << 4) | (id & 0xfeu) | 1u;
    int result = call_callback(record->callback, handle, arg1, arg2, record->parameter);
    if (result != 0) return result;
    record->next = g_pFreeAlarm;
    g_pFreeAlarm = record;
    record->counter = 0;
    return -1;
}
}
