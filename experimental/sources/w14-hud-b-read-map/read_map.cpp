// Observed readMap__3mapPUc at 0x0015c038, 416 bytes, no frame. _3map$maptiles (0x003347c0) is read
// as 216 rows of 216 two-byte tiles (432 bytes per row). Only the high nibble of the second byte of
// each tile is written; the low nibble and the first byte are never changed. The argument is a packed
// source of 108 rows of 54 bytes (5,832 bytes in all), each byte holding two 4-bit values.
//  Pass 1, each even row r = 0, 2, ..., 214 in order, each source byte b in turn (54 per row): the
//    high nibble of tile 4k gets b >> 4 and that of tile 4k + 2 gets b & 15 (k = 0..53).
//  Pass 2, each even row: the high nibble of every odd tile 2j + 1, j = 0..106, becomes the rounded-up
//    mean (a + b + 1) >> 1 of the high nibbles of tiles 2j and 2j + 2; tile 215 copies tile 214's.
//  Pass 3, each column c = 0..215: the high nibble of every odd row 2i + 1, i = 0..106, becomes the
//    rounded-up mean of the high nibbles of rows 2i and 2i + 2 in that column; row 215 copies row 214.
// Nothing is returned.
// Inferred: the source is a 108 by 108 grid of 4-bit values that is doubled in each direction with
// linear interpolation (a height or shade map). Unknown: what the value means, the owner struct
// beyond the tile array, and the meaning of the tiles' other bits.
extern "C" {
extern unsigned char _3map$maptiles[];

void readMap__3mapPUc(unsigned char* source) {
    unsigned char* tiles = _3map$maptiles;
    for (int row = 0; row < 216; row += 2) {
        for (int k = 0; k < 54; ++k) {
            unsigned char packed = *source++;
            unsigned char* first = tiles + row * 432 + k * 8 + 1;
            first[0] = (first[0] & 0x0f) | (packed & 0xf0);
            first[4] = (first[4] & 0x0f) | ((packed & 0x0f) << 4);
        }
    }
    for (int row = 0; row < 216; row += 2) {
        unsigned char* base = tiles + row * 432 + 1;
        for (int j = 0; j < 107; ++j) {
            unsigned char* middle = base + (2 * j + 1) * 2;
            int mean = ((middle[-2] >> 4) + (middle[2] >> 4) + 1) >> 1;
            *middle = (*middle & 0x0f) | (mean << 4);
        }
        base[430] = (base[430] & 0x0f) | (base[428] & 0xf0);
    }
    for (int column = 0; column < 216; ++column) {
        unsigned char* base = tiles + column * 2 + 1;
        for (int i = 0; i < 107; ++i) {
            unsigned char* middle = base + (2 * i + 1) * 432;
            int mean = ((middle[-432] >> 4) + (middle[432] >> 4) + 1) >> 1;
            *middle = (*middle & 0x0f) | (mean << 4);
        }
        base[215 * 432] = (base[215 * 432] & 0x0f) | (base[214 * 432] & 0xf0);
    }
}
}
