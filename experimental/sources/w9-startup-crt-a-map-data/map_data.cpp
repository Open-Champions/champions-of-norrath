// Observed class static storage of map:
//   _3map$mapOffset  0x00483e80    8 bytes, small data, zero  (read here as two floats)
//   _3map$mapwidth   0x00483ec0    4 bytes, small data, zero
//   _3map$mapheight  0x00483ec4    4 bytes, small data, zero
//   _3map$maptiles   0x003347c0   93312 bytes, initialized data, all zero
// The sizes are the symbol sizes. The early-initialization routine that uses them (w8-core-loop-b-map-early-init)
// treats mapOffset as two floats, mapwidth and mapheight as ints and maptiles as a byte array. 93312 is 432 * 216;
// that product is only an arithmetic observation, the tile layout is not established here. Neighbouring statics
// (mapaccu, mapaccucount, autoMapBitsSize) are not defined by this unit. The zero values are written as
// zero-initialized definitions; the executable places them in initialized data. Fresh compilation is not an
// original-byte match.
extern "C" {
float mapOffset[2] __asm__("_3map$mapOffset");
int mapwidth __asm__("_3map$mapwidth");
int mapheight __asm__("_3map$mapheight");
unsigned char maptiles[93312] __asm__("_3map$maptiles");
}
