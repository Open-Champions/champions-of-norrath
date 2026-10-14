// Observed: MCA_ErrorMsg is one 32-bit pointer at 0x00485c10 in initialized data (symbol size 4). The file
// word is 0. Only serviceResult__FP8queueCmdPi stores to it (through its gp-relative address): at entry it
// stores the address of an empty string, and for failed write-length checks the string "Save Data Is Corrupt.".
// No reader was found in the text by gp-relative loads.
// Inferred: a pointer to the text of the last memory card error. Opened by the w9-other-b service-result
// unit as an import; this definition closes it.
// Unknown: whether anything reads it through another addressing form.

extern "C" const char* MCA_ErrorMsg = 0;
#if defined(__mips__)
static_assert(sizeof(MCA_ErrorMsg) == 4, "MCA_ErrorMsg");
#endif
