// Observed: EnableAllDirLights__8VIRasteri at 0x01114068, 16 bytes. The integer
// argument is ignored. It stores 1 at raster + 17888 and returns 0. The same word is
// set to 1 by SetFog, EnableAmbient and EnableSunLight.
// Inferred: raster + 17888 is a "lighting state changed" flag consumed elsewhere.
// Unknown: the member name, and why the argument is accepted and discarded.
extern "C" {
int EnableAllDirLights__8VIRasteri(void* raster, int) {
    *reinterpret_cast<int*>(static_cast<unsigned char*>(raster) + 17888) = 1;
    return 0;
}
}
