// Observed rectangleDraw__FiiiiiUi at 0x0016ec08, 976 bytes.
// Arguments: x0, y0, x1, y1 (screen coordinates), a list selector passed on to
// drawAddToList, and a 32-bit colour. The draw list is addressed by glistp
// (0x004844c8, a pointer word) with gstart (0x004844c4) as its origin and
// GLISTSIZE (0x00484468) as its size in 8-byte units. When fewer than 3000
// units remain, ((glistp - gstart) >> 3 subtracted from GLISTSIZE), nothing is
// done. Otherwise a packet is built at glistp:
//   +0   64-bit DMA tag, written last: quadword count 7, id 1 (id changed to 6
//        in place when the id field is 1, which it always is here);
//   +8   VIF words 0x11000000 and 0x50000007;
//   +16  GIF tag 0x1000000000008000 with 6 added to its low word, then the
//        register word 0xe (A+D);
//   +32  A+D data: colour in the low word, 0 above, register 1;
//   +48  data 0x44, register 0x42;
//   +64  data 0x46, register 0;
//   +80  data 0x31001, register 0x47;
//   +96  data from the first corner, register 5;
//   +112 data from the second corner, register 5.
// A corner is x | (y << 16) | (10 << 32) in 64 bits, where x and y are the
// scaled coordinates and the integers keep their sign extension. With the byte
// supersampleMode (0x004844ac) nonzero x is (x << 5) + 22528, otherwise
// (x << 4) + 27648; y is always (y << 4) + 29184. A tag id other than 1 would
// leave the tag unchanged and append a quadword (0x60000000, 0) after the
// packet, advancing glistp by one more quadword; that branch is present in the
// original code but unreachable. glistp is advanced past the
// packet before drawAddToList is called with the old glistp, the list
// selector, and three zeros; its result is ignored.
extern "C" unsigned long glistp;
extern "C" unsigned long gstart;
extern "C" int GLISTSIZE;
extern "C" signed char supersampleMode;
extern "C" void* drawAddToList__FPviP8_textureP11_drawRecordi(void* packet, int list, void* texture,
                                                              void* record, int flags);

extern "C" void rectangleDraw__FiiiiiUi(int x0, int y0, int x1, int y1, int list, unsigned color) {
    int used = static_cast<int>(glistp - gstart) >> 3;
    if (GLISTSIZE - used < 3000) return;
    unsigned char* packet = reinterpret_cast<unsigned char*>(glistp);
    unsigned long long* quads = reinterpret_cast<unsigned long long*>(packet);
    unsigned* words = reinterpret_cast<unsigned*>(packet);
    words[2] = 0x11000000u;
    words[3] = 0x50000007u;
    quads[2] = 0x1000000000008000ull;
    quads[3] = 0xeull;
    words[8] = color;
    words[9] = 0;
    quads[5] = 1ull;
    quads[7] = 0x42ull;
    quads[6] = 0x44ull;
    quads[8] = 0x46ull;
    quads[9] = 0ull;
    quads[11] = 0x47ull;
    quads[10] = 0x31001ull;
    unsigned char* cursor = packet + 96;
    int sx0, sy0, sx1, sy1;
    if (supersampleMode != 0) {
        sx0 = (x0 << 5) + 22528;
        sy0 = (y0 << 4) + 29184;
        sx1 = (x1 << 5) + 22528;
        sy1 = (y1 << 4) + 29184;
    } else {
        sx0 = (x0 << 4) + 27648;
        sy0 = (y0 << 4) + 29184;
        sx1 = (x1 << 4) + 27648;
        sy1 = (y1 << 4) + 29184;
    }
    unsigned long long* corner = reinterpret_cast<unsigned long long*>(cursor);
    unsigned long long z = 0xa000ull << 20;
    corner[0] = (static_cast<unsigned long long>(static_cast<long long>(sx0)) |
                 (static_cast<unsigned long long>(static_cast<long long>(sy0)) << 16)) | z;
    corner[1] = 5ull;
    corner[2] = (static_cast<unsigned long long>(static_cast<long long>(sx1)) |
                 (static_cast<unsigned long long>(static_cast<long long>(sy1)) << 16)) | z;
    corner[3] = 5ull;
    unsigned char* end = cursor + 32;
    unsigned long long tag =
        ((static_cast<unsigned long long>(static_cast<long long>(
              ((static_cast<int>(end - packet) >> 3) - 2) >> 1))) |
         0x10000000ull);
    unsigned long long* gif = reinterpret_cast<unsigned long long*>(packet + 16);
    long long count = ((static_cast<int>(end - (packet + 16)) >> 3) - 2) >> 1;
    unsigned long long gifOld = gif[0];
    gif[0] = static_cast<unsigned long long>(count) | gifOld;
    quads[0] = tag;
    if (((static_cast<long long>(tag) >> 28) & 7) == 1) {
        quads[0] = (tag & 0x8fffffffull) | 0x60000000ull;
    } else {
        unsigned long long* extra = reinterpret_cast<unsigned long long*>(end);
        extra[0] = 0x60000000ull;
        extra[1] = 0ull;
        end += 16;
    }
    glistp = reinterpret_cast<unsigned long>(end);
    drawAddToList__FPviP8_textureP11_drawRecordi(packet, list, 0, 0, 0);
}
