// Observed: nullBuff starts at 0x008d1570 in zero-filled storage; the next symbol, zeroTile, starts 16384
// bytes later. MCA_Init__Fv takes its address (and clears 16384 bytes there) and MCA_Process__FPi passes it
// to sceMcWrite with a length of at most 16384 to write zeros.
// Inferred: a 16384-byte buffer of zeros, as the existing authored MCA_Process uses it. Note that the
// authored memcard-calls MCA_Init keeps a private file-local copy of this name; that copy is not this one.
// Unknown: nothing further.

extern "C" {
unsigned char nullBuff[16384];
}
static_assert(sizeof(nullBuff) == 16384, "nullBuff");
