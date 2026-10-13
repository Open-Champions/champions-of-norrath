/* Observed SetTimerHandler at 0x002defe0, 124 bytes. It calls DIntr, keeps its
 * result, passes its four arguments unchanged (a counter, a 64-bit value, a
 * handler address and a handler argument, in registers a0..a3) to iSetTimerHandler,
 * and calls EIntr only when the DIntr result was nonzero. The interrupt-context
 * routine's result is returned unchanged. SetTimerAlarm passes a counter, its
 * 64-bit clock count, a local handler and a node; what this routine does with each
 * is unknown. Compiled output is not claimed to match the original.
 */
extern "C" {
int DIntr();
int EIntr();
int iSetTimerHandler(int counter, unsigned long long clocks,
                     int (*handler)(int, int, int, void*), void* argument);
}

extern "C" int SetTimerHandler(int counter, unsigned long long clocks,
                               int (*handler)(int, int, int, void*), void* argument) {
    const int was_enabled = DIntr();
    const int result = iSetTimerHandler(counter, clocks, handler, argument);
    if (was_enabled != 0) EIntr();
    return result;
}
