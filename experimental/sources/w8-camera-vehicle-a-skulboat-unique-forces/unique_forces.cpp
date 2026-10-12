// Observed doMyUniqueForces__8SkulBoat at 0x002bf788, 660 bytes.
// Returns 1 on every path. Nothing here was executed on the EE. What the
// vehicle fields are is not established here, so the offsets stay numeric.
//
// 1. A local 3x3 matrix m copies the 36 bytes at vehicle+1116 and is rotated
//    on axis 2 by matRot__FP8Matrix33is with the short of cvt.w.s of
//    (-float(vehicle+800)) * A * B, where A has bits 0x3bb60b60 and B is
//    32767.0, multiplied in that order.
// 2. With k = float(vehicle+740) * 2671.0, the force is
//    (-m[3]*k, -m[4]*k, -m[5]*k) of the rotated copy, each as (-m[i]) * k.
//    accumForceAndTorque__7VehicleP6Point3T1i receives the vehicle, a point
//    (0.0, 191.0, 0.0), that force, and 0, in that order.
// 3. When the squared length of the three floats at vehicle+1188 is above
//    zero (the observed code takes sqrt.s of the sum and tests it above zero;
//    for a sum of squares that is the same test), three eRand__10GameObject
//    results are drawn per attempt, each turned into r = 2 * (int * 2^-16) - 1
//    in float. The attempt is repeated while r1*r1 + r2*r2 + r3*r3 is above 1.
//    The point (position.x + 80*r1, position.y + 80*r2, position.z - 30),
//    with position the three floats at vehicle+16, is passed with the word
//    `world` (0x004844a0) and the floats 400.0 and 48.0 to
//    worldPerturbWater__FP12_worldHeaderG6Point3ff. The point is the address
//    of a caller-owned copy, as for other Point3 arguments.
extern "C" {
void matRot__FP8Matrix33is(float* matrix, int axis, short angle);
void accumForceAndTorque__7VehicleP6Point3T1i(void* vehicle, const float* point, const float* force, int flag);
int eRand__10GameObject(void* object);
void worldPerturbWater__FP12_worldHeaderG6Point3ff(void* world, const float* point, float a, float b);
extern void* world;
}

typedef float __attribute__((may_alias)) AliasedFloat;

namespace {
inline int cvt_w_s(float value) {
#if defined(__mips__)
    int out;
    float scratch;
    __asm__("cvt.w.s %1, %2\n\tmfc1 %0, %1" : "=r"(out), "=&f"(scratch) : "f"(value));
    return out;
#else
    return static_cast<int>(__builtin_rintf(value));
#endif
}
inline float float_from_bits(unsigned bits) {
    union {
        unsigned bits;
        float number;
    } converted;
    converted.bits = bits;
    return converted.number;
}
inline float unit_random(unsigned char* boat) {
    const float scaled = static_cast<float>(eRand__10GameObject(boat)) * float_from_bits(0x37800000u);
    return scaled + scaled - 1.0f;
}
}

extern "C" int doMyUniqueForces__8SkulBoat(unsigned char* boat) {
    float matrix[9];
    const float where[3] = {0.0f, 191.0f, 0.0f};
    for (int i = 0; i < 9; ++i) matrix[i] = *reinterpret_cast<AliasedFloat*>(boat + 1116 + 4 * i);
    const float angle = -*reinterpret_cast<AliasedFloat*>(boat + 800) * float_from_bits(0x3bb60b60u) * 32767.0f;
    matRot__FP8Matrix33is(matrix, 2, static_cast<short>(cvt_w_s(angle)));
    const float k = *reinterpret_cast<AliasedFloat*>(boat + 740) * 2671.0f;
    const float force[3] = {-matrix[3] * k, -matrix[4] * k, -matrix[5] * k};
    accumForceAndTorque__7VehicleP6Point3T1i(boat, where, force, 0);

    const float vx = *reinterpret_cast<AliasedFloat*>(boat + 1188);
    const float vy = *reinterpret_cast<AliasedFloat*>(boat + 1192);
    const float vz = *reinterpret_cast<AliasedFloat*>(boat + 1196);
    if (vx * vx + vy * vy + vz * vz > 0.0f) {
        float r1, r2, r3;
        do {
            r1 = unit_random(boat);
            r2 = unit_random(boat);
            r3 = unit_random(boat);
        } while (r1 * r1 + r2 * r2 + r3 * r3 > 1.0f);
        const float* position = reinterpret_cast<AliasedFloat*>(boat + 16);
        const float spot[3] = {position[0] + r1 * 80.0f, position[1] + r2 * 80.0f, position[2] + -30.0f};
        worldPerturbWater__FP12_worldHeaderG6Point3ff(world, spot, 400.0f, 48.0f);
    }
    return 1;
}
