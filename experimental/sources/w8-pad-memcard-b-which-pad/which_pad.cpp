// Observed: whichPad is one byte at 0x00485bec, zero in the image; the next
// symbol, originalYaw, starts at 0x00485bf0. The authored browser and heartbeat
// callers store a signed byte into it.
// Inferred: it holds a selected pad number.
// Unknown: its meaning when negative.
extern "C" signed char whichPad = 0;
