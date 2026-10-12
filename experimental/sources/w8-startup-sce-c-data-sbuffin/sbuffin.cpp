// Observed sbuffin at 0x007dd880. The next .bss symbol, ambientGroup, is 0x40
// bytes later; callers send 16 or 0 bytes from it. The ELF gives no size, so 64
// comes from that spacing. The image holds no data. Adjacency to sbuff is not
// guaranteed by the separate object.
extern "C" {
alignas(64) unsigned char sbuffin[64];
}
