// Observed: Collide__9VICollideR12VICollSphereR9VICapsule at 0x01095170 is 988 bytes. Arguments:
// the collide object C, a sphere probe P and a capsule K. Offsets are bytes. K holds a base point
// K[0..8], an axis vector K[12..20] and two floats K[24] (length) and K[28] (radius); P[0..8] is
// the probe centre and P[88] its radius. The function builds 32-byte "capsule-shaped" copies
// {base, axis, length, radius} on the stack, 16-byte spheres {x, y, z, radius}, and does:
//   1. grown = copy of K with radius K[28] + P[88];
//      if PointInSolid__9VICapsuleRC7VIVect3(grown, P centre) is zero (outside the grown capsule):
//        r = CollideNoCaps__9VICollideR9VICollRayRC10VICylinder(C, P, {base, axis, length, K[28] + P[88]}) != 0;
//        S = {base, K[28] + P[88]}; if Collide__9VICollideR9VICollRayRC8VISphere(C, P, S) then r = 1;
//        S += axis * K[24] (x, y, z only); if the same call returns nonzero then r = 1;
//        return r. All three calls are always made.
//   2. otherwise, with plain = {base, axis, K[24], K[28]}:
//      if PointInSolid__10VICylinderRC7VIVect3(plain, P centre) is nonzero, return 0;
//   3. otherwise wide = {base, axis, K[24], K[28] + P[88]};
//      if PointInSolid__10VICylinderRC7VIVect3(wide, P centre) is nonzero,
//        return CollideNoCaps(C, P, wide) != 0;
//   4. otherwise d = dot(P centre, axis) - dot(base, axis) (each dot summed x, then y, then z);
//      S = {base, K[28] + P[88]}; if !(d < 0.0f) then S += axis * K[24]; return
//      Collide__9VICollideR9VICollRayRC8VISphere(C, P, S) != 0.
// Inferred: this is capsule-versus-moving-sphere: the capsule is a cylinder with a spherical cap
// at each end, and "outside the grown capsule" runs the general swept test (side wall plus both
// caps), while the near-cap cases choose the single cap sphere nearest to the probe centre.
// Unknown: the members' real names, what the callees return beyond zero/nonzero, and why a probe
// centre inside the plain cylinder returns 0 (step 2).
extern "C" {
int PointInSolid__9VICapsuleRC7VIVect3(const float* capsule, const float* point);
int PointInSolid__10VICylinderRC7VIVect3(const float* cylinder, const float* point);
int CollideNoCaps__9VICollideR9VICollRayRC10VICylinder(void* collide, void* probe, const float* cylinder);
int Collide__9VICollideR9VICollRayRC8VISphere(void* collide, void* probe, const float* sphere);

int Collide__9VICollideR12VICollSphereR9VICapsule(void* collide, unsigned char* probe_bytes, unsigned char* capsule_bytes) {
    const float* probe = reinterpret_cast<const float*>(probe_bytes);
    const float* k = reinterpret_cast<const float*>(capsule_bytes);
    const float* axis = k + 3;
    alignas(16) float shape[8] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    alignas(16) float sphere[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    alignas(16) float grown[8] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    int result = 0;

    grown[0] = k[0];
    grown[1] = k[1];
    grown[2] = k[2];
    grown[3] = axis[0];
    grown[4] = axis[1];
    grown[5] = axis[2];
    grown[6] = k[6];
    grown[7] = k[7] + probe[22];
    if (!PointInSolid__9VICapsuleRC7VIVect3(grown, probe)) {
        shape[0] = k[0];
        shape[1] = k[1];
        shape[2] = k[2];
        shape[3] = axis[0];
        shape[4] = axis[1];
        shape[5] = axis[2];
        shape[6] = k[6];
        shape[7] = k[7] + probe[22];
        result = CollideNoCaps__9VICollideR9VICollRayRC10VICylinder(collide, probe_bytes, shape) != 0;
        sphere[0] = k[0];
        sphere[1] = k[1];
        sphere[2] = k[2];
        sphere[3] = k[7] + probe[22];
        if (Collide__9VICollideR9VICollRayRC8VISphere(collide, probe_bytes, sphere))
            result = 1;
        sphere[0] = sphere[0] + axis[0] * k[6];
        sphere[1] = sphere[1] + axis[1] * k[6];
        sphere[2] = sphere[2] + axis[2] * k[6];
        if (Collide__9VICollideR9VICollRayRC8VISphere(collide, probe_bytes, sphere))
            result = 1;
        return result;
    }

    shape[0] = k[0];
    shape[1] = k[1];
    shape[2] = k[2];
    shape[3] = axis[0];
    shape[4] = axis[1];
    shape[5] = axis[2];
    shape[6] = k[6];
    shape[7] = k[7];
    if (PointInSolid__10VICylinderRC7VIVect3(shape, probe))
        return 0;

    shape[0] = k[0];
    shape[1] = k[1];
    shape[2] = k[2];
    shape[3] = axis[0];
    shape[4] = axis[1];
    shape[5] = axis[2];
    shape[6] = k[6];
    shape[7] = k[7] + probe[22];
    if (PointInSolid__10VICylinderRC7VIVect3(shape, probe))
        return CollideNoCaps__9VICollideR9VICollRayRC10VICylinder(collide, probe_bytes, shape) != 0 ? 1 : 0;

    float along = probe[0] * axis[0] + probe[1] * axis[1] + probe[2] * axis[2] - (k[0] * axis[0] + k[1] * axis[1] + k[2] * axis[2]);
    sphere[0] = k[0];
    sphere[1] = k[1];
    sphere[2] = k[2];
    sphere[3] = k[7] + probe[22];
    if (!(along < 0.0f)) {
        sphere[0] = sphere[0] + axis[0] * k[6];
        sphere[1] = sphere[1] + axis[1] * k[6];
        sphere[2] = sphere[2] + axis[2] * k[6];
    }
    return Collide__9VICollideR9VICollRayRC8VISphere(collide, probe_bytes, sphere) != 0 ? 1 : 0;
}
}
