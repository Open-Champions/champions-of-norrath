// Observed __12ShockOfFrostG6Point3siP10GameObjectP5Skill at 0x2bc788, 492 bytes. Arguments: the
// object, a position (by value), a short heading, an int N, an owner GameObject (may be null)
// and a Skill pointer. Returns nothing useful. Steps, in order:
//  1. The Projectile constructor runs with (object, position, owner, null map).
//  2. Word +72 = _vt$12ShockOfFrost, word +84 (a list head) = 0. The light record at object+88
//     is filled: byte +8 and +9 = 0, halfwords +10 and +18 = 0, +12 and +20 = 128, +14 and +22 =
//     255, +16 and +24 = 150, floats +28 and +36 = 105.0, +32 and +40 = the single with bits
//     0x38be37c6, float +44 = the single with bits 0x3f4d0e45, and the position at +48
//     (object+136). lightAdd__FP5Light(object+88) runs.
//  3. When the owner is non-null, listOnewayLink__FP8ListHeadT0Pv(object+84, owner, owner) runs.
//  4. Halfword +148 = N, halfword +56 = heading. A vector (icos(heading) * 350.0, isin(halfword
//     +56) * 350.0, 0) is stored at +156, +160, +164.
//  5. Word +200 = the skill, word +196 = 0, word +192 = frameNm + 120, word +204 = 0.
//  6. objectAddToRunList(object); then lumpFindResource("fx.lmp", "spel_ew_shockfrost.anm") and
//     soundPlaySequence(found, object, 1.0).
// Inferred: the vector at +156 is a velocity with speed 350, because it is the heading's unit
// direction scaled. Unknown: what N, the skill word and +192..+204 mean. Nothing here ran on the
// EE.
#include "../w11-ctor-a-common/ctor.hpp"

extern "C" {
extern unsigned char vt_ShockOfFrost[] __asm__("_vt$12ShockOfFrost");
void __12ShockOfFrostG6Point3siP10GameObjectP5Skill(void* self, Point3 position, short heading, int n, void* owner, void* skill);
}

extern "C" void __12ShockOfFrostG6Point3siP10GameObjectP5Skill(void* self, Point3 position, short heading, int n, void* owner, void* skill) {
    using namespace w11;
    unsigned char* const light = static_cast<unsigned char*>(self) + 88;
    projectile_construct(self, position, owner, nullptr);
    u32(self, 72) = word(vt_ShockOfFrost);
    u32(self, 84) = 0;
    u8(light, 9) = 0;
    f32(self, 136) = position.x;
    f32(self, 140) = position.y;
    f32(self, 144) = position.z;
    f32(light, 36) = 105.0f;
    f32(light, 40) = __builtin_bit_cast(float, 0x38be37c6u);
    f32(light, 44) = __builtin_bit_cast(float, 0x3f4d0e45u);
    i16(light, 20) = 128;
    i16(light, 22) = 255;
    i16(light, 24) = 150;
    f32(light, 28) = 105.0f;
    f32(light, 32) = __builtin_bit_cast(float, 0x38be37c6u);
    i16(light, 10) = 0;
    i16(light, 12) = 128;
    i16(light, 14) = 255;
    i16(light, 16) = 150;
    i16(light, 18) = 0;
    u8(light, 8) = 0;
    lightAdd__FP5Light(light);
    if (owner) listOnewayLink__FP8ListHeadT0Pv(static_cast<unsigned char*>(self) + 84, owner, owner);
    i16(self, 148) = static_cast<short>(n);
    i16(self, 56) = heading;
    const float cosine = icos__Fi(heading);
    const float sine = isin__Fi(i16(self, 56));
    const Point3 velocity = {cosine * 350.0f, sine * 350.0f, 0.0f};
    f32(self, 156) = velocity.x;
    f32(self, 160) = velocity.y;
    f32(self, 164) = velocity.z;
    u32(self, 200) = word(skill);
    u32(self, 196) = 0;
    i32(self, 192) = frameNm + 120;
    u32(self, 204) = 0;
    objectAddToRunList__FP10GameObject(self);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
        lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_ew_shockfrost.anm")), self, 1.0f);
}
