// Observed engineGetField__Fv at 0x00191300, 16 bytes in SLUS_205.65. Returns the 32-bit word
// stored 8 bytes into the object `draw` (0x00790ff0, a BSS object followed by dispEnv at
// 0x00791030, so at most 64 bytes long). Nothing is written.
// Inferred: callers use bit 0 of the result as an even/odd field selector (see the
// texture-draw-super-color host contract); that comes from the caller, not from this body.
// Unknown: the type and meaning of the other bytes of `draw`, and who writes the word.
extern "C" int draw[];

extern "C" int engineGetField__Fv() {
    return draw[2];
}
