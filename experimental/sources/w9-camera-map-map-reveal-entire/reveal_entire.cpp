// Observed revealEntireMap__3mapv at 0x0015e078, 112 bytes.
// For each row y from 0 while y < _3map$mapheight (0x00483ec4), and for each
// column x from 0 while x < _3map$mapwidth (0x00483ec0), sets the high four
// bits of byte 1 of the 2-byte tile at _3map$maptiles (0x003347c0) + 432 * y +
// 2 * x (the existing low four bits stay). The bounds are read once, before the
// loops. Finally _3map$revealed (0x004868bc) becomes 1, also when the loops did
// nothing. Nothing is returned. Nothing here was executed on the EE.
extern "C" {
extern int _3map$mapwidth;
extern int _3map$mapheight;
extern int _3map$revealed;
extern unsigned char _3map$maptiles[];
}

extern "C" void revealEntireMap__3mapv() {
    const int height = _3map$mapheight;
    const int width = _3map$mapwidth;
    for (int y = 0; y < height; ++y) {
        unsigned char* tile = _3map$maptiles + 432 * y;
        for (int x = 0; x < width; ++x) {
            tile[1] |= 0xf0;
            tile += 2;
        }
    }
    _3map$revealed = 1;
}
