// Observed in the pinned SLUS_205.65: three consecutive initialised-data words, each
// 4 bytes with initial value 0: bossMeterCurrent 0x00483f48, bossMeterMax 0x00483f4c and
// bossMeterAlpha 0x00483f50. Callers store the first two with single-precision float
// stores (swc1) and load or store the third as an integer word.
// Inferred: bossMeterCurrent and bossMeterMax are floats, bossMeterAlpha is an int.
// Unknown: the unit of the meter values, the range of the alpha, and whether the zeros
// were written in the source or implied.
extern "C" {
float bossMeterCurrent = 0.0f;
float bossMeterMax = 0.0f;
int bossMeterAlpha = 0;
}
