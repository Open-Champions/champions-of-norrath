// Observed: IsPalettized__7VIRFont at 0x01136008 is 12 bytes in the pinned SLUS-20565 image. It returns 1 if the word at offset 52 (0x34)
// of the font object is nonzero, otherwise 0.
// Inferred: the word is a palette pointer or palette size; the routine reports whether the font has one (Palette__7VIRFont returns
// failure when the same word is zero).
// Unknown: what the word holds. Nothing here ran on the EE.
extern "C" {
int IsPalettized__7VIRFont(unsigned char* self) {
    return *reinterpret_cast<unsigned*>(self + 52) != 0;
}
}
