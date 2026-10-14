/* Observed sceGifPkCnt at 0x0133f070, 116 bytes, in the 0x01000000 module
 * image. Arguments: the packet state and three words (the second, third and
 * fourth arguments). sceGifPkTerminate runs first (it pads the cursor to 16
 * bytes, closes the open tag and returns the padded cursor), and its result is stored in the state's third word (the
 * open-tag slot at +8). At the cursor, a 16-byte tag is then written as four
 * words: the fourth argument OR 0x10000000, zero, the second argument, the
 * third argument. The cursor advances by 16 and is returned. The meaning of
 * the arguments is not established by this routine.
 */
extern "C" {
struct GifPacketState {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* dma_tag;
    unsigned char* gif_tag;
};

void* sceGifPkTerminate(GifPacketState* packet);

void* sceGifPkCnt(GifPacketState* packet, unsigned second, unsigned third, unsigned fourth) {
    packet->dma_tag = static_cast<unsigned char*>(sceGifPkTerminate(packet));
    unsigned* const slot = reinterpret_cast<unsigned*>(packet->cursor);
    slot[0] = fourth | 0x10000000u;
    slot[2] = second;
    packet->cursor = packet->cursor + 16;
    slot[3] = third;
    slot[1] = 0;
    return packet->cursor;
}
}
