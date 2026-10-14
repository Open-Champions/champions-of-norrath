// Observed __21PoisonArrowProjectileG6Point3sP10GameObjectiP5Skill at 0x29ff88, 448 bytes.
// Arguments: the object, a position (by value), a short heading, the owner GameObject, an
// integer and the Skill object. Returns nothing useful. Steps, in order:
//  1. The Projectile constructor runs with (object, position, owner, null map).
//  2. Word +72 = _vt$21PoisonArrowProjectile. The light record at object+112 is filled (see
//     w11::fill_arrow_light in ctor.hpp) with halfwords (60, 132, 68), floats 120.0 and
//     the single with bits 0x3891a2b3, and the position, then lightAdd__FP5Light(object+112).
//  3. Word +172 = the skill; halfword +100 = heading; float +28 = 5.0; word +104 = the integer.
//  4. The skill's table (word at skill+96) is read at byte 88: a signed halfword adjusts the
//     skill pointer, the word at +4 is the function. It is called with (skill + adjust, 1, the
//     integer) and returns an int, converted to a float. The image's Skill tables all hold the
//     same function in this slot (evaluateRampAtRank__5Skillii at 0x313548), which reads only
//     those three argument registers.
//  5. Float +84 = icos(halfword +100) * result, word +92 = 0, float +88 = isin(halfword +100) *
//     result, words +108 and +96 = 0. objectAddToRunList runs, then lumpFindResource("fx.lmp",
//     "spel_we_poisarrow.anm") and soundPlaySequence(found, object, 1.0).
// Unknown: the meaning of +104/+172, of the light fields and of the table entry. Nothing here
// ran on the EE.
#include "../w11-ctor-a-common/ctor.hpp"

extern "C" {
extern unsigned char vt_PoisonArrowProjectile[] __asm__("_vt$21PoisonArrowProjectile");
void __21PoisonArrowProjectileG6Point3sP10GameObjectiP5Skill(void* self, Point3 position, short heading, void* owner,
                                                           int amount, void* skill);
}

extern "C" void __21PoisonArrowProjectileG6Point3sP10GameObjectiP5Skill(void* self, Point3 position, short heading, void* owner,
                                                                       int amount, void* skill) {
    using namespace w11;
    projectile_construct(self, position, owner, nullptr);
    u32(self, 72) = word(vt_PoisonArrowProjectile);
    fill_arrow_light(self, 60, 132, 68, 120.0f, __builtin_bit_cast(float, 0x3891a2b3u), position);
    lightAdd__FP5Light(static_cast<unsigned char*>(self) + 112);
    u32(self, 172) = word(skill);
    i16(self, 100) = heading;
    f32(self, 28) = 5.0f;
    u32(self, 104) = static_cast<unsigned>(amount);
    const float scale = static_cast<float>(skill_call_int3(skill, 88, 1, amount));
    f32(self, 84) = icos__Fi(i16(self, 100)) * scale;
    u32(self, 92) = 0;
    f32(self, 88) = isin__Fi(i16(self, 100)) * scale;
    u32(self, 108) = 0;
    u32(self, 96) = 0;
    objectAddToRunList__FP10GameObject(self);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
        lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_we_poisarrow.anm")), self, 1.0f);
}
