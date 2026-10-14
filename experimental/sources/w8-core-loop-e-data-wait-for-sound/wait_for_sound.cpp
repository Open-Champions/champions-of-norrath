// Observed waitForSound at 0x0048501c, one byte in .sdata. The file byte is 0
// (the next byte belongs to animLoadingComplete and is 1). Accessed with seven
// byte loads and four byte stores. Its meaning beyond the name is unknown.
extern "C" {
signed char waitForSound = 0;
}
