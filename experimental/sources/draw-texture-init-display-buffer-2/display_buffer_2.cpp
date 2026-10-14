// Observed displayBuffer2StartAddress at 0x00486a30. textureBufferStartAddress
// at 0x00486a34 begins four bytes later, so this object is 4 bytes. It is
// .sbss and zero in the image. engineInit stores 0x002bc000 here at runtime.

extern "C" {
unsigned displayBuffer2StartAddress;
}
