#include "../w10-actor-q-common/q.hpp"
// Observed msg_draw__14WoodElfSoldier at 0x00249c88, 60 bytes.
// Draws through the Creature draw routine with the model word at m+12, the texture word at m+20
// and a zero fourth argument, where m is the pointer stored at object+36, then calls
// drawArrow__14WoodElfSoldier on the object. Nothing is returned.
// Inferred: drawArrow adds the soldier's arrow after the body. Unknown: what drawArrow draws.
// Nothing here ran on the EE.
extern "C" {
int draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* second);
void drawArrow__14WoodElfSoldier(void* self);
}

extern "C" void msg_draw__14WoodElfSoldier(void* self) {
    using namespace w10q;
    unsigned char* m = link(self, 36);
    draw__8CreatureP12_modelHeaderP8_textureT1(self, link(m, 12), link(m, 20), nullptr);
    drawArrow__14WoodElfSoldier(self);
}
