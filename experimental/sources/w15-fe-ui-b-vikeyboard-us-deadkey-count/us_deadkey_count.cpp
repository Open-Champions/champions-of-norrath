// Observed: sUsDeadkeyCount at 0x004865e0 is one initialised word with the value 0. The word at 0x004865e8,
// sFrDeadkeyCount, is 4 (not defined here), and the word between them is not named.
// Inferred: the count of the US layout's dead-key table, which is empty. Existing code reads this word as the
// bound of a loop over sFrDeadkey, so with the value 0 that loop does not run; this is how the original data
// reads and is not changed here.
// Unknown: why the US count word, not the French one, bounds the loop in the keyboard-queue routine.
int sUsDeadkeyCount = 0;
