#include "word.hpp"
// Observed worldWaterAlpha2 at 0x00484150: four file-backed bytes in .sdata. cvProcess stores the fourth waterColor2 integer here. The file bytes are 62 00 00 00, the integer 98.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
int worldWaterAlpha2 NORRATH_LOADED(".sdata") = 98;
}
