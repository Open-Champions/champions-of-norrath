// Observed fontStart__FP4FontiUii at 0x00172bc8, 504 bytes.
// Arguments: a font, a bucket value, a colour word and a flag word. They are
// stored in the words curFont (0x0048698c), drawBucket (0x004869a8),
// lastFontColor (0x00486988) and fontCharAlpha (0x00484118). Also written:
// restrictChars = 100000, nmChars = 0, scissored = 0.
// A packet is opened at the pointer word glistp (0x004844c8), whose old value is
// the packet start S. The 16-byte object named p (0x006e93d0) holds three
// pointers afterwards: p+0 the write cursor, p+4 the packet start and p+8 the
// start of the first GIF tag; the word at p+12 is not written here. Layout:
//   S+0   eight bytes left for a DMA tag (filled by fontEnd__Fv)
//   S+8   word 0x11000000
//   S+12  zero bytes until the address is 12 mod 16, then D at that address:
//   D+0   word 0x50000000 (a DIRECT-style code whose 16-bit count is filled in
//         later), saved as the pointer word vifDirectQWordCount (0x00486990)
//   D+4   a 16-byte aligned GIF tag G: 0x1000000000000007 and register word 0xe,
//         followed by seven address/data pairs of 16 bytes each (data first,
//         register number second):
//         data 0 reg 0x3f; data 0 reg 6; data 1 reg 0x14; data colour reg 1
//         (the high word of that data is 0); data 0x44 reg 0x42; data 0x156
//         reg 0; data 0x3001d reg 0x47
//   G+128 a second GIF tag 0x2400000000008000 with register word 0x53, saved
//         as the pointer word reglistLoopCount (0x00486994)
//   G+144 the write cursor for glyph data (p+0)
// nmDirectQWords (0x0048699c) becomes 9. The register numbers are the values in
// the image; their meaning as GS registers is inferred from the numbers and is
// not claimed here. The count fields in both tags and the DMA tag are filled by
// fontScissor__Fiiii and fontEnd__Fv. All objects named here are defined
// elsewhere.
extern "C" {
struct FontPacketState {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* first_tag;
};
extern FontPacketState p;
extern unsigned char* glistp;
extern void* curFont;
extern int drawBucket;
extern int restrictChars;
extern int fontCharAlpha;
extern int nmChars;
extern unsigned lastFontColor;
extern int scissored;
extern unsigned char* vifDirectQWordCount;
extern unsigned char* reglistLoopCount;
extern int nmDirectQWords;
}

static void put64(unsigned char* where, unsigned long long value) {
    *reinterpret_cast<unsigned long long*>(where) = value;
}

extern "C" void fontStart__FP4FontiUii(void* font, int size, unsigned color, int flags) {
    unsigned char* start = glistp;
    curFont = font;
    drawBucket = size;
    restrictChars = 100000;
    fontCharAlpha = flags;
    nmChars = 0;
    lastFontColor = color;
    *reinterpret_cast<unsigned*>(start + 8) = 0x11000000;
    unsigned char* direct = start + 12;
    p.cursor = direct;
    p.start = start;
    while ((reinterpret_cast<unsigned long>(direct) & 0xf) != 12) *direct++ = 0;
    unsigned char* tag = direct + 4;
    put64(tag + 0, 0x1000000000000000ULL);
    put64(tag + 8, 0xe);
    put64(tag + 16, 0);
    put64(tag + 24, 0x3f);
    put64(tag + 32, 0);
    put64(tag + 40, 6);
    put64(tag + 48, 1);
    put64(tag + 56, 0x14);
    put64(tag + 72, 1);
    put64(tag + 80, 0x44);
    put64(tag + 88, 0x42);
    put64(tag + 96, 0x156);
    put64(tag + 104, 0);
    put64(tag + 112, 0x3001d);
    put64(tag + 120, 0x47);
    unsigned char* reglist = tag + 128;
    vifDirectQWordCount = direct;
    put64(reglist + 8, 0x53);
    put64(reglist + 0, 0x2400000000008000ULL);
    *reinterpret_cast<unsigned*>(tag + 64) = color;
    *reinterpret_cast<unsigned*>(tag + 68) = 0;
    *reinterpret_cast<unsigned long long*>(tag) |= 7;
    *reinterpret_cast<unsigned*>(direct) = 0x50000000;
    p.cursor = tag + 144;
    p.first_tag = tag;
    reglistLoopCount = reglist;
    nmDirectQWords = 9;
    scissored = 0;
}
