// Observed sceGifPkInit at 0x0133efd0, 16 bytes. The packet is three words:
// the write cursor at +0, the buffer start at +4 and the open-tag word at +8.
// Init stores the buffer into both the cursor and the start and clears the
// tag word. Nothing is returned.
extern "C" {
struct GifPacketState {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* tag;
};
void sceGifPkInit(GifPacketState* packet, void* buffer) {
    packet->cursor = static_cast<unsigned char*>(buffer);
    packet->tag = nullptr;
    packet->start = static_cast<unsigned char*>(buffer);
}
}
