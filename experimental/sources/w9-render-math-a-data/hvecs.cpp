// Observed hvecs at 0x008d5978. hpos begins 0x90 bytes later. The authored select routine
// treats hvecs as ten 12-byte vectors in that range; the remaining 24 bytes are unexplained
// by the authored callers. The 144-byte extent is the symbol gap.
extern "C" {
unsigned char hvecs[144];
}
