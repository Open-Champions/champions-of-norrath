// Observed: Raster__8VISpriteR8VICameraR7VISceneR8VIRasterUi at 0x01008128 is 8 bytes in the pinned SLUS-20565 image: it ignores
// every argument and returns zero.
// Inferred: this is the base-class draw hook of VISprite that draws nothing, and the zero is an int status.
// Unknown: which derived sprites override it, and what a non-zero result would mean to callers. Nothing here ran on the EE.
extern "C" {
int Raster__8VISpriteR8VICameraR7VISceneR8VIRasterUi(unsigned char*, unsigned char*, unsigned char*, unsigned char*, unsigned flags) {
    (void)flags;
    return 0;
}
}
