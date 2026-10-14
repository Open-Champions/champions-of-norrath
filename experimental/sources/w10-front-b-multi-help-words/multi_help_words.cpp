// Observed in the pinned SLUS_205.65 image: four local zero-initialised objects of the multi-player
// help message, indexed by player number 0..3 by multiHudAddHelpMessage, which stores title and
// body pointers, the side value and a release flag.
//   multiRelease  0x00486ae0  4 bytes  (next symbol, drawBustCur, is at +4); byte stores
//   multiTitle    0x007d06e8  16 bytes (multiBody follows at +16); word stores of a pointer
//   multiSide     0x007d0708  16 bytes (drawBustTitle follows at +16); word stores
// The ELF symbols give no size; each size is the gap to the next symbol. multiBody (0x007d06f8)
// sits between title and side and is not defined here.
// Inferred: one slot per player, from the player-number indexing and the 4 / 16 byte extents.
// Unknown: whether any slots past index 3 exist (nothing indexes them).
extern "C" {
unsigned char multiRelease[4];
unsigned short* multiTitle[4];
int multiSide[4];
}
