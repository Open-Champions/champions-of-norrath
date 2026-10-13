/* Observed sceVu0CopyMatrix at 0x01348c80 (overlay range, 100 bytes). Calls DIntr, copies
 * four 16-byte quadwords from the source (second argument) to the destination (first
 * argument), then calls EIntr only when DIntr returned nonzero (a tail call in the
 * original). All four loads happen before the four stores, so overlapping source and
 * destination copy as a snapshot. Nothing is returned.
 * Inferred: DIntr's nonzero result means interrupts were enabled beforehand.
 * Unknown: alignment is assumed 16 bytes (the original uses lq/sq, which ignore the low
 * four address bits); the host copy cannot reproduce that masking.
 */
extern "C" {
int DIntr();
int EIntr();

void sceVu0CopyMatrix(float* destination, const float* source) {
    const int was_enabled = DIntr();
    float copy[16];
    for (int i = 0; i < 16; ++i) copy[i] = source[i];
    for (int i = 0; i < 16; ++i) destination[i] = copy[i];
    if (was_enabled) EIntr();
}
}
