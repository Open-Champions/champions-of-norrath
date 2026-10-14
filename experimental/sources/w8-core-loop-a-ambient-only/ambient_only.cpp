// Observed ambientOnly at 0x00483ab0: 4 bytes of initialized data, value 0 in the file. lightSetAmbientOnly__Fv and lightResetOptionalParams__Fv
// store to it as a word; no other access was found by scanning gp-relative loads and stores.
// Inferred: nothing beyond those accesses. No field meaning is claimed.
// Unknown: what the value represents and its full range.
extern "C" {
int ambientOnly = 0;
}
