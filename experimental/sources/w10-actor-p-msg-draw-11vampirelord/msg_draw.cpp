// Observed msg_draw__11VampireLord at 0x00240ac0, 296 bytes. Always starts with
// modelSetShadowBrightness__Ff(0.0) and ends the model part with modelResetOptionalParams__Fv.
//  - When the word at object+964 is nonzero: the model is lumpFindResource__FPcT0("bat.lmp",
//    "bat.vif") and the texture lumpFindResource("bat.lmp", "bat.tex"), drawn with
//    draw__8CreatureP12_modelHeaderP8_textureT1(this, model, texture, 0). Nothing else is drawn.
//  - Otherwise, with R the block at the word object+36: draw(this, R[12], R[20], 0) returns a record
//    and the reset runs. Then, only if the word at +960 is nonzero:
//    modelSetRelightAttachment__FG6Point3(copy of the point at +16),
//    lightSetAmbientOffset__FPs(the shorts at +1120),
//    modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(R[172], R[180], 0, R[12],
//    the record, -1, -1, 0), lightResetOptionalParams__Fv(), modelResetOptionalParams__Fv().
// Unknown: what +960 and +964 select (the bat path is the second form). Nothing here ran on the EE.
#include "../w10-actor-p-common/access.hpp"

extern "C" {
void modelSetShadowBrightness__Ff(float brightness);
void* lumpFindResource__FPcT0(const char* lump, const char* resource);
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* texture2);
void modelResetOptionalParams__Fv();
void modelSetRelightAttachment__FG6Point3(Point3 position);
void lightSetAmbientOffset__FPs(short* offset);
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* model, void* texture, int a, void* model2, void* record,
                                                                         unsigned long b, int c, int d);
void lightResetOptionalParams__Fv();
}

extern "C" void msg_draw__11VampireLord(void* self) {
    using namespace w10p;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (word(o, 964) != 0u) {
        modelSetShadowBrightness__Ff(0.0f);
        void* const model = lumpFindResource__FPcT0("bat.lmp", "bat.vif");
        void* const texture = lumpFindResource__FPcT0("bat.lmp", "bat.tex");
        draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, nullptr);
        modelResetOptionalParams__Fv();
        return;
    }
    modelSetShadowBrightness__Ff(0.0f);
    unsigned char* const block = from_word(word(o, 36));
    void* const record = draw__8CreatureP12_modelHeaderP8_textureT1(self, from_word(word(block, 12)), from_word(word(block, 20)), nullptr);
    modelResetOptionalParams__Fv();
    if (word(o, 960) != 0u) {
        modelSetRelightAttachment__FG6Point3(*reinterpret_cast<Point3*>(o + 16));
        lightSetAmbientOffset__FPs(reinterpret_cast<short*>(o + 1120));
        unsigned char* const again = from_word(word(o, 36));
        modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
            from_word(word(again, 172)), from_word(word(again, 180)), 0, from_word(word(again, 12)), record, static_cast<unsigned long>(-1), -1, 0);
        lightResetOptionalParams__Fv();
        modelResetOptionalParams__Fv();
    }
}
