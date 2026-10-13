/* Observed AllocTimerCounter at 0x002de8c0, 72 bytes. It calls DIntr, keeps its
 * result, calls iAllocTimerCounter, and calls EIntr only when the DIntr result was
 * nonzero. The interrupt-context routine's result is returned unchanged. What the
 * counter number means is unknown. Compiled output is not claimed to match the
 * original.
 */
extern "C" {
int DIntr();
int EIntr();
int iAllocTimerCounter();
}

extern "C" int AllocTimerCounter() {
    const int was_enabled = DIntr();
    const int counter = iAllocTimerCounter();
    if (was_enabled != 0) EIntr();
    return counter;
}
