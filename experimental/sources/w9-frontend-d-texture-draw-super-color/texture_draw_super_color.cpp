// Observed textureDrawSuperColor at 0x0016e860, 936 bytes, in the pinned
// SLUS-20565 image. Argument order after the texture pointer: three ints
// (x, y and a list selector passed on to drawAddToList, in the order they are
// forwarded), an unsigned colour, four ints (clip x0, clip y0, clip x1,
// clip y1) and two signed bytes (flip flags). The meanings of the first two
// ints and the flips follow from how they reach drawTexture and the swaps.
//
// Draw list: glistp (0x004844c8) addresses the list, gstart (0x004844c4) is
// its origin and GLISTSIZE (0x00484468) its size in 8-byte units. When fewer
// than 3000 units remain nothing is done. Otherwise a packet is built at
// glistp, P:
//   P+0    64-bit tag, written last (see below);
//   P+8    VIF words 0x11000000 and 0x50000000; the second gets a quadword
//          count or-ed in;
//   P+16   GIF tag 0x1000000000008000, a loop count or-ed in later, then the
//          register word 0xe (A+D);
//   then A+D pairs (data, register): (0, 0x3f), (0, 6), (1, 20), (colour, 1),
//   (0x44, 0x42), (342, 0), (0x3001d, 0x47), and a clip rectangle with
//   register 0x40 at P+144. The cursor then stands at P+160.
// The clip value is built from x0, x1 (limited to 640), y0 and y1 where
// engineScreenSlide is subtracted from both y values, y0 is raised to zero,
// y1 is limited to 447 and x0 is raised to zero. Without supersampleMode it
// is x0 | x1 << 16 | y0 << 32 | y1 << 48. With it, x0 and x1 are doubled
// (x1 minus one), and the y1 field is the doubled
// (y1 >> 1) + (engineGetField() & 1) * (y1 & 1), minus one, with y0 unscaled
// in bits 32 up.
// The texture's two halfwords at +0 and +2 become the last two of four integer
// arguments for drawTexture, the first two being zero; a nonzero first flip
// swaps the first and third, a nonzero second flip the second and fourth.
// drawTexture is called with a four-pointer packet record on the stack
// {cursor, P, P+16, P+12}. Afterwards the clip is reset by a quadword at the
// cursor (SCAX 0 to 1279, SCAY engineScissory1 to engineScissory2, register
// 0x40), the cursor advances 16 bytes, the loop count, the VIF count and the
// DMA tag are filled in as in rectangleDraw, glistp becomes the end, and
// drawAddToList(P, list, texture, 0, 1) is called. A tag id other than 1 is
// unreachable here; the original has the branch and it is kept.
// Inferred: the field names of nothing; the stack-argument order. Unknown:
// what the first two ints are in screen terms, and what the 0x44 and 0x156
// values select.
struct TextureHeader {
    short half0;
    short half2;
};

struct DrawPacketRecord {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* gifTag;
    unsigned char* vifWord;
};

typedef unsigned long long __attribute__((may_alias)) U64;
typedef unsigned int __attribute__((may_alias)) U32;

extern "C" unsigned long glistp;
extern "C" unsigned long gstart;
extern "C" int GLISTSIZE;
extern "C" int engineScreenSlide;
extern "C" int engineScissory1;
extern "C" int engineScissory2;
extern "C" signed char supersampleMode;
extern "C" int engineGetField__Fv();
extern "C" void swap__H1Zi_RX01T0_v(int& first, int& second);
extern "C" void drawTexture__FR6PacketP8_textureiiiiii(DrawPacketRecord* packet, TextureHeader* texture, int a, int b,
                                                       int c, int d, int e, int f);
extern "C" void* drawAddToList__FPviP8_textureP11_drawRecordi(void* packet, int list, TextureHeader* texture,
                                                              void* record, int flags);

static inline void put(unsigned char* base, int offset, unsigned long long value) {
    *reinterpret_cast<U64*>(base + offset) = value;
}

extern "C" void textureDrawSuperColor__FP8_textureiiiUiiiiicc(TextureHeader* texture, int x, int y, int list,
                                                               unsigned color, int clipX0, int clipY0, int clipX1,
                                                               int clipY1, signed char flipFirst,
                                                               signed char flipSecond) {
    int used = static_cast<int>(glistp - gstart) >> 3;
    if (GLISTSIZE - used < 3000) return;
    unsigned char* packet = reinterpret_cast<unsigned char*>(glistp);

    int bottom = clipY1 - engineScreenSlide;
    int top = clipY0 - engineScreenSlide;
    if (clipX1 > 640) clipX1 = 640;
    if (bottom > 447) bottom = 447;
    if (clipX0 < 0) clipX0 = 0;
    if (top < 0) top = 0;

    put(packet, 16, 0x1000000000008000ull);
    put(packet, 24, 0xeull);
    put(packet, 32, 0ull);
    put(packet, 40, 0x3full);
    put(packet, 48, 0ull);
    put(packet, 56, 6ull);
    put(packet, 64, 1ull);
    put(packet, 72, 20ull);
    put(packet, 96, 0x44ull);
    put(packet, 104, 0x42ull);
    put(packet, 112, 342ull);
    put(packet, 120, 0ull);
    put(packet, 128, 0x3001dull);
    put(packet, 136, 0x47ull);
    *reinterpret_cast<U32*>(packet + 8) = 0x11000000u;
    *reinterpret_cast<U32*>(packet + 12) = 0x50000000u;
    *reinterpret_cast<U32*>(packet + 80) = color;
    *reinterpret_cast<U32*>(packet + 84) = 0u;
    put(packet, 88, 1ull);

    DrawPacketRecord record;
    record.start = packet;
    record.gifTag = packet + 16;
    record.vifWord = packet + 12;
    record.cursor = packet + 144;

    unsigned long long clip;
    if (supersampleMode != 0) {
        int field = engineGetField__Fv();
        int x0 = clipX0 << 1;
        int x1 = (clipX1 << 1) - 1;
        int y1 = (bottom >> 1) + (field & 1) * (bottom & 1);
        y1 = (y1 << 1) - 1;
        clip = static_cast<unsigned long long>(static_cast<long long>(x0)) |
               (static_cast<unsigned long long>(static_cast<long long>(y1)) << 48) |
               (static_cast<unsigned long long>(static_cast<long long>(top)) << 32) |
               (static_cast<unsigned long long>(static_cast<long long>(x1)) << 16);
        put(packet, 144, clip);
        put(packet, 152, 64ull);
        record.cursor = packet + 160;
    } else {
        clip = static_cast<unsigned long long>(static_cast<long long>(clipX0)) |
               (static_cast<unsigned long long>(static_cast<long long>(bottom)) << 48) |
               ((static_cast<unsigned long long>(static_cast<long long>(top)) << 32) |
                (static_cast<unsigned long long>(static_cast<long long>(clipX1)) << 16));
        put(packet, 144, clip);
        put(packet, 152, 64ull);
        record.cursor = packet + 160;
    }

    int uv[4];
    uv[0] = 0;
    uv[1] = 0;
    uv[2] = texture->half0;
    uv[3] = texture->half2;
    if (flipFirst != 0) swap__H1Zi_RX01T0_v(uv[0], uv[2]);
    if (flipSecond != 0) swap__H1Zi_RX01T0_v(uv[1], uv[3]);
    drawTexture__FR6PacketP8_textureiiiiii(&record, texture, x, y, uv[0], uv[1], uv[2], uv[3]);

    unsigned char* cursor = record.cursor;
    unsigned char* end = cursor + 16;
    unsigned char* start = record.start;
    unsigned char* gifTag = record.gifTag;
    unsigned char* vifWord = record.vifWord;
    unsigned long long reset = (static_cast<unsigned long long>(static_cast<long long>(engineScissory2)) << 48) |
                               ((static_cast<unsigned long long>(static_cast<long long>(engineScissory1)) << 32) |
                                0x04ff0000ull);
    put(cursor, 0, reset);
    put(cursor, 8, 64ull);
    unsigned long long tag =
        static_cast<unsigned long long>(static_cast<long long>(((static_cast<int>(end - start) >> 3) - 2) >> 1)) |
        0x10000000ull;
    int gifCount = ((static_cast<int>(end - gifTag) >> 3) - 2) >> 1;
    int vifCount = (static_cast<int>(end - vifWord) - 4) >> 4;
    unsigned long long gifOld = *reinterpret_cast<U64*>(gifTag);
    unsigned vifOld = *reinterpret_cast<U32*>(vifWord);
    *reinterpret_cast<U64*>(gifTag) = static_cast<unsigned long long>(static_cast<long long>(gifCount)) | gifOld;
    *reinterpret_cast<U32*>(vifWord) = vifOld | static_cast<unsigned>(vifCount);
    unsigned long long idField = static_cast<unsigned long long>(static_cast<long long>(tag) >> 28) & 7ull;
    put(start, 0, tag);
    if (idField == 1) {
        put(start, 0, (tag & 0x8fffffffull) | 0x60000000ull);
    } else {
        put(end, 0, 0x60000000ull);
        put(end, 8, 0ull);
        end = cursor + 32;
    }
    glistp = reinterpret_cast<unsigned long>(end);
    drawAddToList__FPviP8_textureP11_drawRecordi(packet, list, texture, 0, 1);
}
