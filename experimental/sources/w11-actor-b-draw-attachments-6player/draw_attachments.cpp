// Observed drawAttachments__6PlayerP11_drawRecordf at 0x001960f0, 2224 bytes. record is the second argument,
// size the float. R is the player record player + word(+960) * 23984. In order:
//   1. The doubleword at +552 becomes all ones; when charAnimTest is 273 or more it becomes
//      modelGetActiveChunks__FP12_modelHeaderi(word +1332, 1). A flag `hide` is 0. Two locals, model and
//      texture, start null and are NOT reset between the passes below.
//   2. For i = 0..15: the item description d is the word at R+21896+4i (zero skips the pass). The pass is
//      skipped when i = 3 and the description e at R+21960+4k (k = the signed byte R[23572]) is non-zero and
//      the signed byte at +30 of e's first word is 4. With def the first word of d: skipped when the signed
//      byte def[28] is 5, 6, 7, 8 or 14; and, when interfaceOn is zero and bit 28 of levelFlags is set,
//      when def[28] is 0, 2 or 3.
//   3. Finding model and texture, by i (cache = the local image symbol at 0x007cb770 as
//      norrath_player_armor_cache_view_7cb770; element = cache + offset + 88 * word(+960)):
//        i = 4, 10, 9, 11, 12, 13 use offsets 0, 352, 704, 1056, 1408, 1760; i = 2, 0, 1, 3 use 2112, 2464,
//        2816, 3168.
//        i = 4, 9..13 ("A"): when gamePlayersPresent is non-zero and the word at def+48 is non-zero, the
//        model and texture come from the equipment cache; otherwise they are left as they were.
//        i = 0..3 ("B"): for i = 0, 1, 2 nothing is done (model and texture left as they were) when the
//        byte at +964 or the word at +1328 is non-zero. Otherwise: with gamePlayersPresent zero both are set
//        to null; with the word at def+48 zero they are words 8 and 12 of _4item$globalItem at 16 * (the
//        signed halfword at def+52); else from the equipment cache.
//        Cache lookup: getPicName__4itemPciPQ24item11description(buffer, word R+84, d), a zero is written
//        at strlen - 4, then cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(buffer, element,
//        &model, &texture).
//        Any other i (5..8, 14, 15): unless (byte +964 and word +1328 are zero or interfaceOn is non-zero)
//        nothing is done; otherwise getProxyMeshId__4itemPQ24item11description(d) selects words 8 (model)
//        and, from a second call, 12 (texture) of _4item$globalItem at 16 * id.
//   4. The pass ends when model or texture is null. Otherwise, with t = word R+84: if t is not 7 and def[28]
//      is 11 and strstr(word def+0, "circlet") is null, hide = 1; if t is 7: when def[28] is 11 and
//      strstr(word def+48, "cloth1") is null and strstr(word def+0, "circlet") is null, hide = 1; then,
//      when def[28] is 4 and strstr(word def+48, "cloth3body") is non-null, hide = 1; if t is 9 and
//      strstr(word def+0, "chain") is non-null and def[28] is 4, hide = 1.
//   5. setupHalo__4itemPQ24item11descriptionf(d, size). flip = 1 for i = 1, else -1; chunks = all ones,
//      except when def[28] is 3: chunks = modelGetActiveChunks(model, 1 << s) with s from t: 0 -> 1, 1 -> 0,
//      2 -> 9, 3 -> 8, 4 -> 7, 5 -> 6, 6 -> 5, 7 -> 4, 8 -> 3, 9 -> 2, otherwise 0. When flip is not -1 and
//      t is 2, modelSetAttachmentFlip__Fv(). modelDrawAttachment(model, texture, 1, word +1332, record,
//      chunks, flip, 0); when flip is not -1, modelResetOptionalParams__Fv(). Then the doubleword at +552 loses
//      the bits of the doubleword at the start of model.
//   6. After the passes: when hide is zero, modelSetHalo__FP7HaloDefi(0, 0) and, with words +1336 and +1352
//      non-zero, modelDrawAttachment(word +1336, word +1352, 1, word +1332, record, all ones, -1, 0) and
//      the same +552 clearing with the doubleword at the start of the +1336 model. A flag `tint` is 1 when
//      the word at R+21940 is non-zero and either strstr(word of its first word, "leatherhelmet") is non-null
//      or t is 6, else 0. When hide is zero or tint is non-zero, with words +1340 and +1348 non-zero, the
//      same draw with word +1340 / +1348 and clearing. modelResetOptionalParams__Fv().
// Inferred from names only: attachments are armour pieces and helmets. Unknown: what the slots 0..15
// stand for, and the roles of +964, +1328, +1336..+1352 and the byte values in def[28].
// Nothing here ran on the EE.
#include "../w11-actor-b-common/access.hpp"

extern "C" {
extern int charAnimTest;
extern signed char interfaceOn;
extern int levelFlags;
extern int gamePlayersPresent;
extern unsigned char armorCache[] __asm__("norrath_player_armor_cache_view_7cb770");
extern unsigned char player[];
extern unsigned char globalItem[] __asm__("_4item$globalItem");
char* find_text(const char* haystack, const char* needle) __asm__("strstr");
unsigned text_length(const char* text) __asm__("strlen");
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int selected);
void getPicName__4itemPciPQ24item11description(char* buffer, int value, void* description);
void cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(char* name, void* element, void** model, void** texture);
int getProxyMeshId__4itemPQ24item11description(void* description);
void setupHalo__4itemPQ24item11descriptionf(void* description, float size);
void modelSetAttachmentFlip__Fv();
void modelDrawAttachment(void* model, void* texture, int a, void* body, void* record, unsigned long long chunks, int flip, int d)
    __asm__("modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii");
void modelResetOptionalParams__Fv();
void modelSetHalo__FP7HaloDefi(void* table, int count);
}

namespace {
using namespace w11b;
inline unsigned char* maybe(unsigned value) { return value != 0 ? from_word(value) : nullptr; }

inline void cache_lookup(unsigned char* o, unsigned char* record, void* description, unsigned cacheOffset, void** model, void** texture) {
    char name[128];
    getPicName__4itemPciPQ24item11description(name, sword(record, 84), description);
    name[text_length(name) - 4] = 0;
    cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(name, armorCache + cacheOffset + sword(o, 960) * 88, model, texture);
}

inline void clear_attached(unsigned char* o, unsigned char* model) { dword(o, 552) = dword(o, 552) & ~dword(model, 0); }

inline void draw_extra(unsigned char* o, void* record, unsigned modelWord, unsigned textureWord) {
    modelDrawAttachment(from_word(modelWord), from_word(textureWord), 1, from_word(word(o, 1332)), record, ~0ull, -1, 0);
    clear_attached(o, from_word(modelWord));
}
}  // namespace

extern "C" void drawAttachments__6PlayerP11_drawRecordf(void* self, void* drawRecord, float size) {
    unsigned char* o = static_cast<unsigned char*>(self);
    unsigned char* record = player + sword(o, 960) * 23984;
    void* model = nullptr;
    void* texture = nullptr;
    dword(o, 552) = ~0ull;
    if (charAnimTest >= 273) dword(o, 552) = modelGetActiveChunks__FP12_modelHeaderi(from_word(word(o, 1332)), 1);
    int hide = 0;
    for (int i = 0; i < 16; ++i) {
        const unsigned descriptionWord = word(record, 21896 + 4 * i);
        if (descriptionWord == 0) continue;
        unsigned char* description = from_word(descriptionWord);
        const unsigned equippedWord = word(record, 21960 + *reinterpret_cast<signed char*>(record + 23572) * 4);
        if (equippedWord != 0 && i == 3) {
            if (*reinterpret_cast<signed char*>(from_word(word(from_word(equippedWord), 0)) + 30) == 4) continue;
        }
        unsigned char* def = from_word(word(description, 0));
        const int category = *reinterpret_cast<signed char*>(def + 28);
        if (category == 8 || category == 5 || category == 6 || category == 7 || category == 14) continue;
        if (interfaceOn == 0 && (levelFlags & 0x10000000) != 0) {
            if (category == 2 || category == 0 || category == 3) continue;
        }
        switch (i) {
        case 4: case 9: case 10: case 11: case 12: case 13: {
            const unsigned offset = i == 4 ? 0 : i == 10 ? 352 : i == 9 ? 704 : i == 11 ? 1056 : i == 12 ? 1408 : 1760;
            if (gamePlayersPresent != 0 && word(def, 48) != 0) cache_lookup(o, record, description, offset, &model, &texture);
            break;
        }
        case 0: case 1: case 2: case 3: {
            const unsigned offset = i == 2 ? 2112 : i == 0 ? 2464 : i == 1 ? 2816 : 3168;
            if (i != 3 && (*reinterpret_cast<signed char*>(o + 964) != 0 || word(o, 1328) != 0)) break;
            if (gamePlayersPresent == 0) {
                model = nullptr;
                texture = nullptr;
            } else if (word(def, 48) == 0) {
                const int index = half(def, 52);
                model = maybe(word(globalItem, 8 + index * 16));
                texture = maybe(word(globalItem, 12 + index * 16));
            } else {
                cache_lookup(o, record, description, offset, &model, &texture);
            }
            break;
        }
        default: {
            const bool plain = *reinterpret_cast<signed char*>(o + 964) == 0 && word(o, 1328) == 0;
            if (plain || interfaceOn != 0) {
                model = maybe(word(globalItem, 8 + getProxyMeshId__4itemPQ24item11description(description) * 16));
                texture = maybe(word(globalItem, 12 + getProxyMeshId__4itemPQ24item11description(description) * 16));
            }
            break;
        }
        }
        if (model == nullptr) continue;
        if (texture == nullptr) continue;
        const unsigned t = word(record, 84);
        if (t != 7) {
            if (category == 11 && find_text(reinterpret_cast<char*>(from_word(word(def, 0))), "circlet") == nullptr) hide = 1;
        }
        if (word(record, 84) == 7) {
            if (*reinterpret_cast<signed char*>(def + 28) == 11) {
                if (find_text(reinterpret_cast<char*>(from_word(word(def, 48))), "cloth1") == nullptr &&
                    find_text(reinterpret_cast<char*>(from_word(word(def, 0))), "circlet") == nullptr)
                    hide = 1;
            }
            if (*reinterpret_cast<signed char*>(def + 28) == 4) {
                if (find_text(reinterpret_cast<char*>(from_word(word(def, 48))), "cloth3body") != nullptr) hide = 1;
            }
        }
        if (word(record, 84) == 9) {
            if (find_text(reinterpret_cast<char*>(from_word(word(def, 0))), "chain") != nullptr && *reinterpret_cast<signed char*>(def + 28) == 4) hide = 1;
        }
        setupHalo__4itemPQ24item11descriptionf(description, size);
        const int flip = i == 1 ? 1 : -1;
        unsigned long long chunks = ~0ull;
        if (*reinterpret_cast<signed char*>(def + 28) == 3) {
            const unsigned kind = word(record, 84);
            int shift = 0;
            switch (kind) {
            case 0: shift = 1; break;
            case 1: shift = 0; break;
            case 2: shift = 9; break;
            case 3: shift = 8; break;
            case 4: shift = 7; break;
            case 5: shift = 6; break;
            case 6: shift = 5; break;
            case 7: shift = 4; break;
            case 8: shift = 3; break;
            case 9: shift = 2; break;
            default: shift = 0; break;
            }
            chunks = modelGetActiveChunks__FP12_modelHeaderi(model, 1 << shift);
        }
        if (flip != -1 && word(record, 84) == 2) modelSetAttachmentFlip__Fv();
        modelDrawAttachment(model, texture, 1, from_word(word(o, 1332)), drawRecord, chunks, flip, 0);
        if (flip != -1) modelResetOptionalParams__Fv();
        clear_attached(o, static_cast<unsigned char*>(model));
    }
    if (hide == 0) {
        modelSetHalo__FP7HaloDefi(nullptr, 0);
        if (word(o, 1336) != 0 && word(o, 1352) != 0) draw_extra(o, drawRecord, word(o, 1336), word(o, 1352));
    }
    int tint = 0;
    if (word(record, 21940) != 0) {
        if (find_text(reinterpret_cast<char*>(from_word(word(from_word(word(from_word(word(record, 21940)), 0)), 0))), "leatherhelmet") != nullptr) tint = 1;
        else if (word(record, 84) == 6) tint = 1;
    }
    if (hide == 0 || tint != 0) {
        if (word(o, 1340) != 0 && word(o, 1348) != 0) draw_extra(o, drawRecord, word(o, 1340), word(o, 1348));
    }
    modelResetOptionalParams__Fv();
}
