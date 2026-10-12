// Observed fontScissor__Fiiii at 0x00172e88, 256 bytes.
// Arguments: x0, y0, x1, y1. It closes the glyph run opened by fontStart__FP4FontiUii
// and starts a new one with a clipping rectangle.
// 1. The 16-bit loop count of the GIF tag that reglistLoopCount (0x00486994)
//    points to grows by twice nmChars (0x00486998).
// 2. The rectangle is clamped: x1 to at most 640, y1 to at most 447, x0 and y0
//    to at least 0 (x0 and y0 have no upper bound, x1 and y1 no lower bound).
//    The value is (2 * x0) | ((2 * x1 - 1) << 16) | (y0 << 32) | (y1 << 48).
// 3. nmDirectQWords (0x0048699c) grows by twice nmChars, then nmChars is cleared.
// 4. At the write cursor C (word p+0 of the object p at 0x006e93d0) a new GIF
//    tag is written: 0x1000000000000001 with register word 0xe, then at C+16 the
//    rectangle value with register number 0x40, then at C+32 a glyph tag
//    0x2400000000008000 with register word 0x53. p+0 becomes C+48 and p+8
//    becomes C. reglistLoopCount points to C+32, scissored (0x004869a4) is set to
//    1, and nmDirectQWords grows by 3.
// The register numbers are the values in the image; their GS meaning is not
// claimed here. All objects named here are defined elsewhere.
extern "C" {
struct FontPacketState {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* first_tag;
};
extern FontPacketState p;
extern int nmChars;
extern int scissored;
extern unsigned char* reglistLoopCount;
extern int nmDirectQWords;
}

static void put64(unsigned char* where, unsigned long long value) {
    *reinterpret_cast<unsigned long long*>(where) = value;
}

extern "C" void fontScissor__Fiiii(int x0, int y0, int x1, int y1) {
    unsigned short* loops = reinterpret_cast<unsigned short*>(reglistLoopCount);
    *loops = static_cast<unsigned short>(*loops + nmChars * 2);
    if (x1 >= 641) x1 = 640;
    if (y1 >= 448) y1 = 447;
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    const unsigned long long rectangle =
        static_cast<unsigned long long>(static_cast<long long>(x0 << 1)) |
        (static_cast<unsigned long long>(static_cast<long long>(y1)) << 48) |
        (static_cast<unsigned long long>(static_cast<long long>(y0)) << 32) |
        (static_cast<unsigned long long>(static_cast<long long>((x1 << 1) - 1)) << 16);
    unsigned char* cursor = p.cursor;
    nmDirectQWords += nmChars * 2;
    nmChars = 0;
    put64(cursor + 0, 0x1000000000000000ULL);
    put64(cursor + 8, 0xe);
    put64(cursor + 16, rectangle);
    put64(cursor + 24, 0x40);
    p.cursor = cursor + 48;
    *reinterpret_cast<unsigned long long*>(cursor) |= 1;
    p.first_tag = cursor;
    reglistLoopCount = cursor + 32;
    put64(cursor + 40, 0x53);
    put64(cursor + 32, 0x2400000000008000ULL);
    scissored = 1;
    nmDirectQWords += 3;
}
