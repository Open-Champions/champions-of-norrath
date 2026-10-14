// Observed: sortedTimes is 160 zero bytes. The browser indexes it by a
// 16-byte stride and reads bytes inside each record. The local-time patch
// writes 16 bytes and advances 16. The list generator keeps at most ten
// records, which fills exactly 160 bytes. The next symbol starts there.
// Inferred: none.
// Unknown: the meaning of each byte inside a 16-byte record.
extern "C" unsigned char sortedTimes[160] = {};
static_assert(sizeof(sortedTimes) == 160, "sorted times");
