// Observed msg_draw__8Innoruuk at 0x001ff458, 172 bytes.
// Draws through the Creature draw routine with the word at m+12 as the second argument, the word
// at m+20 as the third and zero as the fourth, where m is the pointer stored at object+36; its
// result is kept as the draw record. When the word at object+960 is nonzero it then: calls
// modelSetRelightAttachment__FG6Point3 with a copy of the 12 bytes at object+16, calls
// lightSetAmbientOffset__FPs with object+1168, and calls modelDrawAttachment with the words at
// m+172 and m+180 (m read again from object+36), 0, the word at m+12, the draw record, -1, -1 and
// 0; then lightResetOptionalParams__Fv and modelResetOptionalParams__Fv. Returns nothing.
// Inferred: m is a resource block holding a model and a texture (as in Ghoul's draw), and the
// block's words at +172 and +180 are a second model and texture (the attachment).
// Unknown: what the word at +960 gates. Nothing here ran on the EE.
#include "../w10-actor-h-common/words.hpp"

extern "C" {
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* second);
void modelSetRelightAttachment__FG6Point3(const void* point);
void lightSetAmbientOffset__FPs(short* offsets);
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
    void* model, void* texture, int mode, void* base_model, void* record, unsigned flags, int a, int b);
void lightResetOptionalParams__Fv();
void modelResetOptionalParams__Fv();
}

extern "C" void msg_draw__8Innoruuk(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char* m = w10h::from_word(w10h::word_at(self, 36));
    void* record = draw__8CreatureP12_modelHeaderP8_textureT1(
        self, w10h::from_word(*reinterpret_cast<unsigned*>(m + 12)), w10h::from_word(*reinterpret_cast<unsigned*>(m + 20)), nullptr);
    if (w10h::word_at(self, 960) == 0) return;
    unsigned char point[12];
    w10h::copy_bytes(point, object + 16, 12);
    modelSetRelightAttachment__FG6Point3(point);
    lightSetAmbientOffset__FPs(reinterpret_cast<short*>(object + 1168));
    m = w10h::from_word(w10h::word_at(self, 36));
    modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
        w10h::from_word(*reinterpret_cast<unsigned*>(m + 172)), w10h::from_word(*reinterpret_cast<unsigned*>(m + 180)), 0,
        w10h::from_word(*reinterpret_cast<unsigned*>(m + 12)), record, 0xffffffffu, -1, 0);
    lightResetOptionalParams__Fv();
    modelResetOptionalParams__Fv();
}
