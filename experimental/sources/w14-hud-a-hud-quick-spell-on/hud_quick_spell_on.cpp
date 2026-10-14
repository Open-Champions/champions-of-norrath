// Observed: hudQuickSpellOn is the 16-byte initialised-data object at
// 0x0034b7a8; all 16 image bytes are zero. The authored quick-spell menu code
// indexes it by player and stores 0 or 1. Inferred: four ints, one per player.
extern "C" int hudQuickSpellOn[4] = {};
static_assert(sizeof(hudQuickSpellOn) == 16, "hudQuickSpellOn");
