/* Observed scePadInfoAct at 0x002d3d68, 288 bytes (arguments: port, slot,
 * index, selector). PadInfo (0x008dd8d0) holds 28 bytes per slot and 112 per
 * port; word +16 is zero when the port/slot is not open, in which case the
 * result is 0. Otherwise it reads the 128-byte block from
 * scePadGetDmaStr(port, slot). The result is 0 unless byte 114 is 1 and byte
 * 100 is at least 2, and 0 unless the signed index is less than byte 106
 * (taken as unsigned). An index of -1 returns byte 106. Otherwise selector
 * 1, 2, 3 or 4 returns the byte at 48 + (selector - 1) + 4 * index, any other
 * selector returns 0. A negative index other than -1 passes the range check
 * and addresses before offset 48.
 * Inferred: bytes 48 onward hold four bytes per actuator and byte 106 is the
 * actuator count; neither is established by this routine.
 */
extern "C" {
extern unsigned char norrath_libpad_port_info_view[];

unsigned char* scePadGetDmaStr(int port, int slot);

int scePadInfoAct(int port, int slot, int index, int selector) {
    const unsigned* info = reinterpret_cast<const unsigned*>(norrath_libpad_port_info_view + slot * 28 + port * 112);
    if (info[4] == 0) return 0;
    const unsigned char* block = scePadGetDmaStr(port, slot);
    if (block[114] != 1) return 0;
    if (block[100] < 2) return 0;
    if (!(index < static_cast<int>(block[106]))) return 0;
    if (index == -1) return block[106];
    switch (selector) {
    case 1: return block[48 + index * 4];
    case 2: return block[49 + index * 4];
    case 3: return block[50 + index * 4];
    case 4: return block[51 + index * 4];
    default: return 0;
    }
}
}
