// Observed at 0x00483b70: the six characters "fx.lmp" and a NUL (seven bytes). The next bytes in the image
// belong to neighbouring strings and are not part of this view. An authored caller passes
// this address to lumpFindResource__FPcT0 as a file name.
extern "C" {
char norrath_w9_startup_crt_b_view_483b70[] = "fx.lmp";
}
static_assert(sizeof(norrath_w9_startup_crt_b_view_483b70) == 7u, "norrath_w9_startup_crt_b_view_483b70 extent");
