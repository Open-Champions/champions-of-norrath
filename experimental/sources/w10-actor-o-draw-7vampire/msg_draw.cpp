// Observed msg_draw__7Vampire at 0x00243600, 40 bytes. Calls draw__8Creature(object, R[12], R[20], 0)
// where R is the block at the word object+36; the value it returns is not used. Nothing else is
// read or written. The returned record is discarded here; the image leaves the callee's return
// register in place when it returns, which this void function does not claim to reproduce.
// Unknown: what the block words are (model header and texture by the argument types).
// Nothing here ran on the EE.
#include "../w10-actor-o-common/access.hpp"

extern "C" {
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* texture2);

void msg_draw__7Vampire(void* self) {
    using namespace w10o;
    unsigned char* const group = from_word(word(static_cast<unsigned char*>(self), 36));
    draw__8CreatureP12_modelHeaderP8_textureT1(self, from_word(word(group, 12)), from_word(word(group, 20)), nullptr);
}
}
