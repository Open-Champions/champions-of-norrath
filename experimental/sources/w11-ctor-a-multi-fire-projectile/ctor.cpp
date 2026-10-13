// Observed __19MultiFireProjectileG6Point3sP10GameObjectiP5Skill at 0x2a12d8, 332 bytes.
// Arguments as for the ExplodeArrowProjectile constructor: the object, a position (by value),
// a short heading, the owner GameObject, an integer and the Skill object. Returns nothing
// useful. Steps, in order:
//  1. The Projectile constructor runs with (object, position, owner, null map).
//  2. Halfword +100 = heading; word +72 = _vt$19MultiFireProjectile; float +28 = 8.0; word +112
//     = the skill; word +104 = the integer.
//  3. The skill's table (word at skill+96) is read at byte 112: a signed halfword adjusts the
//     skill pointer and the word at +4 is the function. It is called with (skill + adjust, 1,
//     the integer, the address object+76) and returns an int, converted to a float.
//  4. Float +84 = icos(halfword at +100) * result, word +92 = 0, float +88 = isin(halfword at
//     +100) * result, words +108 and +96 = 0.
//  5. P_StartTrail__Fiifiiiii(1, 122, 0.15, 0, 128, 128, 128, 32) returns a handle stored at
//     +116. P_ExtendTrail__FiG6Point3 receives that handle and a copy of the three floats at
//     object+16 (the object's position). objectAddToRunList runs last.
// Inferred: the trail follows the arrow, since it starts at the object position. Unknown: the
// meaning of the trail arguments, of +104/+112, and what the table entry computes. Nothing here
// ran on the EE.
#include "../w11-ctor-a-common/ctor.hpp"

extern "C" {
extern unsigned char vt_MultiFireProjectile[] __asm__("_vt$19MultiFireProjectile");
void __19MultiFireProjectileG6Point3sP10GameObjectiP5Skill(void* self, Point3 position, short heading, void* owner,
                                                           int amount, void* skill);
}

extern "C" void __19MultiFireProjectileG6Point3sP10GameObjectiP5Skill(void* self, Point3 position, short heading, void* owner,
                                                                       int amount, void* skill) {
    using namespace w11;
    projectile_construct(self, position, owner, nullptr);
    i16(self, 100) = heading;
    u32(self, 72) = word(vt_MultiFireProjectile);
    f32(self, 28) = 8.0f;
    u32(self, 112) = word(skill);
    u32(self, 104) = static_cast<unsigned>(amount);
    const float scale = static_cast<float>(skill_call_int4(skill, 112, 1, amount, static_cast<int>(word(static_cast<unsigned char*>(self) + 76))));
    f32(self, 84) = icos__Fi(i16(self, 100)) * scale;
    u32(self, 92) = 0;
    f32(self, 88) = isin__Fi(i16(self, 100)) * scale;
    u32(self, 108) = 0;
    u32(self, 96) = 0;
    const int trail = P_StartTrail__Fiifiiiii(1, 122, 0.15f, 0, 128, 128, 128, 32);
    i32(self, 116) = trail;
    P_ExtendTrail__FiG6Point3(trail, Point3{f32(self, 16), f32(self, 20), f32(self, 24)});
    objectAddToRunList__FP10GameObject(self);
}
