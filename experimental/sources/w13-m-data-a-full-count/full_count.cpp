// Observed fullCount at 0x00486ab8, a four-byte zero-initialised word in .bss. The next
// symbol is itemToDestroy at 0x00486abc. The shop inventory code clears it before it
// counts items, then shifts it by two when the level flag 0x4000 is set.
// The reference symbol is local; this unit exports it because another unit links to it.
// Unknown: what the count measures beyond the threshold of 128 used by its reader.
extern "C" {
int fullCount;
}
static_assert(sizeof fullCount == 4u, "fullCount width");
