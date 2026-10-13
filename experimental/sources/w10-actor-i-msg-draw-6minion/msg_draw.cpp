// Observed msg_draw__6Minion at 0x002352d0, 100 bytes.
// Calls draw__8Creature with the model header (word +12) and the word at +20 of the resource block
// whose pointer is the word at object+36, and 0 as the third pointer. Its result r is kept. When the
// word at object+964 is nonzero, calls modelDrawAttachment with the block's words +172 and +180, 0,
// the block's header (+12 again, re-read from the object), r, -1, -1 and 0.
// Inferred: r is a draw record, from the callee's parameter type; the attachment is drawn only while
// the word at +964 is set. Unknown: what that word holds. Nothing here ran on the EE.
extern "C" {
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* header, void* texture, void* other);
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
    void* header, void* texture, int index, void* header_again, void* record, unsigned long mask, int a, int b);
}

extern "C" void msg_draw__6Minion(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char* block = *reinterpret_cast<unsigned char**>(object + 36);
    void* record = draw__8CreatureP12_modelHeaderP8_textureT1(
        object, *reinterpret_cast<void**>(block + 12), *reinterpret_cast<void**>(block + 20), nullptr);
    if (*reinterpret_cast<unsigned*>(object + 964) != 0u) {
        block = *reinterpret_cast<unsigned char**>(object + 36);
        modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
            *reinterpret_cast<void**>(block + 172), *reinterpret_cast<void**>(block + 180), 0,
            *reinterpret_cast<void**>(block + 12), record, 0xffffffffu, -1, 0);
    }
}
