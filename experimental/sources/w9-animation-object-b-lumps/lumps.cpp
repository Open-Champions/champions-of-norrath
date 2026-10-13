// Storage for lumps (0x6cb280, .bss, 5120 bytes up to discardable at 0x6cc680).
// The observed extent and the 40-byte entry layout are described in lumps.hpp.
#include "lumps.hpp"
extern "C" {
LumpEntry lumps[128];
}
static_assert(sizeof(lumps) == 128 * sizeof(LumpEntry), "table extent");
