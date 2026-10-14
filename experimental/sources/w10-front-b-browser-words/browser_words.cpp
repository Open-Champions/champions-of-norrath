// Observed in the pinned SLUS_205.65 image, local zero-initialised .bss objects without ELF sizes;
// each size below is the gap to the next symbol or, where noted, the indexing of its users:
//   slotsCorrupted       0x008d14c0  64 bytes  (undoQuickLoadStamp follows); runConfigure clears and
//                                    sets 27 byte entries (one per card slot)
//   undoQuickLoadStamp   0x008d1500  64 bytes  (playerStatus follows at +0x40); copied to and from
//                                    quickLoadStamp as a 64-byte block in eight-byte pieces
//   sortedPlayerRace     0x007f7ca0  160 bytes (sortedDir follows at +0xa0); runBrowser reads the word
//                                    at index slot * 4, so ten 16-byte records
// Inferred: ten browser slots in sortedPlayerRace, from the 160 bytes and the stride of four words.
// Unknown: whether slotsCorrupted uses its 37 bytes after the first 27; the layout of the stamp.
extern "C" {
char slotsCorrupted[64];
alignas(8) unsigned char undoQuickLoadStamp[64];
int sortedPlayerRace[40];
}
