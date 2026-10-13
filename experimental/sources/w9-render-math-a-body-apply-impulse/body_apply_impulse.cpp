// Observed bodyApplyImpulse__FP5_bodyG6Point3T1f at 0x0015adf0, 808 bytes. Arguments:
// a0 = body, a1 = address of a by-value Point3 P (a world point), a2 = address of a
// by-value Point3 N, F12 = a single E. P and N are copied to the stack at entry.
//  1. bodyGetVelocity(body, &P, &v) gives the velocity v of the point.
//  2. vn = (v.x*N.x + v.y*N.y) + v.z*N.z. num = vn*E - vn. If num <= 0 (a NaN does not
//     take this exit) the function returns with the body untouched.
//  3. r = P - body[16..24]. c = r x N. matUntransform(&u, body+104, &c) gives u. The
//     second cross is cu = u x r. The denominator is body[0] + ((cu.x*N.x + cu.y*N.y)
//     + cu.z*N.z) and j = num / denominator.
//  4. J = N * j. Body+44 (three singles) gains J. With t = r x J, body+56 gains t.
//  5. matUntransform(body+140, body+104, body+56) refreshes the angular velocity. The
//     early return skips this call.
// Cross products are x = a.y*b.z - a.z*b.y, y = a.z*b.x - a.x*b.z, z = a.x*b.y - a.y*b.x.
// Inferred only: this is a collision impulse with E as a restitution-like factor, since
// the velocity at the point is the one produced by bodyGetVelocity. Fresh compilation is
// not an original-byte match.
struct Point3 {
    float x, y, z;
};
struct Quat {
    float x, y, z, w;
};
struct Matrix33 {
    float m[9];
};
// Byte offsets are observed from the 152-byte memset in bodyInit and from the accesses
// below. The roles in the member names are inferred, not observed.
struct Body {
    float inverse_mass;                 // 0
    float inverse_inertia[3];           // 4, 8, 12
    Point3 position;                    // 16
    Quat orientation;                   // 28
    Point3 linear_momentum;             // 44
    Point3 angular_momentum;            // 56
    float rotation[9];                  // 68
    float world_inverse_inertia[9];     // 104
    Point3 angular_velocity;            // 140
};
static_assert(sizeof(Body) == 152, "body size");

extern "C" {
void bodyGetVelocity__FP5_bodyP6Point3T1(const Body* body, const Point3* point, Point3* out);
void matUntransform__FP6Point3P8Matrix33T0(Point3* out, const Matrix33* matrix, const Point3* in);
}

namespace {
__attribute__((always_inline)) inline Point3 cross(const Point3& a, const Point3& b) {
    Point3 c;
    c.x = a.y * b.z - a.z * b.y;
    c.y = a.z * b.x - a.x * b.z;
    c.z = a.x * b.y - a.y * b.x;
    return c;
}
}

extern "C" void bodyApplyImpulse__FP5_bodyG6Point3T1f(Body* body, const Point3* point_arg, const Point3* normal_arg,
                                                     float e) {
    Point3 p = *point_arg;
    Point3 n = *normal_arg;
    Point3 v;
    bodyGetVelocity__FP5_bodyP6Point3T1(body, &p, &v);
    const float vn = (v.x * n.x + v.y * n.y) + v.z * n.z;
    const float num = vn * e - vn;
    if (num <= 0.0f) return;
    Point3 r;
    r.x = p.x - body->position.x;
    r.y = p.y - body->position.y;
    r.z = p.z - body->position.z;
    const float inverse_mass = body->inverse_mass;
    const Matrix33* inertia = reinterpret_cast<const Matrix33*>(body->world_inverse_inertia);
    Point3 c = cross(r, n);
    Point3 u;
    matUntransform__FP6Point3P8Matrix33T0(&u, inertia, &c);
    Point3 cu = cross(u, r);
    const float denominator = inverse_mass + ((cu.x * n.x + cu.y * n.y) + cu.z * n.z);
    const float j = num / denominator;
    Point3 impulse;
    impulse.z = n.z * j;
    impulse.x = n.x * j;
    impulse.y = n.y * j;
    body->linear_momentum.x = body->linear_momentum.x + impulse.x;
    body->linear_momentum.y = body->linear_momentum.y + impulse.y;
    body->linear_momentum.z = body->linear_momentum.z + impulse.z;
    Point3 t = cross(r, impulse);
    body->angular_momentum.x = body->angular_momentum.x + t.x;
    body->angular_momentum.y = body->angular_momentum.y + t.y;
    body->angular_momentum.z = body->angular_momentum.z + t.z;
    matUntransform__FP6Point3P8Matrix33T0(&body->angular_velocity, inertia, &body->angular_momentum);
}
