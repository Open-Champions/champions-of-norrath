// Observed drawUpFrom__FiiPCUcT2iUi at 0x002ca660, 624 bytes. Arguments: a0 = x, a1 = y,
// a2 = text (byte string, may be null), a3 = label (byte string, may be null), t0 = an int
// (call it limit), t1 = an unsigned colour. Two 256-halfword stack buffers are used, one
// for the text (A) and one for the label (B); bytes are widened to halfwords with a zero
// terminator and there is no length check.
//  1. Text null: A = " " (a single 32), B = empty.
//     Label non-null: B = label + ": " (58, 32), A = text.  Label null: B = "> " (62, 32),
//     A = text.
//  2. w = fontStringSizeu(smallFont, B, 1000000), where smallFont is a pointer variable read through gp.
//  3. left = x + w and room = limit - w. fontPrintWrapu(left, y, A, room, 0, 0, 21) returns a
//     height h; the new y is y - h.
//  4. fontColor(colour, -1, 0), fontPrintWrapu(x, newY, B, limit, 230, 580, 21).
//  5. A dimmed colour is built bytewise: bytes 0, 1 and 2 are halved (shifted right one) and
//     byte 3 is kept. fontColor(dimmed, -1, 0), fontPrintWrapu(left, newY, A, room, 230, 580,
//     21). The return value is newY.
// The first fontPrintWrapu call (zeros in the 5th and 6th slots) is observed to run before any
// fontColor call here; whether it draws or only measures is unknown. Fresh compilation is not
// an original-byte match.
extern "C" {
extern void* smallFont;
int fontStringSizeu__FP4FontPUsi(void* font, const unsigned short* text, int limit);
int fontPrintWrapu__FiiPUsiiii(int x, int y, const unsigned short* text, int width, int a, int b, int c);
void fontColor__FUiii(unsigned color, int a, int b);

int drawUpFrom__FiiPCUcT2iUi(int x, int y, const unsigned char* text, const unsigned char* label, int limit,
                             unsigned color) {
    unsigned short text_buffer[256];
    unsigned short label_buffer[256];
    if (text == 0) {
        text_buffer[0] = 32;
        text_buffer[1] = 0;
        label_buffer[0] = 0;
    } else {
        int n = 0;
        if (label != 0) {
            for (; label[n] != 0; ++n) label_buffer[n] = label[n];
            label_buffer[n] = 58;
            label_buffer[n + 1] = 32;
            label_buffer[n + 2] = 0;
        } else {
            label_buffer[0] = 62;
            label_buffer[1] = 32;
            label_buffer[2] = 0;
        }
        int m = 0;
        for (; text[m] != 0; ++m) text_buffer[m] = text[m];
        text_buffer[m] = 0;
    }
    const int width = fontStringSizeu__FP4FontPUsi(smallFont, label_buffer, 1000000);
    const int room = limit - width;
    const int left = x + width;
    const int height = fontPrintWrapu__FiiPUsiiii(left, y, text_buffer, room, 0, 0, 21);
    const int new_y = y - height;
    fontColor__FUiii(color, -1, 0);
    fontPrintWrapu__FiiPUsiiii(x, new_y, label_buffer, limit, 230, 580, 21);
    const unsigned dimmed = ((color & 0xffu) >> 1) | (color >> 24 << 24) | ((((color >> 16) & 0xffu) >> 1) << 16) |
                            ((color >> 1) & 0x7f00u);
    fontColor__FUiii(dimmed, -1, 0);
    fontPrintWrapu__FiiPUsiiii(left, new_y, text_buffer, room, 230, 580, 21);
    return new_y;
}
}
