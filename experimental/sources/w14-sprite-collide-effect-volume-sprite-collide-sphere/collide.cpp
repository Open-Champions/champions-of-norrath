// Observed: Collide__20VIEffectVolumeSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide at 0x0114e770, 8 bytes: returns 0 at once. Arguments (this, the
// sphere, the scene filter, the scene, the collide record) are not read, nothing is stored and nothing is
// called. This is a virtual of VIEffectVolumeSprite (the slot is named only by the mangled symbol).
// Inferred: this class never reports a hit for this query. Unknown: why the class has the slot.
extern "C" int Collide__20VIEffectVolumeSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide(unsigned char*, unsigned char*, unsigned char*, unsigned char*, unsigned char*) {
    return 0;
}
