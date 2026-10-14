// Observed: __cl__7VIPlaneRC7VIVect3N21 at 0x010c2f90 is 344 bytes in the pinned SLUS-20565 image. The plane is this (four floats a, b,
// c, d) and the three arguments are points p0, p1, p2 (three floats each). It forms e1 = p1 - p0 and e2 = p2 - p1, stores the cross
// product e2 x e1 in plane[0..2] (x = e2.y*e1.z - e2.z*e1.y, y = e2.z*e1.x - e2.x*e1.z, z = e2.x*e1.y - e2.y*e1.x), calls
// Normalize__7VIVect3(plane) and, if that returns nonzero, stores d = -((a*p0.x + b*p0.y) + c*p0.z) at +12 and returns 1. If
// Normalize returns zero (degenerate triangle) d is set to 0.0f and 0 is returned; a, b and c keep the unnormalised cross product.
// Inferred: this is a call operator building a plane from three points; the plane equation is a*x + b*y + c*z + d = 0 with a unit
// normal, and the return value says whether the triangle was non-degenerate.
// Unknown: the winding convention beyond the formula above, and the class layout past 16 bytes. Nothing here ran on the EE.
extern "C" {
int Normalize__7VIVect3(float* v);

int __cl__7VIPlaneRC7VIVect3N21(float* plane, const float* p0, const float* p1, const float* p2) {
    float e1[3];
    float e2[3];
    e1[0] = p1[0] - p0[0];
    e1[1] = p1[1] - p0[1];
    e1[2] = p1[2] - p0[2];
    e2[0] = p2[0] - p1[0];
    e2[1] = p2[1] - p1[1];
    e2[2] = p2[2] - p1[2];
    plane[0] = e2[1] * e1[2] - e2[2] * e1[1];
    plane[1] = e2[2] * e1[0] - e2[0] * e1[2];
    plane[2] = e2[0] * e1[1] - e2[1] * e1[0];
    if (Normalize__7VIVect3(plane) == 0) {
        plane[3] = 0.0f;
        return 0;
    }
    plane[3] = -(plane[0] * p0[0] + plane[1] * p0[1] + plane[2] * p0[2]);
    return 1;
}
}
