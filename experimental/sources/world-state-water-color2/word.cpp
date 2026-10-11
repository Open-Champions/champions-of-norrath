#include "word.hpp"
// Observed worldWaterColor2 at 0x00484158: four file-backed bytes in .sdata. cvProcess stores the packed waterColor2 word here with sw. The file bytes are 80 80 80 00.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned worldWaterColor2 NORRATH_LOADED(".sdata") = 0x00808080u;
}
