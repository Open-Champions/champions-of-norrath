// Observed msg_run__7Candle2 at 0x230610, 632 bytes. One particle per call:
//  - a velocity: a random point in the unit ball (three components, each
//    2 * (eRand * 2^-16) - 1, redrawn while the squared length exceeds 1.0), then x and y
//    set to zero and z replaced by z * 0.3 (word 0x3e999999) + 3.5;
//  - a position: another random point in the unit ball, doubled componentwise with z made
//    non-negative, whose x and y are then overwritten by the object's x and y (the
//    floats at +16 and +20) and whose z becomes z + the object's z (+24) + 5.0;
//  - P_AddParticle(candle2Particle, position, velocity, 0.0).
// Finally, if the signed byte at +77 is at least 31, the object is removed from the run
// list and the byte at +76 is cleared; the byte at +77 is then incremented.
// The random draws happen in the order x, y, z for the velocity and again for the
// position, including the doubled x and y that are overwritten.
// Unknown: what the two bytes mean.

extern "C" {
struct Point3 { float x, y, z; };
int eRand__10GameObject(void* object);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age);
void objectRemoveFromRunList__FP10GameObject(void* object);
extern unsigned char candle2Particle[80];
}

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

extern "C" void msg_run__7Candle2(void* self) {
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
        position.x = position.x + position.x;
        position.y = position.y + position.y;
        position.z = position.z + position.z;
        if (position.z < 0.0f) position.z = -position.z;
        position.x = *reinterpret_cast<Single*>(object + 16);
        position.y = *reinterpret_cast<Single*>(object + 20);
        position.z = position.z + *reinterpret_cast<Single*>(object + 24) + 5.0f;
        P_AddParticle__FP11ParticleDefP6Point3T1f(candle2Particle, &position, &velocity, 0.0f);
    } while (--remaining >= 0);
    if (static_cast<signed char>(object[77]) >= 31) {
        objectRemoveFromRunList__FP10GameObject(self);
        object[76] = 0;
    }
    object[77] = static_cast<unsigned char>(object[77] + 1);
}
