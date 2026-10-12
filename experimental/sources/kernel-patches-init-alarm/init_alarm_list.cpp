/* Observed _InitAlarm at 0x002df208, 76 bytes. This is not InitAlarm.
 * It stores the address of g_AlarmBuf in g_pFreeAlarm, then writes 64
 * pointer words at a 16-byte stride, from offset 1008 down to 0. Each
 * word holds the address 16 bytes past its own slot, so slot k points at
 * slot k + 1. The word at offset 1008 is then cleared and the function
 * returns 0. No other byte of g_AlarmBuf is written.
 * Inferred: g_AlarmBuf is a 1024-byte pool of 64 sixteen-byte entries
 * and g_pFreeAlarm is the head of its free chain.
 * Unknown: what the other twelve bytes of an entry hold.
 * g_AlarmBuf and g_pFreeAlarm are not defined here.
 */
extern "C" {
struct AlarmSlot {
    unsigned next;
    unsigned unwritten[3];
};
extern AlarmSlot g_AlarmBuf[64];
extern unsigned g_pFreeAlarm;
static inline unsigned address_word(const void* address) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(address));
}
int _InitAlarm() {
    g_pFreeAlarm = address_word(g_AlarmBuf);
    // Observed order: offset 1008 first, down to offset 0.
    for (int index = 63; index >= 0; --index)
        g_AlarmBuf[index].next = address_word(g_AlarmBuf + index + 1);
    g_AlarmBuf[63].next = 0;
    return 0;
}
}
