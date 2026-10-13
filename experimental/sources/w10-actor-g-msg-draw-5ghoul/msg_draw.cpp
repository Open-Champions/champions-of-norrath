// Observed msg_draw__5Ghoul at 0x001f75b8, 40 bytes.
// Draws through the Creature draw routine with the word at m+12 as the second argument, the word
// at m+20 as the third and zero as the fourth, where m is the pointer stored at object+36. The
// result of the draw call is returned (the call is a tail call in the image).
// Inferred: m is a resource block holding a model and a texture. Nothing here ran on the EE.
#include "../w10-actor-g-common/words.hpp"

extern "C" {
int draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* second);
}

extern "C" int msg_draw__5Ghoul(void* self) {
    unsigned char* m = w10g::from_word(w10g::word_at(self, 36));
    void* model = w10g::from_word(*reinterpret_cast<unsigned*>(m + 12));
    void* texture = w10g::from_word(*reinterpret_cast<unsigned*>(m + 20));
    return draw__8CreatureP12_modelHeaderP8_textureT1(self, model, texture, nullptr);
}
