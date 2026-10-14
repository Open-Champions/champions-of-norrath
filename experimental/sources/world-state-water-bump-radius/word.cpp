#include "word.hpp"
// Observed worldWaterBumpRadius at 0x00484140: four file-backed bytes in .sdata. cvProcess passes this address to the waterBumpRadius float conversion. The file bytes are 00 00 80 40, the float 4.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
float worldWaterBumpRadius NORRATH_LOADED(".sdata") = 4.0f;
}
