#include "water_bump_speed.hpp"
// Observed worldWaterBumpSpeed at 0x00484148: four file-backed zero bytes in
// .sdata. worldWaterAlpha begins at the next byte. main stores the integer 1
// here when the word at pad+8 equals 16398. This is not an object already
// defined under a norrath_ name.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
int worldWaterBumpSpeed NORRATH_LOADED(".sdata") = 0;
}
