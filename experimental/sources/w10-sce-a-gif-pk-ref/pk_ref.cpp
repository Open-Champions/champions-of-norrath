/* Observed sceGifPkRef at 0x0133f0e8, 152 bytes, in the 0x01000000 module
 * image. Arguments: the packet state, a data address, a count and three more
 * words (the fourth, fifth and sixth arguments). sceGifPkTerminate
 * runs first and its result is not used. At the cursor, a 16-byte entry is then
 * written as four words: the count OR the sixth argument OR 0x30000000; the
 * data address with bits 31 and 29 cleared (AND 0x9fffffff); the fourth
 * argument; the fifth argument. The cursor advances by 16. The result register
 * is left holding the entry address plus 4 and is not a defined result.
 * The meaning of the entry is not established by this routine.
 */
extern "C" {
struct GifPacketState {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* dma_tag;
    unsigned char* gif_tag;
};

void* sceGifPkTerminate(GifPacketState* packet);

void sceGifPkRef(GifPacketState* packet, const void* data, unsigned count, unsigned fourth, unsigned fifth,
                 unsigned sixth) {
    sceGifPkTerminate(packet);
    unsigned* const slot = reinterpret_cast<unsigned*>(packet->cursor);
    slot[0] = sixth | (count | 0x30000000u);
    slot[1] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(data)) & 0x9fffffffu;
    packet->cursor = packet->cursor + 16;
    slot[2] = fourth;
    slot[3] = fifth;
}
}
