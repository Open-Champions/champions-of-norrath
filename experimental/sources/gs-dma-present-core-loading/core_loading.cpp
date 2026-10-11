// Observed coreLoadingScreenRunning at 0x004866cc. One .sdata byte, file
// value 0. engineFrameEnd reads it as a signed byte.
extern "C" signed char coreLoadingScreenRunning = 0;
static_assert(sizeof(coreLoadingScreenRunning) == 1, "one byte");
