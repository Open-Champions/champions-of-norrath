// Observed msg_run__19MultiFireProjectile at 0x002a1428, 716 bytes. O is the object. Nothing happens when the word at
// +96 is non-zero. Otherwise, in order:
//  1. P_ExtendTrail__FiG6Point3(the word at +116, copy of the three floats at O+16) is called.
//  2. The word at +108 (an age counter) is incremented; H = move__10ProjectileG6Point3(O, copy of the three floats at
//     O+84) & 1. When H is set, terminate__19MultiFireProjectilei(O, H) ends the step; when the counter is above 600,
//     terminate__19MultiFireProjectilei(O, 0) does.
//  3. Ten times: a vector D inside the unit ball is drawn by rejection (each component is
//     float(eRand__10GameObject(O)) * 2^-16 doubled minus 1, drawn x, y, z in that order, redrawn while
//     1.0 < x*x + y*y + z*z); V = D * 8.0; f = float(eRand__10GameObject(O)) * 2^-16 (not doubled); and
//     P_AddParticle__FP11ParticleDefP6Point3T1f(the file-local particle record at 0x008d7040, P, V, 0.0) is called with P = O+16 + V * f.
// The particle record is a file-local object of the original unit (symbol name `trail`, two such statics exist), so it is
// imported under an address name, like the other msg_run particles in this repository.
// Inferred: a multi-shot fire arrow in flight. Unknown: the meaning of the word at +96 and of the trail handle at +116.
// Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
extern unsigned char multifire_msg_run_particle_8d7040[];
int eRand__10GameObject(void* self);
int move__10ProjectileG6Point3(void* self, Point3 velocity);
void terminate__19MultiFireProjectilei(void* self, int hit);
void P_ExtendTrail__FiG6Point3(int handle, Point3 position);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age);

void msg_run__19MultiFireProjectile(void* self) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (word(o, 96) != 0u) return;
    Point3 here = *reinterpret_cast<Point3*>(o + 16);
    P_ExtendTrail__FiG6Point3(sword(o, 116), here);
    sword(o, 108) = sword(o, 108) + 1;
    Point3 step = *reinterpret_cast<Point3*>(o + 84);
    const int hit = move__10ProjectileG6Point3(o, step) & 1;
    if (hit) {
        terminate__19MultiFireProjectilei(o, hit);
        return;
    }
    if (sword(o, 108) > 600) {
        terminate__19MultiFireProjectilei(o, 0);
        return;
    }
    auto rand = [](unsigned char* object) { return static_cast<unsigned>(eRand__10GameObject(object)); };
    for (int i = 0; i < 10; ++i) {
        Point3 d;
        unit_ball(o, d, rand);
        Point3 velocity;
        velocity.x = d.x * 8.0f;
        velocity.y = d.y * 8.0f;
        velocity.z = d.z * 8.0f;
        const float fraction = static_cast<float>(eRand__10GameObject(o)) * bits(0x37800000u);
        Point3 offset;
        offset.x = velocity.x * fraction;
        offset.y = velocity.y * fraction;
        offset.z = velocity.z * fraction;
        Point3 position;
        position.x = real(o, 16) + offset.x;
        position.y = real(o, 20) + offset.y;
        position.z = real(o, 24) + offset.z;
        P_AddParticle__FP11ParticleDefP6Point3T1f(multifire_msg_run_particle_8d7040, &position, &velocity, 0.0f);
    }
}
}
