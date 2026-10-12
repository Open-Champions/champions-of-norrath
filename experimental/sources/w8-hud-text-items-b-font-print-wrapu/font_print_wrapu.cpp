// Observed fontPrintWrapu__FiiPUsiiii at 0x00173c08, 632 bytes.
// Arguments: x, y, wide text, a maximum line width, a lower and an upper y bound,
// and a line step. The result is the final y minus the starting y.
// The text is cut into segments. Starting at the segment start, the scan
// advances one character at a time while the character is not a terminator, a
// space or a newline and _fontStringSizeu__FP4FontPUsi(curFont, start, count),
// with count the characters before the scan position, is below the maximum
// width. A terminator or a newline stops the scan on that character. A space is
// included in the segment. If the measured width reaches the maximum, the scan
// steps back one character and, when that character is a space, includes it
// again. When the signed byte named language (0x004844b4) equals 5 every
// segment is exactly one character (none at the terminator).
// Each segment's width is added to a carry (zero at the start). If the sum is
// below the maximum width the segment joins the current line. Otherwise the
// pending line (from the pending start to the segment start) is output and the
// carry restarts at the segment's own width. If the character after the segment
// is a newline, it is skipped and 1000000 is added to the carry, which forces a
// break before the next segment.
// Outputting a line: the count is the smaller of restrictChars minus the characters
// already counted and the line length. The line is passed to
// _fontPrintu__FiiPUsi(x, y, pending, count) only when the lower bound is less
// than y, y is less than the upper bound and no earlier line reached
// restrictChars. The counted total grows by the count whether or not anything is
// printed, fontLastPrintedPosition (0x00484120) is set to the new total after a
// print, and an output that leaves the total at or above restrictChars stops all
// later printing. y then grows by the line step. After the loop any remaining
// pending text is output the same way.
// For empty text the original returns the starting y minus a register this routine
// does not set; the value 0 is returned here, which is inferred intent.
// The callees and the four objects are defined elsewhere.
extern "C" {
extern int fontLastPrintedPosition;
extern int restrictChars;
extern void* curFont;
extern signed char language;
int _fontStringSizeu__FP4FontPUsi(void* font, const unsigned short* text, int count);
void _fontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int count);
}

static int smaller(int a, int b) { return a < b ? a : b; }

extern "C" int fontPrintWrapu__FiiPUsiiii(int x, int y, const unsigned short* text, int width,
                                          int y_min, int y_max, int line_step) {
    const int start_y = y;
    fontLastPrintedPosition = 0;
    const unsigned short* line = text;
    const unsigned short* pending = text;
    int carry = 0;
    int stopped = 0;
    int counted = 0;
    while (*line != 0) {
        const unsigned short* end = line;
        if (language == 5) {
            if (*line != 0) end = line + 1;
        } else {
            for (;;) {
                const unsigned short c = *end;
                if (c == 0) break;
                if (c == ' ') {
                    ++end;
                    break;
                }
                if (c == '\n') break;
                if (_fontStringSizeu__FP4FontPUsi(curFont, line, static_cast<int>(end - line)) >= width) {
                    --end;
                    if (*end == ' ') ++end;
                    break;
                }
                ++end;
            }
        }
        const int word = _fontStringSizeu__FP4FontPUsi(curFont, line, static_cast<int>(end - line));
        int next_carry = carry + word;
        if (next_carry >= width) {
            const int count = smaller(restrictChars - counted, static_cast<int>(line - pending));
            if (y_min < y && y < y_max && stopped == 0) {
                _fontPrintu__FiiPUsi(x, y, pending, count);
                fontLastPrintedPosition = counted + count;
            }
            counted += count;
            if (!(counted < restrictChars)) stopped = 1;
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
        if (y_min < y && y < y_max && stopped == 0) {
            const int count = smaller(restrictChars - counted, static_cast<int>(line - pending));
            _fontPrintu__FiiPUsi(x, y, pending, count);
            fontLastPrintedPosition = counted + count;
        }
        y += line_step;
    }
    return y - start_y;
}
