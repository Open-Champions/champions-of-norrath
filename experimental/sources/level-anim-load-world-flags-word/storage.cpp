#include "storage.hpp"
// Observed levelFlags at 0x004844c0 in .sdata: four file-backed zero bytes; the factory reads the level flag word here.
// Inferred: nothing beyond the extent above; the layout of the bytes is not claimed.
// Unknown: other readers of this storage outside the cited routines.
extern "C" {
alignas(4) unsigned char norrath_factory_world_flags_word[4] = {};
}
