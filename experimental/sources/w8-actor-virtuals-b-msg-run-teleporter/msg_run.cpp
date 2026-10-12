// Observed msg_run__10Teleporter at 0x00216380, 636 bytes.
// Calls objectActivationCheck__8Creaturei(object, 600) and ignores the result.
// Then emits two particles, one with the data object at 0x0042c670 (symbol
// teleportParticle1) and one with the one at 0x0042c6c0 (teleportParticle2). Each
// loop in the image runs exactly once (counter 0 decremented to -1 before the test).
// Per particle: r1 = eRand(object), r2 = eRand(object), yaw = short at object+56 and
//   a = r1 / 65536 - 0.5
//   x = icos(yaw) * a * 27 * 2 + position.x     (position = floats at object+16)
//   y = isin(yaw) * a * 27 * 2 + position.y
//   z = (r2 / 65536 - 0.5) * 65 * 2 + 65 + position.z
// Velocity is (0, 0, 0). The float argument of P_AddParticle is 0.0.
// Call order per particle: eRand, icos, isin, eRand, P_AddParticle.

extern "C" {
int objectActivationCheck__8Creaturei(void* object, int value);
int eRand__10GameObject(void* object);
float icos__Fi(int angle);
float isin__Fi(int angle);
void P_AddParticle__FP11ParticleDefP6Point3T1f(const void* definition, float* position, float* velocity,
                                               float extra);
extern const unsigned char teleportParticle1[];
extern const unsigned char teleportParticle2[];
}

namespace {
typedef short HalfWord __attribute__((may_alias));
typedef float FloatWord __attribute__((may_alias));

void emit(void* self, const unsigned char* definition) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const FloatWord* origin = reinterpret_cast<const FloatWord*>(object + 16);
    float position[3];
    float velocity[3];
    float a = static_cast<float>(eRand__10GameObject(self)) * 1.52587890625e-05f + -0.5f;
    position[0] = icos__Fi(*reinterpret_cast<const HalfWord*>(object + 56)) * a * 27.0f * 2.0f;
    position[1] = isin__Fi(*reinterpret_cast<const HalfWord*>(object + 56)) * a * 27.0f * 2.0f;
    float b = static_cast<float>(eRand__10GameObject(self)) * 1.52587890625e-05f + -0.5f;
    position[2] = b * 65.0f * 2.0f + 65.0f;
    position[0] += origin[0];
    position[1] += origin[1];
    position[2] += origin[2];
    velocity[0] = 0.0f;
    velocity[1] = 0.0f;
    velocity[2] = 0.0f;
    P_AddParticle__FP11ParticleDefP6Point3T1f(definition, position, velocity, 0.0f);
}
}

extern "C" void msg_run__10Teleporter(void* self) {
    objectActivationCheck__8Creaturei(self, 600);
    emit(self, teleportParticle1);
    emit(self, teleportParticle2);
}
