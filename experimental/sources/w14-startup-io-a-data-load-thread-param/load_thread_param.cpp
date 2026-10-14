// Observed: loadThreadParam starts at 0x008dd400 in zero-filled storage; the next symbol,
// modulePresent, starts 48 bytes later. coreStartLoadingScreen writes words 1 to 5 (entry
// address, stack address, stack size 8192, global pointer, priority 5) and passes the block to
// CreateThread; word 0 is never written there.
// Inferred: twelve words, the kernel thread-parameter block; the extent is the gap, an upper bound.
// Unknown: the use of words 0 and 6 to 11.
extern "C" {
unsigned loadThreadParam[12];
}
