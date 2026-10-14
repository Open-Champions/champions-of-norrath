// Observed: topId at 0x00444958, a 4-byte word. The image bytes are zero.
// InitThread returns early when it is above zero, and stores the result of
// CreateThread in it, even when that result is negative. iWakeupThread
// returns without queueing when it is zero.
// Inferred: none.
// Unknown: none. The original is in .data; this profile places a zero in .bss.
extern "C" {
int topId = 0;
}
