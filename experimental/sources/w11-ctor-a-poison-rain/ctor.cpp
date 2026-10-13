// Observed __10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader at 0x2be410, 704 bytes.
// Arguments: the object, an int N, an owner GameObject (must be non-null: it is dereferenced), a
// position (by value), a Skill pointer and an AnimationHeader pointer (may be null). Returns
// nothing useful. In every skill call below the callee is the entry at byte 112 of the table whose
// address is the word at skill+96 (a signed halfword adjusts `this`, the function word is at +4);
// it is called as (skill + adjust, K, N, address of object+100) and returns an int. Steps:
//  1. The GameObject constructor runs with (object, position, null map, null names).
//  2. Word +72 = _vt$10PoisonRain, word +92 = N, bits 0x421b are set in the word at +12, word +96 =
//     the skill, word +100 (a list head) = 0, and listOnewayLink__FP8ListHeadT0Pv(object+100,
//     owner, owner) runs.
//  3. Skill call with K = 1 -> r1; float +84 = (float)(r1 * 12).
//  4. If (word at owner+12 & 0x60) == 0x20, word +80 = (int)((float)frameNm + 40.5) (the single
//     with bits 0x4221ffff), else word +80 = 0.
//  5. Skill call with K = 2 -> r2; word +76 = word(+80) + 60 * r2. blockAlloc__Fi(188) returns
//     storage; when it is non-null, a skill call with K = 2 -> r3 is made and the LightEffect
//     constructor (__11LightEffectG6Point3ffffffffffffP10GameObjecti) runs on the storage with
//     the point (position.x + 0.0, position.y, position.z + 12.0), the floats 0, 255, 0, 100, 0,
//     255, 0, 100, the float at +84, twice that float, 0.65 (bits 0x3f266666), (float)r3 + 0.675
//     (bits 0x3f2ccccc), a null owner and the int 1.
//  6. Skill call with K = 0 -> r4: word +88 = r4. Word +104 = -1; when the header is non-null,
//     soundPlaySequence(header, object, 1.0) runs and its result replaces word +104.
//  7. objectAddToRunList(object).
// Unknown: what N and the words at +76..+104 mean, why the owner flag test is 0x20 under 0x60, and
// what each LightEffect float controls. Nothing here ran on the EE.
#include "../w11-ctor-a-common/ctor.hpp"

extern "C" {
extern unsigned char vt_PoisonRain[] __asm__("_vt$10PoisonRain");
void __10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader(void* self, int n, void* owner, Point3 position, void* skill,
                                                                    void* header);
}

extern "C" void __10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader(void* self, int n, void* owner, Point3 position, void* skill,
                                                                               void* header) {
    using namespace w11;
    unsigned char* const head = static_cast<unsigned char*>(self) + 100;
    game_object_construct(self, position, nullptr, nullptr);
    u32(self, 72) = word(vt_PoisonRain);
    u32(self, 92) = static_cast<unsigned>(n);
    u32(self, 12) |= 0x421bu;
    u32(self, 96) = word(skill);
    u32(self, 100) = 0;
    listOnewayLink__FP8ListHeadT0Pv(head, owner, owner);
    const int first = skill_call_int4(pointer(u32(self, 96)), 112, 1, i32(self, 92), static_cast<int>(word(head)));
    f32(self, 84) = static_cast<float>(first * 12);
    if ((u32(owner, 12) & 0x60u) == 0x20u)
        i32(self, 80) = static_cast<int>(static_cast<float>(frameNm) + __builtin_bit_cast(float, 0x4221ffffu));
    else
        u32(self, 80) = 0;
    const int second = skill_call_int4(pointer(u32(self, 96)), 112, 2, i32(self, 92), static_cast<int>(word(head)));
    i32(self, 76) = i32(self, 80) + second * 60;
    void* const storage = blockAlloc__Fi(188);
    if (storage) {
        const Point3 where = {position.x + 0.0f, position.y, position.z + 12.0f};
        const float base = f32(self, 84);
        const int third = skill_call_int4(pointer(u32(self, 96)), 112, 2, i32(self, 92), static_cast<int>(word(head)));
        light_effect_construct(storage, where, 0.0f, 255.0f, 0.0f, 100.0f, 0.0f, 255.0f, 0.0f, 100.0f, base, base + base,
                               __builtin_bit_cast(float, 0x3f266666u), static_cast<float>(third) + __builtin_bit_cast(float, 0x3f2cccccu),
                               nullptr, 1);
    }
    const int fourth = skill_call_int4(pointer(u32(self, 96)), 112, 0, i32(self, 92), static_cast<int>(word(head)));
    i32(self, 88) = fourth;
    i32(self, 104) = -1;
    if (header) i32(self, 104) = soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, self, 1.0f);
    objectAddToRunList__FP10GameObject(self);
}
