// Observed __14HateProjectileG6Point3sP10GameObjectP5SkilliP15AnimationHeader at 0x1fd7e0, 452
// bytes. Arguments: the object, a position (by value), a short heading, an owner GameObject
// (may be null), a Skill pointer, an int N and an AnimationHeader pointer. Returns nothing
// useful. Steps, in order:
//  1. The GameObject constructor runs with (object, position, null map, null names).
//  2. Word +72 = _vt$14HateProjectile, words +80 and +84 = 0. The light record at object+324 is
//     filled: byte +8 and +9 = 0, halfwords +10 and +18 = 115, +12 and +20 = 15, +14 and +22 =
//     0, +16 and +24 = -32, floats +28 and +36 = 156.0, +32 and +40 = the single with bits
//     0x382c598a, word +44 = 0 and the position at +48 (object+372). lightAdd__FP5Light runs.
//  3. Word +392 = 0, word +396 = the header, bit 0x1 of the word at +12 is set, halfword +56 =
//     heading, word +88 = frameNm, word +76 = the skill, float +28 = 24.0, byte +384 = N. When
//     the owner is non-null, listOnewayLink__FP8ListHeadT0Pv(object+392, owner, owner) runs.
//  4. Byte +386 = 4, word +92 = 350 and byte +385 = 0, then objectAddToRunList(object).
//  5. lumpFindResource("fx.lmp", "spel_ew_shock.anm") is looked up and
//     soundPlaySequence(found, object, 1.0) is called.
// Unknown: what N, the skill word, +80/+84/+88/+92 and the bytes at +384..+386 mean. Nothing
// here ran on the EE.
#include "../w11-ctor-a-common/ctor.hpp"

extern "C" {
extern unsigned char vt_HateProjectile[] __asm__("_vt$14HateProjectile");
void __14HateProjectileG6Point3sP10GameObjectP5SkilliP15AnimationHeader(void* self, Point3 position, short heading, void* owner,
                                                                       void* skill, int n, void* header);
}

extern "C" void __14HateProjectileG6Point3sP10GameObjectP5SkilliP15AnimationHeader(void* self, Point3 position, short heading, void* owner,
                                                                                   void* skill, int n, void* header) {
    using namespace w11;
    unsigned char* const light = static_cast<unsigned char*>(self) + 324;
    game_object_construct(self, position, nullptr, nullptr);
    u32(self, 72) = word(vt_HateProjectile);
    u32(self, 80) = 0;
    u32(self, 84) = 0;
    u8(light, 9) = 0;
    f32(self, 372) = position.x;
    f32(self, 376) = position.y;
    f32(self, 380) = position.z;
    f32(light, 40) = __builtin_bit_cast(float, 0x382c598au);
    i16(light, 18) = 115;
    i16(light, 20) = 15;
    f32(light, 32) = __builtin_bit_cast(float, 0x382c598au);
    i16(light, 10) = 115;
    i16(light, 12) = 15;
    f32(light, 36) = 156.0f;
    i16(light, 24) = -32;
    f32(light, 28) = 156.0f;
    u32(light, 44) = 0;
    i16(light, 14) = 0;
    i16(light, 16) = -32;
    i16(light, 22) = 0;
    u8(light, 8) = 0;
    lightAdd__FP5Light(light);
    unsigned char* const head = static_cast<unsigned char*>(self) + 392;
    u32(self, 392) = 0;
    u32(self, 396) = word(header);
    u32(self, 12) |= 0x1u;
    i16(self, 56) = heading;
    i32(self, 88) = frameNm;
    u32(self, 76) = word(skill);
    f32(self, 28) = 24.0f;
    u8(self, 384) = static_cast<unsigned char>(n);
    if (owner) listOnewayLink__FP8ListHeadT0Pv(head, owner, owner);
    u8(self, 386) = 4;
    u32(self, 92) = 350;
    u8(self, 385) = 0;
    objectAddToRunList__FP10GameObject(self);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
        lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_ew_shock.anm")), self, 1.0f);
}
