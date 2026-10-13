// Observed __19FireArrowProjectileG6Point3sP10GameObjectiP5Skill at 0x29db90, 484 bytes.
// Arguments: the object, a position (by value), a short heading, the owner GameObject, an
// integer and the Skill object. Returns nothing useful. Steps, in order:
//  1. The Projectile constructor runs with (object, position, owner, null map).
//  2. Word +72 = _vt$19FireArrowProjectile. The light record at object+112 is filled (see
//     w11::fill_arrow_light in ctor.hpp) with halfwords (109, 100, 76), floats 96.0 and the
//     single with bits 0x38e38e38, and the position, then lightAdd__FP5Light(object+112).
//  3. Word +172 = the skill; halfword +100 = heading; word +108 = the integer.
//  4. The skill's table (word at skill+96) is read at byte 88: a signed halfword adjusts the
//     skill pointer, the word at +4 is the function. It is called with (skill + adjust, 2, the
//     integer); the int result is converted to a float and stored at +28. (The image's Skill
//     tables all hold evaluateRampAtRank__5Skillii at 0x313548 in this slot; it reads only
//     those three argument registers.)
//  5. The same slot of the skill read back from +172 is called with (skill + adjust, 1, the word
//     read back from +108); its int result, as a float, scales the direction: float +84 =
//     icos(halfword +100) * result, word +92 = 0, float +88 = isin(halfword +100) * result.
//     Words +104 and +96 = 0.
//  6. objectAddToRunList runs, then lumpFindResource("fx.lmp", "spel_we_firearrow.anm") and
//     soundPlaySequence(found, object, 1.0).
// Unknown: the meaning of +108/+172, of the light fields and of the table calls' first
// arguments (2 and 1). Nothing here ran on the EE.
#include "../w11-ctor-a-common/ctor.hpp"

extern "C" {
extern unsigned char vt_FireArrowProjectile[] __asm__("_vt$19FireArrowProjectile");
void __19FireArrowProjectileG6Point3sP10GameObjectiP5Skill(void* self, Point3 position, short heading, void* owner,
                                                           int amount, void* skill);
}

extern "C" void __19FireArrowProjectileG6Point3sP10GameObjectiP5Skill(void* self, Point3 position, short heading, void* owner,
                                                                       int amount, void* skill) {
    using namespace w11;
    projectile_construct(self, position, owner, nullptr);
    u32(self, 72) = word(vt_FireArrowProjectile);
    fill_arrow_light(self, 109, 100, 76, 96.0f, __builtin_bit_cast(float, 0x38e38e38u), position);
    lightAdd__FP5Light(static_cast<unsigned char*>(self) + 112);
    u32(self, 172) = word(skill);
    i16(self, 100) = heading;
    i32(self, 108) = amount;
    f32(self, 28) = static_cast<float>(skill_call_int3(skill, 88, 2, amount));
    void* const stored_skill = pointer(u32(self, 172));
    const float scale = static_cast<float>(skill_call_int3(stored_skill, 88, 1, i32(self, 108)));
    f32(self, 84) = icos__Fi(i16(self, 100)) * scale;
    u32(self, 92) = 0;
    f32(self, 88) = isin__Fi(i16(self, 100)) * scale;
    u32(self, 104) = 0;
    u32(self, 96) = 0;
    objectAddToRunList__FP10GameObject(self);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
        lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_we_firearrow.anm")), self, 1.0f);
}
