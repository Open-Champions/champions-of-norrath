/* Observed scePadSetActDirect at 0x002d4198, 192 bytes. Calls
 * scePadGetDmaStr(port, slot) and returns 0 unless byte 114 of the block it
 * returns is 1. Otherwise the object whose address is in PadInfo word +4
 * (PadInfo is at 0x008dd8d0, 28 bytes per slot and 112 per port) receives six
 * bytes of the third argument at offset 12, then 1 at word +4 and 6 at word
 * +8, and _send_to_iop(port, slot) is called. The result is 1 when that call
 * returned exactly 1, else 0.
 */
extern "C" {
extern unsigned char norrath_libpad_port_info_view[];

unsigned char* scePadGetDmaStr(int port, int slot);
int _send_to_iop(int port, int slot);

int scePadSetActDirect(int port, int slot, const unsigned char* data) {
    if (scePadGetDmaStr(port, slot)[114] != 1) return 0;
    const unsigned* info = reinterpret_cast<const unsigned*>(norrath_libpad_port_info_view + slot * 28 + port * 112);
    unsigned* request = reinterpret_cast<unsigned*>(static_cast<__UINTPTR_TYPE__>(info[1]));
    unsigned char* destination = reinterpret_cast<unsigned char*>(request) + 12;
    for (int index = 0; index < 6; ++index) destination[index] = data[index];
    request[1] = 1;
    request[2] = 6;
    return _send_to_iop(port, slot) == 1;
}
}
