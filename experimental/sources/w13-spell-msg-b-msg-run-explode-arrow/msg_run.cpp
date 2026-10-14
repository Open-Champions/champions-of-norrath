// Observed msg_run__22ExplodeArrowProjectile at 0x002a1e78, 732 bytes. O is the object. Nothing happens when the word at
// +96 is non-zero. Otherwise, in order:
//  1. The word at +108 (an age counter) is incremented; H = move__10ProjectileG6Point3(O, copy of the three floats at
//     O+84) & 1. When H is set, terminate__22ExplodeArrowProjectilei(O, 1) ends the step; when the counter is above 600,
//     terminate__22ExplodeArrowProjectilei(O, 0) does.
//  2. Six times: a vector D inside the unit ball is drawn by rejection (each component is
//     float(eRand__10GameObject(O)) * 2^-16 doubled minus 1, drawn x, y, z in that order, redrawn while
//     1.0 < x*x + y*y + z*z); V = D * 3.0; f = float(eRand__10GameObject(O)) * 2^-16 (not doubled); and
//     P_AddParticle__FP11ParticleDefP6Point3T1f(the file-local particle record at 0x008d7090, P, V, 0.0) is called with
//     P = O+16 + ((the three floats at O+84) * 0.016666668) * f, the factor being the float 0x3c888888 (about 1/60) read
//     through the global pointer.
// The particle record is a file-local object of the original unit (symbol name `trail`, two such statics exist), so it is
// imported under an address name, like the other msg_run particles in this repository.
// Inferred: an explosive arrow in flight. Unknown: the meaning of the word at +96. Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
extern unsigned char explode_arrow_msg_run_particle_8d7090[];
int eRand__10GameObject(void* self);
int move__10ProjectileG6Point3(void* self, Point3 velocity);
void terminate__22ExplodeArrowProjectilei(void* self, int hit);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age);

void msg_run__22ExplodeArrowProjectile(void* self) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (word(o, 96) != 0u) return;
    sword(o, 108) = sword(o, 108) + 1;
    Point3 step = *reinterpret_cast<Point3*>(o + 84);
    const int hit = move__10ProjectileG6Point3(o, step) & 1;
    if (hit) {
        terminate__22ExplodeArrowProjectilei(o, 1);
        return;
    }
    if (sword(o, 108) > 600) {
        terminate__22ExplodeArrowProjectilei(o, 0);
        return;
    }
    auto rand = [](unsigned char* object) { return static_cast<unsigned>(eRand__10GameObject(object)); };
    const float factor = bits(0x3c888888u);
    for (int i = 0; i < 6; ++i) {
        Point3 d;
        unit_ball(o, d, rand);
        Point3 velocity;
        velocity.x = d.x * 3.0f;
        velocity.y = d.y * 3.0f;
        velocity.z = d.z * 3.0f;
        Point3 drift;
        drift.x = real(o, 84) * factor;
        drift.y = real(o, 88) * factor;
        drift.z = real(o, 92) * factor;
        const float fraction = static_cast<float>(eRand__10GameObject(o)) * bits(0x37800000u);
        Point3 position;
        position.x = real(o, 16) + drift.x * fraction;
        position.y = real(o, 20) + drift.y * fraction;
        position.z = real(o, 24) + drift.z * fraction;
        P_AddParticle__FP11ParticleDefP6Point3T1f(explode_arrow_msg_run_particle_8d7090, &position, &velocity, 0.0f);
    }
}
}
