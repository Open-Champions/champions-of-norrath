// Observed: loadStack starts at 0x008db280 in zero-filled storage. The next symbol,
// titleloadTexCopy, starts 8192 bytes later, and coreStartLoadingScreen passes 8192 as the
// stack size of the loading thread together with this address.
// Inferred: the whole gap is the thread stack.
// Unknown: whether anything else uses the storage.
extern "C" {
unsigned char loadStack[8192];
}
