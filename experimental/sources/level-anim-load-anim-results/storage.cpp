#include "storage.hpp"
// Observed animResults at 0x006a3d40 in .bss: 156800 bytes: 100 records of 1568; identityResult starts at 0x006ca1c0.
// Inferred: nothing beyond the extent above; the layout of the bytes is not claimed.
// Unknown: other readers of this storage outside the cited routines.
extern "C" {
alignas(4) unsigned char animResults[156800] = {};
}
