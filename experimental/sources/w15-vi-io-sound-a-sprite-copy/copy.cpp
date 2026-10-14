// Observed: Copy__8VISpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei at 0x01008150, 8 bytes. Takes the sprite, scene, raster,
// sound device, collide object and a number, uses none of them, and returns -1.
// Inferred: this is the base-class default. Derived sprites define their own Copy (see the VIFloraSprite and VIGroupSprite
// versions) and the -1 reads as "not handled".
// Unknown: whether the base is ever reached through the virtual table.
extern "C" {
int Copy__8VISpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei(unsigned char*, unsigned char*, unsigned char*, unsigned char*, unsigned char*, int) {
    return -1;
}
}
