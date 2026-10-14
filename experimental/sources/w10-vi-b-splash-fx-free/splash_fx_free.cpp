// Observed VISplashFxFree__Fv at 0x01006f48, 76 bytes. If the word at
// 0x011e27e0 (the bird texture slot written by VISplashFxInit) is nonzero it
// is passed to ReleaseSurface__8VIRasteriR12VIDictionary together with the
// raster and dictionary words at 0x011e27d8 and 0x011e27dc. The slot is then
// set to -1 either way. The routine returns nothing. Note that -1 counts as
// nonzero for the next call, as observed.
extern "C" {
struct VIRaster;
struct VIDictionary;

extern VIRaster* norrath_splash_raster __asm__("VISplashFx$sRaster");
extern VIDictionary* norrath_splash_dictionary __asm__("VISplashFx$sDictionary");
extern int norrath_splash_bird_tex __asm__("VISplashFx$sBirdTex");

void ReleaseSurface__8VIRasteriR12VIDictionary(VIRaster* raster, int surface,
                                               VIDictionary* dictionary);

void VISplashFxFree__Fv() {
    if (norrath_splash_bird_tex != 0)
        ReleaseSurface__8VIRasteriR12VIDictionary(norrath_splash_raster, norrath_splash_bird_tex,
                                                  norrath_splash_dictionary);
    norrath_splash_bird_tex = -1;
}
}
