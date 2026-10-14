// Observed modelSetShadowScale__Ff at 0x0016cbe0, extent 8. Stores the float
// argument into modelShadowScale (0x00484080) and returns nothing.
// modelResetOptionalParams__Fv sets the same object to 1.0.
// Unknown: the valid range. modelShadowScale is defined by another package.
extern "C" {
extern float modelShadowScale;
void modelSetShadowScale__Ff(float scale) {
    modelShadowScale = scale;
}
}
