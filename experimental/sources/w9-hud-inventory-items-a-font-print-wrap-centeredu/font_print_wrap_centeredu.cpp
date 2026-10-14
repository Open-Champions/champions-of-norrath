// Observed fontPrintWrapCenteredu__FiiPUsiiii at 0x00173e80, 576 bytes, in the pinned
// SLUS_205.65. Arguments: x (a centre), y, wide text, a maximum line width, a lower
// and an upper y bound, and a line step. The result is the final y minus the starting y.
// This is a word-wrapping printer like fontPrintWrapu__FiiPUsiiii without the character
// limit, and it prints each line centred on x.
// Segmenting: starting at the segment start (the line cursor), the scan stops at a
// terminator or a newline (not included) or after a space (included). When the signed
// byte named language (0x004844b4) is 5, every segment is exactly one character.
// Each segment's width (_fontStringSizeu__FP4FontPUsi with curFont) is added to a carry
// (zero at the start). If carry plus width is below the maximum width the segment joins
// the pending line. Otherwise the pending line (pending start up to the segment start) is
// output, y grows by the step, the pending start moves to the segment start, and the
// carry restarts at the segment's own width. A newline after the segment is skipped and
// 1000000 is added to the carry, which forces a break before the next segment.
// Outputting a line: only when lower < y < upper, the line width is measured and
// _fontPrintu__FiiPUsi(x - width/2, y, pending, count) is called (halved with an
// arithmetic shift). The step is added to y whether or not the line was printed.
// After the loop a non-empty pending line is output the same way (also advancing y).
// Empty text returns 0.
// Not executed on hardware; the font callees are defined elsewhere.
extern "C" {
extern void* curFont;
extern signed char language;
int _fontStringSizeu__FP4FontPUsi(void* font, const unsigned short* text, int count);
void _fontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int count);
}

static void printLine(int x, int y, const unsigned short* pending, int count) {
    const int width = _fontStringSizeu__FP4FontPUsi(curFont, pending, count);
    _fontPrintu__FiiPUsi(x - (width >> 1), y, pending, count);
}

extern "C" int fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, const unsigned short* text, int width,
                                                  int y_min, int y_max, int line_step) {
    const int start_y = y;
    const unsigned short* line = text;
    const unsigned short* pending = text;
    int carry = 0;
    while (*line != 0) {
        const unsigned short* end = line;
        if (language == 5) {
            end = line + 1;
        } else {
            for (;;) {
                const unsigned short c = *end;
                if (c == 0) break;
                if (c == ' ') {
                    ++end;
                    break;
                }
                if (c == '\n') break;
                ++end;
            }
        }
        const int word = _fontStringSizeu__FP4FontPUsi(curFont, line, static_cast<int>(end - line));
        int next_carry = carry + word;
        if (next_carry >= width) {
            if (y_min < y && y < y_max)
                printLine(x, y, pending, static_cast<int>(line - pending));
            pending = line;
            y += line_step;
            next_carry = word;
        }
        if (*end == '\n') {
            next_carry += 1000000;
            ++end;
        }
        carry = next_carry;
        line = end;
    }
    if (pending < line) {
        if (y_min < y && y < y_max)
            printLine(x, y, pending, static_cast<int>(line - pending));
        y += line_step;
    }
    return y - start_y;
}
