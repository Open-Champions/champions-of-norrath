// Observed msg_draw__6Goblin at 0x0022c808, 400 bytes. t is the table at word +36 (t[12] is the model
// header, t[20] the texture). The resource map at 0x42cfc8 (128 bytes) is read as words: W[n] is
// the n-th word. Steps in the order of the image:
//  1. When the word at +988 (call it P) is nonzero: fxRunWeapon (full mangled name below) runs with
//     the description at object+1064, the model word W[1 + 2P], the weapon state at object+1000, a
//     copy of the 12 bytes at +16, the signed halfword at +56, t[12], the animation state at +288,
//     0.5, the int (signed halfword +572 == 6), the int 0, the float at +224 and a null matrix.
//  2. Draws through the Creature draw routine (t[12], t[20], fourth argument zero); the result is
//     the draw record R used below.
//  3. When P is nonzero: modelDrawAttachment(W[1 + 2P], W[15 + 2P], 0, t[12], R, -1, -1, 0).
//  4. When the word at +992 (Q) is nonzero: the same call with Q in place of P.
//  5. When the word at +972 equals 4 and the word at +996 is nonzero: the same call with W[3] and
//     W[17].
// Inferred: the map is the 16-slot resource map authored in w9-ctors-lantern-a-views (a name word
// then a word that is zero in the file); the second word of each slot is read here, so it must be
// filled with the loaded resource at run time. Slots 1..7 are then models and slots 8..14 the matching
// textures (W[15 + 2P] is slot P + 7). P and Q are weapon numbers; the embedded description and
// state belong to the item and effect code. Unknown: what +972/+996 mean. The map is linked through
// the authored name norrath_ctors_ant_lantern_view_42cfc8 (the image symbol is
// goblinWeaponResourceMap). Nothing here ran on the EE.
#include "../w10-actor-g-common/words.hpp"

struct Point3 { float x, y, z; };

extern "C" {
extern unsigned goblinWeaponResourceMap[] __asm__("norrath_ctors_ant_lantern_view_42cfc8");
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* description, void* weaponModel, void* weaponState, Point3 position, short angle, void* model,
    void* animation, float a, int b, int c, float d, void* matrix);
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* second);
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
    void* model, void* texture, int a, void* model2, void* record, unsigned long b, int c, int d);
}

namespace {
void* pointer_word(unsigned word) { return w10g::from_word(word); }
}  // namespace

extern "C" void msg_draw__6Goblin(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    const unsigned P = *reinterpret_cast<unsigned*>(o + 988);
    unsigned char* t = w10g::from_word(w10g::word_at(self, 36));
    if (P != 0) {
        fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
            o + 1064, pointer_word(goblinWeaponResourceMap[1 + 2 * P]), o + 1000,
            *reinterpret_cast<Point3*>(o + 16), *reinterpret_cast<short*>(o + 56),
            pointer_word(*reinterpret_cast<unsigned*>(t + 12)), o + 288, 0.5f,
            *reinterpret_cast<short*>(o + 572) == 6, 0, *reinterpret_cast<float*>(o + 224), nullptr);
    }
    t = w10g::from_word(w10g::word_at(self, 36));
    void* record = draw__8CreatureP12_modelHeaderP8_textureT1(
        self, pointer_word(*reinterpret_cast<unsigned*>(t + 12)), pointer_word(*reinterpret_cast<unsigned*>(t + 20)), nullptr);
    if (P != 0) {
        modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
            pointer_word(goblinWeaponResourceMap[1 + 2 * P]), pointer_word(goblinWeaponResourceMap[15 + 2 * P]), 0,
            pointer_word(*reinterpret_cast<unsigned*>(w10g::from_word(w10g::word_at(self, 36)) + 12)), record, -1, -1, 0);
    }
    const unsigned Q = *reinterpret_cast<unsigned*>(o + 992);
    if (Q != 0) {
        modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
            pointer_word(goblinWeaponResourceMap[1 + 2 * Q]), pointer_word(goblinWeaponResourceMap[15 + 2 * Q]), 0,
            pointer_word(*reinterpret_cast<unsigned*>(w10g::from_word(w10g::word_at(self, 36)) + 12)), record, -1, -1, 0);
    }
    if (*reinterpret_cast<unsigned*>(o + 972) == 4u) {
        if (*reinterpret_cast<unsigned*>(o + 996) != 0) {
            modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
                pointer_word(goblinWeaponResourceMap[3]), pointer_word(goblinWeaponResourceMap[17]), 0,
                pointer_word(*reinterpret_cast<unsigned*>(w10g::from_word(w10g::word_at(self, 36)) + 12)), record, -1, -1, 0);
        }
    }
}
