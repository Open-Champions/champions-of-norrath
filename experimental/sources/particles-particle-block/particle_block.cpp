// block at 0x00700080, 524288 bytes, to blockList.
// 128 pages of 4096. BSS is zero. P_Init writes the page headers later.
// Other ELF symbols named block live in .srs and are not this object.
extern "C" {
unsigned char block[128 * 4096];
}
static_assert(sizeof block == 524288u, "block extent");
