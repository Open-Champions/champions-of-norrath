#include "storage.hpp"
// Observed world at 0x004844a0 in .sdata: four file-backed zero bytes; the factory and door code read the world header pointer here.
// Inferred: nothing beyond the extent above; the layout of the bytes is not claimed.
// Unknown: other readers of this storage outside the cited routines.
extern "C" {
alignas(4) unsigned char norrath_factory_world_pointer_word[4] = {};
}
