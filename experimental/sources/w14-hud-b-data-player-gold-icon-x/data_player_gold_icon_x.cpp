// Observed playerGoldIconx at 0x004282c0, 16 bytes, section .data (local d, no ELF size; the next symbol,
// playerGoldx, starts 16 bytes later and the previous one, playerGoldy, is also 16 bytes). The file bytes
// are four little-endian words: 16, 593, 16 and 593. drawGold__Fv (authored in w12-hud-loop-a-draw-gold)
// reads element p of the array as an x coordinate for the gold icon of player p (p = 0..3).
// Inferred: four ints, one per player slot, and the extent from the gap; the values look like the left
// and right screen positions used for players 0..3. Unknown: whether the true extent is 16, and which
// code (if any) writes the array.
extern "C" {
int playerGoldIconx[4] = {16, 593, 16, 593};
}
