// Observed: numEnvironments is one 16-bit value at 0x00484cb0 in initialized data (symbol size 2). The
// file bytes are 01 00, so the value is 1. runMusic__Fv loads it as a signed halfword and
// gameLoadWorld__FPc stores a halfword.
// Inferred: a count, with signed short as the type.
// Unknown: what an environment is.

extern "C" short numEnvironments = 1;
static_assert(sizeof(numEnvironments) == 2, "numEnvironments");
