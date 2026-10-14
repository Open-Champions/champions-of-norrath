#include "../w10-actor-q-common/q.hpp"
// Observed msg_draw__6Wraith at 0x00205258, 84 bytes.
// Calls modelSetFade__Fc with the signed byte at object+960. Then, when that byte (read again) is
// below 98, draws through the Creature draw routine with the model word at m+12, the texture word at
// m+20 and a zero fourth argument, where m is the pointer stored at object+36. Finally calls
// modelResetOptionalParams__Fv. Nothing is returned.
// Inferred: the byte at +960 is a fade level and values from 98 up skip the draw. Unknown: its range.
// Nothing here ran on the EE.
extern "C" {
void modelSetFade__Fc(char fade);
int draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* second);
void modelResetOptionalParams__Fv();
}

extern "C" void msg_draw__6Wraith(void* self) {
    using namespace w10q;
    modelSetFade__Fc(static_cast<char>(sbyte(self, 960)));
    if (sbyte(self, 960) < 98) {
        unsigned char* m = link(self, 36);
        draw__8CreatureP12_modelHeaderP8_textureT1(self, link(m, 12), link(m, 20), nullptr);
    }
    modelResetOptionalParams__Fv();
}
