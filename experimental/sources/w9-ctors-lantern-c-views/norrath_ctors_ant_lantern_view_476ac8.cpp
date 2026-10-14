// Observed unnamed 8-byte constant in .rodata at 0x476ac8: the IEEE double 0.3 rounded (0x3fd3333333333333), stored little-endian.
// The Cthulu constructor reads it as two words and adds it to a widened float through the software double routines.
// Unknown: whether the original source wrote this literal as 0.3; only the stored bits are observed.
extern "C" const unsigned norrath_ctors_ant_lantern_view_476ac8[2];
const unsigned norrath_ctors_ant_lantern_view_476ac8[2] = {0x33333333u, 0x3fd33333u};
