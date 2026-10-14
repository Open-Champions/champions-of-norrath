// Observed VIEndian at 0x011b1fc8, a four-byte word that is zero in the image, in the overlay section (.srs)
// that also holds the VI file routines. The CSF file reader (Read of a 40-byte header, then the chunk table)
// byte-swaps its multi-byte header and chunk fields whenever the word is not 1.
// Inferred: 1 means the file byte order already matches the machine's and anything else means it must be
// reversed; the zero in the image therefore selects the swap. Nothing was observed writing it.
// Placed in .data explicitly because a zero initialiser alone would go to .bss; the overlay section placement
// of the original is a link-level matter and is not reproduced here.
// Unknown: who sets it, and whether it is read by anything beyond the file reader.
extern "C" {
#if defined(__mips__)
__attribute__((section(".data")))
#endif
int VIEndian = 0;
}
