// Observed run__15SpellHateEffectG6Point3s at 0x001fe8d0, 548 bytes. O is the object, the first argument. The second
// argument (a point, copied to the callee's frame and never used) and the third are not read. Let H be the word at O+28
// (an object) and C the word at H+12 (read only when H is not null).
//  - When frameNm is not below the signed word at O+32, or H is null, the run ends: when O is not null, the entry at
//    offset 8 of the table whose address is the word at O+20 is called on O (adjusted by the entry's signed halfword) with
//    the argument 3.
//  - Otherwise, on C: the halfwords +148 = 100, +154 = 20, +150 = 10 and +152 = 0 are set; the entry at offset 128 of the
//    table whose address is the word at C+72 is called on C (adjusted by its signed halfword) with no argument and returns
//    a model; then fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi is called with
//    (that model, C+288, a copy of the point at C+16, the signed halfword at C+56, C+824, 4.0, the single at C+224, 0.0,
//    0xff000fdc).
//    When frameNm is then a multiple of 30 (signed remainder zero), an attack is resolved on C: with N the int returned by
//    the entry at offset 96 of the table whose address is the word at the object W = (word at O+44), called on W (adjusted
//    by its signed halfword) with (0, the word at O+40), and R = eRand__5Spell(O) * 2^-16, the call is
//    gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(C, 0, N * 0.5, (N + (N * R) * 0.2) * 0.5, 100000,
//    0, 0, 64, description, false). The description is the 80-byte record that w13a::init_description fills (nothing
//    else is written to it); 0.2 is the pool constant with bits 0x3e4ccccc.
// Inferred: a held hate/shock effect that draws a shock on its target each frame and damages it once every 30 frames.
// Unknown: the meaning of the words at O+28, +32, +40 and +44 beyond their use here. Nothing here ran on the EE.
#include "../w13-spell-msg-a-common/spell.hpp"

extern "C" {
extern int frameNm;
int eRand__5Spell(void* spell);
void fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi(
    void* model, void* animation, const void* position, short yaw, void* state, float a, float b, float c, unsigned color);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float a, float b, int c, int d, int e, int f,
                                                                    void* description, bool g);

void run__15SpellHateEffectG6Point3s(void* self, Point3, short) {
    using namespace w12c;
    unsigned char* o = static_cast<unsigned char*>(self);
    const int frame = frameNm;
    const int limit = sword(o, 32);
    if (frame < limit) {
        unsigned char* const holder = from_word(word(o, 28));
        if (holder != nullptr) {
            unsigned char* const c = from_word(word(holder, 12));
            half(c, 148) = 100;
            half(c, 154) = 20;
            half(c, 150) = 10;
            half(c, 152) = 0;
            unsigned char* const entry = from_word(word(c, 72)) + 128;
            void* const model = reinterpret_cast<void* (*)(void*)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(
                c + *reinterpret_cast<short*>(entry));
            const Point3 position = {real(c, 16), real(c, 20), real(c, 24)};
            fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi(model, c + 288, &position, half(c, 56), c + 824, 4.0f,
                                                                                                    real(c, 224), 0.0f, 0xff000fdcu);
            if (frameNm % 30 != 0) return;
            unsigned char description[80];
            w13a::init_description(description);
            unsigned char* const w = from_word(word(o, 44));
            unsigned char* const target = from_word(word(w, 96)) + 96;
            const int n = reinterpret_cast<int (*)(void*, int, unsigned)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(target + 4)))(
                w + *reinterpret_cast<short*>(target), 0, word(o, 40));
            const float base = static_cast<float>(n);
            const float roll = static_cast<float>(eRand__5Spell(o)) * bits(0x37800000u);
            const float total = base + (base * roll) * bits(0x3e4ccccc);
            gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(c, nullptr, base * 0.5f, total * 0.5f, 100000, 0, 0, 64, description, false);
            return;
        }
    }
    __asm__("" : "+r"(o));
    if (o != nullptr) {
        unsigned char* const entry = from_word(word(o, 20)) + 8;
        reinterpret_cast<void (*)(void*, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(o + *reinterpret_cast<short*>(entry), 3);
    }
}
}
