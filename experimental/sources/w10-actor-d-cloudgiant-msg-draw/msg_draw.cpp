// Observed msg_draw__10CloudGiant at 0x001e9990, 112 bytes.
// Let m be the pointer stored at object+36. Draws through the Creature draw routine with the
// object, the words at m+12 and m+20, and zero; call its result r. Then, if the word at object+984
// is nonzero and the signed byte cutMode is zero, calls modelDrawAttachment__FP12_modelHeaderP8_
// textureiT0P11_drawRecordUlii with: the word at m+196, the word at m+204, 0, the word at m+12, r,
// 0xffffffff, -1 and 0. Nothing is returned.
// Inferred: r is the draw record of the body and the second call draws an attachment over it, and
// +984 says the attachment is present (msg_load__10CloudGiantPc clears it). Unknown: the types of
// the last three attachment arguments and what cutMode means. Nothing here ran on the EE.
extern "C" {
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* second);
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* model, void* texture, int a, void* model2, void* record, unsigned long mask, int b, int c);
extern signed char cutMode;
}

#if defined(W10_ACTOR_D_HOST)
extern unsigned char* w10_actor_d_host_base;  // host builds keep 32-bit words as offsets from this base
#endif
namespace {
unsigned char* from_word(unsigned word) {
#if defined(W10_ACTOR_D_HOST)
    return w10_actor_d_host_base + word;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
unsigned char* word_at(const unsigned char* base, unsigned offset) { return from_word(*reinterpret_cast<const unsigned*>(base + offset)); }
}  // namespace

extern "C" void msg_draw__10CloudGiant(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char* m = word_at(object, 36);
    void* record = draw__8CreatureP12_modelHeaderP8_textureT1(self, word_at(m, 12), word_at(m, 20), nullptr);
    if (*reinterpret_cast<int*>(object + 984) != 0 && cutMode == 0) {
        unsigned char* block = word_at(object, 36);
        modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(word_at(block, 196), word_at(block, 204), 0, word_at(block, 12), record, 0xfffffffful, -1, 0);
    }
}
