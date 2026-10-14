/* Observed scePadGetButtonMask at 0x002d4330, 180 bytes (a file-local routine
 * of the pad library). PadInfo (0x008dd8d0) holds 28 bytes per slot and 112
 * per port; word +16 is zero when the port/slot is not open, in which case the
 * result is 0. Otherwise it reads the 128-byte block from
 * scePadGetDmaStr(port, slot). The result is 0 unless byte 114 is exactly 1
 * and bytes 100 and 102 are both at least 2. Then it is the four bytes at
 * 121, 122, 123 and 124 combined with byte 121 as the least significant, as a
 * signed 32-bit value. The meaning of the fields is not established here;
 * scePadInfoPressMode compares the result with 0x3ffff.
 */
extern "C" {
extern unsigned char norrath_libpad_port_info_view[];

unsigned char* scePadGetDmaStr(int port, int slot);

int scePadGetButtonMask(int port, int slot) {
    const unsigned* info = reinterpret_cast<const unsigned*>(norrath_libpad_port_info_view + slot * 28 + port * 112);
    if (info[4] == 0) return 0;
    const unsigned char* block = scePadGetDmaStr(port, slot);
    if (block[114] != 1) return 0;
    if (block[100] < 2) return 0;
    if (block[102] < 2) return 0;
    return static_cast<int>(block[121] + (static_cast<unsigned>(block[122]) << 8) +
                            (static_cast<unsigned>(block[123]) << 16) + (static_cast<unsigned>(block[124]) << 24));
}
}
