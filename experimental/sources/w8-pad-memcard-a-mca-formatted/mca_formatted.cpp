// Observed: MCA_Formatted at 0x0042f400 is 32 bytes, eight words of zero in the executable
// image. The defined object here has that extent and is zero-initialised.
// Inferred: indices correspond to port/slot pairs, since the clear routine writes -1 to all eight.
// Unknown: what each index maps to.

extern "C" {
int MCA_Formatted[8];
}
