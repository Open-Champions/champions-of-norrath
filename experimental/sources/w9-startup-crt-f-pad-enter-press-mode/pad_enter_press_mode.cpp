/* Observed scePadEnterPressMode at 0x002d44f8, 84 bytes. PadInfo
 * (0x008dd8d0) holds 28 bytes per slot and 112 per port; word +16 is zero when
 * the port/slot is not open, in which case the result is 0 and nothing is
 * called. Otherwise it returns the result of
 * scePadSetButtonInfo(port, slot, 0xfff). What the third argument selects is
 * decided by that routine and is not claimed here.
 */
extern "C" {
extern unsigned char norrath_libpad_port_info_view[];

int scePadSetButtonInfo(int port, int slot, unsigned mask);

int scePadEnterPressMode(int port, int slot) {
    const unsigned* info = reinterpret_cast<const unsigned*>(norrath_libpad_port_info_view + slot * 28 + port * 112);
    if (info[4] == 0) return 0;
    return scePadSetButtonInfo(port, slot, 0xfff);
}
}
