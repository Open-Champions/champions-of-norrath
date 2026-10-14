// Observed four bytes at 0x00480bb8 (.sdata, reached through gp-28216): the
// little-endian word 0x3e4ccccc, which as a single is 0.19999999 (one unit below 0.2f). The neighbours at
// 0x480bb4 and 0x480bac are other constants and are not part of this view.
// Unknown: whether the original treats it as one named variable.
extern "C" const unsigned norrath_rtti_entities_a_view_480bb8 = 0x3e4ccccc;
