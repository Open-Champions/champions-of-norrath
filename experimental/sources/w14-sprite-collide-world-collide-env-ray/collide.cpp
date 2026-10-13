// Observed: Collide__7VIWorldR8VIEnvRay at 0x010fe8f0, 112 bytes. Arguments: the world (W) and a ray record (R).
// Calls Collide__7VIWorldiR8VIEnvRayR7VIVect3T3(W, first, R, R, R+12), where `first` is 0 when the word at W+52 is
// nonzero and 0x40000000 when it is zero. The result is 1 if that call returned nonzero, otherwise 0. Nothing else is read or stored.
// Inferred: the integer is a start selector for the traversal and R+0 / R+12 are two 3-float vectors. Unknown: the meaning of the word at W+52
// and of the selector values.
extern "C" {
int Collide__7VIWorldiR8VIEnvRayR7VIVect3T3(unsigned char* world, int selector, unsigned char* ray, unsigned char* from, unsigned char* to);

int Collide__7VIWorldR8VIEnvRay(unsigned char* world, unsigned char* ray) {
    const int selector = *reinterpret_cast<int*>(world + 52) != 0 ? 0 : 0x40000000;
    return Collide__7VIWorldiR8VIEnvRayR7VIVect3T3(world, selector, ray, ray, ray + 12) != 0;
}
}
