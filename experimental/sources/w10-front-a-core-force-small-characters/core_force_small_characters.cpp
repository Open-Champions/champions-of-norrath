// Observed: coreForceSmallCharacters at 0x00486678 is one byte in the initialized data of the
// image, with value 0. The player constructor and loadPlayerLumps__Fv read it as a signed byte:
// a nonzero value makes both treat the game as if two or more players were present when they
// choose the lump to load character data from, and loadPlayerLumps__Fv treat it as three or
// more when it chooses the low-detail character lumps.
// Inferred: a debug or core-mode switch that forces the smaller character models; the name says so.
// Unknown: who sets it. No code in the authored sources writes it. Section placement follows the
// build profile (-G0), not the original .data placement.

extern "C" signed char coreForceSmallCharacters;
signed char coreForceSmallCharacters = 0;
