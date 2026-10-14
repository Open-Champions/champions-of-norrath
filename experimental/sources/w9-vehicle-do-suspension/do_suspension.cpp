// Observed doSuspension__7Vehicle at 0x00273fe0, 1448 bytes. Nothing is
// returned. Nothing here was executed on the EE. Field meanings are not
// established here, so offsets stay numeric. n is the signed word at +1000
// (re-read after each accumForceAndTorque or per the original); for wheel i,
// A is float(+676+4i), B is float(+708+4i), q the pointer at +2056+4i, D the
// Point3 at +1428, and point_i the Point3 at +1536+12i. SPRING(x) is
// x * -q[1] when the word at +2144 is non-zero, otherwise x * -q[1] +
// ((x - B) * 60.0) * -(x < B ? q[2] * q[3] : q[2]).
//   1. State is saved: +608 <- Point3 at +1188, +632 <- Point3 at +1200, +620
//      <- Point3 at +16, +1100 <- 16 bytes at +1084. The Point3 values at
//      +2016 and +2028 are cleared.
//   2. For each wheel with A != 0.0: S[i] = SPRING(A), otherwise S[i] = 0.0; a
//      force D * S[i] is accumulated at point_i with flag 1 (so into the +2016 /
//      +2028 pair) when A != 0.
//   3. getNewRotAndPos(vehicle, +2016, +2028, step, 0, 1) with step the float 0x3c888888;
//      then the Point3 at +16 is copied to +644 and the 36 bytes at +1116 to +1152; the
//      saved values are copied back (+1188, +1200, +16 and the 16 bytes at
//      +1084) and quatToMat rebuilds the matrix at +1116 from +1084.
//   4. g = -(Point3 at +1176). For each wheel with S[i] != 0.0: p = matTransform(
//      matrix at +1152, point_i) + (Point3 at +644); d = p - (Point3 at
//      +1440+12i); T[i] = A + ((g.d as (gx*dx + gy*dy) + gz*dz) / (q[0] *
//      39.0)); otherwise T[i] = 0.0.
//   5. For each wheel with A != 0.0 or T[i] != 0.0: U = SPRING(T[i]); the force
//      D * ((U + S[i]) * 0.5) is accumulated at point_i with flag 0.
// Point3 values passed by value in the original are passed as the address of a
// caller-owned copy.
struct __attribute__((may_alias)) Point3 { float x, y, z; };
struct __attribute__((may_alias)) Quat { float x, y, z, w; };

extern "C" {
void accumForceAndTorque__7VehicleP6Point3T1i(unsigned char* vehicle, const Point3* point, const Point3* force, int flag);
void getNewRotAndPos__7VehicleP6Point3T1fii(unsigned char* vehicle, const Point3* first, const Point3* second, float step, int unused, int flag);
void quatToMat__FP8Matrix33P4Quat(void* matrix, Quat* q);
void matTransform__FP6Point3P8Matrix33T0(Point3* out, const void* matrix, const Point3* in);
}

typedef float AliasedFloat __attribute__((may_alias));
typedef int AliasedInt __attribute__((may_alias));
typedef unsigned char* AliasedPointer __attribute__((may_alias));

namespace {
inline int& word(unsigned char* base, int offset) { return *reinterpret_cast<AliasedInt*>(base + offset); }
inline float& real(unsigned char* base, int offset) { return *reinterpret_cast<AliasedFloat*>(base + offset); }
inline Point3* point(unsigned char* base, int offset) { return reinterpret_cast<Point3*>(base + offset); }

inline float spring(unsigned char* vehicle, const AliasedFloat* q, float x, float b) {
    float result = x * -q[1];
    if (word(vehicle, 2144) == 0) {
        const float scaled = (x - b) * 60.0f;
        float factor = q[2];
        if (x < b) factor = q[2] * q[3];
        result = result + scaled * -factor;
    }
    return result;
}
}

extern "C" void doSuspension__7Vehicle(unsigned char* vehicle) {
    float S[8];
    float T[8];
    Point3 force;
    *point(vehicle, 608) = *point(vehicle, 1188);
    *point(vehicle, 632) = *point(vehicle, 1200);
    *point(vehicle, 620) = *point(vehicle, 16);
    *reinterpret_cast<Quat*>(vehicle + 1100) = *reinterpret_cast<Quat*>(vehicle + 1084);
    Point3* const force_b = point(vehicle, 2016);
    Point3* const torque_b = point(vehicle, 2028);
    force_b->x = 0.0f; force_b->y = 0.0f; force_b->z = 0.0f;
    torque_b->x = 0.0f; torque_b->y = 0.0f; torque_b->z = 0.0f;

    for (int i = 0; i < word(vehicle, 1000); ++i) {
        const float a = real(vehicle, 676 + 4 * i);
        if (a == 0.0f) {
            S[i] = 0.0f;
            continue;
        }
        const AliasedFloat* const q = *reinterpret_cast<AliasedFloat**>(vehicle + 2056 + 4 * i);
        S[i] = spring(vehicle, q, a, real(vehicle, 708 + 4 * i));
        const Point3* const direction = point(vehicle, 1428);
        force.x = direction->x * S[i];
        force.y = direction->y * S[i];
        force.z = direction->z * S[i];
        accumForceAndTorque__7VehicleP6Point3T1i(vehicle, point(vehicle, 1536 + 12 * i), &force, 1);
    }

    getNewRotAndPos__7VehicleP6Point3T1fii(vehicle, force_b, torque_b, 0x1.111110p-6f, 0, 1);
    *point(vehicle, 644) = *point(vehicle, 16);
    for (int k = 0; k < 9; ++k) real(vehicle, 1152 + 4 * k) = real(vehicle, 1116 + 4 * k);
    *point(vehicle, 1188) = *point(vehicle, 608);
    *point(vehicle, 1200) = *point(vehicle, 632);
    *point(vehicle, 16) = *point(vehicle, 620);
    *reinterpret_cast<Quat*>(vehicle + 1084) = *reinterpret_cast<Quat*>(vehicle + 1100);
    quatToMat__FP8Matrix33P4Quat(vehicle + 1116, reinterpret_cast<Quat*>(vehicle + 1084));

    Point3 g;
    g.x = -real(vehicle, 1176);
    g.y = -real(vehicle, 1180);
    g.z = -real(vehicle, 1184);
    for (int i = 0; i < word(vehicle, 1000); ++i) {
        if (S[i] == 0.0f) {
            T[i] = 0.0f;
            continue;
        }
        Point3 p;
        Point3 d;
        matTransform__FP6Point3P8Matrix33T0(&p, vehicle + 1152, point(vehicle, 1536 + 12 * i));
        const Point3* const moved = point(vehicle, 644);
        p.x = p.x + moved->x;
        p.y = p.y + moved->y;
        p.z = p.z + moved->z;
        const Point3* const rest = point(vehicle, 1440 + 12 * i);
        const float a = real(vehicle, 676 + 4 * i);
        const AliasedFloat* const q = *reinterpret_cast<AliasedFloat**>(vehicle + 2056 + 4 * i);
        d.x = p.x - rest->x;
        d.y = p.y - rest->y;
        d.z = p.z - rest->z;
        T[i] = a + (g.x * d.x + g.y * d.y + g.z * d.z) / (q[0] * 39.0f);
    }

    for (int i = 0; i < word(vehicle, 1000); ++i) {
        const float a = real(vehicle, 676 + 4 * i);
        if (a == 0.0f && T[i] == 0.0f) continue;
        const AliasedFloat* const q = *reinterpret_cast<AliasedFloat**>(vehicle + 2056 + 4 * i);
        const float u = spring(vehicle, q, T[i], real(vehicle, 708 + 4 * i));
        const float amount = (u + S[i]) * 0.5f;
        const Point3* const direction = point(vehicle, 1428);
        force.x = direction->x * amount;
        force.z = direction->z * amount;
        force.y = direction->y * amount;
        accumForceAndTorque__7VehicleP6Point3T1i(vehicle, point(vehicle, 1536 + 12 * i), &force, 0);
    }
}
