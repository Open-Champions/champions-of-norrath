/* Observed scePadGetState at 0x002d3bc8, 120 bytes. PadInfo (0x008dd8d0)
 * holds 28 bytes per slot and 112 per port; word +16 is zero when the
 * port/slot is not open, in which case the result is 99 and nothing else is
 * read. Otherwise it reads the 128-byte block from scePadGetDmaStr(port,
 * slot): byte 112 is the result, except that byte 112 equal to 6 together
 * with byte 113 equal to 2 gives 5.
 * Inferred: byte 112 is the pad state value and 99 is the closed marker; the
 * 6-to-5 substitution is observed, its reason is not established here.
 */
extern "C" {
extern unsigned char norrath_libpad_port_info_view[];

unsigned char* scePadGetDmaStr(int port, int slot);

int scePadGetState(int port, int slot) {
    const unsigned* info = reinterpret_cast<const unsigned*>(norrath_libpad_port_info_view + slot * 28 + port * 112);
    if (info[4] == 0) return 99;
    const unsigned char* block = scePadGetDmaStr(port, slot);
    if (block[112] == 6 && block[113] == 2) return 5;
    return block[112];
}
}
