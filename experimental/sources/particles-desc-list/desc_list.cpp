// descList at 0x00780300, 2048 bytes, to trail.
// 32 slots of 64. BSS is zero. allocDescriptor copies into a slot later.
extern "C" {
unsigned char descList[32 * 64];
}
static_assert(sizeof descList == 2048u, "descList extent");
