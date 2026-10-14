#include "storage.hpp"
// Observed frameStates at 0x006162c0 in .bss: 580224 bytes: animInit lays out 150 records of 3868 bytes and the symbol continues 24 bytes beyond them; animResults starts at 0x006a3d40.
// Inferred: nothing beyond the extent above; the layout of the bytes is not claimed.
// Unknown: other readers of this storage outside the cited routines.
extern "C" {
alignas(4) unsigned char frameStates[580224] = {};
}
