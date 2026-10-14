// Observed: loadFrames is a 4-byte word at 0x004866d0 in initialized data; the image word is
// zero. coreStartLoadingScreen stores zero to it through the global pointer. No other store was
// found by scanning the image's global-pointer accesses.
// Inferred: none.
// Unknown: who reads it.
extern "C" {
int loadFrames = 0;
}
