// Observed: markFileName at 0x007f80a0 is a zero-initialised BSS buffer. The next object,
// configFileName, begins 0x30 bytes later and the previous one ends at it, so 48 bytes is the
// room the image gives it. The configure screen passes its address to the memory card command
// queue (command 514), so it holds a file name.
// Inferred: a 48-byte character buffer, from the spacing.
// Unknown: the exact content written to it, and whether the true extent is shorter than 48.
extern "C" char markFileName[48];
char markFileName[48];
