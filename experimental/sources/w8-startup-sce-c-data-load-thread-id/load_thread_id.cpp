// Observed loadThreadID at 0x004866c8, 4 bytes, in the small-data section
// between musicTimer and coreLoadingScreenRunning. The image word is zero.
// game loop code passes it to ReferThreadStatus. Which thread id the load
// path stores here is not established by this definition.
extern "C" {
int loadThreadID = 0;
}
