// Observed: g_Timer at 0x004459a8, 32 bytes of .data. In words: 0, 0, -1, 1,
// 0, 0, 0, -1. The first two words are one 64-bit value read and incremented
// by iGetTimerSystemTime and cbTimerHandler. Offset 8 is the interrupt handler
// number InitTimer tests and stores. Offset 12 is never written by any source
// here. Offset 16 is cleared by InitTimer. Offset 20 holds the address of the
// g_CounterBuf pool entry chain that InitTimer builds. Offset 24 is the
// first-alarm word SetNextComp and cbTimerHandler read. Offset 28 is stored
// by cbTimerHandler (an alarm identifier, then -1) and tested by SetNextComp.
// Inferred: none; the offsets are labels, not original field names.
// Unknown: what the initial 1 at offset 12 means.
extern "C" {
alignas(8) int g_Timer[8] = {0, 0, -1, 1, 0, 0, 0, -1};
}
