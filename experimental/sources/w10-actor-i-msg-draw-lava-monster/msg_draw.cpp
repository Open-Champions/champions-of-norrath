// Observed msg_draw__11LavaMonster at 0x001eba20, 212 bytes.
// In order: modelSetShadowScale(2.0f); lightSetColorScale with the single at object+968 three times;
// virtual slot 128 (entry at table+128: adjustment halfword, function word at +4) on the adjusted
// object gives the model header; getTexture__11LavaMonster gives the texture; draw__8Creature(object,
// header, texture, 0) gives a draw record. When the word at object+960 is nonzero, slot 128 runs
// again and modelDrawAttachment is called with the resource block's (word at object+36) words +228
// and +236, 0, the slot-128 result, the draw record, -1, -1 and 0. Finally modelResetOptionalParams().
// Inferred: slot 128 is the model getter (getModel__11LavaMonster in this class); the attachment is
// drawn only while the word at +960 is set. Unknown: what +960 and the single at +968 hold.
// Nothing here ran on the EE.
extern "C" {
void modelSetShadowScale__Ff(float scale);
void lightSetColorScale__Ffff(float red, float green, float blue);
void* getTexture__11LavaMonster(void* self);
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* header, void* texture, void* other);
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
    void* header, void* texture, int index, void* header_again, void* record, unsigned long mask, int a, int b);
void modelResetOptionalParams__Fv();
}

namespace {
void* model_of(unsigned char* object) {
    unsigned char* entry = *reinterpret_cast<unsigned char**>(object + 72) + 128;
    return reinterpret_cast<void* (*)(void*)>(*reinterpret_cast<unsigned*>(entry + 4))(object + *reinterpret_cast<short*>(entry));
}
}

extern "C" void msg_draw__11LavaMonster(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    modelSetShadowScale__Ff(2.0f);
    const float scale = *reinterpret_cast<float*>(object + 968);
    lightSetColorScale__Ffff(scale, scale, scale);
    void* header = model_of(object);
    void* record = draw__8CreatureP12_modelHeaderP8_textureT1(object, header, getTexture__11LavaMonster(object), nullptr);
    if (*reinterpret_cast<unsigned*>(object + 960) != 0u) {
        void* again = model_of(object);
        unsigned char* block = *reinterpret_cast<unsigned char**>(object + 36);
        modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
            *reinterpret_cast<void**>(block + 228), *reinterpret_cast<void**>(block + 236), 0, again, record, 0xffffffffu, -1, 0);
    }
    modelResetOptionalParams__Fv();
}
