// Observed initial bytes in the pinned SLUS_205.65, all zero:
//   bossMeterCurrent 0x00483f48, bossMeterMax 0x00483f4c,
//   bossMeterAlpha 0x00483f50, each 4 bytes in small data, consecutive;
//   hudNoDraw 0x00484af8, ELF size 1.
// Inferred from the authored callers: the three meter words are ints and
// hudNoDraw is a signed byte flag. Their meaning is not established here.
extern "C" {
int bossMeterCurrent = 0;
int bossMeterMax = 0;
int bossMeterAlpha = 0;
signed char hudNoDraw = 0;
}
