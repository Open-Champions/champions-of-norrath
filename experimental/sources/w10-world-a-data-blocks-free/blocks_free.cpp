// Observed blocksFree at 0x00486c40 (.sbss), 4 bytes. Reached through gp; two word loads. The authored memory-card browser passes its address to the card command that reports free blocks.
// Inferred: A free-block count returned by a memory-card query.
// Unknown: Who else writes it.
extern "C" {
int blocksFree;
}
