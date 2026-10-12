// fadeIconY at 0x0034b828, 112 bytes, in SLUS_205.65.
// Observed: 112 zero bytes in .data, followed directly by other data. Inferred: 28 words, matching authored player-init code that indexes it as 4 records of 7. The element width is not shown by a direct access in the image that was checked.
extern "C" {
int fadeIconY[28] = {};
}
