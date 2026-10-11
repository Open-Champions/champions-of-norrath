// memAllocSize at 0x00486a58, four BSS bytes. vblankSemaphore starts at
// 0x00486a5c, so this object is four bytes and starts at zero.
extern "C" {
unsigned char memAllocSize[4];
}
