#include "../w10-actor-b-common/fields.hpp"
// Observed msg_draw__10Arenabeast at 0x236950, 68 bytes. modelSetShadowScale(5.0), then
// Creature::draw(object, model, texture, 0) where model and texture are the words at
// +12 and +20 of the record whose address is the word at object+36. Nothing is
// written to the object. Unknown: why Arenabeast needs no angle offset (AntQueen does).
extern "C" {
void modelSetShadowScale__Ff(float scale);
void draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* shadow);
}

extern "C" void msg_draw__10Arenabeast(void* self) {
    using namespace actor_b;
    modelSetShadowScale__Ff(5.0f);
    void* group = link(self, 36);
    draw__8CreatureP12_modelHeaderP8_textureT1(self, link(group, 12), link(group, 20), nullptr);
}
