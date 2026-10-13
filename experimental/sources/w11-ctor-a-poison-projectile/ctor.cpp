// Observed __16PoisonProjectilePQ210GameObject11resourceMapiiiiG6Point3T6ifP8Creature at 0x20fbe0,
// 356 bytes. Arguments: the object, a resource map, four ints (A, B, C, D), two positions
// (by value, P1 and P2), an int E, a float F and a Creature pointer (the owner). The routine
// returns nothing useful. Steps, in order:
//  1. The Projectile constructor runs with (object, P1, creature, map).
//  2. Word +72 = _vt$16PoisonProjectile, byte +105 = 1, float +160 = F, words +84, +88, +92,
//     +96 = A, B, C, D.
//  3. animInitState(object+100), then animAdd(object+100, header, 1) where the header is the
//     word at +4 of eight-byte entry C of the table whose address is the word at object+36,
//     then animRandomizeTime(object+100), then objectAddToRunList(object).
//  4. Afterwards: float +164 = P1.z, the three floats of P2 at +140, halfword +56 = E, word
//     +152 = 0, bit 0x1 of the word at +12 is set, and word +156 = frameNm + 300.
// Inferred: the 100-byte-offset region is an animation state because of the calls it is passed
// to. Unknown: what A to E mean (C selects an entry of the table at +36), why +160/+164 hold
// F and P1.z, and what +140..+148 and +156 are for. Nothing here ran on the EE.
#include "../w11-ctor-a-common/ctor.hpp"

extern "C" {
extern unsigned char vt_PoisonProjectile[] __asm__("_vt$16PoisonProjectile");
void __16PoisonProjectilePQ210GameObject11resourceMapiiiiG6Point3T6ifP8Creature(
    void* self, void* map, int a, int b, int c, int d, Point3 first, Point3 second, int e, float f, void* creature);
}

extern "C" void __16PoisonProjectilePQ210GameObject11resourceMapiiiiG6Point3T6ifP8Creature(
    void* self, void* map, int a, int b, int c, int d, Point3 first, Point3 second, int e, float f, void* creature) {
    using namespace w11;
    unsigned char* const state = static_cast<unsigned char*>(self) + 100;
    projectile_construct(self, first, creature, map);
    u32(self, 72) = word(vt_PoisonProjectile);
    u8(state, 5) = 1;
    f32(self, 160) = f;
    i32(self, 84) = a;
    i32(self, 88) = b;
    i32(self, 92) = c;
    i32(self, 96) = d;
    animInitState__FP14AnimationState(state);
    const unsigned char* const table = static_cast<const unsigned char*>(pointer(u32(self, 36)));
    animAdd__FP14AnimationStateP15AnimationHeaderi(state, pointer(*reinterpret_cast<const unsigned*>(table + i32(self, 92) * 8 + 4)), 1);
    animRandomizeTime__FP14AnimationState(state);
    objectAddToRunList__FP10GameObject(self);
    f32(self, 164) = first.z;
    f32(self, 140) = second.x;
    f32(self, 144) = second.y;
    f32(self, 148) = second.z;
    i16(self, 56) = static_cast<short>(e);
    u32(self, 152) = 0;
    u32(self, 12) |= 0x1u;
    i32(self, 156) = frameNm + 300;
}
