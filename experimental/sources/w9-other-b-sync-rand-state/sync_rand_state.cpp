// Observed: syncRandState is one 32-bit word at 0x00483e5c in initialized data. The file word is 0.
// It is loaded and stored as a whole word by 91 functions each way, so it is a running value
// that is read and rewritten, not a constant. The next symbol, asyncRandState, starts 4 bytes later.
// Inferred: unsigned is used for the type because the word is never sign-tested by its callers
// here (the existing authored callers also declare it unsigned).
// Unknown: the generator that updates it, and what the "sync" prefix refers to.

extern "C" unsigned syncRandState = 0;
static_assert(sizeof(syncRandState) == 4, "syncRandState");
