/* Observed iAllocTimerCounter at 0x002de850, 112 bytes. g_Timer (0x004459a8) word
 * at offset 20 heads a chain of 64-byte counter records; the first word of each
 * record is the address of the next. In order:
 *   - an empty chain returns 0x80008005;
 *   - otherwise the g_Timer word at offset 16 is incremented, the record's word at
 *     +40 and word at +12 are cleared, the chain head moves to the record's first
 *     word, the g_Timer word at offset 12 is incremented, the 64-bit word at +24
 *     is cleared, and the record's word at +8 is set to
 *     ((offset-12 word << 1) & 0x3ff) with bit 0 forced on;
 *   - the result is the record address shifted left 4, ORed with the word at +8.
 * Inferred: iStartTimerCounter and iSetTimerHandler accept this result as a
 * handle (they check the low ten bits against the word at +8); the offset-12
 * word is a generation number, so the other g_Timer notes that call it untouched
 * are incomplete (it is written here); the offset-16 word counts allocations.
 * Unknown: what the counter and generation numbers are used for beyond handle
 * checking. Compiled output is not claimed to match the original.
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
    unsigned long long word_16;
    unsigned long long word_24;
    unsigned word_32;
    unsigned word_36;
    unsigned handler;
};
struct TimerWords {
    unsigned long long head;
    int handler;
    unsigned generation;
    unsigned allocated;
    unsigned free_counters;
    unsigned first_alarm;
    int field_28;
};
extern TimerWords g_Timer;

int iAllocTimerCounter() {
    const unsigned head = g_Timer.free_counters;
    if (head == 0u) return static_cast<int>(0x80008005u);
    Counter* const counter = NORRATH_COUNTER(head);
    g_Timer.allocated += 1;
    counter->handler = 0;
    g_Timer.free_counters = counter->next;
    counter->flags = 0;
    g_Timer.generation += 1;
    counter->word_24 = 0;
    counter->generation_word = ((g_Timer.generation << 1) & 0x3feu) | 1u;
    return static_cast<int>((head << 4) | counter->generation_word);
}
}
