// Observed modelSetShadowBrightness__Ff at 0x0016cd10, 16 bytes.
// The argument is multiplied by the float global at 0x00484090 and the
// product is stored to the float global at 0x00484094. Nothing is returned.
// Both names come from the executable's own symbol table and neither is
// defined in this package; they stay unresolved imports.
extern "C" {
extern float globalShadowBrightness;
extern float shadowBrightness;
void modelSetShadowBrightness__Ff(float scale) {
    shadowBrightness = scale * globalShadowBrightness;
}
}
