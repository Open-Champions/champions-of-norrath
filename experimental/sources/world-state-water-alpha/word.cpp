#include "word.hpp"
// Observed worldWaterAlpha at 0x0048414c: four file-backed bytes in .sdata. cvProcess stores the fourth waterColor integer here. The file bytes are 62 00 00 00, the integer 98.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
int worldWaterAlpha NORRATH_LOADED(".sdata") = 98;
}
