// Observed doAI__7Vehicle at 0x002749e0, 560 bytes. Nothing is returned.
// Nothing here was executed on the EE. Field meanings are not established here,
// so offsets stay numeric. R is the Point3 at +656. N is the pointer at +2320
// (its Point3 at +12 is called Q and its Point3 at +24 is called Q2) and M the
// pointer at +2324.
//   1. The word at +1020 becomes 0. d = distanceToPlane(N, Q2, R) is stored at
//      +828. When float(+852) < |d|: the Point3 at +596 becomes Q2, negated
//      component-wise when d > 0, and +1020 becomes 1; otherwise +596 becomes Q.
//   2. T (the Point3 at +1980) = (-m3, -m4, 0) scaled by rsqrt.s (1.0 /
//      sqrt of the squared length), where m3 and m4 are the floats at +1128 and
//      +1132. getBlockingVehicle__7VehiclePf(vehicle, +804) is stored at +2268.
//   3. s = min(1.0, 2 * float(M+56)) (min.s). e = distanceToPlane(M, Q, R);
//      u = (-e < 0 ? 0 : -e). k = s * (1.0 - min(1.0, u * 0.0016667)).
//      float(+852) = (1.0 - k) * 240.0; float(+848) = float(+832) +
//      (1.0 - float(+832)) * k.
//   4. When the word at +2268 is zero: float(+848) *= powf(max(0.0, Q . T),
//      3.0), the dot as (Qx*Tx + Qy*Ty) + Qz*Tz, max.s taking (0.0, dot).
//   5. When float(+848) < 2.0 it becomes 2.0.
struct __attribute__((may_alias)) Point3 { float x, y, z; };

extern "C" {
float distanceToPlane__FP6Point3N20(const Point3* a, const Point3* b, const Point3* c);
int getBlockingVehicle__7VehiclePf(unsigned char* vehicle, float* value);
float powf(float base, float exponent);
}

typedef float AliasedFloat __attribute__((may_alias));
typedef int AliasedInt __attribute__((may_alias));
typedef unsigned char* AliasedPointer __attribute__((may_alias));

namespace {
inline int& word(unsigned char* base, int offset) { return *reinterpret_cast<AliasedInt*>(base + offset); }
inline float& real(unsigned char* base, int offset) { return *reinterpret_cast<AliasedFloat*>(base + offset); }
inline float min_s(float a, float b) {
#if defined(__mips__)
    float out;
    __asm__("min.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b));
    return out;
#else
    return b < a ? b : a;
#endif
}
inline float max_s(float a, float b) {
#if defined(__mips__)
    float out;
    __asm__("max.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b));
    return out;
#else
    return b > a ? b : a;
#endif
}
inline float rsqrt_s(float numerator, float value) {
#if defined(__mips__)
    float out;
    __asm__("rsqrt.s %0, %1, %2" : "=f"(out) : "f"(numerator), "f"(value));
    return out;
#else
    return numerator / __builtin_sqrtf(value);
#endif
}
}

extern "C" void doAI__7Vehicle(unsigned char* vehicle) {
    Point3* const r = reinterpret_cast<Point3*>(vehicle + 656);
    Point3* const aim = reinterpret_cast<Point3*>(vehicle + 596);
    Point3* const t = reinterpret_cast<Point3*>(vehicle + 1980);
    word(vehicle, 1020) = 0;
    unsigned char* const n = *reinterpret_cast<AliasedPointer*>(vehicle + 2320);
    const Point3* const q = reinterpret_cast<Point3*>(n + 12);
    const Point3* const q2 = reinterpret_cast<Point3*>(n + 24);
    const float d = distanceToPlane__FP6Point3N20(reinterpret_cast<Point3*>(n), q2, r);
    real(vehicle, 828) = d;
    const float magnitude = d < 0.0f ? -d : d;
    if (real(vehicle, 852) < magnitude) {
        *aim = *q2;
        if (0.0f < real(vehicle, 828)) {
            aim->x = -aim->x;
            aim->y = -aim->y;
            aim->z = -aim->z;
        }
        word(vehicle, 1020) = 1;
    } else {
        *aim = *q;
    }

    const float nx = -real(vehicle, 1128);
    const float ny = -real(vehicle, 1132);
    t->z = 0.0f;
    const float inverse = rsqrt_s(1.0f, nx * nx + ny * ny);
    t->y = ny * inverse;
    t->x = nx * inverse;
    word(vehicle, 2268) = getBlockingVehicle__7VehiclePf(vehicle, reinterpret_cast<float*>(vehicle + 804));

    unsigned char* const m = *reinterpret_cast<AliasedPointer*>(vehicle + 2324);
    const float size = *reinterpret_cast<AliasedFloat*>(m + 56);
    const float s = min_s(1.0f, size + size);
    const float e = distanceToPlane__FP6Point3N20(reinterpret_cast<Point3*>(m), q, r);
    const float u = -e < 0.0f ? 0.0f : -e;
    const float k = s * (1.0f - min_s(1.0f, u * 0x1.b4e81ap-10f));
    const float old = real(vehicle, 832);
    real(vehicle, 852) = (1.0f - k) * 240.0f;
    real(vehicle, 848) = old + (1.0f - old) * k;
    if (word(vehicle, 2268) == 0) {
        const float dot = q->x * t->x + q->y * t->y + q->z * t->z;
        real(vehicle, 848) = real(vehicle, 848) * powf(max_s(0.0f, dot), 3.0f);
    }
    if (real(vehicle, 848) < 2.0f) real(vehicle, 848) = 2.0f;
}
