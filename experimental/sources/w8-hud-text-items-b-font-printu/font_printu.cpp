// Observed fontPrintu__FiiPUsi at 0x001744b8, 28 bytes.
// It calls _fontPrintu__FiiPUsi at 0x00173958 with its four arguments in the
// same registers and ignores any result. The printing routine is not defined
// here.
extern "C" void _fontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int color);

extern "C" void fontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int color) {
    _fontPrintu__FiiPUsi(x, y, text, color);
}
