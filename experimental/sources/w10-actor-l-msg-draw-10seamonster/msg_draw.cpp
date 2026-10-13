#include "../w10-actor-l-common/access.hpp"
// Observed msg_draw__10SeaMonster at 0x001f8b60, 40 bytes. The word at object+36 is the address
// of the object's resource group; the words at +12 and +20 of that group are passed, after the
// object itself, to the Creature draw routine with a final argument of zero. Nothing is
// written and no value is returned. Nothing here ran on the EE.
extern "C" {
void draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, int extra);

void msg_draw__10SeaMonster(void* self) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const group = w10l::from_word(w10l::word(o, 36));
    draw__8CreatureP12_modelHeaderP8_textureT1(self, w10l::from_word(w10l::word(group, 12)),
                                               w10l::from_word(w10l::word(group, 20)), 0);
}
}
