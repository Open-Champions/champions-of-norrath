// Observed msg_draw__11SpiderQueen at 0x0020fa28, 76 bytes. Sets the model shadow scale to 4.0
// with modelSetShadowScale__Ff, then calls draw__8CreatureP12_modelHeaderP8_textureT1 on the
// object with the word at +12 and the word at +20 of the record the object's word at +36 points
// to, and 0 as the last argument. It then calls modelResetOptionalParams__Fv. Returns nothing.
// Unknown: what the record at +36 is called, and what the two words are beyond their use as the
// second and third draw arguments. Nothing here ran on the EE.
extern "C" {
void modelSetShadowScale__Ff(float scale);
void draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* texture2);
void modelResetOptionalParams__Fv();
}

#include "../w10-actor-n-common/access.hpp"

extern "C" void msg_draw__11SpiderQueen(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    modelSetShadowScale__Ff(4.0f);
    unsigned char* record = w10n::from_word(w10n::word(object, 36));
    draw__8CreatureP12_modelHeaderP8_textureT1(self, w10n::from_word(w10n::word(record, 12)), w10n::from_word(w10n::word(record, 20)), nullptr);
    modelResetOptionalParams__Fv();
}
