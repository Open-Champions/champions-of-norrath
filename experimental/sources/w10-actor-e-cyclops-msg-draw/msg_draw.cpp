#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_draw__7Cyclops at 0x203bc8, 68 bytes.
// Sets the model shadow scale to 3.0, then draws the creature with the model at
// (object+36)+12 and the texture at (object+36)+20, with a null fourth argument.
// Unknown: what the shadow scale value is measured in.
extern "C" void modelSetShadowScale__Ff(float scale);
extern "C" void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra);
extern "C" void msg_draw__7Cyclops(void* self) {
    using namespace vslots_b;
    modelSetShadowScale__Ff(3.0f);
    const Address group = address(pointer(read_word(address(self) + 36u)));
    draw__8CreatureP12_modelHeaderP8_textureT1(self, pointer(read_word(group + 12u)), pointer(read_word(group + 20u)), nullptr);
}
