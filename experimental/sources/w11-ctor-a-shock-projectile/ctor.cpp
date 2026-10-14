// Observed __15ShockProjectileG6Point3sP10GameObjectP5Skilli at 0x28c670, 672 bytes. Arguments: the
// object, a position (by value), a short heading, an owner GameObject (may be null), a Skill
// pointer and an int N. Returns nothing useful. Steps, in order:
//  1. The GameObject constructor runs with (object, position, null map, null names).
//  2. Word +72 = _vt$15ShockProjectile, words +80 and +84 = 0. The light record at object+332 is
//     filled: byte +8 and +9 = 0, halfwords +10 and +18 = 0, +12 and +20 = 225, +14 and +22 = 320,
//     +16 and +24 = -32, floats +28 and +36 = 156.0, +32 and +40 = the single with bits 0x382c598a,
//     float +44 = 0.0 and the position at +48 (object+380). lightAdd__FP5Light(object+332) runs.
//  3. Word +404 (a list head) = 0; lumpFindResource("fx.lmp", "spel_ew_shock.anm") is looked up and
//     soundPlaySequence(found, OWNER, 1.0) is called - the owner, not the object, is passed.
//  4. Bit 0x1 of the word at +12 is set, halfword +56 = heading, word +88 = frameNm, word +76 =
//     the skill, byte +392 = N, float +28 = 12.0. When the owner is non-null,
//     listOnewayLink__FP8ListHeadT0Pv(object+404, owner, owner) runs.
//  5. Float +400 = 341.0 (bits 0x43aa8000) and byte +393 = 0. c = icos(heading), s = isin(halfword
//     +56); float +296 = 0.0; r = rsqrt.s(1.0, c*c + s*s). The direction at +288, +292 is
//     (c*r*k, s*r*k) with k = (float at +400) * 0.0166667 (bits 0x3c888888); k is read back from
//     memory for each component.
//  6. The skill's table (word at the skill word +76, entry at byte 112: a signed halfword adjusts
//     `this`, the function word is at +4) is called with (skill + adjust, 1, the signed byte at
//     +392, the address object+404) and returns an int; byte +394 = result + 1.
//  7. objectAddToRunList(object), then the same lump lookup and soundPlaySequence(found, object,
//     1.0).
// Inferred: the direction is a unit vector scaled to 341/60 units, because it is normalised then
// scaled; the table entry in slot 112 takes a list head in the image. Unknown: what N, +400..+394
// and the table entry's result mean. Nothing here ran on the EE.
#include "../w11-ctor-a-common/ctor.hpp"

namespace {
float reciprocal_sqrt(float length_squared) {
#if defined(__mips__)
    float result;
    float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(length_squared));
    return result;
#else
    return 1.0f / __builtin_sqrtf(length_squared);
#endif
}
}  // namespace

extern "C" {
extern unsigned char vt_ShockProjectile[] __asm__("_vt$15ShockProjectile");
void __15ShockProjectileG6Point3sP10GameObjectP5Skilli(void* self, Point3 position, short heading, void* owner, void* skill, int n);
}

extern "C" void __15ShockProjectileG6Point3sP10GameObjectP5Skilli(void* self, Point3 position, short heading, void* owner, void* skill, int n) {
    using namespace w11;
    unsigned char* const light = static_cast<unsigned char*>(self) + 332;
    unsigned char* const head = static_cast<unsigned char*>(self) + 404;
    game_object_construct(self, position, nullptr, nullptr);
    u32(self, 72) = word(vt_ShockProjectile);
    u32(self, 80) = 0;
    u32(self, 84) = 0;
    u8(light, 9) = 0;
    f32(self, 380) = position.x;
    f32(self, 384) = position.y;
    f32(self, 388) = position.z;
    f32(light, 40) = __builtin_bit_cast(float, 0x382c598au);
    f32(light, 32) = __builtin_bit_cast(float, 0x382c598au);
    f32(light, 36) = 156.0f;
    i16(light, 20) = 225;
    i16(light, 22) = 320;
    i16(light, 24) = -32;
    f32(light, 28) = 156.0f;
    i16(light, 12) = 225;
    i16(light, 14) = 320;
    i16(light, 16) = -32;
    f32(light, 44) = 0.0f;
    i16(light, 10) = 0;
    i16(light, 18) = 0;
    u8(light, 8) = 0;
    lightAdd__FP5Light(light);
    u32(self, 404) = 0;
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
        lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_ew_shock.anm")), owner, 1.0f);
    u32(self, 12) |= 0x1u;
    i16(self, 56) = heading;
    i32(self, 88) = frameNm;
    u32(self, 76) = word(skill);
    u8(self, 392) = static_cast<unsigned char>(n);
    f32(self, 28) = 12.0f;
    if (owner) listOnewayLink__FP8ListHeadT0Pv(head, owner, owner);
    u8(self, 393) = 0;
    f32(self, 400) = __builtin_bit_cast(float, 0x43aa8000u);
    const float cosine = icos__Fi(i16(self, 56));
    const float sine = isin__Fi(i16(self, 56));
    f32(self, 296) = 0.0f;
    const float scale = reciprocal_sqrt(cosine * cosine + sine * sine);
    const float step = __builtin_bit_cast(float, 0x3c888888u);
    f32(self, 292) = sine * scale;
    f32(self, 288) = cosine * scale;
    f32(self, 288) = f32(self, 288) * (f32(self, 400) * step);
    f32(self, 292) = f32(self, 292) * (f32(self, 400) * step);
    void* const stored_skill = pointer(u32(self, 76));
    const int result = skill_call_int4(stored_skill, 112, 1, static_cast<signed char>(u8(self, 392)), static_cast<int>(word(head)));
    u8(self, 394) = static_cast<unsigned char>(result + 1);
    objectAddToRunList__FP10GameObject(self);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
        lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_ew_shock.anm")), self, 1.0f);
}
