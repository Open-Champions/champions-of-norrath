// noiseTable at 0x006021c8, 512 bytes, to _3map$iconList.
// 256 halfwords. BSS is zero. initNoiseTable fills them later.
extern "C" {
unsigned char noiseTable[512];
}
static_assert(sizeof noiseTable == 512u, "noiseTable extent");
