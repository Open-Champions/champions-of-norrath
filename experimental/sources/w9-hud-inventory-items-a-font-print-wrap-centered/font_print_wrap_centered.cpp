// Observed fontPrintWrapCentered__FiiPciiii at 0x001740c0, 616 bytes, in the pinned
// SLUS_205.65. The byte-string twin of fontPrintWrapCenteredu__FiiPUsiiii: x (a centre),
// y, text, maximum line width, lower and upper y bound, line step; the result is the
// final y minus the starting y.
// Segmenting and line breaking are as described for the wide version (terminator or
// newline stops a segment, a space is included, language == 5 gives one-character
// segments, a newline adds 1000000 to the carry), with widths from
// _fontStringSize__FP4FontPci(curFont, start, count).
// Printing a line (only when lower < y < upper): the line width is measured and
// ustrcpy__FPUsPc widens the text from the pending start to its end (not just the
// line: the whole remaining string) into a stack buffer, then
// _fontPrintu__FiiPUsi(x - width/2, y, buffer, count) is called. The image uses two
// separate stack buffers of 2048 bytes, one for lines printed inside the loop and one for
// the last line; the caller's text must therefore fit in 1023 characters from the start
// of a printed line (not checked, as in the image).
// Characters are compared as signed bytes. Empty text returns 0.
// Not executed on hardware; the callees are defined elsewhere.
extern "C" {
extern void* curFont;
extern signed char language;
int _fontStringSize__FP4FontPci(void* font, const char* text, int count);
void _fontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int count);
unsigned short* ustrcpy__FPUsPc(unsigned short* dest, const char* src);
}

extern "C" int fontPrintWrapCentered__FiiPciiii(int x, int y, const char* text, int width,
                                                int y_min, int y_max, int line_step) {
    const int start_y = y;
    const char* line = text;
    const char* pending = text;
    int carry = 0;
    while (*line != 0) {
        const char* end = line;
        if (language == 5) {
            end = line + 1;
        } else {
            for (;;) {
                const char c = *end;
                if (c == 0) break;
                if (c == ' ') {
                    ++end;
                    break;
                }
                if (c == '\n') break;
                ++end;
            }
        }
        const int word = _fontStringSize__FP4FontPci(curFont, line, static_cast<int>(end - line));
        int next_carry = carry + word;
        if (next_carry >= width) {
            if (y_min < y && y < y_max) {
                unsigned short buffer[1024];
                const int count = static_cast<int>(line - pending);
                const int half = _fontStringSize__FP4FontPci(curFont, pending, count) >> 1;
                ustrcpy__FPUsPc(buffer, pending);
                _fontPrintu__FiiPUsi(x - half, y, buffer, count);
            }
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
        if (y_min < y && y < y_max) {
            unsigned short buffer[1024];
            const int count = static_cast<int>(line - pending);
            const int half = _fontStringSize__FP4FontPci(curFont, pending, count) >> 1;
            ustrcpy__FPUsPc(buffer, pending);
            _fontPrintu__FiiPUsi(x - half, y, buffer, count);
        }
        y += line_step;
    }
    return y - start_y;
}
