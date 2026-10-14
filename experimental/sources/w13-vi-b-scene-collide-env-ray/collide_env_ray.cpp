// Observed: Collide__7VISceneR8VIEnvRay at 0x01145478, 60 bytes. It stores 1 into the
// word at ray + 52, calls Collide__7VIWorldR8VIEnvRay on the sub-object at scene + 4 with
// the same ray, then increments the word at scene + 1348 and returns the world call's
// result unchanged (the image does not touch the result register after the call).
// Inferred: scene + 4 is an embedded VIWorld; the ray word is a "query pending" flag and
// the scene word a query counter.
// Unknown: the member names, the meaning of the ray word, and the true return type (an
// int is assumed).
extern "C" {
int Collide__7VIWorldR8VIEnvRay(void* world, void* ray);

int Collide__7VISceneR8VIEnvRay(void* scene, void* ray) {
    unsigned char* s = static_cast<unsigned char*>(scene);
    *reinterpret_cast<int*>(static_cast<unsigned char*>(ray) + 52) = 1;
    int result = Collide__7VIWorldR8VIEnvRay(s + 4, ray);
    *reinterpret_cast<int*>(s + 1348) += 1;
    return result;
}
}
