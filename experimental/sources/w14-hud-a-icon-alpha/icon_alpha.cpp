// Observed: iconAlpha is the 112-byte bss object at 0x007d0860 (the next symbol
// starts 0x70 bytes later), referenced three times by the HUD run routine.
// Inferred: 28 ints, seven icon slots for each of four players, matching the
// seven-slot indexing used by the authored HUD run code. Unknown: units.
extern "C" int iconAlpha[28] = {};
static_assert(sizeof(iconAlpha) == 112, "iconAlpha");
