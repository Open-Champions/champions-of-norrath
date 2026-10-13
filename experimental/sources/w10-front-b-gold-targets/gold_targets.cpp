// Observed in the pinned SLUS_205.65 image: playerGoldx at 0x004282d0 and playerGoldy at 0x004282b0
// are 16-byte initialised local arrays (symbol size 16 each). playerGoldx holds the words
// 50, 590, 50, 590 and playerGoldy the words 200, 200, 250, 250. hudGivePlayerGold converts the entry
// for a player number to float and uses the pair as a screen target.
// Inferred: one pair per player index 0..3 (four entries each), from the extent and the indexing.
// Unknown: why the corners are these values; nothing else reads the arrays in the authored sources.
extern "C" {
int playerGoldx[4] = {50, 590, 50, 590};
int playerGoldy[4] = {200, 200, 250, 250};
}
