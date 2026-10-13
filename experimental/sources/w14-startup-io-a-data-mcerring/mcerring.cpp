// Observed: mcerring is a 4-byte word at 0x01192c58 in the memory-card window module, right
// after mcerror (0x01192c4c) and sCountryName. Found in the image: a single store, of zero,
// inside OnMessage__20VIWndGenericRendererP5VIWndiii just after the SetFocus call that follows
// a message handled on the same path that sets mcerror to 1. The image word is zero.
// Inferred: a flag paired with mcerror, cleared there.
// Unknown: its readers and its meaning beyond the name.
extern "C" {
int mcerring = 0;
}
