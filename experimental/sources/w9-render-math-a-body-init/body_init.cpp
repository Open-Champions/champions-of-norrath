// Observed bodyInit__FP5_bodyfffff at 0x0015bcd0, 552 bytes. Arguments: a0 = body,
// F12..F16 = five singles; call them m, a, b, c, d.
//  1. memset(body, 0, 152) clears the whole body.
//  2. With am = a*m: body[4] = 12 / (am * (c*c + d*d)); body[8] = 12 / (am * (b*b + d*d));
//     body[12] = 12 / (am * (b*b + c*c)). body[0] = 1 / m. The sums are built as
//     (c*c + d*d), (b*b + c*c), (b*b + d*d), in the order the original evaluates them.
//  3. eRandState (0x00483e58) is advanced as state*0x19660d + 0x3c6ef35f (low 32 bits) and
//     the new state's upper 16 bits, converted to single and multiplied by 2^-16, are
//     stored at body+40 before the first normalization (the original stores it in that
//     call's delay slot). Body+36 is set to 1.0. body+28 and +32 stay zero.
//  4. quatNormalize on body+28, zeros at +56, +60, +64 (already zero), then
//     quatNormalize again.
//  5. The rotation matrix at body+68 is built as in bodyUpdateMatrix (0x15b440).
//  6. matUntransform(body+140, body+104, body+56) closes the function; the matrix at +104
//     is zero here, so the angular velocity ends up as whatever that call yields for zeros.
// Inferred only: m is a mass and b, c, d are box extents (the sums match a box moment of
// inertia 1/12 * m * (extent^2 + extent^2)); a is an extra factor of unknown meaning.
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
extern int eRandState;
void* memset(void*, int, unsigned);
void quatNormalize__FP4Quat(Quat*);
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

extern "C" void bodyInit__FP5_bodyfffff(Body* body, float m, float a, float b, float c, float d) {
    const float d2 = d * d;
    const float c2 = c * c;
    const float b2 = b * b;
    const float am = a * m;
    memset(body, 0, 152);
    const unsigned state = static_cast<unsigned>(eRandState) * 0x19660du + 0x3c6ef35fu;
    const float sum_cd = c2 + d2;
    const float sum_bc = b2 + c2;
    const float sum_bd = b2 + d2;
    const float inertia_x = am * sum_cd;
    const float inertia_z = am * sum_bc;
    const float inertia_y = am * sum_bd;
    body->orientation.z = 1.0f;
    const float inverse_mass = 1.0f / m;
    const float random_w = static_cast<float>(static_cast<int>(state >> 16)) * 1.52587890625e-05f;
    body->inverse_mass = inverse_mass;
    body->inverse_inertia[0] = 12.0f / inertia_x;
    body->inverse_inertia[2] = 12.0f / inertia_z;
    body->inverse_inertia[1] = 12.0f / inertia_y;
    eRandState = static_cast<int>(state);
    body->orientation.w = random_w;
    quatNormalize__FP4Quat(&body->orientation);
    body->angular_momentum.x = 0.0f;
    body->angular_momentum.y = 0.0f;
    body->angular_momentum.z = 0.0f;
    quatNormalize__FP4Quat(&body->orientation);
    refresh_rotation(body);
    matUntransform__FP6Point3P8Matrix33T0(&body->angular_velocity,
                                          reinterpret_cast<const Matrix33*>(body->world_inverse_inertia),
                                          &body->angular_momentum);
}
