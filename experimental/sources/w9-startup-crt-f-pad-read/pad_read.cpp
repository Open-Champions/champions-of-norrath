/* Observed scePadRead at 0x002d3a60, 352 bytes. PadInfo (0x008dd8d0) holds 28
 * bytes per slot and 112 per port; word +16 is zero when the port/slot is not
 * open, in which case the result is 0. Otherwise interrupts are disabled
 * (DIntr), the 128-byte block returned by scePadGetDmaStr(port, slot) is
 * copied to a local buffer (the copy handles an address that is not 8-byte
 * aligned), the local word at offset 96 gives a byte count, that many bytes of
 * the local copy are copied to the caller's buffer with memcpy, interrupts are
 * enabled again only when DIntr returned exactly 1, and the count is
 * returned. The count is not checked against 128.
 */
extern "C" {
extern unsigned char norrath_libpad_port_info_view[];

int DIntr();
int EIntr();
unsigned char* scePadGetDmaStr(int port, int slot);
void* memcpy(void* destination, const void* source, unsigned long size);

int scePadRead(int port, int slot, void* buffer) {
    const unsigned* info = reinterpret_cast<const unsigned*>(norrath_libpad_port_info_view + slot * 28 + port * 112);
    if (info[4] == 0) return 0;
    unsigned char local[128];
    const int was_enabled = DIntr();
    const unsigned char* block = scePadGetDmaStr(port, slot);
    for (int index = 0; index < 128; ++index) local[index] = block[index];
    const unsigned count = *reinterpret_cast<const unsigned*>(local + 96);
    memcpy(buffer, local, count);
    if (was_enabled == 1) EIntr();
    return static_cast<int>(*reinterpret_cast<const unsigned*>(local + 96));
}
}
