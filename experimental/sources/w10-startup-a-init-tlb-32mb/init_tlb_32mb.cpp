// Observed InitTLB32MB at 0x002dfda8, 500 bytes (a local symbol in the image; InitTLB calls it
// when GetMemorySize returns 0x02000000). It programs the CPU's TLB from the 28-byte object
// TLBInfo (0x00446000), read as seven words: three entry counts at +0, +4, +8; a word at +12 that
// receives the final index; and three pointers at +16, +20, +24 to tables of 16-byte entries
// (four words each). In order:
//   1. kprintf("# TLB spad=0 kernel=1:%d default=%d:%d extended=%d:%d\n") with A-1, A, A+B-1,
//      A+B and A+B+C-1, where A, B and C are the three counts.
//   2. The wired register (coprocessor 0 register 6) is cleared, with a sync.p after it.
//   3. If A >= 49: kprintf("# TLB over flow (1)") and Exit(1). Otherwise entries 0..A-1 of the
//      first table go through _SetTLBEntry(index, word0, word1, word2, word3) in order.
//   4. The same for B with table two, indices continuing from A, overflow text "(2)" when the
//      running index plus B reaches 49.
//   5. The index after the first two tables is stored at TLBInfo +12 and written to the wired register (sync.p).
//   6. If C > 0, the same for the third table with the overflow text "(3)".
//   7. Indices below 48 that are still unused are filled with _SetTLBEntry(i, 0, address, 0, 0)
//      where address starts at 0xe0000000 + (index after all three tables) * 8192 and grows by
//      8192 per entry. Nothing meaningful is returned (the result register holds that index
//      shifted left 13 on exit; no caller uses it).
// Inferred only: the entry fields and the three table roles beyond their text labels.
// Unknown: what _SetTLBEntry writes (its arguments are passed through as observed).
// Nothing was executed on the EE.
extern "C" {
struct TlbEntry {
    int word0, word1, word2, word3;
};
struct TlbInfoObject {
    int count0, count1, count2;
    int final_index;
    const TlbEntry* table0;
    const TlbEntry* table1;
    const TlbEntry* table2;
};
extern TlbInfoObject TLBInfo;
void kprintf(const char* format, ...);
void Exit(int status);
void _SetTLBEntry(int index, int word0, int word1, int word2, int word3);

#ifndef NORRATH_HOST_CHECK
static inline void set_wired(int value) {
    __asm__ volatile("mtc0 %0, $6\n\tsync.p" : : "r"(value));
}
#endif

__attribute__((always_inline)) static inline int install(int index, int count, const TlbEntry* table, const char* overflow) {
    int end = index + count;
    if (end >= 49) {
        kprintf(overflow);
        Exit(1);
    }
    for (; index < end; ++index, ++table) _SetTLBEntry(index, table->word0, table->word1, table->word2, table->word3);
    return index;
}

void InitTLB32MB() {
    int a = TLBInfo.count0;
    int b = TLBInfo.count1;
    int c = TLBInfo.count2;
    kprintf("# TLB spad=0 kernel=1:%d default=%d:%d extended=%d:%d\n", a - 1, a, a + b - 1, a + b, a + b + c - 1);
    set_wired(0);
    int index = install(0, TLBInfo.count0, TLBInfo.table0, "# TLB over flow (1)");
    index = install(index, TLBInfo.count1, TLBInfo.table1, "# TLB over flow (2)");
    TLBInfo.final_index = index;
    set_wired(index);
    int last = index;
    if (TLBInfo.count2 > 0) last = install(index, TLBInfo.count2, TLBInfo.table2, "# TLB over flow (3)");
    unsigned address = static_cast<unsigned>(last) * 8192u + 0xe0000000u;
    for (int i = last; i < 48; ++i, address += 8192u) _SetTLBEntry(i, 0, static_cast<int>(address), 0, 0);
}
}
