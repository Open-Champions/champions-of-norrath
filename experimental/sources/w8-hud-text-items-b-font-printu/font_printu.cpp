// Observed fontPrintu__FiiPUsi at 0x001744b8, 28 bytes.
// The last argument is a character count (inferred: fontPrintWrapu__FiiPUsiiii passes
// a count in that position); it is not a colour.
// It calls _fontPrintu__FiiPUsi at 0x00173958 with its four arguments in the
// same registers and ignores any result. The printing routine is not defined
// here.
extern "C" void _fontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int count);

extern "C" void fontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int count) {
    _fontPrintu__FiiPUsi(x, y, text, count);
}
