// Observed: Convert32__10VIESFParseR12VIPixelStorePUcT2i at 0x0117a1b0 is 372 bytes. Arguments:
// parse (a0, not read), store (a1, a pixel store record whose first word is a format code), source
// (a2) of 4-byte pixels, destination (a3) and count of pixels (the fifth argument, in t0). The
// format word selects, for each of count pixels (nothing is written when count is zero or less):
//   2: memcpy of count * 4 bytes, returns 0;
//   3: copies the first three source bytes unchanged and writes 255 to destination byte 3;
//   5: writes one 16-bit value to the destination (advancing 2 bytes): source byte 0 shifted right
//      by 3 in bits 0-4, byte 1 >> 3 in bits 5-9, byte 2 >> 3 in bits 10-14 and byte 3 >> 7 in
//      bit 15;
//   6: writes source bytes 2, 1, 0, 3 as destination bytes 0, 1, 2, 3;
//   any other format returns -1 without touching memory.
// Formats 3, 5 and 6 return 0 as well.
// Inferred: this converts 4-byte color pixels from the file into the surface's pixel layout:
// format 2 keeps them, 3 forces them opaque, 5 packs them to a 16-bit 5-5-5-1 layout and 6
// exchanges the first and third byte. Unknown: the format code names.

extern "C" {
void* memcpy(void* destination, const void* source, unsigned long size);

int Convert32__10VIESFParseR12VIPixelStorePUcT2i(unsigned char* parse, int* store, unsigned char* source, unsigned char* destination, int count) {
    (void)parse;
    int format = store[0];
    if (format == 3) {
        for (; count > 0; --count) {
            destination[0] = source[0];
            destination[1] = source[1];
            destination[2] = source[2];
            destination[3] = 255;
            source += 4;
            destination += 4;
        }
        return 0;
    }
    if (format == 2) {
        memcpy(destination, source, count * 4);
        return 0;
    }
    if (format == 5) {
        for (; count > 0; --count) {
            *reinterpret_cast<unsigned short*>(destination) = (source[0] >> 3) | ((source[1] >> 3) << 5) | ((source[2] >> 3) << 10) | ((source[3] >> 7) << 15);
            source += 4;
            destination += 2;
        }
        return 0;
    }
    if (format != 6) return -1;
    for (; count > 0; --count) {
        destination[3] = source[3];
        destination[2] = source[0];
        destination[1] = source[1];
        destination[0] = source[2];
        source += 4;
        destination += 4;
    }
    return 0;
}
}
