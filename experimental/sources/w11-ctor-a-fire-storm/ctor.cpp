// Observed __9FireStormiP10GameObjectG6Point3P5Skill at 0x29bd40, 860 bytes. Arguments: the object,
// an int N, an owner GameObject (may be null for the link; it is dereferenced for a flag test, so it
// must not be), a position (by value) and a Skill pointer. Returns nothing useful. In the skill
// calls below the callee is the entry at byte 112 of the table whose address is the word at
// skill+96 (a signed halfword adjusts `this`, the function word is at +4); it is called as
// (skill + adjust, K, N, address of object+76) and returns an int. Steps, in order:
//  1. The GameObject constructor runs with (object, position, null map, null names).
//  2. Word +72 = _vt$9FireStorm, word +76 (a list head) = 0. The light record at object+80 is
//     filled: byte +8 and +9 = 0, halfwords +10 and +18 = 255, +12 and +20 = 128, +14 and +22 = 0,
//     +16 and +24 = -16, floats +28 and +36 = 0.0, +32 and +40 = 1.0 / zero (a run-time divide on
//     the target; zero is a volatile 0.0), float +44 = the single with bits 0x3f004156, and the
//     position at +48 (object+128). lightAdd__FP5Light(object+80) runs.
//  3. Bits 0x421b are set in the word at +12; when the owner is non-null,
//     listOnewayLink__FP8ListHeadT0Pv(object+76, owner, owner) runs.
//  4. Word +140 = N, the position is stored at +16, +20, +24, word +144 = the skill. Skill call
//     K = 1 -> r1; word +156 = r1 * 12.
//  5. If (word at owner+12 & 0x60) == 0x20: word +152 = (int)((float)frameNm + 40.5) (the single
//     with bits 0x4221ffff); blockAlloc__Fi(188); when it returns storage, the LightEffect
//     constructor (__11LightEffectG6Point3ffffffffffffP10GameObjecti) runs on it with the point
//     (x + 0.0, y, z + 12.0), the floats 255, 128, 0, 100, 255, 128, 0, 100, (float)word(+156),
//     (float)(word(+156) * 2), 0.65 (bits 0x3f266666), 2.025 (bits 0x40019999), a null owner and
//     the int 1. Then skill call K = 2 -> r2 and word +148 = word(+152) + 60 * r2.
//     Otherwise: word +152 = 0, skill call K = 2 -> r2 and word +148 = frameNm + 60 * r2.
//  6. Word +160 = 0; lightUpdatePosition__FP5Light(object+80); objectAddToRunList(object);
//     lumpFindResource("fx.lmp", "spel_ew_firestorm.anm") and soundPlaySequence(found, object, 1.0)
//     whose result is stored at +164 and passed to soundMakePersistant__Fi.
// Unknown: what N and the words at +140..+164 mean, and what each LightEffect float controls.
// Nothing here ran on the EE.
#include "../w11-ctor-a-common/ctor.hpp"

extern "C" {
extern unsigned char vt_FireStorm[] __asm__("_vt$9FireStorm");
void __9FireStormiP10GameObjectG6Point3P5Skill(void* self, int n, void* owner, Point3 position, void* skill);
}

extern "C" void __9FireStormiP10GameObjectG6Point3P5Skill(void* self, int n, void* owner, Point3 position, void* skill) {
    using namespace w11;
    unsigned char* const light = static_cast<unsigned char*>(self) + 80;
    unsigned char* const head = static_cast<unsigned char*>(self) + 76;
    volatile float zero = 0.0f;
    const float quotient = 1.0f / zero;
    game_object_construct(self, position, nullptr, nullptr);
    u32(self, 72) = word(vt_FireStorm);
    u32(self, 76) = 0;
    f32(self, 128) = position.x;
    f32(self, 132) = position.y;
    f32(self, 136) = position.z;
    u8(light, 9) = 0;
    f32(light, 44) = __builtin_bit_cast(float, 0x3f004156u);
    i16(light, 18) = 255;
    f32(light, 40) = quotient;
    i16(light, 20) = 128;
    i16(light, 24) = -16;
    f32(light, 28) = 0.0f;
    f32(light, 36) = 0.0f;
    f32(light, 32) = quotient;
    i16(light, 10) = 255;
    i16(light, 12) = 128;
    i16(light, 14) = 0;
    i16(light, 16) = -16;
    i16(light, 22) = 0;
    u8(light, 8) = 0;
    lightAdd__FP5Light(light);
    u32(self, 12) |= 0x421bu;
    if (owner) listOnewayLink__FP8ListHeadT0Pv(head, owner, owner);
    u32(self, 140) = static_cast<unsigned>(n);
    f32(self, 16) = position.x;
    f32(self, 20) = position.y;
    f32(self, 24) = position.z;
    u32(self, 144) = word(skill);
    const int first = skill_call_int4(pointer(u32(self, 144)), 112, 1, i32(self, 140), static_cast<int>(word(head)));
    i32(self, 156) = first * 12;
    int base;
    int second;
    if ((u32(owner, 12) & 0x60u) == 0x20u) {
        i32(self, 152) = static_cast<int>(static_cast<float>(frameNm) + __builtin_bit_cast(float, 0x4221ffffu));
        void* const storage = blockAlloc__Fi(188);
        if (storage) {
            const Point3 where = {position.x + 0.0f, position.y, position.z + 12.0f};
            light_effect_construct(storage, where, 255.0f, 128.0f, 0.0f, 100.0f, 255.0f, 128.0f, 0.0f, 100.0f,
                                   static_cast<float>(i32(self, 156)), static_cast<float>(i32(self, 156) * 2),
                                   __builtin_bit_cast(float, 0x3f266666u), __builtin_bit_cast(float, 0x40019999u), nullptr, 1);
        }
        second = skill_call_int4(pointer(u32(self, 144)), 112, 2, i32(self, 140), static_cast<int>(word(head)));
        base = i32(self, 152);
    } else {
        u32(self, 152) = 0;
        second = skill_call_int4(pointer(u32(self, 144)), 112, 2, i32(self, 140), static_cast<int>(word(head)));
        base = frameNm;
    }
    i32(self, 148) = base + second * 60;
    u32(self, 160) = 0;
    lightUpdatePosition__FP5Light(light);
    objectAddToRunList__FP10GameObject(self);
    const int sound = soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
        lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_ew_firestorm.anm")), self, 1.0f);
    i32(self, 164) = sound;
    soundMakePersistant__Fi(sound);
}
