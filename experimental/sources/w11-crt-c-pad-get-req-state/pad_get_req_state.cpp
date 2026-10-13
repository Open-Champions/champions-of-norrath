/* Observed scePadGetReqState at 0x002d3ce0, 80 bytes. PadInfo (0x008dd8d0)
 * holds 28 bytes per slot and 112 per port; word +16 is zero when the
 * port/slot is not open, in which case the result is 0 and nothing else is
 * read. Otherwise the result is byte 113 of the 128-byte block returned by
 * scePadGetDmaStr(port, slot), the byte scePadSetReqState writes.
 */
extern "C" {
extern unsigned char norrath_libpad_port_info_view[];

unsigned char* scePadGetDmaStr(int port, int slot);

int scePadGetReqState(int port, int slot) {
    const unsigned* info = reinterpret_cast<const unsigned*>(norrath_libpad_port_info_view + slot * 28 + port * 112);
    if (info[4] == 0) return 0;
    return scePadGetDmaStr(port, slot)[113];
}
}
