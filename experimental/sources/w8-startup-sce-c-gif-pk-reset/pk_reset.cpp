// Observed sceGifPkReset at 0x0133eff0, 16 bytes. It copies the buffer start
// (+4) back into the write cursor (+0) and clears the tag word (+8). Nothing
// is returned.
extern "C" {
struct GifPacketState {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* tag;
};
void sceGifPkReset(GifPacketState* packet) {
    unsigned char* start = packet->start;
    packet->tag = nullptr;
    packet->cursor = start;
}
}
