// Observed: MC_Debug at 0x00485aac is 1 byte, initial value 0, in .sdata.
// Inferred: nothing.
// Unknown: which read treats a nonzero byte as the debug card.
extern "C" {
unsigned char MC_Debug;
}
