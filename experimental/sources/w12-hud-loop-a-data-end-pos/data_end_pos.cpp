// Observed endPos at 0x008d5fd0, 16 bytes, section .bss (local b).
// A local uninitialised object with no ELF size. The next symbol, jumpGatePos, starts 16 bytes later. initJumpEffect__Fv (0x0028485c) takes its address and copies a 12-byte point of three floats into it.
// Inferred: A three-float point plus four bytes of padding; the 16-byte gap is taken as the size, zero-initialised here.
// Unknown: Run-time contents; the padding word is never written by the code seen.
extern "C" {
alignas(4) float endPos[4];
}
