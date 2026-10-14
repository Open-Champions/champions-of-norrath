// Observed lightSetAmbientColor__FPs at 0x0013e7a8, extent 40. Copies four
// halfwords (offsets 0, 2, 4, 6 of the argument) into the 8-byte object
// charAmbient at 0x00486850, in that order, and returns nothing. The loads are
// unsigned halfword and the stores halfword, so the bit pattern is kept.
// Inferred: four 16-bit colour channels. Unknown: channel order and range.
// charAmbient is defined by another package (w8-net-play-a-char-light-state).
extern "C" {
extern unsigned char charAmbient[8];
void lightSetAmbientColor__FPs(const unsigned short* color) {
    unsigned short* dest = reinterpret_cast<unsigned short*>(charAmbient);
    dest[0] = color[0];
    dest[1] = color[1];
    dest[2] = color[2];
    dest[3] = color[3];
}
}
