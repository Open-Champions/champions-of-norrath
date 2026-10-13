// Observed: Collide__9VICSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide at 0x010b2e98, 60 bytes. Arguments: the sprite (S), the sphere, the scene filter, the scene and the
// collide record (C). If the word at S+4456 is nonzero the result is 0. Otherwise it tail-calls
// CollideObj__9VICollideR12VICollSphereR9VICapsule(C, sphere, S+4424) and returns its result.
// Inferred: S+4424 holds a capsule shape and S+4456 is a flag that disables it. Unknown: their names.
extern "C" {
int CollideObj__9VICollideR12VICollSphereR9VICapsule(unsigned char* collide, unsigned char* sphere, unsigned char* capsule);

int Collide__9VICSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide(unsigned char* self, unsigned char* sphere, unsigned char*, unsigned char*, unsigned char* collide) {
    if (*reinterpret_cast<int*>(self + 4456) != 0) return 0;
    return CollideObj__9VICollideR12VICollSphereR9VICapsule(collide, sphere, self + 4424);
}
}
