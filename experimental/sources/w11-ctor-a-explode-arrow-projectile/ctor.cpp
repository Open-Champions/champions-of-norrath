// Observed __22ExplodeArrowProjectileG6Point3sP10GameObjectiP5Skill at 0x2a2930, 296 bytes.
// Arguments: the object, a position (by value), a short heading, the owner GameObject, an
// integer and the Skill object. The routine returns nothing useful. Steps, in order:
//  1. The Projectile constructor runs with (object, position, owner, null map).
//  2. Halfword +100 = heading; word +72 = _vt$22ExplodeArrowProjectile.
//  3. The skill's table (word at skill+96) is read at byte 104: a signed halfword adjusts the
//     skill pointer and the word at +4 is the function. It is called with (skill + adjust, 1,
//     the integer, the word at (word at object+76)+12) and returns an int, converted to a float.
//     Float +28 = 8.0, word +104 = the integer and word +112 = the skill are stored before the call.
//  4. Float +84 = icos(halfword at +100) * result, word +92 = 0, float +88 = isin(halfword at
//     +100) * result, words +108 and +96 = 0.
//  5. objectAddToRunList runs on the object; then lumpFindResource("fx.lmp",
//     "spel_we_explarrow.anm") is looked up and soundPlaySequence(found, object, 1.0) is called.
// Inferred: the call in step 3 yields a speed or strength from the skill, because its result
// scales the unit direction in step 4. Unknown: what +104, +112 and the node word mean, and
// what the table entry computes. Nothing here ran on the EE.
#include "../w11-ctor-a-common/ctor.hpp"

extern "C" {
extern unsigned char vt_ExplodeArrowProjectile[] __asm__("_vt$22ExplodeArrowProjectile");
void __22ExplodeArrowProjectileG6Point3sP10GameObjectiP5Skill(void* self, Point3 position, short heading, void* owner,
                                                              int amount, void* skill);
}

extern "C" void __22ExplodeArrowProjectileG6Point3sP10GameObjectiP5Skill(void* self, Point3 position, short heading, void* owner,
                                                                          int amount, void* skill) {
    using namespace w11;
    projectile_construct(self, position, owner, nullptr);
    i16(self, 100) = heading;
    u32(self, 72) = word(vt_ExplodeArrowProjectile);
    const int node_word = *reinterpret_cast<int*>(static_cast<unsigned char*>(pointer(u32(self, 76))) + 12);
    f32(self, 28) = 8.0f;
    u32(self, 112) = word(skill);
    u32(self, 104) = static_cast<unsigned>(amount);
    const float scale = static_cast<float>(skill_call_int4(skill, 104, 1, amount, node_word));
    f32(self, 84) = icos__Fi(i16(self, 100)) * scale;
    u32(self, 92) = 0;
    f32(self, 88) = isin__Fi(i16(self, 100)) * scale;
    u32(self, 108) = 0;
    u32(self, 96) = 0;
    objectAddToRunList__FP10GameObject(self);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
        lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_we_explarrow.anm")), self, 1.0f);
}
