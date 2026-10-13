// Observed: nullp is at 0x008d5798 in zero-filled storage; the next symbol, gameLines,
// starts 16 bytes later. The vehicle miscellaneous routine passes its address as a Point3
// argument to the force accumulation call.
// Inferred: a 12-byte Point3 (three floats) that stays zero; the extra 4 bytes before the
// next symbol are not claimed. Unknown: whether anything ever writes it.
extern "C" float nullp[3] = {0.0f, 0.0f, 0.0f};
static_assert(sizeof(nullp) == 12, "Point3 size");
