// Observed: sortedDirCount is one 32-bit word at 0x00485be8 in initialized data (symbol size 4). The file
// word is 0. generateSortedListAndInfoHeaders__Fv stores it; asyncSaveGame__Fv, getBestSlotName__FPc and
// patchSortedListWithLocalTime__Fv load it.
// Inferred: the number of valid entries in sortedDir, from the names.
// Unknown: the upper bound.

extern "C" int sortedDirCount = 0;
static_assert(sizeof(sortedDirCount) == 4, "sortedDirCount");
