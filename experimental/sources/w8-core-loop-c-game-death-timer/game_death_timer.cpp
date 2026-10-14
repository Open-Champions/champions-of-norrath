// gameDeathTimer at 0x00484cd0, 2 bytes, in SLUS_205.65.
// Observed: two bytes in .data holding 0xffff in the file. The image loads it with both a signed and an unsigned halfword load, so the signedness is unknown; -1 is the signed reading of the file value.
extern "C" {
short gameDeathTimer = -1;
}
