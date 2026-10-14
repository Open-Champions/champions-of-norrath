// Observed: Ascii2Sjis__FUc at 0x0024fb08 is 264 bytes. The byte argument is classified:
//   0x30-0x39, 0x41-0x5a and 0x61-0x7a pick row 0, 1 or 2 of ascii_table (0x0042e010, three
//   4-byte rows of two half words) and return (row[0] + c - row[1]) & 0xffff;
//   0x20-0x2f, 0x3a-0x40, 0x5b-0x60 and 0x7b-0x7e return the first half word of the entry at
//   index c-0x20, c-0x3a+16, c-0x5b+23 and c-0x7b+29 of ascii_special (0x0042df88, 33 entries
//   of two half words);
//   every other byte returns 0.
// The image holds, per ascii_special entry, the double-byte code in the first half word and the
// ASCII byte it stands for in the second. The three rows hold {0x824f, 0x30}, {0x8260, 0x41} and
// {0x8281, 0x61}.
// Inferred: the result is a Shift-JIS code for the full-width form of the character, because the
// codes lie in the 0x81xx and 0x82xx lead-byte range.
// The original masks the argument to 8 bits itself; this version relies on the caller passing a
// zero-extended byte as the ABI requires, so a dirty upper argument register is not reproduced.
// Unknown: nothing about the control-code range, which returns 0.

extern "C" {
struct AsciiSpecial {
    unsigned short sjis;
    unsigned short ascii;
};
struct AsciiRow {
    unsigned short base;
    unsigned short first;
};
extern AsciiSpecial ascii_special[33];
extern AsciiRow ascii_table[3];
}

extern "C" unsigned short Ascii2Sjis__FUc(unsigned char c) {
    int special = -1;
    int row = -1;
    if (c >= 0x20 && c <= 0x2f) {
        special = c - 0x20;
    } else if (c >= 0x30 && c <= 0x39) {
        row = 0;
    } else if (c >= 0x3a && c <= 0x40) {
        special = c - 0x3a + 16;
    } else if (c >= 0x41 && c <= 0x5a) {
        row = 1;
    } else if (c >= 0x5b && c <= 0x60) {
        special = c - 0x5b + 23;
    } else if (c >= 0x61 && c <= 0x7a) {
        row = 2;
    } else if (c >= 0x7b && c <= 0x7e) {
        special = c - 0x7b + 29;
    } else {
        return 0;
    }
    if (special >= 0) {
        return ascii_special[special].sjis;
    }
    return static_cast<unsigned short>(ascii_table[row].base + c - ascii_table[row].first);
}
