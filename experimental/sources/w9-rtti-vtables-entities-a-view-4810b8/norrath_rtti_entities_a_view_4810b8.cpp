// Observed four bytes at 0x004810b8 (.sdata, reached through gp-26940 or gp-26936):
// the little-endian word 0x3e4ccccc, the single just below 0.2f. A second word
// with the same value sits at the neighbouring address (0x004810b4); the two are
// separate literals in the image and are kept as separate views.
// Unknown: whether the original treats it as one named variable.
extern "C" const unsigned norrath_rtti_entities_a_view_4810b8 = 0x3e4ccccc;
