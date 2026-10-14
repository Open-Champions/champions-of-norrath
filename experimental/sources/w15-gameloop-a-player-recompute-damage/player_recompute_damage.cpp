#include "../w9-animation-object-c-common/mem.hpp"
// Observed playerRecomputeDamage__6PlayerPQ24item11descriptioni at 0x001a6080, 1756 bytes, 112-byte frame. Arguments: the
// Player (word at +960 is its index into `player`, the array of 23984-byte records), an item::description pointer (may
// be null) and an int that is passed on. With P = player + 23984 * index it rewrites three int fields of P: +212 and +216
// (a damage range, low and high) and +208 (a third damage value).
//  1. s0 = P.int(+144) / 4 and s4 = P.int(+152) / 4 (division toward zero). P.+212 = 1, P.+216 = 2, P.+208 = 20 *
//     signed byte at P+88. With a null description: P.+212 = s0 + 1, P.+216 = s0 + 2, P.+208 = 20 * P+88 + s0, then step 6.
//  2. Otherwise getDamageRangeAll(description, &P.+212, &P.+216, flag) and getAttackBonus(description) (added to +208).
//     With the signed byte at description->first word + 30 equal to 4 or 6, s4 is added to all three fields, otherwise
//     s0 is.
//  3. Two multipliers start at 1.0: f21 = rank60 * 0.05 + 1.15 when playerGetSkillRank(index, 60) is nonzero, and
//     f20 = rank61 * 0.05 + 1.15 when playerGetSkillRank(index, 61) is nonzero.
//  4. With the byte at +30 equal to 4: scale = SkillArchery::getDamageScale(skills[48], playerGetSkillRank(index, 48));
//     P.+212 and P.+216 each gain the rounded (half away from zero) product with scale.
//  5. When the word P+21968 is nonzero and the description's word at +32 is 1 (or 8), the skill tree of the player's class
//     (word at P+80, gSkillTree + 40 + 3592 * class, 56-byte entries, 64 of them) is searched for the first entry whose
//     64-bit first word is 0x40000 (only for index below 5); its rank is the signed byte P+243+entry (0 if negative or not
//     found); f21 (or f20) is multiplied by (2 * rank + 60) * 0.01. Then, by that word at +32: 1 multiplies +212 and +216
//     (as floats, plus 0.5, truncated) by f21; 8 multiplies them by f20.
//  6. Aura: over the nmPlayersAlive entries of playersAlive, the creature at player(entry)+21872 is asked
//     checkIfSpellIsRunning with the SpellAncestralCall type_info; for a running spell, posInAura(spell, self + 16,
//     &strength, 0) or, failing that, the spell's word at +12 equal to the Player, makes strength a candidate; the largest
//     candidate is kept (0 if none). When nonzero it multiplies +212, +216 and +208 (rounded half away from zero).
//  7. A nonzero word at Player+904 multiplies all three by 1.5 (truncated) and a nonzero word at +912 multiplies all three
//     by 1.2 (the word 0x3f999999, truncated).
// Nothing is returned.
// Inferred: this is the attack damage recomputation for a player: base from stats, weapon range, archery and two skill
// multipliers, an aura bonus and two status multipliers. The roles of the +904 and +912 words and of the P+21968 flag are
// not established. The skill numbers 48, 60 and 61 are used as given.
// Unknown: the types of the fields beyond what is read, the meaning of the 0x40000 marker, and the second argument of
// getDamageRangeAll (the description's class is `item`, named only in the symbol).
extern "C" {
extern unsigned char player[];
extern unsigned char gSkillTree[];
extern unsigned char skills[];
extern int playersAlive[];
extern int nmPlayersAlive;

void getDamageRangeAll__4itemPQ24item11descriptionPiT2i(const unsigned char* description, int* low, int* high, int flag);
int getAttackBonus__4itemPQ24item11description(const unsigned char* description);
int playerGetSkillRank__Fii(int index, int skill);
float getDamageScale__12SkillArcheryi(void* archery, int rank);
void* __tf18SpellAncestralCall();
void* checkIfSpellIsRunning__8CreatureRC9type_info(void* creature, void* type);
int posInAura__18SpellAncestralCallP6Point3PfT2(void* spell, void* position, float* strength, float* second);
}

namespace {
constexpr float bits(unsigned word) { return __builtin_bit_cast(float, word); }
inline int roundAway(float value) { return static_cast<int>(value >= 0.0f ? value + 0.5f : value - 0.5f); }

inline int rankInTree(unsigned char* record, int index) {
    using namespace norrath_aoc;
    if (static_cast<unsigned>(index) >= 5u) return 0;
    const unsigned char* node = gSkillTree + 40 + 3592 * static_cast<int>(load_word(record + 80));
    for (int entry = 0; entry < 64; ++entry, node += 56) {
        if (*reinterpret_cast<const unsigned long long*>(node) == 0x40000ull) {
            const int rank = load_signed_byte(record + 243 + entry);
            return rank < 0 ? 0 : rank;
        }
    }
    return 0;
}
}

extern "C" void playerRecomputeDamage__6PlayerPQ24item11descriptioni(unsigned char* self, unsigned char* description, int flag) {
    using namespace norrath_aoc;
    const int index = static_cast<int>(load_word(self + 960));
    unsigned char* const record = player + 23984 * index;
    int* const low = reinterpret_cast<int*>(record + 212);
    int* const high = reinterpret_cast<int*>(record + 216);
    int* const third = reinterpret_cast<int*>(record + 208);
    const int quarterA = *reinterpret_cast<int*>(record + 144) / 4;
    const int quarterB = *reinterpret_cast<int*>(record + 152) / 4;
    *low = 1;
    *high = 2;
    *third = 20 * load_signed_byte(record + 88);
    if (description == nullptr) {
        *low = quarterA + 1;
        *high = quarterA + 2;
        *third = 20 * load_signed_byte(record + 88) + quarterA;
    } else {
        getDamageRangeAll__4itemPQ24item11descriptionPiT2i(description, low, high, flag);
        *third += getAttackBonus__4itemPQ24item11description(description);
        const unsigned char* const base = load_pointer(description);
        if (load_signed_byte(base + 30) == 4 || load_signed_byte(base + 30) == 6) {
            *low += quarterB;
            *high += quarterB;
            *third += quarterB;
        } else {
            *low += quarterA;
            *high += quarterA;
            *third += quarterA;
        }
        float first = 1.0f;
        float second = 1.0f;
        if (playerGetSkillRank__Fii(index, 60) != 0)
            first = static_cast<float>(playerGetSkillRank__Fii(index, 60)) * bits(0x3d4cccccu) + bits(0x3f933333u);
        if (playerGetSkillRank__Fii(index, 61) != 0)
            second = static_cast<float>(playerGetSkillRank__Fii(index, 61)) * bits(0x3d4cccccu) + bits(0x3f933333u);
        const unsigned char* kind = load_pointer(description);
        if (load_signed_byte(kind + 30) == 4) {
            const float scale = getDamageScale__12SkillArcheryi(
                load_pointer(skills + 192), playerGetSkillRank__Fii(index, 48));
            const int a = *low;
            *low = a + roundAway(static_cast<float>(a) * scale);
            const int b = *high;
            *high = b + roundAway(static_cast<float>(b) * scale);
            kind = load_pointer(description);
        }
        if (load_word(record + 21968) != 0) {
            const int type = static_cast<int>(load_word(kind + 32));
            if (type == 1 || type == 8) {
                const float rank = static_cast<float>(rankInTree(record, index));
                (type == 1 ? first : second) *= (rank + rank + 60.0f) * bits(0x3c23d70au);
            }
        }
        const int type = static_cast<int>(load_word(kind + 32));
        if (type == 1) {
            *low = static_cast<int>(static_cast<float>(*low) * first + 0.5f);
            *high = static_cast<int>(static_cast<float>(*high) * first + 0.5f);
        } else if (type == 8) {
            *low = static_cast<int>(static_cast<float>(*low) * second + 0.5f);
            *high = static_cast<int>(static_cast<float>(*high) * second + 0.5f);
        }
    }
    float aura = 0.0f;
    for (int i = 0; i < nmPlayersAlive; ++i) {
        void* const type = __tf18SpellAncestralCall();
        unsigned char* const spell = static_cast<unsigned char*>(checkIfSpellIsRunning__8CreatureRC9type_info(
            load_pointer(player + 23984 * playersAlive[i] + 21872), type));
        if (spell == nullptr) continue;
        float strength;
        const int inside = posInAura__18SpellAncestralCallP6Point3PfT2(spell, self + 16, &strength, nullptr);
        if (inside != 0 || load_pointer(spell + 12) == self) {
            if (aura < strength) aura = strength;
        }
    }
    if (aura != 0.0f) {
        *low = roundAway(static_cast<float>(*low) * aura);
        *high = roundAway(static_cast<float>(*high) * aura);
        *third = roundAway(static_cast<float>(*third) * aura);
    }
    if (load_word(self + 904) != 0) {
        const float a = static_cast<float>(*low) * bits(0x3fc00000u);
        const float b = static_cast<float>(*high) * bits(0x3fc00000u);
        const float c = static_cast<float>(*third) * bits(0x3fc00000u);
        *low = static_cast<int>(a);
        *high = static_cast<int>(b);
        *third = static_cast<int>(c);
    }
    if (load_word(self + 912) != 0) {
        const float a = static_cast<float>(*low) * bits(0x3f999999u);
        const float b = static_cast<float>(*high) * bits(0x3f999999u);
        const float c = static_cast<float>(*third) * bits(0x3f999999u);
        *low = static_cast<int>(a);
        *high = static_cast<int>(b);
        *third = static_cast<int>(c);
    }
}
