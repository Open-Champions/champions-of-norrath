// Observed: Release__8VISpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide at 0x01008158, 8 bytes. Takes the
// sprite, scene, dictionary, raster, sound device and collide object, uses none of them, and returns -1.
// Inferred: the base-class default, as for Copy__8VISpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei; derived sprites
// release their own buffers and the -1 reads as "not handled".
// Unknown: whether the base is ever reached through the virtual table.
extern "C" {
int Release__8VISpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(unsigned char*, unsigned char*, unsigned char*, unsigned char*, unsigned char*, unsigned char*) {
    return -1;
}
}
