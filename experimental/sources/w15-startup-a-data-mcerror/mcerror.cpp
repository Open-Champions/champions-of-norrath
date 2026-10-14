// Observed: mcerror is a 4-byte global word at 0x01192c4c in the memory-card window module,
// right before sCountryName and mcerring (0x01192c58). The window's message handler stores 1
// in it on the path that then clears mcerring. The image word is zero.
// Inferred: a flag meaning "a memory-card error is being shown", paired with mcerring.
// Unknown: its readers and any value other than 0 and 1.
extern "C" {
int mcerror = 0;
}
