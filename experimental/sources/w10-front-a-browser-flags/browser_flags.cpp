// Observed: browserOverwrite at 0x00486c90 and browserDelete at 0x00486c94 are two adjacent
// 4-byte zero-initialised words in BSS. Their readers and writers treat them as flags: the memory
// card browser sets browserOverwrite to 1 when an overwrite is confirmed and browserDelete from
// the mode it was opened in, and both are cleared when the browser opens and when it returns to
// its list; the progress drawing picks the overwrite message when browserOverwrite is nonzero and
// the browser holds its delete message while browserDelete is nonzero.
// Inferred: int-sized flags, from the 4-byte spacing and the word accesses.
// Unknown: other readers outside the authored sources, and the values browserDelete can take.
extern "C" {
int browserOverwrite;
int browserDelete;
}
