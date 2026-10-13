/* Observed scePadSetReqState at 0x002d3c78, 100 bytes (a file-local routine
 * of the pad library). PadInfo (0x008dd8d0) holds 28 bytes per slot and 112
 * per port; word +16 is zero when the port/slot is not open, in which case the
 * result is 0 and nothing else happens. Otherwise byte 113 of the 128-byte
 * block returned by scePadGetDmaStr(port, slot) receives the low byte of the
 * third argument, and the result is 1. What the byte means is not established
 * by this routine.
 */
extern "C" {
extern unsigned char norrath_libpad_port_info_view[];

unsigned char* scePadGetDmaStr(int port, int slot);

int scePadSetReqState(int port, int slot, int state) {
    const unsigned* info = reinterpret_cast<const unsigned*>(norrath_libpad_port_info_view + slot * 28 + port * 112);
    if (info[4] == 0) return 0;
    scePadGetDmaStr(port, slot)[113] = static_cast<unsigned char>(state);
    return 1;
}
}
