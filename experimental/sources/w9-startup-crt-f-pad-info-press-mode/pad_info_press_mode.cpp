/* Observed scePadInfoPressMode at 0x002d4498, 92 bytes. PadInfo
 * (0x008dd8d0) holds 28 bytes per slot and 112 per port; word +16 is zero when
 * the port/slot is not open, in which case the result is 0 and nothing is
 * called. Otherwise scePadGetButtonMask(port, slot) is called and the result is
 * 1 exactly when it equals 0x3ffff (all eighteen mask bits set), else 0.
 */
extern "C" {
extern unsigned char norrath_libpad_port_info_view[];

unsigned scePadGetButtonMask(int port, int slot);

int scePadInfoPressMode(int port, int slot) {
    const unsigned* info = reinterpret_cast<const unsigned*>(norrath_libpad_port_info_view + slot * 28 + port * 112);
    if (info[4] == 0) return 0;
    return scePadGetButtonMask(port, slot) == 0x3ffffu;
}
}
