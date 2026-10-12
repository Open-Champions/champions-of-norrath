// Observed loadingScreenRequestExit at 0x004866d4, 1 byte (a local symbol). Every file byte is zero, so the definition is
// zero-initialized storage. Unknown: its readers other than the game loop.
extern "C" {
signed char loadingScreenRequestExit = 0;
}
