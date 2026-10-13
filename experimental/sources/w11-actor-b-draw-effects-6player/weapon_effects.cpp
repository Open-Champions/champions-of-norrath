// Observed playerDrawWeaponEffects__6Playerf at 0x001aaad8, 1208 bytes. The float argument (rate) is passed on
// unchanged. R is the player record player + word(+960) * 23984. In order:
//   1. When netNetworkMode is non-zero and netPlayerIsLocal__Fi(word +960) is non-zero: return at once if
//      bit 0x20000 of the word at +12 is clear, and return at once if netPredictionFastForward is non-zero.
//   2. For slot = 0 and 1: the item description is the word at R+21960+4*k, k = the signed byte R[23572],
//      for slot 0, and the word at R+21968 for slot 1; a zero description skips the slot. Otherwise
//      c = inventoryItemCheckSlot__FiPQ24item11description(word +960, description) selects one of three
//      cases (anything else does nothing):
//        c = 0: cache = cache+2464, weapon state = R+23584, second integer = 0;
//        c = 1: cache = cache+2816, weapon state = R+23648, second integer = 1;
//        c = 2: cache = cache+2112, weapon state = R+23584, second integer = 0.
//      (cache is the local image symbol `cache` at 0x007cb770, imported under the name norrath_player_armor_cache_view_7cb770
//      like the other sources that use it; the element is base + offset + 88 * word(+960).)
//      The model and texture are found first. When gamePlayersPresent is zero both are null. Otherwise, if
//      the word at +48 of the description's first word is non-zero, getPicName__4itemPciPQ24item11description(
//      buffer, word R+84, description) fills a name buffer, the last four characters are cut off by writing
//      a zero at strlen - 4, and cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(buffer, element,
//      &model, &texture) yields both; if that word is zero, the signed halfword h at +52 of the first word
//      selects words 8+16h (model) and 12+16h (texture) of _4item$globalItem.
//      Then fxRunWeapon (see the prototype) is called with: the description, the model, the weapon state, a
//      copy of the 12 bytes at +16, the signed halfword +56, the word at +1332, the animation state at +288,
//      the rate, the flag (word +1400 is 1 or 6), the second integer, 1.0 and a null matrix.
// Inferred from names only: the two slots are the two hand items; the cache holds equipment models.
// Unknown: what the three c values stand for and what the byte R[23572] selects. Nothing here ran on the EE.
#include "../w11-actor-b-common/access.hpp"

extern "C" {
extern signed char netNetworkMode;
extern signed char netPredictionFastForward;
extern int gamePlayersPresent;
extern unsigned char armorCache[] __asm__("norrath_player_armor_cache_view_7cb770");
extern unsigned char player[];
extern unsigned char globalItem[] __asm__("_4item$globalItem");
int netPlayerIsLocal__Fi(int index);
int inventoryItemCheckSlot__FiPQ24item11description(int player, void* description);
void getPicName__4itemPciPQ24item11description(char* buffer, int value, void* description);
unsigned text_length(const char* text) __asm__("strlen");
void cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(char* name, void* element, void** model, void** texture);
void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* description, void* model, void* state, Point3 position, short angle, void* model_again, void* animation,
    float a, int flag, int zero, float b, void* matrix);
}

namespace {
inline void run_case(unsigned char* o, unsigned char* record, void* description, float rate, unsigned cacheOffset,
                     unsigned stateOffset, int second) {
    using namespace w11b;
    void* model;
    void* texture;
    if (gamePlayersPresent == 0) {
        model = nullptr;
        texture = nullptr;
    } else {
        unsigned char* definition = from_word(word(static_cast<unsigned char*>(description), 0));
        if (word(definition, 48) != 0) {
            char name[128];
            getPicName__4itemPciPQ24item11description(name, sword(record, 84), description);
            name[text_length(name) - 4] = 0;
            cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture(name, armorCache + cacheOffset + sword(o, 960) * 88, &model, &texture);
        } else {
            const int index = half(definition, 52);
            model = from_word(word(globalItem, 8 + index * 16));
            texture = from_word(word(globalItem, 12 + index * 16));
        }
    }
    const int type = sword(o, 1400);
    fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
        description, model, record + stateOffset, *reinterpret_cast<Point3*>(o + 16), half(o, 56), from_word(word(o, 1332)), o + 288,
        rate, type == 1 || type == 6 ? 1 : 0, second, 1.0f, nullptr);
}
}  // namespace

extern "C" void playerDrawWeaponEffects__6Playerf(void* self, float rate) {
    using namespace w11b;
    unsigned char* o = static_cast<unsigned char*>(self);
    if (netNetworkMode != 0 && netPlayerIsLocal__Fi(sword(o, 960)) != 0) {
        if ((word(o, 12) & 0x20000u) == 0) return;
        if (netPredictionFastForward != 0) return;
    }
    unsigned char* record = player + sword(o, 960) * 23984;
    for (int slot = 0; slot < 2; ++slot) {
        unsigned char* description = from_word(slot == 0 ? word(record + *reinterpret_cast<signed char*>(record + 23572) * 4, 21960) : word(record, 21968));
        if (to_word(description) == 0) continue;
        const int c = inventoryItemCheckSlot__FiPQ24item11description(sword(o, 960), description);
        if (c == 0) run_case(o, record, description, rate, 2464, 23584, 0);
        if (c == 1) run_case(o, record, description, rate, 2816, 23648, 1);
        if (c == 2) run_case(o, record, description, rate, 2112, 23584, 0);
    }
}
