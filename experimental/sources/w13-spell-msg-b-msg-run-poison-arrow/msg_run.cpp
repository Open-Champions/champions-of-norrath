// Observed msg_run__21PoisonArrowProjectile at 0x002a0148, 952 bytes. O is the object. Nothing happens when the word at
// +96 is non-zero. Otherwise, in order:
//  1. The word at +108 (an age counter) is incremented; H = move__10ProjectileG6Point3(O, copy of the three floats at
//     O+84) & 1. When H is set, terminate__21PoisonArrowProjectilei(O, H) ends the step; when the counter is above 600,
//     terminate__21PoisonArrowProjectilei(O, 0) does.
//  2. The three floats at O+16 are copied to O+160 and lightUpdatePosition__FP5Light(O+112) is called.
//  3. Forty-six times, counting i from 0: a vector D inside the unit ball is drawn by rejection (each component is
//     float(eRand__10GameObject(O)) * 2^-16 doubled minus 1, drawn x, y, z in that order, redrawn while
//     1.0 < x*x + y*y + z*z), V = D * 30.0, a second unit-ball vector E is drawn, r = the float at O+28, and
//     P_AddParticle__FP11ParticleDefP6Point3T1f(def, P, V, 0.0) is called with P = O+16 + (E * r) * 0.50799495 (the
//     float 0x3f020bf5, read through the global pointer). def is poisonSmoke for odd i and poisonFleck for even i.
// Inferred: a poison arrow in flight leaves a trail. Unknown: the meaning of the word at +96 and the float at +28.
// Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
extern unsigned char poisonSmoke[];
extern unsigned char poisonFleck[];
int eRand__10GameObject(void* self);
int move__10ProjectileG6Point3(void* self, Point3 velocity);
void terminate__21PoisonArrowProjectilei(void* self, int hit);
void lightUpdatePosition__FP5Light(void* light);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age);

namespace {
void emit(unsigned char* o, void* definition) {
    using namespace w13smb;
    auto rand = [](unsigned char* object) { return static_cast<unsigned>(eRand__10GameObject(object)); };
    Point3 d;
    unit_ball(o, d, rand);
    Point3 velocity;
    velocity.x = d.x * 30.0f;
    velocity.y = d.y * 30.0f;
    velocity.z = d.z * 30.0f;
    Point3 e;
    unit_ball(o, e, rand);
    const float r = real(o, 28);
    Point3 offset;
    offset.x = e.x * r;
    offset.y = e.y * r;
    offset.z = e.z * r;
    const float factor = bits(0x3f020bf5u);
    Point3 scaled;
    scaled.x = offset.x * factor;
    scaled.y = offset.y * factor;
    scaled.z = offset.z * factor;
    Point3 position;
    position.x = real(o, 16) + scaled.x;
    position.y = real(o, 20) + scaled.y;
    position.z = real(o, 24) + scaled.z;
    P_AddParticle__FP11ParticleDefP6Point3T1f(definition, &position, &velocity, 0.0f);
}
}  // namespace

void msg_run__21PoisonArrowProjectile(void* self) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (word(o, 96) != 0u) return;
    sword(o, 108) = sword(o, 108) + 1;
    Point3 step = *reinterpret_cast<Point3*>(o + 84);
    const int hit = move__10ProjectileG6Point3(o, step) & 1;
    if (hit) {
        terminate__21PoisonArrowProjectilei(o, hit);
        return;
    }
    if (sword(o, 108) > 600) {
        terminate__21PoisonArrowProjectilei(o, 0);
        return;
    }
    *reinterpret_cast<Point3*>(o + 160) = *reinterpret_cast<Point3*>(o + 16);
    lightUpdatePosition__FP5Light(o + 112);
    for (int i = 0; i < 46; ++i) emit(o, (i & 1) ? poisonSmoke : poisonFleck);
}
}
