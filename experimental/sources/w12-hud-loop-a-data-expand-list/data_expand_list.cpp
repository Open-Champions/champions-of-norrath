// Observed expandList at 0x007940b0, 320 bytes, section .bss (local b).
// A local uninitialised object with no ELF size. The next symbol, sprPacket, starts 0x140 = 320 bytes later. floorAdd__FG6Point3sfii and floorDraw__Fv take its address.
// Inferred: 320 bytes taken from the gap to the next symbol; the record layout is owned by the floor code, so it is exposed as bytes here.
// Unknown: The record size and count (not derivable from the gap alone) and whether the true extent is 320.
extern "C" {
alignas(8) unsigned char expandList[320];
}
