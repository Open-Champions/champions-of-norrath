#include "splash_fx_free.cpp"

extern "C" {
VIRaster* norrath_splash_raster __asm__("VISplashFx$sRaster");
VIDictionary* norrath_splash_dictionary __asm__("VISplashFx$sDictionary");
int norrath_splash_bird_tex __asm__("VISplashFx$sBirdTex");
}

static int calls;
static VIRaster* seen_raster;
static int seen_surface;
static VIDictionary* seen_dictionary;

extern "C" void ReleaseSurface__8VIRasteriR12VIDictionary(VIRaster* raster, int surface,
                                                          VIDictionary* dictionary) {
    ++calls;
    seen_raster = raster;
    seen_surface = surface;
    seen_dictionary = dictionary;
}

int main() {
    norrath_splash_raster = reinterpret_cast<VIRaster*>(0x10);
    norrath_splash_dictionary = reinterpret_cast<VIDictionary*>(0x20);
    norrath_splash_bird_tex = 9;
    VISplashFxFree__Fv();
    if (calls != 1 || seen_raster != norrath_splash_raster || seen_surface != 9 ||
        seen_dictionary != norrath_splash_dictionary || norrath_splash_bird_tex != -1)
        return 1;
    VISplashFxFree__Fv();  // -1 is nonzero, so it releases again
    if (calls != 2 || seen_surface != -1 || norrath_splash_bird_tex != -1) return 2;
    norrath_splash_bird_tex = 0;
    VISplashFxFree__Fv();
    if (calls != 2 || norrath_splash_bird_tex != -1) return 3;
    return 0;
}
