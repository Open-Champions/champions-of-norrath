/* Observed sceSifRemoveRebootNotifyHandler at 0x002df758, 84 bytes. The
 * argument is an identifier. With the top bit clear the entries are those of
 * the user table (g_pUserRebootBuf at 0x004459c8, entry count in
 * g_iUserRebootSize at 0x004459cc); with the top bit set the entries are
 * those of the 512-byte system table g_SysRebootBuf (0x008e4548), 32 entries,
 * and the top bit is dropped from the index. Entries are 16 bytes. An index
 * that is not below the entry count (compared unsigned) returns -105 and
 * changes nothing. Otherwise the first word of the entry is cleared and the
 * result is 0. The other words of the entry are left as they were.
 * What a registered entry is used for by a reboot is not established here.
 */
extern "C" {
extern void* g_pUserRebootBuf;
extern int g_iUserRebootSize;
extern unsigned char g_SysRebootBuf[];

int sceSifRemoveRebootNotifyHandler(unsigned identifier) {
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
    if (identifier >= count) return -105;
    *reinterpret_cast<unsigned*>(table + identifier * 16) = 0;
    return 0;
}
}
