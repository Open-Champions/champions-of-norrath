#include "word.hpp"
// Observed worldWaterWavyness2 at 0x00484160: four file-backed bytes in .sdata. cvProcess loads this word with lwc1, multiplies, and stores it with swc1. The file bytes are 00 00 c0 3c.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
float worldWaterWavyness2 NORRATH_LOADED(".sdata") = 0.0234375f;
}
