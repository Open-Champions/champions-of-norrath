// Observed dualImpulse__7VehicleP7VehicleT1P6Point3T3 at 0x002726a8, 1292
// bytes. Returns 0 or 1. The first argument (`this`) is never read; the code
// works on the next two Vehicle pointers, called A and B here, a contact point
// and a normal (Point3 pointers). K is the float word 0x3cd20d20, M_X the
// matrix at X+1116, Iinv_X the Point3 at X+1224, minv_X the float at X+672.
//   vA, vB = getParticleVelocity__7VehicleP6Point3(A, point), (B, point), each
//   with z forced to 0.0; rel = vA - vB (x, y, z);
//   scale = -2.0 when the word at A+2144 or at B+2144 is non-zero, otherwise -1.5;
//   s = ((rel.x*scale)*normal.x + (rel.y*scale)*normal.y) + (rel.z*scale)*normal.z;
//   when s < 0.0 return 0.
//   For X in A, B: r = ((point.x - pos_X.x) * K, (point.y - pos_X.y) * K, 0.0)
//   with pos_X the Point3 at X+16; T = (r.y*n.z, -(r.x*n.z), r.x*n.y - r.y*n.x);
//   J = matTransform(M_X, Iinv_X); W = (T.x*J.x, T.y*J.y, T.z*J.z);
//   U = W x r (x: W.y*r.z - W.z*r.y, y: W.z*r.x - W.x*r.z, z: W.x*r.y - W.y*r.x);
//   d_X = max(0.0, (U.x*n.x + U.y*n.y) + U.z*n.z) (max.s).
//   A's terms are all computed before B's.
//   j = s / (((minv_A + minv_B) + d_A) + d_B);
//   backupJustForces__7Vehicle(A); backupJustForces__7Vehicle(B);
//   addImpulse__7VehicleP6Point3T1i(A, &(normal * j), &r_A, 0);
//   addImpulse__7VehicleP6Point3T1i(B, &(normal * -j), &r_B, 0); return 1.
// The original also tests the words at A+2148 and B+2148 right before the
// first addImpulse, but both branches multiply j by the same constant 1.0, so
// nothing depends on them and they are not read here. Reading this as an
// impulse exchanged between two bodies at a contact is an inference; field
// meanings are not established. Nothing here was executed on the EE.
struct __attribute__((may_alias)) Point3 { float x, y, z; };

extern "C" {
Point3* getParticleVelocity__7VehicleP6Point3(Point3* out, unsigned char* vehicle, const Point3* point);
void matTransform__FP6Point3P8Matrix33T0(Point3* out, const void* matrix, const Point3* in);
void backupJustForces__7Vehicle(unsigned char* vehicle);
void addImpulse__7VehicleP6Point3T1i(unsigned char* vehicle, const Point3* impulse, const Point3* point, int flag);
}

typedef float AliasedFloat __attribute__((may_alias));
typedef int AliasedInt __attribute__((may_alias));

static inline float fp_max(float a, float b) {
#if defined(__mips__)
    float out;
    __asm__("max.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b));
    return out;
#else
    return a > b ? a : b;
#endif
}

namespace {
inline float real(unsigned char* base, int offset) { return *reinterpret_cast<AliasedFloat*>(base + offset); }

// Everything the original computes for one vehicle once the contact lever
// arm r is known: the rotational denominator term.
inline float rotational_term(unsigned char* vehicle, const Point3& r, const Point3& n, Point3& torque) {
    torque.x = r.y * n.z;
    torque.y = -(r.x * n.z);
    torque.z = r.x * n.y - r.y * n.x;
    Point3 j;
    matTransform__FP6Point3P8Matrix33T0(&j, vehicle + 1116, reinterpret_cast<const Point3*>(vehicle + 1224));
    Point3 w;
    w.x = torque.x * j.x;
    w.y = torque.y * j.y;
    w.z = torque.z * j.z;
    Point3 u;
    u.x = w.y * r.z - w.z * r.y;
    u.y = w.z * r.x - w.x * r.z;
    u.z = w.x * r.y - w.y * r.x;
    return fp_max((u.x * n.x + u.y * n.y) + u.z * n.z, 0.0f);
}
}

extern "C" int dualImpulse__7VehicleP7VehicleT1P6Point3T3(unsigned char* self, unsigned char* a, unsigned char* b, const Point3* point, const Point3* normal) {
    (void)self;
    Point3 va;
    Point3 vb;
    getParticleVelocity__7VehicleP6Point3(&va, a, point);
    getParticleVelocity__7VehicleP6Point3(&vb, b, point);
    va.z = 0.0f;
    vb.z = 0.0f;
    Point3 rel;
    rel.x = va.x - vb.x;
    rel.y = va.y - vb.y;
    rel.z = va.z - vb.z;
    float scale;
    if (*reinterpret_cast<AliasedInt*>(a + 2144) != 0 || *reinterpret_cast<AliasedInt*>(b + 2144) != 0) {
        scale = -2.0f;
    } else {
        scale = -1.5f;
    }
    const float s = (rel.x * scale * normal->x + rel.y * scale * normal->y) + rel.z * scale * normal->z;
    if (s < 0.0f) return 0;
    const float k = 0x1.a41a40p-6f;
    const Point3* const pos_a = reinterpret_cast<const Point3*>(a + 16);
    const Point3* const pos_b = reinterpret_cast<const Point3*>(b + 16);
    Point3 r_a;
    r_a.x = (point->x - pos_a->x) * k;
    r_a.y = (point->y - pos_a->y) * k;
    r_a.z = 0.0f;
    Point3 torque_a;
    const float d_a = rotational_term(a, r_a, *normal, torque_a);
    Point3 r_b;
    r_b.x = (point->x - pos_b->x) * k;
    r_b.y = (point->y - pos_b->y) * k;
    r_b.z = 0.0f;
    Point3 torque_b;
    const float d_b = rotational_term(b, r_b, *normal, torque_b);
    const float denominator = ((real(a, 672) + real(b, 672)) + d_a) + d_b;
    const float j = s / denominator;
    backupJustForces__7Vehicle(a);
    backupJustForces__7Vehicle(b);
    Point3 impulse;
    impulse.x = normal->x * j;
    impulse.y = normal->y * j;
    impulse.z = normal->z * j;
    addImpulse__7VehicleP6Point3T1i(a, &impulse, &r_a, 0);
    const float negated = -j;
    impulse.x = normal->x * negated;
    impulse.y = normal->y * negated;
    impulse.z = normal->z * negated;
    addImpulse__7VehicleP6Point3T1i(b, &impulse, &r_b, 0);
    return 1;
}
