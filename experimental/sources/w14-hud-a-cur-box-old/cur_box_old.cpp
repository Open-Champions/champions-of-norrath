// Observed: curBoxOld is the 16-byte initialised-data object at 0x00428290; the
// image holds four words of -2 (0xfffffffe). The authored quick-spell menu code
// compares the current selection with it per player and then overwrites it.
// Inferred: four ints, one per player; -2 is a value no selection takes.
extern "C" int curBoxOld[4] = {-2, -2, -2, -2};
static_assert(sizeof(curBoxOld) == 16, "curBoxOld");
