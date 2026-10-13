// Storage for the jump-gate target position and the hidden flag, with addresses from the private
// image's symbol table (SLUS_205.65). Both are BSS in the image.
//   jumpGatePos  0x008d5fe0  the symbol is followed by jumpParticleOn 16 bytes later and the
//                            previous symbols (startPos, endPos) are also 16 bytes apart, so
//                            16 bytes at 16-byte alignment are reserved; the code uses three floats
//                            (x, y, z) as a Point3 and none touches a fourth word.
//   jumpHidden   0x00486da8  1 byte (the next symbol, monstersFound, is the following byte);
//                            written by jumpGateInit from its third argument, read as a signed
//                            byte; zero lets the jump effect start a lightning bolt at the player.
// Unknown: whether the fourth word of jumpGatePos is padding or used by code not yet seen.
// Nothing here ran on the EE.
extern "C" {
alignas(16) float jumpGatePos[4];
signed char jumpHidden;
}
