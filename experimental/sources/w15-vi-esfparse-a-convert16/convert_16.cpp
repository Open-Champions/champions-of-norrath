// Observed: Convert16__10VIESFParseR12VIPixelStorePUcT2i at 0x0117a4d0 is 396 bytes. Arguments:
// parse (a0, not read), store (a1, a pixel store record whose first word is a format code), source
// (a2) of 16-bit pixels, destination (a3) and count of pixels (the fifth argument, in t0). With p
// the little-endian 16-bit source value, the format word selects, for each of count pixels (nothing
// is written when count is zero or less):
//   2: writes 4 destination bytes: (p << 3) & 0xf8 (low five bits of p), (p >> 2) & 0xf8,
//      (p >> 7) & 0xf8 and (p >> 15) << 7;
//   3: the same three color bytes and 255 as byte 3;
//   5: memcpy of count * 2 bytes, returns 0;
//   6: writes (p >> 7) & 0xf8, (p >> 2) & 0xf8, (p << 3) & 0xf8 and (p >> 15) << 7 (the first
//      and third color bytes exchanged);
//   any other format returns -1 without touching memory.
// The other formats return 0.
// Inferred: this expands 5-5-5-1 16-bit pixels from the file to 4-byte pixels (alpha from the top
// bit in 2 and 6, forced opaque in 3), or copies them (5). Unknown: the format code names.

extern "C" {
void* memcpy(void* destination, const void* source, unsigned long size);

int Convert16__10VIESFParseR12VIPixelStorePUcT2i(unsigned char* parse, int* store, unsigned char* source, unsigned char* destination, int count) {
    (void)parse;
    int format = store[0];
    if (format == 3) {
        for (; count > 0; --count) {
            unsigned pixel = *reinterpret_cast<unsigned short*>(source);
            destination[0] = pixel << 3;
            destination[1] = (pixel >> 2) & 0xf8;
            destination[2] = (pixel >> 7) & 0xf8;
            destination[3] = 255;
            source += 2;
            destination += 4;
        }
        return 0;
    }
    if (format == 2) {
        for (; count > 0; --count) {
            unsigned pixel = *reinterpret_cast<unsigned short*>(source);
            destination[0] = pixel << 3;
            destination[1] = (pixel >> 2) & 0xf8;
            destination[2] = (pixel >> 7) & 0xf8;
            destination[3] = (pixel >> 15) << 7;
            source += 2;
            destination += 4;
        }
        return 0;
    }
    if (format == 5) {
        memcpy(destination, source, count * 2);
        return 0;
    }
    if (format != 6) return -1;
    for (; count > 0; --count) {
        unsigned pixel = *reinterpret_cast<unsigned short*>(source);
        destination[3] = (pixel >> 15) << 7;
        destination[2] = pixel << 3;
        destination[1] = (pixel >> 2) & 0xf8;
        destination[0] = (pixel >> 7) & 0xf8;
        source += 2;
        destination += 4;
    }
    return 0;
}
}
