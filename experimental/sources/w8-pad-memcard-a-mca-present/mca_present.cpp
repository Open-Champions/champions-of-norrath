// Observed: MCA_Present at 0x0042f420 is 32 bytes, eight words of zero in the executable
// image. The defined object here has that extent and is zero-initialised.
// Inferred: indices correspond to port/slot pairs, since the clear routine writes 0 to all eight.
// Unknown: what each index maps to.

extern "C" {
int MCA_Present[8];
}
