// Observed: PointInSolid__C8VISphereRC7VIVect3 at 0x01005320, 104 bytes, in the pinned SLUS-20565 image. Arguments: the sphere (centre x, y, z
// then radius at byte offset 12) and a point. The difference d = point - centre is formed per lane and also stored to a 12-byte stack
// temporary (a by-value vector). The result is 1 when dx*dx + dy*dy + dz*dz <= radius*radius (inclusive), computed as (dx*dx + dy*dy) + dz*dz, otherwise 0.
// Inferred: a point-in-ball test with the boundary counted as inside. Unknown: whether radius may be negative (the square hides the sign).
extern "C" {
int PointInSolid__C8VISphereRC7VIVect3(const float* self, const float* point) {
    float d[3];
    d[0] = point[0] - self[0];
    d[1] = point[1] - self[1];
    d[2] = point[2] - self[2];
    return d[0] * d[0] + d[1] * d[1] + d[2] * d[2] <= self[3] * self[3] ? 1 : 0;
}
}
