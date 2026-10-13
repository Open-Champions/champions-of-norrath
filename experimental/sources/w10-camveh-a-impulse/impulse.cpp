// Observed impulse__7VehicleP6Point3N21fii at 0x002723e0, 712 bytes. Returns
// 0 or 1. Arguments after the vehicle: three Point3 pointers (point, normal,
// velocity), a float e, and two ints (skip_backup, flat). K is the float word
// 0x3cd20d20, M the matrix at vehicle+1116, Iinv the three floats at +1224..+1232
// and minv the float at +672.
//   v = velocity * -(e + 1.0), componentwise, with v.z = 0 when flat != 0;
//   s = (v.x*normal.x + v.y*normal.y) + v.z*normal.z; when s < 0.0 return 0.
//   r = (point - Point3 at +16) * K, with r.z = 0 when flat != 0;
//   T = r x normal; L = matUntransform(M, T); L = (L.x*Iinv[0], L.y*Iinv[1],
//   L.z*Iinv[2]); W = matTransform(M, L); U = W x r;
//   d = minv + (U.x*normal.x + U.y*normal.y) + U.z*normal.z (summed in that
//   order); j = s / d; impulse = normal * j.
//   When skip_backup is 0, backup__7Vehicle(vehicle) runs first; then
//   addImpulse__7VehicleP6Point3T1i(vehicle, &impulse, &r, 1); return 1.
// This reads as a collision response along a contact normal with the rotational
// term of an inverse inertia in the denominator; that reading, and the names of
// the arguments, are inferences, not established. Nothing here was executed on
// the EE.
struct __attribute__((may_alias)) Point3 { float x, y, z; };

extern "C" {
void matUntransform__FP6Point3P8Matrix33T0(Point3* out, const void* matrix, const Point3* in);
void matTransform__FP6Point3P8Matrix33T0(Point3* out, const void* matrix, const Point3* in);
void backup__7Vehicle(unsigned char* vehicle);
void addImpulse__7VehicleP6Point3T1i(unsigned char* vehicle, const Point3* impulse, const Point3* point, int flag);
}

typedef float AliasedFloat __attribute__((may_alias));

static inline Point3 cross(const Point3& a, const Point3& b) {
    Point3 c;
    c.x = a.y * b.z - a.z * b.y;
    c.y = a.z * b.x - a.x * b.z;
    c.z = a.x * b.y - a.y * b.x;
    return c;
}

extern "C" int impulse__7VehicleP6Point3N21fii(unsigned char* vehicle, const Point3* point, const Point3* normal, const Point3* velocity, float e, int skip_backup, int flat) {
    const float scale = -(e + 1.0f);
    Point3 v;
    v.z = velocity->z * scale;
    v.x = velocity->x * scale;
    v.y = velocity->y * scale;
    if (flat != 0) v.z = 0.0f;
    const Point3& n = *normal;
    const float s = (v.x * n.x + v.y * n.y) + v.z * n.z;
    if (s < 0.0f) return 0;
    const Point3* const position = reinterpret_cast<const Point3*>(vehicle + 16);
    const float k = 0x1.a41a40p-6f;
    Point3 r;
    r.x = point->x - position->x;
    r.y = point->y - position->y;
    const float dz = point->z - position->z;
    r.y = r.y * k;
    r.x = r.x * k;
    if (flat != 0) {
        r.z = 0.0f;
    } else {
        r.z = dz * k;
    }
    Point3 t = cross(r, n);
    Point3 l;
    matUntransform__FP6Point3P8Matrix33T0(&l, vehicle + 1116, &t);
    const AliasedFloat* const inertia = reinterpret_cast<AliasedFloat*>(vehicle + 1224);
    l.x = l.x * inertia[0];
    l.y = l.y * inertia[1];
    l.z = l.z * inertia[2];
    Point3 w;
    matTransform__FP6Point3P8Matrix33T0(&w, vehicle + 1116, &l);
    const float minv = *reinterpret_cast<AliasedFloat*>(vehicle + 672);
    const Point3 u = cross(w, r);
    const float denominator = minv + ((u.x * n.x + u.y * n.y) + u.z * n.z);
    const float j = s / denominator;
    Point3 impulse;
    impulse.x = n.x * j;
    impulse.y = n.y * j;
    impulse.z = n.z * j;
    if (skip_backup == 0) backup__7Vehicle(vehicle);
    addImpulse__7VehicleP6Point3T1i(vehicle, &impulse, &r, 1);
    return 1;
}
