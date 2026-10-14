// Observed lightDisableVolumetricLighting__Fv at 0x0013e720, extent 12. Takes nothing and
// returns nothing. It stores 1 into the word disableVolumetricLighting (0x00483aa8).
// The authored lightSetupForCharacter treats any nonzero value as "do not query the world light
// map", so the stored 1 only has a known effect there. Unknown: who resets the word to 0.
extern "C" {
extern int disableVolumetricLighting;
void lightDisableVolumetricLighting__Fv() {
    disableVolumetricLighting = 1;
}
}
