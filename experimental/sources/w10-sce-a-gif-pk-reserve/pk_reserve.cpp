/* Observed sceGifPkReserve at 0x0133f1f8, 20 bytes, in the 0x01000000 module
 * image. The first argument is the packet state whose first word is the write
 * cursor. The cursor advances by four times the second argument (words), and
 * the old cursor is returned. Nothing is checked: no bound, no alignment, no
 * padding.
 */
extern "C" {
struct GifPacketState {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* dma_tag;
    unsigned char* gif_tag;
};

void* sceGifPkReserve(GifPacketState* packet, int words) {
    unsigned char* const old = packet->cursor;
    packet->cursor = old + words * 4;
    return old;
}
}
