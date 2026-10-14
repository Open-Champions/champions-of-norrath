// Observed StartAmbientSoundtrack__7VISetup at 0x01150d88, 8 bytes. It
// returns at once without reading or writing anything.
extern "C" {
struct VISetup;
void StartAmbientSoundtrack__7VISetup(VISetup* setup) {
    (void)setup;
}
}
