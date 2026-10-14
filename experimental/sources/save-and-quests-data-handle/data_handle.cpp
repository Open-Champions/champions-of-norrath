// Observed dataHandle at 0x00486db0 in .sbss, a four-byte word (the next
// symbol, sid, starts four bytes later). The save-net-anim notes record it as
// the id returned when textHandler__FPUci is registered as a message handler.
// It starts as zero; nothing in this package reads it.
extern "C" {
int dataHandle;
}
