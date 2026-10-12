// Observed _fontPrintu__FiiPUsi at 0x00173958, 684 bytes. The routine behind the
// fontPrintu__FiiPUsi wrapper and the centered and wrapped printers: it prints
// count 16-bit characters (arguments x, y, text, count) through subfontPrintu__FiiPUsi
// (0x00173408), which is a separate routine not defined here.
//
// Observed behavior, in order:
//  1. Returns without drawing when GLISTSIZE minus the number of eight-byte
//     units used in the display list, (glistp - gstart) >> 3, is below 6000.
//  2. When engineFullFrameMode (0x00484474) is zero it tail-calls subfontPrintu
//     with the same four arguments.
//  3. Otherwise it prints twice and then restores, each step inside the font
//     packet that fontStart__FP4FontiUii opened (p at 0x006e93d0: p+0 is the write
//     cursor, p+8 receives the start of the block written below):
//       a. The pending glyph count is folded in first: the 16-bit loop count at
//          reglistLoopCount grows by 2 * nmChars, nmDirectQWords grows by
//          2 * nmChars, and nmChars becomes 0.
//       b. A 48-byte block is written at the cursor: a GIF tag with NREG 1, the A+D
//          register descriptor 0xe, and one loop (0x1000000000000001, 0xe); one
//          address/data pair holding a data word and register number 0x22; and a
//          new GIF tag 0x2400000000008000 (end-of-packet, register-list mode, two
//          registers) with register word 0x53. The cursor advances by 48, p+8 is
//          set to the block start, reglistLoopCount is set to the new register-list
//          tag, scanMask (0x004869ac) is set to the data value and
//          nmDirectQWords grows by 3.
//       The data value is 2 for the first print, 3 for the second, and 0 for the
//       final restore block, which is followed by no print. Each of the first two
//       blocks is followed by subfontPrintu(x, y, text, count).
// Register 0x22 is the scan-mask register by its number; that, and the meaning of
// 0x53, are inferred from GS register numbering and not otherwise established.
// The odd/even-line reading of the values 2 and 3 is likewise inferred.
extern "C" {
struct FontPacketState {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* first_tag;
};
extern FontPacketState p;
extern unsigned char* glistp;
extern unsigned char* gstart;
extern int GLISTSIZE;
extern int engineFullFrameMode;
extern int nmChars;
extern int nmDirectQWords;
extern int scanMask;
extern unsigned char* reglistLoopCount;
void subfontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int count);
}

static void put64(unsigned char* where, unsigned long long value) {
    *reinterpret_cast<unsigned long long*>(where) = value;
}

// Folds the pending glyph count into the open tags, then appends the scan-mask block.
static inline __attribute__((always_inline)) void append_scan_mask(unsigned long long mask) {
    unsigned short* loops = reinterpret_cast<unsigned short*>(reglistLoopCount);
    *loops = static_cast<unsigned short>(*loops + nmChars * 2);
    unsigned char* block = p.cursor;
    nmDirectQWords += nmChars * 2;
    nmChars = 0;
    put64(block, 0x1000000000000000ULL);
    put64(block + 8, 0xe);
    put64(block + 16, mask);
    put64(block + 24, 0x22);
    p.first_tag = block;
    p.cursor = block + 48;
    put64(block, *reinterpret_cast<unsigned long long*>(block) | 1);
    reglistLoopCount = block + 32;
    put64(block + 40, 0x53);
    put64(block + 32, 0x2400000000008000ULL);
    scanMask = static_cast<int>(mask);
    nmDirectQWords += 3;
}

extern "C" void _fontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int count) {
    if (GLISTSIZE - ((glistp - gstart) >> 3) < 6000)
        return;
    if (engineFullFrameMode == 0) {
        subfontPrintu__FiiPUsi(x, y, text, count);
        return;
    }
    append_scan_mask(2);
    subfontPrintu__FiiPUsi(x, y, text, count);
    append_scan_mask(3);
    subfontPrintu__FiiPUsi(x, y, text, count);
    append_scan_mask(0);
}
