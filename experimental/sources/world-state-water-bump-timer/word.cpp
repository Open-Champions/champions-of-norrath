#include "word.hpp"
// Observed worldWaterBumpTimer at 0x00484144: four file-backed bytes in .sdata. cvProcess passes this address to the waterBumpTimer integer conversion. The file bytes are 06 00 00 00, the integer 6.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
int worldWaterBumpTimer NORRATH_LOADED(".sdata") = 6;
}
