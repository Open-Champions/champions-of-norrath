// Observed writeMap__3mapPUc at 0x0015e010, 88 bytes. Tiles of _3map$maptiles
// (0x003347c0) are 2 bytes, 216 per row, 432 bytes per row, 216 rows. For every
// second row (0, 2, ... 214) and for every fourth tile in that row (0, 4, ...
// 212) one output byte is written to the destination, which advances by one:
// the high four bits of tile byte 1 of that tile, plus the high four bits of
// tile byte 1 of the tile two columns further on, shifted down into the low four
// bits. That is 54 bytes per row and 108 rows, 5832 bytes in all, in row order.
// Nothing is returned and nothing else is written.
// Inferred: a half-resolution packing of the high-nibble flags of the tile map
// (the same nibble revealEntireMap sets), for the save path.
// Unknown: the meaning of the nibble, and why the in-between tiles are skipped.
extern "C" unsigned char _3map$maptiles[];

extern "C" void writeMap__3mapPUc(unsigned char* out) {
    for (int y = 0; y < 216; y += 2) {
        const unsigned char* row = _3map$maptiles + 432 * y;
        for (int x = 0; x < 216; x += 4) {
            const unsigned char* tile = row + 2 * x;
            *out++ = (tile[1] & 0xf0) + (tile[5] >> 4);
        }
    }
}
