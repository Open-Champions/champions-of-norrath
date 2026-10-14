// Observed msg_run__6Candle at 0x2302f8, 792 bytes. One particle per call:
//  - a velocity: a random point in the unit ball (three components, each
//    2 * (eRand * 2^-16) - 1, redrawn while the squared length exceeds 1.0), then x and y
//    set to zero and z replaced by z * 0.3 (word 0x3e999999) + 3.5;
//  - a position: another random point in the unit ball, whose x and y are overwritten by
//    the object's x and y (the floats at +16 and +20), and whose z becomes
//    |z| + the object's z (+24) + 8.0 when the word at +144 is 1, otherwise
//    |z| + the object's z - 4.0;
//  - P_AddParticle(candleParticle, position, velocity, 0.0).
// Then the counter at +136 is incremented; when it reaches 5 it is reset to 0 and the
// floats at +104 and +112 are set to n and those at +108 and +116 to 1 / (n * n), where
// n = ((eRand - 8) & 15) + 84 if the word at +144 is 1, else + 48.
// Finally, if the signed byte at +141 is at least 31, the object is removed from the
// run list and the byte at +140 is cleared; the byte at +141 is then incremented.
// The random draws happen in the order x, y, z for the velocity and again for the
// position, including the ones whose x and y are overwritten.
// Unknown: what the fields at +104..+116, +136, +144 and the two bytes mean.

extern "C" {
struct Point3 { float x, y, z; };
int eRand__10GameObject(void* object);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age);
void objectRemoveFromRunList__FP10GameObject(void* object);
extern unsigned char candleParticle[80];
}

typedef int Word __attribute__((may_alias));
typedef float Single __attribute__((may_alias));

static inline float single_from_bits(unsigned bits) { return __builtin_bit_cast(float, bits); }

static inline float random_unit(void* object) {
    float t = static_cast<float>(eRand__10GameObject(object)) * single_from_bits(0x37800000u);
    return (t + t) - 1.0f;
}

static inline void random_in_ball(void* object, Point3& p) {
    do {
        p.x = random_unit(object);
        p.y = random_unit(object);
        p.z = random_unit(object);
    } while (1.0f < p.x * p.x + p.y * p.y + p.z * p.z);
}

extern "C" void msg_run__6Candle(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    Point3 velocity;
    Point3 position;
    int remaining = 0;
    do {
        random_in_ball(self, velocity);
        velocity.z = velocity.z * single_from_bits(0x3e999999u) + 3.5f;
        velocity.x = 0.0f;
        velocity.y = 0.0f;
        random_in_ball(self, position);
        if (position.z < 0.0f) position.z = -position.z;
        position.x = *reinterpret_cast<Single*>(object + 16);
        position.y = *reinterpret_cast<Single*>(object + 20);
        if (*reinterpret_cast<Word*>(object + 144) == 1)
            position.z = position.z + *reinterpret_cast<Single*>(object + 24) + 8.0f;
        else
            position.z = position.z + *reinterpret_cast<Single*>(object + 24) - 4.0f;
        P_AddParticle__FP11ParticleDefP6Point3T1f(candleParticle, &position, &velocity, 0.0f);
    } while (--remaining >= 0);
    int count = *reinterpret_cast<Word*>(object + 136) + 1;
    *reinterpret_cast<Word*>(object + 136) = count;
    if (count == 5) {
        *reinterpret_cast<Word*>(object + 136) = 0;
        int base = (*reinterpret_cast<Word*>(object + 144) == 1) ? 84 : 48;
        int n = ((eRand__10GameObject(self) - 8) & 15) + base;
        float f = static_cast<float>(n);
        *reinterpret_cast<Single*>(object + 104) = f;
        *reinterpret_cast<Single*>(object + 112) = f;
        float inverse = 1.0f / (f * f);
        *reinterpret_cast<Single*>(object + 116) = inverse;
        *reinterpret_cast<Single*>(object + 108) = inverse;
    }
    if (static_cast<signed char>(object[141]) >= 31) {
        objectRemoveFromRunList__FP10GameObject(self);
        object[140] = 0;
    }
    object[141] = static_cast<unsigned char>(object[141] + 1);
}
