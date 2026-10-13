// Observed bodyApplyForce__FP5_bodyG6Point3T1f at 0x0015b118, 808 bytes. Arguments:
// a0 = body, a1 = address of a by-value Point3 (call it A), a2 = address of a by-value
// Point3 (B), F12 = dt. Both Point3 values are copied to the stack at entry.
//  1. An omega quaternion is built from the angular velocity at body+140 with w = 0
//     and quatMul(&dq, &omega, &orientation) is called (all Quat pointers).
//  2. With half = dt * 0.5, the orientation (body+28: x, y, z, w) gains dq * half
//     component by component.
//  3. A and B are each multiplied by dt. Body+44 (three singles) gains A*dt, body+56
//     gains B*dt.
//  4. quatNormalize on the orientation, then the same rotation build, scratch matrix,
//     matTranspose, matMul and matUntransform sequence as bodyUpdateMatrix (0x15b440),
//     except that no second normalization is made.
// Inferred only: A is a force, B a torque, and body+44/+56 are linear/angular momentum.
// Fresh compilation is not an original-byte match.
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
void quatMul__FP4QuatN20(Quat* out, const Quat* a, const Quat* b);
void quatNormalize__FP4Quat(Quat*);
void matTranspose__FP8Matrix33T0(Matrix33* out, const Matrix33* in);
void matMul__FP8Matrix33N20(Matrix33* out, const Matrix33* a, const Matrix33* b);
void matUntransform__FP6Point3P8Matrix33T0(Point3* out, const Matrix33* matrix, const Point3* in);
}

namespace {
// Rotation matrix from the orientation quaternion (scale 2 / |q|^2), written to body+68.
__attribute__((always_inline)) inline void refresh_rotation(Body* body) {
    const float q0 = body->orientation.x;
    const float q1 = body->orientation.y;
    const float q2 = body->orientation.z;
    const float q3 = body->orientation.w;
    const float norm = (((q0 * q0) + (q1 * q1)) + (q2 * q2)) + (q3 * q3);
    const float s = 2.0f / norm;
    const float sz = q2 * s;
    const float sy = q1 * s;
    const float sx = q0 * s;
    const float zz = q2 * sz;
    const float yy = q1 * sy;
    const float xx = q0 * sx;
    const float wz = q3 * sz;
    const float wx = q3 * sx;
    const float xz = q0 * sz;
    const float wy = q3 * sy;
    const float yz = q1 * sz;
    const float xy = q0 * sy;
    float* r = body->rotation;
    r[5] = yz - wx;
    r[1] = xy - wz;
    r[2] = xz + wy;
    r[3] = xy + wz;
    r[8] = 1.0f - (xx + yy);
    r[0] = 1.0f - (yy + zz);
    r[4] = 1.0f - (xx + zz);
    r[6] = xz - wy;
    r[7] = yz + wx;
}
}

extern "C" void bodyApplyForce__FP5_bodyG6Point3T1f(Body* body, const Point3* first, const Point3* second,
                                                   float dt) {
    Point3 a = *first;
    Point3 b = *second;
    Quat omega;
    omega.x = body->angular_velocity.x;
    omega.y = body->angular_velocity.y;
    omega.z = body->angular_velocity.z;
    omega.w = 0.0f;
    Quat dq;
    quatMul__FP4QuatN20(&dq, &omega, &body->orientation);
    const float half = dt * 0.5f;
    body->orientation.w = body->orientation.w + dq.w * half;
    body->orientation.x = body->orientation.x + dq.x * half;
    body->orientation.y = body->orientation.y + dq.y * half;
    body->orientation.z = body->orientation.z + dq.z * half;
    a.x = a.x * dt;
    a.y = a.y * dt;
    a.z = a.z * dt;
    body->linear_momentum.x = body->linear_momentum.x + a.x;
    body->linear_momentum.y = body->linear_momentum.y + a.y;
    body->linear_momentum.z = body->linear_momentum.z + a.z;
    b.x = b.x * dt;
    b.y = b.y * dt;
    b.z = b.z * dt;
    body->angular_momentum.x = body->angular_momentum.x + b.x;
    body->angular_momentum.y = body->angular_momentum.y + b.y;
    body->angular_momentum.z = body->angular_momentum.z + b.z;
    quatNormalize__FP4Quat(&body->orientation);
    refresh_rotation(body);
    const float i1 = body->inverse_inertia[0];
    const float i2 = body->inverse_inertia[1];
    const float i3 = body->inverse_inertia[2];
    Matrix33 scaled;
    scaled.m[0] = body->rotation[0] * i1;
    scaled.m[1] = body->rotation[1] * i2;
    scaled.m[2] = body->rotation[2] * i3;
    scaled.m[3] = body->rotation[3] * i1;
    scaled.m[4] = body->rotation[4] * i2;
    scaled.m[5] = body->rotation[5] * i3;
    scaled.m[6] = body->rotation[6] * i1;
    scaled.m[7] = body->rotation[7] * i2;
    scaled.m[8] = body->rotation[8] * i3;
    Matrix33 transposed;
    matTranspose__FP8Matrix33T0(&transposed, reinterpret_cast<const Matrix33*>(body->rotation));
    matMul__FP8Matrix33N20(reinterpret_cast<Matrix33*>(body->world_inverse_inertia), &scaled, &transposed);
    matUntransform__FP6Point3P8Matrix33T0(&body->angular_velocity,
                                          reinterpret_cast<const Matrix33*>(body->world_inverse_inertia),
                                          &body->angular_momentum);
}
