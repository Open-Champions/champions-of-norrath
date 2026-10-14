// Observed displayBuffer1StartAddress at 0x00486a2c. The next word,
// displayBuffer2StartAddress, begins at 0x00486a30, so this object is 4
// bytes. It is .sbss and zero in the image. engineInit stores 0x00230000
// here at runtime; that store is not this object's initial value.

extern "C" {
int displayBuffer1StartAddress;
}
