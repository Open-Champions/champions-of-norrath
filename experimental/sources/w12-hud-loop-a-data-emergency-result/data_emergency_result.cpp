// Observed emergencyResult at 0x006ca800, 1568 bytes, section .bss (local b).
// A local uninitialised object with no ELF size. The next symbol, soundLimit, starts 0x620 = 1568 bytes later. evaluateAnimation__FP14AnimationStateb (0x00161df0) stores its address into the result slot of an animation state when no result is free, and animLoadBoneMatricies (0x001628e0) compares that slot with this address, clears the slot and writes zero at offset 1552 of it.
// Inferred: A spare animation-result record at least 1556 bytes long; the 1568-byte gap is taken as its size, exposed as bytes, zero-initialised here.
// Unknown: The record layout.
extern "C" {
alignas(8) unsigned char emergencyResult[1568];
}
