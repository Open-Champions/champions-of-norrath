/* Observed sceSifAddRebootNotifyHandler at 0x002df6a0, 184 bytes. The first
 * argument is an identifier. With the top bit clear the entries are those of
 * the user table (g_pUserRebootBuf at 0x004459c8, entry count in
 * g_iUserRebootSize at 0x004459cc); with the top bit set the entries are
 * those of the 512-byte system table g_SysRebootBuf (0x008e4548), 32 entries,
 * and the top bit is dropped from the index. Entries are 16 bytes. An index
 * that is not below the entry count (compared unsigned) returns 0x80000069
 * and changes nothing. Otherwise, with interrupts disabled through DIntr, it
 * stores the second argument in word 0 of the entry, the third argument in
 * word 1 and the current $gp in word 2, calls EIntr only if DIntr returned
 * nonzero, and returns 0. Word 3 is left as it was. The table pointer and
 * count are read before DIntr is called.
 * Inferred: the second argument is the handler later called by
 * sceSifExecNotifyHandler with the third as its data argument, and the saved
 * $gp is what that routine installs for the call.
 */
extern "C" {
extern void* g_pUserRebootBuf;
extern int g_iUserRebootSize;
extern unsigned char g_SysRebootBuf[];

int DIntr();
int EIntr();
}

#if defined(__mips__) && !defined(NORRATH_HOST_CONTRACT)
static unsigned read_gp() {
    unsigned value;
    __asm__ volatile("move %0,$gp" : "=r"(value));
    return value;
}
#else
extern "C" unsigned norrath_host_gp;
static unsigned read_gp() { return norrath_host_gp; }
#endif

extern "C" int sceSifAddRebootNotifyHandler(unsigned identifier, void (*handler)(), void* data) {
    unsigned char* table;
    unsigned count;
    if (static_cast<int>(identifier) >= 0) {
        table = static_cast<unsigned char*>(g_pUserRebootBuf);
        count = static_cast<unsigned>(g_iUserRebootSize);
    } else {
        identifier &= 0x7fffffffu;
        table = g_SysRebootBuf;
        count = 32;
    }
    if (identifier >= count) return static_cast<int>(0x80000069u);
    int was_enabled = DIntr();
    unsigned* entry = reinterpret_cast<unsigned*>(table + identifier * 16);
    entry[0] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(handler));
    entry[1] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(data));
    entry[2] = read_gp();
    if (was_enabled != 0) EIntr();
    return 0;
}
