// Observed identityResult at 0x6ca1c0, 1600 nobits bytes in .bss. The
// animation initializer stores 0x6dc18000 at offset 4, the halfword 64 at
// offset 8, and 64 groups of 12 halfwords from offset 16, holding 4096 at
// halfword positions 1, 5, and 10 of each group; the object starts cleared.
// Inferred: this reconstruction name is that object, zero-initialized like
// the nobits image.
// Unknown: later stores by code outside the initializer, and placement at
// the original address.
extern "C" {
unsigned char identityResult[1600] = {};
}
