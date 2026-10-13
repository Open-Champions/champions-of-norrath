// Observed view norrath_ctors_lava_wraith_view_601fd0 at 0x601fd0 in .bss.
// ELF symbol splashInfoBits has size 0; the next symbol starts 504 bytes later.
// Observed: the only caller clears exactly 504 bytes. Inferred: 126 words used as a bit set;
// WaveMaker clears 504 bytes and sets bits indexed by sample count over 32. Starts zero.
// Unknown: the meaning of a set bit beyond the caller's water-height test.
extern "C" {
unsigned norrath_ctors_lava_wraith_view_601fd0[126];
}
