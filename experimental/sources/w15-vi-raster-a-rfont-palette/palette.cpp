// Observed: Palette__7VIRFontRiR12VIPixelStorePPUc at 0x01136088 is 48 bytes in the pinned SLUS-20565 image. If the word at offset 52
// (0x34) of the font object is zero it returns -1 and writes nothing. Otherwise it stores that word through the first reference
// argument, the word at offset 32 (0x20) through the second (a full 32-bit store at the start of the VIPixelStore), and the word at
// offset 72 (0x48) through the third (a pointer-to-pointer), and returns 0.
// Inferred: the offset-52 word is a palette entry count or size, offset 72 a palette data pointer, and offset 32 a pixel-store value
// describing the palette.
// Unknown: the VIPixelStore layout beyond its first word and the palette format. Nothing here ran on the EE.
extern "C" {
int Palette__7VIRFontRiR12VIPixelStorePPUc(unsigned char* self, int* count, unsigned* pixelStore, unsigned* data) {
    unsigned n = *reinterpret_cast<unsigned*>(self + 52);
    if (n == 0) return -1;
    unsigned store = *reinterpret_cast<unsigned*>(self + 32);
    unsigned pointer = *reinterpret_cast<unsigned*>(self + 72);
    *count = static_cast<int>(n);
    *pixelStore = store;
    *data = pointer;
    return 0;
}
}
