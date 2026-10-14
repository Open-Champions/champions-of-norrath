#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_draw__11SmallSpider at 0x20f8d0, 72 bytes.
// modelSetShadowBrightness is called with positive zero, then the same draw
// arguments as the mermaid draw, then modelResetOptionalParams. The object
// is not written. Those two model calls are not defined here.
extern "C" void modelSetShadowBrightness__Ff(float brightness);
extern "C" void modelResetOptionalParams__Fv();
extern "C" void draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra);
extern "C" void msg_draw__11SmallSpider(void* self) {
    using namespace vslots_b;
    modelSetShadowBrightness__Ff(0.0f);
    const Address group = address(pointer(read_word(address(self) + 36u)));
    draw__8CreatureP12_modelHeaderP8_textureT1(self, pointer(read_word(group + 12u)), pointer(read_word(group + 20u)), nullptr);
    modelResetOptionalParams__Fv();
}
