// Observed: drewBitmap is the 16-byte initialised-data object at 0x00427458;
// all 16 image bytes are zero and the only code reference is one word store.
// The authored item-bitmap drawing code stores 1 at a per-player index.
// Inferred: four ints, one per player. Unknown: where it is cleared.
extern "C" int drewBitmap[4] = {};
static_assert(sizeof(drewBitmap) == 16, "drewBitmap");
