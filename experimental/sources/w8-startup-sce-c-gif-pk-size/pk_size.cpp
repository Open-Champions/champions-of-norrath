// Observed sceGifPkSize at 0x0133f058, 20 bytes. It returns the byte distance
// from the buffer start (+4) to the write cursor (+0), shifted right by four
// as an unsigned value, which counts 16-byte quadwords.
extern "C" {
struct GifPacketState {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* tag;
};
int sceGifPkSize(GifPacketState* packet) {
    unsigned start = static_cast<unsigned>(reinterpret_cast<unsigned long>(packet->start));
    unsigned cursor = static_cast<unsigned>(reinterpret_cast<unsigned long>(packet->cursor));
    return static_cast<int>((cursor - start) >> 4);
}
}
