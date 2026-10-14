// Observed modelSetGlobalShadowBrightness__Ff at 0x0016ccf8, 24 bytes. One float
// argument. The argument is multiplied by 0.25f (0x3e80 in the high half of the
// literal, loaded as an immediate) and the product is stored to two float globals:
// shadowBrightness (0x00484094) and globalShadowBrightness (0x00484090). The
// meaning of the 0.25 scale is unknown.
extern "C" float shadowBrightness;
extern "C" float globalShadowBrightness;

extern "C" void modelSetGlobalShadowBrightness__Ff(float value) {
    float scaled = value * 0.25f;
    shadowBrightness = scaled;
    globalShadowBrightness = scaled;
}
