// Observed: Collide__8VIEnvRayUifR7VIVect3 at 0x0116d2c0, 16 bytes, in the pinned SLUS-20565 image. Arguments: the ray (R), an unsigned mask, a float and a vector reference.
// The float and the vector are not read. It returns 1 when (mask AND the 32-bit word at R+44) is nonzero, else 0.
// Inferred: R+44 is a bit set of collision categories the ray answers to. Unknown: the meaning of the float and the vector (a distance and a direction or hit point is a guess only).
extern "C" {
int Collide__8VIEnvRayUifR7VIVect3(unsigned char* self, unsigned mask, float, const float*) {
    return (mask & *reinterpret_cast<unsigned*>(self + 44)) != 0;
}
}
