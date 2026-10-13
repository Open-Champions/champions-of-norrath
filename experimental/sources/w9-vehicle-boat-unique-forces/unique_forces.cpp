// Observed doMyUniqueForces__4Boat at 0x002bf3d8, 616 bytes. Returns 1 on every
// path. Nothing here was executed on the EE. What the vehicle fields are is not
// established here, so the offsets stay numeric. Compare the SkulBoat version.
//
// 1. A local 3x3 matrix m copies the 36 bytes at vehicle+1116 and is rotated
//    on axis 2 by matRot__FP8Matrix33is with the short of cvt.w.s of
//    float(vehicle+800) * A * B, where A has bits 0x3bb60b60 and B is 32767.0,
//    multiplied in that order (no negation here).
// 2. With k = float(vehicle+740) * 1216.0, the force is
//    ((-m[3]) * k, (-m[4]) * k, (-m[5]) * k) of the rotated copy.
//    accumForceAndTorque__7VehicleP6Point3T1i receives the vehicle, a point
//    (0.0, 191.0, 0.0), that force, and 0, in that order.
// 3. If float(vehicle+740) equals 0.0, or the word frameNm (0x004844a4) has
//    either of its low two bits set, nothing more happens. Otherwise three
//    eRand__10GameObject results are drawn per attempt, each turned into
//    r = 2 * (int * 2^-16) - 1 in float, and the attempt repeats while
//    r1*r1 + r2*r2 + r3*r3 is above 1.0. The point
//    (position.x + 60*r1, position.y + 60*r2, position.z), with position the
//    three floats at vehicle+16, is passed with the word `world` (0x004844a0)
//    and the floats 400.0 and 24.0 to worldPerturbWater__FP12_worldHeaderG6Point3ff.
//    The point is the address of a caller-owned copy, as for other Point3
//    arguments. The third random value only takes part in the rejection test.
extern "C" {
void matRot__FP8Matrix33is(float* matrix, int axis, short angle);
void accumForceAndTorque__7VehicleP6Point3T1i(void* vehicle, const float* point, const float* force, int flag);
int eRand__10GameObject(void* object);
void worldPerturbWater__FP12_worldHeaderG6Point3ff(void* world, const float* point, float a, float b);
extern void* world;
extern int frameNm;
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

extern "C" int doMyUniqueForces__4Boat(unsigned char* boat) {
    float matrix[9];
    const float where[3] = {0.0f, 191.0f, 0.0f};
    for (int i = 0; i < 9; ++i) matrix[i] = *reinterpret_cast<AliasedFloat*>(boat + 1116 + 4 * i);
    const float angle = *reinterpret_cast<AliasedFloat*>(boat + 800) * float_from_bits(0x3bb60b60u) * 32767.0f;
    matRot__FP8Matrix33is(matrix, 2, static_cast<short>(cvt_w_s(angle)));
    const float k = *reinterpret_cast<AliasedFloat*>(boat + 740) * 1216.0f;
    const float force[3] = {-matrix[3] * k, -matrix[4] * k, -matrix[5] * k};
    accumForceAndTorque__7VehicleP6Point3T1i(boat, where, force, 0);

    if (*reinterpret_cast<AliasedFloat*>(boat + 740) != 0.0f && (frameNm & 3) == 0) {
        float r1, r2, r3;
        do {
            r1 = unit_random(boat);
            r2 = unit_random(boat);
            r3 = unit_random(boat);
        } while (r1 * r1 + r2 * r2 + r3 * r3 > 1.0f);
        const float* position = reinterpret_cast<AliasedFloat*>(boat + 16);
        const float spot[3] = {position[0] + r1 * 60.0f, position[1] + r2 * 60.0f, position[2]};
        worldPerturbWater__FP12_worldHeaderG6Point3ff(world, spot, 400.0f, 24.0f);
    }
    return 1;
}
