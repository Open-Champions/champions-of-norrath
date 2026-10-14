// Observed: pnums starts at 0x008d5da0 in zero-filled storage; the next symbol, nameTags, starts 32 bytes
// later and dummyList precedes it. init__Fv takes its address.
// Inferred: four 8-byte strings (one per player), from the existing authored init which writes "pnum=<k>"
// into pnums[k] for each of up to four players. 4 * 8 = 32.
// Unknown: whether the entries are used for anything else.

extern "C" {
char pnums[4][8];
}
static_assert(sizeof(pnums) == 32, "pnums");
