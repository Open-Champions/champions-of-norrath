// Observed: scaleScale is one 32-bit word at 0x00484cdc in initialized data (symbol size 4). The file
// bytes are 00 00 80 3f, which is the single-precision value 1.0. gameClear__Fi and gameLoadWorld__FPc
// access it. In the image it is a local symbol of its original object; the unresolved import list needs
// it with external linkage, which this definition provides.
// Inferred: a float scale factor.
// Unknown: what it scales.

extern "C" float scaleScale = 1.0f;
static_assert(sizeof(scaleScale) == 4, "scaleScale");
