// Observed in the pinned SLUS_205.65 image: two file-local 16-byte objects used by
// playerItemScan__6PlayerP3padi, one int per player slot, both zero at start.
//   pokeMessageAlpha  .data  0x004085e0  16 bytes (pokeItemMagic follows at +16)
//   pokeItemMagic     .data  0x004085f0  16 bytes
// Observed use: pokeMessageAlpha[player] is the alpha of the on-screen interaction message (it
// rises by 7 per call up to 128 and falls by 7 when nothing is in reach); pokeItemMagic[player] is
// 0, 1 or 2 and picks the message colour.
// Inferred: four slots, one per local player, from the 16-byte size and the player-index addressing.
// Unknown: whether other code reads or writes these words.
extern "C" {
int pokeMessageAlpha[4];
int pokeItemMagic[4];
}
