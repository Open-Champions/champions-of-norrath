// Observed: Convert4__10VIESFParseR12VIPixelStorePUcT2i at 0x0117a660 is 412 bytes. Arguments:
// parse (a0), store (a1, a pixel store record whose first word is a format code), source (a2),
// destination (a3) and count of pixels (the fifth argument, in t0). The format word selects:
//   0: copies (count + 1) / 2 bytes (count shifted right by one, plus one when count is odd) from
//      source to destination with memcpy and returns 0;
//   1: reads source bytes and writes two destination bytes per source byte, first the low nibble
//      and then the high nibble, until count bytes were written (a final odd count writes only
//      the low nibble of the last source byte); returns 0, and writes nothing for a count of zero
//      or less;
//   6: like format 1, but each nibble indexes the 4-byte entry at parse + 40 + 4 * nibble, which
//      is written to the next 4 destination bytes with the first three bytes reversed (entry bytes
//      2, 1, 0, 3 become destination bytes 0, 1, 2, 3); returns 0;
//   any other format: returns -1 without touching memory.
// Inferred: format 0 keeps 4-bit indexed pixels packed (two per byte), format 1 unpacks them to one
// byte each, and format 6 expands them through the palette at parse + 40 to 4-byte pixels with
// the first and third bytes exchanged (the surface parser calls it per row with the row width as
// count). Unknown: the format code names and whether the byte exchange is red/blue.

extern "C" {
void* memcpy(void* destination, const void* source, unsigned long size);

int Convert4__10VIESFParseR12VIPixelStorePUcT2i(unsigned char* parse, int* store, unsigned char* source, unsigned char* destination, int count) {
    int format = store[0];
    if (format == 1) {
        for (; count > 0; ++source) {
            *destination++ = *source & 0xf;
            --count;
            if (count > 0) {
                *destination++ = *source >> 4;
                --count;
            }
        }
        return 0;
    }
    if (format == 0) {
        if (count & 1) memcpy(destination, source, (count >> 1) + 1);
        else memcpy(destination, source, count >> 1);
        return 0;
    }
    if (format != 6) return -1;
    for (; count > 0; ++source) {
        const unsigned char* entry = parse + 40 + 4 * (*source & 0xf);
        destination[0] = entry[2];
        destination[1] = entry[1];
        destination[2] = entry[0];
        destination[3] = entry[3];
        destination += 4;
        --count;
        if (count > 0) {
            entry = parse + 40 + 4 * (*source >> 4);
            destination[0] = entry[2];
            destination[1] = entry[1];
            destination[2] = entry[0];
            destination[3] = entry[3];
            destination += 4;
            --count;
        }
    }
    return 0;
}
}
