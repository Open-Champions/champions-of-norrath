// Observed fontPrint__FiiPci at 0x001743c8, 88 bytes.
// The last argument is a character count (inferred: fontPrintWrapu__FiiPUsiiii passes
// a count in that position); it is not a colour.
// The 2048-byte frame holds a 1024-element buffer of 16-bit characters.
// The narrow text is widened into that buffer with ustrcpy__FPUsPc, then the
// wide printer fontPrintu__FiiPUsi is called with the original x, y and last
// argument and the buffer. The buffer capacity is the frame layout; this unit
// does not bound the input. Neither callee is defined here.
extern "C" void ustrcpy__FPUsPc(unsigned short* dst, const char* src);
extern "C" void fontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int count);

extern "C" void fontPrint__FiiPci(int x, int y, const char* text, int count) {
    unsigned short wide[1024];
    ustrcpy__FPUsPc(wide, text);
    fontPrintu__FiiPUsi(x, y, wide, count);
}
