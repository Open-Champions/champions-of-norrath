// Observed sceGifPkEnd at 0x0133f180, 116 bytes. Arguments: the packet state (cursor at
// +0, start at +4, tag at +8) and three words. sceGifPkTerminate(packet) runs first. The
// cursor it left in the packet then receives four words: the third argument OR 0x70000000
// at +0, zero at +4, the first argument at +8 and the second at +12. The tag field (+8 of
// the packet) is set to the pointer that sceGifPkTerminate returned, which is the address
// of the first of these words, and the cursor advances by 16.
// Inferred: 0x70000000 is a DMA tag with the identifier field 7; the observed code only
// ORs the constant.
// Unknown: how the stored tag field is consumed afterwards.
extern "C" {
struct GifPacketState {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* tag;
};
void* sceGifPkTerminate(GifPacketState* packet);

void sceGifPkEnd(GifPacketState* packet, unsigned first, unsigned second, unsigned count) {
    void* const end = sceGifPkTerminate(packet);
    unsigned* const words = reinterpret_cast<unsigned*>(packet->cursor);
    packet->tag = static_cast<unsigned char*>(end);
    words[0] = count | 0x70000000u;
    words[1] = 0;
    words[2] = first;
    words[3] = second;
    packet->cursor = packet->cursor + 16;
}
}
