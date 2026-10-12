// Observed sceGifPkTerminate at 0x0133f000, 88 bytes. While the cursor (+0)
// has either of address bits 2 and 3 set it stores a zero word and advances
// four bytes, padding to a 16-byte boundary. If the tag word (+8) is nonzero,
// the word it points at is increased by the quadword distance from the tag to
// the padded cursor, minus one (arithmetic shift). The tag word is then cleared,
// the padded cursor is stored back and also returned.
extern "C" {
struct GifPacketState {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* tag;
};
void* sceGifPkTerminate(GifPacketState* packet) {
    unsigned cursor = static_cast<unsigned>(reinterpret_cast<unsigned long>(packet->cursor));
    unsigned char* base = packet->cursor;
    unsigned char* tag = packet->tag;
    while ((cursor & 0xcu) != 0) {
        *reinterpret_cast<unsigned*>(base) = 0;
        base += 4;
        cursor += 4;
    }
    if (tag != nullptr) {
        unsigned tag_address = static_cast<unsigned>(reinterpret_cast<unsigned long>(tag));
        *reinterpret_cast<unsigned*>(tag) += (static_cast<int>(cursor - tag_address) >> 4) - 1;
    }
    packet->tag = nullptr;
    packet->cursor = base;
    return base;
}
}
