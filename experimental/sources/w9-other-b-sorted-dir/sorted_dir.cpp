// Observed: sortedDir starts at 0x007f7d40 in zero-filled storage; the next symbol, quickLoadStamp, starts
// 640 bytes later and sortedPlayerRace precedes it. asyncLoadGame__Fv, generateSortedListAndInfoHeaders__Fv
// and patchSortedListWithLocalTime__Fv take its address; asyncChangePlayers__Fv, asyncSaveGame__Fv and
// runBrowser__FPUs use offset 32, and getBestSlotName__FPc offset 33.
// Inferred: ten records of 64 bytes, because sortedTimes (authored elsewhere) holds ten 16-byte records
// and the gap here is 640. The size is the gap to the next symbol, not a symbol-table size.
// Unknown: the record layout beyond the name-like bytes near offset 32.

extern "C" {
char sortedDir[640];
}
static_assert(sizeof(sortedDir) == 640, "sortedDir");
