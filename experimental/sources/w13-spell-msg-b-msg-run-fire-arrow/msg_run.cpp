// Observed msg_run__19FireArrowProjectile at 0x0029dd78, 300 bytes. O is the object. Nothing happens when the word
// at +96 is non-zero. Otherwise, in order:
//  1. The word at +104 (an age counter) is incremented.
//  2. H = move__10ProjectileG6Point3(O, copy of the three floats at O+84) & 1.
//  3. When H is set, terminate__19FireArrowProjectilei(O, H) ends the step.
//  4. Else when the counter is above 600, terminate__19FireArrowProjectilei(O, 0) ends the step.
//  5. Else fxFlyingFire__FP6Point3T0ffb(O+16, V, s, s * s * 0.021989 + 0.267, true) with s the float at +28 and V the
//     three floats at O+84 each multiplied by 1/60; the three floats at O+16 are copied to O+160; and
//     lightUpdatePosition__FP5Light(O+112) is called.
// Inferred: O+84 is a velocity and +104 an age in frames (the 600 limit is ten seconds at 60 per second); +112 holds
// an embedded light. Unknown: the meaning of the word at +96 and of the float at +28. Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
int move__10ProjectileG6Point3(void* self, Point3 velocity);
void terminate__19FireArrowProjectilei(void* self, int hit);
void fxFlyingFire__FP6Point3T0ffb(Point3* position, Point3* velocity, float a, float b, bool lead);
void lightUpdatePosition__FP5Light(void* light);

void msg_run__19FireArrowProjectile(void* self) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (word(o, 96) != 0u) return;
    sword(o, 104) = sword(o, 104) + 1;
    Point3 step = *reinterpret_cast<Point3*>(o + 84);
    const int hit = move__10ProjectileG6Point3(o, step) & 1;
    if (hit) {
        terminate__19FireArrowProjectilei(o, hit);
        return;
    }
    if (sword(o, 104) > 600) {
        terminate__19FireArrowProjectilei(o, 0);
        return;
    }
    const float s = real(o, 28);
    const float k = bits(0x3c888888u);
    Point3 v;
    v.x = real(o, 84) * k;
    v.y = real(o, 88) * k;
    v.z = real(o, 92) * k;
    fxFlyingFire__FP6Point3T0ffb(reinterpret_cast<Point3*>(o + 16), &v, s, s * s * bits(0x3cb42246u) + bits(0x3e88b439u), true);
    *reinterpret_cast<Point3*>(o + 160) = *reinterpret_cast<Point3*>(o + 16);
    lightUpdatePosition__FP5Light(o + 112);
}
}
