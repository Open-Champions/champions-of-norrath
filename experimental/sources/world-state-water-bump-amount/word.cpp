#include "word.hpp"
// Observed worldWaterBumpAmount at 0x0048413c: four file-backed bytes in .sdata. cvProcess passes this address to the waterBumpAmount float conversion. The file bytes are 00 00 24 42, the float 41.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
float worldWaterBumpAmount NORRATH_LOADED(".sdata") = 41.0f;
}
