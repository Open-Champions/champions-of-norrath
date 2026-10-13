// Observed .bss symbol emergencyResult at 0x6ca800. The ELF size field is 0; the next symbol
// (soundLimit) starts 0x620 = 1568 bytes later. evaluateAnimation__FP14AnimationStateb
// (0x161df0) stores its address into the result slot of an animation state when no result is free,
// and animLoadBoneMatricies__FP6PacketP14AnimationState (0x1628e0) compares the slot with this
// address, clears the slot and writes zero at offset 1552 of the object.
// Inferred: a spare animation-result record with offset 1552 inside it, so at least 1556 bytes;
// the gap is taken as its size. Unknown: the record layout.
extern "C" {
unsigned char norrath_front_a_view_6ca800[1568];
}
