#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_draw__5Demon at 0x1eb348, 108 bytes.
// Draws the creature with the model at (object+36)+12 and the texture at (object+36)+20
// (null fourth argument). When the word at object+960 is non-zero, it indexes eight-byte
// records at the group (object+36): modelDrawAttachment is called with the model word at
// record+4, the texture word at record+12, zero, the group model at group+12, the value
// the creature draw returned, then -1, -1 and 0.
// Unknown: what the creature draw returns beyond being handed on, and what the index selects.
extern "C" void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra);
extern "C" void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
    void* model, void* texture, int zero, void* group_model, void* draw_record, unsigned long mask, int a, int b);
extern "C" void msg_draw__5Demon(void* self) {
    using namespace vslots_b;
    Address group = address(pointer(read_word(address(self) + 36u)));
    void* record = draw__8CreatureP12_modelHeaderP8_textureT1(self, pointer(read_word(group + 12u)),
                                                              pointer(read_word(group + 20u)), nullptr);
    const unsigned index = read_word(address(self) + 960u);
    if (index != 0u) {
        group = address(pointer(read_word(address(self) + 36u)));
        const Address entry = group + (index << 3);
        modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
            pointer(read_word(entry + 4u)), pointer(read_word(entry + 12u)), 0, pointer(read_word(group + 12u)),
            record, static_cast<unsigned long>(-1), -1, 0);
    }
}
