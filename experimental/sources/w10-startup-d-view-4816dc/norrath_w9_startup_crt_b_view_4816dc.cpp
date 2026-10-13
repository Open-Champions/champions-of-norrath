// Observed four bytes at 0x004816dc (.sdata): the little-endian word 0x3891a2b3, which as
// a single is about 7.0e-5. The word at 0x004816e0 is a different constant and is not part
// of this view. Read once by the Sparks constructor, which stores it into two floats of
// its embedded light.
// Unknown: whether the original treats it as one named variable, and what it means.
extern "C" const float norrath_w9_startup_crt_b_view_4816dc = __builtin_bit_cast(float, 0x3891a2b3u);
