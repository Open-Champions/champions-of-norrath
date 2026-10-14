// wigglyTab at 0x006ffe60, 544 bytes, to block. BSS is zero.
// P_Init writes the first 512 bytes later.
extern "C" {
unsigned char wigglyTab[544];
}
static_assert(sizeof wigglyTab == 544u, "wigglyTab extent");
