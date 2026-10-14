// Observed: Collide__8VISpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide at 0x01008130, 8 bytes, in the pinned SLUS-20565 image. It takes the sprite, a sphere, a filter,
// the scene and a collide record, reads none of them, writes nothing and returns 0.
// Inferred: the base sprite class reports no hit for a sphere test; derived sprites supply their own versions. Unknown: whether any caller relies on that default.
extern "C" {
int Collide__8VISpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide(unsigned char*, unsigned char*, unsigned char*, unsigned char*, unsigned char*) {
    return 0;
}
}
