// Observed printStat__FiPUsiiii at 0x001bc708, 576 bytes, 592-byte frame. Arguments: a column index `slot`, a UString
// label, an integer value, a layout selector `mode`, and two flags `centered` and `shrink`. It prints the label and the
// value of one row of the stats screen with smallFont:
//  1. base = 320 * slot + 128. lineA = 22 and lineB = 11 when the signed byte `language` is 1 or 4, otherwise both 0.
//  2. Three positions are chosen from the mode (left, right, y):
//       mode below -5: left = base + lineA - 10, right = base - 75, y = 179 for mode -6 and 158 otherwise;
//       mode 1 to 4:   left = base + 4, right = base + 51, y = 317 + 20 * mode;
//       mode -2:       left = base - 43, right = base - 47, y = 286;
//       any other:     left = base + lineB - 48, right = base - 89, y = 257.
//     When `shrink` is nonzero y is lowered by 56.
//  3. The label is formatted into a local text buffer with usprintf(buffer, label, 0.0 eight times), and printed
//     right-aligned to `left` (fontPrintu at left minus fontStringSizeu width, with width limit 1000000).
//  4. The value is formatted with usprintf(buffer, "%d", (float)value, 0.0 seven times). When `centered` is zero it is
//     printed right-aligned to `right`; otherwise it is printed centered with fontPrintCenteredu at left + right (minus
//     320 when slot is nonzero).
// Nothing is returned.
// Inferred: a row of the stats table (label on the left, number on the right), with per-language spacing; the value is
// converted to a float because the formatter takes floating arguments, as the image does.
// Unknown: which stat each mode value stands for, the meaning of lineA and lineB, and the origin of "%d" being a
// plain narrow string handed to the wide formatter.
typedef unsigned short UString;

extern "C" {
extern signed char language;
extern unsigned smallFont;

void usprintf__FPUsT0ffffffff(UString* destination, UString* format, float a, float b, float c, float d, float e, float f,
                              float g, float h);
void usprintf__FPUsPcffffffff(UString* destination, const char* format, float a, float b, float c, float d, float e, float f,
                              float g, float h);
int fontStringSizeu__FP4FontPUsi(void* font, UString* text, int width);
void fontPrintu__FiiPUsi(int x, int y, UString* text, int width);
void fontPrintCenteredu__FiiPUsi(int x, int y, UString* text, int width);

void printStat__FiPUsiiii(int slot, UString* label, int value, int mode, int centered, int shrink) {
    UString text[256];
    void* const font = reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(smallFont));
    const int base = slot * 320 + 128;
    int lineA = 0, lineB = 0;
    if (language == 4 || language == 1) {
        lineA = 22;
        lineB = 11;
    }
    int left, right, y;
    if (mode < -5) {
        left = base + lineA - 10;
        right = base - 75;
        y = mode == -6 ? 179 : 158;
    } else if (static_cast<unsigned>(mode - 1) < 4u) {
        right = base + 51;
        left = base + 4;
        y = 317 + mode * 20;
    } else if (mode == -2) {
        right = base - 47;
        left = base - 43;
        y = 286;
    } else {
        right = base - 89;
        left = base + lineB - 48;
        y = 257;
    }
    usprintf__FPUsT0ffffffff(text, label, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    if (shrink != 0) y -= 56;
    const int labelWidth = fontStringSizeu__FP4FontPUsi(font, text, 1000000);
    fontPrintu__FiiPUsi(left - labelWidth, y, text, 1000000);
    usprintf__FPUsPcffffffff(text, "%d", static_cast<float>(value), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    if (centered == 0) {
        const int width = fontStringSizeu__FP4FontPUsi(font, text, 1000000);
        fontPrintu__FiiPUsi(right - width, y, text, 1000000);
    } else if (slot != 0) {
        fontPrintCenteredu__FiiPUsi(left + right - 320, y, text, 1000000);
    } else {
        fontPrintCenteredu__FiiPUsi(left + right, y, text, 1000000);
    }
}
}
