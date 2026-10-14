// Observed wigglyTab at 0x6ffe60, 544 nobits bytes in .bss, and
// descListMark at 0x780280, 128 nobits bytes in .bss. The particle
// initializer writes 64 sine/cosine word pairs into the first 512 bytes of
// wigglyTab and fills descListMark with 0xffffffff; both start cleared.
// Inferred: these reconstruction names are those two objects,
// zero-initialized like the nobits image.
// Unknown: later stores by code outside the initializer, and placement at
// the original addresses.
extern "C" {
unsigned char wigglyTab[544] = {};
unsigned char descListMark[128] = {};
}
