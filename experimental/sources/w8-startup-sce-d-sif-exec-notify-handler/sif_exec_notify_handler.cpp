/* Observed sceSifExecNotifyHandler at 0x002df7b0, 168 bytes. The first
 * argument selects a table of 16-byte entries. A nonzero argument selects
 * g_SysRebootBuf (0x008e4548) with a fixed count of 32. Zero selects the
 * pointer g_pUserRebootBuf (0x004459c8) with the count g_iUserRebootSize
 * (0x004459cc), both read at call time. A count of zero or below visits
 * nothing. Entries are visited in order. An entry whose word 0 is zero is
 * skipped. Otherwise $gp is set from entry word 2 and the function at word 0
 * is called with the second argument of this function and entry word 1. The
 * caller's $gp is restored at the end. The original leaves a $gp value in
 * $v0 on return; that is not treated as a result here.
 */
struct NotifyEntry {
    void (*handler)(unsigned argument, void* data);
    void* data;
    unsigned gp;
    unsigned unused;
};
#if __SIZEOF_POINTER__ == 4
static_assert(sizeof(NotifyEntry) == 16, "entry stride");
#endif

extern "C" {
extern unsigned char g_SysRebootBuf[];
extern void* g_pUserRebootBuf;
extern int g_iUserRebootSize;
}

#if defined(__mips__) && !defined(NORRATH_HOST_CONTRACT)
static unsigned read_gp() {
    unsigned value;
    __asm__ volatile("move %0,$gp" : "=r"(value));
    return value;
}
static void write_gp(unsigned value) {
    __asm__ volatile("move $gp,%0" : : "r"(value));
}
#else
extern "C" unsigned norrath_host_gp;
static unsigned read_gp() { return norrath_host_gp; }
static void write_gp(unsigned value) { norrath_host_gp = value; }
#endif

extern "C" void sceSifExecNotifyHandler(int use_system_table, unsigned argument) {
    NotifyEntry* entry;
    int count;
    if (use_system_table) {
        entry = reinterpret_cast<NotifyEntry*>(g_SysRebootBuf);
        count = 32;
    } else {
        entry = static_cast<NotifyEntry*>(g_pUserRebootBuf);
        count = g_iUserRebootSize;
    }
    const unsigned saved_gp = read_gp();
    for (; count > 0; --count, ++entry) {
        if (!entry->handler) continue;
        write_gp(entry->gp);
        entry->handler(argument, entry->data);
    }
    write_gp(saved_gp);
}
