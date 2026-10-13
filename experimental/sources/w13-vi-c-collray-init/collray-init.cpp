// Observed: Init__9VICollRayRC7VIVect3T1 at 0x0109c610, 164 bytes. With the ray as this
// and two vectors a, b: the three floats of a are copied to this + 0, the three
// floats of b to this + 12, then Precalc__9VICollRay(this) is called. Afterwards 1.0f
// is stored at +68, -1 at +72, zero floats at +76, +80 and +84, and the three floats
// at +36 (written by Precalc) are copied to +56. The result is zero.
// Inferred: a is the ray origin and b its direction, +68 a hit parameter initialised
// to one, +72 a hit index initialised to -1, +76 a hit normal initialised to zero.
// Unknown: member names and what +36 and +56 hold exactly.
extern "C" {
void Precalc__9VICollRay(void* ray);

int Init__9VICollRayRC7VIVect3T1(void* ray, const float* origin, const float* direction) {
    float* r = static_cast<float*>(ray);
    r[0] = origin[0];
    r[1] = origin[1];
    r[2] = origin[2];
    r[3] = direction[0];
    r[4] = direction[1];
    r[5] = direction[2];
    Precalc__9VICollRay(ray);
    r[17] = 1.0f;
    *reinterpret_cast<int*>(r + 18) = -1;
    r[21] = 0.0f;
    r[19] = 0.0f;
    r[20] = 0.0f;
    r[14] = r[9];
    r[15] = r[10];
    r[16] = r[11];
    return 0;
}
}
