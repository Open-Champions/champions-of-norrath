// Observed: sConvert16 at 0x011e5418 is a file-local symbol with ELF size 0. The next symbol, sMessagePool, is at
// 0x011e5c18, 2048 bytes later, and all 2048 bytes in the original image are zero.
// Inferred: a static scratch buffer for 8-bit to 16-bit string conversion (the name), 2048 bytes long when the
// extent is taken from the next symbol; it is declared here as 1024 sixteen-bit units. Zero initialised, so it is
// emitted in BSS by this compiler.
// Unknown: the true element type and size (the 2048-byte extent is a bound, not a measured length), and the
// readers.
unsigned short sConvert16[1024];
