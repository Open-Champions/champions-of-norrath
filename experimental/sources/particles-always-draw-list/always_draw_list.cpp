// alwaysDrawList at 0x00486888. The ELF size field is 0.
// offTheGrid at 0x00486890 is the next symbol, so the extent is eight bytes.
// .sbss is zero. drawObjectsTask passes this address and does not read a size.
extern "C" {
unsigned char alwaysDrawList[8];
}
static_assert(sizeof alwaysDrawList == 8u, "alwaysDrawList extent");
