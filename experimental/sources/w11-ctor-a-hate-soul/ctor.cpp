// Observed __8HateSoulG6Point3T1iii at 0x1feaf8, 508 bytes. Arguments: the object, two positions
// (by value, P1 and P2) and three ints (A, B, C; B is never read). Returns nothing useful. Steps,
// in order:
//  1. The Projectile constructor runs with (object, P1, null owner, null map).
//  2. Word +72 = _vt$8HateSoul. The light record at object+100 is filled: byte +8 and +9 = 0,
//     halfwords +10 and +18 = 200, +12 and +20 = 30, +14 and +22 = 0, +16 and +24 = -32, floats
//     +28 and +36 = 120.0, +32 and +40 = the single with bits 0x3891a2b3, float +44 = 0.0, and P1 at
//     +48 (object+148). lightAdd__FP5Light(object+100) runs.
//  3. Float +28 = 0.0, halfword +56 = A and P2 is stored at +88, +92, +96.
//  4. eRand__10GameObject(object) is called: word +164 = C, bit 0x4003 is set in the word at
//     +12, word +160 = the roll & 1 and word +84 = frameNm; then objectAddToRunList(object).
//  5. P_StartTrail__Fiifiiiii(8, 35, 0.322 (bits 0x3ea4dd2f), 4, 255, 135, 100, 171) returns a
//     handle stored at +168; P_ExtendTrail__FiG6Point3 receives it and a copy of the three floats
//     at object+16.
//  6. When the word at +160 equals 1 and the word at +164 is 0, lightDeactivate__FP5Light
//     (object+100) runs.
// Unknown: what A and C mean, why B is accepted, and what the roll bit and +84 are used for.
// Nothing here ran on the EE.
#include "../w11-ctor-a-common/ctor.hpp"

extern "C" {
extern unsigned char vt_HateSoul[] __asm__("_vt$8HateSoul");
void __8HateSoulG6Point3T1iii(void* self, Point3 first, Point3 second, int a, int b, int c);
}

extern "C" void __8HateSoulG6Point3T1iii(void* self, Point3 first, Point3 second, int a, int b, int c) {
    using namespace w11;
    (void)b;
    unsigned char* const light = static_cast<unsigned char*>(self) + 100;
    projectile_construct(self, first, nullptr, nullptr);
    u32(self, 72) = word(vt_HateSoul);
    u8(light, 9) = 0;
    f32(self, 148) = first.x;
    f32(self, 152) = first.y;
    f32(self, 156) = first.z;
    f32(light, 36) = 120.0f;
    f32(light, 40) = __builtin_bit_cast(float, 0x3891a2b3u);
    i16(light, 20) = 30;
    i16(light, 24) = -32;
    f32(light, 28) = 120.0f;
    f32(light, 32) = __builtin_bit_cast(float, 0x3891a2b3u);
    i16(light, 12) = 30;
    i16(light, 16) = -32;
    i16(light, 18) = 200;
    i16(light, 10) = 200;
    f32(light, 44) = 0.0f;
    i16(light, 14) = 0;
    i16(light, 22) = 0;
    u8(light, 8) = 0;
    lightAdd__FP5Light(light);
    f32(self, 28) = 0.0f;
    i16(self, 56) = static_cast<short>(a);
    f32(self, 88) = second.x;
    f32(self, 92) = second.y;
    f32(self, 96) = second.z;
    const unsigned roll = eRand__10GameObject(self);
    u32(self, 164) = static_cast<unsigned>(c);
    u32(self, 160) = roll & 1u;
    u32(self, 12) |= 0x4003u;
    i32(self, 84) = frameNm;
    objectAddToRunList__FP10GameObject(self);
    const int trail = P_StartTrail__Fiifiiiii(8, 35, __builtin_bit_cast(float, 0x3ea4dd2fu), 4, 255, 135, 100, 171);
    i32(self, 168) = trail;
    P_ExtendTrail__FiG6Point3(trail, Point3{f32(self, 16), f32(self, 20), f32(self, 24)});
    if (u32(self, 160) == 1 && u32(self, 164) == 0) lightDeactivate__FP5Light(light);
}
