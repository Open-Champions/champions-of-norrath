#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_draw__7Mermaid at 0x1f8060, 40 bytes.
// The word at object+36 is a pointer. draw receives that pointer's word at
// +12, its word at +20, and a null fourth argument. The object is not written.
extern "C" void draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra);
extern "C" void msg_draw__7Mermaid(void* self) {
    using namespace vslots_b;
    const Address group = address(pointer(read_word(address(self) + 36u)));
    draw__8CreatureP12_modelHeaderP8_textureT1(self, pointer(read_word(group + 12u)), pointer(read_word(group + 20u)), nullptr);
}
