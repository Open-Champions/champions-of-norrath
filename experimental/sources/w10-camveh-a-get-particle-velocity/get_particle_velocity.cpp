// Observed getParticleVelocity__7VehicleP6Point3 at 0x002722b8, 296 bytes.
// The Point3 result comes back through a hidden first pointer (returned in
// v0), then the vehicle, then a pointer to a Point3 argument p. Steps, with
// pos the Point3 at vehicle+16, M the 3x3 matrix at +1116, w the Point3 at
// +1200, vel the Point3 at +1188 and K the float word 0x3cd20d20:
//   d = (p - pos) * K, componentwise;
//   a = matTransform__FP6Point3P8Matrix33T0(M, w) -- the call writes a Point3;
//   result = (a x d) + vel, with the cross product
//     (a.y*d.z - a.z*d.y, a.z*d.x - a.x*d.z, a.x*d.y - a.y*d.x).
// Reading w as an angular velocity and the result as the velocity of a point on
// the body is an inference from the arithmetic; the field meanings are not
// established. Nothing here was executed on the EE.
struct __attribute__((may_alias)) Point3 { float x, y, z; };

extern "C" void matTransform__FP6Point3P8Matrix33T0(Point3* out, const void* matrix, const Point3* in);

extern "C" Point3* getParticleVelocity__7VehicleP6Point3(Point3* out, unsigned char* vehicle, const Point3* p) {
    const Point3* const pos = reinterpret_cast<const Point3*>(vehicle + 16);
    const Point3* const vel = reinterpret_cast<const Point3*>(vehicle + 1188);
    Point3 d;
    Point3 a;
    Point3 c;
    const float k = 0x1.a41a40p-6f;
    d.x = (p->x - pos->x) * k;
    d.y = (p->y - pos->y) * k;
    d.z = (p->z - pos->z) * k;
    matTransform__FP6Point3P8Matrix33T0(&a, vehicle + 1116, reinterpret_cast<const Point3*>(vehicle + 1200));
    c.x = a.y * d.z - a.z * d.y;
    c.y = a.z * d.x - a.x * d.z;
    c.z = a.x * d.y - a.y * d.x;
    out->x = c.x + vel->x;
    out->y = c.y + vel->y;
    out->z = c.z + vel->z;
    return out;
}
