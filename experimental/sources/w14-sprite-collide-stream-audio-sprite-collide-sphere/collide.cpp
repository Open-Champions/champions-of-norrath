// Observed: Collide__19VIStreamAudioSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide at 0x010190d8, 8 bytes: returns 0 at once. Arguments (this, the
// sphere, the scene filter, the scene, the collide record) are not read, nothing is stored and nothing is
// called. This is a virtual of VIStreamAudioSprite (the slot is named only by the mangled symbol).
// Inferred: this class never reports a hit for this query. Unknown: why the class has the slot.
extern "C" int Collide__19VIStreamAudioSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide(unsigned char*, unsigned char*, unsigned char*, unsigned char*, unsigned char*) {
    return 0;
}
