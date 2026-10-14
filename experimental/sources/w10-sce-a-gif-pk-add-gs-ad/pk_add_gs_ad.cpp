/* Observed sceGifPkAddGsAD at 0x0133f2f8, 36 bytes, in the 0x01000000 module
 * image. Arguments: the packet state (cursor in the first word), a 32-bit
 * register-address value and a 64-bit data value. At the cursor it stores the
 * 64-bit data value, then the register value zero-extended to 64 bits, and
 * advances the cursor by 16. The data value goes first. The result register
 * holds the advanced cursor. Nothing is checked.
 */
extern "C" {
struct GifPacketState {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* dma_tag;
    unsigned char* gif_tag;
};

void* sceGifPkAddGsAD(GifPacketState* packet, unsigned address, unsigned long long data) {
    unsigned char* slot = packet->cursor;
    *reinterpret_cast<unsigned long long*>(slot) = data;
    slot += 8;
    *reinterpret_cast<unsigned long long*>(slot) = address;
    slot += 8;
    packet->cursor = slot;
    return slot;
}
}
