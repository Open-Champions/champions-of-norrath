// Observed doWaveletReflection__9WaveMakerP6Point3 at 0x00152938, 300 bytes. Arguments: the wave maker s and
// a point p. The word at s+68 is a 32-bit linear-congruential generator state (state * 1664525 + 1013904223,
// the same step eRand-style code elsewhere inlines). Four steps are taken and the upper 16 bits of each,
// scaled by 2^-16 (u1..u4, each in [0,1)), are used in order:
//   velocity = ((u1 - 0.5) * 5.0, (u2 - 0.5) * 5.0, 0.0);
//   position = p with x raised by (u3 - 0.5) * 36.0 and y raised by (u4 - 0.5) * 36.0 (z unchanged).
// The new state is written back and P_AddParticle is called with the particle definition waveTrail (a
// global of 80 bytes in the image), the position, the velocity and the float 0.0.
// Inferred: this emits one trail particle scattered around a wavelet reflection point; the particle
// definition's contents and the meaning of the last float argument are not established here. Nothing here
// ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
extern unsigned char waveTrail[];
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float arg);

void doWaveletReflection__9WaveMakerP6Point3(void* self, const Point3* point) {
    unsigned& state = *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 68);
    Point3 position = *point;
    Point3 velocity;
    const float unit = __builtin_bit_cast(float, 0x37800000u);
    const float half_one = 0.5f;
    unsigned s = state;
    s = s * 1664525u + 1013904223u;
    const float u1 = static_cast<float>(static_cast<int>(s >> 16)) * unit;
    s = s * 1664525u + 1013904223u;
    const float u2 = static_cast<float>(static_cast<int>(s >> 16)) * unit;
    s = s * 1664525u + 1013904223u;
    const float u3 = static_cast<float>(static_cast<int>(s >> 16)) * unit;
    s = s * 1664525u + 1013904223u;
    const float u4 = static_cast<float>(static_cast<int>(s >> 16)) * unit;
    state = s;
    velocity.x = (u1 - half_one) * 5.0f;
    velocity.y = (u2 - half_one) * 5.0f;
    velocity.z = 0.0f;
    position.x += (u3 - half_one) * 36.0f;
    position.y += (u4 - half_one) * 36.0f;
    P_AddParticle__FP11ParticleDefP6Point3T1f(waveTrail, &position, &velocity, 0.0f);
}
}
