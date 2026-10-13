// Observed doDriveline__7Vehicle at 0x00275298, 3712 bytes. Nothing is
// returned. Nothing here was executed on the EE. Field meanings are not
// established here, so offsets stay numeric. n is the signed word at +1000;
// for wheel i: A = float(+676+4i) (a contact amount, zero means no contact),
// q = pointer at +2056+4i, w = pointer at +1052+4i, N = Point3 at +1740+12i,
// R = Point3 at +1644+12i, vel = Point3 at +1836+12i, H = Point3 at
// +1440+12i, point_i = Point3 at +1536+12i, E = float at +768+4i. spec is the
// pointer at +2052. M is the matrix floats at +1116 (so M[3..5] are at +1128 and
// M[6..8] at +1140). K_a..K_e below are the floats 0x3dec4ec4, 0x3ecccccc,
// 0x3c888888, 0x3ba3d70a, 0x3c888888. D2 is the double 0x404ca5bc00000000.
// Double arithmetic goes through the original's soft routines fptodp, dpcmp,
// dpsub, dpmul, dptofp, acos and sqrt, whose doubles travel in one 64-bit register.
//   1. For each wheel with A != 0: E = A * q[1] * C1 with C1 = spec float +56 *
//      K_a * float(+868); u = floats at w+400..w+408; the angle g = 90 -
//      dptofp(acos(|fptodp(N.u)|) * D2) is limited to [float(+948) * f, float(+944) *
//      f] with f = K_b / spec float +56 (below-lower becomes 0.0, above-upper
//      becomes the upper); E *= 1 - min(1, dptofp(sqrt(fptodp(g / upper)))); R is
//      the unit vector of (N x a) x N with a = -(w floats at +412..+420), each
//      cross product and its normalisation with rsqrt.s. A wheel with A == 0 gets
//      E = 0 and R = 0.
//   2. For driven wheels in contact (w word +388 non-zero, A != 0) the sum s of
//      w float +360 * (spec[0] / float(+912)) and their count c are formed (the
//      sum is negated when word +2152 is non-zero). With c == 0 the average is
//      float(+824) itself, otherwise s / c with the old +824 as the base; it is
//      limited to [0, spec float +4], float(+748) moves by the change (clamped to
//      [0, spec float +4]) and float(+824) takes the average. Then
//      doDifferential__7Vehicle and getEngineTorque__7Vehicle run.
//   3. For each wheel with A != 0 the contact force F (a Point3 built from the
//      side velocity and R; plus a driven-wheel term when w word +388 is set)
//      is limited with scalePatchLimit__7VehicleiP6Point3 and E, F is rescaled
//      to length E when longer, dust particles (slideDustParticle family) or, for
//      driven wheels while float(+740) > 0.25 on wheels 1 and 2, three
//      lavaGoopParticle sprays are emitted; the driven-wheel work is summed in two
//      totals; accumForceAndTorque__7VehicleP6Point3T1i is called with flag 0.
//   4. float(+748) is moved by ((total1 - total2) / (float(+756) * float(+820)) *
//      150.0) * K_e (subtracted when word +2152 is set) and limited by the two
//      sums and by [0, spec float +4].
// The exact expressions are in the code below; they follow the original's
// operation order. Point3 values passed by value in the original are passed as
// the address of a caller-owned copy.
struct __attribute__((may_alias)) Point3 { float x, y, z; };

typedef unsigned long long Double;

extern "C" {
Double fptodp(float value);
int dpcmp(Double a, Double b);
Double dpsub(Double a, Double b);
Double dpmul(Double a, Double b);
float dptofp(Double a);
Double driveline_acos(Double a) __asm__("acos");
Double driveline_sqrt(Double a) __asm__("sqrt");
void doDifferential__7Vehicle(unsigned char* vehicle);
float getEngineTorque__7Vehicle(unsigned char* vehicle);
float scalePatchLimit__7VehicleiP6Point3(unsigned char* vehicle, int index, Point3* force);
void accumForceAndTorque__7VehicleP6Point3T1i(unsigned char* vehicle, const Point3* point, const Point3* force, int flag);
int eRand__10GameObject(void* object);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, const Point3* position, const Point3* velocity, float value);
extern unsigned char slideDustParticle[];
extern unsigned char slideDustParticleDark[];
extern unsigned char slideDustParticleBig[];
extern unsigned char slideDustParticleBigDark[];
extern unsigned char lavaGoopParticle[];
}

typedef float AliasedFloat __attribute__((may_alias));
typedef int AliasedInt __attribute__((may_alias));
typedef unsigned char* AliasedPointer __attribute__((may_alias));

namespace {
inline int& word(unsigned char* base, int offset) { return *reinterpret_cast<AliasedInt*>(base + offset); }
inline float& real(unsigned char* base, int offset) { return *reinterpret_cast<AliasedFloat*>(base + offset); }
inline Point3* point(unsigned char* base, int offset) { return reinterpret_cast<Point3*>(base + offset); }
inline unsigned char* pointer(unsigned char* base, int offset) { return *reinterpret_cast<AliasedPointer*>(base + offset); }
inline float rsqrt_s(float numerator, float value) {
#if defined(__mips__)
    float out;
    __asm__("rsqrt.s %0, %1, %2" : "=f"(out) : "f"(numerator), "f"(value));
    return out;
#else
    return numerator / __builtin_sqrtf(value);
#endif
}
inline float draw(unsigned char* vehicle) {
    return static_cast<float>(eRand__10GameObject(vehicle)) * 0x1p-16f;
}
}

extern "C" void doDriveline__7Vehicle(unsigned char* vehicle) {
    unsigned char* spec = pointer(vehicle, 2052);
    const float c1 = real(spec, 56) * 0x1.d89d88p-4f * real(vehicle, 868);

    for (int i = 0; i < word(vehicle, 1000); ++i) {
        const float a = real(vehicle, 676 + 4 * i);
        if (a == 0.0f) {
            real(vehicle, 768 + 4 * i) = 0.0f;
            real(vehicle, 1652 + 12 * i) = 0.0f;
            real(vehicle, 1644 + 12 * i) = 0.0f;
            real(vehicle, 1648 + 12 * i) = 0.0f;
            continue;
        }
        unsigned char* const q = pointer(vehicle, 2056 + 4 * i);
        unsigned char* const w = pointer(vehicle, 1052 + 4 * i);
        const Point3* const n = point(vehicle, 1740 + 12 * i);
        real(vehicle, 768 + 4 * i) = a * real(q, 4) * c1;
        Point3 u;
        u.x = real(w, 400);
        u.y = real(w, 404);
        u.z = real(w, 408);
        Double d = fptodp(n->x * u.x + n->y * u.y + n->z * u.z);
        if (dpcmp(d, 0ull) < 0) d = dpsub(0ull, d);
        const float angle = dptofp(dpmul(driveline_acos(d), 0x404ca5bc00000000ull));
        spec = pointer(vehicle, 2052);
        float g = 90.0f - angle;
        const float f = 0x1.999998p-2f / real(spec, 56);
        if (g < real(vehicle, 948) * f) g = 0.0f;
        const float upper = real(vehicle, 944) * f;
        if (upper < g) g = upper;
        const float s = dptofp(driveline_sqrt(fptodp(g / upper)));
        float keep = 1.0f - s;
        if (1.0f < keep) keep = 1.0f;
        real(vehicle, 768 + 4 * i) = real(vehicle, 768 + 4 * i) * keep;

        Point3 axis;
        Point3 tangent;
        Point3 cross;
        axis.x = -real(w, 412);
        axis.y = -real(w, 416);
        axis.z = -real(w, 420);
        tangent.x = n->y * axis.z - n->z * axis.y;
        tangent.y = n->z * axis.x - n->x * axis.z;
        tangent.z = n->x * axis.y - n->y * axis.x;
        const float t_inverse = rsqrt_s(1.0f, tangent.x * tangent.x + tangent.y * tangent.y + tangent.z * tangent.z);
        tangent.x = tangent.x * t_inverse;
        tangent.y = tangent.y * t_inverse;
        tangent.z = tangent.z * t_inverse;
        cross.x = tangent.y * n->z - tangent.z * n->y;
        cross.y = tangent.z * n->x - tangent.x * n->z;
        cross.z = tangent.x * n->y - tangent.y * n->x;
        Point3* const r = point(vehicle, 1644 + 12 * i);
        *r = cross;
        const float r_inverse = rsqrt_s(1.0f, r->x * r->x + r->y * r->y + r->z * r->z);
        r->z = r->z * r_inverse;
        r->x = r->x * r_inverse;
        r->y = r->y * r_inverse;
    }

    spec = pointer(vehicle, 2052);
    float average = 0.0f;
    float count = 0.0f;
    if (word(vehicle, 1000) > 0) {
        const int total = word(vehicle, 1000);
        for (int i = 0; i < total; ++i) {
            unsigned char* const w = pointer(vehicle, 1052 + 4 * i);
            if (word(w, 388) == 0) continue;
            if (real(vehicle, 676 + 4 * i) == 0.0f) continue;
            count = count + 1.0f;
            average = average + real(w, 360) * (real(spec, 0) / real(vehicle, 912));
        }
    }
    if (word(vehicle, 2152) != 0) average = -average;
    float before;
    if (count == 0.0f) {
        average = real(vehicle, 824);
        before = average;
    } else {
        average = average / count;
        before = real(vehicle, 824);
    }
    if (average < 0.0f) average = 0.0f;
    if (real(spec, 4) < average) average = real(spec, 4);
    const float change = average - before;
    real(vehicle, 748) = real(vehicle, 748) + change;
    if (real(vehicle, 748) < 0.0f) real(vehicle, 748) = 0.0f;
    if (real(spec, 4) < real(vehicle, 748)) real(vehicle, 748) = real(spec, 4);
    real(vehicle, 824) = average;

    doDifferential__7Vehicle(vehicle);
    float sum_drive = 0.0f;
    float sum_along = 0.0f;
    const float torque = getEngineTorque__7Vehicle(vehicle);
    const float drive = real(vehicle, 908) * torque;
    for (int i = 0; i < word(vehicle, 1000); ++i) {
        if (real(vehicle, 676 + 4 * i) == 0.0f) continue;
        Point3 force;
        Point3 side;
        Point3 extra;
        const float* const vel = reinterpret_cast<float*>(vehicle + 1836 + 12 * i);
        const Point3* const n = point(vehicle, 1740 + 12 * i);
        const Point3* const r = point(vehicle, 1644 + 12 * i);
        unsigned char* const w = pointer(vehicle, 1052 + 4 * i);
        const float dot = n->x * vel[0] + n->y * vel[1] + n->z * vel[2];
        side.x = vel[0] - n->x * dot;
        side.y = vel[1] - n->y * dot;
        side.z = vel[2] - n->z * dot;
        const float brake = -(real(w, 364) * real(w, 380)) * 0x1.111110p-6f;
        extra.x = r->x * brake;
        extra.y = r->y * brake;
        extra.z = r->z * brake;
        const float cx = side.x + extra.x;
        const float cy = side.y + extra.y;
        const float cz = side.z + extra.z;
        const float e0 = real(vehicle, 768 + 4 * i);
        force.x = -cx * e0;
        force.y = -cy * e0;
        force.z = -cz * e0;
        if (word(w, 388) != 0) {
            float k = drive / real(vehicle, 820);
            if (word(vehicle, 2152) != 0) k = -k;
            spec = pointer(vehicle, 2052);
            sum_drive = sum_drive + k;
            const float m = (k * (real(spec, 0) / real(vehicle, 912))) * (1.0f / real(w, 376));
            force.x = force.x + r->x * m;
            force.y = force.y + r->y * m;
            force.z = force.z + r->z * m;
        }
        float limited = real(vehicle, 768 + 4 * i) * scalePatchLimit__7VehicleiP6Point3(vehicle, i, &force);
        real(vehicle, 768 + 4 * i) = limited;
        if (word(pointer(vehicle, 1052 + 4 * i), 392) != 0) {
            limited = limited * real(vehicle, 872);
            real(vehicle, 768 + 4 * i) = limited;
        }
        const float length = __builtin_sqrtf(force.x * force.x + force.y * force.y + force.z * force.z);
        if (real(vehicle, 768 + 4 * i) < length) {
            const float e = real(vehicle, 768 + 4 * i);
            Point3 direction;
            Point3 spot;
            const float inverse_e = 1.0f / e;
            direction.x = force.x * inverse_e;
            direction.y = force.y * inverse_e;
            direction.z = force.z * inverse_e;
            spot = *point(vehicle, 1440 + 12 * i);
            const float f_inverse = rsqrt_s(1.0f, force.x * force.x + force.y * force.y + force.z * force.z);
            force.x = force.x * f_inverse;
            force.y = force.y * f_inverse;
            force.z = force.z * f_inverse;
            force.x = force.x * e;
            force.y = force.y * e;
            force.z = force.z * e;
            if (word(w, 392) == 0) {
                direction.x = direction.x * -0.25f;
                direction.y = direction.y * -0.25f;
                direction.z = direction.z * -0.25f;
                spot.x = spot.x + static_cast<float>(static_cast<int>(static_cast<unsigned>(eRand__10GameObject(vehicle)) % 12u) - 6);
                spot.y = spot.y + static_cast<float>(static_cast<int>(static_cast<unsigned>(eRand__10GameObject(vehicle)) % 12u) - 6);
                spot.z = spot.z + static_cast<float>(static_cast<int>(static_cast<unsigned>(eRand__10GameObject(vehicle)) % 12u) - 6);
                void* definition;
                if (e * 8.0f < length) {
                    definition = (eRand__10GameObject(vehicle) & 1) != 0 ? slideDustParticleBigDark : slideDustParticleBig;
                } else {
                    definition = (eRand__10GameObject(vehicle) & 1) != 0 ? slideDustParticleDark : slideDustParticle;
                }
                P_AddParticle__FP11ParticleDefP6Point3T1f(definition, &spot, &direction, 0.0f);
            }
        }
        if (word(pointer(vehicle, 1052 + 4 * i), 392) != 0 && 0.25f < real(vehicle, 740) && static_cast<unsigned>(i - 1) < 2u) {
            float push = real(vehicle, 748) * 0x1.47ae14p-8f;
            if (word(vehicle, 2152) != 0) push = -push;
            for (int burst = 0; burst < 3; ++burst) {
                Point3 velocity;
                Point3 position;
                unsigned char* const wheel = pointer(vehicle, 1052 + 4 * i);
                velocity.x = real(wheel, 412) * push;
                velocity.z = real(wheel, 420) * push;
                velocity.y = real(wheel, 416) * push;
                float spread = draw(vehicle);
                spread = spread + spread - 1.0f;
                velocity.x = velocity.x + spread * 10.0f;
                spread = draw(vehicle);
                spread = spread + spread - 1.0f;
                velocity.y = velocity.y + spread * 10.0f;
                velocity.z = draw(vehicle) * 20.0f + 10.0f;
                const float swing = draw(vehicle) - 0.5f;
                const float lift = draw(vehicle);
                const float along = (swing + swing) * 12.0f;
                const float up = lift * 12.0f;
                position = *point(vehicle, 1440 + 12 * i);
                const float m3 = real(vehicle, 1128), m4 = real(vehicle, 1132), m5 = real(vehicle, 1136);
                const float m6 = real(vehicle, 1140), m7 = real(vehicle, 1144), m8 = real(vehicle, 1148);
                Point3 first;
                Point3 second;
                first.x = m3 * along;
                first.z = m5 * along;
                first.y = m4 * along;
                second.x = m6 * up;
                second.y = m7 * up;
                second.z = m8 * up;
                position.x = position.x + first.x;
                position.y = position.y + first.y;
                position.z = position.z + first.z;
                position.x = position.x + second.x;
                position.y = position.y + second.y;
                position.z = position.z + second.z;
                P_AddParticle__FP11ParticleDefP6Point3T1f(lavaGoopParticle, &position, &velocity, 0.0f);
            }
        }
        unsigned char* const wheel = pointer(vehicle, 1052 + 4 * i);
        if (word(wheel, 388) != 0) {
            spec = pointer(vehicle, 2052);
            const float along = ((force.x * r->x + force.y * r->y) + force.z * r->z) / (real(spec, 0) / real(vehicle, 912)) / (1.0f / real(wheel, 376));
            sum_along = sum_along + along;
        }
        accumForceAndTorque__7VehicleP6Point3T1i(vehicle, point(vehicle, 1536 + 12 * i), &force, 0);
    }

    const float previous = real(vehicle, 748);
    const float scale = ((sum_drive - sum_along) / (real(vehicle, 756) * real(vehicle, 820))) * 150.0f;
    const float amount = scale * 0x1.111110p-6f;
    real(vehicle, 748) = word(vehicle, 2152) == 0 ? previous + amount : previous - amount;
    float current = real(vehicle, 748);
    if (sum_along != 0.0f) {
        if (1.0f < amount && previous < average && average < current) {
            real(vehicle, 748) = average;
            current = average;
        }
        if (amount < -1.0f && current < average) {
            real(vehicle, 748) = average;
            current = average;
        }
    }
    if (current < 0.0f) {
        real(vehicle, 748) = 0.0f;
        current = 0.0f;
    }
    spec = pointer(vehicle, 2052);
    if (real(spec, 4) < current) real(vehicle, 748) = real(spec, 4);
}
