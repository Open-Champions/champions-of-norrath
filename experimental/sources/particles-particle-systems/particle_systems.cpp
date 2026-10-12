// PS at 0x006f5ad0, 4992 bytes, to bolt.
// 16 records of 312. BSS is zero. initParticleSystems clears word +304 later.
extern "C" {
unsigned char PS[16 * 312];
}
static_assert(sizeof PS == 4992u, "PS extent");
