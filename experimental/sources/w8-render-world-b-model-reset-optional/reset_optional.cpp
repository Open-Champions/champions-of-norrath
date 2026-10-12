// Observed modelResetOptionalParams__Fv at 0x0016cd20, 48 bytes.
// Restores six model-draw globals and returns nothing:
//   0x00484084 (halos)        <- address of the object at 0x0034c018 (defaultHalos)
//   0x00484080 (modelShadowScale) <- 1.0f
//   0x00484094 (shadowBrightness) <- current value of 0x00484090 (globalShadowBrightness)
//   0x0048407d (modelDrawFade, one byte) <- 0
//   0x00484088 (nmHalos)      <- 0
//   0x0048408c (modelOption)  <- 0
// Names come from the executable's symbol table. None of these objects is
// defined in this package; they stay unresolved imports. The types of halos,
// nmHalos and modelOption are not known beyond their 4-byte width; they are
// declared as pointer-width / word-width values only because the function
// stores a pointer or zero there.
extern "C" {
extern char defaultHalos[];
extern const void* halos;
extern float modelShadowScale;
extern float shadowBrightness;
extern float globalShadowBrightness;
extern unsigned char modelDrawFade;
extern int nmHalos;
extern int modelOption;
void modelResetOptionalParams__Fv() {
    halos = defaultHalos;
    modelShadowScale = 1.0f;
    shadowBrightness = globalShadowBrightness;
    modelDrawFade = 0;
    nmHalos = 0;
    modelOption = 0;
}
}
