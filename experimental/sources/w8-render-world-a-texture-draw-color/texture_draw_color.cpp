// Observed _textureDrawColor__FP8_textureiiiUi at 0x0016e658, 516 bytes. Arguments:
// a texture, two integers, an integer that is passed on to drawAddToList, and a
// 32-bit colour.
// The display list is built at the word glistp (0x004844c8), with gstart
// (0x004844c4) as its start and GLISTSIZE (0x00484468) as its capacity in 8-byte
// units. When fewer than 3000 units remain, the function returns without writing.
// Otherwise it writes, from glistp, one packet:
//   +0   DMA tag (filled in at the end)
//   +8   32-bit 0x11000000, then 32-bit 0x50000000 at +12 (a count is ORed in)
//   +16  GIF tag: low half 0x1000000000008000, high half 14 (a count is ORed in)
//   +32  seven address/data pairs, data first: (0, 0x3f), (0, 6), (1, 0x14),
//        (colour, 1), (0x44, 0x42), (0x156, 0), (0x3001d, 0x47)
// drawTexture__FR6PacketP8_textureiiiiii then appends to the packet through a
// cursor passed by reference, with the arguments (first integer, second integer
// plus one, 0, 0, the halfword at texture + 0, the halfword at texture + 2).
// Then the end is closed: the DMA tag low word becomes ((units - 2) >> 1) |
// 0x10000000 over the units between start and end; the GIF tag gets ORed
// ((units from the GIF tag to the end - 2) >> 1); the second word at +12 gets
// ORed ((bytes from it to the end - 4) >> 4). When the tag id is 1 (always, for
// that value) the tag becomes id 6; the other branch appends the 16-byte tag
// 0x60000000 and moves the end on. glistp becomes the end, and
// drawAddToList__FPviP8_textureP11_drawRecordi receives the old glistp, the
// fourth argument, the texture, 0 and 1.
// Inferred: the pairs are GS register writes (address 0x3f TEXFLUSH, 0x14 TEX1_1,
// 1 RGBAQ, 0x42 ALPHA_1, 0 PRIM, 0x47 TEST_1) in the A+D format; the names are the
// GS register map, not names from this image. The texture's first halfwords are
// presumably its size.
// Unknown: what the zero pair at address 6 holds, and what the fourth argument
// means to the draw list.
struct Packet {
    unsigned long long* cursor;
};

extern "C" {
extern unsigned char* glistp;
extern unsigned char* gstart;
extern int GLISTSIZE;
void drawTexture__FR6PacketP8_textureiiiiii(Packet& packet, void* texture, int a, int b, int c, int d, int e, int f);
void drawAddToList__FPviP8_textureP11_drawRecordi(void* packet, int order, void* texture, void* record, int flag);

void _textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int order, unsigned color) {
    unsigned long long* const start = reinterpret_cast<unsigned long long*>(glistp);
    if (GLISTSIZE - (static_cast<int>(glistp - gstart) >> 3) < 3000) return;

    unsigned* const words = reinterpret_cast<unsigned*>(start);
    words[2] = 0x11000000u;
    unsigned* const direct = words + 3;
    *direct = 0x50000000u;
    unsigned long long* const gif = start + 2;
    gif[0] = 0x1000000000008000ull;
    gif[1] = 14ull;
    start[4] = 0ull;           start[5] = 0x3full;
    start[6] = 0ull;           start[7] = 6ull;
    start[8] = 1ull;           start[9] = 0x14ull;
    start[10] = color;         start[11] = 1ull;
    start[12] = 0x44ull;       start[13] = 0x42ull;
    start[14] = 0x156ull;      start[15] = 0ull;
    start[16] = 0x3001dull;    start[17] = 0x47ull;

    Packet packet;
    packet.cursor = start + 18;
    const short* const size = static_cast<const short*>(texture);
    drawTexture__FR6PacketP8_textureiiiiii(packet, texture, x, y + 1, 0, 0, size[0], size[1]);

    unsigned long long* end = packet.cursor;
    const unsigned long long tag = static_cast<unsigned>(((static_cast<int>(end - start) - 2) >> 1) | 0x10000000);
    start[0] = tag;
    gif[0] |= static_cast<long long>((static_cast<int>(end - gif) - 2) >> 1);
    *direct |= static_cast<unsigned>((static_cast<int>(reinterpret_cast<unsigned char*>(end) - reinterpret_cast<unsigned char*>(direct)) - 4) >> 4);
    if (((static_cast<long long>(tag) >> 28) & 7) == 1) {
        start[0] = (tag & 0x8fffffffull) | 0x60000000ull;
    } else {
        end[1] = 0ull;
        end[0] = 0x60000000ull;
        end += 2;
    }
    unsigned char* const previous = glistp;
    glistp = reinterpret_cast<unsigned char*>(end);
    drawAddToList__FPviP8_textureP11_drawRecordi(previous, order, texture, nullptr, 1);
}
}
