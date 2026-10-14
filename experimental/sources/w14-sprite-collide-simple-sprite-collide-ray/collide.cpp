// Observed: Collide__14VISimpleSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide at 0x01167098, 28 bytes. Arguments: the sprite (S), the ray, the scene filter, the scene and
// the collide record (C). Tail-calls CollideObj__9VICollideR9VICollRayi(C, ray, word at S+68) and returns its result; the filter and
// scene arguments are not read.
// Inferred: the word at S+68 identifies the object whose collision shape is tested. Unknown: its name.
extern "C" {
int CollideObj__9VICollideR9VICollRayi(unsigned char* collide, unsigned char* shape, int object);

int Collide__14VISimpleSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide(unsigned char* self, unsigned char* shape, unsigned char*, unsigned char*, unsigned char* collide) {
    return CollideObj__9VICollideR9VICollRayi(collide, shape, *reinterpret_cast<int*>(self + 68));
}
}
