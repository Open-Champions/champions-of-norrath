#include "../w10-actor-l-common/access.hpp"
// Observed msg_draw__8Scorpion at 0x001f2dc8, 56 bytes. The word at object+36 is the address of
// the object's resource group and the signed byte at object+960 selects an 8-byte entry of it.
// The Creature draw routine receives the object, the group word at +12, the selected entry's
// word at +20 (that is, group + index * 8 + 20) and a final zero. Nothing is written and no
// value is returned. Unknown: what the byte at +960 means beyond selecting the entry; whether
// negative values are ever stored. Nothing here ran on the EE.
extern "C" {
void draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, int extra);

void msg_draw__8Scorpion(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    const signed char index = static_cast<signed char>(o[960]);
    unsigned char* const group = w10l::from_word(w10l::word(o, 36));
    draw__8CreatureP12_modelHeaderP8_textureT1(self, w10l::from_word(w10l::word(group, 12)),
                                               w10l::from_word(w10l::word(group + index * 8, 20)), 0);
}
}
