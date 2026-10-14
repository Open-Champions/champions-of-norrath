// Observed _sceSifMInitCheck at 0x00447c48, pinned SLUS_205.65: a four-byte word in the initialised data
// section (a file-local symbol of the SIF library). The image holds zero. sceSifMExitRpc stores zero
// in it.
// The object is placed in .data explicitly because a zero initialiser alone would go to .bss.
// Unknown: what stores a nonzero value and what reads it; no authored routine does.
extern "C" {
#if defined(__mips__)
__attribute__((section(".data")))
#endif
int _sceSifMInitCheck = 0;
}
