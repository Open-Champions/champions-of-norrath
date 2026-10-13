// Observed getBlockingVehicle__7VehiclePf at 0x00274c10, 448 bytes. Returns a
// vehicle pointer or 0. With `this` the vehicle, m the nine floats at +1116,
// back = (-m[3], -m[4], -m[5]) and side = (m[0], m[1], m[2]), P the Point3 at
// +16, R the float at +836, S the float at +840, and the global array
// vehicleList / count vehicleCount (re-read each pass), every index i other
// than the word at +2156 is tried:
//   o = vehicleList[i];
//   skip when S < (velocity at o+1188) . back, summed x, y, z in that order;
//   d1 = distanceToPlane__FP6Point3N20(P, back, o + 16);
//   gap = d1 - (float at o+836); skip unless 0 < gap;
//   limit = R * (min(1.0, S * K1) * K2 + K3) with K1 = 0x3dcccccc,
//       K2 = 0x3ff33334, K3 = 0x40066666; skip unless gap < limit;
//   d2 = distanceToPlane__FP6Point3N20(P, side, o + 16);
//   skip unless |d2| < (R + float at o+836) * 0.75;
//   otherwise store d2 through the argument when it is not null and return o.
// No match returns 0 and leaves the output alone. Reading the first plane as
// "in front of" and the second as "beside" is an inference from the arithmetic.
// Nothing here was executed on the EE.
struct __attribute__((may_alias)) Point3 { float x, y, z; };

extern "C" {
float distanceToPlane__FP6Point3N20(const Point3* a, const Point3* b, const Point3* c);
extern int vehicleCount;
extern unsigned char* vehicleList[];
}

typedef float AliasedFloat __attribute__((may_alias));
typedef int AliasedInt __attribute__((may_alias));

static inline float fp_min(float a, float b) {
#if defined(__mips__)
    float out;
    __asm__("min.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b));
    return out;
#else
    return a < b ? a : b;
#endif
}

extern "C" unsigned char* getBlockingVehicle__7VehiclePf(unsigned char* vehicle, float* out) {
    const AliasedFloat* const m = reinterpret_cast<AliasedFloat*>(vehicle + 1116);
    Point3 back = {-m[3], -m[4], -m[5]};
    Point3 side = {m[0], m[1], m[2]};
    const Point3* const position = reinterpret_cast<const Point3*>(vehicle + 16);
    const float radius = *reinterpret_cast<AliasedFloat*>(vehicle + 836);
    const float speed = *reinterpret_cast<AliasedFloat*>(vehicle + 840);
    for (int i = 0; i < vehicleCount; ++i) {
        if (i == *reinterpret_cast<AliasedInt*>(vehicle + 2156)) continue;
        unsigned char* const other = vehicleList[i];
        const Point3* const velocity = reinterpret_cast<const Point3*>(other + 1188);
        const float along = velocity->x * back.x + velocity->y * back.y + velocity->z * back.z;
        if (speed < along) continue;
        const float d1 = distanceToPlane__FP6Point3N20(position, &back, reinterpret_cast<const Point3*>(vehicleList[i] + 16));
        const float gap = d1 - *reinterpret_cast<AliasedFloat*>(vehicleList[i] + 836);
        if (!(0.0f < gap)) continue;
        const float limit = radius * (fp_min(1.0f, speed * 0x1.999998p-4f) * 0x1.e66668p+0f + 0x1.0cccccp+1f);
        if (!(gap < limit)) continue;
        const float d2 = distanceToPlane__FP6Point3N20(position, &side, reinterpret_cast<const Point3*>(vehicleList[i] + 16));
        const float width = (radius + *reinterpret_cast<AliasedFloat*>(vehicleList[i] + 836)) * 0.75f;
        if (!(__builtin_fabsf(d2) < width)) continue;
        if (out != 0) *out = d2;
        return vehicleList[i];
    }
    return 0;
}
