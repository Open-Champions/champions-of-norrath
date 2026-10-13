// Observed C string at 0x477598 in .rodata. Callers pass it as text.
// The view ends at the terminating NUL. Padding bytes after that NUL are a
// placement gap, not reads through this symbol.
extern "C" char norrath_ctors_lava_wraith_view_477598[];
char norrath_ctors_lava_wraith_view_477598[] = "maledarkelfsoldier";
