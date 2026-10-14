// Observed: Pick__8VISpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster at 0x01008140 is 8 bytes in the pinned SLUS-20565
// image: it ignores every argument and returns zero.
// Inferred: this is the base-class pick of VISprite that reports no hit, and the zero is an int status.
// Unknown: which derived sprites override it, and what a non-zero result would mean to callers. Nothing here ran on the EE.
extern "C" {
int Pick__8VISpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster(unsigned char*, unsigned char*, unsigned char*, unsigned char*, unsigned char*) {
    return 0;
}
}
