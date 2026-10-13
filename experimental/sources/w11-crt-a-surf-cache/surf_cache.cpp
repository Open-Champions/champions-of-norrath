// Observed SurfCache at 0x011eda68, 152 bytes (the gap to the next symbol, sSend at 0x011edb00; the
// symbol has no recorded size), pinned SLUS_205.65: a zero-initialised file-local table in the
// raster module. Begin scene (w10-vi-b-raster-begin-scene) clears the word at byte offset 28 (0x011eda84)
// before it calls SetGSScissoring__8VIRaster. That is the only access known; the table's entry
// layout is not established, so it is an array of 38 words.
// Unknown: the true size (152 is an upper bound) and the meaning of the cleared word.
extern "C" {
unsigned SurfCache[38] __attribute__((aligned(8)));
}
