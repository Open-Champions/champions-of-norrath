// Observed VIGetFeatureFlags at 0x010001a0, 12 bytes, in the overlay range. It returns the word sFeatureFlags
// (0x0118e540, file-local in the image) unchanged. The setter VISetFeatureFlags__FUi sits directly after it
// and stores into the same word.
// Callers: the battle-music unload and the options window drawing code, among others.
// Unknown: which bits callers test and where they are set; nothing is written by this routine.
extern "C" {
extern unsigned sFeatureFlags;

unsigned VIGetFeatureFlags__Fv() {
    return sFeatureFlags;
}
}
