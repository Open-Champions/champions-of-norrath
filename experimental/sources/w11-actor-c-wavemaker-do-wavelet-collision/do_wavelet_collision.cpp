// Observed doWaveletCollision__9WaveMakerP6Point3i at 0x00152a68, 608 bytes. Arguments: the wave maker s, a point
// p and an index n. Bit (n & 31) of the word n >> 5 (arithmetic shift) of the array splashInfoBits (a local array of the
// WaveMaker translation unit, in the image's symbols) chooses the kind of splash:
//   set:   particle definition waveSplashBig, 56 particles, lift h = 9.0, speed v = 38.0;
//   clear: particle definition waveSplashSmall, 28 particles, lift h = 2.0, speed v = 15.0.
// Each particle takes five steps of the 32-bit generator at s+68 (state * 1664525 + 1013904223, draw = upper 16
// bits, u = draw * 2^-16, the state is stored back after the fifth) and calls
// P_AddParticle(definition, position, velocity, 0.0) with
//   position = (p.x + (u1 - 0.5) * 25.0, p.y + (u2 - 0.5) * 25.0, p.z + h),
//   velocity = ((u3 - 0.5) * 15.0, (u4 - 0.5) * 15.0, u5 * v).
// Inferred: this is the burst of droplets when a wavelet hits something; whether n selects a spot
// type is not established here. Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
extern unsigned splashInfoBits[] __asm__("splashInfoBits");
extern unsigned char waveSplashBig[];
extern unsigned char waveSplashSmall[];
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float arg);

void doWaveletCollision__9WaveMakerP6Point3i(void* self, const Point3* point, int index) {
    unsigned& state = *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 68);
    const bool big = (splashInfoBits[index >> 5] & (1u << (index & 31))) != 0u;
    unsigned char* const definition = big ? waveSplashBig : waveSplashSmall;
    const float speed = big ? 38.0f : 15.0f;
    const float lift = big ? 9.0f : 2.0f;
    const int count = big ? 56 : 28;
    const float unit = __builtin_bit_cast(float, 0x37800000u);
    const float half_one = 0.5f;
    for (int i = 0; i < count; ++i) {
        unsigned s = state;
        s = s * 1664525u + 1013904223u;
        const float u1 = static_cast<float>(static_cast<int>(s >> 16)) * unit;
        s = s * 1664525u + 1013904223u;
        const float u2 = static_cast<float>(static_cast<int>(s >> 16)) * unit;
        s = s * 1664525u + 1013904223u;
        const float u3 = static_cast<float>(static_cast<int>(s >> 16)) * unit;
        s = s * 1664525u + 1013904223u;
        const float u4 = static_cast<float>(static_cast<int>(s >> 16)) * unit;
        s = s * 1664525u + 1013904223u;
        const float u5 = static_cast<float>(static_cast<int>(s >> 16)) * unit;
        state = s;
        Point3 position;
        Point3 velocity;
        position.x = point->x + (u1 - half_one) * 25.0f;
        position.y = point->y + (u2 - half_one) * 25.0f;
        position.z = point->z + lift;
        velocity.x = (u3 - half_one) * 15.0f;
        velocity.y = (u4 - half_one) * 15.0f;
        velocity.z = u5 * speed;
        P_AddParticle__FP11ParticleDefP6Point3T1f(definition, &position, &velocity, 0.0f);
    }
}
}
