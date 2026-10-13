#include "../w10-actor-m-common/access.hpp"
// Observed msg_draw__8Skeleton at 0x2446f8, 388 bytes. Nothing is returned. The word at object+36 points to a
// table of resource pointers; entries at byte 12 (model) and 20 (texture) are used. skeletonResMap
// (0x42d8d0) is read as an array of words with an 8-byte stride; Skeleton uses entries chosen by the words at
// object+976 and +1120, and two fixed words at map+652 and map+660. In order:
//  1. When the word at +976 (k) is not zero: fxRunWeapon (0.5f, 1.0f; see the prototype) is called with the
//     description at object+1048, the map word at 4+8k, the state at object+980, a copy of the 12 bytes at
//     object+16, the signed halfword at +56, the table's model, the animation state at object+288, a flag that is
//     1 when the signed halfword at +572 is 6, and zeros for the int and matrix arguments.
//  2. draw__8CreatureP12_modelHeaderP8_textureT1(object, model, texture, 0) runs; its result r is kept.
//  3. When k is not zero: modelDrawAttachment(map word 4+8k, map word 12+8k, 0, table model, r, -1, -1, 0).
//  4. When the word at +1120 (j) is not zero: the same call with the map words 4+8j and 12+8j.
//  5. When the word at +1124 is not zero: the same call with the map words at byte offsets 652 and 660.
// Inferred: the +1048 description and +980 weapon state, and the 8-byte map pairs, are weapon attachments
// (names from the callee types); Unknown: which pairs the map holds, the fixed 652/660 pair, and the meaning of
// the argument 0.5f and 1.0f.
extern "C" {
extern unsigned char skeletonResMap[];
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* description, void* model, void* state, Point3 position, short angle, void* model_again, void* animation,
    float a, int flag, int zero, float b, void* matrix);
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra);
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* model, void* texture, int a, void* model_again,
                                                                         void* record, unsigned long b, int c, int d);
}

namespace {
inline unsigned map_word(unsigned offset) {
    return w10m::word(skeletonResMap, offset);
}
inline void attach(unsigned a_offset, unsigned b_offset, unsigned char* table, void* record) {
    modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
        w10m::from_word(map_word(a_offset)), w10m::from_word(map_word(b_offset)), 0, w10m::from_word(w10m::word(table, 12)), record,
        static_cast<unsigned long>(-1), -1, 0);
}
}  // namespace

extern "C" void msg_draw__8Skeleton(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    const unsigned k = w10m::word(o, 976);
    if (k != 0) {
        unsigned char* table = w10m::from_word(w10m::word(o, 36));
        fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
            o + 1048, w10m::from_word(map_word(4u + (k << 3))), o + 980, *reinterpret_cast<Point3*>(o + 16), w10m::half(o, 56),
            w10m::from_word(w10m::word(table, 12)), o + 288, 0.5f, w10m::half(o, 572) == 6 ? 1 : 0, 0, 1.0f, nullptr);
    }
    unsigned char* table = w10m::from_word(w10m::word(o, 36));
    void* record = draw__8CreatureP12_modelHeaderP8_textureT1(self, w10m::from_word(w10m::word(table, 12)),
                                                            w10m::from_word(w10m::word(table, 20)), nullptr);
    if (w10m::word(o, 976) != 0) {
        attach(4u + (w10m::word(o, 976) << 3), 12u + (w10m::word(o, 976) << 3), w10m::from_word(w10m::word(o, 36)), record);
    }
    const unsigned j = w10m::word(o, 1120);
    if (j != 0) attach(4u + (j << 3), 12u + (j << 3), w10m::from_word(w10m::word(o, 36)), record);
    if (w10m::word(o, 1124) != 0) attach(652u, 660u, w10m::from_word(w10m::word(o, 36)), record);
}
