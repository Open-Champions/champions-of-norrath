// Observed doMisc__7Vehicle at 0x00273ad8, 1288 bytes. Nothing is returned.
// Nothing here was executed on the EE. Field meanings are not established
// here, so offsets stay numeric. accumForceAndTorque means
// accumForceAndTorque__7VehicleP6Point3T1i(vehicle, point, force, 0). Parts:
//   1. When bit 0 of the word at +1044 is clear: force (0, 0, float(+668) *
//      -10.0) at the global Point3 nullp.
//   2. When the word at +2144 is zero, for each of the n = signed word at
//      +1000 entries (re-read after each): k = (wheel->word392 != 0 ?
//      float(+880) : float(+876)) * C * float(+668), with wheel the pointer at
//      +1052+4i and C = float 0x3cd20d20; force (Point3 at +1836+12i) * k at
//      the point at +1536+12i.
//   3. For each seat i below the signed word at +976: when the word at
//      +984+4i is non-zero, the seat position p (getSeatPosition) is
//      un-rotated by the matrix at +1116 (matUntransform) and force (0, 0,
//      float(-word) * D) with D = float 0x44c6dd17 is applied at that
//      result; the word is then cleared.
//   4. With the word +2168 (m) non-zero, unless the driver word +956 is -1 and
//      the word +2148 is zero: with x = float(+740): if x == 1.0 a burst is
//      armed with s = rand * 2^-16 * 50.0 + 10.0 (one eRand draw); else if x
//      == 0.0 and frameNm % 7 == 0 it is armed with s = rand * 2^-16 * 25.0 +
//      5.0. When armed, only exhaust i == frameNm % m (signed remainder, m
//      read afresh each pass) emits: pos = matTransform(M, point at
//      +2172+24i) + P (P = Point3 at +16), vel = matTransform(M, point at
//      +2184+24i) * s plus r * 4.0, r drawn as 3 values 2*(draw*2^-16)-1
//      repeated while r.r > 1.0, then vel += getParticleVelocity(vehicle, pos);
//      P_AddParticle(lavaSteamParticle, pos, vel, 0.0f) is called.
// Vector values passed by value in the original are passed as the address of a
// caller-owned copy.
struct __attribute__((may_alias)) Point3 { float x, y, z; };

extern "C" {
void accumForceAndTorque__7VehicleP6Point3T1i(unsigned char* vehicle, const Point3* point, const Point3* force, int flag);
void getSeatPosition__7VehicleiP6Point3(unsigned char* vehicle, int seat, Point3* out);
void matUntransform__FP6Point3P8Matrix33T0(Point3* out, const void* matrix, const Point3* in);
void matTransform__FP6Point3P8Matrix33T0(Point3* out, const void* matrix, const Point3* in);
int eRand__10GameObject(void* object);
void getParticleVelocity__7VehicleP6Point3(Point3* result, unsigned char* vehicle, const Point3* point);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, const Point3* position, const Point3* velocity, float value);
extern Point3 nullp;
extern unsigned char lavaSteamParticle[];
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
static inline float draw_unit(unsigned char* vehicle) {
    const float scaled = static_cast<float>(eRand__10GameObject(vehicle)) * 0x1p-16f;
    return scaled + scaled - 1.0f;
}

extern "C" void doMisc__7Vehicle(unsigned char* vehicle) {
    Point3 force;
    if ((word(vehicle, 1044) & 1) == 0) {
        force.x = 0.0f;
        force.y = 0.0f;
        force.z = real(vehicle, 668) * -10.0f;
        accumForceAndTorque__7VehicleP6Point3T1i(vehicle, &nullp, &force, 0);
    }
    if (word(vehicle, 2144) == 0) {
        for (int i = 0; i < word(vehicle, 1000); ++i) {
            unsigned char* const wheel = *reinterpret_cast<AliasedPointer*>(vehicle + 1052 + 4 * i);
            const float pick = word(wheel, 392) != 0 ? real(vehicle, 880) : real(vehicle, 876);
            const float k = pick * 0x1.a41a40p-6f * real(vehicle, 668);
            const Point3* const velocity = reinterpret_cast<Point3*>(vehicle + 1836 + 12 * i);
            force.z = velocity->z * k;
            force.x = velocity->x * k;
            force.y = velocity->y * k;
            accumForceAndTorque__7VehicleP6Point3T1i(vehicle, reinterpret_cast<Point3*>(vehicle + 1536 + 12 * i), &force, 0);
        }
    }
    for (int i = 0; i < word(vehicle, 976); ++i) {
        int* const flag = reinterpret_cast<AliasedInt*>(vehicle + 984 + 4 * i);
        if (*flag == 0) continue;
        Point3 seat;
        Point3 local;
        getSeatPosition__7VehicleiP6Point3(vehicle, i, &seat);
        matUntransform__FP6Point3P8Matrix33T0(&local, vehicle + 1116, &seat);
        const float load = static_cast<float>(-*flag);
        force.x = 0.0f;
        force.y = 0.0f;
        force.z = load * 0x1.8dba2ep+10f;
        accumForceAndTorque__7VehicleP6Point3T1i(vehicle, &local, &force, 0);
        *flag = 0;
    }

    int count = word(vehicle, 2168);
    if (count == 0) return;
    if (word(vehicle, 956) == -1 && word(vehicle, 2148) == 0) return;
    const float x = real(vehicle, 740);
    float strength = 0.0f;
    int armed = 0;
    if (x == 1.0f) {
        armed = 1;
        const float scaled = static_cast<float>(eRand__10GameObject(vehicle)) * 0x1p-16f;
        count = word(vehicle, 2168);
        strength = scaled * 50.0f + 10.0f;
    } else if (x == 0.0f && frameNm % 7 == 0) {
        armed = 1;
        const float scaled = static_cast<float>(eRand__10GameObject(vehicle)) * 0x1p-16f;
        count = word(vehicle, 2168);
        strength = scaled * 25.0f + 5.0f;
    }
    for (int i = 0; i < count; ++i) {
        if (armed != 0 && frameNm % count == i) {
            Point3 position;
            Point3 velocity;
            Point3 direction;
            Point3 base;
            Point3 reported;
            matTransform__FP6Point3P8Matrix33T0(&position, vehicle + 1116, reinterpret_cast<Point3*>(vehicle + 2172 + 24 * i));
            const Point3* const origin = reinterpret_cast<Point3*>(vehicle + 16);
            position.x = position.x + origin->x;
            position.y = position.y + origin->y;
            position.z = position.z + origin->z;
            matTransform__FP6Point3P8Matrix33T0(&velocity, vehicle + 1116, reinterpret_cast<Point3*>(vehicle + 2184 + 24 * i));
            velocity.x = velocity.x * strength;
            velocity.y = velocity.y * strength;
            velocity.z = velocity.z * strength;
            do {
                direction.x = draw_unit(vehicle);
                direction.y = draw_unit(vehicle);
                direction.z = draw_unit(vehicle);
            } while (1.0f < direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
            direction.x = direction.x * 4.0f;
            direction.y = direction.y * 4.0f;
            direction.z = direction.z * 4.0f;
            velocity.x = velocity.x + direction.x;
            velocity.y = velocity.y + direction.y;
            velocity.z = velocity.z + direction.z;
            getParticleVelocity__7VehicleP6Point3(&base, vehicle, &position);
            reported = base;
            velocity.x = velocity.x + reported.x;
            velocity.y = velocity.y + reported.y;
            velocity.z = velocity.z + reported.z;
            P_AddParticle__FP11ParticleDefP6Point3T1f(lavaSteamParticle, &position, &velocity, 0.0f);
            count = word(vehicle, 2168);
        }
    }
}
