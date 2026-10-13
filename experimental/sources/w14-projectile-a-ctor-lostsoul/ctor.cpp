// Observed __8LostSoulG6Point3T1iP10_vagHeaderii at 0x00219638, 540 bytes. Arguments: the object, a position P and a velocity V (both
// Point3 by value), an integer A, a sound header pointer (not read), and integers B and C. Returns nothing. In order:
//  1. The Projectile constructor runs with (object, P, null, null). _vt$8LostSoul goes to +72.
//  2. A light record at +100 is filled: byte +9 = 0, halfwords +10 and +18 = 172, +12 and +20 = 252, +14 and +22 = 500, +16 and +24 =
//     -32, the singles at +28 and +36 = 120.0, the singles at +32 and +40 = the single 0x3891a2b3, the single +44 = 0.0 and byte +8 = 0.
//     P is copied to the three words at +148 and lightAdd runs with the address object+100.
//  3. The single at +28 is 0.0, the halfword at +56 is A, V goes to the three words at +88, word +168 is C, flag bits 0x4001 are
//     set at +12 and word +164 is B. Word +84 becomes frameNm + 60 when B is 1, else frameNm + 600; objectAddToRunList runs.
//  4. P_StartTrail(4, 750, the single 0x3ea4dd2f, 4, 107, 100, 234, 171) returns a trail id stored in word +172, and
//     P_ExtendTrail(id, the object's position at +16) runs.
//  5. When B is 1 and C is 0, lightDeactivate runs on the light at +100.
// Unknown: what the integers A, B, C mean (the Repulse Undead cast passes the ring angle, 1 and 0), the meaning of the trail and
// light constants, and the role of the two words at +84 and +88. Nothing here ran on the EE.
#include "../w14-projectile-a-common/access.hpp"

extern "C" {
extern int frameNm;
extern unsigned char vt_LostSoul[] __asm__("_vt$8LostSoul");
void projectile_construct(void* self, Point3 position, void* owner, void* map) __asm__("__10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap");
void lightAdd__FP5Light(void* light);
void lightDeactivate__FP5Light(void* light);
void objectAddToRunList__FP10GameObject(void* object);
int P_StartTrail__Fiifiiiii(int a, int b, float c, int d, int e, int f, int g, int h);
void P_ExtendTrail__FiG6Point3(int trail, Point3 position);
void __8LostSoulG6Point3T1iP10_vagHeaderii(void* self, Point3 position, Point3 velocity, int a, void* vag, int b, int c);
}

extern "C" void __8LostSoulG6Point3T1iP10_vagHeaderii(void* self, Point3 position, Point3 velocity, int a, void*, int b, int c) {
    using namespace w14pa;
    unsigned char* const o = static_cast<unsigned char*>(self);
    projectile_construct(self, position, nullptr, nullptr);
    unsigned char* const light = o + 100;
    word(o, 72) = to_word(vt_LostSoul);
    light[9] = 0;
    *reinterpret_cast<Point3*>(o + 148) = position;
    half(light, 18) = 172;
    half(light, 20) = 252;
    half(light, 22) = 500;
    half(light, 24) = -32;
    half(light, 10) = 172;
    half(light, 12) = 252;
    half(light, 14) = 500;
    half(light, 16) = -32;
    real(light, 36) = 120.0f;
    word(light, 40) = 0x3891a2b3u;
    real(light, 28) = 120.0f;
    word(light, 32) = 0x3891a2b3u;
    real(light, 44) = 0.0f;
    light[8] = 0;
    lightAdd__FP5Light(light);
    real(o, 28) = 0.0f;
    half(o, 56) = static_cast<short>(a);
    *reinterpret_cast<Point3*>(o + 88) = velocity;
    word(o, 168) = static_cast<unsigned>(c);
    word(o, 164) = static_cast<unsigned>(b);
    word(o, 12) |= 0x4001u;
    word(o, 84) = static_cast<unsigned>(frameNm) + (b == 1 ? 60u : 600u);
    objectAddToRunList__FP10GameObject(self);
    const int trail = P_StartTrail__Fiifiiiii(4, 750, bits(0x3ea4dd2fu), 4, 107, 100, 234, 171);
    word(o, 172) = static_cast<unsigned>(trail);
    P_ExtendTrail__FiG6Point3(trail, *reinterpret_cast<Point3*>(o + 16));
    if (word(o, 164) == 1 && word(o, 168) == 0) lightDeactivate__FP5Light(light);
}
