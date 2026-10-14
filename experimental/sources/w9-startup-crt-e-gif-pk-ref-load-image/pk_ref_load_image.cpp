// Observed sceGifPkRefLoadImage at 0x0133f320, 448 bytes, in the 0x01000000 module image.
// Arguments: the packet state, then a1, a2, a3, a data pointer, a quadword count, x, y,
// and two more words passed on the stack (width, height). Calls, in order:
//   sceGifPkCnt(packet, 0, 0, 0)
//   sceGifPkOpenGifTag(packet, tag) where tag is the 16-byte constant at 0x0133f4e0: the
//     low doubleword is 0x1000000000000000 and the high doubleword is 0x0e. It is passed
//     as one 128-bit value in register a1.
//   sceGifPkAddGsAD(packet, 0x50, (a1 & 0xffff) << 32 | (a3 & 0xffff) << 48 | (a2 & 0xff) << 56)
//   sceGifPkAddGsAD(packet, 0x51, x << 32 | y << 48)
//   sceGifPkAddGsAD(packet, 0x52, width | height << 32)   (width as a 32-bit value)
//   sceGifPkAddGsAD(packet, 0x53, 0)
//   sceGifPkCloseGifTag(packet)
// A zero quadword count ends there. Otherwise the data is sent in chunks of at most 32767
// quadwords: for each chunk sceGifPkCnt(packet, 0, 0, 0), then sceGifPkReserve(packet, 4)
// returns a 16-byte slot at which the doubleword at +8 is zeroed and the doubleword at +0
// is chunk | 2 << 58, with bit 15 also set on the last chunk; then
// sceGifPkRef(packet, data, chunk, 0, 0, 0). The data pointer advances by chunk * 16 and
// the remaining count falls by chunk.
// Inferred: the AD register addresses 0x50 to 0x53 and the field positions match the GS
// transfer registers; the observed code only builds these numbers.
// The tag constant is held in .rodata of this unit.
// On the target the tag call uses an inline-assembly sequence, because the reviewed
// compiler passes a 128-bit value by reference while the original passes it in register
// a1 (a quadword load).
extern "C" {
struct GifPacketState {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* dma_tag;
    unsigned char* gif_tag;
};
void* sceGifPkCnt(GifPacketState* packet, unsigned a1, unsigned a2, unsigned a3);
void sceGifPkAddGsAD(GifPacketState* packet, unsigned reg, unsigned long long value);
void sceGifPkCloseGifTag(GifPacketState* packet);
void* sceGifPkReserve(GifPacketState* packet, int words);
void sceGifPkRef(GifPacketState* packet, const void* data, unsigned quadwords, unsigned a3, unsigned a4, unsigned a5);

#if defined(NORRATH_HOST_CONTRACT)
void norrath_open_gif_tag(GifPacketState* packet, const unsigned long long* tag);
static void open_gif_tag(GifPacketState* packet, const unsigned long long* tag) {
    norrath_open_gif_tag(packet, tag);
}
#else
void sceGifPkOpenGifTag(GifPacketState* packet);
static void open_gif_tag(GifPacketState* packet, const unsigned long long* tag) {
    register GifPacketState* packet_in asm("$4") = packet;
    asm volatile(
        ".set push\n\t.set noreorder\n\t.set noat\n\t"
        "lq $5, 0(%1)\n\t"
        "jal sceGifPkOpenGifTag\n\t"
        "nop\n\t"
        ".set pop"
        : "+r"(packet_in)
        : "r"(tag)
        : "$1", "$2", "$3", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15",
          "$24", "$25", "$31", "memory");
}
#endif

void sceGifPkRefLoadImage(GifPacketState* packet, unsigned a1, unsigned a2, unsigned a3, const void* image,
                          unsigned quadwords, unsigned x, unsigned y, unsigned width, unsigned height) {
    static const unsigned long long tag[2] = {0x1000000000000000ull, 0x0eull};
    sceGifPkCnt(packet, 0, 0, 0);
    open_gif_tag(packet, tag);
    sceGifPkAddGsAD(packet, 0x50,
                    (static_cast<unsigned long long>(a1 & 0xffffu) << 32) |
                        (static_cast<unsigned long long>(a3 & 0xffffu) << 48) |
                        (static_cast<unsigned long long>(a2 & 0xffu) << 56));
    sceGifPkAddGsAD(packet, 0x51,
                    (static_cast<unsigned long long>(x) << 32) | (static_cast<unsigned long long>(y) << 48));
    sceGifPkAddGsAD(packet, 0x52, static_cast<unsigned long long>(width) | (static_cast<unsigned long long>(height) << 32));
    sceGifPkAddGsAD(packet, 0x53, 0);
    sceGifPkCloseGifTag(packet);
    const unsigned char* data = static_cast<const unsigned char*>(image);
    while (quadwords != 0) {
        const unsigned chunk = quadwords < 0x7fffu ? quadwords : 0x7fffu;
        sceGifPkCnt(packet, 0, 0, 0);
        unsigned long long* const slot = static_cast<unsigned long long*>(sceGifPkReserve(packet, 4));
        unsigned long long header = static_cast<unsigned long long>(chunk) | 0x0800000000000000ull;
        if (chunk == quadwords) header |= 0x8000ull;
        slot[1] = 0;
        slot[0] = header;
        sceGifPkRef(packet, data, chunk, 0, 0, 0);
        quadwords -= chunk;
        data += chunk * 16u;
    }
}
}
