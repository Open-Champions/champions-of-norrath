// Observed msg_draw__9MummyKing at 0x0023beb8, 108 bytes. Let R be the block at the word object+36.
// draw(this, R[12], R[20], 0) draws the creature and returns a record. When the word k at +960 is
// non-zero, modelDrawAttachment(R[8k+4], R[8k+12], 0, R[12], record, -1, -1, 0) follows; no light or
// relight setup surrounds it here. Nothing here ran on the EE.
#include "../w10-actor-j-common/access.hpp"

extern "C" {
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* texture2);
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* model, void* texture, int a, void* model2, void* record,
                                                                         unsigned long b, int c, int d);

void msg_draw__9MummyKing(void* self) {
    using namespace w10j;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char* const block = from_word(word(o, 36));
    void* const record = draw__8CreatureP12_modelHeaderP8_textureT1(self, from_word(word(block, 12)), from_word(word(block, 20)), nullptr);
    const unsigned k = word(o, 960);
    if (k != 0u) {
        unsigned char* const entry = block + k * 8u;
        modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
            from_word(word(entry, 4)), from_word(word(entry, 12)), 0, from_word(word(block, 12)), record, static_cast<unsigned long>(-1), -1, 0);
    }
}
}
