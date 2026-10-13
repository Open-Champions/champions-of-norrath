#include "../w10-actor-m-common/access.hpp"
// Observed msg_draw__8Skelight at 0x2468a0, 304 bytes. Nothing is returned. The word at object+36 points to a
// table of resource pointers; entries at byte 12 (model) and 20 (texture) are used. When the word at
// object+960 is not zero, a weapon model is drawn along with the creature: the resource named
// "long_sword1.vif" is looked up in the lump "equipmnt.lmp" with lumpFindResource__FPcT0 and passed to
// fxRunWeapon (description at object+1032, state at +968, a copy of the 12 bytes at +16, the signed halfword at
// +56, the table model, the animation state at +288, 0.5f, a flag that is 1 when the signed halfword at +572 is
// 6, 0, 1.0f and a null matrix). Then draw__8CreatureP12_modelHeaderP8_textureT1(object, model, texture, 0) runs
// regardless and its result r is kept. When the +960 word is still not zero, the same model and the texture
// "long_sword1.tex" (same lump) are looked up again, in that order, and passed to
// modelDrawAttachment(model, texture, 0, table model, r, -1, -1, 0). Inferred: the word at +960 selects
// whether the creature carries the sword; Unknown: what else the word holds.
extern "C" {
char* lumpFindResource__FPcT0(const char* lump, const char* name);
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* description, void* model, void* state, Point3 position, short angle, void* model_again, void* animation,
    float a, int flag, int zero, float b, void* matrix);
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra);
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* model, void* texture, int a, void* model_again,
                                                                         void* record, unsigned long b, int c, int d);
}

extern "C" void msg_draw__8Skelight(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    if (w10m::word(o, 960) != 0) {
        void* model = lumpFindResource__FPcT0("equipmnt.lmp", "long_sword1.vif");
        unsigned char* table = w10m::from_word(w10m::word(o, 36));
        fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
            o + 1032, model, o + 968, *reinterpret_cast<Point3*>(o + 16), w10m::half(o, 56),
            w10m::from_word(w10m::word(table, 12)), o + 288, 0.5f, w10m::half(o, 572) == 6 ? 1 : 0, 0, 1.0f, nullptr);
    }
    unsigned char* table = w10m::from_word(w10m::word(o, 36));
    void* record = draw__8CreatureP12_modelHeaderP8_textureT1(self, w10m::from_word(w10m::word(table, 12)),
                                                            w10m::from_word(w10m::word(table, 20)), nullptr);
    if (w10m::word(o, 960) != 0) {
        void* model = lumpFindResource__FPcT0("equipmnt.lmp", "long_sword1.vif");
        void* texture = lumpFindResource__FPcT0("equipmnt.lmp", "long_sword1.tex");
        modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
            model, texture, 0, w10m::from_word(w10m::word(w10m::from_word(w10m::word(o, 36)), 12)), record,
            static_cast<unsigned long>(-1), -1, 0);
    }
}
