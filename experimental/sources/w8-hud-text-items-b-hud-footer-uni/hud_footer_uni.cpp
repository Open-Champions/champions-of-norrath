// Observed hudFooterUni at 0x004283e0, 256 bytes in the initialized data region,
// all zero in the image: 128 16-bit characters. It receives the formatted footer
// text that is passed to hudShowMsg__FPUsN20iii. The zero bytes were placed in
// the uninitialized region by this compiler.
extern "C" unsigned short hudFooterUni[128];
unsigned short hudFooterUni[128];
