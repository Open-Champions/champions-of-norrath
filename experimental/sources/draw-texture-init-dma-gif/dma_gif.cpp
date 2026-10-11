// Observed dmaGif at 0x00486a3c, the word initSupersample and engineInit
// load. dmaVif0 begins at 0x00486a40, so this object is 4 bytes. It is
// .sbss and zero in the image. A different local dmaGif is at 0x004868d4;
// drawInit keeps that one private.

extern "C" {
unsigned* dmaGif;
}
