// Observed megatileSeed (0x00485c64) and megatileRot (0x00485c68): two adjacent
// 4-byte words in the initialized data region, both zero in the file image. They
// are read as the third and fourth arguments of a sprintf in
// compareStatePackets__FP11statePacketT0i and stored by the routines near
// 0x0025e5b4. Plain int words are used.
// Inferred: seed and rotation selectors for megatile placement, from the names
// only. Unknown: value ranges and signedness.
extern "C" {
int megatileSeed = 0;
int megatileRot = 0;
}
