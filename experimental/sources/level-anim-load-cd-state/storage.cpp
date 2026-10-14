#include "storage.hpp"
// Observed cd at 0x004f3e90 in .bss: 64 bytes of zero-initialized storage; msgHandler starts at 0x004f3ed0. netFree at 0x00130de0 passes its address to sceInetClose, sceInetCtlDownInterface and sceLibnetTerminate.
// Inferred: nothing beyond the extent above; the layout of the bytes is not claimed.
// Unknown: other readers of this storage outside the cited routines.
extern "C" {
alignas(4) unsigned char cd[64] = {};
}
