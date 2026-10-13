// Observed: progress is a 4-byte word at 0x00486ca0, zero in the file. drawProgress loads
// it as a float and converts it to an integer; startProgress stores zero into it.
// Inferred: a float, from those two uses. Unknown: the writer that advances it.
extern "C" float progress = 0.0f;
static_assert(sizeof(progress) == 4, "observed size");
