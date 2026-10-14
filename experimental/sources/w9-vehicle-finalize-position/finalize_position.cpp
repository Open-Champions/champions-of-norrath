// Observed finalizePosition__7Vehicle at 0x00272f70, 1116 bytes. Returns 0
// normally and 1 when impulse__7VehicleP6Point3N21fii reports non-zero in the
// ray pass. Nothing here was executed on the EE. The meaning of the fields is
// not established here; offsets stay numeric. n is the signed word at +1000,
// re-read at each loop test. P is the Point3 at +16 and M the matrix at +1116.
//   1. For i below n: the Point3 at +1332+12i becomes matTransform(M, the
//      Point3 at +1236+12i) plus P.
//   2. The Point3 at +1428 becomes the negated third matrix row (+1140..+1148).
//      objectUpdateInGrid__FP10GameObject runs on the vehicle.
//   3. For i below n: with q = the pointer at +2056+4i, the point W = +1332+12i
//      and the direction D = +1428, rayHit is called with (vehicle, W, D,
//      float(*q) * 39.0, hit) and returns f. The float at +676+4i moves to
//      +708+4i and +676+4i is cleared. When f is not -9999.0 and f < 1.0:
//      if f < 0.0 then f becomes 0.0 after getParticleVelocity on W, and
//      impulse(vehicle, W, hit, velocity, 0.5, 0, 0) runs; a non-zero result
//      returns 1 at once. The float at +676+4i becomes 1.0 - f, and the hit
//      Point3 is stored at +1740+12i.
//   4. For i below n: with c = D * (1.0 - float(+676+4i)) and m = float(*q) *
//      39.0, Q = W + (c * m) is stored at +1440+12i; the vector Q - P is
//      un-rotated by M into +1536+12i; getParticleVelocity(Q) is stored at
//      +1836+12i; FinalizePosition__5Wheel runs on the pointer at +1052+4i.
//   5. resolveCreatureCollisions__7Vehicle runs. The float at +840 becomes the
//      square root of the sum of squares of the Point3 at +1188 (0.0 if that is
//      below zero), and the word at +2160 becomes frameNm.
// Point3 values passed by value in the original are passed here as the
// address of a caller-owned copy, and the Point3 result of
// getParticleVelocity is returned through its first (hidden) pointer.
struct __attribute__((may_alias)) Point3 { float x, y, z; };

extern "C" {
void matTransform__FP6Point3P8Matrix33T0(Point3* out, const void* matrix, const Point3* in);
void matUntransform__FP6Point3P8Matrix33T0(Point3* out, const void* matrix, const Point3* in);
void objectUpdateInGrid__FP10GameObject(void* object);
float rayHit__7VehicleP6Point3T1fT1(unsigned char* vehicle, const Point3* from, const Point3* direction, float distance, Point3* hit);
void getParticleVelocity__7VehicleP6Point3(Point3* result, unsigned char* vehicle, const Point3* point);
int impulse__7VehicleP6Point3N21fii(unsigned char* vehicle, const Point3* point, const Point3* normal, const Point3* velocity, float factor, int a, int b);
void FinalizePosition__5Wheel(void* wheel);
void resolveCreatureCollisions__7Vehicle(unsigned char* vehicle);
extern int frameNm;
}

typedef float AliasedFloat __attribute__((may_alias));
typedef int AliasedInt __attribute__((may_alias));
typedef unsigned char* AliasedPointer __attribute__((may_alias));

static inline int& word(unsigned char* base, int offset) {
    return *reinterpret_cast<AliasedInt*>(base + offset);
}
static inline float& real(unsigned char* base, int offset) {
    return *reinterpret_cast<AliasedFloat*>(base + offset);
}
static inline Point3* point(unsigned char* base, int offset) {
    return reinterpret_cast<Point3*>(base + offset);
}

extern "C" int finalizePosition__7Vehicle(unsigned char* vehicle) {
    for (int i = 0; i < word(vehicle, 1000); ++i) {
        Point3* const target = point(vehicle, 1332 + 12 * i);
        matTransform__FP6Point3P8Matrix33T0(target, vehicle + 1116, point(vehicle, 1236 + 12 * i));
        const Point3* const position = point(vehicle, 16);
        target->x = target->x + position->x;
        target->y = target->y + position->y;
        target->z = target->z + position->z;
    }
    real(vehicle, 1428) = -real(vehicle, 1140);
    real(vehicle, 1432) = -real(vehicle, 1144);
    real(vehicle, 1436) = -real(vehicle, 1148);
    objectUpdateInGrid__FP10GameObject(vehicle);

    for (int i = 0; i < word(vehicle, 1000); ++i) {
        Point3 hit;
        Point3 velocity;
        Point3 raw;
        unsigned char* const q = *reinterpret_cast<AliasedPointer*>(vehicle + 2056 + 4 * i);
        const float f = rayHit__7VehicleP6Point3T1fT1(vehicle, point(vehicle, 1332 + 12 * i), point(vehicle, 1428),
                                                      *reinterpret_cast<AliasedFloat*>(q) * 39.0f, &hit);
        const bool found = f != -9999.0f;
        const float previous = real(vehicle, 676 + 4 * i);
        real(vehicle, 676 + 4 * i) = 0.0f;
        real(vehicle, 708 + 4 * i) = previous;
        if (!found) continue;
        float fraction = f;
        if (!(fraction < 1.0f)) continue;
        if (fraction < real(vehicle, 676 + 4 * i)) {
            fraction = real(vehicle, 676 + 4 * i);
            getParticleVelocity__7VehicleP6Point3(&raw, vehicle, point(vehicle, 1332 + 12 * i));
            velocity = raw;
            if (impulse__7VehicleP6Point3N21fii(vehicle, point(vehicle, 1332 + 12 * i), &hit, &velocity, 0.5f, 0, 0) != 0) return 1;
        }
        real(vehicle, 676 + 4 * i) = 1.0f - fraction;
        *point(vehicle, 1740 + 12 * i) = hit;
    }

    for (int i = 0; i < word(vehicle, 1000); ++i) {
        Point3 offset;
        Point3 scaled;
        Point3 result;
        Point3 to_point;
        const float remaining = 1.0f - real(vehicle, 676 + 4 * i);
        const Point3* const direction = point(vehicle, 1428);
        const Point3* const wheel_point = point(vehicle, 1332 + 12 * i);
        offset.x = direction->x * remaining;
        offset.y = direction->y * remaining;
        offset.z = direction->z * remaining;
        unsigned char* const q = *reinterpret_cast<AliasedPointer*>(vehicle + 2056 + 4 * i);
        const float m = *reinterpret_cast<AliasedFloat*>(q) * 39.0f;
        scaled.x = offset.x * m;
        scaled.y = offset.y * m;
        scaled.z = offset.z * m;
        Point3* const slot = point(vehicle, 1440 + 12 * i);
        slot->x = wheel_point->x + scaled.x;
        slot->y = wheel_point->y + scaled.y;
        slot->z = wheel_point->z + scaled.z;
        const Point3* const position = point(vehicle, 16);
        to_point.x = slot->x - position->x;
        to_point.y = slot->y - position->y;
        to_point.z = slot->z - position->z;
        matUntransform__FP6Point3P8Matrix33T0(point(vehicle, 1536 + 12 * i), vehicle + 1116, &to_point);
        getParticleVelocity__7VehicleP6Point3(&result, vehicle, slot);
        *point(vehicle, 1836 + 12 * i) = result;
        FinalizePosition__5Wheel(*reinterpret_cast<AliasedPointer*>(vehicle + 1052 + 4 * i));
    }

    resolveCreatureCollisions__7Vehicle(vehicle);
    const Point3* const v = point(vehicle, 1188);
    const float length = __builtin_sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
    real(vehicle, 840) = length;
    if (length < 0.0f) real(vehicle, 840) = 0.0f;
    word(vehicle, 2160) = frameNm;
    return 0;
}
