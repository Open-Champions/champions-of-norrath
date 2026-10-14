// Observed fontLastPrintedPosition at 0x00484120, a 4-byte word that is zero in the
// image. fontPrintWrapu__FiiPUsiiii clears it on entry and stores the running
// character total after each printed line. The zero word was placed in the
// uninitialized region by this compiler.
extern "C" int fontLastPrintedPosition;
int fontLastPrintedPosition = 0;
