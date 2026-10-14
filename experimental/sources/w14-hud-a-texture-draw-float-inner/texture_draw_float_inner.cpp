// Observed _textureDrawFloat__FP8_textureffiUiff at 0x0016d978, 744 bytes.
// Arguments: a texture, a float x, a float y, an integer passed on to
// drawAddToList, a 32-bit colour, and two floats that scale the texture's width
// and height (they multiply the halfwords at texture + 0 and + 2).
//
// The display list is the word glistp with gstart as its start and GLISTSIZE as
// its capacity in 8-byte units (see _textureDrawColor). With fewer than 3000
// units left the function returns without writing. Otherwise it writes one fixed
// 208-byte packet at glistp (26 doublewords):
//   +0   DMA tag, +8 word 0x11000000, +12 word 0x5000000c
//   +16  GIF tag: 0x1000000000008000 | 11, high half 14
//   then twelve data/address pairs, data first:
//   (0,0x3f) (0,6) (0x61,0x14) (colour,1) (0x44,0x42) (0x156,0) (0x3001d,0x47)
//   (0x80008,3) (P0,5) (T1,3) (P1,5)
// with P0 = (int(y*16+29184) << 16 | int(x*32+22528)) | 0xA00000000,
//      T1 = (height << 20) | (width << 4),
//      P1 = (int(y*16+29184 + (height*16)*vscale) << 16 |
//            int(x*32+22528 + (width*32)*uscale)) | 0xA00000000.
// The tag arithmetic is the same close-out used by _textureDrawColor: the DMA tag
// low word is ((units - 2) >> 1) | 0x10000000, which becomes tag id 6 because its
// id is 1; the other branch (append 0x60000000 and move the end 16 bytes) is
// present but cannot run. glistp moves to the end and drawAddToList receives the
// old glistp, the integer, the texture, 0 and 1.
// Inferred: the pairs are GS register writes in A+D form (3 UV, 5 XYZ2, 1 RGBAQ,
// 0x14 TEX1_1, 0x42 ALPHA_1, 0x47 TEST_1) and the two corner pairs draw a
// sprite; these names are the GS register map, not names from the image.
// Unknown: the unit of the coordinates and the meaning of the integer argument.
struct Texture;

extern "C" {
extern unsigned char* glistp;
extern unsigned char* gstart;
extern int GLISTSIZE;
void drawAddToList__FPviP8_textureP11_drawRecordi(void* packet, int order, Texture* texture, void* record, int flag);

void _textureDrawFloat__FP8_textureffiUiff(Texture* texture, float x, float y, int order, unsigned color,
                                           float uscale, float vscale) {
    unsigned long long* const start = reinterpret_cast<unsigned long long*>(glistp);
    if (GLISTSIZE - (static_cast<int>(glistp - gstart) >> 3) < 3000) return;

    unsigned long long* const end = start + 26;
    const short* const size = reinterpret_cast<const short*>(texture);
    const int width = size[0];
    const int height = size[1];

    float py = y * 16.0f + 29184.0f;
    float px = x * 32.0f + 22528.0f;
    float py1 = py + (float)(height << 4) * vscale;
    float px1 = px + (float)(width << 5) * uscale;
    const long long z = 0xa000ll << 20;
    const long long p0 = (((long long)(int)py << 16) | (long long)(int)px) | z;
    const long long p1 = (((long long)(int)py1 << 16) | (long long)(int)px1) | z;
    const long long t1 = (long long)(height << 20) | (long long)(width << 4);

    unsigned* const words = reinterpret_cast<unsigned*>(start);
    words[2] = 0x11000000u;
    unsigned* const direct = words + 3;
    *direct = 0x50000000u;
    unsigned long long* const gif = start + 2;
    gif[0] = 0x1000000000008000ull;
    gif[1] = 14ull;
    start[4] = 0ull;           start[5] = 0x3full;
    start[6] = 0ull;           start[7] = 6ull;
    start[8] = 0x61ull;        start[9] = 0x14ull;
    start[10] = color;         start[11] = 1ull;
    start[12] = 0x44ull;       start[13] = 0x42ull;
    start[14] = 0x156ull;      start[15] = 0ull;
    start[16] = 0x3001dull;    start[17] = 0x47ull;
    start[18] = 0x80008ull;    start[19] = 3ull;
    start[20] = p0;            start[21] = 5ull;
    start[22] = t1;            start[23] = 3ull;
    start[24] = p1;            start[25] = 5ull;

    const unsigned long long tag = static_cast<unsigned>(((static_cast<int>(end - start) - 2) >> 1) | 0x10000000);
    start[0] = tag;
    gif[0] |= static_cast<long long>((static_cast<int>(end - gif) - 2) >> 1);
    *direct |= static_cast<unsigned>((static_cast<int>(reinterpret_cast<unsigned char*>(end) - reinterpret_cast<unsigned char*>(direct)) - 4) >> 4);
    unsigned long long* last = end;
    if (((static_cast<long long>(tag) >> 28) & 7) == 1) {
        start[0] = (tag & 0x8fffffffull) | 0x60000000ull;
    } else {
        last[1] = 0ull;
        last[0] = 0x60000000ull;
        last += 2;
    }
    unsigned char* const previous = glistp;
    glistp = reinterpret_cast<unsigned char*>(last);
    drawAddToList__FPviP8_textureP11_drawRecordi(previous, order, texture, nullptr, 1);
}
}
