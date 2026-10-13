// Observed: Collide__12VISkinSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide at 0x0116d540, 8 bytes: returns 0 at once. Arguments (this, the
// ray, the scene filter, the scene, the collide record) are not read, nothing is stored and nothing is
// called. This is a virtual of VISkinSprite (the slot is named only by the mangled symbol).
// Inferred: this class never reports a hit for this query. Unknown: why the class has the slot.
extern "C" int Collide__12VISkinSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide(unsigned char*, unsigned char*, unsigned char*, unsigned char*, unsigned char*) {
    return 0;
}
