// Observed: textureDrawColorSpecialHalo__FP8_textureiiiUiiiiii at 0x0016e050 is 1016 bytes in the pinned SLUS-20565 image. It is a
// sibling of textureDrawSuperColor (authored in w9-frontend-d) with these differences. Arguments after the texture pointer: three
// ints (x, y and the list selector forwarded to drawAddToList), an unsigned colour, an int 'mode', then four ints (clip x0, clip y0,
// clip x1, clip y1). There are no flip flags.
//
// Draw list: glistp (0x004844c8) addresses the list, gstart (0x004844c4) is its origin and GLISTSIZE its size in 8-byte units; when
// fewer than 3000 units remain nothing is done. Otherwise a packet is built at glistp, P:
//   P+8   VIF words 0x11000000 and 0x50000000 (the second gets a quadword count or-ed in later);
//   P+16  GIF tag 0x1000000000008000 (a loop count or-ed in later), register word 0xe (A+D);
//   then A+D pairs (data, register): (0, 0x3f) at P+32, (0, 6) at P+48, (1, 20) at P+64, (0x80808080, 1) at P+80 (the data word is
//   0x80808080 with a zero high word and the register word is at P+88), (0x48, 0x42) at P+96, (colour & 0xffffff, 0x3d) at P+112,
//   (0, 10) at P+128, (0x176, 0) at P+144, (0x1212303012123030, 0x44) at P+160, ((mode << 4) | 0x3000d, 0x47) at P+176 and the clip
//   rectangle with register 0x40 at P+192; the cursor then stands at P+208.
// Clip: y0 and y1 have engineScreenSlide subtracted; x1 of 640 or more becomes 639, y1 of 448 or more becomes 447, x0 and y0 below zero
// become zero. Without supersampleMode the clip word is x0 | x1 << 16 | y0 << 32 | y1 << 48. With it, x0 and x1 are doubled (x1
// minus one) and y1 becomes ((y1 >> 1) + (engineGetField() & 1) * (y1 & 1)) doubled minus one, y0 unscaled.
// The texture's halfwords at +0 and +2 are the last two integers for drawTexture(&record, texture, x, y, 0, 0, half0, half2), the
// record being the four pointers {cursor, P, P+16, P+12}. Afterwards the clip is reset with a quadword at the cursor (SCAX 0 to 1279,
// SCAY engineScissory1 to engineScissory2, register 0x40) preceded by a quadword (0x3131757531317575, 0x44) at the cursor, the cursor advances 32 bytes, the loop count, VIF count and DMA tag are
// filled in as in textureDrawSuperColor, glistp becomes the end and drawAddToList(P, list, texture, 0, 1) is called. A tag id other
// than 1 is unreachable here; the original has the branch and it is kept.
// Inferred: this draws a texture with a fixed grey vertex colour and the given colour masked to 24 bits in a second register, and
// 'mode' feeds a 4-bit-shifted field of a GS register word.
// Unknown: the meanings of the registers and field values, and what the callee does with them. Nothing here ran on the EE.
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
extern "C" void drawTexture__FR6PacketP8_textureiiiiii(DrawPacketRecord* packet, TextureHeader* texture, int a, int b,
                                                       int c, int d, int e, int f);
extern "C" void* drawAddToList__FPviP8_textureP11_drawRecordi(void* packet, int list, TextureHeader* texture,
                                                              void* record, int flags);

static inline void put(unsigned char* base, int offset, unsigned long long value) {
    *reinterpret_cast<U64*>(base + offset) = value;
}

extern "C" void textureDrawColorSpecialHalo__FP8_textureiiiUiiiiii(TextureHeader* texture, int x, int y, int list,
                                                                     unsigned color, int mode, int clipX0, int clipY0,
                                                                     int clipX1, int clipY1) {
    int used = static_cast<int>(glistp - gstart) >> 3;
    if (GLISTSIZE - used < 3000) return;
    unsigned char* packet = reinterpret_cast<unsigned char*>(glistp);

    int bottom = clipY1 - engineScreenSlide;
    int top = clipY0 - engineScreenSlide;
    if (clipX1 >= 640) clipX1 = 639;
    if (bottom >= 448) bottom = 447;
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
    put(packet, 96, 0x48ull);
    put(packet, 104, 0x42ull);
    put(packet, 112, static_cast<unsigned long long>(color & 0xffffffu));
    put(packet, 120, 0x3dull);
    put(packet, 128, 0ull);
    put(packet, 136, 10ull);
    put(packet, 144, 0x176ull);
    put(packet, 152, 0ull);
    put(packet, 160, 0x1212303012123030ull);
    put(packet, 168, 0x44ull);
    put(packet, 176, static_cast<unsigned long long>(static_cast<long long>((mode << 4) | 0x1000c | 0x20001)));
    put(packet, 184, 0x47ull);
    *reinterpret_cast<U32*>(packet + 8) = 0x11000000u;
    *reinterpret_cast<U32*>(packet + 12) = 0x50000000u;
    *reinterpret_cast<U32*>(packet + 80) = 0x80808080u;
    *reinterpret_cast<U32*>(packet + 84) = 0u;
    put(packet, 88, 1ull);

    DrawPacketRecord record;
    record.start = packet;
    record.gifTag = packet + 16;
    record.vifWord = packet + 12;

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
    } else {
        clip = static_cast<unsigned long long>(static_cast<long long>(clipX0)) |
               (static_cast<unsigned long long>(static_cast<long long>(bottom)) << 48) |
               ((static_cast<unsigned long long>(static_cast<long long>(top)) << 32) |
                (static_cast<unsigned long long>(static_cast<long long>(clipX1)) << 16));
    }
    put(packet, 192, clip);
    put(packet, 200, 64ull);
    record.cursor = packet + 208;

    drawTexture__FR6PacketP8_textureiiiiii(&record, texture, x, y, 0, 0, texture->half0, texture->half2);

    unsigned char* cursor = record.cursor;
    unsigned char* end = cursor + 32;
    unsigned char* start = record.start;
    unsigned char* gifTag = record.gifTag;
    unsigned char* vifWord = record.vifWord;
    unsigned long long reset = (static_cast<unsigned long long>(static_cast<long long>(engineScissory2)) << 48) |
                               ((static_cast<unsigned long long>(static_cast<long long>(engineScissory1)) << 32) |
                                0x04ff0000ull);
    put(cursor, 0, 0x3131757531317575ull);
    put(cursor, 8, 0x44ull);
    put(cursor, 16, reset);
    put(cursor, 24, 64ull);
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
        end = cursor + 48;
    }
    glistp = reinterpret_cast<unsigned long>(end);
    drawAddToList__FPviP8_textureP11_drawRecordi(packet, list, texture, 0, 1);
}
