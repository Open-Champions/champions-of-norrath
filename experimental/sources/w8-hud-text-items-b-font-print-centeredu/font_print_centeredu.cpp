// Observed fontPrintCenteredu__FiiPUsi at 0x00174548, 108 bytes.
// The last argument is a character count (inferred: fontPrintWrapu__FiiPUsiiii passes
// a count in that position); it is not a colour.
// The width comes from _fontStringSizeu__FP4FontPUsi called with the word named
// curFont, the wide text and the last argument. _fontPrintu__FiiPUsi then prints
// the same wide text at x minus half the width, using an arithmetic shift for
// the half. Callees and curFont are not defined here.
extern "C" void* curFont;
extern "C" int _fontStringSizeu__FP4FontPUsi(void* font, const unsigned short* text, int count);
extern "C" void _fontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int count);

extern "C" void fontPrintCenteredu__FiiPUsi(int x, int y, const unsigned short* text, int count) {
    int width = _fontStringSizeu__FP4FontPUsi(curFont, text, count);
    _fontPrintu__FiiPUsi(x - (width >> 1), y, text, count);
}
