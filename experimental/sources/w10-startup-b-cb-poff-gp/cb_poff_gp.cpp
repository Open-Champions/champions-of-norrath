/* Observed sceCbPoff_Gp at 0x008e1c80, uninitialised data (symbol size 0; the next symbol,
 * cd, is 64 bytes later and the previous one, _sif_FsPoff_Data, is 64 bytes earlier).
 * _sceFs_Poff_Intr loads one word from its start into the global-pointer register before
 * calling the registered power-off callback. The image bytes are zero.
 * Inferred: the word holds the global pointer to run that callback under.
 * Unknown: who stores it, and whether the other 60 bytes of the span belong to this
 * object; only the first word is used. It is aligned to 64 here because the original
 * address is.
 */
extern "C" {
alignas(64) unsigned sceCbPoff_Gp;
}
