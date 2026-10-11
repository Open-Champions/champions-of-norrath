// engineCurrentLevelName at 0x0037f988, 80 file bytes, all zero, in .data.
// packetBuffer begins 88 bytes later; the eight bytes between the symbol size
// and that neighbor are not part of this object. The reviewed -G0 profile may
// place these zeros in BSS. engineInit clears the first byte; these bytes are
// already zero. No EE startup stage has run.
extern "C" {
char engineCurrentLevelName[80];
}
static_assert(sizeof(engineCurrentLevelName) == 80, "level name extent");
