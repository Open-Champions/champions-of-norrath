// Observed StopAmbientSoundtrack__7VISetup at 0x01150d90, 8 bytes. It
// returns at once without reading or writing anything.
extern "C" {
struct VISetup;
void StopAmbientSoundtrack__7VISetup(VISetup* setup) {
    (void)setup;
}
}
