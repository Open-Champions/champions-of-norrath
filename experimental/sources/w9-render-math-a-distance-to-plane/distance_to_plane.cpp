// Observed distanceToPlane__FP6Point3N20 at 0x0015a1d0, 72 bytes; all three arguments
// are pointers to three singles. With a0 = plane point P, a1 = normal N and a2 = query
// point Q, the result in F0 is the dot product (Q - P) . N, accumulated as
// ((n.x*dx + n.y*dy) + n.z*dz) with each difference taken first. The normal is not
// normalized. No memory is written. Fresh compilation is not an original-byte match.
struct Point3 {
    float x, y, z;
};

extern "C" float distanceToPlane__FP6Point3N20(const Point3* plane_point, const Point3* normal,
                                               const Point3* point) {
    const float dx = point->x - plane_point->x;
    const float dy = point->y - plane_point->y;
    const float dz = point->z - plane_point->z;
    return (normal->x * dx + normal->y * dy) + normal->z * dz;
}
