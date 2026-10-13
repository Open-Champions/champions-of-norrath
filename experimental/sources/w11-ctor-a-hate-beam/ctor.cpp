// Observed __8HateBeamiG6Point3T2P8CreatureP5Skill at 0x1fcbf8, 372 bytes. Arguments: the object,
// an int N, two positions (by value, P1 and P2; P2 is never read), a Creature pointer and a
// Skill pointer. Returns nothing useful. Steps, in order:
//  1. The GameObject constructor runs with (object, P1, null map, null names).
//  2. Word +72 = _vt$8HateBeam, word +76 = 0. The light record at object+104 is filled: bytes
//     +8 and +9 = 0, halfwords +10 and +18 = 115, +12 and +20 = 10, +14 and +22 = 0, +16 and +24 =
//     -32, floats +28 and +36 = 156.0, +32 and +40 = the single with bits 0x382c598a, word +44 =
//     0 and the three floats of P1 at +48 (object+152). lightAdd__FP5Light(object+104) runs.
//  3. Word +168 = N and 0x421b is set in the word at +12; when the creature is non-null,
//     listOnewayLink__FP8ListHeadT0Pv(object+76, creature, creature) runs.
//  4. Word +164 = the skill and word +172 = frameNm + 180, then objectAddToRunList(object).
// Unknown: the meaning of N, of +164/+168/+172, of the flag bits and of the light fields. Nothing
// here ran on the EE.
#include "../w11-ctor-a-common/ctor.hpp"

extern "C" {
extern unsigned char vt_HateBeam[] __asm__("_vt$8HateBeam");
void __8HateBeamiG6Point3T2P8CreatureP5Skill(void* self, int n, Point3 first, Point3 second, void* creature, void* skill);
}

extern "C" void __8HateBeamiG6Point3T2P8CreatureP5Skill(void* self, int n, Point3 first, Point3 second, void* creature, void* skill) {
    using namespace w11;
    (void)second;
    unsigned char* const light = static_cast<unsigned char*>(self) + 104;
    game_object_construct(self, first, nullptr, nullptr);
    u32(self, 72) = word(vt_HateBeam);
    u32(self, 76) = 0;
    u8(light, 9) = 0;
    f32(self, 152) = first.x;
    f32(self, 156) = first.y;
    f32(self, 160) = first.z;
    i16(light, 18) = 115;
    i16(light, 10) = 115;
    f32(light, 36) = 156.0f;
    f32(light, 40) = __builtin_bit_cast(float, 0x382c598au);
    i16(light, 20) = 10;
    i16(light, 24) = -32;
    f32(light, 28) = 156.0f;
    f32(light, 32) = __builtin_bit_cast(float, 0x382c598au);
    u32(light, 44) = 0;
    i16(light, 12) = 10;
    i16(light, 14) = 0;
    i16(light, 16) = -32;
    i16(light, 22) = 0;
    u8(light, 8) = 0;
    lightAdd__FP5Light(light);
    u32(self, 168) = static_cast<unsigned>(n);
    u32(self, 12) |= 0x421bu;
    if (creature) listOnewayLink__FP8ListHeadT0Pv(static_cast<unsigned char*>(self) + 76, creature, creature);
    u32(self, 164) = word(skill);
    i32(self, 172) = frameNm + 180;
    objectAddToRunList__FP10GameObject(self);
}
