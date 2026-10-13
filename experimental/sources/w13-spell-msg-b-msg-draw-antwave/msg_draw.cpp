// Observed msg_draw__7AntWave at 0x00208480, 360 bytes. O is the object. Nothing happens unless the signed word at +808
// (a frame counter) has remainder zero when divided by 3. Otherwise a vector D inside the unit ball is drawn by rejection
// (each component is float(eRand__10GameObject(O)) * 2^-16 doubled minus 1, drawn x, y, z in that order, redrawn while
// 1.0 < x*x + y*y + z*z) and P_AddParticle__FP11ParticleDefP6Point3T1f(AntWaveParticle1, P, V, 0.0) is called with
// P = O+16 + D * 10.0 and V = (0, 0, 0).
// Inferred: a puff of ant-wave particles every third frame. Unknown: what the particle is called in the original and
// what the counter measures. Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
extern unsigned char AntWaveParticle1[];
int eRand__10GameObject(void* self);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age);

void msg_draw__7AntWave(void* self) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (sword(o, 808) % 3 != 0) return;
    auto rand = [](unsigned char* object) { return static_cast<unsigned>(eRand__10GameObject(object)); };
    Point3 d;
    unit_ball(o, d, rand);
    Point3 position;
    position.x = d.x * 10.0f + real(o, 16);
    position.y = d.y * 10.0f + real(o, 20);
    position.z = d.z * 10.0f + real(o, 24);
    Point3 velocity;
    velocity.x = 0.0f;
    velocity.y = 0.0f;
    velocity.z = 0.0f;
    P_AddParticle__FP11ParticleDefP6Point3T1f(AntWaveParticle1, &position, &velocity, 0.0f);
}
}
