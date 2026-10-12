// Observed: actLevelCount is five words, 22, 10, 18, 9 and 5. MC_Init indexes
// it by act. The symbol size is 20. The next four bytes before allActs are
// zero and are not part of this symbol.
// Inferred: each count is the number of 8-byte level records in that act.
// Unknown: whether a later store replaces a count.
extern "C" int actLevelCount[5] = {22, 10, 18, 9, 5};
static_assert(sizeof(actLevelCount) == 20, "five act counts");
