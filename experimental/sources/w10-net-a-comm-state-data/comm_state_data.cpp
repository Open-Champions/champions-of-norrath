// Observed storage in the pinned SLUS_205.65 image for the voice-chat state
// words that commProcess updates. Each is four bytes with initial value zero.
//   commCVol            0x0048663c
//   commNameMask        0x00486648
//   commRVInternalLast  0x00486674
// Inferred: commCVol is a float (commProcess stores a scaled sample maximum into
// it); commNameMask is a bit mask that commProcess clears; commRVInternalLast
// remembers the previous commRVInternal value so that a change can be noticed.
// Unknown: every reader of the three words outside commProcess.
// The image holds these in its data section; this source lets the zero
// initialisers fall into bss, and section placement is not settled here.
extern "C" {
float commCVol;
int commNameMask;
int commRVInternalLast;
}
