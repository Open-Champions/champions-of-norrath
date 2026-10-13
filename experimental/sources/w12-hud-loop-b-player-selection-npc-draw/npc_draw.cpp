// Observed playerSelectionNPCDraw__FP3NPCP12_modelHeaderP11_drawRecord at 0x0027a0b0, 836 bytes: three
// arguments (the NPC object, a model header, a draw record), no result.
//  - The target index i is found as in playerSelectionGetNPCActiveChunks: the NPC "name" tag against
//    the first string of each 636-byte record of targets, 10 when none matches.
//  - hairStyle, hairColor and beard are signed bytes. With a "pnum" tag the three findTagInt("pnum")
//    calls select playerSelectInfo records (41 bytes each) whose bytes +33, +34 and +36 are read in
//    that order (each call feeds the next byte). Without a "pnum" tag they are hairStyleSetting[i],
//    hairColorSetting[i] and beardSetting[i].
//  - Up to six attachments are drawn with modelDrawAttachment(attachment, texture, 1, model, record,
//    -1, -1, 0), in this order, each only when its attachment word is nonzero. Offsets are into
//    record i of targets (words):
//        attachment 104                          texture 632
//        attachment 28 + 4 * hairStyle           texture 108 + 4 * (8 * hairStyle + hairColor)
//          (only when hairStyle is not -1)
//        attachment 60 + 4 * beard               texture 364 + 4 * (8 * beard + hairColor)
//          (only when beard is not -1)
//        attachment 92                           texture 620
//        attachment 100                          texture 628
//        attachment 96                           texture 624
// Inferred: the attachment words are model headers and the texture words their textures; the stride
// of 8 per style gives eight colours per style.
// Unknown: the meaning of the final three modelDrawAttachment arguments.
extern "C" {
extern signed char playerSelectInfo[];
extern signed char hairStyleSetting[];
extern signed char hairColorSetting[];
extern signed char beardSetting[];
extern unsigned char targets[];
char* findTagString__10GameObjectPci(void* self, const char* tag, int fallback);
int findTagInt__10GameObjectPci(void* self, const char* tag, int fallback);
int strcmp(const char* a, const char* b);
void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* attachment, void* texture, int one, void* model,
                                                                          void* record, unsigned long mask, int a, int b);
}

#ifdef W12_HUD_HOST
__UINTPTR_TYPE__ hostWordAddress(unsigned word);
#endif

namespace {
inline unsigned wordAt(const unsigned char* base, int offset) {
    unsigned word;
    __builtin_memcpy(&word, base + offset, 4);
    return word;
}
inline void* asPointer(unsigned word) { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(static_cast<int>(word))); }
inline const char* nameOf(int i) {
    const unsigned word = wordAt(targets, i * 636);
#ifdef W12_HUD_HOST
    return reinterpret_cast<const char*>(hostWordAddress(word));
#else
    return reinterpret_cast<const char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
inline void drawPart(const unsigned char* record, int attachmentOffset, int textureOffset, void* model, void* drawRecord) {
    const unsigned attachment = wordAt(record, attachmentOffset);
    if (attachment == 0) return;
    modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(asPointer(attachment), asPointer(wordAt(record, textureOffset)), 1,
                                                                         model, drawRecord, static_cast<unsigned long>(-1), -1, 0);
}
}

extern "C" void playerSelectionNPCDraw__FP3NPCP12_modelHeaderP11_drawRecord(void* npc, void* model, void* drawRecord) {
    const char* name = findTagString__10GameObjectPci(npc, "name", 0);
    int i = 0;
    while (i < 10 && strcmp(nameOf(i), name) != 0) ++i;
    int hairStyle, hairColor, beard;
    if (findTagString__10GameObjectPci(npc, "pnum", 0) != nullptr) {
        hairStyle = playerSelectInfo[findTagInt__10GameObjectPci(npc, "pnum", 0) * 41 + 33];
        hairColor = playerSelectInfo[findTagInt__10GameObjectPci(npc, "pnum", 0) * 41 + 34];
        beard = playerSelectInfo[findTagInt__10GameObjectPci(npc, "pnum", 0) * 41 + 36];
    } else {
        hairStyle = hairStyleSetting[i];
        hairColor = hairColorSetting[i];
        beard = beardSetting[i];
    }
    const unsigned char* record = targets + i * 636;
    drawPart(record, 104, 632, model, drawRecord);
    if (hairStyle != -1) drawPart(record, 28 + hairStyle * 4, 108 + (hairStyle * 8 + hairColor) * 4, model, drawRecord);
    if (beard != -1) drawPart(record, 60 + beard * 4, 364 + (beard * 8 + hairColor) * 4, model, drawRecord);
    drawPart(record, 92, 620, model, drawRecord);
    drawPart(record, 100, 628, model, drawRecord);
    drawPart(record, 96, 624, model, drawRecord);
}
