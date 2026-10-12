// Observed: mcRunCmdNo at 0x00446070 is 4 bytes, a word of zero in the executable
// image. The defined object here has that extent and is zero-initialised.
// Inferred: nothing.
// Unknown: the command numbering; the symbol is local in the original.

extern "C" {
int mcRunCmdNo;
}
