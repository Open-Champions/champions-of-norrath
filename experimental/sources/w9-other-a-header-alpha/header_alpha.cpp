// Observed headerAlpha at 0x00486d48 (.sbss). The image bytes are zero. Three single-float stores and one single-float load use it.
// Inferred: only the access width. Unknown: the meaning of the value and its writers.
extern "C" float headerAlpha = 0;
