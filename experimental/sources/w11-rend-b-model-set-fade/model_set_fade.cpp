// Observed modelSetFade__Fc at 0x0016cbd8, extent 8. Stores its one-byte argument
// into modelDrawFade (0x0048407d, one byte) and returns nothing. modelDrawFade is
// the byte modelDraw hands to modelSetupCharacter; modelResetOptionalParams clears it.
// Unknown: the range of values the callers use and what the byte means to the renderer.
extern "C" {
extern signed char modelDrawFade;
void modelSetFade__Fc(signed char fade) {
    modelDrawFade = fade;
}
}
