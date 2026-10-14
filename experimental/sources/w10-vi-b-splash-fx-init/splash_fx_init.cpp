// Observed VISplashFxInit__FP8VIRasterP12VIDictionary at 0x01006ec8, 124 bytes.
// It stores its two arguments in the words at 0x011e27d8 (raster) and
// 0x011e27dc (dictionary), sets the word at 0x011e27e0 to -1, hashes the
// 18-character resource name "Bird_Animation.tga" with VIHashResourceID and
// calls VIDictionary::Find(dictionary, hash, <local type slot>, &0x011e27e0).
// A nonzero result calls VISplashFxReset__Fv and returns 0; zero returns -1.
// In the image these three words are local (sRaster, sDictionary, sBirdTex);
// they are published here under VISplashFx$ names so the sibling splash
// routines can share them. Which type the third Find argument receives is not
// established here; the local slot is never read.
extern "C" {
struct VIRaster;
struct VIDictionary;
struct VIResourceType;

VIRaster* norrath_splash_raster __asm__("VISplashFx$sRaster");
VIDictionary* norrath_splash_dictionary __asm__("VISplashFx$sDictionary");
int norrath_splash_bird_tex __asm__("VISplashFx$sBirdTex");

unsigned VIHashResourceID(const char* name);
int Find__12VIDictionaryUiR14VIResourceTypeRi(VIDictionary* dictionary, unsigned id,
                                              VIResourceType* type, int* out);
void VISplashFxReset__Fv();

int VISplashFxInit__FP8VIRasterP12VIDictionary(VIRaster* raster, VIDictionary* dictionary) {
    unsigned char type_slot[8];
    norrath_splash_raster = raster;
    norrath_splash_dictionary = dictionary;
    norrath_splash_bird_tex = -1;
    unsigned id = VIHashResourceID("Bird_Animation.tga");
    if (!Find__12VIDictionaryUiR14VIResourceTypeRi(
            dictionary, id, reinterpret_cast<VIResourceType*>(type_slot), &norrath_splash_bird_tex))
        return -1;
    VISplashFxReset__Fv();
    return 0;
}
}
