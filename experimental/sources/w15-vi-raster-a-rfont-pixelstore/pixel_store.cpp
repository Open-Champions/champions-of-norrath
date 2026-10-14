// Observed: PixelStore__7VIRFont at 0x01136018 is 8 bytes in the pinned SLUS-20565 image. It returns the word at offset 28 (0x1c)
// of the font object.
// Inferred: the word is an int status or a pointer-sized value describing the pixel storage; the return type is not established.
// Unknown: its meaning and layout. Nothing here ran on the EE.
extern "C" {
unsigned PixelStore__7VIRFont(unsigned char* self) {
    return *reinterpret_cast<unsigned*>(self + 28);
}
}
