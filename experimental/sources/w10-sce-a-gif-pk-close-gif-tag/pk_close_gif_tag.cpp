/* Observed sceGifPkCloseGifTag at 0x0133f228, 168 bytes, in the 0x01000000
 * module image. The tag address is the state's fourth word (+12, stored by
 * sceGifPkOpenGifTag) and the cursor is the first word. The distance from the
 * tag to the cursor in 8-byte units, minus 2 for the tag itself, is the
 * data size D (signed 32-bit arithmetic). The tag's low 64 bits are read, and
 * bits 58 and 59 (a two-bit field F) and bits 60 to 63 (a four-bit field N)
 * select the count that is added to that doubleword:
 *   F = 2: D / 2 (logical shift of the 32-bit value)
 *   F = 1: ceil(D / M)
 *   F = 0 or 3: ceil((D / 2) / M)
 * where M is N, or 16 when N is zero. The divisions are unsigned 32-bit. The
 * count is zero-extended and added to the doubleword at the tag (the field
 * it lands in is not asserted here). The state's tag word is cleared. The
 * cursor is then padded with zero words until bits 2 and 3 are clear and
 * stored back. The names F, N, M and D are descriptive only; the field
 * meanings were not taken from this routine.
 */
extern "C" {
struct GifPacketState {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* dma_tag;
    unsigned char* gif_tag;
};

void sceGifPkCloseGifTag(GifPacketState* packet) {
    unsigned char* const tag = packet->gif_tag;
    unsigned char* cursor = packet->cursor;
    const unsigned long long low = *reinterpret_cast<unsigned long long*>(tag);
    const int dwords = static_cast<int>((cursor - tag)) >> 3;
    const unsigned field = static_cast<unsigned>(low >> 58) & 3u;
    unsigned count = static_cast<unsigned>(dwords - 2);
    const unsigned halved = count >> 1;
    if (field != 2) {
        if (field != 1) count = halved;
        unsigned divisor = static_cast<unsigned>(low >> 60);
        if (divisor == 0) divisor = 16;
        count = (count + divisor - 1) / divisor;
    } else {
        count = halved;
    }
    packet->gif_tag = nullptr;
    *reinterpret_cast<unsigned long long*>(tag) = low + count;
    while ((reinterpret_cast<__UINTPTR_TYPE__>(cursor) & 0xc) != 0) {
        *reinterpret_cast<unsigned*>(cursor) = 0;
        cursor += 4;
    }
    packet->cursor = cursor;
}
}
