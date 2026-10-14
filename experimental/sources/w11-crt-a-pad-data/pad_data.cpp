// Observed PadData at 0x011e9480, 256 bytes (the gap to the next symbol, backTile.0 at 0x011e9580;
// the symbol has no recorded size), pinned SLUS_205.65: a zero-initialised file-local buffer. Init__5VIPadii passes
// its address as the third argument of scePadPortOpen for every pad, so it is the pad library's DMA
// buffer. Its use as one shared buffer for all pads comes from the single caller; the library's
// own required alignment is 64 bytes and the address here is 128-byte aligned.
// Unknown: whether the true size is below 256; the extent is an upper bound taken from the next symbol.
extern "C" {
unsigned char PadData[256] __attribute__((aligned(64)));
}
