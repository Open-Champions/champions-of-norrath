// Observed: globDir is zero-filled storage at 0x006d8780. The previous symbol,
// lumpLoadName, ends there, and the next symbol, table, begins at 0x006d8fa0, so the extent
// is 2080 bytes. lumpLoadGlob reads the first 2048 bytes of a packed file into it and walks
// 40-byte directory records (name at +0, byte size in the word at +36).
// Inferred: the extra 32 bytes are padding or an unrelated object before the next symbol; the
// extent is the gap, not a stated symbol size.
// Unknown: whether anything else uses the tail.
extern "C" char globDir[2080] = {};
static_assert(sizeof(globDir) == 2080, "observed extent");
