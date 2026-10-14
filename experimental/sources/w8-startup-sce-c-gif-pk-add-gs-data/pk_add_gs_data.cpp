// Observed sceGifPkAddGsData at 0x0133f2d0, 40 bytes. It writes the low 32
// bits of the 64-bit argument at the write cursor (+0), the high 32 bits at
// the next word, and advances the cursor by eight bytes. Nothing is returned.
extern "C" {
struct GifPacketState {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* tag;
};
void sceGifPkAddGsData(GifPacketState* packet, unsigned long long value) {
    unsigned* cursor = reinterpret_cast<unsigned*>(packet->cursor);
    cursor[0] = static_cast<unsigned>(value);
    cursor[1] = static_cast<unsigned>(value >> 32);
    packet->cursor = reinterpret_cast<unsigned char*>(cursor + 2);
}
}
