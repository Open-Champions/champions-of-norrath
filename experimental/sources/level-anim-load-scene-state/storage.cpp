#include "storage.hpp"
// Observed scene at 0x004f4060 in .bss: 56 bytes of zero-initialized storage; savedScene starts at 0x004f4098. lightInit writes through byte 55.
// Inferred: nothing beyond the extent above; the layout of the bytes is not claimed.
// Unknown: other readers of this storage outside the cited routines.
extern "C" {
alignas(4) unsigned char scene[56] = {};
}
