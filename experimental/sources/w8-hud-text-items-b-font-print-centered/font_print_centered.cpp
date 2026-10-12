// Observed fontPrintCentered__FiiPci at 0x00174420, 124 bytes.
// The width comes from _fontStringSize__FP4FontPci called with the word named
// curFont, the narrow text and the last argument. The text is then widened into
// a 1024-element 16-bit buffer with ustrcpy__FPUsPc, and _fontPrintu__FiiPUsi
// (not the fontPrintu wrapper) prints it at x minus half the width. The half is
// an arithmetic shift, so a negative width rounds toward negative infinity.
// defined here.
extern "C" void* curFont;
extern "C" int _fontStringSize__FP4FontPci(void* font, const char* text, int color);
extern "C" void ustrcpy__FPUsPc(unsigned short* dst, const char* src);
extern "C" void _fontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int color);

extern "C" void fontPrintCentered__FiiPci(int x, int y, const char* text, int color) {
    unsigned short wide[1024];
    int width = _fontStringSize__FP4FontPci(curFont, text, color);
    ustrcpy__FPUsPc(wide, text);
    _fontPrintu__FiiPUsi(x - (width >> 1), y, wide, color);
}
