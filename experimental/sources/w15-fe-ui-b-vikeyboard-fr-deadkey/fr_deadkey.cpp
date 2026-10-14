// Observed: sFrDeadkey at 0x00442148 is four initialised words: 94, 96, 126 and 168, 16 bytes in all. These are the
// codes of ^, `, ~ and the diaeresis sign; CombineDeadkey__10VIKeyboardUc treats the same four values as its
// pending dead keys (together with 180 for the acute accent, which is not in this table).
// Inferred: the dead-key code list of the French layout; the acute accent 180 being absent is as observed.
// Unknown: the readers beyond the keyboard-queue routine, and why the symbol is local in the original.
int sFrDeadkey[4] = {94, 96, 126, 168};
