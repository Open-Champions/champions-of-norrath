#include "../w10-actor-b-common/fields.hpp"
// Observed msg_draw__8AntQueen at 0x208938, 104 bytes. In order:
//  1. modelSetShadowScale(2.0).
//  2. The halfword at object+56 has the halfword at +988 subtracted (16-bit store).
//  3. Creature::draw(object, model, texture, 0): model and texture are the words at
//     +12 and +20 of the record whose address is the word at object+36.
//  4. The halfword at +56 has the halfword at +988 added back, then
//     modelResetOptionalParams().
// Inferred: +56 is the facing angle and +988 an angle offset applied only while drawing.
// Unknown: what +988 holds for other creature classes.
extern "C" {
void modelSetShadowScale__Ff(float scale);
void draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* shadow);
void modelResetOptionalParams__Fv();
}

extern "C" void msg_draw__8AntQueen(void* self) {
    using namespace actor_b;
    modelSetShadowScale__Ff(2.0f);
    field<Half>(self, 56) = field<Half>(self, 56) - field<Half>(self, 988);
    void* group = link(self, 36);
    draw__8CreatureP12_modelHeaderP8_textureT1(self, link(group, 12), link(group, 20), nullptr);
    field<Half>(self, 56) = field<Half>(self, 56) + field<Half>(self, 988);
    modelResetOptionalParams__Fv();
}
