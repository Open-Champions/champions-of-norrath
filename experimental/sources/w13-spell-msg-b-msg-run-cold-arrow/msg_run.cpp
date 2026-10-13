// Observed msg_run__19ColdArrowProjectile at 0x0029e868, 1540 bytes. O is the object. Nothing happens when the word at
// +96 is non-zero. Otherwise, in order:
//  1. The word at +108 (an age counter) is incremented; H = move__10ProjectileG6Point3(O, copy of the three floats at
//     O+84) & 1. When H is set, terminate__19ColdArrowProjectilei(O, H) ends the step; when the counter is above 600,
//     terminate__19ColdArrowProjectilei(O, 0) does.
//  2. The three floats at O+16 are copied to O+160 and lightUpdatePosition__FP5Light(O+112) is called.
//  3. Seven puffs, then one speck. Each draws a vector D inside the unit ball by rejection (each component is
//     float(eRand__10GameObject(O)) * 2^-16 doubled minus 1, drawn x, y, z in that order, redrawn while
//     1.0 < x*x + y*y + z*z), makes the velocity V = D * k, draws a second unit-ball vector E, takes r = the float at
//     O+28, and calls P_AddParticle__FP11ParticleDefP6Point3T1f(def, P, V, 0.0) with P = O+16 + (E * r) * 0.25.
//     The puffs use def = coldSmoke and k = 15.0; the single speck uses coldSpeck and k = 14.0.
// Inferred: a cold arrow in flight leaves a smoke trail. Unknown: the meaning of the word at +96 and the float at +28.
// Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
extern unsigned char coldSmoke[];
extern unsigned char coldSpeck[];
int eRand__10GameObject(void* self);
int move__10ProjectileG6Point3(void* self, Point3 velocity);
void terminate__19ColdArrowProjectilei(void* self, int hit);
void lightUpdatePosition__FP5Light(void* light);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age);

namespace {
void emit(unsigned char* o, void* definition, float k) {
    using namespace w13smb;
    auto rand = [](unsigned char* object) { return static_cast<unsigned>(eRand__10GameObject(object)); };
    Point3 d;
    unit_ball(o, d, rand);
    Point3 velocity;
    velocity.x = d.x * k;
    velocity.y = d.y * k;
    velocity.z = d.z * k;
    Point3 e;
    unit_ball(o, e, rand);
    const float r = real(o, 28);
    Point3 offset;
    offset.x = e.x * r;
    offset.y = e.y * r;
    offset.z = e.z * r;
    Point3 scaled;
    scaled.x = offset.x * 0.25f;
    scaled.y = offset.y * 0.25f;
    scaled.z = offset.z * 0.25f;
    Point3 position;
    position.x = real(o, 16) + scaled.x;
    position.y = real(o, 20) + scaled.y;
    position.z = real(o, 24) + scaled.z;
    P_AddParticle__FP11ParticleDefP6Point3T1f(definition, &position, &velocity, 0.0f);
}
}  // namespace

void msg_run__19ColdArrowProjectile(void* self) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (word(o, 96) != 0u) return;
    sword(o, 108) = sword(o, 108) + 1;
    Point3 step = *reinterpret_cast<Point3*>(o + 84);
    const int hit = move__10ProjectileG6Point3(o, step) & 1;
    if (hit) {
        terminate__19ColdArrowProjectilei(o, hit);
        return;
    }
    if (sword(o, 108) > 600) {
        terminate__19ColdArrowProjectilei(o, 0);
        return;
    }
    *reinterpret_cast<Point3*>(o + 160) = *reinterpret_cast<Point3*>(o + 16);
    lightUpdatePosition__FP5Light(o + 112);
    for (int i = 0; i < 7; ++i) emit(o, coldSmoke, 15.0f);
    emit(o, coldSpeck, 14.0f);
}
}
