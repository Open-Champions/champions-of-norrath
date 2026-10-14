// Observed: Convert8__10VIESFParseR12VIPixelStorePUcT2i at 0x0117b6c0 is 196 bytes. Arguments:
// parse (a0), store (a1, a pixel store record whose first word is a format code), source (a2),
// destination (a3) and count (the fifth argument, in t0). The format word selects:
//   1: copies count bytes from source to destination with memcpy and returns 0;
//   6: for each of count source bytes, takes the 4-byte entry at parse + 40 + 4 * byte and writes it
//      to the next 4 destination bytes with the first three bytes reversed (entry bytes 2, 1, 0, 3
//      become destination bytes 0, 1, 2, 3); a count of zero or less writes nothing; returns 0;
//   any other format: returns -1 without touching memory.
// Inferred: the 256-entry table at parse + 40 is a palette of 4-byte colors, so this expands
// 8-bit indexed pixels to 4-byte pixels with red and blue exchanged (the surface parser calls it
// per row with the row width as count). Unknown: the format code names, the meaning of format 1
// (a straight copy) and whether the byte order exchange is red/blue.

extern "C" {
void* memcpy(void* destination, const void* source, unsigned long size);

int Convert8__10VIESFParseR12VIPixelStorePUcT2i(unsigned char* parse, int* store, unsigned char* source, unsigned char* destination, int count) {
    int format = store[0];
    if (format == 1) {
        memcpy(destination, source, count);
        return 0;
    }
    if (format != 6) return -1;
    for (; count > 0; --count) {
        const unsigned char* entry = parse + 40 + 4 * *source;
        ++source;
        destination[0] = entry[2];
        destination[1] = entry[1];
        destination[2] = entry[0];
        destination[3] = entry[3];
        destination += 4;
    }
    return 0;
}
}
