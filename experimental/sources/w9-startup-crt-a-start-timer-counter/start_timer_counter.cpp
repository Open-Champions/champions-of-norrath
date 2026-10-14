/* Observed StartTimerCounter at 0x002deb58, 76 bytes. It calls DIntr, keeps its
 * result, passes its one argument to iStartTimerCounter, and calls EIntr only when
 * the DIntr result was nonzero. The interrupt-context routine's result is returned
 * unchanged. What the result means is unknown. Compiled output is not claimed to
 * match the original.
 */
extern "C" {
int DIntr();
int EIntr();
int iStartTimerCounter(int counter);
}

extern "C" int StartTimerCounter(int counter) {
    const int was_enabled = DIntr();
    const int result = iStartTimerCounter(counter);
    if (was_enabled != 0) EIntr();
    return result;
}
