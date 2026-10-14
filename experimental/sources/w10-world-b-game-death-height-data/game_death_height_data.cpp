// Observed gameDeathHeight at 0x00484cc4, 4 bytes of initialised data in SLUS_205.65. The
// stored word is 0xc7c35000, the float -100000.0. Code reads it as a float (and a loader
// writes it); the code reading it is not part of this unit.
// Inferred: a world-height limit below which something counts as dead, from its name and the
// users in the loader and the death-screen check. Unknown: the writers and their units.
extern "C" {
float gameDeathHeight = -100000.0f;
}
