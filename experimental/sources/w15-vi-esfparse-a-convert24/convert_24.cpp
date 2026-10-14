// Observed: Convert24__10VIESFParseR12VIPixelStorePUcT2i at 0x0117a328 is 420 bytes. Arguments:
// parse (a0, not read), store (a1, a pixel store record whose first word is a format code), source
// (a2) of 3-byte pixels, destination (a3) and count of pixels (the fifth argument, in t0). The
// format word is dispatched through a five-entry jump table for values 2 to 6; for each of count
// pixels (nothing is written when count is zero or less):
//   2 and 3: copy source bytes 0, 1, 2 to destination bytes 0, 1, 2 and write 255 to byte 3
//      (4 destination bytes per pixel);
//   4: memcpy of count * 3 bytes, returns 0;
//   5: write one 16-bit value (advancing 2 bytes): source byte 0 >> 3 in bits 0-4, byte 1 >> 3 in
//      bits 5-9, byte 2 >> 3 in bits 10-14 and bit 15 set;
//   6: write source bytes 2, 1, 0 as destination bytes 0, 1, 2 and 255 to byte 3;
//   any other format (including values below 2 and above 6) returns -1 without touching memory.
// The other formats return 0.
// Inferred: this converts 3-byte color pixels from the file into the surface's pixel layout:
// opaque 4-byte pixels (2, 3), a straight copy (4), a 16-bit 5-5-5-1 pack with the top bit set
// (5) and an exchange of the first and third byte (6). Unknown: the format code names and why
// formats 2 and 3 behave identically here.

extern "C" {
void* memcpy(void* destination, const void* source, unsigned long size);

int Convert24__10VIESFParseR12VIPixelStorePUcT2i(unsigned char* parse, int* store, unsigned char* source, unsigned char* destination, int count) {
    (void)parse;
    switch (store[0]) {
    case 2:
    case 3:
        for (; count > 0; --count) {
            destination[0] = source[0];
            destination[1] = source[1];
            destination[2] = source[2];
            destination[3] = 255;
            source += 3;
            destination += 4;
        }
        return 0;
    case 4:
        memcpy(destination, source, count * 3);
        return 0;
    case 5:
        for (; count > 0; --count) {
            *reinterpret_cast<unsigned short*>(destination) = (source[0] >> 3) | ((source[1] >> 3) << 5) | ((source[2] >> 3) << 10) | 0x8000;
            source += 3;
            destination += 2;
        }
        return 0;
    case 6:
        for (; count > 0; --count) {
            destination[3] = 255;
            destination[2] = source[0];
            destination[1] = source[1];
            destination[0] = source[2];
            source += 3;
            destination += 4;
        }
        return 0;
    default:
        return -1;
    }
}
}
