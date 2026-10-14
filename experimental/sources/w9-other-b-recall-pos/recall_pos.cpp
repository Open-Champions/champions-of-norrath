// Observed: recallPos is 48 bytes at 0x0034b540 in initialized data (symbol size 48, next symbol
// experienceTable). All file bytes are 0. playerRecallSpell__Fv and startNewWorld__Fv take its address.
// Inferred: four triples of single-precision floats, because the existing authored startNewWorld
// treats it as an array of four Point3 values; 4 * 12 = 48.
// Unknown: what each of the four entries records.

extern "C" float recallPos[4][3] = {};
static_assert(sizeof(recallPos) == 48, "recallPos");
