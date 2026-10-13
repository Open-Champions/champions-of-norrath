// Observed .bss symbol endPos at 0x8d5fd0. The ELF size field is 0; the next symbol (jumpGatePos)
// is 16 bytes later. initJumpEffect__Fv (0x28485c) copies a 12-byte point (three floats) into it.
// Inferred: a 3-float point plus 4 bytes of padding; the gap is taken as the size.
extern "C" {
float norrath_front_a_view_8d5fd0[4];
}
